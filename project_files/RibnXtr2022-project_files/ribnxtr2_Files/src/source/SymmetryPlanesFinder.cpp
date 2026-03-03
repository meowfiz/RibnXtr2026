#include "SymmetryPlanesFinder.h"

vtkPoints * SymmetryPlanesFinder::GenerateTransformedPoints(double *params)
{

	//params beda rowne obrotowi 

	double px0[3];
	double px1[3];
	double px2[3];
	double px3[3];

	double params1[7]={0};
	double distL,distR,distU,distD;
	double p00[3];
	double p01[3];
	double p02[3];
	double v1[3],v2[3];
	int i,j,k;
	
	
	 
	double finalVal = GetFunctionValue(params);

	params1[1] = params[4];
	params1[2] = params[5];
	params1[3] = params[6];
	params1[4] = params[1] * to_rad;
	params1[5] = params[2] * to_rad;
	params1[6] = params[3] * to_rad;
	transform->CalculateTrigonometricFunctions(params1);

	double d01[3];
	transform->ApplyTransformationFloat(direction, d01, params1);//opbrocony werktor prostopadly do plaszczyzny
	p01[0] = p0[0] + params1[1];
	p01[1] = p0[1] + params1[2];
	p01[2] = p0[2] + params1[3];


	plane->SetOrigin(p01);
	plane->SetNormal(d01);

	double fn=0;
	double dist;
	double xproj[3];
	double xn[3];
	double vv[3];
	double myP[3];
	vtkPoints *pts=vtkPoints::New();
	for (k=0;k<nrr;k++)
	{
		myP[0]=tab_right[k*3];
		myP[1]=tab_right[k*3+1];
		myP[2]=tab_right[k*3+2];

		plane->ProjectPoint(myP,xproj);

		vv[0]=xproj[0]-myP[0];
		vv[1]=xproj[1]-myP[1];
		vv[2]=xproj[2]-myP[2];

		xn[0]=xproj[0]+vv[0];
		xn[1]=xproj[1]+vv[1];
		xn[2]=xproj[2]+vv[2];

	

		pts->InsertNextPoint(xn);
	

	}


	return pts;
}
		



	double SymmetryPlanesFinder::GetFunctionValue(double *params)
{

	//params beda rowne obrotowi 

	double px0[3];
	double px1[3];
	double px2[3];
	double px3[3];

	double params1[7]={0};
	double distL,distR,distU,distD;
	double p00[3];
	double p01[3];
	double p02[3];
	double v1[3],v2[3];
	int i,j,k;
	
	//podmianka bo chce zeby w parametrach na poczatku byly rotacje a potem translacje
	
	params1[1] = params[4];//do 
	params1[2]=params[5];
	params1[3]=params[6];
	params1[4]=params[1] * to_rad;
	params1[5]=params[2] * to_rad;
	params1[6]=params[3] * to_rad;


	/*params1[1]=direction[0]*params[1];
	params1[2]=direction[1]*params[1];
	params1[3]=direction[2]*params[1];

	params1[4]=params[2];
	params1[5]=params[3];
	params1[6]=params[4];*/

	transform->CalculateTrigonometricFunctions(params1);

	//params

	//rotx,roty,rotz
	//tx ty,tz
	
	

	//przekszta³cam
	//for(j=0;j<3;j++)
	//{
	//	//p00[j]=p0[j]-rCenter[j];
	//	d00[j] = (p0[j] + direction[j]) - rCenter[j];
	//	//p01[j]=p1[j]-rCenter[j];
	//	//p02[j]=p2[j]-rCenter[j];
	//	
	//}

	//transform->ApplyTransformationFloat(p00, p01, params1);
	transform->ApplyTransformationFloat(direction, d01, params1);//opbrocony werktor prostopadly do plaszczyzny

	//for (j = 0;j < 3;j++)
	//{


	//nowa pozycja punktu origin plaszczyzny - orginal plus translacja z optymalizacji
	p01[0] = p0[0] + params1[1];
	p01[1] = p0[1] + params1[2];
	p01[2] = p0[2] + params1[3];


	plane->SetOrigin(p01);
	plane->SetNormal(d01);

		//d01[j] += rCenter[j] - p01[j];
	//}




	//transform->ApplyTransformationFloat(p00,px0,params1);
	//transform->ApplyTransformationFloat(p01,px1,params1);
	//transform->ApplyTransformationFloat(p02,px2,params1);






	//for(j=0;j<3;j++)
	//{
	//	v1[j]=px1[j]-px0[j];
	//	v2[j]=px2[j]-px0[j];
	//}


	//CROSS(px3,v2,v1);

	///*for(j=0;j<3;j++)
	//{
	//	px3[j]+=px0[j];
	//}*/


	///*planeObject1->SetPlanePoints(px0,px1,px2);
	//planeObject2->SetPlanePoints(px0,px1,px3);*/


	//plane->SetOrigin(px0);
	//plane->SetNormal(px3);

	//transform->ApplyTransformationFloat(p00, px0, params1);

	/*plane->SetOrigin(p0);
	plane->SetNormal(direction);*/


	double fn=0;
	double dist;
	double xproj[3];
	double xn[3];
	double vv[3];
	double myP[3];
	for (k=0;k<nrr;k++)
	{
		myP[0]=tab_right[k*3];
		myP[1]=tab_right[k*3+1];
		myP[2]=tab_right[k*3+2];

		plane->ProjectPoint(myP,xproj);

		vv[0]=xproj[0]-myP[0];
		vv[1]=xproj[1]-myP[1];
		vv[2]=xproj[2]-myP[2];

		xn[0]=xproj[0]+vv[0];
		xn[1]=xproj[1]+vv[1];
		xn[2]=xproj[2]+vv[2];

		xn[0]*=sScale[0];
		xn[1]*=sScale[1];
		xn[2]*=sScale[2];

		dist=distMap->GetDistanceValue(xn[0],xn[1],xn[2]);
		fn+=dist*dist;

	}
	fn/=(double)nrr;

	return fn;
}
		


	


SymmetryPlanesFinder::SymmetryPlanesFinder(vtkPoints *leftPoints,vtkPoints *rightPoints,double *_p0,double *_p1,double *_p2):Function()
{
	plane=NULL;
	distMap=NULL;
	/*leftPoints=NULL;
	rightPoints=NULL;*/
	nrl=0,nrr=0;
	tab_left=NULL;
	tab_right=NULL;
	int i,j,k;
	direction[0]=1.0;
	direction[1]=0;
	direction[2]=0;
	if ((leftPoints!=NULL)&&(rightPoints!=NULL))
	{
		
		/*leftPoints=_LeftPoints;
		rightPoints=_RightPoints;*/
		 nrl=leftPoints->GetNumberOfPoints();
		 nrr=rightPoints->GetNumberOfPoints();

		if ((_p0==NULL)||(_p1==NULL)||(_p2==NULL)||(nrl==0)||(nrr==0))
		{
			//policz punkty startowe

			//licze srodki dla obu zestawow...

			
			tab_left=new double[nrl*3];
			tab_right=new double[nrr*3];
			double c1[3]={0};
			double c2[3]={0};
			double c[3]={0};
			

			for(j=0;j<nrl;j++)
			{
				leftPoints->GetPoint(j,tab_left+j*3);
				c1[0]+=tab_left[j*3];
				c1[1]+=tab_left[j*3+1];
				c1[2]+=tab_left[j*3+2];
			}

			for(j=0;j<nrr;j++)
			{
				rightPoints->GetPoint(j,tab_right+j*3);
				c2[0]+=tab_right[j*3];
				c2[1]+=tab_right[j*3+1];
				c2[2]+=tab_right[j*3+2];
			}

			for(j=0;j<3;j++)
			{
				c1[j]/=(double)nrl;
				c2[j]/=(double)nrr;
				p0[j]=(c1[j]+c2[j])*0.5;
				direction[j]=c2[j]-c1[j];
			}

			normV(direction);


			
			
			//mam p0

			//p1 zrobie dodajac do p0 0,0,1
			//p2 zrobie dodajac do p0 0,1,0
		
			double v1[3]={0,0,1};
			double v2[3]={0,1,0};

			for(j=0;j<3;j++)
			{
				p1[j]=p0[j]+v1[j];
				p2[j]=p0[j]+v2[j];
			}



		}
		else
		{
			for(j=0;j<3;j++)
			{
				p0[j]=_p0[j];
				p1[j]=_p1[j];
				p2[j]=_p2[j];
			}
		}
	


	/*	planeObject1=new PlaneObject();
		planeObject2=new PlaneObject();*/


		double bounds[6];
		double minBound[3];
		double maxBound[3];
		double margin=5.0;
		
		leftPoints->GetBounds(bounds);

		minBound[0]=bounds[0]-margin;
		maxBound[0]=bounds[1]+margin;
		minBound[1]=bounds[2]-margin;
		maxBound[1]=bounds[3]+margin;
		minBound[2]=bounds[4]-margin;
		maxBound[2]=bounds[5]+margin;

		double floatSize[3];
		double cuboidScale[3];
			int size[3]={128,128,128};
		for(j=0;j<3;j++)
		{
			floatSize[j]=maxBound[j]-minBound[j];
			cuboidScale[j]=floatSize[j]/(double)(size[j]);
			sScale[j]=1.0/cuboidScale[j];
		}

		distMap=new DistanceMap(leftPoints,minBound,size,cuboidScale);
		distMap->PrecalucateDistanceMap();
		plane=vtkPlane::New();
		/*sssData=distMap->Create3DDistanceMapRawDataSet();
		sssData->SetRawDataSetName("mapa");*/
		
		
		//FloatfloatSizeSize[2]=PSD->GetMaxBound()[2]-PSD->GetMinBound()[2];

		//for(i=0;i<3;i++)
		//	CuboidScale[i]=PSD->GetPScale()[i]=FloatSize[i]/(double)(SizeTemp[i]-1);
		//distMap=new DistanceMap(	leftPoints,PSD->GetMinBound(),SizeTemp,CuboidScale);

		for (j=0;j<3;j++)
		{
			//rCenter[j]=p0[j];
			rCenter[j] = 0;
		}
		transform=new Transform(rCenter,1.0,1.0,1.0);
	
	} 

	


	//Tolerance=tol;
	//trans=_trans;
	//VWidget=_VWidget;
	//ModelPoints=_ModelPoints;
	////ObjectPoints=_ObjectPoints;
	//RotateCenter=_RotateCenter;
	//
	//ObjectActor=ObjectSurfaceData->GetVActor();



	//transform=new Transform(ObjectSurfaceData,RotateCenter,-1);

	//NumberOfPoints=MIN(ModelPoints->GetNumberOfPoints(),_ObjectPoints->GetNumberOfPoints());

	//// cofnac wszystkie pkty obiektu o RotateCenter
	//ObjectPoints=vtkPoints::New();

	//int i,j;
	//double pPoint1[3];
	//for(i=0;i<NumberOfPoints;i++)
	//{
	//	_ObjectPoints->GetPoint(i,pPoint1);
	//	for(j=0;j<3;j++)
	//		pPoint1[j]-=RotateCenter[j];

	//	ObjectPoints->InsertNextPoint(pPoint1);

	//}

	//transform->CreateIndexedArray(ModelSurfaceData->PScale[0],ModelSurfaceData->PScale[1],ModelSurfaceData->PScale[2]);


}

SymmetryPlanesFinder::~SymmetryPlanesFinder(void)
{
	if (tab_left!=NULL) delete []tab_left;
	if (tab_right!=NULL) delete []tab_right;
	/*delete planeObject1;
	delete planeObject2;*/
	if (distMap!=NULL) delete distMap;
	if (plane!=NULL) plane->Delete();
	
}


//void SymmetryPlanesFinder::ActorUpdate()
//{
//	double Point1[3],Point2[3];
//	int i;
//		for(i=0;i<3;i++)
//			Point1[i]=trans[i+1];
//		for(i=0;i<3;i++)
//			Point2[i]=-trans[i+4]*deg;
//
//
//		ObjectActor->SetOrientation(Point2);
//		ObjectActor->SetPosition(Point1);
//
//		VWidget->Interactor->repaint();
//
//}

//void FeaturePointRegistration::ActorUpdate(double *_trans)
//{
//	double Point1[3],Point2[3];
//	int i;
//		for(i=0;i<3;i++)
//			Point1[i]=_trans[i+1];
//		for(i=0;i<3;i++)
//			Point2[i]=-_trans[i+4]*deg;
//
//
//		ObjectActor->SetOrientation(Point2);
//		ObjectActor->SetPosition(Point1);
//
//		VWidget->Interactor->repaint();
//
//}

void SymmetryPlanesFinder::Optimize()
{
	//emit this->SignalSendNewRawDataSet(sssData);
	//double Tolerance=1e-5;
	int Iterations=0;
	double FValue;

	//DistFunction *Fun=new DistFunction(transform,ModelPoints,ObjectPoints);
	double tol=1e-6;
	double trans[7]={0};
	OptimizationPowell *Optim=new OptimizationPowell(trans,6,tol,&Iterations,&FValue,this); //:) 
	//connect(Optim,SIGNAL(SignalUpdatedTrans(double *)),this,SLOT(ActorUpdate(double *)));

	Optim->StartOptimization();
	valueFUN=FValue;
	int j;
	double params1[7]={0};
	//for(j=1;j<7;j++)
	//{
	//	params1[j]=trans[j];
	//}
	//params1[1]=trans[4],
	//params1[2]=trans[5];
	//params1[3]=trans[6];
	//params1[4]=trans[1];
	//params1[5]=trans[2];
	//params1[6]=trans[3];

	//params1[4]=trans[0];
	//params1[5]=trans[1];
	//params1[6]=trans[2];

	
	
	double px0[3];
	double px1[3];
	double px2[3];
	double px3[3];
	double p00[3];
	double p01[3];
	double p02[3];
	for(j=0;j<3;j++)
	{
		p00[j]=p0[j]-rCenter[j];
		p01[j]=p1[j]-rCenter[j];
		p02[j]=p2[j]-rCenter[j];
	}

	
	double val=this->GetFunctionValue(trans);
	
	for (j = 0;j < 3;j++)
	{
		direction[j] = d01[j];
	}
	//params1[1]=direction[0]*trans[1];
	//params1[2]=direction[1]*trans[1];
	//params1[3]=direction[2]*trans[1];

	//params1[4]=trans[2];
	//params1[5]=trans[3];
	//params1[6]=trans[4];

	//transform->CalculateTrigonometricFunctions(params1);
	//transform->ApplyTransformationFloat(p00,px0,params1);
	//transform->ApplyTransformationFloat(p01,px1,params1);
	//transform->ApplyTransformationFloat(p02,px2,params1);
	
	//for(j=0;j<3;j++)
	//{
	//	p0[j]=px0[j];
	//	p1[j]=px1[j];
	//	p2[j]=px2[j];
	//}
	p0[0] = p0[0] + trans[4];
	p0[1] = p0[1] + trans[5];
	p0[2] = p0[2] + trans[6];
	



	//p1 ma byc tym zetowym
//p2 ma byc ygrekowym


	double vZ[3] = { 0,0,1 };
	double p0z[3];
	double projZ[3];
	for (j = 0;j < 3;j++)
	{
		p0z[j] = p0[j] + vZ[j];
	}
	plane->ProjectPoint(p0z, projZ);
	for (j = 0;j < 3;j++)
	{
		projZ[j] -= p0[j];
		vZ[j] = projZ[j];
	}
	normV(projZ);
	for (j = 0;j < 3;j++)
	{
		p1[j] = p0[j] + projZ[j];
	}
	double vY[3];
	CROSS(vY, vZ,direction);
	for (j = 0;j < 3;j++)
	{
		p2[j] = p0[j] + vY[j];
	}

///*for(j=0;j<3;j++)
//{
//	px3[j]+=px0[j];
//}*/

	for(j=0;j<7;j++)
	{
		finalParams[j]=trans[j];
	}

	delete Optim;
 
 
 /*
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
	delete []y;*/
	
	
}
