# Makefile -- GNU Makefile for making alll sub directories of trk 
# 
# $Id: Makefile,v 1.3 2003/02/28 21:19:42 fpatrick Exp $
#

include ./MakeVars

all:  obj libs prg 

obj:
	@echo **************Building .o files***************************
	$(IDIRCMD) $(TWSL_DIR)/lib/objs
	$(MAKE) -C ./cpp obj || exit 1;

libs: obj
	$(MAKE) -C ./cpp lib || exit 1;

prg: libs
	@echo **************Building executable programs*****************
	$(IDIRCMD) $(TWSL_DIR)/bin
	$(MAKE) -C ./cpp prg || exit 1;

clean: 
	$(MAKE) -C ./cpp clean || exit 1;
	$(MAKE) -C ./lib clean || exit 1;


setup:
	$(IDIRCMD) $(TWSL_DIR)/bin
	echo $(ALL_LIB_DIRS)
	./admin/make_ld_library_path $(ALL_LIB_DIRS) > ./bin/setup_twsl.sh
