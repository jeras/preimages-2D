CFLAGS = -g -std=c99 -lm -lgmp

HEADERS = ca2d.h ca2d_rule.h ca2d_array.h ca2d_configuration.h ca2d_forward.h
SOURCES = ca2d_preimages.c ca2d_rule.c ca2d_array.c ca2d_configuration.c ca2d_forward.c

CC = clang

all: $(SOURCES) $(HEADERS)
	$(CC) -o ca2d_preimages $(SOURCES) $(CFLAGS)
