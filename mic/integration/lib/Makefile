.PHONY: all

OBJS =  $(shell files=""; for file in `ls *.cpp *.c 2>/dev/null`; do obj=`echo $$file |sed -r 's/(.*)\.cpp/\1.o/g'`; files="$$files $$obj"; done ; echo $$files)
SRCS =  $(shell files=""; for file in `ls *.cpp *.c 2>/dev/null`; files="$$files $$file"; done ; echo $$files)

CXXFLAGS = -Wno-format-security -Wno-write-strings -g

all: $(OBJS)

%.o: %.cpp
	g++  -o $@ -c $< $(CXXFLAGS)

clean: 
	rm -fr $(OBJS)
