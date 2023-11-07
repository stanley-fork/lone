#ifndef LONE_STRUCT_LIST_HEADER
#define LONE_STRUCT_LIST_HEADER

#include <lone/types.h>

struct lone_list {
	struct lone_value *first;
	struct lone_value *rest;
};

#endif /* LONE_STRUCT_LIST_HEADER */
