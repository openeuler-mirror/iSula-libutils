%global _version 1.0.0
%global _release 20200519.220857.git3a7aa4a4
Name:      isula_libutils
Version:   %{_version}
Release:   %{_release}
Summary:   Provide json parse and log common functions
Group:     System Environment/Libraries
License:   LGPLv2.1+
URL:       iSula_libutils
Source0:   %{name}-1.0.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}

BuildRequires: gcc
BuildRequires: cmake
BuildRequires: zlib-devel yajl-devel gtest-devel

Requires:      yajl zlib

%ifarch x86_64 aarch64
Provides:       lib%{name}.so()(64bit)
%endif

%description
Provide common functions to iSula, such as log functions, json parser and so on.

%package devel
Summary: Huawei json and log C Library
Group:   Libraries
ExclusiveArch:  x86_64 aarch64
Requires:       %{name} = %{version}-%{release}

%description devel
the %{name}-libs package contains libraries for running %{name} applications.

%global debug_package %{nil}

%prep
%setup -c -n %{name}-%{version}

%build
mkdir -p build
cd build
%cmake -DDEBUG=OFF -DLIB_INSTALL_DIR=%{_libdir} ../
%make_build

%install
rm -rf %{buildroot}
cd build
install -d $RPM_BUILD_ROOT/%{_libdir}
install -m 0644 ./src/libisula_libutils.so        %{buildroot}/%{_libdir}/libisula_libutils.so

install -d $RPM_BUILD_ROOT/%{_libdir}/pkgconfig
install -m 0644 ./conf/isula_libutils.pc      %{buildroot}/%{_libdir}/pkgconfig/isula_libutils.pc

install -d $RPM_BUILD_ROOT/%{_includedir}/isula_libutils
install -m 0644 ../build/json/*.h  %{buildroot}/%{_includedir}/isula_libutils/
install -m 0644 ../src/json/*.h  %{buildroot}/%{_includedir}/isula_libutils/
install -m 0644 ../third_party/log.h  %{buildroot}/%{_includedir}/isula_libutils/log.h

find %{buildroot} -type f -name '*.la' -exec rm -f {} ';'
find %{buildroot} -name '*.a' -exec rm -f {} ';'
find %{buildroot} -name '*.cmake' -exec rm -f {} ';'

%clean
rm -rf %{buildroot}

%pre

%post   -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/lib%{name}.so*

%files devel
%defattr(-,root,root,-)
%{_includedir}/%{name}/*.h
%{_libdir}/pkgconfig/%{name}.pc


%changelog
