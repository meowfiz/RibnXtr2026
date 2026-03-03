#include "TextureSegmentation.h"


TextureSegmentation::TextureSegmentation(RawDataSet	*_rawData,unsigned char	*_maskData,	RawDataSet *_distanceMapRawData)
{


	intensityProbabilityRawData=NULL;
	rawData=_rawData;
	//	maskData=_maskData;
	if (_maskData!=NULL)
	{
		maskData=new unsigned char[rawData->GetVolumeSize()];
		memcpy(maskData,_maskData,rawData->GetVolumeSize());
	}
	else
	{
		maskData=NULL;
	}
	//boundaryMaskData=new unsigned	char[rawData->GetVolumeSize()];
	//memset(boundaryMaskData,0,rawData->GetVolumeSize());
	//UpdateBoundaryMaskData(maskData,boundaryMaskData);

	//	implicitDistanceDataSet=NULL;
	implicitDistanceDataSet=new	RawDataSet();
	implicitDistanceDataSet->Copy(*rawData,false);
	//implicitDistanceDataSet->ZeroQ();

	DistanceMap	*mapDist=new DistanceMap(rawData,0,0,maskData);
	factorDistImplicit=mapDist->Generate3DUnsignedDistanceMapVolume(implicitDistanceDataSet);
	delete mapDist;
	//implicitDistanceDataSet->SaveRawDataSet("c:\\aa.dat");
	//	void	Generate3DUnsignedDistanceMapVolume(RawDataSet *rdata);

	distanceMapRawData=_distanceMapRawData;
	maskTab=NULL;
	numberOfMaskVoxels=-1;
	expTab=NULL;
	expTabLength=-1;
	intensityProbabilityTab=NULL;
	BSplineTransform=NULL;

	transx=NULL;
	Grid=NULL;
	pDef[1]=-1000;	pDef[2]=-1000;	pDef[3]=-1000;
	oldNod[0]=-1000;	oldNod[1]=-1000;	oldNod[2]=-1000;

	Step[0]=1.0;//fx;
	Step[1]=1.0;//fy;
	Step[2]=1.0;//fz;
	maxDisp=1.5;
	factorDist=0;


	minP=1.5;
	maxP=1.5;

}
//---------------------------------------------
TextureSegmentation::~TextureSegmentation()
{
	rawData=NULL;
	maskData=NULL;
	distanceMapRawData=NULL;

	if (implicitDistanceDataSet!=NULL) delete implicitDistanceDataSet;
	implicitDistanceDataSet=NULL;

	if (maskTab!=NULL) delete maskTab;
	maskTab=NULL;

	if (expTab!=NULL) delete []expTab;
	expTab=NULL;

	//if (boundaryMaskData!=NULL) delete boundaryMaskData;
	//boundaryMaskData=NULL;


	if (intensityProbabilityTab!=NULL) delete []intensityProbabilityTab;
	intensityProbabilityTab=NULL;

	if (distanceMapRawData!=NULL) delete []distanceMapRawData;
	distanceMapRawData=NULL;



}
//----------------------------------------------
void TextureSegmentation::GetIntensityProbabilityMap(RawDataSet	*_data)
{
	//if ((maskData==NULL)||(rawData==NULL)||(_data==NULL))	return;	
	if ((maskData!=NULL)&&(rawData!=NULL)&&(_data!=NULL))
	{
		double sigma=1.5;
		//intensityFactor=1.0/(double)rawData->GetMaxValue();
		intensityFactor=sqrt(40*sigma*sigma)/(double)rawData->GetMaxValue();
		intensityFactor2=intensityFactor*intensityFactor;

		numberOfMaskVoxels=GetNrOfMaskVoxels();

		if (maskTab!=NULL) delete []maskTab;
		maskTab=new	unsigned short[numberOfMaskVoxels];
		GetMaskTab(maskTab,numberOfMaskVoxels);

		int	nr=rawData->GetMaxValue();
		if (expTab!=NULL) delete []expTab;
		expTab=new double[2*nr+2];
		GetExpTab(expTab,nr,sigma,(double)numberOfMaskVoxels);


		//znajde min i max zeby	odpowiednio	przeskalowac i do obrazka

		//double min=GetIntensityProbability(rawData->GetMaxValue(),nr);
		//double max=GetIntensityProbability(rawData->GetMinValue(),nr);





		int	i;
		unsigned short *Q=_data->GetDataArray();
		unsigned short *Q0=rawData->GetDataArray();
		unsigned short val;


		if (intensityProbabilityTab!=NULL) delete []intensityProbabilityTab;
		intensityProbabilityTab=new	double[nr];

		double min,max;
		GetIntensityProbabilityTab(intensityProbabilityTab,nr,min,max,sigma);//ta	sie	przyda
		double factor=(max-min!=0)?65535.0/(max-min):1;

		for(i=0;i<_data->GetVolumeSize();i++)
			Q[i]=(unsigned short)((intensityProbabilityTab[Q0[i]]-min)*factor);


		Q=NULL;
		Q0=NULL;


		delete []intensityProbabilityTab;
		intensityProbabilityTab=NULL;

		delete maskTab;
		maskTab=NULL;

		delete []expTab;
		expTab=NULL;
	}

	_data->CalculateHistogram();
}
//-----------------------------------------------
int	TextureSegmentation::GetNrOfMaskVoxels()
{
	if (maskData==NULL)	return -1;

	int	i;
	int	volume=0;

	for(i=0;i<rawData->GetVolumeSize();i++)
		if (maskData[i]==1)	volume++;
	return volume;
}
//---------------------------------------------
void TextureSegmentation::GetMaskTab(unsigned short	*_maskTab,int numberOfVoxels)
{
	int	i;
	int	pos=0;
	unsigned short *Q=rawData->GetDataArray();
	for(i=0;i<rawData->GetVolumeSize();i++)
		if (maskData[i]==1)
			_maskTab[pos++]=Q[i];
}
//--------------------------------------------
void TextureSegmentation::GetExpTab(double *_Tab,int number,double sigma,double	v)
{
	const double tolX=10e-8;
	//	double fct=255.0/(double)number;
	//	fct*=fct;
	if (fabs(sigma*v)>tolX)	
	{
		//if ((sigma==0)||(v==0)) return;//	dziadostwo
		int	i; 

		double b=1/(2*sigma*sigma);
		double tmp;
		double t;
		double min= INT_MAX;
		double max=- INT_MAX;

		int qmin; int qmax;
		for	(i=0;i<=2*number;i++)
		{
			t=-(double)((intensityFactor*(i-number))*(intensityFactor*(i-number)))*b;
			if (t<min)	{min=t;qmin=i;}
			if (t>max)	{max=t;qmax=i;}
			tmp=exp(t);
			_Tab[i]=tmp;
		}
		t=t;
	}
}
//--------------------------------------------
double TextureSegmentation::GetIntensityProbability(unsigned short intensity,int max,double sigma)
{
	double prop=0.0;
	int	i;

	for(i=0;i<numberOfMaskVoxels;i++)
		prop+=expTab[intensity-maskTab[i]+max];

	prop /= (sqrt(2.0*M_PI)*sigma*numberOfMaskVoxels);

	return prop;
}
//---------------------------------------------
void TextureSegmentation::GetIntensityProbabilityTab(double	*tab,int nr,double &min,double &max,double sigma)
{
	min= INT_MAX;
	max=- INT_MAX;;
	if (tab==NULL) return;
	int	i;
	for(i=0;i<nr;i++)
	{
		tab[i]=GetIntensityProbability(i,nr,sigma);
		if (tab[i]<min)	min=tab[i];
		if (tab[i]>max)	max=tab[i];
	}
}
//----------------------------------------------
RawDataSet	*TextureSegmentation::ExtractEdgesMap(int radius,double	stddev,int dim)
{

	EdgesExtraction3D *edges=new EdgesExtraction3D(rawData);
	return edges->ApplyCanny3DEdgesExtraction((double)radius,stddev,dim);
	delete edges;
	//if ((rawData==NULL)||(_data==NULL)) return;
	/*RawDataSet rData=*rawData;
	RawDataSet *tempData=&rData;


	double stddev=2.5;
	int	radius=10.0;
	GaussianSmoothingFiltering(	stddev,	 radius, tempData,_data);*/
	//	radius=4.0;
	//	GaussianSmoothingFiltering(	stddev,	 radius, _data,tempData);

	/*
	int	i;
	int	val;
	unsigned short *Q=_data->GetDataArray();
	unsigned short *Q1=tempData->GetDataArray();

	for	(i=0;i<_data->GetVolumeSize();i++)
	{
	val=Q1[i]-Q[i];
	if (val<0) val=0;
	//else	val=255;
	Q[i]=val;
	}
	Q1=NULL;
	Q=NULL;
	*/


}
//----------------------------------------------
//----------------------------------------------
unsigned char	*TextureSegmentation::DeformationBasedSegmetation2(FloatGrid3D *_Grid,int gridS,double sh1,double	sh2, double intens1,double intens2)
{
	shape1		=sh1;
	shape2		=sh2;
	intense1	=intens1;
	intense2	=intens2;
	gridSize	=gridS;

	GetFValue=&TextureSegmentation::GetEnergy;

	if ((rawData!=NULL)&&(distanceMapRawData!=NULL)&&(maskData!=NULL))
	{

		if (rawData->CompareWithOtherDataSet(distanceMapRawData)==true)
		{
			int	size=gridSize;
			iter=Round(intense2);
			int	st=1;
			optimStep=1;
			boundaryMaskData=NULL;
			boundaryMaskData=new unsigned char[rawData->GetVolumeSize()];
			memset(boundaryMaskData,0,rawData->GetVolumeSize());
			UpdateBoundaryMaskData(maskData,boundaryMaskData);


			///
			if (intensityProbabilityRawData!=NULL) delete intensityProbabilityRawData;
			intensityProbabilityRawData=new	RawDataSet();
			intensityProbabilityRawData->Copy(*rawData,false);
			intensityProbabilityRawData->ZeroQ();

			GetIntensityProbabilityMap(intensityProbabilityRawData);


			if (intensityProbabilityRawData->GetMaxValue()>0)
				factorIntensity=1.0/(double)intensityProbabilityRawData->GetMaxValue();
			else 
				factorIntensity=1.0;

			if (rawData->GetMaxValue()>0)
				factorRaw=255.0/(double)rawData->GetMaxValue();
			else 
				factorRaw=1.0;

			if (distanceMapRawData->GetMaxValue()>0)
				factorDist=255.0/(double)distanceMapRawData->GetMaxValue();
			else
				factorDist=1.0;

			///

			nx=size;
			ny=size;
			nz=size;

			if (Grid!=NULL)	delete Grid;
			//Grid=new FloatGrid3D();
			//Grid->Copy(*_Grid,true);
			Grid=new FloatGrid3D(nx,ny,nz);


			nx1=rawData->GetNx();
			ny1=rawData->GetNy();
			nz1=rawData->GetNz();
			mx1=nx1;my1=ny1;mz1=nz1;
			fx=((nx>0)&&(nx1>0))?(double)(nx1-1)/(nx-1):1;
			fy=((ny>0)&&(ny1>0))?(double)(ny1-1)/(ny-1):1;
			fz=((nz>0)&&(nz1>0))?(double)(nz1-1)/(nz-1):1;
			tdx=1.0/fx;
			tdy=1.0/fy;
			tdz=1.0/fz;
			nnx=(double)(nx1-1);
			nny=(double)(ny1-1);
			nnz=(double)(nz1-1);





			if (BSplineTransform!=NULL)	delete BSplineTransform;
			BSplineTransform=new BSplineGridTransformation(Grid,nnx,nny,nnz);


			if (transx!=NULL) delete []transx;
			transx=new double[4];

			double temptrans[4];
			double Tolerance=10e-2;
			double tolX=10e-8;
			int	Iterations=0;
			double FValue=0;

			bool stopOptimization=false;

			int	NumberOfOptimParams=3;

			OptimizationPowell *Optim=NULL;

			int	i,j,k,l;


			int	numberOfPointsInsideSection;
			double e,e1;

			double testtol=MIN3(BSplineTransform->GetGxDivNx(),BSplineTransform->GetGyDivNy(),BSplineTransform->GetGzDivNz())/100.0;
			double bounds[6];

			for(l=0;l<iter;l++)
			{
				for	(k=st;k<nz-st;k++)
				{
					emit SendMessage("iter:	(" + QString::number(l)+"/"	+QString::number(iter-1)+")" +"	(" +	QString::number(k)+")");//	+	"/"	+	QString::number(j)	+"/"	+QString::number(i)+")"	);
					/*	if (k==13)
					{
					k=k;
					}*/
					for	(j=st;j<ny-st;j++)
						for	(i=st;i<nx-st;i++)
						{

							if (stopOptimization==false)	
							{
								SetCurrentNode(i,j,k);
								GetTransFromCurrentNode(transx);
								numberOfPointsInsideSection=CheckNumberOfPointsInsideSection(i,j,k);
								if (numberOfPointsInsideSection>0)
								{

									e=GetFunctionValue(transx);	temptrans[1]=transx[1];	temptrans[2]=transx[2];	temptrans[3]=transx[3];

									Optim=new OptimizationPowell(transx,NumberOfOptimParams,Tolerance,&Iterations,&FValue,this,testtol); //:) 
									Optim->StartOptimization();
									delete Optim;
									e1=FValue;
									// jezeli stara-nowa jest w	gr tolerancji to ustaw stara
									if (fabs(e1-e)<tolX)
									{
										transx[1]=temptrans[1];	transx[2]=temptrans[2];	transx[3]=temptrans[3];e1=e;
										//	Do_opa=fopen("c:\\powellvolelas.txt","a");fprintf(Do_opa,"qq\n");fclose(Do_opa);	
									}
									SetTransToCurrentNode(transx);

								}
							}
						}
				}
				//nowa deformacja

				GetDeformationBounds(bounds);
				if (CreateNewMask(bounds)==false)
				{
					emit SignalSendNewMask(maskData);
					UpdateBoundaryMaskData(maskData,boundaryMaskData);
					GetIntensityProbabilityMap(intensityProbabilityRawData);
				}
				else
					break;
				if (l<iter-1)	Grid->Clear();


			}
			_Grid->Copy(*Grid);
		}
		//	return boundaryMaskData;

		if (boundaryMaskData!=NULL)	delete []boundaryMaskData;
		boundaryMaskData=NULL;

		if (intensityProbabilityRawData!=NULL) delete intensityProbabilityRawData;
		intensityProbabilityRawData=NULL;
		return maskData;
	}
	else
		return NULL;
}
//--------------------------------------------


void	TextureSegmentation::SetCurrentNode(int	x,int y,int	z)
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
//--------------------------------------
void TextureSegmentation::GetTransFromCurrentNode(double	*tr)
{

	tr[1]=Grid->GetDX(NodX,NodY,NodZ);
	tr[2]=Grid->GetDY(NodX,NodY,NodZ);
	tr[3]=Grid->GetDZ(NodX,NodY,NodZ);
}
//-------------------------------------------------------------------
void TextureSegmentation::SetTransToCurrentNode(double *tr)
{

	Grid->SetDx(NodX,NodY,NodZ,tr[1]);
	Grid->SetDy(NodX,NodY,NodZ,tr[2]);
	Grid->SetDz(NodX,NodY,NodZ,tr[3]);

}
//------------------------------------------------------
void TextureSegmentation::GetTransFromCurrentNode(double *tr, FloatGrid3D *grid)
{
	tr[1]=grid->GetDX(NodX,NodY,NodZ);
	tr[2]=grid->GetDY(NodX,NodY,NodZ);
	tr[3]=grid->GetDZ(NodX,NodY,NodZ);
}
//-------------------------------------------------------------------
void TextureSegmentation::SetTransToCurrentNode(double *tr, FloatGrid3D *grid)
{
	grid->SetDx(NodX,NodY,NodZ,tr[1]);
	grid->SetDy(NodX,NodY,NodZ,tr[2]);
	grid->SetDz(NodX,NodY,NodZ,tr[3]);
}

//-------------------------------------------------------------------
double	TextureSegmentation::GetFunctionValue(double	*params)
{
	SetTransToCurrentNode(params);
	double e=(this->*GetFValue)(NodX,NodY,NodZ,params);
	return e;
}
//------------------------------------------------
double	TextureSegmentation::GetEnergy(int x,int y,int z,double *tmp)
{
	//trzenba koniecznie wczesniej wywolac SetCurrentNode !!!
	//double	def[4];
	double def[4];
	int	i,j,k,l;
	int	pos;

	//double	gamma=0.0002;
	//double	alpha=0.001;
	//double	beta=0.006;



	double	energy=0.0;
	double	boundEnergy=0.0;
	double	intensityEnergy=0.0;
	double	tttol=10e-7;
	int		countBound=0;

	GetTransFromCurrentNode(def);

	if ((fabs(pDef[1]-def[1])<tttol)&&(fabs(pDef[2]-def[2])<tttol)&&(fabs(pDef[3]-def[3])<tttol))
		if ( (oldNod[0]==NodX) && (oldNod[1]==NodY)	&& (oldNod[2]==NodZ) )
			return pFun;

	//if ((fabs(def[1])>Step[0])||(fabs(def[2])>Step[1])||(fabs(def[3])>Step[2]))
	if	(
		(sqrt(def[1]*def[1]+def[2]*def[2]+def[3]*def[3])>maxDisp)
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
		count=0;
		for(k=posZstart;k<posZstop;k+=optimStep)
			for(j=posYstart;j<posYstop;j+=optimStep)
				for(i=posXstart;i<posXstop;i+=optimStep)
					UpdateEnergy(i,j,k,energy,count,boundEnergy,countBound,intensityEnergy);
		//energy+=GetPointEnergy(i,j,k,count);
		//AddNewPointsToHistTabs(i,j,k,count);

		//count++;	
		//if (count<countsize) {	return INT_MAX;}
		//if (count==0)	 return	INT_MAX;

		if (count!=0)
		{		
			energy/=(double)count;
			intensityEnergy/=(double)count;
		}
		if (countBound!=0)
			boundEnergy/=(double)countBound;



	}

	pDef[1]=def[1];pDef[2]=def[2];pDef[3]=def[3];
	oldNod[0]=NodX;oldNod[1]=NodY;oldNod[2]=NodZ;
	pFun=energy;



	return shape1*energy+shape2*boundEnergy+intense1*intensityEnergy;
}
///-------------------
void	TextureSegmentation::UpdateEnergy(int i,int	j,int k,double &energy,int &count,double &boundEnergy,int	&boundCount,double &intensityEnergy)
{
	int	pos=rawData->GetVoxelPos(i,j,k);
	//if (maskData[pos]==0)
	//{
	double tol=10e-2;
	double Point[3];
	double val1;
	double val2;
	double val3;
	double fval;


	BSplineTransform->TransformPoint(i,j,k,Point);

	if ((Point[0]>=0)&&(Point[0]<nx1-1)&&
		(Point[1]>=0)&&(Point[1]<ny1-1)&&
		(Point[2]>=0)&&(Point[2]<nz1-1))
	{

		count++;
		//val1=Interpolation3Linear(rawData,Point[0],Point[1],Point[2])*factorRaw;
		val2=Interpolation3LinearDouble(distanceMapRawData,Point[0],Point[1],Point[2])*factorDist;
		//*************
		val1=Interpolation3LinearDouble(distanceMapRawData,NodX,NodY,NodZ)*factorDist;
		val1=(maskData[pos]>0)?val1:-val1;	
		//val1=(maskData[pos]>0)?val2:-val2;
		//*************
		energy+= (val1-val2)*(val1-val2);

		val3=(Interpolation3LinearDouble(intensityProbabilityRawData,Point[0],Point[1],Point[2])+1.0)*factorIntensity;

		intensityEnergy+=(val3)*log2(val3);



		//if (distanceMapRawData->GetDataArray()[pos]==0)
		//(val2<tol)//
		if (boundaryMaskData[pos]!=0) //jesli aktualnie	przeksztalcany nalezy do krawedzi
			//if (Check3LinearMaskVoxelNeighborhood(boundaryMaskData,Point)==true)
		{
			// tylko dla brzegow maski
			boundCount++;
			boundEnergy+=val2*val2;
		}

		//if (val3!=0) val1/=val3;


	}
	//}
}
//---------------------------------------------------
int		TextureSegmentation::CheckNumberOfPointsInsideSection(int x,int	y,int z)
{
	int	i,j,k,l;
	int	nr=0;
	int	pos;
	int nr2=0;


	for(k=posZstart;k<posZstop;k+=optimStep)
		for(j=posYstart;j<posYstop;j+=optimStep)
			for(i=posXstart;i<posXstop;i+=optimStep)
			{

				

				nr2++;
				pos=rawData->GetVoxelPos(i,j,k);
				if (maskData[pos]>0) nr++;
			}

			if (nr==nr2) nr=-1;
			return nr;
}
//-------------------------------------------------
void	TextureSegmentation::GetDeformationBounds(double	*bounds)
{

	int	i,j,k;
	int	value;
	double Point[3];
	bounds[0]=0; 
	bounds[1]=0;
	bounds[2]=0;
	bounds[3]=nx1;
	bounds[4]=ny1;
	bounds[5]=nz1;
	/*	bounds[0]=nx1; 
	bounds[1]=ny1;
	bounds[2]=nz1;
	bounds[3]=0;
	bounds[4]=0;
	bounds[5]=0;
	int	pos=0;

	for	(k=0;k<nz1;k++)
	for	(j=0;j<ny1;j++)
	for	(i=0;i<nx1;i++)
	{
	value=maskData[pos++];
	if (value>0)
	{
	BSplineTransform->TransformPoint(i,j,k,Point);

	if (Point[0]<bounds[0])	bounds[0]=Point[0];
	else if	 (Point[0]>bounds[3]) bounds[3]=Point[0];

	if (Point[1]<bounds[1])	bounds[1]=Point[1];
	else if	 (Point[1]>bounds[4]) bounds[4]=Point[1];

	if (Point[2]<bounds[2])	bounds[2]=Point[2];
	else if	 (Point[2]>bounds[5]) bounds[5]=Point[2];
	}
	}
	*/

}
//-------------------------------------------------
bool	TextureSegmentation::CreateNewMask(double *bounds)
{
	if (bounds==NULL)
	{
		bounds=new double[6];
		bounds[0]=0; 
		bounds[1]=0;
		bounds[2]=0;
		bounds[3]=nx1;
		bounds[4]=ny1;
		bounds[5]=nz1;
	}

	FloatGrid3D g(*Grid);
	//BSplineGridTransformation b(&g,grid->GetNx(),grid->GetNy(),grid->GetNz(),0,0,0);
	BSplineTransform->InvertGrid();
	//Grid->ReverseField();

	unsigned char *newMask=new unsigned	char[rawData->GetVolumeSize()];
	memset(newMask,0, rawData->GetVolumeSize());

	int	i,j,k;
	double Point[3];
	int	xmin=Round(bounds[0])-1; if	(xmin<0) xmin=0;
	int	ymin=Round(bounds[1])-1; if	(ymin<0) ymin=0;
	int	zmin=Round(bounds[2])-1; if	(zmin<0) zmin=0;
	int	xmax=Round(bounds[3])+1; if	(xmax>rawData->GetNx())	xmax=rawData->GetNx();
	int	ymax=Round(bounds[4])+1; if	(ymax>rawData->GetNy())	ymax=rawData->GetNy();
	int	zmax=Round(bounds[5])+1; if	(zmax>rawData->GetNz())	zmax=rawData->GetNz();

	int	pos,pos2;
	double val;

	for	(k=zmin;k<zmax;k++)
		for	(j=ymin;j<ymax;j++)
			for	(i=xmin;i<xmax;i++)
			{

				BSplineTransform->TransformPoint(i,j,k,Point);

				pos2=rawData->GetVoxelPos(i,j,k);

				if ((Point[0]>=0)&&(Point[0]<nx1-1)&&
					(Point[1]>=0)&&(Point[1]<ny1-1)&&
					(Point[2]>=0)&&(Point[2]<nz1-1))
				{
					val= Interpolation3LinearUnsignedChar(maskData,
						rawData->GetNx(),rawData->GetNy(),
						rawData->GetNz(),Point[0],Point[1],Point[2]);
					/*if ((val>0)&&(val<1))
					{
					val=val;
					}*/
					if (val>0.49999)newMask[pos2]=1;
				}

			}


			/*for	(k=zmin;k<zmax;k++)
			for	(j=ymin;j<ymax;j++)
			for	(i=xmin;i<xmax;i++)
			{
			pos=rawData->GetVoxelPos(i,j,k);
			if (maskData[pos]!=0)
			{
			BSplineTransform->TransformPoint(i,j,k,Point);


			if ((Point[0]>=0)&&(Point[0]<nx1-1)&&
			(Point[1]>=0)&&(Point[1]<ny1-1)&&
			(Point[2]>=0)&&(Point[2]<nz1-1))
			{

			pos2=rawData->GetVoxelPos(Round(Point[0]),Round(Point[1]),Round(Point[2]));
			newMask[pos2]=1;


			}
			}

			}*/

			Grid->Copy(g);
			//closing
			/// z new mask do mask data
			//BinaryMorphology(newMask,maskData,1);
			//dilate

			//erode
			// z mask data do new mask
			//BinaryMorphology(maskData,newMask,0);

			//--------

			//porownam maski
			bool identical=true;
			for	(i=0;i<rawData->GetVolumeSize();i++)
				if (maskData[i]!=newMask[i])
				{
					identical=false;
					break;
				}
				delete []maskData;
				maskData=newMask;

				return identical;


}
//---------------------------------------------
void TextureSegmentation::BinaryMorphology(unsigned char *source, unsigned char *destination, bool mode)
{
	if ((source!=NULL)&&(destination!=NULL))
	{
		memset(destination,0,rawData->GetVolumeSize());
		//memcpy(boundaryMask,mask,rawData->GetVolumeSize());
		int	i,j,k;

		int	nx,ny,nz;
		int	nxny;
		nx=rawData->GetNx();
		ny=rawData->GetNy();
		nz=rawData->GetNz();
		nxny=rawData->GetSliceSize();

		unsigned char cmode=mode;
		int x,y,z;
		int	p;
		bool pp;
		bool stop;
		for(k=1;k<nz-1;k++)
			for(j=1;j<ny-1;j++)
				for(i=1;i<nx-1;i++)
				{
					p=nxny*k+nx* j + i;	
					pp=source[p];


					stop=false;
					if (pp!=mode)
						for(z=-1;z<2;z++)
						{
							for(y=-1;y<2;y++)
							{
								for(x=-1;x<2;x++)
								{
									if (source[nxny*(k+z)+nx* (j+y) + i+x]==cmode)
										stop=true;
									if (stop) break;
								}
								if (stop) break;
							}
							if (stop) break;
						}

						if (stop) destination[p]=cmode;
						else destination[p]=pp;
				}	

	}
}
//---------------------------------------------
void TextureSegmentation::UpdateBoundaryMaskData(unsigned char *mask, unsigned char	*boundaryMask)
{
	if ((mask!=NULL)&&(boundaryMask!=NULL))
	{
		//memset(boundaryMask,0,rawData->GetVolumeSize());
		memcpy(boundaryMask,mask,rawData->GetVolumeSize());
		int	i,j,k;

		int	nx,ny,nz;
		int	nxny;
		nx=rawData->GetNx();
		ny=rawData->GetNy();
		nz=rawData->GetNz();
		nxny=rawData->GetSliceSize();
		int step;

		int x,y,z;
		int	p,pp;
		bool stop;
		for(k=1;k<nz-1;k++)
			for(j=1;j<ny-1;j++)
				for(i=1;i<nx-1;i++)
				{
					p=nxny*k+nx* j + i;	
					pp=mask[p];
					/*step=0;	
					if (pp==1)
					{
					stop=false;
					for(z=-1;z<2;z++)
					{
					for(y=-1;y<2;y++)
					{
					for(x=-1;x<2;x++)
					{

					if (mask[nxny*(k+z)+nx* (j+y) + i+x]==0)
					{

					//if ((x!=0)&&(y!=0)&&(z!=0))
					//	boundaryMask[p]=0;
					stop=true;
					}
					if (stop) break;
					}
					if (stop) break;
					}
					if (stop) break;
					}

					if (!stop) boundaryMask[p]=0;
					}*/

					if ((pp==1)&&(!(
						(mask[p-1]		!=pp)||
						(mask[p+1]		!=pp)||
						(mask[p+nx]		!=pp)||
						(mask[p-nx]		!=pp)||
						(mask[p+nxny]	!=pp)||
						(mask[p-nxny]	!=pp))))


					{
						boundaryMask[p]=0;
					}
				}
	}
}
//----------------------------------------------
int	TextureSegmentation::GetNumberOfMaskVoxelNeighbors(unsigned	char *data,	double *point)
{
	// jak bedzie potrzeba to sie dopisze
	return 0;
}
//----------------------------------------------
bool TextureSegmentation::Check3LinearMaskVoxelNeighborhood(unsigned char *data, double *point)
{

	//	int	p=rawData->GetVoxelPos((int)(point[0]),(int)(point[1]),(int)(point[2]));
	int	p=rawData->GetVoxelPos(Round(point[0]),Round(point[1]),Round(point[2]));

	if (	(data[p]!=0)
		||	(data[p+1]!=0)
		||	(data[p+rawData->GetNx()]!=0)
		||	(data[p+rawData->GetNx()+1]!=0)
		||	(data[p+rawData->GetSliceSize()]!=0)
		||	(data[p+rawData->GetSliceSize()+1]!=0)
		||	(data[p+rawData->GetSliceSize()+rawData->GetNx()]!=0)
		||	(data[p+rawData->GetSliceSize()+rawData->GetNx()+1]!=0)
		)
		return true;
	else
		return false;

}
//--------------------------------------------------------------
//--------------------------------------------------------------
unsigned char	*TextureSegmentation::DeformationBasedSegmetation(FloatGrid3D *_Grid,int gridS,double sh1,double sh2,	double intens1,double	intens2)
{
	double stddev=1.0;
	int radius=5;
	int lThreshold;
	int rThreshold;

	if ((rawData!=NULL)&&(maskData!=NULL))
	{

		if ((distanceMapRawData!=NULL)&&(rawData->CompareWithOtherDataSet(distanceMapRawData)==false))
		{
			delete distanceMapRawData;
			distanceMapRawData=NULL;
		}
		if (distanceMapRawData==NULL)
		{
			emit SendMessage("3D edges map generation ...");
			RawDataSet *edgesMap=ExtractEdgesMap(radius,stddev,3);

			lThreshold=8000.0;
			rThreshold=65535;
			emit SendMessage("3D distance map generation ...");
			DistanceMap *mapOfDistances=new DistanceMap(edgesMap,lThreshold,rThreshold);
			distanceMapRawData=new	RawDataSet();	
			distanceMapRawData->Copy(*rawData,false);
			//	emit SignalSendRawDataSet(distanceMapRawData);
			factorDist=mapOfDistances->Generate3DUnsignedDistanceMapVolume(distanceMapRawData);
			delete edgesMap;
			delete mapOfDistances;

		}
		//GetFValue=&TextureSegmentation::GetNewEnergy;
		//GetDerivatives=&TextureSegmentation::CalculateSingleNodDerivative;
		GetFValue=&TextureSegmentation::GetTotalFunctionValue;
		GetDerivatives=&TextureSegmentation::CalculateTotalDerivative;
		FILE *Do_opa;Do_opa=fopen("c:\\opter.txt","w");fprintf(Do_opa,"START\n");fclose(Do_opa);	

		/*gridSize=30;
		shape1=0.1;
		shape2=1.0;
		intense1=0.5;*/
		gridSize=gridS;
		shape1=sh1;
		shape2=sh2;
		intense1=intens1;
		iter=Round(intens2);
		//double beta=1.0;
		bool stopOptimization=false;

		int st=1;
		optimStep=1;
		// prepare data
		boundaryMaskData=NULL;
		boundaryMaskData=new unsigned char[rawData->GetVolumeSize()];
		memset(boundaryMaskData,0,rawData->GetVolumeSize());
		UpdateBoundaryMaskData(maskData,boundaryMaskData);
		//emit SignalSendNewMask(boundaryMaskData);
		//return 0;
		//---------------------------------------------
		factorRaw  = (rawData->GetMaxValue()>0)?factorRaw=255.0/(double)rawData->GetMaxValue():	factorRaw=1.0;
		//factorDist = (distanceMapRawData->GetMaxValue()>0)?factorDist=255.0/(double)distanceMapRawData->GetMaxValue():factorDist=1.0;


		//----------
		if (intensityProbabilityRawData!=NULL) delete intensityProbabilityRawData;
		intensityProbabilityRawData=new	RawDataSet();	
		intensityProbabilityRawData->Copy(*rawData,false);
		intensityProbabilityRawData->ZeroQ();
		GetIntensityProbabilityMap(intensityProbabilityRawData);
		factorIntensity=(intensityProbabilityRawData->GetMaxValue()>0)?1.0/(double)intensityProbabilityRawData->GetMaxValue():factorIntensity=1.0;
		//-------------


		//	emit SignalSendRawDataSet(intensityProbabilityRawData);
		//	emit SignalSendRawDataSet(distanceMapRawData);
		//	emit SignalSendRawDataSet(implicitDistanceDataSet);
		//intensityProbabilityRawData->SaveRawDataSet("c:\\intens.dat");
		//distanceMapRawData->SaveRawDataSet("c:\\d1.dat");
		//implicitDistanceDataSet->SaveRawDataSet("c:\\d2.dat");
		//emit SignalSendNewMask(boundaryMaskData);

		nx1=rawData->GetNx();	ny1=rawData->GetNy();	nz1=rawData->GetNz();
		nx=gridSize;			ny=gridSize;			nz=gridSize;

		fx=((nx>0)&&(nx1>0))?(double)(nx1-1)/(nx-1):1;
		fy=((ny>0)&&(ny1>0))?(double)(ny1-1)/(ny-1):1;
		fz=((nz>0)&&(nz1>0))?(double)(nz1-1)/(nz-1):1;

		tdx=1.0/fx;				tdy=1.0/fy;				tdz=1.0/fz;
		nnx=(double)(nx1-1);		nny=(double)(ny1-1);		nnz=(double)(nz1-1);
		mx1=nx1;my1=ny1;mz1=nz1;
		if (Grid!=NULL)				delete Grid;				Grid=new FloatGrid3D(nx,ny,nz);
		if (BSplineTransform!=NULL)	delete BSplineTransform;	BSplineTransform=new BSplineGridTransformation(Grid,nnx,nny,nnz);
		if (transx!=NULL)			delete []transx;			transx=new double[4];

		smallStepX=BSplineTransform->GetGxDivNx();
		smallStepY=BSplineTransform->GetGyDivNy();
		smallStepZ=BSplineTransform->GetGzDivNz();

		double temptrans[4];

		double bounds[6];
		OptimizationDFPowell *Optim=NULL;
		int	numberOfPointsInsideSection;
		int	Iterations=0;
		double FValue=0;
		int	i,j,k,l;
		QString	t;
		t=QString::number((nx-2*st)*(ny-2*st)*(nz-2*st));
		int	numberOfOptimParams=3;
		int	iterx;
		//----------------------------

		double tolerance=10e-2;
		double threshold=0;

		int pos=0;

		{FILE *Do_opa;Do_opa=fopen("c:\\segmentation.txt","w");fprintf(Do_opa,"");fclose(Do_opa);}

		//--------------------------------
		nodes=NULL;
		nrOfNodes=0;
		int NumberOfOptimParams;	

		int thresh=0;
		double	*trans=NULL;
		//-------------------------


		for(l=0;l<iter;l++)
		{

			nrOfNodes=GetNodesToOptimization(Grid,fixBounds,thresh,nodes);
			NumberOfOptimParams=3*nrOfNodes;

			if (trans!=NULL) delete []trans;
			trans=new double[NumberOfOptimParams+1];
			memset(trans,0,(NumberOfOptimParams+1)*sizeof(double));

			GradientDescentOptimizer(trans,NumberOfOptimParams,tolerance,&Iterations,&FValue);

			SetParamsFromArrayToGrid(Grid,trans,fixBounds);

			Grid->SaveGrid("c:\\gridx.grid");
			// tu ma byc update	parametrow do nastepnej	iteracji

			//GetDeformationBounds(bounds);

			if (CreateNewMask(NULL)==false)
			{
				emit SignalSendNewMask(maskData);
				UpdateBoundaryMaskData(maskData,boundaryMaskData);
				//emit SignalSendNewMask(boundaryMaskData);
				GetIntensityProbabilityMap(intensityProbabilityRawData);

				DistanceMap	*mapDist=new DistanceMap(rawData,0,0,maskData);
				factorDistImplicit=mapDist->Generate3DUnsignedDistanceMapVolume(implicitDistanceDataSet);
				delete mapDist;

			}
			else
				break;
			if (l<iter-1)	Grid->Clear();
		}


		if (nodes!=NULL) 	{	delete []nodes;	nodes=NULL;	}
		delete []transx;transx=NULL;
		if (boundaryMaskData!=NULL)	delete []boundaryMaskData;	boundaryMaskData=NULL;

		return maskData;


	}
	else
		return NULL;

}

//--------------------------------------------
void	TextureSegmentation::CalculateDerivative(double *Deriv,double	*Params)
{
	if ((Deriv!=NULL)&&(Params!=NULL))
		(this->*GetDerivatives)(Deriv,Params);

}
//---------------------------------------------
//-------------------ooooooooooo----------------
double 	TextureSegmentation::GetTotalFunctionValue(int a,int b,int c,double *Params)
{
	SetParamsFromArrayToGrid(Grid,Params,fixBounds);
	int i,j,k;
	double energy[3];energy[0]=0;energy[1]=0;energy[2]=0;
	count=0;
	int	countB=0;

	for(k=0;k<nz1;k+=optimStep)
		for(j=0;j<ny1;j+=optimStep)
			for(i=0;i<nx1;i+=optimStep)	
				AddNewEnergyPointWithoutDerivs(i,j,k,count,countB,energy);



	energy[0]=(count>0)?shape1*energy[0]/(double)count:tE[0];
	energy[1]=(countB>0)?shape2*energy[1]/(double)countB:tE[1];
	energy[2]=(count>0)?intense1*energy[2]/(double)countB:tE[2];

	if (energy[0]>tE[0]) tE[0]=energy[0];
	if (energy[1]>tE[1]) tE[1]=energy[1];
	if (energy[2]>tE[2]) tE[2]=energy[2];

	double f= energy[0]+energy[1]+energy[2];

	{FILE *Do_opa;Do_opa=fopen("c:\\segmentation.txt","a");fprintf(Do_opa," %d\t(%lf\t%lf\t%lf)=%lf\n",count,energy[0],energy[1],energy[2],f);fclose(Do_opa);}

	return f;

}
//----------------0000000000000000000============
double	TextureSegmentation::GetNewEnergy(int x,int	y,int z,double *params)
{

	SetTransToCurrentNode(params,Grid);
	double def[4];
	double def0[4];
	int	i,j,k,l;
	int	pos;

	//double	energy=0.0;
	double energy[3];energy[0]=0;energy[1]=0;energy[2]=0;
	double tttol=10e-7;
	//	double der[9];
	//	der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;der[6]=0;der[7]=0;der[8]=0;
	count=0;
	int	countB=0;

	for(k=posZstart;k<posZstop;k++)
		for(j=posYstart;j<posYstop;j++)
			for(i=posXstart;i<posXstop;i++)
				AddNewEnergyPointWithoutDerivs(i,j,k,count,countB,energy);

	energy[0]=(count>0)?shape1*energy[0]/(double)count:tE[0];
	energy[1]=(countB>0)?shape2*energy[1]/(double)countB:tE[1];
	energy[2]=(count>0)?intense1*energy[2]/(double)countB:tE[2];

	if (energy[0]>tE[0]) tE[0]=energy[0];
	if (energy[1]>tE[1]) tE[1]=energy[1];
	if (energy[2]>tE[2]) tE[2]=energy[2];

	double f= energy[0]+energy[1]+energy[2];

	{FILE *Do_opa;Do_opa=fopen("c:\\segmentation.txt","a");fprintf(Do_opa,"(%d %d %d) %d (%lf %lf %lf)\t(%lf %lf %lf)=%lf\n",NodX,NodY,NodZ,count,params[1],params[2],params[3],energy[0],energy[1],energy[2],f);fclose(Do_opa);}

	return f;
}
//--------------------------------------------
void	TextureSegmentation::AddNewEnergyPointWithoutDerivs(int	i,int j,int	k, int &count,int &countB,double *energy)
{
	/*double Point[3];
	BSplineTransform->TransformPoint(i,j,k,Point);
	double val1,val2,val3;
	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))
	{	
	int	pos=rawData->GetVoxelPos(i,j,k);
	val2=Interpolation3LinearDouble(distanceMapRawData,Point[0],Point[1],Point[2])*factorDist;
	//*************
	val1=Interpolation3LinearDouble(implicitDistanceDataSet,NodX,NodY,NodZ)*factorDistImplicit;
	val1=(maskData[pos]>0)?val1:-val1;	
	//val1=(maskData[pos]>0)?val2:-val2;
	//*************
	energy[0]+=	(val1-val2)*(val1-val2);



	if (boundaryMaskData[pos]!=0) //jesli aktualnie	przeksztalcany nalezy do krawedzi
	//if (Check3LinearMaskVoxelNeighborhood(boundaryMaskData,Point)==true)
	{

	val3=(Interpolation3LinearDouble(intensityProbabilityRawData,Point[0],Point[1],Point[2])+1.0)*factorIntensity;
	energy[2]+=(val3)*log(val3);
	countB++;
	energy[1]+=val2*val2;
	}
	count++;
	}*/
	double Point[3];
	int	pos=rawData->GetVoxelPos(i,j,k);
	if (maskData[pos]>0)
	{
		BSplineTransform->TransformPoint(i,j,k,Point);
		double val1,val2,val3;
		if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))
		{
			val2=Interpolation3LinearDouble(distanceMapRawData,Point[0],Point[1],Point[2])*factorDist;
			//*************
			val1=implicitDistanceDataSet->GetVoxelValue(i,j,k)*factorDistImplicit;
			//if (!(Check3LinearMaskVoxelNeighborhood(maskData,Point))) val2*=-1;

			//val1=(maskData[pos]>0)?val1:-val1;	
			//val1=(maskData[pos]>0)?val2:-val2;
			//*************
			energy[0]+=	(val1-val2)*(val1-val2);
			if (boundaryMaskData[pos]!=0) //jesli aktualnie	przeksztalcany nalezy do krawedzi
				//if (Check3LinearMaskVoxelNeighborhood(boundaryMaskData,Point)==true)
			{

				val3=(Interpolation3LinearDouble(intensityProbabilityRawData,Point[0],Point[1],Point[2])+1.0)*factorIntensity;
				energy[2]+=(val3)*log(val3);
				countB++;
				energy[1]+=val2*val2;
			}
			count++;


		}
	}













}
//--------------------------------------------------
//----------------------eeeeeeeeeee-----------------
void	TextureSegmentation::CalculateTotalDerivative(double *Deriv,double *Params)
{

	QTime t;
	t.start();
	SetParamsFromArrayToGrid(Grid,Params,fixBounds);
	//jade po kolejnych nodach wyliczam dla nich pochodne i wrzucam do Deriv[pos++]
	int pos=1;
	double der[4];
	int st=(fixBounds==true)?1:0;

	double cn;
	int i,j,k,ii,jj,kk;
	int l;
	double transx[4];
	mmx=0;
	QString text="/"+QString::number(nrOfNodes)+") ";
	for (l=0;l<nrOfNodes;l++)
	{
		i=nodes[l].x;j=nodes[l].y;k=nodes[l].z;
		SetCurrentNode(i,j,k);
		GetTransFromCurrentNode(transx);

		CalculateSingleNodDerivative(der,transx);

		Deriv[pos++]=der[1];
		Deriv[pos++]=der[2];
		Deriv[pos++]=der[3];

		emit SendMessage("("+QString::number(l+1)+text);
	}

}
//------------------------eeeeeeeeeeeee---------------
//--------------------------------------------------
void	TextureSegmentation::CalculateSingleNodDerivative(double	*Deriv,double *Params)
{
	int	pos=1;
	double der[9];
	int	st=(fixBounds==true)?1:0;
	int	count=0;
	double energy[3];energy[0]=0;energy[1]=0;energy[2]=0;
	double cn;
	int	i,j,k,ii,jj,kk;

	der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;der[6]=0;	der[7]=0;	der[8]=0;
	energy[0]=0;	energy[1]=0; count=0;
	int	countB=0;
	//--------------------------------------
	for(kk=posZstart;kk<posZstop;kk+=optimStep)
		for(jj=posYstart;jj<posYstop;jj+=optimStep)
			for(ii=posXstart;ii<posXstop;ii+=optimStep)
				AddNewEnergyPoint(ii,jj,kk,	count,countB,energy,der);

	if (count>0)	{	der[0]/=(double)count;	der[1]/=(double)count;	der[2]/=(double)count;	}
	if (countB>0)	{	der[3]/=(double)countB;	der[4]/=(double)countB;	der[5]/=(double)countB;	}
	if (count>0)	{	der[6]/=(double)count;	der[7]/=(double)count;	der[8]/=(double)count;	}

	Deriv[1]=2*(shape1*der[0]+shape2*der[3])+intense1*der[6];
	Deriv[2]=2*(shape1*der[1]+shape2*der[4])+intense1*der[7];
	Deriv[3]=2*(shape1*der[2]+shape2*der[5])+intense1*der[8];

	if (fabs(der[6])>mmx) mmx=fabs(der[6]);
	if (fabs(der[7])>mmx) mmx=fabs(der[7]);
	if (fabs(der[8])>mmx) mmx=fabs(der[8]);
}
//------------------------------------------
void TextureSegmentation::AddNewEnergyPoint(int	i,int j,int	k, int &count,int &countB,double *energy,double	*drv)
{

	/*int	pa[3];pa[0]=i;pa[1]=j;pa[2]=k;
	int	nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=NodZ;
	double ddDdQ;
	double Point[3];

	BSplineTransform->TransformPoint(nodPos,pa,Point,&ddDdQ);

	double val1,val2,val3,val3l;
	if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))
	{	

	double deriv,a;
	double gradIx=0;
	double gradIy=0;
	double gradIz=0;
	int	pos=rawData->GetVoxelPos(i,j,k);
	val2=Interpolation3LinearDouble(distanceMapRawData,Point[0],Point[1],Point[2])*factorDist;
	//*************
	val1=Interpolation3LinearDouble(implicitDistanceDataSet,NodX,NodY,NodZ)*factorDistImplicit;
	val1=(maskData[pos]>0)?val1:-val1;	
	//val1=(maskData[pos]>0)?val2:-val2;
	//*************


	a=val1-val2;	
	energy[0]+=a*a;

	deriv=-a*ddDdQ;

	if ((ddDdQ!=0)&&((a!=0)||((val2!=0)&&(boundaryMaskData[pos]!=0))))//||(val3*val3l!=0)))
	{
	gradIx=GetGradI(i,j,k,smallStepX,0,distanceMapRawData); 
	gradIy=GetGradI(i,j,k,smallStepY,1,distanceMapRawData); 
	gradIz=GetGradI(i,j,k,smallStepZ,2,distanceMapRawData); 
	}

	if (deriv!=0)
	{
	val3=(Interpolation3LinearDouble(intensityProbabilityRawData,Point[0],Point[1],Point[2])+1.0)*factorIntensity;
	val3l=log(val3);
	energy[2]+=val3*val3l;

	drv[0]+=deriv*gradIx;
	drv[1]+=deriv*gradIy;
	drv[2]+=deriv*gradIz;

	deriv=(1.0+val3l)*ddDdQ;
	drv[6]+=deriv*gradIx;
	drv[7]+=deriv*gradIy;
	drv[8]+=deriv*gradIz;

	}

	if (boundaryMaskData[pos]!=0) //jesli aktualnie	przeksztalcany nalezy do krawedzi
	//if (Check3LinearMaskVoxelNeighborhood(boundaryMaskData,Point)==true)
	{
	countB++;
	energy[1]+=val2*val2;

	deriv=val2*ddDdQ;
	drv[3]+=deriv*gradIx;
	drv[4]+=deriv*gradIy;
	drv[5]+=deriv*gradIz;
	}
	count++;
	}
	*/
	int	pos=rawData->GetVoxelPos(i,j,k);
	if (maskData[pos]>0)
	{
		int	nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=NodZ;
		double ddDdQ;
		double Point[3];
		int	pa[3];pa[0]=i;pa[1]=j;pa[2]=k;
		BSplineTransform->TransformPoint(nodPos,pa,Point,&ddDdQ);

		double val1,val2,val3,val3l;
		if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))
		{	
			double deriv,a;
			double gradIx=0;
			double gradIy=0;
			double gradIz=0;

			val2=Interpolation3LinearDouble(distanceMapRawData,Point[0],Point[1],Point[2])*factorDist;
			//*************
			val1=implicitDistanceDataSet->GetVoxelValue(i,j,k)*factorDistImplicit;
			//if (!(Check3LinearMaskVoxelNeighborhood(maskData,Point)) val2*=-1;
			//val1=(maskData[pos]>0)?val1:-val1;	
			//val1=(maskData[pos]>0)?val2:-val2;


			a=val1-val2;	
			energy[0]+=a*a;

			deriv=-a*ddDdQ;

			if ((ddDdQ!=0)&&((a!=0)||((val2!=0)&&(boundaryMaskData[pos]!=0))))//||(val3*val3l!=0)))
			{
				gradIx=GetGradI(i,j,k,smallStepX,0,distanceMapRawData); 
				gradIy=GetGradI(i,j,k,smallStepY,1,distanceMapRawData); 
				gradIz=GetGradI(i,j,k,smallStepZ,2,distanceMapRawData); 
			}

			if (deriv!=0)
			{


				drv[0]+=deriv*gradIx;
				drv[1]+=deriv*gradIy;
				drv[2]+=deriv*gradIz;



				val3=(Interpolation3LinearDouble(intensityProbabilityRawData,Point[0],Point[1],Point[2])+1.0)*factorIntensity;
				val3l=log(val3);
				energy[2]+=val3*val3l;

				gradIx=GetGradI(i,j,k,smallStepX,0,intensityProbabilityRawData); 
				gradIy=GetGradI(i,j,k,smallStepY,1,intensityProbabilityRawData); 
				gradIz=GetGradI(i,j,k,smallStepZ,2,intensityProbabilityRawData); 
				deriv=(1.0+val3l)*ddDdQ;
				drv[6]+=deriv*gradIx;
				drv[7]+=deriv*gradIy;
				drv[8]+=deriv*gradIz;

			}

			if (boundaryMaskData[pos]!=0) //jesli aktualnie	przeksztalcany nalezy do krawedzi
				//if (Check3LinearMaskVoxelNeighborhood(boundaryMaskData,Point)==true)
			{
				countB++;
				energy[1]+=val2*val2;

				deriv=val2*ddDdQ;
				drv[3]+=deriv*gradIx;
				drv[4]+=deriv*gradIy;
				drv[5]+=deriv*gradIz;
			}
			count++;

		}

	}



	//*************





}
//----------------------------------------
double  TextureSegmentation::GetGradI(int i,int j,int k,double smallStep,int view,RawDataSet *data)
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
	plus=Interpolation3LinearDouble(data,Point[0],Point[1],Point[2]);//-fvalue;

	BSplineTransform->TransformPont(i,j,k,Point,-x,-y,-z,NodX,NodY,NodZ);
	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&
		(Point[1]>=0)&&(Point[1]<my1-1)&&
		(Point[2]>=0)&&(Point[2]<mz1-1)))
		return 0;
	minus=Interpolation3LinearDouble(data,Point[0],Point[1],Point[2]);//-fvalue;

	value=0.5*(plus-minus);

	return value;
}
//------------------------------------------------------------------------------------
int TextureSegmentation::GetNodesToOptimization(FloatGrid3D *Grid, bool fixbounds,int threshold,Node *&Nodes)
{

	int NrOfNodes=(fixbounds)?
		(Grid->GetNx()-2)*(Grid->GetNy()-2)*(Grid->GetNz()-2):
	Grid->GetNx()*Grid->GetNy()*Grid->GetNz();


	double energy[2];energy[0]=0;energy[1]=0;
	int pos=0;
	int i,j,k;
	int ii,jj,kk;
	double f;
	int Nr=0;
	int st=(fixbounds==true)?1:0;
	int numberOfPointsInsideSection;

	Node *tmpNodes=new Node[NrOfNodes];

	for (k=st;k<nz-st;k++)
		for (j=st;j<ny-st;j++)
			for (i=st;i<nx-st;i++)
			{
				energy[0]=0;	energy[1]=0; count=0;
				SetCurrentNode(i,j,k);

				numberOfPointsInsideSection=CheckNumberOfPointsInsideSection(i,j,k);
				if (numberOfPointsInsideSection>threshold)
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

			return Nr;
}
//------------------------------------
void TextureSegmentation::SetParamsFromGridToArray(FloatGrid3D *Grid,double *Params, bool fixbounds)
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
void TextureSegmentation::SetParamsFromArrayToGrid(FloatGrid3D *Grid,double *Params, bool fixbounds)
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
//-------------------------------------------------
void	TextureSegmentation::GradientDescentOptimizer(double *params0,int nrOfParams,double tol,int *iter,double *fval)
{
	/*	if (params0!=NULL)
	{

	int itmax=100;
	double *der=new double [nrOfParams+1];
	//double *params1=new double [nrOfParams+1];
	double e0,e1,tmpE;
	int i,its;
	double lambda=0.01;
	double sLambda;
	double fLambda=15;
	double tmpLambda;
	double nrmSq;
	double dotPr;
	double tmp1,tmp2;

	//e0=GetFunctionValue(params0);
	CalculateDerivative(der,params0);

	for(i=1;i<=nrOfParams;i++)
	params0[i]=params0[i]-lambda*der[i];

	delete []der;

	}*/

	if (params0!=NULL)
	{

		FILE *Do_opa;
		int it=10;
		double *der=new double [nrOfParams+1];
		double *xi=new double[ nrOfParams+1];
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




		// taka wartosc fkcji jest na starcie
		e0=GetFunctionValue(params0);
		CalculateDerivative(der,params0);

		for(i=1;i<=nrOfParams;i++)
			xi[i] = -der[i];

		int check;
		lnsrch(nrOfParams, params0, e0, der, xi, params1,&e1, 20.0, &check);// double (*func)(double []))
		//	Given an n-dimensional point xold[1..n], the value of the function and gradient there, fold
		//	and g[1..n], and a direction p[1..n], finds a new point x[1..n] along the direction p from
		//	xold where the function func has decreased “sufficiently.” The new function value is returned
		//	in f. stpmax is an input quantity that limits the length of the steps so that you do not try to
		//	evaluate the function in regions where it is undefined or subject to overflow. p is usually the
		//	Newton direction. The output quantity check is false (0) on a normal exit. It is true (1) when
		//	x is too close to xold. In a minimization algorithm, this usually signals convergence and can
		//	be ignored. However, in a zero-finding algorithm the calling program should check whether the
		//	convergence is spurious. Some “difficult” problems may require double precision in this routine.



		for(i=1;i<=nrOfParams;i++)
			params0[i]=params1[i];	
		delete []der;
		delete []xi;
		delete []params1;

		/*	//ustal nowe parametry i wartosc fkcji
		for(i=1;i<=nrOfParams;i++)
		params1[i]=params0[i]-lambda*der[i];

		//for(i=1;i<=nrOfParams;i++)
		//	params0[i]=params1[i];

		e1=GetFunctionValue(params1);

		//-------------------------------------
		Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"start %lf nowa %lf\n",e0,e1);fclose(Do_opa);	

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
		Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"gorzej - po zmniejszeniu %lf,%d\n",e1,it);fclose(Do_opa);	
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
		Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"lepiej - zwieksz %lf %d\n",e1,it);fclose(Do_opa);	

		e0=e1;
		it--;
		lambda*=2.0;

		for(i=1;i<=nrOfParams;i++)
		params1[i]=params0[i]-lambda*der[i];
		e1=GetFunctionValue(params1);

		Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"lepiej - nowy test %lf %d\n",e1,it);fclose(Do_opa);	

		}while ((e1<e0)&&(it>0));

		if (it>0)
		{
		lambda/=2.0;
		for(i=1;i<=nrOfParams;i++)
		params1[i]=params0[i]-lambda*der[i];
		}
		}

		if (it>0)
		{
		Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"ustalam nowe parametry \n");fclose(Do_opa);	

		for(i=1;i<=nrOfParams;i++)
		params0[i]=params1[i];	
		}

		delete []der;
		delete []params1;
		*/

	}






	/*
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
	return ;*/


}

void	TextureSegmentation::lnsrch(int n, double xold[], double fold, double g[], double p[], double x[],
									double *f, double stpmax, int *check)// double (*func)(double []))
									//	Given an n-dimensional point xold[1..n], the value of the function and gradient there, fold
									//	and g[1..n], and a direction p[1..n], finds a new point x[1..n] along the direction p from
									//	xold where the function func has decreased “sufficiently.” The new function value is returned
									//	in f. stpmax is an input quantity that limits the length of the steps so that you do not try to
									//	evaluate the function in regions where it is undefined or subject to overflow. p is usually the
									//	Newton direction. The output quantity check is false (0) on a normal exit. It is true (1) when
									//	x is too close to xold. In a minimization algorithm, this usually signals convergence and can
									//	be ignored. However, in a zero-finding algorithm the calling program should check whether the
									//	convergence is spurious. Some “difficult” problems may require double precision in this routine.
{ 
	//FILE *Do_opa;
	//Do_opa=fopen("c:\\linmin.txt","a");fprintf(Do_opa,"start\n");fclose(Do_opa);	

	int i;
	double a,alam,alam2,alamin,b,disc,f2,rhs1,rhs2,slope,sum,temp,
		test,tmplam;
	*check=0;
	for (sum=0.0,i=1;i<=n;i++) sum += p[i]*p[i];
	sum=sqrt(sum);
	if (sum > stpmax)
		for (i=1;i<=n;i++) p[i] *= stpmax/sum;// Scale if attempted step is too big.
	for (slope=0.0,i=1;i<=n;i++)
		slope += g[i]*p[i];
	if (slope >= 0.0) 
	{
		//free_vector(xi,1,n);
		//free_vector(pnew,1,n); 
		//free_matrix(hessin,1,n,1,n);
		//free_vector(hdg,1,n);
		//free_vector(g,1,n); 
		//free_vector(dg,1,n);	
		//Do_opa=fopen("c:\\linmin.txt","a");fprintf(Do_opa,"lipa\n");fclose(Do_opa);	

		throw 0;
	}
	//nrerror("Roundoff problem in lnsrch.");
	test=0.0; //Compute ?min.
	for (i=1;i<=n;i++) 
	{
		temp=fabs(p[i])/FMAX(fabs(xold[i]),1.0);
		if (temp > test) test=temp;
	}
	alamin=TOLX/test;
	alam=1.0;// Always try full Newton step first.
	for (;;)
	{ 
		//Start of iteration loop.
		for (i=1;i<=n;i++) x[i]=xold[i]+alam*p[i];
		*f=GetFunctionValue(x);
	//	Do_opa=fopen("c:\\linmin.txt","a");fprintf(Do_opa,"%lf\n",*f);fclose(Do_opa);
		//	*f=(*func)(x);
		if (alam < alamin) 
		{ 
			//Convergence on ?x. For zero finding,
			//the calling program should
			//verify the convergence.
			for (i=1;i<=n;i++) x[i]=xold[i];
			*check=1;
			return;
		} else if (*f <= fold+ALF*alam*slope) 
		{
			return;// Sufficient function decrease.
		}
		else 
		{// Backtrack.
			if (alam == 1.0)
				tmplam = -slope/(2.0*(*f-fold-slope));// First time.
			else 
			{ //Subsequent backtracks.
				rhs1 = *f-fold-alam*slope;
				rhs2=f2-fold-alam2*slope;
				a=(rhs1/(alam*alam)-rhs2/(alam2*alam2))/(alam-alam2);
				b=(-alam2*rhs1/(alam*alam)+alam*rhs2/(alam2*alam2))/(alam-alam2);

				if (a == 0.0) tmplam = -slope/(2.0*b);
				else 
				{
					disc=b*b-3.0*a*slope;
					if (disc < 0.0) tmplam=0.5*alam;
					else if (b <= 0.0) tmplam=(-b+sqrt(disc))/(3.0*a);
					else tmplam=-slope/(b+sqrt(disc));
				}
				if (tmplam > 0.5*alam)
					tmplam=0.5*alam;// ? ? 0.5?1.
			}
		}
		alam2=alam;
		f2 = *f;
		alam=FMAX(tmplam,0.1*alam); //? ? 0.1?1.
	} //Try again.
	//Do_opa=fopen("c:\\linmin.txt","a");fprintf(Do_opa,"stop\n");fclose(Do_opa);
}
//---------------------------------
unsigned char	*TextureSegmentation::DeformationBasedSegmetationX(FloatGrid3D *_Grid,int gridS,double sh1,double sh2,	double intens1,double	intens2)
{
	double stddev=1.0;
	int radius=5;
	int lThreshold;
	int rThreshold;


	if ((rawData!=NULL)&&(maskData!=NULL))
	{

		if ((distanceMapRawData!=NULL)&&(rawData->CompareWithOtherDataSet(distanceMapRawData)==false))
		{
			delete distanceMapRawData;
			distanceMapRawData=NULL;
		}
		if (distanceMapRawData==NULL)
		{
			emit SendMessage("3D edges map generation ...");
			RawDataSet *edgesMap=ExtractEdgesMap(radius,stddev,3);

			lThreshold=8000.0;
			rThreshold=65535;
			emit SendMessage("3D distance map generation ...");
			DistanceMap *mapOfDistances=new DistanceMap(edgesMap,lThreshold,rThreshold);
			distanceMapRawData=new	RawDataSet();	
			distanceMapRawData->Copy(*rawData,false);
			//	emit SignalSendRawDataSet(distanceMapRawData);
			factorDist=mapOfDistances->Generate3DUnsignedDistanceMapVolume(distanceMapRawData);
			delete edgesMap;
			delete mapOfDistances;

		}
		GetFValue=&TextureSegmentation::GetNewEnergy;
		GetDerivatives=&TextureSegmentation::CalculateSingleNodDerivative;
		//GetFValue=&TextureSegmentation::GetTotalFunctionValue;
		//GetDerivatives=&TextureSegmentation::CalculateTotalDerivative;
		FILE *Do_opa;Do_opa=fopen("c:\\opter.txt","w");fprintf(Do_opa,"START\n");fclose(Do_opa);	

		/*gridSize=30;
		shape1=0.1;
		shape2=1.0;
		intense1=0.5;*/
		gridSize=gridS;
		shape1=sh1;
		shape2=sh2;
		intense1=intens1;
		iter=Round(intens2);
		//double beta=1.0;
		bool stopOptimization=false;

		int st=1;
		optimStep=1;
		// prepare data
		boundaryMaskData=NULL;
		boundaryMaskData=new unsigned char[rawData->GetVolumeSize()];
		memset(boundaryMaskData,0,rawData->GetVolumeSize());
		UpdateBoundaryMaskData(maskData,boundaryMaskData);
		//emit SignalSendNewMask(boundaryMaskData);
		//return 0;
		//---------------------------------------------
		factorRaw  = (rawData->GetMaxValue()>0)?factorRaw=255.0/(double)rawData->GetMaxValue():	factorRaw=1.0;
		//factorDist = (distanceMapRawData->GetMaxValue()>0)?factorDist=255.0/(double)distanceMapRawData->GetMaxValue():factorDist=1.0;


		//----------
		if (intensityProbabilityRawData!=NULL) delete intensityProbabilityRawData;
		intensityProbabilityRawData=new	RawDataSet();	
		intensityProbabilityRawData->Copy(*rawData,false);
		intensityProbabilityRawData->ZeroQ();
		GetIntensityProbabilityMap(intensityProbabilityRawData);
		factorIntensity=(intensityProbabilityRawData->GetMaxValue()>0)?1.0/(double)intensityProbabilityRawData->GetMaxValue():factorIntensity=1.0;
		//-------------


		//	emit SignalSendRawDataSet(intensityProbabilityRawData);
		//	emit SignalSendRawDataSet(distanceMapRawData);
		//	emit SignalSendRawDataSet(implicitDistanceDataSet);
		//intensityProbabilityRawData->SaveRawDataSet("c:\\intens.dat");
		//distanceMapRawData->SaveRawDataSet("c:\\d1.dat");
		//implicitDistanceDataSet->SaveRawDataSet("c:\\d2.dat");
		//emit SignalSendNewMask(boundaryMaskData);

		nx1=rawData->GetNx();	ny1=rawData->GetNy();	nz1=rawData->GetNz();
		nx=gridSize;			ny=gridSize;			nz=gridSize;

		fx=((nx>0)&&(nx1>0))?(double)(nx1-1)/(nx-1):1;
		fy=((ny>0)&&(ny1>0))?(double)(ny1-1)/(ny-1):1;
		fz=((nz>0)&&(nz1>0))?(double)(nz1-1)/(nz-1):1;

		tdx=1.0/fx;				tdy=1.0/fy;				tdz=1.0/fz;
		nnx=(double)(nx1-1);		nny=(double)(ny1-1);		nnz=(double)(nz1-1);
		mx1=nx1;my1=ny1;mz1=nz1;
		if (Grid!=NULL)				delete Grid;				Grid=new FloatGrid3D(nx,ny,nz);
		if (BSplineTransform!=NULL)	delete BSplineTransform;	BSplineTransform=new BSplineGridTransformation(Grid,nnx,nny,nnz);
		if (transx!=NULL)			delete []transx;			transx=new double[4];

		smallStepX=BSplineTransform->GetGxDivNx();
		smallStepY=BSplineTransform->GetGyDivNy();
		smallStepZ=BSplineTransform->GetGzDivNz();

		double temptrans[4];

		double bounds[6];
		OptimizationDFPowell *Optim=NULL;
		OptimizationPowell *Optim2=NULL;

		int	numberOfPointsInsideSection;
		int	Iterations=0;
		double FValue=0;
		int	i,j,k,l;
		QString	t;
		t=QString::number((nx-2*st)*(ny-2*st)*(nz-2*st));
		int	numberOfOptimParams=3;
		int	iterx;
		//----------------------------

		double tolerance=10e-2;
		double threshold=0;

		int pos=0;

		//{FILE *Do_opa;Do_opa=fopen("c:\\segmentationX.txt","w");fprintf(Do_opa,"");fclose(Do_opa);}

		//--------------------------------
		nodes=NULL;
		nrOfNodes=0;
		int NumberOfOptimParams;	

		int thresh=0;
		double	*trans=NULL;
		//-------------------------
		/*
		for(l=0;l<iter;l++)
		{
		for	(k=st;k<nz-st;k++)
		{
		emit SendMessage("iter:	(" + QString::number(l)+"/"	+QString::number(iter-1)+")" +"	(" +	QString::number(k)+")");//	+	"/"	+	QString::number(j)	+"/"	+QString::number(i)+")"	);


		for	(j=st;j<ny-st;j++)
		for	(i=st;i<nx-st;i++)
		{

		if (stopOptimization==false)	
		{
		SetCurrentNode(i,j,k);
		GetTransFromCurrentNode(transx);
		numberOfPointsInsideSection=CheckNumberOfPointsInsideSection(i,j,k);
		if (numberOfPointsInsideSection>0)
		{

		e=GetFunctionValue(transx);	temptrans[1]=transx[1];	temptrans[2]=transx[2];	temptrans[3]=transx[3];

		Optim=new OptimizationPowell(transx,NumberOfOptimParams,Tolerance,&Iterations,&FValue,this,testtol); //:) 
		Optim->StartOptimization();
		delete Optim;
		e1=FValue;
		// jezeli stara-nowa jest w	gr tolerancji to ustaw stara
		if (fabs(e1-e)<tolX)
		{
		transx[1]=temptrans[1];	transx[2]=temptrans[2];	transx[3]=temptrans[3];e1=e;
		//	Do_opa=fopen("c:\\powellvolelas.txt","a");fprintf(Do_opa,"qq\n");fclose(Do_opa);	
		}
		SetTransToCurrentNode(transx);

		}
		}
		}
		}
		//nowa deformacja

		GetDeformationBounds(bounds);
		if (CreateNewMask(bounds)==false)
		{
		emit SignalSendNewMask(maskData);
		UpdateBoundaryMaskData(maskData,boundaryMaskData);
		GetIntensityProbabilityMap(intensityProbabilityRawData);
		}
		else
		break;
		if (l<iter-1)	Grid->Clear();


		}
		_Grid->Copy(*Grid);
		}*/
		NumberOfOptimParams=3;
		double e,e1;
		double tolX=10e-8;

		int nrrr=0;
		for (i=0;i<rawData->GetVolumeSize();i++)
		{
			if (maskData[i]!=0) nrrr++;
		}


		for(l=0;l<iter;l++)
		{
			for	(k=st;k<nz-st;k++)
			{
				emit SendMessage("iter:	(" + QString::number(l)+"/"	+QString::number(iter-1)+")" +"	(" +	QString::number(k)+")");//	+	"/"	+	QString::number(j)	+"/"	+QString::number(i)+")"	);

				for	(j=st;j<ny-st;j++)
					for	(i=st;i<nx-st;i++)
					{

						if (stopOptimization==false)	
						{
							SetCurrentNode(i,j,k);
			

							GetTransFromCurrentNode(transx);
							numberOfPointsInsideSection=CheckNumberOfPointsInsideSection(i,j,k);
							{FILE *Do_opa;Do_opa=fopen("c:\\segmentationX.txt","a");fprintf(Do_opa,"[%d %d %d] ",i,j,k);fclose(Do_opa);}
							if (numberOfPointsInsideSection>0)
							{

								e=(this->*GetFValue)(i,j,k,transx);	temptrans[1]=transx[1];	temptrans[2]=transx[2];	temptrans[3]=transx[3];

								//Optim2=new OptimizationPowell(transx,NumberOfOptimParams,tolerance,&Iterations,&FValue,this,tolX); //:) 
								//Optim2->StartOptimization();
								//delete Optim2;
								GradientDescentOptimizer(transx,NumberOfOptimParams,tolerance,&Iterations,&FValue);
								e1=FValue;
								// jezeli stara-nowa jest w	gr tolerancji to ustaw stara
								if (fabs(e1-e)<tolX)
								{
									transx[1]=temptrans[1];	transx[2]=temptrans[2];	transx[3]=temptrans[3];e1=e;
									//	Do_opa=fopen("c:\\powellvolelas.txt","a");fprintf(Do_opa,"qq\n");fclose(Do_opa);	
								}
//
//								
//{FILE *Do_opa;Do_opa=fopen("c:\\segmentationX.txt","a");fprintf(Do_opa,"-done \n");fclose(Do_opa);}
//								if ((NodX==3)&&(NodY==325)&&(NodZ==2))
//								{
//									i=i;
//								}
								SetTransToCurrentNode(transx);

							}
						}
					}

					//nowa deformacja

					//nrOfNodes=GetNodesToOptimization(Grid,fixBounds,thresh,nodes);
					//NumberOfOptimParams=3*nrOfNodes;

					//if (trans!=NULL) delete []trans;
					//trans=new double[NumberOfOptimParams+1];
					//memset(trans,0,(NumberOfOptimParams+1)*sizeof(double));

					//GradientDescentOptimizer(trans,NumberOfOptimParams,tolerance,&Iterations,&FValue);

					//SetParamsFromArrayToGrid(Grid,trans,fixBounds);

					//Grid->SaveGrid("c:\\gridx.grid");
					// tu ma byc update	parametrow do nastepnej	iteracji

					//GetDeformationBounds(bounds);
			}
					if (CreateNewMask(NULL)==false)
					{
						emit SignalSendNewMask(maskData);
						UpdateBoundaryMaskData(maskData,boundaryMaskData);
						//emit SignalSendNewMask(boundaryMaskData);
						GetIntensityProbabilityMap(intensityProbabilityRawData);

						DistanceMap	*mapDist=new DistanceMap(rawData,0,0,maskData);
						factorDistImplicit=mapDist->Generate3DUnsignedDistanceMapVolume(implicitDistanceDataSet);
						delete mapDist;

					}
					else
						break;
					if (l<iter-1)	Grid->Clear();
			


			if (nodes!=NULL) 	{	delete []nodes;	nodes=NULL;	}
			delete []transx;transx=NULL;
			if (boundaryMaskData!=NULL)	delete []boundaryMaskData;	boundaryMaskData=NULL;
		}
		delete Grid;
		Grid=NULL;
		
		return maskData;
		

		}
		else
			return NULL;

	}
