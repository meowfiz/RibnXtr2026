#include "ImageSetElasticRegistration.h"


ImageSetElasticRegistration::ImageSetElasticRegistration(RawDataSet *_data,int gSizeX,int gSizeY,int _iter,int optimS,int multiResS,bool fixB,QString _filename)
{
	iter=_iter;
	filename=_filename;
	data=_data;

	gridSizeX=gSizeX;
	gridSizeY=gSizeY;
	optimStep=optimS;
	multiResStep=multiResS;
	fixedBounds=fixB;

	ModelData=NULL;
	ObjectData=NULL;
	grid=NULL;
	BSplineTransform=NULL;
	intrp=new InterpolationFunction2DBilinear();
	stopOptimization=false;
	transx = NULL;


}
//----------------------------------------------------------
ImageSetElasticRegistration::~ImageSetElasticRegistration()
{
	if (grid!=NULL) delete grid;

}
//----------------------------------------------------------
RawDataSet *ImageSetElasticRegistration::CreateArrowDataSet(QString filename,int step,int mode,unsigned char*mask)
{
	if(!filename.isEmpty())
	{
		int i,j,k;
		RawDataSet *d1=new RawDataSet();
		d1->Copy(*data,false);
		d1->ZeroQ();

		//wczytac ity grid;
		if (BSplineTransform!=NULL) delete BSplineTransform;
		if (grid!=NULL) delete grid;
		grid=new FloatGrid3D();
		int nr=grid->GetNumberOfGrids(filename);
		int nx=data->GetNx();
		int ny=data->GetNy();
		BSplineTransform=new BSplineGridTransformation(grid,nx-1,ny-1,1);


		int min=MIN(nr,data->GetNz()-1);
		double *pts=NULL;
		//double *pts2=NULL;
		int ptsNr;//,pts2Nr;
		//if (mode==1)
		{
			ptsNr=((int)((nx+1)/(step-1))*(int)((ny+1)/(step-1)))*3;
			pts=new double[ptsNr];
			memset(pts,0,ptsNr*sizeof(double));
			//	pts2Nr=((int)((nx+1)/(step-1))*(int)((ny+1)/(step-1)))*3;
			//	pts2=new double[ptsNr];
			//	memset(pts2,0,ptsNr*sizeof(double));

		}
		int pos=0;
		for (j=0;j<ny;j+=step)
			for (i=0;i<nx;i+=step)
			{
				if (!((mask!=NULL)&&(mask[i+j*nx]==0))) 
				{


					pts[pos++]=i;
					pts[pos++]=j;
				}

			}
			//create random grid
			/*	FloatGrid3D *g1,*g2,*tmpg;
			g1=new FloatGrid3D();
			grid->LoadGrid(filename,0);
			g1->Copy(*grid);

			double s=0.5;
			for(j=0;j<g1->GetNy();j++)
			for(i=0;i<g1->GetNx();i++)
			{
			g1->SetDx(i,j,0,(rand()*2.0*s)/(double)RAND_MAX-s);
			g1->SetDy(i,j,0,(rand()*2.0*s)/(double)RAND_MAX-s);

			}
			g2=new FloatGrid3D();
			g2->Copy(*g1);
			g2->ReverseField();

			*/





			//create reversed random grid


			for(k=0;k<min;k++)
			{
				grid->LoadGrid(filename,k);


				//grid->Copy(*g1);
				//grid->Clear();

				GenerateArrows(k+1,k,d1,step,mode,pts,mask);//,pts2);
				emit SendMessage(initText+" ("+QString::number(k)+"/"+QString::number(min-1)+")");

				if (stopOptimization==true) break;
				//	tmpg=g1;
				//	g1=g2;
				//	g2=tmpg;

			}

			if (pts!=NULL)
				delete []pts;
			//if (pts2!=NULL)
			//	delete []pts2;
			delete BSplineTransform;
			BSplineTransform=NULL;
			return d1;

			//tmpg=NULL;
			//if (g1!=NULL) delete g1;
			//if (g2!=NULL) delete g2;
	}
	return NULL;
}

//--------------------------------------------------------------
void ImageSetElasticRegistration::GenerateArrows(int dest,int src,RawDataSet *d,int step,int mode, double *points,unsigned char*mask)//,double *points2)
{
	int i,j,k;

	int pos=0;

	unsigned short *Tab=data->GetDataArray()+data->GetSliceSize()*src;
	double Point[2];
	Point[0]=0;
	Point[1]=0;

	unsigned short value;
	// wyczyszcze zerowym
	// 
	unsigned short *Q=d->GetDataArray()+d->GetSliceSize()*dest;
	unsigned short min=0;



	int nx=data->GetNx();
	int ny=data->GetNy();

	///2.11.2006 - zmiana teraz to normalna transformacja

	//Grid->ReverseField();

	//	grid->ReverseField();

	int length;
	double factor=0.8;
	double p0[2];
	double pa[2];
	double pb[2];
	double l;
	double len=2;
	double pointS[2];
	double delta[2];
	double tol=1e-7;

	//FILE *Do_opa;	Do_opa=fopen("c:\\pox.txt","w");	fprintf(Do_opa,"%d\n",dest);	fclose(Do_opa);
	for (j=0;j<ny;j+=step)
		for (i=0;i<nx;i+=step)
		{
			if (!((mask!=NULL)&&(mask[i+j*nx]==0))) 
			{
				//FILE *Do_opa;	Do_opa=fopen("c:\\pox.txt","a");	fprintf(Do_opa,"%d %d \n",i,j);	fclose(Do_opa);
				pointS[0]=points[pos];
				pointS[1]=points[pos+1];
				if ((pointS[0]>=0)&&(pointS[0]<nx-1.0)&&
					(pointS[1]>=0)&&(pointS[1]<ny-1.0))
				{
					BSplineTransform->TransformPoint2D(pointS[0],pointS[1],Point);
					//pa[0]=i;pa[1]=j;pa[2]=k;		
					//BSplineTransform->TransformPoint(pa,Point);

					//Point[0]=2*pointS[0]-Point[0];	
					//	Point[1]=2*pointS[1]-Point[1];	

					if (points!=NULL)
					{

						delta[0]=(Point[0]-pointS[0]);
						delta[1]=(Point[1]-pointS[1]);
						//if (fabs(delta[0])>tol)
						//{
						//	tol=tol;
						//}
						/*if (mode==1)
						{
						if (fabs(delta[0])>0.5)
						points[pos]+=delta[0];
						if (fabs(delta[1])>0.5)
						points[pos+1]+=delta[1];
						}
						else*/
						{
							//	if (fabs(delta[0])>0.5)
							//		points[pos]+=delta[0];
							//	else
							//		points[pos]+=delta[0]/5;

							//	if (fabs(delta[1])>0.5)
							//		points[pos+1]+=delta[1];
							//	else
							//		points[pos+1]+=delta[1]/5;
						}

						points[pos]+=delta[0];
						points[pos+1]+=delta[1];
					}


					if( points!=NULL)
					{
						Point[0]=points[pos];
						Point[1]=points[pos+1];
					}
					//FILE *Do_opa;	Do_opa=fopen("c:\\pox.txt","a");	fprintf(Do_opa,"[%lf %lf] [%lf %lf]\n",pointS[0],pointS[1],Point[0],Point[1]);	fclose(Do_opa);
					if (mode==0)	
						length=sqrt((Point[0]-pointS[0])*(Point[0]-pointS[0])+(Point[1]-pointS[1])*(Point[1]-pointS[1]))*100;
					else
						length=sqrt((Point[0]-i)*(Point[0]-i)+(Point[1]-j)*(Point[1]-j))*100;

					if ((Point[0]>=0)&&(Point[0]<nx-1.0)&&
						(Point[1]>=0)&&(Point[1]<ny-1.0))
					{

						//value=intrp->GetInterpolatedValue(data,Point[0],Point[1],src);
						if (mode==0)
						{

							DrawLineUSHORT(Q,nx,ny,pointS[0], pointS[1], Point[0], Point[1], length);

							//FILE *Do_opa;	Do_opa=fopen("c:\\pox.txt","a");	fprintf(Do_opa,"drawn\n");	fclose(Do_opa);
							//proba rysowania strzalki

							p0[0]=pointS[0]+factor*(Point[0]-pointS[0]);
							p0[1]=pointS[1]+factor*(Point[1]-pointS[1]);
						}
						else
						{
							DrawLineUSHORT(Q,nx,ny,i, j, Point[0], Point[1], length);
							p0[0]=i+factor*(Point[0]-i);
							p0[1]=j+factor*(Point[1]-j);
						}
						// wetkor prostopadly
						pa[0]=-(Point[1]-p0[1]);
						pa[1]=(Point[0]-p0[0]);
						//normalizacja
						l=sqrt(pa[0]*pa[0]+pa[1]*pa[1]);
						if (l>0)
						{
							pa[0]/=l;
							pa[1]/=l;

						}
						pa[0]*=len;
						pa[1]*=len;

						pb[0]=-pa[0]+p0[0];
						pb[1]=-pa[1]+p0[1];
						pa[0]+=p0[0];
						pa[1]+=p0[1];

						if ((pa[0]>=0)&&(pa[0]<nx-1.0)&&(pa[1]>=0)&&(pa[1]<ny-1.0))
							if ((pb[0]>=0)&&(pb[0]<nx-1.0)&&(pb[1]>=0)&&(pb[1]<ny-1.0))
							{
								DrawLineUSHORT(Q,nx,ny,(int)pa[0], (int)pa[1], Point[0], Point[1], length);
								DrawLineUSHORT(Q,nx,ny,(int)pb[0], (int)pb[1], Point[0], Point[1], length);
							}







							//value=sqrt((i-Point[0])*(i-Point[0])+(j-Point[1])*(j-Point[1]))*10;
					}
					else
						value=min;
				}

				pos+=2;

			}

			//Q[pos++]=value;
		}

		Q=NULL;
		Tab=NULL;


		//Grid->ReverseField()
}
//----------------------------------------------------------
RawDataSet *ImageSetElasticRegistration::TransformTest(QString filename)
{


	if(!filename.isEmpty())
	{
		int i,j;
		RawDataSet *d1=new RawDataSet();
		d1->Copy(*data,false);
		d1->ZeroQ();

		//wczytac ity grid;
		if (BSplineTransform!=NULL) delete BSplineTransform;
		if (grid!=NULL) delete grid;
		grid=new FloatGrid3D();
		int nr=grid->GetNumberOfGrids(filename);
		int nx=data->GetNx();
		int ny=data->GetNy();
		BSplineTransform=new BSplineGridTransformation(grid,nx-1,ny-1,1);


		int min=MIN(nr,data->GetNz()-1);
		for(i=0;i<min;i++)
		{
			grid->LoadGrid(filename,i);
			//grid->Clear();
			TransformImages(i+1,i,d1);

		}

		delete BSplineTransform;
		BSplineTransform=NULL;
		return d1;
	}
	return NULL;
}
//------------------
void ImageSetElasticRegistration::TransformImages(int dest,int src,RawDataSet *d)
{
	int i,j,k;

	int pos=0;

	unsigned short *Tab=data->GetDataArray()+data->GetSliceSize()*src;
	double Point[2];
	Point[0]=0;
	Point[1]=0;

	unsigned short value;
	// wyczyszcze zerowym
	// 
	unsigned short *Q=d->GetDataArray()+d->GetSliceSize()*dest;
	unsigned short min=0;



	int nx=data->GetNx();
	int ny=data->GetNy();

	//grid->ReverseField();
	/*for (j=0;j<ny;j++)
	for (i=0;i<nx;i++)
	{
	value=Tab[pos++];
	BSplineTransform->TransformPoint2D(i,j,Point);

	if ((Point[0]>=0)&&(Point[0]<nx-1.0)&&
	(Point[1]>=0)&&(Point[1]<ny-1.0))
	d->SetVoxelValue(Round(Point[0]),Round(Point[1]),dest,value);
	//		Data->SetVoxelValue(Point[0],Point[1],Point[2],Interpolation3Linear(ObjectData,Point[0],Point[1],Point[2]));
	}
	Tab=NULL;*/


	//proba odwrotnej!!! 
	///2.11.2006 - zmiana teraz to normalna transformacja
	int pa[3];
	//Grid->ReverseField();

	/*grid->ReverseField();
	*/for (j=0;j<ny;j++)
	for (i=0;i<nx;i++)
	{
		BSplineTransform->TransformPoint2D(i,j,Point);
		//pa[0]=i;pa[1]=j;pa[2]=k;		
		//BSplineTransform->TransformPoint(pa,Point);

		if ((Point[0]>=0)&&(Point[0]<nx-1.0)&&
			(Point[1]>=0)&&(Point[1]<ny-1.0))
		{
			value=intrp->GetInterpolatedValue(data,Point[0],Point[1],src);


			//value=sqrt((i-Point[0])*(i-Point[0])+(j-Point[1])*(j-Point[1]))*10;
		}
		else
			value=min;

		Q[pos++]=value;
	}

	Q=NULL;
	Tab=NULL;

	//Grid->ReverseField()
}
//---------------------------------------------------------
void ImageSetElasticRegistration::ApplyRegistration()
{
	if(!filename.isEmpty())
	{
		int i,j,k;
		for(i=0;i<data->GetNz()-1;i++)
		{

			RegisterImages(i,i+1);
		}
	}
}
//------------------------------------------------------------
void ImageSetElasticRegistration::RegisterImages(int dest,int src)
{

	if (grid!=NULL) delete grid;
	if (BSplineTransform!=NULL) delete BSplineTransform;
	grid=new FloatGrid3D(gridSizeX,gridSizeY,1);
	//SetSizeVariables(grid);

	BSplineTransform=new BSplineGridTransformation(grid,nnx,nny,1);

	bool fixBounds=fixedBounds;
	GetFValue=&ImageSetElasticRegistration::GetSqDistEnergy;
	GetDerivatives=&ImageSetElasticRegistration::CalculateSingleNodDerivative;

	//////////////////
	//bool fixBounds=true;


	/////////////////

	int i,j,k,l,m;

	double derivs[4];

	////////////////////////////
	if (transx!=NULL) delete []transx;
	transx=new double[4];

	double temptrans[4];
	double Tolerance;//10e-4;
	int Iterations=0;
	double FValue=0;

	int NumberOfOptimParams=2;
	nrOfParams=NumberOfOptimParams;

	//OptimizationPowell *Optim=NULL;

	//FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelas.txt","w");	fprintf(Do_opa,"");	fclose(Do_opa);

	double e,e1;

	//////////////////////////////////

	int st=(fixBounds==true)?1:0;
	QString t;
	iterx=0;
	int ii,jj,kk;
	int pos;

	double testtol;
	//Function *f=new TempFunction();
	OptimizationDFPowell *Optim=NULL;

	//zabezpieczenie zeby rozmiar gridu nie byl wiekszy od zbioru
	int min=MIN(nnx,nny);
	int maxgrid=MAX(nx,ny);
	int multimax=(maxgrid)*pow((double)2,multiResStep);

	//-----------------------------------
	//----------------------------------
	int optimStepX=optimStep;
	optimStep=1;
	double threshold=1.0;
	double ss=1.0;

	//double gSize=4;
	//UpdateData(gSize,gSize,gSize);
	//multiressteps=1;
	//Do_opa=fopen("c:\\kurde.txt","a");fprintf(Do_opa,"%s - %s\n",ModelData->GetRawDataSetName().toLocal8Bit(),ObjectData->GetRawDataSetName().toLocal8Bit());fclose(Do_opa);


	minP=1.7;
	maxP=1.7;
	alpha=1000.0;
	Tolerance=1e-5;
	if (ObjectData!=NULL)
		delete ObjectData;

	ObjectData=ScaleImageFromDataSet(data,src,1.0);
	RawDataSet rData=*ObjectData;
	RawDataSet *origObjData=&rData;
	ModelData=ScaleImageFromDataSet(data,dest,1.0);
	ObjectData->CalculateHistogram();
	QString ttt=" ("+QString::number(src)+"/"+QString::number(data->GetNz()-1)+")";
	for(m=0;m<multiResStep;m++)	
	{
		if (optimStepX==1)
		{
			delete ObjectData;
			ObjectData=ScaleImageFromDataSet(data,src,1.0);
		}
		else
		{
			GaussianSmoothingFiltering(0.5, optimStepX*2+1, origObjData,ObjectData);
		}
		//optimStepX
		//optimStepX=scaleTab[m];
		//przeskalowac zbior i ustawic paramtetry dla 
		/*	if (ModelData!=NULL)
		delete ModelData;

		ModelData=ScaleImageFromDataSet(data,dest,1.0/optimStepX);
		ModelData->CalculateHistogram();*/


		//!!!!!!!!!!!!!!!!11

		BSplineTransform->SetupForwardAndBackwardScalingFactors(
			ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz(),
			ObjectData->GetNx(),ObjectData->GetNy(),ObjectData->GetNz());
		SetSizeVariables(grid);
		t=QString::number((nx-2*st)*(ny-2*st)*iter);
		pos=0;
		smallStepX=BSplineTransform->GetGxDivNx()*ss;
		smallStepY=BSplineTransform->GetGyDivNy()*ss;
		//smallStepZ=BSplineTransform->GetGzDivNz()*ss;


		maxFactor=MAX(ModelData->GetMaxValue(),ObjectData->GetMaxValue());
		maxFactor=(maxFactor>0)?255.0/maxFactor:1.0;


		for(l=0;l<iter;l++)	
			for (j=st;j<ny-st;j++)
				for (i=st;i<nx-st;i++)
				{
					//i=1;j=1;k=1;
					if ((stopOptimization==false))	
					{
						emit SendMessage(initText+ttt+" ("+QString::number(++pos)+"/"+t+") "+" multires:("+ QString::number(m+1)+"/"+QString::number(multiResStep)+")");
						SetCurrentNode(i,j,0);
						GetTransFromCurrentNode(transx,grid);

						e=GetFunctionValue(transx);
						if (e>threshold)
						{
							iterx=0;
							temptrans[1]=transx[1];	temptrans[2]=transx[2];	//temptrans[3]=transx[3];
							Optim=new OptimizationDFPowell(transx,NumberOfOptimParams,Tolerance,&Iterations,&FValue,this); //:) 
							Optim->StartOptimization();
							delete Optim;


							SetTransToCurrentNode(transx,grid);


							//			Do_opa=fopen("c:\\powellvolelas.txt","a");fprintf(Do_opa,"%d\t%d\n%lf\t%lf\t(s: %lf f: %lf) (%d)\n",i,j,transx[1],transx[2],e,FValue,iterx);fclose(Do_opa);

						}
					}
				}

				if (stopOptimization==false)
					if (multiResStep-m>1)
					{
						//przebudowac
						//UpdateData(gridSize[m+1],gridSize[m+1],gridSize[m+1]);
						UpdateData(nx*2-1,ny*2-1,1);
						//delete TempGrid;
						//FloatGrid3D *TempGrid=new FloatGrid3D();
						//TempGrid->Copy(*Grid);
						//UpdateData(nx+1,ny+1,nz+1);
						if (optimStepX>=2) optimStepX/=2;
						Tolerance*=50.0;
						minP-=0.3;
						maxP-=0.3;
					}

	}
	grid->SaveGrid(filename,true);//save to multigrid

	if (ModelData!=NULL)
		delete ModelData;
	if (ObjectData!=NULL)
		delete ObjectData;

	ModelData=NULL;
	ObjectData=NULL;


	//OrigGrid->Copy(*Grid);
	if (grid!=NULL) delete grid;
	if (BSplineTransform!=NULL) delete BSplineTransform;
	grid=NULL;
	BSplineTransform=NULL;

	stopOptimization=false;



}
//-------------------------------------------------------------------
double	ImageSetElasticRegistration::GetFunctionValue(double *params)
{
	iterx++;

	//double e=GetSqDistEnergy(NodX,NodY,NodZ);
	double e=(this->*GetFValue)(NodX,NodY,params);
	//	emit SendMessage(initText+" ("+QString::number(iterx)+"="+QString::number(e)+") ");
	//	FILE *Do_opa;	Do_opa=fopen("c:\\fval.txt","a");	fprintf(Do_opa,"%d %lf\t[%lf %lf ]\n",iterx,e,params[1],params[2]);	fclose(Do_opa);

	return e;
}
//-------------------------------------------------------------------
void	ImageSetElasticRegistration::CalculateDerivative(double *Deriv,double *Params)
{
	if ((Deriv!=NULL)&&(Params!=NULL))
	{
		(this->*GetDerivatives)(Deriv,Params);
		//CalculateTotalDerivative(Deriv,Params);
		//CalculateSingleNodDerivative(Deriv,Params);
		//		emit SendMessage(initText+" ("+QString::number(iterx)+"="+"DERIV"+") ");
		//			FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasB.txt","a");	fprintf(Do_opa,"Deriv [%lf %lf]\n", Deriv[1],Deriv[2]);	fclose(Do_opa);
	}

}
//-------------------------------------------------------------------
double	ImageSetElasticRegistration::GetSqDistEnergy(int x,int y,double *params)
{
	SetTransToCurrentNode(params,grid);
	double def[4];
	double def0[4];
	int i,j,l;
	int pos;

	//double energy=0.0;
	double energy[2];energy[0]=0;energy[1]=0;
	double tttol=10e-7;
	double der[6];
	der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;
	count=0;
	//////


	for(j=posYstart;j<posYstop;j+=optimStep)
		for(i=posXstart;i<posXstop;i+=optimStep)
			//	AddNewEnergyPoint(i,j,k,count,energy,NULL);
			AddNewEnergyPointWithoutDerivs(i,j,count,energy);






	double f= energy[0]+alpha*energy[1];
	//if (count >0)	if (f!=0) f/=(double)count;
	f= (count >0)?f/=(double)count:INT_MAX;

	//if ((NodX==2)&&(NodY==2)&&(NodZ==2))
	//{FILE *Do_opa;Do_opa=fopen("c:\\powellvolelasFAILURE.txt","a");fprintf(Do_opa,"%d %d (%lf %lf %lf)(%lf %lf)\n",iterx,count,params[1],params[2],params[3],energy[0],energy[1]);fclose(Do_opa);}
	//if (count>0)
	//{FILE *Do_opa;Do_opa=fopen("c:\\textXX.txt","a");fprintf(Do_opa,"[ %lf \t %lf ]( %lf \t %lf  , %d) = %lf \n ",energy[0]/count,energy[1]*alpha/count,params[1],params[2],count,f);fclose(Do_opa);}
	//FILE *Do_opa;Do_opa=fopen("c:\\powellvolelasFAILURE.txt","a");fprintf(Do_opa,"%d\t%d\t(%lf\t%lf\t%lf)\t(%lf\t%lf)\t%lf\t[%lf]\n",iterx,count,params[1],params[2],params[3],energy[0],energy[1],energy[1]/energy[0],f);fclose(Do_opa);
	return f;
}
//----------------------------------
double  ImageSetElasticRegistration::GetGradI(int i,int j,double smallStep,double fvalue,int view)
{
	//fkcja liczy gradient w zadanym kierunku osi glownych - view
	//dla danego pktu zbioru danych Model Data i danego noda
	//smallStep jest to dx=dy,dz dla liczenia gradientu
	double plus, minus,value=0;
	double Point[3];
	double x,y;
	switch (view)
	{
	case 0:		x=smallStep;y=0;	break;
	case 1:		x=0;y=smallStep;	break;
	}

	BSplineTransform->TransformPont2D(i,j,Point,x,y,NodX,NodY,NodZ);
	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&
		(Point[1]>=0)&&(Point[1]<my1-1)))
		return 0;
	plus=intrp->GetInterpolatedValue(ObjectData,Point[0],Point[1], 0)*maxFactor;//-fvalue;

	BSplineTransform->TransformPont2D(i,j,Point,-x,-y,NodX,NodY,NodZ);
	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&
		(Point[1]>=0)&&(Point[1]<my1-1)))
		return 0;
	minus=intrp->GetInterpolatedValue(ObjectData,Point[0],Point[1], 0)*maxFactor;//-fvalue;

	value=0.5*(plus-minus);

	return value;

};

//---------------------------------------
void	ImageSetElasticRegistration::CalculateSingleNodDerivative(double *Deriv,double *Params)
{
	int pos=1;
	double der[6];
	int st=(fixBounds==true)?1:0;
	int count=0;
	double energy[2];energy[0]=0;energy[1]=0;
	//double alpha=3;
	double cn;
	int i,j,ii,jj;

	der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;
	energy[0]=0;	energy[1]=0; count=0;
	//--------------------------------------
	// tu policze pochodne po x,y,z dla danego noda

	//GetTransFromCurrentNode(transx,Grid);
	//FILE *Do_opa;	Do_opa=fopen("c:\\dd.txt","w");	fprintf(Do_opa,"start\n");	fclose(Do_opa);

	//SetCurrentNode(i,j,k);
	for(jj=posYstart;jj<posYstop;jj+=optimStep)
		for(ii=posXstart;ii<posXstop;ii+=optimStep)
		{
			//FILE *Do_opa;	Do_opa=fopen("c:\\dd.txt","a");	fprintf(Do_opa,"%lf %lf %lf %lf %lf %lf\n",der[0],der[1],der[2],der[3],der[4],der[5]);	fclose(Do_opa);
			AddNewEnergyPoint(ii,jj, count,energy,der);
		}

		//				der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;
		//CalculateDerivativesAtPoint(i,j,k,der,drvTempTab,count, bTr1,bTr2);
		//---------------------------------------

		cn=(count>0)?1.0/count:1.0;

		Deriv[1]=2*cn*(der[0]+alpha*der[2]);
		Deriv[2]=2*cn*(der[1]+alpha*der[3]);

}
RawDataSet *ImageSetElasticRegistration::ScaleImageFromDataSet(RawDataSet *data,int nr, double factor)
{

	if (factor>0)
	{
		int newNx=data->GetNx()*factor;
		int newNy=data->GetNy()*factor;
		double newSx=data->GetXscale()/factor;
		double newSy=data->GetYscale()/factor;

		RawDataSet *dataX=new RawDataSet(newNx,newNy,1,newSx,newSy,1);

		double factX=(double)data->GetNx()/(double)newNx;
		double factY=(double)data->GetNy()/(double)newNy;


		int i,j,k;
		double x,y,z;
		unsigned short *Q=dataX->GetDataArray();
		for(j=0;j<newNy;j++)
			for(i=0;i<newNx;i++)
			{
				x=(int)((double)i*factX);
				y=(int)((double)j*factY);
				*(Q++)=data->GetVoxelValue(x,y,nr);
			}

			return dataX;
	}
	else return NULL;
}
//-------------------------------------
void ImageSetElasticRegistration::AddNewEnergyPoint(int i,int j, int &count,double *energy,double *drv)
{

	int pa[2];pa[0]=i;pa[1]=j;
	int nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=0;
	double ddDdP[6];
	double ddDdPL[2];
	double ddDdQ;
	double ddDPdQ[2];
	double Point[2];

	//double a=0;

	BSplineTransform->TransformPoint2D(pa,Point,true,nodPos,ddDdP,ddDdPL,&ddDdQ,ddDPdQ);

	double m=0;
	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1))
	{	
		m=intrp->GetInterpolatedValue(ObjectData,Point[0],Point[1],0)*maxFactor;
		double a=ModelData->GetVoxelValue(i,j,0)*maxFactor-m;
		double deriv=-a*ddDdQ;

		if (drv!=NULL)
		{
			if (deriv!=0)
			{
				if ((calculateDerivs)||(drv!=NULL))
				{
					double gradIx=GetGradI(i,j,smallStepX,m,0); 
					double gradIy=GetGradI(i,j,smallStepY,m,1); 

					drv[0]+=deriv*gradIx;
					drv[1]+=deriv*gradIy;

				}
			}

			drv[2]+=(ddDdP[0]*ddDPdQ[0]+ddDdP[2]*ddDPdQ[1]);
			drv[3]+=(ddDdP[1]*ddDPdQ[0]+ddDdP[3]*ddDPdQ[1]);

		}
		energy[0]+=a*a;	
		energy[1]+=(ddDdPL[0]+ddDdPL[1]);	
		count++;
	}

	if (drv[0]>0)
	{
		count=count;
	}
	//FILE *Do_opa;	Do_opa=fopen("c:\\dd.txt","a");	fprintf(Do_opa,"%lf %lf %lf %lf %lf %lf\n",der[0],der[1],der[2],der[3],der[4],der[5]);	fclose(Do_opa);


}
//-------------------------------
void ImageSetElasticRegistration::AddNewEnergyPointWithoutDerivs(int i,int j, int &count,double *energy)
{
	double Point[3];
	int pa[3];pa[0]=i;pa[1]=j;
	int nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=0;
	double ddDdPL[2];
	BSplineTransform->TransformPoint2D(pa,Point,nodPos,ddDdPL);
	double m=0;
	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1))
	{	
		m=intrp->GetInterpolatedValue(ObjectData,Point[0],Point[1],0)*maxFactor;
		double a=ModelData->GetVoxelValue(i,j,0)*maxFactor-m;
		if (a!=0)
		{
			a=a;
		}
		energy[0]+=a*a;	
		energy[1]+=(ddDdPL[0]+ddDdPL[1]);	
		count++;
	}


}
//--------------------------------
void ImageSetElasticRegistration::UpdateData(int gridNx, int gridNy, int gridNz)
{

	FloatGrid3D *tempGrid=new FloatGrid3D(gridNx,gridNy,gridNz);
	BSplineTransform->ScaleGrid2D(tempGrid);

	delete BSplineTransform;
	SetSizeVariables(tempGrid);
	BSplineTransform=new BSplineGridTransformation(tempGrid,nnx,nny,nnz);

	delete grid;
	grid=tempGrid;



}
//-----------------------------------
void	ImageSetElasticRegistration::SetSizeVariables(FloatGrid3D *_grid)
{

	nx=_grid->GetNx();			ny=_grid->GetNy();			nz=_grid->GetNz();

	if (ModelData==NULL)
	{	nx1=ModelData->GetNx();		ny1=ModelData->GetNy();		nz1=ModelData->GetNz();	}
	else
	{	nx1=ModelData->GetNx();	ny1=ModelData->GetNy();	nz1=ModelData->GetNz();}

	mx1=ObjectData->GetNx();		my1=ObjectData->GetNy();		mz1=ObjectData->GetNz();

	fx=((nx>0)&&(nx1>0))?(double)(nx1-1)/(nx-1):1;
	fy=((ny>0)&&(ny1>0))?(double)(ny1-1)/(ny-1):1;
	fz=((nz>0)&&(nz1>0))?(double)(nz1-1)/(nz-1):1;

	fx2=fx*fx;			fy2=fy*fy;			fz2=fz*fz;
	tdx=1.0/fx;			tdy=1.0/fy;			tdz=1.0/fz;
	nnx=(double)(nx1-1);	nny=(double)(ny1-1);	nnz=(double)(nz1-1);




}
//--------------------------------
void	ImageSetElasticRegistration::SetCurrentNode(int x,int y,int z)
{

	if ((x>=0) && (x<nx))
		if ((y>=0) && (y<ny))
		{
			double a,b,c;


			NodX=x;					NodY=y;		NodZ=0;			
			a=(x-minP)*fx;			b=(y-minP)*fy;			
			posXstart=(a<0)?0:a;	posYstart=(b<0)?0:b;	
			a=(x+maxP)*fx;			b=(y+maxP)*fy;			
			posXstop=(a<nnx)?a:nnx;	posYstop=(b<nny)?b:nny;	



			countsize=	(1+(int)((posZstop-posZstart-1)/optimStep))*
				(1+(int)((posYstop-posYstart-1)/optimStep)); //SPRAWDZIC!!
			nrofsamples=(int)countsize;
			countsize*=0.48;
		}


}
//-------------------------------------------------------------------
void ImageSetElasticRegistration::GetTransFromCurrentNode(double *tr, FloatGrid3D *grid)
{
	tr[1]=grid->GetDX(NodX,NodY,NodZ);
	tr[2]=grid->GetDY(NodX,NodY,NodZ);
	//tr[3]=grid->GetDZ(NodX,NodY,NodZ);
}
//-------------------------------------------------------------------
void ImageSetElasticRegistration::SetTransToCurrentNode(double *tr, FloatGrid3D *grid)
{
	grid->SetDx(NodX,NodY,NodZ,tr[1]);
	grid->SetDy(NodX,NodY,NodZ,tr[2]);
	//grid->SetDz(NodX,NodY,NodZ,tr[3]);
}
//-------------------------------------------
void ImageSetElasticRegistration::SlotStopOptimization()
{
	stopOptimization=true;
}
//---------------------------------------
//-----------------------------------------
//------------------------------------------

RawDataSet *ImageSetElasticRegistration::CreateTracedArrowsDataSet(QString filename,int step,int mode,unsigned char*mask)
{
	if(!filename.isEmpty())
	{
		int i,j,k;
		RawDataSet *d1=new RawDataSet();
		d1->Copy(*data,false);
		d1->ZeroQ();

		//wczytac ity grid;
		if (BSplineTransform!=NULL) delete BSplineTransform;
		if (grid!=NULL) delete grid;
		grid=new FloatGrid3D();
		int nr=grid->GetNumberOfGrids(filename);
		int nx=data->GetNx();
		int ny=data->GetNy();
		BSplineTransform=new BSplineGridTransformation(grid,nx-1,ny-1,1);


		int min=MIN(nr,data->GetNz()-1);
		double *pts=NULL;
		double *startPts=NULL;
		//double *pts2=NULL;
		int ptsNr;//,pts2Nr;
		//if (mode==1)
		{
			ptsNr=((int)((nx+1)/(step-1))*(int)((ny+1)/(step-1)))*3;
			pts=new double[ptsNr];
			startPts=new double[ptsNr];
			memset(pts,0,ptsNr*sizeof(double));
			memset(startPts,0,ptsNr*sizeof(double));

			//	pts2Nr=((int)((nx+1)/(step-1))*(int)((ny+1)/(step-1)))*3;
			//	pts2=new double[ptsNr];
			//	memset(pts2,0,ptsNr*sizeof(double));

		}
		int pos=0;
		for (j=0;j<ny;j+=step)
			for (i=0;i<nx;i+=step)
			{
				if (!((mask!=NULL)&&(mask[i+j*nx]==0))) 
				{


					pts[pos++]=i;
					pts[pos++]=j;
				}

			}
			//create random grid
			/*	FloatGrid3D *g1,*g2,*tmpg;
			g1=new FloatGrid3D();
			grid->LoadGrid(filename,0);
			g1->Copy(*grid);

			double s=0.5;
			for(j=0;j<g1->GetNy();j++)
			for(i=0;i<g1->GetNx();i++)
			{
			g1->SetDx(i,j,0,(rand()*2.0*s)/(double)RAND_MAX-s);
			g1->SetDy(i,j,0,(rand()*2.0*s)/(double)RAND_MAX-s);

			}
			g2=new FloatGrid3D();
			g2->Copy(*g1);
			g2->ReverseField();

			*/





			//create reversed random grid

			memcpy(startPts,pts,ptsNr*sizeof(double));

			for(k=0;k<min;k++)
			{
				grid->LoadGrid(filename,k);


				//grid->Copy(*g1);
				//grid->Clear();

				GenerateTracedArrows(k+1,k,d1,step,mode,pts,startPts,mask);//,pts2);
				emit SendMessage(initText+" ("+QString::number(k)+"/"+QString::number(min-1)+")");

				if (stopOptimization==true) break;
				//	tmpg=g1;
				//	g1=g2;
				//	g2=tmpg;

			}

			if (pts!=NULL)
				delete []pts;
			//if (pts2!=NULL)
			//	delete []pts2;
			delete BSplineTransform;
			BSplineTransform=NULL;
			return d1;

			//tmpg=NULL;
			//if (g1!=NULL) delete g1;
			//if (g2!=NULL) delete g2;
	}
	return NULL;
}

//--------------------------------------------------------------
void ImageSetElasticRegistration::GenerateTracedArrows(int dest,int src,RawDataSet *d,int step,int mode, double *points,double *startPoints,unsigned char*mask)//,double *points2)
{
	int i,j,k,l,m;

	int pos=0;

	unsigned short *tab=data->GetDataArray()+data->GetSliceSize()*src;
	double Point[2];
	Point[0]=0;
	Point[1]=0;
	double sPoint[2];

	unsigned short value;
	// wyczyszcze zerowym
	// 
	unsigned short *Q=d->GetDataArray()+d->GetSliceSize()*dest;
	unsigned short min=0;



	int nx=data->GetNx();
	int ny=data->GetNy();

	///2.11.2006 - zmiana teraz to normalna transformacja

	//Grid->ReverseField();

	//	grid->ReverseField();

	int length;
	double factor=0.5;
	double p0[2];
	double pa[2];
	double pb[2];
	//	double l;
	double len=2;
	double pointS[2];
	double delta[2];
	double tol=1e-7;

	//FILE *Do_opa;	Do_opa=fopen("c:\\pox.txt","w");	fprintf(Do_opa,"%d\n",dest);	fclose(Do_opa);
	for (j=0;j<ny;j+=step)
		for (i=0;i<nx;i+=step)
		{
			if (!((mask!=NULL)&&(mask[i+j*nx]==0))) 
			{
				//FILE *Do_opa;	Do_opa=fopen("c:\\pox.txt","a");	fprintf(Do_opa,"%d %d \n",i,j);	fclose(Do_opa);
				pointS[0]=points[pos];
				pointS[1]=points[pos+1];

				if ((pointS[0]>=0)&&(pointS[0]<nx-1.0)&&
					(pointS[1]>=0)&&(pointS[1]<ny-1.0))
				{
					BSplineTransform->TransformPoint2D(pointS[0],pointS[1],Point);

					//- mam nowa pozycje ...
					// sprawdzam czy otoczenie nowej pozycji jest podobne do otoczenia starej
					double stddevSrc=0;
					double avgSrc=0;
					int neighbrs=2;
					getLocalStatistics(src, pointS,neighbrs, avgSrc,stddevSrc);
					double stddevDst=0;
					double avgDst=0;
					getLocalStatistics(dest, Point,neighbrs, avgDst,stddevDst);

					double tol0=0.2;
					double tol1=0.8;
				

					//if ((fabs(stddevSrc-stddevDst)<tol1)&& (fabs(avgSrc-avgDst)<tol0))
					if (fabs(stddevSrc-stddevDst)<tol1)
					{
						//dobrze
					}
					else
					{

						// jesli nie jest to szukam nowej

						int steps=5;
						double ddelta[2];
						double bestPoint[2];
						double cPoint[2];
						//double bStddev=stddevDst;
						//double bAvg=avgDst;
						double tStddev;
						double tAvg;
						bool chg=false;
						double vval=INT_MAX;
						double tval;

						double t1,t2;
						ddelta[0]=Point[0]-pointS[0];
						ddelta[1]=Point[1]-pointS[1];

						double len=sqrt(ddelta[0]*ddelta[0]+ddelta[1]*ddelta[1]);

						if (len>tol)
						{
							ddelta[0]/=(double)len;
							ddelta[1]/=(double)len;

							//i=0;
							for(k=-steps;k<=steps;k++)
							{
								//probny punkt
								cPoint[0]=k*ddelta[0]+Point[0];
								cPoint[1]=k*ddelta[1]+Point[1];
								if ((cPoint[0]>0)&&(cPoint[1]>0)&&(cPoint[0]<nx)&&(cPoint[1]<ny))
								{
									getLocalStatistics(dest, cPoint,neighbrs, tAvg,tStddev);
									t1=fabs(stddevSrc-tStddev);
									t2=fabs(tAvg-avgDst);
									if ((t1<tol0))//&& (t2<tol0))
									{
										chg=true;
										tval=t1;//*t2;
										if (tval<vval)
										{
											bestPoint[0]=cPoint[0];
											bestPoint[1]=cPoint[1];
											vval=tval;
										}


									}
								}

							}

							if (chg==true)
							{
								//znalazlem i zrobie update
								Point[0]=bestPoint[0];
								Point[1]=bestPoint[1];

							}
							else
							{
								// jesli nie znalazlem to resetuje pozycje startowa
								startPoints[pos]=Point[0];
								startPoints[pos+1]=Point[1];
							}
						}
					}
					sPoint[0]=startPoints[pos];
					sPoint[1]=startPoints[pos+1];

					//getLocalStatistics(int frame, double *p0,int size, double &avg,double &stddev)

					//pa[0]=i;pa[1]=j;pa[2]=k;		
					//BSplineTransform->TransformPoint(pa,Point);

					//Point[0]=2*pointS[0]-Point[0];	
					//	Point[1]=2*pointS[1]-Point[1];	

					if (points!=NULL)
					{

						delta[0]=(Point[0]-pointS[0]);
						delta[1]=(Point[1]-pointS[1]);
						//if (fabs(delta[0])>tol)
						//{
						//	tol=tol;
						//}
						/*if (mode==1)
						{
						if (fabs(delta[0])>0.5)
						points[pos]+=delta[0];
						if (fabs(delta[1])>0.5)
						points[pos+1]+=delta[1];
						}
						else*/
						{
							//	if (fabs(delta[0])>0.5)
							//		points[pos]+=delta[0];
							//	else
							//		points[pos]+=delta[0]/5;

							//	if (fabs(delta[1])>0.5)
							//		points[pos+1]+=delta[1];
							//	else
							//		points[pos+1]+=delta[1]/5;
						}

						points[pos]+=delta[0];
						points[pos+1]+=delta[1];
					}


					if( points!=NULL)
					{
						Point[0]=points[pos];
						Point[1]=points[pos+1];
					}
					//FILE *Do_opa;	Do_opa=fopen("c:\\pox.txt","a");	fprintf(Do_opa,"[%lf %lf] [%lf %lf]\n",pointS[0],pointS[1],Point[0],Point[1]);	fclose(Do_opa);
					if (mode==0)	
						length=sqrt((Point[0]-pointS[0])*(Point[0]-pointS[0])+(Point[1]-pointS[1])*(Point[1]-pointS[1]))*100;
					else
						length=sqrt((Point[0]-sPoint[0])*(Point[0]-sPoint[0])+(Point[1]-sPoint[1])*(Point[1]-sPoint[1]))*100;

					if ((Point[0]>=0)&&(Point[0]<nx-1.0)&&
						(Point[1]>=0)&&(Point[1]<ny-1.0))
					{

						//value=intrp->GetInterpolatedValue(data,Point[0],Point[1],src);
						if (mode==0)
						{

							DrawLineUSHORT(Q,nx,ny,pointS[0], pointS[1], Point[0], Point[1], length);

							//FILE *Do_opa;	Do_opa=fopen("c:\\pox.txt","a");	fprintf(Do_opa,"drawn\n");	fclose(Do_opa);
							//proba rysowania strzalki

							p0[0]=pointS[0]+factor*(Point[0]-pointS[0]);
							p0[1]=pointS[1]+factor*(Point[1]-pointS[1]);
						}
						else
						{
							DrawLineUSHORT(Q,nx,ny,sPoint[0], sPoint[1], Point[0], Point[1], length);
							p0[0]=sPoint[0]+factor*(Point[0]-sPoint[0]);
							p0[1]=sPoint[1]+factor*(Point[1]-sPoint[1]);
						}
						// wetkor prostopadly
						pa[0]=-(Point[1]-p0[1]);
						pa[1]=(Point[0]-p0[0]);
						//normalizacja
						l=sqrt(pa[0]*pa[0]+pa[1]*pa[1]);
						if (l>0)
						{
							pa[0]/=l;
							pa[1]/=l;

						}
						pa[0]*=len;
						pa[1]*=len;

						pb[0]=-pa[0]+p0[0];
						pb[1]=-pa[1]+p0[1];
						pa[0]+=p0[0];
						pa[1]+=p0[1];

						if ((pa[0]>=0)&&(pa[0]<nx-1.0)&&(pa[1]>=0)&&(pa[1]<ny-1.0))
							if ((pb[0]>=0)&&(pb[0]<nx-1.0)&&(pb[1]>=0)&&(pb[1]<ny-1.0))
							{
								DrawLineUSHORT(Q,nx,ny,(int)pa[0], (int)pa[1], Point[0], Point[1], length);
								DrawLineUSHORT(Q,nx,ny,(int)pb[0], (int)pb[1], Point[0], Point[1], length);
							}







							//value=sqrt((i-Point[0])*(i-Point[0])+(j-Point[1])*(j-Point[1]))*10;
					}
					else
						value=min;
				}

				pos+=2;

			}

			//Q[pos++]=value;
		}

		Q=NULL;
		tab=NULL;


		//Grid->ReverseField()
}
//----------------------------------------------------------
void ImageSetElasticRegistration::getLocalStatistics(int frame, double *p0,int size, double &avg,double &stddev)
{
	if (data!=NULL)
	{
		int i,j,k;
		int nx=data->GetNx()-1;
		int ny=data->GetNy()-1;

		int min[2],max[2];

		min[0]=((int)(p0[0]-size)<0)?0:(int)(p0[0]-size);
		min[1]=((int)(p0[1]-size)<0)?0:(int)(p0[1]-size);
		max[0]=((int)(p0[0]+size)<nx)?(int)(p0[0]+size):nx;
		max[1]=((int)(p0[1]+size)<ny)?(int)(p0[1]+size):ny;

		avg=0;
		stddev=0;
		int nr=0;
		unsigned short *tab=new unsigned short[4*size*size];
		for(j=min[1];j<max[1];j++)
		{
			for(i=min[0];i<max[0];i++)
			{
				tab[nr]=data->GetVoxelValue(i,j,frame);
				avg+=tab[nr++];
			}

		}

		if (nr>0)
		{
			avg/=(double)nr;	


			for(i=0;i<nr;i++)
				stddev+=(avg-tab[i])*(avg-tab[i]);

			stddev/=(double)nr;
			stddev=sqrt(stddev);

		}

		delete []tab;

	}


}