#pragma once

#include "article_internal.h"
#include "inout.h"


#define err_exit(err_message) do {\
        fprintf(stderr, "%s", err_message);\
        exit(1);} while (0) 

#define mem_check_exit(x) do {                              \
    if(x == NULL) {                                         \
        fprintf(stderr, "%s", "Не удалось выделить память");\
        exit(1);                                            \
    }} while (0)

#define ASCII_DIGITS_OFFSET 48 ///< разница между числом и его символом char
#define MAX_INPUT_LEN MAX_ARTICLE_SIZE + MAX_ARTICLE_NAME_LEN + MAX_MAGAZINE_NAME_LEN ///< максимальная длина строки формата .csv с запасом на кавычки и их экранирование
                                    
#define MAX_NAME_WIDTH 40 ///< ширина столбца с названием статьи в таблице
#define MAX_MAGAZINE_WIDTH 30 ///< ширина столбца с названием журнала в таблице


typedef enum {
        NAME,
        SURN,
        INIT,
        MAGNAME,
        YEAR,
        MAGVOL,
        PAGES,
        CIT,
        RSCI
    } field_tag;


void printline_csv(Article* a, FILE* output);