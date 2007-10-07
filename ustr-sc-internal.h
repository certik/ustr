/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_SC_INTERNAL_H
#define USTR_SC_INTERNAL_H 1

#ifndef USTR_SC_H
# error " You should have already included ustr-sc.h, or just include ustr.h"
#endif

USTR_CONF_e_PROTO
struct Ustr *ustrp__sc_dupx(struct Ustr_pool *p,
                            size_t sz, size_t rbytes, int exact, int emem,
                            struct Ustr **ps1)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO struct Ustr *ustrp__sc_dup(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__sc_ensure_owner(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustrp__sc_free_shared(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustr__reverse(char *ptr, size_t pos, size_t len)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustrp__sc_reverse(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__sc_tolower(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__sc_toupper(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO char *ustrp__sc_wstr(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
char *ustrp__sc_export_subustr(struct Ustr_pool *, const struct Ustr *,
                               size_t, size_t, void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
    
#endif
