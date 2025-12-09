#include <stdio.h>

#include "article.h"
#include "container.h"

size_t str_to_ull(char* str);
void printline_csv(Article* a, FILE* output);
void scanline_csv(Article* a, FILE* input, char* buff);
void scan_csv(DLList* list, FILE* input);
void print_csv(DLList* list, FILE* output);
void print_table(DLList* list, FILE* output);
