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
#### callback_t
```c
typedef void(*callback_t)(LIST_TYPE);
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
This just calls `printf` on each value in the list. Use `list_map` for a more customized display.
```c
void list_print(plist l, const char* val_fmt);
void list_print_reverse(plist l, const char* val_fmt);
```
#### Iterating the list
```c
pnode list_iter(plist l);
bool list_iter_next(pnode *n);
void list_map(callback_t callback, plist l);
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
## Useful Macros
These macros will be of use if you choose to use this.
#### LIST_LOCK(list\*)
This will acquire the pthread mutex. Use this prior to iterating over the items in a list **Except if using list_map**, as the size may change between the time you get the size and the time you start iterating if you do not lock it.
```c
#define LIST_LOCK(l) pthread_mutex_lock(&((l)->mu))
```
#### LIST_UNLOCK(list\*)
Similarly, this will release the lock. Within the functions, all locks are paired with an unlock. No function should leave a lock count with a value different to what it received.
```c
#define LIST_UNLOCK(l) pthread_mutex_unlock(&((l)->mu))
```
### LIST_INC_SIZE(list\*) and LIST_DEC_SIZE(list\*)
**Do not use this** - These are used internally to control the size of the mutex.
```c
#define LIST_INC_SIZE(l) (l)->size++
#define LIST_DEC_SIZE(l) (l)->size--
```
### LIST_DESTROY_MUTEX(list\*)
**Do not use this** - Destroys the list's mutex. This is executed upon the deconstruction of a list.
```c
#define LIST_DESTROY_MUTEX(l) pthread_mutex_destroy(&((l)->mu))
```
