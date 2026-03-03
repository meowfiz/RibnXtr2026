#ifndef	GEOMETRY_H
#define	GEOMETRY_H

#include <math.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkGenericCell.h>
#include <vtkFeatureEdges.h>
#include <vtkCleanPolyData.h>
#include <vtkCellArray.h>
#include <vtkPolygon.h>
#include <vtkPointData.h>
#include <vtkLine.h>
#include <vtkTriangle.h>
#include <vtkFloatArray.h>
#include <vtkTriangleFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkPlatonicSolidSource.h>
 
 
#include <QPolygon>

#include "OtherCommonFunctions.h"
#include "LinkedList.h"
#include "OtherCommonFunctionsVTK.h"
#include "PlaneObject.h"

#include <armadillo>

struct mySphereDistTabsElement
{
	int nrOfElements;
	double maxDist;
	int *distTab;
	int *indexTab;
};
void LoadMySphereData(const QString &filename, vtkPolyData *&mySphereData, mySphereDistTabsElement*&mySphereDistanceTab);
void SaveMySphereData(const QString &filename, vtkPolyData *mySphereData, mySphereDistTabsElement*mySphereDistanceTab);

mySphereDistTabsElement *CreateMySphereDistanceTabs(vtkPolyData *mySphereData,double dist=0,int lenMax=0);
struct Point2 {
	double x, y;
};

arma::mat findHomography(Point2 *src, Point2 *dst);
Point2 projTransform(arma::mat H, Point2 src);

class JointPoint
{

public:
	JointPoint()
	{
		isAneurysm = false;
	};

	int		mainJointPoint;
	int		nrOfTubes;
	bool	lr[200];
	int		tubes[200];
	int		flags[200];
	int		neighJoint[200];
	bool isAneurysm;
	
	
	
};

//-------------------------

class BoundaryElement
{
	public:
		BoundaryElement(int Nr=0,int id=-1);
		BoundaryElement(int Nr,int *Tab,int id=-1);

		~BoundaryElement();

		int specialFlag;
		int NumberOfPoints;
		int		*PointIndex;	
		int     *tmpIndex;
		double	*radiusTab;
		int		leftJoint;
		int		rightJoint;


		//tu bede trzymal zapasowe
		int tmpLeftJoint;
		int tmpRightJoint;

		bool flag;
		int idNumber; 
	
};


//--------------------------------------------------
/*
* "Fast and Robust Triangle-Triangle Overlap Test 
*  Using Orientation Predicates"  P. Guigue - O. Devillers
*                                                 
*  Journal of Graphics Tools, 8(1), 2003   
*/

/* some 3D macros */
#define CROSS(dest,v1,v2)                       \
               dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
               dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
               dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
 


#define SUB(dest,v1,v2) dest[0]=v1[0]-v2[0]; \
                        dest[1]=v1[1]-v2[1]; \
                        dest[2]=v1[2]-v2[2]; 


#define SCALAR(dest,alpha,v) dest[0] = alpha * v[0]; \
                             dest[1] = alpha * v[1]; \
                             dest[2] = alpha * v[2];



#define CHECK_MIN_MAX(p1,q1,r1,p2,q2,r2) {\
  SUB(v1,p2,q1)\
  SUB(v2,p1,q1)\
  CROSS(N1,v1,v2)\
  SUB(v1,q2,q1)\
  if (DOT(v1,N1) > 0.0f) return 0;\
  SUB(v1,p2,p1)\
  SUB(v2,r1,p1)\
  CROSS(N1,v1,v2)\
  SUB(v1,r2,p1) \
  if (DOT(v1,N1) > 0.0f) return 0;\
  else return 1; }
/* Permutation in a canonical form of T2's vertices */

#define TRI_TRI_3D(p1,q1,r1,p2,q2,r2,dp2,dq2,dr2) { \
  if (dp2 > 0.0f) { \
     if (dq2 > 0.0f) CHECK_MIN_MAX(p1,r1,q1,r2,p2,q2) \
     else if (dr2 > 0.0f) CHECK_MIN_MAX(p1,r1,q1,q2,r2,p2)\
     else CHECK_MIN_MAX(p1,q1,r1,p2,q2,r2) }\
  else if (dp2 < 0.0f) { \
    if (dq2 < 0.0f) CHECK_MIN_MAX(p1,q1,r1,r2,p2,q2)\
    else if (dr2 < 0.0f) CHECK_MIN_MAX(p1,q1,r1,q2,r2,p2)\
    else CHECK_MIN_MAX(p1,r1,q1,p2,q2,r2)\
  } else { \
    if (dq2 < 0.0f) { \
      if (dr2 >= 0.0f)  CHECK_MIN_MAX(p1,r1,q1,q2,r2,p2)\
      else CHECK_MIN_MAX(p1,q1,r1,p2,q2,r2)\
    } \
    else if (dq2 > 0.0f) { \
      if (dr2 > 0.0f) CHECK_MIN_MAX(p1,r1,q1,p2,q2,r2)\
      else  CHECK_MIN_MAX(p1,q1,r1,q2,r2,p2)\
    } \
    else  { \
      if (dr2 > 0.0f) CHECK_MIN_MAX(p1,q1,r1,r2,p2,q2)\
      else if (dr2 < 0.0f) CHECK_MIN_MAX(p1,r1,q1,r2,p2,q2)\
      else return coplanar_tri_tri3d(p1,q1,r1,p2,q2,r2,N1,N2);\
     }}};
#define INTERSECTION_TEST_VERTEX(P1, Q1, R1, P2, Q2, R2) {\
  if (ORIENT_2D(R2,P2,Q1) >= 0.0f)\
    if (ORIENT_2D(R2,Q2,Q1) <= 0.0f)\
      if (ORIENT_2D(P1,P2,Q1) > 0.0f) {\
	if (ORIENT_2D(P1,Q2,Q1) <= 0.0f) return 1; \
	else return 0;} else {\
	if (ORIENT_2D(P1,P2,R1) >= 0.0f)\
	  if (ORIENT_2D(Q1,R1,P2) >= 0.0f) return 1; \
	  else return 0;\
	else return 0;}\
    else \
      if (ORIENT_2D(P1,Q2,Q1) <= 0.0f)\
	if (ORIENT_2D(R2,Q2,R1) <= 0.0f)\
	  if (ORIENT_2D(Q1,R1,Q2) >= 0.0f) return 1; \
	  else return 0;\
	else return 0;\
      else return 0;\
  else\
    if (ORIENT_2D(R2,P2,R1) >= 0.0f) \
      if (ORIENT_2D(Q1,R1,R2) >= 0.0f)\
	if (ORIENT_2D(P1,P2,R1) >= 0.0f) return 1;\
	else return 0;\
      else \
	if (ORIENT_2D(Q1,R1,Q2) >= 0.0f) {\
	  if (ORIENT_2D(R2,R1,Q2) >= 0.0f) return 1; \
	  else return 0; }\
	else return 0; \
    else  return 0; \
 };



#define INTERSECTION_TEST_EDGE(P1, Q1, R1, P2, Q2, R2) { \
  if (ORIENT_2D(R2,P2,Q1) >= 0.0f) {\
    if (ORIENT_2D(P1,P2,Q1) >= 0.0f) { \
        if (ORIENT_2D(P1,Q1,R2) >= 0.0f) return 1; \
        else return 0;} else { \
      if (ORIENT_2D(Q1,R1,P2) >= 0.0f){ \
	if (ORIENT_2D(R1,P1,P2) >= 0.0f) return 1; else return 0;} \
      else return 0; } \
  } else {\
    if (ORIENT_2D(R2,P2,R1) >= 0.0f) {\
      if (ORIENT_2D(P1,P2,R1) >= 0.0f) {\
	if (ORIENT_2D(P1,R1,R2) >= 0.0f) return 1;  \
	else {\
	  if (ORIENT_2D(Q1,R1,R2) >= 0.0f) return 1; else return 0;}}\
      else  return 0; }\
    else return 0; }}
#define ORIENT_2D(a, b, c)  ((a[0]-c[0])*(b[1]-c[1])-(a[1]-c[1])*(b[0]-c[0]))
#define CONSTRUCT_INTERSECTION(p1,q1,r1,p2,q2,r2) { \
  SUB(v1,q1,p1) \
  SUB(v2,r2,p1) \
  CROSS(N,v1,v2) \
  SUB(v,p2,p1) \
  if (DOT(v,N) > 0.0f) {\
    SUB(v1,r1,p1) \
    CROSS(N,v1,v2) \
    if (DOT(v,N) <= 0.0f) { \
      SUB(v2,q2,p1) \
      CROSS(N,v1,v2) \
      if (DOT(v,N) > 0.0f) { \
	SUB(v1,p1,p2) \
	SUB(v2,p1,r1) \
	alpha = DOT(v1,N2) / DOT(v2,N2); \
	SCALAR(v1,alpha,v2) \
	SUB(source,p1,v1) \
	SUB(v1,p2,p1) \
	SUB(v2,p2,r2) \
	alpha = DOT(v1,N1) / DOT(v2,N1); \
	SCALAR(v1,alpha,v2) \
	SUB(target,p2,v1) \
	return 1; \
      } else { \
	SUB(v1,p2,p1) \
	SUB(v2,p2,q2) \
	alpha = DOT(v1,N1) / DOT(v2,N1); \
	SCALAR(v1,alpha,v2) \
	SUB(source,p2,v1) \
	SUB(v1,p2,p1) \
	SUB(v2,p2,r2) \
	alpha = DOT(v1,N1) / DOT(v2,N1); \
	SCALAR(v1,alpha,v2) \
	SUB(target,p2,v1) \
	return 1; \
      } \
    } else { \
      return 0; \
    } \
  } else { \
    SUB(v2,q2,p1) \
    CROSS(N,v1,v2) \
    if (DOT(v,N) < 0.0f) { \
      return 0; \
    } else { \
      SUB(v1,r1,p1) \
      CROSS(N,v1,v2) \
      if (DOT(v,N) >= 0.0f) { \
	SUB(v1,p1,p2) \
	SUB(v2,p1,r1) \
	alpha = DOT(v1,N2) / DOT(v2,N2); \
	SCALAR(v1,alpha,v2) \
	SUB(source,p1,v1) \
	SUB(v1,p1,p2) \
	SUB(v2,p1,q1) \
	alpha = DOT(v1,N2) / DOT(v2,N2); \
	SCALAR(v1,alpha,v2) \
	SUB(target,p1,v1) \
	return 1; \
      } else { \
	SUB(v1,p2,p1) \
	SUB(v2,p2,q2) \
	alpha = DOT(v1,N1) / DOT(v2,N1); \
	SCALAR(v1,alpha,v2) \
	SUB(source,p2,v1) \
	SUB(v1,p1,p2) \
	SUB(v2,p1,q1) \
	alpha = DOT(v1,N2) / DOT(v2,N2); \
	SCALAR(v1,alpha,v2) \
	SUB(target,p1,v1) \
	return 1; \
      }}}} 
#define TRI_TRI_INTER_3D(p1,q1,r1,p2,q2,r2,dp2,dq2,dr2) { \
  if (dp2 > 0.0f) { \
     if (dq2 > 0.0f) CONSTRUCT_INTERSECTION(p1,r1,q1,r2,p2,q2) \
     else if (dr2 > 0.0f) CONSTRUCT_INTERSECTION(p1,r1,q1,q2,r2,p2)\
     else CONSTRUCT_INTERSECTION(p1,q1,r1,p2,q2,r2) }\
  else if (dp2 < 0.0f) { \
    if (dq2 < 0.0f) CONSTRUCT_INTERSECTION(p1,q1,r1,r2,p2,q2)\
    else if (dr2 < 0.0f) CONSTRUCT_INTERSECTION(p1,q1,r1,q2,r2,p2)\
    else CONSTRUCT_INTERSECTION(p1,r1,q1,p2,q2,r2)\
  } else { \
    if (dq2 < 0.0f) { \
      if (dr2 >= 0.0f)  CONSTRUCT_INTERSECTION(p1,r1,q1,q2,r2,p2)\
      else CONSTRUCT_INTERSECTION(p1,q1,r1,p2,q2,r2)\
    } \
    else if (dq2 > 0.0f) { \
      if (dr2 > 0.0f) CONSTRUCT_INTERSECTION(p1,r1,q1,p2,q2,r2)\
      else  CONSTRUCT_INTERSECTION(p1,q1,r1,q2,r2,p2)\
    } \
    else  { \
      if (dr2 > 0.0f) CONSTRUCT_INTERSECTION(p1,q1,r1,r2,p2,q2)\
      else if (dr2 < 0.0f) CONSTRUCT_INTERSECTION(p1,r1,q1,r2,p2,q2)\
      else { \
       	*coplanar = 1; \
	return coplanar_tri_tri3d(p1,q1,r1,p2,q2,r2,N1,N2);\
     } \
  }} }
/* function prototype */

int tri_tri_overlap_test_3d(double p1[3], double q1[3], double r1[3], 
			    double p2[3], double q2[3], double r2[3]);


int coplanar_tri_tri3d(double  p1[3], double  q1[3], double  r1[3],
		       double  p2[3], double  q2[3], double  r2[3],
		       double  N1[3], double  N2[3]);


int tri_tri_overlap_test_2d(double p1[2], double q1[2], double r1[2], 
			    double p2[2], double q2[2], double r2[2]);


int tri_tri_intersection_test_3d(double p1[3], double q1[3], double r1[3], 
				 double p2[3], double q2[3], double r2[3],
				 int * coplanar, 
				 double source[3],double target[3]);
int ccw_tri_tri_intersection_2d(double p1[2], double q1[2], double r1[2], 
				double p2[2], double q2[2], double r2[2]);


int fitLine3D_wods( double *points, int count, double *line );
int JacobiEigensf(double *A, double *V, double *E, int n, double eps);
int fitLine3D_wodsFull( double *points, int count, double *line,double  eps, int iter);
void calcDist3D( double * points, int count, double *_line, double *dist );
void weightL12( double *d, int count, double *w );
void weightFair( double *d, int count, double *w, double _c );
bool Inscribe (double v0[3], double  v1[3],    double v2[3], double center[3],double &radius);
struct edge 
{
  int top, bottom; // Indices within the polygon
};
//----------------------------------------------------------------------------
	void normV(double *v);
	void diffV(double *v1, double *v2,double *v=NULL);
	void mulV(double *v1, double *v2,double *v=NULL);
	void addV(double *v1, double *v2,double *v=NULL);


	double getPointDistanceToLine (double *x,double *p1, double *p2);
	int GetNumberOfCommonPointsOf2Tiangles(int p1a,int p1b,int p1c,int p2a,int p2b,int p2c);
	bool TriangleInsideBoundingBox(int p1,int p2,int p3,vtkPoints *Points,double *Bounds);
	void FillTabWithPolygon(QPolygon *Points, int MaskX, int MaskY,unsigned char *Tab);

	vtkPolyData	*GetBoundaryData(vtkPolyData *Input);
	int		*GetLineTab(vtkPolyData *Boundary,int &number);
	bool	GetSortedBoundaryList(LinkedList<BoundaryElement*> *BoundaryList,vtkPolyData *OutputBoundary, vtkPolyData *Input);
	bool	GetSortedBoundaryList2(LinkedList<BoundaryElement*> *BoundaryList, vtkPolyData *Input);
	double	*GetBoundaryBoundingBox(int *Tab,int Nr,vtkPoints *Points);

	double	GetSquareDistanceBetweenTwoPoints(double Point1[3],double Point2[3]);
	double	TriangleArea(vtkPoints *Points,int A,int B,int C);
	double	TriangleArea(double *tabX,int A,int B,int C);
	double	TriangleTetraVolume(double *tabX,int A,int B,int C);
	double	TriangleTetraVolume(vtkPoints *Points,int A,int B,int C);
	double	GetAngleBetweenLines( double *Point0,double *Point1, double *Point2);
	double	GetAngleBetweenNormalizedVectors( double *l1,double *l2);
	void	CalculateMeshCenterOfMass(vtkPolyData *pdata,double *center);

	void	RotateVectorInPlane(double *v,double *v1,double *x,double *y,double angle);

	int		ChceckSideOfPointAndLine(double *p0,double *p1,double *p);

	double getPointDistanceToLine (double *x,double *p1, double *p2);
	int		GetAdjacentTriangles(vtkPolyData *OutputPolys,vtkPolyData *Input, vtkPolyData *Boundary, int *LineTab, int NumberOfPoints);
	int		GetAdjacentTriangles(vtkPolyData *OutputPolys,vtkPolyData *Input, int *LineTab, int NumberOfPoints);
	double	GetDihedralAngle(int *Triangle1,int *Triangle2,vtkPoints *Points1,vtkPoints *Points2=NULL);
	double	GetOrientedDihedralAngle(int *Triangle1,int *Triangle2,vtkPoints *Points1,vtkPoints *Points2=NULL);
	int	IntersectTriangleWithPolyData(int p1,int p2,int p3,vtkPolyData *PolyData);
	int	IntersectTriangleWithPolyData(int p1,int p2,int p3,vtkPoints *Points,vtkCellArray *Cells);

	int	GetAdjacentTriangles(vtkPolyData *AdjacentPolydata,vtkPolyData *Input, vtkPolyData *Boundary,int *LineTab, int NumberOfPoints);
	int	GetAdjacentTriangles(vtkPolyData *AdjacentPolydata,vtkPolyData *Input,int *LineTab, int NumberOfPoints);
	int	*GetForbiddenPointConnectionList(vtkPolyData *InputPolyData,LinkedList<BoundaryElement*> *BoundaryList,int &Number);
	bool	CheckIfTriangleNorForbidden(int X,int Y,int Z, int *List,int Nr);

//	vtkPolyData	*SortedPlanarBoundaryPointsToPolyDataSurface(vtkPoints *Input); // do wywalenia
	vtkPolyData	*AppendPolyData(vtkPolyData *Input1,vtkPolyData *Input2);
	vtkPolyData	*CreateFillHolesPolydataOutput(vtkPolyData *Input,vtkCellArray *Polys,vtkPoints *Points);
	
	// sprawdza, czy orientacja odcinka jest zgodna z orientacja trojkatow w meshu
	bool CheckLineSegmentOrientation(int p1,int p2,vtkPolyData *Data); 
	double	CalculateMeshVolume(vtkPolyData *TempPoly);
	 
	void	swapTwoVectors(double *v1, double *v2);
	void	OrientTwoVectorsUsingStartingClosestPoint(double *p1A,double *p1B,double *p2A,double *p2B, double *l1,double *l2,bool *swap);
	void	SlotGetFittedLineParams1(double *line,vtkPoints *points);

	int LineLineIntersect(
   double p1[3],double p2[3],double p3[3],double p4[3],double *pa,double *pb,
   double *mua, double *mub);
	double CylTest_CapsFirst( double p1[3], double p2[3], float lengthsq, float radius_sq, double testpt[3],double tol=0 );
	
	vtkPolyData	*CreateMySphere(int iter, double radius, double *ctr=NULL);
	bool	GetArc2(double* p0, double* p1, double* p2, double radius, int nrOfPts, double* pts, double* orient, double* orient2, double* rr = NULL, double minAlpha = 0, double maxAlpha = 360, QString path = "");
	bool	GetCircle2(double* p0, double* p1, double* p2, double radius, int nrOfPts, double* pts, double* orient, double* orient2);
	void	GetCircle(double* p0, double* p1, double* p2, double radius, int nrOfPts, double* pts, double* orient);

#endif //   GEOMETRY_H
