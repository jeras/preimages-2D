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

#ifndef CA2D_H
#define CA2D_H

// math libraries
#include <gmp.h>

#define uintca_t long long int unsigned

typedef struct {
    // basic parameters
    int unsigned y;  // Y size
    int unsigned x;  // X size
    // calculated parameters
    int unsigned a;  // area (x*y)
    int unsigned n;  // number of states
} ca2d_size_t;

typedef struct {
    // basic parameters
    int unsigned sts;  // number of cell states
    ca2d_size_t  ngb;  // neighborhood size
    mpz_t        rule; // rule
    // calculated parameters
    struct {
        ca2d_size_t y;
        ca2d_size_t x;
    } ovl;             // overlap size
    struct {
        ca2d_size_t y;
        ca2d_size_t x;
    } rem;             // remainder size (remainder and overlap add to a whole neighborhood)
    ca2d_size_t ver;   // vertice size (corner overlap)
    struct {
        ca2d_size_t y;
        ca2d_size_t x;
    } shf;             // shift size (shift and vertice add to a whole overlap)
} ca2d_t;

int ca2d_update (ca2d_t *ca2d);
int ca2d_bprint (ca2d_t  ca2d);

#endif // CA2D_H
