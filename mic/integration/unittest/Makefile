.PHONY: all
.PHONY: clean
.PHONY: lalibs 

LIBS = -lpthread  

OBJS = ../lib/*.o

CXXFLAGS = -g -Wformat-security -I ../lib/ -L ../lib/  $(OBJS)

UTAPPS :=  $(shell files=""; for file in `ls *.cpp *.c 2>/dev/null`; do app=`echo $$file |sed -r 's/(.*)\.cpp/\1/g'`; files="$$files $$app"; done ; echo $$files)

all:  
	$(MAKE) lalibs 
	$(MAKE) $(UTAPPS)

lalibs:
	cd ../lib; $(MAKE)
clean:
	rm -fr $(UTAPPS)
	cd ../lib; $(MAKE) clean
