/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_SRCH_INTERNAL_H
#define USTR_SRCH_INTERNAL_H 1

#ifndef USTR_MAIN_H
# error " You should have already included ustr-main.h, or just include ustr.h"
#endif

#ifndef USTR_CONF_HAVE_MEMMEM /* GNU extension */
#ifdef __GLIBC__
#define USTR_CONF_HAVE_MEMMEM 1
#else
#define USTR_CONF_HAVE_MEMMEM 0
#endif
#endif

#if USTR_CONF_HAVE_MEMMEM /* GNU extension */
# define USTR__SYS_MEMMEM memmem
#else
USTR_CONF_e_PROTO void *ustr__sys_memmem(const void*, size_t,const void*,size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
# define USTR__SYS_MEMMEM ustr__sys_memmem
#endif

USTR_CONF_e_PROTO void *ustr__sys_memrepchr(const void *hs, size_t hslen,
                                            char nd, size_t ndlen)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

#endif
