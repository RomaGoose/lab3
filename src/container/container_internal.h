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

typedef struct _Iterator Iterator;
typedef int (*next_func)(Iterator* i);
typedef int (*prev_func)(Iterator* i);
// typedef void* (*get_elem_func)(Iterator* i);
// typedef void* (*set_elem_func)(Iterator* i);
// typedef size_t (*get_pos_func)(Iterator* i);

typedef struct _IteratorVFTable {   ///< виртуальная таблица функций итератора
    next_func next;                 ///< функция перехода к следующему элементу
    prev_func prev;                 ///< функция перехода к предыдущему элементу
    // get_elem_func get_element;      ///< функция получения текущего элемента 
    // set_elem_func set_element;      ///< функция установления значения текущего элемента 
    // get_pos_func get_pos;           ///< функция получения текущей позиции
} IteratorVFTable;

typedef struct _Iterator {
    DLList* list;           ///< указатель на список, по которому идет итерация
    size_t pos;             ///< позиция (индекс) итератора 
    Node* elem;             ///< указатель на текущий элемент
    IteratorVFTable vft;    ///< виртуальная таблица функций итератора
} Iterator;
