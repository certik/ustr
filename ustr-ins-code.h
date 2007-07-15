/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_INS_H
#error " Include ustr-ins.h before this file."
#endif


USTR_CONF_I_PROTO
int ustrp__ins_undef(struct Ustr_pool*p,struct Ustr **ps1,size_t pos,size_t len)
{ /* basically the opposite of ustr_del_subustr() */
  struct Ustr *s1  = USTR_NULL;
  struct Ustr *ret = USTR_NULL;
  size_t clen = 0;
  size_t nlen = 0;
  size_t sz   = 0;
  size_t oh   = 0;
  size_t osz  = 0;
  size_t nsz  = 0;
  int alloc = USTR_FALSE;
  const char *ocstr = 0;

  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (!len)
    return (USTR_TRUE);

  s1   = *ps1;
  clen = ustr_len(s1);
  USTR_ASSERT_RET(pos <= clen, USTR_FALSE);
  if (pos-- == clen) /* inserting at the end */
    return (ustrp__add_undef(p, ps1, len));
  
  if ((nlen = clen + len) < clen) /* overflow */
    goto fail_enomem;

  if (ustr__rw_add(*ps1, nlen, &sz, &oh, &osz, &nsz, &alloc))
  {
    char *ptr = ustr_wstr(s1);
    
    if (!ustrp__add_undef(p, ps1, len))
      return (USTR_FALSE);

    memmove(ptr + pos + len, ptr + pos, (nlen - pos));

    USTR_ASSERT(ustr_assert_valid(*ps1));
    return (USTR_TRUE);
  }
    
  if (!(ret = ustrp__dupx_undef(p, USTR__DUPX_FROM(s1), nlen)))
    goto fail_enomem;

  ocstr = ustr_cstr(s1);

  USTR_ASSERT(pos || (clen - pos)); /* can be both */
  
  ustr__memcpy(ret, 0,         ocstr,              pos);
  ustr__memcpy(ret, pos + len, ocstr + pos, clen - pos);

  ustrp__sc_free2(p, ps1, ret);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  return (USTR_TRUE);

 fail_enomem:
  ustr_setf_enomem_err(s1);
  return (USTR_FALSE);
}
USTR_CONF_I_PROTO int ustr_ins_undef(struct Ustr **ps1, size_t pos, size_t len)
{ return (ustrp__ins_undef(0, ps1, pos, len)); }
USTR_CONF_I_PROTO
int ustrp_ins_undef(struct Ustr_pool *p,struct Ustrp**ps1,size_t pos,size_t len)
{ return (ustrp__ins_undef(p, USTR__PPTR(ps1), pos, len)); }

USTR_CONF_i_PROTO int ustrp__ins_buf(struct Ustr_pool *p, struct Ustr **ps1,
                                     size_t pos, const void *buf, size_t len)
{
  if (!ustrp__ins_undef(p, ps1, pos, len))
    return (USTR_FALSE);
  --pos;
  
  ustr__memcpy(*ps1, pos, buf, len);

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO
int ustr_ins_buf(struct Ustr **ps1, size_t pos, const void *buf, size_t len)
{ return (ustrp__ins_buf(0, ps1, pos, buf, len)); }
USTR_CONF_I_PROTO int ustrp_ins_buf(struct Ustr_pool *p, struct Ustrp **ps1,
                                    size_t pos, const void *buf, size_t len)
{ return (ustrp__ins_buf(p, USTR__PPTR(ps1), pos, buf, len)); }

USTR_CONF_i_PROTO int ustrp__ins(struct Ustr_pool *p, struct Ustr **ps1,
                                 size_t pos, const struct Ustr *s2)
{ /* opts needed? */
  return (ustrp__ins_buf(p, ps1, pos, ustr_cstr(s2), ustr_len(s2)));
}
USTR_CONF_I_PROTO
int ustr_ins(struct Ustr **ps1, size_t pos, const struct Ustr *s2)
{ return (ustrp__ins(0, ps1, pos, s2)); }
USTR_CONF_I_PROTO int ustrp_ins(struct Ustr_pool *p, struct Ustrp **ps1,
                                size_t pos, const struct Ustrp *s2)
{ return (ustrp__ins(p, USTR__PPTR(ps1), pos, &s2->s)); }

USTR_CONF_i_PROTO
int ustrp__ins_subustr(struct Ustr_pool *p, struct Ustr **ps1, size_t pos1,
                       const struct Ustr *s2, size_t pos2, size_t len2)
{
  if (!ustr_assert_valid_subustr(s2, pos2, len2))
    return (USTR_FALSE);
  --pos2;
  
  return (ustrp__ins_buf(p, ps1, pos1, ustr_cstr(s2) + pos2, len2));
}
USTR_CONF_I_PROTO
int ustr_ins_subustr(struct Ustr **ps1, size_t pos1,
                     const struct Ustr *s2, size_t pos2, size_t len2)
{ return (ustrp__ins_subustr(0, ps1, pos1, s2, pos2, len2)); }
USTR_CONF_I_PROTO
int ustrp_ins_subustrp(struct Ustr_pool *p, struct Ustrp **ps1, size_t pos1,
                       const struct Ustrp *s2, size_t pos2, size_t len2)
{ return (ustrp__ins_subustr(p, USTR__PPTR(ps1), pos1, &s2->s, pos2, len2)); }

USTR_CONF_i_PROTO int ustrp__ins_rep_chr(struct Ustr_pool *p, struct Ustr **ps1,
                                         size_t pos, char chr, size_t len)
{
  if (!ustrp__ins_undef(p, ps1, pos, len))
    return (USTR_FALSE);
  --pos;
  
  ustr__memset(*ps1, pos, chr, len);

  return (USTR_FALSE);
}
USTR_CONF_I_PROTO
int ustr_ins_rep_chr(struct Ustr **ps1, size_t pos, char chr, size_t len)
{ return (ustrp__ins_rep_chr(0, ps1, pos, chr, len)); }
USTR_CONF_I_PROTO int ustrp_ins_rep_chr(struct Ustr_pool *p, struct Ustrp **ps1,
                                        size_t pos, char chr, size_t len)

{ return (ustrp__ins_rep_chr(p, USTR__PPTR(ps1), pos, chr, len)); }
