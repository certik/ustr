/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_SET_H
#define USTR_SET_H 1

#ifndef USTR_MAIN_H
#error " You should have already included ustr-main.h, or just include ustr.h."
#endif


USTR_CONF_E_PROTO int ustr_set_undef(struct Ustr **, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_set_buf(struct Ustr **, const void *, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_set_cstr(struct Ustr **, const char *)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_set(struct Ustr **, const struct Ustr *)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
int ustr_set_subustr(struct Ustr **, const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_set_rep_chr(struct Ustr **, char, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO int ustrp_set_undef(void *, struct Ustrp **, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_E_PROTO int ustrp_set_buf(void *, struct Ustrp **,const void *,size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_EI_PROTO int ustrp_set_cstr(void *, struct Ustrp **, const char *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_E_PROTO int ustrp_set(void *, struct Ustrp **, const struct Ustrp *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_E_PROTO
int ustrp_set_subustrp(void *, struct Ustrp**,const struct Ustrp*,size_t,size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_E_PROTO int ustrp_set_rep_chr(void *, struct Ustrp **, char, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-set-code.h"
#endif

USTR_CONF_II_PROTO int ustr_set_cstr(struct Ustr **ps1, const char *cstr)
{ return (ustr_set_buf(ps1, cstr, strlen(cstr))); }

USTR_CONF_II_PROTO int ustrp_set_cstr(void*p,struct Ustrp **ps1,const char*cstr)
{ return (ustrp_set_buf(p, ps1, cstr, strlen(cstr))); }

#endif
