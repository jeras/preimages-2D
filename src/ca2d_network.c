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
//#include <stdint.h>
#include <math.h>
#include <gmp.h>

#include "ca2d.h"
#include "ca2d_rule.h"
#include "ca2d_array.h"
#include "ca2d_configuration.h"

int ca2d_network_print (ca2d_t ca2d, ca2d_size_t siz, int unsigned res [siz.y] [siz.x] [ca2d.ngb.n]) {
    printf ("[");
    for (int unsigned y=0; y<siz.y; y++) {
        printf ("[");
        for (int unsigned x=0; x<siz.x; x++) {
            printf ("[");
            for (int unsigned n=0; n<ca2d.ngb.n; n++) {
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
static int ca2d_network_table_v2o (ca2d_t ca2d, int unsigned v2o_y [2] [ca2d.ovl.y.n] [ca2d.shf.y.n],
                                                int unsigned v2o_x [2] [ca2d.ovl.x.n] [ca2d.shf.x.n]) {
    int unsigned ovl_ay [ca2d.ovl.y.y] [ca2d.ovl.y.x];
    int unsigned ovl_ax [ca2d.ovl.x.y] [ca2d.ovl.x.x];
    int unsigned ovl_ayo[ca2d.ovl.y.y] [ca2d.ovl.y.x];
    int unsigned ovl_axo[ca2d.ovl.x.y] [ca2d.ovl.x.x];
    int unsigned shf_ay [ca2d.shf.y.y] [ca2d.shf.y.x];
    int unsigned shf_ax [ca2d.shf.x.y] [ca2d.shf.x.x];
    int unsigned ver_a  [ca2d.ver.y  ] [ca2d.ver.x  ];

    for (int unsigned ovl=0; ovl<ca2d.ovl.y.n; ovl++) {
        ca2d_array_from_ui (ca2d.sts, ca2d.ovl.y, ovl_ay, ovl);
        for (int unsigned shf=0; shf<ca2d.shf.y.n; shf++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.shf.y, shf_ay, shf);
            for (int unsigned d=0; d<2; d++) {
                ca2d_array_slice (ca2d.ovl.y, (ca2d_size_t) {0, d}, ca2d.ver, ovl_ay, ver_a);
                if (!d)  ca2d_array_combine_x (ca2d.shf.y, ca2d.ver, shf_ay, ver_a, ovl_ayo);  // shift added to the left  of overlap
                else     ca2d_array_combine_x (ca2d.ver, ca2d.shf.y, ver_a, shf_ay, ovl_ayo);  // shift added to the right of overlap
                ca2d_array_to_ui (ca2d.sts, ca2d.ovl.y, ovl_ayo, &v2o_y [d] [ovl] [shf]);
            }
        }
    }
    for (int unsigned ovl=0; ovl<ca2d.ovl.x.n; ovl++) {
        ca2d_array_from_ui (ca2d.sts, ca2d.ovl.x, ovl_ax, ovl);
        for (int unsigned shf=0; shf<ca2d.shf.x.n; shf++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.shf.x, shf_ax, shf);
            for (int unsigned d=0; d<2; d++) {
                ca2d_array_slice (ca2d.ovl.x, (ca2d_size_t) {d, 0}, ca2d.ver, ovl_ax, ver_a);
                if (!d)  ca2d_array_combine_y (ca2d.shf.x, ca2d.ver, shf_ax, ver_a, ovl_axo);  // shift added to the left  of overlap
                else     ca2d_array_combine_y (ca2d.ver, ca2d.shf.x, ver_a, shf_ax, ovl_axo);  // shift added to the right of overlap
                ca2d_array_to_ui (ca2d.sts, ca2d.ovl.x, ovl_axo, &v2o_x [d] [ovl] [shf]);
            }
        }
    }
    return (0);
}

// tables for geting overlap values from neighborhood values
static int ca2d_network_table_v2o_print (ca2d_t ca2d, int unsigned v2o_y [2] [ca2d.ovl.y.n] [ca2d.shf.y.n],
                                                      int unsigned v2o_x [2] [ca2d.ovl.x.n] [ca2d.shf.x.n]) {
    int unsigned ovl_ay [ca2d.ovl.y.y] [ca2d.ovl.y.x];
    int unsigned ovl_ax [ca2d.ovl.x.y] [ca2d.ovl.x.x];
    int unsigned ovl_ayo[ca2d.ovl.y.y] [ca2d.ovl.y.x];
    int unsigned ovl_axo[ca2d.ovl.x.y] [ca2d.ovl.x.x];
    int unsigned shf_ay [ca2d.shf.y.y] [ca2d.shf.y.x];
    int unsigned shf_ax [ca2d.shf.x.y] [ca2d.shf.x.x];
    int unsigned ver_a  [ca2d.ver.y  ] [ca2d.ver.x  ];

    for (int unsigned ovl=0; ovl<ca2d.ovl.y.n; ovl++) {
        ca2d_array_from_ui (ca2d.sts, ca2d.ovl.y, ovl_ay, ovl);
        printf (" oy=%x -> ", ovl);
        ca2d_array_print (ca2d.ovl.y, ovl_ay);
        for (int unsigned shf=0; shf<ca2d.shf.y.n; shf++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.shf.y, shf_ay, shf);
            printf (" shf=%x -> ", shf);
            ca2d_array_print (ca2d.shf.y, shf_ay);
            for (int unsigned d=0; d<2; d++) {
                ca2d_array_from_ui (ca2d.sts, ca2d.ovl.y, ovl_ayo, v2o_y[d][ovl][shf]);
                printf (" v2o_y[%u][%x][%x]=%x -> ", d, ovl, shf, v2o_y[d][ovl][shf]);
                ca2d_array_print (ca2d.ovl.y, ovl_ayo);
            }
        }
        printf ("\n");
    }
    return (0);
}

// tables for geting overlap values from neighborhood values
static int ca2d_network_table_n2o (ca2d_t ca2d, int unsigned n2o_y [2] [ca2d.ngb.n],
                                                int unsigned n2o_x [2] [ca2d.ngb.n]) {
    int unsigned ovl_ay [ca2d.ovl.y.y] [ca2d.ovl.y.x];
    int unsigned ovl_ax [ca2d.ovl.x.y] [ca2d.ovl.x.x];
    int unsigned ngb_a  [ca2d.ngb.y  ] [ca2d.ngb.x  ];
 
    for (int unsigned ngb=0; ngb<ca2d.ngb.n; ngb++) {
        // neighborhood integer is converted into array
        ca2d_array_from_ui (ca2d.sts, ca2d.ngb, ngb_a, ngb);
        for (int unsigned d=0; d<2; d++) {
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {d, 0}, ca2d.ovl.y, ngb_a, ovl_ay);
            ca2d_array_to_ui (ca2d.sts, ca2d.ovl.y, ovl_ay, &n2o_y [d] [ngb]);
        }
        for (int unsigned d=0; d<2; d++) {
            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, d}, ca2d.ovl.x, ngb_a, ovl_ax);
            ca2d_array_to_ui (ca2d.sts, ca2d.ovl.x, ovl_ax, &n2o_x [d] [ngb]);
        }
    }
    return (0);
}

static int ca2d_network_table_n2o_print (ca2d_t ca2d, int unsigned n2o_y [2] [ca2d.ngb.n],
                                                      int unsigned n2o_x [2] [ca2d.ngb.n]) {
    int unsigned ovl_ay [ca2d.ovl.y.y] [ca2d.ovl.y.x];
    int unsigned ovl_ax [ca2d.ovl.x.y] [ca2d.ovl.x.x];
    int unsigned ngb_a  [ca2d.ngb.y  ] [ca2d.ngb.x  ];
 
    for (int unsigned ngb=0; ngb<ca2d.ngb.n; ngb++) {
        // neighborhood integer is converted into array
        ca2d_array_from_ui (ca2d.sts, ca2d.ngb, ngb_a, ngb);
        printf ("n=%x -> ", ngb);
        ca2d_array_print (ca2d.ngb, ngb_a);
        for (int unsigned d=0; d<2; d++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.ovl.y, ovl_ay, n2o_y[d][ngb]);
            printf (" n2o_y[%u]=%x -> ", d, n2o_y[d][ngb]);
            ca2d_array_print (ca2d.ovl.y, ovl_ay);
        }
        for (int unsigned d=0; d<2; d++) {
            ca2d_array_from_ui (ca2d.sts, ca2d.ovl.x, ovl_ax, n2o_x[d][ngb]);
            printf (" n2o_x[%u]=%x -> ", d, n2o_x[d][ngb]);
            ca2d_array_print (ca2d.ovl.x, ovl_ax);
        }
        printf ("\n");
    }
    return (0);
}

// tables for geting neighborhood values from overlap values and rest
int ca2d_network_table_o2n (ca2d_t ca2d, int unsigned o2n_y [2] [ca2d.ovl.y.n] [ca2d.rem.x.n],
                                         int unsigned o2n_x [2] [ca2d.ovl.x.n] [ca2d.rem.y.n]) {
    int unsigned ovl_ay [ca2d.ovl.y.y] [ca2d.ovl.y.x];
    int unsigned ovl_ax [ca2d.ovl.x.y] [ca2d.ovl.x.x];
    int unsigned rem_ay [ca2d.rem.y.y] [ca2d.rem.y.x];
    int unsigned rem_ax [ca2d.rem.x.y] [ca2d.rem.x.x];
    int unsigned ngb_a  [ca2d.ngb.y  ] [ca2d.ngb.x  ];
    int unsigned ovl_y;
    int unsigned ovl_x;
    int unsigned rem_y;
    int unsigned rem_x;

    // for every neighborhood integer
    for (int unsigned ngb=0; ngb<ca2d.ngb.n; ngb++) {
        // neighborhood integer is converted into array
        ca2d_array_from_ui (ca2d.sts, ca2d.ngb, ngb_a, ngb);
        for (int unsigned d=0; d<2; d++) {
            // neighborhood array is split into overlay and reminder arrays
            if (!d) {
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0           , 0}, ca2d.ovl.y, ngb_a, ovl_ay);
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {ca2d.ovl.y.y, 0}, ca2d.rem.y, ngb_a, rem_ay);
            } else {
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0           , 0}, ca2d.rem.y, ngb_a, rem_ay);
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {ca2d.rem.y.y, 0}, ca2d.ovl.y, ngb_a, ovl_ay);
            }
            // overlay and reminder arrays are converted into integers
            ca2d_array_to_ui (ca2d.sts, ca2d.ovl.y, ovl_ay, &ovl_y);
            ca2d_array_to_ui (ca2d.sts, ca2d.rem.y, rem_ay, &rem_y);
            // table is pupulated
            o2n_y [d] [ovl_y] [rem_y] = ngb;
        }
        for (int unsigned d=0; d<2; d++) {
            // neighborhood array is split into overlay and reminder arrays
            if (!d) {
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0           , 0}, ca2d.ovl.x, ngb_a, ovl_ax);
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {ca2d.ovl.x.x, 0}, ca2d.rem.x, ngb_a, rem_ax);
            } else {
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0           , 0}, ca2d.rem.x, ngb_a, rem_ax);
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {ca2d.rem.x.x, 0}, ca2d.ovl.x, ngb_a, ovl_ax);
            }
            ca2d_array_to_ui (ca2d.sts, ca2d.ovl.x, ovl_ax, &ovl_x);
            ca2d_array_to_ui (ca2d.sts, ca2d.rem.x, rem_ax, &rem_x);
            o2n_x [d] [ovl_x] [rem_x] = ngb;
        }
    }
    return (0);
}

// tables for geting neighborhood values from overlap values and rest
static int ca2d_network_table_o2n_print (ca2d_t ca2d, int unsigned o2n_y [2]  [ca2d.ovl.y.n] [ca2d.rem.x.n],
                                                      int unsigned o2n_x [2]  [ca2d.ovl.x.n] [ca2d.rem.y.n]) {
    int unsigned ovl_ay [ca2d.ovl.y.y] [ca2d.ovl.y.x];
    int unsigned ovl_ax [ca2d.ovl.x.y] [ca2d.ovl.x.x];
    int unsigned rem_ay [ca2d.rem.y.y] [ca2d.rem.y.x];
    int unsigned rem_ax [ca2d.rem.x.y] [ca2d.rem.x.x];
    int unsigned ngb_a  [ca2d.ngb.y  ] [ca2d.ngb.x  ];

    // for every overlap integer
    for (int unsigned ovl=0; ovl<ca2d.ovl.y.n; ovl++) {
        // for every overlap integer
        for (int unsigned rem=0; rem<ca2d.rem.y.n; rem++) {
            for (int unsigned d=0; d<2; d++) {
                // overlay and reminder arrays are converted into integers
//                ca2d_array_from_ui (ca2d.sts, ca2d.ovl.y, ovl_ay, ovl);
//                printf (" oy[%u]=%x -> ", d, ovl);
//                ca2d_array_print (ca2d.ovl.y, ovl_ay);
//                ca2d_array_from_ui (ca2d.sts, ca2d.rem.y, rem_ay, rem);
//                printf (" ry[%u]=%x -> ", d, rem);
//                ca2d_array_print (ca2d.rem.y, rem_ay);
                ca2d_array_from_ui (ca2d.sts, ca2d.ngb, ngb_a, o2n_y [d] [ovl] [rem]);
                printf (" n=o2n_y[%u][%x][%x]=%x -> ", d, ovl, rem, o2n_y [d] [ovl] [rem]);
                ca2d_array_print (ca2d.ngb, ngb_a);
            }
            printf ("\n");
        }
//        for (int unsigned d=0; d<2; d++) {
//            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0,              d}, ca2d.ovl.x, ngb_a, ovl_ax);
//            ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, ca2d.ngb.x-1-d}, ca2d.rem.x, ngb_a, rem_ax);
//            ca2d_array_to_ui (ca2d.sts, ca2d.ovl.x, ovl_ax, &ovl_x);
//            ca2d_array_to_ui (ca2d.sts, ca2d.rem.x, rem_ax, &rem_x);
//            printf (" ox[%u]=%x -> ", d, ovl_x);
//            ca2d_array_print (ca2d.ovl.x, ovl_ax);
//            printf (" rx[%u]=%x -> ", d, rem_x);
//            ca2d_array_print (ca2d.rem.x, rem_ax);
//            printf (" o2n_x[%u][%x][%x]=%x -> ", d, ovl_x, rem_x, o2n_x [d] [ovl_x] [rem_x]);
//        }
        printf ("\n");
    }
    return (0);
}

// function for geting array of overlap integers from edge integer in X dimension
int ca2d_network_ex2o (ca2d_t ca2d, size_t siz, int unsigned e, int unsigned o [siz]) {
    ca2d_size_t se = {ca2d.ngb.y-1, ca2d.ngb.x-1 + siz};
    ca2d_size_t so = {ca2d.ngb.y-1, ca2d.ngb.x  };
    int unsigned ae [se.y] [se.x];
    int unsigned ao [so.y] [so.x];
    ca2d_array_from_ui (ca2d.sts, se, ae, e);
    for (int unsigned x=0; x<siz; x++) {
        ca2d_array_slice (se, (ca2d_size_t) {0, x}, so, ae, ao);
        ca2d_array_to_ui (ca2d.sts, so, ao, &o[x]);
    }
    return (0);
}

// function for geting edge integer in X dimension from array of overlap integers
int ca2d_network_o2ex (ca2d_t ca2d, size_t siz, int unsigned *e, int unsigned o [siz]) {
    ca2d_size_t se = {ca2d.ngb.y-1, ca2d.ngb.x-1 + siz};
    ca2d_size_t so = {ca2d.ngb.y-1, ca2d.ngb.x  };
    ca2d_size_t sr = {           1, ca2d.ngb.x  };
    int unsigned ae [se.y] [se.x];
    int unsigned ao [so.y] [so.x];
    int unsigned ar [sr.y] [sr.x];
    // set first overlap
    ca2d_array_from_ui (ca2d.sts, so, ao, o[0]);
    ca2d_array_fit (se, (ca2d_size_t) {0, 0}, so, ae, ao);
    // apprend remaining remainders
    for (int unsigned x=0; x<siz; x++) {
        ca2d_array_from_ui (ca2d.sts, so, ao, o[x]);
        ca2d_array_slice (so, (ca2d_size_t) {0,   ca2d.ngb.x-1}, sr, ao, ar);
        ca2d_array_fit   (se, (ca2d_size_t) {0, x+ca2d.ngb.x-1}, sr, ae, ar);
    }
    ca2d_array_to_ui (ca2d.sts, se, ae, &(*e));
    return (0);
}

// print CA state
static int ca1d_network_print (ca2d_t ca2d, size_t siz, int unsigned net [siz] [ca2d.ovl.y.n]) {
    for (int unsigned ovl=0; ovl<ca2d.ovl.y.n; ovl++) {
        printf ("CA [o=%u]:", ovl);
        for (int unsigned x=0; x<siz; x++) {
            printf (" %u", net [x] [ovl]);
        }
        printf ("\n");
    }
    return (0);
}

static int ca1d_network (ca2d_t ca2d, size_t siz, int unsigned ca [siz], int unsigned dy, int unsigned edg_i, mpz_t edg_w, int unsigned *edg_n, mpz_t edg_o [(size_t) pow (ca2d.sts, (ca2d.ngb.y-1)*((ca2d.ngb.x-1)+siz))]) {
    int unsigned tab [ca2d.ngb.n];
    ca2d_rule_table (ca2d, tab);

    // tables
    int unsigned n2o_y [2] [ca2d.ngb.n];
    int unsigned n2o_x [2] [ca2d.ngb.n];
    int unsigned o2n_y [2] [ca2d.ovl.y.n] [ca2d.rem.y.n];
    int unsigned o2n_x [2] [ca2d.ovl.x.n] [ca2d.rem.x.n];
    ca2d_network_table_n2o (ca2d, n2o_y, n2o_x);
    ca2d_network_table_o2n (ca2d, o2n_y, o2n_x);
//    ca2d_network_table_n2o_print (ca2d, n2o_y, n2o_x);
//    ca2d_network_table_o2n_print (ca2d, o2n_y, o2n_x);

    // memory allocation for preimage network
    int unsigned net [siz] [ca2d.ovl.y.n];

//    printf ("DEBUG: before edge conversion\n");
    // convert input edge into array of overlaps
    int unsigned ovl_i [siz];
    ca2d_network_ex2o (ca2d, siz, edg_i, ovl_i);
//    printf ("edg=%x", edg_i);
//    for (int x=0; x<siz; x++)
//        printf (" %x", ovl_i[x]);
//        //ca2d_array_print (ca2d.ovl.y, ovl_i[x]);
//    printf ("\n");
//    printf ("DEBUG: after edge conversion\n");


    int unsigned ngb_a  [ca2d.ngb.y  ] [ca2d.ngb.x  ];
    // create unprocessed 1D preimage network for the current edge
    for (int unsigned x=0; x<siz; x++) {
        // initialize network weights to zero
        for (int unsigned ovl=0; ovl<ca2d.ovl.y.n; ovl++) {
            net [x] [ovl] = 0;
        }
        // get segment x from current edge
        int unsigned o = ovl_i [x];
        // for all neighborhoods with the curent edge check them against the table
        for (int unsigned rem=0; rem<ca2d.rem.x.n; rem++) {
            // combine overlap and remainder into neighborhood
            int unsigned ngb = o2n_y [dy] [o] [rem];
            // check neighborhood against the rule
//            ca2d_array_from_ui (ca2d.sts, ca2d.ngb, ngb_a, ngb);
//            printf (" n=%x -> ", ngb);
//            ca2d_array_print (ca2d.ngb, ngb_a);
            if (ca[x] == tab[ngb]) {
                // get end edge overlap from pointer table
                int unsigned ovl = n2o_y [1-dy] [ngb];
                // set weight to overlap
                net [x] [ovl] = 1;
            }
        }
//        printf ("\n");
    }
    printf ("DEBUG: after edge is applied\n");
    ca1d_network_print (ca2d, siz, net);

    // count 1D network preimages
    int unsigned v2o_y [2] [ca2d.ovl.y.n] [ca2d.shf.y.n];
    int unsigned v2o_x [2] [ca2d.ovl.x.n] [ca2d.shf.x.n];

    ca2d_network_table_v2o (ca2d, v2o_y, v2o_x);
//    ca2d_network_table_v2o_print (ca2d, v2o_y, v2o_x);
    printf ("DEBUG: vertice teble created\n");

    for (int x=1; x<siz; x++) {
        for (int unsigned ovl=0; ovl<ca2d.ovl.y.n; ovl++) {
            // first check if the path is available
            if (net [x] [ovl]) {
                int unsigned sum = 0;
                for (int unsigned shf=0; shf<ca2d.shf.y.n; shf++) {
                    sum += net [x-1] [v2o_y[0][ovl][shf]];
                }
                net [x] [ovl] = sum;
            }
        }
    }
    ca1d_network_print (ca2d, siz, net);

    // calculate preimage number
    *edg_n = 0;
    for (int unsigned ovl=0; ovl<ca2d.ovl.y.n; ovl++) {
        *edg_n += net [siz-1] [ovl];
    }
    // allocate memory for preimage list
    int unsigned lst [*edg_n] [siz];

    // initialize list of 1D network preimages
    int unsigned p = 0;
    for (int unsigned ovl=0; ovl<ca2d.ovl.y.n; ovl++) {
        for (int unsigned i=0; i<net[siz-1][ovl]; i++) {
            lst [p] [siz-1] = ovl;
//            printf ("DEBUG: p=%u, x=%lu, ovl=%x\n", p, siz-1, lst [p] [siz-1]);
            p++;
        }
    }
    printf ("DEBUG: edg_n = %u\n", *edg_n);
    // list 1D network preimages
    int unsigned t = 0;
    for (int x=siz-2; x>=0; x--) {
        int unsigned p = 0;
        while (p < *edg_n) {
            int unsigned ovl = lst [p] [x+1];
//            printf ("DEBUG: p=%u x=%u, ovl=%x\n", p, x+1, ovl);
            for (int unsigned shf=0; shf<ca2d.shf.y.n; shf++) {
                int unsigned o = v2o_y[0][ovl][shf];
//                printf ("DEBUG: p=%u x=%u, o=%x net[x][o]=%u\n", p, x, o, net[x][o]);
                for (int unsigned i=0; i<net[x][o]; i++) {
                    lst [p] [x] = o;
                    p++;
                }
            }
            t++;
            if (t>10) {
                printf ("DEBUG: t=10\n");
                exit(1);
            }
        }
    }
    // print preimages
    printf ("DEBUG: edgn = %u\n", *edg_n);
    for (int unsigned i=0; i<*edg_n; i++) {
        printf ("DEBUG: edg[%u] =", i);
        for (int x=0; x<siz; x++) {
            printf (" %x", lst [i] [x]);
        }
        printf ("\n");
    }
    
    // put preimages into edge list
    for (int unsigned i=0; i<*edg_n; i++) {
        int unsigned edg;
        ca2d_network_o2ex (ca2d, siz, &edg, lst [i]);
        mpz_add (edg_o [edg], edg_o [edg], edg_w);
    }

    printf ("\n");
    return (0);
}

int ca2d_network (ca2d_t ca2d, ca2d_size_t siz, int unsigned ca [siz.y] [siz.x], mpz_t cnt) {
    // edge size
    const int unsigned edg_x  = pow (ca2d.sts,  (ca2d.ngb.y-1)       *((ca2d.ngb.x-1)+siz.x));
    const int unsigned edg_y  = pow (ca2d.sts, ((ca2d.ngb.y-1)+siz.y)* (ca2d.ngb.x-1)       );

    printf ("DEBUG: begin of network\n");
    // memory allocation for preimage network
    mpz_t net [siz.y+1] [edg_x];

    // compact list of edges
    int unsigned edg_n;

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

    int unsigned d = 0;
    // compute network weights
    for (int y=0; y<siz.y; y++) {
//    for (int y=0; y<1; y++) {
        // loop over all edges
        for (int unsigned e=0; e<edg_x; e++) {
            // only process edge if it's weight is not zero
            if (mpz_sgn (net [y] [e]) > 0) {
                ca1d_network (ca2d, siz.x, ca [y], d, e, net [y] [e], &edg_n, net [y+1]);
            }
        }
    }

    // count all preimages
    mpz_set_ui (cnt, 0);
    for (int unsigned i=0; i<edg_x; i++) {
        mpz_add (cnt, cnt, net [siz.y] [i]);
    }
    gmp_printf ("cnt = %Zi\n", cnt);

    printf ("DEBUG: end of network\n");
    return (0);
}

