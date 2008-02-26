#include "tst.h"

static const char *rf = __FILE__;

#if 0
#	include <stdio.h>
#	define PRINT_RESULT(x) printf("'%s'\n",ustr_cstr(x))
#else
#	define PRINT_RESULT(x)  /* do nothing */
#endif

int tst(void)
{
  const char *str = "the quick brown fox";
  Ustr *a = ustr_dup_cstr(str);
  const char *repl_cstr = "smart";
  Ustr *repl = ustr_dup_cstr(repl_cstr);
  Ustr *c;
  Ustr *blank;
  Ustr *same_len_str;
  Ustr *d;
  Ustr *needle;        
  const char *newstr;
  Ustr *ro = USTR1(\x4, "abcd"); 
  Ustr *haystack;
  size_t num = 0;
  
  ASSERT(ustr_sub_fmt(&ro, 2, "%s%c", "x", 'x'));
  ASSERT(ustr_cmp_cstr_eq(ro, "axxd"));
  ustr_sc_free2(&ro, USTR1(\x7, "1234567"));
  ASSERT(ustr_sub_fmt(&ro, 7, "%s%c", "x", 'x'));
  ASSERT(ustr_cmp_cstr_eq(ro, "123456xx"));
  ASSERT(ustr_sub_fmt(&ro, 1, "%s%c", "x", 'x'));
  ASSERT(ustr_cmp_cstr_eq(ro, "xx3456xx"));
  ustr_sc_free2(&ro, USTR1(\x7, "1234567"));
  ASSERT(ustr_sub_fmt_lim(&ro, 2, 1, "%s%c", "x", 'x'));
  ASSERT(ustr_cmp_cstr_eq(ro, "1x34567"));
  ASSERT(ustr_sub_fmt_lim(&ro, 2, 1, "%s%s", "", ""));
  ASSERT(ustr_cmp_cstr_eq(ro, "1x34567"));
  ASSERT(ustr_sub_fmt_lim(&ro, 2, 1, "%s%s", "", ""));
  ASSERT(ustr_cmp_cstr_eq(ro, "1x34567"));
  ASSERT(ustr_sub_fmt_lim(&ro, 2, 1, "%s%s", "", ""));
  ASSERT(ustr_cmp_cstr_eq(ro, "1x34567"));
  ustr_sc_free2(&ro, USTR1(\x7, "1234567"));
  
  ASSERT(ustr_sc_sub_fmt(&ro, 2, 1, "%s%c", "x", 'x'));
  ASSERT(ustr_cmp_cstr_eq(ro, "1xx34567"));
  ustr_sc_free2(&ro, USTR1(\x7, "1234567"));
  ASSERT(ustr_sc_sub_fmt_lim(&ro, 2, 2, 3, "%s%c", "yyyy", 'y'));
  ASSERT(ustr_cmp_cstr_eq(ro, "1yyy4567"));
  ASSERT(ustr_sc_sub_fmt_lim(&ro, 2, 1, 1, "%s%s", "", ""));
  ASSERT(ustr_cmp_cstr_eq(ro, "1yy4567"));
  ASSERT(ustr_sc_sub_fmt_lim(&ro, 2, 1, 1, "%s%s", "", ""));
  ASSERT(ustr_cmp_cstr_eq(ro, "1y4567"));
  ASSERT(ustr_sc_sub_fmt_lim(&ro, 2, 1, 1, "%s%s", "", ""));
  ASSERT(ustr_cmp_cstr_eq(ro, "14567"));
  ASSERT(ustr_sc_sub_fmt_lim(&ro, 2, 1, 1, "%s%s", "", ""));
  ASSERT(ustr_cmp_cstr_eq(ro, "1567"));

  ASSERT(ustr_sub_rep_chr(&ro, ustr_len(ro), 'y', 3));
  ASSERT(ustr_sub_rep_chr(&ro, ustr_len(ro), '3', 2));
  ASSERT(ustr_sub_rep_chr(&ro, ustr_len(ro), '4', 1));
  ASSERT(ustr_sc_sub_fmt_lim(&ro, 2, ustr_len(ro) - 5,
                             1, "%sxxx%s", "s", "x"));
  ASSERT(ustr_cmp_cstr_eq(ro, "1syy34"));
  ASSERT(ustr_sc_sub_rep_chr(&ro, 3, 1, '2', 0));
  ASSERT(ustr_cmp_cstr_eq(ro, "1sy34"));
  ASSERT(ustr_sc_sub_cstr(&ro, 2, 2, "2x"));
  ASSERT(ustr_cmp_cstr_eq(ro, "12x34"));
  ASSERT(ustr_sc_sub_fmt_lim(&ro, 3, 1, 150,
                             "%75s%-75sXYZ", "|", "|"));
  ASSERT(ustr_cmp_cstr_eq(ro, "12                                                                          ||                                                                          34"));
  ASSERT(ustr_sc_sub_rep_chr(&ro, 2, 0, 'y', 0));
  ASSERT(ustr_sub_rep_chr(&ro, 2, 'y', 0));
  ASSERT(ustr_sc_sub_cstr(&ro, 2, 0, ""));
  ASSERT(ustr_sub_cstr(&ro, 2, ""));
  ASSERT(ustr_cmp_cstr_eq(ro, "12                                                                          ||                                                                          34"));
  ASSERT(ustr_sub_fmt(&ro, 1, "%154s", "abcd"));
  ASSERT(ustr_cmp_cstr_eq(ro, "                                                                                                                                                      abcd"));
  ustr_sc_free2(&ro, USTR1(\x7, "1234567"));
  ASSERT(ustr_sub_fmt_lim(&ro, 1, 154, "%156s", "abcd"));
  ASSERT(ustr_cmp_cstr_eq(ro, "                                                                                                                                                        ab"));
  ustr_sc_free2(&ro, USTR1(\x7, "1234567"));
  ustr_sub_subustr(&ro, 4, USTR1(\x7, "abcdefg"), 3, 4);
  ASSERT(ustr_cmp_cstr_eq(ro, "123cdef"));
  ustr_sc_free2(&ro, USTR1(\x7, "1234567"));
  ustr_sc_sub_subustr(&ro, 4, 2, USTR1(\x7, "abcdefg"), 3, 4);
  ASSERT(ustr_cmp_cstr_eq(ro, "123cdef67"));

  ASSERT(ustr_cmp_cstr_eq(a,str));
  /* replace will grow the string */
  ASSERT(ustr_sub_cstr(&a,17,repl_cstr));
  PRINT_RESULT(a);
  ASSERT(ustr_cmp_cstr_eq(a,"the quick brown smart"));
  
  /* sub cstr */
  ASSERT(ustr_sub_cstr(&a,5,repl_cstr));
  PRINT_RESULT(a);
  ASSERT(ustr_cmp_cstr_eq(a,"the smart brown smart"));
  
  /* sub ustr */
  ASSERT(ustr_sub(&a,11,repl));
  PRINT_RESULT(a);
  ASSERT(ustr_cmp_cstr_eq(a,"the smart smart smart"));
  ustr_sc_free(&repl);
  
  str = "this is a test";
  c = ustr_dup_cstr(str);
  repl = ustr_dup_cstr("a real donut");
  /* invalid position */
  if (!USTR_DEBUG)
  ASSERT(!ustr_sc_sub(&c,1500,3,repl));
  if (!USTR_DEBUG)
  ASSERT(!ustr_sc_sub(&c,1500,3,c));
  PRINT_RESULT(c);
  ASSERT(ustr_cmp_cstr_eq(c,str));
  
  ASSERT(ustr_sc_sub(&c,9,6,repl));
  ASSERT(ustr_cmp_cstr_eq(c,"this is a real donut"));
  
  /* use sub as an erase */
  blank = ustr_dup_empty();
  ASSERT(ustr_sc_sub(&c,1,10,blank));
  PRINT_RESULT(c);
  ASSERT(ustr_cmp_cstr_eq(c,"real donut"));
  /* replacement string length = old string length */
  same_len_str = ustr_dup_cstr("mmmm");
  ASSERT(ustr_sc_sub(&c,1,4,same_len_str));
  PRINT_RESULT(c);
  ASSERT(ustr_cmp_cstr_eq(c,"mmmm donut"));
  ustr_sc_free(&repl);
  /* replacement expands string */
  repl = ustr_dup_cstr("sprinkles");
  ASSERT(ustr_sc_sub(&c,6,5,repl));
  PRINT_RESULT(c);
  ASSERT(ustr_cmp_cstr_eq(c,"mmmm sprinkles"));
  ustr_sc_free(&repl);
  d = ustr_dup_cstr("aabbccabcabc");
  repl = ustr_dup_cstr(" ");
  needle = ustr_dup_cstr("a");
  /* replace -- same len */
  ASSERT(ustr_replace(&d,needle,repl,0) == 4);
  PRINT_RESULT(d);
  newstr = "  bbcc bc bc";
  ASSERT(ustr_cmp_cstr_eq(d,newstr));
  ustr_sc_free(&repl);
  ustr_sc_free(&needle);
  /* limited number of replacements -- same len */
  repl = ustr_dup_cstr(".");
  needle = ustr_dup_cstr("c");
  ASSERT(ustr_replace(&d,needle,repl,1) == 1);
  PRINT_RESULT(d);
  newstr = "  bb.c bc bc";
  ASSERT(ustr_cmp_cstr_eq(d,newstr));
  ustr_sc_free(&needle);
  ustr_sc_free(&repl);
  
  /* limited number of replacements -- different len */
  ustr_sc_free2(&d, USTR1(\x20, "123456789 x123456789 x123456789 "));
  ASSERT(ustr_replace(&d, USTR1(\1, "x"), USTR1(\2, "ab"), 1) == 1);
  ASSERT(ustr_cmp_cstr_eq(d, "123456789 ab123456789 x123456789 "));
  /* replacement -- different len */
  ustr_sc_free2(&d, USTR1(\x20, "123456789 x123456789 x123456789 "));
  ASSERT(ustr_replace_cstr(&d, "x", "ab", 0) == 2);
  ASSERT(ustr_cmp_cstr_eq(d, "123456789 ab123456789 ab123456789 "));
  ASSERT(ustr_replace_cstr(&d, "x", "ab", 0) == 0);
  ASSERT(ustr_cmp_cstr_eq(d, "123456789 ab123456789 ab123456789 "));
  
  ASSERT(ustr_replace_cstr(&d, "ab1", "!", 0) == 2);
  ASSERT(ustr_cmp_cstr_eq(d, "123456789 !23456789 !23456789 "));
  ASSERT(ustr_replace_cstr(&d, "12", "!Z", 0) == 1);
  ASSERT(ustr_cmp_cstr_eq(d, "!Z3456789 !23456789 !23456789 "));
        
  ASSERT(ustr_replace(&d, USTR1(\2, "34"), USTR1(\1, "x"), 0) == 3);
  ASSERT(ustr_cmp_cstr_eq(d, "!Zx56789 !2x56789 !2x56789 "));
  
  ustr_sc_free2(&d, USTR1(\7, "xxxxxxx"));
  ASSERT(ustr_len(d) == 7);
  ASSERT(ustr_replace_buf(&d, "x", 1, "y", 0, 0) == 7);
  ASSERT(!ustr_len(d));
  ustr_sc_free2(&d, USTR1(\7, "xxxxxxx"));
  ASSERT(ustr_replace_buf(&d, "x", 1, "yy", 1, 0) == 7);
  ASSERT(ustr_cmp_cstr_eq(d, "yyyyyyy"));
  ustr_sc_free2(&d, USTR1(\7, "xxxxxxx"));
  ASSERT(ustr_replace_buf(&d, "x", 1, "yy", 2, 0) == 7);
  ASSERT(ustr_cmp_cstr_eq(d, "yyyyyyyyyyyyyy"));
  
  ASSERT(ustr_sub_subustr(&d, 1, USTR1(\2, "aa"), 1, 0));
  ASSERT(ustr_cmp_cstr_eq(d, "yyyyyyyyyyyyyy"));
  ASSERT(ustr_sc_sub_subustr(&d, 1, 0, USTR1(\2, "aa"), 1, 0));
  ASSERT(ustr_cmp_cstr_eq(d, "yyyyyyyyyyyyyy"));
  ASSERT(ustr_sc_sub_subustr(&d, 1, 2, USTR1(\2, "aa"), 1, 2));
  ASSERT(ustr_cmp_cstr_eq(d, "aayyyyyyyyyyyy"));
  ASSERT(ustr_sc_sub_subustr(&d, 1, 2, USTR1(\2, "aa"), 1, 0));
  ASSERT(ustr_cmp_cstr_eq(d, "yyyyyyyyyyyy"));
  
  ASSERT(ustr_sub_undef(&d, 3, 2));
  ASSERT(ustr_sc_sub_undef(&d, 1, 2, 2));
  
  if (!USTR_DEBUG)
  ASSERT(!ustr_sub_undef(&d, 100, 1));
  if (!USTR_DEBUG)
  ASSERT(!ustr_sub_subustr(&d, 1, USTR1(\4, "abcd"), 4, 2));
  if (!USTR_DEBUG)
  ASSERT(!ustr_sub_subustr(&d, 1, USTR1(\4, "abcd"), 5, 1));
  if (!USTR_DEBUG)
  ASSERT(!ustr_sc_sub_undef(&d, 100, 1, 1));
  if (!USTR_DEBUG)
  ASSERT(!ustr_sc_sub_subustr(&d, 1, 1, USTR1(\4, "abcd"), 4, 2));
  if (!USTR_DEBUG)
  ASSERT(!ustr_sc_sub_subustr(&d, 1, 1, USTR1(\4, "abcd"), 5, 1));
  PRINT_RESULT(c);

  if (!USTR_CONF_HAVE_64bit_SIZE_MAX) /* 10MB is probably enough... */
  {
    ASSERT(ustr_set_rep_chr(&s1, '-', 1000 * 1000 * 10));
    ASSERT(!ustr_replace(&s1, USTR1(\1, "-"), s1, 0));
  }
  
  ustr_sc_free(&a);
  ustr_sc_free(&c);
  ustr_sc_free(&d);
  ustr_sc_free(&needle);
  ustr_sc_free(&same_len_str);
  ustr_sc_free(&blank);
  ustr_sc_free(&ro);

  /* rep_chr -- shrink string */
  haystack = ustr_dup_cstr("xxxBxxCxDxxx");
  ASSERT(ustr_replace_rep_chr(&haystack, 'x', 2, 'y', 1, 0) == 3);
  ASSERT(ustr_cmp_cstr_eq(haystack, "yxByCxDyx"));
  ustr_sc_free(&haystack);
  /* rep_chr -- expand string */
  haystack = ustr_dup_cstr("yyyyByy");
  ASSERT(!ustr_replace_rep_chr(&haystack, 'x', 2, 'a', 2, 0));
  ASSERT(ustr_replace_rep_chr(&haystack, 'y', 2, 'a', 3, 0) == 3);
  ASSERT(ustr_cmp_cstr_eq(haystack, "aaaaaaBaaa"));
  /* rep_chr -- limited number of searches */
  ASSERT(ustr_replace_rep_chr(&haystack, 'a', 2, 'x', 1, 2) == 2);
  ASSERT(ustr_cmp_cstr_eq(haystack, "xxaaBaaa"));
  /* rep_chr -- stumble upon nonrepeating character in buffer middle */
  ASSERT(ustr_replace_rep_chr(&haystack, 'a', 4, 'x', 1, 2) == 0);
  ASSERT(ustr_cmp_cstr_eq(haystack, "xxaaBaaa"));
  /* rep_chr -- shortcut case where end of buffer is not equal to rep_chr */
  ASSERT(ustr_replace_rep_chr(&haystack, 'a', 3, 'z', 1, 2) == 1);
  ASSERT(ustr_cmp_cstr_eq(haystack, "xxaaBz"));
  ustr_sc_free(&haystack);
  /* rep_chr -- case where "check the end" succeeds, but multiple characters
   * interfere in the middle of the buffer */
  haystack = ustr_dup_cstr("xxABCDyyEFGyyz");
  ASSERT(ustr_replace_rep_chr(&haystack, 'y', 7, 'a', 1, 0) == 0);
  ASSERT(ustr_cmp_cstr_eq(haystack,"xxABCDyyEFGyyz"));
  ustr_sc_free(&haystack);
  /* rep_chr --  case where "check the end" succeeds, has to be skipped
   * and after the end there is a valid pattern */
  haystack = ustr_dup_cstr("xxABCDyyEFGyyyyyyyz");
  ASSERT(ustr_replace_rep_chr(&haystack, 'y', 7, 'a', 1, 0) == 1);
  ASSERT(ustr_cmp_cstr_eq(haystack,"xxABCDyyEFGaz"));
  ustr_sc_free(&haystack);

  ustr_sc_free2(&haystack, USTR1(\4, "aaaa"));
  ASSERT(ustr_replace_rep_chr(&haystack, 'a', 1, 'x', 0, 0) == 4);
  ASSERT(!ustr_len(haystack));
  ASSERT(ustr_ro(haystack));

  /*
  ustr_sc_free2(&s2, USTR(""));
  ASSERT(ustr_sub(&s2, 1, s2));
  ASSERT(ustr_sub_subustr(&s2, 1, s2, 1, 0));
  ASSERT(ustr_sc_sub(&s2, 1, 0, s2));
  ASSERT(ustr_sc_sub_subustr(&s2, 1, 0, s2, 1, 0));
  */
  
  ustr_sc_free2(&s1, USTR1(\1, "x"));
  ASSERT(ustr_sub(&s1, 1, s1));
  ASSERT_EQ(s1, USTR1(\1, "x"));
  ASSERT(ustr_sub_subustr(&s1, 0, s1, 1, 0));
  ASSERT_EQ(s1, USTR1(\1, "x"));
  ASSERT(ustr_sub_subustr(&s1, 1, s1, 1, 1));
  ASSERT_EQ(s1, USTR1(\1, "x"));
  /* ASSERT(ustr_ro(s1)); */
  ustr_sc_free2(&s1, USTR1(\1, "x"));
  ASSERT(ustr_sc_sub(&s1, 1, 0, s1));
  ASSERT_EQ(s1, USTR1(\2, "xx"));
  ustr_sc_free2(&s1, USTR1(\1, "x"));
  ASSERT(ustr_sc_sub_subustr(&s1, 1, 0, s1, 1, 0));
  ASSERT_EQ(s1, USTR1(\1, "x"));
  ASSERT(ustr_ro(s1));

  num = 0;
  ustr_sc_free2(&s1, USTR1(\2, "xy"));
  while (num++ < 6)
    ASSERT(ustr_sub(&s1, (ustr_len(s1) / 2) + 1, s1));
  ASSERT_EQ(s1, USTR1(\x13, "xxxxxxxxxxxxxxxxxxy"));
  
  ustr_sc_free2(&s1, USTR1(\2, "xy"));
  num = 0;
  while (num++ < 7)
  {
    size_t spos = 1;
    size_t slen = ustr_len(s1);

    if (slen > 4)
    {
      ++spos;
      --slen;
    }
    
    ASSERT(ustr_sub_subustr(&s1, (ustr_len(s1) / 2) + 1, s1, spos, slen));
  }
  ASSERT_EQ(s1, USTR1(\x15, "xxxxxxxxxxxxxxxxxxxxy"));

  ustr_sc_free2(&s1, USTR1(\2, "xy"));
  ASSERT(ustr_sc_sub(&s1, 1, 1, s1));
  ASSERT_EQ(s1, USTR1(\3, "xyy"));
  ASSERT(ustr_sc_sub(&s1, 1, 2, s1));
  ASSERT_EQ(s1, USTR1(\4, "xyyy"));
  ASSERT(ustr_sc_sub(&s1, 4, 1, s1));
  ASSERT_EQ(s1, USTR1(\7, "xyyxyyy"));
  ASSERT(ustr_sc_sub(&s1, 6, 2, s1));
  ASSERT_EQ(s1, USTR1(\xC, "xyyxyxyyxyyy"));
  ASSERT(ustr_sc_sub(&s1, 4, 5, s1));
  ASSERT_EQ(s1, USTR1(\x13, "xyyxyyxyxyyxyyyxyyy"));
  
  ustr_sc_free2(&s1, USTR1(\5, "1234-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 1, 1, s1, 1, 2));
  ASSERT_EQ(s1, USTR1(\6, "12234-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 3, 1, s1, 5, 2));
  ASSERT_EQ(s1, USTR1(\7, "124-34-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 3, 2, s1, 4, 1));
  ASSERT_EQ(s1, USTR1(\6, "12-34-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 2, 3, s1, 1, 5));
  ASSERT_EQ(s1, USTR1(\x8, "112-344-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 1, 2, s1, 2, 3));
  ASSERT_EQ(s1, USTR1(\x9, "12-2-344-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 2, 2, s1, 5, 2));
  ASSERT_EQ(s1, USTR1(\x9, "1-32-344-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 3, 3, s1, 4, 2));
  ASSERT_EQ(s1, USTR1(\x8, "1-2-344-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 6, 1, s1, 2, 1));
  ASSERT_EQ(s1, USTR1(\x8, "1-2-3-4-"));
  ASSERT(ustr_sc_sub_subustr(&s1, 1, 8, s1, 1, 7));
  ASSERT_EQ(s1, USTR1(\7, "1-2-3-4"));

  ustr_set_cstr(&s1, "123456789 ");
  ASSERT(ustr_sub_subustr(&s1, 8, s1, 6, 5));
  ASSERT_EQ(s1, USTR1(\xc, "12345676789 "));

  ustr_set_cstr(&s1, "123456789 ");
  ASSERT(ustr_sub_subustr(&s1, 4, s1, 4, 4));
  ASSERT_EQ(s1, USTR1(\7, "1234567"));

  ustr_set_cstr(&s1, "123456789 abcdefghijklmnopqrstuvqxyz");
  ASSERT(ustr_sc_sub_subustr(&s1, 4, 8, s1, 6, 12));
  ASSERT_EQ(s1, USTR1(\x28, "1236789 abcdefgbcdefghijklmnopqrstuvqxyz"));

  
  return (EXIT_SUCCESS);
}
