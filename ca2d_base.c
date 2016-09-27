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

// math libraries
#include <stdio.h>
#include <math.h>
#include <gmp.h>

#include "ca2d.h"
#include "ca2d_rule.h"
#include "ca2d_array.h"
#include "ca2d_cfg.h"

int ca2d_update (ca2d_t *ca2d) {
    // overlay sizes
    ca2d->ovl.y   = (ca2d_size_t) {(ca2d->ngb.y-1) , (ca2d->ngb.x  )};
    ca2d->ovl.x   = (ca2d_size_t) {(ca2d->ngb.y  ) , (ca2d->ngb.x-1)};
    // remainder sizes
    ca2d->rem.y   = (ca2d_size_t) {(            1) , (ca2d->ngb.x  )};
    ca2d->rem.x   = (ca2d_size_t) {(ca2d->ngb.y  ) , (            1)};
    // vertice size
    ca2d->ver     = (ca2d_size_t) {(ca2d->ngb.y-1) , (ca2d->ngb.x-1)};
    // shift sizes
    ca2d->shf.y   = (ca2d_size_t) {(ca2d->ngb.y-1) , (            1)};
    ca2d->shf.x   = (ca2d_size_t) {(            1) , (ca2d->ngb.y-1)};
    // areas
    ca2d->ngb.a   = ca2d->ngb.y   * ca2d->ngb.x  ;
    ca2d->ovl.y.a = ca2d->ovl.y.y * ca2d->ovl.y.x;
    ca2d->ovl.x.a = ca2d->ovl.x.y * ca2d->ovl.x.x;
    ca2d->rem.y.a = ca2d->rem.y.y * ca2d->rem.y.x;
    ca2d->rem.x.a = ca2d->rem.x.y * ca2d->rem.x.x;
    ca2d->ver.a   = ca2d->ver.y   * ca2d->ver.x  ;
    ca2d->shf.y.a = ca2d->shf.y.y * ca2d->shf.y.x;
    ca2d->shf.x.a = ca2d->shf.x.y * ca2d->shf.x.x;
    // number of states
    ca2d->ngb.n   = (size_t) pow (ca2d->sts, ca2d->ngb.a  );
    ca2d->ovl.y.n = (size_t) pow (ca2d->sts, ca2d->ovl.y.a);
    ca2d->ovl.x.n = (size_t) pow (ca2d->sts, ca2d->ovl.x.a);
    ca2d->rem.y.n = (size_t) pow (ca2d->sts, ca2d->rem.y.a);
    ca2d->rem.x.n = (size_t) pow (ca2d->sts, ca2d->rem.x.a);
    ca2d->ver.n   = (size_t) pow (ca2d->sts, ca2d->ver.a  );
    ca2d->shf.y.n = (size_t) pow (ca2d->sts, ca2d->shf.y.a);
    ca2d->shf.x.n = (size_t) pow (ca2d->sts, ca2d->shf.x.a);
    // rule table
    ca2d_rule_table (ca2d);

//    ca2d_bprint (*ca2d);
    return (0);
}

int ca2d_bprint (ca2d_t ca2d) {
    printf ("CA parameters:\n");
    printf ("  sts  : %u\n", ca2d.sts);
    printf ("  ngb  : siz={%u,%u}, a=%u, n=%u\n", ca2d.ngb.y  , ca2d.ngb.x  , ca2d.ngb.a  , ca2d.ngb.n  );
    printf ("  ovl-y: siz={%u,%u}, a=%u, n=%u\n", ca2d.ovl.y.y, ca2d.ovl.y.x, ca2d.ovl.y.a, ca2d.ovl.y.n);
    printf ("  ovl-x: siz={%u,%u}, a=%u, n=%u\n", ca2d.ovl.x.y, ca2d.ovl.x.x, ca2d.ovl.x.a, ca2d.ovl.x.n);
    printf ("  rem-y: siz={%u,%u}, a=%u, n=%u\n", ca2d.rem.y.y, ca2d.rem.y.x, ca2d.rem.y.a, ca2d.rem.y.n);
    printf ("  rem-x: siz={%u,%u}, a=%u, n=%u\n", ca2d.rem.x.y, ca2d.rem.x.x, ca2d.rem.x.a, ca2d.rem.x.n);
    printf ("  ver  : siz={%u,%u}, a=%u, n=%u\n", ca2d.ver.y  , ca2d.ver.x  , ca2d.ver.a  , ca2d.ver.n  );
    printf ("  shf-y: siz={%u,%u}, a=%u, n=%u\n", ca2d.shf.y.y, ca2d.shf.y.x, ca2d.shf.y.a, ca2d.shf.y.n);
    printf ("  shf-x: siz={%u,%u}, a=%u, n=%u\n", ca2d.shf.x.y, ca2d.shf.x.x, ca2d.shf.x.a, ca2d.shf.x.n);
    printf ("CA rule table:\n");
    ca2d_rule_print (ca2d);
    return (0);
}

