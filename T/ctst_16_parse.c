#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  unsigned int flg_sep   = USTR_FLAG_PARSE_NUM_SEP;
  unsigned int flg_ovr   = USTR_FLAG_PARSE_NUM_OVERFLOW;
  unsigned int flg_spc   = USTR_FLAG_PARSE_NUM_SPACE;
  unsigned int flg_no0   = USTR_FLAG_PARSE_NUM_NO_BEG_ZERO;
  unsigned int flg_nopm  = USTR_FLAG_PARSE_NUM_NO_BEG_PM;
  unsigned int flg_noneg = USTR_FLAG_PARSE_NUM_NO_NEGATIVE;
  unsigned int flg_ext   = USTR_FLAG_PARSE_NUM_EXACT;
  unsigned int flg_sep_spc = flg_sep | flg_spc;
  unsigned int ern = 0;

  ASSERT(4000   == ustr_parse_uint(USTR1(\x5, "4_000"),   flg_sep, NULL));
  ASSERT(0x4000 == ustr_parse_uint(USTR1(\x7, "0x4_000"), flg_sep, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_NONE);
  
  ASSERT(0 == ustr_parse_uint(USTR1(\x6, " 4_000"),      flg_sep, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OOB);
  ASSERT(0 == ustr_parse_uint(USTR1(\x6, " 4_000"), 16 | flg_sep, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OOB);
  
  ASSERT(8240   == ustr_parse_uint(USTR1(\x9, "  8_2_4_0"), flg_sep_spc, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_NONE);
  ASSERT(0x8421 == ustr_parse_uint(USTR1(\x7, " 84__21"),16|flg_sep_spc, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_NONE);
  
  ASSERT(0xFFFFFFFF == ustr_parse_ulong(USTR1(\x8, "FFFFFFFF"), 16, &ern));
  ASSERT(0xFFFFFFFF == ustr_parse_ulong(USTR1(\x9, "100000000"), 16, &ern));
  ASSERT(!ern);
  ASSERT(0xFFFFFFFF == ustr_parse_ulong(USTR1(\x9, "100000001"), 16, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustr_parse_ulong(USTR1(\x9, "100000001"), 16 | flg_ovr, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OVERFLOW);

  ASSERT(9 == ustr_parse_ushort(USTR1(\x4, "1001"), 2, &ern));
  ASSERT(!ern);
  ASSERT(9 == ustr_parse_short(USTR1(\x8, " +0b1001"), flg_spc, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustr_parse_short(USTR1(\x8, "+ 0b1001"), 0, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OOB);
  ASSERT(-9 == ustr_parse_short(USTR1(\x7, "-0b1001"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustr_parse_int(USTR1(\x8, "- 0b1001"), 0, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OOB);
  
  ASSERT(2 == ustr_parse_int(USTR1(\x8, " 0b10-01"), flg_spc | flg_ext, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OOB);
  ASSERT(2345 == ustr_parse_ulongx(USTR1(\x8, " +2,34,5"),
                                   10 | flg_ovr | flg_sep_spc,
                                   0, 2345, ",", &ern));
  ASSERT(!ern);

  ASSERT(-2345 == (int)ustr_parse_ulongx(USTR1(\x8, " -2,34,5"), 
                                         10 | flg_ovr | flg_sep_spc,
                                         2346, 2345, ",", &ern));
  ASSERT(!ern);
  ASSERT(-2346 == (int)ustr_parse_ulongx(USTR1(\x8, " -2,34,6"), 
                                         10 | flg_ovr | flg_sep_spc,
                                         2346, 2345, ",", &ern));
  ASSERT(!ern);
  ASSERT(0 == (int)ustr_parse_ulongx(USTR1(\x8, " -4,34,5"), 
                                     10 | flg_ovr | flg_sep_spc,
                                     2346, 2345, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OVERFLOW);
  
  ASSERT(0 == ustr_parse_ulongx(USTR1(\x7, "-4,34,5"), 
                                10 | flg_ovr | flg_sep | flg_noneg,
                                0, 8345, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_NEGATIVE);
  
  ASSERT(0 == ustr_parse_ulongx(USTR1(\x8, " -4,34,5"), 
                                flg_ovr | flg_sep_spc | flg_nopm,
                                8346, 8345, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OOB);
  ASSERT(0 == ustr_parse_ulongx(USTR1(\x8, " +4,34,5"), 
                                flg_ovr | flg_sep_spc | flg_nopm,
                                8346, 8345, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OOB);
  
  ASSERT(4345 == ustr_parse_uintmax(USTR1(\x9, " +04_34_5"),
                                    10 | flg_ovr | flg_sep_spc, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustr_parse_uintmax(USTR1(\x9, " +04_34_5"),
                                 10 | flg_ovr | flg_sep_spc | flg_no0, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_BEG_ZERO);

  ASSERT(0 == ustrp_parse_int(USTRP1(\x2, " 0"), flg_spc, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x4, "   0"), flg_spc | flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x2, "+0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x3, "0x0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_short(USTRP1(\x3, "0b0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_long(USTRP1(\x3, "0o0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x2, " 0"), flg_spc | flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x4, "   0"), flg_spc | flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x2, "+0"), flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_ulong(USTRP1(\x3, "0x0"), flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_ushort(USTRP1(\x3, "0b0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_uint(USTRP1(\x3, "0o0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_uintmax(USTRP1(\x4, "0x00"), flg_no0, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_BEG_ZERO);

  ASSERT(0 == ustrp_parse_ulongx(USTRP1(\x1, " "), 37|flg_spc, 0,8, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_ONLY_S);

  ASSERT(0 == ustrp_parse_ulongx(USTRP1(\x1, "+"), 1, 0,8, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_ONLY_SPM);
  ASSERT(0 == ustrp_parse_uintmaxx(USTRP1(\x1, "-"), 1, 0,8, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_ONLY_SPM);

  ASSERT(0 == ustrp_parse_uintmaxx(USTRP1(\x2, "0b"), 0, 0,8, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_ONLY_SPMX);
  ASSERT(0 == ustrp_parse_uintmaxx(USTRP1(\x2, "0o"), 0, 0,8, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_ONLY_SPMX);
  ASSERT(0 == ustrp_parse_uintmaxx(USTRP1(\x2, "0x"), 0, 0,8, ",", &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_ONLY_SPMX);

  ASSERT( 4 == ustrp_parse_short(USTRP1(\x4, "0o04"), 0, &ern));
  ASSERT(!ern);  
  ASSERT( 4 == ustrp_parse_int(USTRP1(\x2, "04"), 0, &ern));
  ASSERT(!ern);
  ASSERT(12 == ustrp_parse_long(USTRP1(\x4, "0o14"), 0, &ern));
  ASSERT(!ern);  
  ASSERT(-12 == ustrp_parse_intmax(USTRP1(\x4, "-014"), 0, &ern));
  ASSERT(!ern);
  
  ASSERT( 20 == ustrp_parse_ushort(USTRP1(\x5, "0O024"), 0, &ern));
  ASSERT(!ern);  
  ASSERT( 20 == ustrp_parse_uint(USTRP1(\x3, "024"), 0, &ern));
  ASSERT(!ern);
  ASSERT(140 == ustrp_parse_ulong(USTRP1(\x5, "0O214"), 0, &ern));
  ASSERT(!ern);  
  ASSERT(140 == ustrp_parse_uintmax(USTRP1(\x4, "0214"), 0, &ern));
  ASSERT(!ern);
  
  ASSERT(140 == ustrp_parse_uintmax(USTRP1(\x4, "0x8C"), 0, &ern));
  ASSERT(!ern);
  ASSERT(140 == ustrp_parse_uintmax(USTRP1(\x4, "0X8c"), 0, &ern));
  ASSERT(!ern);
  ASSERT(140 == ustrp_parse_uintmax(USTRP1(\xb, "0b1000_1100"), flg_sep, &ern));
  ASSERT(!ern);
  ASSERT(140 == ustrp_parse_uintmax(USTRP1(\xb, "0B1000_1100"), flg_sep, &ern));
  ASSERT(!ern);
  
  ASSERT(0xF0FfF0FFf0FfF0ffULL ==
         ustrp_parse_uintmax(USTRP1(\x15, "0xF0FfF0FFf0FfF0ffULL"),
                             flg_ovr, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_intmax(USTRP1(\x15, "0xF0FfF0FFf0FfF0ffULL"),
                                 flg_ovr, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OVERFLOW);
  
  ASSERT(0 == ustrp_parse_uintmax(USTRP1(\x16, "0x1F0FfF0FFf0FfF0ffULL"),
                                  flg_ovr, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OVERFLOW);
  
  ASSERT(0 == ustrp_parse_intmax(USTRP1(\x16, "0x1F0FfF0FFf0FfF0ffULL"),
                                 flg_ovr, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OVERFLOW);
  
  ASSERT(0 == ustrp_parse_intmax(USTRP1(\x17, "-0x1F0FfF0FFf0FfF0ffULL"),
                                 flg_ovr, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OVERFLOW);
  
  ASSERT(0 == ustrp_parse_uintmaxx(USTRP1(\x2, "-8"), flg_ovr, 7,7, "-",&ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OVERFLOW);
  ASSERT(-8 ==
         (int)ustrp_parse_uintmaxx(USTRP1(\x2, "-8"), flg_ovr, 8,7, "-",&ern));
  ASSERT(!ern);
  ASSERT(-7 ==
         (int)ustrp_parse_uintmaxx(USTRP1(\x2, "-8"), 0, 7,7, "-",&ern));
  ASSERT(!ern);
  ASSERT(-8 ==
         (int)ustrp_parse_uintmaxx(USTRP1(\x2, "-8"), 0, 8,7, "-", &ern));
  ASSERT(!ern);
  
  return (EXIT_SUCCESS);
}
