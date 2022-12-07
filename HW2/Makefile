cc = g++
prog = main
objects = main.o SkipList.o

$(prog): $(objects)
	$(cc) -o $(prog) $(objects)

main.o : Container.h SkipList.h
SkipList.o: SkipList.h