#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <limits.h>

#include "article_internal.h"
#include "inout_internal.h"
#include "inout.h"
#include "generate.h"   

static uint32_t _rand(){
    return rand()*rand();
}

/**
 * @brief Генерирация набора слов из wordlist, не превосходящяя по длине size  
 * @param size максимальная длина набора слов
 * @param wordlist массив строк слов
 * @return строка с набором слов 
 */
static char* word_gen(size_t size, char** wordlist){
    int word_num = _rand() % (MAX_WORD_NUM(size)) + 1;
    int cur_len = 0;
    char* tmp = malloc(size);
    mem_check_exit(tmp);
    
    for (int j = 0; j < word_num; j++) {
        char* rand_word = wordlist[_rand() % WORDLIST_LEN];
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

static char** words;
static char** surnames;

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

    file_check_exit(file, path);
    
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

void read_lists(){
    words = read_list(WORDS);
    surnames = read_list(SURNS);
}
void free_lists(){
    for(size_t i = 0; i < WORDLIST_LEN; ++i)
        free(words[i]);
    free(words);
    for(size_t i = 0; i < SURNLIST_LEN; ++i) 
        free(surnames[i]);
    free(surnames);
}

void generate(size_t N, FILE* output){
    srand(time(NULL));

    Article* tmp = malloc(sizeof(Article));
    mem_check_exit(tmp);

    fprintf(output, "%s", "\"Название статьи\",Фамилия,Инициалы,Журнал,\"Год выпуска\",Том,Страницы,Цитирования,\"В РИНЦ\"\n");

    for(size_t i = 0; i < N; ++i){
        char* art_name = word_gen(MAX_ARTICLE_NAME_LEN, words);
        char* mag_name = word_gen(MAX_MAGAZINE_NAME_LEN, words);
        
        strncpy(tmp->article_name, art_name, MAX_ARTICLE_NAME_LEN);
        strncpy(tmp->author_surname, surnames[_rand() % SURNLIST_LEN], MAX_SURN_LEN);
        strncpy(tmp->magazine, mag_name, MAX_MAGAZINE_NAME_LEN);
        sprintf(tmp->author_initials, "%.2s.%.2s.", surnames[_rand() % SURNLIST_LEN], surnames[_rand()%SURNLIST_LEN]);

        tmp->year = _rand() % MAX_YEAR + 1;
        tmp->mag_vol = _rand() % UINT16_MAX + 1;
        tmp->pages = _rand() % UINT16_MAX + 1;
        tmp->citations = _rand() % MAX_CITATIONS;
        tmp->in_RSCI = _rand() % 2;

        free(art_name);
        free(mag_name);

        printline_csv(tmp, output); 
    }
    free(tmp);
}