cc = g++
prog = main
objects = main.o SkipList.o AVLTree.o

$(prog): $(objects)
	$(cc) -o $(prog) $(objects)

main.o : SkipList.h AVLTree.h
SkipList.o: SkipList.h
AVLTree.o: AVLTree.h