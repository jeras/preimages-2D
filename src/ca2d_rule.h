#ifndef CA2D_RULE_H
#define CA2D_RULE_H

// math libraries
#include <gmp.h>

////////////////////////////////////////////////////////////////////////////////
// rule handling
////////////////////////////////////////////////////////////////////////////////

// function for transforming the Game of Life rule description into a number
int ca2d_rule_gol ();

int ca2d_rule_print (ca2d_t ca2d, int unsigned tab[(size_t) pow (ca2d.sts, ca2d.ngb.y*ca2d.ngb.x)]);
int ca2d_rule_table (ca2d_t ca2d, int unsigned tab[(size_t) pow (ca2d.sts, ca2d.ngb.y*ca2d.ngb.x)]);

#endif