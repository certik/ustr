#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  struct Ustr *s3 = NULL;
  struct Ustr *s4 = NULL;
  int num = -1;
  
  assert(!USTR_CONF_USE_DYNAMIC_CONF ||
         ustr_cntl_opt(USTR_CNTL_OPT_SET_REF_BYTES, 1));
  /* move to the new "default" conf */
  ustr_sc_free2(&s2, ustr_dup_buf(ustr_cstr(s2), ustr_len(s2)));

  s3 = ustr_dup_cstr("s3 abcd s2");
  s4 = ustr_dup_empty(); /* always allocs */
  
  ASSERT(s2);
  ASSERT(s3);
  ASSERT(s4);
  ASSERT(ustr_cmp_eq(s1, s4));
  
  ASSERT(ustr_len(s1)  ==  0);
  ASSERT(ustr_len(s2)  ==  2);
  ASSERT(ustr_len(s3)  == 10);
  ASSERT(ustr_len(s4)  ==  0);
  
  ASSERT(ustr_size(s1) ==  0);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s2) ==  2);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s3) == 12);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s4) ==  0);

  ASSERT(ustr_srch_fwd(s3, 0, s2) == 9);
  ASSERT(ustr_srch_rev(s3, 0, s2) == 9);
  ASSERT(ustr_srch_fwd(s2, 0, s3) == 0);
  ASSERT(ustr_srch_rev(s2, 0, s3) == 0);
  
  ASSERT(ustr_add_cstr(&s2, "x"));
  ASSERT(ustr_len(s2)  ==   3);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s2) ==   4);
  ASSERT(ustr_add_cstr(&s2, "y"));
  ASSERT(ustr_len(s2)  ==   4);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s2) ==   4);
  ASSERT(ustr_add_cstr(&s2, "z"));
  ASSERT(ustr_len(s2)  ==   5);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s2) ==   8);
  ASSERT(ustr_add_rep_chr(&s2, '-', 11));
  ASSERT(ustr_len(s2)  ==  16);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s2) ==  20);
  ASSERT(ustr_cmp_cstr_eq(s2,   "s2xyz-----------"));
  ASSERT(!strcmp(ustr_cstr(s2), "s2xyz-----------"));
  
  ASSERT(ustr_srch_fwd(s3, 0, s2) == 0);
  ASSERT(ustr_srch_rev(s3, 0, s2) == 0);
  ASSERT(ustr_srch_fwd(s2, 0, s3) == 0);
  ASSERT(ustr_srch_rev(s2, 0, s3) == 0);

  /* NOTE: Using system *printf, so can't use %zu as Solaris is retarded */
  ASSERT(ustr_add_fmt(&s1, "%s abcd %13.100s %d %c %lu%n",
                      "------abc------", "", 42, 0,
                      (unsigned long)ustr_len(s3), &num) != -1);
  ASSERT((unsigned)num == ustr_len(s1));
  ASSERT(42  == num);
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(44  == ustr_size(s1));

  ASSERT(ustr_add_fmt(&s4, "%2$d%1$u", 2, 4));

  ASSERT(ustr_srch_cstr_fwd(s1, 0, "abcd") == 17);
  ASSERT(ustr_srch_cstr_rev(s1, 0, "abcd") == 17);
  ASSERT(ustr_srch_cstr_fwd(s1, 0, "abc")  ==  7);
  ASSERT(ustr_srch_cstr_rev(s1, 0, "abc")  == 17);
  ASSERT(ustr_srch_cstr_fwd(s1, 0, "10")  == 41);
  ASSERT(ustr_srch_cstr_rev(s1, 0, "10")  == 41);
  ASSERT(ustr_srch_chr_fwd(s1, 0, 0)  == 39);
  ASSERT(ustr_srch_chr_rev(s1, 0, 0)  == 39);
  ASSERT(ustr_srch_fwd(s1, 0, s4) == 36);

  ASSERT(ustr_srch_cstr_fwd(s1,  1, "abcd") == 17);
  ASSERT(ustr_srch_cstr_rev(s1,  1, "abcd") == 17);
  ASSERT(ustr_srch_cstr_fwd(s1, 10, "abcd") == 17);
  ASSERT(ustr_srch_cstr_rev(s1, 10, "abcd") == 17);
  
  ASSERT(ustr_srch_cstr_fwd(s1,  0, " ") == 16);
  ASSERT(ustr_srch_cstr_fwd(s1, 10, " ") == 16);
  ASSERT(ustr_srch_cstr_fwd(s1, 16, " ") == 21);
  ASSERT(ustr_srch_cstr_fwd(s1, 20, " ") == 21);
  ASSERT(ustr_srch_cstr_fwd(s1, 21, " ") == 22);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 21, ' ', 1) == 22);
  ASSERT(ustr_srch_cstr_fwd(s1, 21, "  ") == 22);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 21, ' ', 2) == 22);
  ASSERT(ustr_srch_cstr_fwd(s1, 21, "   ") == 22);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 21, ' ', 3) == 22);
  ASSERT(ustr_srch_cstr_fwd(s1, 21, "    ") == 22);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 21, ' ', 4) == 22);
  ASSERT(ustr_srch_cstr_fwd(s1, 21, "     ") == 22);
  ASSERT(ustr_srch_rep_chr_fwd(s1, 21, ' ', 5) == 22);
  
  ASSERT(ustr_srch_cstr_fwd(s1,  0, "a") ==  7);
  ASSERT(ustr_srch_cstr_fwd(s1,  6, "a") ==  7);
  ASSERT(ustr_srch_cstr_fwd(s1,  7, "a") == 17);
  ASSERT(ustr_srch_cstr_fwd(s1, 16, "a") == 17);
  ASSERT(ustr_srch_cstr_fwd(s1, 17, "a") ==  0);
  
  ASSERT(ustr_srch_cstr_rev(s1, 0, "a") == 17);
  ASSERT(ustr_srch_cstr_rev(s1, ustr_len(s1) - 17, "a") == 17);
  ASSERT(ustr_srch_cstr_rev(s1, ustr_len(s1) - 16, "a") ==  7);
  ASSERT(ustr_srch_cstr_rev(s1, ustr_len(s1) -  7, "a") ==  7);
  ASSERT(ustr_srch_cstr_rev(s1, ustr_len(s1) -  6, "a") ==  0);
  ASSERT(ustr_srch_cstr_rev(s1, ustr_len(s1) -  1, "a") ==  0);

  /* srch_case */
  ASSERT(ustr_srch_case_cstr_fwd(s1,  0, " ") == 16);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 10, " ") == 16);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 16, " ") == 21);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 20, " ") == 21);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 21, " ") == 22);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 21, ' ', 1) == 22);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 21, "  ") == 22);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 21, ' ', 2) == 22);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 21, "   ") == 22);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 21, ' ', 3) == 22);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 21, "    ") == 22);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 21, ' ', 4) == 22);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 21, "     ") == 22);
  ASSERT(ustr_srch_case_rep_chr_fwd(s1, 21, ' ', 5) == 22);
  
  ASSERT(ustr_srch_case_cstr_fwd(s1,  0, "a") ==  7);
  ASSERT(ustr_srch_case_cstr_fwd(s1,  6, "a") ==  7);
  ASSERT(ustr_srch_case_cstr_fwd(s1,  7, "a") == 17);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 16, "a") == 17);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 17, "a") ==  0);
  ASSERT(ustr_srch_case_cstr_fwd(s1,  0, "A") ==  7);
  ASSERT(ustr_srch_case_cstr_fwd(s1,  6, "A") ==  7);
  ASSERT(ustr_srch_case_cstr_fwd(s1,  7, "A") == 17);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 16, "A") == 17);
  ASSERT(ustr_srch_case_cstr_fwd(s1, 17, "A") ==  0);
  
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "a") == 17);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) - 17, "a") == 17);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) - 16, "a") ==  7);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) -  7, "a") ==  7);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) -  6, "a") ==  0);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) -  1, "a") ==  0);
  ASSERT(ustr_srch_case_cstr_rev(s1, 0, "A") == 17);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) - 17, "A") == 17);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) - 16, "A") ==  7);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) -  7, "A") ==  7);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) -  6, "A") ==  0);
  ASSERT(ustr_srch_case_cstr_rev(s1, ustr_len(s1) -  1, "A") ==  0);

  if (!USTR_DEBUG)
  ASSERT(ustr_srch_cstr_fwd(s1, ustr_len(s1), "a") ==  0);
  if (!USTR_DEBUG)
  ASSERT(ustr_srch_cstr_rev(s1, ustr_len(s1), "a") ==  0);

  /*  puts(ustr_cstr(s4)); */
  
  ustr_sc_free(&s3);

  ASSERT((s3 = ustr_dup(s4)));
  ASSERT(ustr_add_fmt(&s4, "x"));

  ustr_sc_free(&s4);
  ustr_sc_free(&s3);

  /*
  ASSERT(!ustr_assert_valid(USTR1(\x000F, "123456789 123456")));
  ASSERT(!ustr_assert_valid(USTR1(\x000F, "123456789 1234\0xxx"))); */
  ASSERT( ustr_assert_valid(USTR1(\x000F, "123456789 12345")));

  /*    ASSERT(!ustr_assert_valid(USTR1(\x000F, "123456789 12345\0xxx")));  */
  
  s3 = ustr_dupx(0, 2, 0, 1, USTR1(\x000F, "123456789 12345"));

  ASSERT(ustr_cmp_cstr_eq(s3, "123456789 12345"));
  ASSERT(ustr_cmp_eq(s3, USTR1(\x000F, "123456789 12345")));
  ASSERT(!ustr_ro(s3));
  if (!USTR_CONF_USE_EOS_MARK)
  ASSERT(ustr_size(s3) == 19);
  
  ustr_sc_free(&s3);
  
  return (EXIT_SUCCESS);
}

