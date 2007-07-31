/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_IO_H
#error " You should have already included ustr-io.h, or just include ustr.h."
#endif

USTR_CONF_i_PROTO int ustrp__io_get(void *p, struct Ustr **ps1, FILE *fp,
                                    size_t minlen, size_t *got)
{
  size_t olen = ustr_len(*ps1);
  size_t ret  = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1) && fp);

  if (!minlen)
  {
    if (got)
      *got = 0;
    return (USTR_TRUE);
  }
  
  if (!ustrp__add_undef(p, ps1, minlen))
    return (USTR_FALSE);

  ret = fread(ustr_wstr(*ps1) + olen, 1, minlen, fp);
  if (ret < minlen)
    ustrp__del(p, ps1, minlen - ret);

  if (got)
    *got = ret;
  
  return (ret > 0);
}
USTR_CONF_I_PROTO
int ustr_io_get(struct Ustr **ps1, FILE *fp, size_t minlen, size_t *got)
{ return (ustrp__io_get(0, ps1, fp, minlen, got)); }
USTR_CONF_I_PROTO int ustrp_io_get(void *p, struct Ustrp **ps1, FILE *fp,
                                   size_t minlen, size_t *got)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__io_get(p, &tmp, fp, minlen, got);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO int ustrp__io_getfile(void *p, struct Ustr **ps1, FILE *fp)
{
  const size_t blksz = (1024 * 8) - (1 + 8 + 8 + 8 + sizeof(USTR_END_ALOCDx));
  size_t num = blksz;
  size_t got = 0;

  do
  { /* round up... not perfect as we'll be able to round up post add_undef */
    size_t sz   = ustr_size(*ps1);
    size_t clen = ustr_len(*ps1);

    num = blksz;
    if (num < (sz - clen))
      num = sz - clen;
  } while (ustrp__io_get(p, ps1, fp, num, &got) && (got == num));

  return (feof(fp));
}
USTR_CONF_I_PROTO
int ustr_io_getfile(struct Ustr **ps1, FILE *fp)
{ return (ustrp__io_getfile(0, ps1, fp)); }
USTR_CONF_I_PROTO
int ustrp_io_getfile(void *p, struct Ustrp **ps1, FILE *fp)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__io_getfile(p, &tmp, fp);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO
int ustrp__io_getfilename(void *p, struct Ustr **ps1, const char *name)
{
  FILE *fp = fopen(name, "rb");
  int ret = USTR_FALSE;
  int save_errno = 0;
  
  if (!fp)
    return (USTR_FALSE);
  
  ret = ustrp__io_getfile(p, ps1, fp);
  
  save_errno = errno;
  fclose(fp);
  errno = save_errno;
  
  return (ret);
}
USTR_CONF_I_PROTO
int ustr_io_getfilename(struct Ustr **ps1, const char *name)
{ return (ustrp__io_getfilename(0, ps1, name)); }
USTR_CONF_I_PROTO
int ustrp_io_getfilename(void *p, struct Ustrp **ps1, const char *name)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__io_getfilename(p, &tmp, name);
  *ps1 = USTRP(tmp);
  return (ret);
}

/* We bow to retarded GLibc getc_unlocked */
#ifdef getc_unlocked
# define USTR__IO_GETC(x) getc_unlocked(x)
#else
# define USTR__IO_GETC(x) getc(x)
#endif

USTR_CONF_i_PROTO int ustrp__io_getline(void *p, struct Ustr **ps1, FILE *fp)
{
  int val = EOF;
  size_t olen = 0;
  const unsigned int linesz = 81; /* assume a line is roughly 80 chars */
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1) && fp);

  olen = ustr_len(*ps1);
  while (ustrp__add_undef(p, ps1, linesz))
  {
    char *wstr = ustr_wstr(*ps1) + olen;
    size_t num = linesz;
    
    while (num && ((val = USTR__IO_GETC(fp)) != EOF))
    {
      --num;
      if ((*wstr++ = val) == '\n')
        break;
    }
    
    if (num)
    {
      ustrp__del(p, ps1, num);
      break;
    }
    
    olen += linesz;
  }

  return (val == '\n');
}
#undef USTR__IO_GETC
USTR_CONF_I_PROTO
int ustr_io_getline(struct Ustr **ps1, FILE *fp)
{ return (ustrp__io_getline(0, ps1, fp)); }
USTR_CONF_I_PROTO
int ustrp_io_getline(void *p, struct Ustrp **ps1, FILE *fp)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__io_getline(p, &tmp, fp);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO
int ustrp__io_put(void *p, struct Ustr **ps1, FILE *fp, size_t beglen)
{
  size_t ret = 0;
  size_t clen = ustr_len(*ps1);
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1) && fp);

  USTR_ASSERT_RET(beglen <= clen, USTR_FALSE);
  
  if (!beglen)
    return (USTR_TRUE);

  /* Because most errors only happen when a fflush() of the stdio stream,
   * you have to assume that any data written is indeterminate anyway.
   * So only go to the effort of making it determinate if beglen != clen.
   * If you need efficient IO, that you can deal with errors for, use Vstr
   * and POSIX IO. */
  if ((beglen != clen) && !ustrp__sc_ensure_owner(p, ps1))
    return (USTR_FALSE);
  
  if ((ret = fwrite(ustr_cstr(*ps1), 1, beglen, fp)))
  {
    int save_errno = errno;
    
    if (beglen == clen) /* Note not ret == clen, see above */
      ustrp__sc_del(p, ps1);
    else
      ustrp__del_subustr(p, ps1, 1, ret); /* if it fails, see above */
    errno = save_errno;
  }
  
  return (ret == beglen);
}
USTR_CONF_I_PROTO
int ustr_io_put(struct Ustr **ps1, FILE *fp, size_t beglen)
{ return (ustrp__io_put(0, ps1, fp, beglen)); }
USTR_CONF_I_PROTO
int ustrp_io_put(void *p, struct Ustrp **ps1, FILE *fp, size_t beglen)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__io_put(p, &tmp, fp, beglen);
  *ps1 = USTRP(tmp);
  return (ret);
}

/* We bow to retarded GLibc putc_unlocked */
#ifdef putc_unlocked
# define USTR__IO_PUTC(x, y) putc_unlocked(x, y)
#else
# define USTR__IO_PUTC(x, y) putc(x, y)
#endif
USTR_CONF_i_PROTO
int ustrp__io_putline(void *p, struct Ustr **ps1, FILE *fp, size_t beglen)
{
  if (!ustrp__io_put(p, ps1, fp, beglen))
    return (USTR_FALSE);

  return (USTR__IO_PUTC('\n', fp) != EOF);
}
#undef USTR__IO_PUTC
USTR_CONF_I_PROTO
int ustr_io_putline(struct Ustr **ps1, FILE *fp, size_t beglen)
{ return (ustrp__io_putline(0, ps1, fp, beglen)); }
USTR_CONF_I_PROTO
int ustrp_io_putline(void *p, struct Ustrp **ps1, FILE *fp, size_t beglen)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__io_putline(p, &tmp, fp, beglen);
  *ps1 = USTRP(tmp);
  return (ret);
}

USTR_CONF_i_PROTO int ustrp__io_putfilename(void *p, struct Ustr **ps1,
                                            const char *name, const char *mode)
{
  FILE *fp = fopen(name, mode);
  int ret = USTR_FALSE;
  
  if (!fp)
    return (USTR_FALSE);
  
  if ((ret = ustrp__io_put(p, ps1, fp, ustr_len(*ps1))))
    ret = !fclose(fp); /* if everything OK, defer to the fclose() return */
  else
  {
    int save_errno = errno; /* otherwise ignore it */
    fclose(fp);
    errno = save_errno;
  }
  
  return (ret);
}
USTR_CONF_I_PROTO
int ustr_io_putfilename(struct Ustr **ps1, const char *name, const char *mode)
{ return (ustrp__io_putfilename(0, ps1, name, mode)); }
USTR_CONF_I_PROTO int ustrp_io_putfilename(void *p, struct Ustrp **ps1,
                                           const char *name, const char *mode)
{
  struct Ustr *tmp = &(*ps1)->s;
  int ret = ustrp__io_putfilename(p, &tmp, name, mode);
  *ps1 = USTRP(tmp);
  return (ret);
}
