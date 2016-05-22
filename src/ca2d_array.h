#ifndef CA2D_ARRAY_H
#define CA2D_ARRAY_H

////////////////////////////////////////////////////////////////////////////////
// array <--> number  conversions
////////////////////////////////////////////////////////////////////////////////

int unsigned ca2d_array_print   (size2D_t s, int unsigned array[s.y][s.x]);
int unsigned ca2d_array_to_ui   (int unsigned base, size2D_t s, int unsigned array[s.y][s.x], int unsigned *number);
int unsigned ca2d_array_from_ui (int unsigned base, size2D_t s, int unsigned array[s.y][s.x], int unsigned  number);
int unsigned ca2d_array_slice   (size2D_t is, size2D_t os0, size2D_t os1, int unsigned ia[is.y][is.x], int unsigned oa[os1.y-os0.y][os1.x-os0.x]);
int unsigned ca2d_array_combine_x (size2D_t is0, size2D_t is1, int unsigned ia0[is0.y][is0.x], int unsigned ia1[is1.y][is1.x], int unsigned oa[is0.y][is1.x+is0.x]);
int unsigned ca2d_array_combine_y (size2D_t is0, size2D_t is1, int unsigned ia0[is0.y][is0.x], int unsigned ia1[is1.y][is1.x], int unsigned oa[is1.y+is0.y][is0.x]);

#endif
