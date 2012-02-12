all:
	gcc -Wall -c queuemanager.c -o queuemanager.o
	gcc -Wall queuemanager_test.c -o queuemanager_test.o
	gcc -Wall queuemanager_testgenerator.c -o queuemanager_testgenerator.o
	gcc -Wall -c processmanager.c -o processmanager.o
clean:
	-rm -f *.o

check-syntax:
	gcc -o nul -Wall -S $(CHK_SOURCES)
