#pragma ocne

#include <stdlib.h>
#include <stdio.h>

#define MAX_F_ARGC 8
#define mem_check_exit(x) do {                             \
    if(x == NULL) {                                        \
        fprintf(stderr, "%s", "UNABLE TO ALLOCATE MEMORY");\
        exit(1);                                           \
    }} while (0)
#define clear() do{char c; while((c=getchar())!=EOF && c != '\n');} while(0)

typedef enum { ASC, DESC } sort_type_tag;
typedef enum {
    GEN,
    PRINT,
    SORT,
    TYPE,
    IN,
    OUT,

    END
} arg_tag;

typedef struct _State {
    arg_tag task;
    union {
        size_t N;
        sort_type_tag sort_type;
    } task_value;
    FILE* input;
    FILE* output;
} State;

typedef struct _Argument {
    arg_tag tag;
    const char* long_name;
    char name;
    const char* long_arg_value;
    const char* short_arg_value;
    const char* description;
} Argument;

void parse_args(int argc, char** argv);