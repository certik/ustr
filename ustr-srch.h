/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_SRCH_H
#define USTR_SRCH_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

USTR_CONF_E_PROTO size_t ustr_srch_chr_fwd(const struct Ustr *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO size_t ustr_srch_chr_rev(const struct Ustr *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO
size_t ustr_srch_buf_fwd(const struct Ustr *, const void *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
size_t ustr_srch_buf_rev(const struct Ustr *, const void *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_EI_PROTO size_t ustr_srch_fwd(const struct Ustr *,const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO size_t ustr_srch_rev(const struct Ustr *,const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_EI_PROTO size_t ustr_srch_cstr_fwd(const struct Ustr *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO size_t ustr_srch_cstr_rev(const struct Ustr *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO
size_t ustr_srch_subustr_fwd(const struct Ustr *,
                             const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
size_t ustr_srch_subustr_rev(const struct Ustr *,
                             const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-srch-code.h"
#endif

USTR_CONF_II_PROTO
size_t ustr_srch_fwd(const struct Ustr *s1, const struct Ustr *s2)
{ return (ustr_srch_buf_fwd(s1, ustr_cstr(s2), ustr_len(s2))); }
USTR_CONF_II_PROTO
size_t ustr_srch_rev(const struct Ustr *s1, const struct Ustr *s2)
{ return (ustr_srch_buf_rev(s1, ustr_cstr(s2), ustr_len(s2))); }

USTR_CONF_II_PROTO
size_t ustr_srch_cstr_fwd(const struct Ustr *s1, const char *cstr)
{ return (ustr_srch_buf_fwd(s1, cstr, strlen(cstr))); }
USTR_CONF_II_PROTO
size_t ustr_srch_cstr_rev(const struct Ustr *s1, const char *cstr)
{ return (ustr_srch_buf_rev(s1, cstr, strlen(cstr))); }

/* ---------------- pool wrapper APIs ---------------- */

USTR_CONF_EI_PROTO size_t ustrp_srch_chr_fwd(const struct Ustrp *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO size_t ustrp_srch_chr_fwd(const struct Ustrp *s1, char chr)
{ return (ustr_srch_chr_fwd(&s1->s, chr)); }

USTR_CONF_EI_PROTO size_t ustrp_srch_chr_rev(const struct Ustrp *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO size_t ustrp_srch_chr_rev(const struct Ustrp *s1, char chr)
{ return (ustr_srch_chr_rev(&s1->s, chr)); }

USTR_CONF_EI_PROTO
size_t ustrp_srch_buf_fwd(const struct Ustrp *, const void *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_srch_buf_fwd(const struct Ustrp *s1, const void *buf, size_t len)
{ return (ustr_srch_buf_fwd(&s1->s, buf, len)); }

USTR_CONF_EI_PROTO
size_t ustrp_srch_buf_rev(const struct Ustrp *, const void *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_srch_buf_rev(const struct Ustrp *s1, const void *buf, size_t len)
{ return (ustr_srch_buf_rev(&s1->s, buf, len)); }

USTR_CONF_EI_PROTO
size_t ustrp_srch_fwd(const struct Ustrp *, const struct Ustrp *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_srch_fwd(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (ustrp_srch_buf_fwd(s1, ustrp_cstr(s2), ustrp_len(s2))); }
USTR_CONF_EI_PROTO
size_t ustrp_srch_rev(const struct Ustrp *, const struct Ustrp *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_srch_rev(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (ustrp_srch_buf_rev(s1, ustrp_cstr(s2), ustrp_len(s2))); }

USTR_CONF_EI_PROTO
size_t ustrp_srch_subustrp_fwd(const struct Ustrp *,
                               const struct Ustrp *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_srch_subustrp_fwd(const struct Ustrp *s1,
                               const struct Ustrp *s2, size_t pos, size_t len)
{ return (ustr_srch_subustr_fwd(&s1->s, &s2->s, pos, len)); }
USTR_CONF_EI_PROTO
size_t ustrp_srch_subustrp_rev(const struct Ustrp *,
                               const struct Ustrp *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_srch_subustrp_rev(const struct Ustrp *s1,
                               const struct Ustrp *s2, size_t pos, size_t len)
{ return (ustr_srch_subustr_rev(&s1->s, &s2->s, pos, len)); }

USTR_CONF_EI_PROTO size_t ustrp_srch_cstr_fwd(const struct Ustrp *,const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_srch_cstr_fwd(const struct Ustrp *s1, const char *cstr)
{ return (ustr_srch_buf_fwd(&s1->s, cstr, strlen(cstr))); }
USTR_CONF_EI_PROTO size_t ustrp_srch_cstr_rev(const struct Ustrp *,const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_srch_cstr_rev(const struct Ustrp *s1, const char *cstr)
{ return (ustr_srch_buf_rev(&s1->s, cstr, strlen(cstr))); }




#endif
