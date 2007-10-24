/* this compiles with:
   gcc -std=c89 -ansi -pedantic -Wall -W -lustr -DUSTR_CONF_COMPILE_USE_INLINE=0
*/
#include <ustr.h>

int main(void)
{
	Ustr *s1 = USTR1_CHK(\2, "ab");
	Ustr *s2 = USTR1_CHK(\0, "");
	Ustr *s3 = USTR("");
	if (!ustr_len(s1)) return 1;
	if ( ustr_len(s2)) return 2;
	if ( ustr_cstr(s1) == (char *)s1) return 3;
	if ( ustr_cstr(s2) == (char *)s2) return 4;
	if ( ustr_cstr(s3) != (char *)s3) return 5;
	if (!*(char *)s1) return 6;
	if (!*(char *)s2) return 7;
	if ( *(char *)s3) return 8;

	return 0;
}
