/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_FMT_H
#error " Include ustr-fmt.h before this file."
#endif

#include <stdio.h>  /* vsnprintf */
#include <errno.h>  /* ENOMEM, for fmt */

/* snprintf is "expensive", so calling it once with a stack buffer followed by
 * a memcpy() is almost certainly better than calling it twice */
#define USTR__SNPRINTF_LOCAL        128

/*  Retarded versions of snprintf() return -1 (Ie. Solaris) instead of the
 * desired length so we have to loop calling vsnprintf() until we find a
 * value "big enough".
 *  However ISO 9899:1999 says vsnprintf returns -1 for bad multi-byte strings,
 * so we don't want to loop forever.
 *  So we try and work out USTR_CONF_HAVE_RETARDED_VSNPRINTF, but if it's on
 * when it doesn't need to be it'll be horribly slow on multi-byte errors. */
#define USTR__RETARDED_SNPRINTF_MIN (USTR__SNPRINTF_LOCAL * 2)
#define USTR__RETARDED_SNPRINTF_MAX (1024 * 1024 * 256)

#ifndef USTR_CONF_HAVE_RETARDED_VSNPRINTF /* safe side, but see above */
#define USTR_CONF_HAVE_RETARDED_VSNPRINTF 1
#endif

#if USTR_CONF_HAVE_VA_COPY

# if USTR_CONF_HAVE_RETARDED_VSNPRINTF
USTR_CONF_e_PROTO
int ustr__retard_vfmt_ret(const char *fmt, va_list ap)
    USTR__COMPILE_ATTR_NONNULL_A() USTR__COMPILE_ATTR_FMT(1, 0);
USTR_CONF_i_PROTO
int ustr__retard_vfmt_ret(const char *fmt, va_list ap)
{
  size_t sz = USTR__RETARDED_SNPRINTF_MIN;
  char *ptr = 0;
  int ret = -1;
  va_list nap;
  
  if (!(ptr = USTR_CONF_MALLOC(sz)))
    return (-1);

  va_copy(nap, ap);
  while ((ret = vsnprintf(ptr, sz, fmt, nap)) == -1)
  {
    char *tmp = 0;
    
    va_end(nap);
    
    sz *= 2;
    
    if ((sz >= USTR__RETARDED_SNPRINTF_MAX) ||
        !(tmp = USTR_CONF_REALLOC(ptr, sz)))
    {
      USTR_CONF_FREE(ptr);
      return (-1);
    }
    
    va_copy(nap, ap);
  }
  va_end(nap);
  
  /*  We could probably do the memcpy optimization here, with a bit of jumping
   * through hoops (and a different interface ... but I prefer the single code
   * path to sane platforms that I'm likely to be testing on. */
  USTR_CONF_FREE(ptr);

  return (ret);
}
# else
#  define ustr__retard_vfmt_ret(x, y) (-1)
# endif

USTR_CONF_e_PROTO int ustrp__add_vfmt_lim(void *p, struct Ustr **ps1,size_t lim,
                                          const char *fmt, va_list ap)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4)) USTR__COMPILE_ATTR_FMT(4, 0);
USTR_CONF_i_PROTO int ustrp__add_vfmt_lim(void *p, struct Ustr **ps1,size_t lim,
                                          const char *fmt, va_list ap)
{ /* you might think we want to juse use ustr_wstr()
   * if ustr_size() - ustr_len() > 0 ... however the problem there is about
   * screwing up the data when we fail. Which we just don't do */
  char *tmp = 0;
  size_t os1len = 0;
  va_list nap;
  int rc = -1;
  char buf[USTR__SNPRINTF_LOCAL];
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  va_copy(nap, ap);
  rc = vsnprintf(buf, sizeof(buf), fmt, nap);
  va_end(nap);

  if (rc == -1)
  {
    va_copy(nap, ap);
    rc = ustr__retard_vfmt_ret(fmt, nap);
    va_end(ap);

    if (rc == -1)
      return (USTR_FALSE);
  }

  if (lim && ((size_t)rc > lim))
    rc = lim;
  
  if ((size_t)rc < sizeof(buf)) /* everything is done */
    return (ustrp__add_buf(p, ps1, buf, rc));
  
  os1len = ustr_len(*ps1);
  if (!ustrp__add_undef(p, ps1, rc))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  tmp = ustr_wstr(*ps1) + os1len;
  
  vsnprintf(tmp, rc + 1, fmt, ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO
int ustr_add_vfmt_lim(struct Ustr **ps1, size_t lim, const char *fmt,va_list ap)
{ return (ustrp__add_vfmt_lim(0, ps1, lim, fmt, ap)); }
USTR_CONF_I_PROTO int ustrp_add_vfmt_lim(void *p, struct Ustrp **ps1,
                                         size_t lim, const char *fmt,va_list ap)
{ return (ustrp__add_vfmt_lim(p, USTR__PPTR(ps1), lim, fmt, ap)); }

USTR_CONF_I_PROTO
int ustr_add_fmt_lim(struct Ustr **ps1, size_t lim, const char *fmt, ...)
{
  va_list ap;
  int ret = -1;
  
  va_start(ap, fmt);
  ret = ustr_add_vfmt_lim(ps1, lim, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO int ustrp_add_fmt_lim(void *p, struct Ustrp **ps1, size_t lim,
                                        const char *fmt, ...)
{
  va_list ap;
  int ret = -1;
  
  va_start(ap, fmt);
  ret = ustrp_add_vfmt_lim(p, ps1, lim, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO int ustr_add_vfmt(struct Ustr **ps1,const char*fmt,va_list ap)
{ return (ustr_add_vfmt_lim(ps1, 0, fmt, ap)); }

USTR_CONF_I_PROTO
int ustrp_add_vfmt(void *p, struct Ustrp **ps1, const char *fmt, va_list ap)
{ return (ustrp_add_vfmt_lim(p, ps1, 0, fmt, ap)); }

USTR_CONF_I_PROTO int ustr_add_fmt(struct Ustr **ps1, const char *fmt, ...)
{
  va_list ap;
  int ret = -1;
  
  va_start(ap, fmt);
  ret = ustr_add_vfmt(ps1, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO
int ustrp_add_fmt(void *p, struct Ustrp **ps1, const char *fmt, ...)
{
  va_list ap;
  int ret = -1;
  
  va_start(ap, fmt);
  ret = ustrp_add_vfmt(p, ps1, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_vfmt_lim(void *, size_t, size_t, int, int, size_t,
                                  const char *, va_list)
    USTR__COMPILE_ATTR_NONNULL_L((7)) USTR__COMPILE_ATTR_FMT(7, 0);
USTR_CONF_i_PROTO
struct Ustr *ustrp__dupx_vfmt_lim(void *p, size_t sz, size_t rbytes, int exact,
                                  int emem, size_t lim,
                                  const char *fmt, va_list ap)
{ /* NOTE: Copy and pasted so we don't have to allocate twice, just to get the
   * options */
  struct Ustr *ret = USTR_NULL;
  va_list nap;
  int rc = -1;
  char buf[USTR__SNPRINTF_LOCAL];
  
  va_copy(nap, ap);
  rc = vsnprintf(buf, sizeof(buf), fmt, nap);
  va_end(nap);

  if (rc == -1)
  {
    va_copy(nap, ap);
    rc = ustr__retard_vfmt_ret(fmt, nap);
    va_end(ap);

    if (rc == -1)
      return (USTR_FALSE);
  }

  if (lim && ((size_t)rc > lim))
    rc = lim;
  
  if ((size_t)rc < sizeof(buf)) /* everything is done */
    return (ustrp__dupx_buf(p, sz, rbytes, exact, emem, buf, rc));
  
  if (!(ret = ustrp__dupx_undef(p, sz, rbytes, exact, emem, rc)))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  vsnprintf(ustr_wstr(ret), rc + 1, fmt, ap);

  USTR_ASSERT(ustr_assert_valid(ret));
  
  return (ret);
}

USTR_CONF_I_PROTO
struct Ustr *ustr_dupx_vfmt_lim(size_t sz, size_t rbytes, int exact,
                                int emem,size_t lim,const char *fmt, va_list ap)
{ return (ustrp__dupx_vfmt_lim(0, sz, rbytes, exact, emem, lim, fmt, ap)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_vfmt_lim(void *p, size_t sz, size_t rb, int exact,
                                  int emem, size_t lim,
                                  const char *fmt, va_list ap)
{ return (USTRP(ustrp__dupx_vfmt_lim(p, sz, rb, exact, emem, lim, fmt, ap))); }

USTR_CONF_I_PROTO
struct Ustr *ustr_dupx_fmt_lim(size_t sz, size_t rbytes, int exact,
                               int emem, size_t lim, const char *fmt, ...)
{
  va_list ap;
  struct Ustr *ret = USTR_NULL;
  
  va_start(ap, fmt);
  ret = ustr_dupx_vfmt_lim(sz, rbytes, exact, emem, lim, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_fmt_lim(void *p, size_t sz, size_t rbytes, int exact,
                                 int emem, size_t lim, const char *fmt, ...)
                 
{
  va_list ap;
  struct Ustrp *ret = USTRP_NULL;
  
  va_start(ap, fmt);
  ret = ustrp_dupx_vfmt_lim(p, sz, rbytes, exact, emem, lim, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO
struct Ustr *ustr_dupx_vfmt(size_t sz, size_t rbytes, int exact,
                            int emem, const char *fmt, va_list ap)
{ return (ustr_dupx_vfmt_lim(sz, rbytes, exact, emem, 0, fmt, ap)); }

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_vfmt(void *p, size_t sz, size_t rbytes, int exact,
                              int emem, const char *fmt, va_list ap)
{ return (ustrp_dupx_vfmt_lim(p, sz, rbytes, exact, emem, 0, fmt, ap)); }

USTR_CONF_I_PROTO struct Ustr *ustr_dupx_fmt(size_t sz, size_t rbytes,int exact,
                                             int emem, const char *fmt, ...)
{
  va_list ap;
  struct Ustr *ret = USTR_NULL;
  
  va_start(ap, fmt);
  ret = ustr_dupx_vfmt(sz, rbytes, exact, emem, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_fmt(void *p, size_t sz, size_t rbytes, int exact,
                             int emem, const char *fmt, ...)
                 
{
  va_list ap;
  struct Ustrp *ret = USTRP_NULL;
  
  va_start(ap, fmt);
  ret = ustrp_dupx_vfmt(p, sz, rbytes, exact, emem, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO
struct Ustr *ustr_dup_vfmt_lim(size_t lim, const char *fmt, va_list ap)
{ return (ustr_dupx_vfmt_lim(USTR__DUPX_DEF, lim, fmt, ap)); }

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dup_vfmt_lim(void *p, size_t lim,const char *fmt,va_list ap)
{ return (ustrp_dupx_vfmt_lim(p, USTR__DUPX_DEF, lim, fmt, ap)); }

USTR_CONF_I_PROTO
struct Ustr *ustr_dup_fmt_lim(size_t lim, const char *fmt, ...)
{
  va_list ap;
  struct Ustr *ret = USTR_NULL;
  
  va_start(ap, fmt);
  ret = ustr_dup_vfmt_lim(lim, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dup_fmt_lim(void *p, size_t lim, const char *fmt, ...)
                 
{
  va_list ap;
  struct Ustrp *ret = USTRP_NULL;
  
  va_start(ap, fmt);
  ret = ustrp_dup_vfmt_lim(p, lim, fmt, ap);
  va_end(ap);

  return (ret);
}

USTR_CONF_I_PROTO struct Ustr *ustr_dup_vfmt(const char *fmt, va_list ap)
{ return (ustr_dupx_vfmt(USTR__DUPX_DEF, fmt, ap)); }

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dup_vfmt(void *p, const char *fmt, va_list ap)
{ return (ustrp_dupx_vfmt(p, USTR__DUPX_DEF, fmt, ap)); }

USTR_CONF_I_PROTO struct Ustr *ustr_dup_fmt(const char *fmt, ...)
{
  va_list ap;
  struct Ustr *ret = USTR_NULL;
  
  va_start(ap, fmt);
  ret = ustr_dup_vfmt(fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO struct Ustrp *ustrp_dup_fmt(void *p, const char *fmt, ...)
{
  va_list ap;
  struct Ustrp *ret = USTRP_NULL;
  
  va_start(ap, fmt);
  ret = ustrp_dup_vfmt(p, fmt, ap);
  va_end(ap);
  
  return (ret);
}

# ifdef USTR_SET_H
USTR_CONF_e_PROTO int ustrp__set_vfmt_lim(void *p, struct Ustr **ps1,size_t lim,
                                          const char *fmt, va_list ap)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4)) USTR__COMPILE_ATTR_FMT(4, 0);
USTR_CONF_i_PROTO int ustrp__set_vfmt_lim(void *p, struct Ustr **ps1,size_t lim,
                                          const char *fmt, va_list ap)
{ /* NOTE: Copy and pasted so we can use ustrp_set_undef() */
  va_list nap;
  int rc = -1;
  char buf[USTR__SNPRINTF_LOCAL];
  
  va_copy(nap, ap);
  rc = vsnprintf(buf, sizeof(buf), fmt, nap);
  va_end(nap);

  if (rc == -1)
  {
    va_copy(nap, ap);
    rc = ustr__retard_vfmt_ret(fmt, nap);
    va_end(ap);

    if (rc == -1)
      return (USTR_FALSE);
  }

  if (lim && ((size_t)rc > lim))
    rc = lim;
  
  if ((size_t)rc < sizeof(buf)) /* everything is done */
    return (ustrp__set_buf(p, ps1, buf, rc));
  
  if (!ustrp__set_undef(p, ps1, rc))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  vsnprintf(ustr_wstr(*ps1), rc + 1, fmt, ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_set_vfmt_lim(struct Ustr **ps1, size_t lim,
                                        const char *fmt, va_list ap)
{ return (ustrp__set_vfmt_lim(0, ps1, lim, fmt, ap)); }
USTR_CONF_I_PROTO int ustrp_set_vfmt_lim(void *p,struct Ustrp **ps1, size_t lim,
                                         const char *fmt, va_list ap)
{ return (ustrp__set_vfmt_lim(p, USTR__PPTR(ps1), lim, fmt, ap)); }

USTR_CONF_I_PROTO
int ustr_set_fmt_lim(struct Ustr **ps1, size_t lim, const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustr_set_vfmt_lim(ps1, lim, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO
int ustrp_set_fmt_lim(void *p,struct Ustrp **ps1,size_t lim,const char*fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustrp_set_vfmt_lim(p, ps1, lim, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO int ustr_set_vfmt(struct Ustr **ps1,
                                    const char *fmt, va_list ap)
{ return (ustr_set_vfmt_lim(ps1, 0, fmt, ap)); }

USTR_CONF_I_PROTO int ustrp_set_vfmt(void *p, struct Ustrp **ps1,
                                     const char *fmt, va_list ap)
{ return (ustrp_set_vfmt_lim(p, ps1, 0, fmt, ap)); }

USTR_CONF_I_PROTO int ustr_set_fmt(struct Ustr **ps1, const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustr_set_vfmt(ps1, fmt, ap);
  va_end(ap);
  
  return (ret);
}

USTR_CONF_I_PROTO int ustrp_set_fmt(void *p, struct Ustrp **ps1,
                                    const char *fmt, ...)
{
  va_list ap;
  int ret = USTR_FALSE;
  
  va_start(ap, fmt);
  ret = ustrp_set_vfmt(p, ps1, fmt, ap);
  va_end(ap);
  
  return (ret);
}

# endif
#else

/* twice the copy and paste, none of the value (for any decent platform) */
USTR_CONF_I_PROTO
int ustr_add_fmt_lim(struct Ustr **ps1, size_t lim, const char *fmt, ...)
{
  char *tmp = 0;
  size_t os1len = 0;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_FALSE);
  
  if (lim && ((size_t)ret > lim))
    ret = lim;
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
    return (ustr_add_buf(ps1, buf, ret));
  
  os1len = ustr_len(*ps1);
  if (!ustr_add_undef(ps1, ret))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  tmp = ustr_wstr(*ps1) + os1len;
  
  va_start(ap, fmt);
  vsnprintf(tmp, ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO int ustrp_add_fmt_lim(void *p, struct Ustrp **ps1, size_t lim,
                                        const char *fmt, ...)
{
  char *tmp = 0;
  size_t os1len = 0;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_FALSE);
  
  if (lim && ((size_t)ret > lim))
    ret = lim;
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
    return (ustrp_add_buf(p, ps1, buf, ret));
  
  os1len = ustr_len(*ps1);
  if (!ustrp_add_undef(p, ps1, ret))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  tmp = ustr_wstr(*ps1) + os1len;
  
  va_start(ap, fmt);
  vsnprintf(tmp, ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO int ustr_add_fmt(struct Ustr **ps1, const char *fmt, ...)
{
  char *tmp = 0;
  size_t os1len = 0;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_FALSE);
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
    return (ustr_add_buf(ps1, buf, ret));
  
  os1len = ustr_len(*ps1);
  if (!ustr_add_undef(ps1, ret))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  tmp = ustr_wstr(*ps1) + os1len;
  
  va_start(ap, fmt);
  vsnprintf(tmp, ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO
int ustrp_add_fmt(void *p, struct Ustrp **ps1, const char *fmt, ...)
{
  char *tmp = 0;
  size_t os1len = 0;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_FALSE);
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
    return (ustrp_add_buf(p, ps1, buf, ret));
  
  os1len = ustr_len(*ps1);
  if (!ustrp_add_undef(p, ps1, ret))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  tmp = ustr_wstr(*ps1) + os1len;
  
  va_start(ap, fmt);
  vsnprintf(tmp, ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO
struct Ustr *ustr_dupx_fmt_lim(size_t sz, size_t rbytes, int exact,
                               int emem, size_t lim, const char *fmt, ...)
{
  struct Ustr *s1 = USTR_NULL;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_NULL);
  
  if (lim && ((size_t)ret > lim))
    ret = lim;
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
  {
    if (!(s1 = ustr_dupx_buf(sz, rbytes, exact, emem, buf, ret)))
      errno = ENOMEM; /* for EILSEQ etc. */
    return (s1);
  }
  
  if (!(s1 = ustr_dupx_undef(sz, rbytes, exact, emem, ret)))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_NULL);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(s1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  return (s1);
}

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_fmt_lim(void *p, size_t sz, size_t rbytes, int exact,
                                 int emem, const char *fmt, ...)
{
  struct Ustrp *s1 = USTRP_NULL;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_NULL);
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
  {
    if (!(s1 = ustrp_dupx_buf(p, sz, rbytes, exact, emem, buf, ret)))
      errno = ENOMEM; /* for EILSEQ etc. */
    return (s1);
  }
  
  if (!(s1 = ustrp_dupx_undef(p, sz, rbytes, exact, emem, ret)))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_NULL);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(s1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  return (s1);
}

USTR_CONF_I_PROTO struct Ustr *ustr_dup_fmt_lim(size_t lim,const char *fmt, ...)
{
  struct Ustr *s1 = USTR_NULL;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (ret == -1)
    return (USTR_FALSE);
  
  if (lim && ((size_t)ret > lim))
    ret = lim;
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
  {
    if (!(s1 = ustr_dup_buf(buf, ret)))
      errno = ENOMEM; /* for EILSEQ etc. */
    return (s1);
  }
  
  if (!(s1 = ustr_dup_undef(ret)))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(s1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  return (s1);
}

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dup_fmt_lim(void *p, size_t lim, const char *fmt, ...)
{
  struct Ustrp *s1 = USTRP_NULL;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (ret == -1)
    return (USTR_FALSE);
  
  if (lim && ((size_t)ret > lim))
    ret = lim;
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
  {
    if (!(s1 = ustrp_dup_buf(p, buf, ret)))
      errno = ENOMEM; /* for EILSEQ etc. */
    return (s1);
  }
  
  if (!(s1 = ustrp_dup_undef(p, ret)))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(s1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  return (s1);
}

USTR_CONF_I_PROTO struct Ustr *ustr_dup_fmt(const char *fmt, ...)
{
  struct Ustr *s1 = USTR_NULL;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (ret == -1)
    return (USTR_FALSE);
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
  {
    if (!(s1 = ustr_dup_buf(buf, ret)))
      errno = ENOMEM; /* for EILSEQ etc. */
    return (s1);
  }
  
  if (!(s1 = ustr_dup_undef(ret)))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(s1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  return (s1);
}

USTR_CONF_I_PROTO struct Ustrp *ustrp_dup_fmt(void *p, const char *fmt, ...)
{
  struct Ustrp *s1 = USTRP_NULL;
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (ret == -1)
    return (USTR_FALSE);
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
  {
    if (!(s1 = ustrp_dup_buf(p, buf, ret)))
      errno = ENOMEM; /* for EILSEQ etc. */
    return (s1);
  }
  
  if (!(s1 = ustrp_dup_undef(p, ret)))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(s1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  return (s1);
}

# ifdef USTR_SET_H
USTR_CONF_I_PROTO
int ustr_set_fmt_lim(struct Ustr **ps1, size_t lim, const char *fmt, ...)
{ /* This version used even if we have va_copy(), due to ustr_set_undef() */
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_FALSE);

  if (lim && ((size_t)ret > lim))
    ret = lim;
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
    return (ustr_set_buf(ps1, buf, ret));
  
  if (!ustr_set_undef(ps1, ret))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(*ps1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO int ustrp_set_fmt_lim(void *p, struct Ustrp **ps1,
                                        size_t lim, const char *fmt, ...)
{ /* This version used even if we have va_copy(), due to ustr_set_undef() */
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_FALSE);

  if (lim && ((size_t)ret > lim))
    ret = lim;
  
  if ((size_t)ret < sizeof(buf)) /* everything is done */
    return (ustrp_set_buf(p, ps1, buf, ret));
  
  if (!ustrp_set_undef(p, ps1, ret))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(*ps1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO int ustr_set_fmt(struct Ustr **ps1, const char *fmt, ...)
{ /* This version used even if we have va_copy(), due to ustr_set_undef() */
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_FALSE);

  if ((size_t)ret < sizeof(buf)) /* everything is done */
    return (ustr_set_buf(ps1, buf, ret));
  
  if (!ustr_set_undef(ps1, ret))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(*ps1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  return (USTR_TRUE);
}

USTR_CONF_I_PROTO int ustrp_set_fmt(void *p, struct Ustrp **ps1,
                                    const char *fmt, ...)
{ /* This version used even if we have va_copy(), due to ustr_set_undef() */
  va_list ap;
  char buf[USTR__SNPRINTF_LOCAL];
  int ret = -1;
  
  va_start(ap, fmt);
  ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  
  if (ret == -1)
    return (USTR_FALSE);

  if ((size_t)ret < sizeof(buf)) /* everything is done */
    return (ustrp_set_buf(p, ps1, buf, ret));
  
  if (!ustrp_set_undef(p, ps1, ret))
  {
    errno = ENOMEM; /* for EILSEQ etc. */
    return (USTR_FALSE);
  }
  
  va_start(ap, fmt);
  vsnprintf(ustr_wstr(*ps1), ret + 1, fmt, ap); /* assuming it works now */
  va_end(ap);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  return (USTR_TRUE);
}
# endif
#endif

