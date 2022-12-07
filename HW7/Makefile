cc = g++
prog = main
objects = main.o AVLTree.o RBTree.o

$(prog): $(objects)
	$(cc) -o $(prog) $(objects)

datacreater: datacreater.cpp

main.o : RBTree.h AVLTree.h
RBTree.o: RBTree.h
AVLTree.o: AVLTree.h