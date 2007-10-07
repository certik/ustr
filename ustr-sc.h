/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_SC_H
#define USTR_SC_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

USTR_CONF_E_PROTO
struct Ustr *ustr_sc_dupx(size_t, size_t, int, int, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO struct Ustr *ustr_sc_dup(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO int ustr_sc_ensure_owner(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO void ustr_sc_free_shared(struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_A();
/* USTR_CONF_E_PROTO
struct Ustr *ustr_sc_reconf(struct Ustr **, size_t, size_t, int, int)
USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6)); */
USTR_CONF_E_PROTO int ustr_sc_reverse(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_sc_tolower(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_sc_toupper(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO char *ustr_sc_wstr(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO char *ustr_sc_export_subustr(struct Ustr *, size_t, size_t,
                                               void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO char *ustr_sc_export(struct Ustr *, void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO
struct Ustrp *ustrp_sc_dupx(struct Ustr_pool *, size_t, size_t, int, int,
                            struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_E_PROTO struct Ustrp *ustrp_sc_dup(struct Ustr_pool *,struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_E_PROTO int ustrp_sc_ensure_owner(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));

USTR_CONF_E_PROTO void ustrp_sc_free_shared(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
/* USTR_CONF_E_PROTO
struct Ustrp *ustrp_sc_reconf(struct Ustr_pool *, struct Ustrp **,
                              size_t, size_t, int, int)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6)); */
USTR_CONF_E_PROTO int ustrp_sc_reverse(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustrp_sc_tolower(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustrp_sc_toupper(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO char *ustrp_sc_wstr(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));

USTR_CONF_E_PROTO
char *ustrp_sc_export_subustrp(struct Ustr_pool *,
                               struct Ustrp *, size_t,size_t, void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_EI_PROTO
char *ustrp_sc_export(struct Ustr_pool *p, struct Ustrp *,  void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
# include "ustr-sc-internal.h"
#endif

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-sc-code.h"
#endif

#if USTR_CONF_COMPILE_USE_INLINE
USTR_CONF_II_PROTO
char *ustr_sc_export(struct Ustr *s1, void *(*my_alloc)(size_t))
{ return (ustr_sc_export_subustr(s1, 1, ustr_len(s1), my_alloc)); }
USTR_CONF_II_PROTO
char *ustrp_sc_export(struct Ustr_pool *p, struct Ustrp*s1, void*(*mya)(size_t))
{ return (ustrp_sc_export_subustrp(p, s1, 1, ustrp_len(s1), mya)); }
#endif


#endif
