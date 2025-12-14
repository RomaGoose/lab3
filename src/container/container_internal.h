#pragma once

#include <stdlib.h>


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

typedef struct _DLList {
    Node* head;         ///< указатель на первый элемент списка
    Node* tail;         ///< указатель на последний элемент списка
    size_t size;        ///< количество элементов в списке
    size_t element_size;///< размер хранимого элемента (в байтах)
}DLList;

typedef struct _Iterator {
    DLList* list;   ///< указатель на список, по которому идет итерация
    size_t pos;     ///< позиция (индекс) итератора 
} Iterator;
