/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SUB_H
#error " Include ustr-sub.h before this file."
#endif

USTR_CONF_I_PROTO
int ustr_sub_undef(struct Ustr **ps1, size_t pos, size_t len)
{
  size_t clen;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (!len)
    return (USTR_TRUE);

  clen = ustr_assert_valid_subustr(*ps1, pos, 1);
  if (!clen)
    return (USTR_FALSE);
  --pos;
  
  if ((clen - pos) < len)
  { /* need to expand s1, it's basically like ustr_set() with an offset */
    if (!ustr_add_undef(ps1, len - (clen - pos)))
      return (USTR_FALSE);
  }
  else if (!ustr_sc_ensure_owner(ps1))
    return (USTR_FALSE);
  
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO
int ustr_sub_buf(struct Ustr **ps1, size_t pos, const void *buf, size_t len)
{
  if (!ustr_sub_undef(ps1, pos, len))
    return (USTR_FALSE);
  --pos;
  
  ustr__memcpy(*ps1, pos, buf, len);

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
{
  if (!ustr_assert_valid_subustr(s2, pos2, len2))
    return (USTR_FALSE);
  --pos2;
  
  return (ustr_sub_buf(ps1, pos1, ustr_cstr(s2) + pos2, len2));
}

USTR_CONF_I_PROTO
int ustr_sub_rep_chr(struct Ustr **ps1, size_t pos, char chr, size_t len)
{
  if (!ustr_sub_undef(ps1, pos, len))
    return (USTR_FALSE);
  --pos;
  
  ustr__memset(*ps1, pos, chr, len);

  return (USTR_FALSE);
}

USTR_CONF_I_PROTO
int ustr_sc_sub_undef(struct Ustr **ps1, size_t pos,size_t olen, size_t len)
{
  struct Ustr *s1 = *ps1;
  size_t clen;
  struct Ustr *ret = USTR_NULL;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));  

  if (len < olen)
    return (ustr_del_subustr(ps1, pos + len, olen - len));

  if (len == olen)
    return (ustr_sc_ensure_owner(ps1));
    
  clen = ustr_assert_valid_subustr(s1, pos, olen);
  if (!clen)
    return (USTR_FALSE);
  
  /* move to using ustr_ins_undef*() ? */
  if (!(ret = ustr_dupx_empty(USTR__DUPX_FROM(s1))))
    return (USTR_FALSE);
  
  ustr_add_subustr(&ret, s1, 1, pos - 1);
  ustr_add_undef(&ret, len);
  ustr_add_subustr(&ret, s1, pos + olen, clen - (pos + olen) + 1);
  
  if (ustr_enomem(ret))
  {
    ustr_free(ret);
    return (USTR_FALSE);
  }
  
  ustr_sc_free2(ps1, ret);
  
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO int ustr_sc_sub_buf(struct Ustr **ps1, size_t pos,size_t olen,
                                      const void *buf, size_t len)
{
  if (!ustr_sc_sub_undef(ps1, pos, olen, len))
    return (USTR_FALSE);

  return (ustr_sub_buf(ps1, pos, buf, len));
}

USTR_CONF_I_PROTO
int ustr_sc_sub(struct Ustr **ps1,size_t pos,size_t olen, const struct Ustr *s2)
{
  return (ustr_sc_sub_buf(ps1, pos, olen, ustr_cstr(s2), ustr_len(s2)));
}

USTR_CONF_I_PROTO
int ustr_sc_sub_subustr(struct Ustr **ps1, size_t pos1, size_t len1,
                        const struct Ustr *s2, size_t pos2, size_t len2)
{
  if (!ustr_assert_valid_subustr(s2, pos2, len2))
    return (USTR_FALSE);
  --pos2;
  
  return (ustr_sc_sub_buf(ps1, pos1, len1, ustr_cstr(s2) + pos2, len2));
}

USTR_CONF_I_PROTO
int ustr_sc_sub_rep_chr(struct Ustr **ps1, size_t pos, size_t olen,
                        char chr, size_t len)
{
  if (!ustr_sc_sub_undef(ps1, pos, olen, len))
    return (USTR_FALSE);

  return (ustr_sub_rep_chr(ps1, pos, chr, len));
}

USTR_CONF_I_PROTO
size_t ustr_sc_replace_buf(struct Ustr **ps1, const void *optr, size_t olen,
                           const void *nptr, size_t nlen, size_t lim)
{
  size_t num  = 0;
  size_t tlen = 0;
  size_t pos  = 0;
  struct Ustr *ret = USTR_NULL;
  const char *rptr;
  size_t lpos = 0;
  size_t roff = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (nlen == olen)
  { /* "fast path" ... as we can't fail after we are the owner(). In theory
     * we can do nlen <= olen, but then we'll spend a lot of time calling
     * memmove(). Which might be painful, so let that fall through to dupx(). */
    if (!ustr_sc_ensure_owner(ps1))
      goto fail_alloc;
    
    while ((pos = ustr_srch_buf_fwd(*ps1, pos, optr, olen)))
    {
      ustr_sub_buf(ps1, pos, nptr, nlen);
      pos += nlen - 1;
      
      ++num;
      if (lim && (num == lim))
        break;
    }
    
    return (num);
  }
  
  /* pre-calc size, and do single alloc and then memcpy.
   * Using dup()/ustr_sc_sub() is much simpler but very slow
   * for large strings. */
  tlen = ustr_len(*ps1);
  while ((pos = ustr_srch_buf_fwd(*ps1, pos, optr, olen)))
  {
    size_t tmp = tlen + (nlen - olen); /* can go up or down */
    
    pos += olen - 1;

    if (tmp < tlen)
      return (0);
    tlen = tmp;

    ++num;
    if (lim && (num == lim))
      break;
  }

  if (!(ret = ustr_dupx_undef(USTR__DUPX_FROM(*ps1), tlen)))
    goto fail_alloc;

  if (!tlen) /* minor speed hack */
    goto done_slow_sub;
  
  rptr = ustr_cstr(*ps1);
  lpos = 1;
  roff = 0;
  pos  = 0;
  num  = 0;
  while ((pos = ustr_srch_buf_fwd(*ps1, pos, optr, olen)))
  {
    const char *tptr = rptr + roff;
    size_t blen = pos - lpos;
    
    ustr_sub_buf(&ret, lpos, tptr, blen);
    ustr_sub_buf(&ret,  pos, nptr, nlen);

    lpos = pos + nlen;
    roff = pos + olen - 1;

    ++num;
    if (lim && (num == lim))
      break;
  }
  ustr_sub_buf(&ret, lpos, rptr + roff, ustr_len(*ps1) - (lpos - 1));
  
 done_slow_sub:
  ustr_sc_free2(ps1, ret);
  
  return (num);

 fail_alloc:
  ustr_setf_enomem_err(*ps1);
  return (0);
}

USTR_CONF_I_PROTO
size_t ustr_sc_replace(struct Ustr **ps1, const struct Ustr *srch,
                       const struct Ustr *repl, size_t lim)
{
  USTR_ASSERT(ustr_assert_valid(srch));
  USTR_ASSERT(ustr_assert_valid(repl));

  return (ustr_sc_replace_buf(ps1, ustr_cstr(srch), ustr_len(srch),
                              ustr_cstr(repl), ustr_len(repl), lim));
}
