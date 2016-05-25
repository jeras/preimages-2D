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

#include "ca2d.h"
#include "ca2d_rule.h"
#include "ca2d_array.h"
#include "ca2d_configuration.h"
#include "ca2d_forward.h"

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv) {
    // configuration
    ca2d_t  ca2d;
    ca2d_size_t  siz;
    char *filename;
    FILE  file;

    // read input arguments
    if (argc < 8) {
        fprintf (stderr, "Usage:\t%s STATES NEIGHBORHOOD_SIZE_X NEIGHBORHOOD_SIZE_Y RULE CA_SIZE_X CA_SIZE_Y ca_state_filename.cas\n", argv[0]);
        return (1);
    }
    ca2d.sts   = strtoul (argv[1], 0, 0);
    ca2d.ngb.x = strtoul (argv[2], 0, 0);
    ca2d.ngb.y = strtoul (argv[3], 0, 0);
    mpz_init_set_str (ca2d.rule, argv[4], 0);
    siz.x = strtoul (argv[5], 0, 0);
    siz.y = strtoul (argv[6], 0, 0);
    filename = argv[7];

    // printout call arguments for debugging purposes
    printf     ("CA_SIZE_X           = %i\n", siz.x);
    printf     ("CA_SIZE_Y           = %i\n", siz.y);
    printf     ("filename            = %s\n", filename);
    ca2d_rule_print (ca2d);   

    // overlap states
    uintca_t ovl = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x - 1);
    printf     ("ovl                 = %lld\n", ovl);

    // read CA configuration file
    int unsigned ca [siz.y] [siz.x];
    ca2d_read (filename, siz, ca);
    ca2d_print (siz, ca);




    // neighborhood states (sts ** ngb_n)
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);
    int unsigned tab [ngb_n];
    ca2d_rule_table (ca2d, tab);

    // memory allocation for preimage network
    mpz_t net [2] [2] [siz.y] [siz.x] [ngb_n];

    // initialize array
    for (int unsigned dy=0; dy<2; dy++) {
        for (int unsigned dx=0; dx<2; dx++) {
            for (int unsigned y=0; y<siz.y; y++) {
                for (int unsigned x=0; x<siz.x; x++) {
                    for (int unsigned n=0; n<ngb_n; n++) {
                        // TODO: for now only a unit weight edge is supported unsigned int *
                        mpz_init (net [dy] [dx] [y] [x] [n]);
                    }
                }
            }
        }
    }

    // temporary structure
    const int unsigned ovl_x = pow (ca2d.sts, (ca2d.ngb.x-1)*(ca2d.ngb.y  ));
    const int unsigned ovl_y = pow (ca2d.sts, (ca2d.ngb.x  )*(ca2d.ngb.y-1));

    // pointers to X/Y/Z(xy) dimension edges
    int unsigned px [2] [ngb_n];
    int unsigned py [2] [ngb_n];

    ca2d_size_t     sx = {ca2d.ngb.y, ca2d.ngb.x-1}; 
    int unsigned ax [sx.y] [sx.x];
 
    ca2d_size_t     sy = {ca2d.ngb.y-1, ca2d.ngb.x}; 
    int unsigned ay [sy.y] [sy.x];
        
    int unsigned a [ca2d.ngb.y] [ca2d.ngb.x];
        
    for (int unsigned y=0; y<2; y++) {
        for (int unsigned n=0; n<ngb_n; n++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, n);

            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {y, 0}, sy, a, ay);
            ca2d_array_to_ui (ca2d.sts, sy, ay, &py [y] [n]);

            printf ("pointers i: ");
            ca2d_array_print (ca2d.ngb, a); printf (" :: ");
            ca2d_array_print (sy, ay); printf (" -> py[%u][%X] = %u | ", y, n, py [y] [n]);
            printf ("\n");
        }
        printf ("\n");
    }
    for (int unsigned x=0; x<2; x++) {
        for (int unsigned n=0; n<ngb_n; n++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, n);

            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, x}, sx, a, ax);
            ca2d_array_to_ui (ca2d.sts, sx, ax, &px [x] [n]);

            printf ("pointers i: ");
            ca2d_array_print (ca2d.ngb, a); printf (" :: ");
            ca2d_array_print (sx, ax); printf (" -> px[%u][%X] = %u | ", x, n, px [x] [n]);
            printf ("\n");
        }
        printf ("\n");
    }

    // compute network weights
    // weights for X/Y/Z dimension edges
    mpz_t wy [ovl_y];
    mpz_t wx [ovl_x];
    mpz_t w;
    for (int unsigned o=0; o<ovl_y; o++)  mpz_init (wy[o]);
    for (int unsigned o=0; o<ovl_x; o++)  mpz_init (wx[o]);
    mpz_init (w);

    for (int unsigned dy=0; dy<2; dy++) {
        for (int y=dy?siz.y-1:0; dy?(y>=0):(y<siz.y); y=dy?y-1:y+1) {
//    for (int unsigned dy=0; dy<1; dy++) {
//        for (int y=dy?siz.y-1:0; dy?(y>=0):(y<1); y=dy?y-1:y+1) {
            for (int unsigned dx=0; dx<2; dx++) {
                for (int x=dx?siz.x-1:0; dx?(x>=0):(x<siz.x); x=dx?x-1:x+1) {
                    int ny = dy ? y+1 : y-1;
                    int nx = dx ? x+1 : x-1;
                    // initialize temporary result array/structure
                    for (int unsigned o=0; o<ovl_y; o++)  mpz_set_ui (wy[o], dy ? y==(siz.y-1) : y==0);
                    for (int unsigned o=0; o<ovl_x; o++)  mpz_set_ui (wx[o], dx ? x==(siz.x-1) : x==0);
                    // construct temporary result array/structure
                    printf ("%sy%d%sx%d: ", dy?"-":"+", y, dx?"-":"+", x);
                    for (int unsigned n=0; n<ngb_n; n++) {
                        if (!(dy ? y==(siz.y-1) : y==0)) {
                            mpz_mul (w, net [dy] [0] [ny] [x] [n], net [dy] [1] [ny] [x] [n]);
                            mpz_add (wy [py[1-dy][n]], wy [py[1-dy][n]], w);
                        }
                        if (!(dx ? x==(siz.x-1) : x==0)) {
                            mpz_add (wx [px[1-dx][n]], wx [px[1-dx][n]], net [dy] [dx] [y] [nx] [n]);
                        }
//                        gmp_printf ("n%X(%Zi,%Zi,%x) ", n, wy [py[dy][n]], wx [px[dx][n]], py[dy][n]);
                    }
                    for (int unsigned o=0; o<ovl_y; o++) {
                        gmp_printf ("n%X(%Zi,%Zi) ", o, wy [o], wx [o]);
                    }
                    printf ("\n");
                    // compute new weight from surrounding weights
                    for (int unsigned n=0; n<ngb_n; n++) {
                        if (tab [n] == ca [y] [x]) {
                            mpz_mul (net [dy] [dx] [y] [x] [n], wy [py[dy][n]], wx [px[dx][n]]);
                        }
                        // TODO: when a proper algorithm for computing weights is found, remove this line
                        if (mpz_sgn (net [dy] [dx] [y] [x] [n])) {
                            mpz_set_ui (net [dy] [dx] [y] [x] [n], 1);
                        }
                    }
                }
            }
        }
    }
    printf ("\n");

    int unsigned dy=0;
    int unsigned dx=0;

    // printout preimage network weights
    printf ("network\n");
    for (int unsigned dy=0; dy<2; dy++) {
        for (int unsigned y=0; y<siz.y; y++) {
            for (int unsigned dx=0; dx<2; dx++) {
                for (int unsigned x=0; x<siz.x; x++) {
                    printf (" %sy%d%sx%d [", dy?"-":"+", y, dx?"-":"+", x);
                    for (int unsigned n=0; n<ngb_n; n++) {
                        gmp_printf ("%s%Zi", n ? "," : "", net [dy] [dx] [y] [x] [n]);
                    }
                    printf ("]");
                }
                printf ("\n");
            }
            printf ("\n");
        }
        printf ("\n");
    }

    // printout preimage network weights
    printf ("network overview\n");
    for (int unsigned y=0; y<siz.y; y++) {
        for (int unsigned x=0; x<siz.x; x++) {
            mpz_t w;
            mpz_init (w);
            for (int unsigned n=0; n<ngb_n; n++) {
                mpz_t wn;
                mpz_init (wn);
                mpz_set_ui (wn, 1);
                for (int unsigned dy=0; dy<2; dy++) {
                    for (int unsigned dx=0; dx<2; dx++) {
                        mpz_mul (wn, wn, net [dy] [dx] [y] [x] [n]);
                    }
                }
                mpz_add (w, w, wn);
            }
            gmp_printf ("%s%Zi", x ? "," : "", w);
        }
        printf ("\n");
    }
    printf ("\n");



    // test conversion array <-> gmp_t
    ca2d_size_t tsi = {3,3};
    ca2d_size_t tso = {tsi.y-(ca2d.ngb.y-1), tsi.x-(ca2d.ngb.x-1)};
    int unsigned nmi = (size_t) pow (ca2d.sts, tsi.y*tsi.x);
    int unsigned nmo = (size_t) pow (ca2d.sts, tso.y*tso.x);
    mpz_t li [nmi];
    int unsigned lo [nmo];
    for (int unsigned i=0; i<nmo; i++) {
       lo [i] = 0;
    }
    int unsigned tai [tsi.y] [tsi.x];
    int unsigned tao [tso.y] [tso.x];
    mpz_t num;
    mpz_init (num);
    for (int unsigned i=0; i<nmi; i++) {
        mpz_set_ui (num, i);
        ca2d_array_from_mpz (ca2d.sts, tsi, tai, num);
//        ca2d_print (tsi, tai);
        ca2d_forward (ca2d.sts, tsi, ca2d.ngb, ngb_n, tab, tai, tao);
        mpz_init (li[i]);
        ca2d_array_to_mpz (ca2d.sts, tso, tao, li[i]);
        lo [mpz_get_ui (li[i])] ++;
    }
    for (int unsigned i=0; i<nmo; i++) {
        gmp_printf ("%3u --> %3u%s", i, lo[i], (i+1)%8 ? " " : "\n");
    }

    return (0);
}

