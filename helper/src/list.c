#include "list.h"

#define LIST_OR_NODE_IS_EMPTY(X) ((X)==NULL?true:false)

bool list_push_back(struct _list *list, struct _node *node)
{
    struct _node **ptr_node = NULL;

    if (LIST_OR_NODE_IS_EMPTY(list) || LIST_OR_NODE_IS_EMPTY(node)) {
        return false;
    }

    ptr_node = &(list->ptr_head);

    while (*ptr_node) {
        ptr_node = &(*ptr_node)->next;
    }

    *ptr_node = node;

    list->length++;

    return true;
}

bool list_pop(struct _list *list)
{
    struct _node **ptr_node = NULL;

    if (LIST_OR_NODE_IS_EMPTY(list)) {
        return false;
        
    }

    ptr_node = &(list->ptr_head);

    while ((*ptr_node)->next) {
        ptr_node = &(*ptr_node)->next;
    }

    *ptr_node = NULL;

    list->length--;

    return true;
}

struct _list* new_list(void)
{
    struct _list *new_list = (struct _list*)calloc(1, sizeof(struct _list));
    new_list->ptr_head = NULL;
    new_list->length = 0;
    return new_list;
}