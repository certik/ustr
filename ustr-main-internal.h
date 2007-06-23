/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_MAIN_INTERNAL_H
#define USTR_MAIN_INTERNAL_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

struct Ustr__pool_si_node
{
 struct Ustr__pool_si_node *next;
 void *ptr;
};

struct Ustr__pool_si_base
{ /* "simple" pool implementation */
 struct Ustr_pool cbs;
 struct Ustr__pool_si_node *beg;
 
 struct Ustr__pool_si_base *sbeg; /* wasting a lot of space for sub pools */
 struct Ustr__pool_si_base *base;
 struct Ustr__pool_si_base *next;
 struct Ustr__pool_si_base *prev;
};

#define USTR__POOL_NULL ((struct Ustr__pool_si_base *) 0)

#define USTR__PPTR(x) ((struct Ustr **) x) /* for converting a Ustrp** ...
                                            * safe from aliasing, I think */

/* default sized to 1 ... */
#define USTR__DUPX_DEF                                  \
    USTR_CONF_HAS_SIZE, USTR_CONF_REF_BYTES,            \
    USTR_CONF_EXACT_BYTES, USTR_FALSE
#define USTR__DUPX_FROM(x)                                              \
    (ustr_alloc(x) ?                                                    \
     (ustr_sized(x) ? ustr__sz_get(x) : 0) : USTR_CONF_HAS_SIZE),       \
    (ustr_alloc(x) ? USTR__REF_LEN(x) : USTR_CONF_REF_BYTES),           \
    (ustr_alloc(x) ? ustr_exact(x) : USTR_CONF_EXACT_BYTES), ustr_enomem(x)

 /* #include <stdio.h>
    printf("sz=%zu rbytes=%zu exact=%s len=%zu --> rsz=%zu\n", sz, rbytes, exact ? "TRUE" : "FALSE", len, rsz); */

USTR_CONF_e_PROTO void *ustr__pool_sys_malloc(struct Ustr_pool *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A()
    USTR__COMPILE_ATTR_MALLOC();
USTR_CONF_e_PROTO
void *ustr__pool_sys_realloc(struct Ustr_pool *, void *, size_t, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((1))
    USTR__COMPILE_ATTR_MALLOC();
USTR_CONF_e_PROTO void ustr__pool_sys_free(struct Ustr_pool *, void *)
    USTR__COMPILE_ATTR_NONNULL_L((1));

USTR_CONF_e_PROTO struct Ustr_pool *ustr__pool_make_subpool(struct Ustr_pool *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_MALLOC();
USTR_CONF_e_PROTO void ustr__pool__clear(struct Ustr__pool_si_base *, int);
USTR_CONF_e_PROTO void ustr__pool_clear(struct Ustr_pool *)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO void ustr__pool__free(struct Ustr__pool_si_base *, int);
USTR_CONF_e_PROTO void ustr__pool_free(struct Ustr_pool *);

USTR_CONF_e_PROTO size_t ustr__dupx_cmp_eq(size_t, size_t, size_t, size_t,
                                           size_t, size_t, size_t, size_t)
    USTR__COMPILE_ATTR_CONST() USTR__COMPILE_ATTR_WARN_UNUSED_RET();

USTR_CONF_e_PROTO size_t ustr__sz_get(const struct Ustr *)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO size_t ustr__nb(size_t num)
    USTR__COMPILE_ATTR_CONST() USTR__COMPILE_ATTR_WARN_UNUSED_RET();

USTR_CONF_e_PROTO void ustr__embed_val_set(unsigned char *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustr__ref_set(struct Ustr *, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO void ustr__len_set(struct Ustr *s1, size_t len)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO void ustr__sz_set(struct Ustr *, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustr__ref_add(struct Ustr *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO size_t ustr__ref_del(struct Ustr *s1)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO void ustrp__free(struct Ustr_pool *, struct Ustr *);
USTR_CONF_e_PROTO
void ustrp__sc_free2(struct Ustr_pool *p, struct Ustr **ps1, struct Ustr *s2)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO size_t ustr__ns(size_t num)
    USTR__COMPILE_ATTR_CONST() USTR__COMPILE_ATTR_WARN_UNUSED_RET();
USTR_CONF_e_PROTO void ustr__terminate(unsigned char *, int, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_undef(struct Ustr_pool *, size_t, size_t, int, int,
                               size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET();

USTR_CONF_e_PROTO
int ustrp__rw_realloc(struct Ustr_pool *, struct Ustr **, int, size_t, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustr__memcpy(struct Ustr *, size_t, const void *,size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO void ustr__memset(struct Ustr *, size_t, int, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustrp__reallocx(struct Ustr_pool *, struct Ustr **, int)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__realloc(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustr__rw_add(struct Ustr *, size_t, size_t *, size_t *,
                                   size_t *, size_t *, int *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustr__rw_del(struct Ustr *, size_t, size_t *,
                                   size_t *, size_t *, size_t *, int *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustrp__del(struct Ustr_pool *, struct Ustr **, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO size_t ustr__valid_subustr(const struct Ustr *, size_t,size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO
int ustrp__del_subustr(struct Ustr_pool *, struct Ustr **, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_empty(struct Ustr_pool *, size_t, size_t, int, int)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET();
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_buf(struct Ustr_pool *, size_t, size_t, int, int,
                             const void *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dup(struct Ustr_pool *, const struct Ustr *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx(struct Ustr_pool *, size_t, size_t, int ,
                         int, const struct Ustr *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_subustr(struct Ustr_pool *,
                                 size_t, size_t, int, int,
                                 const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dup_subustr(struct Ustr_pool *,
                                const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_rep_chr(struct Ustr_pool *, size_t, size_t, int, int,
                                 char, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET();
USTR_CONF_e_PROTO
int ustrp__add_undef(struct Ustr_pool *, struct Ustr **, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
int ustrp__add_buf(struct Ustr_pool *, struct Ustr **, const void *, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO int ustr__treat_as_buf(const struct Ustr *, size_t,
                                         const struct Ustr *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO
int ustrp__add(struct Ustr_pool *, struct Ustr **, const struct Ustr *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO
int ustrp__add_subustr(struct Ustr_pool *, struct Ustr **,
                       const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO
int ustrp__add_rep_chr(struct Ustr_pool *p, struct Ustr **, char, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustrp__sc_del(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
struct Ustr *ustrp__sc_dupx(struct Ustr_pool *p,
                            size_t sz, size_t rbytes, int exact, int emem,
                            struct Ustr **ps1)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO struct Ustr *ustrp__sc_dup(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__sc_ensure_owner(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustrp__sc_free(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustrp__sc_free_shared(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__sc_reverse(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__sc_tolower(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__sc_toupper(struct Ustr_pool *, struct Ustr **)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));

#endif
