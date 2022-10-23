# Makefile -- GNU Makefile for making tst libraries - assumes that objectfiles have already been built
#                   libraries
#

include ../MakeVars


lib: libtwsl$(TWSL_VERSION).so

libtwsl$(TWSL_VERSION).so : $(wildcard ./objs/*.o)
	$(CXX) $(LIBFLAGS) -o $@ $(wildcard ./objs/*.o)

clean: 
	$(RM) libtwsl$(TWSL_VERSION).so
	$(RM) ./objs/*.o

