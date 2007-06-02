/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_CMP_H
#define USTR_CMP_H 1

#ifndef USTR_MAIN_H
#error " You should have already included ustr-main.h, or just include ustr.h."
#endif

/* normal strcmp() like, deals with embeded NILs */
USTR_CONF_E_PROTO int ustr_cmp_buf(const struct Ustr *, const void *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_E_PROTO int ustr_cmp(const struct Ustr *, const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_cmp_subustr(const struct Ustr *,
                                       const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_cmp_cstr(const struct Ustr *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

/* faster compare, sorts in a very non-human way */
USTR_CONF_EI_PROTO int ustr_cmp_fast_buf(const struct Ustr*, const void*,size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_EI_PROTO int ustr_cmp_fast(const struct Ustr *, const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO int ustr_cmp_fast_subustr(const struct Ustr *,
                                            const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_cmp_fast_cstr(const struct Ustr *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

/* EQ functions to make code readable ... also faster due to ustr_cmp_fast() */
USTR_CONF_EI_PROTO int ustr_cmp_eq(const struct Ustr *, const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_cmp_buf_eq(const struct Ustr *, const void *,size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_cmp_subustr_eq(const struct Ustr *,
                                           const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO int ustr_cmp_cstr_eq(const struct Ustr *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_II_PROTO
int ustr_cmp_fast_buf(const struct Ustr *s1, const void *buf, size_t len2)
{
  size_t len1 = 0;

  USTR_ASSERT(ustr_assert_valid(s1) && buf);

  len1 = ustr_len(s1);
  if (len1 != len2)
    return (len1 - len2);

  return (memcmp(ustr_cstr(s1), buf, len1));
}

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-cmp-code.h"
#endif

USTR_CONF_II_PROTO int ustr_cmp_fast(const struct Ustr *s1,const struct Ustr*s2)
{
  USTR_ASSERT(ustr_assert_valid(s1) && ustr_assert_valid(s2));

  if (s1 == s2)
    return (0);

  return (ustr_cmp_fast_buf(s1, ustr_cstr(s2), ustr_len(s2)));
}

USTR_CONF_II_PROTO int ustr_cmp_cstr(const struct Ustr *s1, const char *s2)
{ return (ustr_cmp_buf(s1, s2, strlen(s2))); }
USTR_CONF_II_PROTO int ustr_cmp_fast_cstr(const struct Ustr *s1, const char *s2)
{ return (ustr_cmp_fast_buf(s1, s2, strlen(s2))); }

USTR_CONF_II_PROTO int ustr_cmp_eq(const struct Ustr *s1, const struct Ustr *s2)
{ return (!ustr_cmp_fast(s1, s2)); }
USTR_CONF_II_PROTO
int ustr_cmp_buf_eq(const struct Ustr *s1, const void *buf, size_t len)
{ return (!ustr_cmp_fast_buf(s1, buf, len)); }
USTR_CONF_II_PROTO int ustr_cmp_subustr_eq(const struct Ustr *s1,
                                           const struct Ustr *s2,
                                           size_t pos, size_t len)
{ return (!ustr_cmp_fast_subustr(s1, s2, pos, len)); }
USTR_CONF_II_PROTO int ustr_cmp_cstr_eq(const struct Ustr *s1, const char *cstr)
{ return (!ustr_cmp_fast_buf(s1, cstr, strlen(cstr))); }

/* ---------------- pool wrapper APIs ---------------- */

/* normal strcmp() like, deals with embeded NILs */
USTR_CONF_EI_PROTO
int ustrp_cmp_buf(const struct Ustrp *, const void *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_buf(const struct Ustrp *s1, const void *s2, size_t len)
{ return (ustr_cmp_buf(&s1->s, s2, len)); }

USTR_CONF_EI_PROTO int ustrp_cmp(const struct Ustrp *, const struct Ustrp *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO int ustrp_cmp(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (ustr_cmp(&s1->s, &s2->s)); }
    
USTR_CONF_EI_PROTO int ustrp_cmp_subustrp(const struct Ustrp *,
                                          const struct Ustrp *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_subustrp(const struct Ustrp *s1,
                       const struct Ustrp *s2, size_t pos, size_t len)
{ return (ustr_cmp_subustr(&s1->s, &s2->s, pos, len)); }

USTR_CONF_EI_PROTO int ustrp_cmp_cstr(const struct Ustrp *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO int ustrp_cmp_cstr(const struct Ustrp *s1, const char *s2)
{ return (ustrp_cmp_buf(s1, s2, strlen(s2))); }

/* faster compare, sorts in a very non-human way */
USTR_CONF_EI_PROTO
int ustrp_cmp_fast_buf(const struct Ustrp *, const void *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_fast_buf(const struct Ustrp *s1, const void *s2, size_t len)
{ return (ustr_cmp_fast_buf(&s1->s, s2, len)); }

USTR_CONF_EI_PROTO int ustrp_cmp_fast(const struct Ustrp *, const struct Ustrp*)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_fast(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (ustr_cmp_fast(&s1->s, &s2->s)); }
    
USTR_CONF_EI_PROTO
int ustrp_cmp_fast_subustrp(const struct Ustrp *,
                            const struct Ustrp *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_fast_subustrp(const struct Ustrp *s1,
                            const struct Ustrp *s2, size_t pos, size_t len)
{ return (ustr_cmp_fast_subustr(&s1->s, &s2->s, pos, len)); }

USTR_CONF_EI_PROTO int ustrp_cmp_fast_cstr(const struct Ustrp *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_fast_cstr(const struct Ustrp *s1, const char *s2)
{ return (ustrp_cmp_fast_buf(s1, s2, strlen(s2))); }

/* EQ functions to make code readable ... also faster due to ustr_cmp_fast() */
USTR_CONF_EI_PROTO int ustrp_cmp_eq(const struct Ustrp *, const struct Ustrp *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_eq(const struct Ustrp *s1, const struct Ustrp *s2)
{ return (!ustrp_cmp_fast(s1, s2)); }
USTR_CONF_EI_PROTO int ustrp_cmp_buf_eq(const struct Ustrp *,const void*,size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_buf_eq(const struct Ustrp *s1, const void *buf, size_t len)
{ return (!ustrp_cmp_fast_buf(s1, buf, len)); }
USTR_CONF_EI_PROTO int ustrp_cmp_subustrp_eq(const struct Ustrp *,
                                           const struct Ustrp *, size_t, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO int ustrp_cmp_subustrp_eq(const struct Ustrp *s1,
                                           const struct Ustrp *s2,
                                           size_t pos, size_t len)
{ return (!ustrp_cmp_fast_subustrp(s1, s2, pos, len)); }
USTR_CONF_EI_PROTO int ustrp_cmp_cstr_eq(const struct Ustrp *, const char *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_II_PROTO
int ustrp_cmp_cstr_eq(const struct Ustrp *s1, const char *cstr)
{ return (!ustrp_cmp_fast_buf(s1, cstr, strlen(cstr))); }



#endif
