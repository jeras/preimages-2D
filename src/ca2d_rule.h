#ifndef CA2D_RULE_H
#define CA2D_RULE_H

// math libraries
#include <gmp.h>

////////////////////////////////////////////////////////////////////////////////
// rule handling
////////////////////////////////////////////////////////////////////////////////

// function for transforming the Game of Life rule description into a number
int ca2d_rule_gol ();

int ca2d_rule_print (int unsigned sts, ca2d_size_t ngb, int unsigned tab[(size_t) pow (sts, ngb.y*ngb.x)]);
int ca2d_rule_table (int unsigned sts, ca2d_size_t ngb, mpz_t rule, int unsigned tab[(size_t) pow (sts, ngb.y*ngb.x)]);

#endif
