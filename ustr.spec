
%define show_all_cmds       0
%define broken_fed_dbg_opts 0
%define multilib_inst       1

%if %{show_all_cmds}
%define policy_cflags_hide HIDE=
%else
%define policy_cflags_hide %{nil}
%endif

%if %{broken_fed_dbg_opts}
# Variable name explains itself.
%define policy_cflags_broken DBG_ONLY_BAD_POLICIES_HAVE_THIS_EMPTY_CFLAGS=
%else
%define policy_cflags_broken %{nil}
%endif

%define policy_cflags %{policy_cflags_hide}  %{policy_cflags_broken}

%if %{multilib_inst}
%define ustr_make_install install-multilib-linux
%else
%define ustr_make_install install
%endif


Name: ustr
Version: 1.0.4
Release: 1%{?dist}
Summary: String library, very low memory overhead, simple to import
Group: System Environment/Libraries
License: MIT or LGPLv2+ or BSD
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
make %{?_smp_mflags} all-shared CFLAGS="${CFLAGS:-%optflags}" %{policy_cflags}

%check
%if %{?chk}%{!?chk:1}
make %{?_smp_mflags} check CFLAGS="${CFLAGS:-%optflags}" %{policy_cflags}
%endif

%install
rm -rf $RPM_BUILD_ROOT
make $@ %{ustr_make_install} prefix=%{_prefix} \
                bindir=%{_bindir}         mandir=%{_mandir} \
                datadir=%{_datadir}       libdir=%{_libdir} \
                includedir=%{_includedir} libexecdir=%{_libexecdir} \
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
%if %{multilib_inst}
%{_libexecdir}/ustr-%{version}/ustr-import*
%endif
%{_includedir}/ustr.h
%{_includedir}/ustr-*.h
%exclude %{_includedir}/ustr*debug*.h
%{_libdir}/pkgconfig/ustr.pc
%{_libdir}/libustr.so
%{_datadir}/doc/ustr-devel-%{version}
%{_mandir}/man1/*
%{_mandir}/man3/*

%files static
%{_libdir}/libustr.a

%files debug
%defattr(-,root,root,-)
%{_libdir}/libustr-debug-1.0.so.*
%{_libdir}/libustr-debug.so
%{_includedir}/ustr*debug*.h
%{_libdir}/pkgconfig/ustr-debug.pc

%files debug-static
%{_libdir}/libustr-debug.a


%changelog
* Mon Oct 29 2007 James Antill <james@and.org> - 1.0.2-1
- New release

* Tue Aug 28 2007 James Antill <jantill@redhat.com> - 1.0.1-6
- Add options for fedora policy brokeness, so it's easy to undo.
- Rebuild for buildid.

* Wed Aug  8 2007 James Antill <james@and.org> - 1.0.1-5
- Import fix for ustr-import, wrt. repl <=> replace

* Sun Aug  5 2007 James Antill <james@and.org> - 1.0.1-4
- Patches for minor GIT HEAD documentation fixes.
- Install mkdir_p and fgrep examples.

* Sat Aug  4 2007 James Antill <james@and.org> - 1.0.1-2
- First upload to Fedora repos.

* Fri Aug  3 2007 James Antill <james@and.org> - 1.0.1-0.10.fc7
- Re-fix dups in -devel and -debug file lists.
- Change license to new format

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

