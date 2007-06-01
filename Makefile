
VERS=0.99
VERS_FULL=$(VERS).1
VERS_BSO =1.0
VERS_ESONAME =0.0.1
VERS_ESO     =$(VERS_ESONAME).0.1

DESTDIR =
datadir=/usr/share
libdir=/usr/lib
bindir=/usr/bin
SHRDIR=$(datadir)/ustr-$(VERS_FULL)
DOCSHRDIR=$(datadir)/doc/ustr-devel-$(VERS_FULL)


CC = gcc
AR = ar
RANLIB = ranlib

CFLAGS  = -O2 -g

# Debug versions...
WARNS = -W -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-format-zero-length -Wformat-nonliteral -Wformat-security # -Wfloat-equal -- no floats
DBG_CFLAGS = $(CFLAGS) $(WARNS) -O1 -ggdb3

# Inline tst_*.c and otst_*.c files.
CFLG_C = -DUSTR_CONF_INCLUDE_CODEONLY_HEADERS=1 -DUSTR_CONF_REF_BYTES=4

# Optimized o*.c files
CFLG_O = -DUSTR_DEBUG=0

CFLG_LIB_OPT = $(CFLG_O) -DNDEBUG
CFLG_LIB_DBG =

DOCS = TODO \
       Documentation/strdup\ vs.\ ustr.gnumeric \
       Documentation/index.html \
       Documentation/design.html

TST_ALL =  tst_0_sizes  tst_0_nothing  ctst_0_nothing \
          otst_0_sizes otst_0_nothing octst_0_nothing \
           tst_1_basic  tst_2_small  tst_3_medium  ctst_3_medium \
          otst_1_basic otst_2_small otst_3_medium octst_3_medium \
           tst_4_grow  ctst_4_grow  tst_5_shrink  ctst_5_shrink \
          otst_4_grow octst_4_grow otst_5_shrink octst_5_shrink \
           tst_6_tiny \
          otst_6_tiny \
           tst_7_auto  ctst_7_auto \
          otst_7_auto octst_7_auto \
           tst_8_spn  ctst_8_spn \
          otst_8_spn octst_8_spn \
           tst_9_pool  ctst_9_pool \
          otst_9_pool octst_9_pool \
           tst_10_b  ctst_10_b \
          otst_10_b octst_10_b
XFAIL_TESTS = 

SRC_HDRS = ustr.h      ustr-debug.h \
           ustr-conf.h ustr-conf-debug.h \
           ustr-b.h \
           ustr-main.h \
           ustr-set.h \
           ustr-fmt.h \
           ustr-cmp.h \
           ustr-srch.h \
           ustr-spn.h

SRC_SRCS = ustr-main-code.h \
           ustr-set-code.h \
           ustr-fmt-code.h \
           ustr-cmp-code.h \
           ustr-srch-code.h \
           ustr-spn-code.h \
           \
           ustr-main-dbg-code.c \
           ustr-set-dbg-code.c \
           ustr-fmt-dbg-code.c \
           ustr-cmp-dbg-code.c \
           ustr-srch-dbg-code.c \
           ustr-b-dbg-code.c \
           ustr-spn-dbg-code.c \
           \
           ustr-main-opt-code.c \
           ustr-set-opt-code.c \
           ustr-fmt-opt-code.c \
           ustr-cmp-opt-code.c \
           ustr-srch-opt-code.c \
           ustr-b-opt-code.c \
           ustr-spn-opt-code.c
XSRC_SRCS = .gdbinit

DBG_LIB_SHAREDEV    = libustr-debug.so
DBG_LIB_SHARED_NAME = libustr-debug-$(VERS_BSO).so.$(VERS_ESONAME)
DBG_LIB_SHARED      = libustr-debug-$(VERS_BSO).so.$(VERS_ESO)
DBG_LIB_STATIC      = libustr-debug.a

OPT_LIB_SHAREDEV    = libustr.so
OPT_LIB_SHARED_NAME = libustr-$(VERS_BSO).so.$(VERS_ESONAME)
OPT_LIB_SHARED      = libustr-$(VERS_BSO).so.$(VERS_ESO)
OPT_LIB_STATIC      = libustr.a

LIB_SHARED = $(DBG_LIB_SHARED) $(OPT_LIB_SHARED)
LIB_STATIC = $(DBG_LIB_STATIC) $(OPT_LIB_STATIC)

DEPS_NONC_ALL  = $(SRC_HDRS) $(SRC_SRCS)
DEPS_C_ALL     = $(SRC_HDRS)
OBJS_C_DBG_ALL = $(DBG_LIB_STATIC)
OBJS_C_OPT_ALL = $(OPT_LIB_STATIC)

LIB_SHARED_DBG = \
  ustr-main-code-so-dbg.o \
  ustr-b-code-so-dbg.o \
  ustr-cmp-code-so-dbg.o \
  ustr-fmt-code-so-dbg.o \
  ustr-set-code-so-dbg.o \
  ustr-srch-code-so-dbg.o \
  ustr-spn-code-so-dbg.o
LIB_STATIC_DBG = \
  ustr-main-code-a-dbg.o \
  ustr-b-code-a-dbg.o \
  ustr-cmp-code-a-dbg.o \
  ustr-fmt-code-a-dbg.o \
  ustr-set-code-a-dbg.o \
  ustr-srch-code-a-dbg.o \
  ustr-spn-code-a-dbg.o

LIB_SHARED_OPT = \
  ustr-main-code-so-opt.o \
  ustr-b-code-so-opt.o \
  ustr-cmp-code-so-opt.o \
  ustr-fmt-code-so-opt.o \
  ustr-set-code-so-opt.o \
  ustr-srch-code-so-opt.o \
  ustr-spn-code-so-opt.o
LIB_STATIC_OPT = \
  ustr-main-code-a-opt.o \
  ustr-b-code-a-opt.o \
  ustr-cmp-code-a-opt.o \
  ustr-fmt-code-a-opt.o \
  ustr-set-code-a-opt.o \
  ustr-srch-code-a-opt.o \
  ustr-spn-code-a-opt.o

all: ustr-import $(DBG_LIB_STATIC) $(TST_ALL)
		@echo Done static

all-shared: all $(LIB_SHARED)
		@echo Done shared

install: all $(TST_ALL) ustr.pc ustr-debug.pc
		@echo Making directories
		install -d $(DESTDIR)$(libdir)
		install -d $(DESTDIR)/usr/include
		install -d $(DESTDIR)$(SHRDIR)
		install -d $(DESTDIR)$(DOCSHRDIR)
		install -d $(DESTDIR)$(bindir)
		install -d $(DESTDIR)$(libdir)/pkgconfig
		@echo Installing files
		install -m 644 -t $(DESTDIR)$(libdir) $(LIB_SHARED) $(LIB_STATIC)
		ln -s $(OPT_LIB_SHARED) $(DESTDIR)$(libdir)/$(OPT_LIB_SHAREDEV)
		ln -s $(DBG_LIB_SHARED) $(DESTDIR)$(libdir)/$(DBG_LIB_SHAREDEV)
		-ldconfig -n $(DESTDIR)$(libdir)
		install -m 644 -t $(DESTDIR)/usr/include $(SRC_HDRS)
		install -m 644 -t $(DESTDIR)$(SHRDIR) $(SRC_SRCS)
		install -m 644 -t $(DESTDIR)$(SHRDIR) $(XSRC_SRCS)
		install -m 644 -t $(DESTDIR)$(DOCSHRDIR) $(DOCS)
		install -m 644 -t $(DESTDIR)$(bindir) ustr-import
		install -m 644 -t $(DESTDIR)/usr/lib/pkgconfig ustr.pc ustr-debug.pc

clean:
		rm -f $(LIB_SHARED) $(LIB_STATIC)
		rm -f $(TST_ALL)
		rm -f *.o
		rm -f perf-sizes perf-sizes32 perf-sizes64
		rm -f *.gcda *.gcno

distclean: clean
		rm -f ustr-import
		rm -f autoconf_64b autoconf_vsnprintf
		rm -f ustr-conf.h ustr-conf-debug.h


ustr-import: ustr-import.in autoconf_64b autoconf_vsnprintf
		@echo Creating $@
		@sz64=`./autoconf_64b`; vsnp=`./autoconf_vsnprintf`; \
		sed -e 's,@SHRDIR@,$(SHRDIR),g' -e 's,@VERS@,$(VERS),g'  -e 's,@VERS_FULL@,$(VERS_FULL),g' -e "s,@HAVE_64bit_SIZE_MAX@,$$sz64,g" -e "s,@HAVE_RETARDED_VSNPRINTF@,$$vsnp,g" < $< > $@
		@chmod 755 $@

# Use CFLAGS so that CFLAGS="... -m32" does the right thing
autoconf_64b: autoconf_64b.c
		@echo Compiling: auto configuration test:  64bit
		@$(CC) $(CFLAGS) -o $@ $<

autoconf_vsnprintf: autoconf_vsnprintf.c
		@echo Compiling: auto configuration test:  vsnprintf
		@$(CC) -o $@ $<

# Use LDFLAGS for LDFLAGS="-m32"
$(OPT_LIB_SHARED): $(LIB_SHARED_OPT)
		@echo Linking SO OPT lib: $@
		@$(CC) -shared $^ -Wl,-soname -Wl,$(OPT_LIB_SHARED_NAME) -Wl,-version-script -Wl,libustr.ver $(LDFLAGS) -o $@

$(DBG_LIB_SHARED): $(LIB_SHARED_DBG)
		@echo Linking SO DBG lib: $@
		@$(CC) -shared $^ -Wl,-soname -Wl,$(DBG_LIB_SHARED_NAME) -Wl,-version-script -Wl,libustr.ver $(LDFLAGS) -o $@

libustr.a: $(LIB_STATIC_OPT)
		@echo Linking A OPT lib: $@
		@$(AR) ru $@ $^
		@$(RANLIB) $@
libustr-debug.a: $(LIB_STATIC_DBG)
		@echo Linking A DBG lib: $@
		@$(AR) ru $@ $^
		@$(RANLIB) $@

ustr-conf.h: ustr-conf.h.in autoconf_64b autoconf_vsnprintf
		@echo Creating $@
		@have_stdint_h=0; dbg1=0; dbg2=0; \
                sz64=`./autoconf_64b`; vsnp=`./autoconf_vsnprintf`; \
                if test -f "/usr/include/stdint.h"; then have_stdint_h=1; fi; \
		sed -e "s,@HAVE_STDINT_H@,$$have_stdint_h,g" -e "s,@USE_ASSERT@,$$dbg1,g" -e "s,@USE_EOS_MARK@,$$dbg2,g" -e "s,@HAVE_64bit_SIZE_MAX@,$$sz64,g" -e "s,@HAVE_RETARDED_VSNPRINTF@,$$vsnp,g" < $< > $@

ustr-conf-debug.h: ustr-conf.h.in autoconf_64b autoconf_vsnprintf
		@echo Creating $@
		@have_stdint_h=0; dbg1=1; dbg2=1; \
                sz64=`./autoconf_64b`; vsnp=`./autoconf_vsnprintf`; \
                if test -f "/usr/include/stdint.h"; then have_stdint_h=1; fi; \
		sed -e "s,@HAVE_STDINT_H@,$$have_stdint_h,g" -e "s,@USE_ASSERT@,$$dbg1,g" -e "s,@USE_EOS_MARK@,$$dbg2,g" -e "s,@HAVE_64bit_SIZE_MAX@,$$sz64,g" -e "s,@HAVE_RETARDED_VSNPRINTF@,$$vsnp,g" < $< > $@


#  We don't use $(DEPS_NONC_ALL) as deps. because it then recompiles everything
# too much, and we care about speed more. Do make clean to "fix".
#  Yes, scons fixes this.
%-code-so-opt.o: %-opt-code.c %-code.h %.h $(DEPS_C_ALL)
		@echo Compiling for SO OPT lib: $<
		@$(CC) $(CFLAGS)     -fPIC $(CFLG_LIB_OPT) -o $@ -c $<

%-code-so-dbg.o: %-dbg-code.c %-code.h %.h $(DEPS_C_ALL)
		@echo Compiling for SO DBG lib: $<
		@$(CC) $(DBG_CFLAGS) -fPIC $(CFLG_LIB_DBG) -o $@ -c $<

%-code-a-opt.o:  %-opt-code.c %-code.h %.h $(DEPS_C_ALL)
		@echo Compiling for A OPT lib: $<
		@$(CC) $(CFLAGS)     $(CFLG_LIB_OPT) -o $@ -c $<

%-code-a-dbg.o:  %-dbg-code.c %-code.h %.h $(DEPS_C_ALL)
		@echo Compiling for A DBG lib: $<
		@$(CC) $(DBG_CFLAGS) $(CFLG_LIB_DBG) -o $@ -c $<


perf-sizes: perf-sizes.c $(OBJS_C_OPT_ALL)
		$(CC) $(WARNS) -O2 -g $(LDFLAGS) -o $@ $^

perf-sizes32: perf-sizes.c $(OBJS_C_OPT_ALL)
		$(CC) $(WARNS) -O2 -g -m32 -o $@ $^

perf-sizes64: perf-sizes.c $(OBJS_C_OPT_ALL)
		$(CC) $(WARNS) -O2 -g -m64 -o $@ $^

# Debugging tst and "compiled" tst (links with libustr-debug)
tst_%.o: tst_%.c tst.h $(DEPS_NONC_ALL)
		@echo Compiling: $@
		@$(CC) $(DBG_CFLAGS) $(CFLG_C) -o $@ -c $<

tst_%: tst_%.o
		@echo Linking: $@
		@$(CC) $(LDFLAGS)  -o $@ $<

ctst_%.o: ctst_%.c tst.h $(DEPS_C_ALL)
		@echo Compiling: $@
		@$(CC) $(DBG_CFLAGS) -o $@ -c $<

ctst_%: ctst_%.o $(OBJS_C_DBG_ALL)
		@echo Linking: $@
		@$(CC) $(LDFLAGS) -o $@ $^

# Optimized tst and "compiled" tst (links with libustr)
otst_%.o: tst_%.c tst.h $(DEPS_NONC_ALL)
		@echo Compiling: $@
		@$(CC) $(CFLAGS) $(CFLG_O) $(CFLG_C) -o $@ -c $<

otst_%: otst_%.o
		@echo Linking: $@
		@$(CC) $(LDFLAGS)  -o $@ $<

octst_%.o: ctst_%.c tst.h $(DEPS_C_ALL)
		@echo Compiling: $@
		@$(CC) $(CFLAGS) $(CFLG_O) -o $@ -c $<

octst_%: octst_%.o $(OBJS_C_OPT_ALL)
		@echo Linking: $@
		@$(CC) $(LDFLAGS) -o $@ $^


# --------------------------------
# Borrowed from automake output...
# --------------------------------
PACKAGE_BUGREPORT = "james@and.org"
check: all
	@failed=0; all=0; xfail=0; xpass=0; skip=0; ws='[        ]'; \
	list=' $(TST_ALL) '; \
        if test -n "$$list"; then \
	  for tst in $$list; do \
	    if ./$$tst; then \
              all=`expr $$all + 1`; \
              case " $(XFAIL_TESTS) " in \
              *$$ws$$tst$$ws*) \
                xpass=`expr $$xpass + 1`; \
                failed=`expr $$failed + 1`; \
                echo "XPASS: $$tst"; \
              ;; \
              *) \
                echo "PASS: $$tst"; \
              ;; \
              esac; \
            elif test $$? -ne 77; then \
              all=`expr $$all + 1`; \
              case " $(XFAIL_TESTS) " in \
              *$$ws$$tst$$ws*) \
                xfail=`expr $$xfail + 1`; \
                echo "XFAIL: $$tst"; \
              ;; \
              *) \
                failed=`expr $$failed + 1`; \
                echo "FAIL: $$tst"; \
              ;; \
              esac; \
            else \
              skip=`expr $$skip + 1`; \
              echo "SKIP: $$tst"; \
            fi; \
          done; \
          if test "$$failed" -eq 0; then \
            if test "$$xfail" -eq 0; then \
              banner="All $$all tests passed"; \
            else \
              banner="All $$all tests behaved as expected ($$xfail expected failures)"; \
            fi; \
          else \
            if test "$$xpass" -eq 0; then \
              banner="$$failed of $$all tests failed"; \
            else \
              banner="$$failed of $$all tests did not behave as expected ($$xpass unexpected passes)"; \
            fi; \
          fi; \
          dashes="$$banner"; \
          skipped=""; \
          if test "$$skip" -ne 0; then \
            skipped="($$skip tests were not run)"; \
            test `echo "$$skipped" | wc -c` -le `echo "$$banner" | wc -c` || \
              dashes="$$skipped"; \
          fi; \
          report=""; \
          if test "$$failed" -ne 0 && test -n "$(PACKAGE_BUGREPORT)"; then \
            report="Please report to $(PACKAGE_BUGREPORT)"; \
            test `echo "$$report" | wc -c` -le `echo "$$banner" | wc -c` || \
              dashes="$$report"; \
          fi; \
          dashes=`echo "$$dashes" | sed s/./=/g`; \
          echo "$$dashes"; \
          echo "$$banner"; \
          test -z "$$skipped" || echo "$$skipped"; \
          test -z "$$report" || echo "$$report"; \
          echo "$$dashes"; \
          test "$$failed" -eq 0; \
        else :; fi


