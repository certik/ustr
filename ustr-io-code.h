/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_IO_H
#error " You should have already included ustr-io.h, or just include ustr.h."
#endif

USTR_CONF_i_PROTO
int ustrp__io_get(void *p, struct Ustr **ps1, FILE *fp, size_t minlen)
{
  size_t olen = ustr_len(*ps1);
  size_t ret  = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1) && fp);

  if (!minlen)
    return (USTR_TRUE);
  
  if (!ustrp__add_undef(p, ps1, minlen))
    return (USTR_FALSE);

  ret = fread(ustr_wstr(*ps1) + olen, 1, minlen, fp);
  if (ret < minlen)
    ustrp__del(p, ps1, minlen - ret);
  
  return (ret > 0);
}
USTR_CONF_I_PROTO
int ustr_io_get(struct Ustr **ps1, FILE *fp, size_t minlen)
{ return (ustrp__io_get(0, ps1, fp, minlen)); }
USTR_CONF_I_PROTO
int ustrp_io_get(void *p, struct Ustrp **ps1, FILE *fp, size_t minlen)
{ return (ustrp__io_get(p, USTR__PPTR(ps1), fp, minlen)); }

USTR_CONF_i_PROTO int ustrp__io_getfile(void *p, struct Ustr **ps1, FILE *fp)
{
  const size_t blksz = (1024 * 8) - (1 + 8 + 8 + 8 + sizeof(USTR_END_ALOCDx));
  size_t num = blksz;
  
  while (ustrp__io_get(p, ps1, fp, num))
  { /* round up... */
    size_t sz   = ustr_size(*ps1);
    size_t clen = ustr_len(*ps1);

    num = blksz;
    if (sz > (clen + num))
      num = sz - clen;
  }

  return (feof(fp));
}
USTR_CONF_I_PROTO
int ustr_io_getfile(struct Ustr **ps1, FILE *fp)
{ return (ustrp__io_getfile(0, ps1, fp)); }
USTR_CONF_I_PROTO
int ustrp_io_getfile(void *p, struct Ustrp **ps1, FILE *fp)
{ return (ustrp__io_getfile(p, USTR__PPTR(ps1), fp)); }

USTR_CONF_i_PROTO
int ustrp__io_getfilename(void *p, struct Ustr **ps1, const char *name)
{
  FILE *fp = fopen(name, "rb");
  int ret = USTR_FALSE;
  
  if (!fp)
    return (USTR_FALSE);

  ret = ustrp__io_getfile(p, ps1, fp);
  
  fclose(fp);
  
  return (ret);
}
USTR_CONF_I_PROTO
int ustr_io_getfilename(struct Ustr **ps1, const char *name)
{ return (ustrp__io_getfilename(0, ps1, name)); }
USTR_CONF_I_PROTO
int ustrp_io_getfilename(void *p, struct Ustrp **ps1, const char *name)
{ return (ustrp__io_getfilename(p, USTR__PPTR(ps1), name)); }

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
{ return (ustrp__io_getline(p, USTR__PPTR(ps1), fp)); }

USTR_CONF_i_PROTO
int ustrp__io_put(void *p, struct Ustr **ps1, FILE *fp, size_t beglen)
{
  size_t ret = 0;
  size_t clen = ustr_len(*ps1);
  struct Ustr *next = NULL;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1) && fp);

  USTR_ASSERT_RET(beglen <= clen, USTR_FALSE);
  
  if (!beglen)
    return (USTR_TRUE);

  if (!ustr_owner(*ps1) && (beglen != clen))
  {
    const char *ptr = ustr_cstr(*ps1) + beglen;
    size_t      len =      clen       - beglen;
    
    if (!(next = ustrp__dupx_buf(p, USTR__DUPX_FROM(*ps1), ptr, len)))
    {
      ustr_setf_enomem_err(*ps1);
      return (USTR_FALSE);
    }
  }
  
  ret = fwrite(ustr_cstr(*ps1), 1, beglen, fp);

  if (next && (ret == beglen))
    ustrp__sc_free2(p, ps1, next);
  else if ((ret == beglen) && (beglen == clen))
    ustrp__sc_del(p, ps1);
  else
    ustrp__del_subustr(p, ps1, 1, ret); /* don't need to check */
  
  return (ret == beglen);
}
USTR_CONF_I_PROTO
int ustr_io_put(struct Ustr **ps1, FILE *fp, size_t beglen)
{ return (ustrp__io_put(0, ps1, fp, beglen)); }
USTR_CONF_I_PROTO
int ustrp_io_put(void *p, struct Ustrp **ps1, FILE *fp, size_t beglen)
{ return (ustrp__io_put(p, USTR__PPTR(ps1), fp, beglen)); }

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
{ return (ustrp__io_putline(p, USTR__PPTR(ps1), fp, beglen)); }

USTR_CONF_i_PROTO int ustrp__io_putfilename(void *p, struct Ustr **ps1,
                                            const char *name, const char *mode)
{
  FILE *fp = fopen(name, mode);
  int ret = USTR_FALSE;
  int fpret = USTR_FALSE;
  
  if (!fp)
    return (USTR_FALSE);
  
  ret = ustrp__io_put(p, ps1, fp, ustr_len(*ps1));
  fpret = fclose(fp);

  if (ret) /* if everything OK, defer to the fclose() return value */
    ret = !fpret;
  
  return (ret);
}
USTR_CONF_I_PROTO
int ustr_io_putfilename(struct Ustr **ps1, const char *name, const char *mode)
{ return (ustrp__io_putfilename(0, ps1, name, mode)); }
USTR_CONF_I_PROTO int ustrp_io_putfilename(void *p, struct Ustrp **ps1,
                                           const char *name, const char *mode)
{ return (ustrp__io_putfilename(p, USTR__PPTR(ps1), name, mode)); }
