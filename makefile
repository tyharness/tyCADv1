CC = gcc
CFLAGS  = -Wall -Wpedantic -g -std=c99 
LIBS = -lm

default: tyCADv1

tyCADv1:  tyCADv1.o tydxfout.o tyMath.o tyMems1.o tyZ88.o SIR.o
	$(CC) $(CFLAGS)  -o tyCADv1 tyCADv1.o tydxfout.o tyMath.o tyMems1.o tyZ88.o SIR.o $(LIBS)

tyCADv1.o:  tyCADv1.c tyCADv1.h
	$(CC) $(CFLAGS) -c tyCADv1.c tyCADv1.h $(LIBS)

tydxfout.o:  tydxfout.c tydxfout.h
	$(CC) $(CFLAGS)  -c tydxfout.c tydxfout.h $(LIBS)
	
tyMath.o:  tyMath.c tyMath.h
	$(CC) $(CFLAGS)  -c tyMath.c tyMath.h $(LIBS)	

tyMems1.o:  tydxfout.c tydxfout.h
	$(CC) $(CFLAGS)  -c tyMems1.c tyMems1.h $(LIBS)
	
tyZ88.o:  tyZ88.c tyZ88.h
	$(CC) $(CFLAGS)  -c tyZ88.c tyZ88.h $(LIBS)

SIR.o:  SIR.c SIR.h
	$(CC) $(CFLAGS)  -c SIR.c SIR.h $(LIBS)


cleanz88:
	$(RM) output/z88example/*

clean: 
	$(RM) *.o *.gch output/*.dxf output/debug.txt output/z88example/* input/mems1e.csv *~ tyCADv1

	
