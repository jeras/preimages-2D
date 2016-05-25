#ifndef CA2D_ARRAY_H
#define CA2D_ARRAY_H

// math libraries
#include <gmp.h>

////////////////////////////////////////////////////////////////////////////////
// array <--> number  conversions
////////////////////////////////////////////////////////////////////////////////

int unsigned ca2d_array_print     (ca2d_size_t s, int unsigned array[s.y][s.x]);
int unsigned ca2d_array_to_ui     (int unsigned base, ca2d_size_t s, int unsigned array[s.y][s.x], int unsigned *number);
int unsigned ca2d_array_from_ui   (int unsigned base, ca2d_size_t s, int unsigned array[s.y][s.x], int unsigned  number);
int unsigned ca2d_array_to_mpz    (int unsigned base, ca2d_size_t s, int unsigned array[s.y][s.x], mpz_t number);
int unsigned ca2d_array_from_mpz  (int unsigned base, ca2d_size_t s, int unsigned array[s.y][s.x], mpz_t number);
int unsigned ca2d_array_slice     (ca2d_size_t is, ca2d_size_t ob, ca2d_size_t os, int unsigned ia[is.y][is.x], int unsigned oa[os.y][os.x]);
int unsigned ca2d_array_combine_x (ca2d_size_t is0, ca2d_size_t is1, int unsigned ia0[is0.y][is0.x], int unsigned ia1[is1.y][is1.x], int unsigned oa[is0.y][is1.x+is0.x]);
int unsigned ca2d_array_combine_y (ca2d_size_t is0, ca2d_size_t is1, int unsigned ia0[is0.y][is0.x], int unsigned ia1[is1.y][is1.x], int unsigned oa[is1.y+is0.y][is0.x]);

#endif
