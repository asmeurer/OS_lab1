SOURCES:=$(shell find . -name "*.c" | sed 's/\.\///g')

all: $(patsubst %.c,%.o,$(SOURCES))

Process_Manager/queuemanager.o: Process_Manager/queuemanager.c Process_Manager/queuemanager.h Process_Manager/definitions.h
	gcc -Wall -c Process_Manager/queuemanager.c -o Process_Manager/queuemanager.o

Process_Manager/queuemanager_testgenerator.o: Process_Manager/queuemanager_testgenerator.c
	gcc -Wall Process_Manager/queuemanager_testgenerator.c -o Process_Manager/queuemanager_testgenerator.o

Process_Manager/processmanager.o: Process_Manager/processmanager.c Process_Manager/processmanager.h Process_Manager/definitions.h Process_Manager/queuemanager.h
	gcc -Wall -c Process_Manager/processmanager.c -o Process_Manager/processmanager.o

Process_Manager/processmanager_testgenerator.o: Process_Manager/processmanager_testgenerator.c
	gcc -Wall Process_Manager/processmanager_testgenerator.c -o Process_Manager/processmanager_testgenerator.o

Process_Manager/queuemanager_test.o: Process_Manager/queuemanager_test.c Process_Manager/processmanager.h Process_Manager/definitions.h Process_Manager/queuemanager.c Process_Manager/queuemanager.h
	gcc -Wall Process_Manager/queuemanager_test.c  -o Process_Manager/queuemanager_test.o

Process_Manager/processmanager_test.o: textcolor.c Process_Manager/processmanager_test.c Process_Manager/processmanager_test.h Process_Manager/definitions.h Process_Manager/processmanager.h Process_Manager/queuemanager.h Process_Manager/queuemanager.o Process_Manager/processmanager.o
	gcc -Wall Process_Manager/processmanager_test.c Process_Manager/processmanager.o Process_Manager/queuemanager.o -o Process_Manager/processmanager_test.o

Process_Manager/scheduler_testgenerator.o: Process_Manager/scheduler_testgenerator.c Process_Manager/definitions.h Process_Manager/queuemanager.o Process_Manager/processmanager.o
	gcc -Wall Process_Manager/scheduler_testgenerator.c -o Process_Manager/scheduler_testgenerator.o

textcolor.o: textcolor.c
	gcc -Wall -c textcolor.c -o textcolor.o

IPC/ipc_queuemanager.o: IPC/ipc_queuemanager.c IPC/ipc_queuemanager.h Process_Manager/definitions.h
	gcc -Wall -c IPC/ipc_queuemanager.c -o IPC/ipc_queuemanager.o

IPC/ipc.o: IPC/ipc.c IPC/ipc_queuemanager.h IPC/ipc.h Process_Manager/definitions.h
	gcc -Wall -c IPC/ipc.c -o IPC/ipc.o

clean:
	-rm -f Process_Manager/*.o IPC/*o

check-syntax:
	gcc -o nul -Wall -S $(CHK_SOURCES)
