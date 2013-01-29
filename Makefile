UTILC_DIR = ./utilc
UTILC_BIN_DIR = ${UTILC_DIR}/bin

UTILC_STACK_OUT = stack.o

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

${TARGET}: $(OBJECTS)
	${LD} ${OBJECTS} -o ${TARGET}

%.o: %.c 
	${CC} -c ${CFLAGS} ${HEADERS} ${MINX_FLAGS} $^ -o $@

link:
	${CC} ${BIN}/${OBJECTS} -o ${BINARY}
	

clean:
	@rm ${OBJECTS}

#
#
# compiling utilc
#
#

utilc: setup_utilc compile_utilc_stack get_utilc_stack clean_utilc

setup_utilc:
	mkdir -p ${UTILC_DIR}/bin

compile_utilc_stack:
	make -C ${UTILC_DIR} stack 

get_utilc_stack:
	cp -v ${UTILC_BIN_DIR}/${UTILC_STACK_OUT} ${BIN}/${UTILC_STACK_OUT}

clean_utilc:
	rm -r ${UTILC_DIR}/bin
