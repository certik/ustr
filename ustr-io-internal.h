/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_IO_INTERNAL_H
#define USTR_IO_INTERNAL_H 1

#ifndef USTR_IO_H
# error " You should have already included ustr-io.h, or just include ustr.h"
#endif

USTR_CONF_e_PROTO
int ustrp__io_get(void *, struct Ustr **, FILE *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO int ustrp__io_getfile(void *, struct Ustr **, FILE *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO int ustrp__io_getfilename(void *,struct Ustr **, const char *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));

USTR_CONF_e_PROTO int ustrp__io_getline(void *, struct Ustr **, FILE *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));

USTR_CONF_e_PROTO int ustrp__io_put(void *, struct Ustr **, FILE *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO int ustrp__io_putline(void *, struct Ustr **, FILE *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO
int ustrp__io_putfilename(void *, struct Ustr **, const char *, const char *)
   USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2, 3, 4));

#endif
