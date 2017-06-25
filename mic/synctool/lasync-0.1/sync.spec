%define buildroot %{_topdir}/BUILDROOT

Name:		lasync
Version:	0.1
Release:	1
Summary:	controller for lasync sync tool

Group:		Applications/System
License:	3 clause BSD License
URL:		http://www.lasync.com
Source:	        lasync-0.1-1.tar.gz
BuildRoot:	%{buildroot}

#BuildRequires:	gcc gcc-c++

%description
This is test package for demo purpose

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
/opt/lasync/sbin/lasync
/opt/lasync/syncdir/*
/opt/lasync/etc/*
/usr/sbin/lasync

%post

%postun



%changelog
* Fri Jul 12 2013 Leo.C.Wu <leo@lasync.com> -0.1-1
- Init build

