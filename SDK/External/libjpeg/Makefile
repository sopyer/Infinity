ifeq ("$(shell uname)", "Darwin")
  OS="MacOSX"
endif

ifeq ("$(shell uname)", "Linux")
  OS="Unix"
endif

ifeq ("$(shell uname)", "FreeBSD")
  OS="Unix"
endif

ifndef OS
  $(error Unsupported operating system)
endif

MAIN=../../lib/libnvjpeg.a
CC=gcc
CFLAGS= -Wall -O3
INCLUDES= -I../../inc
SRCS = cdjpeg.c jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c jcphuff.c jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdphuff.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jmemansi.c jmemmgr.c jquant1.c jquant2.c jutils.c rdbmp.c rdcolmap.c rdgif.c rdppm.c rdswitch.c rdtarga.c transupp.c wrbmp.c wrgif.c wrppm.c wrtarga.c

ifeq ($(OS), "Unix")
  CFLAGS += -DUNIX
endif

ifeq ($(OS), "MacOSX")
  CFLAGS += -DMACOS
endif

OBJS = $(SRCS:.c=.o)

$(MAIN):  $(OBJS)
	$(AR) -rs $(MAIN) $(OBJS)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm -f *.o *~ $(MAIN)
