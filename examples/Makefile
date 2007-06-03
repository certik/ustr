
WARNS = -W -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-format-zero-length -Wformat-nonliteral -Wformat-security # -Wfloat-equal -- no floats

DEF_CFLAGS  = -O2 -g

## Compile "inline" using parent dir.
#CFLAGS  = $(DEF_CFLAGS) -DUSTR_CONF_INCLUDE_CODEONLY_HEADERS=1 -I..
#LDFLAGS = 

## Compile "static" using parent dir.
#CFLAGS  = $(DEF_CFLAGS) -I..
#LDFLAGS = ../libustr.a

## Compile "inline" using ustr-import output
#CFLAGS  = $(DEF_CFLAGS)
#LDFLAGS =

## Compile "inline" using /ustr/include/ustr*.h
#CFLAGS  = $(DEF_CFLAGS) -DUSTR_CONF_INCLUDE_CODEONLY_HEADERS=1
#LDFLAGS =

# Compile "normal" using /ustr/include/ustr*.h and /usr/lib/libustr.so
CFLAGS  = $(DEF_CFLAGS)
LDFLAGS = -lustr

# This is a simple makefile for example programs...

ALL = netstr hello_world

all: $(ALL)

clean:
		rm -f *.o $(ALL)

netstr: netstr.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

hello_world: hello_world.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
