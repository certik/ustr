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
	ustr_free(repl);

	str = "this is a test";
	c = ustr_dup_cstr(str);
	repl = ustr_dup_cstr("a real donut");
	/* invalid position */
        if (!USTR_DEBUG)
	ASSERT(!ustr_sc_sub(&c,1500,3,repl));
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
	ustr_free(repl);
	/* replacement expands string */
	repl = ustr_dup_cstr("sprinkles");
	ASSERT(ustr_sc_sub(&c,6,5,repl));
	PRINT_RESULT(c);
	ASSERT(ustr_cmp_cstr_eq(c,"mmmm sprinkles"));
	ustr_free(repl);
	d = ustr_dup_cstr("aabbccabcabc");
	repl = ustr_dup_cstr(" ");
	needle = ustr_dup_cstr("a");
	/* replace -- same len */
	ASSERT(ustr_replace(&d,needle,repl,0) == 4);
	PRINT_RESULT(d);
	newstr = "  bbcc bc bc";
	ASSERT(ustr_cmp_cstr_eq(d,newstr));
	ustr_free(repl);
	ustr_free(needle);
	/* limited number of replacements -- same len */
	repl = ustr_dup_cstr(".");
	needle = ustr_dup_cstr("c");
	ASSERT(ustr_replace(&d,needle,repl,1) == 1);
	PRINT_RESULT(d);
	newstr = "  bb.c bc bc";
	ASSERT(ustr_cmp_cstr_eq(d,newstr));
	ustr_free(needle);
	ustr_free(repl);

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
        
	ustr_free(a);
	ustr_free(c);
	ustr_free(d);
	ustr_free(needle);
	ustr_free(same_len_str);
	ustr_free(blank);
	ustr_free(ro);
	PRINT_RESULT(a);
          
        return EXIT_SUCCESS;
}
