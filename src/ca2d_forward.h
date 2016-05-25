#ifndef CA2D_FORWARD_H
#define CA2D_FORWARD_H

////////////////////////////////////////////////////////////////////////////////
// array <--> number  conversions
////////////////////////////////////////////////////////////////////////////////

int unsigned ca2d_forward (int unsigned base, ca2d_size_t siz, ca2d_size_t ngb, int unsigned ngb_n, int unsigned tab [ngb_n],
    int unsigned ai[siz.y][siz.x],
    int unsigned ao[siz.y-(ngb.y-1)][siz.x-(ngb.x-1)]
);

#endif
