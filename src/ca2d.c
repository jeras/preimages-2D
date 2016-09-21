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
    ca2d_t ca2d;
    ca2d_size_t siz;
    char *dir;
    char *filename;
    FILE  file;

    // check number of input arguments
    if (argc < 8) {
        fprintf (stderr, "Usage:\t%s STATES NEIGHBORHOOD_SIZE_Y NEIGHBORHOOD_SIZE_X RULE CA_SIZE_Y CA_SIZE_X ca_state_filename.cas DIRECTION(f/b) STEPS\n", argv[0]);
        return (1);
    }
    // read input arguments
    ca2d.sts   = strtoul (argv[1], 0, 0);
    ca2d.ngb.y = strtoul (argv[2], 0, 0);
    ca2d.ngb.x = strtoul (argv[3], 0, 0);
    mpz_init_set_str (ca2d.rule, argv[4], 0);
    siz.y = strtoul (argv[5], 0, 0);
    siz.x = strtoul (argv[6], 0, 0);
    filename = argv[7];
    dir = argv[8];

    // update ca2d structure
    ca2d_update (&ca2d);
    printf ("\n");

    // read CA configuration file
    int unsigned cai [siz.y] [siz.x];
    ca2d_read (filename, siz, cai);
    ca2d_print (siz, cai);
    printf ("\n");

    if (dir[0] == 'f') {
        ca2d_size_t siz_fwd = {siz.y-ca2d.ver.y, siz.x-ca2d.ver.x};
        int unsigned cao [siz_fwd.y] [siz_fwd.x];

        printf ("FORWARD:\n");
        ca2d_forward (ca2d, siz, cai, cao);
        ca2d_print (siz_fwd, cao);
    } else {
    }

    return (0);
}

