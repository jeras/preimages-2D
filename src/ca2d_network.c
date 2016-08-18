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

// tables for geting overlap values from neighborhood values
static int ca2d_network_table_n2o (ca2d_t ca2d, int unsigned n2o_y [2] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)],
                                         int unsigned n2o_x [2] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]) {
    ca2d_size_t sn = {ca2d.ngb.y  , ca2d.ngb.x  }; 
    ca2d_size_t sx = {ca2d.ngb.y  , ca2d.ngb.x-1}; 
    ca2d_size_t sy = {ca2d.ngb.y-1, ca2d.ngb.x  }; 
    int unsigned an [sn.y] [sn.x];
    int unsigned ax [sx.y] [sx.x];
    int unsigned ay [sy.y] [sy.x];
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);
 
    for (int unsigned n=0; n<ngb_n; n++) {
        // neighborhood integer is converted into array
        ca2d_array_from_ui (ca2d.sts, sn, an, n);
        for (int unsigned dy=0; dy<2; dy++) {
            ca2d_array_slice (sn, (ca2d_size_t) {dy, 0}, sy, an, ay);
            ca2d_array_to_ui (ca2d.sts, sy, ay, &n2o_y [dy] [n]);
        }
        for (int unsigned dx=0; dx<2; dx++) {
            ca2d_array_slice (sn, (ca2d_size_t) {0, dx}, sx, an, ax);
            ca2d_array_to_ui (ca2d.sts, sx, ax, &n2o_x [dx] [n]);
        }
    }
    return (0);
}

static int ca2d_network_table_n2o_print (ca2d_t ca2d, int unsigned n2o_y [2] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)],
                                                      int unsigned n2o_x [2] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]) {
    ca2d_size_t sn = {ca2d.ngb.y  , ca2d.ngb.x  }; 
    ca2d_size_t sx = {ca2d.ngb.y  , ca2d.ngb.x-1}; 
    ca2d_size_t sy = {ca2d.ngb.y-1, ca2d.ngb.x  }; 
    int unsigned an [sn.y] [sn.x];
    int unsigned ax [sx.y] [sx.x];
    int unsigned ay [sy.y] [sy.x];
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);
 
    for (int unsigned n=0; n<ngb_n; n++) {
        // neighborhood integer is converted into array
        ca2d_array_from_ui (ca2d.sts, sn, an, n);
        printf ("n=%x -> ", n);
        ca2d_array_print (sn, an);
        for (int unsigned dy=0; dy<2; dy++) {
            ca2d_array_from_ui (ca2d.sts, sy, ay, n2o_y[dy][n]);
            printf (" n2o_y[%u]=%x -> ", dy, n2o_y[dy][n]);
            ca2d_array_print (sy, ay);
        }
        for (int unsigned dx=0; dx<2; dx++) {
            ca2d_array_from_ui (ca2d.sts, sx, ax, n2o_x[dx][n]);
            printf (" n2o_x[%u]=%x -> ", dx, n2o_x[dx][n]);
            ca2d_array_print (sx, ax);
        }
        printf ("\n");
    }
    return (0);
}

// tables for geting neighborhood values from overlap values and rest
int ca2d_network_table_o2n (ca2d_t ca2d, int unsigned o2n_y [2] [(size_t) pow (ca2d.sts, (ca2d.ngb.y-1) *  ca2d.ngb.x   )] [(size_t) pow (ca2d.sts, ca2d.ngb.x)],
                                         int unsigned o2n_x [2] [(size_t) pow (ca2d.sts,  ca2d.ngb.y    * (ca2d.ngb.x-1))] [(size_t) pow (ca2d.sts, ca2d.ngb.y)]) {
    ca2d_size_t sn  = {ca2d.ngb.y  , ca2d.ngb.x  }; 
    ca2d_size_t sox = {ca2d.ngb.y  , ca2d.ngb.x-1}; 
    ca2d_size_t soy = {ca2d.ngb.y-1, ca2d.ngb.x  }; 
    ca2d_size_t srx = {ca2d.ngb.y  ,            1}; 
    ca2d_size_t sry = {           1, ca2d.ngb.x  }; 
    int unsigned an  [sn.y]  [sn.x] ;
    int unsigned aox [sox.y] [sox.x];
    int unsigned aoy [soy.y] [soy.x];
    int unsigned arx [srx.y] [srx.x];
    int unsigned ary [sry.y] [sry.x];
    int unsigned ox;
    int unsigned oy;
    int unsigned rx;
    int unsigned ry;
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);

    // for every neighborhood integer
    for (int unsigned n=0; n<ngb_n; n++) {
        // neighborhood integer is converted into array
        ca2d_array_from_ui (ca2d.sts, sn, an, n);
        for (int unsigned dy=0; dy<2; dy++) {
            // neighborhood array is split into overlay and rest arrays
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {       dy, 0}, soy, an, aoy);
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {sn.y-1-dy, 0}, sry, an, ary);
            // overlay and rest arrays are converted into integers
            ca2d_array_to_ui (ca2d.sts, soy, aoy, &oy);
            ca2d_array_to_ui (ca2d.sts, sry, ary, &ry);
            // table is pupulated
            o2n_y [dy] [oy] [ry] = n;
        }
        for (int unsigned dx=0; dx<2; dx++) {
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0,        dx}, sox, an, aox);
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, sn.x-1-dx}, srx, an, arx);
            ca2d_array_to_ui (ca2d.sts, sox, aox, &ox);
            ca2d_array_to_ui (ca2d.sts, srx, arx, &rx);
            o2n_y [dx] [ox] [rx] = n;
        }
        printf ("\n");
    }
    return (0);
}

// tables for geting neighborhood values from overlap values and rest
// TODO: use a different strategy or just remove this debug code
static int ca2d_network_table_o2n_print (ca2d_t ca2d, int unsigned o2n_y [2] [(size_t) pow (ca2d.sts, (ca2d.ngb.y-1) *  ca2d.ngb.x   )] [(size_t) pow (ca2d.sts, ca2d.ngb.x)],
                                                      int unsigned o2n_x [2] [(size_t) pow (ca2d.sts,  ca2d.ngb.y    * (ca2d.ngb.x-1))] [(size_t) pow (ca2d.sts, ca2d.ngb.y)]) {
    ca2d_size_t sn  = {ca2d.ngb.y  , ca2d.ngb.x  }; 
    ca2d_size_t sox = {ca2d.ngb.y  , ca2d.ngb.x-1}; 
    ca2d_size_t soy = {ca2d.ngb.y-1, ca2d.ngb.x  }; 
    ca2d_size_t srx = {ca2d.ngb.y  ,            1}; 
    ca2d_size_t sry = {           1, ca2d.ngb.x  }; 
    int unsigned an  [sn.y]  [sn.x] ;
    int unsigned aox [sox.y] [sox.x];
    int unsigned aoy [soy.y] [soy.x];
    int unsigned arx [srx.y] [srx.x];
    int unsigned ary [sry.y] [sry.x];
    int unsigned ox;
    int unsigned oy;
    int unsigned rx;
    int unsigned ry;
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);

    // for every neighborhood integer
    for (int unsigned n=0; n<ngb_n; n++) {
        // neighborhood integer is converted into array
        ca2d_array_from_ui (ca2d.sts, sn, an, n);
        printf ("n=%x -> ", n);
        ca2d_array_print (sn, an);
        for (int unsigned dy=0; dy<2; dy++) {
            // neighborhood array is split into overlay and rest arrays
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {       dy, 0}, soy, an, aoy);
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {sn.y-1-dy, 0}, sry, an, ary);
            // overlay and rest arrays are converted into integers
            ca2d_array_to_ui (ca2d.sts, soy, aoy, &oy);
            ca2d_array_to_ui (ca2d.sts, sry, ary, &ry);
            printf (" oy[%u]=%x -> ", dy, oy);
            ca2d_array_print (soy, aoy);
            printf (" ry[%u]=%x -> ", dy, ry);
            ca2d_array_print (sry, ary);
            printf (" o2n_y[%u][%x][%x]=%x -> ", dy, oy, ry, o2n_y [dy] [oy] [ry]);
        }
        for (int unsigned dx=0; dx<2; dx++) {
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0,        dx}, sox, an, aox);
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, sn.x-1-dx}, srx, an, arx);
            ca2d_array_to_ui (ca2d.sts, sox, aox, &ox);
            ca2d_array_to_ui (ca2d.sts, srx, arx, &rx);
            printf (" ox[%u]=%x -> ", dx, ox);
            ca2d_array_print (sox, aox);
            printf (" rx[%u]=%x -> ", dx, rx);
            ca2d_array_print (srx, arx);
            printf (" o2n_x[%u][%x][%x]=%x -> ", dx, ox, rx, o2n_x [dx] [ox] [rx]);
        }
        printf ("\n");
    }
    return (0);
}

// function for geting array of overlap integers from edge integer in X dimension
int ca2d_network_ex2o (ca2d_t ca2d, ca2d_size_t siz, int unsigned e, int unsigned o [siz.x]) {
    return (0);
}

// function for geting edge integer in X dimension from array of overlap integers
int ca2d_network_o2ex (ca2d_t ca2d, ca2d_size_t siz, int unsigned e, int unsigned o [siz.x]) {
    return (0);
}

static int ca1d_network (ca2d_t ca2d, size_t siz, int unsigned ca [siz], int unsigned dy, int unsigned edg_i, int unsigned *edgn, int unsigned edg_o [*edgn]) {
    // neighborhood states (sts ** ngb_n)
    const int unsigned ngb_n  = pow (ca2d.sts,  ca2d.ngb.y   * ca2d.ngb.x   );
    // overlay sizes
    const int unsigned ovl_x  = pow (ca2d.sts, (ca2d.ngb.y  )*(ca2d.ngb.x-1));
    const int unsigned ovl_y  = pow (ca2d.sts, (ca2d.ngb.y-1)*(ca2d.ngb.x  ));
    // leftover sizes
    const int unsigned lft_x  = pow (ca2d.sts, (ca2d.ngb.y  )               );
    const int unsigned lft_y  = pow (ca2d.sts,                (ca2d.ngb.x  ));

    int unsigned tab [ngb_n];
    ca2d_rule_table (ca2d, tab);

    // tables
    int unsigned n2o_y [2] [ngb_n];
    int unsigned n2o_x [2] [ngb_n];
    int unsigned o2n_y [2] [ovl_y] [lft_y];
    int unsigned o2n_x [2] [ovl_x] [lft_x];
    ca2d_network_table_n2o (ca2d, n2o_y, n2o_x);
    ca2d_network_table_o2n (ca2d, o2n_y, o2n_x);
    printf ("DEBUG 1\n");
//    ca2d_network_table_n2o_print (ca2d, n2o_y, n2o_x);
//    ca2d_network_table_o2n_print (ca2d, o2n_y, o2n_x);
    printf ("DEBUG 2\n");

    // memory allocation for preimage network
    int unsigned net [siz] [ovl_x];

    // create unprocessed 1D preimage network for the current edge
    for (int x=0; x<siz; x++) {
        // initialize network weights to zero
        for (int o=0; o<ovl_x; o++) {
            net [x] [o] = 0;
        }
        int unsigned c = ca [x];
        // get segment x from current edge
        // for all neighborhoods with the curent edge check them against the table
        for (int unsigned lft=0; lft<lft_x; lft++) {
            // combine edge and leftover into neighborhood
            int unsigned n=0;
            // check neighborhood against the rule
            if (c == tab[n]) {
                // get end edge overlap from pointer table
                int unsigned o = n2o_y [dy?0:1] [n];
                // set weight to overlap
                net [x] [o] = 1;
            }
        }
        // 
    }
    return (0);
}

int ca2d_network (ca2d_t ca2d, ca2d_size_t siz, int unsigned ca [siz.y] [siz.x], int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]) {
    // edge size
    const int unsigned edg_x  = pow (ca2d.sts,  (ca2d.ngb.y-1)       *((ca2d.ngb.x-1)+siz.x));
    const int unsigned edg_y  = pow (ca2d.sts, ((ca2d.ngb.y-1)+siz.y)* (ca2d.ngb.x-1)       );

    // memory allocation for preimage network
    mpz_t net [siz.y+1] [edg_x];

    // compact list of edges
    int unsigned edgn = edg_x;
    int unsigned edgs [edgn];

    // initialize array variable
    for (int y=0; y<=siz.y; y++) {
        for (int unsigned e=0; e<edg_x; e++) {
            mpz_init (net [y] [e]);
        }
    }
    // initialize starting edge to unit (open edge)
    // TODO: this can be generalised, provided as input
    for (int unsigned e=0; e<edg_x; e++) {
        mpz_set_ui (net [0] [e], 1);
    }

    int unsigned dy = 0;

    // compute network weights
    for (int y=0; y<siz.y; y++) {
        // loop over all edges
        for (int unsigned e=0; e<edg_x; e++) {
            // only process edge if it's weight is not zero
            if (net [y] [e]) {
                edgn = edg_x;
                ca1d_network (ca2d, siz.x, ca [y], dy, e, &edgn, edgs);
            }
            // count 1D network preimages

            // 1D network preimages are end edges, put their weights into the list
        }
    }
    return (0);
}

