#pragma ocne

#include <stdlib.h>
#include <stdio.h>

#define MAX_F_ARGC 10 ///< максимальное количество отформатированных элемнтов командной строки 
#define mem_check_exit(x) do {                             \
    if(x == NULL) {                                        \
        fprintf(stderr, "%s", "UNABLE TO ALLOCATE MEMORY");\
        exit(1);                                           \
    }} while (0)
#define clear() do{char c; while((c=getchar())!=EOF && c != '\n');} while(0)

typedef enum { ASC, DESC } sort_type_tag;
typedef enum { SEL, QUICK } sort_alg_tag;
typedef enum {
    GEN,
    PRINT,
    SORT,
    TYPE,
    ALG,
    IN,
    OUT,

    END
} arg_tag;

/**
 * @typedef State
 * @brief Состояние приложения. Вся информация о режиме работы приложения.
 */
typedef struct _State {
    arg_tag task;                    ///< задача, режим работы приложения 
    union {
        size_t N;                    ///< количество записей для генерации
        struct {
            sort_type_tag sort_type; ///< тип сортивроки (возр/убыв)
            sort_alg_tag sort_alg;   ///< алгоритм сортивроки (выборов/быстрая)
        } sort_value;                ///< информация о сортировке
    } task_value;                    ///< информация о задаче
    FILE* input;                     ///< поток ввода 
    FILE* output;                    ///< поток вывода
} State;

typedef struct _Argument {
    arg_tag tag;
    const char* long_name;
    char name;
    const char* long_arg_value;
    const char* short_arg_value;
    const char* description;
} Argument;


/**
 * @brief Обработка аргументов командной строки. Исполнение приложения 
 * @param argc количество введенных аргументов командной строки 
 * @param argv массив строк с аргументами командной строки
 */
void parse_args(int argc, char** argv);