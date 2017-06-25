.PHONY: all
.PHONY: lib
.PHONY: unittest
.PHONY: pic
.PHONY: mic

.PHONY: mic

.PHONY: clean

LIBS = -lpthread
CXXFLAGS = -Wno-write-strings -Wno-format-security -g

ifdef DIST
    CXXFLAGS += -D __REDHAT__
endif  

all: lib unittest pic

lib:
	cd lib; $(MAKE)

unittest:
	cd unittest; $(MAKE)

pic: lib 
	cd pic; $(MAKE)

%.o: %.cpp
	g++  -o $@ -c $< $(CXXFLAGS)

clean:
	cd pic; $(MAKE) clean
	cd lib; $(MAKE) clean
	cd unittest; $(MAKE) clean
