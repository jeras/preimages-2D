////////////////////////////////////////////////////////////////////////////////
// rule handling
////////////////////////////////////////////////////////////////////////////////

// user interface libraries
#include <stdio.h>
//#include <stdlib.h>

// math libraries
//#include <stdint.h>
#include <math.h>
#include <gmp.h>

// CA libraries
#include "ca2d.h"
#include "ca2d_array.h"

// function for transforming the Game of Life rule description into a number
int ca2d_rule_gol () {
    int unsigned len = 1<<9;
    mpz_t rule;
    mpz_init_set_ui (rule, 0);
    // loop through the rule table
    for (int unsigned i=0; i<len; i++) {
        // calculate neighborhood sum
        int unsigned sum = 0;
        for (int unsigned b=0; b<9; b++) {
            sum += (i >> b) & 0x1;
        }
        // calculate GoL transition function
        int unsigned out;
        if      (sum == 3)  out = 1;
        else if (sum == 2)  out = (i >> 4) % 0x1;
        else                out = 0;
        // add rule table element to rule number
        mpz_mul_ui (rule, rule, 2);
        mpz_add_ui (rule, rule, out);
    }
    // return rule
    gmp_printf ("RULE GoL             = %Zi\n", rule);
    return (0);
}

int ca2d_rule_print (int unsigned sts, size2D_t ngb, int unsigned tab[(size_t) pow (sts, ngb.y*ngb.x)]) {
    int unsigned a [ngb.y] [ngb.x];
    for (int unsigned n=0; n<pow(sts, ngb.y*ngb.x); n++) {
        // convert table index into neighborhood status 2D array
        ca2d_array_from_ui (sts, ngb, a, n);
        // populate pointer table
        printf ("tab [%X] = [", n);
        for (int unsigned y=0; y<ngb.y; y++) {
            printf ("%s[", y ? "," : "");
            for (int unsigned x=0; x<ngb.x; x++) {
                printf ("%s%u", x ? "," : "", a[y][x]);
            }
            printf ("]");
        }
        printf ("] = %u\n", tab[n]);
    }
    return (0);
}


