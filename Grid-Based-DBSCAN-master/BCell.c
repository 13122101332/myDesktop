#include "BCell.h"

CellDataHd initCellDataHd(){
    CellDataHd newCellDataHd = (CellDataHd)malloc(sizeof(struct celldatahd));
    ASSERT(newCellDataHd);
    newCellDataHd->count=0;
    newCellDataHd->first=NULL;
    newCellDataHd->isProcessed=0;
    newCellDataHd->isDetermined=0;
    return newCellDataHd;
}

BCell initBCell(Region rectTemp)
{
    // initializes an empty BCell
    BCell newBCell = (BCell) malloc(sizeof(*newBCell));
    newBCell->minOriginalBoundary = (Dimension)malloc(DIMENSION*sizeof(double));
    newBCell->maxOriginalBoundary = (Dimension)malloc(DIMENSION*sizeof(double));
    newBCell->minActualBoundary = (Dimension)malloc(DIMENSION*sizeof(double));
    newBCell->maxActualBoundary = (Dimension)malloc(DIMENSION*sizeof(double));
    newBCell->id= BCELLID++;

    int i =0;
    for(i = 0; i < DIMENSION; i++)
    {
        newBCell->minOriginalBoundary[i] = rectTemp->iBottomLeft[i];
        newBCell->maxOriginalBoundary[i] = rectTemp->iTopRight[i];
    }

    newBCell->cellDataList = initCellDataHd();

    return newBCell;

}

CellData initCellData(Data dataClstElem)
{
    CellData currCellData = (CellData) malloc(sizeof(*currCellData));
    ASSERT(currCellData);
    currCellData->data = dataClstElem;
    currCellData->data->core_tag = 0;

    currCellData->next = NULL;
  //  currCellData->isProcessed=0;
    currCellData->nbhFlag=0;
   

    return currCellData;
}

BCellListHd initBCellListHd()
{
    BCellListHd cellsListHd = (BCellListHd) malloc(sizeof(*cellsListHd));
    ASSERT(cellsListHd);
    cellsListHd->first = NULL;
    cellsListHd->count=0;
    //printf("initialized succesfully %d\n", cellsListHd->count);
    return cellsListHd;
}

void freeCellsList(BCellListHd cellsListNbh)
{
    BCellListNode currListNode = cellsListNbh->first;
    BCellListNode nextListNode;
    
    while(currListNode!=NULL)
    {
        nextListNode = currListNode->next;
        free (currListNode);
        currListNode = nextListNode;
    }
    
    free(cellsListNbh);
    return;
}

void freeAllBCells(BCellListHd cellsList)
{

    BCellListNode currListNode = cellsList->first;
    BCellListNode nextListNode;
    BCell currBCell;
    
    while(currListNode!=NULL)
    {
        nextListNode = currListNode->next;
        freeBCell(currListNode->bCellElem);
        free(currListNode);
        currListNode = nextListNode;
    }

    free(cellsList);
    return;  

}

void freeBCell(BCell bCellElem)
{
    freeCellDataList(bCellElem->cellDataList);
    free(bCellElem->minOriginalBoundary);
    free(bCellElem->maxOriginalBoundary);
    free(bCellElem->minActualBoundary);
    free(bCellElem->maxActualBoundary);
    free(bCellElem);

    return;
}

void freeCellDataList(CellDataHd cellDataList)
{
    CellData currCellData = cellDataList->first;
    CellData nextCellData;
    while(currCellData!=NULL)
    {
        nextCellData=currCellData->next;
        free(currCellData);
        currCellData = nextCellData;
    }

    free(cellDataList);
    return;
}