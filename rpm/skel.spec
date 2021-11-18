Name:           cello
Version:
Release:
Summary:        hello world sample
Summary(ja):    これは日本語のSummaryです

License:        closed license
URL:            www.seiwa.co.jp
Source:         %{name}-%{version}.tar.gz

BuildRequires:  gcc make cmake
Requires:       boost

%description
the long description for our hello world example implemented c.
line2

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%make_install

%files
%license LICENSE
%{_bindir}/%{name}

%changelog
* Thu Nov 18 2021 Keisuke Urakari <urakari@benest.co.jp>
- 
