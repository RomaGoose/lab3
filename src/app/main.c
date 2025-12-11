#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include "parse_args.h"
#include "inout.h"
#include "article.h"

#define NUM_OF_ITERATIONS 3

int main(int argc, char** argv){
    setlocale(LC_ALL, "ru_RU.UTF-8");

    read_lists();

    if (argc == 1){
        FILE* quick_log = fopen("quick_log.csv", "w"); 
        FILE* select_log = fopen("select_log.csv", "w"); 
        FILE* gen;

        unsigned int data_num[] = {
            100, 250, 500, 750,
            1000, 1250, 1500, 1750, 
            2000, 2250, 2500, 2750,
            3000
        };

        clock_t start, end;
        double time_spent;
        DLList* list;
        for(int d = 0; d < sizeof(data_num)/sizeof(data_num[0]); ++d){
            for(int i = 0; i < NUM_OF_ITERATIONS; ++i){
                gen = fopen("gen.csv", "w");
                generate(data_num[d], gen);
                fclose(gen);
                gen = fopen("gen.csv", "r");
                DLList* list = scan_csv(gen);
                fclose(gen);

                start = clock();
                sel_sort(list, cmp);
                end = clock();
                fprintf(select_log, "%u,%llu\n", data_num[d], (size_t)(end-start));
                fflush(select_log);

                start = clock();
                quick_sort(list, cmp, 0, get_size(list)-1);
                end = clock();
                fprintf(quick_log, "%u,%llu\n", data_num[d], (size_t)(end-start));
                fflush(quick_log);
            }
        }
        
        fclose(quick_log);
        fclose(select_log);
        fclose(gen);
    }
    else{
    parse_args(argc, argv);
    }
    free_lists();
    puts("success");
    return 0;
}