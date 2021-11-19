Name:           librpmtest
Version:
Release:
Summary:        hello world sample
Summary(ja):    これは日本語のSummaryです

License:        closed license
URL:            www.seiwa.co.jp
Source:         %{name}-%{version}.tar.gz

BuildRequires:  gcc make cmake
#Requires:       

%description
the long description for our hello world example implemented c.
line2

%global debug_package %{nil}

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%make_install

%files
%license LICENSE
%{_libdir}/%{name}.so
%{_libdir}/%{name}.%{version}.so
/usr/include/rpm-test.h

%changelog
* Thu Nov 18 2021 Keisuke Urakari <urakari@benest.co.jp>
- 
