#! /bin/sh -e

if false; then
 echo "Not reached."
elif [ -f ./ustr.pc ]; then
        doln=false
else
  echo "Not in right place, goto to the root."
  exit 1;
fi

./scripts/list_functions_doc.sh   | sort > /tmp/ustr.doc.sort
./scripts/list_functions_src.pl x | sort > /tmp/ustr.gen.sort

diff -u /tmp/ustr.doc.sort /tmp/ustr.gen.sort
