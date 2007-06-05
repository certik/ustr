/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_MAIN_H
#error " You should have already included ustr-main.h, or just include ustr.h."
#endif

USTR_CONF_I_PROTO struct Ustr_pool *ustr_pool_make(void)
{
  struct Ustr_pool *ret = USTR_CONF_MALLOC(sizeof(struct Ustr_pool));

  if (!ret)
    return (ret);
  
  ret->next = 0;
  ret->ptr  = 0;

  return (ret);
}

USTR_CONF_I_PROTO void ustr_pool_free(struct Ustr_pool *scan)
{
  if (!scan)
    return;
  
  do
  {
    struct Ustr_pool *scan_next = scan->next;

    USTR_CONF_FREE(scan->ptr);
    USTR_CONF_FREE(scan);

    scan = scan_next;
  } while (scan);
}

USTR_CONF_I_PROTO void ustr_pool_clear(struct Ustr_pool *scan)
{
  USTR_ASSERT(scan);
  
  USTR_CONF_FREE(scan->ptr);
  scan->ptr  = 0;

  ustr_pool_free(scan->next);
  scan->next = 0;
}

USTR_CONF_I_PROTO void *ustr_pool_sys_malloc(void *vp, size_t len)
{
  struct Ustr_pool *p = vp;
  void *ret = USTR_CONF_MALLOC(len);
  struct Ustr_pool *np = 0;

  USTR_ASSERT(p);
  
  if (!ret)
    return (ret);
  
  if (p->ptr)
  {
    if (!(np = USTR_CONF_MALLOC(sizeof(struct Ustr_pool))))
    {
      USTR_CONF_FREE(ret);
      return (0);
    }

    np->next = p->next;
    np->ptr  = p->ptr;
      
    p->next  = np;
  }
  p->ptr = ret;
  
  return (ret);
}

USTR_CONF_I_PROTO
void *ustr_pool_sys_realloc(void *vp, void *old,
                            size_t olen, size_t nlen)
{
  struct Ustr_pool *p = vp;
  void *ret = 0;

  USTR_ASSERT(p && old && p->ptr);
  
  if (p->ptr == old) /* let the last allocated Ustrp grow/shrink */
  {
    if ((ret = USTR_CONF_REALLOC(old, nlen)))
      p->ptr = ret;
  }
  else if (olen >= nlen) /* always allow reductions/nothing */
    return (old);
  else if ((ret = ustr_pool_sys_malloc(p, nlen)))
    memcpy(ret, old, olen);
  
  return (ret);
}

USTR_CONF_I_PROTO
void ustr_pool_sys_free(void *p, void *old)
{ /* does nothing */
  (void) p;
  (void) old;
}

USTR_CONF_i_PROTO
size_t ustr__dupx_cmp_eq(size_t w1, size_t x1, size_t y1, size_t z1,
                         size_t w2, size_t x2, size_t y2, size_t z2)
{ return ((w1 == w2) && (!x1 == !x2) && (y1 == y2) && (z1 == z2)); }

USTR_CONF_i_PROTO size_t ustr__sz_get(const struct Ustr *s1)
{
  size_t lenn = 0;
  
  USTR_ASSERT(!ustr_ro(s1));
  USTR_ASSERT( ustr_sized(s1));
  
  lenn = USTR__LEN_LEN(s1);
  
  return (ustr__embed_val_get(s1->data + 1 + USTR__REF_LEN(s1) + lenn, lenn));
}

USTR_CONF_i_PROTO size_t ustr__nb(size_t num)
{
  USTR_ASSERT((num <= (0xFFFFFFFF - (1 + 4 + 1))) ||
              USTR_CONF_HAVE_64bit_SIZE_MAX);
  
  if (num > 0xFFFFFFFF)                 return (8);
  if (num > 0xFFFF)                     return (4);
  if (num > 0xFF)                       return (2);
  else                                  return (1);
}

USTR_CONF_I_PROTO int ustr_assert_valid(const struct Ustr *s1)
{
  const char *eos_ptr = 0;
  size_t      eos_len = sizeof(USTR_END_ALOCDx);
  size_t rbytes = 0;
  size_t lbytes = 0;
  size_t sbytes = 0;
  size_t sz = 0;
  size_t oh = 0;
  
  USTR_ASSERT_RET(s1, USTR_FALSE);
  
  if (!s1->data[0])
    return (USTR_TRUE);

  /* just make sure for compound "bits" tests */
  USTR_ASSERT(( ustr_alloc(s1) || ustr_sized(s1)) != ustr_ro(s1));
  USTR_ASSERT((!ustr_alloc(s1) && ustr_sized(s1)) == ustr_fixed(s1));
  USTR_ASSERT(( ustr_fixed(s1) && ustr_exact(s1)) == ustr_limited(s1));
  
  rbytes = USTR__REF_LEN(s1);
  lbytes = USTR__LEN_LEN(s1);
  ustr_assert_ret(lbytes, USTR_FALSE);

  if (ustr_sized(s1))
  {
    sbytes = lbytes;
    sz = ustr__sz_get(s1);
  }
  oh = 1 + rbytes + lbytes + sbytes + eos_len;
  
  USTR_ASSERT_RET(!ustr_sized(s1) || (ustr_len(s1) <= sz), USTR_FALSE);

  USTR_ASSERT(!sz || (ustr__nb(sz) == lbytes) ||
              ((ustr__nb(sz) == 1) && (lbytes == 2))); /* 2 is the minimum */
  
  USTR_ASSERT_RET(!sz || (oh <= sz),                       USTR_FALSE);
  USTR_ASSERT_RET(!sz || ((ustr_len(s1) + oh) <= sz),      USTR_FALSE);
  
  USTR_ASSERT_RET( ustr_exact(s1)  || !ustr_ro(s1),         USTR_FALSE);
  USTR_ASSERT_RET(!ustr_enomem(s1) || !ustr_ro(s1),         USTR_FALSE);

  if (!USTR_CONF_USE_EOS_MARK)
  {
    USTR_ASSERT_RET(!ustr_cstr(s1)[ustr_len(s1)], USTR_FALSE);
    return (USTR_TRUE);
  }
  
  if (ustr_ro(s1))
    eos_ptr =        USTR_END_CONSTx;
  else if (ustr_fixed(s1))
    eos_ptr =        USTR_END_FIXEDx;
  else
    eos_ptr =        USTR_END_ALOCDx;

  USTR_ASSERT_RET(!memcmp(ustr_cstr(s1) + ustr_len(s1), eos_ptr, eos_len),
                  USTR_FALSE);

  return (USTR_TRUE);
}

USTR_CONF_I_PROTO char *ustr_wstr(struct Ustr *s1)
{ /* NOTE: Not EI/II so we can call ustr_assert_valid() here. */
  unsigned char *data = s1->data;
  size_t lenn = 0;
  
  USTR_ASSERT(ustr_assert_valid(s1));
  
  USTR_ASSERT_RET(!ustr_ro(s1), 0);
  
  lenn = USTR__LEN_LEN(s1);
  if (ustr_sized(s1))
    lenn *= 2;
  
  return ((char *)(data + 1 + USTR__REF_LEN(s1) + lenn));
}

USTR_CONF_I_PROTO int ustr_owner(const struct Ustr *s1)
{
  USTR_ASSERT(ustr_assert_valid(s1));
  
  if (ustr_ro(s1))    return (USTR_FALSE);
  if (ustr_fixed(s1)) return (USTR_TRUE);

  switch (USTR__REF_LEN(s1))
  {
    case 8: if (s1->data[8]) return (USTR_FALSE);
            if (s1->data[7]) return (USTR_FALSE);
            if (s1->data[6]) return (USTR_FALSE);
            if (s1->data[5]) return (USTR_FALSE);
    case 4: if (s1->data[4]) return (USTR_FALSE);
            if (s1->data[3]) return (USTR_FALSE);
    case 2: if (s1->data[2]) return (USTR_FALSE);
      
    case 1:                  return (s1->data[1] == 1);
      
    case 0:                  return (USTR_TRUE);  /* Ustr with no ref. count */
      
    default:
      ustr_assert(! "Reference count length invalid for ustr__ref_owner()");
      abort(); /* Keeps CC happy when it can't tell this doesn't happen */
      return (USTR_FALSE);
  }
}

USTR_CONF_I_PROTO int ustr_setf_enomem_err(struct Ustr *s1)
{
  USTR_ASSERT(ustr_assert_valid(s1));
  
  if (ustr_ro(s1))
    return (USTR_FALSE);
  
  s1->data[0] |=  USTR__BIT_ENOMEM;
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_setf_enomem_clr(struct Ustr *s1)
{
  USTR_ASSERT(ustr_assert_valid(s1));
  
  if (ustr_ro(s1))
    return (USTR_TRUE);
  
  s1->data[0] &= ~USTR__BIT_ENOMEM;
  return (USTR_TRUE);
}

USTR_CONF_i_PROTO void ustr__embed_val_set(unsigned char *data,
                                           size_t len, size_t val)
{
  switch (len)
  {
#if USTR_CONF_HAVE_64bit_SIZE_MAX
    case 8:
      data[7] = (val >> 56) & 0xFF;
      data[6] = (val >> 48) & 0xFF;
      data[5] = (val >> 40) & 0xFF;
      data[4] = (val >> 32) & 0xFF;
#endif
    case 4:
      data[3] = (val >> 24) & 0xFF;
      data[2] = (val >> 16) & 0xFF;
    case 2:
      data[1] = (val >>  8) & 0xFF;
    case 1:
      data[0] = (val >>  0) & 0xFF;
      return;
      
    default:
      abort();
  }
}

/* no warn here, because most callers already know it's not going to fail */
USTR_CONF_i_PROTO int ustr__ref_set(struct Ustr *s1, size_t ref)
{
  size_t len = 0;
  
  USTR_ASSERT(ustr_assert_valid(s1));
  USTR_ASSERT(ustr_alloc(s1) || (ustr_fixed(s1) && !ref));

  if (!(len = USTR__REF_LEN(s1)))
    return (USTR_FALSE);
  
  ustr__embed_val_set(s1->data + 1, len, ref);
  
  USTR_ASSERT(ustr_assert_valid(s1));

  return (USTR_TRUE);
}

USTR_CONF_I_PROTO int ustr_setf_share(struct Ustr *s1)
{
  USTR_ASSERT(ustr_assert_valid(s1));

  if (!ustr_alloc(s1))
    return (USTR_TRUE);
  
  if (!ustr__ref_set(s1, 0))
    return (USTR_FALSE);
  
  USTR_ASSERT(ustr_assert_valid(s1));
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_setf_owner(struct Ustr *s1)
{
  USTR_ASSERT(ustr_assert_valid(s1));

  if (!ustr_alloc(s1))
    return (USTR_FALSE);

  ustr__ref_set(s1, 1); /* 0 means it's unsharable and so they are the owner */
  
  USTR_ASSERT(ustr_assert_valid(s1));
  return (USTR_TRUE);
}

USTR_CONF_i_PROTO void ustr__len_set(struct Ustr *s1, size_t len)
{ /* can only validate after the right len is in place */
  unsigned char *data = s1->data;
  
  USTR_ASSERT(!ustr_ro(s1));
  ustr__embed_val_set(data + 1 + USTR__REF_LEN(s1), USTR__LEN_LEN(s1), len);
  USTR_ASSERT(ustr_assert_valid(s1));
}

USTR_CONF_i_PROTO void ustr__sz_set(struct Ustr *s1, size_t sz)
{ /* can't validate as this is called before anything is setup */
  size_t lenn = 0;
  
  USTR_ASSERT(!ustr_ro(s1));
  USTR_ASSERT(ustr_sized(s1));

  lenn = USTR__LEN_LEN(s1);
  
  ustr__embed_val_set(s1->data + 1 + USTR__REF_LEN(s1) + lenn, lenn, sz);
}

#define USTR__REF_T_ADD(s1, num, lim)              \
    case num:                                      \
    {                                              \
      ref = ustr__ref_get(s1);                     \
      if (ref == 0)   return (USTR_TRUE);         \
      if (ref == lim) return (USTR_FALSE);        \
      ustr__ref_set(s1, ref + 1);                  \
    }                                              \
    return (USTR_TRUE)
USTR_CONF_i_PROTO int ustr__ref_add(struct Ustr *s1)
{
  size_t ref = 0;

  USTR_ASSERT(ustr_assert_valid(s1));
  
  if (ustr_ro(s1))    return (USTR_TRUE);
  if (ustr_fixed(s1)) return (USTR_FALSE);
  
  switch (USTR__REF_LEN(s1))
  {
    case 0: return (USTR_FALSE); /* Ustr with no reference count */

    USTR__REF_T_ADD(s1, 1, 0xFF);
    USTR__REF_T_ADD(s1, 2, 0xFFFF);
    USTR__REF_T_ADD(s1, 4, 0xFFFFFFFFUL);
#if USTR_CONF_HAVE_64bit_SIZE_MAX
    USTR__REF_T_ADD(s1, 8, 0xFFFFFFFFFFFFFFFFULL);
#endif
      
    default:
      ustr_assert(! "Reference count length invalid for ustr__ref_add()");
      /* Keeps CC happy when it can't tell this doesn't happen */
      return (USTR_FALSE);
  }
}
#undef USTR__REF_T_ADD

USTR_CONF_i_PROTO size_t ustr__ref_del(struct Ustr *s1)
{
  USTR_ASSERT(ustr_assert_valid(s1));
  
  if (!ustr_alloc(s1))
    return (-1);

  switch (USTR__REF_LEN(s1))
  {
    case 8: case 4: case 2: case 1:
    {
      size_t ref = ustr__ref_get(s1);
      
      if (ref == 0)
        return (-1);
      
      ustr__ref_set(s1, ref - 1);
      return (ref - 1);
    }
          
    case 0: return (0);  /* Ustr with no reference count */

    default:
      ustr_assert(! "Reference count length invalid for ustr__ref_del()");
      return (-1); /* Keeps CC happy when it can't tell this doesn't happen */
  }
}

USTR_CONF_i_PROTO void ustrp__free(void *p, struct Ustr *s1)
{
  if (!s1) return;
  
  if (!ustr__ref_del(s1))
  {
    if (p)
      USTR_CONF_POOL_FREE(p, s1);
    else
      USTR_CONF_FREE(s1);
  }
}

USTR_CONF_I_PROTO void ustr_free(struct Ustr *s1)
{ return (ustrp__free(0, s1)); }
USTR_CONF_I_PROTO void ustrp_free(void *p, struct Ustrp *s1)
{ return (ustrp__free(p, &s1->s)); }

/* shortcut -- needs to be here, as lots of things calls this */
USTR_CONF_i_PROTO
void ustrp__sc_free2(void *p, struct Ustr **ps1, struct Ustr *s2)
{
  USTR_ASSERT(ps1);
  USTR_ASSERT(ustr_assert_valid(s2)); /* don't pass NULL */
  
  ustrp__free(p, *ps1);
  *ps1 = s2;
}

USTR_CONF_i_PROTO size_t ustr__ns(size_t num)
{
  size_t min_sz = 4;
  
  if (num > ((USTR__SIZE_MAX / 4) * 3))
    return (USTR__SIZE_MAX);

  /* *2 is too much, we end up wasting a lot of RAM. So we do *1.5. */
  while (min_sz < num)
  {
    size_t adder = min_sz / 2;

    min_sz += adder;
    if (min_sz >= num) break;
    min_sz += adder;
  }
      
  return (min_sz);
}

/* ---------------- init - helpers so others can make Ustr's ---------------- */
USTR_CONF_I_PROTO size_t ustr_init_size(size_t sz, size_t rbytes, int exact,
                                        size_t len)
{
  size_t lbytes = ustr__nb(sz ? sz : len);
  size_t sbytes = 0;
  size_t oh = 0;
  
  USTR_ASSERT_RET((rbytes == 0) ||
                  (rbytes == 1) || (rbytes == 2) || (rbytes == 4) ||
                  (USTR_CONF_HAVE_64bit_SIZE_MAX && (rbytes == 8)), 0);
  USTR_ASSERT(    (lbytes == 1) || (lbytes == 2) || (lbytes == 4) ||
                  (USTR_CONF_HAVE_64bit_SIZE_MAX && (lbytes == 8)));

  if (!sz && ((lbytes == 8) || (rbytes == 8)))
    sz = 1;
  
  if (sz)
  {
    if (rbytes <= 1)
      rbytes = 2;
    if (lbytes <= 1)
      lbytes = 2;
    sbytes = lbytes;
  }

  oh = 1 + rbytes + lbytes + sbytes + sizeof(USTR_END_ALOCDx);
  sz = oh + len;
  
  if (sz < len)
    return (0);
  
  if (exact)
    return (sz);
  
  return (ustr__ns(sz));
}

/* NIL terminate -- with possible end marker */
USTR_CONF_i_PROTO void ustr__terminate(unsigned char *ptr, int alloc,size_t len)
{
  if (sizeof(USTR_END_ALOCDx) == 1)
    ptr[len] = 0;
  else if (alloc)
    memcpy(ptr + len, USTR_END_ALOCDx, sizeof(USTR_END_ALOCDx));
  else
    memcpy(ptr + len, USTR_END_FIXEDx, sizeof(USTR_END_FIXEDx));
}

USTR_CONF_I_PROTO
struct Ustr *ustr_init_alloc(void *data, size_t rsz, size_t sz,
                             size_t rbytes, int exact, int emem, size_t len)
{
  static const unsigned char map_big_pow2[9] = {-1, -1, 0, -1, 1, -1, -1, -1,2};
  static const unsigned char map_pow2[5] = {0, 1, 2, -1, 3};
  struct Ustr *ret = data;
  int nexact = !exact;
  int sized  = 0;
  size_t lbytes = ustr__nb(sz ? sz : len);
  size_t sbytes = 0;
  size_t oh = 0;
  const size_t eos_len = sizeof(USTR_END_ALOCDx);
  
  USTR_ASSERT_RET((rbytes == 0) ||
                  (rbytes == 1) || (rbytes == 2) || (rbytes == 4) ||
                  (USTR_CONF_HAVE_64bit_SIZE_MAX && (rbytes == 8)), USTR_NULL);
  USTR_ASSERT(    (lbytes == 1) || (lbytes == 2) || (lbytes == 4) ||
                  (USTR_CONF_HAVE_64bit_SIZE_MAX && (lbytes == 8)));
  USTR_ASSERT(data);
  USTR_ASSERT(exact == !!exact);
  USTR_ASSERT(emem  == !!emem);
  USTR_ASSERT(!sz || (sz == rsz));
  USTR_ASSERT_RET(!sz || (sz > len), USTR_NULL);

  if (!sz && ((lbytes == 8) || (rbytes == 8)))
    sz = rsz; /* whee... */
  
  if (sz)
  {
    if (sz < (1 + 2 + 2 + 1))
      return (USTR_NULL);

    if (rbytes <= 1)
      rbytes = 2;
    if (lbytes <= 1)
      lbytes = 2;
    sbytes = lbytes;
  }
  oh = 1 + rbytes + lbytes + sbytes + eos_len;
  
  if (rsz < (oh + len))
    return (USTR_NULL);
  
  if (sz)     sized  = USTR__BIT_HAS_SZ;
  if (nexact) nexact = USTR__BIT_NEXACT;
  if (emem)   emem   = USTR__BIT_ENOMEM;
    
  ret->data[0]  = USTR__BIT_ALLOCD | sized | nexact | emem;
  if (sz)
    ret->data[0] |= (map_big_pow2[rbytes] << 2) | map_big_pow2[lbytes];
  else
    ret->data[0] |= (map_pow2[rbytes] << 2) | map_pow2[lbytes];

  ustr__terminate(ret->data, USTR_TRUE, (oh - eos_len) + len);

  if (sz)
    ustr__sz_set(ret, sz);
  ustr__len_set(ret, len);
  if (rbytes) /* any reference value is valid */
    ustr__ref_set(ret, 1);

  USTR_ASSERT(ustr_assert_valid(ret));
  USTR_ASSERT( ustr_alloc(ret));
  USTR_ASSERT(!ustr_fixed(ret));
  USTR_ASSERT(!ustr_ro(ret));
  USTR_ASSERT( ustr_enomem(ret) == !!emem);
  USTR_ASSERT( ustr_exact(ret)  == exact);
  USTR_ASSERT(!ustr_shared(ret));
  USTR_ASSERT( ustr_owner(ret));

  return (ret);
}
USTR_CONF_I_PROTO
struct Ustrp *ustrp_init_alloc(void *data, size_t rsz, size_t sz,
                               size_t rbytes, int exact, int emem, size_t len)
{ return (USTRP(ustr_init_alloc(data, rsz, sz, rbytes, exact, emem, len))); }

USTR_CONF_I_PROTO
struct Ustr *ustr_init_fixed(void *data, size_t sz, int exact, size_t len)
{
  struct Ustr *ret = data;
  void *tmp = 0; /* move type between char and unsigned char */
  const size_t rbytes = 0;
  const int    emem   = USTR_FALSE;
  
  USTR_ASSERT(sz);
  
  if (!ustr_init_alloc(data, sz, sz, rbytes, exact, emem, len))
    return (USTR_NULL);

  tmp = ustr_wstr(ret); /* done here,
                           as it might not be valid until we terminate */

  ret->data[0] &= ~USTR__BIT_ALLOCD;
  ustr__terminate(tmp, ustr_alloc(ret), len);
  ustr__ref_set(ret, 0);

  USTR_ASSERT(ustr_assert_valid(ret));
  USTR_ASSERT( ustr_fixed(ret));
  USTR_ASSERT(!ustr_alloc(ret));
  USTR_ASSERT(!ustr_ro(ret));
  USTR_ASSERT( ustr_enomem(ret) == emem);
  USTR_ASSERT(!ustr_shared(ret));
  USTR_ASSERT( ustr_owner(ret));

  return (ret);
}
USTR_CONF_I_PROTO
struct Ustrp *ustrp_init_fixed(void *data, size_t sz, int exact, size_t len)
{ return (USTRP(ustr_init_fixed(data, sz, exact, len))); }

USTR_CONF_I_PROTO size_t ustr_overhead(const struct Ustr *s1)
{
  size_t lenn = 0;
  
  USTR_ASSERT(ustr_assert_valid(s1));

  if (!s1->data[0]) return (0);

  lenn = USTR__LEN_LEN(s1);
  if (ustr_sized(s1))
    lenn *= 2;
  
  return (1 + USTR__REF_LEN(s1) + lenn + sizeof(USTR_END_ALOCDx));
}

USTR_CONF_I_PROTO size_t ustr_size(const struct Ustr *s1)
{ /* size of available space in the string */
  size_t oh = 0;

  USTR_ASSERT(ustr_assert_valid(s1));

  if (ustr_ro(s1))
    return (0);
  USTR_ASSERT(ustr_overhead(s1));
  
  if (ustr_sized(s1))
    return (ustr__sz_get(s1) - ustr_overhead(s1));
  if (ustr_exact(s1))
    return (ustr_len(s1));

  oh = ustr_overhead(s1);
  return (ustr__ns(ustr_len(s1) + oh) - oh);
}

USTR_CONF_I_PROTO size_t ustr_size_alloc(const struct Ustr *s1)
{ /* copy and paste so that ustr_ro() does the right thing */
  size_t oh = 0;

  USTR_ASSERT(ustr_assert_valid(s1));

  if (ustr_ro(s1))
    return (0);
  USTR_ASSERT(ustr_overhead(s1));
  
  if (ustr_sized(s1))
    return (ustr__sz_get(s1));

  oh = ustr_overhead(s1);
  USTR_ASSERT((oh + ustr_len(s1)) >= oh);
  
  if (ustr_exact(s1))
    return (ustr_len(s1) + oh);

  return (ustr__ns(ustr_len(s1) + oh));
}

/* ---------------- allocations ---------------- */
/* NOTE: This is one of the two main "allocation" functions --
 * this is the only thing that calls MALLOC. */
USTR_CONF_i_PROTO
struct Ustr *ustrp__dupx_undef(void *p, size_t sz, size_t rbytes, 
                               int exact, int emem, size_t len)
{
  struct Ustr *ret = USTR_NULL;
  size_t rsz = 0;
  
  USTR_ASSERT((rbytes == 0) || (rbytes == 1) || (rbytes == 2) || (rbytes == 4)||
              (USTR_CONF_HAVE_64bit_SIZE_MAX && (rbytes == 8)));
  USTR_ASSERT(exact == !!exact);
  USTR_ASSERT(emem  == !!emem);

  if (!len && ustr__dupx_cmp_eq(sz, rbytes, exact, emem, USTR__DUPX_DEF))
    return USTR("");
  
  if (!(rsz = ustr_init_size(sz, rbytes, exact, len)))
    return (USTR_NULL);

  if (p)
    ret = USTR_CONF_POOL_MALLOC(p, rsz);
  else
    ret = USTR_CONF_MALLOC(rsz);
  
  if (!ret)
    return (USTR_NULL);
  
  ustr_init_alloc(ret, rsz, sz ? rsz : 0, rbytes, exact, emem, len);
  
  USTR_ASSERT(ustr_assert_valid(ret));
  return (ret);
}

USTR_CONF_i_PROTO
int ustrp__rw_realloc(void *p, struct Ustr **ps1,
                      int sized, size_t osz, size_t nsz)
{
  struct Ustr *ret = USTR_NULL;
  
  USTR_ASSERT(ustr_alloc(*ps1));
  USTR_ASSERT(sized == !!sized);
  USTR_ASSERT(sized == ustr_sized(*ps1));

  /*  printf("1. p=%p, osz=%zu, nsz=%zu\n", p, osz, nsz); */
  if (p)
    ret = USTR_CONF_POOL_REALLOC(p, *ps1, osz, nsz);
  else
    ret = USTR_CONF_REALLOC(*ps1, nsz);
  
  if (!ret)
  {
    ustr_setf_enomem_err(*ps1);
    return (USTR_FALSE);
  }
  if (sized)
    ustr__sz_set(ret, nsz);

  *ps1 = ret;

  return (USTR_TRUE);
}

USTR_CONF_i_PROTO void ustr__memcpy(struct Ustr *s1, size_t off,
                                    const void *ptr, size_t len)
{ /* can't call ustr_wstr() if len == 0, as it might be RO */
  if (!len)
    return;

  memcpy(ustr_wstr(s1) + off, ptr, len);
}

USTR_CONF_i_PROTO void ustr__memset(struct Ustr *s1, size_t off,
                                    int chr, size_t len)
{ /* can't call ustr_wstr() if len == 0, as it might be RO */
  if (!len)
    return;

  memset(ustr_wstr(s1) + off, chr, len);
}

/* ---------------- del ---------------- */

/* shrink the size of the Ustr to accomodate _just_ it's current len */
USTR_CONF_i_PROTO int ustrp__reallocx(void *p, struct Ustr **ps1, int exact)
{
  struct Ustr *s1 = USTR_NULL;
  size_t oh  = 0;
  size_t len = 0;
  size_t msz = 0; /* min size */
  size_t sz  = 0;
  int    ret = USTR_TRUE;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));

  s1 = *ps1;
  if (!ustr_sized(s1) || !ustr_alloc(s1) || !ustr_owner(s1))
    return (USTR_FALSE);
  
  oh  = ustr_overhead(s1);
  len = ustr_len(s1);
  msz = oh + len;
  if (!exact)
    msz = ustr__ns(msz);
  sz  = ustr__sz_get(s1);

  if (msz == sz)
    return (USTR_TRUE);
  
  /* we'd have to copy the Ustr, to add extra bytes to szn/lenn */
  if (USTR__LEN_LEN(s1) < ustr__nb(msz))
    return (USTR_FALSE);
  
  ret = ustrp__rw_realloc(p, ps1, USTR_TRUE, sz, msz);
  USTR_ASSERT(ustr_assert_valid(*ps1));

  if (!ret)
    return (USTR_FALSE);
  
  /* if (exact != ustr_exact(*ps1)) -- just always do it */
  if (exact)
    (*ps1)->data[0] &= ~USTR__BIT_NEXACT;
  else
    (*ps1)->data[0] |=  USTR__BIT_NEXACT;
  
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_reallocx(struct Ustr **ps1, int exact)
{ return (ustrp__reallocx(0, ps1, exact)); }
USTR_CONF_I_PROTO int ustrp_reallocx(void *p, struct Ustrp **ps1, int exact)
{ return (ustrp__reallocx(p, USTR__PPTR(ps1), exact)); }

USTR_CONF_i_PROTO int ustrp__realloc(void *p, struct Ustr **ps1)
{
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));

  return (ustrp__reallocx(p, ps1, ustr_exact(*ps1)));
}
USTR_CONF_I_PROTO int ustr_realloc(struct Ustr **ps1)
{ return (ustrp__realloc(0, ps1)); }
USTR_CONF_I_PROTO int ustrp_realloc(void *p, struct Ustrp **ps1)
{ return (ustrp__realloc(p, USTR__PPTR(ps1))); }

/* Can we actually RW adding to this Ustr, at _this_ moment, _this_ len */
USTR_CONF_i_PROTO
int ustr__rw_add(struct Ustr *s1, size_t nlen, size_t *sz, size_t *oh,
                 size_t *osz, size_t *nsz, int *alloc)
{
  size_t lbytes = 0;
  size_t sbytes = 0;
  
  if (!ustr_owner(s1))
    return (USTR_FALSE);

  *sz = 0;
  if (ustr_sized(s1))
    *sz = ustr__sz_get(s1);
  *osz = *sz;
  
  lbytes = USTR__LEN_LEN(s1);
  if (*sz)
    sbytes = lbytes;
  USTR_ASSERT(!*sz || (ustr__nb(*sz) == lbytes) ||
              ((ustr__nb(*sz) == 1) && (lbytes == 2))); /* 2 is the minimum */
  
  if (ustr__nb(nlen) > lbytes)
    return (USTR_FALSE); /* in theory we could do better, but it's _hard_ */
  
  *oh  = 1 + USTR__REF_LEN(s1) + lbytes + sbytes + sizeof(USTR_END_ALOCDx);
  *nsz = *oh + nlen;

  if (*nsz < nlen)
    return (USTR_FALSE);
  if (*nsz > USTR__SIZE_MAX) /* 32bit overflow on 64bit size_t */
    return (USTR_FALSE);

  *alloc = USTR_FALSE; /* don't need to allocate/deallocate anything */
  if (*nsz <= *sz)
    return (USTR_TRUE); /* ustr_fixed() */
  
  if (!ustr_exact(s1))
    *nsz = ustr__ns(*nsz);
  
  *osz = ustr_size_alloc(s1);
  
  if (!*sz && (*nsz <= *osz))
    return (USTR_TRUE);
  *alloc = USTR_TRUE; /* _do_   need to allocate/deallocate */
  
  if (!ustr_alloc(s1))
    return (USTR_FALSE);

  return (USTR_TRUE);
}

/* Can we actually RW this Ustr, at _this_ moment, to _this_ len */
USTR_CONF_i_PROTO int
ustr__rw_del(struct Ustr *s1, size_t nlen, size_t *sz, size_t *oh,
             size_t *osz, size_t *nsz, int *alloc)
{
  int ret = ustr__rw_add(s1, nlen, sz, oh, osz, nsz, alloc);

  if (ret && !*alloc && !*sz && ustr_alloc(s1))
  {
    if (*nsz != *sz)
      *alloc = USTR_TRUE;
  }

  return (ret);
}

/* NOTE: This is the main "deallocation" function (apart from plain free) --
 * this is one of only three things that removes data via. REALLOC.
 * Others are in ustr-set.h */
USTR_CONF_i_PROTO int ustrp__del(void *p, struct Ustr **ps1, size_t len)
{
  struct Ustr *s1  = USTR_NULL;
  struct Ustr *ret = USTR_NULL;
  size_t sz  = 0;
  size_t oh  = 0;
  size_t osz = 0;
  size_t nsz = 0;
  size_t clen = 0;
  size_t nlen = 0;
  int alloc = USTR_FALSE;

  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (!len) return (USTR_TRUE);

  s1   = *ps1;
  clen = ustr_len(s1);
  if (!(nlen = clen - len) && !ustr_fixed(*ps1) &&
      ustr__dupx_cmp_eq(USTR__DUPX_DEF, USTR__DUPX_FROM(s1)))
  {
    ustrp__sc_free2(p, ps1, USTR(""));
    return (USTR_TRUE);
  }
  
  if (nlen > clen) /* underflow */
    return (USTR_FALSE);

  if (ustr__rw_del(s1, nlen, &sz, &oh, &osz, &nsz, &alloc))
  {
    size_t eos_len = sizeof(USTR_END_ALOCDx);
    
    if (alloc)
    { /* ignore errors? -- can they happen on truncate? */
      int emem = ustr_enomem(*ps1);

      if (!ustrp__rw_realloc(p, ps1, !!sz, osz, nsz) && !emem)
        ustr_setf_enomem_clr(*ps1);
    }
      
    ustr__terminate((*ps1)->data, ustr_alloc(*ps1), (oh - eos_len) + nlen);
    ustr__len_set(*ps1, nlen);
    
    USTR_ASSERT(ustr_assert_valid(*ps1));
    return (USTR_TRUE);
  }

  USTR_ASSERT(!ustr_limited(s1));
  
  if (!(ret = ustrp__dupx_undef(p, USTR__DUPX_FROM(s1), nlen)))
    return (USTR_FALSE);

  ustr__memcpy(ret, 0, ustr_cstr(s1), nlen);
  ustrp__sc_free2(p, ps1, ret);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_del(struct Ustr **ps1, size_t len)
{ return (ustrp__del(0, ps1, len)); }
USTR_CONF_I_PROTO int ustrp_del(void *p, struct Ustrp **ps1, size_t len)
{ return (ustrp__del(p, USTR__PPTR(ps1), len)); }

USTR_CONF_i_PROTO
size_t ustr__valid_subustr(const struct Ustr *s1, size_t pos, size_t len)
{
  size_t clen = 0;
  
  USTR_ASSERT(ustr_assert_valid(s1));
  USTR_ASSERT_RET(pos, USTR_FALSE);
  
  clen = ustr_len(s1);
  if ((pos == 1) && (len == clen))
    return (clen);
  if (!pos || (clen < pos))
    return (0);
  if (clen < (len + --pos))
    return (0);

  return (clen);
}

USTR_CONF_i_PROTO
int ustrp__del_subustr(void *p, struct Ustr **ps1, size_t pos, size_t len)
{
  struct Ustr *s1  = USTR_NULL;
  struct Ustr *ret = USTR_NULL;
  size_t sz  = 0;
  size_t oh  = 0;
  size_t osz = 0;
  size_t nsz = 0;
  size_t clen = 0;
  size_t nlen = 0;
  const char *ocstr = 0;
  int alloc = USTR_FALSE;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));

  if (!len) return (USTR_TRUE);

  s1   = *ps1;
  clen = ustr__valid_subustr(s1, pos, len);
  if (!clen)
    return (USTR_FALSE);
  if (--pos == (clen - len)) /* deleting from the end */
    return (ustrp__del(p, ps1, len));

  nlen = clen - len;
  USTR_ASSERT(nlen < clen);
  
  if (ustr__rw_del(s1, nlen, &sz, &oh, &osz, &nsz, &alloc))
  { /* Move everything to the begining and call ustr_del() */
    char *ptr = ustr_wstr(s1);
    
    USTR_ASSERT(nlen - pos);

    memmove(ptr + pos, ptr + pos + len, (nlen - pos));

    return (ustrp__del(p, ps1, len));
  }

  USTR_ASSERT(!ustr_limited(s1));
  
  /* Can't do anything sane, give up and build a new string from scratch */
  if (!(ret = ustrp__dupx_undef(p, USTR__DUPX_FROM(s1), nlen)))
    return (USTR_FALSE);

  ocstr = ustr_cstr(*ps1);

  USTR_ASSERT(pos || (nlen - pos)); /* can be both */
  
  ustr__memcpy(ret, 0,   ocstr,                    pos);
  ustr__memcpy(ret, pos, ocstr + pos + len, nlen - pos);

  ustrp__sc_free2(p, ps1, ret);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_del_subustr(struct Ustr **ps1, size_t pos,size_t len)
{ return (ustrp__del_subustr(0, ps1, pos, len)); }
USTR_CONF_I_PROTO
int ustrp_del_subustrp(void *p, struct Ustrp **ps1, size_t pos, size_t len)
{ return (ustrp__del_subustr(p, USTR__PPTR(ps1), pos, len)); }

/* ---------------- dupx/dup ---------------- */

USTR_CONF_I_PROTO struct Ustr *ustr_dupx_undef(size_t sz, size_t rbytes, 
                                               int exact, int emem, size_t len)
{ return (ustrp__dupx_undef(0, sz, rbytes, exact, emem, len)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_undef(void *p, size_t sz, size_t rbytes, 
                               int exact, int emem, size_t len)
{ return (USTRP(ustrp__dupx_undef(p, sz, rbytes, exact, emem, len))); }

USTR_CONF_I_PROTO struct Ustr *ustr_dup_undef(size_t len)
{ return (ustr_dupx_undef(USTR__DUPX_DEF, len)); }

USTR_CONF_I_PROTO struct Ustrp *ustrp_dup_undef(void *p, size_t len)
{ return (ustrp_dupx_undef(p, USTR__DUPX_DEF, len)); }

USTR_CONF_i_PROTO
struct Ustr *ustrp__dupx_empty(void *p, size_t sz, size_t rbytes, 
                               int exact, int emem)
{ /* set the error bit, so we always get malloc()'d data, then clear it */
  struct Ustr *s1 = ustrp__dupx_undef(p, sz, rbytes, exact, USTR_TRUE, 0);
  int eret = USTR_FALSE;

  if (!s1 || emem)
    return (s1);

  eret = ustr_setf_enomem_clr(s1);
  USTR_ASSERT(eret);
  
  return (s1);
}
USTR_CONF_I_PROTO struct Ustr *ustr_dupx_empty(size_t sz, size_t rbytes, 
                                               int exact, int emem)
{ return (ustrp__dupx_empty(0, sz, rbytes, exact, emem)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_empty(void *p, size_t sz, size_t rbytes,
                               int exact, int emem)
{ return (USTRP(ustrp__dupx_empty(p, sz, rbytes, exact, emem))); }

USTR_CONF_I_PROTO struct Ustr *ustr_dup_empty(void)
{ return (ustr_dupx_empty(USTR__DUPX_DEF)); }

USTR_CONF_I_PROTO struct Ustrp *ustrp_dup_empty(void *p)
{ return (ustrp_dupx_empty(p, USTR__DUPX_DEF)); }

USTR_CONF_i_PROTO
struct Ustr *ustrp__dupx_buf(void *p, size_t sz, size_t rbytes, int exact,
                             int emem, const void *data, size_t len)
{
  struct Ustr *s1 = ustrp__dupx_undef(p, sz, rbytes, exact, emem, len);

  if (!s1)
    return (USTR_NULL);

  ustr__memcpy(s1, 0, data, len);

  USTR_ASSERT(ustr_assert_valid(s1));
  return (s1);
}
USTR_CONF_I_PROTO
struct Ustr *ustr_dupx_buf(size_t sz, size_t rb, int exact,
                           int emem, const void *data, size_t len)
{ return (ustrp__dupx_buf(0, sz, rb, exact, emem, data, len)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_buf(void *p, size_t sz, size_t rb, int exact,
                             int emem, const void *data, size_t len)
{ return (USTRP(ustrp__dupx_buf(p, sz, rb, exact, emem, data, len))); }

USTR_CONF_I_PROTO struct Ustr *ustr_dup_buf(const void *data, size_t len)
{ return (ustr_dupx_buf(USTR__DUPX_DEF, data, len)); }

USTR_CONF_I_PROTO
struct Ustrp *ustrp_dup_buf(void *p, const void *data, size_t len)
{ return (USTRP(ustrp_dupx_buf(p, USTR__DUPX_DEF, data, len))); }

USTR_CONF_i_PROTO struct Ustr *ustrp__dup(void *p, const struct Ustr *s1)
{ /* This ignores the const argument, because it doesn't alter the data, or at
   * all when ustr_ro(). */
  if (ustr__ref_add((struct Ustr *)s1))
    return ((struct Ustr *)s1);

  return (ustrp__dupx_buf(p, USTR__DUPX_FROM(s1), ustr_cstr(s1), ustr_len(s1)));
}
USTR_CONF_I_PROTO struct Ustr *ustr_dup(const struct Ustr *s1)
{ return (ustrp__dup(0, s1)); }
USTR_CONF_I_PROTO struct Ustrp *ustrp_dup(void *p, const struct Ustrp *s1)
{ return (USTRP(ustrp__dup(p, &s1->s))); }

USTR_CONF_i_PROTO
struct Ustr *ustrp__dupx(void *p, size_t sz, size_t rbytes, int exact,
                         int emem, const struct Ustr *s1)
{ /* the exactneustr of the options is more important than the reference */
  USTR_ASSERT((rbytes == 0) || (rbytes == 1) || (rbytes == 2) || (rbytes == 4)||
              (USTR_CONF_HAVE_64bit_SIZE_MAX && (rbytes == 8)));
  USTR_ASSERT(exact == !!exact);
  USTR_ASSERT(emem  == !!emem);
  
  if (ustr__dupx_cmp_eq(sz, rbytes, exact, emem, USTR__DUPX_FROM(s1)))
    return (ustrp__dup(p, s1));
  
  return (ustrp__dupx_buf(p, sz,rbytes,exact,emem, ustr_cstr(s1),ustr_len(s1)));
}
USTR_CONF_I_PROTO struct Ustr *ustr_dupx(size_t sz, size_t rbytes, int exact,
                                         int emem, const struct Ustr *s1)
{ return (ustrp__dupx(0, sz, rbytes, exact, emem, s1)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx(void *p, size_t sz, size_t rbytes, int exact,
                         int emem, const struct Ustrp *s1)
{ return (USTRP(ustrp__dupx(p, sz, rbytes, exact, emem, &s1->s))); }

USTR_CONF_i_PROTO
struct Ustr *ustrp__dupx_subustr(void *p,
                                 size_t sz, size_t rbytes, int exact, int emem,
                                 const struct Ustr *s2, size_t pos, size_t len)
{
  size_t clen = 0;
  
  USTR_ASSERT(ustr_assert_valid(s2));
  USTR_ASSERT(pos);

  if (!len)
    return (ustrp__dupx_undef(p, sz, rbytes, exact, emem, len));
  
  clen = ustr__valid_subustr(s2, pos, len);
  if (!clen)
    return (USTR_NULL);
  if (len == clen)
    return (ustrp__dupx(p, sz, rbytes, exact, emem, s2));
  
  return (ustrp__dupx_buf(p,sz,rbytes,exact,emem, ustr_cstr(s2) + --pos, len));
}
USTR_CONF_I_PROTO
struct Ustr *ustr_dupx_subustr(size_t sz, size_t rbytes, int exact, int emem,
                               const struct Ustr *s2, size_t pos, size_t len)
{ return (ustrp__dupx_subustr(0, sz, rbytes, exact, emem, s2, pos, len)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_subustrp(void *p,
                                  size_t sz, size_t rbytes, int exact, int emem,
                                  const struct Ustrp *s2, size_t pos,size_t len)
{ return (USTRP(ustrp__dupx_subustr(p, sz, rbytes, exact, emem,
                                    &s2->s, pos, len))); }

USTR_CONF_i_PROTO
struct Ustr *ustrp__dup_subustr(void *p, const struct Ustr *s2,
                                size_t pos, size_t len)
{ return (ustrp__dupx_subustr(p, USTR__DUPX_FROM(s2), s2, pos, len)); }
USTR_CONF_I_PROTO struct Ustr *ustr_dup_subustr(const struct Ustr *s2,
                                                size_t pos, size_t len)
{ return (ustrp__dup_subustr(0, s2, pos, len)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_dup_subustrp(void *p, const struct Ustrp *s2,
                                 size_t pos, size_t len)
{ return (USTRP(ustrp__dup_subustr(p, &s2->s, pos, len))); }

USTR_CONF_i_PROTO
struct Ustr *ustrp__dupx_rep_chr(void *p,
                                 size_t sz, size_t rbytes, int exact, int emem,
                                 char chr, size_t len)
{
  struct Ustr *s1 = ustrp__dupx_undef(p, sz, rbytes, exact, emem, len);
  
  if (!s1)
    return (USTR_NULL);

  if (len)
    ustr__memset(s1, 0, chr, len);

  USTR_ASSERT(ustr_assert_valid(s1));
  return (s1);
}
USTR_CONF_I_PROTO
struct Ustr *ustr_dupx_rep_chr(size_t sz, size_t rbytes, int exact, int emem,
                               char chr, size_t len)
{ return (ustrp__dupx_rep_chr(0, sz, rbytes, exact, emem, chr, len)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_dupx_rep_chr(void *p,
                                 size_t sz, size_t rbytes, int exact, int emem,
                                 char chr, size_t len)
{ return (USTRP(ustrp__dupx_rep_chr(p, sz, rbytes, exact, emem, chr, len))); }
USTR_CONF_I_PROTO struct Ustr *ustr_dup_rep_chr(char chr, size_t len)
{ return (ustr_dupx_rep_chr(USTR__DUPX_DEF, chr, len)); }
USTR_CONF_I_PROTO struct Ustrp *ustrp_dup_rep_chr(void *p, char chr, size_t len)
{ return (ustrp_dupx_rep_chr(p, USTR__DUPX_DEF, chr, len)); }

/* ---------------- add ---------------- */

/* NOTE: This is one of the two main "allocation" functions --
 * this is one of three things funcs that gets more data via. REALLOC.
 * Others are in ustr-set.h */
USTR_CONF_i_PROTO int ustrp__add_undef(void *p, struct Ustr **ps1, size_t len)
{
  struct Ustr *s1  = USTR_NULL;
  struct Ustr *ret = USTR_NULL;
  size_t clen = 0;
  size_t nlen = 0;
  size_t sz   = 0;
  size_t oh   = 0;
  size_t osz  = 0;
  size_t nsz  = 0;
  int alloc = USTR_FALSE;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (!len) return (USTR_TRUE);

  s1   = *ps1;
  clen = ustr_len(s1);
  if ((nlen = clen + len) < clen) /* overflow */
    goto fail_enomem;

  if (ustr__rw_add(s1, nlen, &sz, &oh, &osz, &nsz, &alloc))
  {
    size_t eos_len = sizeof(USTR_END_ALOCDx);

    if (alloc && !ustrp__rw_realloc(p, ps1, !!sz, osz, nsz))
      return (USTR_FALSE);
    ustr__terminate((*ps1)->data, ustr_alloc(*ps1), (oh - eos_len) + nlen);
    ustr__len_set(*ps1, nlen);
    
    USTR_ASSERT(ustr_assert_valid(*ps1));
    return (USTR_TRUE);
  }

  if (ustr_limited(s1))
  {
    ustr_setf_enomem_err(s1);
    return (USTR_FALSE);
  }
  
  if (!(ret = ustrp__dupx_undef(p, USTR__DUPX_FROM(s1), nlen)))
    goto fail_enomem;
  
  ustr__memcpy(ret, 0, ustr_cstr(s1), ustr_len(s1));
  ustrp__sc_free2(p, ps1, ret);

  USTR_ASSERT(ustr_assert_valid(*ps1));
  return (USTR_TRUE);

 fail_enomem:
  ustr_setf_enomem_err(s1);
  return (USTR_FALSE);
}
USTR_CONF_I_PROTO int ustr_add_undef(struct Ustr **ps1, size_t len)
{ return (ustrp__add_undef(0, ps1, len)); }
USTR_CONF_I_PROTO int ustrp_add_undef(void *p, struct Ustrp **ps1, size_t len)
{ return (ustrp__add_undef(p, USTR__PPTR(ps1), len)); }

USTR_CONF_i_PROTO
int ustrp__add_buf(void *p, struct Ustr **ps1, const void *s2, size_t len)
{
  size_t os1len = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (!len) return (USTR_TRUE);

  os1len = ustr_len(*ps1);
  if (!ustrp__add_undef(p, ps1, len))
    return (USTR_FALSE);

  ustr__memcpy(*ps1, os1len, s2, len);

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_add_buf(struct Ustr **ps1, const void *s2,size_t len)
{ return (ustrp__add_buf(0, ps1, s2, len)); }
USTR_CONF_I_PROTO
int ustrp_add_buf(void *p, struct Ustrp **ps1, const void *s2, size_t len)
{ return (ustrp__add_buf(p, USTR__PPTR(ps1), s2, len)); }

/* don't allocate, unless we have to ... also could be limited */
USTR_CONF_i_PROTO int ustr__treat_as_buf(const struct Ustr *s1, size_t len1,
                                         const struct Ustr *s2, size_t len2)
{
  USTR_ASSERT(!len1 || (len1 == ustr_len(s1)));
  USTR_ASSERT(len1 < (len1 + len2)); /* no overflow allowed */
  
  if (ustr_limited(s1))
    return (USTR_TRUE);

  if (ustr_owner(s1) && (ustr_size(s1) >= (len1 + len2)))
    return (USTR_TRUE);

  if (ustr_alloc(s1) && (s1 != s2))
    return (USTR_TRUE);
  
  return (USTR_FALSE);
}

USTR_CONF_i_PROTO
int ustrp__add(void *p, struct Ustr **ps1, const struct Ustr *s2)
{
  struct Ustr *ret = USTR_NULL;
  size_t len1 = 0;
  size_t len2 = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  USTR_ASSERT(ustr_assert_valid(s2));

  len1 = ustr_len(*ps1);
  len2 = ustr_len(s2);

  if (len1 > (len1 + len2))
    return (USTR_FALSE);
  
  if (ustr__treat_as_buf(*ps1, len1, s2, len2))
    return (ustrp__add_buf(p, ps1, ustr_cstr(s2), ustr_len(s2)));
    
  if (!len1)
  {  
    if (!(ret = ustrp__dupx(p, USTR__DUPX_FROM(*ps1), s2)))
      return (USTR_FALSE);
  }
  else
  {
    if (!(ret = ustrp__dupx_undef(p, USTR__DUPX_FROM(*ps1), len1 + len2)))
      return (USTR_FALSE);
    
    ustr__memcpy(ret,    0, ustr_cstr(*ps1), len1);
    ustr__memcpy(ret, len1, ustr_cstr(s2),   len2);
  }
  
  ustrp__sc_free2(p, ps1, ret);
  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_add(struct Ustr **ps1, const struct Ustr *s2)
{ return (ustrp__add(0, ps1, s2)); }
USTR_CONF_I_PROTO
int ustrp_add(void *p, struct Ustrp **ps1, const struct Ustrp *s2)
{ return (ustrp__add(p, USTR__PPTR(ps1), &s2->s)); }

USTR_CONF_i_PROTO
int ustrp__add_subustr(void *p, struct Ustr **ps1,
                       const struct Ustr *s2, size_t pos, size_t len)
{
  size_t clen = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  USTR_ASSERT(ustr_assert_valid(s2));
  USTR_ASSERT(pos);

  if (!len) return (USTR_TRUE);
  
  clen = ustr__valid_subustr(s2, pos, len);
  if (!clen)
    return (USTR_FALSE);
  if (len == clen)
    return (ustrp__add(p, ps1, s2));
  
  if (*ps1 != s2)
    return (ustrp__add_buf(p, ps1, ustr_cstr(s2) + pos - 1, len));

  if (!ustrp__add_undef(p, ps1, len))
    return (USTR_FALSE);
  s2 = *ps1;

  ustr__memcpy(*ps1, clen, ustr_cstr(s2) + pos - 1, len);

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_add_subustr(struct Ustr **ps1, const struct Ustr *s2,
                                       size_t pos, size_t len)
{ return (ustrp__add_subustr(0, ps1, s2, pos, len)); }
USTR_CONF_I_PROTO
int ustrp_add_subustrp(void *p, struct Ustrp **ps1, const struct Ustrp *s2,
                       size_t pos, size_t len)
{ return (ustrp__add_subustr(p, USTR__PPTR(ps1), &s2->s, pos, len)); }

USTR_CONF_i_PROTO
int ustrp__add_rep_chr(void *p, struct Ustr **ps1, char chr, size_t len)
{
  size_t os1len = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (!len) return (USTR_TRUE);

  os1len = ustr_len(*ps1);
  if (!ustrp__add_undef(p, ps1, len))
    return (USTR_FALSE);

  ustr__memset(*ps1, os1len, chr, len);

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_add_rep_chr(struct Ustr **ps1, char chr, size_t len)
{ return (ustrp__add_rep_chr(0, ps1, chr, len)); }
USTR_CONF_I_PROTO
int ustrp_add_rep_chr(void *p, struct Ustrp **ps1, char chr, size_t len)
{ return (ustrp__add_rep_chr(p, USTR__PPTR(ps1), chr, len)); }

/* ---------------- shortcut ---------------- */

USTR_CONF_I_PROTO void ustr_sc_free2(struct Ustr **ps1, struct Ustr *s2)
{ ustrp__sc_free2(0, ps1, s2); }
USTR_CONF_I_PROTO void ustrp_sc_free2(void *p,
                                      struct Ustrp **ps1, struct Ustrp *s2)
{ ustrp__sc_free2(p, USTR__PPTR(ps1), &s2->s); }

USTR_CONF_i_PROTO void ustrp__sc_free(void *p, struct Ustr **ps1)
{
  USTR_ASSERT(ps1);
  
  ustrp__free(p, *ps1);
  *ps1 = USTR_NULL;
}
USTR_CONF_I_PROTO void ustr_sc_free(struct Ustr **ps1)
{ ustrp__sc_free(0, ps1); }
USTR_CONF_I_PROTO void ustrp_sc_free(void *p, struct Ustrp **ps1)
{ ustrp__sc_free(p, USTR__PPTR(ps1)); }

USTR_CONF_i_PROTO void ustrp__sc_free_shared(void *p, struct Ustr **ps1)
{
  USTR_ASSERT(ps1);

  if (!*ps1)
    return;
  
  USTR_ASSERT(ustr_shared(*ps1));

  ustr_setf_owner(*ps1);
  ustrp__free(p, *ps1);
  *ps1 = USTR_NULL;
}
USTR_CONF_I_PROTO void ustr_sc_free_shared(struct Ustr **ps1)
{ ustrp__sc_free_shared(0, ps1); }
USTR_CONF_I_PROTO void ustrp_sc_free_shared(void *p, struct Ustrp **ps1)
{ ustrp__sc_free_shared(p, USTR__PPTR(ps1)); }

USTR_CONF_i_PROTO void ustrp__sc_del(void *p, struct Ustr **ps1)
{
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  if (!ustrp__del(p, ps1, ustr_len(*ps1)))
    /* very unlikely, but in this case ignore saving the options */
    ustrp__sc_free2(p, ps1, USTR(""));

  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  USTR_ASSERT(!ustr_len(*ps1));
}
USTR_CONF_I_PROTO void ustr_sc_del(struct Ustr **ps1)
{ ustrp__sc_del(0, ps1); }
USTR_CONF_I_PROTO void ustrp_sc_del(void *p, struct Ustrp **ps1)
{ ustrp__sc_del(p, USTR__PPTR(ps1)); }

USTR_CONF_i_PROTO
struct Ustr *ustrp__sc_dupx(void *p,
                            size_t sz, size_t rbytes, int exact, int emem,
                            struct Ustr **ps1)
{
  struct Ustr *ret = ustrp__dupx(p, sz, rbytes, exact, emem, *ps1);
  struct Ustr *tmp = USTR_NULL;
  
  if (!ret)
    return (USTR_NULL);

  /* swap */
  tmp  = *ps1;
  *ps1 = ret;
  ret  = tmp;
  
  return (ret);
}
USTR_CONF_I_PROTO
struct Ustr *ustr_sc_dupx(size_t sz, size_t rbytes, int exact, int emem,
                          struct Ustr **ps1)
{ return (ustrp__sc_dupx(0, sz, rbytes, exact, emem, ps1)); }
USTR_CONF_I_PROTO
struct Ustrp *ustrp_sc_dupx(void *p, size_t sz, size_t rbytes, int exact,
                            int emem, struct Ustrp **ps1)
{ return (USTRP(ustrp__sc_dupx(p, sz, rbytes, exact, emem, USTR__PPTR(ps1)))); }

USTR_CONF_i_PROTO struct Ustr *ustrp__sc_dup(void *p, struct Ustr **ps1)
{
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));
  
  return (ustrp__sc_dupx(p, USTR__DUPX_FROM(*ps1), ps1));
}
USTR_CONF_I_PROTO struct Ustr *ustr_sc_dup(struct Ustr **ps1)
{ return (ustrp__sc_dup(0, ps1)); }
USTR_CONF_I_PROTO struct Ustrp *ustrp_sc_dup(void *p, struct Ustrp **ps1)
{ return (USTRP(ustrp__sc_dup(p, USTR__PPTR(ps1)))); }

USTR_CONF_i_PROTO int ustrp__sc_ensure_owner(void *p, struct Ustr **ps1)
{
  struct Ustr *ret = USTR_NULL;
  size_t len = 0;
  
  USTR_ASSERT(ps1 && ustr_assert_valid(*ps1));

  if (ustr_owner(*ps1))
    return (USTR_TRUE);

  len = ustr_len(*ps1);
  if (!len)
    ret = ustrp__dupx_empty(p, USTR__DUPX_FROM(*ps1));
  else
    ret = ustrp__dupx_buf(p,   USTR__DUPX_FROM(*ps1), ustr_cstr(*ps1), len);
  
  if (!ret)
    return (USTR_FALSE);
      
  ustrp__sc_free2(p, ps1, ret);

  return (USTR_TRUE);
}
USTR_CONF_I_PROTO int ustr_sc_ensure_owner(struct Ustr **ps1)
{ return (ustrp__sc_ensure_owner(0, ps1)); }
USTR_CONF_I_PROTO int ustrp_sc_ensure_owner(void *p, struct Ustrp **ps1)
{ return (ustrp__sc_ensure_owner(p, USTR__PPTR(ps1))); }

