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
    int unsigned y;
    int unsigned x;
} size2D_t;

////////////////////////////////////////////////////////////////////////////////
// array <--> number  conversions
////////////////////////////////////////////////////////////////////////////////

int unsigned array_print (size2D_t s, int unsigned array[s.y][s.x]) {
    printf ("[");
    for (int unsigned y=0; y<s.y; y++) {
        printf ("%s[", y ? "," : "");
        for (int unsigned x=0; x<s.x; x++) {
            printf ("%s%u", x ? "," : "", array [y] [x]);
        }
        printf ("]");
    }
    printf ("]");
    return 0;
}

int unsigned array2number (int unsigned base, size2D_t s, int unsigned array[s.y][s.x], int unsigned *number) {
    *number = 0;
    int unsigned mul = 1;
    for (int unsigned y=0; y<s.y; y++) {
        for (int unsigned x=0; x<s.x; x++) {
            *number += array [y] [x] * mul;
            mul *= base;
        }
    }
    return 0;
}

int unsigned number2array (int unsigned base, size2D_t s, int unsigned array[s.y][s.x], int unsigned number) {
    for (int unsigned y=0; y<s.y; y++) {
        for (int unsigned x=0; x<s.x; x++) {
            array [y] [x] = number % base;
            number /= base;
        }
    }
    return 0;
}

int unsigned array_slice (
    size2D_t is,
    size2D_t os0, size2D_t os1,
    int unsigned ia[is.y][is.x], int unsigned oa[os1.y-os0.y][os1.x-os0.x])
{
    for (int unsigned y=0; y<os1.y-os0.y; y++) {
        for (int unsigned x=0; x<os1.x-os0.x; x++) {
            oa[y][x] = ia[os0.y+y][os0.x+x];
        }
    }
    return 0;
}

int unsigned array_combine_x (
    size2D_t is0, size2D_t is1,
    int unsigned ia0[is0.y][is0.x], int unsigned ia1[is1.y][is1.x], int unsigned oa[is0.y][is1.x+is0.x])
{
    for (int unsigned y=0; y<is0.y; y++) {
        for (int unsigned x=0; x<is0.x; x++) {
            oa[y][x      ] = ia0[y][x];
        }
        for (int unsigned x=0; x<is1.x; x++) {
            oa[y][x+is0.x] = ia1[y][x];
        }
    }
    return 0;
}

int unsigned array_combine_y (
    size2D_t is0, size2D_t is1,
    int unsigned ia0[is0.y][is0.x], int unsigned ia1[is1.y][is1.x], int unsigned oa[is1.y+is0.y][is0.x])
{
    for (int unsigned x=0; x<is0.x; x++) {
        for (int unsigned y=0; y<is0.y; y++) {
            oa[y      ][x] = ia0[y][x];
        }
        for (int unsigned y=0; y<is0.y; y++) {
            oa[y+is0.y][x] = ia1[y][x];
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// rule handling
////////////////////////////////////////////////////////////////////////////////

// function for transforming the Game of Life rule description into a number
int rule_gol () {
    int unsigned len = 1<<9;
    mpz_t rule;
    mpz_init_set_ui (rule, 0);
    // loop through the rule table
    for (int unsigned i=0; i<len; i++) {
        // calculate neighborhood sum
        int unsigned sum = 0;
        for (int unsigned b=0; b<9; b++) {
            sum += (i >> b) & 0x1;
        }
        // calculate GoL transition function
        int unsigned out;
        if      (sum == 3)  out = 1;
        else if (sum == 2)  out = (i >> 4) % 0x1;
        else                out = 0;
        // add rule table element to rule number
        mpz_mul_ui (rule, rule, 2);
        mpz_add_ui (rule, rule, out);
    }
    // return rule
    gmp_printf ("RULE GoL             = %Zi\n", rule);
    return (0);
}

//int rule_print (int unsigned nx, int unsigned ny, int unsigned sts, tab[pow (sts, ny*nx)]) {
//    for (int unsigned i=0; i<ngb_n; i++) {
//        // populate pointer table
//        printf ("tab [");
//        for (int unsigned y=0; y<ngb.y; y++) {
//            printf ("%s[", y ? "," : "");
//            for (int unsigned x=0; x<ngb.x; x++) {
//                printf ("%s%u", x ? "," : "", tab[i].n [y] [x]);
//            }
//            printf ("]");
//        }
//        printf ("] = %u\n", tab[i]);
//    }
//}

////////////////////////////////////////////////////////////////////////////////
// CA configuration handling
////////////////////////////////////////////////////////////////////////////////

// read CA state from file
int ca_read (char *filename, size2D_t siz, int unsigned ca [siz.y] [siz.x]) {
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
}

// print CA state
int ca_print (size2D_t siz, int unsigned ca [siz.y] [siz.x]) {
    for (int unsigned y=0; y<siz.y; y++) {
        printf ("CA [y=%u]:", y);
        for (int unsigned x=0; x<siz.x; x++) {
            printf (" %u", ca [y] [x]);
        }
        printf ("\n");
    }
}

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv) {
    // configuration
    int unsigned sts;
    size2D_t     ngb;
    mpz_t        rule;
    size2D_t     siz;
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
    // check if it is within allowed values, for example less then 9==3*3
    if ((ngb.x == 0) || (ngb.y == 0) || ((ngb.y * ngb.x) > 9)) {
        fprintf (stderr, "ERROR: neighborhood area %u is outside range [1:9].\n", ngb.y * ngb.x);
        return (1);
    }

    // neighborhood states (sts ** ngb_n)
    uintca_t ngb_n = pow (sts, ngb.y * ngb.x);
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
    mpz_clear (range);

    // rule table (conversion to base sts)
    int unsigned tab [ngb_n];
    mpz_t rule_q;
    mpz_init_set (rule_q, rule);
    for (int unsigned i=0; i<ngb_n; i++) {
        // populate transition function
        tab[i] = mpz_tdiv_q_ui (rule_q, rule_q, sts);
    }
    mpz_clear (rule_q);
    
    // overlap states
    uintca_t ovl = pow (sts, ngb.y * ngb.x - 1);
    printf     ("ovl                 = %lld\n", ovl);

    // read CA configuration file
    int unsigned ca [siz.y] [siz.x];
    ca_read (filename, siz, ca);
    ca_print (siz, ca);

    // counting the possible number of preimages, used to reserve memory
    mpz_t max;
    mpz_init (max);
    mpz_ui_pow_ui (max, sts, siz.y * siz.x);
    gmp_printf ("max                 = %Zi\n", max);
    size_t btn;
    btn = mpz_sizeinbase (max, 2);
    mpz_clear (max);
    printf     ("btn                 = %zu\n", btn);



    // memory allocation for preimage network
    mpz_t net [siz.y+1] [siz.x+1] [ngb_n];

    // initialize array
    for (int unsigned y=0; y<=siz.y; y++) {
        for (int unsigned x=0; x<=siz.x; x++) {
            for (int unsigned n=0; n<ngb_n; o++) {
                // TODO: for now only a unit weight edge is supported unsigned int *
                mpz_init2 (net [y] [x] [n], btn);
            }
        }
    }

    // set boundary value
    for (int unsigned y=0; y<=siz.y; y++) {
        for (int unsigned n=0; n<ngb_n; n++) {
            mpz_set_ui (net [y] [0] [n], 1);
        }
    }
    for (int unsigned x=0; x<=siz.x; x++) {
        for (int unsigned n=0; n<ngb_n; n++) {
            mpz_set_ui (net [0] [x] [n], 1);
        }
    }




    return (0);
}

