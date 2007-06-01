/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_CNTL_H
#define USTR_CNTL_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

#ifndef USTR_CONF_USE_MALLOC_CHECK
#define USTR_CONF_USE_MALLOC_CHECK 0
#endif

#include <stdarg.h> /* va_list for va_arg() like functionality */

#define USTR_CNTL_OPT_GET_REF_BYTES   (1)
#define USTR_CNTL_OPT_SET_REF_BYTES   (2)
#define USTR_CNTL_OPT_GET_HAS_SIZE    (3)
#define USTR_CNTL_OPT_SET_HAS_SIZE    (4)
#define USTR_CNTL_OPT_GET_EXACT_BYTES (5)
#define USTR_CNTL_OPT_SET_EXACT_BYTES (6)
#define USTR_CNTL_OPT_GET_MEM         (7)
#define USTR_CNTL_OPT_SET_MEM         (8)
#define USTR_CNTL_OPT_GET_MEMP        (9)
#define USTR_CNTL_OPT_SET_MEMP       (10)

/* move to dynamic configuration, so it's more usable from a shared library */
#undef  USTR_CONF_REF_BYTES
#define USTR_CONF_REF_BYTES (ustr__opts->ref_bytes)
#undef  USTR_CONF_HAS_SIZE
#define USTR_CONF_HAS_SIZE (ustr__opts->has_size)
#undef  USTR_CONF_EXACT_BYTES
#define USTR_CONF_EXACT_BYTES (ustr__opts->exact_bytes)
#undef  USTR_CONF_MALLOC
#define USTR_CONF_MALLOC(x)     ((*ustr__opts->ustr.sys_malloc)(x))
#undef  USTR_CONF_REALLOC
#define USTR_CONF_REALLOC(x, y) ((*ustr__opts->ustr.sys_realloc)((x), (y)))
#undef  USTR_CONF_FREE
#define USTR_CONF_FREE(x)       ((*ustr__opts->ustr.sys_free)(x))
#undef  USTR_CONF_POOL_MALLOC
#define USTR_CONF_POOL_MALLOC(p, x)             \
    ((*ustr__opts->ustrp.pool_sys_malloc)((p), (x)))
#undef  USTR_CONF_POOL_REALLOC
#define USTR_CONF_POOL_REALLOC(p, x, y, z)      \
    ((*ustr__opts->ustrp.pool_sys_realloc)((p), (x), (y), (z)))
#undef  USTR_CONF_POOL_FREE
#define USTR_CONF_POOL_FREE(p, x)               \
    ((*ustr__opts->ustrp.pool_sys_free)((p), (x)))

struct Ustr_cntl_mem
{
 void *(*sys_malloc)(size_t);
 void *(*sys_realloc)(void *, size_t);
 void  (*sys_free)(void *);
};

struct Ustr_cntl_memp
{
 void *(*pool_sys_malloc)(void *, size_t);
 void *(*pool_sys_realloc)(void *, void *, size_t, size_t);
 void  (*pool_sys_free)(void *, void *); 
};

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
struct Ustr_opts
{
 size_t ref_bytes;

 struct Ustr_cntl_mem  ustr;
 struct Ustr_cntl_memp ustrp;

 unsigned int has_size    : 1;
 unsigned int exact_bytes : 1;
};

/* this is for use within the shared library only... */
extern struct Ustr_opts ustr__opts[1];
#else
struct Ustr_opts; /* declare opaque struct */
#endif

#if USTR_CONF_COMPILE_TYPEDEF
typedef struct Ustr_cntl_mem  Ustr_cntl_mem;
typedef struct Ustr_cntl_memp Ustr_cntl_memp;
typedef struct Ustr_opts      Ustr_opts;
#endif

USTR_CONF_E_PROTO int ustr_cntl_opt(int, ...)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();

#if USTR_CONF_INCLUDE_CODEONLY_HEADERS
#include "ustr-cntl-code.h"
#endif

#endif
