/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SPLIT_H
#error " Include ustr-split.h before this file."
#endif

#if !defined(USTR_FMT_INTERNAL_H) && !defined(USTR_IO_H)
#include <stdio.h>
#endif

USTR_CONF_i_PROTO
struct Ustr *ustrp__split_buf(struct Ustr_pool *p,
                              const struct Ustr *s1, size_t *poff, 
                              const void *sep, size_t slen, unsigned int flags)
{
  size_t len = ustr_len(s1);
  size_t off = *poff;
  size_t found_pos = 0;
  size_t ret_len   = 0;
  
  USTR_ASSERT(ustr_assert_valid(s1));

  USTR_ASSERT_RET(off <= len, USTR_NULL);

  if (!slen || (off == len))
    return (USTR_NULL);

  /* Separator not found, just return the rest of the string 
   * the user will have to interpret whether this is an error condition */
  if (!(found_pos = ustr_srch_buf_fwd(s1, off, sep, slen)))
  {
    ret_len = len - off;
    *poff   = len;
    goto copy_buf; /* No more tokens, return remaining */
  }

  /* If we don't wish to return blanks or separators, we could get a situation
   * where we have "a,,,,,,,,b" and sep="," in which case the only two tokens
   * we need to return are 'a' and 'b' -- so skip all of the middle ones */
  if (((found_pos - 1) == off) &&
      !(flags & (USTR_FLAG_SPLIT_RET_SEP | USTR_FLAG_SPLIT_RET_NON)))
    /* The found position is one away from the start position 
     * and we are not at the end of the string yet */
  {
    *poff += slen;
    return (ustrp__split_buf(p, s1, poff, sep, slen, flags));
  }

  /* Make sure the offset for the next call is after the delimiter we found 
   * this time */
  *poff = found_pos + slen - 1;
  
  ret_len = (found_pos - 1) - off;
  
  /* Include sep in the substring we are about to return (if we are not at 
   * end of line) */
  if (flags & USTR_FLAG_SPLIT_RET_SEP)
    ret_len += slen;
  
 copy_buf:
  if (flags & USTR_FLAG_SPLIT_KEEP_CONF)
    return (ustrp__dup_subustr(p, s1, off + 1, ret_len));

  return (ustrp__dupx_buf(p, USTR__DUPX_DEF, ustr_cstr(s1) + off, ret_len));
}
USTR_CONF_I_PROTO
struct Ustr *ustr_split_buf(const struct Ustr *s1, size_t *off, 
                            const void *sep, size_t slen, unsigned int flags)
{ return (ustrp__split_buf(0, s1, off, sep, slen, flags)); }

USTR_CONF_I_PROTO
struct Ustr *ustr_split(const struct Ustr *s1, size_t *off, 
                        const struct Ustr *sep, unsigned int flags)
{
  USTR_ASSERT(ustr_assert_valid(sep));
  return (ustrp__split_buf(0, s1, off, ustr_cstr(sep), ustr_len(sep), flags));
}

