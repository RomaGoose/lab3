#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct _Article {
    char article_name[120];
    char author_surname[24];
    char author_initials[8];
    char magazine[50];
    uint16_t year;
    uint16_t mag_vol;
    uint16_t pages;
    uint16_t citations;
    uint8_t in_RSCI;
} Article;

int cmp(void* left, void* right){
    Article* l = (Article*)left;
    Article* r = (Article*)right;

    
    if(l->citations == r->citations){
        if(l->in_RSCI == r->in_RSCI){
            if(l->year == r->year){
                if(l->pages == r->pages){
                    if(strcmp(l->article_name, r->article_name)){
                        if(strcmp(l->author_surname,r->author_surname) == 0){
                            return 0;
                        }return strcmp(l->author_surname,r->author_surname);
                    }return l->article_name - r->article_name;
                }return l->pages - r->pages;
            }return l->year - r->year;
        }return l->in_RSCI - r->in_RSCI;
    }return l->citations - r->citations;
}