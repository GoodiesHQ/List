#include "list.h"

void list_print_status(status s){
    switch(s)
    {
        case SUCCESS:
            // Upon success, print nothing.
            //fprintf(stderr, "OK\n");
            break;
        case MEMORY_FAILURE:
            fprintf(stderr, "Could not allocate enough memory.\n");
            break;
        case OUT_OF_BOUNDS:
            fprintf(stderr, "The index provided does not exist.\n");
            break;
        case FAILURE:
            fprintf(stderr, "An unspecified error has occurred.\n");
            break;
    }
}

plist list_new(void)
{
    plist new_list = calloc(1, sizeof(*new_list));
    if(!new_list)
    {
        return NULL;
    }
    list_init(new_list);
    return new_list;
}

plist list_copy(plist p)
{
    plist lst = list_new();
    pnode it = p->head;
    while(it != NULL)
    {
        if(list_append(lst, it->value) != SUCCESS)
        {
            list_del(&lst);
            return NULL;
        }
        it = it->next;
    }
    return lst;
}

// construct a new node based on the provided value
pnode node_new(val value)
{
    pnode new_node = calloc(1, sizeof(*new_node));
    if(!new_node)
    {
        return NULL;
    }
    new_node->next=NULL;
    new_node->prev=NULL;
    new_node->value = value;

    return new_node;
}

// initialize a list's objects to NULL and initializes the mutex
void list_init(plist l)
{
    l->head=NULL;
    l->tail=NULL;
    pthread_mutexattr_init(&l->mu_attr);
    pthread_mutexattr_settype(&l->mu_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&l->mu, &l->mu_attr);
}

// free a list and delete the 
void list_del(plist *l)
{
    LIST_LOCK(*l);

    pnode n = (*l)->head;
    pnode tmp = NULL;

    while(n != NULL)
    {
        tmp = n->next;
        free(n);
        n = tmp;
    }

    LIST_UNLOCK(*l);
    LIST_DESTROY_MUTEX(*l);
    free(*l);
    *l = NULL;
}

// store value of l[index] inside the provided pointer
status list_get_val(plist l, size_t index, val *value)
{
    LIST_LOCK(l);
    pnode n = NULL;
    status s = list_get_node(l, index, &n);
    if(s != SUCCESS)
    {
        LIST_UNLOCK(l);
        return s;
    }
    *value = n->value;
    LIST_UNLOCK(l);
    return SUCCESS;
}

// store the pointer to the value inside the provided pointer
status list_get_ptr(plist l, size_t index, val **value)
{
    LIST_LOCK(l);
    pnode n = NULL;
    status s = list_get_node(l, index, &n);
    if(s != SUCCESS)
    {
        LIST_UNLOCK(l);
        return s;
    }
    *value = &n->value;
    LIST_UNLOCK(l);
    return SUCCESS;
}


// store a pointer to the node inside the provided pointer
status list_get_node(plist l, size_t index, pnode *n)
{
    LIST_LOCK(l);
    if(index >= l->size)
    {
        LIST_UNLOCK(l);
        return OUT_OF_BOUNDS;
    }
    pnode it = l->head;
    while(index--)
    {
        it = it->next;
    }

    *n = it;

    LIST_UNLOCK(l);
    return SUCCESS;
}

// swap two nodes at the provided indecies
status list_swap(plist l, size_t index1, size_t index2){
    LIST_LOCK(l);
    pnode n1 = NULL, n2 = NULL;
    val tmp;
    status s;

    if(
        (s = list_get_node(l, index1, &n1) != SUCCESS) ||
        (s = list_get_node(l, index2, &n2) != SUCCESS)
    )
    {
        LIST_UNLOCK(l);
        return s;
    }
    tmp = n2->value;
    n2->value = n1->value;
    n1->value = tmp;

    LIST_UNLOCK(l);
    return SUCCESS;
}

status list_reverse(plist l)
{
    LIST_LOCK(l);
    size_t size = l->size;
    if(size <= 1)
    {
        LIST_UNLOCK(l);
        return SUCCESS;
    }
    size_t mid = size / 2;
    status s;

    for(size_t ctr = 0; ctr < mid; ++ctr)
    {
        if((s=list_swap(l, ctr, size-ctr-1)) != SUCCESS)
        {
            LIST_UNLOCK(l);
            return s;
        }
    }

    LIST_UNLOCK(l);
    return SUCCESS;
}

void list_print(plist l, const char* val_fmt)
{
    LIST_LOCK(l);
    if(l->size == 0)
    {
        LIST_UNLOCK(l);
        return;
    }
    pnode n = l->head;
    while(n != NULL)
    {
        printf(val_fmt, n->value);
        n = n->next;
    }
    LIST_UNLOCK(l);
}

void list_print_reverse(plist l, const char* val_fmt){
    LIST_LOCK(l);
    if(l->size == 0)
    {
        LIST_UNLOCK(l);
        return;
    }
    pnode n = l->tail;
    while(n != NULL)
    {
        printf(val_fmt, n->value);
        n = n->prev;
    }
    LIST_UNLOCK(l);
}


pnode list_iter(plist l)
{
    return l->head;
    return NULL;
}

bool list_iter_next(pnode *n)
{
    pnode nd = *n;
    *n = nd->next;
    return *n != NULL;
}

void list_map(callback_t callback, plist l)
{
    LIST_LOCK(l);
    pnode it = list_iter(l);
    if(it)
    {
        do
        {
            callback(it->value);
        } while(list_iter_next(&it));
    }
    LIST_UNLOCK(l);
}

status list_append(plist l, val value)
{
    pnode n = node_new(value);
    if(!n)
    {
        return MEMORY_FAILURE;
    }

    n->owner = l;

    LIST_LOCK(l);
    LIST_INC_SIZE(l);

    if(l->head == NULL || l->tail == NULL)
    {
        assert(l->head == NULL && l->tail == NULL); // they should never be either/or
        l->head = l->tail = n;
        LIST_UNLOCK(l);
        return SUCCESS;
    }

    assert(l->tail->next == NULL);
    n->prev = l->tail;
    l->tail->next = n;
    l->tail = n;
    LIST_UNLOCK(l);
    return SUCCESS;
}

status list_prepend(plist l, val value){
    pnode n = node_new(value);
    if(!n)
    {
        return MEMORY_FAILURE;
    }

    n->owner = l;

    LIST_LOCK(l);
    LIST_INC_SIZE(l);

    if(l->head == NULL || l->tail == NULL)
    {
        assert(l->head == NULL && l->tail == NULL); // they should never be either/or
        l->head = l->tail = n;
        LIST_UNLOCK(l);
        return SUCCESS;
    }

    assert(l->head->prev == NULL);
    n->next = l->head;
    l->head->prev = n;
    l->head = n;
    LIST_UNLOCK(l);
    return SUCCESS;
}
