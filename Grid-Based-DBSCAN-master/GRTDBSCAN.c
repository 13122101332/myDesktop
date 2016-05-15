#include "GRTDBSCAN.h"

void printCheck(BCellListHd cellList, char * strFileName) //for printing in file
{
	FILE *fp = fopen(strFileName, "w");
	int temp;
	BCellListNode currentCell = cellList->first;
	if (cellList->count != 0)
	{
		while (currentCell != NULL)
		{
			BCell currentBCell = currentCell->bCellElem;
			CellData currentNbhDataPoint = currentBCell->cellDataList->first;
			while (currentNbhDataPoint != NULL)
			{
				//if cell cluster is there
				if (cluster[currentBCell->id + totalPoints].parent != currentBCell->id + totalPoints || cluster[currentBCell->id + totalPoints].rank != 0)
				{
					temp = find2(cluster, currentBCell->id + totalPoints);
					fprintf(fp, "%d %d %d\n", currentNbhDataPoint->data->iNum, currentNbhDataPoint->data->core_tag, temp);
				}

				//if cell cluster not there but point cluster is there
				else if (cluster[currentNbhDataPoint->data->iNum].parent != currentNbhDataPoint->data->iNum || cluster[currentNbhDataPoint->data->iNum].rank != 0)
				{
					temp = find2(cluster, currentNbhDataPoint->data->iNum);
					fprintf(fp, "%d %d %d\n", currentNbhDataPoint->data->iNum, currentNbhDataPoint->data->core_tag, temp);

				}

				//else no cluster for this data point
				else
					fprintf(fp, "%d %d -1\n", currentNbhDataPoint->data->iNum, currentNbhDataPoint->data->core_tag);
				currentNbhDataPoint = currentNbhDataPoint->next;
			}
			currentCell = currentCell->next;
		}
	}
	fclose(fp);
}


int find2(Cluster subsets, int i) //Optimised Union-Find.
{
	// find root and make root as parent of i (path compression)
	if (subsets[i].parent != i)
		subsets[i].parent = find2(subsets, subsets[i].parent);
	return subsets[i].parent;
}


void Union2(Cluster subsets, int x, int y) //Optimised Union-Find.
{
	int xroot = find2(subsets, x);
	int yroot = find2(subsets, y);

	// Attach smaller rank tree under root of high rank tree
	// (Union by Rank)
	if (subsets[xroot].rank < subsets[yroot].rank)
		subsets[xroot].parent = yroot;
	else if (subsets[xroot].rank > subsets[yroot].rank)
		subsets[yroot].parent = xroot;

	// If ranks are same, then make one as root and increment
	// its rank by one
	else
	{
		subsets[yroot].parent = xroot;
		subsets[xroot].rank++;
	}
}



void mergeCellClusterIds(int cell1, int cell2) //merge two cells.
{
	Union2(cluster, cell1 + totalPoints, cell2 + totalPoints);
	return;
}

void mergePointClusters(int point1, int point2, int cell1, int cell2) //merge two points. Either cell cluster if it exists or point directly.
{
	if (cluster[cell1 + totalPoints].parent != cell1 || cluster[cell1 + totalPoints].rank != 0)
		point1 = cell1 + totalPoints;

	if (cluster[cell2 + totalPoints].parent != cell2 || cluster[cell2 + totalPoints].rank != 0)
		point2 = cell2 + totalPoints;

	Union2(cluster, point1, point2);
	return;
}


void merge3(int point1, int point2, int cell2) //merging a point and a point whose cell cluster or point cluster needs to be checked.
{
	if (cluster[cell2 + totalPoints].parent != cell2 || cluster[cell2 + totalPoints].rank != 0)
		point2 = cell2 + totalPoints;
	Union2(cluster, point1, point2);
}


void freeRegion(Region cellRgnRect)	//frees region
{
	free(cellRgnRect->iBottomLeft);
	free(cellRgnRect->iTopRight);
	free(cellRgnRect);
}


void printcelltype(BCellListHd cellList) //prints cell type of every cell in cell list
{
	BCellListNode cellListNode = cellList->first;
	if (cellList->count != 0)
	{
		while (cellListNode != NULL)
		{
			BCell currentCell = cellListNode->bCellElem;
			printf("%d\n", currentCell->cellDataList->cellType);
			cellListNode = cellListNode->next;
		}
	}
}


void determineCellList(GHdrNd GRTree, BCellListHd cellList) //determines the cell as core, dense, sparse or noise
{
	BCellListNode cellListNode = cellList->first;
	if (cellList->count != 0)
	{
		while (cellListNode != NULL)
		{
			BCell currentCell = cellListNode->bCellElem;
			if (currentCell->cellDataList->count > MINPOINTS)
			{
				currentCell->cellDataList->cellType = CORE;
				currentCell->cellDataList->hasCorePoint = TRUE;
				currentCell->cellDataList->isDetermined = TRUE;
				CellData currentNbhDataPoint = currentCell->cellDataList->first;
				while (currentNbhDataPoint != NULL) ////every point of core cell is a core point
				{
					currentNbhDataPoint->data->core_tag = TRUE;
					currentNbhDataPoint = currentNbhDataPoint->next;
				}
			}
			else
			{
				Region currentCellRegion = createRegionofCell(currentCell);
				Region immExtendedOfEpsCell = getEpsExtendedRegion(currentCellRegion, CELLSIZE);//Immediate Cells
				Region epsExtendedOfEpsCell = getEpsExtendedRegion(currentCellRegion, EPS);//Cells in EPS region
				GgetCellsInRegion3(GRTree, epsExtendedOfEpsCell, immExtendedOfEpsCell);//Returns number of points in Immediate and EPS region cells
				if (EPSCOUNT < MINPOINTS)
				{
					currentCell->cellDataList->cellType = NOISE;
					currentCell->cellDataList->isDetermined = TRUE;
				}
				else if (IMMCOUNT > MINPOINTS)
				{
					currentCell->cellDataList->cellType = DENSE;
					currentCell->cellDataList->hasCorePoint = TRUE;
					currentCell->cellDataList->isDetermined = TRUE;
					CellData currentNbhDataPoint = currentCell->cellDataList->first;
					while (currentNbhDataPoint != NULL) //every point of dense cell is a core point
					{
						currentNbhDataPoint->data->core_tag = TRUE;
						currentNbhDataPoint = currentNbhDataPoint->next;
					}
				}
				else
				{
					currentCell->cellDataList->cellType = SPARSE;
					currentCell->cellDataList->isDetermined = TRUE;
				}
				freeRegion(currentCellRegion);
				freeRegion(immExtendedOfEpsCell);
				freeRegion(epsExtendedOfEpsCell);
			}
			cellListNode = cellListNode->next;
		}
	}
}


void processCellList(GHdrNd GRTree, BCellListHd cellList, DataHdr dataList) //processes each cell in two traversals. Initially core and dense. Then sparse pointwise.
{
	BCellListNode cellListNode = cellList->first;
	if (cellList->count != 0)	//First traversal
	{
		while (cellListNode != NULL)
		{
			BCell currentCell = cellListNode->bCellElem;
			if (currentCell->cellDataList->cellType == CORE)
			{
				Region currentCellRegion = createRegionofCell(currentCell);
				Region cellExtendedOfCurrentCell = getEpsExtendedRegion(currentCellRegion, CELLSIZE);
				BCellListHd cellExtendedCellsOfCurrentCell;
				cellExtendedCellsOfCurrentCell = GgetCellsInRegion2(GRTree, cellExtendedOfCurrentCell, currentCellRegion);
				BCellListNode twoEpsCursor = cellExtendedCellsOfCurrentCell->first;
				if (cellExtendedCellsOfCurrentCell->count != 0)
				{
					while (twoEpsCursor != NULL)
					{
						BCell twoEpsCursorCell = twoEpsCursor->bCellElem;
						mergeCellClusterIds(currentCell->id, twoEpsCursorCell->id); //merge cluster with all immediate cells
						twoEpsCursor = twoEpsCursor->next;
					}
				}
				freeRegion(currentCellRegion);
				freeRegion(cellExtendedOfCurrentCell);
				freeCellsList(cellExtendedCellsOfCurrentCell);
			}
			else if (currentCell->cellDataList->cellType == DENSE)
			{
				Region currentCellRegion = createRegionofCell(currentCell);
				Region cellExtendedOfCurrentCell = getEpsExtendedRegion(currentCellRegion, CELLSIZE);
				BCellListHd cellExtendedCellsOfCurrentCell;
				cellExtendedCellsOfCurrentCell = GgetCellsInRegion2(GRTree, cellExtendedOfCurrentCell, currentCellRegion);
				BCellListNode twoEpsCursor = cellExtendedCellsOfCurrentCell->first;
				if (cellExtendedCellsOfCurrentCell->count != 0)
				{
					while (twoEpsCursor != NULL)
					{
						BCell twoEpsCursorCell = twoEpsCursor->bCellElem;
						if (twoEpsCursorCell->cellDataList->cellType == CORE || twoEpsCursorCell->cellDataList->cellType == DENSE || twoEpsCursorCell->cellDataList->hasCorePoint == TRUE)
						{
							mergeCellClusterIds(currentCell->id, twoEpsCursorCell->id); //merge cluster with all immediate cells
						}
						else
						{
							CellData currentNbhDataPoint = twoEpsCursorCell->cellDataList->first;
							while (currentNbhDataPoint != NULL)
							{
								//condition for checking if cluster is not yet assigned need to check both cell cluster and point cluster. cluster is the array.
								if ((cluster[twoEpsCursorCell->id + totalPoints].parent == twoEpsCursorCell->id + totalPoints && cluster[twoEpsCursorCell->id + totalPoints].rank == 0) && (cluster[currentNbhDataPoint->data->iNum].parent == currentNbhDataPoint->data->iNum && cluster[currentNbhDataPoint->data->iNum].rank == 0))
								{
									Union2(cluster, currentCell->id + totalPoints, currentNbhDataPoint->data->iNum); //directly union called because already know cluster of one is cell cluster.
								}
								currentNbhDataPoint = currentNbhDataPoint->next;
							}
						}
						twoEpsCursor = twoEpsCursor->next;
					}
				}
				freeRegion(currentCellRegion);
				freeRegion(cellExtendedOfCurrentCell);
				freeCellsList(cellExtendedCellsOfCurrentCell);
			}
			cellListNode = cellListNode->next;
		}
	}





	cellListNode = cellList->first;
	int nbhPointCount = 0;

	if (cellList->count != 0) //second traversal of cell list for sparse
	{
		while (cellListNode != NULL)
		{
			BCell currentCell = cellListNode->bCellElem;
			if (currentCell->cellDataList->cellType == SPARSE) //dont need for noise cell.
			{
				CellData currentSparseCellData = currentCell->cellDataList->first;
				while (currentSparseCellData != NULL)
				{
					/* This is with RgetNeighborHood call.
					//RHdrNd tempRtree= RbuildRTree(dataList);
					//RgetNeighborHood(tempRtree, currentSparseCellData->data, 0);
					//Data tempData= (Data)malloc(sizeof(struct data));
					//*tempData= *currentSparseCellData->data;
					//validateNeighborhood(currentSparseCellData->data->neighbors,tempData->neighbors);
					*/

					//Optimized Point wise query with GgetNeighborHood
					GgetNeighborHood(GRTree, currentSparseCellData->data); //gets all points in eps region of current data point


					if (currentSparseCellData->data->neighbors != NULL)
					{
						if (currentSparseCellData->data->neighbors->nbhCnt >= MINPOINTS)//point is core point
						{
							if ((cluster[currentSparseCellData->data->cellId + totalPoints].parent == currentSparseCellData->data->cellId + totalPoints && cluster[currentSparseCellData->data->cellId + totalPoints].rank == 0) && (cluster[currentSparseCellData->data->iNum].parent == currentSparseCellData->data->iNum && cluster[currentSparseCellData->data->iNum].rank == 0))
							{
								cluster[currentSparseCellData->data->iNum].parent = currentSparseCellData->data->cellId + totalPoints;
							}
							currentSparseCellData->data->core_tag = TRUE;
							currentCell->cellDataList->hasCorePoint == TRUE;
							RNB temp = currentSparseCellData->data->neighbors->nbFirst;
							while (temp != NULL)
							{
								Region nbhRegion = createCellRegOfPoint(dataList->dataClstElem + (temp->n - 1));
								BCell epsNbhNode = GfindCell(GRTree, nbhRegion);
								if (dataList->dataClstElem[temp->n - 1].core_tag == TRUE || epsNbhNode->cellDataList->cellType == CORE || epsNbhNode->cellDataList->cellType == DENSE)
								{
									//merge a core point in a sparse cell with another core point in its neighbourhood
									mergePointClusters(currentSparseCellData->data->iNum, dataList->dataClstElem[temp->n - 1].iNum, currentSparseCellData->data->cellId, dataList->dataClstElem[temp->n - 1].cellId);
								}
								else if (dataList->dataClstElem[temp->n - 1].core_tag != TRUE && (cluster[epsNbhNode->id + totalPoints].parent == epsNbhNode->id + totalPoints && cluster[epsNbhNode->id + totalPoints].rank == 0) && (cluster[dataList->dataClstElem[temp->n - 1].iNum].parent == dataList->dataClstElem[temp->n - 1].iNum && cluster[dataList->dataClstElem[temp->n - 1].iNum].rank == 0))
								{
									//merge a core point in a sparse cell with another point in its neighbourhood which has no cluster assigned till now-border point case.
									merge3(dataList->dataClstElem[temp->n - 1].iNum, currentSparseCellData->data->iNum, currentSparseCellData->data->cellId);
								}
								temp = temp->nbNext;
							}
						}
						else
						{
							RNB temp = currentSparseCellData->data->neighbors->nbFirst;
							while (temp != NULL)
							{
								Region nbhRegion = createCellRegOfPoint(dataList->dataClstElem + (temp->n - 1));
								BCell epsNbhNode = GfindCell(GRTree, nbhRegion);
								if (dataList->dataClstElem[temp->n - 1].core_tag == TRUE)//neigbour point is core
								{
									//merge if current point of sparse cell is not assigned any cluster till now.
									if ((cluster[currentSparseCellData->data->cellId + totalPoints].parent == currentSparseCellData->data->cellId + totalPoints && cluster[currentSparseCellData->data->cellId + totalPoints].rank == 0) && (cluster[currentSparseCellData->data->iNum].parent == currentSparseCellData->data->iNum && cluster[currentSparseCellData->data->iNum].rank == 0))
									{
										merge3(currentSparseCellData->data->iNum, dataList->dataClstElem[temp->n - 1].iNum, epsNbhNode->id);
									}
								}
								temp = temp->nbNext;
							}
						}
					}
					RfreeNeighborhood(currentSparseCellData->data); //free neighbourhood
					currentSparseCellData = currentSparseCellData->next;
				}
			}
			cellListNode = cellListNode->next;
		}
	}
}