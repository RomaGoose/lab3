#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#include "article.h"
#include "inout.h"

#define WORDLIST_LEN 378343
#define WORDLIST_PATH "wordlists/words.txt"
#define MAX_WORD_LEN 32*2
#define SURNLIST_LEN 99072
#define SURNLIST_PATH "wordlists/surnames.txt"
#define MAX_SURN_LEN 18*2

#define MAX_WORD_NUM(size) size/7

#define mem_check_exit(x) do {                          \
    if(x == NULL) {                                     \
    fprintf(stderr, "%s", "Не удалось выделить память");\
        exit(1);                                        \
    }} while (0)

typedef enum _wordlist_t { WORDS, SURNS } wordlist_t;

static char* word_gen(uint8_t size, char** wordlist){
    int word_num = rand() % (MAX_WORD_NUM(size)) + 1;
    int cur_len = 0;
    char* tmp = malloc(size);
    mem_check_exit(tmp);
    
    for (int j = 0; j < word_num; j++) {
        char* rand_word = wordlist[rand() % WORDLIST_LEN];
        int rword_len = strlen(rand_word);
        
        if (cur_len + rword_len + (j > 0 ? 1 : 0) + 1 > size) 
            break;
       
        if (j > 0)
            tmp[cur_len++] = ' ';
        
        strcpy(tmp + cur_len, rand_word);
        cur_len += rword_len;
    }
    tmp[cur_len] = '\0';

    return tmp;
}

static char** read_list(wordlist_t type){
    setlocale(LC_ALL, "RU_ru.UTF-8");
    char* path;
    size_t file_len;
    size_t max_word_len;
    switch(type){
        case WORDS:{
            path = WORDLIST_PATH;
            file_len = WORDLIST_LEN;
            max_word_len = MAX_WORD_LEN;
            break;
        }
        case SURNS:{
            path = SURNLIST_PATH;
            file_len = SURNLIST_LEN;
            max_word_len = MAX_SURN_LEN;
            break;
        }
    }

    FILE* file = fopen(path, "r");

    if (!file) {
        fprintf(stderr, "%s:%s", "Не удалось открыть файл:", path); 
        exit(1);
    }
    
    char** words = malloc(file_len * sizeof(char*));
    mem_check_exit(words);
    for (int i = 0; i < file_len; i++) {
        words[i] = malloc(max_word_len * sizeof(char));
        mem_check_exit(words[i]);
    }
    
    size_t count = 0;
    while (count < file_len && fgets(words[count], max_word_len, file)) {
        words[count][strcspn(words[count], "\n")] = '\0'; 
        count++;
    }
    fclose(file);
    
    return words;
}

void generate(size_t N, FILE* output){
    srand(time(NULL));
    Article* tmp = malloc(sizeof(Article));
    mem_check_exit(tmp);
    char** words = read_list(WORDS);
    char** surnames = read_list(SURNS);

    for(size_t i = 0; i < N; ++i){
        char* art_name = word_gen(MAX_ARTICLE_NAME_LEN, words);
        char* mag_name = word_gen(MAX_MAGAZINE_NAME_LEN, words);
        char first_initial[3] = {[2] = '\0'};
        char second_initial[3] = {[2] = '\0'};
        sprintf(first_initial, "%.2s", surnames [rand() % SURNLIST_LEN]);
        sprintf(second_initial, "%.2s", surnames[rand() % SURNLIST_LEN]);

        strncpy(tmp->article_name, art_name, MAX_ARTICLE_NAME_LEN);
        strncpy(tmp->author_surname, surnames[rand() % SURNLIST_LEN], MAX_SURN_LEN);
        strncpy(tmp->magazine, mag_name, MAX_MAGAZINE_NAME_LEN);
        sprintf(tmp->author_initials, "%s.%s.", first_initial, second_initial);

        tmp->year = rand() % 10000;
        tmp->mag_vol = rand() % ((1 << 16) - 1);
        tmp->pages = rand() % ((1 << 16) - 1);
        tmp->citations = rand() % ((1 << 16) - 1);
        tmp->in_RSCI = rand() % 2;

        free(art_name);
        free(mag_name);

        print_csv(tmp, output);  
        
        // fprintf(output, "name: %s\ninit: %s\nsurn: %s\nmag: %s\ncit: %d\npages: %d\nyear: %d\nrcsi: %d\nmagvol: %d\n\n",
        //     tmp->article_name,
        //     tmp->author_surname,
        //     tmp->author_initials,
        //     tmp->magazine,
        //     tmp->year,
        //     tmp->mag_vol,
        //     tmp->pages,
        //     tmp->citations,
        //     tmp->in_RSCI);
    }

    free(tmp);
    for(size_t i = 0; i < WORDLIST_LEN; ++i)
        free(words[i]);
    free(words);
    for(size_t i = 0; i < SURNLIST_LEN; ++i) 
        free(surnames[i]);
    free(surnames);
}