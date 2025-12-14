#include <stdio.h>
#include <stdint.h>

#include "container_internal.h"
#include "inout.h"

DLList* init_DLList(size_t element_size){
    DLList* new = malloc(sizeof(DLList));
    mem_check_exit(new);
    new->head = NULL;
    new->tail = NULL;
    new->size = 0;
    new->element_size = element_size;
    return new;
}

static Node* get_node(size_t index, DLList* list) {
    if (index >= list->size) { LDBG("Выход за предел списка: получение элемента по индексу"); return NULL; }
    
    Node* curr_node;
    if (index > list->size / 2) {
        curr_node = list->tail;
        for (size_t i = list->size-1; i > index; --i)
            curr_node=curr_node->prev;

        return curr_node;
    }
    curr_node = list->head;
    for (size_t i = 0; i < index; ++i)
        curr_node=curr_node->next;
    return curr_node;
}

size_t get_size(DLList* list) { return list->size; }
void* get_element(size_t index, DLList* list){ return get_node(index, list)->info; }

void clear_list(DLList* list){
    Node* tmp;
    for(Node* node = list->head; node != NULL; node = tmp){
        tmp = node->next;
        free(node->info);
        free(node);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void kill_list(DLList* list){
    clear_list(list);
    free(list);
}

#pragma region inserts

void insert_start(void* info, DLList* list){
    Node* new_node = malloc(sizeof(Node));
    mem_check_exit(new_node);
    
    new_node->next = list->head;
    new_node->prev = NULL;
    new_node->info = info;

    if(list->size != 0)
        list->head->prev = new_node;
    
    list->head = new_node;
    if(list->size == 0){
        list->tail = new_node;
    }
    ++list->size;
}

void insert_end(void* info, DLList* list){
    Node* new_node = malloc(sizeof(Node));
    mem_check_exit(new_node);

    new_node->next = NULL;
    new_node->prev = list->tail;
    new_node->info = info;

    if(list->size != 0)
        list->tail->next = new_node;
    
    list->tail = new_node;
    if(list->size == 0){
        list->head = new_node;
    }
    ++list->size;
}

void insert_(size_t index, void* info, DLList* list){
    if (index >= list->size) { LWARN("Выход за пределы списка: вставка по индексу"); return; }  
    if (index == 0) { insert_start(info, list); return; }
    if (index == list->size-1) { insert_end(info, list); return; }

    Node* new = malloc(sizeof(Node));
    mem_check_exit(new);

    Node* old = get_node(index, list);
    new->info = info;

    new->next = old;       //связки нового
    new->prev = old->prev;

    old->prev->next = new; //сосед слева
    old->prev = new;       //сосед справа
}

#pragma endregion

#pragma region removes

void remove_start(DLList* list) {
    if (list->size == 1) {
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        return;
    }
    Node* tmp = list->head->next;
    free(tmp->prev);
    tmp->prev = NULL;
    list->head = tmp;

    --list->size;
}

void remove_end(DLList* list) {
    if (list->size == 1) {
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        return;
    }
    Node* tmp = list->tail->prev;
    free(tmp->next);
    tmp->next = NULL;
    list->tail = tmp;
    
    --list->size;
}

void remove_(size_t index, DLList* list){
    Node* victim = get_node(index, list);

    victim->prev->next = victim->next; //сосед слева
    victim->next->prev = victim->prev; //сосед справа

    --list->size;
    free(victim);
}

#pragma endregion

#pragma region swaps

static void swap_adjacent(size_t index_1, size_t index_2, DLList* list);
static void swap_distant(size_t index_1, size_t index_2, DLList* list);

void swap(size_t index_1, size_t index_2, DLList* list){
    if(index_1 == index_2) return;
    
    if(index_1 > index_2){
        size_t tmp_index = index_1;
        index_1 = index_2;
        index_2 = tmp_index;
    }

    if(index_2 - index_1 == 1) {
        swap_adjacent(index_1, index_2, list); 
        return;
    }
    else{
        swap_distant(index_1, index_2, list);
        return;
    }
}

static void swap_adjacent(size_t index_1, size_t index_2, DLList* list){
    Node* node_1 = get_node(index_1, list);
    Node* node_1_prev = node_1->prev;
    Node* node_2 = get_node(index_2, list);


    if (node_1->prev == NULL)
        list->head = node_2;
    else 
        node_1->prev->next = node_2;
    
    node_1->next = node_2->next;
    node_1->prev = node_2;


    if (node_2->next == NULL)
        list->tail = node_1;
    else
        node_2->next->prev = node_1;

    node_2->next = node_1;
    node_2->prev = node_1_prev;
}

static void swap_distant(size_t index_1, size_t index_2, DLList* list){
    Node* node_1 = get_node(index_1, list);
    Node* node_1_prev = node_1->prev;
    Node* node_1_next = node_1->next;
    Node* node_2 = get_node(index_2, list);


    if(node_1->prev == NULL)
        list->head = node_2;
    else
        node_1->prev->next = node_2;

    node_1->next->prev = node_2;

    node_1->next = node_2->next;      
    node_1->prev = node_2->prev;


    if(node_2->next == NULL)         
        list->tail = node_1;
    else 
        node_2->next->prev = node_1;
    
    node_2->prev->next = node_1;
    
    node_2->next = node_1_next;        
    node_2->prev = node_1_prev;
}

#pragma endregion

#pragma region converts and memory

#ifndef __GNUC__
    #error "Unsupported type: __uint128_t. Use gcc or clang."
#endif

static void copy_element(void* src, void* destination, size_t size){
    size_t num_of_16bytes = size/16;
    size_t remainder_bytes = size%16;

    for(size_t i = 0; i < num_of_16bytes; ++i)
        *((__uint128_t*)destination + i) = *((__uint128_t*)src + i);
    
    destination = (__uint128_t*)destination + num_of_16bytes;
    src = (__uint128_t*)src + num_of_16bytes;

    for(size_t i = 0; i < remainder_bytes; ++i)
        *((char*)destination + i) = *((char*)src + i);
    
}

void* convert_list_to_array(DLList* list){
    void* arr = malloc((list->size)*(list->element_size));
    mem_check_exit(arr);

    size_t bit_size = list->element_size;
    
    for(size_t i = 0; i < list->size; ++i){
        copy_element(get_element(i, list), (char*)arr + (list->element_size)*i, list->element_size);
    }

    return arr;
}

DLList* convert_array_to_list(void* arr, size_t count, size_t element_size){
    DLList* list = init_DLList(element_size);

    for(size_t i = 0; i < count; ++i){
        void* new_info = malloc(element_size);
        copy_element((char*)arr + element_size*i, new_info, element_size);
        insert_end(new_info, list);
    }

    return list;
}

#pragma endregion 

#pragma region iterator

size_t iterator_get_pos(Iterator* i) { return i->pos; }

void* iterator_get(Iterator* i){ return i->elem->info; }

void iterator_set(void* value, Iterator* i){ i->elem->info = value; }

uint8_t eq(Iterator* i_1, Iterator* i_2) { return i_1->elem == i_2->elem; }

static int next_f(Iterator* i){
    i->elem = i->elem->next;
    ++i->pos;
    if (i->elem == NULL) return 1;
    return 0;
}

static int rnext_f(Iterator* i){
    i->elem = i->elem->prev;
    --i->pos;
    if (i->elem == NULL) return 1;
    return 0;
}

static int prev_f(Iterator* i){
    i->elem = i->elem->prev;
    --i->pos;
    if (i->elem == NULL) return 1;
    return 0;
}

static int rprev_f(Iterator* i){
    i->elem = i->elem->next;
    ++i->pos;
    if (i->elem == NULL) return 1;
    return 0;
}

int next(Iterator* i) { return i->vft.next(i); }
int prev(Iterator* i) { return i->vft.prev(i); }

Iterator* begin(DLList* list){
    Iterator* i = malloc(sizeof(Iterator));
    mem_check_exit(i);

    i->list = list;
    i->pos = 0;
    i->elem = list->head;

    i->vft.next = next_f;
    i->vft.prev = prev_f;
    // i->vft.get_pos = iterator_get_pos;
    // i->vft.get_element = iterator_get;
    // i->vft.set_element = iterator_set;

    return i;
}

Iterator* end(DLList* list) {
    Iterator* i = malloc(sizeof(Iterator));
    mem_check_exit(i);

    i->list = list;
    i->pos = list->size;
    i->elem = NULL;

    return i;
}

Iterator* rbegin(DLList* list){
    Iterator* i = malloc(sizeof(Iterator));
    mem_check_exit(i);

    i->list = list;
    i->pos = 0;
    i->elem = list->tail;

    i->vft.next = rnext_f;
    i->vft.prev = rprev_f;
    // i->vft.get_pos = iterator_get_pos;
    // i->vft.get_element = iterator_get;
    // i->vft.set_element = iterator_set;

    return i;
}

Iterator* rend(DLList* list) {
    Iterator* i = malloc(sizeof(Iterator));
    mem_check_exit(i);

    i->list = list;
    i->pos = ULLONG_MAX;
    i->elem = NULL;

    return i;
}


#pragma endregion

void sel_sort(DLList* list, int (*cmp)(void* l, void*r)){
    Iterator* i;
    size_t list_size = get_size(list);
    size_t pos_of_max;
    char sorted;
    for(size_t run = 0; run < list_size; ++run){
        pos_of_max = 0;
        sorted = 1;
        i = begin(list);
        for (next(i); iterator_get_pos(i) < list_size - run; next(i)){
            if(cmp(get_element(pos_of_max, list), iterator_get(i)) < 0) 
                pos_of_max = iterator_get_pos(i);
            else sorted = 0;
        }
        free(i);
        if (sorted) return;
        swap(pos_of_max, list_size-1 - run, list);
    }
}

static void quick_sort_lr(DLList* list, int (*cmp)(void* l, void*r), size_t l, size_t r){
    if(l>=r) return;
    void* pivot = get_element((l+r)/2, list);
    size_t i = l, j = r;
    while(i<=j){
        while(cmp(get_element(i, list), pivot) < 0) ++i;
        while(cmp(get_element(j, list), pivot) > 0) --j;
        if(i<=j){
            swap(i, j, list);
            ++i; 
            if (j > 0) --j; else break;
        }
    }
    quick_sort_lr(list, cmp, l, j);
    quick_sort_lr(list, cmp, i, r);
}

void quick_sort(DLList* list, int(*cmp)(void* l, void* r)) {
    quick_sort_lr(list, cmp, 0, list->size - 1);
}
