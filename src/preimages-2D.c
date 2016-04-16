////////////////////////////////////////////////////////////////////////////////
//
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
//
////////////////////////////////////////////////////////////////////////////////

// user interface libraries
#include <stdio.h>
#include <stdlib.h>

// math libraries
#include <stdint.h>
#include <math.h>
#include <gmp.h>

#define uintca_t long long int unsigned

typedef struct {
    int unsigned x;
    int unsigned y;
    int unsigned a;
//    int unsigned n;
} ca_size_t;

int unsigned array2number (int unsigned base, int unsigned len, int unsigned array[len], int unsigned *number) {
    *number = 0;
    for (int unsigned i=0; i<len; i++) {
        *number *= base;
        *number += array[i];
    }
    return 0;
}

int unsigned number2array (int unsigned base, int unsigned len, int unsigned array[len], int unsigned number) {
    for (int unsigned i=0; i<len; i++) {
        array[i] = number % base;
        number /= base;
    }
    return 0;
}

int unsigned array_slice (
    int unsigned ix, int unsigned iy,
    int unsigned ox0, int unsigned ox1, int unsigned oy0, int unsigned oy1,
    int unsigned ia[iy][ix], int unsigned oa[oy1-oy0][ox1-ox0])
{
    for (int unsigned y=0; y<oy1-oy0; y++) {
        for (int unsigned x=0; x<ox1-ox0; x++) {
            oa[y][x] = ia[oy0+y][ox0+x];
        }
    }
    return 0;
}

int main (int argc, char **argv) {
    // configuration
    int unsigned sts;
    ca_size_t    ngb;
    mpz_t        rule;
    ca_size_t    siz;
    char *filename;
    FILE  file;

    // read input arguments
    if (argc < 8) {
       fprintf (stderr, "Usage:\t%s STATES NEIGHBORHOOD_SIZE_X NEIGHBORHOOD_SIZE_Y RULE CA_SIZE_X CA_SIZE_Y ca_state_filename.cas\n", argv[0]);
       return (1);
    }
    sts   = strtoul (argv[1], 0, 0);
    ngb.x = strtoul (argv[2], 0, 0);
    ngb.y = strtoul (argv[3], 0, 0);
    mpz_init_set_str (rule, argv[4], 0);
    siz.x = strtoul (argv[5], 0, 0);
    siz.y = strtoul (argv[6], 0, 0);
    filename = argv[7];

    // printout call arguments for debugging purposes
    printf     ("STATES              = %i\n", sts  );
    printf     ("NEIGHBORHOOD_SIZE_X = %i\n", ngb.x);
    printf     ("NEIGHBORHOOD_SIZE_Y = %i\n", ngb.y);
    gmp_printf ("RULE                = %Zi\n", rule);
    printf     ("CA_SIZE_X           = %i\n", siz.x);
    printf     ("CA_SIZE_Y           = %i\n", siz.y);
    printf     ("filename            = %s\n", filename);

    // neighborhood area
    ngb.a = ngb.x * ngb.y;
    printf     ("ngb.a               = %i\n", ngb.a);
    // check if it is within allowed values, for example (ngb.a < 9==3*3)
    if ((ngb.a == 0) || (ngb.a >= 9)) {
        fprintf (stderr, "ERROR: neighborhood area (ngb.a = %u) is outside range [1:9].\n", ngb.a);
        return (1);
    }

    // neighborhood states (sts ** ngb_n)
    uintca_t ngb_n = pow (sts, ngb.a);
    printf     ("ngb_n               = %lld\n", ngb_n);

    // check if rule is within range = sts ** (sts ** ngb_n)
    mpz_t range;
    mpz_init (range);
    mpz_ui_pow_ui (range, sts, ngb_n);
    gmp_printf ("range               = %Zi\n", range);
    if (mpz_cmp (rule, range) > 0) {
        fprintf (stderr, "ERROR: rule is outside of range\n");
        return (1);
    }

    typedef struct {
        int unsigned w;
        int unsigned x [2]; // left/right edge index
        int unsigned y [2]; // bottom/top edge index
    } map_t;

    // rule table (conversion to base sts)
    map_t tab [ngb_n];
    mpz_t rule_q, rule_r;
    mpz_init_set (rule_q, rule);
    mpz_init (rule_r);
    for (int unsigned i=0; i<ngb_n; i++) {
        // populate weight
        mpz_t tmp;
        mpz_init (tmp);
        tab[i].w = mpz_tdiv_q_ui (tmp, rule_q, sts);
        mpz_init_set (rule_q, tmp);
        // populate X overlap pointer table
        int unsigned a [ngb.y] [ngb.x];
        number2array (sts, ngb.a, (unsigned int *) a, i);
        for (int unsigned j=0; j<2; j++) {
             int unsigned o [ngb.y-1] [ngb.x];
             array_slice (ngb.x, ngb.y, 0, ngb.x, j, ngb.y+j-1, a, o);
             array2number (sts, (ngb.y-1)*ngb.x, (unsigned int *) o, &(tab[i].y[j]));
        }
        for (int unsigned j=0; j<2; j++) {
             int unsigned o [ngb.y] [ngb.x-1];
             array_slice (ngb.x, ngb.y, j, ngb.x+j-1, 0, ngb.y, a, o);
             array2number (sts, ngb.y*(ngb.x-1), (int unsigned *) o, &(tab[i].x[j]));
        }
        printf     ("tab[%4i].w       = %u, ox{%u,%u} oy{%u,%u}\n", i, tab[i].w, tab[i].x[0], tab[i].x[1], tab[i].y[0], tab[i].y[1]);
    }
    
    // overlap states (sts ** ovl_n)
    uintca_t ovl_x = pow (sts, ngb.a - ngb.y);
    uintca_t ovl_y = pow (sts, ngb.a - ngb.x);
    printf     ("ovl_x               = %lld\n", ovl_x);
    printf     ("ovl_y               = %lld\n", ovl_y);

    // read CA configuration file
    int unsigned ca [siz.y] [siz.x];
    FILE *fp;
    fp = fopen (filename, "r");
    if (!fp) {
        fprintf (stderr, "ERROR: file %s not found\n", filename);
        return (1);
    }
    for (int unsigned y=0; y<siz.y; y++) {
        for (int unsigned x=0; x<siz.x; x++) {
             fscanf (fp, "%u", &ca [y] [x]);
        }
    }
    fclose (fp);
    // printout CA state
    for (int unsigned y=0; y<siz.y; y++) {
        printf ("CA [y=%u]:", y);
        for (int unsigned x=0; x<siz.x; x++) {
            printf (" %u", ca [y] [x]);
        }
        printf ("\n");
    }

    // memory allocation for preimage network
    uintca_t net_x [siz.y  ] [siz.x+1] [ovl_x];
    uintca_t net_y [siz.y+1] [siz.x  ] [ovl_y];

    // initialize array x
    for (int unsigned y=0; y<siz.y; y++) {
        for (int unsigned x=0; x<=siz.x; x++) {
             for (int unsigned i=0; i<ovl_x; i++) {
                 // TODO: for now only a unit weight edge is supportedunsigned int *
                 if (!x)  net_x [y] [x] [i] = 1;
                 else     net_x [y] [x] [i] = 0;
             }
        }
    }

    // initialize array y
    for (int unsigned y=0; y<=siz.y; y++) {
        for (int unsigned x=0; x<siz.x; x++) {
             for (int unsigned i=0; i<ovl_y; i++) {
                 // TODO: for now only a unit weight edge is supportedunsigned int *
                 if (!y)  net_y [y] [x] [i] = 1;
                 else     net_y [y] [x] [i] = 0;
             }
        }
    }

    // compute network weights
    uintca_t mul;
    for (int unsigned y=0; y<siz.y; y++) {
        for (int unsigned x=0; x<siz.x; x++) {
             for (int unsigned i=0; i<ngb_n; i++) {
                  if (tab [i].w == ca [y] [x]) {
                      mul = net_x [y] [x] [tab [i].x [0]]
                          * net_y [y] [x] [tab [i].y [0]];
                      net_x [y  ] [x+1] [tab [i].x [1]] += mul;
                      net_y [y+1] [x  ] [tab [i].y [1]] += mul;
//                      printf ("x=%u y=%u i=%u tab[i].x[1]=%u tab[i].y[1]=%u\n", x, y, i, tab [i].x [1], tab [i].y [1]);
                  }
             }
        }
    }


    return (0);
}

