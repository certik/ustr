/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SUB_H
#error " Include ustr-sub.h before this file."
#endif

USTR_CONF_i_PROTO int ustrp__sub_undef(struct Ustr_pool *p, struct Ustr **ps1,
                                       size_t pos, size_t len)
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
    if (!ustrp__add_undef(p, ps1, len - (clen - pos)))
      return (USTR_FALSE);
  }
  else if (!ustrp__sc_ensure_owner(p, ps1))
    return (USTR_FALSE);
  
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO
int ustr_sub_undef(struct Ustr **ps1, size_t pos, size_t len)
{ return (ustrp__sub_undef(0, ps1, pos, len)); }
USTR_CONF_I_PROTO int ustrp_sub_undef(struct Ustr_pool *p, struct Ustrp **ps1,
                                      size_t pos, size_t len)
{ return (ustrp__sub_undef(p, USTR__PPTR(ps1), pos, len)); }

USTR_CONF_i_PROTO int ustrp__sub_buf(struct Ustr_pool *p, struct Ustr **ps1,
                                     size_t pos, const void *buf, size_t len)
{
  if (!ustrp__sub_undef(p, ps1, pos, len))
    return (USTR_FALSE);
  --pos;
  
  ustr__memcpy(*ps1, pos, buf, len);

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO
int ustr_sub_buf(struct Ustr **ps1, size_t pos, const void *buf, size_t len)
{ return (ustrp__sub_buf(0, ps1, pos, buf, len)); }
USTR_CONF_I_PROTO int ustrp_sub_buf(struct Ustr_pool *p, struct Ustrp **ps1,
                                    size_t pos, const void *buf, size_t len)
{ return (ustrp__sub_buf(p, USTR__PPTR(ps1), pos, buf, len)); }

USTR_CONF_i_PROTO int ustrp__sub(struct Ustr_pool *p, struct Ustr **ps1,
                                 size_t pos, const struct Ustr *s2)
{ /* opts needed? */
  return (ustrp__sub_buf(p, ps1, pos, ustr_cstr(s2), ustr_len(s2)));
}
USTR_CONF_I_PROTO
int ustr_sub(struct Ustr **ps1, size_t pos, const struct Ustr *s2)
{ return (ustrp__sub(0, ps1, pos, s2)); }
USTR_CONF_I_PROTO int ustrp_sub(struct Ustr_pool *p, struct Ustrp **ps1,
                                size_t pos, const struct Ustrp *s2)
{ return (ustrp__sub(p, USTR__PPTR(ps1), pos, &s2->s)); }

USTR_CONF_i_PROTO
int ustrp__sub_subustr(struct Ustr_pool *p, struct Ustr **ps1, size_t pos1,
                       const struct Ustr *s2, size_t pos2, size_t len2)
{
  if (!ustr_assert_valid_subustr(s2, pos2, len2))
    return (USTR_FALSE);
  --pos2;
  
  return (ustrp__sub_buf(p, ps1, pos1, ustr_cstr(s2) + pos2, len2));
}
USTR_CONF_I_PROTO
int ustr_sub_subustr(struct Ustr **ps1, size_t pos1,
                     const struct Ustr *s2, size_t pos2, size_t len2)
{ return (ustrp__sub_subustr(0, ps1, pos1, s2, pos2, len2)); }
USTR_CONF_I_PROTO
int ustrp_sub_subustrp(struct Ustr_pool *p, struct Ustrp **ps1, size_t pos1,
                      const struct Ustrp *s2, size_t pos2, size_t len2)
{ return (ustrp__sub_subustr(p, USTR__PPTR(ps1), pos1, &s2->s, pos2, len2)); }

USTR_CONF_i_PROTO int ustrp__sub_rep_chr(struct Ustr_pool *p, struct Ustr **ps1,
                                         size_t pos, char chr, size_t len)
{
  if (!ustrp__sub_undef(p, ps1, pos, len))
    return (USTR_FALSE);
  --pos;
  
  ustr__memset(*ps1, pos, chr, len);

  return (USTR_FALSE);
}
USTR_CONF_I_PROTO
int ustr_sub_rep_chr(struct Ustr **ps1, size_t pos, char chr, size_t len)
{ return (ustrp__sub_rep_chr(0, ps1, pos, chr, len)); }
USTR_CONF_I_PROTO int ustrp_sub_rep_chr(struct Ustr_pool *p, struct Ustrp **ps1,
                                        size_t pos, char chr, size_t len)
{ return (ustrp__sub_rep_chr(p, USTR__PPTR(ps1), pos, chr, len)); }

USTR_CONF_i_PROTO int ustrp__sc_sub_undef(struct Ustr_pool *p,struct Ustr **ps1,
                                        size_t pos, size_t olen, size_t len)
{
  USTR_ASSERT(ps1);

  if (len == olen)
    return (ustrp__sc_ensure_owner(p, ps1));

  if (!ustr_assert_valid_subustr(*ps1, pos, olen))
    return (USTR_FALSE);
  
  /* work at end, so we don't have to memmove as much */
  if (len < olen)
    return (ustrp__del_subustr(p, ps1, pos +  len,     olen -  len));

  return (ustrp__ins_undef(    p, ps1, pos + olen - 1,  len - olen));
}
USTR_CONF_I_PROTO
int ustr_sc_sub_undef(struct Ustr **ps1, size_t pos, size_t olen, size_t len)
{ return (ustrp__sc_sub_undef(0, ps1, pos, olen, len)); }
USTR_CONF_I_PROTO int ustrp_sc_sub_undef(struct Ustr_pool *p,struct Ustrp **ps1,
                                         size_t pos, size_t olen, size_t len)
{ return (ustrp__sc_sub_undef(p, USTR__PPTR(ps1), pos, olen, len)); }

USTR_CONF_i_PROTO
int ustrp__sc_sub_buf(struct Ustr_pool *p, struct Ustr **ps1,
                      size_t pos, size_t olen, const void *buf, size_t len)
{
  if (!ustrp__sc_sub_undef(p, ps1, pos, olen, len))
    return (USTR_FALSE);

  return (ustrp__sub_buf(p, ps1, pos, buf, len));
}
USTR_CONF_I_PROTO
int ustr_sc_sub_buf(struct Ustr **ps1,
                    size_t pos, size_t olen, const void *buf, size_t len)
{ return (ustrp__sc_sub_buf(0, ps1, pos, olen, buf, len)); }
USTR_CONF_I_PROTO
int ustrp_sc_sub_buf(struct Ustr_pool *p, struct Ustrp **ps1,
                      size_t pos, size_t olen, const void *buf, size_t len)
{ return (ustrp__sc_sub_buf(p, USTR__PPTR(ps1), pos, olen, buf, len)); }

USTR_CONF_i_PROTO
int ustrp__sc_sub(struct Ustr_pool *p, struct Ustr **ps1,size_t pos,size_t olen,
                  const struct Ustr *s2)
{
  return (ustrp__sc_sub_buf(p, ps1, pos, olen, ustr_cstr(s2), ustr_len(s2)));
}
USTR_CONF_I_PROTO
int ustr_sc_sub(struct Ustr **ps1,size_t pos,size_t olen,
                  const struct Ustr *s2)
{ return (ustrp__sc_sub(0, ps1, pos, olen, s2)); }
USTR_CONF_I_PROTO
int ustrp_sc_sub(struct Ustr_pool *p, struct Ustrp **ps1,size_t pos,size_t olen,
                 const struct Ustrp *s2)
{ return (ustrp__sc_sub(p, USTR__PPTR(ps1), pos, olen, &s2->s)); }

USTR_CONF_i_PROTO
int ustrp__sc_sub_subustr(struct Ustr_pool *p,
                          struct Ustr **ps1, size_t pos1, size_t len1,
                          const struct Ustr *s2, size_t pos2, size_t len2)
{
  if (!ustr_assert_valid_subustr(s2, pos2, len2))
    return (USTR_FALSE);
  --pos2;
  
  return (ustrp__sc_sub_buf(p, ps1, pos1, len1, ustr_cstr(s2) + pos2, len2));
}
USTR_CONF_I_PROTO
int ustr_sc_sub_subustr(struct Ustr **ps1, size_t pos1, size_t len1,
                        const struct Ustr *s2, size_t pos2, size_t len2)
{ return (ustrp__sc_sub_subustr(0, ps1, pos1, len1, s2, pos2, len2)); }
USTR_CONF_I_PROTO
int ustrp_sc_sub_subustrp(struct Ustr_pool *p,
                          struct Ustrp **ps1, size_t pos1, size_t len1,
                          const struct Ustrp *s2, size_t pos2, size_t len2)
{ return (ustrp__sc_sub_subustr(p, USTR__PPTR(ps1), pos1, len1,
                                &s2->s, pos2, len2)); }

USTR_CONF_i_PROTO
int ustrp__sc_sub_rep_chr(struct Ustr_pool *p, struct Ustr **ps1,
                          size_t pos, size_t olen, char chr, size_t len)
{
  if (!ustrp__sc_sub_undef(p, ps1, pos, olen, len))
    return (USTR_FALSE);

  return (ustrp__sub_rep_chr(p, ps1, pos, chr, len));
}
USTR_CONF_I_PROTO int ustr_sc_sub_rep_chr(struct Ustr **ps1, size_t pos,
                                          size_t olen, char chr, size_t len)
{ return (ustrp__sc_sub_rep_chr(0, ps1, pos, olen, chr, len)); }
USTR_CONF_I_PROTO
int ustrp_sc_sub_rep_chr(struct Ustr_pool *p, struct Ustrp **ps1,
                         size_t pos, size_t olen, char chr, size_t len)
{ return (ustrp__sc_sub_rep_chr(p, USTR__PPTR(ps1), pos, olen, chr, len)); }

#ifdef USTR_SRCH_H
USTR_CONF_i_PROTO
size_t ustrp__sc_replace_buf(struct Ustr_pool *p, struct Ustr **ps1,
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
size_t ustr_sc_replace_buf(struct Ustr **ps1, const void *optr, size_t olen,
                           const void *nptr, size_t nlen, size_t lim)
{ return (ustrp__sc_replace_buf(0, ps1, optr, olen, nptr, nlen, lim)); }
USTR_CONF_I_PROTO
size_t ustrp_sc_replace_buf(struct Ustr_pool *p, struct Ustrp **ps1,
                            const void *optr, size_t olen,
                            const void *nptr, size_t nlen, size_t lim)
{ return (ustrp__sc_replace_buf(p, USTR__PPTR(ps1), optr,olen,nptr,nlen,lim)); }

USTR_CONF_i_PROTO
size_t ustrp__sc_replace(struct Ustr_pool *p, struct Ustr **ps1,
                         const struct Ustr *srch,
                         const struct Ustr *repl, size_t lim)
{
  USTR_ASSERT(ustr_assert_valid(srch));
  USTR_ASSERT(ustr_assert_valid(repl));

  return (ustrp__sc_replace_buf(p, ps1,
                                ustr_cstr(srch), ustr_len(srch),
                                ustr_cstr(repl), ustr_len(repl), lim));
}
USTR_CONF_I_PROTO
size_t ustr_sc_replace(struct Ustr **ps1, const struct Ustr *srch,
                       const struct Ustr *repl, size_t lim)
{ return (ustrp__sc_replace(0, ps1, srch, repl, lim)); }
USTR_CONF_I_PROTO
size_t ustrp_sc_replace(struct Ustr_pool *p, struct Ustrp **ps1,
                        const struct Ustrp *srch,
                        const struct Ustrp *repl, size_t lim)
{ return (ustrp__sc_replace(p, USTR__PPTR(ps1), &srch->s, &repl->s, lim)); }
#endif

#ifdef USTR_FMT_H
# if USTR_CONF_HAVE_VA_COPY
USTR_CONF_i_PROTO
int ustrp__sub_vfmt_lim(struct Ustr_pool *p, struct Ustr **ps1, size_t pos,
                        size_t lim, const char *fmt, va_list ap)
{ /* NOTE: Copy and pasted so we can use ustrp_set_undef() */
  va_list nap;
  int rc = -1;
  char buf[USTR__SNPRINTF_LOCAL];
  
  va_copy(nap, ap);
  rc = vsnprintf(buf, sizeof(buf), fmt, nap);
  va_end(nap);

  if ((rc == -1) && ((rc = ustr__retard_vfmt_ret(fmt, ap)) == -1))
    return (USTR_FALSE);

  if (lim && ((size_t)rc > lim))
    rc = lim;
  
  if ((size_t)rc < sizeof(buf)) /* everything is done */
    return (ustrp__sub_buf(p, ps1, pos, buf, rc));
  
  if (!ustrp__sub_undef(p, ps1, pos, rc))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  vsnprintf(ustr_wstr(*ps1), rc + 1, fmt, ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_sub_vfmt_lim(struct Ustr **ps1,size_t pos,size_t lim,
                                        const char *fmt, va_list ap)
{ return (ustrp__sub_vfmt_lim(0, ps1, pos, lim, fmt, ap)); }
USTR_CONF_I_PROTO
int ustrp_sub_vfmt_lim(struct Ustr_pool *p,struct Ustrp **ps1, size_t pos,
                       size_t lim, const char *fmt, va_list ap)
{ return (ustrp__sub_vfmt_lim(p, USTR__PPTR(ps1), pos, lim, fmt, ap)); }

USTR_CONF_I_PROTO int ustr_sub_fmt_lim(struct Ustr **ps1, size_t pos,
                                       size_t lim, const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustr_sub_vfmt_lim(ps1, pos, lim, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO
int ustrp_sub_fmt_lim(struct Ustr_pool *p, struct Ustrp **ps1, size_t pos,
                      size_t lim, const char*fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustrp_sub_vfmt_lim(p, ps1, pos, lim, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO int ustr_sub_vfmt(struct Ustr **ps1, size_t pos,
                                    const char *fmt, va_list ap)
{ return (ustr_sub_vfmt_lim(ps1, pos, 0, fmt, ap)); }

USTR_CONF_I_PROTO int ustrp_sub_vfmt(struct Ustr_pool *p, struct Ustrp **ps1,
                                     size_t pos, const char *fmt, va_list ap)
{ return (ustrp_sub_vfmt_lim(p, ps1, pos, 0, fmt, ap)); }

USTR_CONF_I_PROTO
int ustr_sub_fmt(struct Ustr **ps1, size_t pos, const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustr_sub_vfmt(ps1, pos, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO int ustrp_sub_fmt(struct Ustr_pool *p, struct Ustrp **ps1,
                                    size_t pos, const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustrp_sub_vfmt(p, ps1, pos, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_i_PROTO
int ustrp__sc_sub_vfmt_lim(struct Ustr_pool *p, struct Ustr **ps1, size_t pos,
                           size_t len, size_t lim, const char *fmt, va_list ap)
{ /* NOTE: Copy and pasted so we can use ustrp_set_undef() */
  va_list nap;
  int rc = -1;
  char buf[USTR__SNPRINTF_LOCAL];
  
  va_copy(nap, ap);
  rc = vsnprintf(buf, sizeof(buf), fmt, nap);
  va_end(nap);

  if ((rc == -1) && ((rc = ustr__retard_vfmt_ret(fmt, ap)) == -1))
    return (USTR_FALSE);

  if (lim && ((size_t)rc > lim))
    rc = lim;
  
  if ((size_t)rc < sizeof(buf)) /* everything is done */
    return (ustrp__sc_sub_buf(p, ps1, pos, len, buf, rc));
  
  if (!ustrp__sc_sub_undef(p, ps1, pos, len, rc))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  vsnprintf(ustr_wstr(*ps1), rc + 1, fmt, ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO
int ustr_sc_sub_vfmt_lim(struct Ustr **ps1,size_t pos, size_t len,
                         size_t lim, const char *fmt, va_list ap)
{ return (ustrp__sc_sub_vfmt_lim(0, ps1, pos, len, lim, fmt, ap)); }
USTR_CONF_I_PROTO
int ustrp_sc_sub_vfmt_lim(struct Ustr_pool *p,struct Ustrp **ps1, size_t pos,
                          size_t len, size_t lim, const char *fmt, va_list ap)
{ return (ustrp__sc_sub_vfmt_lim(p, USTR__PPTR(ps1), pos, len, lim, fmt, ap)); }

USTR_CONF_I_PROTO
int ustr_sc_sub_fmt_lim(struct Ustr **ps1, size_t pos, size_t len,
                        size_t lim, const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustr_sc_sub_vfmt_lim(ps1, pos, len, lim, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO
int ustrp_sc_sub_fmt_lim(struct Ustr_pool *p, struct Ustrp **ps1, size_t pos,
                         size_t len, size_t lim, const char*fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustrp_sc_sub_vfmt_lim(p, ps1, pos, len, lim, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO int ustr_sc_sub_vfmt(struct Ustr **ps1, size_t pos,
                                       size_t len, const char *fmt, va_list ap)
{ return (ustr_sc_sub_vfmt_lim(ps1, pos, len, 0, fmt, ap)); }

USTR_CONF_I_PROTO
int ustrp_sc_sub_vfmt(struct Ustr_pool *p, struct Ustrp **ps1,
                   size_t pos, size_t len, const char *fmt, va_list ap)
{ return (ustrp_sc_sub_vfmt_lim(p, ps1, pos, len, 0, fmt, ap)); }

USTR_CONF_I_PROTO int ustr_sc_sub_fmt(struct Ustr **ps1, size_t pos, size_t len,
                                      const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustr_sc_sub_vfmt(ps1, pos, len, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO
int ustrp_sc_sub_fmt(struct Ustr_pool *p, struct Ustrp **ps1,
                     size_t pos, size_t len, const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustrp_sc_sub_vfmt(p, ps1, pos, len, fmt, ap);
  va_end(ap);
  
  return (ret);
}
# endif
#endif
