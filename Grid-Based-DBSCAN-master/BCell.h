#ifndef __BCELL_H
#define __BCELL_H

#include "Def.h"

CellDataHd initCellDataHd();
BCell initBCell(Region rectTemp);
CellData initCellData(Data dataClstElem);
BCellListHd initBCellListHd();
void freeCellsList(BCellListHd cellsListNbh);
void freeAllBCells(BCellListHd cellsList);
void freeBCell(BCell bCellElem);
void freeCellDataList(CellDataHd cellDataList);

#endif
