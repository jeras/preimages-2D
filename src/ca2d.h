#ifndef CA2D_H
#define CA2D_H

// math libraries
//#include <math.h>
#include <gmp.h>

#define uintca_t long long int unsigned

typedef struct {
    int unsigned y;
    int unsigned x;
} ca2d_size_t;

typedef struct {
    int unsigned sts;
    ca2d_size_t  ngb;
    mpz_t        rule;
} ca2d_t;

#endif
