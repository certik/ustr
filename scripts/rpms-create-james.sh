#! /bin/bash -e

pkg=ustr
# FIXME: More C&P of VERSION
v="1.0.1"

if [ ! -f ustr-main.h ]; then
  if [ -f ustr/ustr-main.h ]; then
    cd ustr
  else
    echo "No VERSION, $pkg.spec or configure file." 1&>2
    exit 1
  fi
fi

s="`pwd`"
cd ../build/$pkg

rm -rf $pkg-$v
cp -a $s ./$pkg-$v
cd ./$pkg-$v

./scripts/clean.sh full
( cd Documentation && make )

rm -rf tmp

# Backup files...
find . \
 \( -name "*.o" -o -name ".*[%~]" -o -name "*[%~]" -o -name "#*#" \) \
 -print0 | xargs --no-run-if-empty -0 rm -f

# git
rm -rf .git

# Arch stuff...
rm -rf ./{arch}
find . -name .arch-ids -type d -print0 | xargs -0 rm -rf

# Create tarballs/RPMS
cp $s/$pkg.spec .

cd ..

chk=1
rel=1
if [ "x$1" = "xnochk" ]; then
echo Not doing checking.
chk=0
shift
else
echo Doing checking.
args="$args  --define \"chk 1\""
fi

if [ "x$1" = "xrel" ]; then
shift
echo Using custom release $1.
rel=$1
shift
else
echo Using normal release of 1.
fi

perl -i -pe "s/^Release: 1/Release: $rel/" $pkg-$v/$pkg.spec

tar -cf   $pkg-$v.tar $pkg-$v
bzip2 -9f $pkg-$v.tar

tar -cf   $pkg-$v.tar $pkg-$v
gzip -9f  $pkg-$v.tar

# sudo rpmbuild -ts --define "chk $chk" $pkg-$v.tar.gz
rpmbuild --define "_sourcedir `pwd`/tmp"   --define "_specdir `pwd`/tmp" \
         --define "_srcrpmdir `pwd`" -ts \
  $pkg-$v.tar.gz

mock --autocache $pkg-$v-$rel*.src.rpm

ls -ahslF `pwd`/$pkg-$v-$rel*.src.rpm
