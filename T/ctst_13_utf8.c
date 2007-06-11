#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make();
  Ustrp *sp1 = USTRP1(\x3, "\xE2\x80\xA0");
  Ustrp *sp2 = USTRP1(\x3, "†");
  size_t c2b_pos = 1;

  ASSERT_PEQ(sp1, sp2);
  ASSERT( ustrp_utf8_valid(sp1));
  ASSERT(ustrp_len(sp2)      == 3);
  ASSERT(ustrp_utf8_len(sp2) == 1);
  ASSERT(ustrp_utf8_width(sp2) == 1);

  s1 = USTR("");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 0);
  ASSERT(ustr_utf8_len(s1) == 0);
  ASSERT(ustr_utf8_width(s1) == 0);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 0, &c2b_pos) == 0);
  ASSERT(c2b_pos == 0);

  s1 = USTR1(\x5, "ab\0xy");
  ASSERT(!ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 5);
  ASSERT(ustr_utf8_len(s1) == 0);
  ASSERT(ustr_utf8_width(s1) == 0);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 5, NULL) == 0);
  ASSERT(c2b_pos == 0);

  s1 = USTR1(\x2, "ab");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 2);
  ASSERT(ustr_utf8_len(s1) == 2);
  ASSERT(ustr_utf8_width(s1) == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 2, NULL) == 2);
  ASSERT(c2b_pos == 0);

  s1 = USTR1(\x1, "\x90");
  ASSERT(!ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 1);
  ASSERT(ustr_utf8_len(s1) == 0);
  ASSERT(ustr_utf8_width(s1) == 0);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 0);
  ASSERT(c2b_pos == 0);

  s1 = USTR1(\x1, "\b");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 1);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == -1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 1);
  ASSERT(c2b_pos == 1);

  s1 = USTR1(\x2, "\xc2\x80");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 2);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == -1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 2);
  ASSERT(c2b_pos == 1);

  s1 = USTR1(\x2, "\xca\x80");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 2);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 2);
  ASSERT(c2b_pos == 1);

  s1 = USTR1(\x2, "\xcd\x80");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 2);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == 0);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 2);
  ASSERT(c2b_pos == 1);

  s1 = USTR1(\x3, "\xe1\x83\x8c");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 3);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 3);
  ASSERT(c2b_pos == 1);

  s1 = USTR1(\x4, "\xf1\x83\x8c\x89");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 4);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 4);
  ASSERT(c2b_pos == 1);

 /* chars2bytes */
  s1 = USTR1(\xa, "a\xca\x80\xe1\x83\x8c\xf1\x83\x8c\x89");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 10);
  ASSERT(ustr_utf8_len(s1) ==  4);
  ASSERT(ustr_utf8_width(s1) == 4);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 4, &c2b_pos) == 10);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 3, &c2b_pos) ==  6);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 2, &c2b_pos) ==  3);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) ==  1);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 2, 3, &c2b_pos) ==  9);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 2, 2, &c2b_pos) ==  5);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 2, 1, &c2b_pos) ==  2);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 3, 2, &c2b_pos) ==  7);
  ASSERT(c2b_pos == 4);
  ASSERT(ustr_utf8_chars2bytes(s1, 3, 1, &c2b_pos) ==  3);
  ASSERT(c2b_pos == 4);
  ASSERT(ustr_utf8_chars2bytes(s1, 4, 1, &c2b_pos) ==  4);
  ASSERT(c2b_pos == 7);

  sp1 = USTRP1(\x2, "-\x90");
  ASSERT(!ustrp_utf8_valid(sp1));
  ASSERT(ustrp_len(sp1)      == 2);
  ASSERT(ustrp_utf8_len(sp1) == 0);
  ASSERT(ustrp_utf8_width(sp1) == 0);
  ASSERT(ustrp_utf8_chars2bytes(sp1, 1, 2, &c2b_pos) ==  0);
  ASSERT(c2b_pos == 7);

  ustr_pool_free(pool);

#ifdef USTR__UTF8_WCHAR /* internal testing for utf8 functions */
  ASSERT(ustr__utf8_mk_wcwidth(0) == 0);
#endif

  return (EXIT_SUCCESS);
}
