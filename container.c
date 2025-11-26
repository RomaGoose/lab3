#include <stdlib.h>

#define MemCheckExit(x) do {if(x == NULL) exit(1);} while (0)

typedef struct _Research {int n;} Research;
typedef struct _Node Node;
typedef struct _Node {
    Research* info;
    Node* next;
    Node* prev;
} Node;

typedef struct _DLList DLList;
typedef struct _DLList {
    Node* head;
    Node* tail;
    size_t size;
}DLList;

DLList* init_DLList(){
    DLList* new = malloc(sizeof(DLList));
    MemCheckExit(new);
    new->head = NULL;
    new->tail = NULL;
    new->size = 0;
    return new;
}

static Node* get_node(size_t index, DLList* list) {
    if (index > list->size) return NULL;
    
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
void* get_head(DLList* list) { return (void*)list->head; }
void* get_tail(DLList* list) { return (void*)list->tail; }
void* get_element(size_t index, DLList* list){ return (void*)(get_node(index, list)->info); }

#pragma region inserts

void insert_start(void* info, DLList* list){
    Node* new_node = malloc(sizeof(Node));
    MemCheckExit(new_node);
    
    new_node->next = list->head;
    new_node->prev = NULL;
    new_node->info = (Research *)info;

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
    MemCheckExit(new_node);

    new_node->next = NULL;
    new_node->prev = list->tail;
    new_node->info = (Research *)info;

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
    MemCheckExit(new);

    Node* old = get_node(index, list);
    new->info = (Research *)info;

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

void kill_list(DLList* list){
    Node* tmp;
    for(Node* node = list->head; node != NULL; node = tmp){
        tmp = node->next;
        free(node);
    }
    free(list);
}