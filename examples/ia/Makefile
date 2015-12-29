#--------------------------#
# Fichier MAKE pour IA     #
# Par Asher256
#--------------------------#

CFLAG = -O2 -Wall `allegro-config --cflags` 

OBJS = carte.o  \
       camera.o \
       iaengine.o \
       pos.o \
       ia.o

CC = g++

default: message compilation

carte.o:carte.cpp
	$(CC) $(CFLAG) -c carte.cpp -o carte.o 
camera.o:camera.cpp
	$(CC) $(CFLAG) -c camera.cpp -o camera.o 
ia.o:ia.cpp
	$(CC) $(CFLAG) -c ia.cpp -o ia.o 
iaengine.o:iaengine.cpp
	$(CC) $(CFLAG) -c iaengine.cpp -o iaengine.o 
pos.o:pos.cpp
	$(CC)  $(CFLAG) -c pos.cpp -o pos.o

message:
	 @echo ****************************************
	 @echo  COMPILATION DE IA ENGINE
	 @echo ****************************************

compilation: $(OBJS)
	 $(CC) -o ia $(OBJS) `allegro-config --libs` -l2dglib
	 
clean:
	rm -f *.o ia
