# This automatically generates dependencies.  See
# http://stackoverflow.com/questions/313778/generate-dependencies-for-a-makefile-for-a-project-in-c-c.

# Add .d to Make's recognized suffixes.
SUFFIXES += .d

.DEFAULT_GOAL = all

#Find all the C files in the lab directory
SOURCES:=$(shell find . -name "*.c")
CC = gcc
FLAGS = -Wall

#We don't need to clean up when we're making these targets
NODEPS:=clean check-syntax

#These are the dependency files, which make will clean up after it creates them
DEPFILES:=$(patsubst %.c,%.d,$(SOURCES))

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    #Chances are, these files don't exist.  GMake will create them and
    #clean up automatically afterwards
    -include $(DEPFILES)
endif

all: $(patsubst %.c,%.o,$(SOURCES))

#This is the rule for creating the dependency files
%.d: %.c
	$(CC) $(FLAGS) -MM -MT $(patsubst %.c,%.o,$<) $< > $@
#    $(CC) $(FLAGS) -MM -MT '$(patsubst src/%,obj/%,$(patsubst %.cpp,%.o,$<))' $< > $@


#This rule does the compilation
%.o: %.c %.d %.h
	$(CC) $(FLAGS) -o $@ -c $<


# all: queuemanager.o queuemanager_testgenerator.o processmanager.o queuemanager_test.o processmanager_test.o
#
# queuemanager.o: queuemanager.c queuemanager.h
#	gcc -Wall -c queuemanager.c -o queuemanager.o
#
# queuemanager_testgenerator.o:
#	gcc -Wall queuemanager_testgenerator.c -o queuemanager_testgenerator.o
#
# processmanager.o: processmanager.c processmanager.h queuemanager.o
#	gcc -Wall -c processmanager.c -o processmanager.o
#
# processmanager_testgenerator.o: processmanager_testgenerator.c
#	gcc -Wall processmanager_testgenerator.c -o processmanager_testgenerator.o
#
# queuemanager_test.o: processmanager.c processmanager.h queuemanager.c queuemanager.h definitions.h
#	gcc -Wall -c queuemanager_test.c processmanager.o queuemanager.o -o queuemanager_test.o
#
# processmanager_test.o:
#	gcc -Wall processmanager_test.c processmanager.o queuemanager.o -o processmanager_test.o

clean:
	-rm -f *.o *.d

check-syntax:
	$(CC) -o nul $(FLAGS) -S $(CHK_SOURCES)
