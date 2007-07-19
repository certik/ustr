/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SPLIT_H
#error " Include ustr-split.h before this file."
#endif
#include <stdio.h>
USTR_CONF_i_PROTO
struct Ustr *ustr__split(const struct Ustr *target, size_t *off, 
                            const struct Ustr *sep, unsigned int flags)
{
  USTR_ASSERT(ustr_assert_valid(target) && ustr_assert_valid(sep));
  size_t len = ustr_len(target);
  size_t slen = ustr_len(sep);
  size_t start_pos = *off;
  unsigned int eol=0; /* End of Line flag -- true when there
                         are no more tokens left in this string */
  if (!len || !slen || start_pos > len - 1)
    return USTR_NULL;

  size_t found_pos = ustr_srch_fwd(target, start_pos, sep);
  /* separator not found, just return the rest of the string 
   *    the user will have to interpret whether this is an error condition */
  if (!found_pos)
  {
    found_pos = len + 1; 
    eol=1;
    goto copy_buf;
  }

  /* if we don't wish to return blanks or separators, we could get a situation
   * where we have "a,,,,,,,,b" and sep="," in which case the only two tokens
   * we need to return are 'a' and 'b' -- so skip all of the middle ones */
  if (!(flags & USTR_SPLIT_RET_SEP)) 
  {
    if (!(flags & USTR_SPLIT_RET_NON)) {
      /* The found position is one away from the start position 
       *    and we are not at the end of the string yet */
      while ((found_pos - 1 == start_pos) && (start_pos + slen < len))
      {
        start_pos += slen;
        found_pos = ustr_srch_fwd(target, start_pos, sep);
        if (!found_pos)
        {
          eol=1;
          found_pos=len + 1;
          break;
        }
      }
    }
  }

    /* next character is a separator, and we DO want blank tokens */
  if (!(flags & USTR_SPLIT_RET_SEP) && (flags & USTR_SPLIT_RET_NON) && 
      (found_pos - start_pos) == 1) 
  {
    (*off) += slen;
    return USTR("");
  }

copy_buf:
  /* make sure the offset for the next call is after the delimiter we found 
   * this time */
  *off = (found_pos - 1) + slen;

  /* include sep in the substring we are about to return (if we are not at 
   * end of line) */
  if (!eol && flags & USTR_SPLIT_RET_SEP)
    found_pos += slen;

  const char *buf = ustr_cstr(target) + start_pos;
  size_t clen = (found_pos-1) - start_pos;
  if (clen)
  {
    if (!(flags & USTR_SPLIT_KEEP_CONF))
      return ustr_dup_buf(buf, clen); 
    else
     return ustr_dupx_buf(USTR__DUPX_FROM(target),buf,clen);
  }
  else
    return USTR_NULL;
}

USTR_CONF_I_PROTO
struct Ustr *ustr_split(const struct Ustr *target, size_t *off, 
                        const struct Ustr *sep, unsigned int flags)
{
	return ustr__split(target, off, sep, flags);	
}

