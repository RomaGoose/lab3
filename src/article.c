#include <string.h>
typedef struct _Article {
    int n; 
    char name[20];
} Article;

int cmp(void* left, void* right){
    Article* l = (Article*)left;
    Article* r = (Article*)right;

    if(l->n == r->n){
        if(strcmp(l->name,r->name) == 0){
            return 0;
        }return strcmp(l->name,r->name);
    }return l->n - r->n;
}