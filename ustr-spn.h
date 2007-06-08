/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_SPN_H
#define USTR_SPN_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

/* normal strspn() like, deals with embeded NILs */
USTR_CONF_E_PROTO size_t ustr_spn_chr_fwd(const struct Ustr *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO size_t ustr_spn_chr_rev(const struct Ustr *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
size_t ustr_spn_chrs_fwd(const struct Ustr *, const char *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
size_t ustr_spn_chrs_rev(const struct Ustr *, const char *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO
size_t ustr_spn_fwd(const struct Ustr *, const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO
size_t ustr_spn_rev(const struct Ustr *, const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO size_t ustr_spn_cstr_fwd(const struct Ustr*,const char*)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO size_t ustr_spn_cstr_rev(const struct Ustr*,const char*)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO size_t ustr_cspn_chr_fwd(const struct Ustr *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO size_t ustr_cspn_chr_rev(const struct Ustr *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
size_t ustr_cspn_chrs_fwd(const struct Ustr *, const char *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
size_t ustr_cspn_chrs_rev(const struct Ustr *, const char *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO
size_t ustr_cspn_fwd(const struct Ustr *, const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO
size_t ustr_cspn_rev(const struct Ustr *, const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO
size_t ustr_cspn_cstr_fwd(const struct Ustr *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO
size_t ustr_cspn_cstr_rev(const struct Ustr *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-spn-code.h"
#endif

USTR_CONF_II_PROTO
size_t ustr_spn_cstr_fwd(const struct Ustr *s1, const char *cstr)
{ return (ustr_spn_chrs_fwd(s1, cstr, strlen(cstr))); }
USTR_CONF_II_PROTO
size_t ustr_spn_cstr_rev(const struct Ustr *s1, const char *cstr)
{ return (ustr_spn_chrs_rev(s1, cstr, strlen(cstr))); }

USTR_CONF_II_PROTO
size_t ustr_cspn_cstr_fwd(const struct Ustr *s1, const char *cstr)
{ return (ustr_cspn_chrs_fwd(s1, cstr, strlen(cstr))); }
USTR_CONF_II_PROTO
size_t ustr_cspn_cstr_rev(const struct Ustr *s1, const char *cstr)
{ return (ustr_cspn_chrs_rev(s1, cstr, strlen(cstr))); }

USTR_CONF_II_PROTO
size_t ustr_spn_fwd(const struct Ustr *s1, const struct Ustr *s2)
{ return (ustr_spn_chrs_fwd(s1, ustr_cstr(s2), ustr_len(s2))); }
USTR_CONF_II_PROTO
size_t ustr_spn_rev(const struct Ustr *s1, const struct Ustr *s2)
{ return (ustr_spn_chrs_rev(s1, ustr_cstr(s2), ustr_len(s2))); }

USTR_CONF_II_PROTO
size_t ustr_cspn_fwd(const struct Ustr *s1, const struct Ustr *s2)
{ return (ustr_cspn_chrs_fwd(s1, ustr_cstr(s2), ustr_len(s2))); }
USTR_CONF_II_PROTO
size_t ustr_cspn_rev(const struct Ustr *s1, const struct Ustr *s2)
{ return (ustr_cspn_chrs_rev(s1, ustr_cstr(s2), ustr_len(s2))); }

/* ---------------- pool wrapper APIs ---------------- */

/* normal strspn() like, deals with embeded NILs */
USTR_CONF_EI_PROTO size_t ustrp_spn_chr_fwd(const struct Ustrp *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO size_t ustrp_spn_chr_fwd(const struct Ustrp *s1, char chr)
{ return (ustr_spn_chr_fwd(&s1->s, chr)); }
USTR_CONF_EI_PROTO size_t ustrp_spn_chr_rev(const struct Ustrp *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO size_t ustrp_spn_chr_rev(const struct Ustrp *s1, char chr)
{ return (ustr_spn_chr_rev(&s1->s, chr)); }
USTR_CONF_EI_PROTO
size_t ustrp_spn_chrs_fwd(const struct Ustrp *, const char *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_spn_chrs_fwd(const struct Ustrp *s1, const char *chrs, size_t len)
{ return (ustr_spn_chrs_fwd(&s1->s, chrs, len)); }
USTR_CONF_EI_PROTO
size_t ustrp_spn_chrs_rev(const struct Ustrp *, const char *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_spn_chrs_rev(const struct Ustrp *s1, const char *chrs, size_t len)
{ return (ustr_spn_chrs_rev(&s1->s, chrs, len)); }
USTR_CONF_EI_PROTO
size_t ustrp_spn_fwd(const struct Ustrp *, const struct Ustrp *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_spn_fwd(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (ustr_spn_fwd(&s1->s, &s2->s)); }
USTR_CONF_EI_PROTO
size_t ustrp_spn_rev(const struct Ustrp *, const struct Ustrp *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_spn_rev(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (ustr_spn_rev(&s1->s, &s2->s)); }
USTR_CONF_EI_PROTO
size_t ustrp_spn_cstr_fwd(const struct Ustrp *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_spn_cstr_fwd(const struct Ustrp *s1, const char *chrs)
{ return (ustrp_spn_chrs_fwd(s1, chrs, strlen(chrs))); }
USTR_CONF_EI_PROTO
size_t ustrp_spn_cstr_rev(const struct Ustrp *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_spn_cstr_rev(const struct Ustrp *s1, const char *chrs)
{ return (ustrp_spn_chrs_rev(s1, chrs, strlen(chrs))); }

USTR_CONF_EI_PROTO size_t ustrp_cspn_chr_fwd(const struct Ustrp *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO size_t ustrp_cspn_chr_fwd(const struct Ustrp *s1, char chr)
{ return (ustr_cspn_chr_fwd(&s1->s, chr)); }
USTR_CONF_EI_PROTO size_t ustrp_cspn_chr_rev(const struct Ustrp *, char)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO size_t ustrp_cspn_chr_rev(const struct Ustrp *s1, char chr)
{ return (ustr_cspn_chr_rev(&s1->s, chr)); }
USTR_CONF_EI_PROTO
size_t ustrp_cspn_chrs_fwd(const struct Ustrp *, const char *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_cspn_chrs_fwd(const struct Ustrp *s1, const char *chrs, size_t len)
{ return (ustr_cspn_chrs_fwd(&s1->s, chrs, len)); }
USTR_CONF_EI_PROTO
size_t ustrp_cspn_chrs_rev(const struct Ustrp *, const char *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_cspn_chrs_rev(const struct Ustrp *s1, const char *chrs, size_t len)
{ return (ustr_cspn_chrs_rev(&s1->s, chrs, len)); }
USTR_CONF_EI_PROTO
size_t ustrp_cspn_fwd(const struct Ustrp *, const struct Ustrp *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_cspn_fwd(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (ustr_cspn_fwd(&s1->s, &s2->s)); }
USTR_CONF_EI_PROTO
size_t ustrp_cspn_rev(const struct Ustrp *, const struct Ustrp *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_cspn_rev(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (ustr_cspn_rev(&s1->s, &s2->s)); }
USTR_CONF_EI_PROTO
size_t ustrp_cspn_cstr_fwd(const struct Ustrp *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_cspn_cstr_fwd(const struct Ustrp *s1, const char *chrs)
{ return (ustrp_cspn_chrs_fwd(s1, chrs, strlen(chrs))); }
USTR_CONF_EI_PROTO
size_t ustrp_cspn_cstr_rev(const struct Ustrp *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
size_t ustrp_cspn_cstr_rev(const struct Ustrp *s1, const char *chrs)
{ return (ustrp_cspn_chrs_rev(s1, chrs, strlen(chrs))); }

#endif
