#ifndef SURFACE_REGISTRATION_H
#define SURFACE_REGISTRATION_H

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
#include	"AbstractFunctionClasses.h"


#include	<vtkPolyData.h>
#include	<vtkPoints.h>
#include	<vtkMatrix4x4.h>
#include	<vtkTransform.h>
#include	<vtkTransformPolyDataFilter.h> 
#include	<vtkActor.h>



class EuclideanDistFunctionWithDerivs: public Function
{
	public:
			EuclideanDistFunctionWithDerivs(DistanceMap *_distancemap,Transform *_transform);
			double	GetFunctionValue(double ,double *);
			void	CalculateDerivative(double *Deriv,double *Params);
			void	PrecalculateVariables(double *);
			void    intit(){};

			void SetNormalize(bool norm) { normalize = norm; };
			bool GetNormalize() { return normalize; };
			bool normalize = false;
			double normalizeVal;

			DistanceMap	*distancemap;
			Transform	*transf;
			Derivatives *derivs;
			double DistX,DistY,DistZ,rx1,ry1,rz1;
};



class SurfaceRegistration: public QObject
{
	Q_OBJECT

	public:
		
		SurfaceRegistration(PolygonSurfaceData *ModelSurfaceData,PolygonSurfaceData *ObjectSurfaceData,
										 DistanceMap *_Distance,double *_trans,double *Origin,VtkWidget *_VWidget,int _Percent=100,bool BoundingboxOnly=false);
		~SurfaceRegistration();
		
	

		void Optimize();

	


		vtkPolyData *ModelPolyData;
		vtkPolyData *ObjectPolyData;
		vtkActor	*ObjectCuttingEdgeActor;
		vtkActor	*ObjectActor;
		vtkMatrix4x4 *Matrix;
		VtkWidget	*VWidget;
		double		*trans;
		DistanceMap	*distance;
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

		void	SignalGrab3Window();
	
	public slots:

		void ActorUpdate();

};





#endif //SURFACE_REGISTRATION_H
