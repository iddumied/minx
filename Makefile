UTILC_DIR = ./utilc
UTILC_BIN_DIR = ${UTILC_DIR}/bin

UTILC_STACK_OUT = stack.o

UTILC_HEADERS_LOCATIONS=${UTILC_DIR}/src/

#
#
# Variables 
#
#
SRC=./src
BIN=./bin
CC=/usr/bin/gcc

MAIN_SRC=${SRC}/main.c
VM_SRC=${SRC}/vm.c
SIMPLE_READER_SRC=${SRC}/simple_reader.c

MAIN_OUT=${BIN}/main.o
VM_OUT=${BIN}/vm.o
SIMPLE_READER_OUT=${BIN}/simple_reader.o

BINARY=${BIN}/minx

HEADERS= -I${SRC} -I${UTILC_HEADERS_LOCATIONS}

#
#
# modifying vm 
#
#
MINX_FLAGS = -D DEBUG -D DEBUGGING

#
#
# modifying c flags
#
#
CFLAGS += -Wall 
CFLAGS += -std=c99
CFLAGS += -g
CFLAGS += -D DEBUG

#
#
# Compiling the VM
#
# 
simple_vm: compile_main compile_vm compile_simple_reader
	echo "simple_vm:"
	echo "linking..."
	${CC} ${MAIN_OUT} ${VM_OUT} ${SIMPLE_READER_OUT} ${BIN}/${UTILC_STACK_OUT} -o ${BINARY}
	echo "ready!"

compile_main:
	echo "compile_main:"
	${CC} -c ${CFLAGS} ${MINX_FLAGS} ${HEADERS} ${MAIN_SRC} -o ${MAIN_OUT}

compile_vm:
	echo "compile_vm:"
	${CC} -c ${CFLAGS} ${MINX_FLAGS} ${HEADERS} ${VM_SRC} -o ${VM_OUT}

compile_simple_reader:
	echo "compile_simple_reader:"
	${CC} -c ${CFLAGS} ${MINX_FLAGS} ${HEADERS} ${SIMPLE_READER_SRC} -o ${SIMPLE_READER_OUT}

compile_binary_reader:
	echo "compile_binary_reader:"
	echo "Not setup yet"
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
