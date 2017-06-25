.PHONY: all
.PHONY: clean
.PHONY: lalibs 

LIBS= -lpthread  -I ../lib
OBJS= ../lib/*.o

FLAGS= -g 

all: lalibs 
	g++ $(LIBS) $(FLAGS) $(OBJS) main.cpp -o app -Wformat-security

lalibs:
	cd ../lib; $(MAKE)

clean:
	rm -fr app core
	rm -fr ../lib/*.o
