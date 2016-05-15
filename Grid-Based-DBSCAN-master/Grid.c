#include"Grid.h"

GHdrNd constructMainGRTree(DataHdr dataList1, BCellListHd cellsList, int gMinEntries, int gMaxEntries)
{
	GHdrNd GridRTree1 = (GHdrNd) populateGridRTree(dataList1,cellsList,gMinEntries,gMaxEntries);
	return GridRTree1;
}

void constructAuxRTrees(DataHdr dataList1, BCellListHd cellsList)
{
	
	int cnt = 0,i;
	int j=0;
	Boolean cellFlag;
    //retrieve element one by one and insert them into tree invoking create root incase of root split
	
	BCellListNode currCellNode = cellsList->first;

	for(i=0;i<cellsList->count;i++)
	{
		BCell currCell = currCellNode->bCellElem;			
		CellDataHd currCellDataList = currCell->cellDataList;
		CellData currCellData = currCellDataList->first;

		RHdrNd hdrNdTree = RinitHdrNd();
		hdrNdTree->ptrFirstNd = RinitLstNd(RinitIntNd(NULL, NULL));
		hdrNdTree->uiCnt++;
		hdrNdTree->ptrFirstNd->ptrChildLst = RinitHdrNd();
		
		while(currCellData!=NULL)
		{
			/*printf("inserting %d %lf %lf\n",currCellData->data->iNum, currCellData->data->iData[0], currCellData->data->iData[1] );
			if(currCellData->data->iNum==58 || currCellData->data->iNum==62 || currCellData->data->iNum==63)
				getchar(); */
			
			RinsertTree(hdrNdTree, RinitExtNd(currCellData->data));
			//printf("root count = %d\n",hdrNdTree->uiCnt );
			// if(currCellData->data->iNum==58 || currCellData->data->iNum==62 || currCellData->data->iNum==63)
			// {
			// 	getchar();
			// 	RprintTree(hdrNdTree);
			// }
				
			if(hdrNdTree->uiCnt > 1)
				hdrNdTree = RcreateRoot(hdrNdTree);

			// if(currCellData->data->iNum==58 || currCellData->data->iNum==62 || currCellData->data->iNum==63)
			// {
			// 	getchar();
			// 	RprintTree(hdrNdTree);
			// }

			currCellData=currCellData->next;			

		}

		currCell->auxCellRTree = hdrNdTree;

		currCellNode = currCellNode->next;
		
	}

	return;
}

void printAuxRTrees(BCellListHd cellsList)
{
	BCellListNode currCellNode=cellsList->first;
	BCell currBCell;
	int i =1;

	while(currCellNode!=NULL)
	{
		printf("\nPrinting %dth RTree\n",i++);

		currBCell = currCellNode->bCellElem;

		RprintTree(currBCell->auxCellRTree);

		currCellNode = currCellNode->next;
	}

	return;
}

void verifyAuxRTrees(BCellListHd cellsList)
{
	BCellListNode currCellNode=cellsList->first;
	BCell currBCell;
	int i =1;

	while(currCellNode!=NULL)
	{
		printf("\nVerifying %dth RTree\n",i++);

		currBCell = currCellNode->bCellElem;

		isCorrectRTree(currBCell->auxCellRTree);
		
		currCellNode = currCellNode->next;
	}

	return;
}

void freeAuxRTrees(BCellListHd cellsList)
{
	int cnt = 0,i;
	int j=0;
	Boolean cellFlag; int rmin, rmax;
	//retrieve element one by one and insert them into tree invoking create root incase of root split
	BCellListNode currCellNode = cellsList->first;
	BCell currCell;

	for(i=0;i<cellsList->count;i++)
//	while(currCellNode!=NULL)
	{
		currCell = currCellNode->bCellElem;

		if(currCell->auxCellRTree !=NULL)
		{
			freeRTree(currCell->auxCellRTree);
		}	

		currCellNode = currCellNode->next;
	}

	return;
}
