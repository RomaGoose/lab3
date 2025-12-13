#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "article_internal.h"
#include "container.h"
#include "inout_internal.h"

/**
 * @brief форматированный вывод поля с возможными разделителями, в формате .csv
 * @param field поле
 * @param output поток вывода 
 */
static void print_field_csv(char* field, FILE* output){
    uint8_t needs_format = (strchr(field, ',') != NULL  || strchr(field, '"') != NULL || 
                            strchr(field, '\t') != NULL || strchr(field, ';') != NULL || 
                            strchr(field, ' ') != NULL);
    
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

static void clean_string(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if ((unsigned char)*src >= 32)
            *dst++ = *src;
        src++;
    }
    *dst = '\0';
}

void printline_csv(Article* a, FILE* output){

    clean_string(a->author_surname);
    clean_string(a->article_name);
    clean_string(a->author_initials);
    clean_string(a->magazine);

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

void print_csv(DLList* list, FILE* output){
    Iterator* i;
    fprintf(output, "%s", "\"Название статьи\",Фамилия,Инициалы,Журнал,\"Год выпуска\",Том,Страницы,Цитирования,\"В РИНЦ\"\n");
    for(i = begin(list); get_pos(i) < get_size(list); next(i))
        printline_csv(iterator_get(i), output);
    free_iterator(i);
}

static int scanline_csv(Article* a, FILE* input, char* buff);
DLList* scan_csv(FILE* input){
    DLList* list = init_DLList(sizeof(Article));

    Article* a;
    
    char buff[MAX_INPUT_LEN];
    fgets(buff, MAX_INPUT_LEN, input); //пропускаем шапку

    a = malloc(sizeof(Article));
    while(scanline_csv(a, input, buff) != -1){
        mem_check_exit(a);
        insert_end(a, list);
        a = malloc(sizeof(Article));
    }
    free(a);

    return list;
}

static size_t count_digits(size_t n) {
    int d = 0;
    while (n!=0){
        d++;
        n /= 10;
    }
    return d;
}

void print_table(DLList* list, FILE* output){

    size_t max_num_len = count_digits(get_size(list));
    fwprintf(output, L" %*ls %-40.40ls %-23.23ls %-30.30ls %-4ls %-5ls %-5ls %-6ls %-4ls\n", 
        max_num_len, L" ", 
        L"Название статьи", 
        L"Первый автор", 
        L"Название журнала",
        L"Год",
        L"Том",
        L"Стр",
        L"Цитир",
        L"РИНЦ");

    Article* a;
    Iterator* i;
    wchar_t name[MAX_ARTICLE_NAME_LEN];
    wchar_t sur[MAX_SURN_LEN];
    wchar_t init[MAX_INITIALS_LEN];
    wchar_t mag[MAX_MAGAZINE_NAME_LEN];
    
    for(i = begin(list); get_pos(i) < get_size(list); next(i)){

        a = iterator_get(i);
        
        clean_string(a->author_surname);
        clean_string(a->article_name);
        clean_string(a->author_initials);
        clean_string(a->magazine);
        wmemset(name, L'\0', MAX_ARTICLE_NAME_LEN);
        wmemset(sur, L'\0', MAX_SURN_LEN);
        wmemset(init, L'\0', MAX_INITIALS_LEN);
        wmemset(mag, L'\0', MAX_MAGAZINE_NAME_LEN);
        mbstowcs(name, a->article_name, MAX_ARTICLE_NAME_LEN);
        mbstowcs(sur, a->author_surname, MAX_SURN_LEN);
        mbstowcs(init, a->author_initials, MAX_INITIALS_LEN);
        mbstowcs(mag, a->magazine, MAX_MAGAZINE_NAME_LEN);

        if(wcslen(name) > MAX_NAME_WIDTH){
            wcsncpy(name + MAX_NAME_WIDTH - 3, L"...", 3);
            name[MAX_NAME_WIDTH] = L'\0';
        }
        if(wcslen(mag) > MAX_MAGAZINE_WIDTH){
            wcsncpy(mag + MAX_MAGAZINE_WIDTH - 3, L"...", 3);
            mag[MAX_MAGAZINE_WIDTH] = L'\0';
        }
        
        fwprintf(output, L" %*llu %-40.40ls %ls %ls%*ls %-30.30ls %*d %*d %*d %*d %-4ls\n", 
            max_num_len, get_pos(i) + 1, 
            name, 
            sur, 
            init, 23 - wcslen(sur) - wcslen(init) - 1, L" ",
            mag,
            4, a->year,
            5, a->mag_vol,
            5, a->pages,
            6, a->citations,
            a->in_RSCI ? L"YES" : L"NO");
    }
    free_iterator(i);
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
        ull = 10*ull + *(c) - ASCII_DIGITS_OFFSET;
    }
    
    return ull;
}

/**
 * @brief Чтение данных и заполнение ими структуры статьи из формате .csv
 * @param a указатель на статью
 * @param input поток ввода 
 * @param buff буфер для чтения строки 
 * @return -1 если достигнут конец файла, 0 в противном случае
 */
static int scanline_csv(Article* a, FILE* input, char* buff){
    if(fgets(buff, MAX_INPUT_LEN, input)==NULL)
        return -1;                    

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
    for(size_t i = 0; i < MAX_INPUT_LEN; ++i){
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
                    if(size>6)
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
    return 0;
}