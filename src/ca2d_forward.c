////////////////////////////////////////////////////////////////////////////////
// array <--> number  conversions
////////////////////////////////////////////////////////////////////////////////

// math libraries
#include <math.h>

#include "ca2d.h"
#include "ca2d_rule.h"
#include "ca2d_array.h"

int unsigned ca2d_forward (ca2d_t ca2d, ca2d_size_t siz,
    int unsigned ai[siz.y][siz.x],
    int unsigned ao[siz.y-(ca2d.ngb.y-1)][siz.x-(ca2d.ngb.x-1)]
) {
    int unsigned at [ca2d.ngb.y] [ca2d.ngb.x];
    int unsigned nt;
    uintca_t ngb_n = pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x);
    int unsigned tab[ngb_n];
    ca2d_rule_table (ca2d, tab);
    for (int unsigned y=0; y<siz.y-(ca2d.ngb.y-1); y++) {
        for (int unsigned x=0; x<siz.x-(ca2d.ngb.x-1); x++) {
            ca2d_array_slice (siz, (ca2d_size_t) {y, x}, ca2d.ngb, ai, at);
            ca2d_array_to_ui (ca2d.sts, ca2d.ngb, at, &nt);
            ao [y] [x] = tab [nt];
        }
    }
    return 0;
}

