#ifndef CA2D_CONFIGURATION_H
#define CA2D_CONFIGURATION_H

////////////////////////////////////////////////////////////////////////////////
// CA configuration handling
////////////////////////////////////////////////////////////////////////////////

int ca_read (char *filename, size2D_t siz, int unsigned ca [siz.y] [siz.x]);
int ca_print (size2D_t siz, int unsigned ca [siz.y] [siz.x]);

#endif