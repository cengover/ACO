CFLAGS = -O3 -fopenmp -Wall -I${HOME}/adevs-code/include 
CXX = g++
LIBS = -L${HOME}/adevs-code/src -ladevs

.SUFFIXES: .cpp
.cpp.o:
	${CXX} ${CFLAGS} -o $@ -c $<

all:Bene.o \
BeneNetwork.o \
main.o \
Provider.o \
Payer.o

abm: all main.o
	${CXX} ${CFLAGS} main.o Bene.o BeneNetwork.o Provider.o Payer.o ${LIBS} -o abm

clean:
	rm -f *.o abm
	
