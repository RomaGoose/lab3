#pragma once

#include <stdint.h>
#include <stdio.h>

#define MAX_SURN_LEN 18*2 ///< максимальный размер в байтах строки с фамилией
#define MAX_ARTICLE_NAME_LEN 128*2 ///< максимальный размер в байтах строки с названием статьи
#define MAX_MAGAZINE_NAME_LEN 80*2 ///< максимальный размер в байтах строки с названием журнала
#define MAX_INITIALS_LEN 12 ///< максимальный размер в байтах строки с инициалами автора
#define MAX_YEAR 9999
#define MAX_CITATIONS 1000000

#define NUM_OF_FIELDS 9 

#define MAX_ARTICLE_SIZE MAX_ARTICLE_NAME_LEN + MAX_SURN_LEN +      \
        MAX_INITIALS_LEN + MAX_MAGAZINE_NAME_LEN + 4 + 6*3 + 7 + 2 + 2  ///< максимальный размер в байтах строки со всеми полями структуры

typedef struct _Article {
    char article_name[MAX_ARTICLE_NAME_LEN];
    char author_surname[MAX_SURN_LEN];
    char author_initials[MAX_INITIALS_LEN];
    char magazine[MAX_MAGAZINE_NAME_LEN];
    uint16_t year;        // 0 <= year <= 9999
    uint16_t mag_vol;
    uint16_t pages;
    uint32_t citations;   // <1 000 000
    uint8_t in_RSCI;      // 0/1
} Article;
