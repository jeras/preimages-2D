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

int ca2d_network_pointers (ca2d_t ca2d, int unsigned p [2] [2] [(size_t) pow (ca2d.sts, (ca2d.ngb.y+1) * (ca2d.ngb.x+1))]) {
    // neighborhood states (sts ** ngb_c)
    ca2d_size_t sc = {ca2d.ngb.y+1, ca2d.ngb.x+1}; 
    uintca_t ngb_c = pow (ca2d.sts, sc.y * sc.x);
    int unsigned ac [sc.y] [sc.x];
    int unsigned a [ca2d.ngb.y] [ca2d.ngb.x];
 
#ifdef CA2D_DEBUG
    printf ("\n");
#endif
    for (int unsigned c=0; c<ngb_c; c++) {
        ca2d_array_from_ui (ca2d.sts, sc, a, c);
        for (int unsigned dy=0; dy<2; dy++) {
            for (int unsigned dx=0; dx<2; dx++) {
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {dy, dx}, (ca2d_size_t) {sc.y-dy, sc.x-dx}, ac, a);
                ca2d_array_to_ui (ca2d.sts, ca2d.ngb, a, &p [dy] [dx] [c]);
#ifdef CA2D_DEBUG
                printf ("pointers i: ");
                ca2d_array_print (ca2d.ngb, a); printf (" :: ");
                ca2d_array_print (sy, ay); printf (" -> p[%u][%u][%u] = %u | ", dy, dx, c, p [dy] [dx] [c]);
                printf ("\n");
#endif
            }
        }
    }
#ifdef CA2D_DEBUG
    printf ("\n");
#endif
    return (0);
}

int ca2d_network (ca2d_t ca2d, ca2d_size_t siz, int unsigned ca [siz.y] [siz.x], int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]) {
    // neighborhood states (sts ** ngb_n)
    const int unsigned ngb_n  = pow (ca2d.sts,  ca2d.ngb.y   * ca2d.ngb.x   );
    const int unsigned ngb_ix = pow (ca2d.sts,  ca2d.ngb.y   * 1            );
    const int unsigned ngb_iy = pow (ca2d.sts,  1            * ca2d.ngb.x   );
    // overlay sizes
    const int unsigned ovl_x  = pow (ca2d.sts, (ca2d.ngb.y  )*(ca2d.ngb.x-1));
    const int unsigned ovl_y  = pow (ca2d.sts, (ca2d.ngb.y-1)*(ca2d.ngb.x  ));

    const int unsigned ngb_c  = pow (ca2d.sts, (ca2d.ngb.y+1)*(ca2d.ngb.x+1));
    int unsigned tab [ngb_n];
    ca2d_rule_table (ca2d, tab);

    // memory allocation for preimage network
    mpz_t net [2] [2] [siz.y] [siz.x] [ngb_n];

    // pointers
    int unsigned p [2] [2] [ngb_c];

    ca2d_network_pointers (ca2d, p);

    // compute network weights
    for (int unsigned dy=0; dy<2; dy++) {
        for (int y=dy?siz.y-1:0; dy?(y>=0):(y<siz.y); y=dy?y-1:y+1) {
            for (int unsigned dx=0; dx<2; dx++) {
                for (int x=dx?siz.x-1:0; dx?(x>=0):(x<siz.x); x=dx?x-1:x+1) {
                    int ny = dy ? y+1 : y-1;
                    int nx = dx ? x+1 : x-1;
                    // initialize result
                    for (int unsigned n=0; n<ngb_n; n++) {
                        mpz_init (net [dy] [dx] [y] [x] [n]);
                    }
                    for (int unsigned c=0; c<ngb_c; c++) {
                        int unsigned wx;
                        int unsigned wy;
                        int unsigned wz;
                        int unsigned p0 = p [1-dy] [1-dx] [c];
                        int unsigned px = p [1-dy] [  dx] [c];
                        int unsigned py = p [  dy] [1-dx] [c];
                        int unsigned pz = p [  dy] [  dx] [c];
                        if (((nx<0) || (nx>=siz.x)) || ((ny<0) || (ny>=siz.y))) {
                            wz = 1;
                        } else {
                            wz = mpz_sgn (net [dy] [0] [ny] [nx] [pz])  &&  mpz_sgn (net [dy] [1] [ny] [nx] [pz]);
                        }
                        if ((nx<0) || (nx>=siz.x)) {
                            wx = 1;
                        } else {
                            wx = mpz_sgn (net [dy] [dx] [y] [nx] [px]);
                        }
                        if ((ny<0) || (ny>=siz.y)) {
                            wy = 1;
                        } else {
                            wy = mpz_sgn (net [dy] [0] [ny] [x] [py])  &&  mpz_sgn (net [dy] [1] [ny] [x] [py]);
                        }
                        mpz_set_ui (net [dy] [dx] [y] [x] [p0] , mpz_sgn (net [dy] [dx] [y] [x] [p0]) || ((tab [p0] == ca [y] [x]) && wz && wz && wx));
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

//int ca2d_network_preimage (ca2d_t ca2d, ca2d_size_t siz,
//    int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)],
//    int unsigned ca [siz.y+(ca2d.ngb.y-1)] [siz.x+(ca2d.ngb.x-1)]
//) {
//    // neighborhood states (sts ** ngb_n)
//    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);
//
//    int unsigned hit [siz.y] [siz.x];
//
//    // pointers to X/Y dimension edges
//    int unsigned py [2] [ngb_n];
//    int unsigned px [2] [ngb_n];
//
//    ca2d_network_pointers (ca2d, py, px);
//
//    // overlay sizes
//    const int unsigned ovl_x = pow (ca2d.sts, (ca2d.ngb.x-1)*(ca2d.ngb.y  ));
//    const int unsigned ovl_y = pow (ca2d.sts, (ca2d.ngb.x  )*(ca2d.ngb.y-1));
//
//    int unsigned wy [ovl_y];
//    int unsigned wx [ovl_x];
//    int unsigned done;
//
//    // mark network nodes
//    printf ("HIT:\n");
//    for (int unsigned y=0; y<siz.y; y++) {
//        // clean pass
//        for (int x=siz.x-1; x>=0; x--) {
//            for (int unsigned o=0; o<ovl_y; o++)  wy [o] = 0;
//            for (int unsigned o=0; o<ovl_x; o++)  wx [o] = 0;
//            for (int unsigned n=0; n<ngb_n; n++)  wy [py[1][n]] |=  y           ? (hit [y-1] [x] == n) : 0x1;
//            for (int unsigned n=0; n<ngb_n; n++)  wx [px[0][n]] |= (x!=siz.x-1) ? (res [y] [x+1] [n])  : 0x1;
//            for (int unsigned o=0; o<ovl_x; o++) {
//                 printf ("-wyx[%x,%x] ", wy [o], wx [o]);
//            }
//            for (int unsigned n=0; n<ngb_n; n++) {
//                res [y][x][n] = res[y][x][n] && wy [py[0][n]] && wx [px[1][n]];
//                printf ("r[%x]=%x ", n, res[y][x][n]);
//            }
////            printf ("%x ", hit [y] [x]);
//            printf ("\n");
//        }
//        printf ("\n");
//        // select pass
//        for (int x=0; x<siz.x; x++) {
//            for (int unsigned o=0; o<ovl_y; o++)  wy [o] = 0;
//            for (int unsigned o=0; o<ovl_x; o++)  wx [o] = 0;
//            for (int unsigned n=0; n<ngb_n; n++)  wy [py[1][n]] |= y ? (hit [y-1] [x] == n) : 0x1;
//            for (int unsigned n=0; n<ngb_n; n++)  wx [px[1][n]] |= x ? (hit [y] [x-1] == n) : 0x1;
//            for (int unsigned o=0; o<ovl_x; o++) {
//                 printf ("+wyx[%x,%x] ", wy [o], wx [o]);
//            }
//            done = 0;
//            for (int unsigned n=0; n<ngb_n; n++) {
//                if (!done && res[y][x][n] && wy [py[0][n]] && wx [px[0][n]]) {
//                    done = 1;
//                    hit [y] [x] = n;
//                }
//                printf ("r[%x]=%x ", n, res[y][x][n]);
//            }
//            if (!done) {
//                printf ("ERROR, no preimages\n");
//                return (1);
//            }
//            printf ("%x ", hit [y] [x]);
//            printf ("\n");
//        }
//        printf ("\n");
//    }
//    printf ("\n");
//
//    // construct preimage from marked network nodes
//    int unsigned a [ca2d.ngb.y] [ca2d.ngb.x];
//    // buttom-left area
//    for (int unsigned y=0; y<siz.y; y++) {
//        for (int unsigned x=0; x<siz.x; x++) {
//            ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, hit [y] [x]);
//            ca [y] [x] = a[0][0];
//        }
//    }
//    // right border
//    for (int unsigned y=0; y<siz.y; y++) {
//        ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, hit [y] [siz.x-1]);
//        for (int unsigned x=0; x<(ca2d.ngb.x-1); x++) {
//            ca [y] [x+siz.x] = a[0][1+x];
//        }
//    }
//    // top border
//    for (int unsigned x=0; x<siz.x; x++) {
//        ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, hit [siz.y-1] [x]);
//        for (int unsigned y=0; y<(ca2d.ngb.y-1); y++) {
//            ca [y+siz.y] [x] = a[1+y][0];
//        }
//    }
//    // top-right corner
//    ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, hit [siz.y-1] [siz.x-1]);
//    for (int unsigned y=0; y<(ca2d.ngb.y-1); y++) {
//        for (int unsigned x=0; x<(ca2d.ngb.x-1); x++) {
//            ca [y+siz.y] [x+siz.x] = a[1+y][1+x];
//        }
//    }
//
//    return (0);
//}

