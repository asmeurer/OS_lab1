all:
	gcc -c queuemanager.c -o queuemanager.o
	gcc queuemanager_test.c -o queuemanager_test.o
	gcc testgenerator.c -o testgenerator.o
clean:
	-rm -f *.o
