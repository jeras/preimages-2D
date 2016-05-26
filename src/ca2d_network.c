// user interface libraries
#include <stdio.h>
//#include <stdlib.h>

// math libraries
//#include <stdint.h>
#include <math.h>
#include <gmp.h>

#include "ca2d.h"
#include "ca2d_rule.h"
#include "ca2d_array.h"
#include "ca2d_configuration.h"

int ca2d_network (ca2d_t ca2d, ca2d_size_t siz, int unsigned ca [siz.y] [siz.x], mpz_t sum) {
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

//            printf ("pointers i: ");
//            ca2d_array_print (ca2d.ngb, a); printf (" :: ");
//            ca2d_array_print (sy, ay); printf (" -> py[%u][%X] = %u | ", y, n, py [y] [n]);
//            printf ("\n");
        }
//        printf ("\n");
    }
    for (int unsigned x=0; x<2; x++) {
        for (int unsigned n=0; n<ngb_n; n++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, n);

            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, x}, sx, a, ax);
            ca2d_array_to_ui (ca2d.sts, sx, ax, &px [x] [n]);

//            printf ("pointers i: ");
//            ca2d_array_print (ca2d.ngb, a); printf (" :: ");
//            ca2d_array_print (sx, ax); printf (" -> px[%u][%X] = %u | ", x, n, px [x] [n]);
//            printf ("\n");
        }
//        printf ("\n");
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
//                    printf ("%sy%d%sx%d: ", dy?"-":"+", y, dx?"-":"+", x);
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
//                        gmp_printf ("n%X(%Zi,%Zi) ", o, wy [o], wx [o]);
                    }
//                    printf ("\n");
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
//    printf ("\n");

    int unsigned dy=0;
    int unsigned dx=0;

//#define CA2D_DEBUG
#ifdef CA2D_DEBUG
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
#endif

//    // printout preimage network weights
//    printf ("network overview\n");
//    for (int unsigned y=0; y<siz.y; y++) {
//        for (int unsigned x=0; x<siz.x; x++) {
//            mpz_t w;
//            mpz_init (w);
//            for (int unsigned n=0; n<ngb_n; n++) {
//                mpz_t wn;
//                mpz_init (wn);
//                mpz_set_ui (wn, 1);
//                for (int unsigned dy=0; dy<2; dy++) {
//                    for (int unsigned dx=0; dx<2; dx++) {
//                        mpz_mul (wn, wn, net [dy] [dx] [y] [x] [n]);
//                    }
//                }
//                mpz_add (w, w, wn);
//            }
//            gmp_printf ("%s%Zi", x ? "," : "", w);
//        }
//        printf ("\n");
//    }
//    printf ("\n");

    // printout preimage network weights
    mpz_t wn;
    mpz_init (wn);
    mpz_set_ui (sum, 0);
    for (int unsigned n=0; n<ngb_n; n++) {
        mpz_set_ui (wn, 1);
        for (int unsigned dy=0; dy<2; dy++) {
            for (int unsigned dx=0; dx<2; dx++) {
                mpz_mul (wn, wn, net [dy] [dx] [siz.y-1] [siz.x-1] [n]);
            }
        }
        mpz_add (sum, sum, wn);
    }

    return (0);
}
