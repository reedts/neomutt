/**
 * @file
 * Convenience wrapper for the debug headers
 *
 * @authors
 * Copyright (C) 2019 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @page debug DEBUG: Debugging tools
 *
 * Debugging tools
 *
 * | File                | Description                |
 * | :------------------ | :------------------------- |
 * | debug/backtrace.c   | @subpage debug_backtrace   |
 * | debug/parse_test.c  | @subpage debug_parse       |
 */

#ifndef MUTT_DEBUG_LIB_H
#define MUTT_DEBUG_LIB_H

#include "config.h"
#include "core/lib.h"

#ifdef HAVE_LIBUNWIND
void show_backtrace(void);
#endif

// Notify
int         debug_notify_observer(struct NotifyCallback *nc);
const char *get_config_type      (int id);
const char *get_context          (int id);
const char *get_event_type       (enum NotifyType type);
const char *get_global_event     (int id);
const char *get_mailbox_event    (int id);
const char *get_mailbox_type     (enum MailboxType type);
void        notify_dump_account  (struct NotifyCallback *nc);
void        notify_dump_color    (struct NotifyCallback *nc);
void        notify_dump_command  (struct NotifyCallback *nc);
void        notify_dump_config   (struct NotifyCallback *nc);
void        notify_dump_context  (struct NotifyCallback *nc);
void        notify_dump_email    (struct NotifyCallback *nc);
void        notify_dump_global   (struct NotifyCallback *nc);
void        notify_dump_mailbox  (struct NotifyCallback *nc);

// Parse Set
void test_parse_set(void);

#endif /* MUTT_DEBUG_LIB_H */
