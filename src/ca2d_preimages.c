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

    // update ca2d structure
    ca2d_update (&ca2d);

    // read CA configuration file
    int unsigned cai [siz.y] [siz.x];
    int unsigned cao [siz.y] [siz.x];
    ca2d_read (filename, siz, cai);
    ca2d_print (siz, cai);
    printf ("\n");

    // calculate network
    int unsigned res [siz.y] [siz.x] [ca2d.ngb.n];  // result
    ca2d_network       (ca2d, siz, cai, res);
    ca2d_network_print (ca2d, siz,      res);
    // check if there is a preimage
    int unsigned cnt = 0;
    for (int unsigned n=0; n<ca2d.ngb.n; n++) {
         cnt += res [0] [0] [n];
    }
    if (!cnt) {
         printf ("There are no preimages\n");
         return (0);
    } else {
         printf ("CNT = %u\n", cnt);
    }

    // calculate preimage from network
    ca2d_size_t sizp = {siz.y+(ca2d.ngb.y-1), siz.x+(ca2d.ngb.x-1)};
    int unsigned preimage [sizp.y] [sizp.x];
//    ca2d_network_preimage (ca2d, siz, res, preimage);
//
//    printf ("PREIMAGE:\n");
//    ca2d_print (sizp, preimage);
//    printf ("\n");
//
//    printf ("FORWARD:\n");
//    ca2d_forward (ca2d, sizp, preimage, cao);
//    ca2d_print (siz, cao);
//
//    printf ("COMPARE: ");
//    int status;
//    status = ca2d_lattice_compare (siz, cai, cao);
//    if (status) {
//        printf ("FAILURE\n");
//    } else {
//        printf ("SUCCESS\n");
//    }
//
//
//    // get al preimages using bute force
//    printf ("BRUTE FORCE:\n");
//    for (int unsigned p=0; p<pow(ca2d.sts,sizp.y*sizp.x); p++) {
//        ca2d_array_from_ui (ca2d.sts, sizp, preimage, p);
//        ca2d_forward (ca2d, sizp, preimage, cao);
//        status = ca2d_lattice_compare (siz, cai, cao);
//        if (status) {
//        } else {
//            ca2d_print (sizp, preimage);
//            printf ("\n");
//        }
//    }

    return (0);
}

