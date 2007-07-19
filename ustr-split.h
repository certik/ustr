/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SPLIT_H
#define USTR_SPLIT_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

#define USTR_SPLIT_RET_SEP 0x1
#define USTR_SPLIT_RET_NON 0x2
#define USTR_SPLIT_KEEP_CONF 0x4


#ifdef USTR_SRCH_H
USTR_CONF_E_PROTO
struct Ustr *ustr_split(const struct Ustr *, size_t *, const struct Ustr *,
							unsigned int);
#endif

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
# include "ustr-split-internal.h"
#endif

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
# include "ustr-split-code.h"
#endif

#if USTR_CONF_COMPILE_USE_INLINE
#endif

#endif
