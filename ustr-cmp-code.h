/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_CMP_H
#error " You should have already included ustr-cmp.h, or just include ustr.h."
#endif

USTR_CONF_I_PROTO
int ustr_cmp_buf(const struct Ustr *s1, const void *buf, size_t len2)
{
  size_t len1 = 0;
  size_t lenm = 0;
  int    ret = 0;
  size_t def = 0;
  
  USTR_ASSERT(ustr_assert_valid(s1) && buf);

  len1 = ustr_len(s1);
  if (len1 == len2)
    return (memcmp(ustr_cstr(s1), buf, len1));
  
  if (len1 > len2)
  {
    lenm = len2;
    def  = 1;
  }
  if (len2 > len1)
  {
    lenm = len1;
    def  = -1;
  }

  if (lenm && (ret = memcmp(ustr_cstr(s1), buf, lenm)))
    return (ret);
  
  return (def);
}

USTR_CONF_I_PROTO int ustr_cmp(const struct Ustr *s1, const struct Ustr *s2)
{
  USTR_ASSERT(ustr_assert_valid(s1) && ustr_assert_valid(s2));
  if (s1 == s2)
    return (0);
  return (ustr_cmp_buf(s1, ustr_cstr(s2), ustr_len(s2)));
}

USTR_CONF_I_PROTO
int ustr_cmp_subustr(const struct Ustr *s1,
                     const struct Ustr *s2, size_t pos, size_t len)
{
  USTR_ASSERT(ustr_assert_valid(s1));

  if (!ustr__valid_subustr(s2, pos, len))
    return (ustr_cmp(s1, USTR("")));

  return (ustr_cmp_buf(s1, ustr_cstr(s2) + --pos, len));
}

USTR_CONF_I_PROTO
int ustr_cmp_fast_subustr(const struct Ustr *s1,
                          const struct Ustr *s2, size_t pos, size_t len)
{
  USTR_ASSERT(ustr_assert_valid(s1));

  if (!ustr__valid_subustr(s2, pos, len))
    return (ustr_cmp_fast(s1, USTR("")));

  return (ustr_cmp_fast_buf(s1, ustr_cstr(s2) + pos - 1, len));
}

