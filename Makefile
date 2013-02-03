UTILC_DIR = ./utilc
UTILC_BIN_DIR = ${UTILC_DIR}/bin

UTILC_STACK_OUT = ${UTILC_BIN_DIR}/stack.o

UTILC_HEADERS_LOCATIONS=${UTILC_DIR}/src/

#
#
# Variables 
#
#

CC=/usr/bin/gcc
LD=${CC}

SRC=./src
BIN=./bin

HEADERS= -I${SRC} -I${UTILC_HEADERS_LOCATIONS}

#
#
# modifying kernel.
#
#
MINX_FLAGS = -D DEBUG -D DEBUGGING -D VERBOSITY -D DISASSEMBLE

#
#
# modifying c flags
#
#
CFLAGS += -Wall 
CFLAGS += -std=c99
CFLAGS += -g

#
##
#  Specific variables
##
#
TARGET=minx
SOURCES= ${shell find ${SRC} -type f -name '*.c'}
OBJECTS=${foreach x, $(basename $(SOURCES)), $(x).o}

#
# 
# Lib compiling stuff
# 
#

LIB_SETUP_TASKS = setup_utilc
LIB_COMPILE_TASKS = compile_utilc_stack

ADDITIONAL_TASKS = ${LIB_SETUP_TASKS} ${LIB_COMPILE_TASKS}

#
#
# Link all objects
# 
#
LINK_OBJECTS = ${OBJECTS} ${UTILC_STACK_OUT}

#
##
# The tasks
##
#

${TARGET}: $(OBJECTS) $(ADDITIONAL_TASKS)
	${LD} ${LINK_OBJECTS} -o ${TARGET}

%.o: %.c 
	${CC} -c ${CFLAGS} ${HEADERS} ${MINX_FLAGS} $^ -o $@

compile: ${OBJECTS} ${ADDITIONAL_TASKS}

recompile: clean compile

link:
	${LD} ${LDFLAGS} ${LINK_OBJECTS} -o ${TARGET}

move:
	@mv ${TARGET} ${BIN}/${TARGET}
	
clean:
	@rm ${OBJECTS}

#
#
# compiling utilc
#
#

utilc: setup_utilc compile_utilc_stack clean_utilc

setup_utilc:
	mkdir -p ${UTILC_DIR}/bin

compile_utilc_stack:
	make -C ${UTILC_DIR} stack 

clean_utilc:
	rm -r ${UTILC_DIR}/bin
