# Makefile pour INF3105 / TP2

#OPTIONS = -Wall           # option standard
#OPTIONS = -g -O0 -Wall    # pour rouler dans gdb
OPTIONS = -O3 -Wall    # pour optimiser

all : tp2

tp2 : tp2.cpp tp2.h personne.h personne.o coordonnees.h coordonnees.o tableau.h
	g++ ${OPTIONS} -o tp2 tp2.cpp coordonnees.o personne.o
	
personne.o : personne.cpp personne.h
	g++ ${OPTIONS} -c -o personne.o personne.cpp

coordonnees.o : coordonnees.cpp coordonnees.h
	g++ ${OPTIONS} -c -o coordonnees.o coordonnees.cpp

clean:
	rm -rf tp2 *~ *.o traces**+.txt log*.txt
