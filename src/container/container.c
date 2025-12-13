#include <stdio.h>

#include "container_internal.h"

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
    if (index >= list->size) return NULL;
    
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
// void* get_head(DLList* list) { return (void*)list->head; }
// void* get_tail(DLList* list) { return (void*)list->tail; }
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
    if (index >= list->size) {return;}  //как ошибки такие обрабатывать?
    if (index == 0) {insert_start(info, list); return;}
    if (index == list->size-1) {insert_end(info, list); return;}

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

typedef struct _Iterator {
    DLList* list;
    size_t pos;
} Iterator;

int next(Iterator* i){
    if (++i->pos >= i->list->size) return 1;
    return 0;
}

size_t get_pos(Iterator* i) { return i->pos; }

void* iterator_get(Iterator* i){ return get_element(i->pos, i->list); }

void set(void* value, Iterator* i){
    get_node(i->pos, i->list)->info = value;
}

Iterator* begin(DLList* list){
    Iterator* i = malloc(sizeof(Iterator));
    mem_check_exit(i);

    i->list = list;
    i->pos = 0;

    return i;
}

void free_iterator(Iterator* i) { free(i); }

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
        for (next(i); get_pos(i) < list_size - run; next(i)){
            if(cmp(get_element(pos_of_max, list), iterator_get(i)) < 0) 
                pos_of_max = get_pos(i);
            else sorted = 0;
        }
        free_iterator(i);
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
