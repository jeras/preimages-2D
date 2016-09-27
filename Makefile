CFLAGS = -g -std=c99 -lm -lgmp

HEADERS = ca2d.h ca2d_rule.h ca2d_array.h ca2d_cfg.h ca2d_fwd.h ca2d_net.h
SOURCES =        ca2d_rule.c ca2d_array.c ca2d_cfg.c ca2d_fwd.c ca2d_net.c ca2d_base.c

CC = clang

all: $(SOURCES) $(HEADERS)
	$(CC) -o ca2d ca2d.c $(SOURCES) $(CFLAGS)
