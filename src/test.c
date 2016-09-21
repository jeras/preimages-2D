#include <stdio.h>
#include <stdlib.h>

int function (size_t size, size_t *length, int unsigned (*list) [] [size]);

int function (size_t size, size_t *length, int unsigned (*list) [] [size]) {
    size_t list_size;

    *length = 9;
    list_size = (*length) * size;

    printf ("INSIDE malloc: length=%zu size=%zu list_size=%zu\n", *length, size, list_size);
    printf ("@*list=%p\n", *list);
    printf ("@ list=%p\n",  list);
    fflush(stdout);
    list = (int unsigned (*) [] [size]) malloc (sizeof(int unsigned) * list_size);

    printf ("INSIDE assign: length=%zu size=%zu\n", *length, size);
    printf ("@*list=%p\n", *list);
    printf ("@ list=%p\n",  list);
    fflush(stdout);
    for (int unsigned l=0; l<*length; l++) {
        for (int unsigned s=0; s<size; s++) {
            (*list) [l] [s] = (l << 16) + s;
        }
    }
    (*list) [0] [0] = 0xDEADC0DE;

    printf ("INSIDE print: length=%zu size=%zu\n", *length, size);
    fflush(stdout);
    for (int unsigned l=0; l<*length; l++) {
        printf ("l=%u:", l);
        for (int unsigned s=0; s<size; s++) {
            printf (" %08x", (*list) [l] [s]);
        }
        printf ("\n");
    }

    return (0);
}

int main (int argc, char **argv) {
    size_t size;
    size_t length;

    size = 5;
    int unsigned (* list) [size];

    printf ("TEST:\n");
    printf ("@&list=%p\n", &list);
    printf ("@*list=%p\n", *list);
    printf ("@ list=%p\n",  list);
    fflush(stdout);

    function (size, &length, &list);

    printf ("@&list=%p\n", &list);
    printf ("@*list=%p\n", *list);
    printf (" *list=%u\n", *list);
    printf ("@ list=%p\n",  list);
    printf ("  list=%u\n",  list);
    fflush(stdout);
//    printf ("OUTSIDE print: length=%zu size=%zu\n", length, size);
//    fflush(stdout);
//    for (int unsigned l=0; l<length; l++) {
//        printf ("l=%u:", l);
//        for (int unsigned s=0; s<size; s++) {
//            printf (" %08x", (*list) [l] [s]);
//        }
//        printf ("\n");
//    }

    return (0);
}
