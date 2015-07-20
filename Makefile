##
# Makefile
#
# Simple Makefile to build photomosaic program
# and tests
#
#  Authors:
#  Jack Toole
#  Matt Sachtler
#  Scott Wegner
#
#  Developed for CS225 PhotoMosaic MP
#  Fall 2008-2011
#
#  Updates:
#  	Spring 2009 - Matt Sachtler - removed KDElement
#   Spring 2011 - Jack Toole - tons of changes to MP 6.1
#   Fall 2011 - Jack Toole - MP 6.2 rewrite
#   Fall 2012 - Sean Massung - no_sort added
##
CXX = clang++
CXXFLAGS = -Iinclude -std=c++14 -stdlib=libc++ -g -O0 -c -Wall -Wextra
LDFLAGS = -std=c++14 -stdlib=libc++ -lc++abi -lpng

.PHONY: all clean tidy test check

ifdef SANITIZE
CXXFLAGS += -fsanitize=$(SANITIZE)
LDFLAGS += -fsanitize=$(SANITIZE)
endif

EXENAME=photomosaic
KDTEXENAME=testkdtree
KDTMEXENAME=testmaptiles

OBJS = photomosaic.o util.o mosaiccanvas.o sourceimage.o maptiles.o \
       rgba_pixel.o epng.o coloredout.o tileimage.o

KDTOBJS = testkdtree.o coloredout.o
KDTMOBJS = testmaptiles.o mosaiccanvas.o sourceimage.o maptiles.o rgba_pixel.o \
           epng.o coloredout.o tileimage.o

# -msse2 is to try to make floating point arithmetic as uniform as possible
# across different systems, so that output images can be diffed
#
# TODO: Is this still needed? -cag
CXXFLAGS += -msse2

PROVIDED_OPTS = -O2
STUDENT_OPTS = -O0

all : $(KDTEXENAME) $(KDTMEXENAME) $(EXENAME)
test : $(KDTMEXENAME) $(KDTMEXENAME)
check : $(KDTEXENAME) $(KDTMEXENAME)
	./$(KDTEXENAME)
	./$(KDTMEXENAME)

$(EXENAME) : $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(EXENAME)

$(KDTEXENAME) : $(KDTOBJS)
	$(CXX) $(KDTOBJS) $(LDFLAGS) -o $(KDTEXENAME)

$(KDTMEXENAME) : $(KDTMOBJS)
	$(CXX) $(KDTMOBJS) $(LDFLAGS) -o $(KDTMEXENAME)

photomosaic.o : include/util.h src/photomosaic.cpp include/epng.h \
                $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) $(PROVIDED_OPTS) src/photomosaic.cpp

util.o : include/util.h src/util.cpp
	$(CXX) $(CXXFLAGS) $(PROVIDED_OPTS) src/util.cpp

mosaiccanvas.o : include/mosaiccanvas.h src/mosaiccanvas.cpp include/epng.h
	$(CXX) $(CXXFLAGS) $(PROVIDED_OPTS) src/mosaiccanvas.cpp

sourceimage.o : include/sourceimage.h src/sourceimage.cpp include/epng.h
	$(CXX) $(CXXFLAGS) $(PROVIDED_OPTS) src/sourceimage.cpp

tileimage.o : include/tileimage.h src/tileimage.cpp include/epng.h
	$(CXX) $(CXXFLAGS) $(PROVIDED_OPTS) src/tileimage.cpp

maptiles.o : include/mosaiccanvas.h include/sourceimage.h \
             include/maptiles.h src/maptiles.cpp include/kdtree.h \
             include/kdtree.tcc include/kdtree_extras.tcc include/point.h \
             include/point.tcc include/epng.h
	$(CXX) $(CXXFLAGS) $(STUDENT_OPTS) src/maptiles.cpp

testmaptiles.o : src/testmaptiles.cpp include/maptiles.h include/epng.h
	$(CXX) $(CXXFLAGS) $(STUDENT_OPTS) src/testmaptiles.cpp

testkdtree.o : src/testkdtree.cpp include/kdtree.h include/kdtree.tcc \
               include/kdtree_extras.tcc include/point.h include/point.tcc
	$(CXX) $(CXXFLAGS) $(STUDENT_OPTS) src/testkdtree.cpp

epng.o: include/epng.h src/epng.cpp
	$(CXX) $(CXXFLAGS) $(PROVIDED_OPTS) src/epng.cpp

rgba_pixel.o: include/rgba_pixel.h src/rgba_pixel.cpp
	$(CXX) $(CXXFLAGS) $(PROVIDED_OPTS) src/rgba_pixel.cpp

coloredout.o: include/coloredout.h src/coloredout.cpp
	$(CXX) $(CXXFLAGS) $(STUDENT_OPTS) src/coloredout.cpp

doc: $(wildcard src/*) $(wildcard include/*) mp_mosaic.doxygen
	doxygen mp_mosaic.doxygen

clean:
	rm -f $(EXENAME) $(KDTEXENAME) $(KDTMEXENAME) *.o 2>/dev/null

tidy:
	rm -rf doc
