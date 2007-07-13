#ifndef USTR_SUB_H
#define USTR_SUB_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

USTR_CONF_E_PROTO int ustr_sub_buf(struct Ustr **, size_t, const void*, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET()
	USTR__COMPILE_ATTR_NONNULL_L((1,3));

USTR_CONF_E_PROTO int ustr_sc_sub_buf(struct Ustr **, size_t , size_t , const void *, size_t )
    USTR__COMPILE_ATTR_WARN_UNUSED_RET()
	USTR__COMPILE_ATTR_NONNULL_L((1,4));

USTR_CONF_E_PROTO int ustr_sub(struct Ustr **, size_t, const struct Ustr *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET()
	USTR__COMPILE_ATTR_NONNULL_L((1));

USTR_CONF_E_PROTO int ustr_sub_cstr(struct Ustr **, size_t, const char *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET()
	USTR__COMPILE_ATTR_NONNULL_L((1));

USTR_CONF_E_PROTO int ustr_sc_sub(struct Ustr **, size_t, size_t, const struct Ustr *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET()
	USTR__COMPILE_ATTR_NONNULL_L((1));

USTR_CONF_E_PROTO
int ustr_replace(struct Ustr **,const struct Ustr *,const struct Ustr *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
	USTR__COMPILE_ATTR_NONNULL_L((1));

/*
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
# include "ustr-sub-internal.h"
#endif
*/

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-sub-code.h"
#endif
/*
#if USTR_CONF_COMPILE_USE_INLINE
#endif
*/

#endif
