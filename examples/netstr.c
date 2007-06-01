#include "ustr.h"

int main(int argc, char *argv[])
{
  Ustr **nstrs = calloc(argc, sizeof(Ustr *));
  Ustr *out = NULL;
  int scan = 1;
  size_t total = 0;
  
  if (!argc || !nstrs)
    exit (EXIT_FAILURE);

  while (scan < argc)
  {
    size_t len = strlen(argv[scan]);
    
    if (!(nstrs[scan] = ustr_dup_fmt("%zu:%s,", len, argv[scan])))
      exit (EXIT_FAILURE);
    total += ustr_len(nstrs[scan++]);
  }

  if (argc == 2)
  { /* one arg */
    out = nstrs[1];
    ustr_add_rep_chr(&out, '\n', 1);
  }
  else
  {
    size_t sz = 0;
    size_t rbytes = 0;
  
    sz = total + (sizeof(size_t) * 4) + 6; /* close to total size needed */
    if (!(out = ustr_dupx_fmt(sz, rbytes, USTR_TRUE, USTR_FALSE, "%zu:",total)))
      exit (EXIT_FAILURE);

    scan = 1;
    while (scan < argc)
    {
      ustr_add(&out, nstrs[scan]);
      ustr_free(nstrs[scan++]); /* to make it allocation "clean" */
    }
    
    ustr_add_cstr(&out, ",\n");
  }
  
  if (ustr_enomem(out)) /*  Not really needed for the multi arg. case unless,
                         * we screwed up sz. */
    exit (EXIT_FAILURE);

  if (!ustr_io_putfile(&out, stdout))
    exit (EXIT_FAILURE);
  ustr_free(out); /* to make it allocation "clean" */
  
  return (EXIT_SUCCESS);
}

