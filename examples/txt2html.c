#include "ustr.h"
/* this is a simple program showing how you can do the perl code:

#!/usr/bin/perl -p

# tchrist@perl.com
# Sunday, December 19th, 1999

BEGIN { print "<TT>\n" }        # and the spirit of awk...

# first kill all the tabs
1 while s{ \t + }
         { " " x (length($&)*8 - length($`)%8) }ex;

# then the four standard naughty bits 
s/&/&amp;/g;        # must remember to do this one first!
s/</&lt;/g;         # this is the most important one
s/>/&gt;/g;         # don't close too early
s/"/&quot;/g;       # only in embedded tags, i guess

# make lines break where they should
s/^\s*$/<P>/ || s/$/<BR>/;

# make sure spaces aren't squishticated so we
# can do indentation and properly align comments
s/( {2,})/'&nbsp;' x length($1)/ge;       

END { print "</TT>\n" }         # ...shall be with us always
*/

#include <getopt.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h> /* -r */

#ifndef _D_EXACT_NAMLEN
# define _D_EXACT_NAMLEN(d) strlen((d)->d_name)
#endif

static const char *prog_name = NULL;

static void die(const char *prog_name, const char *msg)
{
  fprintf(stderr, "%s: %s\n", prog_name, msg);
  exit (EXIT_FAILURE);
}

static void usage(int xcode)
{
  fprintf((xcode == EXIT_SUCCESS) ? stdout : stderr, "\
 Format: %s [-hV] [filename]...\n\
           -h           = Print help.\n\
           -V           = Print version.\n\
", prog_name);
  
  exit (xcode);
}

static void fp_loop(FILE *in, const char *prog_name)
{
  char buf_line[128]; /* can grow */
  Ustr *line = USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0);
  size_t line_num = 0;
  Ustr *beg = USTR1(\5, "<TT>\n");
  Ustr *end = USTR1(\6, "</TT>\n");
  
  if (!ustr_io_putfile(&beg, stdout))
    die(prog_name, strerror(errno));
    
  while (ustr_io_getline(&line, in))
  {
    size_t tab_pos = 0;
    size_t tab_off = 0;

    /* convert tabs to spaces */
    while ((tab_pos = ustr_srch_chr_fwd(line, tab_off, '\t')))
    {
      size_t tabs_len = ustr_spn_chr_fwd(line, tab_pos - 1, '\t');
      size_t spcs_len = (tabs_len * 8) - ((tab_pos - 1) % 8);

      ustr_sc_sub_rep_chr(&line, tab_pos, tabs_len, ' ', spcs_len);

      tab_off = tab_pos + spcs_len - 1;
    }
    
    if (ustr_spn_fwd(line, 0, USTR1(\1, " ")) == ustr_len(line) - 1)
      ustr_set(&line, USTR1(\3, "<P>"));
    else
    {
      size_t spcs_off = 0;
      size_t spcs_pos = 0;
      
      ustr_replace_cstr(&line, "&",  "&amp;",  0);
      ustr_replace_cstr(&line, "<",  "&lt;",   0);
      ustr_replace_cstr(&line, ">",  "&gt;",   0);
      ustr_replace_cstr(&line, "\"", "&quot;", 0);
      ustr_del(&line, 1);
      ustr_add_cstr(&line, "<BR>\n");

      while ((spcs_pos = ustr_srch_cstr_fwd(line, spcs_off, "  ")))
      {
        size_t spcs_len = ustr_spn_chr_fwd(line, spcs_pos - 1, ' ');
        size_t rep = spcs_len;
        Ustr *tmp = ustr_dupx_empty((rep * 6) + 20, 0, USTR_TRUE, USTR_FALSE);

        if (!tmp)
          die(prog_name, strerror(ENOMEM));
        
        while (rep--)
          ustr_add_cstr(&tmp, "&nbsp;");

        if (ustr_enomem(tmp) || !ustr_sc_sub(&line, spcs_pos, spcs_len, tmp))
          die(prog_name, strerror(ENOMEM));

        spcs_off = spcs_pos + ustr_len(tmp) - 1;
        
        ustr_free(tmp);
      }
    }
    
    if (ustr_enomem(line))
      die(prog_name, strerror(errno));
    if (!ustr_io_putfile(&line, stdout))
      die(prog_name, strerror(errno));
    
    if (line != USTR(buf_line)) /* re-init */
      ustr_sc_free2(&line, USTR_SC_INIT_AUTO(buf_line, USTR_FALSE, 0));
  }
  if (errno)
    die(prog_name, strerror(errno));

  if (!ustr_io_putfile(&end, stdout))
    die(prog_name, strerror(errno));  
}

static void file_loop(Ustr *fname, const char *prog_name, int sure_file)
{
  FILE *fp = NULL;
  char buf[32 * 1024];
  struct stat st[1];
  
  if (!sure_file &&
      ((stat(ustr_cstr(fname), st) == -1) || !S_ISREG(st->st_mode)))
    return;
  
  if (!(fp = fopen(ustr_cstr(fname), "rb")))
    die(prog_name, strerror(errno));
  
  setbuffer(fp, buf, sizeof(buf));
  
  if (!ustr_add_cstr(&fname, ":"))
    die(prog_name, strerror(ENOMEM));
  
  fp_loop(fp, prog_name);
  
  if (fclose(fp) == EOF)
    die(prog_name, strerror(errno));
}

static void loop(Ustr *fname, const char *prog_name)
{ /* NOTE: takes the reference to fname */
  file_loop(fname, prog_name, USTR_FALSE);

  ustr_free(fname);
}


int main(int argc, char *argv[])
{
  struct option long_options[] =
  {
   {"help", no_argument, NULL, 'h'},
   {"version", no_argument, NULL, 'V'},
   
   {NULL, 0, NULL, 0}
  };
  int scan = 0;
  int optchar = -1;
  
  USTR_CNTL_MALLOC_CHECK_BEG();
  
  if (!argc)
    exit (EXIT_FAILURE);
  
  if ((prog_name = strrchr(argv[0], '/')))
    ++prog_name;
  else
    prog_name = argv[0];
  
  while ((optchar = getopt_long(argc, argv, "hV", long_options, NULL)) != -1)
    switch (optchar)
    {
      case '?': usage(EXIT_FAILURE);
      case 'h': usage(EXIT_SUCCESS);
      case 'V':
        printf("%s version %s\n", prog_name, "1.0.0");
        exit (EXIT_SUCCESS);
    }
  
  argc -= optind;
  argv += optind;

  if (!argc)
    fp_loop(stdin, prog_name);
  
  scan = 0;
  while (scan < argc)
  {
    if (ustr_cmp_cstr_eq(USTR1(\1, "-"), argv[scan]))
      fp_loop(stdin, prog_name);
    else
    {
      Ustr *arg = USTR_NULL;
      
      if (!(arg = ustr_dup_cstr(argv[scan])))
        die(prog_name, strerror(ENOMEM));

      loop(arg, prog_name);
    }
    
    ++scan;
  }

  USTR_CNTL_MALLOC_CHECK_END();
  
  return (EXIT_SUCCESS);
}
