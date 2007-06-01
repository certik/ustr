
#include "ustr.h"
#include <stdio.h>

 struct NM_Ustr
 {
   const char *ptr;
   size_t reference_count;
   size_t length;

   unsigned char flag_is_readonly            : 1;
   unsigned char flag_exact_byte_allocations : 1;
   unsigned char flag_has_enomem_error       : 1;
   unsigned char flag_reference_count_is_inf : 1;
 };

#define TST "Hello!"

/* for retarded Solaris printf */
#define CLU(x) ((unsigned long) x)

static void tst_ustr(const char *tst, size_t num, size_t rb, int exact)
{
  struct Ustr *s1 = ustr_dupx_cstr(rb, exact, 0, tst);

  /* NOTE: Using system *printf, so can't use %zu as Solaris is retarded */
  printf("\t Ustr%lu(%lu, %s)     %s= (%8lu / %-8lu = %5.2f%% )\n",
         CLU(num), CLU(rb), exact ? "TRUE" : "FALSE", exact ? " " : "",
         CLU(ustr_overhead(s1) + ustr_size(s1)),
         CLU(ustr_overhead(s1) + ustr_len(s1)),
         (100. * (ustr_overhead(s1) + ustr_size(s1))) / ustr_len(s1));

  ustr_sc_free(&s1);
}

static size_t min_pow(size_t num)
{
  size_t min_sz = 1;
  
  if (num > (USTR__SIZE_MAX / 2))
    return (USTR__SIZE_MAX);

  while (min_sz < num)
    min_sz <<= 1;

  return (min_sz);
}

static size_t min_size(size_t num)
{
  size_t min_sz = 1;
  
  if (num > ((USTR__SIZE_MAX / 4) * 3))
    return (USTR__SIZE_MAX);

  /* *2 is too much, we end up wasting a lot of RAM. So we do *1.5. */
  while (min_sz < num)
  {
    size_t adder = min_sz / 2;

    if (!adder) { min_sz = 2; continue; }

    min_sz += adder;
    if (min_sz >= num) break;
    min_sz += adder;
  }
      
  return (min_sz);
}

int main(int argc, char *argv[])
{
  struct Ustr *s1 = USTR("");
  static const char txt[] = "123456789 ";
  size_t beg = 0;
  size_t end = 6;
  size_t count = 0;

  if (argc == 3) {
    beg = atoi(argv[1]);
    end = atoi(argv[2]);
  }

  while (count < beg) {
    ustr_add_rep_chr(&s1, txt[count++ % 10], 1);
  }

  while (count < end) {

    ustr_add_rep_chr(&s1, txt[count++ % 10], 1);

    printf("String: %lu \"%s\"\n", CLU(ustr_len(s1)), ustr_cstr(s1));

    tst_ustr(ustr_cstr(s1), 0, 0, USTR_TRUE);
    tst_ustr(ustr_cstr(s1), 1, 0, USTR_FALSE);
    tst_ustr(ustr_cstr(s1), 2, 1, USTR_TRUE);
    tst_ustr(ustr_cstr(s1), 3, 1, USTR_FALSE);
    tst_ustr(ustr_cstr(s1), 4, 2, USTR_TRUE);
    tst_ustr(ustr_cstr(s1), 5, 2, USTR_FALSE);
    tst_ustr(ustr_cstr(s1), 6, 4, USTR_TRUE);
    tst_ustr(ustr_cstr(s1), 7, 4, USTR_FALSE);

    if (USTR_CONF_HAVE_68bit_SIZE_MAX) {
      tst_ustr(ustr_cstr(s1), 8, 8, USTR_TRUE);
      tst_ustr(ustr_cstr(s1), 9, 8, USTR_FALSE);
    }

    printf("\t strdup()            = (%8lu / %-8lu = %5.2f%% )\n",
           CLU(ustr_len(s1) + 1), CLU(ustr_len(s1)),
           100. * ((1 + ustr_len(s1)) / ustr_len(s1)));

    printf("\t NM_Ustr             = (%8lu / %-8lu = %5.2f%% )\n",
           CLU(sizeof(struct NM_Ustr) + ustr_len(s1) + 1),
           CLU(sizeof(struct NM_Ustr) + ustr_len(s1)),
           (100. * (sizeof(struct NM_Ustr) + ustr_len(s1) + 1)) / ustr_len(s1));
    printf("\t NM_Ustr x2          = (%8lu / %-8lu = %5.2f%% )\n",
           CLU(sizeof(struct NM_Ustr) + min_pow(ustr_len(s1) + 1)),
           CLU(sizeof(struct NM_Ustr) + ustr_len(s1)),
           (100. * (sizeof(struct NM_Ustr) + min_pow(ustr_len(s1) + 1))) / ustr_len(s1));
    printf("\t NM_Ustr xUstr       = (%8lu / %-8lu = %5.2f%% )\n",
           CLU(sizeof(struct NM_Ustr) + min_size(ustr_len(s1) + 1)),
           CLU(sizeof(struct NM_Ustr) + ustr_len(s1)),
           (100. * (sizeof(struct NM_Ustr) + min_size(ustr_len(s1) + 1))) / ustr_len(s1));
  }

  ustr_sc_free(&s1);

  return 0;
}
