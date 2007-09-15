#! /bin/sh -e

if false; then
 echo "Not reached."
elif [ -f ./Makefile ]; then
	doln=false
else
  echo "Not in right place, goto the root."
  exit 1;
fi

make clean
make  CFLAGS="-g -fprofile-arcs -ftest-coverage -O0" \
     LDFLAGS="-g -fprofile-arcs -ftest-coverage -O0" check-lcov \
     -j $(getconf _NPROCESSORS_ONLN)
