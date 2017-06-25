%define buildroot %{_topdir}/BUILDROOT

Name:		ladaemon
Version:	0.1
Release:	1
Summary:	daemon client for ladaemon sync tool
Group:		Applications/System
License:	3 clause BSD License
URL:		http://www.ladaemon.com.cn
BuildRoot:	%{buildroot}

#BuildRequires:	gcc gcc-c++
AutoReq:        no

%description
This is test package for demo purpose

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
/opt/ladaemon/sbin/ladaemon
/opt/ladaemon/scripts/*
/opt/ladaemon/syncdir/*
/opt/ladaemon/etc/*
/opt/ladaemon/log/*


%post
ln -sf /opt/ladaemon/etc/21-laworks.conf /etc/rsyslog.d/
cp /opt/ladaemon/etc/init.d/svcladaemon /etc/init.d/ladaemon
service ladaemon start

%postun
service ladaemon stop
rm -fr /etc/rsyslog.d/21-ladaemon.conf
rm -fr /opt/ladaemon
rm -fr /etc/init.d/ladaemon



%changelog
* Fri Jul 12 2013 Leo.C.Wu <leo@ladaemon.com> -0.1-1
- Init build

