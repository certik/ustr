/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SUB_H
#define USTR_SUB_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

USTR_CONF_E_PROTO int ustr_sub_undef(struct Ustr **ps1, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_sub_buf(struct Ustr **, size_t, const void *, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_sub(struct Ustr **, size_t, const struct Ustr *)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_sub_cstr(struct Ustr **, size_t, const char *)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
int ustr_sub_subustr(struct Ustr **, size_t, const struct Ustr *, size_t,size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_sub_rep_chr(struct Ustr **, size_t, char, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();

/* FIXME: ustr_sub_fmt*() */

USTR_CONF_E_PROTO
int ustrp_sub_undef(struct Ustr_pool *, struct Ustrp **, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_E_PROTO
int ustrp_sub_buf(struct Ustr_pool *,struct Ustrp **,size_t,const void *,size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4));
USTR_CONF_E_PROTO
int ustrp_sub(struct Ustr_pool *, struct Ustrp **, size_t, const struct Ustrp *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4));
USTR_CONF_EI_PROTO
int ustrp_sub_cstr(struct Ustr_pool *, struct Ustrp **, size_t, const char *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4));
USTR_CONF_E_PROTO
int ustrp_sub_subustrp(struct Ustr_pool *, struct Ustrp **, size_t,
                       const struct Ustrp *, size_t,size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4));
USTR_CONF_E_PROTO
int ustrp_sub_rep_chr(struct Ustr_pool *, struct Ustrp **, size_t, char, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));

USTR_CONF_E_PROTO
int ustr_sc_sub_undef(struct Ustr **, size_t, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
int ustr_sc_sub_buf(struct Ustr **, size_t, size_t, const void *, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
int ustr_sc_sub(struct Ustr **, size_t, size_t, const struct Ustr *)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO
int ustr_sc_sub_cstr(struct Ustr **, size_t, size_t, const char *)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
int ustr_sc_sub_subustr(struct Ustr **, size_t, size_t,
                        const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
int ustr_sc_sub_rep_chr(struct Ustr **, size_t, size_t, char, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO
int ustrp_sc_sub_undef(struct Ustr_pool *, struct Ustrp **,size_t,size_t,size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_E_PROTO
int ustrp_sc_sub_buf(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                     const void *, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5));
USTR_CONF_E_PROTO
int ustrp_sc_sub(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                 const struct Ustrp *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5));
USTR_CONF_EI_PROTO
int ustrp_sc_sub_cstr(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                      const char *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5));
USTR_CONF_E_PROTO
int ustrp_sc_sub_subustrp(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                          const struct Ustrp *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5));
USTR_CONF_E_PROTO
int ustrp_sc_sub_rep_chr(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                         char, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));

#ifdef USTR_SRCH_H
USTR_CONF_E_PROTO size_t ustr_sc_replace_buf(struct Ustr **,
                                             const void *, size_t,
                                             const void *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO
size_t ustr_sc_replace_cstr(struct Ustr **, const char *, const char *, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO size_t ustr_sc_replace(struct Ustr **, const struct Ustr *,
                                         const struct Ustr *, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO
size_t ustrp_sc_replace_buf(struct Ustr_pool *, struct Ustrp **,
                            const void *, size_t, const void *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3, 5));
USTR_CONF_EI_PROTO
size_t ustrp_sc_replace_cstr(struct Ustr_pool *, struct Ustrp **,
                             const char *, const char *, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3, 4));
USTR_CONF_E_PROTO
size_t ustrp_sc_replace(struct Ustr_pool *,struct Ustrp **,const struct Ustrp *,
                        const struct Ustrp *, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3, 4));
#endif

#ifdef USTR_FMT_H
# if USTR_CONF_HAVE_VA_COPY
/* sub_fmt */
USTR_CONF_E_PROTO
int ustr_sub_vfmt_lim(struct Ustr **, size_t, size_t, const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((1, 4)) USTR__COMPILE_ATTR_FMT(4, 0);
USTR_CONF_E_PROTO
int ustr_sub_vfmt(struct Ustr **, size_t, const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((1, 3)) USTR__COMPILE_ATTR_FMT(3, 0);

USTR_CONF_E_PROTO
int ustrp_sub_vfmt_lim(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                       const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5)) USTR__COMPILE_ATTR_FMT(5, 0);
USTR_CONF_E_PROTO int ustrp_sub_vfmt(struct Ustr_pool *, struct Ustrp **,size_t,
                                     const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4)) USTR__COMPILE_ATTR_FMT(4, 0);
USTR_CONF_E_PROTO
int ustr_sub_fmt_lim(struct Ustr **, size_t, size_t, const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_L((1, 4)) USTR__COMPILE_ATTR_FMT(4, 5);
USTR_CONF_E_PROTO int ustr_sub_fmt(struct Ustr **, size_t, const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_L((1, 3)) USTR__COMPILE_ATTR_FMT(3, 4);

USTR_CONF_E_PROTO
int ustrp_sub_fmt_lim(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                      const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5)) USTR__COMPILE_ATTR_FMT(5, 6);
USTR_CONF_E_PROTO
int ustrp_sub_fmt(struct Ustr_pool *, struct Ustrp **,size_t, const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4)) USTR__COMPILE_ATTR_FMT(4, 5);

/* sc_sub_fmt */
USTR_CONF_E_PROTO
int ustr_sc_sub_vfmt_lim(struct Ustr **, size_t, size_t,
                         size_t, const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((1, 5)) USTR__COMPILE_ATTR_FMT(5, 0);
USTR_CONF_E_PROTO
int ustr_sc_sub_vfmt(struct Ustr **, size_t, size_t, const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((1, 4)) USTR__COMPILE_ATTR_FMT(4, 0);

USTR_CONF_E_PROTO
int ustrp_sc_sub_vfmt_lim(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                          size_t, const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((2, 6)) USTR__COMPILE_ATTR_FMT(6, 0);
USTR_CONF_E_PROTO
int ustrp_sc_sub_vfmt(struct Ustr_pool *, struct Ustrp **,size_t, size_t,
                      const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5)) USTR__COMPILE_ATTR_FMT(5, 0);
USTR_CONF_E_PROTO
int ustr_sc_sub_fmt_lim(struct Ustr **, size_t, size_t, size_t,
                        const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_L((1, 5)) USTR__COMPILE_ATTR_FMT(5, 6);
USTR_CONF_E_PROTO int ustr_sc_sub_fmt(struct Ustr **, size_t, size_t,
                                      const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_L((1, 4)) USTR__COMPILE_ATTR_FMT(4, 5);

USTR_CONF_E_PROTO
int ustrp_sc_sub_fmt_lim(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                         size_t, const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_L((2, 6)) USTR__COMPILE_ATTR_FMT(6, 7);
USTR_CONF_E_PROTO
int ustrp_sc_sub_fmt(struct Ustr_pool *, struct Ustrp **, size_t, size_t,
                     const char *, ...)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5)) USTR__COMPILE_ATTR_FMT(5, 6);
# endif
#endif

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
# include "ustr-sub-internal.h"
#endif

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-sub-code.h"
#endif

#if USTR_CONF_COMPILE_USE_INLINE
USTR_CONF_II_PROTO int ustr_sub_cstr(struct Ustr **s1, size_t p, const char *c) 
{ return (ustr_sub_buf(s1, p, c, strlen(c))); }
USTR_CONF_II_PROTO
int ustr_sc_sub_cstr(struct Ustr **s1, size_t pos, size_t len,const char *cstr) 
{ return (ustr_sc_sub_buf(s1, pos, len, cstr, strlen(cstr))); }
USTR_CONF_II_PROTO
size_t ustr_sc_replace_cstr(struct Ustr **s1, const char *oc, const char *nc,
                            size_t lim)
{ return (ustr_sc_replace_buf(s1, oc, strlen(oc), nc, strlen(nc), lim)); }

USTR_CONF_II_PROTO int ustrp_sub_cstr(struct Ustr_pool *x, struct Ustrp **s1,
                                      size_t p, const char *c) 
{ return (ustrp_sub_buf(x, s1, p, c, strlen(c))); }
USTR_CONF_II_PROTO
int ustrp_sc_sub_cstr(struct Ustr_pool *p, struct Ustrp **s1,
                      size_t pos, size_t len, const char *cstr)
{ return (ustrp_sc_sub_buf(p, s1, pos, len, cstr, strlen(cstr))); }
# ifdef USTR_SRCH_H
USTR_CONF_II_PROTO
size_t ustrp_sc_replace_cstr(struct Ustr_pool *p, struct Ustrp **s1,
                             const char *oc, const char *nc, size_t lim)
{ return (ustrp_sc_replace_buf(p, s1, oc, strlen(oc), nc, strlen(nc), lim)); }
# endif
#endif


#endif
