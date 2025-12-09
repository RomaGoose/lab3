#pragma once

#include <stdlib.h>

#define mem_check_exit(x) do {                             \
    if(x == NULL) {                                        \
        fprintf(stderr, "%s", "UNABLE TO ALLOCATE MEMORY");\
        exit(1);                                           \
    }} while (0)

typedef struct _Node Node;
typedef struct _Node {
    void* info;
    Node* next;
    Node* prev;
} Node;

typedef struct _DLList {
    Node* head;
    Node* tail;
    size_t size;
    size_t element_size;
}DLList;
