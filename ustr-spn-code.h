/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_SPN_H
#error " You should have already included ustr-spn.h, or just include ustr.h."
#endif

#ifndef USTR_SRCH_H
#error " You should have already included ustr-srch.h, or just include ustr.h."
#endif

USTR_CONF_I_PROTO size_t ustr_spn_chr_fwd(const struct Ustr *s1, char chr)
{
  const char *ptr = 0;
  size_t len = 0;

  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = ustr_cstr(s1);
  len = ustr_len(s1);
  while (len)
  {
    if (*ptr != chr)
      return (ustr_len(s1) - len);

    ++ptr;
    --len;
  }

  return (ustr_len(s1));  
}

USTR_CONF_I_PROTO size_t ustr_spn_chr_rev(const struct Ustr *s1, char chr)
{
  const char *ptr = 0;
  size_t len = 0;

  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = ustr_cstr(s1);
  len = ustr_len(s1);
  ptr += len;
  while (len)
  {
    --ptr;
    
    if (*ptr != chr)
      return (ustr_len(s1) - len);

    --len;
  }

  return (ustr_len(s1));
}

USTR_CONF_I_PROTO
size_t ustr_spn_chrs_fwd(const struct Ustr *s1, const char *chrs, size_t clen)
{
  const char *ptr = 0;
  size_t len = 0;
  
  USTR_ASSERT(chrs);

  if (clen == 1)
    return (ustr_spn_chr_fwd(s1, *chrs));

  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = ustr_cstr(s1);
  len = ustr_len(s1);
  while (len)
  {
    if (!memchr(chrs, *ptr, clen))
      return (ustr_len(s1) - len);

    ++ptr;
    --len;
  }

  return (ustr_len(s1));  
}

USTR_CONF_I_PROTO
size_t ustr_spn_chrs_rev(const struct Ustr *s1, const char *chrs, size_t clen)
{
  const char *ptr = 0;
  size_t len = 0;
  
  USTR_ASSERT(chrs);

  if (clen == 1)
    return (ustr_spn_chr_rev(s1, *chrs));

  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = ustr_cstr(s1);
  len = ustr_len(s1);
  ptr += len;
  while (len)
  {
    --ptr;
    
    if (!memchr(chrs, *ptr, clen))
      return (ustr_len(s1) - len);

    --len;
  }

  return (ustr_len(s1));
}

USTR_CONF_I_PROTO size_t ustr_cspn_chr_fwd(const struct Ustr *s1, char chr)
{
  size_t f_pos = ustr_srch_chr_fwd(s1, chr);

  if (!f_pos)
    return (ustr_len(s1));
    
  return (f_pos - 1);
}

USTR_CONF_I_PROTO size_t ustr_cspn_chr_rev(const struct Ustr *s1, char chr)
{
  size_t f_pos = ustr_srch_chr_rev(s1, chr);

  if (!f_pos)
    return (ustr_len(s1));
    
  return (ustr_len(s1) - f_pos);
}

USTR_CONF_I_PROTO
size_t ustr_cspn_chrs_fwd(const struct Ustr *s1, const char *chrs, size_t clen)
{
  const char *ptr = 0;
  size_t len = 0;
  
  USTR_ASSERT(chrs);

  if (clen == 1)
    return (ustr_cspn_chr_fwd(s1, *chrs));

  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = ustr_cstr(s1);
  len = ustr_len(s1);
  while (len)
  {
    if (memchr(chrs, *ptr, clen))
      return (ustr_len(s1) - len);

    ++ptr;
    --len;
  }

  return (ustr_len(s1));
}

USTR_CONF_I_PROTO
size_t ustr_cspn_chrs_rev(const struct Ustr *s1, const char *chrs, size_t clen)
{
  const char *ptr = 0;
  size_t len = 0;
  
  USTR_ASSERT(chrs);

  if (clen == 1)
    return (ustr_cspn_chr_rev(s1, *chrs));

  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = ustr_cstr(s1);
  len = ustr_len(s1);
  ptr += len;
  while (len)
  {
    --ptr;
    
    if (memchr(chrs, *ptr, clen))
      return (ustr_len(s1) - len);

    --len;
  }

  return (ustr_len(s1));
}

#ifdef USTR_UTF8_H
USTR_CONF_I_PROTO size_t ustr_utf8_spn_chrs_fwd(const struct Ustr *s1,
                                                const char *pchrs, size_t clen)
{
  const unsigned char *chrs = (const unsigned char *)pchrs;
  const unsigned char *ptr = 0;
  size_t ret = 0;
  
  USTR_ASSERT(chrs);
  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = (const unsigned char *)ustr_cstr(s1);
  while (*ptr)
  {
    const unsigned char *bptr = ptr;

    ptr = ustr__utf8_nxt(ptr + 1);
    if (!USTR__SYS_MEMMEM(chrs, clen, bptr, ptr - bptr))
      break;
    
    ++ret;
  }

  return (ret);
}

USTR_CONF_I_PROTO size_t ustr_utf8_spn_chrs_rev(const struct Ustr *s1,
                                                const char *chrs, size_t clen)
{
  const unsigned char *chrs = (const unsigned char *)pchrs;
  const unsigned char *ptr = 0;
  size_t len = 0;
  size_t ret = 0;
  
  USTR_ASSERT(chrs);
  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = (const unsigned char *)ustr_cstr(s1);
  len = ustr_len(s1);
  while (len)
  {
    const unsigned char *eptr = ptr + len;
    const unsigned char *bptr = ustr__utf8_beg(eptr - 1, len);

    if (!USTR__SYS_MEMMEM(chrs, clen, bptr, eptr - bptr))
      break;
    
    ++ret;
    len -= (eptr - bptr);
  }

  return (ret);
}

USTR_CONF_I_PROTO size_t ustr_utf8_cspn_chrs_fwd(const struct Ustr *s1,
                                                 const char *chrs, size_t clen)
{
  const unsigned char *chrs = (const unsigned char *)pchrs;
  const unsigned char *ptr = 0;
  size_t ret = 0;
  
  USTR_ASSERT(chrs);
  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = (const unsigned char *)ustr_cstr(s1);
  while (*ptr)
  {
    const unsigned char *bptr = ptr;

    ptr = ustr__utf8_nxt(ptr + 1);
    if (USTR__SYS_MEMMEM(chrs, clen, bptr, ptr - bptr))
      break;
    
    ++ret;
  }

  return (ret);
}

USTR_CONF_I_PROTO size_t ustr_utf8_cspn_chrs_rev(const struct Ustr *s1,
                                                 const char *chrs, size_t clen)
{
  const unsigned char *chrs = (const unsigned char *)pchrs;
  const unsigned char *ptr = 0;
  size_t len = 0;
  size_t ret = 0;
  
  USTR_ASSERT(chrs);
  USTR_ASSERT(ustr_assert_valid(s1));

  ptr = (const unsigned char *)ustr_cstr(s1);
  len = ustr_len(s1);
  while (len)
  {
    const unsigned char *eptr = ptr + len;
    const unsigned char *bptr = ustr__utf8_beg(eptr - 1, len);

    if (USTR__SYS_MEMMEM(chrs, clen, bptr, eptr - bptr))
      break;
    
    ++ret;
    len -= (eptr - bptr);
  }

  return (ret);
}
#endif
