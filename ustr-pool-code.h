/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_POOL_H
#error " You should have already included ustr-pool.h, or just include ustr.h."
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

USTR_CONF_i_PROTO void *ustr__pool_sys_malloc(struct Ustr_pool *p, size_t len)
{
  struct Ustr__pool_si_base *sip = (struct Ustr__pool_si_base *)p;
  struct Ustr__pool_si_node *np;
  void *ret = USTR_CONF_MALLOC(len);

  USTR_ASSERT(sip);
  
  if (!ret)
    return (ret);

  if (!(np = USTR_CONF_MALLOC(sizeof(struct Ustr__pool_si_node))))
  {
    USTR_CONF_FREE(ret);
    return (0);
  }

  np->next = sip->beg;
  sip->beg = np;
  np->ptr  = ret;
  
  return (ret);
}

USTR_CONF_i_PROTO
void *ustr__pool_sys_realloc(struct Ustr_pool *p, void *old,
                             size_t olen, size_t nlen)
{
  struct Ustr__pool_si_base *sip = (struct Ustr__pool_si_base *)p;
  void *ret = 0;

  USTR_ASSERT(sip && ((old && sip->beg && sip->beg->ptr) || !olen));

  if (!nlen)
    ++nlen;
  
  if (olen && (sip->beg->ptr == old))
  { /* let the last allocated Ustrp grow/shrink */
    if ((ret = USTR_CONF_REALLOC(old, nlen)))
      sip->beg->ptr = ret;
  }
  else if (olen >= nlen) /* always allow reductions/nothing */
    return (old);
  else if ((ret = ustr__pool_sys_malloc(p, nlen)))
    memcpy(ret, old, olen);
  
  return (ret);
}

USTR_CONF_i_PROTO
void ustr__pool_sys_free(struct Ustr_pool *p, void *old)
{
  struct Ustr__pool_si_base *sip = (struct Ustr__pool_si_base *)p;

  if (sip->beg && (sip->beg->ptr == old))
  {
    struct Ustr__pool_si_node *op = sip->beg;

    sip->beg = op->next;
    
    USTR_CONF_FREE(op->ptr);
    USTR_CONF_FREE(op);
  }  
}

USTR_CONF_i_PROTO void ustr__pool__clear(struct Ustr__pool_si_base *base,
                                         int siblings)
{
  struct Ustr__pool_si_node *scan;
  
  if (!base)
    return;

  scan = base->beg;
  while (scan)
  {
    struct Ustr__pool_si_node *scan_next = scan->next;

    USTR_CONF_FREE(scan->ptr);
    USTR_CONF_FREE(scan);

    scan = scan_next;
  }
  base->beg = 0;

  if (siblings)
    ustr__pool__clear(base->next, USTR_TRUE);

  ustr__pool__clear(base->sbeg, USTR_TRUE);
}
USTR_CONF_i_PROTO void ustr__pool_clear(struct Ustr_pool *base)
{
  USTR_ASSERT(base);
  ustr__pool__clear((struct Ustr__pool_si_base *)base, USTR_FALSE);
}

USTR_CONF_i_PROTO void ustr__pool__free(struct Ustr__pool_si_base *base,
                                        int siblings)
{
  if (!base)
    return;
  
  if (siblings)
    ustr__pool__free(base->next, USTR_TRUE);
  ustr__pool__free(base->sbeg, USTR_TRUE);
  base->sbeg = 0;
  
  ustr__pool__clear(base, USTR_FALSE);
  USTR_CONF_FREE(base);
}
USTR_CONF_i_PROTO void ustr__pool_free(struct Ustr_pool *p)
{
  struct Ustr__pool_si_base *sip = (struct Ustr__pool_si_base *)p;
  
  USTR_ASSERT(p);

  if (sip->prev)
    sip->prev->next = sip->next;
  else if (sip->base)
    sip->base->sbeg = sip->next;

  if (sip->next)
    sip->next->prev = sip->prev;
  
  ustr__pool__free(sip, USTR_FALSE);
}

USTR_CONF_i_PROTO
struct Ustr_pool *ustr__pool_make_subpool(struct Ustr_pool *p)
{
  struct Ustr__pool_si_base *sip = (struct Ustr__pool_si_base *)p;
  struct Ustr__pool_si_base *tmp;

  if (!(tmp = USTR_CONF_MALLOC(sizeof(struct Ustr__pool_si_base))))

    return (0);

  tmp->cbs.pool_sys_malloc   = ustr__pool_sys_malloc;
  tmp->cbs.pool_sys_realloc  = ustr__pool_sys_realloc;
  tmp->cbs.pool_sys_free     = ustr__pool_sys_free;

  tmp->cbs.pool_make_subpool = ustr__pool_make_subpool;
  tmp->cbs.pool_clear        = ustr__pool_clear;
  tmp->cbs.pool_free         = ustr__pool_free;

  tmp->beg  = 0;
  tmp->sbeg = USTR__POOL_NULL;
  tmp->prev = USTR__POOL_NULL;

  if (!p)
    return (&tmp->cbs);
  
  if ((tmp->next = sip->sbeg))
    tmp->next->prev = tmp;
  sip->sbeg = tmp;

  tmp->base = sip;
  
  return (&tmp->cbs);
}

USTR_CONF_I_PROTO struct Ustr_pool *ustr_pool_make_pool(void)
{
  struct Ustr_pool *ret;
  struct Ustr__pool_si_base *tmp;

  if (!(ret = ustr__pool_make_subpool(0)))
    return (0);

  tmp = (struct Ustr__pool_si_base *)ret;

  tmp->next = USTR__POOL_NULL;
  tmp->base = USTR__POOL_NULL;

  return (&tmp->cbs);
}
