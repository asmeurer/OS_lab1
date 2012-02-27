SOURCES:=$(shell find . -name "*.c" | cut -d/ -f 2)

all: processmanager_test.o

queuemanager.o: queuemanager.c queuemanager.h definitions.h
	gcc -Wall -c queuemanager.c -o queuemanager.o

processmanager.o: processmanager.c processmanager.h definitions.h queuemanager.h
	gcc -Wall -c processmanager.c -o processmanager.o

processmanager_test.o: processmanager_test.c processmanager_test.h definitions.h processmanager.h queuemanager.h queuemanager.o processmanager.o
	gcc -Wall processmanager_test.c processmanager.o queuemanager.o -o processmanager_test.o

clean:
	-rm -f *.o

check-syntax:
	gcc -o nul -Wall -S $(CHK_SOURCES)
