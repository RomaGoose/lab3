#include "container.h"
void sel_sort(DLList* list, int (*cmp)(void* l, void*r)){
    Iterator* i;
    size_t list_size = get_size(list);
    for(size_t run = 0; run < list_size; ++run){
        size_t pos_of_max = 0;
        for (i = begin(list); get_pos(i) < list_size - run; next(i)){
            if(cmp(get_element(pos_of_max, list), get(i)) < 0) 
                pos_of_max = get_pos(i);
        }
        swap(pos_of_max, list_size-1 - run, list);
    }
}