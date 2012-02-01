all:
	gcc -Wall -c queuemanager.c -o queuemanager.o
	gcc -Wall queuemanager_test.c -o queuemanager_test.o
	gcc -Wall testgenerator.c -o testgenerator.o
clean:
	-rm -f *.o

check-syntax:
	gcc -o nul -Wall -S $(CHK_SOURCES)
