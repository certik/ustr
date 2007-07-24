/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_REPLACE_H
#error " Include ustr-replace.h before this file."
#endif

#ifdef USTR_SRCH_H
USTR_CONF_i_PROTO
size_t ustrp__replace_buf(struct Ustr_pool *p, struct Ustr **ps1,
                          const void *optr, size_t olen,
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
    if (!ustrp__sc_ensure_owner(p, ps1))
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
    pos += olen - 1;

    if (nlen < olen) /* can go up or down */
      tlen -= (olen - nlen);
    else
    {
      if (tlen > (tlen + (nlen - olen)))
        return (0);
      tlen += (nlen - olen);
    }

    ++num;
    if (lim && (num == lim))
      break;
  }

  if (!num) /* minor speed hack */
    return (0);
  
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
    size_t blen = pos - (roff + 1);
    
    pos  += olen - 1;
    USTR_ASSERT(pos == (roff + blen + olen));
    
    ustr_sub_buf(&ret, lpos, tptr, blen); lpos += blen;
    ustr_sub_buf(&ret, lpos, nptr, nlen); lpos += nlen;

    roff = pos;
    
    ++num;
    if (lim && (num == lim))
      break;
  }
  ustr_sub_buf(&ret, lpos, rptr + roff, ustr_len(*ps1) - roff);
  
 done_slow_sub:
  ustr_sc_free2(ps1, ret);
  
  return (num);

 fail_alloc:
  ustr_setf_enomem_err(*ps1);
  return (0);
}

USTR_CONF_I_PROTO
size_t ustr_replace_buf(struct Ustr **ps1, const void *optr, size_t olen,
                        const void *nptr, size_t nlen, size_t lim)
{ return (ustrp__replace_buf(0, ps1, optr, olen, nptr, nlen, lim)); }
USTR_CONF_I_PROTO
size_t ustrp_replace_buf(struct Ustr_pool *p, struct Ustrp **ps1,
                         const void *optr, size_t olen,
                         const void *nptr, size_t nlen, size_t lim)
{ return (ustrp__replace_buf(p, USTR__PPTR(ps1), optr,olen,nptr,nlen,lim)); }

USTR_CONF_i_PROTO
size_t ustrp__replace(struct Ustr_pool *p, struct Ustr **ps1,
                      const struct Ustr *srch,
                      const struct Ustr *repl, size_t lim)
{
  USTR_ASSERT(ustr_assert_valid(srch));
  USTR_ASSERT(ustr_assert_valid(repl));

  return (ustrp__replace_buf(p, ps1,
                             ustr_cstr(srch), ustr_len(srch),
                             ustr_cstr(repl), ustr_len(repl), lim));
}
USTR_CONF_I_PROTO
size_t ustr_replace(struct Ustr **ps1, const struct Ustr *srch,
                    const struct Ustr *repl, size_t lim)
{ return (ustrp__replace(0, ps1, srch, repl, lim)); }
USTR_CONF_I_PROTO
size_t ustrp_replace(struct Ustr_pool *p, struct Ustrp **ps1,
                     const struct Ustrp *srch,
                     const struct Ustrp *repl, size_t lim)
{ return (ustrp__replace(p, USTR__PPTR(ps1), &srch->s, &repl->s, lim)); }

USTR_CONF_I_PROTO
size_t ustr_replace_rep_chr(struct Ustr **ps1, char odata, size_t olen, 
                      char ndata, size_t nlen, size_t lim)
{
  if (odata == ndata && olen == nlen)
    return 0;
  if (!ps1 || !ustr_assert_valid(*ps1))
    return 0;
  size_t num_replaced = 0;
  size_t len = ustr_len(*ps1);
  size_t rlen = len; /* remaining length to scan */
  const char *beg = ustr_cstr(*ps1);
  const char *end = beg + len;
  const char *tmp = beg;
scan:
  while (tmp < end && (tmp = memchr(tmp, odata, rlen)))
  {
    /* shortest case -- the span would go off the end */
    if (olen > rlen) 
    {
      break;
    }
    /* start scanning at where the needle would end because we can
     * eliminate more characters from the search this way */
    const char *spn_beg = tmp;
    const char *spn_end = tmp + olen - 1;
    for (tmp = spn_end ; tmp > spn_beg ; --tmp)
    {
      if (*tmp != odata)
      {
        rlen = end - tmp;
        goto scan; /* this is a shortcut to continue the while loop */
      }
    }
    size_t pos = (size_t)(tmp - beg)+1; /* pos indexed at 1 */ 
    ustr_sc_sub_rep_chr(ps1, pos, olen, ndata, nlen);
    /* after modifying the internals of the ustr, we need to recalculate 
     * all of the pointers */
    beg = ustr_cstr(*ps1);
    len = ustr_len(*ps1);
    end = beg + len;
    tmp = beg + (pos - 1) + nlen;
    rlen = end - tmp;
    num_replaced++;
    if (lim && num_replaced == lim)
      break;
    }
  return num_replaced;
}

#endif
