#include <stdio.h>
#include <stdlib.h>

#include "container.h"

typedef struct _Research {int n; char name[20];} Research;

int main(){
    DLList* list = init_DLList(sizeof(Research));
    Research r1 = {.n = 5, .name = "baboon"};
    Research r2 = {.n = 7, .name = "australopithecus"};
    Research r3 = {.n = 9, .name = "123456789012345678"};
    
    insert_start(&r1, list);
    insert_end(&r2, list);
    insert_(0, &r3, list);

    printf("array: \n%d\n%d\n%d\n --- \n",  ((Research*)get_element(0, list))->n, 
                                    ((Research*)get_element(1, list))->n, 
                                    ((Research*)get_element(2, list))->n);
                           
    swap(0, 1, list);
    
    printf("swapped: \n%d\n%d\n%d\n --- \n",   ((Research*)get_element(0, list))->n, 
                                        ((Research*)get_element(1, list))->n, 
                                        ((Research*)get_element(2, list))->n);

    Research* arr = convert_list_to_array(list);

    for(int i = 0; i < get_size(list); i++){
        printf("%d -- %s\n", arr[i].n, arr[i].name);
    }

    DLList* copy_list = convert_array_to_list(arr, get_size(list), sizeof(Research));
    puts("copied list:");
    for(int i = 0; i < get_size(list); i++){
        printf("%d -- %s\n", ((Research*)get_element(i, copy_list))->n, ((Research*)get_element(i, copy_list))->name);
    }


    remove_(1, list);
    remove_end(list);
    remove_start(list);

    kill_list(list);
    puts("success");
    return 0;
}