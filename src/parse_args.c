#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LONG_NAME_OFFSET 3
#define mem_check_exit(x) do {                             \
    if(x == NULL) {                                        \
        fprintf(stderr, "%s", "UNABLE TO ALLOCATE MEMORY");\
        exit(1);                                           \
    }} while (0)
#define ASCII_OFFSET 48 
#define clear() do{char c; while((c=getchar())!=EOF && c != '\n');} while(0)

typedef enum { ASC, DESC } sort_type_tag;
typedef enum {
    GEN = 1,
    PRINT,
    SORT,
    TYPE,
    IN,
    OUT,

    END = 0
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

static Argument arguments[] = {
    {GEN, "generate", 'g', " <N>", " <N>", "Генерирация и вывод N записей"},
    {PRINT, "print", 'P', "", "", "Cчитывает данные и выводит их в формате таблицы"},
    {SORT, "sort", 's',  "", "", "Принимает на ввод, сортирует и выводит данные"},
    {TYPE, "type", 't', "=<asc/desc>", " <A/D>", "Выбор типа сортировки: asc или A - по возрастанию, desc или D - по убыванию"},
    {IN, "in", 'i',  "=\"<имя_файла>\"", " <имя_файла>", "Выбор файла для данных на ввод"},
    {OUT, "out", 'o', "=\"<имя_файла>\"", " <имя_файла>", "Выбор файла для выходных данных"},
    
    {END, NULL, '\0', NULL, NULL, NULL}
};

static void help_arg(Argument* a);
static void help_all();

static void process_arg(Argument* a, void* value, State* state);
static void process_state(State* state);
static char** format_argv(int argc, char** argv);
void parse_args(int argc, char** argv){
    char** fargv = format_argv(argc, argv);

    if (argc == 1){
        help_all();
        exit(1);
    }
    
    State* state = malloc(sizeof(state));
    state->task = END;
    state->input = stdin;
    state->output = stdout;

    char arg_is_found, prev_is_arg = 0;
    Argument* a;
    for(size_t i = 1; fargv[i] != NULL; ++i){
        arg_is_found = 0;
        if(fargv[i][0] == '-'){
            for(a = arguments; a->tag != END; ++a){
                if(a->name == fargv[i][1]|| strcmp(a->long_name, &fargv[i][2]) == 0){
                    arg_is_found = 1;
                    prev_is_arg = 1;
                    process_arg(a, fargv[i+1], state);
                    break;
                }
            }
            if (!arg_is_found) {help_arg(a); exit(1);}
        }
        else{
            if(prev_is_arg) prev_is_arg = 0;
            else{
            puts("Ошибка аргументов командной строки");
            help_all();
            exit(1);
            }
        }
    }

    process_state(state);
}

/* попытки выпросиьт нормальный тип сортировки 
            char new_value[5];
            char got_type = 0;
            int sec = scanf("Введено некорректное значение для аргумента type\n"
                                "Введите снова: %s\n", new_value);
            if(strcmp(new_value, "desc") == 0){
                state->task_value.sort_type = DESC;
                got_type = 1;
                break;
            }
            if(strcmp(new_value, "asc") == 0){
                got_type = 1;
                break;
            }
            while (got_type == 0){
                clear();
                puts("Опять мимо\n");
                help_arg(arguments + TYPE);
                sec = scanf("Введите снова: %s\n", new_value);
                if(strcmp(new_value, "desc") == 0){
                    state->task_value.sort_type = DESC;
                    got_type = 1;
                    break;
                }
                if(strcmp(new_value, "asc") == 0){
                    got_type = 1;
                }
            }*/

static size_t str_to_ull(char* str);
static void process_arg(Argument* a, void* value, State* state){    
    switch(a->tag){
        case GEN:{
            if(state->task != END){
                puts("Введено несколько режимов работы");
                exit(1);
            }

            size_t N = str_to_ull(value);
            if(N==0){
                int sec = scanf("Введено некорректное значение для аргумента generate\n"
                                "Введите снова: %llu\n", N);
                while (sec != 1){
                    clear();
                    puts("Опять мимо\n");
                    help_arg(arguments + GEN);
                    sec = scanf("Введите снова: %llu\n", N);
                }
            }
            state->task = GEN;
            state->task_value.N = N;
            break;
        }
        case PRINT:{
            if(state->task != END){
                puts("Введено несколько режимов работы");
                exit(1);
            }
            state->task = PRINT;
            break;
        }
        case SORT:{
            if(state->task != END){
                puts("Введено несколько режимов работы");
                exit(1);
            }
            state->task = SORT;
            state->task_value.sort_type = ASC;
            break;
        }
        case TYPE:{
            if(strcmp(value, "desc") == 0 || *(char* )value == 'D'){
                state->task_value.sort_type = DESC;
                break;
            }
            if(strcmp(value, "asc") == 0 || *(char* )value == 'A'){
                break;
            }
            else {
                puts("Введён некорректный тип сортировки"); 
                help_arg(arguments + TYPE);
                exit(1);
            }
            break;
        }
        case IN:{
            state->input = fopen(value, "r");
            if(state->input == NULL){
                if(*(char*)value == '-'){
                    puts("Введено некорректное название файла для ввода.");
                    help_arg(arguments + IN);
                    exit(1);
                }
                else if(*(char*)value == '"'){
                    state->input = fopen((char* )value + 1, "r");
                    if (state->input == NULL){
                         puts("Введено некорректное название файла для ввода");
                        help_arg(arguments + IN);
                        exit(1);
                    }
                }
            }
            break;
        }
        case OUT:{
            state->output = fopen(value, "w");
            if(state->output == NULL){
                if(*(char*)value == '-'){
                    puts("Введено некорректное название файла для вывода.");
                    help_arg(arguments + IN);
                    exit(1);
                }
                else if(*(char*)value == '"'){
                    state->output = fopen((char* )value + 1, "w");
                    if (state->output == NULL){
                         puts("Введено некорректное название файла для вывода");
                        help_arg(arguments + IN);
                        exit(1);
                    }
                }
            }
            break;
        }
    }
}

static void process_state(State* state){
    switch (state->task){
        case GEN:{
            fprintf(state->output, "Сгенерировал %d записей", state->task_value.N);
            break;
        }
        case SORT:{
            fprintf(state->output, "Отсортировал, код сортировки: %d", state->task_value);
            break;
        }
        case PRINT:{
            fprintf(state->output, "Запринтил чето");
            break;
        }
    }
}

static char* copy_str_trim(char* src, char divider);
static char** format_argv(int argc, char** argv){
    char** f_argv = malloc((argc+1)*sizeof(char*));
    mem_check_exit(f_argv);
    f_argv[0] = argv[0];
    unsigned char found_splits = 0;
    for(size_t s = 1; s < argc; ++s){
        f_argv[s+found_splits] = argv[s];
        for(size_t c = LONG_NAME_OFFSET; argv[s][c] != '\0'; ++c){
            if(argv[s][c] == '='){
                f_argv[s+found_splits] = copy_str_trim(argv[s], '=');
                ++found_splits;
                f_argv = realloc(f_argv, (argc+found_splits+1)*sizeof(char*));
                mem_check_exit(f_argv);
                if(argv[s][c+1] == '"')
                    f_argv[s+found_splits] = copy_str_trim(&argv[s][c+1], '"');
                else
                    f_argv[s+found_splits] = &argv[s][c+1];
            }
        }
    }
    f_argv[argc + found_splits] = NULL;
    return f_argv;
}

static size_t str_to_ull(char* str){
    size_t ull = 0;

    for (char* c = str; *c != '\0'; ++c){
        if(*c < 48 || *c > 57 || c == NULL){
            return 0;
        }
        ull = 10*ull + *(c) - ASCII_OFFSET;
    }
    
    return ull;
}

static char* copy_str_trim(char* src, char divider){
    size_t div_i;
    for (div_i = 1; src[div_i] != divider && src[div_i] != '\0'; ++div_i);
    if (src[div_i] == '\0') return NULL;

    char* tmp = malloc((div_i+1)*sizeof(char));
    mem_check_exit(tmp);
    tmp[div_i] = '\0';
    memcpy(tmp, src, div_i);

    return tmp;
}

static void help_arg(Argument* a){
    printf("--%s%s ИЛИ -%c%s ДЛЯ: %s\n", a->long_name, a->long_arg_value, a->name, a->short_arg_value, a->description);
}

static void help_all(){
        puts("ДОСТУПНЫЕ ФЛАГИ:");
        for(Argument* a = (Argument*)arguments; a->tag != END; ++a)
            help_arg(a);
}
