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

int main (int argc, char **argv) {
    // configuration
    int unsigned sts;
    int unsigned ngb_x;
    int unsigned ngb_y;
    mpz_t        rule;
    int unsigned siz_x;
    int unsigned siz_y;
    char *filename;
    FILE  file;

    // read input arguments
    if (argc < 8) {
       fprintf (stderr, "Usage:\t%s STATES NEIGHBORHOOD_SIZE_X NEIGHBORHOOD_SIZE_Y RULE CA_SIZE_X CA_SIZE_Y ca_state_filename.cas\n", argv[0]);
       return (1);
    }
    sts   = strtoul (argv[1], 0, 0);
    ngb_x = strtoul (argv[2], 0, 0);
    ngb_y = strtoul (argv[3], 0, 0);
    mpz_init_set_str (rule, argv[4], 0);
    siz_x = strtoul (argv[5], 0, 0);
    siz_y = strtoul (argv[6], 0, 0);
    filename = argv[7];

    // printout call arguments for debugging purposes
    printf     ("STATES              = %i\n", sts  );
    printf     ("NEIGHBORHOOD_SIZE_X = %i\n", ngb_x);
    printf     ("NEIGHBORHOOD_SIZE_Y = %i\n", ngb_y);
    gmp_printf ("RULE                = %Zi\n", rule);
    printf     ("CA_SIZE_X           = %i\n", siz_x);
    printf     ("CA_SIZE_Y           = %i\n", siz_y);
    printf     ("filename            = %s\n", filename);

    // neighborhood area
    int unsigned ngb_a = ngb_x * ngb_y;
    printf     ("ngb_a               = %i\n", ngb_a);
    // check if it is within allowed values, for example (ngb_a < 9==3*3)
    if ((ngb_a == 0) || (ngb_a >= 9)) {
        fprintf (stderr, "ERROR: neighborhood area (ngb_a = %u) is outside range [1:9].\n", ngb_a);
        return (1);
    }

    // neighborhood states (sts ** ngb_n)
    uintca_t ngb_n = pow (sts, ngb_a);
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

    // rule table (conversion to base sts)
    uintca_t tab_r [ngb_n];
    mpz_t rule_q, rule_r;
    mpz_init_set (rule_q, rule);
    mpz_init (rule_r);
    for (int unsigned i=0; i<ngb_n; i++) {
        mpz_t tmp;
        mpz_init (tmp);
        tab_r [i] = mpz_tdiv_q_ui (tmp, rule_q, sts);
        mpz_init_set (rule_q, tmp);
        printf     ("tab_r [%4i]        = %lld\n", i, tab_r [i]);
    }
    
    // overlap size
    int unsigned ovl_x = ngb_x - 1;
    int unsigned ovl_y = ngb_y - 1;
    uintca_t ovl_n = pow (sts, ovl_y * ovl_x);


    // neighborhood table

    // overlap transition matrix
    // [0-x, 1-y] 
    uintca_t tab_o [2] [2] [ovl_n];

    // memory allocation for preimage network
    uintca_t net [siz_y] [siz_x] [ovl_n];

    for (int unsigned y=0; y<=siz_y; y++) {
        for (int unsigned x=0; x<=siz_x; x++) {
        }
    }

    return (0);
}


