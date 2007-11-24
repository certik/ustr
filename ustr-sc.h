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

USTR_CONF_E_PROTO void ustr_sc_free_shared(struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_A();
/* USTR_CONF_E_PROTO
struct Ustr *ustr_sc_reconf(struct Ustr **, size_t, size_t, int, int)
USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6)); */
USTR_CONF_E_PROTO int ustr_sc_reverse(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
#ifdef USTR_UTF8_H
USTR_CONF_E_PROTO
int ustr_sc_utf8_reverse(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
#endif
USTR_CONF_E_PROTO int ustr_sc_tolower(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_sc_toupper(struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO char *ustr_sc_export_subustr(const struct Ustr*,size_t,size_t,
                                               void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO char *ustr_sc_export(const struct Ustr *, void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO int ustr_sc_lstrip_chrs(struct Ustr **, const char *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_sc_lstrip(struct Ustr **, const struct Ustr *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_sc_lstrip_cstr(struct Ustr **, const char *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_sc_rstrip_chrs(struct Ustr **, const char *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_sc_rstrip(struct Ustr **, const struct Ustr *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_sc_rstrip_cstr(struct Ustr **, const char *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_sc_strip_chrs(struct Ustr **, const char *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_sc_strip(struct Ustr **, const struct Ustr *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_sc_strip_cstr(struct Ustr **, const char *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO
struct Ustrp *ustrp_sc_dupx(struct Ustr_pool *, size_t, size_t, int, int,
                            struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_E_PROTO struct Ustrp *ustrp_sc_dup(struct Ustr_pool *,struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));

USTR_CONF_E_PROTO void ustrp_sc_free_shared(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
/* USTR_CONF_E_PROTO
struct Ustrp *ustrp_sc_reconf(struct Ustr_pool *, struct Ustrp **,
                              size_t, size_t, int, int)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6)); */
USTR_CONF_E_PROTO int ustrp_sc_reverse(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
#ifdef USTR_UTF8_H
USTR_CONF_E_PROTO
int ustrp_sc_utf8_reverse(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
#endif
USTR_CONF_E_PROTO int ustrp_sc_tolower(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustrp_sc_toupper(struct Ustr_pool *, struct Ustrp **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO
char *ustrp_sc_export_subustrp(struct Ustr_pool *, const struct Ustrp *,
                               size_t, size_t, void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_EI_PROTO
char *ustrp_sc_export(struct Ustr_pool *p,const struct Ustrp*,void *(*)(size_t))
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));

USTR_CONF_E_PROTO
int ustrp_sc_lstrip_chrs(struct Ustr_pool *, struct Ustrp **,const char*,size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_EI_PROTO int ustrp_sc_lstrip(struct Ustr_pool *, struct Ustrp **,
                                       const struct Ustrp *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_EI_PROTO
int ustrp_sc_lstrip_cstr(struct Ustr_pool *p, struct Ustrp **, const char *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_E_PROTO
int ustrp_sc_rstrip_chrs(struct Ustr_pool *, struct Ustrp **,const char*,size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_EI_PROTO int ustrp_sc_rstrip(struct Ustr_pool *, struct Ustrp **,
                                       const struct Ustrp *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_EI_PROTO
int ustrp_sc_rstrip_cstr(struct Ustr_pool *p, struct Ustrp **, const char *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_E_PROTO
int ustrp_sc_strip_chrs(struct Ustr_pool *, struct Ustrp **,const char *,size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_EI_PROTO int ustrp_sc_strip(struct Ustr_pool *, struct Ustrp **,
                                       const struct Ustrp *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_EI_PROTO
int ustrp_sc_strip_cstr(struct Ustr_pool *p, struct Ustrp **, const char *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
# include "ustr-sc-internal.h"
#endif

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-sc-code.h"
#endif

#if USTR_CONF_COMPILE_USE_INLINE
USTR_CONF_II_PROTO
char *ustr_sc_export(const struct Ustr *s1, void *(*my_alloc)(size_t))
{ return (ustr_sc_export_subustr(s1, 1, ustr_len(s1), my_alloc)); }
USTR_CONF_II_PROTO
char *ustrp_sc_export(struct Ustr_pool *p,
                      const struct Ustrp *s1, void *(*my_alloc)(size_t))
{ return (ustrp_sc_export_subustrp(p, s1, 1, ustrp_len(s1), my_alloc)); }

USTR_CONF_II_PROTO int ustr_sc_lstrip(struct Ustr **ps1, const struct Ustr *s2)
{ return (ustr_sc_lstrip_chrs(ps1, ustr_cstr(s2), ustr_len(s2))); }
USTR_CONF_II_PROTO int ustr_sc_lstrip_cstr(struct Ustr **ps1, const char *cstr)
{ return (ustr_sc_lstrip_chrs(ps1, cstr, strlen(cstr))); }

USTR_CONF_II_PROTO int ustr_sc_rstrip(struct Ustr **ps1, const struct Ustr *s2)
{ return (ustr_sc_rstrip_chrs(ps1, ustr_cstr(s2), ustr_len(s2))); }
USTR_CONF_II_PROTO int ustr_sc_rstrip_cstr(struct Ustr **ps1, const char *cstr)
{ return (ustr_sc_rstrip_chrs(ps1, cstr, strlen(cstr))); }

USTR_CONF_II_PROTO int ustr_sc_strip(struct Ustr **ps1, const struct Ustr *s2)
{ return (ustr_sc_strip_chrs(ps1, ustr_cstr(s2), ustr_len(s2))); }
USTR_CONF_II_PROTO int ustr_sc_strip_cstr(struct Ustr **ps1, const char *cstr)
{ return (ustr_sc_strip_chrs(ps1, cstr, strlen(cstr))); }

USTR_CONF_II_PROTO int ustrp_sc_lstrip(struct Ustr_pool *p, struct Ustrp **ps1,
                                     const struct Ustrp *s2)
{ return (ustrp_sc_lstrip_chrs(p, ps1, ustrp_cstr(s2), ustrp_len(s2))); }
USTR_CONF_II_PROTO
int ustrp_sc_lstrip_cstr(struct Ustr_pool *p, struct Ustrp **ps1, const char *c)
{ return (ustrp_sc_lstrip_chrs(p, ps1, c, strlen(c))); }

USTR_CONF_II_PROTO int ustrp_sc_rstrip(struct Ustr_pool *p, struct Ustrp **ps1,
                                     const struct Ustrp *s2)
{ return (ustrp_sc_rstrip_chrs(p, ps1, ustrp_cstr(s2), ustrp_len(s2))); }
USTR_CONF_II_PROTO
int ustrp_sc_rstrip_cstr(struct Ustr_pool *p, struct Ustrp **ps1, const char *c)
{ return (ustrp_sc_rstrip_chrs(p, ps1, c, strlen(c))); }

USTR_CONF_II_PROTO int ustrp_sc_strip(struct Ustr_pool *p, struct Ustrp **ps1,
                                     const struct Ustrp *s2)
{ return (ustrp_sc_strip_chrs(p, ps1, ustrp_cstr(s2), ustrp_len(s2))); }
USTR_CONF_II_PROTO
int ustrp_sc_strip_cstr(struct Ustr_pool *p, struct Ustrp **ps1, const char *c)
{ return (ustrp_sc_strip_chrs(p, ps1, c, strlen(c))); }


#endif


#endif
