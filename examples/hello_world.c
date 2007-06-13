#include "ustr.h"

#include <errno.h>

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

int main(void)
{
  Ustr *hello = USTR1(\5, "Hello");
  Ustr *sp    = USTR1(\1, " ");
  Ustr *world = USTR1(\6, "world!");
  Ustr *out = NULL;

  out = hello;
  if (!ustr_io_putfile(&out, stdout))
    die("hello_world", strerror(errno));
  out = sp;
  if (!ustr_io_putfile(&out, stdout))
    die("hello_world", strerror(errno));
  out = world;
  if (!ustr_io_putfileline(&out, stdout))
    die("hello_world", strerror(errno));

  if (!(out = ustr_dup_empty()))
    die("hello_world", strerror(ENOMEM));
  ustr_add(&out, hello);
  ustr_add(&out, sp);
  ustr_add(&out, world);
  if (ustr_enomem(out)) /* check all 3 additions at once */
    die("hello_world", strerror(ENOMEM));
  
  if (!ustr_io_putfileline(&out, stdout))
    die("hello_world", strerror(errno));
  
  ustr_free(out);
  
  return (EXIT_SUCCESS);
}

