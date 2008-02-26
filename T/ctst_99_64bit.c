#include "tst.h"

static const char *rf = __FILE__;

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#endif

static int big_tst(size_t fsz, size_t usz, int hack, int tst_add)
{
#ifdef __linux__
  int fd = -1;
  void *ptr = NULL;
  Ustr *s3 = NULL;
  
  ASSERT(ustr_set_cstr(&s2, "/tmp/ustr-build-tst-XXXXXX"));
  if ((fd = mkstemp(ustr_wstr(s2))) == -1)
    goto fail_mkstemp;

  unlink(ustr_cstr(s2));
  
  usz /= 2;
  usz += 60;
  ASSERT(usz);
  if (!fsz)
    fsz = usz;
  
  if (ftruncate(fd, fsz) == -1)
    goto fail_ftrunc;
  ptr = mmap(0, fsz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED)
    goto fail_mmap;
  memset(ptr, '-', ((fsz < 32) ? fsz : 32));
  
  if (hack)
  {
    if (USTR_DEBUG)
      goto fail_init;

#if ! USTR_CONF_INCLUDE_INTERNAL_HEADERS
    goto fail_init;
#else
    /* massive hack,
       ustr_assert_valid() fails here, as we aren't terminating */
    s3 = ptr;
    
    s3->data[0] = USTR__BIT_ALLOCD | USTR__BIT_HAS_SZ | USTR__BIT_NEXACT;
    if (!USTR_CONF_HAVE_64bit_SIZE_MAX)
      s3->data[0] |= (1 << 2) | 1;
    else
      s3->data[0] |= (2 << 2) | 2;
    ustr__sz_set(s3, usz);
    ustr__len_set(s3, usz - 59);
    ustr__ref_set(s3, 1);
#endif
  }
  else
  {
    if (!(s3 = ustr_init_alloc(ptr, usz, 0, 0, 0, 0, usz - 59)))
      goto fail_init;
  }
  
  ASSERT(ustr_sized(s3));

  if (tst_add)
  {
    ASSERT(!ustr_add(&s3, s3));
    ASSERT(!ustr_replace(&s3, USTR1(\1, "-"), s3, 0));
    ASSERT(!ustr_replace_rep_chr(&s3, '-', 1, 'x', usz, 0));
  }

  munmap(ptr, fsz);
  close(fd);
  /* ignore it because it wasn't actually allocated from the system */
  
  return (EXIT_SUCCESS);
  
 fail_init:
 fail_mmap:
 fail_ftrunc:
  close(fd);
 fail_mkstemp:
  return (EXIT_FAILED_OK);
#endif
}

int tst(void)
{
  size_t end = USTR__SIZE_MAX;
  size_t large = 1;
  size_t small = 4096;
  size_t num = 0;
  Ustr *s3 = USTR1(\3, "124");
  
  ASSERT((s1 = ustr_dupx_empty(0, 0, 0, 0)));
  ASSERT(ustr_size_alloc(s1) > 0);

  /* max 32bit overhead */
  while (num < (1 + 4 + sizeof(USTR_END_ALOCDx)))
  {
    ASSERT(!ustr_add_undef(&s1, end - num));
    ASSERT(!ustr_dup_undef(     end - num));
    ASSERT(!ustr_ins_undef(&s3, 1, end - num));
    ASSERT(!ustr_set_undef(&s1, end - num));
    ++num;
  }
  while (num < (1 + 4 + sizeof(USTR_END_ALOCDx) + ustr_len(s3)))
  {
    ASSERT(!ustr_ins_undef(&s3, 1, end - num));
    ++num;
  }
  num = (1 + 4 + sizeof(USTR_END_ALOCDx));
  
  if (!USTR_CONF_HAVE_64bit_SIZE_MAX)
  {
    big_tst(small, end, USTR_TRUE, USTR_TRUE);
    big_tst(    0, end, USTR_TRUE, USTR_TRUE);
    return (EXIT_FAILED_OK);
  }

  /* max 64bit overhead */
  while (num < (1 + 2 + 8 + 8 + sizeof(USTR_END_ALOCDx)))
  {
    ASSERT(!ustr_add_undef(&s1, end - num));
    ASSERT(!ustr_dup_undef(     end - num));
    ASSERT(!ustr_ins_undef(&s3, 1, end - num));
    ASSERT(!ustr_set_undef(&s1, end - num));
    ++num;
  }
  while (num < (1 + 2 + 8 + 8 + sizeof(USTR_END_ALOCDx) + ustr_len(s3)))
  {
    ASSERT(!ustr_ins_undef(&s3, 1, end - num));
    ++num;
  }
  
  ustr_free(s1);

  if (USTR_CONF_USE_DYNAMIC_CONF)
  {
    assert(ustr_cntl_opt(USTR_CNTL_OPT_SET_REF_BYTES, 8));
    assert(ustr_cntl_opt(USTR_CNTL_OPT_SET_HAS_SIZE,  0));
    
    ASSERT((s1 = ustr_dupx_undef(0, 8, 0, 0, 0)));
    ASSERT(ustr_ro(s1));
    ASSERT((s1 = ustr_dupx_undef(1, 8, 0, 0, 0)));
    ASSERT(ustr_ro(s1));
  }    
    
  ASSERT((s1 = ustr_dupx_empty(0, 8, USTR_FALSE, USTR_FALSE)));
  ASSERT(ustr_sized(s1));
  ASSERT(ustr_size_alloc(s1) >= 14);
  ASSERT(ustr_size_overhead(s1) >= 14);
  ustr_free(ustr_dup(s1));
  
  big_tst(small, end, USTR_TRUE, USTR_TRUE);
  big_tst(    0, end, USTR_TRUE, USTR_TRUE);
  
  large *= 1024;
  large *= 1024;
  large *= 1024;
  large *= 9;
  
  return (big_tst(0, large, USTR_FALSE, USTR_FALSE));
}
