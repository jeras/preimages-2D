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
//#include <stdlib.h>

// math libraries
//#include <stdint.h>
#include <math.h>
#include <gmp.h>

#include "ca2d.h"
#include "ca2d_rule.h"
#include "ca2d_array.h"
#include "ca2d_configuration.h"

int ca2d_network_print (ca2d_t ca2d, ca2d_size_t siz, int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]) {
    // neighborhood states (sts ** ngb_n)
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);
    printf ("[");
    for (int unsigned y=0; y<siz.y; y++) {
        printf ("[");
        for (int unsigned x=0; x<siz.x; x++) {
            printf ("[");
            for (int unsigned n=0; n<ngb_n; n++) {
                printf ("%u", res[y][x][n]);
            }
            printf ("]");
	    printf ("%s", x != (siz.x-1) ? "," : "]\n");
        }
        printf ("%s", y != (siz.y-1) ? "," : "]\n");
    }
    return (0);
}

int ca2d_network_pointers (ca2d_t ca2d,
    int unsigned py [2] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)],
    int unsigned px [2] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]
) {
    // neighborhood states (sts ** ngb_n)
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);

    ca2d_size_t  sy = {ca2d.ngb.y-1, ca2d.ngb.x}; 
    int unsigned ay [sy.y] [sy.x];
 
    ca2d_size_t  sx = {ca2d.ngb.y, ca2d.ngb.x-1}; 
    int unsigned ax [sx.y] [sx.x];
        
    int unsigned a [ca2d.ngb.y] [ca2d.ngb.x];
        
    for (int unsigned y=0; y<2; y++) {
        for (int unsigned n=0; n<ngb_n; n++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, n);

            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {y, 0}, sy, a, ay);
            ca2d_array_to_ui (ca2d.sts, sy, ay, &py [y] [n]);
#ifdef CA2D_DEBUG
            printf ("pointers i: ");
            ca2d_array_print (ca2d.ngb, a); printf (" :: ");
            ca2d_array_print (sy, ay); printf (" -> py[%u][%X] = %u | ", y, n, py [y] [n]);
            printf ("\n");
#endif
        }
#ifdef CA2D_DEBUG
        printf ("\n");
#endif
    }
    for (int unsigned x=0; x<2; x++) {
        for (int unsigned n=0; n<ngb_n; n++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, n);

            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, x}, sx, a, ax);
            ca2d_array_to_ui (ca2d.sts, sx, ax, &px [x] [n]);

#ifdef CA2D_DEBUG
            printf ("pointers i: ");
            ca2d_array_print (ca2d.ngb, a); printf (" :: ");
            ca2d_array_print (sx, ax); printf (" -> px[%u][%X] = %u | ", x, n, px [x] [n]);
            printf ("\n");
#endif
        }
#ifdef CA2D_DEBUG
        printf ("\n");
#endif
    }
    return (0);
}

int ca2d_network (ca2d_t ca2d, ca2d_size_t siz, int unsigned ca [siz.y] [siz.x], int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]) {
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

    // pointers to X/Y dimension edges
    int unsigned py [2] [ngb_n];
    int unsigned px [2] [ngb_n];

    ca2d_network_pointers (ca2d, py, px);

    // overlay sizes
    const int unsigned ovl_x = pow (ca2d.sts, (ca2d.ngb.x-1)*(ca2d.ngb.y  ));
    const int unsigned ovl_y = pow (ca2d.sts, (ca2d.ngb.x  )*(ca2d.ngb.y-1));

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
            for (int unsigned dx=0; dx<2; dx++) {
                for (int x=dx?siz.x-1:0; dx?(x>=0):(x<siz.x); x=dx?x-1:x+1) {
                    int ny = dy ? y+1 : y-1;
                    int nx = dx ? x+1 : x-1;
                    // initialize temporary result array/structure
                    for (int unsigned o=0; o<ovl_y; o++)  mpz_set_ui (wy[o], dy ? y==(siz.y-1) : y==0);
                    for (int unsigned o=0; o<ovl_x; o++)  mpz_set_ui (wx[o], dx ? x==(siz.x-1) : x==0);
#ifdef CA2D_DEBUG
                    // construct temporary result array/structure
                    printf ("%sy%d%sx%d: ", dy?"-":"+", y, dx?"-":"+", x);
#endif
                    for (int unsigned n=0; n<ngb_n; n++) {
                        if (!(dy ? y==(siz.y-1) : y==0)) {
                            mpz_mul (w, net [dy] [0] [ny] [x] [n], net [dy] [1] [ny] [x] [n]);
                            mpz_add (wy [py[1-dy][n]], wy [py[1-dy][n]], w);
                        }
                        if (!(dx ? x==(siz.x-1) : x==0)) {
                            mpz_add (wx [px[1-dx][n]], wx [px[1-dx][n]], net [dy] [dx] [y] [nx] [n]);
                        }
#ifdef CA2D_DEBUG
                        gmp_printf ("n%X(%Zi,%Zi,%x) ", n, wy [py[dy][n]], wx [px[dx][n]], py[dy][n]);
#endif
                    }
#ifdef CA2D_DEBUG
                    for (int unsigned o=0; o<ovl_y; o++) {
                        gmp_printf ("n%X(%Zi,%Zi) ", o, wy [o], wx [o]);
                    }
                    printf ("\n");
#endif
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
#endif

    // printout preimage network weights
    for (int unsigned y=0; y<siz.y; y++) {
        for (int unsigned x=0; x<siz.x; x++) {
            for (int unsigned n=0; n<ngb_n; n++) {
                mpz_t wn;
                mpz_init (wn);
                mpz_set_ui (wn, 1);
                for (int unsigned dy=0; dy<2; dy++) {
                    for (int unsigned dx=0; dx<2; dx++) {
                        mpz_mul (wn, wn, net [dy] [dx] [y] [x] [n]);
                    }
                }
                res [y][x][n] = mpz_sgn (wn);
            }
        }
    }

    return (0);
}

int ca2d_network_preimage (ca2d_t ca2d, ca2d_size_t siz,
    int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)],
    int unsigned ca [siz.y+(ca2d.ngb.y-1)] [siz.x+(ca2d.ngb.x-1)]
) {
    // neighborhood states (sts ** ngb_n)
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);

    int unsigned hit [siz.y] [siz.x];

    // pointers to X/Y dimension edges
    int unsigned py [2] [ngb_n];
    int unsigned px [2] [ngb_n];

    ca2d_network_pointers (ca2d, py, px);

    // overlay sizes
    const int unsigned ovl_x = pow (ca2d.sts, (ca2d.ngb.x-1)*(ca2d.ngb.y  ));
    const int unsigned ovl_y = pow (ca2d.sts, (ca2d.ngb.x  )*(ca2d.ngb.y-1));

    int unsigned wy [ovl_y];
    int unsigned wx [ovl_x];
    int unsigned done;

    // mark network nodes
    for (int unsigned y=0; y<siz.y; y++) {
        for (int unsigned x=0; x<siz.x; x++) {
            for (int unsigned o=0; o<ovl_y; o++)  wy [o] = 0;
            for (int unsigned o=0; o<ovl_x; o++)  wx [o] = 0;
            for (int unsigned n=0; n<ngb_n; n++)  wy [py[1][n]] = y ? hit [y-1] [x] == n : 0x1;
            for (int unsigned n=0; n<ngb_n; n++)  wy [px[1][n]] = x ? hit [y] [x-1] == n : 0x1;

            done = 0;
            for (int unsigned n=0; n<ngb_n; n++) {
                if (!done && res [y] [x] && wy [py[0][n]] && wx [px[0][n]]) {
                    done = 1;
                    hit [y] [x] = n;
                }
            }
        }
    }

    // construct preimage from marked network nodes
    int unsigned a [ca2d.ngb.y] [ca2d.ngb.x];
    // buttom-left area
    for (int unsigned y=0; y<siz.y; y++) {
        for (int unsigned x=0; x<siz.x; x++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, hit [y] [x]);
            ca [y] [x] = a[0][0];
        }
    }
    // right border
    for (int unsigned y=0; y<siz.y; y++) {
        ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, hit [y] [siz.x-1]);
        for (int unsigned x=0; x<(ca2d.ngb.x-1); x++) {
            ca [y] [x+siz.x] = a[0][1+x];
        }
    }
    // top border
    for (int unsigned x=0; x<siz.x; x++) {
        ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, hit [siz.y-1] [x]);
        for (int unsigned y=0; y<(ca2d.ngb.y-1); y++) {
            ca [y+siz.y] [x] = a[1+y][0];
        }
    }
    // top-right corner
    ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, hit [siz.y-1] [siz.x-1]);
    for (int unsigned y=0; y<(ca2d.ngb.y-1); y++) {
        for (int unsigned x=0; x<(ca2d.ngb.x-1); x++) {
            ca [y+siz.y] [x+siz.x] = a[1+y][1+x];
        }
    }

    return (0);
}

