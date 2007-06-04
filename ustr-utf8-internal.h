/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_UTF8_INTERNAL_H
#define USTR_UTF8_INTERNAL_H 1

#ifndef USTR_MAIN_H
# error " You should have already included ustr-main.h, or just include ustr.h"
#endif

struct ustr__utf8_interval {
  int first;
  int last;
};

#include <wchar.h>

USTR_CONF_e_PROTO
int ustr__utf8_bisearch(wchar_t ucs,
                        const struct ustr__utf8_interval *table, int max);
USTR_CONF_e_PROTO
ssize_t ustr__utf8_mk_wcwidth(wchar_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET();

USTR_CONF_e_PROTO
wchar_t ustr__utf8_check(const unsigned char **s1)
    USTR__COMPILE_ATTR_NONNULL_A();


#endif
