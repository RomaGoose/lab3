#include <stdio.h>
#include <stdlib.h>

#include "container.h"

typedef struct _Research {int n;} Research;

int main(){
    DLList* list = init_DLList();
    Research r1 = {.n = 5};
    Research r2 = {.n = 7};
    Research r3 = {.n = 9};
    
    insert_start(&r1, list);
    insert_end(&r2, list);
    insert_(0, &r3, list);

    printf("%d\n%d\n%d\n", ((Research*)get_element(0, list))->n, 
                           ((Research*)get_element(1, list))->n, 
                           ((Research*)get_element(2, list))->n);

    remove_(1, list);
    remove_end(list);
    remove_start(list);

    kill_list(list);
    puts("success");
    return 0;
}