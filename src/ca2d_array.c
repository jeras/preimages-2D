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

////////////////////////////////////////////////////////////////////////////////
// array <--> number  conversions
////////////////////////////////////////////////////////////////////////////////

// user interface libraries
#include <stdio.h>

// math libraries
#include <gmp.h>

#include "ca2d.h"

int unsigned ca2d_array_print (ca2d_size_t s, int unsigned array[s.y][s.x]) {
    printf ("[");
    for (int unsigned y=0; y<s.y; y++) {
        printf ("%s[", y ? "," : "");
        for (int unsigned x=0; x<s.x; x++) {
            printf ("%s%u", x ? "," : "", array [y] [x]);
        }
        printf ("]");
    }
    printf ("]");
    return 0;
}

int unsigned ca2d_array_to_ui (int unsigned base, ca2d_size_t s, int unsigned array[s.y][s.x], int unsigned *number) {
    *number = 0;
    int unsigned mul = 1;
    for (int unsigned y=0; y<s.y; y++) {
        for (int unsigned x=0; x<s.x; x++) {
            *number += array [y] [x] * mul;
            mul *= base;
        }
    }
    return 0;
}

int unsigned ca2d_array_from_ui (int unsigned base, ca2d_size_t s, int unsigned array[s.y][s.x], int unsigned number) {
    for (int unsigned y=0; y<s.y; y++) {
        for (int unsigned x=0; x<s.x; x++) {
            array [y] [x] = number % base;
            number /= base;
        }
    }
    return 0;
}

int unsigned ca2d_array_to_mpz (int unsigned base, ca2d_size_t s, int unsigned array[s.y][s.x], mpz_t number) {
    mpz_t mul;
    mpz_init (mul);
    mpz_set_ui (mul, 1);
    mpz_set_ui (number, 0);
    for (int unsigned y=0; y<s.y; y++) {
        for (int unsigned x=0; x<s.x; x++) {
            mpz_addmul_ui (number, mul, array [y] [x]);
            mpz_mul_ui (mul, mul, base);
        }
    }
    return 0;
}

int unsigned ca2d_array_from_mpz (int unsigned base, ca2d_size_t s, int unsigned array[s.y][s.x], mpz_t number) {
    mpz_t num;
    mpz_init_set (num, number);
    for (int unsigned y=0; y<s.y; y++) {
        for (int unsigned x=0; x<s.x; x++) {
            array [y] [x] = mpz_tdiv_q_ui (num, num, base);
        }
    }
    return 0;
}

int unsigned ca2d_array_slice (
    ca2d_size_t is,
    ca2d_size_t ob, ca2d_size_t os,
    int unsigned ia[is.y][is.x], int unsigned oa[os.y][os.x])
{
    for (int unsigned y=0; y<os.y; y++) {
        for (int unsigned x=0; x<os.x; x++) {
            oa[y][x] = ia[ob.y+y][ob.x+x];
        }
    }
    return 0;
}

int unsigned ca2d_array_combine_x (
    ca2d_size_t is0, ca2d_size_t is1,
    int unsigned ia0[is0.y][is0.x], int unsigned ia1[is1.y][is1.x], int unsigned oa[is0.y][is1.x+is0.x])
{
    for (int unsigned y=0; y<is0.y; y++) {
        for (int unsigned x=0; x<is0.x; x++) {
            oa[y][x      ] = ia0[y][x];
        }
        for (int unsigned x=0; x<is1.x; x++) {
            oa[y][x+is0.x] = ia1[y][x];
        }
    }
    return 0;
}

int unsigned ca2d_array_combine_y (
    ca2d_size_t is0, ca2d_size_t is1,
    int unsigned ia0[is0.y][is0.x], int unsigned ia1[is1.y][is1.x], int unsigned oa[is1.y+is0.y][is0.x])
{
    for (int unsigned x=0; x<is0.x; x++) {
        for (int unsigned y=0; y<is0.y; y++) {
            oa[y      ][x] = ia0[y][x];
        }
        for (int unsigned y=0; y<is0.y; y++) {
            oa[y+is0.y][x] = ia1[y][x];
        }
    }
    return 0;
}


