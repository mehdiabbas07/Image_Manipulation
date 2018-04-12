# design names
DESIGN = PhotoLab PhotoLabTest

#libs
LIBS   = libFilter.a libFileIO.a

CC     = gcc
DEBUG  = -DDEBUG
CFLAGS = -ansi -std=c99 -Wall -c -g
LFLAGS = -ansi -std=c99 -Wall -g
AR     = ar rc
RANLIB = ranlib

IMAGES = bw.ppm \
         negative.ppm \
         colorfilter.ppm \
         edge.ppm \
         vflip.ppm \
         shuffle.ppm \
         vmirror.ppm \
         border.ppm \
         noise.ppm \
         sharpen.ppm \
         posterize.ppm \
	 crop.ppm \
	 brightnessandcontrast.ppm \
	 bigresize.ppm \
	 smallresize.ppm \
         blur.ppm

all: PhotoLab PhotoLabTest


########### generate object files ###########

#target to generate Image.o
Image.o: Image.h Image.c Constants.h
	$(CC) $(CFLAGS) Image.c -o Image.o

#target to generate FileIO.o
FileIO.o: FileIO.h FileIO.c Constants.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

#target to generate DIPs.o
DIPs.o: DIPs.h DIPs.c Constants.h
	$(CC) $(CFLAGS)  DIPs.c -o DIPs.o

#target to generate Advanced.o
Advanced.o: Advanced.h Advanced.c Constants.h
	$(CC) $(CFLAGS) Advanced.c -o Advanced.o

#target to generate Test.o
Test.o: Test.h Test.c Constants.h
	$(CC) $(CFLAGS) $(DEBUG) Test.c -o Test.o

#target to generate PhotoLab.o
PhotoLab.o: PhotoLab.c Image.h Test.h DIPs.h Advanced.h FileIO.h Constants.h
	$(CC) $(CFLAGS) PhotoLab.c -o PhotoLab.o

#target to generate FileIO_DEBUG.o
PhotoLab_DEBUG.o: PhotoLab.c Image.h Test.h DIPs.h Advanced.h FileIO.h Constants.h
	$(CC) $(CFLAGS) $(DEBUG) -c PhotoLab.c -o PhotoLab_DEBUG.o


########### generate library files ###########

#target to generate libFilter.a
libFilter.a: DIPs.o Advanced.o Test.o
	$(AR) libFilter.a Advanced.o DIPs.o Test.o
	$(RANLIB) libFilter.a

libFileIO.a: Image.o FileIO.o
	$(AR) libFileIO.a Image.o FileIO.o
	$(RANLIB) libFileIO.a


########### generate executables ###########

#target to generate PhotoLab
PhotoLab: PhotoLab.o FileIO.o libFilter.a libFileIO.a
	$(CC) $(LFLAGS) PhotoLab.o FileIO.o -L. -lFileIO -lFilter -o PhotoLab

#target to generate test
PhotoLabTest: PhotoLab_DEBUG.o FileIO.o libFilter.a libFileIO.a
	$(CC) $(lflags) -g -DDEBUG PhotoLab_DEBUG.o FileIO.o -L. -lFileIO -lFilter -o PhotoLabTest
	
#target to clean the directory
clean:
	rm -f *.o *.jpg $(DESIGN) $(IMAGES) $(LIBS)
