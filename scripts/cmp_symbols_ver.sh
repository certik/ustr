#! /bin/sh -e

if false; then
 echo "Not reached."
elif [ -f ./ustr.pc ]; then
        doln=false
else
  echo "Not in right place, goto to the root."
  exit 1;
fi

sort libustr.ver > /tmp/libustr.ver.sort
tr -d '; ' < /tmp/libustr.ver.sort  > /tmp/ustr.libver.sort.tr
egrep -v '^$' < /tmp/ustr.libver.sort.tr > /tmp/ustr.libver.sort.tr.grepv


./scripts/list_functions_src.pl | sort > /tmp/ustr.gen.sort

tr -d '() ' < /tmp/ustr.gen.sort > /tmp/ustr.gen.sort.tr

# -Bb doesn't work *sighs*
diff -u /tmp/ustr.libver.sort.tr.grepv /tmp/ustr.gen.sort.tr
