CFLAGS = -g -std=c99 -lm -lgmp

HEADERS = ca2d.h ca2d_array.h ca2d_configuration.h
SOURCES = ca2d_preimages.c ca2d_array.c ca2d_configuration.c

all: $(SOURCES) $(HEADERS)
	gcc -o ca2d_preimages $(SOURCES) $(CFLAGS)
