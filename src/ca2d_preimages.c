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
#include "ca2d_network.h"

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

    // create network
    ca2d_network (ca2d, siz, ca);


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
        ca2d_forward (ca2d, tsi, tai, tao);
        mpz_init (li[i]);
        ca2d_array_to_mpz (ca2d.sts, tso, tao, li[i]);
        lo [mpz_get_ui (li[i])] ++;
    }
    for (int unsigned i=0; i<nmo; i++) {
        gmp_printf ("%3u --> %3u%s", i, lo[i], (i+1)%8 ? " " : "\n");
    }

    return (0);
}

