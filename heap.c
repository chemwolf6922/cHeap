#include "heap.h"
#include <stdlib.h>
#include <stddef.h>

typedef struct heap_value_s heap_value_t;

typedef struct heap_node_s
{
    heap_value_t* heap_value;
    struct heap_node_s* left;
    struct heap_node_s* right;
    struct heap_node_s* top;
}heap_node_t;

struct heap_value_s
{
    heap_node_t* node;
    void* value;
};

typedef struct
{
    struct heap_node_s* entry;
    int length;
    bool(*compare)(void* A,void* B);
}heap_t;

/* function predefines */
heap_node_t* heap_get_top(heap_t* heap,int n,bool* is_left);
heap_node_t* heap_adjust_backward(heap_t* heap,heap_node_t* node);
heap_node_t* heap_adjust_forward(heap_t* heap,heap_node_t* node);
void heap_free_node(heap_node_t* node,void(*free_value)(void* value,void* ctx), void* ctx,bool propagate);
heap_node_t* heap_new_node(heap_value_t* heap_value);
heap_value_t* heap_new_value(void* value);

/* public functions */

heap_handle_t heap_create(bool(*compare)(void* A,void* B))
{
    if(compare == NULL)
        return NULL;
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap)
        return NULL;
    heap->compare = compare;
    heap->entry = NULL;
    heap->length = 0;
    return (heap_handle_t)heap;
}

void heap_free(heap_handle_t handle,void(*free_value)(void* value,void* ctx),void* ctx)
{
    if(handle == NULL)
        return;
    heap_t* heap = (heap_t*)handle;
    heap_free_node(heap->entry,free_value,ctx,true);
    free(heap);
}

heap_value_handle_t heap_add(heap_handle_t handle,void* value)
{
    if(!handle)
        return NULL;
    heap_t* heap = (heap_t*)handle;
    heap_value_t* heap_value = heap_new_value(value);
    if(!heap_value)
        return NULL;
    heap_node_t* new_node = heap_new_node(heap_value);
    if(!new_node)
    {
        free(heap_value);
        return NULL;
    }
    if(heap->entry == NULL)
    {
        heap->entry = new_node;
        heap->length ++;
        return (void*)heap_value;
    }
    bool is_left;
    heap_node_t* node = heap_get_top(heap,++heap->length,&is_left);
    if(is_left)
        node->left = new_node;
    else
        node->right = new_node;
    new_node->top = node;
    heap_adjust_backward(heap,new_node);
    return (void*)heap_value;
}

bool heap_delete(heap_handle_t handle,heap_value_handle_t heap_value_handle)
{
    if(!handle)
        return false;
    heap_t* heap = (heap_t*)handle;
    if(!heap_value_handle)
        return false;
    heap_value_t* heap_value = (heap_value_t*)heap_value_handle;
    heap_node_t* node = heap_value->node;
    if(!node)
        return false;
    if(heap->length == 1)
    {
        /* free the only node */
        heap_free_node(node,NULL,NULL,false);
        heap->entry = NULL;
        heap->length --;
        return true;
    }
    bool is_left;
    heap_node_t* tail_top_node = heap_get_top(heap,heap->length,&is_left);
    heap_node_t* tail_node = is_left?tail_top_node->left:tail_top_node->right;
    /* switch node & tail_node value */
    heap_value_t* temp_heap_value = node->heap_value;
    node->heap_value = tail_node->heap_value;
    node->heap_value->node = node;
    tail_node->heap_value = temp_heap_value;
    /* redundant */
    // tail_node->heap_value->node = tail_node;
    /* free the tail node */
    heap_free_node(tail_node,NULL,NULL,false);
    heap->length --;
    if(is_left)
        tail_top_node->left = NULL;
    else
        tail_top_node->right = NULL;
    if(node != tail_node)
    {
        node = heap_adjust_backward(heap,node);
        heap_adjust_forward(heap,node);
    } 
    return true;
}

bool heap_update(heap_handle_t handle,heap_value_handle_t value_handle)
{
    if(!handle)
        return false;
    heap_t* heap = (heap_t*)handle;
    if(!value_handle)
        return false;
    heap_value_t* heap_value = (heap_value_t*)value_handle;
    heap_node_t* node = heap_value->node;
    if(!node)
        return false;
    node = heap_adjust_backward(heap,node);
    heap_adjust_forward(heap,node);
    return true;
}

void* heap_pop(heap_handle_t handle)
{
    if(!handle)
        return NULL;
    heap_t* heap = (heap_t*)handle;
    void* result = NULL;
    if(heap->entry != NULL)
        result = heap->entry->heap_value->value;
    if(heap->length == 1)
    {
        heap->length--;
        /* free the only node */
        heap_free_node(heap->entry,NULL,NULL,false);
        heap->entry = NULL;
    }
    else if(heap->length > 1)
    {
        bool is_left;
        heap_node_t* tail_top_node = heap_get_top(heap,heap->length,&is_left);
        heap_node_t* tail_node = is_left?tail_top_node->left:tail_top_node->right;
        heap_value_t* temp_heap_value = heap->entry->heap_value;
        /* switch entry & tail value */
        heap->entry->heap_value = tail_node->heap_value;
        heap->entry->heap_value->node = heap->entry;
        tail_node->heap_value = temp_heap_value;
        /* redundant */
        // tail_node->heap_value->node = tail_node;
        /* free the tail node */
        heap_free_node(tail_node,NULL,NULL,false);
        if(is_left)
            tail_top_node->left = NULL;
        else
            tail_top_node->right = NULL;
        heap->length--;
        heap_adjust_forward(heap,heap->entry);
    }
    return result;
}

void* heap_get(heap_handle_t handle)
{
    if(!handle)
        return NULL;
    heap_t* heap = (heap_t*)handle;
    void* result = NULL;
    if(heap->entry != NULL)
        result = heap->entry->heap_value->value;
    return result;
}

int heap_get_length(heap_handle_t handle)
{
    if(!handle)
        return -1;
    heap_t* heap = (heap_t*)handle;
    return heap->length;
}

/* private functions */
heap_node_t* heap_get_top(heap_t* heap,int n,bool* is_left)
{
    if(n==1)
        return NULL;
    heap_node_t* node = heap->entry;
    int layers = 32 - __builtin_clz(n);
    for(int i=layers-2;i>0;i--)
    {
        if((n>>i)&1)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
    *is_left = !(n&1);
    return node;
}

heap_node_t* heap_adjust_backward(heap_t* heap,heap_node_t* node)
{
    if(node->top == NULL)
        return node;
    if(heap->compare(node->top->heap_value->value,node->heap_value->value))
    {
        heap_value_t* temp_heap_value = node->heap_value;
        node->heap_value = node->top->heap_value;
        node->heap_value->node = node;
        node->top->heap_value = temp_heap_value;
        node->top->heap_value->node = node->top;
        return heap_adjust_backward(heap,node->top);
    }
    else
    {
        return node;
    }
}

heap_node_t* heap_adjust_forward(heap_t* heap,heap_node_t* node)
{
    heap_node_t* target_node = NULL;
    if(node->left != NULL && node->right != NULL)
    {
        heap_node_t* less_node = NULL;
        if(heap->compare(node->left->heap_value->value,node->right->heap_value->value))
        {
            less_node = node->right;
        }
        else
        {
            less_node = node->left;
        }
        if(heap->compare(node->heap_value->value,less_node->heap_value->value))
        {
            target_node = less_node;
        }
    }
    else if(node->left != NULL && node->right == NULL)
    {
        if(heap->compare(node->heap_value->value,node->left->heap_value->value))
        {
            target_node = node->left;
        }
    }
    else if(node->left == NULL && node->right != NULL)
    {
        if(heap->compare(node->heap_value->value,node->right->heap_value->value))
        {
            target_node = node->right;
        }
    }
    if(target_node != NULL)
    {
        heap_value_t* temp_heap_value = node->heap_value;
        node->heap_value = target_node->heap_value;
        node->heap_value->node = node;
        target_node->heap_value = temp_heap_value;
        target_node->heap_value->node = target_node;
        return heap_adjust_forward(heap,target_node);
    }
    else
    {
        return node;
    }
}

void heap_free_node(heap_node_t* node,void(*free_value)(void* value,void* ctx), void* ctx,bool propagate)
{
    if(node == NULL)
        return;
    
    heap_node_t* left = node->left;
    heap_node_t* right = node->right;

    if(node->heap_value != NULL)
    {
        if(free_value != NULL)
            free_value(node->heap_value->value,ctx);
        free(node->heap_value);
    }
    free(node);
    
    if(propagate)
    {
        heap_free_node(left,free_value,ctx,propagate);
        heap_free_node(right,free_value,ctx,propagate);
    }
}

heap_node_t* heap_new_node(heap_value_t* heap_value)
{
    heap_node_t* node = malloc(sizeof(heap_node_t));
    if(!node)
        return NULL;
    node->left = NULL;
    node->right = NULL;
    node->top = NULL;
    node->heap_value = heap_value;
    heap_value->node = node;
    return node;
}

heap_value_t* heap_new_value(void* value)
{
    heap_value_t* heap_value = malloc(sizeof(heap_value_t));
    if(!heap_value)
        return NULL;
    heap_value->value = value;
    return heap_value;
}
