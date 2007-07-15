/* Copyright (c) 2007 Paul Rosenfeld
                      James Antill -- See LICENSE file for terms. */
#ifndef USTR_SUB_INTERNAL_H
#define USTR_SUB_INTERNAL_H 1

#ifndef USTR_MAIN_H
# error " You should have already included ustr-main.h, or just include ustr.h"
#endif

USTR_CONF_e_PROTO
int ustrp__sub_undef(struct Ustr_pool *, struct Ustr **, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
int ustrp__sub_buf(struct Ustr_pool *,struct Ustr **,size_t,const void*,size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4));
USTR_CONF_e_PROTO
int ustrp__sub(struct Ustr_pool *, struct Ustr **,size_t, const struct Ustr *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4));
USTR_CONF_e_PROTO
int ustrp__sub_subustr(struct Ustr_pool *, struct Ustr **, size_t,
                       const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 4));
USTR_CONF_e_PROTO
int ustrp__sub_rep_chr(struct Ustr_pool *, struct Ustr **, size_t, char,size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));


USTR_CONF_e_PROTO
int ustrp__sc_sub_undef(struct Ustr_pool *,struct Ustr **,size_t,size_t,size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
int ustrp__sc_sub_buf(struct Ustr_pool *, struct Ustr **, size_t, size_t,
                     const void *, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5));
USTR_CONF_e_PROTO
int ustrp__sc_sub(struct Ustr_pool *, struct Ustr **, size_t, size_t,
                 const struct Ustr *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5));
USTR_CONF_e_PROTO
int ustrp__sc_sub_subustr(struct Ustr_pool *, struct Ustr **, size_t, size_t,
                          const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 5));
USTR_CONF_e_PROTO
int ustrp__sc_sub_rep_chr(struct Ustr_pool *, struct Ustr **, size_t, size_t,
                         char, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));

USTR_CONF_e_PROTO
size_t ustrp__sc_replace_buf(struct Ustr_pool *, struct Ustr **,
                             const void *, size_t, const void *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3, 5));
USTR_CONF_e_PROTO
size_t ustrp__sc_replace(struct Ustr_pool *, struct Ustr **,const struct Ustr *,
                         const struct Ustr *, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3, 4));

#endif
