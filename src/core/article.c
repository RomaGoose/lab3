#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "article_internal.h"

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

int cmp_d(void* left, void* right){ return -cmp(left, right); }
