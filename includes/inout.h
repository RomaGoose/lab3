#pragma once

#include <stdio.h>

#include "container.h"


size_t str_to_ull(char* str);
DLList* scan_csv(FILE* input);
void print_csv(DLList* list, FILE* output);
void print_table(DLList* list, FILE* output);
void generate(size_t N, FILE* output);
