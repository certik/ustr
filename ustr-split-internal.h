/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SPLIT_INTERNAL_H
#define USTR_SPLIT_INTERNAL_H 1

#ifndef USTR_MAIN_H
# error " You should have already included ustr-main.h, or just include ustr.h"
#endif

#ifdef USTR_SRCH_H
USTR_CONF_e_PROTO
struct Ustr *ustr__split(const struct Ustr *, size_t *, const struct Ustr *,
							unsigned int);
#endif

#endif
