#ifndef SURFACE_PROCESSING
#define SURFACE_PROCESSING

#include	<cmath>
#include	<stdio.h>
#include	<math.h>
#include	<time.h>

#include	"VtkWidget.h"
#include	"SurfaceCollection.h"
#include	"LinkedList.h"
#include	"GraftVoxelize.h"
#include	"ClosingMeshHoles.h"
#include	"TransformBSpline.h"
#include	"SurfaceColorize.h"

#include	<vtkTriangleStrip.h>
#include	<vtkFloatArray.h>
#include	<vtkPlane.h>
#include	<vtkCellData.h>
#include	<vtkPointData.h>
#include	<vtkPolyDataReader.h>
#include	<vtkPolyDataMapper.h>
#include	<vtkActor.h>
#include	<vtkProperty.h>
#include	<vtkPoints.h>
#include	<vtkMatrix4x4.h>

#include	<vtkDataArray.h>
#include	<vtkPolyDataNormals.h>

#include	<vtkClipPolyData.h>
#include 	<vtkPlaneSource.h>
#include	<vtkSphereSource.h>
#include	<vtkSphere.h>
#include	<vtkPolygon.h>

#include 	<vtkDataSetWriter.h>
#include	<vtkAppendPolyData.h>
#include	<vtkAxes.h>

#include	<vtkAxisActor2D.h>
#include	<vtkLineSource.h>
#include	<vtkConeSource.h>

#include	<vtkCylinderSource.h>
#include	<vtkPolyDataConnectivityFilter.h>
#include	<vtkFeatureEdges.h>

#include	<vtkTriangleFilter.h>
#include	<vtkInterpolatingSubdivisionFilter.h> 
#include	<vtkRuledSurfaceFilter.h>
//#include	<vtkPolyDataToPolyDataFilter.h> 
#include	<vtkLinearSubdivisionFilter.h> 
#include	<vtkCleanPolyData.h> 
#include	<vtkDelaunay2D.h> 
#include	<vtkSurfaceReconstructionFilter.h> 
#include	<vtkDataSetTriangleFilter.h> 
#include	<vtkCellArray.h>
#include	<vtkContourGrid.h> 

#include	<vtkTransform.h>

#include	<vtkTransformPolyDataFilter.h> 
#include	<vtkStripper.h>
#include	<vtkCellArray.h>
#include	<vtkLine.h>
#include	<vtkDataArray.h> 
#include	<vtkGenericCell.h>

#include	<vtkPolygon.h>
#include	<vtkImplicitFunction.h>
#include	<vtkFloatArray.h>
#include	<vtkPolyDataNormals.h>
#include	<vtkDecimatePro.h>
//#include	<vtkDecimate.h>
#include	<vtkQuadricDecimation.h>
#include	<vtkQuadricClustering.h>

#include	<vtkSmoothPolyDataFilter.h>
#include	<vtkPolyDataNormals.h>
#include	<vtkDepthSortPolyData.h> 
#include	<vtkUnsignedShortArray.h>
#include	<vtkPiecewiseFunction.h>
#include	<vtkUnstructuredGrid.h>
#include	<vtkGlyph3D.h>
#include	<vtkCardinalSpline.h>
#include	<vtkSpline.h>

#include	<vtkStructuredPoints.h>
#include	<vtkMarchingCubes.h>
#include	<vtkDiscreteMarchingCubes.h>
#include	<vtkCharArray.h>

#include	<vtkTriangularTCoords.h>
#include	<vtkTextureMapToCylinder.h>
#include	<vtkTextureMapToPlane.h>
#include	<vtkTextureMapToSphere.h>
#include	<vtkHedgeHog.h>

#include	"OtherCommonFunctions.h"
#include	"Transform.h"

#include	<vtkWindowedSincPolyDataFilter.h>


#include	<qapplication.h>
#include	<qstring.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
class SurfaceProcessing: public QObject
{

	Q_OBJECT

public:

	SurfaceProcessing(VtkWidget *_VWidget);
	~SurfaceProcessing();

	
	vtkPolyData	*GenerateCuttingBoundary(vtkPolyData *PData,vtkImplicitFunction *Func);

	void	ReplaceCurrentSurface(vtkPolyData *Input, bool GeomOrig, bool AppendName,const QString &text);
	bool	CompareOrientationOf2Triangles(int *Tab1,int pos1,int pos2);
	void	ApplyFFDDeformation(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,double *min=NULL,double *nn=NULL,PaletteRGBA *palette=NULL,double max=0,vtkPolyData *startPolys=NULL, int vcomponent=0);
	
	void	ApplyFFDDeformation1(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min=NULL,double *nn=NULL,PaletteRGBA *palette=NULL,double max=0,vtkPolyData *startPolys=NULL, int vcomponent=0);
//	PolygonSurfaceData	*ApplyFFDDeformation1b(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min=NULL,double *nn=NULL,PaletteRGBA *palette=NULL,double max=0,vtkPolyData *startPolys=NULL, int vcomponent=0);

	
	void	ApplyFFDSurfaceDeformationArrows(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min=NULL,double *nn=NULL,PaletteRGBA *palette=NULL,double max=0,vtkPolyData *startPolys=NULL, int vcomponent=0);
	void	ApplyFFDSurfaceDeformationLines(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min=NULL,double *nn=NULL,PaletteRGBA *palette=NULL,double max=0,vtkPolyData *startPolys=NULL, int vcomponent=0);
	void	MultiGridSurfaceProcess(void (SurfaceProcessing::*funcs)(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,
			const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys,int vcomponent),LinkedList <PolygonSurfaceData*> *SurfaceDataList,
			const QString &filename, PaletteRGBA *pal,int subframes,bool loop,bool relative=false, int vcomponent=0,double maxDisp=0 );
	
	
	PolygonSurfaceData *	GridSurfaceProcess(PolygonSurfaceData *(SurfaceProcessing::*funcs)(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,
			const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys,int vcomponent),LinkedList <PolygonSurfaceData*> *SurfaceDataList,
			FloatGrid3D *_Grid, PaletteRGBA *pal,int subframes,bool loop,bool relative=false, int vcomponent=0,double maxDisp=0 );

	void	ApplyGridBasedProcessing(void (SurfaceProcessing::*funcs)(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys,int vcomponent),
												 FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys=NULL, int vcomponent=0,double maxDisp=0);

	PolygonSurfaceData	*ApplyGridBasedProcessing2(void (SurfaceProcessing::*funcs)(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys,int vcomponent),
												 FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys=NULL, int vcomponent=0,double maxDisp=0);

	double	DetectMaxDeformation(vtkPolyData *Polys,double *min, double *nn, const QString &filename,bool relative=false, int vcomponent=0);
	void	CalculateMinAndSizeOfPolys(vtkPolyData *Polys,int *N,double *S,double *min, double *nn);
	
	void	CalculateArrowPoint(double *q, double *p,double *r,double *s,double rc,double rs);
	void	GetVectorComponents(double *p0,double *p1, double *n,double *v,int vcomponent);

	int		GetCuttingMode(){return CuttingMode;};
	void	GetCuttingSphereParams(double *center,double &size);
	double	 GenerateLine(double p1[3],double p2[3], QString name, QString group, double col[3]);
	void	 GeneratePoly(double **pts,int nrpts,QString name, QString group, double col[4],double *lenghts, double *angles,double &area,double &circumference,double normal[3] ,bool genPolydata=true );
	
	//CUTTING

	int		CuttingMode;
	bool	CuttingToolVisible;
	bool	CopyTransform;

	int		CurrentPointNumber;
	double	CutingToolSize;



	VtkWidget			*VWidget;
	PolygonSurfaceData	*ObjectSurfaceData;



	vtkActor			*PlaneActor;
	vtkActor			*SphereActor;
	vtkActor			*CylinderActor;

	vtkActor			*CurrentCuttingToolActor;
	vtkActor			*PreviousCuttingToolActor;
	vtkMatrix4x4		*CuttingToolMatrix;

	vtkCellArray *skeletLine;
	vtkPoints	*skelPts;
	RibonTimer RTimer;
	int gCounter;
	bool		enableAffineTransformation;
	double		trans[15];
	double		rotCenter[3];
	Transform	*affineTransform;
	double		matrix[16];
	double		CalculateMeshVolume();

	double		GetArteryArea(int density);
	vtkPoints	*GetSkeletalPoints();
	vtkPoints	*GetSkeletalPoints(vtkPolyData *pData);
	double GetSumOfArteryDiameters(int &npts,double &startDi,double &endDi);
	void	AddTextureCoords(vtkPolyData *pData,int mode,double *params,int nrOfParams);

	void	AddTupleForNeighborTriangle(vtkFloatArray *tc,int id0,int id1, int idlast,int idnew, double length,double alpha);

	void	GenerateNormalsPolyData( vtkPolyData *pData, double scale, QString name);
//ApplyFFDSurfaceDeformationLines
public slots:

	void	SlotTransformAffineSurface(double *trans,bool inv);

	void	SlotGenerateSphericalPointsCuts( LinkedList <PolygonSurfaceData*> *sDataList,QStringList namesList, QVector<double> v, double radius);
	void	SlotDisableAffineTransform();
	void	SlotEnableAffineTransform(double *trans, int nr,double *rotCnt);


	void	SlotCreateHeightMapFromPictureData(RawDataSet *data, double *scales,PaletteRGBA *palette=NULL);
	void	SlotCreatePolygonalCut3DData(RawDataSet  *data, RawDataSet *newData, int view, vtkPoints *pts,double *dd,PaletteRGBA *palette=NULL);
	void	SlotCreateSpheresMapFromPictureData(RawDataSet *data, double *scales);
	void	SlotCreatePointPolyData(RawDataSet *data,unsigned char *mask);
	void	SlotApplyFFDSurfaceDeformationArrows(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys=NULL, int vcomponent=0);
	void	SlotApplyFFDSurfaceDeformationLines(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys=NULL, int vcomponent=0);

	PolygonSurfaceData	*SlotGridSurfaceDeform(	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
		FloatGrid3D *_Grid, PaletteRGBA *pal,	int subframes,bool loop, bool relative=false, int vcomponent=0,double maxDisp=0);

	
	
	void	SlotMultiGridSurfaceDeform(	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
		const QString &filename, PaletteRGBA *pal,	int subframes,bool loop, bool relative=false, int vcomponent=0,double maxDisp=0);
	void	SlotMultiGridSurfaceDeformationArrows(	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
		const QString &filename, PaletteRGBA *pal,	int subframes,bool loop, bool relative=false, int vcomponent=0,double maxDisp=0);
	void	SlotMultiGridSurfaceDeformationLines(	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
		const QString &filename, PaletteRGBA *pal,	int subframes,bool loop, bool relative=false, int vcomponent=0,double maxDisp=0);
	void SlotMultiGridSurfaceDeformationLinePaths(	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
		const QString &filename, PaletteRGBA *pal);



	void	SlotRecomputeNormals(vtkPolyData	*Polys);
	void	SlotComputeNormals(vtkPolyData *&Polys);
	void	SlotShowHideCuttingTool();
	void	SlotCutSurface();
	void	SlotSetCuttingMode(int Mode);
	void	SlotSetCuttingToolSize(int Size);
	void	SlotSetCurrentSurface(PolygonSurfaceData *_SurfaceData);
	void	SlotDecimateSurface(double Value,double Error);
	void	SlotSmoothAndDecimSurfcace(double Factor, int IterNumber,double Value,double Error);
	void	SlotDepthSortSurface();
	void	SlotComputeNormals();
	void	ExtractLinesFromSurface();

	void	SlotSetSurfacePoint(int Nr);
	void	SlotTransformSurfacePoint();

	void	SlotResetActorTransformation();
	void	SlotApplyActorTransformation();
	void	SlotApplyActorTransformationAndRebuild();

	void	SlotChlebiejDeformation1(double Factor);

	void	SlotSmoothSurface(double Factor, int IterNumber);
	void	SlotLargestSurfaceOnly();
	void	SlotClosestToPointerSurfaceOnly();
	void	SlotExtractSeparatedSurfaces();
	void	SlotCleanPolyData();

	void	SlotUnifyTriangleOrientation();
	void	SlotFlipTriangleOrientation();

	void	SlotGenerateNewMarchingCubesSurface(unsigned char *MaskData,int *N,double *Scale,double *origin,const QString&name,bool repaintWithoutReset=false,bool isSegCutted=false,double *color=NULL);
	void	SlotGenerateNewMarchingCubesDecimSurface(unsigned char *MaskData,int *N,double *Scale,double *origin,const QString&name,bool repaintWithoutReset=false,bool isSegCutted=false);
	PolygonSurfaceData*	GenerateMarchingCubesSmDecSurface(unsigned char *MaskData, int *N, double *Scale, double *origin, bool smooth1On=true, bool decimOn=true, bool smooth2On = true, int smoothIterations=100,
		double relaxionsFactor=0.01, double smoothAngle=60, double featureAngle=60, double targetred=0.3);


	void	SlotGenerateMarchingCubesSurface(RawDataSet *_Data,int lt,int rt);
	void	SlotFillPolyDataHolesAsPlanar();
	void	SlotFillPolyDataExtractedPlanarHoles();
	void	SlotFillPolyDataHoles();
	void	SlotFillPolyDataHolesWithCrenellations();
	void	SlotFillPolyDataHolesMaxVolume();

	
	void	SlotCreateGridPolyData(int nx,int ny,int nz,int de);
	vtkPolyData	*SlotCreateGridPolyData2(int nx,int ny,int nz,int de);
	
	PolygonSurfaceData	*SlotApplyFFDDeformation2(FloatGrid3D *Gr,vtkPolyData *Poly,double amount=1.0,const QString &name="",double *min=NULL,double *nn=NULL,PaletteRGBA *palette=NULL,double max=0,vtkPolyData *startPolys=NULL, int vcomponent=0);
//	
	void	SlotApplyFFDDeformation(FloatGrid3D *Gr,vtkPolyData *Poly,double amount=1.0,const QString &name="",double *min=NULL,double *nn=NULL,PaletteRGBA *palette=NULL,double max=0,vtkPolyData *startPolys=NULL, int vcomponent=0);
	void	SlotApplyFFDDeformation(FloatGrid3D *Grid,double amount=1.0,PaletteRGBA *palette=NULL,double max=0);
	void	SlotTransformPolygonalData(double ScaleX,double ScaleY,double ScaleZ,	double ShearX,double ShearY,double ShearZ,	double OriginX,double OriginY,double OriginZ);
	void	SlotFindMaxDistance();
	void	SlotSphereDataSet(int PhiResolution,int ThetaResolution);
	void	SlotSetNewSurfaceOrigin();
	void	SlotCalculateNewSurfaceOrigin( LinkedList <PolygonSurfaceData*> *DataSetList);
	void	SlotCombineAllSurfaces( LinkedList <PolygonSurfaceData*> *DataSetList);
	void	SlotCreateBoundaryDataFromSurface();
	int		SlotMapSurfaceIntoVolume(RawDataSet *data,double *pos, unsigned short val);
	

	void	SlotShowWhichSurfaceIsCurrent();




	PolygonSurfaceData*	SlotCreateErrorSurface(vtkPolyData *p0,vtkPolyData *p1,vtkPolyData *p2,PaletteRGBA *palette,double max=0,const QString &name="",const QString &name2="");
	
	
	void	SlotGenerateFusionOfMultiTubes(LinkedList<PolygonSurfaceData *> *pDataList,QString *names);
	vtkPolyData *joinTwoTubes(vtkPolyData *tube1,vtkPolyData *tube2,bool *ends);
		void	GenerateMissingTubePart(vtkPoints *pts,vtkCellArray *polysFinal,int *startMPoints, int*direction,int dest,int *centerPoints);

signals:

	void	SignalSendNewMaxDisp(  const QString & );
	void	SignalSendNewSurfaceOrigin(double *);
	void	SignalDeleteCurrentSurface();
	void	SignalHideCurrentSurface();
	void	SignalSendNewSurface(PolygonSurfaceData *_SurfaceData);
	void	SignalSendNewSurfaceAfterDelete(PolygonSurfaceData *_SurfaceData);
	void	SignalSendCuttingActor();
	void	SignalDeleteAllSurfaces();

	void	SignalShowNewPolygonalSurfaces();
	void	SignalHideNewPolygonalSurfaces();

	void	SendMessage(const QString &Message);


};



#endif //SURFACE_PROCESSING
