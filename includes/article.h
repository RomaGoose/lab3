#include <stdint.h>

#define MAX_SURN_LEN 18*2
#define MAX_ARTICLE_NAME_LEN 120*2
#define MAX_MAGAZINE_NAME_LEN 48*2
#define MAX_INITIALS_LEN 12

#define NUM_OF_FIELDS 9

#define MAX_ARTICLE_SIZE MAX_ARTICLE_NAME_LEN + MAX_SURN_LEN  + \ 
        MAX_INITIALS_LEN + MAX_MAGAZINE_NAME_LEN - 1 + 6 + 7*3 + 1 + 2\

typedef struct _Article {
    char article_name[MAX_ARTICLE_NAME_LEN];
    char author_surname[MAX_SURN_LEN];
    char author_initials[MAX_INITIALS_LEN];
    char magazine[MAX_MAGAZINE_NAME_LEN];
    uint16_t year;
    uint16_t mag_vol;
    uint16_t pages;
    uint16_t citations;
    uint8_t in_RSCI;
} Article;

int cmp(void* left, void* right);