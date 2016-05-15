#ifndef __GRID_H
#define __GRID_H

#include "GridRTree.h"

GHdrNd constructMainGRTree(DataHdr dataList1, BCellListHd cellsList, int gMinEntries, int gMaxEntries);
void constructAuxRTrees(DataHdr dataList1, BCellListHd cellsList);
void printAuxRTrees(BCellListHd cellsList);
void verifyAuxRTrees(BCellListHd cellsList);
void freeAuxRTrees(BCellListHd cellsList);
#endif
