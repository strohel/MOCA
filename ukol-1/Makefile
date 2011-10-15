program=integraly

HEAD=integraly.h random.h funkce.h
SRC=main.c integraly.c random.c funkce.c
OBJ=main.o integraly.o random.o funkce.o

# implicit switches for gcc
CFLAGS=-O2 -pipe -Wall -Wextra -ggdb

# switches for linker
LDFLAGS=-lm

# compiler
CC=gcc
#CC=g++

# build, install, uninstall, clean a distrib do not have a file on their own
.PHONY: build
.PHONY: clean

# default target
build: ${program}

# cleans files form compilation
clean:
	rm -f ${OBJ} ${program}

${program}: ${OBJ}
	$(CC) $(LDFLAGS) ${OBJ} -o ${program}

# every object file depends on all header files
${OBJ}: ${HEAD}

# rest is done automagically by implicit rules
