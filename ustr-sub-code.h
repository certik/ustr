#ifndef USTR_SUB_H
#error " Include ustr-sub.h before this file."
#endif

USTR_CONF_I_PROTO
int ustr_sub_buf(struct Ustr **ps1, size_t pos, const void *buf, size_t blen)
{
  size_t clen;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (!blen)
    return (USTR_TRUE);

  clen = ustr_assert_valid_subustr(*ps1, pos, 1);
  if (!clen)
    return (USTR_FALSE);
  --pos;
  
  if ((clen - pos) < blen)
  { /* need to expand s1, it's basically like ustr_set() with an offset */
    if (!ustr_add_undef(ps1, blen - (clen - pos)))
      return (USTR_FALSE);
  }
  else if (!ustr_sc_ensure_owner(ps1))
    return (USTR_FALSE);
  
  ustr__memcpy(*ps1, pos, buf, blen);

  return (USTR_TRUE);
}

USTR_CONF_I_PROTO
int ustr_sub(struct Ustr **s1, size_t pos, const struct Ustr *s2)
{ /* opts needed? */
  return (ustr_sub_buf(s1, pos, ustr_cstr(s2), ustr_len(s2)));
}

USTR_CONF_I_PROTO
int ustr_sub_subustr(struct Ustr **ps1, size_t pos1,
                     const struct Ustr *s2, size_t pos2, size_t len2)
{ /* opts needed? */
  if (!ustr_assert_valid_subustr(*ps1, pos2, len2))
    return (USTR_FALSE);
  --pos2;
  
  return (ustr_sub_buf(ps1, pos1, ustr_cstr(s2) + pos2, len2));
}

USTR_CONF_I_PROTO int ustr_sc_sub_buf(struct Ustr **ps1, size_t pos,size_t olen,
                                      const void *buf, size_t blen)
{
  struct Ustr *s1 = *ps1;
  size_t clen;
  struct Ustr *ret = USTR_NULL;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));  

  if (blen <= olen)
  {
    if (!ustr_sc_ensure_owner(ps1))
      return (USTR_FALSE);
    
    ustr_sub_buf(ps1, pos, buf, blen);
    if (blen < olen)
      ustr_del_subustr(ps1, pos + blen, olen - blen);
    return (USTR_TRUE);
  }
  
  clen = ustr_assert_valid_subustr(s1, pos, olen);
  if (!clen)
    return (USTR_FALSE);
  
  /* FIXME: slow, esp. when used for ustr_sc_replace()
     ustr_ins_*() */
  if (!(ret = ustr_dupx_empty(USTR__DUPX_FROM(s1))))
    return (USTR_FALSE);
  
  ustr_add_subustr(&ret, s1, 1, pos - 1);
  ustr_add_buf(&ret, buf, blen);
  ustr_add_subustr(&ret, s1, pos + olen, clen - (pos + olen) + 1);
  
  if (ustr_enomem(ret))
  {
    ustr_free(ret);
    return (USTR_FALSE);
  }
  
  ustr_sc_free2(ps1, ret);
  
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO
int ustr_sc_sub(struct Ustr **ps1,size_t pos,size_t olen, const struct Ustr *s2)
{
  return (ustr_sc_sub_buf(ps1, pos, olen, ustr_cstr(s2), ustr_len(s2)));
}

USTR_CONF_I_PROTO
size_t ustr_sc_replace(struct Ustr **ps1, const struct Ustr *srch,
                       const struct Ustr *repl, size_t lim)
{
  size_t olen = ustr_len(srch);
  size_t nlen = ustr_len(repl);
  size_t num = 0;
  size_t pos = 0;
  struct Ustr *ret = USTR_NULL;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  USTR_ASSERT(ustr_assert_valid(srch));
  USTR_ASSERT(ustr_assert_valid(repl));

  if (nlen <= olen)
  { /* "fast path" ... as we can't fail after we are the owner() */
    if (!ustr_sc_ensure_owner(ps1))
      return (0);
    
    while ((pos = ustr_srch_fwd(*ps1, pos, srch)))
    {
      ustr_sc_sub(ps1, pos, olen, repl);
      pos += nlen - 1;
      
      ++num;
      if (lim && (num == lim))
        break;
    }
    
    return (num);
  }

  /* Other option is to pre-calc size, and do single alloc. Using dup is much
   * slower but simpler.

    while ((pos = ustr_srch_fwd(*ps1, pos, srch)))
    {
      pos += olen - 1;
      ++num;
      if (lim && (num == lim))
        break;
    }

    if (!(ret = ustr_dupx_undef(USTR__DUPX_FROM(*ps1))))
      fail;

    while (srch)
    { add_before; add_repl; }
    add_after;
  */
  if (!(ret = ustr_dup(*ps1)))
    return (0);

  while ((pos = ustr_srch_fwd(ret, pos, srch)))
  {
    if (!ustr_sc_sub(ps1, pos, olen, repl))
      goto fail_sub;
    pos += nlen - 1;
    
    ++num;
    if (lim && (num == lim))
      break;
  }

  ustr_sc_free2(ps1, ret);
  
  return (num);

 fail_sub:
  ustr_free(ret);
  return (0);
}
