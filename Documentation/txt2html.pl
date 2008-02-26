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

# FIXME: More C&P of VERSION
my $name = "Ustr - 1.0.4 reference documentation";

my $html_header = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">\n<html>\n<head>\n";
my $html_body = <<EOF;
    <style type="text/css">
      A:visited { color: #ff4040; }
      A:hover { color: #20b2aa; }

      P { text-indent: 1em; }

      body     { background: #FFF; color: #000; }

      h2.ind   { background: #DDF; }

      td.title { background: #DFD; }
      td.sect  { background: #DDF; }
      td.obj   { background: #DDD; }

      ul li                       { list-style-type: lower-roman; }
      ul li:hover                 { list-style-type: square; }
      ul:hover li                 { list-style-type: decimal; }
      ul li:hover ul li.obj       { list-style-type: decimal; }
      ul li:hover ul li.obj:hover { list-style-type: square; }
    </style>

  </head>
  <body>
EOF

my $html_footer = "\n</td></tr></table>\n</body></html>";

sub convert_index
  {
    my $ftype = shift;
    my $in_pre_tag = 0;
    my $txt_indx_ful = "<ul>\n";
    my $txt_indx_min = "<ul>\n";

    my $mem_count = 0;
    my $mem_tot   = 0;
    my $done = 0;

    while (<IN>)
      {
	if (/^(Constant|Function|Member): (.*)$/)
	  {
	    my $name = $2;
	    my $uri = $2;
	    $uri =~ s/[^[:alnum:]:_]//g;

	    $txt_indx_ful .= "<li class=\"obj\"><a href=\"#$uri\">$name</a>\n";
	    ++$mem_count;
	  }
        elsif (/^Section:\s*(.*)$/)
	  {
	    my $section = $1;
	    my $uri = $1;
	    $uri =~ s/\s/_/g;
	    $uri =~ s/[^[:alnum:]:_]//g;

	    if ($done)
	      {
		$txt_indx_min .= " ($mem_count)</a>\n";
		$mem_tot += $mem_count;
		$mem_count = 0;
		$txt_indx_ful .= "</ul>";
	      }
	    $done = 1;

	    $txt_indx_min .= "<li><a href=\"#indx-$uri\">$section";
	    $txt_indx_ful .= "<li><a id=\"indx-$uri\" href=\"#$uri\">$section</a>\n<ul>\n";
	  }
      }
    $mem_tot += $mem_count;

    OUT->print("<h1>", "$name -- $ftype ($mem_tot)", "</h1>", "\n");
    OUT->print("</td></tr></table><table width=\"90%\"><tr><td>\n");
    OUT->print("<h2 class=\"ind\">Index of sections</h2>\n");

    OUT->print($txt_indx_min . " ($mem_count)</a></ul>\n");
    OUT->print("<h2 class=\"ind\">Index of sections, and their contents</h2>\n");
    OUT->print($txt_indx_ful . "</ul></ul>\n");
  }

sub conv_html
{
  my $text = shift;

  if (defined ($text)) { $_ = $text; }

s/&/&amp;/g;        # must remember to do this one first!
s/</&lt;/g;         # this is the most important one
s/>/&gt;/g;         # don't close too early
s/\"/&quot;/g;       # only in embedded tags, i guess

  return ($_);
}

my $current_function = undef;

sub conv_A_refs
  {
    my $params = shift;
    my $markup = shift;

    s{(\W|^)(Ustr string)(\W|$)}
      {$1<a href="design">$2</a>$3}g;
    s{(\W|^)(Ustrp string)(\W|$)}
      {$1<a href="design">$2</a>$3}g;

    s{(\W|^)(Ustr pool)(\W|$)}
      {$1<a href="design#possibilities">$2</a>$3}g;

    s{([^#"_0-9a-z])ustr(p|)_([_0-9a-z]+)\(\)}
      {$1<a href="functions#ustr${2}_${3}">ustr${2}_$3()</a>}g;
    s{([^#"_0-9A-Z])USTR(P|)_([_0-9A-Z]+)\(\)}
      {$1<a href="functions#USTR${2}_${3}">USTR${2}_$3()</a>}g;

    s{([^#"_0-9A-Z])USTR(P|)([1-4]+)\(\)}
      {$1<a href="functions#USTR${2}${3}">USTR$3()</a>}g;

    if ($params && defined($current_function))
      {
	s{([^#"_0-9A-Z])USTR_([_0-9A-Z]+[*])}
	  {$1<a href="constants#$current_function">USTR_$2</a>}g;
      }

    s{([^#"_0-9A-Z])USTR_([_0-9A-Z]+)([^_0-9A-Z(*])}
      {$1<a href="constants#USTR_$2">USTR_$2</a>$3}g;

    s![*]{2}([^*]+)[*]{2}!<b>$1</b>!g if ($markup);
  }

sub convert()
  {
    my $in_pre_tag = 0;
    my $in_const = 0;

    while (<IN>)
      {
	my $next_in_const = 0;

	my $beg_replace = <<EOL;
</td></tr></table><table width="80%"><tr><td class="obj">
EOL

	if ($in_const)
	  {
	    $beg_replace = qw(<br>);
	  }
	if ($in_pre_tag)
	  {
	    $beg_replace = "</pre>$beg_replace";
	  }


	if (s!^(Constant|Function|Member): (.*)$!$beg_replace<strong>$1: </strong> $2! ||
	    s!^ Explanation:\s*$!</td></tr><tr><td><strong>Explanation:</strong></td></tr><tr><td><p>! ||
	    s!^ Note:\s*$!</td></tr><tr><td><strong>Note:</strong></td></tr><tr><td><p>! ||
	    s!^Section:\s*(.*)$!</td></tr></table><table width="90%"><tr><td class="sect"><h2>$1</h2>! ||
	    0)
	  {
	    $in_pre_tag = 0;
	    if (defined ($1))
            {
              if ($1 eq "Constant")
	      {
                my $iduri = $2;
		my $orig_str = $_;

		$next_in_const = 1;

		$iduri =~ s/([^[:alnum:]:_])//g;
		s!(: </strong> )(.*)!$1<a id="$iduri">$2</a>!;

		$current_function = undef;
	      }
              elsif ($1 eq "Function")
              {
                my $iduri = $2;
                $iduri =~ s/([^[:alnum:]:_])//g;

		s!(: </strong> )(.*)!$1<a id="$iduri">$2</a>!;

		$current_function = $iduri;
              }
              elsif ($1 eq "Member")
              {
		$current_function = undef;
	      }
              else
              { # Section...
                my $uri = $1;
                my $sect_str = $1;
                $uri =~ s/([^[:alnum:]:_])/sprintf("%%%02x", ord($1))/eg;

		if (/Constants passed to /)
		  { # More hacks...
		    s {
		       ([^#"_0-9a-z])(ustr_[_0-9a-z]+)\(\)
		      }
		      {
			$1\t<a id=\"$2()\">$2()</a>\n
		      }gx;
		  }

                s/<h2>/<h2><a id="$uri">/;
                s!</h2>!</a></h2>!;
		conv_A_refs(0, 0);
              }
            }
	  }
	elsif (m!^ ([A-Z][a-z]+)(\[\d\]|\[ \.\.\. \])?: (.*)$!)
	  {
            my $attr = $1;
            my $param_num = $2;
            my $text = $3;

            $text = conv_html($text);
            $_ = conv_html($_);
	    if (defined $param_num)
	      {
		if ($attr eq "Type")
		  {
		    $_ = "<br>$attr<strong>$param_num</strong>: $text";
		  }
		else
		  {
		    $_ = "</td></tr><tr><td>$attr<strong>$param_num</strong>: $text";
		  }
	      }
	    else
	      {
		if ($attr eq "Type" || $attr eq "Returns")
		  {
		    $_ = "<br>$attr: $text";
		  }
		else
		  {
		    $_ = "</td></tr><tr><td>$attr: $text";
		  }
	      }

	    conv_A_refs(1, 0);
	  }
	elsif ( $in_pre_tag && /^ \.\.\./)
	  {
	    if (/\.\.\.$/)
	      {
		conv_A_refs(1, 1);
		$_ = "</pre><p>$_</p><pre>";
		$in_pre_tag = 1;
	      }
	    else
	      {
		conv_A_refs(1, 1);
		$_ = "</pre><p>$_";
		$in_pre_tag = 0;
	      }
	  }
	elsif (/\.\.\.$/)
	  {
            $_ = conv_html($_);
	    conv_A_refs(1, 1);
	    $_ = "$_</p><pre>";
	    $in_pre_tag = 1;
	  }
	elsif (!$in_pre_tag && /^  /)
	  {
            $_ = conv_html($_);
	    conv_A_refs(1, 1);
	    $_ = "</p><p>$_";

	  }
	elsif (!$in_pre_tag)
	  {
	    conv_A_refs(1, 1);
	  }

	$in_const = $next_in_const;

	print OUT;
      }
  }

open (OUT, "> functions.html")    || die "open(functions.html): $!";

print OUT $html_header;
print OUT "<title>", "$name -- functions", "</title>", "\n";
print OUT $html_body;
print OUT "<table width=\"100%\"><tr><td class=\"title\">";

open (IN, "< $docs/functions.txt") || die "open(functions.txt): $!";

convert_index("functions");

open (IN, "< $docs/functions.txt") || die "open(functions.txt): $!";

convert();

print OUT $html_footer;

open (OUT, "> constants.html")     || die "open(constants.html): $!";

print OUT $html_header;
print OUT "<title>", "$name -- constants", "</title>", "\n";
print OUT $html_body;
print OUT "<table width=\"100%\"><tr><td class=\"title\">";

open (IN, "< $docs/constants.txt") || die "open(constants.txt): $!";

convert_index("constants");

open (IN, "< $docs/constants.txt") || die "open(constants.txt): $!";

convert();

print OUT $html_footer;

if (-r "structs.txt")
  {
    open (OUT, "> structs.html")     || die "open(struts.html): $!";

    print OUT $html_header;
    print OUT "<title>", "$name -- structs", "</title>", "\n";
    print OUT $html_body;
    print OUT "<table width=\"100%\"><tr><td bgcolor=\"#DDFFDD\">";

    open (IN, "< $docs/structs.txt") || die "open(structs.txt): $!";

    convert_index("structs");

    open (IN, "< $docs/structs.txt") || die "open(structs.txt): $!";

    convert();

    print OUT $html_footer;
  }

exit 0;
