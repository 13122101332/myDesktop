#ifndef __GRTDBSCAN_H
#define __GRTDBSCAN_H

#include "Test_GRTree.h"
#include "Grid.h"

void printCheck(BCellListHd cellList,char * strFileName);
int find2(Cluster subsets, int i);
void Union2(Cluster subsets, int x, int y);
void mergeCellClusterIds(int cell1, int cell2);
void mergePointClusters(int point1, int point2,int cell1, int cell2);
void merge3(int point1, int point2,int cell2 );
void freeRegion(Region cellRgnRect);
void printcelltype(BCellListHd cellList);
void determineCellList(GHdrNd GRTree, BCellListHd cellList);
void processCellList(GHdrNd GRTree, BCellListHd cellList,DataHdr dataList);

#endif