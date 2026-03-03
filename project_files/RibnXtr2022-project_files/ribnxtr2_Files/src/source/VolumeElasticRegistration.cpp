#include "VolumeElasticRegistration.h"



void VolumeElasticRegistration::UpdateData(int gridNx, int gridNy, int gridNz)
{

	FloatGrid3D *tempGrid=new FloatGrid3D(gridNx,gridNy,gridNz);
	BSplineTransform->ScaleGrid(tempGrid);

	delete BSplineTransform;
	SetSizeVariables(tempGrid);
	BSplineTransform=new BSplineGridTransformation(tempGrid,nnx,nny,nnz);

	delete Grid;
	Grid=tempGrid;



}
//-----------------------------------
void	VolumeElasticRegistration::SetSizeVariables(FloatGrid3D *grid)
{

	nx=grid->GetNx();			ny=grid->GetNy();			nz=grid->GetNz();

	if (ModelDataScaled==NULL)
	{	nx1=ModelData->GetNx();		ny1=ModelData->GetNy();		nz1=ModelData->GetNz();	}
	else
	{	nx1=ModelDataScaled->GetNx();	ny1=ModelDataScaled->GetNy();	nz1=ModelDataScaled->GetNz();}

	mx1=ObjectData->GetNx();		my1=ObjectData->GetNy();		mz1=ObjectData->GetNz();

	fx=((nx>0)&&(nx1>0))?(double)(nx1-1)/(nx-1):1;
	fy=((ny>0)&&(ny1>0))?(double)(ny1-1)/(ny-1):1;
	fz=((nz>0)&&(nz1>0))?(double)(nz1-1)/(nz-1):1;

	fx2=fx*fx;			fy2=fy*fy;			fz2=fz*fz;
	tdx=1.0/fx;			tdy=1.0/fy;			tdz=1.0/fz;
	nnx=(double)(nx1-1);	nny=(double)(ny1-1);	nnz=(double)(nz1-1);




}
//--------------------------------------
VolumeElasticRegistration::VolumeElasticRegistration(ListItem<DataSet*>	*ModelListItem,	ListItem<DataSet*>	*ObjectListItem,FloatGrid3D *_Grid,double _maxDisp,const QString &text)
{
	parallelComputing = false;

#pragma omp parallel

	this->numberOfThreads = omp_get_num_threads();
	numberOfThreadsUsed = numberOfThreads;


	nodes=NULL;
	nrOfNodes=0;
	ModelDataScaled=NULL;

	dMax[0]=0;dMax[1]=0;dMax[2]=0;
	fval=INT_MAX;

	derivs=NULL;
	fixBounds=false;
	calculateDerivs=false;
	stopOptimization=false;
	int i,j,k;
	tab1=NULL;
	tab2=NULL;
	ObjectTab=NULL;
	ModelTab=NULL;
	ScatterTab=NULL;


	initText=text;
	transx=NULL;

	NodX=0;		NodY=0;		NodZ=0;

	ModelData=ModelListItem->GetData()->GetRawData();	
	ObjectData=ObjectListItem->GetData()->GetRawData();

	/*

	RawDataSet *ModelData=new RawDataSet();
	Data->Copy(*ModelData,false);
	double stddev=2.5;
	int radius=5.0;
	GaussianSmoothingFiltering( stddev,  radius, data,Data);


	*/



	mMax=ModelData->GetMaxValue();


	int atmp=MAX(ModelData->GetMaxValue(),ObjectData->GetMaxValue());

	//mVolMax=1.0/(double)atmp;
	mVolMax=(double)atmp;
	mVolMax2=mVolMax*mVolMax;

	mMax*=2;
	Grid=new FloatGrid3D();
	Grid->Copy(*_Grid);
	OrigGrid=_Grid;

	TempGrid=new FloatGrid3D();
	TempGrid->Copy(*_Grid);
	SetSizeVariables(Grid);
	BSplineTransform=new BSplineGridTransformation(Grid,nnx,nny,nnz);

	// w pixelach jest maxdisp
	if (_maxDisp!=0)
		maxDisp=_maxDisp;
	else
	{
		maxDisp=1.8;//pow((double)(fx*fy*fz),(1.0/3.0));		
	}
	maxDisp*=maxDisp;
	//*ModelData->GetXscale()*ModelData->GetYscale()*ModelData->GetZscale()


	pDef[1]=-1000;		pDef[2]=-1000;		pDef[3]=-1000;
	oldNod[0]=-1000;	oldNod[1]=-1000;	oldNod[2]=-1000;
	optimStep=1;
	randomSampling=true;

	//alpha=0.0;
	//alpha*=mVolMax;
	alpha=100.0;

	minP=1.5;
	maxP=1.5;

}
//----------------------------------------------------
VolumeElasticRegistration::~VolumeElasticRegistration()
{

	if (Grid!=NULL)
		delete Grid;
	OrigGrid=NULL;

	if (TempGrid!=NULL)
	{
		delete TempGrid;
		TempGrid=NULL;
	}

	if (tab1!=NULL)			{delete []tab1;tab1=NULL;}
	if (tab2!=NULL)			{delete []tab2;tab2=NULL;}
	if (ObjectTab!=NULL)	{delete []ObjectTab;ObjectTab=NULL;}
	if (ModelTab!=NULL)		{delete []ModelTab;ModelTab=NULL;}
	if (ScatterTab!=NULL)	{delete []ScatterTab;ScatterTab=NULL;}


}
//----------------------------------------------------
RawDataSet *VolumeElasticRegistration::GenerateAbsoluteDisplacementDataSet()
{
	// znalezc max displacement
	int i,j,k;
	double p1[3],p2[3],def[3];
	//Grid->ReverseField();
	double maxDisp=0;
	double tmpDisp;
	for (k=0;k<nz;k++)
		for (j=0;j<ny;j++)
			for (i=0;i<nx;i++)
			{
				p1[0]=fx*i;
				p1[1]=fy*j;
				p1[2]=fz*k;
				BSplineTransform->TransformPoint(p1,p2,def);
				tmpDisp=sqrt(def[0]*def[0]+def[1]*def[1]+def[2]*def[2]);
				if (tmpDisp>maxDisp) maxDisp=tmpDisp;

			}

			double factor=(maxDisp>0)?65535.0/maxDisp:1.0;

			RawDataSet *Data=new RawDataSet();
			Data->Copy(*ModelData,false);
			Data->ZeroQ();
			unsigned short *tQ=Data->GetDataArray();

			for (k=0;k<nz1;k++)
				for (j=0;j<ny1;j++)
					for (i=0;i<nx1;i++)
					{
						p1[0]=i;
						p1[1]=j;
						p1[2]=k;
						BSplineTransform->TransformPoint(p1,p2,def);
						tmpDisp=sqrt(def[0]*def[0]+def[1]*def[1]+def[2]*def[2]);
						*(tQ++)=(int)(tmpDisp*factor);

					}
					Data->SetRawDataSetName(ObjectData->GetRawDataSetName()+"_disp");		
					return Data;

					//przetransformowac caly zbior
					//Grid->ReverseField();

}
//----------------------------------------------------
RawDataSet *VolumeElasticRegistration::ApplyTransformation()
{

	RawDataSet *Data=new RawDataSet();
	Data->Copy(*ModelData,false);
	Data->ZeroQ();

	int i,j,k;

	int pos=0;
	unsigned short *Tab=ObjectData->GetDataArray();
	double Point[3];
	Point[0]=0;
	Point[1]=0;
	Point[2]=0;
	unsigned short value;
	// wyczyszcze zerowym
	// 
	unsigned short *Q=Data->GetDataArray();
	unsigned short min=ObjectData->GetMinValue();


	/*for (k=0;k<nx1*ny1*nz1;k++)
	Q[k]=min;

	for (k=0;k<nz1;k++)
	for (j=0;j<ny1;j++)
	for (i=0;i<nx1;i++)
	{
	value=Tab[pos++];
	BSplineTransform->TransformPoint(i,j,k,Point);

	if ((Point[0]>=0)&&(Point[0]<nx1-1.0)&&
	(Point[1]>=0)&&(Point[1]<ny1-1.0)&&
	(Point[2]>=0)&&(Point[2]<nz1-1.0))
	Data->SetVoxelValue(Round(Point[0]),Round(Point[1]),Round(Point[2]),value);
	//		Data->SetVoxelValue(Point[0],Point[1],Point[2],Interpolation3Linear(ObjectData,Point[0],Point[1],Point[2]));
	}
	Tab=NULL;
	*/

	//proba odwrotnej!!! 
	///2.11.2006 - zmiana teraz to normalna transformacja
	int pa[3];
	//Grid->ReverseField();
	for (k=0;k<nz1;k++)
		for (j=0;j<ny1;j++)
			for (i=0;i<nx1;i++)
			{
				BSplineTransform->TransformPoint(i,j,k,Point);
				//pa[0]=i;pa[1]=j;pa[2]=k;		
				//BSplineTransform->TransformPoint(pa,Point);

				if ((Point[0]>=0)&&(Point[0]<mx1-1.0)&&
					(Point[1]>=0)&&(Point[1]<my1-1.0)&&
					(Point[2]>=0)&&(Point[2]<mz1-1.0))
					value=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2]);
				else
					value=min;

				Q[pos++]=value;
			}

			Q=NULL;
			Tab=NULL;
			Data->SetRawDataSetName(ObjectData->GetRawDataSetName()+"_def");		
			return Data;
			//Grid->ReverseField();
}
//----------------------------------------------------
void VolumeElasticRegistration::Optimize2(int _iter,int _optimStep,bool _randomSampling,int multiressteps, bool _fixBounds,int valx)
{
	valX=valX;
	GetFValue=&VolumeElasticRegistration::GetEnergy;
	//////////////////
	fixBounds=_fixBounds;
	iter=_iter;
	optimStep=_optimStep;
	randomSampling=_randomSampling;
	/////////////////

	int i,j,k,l,m;
	scatterSize=64;
	ObjectTab=new int[scatterSize];
	ModelTab=new int[scatterSize];
	ScatterTab=new int[scatterSize*scatterSize];

	tab1=new int[ModelData->GetMaxValue()+1];
	tab2=new int[ObjectData->GetMaxValue()+1];

	double xmin,xfact;

	xmin=(double)ModelData->GetMinValue();
	xfact=(double)(scatterSize-1)/((double)ModelData->GetMaxValue()-xmin);
	for(i=0;i<ModelData->GetMaxValue()+1;i++)
		//tab1[i]=Round((double)((scatterSize-1)*i)/(double)ModelData->GetMaxValue());
		tab1[i]=Round((double)(i-xmin)*xfact);

	xmin=(double)ObjectData->GetMinValue();
	xfact=(double)(scatterSize-1)/((double)ObjectData->GetMaxValue()-xmin);
	for(i=0;i<ObjectData->GetMaxValue()+1;i++)
		//tab2[i]=Round((double)((scatterSize-1)*i)/(double)ObjectData->GetMaxValue());
		tab2[i]=Round((double)(i-xmin)*xfact);

	////////////////////////////
	if (transx!=NULL) delete []transx;
	transx=new double[4];

	double temptrans[4];
	double Tolerance=10e-2;
	double tolX=10e-8;
	int Iterations=0;
	double FValue=0;

	int NumberOfOptimParams=3;
	nrOfParams=NumberOfOptimParams;

	OptimizationPowell *Optim=NULL;

	//FILE *Do_opa;
	//Do_opa=fopen("d:\\t\\powellvolelas.txt","w");
	//fprintf(Do_opa,"start \t%lf\n",FValue);
	//fclose(Do_opa);

	double e,e1;

	//zabezpieczenie zeby rozmiar gridu nie byl wiekszy od zbioru
	int min=MIN3(nnx,nny,nnz);
	int maxgrid=MAX3(nx,ny,nz);
	int multimax=(maxgrid)*pow((double)2,multiressteps);

	if (multimax>min)
	{
		int a;
		multiressteps=0;
		while((a=(maxgrid)*pow((double)2,multiressteps+1))<min) 	
		{
			multiressteps+=1;
		}

	}

	//////////////////////////////////

	int st=(fixBounds==true)?1:0;
	QString t;

	int ii,jj,kk;
	int pos;

	double testtol;

	for(m=0;m<multiressteps;m++)
	{
		t=QString::number((nx-2*st)*(ny-2*st)*(nz-2*st)*iter);
		pos=0;
		testtol=MIN3(BSplineTransform->GetGxDivNx(),BSplineTransform->GetGyDivNy(),BSplineTransform->GetGzDivNz())/100.0;
		for(l=0;l<iter;l++)

			//for (k=st;k<st+2;k++)
			//	for (j=st;j<st+1;j++)
			//		for (i=st;i<st+1;i++)
			//i=st;j=st;k=st;
			for (k=st;k<nz-st;k++)
				for (j=st;j<ny-st;j++)
					for (i=st;i<nx-st;i++)
					{
						//i=1;j=1;k=1;
						if (stopOptimization==false)	
						{
							//	if ((l%2)==0)	
							{	ii=i;	jj=j;	kk=k;}
							//	else		
							//	{ii=nx-1-i;	jj=ny-1-j;	kk=nz-1-k;}

							emit SendMessage(initText+" ("+QString::number(++pos)+"/"+t+") "+" multires:("+ QString::number(m+1)+"/"+QString::number(multiressteps)+")");
							SetCurrentNode(ii,jj,kk);
							GetTransFromCurrentNode(transx,Grid);


							e=GetFunctionValue(transx);	temptrans[1]=transx[1];	temptrans[2]=transx[2];	temptrans[3]=transx[3];
							//Do_opa=fopen("d:\\t\\powellvolelas.txt","a");fprintf(Do_opa,"%d\t%d\t%d\n%lf\t%lf\t%lf\t%lf-s\n",ii,jj,kk,transx[1],transx[2],transx[3],e);fclose(Do_opa);
							iterx=0;
							Optim=new OptimizationPowell(transx,NumberOfOptimParams,Tolerance,&Iterations,&FValue,this,testtol); //:) 

							Optim->StartOptimization();
							delete Optim;

							e1=FValue;
							// jezeli stara-nowa jest w gr tolerancji to ustaw stara
							if (fabs(e1-e)<tolX)
							{
								transx[1]=temptrans[1];	transx[2]=temptrans[2];	transx[3]=temptrans[3];e1=e;
								//Do_opa=fopen("d:\\t\\powellvolelas.txt","a");fprintf(Do_opa,"qq\n");fclose(Do_opa);	
							}

							//Do_opa=fopen("d:\\t\\powellvolelas.txt","a");fprintf(Do_opa,"%d\t%d\t%d\n%lf\t%lf\t%lf\t%lf-f (%d)\n",i,j,k,transx[1],transx[2],transx[3],e1,iterx);fclose(Do_opa);

							SetTransToCurrentNode(transx,Grid);
						}
					}

					if (stopOptimization==false)
						if (multiressteps-m>1)
						{
							//przebudowac
							UpdateData(nx*2,ny*2,nz*2);
							delete TempGrid;
							TempGrid=new FloatGrid3D();
							TempGrid->Copy(*Grid);
							//UpdateData(nx+1,ny+1,nz+1);
							if (optimStep>=2) optimStep/=2;
						}
						//Tolerance/=2;
	}
	OrigGrid->Copy(*Grid);
	//				OrigGrid->Scale(tdx,tdy,tdz);
	/////////////////////////////


	stopOptimization=false;
	if (tab1!=NULL){delete []tab1;tab1=NULL;}
	if (tab2!=NULL){delete []tab2;tab2=NULL;}
	if (ObjectTab!=NULL){delete []ObjectTab;ObjectTab=NULL;}
	if (ModelTab!=NULL){delete []ModelTab;ModelTab=NULL;}
	if (ScatterTab!=NULL){delete []ScatterTab;ScatterTab=NULL;}

}
//-------------------------------------------------------------------
void	VolumeElasticRegistration::SetCurrentNode(int x,int y,int z)
{

	if ((x>=0) && (x<nx))
		if ((y>=0) && (y<ny))
			if ((z>=0) && (z<nz))
			{
				double a,b,c;


				NodX=x;					NodY=y;					NodZ=z;
				a=(x-minP)*fx;			b=(y-minP)*fy;			c=(z-minP)*fz;
				posXstart=(a<0)?0:a;	posYstart=(b<0)?0:b;	posZstart=(c<0)?0:c;
				a=(x+maxP)*fx;			b=(y+maxP)*fy;			c=(z+maxP)*fz;
				posXstop=(a<nnx)?a:nnx;	posYstop=(b<nny)?b:nny;	posZstop=(c<nnz)?c:nnz;

				pSx=(double)(posXstop-posXstart-1)/32767.0;
				pSy=(double)(posYstop-posYstart-1)/32767.0;
				pSz=(double)(posZstop-posZstart-1)/32767.0;

				countsize=	(1+(int)((posZstop-posZstart-1)/optimStep))*
					(1+(int)((posYstop-posYstart-1)/optimStep))*
					(1+(int)((posXstop-posXstart-1)/optimStep)); //SPRAWDZIC!!
				nrofsamples=(int)countsize;
				countsize*=0.48;
			}


}
//-------------------------------------------------------------------
void VolumeElasticRegistration::GetTransFromCurrentNode(double *tr, FloatGrid3D *grid)
{
	tr[1]=grid->GetDX(NodX,NodY,NodZ);
	tr[2]=grid->GetDY(NodX,NodY,NodZ);
	tr[3]=grid->GetDZ(NodX,NodY,NodZ);
}
//-------------------------------------------------------------------
void VolumeElasticRegistration::SetTransToCurrentNode(double *tr, FloatGrid3D *grid)
{
	grid->SetDx(NodX,NodY,NodZ,tr[1]);
	grid->SetDy(NodX,NodY,NodZ,tr[2]);
	grid->SetDz(NodX,NodY,NodZ,tr[3]);
}

//-------------------------------------------------------------------
/*double	VolumeElasticRegistration::GetFunctionValue2(double *params)
{

SetTransToCurrentNode(params,Grid);
//double e=GetEnergy();

double e=GetEnergy(NodX,NodY,NodZ);
/*FILE *Do_opa;
if (	tmpbool==true)
{
Do_opa=fopen("d:\\t\\powellvolelas.txt","a");
for(int i=1;i<4;i++)
{fprintf(Do_opa,"%lf\t",params[i]);}
fprintf(Do_opa," (%d) ",count);
fprintf(Do_opa,"e= %lf \n",e);

fclose(Do_opa);
}
return e;

}*/
//------------------------------------------------
double	VolumeElasticRegistration::GetEnergy(int x,int y,int z,double *params)
{

	SetTransToCurrentNode(params,Grid);
	//trzenba koniecznie wczesniej wywolac SetCurrentNode !!!
	double def[4];
	double def0[4];
	int i,j,k,l;
	int pos;

	double energy=0.0;
	double tttol=10e-7;
	double beta;

	if (valX!=-1)
	{

		GetTransFromCurrentNode(def,Grid);
		GetTransFromCurrentNode(def0,TempGrid);
		def0[1]+=def[1];def0[2]+=def[2];def0[3]+=def[3];

		if ((fabs(pDef[1]-def[1])<tttol)&&(fabs(pDef[2]-def[2])<tttol)&&(fabs(pDef[3]-def[3])<tttol))
			if ( (oldNod[0]==NodX) && (oldNod[1]==NodY) && (oldNod[2]==NodZ) )
				return pFun;

		//if ((fabs(def[1])>Step[0])||(fabs(def[2])>Step[1])||(fabs(def[3])>Step[2]))
		if  (
			((def0[1]*def0[1]+def0[2]*def0[2]+def0[3]*def0[3])>maxDisp)
			//(sqrt(def[1]*def[1]*fx2+def[2]*def[2]*fy2+def[3]*def[3]*fz2)>maxDisp)
			//	(sqrt(def[1]*def[1]*fx2+def[2]*def[2]*fy2+def[3]*def[3]*fz2)>maxDisp)
			//((fabs(def[1])>Step[0])||(fabs(def[2])>Step[1])||(fabs(def[3])>Step[2]))
			||
			(Grid->Check3DLocus(NodX,NodY,NodZ)==false)
			)
		{
			return INT_MAX;
		}
		else
		{

			//stworze histogramy

			memset(ObjectTab, 0, scatterSize * sizeof(int));
			memset(ModelTab, 0, scatterSize * sizeof(int));
			memset(ScatterTab, 0, scatterSize*scatterSize * sizeof(int));
			count=0;


			//////
			/*	if (randomSampling==true)
			{
			srand( (unsigned)time( NULL ) );
			for(l=0;l<countsize;l++)
			{
			i=(int)(pSx*rand())+posXstart;
			j=(int)(pSy*rand())+posYstart;
			k=(int)(pSz*rand())+posZstart;
			AddNewPointsToHistTabs(i,j,k,count);
			}
			}
			else*/
			{
				for(k=posZstart;k<posZstop;k+=optimStep)
					for(j=posYstart;j<posYstop;j+=optimStep)
						for(i=posXstart;i<posXstop;i+=optimStep)
							AddNewPointsToHistTabs(i,j,k,count);

			}

			count++;// to zabezpieczenie testowe zeby gdy oba obrazki nalozone maja same zera zeby MI nie byla zero
			double a,b,c;
			if (count<countsize) {	return INT_MAX;}
			// policze mutuala

			a=GetEntropy(ModelTab,count,scatterSize);
			b=GetEntropy(ObjectTab,count,scatterSize);
			c=GetEntropy(ScatterTab,count,scatterSize*scatterSize);
			beta=Grid->GetMaxDistanceWith3DLocus(NodX,NodY,NodZ);
			beta=pow(exp(beta),0.01)-1;
			beta*=beta;
			energy=-(a+b)/c;

		}

		pDef[1]=def[1];pDef[2]=def[2];pDef[3]=def[3];
		oldNod[0]=NodX;oldNod[1]=NodY;oldNod[2]=NodZ;
		pFun=beta*energy;
	}

	else
	{
		GetTransFromCurrentNode(def,Grid);
		GetTransFromCurrentNode(def0,TempGrid);
		def0[1]+=def[1];def0[2]+=def[2];def0[3]+=def[3];

		if ((fabs(pDef[1]-def[1])<tttol)&&(fabs(pDef[2]-def[2])<tttol)&&(fabs(pDef[3]-def[3])<tttol))
			if ( (oldNod[0]==NodX) && (oldNod[1]==NodY) && (oldNod[2]==NodZ) )
				return pFun;

		
		if  (
			((def0[1]*def0[1]+def0[2]*def0[2]+def0[3]*def0[3])>maxDisp)
			||
			(Grid->Check3DLocus(NodX,NodY,NodZ)==false)
			)
		{
			return INT_MAX;
		}
		else
		{

			//stworze histogramy

			memset(ObjectTab, 0, scatterSize * sizeof(int));
			memset(ModelTab, 0, scatterSize * sizeof(int));
			memset(ScatterTab, 0, scatterSize*scatterSize * sizeof(int));
			count=0;


		
				for(k=posZstart;k<posZstop;k+=optimStep)
					for(j=posYstart;j<posYstop;j+=optimStep)
						for(i=posXstart;i<posXstop;i+=optimStep)
							AddNewPointsToHistTabs2(i,j,k,count);

			

			count++;// to zabezpieczenie testowe zeby gdy oba obrazki nalozone maja same zera zeby MI nie byla zero
			double a,b,c;
			if (count<countsize) {	return INT_MAX;}
			// policze mutuala

			a=GetEntropy(ModelTab,count,scatterSize);
			b=GetEntropy(ObjectTab,count,scatterSize);
			c=GetEntropy(ScatterTab,count,scatterSize*scatterSize);
			beta=Grid->GetMaxDistanceWith3DLocus(NodX,NodY,NodZ);
			beta=pow(exp(beta),0.01)-1;
			beta*=beta;
			energy=-(a+b)/c;

		}

		pDef[1]=def[1];pDef[2]=def[2];pDef[3]=def[3];
		oldNod[0]=NodX;oldNod[1]=NodY;oldNod[2]=NodZ;
		pFun=beta*energy;
	}

	return energy;
}
//------------------------------------------------
void VolumeElasticRegistration::AddNewPointsToHistTabs(int i,int j,int k, int &count)
{
	double Point[3];
	BSplineTransform->TransformPoint(i,j,k,Point);

	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&
		(Point[1]>=0)&&(Point[1]<my1-1)&&
		(Point[2]>=0)&&(Point[2]<mz1-1))
	{
		//pozycja mvalue bezposrednio
		// pozycjaovalue z bsppline transform

		// to do poprawy
		//OValue=ObjectData->GetVoxelValue(i,j,k);
		//////////////////////
		//MValue=Interpolation3Linear(ModelData,Point[0],Point[1],Point[2]);
		//Value=ModelData->GetVoxelValue(Round(Point[0]),Round(Point[1]),Round(Point[2]));

		//ODWROTNIE NIZ DO TEJ PORY!!!
		MValue=ModelData->GetVoxelValue(i,j,k);
		OValue=Interpolation3Linear(ObjectData,Point[0],Point[1],Point[2]);

		ModelValue=tab1[MValue];
		ObjectValue=tab2[OValue];
		ObjectTab[ObjectValue]++;
		ModelTab[ModelValue]++;
		ScatterTab[ModelValue*scatterSize+ObjectValue]++;
		count++;
	}
}
//------------------------------------------------
void VolumeElasticRegistration::AddNewPointsToHistTabs2(int i,int j,int k, int &count)
{
	double Point[3];
	MValue=ModelData->GetVoxelValue(i,j,k);
	if (MValue!=valX)
	{
	BSplineTransform->TransformPoint(i,j,k,Point);

	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&
		(Point[1]>=0)&&(Point[1]<my1-1)&&
		(Point[2]>=0)&&(Point[2]<mz1-1))
	{
		//pozycja mvalue bezposrednio
		// pozycjaovalue z bsppline transform

		// to do poprawy
		//OValue=ObjectData->GetVoxelValue(i,j,k);
		//////////////////////
		//MValue=Interpolation3Linear(ModelData,Point[0],Point[1],Point[2]);
		//Value=ModelData->GetVoxelValue(Round(Point[0]),Round(Point[1]),Round(Point[2]));

		//ODWROTNIE NIZ DO TEJ PORY!!!
		
		OValue=Interpolation3Linear(ObjectData,Point[0],Point[1],Point[2]);

		ModelValue=tab1[MValue];
		ObjectValue=tab2[OValue];
		ObjectTab[ObjectValue]++;
		ModelTab[ModelValue]++;
		ScatterTab[ModelValue*scatterSize+ObjectValue]++;
		count++;
	}
	}
}
//-----------------------------------------------
double VolumeElasticRegistration::CompMutual(int *ST,int *MT,int *OT,int count)
{
	double Energy=0;
	double p,p1,p2;
	int *sp=ST;
	int i,j;
	double Count1=1.0/count;
	for (j = 0; j < scatterSize; j++)
	{
		p1 = (double)MT[j]*Count1;

		for (i = 0; i < scatterSize; i++)
		{    
			if ((*sp!=0)&&(MT[j]!=0)&&(OT[i]!=0))             
			{
				p = (double)*sp*Count1;
				p2 = (double)OT[i]*Count1;

				Energy+= 
					//log2tab[(int)(1000000*p)];
					-p*log(p);//-p*log2(p/(p1*p2));	

			}
			sp++;
		} 
	}

	return (double)Energy;

}
//---------------------------------------------------
//----------------------------------------------------------------------
double VolumeElasticRegistration::GetEntropy(int *Histogram,int count,int size)
{
	int i;
	double Entropy=0;
	double Count1=1.0/count;
	double p1;

	for (i = 0; i < size; i++)
	{
		if(Histogram[i]!=0)
		{
			p1 = (double)Histogram[i]*Count1;
			Entropy+= 
				//log2tab[(int)(1000000*p1)];
				-p1*log2(p1);
		}
	}
	return Entropy;

}
//---------------------------------------------------
void VolumeElasticRegistration::SlotStopOptimization()
{
	stopOptimization=true;
}
//---------------------------------------------------
void VolumeElasticRegistration::Optimize(int _iter,int _optimStep,bool _randomSampling,int multiressteps, bool _fixBounds)
{

	fixBounds=_fixBounds;
	GetFValue=&VolumeElasticRegistration::GetSqDistEnergy;
	GetDerivatives=&VolumeElasticRegistration::CalculateSingleNodDerivative;

	//////////////////
	//bool fixBounds=true;
	iter=_iter;
	optimStep=_optimStep;
	randomSampling=_randomSampling;
	/////////////////

	int i,j,k,l,m;

	derivs=new double[4];

	////////////////////////////
	if (transx!=NULL) delete []transx;
	transx=new double[4];

	double temptrans[4];
	double Tolerance;//10e-4;
	int Iterations=0;
	double FValue=0;

	int NumberOfOptimParams=3;
	nrOfParams=NumberOfOptimParams;

	//OptimizationPowell *Optim=NULL;

	//FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelas.txt","w");	fprintf(Do_opa,"");	fclose(Do_opa);

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
	int min=MIN3(nnx,nny,nnz);
	int maxgrid=MAX3(nx,ny,nz);
	int multimax=(maxgrid)*pow((double)2,multiressteps);

	//-----------------------------------
	//----------------------------------
	int optimStepX=optimStep;
	optimStep=1;
	ModelDataScaled=NULL;
	double threshold=1.0;
	double ss=1.0;

	//double gSize=4;
	//UpdateData(gSize,gSize,gSize);
	//multiressteps=1;
	//Do_opa=fopen("d:\\t\\kurde.txt","a");fprintf(Do_opa,"%s - %s\n",ModelData->GetRawDataSetName().toLocal8Bit(),ObjectData->GetRawDataSetName().toLocal8Bit());fclose(Do_opa);


	minP=1.3;
	maxP=1.3;
	alpha=500.0;
	Tolerance=1e-5;

	for(m=0;m<multiressteps;m++)	
	{
		//optimStepX=scaleTab[m];
		//przeskalowac zbior i ustawic paramtetry dla 
		if ((ModelDataScaled!=NULL)&& (ModelDataScaled!=ModelData)) 
			delete ModelDataScaled;
		if (optimStepX>1)
		{
			ModelDataScaled=ScaleDataSet(ModelData,1.0/optimStepX);
			ModelDataScaled->CalculateHistogram();
		}
		else
			ModelDataScaled=ModelData;

		BSplineTransform->SetupForwardAndBackwardScalingFactors(
			ModelDataScaled->GetNx(),ModelDataScaled->GetNy(),ModelDataScaled->GetNz(),
			ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz());
		SetSizeVariables(Grid);
		t=QString::number((nx-2*st)*(ny-2*st)*(nz-2*st)*iter);
		pos=0;
		smallStepX=BSplineTransform->GetGxDivNx()*ss;
		smallStepY=BSplineTransform->GetGyDivNy()*ss;
		smallStepZ=BSplineTransform->GetGzDivNz()*ss;


		maxFactor=MAX(ModelDataScaled->GetMaxValue(),ObjectData->GetMaxValue());
		maxFactor=(maxFactor>0)?255.0/maxFactor:1.0;


		for(l=0;l<iter;l++)	
			for (k=st;k<nz-st;k++)
				for (j=st;j<ny-st;j++)
					for (i=st;i<nx-st;i++)
					{
						//i=1;j=1;k=1;
						if ((stopOptimization==false)&&(!((i==2)&&(j==2)&&(k==2))))	
						{
							emit SendMessage(initText+" ("+QString::number(++pos)+"/"+t+") "+" multires:("+ QString::number(m+1)+"/"+QString::number(multiressteps)+")");
							SetCurrentNode(i,j,k);
							GetTransFromCurrentNode(transx,Grid);

							e=GetFunctionValue(transx);
							if (e>threshold)
							{
								iterx=0;
								temptrans[1]=transx[1];	temptrans[2]=transx[2];	temptrans[3]=transx[3];
								Optim=new OptimizationDFPowell(transx,NumberOfOptimParams,Tolerance,&Iterations,&FValue,this); //:) 
								Optim->StartOptimization();
								delete Optim;


								SetTransToCurrentNode(transx,Grid);

								/*	if ((i==6)&&(j==13)&&(k==10))
								{
								Do_opa=fopen("d:\\t\\kurde.txt","w");fprintf(Do_opa,"%d %d %d(%d %d %d) %lf %lf %lf", NodX,NodY,NodZ,k,j,i,transx[1],transx[2],transx[3]);fclose(Do_opa);
								}*/
								/*if (Grid->Check3DLocus(NodX,NodY,NodZ)==false)
								{
								//	transx[1]=temptrans[1];	transx[2]=temptrans[2];	transx[3]=temptrans[3];
								//	SetTransToCurrentNode(transx,Grid);
								//		Do_opa=fopen("d:\\t\\Failed.txt","a");fprintf(Do_opa,"b");fclose(Do_opa);
								}
								else
								{
								int oo=1;
								}*/
								/*	if ((transx[1]*transx[1]+transx[2]*transx[2]+transx[3]*transx[3])>10.0)
								{
								Do_opa=fopen("d:\\t\\kurde.txt","a");fprintf(Do_opa,"%d %d %d(%d %d %d) %lf %lf %lf", NodX,NodY,NodZ,k,j,i,transx[1],transx[2],transx[3]);fclose(Do_opa);
								i=i;
								}	*/


							//	Do_opa=fopen("d:\\t\\powellvolelas.txt","a");fprintf(Do_opa,"%d\t%d\t%d\n%lf\t%lf\t%lf\t(s: %lf f: %lf) (%d)\n",i,j,k,transx[1],transx[2],transx[3],e,FValue,iterx);fclose(Do_opa);

							}
						}
					}

					if (stopOptimization==false)
						if (multiressteps-m>1)
						{
							//przebudowac
							//UpdateData(gridSize[m+1],gridSize[m+1],gridSize[m+1]);
							UpdateData(nx*2-1,ny*2-1,nz*2-1);
							delete TempGrid;
							TempGrid=new FloatGrid3D();
							TempGrid->Copy(*Grid);
							//UpdateData(nx+1,ny+1,nz+1);
							if (optimStepX>=2) optimStepX/=2;
							Tolerance*=50.0;
							minP-=0.3;
							maxP-=0.3;
						}

	}
	if ((ModelDataScaled!=NULL)&& (ModelDataScaled!=ModelData)) 
		delete ModelDataScaled;

	OrigGrid->Copy(*Grid);

	stopOptimization=false;
	delete []derivs;derivs=NULL;


}
//-------------------------------------------------------------------
double	VolumeElasticRegistration::GetFunctionValue(double *params)
{
	iterx++;

	//double e=GetSqDistEnergy(NodX,NodY,NodZ);
	double e=(this->*GetFValue)(NodX,NodY,NodZ,params);
	//	emit SendMessage(initText+" ("+QString::number(iterx)+"="+QString::number(e)+") ");
	//		FILE *Do_opa;	Do_opa=fopen("d:\\t\\fval.txt","a");	fprintf(Do_opa,"%d %lf\t[%lf %lf %lf]\n",iterx,e,params[1],params[2],params[3]);	fclose(Do_opa);

	return e;
}
//-------------------------------------------------------------------
void	VolumeElasticRegistration::CalculateDerivative(double *Deriv,double *Params)
{
	if ((Deriv!=NULL)&&(Params!=NULL))
	{
		(this->*GetDerivatives)(Deriv,Params);
		//CalculateTotalDerivative(Deriv,Params);
		//CalculateSingleNodDerivative(Deriv,Params);
		//		emit SendMessage(initText+" ("+QString::number(iterx)+"="+"DERIV"+") ");
		//FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelasB.txt","a");	fprintf(Do_opa,"Deriv [%lf %lf %lf]\n", Deriv[1],Deriv[2],Deriv[3]);	fclose(Do_opa);
	}

}
//-------------------------------------------------------------------
double	VolumeElasticRegistration::GetSqDistEnergy(int x, int y, int z, double* params)
{
	SetTransToCurrentNode(params, Grid);
	double def[4];
	double def0[4];
	int i, j, k, l;
	int pos;

	//double energy=0.0;
	double energy[2];energy[0] = 0;energy[1] = 0;
	double tttol = 10e-7;
	double der[6];
	der[0] = 0;	der[1] = 0;	der[2] = 0; der[3] = 0;	der[4] = 0;	der[5] = 0;
	count = 0;
	//////
	//parallelComputing = false;
	if (parallelComputing == false)
	{
		if (randomSampling == true)
		{
			srand((unsigned)time(NULL));
			for (l = 0;l < countsize;l++)
			{
				i = (int)(pSx * rand()) + posXstart;
				j = (int)(pSy * rand()) + posYstart;
				k = (int)(pSz * rand()) + posZstart;
				//	AddNewEnergyPoint(i,j,k,count,energy,NULL);
				AddNewEnergyPointWithoutDerivs(i, j, k, count, energy);
			}
		}
		else
		{
			for (k = posZstart;k < posZstop;k += optimStep)
				for (j = posYstart;j < posYstop;j += optimStep)
					for (i = posXstart;i < posXstop;i += optimStep)
						//	AddNewEnergyPoint(i,j,k,count,energy,NULL);
						AddNewEnergyPointWithoutDerivs(i, j, k, count, energy);


		}
	}
	else
	{
//---------------------------------------------------------------------------------------------
		double* tabs;

		tabs = new double [numberOfThreadsUsed*2];

		for (k = 0; k < this->numberOfThreadsUsed*2; k++)
		{
			///tabs[k] = new double[2];
			//for (int i = 0; i < 8; i++)
				tabs[k] = 0;

		}
		int* counts;
#pragma omp parallel  num_threads(numberOfThreadsUsed) private(counts)//private (counts,tabs)//shared(Q1,paddQ)  //num_threads(1)
		{
			int tid = omp_get_thread_num();


			counts = new int[numberOfThreadsUsed];
#pragma omp critical
			for (k = 0; k < this->numberOfThreadsUsed; k++)counts[k] = 0;
			//#pragma omp critical

#pragma omp for  nowait ordered schedule (guided) private(k) //private (counts)//ordered schedule(dynamic) p
			for (k = posZstart; k < posZstop; k += optimStep)
			{
				//int thNr= omp_get_thread_num();
				AddNewEnergyPointSetWithoutDerivs(k, counts[tid], tabs+tid*2);
			}

#pragma omp critical
			{

				for (k = 0; k < numberOfThreadsUsed; k++)
					count += counts[k];
				delete[]counts;
			}

		}


		for (i = 0; i < this->numberOfThreadsUsed; i++)
		{
			energy[0] += tabs[i*2];
			energy[1] += tabs[i*2+1];

			
		}
	
		delete[]tabs;







//---------------------------------------------------------------------------------------------
	}


	double f= energy[0]+alpha*energy[1];
	//if (count >0)	if (f!=0) f/=(double)count;
	f= (count >0)?f/=(double)count:INT_MAX;

	//if ((NodX==2)&&(NodY==2)&&(NodZ==2))
	//{FILE *Do_opa;Do_opa=fopen("d:\\t\\powellvolelasFAILURE.txt","a");fprintf(Do_opa,"%d %d (%lf %lf %lf)(%lf %lf)\n",iterx,count,params[1],params[2],params[3],energy[0],energy[1]);fclose(Do_opa);}
//	if (count>0)
	////{FILE *Do_opa;Do_opa=fopen("d:\\t\\textXX.txt","a");fprintf(Do_opa,"[ %lf \t %lf ]( %lf \t %lf \t %lf , %d) = %lf \n ",energy[0]/count,energy[1]*alpha/count,params[1],params[2],params[3],count,f);fclose(Do_opa);}
	//FILE *Do_opa;Do_opa=fopen("d:\\t\\powellvolelasFAILURE.txt","a");fprintf(Do_opa,"%d\t%d\t(%lf\t%lf\t%lf)\t(%lf\t%lf)\t%lf\t[%lf]\n",iterx,count,params[1],params[2],params[3],energy[0],energy[1],energy[1]/energy[0],f);fclose(Do_opa);
	return f;
}
//-------------------------------------------------

void		 VolumeElasticRegistration::AddNewEnergyPointSetWithoutDerivs(int k, int& count, double* energy)
{
	for (int j = posYstart; j < posYstop; j += optimStep)
	{
		for (int i = posXstart; i < posXstop; i += optimStep)
		{
			AddNewEnergyPointWithoutDerivs(i, j, k, count, energy);
		}
	}
}
//------------------------
void	VolumeElasticRegistration::AddNewEnergyPointSet(int k, int& count, double* energy, double *drv)
{
	
	for (int j = posYstart; j < posYstop; j += optimStep)
	{
		for (int i = posXstart; i < posXstop; i += optimStep)
		{
			AddNewEnergyPoint( i,  j,  k,  count,  energy,  drv);
		}
	}

}
//--------------------------------------------------------
void VolumeElasticRegistration::AddNewEnergyPoint(int i,int j,int k, int &count,double *energy,double *drv)
{

	int pa[3];pa[0]=i;pa[1]=j;pa[2]=k;
	int nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=NodZ;
	double ddDdP[9];
	double ddDdPL[3];
	double ddDdQ;
	double ddDPdQ[3];
	double Point[3];

	//double a=0;

	BSplineTransform->TransformPoint(pa,Point,true,nodPos,ddDdP,ddDdPL,&ddDdQ,ddDPdQ);

	double m=0;
	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))
	{	
		m=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2])*maxFactor;
		double a=ModelDataScaled->GetVoxelValue(i,j,k)*maxFactor-m;
		double deriv=-a*ddDdQ;

		if (drv!=NULL)
		{
			if (deriv!=0)
			{
				if ((calculateDerivs)||(drv!=NULL))
				{
					double gradIx=GetGradI(i,j,k,smallStepX,m,0); 
					double gradIy=GetGradI(i,j,k,smallStepY,m,1); 
					double gradIz=GetGradI(i,j,k,smallStepZ,m,2); 
					drv[0]+=deriv*gradIx;
					drv[1]+=deriv*gradIy;
					drv[2]+=deriv*gradIz;
				}
			}

			drv[3]+=(ddDdP[0]*ddDPdQ[0]+ddDdP[3]*ddDPdQ[1]+ddDdP[6]*ddDPdQ[2]);
			drv[4]+=(ddDdP[1]*ddDPdQ[0]+ddDdP[4]*ddDPdQ[1]+ddDdP[7]*ddDPdQ[2]);
			drv[5]+=(ddDdP[2]*ddDPdQ[0]+ddDdP[5]*ddDPdQ[1]+ddDdP[8]*ddDPdQ[2]);
		}
		energy[0]+=a*a;	
		energy[1]+=(ddDdPL[0]+ddDdPL[1]+ddDdPL[2]);	

		if ((i == 1) && (j == 0) && (k == 16))
		{
			i = i;
		}
		if (energy[1] > 0)
		{
			i = i;
		}
		count++;
	}

	if (drv[0]>0)
	{
		count=count;
	}
	//FILE *Do_opa;	Do_opa=fopen("d:\\t\\dd.txt","a");	fprintf(Do_opa,"%lf %lf %lf %lf %lf %lf\n",der[0],der[1],der[2],der[3],der[4],der[5]);	fclose(Do_opa);


}
//----------------------------------
double  VolumeElasticRegistration::GetGradI(int i,int j,int k,double smallStep,double fvalue,int view)
{
	//fkcja liczy gradient w zadanym kierunku osi glownych - view
	//dla danego pktu zbioru danych Model Data i danego noda
	//smallStep jest to dx=dy,dz dla liczenia gradientu
	double plus, minus,value=0;
	double Point[3];
	double x,y,z;
	switch (view)
	{
	case 0:		x=smallStep;y=0;z=0;	break;
	case 1:		x=0;y=smallStep;z=0;	break;
	case 2:		x=0;y=0;z=smallStep;	break;
	}

	BSplineTransform->TransformPont(i,j,k,Point,x,y,z,NodX,NodY,NodZ);
	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&
		(Point[1]>=0)&&(Point[1]<my1-1)&&
		(Point[2]>=0)&&(Point[2]<mz1-1)))
		return 0;
	plus=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2])*maxFactor;//-fvalue;

	BSplineTransform->TransformPont(i,j,k,Point,-x,-y,-z,NodX,NodY,NodZ);
	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&
		(Point[1]>=0)&&(Point[1]<my1-1)&&
		(Point[2]>=0)&&(Point[2]<mz1-1)))
		return 0;
	minus=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2])*maxFactor;//-fvalue;

	value=0.5*(plus-minus);

	return value;

};
//------------------
double	VolumeElasticRegistration::GetFunctionValue(double i,double *Params)
{
	return 0;
}
//--------------------------------------
double 	VolumeElasticRegistration::GetTotalFunctionValue(int a,int b,int c,double *Params)
{
	SetParamsFromArrayToGrid(Grid,Params,fixBounds);
	int i,j,k;
	double energy[2];energy[0]=0;energy[1]=0;
	int count=0;



	for(k=0;k<nz1;k+=optimStep)
		for(j=0;j<ny1;j+=optimStep)
			for(i=0;i<nx1;i+=optimStep)	
				AddNewEnergyPointWithoutDerivs(i,j,k,count,energy);

	double f=energy[0]+alpha*energy[1];
	//if (count >0)	if (f!=0) f/=(double)count;
	f= (count >0)?f/=(double)count:INT_MAX;
	//if (f<fval)
	//{
	//	fval=f;
	////	FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelasB.txt","a");	fprintf(Do_opa,"%lf-min\n",f);	fclose(Do_opa);
	//}
	//else
	//{	FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelasB.txt","a");	fprintf(Do_opa,"%lf\n",f);	fclose(Do_opa);}

	emit SendMessage(initText+" ("+QString::number(iterx)+"="+QString::number(f)+") ");

	return (double)f;

}
//---------------------------------------
void	VolumeElasticRegistration::CalculateSingleNodDerivative(double *Deriv,double *Params)
{
	int pos=1;
	double der[6];
	int st=(fixBounds==true)?1:0;
	int count=0;
	double energy[2];energy[0]=0;energy[1]=0;
	//double alpha=3;
	double cn;
	int i,j,k,ii,jj,kk;

	der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;
	energy[0]=0;	energy[1]=0; count=0;
	//--------------------------------------
	// tu policze pochodne po x,y,z dla danego noda

	//GetTransFromCurrentNode(transx,Grid);
	//FILE *Do_opa;	Do_opa=fopen("d:\\t\\dd.txt","w");	fprintf(Do_opa,"start\n");	fclose(Do_opa);
//	parallelComputing = false;
	if (parallelComputing == false)


	{

		//SetCurrentNode(i,j,k);
		for (kk = posZstart; kk < posZstop; kk += optimStep)
			for (jj = posYstart; jj < posYstop; jj += optimStep)
				for (ii = posXstart; ii < posXstop; ii += optimStep)
				{
					//FILE *Do_opa;	Do_opa=fopen("d:\\t\\dd.txt","a");	fprintf(Do_opa,"%lf %lf %lf %lf %lf %lf\n",der[0],der[1],der[2],der[3],der[4],der[5]);	fclose(Do_opa);
					AddNewEnergyPoint(ii, jj, kk, count, energy, der);
				}
	}
	else
	{
		double* tabsEnergy;
		tabsEnergy = new double[numberOfThreadsUsed * 2];
		for (k = 0; k < this->numberOfThreadsUsed * 2; k++)
		{
			tabsEnergy[k] = 0;
		}
		double* tabsDeriv;
		tabsDeriv = new double[numberOfThreadsUsed * 6];
		for (k = 0; k < this->numberOfThreadsUsed * 6; k++)
		{
			tabsDeriv[k] = 0;
		}


		int* counts;
#pragma omp parallel  num_threads(numberOfThreadsUsed) private(counts)//private (counts,tabs)//shared(Q1,paddQ)  //num_threads(1)
		{
			int tid = omp_get_thread_num();


			counts = new int[numberOfThreadsUsed];
#pragma omp critical
			for (k = 0; k < this->numberOfThreadsUsed; k++)counts[k] = 0;
			//#pragma omp critical

#pragma omp for  nowait ordered schedule (guided) private(k) //private (counts)//ordered schedule(dynamic) p
			for (k = posZstart; k < posZstop; k += optimStep)
			{
				//int thNr= omp_get_thread_num();
				AddNewEnergyPointSet(k, counts[tid], tabsEnergy+tid*2, tabsDeriv+tid*6);
			}

#pragma omp critical
			{

				for (k = 0; k < numberOfThreadsUsed; k++)
					count += counts[k];
				delete[]counts;
			}

		}


		for (i = 0; i < this->numberOfThreadsUsed; i++)
		{
			energy[0] += tabsEnergy[i*2];
			energy[1] += tabsEnergy[i * 2+1];

			for (j = 0; j < 6; j++)
			{
				der[j] += tabsDeriv[i*6+j];
			}
		}
		
	

		delete[]tabsEnergy;
		delete[]tabsDeriv;
	}
		//				der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;
			//CalculateDerivativesAtPoint(i,j,k,der,drvTempTab,count, bTr1,bTr2);
			//---------------------------------------

			cn=(count>0)?1.0/count:1.0;

			Deriv[1]=2*cn*(der[0]+alpha*der[3]);
			Deriv[2]=2*cn*(der[1]+alpha*der[4]);
			Deriv[3]=2*cn*(der[2]+alpha*der[5]);

}
//-------------------------------------
void	VolumeElasticRegistration::CalculateTotalDerivative(double *Deriv,double *Params)
{
	double mm=(maxP+minP)/(double)optimStep;
	maxNrOfSamples=	(1+(int)(mm*fx))*
		(1+(int)(mm*fy))*
		(1+(int)(mm*fz)); //SPRAWDZIC!!


	double *drvTempTab=new double [maxNrOfSamples];
	FloatGrid3D *g1=new FloatGrid3D();
	FloatGrid3D *g2=new FloatGrid3D();
	g1->Copy(*Grid);
	g2->Copy(*Grid);
	BSplineGridTransformation *bTr1=new BSplineGridTransformation(g1,nnx,nny,nnz);
	BSplineGridTransformation *bTr2=new BSplineGridTransformation(g2,nnx,nny,nnz);
	bTr1->SetupForwardAndBackwardScalingFactors(nx1,ny1,nz1,mx1,my1,mz1);
	bTr2->SetupForwardAndBackwardScalingFactors(nx1,ny1,nz1,mx1,my1,mz1);

	QTime t;
	t.start();
	SetParamsFromArrayToGrid(Grid,Params,fixBounds);
	//jade po kolejnych nodach wyliczam dla nich pochodne i wrzucam do Deriv[pos++]
	int pos=1;
	double der[6];
	int st=(fixBounds==true)?1:0;
	int count=0;
	double energy[2];energy[0]=0;energy[1]=0;

	double cn;
	int i,j,k,ii,jj,kk;
	int l;
	//for (k=st;k<nz-st;k++)
	//	for (j=st;j<ny-st;j++)
	//		for (i=st;i<nx-st;i++)
	for (l=0;l<nrOfNodes;l++)
	{
		i=nodes[l].x;j=nodes[l].y;k=nodes[l].z;

		der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;
		energy[0]=0;	energy[1]=0; count=0;
		CalculateDerivativesAtPoint(i,j,k,der,drvTempTab,count, bTr1,bTr2);
		//---------------------------------------

		cn=(count>0)?1.0/count:1.0;

		Deriv[pos++]=cn*(der[0]+alpha*der[3]);
		Deriv[pos++]=cn*(der[1]+alpha*der[4]);
		Deriv[pos++]=cn*(der[2]+alpha*der[5]);

	}

	//FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelasB.txt","a");	fprintf(Do_opa,"der=%d\n",t.elapsed());	fclose(Do_opa);
	emit SendMessage(initText+" ("+QString::number(iterx)+"="+"DERIV"+") ");
	delete bTr1;
	delete bTr2;
	delete g1;
	delete g2;
	delete []drvTempTab;


}
//-------------------------------------
void VolumeElasticRegistration::SetParamsFromGridToArray(FloatGrid3D *Grid,double *Params, bool fixbounds)
{
	int pos=1;
	double transx[4];
	int i;

	if ((nodes!=NULL)&&(nrOfNodes>0))
	{
		for (i=0;i<nrOfNodes;i++)
		{
			SetCurrentNode(nodes[i].x,nodes[i].y,nodes[i].z);
			GetTransFromCurrentNode(transx,Grid);
			Params[pos++]=transx[1];Params[pos++]=transx[2];Params[pos++]=transx[3];
		}
	}
	else
	{
		int j,k;
		int st=(fixbounds==true)?1:0;
		for (k=st;k<nz-st;k++)
			for (j=st;j<ny-st;j++)
				for (i=st;i<nx-st;i++)
				{
					SetCurrentNode(i,j,k);
					GetTransFromCurrentNode(transx,Grid);
					Params[pos++]=transx[1];Params[pos++]=transx[2];Params[pos++]=transx[3];
				}
	}

}
//---------------------------------------
void VolumeElasticRegistration::SetParamsFromArrayToGrid(FloatGrid3D *Grid,double *Params, bool fixbounds)
{
	int pos=1;
	double transx[4];
	int i;

	if ((nodes!=NULL)&&(nrOfNodes>0))
	{
		for (i=0;i<nrOfNodes;i++)
		{
			transx[1]=Params[pos++];transx[2]=Params[pos++];transx[3]=Params[pos++];
			SetCurrentNode(nodes[i].x,nodes[i].y,nodes[i].z);
			SetTransToCurrentNode(transx,Grid);
		}
	}
	else
	{
		int j,k;
		int st=(fixbounds==true)?1:0;
		for (k=st;k<nz-st;k++)
			for (j=st;j<ny-st;j++)
				for (i=st;i<nx-st;i++)
				{
					transx[1]=Params[pos++];transx[2]=Params[pos++];transx[3]=Params[pos++];
					SetCurrentNode(i,j,k);
					SetTransToCurrentNode(transx,Grid);
				}
	}


}
//-----------------
void VolumeElasticRegistration::OptimizeBig(int _iter,int _optimStep,bool _randomSampling,int multiressteps, bool _fixBounds)
{
	fixBounds=_fixBounds;
	GetFValue=&VolumeElasticRegistration::GetTotalFunctionValue;
	GetDerivatives=&VolumeElasticRegistration::CalculateTotalDerivative;

	optimStep=_optimStep;
	randomSampling=_randomSampling;

	int i,j,k,l,m;
	QString t;
	int ii,jj,kk;
	int pos;
	double testtol;

	double Tolerance=10e-4;//10e-4;
	double tolX=10e-4;
	int Iterations=0;
	double FValue=0;
	double threshold=10.0;

	int NumberOfOptimParams;
	//FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelasB.txt","w");	fprintf(Do_opa,"start\n");	fclose(Do_opa);

	double e,e1;

	//zabezpieczenie zeby rozmiar gridu nie byl wiekszy od zbioru
	int min=MIN3(nnx,nny,nnz);
	int maxgrid=MAX3(nx,ny,nz);
	int multimax=(maxgrid)*pow((double)2,multiressteps);

	if (multimax>min)
	{
		int a;
		multiressteps=0;
		while((a=(maxgrid)*pow((double)2,multiressteps+1))<min) 	
			multiressteps+=1;
	}
	//////////////////////////////////

	//Function *f=new TempFunction();
	OptimizationDFPowell *Optim=NULL;
	int optimStepX=optimStep;
	optimStep=1;
	ModelDataScaled=NULL;

	for(m=0;m<multiressteps;m++)
	{
		iterx=0;
		fval=INT_MAX;

		if ((ModelDataScaled!=NULL)&& (ModelDataScaled!=ModelData)) 
			delete ModelDataScaled;
		if (optimStepX>1)
			ModelDataScaled=ScaleDataSet(ModelData,1.0/optimStepX);
		else
			ModelDataScaled=ModelData;

		BSplineTransform->SetupForwardAndBackwardScalingFactors(
			ModelDataScaled->GetNx(),ModelDataScaled->GetNy(),ModelDataScaled->GetNz(),
			ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz());
		SetSizeVariables(Grid);

		pos=0;

		nrOfNodes=GetNodesToOptimization(Grid,transx,fixBounds,threshold,nodes);
		t=nrOfNodes;
		NumberOfOptimParams=3*nrOfNodes;
		/*
		NumberOfOptimParams=(fixBounds)?
		(Grid->GetNx()-2)*(Grid->GetNy()-2)*(Grid->GetNz()-2):
		Grid->GetNx()*Grid->GetNy()*Grid->GetNz();
		NumberOfOptimParams*=3;
		*/

		nrOfParams=NumberOfOptimParams;
		derivs=new double[NumberOfOptimParams+1];
		if (transx!=NULL) delete []transx;
		transx=new double[NumberOfOptimParams+1];

		//		t=QString::number((nx-2*st)*(ny-2*st)*(nz-2*st)*iter);
		pos=0;

		double ss=1.0;
		//double ss=1.0;
		smallStepX=BSplineTransform->GetGxDivNx()*ss;
		smallStepY=BSplineTransform->GetGyDivNy()*ss;
		smallStepZ=BSplineTransform->GetGzDivNz()*ss;



		/*SetParamsFromGridToArray(Grid,transx,fixBounds);
		Optim=new OptimizationDFPowell(transx,NumberOfOptimParams,tolX,&Iterations,&FValue,this); //:) 
		Optim->StartOptimization();
		delete Optim;
		*/
		for (int tt=0;tt<1;tt++)
			GradientDescentOptimizer(transx,NumberOfOptimParams,tolX,&Iterations,&FValue);	
		SetParamsFromArrayToGrid(Grid,transx,fixBounds);

		//FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelasB.txt","a");	fprintf(Do_opa,"iter=%d\n",iterx);	fclose(Do_opa);
		if (multiressteps-m>1)
		{
			//przebudowac
			UpdateData(nx*2-1,ny*2-1,nz*2-1);
			delete TempGrid;
			TempGrid=new FloatGrid3D();
			TempGrid->Copy(*Grid);
			if (optimStepX>=2) optimStepX/=2;
		}
		delete []derivs;derivs=NULL;

		delete []transx;transx=NULL;
		if (nodes!=NULL) 
		{		
			delete []nodes;
			nodes=NULL;
		}
	}
	if ((ModelDataScaled!=NULL)&& (ModelDataScaled!=ModelData)) 
		delete ModelDataScaled;
	OrigGrid->Copy(*Grid);
	stopOptimization=false;
}
//------------------------------------
double  VolumeElasticRegistration::GetGradI(int i,int j,int k,BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf)
{
	double plus, minus,value;
	double Point[3];

	plusTransf->TransformPoint(i,j,k,Point);
	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&		(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))) return 0;
	plus=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2]);
	minusTransf->TransformPoint(i,j,k,Point);
	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&		(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1)))	return 0;
	minus=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2]);
	value=0.5*(plus-minus);
	return value;

};
//-------------------------------------
void VolumeElasticRegistration::CalculateDerivativesAtPoint(int i,int j,int k,double *der,double *drvTempTab,int &count, BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf)
{
	int ii,jj,kk;
	double *ptr=drvTempTab;
	SetCurrentNode(i,j,k);

	if (der!=NULL)
	{
		int pos=0;
		for(kk=posZstart;kk<posZstop;kk+=optimStep)
			for(jj=posYstart;jj<posYstop;jj+=optimStep)
				for(ii=posXstart;ii<posXstop;ii+=optimStep)
					AddGradPart1(ii,jj,kk,count,der,drvTempTab[pos++]);

		AddGradPart2(der,drvTempTab,smallStepX,0,plusTransf,minusTransf);
		AddGradPart2(der,drvTempTab,smallStepY,1,plusTransf,minusTransf);
		AddGradPart2(der,drvTempTab,smallStepZ,2,plusTransf,minusTransf);
	}

}
//---------------------------------------------------
void VolumeElasticRegistration::AddGradPart2(double *der,double *drvTempTab,double smallStep,int view,BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf)
{

	FloatGrid3D *plusGrid	=	plusTransf->GetGrid();
	FloatGrid3D *minusGrid	=	minusTransf->GetGrid();
	int ii,jj,kk;
	int pos=Grid->GetPos(NodX,NodY,NodZ);
	double val;//bo plus i minus maja byc takie same na starcie
	//int v=view+1;
	switch (view)
	{
	case 0:	val=plusGrid->GetDXNP(pos);	plusGrid->SetDx(pos,val+smallStep);	minusGrid->SetDx(pos,val-smallStep);	break;
	case 1:	val=plusGrid->GetDYNP(pos);	plusGrid->SetDy(pos,val+smallStep);	minusGrid->SetDy(pos,val-smallStep);	break;
	case 2:	val=plusGrid->GetDZNP(pos);	plusGrid->SetDz(pos,val+smallStep);	minusGrid->SetDz(pos,val-smallStep);	break;

	}

	for(kk=posZstart;kk<posZstop;kk+=optimStep)
		for(jj=posYstart;jj<posYstop;jj+=optimStep)
			for(ii=posXstart;ii<posXstop;ii+=optimStep)
			{
				if (*drvTempTab!=0)
					der[view]+=*(drvTempTab)*GetGradI(ii,jj,kk,plusTransf,minusTransf);
				drvTempTab++;
			}

			switch (view)
			{
			case 0:	plusGrid->SetDx(pos,val);	minusGrid->SetDx(pos,val);	break;
			case 1:	plusGrid->SetDy(pos,val);	minusGrid->SetDy(pos,val);	break;
			case 2:	plusGrid->SetDz(pos,val);	minusGrid->SetDz(pos,val);	break;
			}

}
//---------------------------------------
void VolumeElasticRegistration::AddGradPart1(int i,int j,int k, int &count,double *drv,double &drvel)
{
	int pa[3];pa[0]=i;pa[1]=j;pa[2]=k;
	int nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=NodZ;
	double ddDdP[9];
	double ddDdPL[3];
	double ddDdQ;
	double ddDPdQ[3];
	double Point[3];
	BSplineTransform->TransformPoint(pa,Point,true,nodPos,ddDdP,ddDdPL,&ddDdQ,ddDPdQ);

	double m=0;
	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))
	{	
		m=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2]);
		double a=ModelDataScaled->GetVoxelValue(i,j,k)-m;
		drvel=-a*ddDdQ;

		drv[3]+=(ddDdP[0]*ddDPdQ[0]+ddDdP[3]*ddDPdQ[1]+ddDdP[6]*ddDPdQ[2]);
		drv[4]+=(ddDdP[1]*ddDPdQ[0]+ddDdP[4]*ddDPdQ[1]+ddDdP[7]*ddDPdQ[2]);
		drv[5]+=(ddDdP[2]*ddDPdQ[0]+ddDdP[5]*ddDPdQ[1]+ddDdP[8]*ddDPdQ[2]);	
		count++;
	}
}
//----------------------------------
void VolumeElasticRegistration::AddNewEnergyPointWithoutDerivs(int i,int j,int k, int &count,double *energy)
{
	double Point[3];
	int pa[3];pa[0]=i;pa[1]=j;pa[2]=k;
	int nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=NodZ;
	double ddDdPL[3];
	BSplineTransform->TransformPoint(pa,Point,nodPos,ddDdPL);
	double m=0;
	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))
	{	
		m=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2])*maxFactor;
		if (m > 0)
		{
			m = m;
		}
		double a=ModelDataScaled->GetVoxelValue(i,j,k)*maxFactor-m;
		energy[0]+=a*a;	
		energy[1]+=(ddDdPL[0]+ddDdPL[1]+ddDdPL[2]);	
		count++;
	}


}
//--------------------------------
void VolumeElasticRegistration::GetVolumeSpacing(int minX,int maxX,int minY,int maxY,int minZ, int nx,int ny,int slicesize,int step,int &start,int &sy,int &sz)
{
	int sizeX=maxX-minX;
	int sizeY=maxY-minY;
	int xy=nx*minY+minX;

	int RestX=(sizeX%step)+1+(nx-1-maxX);
	int RestY=((sizeY%step)+(ny-1-maxY))*nx;
	int SSY=(step-1)*nx+minX;
	int SSZ=(step-1)*slicesize;

	start=xy+minZ*slicesize;
	sy=RestX-step+SSY;
	sz=RestY+SSZ-SSY+xy;
}
//---------------------------------
int VolumeElasticRegistration::GetNodesToOptimization(FloatGrid3D *Grid,double *Params, bool fixbounds,double threshold,Node *&Nodes)
{

	int NrOfNodes=(fixbounds)?
		(Grid->GetNx()-2)*(Grid->GetNy()-2)*(Grid->GetNz()-2):
	Grid->GetNx()*Grid->GetNy()*Grid->GetNz();
	//double *mEnergyTab=new double[NrOfNodes];

	double energy[2];energy[0]=0;energy[1]=0;
	int pos=0;
	int i,j,k;
	int ii,jj,kk;
	double f;
	int Nr=0;
	int st=(fixbounds==true)?1:0;

	Node *tmpNodes=new Node[NrOfNodes];

	for (k=st;k<nz-st;k++)
		for (j=st;j<ny-st;j++)
			for (i=st;i<nx-st;i++)
			{
				energy[0]=0;	energy[1]=0; count=0;
				SetCurrentNode(i,j,k);

				for(kk=posZstart;kk<posZstop;kk+=optimStep)
					for(jj=posYstart;jj<posYstop;jj+=optimStep)
						for(ii=posXstart;ii<posXstop;ii+=optimStep)
							AddNewEnergyPointWithoutDerivs(ii,jj,kk,count,energy);

				f=energy[0]+alpha*energy[1];
				//if (count >0)	if (f!=0) f/=(double)count;
				f= (count >0)?f/=(double)count:INT_MAX;
				//mEnergyTab[pos++]=f;
				if (f>threshold) 
				{
					tmpNodes[Nr].x=i;tmpNodes[Nr].y=j;tmpNodes[Nr].z=k;
					Nr++;
				}
			}

			//przepisac do wlasciwej	
			if (Nodes!=NULL) delete []Nodes;
			Nodes=new Node[Nr];
			for (i=0;i<Nr;i++)
				Nodes[i]=tmpNodes[i];
			delete []tmpNodes;

			//FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelasC.txt","w");	fprintf(Do_opa,"Nodes: %d,%d",NrOfNodes,Nr);
			//sort
			//QSortDouble(mEnergyTab,0,NrOfNodes-1);
			/*FILE *Do_opa;	Do_opa=fopen("d:\\t\\powellvolelasC.txt","a");	fprintf(Do_opa,"Nodes: %d\n",NrOfNodes);	

			for (i=0;i<NrOfNodes;i++)
			fprintf(Do_opa,"%d %lf\n",i,mEnergyTab[NrOfNodes-i-1]);	

			fclose(Do_opa);*/



			//delete []mEnergyTab;

			return Nr;
}
//----------------------------------------------------------------
void	VolumeElasticRegistration::GradientDescentOptimizer(double *params0,int nrOfParams,double tol,int *iter,double *fval)
{
	/*	if (params0!=NULL)
	{

	int itmax=100;
	double *der=new double [nrOfParams+1];
	double *params1=new double [nrOfParams+1];
	double e0,e1,tmpE;
	int i,its;
	double lambda=1.0;
	double sLambda;
	double fLambda=15;
	double tmpLambda;
	double nrmSq;
	double dotPr;
	double tmp1,tmp2;

	e0=GetFunctionValue(params0);
	CalculateDerivative(der,params0);

	for(i=1;i<=nrOfParams;i++)
	params1[i]=params0[i]-lambda*der[i];

	e1=GetFunctionValue(params1);

	if (e1>e0)
	{
	lambda/=fLambda;
	for(i=1;i<=nrOfParams;i++)
	params1[i]=params0[i]-lambda*der[i];
	e1=GetFunctionValue(params1);
	}

	if (fabs(e1-e0)<tol)
	{
	for(i=1;i<=nrOfParams;i++)
	params0[i]=params1[i];
	*fval=e1;
	delete []params1;delete []der; *fval=e1; return ;
	}

	for (its=0;its<itmax;its++) 
	{
	CalculateDerivative(der,params1);

	tmpE=e0-e1;
	nrmSq=0;
	dotPr=0;
	for(i=1;i<=nrOfParams;i++)
	{
	//--- sLambda -----------------------
	tmp1=params1[i]-params0[i];
	nrmSq+=tmp1*tmp1;
	dotPr+=tmp1*der[i];
	}
	tmp2=tmpE+dotPr;	
	sLambda=(tmp2!=0)?0.5*(nrmSq/tmp2):sLambda=0;

	for(i=1;i<=nrOfParams;i++)
	params0[i]=params1[i]-sLambda*der[i];


	e0=e1;
	e1=GetFunctionValue(params0);

	lambda=sLambda;
	if (e1>e0)
	{	//???????????
	lambda/=fLambda;
	for(i=1;i<=nrOfParams;i++)
	params1[i]=params0[i]-lambda*der[i];
	e1=GetFunctionValue(params1);
	}
	//else
	//	lambda=sLambda;

	if (fabs(e1-e0)<tol)
	{
	for(i=1;i<=nrOfParams;i++)
	params0[i]=params1[i];
	delete []params1;delete []der; *fval=e1; return ;
	}
	//-----------------------------------
	}

	for(i=1;i<=nrOfParams;i++)
	params0[i]=params1[i];
	delete []params1;delete []der; *fval=e1; return ;

	}
	return ;
	*/

	if (params0!=NULL)
	{

		//FILE *Do_opa;
		int it=10;
		double *der=new double [nrOfParams+1];
		double *params1=new double [nrOfParams+1];
		double e0,e1,tmpE;
		int i,its;
		double lambda=0.1;
		double sLambda;
		double fLambda=15;
		double tmpLambda;
		double nrmSq;
		double dotPr;
		double tmp1,tmp2;
		double epsilon=1e+6;



		its=0;

		// taka wartosc fkcji jest na starcie
		e0=GetFunctionValue(params0);
		CalculateDerivative(der,params0);

		//ustal nowe parametry i wartosc fkcji
		for(i=1;i<=nrOfParams;i++)
			params1[i]=params0[i]-lambda*der[i];
		/*	e1=GetFunctionValue(params1);
		//-------------------------------------
		Do_opa=fopen("d:\\t\\opter.txt","a");fprintf(Do_opa,"start %lf nowa %lf\n",e0,e1);fclose(Do_opa);	

		// jezeli nowa wartosc fkcji jest gorsza
		if (e1>e0)
		{
		do
		{
		it--;
		//zmniejsz lambde i oblicz nowa wartosc fkcji
		lambda/=2.0;
		for(i=1;i<=nrOfParams;i++)
		params1[i]=params0[i]-lambda*der[i];
		e1=GetFunctionValue(params1);
		Do_opa=fopen("d:\\t\\opter.txt","a");fprintf(Do_opa,"gorzej - po zmniejszeniu %lf,%d\n",e1,it);fclose(Do_opa);	
		if (e1<e0+epsilon) break;
		// rob to do czasu az wartosc fkcji bedzie lepsza lub iteracje Ci sie skoncza
		}while (it>0);
		// a jesli jest lepsza
		}
		else
		{
		//zwiekszaj krok lambda do czasu az e1 nie bedzi lepsze od poprzedniego
		do
		{
		Do_opa=fopen("d:\\t\\opter.txt","a");fprintf(Do_opa,"lepiej - zwieksz %lf %d\n",e1,it);fclose(Do_opa);	

		e0=e1;
		it--;
		lambda*=2.0;

		for(i=1;i<=nrOfParams;i++)
		params1[i]=params0[i]-lambda*der[i];
		e1=GetFunctionValue(params1);

		Do_opa=fopen("d:\\t\\opter.txt","a");fprintf(Do_opa,"lepiej - nowy test %lf %d\n",e1,it);fclose(Do_opa);	

		}while ((e1<e0)&&(it>0));

		if (it>0)
		{
		lambda/=2.0;
		for(i=1;i<=nrOfParams;i++)
		params1[i]=params0[i]-lambda*der[i];
		}
		}
		*/
		//	if (it>0)
		//	{
		//Do_opa=fopen("d:\\t\\opter.txt","a");fprintf(Do_opa,"ustalam nowe parametry \n");fclose(Do_opa);	

		for(i=1;i<=nrOfParams;i++)
			params0[i]=params1[i];	
		//	}

		delete []der;
		delete []params1;

	}







}
//----------------------------------------------------------------
RawDataSet *VolumeElasticRegistration::ScaleDataSet(RawDataSet *data, double factor)
{

	if (factor>0)
	{
		int newNx=data->GetNx()*factor;
		int newNy=data->GetNy()*factor;
		int newNz=data->GetNz()*factor;
		double newSx=data->GetXscale()/factor;
		double newSy=data->GetYscale()/factor;
		double newSz=data->GetZscale()/factor;
		RawDataSet *dataX=new RawDataSet(newNx,newNy,newNz,newSx,newSy,newSz);

		double factX=(double)data->GetNx()/(double)newNx;
		double factY=(double)data->GetNy()/(double)newNy;
		double factZ=(double)data->GetNz()/(double)newNz;

		int i,j,k;
		double x,y,z;
		unsigned short *Q=dataX->GetDataArray();
		for(k=0;k<newNz;k++)
			for(j=0;j<newNy;j++)
				for(i=0;i<newNx;i++)
				{
					x=(int)((double)i*factX);
					y=(int)((double)j*factY);
					z=(int)((double)k*factZ);
					*(Q++)=data->GetVoxelValue(x,y,z);
				}

				return dataX;
	}
	else return NULL;
}
//----------------------------------
//---------------------------
void VolumeElasticRegistration::SlotSetParalellComputing(bool p, int nr)
{
	this->parallelComputing = p;
	this->numberOfThreadsUsed = nr;

}