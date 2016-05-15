Grid-R-tree is a
two-level R-tree in which the first level R-tree is for indexing non-empty cells resulting from virtual
gridding of the search space. The second level comprises of multiple R-trees, one each for every cell
to index points lying in it. Bottleneck step in DBSCAN is the redundant neighborhood computations
for every point but we perform this computation only once for a particular cell.

Commands to run

For DBSCAN and GRDBSCAN:
sh serialProf.sh s1data5k 30000 30 makefile

For ONE-ONE MAPPING:
sh serialProf.sh s1data5k ../DBSCAN/1.txt ../GRDBSCAN/2.txt makefile

Note:Change Dataset path and values of m,M etc in serialProf.sh directly

GDB:
r ../Dataset/s1data5k 30000 30

VALGRIND:
valgrind --tool=memcheck --leak-check=yes ./output ../Dataset/s1data5k 30000 30 makefile