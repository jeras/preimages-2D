////////////////////////////////////////////////////////////////////////////////
// array <--> number  conversions
////////////////////////////////////////////////////////////////////////////////

#include "ca2d.h"
#include "ca2d_array.h"

int unsigned ca2d_forward (int unsigned base, size2D_t siz, size2D_t ngb, int unsigned ngb_n, int unsigned tab [ngb_n],
    int unsigned ai[siz.y][siz.x],
    int unsigned ao[siz.y-(ngb.y-1)][siz.x-(ngb.x-1)]
) {
    int unsigned at [ngb.y] [ngb.x];
    int unsigned nt;
    for (int unsigned y=0; y<siz.y-(ngb.y-1); y++) {
        for (int unsigned x=0; x<siz.x-(ngb.x-1); x++) {
            ca2d_array_slice (siz, (size2D_t) {y, x}, (size2D_t) {y+(ngb.y-1), x+(ngb.x-1)}, ai, at);
            ca2d_array_to_ui (base, ngb, at, &nt);
            ao [y] [x] = tab [nt];
        }
    }
    return 0;
}

