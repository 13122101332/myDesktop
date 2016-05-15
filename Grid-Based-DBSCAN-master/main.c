#include "GRTDBSCAN.h"


int main(int argc, char **argv) {

	//printf("Execution started!\n");


	if (argc < 2) {
#ifdef PRINT
		printf("Input file not entered!\n");
#endif
#ifdef PRINT
		printf("Usage: ./output inputFile EPS MINPOINTS CELLSIZE GMINENTRIES GMAXENTRIES GAUXMINENTRIES GAUXMAXENTRIES RMINENTRIES RMAXENTRIES\n");
		// inputFile eps min_points cell_size g_min_entries g_max_entries g_aux_min_entries g_aux_max_entries r_min_entries r_max_entries
#endif
		exit(-1);
	}

	// Check if input file exists
	struct stat st;		// file information
	int fileExists = stat(argv[1], &st);

	if (fileExists != 0) {
#ifdef PRINT
		printf("Input file doesn't exist.\n");
#endif
		exit(-1);
	}
	CLUSTERID = 0;		// cluster_id
	BCELLID = 0;			// bcell_id


	// Initialize parameters

	if (argc != 13) {
#ifdef PRINT
		printf("Using default parameter values\n");
#endif
		EPS = 7;
		MINPOINTS = 3;

		GMINENTRIES = 4;	// g_min_entries
		GMAXENTRIES = 8;	// g_max_entries
		GAUXMINENTRIES = 4;	// g_aux_min_entries
		GAUXMAXENTRIES = 8;	// g_aux_max_entries
		RMINENTRIES = 2;	// r_min_entries
		RMAXENTRIES = 4;	// r_max_entries
		UNDEFINED = 10000000;	// undefined

	}

	else {
#ifdef PRINT
		printf("Not Using default parameter values\n");
#endif
		EPS = atof(argv[10]);
		MINPOINTS = atoi(argv[11]);
		CELLSIZE = atof(argv[9]);
		GMINENTRIES = atoi(argv[3]);
		GMAXENTRIES = atoi(argv[4]);
		GAUXMINENTRIES = atoi(argv[5]);
		GAUXMAXENTRIES = atoi(argv[6]);
		RMINENTRIES = atoi(argv[7]);
		RMAXENTRIES = atoi(argv[8]);
		UNDEFINED = atoi(argv[12]);

	}

#ifdef PRINT
	printf("EPS: %f MINPOINTS: %d CELLSIZE: %f\n", EPS, MINPOINTS, CELLSIZE);
#endif
	// Define variables for storing input data, and for the GRTree
	DataHdr dataList;	// Head Node for the list of data elements to store neighbours

	// Read input data from file
	dataList = readData(argv[1]);
	DIMENSION = 2;	// dimension
	CELLSIZE = EPS / (2 * sqrt(DIMENSION));	// eps cell_size

	// Initialize a list of cells
	BCellListHd cellList = initBCellListHd();	// cell_list_head


	// Construct GRTree
	GHdrNd GRTree = populateGridRTree(dataList, cellList, GMINENTRIES, GMAXENTRIES);
	constructAuxRTrees(dataList, cellList);

	totalPoints = readPoints(argv[1]);

	cluster = (Cluster)malloc((cellList->count + totalPoints + 1) * sizeof(struct cluster));

	int v;
	for (v = 0; v < cellList->count + totalPoints + 1; v++)
	{
		cluster[v].parent = v;
		cluster[v].rank = 0;
	}

	determineCellList(GRTree, cellList);
	//printcelltype(cellList);
	processCellList(GRTree, cellList, dataList);
	printCheck(cellList, argv[2]);

	freeDataList(dataList);
	freeCellsList(cellList);
	freeGRTree(GRTree);

	return 0;

}
