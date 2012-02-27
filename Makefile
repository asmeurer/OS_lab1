SOURCES:=$(shell find . -name "*.c" | cut -d/ -f 2)

all: $(patsubst %.c,%.o,$(SOURCES))

queuemanager.o: queuemanager.c queuemanager.h definitions.h
	gcc -Wall -c queuemanager.c -o queuemanager.o

queuemanager_testgenerator.o: queuemanager_testgenerator.c
	gcc -Wall queuemanager_testgenerator.c -o queuemanager_testgenerator.o

processmanager.o: processmanager.c processmanager.h definitions.h queuemanager.h
	gcc -Wall -c processmanager.c -o processmanager.o

processmanager_testgenerator.o: processmanager_testgenerator.c
	gcc -Wall processmanager_testgenerator.c -o processmanager_testgenerator.o

queuemanager_test.o: queuemanager_test.c processmanager.h definitions.h queuemanager.c queuemanager.h
	gcc -Wall queuemanager_test.c  -o queuemanager_test.o

processmanager_test.o: processmanager_test.c processmanager_test.h definitions.h processmanager.h queuemanager.h queuemanager.o processmanager.o
	gcc -Wall processmanager_test.c processmanager.o queuemanager.o -o processmanager_test.o

scheduler_testgenerator.o: scheduler_testgenerator.c definitions.h queuemanager.o processmanager.o
	gcc -Wall scheduler_testgenerator.c -o scheduler_testgenerator.o

clean:
	-rm -f *.o

check-syntax:
	gcc -o nul -Wall -S $(CHK_SOURCES)
