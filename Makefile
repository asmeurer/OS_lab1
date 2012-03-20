SOURCES:=$(shell find . -name "*.c" | sed 's/\.\///g')

all: $(patsubst %.c,%.o,$(SOURCES))

process_manager/queuemanager.o: process_manager/queuemanager.c process_manager/queuemanager.h process_manager/definitions.h
	cc -Wall -c process_manager/queuemanager.c -o process_manager/queuemanager.o

process_manager/queuemanager_testgenerator.o: process_manager/queuemanager_testgenerator.c
	cc -Wall process_manager/queuemanager_testgenerator.c -o process_manager/queuemanager_testgenerator.o

process_manager/processmanager.o: process_manager/processmanager.c process_manager/processmanager.h process_manager/definitions.h process_manager/queuemanager.h
	cc -Wall -c process_manager/processmanager.c -o process_manager/processmanager.o

process_manager/processmanager_testgenerator.o: process_manager/processmanager_testgenerator.c
	cc -Wall process_manager/processmanager_testgenerator.c -o process_manager/processmanager_testgenerator.o

process_manager/queuemanager_test.o: process_manager/queuemanager_test.c process_manager/processmanager.h process_manager/definitions.h process_manager/queuemanager.c process_manager/queuemanager.h
	cc -Wall process_manager/queuemanager_test.c  -o process_manager/queuemanager_test.o

process_manager/processmanager_test.o: textcolor.c process_manager/processmanager_test.c process_manager/processmanager_test.h process_manager/definitions.h process_manager/processmanager.h process_manager/queuemanager.h process_manager/queuemanager.o process_manager/processmanager.o
	cc -Wall process_manager/processmanager_test.c process_manager/processmanager.o process_manager/queuemanager.o -o process_manager/processmanager_test.o

process_manager/scheduler_testgenerator.o: process_manager/scheduler_testgenerator.c process_manager/definitions.h process_manager/queuemanager.o process_manager/processmanager.o
	cc -Wall process_manager/scheduler_testgenerator.c -o process_manager/scheduler_testgenerator.o

textcolor.o: textcolor.c
	cc -Wall -c textcolor.c -o textcolor.o

ipc/ipc_queuemanager.o: ipc/ipc_queuemanager.c ipc/ipc_queuemanager.h process_manager/definitions.h
	cc -Wall -c ipc/ipc_queuemanager.c -o ipc/ipc_queuemanager.o

ipc/ipc.o: ipc/ipc.c ipc/ipc_queuemanager.h ipc/ipc.h process_manager/definitions.h
	cc -Wall -c ipc/ipc.c -o ipc/ipc.o

clean:
	-rm -f process_manager/*.o ipc/*o

check-syntax:
	cc -o nul -Wall -S $(CHK_SOURCES)
