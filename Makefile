UTILC_DIR = ./utilc
UTILC_BIN_DIR = ${UTILC_DIR}/bin

UTILC_STACK_OUT = stack.o

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
