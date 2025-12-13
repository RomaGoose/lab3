#pragma once

#include <stdio.h>

#include "container.h"

/**
 * @brief Конвертация строки в число
 * @param str строка
 * @return число
 */
size_t str_to_ull(char* str);
/**
 * @brief Сканирует данные в формате .csv, создает список на основе данных из файла
 * @param input поток ввода 
 * @return указатель на созданный список
 */
DLList* scan_csv(FILE* input);
/**
 * @brief Выводит список в формате .csv
 * @param input поток вывода
 */
void print_csv(DLList* list, FILE* output);
/**
 * @brief Выводит список в формате таблицы с фиксированной шириной
 * @param input поток вывода
 */
void print_table(DLList* list, FILE* output);
/**
 * @brief Чтение данных из wordlists для генерации
 */
void read_lists();
/**
 * @brief Очистка памяти от прочтенных данных из wordlists 
 */
void free_lists();
/**
 * @brief Генерация N записей в формате .csv 
 * @param N количество записей
 * @param output поток вывода
 */
void generate(size_t N, FILE* output);
