//#ifndef SURFACE_ELASTIC_REGISTRATION_H
//#define SURFACE_ELASTIC_REGISTRATION_H
//
//#include	<qapplication.h>
//#include	<qfiledialog.h>
//#include	<qstring.h>
//#include	<qregexp.h>
//
//#include	"DistanceMap.h"
//#include	"RawDataSet.h"
//#include	"Transform.h"
//#include	"SurfaceCollection.h"
//#include	"VtkWidget.h"
//
//#include	"OptimizationPowell.h"
//#include	"AbstractFunctionClasses.h"
//#include	"OtherCommonFunctions.h"
//#include	"TransformBSpline.h"
//#include	"SurfaceProcessing.h"
//#include	"OptimizationDFPowell.h"
//
//#include	<vtkPolyData.h>
//#include	<vtkPoints.h>
//#include	<vtkMatrix4x4.h>
//#include	<vtkTransform.h>
//#include	<vtkTransformPolyDataFilter.h> 
//#include	<vtkActor.h>
//	
/////////////////////////////////////////////////////////
//class SurfaceElasticRegistration: public QObject,public Function
//{
//	Q_OBJECT
//
//	public:
//		
//
//		SurfaceElasticRegistration(PolygonSurfaceData *ModelSurfaceData,PolygonSurfaceData *ObjectSurfaceData,
//										 DistanceMap *_Distance,double *_trans,double *Origin,VtkWidget *_VWidget,FloatGrid3D *_Grid ,const QString& text="",QString fileDefaultDir="");
//		~SurfaceElasticRegistration();
//		
//		void Optimize(int iter=1);
//
//	
//		double	GetEnergy();
//		double	GetEnergy(int x,int y,int z);
//		double	GetEnergy(int pos);
//		double	GetFunctionValue(double *);
//		void    intit(){};
//		void	SetCurrentNode(int x,int y,int z);
//		int		GetNumberOfNeighboringPoints(int x,int y,int z);
//		
//		double			(SurfaceElasticRegistration::*GetFValue)(int x,int y,int z,double *);
//		void			(SurfaceElasticRegistration::*GetDerivatives)(double *Deriv,double *Params);
//		double			GetEnergy(int x,int y,int z,double *params);
//void	OptimizeBig(int _iter,int _optimStep,bool _randomSampling,int multiressteps, bool _fixBounds);
//double 	GetTotalFunctionValue(int a,int b,int c,double *Params);
//void	CalculateTotalDerivative(double *Deriv,double *Params);
//void	SetParamsFromGridToArray(FloatGrid3D *Grid,double *Params, bool fixbounds);
//void	SetParamsFromArrayToGrid(FloatGrid3D *Grid,double *Params, bool fixbounds);
////double  GetGradI(int i,int j,int k,double smallStep,double fvalue,int view);
//double  GetGradI(int posx,BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf);
//void	CalculateDerivativesAtPoint(int i,int j,int k,double *der,double *drvTempTab,int &count, BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf);
//void	AddGradPart2(double *der,double *drvTempTab,double smallStep,int view,BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf);
//void	AddGradPart1(int posx, int &count,double *drv,double &drvel);
//void	GradientDescentOptimizer(double *_Params,int _NumberOfParameters,double _Tolerance,   int *_IterationNumber,double *_FunctionValue);
//void	UpdateData(int gridNx, int gridNy, int gridNz);
//void	CalculateDerivative(double *Deriv,double *Params);
//int	GetNodesToOptimization(FloatGrid3D *Grid,double *Params, bool fixbounds,double threshold,Node *&Nodes);
//
//	private:		
//		Node *nodes;
//		int maxNrOfSamples;
//		double alpha;
//	double mMax;
//	double mVolMax,mVolMax2;
//	int nx1,ny1,nz1;
//	int mx1,my1,mz1;
//	bool stopOptimization; 
//			FloatGrid3D *TempGrid;
//		bool fixBounds;
//			int		countsize;
//	int		nrofsamples;
//	int		optimStep;
//	bool	randomSampling;
//			int		nrOfParams;
//		double	*derivs;
//			double smallStepX;
//	double smallStepY;
//	double smallStepZ;
//	
//	int nrOfNodes;
//	double threshold;
//	
//	int iterx;
//		double fval;
//		QString initText;
//		FloatGrid3D *Grid;
//		FloatGrid3D *OrigGrid;
//	
//		BSplineGridTransformation *BSplineTransform;
//
//		vtkPolyData *ModelPolyData;
//		vtkPolyData *ObjectPolyData;
//		vtkActor	*ObjectCuttingEdgeActor;
//		vtkActor	*ObjectActor;
//		vtkMatrix4x4 *Matrix;
//		VtkWidget	*VWidget;
//		double		*trans,*transx;
//		DistanceMap	*distance;
//		Transform	*transform;
//
//		int **tabpos;
//		int *tabnr;
//		int nsize,nxny,nx,ny,nz;
//		double scaleX,scaleY,scaleZ;
//		double tdx,tdy,tdz;
//		double fx,fy,fz;
//		int *tabXXX;
//		double nnx,nny,nnz;
//		
//		vtkPolyData *tempPolyData;
//		vtkActor	*tempActor;
//		vtkProperty *tempProperty;
//		int  *objN;
//		double *objS;
//		double tempOpacity;
//	
//		
//		QString fileDefaultDir;
//		double Step[3];
//
//		double		*RotateCenter;
//		bool		ArithmeticCenter;
//		bool		precalculate;
//		double		chisq;
//
//		int		NodX,NodY,NodZ;
//
//		void	GetTransFromCurrentNode(double *tr);
//		void	SetTransToCurrentNode(double *tr);
//
//		void	GetTransFromCurrentNode(double *tr, FloatGrid3D *grid);
//		void	SetTransToCurrentNode(double *tr, FloatGrid3D *grid);
//	
//
//
//	signals:
//
//		void	SignalGrab3Window();
//	
//		void	SendMessage(const QString &Message);
//		void	SignalSendProgress(int);
//
//	public slots:
//
//		void ActorUpdate();
//
//};
//
//
//
//
//
//#endif //SURFACE_REGISTRATION_H

#ifndef SURFACE_ELASTIC_REGISTRATION_H
#define SURFACE_ELASTIC_REGISTRATION_H

#include	<qapplication.h>
//#include	<q3filedialog.h>
#include	<qstring.h>
#include	<qregexp.h>

#include	"DistanceMap.h"
#include	"RawDataSet.h"
#include	"Transform.h"
#include	"SurfaceCollection.h"
#include	"VtkWidget.h"

#include	"OptimizationPowell.h"
#include	"AbstractFunctionClasses.h"
#include	"OtherCommonFunctions.h"
#include	"TransformBSpline.h"
#include	"SurfaceProcessing.h"


#include	<vtkPolyData.h>
#include	<vtkPoints.h>
#include	<vtkMatrix4x4.h>
#include	<vtkTransform.h>
#include	<vtkTransformPolyDataFilter.h> 
#include	<vtkActor.h>
	
///////////////////////////////////////////////////////
class SurfaceElasticRegistration: public QObject,public Function
{
	Q_OBJECT

	public:
		

		SurfaceElasticRegistration(PolygonSurfaceData *ModelSurfaceData,PolygonSurfaceData *ObjectSurfaceData,
										 DistanceMap *_Distance,double *_trans,double *Origin,VtkWidget *_VWidget,FloatGrid3D *_Grid ,const QString& text="");
		~SurfaceElasticRegistration();
		
		void Optimize(int iter=1);

		double	GetEnergy();
		double	GetEnergy(int x,int y,int z);
		double	GetEnergy(int pos);
		double	GetFunctionValue(double *);
		void    intit(){};
		void	SetCurrentNode(int x,int y,int z);
		int		GetNumberOfNeighboringPoints(int x,int y,int z);
		
		

	private:	
		QString initText;
		FloatGrid3D *Grid;
		FloatGrid3D *OrigGrid;
	
		BSplineGridTransformation *BSplineTransform;

		vtkPolyData *ModelPolyData;
		vtkPolyData *ObjectPolyData;
		vtkActor	*ObjectCuttingEdgeActor;
		vtkActor	*ObjectActor;
		vtkMatrix4x4 *Matrix;
		VtkWidget	*VWidget;
		double		*trans,*transx;
		DistanceMap	*distance;
		Transform	*transform;

		int **tabpos;
		int *tabnr;
		int nsize,nxny,nx,ny,nz;
		double scaleX,scaleY,scaleZ;
		double tdx,tdy,tdz;
		double fx,fy,fz;
		int *tabXXX;
		
		vtkPolyData *tempPolyData;
		vtkActor	*tempActor;
		vtkProperty *tempProperty;
		int  *objN;
		double *objS;
		double tempOpacity;
	
		
		
		double Step[3];

		double		*RotateCenter;
		bool		ArithmeticCenter;
		bool		precalculate;
		double		chisq;

		int		NodX,NodY,NodZ;

		void	GetTransFromCurrentNode(double *tr);
		void	SetTransToCurrentNode(double *tr);


	signals:

		void	SignalGrab3Window();
	
		void	SendMessage(const QString &Message);
		void	SignalSendProgress(int);

	public slots:

		void ActorUpdate();

};





#endif //SURFACE_REGISTRATION_H 