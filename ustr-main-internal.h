/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */
#ifndef USTR_MAIN_INTERNAL_H
#define USTR_MAIN_INTERNAL_H 1

#ifndef USTR_MAIN_H
# error " You should include ustr-main.h before this file, or just ustr.h"
#endif

#define USTR__PPTR(x) ((struct Ustr **) x) /* for converting a Ustrp** ...
                                            * safe from aliasing, I think */

/* default sized to 1 ... */
#define USTR__DUPX_DEF                                  \
    USTR_CONF_HAS_SIZE, USTR_CONF_REF_BYTES,            \
    USTR_CONF_EXACT_BYTES, 0
#define USTR__DUPX_FROM(x)                                              \
    (ustr_sized(x) ? ustr__sz_get(x) : 0), USTR__REF_LEN(x),            \
    ustr_exact(x), ustr_enomem(x)

 /* #include <stdio.h>
    printf("sz=%zu rbytes=%zu exact=%s len=%zu --> rsz=%zu\n", sz, rbytes, exact ? "TRUE" : "FALSE", len, rsz); */

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
USTR_CONF_e_PROTO void ustrp__free(void *, struct Ustr *);
USTR_CONF_e_PROTO
void ustrp__sc_free2(void *p, struct Ustr **ps1, struct Ustr *s2)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO size_t ustr__ns(size_t num)
    USTR__COMPILE_ATTR_CONST() USTR__COMPILE_ATTR_WARN_UNUSED_RET();
USTR_CONF_e_PROTO void ustr__terminate(unsigned char *, int, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();

USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_undef(void *, size_t, size_t, int, int, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET();

USTR_CONF_e_PROTO
int ustrp__rw_realloc(void *, struct Ustr **, int, size_t, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustr__memcpy(struct Ustr *, size_t,const void *,size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO void ustr__memset(struct Ustr *, size_t, int, size_t)
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustrp__reallocx(void *, struct Ustr **, int)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__realloc(void *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustr__rw_add(struct Ustr *, size_t, size_t *, size_t *,
                                   size_t *, size_t *, int *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustr__rw_del(struct Ustr *, size_t, size_t *,
                                   size_t *, size_t *, size_t *, int *)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO int ustrp__del(void *, struct Ustr **, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO size_t ustr__valid_subustr(const struct Ustr *, size_t,size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO
int ustrp__del_subustr(void *, struct Ustr **, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_empty(void *, size_t, size_t, int, int)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET();
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_buf(void *, size_t, size_t, int, int,
                             const void *, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO struct Ustr *ustrp__dup(void *p, const struct Ustr *s1)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx(void *p, size_t sz, size_t rbytes, int exact,
                         int emem, const struct Ustr *s1)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_subustr(void *,
                                 size_t, size_t, int, int,
                                 const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dup_subustr(void *, const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
struct Ustr *ustrp__dupx_rep_chr(void *, size_t, size_t, int, int, char, size_t)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET();
USTR_CONF_e_PROTO int ustrp__add_undef(void *, struct Ustr **, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
int ustrp__add_buf(void *p, struct Ustr **, const void *, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO int ustr__treat_as_buf(const struct Ustr *, size_t,
                                         const struct Ustr *, size_t)
    USTR__COMPILE_ATTR_PURE() USTR__COMPILE_ATTR_WARN_UNUSED_RET()
    USTR__COMPILE_ATTR_NONNULL_A();
USTR_CONF_e_PROTO
int ustrp__add(void *, struct Ustr **, const struct Ustr *)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO
int ustrp__add_subustr(void *, struct Ustr **,
                       const struct Ustr *, size_t, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2, 3));
USTR_CONF_e_PROTO
int ustrp__add_rep_chr(void *p, struct Ustr **, char, size_t)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO int ustrp__sc_ensure_owner(void *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustrp__sc_free(void *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustrp__sc_free_shared(void *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO void ustrp__sc_del(void *, struct Ustr **)
    USTR__COMPILE_ATTR_NONNULL_L((2));
USTR_CONF_e_PROTO
struct Ustr *ustrp__sc_dupx(void *p,
                            size_t sz, size_t rbytes, int exact, int emem,
                            struct Ustr **ps1)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((6));
USTR_CONF_e_PROTO struct Ustr *ustrp__sc_dup(void *p, struct Ustr **ps1)
    USTR__COMPILE_ATTR_WARN_UNUSED_RET() USTR__COMPILE_ATTR_NONNULL_L((2));

#endif
