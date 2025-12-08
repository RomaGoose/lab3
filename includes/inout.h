#include <stdio.h>

#include "article.h"

size_t str_to_ull(char* str);
void print_csv(Article* a, FILE* output);
void scan_csv(Article* a, FILE* input, char* buff);
