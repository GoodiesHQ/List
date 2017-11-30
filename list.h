#ifndef LIST_H
#define LIST_H
#include <assert.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_TYPE
#define LIST_TYPE int
#endif
typedef LIST_TYPE val;
typedef struct _list list, *plist;
typedef struct _node node, *pnode;
typedef val(*list_func)(val v1, val v2);


#ifdef WIN32
#include <Windows.h>
typedef HANDLE mutex_t;
#define LIST_LOCK(l) WaitForSingleObject((l)->mu, INFINITE)
#define LIST_UNLOCK(l) ReleaseMutex((l)->mu)
#define LIST_DESTROY_MUTEX(l) 
#else
typedef pthread_mutex_t mutex_t;
#define LIST_LOCK(l) pthread_mutex_lock(&((l)->mu))
#define LIST_UNLOCK(l) pthread_mutex_unlock(&((l)->mu))
#define LIST_DESTROY_MUTEX(l) pthread_mutex_destroy(&((l)->mu))
#endif

#define LIST_INC_SIZE(l) (l)->size++
#define LIST_DEC_SIZE(l) (l)->size--

extern bool debugging;

typedef enum _status
{
    SUCCESS,
    MEMORY_FAILURE,
    OUT_OF_BOUNDS,
    FAILURE,
} status;

struct _list
{
    pnode head;
    pnode tail;
    size_t size;
    mutex_t mu;
#ifndef WIN32
    pthread_mutexattr_t mu_attr;
#endif
};

struct _node
{
    plist owner;
    pnode next;
    pnode prev;
    val value;
};

typedef void (*callback_t)(val);

// initialization and deconstruction of a list
void list_print_status(status);
plist list_new(void);
plist list_copy(plist);
pnode node_new(val value);
void list_init(plist l);
void list_del(plist *l);

// getting an item from a list
status list_get_val(plist l, size_t index, val *value);
status list_get_ptr(plist l, size_t index, val **value);
status list_get_node(plist l, size_t index, pnode *n);
status list_swap(plist l, size_t index1, size_t index2);
status list_reverse(plist l);

// display the list
void list_print(plist l, const char* val_fmt);
void list_print_reverse(plist l, const char* val_fmt);

// iterating the list
pnode list_iter(plist l);
bool list_iter_next(pnode *n);
void list_map(callback_t callback, plist l);

// adding elements to a list
status list_append(plist l, val value);
status list_prepend(plist l, val value);
status list_insert_at(plist l, size_t index, val Value);

// removing elements from a list
status list_remove_back(plist l);
status list_remove_font(plist l);
status list_remove_at(plist l, size_t index);
#endif
