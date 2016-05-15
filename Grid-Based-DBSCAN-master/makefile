output: Data.o Grid.o GList.o RList.o BCell.o RTree.o GridRTree.o Test_GRTree.o GRTDBSCAN.o main.o
	gcc  -o output Grid.o Data.o GList.o RList.o BCell.o RTree.o GridRTree.o Test_GRTree.o GRTDBSCAN.o main.o -lm  

main.o: main.c
	gcc -c  -g main.c -lm

GRTDBSCAN.o: GRTDBSCAN.c
	gcc -c  -g GRTDBSCAN.c -lm

Grid.o: Grid.c
	gcc -c  -g Grid.c

Data.o: Data.c
	gcc -c  -g Data.c -lm
	
GridRTree.o: GridRTree.c
	gcc -c  -g GridRTree.c -lm

GList.o: GList.c
	gcc -c  -g GList.c -lm 

RList.o: RList.c
	gcc -c  -g RList.c -lm 

BCell.o: BCell.c
	gcc -c  -g BCell.c -lm 

RTree.o: RTree.c
	gcc -c  -g RTree.c -lm

Test_GRTree.o: Test_GRTree.c
	gcc -c  -g Test_GRTree.c -lm
	
clean:
	rm *.o
	rm output
