#ifndef __HEAP_H
#define __HEAP_H

#include <stdint.h>
#include <stdbool.h>

typedef void* heap_handle_t;

typedef void* heap_value_handle_t;

// compare: relation of leaf->top
heap_handle_t heap_create(bool(*compare)(void* A,void* B));
// If free_value is NULL, no operation will be applied to the assigned value
void heap_free(heap_handle_t handle,void(*free_value)(void* value,void* ctx),void* ctx);
heap_value_handle_t heap_add(heap_handle_t handle,void* value);
// This operation will not free value, since value is known to the outside
bool heap_delete(heap_handle_t handle,heap_value_handle_t value_handle);
// call this after altering the value to update the heap
bool heap_update(heap_handle_t handle,heap_value_handle_t value_handle);
void* heap_pop(heap_handle_t handle);
void* heap_get(heap_handle_t handle);
int heap_get_length(heap_handle_t handle);

#endif