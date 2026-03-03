#ifndef SYMMETRY_PLANES_FINDER_H
#define SYMMETRY_PLANES_FINDER_H

#include	<qapplication.h>
//#include	<q3filedialog.h>
#include	<qstring.h>
#include	<qregexp.h>
#include	"DistanceMap.h"
#include	"RawDataSet.h"
#include	"Transform.h"
#include	"SurfaceCollection.h"
#include	"VtkWidget.h"

#include	"OptimizationMarquardt.h"
#include	"OptimizationPowell.h"
#include	"AbstractFunctionClasses.h"

#include	"PlaneObject.h"

#include	<vtkPolyData.h>
#include	<vtkPoints.h>
#include	<vtkMatrix4x4.h>
#include	<vtkTransform.h>
#include	<vtkTransformPolyDataFilter.h> 
#include	<vtkActor.h>
#include	<vtkPlane.h>
#include	"SurfaceData.h"

//class PlaneDistFunction: public Function
//{
//	public:
//		PlaneDistFunction(double *_tab_left,double *_tab_right,int _nrr,int _nrl,double *_p0,double *_p1,double *_p2);
//		double GetFunctionValue(double *);
//		
//		void    intit(){};
//		
//		PlaneObject *planeObject;
//
//		int nrl,nrr;
//		double *tab_left,*tab_right;
//		double p0[3],p1[3],p2[3];
//};





class SymmetryPlanesFinder: public QObject, public Function
{
	Q_OBJECT

	public:
		

		SymmetryPlanesFinder(vtkPoints *_LeftPoints,vtkPoints *_RightPoints,double *_p0=NULL,double *_p1=NULL,double *_p2=NULL);
		~SymmetryPlanesFinder();
	
	
		vtkPoints * GenerateTransformedPoints(double *params);
		double * getTransParams()
		{
			return finalParams;
		};
		//double Tolerance;
		void Optimize();
		double finalParams[7];
	//	vtkPoints *leftPoints;
		//vtkPoints *rightPoints;
		int nrl,nrr;
		double *tab_left,*tab_right;
		double p0[3],p1[3],p2[3];
		PlaneObject *planeObject1,*planeObject2;

		double GetFunctionValue(double *);

		Transform	*transform;
		
		double rCenter[3];
		//		
		void    intit(){};
		DistanceMap *distMap;
		vtkPlane *plane;
		RawDataSet *sssData;
		double direction[3];
		double sScale[3];
		double d01[3];
		/*int NumberOfPoints;

		vtkActor	*ObjectActor;
		vtkMatrix4x4 *Matrix;
		VtkWidget	*VWidget;
		double		*trans;
	
		
		

		double		*RotateCenter;
		bool		ArithmeticCenter;
		bool		precalculate;
		double		chisq;


		int		GraphXResolution;
		int		GraphYResolution;
		double	ROTT,TRANST;

		double	GetEnergy(double *ta);
		void	GenerateMisregistrationGraphData();
		QString TransSymbolName(int nr);
		void	GenerateMinimumPaths();*/

	signals:

		//void	SignalGrab3Didnow();
		void	SignalSendNewRawDataSet(RawDataSet*);
	
	public slots:

	/*	void ActorUpdate();
		void ActorUpdate(double *);*/

};





#endif //SYMMETRY_PLANES_FINDER_H
