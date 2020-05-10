CC = gcc
CFLAGS  = -Wall -Wpedantic -g -std=c99 
LIBS = -lm

default: tyCADv1

tyCADv1:  tyCADv1.o tydxfout.o tyMems1.o SIR.o
	$(CC) $(CFLAGS) $(LIBS) -o tyCADv1 tyCADv1.o tydxfout.o tyMems1.o SIR.o

tyCADv1.o:  tyCADv1.c
	$(CC) $(CFLAGS) $(LIBS) -c tyCADv1.c

tydxfout.o:  tydxfout.c tydxfout.h
	$(CC) $(CFLAGS) $(LIBS) -c tydxfout.c tydxfout.h

tyMems1.o:  tydxfout.c tydxfout.h
	$(CC) $(CFLAGS) $(LIBS) -c tyMems1.c tyMems1.h

SIR.o:  SIR.c SIR.h
	$(CC) $(CFLAGS) $(LIBS) -c SIR.c SIR.h


clean: 
	$(RM) *.o *.gch output/*.dxf input/mems1e.csv *~ tyCADv1

	
