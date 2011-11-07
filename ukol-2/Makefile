program = generovani

HEAD := $(wildcard *.h)
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))

# implicit switches for gcc
CFLAGS = -O2 -pipe -Wall -Wextra

# switches for linker
LDFLAGS = -lm -lrt

# compiler
CC = gcc
#CC = g++

# clean does not have a file on its own
.PHONY: clean

# also a default target
${program}: ${OBJ}
	$(CC) $(LDFLAGS) ${OBJ} -o ${program}

# cleans files form compilation
clean:
	rm -f ${OBJ} ${program}

# every object file depends on all header files
${OBJ}: ${HEAD}

# rest is done automagically by implicit rules
