#ifndef __LIST_H__
#define __LIST_H__
#include <stdbool.h>
#include <stdlib.h>

#define EACH_FOR_LIST(PTR_LIST, PTR_NODE) \
                for ((PTR_NODE) = (PTR_LIST)->ptr_head; (PTR_NODE) != NULL; (PTR_NODE) = (PTR_NODE)->next)
#define INIT_LIST(LIST_NAME) \
            struct _list* (LIST_NAME) = new_list();
#define OFFSETOF(TYPE, MEMBER) \
                (size_t)&(((TYPE*)0)->MEMBER)
#define CONTAINER_OF(PTR, TYPE, MEMBER) \
                ({const typeof(((TYPE*)0)->MEMBER) *__mptr=PTR; \
                (TYPE*)((char*)__mptr - OFFSETOF(TYPE, MEMBER));})


struct _node {
    struct _node *next;
};

struct _list { 
    struct _node *ptr_head;
    int length;
};


struct _list* new_list(void);
bool list_push_back(struct _list*, struct _node*);
bool list_pop(struct _list*);

#endif