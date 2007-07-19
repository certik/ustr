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

  return (USTR_TRUE);
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

#ifdef USTR_FMT_H
# if USTR_CONF_HAVE_VA_COPY
USTR_CONF_i_PROTO
int ustrp__sub_vfmt_lim(struct Ustr_pool *p, struct Ustr **ps1, size_t pos,
                        size_t lim, const char *fmt, va_list ap)
{ /* NOTE: Copy and pasted so we can use ustrp_set_undef() */
  va_list nap;
  int rc = -1;
  char buf[USTR__SNPRINTF_LOCAL];
  char *ptr;
  char save_end;
  
  va_copy(nap, ap);
  rc = vsnprintf(buf, sizeof(buf), fmt, nap);
  va_end(nap);

  if ((rc == -1) && ((rc = ustr__retard_vfmt_ret(fmt, ap)) == -1))
    return (USTR_FALSE);

  if (lim && ((size_t)rc > lim))
    rc = lim;
  
  if ((size_t)rc < sizeof(buf)) /* everything is done */
    return (ustrp__sub_buf(p, ps1, pos, buf, rc));
  
  if (!ustrp__sub_undef(p, ps1, pos--, rc))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  ptr = ustr_wstr(*ps1);
  
  save_end = ptr[pos + rc]; /* might be NIL, might be a char */
  vsnprintf(ptr + pos, rc + 1, fmt, ap);
  ptr[pos + rc] = save_end;

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
  char *ptr;
  char save_end;
  
  va_copy(nap, ap);
  rc = vsnprintf(buf, sizeof(buf), fmt, nap);
  va_end(nap);

  if ((rc == -1) && ((rc = ustr__retard_vfmt_ret(fmt, ap)) == -1))
    return (USTR_FALSE);

  if (lim && ((size_t)rc > lim))
    rc = lim;
  
  if ((size_t)rc < sizeof(buf)) /* everything is done */
    return (ustrp__sc_sub_buf(p, ps1, pos, len, buf, rc));
  
  if (!ustrp__sc_sub_undef(p, ps1, pos--, len, rc))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  ptr = ustr_wstr(*ps1);
  
  save_end = ptr[pos + rc]; /* might be NIL if at end, might be a char */
  vsnprintf(ptr + pos, rc + 1, fmt, ap);
  ptr[pos + rc] = save_end;

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
