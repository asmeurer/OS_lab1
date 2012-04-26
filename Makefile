.DEFAULT_GOAL := all
.PHONY := clean check-syntax all

SOURCES:=$(shell find . -name "*.c" | sed 's/\.\///g')
OUTPUT:=$(shell find . -name "*.o" | sed 's/\.\///g')

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

shared/textcolor.o: shared/textcolor.c shared/textcolor.h
	$(CC) $(FLAGS) -c shared/textcolor.c -o shared/textcolor.o

shared/binary.o: shared/binary.c
	$(CC) $(FLAGS) -c shared/binary.c -o shared/binary.o

ipc/ipc_queuemanager.o: ipc/ipc_queuemanager.c ipc/ipc_queuemanager.h process_manager/definitions.h
	$(CC) $(FLAGS) -c ipc/ipc_queuemanager.c -o ipc/ipc_queuemanager.o

ipc/ipc.o: ipc/ipc.c ipc/ipc_queuemanager.h ipc/ipc.h process_manager/definitions.h
	$(CC) $(FLAGS) -c ipc/ipc.c -o ipc/ipc.o

ipc/ipc_test.o: ipc/ipc_test.c ipc/ipc_test.h shared/textcolor.c ipc/ipc_definitions.h ipc/ipc_queuemanager.h ipc/ipc.c ipc/ipc.h
	$(CC) $(FLAGS) ipc/ipc_test.c ipc/ipc.o ipc/ipc_queuemanager.o -o ipc/ipc_test.o

ipc/ipc_testgenerator.o: ipc/ipc_testgenerator.c ipc/ipc_definitions.h
	$(CC) $(FLAGS) ipc/ipc_testgenerator.c -o ipc/ipc_testgenerator.o

memory/memory_manager.o: memory/memory_manager.c memory/memory_manager.h \
  memory/memory_definitions.h
	$(CC) $(FLAGS) -c memory/memory_manager.c -o memory/memory_manager.o

memory/memory_hardware.o: memory/memory_hardware.c memory/memory_hardware.h
	$(CC) $(FLAGS) -c memory/memory_hardware.c -o memory/memory_hardware.o

memory/memory_test.o: memory/memory_test.c memory/memory_test.h \
  memory/memory_definitions.h memory/../shared/textcolor.h \
  memory/list_memory.h memory/memory_manager.h memory/memory_hardware.h \
  memory/memory_manager.c shared/textcolor.c shared/textcolor.o \
  memory/list_memory.o
	$(CC) $(FLAGS) memory/memory_test.c memory/memory_manager.o memory/list_memory.o memory/memory_hardware.o shared/textcolor.o -o memory/memory_test.o

memory/list_memory.o: memory/list_memory.c memory/memory_definitions.h \
  memory/list_memory.h memory/../shared/textcolor.h
	$(CC) $(FLAGS) -c memory/list_memory.c -o memory/list_memory.o

memory/test_gen.o: memory/test_gen.c
	$(CC) $(FLAGS) memory/test_gen.c -o memory/test_gen.o

filesystem/file_queuemanager.o: filesystem/file_queuemanager.c \
  filesystem/file_queuemanager.h filesystem/definitions.h \
  filesystem/file_hardware.h
	$(CC) $(FLAGS) -c filesystem/file_queuemanager.c -o filesystem/file_queuemanager.o

filesystem/file_hardware.o: filesystem/file_hardware.c filesystem/file_hardware.h \
  filesystem/definitions.h filesystem/file_queuemanager.h filesystem/file_queuemanager.o
	$(CC) $(FLAGS) -c filesystem/file_hardware.c -o filesystem/file_hardware.o

filesystem/file_manager.o: filesystem/file_manager.c filesystem/file_manager.h \
  filesystem/definitions.h filesystem/file_hardware.h \
  filesystem/file_queuemanager.h
	$(CC) $(FLAGS) -c filesystem/file_manager.c -o filesystem/file_manager.o

filesystem/file_test.o: filesystem/file_test.c filesystem/file_test.h \
  filesystem/definitions.h filesystem/../shared/textcolor.h shared/textcolor.o
	$(CC) $(FLAGS) filesystem/file_test.c shared/textcolor.o filesystem/file_manager.o filesystem/file_hardware.o filesystem/file_queuemanager.o -o filesystem/file_test.o

clean:
	-rm -f $(OUTPUT) nul

check-syntax:
	$(CC) -o nul $(FLAGS) -S $(CHK_SOURCES)
