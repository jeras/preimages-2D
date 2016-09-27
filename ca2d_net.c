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
#include "ca2d_cfg.h"

//    // print edge counters
//    printf ("NETWORK: edge weights from forward/backward direction,\n");
//    printf ("         summed preimages\n");
//    for (int d=0; d<2; d++) {
//        for (int y=0; y<=siz.y; y++) {
//            printf ("net [dy=%u][y=%u] = [", d, y);
//            for (int unsigned edg=0; edg<edg_x; edg++) {
//                gmp_printf ("%Zi ", net[d][y][edg]);
//            }
//            printf ("]");
//            if ((y-(1-d)*(siz.y))==0) {
//                gmp_printf ("  cnt [%u] = %Zi", d, cnt [d]);
//            }
//            printf ("\n");
//        }
//    }
//    printf ("\n");

// tables for geting overlap values from neighborhood values
static int ca2d_net_table_v2o (ca2d_t ca2d, int unsigned v2o_y [2] [ca2d.ovl.y.n] [ca2d.shf.y.n],
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
static int ca2d_net_table_n2o (ca2d_t ca2d, int unsigned n2o_y [2] [ca2d.ngb.n],
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

// tables for geting neighborhood values from overlap values and rest
int ca2d_net_table_o2n (ca2d_t ca2d, int unsigned o2n_y [2] [ca2d.ovl.y.n] [ca2d.rem.x.n],
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
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, 0           }, ca2d.ovl.x, ngb_a, ovl_ax);
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, ca2d.ovl.x.x}, ca2d.rem.x, ngb_a, rem_ax);
            } else {
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, 0           }, ca2d.rem.x, ngb_a, rem_ax);
                ca2d_array_slice (ca2d.ngb, (ca2d_size_t) {0, ca2d.rem.x.x}, ca2d.ovl.x, ngb_a, ovl_ax);
            }
            ca2d_array_to_ui (ca2d.sts, ca2d.ovl.x, ovl_ax, &ovl_x);
            ca2d_array_to_ui (ca2d.sts, ca2d.rem.x, rem_ax, &rem_x);
            o2n_x [d] [ovl_x] [rem_x] = ngb;
        }
    }
    return (0);
}

// function for geting array of overlap integers from edge integer in X dimension
int ca2d_net_ex2o (ca2d_t ca2d, size_t siz, int unsigned e, int unsigned o [siz]) {
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
int ca2d_net_o2ex (ca2d_t ca2d, size_t siz, int unsigned *e, int unsigned o [siz]) {
    ca2d_size_t se = {ca2d.ngb.y-1, ca2d.ngb.x-1 + siz};
    ca2d_size_t so = {ca2d.ngb.y-1, ca2d.ngb.x  };
    ca2d_size_t sr = {ca2d.ngb.y-1,            1};
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

static int ca1d_net (
    // CA properties
    ca2d_t ca2d,
    size_t siz,
    // configuration
    int unsigned ca [siz],
    // direction in Y dimmension
    int unsigned dy,
    int unsigned edg_i,
    mpz_t edg_w,
    int unsigned *edg_n,
    mpz_t edg_o [(size_t) pow (ca2d.sts, (ca2d.ngb.y-1)*((ca2d.ngb.x-1)+siz))]
) {
    // tables
    int unsigned n2o_y [2] [ca2d.ngb.n];
    int unsigned n2o_x [2] [ca2d.ngb.n];
    int unsigned o2n_y [2] [ca2d.ovl.y.n] [ca2d.rem.y.n];
    int unsigned o2n_x [2] [ca2d.ovl.x.n] [ca2d.rem.x.n];
    ca2d_net_table_n2o (ca2d, n2o_y, n2o_x);
    ca2d_net_table_o2n (ca2d, o2n_y, o2n_x);

    // memory allocation for preimage network
    int unsigned net [siz] [ca2d.ovl.y.n];

    // convert input edge into array of overlaps
    int unsigned ovl_i [siz];
    ca2d_net_ex2o (ca2d, siz, edg_i, ovl_i);

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
            if (ca[x] == ca2d.tab[ngb]) {
                // get end edge overlap from pointer table
                int unsigned ovl = n2o_y [1-dy] [ngb];
                // set weight to overlap
                net [x] [ovl] = 1;
            }
        }
    }

    // count 1D network preimages
    int unsigned v2o_y [2] [ca2d.ovl.y.n] [ca2d.shf.y.n];
    int unsigned v2o_x [2] [ca2d.ovl.x.n] [ca2d.shf.x.n];

    ca2d_net_table_v2o (ca2d, v2o_y, v2o_x);

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
            p++;
        }
    }
    // list 1D network preimages
    for (int x=siz-2; x>=0; x--) {
        int unsigned p = 0;
        while (p < *edg_n) {
            int unsigned ovl = lst [p] [x+1];
            for (int unsigned shf=0; shf<ca2d.shf.y.n; shf++) {
                int unsigned o = v2o_y[0][ovl][shf];
                for (int unsigned i=0; i<net[x][o]; i++) {
                    lst [p] [x] = o;
                    p++;
                }
            }
        }
    }
    
    // put preimages into edge list
    for (int unsigned i=0; i<*edg_n; i++) {
        int unsigned edg;
        ca2d_net_o2ex (ca2d, siz, &edg, lst [i]);
        mpz_add (edg_o [edg], edg_o [edg], edg_w);
    }

    return (0);
}

int ca2d_net (ca2d_t ca2d, ca2d_size_t siz, int unsigned ca [siz.y] [siz.x], mpz_t cnt [2], int unsigned (** p_list) [] [siz.y+ca2d.ver.y] [siz.x+ca2d.ver.x]) {
    // edge size
    const int unsigned edg_x  = pow (ca2d.sts,  (ca2d.ngb.y-1)       *((ca2d.ngb.x-1)+siz.x));
    const int unsigned edg_y  = pow (ca2d.sts, ((ca2d.ngb.y-1)+siz.y)* (ca2d.ngb.x-1)       );

    // compact list of edges
    int unsigned edg_n;

    // memory allocation for preimage network
    mpz_t net [2] [siz.y+1] [edg_x];

    // initialize array variable
    for (int d=0; d<2; d++) {
        for (int y=0; y<=siz.y; y++) {
            for (int unsigned edg=0; edg<edg_x; edg++) {
                mpz_init (net [d] [y] [edg]);
            }
        }
    }
    // initialize starting edge to unit (open edge)
    for (int unsigned edg=0; edg<edg_x; edg++) {
        mpz_set_ui (net [0] [0    ] [edg], 1);
        mpz_set_ui (net [1] [siz.y] [edg], 1);
    }
    // compute network weights in both directions
    for (int y=0; y<siz.y; y++) {
        // loop over all edges
        for (int unsigned edg=0; edg<edg_x; edg++) {
            // only process edge if it's weight is not zero
            if (mpz_sgn (net [0] [      y] [edg]) > 0) {
                ca1d_net (ca2d, siz.x, ca [        y], 0, edg, net [0] [      y] [edg], &edg_n, net [0] [      (y+1)]);
            }
            if (mpz_sgn (net [1] [siz.y-y] [edg]) > 0) {
                ca1d_net (ca2d, siz.x, ca [siz.y-1-y], 1, edg, net [1] [siz.y-y] [edg], &edg_n, net [1] [siz.y-(y+1)]);
            }
        }
    }
    // count all preimages
    for (int unsigned d=0; d<2; d++) {
        mpz_init   (cnt [d]);
        for (int unsigned edg=0; edg<edg_x; edg++) {
            mpz_add (cnt [d], cnt [d], net [d] [d ? 0 : siz.y] [edg]);
        }
    }

    // allocate memory for preimages described by edges
    mpz_t weight;
    mpz_init (weight);
    int unsigned lst [mpz_get_ui(cnt[0])] [siz.y+1];

    // initialize list of 2D network preimages
    int unsigned p = 0;
    for (int unsigned edg=0; edg<edg_x; edg++) {
        int unsigned max;
        max = mpz_get_ui(net[1][0][edg]);
        for (int unsigned i=0; i<max; i++) {
            lst [p] [0] = edg;
            p++;
        }
    }
    // list 2D network preimages
    // memory allocation for preimage network
    mpz_t edges [edg_x];
    // initialize array variable
    for (int unsigned edg=0; edg<edg_x; edg++) {
        mpz_init (edges [edg]);
    }
    for (int y=1; y<=siz.y; y++) {
        int unsigned p = 0;
        while (p < mpz_get_ui(cnt[0])) {
            // gain process 1D preimage for current edge (lst [p] [y])
            mpz_set_ui (weight, 1);
            // re initialize edges
            for (int unsigned edg=0; edg<edg_x; edg++) {
                mpz_set_ui (edges [edg], 0);
            }
            ca1d_net (ca2d, siz.x, ca [y-1], 0, lst [p] [y-1], weight, &edg_n, edges);
            for (int unsigned edg=0; edg<edg_x; edg++) {
                mpz_set (weight, edges[edg]);
                if ((mpz_sgn (weight) > 0) && (mpz_sgn (net[1][y][edg]) > 0)) {
                    for (int unsigned i=0; i<mpz_get_ui(net[1][y][edg]); i++) {
                       lst [p] [y] = edg;
                       p++;
                    }
                }
            }
        }
    }

    // allocate memory for preimages
    ca2d_size_t siz_pre = {siz.y+ca2d.ver.y, siz.x+ca2d.ver.x};
    siz_pre.a = siz_pre.y * siz_pre.x;
    *p_list = (int unsigned (*) [] [siz_pre.y] [siz_pre.x]) malloc (sizeof(int unsigned) * siz_pre.a * mpz_get_ui(cnt[0]));
    // convert edge list into actual preimage list
    for (int unsigned i=0; i<mpz_get_ui(cnt[0]); i++) {
        ca2d_size_t siz_lin0 = (ca2d_size_t) {ca2d.ver.y, siz.x+ca2d.ver.x};
        int unsigned line0 [siz_lin0.y] [siz_lin0.x];
        ca2d_array_from_ui (ca2d.sts, siz_lin0, line0, lst[i][0]);
        ca2d_array_fit (siz_pre, (ca2d_size_t) {0, 0}, siz_lin0, (**p_list)[i], line0);
        for (int unsigned y=0; y<siz.y; y++) {
            ca2d_size_t siz_lin = (ca2d_size_t) {1, siz.x+ca2d.ver.x};
            int unsigned line [siz_lin.y] [siz_lin.x];
            ca2d_array_from_ui (ca2d.sts, siz_lin0, line0, lst[i][y+1]);
            ca2d_array_slice   (siz_lin0, (ca2d_size_t) {ca2d.ver.y-1, 0}, siz_lin, line0, line);
            ca2d_array_fit (siz_pre, (ca2d_size_t) {y+ca2d.ver.y, 0}, siz_lin, (**p_list)[i], line);
        }
    }

    printf ("DEBUG: end of network\n");
    return (0);
}

