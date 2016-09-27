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
// rule handling
////////////////////////////////////////////////////////////////////////////////

// user interface libraries
#include <stdio.h>
#include <stdlib.h>

// math libraries
#include <math.h>
#include <gmp.h>

// CA libraries
#include "ca2d.h"
#include "ca2d_array.h"

// function for transforming the Game of Life rule description into a number
int ca2d_rule_gol () {
    int unsigned len = 1<<9;
    int unsigned sum, out;
    mpz_t rule;
    mpz_init (rule);
    // loop through the rule table
    for (int i=len-1; i>=0; i--) {
        // calculate neighborhood sum
        sum = 0;
        for (int unsigned b=0; b<9; b++) {
            sum += ((i & 0b111101111) >> b) & 0x1;
        }
        // calculate GoL transition function
        if      (sum == 3)  out = 1;
        else if (sum == 2)  out = (i & 0b000010000) >> 4;
        else                out = 0;
        // add rule table element to rule number
        mpz_mul_ui (rule, rule, 2);
        mpz_add_ui (rule, rule, out);
    }
    // return rule
    gmp_printf ("RULE GoL = 0x%ZX\n", rule);
    return (0);
}

int ca2d_rule_table (ca2d_t *ca2d) {
    // neighborhood area
    // check if it is within allowed values, for example less then 9==3*3
    if ((ca2d->ngb.x == 0) || (ca2d->ngb.y == 0) || ((ca2d->ngb.y * ca2d->ngb.x) > 9)) {
        printf ("ERROR: neighborhood area %u is outside range [1:9].\n", ca2d->ngb.a);
        return (1);
    }

    mpz_t range;
    mpz_init (range);
    mpz_ui_pow_ui (range, ca2d->sts, ca2d->ngb.n);
    if (mpz_cmp (ca2d->rule, range) > 0) {
        gmp_printf ("ERROR: rule is outside of range = %Zi\n", range);
        return (1);
    }
    mpz_clear (range);

    // allocate rule table memory
    ca2d->tab = malloc (ca2d->ngb.n * sizeof(int unsigned));

    // rule table (conversion to base sts)
    mpz_t rule_q;
    mpz_init_set (rule_q, ca2d->rule);
    for (int unsigned i=0; i<ca2d->ngb.n; i++) {
        // populate transition function
        ca2d->tab[i] = mpz_tdiv_q_ui (rule_q, rule_q, ca2d->sts);
    }
    mpz_clear (rule_q);
 
    return (0);
}

int ca2d_rule_print (ca2d_t ca2d) {
    int unsigned a [ca2d.ngb.y] [ca2d.ngb.x];
    for (int unsigned n=0; n<ca2d.ngb.n; n++) {
        // convert table index into neighborhood status 2D array
        ca2d_array_from_ui (ca2d.sts, ca2d.ngb, a, n);
        // print rule table
        printf ("  tab [%X] = [", n);
        for (int unsigned y=0; y<ca2d.ngb.y; y++) {
            printf ("%s[", y ? "," : "");
            for (int unsigned x=0; x<ca2d.ngb.x; x++) {
                printf ("%s%u", x ? "," : "", a[y][x]);
            }
            printf ("]");
        }
        printf ("] = %u\n", ca2d.tab[n]);
    }
    return (0);
}

