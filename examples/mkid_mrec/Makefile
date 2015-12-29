PREFIX = /usr/local
CFLAGS = `allegro-config --cflags` 
LIBS = `allegro-config --libs` -leg

CC = gcc
RM = rm -f
CP = cp

all:  mkid.o mrec.o mkid

mkid.o : mkid.c
	$(CC) -c mkid.c $(CFLAGS) -o mkid.o

mrec.o : mrec.c
	$(CC) -c mrec.c $(CFLAGS) -o mrec.o

mkid :mkid.o mrec.o
	$(CC) mkid.o mrec.o -o mkid $(LIBS)

clean :
	$(RM) mkid.o mrec.o
	$(RM) mkid

install :
	$(CP) mkid $(PREFIX)/bin

uninstall :
	$(RM) $(PREFIX)/bin/mkid
