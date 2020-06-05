CC = gcc
CFLAGS  = -Wall -Wpedantic -g -std=c99 
LIBS = -lm

default: tyCADv1

tyCADv1:  tyCADv1.o tydxfout.o tyMems1.o SIR.o
	$(CC) $(CFLAGS)  -o tyCADv1 tyCADv1.o tydxfout.o tyMems1.o SIR.o $(LIBS)

tyCADv1.o:  tyCADv1.c
	$(CC) $(CFLAGS) -c tyCADv1.c $(LIBS)

tydxfout.o:  tydxfout.c tydxfout.h
	$(CC) $(CFLAGS)  -c tydxfout.c tydxfout.h $(LIBS)

tyMems1.o:  tydxfout.c tydxfout.h
	$(CC) $(CFLAGS)  -c tyMems1.c tyMems1.h $(LIBS)

SIR.o:  SIR.c SIR.h
	$(CC) $(CFLAGS)  -c SIR.c SIR.h $(LIBS)


clean: 
	$(RM) *.o *.gch output/*.dxf input/mems1e.csv *~ tyCADv1

	
