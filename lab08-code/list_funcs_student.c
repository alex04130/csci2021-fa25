#include <stdlib.h>

#include "linked_list.h"

int list_find_student(list_t *list, int value) {
    if (list == NULL) {
        return -1;
    }
    node_t *ptr = list->head;
    int i = 0;
    while (ptr != NULL) {
        if (ptr->value == value) {
            return i;
        }
        i++;
        ptr = ptr->next;
    }
    return -1;
}
