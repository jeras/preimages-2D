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

// math libraries
#include <math.h>

#include "ca2d.h"
#include "ca2d_rule.h"
#include "ca2d_array.h"

int unsigned ca2d_forward (ca2d_t ca2d, ca2d_size_t siz,
    int unsigned ai[siz.y][siz.x],
    int unsigned ao[siz.y-(ca2d.ngb.y-1)][siz.x-(ca2d.ngb.x-1)]
) {
    int unsigned at [ca2d.ngb.y] [ca2d.ngb.x];
    int unsigned nt;
    for (int unsigned y=0; y<siz.y-(ca2d.ngb.y-1); y++) {
        for (int unsigned x=0; x<siz.x-(ca2d.ngb.x-1); x++) {
            ca2d_array_slice (siz, (ca2d_size_t) {y, x}, ca2d.ngb, ai, at);
            ca2d_array_to_ui (ca2d.sts, ca2d.ngb, at, &nt);
            ao [y] [x] = ca2d.tab [nt];
        }
    }
    return 0;
}

