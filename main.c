#include "list.h"

int main(){
    plist lst1 = list_new();
    list_append(lst1, 10);
    list_append(lst1, 20);
    list_append(lst1, 30);

    plist lst2 = list_copy(lst1);

    int *pint;
    list_reverse(lst2);
    list_get_ptr(lst2, 0, &pint);

    (*pint)++;

    char *fmt = "%d ";
    list_print(lst1, fmt);
    printf("%p\n", lst1);
    list_print(lst2, fmt);
    printf("%p\n", lst2);

    list_del(&lst1);
    list_del(&lst2);

    return 0;
}
