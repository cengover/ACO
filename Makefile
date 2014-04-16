CFLAGS = -O3 -fopenmp -Wall -I${HOME}/adevs-code/include 
CXX = g++
LIBS = -L${HOME}/adevs-code/src -ladevs

.SUFFIXES: .cpp
.cpp.o:
	${CXX} ${CFLAGS} -o $@ -c $<

all:BeneNetwork.o \
Bene.o \
Provider.o \
Payer.o \
main.o \

abm: all
	${CXX} ${CFLAGS} main.o BeneNetwork.o Provider.o Bene.o Payer.o ${LIBS} -o abm

clean:
	rm -f *.o abm
	
