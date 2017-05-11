typedef struct _foo
{
    char *key;
    char *value;
} foo;

#define LIST_TYPE foo*


#include "list.h"

void printfoo(foo *f)
{
    printf("(%p): %s : %s\n", f, f->key, f->value);
}

int main(){
    plist l = list_new();
    foo *f1 = malloc(sizeof(foo));
    f1->key = "KeyA";
    f1->value = "Val1";

    foo *f2 = malloc(sizeof(foo));
    f2->key = "KeyB";
    f2->value = "Val2";

    foo *f3 = malloc(sizeof(foo));
    f3->key = "KeyC";
    f3->value = "Val3";

    list_append(l, f1);
    list_append(l, f2);
    list_append(l, f3);

    list_reverse(l);
    list_map(&printfoo, l);
    list_del(&l);

    free(f1);
    free(f2);
    free(f3);

    return 0;
}
