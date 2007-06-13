
CC = cc
AR = ar
RANLIB = ranlib


WARNS = -W -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-format-zero-length -Wformat-nonliteral -Wformat-security # -Wfloat-equal -- no floats

CFLG_LFS = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

CFLAGS  = -O2 -g $(CFLG_LFS)

PCFLAGS = $(DEF_CFLAGS)
LDFLAGS =
DEPS    =

CFLG_LIB_OPT = $(CFLAGS) -DUSTR_DEBUG=0 -DNDEBUG
CFLG_LIB_DBG = $(CFLAGS)

## Compile "inline" using parent dir.
#PCFLAGS = $(CFLAGS) -DUSTR_CONF_INCLUDE_CODEONLY_HEADERS=1 -I..

## Compile "static" using parent dir.
#PCFLAGS = $(CFLAGS) -I..
#LDFLAGS = ../libustr.a

## Compile "inline" using ustr-import output
# - do nothing

## Compile "inline" using /ustr/include/ustr*.h / ustr-import -c
#PCFLAGS = $(CFLAGS) -DUSTR_CONF_INCLUDE_CODEONLY_HEADERS=1

## Compile "static" opt using ustr-import
#PCFLAGS = $(CFLAGS)
#DEPS    = libustr.a
#LDFLAGS = libustr.a

## Compile "static" dbg using ustr-import
#PCFLAGS = $(CFLAGS)
#DEPS    = libustr-debug.a
#LDFLAGS = libustr-debug.a

# Compile "normal" using /ustr/include/ustr*.h and /usr/lib/libustr.so
LDFLAGS = -lustr

# This is a simple makefile for example programs...

ALL = hello_world netstr hexdump

DBG_LIB_STATIC      = libustr-debug.a
OPT_LIB_STATIC      = libustr.a
LIB_STATIC = $(DBG_LIB_STATIC) $(OPT_LIB_STATIC)

LIB_STATIC_DBG = \
  ustr-b-code-a-dbg.o \
  ustr-cmp-code-a-dbg.o \
  ustr-fmt-code-a-dbg.o \
  ustr-io-code-a-dbg.o \
  ustr-main-code-a-dbg.o \
  ustr-set-code-a-dbg.o \
  ustr-spn-code-a-dbg.o \
  ustr-srch-code-a-dbg.o \
  ustr-utf8-code-a-dbg.o
LIB_STATIC_OPT = \
  ustr-b-code-a-opt.o \
  ustr-cmp-code-a-opt.o \
  ustr-fmt-code-a-opt.o \
  ustr-io-code-a-opt.o \
  ustr-main-code-a-opt.o \
  ustr-set-code-a-opt.o \
  ustr-spn-code-a-opt.o \
  ustr-srch-code-a-opt.o \
  ustr-utf8-code-a-opt.o




all: $(DEPS) $(ALL)

libustr.a: $(LIB_STATIC_OPT)
		@echo Linking A OPT lib: $@
		@$(AR) ru $@ $^
		@$(RANLIB) $@
libustr-debug.a: $(LIB_STATIC_DBG)
		@echo Linking A DBG lib: $@
		@$(AR) ru $@ $^
		@$(RANLIB) $@

%-code-a-opt.o:  %-opt-code.c %-code.h %.h
		@echo Compiling for A OPT lib: $<
		@$(CC) $(CFLG_LIB_OPT) -o $@ -c $<

%-code-a-dbg.o:  %-dbg-code.c %-code.h %.h
		@echo Compiling for A DBG lib: $<
		@$(CC) $(CFLG_LIB_DBG) -o $@ -c $<

clean:
		rm -f *.o $(ALL) $(LIB_STATIC)

hello_world: hello_world.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

netstr: netstr.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

hexdump: hexdump.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

