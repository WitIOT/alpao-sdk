#*******************************************************************************
# ALPAO - SDK Project
#*******************************************************************************

#
# <brief description of lllll library>
ex_OBJ_c   = example_legacy example_wrapper_DMM example_modal \
	example_wrapper
	
ex_OBJ_cpp = example example_multidm example_DMM

#
# user definable C-compilation flags
COPTS = -Wall -O3 -DASDK_EXPORTS

#
# additional include and library search paths
USER_INC = -I..

CC	= g++

USER_LIB32 = -L../../Lib/x86 -lasdk
USER_LIB64 = -L../../Lib/x64 -lasdk
	
LBITS := $(shell getconf LONG_BIT)
OUT_DIR32=Bin/x86
OUT_DIR64=Bin/x64

SRC_DIR=.

#
# TARGETS
# -------    
all:	do_all
	@echo " . . . 'all' done" 

clean : clean_all 
	@echo " . . . clean done"

mk_dir:
	@mkdir -p $(OUT_DIR32) $(OUT_DIR64)
	
do_all: mk_dir
# Build C examples
	@for file in $(ex_OBJ_c) ; do \
		echo "Compiling $${file}..."; \
		if [ $(LBITS) -eq 32 ] ; then $(CC) -m32 $(COPTS) -o $(OUT_DIR32)/$${file} $(SRC_DIR)/$${file}.c $(USER_INC) $(USER_LIB32); fi; \
		if [ $(LBITS) -eq 64 ] ; then $(CC) -m64 $(COPTS) -o $(OUT_DIR64)/$${file} $(SRC_DIR)/$${file}.c $(USER_INC) $(USER_LIB64); fi; \
	done
	
# Build C++ examples
	@for file in $(ex_OBJ_cpp) ; do \
		echo "Compiling $${file}..."; \
		if [ $(LBITS) -eq 32 ] ; then $(CC) -m32 $(COPTS) -o $(OUT_DIR32)/$${file} $(SRC_DIR)/$${file}.cpp $(USER_INC) $(USER_LIB32); fi; \
		if [ $(LBITS) -eq 64 ] ; then $(CC) -m64 $(COPTS) -o $(OUT_DIR64)/$${file} $(SRC_DIR)/$${file}.cpp $(USER_INC) $(USER_LIB64); fi; \
	done

clean_all:
	@-for file in $(ex_OBJ) ; do \
		echo "Removing obj $${file} ...";\
		rm -f $(OUT_DIR32)/$${file}; \
		rm -f $(OUT_DIR64)/$${file}; \
	done


