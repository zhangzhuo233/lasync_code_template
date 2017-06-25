.PHONY: all

.PHONY: ladaemon

.PHONY: clean
LIBS = -lpthread
CXXFLAGS = -Wno-write-strings -Wno-format-security

ifdef DIST
    CXXFLAGS += -D __REDHAT__
endif  

all: ladaemon
RPMNAME=ladaemon-0.1-1.x86_64

rpm:
	mkdir -p SOURCES SPECS RPMS SRPMS BUILD BUILDROOT/$(RPMNAME)/opt
	$(MAKE) DIST=RHEL
	$(MAKE) install prefix=BUILDROOT/$(RPMNAME)/opt
	cp daemon.spec SPECS -fr
	rpmbuild -bb SPECS/daemon.spec --define "_topdir `pwd`" 
	find . -name *.rpm -exec mv {} . \;

deb:
	-echo | dh_make -s -c bsd --createorig -e naughtyboywx@yeah.net
	dpkg-buildpackage -uc -us
	rm -f ../*.tar.gz ../*.dsc ../*.changes
	rm -fr debian/ladaemon/
	rm -f debian/ladaemon.debhelper.log  debian/ladaemon.substvars 
	rm -f debian/files

ladaemon: main.o lib/netutils.o lib/logutils.o lib/fsutils.o lib/mscutils.o lib/command.o lib/lathread.o lib/nfsutils.o lib/strutils.o lib/scriptexecutor.o lib/configparser.o lib/exception.o lib/dirreader.o 
	g++ -o $@ $^ $(LIBS) -g $(CXXFLAGS)

ut: utreader utconfigparser utstrutils utfsutils utscriptexecutor utlogger utcommand utnfsutils utnetutils

utnetutils: lib/netutils.o lib/utnetutils.o
	g++ -o $@ $^ -g $(CXXFLAGS)


utcommand: lib/command.o lib/utcommand.o lib/fsutils.o lib/nfsutils.o lib/netutils.o lib/strutils.o lib/scriptexecutor.o lib/configparser.o lib/exception.o lib/dirreader.o lib/logutils.o
	g++ -o $@ $^ -g $(CXXFLAGS)


utnfsutils: lib/nfsutils.o lib/utnfsutils.o lib/fsutils.o
	g++ -o $@ $^ -g $(CXXFLAGS)


utscriptexecutor: lib/scriptexecutor.o lib/utscriptexecutor.o lib/exception.o lib/dirreader.o lib/configparser.o lib/fsutils.o lib/strutils.o
	g++ -o $@ $^ -g $(CXXFLAGS)


utstrutils: lib/strutils.o lib/utstrutils.o
	g++ -o $@ $^ -g $(CXXFLAGS)

utfsutils: lib/fsutils.o lib/utfsutils.o
	g++ -o $@ $^ -g $(CXXFLAGS)

utreader:lib/dirreader.o lib/utdirreader.o lib/exception.o 
	g++ -o $@ $^ -g $(CXXFLAGS)

utlogger:lib/logutils.o lib/utlog.o lib/fsutils.o
	g++ -o $@ $^ -g $(CXXFLAGS)

utconfigparser: lib/configparser.o lib/utconfigparser.o lib/exception.o lib/fsutils.o lib/strutils.o
	g++ -o $@ $^ -g $(CXXFLAGS)
	
%.o: %.cpp
	g++ -g -o $@ -c $< $(CXXFLAGS)


prefix = /opt
DEST = $(prefix)/ladaemon

install:all
	mkdir -p $(DEST)/sbin
	mkdir -p $(DEST)/etc/init.d
	cp -r ladaemon $(DEST)/sbin
	cp -r etc log syncdir scripts $(DEST)
	cp -r svcladaemon $(DEST)/etc/init.d/



clean:
	rm -f *.o lib/*.o ; rm -f ladaemon log.txt utreader ladaemon core utconfigparser utstrutils utfsutils utscriptexecutor utlogger utcommand utnfsutils utnetutils
	rm -f ../*.tar.gz ../*.dsc ../*.deb ../*.changes
	rm -fr debian/ladaemon/
	rm -f debian/ladaemon.debhelper.log  debian/ladaemon.substvars 
	rm -f debian/files
	rm -fr SOURCES SPECS RPMS SRPMS BUILD BUILDROOT
	rm -fr *.rpm

