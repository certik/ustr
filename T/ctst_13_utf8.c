#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make_pool();
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
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 0, &c2b_pos) == 0);
  ASSERT(c2b_pos == 0);

  s1 = USTR1(\x5, "ab\0xy");
  ASSERT(!ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 5);
  ASSERT(ustr_utf8_width(s1) == 0);
  ASSERT(ustr_utf8_len(s1) == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 5, NULL) == 0);
  ASSERT(c2b_pos == 0);
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 5, NULL) == 5);
  ASSERT(c2b_pos == 0);

  s1 = USTR1(\x2, "ab");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 2);
  ASSERT(ustr_utf8_len(s1) == 2);
  ASSERT(ustr_utf8_width(s1) == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 2, NULL) == 2);
  ASSERT(c2b_pos == 0);
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 2, NULL) == 2);
  ASSERT(c2b_pos == 0);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 0, &c2b_pos) == 1);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 0, &c2b_pos) == 0);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 2, 0, &c2b_pos) == 1);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_bytes2chars(s1, 2, 0, &c2b_pos) == 0);
  ASSERT(c2b_pos == 2);

  s1 = USTR1(\x1, "\x90");
  ASSERT(!ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 1);
  ASSERT(ustr_utf8_width(s1) == 0);
  ASSERT(ustr_utf8_len(s1) == 0);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 1);
  ASSERT(c2b_pos == 1);

  s1 = USTR1(\x1, "\b");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 1);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == -1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 1);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 1, &c2b_pos) == 1);
  ASSERT(c2b_pos == 1);

  s1 = USTR1(\x2, "\xc2\x80");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 2);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == -1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 2);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 2, &c2b_pos) == 1);
  ASSERT(c2b_pos == 1);

  s1 = USTR1(\x2, "\xca\x80");
  ASSERT( ustr_utf8_valid(s1));
  ASSERT(ustr_len(s1)      == 2);
  ASSERT(ustr_utf8_len(s1) == 1);
  ASSERT(ustr_utf8_width(s1) == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 1, &c2b_pos) == 2);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 2, &c2b_pos) == 1);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1, 0, &c2b_pos) == 2);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 0, &c2b_pos) == 0);
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
  ASSERT(ustr_utf8_chars2bytes(s1, 1,  4, &c2b_pos) == 10);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1, 10, &c2b_pos) ==  4);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1,  3, &c2b_pos) ==  6);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1,  6, &c2b_pos) ==  3);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1,  2, &c2b_pos) ==  3);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1,  3, &c2b_pos) ==  2);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 1,  1, &c2b_pos) ==  1);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_bytes2chars(s1, 1,  1, &c2b_pos) ==  1);
  ASSERT(c2b_pos == 1);
  ASSERT(ustr_utf8_chars2bytes(s1, 2,  3, &c2b_pos) ==  9);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_bytes2chars(s1, 2,  9, &c2b_pos) ==  3);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 2,  2, &c2b_pos) ==  5);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_bytes2chars(s1, 2,  9, &c2b_pos) ==  3);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 2,  1, &c2b_pos) ==  2);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_bytes2chars(s1, 2,  2, &c2b_pos) ==  1);
  ASSERT(c2b_pos == 2);
  ASSERT(ustr_utf8_chars2bytes(s1, 3,  2, &c2b_pos) ==  7);
  ASSERT(c2b_pos == 4);
  ASSERT(ustr_utf8_bytes2chars(s1, 4,  7, &c2b_pos) ==  2);
  ASSERT(c2b_pos == 3);
  ASSERT(ustr_utf8_chars2bytes(s1, 3,  1, &c2b_pos) ==  3);
  ASSERT(c2b_pos == 4);
  ASSERT(ustr_utf8_bytes2chars(s1, 4,  3, &c2b_pos) ==  1);
  ASSERT(c2b_pos == 3);
  ASSERT(ustr_utf8_chars2bytes(s1, 4,  1, &c2b_pos) ==  4);
  ASSERT(c2b_pos == 7);
  ASSERT(ustr_utf8_bytes2chars(s1, 7,  4, &c2b_pos) ==  1);
  ASSERT(c2b_pos == 4);

  sp1 = USTRP1(\x2, "-\x90");
  ASSERT(!ustrp_utf8_valid(sp1));
  ASSERT(ustrp_len(sp1)      == 2);
  ASSERT(ustrp_utf8_width(sp1) == 0);
  ASSERT(ustrp_utf8_len(sp1) == 1);
  ASSERT(ustrp_utf8_chars2bytes(sp1, 1, 2, &c2b_pos) ==  0);
  ASSERT(c2b_pos == 4);
  ASSERT(ustrp_utf8_chars2bytes(sp1, 1, 1, &c2b_pos) ==  2);
  ASSERT(c2b_pos == 1);
  sp1 = USTRP1(\x1e,"\xc2\xa0\xc2\xa1\xc2\xa2\xc2\xa3\xc2\xa4\xc2\xa5\xc2\xa6"
                    "\xc2\xa7\xc2\xa8\xc2\xa9\xc2\xaa\xc2\xab\xc2\xac\xc2\xad"
                    "\xc2\xae");
  sp2 = USTRP1(\x1e," ¡¢£¤¥¦§¨©ª«¬­®");
  ASSERT( ustrp_utf8_valid(sp1));
  ASSERT( ustrp_utf8_valid(sp2));
  ASSERT(ustrp_len(sp1) == 0x1e);
  ASSERT(ustrp_utf8_len(sp1) == 15);
  ASSERT_PEQ(sp1, sp2);
  ASSERT(ustrp_utf8_chars2bytes(sp1, 1, 15, &c2b_pos) == 0x1e);
  ASSERT(c2b_pos == 1);
  ASSERT(ustrp_utf8_bytes2chars(sp1, 1, 0x1e, &c2b_pos) == 15);
  ASSERT(c2b_pos == 1);
  ASSERT(ustrp_utf8_chars2bytes(sp1, 4, 4, &c2b_pos) == 8);
  ASSERT(c2b_pos == 7);
  ASSERT(ustrp_utf8_bytes2chars(sp1, 7, 8, &c2b_pos) == 4);
  ASSERT(c2b_pos == 4);

  ASSERT(ustrp_spn_cstr_fwd(sp2, " ¡¢ãäåæçèé") == 21);
  ASSERT(ustrp_spn_cstr_fwd(sp2, "ãäåæçèéàáâµ") == 21);
  ASSERT(ustrp_utf8_spn_cstr_fwd(sp2, "ãäåæçèé ¡¢") == 3);
  ASSERT(ustrp_utf8_spn_cstr_rev(sp2, "ãäåæçèé ¡¢") == 0);
  ASSERT(ustrp_utf8_spn_cstr_rev(sp2, "ãäå¬­®æçèé ¡¢") == 3);
  ASSERT(ustrp_utf8_spn_fwd(sp2, USTRP1(\xa, " ¡¢ãäåæçèé")) == 3);
  ASSERT(ustrp_utf8_spn_rev(sp2, USTRP1(\x14, "¡¢ãäåæçèé ")) == 0);
  ASSERT(ustrp_utf8_spn_rev(sp2, USTRP1(\x1a, "¡¢ãäå¬­®æçèé ")) == 3);
  ASSERT(ustrp_cspn_cstr_fwd(sp2, "àáâãäåæçèé") == 1);
  ASSERT(ustrp_cspn_cstr_fwd(sp2, "àáâãäåæçèéµ") == 0);
  ASSERT(ustrp_cspn_cstr_rev(sp2, "àáâãäåæçèéµ") == 1);
  ASSERT(ustrp_cspn_cstr_rev(sp2, "àáâãäåæçèéµî") == 0);
  ASSERT(ustrp_utf8_cspn_fwd(sp2, USTRP1(\xb, "àáâãäåæçèéµ")) == 15);
  ASSERT(ustrp_utf8_cspn_rev(sp2, USTRP1(\xb, "àáâãäåæçèéµ")) == 15);
  ASSERT(ustrp_utf8_cspn_cstr_fwd(sp2, "©®¢") == 2);
  ASSERT(ustrp_utf8_cspn_cstr_fwd(sp2, "¢©®") == 2);
  ASSERT(ustrp_utf8_cspn_cstr_rev(sp2, "©¢®") == 0);
  ASSERT(ustrp_utf8_cspn_cstr_rev(sp2, "©¢") == 5);

  ASSERT(ustr_utf8_spn_cstr_fwd(&sp2->s, "ãäåæçèé ¡¢") == 3);
  ASSERT(ustr_utf8_spn_cstr_rev(&sp2->s, "ãäåæçèé ¡¢") == 0);
  ASSERT(ustr_utf8_spn_cstr_rev(&sp2->s, "ãäå¬­®æçèé ¡¢") == 3);

  ASSERT(ustr_utf8_cspn_cstr_fwd(&sp2->s, "©®¢") == 2);
  ASSERT(ustr_utf8_cspn_cstr_fwd(&sp2->s, "¢©®") == 2);
  ASSERT(ustr_utf8_cspn_cstr_rev(&sp2->s, "©¢®") == 0);
  ASSERT(ustr_utf8_cspn_cstr_rev(&sp2->s, "©¢") == 5);

  ASSERT(ustr_srch_cstr_fwd(&sp2->s, "¢") == 5);
  ASSERT(ustr_srch_cstr_rev(&sp2->s, "¢") == 5);

  ASSERT(ustr_srch_subustr_fwd(&sp2->s, USTR1(\x4, "¢£"), 1, 4) ==  5);
  ASSERT(ustr_srch_subustr_fwd(&sp2->s, USTR1(\x4, "¡¢"), 1, 4) ==  3);
  ASSERT(ustr_srch_subustr_rev(&sp2->s, USTR1(\x4, "¢£"), 1, 4) ==  5);
  ASSERT(ustr_srch_subustr_rev(&sp2->s, USTR1(\x4, "¡¢"), 1, 4) ==  3);
  ASSERT(ustr_srch_subustr_fwd(&sp2->s, USTR1(\x4, "¢©"), 3, 2) == 19);
  ASSERT(ustr_srch_subustr_fwd(&sp2->s, USTR1(\x4, "©¢"), 1, 2) == 19);
  ASSERT(ustr_srch_subustr_rev(&sp2->s, USTR1(\x4, "¢©"), 3, 2) == 19);
  ASSERT(ustr_srch_subustr_rev(&sp2->s, USTR1(\x4, "©¢"), 1, 2) == 19);
  ASSERT(ustr_srch_subustr_fwd(&sp2->s, USTR1(\x4, "¢©"), 1, 2) ==  5);
  ASSERT(ustr_srch_subustr_fwd(&sp2->s, USTR1(\x4, "©¢"), 3, 2) ==  5);
  ASSERT(ustr_srch_subustr_rev(&sp2->s, USTR1(\x4, "¢©"), 1, 2) ==  5);
  ASSERT(ustr_srch_subustr_rev(&sp2->s, USTR1(\x4, "©¢"), 3, 2) ==  5);

  if (!USTR_DEBUG) {
  ASSERT(ustr_srch_subustr_fwd(&sp2->s, USTR1(\x4, "©¢"), 3, 8) == 1);
  ASSERT(ustr_srch_subustr_rev(&sp2->s, USTR1(\x4, "¢©"), 1, 8) == 0x1e);
  }

  /* hack ... invalid utf-8 cstr, ask to go backwards */
  ASSERT(ustr_utf8_cspn_cstr_fwd(USTR1(\x4, "\x80\x80\x80\x80"), "©¢") == 1);
  ASSERT(ustr_utf8_cspn_cstr_rev(USTR1(\x4, "\x80\x80\x80\x80"), "©¢") == 0);
  ASSERT(ustr_utf8_cspn_cstr_fwd(USTR1(\x5, ".\x80\x80\x80\x80"), "©¢") == 1);
  ASSERT(ustr_utf8_cspn_cstr_rev(USTR1(\x5, ".\x80\x80\x80\x80"), "©¢") == 1);

  c2b_pos = 99;
  ASSERT(ustr_utf8_bytes2chars(USTR1(\x4, "\x80\x80\x80\x80"), 1, 1, &c2b_pos) == 0);
  ASSERT(c2b_pos == 99);
  ASSERT(ustr_utf8_bytes2chars(USTR1(\x4, "\x80\x80\x80\x80"), 2, 1, &c2b_pos) == 0);
  ASSERT(c2b_pos == 99);
  ASSERT(ustr_utf8_bytes2chars(USTR1(\x4, "\x80\x80\x80\x80"), 3, 0, &c2b_pos) == 0);
  ASSERT(c2b_pos == 99);
  if (!USTR_DEBUG)
  ASSERT(ustr_utf8_bytes2chars(USTR1(\x4, "\x80\x80\x80\x80"), 8, 0, &c2b_pos) == 0);
  ASSERT(c2b_pos == 99);
  ASSERT(ustr_utf8_bytes2chars(USTR1(\x5, ".\x80\x80\x80\x80"), 1, 1, &c2b_pos) == 1);
  ASSERT(c2b_pos ==  1); c2b_pos = 99;
  ASSERT(ustr_utf8_bytes2chars(USTR1(\x5, ".\x80\x80\x80\x80"), 1, 2, &c2b_pos) == 1);
  ASSERT(c2b_pos ==  1); c2b_pos = 99;
  ASSERT(ustr_utf8_bytes2chars(USTR1(\x5, ".\x80\x80\x80\x80"), 3, 1, &c2b_pos) == 1);
  ASSERT(c2b_pos ==  1);

  ustr_pool_free(pool);

#ifdef USTR__UTF8_WCHAR /* internal testing for utf8 functions */
  ASSERT(ustr__utf8_mk_wcwidth(0) == 0);
#endif

  return (EXIT_SUCCESS);
}
