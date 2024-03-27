/*
 * This file is part of Crystal Dock.
 * Copyright (C) 2022 Viet Dang (dangvd@gmail.com)
 *
 * Crystal Dock is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Crystal Dock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Crystal Dock.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "program.h"

#include <QGuiApplication>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QTimer>

#include "display/window_system.h"

#include "dock_panel.h"
#include <utils/command_utils.h>
#include <utils/draw_utils.h>

namespace crystaldock {

Program::Program(DockPanel* parent, MultiDockModel* model, const QString& appId,
                 const QString& label, Qt::Orientation orientation, const QString& iconName,
                 int minSize, int maxSize, const QString& command, bool isAppMenuEntry,
                 bool pinned)
    : IconBasedDockItem(parent, model, label, orientation, iconName, minSize, maxSize),
      appId_(appId),
      command_(command),
      isAppMenuEntry_(isAppMenuEntry),
      pinned_(pinned),
      demandsAttention_(false),
      attentionStrong_(false) {
  init();
}

Program::Program(DockPanel* parent, MultiDockModel* model, const QString& appId,
                 const QString& label, Qt::Orientation orientation, const QPixmap& icon,
                 int minSize, int maxSize)
    : IconBasedDockItem(parent, model, label, orientation, icon, minSize, maxSize),
      appId_(appId),
      command_(""),
      isAppMenuEntry_(false),
      pinned_(false),
      demandsAttention_(false),
      attentionStrong_(false) {
  init();
}

void Program::init() {
  createMenu();

  animationTimer_.setInterval(500);
  connect(&animationTimer_, &QTimer::timeout, this, [this]() {
    attentionStrong_ = !attentionStrong_;
    parent_->update();
  });
}

void Program::draw(QPainter *painter) const {
  if ((!tasks_.empty() && active()) || attentionStrong_) {
    drawHighlightedIcon(model_->backgroundColor(), left_, top_, getWidth(), getHeight(),
                        minSize_ / 7, size_ / 4, painter);
  } else if (!tasks_.empty()) {
    int r = 4;
    int cy = top_ + getHeight() + std::round(minSize_ / 9.0);
    fillCircle(left_ + getWidth() / 2 - r, cy - r, 2 * r, 2 * r,
               model_->backgroundColor().darker(30), painter);
    r = 3;
    fillCircle(left_ + getWidth() / 2 - r, cy - r, 2 * r, 2 * r,
               model_->backgroundColor().lighter(300), painter);
    r = 2;
    fillCircle(left_ + getWidth() / 2 - r, cy - r, 2 * r, 2 * r,
               model_->backgroundColor().lighter(500), painter);
  }
  IconBasedDockItem::draw(painter);
}

void Program::mousePressEvent(QMouseEvent* e) {
  if (e->button() == Qt::LeftButton) { // Run the application.
    if (command_ == kShowDesktopCommand) {
      WindowSystem::setShowingDesktop(!WindowSystem::showingDesktop());
    } else if (isCommandLockScreen(command_)) {
      parent_->leaveEvent(nullptr);
      QTimer::singleShot(500, []() {
        lockScreen();
      });
    } else {
      if (tasks_.empty()) {
        launch();
      } else {
        const auto mod = QGuiApplication::keyboardModifiers();
        if (mod & Qt::ShiftModifier) {
          launch();
        } else if (tasks_.size() == 1) {
          WindowSystem::activateOrMinimizeWindow(tasks_[0].uuid);
        } else {
          const auto activeTask = getActiveTask();
          if (activeTask >= 0) {
            // Cycles through tasks.
            auto nextTask = (activeTask < static_cast<int>(tasks_.size() - 1)) ?
                (activeTask + 1) : 0;
            WindowSystem::activateWindow(tasks_[nextTask].uuid);
          } else {
            for (unsigned i = 0; i < tasks_.size(); ++i) {
              WindowSystem::activateWindow(tasks_[i].uuid);
            }
          }
        }
      }
    }
  } else if (e->button() == Qt::RightButton) {
    menu_.popup(e->globalPosition().toPoint());
  }
}

QString Program::getLabel() const {
  const unsigned taskCount = tasks_.size();
  return (taskCount > 1) ?
      label_ + " (" + QString::number(tasks_.size()) + " windows)" :
      label_;
}

bool Program::addTask(const WindowInfo* task) {
  if (task->appId == appId_.toStdString()) {
    tasks_.push_back(ProgramTask(task->uuid, QString::fromStdString(task->title),
                                 task->demandsAttention));
    if (task->demandsAttention) {
      setDemandsAttention(true);
    }
    return true;
  }
  return false;
}

bool Program::updateTask(const WindowInfo* task) {
  if (task->appId != appId_.toStdString()) {
    return false;
  }

  for (auto& existingTask : tasks_) {
    if (existingTask.uuid == task->uuid) {
      existingTask.demandsAttention = task->demandsAttention;
      updateDemandsAttention();
      return true;
    }
  }

  return false;
}

bool Program::removeTask(std::string_view uuid) {
  for (int i = 0; i < static_cast<int>(tasks_.size()); ++i) {
    if (tasks_[i].uuid == uuid) {
      tasks_.erase(tasks_.begin() + i);
      return true;
    }
  }
  return false;
}

bool Program::hasTask(std::string_view uuid) {
  for (const auto& task : tasks_) {
    if (task.uuid == uuid) {
      return true;
    }
  }
  return false;
}

bool Program::beforeTask(const QString& program) {
  return pinned_ || label_ < program;
}

void Program::launch() {
  launch(command_);
  parent_->showWaitCursor();
  parent_->update();
}

void Program::pinUnpin() {
  pinned_ = !pinned_;
  if (pinned_) {
    model_->addLauncher(parent_->dockId(), LauncherConfig(appId_, label_, iconName_, command_));
  } else {  // !pinned
    model_->removeLauncher(parent_->dockId(), command_);
    if (shouldBeRemoved()) {
      parent_->delayedRefresh();
    }
  }
}

void Program::launch(const QString& command) {
  QStringList list = QProcess::splitCommand(command);
  if (!QProcess::startDetached(list.at(0), list.mid(1))) {
    QMessageBox warning(QMessageBox::Warning, "Error",
                        QString("Could not run command: ") + command,
                        QMessageBox::Ok, nullptr, Qt::Tool);
    warning.exec();
  }
}

void Program::createMenu() {
  menu_.addSection(QIcon::fromTheme(iconName_), label_);

  if (isAppMenuEntry_ || pinned_) {
    pinAction_ = menu_.addAction(
        QString("Pinned"), this,
        [this] {
          pinUnpin();
        });
    pinAction_->setCheckable(true);
    pinAction_->setChecked(pinned_);    
  }

  if (isAppMenuEntry_ || !pinned_) {
    menu_.addAction(QIcon::fromTheme("list-add"), QString("&New Window"), this,
                    [this] { launch(); });
  }

  menu_.addSeparator();
  menu_.addAction(QIcon::fromTheme("configure"), QString("Edit &Launchers"), parent_,
                  [this] { parent_->showEditLaunchersDialog(); });

  if (model_->showTaskManager(parent_->dockId())) {
    menu_.addAction(QIcon::fromTheme("configure"),
                    QString("Task Manager &Settings"),
                    parent_,
                    [this] { parent_->showTaskManagerSettingsDialog(); });
  }

  menu_.addSeparator();
  parent_->addPanelSettings(&menu_);
}

void Program::setDemandsAttention(bool value) {
  if (demandsAttention_ == value) {
    return;
  }

  demandsAttention_ = value;
  if (demandsAttention_) {
    animationTimer_.start();
  } else if (animationTimer_.isActive()) {
    animationTimer_.stop();
  }
}

void Program::updateDemandsAttention() {
  for (const auto& task : tasks_) {
    if (task.demandsAttention) {
      setDemandsAttention(true);
      return;
    }
  }
  setDemandsAttention(false);
}

}  // namespace crystaldock
