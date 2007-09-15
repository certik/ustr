#! /usr/bin/perl -w

use strict;
use FileHandle;

my $inc = undef;

if (0) {}
elsif (-r "./Makefile")
  {
    $inc ="./";
  }

if (!defined ($inc))
  {
    STDERR->print("Can't find Makefile.\n");
    exit (1);
  }

my $all = 0;

$all = 1 if (@ARGV);

my @files = qw(main set cmp srch spn fmt b io utf8 parse cntl sc pool ins sub replace split);

for my $name (@files) {
if (!open(IN, "< $inc/ustr-$name.h"))
  {
    die "open: $!\n";
  }

$/ = undef;

$_ = <IN>;

# Not valid for everything C, but good enough...

if ($all) {
  while (/^#define\s+(USTRP?_?[0-9a-zA-Z][0-9a-zA-Z_]*)\s*\(/gm)
    {
      print "$1()\n";
    }
}

# Note all macro function are above prototypes...

while (/^USTR_CONF_EI?_PROTO\s+[\s0-9a-zA-Z_*]*\s+\**(ustrp?_(_)?[0-9a-zA-Z][0-9a-zA-Z_]*)\(/gm)
  {
    print "$1()\n";
  }
}

