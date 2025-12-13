#include <string.h>

#include "inout.h"
#include "parse_args.h"
#include "article.h"


static Argument arguments[] = {
    {GEN, "generate", 'g', " <N>", " <N>", "Генерирация и вывод N записей"},
    {PRINT, "print", 'P', "", "", "Cчитывает данные и выводит их в формате таблицы"},
    {SORT, "sort", 's',  "", "", "Принимает на ввод, сортирует и выводит данные"},
    {TYPE, "type", 't', "=<asc/desc>", " <A/D>", "Выбор типа сортировки: asc или A - по возрастанию, desc или D - по убыванию"},
    {ALG, "algorithm", 'a', "=<sel/quick>", " <S/Q>", "Выбор алгоритма сортировки: sel или S - выбором, quick или Q - быстрая (Хоара)"},
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
    if (argc == 1){
        help_all();
        exit(1);
    }
    
    char** fargv = format_argv(argc, argv);
    
    State* state = malloc(sizeof(State));
    state->task = END;
    state->input = stdin;
    state->output = stdout;

    char arg_is_found, value_expected = 0;
    Argument* a;
    for(size_t i = 1; fargv[i] != NULL; ++i){
        arg_is_found = 0;
        if(fargv[i][0] == '-' && value_expected == 0){
            for(a = arguments; a->tag != END; ++a){
                if(a->name == fargv[i][1]|| strcmp(a->long_name, &fargv[i][2]) == 0){
                    arg_is_found = 1;
                    value_expected = a->tag == PRINT || a->tag == SORT ? 0 : 1;
                    process_arg(a, fargv[i+1], state);
                    break;
                }
            }
            if (!arg_is_found) {help_all(); exit(1);}
        }
        else{
            if(value_expected) value_expected = 0;
            else{
            puts("Ошибка аргументов командной строки");
            help_all();
            exit(1);
            }
        }
    }

    for(size_t i = 0; fargv[i] != NULL; ++i) free(fargv[i]);
    free(fargv);
    process_state(state);
    free(state);
}

/** 
 * @brief Обработка аргумента командной строки. Заполнение информации о состоянии приложения
 * @param a аргумент 
 * @param value указатель на значение аргумента
 * @param state указатель на состояние приложения
 */ 
static void process_arg(Argument* a, void* value, State* state){    
    switch(a->tag){
        case GEN:{
            if(state->task != END){
                puts("Введено несколько режимов работы");
                exit(1);
            }

            size_t N = str_to_ull(value);
            if(N==0){
                puts("Введено некорректное значение аргумента generate");
                exit(1);
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
            state->task_value.sort_value.sort_type = ASC;
            state->task_value.sort_value.sort_alg = QUICK;
            break;
        }
        case TYPE:{
            if(state->task != SORT){
                puts("Выбор типа сортировки, в то время, как режим работы не сортировка.");
                exit(1);
            }
            if(strcmp(value, "desc") == 0 || *(char* )value == 'D'){
                state->task_value.sort_value.sort_type = DESC;
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
        case ALG:{
            if(state->task != SORT){
                puts("Выбор алгоритма сортировки, в то время, как режим работы не сортировка.");
                exit(1);
            }
            if(strcmp(value, "sel") == 0 || *(char* )value == 'S'){
                state->task_value.sort_value.sort_alg = SEL;
                break;
            }
            if(strcmp(value, "quick") == 0 || *(char* )value == 'Q'){
                break;
            }
            else {
                puts("Введён некорректный тип сортировки"); 
                help_arg(arguments + ALG);
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

/** 
 * @brief Обработка состояния приложения
 * @param state указатель на состояние приложения
 */ 
static void process_state(State* state){
    switch (state->task){
        case GEN:{
            generate(state->task_value.N, state->output);
            puts("Успешно сгенерировал");
            break;
        }
        case SORT:{
            DLList* list = scan_csv(state->input);
            if(state->task_value.sort_value.sort_type == ASC){
                if(state->task_value.sort_value.sort_alg == SEL)
                    sel_sort(list, cmp);
                else
                    quick_sort(list, cmp);
            }
            if(state->task_value.sort_value.sort_type == DESC){
                if(state->task_value.sort_value.sort_alg == SEL)
                    sel_sort(list, cmp_d);
                else
                    quick_sort(list, cmp_d);
            }
            print_csv(list, state->output);
            kill_list(list);
            break;
        }
        case PRINT:{
            DLList* list = scan_csv(state->input);
            print_table(list, state->output);
            kill_list(list);
            break;
        }
    }
}

static char* copy_str_trim(char* src, char divider);
/**
 * @brief Форматирование аргументов командной строки
 * @param argc количество исходных аргументов
 * @param argv массив строк исходных аргументов
 * @return массив строк отформатированных аргументов
 */
static char** format_argv(int argc, char** argv){
    char** f_argv = malloc((MAX_F_ARGC+1)*sizeof(char*));
    mem_check_exit(f_argv);

    f_argv[0] = strdup(argv[0]);
    
    unsigned char found_splits = 0;
    for(size_t s = 1; s < argc; ++s){
        f_argv[s+found_splits] = strdup(argv[s]);
    
        for(size_t c = 0; argv[s][c] != '\0'; ++c){
            if(argv[s][c] == '='){
                free(f_argv[s+found_splits]);
                f_argv[s+found_splits] = copy_str_trim(argv[s], '=');
                
                ++found_splits;
                if(argv[s][c+1] == '"')
                    f_argv[s+found_splits] = copy_str_trim(&argv[s][c+2], '"');
                else
                    f_argv[s+found_splits] = strdup(&argv[s][c+1]);
            }
        }
    }
    f_argv[argc + found_splits] = NULL;
    
    return f_argv;
}

/**
 * @brief Копирование строки от src до первого разделителя (divider)
 * @param src указатель на начало строки источника
 * @param divider разделитель
 * @return указатель на обрезанную строку 
 */
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
