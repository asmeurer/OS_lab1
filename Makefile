all:
	gcc -c queuemanager.c -o queuemanager.o
	gcc queuemanager_test.c -o queuemanager_test.o
clean:
	-rm -f *.o
