CXX ?= g++
CC ?= gcc
CFLAGS = -Wall -Wconversion -O3 -fPIC
LIBS = blas/blas.a
SHVER = 2
OS = $(shell uname)
#LIBS = -lblas

all: train predict trainParallel minmaxPredict trainSerial

lib: linear.o tron.o blas/blas.a
	if [ "$(OS)" = "Darwin" ]; then \
		SHARED_LIB_FLAG="-dynamiclib -Wl,-install_name,liblinear.so.$(SHVER)"; \
	else \
		SHARED_LIB_FLAG="-shared -Wl,-soname,liblinear.so.$(SHVER)"; \
	fi; \
	$(CXX) $${SHARED_LIB_FLAG} linear.o tron.o blas/blas.a -o liblinear.so.$(SHVER)

trainSerial:tron.o linear.o trainSerial.cpp blas/blas.a
	$(CXX) $(CFLAGS) -o trainSerial trainSerial.cpp tron.o linear.o $(LIBS)

trainParallel: trainParallel.c linear.o blas/blas.a
	mpicxx $(CFLAGS) -o trainParallel trainParallel.c tron.o linear.o $(LIBS)

minmaxPredict:minmaxPredict.cpp tron.o linear.o blas/blas.a
	$(CXX) $(CFLAGS) -o minmaxPredict minmaxPredict.cpp tron.o linear.o $(LIBS)

train: tron.o linear.o train.c blas/blas.a
	$(CXX) $(CFLAGS) -o train train.c tron.o linear.o $(LIBS)

predict: tron.o linear.o predict.c blas/blas.a
	$(CXX) $(CFLAGS) -o predict predict.c tron.o linear.o $(LIBS)

tron.o: tron.cpp tron.h
	$(CXX) $(CFLAGS) -c -o tron.o tron.cpp

linear.o: linear.cpp linear.h
	$(CXX) $(CFLAGS) -c -o linear.o linear.cpp

blas/blas.a: blas/*.c blas/*.h
	make -C blas OPTFLAGS='$(CFLAGS)' CC='$(CC)';

clean:
	make -C blas clean
	make -C matlab clean
	rm -f *~ tron.o linear.o train predict liblinear.so.$(SHVER)
