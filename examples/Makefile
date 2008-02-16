

CC = cc
AR = ar
RANLIB = ranlib


WARNS = -W -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-format-zero-length -Wformat-nonliteral -Wformat-security # -Wfloat-equal -- no floats

# DEF_CFLAGS = -Os -g
DEF_CFLAGS = -O2 -g # $(WARNS)

CFLG_LFS = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

CFLAGS  = $(DEF_CFLAGS) $(CFLG_LFS)

CFLG_LIB_OPT = $(CFLAGS) -DUSTR_DEBUG=0 -DNDEBUG
CFLG_LIB_DBG = $(CFLAGS)

# Compile "normal" using /ustr/include/ustr*.h and /usr/lib/libustr.so
PCFLAGS = $(CFLAGS)
LDFLAGS = -lustr
DEPS    =

## Compile "debug" using /ustr/include/ustr*.h and /usr/lib/libustr-debug.so
#PCFLAGS = $(CFLAGS) $(WARNS) -DUSTR_DEBUG=1
#LDFLAGS = -lustr-debug
#DEPS    =

## Compile "inline" using parent dir.
#PCFLAGS = $(CFLAGS) -DUSTR_CONF_INCLUDE_CODEONLY_HEADERS=1 -I..
#LDFLAGS =

## Compile "static" using parent dir.
#PCFLAGS = $(CFLAGS) -I..
#LDFLAGS = ../libustr.a
#PCFLAGS = $(CFLAGS) $(WARNS) -I.. -DUSTR_DEBUG=1
#LDFLAGS = ../libustr-debug.a

## Compile "inline" using ustr-import output
# - do nothing

## Compile "inline" using /ustr/include/ustr*.h / ustr-import -c
#PCFLAGS = $(CFLAGS) -DUSTR_CONF_INCLUDE_CODEONLY_HEADERS=1
#LDFLAGS  =

## Compile "static" opt using ustr-import
#PCFLAGS = $(CFLAGS)
#DEPS    = libustr.a
#LDFLAGS = libustr.a

## Compile "static" dbg using ustr-import
#PCFLAGS = $(CFLAGS) $(WARNS)
#DEPS    = libustr-debug.a
#LDFLAGS = libustr-debug.a


# This is a simple makefile for example programs...

ALL = hello_world netstr hexdump nums mkdir_p fgrep txt2html custr \
      linecat_buf linecat_sized basename dirname

DBG_LIB_STATIC      = libustr-debug.a
OPT_LIB_STATIC      = libustr.a
LIB_STATIC = $(DBG_LIB_STATIC) $(OPT_LIB_STATIC)

LIB_STATIC_DBG = \
  ustr-b-code-a-dbg.o \
  ustr-cmp-code-a-dbg.o \
  ustr-fmt-code-a-dbg.o \
  ustr-io-code-a-dbg.o \
  ustr-ins-code-a-dbg.o \
  ustr-main-code-a-dbg.o \
  ustr-parse-code-a-dbg.o \
  ustr-pool-code-a-dbg.o \
  ustr-sc-code-a-dbg.o \
  ustr-set-code-a-dbg.o \
  ustr-spn-code-a-dbg.o \
  ustr-srch-code-a-dbg.o \
  ustr-utf8-code-a-dbg.o
LIB_STATIC_OPT = \
  ustr-b-code-a-opt.o \
  ustr-cmp-code-a-opt.o \
  ustr-fmt-code-a-opt.o \
  ustr-io-code-a-opt.o \
  ustr-ins-code-a-opt.o \
  ustr-main-code-a-opt.o \
  ustr-parse-code-a-opt.o \
  ustr-pool-code-a-opt.o \
  ustr-sc-code-a-opt.o \
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

hello_world: hello_world.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

netstr: netstr.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

hexdump: hexdump.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

nums: nums.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

mkdir_p: mkdir_p.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

fgrep: fgrep.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

txt2html: txt2html.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

custr: custr.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

linecat_buf: linecat.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS) -DCAT_TYPE=1

linecat_sized: linecat.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS) -DCAT_TYPE=2

basename: basename.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)

dirname: dirname.c
		$(CC) $(PCFLAGS) -o $@ $< $(LDFLAGS)


strip: $(ALL)
		@for i in $(ALL); do \
		echo "Creating: $$i-s"; cp $$i $$i-s; strip $$i-s; \
		done

clean:
		rm -f *.o $(ALL) $(LIB_STATIC) *-s

