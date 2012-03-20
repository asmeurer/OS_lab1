SOURCES:=$(shell find . -name "*.c" | sed 's/\.\///g')

CC=cc
FLAGS=-Wall

all: $(patsubst %.c,%.o,$(SOURCES))

process_manager/queuemanager.o: process_manager/queuemanager.c process_manager/queuemanager.h process_manager/definitions.h
	$(CC) $(FLAGS) -c process_manager/queuemanager.c -o process_manager/queuemanager.o

process_manager/queuemanager_testgenerator.o: process_manager/queuemanager_testgenerator.c
	$(CC) $(FLAGS) process_manager/queuemanager_testgenerator.c -o process_manager/queuemanager_testgenerator.o

process_manager/processmanager.o: process_manager/processmanager.c process_manager/processmanager.h process_manager/definitions.h process_manager/queuemanager.h
	$(CC) $(FLAGS) -c process_manager/processmanager.c -o process_manager/processmanager.o

process_manager/processmanager_testgenerator.o: process_manager/processmanager_testgenerator.c
	$(CC) $(FLAGS) process_manager/processmanager_testgenerator.c -o process_manager/processmanager_testgenerator.o

process_manager/queuemanager_test.o: process_manager/queuemanager_test.c process_manager/processmanager.h process_manager/definitions.h process_manager/queuemanager.c process_manager/queuemanager.h
	$(CC) $(FLAGS) process_manager/queuemanager_test.c  -o process_manager/queuemanager_test.o

process_manager/processmanager_test.o: shared/textcolor.c process_manager/processmanager_test.c process_manager/processmanager_test.h process_manager/definitions.h process_manager/processmanager.h process_manager/queuemanager.h process_manager/queuemanager.o process_manager/processmanager.o
	$(CC) $(FLAGS) process_manager/processmanager_test.c process_manager/processmanager.o process_manager/queuemanager.o -o process_manager/processmanager_test.o

process_manager/scheduler_testgenerator.o: process_manager/scheduler_testgenerator.c process_manager/definitions.h process_manager/queuemanager.o process_manager/processmanager.o
	$(CC) $(FLAGS) process_manager/scheduler_testgenerator.c -o process_manager/scheduler_testgenerator.o

shared/textcolor.o: shared/textcolor.c
	$(CC) $(FLAGS) -c shared/textcolor.c -o shared/textcolor.o

ipc/ipc_queuemanager.o: ipc/ipc_queuemanager.c ipc/ipc_queuemanager.h process_manager/definitions.h
	$(CC) $(FLAGS) -c ipc/ipc_queuemanager.c -o ipc/ipc_queuemanager.o

ipc/ipc.o: ipc/ipc.c ipc/ipc_queuemanager.h ipc/ipc.h process_manager/definitions.h
	$(CC) $(FLAGS) -c ipc/ipc.c -o ipc/ipc.o

clean:
	-rm -f process_manager/*.o ipc/*o shared/*.o

check-syntax:
	$(CC) -o nul $(FLAGS) -S $(CHK_SOURCES)
