#ifndef __DEF_H
#define __DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <sys/time.h>
#include <sys/stat.h>
#include <assert.h>
#include <math.h>


#define DELIM " " //Delimiter in the input file
#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) ((x-1) / 2)
//#define PRINT 1 //Uncomment this line if want to print anything
#define ASSERT(x) assert(x!=NULL)

double EPS;
double EPS1;
int MINPOINTS;
int UNDEFINED;
int DIMENSION;    //value for the number of dimensions
int GMINENTRIES;  
int GMAXENTRIES;
int GAUXMINENTRIES;
int GAUXMAXENTRIES; 
int RMINENTRIES;        //Minimum entries for node in RTree
int RMAXENTRIES;       //Maximum entries for node in RTree
double CELLSIZE;
int TIMESTAMP;
int CHOICE;
int CLUSTERID;
int IMMCOUNT;
int EPSCOUNT;
int BCELLID;
double * MINGRIDSIZE;
double * MAXGRIDSIZE;


typedef struct data *Data;  // Structure holding the data of each element
//typedef struct dataNd *DataNd;  // Node in the list of data elements
typedef struct dataHdr *DataHdr;// Head Node for the list of data elements
                                //to store neighbours
typedef double *DataPoint;    // type of pointer to data point
typedef double dataPoint;   // type of data point
typedef struct region *Region;  // type of pointer to structure for rectangle

typedef struct cluster *Cluster; // datatype for pointer to structure for cluster

typedef double *Dimension;  //type of pointer to one corner of rectangle
typedef double dimension; //type of corner of rectangle
typedef double *Double; //type of pointer to double
typedef int RecNum;

typedef struct RhdrNd *RHdrNd;  // Head Node for a list of children of RTree
typedef struct RlstNd *RLstNd;

typedef union RtreeData *RTreeData; // type of pointer to Data in tree node
typedef struct RtreeNode *RTreeNode;

typedef struct RnbHdr *RNbHdr;
typedef struct Rnb *RNB;

typedef struct bcell* BCell;
typedef struct celldatahd* CellDataHd;
typedef struct celldata* CellData;

typedef struct bcelllistnode* BCellListNode;
typedef struct bcelllisthd* BCellListHd;

typedef struct Kdtree * KdTree;

typedef struct KNNdistnode * KNNDistNode;


typedef struct GhdrNd *GHdrNd;  // Head Node for a list of children of RTree
typedef struct GlstNd *GLstNd;  // Nodes in the list of children of RTree node

typedef union GtreeData *GTreeData; // type of pointer to Data in tree node
typedef struct GtreeNode *GTreeNode;  // type of pointer to tree node

// common / generic definitions

typedef enum{
  INTNODE,
  EXTNODE
}NODETYPE;  // used to mark a node of RTree as internal or external


typedef enum{
  FALSE,      // 
  TRUE      // 1
} Boolean;  


typedef enum{ NONE,CORE, DENSE, SPARSE, NOISE } CELLTYPE;

typedef enum{ NONEPOINT,COREPOINT, BORDERPOINT, NOISEPOINT} POINTTYPE;


struct data{
  //int timestamp;
  DataPoint iData;  // data point
  //double core_distance;
  //double reachability_distance;
  RecNum iNum;
  //Boolean isProcessed;  
  Boolean core_tag;   //#
  RNbHdr neighbors;
  int cellId;
  Boolean isProcessed;
  
};


struct dataHdr{     //header for data list
  unsigned int uiCnt;
  Data dataClstElem;
};


struct region{
  Dimension iBottomLeft;  // bottom left corner of rectangle
  Dimension iTopRight;  // top right corner of rectanngle
};


struct cluster{
  int parent;
  int rank;
};


// defs for cell and grid

// defining bigger cell
struct bcell{
  double * minOriginalBoundary;
  double * maxOriginalBoundary;
  double * minActualBoundary;
  double * maxActualBoundary;
  CellDataHd cellDataList;
  RHdrNd auxCellRTree;
  GHdrNd auxCellGRTree;
  Boolean isDenseCell;
  BCellListHd auxCellsList;
  int id;
};


struct celldatahd{
  int count;
  CellData first;
  Boolean coreTag; // jagat
  Boolean isDetermined;
  Boolean isProcessed;
  Boolean flag;
  Boolean hasCorePoint;
  CELLTYPE cellType;
};


struct celldata{
  Data data;
  POINTTYPE pointType;
   
  int nbhFlag;
  CellData next;
};


struct bcelllisthd
{
  int count;
  BCellListNode first;
};


struct bcelllistnode
{
  BCell bCellElem;
  BCellListNode next;
};


// defs for GridRTree



union GtreeData{
    Region rgnRect;   // pointer to rectangle incase of internal node
  BCell bCellElem;  //pointer to a Data in case of external node
};

struct GtreeNode{
    NODETYPE ndType;  //type of tree node (internal or external)
  GTreeData tdInfo;  //pointer to treedata structure
};

struct GhdrNd{     //node in data list
  unsigned int uiCnt; // Number of elements in the list
  GLstNd ptrFirstNd; // First node of the list
  GLstNd ptrParentNd;  // Parent node of the list
};

struct GlstNd{     //node of child list
  GTreeNode tnInfo;  // Data
  GHdrNd ptrChildLst;  // List of child nodes
  GLstNd ptrNextNd;  // Next node in the list
};



// the defs for R Tree



struct Rnb{
       int n;
       double dist;
       RNB nbNext;
};

struct RnbHdr{
       int nbhCnt;
       RNB nbFirst;
       RNB nbLast;
};

union RtreeData{
    Region rgnRect;   // pointer to rectangle incase of internal node
  Data dataClstElem;  //pointer to a Data in case of external node
};

struct RtreeNode{
    NODETYPE ndType;  //type of tree node (internal or external)
  RTreeData tdInfo;  //pointer to treedata structure
};

struct RhdrNd{     //node in data list
  unsigned int uiCnt; // Number of elements in the list
  RLstNd ptrFirstNd; // First node of the list
  RLstNd ptrParentNd;  // Parent node of the list
};

struct RlstNd{     //node of child list
  RTreeNode tnInfo;  // Data
  RHdrNd ptrChildLst;  // List of child nodes
  RLstNd ptrNextNd;  // Next node in the list
};

// the defs for kdTree

struct Kdtree{
  Data KdNodeList;

  //double x[MAX_DIM];
  //struct kd_node_t *left, *right;
};

struct KNNdistnode{
  Data data;
  double distance;
};

/**
* Debugging macro .
*
* Checks for a NULL pointer, and prints the error message, source file and
* line via 'stderr' .
* If the check fails the program exits with error code (-1) .
*/
#define NP_CHECK(ptr) \
    { \
        if (NULL == (ptr)) { \
            fprintf(stderr, "%s:%d NULL POINTER: %s n", \
                __FILE__, __LINE__, #ptr); \
            exit(-1); \
        } \
    } \

#define DEBUG(msg) fprintf(stderr, "%s:%d %s", __FILE__, __LINE__, (msg))

/**
* Priority Queue Structure
*/
typedef enum {GNODE,RNODE,DATA} PQNODETYPE;

// typedef struct pqIntNode *PQIntNode;
typedef struct pqElement *PQElement;
typedef union pqElementUnion *PQElementUnion;

// struct pqIntNode
// {
//     RhdrNd nodePtr;
//     Region mbr;
// };

union pqElementUnion
{
    GHdrNd gNodePtr;
    RHdrNd rNodePtr;
    Data dataElem;
};

struct pqElement
{
    double distance;   
    PQNODETYPE nodeType;
    PQElementUnion element;
};


typedef struct PQueue_s {
    /* The actual size of heap at a certain time */
    size_t size;
    /* The amount of allocated memory for the heap */
    size_t capacity;
    /* An array of (void*), the actual max-heap */
    void **data;
    /* A pointer to a comparator function, used to prioritize elements */
    int (*cmp)(PQElement d1, PQElement d2);
} PQueue;

/** Allocates memory for a new Priority Queue .
Needs a pointer to a comparator function, thus establishing priorities .
*/
// PQueue *pqueue_new(int (*cmp)(PQElement d1,  PQElement d2),
//                    size_t capacity);

// /** De-allocates memory for a given Priority Queue */
// void pqueue_delete(PQueue *q);

// * Add an element inside the Priority Queue 
// void pqueue_enqueue(PQueue *q, const void *PQElement);

// /** Removes the element with the greatest priority from within the Queue */
// void *pqueue_dequeue(PQueue *q); 

int totalPoints;
Cluster cluster;
#endif
