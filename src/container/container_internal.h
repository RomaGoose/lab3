#pragma once

#include <stdlib.h>

/**
 * @def mem_check_exit(x)
 * @brief проверка, получилось ли выделить память. если нет то завершает программу
 * @param x указатель на блок памяти
 */
#define mem_check_exit(x) do {                             \
    if(x == NULL) {                                        \
        fprintf(stderr, "%s", "UNABLE TO ALLOCATE MEMORY");\
        exit(1);                                           \
    }} while (0)

typedef struct _Node Node;
/** 
 * @typedef Node 
 * @brief элемент двусвязного списка
 */
typedef struct _Node {
    void* info;  ///< указатель на блок информации
    Node* next;  ///< указатель на следующий элемент
    Node* prev;  ///< указатель на предыдущий элемент
} Node;

/** 
 * @typedef DLList 
 * @brief двусвязный список
 */
typedef struct _DLList {
    Node* head; ///< указатель на первый элемент списка
    Node* tail; ///< указатель на последний элемент списка
    size_t size; ///< количество элементов в списке
    size_t element_size; ///< размер хранимого элемента (в байтах)
}DLList;
