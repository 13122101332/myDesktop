#include "GridRTree.h"

Boolean GisOverLap(Region rgnRectOne, Region rgnRectTwo)
{   //returs TRUE if the two rectangles are over lapping
	int iDim = 0;
	for(iDim = 0; iDim < DIMENSION; iDim++)
		if(rgnRectOne->iTopRight[iDim] <= rgnRectTwo->iBottomLeft[iDim] || rgnRectTwo->iTopRight[iDim] < rgnRectOne->iBottomLeft[iDim])
			return FALSE;
	return TRUE;
}

Boolean GisOverLap2(Region rgnRectOne, Region rgnRectTwo)
{   //returs TRUE if the two rectangles are over lapping
	int iDim = 0;
	for(iDim = 0; iDim < DIMENSION; iDim++)
		//if(rgnRectOne->iTopRight[iDim] <= rgnRectTwo->iBottomLeft[iDim] || rgnRectTwo->iTopRight[iDim] < rgnRectOne->iBottomLeft[iDim])
		if(rgnRectOne->iTopRight[iDim] <= rgnRectTwo->iBottomLeft[iDim] || rgnRectTwo->iTopRight[iDim] <= rgnRectOne->iBottomLeft[iDim])
			return FALSE;
	return TRUE;
}

Region GinitRgnRect(Dimension iBottomLeft, Dimension iTopRight)
{   //initializes the rectangle with the given bottom left and top right corners
    //if the values for the corners are specified NULL, initializes a rectangle with origin as co-ordinates for both corners.
	Region rgnRect = (Region)malloc(sizeof(*rgnRect));
	assert(rgnRect!=NULL);
    
    if(iBottomLeft != NULL)
		rgnRect->iBottomLeft = iBottomLeft;
	else{
		rgnRect->iBottomLeft = (Dimension)malloc(sizeof(double)*DIMENSION);
		assert(rgnRect->iBottomLeft!=NULL);
	}
    //rgnRect->iBottomLeft = (Dimension) calloc( DIMENSION, sizeof( dimension ) );


	if(iTopRight != NULL)
		rgnRect->iTopRight = iTopRight;
	else{
		rgnRect->iTopRight = (Dimension)malloc(sizeof(double)*DIMENSION);
		assert(rgnRect->iTopRight!=NULL);

	}

	return rgnRect;
}

GTreeNode GinitIntNd(Dimension iBottomLeft, Dimension iTopRight)
{   //intializes internal node of a Tree with rectangle whose bottom left and topright corners are given

	Region rgnRect = GinitRgnRect(iBottomLeft, iTopRight);
	// initializes a rectangle with the given coordonates for the bottom left and top right corners

	GTreeData tdInfo = (GTreeData)malloc(sizeof(*tdInfo));
	assert(tdInfo!=NULL);

	
	tdInfo->rgnRect = rgnRect;
    GTreeNode tnInfo = (GTreeNode)malloc(sizeof(*tnInfo));
    assert(tnInfo!=NULL);

	
	tnInfo->ndType = INTNODE;
	tnInfo->tdInfo = tdInfo;

	return tnInfo;
}

GTreeNode GinitExtNd(BCell bcellElem)
{  // initializes the external node of a tree with Data
   	assert(bcellElem!=NULL);

	GTreeNode tnInfo = (GTreeNode)malloc(sizeof(*tnInfo));
	assert(tnInfo!=NULL);
	
	GTreeData tdInfo = (GTreeData)malloc(sizeof(*tdInfo));
    assert(tdInfo!=NULL);

	tdInfo->bCellElem = bcellElem;	// Data
    tnInfo->ndType = EXTNODE;	// external node
	tnInfo->tdInfo = tdInfo;

	return tnInfo;
}

void GsetRect(GLstNd lstNd, GTreeNode tnInfo)
{   // copies the data in the tree node tnInfo to lstNd
    int iCnt = 0;
    switch(tnInfo->ndType)
    {   case INTNODE:
		//incase of internal node copy the bottom left and top right corners
		for(iCnt = 0; iCnt < DIMENSION; iCnt++)
        {   lstNd->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] = tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
			lstNd->tnInfo->tdInfo->rgnRect->iTopRight[iCnt] = tnInfo->tdInfo->rgnRect->iTopRight[iCnt];
     	}
		break;
        case EXTNODE:
		// in case of external node copy the data element
		for(iCnt = 0; iCnt < DIMENSION; iCnt++)
        {   lstNd->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] = tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt];
			lstNd->tnInfo->tdInfo->rgnRect->iTopRight[iCnt] = tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt];
		}
		break;
	}

	return;
}

GLstNd GpickChild(GHdrNd ptrChildLst, GTreeNode tnInfo)
{  // decides which node among the child nodes to be picked for insertion and returns a pointer to that node
    if(ptrChildLst == NULL)
		return NULL;

	GLstNd lstNdTemp = ptrChildLst->ptrFirstNd;
	GLstNd lstNdChild = NULL;
	double dMinExp = -1;
    Region rgnNewRect = GinitRgnRect(NULL, NULL);
    Region rgnFinalRect = GinitRgnRect(NULL, NULL);
    int iCnt;

    // for each child child in the list of child nodes do the following
	while(lstNdTemp != NULL)
    {   //call the expansionArea function to determine the are by which the child node has to enlarged to accomodate the new point or region.
		if(GexpansionArea(lstNdTemp->tnInfo->tdInfo->rgnRect, tnInfo, &dMinExp, rgnNewRect))
        {//if the expansionArea return true mark the node to be the one that might be picked. if the expansion is same as one of the previous nodes then compare the ares of the current noe and the previous node and pick the one with least area.
               if(dMinExp < 0)
               {     dMinExp = 0 - dMinExp;
                     if(Garea(lstNdChild->tnInfo->tdInfo->rgnRect) > Garea(lstNdTemp->tnInfo->tdInfo->rgnRect))
                     {
                     	lstNdChild = lstNdTemp;
                     	    for(iCnt =0; iCnt< DIMENSION; iCnt++)
					        {	rgnFinalRect->iBottomLeft[iCnt] = rgnNewRect->iBottomLeft[iCnt];
								rgnFinalRect->iTopRight[iCnt] = rgnNewRect->iTopRight[iCnt];
							}
                     }
                           
               }
			   else
			   {
			   		lstNdChild = lstNdTemp;
				    for(iCnt =0; iCnt< DIMENSION; iCnt++)
		         	{	rgnFinalRect->iBottomLeft[iCnt] = rgnNewRect->iBottomLeft[iCnt];
						rgnFinalRect->iTopRight[iCnt] = rgnNewRect->iTopRight[iCnt];
				 	}
			   }
				   //lstNdChild = lstNdTemp;
		}
    	lstNdTemp = lstNdTemp->ptrNextNd;
	}
    //for the node that is picked assign the region pointed by new rectangle region and return the node
    Region rgnRectTemp = lstNdChild->tnInfo->tdInfo->rgnRect;
	lstNdChild->tnInfo->tdInfo->rgnRect = rgnFinalRect;
	//rgnNewRect = NULL;

	free(rgnRectTemp->iBottomLeft);
	free(rgnRectTemp->iTopRight);
	free(rgnRectTemp);

	free(rgnNewRect->iBottomLeft);
	free(rgnNewRect->iTopRight);
	free(rgnNewRect);

	return lstNdChild;
}


Boolean GexpansionArea(Region rgnRect, GTreeNode tnInfo, Double ptrDMinExp, Region rgnNewRect)
{   // calculates if the area by which the rgnRect should be enlarged so as to include the tnInfo is less than the value pointed by ptrDMinExp and return TRUE and assigns rgnNewRect with the new enlarged rectangle.
    int iCnt = 0;
    Region rgnRectTemp = GinitRgnRect(NULL, NULL);
    for(iCnt = 0; iCnt < DIMENSION; iCnt++)
    {   switch(tnInfo->ndType)
        {   case INTNODE:
            //assign least of bottom left corner along each dimension to rgnRectTemp
			rgnRectTemp->iTopRight[iCnt] = (tnInfo->tdInfo->rgnRect->iTopRight[iCnt] > rgnRect->iTopRight[iCnt]) ? tnInfo->tdInfo->rgnRect->iTopRight[iCnt] : rgnRect->iTopRight[iCnt];
			rgnRectTemp->iBottomLeft[iCnt] = (tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] < rgnRect->iBottomLeft[iCnt]) ? tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] : rgnRect->iBottomLeft[iCnt];
            break;

		    case EXTNODE:
           //assign maximum of top right corner along each dimension to rgnRectTemp
			rgnRectTemp->iTopRight[iCnt] = (tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt] > rgnRect->iTopRight[iCnt]) ? tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt] : rgnRect->iTopRight[iCnt];
			rgnRectTemp->iBottomLeft[iCnt] = (tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt] < rgnRect->iBottomLeft[iCnt]) ? tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt] : rgnRect->iBottomLeft[iCnt];
			break;
		}
	}
    //calculate the difference in area for new rectangle and old rectangle
	double dExp = Garea(rgnRectTemp) - Garea(rgnRect);
    //in case there no value to compare ( -1 ) or incase the value is less than the value to be comparedcopy the rgnRectTemp to rgnRectNew to Return it.
	if(*ptrDMinExp == -1 || dExp <= *ptrDMinExp)
    {   if(dExp == *ptrDMinExp)
			*ptrDMinExp = 0 - dExp;
		else
			*ptrDMinExp = dExp;
        for(iCnt =0; iCnt< DIMENSION; iCnt++)
        {	rgnNewRect->iBottomLeft[iCnt] = rgnRectTemp->iBottomLeft[iCnt];
			rgnNewRect->iTopRight[iCnt] = rgnRectTemp->iTopRight[iCnt];
		}
		free(rgnRectTemp->iBottomLeft);
		free(rgnRectTemp->iTopRight);
		free(rgnRectTemp);
    //area to be enlarged is less than the previous value
		return TRUE;
	}

	free(rgnRectTemp->iBottomLeft);
	free(rgnRectTemp->iTopRight);
	free(rgnRectTemp);
    //area to be enlarged is not less than the previous value
	return FALSE;
}

double Garea(Region rgnRect)
{   //calcluates the area of rectangle
    if(rgnRect == NULL)
		return 0;
    double dArea = 1;
	int iCnt = 0;
    //multiply values along each dimension
	for(iCnt = 0; iCnt < DIMENSION; iCnt++)
		dArea = dArea * (rgnRect->iTopRight[iCnt] - rgnRect->iBottomLeft[iCnt]);
	return dArea;
}

Boolean GinsertTree(GHdrNd hdrNdTree, GTreeNode tnInfo, int gMinEntries, int gMaxEntries)
{   //insert a node into tree
	int iCnt = 0;

	if(hdrNdTree->ptrFirstNd == NULL || hdrNdTree->ptrFirstNd->tnInfo->ndType == EXTNODE)
    {    //incase of the external node insert the node into the child list and if the node has more tha max entries return true to indicat that
		GinsertLstElem(hdrNdTree, tnInfo);
		//should be there: printf(".... ...entries nw after insertion is : %d and limit is %d\n",hdrNdTree->uiCnt,GMAXENTRIES);
		return(hdrNdTree->uiCnt > gMaxEntries) ? TRUE : FALSE;
    }

	if(hdrNdTree->ptrFirstNd->ptrChildLst->uiCnt == 0)
		GsetRect(hdrNdTree->ptrFirstNd, tnInfo);

    //pick the child into which the node can be inserted
	GLstNd lstNdTemp = GpickChild(hdrNdTree, tnInfo);
	//expnandRect( lstNdTemp, tnInfo );

   //call insert tree on the child that is picked
	if(GinsertTree(lstNdTemp->ptrChildLst, tnInfo, gMinEntries, gMaxEntries))
    {   //incase the resultant insert has caused the node to over flow invoke split node
        //should be there: printf("\nBHAIYYA SPLIT\n");
        //should be there: printf("%lf\t%lf\t%d\n",lstNdTemp->tnInfo->tdInfo->dataClstElem->iData[0],lstNdTemp->tnInfo->tdInfo->dataClstElem->iData[0],isLstEmpty(lstNdTemp->ptrChildLst));
        GsplitNode(lstNdTemp,gMinEntries);
	    hdrNdTree->uiCnt++;
        //if after split node is invoked the node is overflowing return treu to to its parent to let it know that node is over flowing
	    return (hdrNdTree->uiCnt > gMaxEntries) ? TRUE : FALSE;
	}

	return FALSE;
}

GHdrNd GcreateRoot(GHdrNd hdrNdTree)
{   //in case of root split this is called to create a new root
    GHdrNd hdrNdRoot = GinitHdrNd();
    Dimension iBottomLeft = (Dimension)calloc(DIMENSION, sizeof(dimension));
	Dimension iTopRight = (Dimension)calloc(DIMENSION,sizeof(dimension));

	GLstNd lstNdTemp = hdrNdTree->ptrFirstNd;
	int iCnt = 0;
	Boolean bIsFirst = TRUE;

   //set the bottom left and top right corners for the new root
	while(lstNdTemp != NULL)
    {	for(iCnt = 0; iCnt < DIMENSION; iCnt++)
        {   if(bIsFirst)
            {   iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
				iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];
         	}
			else
            {   if(lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] < iBottomLeft[iCnt])
					iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
				if(lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt] > iTopRight[iCnt])
					iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];
    	    }
		}
		lstNdTemp = lstNdTemp->ptrNextNd;
		bIsFirst = FALSE;
	}

//initialize a node with the bottomleft and top right corners obtained and insert into the list
	hdrNdRoot->ptrFirstNd = GinitLstNd(GinitIntNd(iBottomLeft, iTopRight));
    hdrNdRoot->ptrFirstNd->ptrChildLst = hdrNdTree;
	hdrNdRoot->uiCnt = 1;

	return hdrNdRoot;
}

void GsplitNode(GLstNd ptrChild, int gminEntries)
{    
	// splits the node into two nodes
     //printf("\nBHAIYYA SPLIT\n");
     //printf("%lf\t%lf\n",ptrChild->tnInfo->tdInfo->dataClstElem->iData[0],ptrChild->tnInfo->tdInfo->dataClstElem->iData[0]);
	if(ptrChild == NULL || GisLstEmpty(ptrChild->ptrChildLst))
		return;

	GLstNd lstNdOne = NULL;
	GLstNd lstNdTwo = NULL;
	GTreeNode tnInfoTemp = NULL;

	GLstNd lstNdTemp = NULL;

	double dExpOne = -1;
	double dExpTwo = -1;

    //pick two nodes that are farthest along any dimension
	GpickSeeds(ptrChild->ptrChildLst, &lstNdOne, &lstNdTwo);

	if(lstNdOne == NULL || lstNdTwo == NULL)
		return;
    //printf("BHAIYYA SPLIT\t");
    //printf("%lf\t%lf\n",ptrChild->tnInfo->tdInfo->dataClstElem->iData[0],ptrChild->tnInfo->tdInfo->dataClstElem->iData[1]);

    //create two child lists
	GLstNd ptrChildTemp = GinitLstNd(GinitIntNd(NULL, NULL));
	GLstNd ptrChildSib = GinitLstNd(GinitIntNd(NULL, NULL));
	//GTreeNode tn = GinitIntNd(NULL, NULL);

    //link the two child lists so that one follows the other
	ptrChildTemp->ptrChildLst = GinitHdrNd();
	ptrChildSib->ptrChildLst = GinitHdrNd();
	ptrChildSib->ptrNextNd = ptrChild->ptrNextNd;

    //insert the picked children one into each of the list
	GinsertLstNd(ptrChildTemp->ptrChildLst, lstNdOne);
	GsetRect(ptrChildTemp, lstNdOne->tnInfo);
	GinsertLstNd(ptrChildSib->ptrChildLst, lstNdTwo);
	GsetRect(ptrChildSib, lstNdTwo->tnInfo);

	Region rgnNewRectOne = GinitRgnRect(NULL, NULL);
	Region rgnNewRectTwo = GinitRgnRect(NULL, NULL);

	Boolean bIsOne = FALSE;
	Boolean bIsNdOneInComp = FALSE;
	Boolean bIsNdTwoInComp = FALSE;

	int iCnt = 0;

	lstNdTemp = GdeleteLstFirst(ptrChild->ptrChildLst);

    //pick one element from the list of children of the node to be split
	while(lstNdTemp != NULL)
    {   //if one of the nodes has so few entires that all the remaining children are to be assigned set that node to be incomplete
		if(ptrChildTemp->ptrChildLst->uiCnt + ptrChild->ptrChildLst->uiCnt == gminEntries - 1)
			bIsNdOneInComp = TRUE;

		if(ptrChildSib->ptrChildLst->uiCnt + ptrChild->ptrChildLst->uiCnt == gminEntries - 1)
			bIsNdTwoInComp = TRUE;
        //if both nodes are not potentiall incomplete i.e. all the remaining children need not be assigned to it for the node not to underflow
		if(!bIsNdOneInComp && !bIsNdTwoInComp)
        {   dExpOne = -1;
		    dExpTwo = -1;
            //find the area by which the rectangle in each node should be expanded to accomodat the given rectangle
		    GexpansionArea(ptrChildTemp->tnInfo->tdInfo->rgnRect, lstNdTemp->tnInfo, &dExpOne, rgnNewRectOne);
		    GexpansionArea(ptrChildSib->tnInfo->tdInfo->rgnRect, lstNdTemp->tnInfo, &dExpTwo, rgnNewRectTwo);
            //printf("kahan:%d\t%d\t%d\t%d\n",rgnNewRectOne->iBottomLeft[0], rgnNewRectOne->iBottomLeft[1], rgnNewRectOne->iTopRight[0], rgnNewRectOne->iTopRight[1]);

            //ark the node that requires least expansion to be the list into which the child can be added
		    if(dExpOne < dExpTwo)
			     bIsOne = TRUE;
	        if(dExpOne > dExpTwo)
		         bIsOne = FALSE;
	        if(dExpOne == dExpTwo)
            {    //incase both reequired to be enlarged by same amount pick the one with lower area currently
		         double dAreaOne = Garea(ptrChildTemp->tnInfo->tdInfo->rgnRect);
			     double dAreaTwo = Garea(ptrChildSib->tnInfo->tdInfo->rgnRect);
			     if(dAreaOne < dAreaTwo)
                      bIsOne = TRUE;
                 if(dAreaOne > dAreaTwo)
                      bIsOne = FALSE;
                 if(dAreaOne == dAreaTwo)
                 {    //incase the area are same too pick the node which has lesser number of children
                      if(ptrChildTemp->ptrChildLst->uiCnt < ptrChildSib->ptrChildLst->uiCnt)
                           bIsOne = TRUE;
                      else
					       bIsOne = FALSE;
                 }
           }
		} //if
		
		else
        {   //if one of the nodes is potentially incomplete mark it to be the node to which the child has to be assigned
		    if(bIsNdOneInComp)
                  bIsOne = TRUE;
		    if(bIsNdTwoInComp)
			      bIsOne = FALSE;
		}

		if(bIsOne)
        {   //insert the new child
			GinsertLstNd(ptrChildTemp->ptrChildLst, lstNdTemp);
            if(bIsNdOneInComp)
            {   dExpOne = -1;
				GexpansionArea(ptrChildTemp->tnInfo->tdInfo->rgnRect, lstNdTemp->tnInfo, &dExpOne, rgnNewRectOne);
			}
            //expand the rectangle to accomodate new child
			for(iCnt = 0; iCnt < DIMENSION; iCnt++)
            {	ptrChildTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] = rgnNewRectOne->iBottomLeft[iCnt];
				ptrChildTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt] = rgnNewRectOne->iTopRight[iCnt];
			}
		}
		else
        {   //insert the new child
			GinsertLstNd(ptrChildSib->ptrChildLst, lstNdTemp);
            if(bIsNdTwoInComp)
            {   dExpTwo = -1;
				GexpansionArea(ptrChildSib->tnInfo->tdInfo->rgnRect, lstNdTemp->tnInfo, &dExpTwo, rgnNewRectTwo);
			}
            //expand the rectangle to accomodate the new child
			for(iCnt = 0; iCnt < DIMENSION; iCnt++)
            {	ptrChildSib->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] = rgnNewRectTwo->iBottomLeft[iCnt];
				ptrChildSib->tnInfo->tdInfo->rgnRect->iTopRight[iCnt] = rgnNewRectTwo->iTopRight[iCnt];
			}
		}
        //picke next node
		lstNdTemp = GdeleteLstFirst(ptrChild->ptrChildLst);
	}
    //set the node that is passed too first of the two nodes and set the next pointer of the second to the next pointer of the node that is passed..so that now in place of the node that is passed we have to nodes instead
	ptrChildTemp->ptrChildLst->ptrParentNd = ptrChildTemp;
	ptrChildSib->ptrChildLst->ptrParentNd = ptrChildSib;
	ptrChildTemp->ptrNextNd = ptrChildSib;
	ptrChild->ptrChildLst->ptrParentNd = NULL;
	free(ptrChild->ptrChildLst);

	free(ptrChild->tnInfo->tdInfo->rgnRect->iBottomLeft);
	free(ptrChild->tnInfo->tdInfo->rgnRect->iTopRight);
	free(ptrChild->tnInfo->tdInfo->rgnRect);

	free(ptrChild->tnInfo->tdInfo);
	free(ptrChild->tnInfo);

	ptrChild->tnInfo = ptrChildTemp->tnInfo;
	ptrChild->ptrChildLst = ptrChildTemp->ptrChildLst;
	ptrChild->ptrNextNd = ptrChildTemp->ptrNextNd;

	ptrChildTemp->ptrNextNd = NULL;
	ptrChildTemp->ptrChildLst = NULL;
	ptrChildTemp->tnInfo = NULL;
	free(ptrChildTemp);

	free(rgnNewRectOne->iBottomLeft);
	free(rgnNewRectOne->iTopRight);
	free(rgnNewRectOne);
	free(rgnNewRectTwo->iBottomLeft);
	free(rgnNewRectTwo->iTopRight);
	free(rgnNewRectTwo);

	return;
}

void GpickSeeds(GHdrNd ptrChildLst, GLstNd *lstNdChildOne, GLstNd *lstNdChildTwo)
{    //in case a node has to be split pick the two child nodes that are used to create new child lists
	if(ptrChildLst == NULL)
		return;
    //used to store pointers to nodes of maximum bottom left and minimum top right corners along each dimension
	GTreeNode *tnInfoMin = (GTreeNode *)malloc(DIMENSION * sizeof(GTreeNode));
	GTreeNode *tnInfoMax = (GTreeNode *)malloc(DIMENSION * sizeof(GTreeNode));

	GLstNd lstNdTemp = NULL;
    int iCnt = 0;
	Boolean bIsFirst = TRUE;

	Region rgnRectTemp;
	Region rgnRectOut;

	double dNormSep = 0;
	double dMaxNormSep = 0;
	dimension dimMaxNormSep = 0;
	GTreeNode temp;

	switch(ptrChildLst->ptrFirstNd->tnInfo->ndType)
    {	case INTNODE:   lstNdTemp = ptrChildLst->ptrFirstNd;
                        //for storing maximum bottomleft and minimum top right corners that are found till now
	                    Region rgnRectTemp = GinitRgnRect(NULL, NULL);
	                    // Region rgnRectTemp2= initRgnRect(NULL, NULL);
	           //          for(iCnt = 0; iCnt < DIMENSION; iCnt++)
            //             {
            //             	if(lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] > lstNdTemp->ptrNextNd->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt])
            //             	{
            //             		rgnRectTemp->iBottomLeft[iCnt] =   lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
            //             		rgnRectTemp->iTopRight[iCnt] = lstNdTemp->ptrNextNd->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];

            //             	}
            //             	else
            //             	{
            //             		rgnRectTemp->iBottomLeft[iCnt] =   lstNdTemp->ptrNextNd->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
            //             		rgnRectTemp->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];

            //             	}
                        	 
				        // rgnRectTemp->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];
            //             }
	                    
                        //for storing the minim bottom left and maximum topr right corner
	                    Region rgnRectOut = GinitRgnRect(NULL, NULL);
                        while(lstNdTemp != NULL)
                        {     for(iCnt = 0; iCnt < DIMENSION; iCnt++)
                              {     
                              	if(bIsFirst)
                                    {     //if this is first node set its corners to max bottom left and min top right corners
				                          rgnRectTemp->iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
				                          rgnRectTemp->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];
				                          rgnRectOut->iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
				                          rgnRectOut->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];
                                          //set the marker to the at the iCnt to dimesnion to current node if it has the max bottom left and min top right corners.
				                          tnInfoMin[iCnt] = lstNdTemp->tnInfo;
				                          tnInfoMax[iCnt] = lstNdTemp->tnInfo;
				                          continue;
                                    }
			                        if(lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] > rgnRectTemp->iBottomLeft[iCnt])
                                    {     //if the current node has greater bottom left corner than the node pointed by the tnInfoMin[ iCnt ] along iCnt Dimension assign it to tnInfoMin[ iCnt ]
				                          rgnRectTemp->iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
				                          tnInfoMin[iCnt] = lstNdTemp->tnInfo;
                                    }
			                        if(lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt] < rgnRectTemp->iTopRight[iCnt])
                                    {     //if the current node has lower top right corner than the node pointed by the tnInfoMax[ iCnt ] along iCnt Dimension assign it to tnInfoMax[ iCnt ]
				                          rgnRectTemp->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];
				                          tnInfoMax[iCnt] = lstNdTemp->tnInfo;
                                    }
			                        // else
                           //          {     //make sure that the max bottom left and min top right are not for the same rectangle
				                       //    if(tnInfoMin[iCnt] == tnInfoMax[iCnt])
                           //                      tnInfoMax[ iCnt ] = lstNdTemp->tnInfo;
                           //          }
                                    //sotre the minimum bottom left corner along each dimension
			                        if(lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt] < rgnRectOut->iBottomLeft[iCnt])
                                                rgnRectOut->iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt];
                                    //sotre the maximum top right corner along each dimension
			                        if(lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt] > rgnRectOut->iTopRight[iCnt])
                                                rgnRectOut->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt];
                               }
                               lstNdTemp = lstNdTemp->ptrNextNd;
		                       if(bIsFirst)
                                    bIsFirst = FALSE;
                         }	//while
	                     double dNormSep = 0;
	                     double dMaxNormSep = 0;
	                     dimension dimMaxNormSep = 0;
	                     for(iCnt = 0; iCnt < DIMENSION; iCnt++)
                         {     //calculate normal seperation along each dimension
                               dNormSep = fabs(rgnRectTemp->iBottomLeft[iCnt] - rgnRectTemp->iTopRight[iCnt]) / fabs(rgnRectOut->iTopRight[iCnt] - rgnRectOut->iBottomLeft[iCnt]);
                               if(dNormSep > dMaxNormSep)
                               {   dMaxNormSep = dNormSep;
			                       dimMaxNormSep = iCnt;
                               }
                         }
                         
                         if(tnInfoMin[(int)dimMaxNormSep] == tnInfoMax[(int)dimMaxNormSep])
                         {
                         	lstNdTemp = ptrChildLst->ptrFirstNd;
                         	temp=tnInfoMax[(int)dimMaxNormSep];
                         	if(temp != lstNdTemp->tnInfo)
                         	{
                         		rgnRectTemp->iTopRight[(int)dimMaxNormSep] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[(int)dimMaxNormSep];
                         		 tnInfoMax[(int)dimMaxNormSep] = lstNdTemp->tnInfo;
                         		lstNdTemp = lstNdTemp->ptrNextNd;
                         	}
                         	else
                         	{
                         		rgnRectTemp->iTopRight[(int)dimMaxNormSep] = lstNdTemp->ptrNextNd->tnInfo->tdInfo->rgnRect->iTopRight[(int)dimMaxNormSep];
                         		tnInfoMax[(int)dimMaxNormSep] = lstNdTemp->ptrNextNd->tnInfo;
                         		lstNdTemp = lstNdTemp->ptrNextNd->ptrNextNd;
                         	}
                         	while(lstNdTemp != NULL)
                         	{
                         		if(lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[(int)dimMaxNormSep] < rgnRectTemp->iTopRight[(int)dimMaxNormSep] && temp != lstNdTemp->tnInfo)
                         		{
                         			rgnRectTemp->iTopRight[(int)dimMaxNormSep] = lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[(int)dimMaxNormSep];
				                          tnInfoMax[(int)dimMaxNormSep] = lstNdTemp->tnInfo;
                         		}
                         			lstNdTemp = lstNdTemp->ptrNextNd;
                         	} 
                         }
                         
                         if(tnInfoMin[(int)dimMaxNormSep]==tnInfoMax[(int)dimMaxNormSep])
                         {
                         	printf("error in the code\n");
                         	exit(-1);
                         }
                         // remove the node pointed by tnInfoMin at tnInfoMax at dMaxNormSep and assign them to be the two nodes that are picked for split
                         // printf("inside pickSeeds\n");
                         // printAllChildrenInList(ptrChildLst);
                         //  printf("printing first node to be deleted\n");
                         // printTnInfo(tnInfoMin[(int)dimMaxNormSep]);
	                     *lstNdChildOne = GdeleteLstElem(ptrChildLst, tnInfoMin[(int)dimMaxNormSep]);
	                     // printf("printing first node deleted\n");
	                     // printNode(*lstNdChildOne);
	                     // printf("intermediate pickSeeds\n");
                      //    printAllChildrenInList(ptrChildLst);
                      //    printf("printing second node to be deleted\n");
                      //    printTnInfo(tnInfoMax[(int)dimMaxNormSep]);
	                     // if(tnInfoMin[(int)dimMaxNormSep]==tnInfoMax[(int)dimMaxNormSep])
	                     // 		printAllChildrenInList(ptrChildLst);
	                     *lstNdChildTwo = GdeleteLstElem(ptrChildLst, tnInfoMax[(int)dimMaxNormSep]);
	                      // if(tnInfoMin[(int)dimMaxNormSep]==tnInfoMax[(int)dimMaxNormSep])
	                     	// 	printAllChildrenInList(ptrChildLst);
	     //                 printf("printing second node deleted\n");
	     //                 printNode(*lstNdChildTwo);
						// printf("after both deletion\n");
      //                    printAllChildrenInList(ptrChildLst);
	                     // if(*lstNdChildTwo==NULL)//in case both seeds pick up same child
	                     // {
	                     // 	// if(ptrChildLst->uiCnt==0)
	                     // 	// 	printf("list isLstEmpty\n");
	                     // 	// printf("lstNdTemp NULL %d\n",ptrChildLst->uiCnt);
	                     // 	// printAllChildrenInList(ptrChildLst);
	                     // 	// getchar();
	                     // 	*lstNdChildTwo = deleteLstFirst(ptrChildLst);
	                     // 	// printAllChildrenInList(ptrChildLst);
	                     // 	// getchar();
	                     // }
	                     // if(*lstNdChildTwo==NULL)
	                     // 	printf("lstNdChildTwo nul\n");
	                     free(rgnRectTemp->iBottomLeft);
	                     free(rgnRectTemp->iTopRight);
	                     free(rgnRectTemp);
	                     free(rgnRectOut->iBottomLeft);
	                     free(rgnRectOut->iTopRight);
	                     free(rgnRectOut);
                         break;


	case EXTNODE:        //same as in case of INTNODE but only with the difference that max seperation instead of normal seperation is picked.. so just find two nodes which are at the farthest distance along each dimension
	                     // lstNdTemp = ptrChildLst->ptrFirstNd;
	                     // DataPoint iDataMin = (DataPoint)calloc(DIMENSION, sizeof(dataPoint));
	                     // DataPoint iDataMax = (DataPoint)calloc(DIMENSION, sizeof(dataPoint));
                      //    bIsFirst = TRUE;
	                     // while(lstNdTemp != NULL)
                      //    {     for(iCnt = 0; iCnt < DIMENSION; iCnt++)
                      //          {   if(bIsFirst)
                      //              {    iDataMin[iCnt] = lstNdTemp->tnInfo->tdInfo->dataClstElem->iData[iCnt];
				                  //       iDataMax[iCnt] = lstNdTemp->tnInfo->tdInfo->dataClstElem->iData[iCnt];
                      //                  	tnInfoMin[iCnt] = lstNdTemp->tnInfo;
				                  //       tnInfoMax[iCnt] = lstNdTemp->tnInfo;
			                   //       	continue;
                      //              }
			                   //     if(lstNdTemp->tnInfo->tdInfo->dataClstElem->iData[iCnt] <= iDataMin[iCnt])
                      //              {    iDataMin[iCnt] = lstNdTemp->tnInfo->tdInfo->dataClstElem->iData[iCnt];
				                  //       tnInfoMin[iCnt] = lstNdTemp->tnInfo;
                      //          	   }
		                    //        if(lstNdTemp->tnInfo->tdInfo->dataClstElem->iData[iCnt] > iDataMax[iCnt])
                      //              {    iDataMax[iCnt] = lstNdTemp->tnInfo->tdInfo->dataClstElem->iData[iCnt];
                      //                   tnInfoMax[iCnt]  = lstNdTemp->tnInfo;
                      //              }
                      //      	    }
                      //           lstNdTemp = lstNdTemp->ptrNextNd;
                      //          	if(bIsFirst)
                      //               bIsFirst = FALSE;
                      //    } //while
	                     // double dSep = 0;
	                     // double dMaxSep = 0;
	                     // dimension dimMaxSep = 0;
	                     // for(iCnt = 0; iCnt < DIMENSION; iCnt++)
                      //    {      dSep = fabs(iDataMax[iCnt] - iDataMin[iCnt]);
		                    //     if(dSep > dMaxSep)
                      //           {   dMaxSep = dSep;
			                   //      dimMaxSep = iCnt;
	                     //        }
                      //    }
	                     // *lstNdChildOne = deleteLstElem(ptrChildLst, tnInfoMin[(int)dimMaxSep]);
	                     // *lstNdChildTwo = deleteLstElem(ptrChildLst, tnInfoMax[(int)dimMaxSep]);

	                     // free(iDataMin);
	                     // free(iDataMax);
                      //    break;
						lstNdTemp = ptrChildLst->ptrFirstNd;
                        //for storing maximum bottomleft and minimum top right corners that are found till now
	                    rgnRectTemp = GinitRgnRect(NULL, NULL);
                        //for storing the minim bottom left and maximum topr right corner
	                    rgnRectOut = GinitRgnRect(NULL, NULL);
                        while(lstNdTemp != NULL)
                        {     for(iCnt = 0; iCnt < DIMENSION; iCnt++)
                              {     if(bIsFirst)
                                    {     //if this is first node set its corners to max bottom left and min top right corners
				                          rgnRectTemp->iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt];
				                          rgnRectTemp->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt];
				                          rgnRectOut->iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt];
				                          rgnRectOut->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt];
                                          //set the marker to the at the iCnt to dimesnion to current node if it has the max bottom left and min top right corners.
				                          tnInfoMin[iCnt] = lstNdTemp->tnInfo;
				                          tnInfoMax[iCnt] = lstNdTemp->tnInfo;
				                          continue;
                                    }
			                        if(lstNdTemp->tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt] > rgnRectTemp->iBottomLeft[iCnt])
                                    {     //if the current node has greater bottom left corner than the node pointed by the tnInfoMin[ iCnt ] along iCnt Dimension assign it to tnInfoMin[ iCnt ]
				                          rgnRectTemp->iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt];
				                          tnInfoMin[iCnt] = lstNdTemp->tnInfo;
                                    }
			                        if(lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt] < rgnRectTemp->iTopRight[iCnt])
                                    {     //if the current node has lower top right corner than the node pointed by the tnInfoMax[ iCnt ] along iCnt Dimension assign it to tnInfoMax[ iCnt ]
				                          rgnRectTemp->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt];
				                          tnInfoMax[iCnt] = lstNdTemp->tnInfo;
                                    }
			                        // else
                           //          {     //make sure that the max bottom left and min top right are not for the same rectangle
				                       //    if(tnInfoMin[iCnt] == tnInfoMax[iCnt])
                           //                      tnInfoMax[ iCnt ] = lstNdTemp->tnInfo;
                           //          }
                                    //sotre the minimum bottom left corner along each dimension
			                        if(lstNdTemp->tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt] < rgnRectOut->iBottomLeft[iCnt])
                                                rgnRectOut->iBottomLeft[iCnt] = lstNdTemp->tnInfo->tdInfo->bCellElem->minOriginalBoundary[iCnt];
                                    //sotre the maximum top right corner along each dimension
			                        if(lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt] > rgnRectOut->iTopRight[iCnt])
                                                rgnRectOut->iTopRight[iCnt] = lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[iCnt];
                               }
                               lstNdTemp = lstNdTemp->ptrNextNd;
		                       if(bIsFirst)
                                    bIsFirst = FALSE;
                         }	//while
	                     dNormSep = 0;
	                     dMaxNormSep = 0;
	                     dimMaxNormSep = 0;
	                     for(iCnt = 0; iCnt < DIMENSION; iCnt++)
                         {     //calculate normal seperation along each dimension
                               dNormSep = fabs(rgnRectTemp->iBottomLeft[iCnt] - rgnRectTemp->iTopRight[iCnt]) / fabs(rgnRectOut->iTopRight[iCnt] - rgnRectOut->iBottomLeft[iCnt]);
                               if(dNormSep > dMaxNormSep)
                               {   dMaxNormSep = dNormSep;
			                       dimMaxNormSep = iCnt;
                               }
                         }

                         if(tnInfoMin[(int)dimMaxNormSep] == tnInfoMax[(int)dimMaxNormSep])
                         {
                         	lstNdTemp = ptrChildLst->ptrFirstNd;
                         	temp=tnInfoMax[(int)dimMaxNormSep];
                         	if(temp != lstNdTemp->tnInfo)
                         	{
                         		rgnRectTemp->iTopRight[(int)dimMaxNormSep] = lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[(int)dimMaxNormSep];
                         		 tnInfoMax[(int)dimMaxNormSep] = lstNdTemp->tnInfo;
                         		lstNdTemp = lstNdTemp->ptrNextNd;
                         	}
                         	else
                         	{
                         		rgnRectTemp->iTopRight[(int)dimMaxNormSep] = lstNdTemp->ptrNextNd->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[(int)dimMaxNormSep];
                         		tnInfoMax[(int)dimMaxNormSep] = lstNdTemp->ptrNextNd->tnInfo;
                         		lstNdTemp = lstNdTemp->ptrNextNd->ptrNextNd;
                         	}
                         	while(lstNdTemp != NULL)
                         	{
                         		if(lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[(int)dimMaxNormSep] < rgnRectTemp->iTopRight[(int)dimMaxNormSep] && temp != lstNdTemp->tnInfo)
                         		{
                         			rgnRectTemp->iTopRight[(int)dimMaxNormSep] = lstNdTemp->tnInfo->tdInfo->bCellElem->maxOriginalBoundary[(int)dimMaxNormSep];
				                          tnInfoMax[(int)dimMaxNormSep] = lstNdTemp->tnInfo;
                         		}
                         			lstNdTemp = lstNdTemp->ptrNextNd;
                         	} 
                         }

                         // remove the node pointed by tnInfoMin at tnInfoMax at dMaxNormSep and assign them to be the two nodes that are picked for split
	                     *lstNdChildOne = GdeleteLstElem(ptrChildLst, tnInfoMin[(int)dimMaxNormSep]);
	                     *lstNdChildTwo = GdeleteLstElem(ptrChildLst, tnInfoMax[(int)dimMaxNormSep]);

	                     free(rgnRectTemp->iBottomLeft);
	                     free(rgnRectTemp->iTopRight);
	                     free(rgnRectTemp);
	                     free(rgnRectOut->iBottomLeft);
	                     free(rgnRectOut->iTopRight);
	                     free(rgnRectOut);
                         break;
	}//switch

	free(tnInfoMin);
	free(tnInfoMax);

	return;
}

Boolean GisContainsCell(Region rgnRect, BCell bCellElem)
{   //return TRUE if the cell is contained in a rectangle
	int iCnt = 0;
	Boolean bIsContains = TRUE;
	for(iCnt = 0; iCnt < DIMENSION; iCnt++)
    {   if((rgnRect->iBottomLeft[iCnt] > bCellElem->minOriginalBoundary[iCnt]) || (rgnRect->iTopRight[iCnt] < bCellElem->maxOriginalBoundary[iCnt]))
        {   bIsContains = FALSE;
			break;
		}
	}
	return bIsContains;
}

BCell GfindCell(GHdrNd hdrNdTree, Region rgnRect)
{   
	//finds the record in the given search rectangle and returns  number of records found
    //if datapoint is passed in iData finds the number of records in eps neighborhood

	if(GisLstEmpty(hdrNdTree) || rgnRect == NULL || rgnRect->iBottomLeft == NULL || rgnRect->iTopRight == NULL)
		return NULL;

	unsigned int uiRecCnt = 0;
	int iCnt = 0;
    double t;
    static int flag = 0;
	GLstNd lstNdTemp = hdrNdTree->ptrFirstNd;

	while(lstNdTemp != NULL)
    {	//nodesVisited++;
		switch(lstNdTemp->tnInfo->ndType)
        {   case INTNODE:   //incase of internal node if the node is over lapping with search rectangle descend into the node and add to the count the number of new records found if any
				            if(GisOverLap(lstNdTemp->tnInfo->tdInfo->rgnRect, rgnRect))
				            {
				            	BCell currBCell = GfindCell(lstNdTemp->ptrChildLst, rgnRect);
				            	if(currBCell!=NULL)
				            	{
				            		return currBCell;
				            	}
				            }
				            	                                   
                            //printf("\nuiRecCnt = %d", uiRecCnt);
                            break;
            case EXTNODE:   
            				if(GisContainsCell(rgnRect, lstNdTemp->tnInfo->tdInfo->bCellElem))
            				{
            					return lstNdTemp->tnInfo->tdInfo->bCellElem;

            				}
                   			            				
	                        break;

		}//switch
		lstNdTemp = lstNdTemp->ptrNextNd;
	}  
	return NULL;
}

void GprintTree(GHdrNd hdrNdTree)
{   //travers along the tree and print the tree
	GLstNd lstNdTemp = hdrNdTree->ptrFirstNd;
	int iCnt = 0;
	static int iIndent = 0;
	iIndent++;

	while(lstNdTemp != NULL)
    {   
    	for(iCnt = 0; iCnt < iIndent; iCnt++)
			printf("---");

		if(lstNdTemp->tnInfo->ndType == INTNODE)
        {   printf("i hav %d children..\n",lstNdTemp->ptrChildLst->uiCnt);
			printf(" BottomLeft: ");
			for(iCnt = 0; iCnt < DIMENSION; iCnt++)
				printf("%lf ", lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt]);
            printf(" TopRight: ");
			for(iCnt = 0; iCnt < DIMENSION; iCnt++)
				printf("%lf ", lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt]);
            printf("\n");
			printf("i hav %d children..\n",lstNdTemp->ptrChildLst->uiCnt);
			GprintTree(lstNdTemp->ptrChildLst);
		}
		else
        {   printf("Cell:\n");
    		//int dim = 0;
			// for(iCnt = 0; iCnt < DIMENSION; iCnt++)
			// 	printf("%lf ", lstNdTemp->tnInfo->tdInfo->bcellElem->iData[iCnt]);
			// printf("\n");
    		BCell currBCell = lstNdTemp->tnInfo->tdInfo->bCellElem;
    		
			if(currBCell->cellDataList->count!=0)
			{
				printCell(currBCell);								
			}
			else
			{
				printf("There are no points in this Cell\n");
			}

			printf("End of this Cell:\n ");    				
    		
		}
		lstNdTemp = lstNdTemp->ptrNextNd;
	}
	iIndent--;
	return;
}

void GprintRegion(Region region)
{
    int i=0;
    printf("Printing Region:\n");

    printf("iBottomLeft is: ");
    for (i=0;i<DIMENSION;i++)
    {
        printf("%lf ", region->iBottomLeft[i]);
        
    }
    
    printf("iTopRight is: ");
    for (i=0;i<DIMENSION;i++)
    {
        printf("%lf ", region->iTopRight[i]);
        
    }
    printf("\n");

    return;
    
}

// unsigned int getNeighborHood(HdrNd hdrNdTree, Data dataTemp,int size_data)
// {   
	
// 	if(hdrNdTree == NULL || dataTemp == NULL)
// 		return 0;

// 	Region rgnRect = initRgnRect(NULL, NULL);
// 	int iCnt = 0;
// 	unsigned int uiRecCnt = 0;
// 	for(iCnt = 0; iCnt < DIMENSION; iCnt++)
//     {   if((dataTemp->iData[iCnt] - EPS) < 0)
// 			rgnRect->iBottomLeft[iCnt] = 0;
// 		else
// 			rgnRect->iBottomLeft[iCnt] = dataTemp->iData[iCnt] - EPS;
// 		rgnRect->iTopRight[iCnt] = dataTemp->iData[iCnt] + EPS;
// 	}
// 	dataTemp->neighbors = initNbHdr(size_data);
//     uiRecCnt = findRecords(hdrNdTree, rgnRect, dataTemp);

//     if(dataTemp->neighbors->nbhCnt >= MINPOINTS)
//     {   
//     	dataTemp->core_tag = TRUE;
//     } 
// 	free(rgnRect->iBottomLeft);
// 	free(rgnRect->iTopRight);
// 	free(rgnRect);

// 	return uiRecCnt;
// }

void GgetNeighborHood(GHdrNd hdrNdTree, Data currDataPoint)
{
	Region pointRgn = getEpsExtendedRegionPoint(currDataPoint,EPS);

	BCellListHd pointNbhCells = GgetCellsInRegion(hdrNdTree,pointRgn,NULL);
	Data temp = (Data) malloc(sizeof(*temp));
	*temp = *currDataPoint;
	int i=0; int val=0;

	currDataPoint->neighbors = RinitNbHdr();

	BCellListNode currCellNode = pointNbhCells->first;

	BCell currBCell;

	RHdrNd currAuxRTree;

	while(currCellNode!=NULL)
	{
		currBCell = currCellNode->bCellElem;
		currAuxRTree = currBCell->auxCellRTree;
		val = RgetNeighborHood(currAuxRTree,temp,0);

		//nbh of currDataPoint is to be appended to the temp
		appendNbh(currDataPoint,temp);
		currCellNode = currCellNode->next;
	}

	free(pointRgn->iBottomLeft);
    free(pointRgn->iTopRight);
    free(pointRgn);

    free(temp);

	freeCellsList(pointNbhCells);

	//printf("the coords are %d \n",currDataPoint->iNum);

	//RprintNbhLst(currDataPoint->neighbors,NULL);

	return;

}


void appendNbh(Data currDataPoint, Data temp)
{
	if(currDataPoint->neighbors->nbFirst==NULL)
	{
		free(currDataPoint->neighbors);
		currDataPoint->neighbors=temp->neighbors;
		
		return;
	}
	else if (temp->neighbors->nbFirst==NULL)
	{
		free(temp->neighbors);
		return;
	}
	else
	{
		currDataPoint->neighbors->nbLast->nbNext=temp->neighbors->nbFirst;
		currDataPoint->neighbors->nbLast = temp->neighbors->nbLast;
		currDataPoint->neighbors->nbhCnt+=temp->neighbors->nbhCnt;
		free(temp->neighbors);
		return;

	}

	return;

}


BCellListHd GgetCellsInRegion(GHdrNd hdrNdTree, Region regRect, Region cellRgnRect)
{
	// retrieve all the cells overlapping with this region
	BCellListHd cellsList = initBCellListHd();
	//printf("%d\n", cellsList->count);
	int noOfCells = GfindOverlappingCells(hdrNdTree,regRect,cellRgnRect,cellsList);
	return cellsList;
}

// this function excludes the border points located in remote cells falling out of the eps extended region when eps = cellsize
BCellListHd GgetCellsInRegion2(GHdrNd hdrNdTree, Region regRect, Region cellRgnRect)
{
	// retrieve all the cells overlapping with this region
	BCellListHd cellsList = initBCellListHd();
	//printf("%d\n", cellsList->count);
	int noOfCells = GfindOverlappingCells2(hdrNdTree,regRect,cellRgnRect,cellsList);
	return cellsList;
}

void GgetCellsInRegion3(GHdrNd hdrNdTree, Region regRect, Region cellRgnRect)
{
	// retrieve all the cells overlapping with this region
	BCellListHd cellsList = initBCellListHd();
	BCellListHd cellsList2 = initBCellListHd();
	//printf("%d\n", cellsList->count);
	int noOfCells = GfindOverlappingCells3(hdrNdTree,regRect,cellRgnRect,cellsList,cellsList2);

	IMMCOUNT=cellsList2->count;
	EPSCOUNT= cellsList->count;
	return;
}

unsigned int GfindOverlappingCells(GHdrNd hdrNdTree, Region rgnRect, Region cellRgnRect, BCellListHd cellsList)
{
	// recursively adds the ovelapping cells to the cellsList

	if(GisLstEmpty(hdrNdTree) || rgnRect == NULL || rgnRect->iBottomLeft == NULL || rgnRect->iTopRight == NULL)
		return;

	Region currCellRgn;
	unsigned int uiRecCnt = 0;
	int iCnt = 0;
    double t;
    static int flag = 0;
    int k = 0, currflag=0;
	GLstNd lstNdTemp = hdrNdTree->ptrFirstNd;
	// int nodesVisited;
	while(lstNdTemp != NULL)
    {	
    	//nodesVisited++;
		switch(lstNdTemp->tnInfo->ndType)
        {   case INTNODE:   //incase of internal node if the node is over lapping with search rectangle descend into the node and add to the count the number of new records found if any
				            if(GisOverLap(lstNdTemp->tnInfo->tdInfo->rgnRect, rgnRect)==TRUE)
                                   uiRecCnt += GfindOverlappingCells(lstNdTemp->ptrChildLst, rgnRect, cellRgnRect, cellsList);
                            //printf("\nuiRecCnt = %d", uiRecCnt);
                            break;
            case EXTNODE:  
            				currCellRgn = createRegionofCell(lstNdTemp->tnInfo->tdInfo->bCellElem);
            				if(GisOverLap(currCellRgn, rgnRect)==TRUE)
            				{
            					// Region currCellRgn = createRegionofCell(lstNdTemp->tnInfo->tdInfo->bCellElem);

            					// k=0; currflag=0;
            					// if(cellRgnRect!=NULL)
            					// {
            					// 	for(k=0;k<DIMENSION;k++)
            					// 	{
	            				// 		if(cellRgnRect->iBottomLeft[k] != currCellRgn->iBottomLeft[k] || cellRgnRect->iTopRight[k] != currCellRgn->iTopRight[k])
	            				// 	 	{
	            				// 	 		currflag=1;
	            				// 	 		break;
	            				// 	 	}

            					// 	}

            					// }
            					// else
            					// {
            					// 	currflag=1;
            					// }            					

            					// if (currflag==1)
            					// {
            						uiRecCnt++;
            						insertBCellIntoBCellList(lstNdTemp->tnInfo->tdInfo->bCellElem,cellsList);
            					// }            					

            				}

            				free(currCellRgn->iBottomLeft);
        					free(currCellRgn->iTopRight);
        					free(currCellRgn);
                                   
                            //printf("\nuiRecCnt = %d", uiRecCnt);
                               
	                        break;

		}//switch
		lstNdTemp = lstNdTemp->ptrNextNd;
	}  
	return uiRecCnt;

}

unsigned int GfindOverlappingCells2(GHdrNd hdrNdTree, Region rgnRect, Region cellRgnRect, BCellListHd cellsList)
{
	// recursively adds the ovelapping cells to the cellsList

	if(GisLstEmpty(hdrNdTree) || rgnRect == NULL || rgnRect->iBottomLeft == NULL || rgnRect->iTopRight == NULL)
		return;

	Region currCellRgn;
	unsigned int uiRecCnt = 0;
	int iCnt = 0;
    double t;
    static int flag = 0;
    int k = 0, currflag=0;
	GLstNd lstNdTemp = hdrNdTree->ptrFirstNd;
	// int nodesVisited;
	while(lstNdTemp != NULL)
    {	
    	//nodesVisited++;
		switch(lstNdTemp->tnInfo->ndType)
        {   case INTNODE:   //incase of internal node if the node is over lapping with search rectangle descend into the node and add to the count the number of new records found if any
				            if(GisOverLap2(lstNdTemp->tnInfo->tdInfo->rgnRect, rgnRect)==TRUE)
                                   uiRecCnt += GfindOverlappingCells2(lstNdTemp->ptrChildLst, rgnRect, cellRgnRect, cellsList);
                            //printf("\nuiRecCnt = %d", uiRecCnt);
                            break;
            case EXTNODE:  
            				currCellRgn = createRegionofCell(lstNdTemp->tnInfo->tdInfo->bCellElem);
            				if(GisOverLap2(currCellRgn, rgnRect)==TRUE)
            				{
            					// Region currCellRgn = createRegionofCell(lstNdTemp->tnInfo->tdInfo->bCellElem);

            					// k=0; currflag=0;
            					// if(cellRgnRect!=NULL)
            					// {
            					// 	for(k=0;k<DIMENSION;k++)
            					// 	{
	            				// 		if(cellRgnRect->iBottomLeft[k] != currCellRgn->iBottomLeft[k] || cellRgnRect->iTopRight[k] != currCellRgn->iTopRight[k])
	            				// 	 	{
	            				// 	 		currflag=1;
	            				// 	 		break;
	            				// 	 	}

            					// 	}

            					// }
            					// else
            					// {
            					// 	currflag=1;
            					// }            					

            					// if (currflag==1)
            					// {
            						uiRecCnt++;
            						insertBCellIntoBCellList(lstNdTemp->tnInfo->tdInfo->bCellElem,cellsList);
            					// }            					

            				}

            				free(currCellRgn->iBottomLeft);
        					free(currCellRgn->iTopRight);
        					free(currCellRgn);
                                   
                            //printf("\nuiRecCnt = %d", uiRecCnt);
                               
	                        break;

		}//switch
		lstNdTemp = lstNdTemp->ptrNextNd;
	}  
	return uiRecCnt;

}

unsigned int GfindOverlappingCells3(GHdrNd hdrNdTree, Region rgnRect, Region cellRgnRect, BCellListHd cellsList,BCellListHd cellsList2)
{
	// recursively adds the ovelapping cells to the cellsList

	if(GisLstEmpty(hdrNdTree) || rgnRect == NULL || rgnRect->iBottomLeft == NULL || rgnRect->iTopRight == NULL || cellRgnRect == NULL || cellRgnRect->iBottomLeft == NULL || cellRgnRect->iTopRight == NULL)
		return;

	Region currCellRgn;
	unsigned int uiRecCnt = 0;
	int iCnt = 0;
    double t;
    static int flag = 0;
    int k = 0, currflag=0;
	GLstNd lstNdTemp = hdrNdTree->ptrFirstNd;
	// int nodesVisited;
	while(lstNdTemp != NULL)
    {	
    	//nodesVisited++;
		switch(lstNdTemp->tnInfo->ndType)
        {   case INTNODE:   //incase of internal node if the node is over lapping with search rectangle descend into the node and add to the count the number of new records found if any
				            if(GisOverLap2(lstNdTemp->tnInfo->tdInfo->rgnRect, rgnRect)==TRUE)
                                   uiRecCnt += GfindOverlappingCells3(lstNdTemp->ptrChildLst, rgnRect, cellRgnRect, cellsList,cellsList2);
                            //printf("\nuiRecCnt = %d", uiRecCnt);
                            break;
            case EXTNODE:  
            				currCellRgn = createRegionofCell(lstNdTemp->tnInfo->tdInfo->bCellElem);
            				if(GisOverLap2(currCellRgn, rgnRect)==TRUE)
            				{
            					// Region currCellRgn = createRegionofCell(lstNdTemp->tnInfo->tdInfo->bCellElem);

            					// k=0; currflag=0;
            					// if(cellRgnRect!=NULL)
            					// {
            					// 	for(k=0;k<DIMENSION;k++)
            					// 	{
	            				// 		if(cellRgnRect->iBottomLeft[k] != currCellRgn->iBottomLeft[k] || cellRgnRect->iTopRight[k] != currCellRgn->iTopRight[k])
	            				// 	 	{
	            				// 	 		currflag=1;
	            				// 	 		break;
	            				// 	 	}

            					// 	}

            					// }
            					// else
            					// {
            					// 	currflag=1;
            					// }            					

            					// if (currflag==1)
            					// {
            						uiRecCnt++;
            						insertBCellIntoBCellList(lstNdTemp->tnInfo->tdInfo->bCellElem,cellsList);
            					// }            					

            				}

            				if(GisOverLap2(currCellRgn, cellRgnRect)==TRUE)
            				{
            					// Region currCellRgn = createRegionofCell(lstNdTemp->tnInfo->tdInfo->bCellElem);

            					// k=0; currflag=0;
            					// if(cellRgnRect!=NULL)
            					// {
            					// 	for(k=0;k<DIMENSION;k++)
            					// 	{
	            				// 		if(cellRgnRect->iBottomLeft[k] != currCellRgn->iBottomLeft[k] || cellRgnRect->iTopRight[k] != currCellRgn->iTopRight[k])
	            				// 	 	{
	            				// 	 		currflag=1;
	            				// 	 		break;
	            				// 	 	}

            					// 	}

            					// }
            					// else
            					// {
            					// 	currflag=1;
            					// }            					

            					// if (currflag==1)
            					// {
            						uiRecCnt++;
            						insertBCellIntoBCellList(lstNdTemp->tnInfo->tdInfo->bCellElem,cellsList2);
            					// }            					

            				}

            				free(currCellRgn->iBottomLeft);
        					free(currCellRgn->iTopRight);
        					free(currCellRgn);
                                   
                            //printf("\nuiRecCnt = %d", uiRecCnt);
                               
	                        break;

		}//switch
		lstNdTemp = lstNdTemp->ptrNextNd;
	}  
	return uiRecCnt;

}


GHdrNd populateAuxGridRTree(BCellListHd cellsList, int gMinEntries, int gMaxEntries)
{
	int i = 0;
	BCellListNode currBCellListNode = cellsList->first;
	BCell currBCell;

	GHdrNd hdrNdTree = GinitHdrNd();
	hdrNdTree->ptrFirstNd = GinitLstNd(GinitIntNd(NULL, NULL));
	hdrNdTree->uiCnt++;
	hdrNdTree->ptrFirstNd->ptrChildLst = GinitHdrNd();

	for(i=0;i<cellsList->count;i++)
	{
		currBCell = currBCellListNode->bCellElem;
		hdrNdTree = insertBCellIntoRTree(hdrNdTree,currBCell,gMinEntries,gMaxEntries);
		currBCellListNode = currBCellListNode->next;
	}
	return hdrNdTree;

}

GHdrNd populateGridRTree(DataHdr dataHdrLst, BCellListHd cellsList, int gMinEntries, int gMaxEntries)
{
	// for loop to read every data point, and every data point
	// find the coords of the BCell
	// check whether the Bcell exists in the R Tree
	// if yes, then insert this data point in that BCell
	// else create a new B Cell, insert this datapoint in it
	// and insert that BCell into R Tree.


	// first create an GR Tree
	GHdrNd hdrNdTree = GinitHdrNd();
	hdrNdTree->ptrFirstNd = GinitLstNd(GinitIntNd(NULL, NULL));
	hdrNdTree->uiCnt++;
	hdrNdTree->ptrFirstNd->ptrChildLst = GinitHdrNd();

	// for loop to process every point
	int cnt = 0,i;
    //retirve element one by one and insert them into tree invoking create root incase of root split
	for(i=0;i<dataHdrLst->uiCnt;i++)
	{
		// construct the region with the help of the point
		Data currDataPoint;
		currDataPoint= dataHdrLst->dataClstElem + i;

		Region currRegion = createCellRegOfPoint(currDataPoint);
		// print currRegion
		//printRegion(currRegion);

		BCell currBCell = GfindCell(hdrNdTree,currRegion);

		if(currBCell!=NULL){
			// insert this point int he cell
			//printf("Inserting the point into an existing BCell\n");
			insertPointintoBCell(currBCell,currDataPoint);
			currDataPoint->cellId=currBCell->id;
		}
		else
		{
			// create a BCell and insert it into the R Tree
			//printf("Creating a new BCell and inserting the new point into it\n");
			BCell newBCell = initBCell(currRegion);
			insertPointintoBCell(newBCell,currDataPoint);
			currDataPoint->cellId=newBCell->id;
			hdrNdTree = insertBCellIntoRTree(hdrNdTree,newBCell,gMinEntries,gMaxEntries);
			
			insertBCellIntoBCellList(newBCell,cellsList);


		}

		free(currRegion->iBottomLeft);
   		free(currRegion->iTopRight);
    	free(currRegion);


		// call findCell to check if the region lies in the R Tree
		// the region will match with the region of exactly one BCell or the ext node.
		// if yes it will return a BCell.
		// insert the data point in the BCell


		// if it returns null
		// then create a BCell and call insertBCellIntoRTree()


		
		// insertTree(hdrNdTree, initExtNd((dataHdrLst->dataClstElem)+i));
		// if(hdrNdTree->uiCnt > 1)
		// 	hdrNdTree = createRoot(hdrNdTree);
	}

	return hdrNdTree;


}

void insertPointintoBCell(BCell currBCell, Data currDataPoint)
{

	insertPointintoCellDataList(currBCell->cellDataList, currDataPoint);

	int x,i;
	if(currBCell->cellDataList->count == 1)
	{
		for(x = 0; x < DIMENSION; x++)
		{
			currBCell->minActualBoundary[x] = currDataPoint->iData[x];
			currBCell->maxActualBoundary[x] = currDataPoint->iData[x];

		}		
	}
	else
	{
		for(i = 0; i< DIMENSION; i++)
		{
			if(currBCell->minActualBoundary[i] > currDataPoint->iData[i])
			{
				currBCell->minActualBoundary[i] = currDataPoint->iData[i];
			}
			if(currBCell->maxActualBoundary[i] < currDataPoint->iData[i])
			{
				currBCell->maxActualBoundary[i] = currDataPoint->iData[i];
			}
		}

	}

	return;
	
}


void insertBCellIntoBCellList(BCell newBCell,BCellListHd cellsList)
{
	BCellListNode newBCellListNode = (BCellListNode) malloc(sizeof(*newBCellListNode));
	newBCellListNode->bCellElem = newBCell;
	newBCellListNode->next = cellsList->first;
	cellsList->first = newBCellListNode;
	cellsList->count++;

	return;

}

void insertPointintoCellDataList(CellDataHd currCellDataList, Data currDataPoint)
{
	CellData currCellData = initCellData(currDataPoint);
	
	//printCellDataList(currCellDataList);

	if(currCellDataList->first==NULL)
	{
		currCellDataList->first = currCellData;
		currCellDataList->count++;

	}
	else
	{
		currCellData->next = currCellDataList->first;
		currCellDataList->first = currCellData;
		currCellDataList->count++;

	}

	return;

}

Region createCellRegOfPoint(Data currDataPoint)
{

	Region tempRegion = (Region) malloc(sizeof(struct region));
	assert(tempRegion!=NULL);

	tempRegion->iBottomLeft = (Dimension)malloc(DIMENSION * sizeof(double));
	assert(tempRegion->iBottomLeft!=NULL);

	tempRegion->iTopRight = (Dimension)malloc(DIMENSION * sizeof(double));
	assert(tempRegion->iTopRight!=NULL);

	int i = 0;
	double temp;
	//double edgeLength = EPS/sqrt((double)DIMENSION);
	//double edgeLength = 10;

	for(i=0;i<DIMENSION;i++)
	{
		temp = floor((currDataPoint->iData[i] - MINGRIDSIZE[i]) / CELLSIZE);		
		tempRegion->iBottomLeft[i] = MINGRIDSIZE[i] + temp * CELLSIZE;
		tempRegion->iTopRight[i] = MINGRIDSIZE[i] + (temp + 1) * CELLSIZE;
	}

	return tempRegion;
}

Region createRegionofCell(BCell bCellElem)
{
	Region tempRegion = (Region) malloc(sizeof(struct region));
	assert(tempRegion!=NULL);

	tempRegion->iBottomLeft = (Dimension)malloc(DIMENSION * sizeof(double));
	assert(tempRegion->iBottomLeft!=NULL);

	tempRegion->iTopRight = (Dimension)malloc(DIMENSION * sizeof(double));
	assert(tempRegion->iTopRight!=NULL);

	int iCnt;
	for(iCnt = 0; iCnt < DIMENSION; iCnt++)
    {   
    	tempRegion->iBottomLeft[iCnt] = bCellElem->minOriginalBoundary[iCnt];
		tempRegion->iTopRight[iCnt] = bCellElem->maxOriginalBoundary[iCnt];
	}

	return tempRegion;
}


GHdrNd insertBCellIntoRTree(GHdrNd hdrNdTree, BCell BCellNode, int gMinEntries, int gMaxEntries)
{

	GinsertTree(hdrNdTree, GinitExtNd(BCellNode), gMinEntries, gMaxEntries);
		
	// we are doing this because the first node of the R Tree can' t have more than 1 element
	if(hdrNdTree->uiCnt > 1)
		hdrNdTree = GcreateRoot(hdrNdTree);

	return hdrNdTree;

}


void printMinGridSize()
{
	int i = 0;
	printf("MINGRIDSIZE is: ");
	for(i=0;i<DIMENSION;i++)
	{
		printf("%lf ",MINGRIDSIZE[i]);
	}
	printf("\n");

	return;
}

void printMaxGridSize()
{
	int i = 0;
	printf("MAXGRIDSIZE is: ");
	for(i=0;i<DIMENSION;i++)
	{
		printf("%lf ",MAXGRIDSIZE[i]);
	}
	printf("\n");

	return;

}

void printCellsList(BCellListHd cellsList)
{
	int i=0;
	BCellListNode currCellNode = cellsList->first;
	printf("cell count=%d\n",cellsList->count);
	
	if(cellsList->count != 0)
	{
		while(currCellNode!=NULL)
		{
			BCell currBCellElem = currCellNode->bCellElem;
			if(currBCellElem->cellDataList->count!=0)
			{				
				printf("Printing Cell:\n");
				printCell(currBCellElem);
				printf("End of Printing Cell\n");
			}
			else
			{
				printf("The current cell doesn't have any datapoints in it\n");
			}
			
			currCellNode = currCellNode->next;
		}

	}
	else
	{
		printf("The CellList doesn't have any cells populated\n");
	}	
	
	return;

}

void printNoOfCoreCells(BCellListHd cellsList)
{
	int i=0;
	int coreCells = 0;
	BCellListNode currCellNode = cellsList->first;
	printf("cell count=%d\n",cellsList->count);
	if(cellsList->count != 0)
	{
		while(currCellNode!=NULL)
		{
			BCell currBCellElem = currCellNode->bCellElem;
			if((currBCellElem->cellDataList->count) >= MINPOINTS)
			{
				coreCells++;
			}
			currCellNode = currCellNode->next;
		}
	}
	else
	{
		printf("The CellList doesn't have any cells populated\n");
	}
	
	printf("No of core cells = %d\n", coreCells);

	return;
}

void printCell(BCell bCellElem)
{
	int iCnt=0;
	CellData currDataPt = bCellElem->cellDataList->first;

	for(iCnt=0;iCnt<DIMENSION;iCnt++)
	{
	//	printf("%lf ", bCellElem->minOriginalBoundary[iCnt]);
	}
	for(iCnt=0;iCnt<DIMENSION;iCnt++)
	{
	//	printf("%lf ", bCellElem->maxOriginalBoundary[iCnt]);		
	}
	//printf("\n");
	
	while(currDataPt!=NULL)
	{
		printf("%d ", currDataPt->data->iNum);
		for(iCnt=0;iCnt<DIMENSION;iCnt++)
		{
			//printf("%lf ", currDataPt->data->iData[iCnt]);
		}
		//printf("\n");
		currDataPt = currDataPt->next;
	}

	return;

}

void printCellDataList(CellDataHd currCellDataList)
{
	CellData currCellData = currCellDataList->first;
	//printf("Printing the CellDataList:\n");
	printf("\n");
	while(currCellData!=NULL)
	{
		//printCellData(currCellData);
		printf("%d ",currCellData->data->iNum);
		currCellData=currCellData->next;
	}
	//printf("End of Printing CellDataList.\n");

	return;
	
}

void printCellData(CellData currCellData)
{
	int i = 0;
	printf("Printing Cell Data: ");
	printf("%d ",currCellData->data->iNum);

	for(i=0;i<DIMENSION;i++)
	{
		//printf("%lf ",currCellData->data->iData[i]);
	}
	printf("\n");
	return;

}

// RHdrNd getAuxRTreeofNeighbors(BCell bCellElem, GHdrNd hdrNdTree)
// {
// 	Region cellRgnRect = createRegionofCell(bCellElem);
// 	Region rgnRect = getEpsExtendedRegion(cellRgnRect);

// 	//GprintRegion(rgnRect);

// 	BCellListHd cellsList = GgetCellsInRegion(hdrNdTree,rgnRect,cellRgnRect);	

// 	RHdrNd auxRTree = RbuildRTreeFromCells(cellsList);

// 	return auxRTree;
	
// }

Region getEpsExtendedRegion(Region cellRgnRect, double eps)
{
	Region tempRegion = (Region) malloc(sizeof(struct region));
	assert(tempRegion!=NULL);

	tempRegion->iBottomLeft = (Dimension)malloc(DIMENSION * sizeof(double));
	assert(tempRegion->iBottomLeft!=NULL);

	tempRegion->iTopRight = (Dimension)malloc(DIMENSION * sizeof(double));
	assert(tempRegion->iTopRight!=NULL);

	int iCnt;
	for(iCnt = 0; iCnt < DIMENSION; iCnt++)
    {   
    	tempRegion->iBottomLeft[iCnt] = cellRgnRect->iBottomLeft[iCnt] - eps;
		tempRegion->iTopRight[iCnt] = cellRgnRect->iTopRight[iCnt] + eps;
	}

	return tempRegion;

}

Region getEpsExtendedRegionPoint(Data dataPoint, double eps)
{
	Region tempRegion = (Region) malloc(sizeof(struct region));
	assert(tempRegion!=NULL);

	tempRegion->iBottomLeft = (Dimension)malloc(DIMENSION * sizeof(double));
	assert(tempRegion->iBottomLeft!=NULL);

	tempRegion->iTopRight = (Dimension)malloc(DIMENSION * sizeof(double));
	assert(tempRegion->iTopRight!=NULL);

	int iCnt;
	for(iCnt = 0; iCnt < DIMENSION; iCnt++)
    {   
    	tempRegion->iBottomLeft[iCnt] = dataPoint->iData[iCnt] - eps;
		tempRegion->iTopRight[iCnt] = dataPoint->iData[iCnt] + eps;
	}

	return tempRegion;

}

void freeGRTree(GHdrNd hdrNdTree)
{

	// code to Free GR Tree	

	if(hdrNdTree == NULL) 
		return;

	if(hdrNdTree->uiCnt == 0)
	{
		free(hdrNdTree);
		return;
	}
	
	GLstNd lstNdTemp = hdrNdTree->ptrFirstNd;
	GLstNd lstNdNextTemp;
	
	if(lstNdTemp!=NULL)
	{
		while(lstNdTemp != NULL)
    	{
			switch(lstNdTemp->tnInfo->ndType)
        	{   case INTNODE:   
				    			free(lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft);
				    			free(lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight);
				    			free(lstNdTemp->tnInfo->tdInfo->rgnRect);
				    			free(lstNdTemp->tnInfo->tdInfo);
				    			free(lstNdTemp->tnInfo);				    			
				    			freeGRTree(lstNdTemp->ptrChildLst);                            	
                            	lstNdNextTemp = lstNdTemp->ptrNextNd;
                            	free(lstNdTemp);
                            	break;
            	case EXTNODE:   					            
					            free(lstNdTemp->tnInfo->tdInfo);
					            free(lstNdTemp->tnInfo);
					            freeGRTree(lstNdTemp->ptrChildLst);
					            lstNdNextTemp = lstNdTemp->ptrNextNd;
                            	free(lstNdTemp);	                            
		                        break;

			}//switch
			
			lstNdTemp = lstNdNextTemp;
		}
	}	
	
	free(hdrNdTree);	
	return;
}

void isCorrectGRTree(GHdrNd hdrNdTree)
{
    printf("Checking correctness\n");
    GLstNd lstNdTemp = hdrNdTree->ptrFirstNd,temp;
    int iCnt = 0,flag;

    while(lstNdTemp != NULL)
        {   
        if(lstNdTemp->tnInfo->ndType == INTNODE)
            {   
            temp=lstNdTemp->ptrChildLst->ptrFirstNd;
            while(temp!=NULL)
            {
                flag=0;
                for(iCnt=0;iCnt<DIMENSION;iCnt++)
                {
                    if(temp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt]<lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt])
                        flag=1;
                }
                for(iCnt=0;iCnt<DIMENSION;iCnt++)
                {
                    if(temp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt]>lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt])
                        flag=1;
                }
                if(flag==1)
                {
                    printf("WRONG!!!\n");
                    for(iCnt=0;iCnt<DIMENSION;iCnt++)
                    {
                        printf("%lf %lf %lf %lf\n",temp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt],temp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt],lstNdTemp->tnInfo->tdInfo->rgnRect->iBottomLeft[iCnt],lstNdTemp->tnInfo->tdInfo->rgnRect->iTopRight[iCnt]);
                    }
                }
                temp=temp->ptrNextNd;
            }
            isCorrectGRTree(lstNdTemp->ptrChildLst);
        }
        lstNdTemp = lstNdTemp->ptrNextNd;
    }
    return;
}