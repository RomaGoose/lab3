#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "article.h"

#define err_exit(err_message) do {\
        fprintf(stderr, "%s", err_message);\
        exit(1);} while (0) 

#define ASCII_OFFSET 48 

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


static void print_field_csv(char* field, FILE* output){
    uint8_t needs_format = (strchr(field, ',') != NULL || strchr(field, '"') != NULL);
    
    if (needs_format){
        fputc('"', output);
        for(;*field; ++field){
            if (*field == '"') fputc('"', output);
            fputc(*field, output);
        }
        fputc('"', output);
    }
    else fprintf(output, "%s", field);
}

void print_csv(Article* a, FILE* output){
    char article_name[MAX_ARTICLE_NAME_LEN+2];
    char magazine[MAX_MAGAZINE_NAME_LEN+2];
    if(strchr(a->article_name, ',') != NULL){

    }
    print_field_csv(a->article_name, output);
    fprintf(output, ",%s,%s,",
        a->author_surname,
        a->author_initials);
    print_field_csv(a->magazine, output);
    fprintf(output, ",%d,%d,%d,%d,%d\n",
        a->year,
        a->mag_vol,
        a->pages,
        a->citations,
        a->in_RSCI);
}

static uint8_t count_char(char* str, char character){
    uint8_t count = 0;
    while(*str){
        if(*str == character) ++count;
        ++str;
    }
    return count;
}

size_t str_to_ull(char* str){
    size_t ull = 0;

    for (char* c = str; *c != '\0' && *c != ','; ++c){
        if(*c < 48 || *c > 57 || c == NULL){
            return 0;
        }
        ull = 10*ull + *(c) - ASCII_OFFSET;
    }
    
    return ull;
}

void scan_csv(Article* a, FILE* input, char* buff){
    fgets(buff, MAX_ARTICLE_SIZE + MAX_ARTICLE_NAME_LEN + MAX_MAGAZINE_NAME_LEN, input);// запас на экранирование кавычек

    if(count_char(buff, ',') < NUM_OF_FIELDS - 1)
        err_exit("Некорректный ввод данных: недостаточно полей");
    if(count_char(buff, ',') > NUM_OF_FIELDS - 1 && count_char(buff, '"') == 0)
        err_exit("Некорректный ввод данных: превышено количество полей (запятые выделяйте <\">)");
    if(count_char(buff, '"') % 2 != 0)
        err_exit("Некорректный ввод данных: не все кавычки закрыты");

    field_tag field = NAME;
    uint8_t quote_opened = 0;
    uint8_t first_is_quote = 0;
    uint8_t last_is_quote = 0;
    size_t curr_index = 0;
    size_t size;
    for(size_t i = 0; i < MAX_ARTICLE_SIZE + MAX_ARTICLE_NAME_LEN + MAX_MAGAZINE_NAME_LEN; ++i){
        if(!buff[i]) break;
        if(buff[i] == '"') {
            if(i == 0 || buff[i-1] == ',') first_is_quote = 1;
            else if(buff[i+1] == ',' || buff[i+1] == '\n') last_is_quote = 1;
            if(quote_opened && buff[i+1] == '"')
                ++i;
            else 
                quote_opened = !quote_opened;
        }
        else if(buff[i] == ',' && !quote_opened || buff[i] == '\n'){
            if(first_is_quote && last_is_quote) {
                ++curr_index;
                --i;
            }
            else if(first_is_quote || last_is_quote)
                err_exit("Некорректное использование кавычек");
            
            size = i - curr_index;
            switch(field){
                case(NAME):{
                    if(size>MAX_ARTICLE_NAME_LEN-1)
                        err_exit("Некорректный ввод данных: превышена длина имени статьи");

                    size_t j = 0;
                    for(size_t i = 0; i < size; ++i, ++j){
                        if(buff[curr_index + i] == '"' && buff[curr_index + i + 1] == '"')
                            ++i;
                        a->article_name[j] = buff[curr_index + i];
                    }
                    a->article_name[j] = '\0';
                    break;
                }
                case(SURN):{
                    if(size>MAX_SURN_LEN-1)
                        err_exit("Некорректный ввод данных: превышена длина фамилии автора");
                    strncpy(a->author_surname, buff + curr_index, size);
                    a->author_surname[MAX_SURN_LEN-1] = '\0';
                    break;
                }
                case(INIT):{
                    if(size>MAX_INITIALS_LEN-1)
                        err_exit("Некорректный ввод данных: превышена длина иницаиалов автора");
                    strncpy(a->author_initials, buff + curr_index, size);
                    a->author_initials[MAX_INITIALS_LEN-1] = '\0';
                    break;
                }
                case(MAGNAME):{
                    if(size>MAX_MAGAZINE_NAME_LEN-1)
                        err_exit("Некорректный ввод данных: превышена длина названия магазина");
                    size_t j = 0;
                    for(size_t i = 0; i < size; ++i, ++j){
                        if(buff[curr_index + i] == '"' && buff[curr_index + i + 1] == '"')
                            ++i;
                        a->magazine[j] = buff[curr_index + i];
                    }
                    a->magazine[j] = '\0';
                    break;
                }
                case(YEAR):{
                    if(size>4)
                        err_exit("Некорректный ввод данных: превышено значение года публикации");
                    if(buff[curr_index] == '0' && buff[curr_index+1] == ',') a->year = 0;
                    else if(str_to_ull(buff+curr_index) == 0) err_exit("Некорректный ввод данных: неверное значение года публикации");
                    else a->year = str_to_ull(buff+curr_index);
                    break;
                }
                case(MAGVOL):{
                    if(size>5)
                        err_exit("Некорректный ввод данных: превышено значение тома журнала");
                    if(str_to_ull(buff+curr_index) == 0) 
                        err_exit("Некорректный ввод данных: неверное значение тома журнала");
                    a->mag_vol = str_to_ull(buff+curr_index);
                    break;
                }
                case(PAGES):{
                    if(size>5)
                        err_exit("Некорректный ввод данных: превышено значение страниц");
                    if(str_to_ull(buff+curr_index) == 0) 
                        err_exit("Некорректный ввод данных: неверное значение страниц");
                    a->pages = str_to_ull(buff+curr_index);
                    break;
                }
                case(CIT):{
                    if(size>5)
                        err_exit("Некорректный ввод данных: превышено значение цитирований");
                    if(buff[curr_index] == '0' && buff[curr_index+1] == ',') a->citations = 0;
                    else if(str_to_ull(buff+curr_index) == 0) err_exit("Некорректный ввод данных: неверное значение цитирований");
                    else a->citations = str_to_ull(buff+curr_index);
                    break;
                }
                case(RSCI):{
                    if(size>1 || (buff[curr_index] != '0' && buff[curr_index] != '1'))
                        err_exit("Некорректный ввод данных: введено не логическое значение входа в РИНЦ");
                    a->in_RSCI = buff[curr_index] == '1' ? 1 : 0;
                    break;
                }
                default: err_exit("Некорректный ввод данных: ошибка чтения");
            }
            ++field;
            if(first_is_quote && last_is_quote) {
                ++i;
                first_is_quote = 0;
                last_is_quote = 0;
            }
            curr_index = i+1;
        }
    }
}

//alsadl k laks l,lobankov,a.f.,"ajo neg",2025,5,100,10,1
//"alsadl k ""laks"" l",lobankov,a.f.,"ajo neg",2025,5,100,10,1