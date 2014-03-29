MAKE_DIRS= DEV_TOOLS \
           Libraries \
           Tests

# makefile tools library  for bulding multi-directory projects
#
# Luke Humphreys 2012
ALL_WARNINGS=YES

dummy: makefile.include all

makefile.include: 
	$(MAKE) -C DEV_TOOLS/Config/Makefiles/

include makefile.include
