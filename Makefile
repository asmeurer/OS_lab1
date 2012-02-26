all: queuemanager.o queuemanager_testgenerator.o processmanager.o queuemanager_test.o processmanager_test.o

queuemanager.o: queuemanager.c queuemanager.h
	gcc -Wall -c queuemanager.c -o queuemanager.o

queuemanager_testgenerator.o:
	gcc -Wall queuemanager_testgenerator.c -o queuemanager_testgenerator.o

processmanager.o: processmanager.c processmanager.h queuemanager.o
	gcc -Wall -c processmanager.c -o processmanager.o

processmanager_testgenerator.o: processmanager_testgenerator.c
	gcc -Wall processmanager_testgenerator.c -o processmanager_testgenerator.o

queuemanager_test.o: processmanager.c processmanager.h queuemanager.c queuemanager.h definitions.h
	gcc -Wall queuemanager_test.c  -o queuemanager_test.o

processmanager_test.o:
	gcc -Wall processmanager_test.c processmanager.o queuemanager.o -o processmanager_test.o

clean:
	-rm -f *.o

check-syntax:
	gcc -o nul -Wall -S $(CHK_SOURCES)
