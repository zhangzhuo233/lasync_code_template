.PHONY: rpms
.PHONY: debs
.PHONY: clean
.PHONY: all


all:
	@echo "target would be below:"
	@echo "    debs: debian/ubuntu install package"
	@echo "    rpms: redhat/suse install package"
	@echo "    clean: clean all targets and intermediate files"

clean:
	cd lasync*; make clean
	cd ladaemon*; make clean
	rm -fr *.rpm *.deb *.xz

debs: 
	cd lasync*; make deb 
	cd ladaemon*; make deb
	
rpms: 
	cd ladaemon*; make rpm
	cd lasync*; make rpm 
	find . -name *.rpm -exec mv {} . \;
