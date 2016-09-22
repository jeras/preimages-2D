#include <stdio.h>
#include <stdlib.h>

int function (size_t size, size_t *number, int unsigned (**image) [] [size]);

void init(size_t size, size_t number, unsigned image[number][size]) {
   for (int l=0; l<number; ++l)
        for (int s=0; s<size; ++s)
             image [l] [s] = (l << 16) + s;
    image [0] [0] = 0xDEADC0DE;
}

int function (size_t size, size_t *number, int unsigned (**p_image) [] [size]) {
    size_t image_size;

    *number = 9;
    image_size = (*number) * size;

    printf ("INSIDE malloc: number=%zu size=%zu image_size=%zu\n", *number, size, image_size); fflush(stdout);
    *p_image = (int unsigned (*) [] [size]) malloc (sizeof(int unsigned) * image_size);
    printf ("@&p_image=%p\n", &p_image);
    printf ("@*p_image=%p\n", *p_image);
    printf ("@ p_image=%p\n",  p_image);

    printf ("INSIDE assign: number=%zu size=%zu\n", *number, size); fflush(stdout);
    init(size, *number, **p_image);

    printf ("INSIDE print: number=%zu size=%zu\n", *number, size); fflush(stdout);
    for (int unsigned l=0; l<*number; l++) {
        printf ("l=%u:", l);
        for (int unsigned s=0; s<size; s++) {
//            printf (" %08x", (*p_image) [l] [s]);
        }
        printf ("\n");
    }

    return (0);
}

int main (int argc, char **argv) {
    size_t size;
    size_t number;

    size = 5;
    int unsigned (* p_image)[][size];

    printf ("TEST:\n"); fflush(stdout);

    function (size, &number, &p_image);

    fflush(stdout);
    unsigned (*image)[size] = (void *)p_image;
    printf ("@&p_image=%p\n", &p_image);
    printf ("@*p_image=%p\n", *p_image);
    printf ("@ p_image=%p\n",  p_image);
    printf ("OUTSIDE print: number=%zu size=%zu\n", number, size); fflush(stdout);
    for (int unsigned l=0; l<number; l++) {
        printf ("l=%u:", l);
        for (int unsigned s=0; s<size; s++) {
            printf (" %08x", image [l] [s]);
        }
        printf ("\n");
    }

//    free(image);

    return (0);
}
