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
  ASSERT(1 == ustr_parse_ulong(USTR1(\x9, "100000001"), 16, &ern));
  ASSERT(!ern);
  ASSERT(0xFFFFFFFF == ustr_parse_ulong(USTR1(\x9, "100000001"),
                                        16 | flg_ovr, &ern));
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
                                   2345, ',', &ern));
  ASSERT(!ern);

  ASSERT(-2345 == (int)ustr_parse_ulongx(USTR1(\x8, " -2,34,5"), 
                                         10 | flg_ovr | flg_sep_spc,
                                         2345, ',', &ern));
  ASSERT(!ern);
  ASSERT(-2346 == (int)ustr_parse_ulongx(USTR1(\x8, " -2,34,6"), 
                                         10 | flg_ovr | flg_sep_spc,
                                         2345, ',', &ern));
  ASSERT(!ern);
  ASSERT(-2346 == (int)ustr_parse_ulongx(USTR1(\x8, " -4,34,5"), 
                                         10 | flg_ovr | flg_sep_spc,
                                         2345, ',', &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OVERFLOW);
  
  ASSERT(0 == ustr_parse_ulongx(USTR1(\x7, "-4,34,5"), 
                                10 | flg_ovr | flg_sep | flg_noneg,
                                8345, ',', &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_NEGATIVE);
  
  ASSERT(0 == ustr_parse_ulongx(USTR1(\x8, " -4,34,5"), 
                                flg_ovr | flg_sep_spc | flg_nopm,
                                8345, ',', &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_OOB);
  ASSERT(0 == ustr_parse_ulongx(USTR1(\x8, " +4,34,5"), 
                                flg_ovr | flg_sep_spc | flg_nopm,
                                8345, ',', &ern));
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
  ASSERT(0 == ustrp_parse_int(USTRP1(\x3, "0b0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x3, "0o0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x2, " 0"), flg_spc | flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x4, "   0"), flg_spc | flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x2, "+0"), flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x3, "0x0"), flg_no0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x3, "0b0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x3, "0o0"), 0, &ern));
  ASSERT(!ern);
  ASSERT(0 == ustrp_parse_int(USTRP1(\x4, "0x00"), flg_no0, &ern));
  ASSERT(ern == USTR_TYPE_PARSE_NUM_ERR_BEG_ZERO);
  
  return (EXIT_SUCCESS);
}
