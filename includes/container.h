#pragma once

#include <stdlib.h>
#include <stdint.h>

/**
 * @typedef DLList 
 * @brief двусвязный список
 */ 
typedef struct _DLList DLList; 

/**
 * @brief Инициализация списка
 * @param element_size размер хранимого элемента
 * @return указатель на инициализированный список
 */
DLList* init_DLList(size_t element_size);
/**
 * @brief Получение количества элементов в списке
 * @param list указатель на список
 * @return количество элементов в списке
 */
size_t get_size(DLList* list);
/**
 * @brief Получение содержимого элемента списка по индексу
 * @param list указатель на список
 * @param index индекс элемента
 * @return указатель на содержимое элемента
 * 
 * @note NULL в случае выхода за пределы списка
 */
void* get_element(size_t index, DLList* list);
/**
 * @brief Вставка элемента в начало списка
 * @param info указатель на содержимое элемента 
 * @param list указатель на список
 */
void insert_start(void* info, DLList* list);
/**
 * @brief Вставка элемента в конец списка
 * @param info указатель на содержимое элемента 
 * @param list указатель на список
 */
void insert_end(void* info, DLList* list);
/**
 * @brief Вставка элемента в список в указанный индекс
 * @param index индекс, на котором окажется новый элемент 
 * @param info указатель на содержимое элемента 
 * @param list указатель на список
 */
void insert_(size_t index, void* info, DLList* list);
/**
 * @brief Удаление первого элемента списка
 * @param list указатель на список
 */
void remove_start(DLList* list);
/**
 * @brief Удаление последнего элемента списка
 * @param list указатель на список
 */
void remove_end(DLList* list);
/**
 * @brief Удаление элемента списка по указанному индексу
 * @param index индекс удаляемого элемента 
 * @param list указатель на список
 */
void remove_(size_t index, DLList* list);
/**
 * @brief Удаление всех элементов списка и их содержимого
 * @param list указатель на список
 */
void clear_list(DLList* list);
/**
 * @brief Удаление списка, всех его элементов и их содержимого
 * @param list указатель на список
 */
void kill_list(DLList* list);
/**
 * @brief Замена местами двух элементов списка
 * @param index_1 индекс первого элемента 
 * @param index_2 индекс второго элемента 
 * @param list указатель на список
 */
void swap(size_t index_1, size_t index_2, DLList* list);
/**
 * @brief Конвертация списка в массив с содержимым элементов списка
 * @param list указатель на список
 * @return указатель на начало массива с содержимым
 */
void* convert_list_to_array(DLList* list);
/**
 * @brief Конвертация массива в список 
 * @param arr массив
 * @param count количество элементов массива
 * @param element_size размер одного элемента массива
 * @return указатель на список
 */
DLList* convert_array_to_list(void* arr, size_t count, size_t element_size);
/**
 * @brief Сортировка списка Выбором
 * @param list указатель на список
 * @param cmp указатель на функцию-компаратор 
 */
void sel_sort(DLList* list, int (*cmp)(void* l, void*r));
/**
 * @brief Сортировка списка Быстрая (Хоара)
 * @param list указатель на список
 * @param cmp указатель на функцию-компаратор 
 */
void quick_sort(DLList* list, int (*cmp)(void* l, void*r));

/**
 * @typedef Iterator 
 * @brief итератор двусвязного списка
 */ 
typedef struct _Iterator Iterator; 
/**
 * @brief Переход итератора на следующий элемент
 * @param i указатель на итератор 
 * @return 1 если после перехода итератор оказался за пределами списка
 *         0 в противном случае
 */
int next(Iterator* i);
/**
 * @brief Переход итератора на предыдущий элемент
 * @param i указатель на итератор 
 * @return 1 если после перехода итератор оказался за пределами списка 
 *         0 в противном случае
 */
int next(Iterator* i);
/**
 * @brief Получение позиции итератора 
 * @param i указатель на итератор 
 * @return позиция итератора
 */
size_t iterator_get_pos(Iterator* i);
/**
 * @brief Получение содержимого текущего элемента итератора 
 * @param i указатель на итератор 
 * @return указатель на содержимое
 * 
 * @note NULL в случае выхода за пределы списка
 */
void* iterator_get(Iterator* i);
/**
 * @brief Установить указатель на содержимое текущего элемента итератора 
 * @param value указатель на содержимое
 * @param i указатель на итератор 
 */
void iterator_set(void* value, Iterator* i);
/**
 * @brief Сравнение двух итераторов
 * @param i_1 первый итератор
 * @param i_2 второй итератор 
 * @return 1 если равны
 *         0 если не равны
 */
uint8_t eq(Iterator* i_1, Iterator* i_2);
/**
 * @brief Прямой итератор списка
 * @param list указатель на список 
 * @return указатель на итератор
 */
Iterator* begin(DLList* list);
/**
 * @brief Обратный итератор списка
 * @param list указатель на список 
 * @return указатель на итератор
 */
Iterator* rbegin(DLList* list);
/**
 * @brief Конец прямого итератора 
 * @param list указатель на список 
 * @return указатель на итератор
 */
Iterator* end(DLList* list);
/**
 * @brief Конец обратного итератора 
 * @param list указатель на список 
 * @return указатель на итератор
 */
Iterator* rend(DLList* list);
