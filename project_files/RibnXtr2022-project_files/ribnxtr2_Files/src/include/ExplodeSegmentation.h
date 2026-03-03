#ifndef	BRAIN_PROCESSING_H
#define BRAIN_PROCESSING_H

 
#include <qobject.h>
#include <qapplication.h>
////#include <q3filedialog.h>


#include	<stdio.h>
#include	<math.h>

#include	"SegCriterion.h"
#include	"RawDataSet.h"
#include	"SurfaceData.h"
#include	"OtherCommonFunctions.h"
#include "OtherCommonFunctionsVTK.h"
#include	"Geometry.h"
#include	"SurfaceCollection.h"
#include	"VtkWidget.h"
#include	"SurfaceProcessing.h"
#include	"PlaneObject.h"
#include	"Segmentation.h"

#include	<vtkHedgeHog.h>
#include	<vtkLinearSubdivisionFilter.h>
#include	 <vtkSphereSource.h>
#include	<vtkCellData.h>
#include	<vtkPointData.h>
#include	<vtkCellArray.h>
#include	<vtkCleanPolyData.h>
#include	<vtkSmoothPolyDataFilter.h>
#include	<vtkPolyDataNormals.h>
#include	<vtkTubeFilter.h>
#include	<vtkCardinalSpline.h>
#include	<vtkPolyDataToImageStencil.h>
#include	<vtkImageStencil.h>
#include	<vtkImageStencilData.h>
#include	<vtkPlatonicSolidSource.h>
#include	<vtkTriangleFilter.h>

 

//class RawDataSet;

//--------------------------------

	void	UpdateRayPolyData(vtkPolyData *pData, double *center,RawDataSet *mapData, vtkPoints *nrls);  
	void	UpdateRayPolyData(vtkPolyData *pData, double *center,vtkPolyData *sData);
	int		GetGradValueFromDepthMap(RawDataSet *mapData,int x,int y);//int &val,int thresh);
	int		GetMedValueFromDepthMap(RawDataSet *mapData,int x,int y,int size);
	int		GetNrOfNeighborsBetweenThreshInDepthMap(RawDataSet *mapData,int size,int lth,int rth, int x,int y);
//---------------------------------

class RayMapPolyFilter
{
		public: 
	RayMapPolyFilter()
	{
	

	};

	~RayMapPolyFilter(){};

	virtual void ApplyFiltering(vtkPolyData *pData,RawDataSet *_rayData,RawDataSet *_Ray2DHistData, RawDataSet *_rayMapStatusData)=0;

};
//-------------

class RayMapMaxGradMedianPolyFilter : public RayMapPolyFilter
{
	public:
		RayMapMaxGradMedianPolyFilter(int _maxGrad, int _lth, int _rth, int _sth, int _thMode, double _neigh, float _neighPrecent );
		~RayMapMaxGradMedianPolyFilter(){};
	
		void ApplyFiltering(vtkPolyData *polys,RawDataSet *_rayData,RawDataSet *_Ray2DHistData, RawDataSet *_rayMapStatusData);

		
		int lth,rth;
		float neighPercent;

		//int neighTabSize;
		//int *neighTab;
		int sth;
		int mth;
		int thMode;
		double neighDist;



};
//-----------------------

///----
	class RayMapFilter
{
	public: 
		RayMapFilter()
		{
		

		};

		~RayMapFilter(){};

		virtual void ApplyFiltering(RawDataSet *_rayData,RawDataSet *_Ray2DHistData, RawDataSet *_rayMapStatusData)=0;

	/*RawDataSet *rayData;
	RawDataSet *Ray2DHistData;*/

};

//-----------------------------------------------

class RayMapMaxGradMedianFilter : public RayMapFilter
{
	public:
		RayMapMaxGradMedianFilter(int _maxGrad, int _lth, int _rth, int _sth, int _thMode, int _neighSize, int _neighType, float _neighPrecent );
		~RayMapMaxGradMedianFilter(){};
	
		void ApplyFiltering(RawDataSet *_rayData,RawDataSet *_Ray2DHistData, RawDataSet *_rayMapStatusData);

		
		int lth,rth,neighType;
		float neighPercent;

		//int neighTabSize;
		//int *neighTab;
		int sth;
		int mth;
		int thMode;
		int neighSize;



};

class ExplodeSegmentation : public QObject
{

	Q_OBJECT

	public:


	

	ExplodeSegmentation(VtkWidget *v=NULL,RibonTimer *rTimer=NULL);
  	~ExplodeSegmentation();
	
	VtkWidget			*vWidget;
	RibonTimer *timer;
	//RawDataSet *tData,*tData2,*tDataA,*tData2A;

	vtkPolyData *ColorizeData(vtkPolyData *pData,int *tab);


	
	private:

	vtkPolyData *testData;

	int resolution;

	void AddMaskFromStencilBuffer(RawDataSet *data, unsigned char *mask, vtkPolyData *pData, int closingSize,unsigned char *mask0,int addDilate=0);
	double *GetIslandList(int &nrOfCenters,RawDataSet *data, unsigned char *mask0, unsigned char *mask,int minsize);

	void	FindFirstGuessCenterAlongNormals(RawDataSet *data,int lth,int rth,vtkPoints *pts,vtkPoints *normals,double percent, double *center);
	
	void	ShootAlongNormals(RawDataSet *data,int lth,int rth,vtkPoints *pts,vtkPoints *normals,double percent, double *center=NULL);
	void	MedianAgainstCenter(vtkPolyData *pData,int iter);

	bool testFlag;


	//--------------nowe podejscie

	void	UpdateTopPoints(vtkPolyData *pData);

	void RayShoot(	RawDataSet *data, double *center, vtkPolyData *polys,double minsize,double maxRay, double *shootArea, 
					RawDataSet *rayMapData, RawDataSet *rayMapStatusData,bool getNewRayStatusMap, int resolution,SegCriterion *cr,RayMapFilter *rf);
	vtkPolyData *IncreaseRayMapResolution(RawDataSet *data, int newRes, double *center, vtkPolyData *polys, double minsize, RawDataSet *rayMapData,RawDataSet *rayMapStatusData,SegCriterion *cr,RayMapFilter *rf);


	//najnowsze podejscie
	void RayShoot(RawDataSet *data,unsigned char *mask, vtkPolyData *polys, double minsize,SegCriterion *cr,RayMapPolyFilter *rf,int nrPtsOld,unsigned char *mask0=NULL);

	void RayShootSimple(RawDataSet *data, unsigned char *mask, vtkPolyData *polys, double minsize, SegCriterion *cr, int nrPtsOld, double maxDist, int *center = NULL, RawDataSet *shootCentralMaskData = NULL, mySphereDistTabsElement *neighTab = NULL, double *exportParams=NULL);

	public slots:

	void	SlotScanlineSegmentBrain(RawDataSet *Data1,unsigned char *Data2,int x,int y,int z,int lthresh, int hthresh,int iter,int minsize);
	void	ExplodeSegmentation3D(RawDataSet *data, unsigned char *mask, int center[3], int thresh[2], bool invThresh, bool mode, double *probMap, double percentage, double minPerc, double factorPerc, double maxDist, int sphereLevel);
	void	ExplodeSegmentation3D(vtkPolyData *sourcePolys, PolygonSurfaceData *polysForUpdate, RawDataSet *data, unsigned char *mask, int center[3], int radius[3], RawDataSet *shootEllipoidData, int thresh[2],
		bool invThresh, bool mode, double *probMap, double percentage, double minPerc, double factorPerc, double maxDist, int sphereLevel, SegCriterion *cr , mySphereDistTabsElement *neighTab = NULL, double *exportParams = NULL);
	//data - dane oryginalne
	//mask - maskuje segmentacje
	//center - start
	//thresh - dla prostego segm - progi
	//invThresh - czy zrobic inversje progow
	//mode 0-thresh 1-probmap
	//percentage, minperc factorperc - potrzebne do sprawdzenia z probmapy
	//spherelevel - wielkosc sfery do strzelania - docelowo - stablicowana w pliku z sasiedztwem 

	signals:

	void SendMessage(const QString &Message);
	void SignalSendProgress(int Progress);
	void	SignalSendNewSurface(PolygonSurfaceData *_SurfaceData);
	void	SignalSendNewRawDataSet(RawDataSet*);
	void	SignalSendMask(const unsigned char *);

};	 	
			
#endif //   BRAIN_PROCESSING_H
