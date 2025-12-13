#pragma once

#include <stdio.h>

#define WORDLIST_LEN 378343
#define WORDLIST_PATH "wordlists/words.txt"
#define MAX_WORD_LEN 32*2
#define SURNLIST_LEN 99072
#define SURNLIST_PATH "wordlists/surnames.txt"
#define MAX_SURN_LEN 18*2

#define MAX_WORD_NUM(size) size/14 ///< максимальное количество слов в строке длиной size

#define mem_check_exit(x) do {                          \
    if(x == NULL) {                                     \
    fprintf(stderr, "%s", "Не удалось выделить память");\
        exit(1);                                        \
    }} while (0)

typedef enum _wordlist_t { WORDS, SURNS } wordlist_t;
