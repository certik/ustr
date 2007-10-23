
HIDE=@

# These are tests which are run agains the currently installed ustr
# due to ustr-import and libustr usage.

TST_ALL = \
     tst_b_c_norm       tst_b_c_dbg1       tst_b_c_dbg2 \
     tst_b_i_norm       tst_b_i_dbg1       tst_b_i_dbg2 \
     tst_b_l_norm       tst_b_l_dbg1       tst_b_l_dbg2 \
     \
     tst_cmp_c_norm     tst_cmp_c_dbg1     tst_cmp_c_dbg2 \
     tst_cmp_i_norm     tst_cmp_i_dbg1     tst_cmp_i_dbg2 \
     tst_cmp_l_norm     tst_cmp_l_dbg1     tst_cmp_l_dbg2 \
     \
     tst_cntl_c_norm    tst_cntl_c_dbg1    tst_cntl_c_dbg2 \
     tst_cntl_l_norm    tst_cntl_l_dbg1    tst_cntl_l_dbg2 \
     \
     tst_fmt_c_norm     tst_fmt_c_dbg1     tst_fmt_c_dbg2 \
     tst_fmt_i_norm     tst_fmt_i_dbg1     tst_fmt_i_dbg2 \
     tst_fmt_l_norm     tst_fmt_l_dbg1     tst_fmt_l_dbg2 \
     \
     tst_ins_c_norm     tst_ins_c_dbg1     tst_ins_c_dbg2 \
     tst_ins_i_norm     tst_ins_i_dbg1     tst_ins_i_dbg2 \
     tst_ins_l_norm     tst_ins_l_dbg1     tst_ins_l_dbg2 \
     \
     tst_io_c_norm      tst_io_c_dbg1      tst_io_c_dbg2 \
     tst_io_i_norm      tst_io_i_dbg1      tst_io_i_dbg2 \
     tst_io_l_norm      tst_io_l_dbg1      tst_io_l_dbg2 \
     \
     tst_parse_c_norm   tst_parse_c_dbg1   tst_parse_c_dbg2 \
     tst_parse_i_norm   tst_parse_i_dbg1   tst_parse_i_dbg2 \
     tst_parse_l_norm   tst_parse_l_dbg1   tst_parse_l_dbg2 \
     \
     tst_pool_c_norm    tst_pool_c_dbg1    tst_pool_c_dbg2 \
     tst_pool_i_norm    tst_pool_i_dbg1    tst_pool_i_dbg2 \
     tst_pool_l_norm    tst_pool_l_dbg1    tst_pool_l_dbg2 \
     \
     tst_replace_c_norm tst_replace_c_dbg1 tst_replace_c_dbg2 \
     tst_replace_i_norm tst_replace_i_dbg1 tst_replace_i_dbg2 \
     tst_replace_l_norm tst_replace_l_dbg1 tst_replace_l_dbg2 \
     \
     tst_sc_c_norm      tst_sc_c_dbg1      tst_sc_c_dbg2 \
     tst_sc_i_norm      tst_sc_i_dbg1      tst_sc_i_dbg2 \
     tst_sc_l_norm      tst_sc_l_dbg1      tst_sc_l_dbg2 \
     \
     tst_set_c_norm     tst_set_c_dbg1     tst_set_c_dbg2 \
     tst_set_i_norm     tst_set_i_dbg1     tst_set_i_dbg2 \
     tst_set_l_norm     tst_set_l_dbg1     tst_set_l_dbg2 \
     \
     tst_split_c_norm   tst_split_c_dbg1   tst_split_c_dbg2 \
     tst_split_i_norm   tst_split_i_dbg1   tst_split_i_dbg2 \
     tst_split_l_norm   tst_split_l_dbg1   tst_split_l_dbg2 \
     \
     tst_spn_c_norm     tst_spn_c_dbg1     tst_spn_c_dbg2 \
     tst_spn_i_norm     tst_spn_i_dbg1     tst_spn_i_dbg2 \
     tst_spn_l_norm     tst_spn_l_dbg1     tst_spn_l_dbg2 \
     \
     tst_srch_c_norm    tst_srch_c_dbg1    tst_srch_c_dbg2 \
     tst_srch_i_norm    tst_srch_i_dbg1    tst_srch_i_dbg2 \
     tst_srch_l_norm    tst_srch_l_dbg1    tst_srch_l_dbg2 \
     \
     tst_sub_c_norm     tst_sub_c_dbg1     tst_sub_c_dbg2 \
     tst_sub_i_norm     tst_sub_i_dbg1     tst_sub_i_dbg2 \
     tst_sub_l_norm     tst_sub_l_dbg1     tst_sub_l_dbg2 \
     \
     tst_utf8_c_norm    tst_utf8_c_dbg1    tst_utf8_c_dbg2 \
     tst_utf8_i_norm    tst_utf8_i_dbg1    tst_utf8_i_dbg2 \
     tst_utf8_l_norm    tst_utf8_l_dbg1    tst_utf8_l_dbg2


AR = ar
RANLIB = ranlib

CFLAGS=-Wall -W -O1 -g

all: $(TST_ALL)
		$(HIDE)echo Done all.

tst_%_c_norm: dir-c_norm-%/libustr.a tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -o $@ tst_$*.c -Idir-c_norm-$* $<
		$(HIDE)rm -rf dir-c_norm-$*
tst_%_c_dbg1: dir-c_dbg1-%/libustr.a tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -o $@ tst_$*.c -Idir-c_dbg1-$* $<
		$(HIDE)rm -rf dir-c_dbg1-$*
tst_%_c_dbg2: dir-c_dbg2-%/libustr.a tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -o $@ tst_$*.c -Idir-c_dbg2-$* $<
		$(HIDE)rm -rf dir-c_dbg2-$*

tst_%_i_norm: dir-i_norm-% tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -o $@ tst_$*.c -I$<
		$(HIDE)rm -rf $<
tst_%_i_dbg1: dir-i_dbg1-% tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -o $@ tst_$*.c -I$<
		$(HIDE)rm -rf $<
tst_%_i_dbg2: dir-i_dbg2-% tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -o $@ tst_$*.c -I$<
		$(HIDE)rm -rf $<

tst_%_l_norm: tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -o $@ tst_$*.c -lustr
tst_%_l_dbg1: tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -DUSTR_DEBUG=1 -o $@ tst_$*.c -lustr-debug
tst_%_l_dbg2: tst_%.c
		$(HIDE)echo CC: $@
		$(HIDE)$(CC) $(CFLAGS) -DUSTR_DEBUG=1 -o $@ tst_$*.c -lustr-debug

clean:
		rm -f $(TST_ALL)
		rm -rf dir-*


# --------------------

dir-c_norm-%/libustr.a: dir-c_norm-%
		$(HIDE)$(CC) $(CFLAGS) -o $</ustr-main-opt-code.o -c $</ustr-main-opt-code.c
		$(HIDE)( for f in b cmp fmt ins io parse pool replace sc set split \
                      spn srch sub utf8; do \
                    f=$$f-opt-code; if [ -f $</ustr-$$f.c ]; then \
                    $(CC) $(CFLAGS) -o $</ustr-$$f.o -c $</ustr-$$f.c; fi \
                  done )
		$(HIDE)$(AR) ru $@ $</ustr-*-opt-code.o
		$(HIDE)$(RANLIB) $@

dir-c_dbg%/libustr.a: dir-c_dbg%
		$(HIDE)$(CC) $(CFLAGS) -o $</ustr-main-dbg-code.o -c $</ustr-main-dbg-code.c -I$<
		$(HIDE)( for f in b cmp fmt ins io parse pool replace sc set split \
                      spn srch sub utf8; do \
                    f=$$f-dbg-code; if [ -f $</ustr-$$f.c ]; then \
                    $(CC) $(CFLAGS) -o $</ustr-$$f.o -c $</ustr-$$f.c; fi \
                  done )
		$(HIDE)$(AR) ru $@ $</ustr-*-dbg-code.o
		$(HIDE)$(RANLIB) $@

# --------------------

dir-c_norm-%:
		$(HIDE)echo Creating: $@
		$(HIDE)mkdir $@
		$(HIDE)( cd $@ ; ustr-import       -c $* )

dir-c_dbg1-%:
		$(HIDE)echo Creating: $@
		$(HIDE)mkdir $@
		$(HIDE)( cd $@ ; ustr-import -d    -c $* )
		$(HIDE)mv $@/ustr-debug.h  $@/ustr.h 

dir-c_dbg2-%:
		$(HIDE)echo Creating: $@
		$(HIDE)mkdir $@
		$(HIDE)( cd $@ ; ustr-import -d -d -c $* )
		$(HIDE)mv $@/ustr-debug.h  $@/ustr.h 



dir-i_norm-%:
		$(HIDE)echo Creating: $@
		$(HIDE)mkdir $@
		$(HIDE)( cd $@ ; ustr-import       $* )

dir-i_dbg1-%:
		$(HIDE)echo Creating: $@
		$(HIDE)mkdir $@
		$(HIDE)( cd $@ ; ustr-import -d    $* )
		$(HIDE)mv $@/ustr-debug.h  $@/ustr.h 

dir-i_dbg2-%:
		$(HIDE)echo Creating: $@
		$(HIDE)mkdir $@
		$(HIDE)( cd $@ ; ustr-import -d -d $* )
		$(HIDE)mv $@/ustr-debug.h  $@/ustr.h 


# --------------------------------
# Borrowed from automake output...
# --------------------------------
PACKAGE_BUGREPORT = "james@and.org"
check: $(TST_ALL)
	$(HIDE)failed=0; all=0; xfail=0; xpass=0; skip=0; ws='[        ]'; \
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


