#! /usr/bin/perl -w

use strict;
use FileHandle;

my $docs = undef;

if (0) {}
elsif (-d "./Documentation") # In main dir...
  {
    $docs ="./Documentation";
  }
elsif (-d "../Documentation") # Probably in the docs dir...
  {
    $docs ="../Documentation";
  }

if (!defined ($docs))
  {
    STDERR->print("Can't find Documentation.\n");
    exit (1);
  }

my $hdr_date = `date '+%d-%b-%Y'`;
chomp($hdr_date);

# FIXME: More C&P of VERSION
my $hdr_ver = "1.0.4";

my $man_funcs_header = <<EOL;
.TH ustr 3 "$hdr_date" "Ustr $hdr_ver" "Ustr String Library"
.SH "NAME"
ustr \\- ustr string library functions
.SH "SYNOPSIS"
.in \\w'  'u
#include "ustr.h"
.sp
EOL

my $man_consts_header = <<EOL;
.TH ustr_const 3 "$hdr_date" "Ustr $hdr_ver" "Ustr String Library"
.SH "NAME"
ustr_const \\- ustr string library constants
.SH "SYNOPSIS"
.in \\w'  'u
#include "ustr.h"
.sp
EOL

my $man_funcs_desc = <<EOL;
.ti
.SH "DESCRIPTION"
 A very simple overview is that you can use ustr_dup_cstr() to convert a C\\-style
to a Ustr, and ustr_cstr() to convert it back. You can also use USTR1() to
create constant/read\\-only strings, and USTR_SC_INIT_AUTO() to create strings
that use stack space upto a certain amount.
 Use is somewhat unique in that, by default, it has an average overhead of only
55% overhead over strdup(), for strings in the range of 0 to 20 bytes. Or, to
put it another way it only uses 8 bytes to store a 2 byte string where
strdup() uses 3.
EOL

my $man_consts_desc = <<EOL;
.ti
.SH "DESCRIPTION"
EOL

my $man_funcs_seealso = <<EOL;
.SH "SEE ALSO"
.BR ustr_const (3)
EOL

my $man_consts_seealso = <<EOL;
.SH "SEE ALSO"
.BR ustr (3)
EOL

sub synopsis
  {
    my $funcs = shift;
    my $func = undef;
    my $args = 0;

    sub fin
      {
	my $funcs = shift;
	my $args = shift;

	if (!$funcs) { return; }
	if (!$args) { die "Parameter missing in docs"; }
	OUT->print(");\n");
      }

    while (<IN>)
      {
	if (s!^(Function): (.*)\(\)$!$2! ||
	    s!^(Constant|Member): (.*)$!$2!)
	  {
	    chomp;

	    if (!$funcs)
	      {
		OUT->print(".br\n");
		OUT->print(".ti \\w'  'u\n");
		OUT->print("\\fB$_\\fR\n");
	      }
	    else
	      {
		if ($func) { fin($funcs, $args); }
		$args = 0;
		$func = $_;
	      }
	    if (( $funcs && $1 ne "Function") ||
		(!$funcs && $1 eq "Function") ||
		0)
	      { die "Bad txt documentation."; }
	  }
	elsif ($funcs && /^ Type: (.*)/)
	  {
	    my $spc = " ";

	    $_ = $1;
	    chomp;

	    if (/\*$/)
	      { $spc = ""; }

	    OUT->print(".br\n"); # FIXME: \w doesn't account for space filling
	    OUT->print(".in \\w'  $_$spc\\fB$func\\fR('u\n");
	    OUT->print(".ti \\w'  'u\n");
	    OUT->print("$_$spc\\fB$func\\fR(");
	  }
	elsif ($funcs && /^ Type\[.+\]: (.*)/)
	  {
	    $_ = $1;
	    chomp;
	    if ($args)
	      {
		OUT->print(", ");
	      }

	    ++$args;
	    OUT->print("$1");
	  }
	elsif (/^Section:/)
	  {
	    if ($func) { fin($funcs, $args); }
	    $args = 0;
	    $func = 0;
	    OUT->print(".sp\n");
	  }
      }

    fin($funcs, $args);
    OUT->print("\n");
  }

sub conv_A_refs
  {
    my $params = shift;
    my $markup = shift;
    my $raw    = shift;

    s/\\/\\\\/g if (!$raw);
    s/-/\\-/g   ;
    s![*]{2}([^*]+)[*]{2}!\\fB$1\\fR!g if ($markup);
  }

sub convert()
  {
    my $in_pre_tag = "";
    my $in_const = 0;

    while (<IN>)
      {
	my $next_in_const = 0;

	my $beg_replace = "$in_pre_tag.br\n";

	if ($in_const)
	  {
	    $beg_replace = "$in_pre_tag\n";
	  }

	if (s!^(Constant|Function|Member): (.*)$!$beg_replace\\fB$1: \\fR $2! ||
	    s!^ Explanation:\s*$!$beg_replace\\fBExplanation:\\fR! ||
	    s!^ Note:\s*$!.sp\n\\fBNote:\\fR! ||
	    s!^Section:\s*(.*)$!.SH $1! ||
	    0)
	  {
	    s/-/\\-/g  ;

	    if (defined ($1) && ($1 eq "Function"))
              {
	        $_ = ".ti -2\n" . $_;
              }
	    if (defined ($1) && ($1 eq "Constant"))
	      {
		$next_in_const = 1;
	      }
	  }
	elsif (m!^ ([A-Z][a-z]+)(\[\d\]|\[ \.\.\. \])?: (.*)$!)
	  {
	    if (defined $2)
	      {
		if ($1 eq "Type")
		  {
		    $_ = ".br\n$1\\fB$2\\fR: $3\n";
		  }
		else
		  {
		    $_ = ".br\n$1\\fB$2\\fR: $3\n";
		  }
	      }
	    else
	      {
		if ($1 eq "Type")
		  {
		    $_ = ".br\n$1: $3\n";
		  }
		else
		  {
		    $_ = ".br\n$1: $3\n";
		  }
	      }

	    conv_A_refs(0, 0, 1);
	  }
	elsif (/^ \.\.\./)
	  {
	    if (/\.\.\.$/)
	      {
		conv_A_refs(1, 1, 0);
		$_ = ".Ve\n$_.Vb 4\n";
		$in_pre_tag = "\n.Ve";
	      }
	    else
	      {
		conv_A_refs(1, 1, 0);
		$_ = ".Ve\n$_";
		$in_pre_tag = "";
	      }
	  }
	elsif (/\.\.\.$/)
	  {
	    conv_A_refs(1, 1, 0);
	    $_ = "$_\n.Vb 4";
	    $in_pre_tag = "\n.Ve";
	  }
	elsif (!$in_pre_tag)
	  {
	    if (!/^$/)
	      {
		chomp;
		if (/^  /)
		  {
		    $_ = "\n.br\n" . $_;
		  }
	      }
	    conv_A_refs(1, 1, 0);
	  }
        else
          {
	    conv_A_refs(1, 1, 0);
	  }

	$in_const = $next_in_const;

	OUT->print($_);
      }
  }

# MAIN

# functions man page...
open (IN, "< $docs/functions.txt") || die "open(functions.txt): $!";

open (OUT, "> ustr.3")        || die "open(ustr.3): $!";

OUT->print($man_funcs_header);

synopsis(1);

open (IN, "< $docs/functions.txt") || die "open(functions.txt): $!";

OUT->print($man_funcs_desc);

convert();

OUT->print($man_funcs_seealso);

# constants man page...
open (IN, "< $docs/constants.txt") || die "open(constants.txt): $!";

open (OUT, "> ustr_const.3")        || die "open(ustr_const.3): $!";

OUT->print($man_consts_header);

synopsis(0);

open (IN, "< $docs/constants.txt") || die "open(constants.txt): $!";

OUT->print($man_consts_desc);

convert();

OUT->print($man_consts_seealso);

exit (0);
