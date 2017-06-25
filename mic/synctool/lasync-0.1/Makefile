.PHONY: all
.PHONY: lasync
.PHONY: clean
.PHONY: install

all: lasync

CXXFLAGS = -Wno-write-strings -Wno-format-security

ifdef DIST
    CXXFLAGS += -D __REDHAT__
endif  

RPMNAME=lasync-0.1-1.x86_64

rpm:
	mkdir -p SOURCES SPECS RPMS SRPMS BUILD BUILDROOT/$(RPMNAME)/
	$(MAKE) DIST=RHEL
	$(MAKE) install prefix=BUILDROOT/$(RPMNAME)/
	cp sync.spec SPECS -fr
	rpmbuild -bb SPECS/sync.spec --define "_topdir `pwd`" 
	find . -name *.rpm -exec mv {} . \;

deb:
	-echo | dh_make -s -c bsd --createorig -e naughtyboywx@yeah.net
	dpkg-buildpackage -uc -us
	rm -fr debian/lasync/
	rm -f debian/lasync.debhelper.log  debian/lasync.substvars 
	rm -f debian/files
	rm -f ../*.tar.gz ../*.dsc ../*.changes

lasync: main.o lib/cmdutils.o lib/netutils.o lib/configparser.o lib/fsutils.o lib/strutils.o lib/exception.o lib/nfsutils.o
	g++ -g -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	g++ -c -g -o $@ $< $(CXXFLAGS)

prefix = /
DEST = $(prefix)/opt/lasync
install: all
	mkdir -p $(DEST)/sbin; 
	mkdir -p $(prefix)/usr/sbin;
	cp -r lasync $(DEST)/sbin; 
	cp -r lasync.wrapper $(prefix)/usr/sbin/lasync;
	cp -r etc syncdir $(DEST)

uninstall: clean
	rm -fr /opt/laworks;

clean:
	rm -f *.o lib/*.o ; rm -fr lasync
	rm -f ../*.tar.gz ../*.dsc ../*.deb ../*.changes
	rm -fr debian/lasync/
	rm -f debian/lasync.debhelper.log  debian/lasync.substvars 
	rm -f debian/files
	rm -fr SOURCES SPECS RPMS SRPMS BUILD BUILDROOT
	rm -fr *.rpm
