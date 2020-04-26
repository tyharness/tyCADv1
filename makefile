CC = gcc
CFLAGS  = -Wall -Wpedantic -g -std=c99 
LIBS = -lm

default: tyCADv1

tyCADv1:  tyCADv1.o tydxfout.o 
	$(CC) $(CFLAGS) $(LIBS) -o tyCADv1 tyCADv1.o tydxfout.o

tyCADv1.o:  tyCADv1.c
	$(CC) $(CFLAGS) $(LIBS) -c tyCADv1.c

tydxfout.o:  tydxfout.c tydxfout.h
	$(CC) $(CFLAGS) $(LIBS) -c tydxfout.c



clean: 
	$(RM) *.o *~ abc.dxf tyCADv1

	
