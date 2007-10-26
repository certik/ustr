#include "ustr.h"

#include <errno.h>

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

/* simplest, just "create" a Ustr from a constant string */
static void hello_world_one(void)
{
  Ustr *hello_world = USTR1_CHK(\xC, "Hello world!");

  if (!ustr_io_putfileline(&hello_world, stdout))
    die("hello_world", strerror(errno));
}

static void hello_world_two(void)
{ /* next, create a Ustr from a printf() like format. This needs to be
   * free'd. */
  Ustr *out = ustr_dup_fmt("%s %s", "Hello", "world!");

  if (!out || !ustr_io_putfileline(&out, stdout))
    die("hello_world", strerror(errno));
}

static void hello_world_three(void)
{ /* manually create a Ustr, from multiple parts. Often "significantly faster"
   * than using ustr_*_fmt(), due to printf overhead. Still needs to allocate
   * memory, and maybe resize it. Still need to free it. */
  Ustr *hello = USTR1(\5, "Hello");
  Ustr *sp    = USTR1(\1, " ");
  Ustr *world = USTR1(\6, "world!");
  Ustr *out   = ustr_dupx_empty(1, 4, USTR_FALSE, USTR_FALSE);

  if (!out)
    die("hello_world", strerror(ENOMEM));
  
  ustr_add(&out, hello);
  ustr_add(&out, sp);
  ustr_add(&out, world);
  
  if (ustr_enomem(out)) /* check all 3 additions at once */
    die("hello_world", strerror(ENOMEM));
  
  if (!ustr_io_putfileline(&out, stdout))
    die("hello_world", strerror(errno));
  
  ustr_free(out);
}

static void hello_world_four(void)
{ /* manually create a Ustr, but use "auto" allocated storage
   * (stack instead of heap). As long as you don't use more than ustr_size()
   * you don't need to free. Also note that ustr_dup() will now always copy. */
  Ustr *sp    = USTR1(\1, " ");
  Ustr *world = USTR1(\6, "world!");
  char buf_out[1024] = USTR_BEG_FIXED2 "Hello";
  Ustr *out = USTR_SC_INIT_AUTO(buf_out, USTR_TRUE, 5);

  ustr_add(&out, sp);
  ustr_add(&out, world);

  /* in this can we know !ustr_enomem() as there is more than enough space */
  
  if (!ustr_io_putfileline(&out, stdout))
    die("hello_world", strerror(errno));
  
  /* ustr_free() not needed, because nothing was allocated.
   * Although it's often good to call it anyway, as it does no harm. */
}

int main(void)
{
  USTR_CNTL_MALLOC_CHECK_BEG(USTR_DEBUG);
  
  hello_world_one();
  hello_world_two();
  hello_world_three();
  hello_world_four();
  
  USTR_CNTL_MALLOC_CHECK_END();
  
  return (EXIT_SUCCESS);
}

