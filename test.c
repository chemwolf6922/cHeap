#include <stdio.h>
#include <stdbool.h>
#include "heap.h"

bool compare_value(void* A,void* B)
{
    return *(int*)A > *(int*)B;
}

int main(int argc, char const *argv[])
{
    heap_handle_t heap = heap_create(compare_value);

    int v1 = 1;
    int v2 = 10;
    int v3 = 2;
    int v4 = 11;
    int v5 = 12;
    int v6 = 4;
    int v7 = 3;
    heap_value_handle_t v1h = heap_add(heap,&v1);
    heap_value_handle_t v2h = heap_add(heap,&v2);
    heap_value_handle_t v3h = heap_add(heap,&v3);
    heap_value_handle_t v4h = heap_add(heap,&v4);
    heap_value_handle_t v5h = heap_add(heap,&v5);
    heap_value_handle_t v6h = heap_add(heap,&v6);
    heap_value_handle_t v7h = heap_add(heap,&v7);
    heap_delete(heap,v4h);
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    v1h = heap_add(heap,&v1);
    v2h = heap_add(heap,&v2);
    v3h = heap_add(heap,&v3);
    v4h = heap_add(heap,&v4);
    v5h = heap_add(heap,&v5);
    v6h = heap_add(heap,&v6);
    v7h = heap_add(heap,&v7);
    v4 = 0;
    heap_update(heap,v4h);
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));
    printf("%d\n",*(int*)heap_pop(heap));

    heap_free(heap,NULL,NULL);
    return 0;
}
