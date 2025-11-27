#pragma once
#include <stdlib.h>

typedef struct _DLList DLList;

DLList* init_DLList(size_t element_size);
size_t get_size(DLList* list);
void* get_head(DLList* list);
void* get_tail(DLList* list);
void* get_element(size_t index, DLList* list);
void insert_start(void* info, DLList* list);
void insert_end(void* info, DLList* list);
void insert_(size_t index, void* info, DLList* list);
void remove_start(DLList* list);
void remove_end(DLList* list);
void remove_(size_t index, DLList* list);
void kill_list(DLList* list);

void swap(size_t index_1, size_t index_2, DLList* list);
void* convert_list_to_array(DLList* list);
DLList* convert_array_to_list(void* arr, size_t count, size_t element_size);