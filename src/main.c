#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "container.h"
#include "article.h"
#include "selection_sort.h"

typedef struct _Article {int n; char name[20];} Article;
void parse_args(int argc, char** argv);
int main(int argc, char** argv){
    setlocale(LC_ALL, "RU_ru.UTF-8");

    //DLList* list = init_DLList(sizeof(Article));
    Article r1 = {.n = 7, .name = "baboon"};
    Article r2 = {.n = 7, .name = "australopithecus"};
    Article r3 = {.n = 9, .name = "123456789012345678"};
    Article new = {.n = 10, .name = "set"};
    
    // insert_start(&r1, list);
    // insert_end(&r3, list);
    // insert_(0, &r2, list);

    Article arts[] = {r2,new,r3,r1};
    DLList* list = convert_array_to_list(arts, 4, sizeof(arts[0]));

/*
    printf("list: \n%d\n%d\n%d\n --- \n", ((Article*)get_element(0, list))->n, 
                                    ((Article*)get_element(1, list))->n, 
                                    ((Article*)get_element(2, list))->n);
                           
    swap(0, 1, list);
    
    printf("swapped: \n%d\n%d\n%d\n ---\n array:\n",   ((Article*)get_element(0, list))->n, 
                                        ((Article*)get_element(1, list))->n, 
                                        ((Article*)get_element(2, list))->n);

    Article* arr = convert_list_to_array(list);

    for(int i = 0; i < get_size(list); i++){
        printf("%d -- %s\n", arr[i].n, arr[i].name);
    }

    DLList* copy_list = convert_array_to_list(arr, get_size(list), sizeof(Article));
    puts("copied list:");
    for(int i = 0; i < get_size(list); i++){
        printf("%d -- %s\n", ((Article*)get_element(i, copy_list))->n, ((Article*)get_element(i, copy_list))->name);
    }

    clear_list(copy_list);

    insert_start(&r3, copy_list);
    insert_(0, &r2, copy_list);

    kill_list(copy_list);

    remove_(1, list);
    remove_end(list);
    remove_start(list);
    
    
    Iterator* i;
    for(i = begin(list); get_pos(i) < get_size(list);next(i)){
        if (get_pos(i)==1) set(&new, i);
        printf("%d %s\n", ((Article*)get(i))->name);
    }
    end(i);
    */
    /*
    Iterator* i;
    Article* a;
    puts("list:");
    for (i = begin(list); get_pos(i) < get_size(list); next(i)){
        a = get(i);
        printf("%d - %s \n", a->n, a->name);
    }

    sel_sort(list, cmp);

    puts("sorted:");
    for (i = begin(list); get_pos(i) < get_size(list); next(i)){
        a = get(i);
        printf("%d - %s \n", a->n, a->name);
    }
    */

    parse_args(argc, argv);

    kill_list(list);
    puts("success");
    return 0;
}