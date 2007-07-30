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

USTR_CONF_i_PROTO
int ustrp__sc_ensure_owner(struct Ustr_pool *p, struct Ustr **ps1)
{
  struct Ustr *ret = USTR_NULL;
  size_t len = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));

  if (ustr_owner(*ps1))
    return (USTR_TRUE);

  len = ustr_len(*ps1);
  if (!len)
    ret = ustrp__dupx_empty(p, USTR__DUPX_FROM(*ps1));
  else
    ret = ustrp__dupx_buf(p,   USTR__DUPX_FROM(*ps1), ustr_cstr(*ps1), len);
  
  if (!ret)
    return (USTR_FALSE);
  
  ustrp__sc_free2(p, ps1, ret);

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_sc_ensure_owner(struct Ustr **ps1)
{ return (ustrp__sc_ensure_owner(0, ps1)); }
USTR_CONF_I_PROTO
int ustrp_sc_ensure_owner(struct Ustr_pool *p, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__sc_ensure_owner(p, &tmp);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO char *ustrp__sc_wstr(struct Ustr_pool *p, struct Ustr **ps1)
{
  if (!ustrp__sc_ensure_owner(p, ps1))
    return (USTR_FALSE);

  return (ustr_wstr(*ps1));
}
USTR_CONF_I_PROTO char *ustr_sc_wstr(struct Ustr **ps1)
{ return (ustrp__sc_wstr(0, ps1)); }
USTR_CONF_I_PROTO char *ustrp_sc_wstr(struct Ustr_pool *p, struct Ustrp **ps1)
{
  struct Ustr *tmp = &(*ps1)->s;
  char *ret = ustrp__sc_wstr(p, &tmp);
  *ps1 = USTRP(tmp);
  return (ret);
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
