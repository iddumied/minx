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
VPU_SRC=${SRC}/vpu.c
SIMPLE_READER_SRC=${SRC}/simple_reader.c
BINARY_READER_SRC=${SRC}/binary_reader.c
CONFIG_SRC=${SRC}/config.c

MAIN_OUT=${BIN}/main.o
VPU_OUT=${BIN}/vpu.o
SIMPLE_READER_OUT=${BIN}/simple_reader.o
BINARY_READER_OUT=${BIN}/binary_reader.o
CONFIG_OUT=${BIN}/config.o

BINARY=${BIN}/minx

HEADERS= -I${SRC} -I${UTILC_HEADERS_LOCATIONS}

#
#
# modifying vpu 
#
#
MINX_FLAGS = -D DEBUG -D DEBUGGING -D VERBOSITY

#
#
# modifying c flags
#
#
CFLAGS += -Wall 
CFLAGS += -std=c99
CFLAGS += -g
CFLAGS += -c

#
#
# Compiling the VPU
#
# 
minx: utilc compile_config compile_main compile_vpu compile_binary_reader
	echo "minx:"
	echo "WARNING: This is maybe not stable, use 'make simple_vpu' instead!"
	echo "linking..."
	${CC}\
		${CONFIG_OUT}\
		${MAIN_OUT}\
		${VPU_OUT}\
		${BINARY_READER_OUT}\
		${BIN}/${UTILC_STACK_OUT} -o ${BINARY}
	echo "ready!"

simple_vpu: utilc compile_config compile_main compile_vpu compile_simple_reader
	echo "simple_vpu:"
	echo "linking..."
	${CC}\
		${CONFIG_OUT}\
		${MAIN_OUT}\
		${VPU_OUT}\
		${SIMPLE_READER_OUT}\
		${BIN}/${UTILC_STACK_OUT} -o ${BINARY}
	echo "ready!"

compile_main:
	echo "compile_main:"
	${CC} ${CFLAGS} ${MINX_FLAGS} ${HEADERS} ${MAIN_SRC} -o ${MAIN_OUT}

compile_vpu:
	echo "compile_vpu:"
	${CC} ${CFLAGS} ${MINX_FLAGS} ${HEADERS} ${VPU_SRC} -o ${VPU_OUT}

compile_simple_reader:
	echo "compile_simple_reader:"
	${CC} ${CFLAGS} ${MINX_FLAGS} ${HEADERS} ${SIMPLE_READER_SRC} -o ${SIMPLE_READER_OUT}

compile_config:
	echo "compile_config:"
	${CC} ${CFLAGS} ${MINX_FLAGS} ${HEADERS} ${CONFIG_SRC} -o ${CONFIG_OUT}

compile_binary_reader:
	echo "compile_binary_reader:"
	${CC} ${CFLAGS} ${MINX_FLAGS} ${HEADERS} ${BINARY_READER_SRC} -o ${BINARY_READER_OUT}

clean:
	rm -v ${BIN}/*.o
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
