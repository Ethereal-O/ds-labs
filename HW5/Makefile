cc = g++
prog = main
objects = main.o AVLTree.o

$(prog): $(objects)
	$(cc) -o $(prog) $(objects)

main.o : SplayTree.h AVLTree.h
SplayTree.o: SplayTree.h
AVLTree.o: AVLTree.h