#ifndef CA2D_FORWARD_H
#define CA2D_FORWARD_H

////////////////////////////////////////////////////////////////////////////////
// array <--> number  conversions
////////////////////////////////////////////////////////////////////////////////

int unsigned ca2d_forward (ca2d_t ca2d, ca2d_size_t siz,
    int unsigned ai[siz.y][siz.x],
    int unsigned ao[siz.y-(ca2d.ngb.y-1)][siz.x-(ca2d.ngb.x-1)]
);

#endif
