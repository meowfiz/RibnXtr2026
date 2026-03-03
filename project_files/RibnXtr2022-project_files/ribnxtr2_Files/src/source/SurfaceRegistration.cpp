#include "SurfaceRegistration.h"

EuclideanDistFunctionWithDerivs::EuclideanDistFunctionWithDerivs(DistanceMap *_distancemap,Transform *_transform)
{
	distancemap=_distancemap;
	transf=_transform;

	derivs=new Derivatives();
	rx1=ry1=rz1=DistX=DistY=DistZ=0;


	normalize = false;
	normalizeVal = distancemap->GetMaximumDistance();
	if (normalizeVal > 0) normalizeVal = 1.0 / normalizeVal;


}
double EuclideanDistFunctionWithDerivs::GetFunctionValue(double i,double *Params)
{
	double x1,y1,z1;
	double Distance;
	double drx1,dry1,drz1;
	transf->ApplyTransformationFloat(Round(i),Params,x1,y1,z1,drx1,dry1,drz1);
	rx1=drx1;ry1=dry1;rz1=drz1;



	distancemap->GetDistanceValues(x1,y1,z1,Distance,DistX,DistY,DistZ);
	if (normalize == true)
	{
		rx1 = normalizeVal * rx1;
		ry1 = normalizeVal * ry1;
		rz1= normalizeVal * rz1;
		DistX = normalizeVal * DistX;
		DistY = normalizeVal * DistY;
		DistZ = normalizeVal * DistZ;
		Distance = normalizeVal * Distance;

	}
	return Distance;

}
void EuclideanDistFunctionWithDerivs::CalculateDerivative(double *Deriv,double *Params)
{
	derivs->CalculateDerivatives(Deriv,rx1,ry1,rz1,DistX,DistY,DistZ,Params);

}
void EuclideanDistFunctionWithDerivs::PrecalculateVariables(double *Params)
{
	derivs->CalculateTrigonometricParams(Params);
	transf->CalculateTrigonometricFunctions(Params);
}
	



SurfaceRegistration::SurfaceRegistration(PolygonSurfaceData *ModelSurfaceData,PolygonSurfaceData *ObjectSurfaceData,
										 DistanceMap *_Distance,double *_trans,double *Origin,VtkWidget *_VWidget,int Percent,bool BoundingboxOnly)
{



	
	VWidget=_VWidget;
	distance=_Distance;
	ModelPolyData=ModelSurfaceData->GetVPolyData();
	ObjectPolyData=ObjectSurfaceData->GetVPolyData();
	ObjectCuttingEdgeActor=ObjectSurfaceData->GetCuttingEdgeActor();
	ObjectActor=ObjectSurfaceData->GetVActor();
	Matrix=ObjectSurfaceData->GetMatrix();
	trans=_trans;
	precalculate=false;

//	ArithmeticCenter=false;
	RotateCenter = new double[3];
	for(int i=0;i<3;i++)
	{
		RotateCenter[i]=Origin[i];
	}

	transform=new Transform(ObjectSurfaceData,RotateCenter,-1);
	transform->CreateIndexedArray(ModelSurfaceData->GetPScale()[0],ModelSurfaceData->GetPScale()[1],ModelSurfaceData->GetPScale()[2]);


	
	if (BoundingboxOnly==true)
	{


		int np=transform->NPoint;
		/// pozbieramy distance dla wszystkich pktow 
		
		double  x1,y1,z1;
		int i;

		double rx1,ry1,rz1;

		int NumberOfPointsInCuboid=0;

		//IntDoubleStruct	**tempdouble=new IntDoubleStruct*[np];
	
	
		int Nx=distance->GetNx();
		int Ny=distance->GetNy();
		int Nz=distance->GetNz();

		transform->CalculateTrigonometricFunctions(trans);
		
		
		double *TempFloatArray=transform->IndexedArray;
		double *NewIndexedArray= new double [np*3];

		int ii=0;
		int tt=0;


		// tworze nowa tablice z punktami ktore wpadaja w cuboid
		double drx1,dry1,drz1;
		for (i=1;i<=np;i++)
		{
			transform->ApplyTransformationFloat(i,trans,x1,y1,z1,drx1,dry1,drz1);
			x1-=distance->GetX0M();
			y1-=distance->GetY0M();
			z1-=distance->GetZ0M();

			//	distance->GetDistanceValues(x1,y1,z1,Distance,DistX,DistY,DistZ);

			if ((x1>=0)&&(x1<Nx)&&(y1>=0)&&(y1<Ny)&&(z1>=0)&&(z1<Nz))
			{
			
	
				
				tt=3*i;
				NewIndexedArray[ii++]=TempFloatArray[tt++];
				NewIndexedArray[ii++]=TempFloatArray[tt++];
				NewIndexedArray[ii++]=TempFloatArray[tt];

				
				NumberOfPointsInCuboid++;
				
			}

			
	
		}



		///	teraz musze ta tablice skrocic

		double *NewIndexedArray2= new double [(NumberOfPointsInCuboid+1)*3];
		memcpy(NewIndexedArray2,NewIndexedArray,NumberOfPointsInCuboid*3*sizeof(double));
	
		delete []NewIndexedArray;

		// i koncoweczka

		transform->IndexedArray=NewIndexedArray2;
		transform->NPoint=NumberOfPointsInCuboid-1;
		delete []TempFloatArray;

	}
	
	if (Percent<100)
	{
		int np=transform->NPoint;
		/// pozbieramy distance dla wszystkich pktow 
		
		double  x1,y1,z1;
		int i;
		double Distance;
		double DistX,DistY,DistZ,rx1,ry1,rz1;

		IntDoubleStruct	**tempdouble=new IntDoubleStruct*[np];
	
	

		transform->CalculateTrigonometricFunctions(trans);

		double drx1,dry1,drz1;
		bool normalize = false;
		double normalizeVal = _Distance->GetMaximumDistance();
		if (normalizeVal > 0) normalizeVal = 1.0 / normalizeVal;
		for (i=1;i<=np;i++)
		{


			transform->ApplyTransformationFloat(i,trans,x1,y1,z1,drx1,dry1,drz1);
			distance->GetDistanceValues(x1,y1,z1,Distance,DistX,DistY,DistZ);

			if (normalize == true)
			{
				Distance *= normalizeVal;
			}


			tempdouble[i-1]=new IntDoubleStruct;
			tempdouble[i-1]->IntVar=i-1;
			tempdouble[i-1]->DoubleVar=Distance;



		}
		

	

		/// posortujemy wg distance

		QSortIntDouble(tempdouble,0,np-1);

		/// stworzymy nowa indexed array z percentem najblizszych :) i madafakin git


	
		int NewNr=Round((double)(Percent*np)/100.0); //POPRAW TO ZEBY BYLO LADNIEJ!!!!!


		double *TempFloatArray=transform->IndexedArray;

		double *NewIndexedArray= new double [(NewNr+1)*3];

		int tt;
		int ii=0;
		
	//		FILE *Do_opa;
	//	Do_opa=fopen("c:\\Do_opaSS2.txt","a");
		
		
	


		for (i=0;i<NewNr;i++)
		{

			
			tt=(tempdouble[i]->IntVar)*3;
			NewIndexedArray[ii++]=TempFloatArray[tt++];
			NewIndexedArray[ii++]=TempFloatArray[tt++];
			NewIndexedArray[ii++]=TempFloatArray[tt];

	//		fprintf(Do_opa,"%d %lf %lf %lf\n",i,NewIndexedArray[i*3],NewIndexedArray[i*3+1],NewIndexedArray[i*3+2]);

		
		}

//		fclose(Do_opa);

		transform->IndexedArray=NewIndexedArray;




	
		transform->NPoint=NewNr;
		delete []TempFloatArray;
	}




	GraphXResolution=74;
	GraphYResolution=74;
	ROTT=15.0;
	TRANST=15.0;


/*
		FILE *Do_opa;
		Do_opa=fopen("c:\\Do_opaSS.txt","a");
		for (i=0;i<transform->NPoint;i++)
		{

		

			fprintf(Do_opa,"%d  %lf %lf %lf\n",i,transform->IndexedArray[i*3],transform->IndexedArray[i*3+1],transform->IndexedArray[i*3+2]);


		}
			fclose(Do_opa);
*/
}

//--------------------------------------------------------------------

SurfaceRegistration::~SurfaceRegistration()
{

}

//--------------------------------------------------------------------

void SurfaceRegistration::ActorUpdate()
{
	double Point1[3],Point2[3];
	int i;
		for(i=0;i<3;i++)
			Point1[i]=trans[i+1];
		for(i=0;i<3;i++)
			Point2[i]=-trans[i+4]* to_deg;


		ObjectActor->SetOrientation(Point2);
		ObjectActor->SetPosition(Point1);

		if (ObjectCuttingEdgeActor!=NULL)
		{
			ObjectCuttingEdgeActor->SetOrientation(Point2);
			ObjectCuttingEdgeActor->SetPosition(Point1);
		}
		VWidget->Interactor->repaint();

}

//--------------------------------------------------------------------






double SurfaceRegistration::GetEnergy(double *ta)
{

	double  x1,y1,z1;
	int i;
	double Distance;
	double DistX,DistY,DistZ,rx1,ry1,rz1;
	double drx1,dry1,drz1;
	double Energy=0;

	transform->CalculateTrigonometricFunctions(ta);


	for (i=1;i<=transform->NPoint;i++) //noewe 18.04.2004 =
	{

		transform->ApplyTransformationFloat(i,ta,x1,y1,z1,drx1,dry1,drz1);
		//rx1=drx1;ry1=dry1;rz1=drz1;
		distance->GetDistanceValues(x1,y1,z1,Distance,DistX,DistY,DistZ);
		Energy+=Distance*Distance;
	}

	return Energy;


}

//--------------------------------------------------------------------

void SurfaceRegistration::GenerateMisregistrationGraphData()
{

//	if ((ObjectExists==true)&&(ModelExists==true)&&(Prec==true))
	{
		
			


		double XMinT=-TRANST;
		double XMaxT=TRANST;
		double YMinT=-TRANST;
		double YMaxT=TRANST;


		double XMinR=-ROTT* to_rad;
		double XMaxR=ROTT* to_rad;
		double YMinR=-ROTT* to_rad;
		double YMaxR=ROTT* to_rad;

		double XResolution=GraphXResolution;
		double YResolution=GraphYResolution;




		FILE *Do_opa;
		int i,j;
		double XX,YY;
		double XMin,YMin,XMax,YMax;
		double Energy;
	//	double MinEnergy;
	//	double MaxEnergy; 
		double *TempTrans= new double [7];
		double XRange;
		double YRange;
		double RangeStepX;
		double RangeStepY;
		int ParamX;
		int ParamY;
		int StepY=2;

		//const char *name,*name2,*name3;	

		QString filename2;
		QString filename3;
		QString txt;
	
		//name3="Misregistration graphs";

		
		QString filename = QFileDialog::getSaveFileName(0, "", "*.dat");
		if (!filename.isEmpty()) 
		{
			
			
			filename2=filename+".plt";
			//name=filename2.toLocal8Bit();
			Do_opa = fopen(filename2.toLocal8Bit(), "w");
					
			fprintf(Do_opa,"set cntrparam levels 10\n"); 
			fprintf(Do_opa,"set contour base \n");
			fprintf(Do_opa,"set cntrparam bspline \n");
			fprintf(Do_opa,"set cntrparam order 5 \n");
			fprintf(Do_opa,"set nokey \n");
			fprintf(Do_opa,"set hidden3d\n");


			fclose(Do_opa);


			for (ParamX=1;ParamX<7;ParamX++)
			{
				for (ParamY=StepY;ParamY<7;ParamY++)
				{
				
					filename2=filename+"_"+QString::number(ParamX)+"_"+QString::number(ParamY)+".dat";
					//name=filename2.toLocal8Bit();
				
			
				if (ParamX<4) 
				{
					XMin=XMinT;
					XMax=XMaxT;
				}
				else
				{
					XMin=XMinR;
					XMax=XMaxR;
				}

				if (ParamY<4) 
				{
					YMin=YMinT;
					YMax=YMaxT;

				}
				else
				{
					YMin=YMinR;
					YMax=YMaxR;
				}


				XRange=XMax-XMin;
				YRange=YMax-YMin;
				RangeStepX = XRange / (double) XResolution;
				RangeStepY = YRange / (double) YResolution;
				
					for(i=1;i<7;i++)
						TempTrans[i]=trans[i];


					Do_opa = fopen(filename2.toLocal8Bit(), "w");
					for(i=0;i<=XResolution;i++)
					{	
						XX=XMin +i*RangeStepX;
						TempTrans[ParamX]=trans[ParamX]+XX;
						fprintf(Do_opa,"\n# i = %d \n",i);

						for(j=0;j<=YResolution;j++)
						{
						
							YY=YMin +j*RangeStepY;

							
							TempTrans[ParamY]=trans[ParamY]+YY;

							Energy = GetEnergy(TempTrans);
							fprintf(Do_opa,"%lf\n",Energy);      

						}
					}
					fclose(Do_opa);
			
					filename2=filename+".plt";
					//name=filename2.toLocal8Bit();
					Do_opa = fopen(filename2.toLocal8Bit(), "a");
					
					fprintf(Do_opa,"\n");


					
					txt=TransSymbolName(ParamX)+"-"+TransSymbolName(ParamY);
					//name2=txt.toLocal8Bit();

					
					fprintf(Do_opa, "set title '%s %s' -10,-1.5 \n", "Misregistration graphs", txt.toLocal8Bit());
					
					txt=TransSymbolName(ParamY);
					//name2=txt.toLocal8Bit();
					fprintf(Do_opa, "set xlabel '%s'\n", txt.toLocal8Bit());

					txt=TransSymbolName(ParamX);
					//name2=txt.toLocal8Bit();
					fprintf(Do_opa, "set ylabel '%s'\n", txt.toLocal8Bit());

					if (ParamX<4) 
						fprintf(Do_opa,"set ytics ('%4.2f' 0, '0' %4.2f, '%4.2f' %lf)\n",XMin,XResolution/2.0,XMax,XResolution); 
					else
						fprintf(Do_opa,"set ytics ('%4.2f' 0, '0' %lf, '%4.2f' %lf)\n",XMin* to_deg,XResolution/2.0,XMax,XResolution);
					

					if (ParamY<4) 
						fprintf(Do_opa,"set xtics ('%4.2f' 0, '0' %4.2f, '%4.2f' %lf)\n",YMin,YResolution/2.0,YMax,YResolution);
					else
						fprintf(Do_opa,"set xtics ('%4.2f' 0, '0' %4.2f, '%4.2f' %lf)\n",YMin* to_deg,YResolution/2.0,YMax* to_deg,YResolution);
				

					fprintf(Do_opa,"set view 55,60,1,1.5\n");
					fprintf(Do_opa,"set terminal postscript eps monochrome solid 'Times-Roman' 20\n");

					txt=TransSymbolName(ParamX)+"-"+TransSymbolName(ParamY);
					//name2=txt.toLocal8Bit();
					fprintf(Do_opa, "set output 'Plot-%s.eps' \n", txt.toLocal8Bit());
					
					filename3=filename+"_"+QString::number(ParamX)+"_"+QString::number(ParamY)+".dat";
					filename3=filename3.replace( QRegExp("/"), "\\"); 
					//name2=filename3.toLocal8Bit();

					fprintf(Do_opa, "splot '%s' with lines 6\n", filename3.toLocal8Bit());

					

				



					fclose(Do_opa);
				}
				StepY++;
			}

		}
	}


}

//------------------------------------------------

QString SurfaceRegistration::TransSymbolName(int nr)
{
	switch (nr)
	{
			case 1:
				return "Tx";
			case 2:
				return "Ty";
			case 3:
				return "Tz";
			case 4:
				return "Rx";
			case 5:
				return "Ry";
			case 6:
				return "Rz";
	}
	return " ";

}

//------------------------------------------------

void SurfaceRegistration::GenerateMinimumPaths()
{


}

//------------------------------------------------

void SurfaceRegistration::Optimize()
{

	int i;
	int Hsize= transform->NPoint+1;
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






	EuclideanDistFunctionWithDerivs *Fun=new EuclideanDistFunctionWithDerivs(distance,transform);
	//Fun->SetNormalize(true);

	OptimizationMarquardt *optim=new OptimizationMarquardt(x,y,sig,Hsize,trans,ia,Lsize,Fun);
	connect(optim,SIGNAL(Updated()),this,SLOT(ActorUpdate()));
	connect(optim,SIGNAL(Updated()),this,SIGNAL(SignalGrab3Window()));
	optim->StartOptimization();


	/*FILE *Do_opa;
		Do_opa=fopen("c:\\zenon.txt","a");
	for(i=1;i<Lsize;i++)
		fprintf(Do_opa,"%lf\n",trans[i]);

	for(i=0;i<3;i++)
		fprintf(Do_opa,"%lf\n",RotateCenter[i]);*/


	//fclose(Do_opa);

	delete optim;
	delete Fun;
	delete []ia;
	delete []sig;
	delete []x;
	delete []y; 
	
}
