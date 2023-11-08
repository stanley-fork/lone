#ifndef LONE_TYPES_HEADER
#define LONE_TYPES_HEADER

#include <stdbool.h>

#include <linux/types.h>

typedef __kernel_size_t size_t;
typedef __kernel_ssize_t ssize_t;

struct lone_lisp;
struct lone_value;
struct lone_memory;

typedef bool (*lone_predicate)(struct lone_value *);
typedef bool (*lone_comparator)(struct lone_value *, struct lone_value *);

#endif /* LONE_TYPES_HEADER */
