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

#ifndef CA2D_NETWORK_H
#define CA2D_NETWORK_H

////////////////////////////////////////////////////////////////////////////////
// CA preimage network
////////////////////////////////////////////////////////////////////////////////

int ca2d_network_pointers (ca2d_t ca2d, int unsigned py [2] [2] [(size_t) pow (ca2d.sts, (ca2d.ngb.y+1) * (ca2d.ngb.x+1))]);

int ca2d_network_print (ca2d_t ca2d, ca2d_size_t siz,                                  int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]);
int ca2d_network       (ca2d_t ca2d, ca2d_size_t siz, int unsigned ca [siz.y] [siz.x], int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)]);

int ca2d_network_preimage (ca2d_t ca2d, ca2d_size_t siz,
    int unsigned res [siz.y] [siz.x] [(size_t) pow (ca2d.sts, ca2d.ngb.y * ca2d.ngb.x)],
    int unsigned ca [siz.y+(ca2d.ngb.y-1)] [siz.x+(ca2d.ngb.x-1)]
);

#endif
