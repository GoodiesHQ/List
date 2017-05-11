# List
A super simple (potentially thread safe?) implementation of a linked list.

Uses a re-entrant pthread lock in each of the called function. Certainly blocks.

## Types

The types used throughout the linked list application.


#### LIST_TYPE/val

If a preprocessor definition for `LIST_TYPE` is used, the provided type will be used throughought the linked list. By default, `int` is used.
```
#ifndef LIST_TYPE
#define LIST_TYPE int
#endif
typedef LIST_TYPE val;
```

#### status
```c
enum _status
{
    SUCCESS,
    MEMORY_FAILURE,
    OUT_OF_BOUNDS,
    FAILURE,
};
```
#### list
```c
struct _list
{
    pnode head;
    pnode tail;
    size_t size;
    pthread_mutex_t mu;
    pthread_mutexattr_t mu_attr;
};
```
#### node
```c
struct _node
{
    plist owner;
    pnode next;
    pnode prev;
    val value;
};
```

## Functions

These are hte functions available to you. By the names, it should be fairly obvious what they do.

#### Initialization and deconstruction of a list
```c
void list_print_status(status);
plist list_new(void);
plist list_copy(plist);
pnode node_new(val value);
void list_init(plist l);
void list_del(plist *l);
```
#### Getting an item from a list
```c
status list_get_val(plist l, size_t index, val *value);
status list_get_ptr(plist l, size_t index, val **value);
status list_get_node(plist l, size_t index, pnode *n);
status list_swap(plist l, size_t index1, size_t index2);
status list_reverse(plist l);
```
#### Display the list
```c
void list_print(plist l, const char* val_fmt);
void list_print_reverse(plist l, const char* val_fmt);
```
#### Adding elements to a list
```c
status list_append(plist l, val value);
status list_prepend(plist l, val value);
status list_insert_at(plist l, size_t index, val Value);
```
#### Removing elements from a list
```c
status list_remove_back(plist l);
status list_remove_font(plist l);
status list_remove_at(plist l, size_t index);
```
