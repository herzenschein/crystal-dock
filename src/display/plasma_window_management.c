/* Generated by wayland-scanner 1.22.0 */

/*
 * SPDX-FileCopyrightText: 2013-2014 Pier Luigi Fiorini
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"

#ifndef __has_attribute
# define __has_attribute(x) 0  /* Compatibility with non-clang compilers. */
#endif

#if (__has_attribute(visibility) || defined(__GNUC__) && __GNUC__ >= 4)
#define WL_PRIVATE __attribute__ ((visibility("hidden")))
#else
#define WL_PRIVATE
#endif

extern const struct wl_interface org_kde_plasma_activation_interface;
extern const struct wl_interface org_kde_plasma_window_interface;
extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_surface_interface;

static const struct wl_interface *plasma_window_management_types[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	&org_kde_plasma_window_interface,
	NULL,
	&org_kde_plasma_window_interface,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
	NULL,
	&wl_surface_interface,
	&wl_output_interface,
	&org_kde_plasma_window_interface,
	&org_kde_plasma_activation_interface,
};

static const struct wl_message org_kde_plasma_window_management_requests[] = {
	{ "show_desktop", "u", plasma_window_management_types + 0 },
	{ "get_window", "nu", plasma_window_management_types + 4 },
	{ "get_window_by_uuid", "12ns", plasma_window_management_types + 6 },
};

static const struct wl_message org_kde_plasma_window_management_events[] = {
	{ "show_desktop_changed", "u", plasma_window_management_types + 0 },
	{ "window", "u", plasma_window_management_types + 0 },
	{ "stacking_order_changed", "11a", plasma_window_management_types + 0 },
	{ "stacking_order_uuid_changed", "12s", plasma_window_management_types + 0 },
	{ "window_with_uuid", "13us", plasma_window_management_types + 0 },
};

WL_PRIVATE const struct wl_interface org_kde_plasma_window_management_interface = {
	"org_kde_plasma_window_management", 16,
	3, org_kde_plasma_window_management_requests,
	5, org_kde_plasma_window_management_events,
};

static const struct wl_message org_kde_plasma_window_requests[] = {
	{ "set_state", "uu", plasma_window_management_types + 0 },
	{ "set_virtual_desktop", "u", plasma_window_management_types + 0 },
	{ "set_minimized_geometry", "ouuuu", plasma_window_management_types + 8 },
	{ "unset_minimized_geometry", "o", plasma_window_management_types + 13 },
	{ "close", "", plasma_window_management_types + 0 },
	{ "request_move", "3", plasma_window_management_types + 0 },
	{ "request_resize", "3", plasma_window_management_types + 0 },
	{ "destroy", "4", plasma_window_management_types + 0 },
	{ "get_icon", "7h", plasma_window_management_types + 0 },
	{ "request_enter_virtual_desktop", "8s", plasma_window_management_types + 0 },
	{ "request_enter_new_virtual_desktop", "8", plasma_window_management_types + 0 },
	{ "request_leave_virtual_desktop", "8s", plasma_window_management_types + 0 },
	{ "request_enter_activity", "14s", plasma_window_management_types + 0 },
	{ "request_leave_activity", "14s", plasma_window_management_types + 0 },
	{ "send_to_output", "15o", plasma_window_management_types + 14 },
};

static const struct wl_message org_kde_plasma_window_events[] = {
	{ "title_changed", "s", plasma_window_management_types + 0 },
	{ "app_id_changed", "s", plasma_window_management_types + 0 },
	{ "state_changed", "u", plasma_window_management_types + 0 },
	{ "virtual_desktop_changed", "i", plasma_window_management_types + 0 },
	{ "themed_icon_name_changed", "s", plasma_window_management_types + 0 },
	{ "unmapped", "", plasma_window_management_types + 0 },
	{ "initial_state", "4", plasma_window_management_types + 0 },
	{ "parent_window", "5?o", plasma_window_management_types + 15 },
	{ "geometry", "6iiuu", plasma_window_management_types + 0 },
	{ "icon_changed", "7", plasma_window_management_types + 0 },
	{ "pid_changed", "u", plasma_window_management_types + 0 },
	{ "virtual_desktop_entered", "8s", plasma_window_management_types + 0 },
	{ "virtual_desktop_left", "8s", plasma_window_management_types + 0 },
	{ "application_menu", "10ss", plasma_window_management_types + 0 },
	{ "activity_entered", "14s", plasma_window_management_types + 0 },
	{ "activity_left", "14s", plasma_window_management_types + 0 },
	{ "resource_name_changed", "16s", plasma_window_management_types + 0 },
};

WL_PRIVATE const struct wl_interface org_kde_plasma_window_interface = {
	"org_kde_plasma_window", 16,
	15, org_kde_plasma_window_requests,
	17, org_kde_plasma_window_events,
};

static const struct wl_message org_kde_plasma_activation_feedback_requests[] = {
	{ "destroy", "", plasma_window_management_types + 0 },
};

static const struct wl_message org_kde_plasma_activation_feedback_events[] = {
	{ "activation", "n", plasma_window_management_types + 16 },
};

WL_PRIVATE const struct wl_interface org_kde_plasma_activation_feedback_interface = {
	"org_kde_plasma_activation_feedback", 1,
	1, org_kde_plasma_activation_feedback_requests,
	1, org_kde_plasma_activation_feedback_events,
};

static const struct wl_message org_kde_plasma_activation_requests[] = {
	{ "destroy", "", plasma_window_management_types + 0 },
};

static const struct wl_message org_kde_plasma_activation_events[] = {
	{ "app_id", "s", plasma_window_management_types + 0 },
	{ "finished", "", plasma_window_management_types + 0 },
};

WL_PRIVATE const struct wl_interface org_kde_plasma_activation_interface = {
	"org_kde_plasma_activation", 1,
	1, org_kde_plasma_activation_requests,
	2, org_kde_plasma_activation_events,
};

