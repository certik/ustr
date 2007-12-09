/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_SC_H
#error " Include ustr-sc.h before this file."
#endif

USTR_CONF_i_PROTO
void ustrp__sc_free_shared(struct Ustr_pool *p, struct Ustr **ps1)
{
  USTR_ASSERT(ps1);

  if (!*ps1)
    return;
  
  USTR_ASSERT(ustr_shared(*ps1));

  ustr_setf_owner(*ps1);
  ustrp__sc_free(p, ps1);
}
USTR_CONF_I_PROTO void ustr_sc_free_shared(struct Ustr **ps1)
{ ustrp__sc_free_shared(0, ps1); }
USTR_CONF_I_PROTO
void ustrp_sc_free_shared(struct Ustr_pool *p, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  ustrp__sc_free_shared(p, &tmp);
  *ps1 = USTRP(tmp);
}

USTR_CONF_i_PROTO
struct Ustr *ustrp__sc_dupx(struct Ustr_pool *p,
                            size_t sz, size_t rbytes, int exact, int emem,
                            struct Ustr **ps1)
{
  struct Ustr *ret = ustrp__dupx(p, sz, rbytes, exact, emem, *ps1);
  struct Ustr *tmp = USTR_NULL;

  if (!ret)
    return (USTR_NULL);

  if (!ustr__dupx_cmp_eq(sz, rbytes, exact, emem, USTR__DUPX_FROM(*ps1)))
    return (ret); /* different config. so just return */

  /* swap, we only _need_ to do this when ret != *ps1 ... but it doesn't matter
   * if we always do it. */
  tmp  = *ps1;
  *ps1 = ret;
  ret  = tmp;
  
  return (ret);
}
USTR_CONF_I_PROTO
struct Ustr *ustr_sc_dupx(size_t sz, size_t rbytes, int exact, int emem,
                          struct Ustr **ps1)
{ return (ustrp__sc_dupx(0, sz, rbytes, exact, emem, ps1)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_sc_dupx(struct Ustr_pool *p, size_t sz, size_t rbytes,
                            int exact, int emem, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  struct Ustr *ret = ustrp__sc_dupx(p, sz, rbytes, exact, emem, &tmp);
  *ps1 = USTRP(tmp);
  return (USTRP(ret));
}

USTR_CONF_i_PROTO
struct Ustr *ustrp__sc_dup(struct Ustr_pool *p, struct Ustr **ps1)
{
  struct Ustr *ret = ustrp__dup(p, *ps1);
  struct Ustr *tmp = USTR_NULL;
  
  if (!ret)
    return (USTR_NULL);

  /* swap, we only _need_ to do this when ret != *ps1 ... but it doesn't matter
   * if we always do it. */
  tmp  = *ps1;
  *ps1 = ret;
  ret  = tmp;
  
  return (ret);
}
USTR_CONF_I_PROTO struct Ustr *ustr_sc_dup(struct Ustr **ps1)
{ return (ustrp__sc_dup(0, ps1)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_sc_dup(struct Ustr_pool *p, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  struct Ustr *ret = ustrp__sc_dup(p, &tmp);
  *ps1 = USTRP(tmp);
  return (USTRP(ret));
}

USTR_CONF_i_PROTO void ustr__reverse(char *ptr, size_t pos, size_t len)
{
  size_t clen = len;

  --pos;
  while (len > (clen / 2))
  {
    const size_t boff = pos + (clen - len);
    const size_t eoff = pos + (len  - 1);
    char tmp = ptr[boff];
    
    ptr[boff] = ptr[eoff];
    ptr[eoff] = tmp;
    
    --len;
  }
}

USTR_CONF_i_PROTO int ustrp__sc_reverse(struct Ustr_pool *p, struct Ustr **ps1)
{
  if (!ustrp__sc_ensure_owner(p, ps1))
    return (USTR_FALSE);

  ustr__reverse(ustr_wstr(*ps1), 1, ustr_len(*ps1));  

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_sc_reverse(struct Ustr **ps1)
{ return (ustrp__sc_reverse(0, ps1)); }
USTR_CONF_I_PROTO int ustrp_sc_reverse(struct Ustr_pool *p, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__sc_reverse(p, &tmp);
  *ps1 = USTRP(tmp);
  return (ret);
}

#ifdef USTR_UTF8_H
USTR_CONF_i_PROTO
int ustrp__sc_utf8_reverse(struct Ustr_pool *p, struct Ustr **ps1)
{ /* UTF-8 reversing is like word order reversing. The simple way is to reverse
   * each "character", in place, and then reverse the entire string. */
  char *ptr;
  const unsigned char *beg;
  const unsigned char *scan;
  
  USTR_ASSERT(ps1 && ustrp__assert_valid(!!p, *ps1));
  
  if (!(ptr = ustrp__sc_wstr(p, ps1)))
    return (USTR_FALSE);

  scan = beg = (const unsigned char *)ptr;
  while (*scan)
  {
    const unsigned char *prev = scan;
    
    USTR_ASSERT(ustr_len(*ps1) > (size_t)(scan - beg));

    scan = ustr__utf8_next(scan);
    ustr__reverse(ptr, 1 + (prev - beg), (scan - prev));
  }
  
  ustr__reverse(ptr, 1, (scan - beg));

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_sc_utf8_reverse(struct Ustr **ps1)
{ return (ustrp__sc_utf8_reverse(0, ps1)); }
USTR_CONF_I_PROTO
int ustrp_sc_utf8_reverse(struct Ustr_pool *p, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__sc_utf8_reverse(p, &tmp);
  *ps1 = USTRP(tmp);
  return (ret);
}
#endif

USTR_CONF_i_PROTO int ustrp__sc_tolower(struct Ustr_pool *p, struct Ustr **ps1)
{
  size_t clen;
  size_t len;
  char *ptr;
  
  if (!(ptr = ustrp__sc_wstr(p, ps1)))
    return (USTR_FALSE);

  clen = len = ustr_len(*ps1);
  while (len)
  {
    if ((*ptr >= 0x41) && (*ptr <= 0x5a))
      *ptr ^= 0x20;
    ++ptr;
    --len;
  }

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_sc_tolower(struct Ustr **ps1)
{ return (ustrp__sc_tolower(0, ps1)); }
USTR_CONF_I_PROTO int ustrp_sc_tolower(struct Ustr_pool *p, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__sc_tolower(p, &tmp);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO int ustrp__sc_toupper(struct Ustr_pool *p, struct Ustr **ps1)
{
  size_t clen;
  size_t len;
  char *ptr;
  
  if (!(ptr = ustrp__sc_wstr(p, ps1)))
    return (USTR_FALSE);

  clen = len = ustr_len(*ps1);
  while (len)
  {
    if ((*ptr >= 0x61) && (*ptr <= 0x7a))
      *ptr ^= 0x20;
    ++ptr;
    --len;
  }

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_sc_toupper(struct Ustr **ps1)
{ return (ustrp__sc_toupper(0, ps1)); }
USTR_CONF_I_PROTO int ustrp_sc_toupper(struct Ustr_pool *p, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__sc_toupper(p, &tmp);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO
char *ustrp__sc_export_subustr(struct Ustr_pool *p,
                               const struct Ustr *s1, size_t pos,size_t len,
                               void *(*my_alloc)(size_t))
{
  char *ret = 0;

  USTR_ASSERT(my_alloc || p);
  
  if (!ustrp__assert_valid_subustr(!!p, s1, pos, len))
  {
    errno = USTR__EINVAL;
    return (ret);
  }
  --pos;

  if (my_alloc) /* Alloc ustrp_*() to use normal export too */
    ret = (*my_alloc)(len + 1);
  else
    ret = p->pool_sys_malloc(p, len + 1);
  
  if (!ret)
  {
    errno = ENOMEM;
    return (ret);
  }
  
  memcpy(ret, ustr_cstr(s1) + pos, len);
  ret[len] = 0;

  return (ret);
}

USTR_CONF_I_PROTO
char *ustr_sc_export_subustr(const struct Ustr *s1, size_t pos, size_t len,
                             void *(*my_alloc)(size_t))
{
  USTR_ASSERT(my_alloc);
  return (ustrp__sc_export_subustr(0, s1, pos, len, my_alloc));
}
USTR_CONF_I_PROTO
char *ustrp_sc_export_subustrp(struct Ustr_pool *p,
                               const struct Ustrp *s1, size_t pos,size_t len,
                               void *(*my_alloc)(size_t))
{ return (ustrp__sc_export_subustr(p, &s1->s, pos, len, my_alloc)); }

USTR_CONF_i_PROTO
int ustrp__sc_ltrim_chrs(struct Ustr_pool *p, struct Ustr **ps1,
                         const char *chrs, size_t len)
{
  return (ustrp__del_subustr(p, ps1, 1, ustr_spn_chrs_fwd(*ps1, 0, chrs, len)));
}
USTR_CONF_I_PROTO
int ustr_sc_ltrim_chrs(struct Ustr **ps1, const char *chrs, size_t len)
{ return (ustrp__sc_ltrim_chrs(0, ps1, chrs, len)); }
USTR_CONF_I_PROTO
int ustrp_sc_ltrim_chrs(struct Ustr_pool *p, struct Ustrp **ps1,
                        const char *chrs, size_t len)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__sc_ltrim_chrs(p, &tmp, chrs, len);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO
int ustrp__sc_rtrim_chrs(struct Ustr_pool *p, struct Ustr **ps1,
                         const char *chrs, size_t len)
{
  return (ustrp__del(p, ps1, ustr_spn_chrs_rev(*ps1, 0, chrs, len)));
}
USTR_CONF_I_PROTO
int ustr_sc_rtrim_chrs(struct Ustr **ps1, const char *chrs, size_t len)
{ return (ustrp__sc_rtrim_chrs(0, ps1, chrs, len)); }
USTR_CONF_I_PROTO
int ustrp_sc_rtrim_chrs(struct Ustr_pool *p, struct Ustrp **ps1,
                        const char *chrs, size_t len)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__sc_rtrim_chrs(p, &tmp, chrs, len);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO
int ustrp__sc_trim_chrs(struct Ustr_pool *p, struct Ustr **ps1,
                        const char *chrs, size_t len)
{
  size_t ltrim = ustr_spn_chrs_fwd(*ps1, 0, chrs, len);
  size_t rtrim = 0;
  size_t clen = ustr_len(*ps1);
  size_t nlen = 0;
  char *ptr;

  USTR_ASSERT(ps1 && ustrp__assert_valid(!!p, *ps1));
  
  if (ltrim == clen)
    return (ustrp__del(p, ps1, ltrim));

  rtrim = ustr_spn_chrs_rev(*ps1, 0, chrs, len);

  if (!ltrim && !rtrim)
    return (USTR_TRUE); /* minor speed hack */
  
  nlen = clen - (ltrim + rtrim);
  if (!ustr_owner(*ps1))
  {
    struct Ustr *ret = ustrp__dup_subustr(p, *ps1, 1 + ltrim, nlen);

    if (ret)
      ustrp__sc_free2(p, ps1, ret);
    
    return (!!ret);
  }
  
  ptr = ustr_wstr(*ps1);
  memmove(ptr, ptr + ltrim, nlen);

  return (ustrp__del(p, ps1, ltrim + rtrim));
}
USTR_CONF_I_PROTO
int ustr_sc_trim_chrs(struct Ustr **ps1, const char *chrs, size_t len)
{ return (ustrp__sc_trim_chrs(0, ps1, chrs, len)); }
USTR_CONF_I_PROTO
int ustrp_sc_trim_chrs(struct Ustr_pool *p, struct Ustrp **ps1,
                       const char *chrs, size_t len)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__sc_trim_chrs(p, &tmp, chrs, len);
  *ps1 = USTRP(tmp);
  return (ret);
}

