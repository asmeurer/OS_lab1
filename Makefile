all: queuemanager.o processmanager.o processmanager_test.c processmanager_test.h
	gcc -Wall queuemanager_testgenerator.c -o queuemanager_testgenerator.o
	gcc -Wall processmanager_testgenerator.c -o processmanager_testgenerator.o
	gcc -Wall processmanager_test.c processmanager.o queuemanager.o -o processmanager_test.o

processmanager.o: processmanager.c processmanager.h queuemanager.o
	gcc -Wall -c processmanager.c -o processmanager.o

queuemanager.o: queuemanager.c queuemanager.h
	gcc -Wall -c queuemanager.c -o queuemanager.o

clean:
	-rm -f *.o

check-syntax:
	gcc -o nul -Wall -S $(CHK_SOURCES)
