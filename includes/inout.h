#pragma once

#include <stdio.h>

#include "container.h"

#define _LOGF(lvl, fmt, ...) fprintf(stderr, "[%s]|[%s:%s:%d] " fmt "\n", \
                            lvl, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define _LOG(lvl, x) _LOGF(lvl, "%s", x)

#if LOGLVL<0
#error NEGATIVE LEVEL
#endif

#if LOGLVL>= 0
#define LERR(msg)
#define LWARN(msg)
#define LDBG(msg)
#define LERRF(fmt, ...)
#define LWARNF(fmt, ...)
#define LDBGF(fmt, ...)
#endif

#if LOGLVL>= 1
#undef LERRF
#define LERRF(fmt, ...) _LOGF("ERR", fmt, __VA_ARGS__)
#undef LERR
#define LERR(msg) LERRF("%s", msg)
#endif

#if LOGLVL >= 2
#undef LWARNF
#define LWARNF(fmt, ...) _LOGF("WRN", fmt, __VA_ARGS__)
#undef LWARN
#define LWARN(msg) LWARNF("%s", msg)
#endif

#if LOGLVL >= 3
#undef LDBGF
#define LDBGF(fmt, ...) _LOGF("DBG", fmt, __VA_ARGS__)
#undef LDBG
#define LDBG(msg) LDBGF("%s", msg)
#endif

/**
 * @def mem_check_exit(x)
 * @brief проверка, получилось ли выделить память, в случае неудачи завершает программу
 * @param mem_ptr указатель на блок памяти
 */
#define mem_check_exit(mem_ptr) do {       \
    if(mem_ptr == NULL) {                  \
        LERR("Не удалось выделить память");\
        exit(1);                           \
    }} while (0)
/**
 * @def file_check_exit(file, path)
 * @brief проверка, получилось ли открыть файл, в случае неудачи завершает программу
 * @param file файловый поток
 * @param path путь к файлу
 */
#define file_check_exit(file, path) do {              \
    if(file == NULL) {                          \
        LERRF("Не удалось открыть файл:", path);\
        exit(1);                                \
    }} while (0)

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
 * @param list список для вывода
 * @param output поток вывода
 */
void print_csv(DLList* list, FILE* output);
/**
 * @brief Выводит список в формате таблицы с фиксированной шириной
 * @param list список для вывода
 * @param output поток вывода
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
