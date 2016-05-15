#ifndef __RTREE_H
#define __RTREE_H

#include "RList.h"
#include "Data.h"

Region RinitRgnRect(Dimension iBottomLeft, Dimension iTopRight);
void RsetRect(RLstNd lstNd, RTreeNode tnInfo);

RTreeNode RinitExtNd(Data dataClstElem);
RTreeNode RinitIntNd(Dimension iBottomLeft, Dimension iTopRight);

Boolean RexpansionArea(Region rgnRect, RTreeNode tnInfo, Double ptrDMinExp, Region rgnNewRect);
double Rarea(Region rgnRect);

RLstNd RpickChild(RHdrNd ptrChildLst, RTreeNode tnInfo);
void RpickSeeds(RHdrNd ptrChildList, RLstNd *lstNdChildOne, RLstNd *lstNdChildTwo);
void RsplitNode(RLstNd ptrChild);

Boolean RinsertTree(RHdrNd hdrNdTree, RTreeNode tnInfo);
RHdrNd RbuildRTree(DataHdr dataHdrLst);

RHdrNd RbuildRTreeFromCells(BCellListHd cellsList, Region epsExtendedRgn);

RHdrNd RcreateRoot(RHdrNd hdrNdTree);

void RprintTree(RHdrNd hdrNdTree);

unsigned int RgetNeighborHood(RHdrNd hdrNdTree, Data dataNdTemp,int size);
double RfindDist(DataPoint iDataOne, DataPoint iDataTwo);
unsigned int RfindRecords(RHdrNd hdrNdTree, Region rgnRect, Data dataNdTemp);

Boolean RisContains(Region rgnRect, DataPoint iData);
Boolean RisOverLap(Region rgnRectOne, Region rgnRectTwo);

void RappendRTree(RHdrNd hdrNdTree, DataHdr dataHdrLst);

Data getOneNN(RHdrNd hdrNdTree, Data DataPoint);

void freeRTree(RHdrNd auxRTree);

#endif
