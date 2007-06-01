/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_SET_INTERNAL_H
#define USTR_SET_INTERNAL_H 1

#ifndef USTR_MAIN_H
# error " You should have already included ustr-main.h, or just include ustr.h"
#endif

USTR_CONF_e_PROTO int ustrp__set_undef(void *p, struct Ustr **ps1, size_t nlen)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__set_empty(void *p, struct Ustr **ps1)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
int ustrp__set_buf(void *p, struct Ustr **ps1, const void *buf, size_t len)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO int ustrp__set(void *p,struct Ustr **ps1,const struct Ustr*s2)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO
int ustrp__set_subustr(void *p, struct Ustr **ps1, const struct Ustr *s2,
                       size_t pos, size_t len)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO
int ustrp__set_rep_chr(void *p, struct Ustr **ps1, char chr, size_t len)
    USTR__COMPILE_ATTR_NONNULL_L((2));


#endif
