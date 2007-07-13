#ifndef USTR_SUB_H
#error " Include ustr-sub.h before this file."
#endif

USTR_CONF_I_PROTO int ustr_sub_buf(struct Ustr **s1, size_t pos, const void *buf, size_t blen)
{
	if (!s1 || ustr_ro (*s1)) {
		return USTR_FALSE;
	}
	if (!blen) {
		return (USTR_TRUE);
	}

	size_t s1len = ustr_len(*s1);
	if (pos - 1 + blen > s1len) { /* need to expand s1 */
			size_t add_len = pos + blen - s1len - 1;
			ustr_add_undef(s1, add_len);
	}
	ustr__memcpy(*s1,pos-1, buf, blen);

	/* need to explicitly null terminate here? */

	return (USTR_TRUE);
}

USTR_CONF_I_PROTO int ustr_sub_cstr(struct Ustr **s1, size_t pos, const char *cs2) 
{
	return ustr_sub_buf(s1, pos, cs2, strlen(cs2));
}

USTR_CONF_I_PROTO int ustr_sub(struct Ustr **s1, size_t pos, const struct Ustr *s2)
{
	return ustr_sub_cstr(s1, pos, ustr_cstr(s2));
}

USTR_CONF_I_PROTO int ustr_sc_sub_buf(struct Ustr **s1, size_t pos, size_t oldlen, const void *buf, size_t blen)
{
	/* doesn't make sense to run a sub on an invalid or empty string
	   		is there a valid check that doesn't require an assertion? */
	if (!s1 || !ustr_assert_valid(*s1)) {
		return USTR_FALSE;
	}
	
	struct Ustr *orig_ustr = *s1;
	size_t orig_len = ustr_len(orig_ustr);
	if (pos > orig_len) {
		/* after the end of the string */
		return USTR_FALSE;
	}
	if (blen == oldlen) {
		return ustr_sub_buf(s1, pos, buf, blen);
	}
	size_t len_after_split = orig_len - (pos + oldlen) + 1;
	struct Ustr *newstr = ustr_dup_empty();
	/* potential for optimization? */ 
	ustr_add_subustr(&newstr, orig_ustr, 1, pos - 1);
	ustr_add_buf(&newstr, buf, blen);
	ustr_add_subustr(&newstr, orig_ustr, pos + oldlen, len_after_split);
	ustr_free(orig_ustr);
	*s1 = newstr;
	return USTR_TRUE;
}

USTR_CONF_I_PROTO int ustr_sc_sub(struct Ustr **s1, size_t pos, size_t oldlen, const struct Ustr *s2)
{
	return ustr_sc_sub_buf(s1, pos, oldlen, ustr_cstr(s2), ustr_len(s2));
}

USTR_CONF_I_PROTO int ustr_replace(struct Ustr **haystack, const struct Ustr *needle, const struct Ustr *repl, size_t max_repl)
{
	const char *needle_cstr = ustr_cstr(needle);
	size_t repl_len = ustr_len(needle);
	if (!haystack || !ustr_assert_valid(*haystack)) {
		return USTR_FALSE;
	}
	size_t num_found=0;
	size_t found_off=ustr_srch_cstr_fwd(*haystack, 0, needle_cstr);
	while (found_off) {
		if (!ustr_sc_sub(haystack, found_off, repl_len, repl)) {
			return USTR_FALSE;
		}
		found_off=ustr_srch_cstr_fwd(*haystack, found_off, needle_cstr);
		++num_found;
		if (max_repl > 0 && num_found == max_repl) {
			break;
		}
	}
	return num_found;
}

