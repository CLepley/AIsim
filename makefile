a.out: person.o aStar.o main.o sim.o makefile
	g++ -g person.o aStar.o main.o sim.o

main.o: main.cpp makefile
	g++ -c -g main.cpp

aStar.o: aStar.cpp aStar.h makefile
	g++ -c -g aStar.cpp

sim.o: sim.cpp makefile
	g++ -c -g sim.cpp

person.o: person.cpp person.h makefile 
	g++ -c -g person.cpp

clean:
	rm -rf *.o
