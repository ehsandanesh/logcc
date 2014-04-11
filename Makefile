#CC=arm-linux-gnueabihf-gcc-4.4
CC=gcc
MYSHAREDLIB=./
BIN=./
MYLIB=-llogging 
COMPILESTATUS=real
logbin: liblogging.so 
ifeq ($(COMPILESTATUS),testing)
	@echo "compiling this module for testing"
	$(CC) -Wall -DTESTING main.c -o $(BIN)/logbin -lz -lcrypt -lm -L$(MYSHAREDLIB) $(MYLIB) 
else
	$(CC) -Wall main.c -o $(BIN)/logbin -lz -lcrypt -lm -L$(MYSHAREDLIB) $(MYLIB)
endif




liblogging.so: logging.o
	$(CC) -Wall -shared -o $(MYSHAREDLIB)/liblogging.so logging.o 
	
logging.o:
ifeq ($(COMPILESTATUS),testing)
	@echo "compiling this module for testing"
	$(CC) -Wall -DTESTING -c -fpic logging.c
else
	$(CC) -Wall -c -fpic logging.c 
endif


install:
	mv $(MYSHAREDLIB)/liblogging.so /usr/local/lib/
	ldconfig



