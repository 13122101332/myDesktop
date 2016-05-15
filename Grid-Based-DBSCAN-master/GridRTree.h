#ifndef __GRIDRTREE_H
#define __GRIDRTREE_H

#include "GList.h"
#include "BCell.h"
#include "RTree.h"

Boolean GisOverLap(Region rgnRectOne, Region rgnRectTwo);
Boolean GisOverLap2(Region rgnRectOne, Region rgnRectTwo);
Region GinitRgnRect(Dimension iBottomLeft, Dimension iTopRight);
GTreeNode GinitIntNd(Dimension iBottomLeft, Dimension iTopRight);
GTreeNode GinitExtNd(BCell bcellElem);
void GsetRect(GLstNd lstNd, GTreeNode tnInfo);
GLstNd GpickChild(GHdrNd ptrChildLst, GTreeNode tnInfo);
Boolean GexpansionArea(Region rgnRect, GTreeNode tnInfo, Double ptrDMinExp, Region rgnNewRect);
double Garea(Region rgnRect);
Boolean GinsertTree(GHdrNd hdrNdTree, GTreeNode tnInfo, int gMinEntries, int gMaxEntries);
void GsplitNode(GLstNd ptrChild, int gminEntries);
void GpickSeeds(GHdrNd ptrChildLst, GLstNd *lstNdChildOne, GLstNd *lstNdChildTwo);
BCell GfindCell(GHdrNd hdrNdTree, Region rgnRect);
Boolean GisContains(Region rgnRect, DataPoint iData);
GHdrNd GcreateRoot(GHdrNd hdrNdTree);
void GprintTree(GHdrNd hdrNdTree);
void GprintRegion(Region region);
BCellListHd GgetCellsInRegion(GHdrNd hdrNdTree, Region regRect, Region cellRgnRect);
BCellListHd GgetCellsInRegion2(GHdrNd hdrNdTree, Region regRect, Region cellRgnRect);
unsigned int GfindOverlappingCells(GHdrNd hdrNdTree, Region rgnRect, Region cellRgnRect, BCellListHd cellsList);
unsigned int GfindOverlappingCells2(GHdrNd hdrNdTree, Region rgnRect, Region cellRgnRect, BCellListHd cellsList);
GHdrNd populateGridRTree(DataHdr dataHdrLst, BCellListHd cellsList, int gMinEntries, int gMaxEntries);
GHdrNd populateCellGridRTree(CellDataHd cellDataList, BCellListHd cellsList, BCell thisBCell, int gMinEntries, int gMaxEntries);
GHdrNd populateAuxGridRTree(BCellListHd cellsList, int gMinEntries, int gMaxEntries);
void insertPointintoBCell(BCell currBcell, Data currDataPoint);
void insertPointintoCellDataList(CellDataHd currCellDataList, Data currDataPoint);
Region createCellRegOfPoint(Data currDataPoint);
GHdrNd insertBCellIntoRTree(GHdrNd hdrNdTree, BCell BCellNode, int gMinEntries, int gMaxEntries);
void insertBCellIntoBCellList(BCell newBCell,BCellListHd cellsList);
void printCellsList(BCellListHd cellsList);
void printCell(BCell bCellElem);
void printCellDataList(CellDataHd currCellDataList);
void printCellData(CellData currCellData);
Region createRegionofCell(BCell bCellElem);
Region getEpsExtendedRegion(Region cellRgnRect, double eps);
//RHdrNd getAuxRTreeofNeighbors(BCell bCellElem, GHdrNd hdrNdTree);
void printNoOfCoreCells(BCellListHd cellsList);
Region getEpsExtendedRegionPoint(Data dataPoint, double eps);
void GgetNeighborHood(GHdrNd hdrNdTree, Data currDataPoint);
void appendNbh(Data currDataPoint, Data temp);
unsigned int GfindOverlappingCells2(GHdrNd hdrNdTree, Region rgnRect, Region cellRgnRect, BCellListHd cellsList);
unsigned int GfindOverlappingCells3(GHdrNd hdrNdTree, Region rgnRect, Region cellRgnRect, BCellListHd cellsList,BCellListHd cellsList2);


#endif