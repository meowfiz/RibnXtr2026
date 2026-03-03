#ifndef FEATUREPOINT_REGISTRATION_H
#define FEATUREPOINT_REGISTRATION_H

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


#include	<vtkPolyData.h>
#include	<vtkPoints.h>
#include	<vtkMatrix4x4.h>
#include	<vtkTransform.h>
#include	<vtkTransformPolyDataFilter.h> 
#include	<vtkActor.h>



class DistFunction: public Function
{
	public:
		DistFunction(Transform *_transform,vtkPoints *ModelPoints,vtkPoints *_ObjectPoints);
		double GetFunctionValue(double *);
		void    intit(){};
		vtkPoints *ModelPoints;
		vtkPoints *ObjectPoints;

		Transform	*transf;
		double Dist[3];
		double r1[3];
};





class DistFunctionWithDerivs: public Function
{
	public:
			DistFunctionWithDerivs(Transform *_transform,vtkPoints *ModelPoints,vtkPoints *_ObjectPoints);

			double	GetFunctionValue(double ,double *);
			void	CalculateDerivative(double *Deriv,double *Params);
			void	PrecalculateVariables(double *);
			void    intit(){};
	private:
			vtkPoints *ModelPoints;
			vtkPoints *ObjectPoints;


			Transform	*transf;
			Derivatives *derivs;
			//double DistX,DistY,DistZ,rx1,ry1,rz1;
			double Dist[3];
			double r1[3];
};



class FeaturePointRegistration: public QObject
{
	Q_OBJECT

	public:
		

		FeaturePointRegistration(vtkPoints *_ModelPoints,vtkPoints *_ObjectPoints,double *_RotateCenter,PolygonSurfaceData *ObjectSurfaceData,double *_trans,VtkWidget *_VWidget,double tol);
		~FeaturePointRegistration();
		
	
		double Tolerance;
		void Optimize();

		vtkPoints *ModelPoints;
		vtkPoints *ObjectPoints;

		int NumberOfPoints;

		vtkActor	*ObjectActor;
		vtkMatrix4x4 *Matrix;
		VtkWidget	*VWidget;
		double		*trans;
	
		Transform	*transform;


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
		void	GenerateMinimumPaths();

	signals:

		void	SignalGrab3Didnow();
	
	public slots:

		void ActorUpdate();
		void ActorUpdate(double *);

};





#endif //FEATUREPOINT_REGISTRATION_H
