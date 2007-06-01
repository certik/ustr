#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make();
  Ustrp *sp1 = USTRP("");
  unsigned int u16[9] = { 0, 1, 2, 3, 0x00FF, 0x0F0F, 0xF0F0, 0xFF00, 0xFFFF};
  unsigned int u32[9] = { 0x12345678U, 0x87654321U, 0x01234567U, 0x76543210U,
                         0xFF00FF00U, 0xF0F0F0F0U, 0x0F0F0F0FU, 0x00FF00FFU,
                         0xFFFFFFFFU };
  unsigned long long u64[7] = { 0x0123456789abcdefULL, 0xfedcba9876543210ULL,
                                0x00FF00FF00FF00FFULL, 0xFF00FF00FF00FF00ULL,
                                0x0F0F0F0F0F0F0F0FULL, 0xF0F0F0F0F0F0F0F0ULL,
                                0xFFFFFFFFFFFFFFFFULL };
  size_t num = 0;
  size_t off = 0;

  num = 0;
  while (num < 9)
  {
    ustr_add_b_uint16(&s1, u16[num]);
    ustrp_add_b_uint16(pool, &sp1, u16[num]);
    
    ustr_add_b_uint32(&s1, u16[num]);
    ustrp_add_b_uint32(pool, &sp1, u16[num]);
    
    ustr_add_b_uint64(&s1, u16[num]);
    ustrp_add_b_uint64(pool, &sp1, u16[num]);
    
    ++num;
  }  

  num = 0;
  while (num < 9)
  {
    ustr_add_b_uint32(&s1, u32[num]);
    ustrp_add_b_uint32(pool, &sp1, u32[num]);
    
    ustr_add_b_uint64(&s1, u32[num]);
    ustrp_add_b_uint64(pool, &sp1, u32[num]);
    
    ++num;
  }
  
  num = 0;
  while (num < 7)
  {
    ustr_add_b_uint64(&s1, u64[num]);
    ustrp_add_b_uint64(pool, &sp1, u64[num]);
    
    ++num;
  }

  ASSERT(!ustr_enomem(s1));
  ASSERT(!ustrp_enomem(sp1));
  
  /* parse the numbers out again ... */
  num = 0;
  off = 1;
  while (num < 9)
  {
    ASSERT(u16[num] == ustr_parse_b_uint16(s1, off));
    ASSERT(u16[num] == ustrp_parse_b_uint16(sp1, off));
    off += 2;
    
    ASSERT(u16[num] == ustr_parse_b_uint32(s1, off));
    ASSERT(u16[num] == ustrp_parse_b_uint32(sp1, off));
    off += 4;
    
    ASSERT(u16[num] == ustr_parse_b_uint64(s1, off));
    ASSERT(u16[num] == ustrp_parse_b_uint64(sp1, off));
    off += 8;
    
    ++num;
  }  

  num = 0;
  while (num < 9)
  {
    ASSERT(u32[num] == ustr_parse_b_uint32(s1, off));
    ASSERT(u32[num] == ustrp_parse_b_uint32(sp1, off));
    off += 4;
    
    ASSERT(u32[num] == ustr_parse_b_uint64(s1, off));
    ASSERT(u32[num] == ustrp_parse_b_uint64(sp1, off));
    off += 8;
    
    ++num;
  }
  
  num = 0;
  while (num < 7)
  {
    ASSERT(u64[num] == ustr_parse_b_uint64(s1, off));
    ASSERT(u64[num] == ustrp_parse_b_uint64(sp1, off));
    off += 8;
    
    ++num;
  }
  
  
  ustr_pool_free(pool);
  
  return (EXIT_SUCCESS);
}
