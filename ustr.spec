
Name: ustr
# FIXME: More C&P of VERSION
Version: 1.0.0
Release: 1%{?dist}
Summary: String library, very low memory overhead, simple to import
Group: System Environment/Libraries
License: MIT, LGPL, BSD
URL: http://www.and.org/ustr/
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
# BuildRequires: make gcc sed

%description
 Micro string library, very low overhead from plain strdup() (Ave. 44% for
0-20B strings). Very easy to use in existing C code. At it's simplest you can
just include a single header file into your .c and start using it.
 This package also distributes pre-built shared libraries.

%package devel
Summary: String library, very very low memory overhead, simple to import.
Group: Development/Libraries
# Requires: pkgconfig >= 0.14
Requires: %{name} = %{version}

%description devel
 Header files for the Ustr string library, and the .so to link with.
 Also includes a %{name}.pc file for pkg-config usage.
 Includes the ustr-import tool, for if you jsut want to include
the code in your projects ... you don't have to link to the shared lib.

%package static
Summary: String library, very very low memory overhead, simple to import.
Group: Development/Libraries
Requires: %{name}-devel = %{version}

%description static
 Static library for the Ustr string library.

%package debug
Summary: String library, very very low memory overhead, simple to import.
Group: Development/Libraries
Requires: %{name}-static = %{version}

%description debug
 Header files and dynamic libraries for a debug build of the Ustr string
library.
 Also includes a %{name}-debug.pc file for pkg-config usage.

%package debug-static
Summary: String library, very very low memory overhead, simple to import.
Group: Development/Libraries
Requires: %{name}-debug = %{version}

%description debug-static
 Static library for the debug build of the Ustr string library.

%prep
%setup -q

%build
make %{?_smp_mflags} all-shared

%check
%if %{?chk}%{!?chk:1}
make check
%endif

%install
rm -rf $RPM_BUILD_ROOT
make install bindir=%{_bindir}   mandir=%{_mandir} \
             datadir=%{_datadir} libdir=%{_libdir} DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libustr-1.0.so.*
%doc ChangeLog LICENSE* README NEWS

%files devel
%defattr(-,root,root,-)
%{_datadir}/ustr-%{version}/*
%{_datadir}/ustr-%{version}/.gdbinit
%{_bindir}/ustr-import
/usr/include/ustr.h
/usr/include/ustr-*.h
%{_libdir}/pkgconfig/ustr.pc
%{_libdir}/libustr.so
%doc
%{_datadir}/doc/ustr-devel-%{version}/*
%{_mandir}/man3/*

%files static
%{_libdir}/libustr.a

%files debug
%defattr(-,root,root,-)
%{_libdir}/libustr-debug-1.0.so.*
%{_libdir}/libustr-debug.so
/usr/include/ustr-debug.h
/usr/include/ustr-conf-debug.h
%{_libdir}/pkgconfig/ustr-debug.pc

%files debug-static
%{_libdir}/libustr-debug.a


%changelog
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

