#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <inttypes.h>
#include "heap.h"

typedef struct
{
    int v;
} test_value_t;

bool compare_value(void* A,void* B)
{
    return ((test_value_t*)A)->v > ((test_value_t*)B)->v;
}

void function_test()
{
    heap_handle_t heap = heap_create(compare_value);

    test_value_t v1 = {.v = 1};
    test_value_t v2 = {.v = 10};
    test_value_t v3 = {.v = 2};
    test_value_t v4 = {.v = 11};
    test_value_t v5 = {.v = 12};
    test_value_t v6 = {.v = 4};
    test_value_t v7 = {.v = 3};

    heap_add(heap,&v1);
    heap_add(heap,&v2);
    heap_add(heap,&v3);
    heap_add(heap,&v4);
    heap_add(heap,&v5);
    heap_add(heap,&v6);
    heap_add(heap,&v7);
    heap_delete(heap,&v4);
    printf("%d\n",((test_value_t*)heap_pop(heap))->v);
    printf("%d\n",((test_value_t*)heap_pop(heap))->v);
    printf("%d\n",((test_value_t*)heap_pop(heap))->v);
    printf("%d\n",((test_value_t*)heap_pop(heap))->v);
    printf("%d\n",((test_value_t*)heap_pop(heap))->v);
    printf("%d\n",((test_value_t*)heap_pop(heap))->v);
    

    heap_free(heap,NULL);
}

void speed_test(int n)
{
    test_value_t* vs = malloc(sizeof(test_value_t)*n);
    for(int i=0;i<n;i++)
        vs[i].v = rand();
    
    heap_handle_t heap = heap_create(compare_value);

    for(int i=0;i<n;i++)
        heap_add(heap,&vs[i]);

    for(int i=0;i<n/2;i++)
        heap_delete(heap,&vs[i]);
    
    while(heap_pop(heap)!=NULL){};

    heap_free(heap,NULL);

    free(vs);
}

uint64_t get_now_us()
{
    struct timeval now;
    gettimeofday(&now,NULL);
    return ((uint64_t)now.tv_sec)*1000000 + (uint64_t)now.tv_usec;
}

int main(int argc, char const *argv[])
{
    printf("Function test start\n\n");
    function_test();
    printf("\nFunction test End\n\n");

    printf("Speed test start\n\n");

    uint64_t start,end;

    start = get_now_us();
    speed_test(100);
    end = get_now_us();
    printf("100: %"PRIu64" us\n",end-start);

    start = get_now_us();
    speed_test(1000);
    end = get_now_us();
    printf("1000: %"PRIu64" us\n",end-start);

    start = get_now_us();
    speed_test(10000);
    end = get_now_us();
    printf("10000: %"PRIu64" us\n",end-start);

    start = get_now_us();
    speed_test(100000);
    end = get_now_us();
    printf("100000: %"PRIu64" us\n",end-start);

    printf("\nSpeed test End\n");

    return 0;
}
