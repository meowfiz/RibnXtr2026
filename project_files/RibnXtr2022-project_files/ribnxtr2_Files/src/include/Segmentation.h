#ifndef	SEGMENTATION_H
#define SEGMENTATION_H

 
#include <qobject.h>
#include <qapplication.h>
////#include <q3filedialog.h>
//Added by qt3to4:
#include <QPolygon>
#include <QTextStream>
#include <qvector3d.h>
#include <qvector.h>
#include	<stdio.h>
#include	<math.h>

#include	"RawDataSet.h"
#include	"SurfaceData.h"
#include	"OtherCommonFunctions.h"
#include "OtherCommonFunctionsVTK.h"
#include	"Geometry.h"
#include	"SurfaceCollection.h"
#include	"VtkWidget.h"
#include	"SurfaceProcessing.h"
#include	"PlaneObject.h"
#include	"DistanceMap.h"
#include	"ExplodeSegmentation.h"

#include	<vtkCubeSource.h>
#include	 <vtkSphereSource.h>
#include	<vtkCellData.h>
#include	<vtkPointData.h>
#include	<vtkCellArray.h>
#include	<vtkCleanPolyData.h>
#include	<vtkSmoothPolyDataFilter.h>
#include	<vtkPolyDataNormals.h>
#include	<vtkTubeFilter.h>
#include	<vtkCardinalSpline.h>
#include	<vtkDelaunay3D.h>
#include	<vtkGeometryFilter.h>
#include	<vtkTransformCoordinateSystems.h>
#include	<vtkSelectEnclosedPoints.h>
#include	<vtkLandmarkTransform.h>
#include	<vtkPlanesIntersection.h>

#include	<omp.h>
//#include <ppl.h>
// using namespace Concurrency;
 
 

//class RawDataSet;
struct TreeJoint
{

	int nrOfChildren;
	int children[50];
};

class JointPts
{
public:
	JointPts(int *sIndTab,int _nrInd,int *sJptsTab,int _nrN)
	{
		nrInd=_nrInd;
		nrN=_nrN;
		
		if (sIndTab!=NULL)
		{
			indices=new int[nrInd];
			memcpy(indices,sIndTab,nrInd*sizeof(int));
		}

		if (sJptsTab!=NULL)
		{
			jPts=new int[nrN];
			memcpy(jPts,sJptsTab,nrN*sizeof(int));

		}

	};
	~JointPts()
	{

		if (indices!=NULL) delete []indices;
		if (jPts!=NULL) delete []jPts;
	};
	int nrInd;
	int nrN;
	int *indices;
	int *jPts;
};
//--------------------------------
	class SegmentationCriterion
	{

		public:

		
			SegmentationCriterion()
			{
				SeedValue=0;
				Tolerance=0;
				MaskValue=1;
				T1=0;
				T2=0;

				data1=NULL;
				mask1=NULL;

				data2=NULL;
				mask2=NULL;
				CheckPointX=&SegmentationCriterion::CheckPoint;
			};
			virtual ~SegmentationCriterion(){};

			virtual bool CheckValue(int pos)=0;

			bool (SegmentationCriterion::*CheckPointX)(int);
			
		//	bool CheckPointX(int pos);
///////////////////////////////////////////
			bool CheckPoint(int pos)
			{
				return ((mask1[pos]!=MaskValue)&&(CheckValue(pos)));
			}
////////////////////////////////////
			bool CheckPointSecondMask(int pos)
			{
				return (CheckPoint(pos)&&(mask2[pos]==MaskValue));
			}
///////////////////////////////////
			bool CheckPointSecondData(int pos)
			{
				//return (CheckPoint(pos));
				return (CheckPoint(pos)&&((data2[pos]>=T1)&&(data2[pos]<=T2)));
			}
///////////////////////////////////
		
		
///////////////////////////////////
			virtual void SetSeedValue(unsigned short Val){SeedValue=Val;}
			virtual void SetTolerance(unsigned short Val){Tolerance=Val;}
			virtual void SetMaskValue(unsigned short Val){MaskValue=Val;}


			 void SetSecondDataThresholds(unsigned short _T1,unsigned short _T2){T1=_T1;T2=_T2;}
			virtual void SetData(unsigned short *_data1,unsigned char *_mask1,unsigned short *_data2=NULL,unsigned char *_mask2=NULL)
			{
				data1=_data1;
				mask1=_mask1;

				data2=_data2;
				mask2=_mask2;

			

			}
			int pos;


		protected:

			unsigned short	SeedValue;
			double			Tolerance;
			unsigned short	*data1;
			unsigned short	*data2;
			unsigned char			*mask1;
			unsigned char			*mask2;
			unsigned char			MaskValue;

			unsigned short	T1;
			unsigned short	T2;
			
	};


//----------------------------

	class SegmentationCriterionThreshold : public SegmentationCriterion
	{

		public:
			

			SegmentationCriterionThreshold()
			{
				LeftThreshold=0;RightThreshold=0;
			};
			
			bool CheckValue(int pos){return ((data1[pos]>=LeftThreshold)&&(data1[pos]<=RightThreshold))?true:false;}	
			void SetThresholds(unsigned short Left, unsigned short Right)
			{
				LeftThreshold=Left;
				RightThreshold=Right;
			}

		private:

			unsigned short LeftThreshold;
			unsigned short RightThreshold;


	};


//---------------------------------
//----------------------------
//---------------------------------
//----------------------------

	class SegmentationCriterionValMinSeed : public SegmentationCriterion
	{

		public:
		
			bool CheckValue(int pos)
			{
				return (abs(data1[pos]-SeedValue)<=Tolerance)?true:false;
			}
	

	};

//---------------------------------
//----------------------------
//---------------------------------
//----------------------------


	class SegmentationCriterionValMinAverage : public SegmentationCriterion
	{

		public:

			SegmentationCriterionValMinAverage()
			{
				Sum=0;
				Average=0;
				NumberOfVoxels=0;
			}
		
			bool CheckValue(int pos)
			{
				bool tmp=(fabs((double)data1[pos]-Average)<=Tolerance)?true:false;


				if (tmp==true)
				{
					Sum+=data1[pos];
					NumberOfVoxels++;

					Average=Round((double)Sum/(double)NumberOfVoxels);
				}
				return tmp;
			}



			virtual void SetSeedValue(unsigned short Val)
			{
				SeedValue=Val;
				Sum=Val;
				NumberOfVoxels=1;
				Average=Sum;
			}

			

		private:

			int		Sum;
			double	Average;
			int		NumberOfVoxels;

	};

//---------------------------------
//----------------------------
//---------------------------------
//----------------------------

//----------------------------



	class SegmentationCriterionDistance : public SegmentationCriterion
	{

		public:
			

			SegmentationCriterionDistance(int _width,int _sliceSize,int _dataSize, RawDataSet *_data,unsigned char *mask)
			{
				width=_width;
				sliceSize=_sliceSize;
				dataSize=_dataSize;
				
				data=_data;//nx=_nx;ny=_ny;nz=_nz;
				mask1=mask;
			};
			~SegmentationCriterionDistance(){};
			bool CheckValue(int pos);//{return ((data1[pos]>=LeftThreshold)&&(data1[pos]<=RightThreshold))?true:false;}	
			/*void SetThresholds(unsigned short Left, unsigned short Right)
			{
				LeftThreshold=Left;
				RightThreshold=Right;
			}*/

			int CroossShooter(int pos, double dist);
			int CircularCheck(int pos, double thresh);
			int CheckDiff(int pos);
			unsigned short shootTab[27];
			double shootTabFloat[27];
			double CalculateTabDist(unsigned short *t,double thresh);
			
		private:

			//unsigned short LeftThreshold;
			//unsigned short RightThreshold;
			int width,sliceSize,dataSize;
			int nx,ny,nz;
			RawDataSet *data;
			
			/*int tabk1[8];
			int tabk2[8];
			int tabk3[8];

			int tabc1[8];
			int tabc2[8];
			int tabc3[8];
			int tabc4[8];
			int tabc5[8];
			int tabc6[8];*/



	};

//---------------------------------

class Segmentation : public QObject
{

	Q_OBJECT

	public:


	inline int Position(int x,int y,int z)
	{
		return z*sxy+y*sx+x;
	};


	Segmentation(VtkWidget *v=NULL);
  	~Segmentation();


	void SetTmpSurfaceMode(bool m) { tmpSurfaces = m; }

	bool tmpSurfaces;
	bool testFlag;
	int NRX;
	VtkWidget			*vWidget;
	int ObjectPoint;
	int BackgroundPoint;

	unsigned short *SecondData;
	int ThresholdSec1;
	int ThresholdSec2;
	int GrowingMode;

	unsigned long Sum;
	int Counter;
	double Percent;
	RawDataSet *VoxelData;
	unsigned char *MaskData;
	int globalTubeNr;

	int OldValue;
	unsigned short *MedianBuffer;

	void CheckMaskVolumeColisionAtSliceK(int k,
		unsigned char *modelMask,int *modelN, int modelSS, double *modelOrigin,double *modelScale,
		unsigned short *modelData,int *objectN, int objectSS, double *objectOrigin,double *objectScale,unsigned short *objData,
		unsigned char maskInVal, unsigned char maskOutVal,double voxTol,InterpolationFunction *Interpolate,int &chgs,double matrix[16],int  thl=-1,int thr=-1);
	int	CheckMaskVolumeColision(RawDataSet *data, RawDataSet *frustumData,vtkTransform *tr,int nrOfThreads,unsigned char *mask,double voxTol,int thLeft,int thRight,unsigned char maskInVal,unsigned char maskOutVal);

	void	GenerateSphere(double *center, double radius, double *rgba);

	void	OptimizeTubeMeshSet(vtkPoints *origPoints, LinkedList<JointPoint*>	*jointList,	LinkedList<PolygonSurfaceData *> *pDataList,LinkedList<BoundaryElement*> *polylineList,RawDataSet *data,int dest, double distLimit = 0.5);
	void	OptimizeTubeMeshSet2(vtkPoints *origPoints, LinkedList<JointPoint*>	*jointList, LinkedList<PolygonSurfaceData *> *pDataList, LinkedList<BoundaryElement*> *polylineList, RawDataSet *data, int dest);

	void	GenerateCollisionMaps(int jointNr,RawDataSet **collisionMaps, bool *avoidTab,vtkPoints *origPoints, LinkedList<JointPoint*>	*jointList,	LinkedList<PolygonSurfaceData *> *pDataList,LinkedList<BoundaryElement*> *polylineList, RawDataSet *data,int dest);
	void	GenerateCollisionMaps2(int jointNr,RawDataSet **collisionMaps, bool *avoidTab,vtkPoints *origPoints, LinkedList<JointPoint*>	*jointList,	LinkedList<PolygonSurfaceData *> *pDataList,LinkedList<BoundaryElement*> *polylineList, RawDataSet *data,int dest);
	void	CheckTubeCollisonOtherTubesBasedOnCenterlineAndRadius(int jointNr, int actTubeNr, bool *avoidTab, vtkPoints *origPoints, LinkedList<JointPoint*>	*jointList, LinkedList<PolygonSurfaceData *> *pDataList, LinkedList<BoundaryElement*> *polylineList, RawDataSet *data, int dest, double distLimit, int &left, int &right);

	void	CheckTubeCollisonWithMap(int jointNr,int actTubeNr, RawDataSet **collisionMaps,bool *avoidTab,vtkPoints *origPoints, LinkedList<JointPoint*>	*jointList,	LinkedList<PolygonSurfaceData *> *pDataList,LinkedList<BoundaryElement*> *polylineList, 
		RawDataSet *data,int dest,double distLimit,int &left,int &right, int &loopElement,bool &sh);
	LinkedList<BoundaryElement*>* GetSegmentedSkeleton(vtkPolyData* skeleton, int thresh, double minSize, double minSizeInner, double minSizeLoop, double minDest, RawDataSet* distData = NULL, double rangeBack = 0, bool improveLargeCenterlines = false);
	int  BuildJointTab(int *jointNeighTab, int nr,int maxJointP, LinkedList<BoundaryElement*>	*polylineList);
	int  RemoveTooShortSkeletonParts(double minSize, int mode,bool *delTab,int *jointNeighTab, int nrOfItems,int maxJointP, LinkedList<BoundaryElement*>	*polylineList,vtkPoints *pts, RawDataSet *distData,double rangeBack);

	void	GeneratePolyDataAtJoints(LinkedList<JointPoint*>	*jointList,LinkedList<BoundaryElement*> *polylineList,vtkPoints *pts,int dest);
	LinkedList<JointPoint*>	* GetTubesAndJoints(vtkPoints *pts,vtkPoints *origPoints,LinkedList<BoundaryElement*> *polylineList,RawDataSet *distMap,double rBack,bool smooth,int thresh,double *newParams=NULL);
	void CroossShooter(RawDataSet *data,int pos, int *tab);
	void ApplyScanline(int Threshold,int Neighbour,int Percent);
	int CheckNeighbours(int x,int y,int z,int Threshold,int Size);

	void RecursiveCheck2D(int,int,int);
	double	KernelCheck(RawDataSet *distData,int pos,int *kernel, int kernelSize,int val);
	int		*GetRealDistKernel(int &kernelSize,int size);



	int		PreviousPosition;
//	int		MedianFilter(unsigned short *A,int n);
	int		MedianValueUpdate(unsigned short* Table,unsigned short *buffer,int Length, int Value);
	int		Bisection(unsigned short *tab,int length,unsigned short value);

	RawDataSet *GenerateAdvancingFrontMap(RawDataSet *data,int pos,int thresh1, int thresh2,int *&maxList,int &maxNr,int maxP,int maxDist);
	RawDataSet	*GenerateAdvancingFrontMap(RawDataSet *data,unsigned char *mask,int x,int y,int z,int *&maxList,int &maxNr,int maxP,int maxDist,int *&frontParentTab,int &nrOfNronts, RawDataSet *separateData=NULL,bool generateSurfaces=false);
	RawDataSet	*GenerateAdvancingFrontMap(RawDataSet *data, unsigned char *mask, QVector <QVector3D> seeds, int *&maxList, int &maxNr, int maxP, int maxDist, int *&frontParentTab, int &nrOfNronts, RawDataSet *separateData = NULL, bool generateSurfaces = false);



	void	static SmoothPolyLine(int nrOfPoints,int *indices,vtkPoints *pts,vtkPoints *origPoints, int iter,double factor,double dx, double dy, double dz,RawDataSet *distMap=NULL,double minVal=0);
	void	 CenterizePolyLineUsingVolumeData(int nrOfPoints, int* indices, vtkPoints* pts, vtkPoints* origPoints, int iter, double factor, double dx, double dy, double dz, RawDataSet* distMap = NULL, double minVal = 0);
	vtkPolyData *GetSkeletonData(int nrOfPoints,int *indices,vtkPoints *pts);
	vtkPolyData *GetSkeletonData(vtkPoints *pts);

	vtkPolyData *GetArteryData(int nrOfPoints,int *indices,vtkPoints *pts,RawDataSet *distMap,double rBack,int thresh,bool improve=false);
	vtkPolyData *GetArteryData2(int nrOfPoints,int *indices,vtkPoints *pts,RawDataSet *distMap,double rBack,int thresh,bool improve,int &left,int &right);
	double *GetSeparatedTubesAndRads(int nrPts,int *ptsTab,vtkPoints *pts,RawDataSet *distMap,double rBack,int thresh,double factor,int &left,int &right,bool smoothRads,int iter,double count,double *newParams=NULL);

	int TraceSkeleton(int startQ,int seed,double *listX,double *listY,double *listZ,unsigned short *advTab, RawDataSet *Data1,unsigned char *mask,int *tabPts,int maxPts,int nrMaxValues,bool direction,int *frontParentTab,RawDataSet *SeparateData=NULL,unsigned short avoidValue=0,int **frontVisitedTab=NULL,int *realTab=NULL);	
	void FrontBlocker(int pos, RawDataSet *data,signed short *t);	
	bool GetShellTabs(int size,int type, int &kernelSize, int *outerShell, int*innerShell, RawDataSet *data);
	void FrontBlocker(int pos, RawDataSet *data,signed short *dTab,int *outerShell,int *innerShell,int kernelSize);

	void TraceSkeleton2(QVector <JointPoint3D>* startingPts, vtkPoints* skeletonPts, int initNrOfSkeltPts, vtkCellArray* skeletonLines, unsigned short* advFrontTab, unsigned short* dividedFrontTab, ParentAndAvoidData* parentAndAvoid,
		RawDataSet* distMapData, QVector<QVector3D> seeds2, QVector<int> seedFrontValues, bool generateTracingSkeletons = false, QVector <JointPoint3D>* jointPts = NULL, int cutFromBegining = 0,
		int cutFromBegingingTh = 0, int kernelSkelSearchSize = 3, unsigned char* closePtsTab = NULL, int angleTestDist = 0, double angleTestAngle = 120, QVector <int> * visitedVector = NULL, QVector <int> * visitedCloseVector = NULL,
		double minAllowedSkeleton = 0,bool is2D=false);

	int InsertNewJoint(LinkedList <JointPts*> *jointList,int *sIndTab,int _nrInd,int *sJptsTab,int _nrN,bool cleanInput=false);
	int CheckConnectivity(int &totalMax,int cmax,int *tabPtsPrev,int *isleTabPrev,int cMax2,int *nrPts2,int *isleTabOld,int *isleTabNew,int *tabPtsOld,short *tTab,RawDataSet *tempFrontData,
		unsigned short *ttab,int *listS,int *listPos,int ksize,int *step,int counter,LinkedList <JointPts*> *jointList,int *maxFrontTab,int *breakListTab, int &breakListCount,
		TreeJoint *treeJoints=NULL);
	void	GenerateGlyphData(RawDataSet *data, int cmax, int *tabPts, int col, int glyphCount, vtkLookupTable *lut, double opacity = 1, int representation = 2, int shape = 0, QString groupName = "",QString name="");

	void	GenerateGlyphData(RawDataSet *data, int cmax, int *tabPts, int *isleTab, int glyphCount, vtkLookupTable *lut, double opacity = 1, int representation = 2, int shape=0, QString groupName="");
	//void	GenerateGlyphData(RawDataSet *data,int cmax,int *tabPts, int *isleTab,int glyphCount,vtkLookupTable *lut);
	PolygonSurfaceData*	GenerateGlyphData(RawDataSet *data,int cmax,int *tabPts,int glyphCount,vtkLookupTable *lut,int colNr,vtkPolyData *glyphPolys);
	PolygonSurfaceData	*GenerateGlyphData(RawDataSet *data,int cmax,int *tabPts,int glyphCount,vtkLookupTable *lut1,int colNr);
	int *GetGrowingTab(RawDataSet *data,  short *ttab,int pos,int lth,int rth,int *gKernel, int gKernelSize, int &gTabSize,int clear);
	int *GetMaxList(RawDataSet *distData,RawDataSet *nrFrontData,JointPts *jPts,int *gKernel, int gKernelSize, int &gTabSize);
	int *GetMaxListRDist(RawDataSet *distData,RawDataSet *nrFrontData,JointPts *jPtsX,int *Kernel, int *distTab,int gKernelSize, int &gTabSize);
	//tuby!!!!!!!!!!!!!!!!!!!
	vtkPolyData	*GenerateTube(int *p,double *r, int nrPts,vtkPoints *pts,int lCut,int rCut, int dest, int mult,bool improve,bool caps=false,bool skeleton=false);
	vtkPolyData	*GenerateArteryTube(int nr,LinkedList<BoundaryElement*> *polylineList,vtkPoints *pts,int dest,bool improve=false,bool caps=false,bool skeleton=false);
	vtkPolyData *GenerateFinalTube(vtkPolyData *polyT,vtkPoints *skeletonPoints,int dest,int lCut,int rCut,int nrOfSkeletPts,bool caps=false,bool skeleton=false);
	void	GeneratePolyDataJoints(LinkedList<JointPoint*>	*jointList, LinkedList<PolygonSurfaceData *> *pDataList, LinkedList<BoundaryElement*> *polylineList, vtkPoints *pts,  RawDataSet *data, int dest, bool ignoreCuts = false, unsigned char *jointsRGB = NULL);
	void GenerateSphereWithJointCircles(RawDataSet *data, vtkPoints *pts, LinkedList<PolygonSurfaceData *> *pDataList, LinkedList<BoundaryElement*>	*circleList, LinkedList<BoundaryElement*>	*prevCircleList, LinkedList<BoundaryElement*>	*jointSkeletonList, int *tubeIndexTab, int nrOfTubes, int dest, unsigned char *jointsRGB = NULL,bool inflate=false);
	RawDataSet *SlotTestFieldGeneration(RawDataSet *data,unsigned char *maskData);
	void	SlotGetDeformedSphere(vtkPolyData *sphere, double center[3], RawDataSet *data, unsigned char *maskData);
	void	SlotGetMinMaxSizes(vtkPolyData *spherePolys,double &minS,double &maxS);;
	bool tumorStart;
	bool medianTumor;
	int  medianTumorIter;

	bool smoothSkel;
	int smoothSkelIter;
	int smoothSkelWeight;


	unsigned short *InputArray;
	int sx,sy,sz,sxy;
	double XVoxel,YVoxel, ZVoxel;
	
	int globalNR;
	void	ArteryPolydataFilter(vtkPolyData *skeleton,RawDataSet *distMap,double rBack,int dest,int thresh,double minArteryLength=5.0,bool genSkel=true,bool genTubes=true, bool optimizeTubes=true, bool genJoints=true, bool genOrigTubes=false,unsigned char *veinRGB=NULL,unsigned char *jointRGB=NULL);
	
	int *	JoinTwoPointIndexTabs(int *tab1, int len1,bool side1,int *tab2, int len2,bool side2,bool common,int &count);
	int *	JoinTwoPointIndexTabsAutoSide(int *tab1, int len1,int *tab2, int len2,int &count);
	void	JoinTwoPointIndexTabsAutoSideReplace(int *&tab1, int &len1,int *tab2, int len2);
	void	MovePolyData(vtkPolyData *pData, double *point);

	int LowerThreshold, UpperThreshold, *TPaletteArray;

	//int seed[3];
	//int winSize[3];
	int maximumSeeds;
	int maximumDist;
	int maxPtsInOne; 
	int firstThresh; 
	int lastThresh;
	double startPoint[3];
	int nrOfThreads;
	QVector <int>* aneurysmFrontValues;

	unsigned short* volumeTempData;

	void	BinarySimpleMorphologyKernel(int i,int nss1,int nss,int nxx1,int nxx,int size,unsigned short *dataArray,unsigned char *mask,int nx,int nxny,
			int *kernel,int kernelSize,unsigned char cmode,int dsizemin1);
	void BinarySimpleMorphology2Kernel(int i,int nss1,int nss,int nxx1,int nxx,int size,
			bool threshMode,unsigned short *dataArray,unsigned char *mask,int nx,int nxny,int lthresh, int rthresh,
	int *kernel,int kernelSize,unsigned char cmode,bool readFromVol,int dsizemin1,unsigned short cmode2,unsigned char cmodeinv);
	//InterpolationFunction *Interpolate;
	public slots:


	//vtkPolyData *GenerateSmoothedSurface(unsigned short thresh, RawDataSet *data,unsigned char *mdata=NULL,bool decim=true);
	void	SlotSetArterySemgentationParameters( int _maximumDist, int _firstThresh, int _lastThresh,bool _tumorStart,bool _medianTumor,int _medianTumorIter,bool _smoothSkel,int _smoothSkelIter, int _smoothSkelWeight,
		QVector <int>* _neurysmFrontValues = NULL, unsigned short* _volumeTempData = NULL);
	
	
	void	SlotScanLineSegmentation(bool directions[3], RawDataSet *_Data1, unsigned char *_Data2, SegCriterion *cr);

	void	SlotScanLineSegmentation(bool CheckX,bool CheckY,bool CheckZ,RawDataSet *_Data1,unsigned char *_Data2,int Threshold,int Neighbour,int Percent,bool Append);
	void	SlotThresholdSegmentation(RawDataSet *_Data1,unsigned char *_Data2,int LThreshold,int RThreshold,bool Append,int boolMoode=-11);
	void	SlotThresholdRemoveBetween(RawDataSet *_Data1,unsigned char *_Data2,int LThreshold,int RThreshold,bool Append);
	void	SlotRegionGrowingSegmentation(RawDataSet *Data1,unsigned char *Data2,int ThreshLow,int ThreshUp,int Tolerance,int Mode,int view,int x,int y,int z,bool Append,int KernelSize, int KernelType);
	RawDataSet	*SlotArteryExtraction(double rBack, RawDataSet *Data1,unsigned char *Data2,int x,int y,int z,double *startPoint);
	
	RawDataSet	*SlotGenerateArterySimpleTree(RawDataSet *rawData,unsigned char *Data2,QVector <QVector3D> seeds);
	RawDataSet	*SlotGenerateArteryTubes(RawDataSet *rawData,unsigned char *mask,int x,int y,int z,bool *params,double minSkelLen,int dest,unsigned char *veinRGB=NULL,unsigned char *jointRGB=NULL);

	void	SlotFillMask(RawDataSet *Data1,unsigned char *Data2,int View,int x,int y,int z,bool Mode,int _KernelType);
	void	SlotFillRawMask(RawDataSet *Data1,unsigned char *Data2,int x,int y,int z,int value,int _KernelType,int *bounds=NULL);
	void	SlotFillMask2(RawDataSet *Data1,unsigned char *Data2,unsigned char *mask2,int x,int y,int z,int value,int _KernelType);

	void	SlotBinaryMorphology(RawDataSet *Data1,unsigned char *Data2,int View,int SliceNr,int Size, int Mode,int lt=-1,int ht=-1);
	void	SlotBinaryMorphology2(RawDataSet *Data1,unsigned char *Data2,int View,int SliceNr,int Size, int Mode,int lt=-1, int ht=-1);
	void	SlotSetSecondDataForThresholdingCriterion(unsigned short *Data2,int th1, int th2);
	void	SlotSegmentEllipsoid(RawDataSet *Data1,unsigned char *Data2,int X,int Y,int Z,int IX,int IY,int IZ,bool Append);
	
	void	SlotSegmentEllipsoid(RawDataSet *Data1, int X, int Y, int Z, unsigned short v1, unsigned short v2,int IX, int IY, int IZ);

	void	SlotMoveMaskByOnePixel(RawDataSet *Data,unsigned char *MData,int *pos,int crossection,int direction);
	void	SlotJoinTwoPointsWithTubeSemgent(RawDataSet *data,unsigned char *maskData,double p1[3],double p2[3],double radius,int value=1);
	void	SlotJoinPointSetWithTubeSemgent(RawDataSet *data,unsigned char *mask,vtkPoints *pts,double radius,int value=1);

	void	ImproveTubePolyData(vtkPolyData *pData,int nrOfSegments,int dest);
	void	ImproveTubePolyData2(vtkPolyData *pData,int nrOfSegments,int dest);

	void	SlotScreenSurfaceSegmentation(int paintMode,int segMode, double depth,int maxFrustumZ, int maxFrustumSliceSize, double voxSize,double voxTol,bool separateSurfaces,int minFrustumSize,LinkedList<PolygonSurfaceData *> *pDataList,QPolygon array,unsigned char *mask=NULL, RawDataSet *data=NULL,int tl=-1,int tr=-1);
	void	SlotScreenSurfaceFrustimGeneration(LinkedList<PolygonSurfaceData *> *SurfaceDataList,QPolygon qArray,RawDataSet *rData,LinkedList<PolygonSurfaceData *> *backSurfaceDataList=NULL);
	
	void	SlotScreenSurfaceFrustimCalculations(LinkedList<PolygonSurfaceData *> *SurfaceDataList,RawDataSet *data,int mode);
	void	SlotGenerateVisibleMask(RawDataSet *data,unsigned char *mask,LinkedList<PolygonSurfaceData *> *surfaceDataList);

	void	SlotGenerateMarchingCubesFilledIsland(RawDataSet *data,unsigned char *mask);
	void	SlotGenerateMarchingCubesFilledGreyIsland(RawDataSet *data,unsigned char *mask,int minsize=0);
	void	SlotBinarySimpleMorphology(RawDataSet *data,unsigned char *mask,int mode,int size,int kernelType=0);

	void	SlotBinarySimpleMorphology2(RawDataSet *data,unsigned char *mask,int mode,int size, int kernelType, bool readFromVol=false, int volinval=-1, int lthresh=-1,int rthresh=-1);
	signals:
	

	void SignalVoxelCount(int);
	void SendMessage(const QString &Message);
	void SignalSendProgress(int Progress);
	void	SignalSendNewSurface(PolygonSurfaceData *_SurfaceData);
	void	SignalSendNewRawDataSet(RawDataSet*);
	void	SignalSendMask(const unsigned char *mask);
	//void	SignalSendMask( unsigned char *mask);


};	 	
			
#endif //   SEGMENTATION_H
