#pragma once

#include <stdio.h>

#define WORDLIST_LEN 378343
#define WORDLIST_PATH "wordlists/words.txt"
#define MAX_WORD_LEN 32*2
#define SURNLIST_LEN 99072
#define SURNLIST_PATH "wordlists/surnames.txt"
#define MAX_SURN_LEN 18*2

#define MAX_WORD_NUM(size) size/14 ///< максимальное количество слов в строке длиной size

typedef enum _wordlist_t { WORDS, SURNS } wordlist_t;
