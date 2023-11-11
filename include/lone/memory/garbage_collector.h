/* SPDX-License-Identifier: AGPL-3.0-or-later */

#ifndef LONE_MEMORY_GARBAGE_COLLECTOR_HEADER
#define LONE_MEMORY_GARBAGE_COLLECTOR_HEADER

#include <lone/types.h>

struct lone_value *lone_allocate_from_heap(struct lone_lisp *lone);
void lone_garbage_collector(struct lone_lisp *lone);

#endif /* LONE_MEMORY_GARBAGE_COLLECTOR_HEADER */
