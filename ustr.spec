
Name: ustr
# FIXME: More C&P of VERSION
Version: 1.0.1
Release: 1%{?dist}
Summary: String library, very low memory overhead, simple to import
Group: System Environment/Libraries
License: MIT/LGPL/BSD
URL: http://www.and.org/ustr/
Source0: http://www.and.org/ustr/%{version}/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
# BuildRequires: make gcc sed

%description
 Micro string library, very low overhead from plain strdup() (Ave. 44% for
0-20B strings). Very easy to use in existing C code. At it's simplest you can
just include a single header file into your .c and start using it.
 This package also distributes pre-built shared libraries.

%package devel
Summary: Development files for %{name}
Group: Development/Libraries
# This isn't required, but Fedora policy makes it so
Requires: pkgconfig >= 0.14
Requires: %{name} = %{version}-%{release}

%description devel
 Header files for the Ustr string library, and the .so to link with.
 Also includes a %{name}.pc file for pkg-config usage.
 Includes the ustr-import tool, for if you jsut want to include
the code in your projects ... you don't have to link to the shared lib.

%package static
Summary: Static development files for %{name}
Group: Development/Libraries
Requires: %{name}-devel = %{version}-%{release}

%description static
 Static library for the Ustr string library.

%package debug
Summary: Development files for %{name}, with debugging options turned on
Group: Development/Libraries
# This isn't required, but Fedora policy makes it so
Requires: pkgconfig >= 0.14
Requires: %{name}-devel = %{version}-%{release}

%description debug
 Header files and dynamic libraries for a debug build of the Ustr string
library.
 Also includes a %{name}-debug.pc file for pkg-config usage.

%package debug-static
Summary: Static development files for %{name}, with debugging options turned on
Group: Development/Libraries
Requires: %{name}-debug = %{version}-%{release}

%description debug-static
 Static library for the debug build of the Ustr string library.

%prep
%setup -q

%build
# Last variable name explains itself.
make %{?_smp_mflags} all-shared CFLAGS="${CFLAGS:-%optflags}" HIDE= \
     DBG_ONLY_BAD_POLICIES_HAVE_THIS_EMPTY_CFLAGS=

%check
%if %{?chk}%{!?chk:1}
make check HIDE=
%endif

%install
rm -rf $RPM_BUILD_ROOT
make $@ install prefix=%{_prefix} \
                bindir=%{_bindir}         mandir=%{_mandir} \
                datadir=%{_datadir}       libdir=%{_libdir} \
                includedir=%{_includedir} \
                DESTDIR=$RPM_BUILD_ROOT LDCONFIG=/bin/true HIDE=

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%post debug -p /sbin/ldconfig

%postun debug -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libustr-1.0.so.*
%doc ChangeLog LICENSE* README NEWS

%files devel
%defattr(-,root,root,-)
%{_datadir}/ustr-%{version}
%{_bindir}/ustr-import
%{_includedir}/ustr.h
# We need to make sure we don't pickup:
#  ustr-debug.h and ustr-conf-debug.h
%{_includedir}/ustr-[bfimpsu]*.h
#{_includedir}/ustr-b.h
%{_includedir}/ustr-cmp.h
%{_includedir}/ustr-cntl.h
%{_includedir}/ustr-compiler.h
%{_includedir}/ustr-conf.h
#{_includedir}/ustr-fmt.h
#{_includedir}/ustr-io.h
#{_includedir}/ustr-main.h
#{_includedir}/ustr-parse.h
#{_includedir}/ustr-pool.h
#{_includedir}/ustr-sc.h
#{_includedir}/ustr-set.h
#{_includedir}/ustr-spn.h
#{_includedir}/ustr-srch.h
#{_includedir}/ustr-utf8.h
%{_libdir}/pkgconfig/ustr.pc
%{_libdir}/libustr.so
%{_datadir}/doc/ustr-devel-%{version}
%{_mandir}/man3/*

%files static
%{_libdir}/libustr.a

%files debug
%defattr(-,root,root,-)
%{_libdir}/libustr-debug-1.0.so.*
%{_libdir}/libustr-debug.so
%{_includedir}/ustr-debug.h
%{_includedir}/ustr-conf-debug.h
%{_libdir}/pkgconfig/ustr-debug.pc

%files debug-static
%{_libdir}/libustr-debug.a


%changelog
* Thu Aug  2 2007 James Antill <james@and.org> - 1.0.1-0.9.fc7
- Fix dups in -devel and -debug file lists.

* Wed Aug  1 2007 James Antill <james@and.org> - 1.0.1-0.8.fc7
- Required to make DBG_ONLY_BAD_POLICIES_HAVE_THIS_EMPTY_CFLAGS empty
- due to so called "review"

* Fri Jul 27 2007 James Antill <james@and.org> - 1.0.1-0.2.fc7
- Next test release of 1.0.1, lots of fixes from Fedora review.

* Wed Jul 25 2007 James Antill <james@and.org> - 1.0.1-0
- Test release of 1.0.1.

* Wed Jul 11 2007 James Antill <james@and.org> - 1.0.0-1
- Upgrade to 1.0.0
- Minor fixes on specfile

* Sun Jun  3 2007 James Antill <james@and.org> - 0.99.2-1
- Upgrade to 0.99.2

* Thu May 24 2007 James Antill <james@and.org> - 0.99.1-2
- Fix ver typo to be version.

* Fri May 18 2007 James Antill <james@and.org> - 0.99.1-1
- Use all-shared to get GCC-ish specific shared libs.

* Mon May 14 2007 James Antill <james@and.org> - 0.98.1-0
- Initial spec

