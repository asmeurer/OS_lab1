all: queuemanager.o processmanager.o processmanager_test.c processmanager_test.h
	gcc -Wall -O2 queuemanager_testgenerator.c -o queuemanager_testgenerator.o
	gcc -Wall -O2 processmanager_testgenerator.c -o processmanager_testgenerator.o
	gcc -Wall -O2 processmanager_test.c processmanager.o queuemanager.o -o processmanager_test.o

processmanager.o: processmanager.c processmanager.h queuemanager.o
	gcc -Wall -O2 -c processmanager.c -o processmanager.o

queuemanager.o: queuemanager.c queuemanager.h
	gcc -Wall -O2 -c queuemanager.c -o queuemanager.o

clean:
	-rm -f *.o

check-syntax:
	gcc -o nul -Wall -S $(CHK_SOURCES)
