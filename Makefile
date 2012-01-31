all:
	gcc -c queuemanager.c -o queuemanager.o
	gcc test.c -o test.o
clean:
	-rm -f *.o
