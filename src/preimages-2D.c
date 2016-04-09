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

//#include <gmp.h>

#define uintca_t uint64_t

int main (int argc, char **argv) {
    // configuration
    int unsigned sts;
    int unsigned ngb_x;
    int unsigned ngb_y;
    int unsigned rule;
    int unsigned siz_x;
    int unsigned siz_y;
    char *filename;
    FILE  file;

    if(argc < 8) {
       fprintf(stderr, "Usage:\t%s STATES NEIGHBORHOOD_SIZE_X NEIGHBORHOOD_SIZE_Y RULE CA_SIZE_X CA_SIZE_Y ca_state_filename.cas\n", argv[0]);
       return (1);
    }
    sts   = strtoul(argv[1], 0, 0);
    ngb_x = strtoul(argv[2], 0, 0);
    ngb_y = strtoul(argv[3], 0, 0);
    rule  = strtoul(argv[4], 0, 0);
    siz_x = strtoul(argv[5], 0, 0);
    siz_y = strtoul(argv[6], 0, 0);
    filename = argv[7];

    // overlap size
    int unsigned ovl_x = ngb_x - 1;
    int unsigned ovl_y = ngb_y - 1;
    int unsigned ovl_n = pow (sts, ovl_y * ovl_x);

    // memory allocation for rule de Bruijn matrices
    

    // neighborhood table

    // overlap transition matrix
    uintca_t tab_o [2] [2] [ovl_n] [ovl_n];

    // memory allocation for preimage network
    uintca_t net [siz_y] [siz_x] [ovl_n];

    return (0);
}

