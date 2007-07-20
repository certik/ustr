/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SPLIT_H
#define USTR_SPLIT_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

#define USTR_FLAG_SPLIT_RET_SEP   (1<<0)
#define USTR_FLAG_SPLIT_RET_NON   (1<<1)
#define USTR_FLAG_SPLIT_KEEP_CONF (1<<2)

/* FIXME: ustr_split_chrs ... if anyone of the chars is found, split.
 * Do we need this? */

USTR_CONF_E_PROTO
struct Ustr *ustr_split_buf(const struct Ustr *, size_t *,
                            const void *, size_t, unsigned int)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_E_PROTO
struct Ustr *ustr_split(const struct Ustr *, size_t *, const struct Ustr *,
                        unsigned int)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_EI_PROTO struct Ustr *ustr_split_cstr(const struct Ustr *, size_t *,
                                                const char *, unsigned int)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
# include "ustr-split-internal.h"
#endif

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-split-code.h"
#endif

#if USTR_CONF_COMPILE_USE_INLINE
USTR_CONF_II_PROTO
struct Ustr *ustr_split_cstr(const struct Ustr *s1, size_t *off,
                             const char *cstr, unsigned int flags)
{ return (ustr_split_buf(s1, off, cstr, strlen(cstr), flags)); }
#endif

#endif
