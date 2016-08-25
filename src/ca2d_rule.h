#ifndef CA2D_RULE_H
#define CA2D_RULE_H

// math libraries
#include <gmp.h>

////////////////////////////////////////////////////////////////////////////////
// rule handling
////////////////////////////////////////////////////////////////////////////////

// function for transforming the Game of Life rule description into a number
int ca2d_rule_gol ();

int ca2d_rule_table (ca2d_t ca2d, int unsigned tab[ca2d.ngb.n]);
int ca2d_rule_print (ca2d_t ca2d);

#endif
