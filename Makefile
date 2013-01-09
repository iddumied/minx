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
# modifying
#
#
CFLAGS += -Wall 
CFLAGS += -std=c99

#
#
# Compiling the VM
#
# 
simple_vm: compile_main compile_vm compile_simple_reader
	echo "simple_vm:"
	${CC} ${MAIN_OUT} ${VM_OUT} ${SIMPLE_READER_OUT} ${BIN}/${UTILC_STACK_OUT} -o ${BINARY}

compile_main:
	echo "compile_main:"
	${CC} -c ${CFLAGS} ${HEADERS} ${MAIN_SRC} -o ${MAIN_OUT}

compile_vm:
	echo "compile_vm:"
	${CC} -c ${CFLAGS} ${HEADERS} ${VM_SRC} -o ${VM_OUT}

compile_simple_reader:
	echo "compile_simple_reader:"
	${CC} -c ${CFLAGS} ${HEADERS} ${SIMPLE_READER_SRC} -o ${SIMPLE_READER_OUT}

compile_binary_reader:
	echo "compile_binary_reader:"
	echo "Not setup yet"
#
#
# compiling utilc
#
#

utilc: compile_utilc_stack get_utilc_stack


compile_utilc_stack:
	cd ${UTILC_DIR}
	make stack
	cd -

get_utilc_stack:
	cp -v ${UTILC_BIN_DIR}/${UTILC_STACK_OUT} ${BIN}/${UTILC_STACK_OUT}
