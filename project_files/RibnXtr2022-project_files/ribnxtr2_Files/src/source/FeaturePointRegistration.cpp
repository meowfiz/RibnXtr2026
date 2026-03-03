#include "FeaturePointRegistration.h"

DistFunction::DistFunction(Transform *_transform,vtkPoints *_ModelPoints,vtkPoints *_ObjectPoints):Function()
{
	transf=_transform;
	ModelPoints=_ModelPoints;
	ObjectPoints=_ObjectPoints;
}


double DistFunction::GetFunctionValue(double *Params)
{

	int Nr=ObjectPoints->GetNumberOfPoints();

	double Point0[3];
	double Point1[3];
	double Point2[3];
	double Distance,TDistance;
	
	int i,j;
	Distance=0;
	transf->CalculateTrigonometricFunctions(Params);
	for (j=0;j<Nr;j++)
	{
		ObjectPoints->GetPoint(j,Point0);
		ModelPoints->GetPoint(j,Point2);
		transf->ApplyTransformationF( Point0 ,Point1,Params);
		TDistance=0;
		for (i=0;i<3;i++)
		{
			r1[i]=Point1[i];
			Dist[i]=(Point1[i]-Point2[i]);
			TDistance+=Dist[i]*Dist[i];
		}

		TDistance=sqrt(TDistance);
		Distance+=TDistance;
	}


	return Distance;

}
		


DistFunctionWithDerivs::DistFunctionWithDerivs(Transform *_transform,vtkPoints *_ModelPoints,vtkPoints *_ObjectPoints):Function()
{
	
	transf=_transform;
	ModelPoints=_ModelPoints;
	ObjectPoints=_ObjectPoints;
	derivs=new Derivatives();
//	rx1=ry1=rz1=DistX=DistY=DistZ=0;
	int i;
	for (i=0;i<3;i++)
	{
		r1[i]=0;
		Dist[i]=0;
	}



}
double DistFunctionWithDerivs::GetFunctionValue(double nr,double *Params)
{
	int index=Round(nr-1);
	double Point0[3];
	double Point1[3];
	double Point2[3];
	double Distance;

	
	ObjectPoints->GetPoint(index,Point0);
	ModelPoints->GetPoint(index,Point2);

	transf->ApplyTransformationF( Point0 ,Point1,Params);
	Distance=0;

	int i;
	for (i=0;i<3;i++)
	{
		r1[i]=Point2[i];
		Dist[i]=(Point1[i]-Point2[i]);
		Distance+=Dist[i]*Dist[i];
	}
	Distance=sqrt(Distance);
	
	if (Distance!=0)
	{
		double tt=1.0/Distance;
		for (i=0;i<3;i++)
		{
			Dist[i]*=tt;
		}
	}

	//Distance=sqrt(DistX*DistX+DistY*DistY+DistZ*DistZ);


	return Distance;

}
void DistFunctionWithDerivs::CalculateDerivative(double *Deriv,double *Params)
{
	derivs->CalculateDerivatives(Deriv,r1[0],r1[1],r1[2],Dist[0],Dist[1],Dist[2],Params);

}
void DistFunctionWithDerivs::PrecalculateVariables(double *Params)
{
	derivs->CalculateTrigonometricParams(Params);
	transf->CalculateTrigonometricFunctions(Params);
}
	


FeaturePointRegistration::FeaturePointRegistration(vtkPoints *_ModelPoints,vtkPoints *_ObjectPoints,double *_RotateCenter,PolygonSurfaceData *ObjectSurfaceData,double *_trans,VtkWidget *_VWidget,double tol)
{
	Tolerance=tol;
	trans=_trans;
	VWidget=_VWidget;
	ModelPoints=_ModelPoints;
	//ObjectPoints=_ObjectPoints;
	RotateCenter=_RotateCenter;
	
	ObjectActor=ObjectSurfaceData->GetVActor();



	transform=new Transform(ObjectSurfaceData,RotateCenter,-1);

	NumberOfPoints=MIN(ModelPoints->GetNumberOfPoints(),_ObjectPoints->GetNumberOfPoints());

	// cofnac wszystkie pkty obiektu o RotateCenter
	ObjectPoints=vtkPoints::New();

	int i,j;
	double pPoint1[3];
	for(i=0;i<NumberOfPoints;i++)
	{
		_ObjectPoints->GetPoint(i,pPoint1);
		for(j=0;j<3;j++)
			pPoint1[j]-=RotateCenter[j];

		ObjectPoints->InsertNextPoint(pPoint1);

	}

	//transform->CreateIndexedArray(ModelSurfaceData->PScale[0],ModelSurfaceData->PScale[1],ModelSurfaceData->PScale[2]);


}

FeaturePointRegistration::~FeaturePointRegistration(void)
{
}


void FeaturePointRegistration::ActorUpdate()
{
	double Point1[3],Point2[3];
	int i;
		for(i=0;i<3;i++)
			Point1[i]=trans[i+1];
		for(i=0;i<3;i++)
			Point2[i]=-trans[i+4]* to_deg;


		ObjectActor->SetOrientation(Point2);
		ObjectActor->SetPosition(Point1);

		VWidget->Interactor->repaint();

}

void FeaturePointRegistration::ActorUpdate(double *_trans)
{
	double Point1[3],Point2[3];
	int i;
		for(i=0;i<3;i++)
			Point1[i]=_trans[i+1];
		for(i=0;i<3;i++)
			Point2[i]=-_trans[i+4]* to_deg;


		ObjectActor->SetOrientation(Point2);
		ObjectActor->SetPosition(Point1);

		VWidget->Interactor->repaint();

}

void FeaturePointRegistration::Optimize()
{
	
	//double Tolerance=1e-5;
/*	int Iterations=0;
	double FValue;

	DistFunction *Fun=new DistFunction(transform,ModelPoints,ObjectPoints);

	OptimizationPowell *Optim=new OptimizationPowell(trans,6,Tolerance,&Iterations,&FValue,Fun); //:) 
	connect(Optim,SIGNAL(SignalUpdatedTrans(double *)),this,SLOT(ActorUpdate(double *)));

	Optim->StartOptimization();
	
	delete Optim;
 */
 
 
	int i;
	int Hsize= NumberOfPoints+1;
	int Lsize=7;

	double	*y=new double[Hsize];     
	double	*x=new double[Hsize];     
	double	*sig=new double[Hsize]; 
	int		*ia=new int[Lsize];


	for (i=1;i<Hsize;i++)
	{
    	sig[i]=1;
		x[i]=i;
		y[i]=0;
	}

	for (i=1;i<Lsize;i++)
		ia[i]=1;


	DistFunctionWithDerivs *Fun=new DistFunctionWithDerivs(transform,ModelPoints,ObjectPoints);


	OptimizationMarquardt *optim=new OptimizationMarquardt(x,y,sig,Hsize,trans,ia,Lsize,Fun);
	connect(optim,SIGNAL(Updated()),this,SLOT(ActorUpdate()));
	optim->StartOptimization();

	delete optim;
	delete Fun;
	delete []ia;
	delete []sig;
	delete []x;
	delete []y;
	
	
}
