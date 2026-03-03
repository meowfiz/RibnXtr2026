#include "Registration.h"
//Added by qt3to4:
#include <qlabel.h>

void Registration::SlotRegOpacity(int opacity)
{

	RegOpacity = opacity;
}

Registration::Registration(ListItem<DataSet*>	*_ModelListItem,	ListItem<DataSet*>	*_ObjectListItem,ImageWidget *_IWidget,QLabel *_InfoLabel,double *_Trans,
	View *TransversalV,View *SagittalV,View *CoronalV,QWidget *parent,const char *name,Qt::WindowFlags f)
{
	parallelComputing=1;

#pragma omp parallel

	this->numberOfThreads=omp_get_num_threads();
	numberOfThreadsUsed=numberOfThreads;


	ModelListItem = _ModelListItem;
	ObjectListItem = _ObjectListItem;

	//ObjectTabs=new int *[numberOfThreads];
	//ModelTabs=new int *[numberOfThreads];
	ScatterTabs=new int *[numberOfThreads];


	oneScale=false;
	constraints=false;
	PaintTransformedSlicesEnabled=true;

	scatterMaskData=NULL;
	Interpolate=NULL;

	TransSize=20;

	RegOpacity = 128;
	
	//ObjectListItem->GetData()->GetOpacity();
	ModelData=ModelListItem->GetData()->GetRawData();	
	ObjectData=ObjectListItem->GetData()->GetRawData();	
	PaletteReg=ObjectListItem->GetData()->GetPalette();
	Trans=_Trans;
	int i;
	for(i=0;i<12;i++)
		trans2[i]=Trans[i];

	trans2[3]*= to_rad;
	trans2[4]*=  to_rad;
	trans2[5]*= to_rad;


	DataPrepared=false;
	//GrabApplicationWindow=false;

	FinalTrans=new double[TransSize];
	ScatterSize=128;
	CountMutual=0;
	MisregistrationFunctionMode=0;

	InfoLabel=_InfoLabel;
	Iterations=0;

	IWidget=_IWidget;

	Image=NULL;

	TransversalView=TransversalV;
	SagittalView=SagittalV;
	CoronalView=CoronalV;

	InspX=0;
	InspY=0;
	InspZ=0;
	PosX=0;
	PosY=0;
	PosZ=0;

	//PaintTransformedSlicesEnabled=false;

	ParamTransTab=NULL;
	NumberOfOptimParams=0;
	TransXX=NULL;
	OptimizationSuccsessfull=false;
	/*
	log2tab=new double[1000000];
	log2tab[0]=0;//tu nie mozna ale jak ktos zajrzy to sie nie zalamie przynajmniej
	double pos;
	int i;
	for(i=1;i<1000000;i++)
	{
	pos=(double)i/1000000.0;;
	log2tab[i]=-pos*log2(pos);
	}
	*/
}

//-------------------------------------------------

void Registration::TransformPoints(double *TransT,double *pts,int nrOfPts)
{
	Transform *transX=new Transform(RotateCenter,1,1,1,oneScale);
	transX->MakeCompleteAffineMatrix(Matrix,TransT);
	double Point[3];
	double Point1[3];
	double PointF[3];

	int i;
	for(i=0;i<nrOfPts;i++)
	{
		Point[0]=pts[3*i];
		Point[1]=pts[3*i+1];
		Point[2]=pts[3*i+2];

		transX->MultiplyPoint(Point1,Point);

		pts[3*i]=Point1[0];
		pts[3*i+1]=Point1[1];
		pts[3*i+2]=Point1[2];

	}
	delete transX;
}
//--------------------------------------------

void Registration::PaintTransformedSlices(double *TransT)
{

	int i,j;



	unsigned char Opacity=(RegOpacity);


	if(Opacity!=0)
	{
		


		//---------------------------------- 2003.09.19
		
	
	
		//--------------------------------------
		//double tolX=1e-5;	

		double t1=(double)(ObjectData->GetNx()-1);
		double t2=(double)(ObjectData->GetNy()-1);
		double t3=(double)(ObjectData->GetNz()-1);


		unsigned short *TempPointer;

		Transform *transX=new Transform(RotateCenter,1,1,1,oneScale);
		/*		transX->CalculateTrigonometricFunctionsInv(TransT);
		transX->MakeMatrixInv(Matrix,TransT);
		*/
		//shear poprawka 2003.10.04
		/*	transX->CalculateTrigonometricFunctionsInv(TransT);
		transX->CalculateShearParametersInv(TransT);
		transX->MakeAffineMatrixInv(Matrix,TransT);
		*/
		transX->MakeCompleteAffineMatrixInv(Matrix,TransT);

		//Normal
		/*double *Point=new double[3];
		double *Point1=new double[3];
		double *PointF=new double[3];*/

		double Point[3];
		double Point1[3];
		double PointF[3];

		//shear poprawka 2003.10.04

		double Ox=1.0/(ObjectData->GetXscale());
		double Oy=1.0/(ObjectData->GetYscale());
		double Oz=1.0/(ObjectData->GetZscale());





		//--------------------------------------
		//--------------------------------------
		//--------------------------------------
		//--------------------------------------
		//--------------------------------------
		if ((TransversalView != NULL) && (TransversalDataSet != NULL))
		{
			TransversalDataSet->ZeroQ();
			TransversalDataSet->SetFirstHistValue(ObjectData->GetFirstHistValue());
			TransversalDataSet->SetLastHistValue(ObjectData->GetLastHistValue());
			
			if (ModelData->GetNz() == 1)
			{
				TempPointer = TransversalDataSet->GetDataArray();
				Point[0] = 0.0;
				Point[1] = 0.0;
				Point[2] = (PosZ)*ModelData->GetZscale();
				for (j = 0; j < TransversalDataSet->GetNy(); j++)
				{
					for (i = 0; i < TransversalDataSet->GetNx(); i++)
					{

						transX->MultiplyPoint(Point1, Point);
						PointF[0] = (Point1[0])*Ox;
						PointF[1] = (Point1[1])*Oy;
						if ((PointF[0] >= 0) && (PointF[0] < t1) &&
							(PointF[1] >= 0) && (PointF[1] < t2))
							//*(TempPointer)=Interpolation3Linear(ObjectData,PointF[0],PointF[1],PointF[2]);
							*(TempPointer) = ObjectData->GetDataArray()[(int)(PointF[0] + 0.5) + (int)(PointF[1] + 0.5)*ObjectData->GetNx()];
						TempPointer++;
						Point[0] += ModelData->GetXscale();

					}
					Point[0] = 0.0;
					Point[1] += ModelData->GetYscale();
				}

				TransversalDataSet->SetMaxValue(ObjectData->GetMaxValue());
				TransversalView->UpdatRegSlice(TransversalDataSet, PaletteReg, Opacity);

			}
			else
			{


				TempPointer = TransversalDataSet->GetDataArray();
				Point[0] = 0.0;
				Point[1] = 0.0;
				Point[2] = (PosZ)*ModelData->GetZscale();
				for (j = 0; j < TransversalDataSet->GetNy(); j++)
				{
					for (i = 0; i < TransversalDataSet->GetNx(); i++)
					{

						transX->MultiplyPoint(Point1, Point);
						PointF[0] = (Point1[0])*Ox;
						PointF[1] = (Point1[1])*Oy;
						PointF[2] = (Point1[2])*Oz;
						if ((PointF[0] >= 0) && (PointF[0] < t1) &&
							(PointF[1] >= 0) && (PointF[1] < t2) &&
							(PointF[2] >= 0) && (PointF[2] < t3))
							//*(TempPointer)=Interpolation3Linear(ObjectData,PointF[0],PointF[1],PointF[2]);
							*(TempPointer) = ObjectData->GetVoxelValue((int)(PointF[0]), (int)(PointF[1]), (int)(PointF[2]));
						TempPointer++;
						Point[0] += ModelData->GetXscale();

					}
					Point[0] = 0.0;
					Point[1] += ModelData->GetYscale();
				}
			}
			TransversalDataSet->SetMaxValue(ObjectData->GetMaxValue());
			TransversalView->UpdatRegSlice(TransversalDataSet, PaletteReg, Opacity);
		}
		if ((CoronalView != NULL) && (CoronalDataSet != NULL))
		{
		
			CoronalDataSet->ZeroQ();
			CoronalDataSet->SetFirstHistValue(ObjectData->GetFirstHistValue());
			CoronalDataSet->SetLastHistValue(ObjectData->GetLastHistValue());
			//--------------------------------------coronal		
			//--------------------------------------
			TempPointer = CoronalDataSet->GetDataArray();/// 2003.10.05 - odwrocilem X - bylo zle 
			//	Point[0]=CoronalDataSet->GetNx()*ModelData->GetXscale();
			Point[0] = 0.0;
			Point[1] = (PosY)*ModelData->GetYscale();
			Point[2] = CoronalDataSet->GetNy()*ModelData->GetZscale();
			for (j = 0; j < CoronalDataSet->GetNy(); j++)
			{
				for (i = 0; i < CoronalDataSet->GetNx(); i++)
				{

					transX->MultiplyPoint(Point1, Point);
					PointF[0] = (Point1[0])*Ox;
					PointF[1] = (Point1[1])*Oy;
					PointF[2] = (Point1[2])*Oz;
					if ((PointF[0] >= 0) && (PointF[0] < t1) &&
						(PointF[1] >= 0) && (PointF[1] < t2) &&
						(PointF[2] >= 0) && (PointF[2] < t3))
						//	*(TempPointer)=Interpolation3Linear(ObjectData,PointF[0],PointF[1],PointF[2]);
						*(TempPointer) = ObjectData->GetVoxelValue((int)(PointF[0]), (int)(PointF[1]), (int)(PointF[2]));

					TempPointer++;
					Point[0] += ModelData->GetXscale();

				}
				Point[0] = 0.0;
				Point[2] -= ModelData->GetZscale();
			}
			CoronalDataSet->SetMaxValue(ObjectData->GetMaxValue());



			CoronalView->UpdatRegSlice(CoronalDataSet, PaletteReg, Opacity);
		}
		if ((SagittalView != NULL) && (SagittalDataSet != NULL))
		{
			SagittalDataSet->ZeroQ();
			SagittalDataSet->SetFirstHistValue(ObjectData->GetFirstHistValue());
			SagittalDataSet->SetLastHistValue(ObjectData->GetLastHistValue());
			//--------------------------------------
			//--------------------------------------sagittal
			TempPointer = SagittalDataSet->GetDataArray();
			Point[0] = (PosX)*ModelData->GetXscale();
			Point[1] = SagittalDataSet->GetNx()*ModelData->GetYscale();
			Point[2] = SagittalDataSet->GetNy()*ModelData->GetZscale();
			for (j = 0; j < SagittalDataSet->GetNy(); j++)
			{
				TempPointer = SagittalDataSet->GetDataArray() + SagittalDataSet->GetNx()*(j + 1) - 1;
				for (i = 0; i < SagittalDataSet->GetNx(); i++)
				{

					transX->MultiplyPoint(Point1, Point);
					PointF[0] = (Point1[0])*Ox;
					PointF[1] = (Point1[1])*Oy;
					PointF[2] = (Point1[2])*Oz;
					if ((PointF[0] >= 0) && (PointF[0] < t1) &&
						(PointF[1] >= 0) && (PointF[1] < t2) &&
						(PointF[2] >= 0) && (PointF[2] < t3))
						//	*(TempPointer)=Interpolation3Linear(ObjectData,PointF[0],PointF[1],PointF[2]);
						*(TempPointer) = ObjectData->GetVoxelValue((int)(PointF[0]), (int)(PointF[1]), (int)(PointF[2]));

					//TempPointer++;		
					TempPointer--;
					Point[1] -= ModelData->GetYscale();

				}
				Point[1] = SagittalDataSet->GetNx()*ModelData->GetYscale();
				Point[2] -= ModelData->GetZscale();
			}
			SagittalDataSet->SetMaxValue(ObjectData->GetMaxValue());
			SagittalView->UpdatRegSlice(SagittalDataSet, PaletteReg, Opacity);
		}

			//--------------------------------------	
			
		
			
		
		TempPointer=NULL;
		//delete	TempPointer;
		delete   transX;
		//delete	[]Point;
		//delete	[]Point1;
		//delete	[]PointF;

	}

}

//-------------------------------------------------


Registration::~Registration()
{
	/*if (log2tab!=NULL)
	delete []log2tab;
	log2tab=NULL;*/
	//if (scatterMaskData!=NULL) delete []scatterMaskData;

	/*if (numberOfThreads!=0)
	{
	for (int i=0;i<numberOfThreads;i++)
	{
	delete []ObjectTabs;
	delete []ModelTabs;
	delete []ScatterTabs;
	}
	}*/


}

//-------------------------------------------------




//---------------------------------------------------------------------

RawDataSet *Registration::GenerateScatterPlot()
{


	//	FILE *Do_opa;

	if((ModelData!=NULL)&&(ObjectData!=NULL))
	{

		//		int count=0;
		Trans[24]=1.0;//scatter plot mode on
		

		PrepareVariables(1);

		double E=ComputeMutualInformation(TransTemp);

		UnprepareVariables(1);




	}
	return NULL;
}

//----------------------------------------------------------
RawDataSet	*Registration::TransformObjectToModelVolume(bool inv,  unsigned char*& outputMask,int interpolMode)
{

	//	FILE *Do_opa;

	if((ModelData!=NULL)&&(ObjectData!=NULL))
	{
		int i,j,k;

		PrepareVariables(0);

		Transform *transX=new Transform(RotateCenter,1,1,1,oneScale);
		//		transX->CalculateTrigonometricFunctionsInv(TransTemp);
		//		transX->MakeMatrixInv(Matrix,TransTemp);

		//bo affine 2003.10.03
		/*	transX->CalculateTrigonometricFunctionsInv(TransTemp);
		transX->CalculateShearParametersInv(TransTemp);
		transX->MakeAffineMatrixInv(Matrix,TransTemp);
		*/		if (inv==true)
			transX->MakeCompleteAffineMatrixInv(Matrix,TransTemp);
		else
			transX->MakeCompleteAffineMatrix(Matrix,TransTemp);


		RawDataSet	*FinalData = new RawDataSet(ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz(),ModelData->GetXscale(),ModelData->GetYscale(),ModelData->GetZscale(),0);
		FinalData->ZeroQ();

		FinalData->SetRawDataSetName(ObjectData->GetRawDataSetName()+"-TO-"+ModelData->GetRawDataSetName());

		unsigned short *TempPointer=FinalData->GetDataArray();


		double *Point1;




		transX->SetTransformationSteps(	FinalData->GetXscale(),FinalData->GetYscale(),FinalData->GetZscale(),
			ObjectData->GetXscale(),ObjectData->GetYscale(),ObjectData->GetZscale(),
			ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz());
		Point1=transX->FinalPoint;

		//		Point1[0]+=1.0;
		//		Point1[1]+=1.0;
		//		Point1[2]+=1.0;
		//double tolX=1e-5;	

		double t1=(double)(ObjectData->GetNx()-1);
		double t2=(double)(ObjectData->GetNy()-1);
		double t3=(double)(ObjectData->GetNz()-1);


		////////////////////////////////////
		XStep_x=transX->SX1;	XStep_y=transX->SY1;	XStep_z=transX->SZ1;
		YStep_x=transX->SX2;	YStep_y=transX->SY2;	YStep_z=transX->SZ2;
		ZStep_x=transX->SX3;	ZStep_y=transX->SY3;	ZStep_z=transX->SZ3;
		XBack_x=transX->SXB1;	XBack_y=transX->SYB1;	XBack_z=transX->SZB1;
		YBack_x=transX->SXB2;	YBack_y=transX->SYB2;	YBack_z=transX->SZB2;
		ZBack_x=transX->SXB3;	ZBack_y=transX->SYB3;	ZBack_z=transX->SZB3;
		ZeroYNextZ_x=transX->SYZB1;		ZeroYNextZ_y=transX->SYZB2;		ZeroYNextZ_z=transX->SYZB3;
		ZeroXNextY_x=transX->SXYB1;		ZeroXNextY_y=transX->SXYB2;		ZeroXNextY_z=transX->SXYB3;
		//////////////////////////////////////

		unsigned char* objectMask = NULL;

		
		objectMask = new unsigned char[ObjectData->GetVolumeSize()];
		unsigned char* outMask = NULL;
		ObjectListItem->GetData()->GetMasks()->GetMask(objectMask);
		
		int volInt = 0;
		bool getMask = false;
		if (objectMask != NULL)
		{
			volInt = 0;
			for (int i = 0; i < ObjectData->GetVolumeSize(); i++)
				if (objectMask[i] == 1) volInt++;
		}
		if ((volInt > 0) && (volInt < ObjectData->GetVolumeSize()))
		{
			getMask = true;
		}

		if (t3<=0) t3=1; //takie tam

		if (Interpolate!=NULL) delete Interpolate;
		if (interpolMode == 0)

		{

			if (ModelData->GetNz() == 1)
			{
				Interpolate = new InterpolationFunction2DBilinear();
			}
			else
			{
				Interpolate = new InterpolationFunctionTrilinear();
			}
		}
		else
		{
			if (ModelData->GetNz() == 1)
			{
				Interpolate = new InterpolationFunction2DNearest();
			}
			else
			{
				Interpolate = new InterpolationFunctionNearest();
			}
		}

		if (getMask == true)
		{
			outMask = new unsigned char[FinalData->GetVolumeSize()];
			memset(outMask, 0, FinalData->GetVolumeSize());
			outputMask = outMask;
		}
		int posM = 0;
		for (k=0;k<FinalData->GetNz();k++)
		{		
			for (j=0;j<FinalData->GetNy();j++)
			{
				for (i=0;i<FinalData->GetNx();i++)
				{	
					if ((Point1[0] >= 0.0) && (Point1[0] < t1) &&
						(Point1[1] >= 0.0) && (Point1[1] < t2) &&
						(Point1[2] >= 0.0) && (Point1[2] < t3))
					{
						*(TempPointer) = (int)(Interpolate->GetInterpolatedValue(DataPadded, Point1[0], Point1[1], Point1[2]));
						//Interpolation3Linear(DataPadded,Point1[0],Point1[1],Point1[2]);
						if (getMask)
						{
							int ss = ObjectData->GetVoxelPos(Point1[0], Point1[1], Point1[2]);
							outMask[posM] = objectMask[ss];

							//powionna byc maska
						}
					}
					posM++;
					TempPointer++;	
					//transX->SetNextXCoordinate();
					Point1[0]+=XStep_x;
					Point1[1]+=XStep_y;
					Point1[2]+=XStep_z;
				}
				//transX->SetZeroXNextYCoordinate();
				Point1[0]+=ZeroXNextY_x;
				Point1[1]+=ZeroXNextY_y;
				Point1[2]+=ZeroXNextY_z;
			}
			//transX->SetZeroYNextZCoordinate();
			Point1[0]+=ZeroYNextZ_x;
			Point1[1]+=ZeroYNextZ_y;
			Point1[2]+=ZeroYNextZ_z;

		}

		/*	Transform *transX=new Transform(RotateCenter,ModelData->GetXscale(),ModelData->GetYscale(),ModelData->GetZscale(),oneScale);
		transX->CalculateTrigonometricFunctions(TransTemp);

		transX->MakeMatrix(Matrix,TransTemp);


		RawDataSet	*FinalData = new RawDataSet(ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz(),ModelData->GetXscale(),ModelData->GetYscale(),ModelData->GetZscale(),0);
		FinalData->ZeroQ();
		unsigned short *TempPointer=ObjectDataGetDataArray();


		double *Point1;
		transX->SetTransformationSteps(	ObjectData->GetXscale(),ObjectData->GetYscale(),ObjectData->GetZscale(),
		ModelData->GetXscale(),ModelData->GetYscale(),ModelData->GetZscale(),
		ObjectData->GetNx(),ObjectData->GetNy(),ObjectData->GetNz());
		Point1=transX->FinalPoint;



		for (k=0;k<ObjectData->GetNz();k++)
		{		
		for (j=0;j<ObjectData->GetNy();j++)
		{
		for (i=0;i<ObjectData->GetNx();i++)
		{
		if ((Point1[0]>=0)&&(Point1[0]<ModelData->GetNx()-1)&&
		(Point1[1]>=0)&&(Point1[1]<ModelData->GetNy()-1)&&
		(Point1[2]>=0)&&(Point1[2]<ModelData->GetNz()-1))

		FinalData->SetVoxelValue(Round(Point1[0]),Round(Point1[1]),Round(Point1[2]),*(TempPointer));
		TempPointer++;	
		//transX->SetNextXCoordinate();
		Point1[0]+=XStep_x;
		Point1[1]+=XStep_y;
		Point1[2]+=XStep_z;
		}
		//transX->SetZeroXNextYCoordinate();
		Point1[0]+=ZeroXNextY_x;
		Point1[1]+=ZeroXNextY_y;
		Point1[2]+=ZeroXNextY_z;
		}
		//transX->SetZeroYNextZCoordinate();
		Point1[0]+=ZeroYNextZ_x;
		Point1[1]+=ZeroYNextZ_y;
		Point1[2]+=ZeroYNextZ_z;

		}
		*/
		UnprepareVariables(0);
		delete[]objectMask;//
		delete transX;
		//		delete Point1;
		return	FinalData;
	}
	return NULL;

}

//------------------------------------------------------------

void Registration::UpdateHistograms(int Z, int*scatTab, unsigned short *Q1,unsigned short *paddQ,int paddNx,int paddSS,double *Point,int t[3],int T[3],int &countx,bool inv)
{

	int i,j,k;

	//unsigned short v1,v2;
	unsigned short ModelValue, ObjectValue;


	unsigned short MValue;
	unsigned short OValue;

	unsigned short *Value1,*Value2;
	if (inv==false)
	{
		Value2=&MValue;
		Value1=&OValue;
	}
	else
	{
		Value2=&OValue;
		Value1=&MValue;
	}


	k=Z;
	int posQ;
	int ZZ=Z/VolumeStepZ;
	int YY=t[1]/VolumeStepY;
	int XX=t[0]/VolumeStepX;

	double Point1[3]={Point[0],Point[1],Point[2]};



	Point1[0]	+=ZZ	*	(ZeroYNextZ_x	+	YY	*	(ZeroXNextY_x	+	XX	*	XStep_x));
	Point1[1]	+=ZZ	*	(ZeroYNextZ_y	+	YY	*	(ZeroXNextY_y	+	XX	*	XStep_y));
	Point1[2]	+=ZZ	*	(ZeroYNextZ_z	+	YY	*	(ZeroXNextY_z	+	XX	*	XStep_z));
	posQ=ZZ	*	(Add2			+	YY	*	(Add1			+	XX	*	VolumeStepX));


	for (j=0;j<t[1];j+=VolumeStepY,posQ+=Add1,Point1[0]+=ZeroXNextY_x,Point1[1]+=ZeroXNextY_y,Point1[2]+=ZeroXNextY_z)
	{
		for (i=0;i<t[0];i+=VolumeStepX,	posQ+=VolumeStepX,Point1[0]+=XStep_x,Point1[1]+=XStep_y,Point1[2]+=XStep_z)
		{
			if ((Point1[0]>=0.0)&&(Point1[0]<T[0])&&
				(Point1[1]>=0.0)&&(Point1[1]<T[1])&&
				(Point1[2]>=0.0)&&(Point1[2]<T[2]))
			{

				*(Value1)=(int)(Interpolate->GetInterpolatedValue(paddQ,paddNx,paddSS,Point1[0],Point1[1],Point1[2]));
				*(Value2)=Q1[posQ];
				ModelValue=tab1[MValue];
				ObjectValue=tab2[OValue];

				scatTab[ModelValue*ScatterSize+ObjectValue]++;
				/*	{int sum=0; for (int i = 0; i < 100; i++) sum += sqrt((double)(Z+i));   if (sum == 1) return;}*/
				/*	if (newMask!=NULL)
				{
				int pos=nSliceSize*k+nNx*j+i;
				if (scatterMaskData[ModelValue*ScatterSize+ObjectValue]!=0)
				{
				newMask[pos]=1;
				}
				}*/

				countx++;
			}


		}
	}


}

double	Registration::ComputeMutualInformation(double *TransT)
{


	if(DataPrepared==true)
	{

		emit SignalGet2DSlicesPos();
		//FILE *Do_opa;
		CountMutual++;

		unsigned short *Q1;
		Transform *transX;
		double *Point1;
		unsigned short *Value1,*Value2;
		int ObjectValue,ModelValue;

		int i,j,k;
		int count=0;

		unsigned short MValue;
		unsigned short OValue;

		memset(ObjectTab, 0, ScatterSize * sizeof(int));
		memset(ModelTab, 0, ScatterSize * sizeof(int));
		memset(ScatterTab, 0, ScatterSize*ScatterSize * sizeof(int));



		if (parallelComputing==true)
		{
			for (i=0;i<numberOfThreads;i++)
			{
				//	memset(ObjectTabs[i], 0, ScatterSize * sizeof(int));
				//	memset(ModelTabs[i], 0, ScatterSize * sizeof(int));
				memset(ScatterTabs[i], 0, ScatterSize*ScatterSize * sizeof(int));
				//counts[i]=0;
			}
		}

		//memset (counts, 0, numberOfThreads*sizeof(int));
		int tX,tY,tZ,tS;



		int RestX,RestY;


		double t1,t2,t3;

		double FloatEnergy=0;	

		//InterpolationMode=(int)Trans[17];
		//TransformationMode=(int)Trans[16];
		//ScatterPlotVisualizationEnabled=(int)Trans[18];

		/*unsigned char *newMask=NULL;
		int nNx,nSliceSize;
		int newMaskSize=0;*/
		newMask=NULL;

		newMaskSize=0;

		if (scatterMaskData!=NULL)
		{

			int nr=0;

			for(i=0;i<ScatterSize*ScatterSize;i++)
			{
				if (scatterMaskData[i]!=0) nr++;
			}


			if (TransformationMode==1)
			{
				newMaskSize=ModelData->GetVolumeSize();
				nNx=ModelData->GetNx();
				nSliceSize=ModelData->GetSliceSize();
			}
			else
			{
				newMaskSize=ObjectData->GetVolumeSize();
				nNx=ObjectData->GetNx();
				nSliceSize=ObjectData->GetSliceSize();
			}

			newMask=new unsigned char[newMaskSize];
			memset(newMask, 0,newMaskSize);
		}
		//-----------------------
		//transformacja odwrotna
		//-----------------------
		bool inv=false;
		if(TransformationMode==1)	

		{

			Q1=ModelData->GetDataArray();	
			transX=new Transform(RotateCenter,ObjectData->GetXscale(),ObjectData->GetYscale(),ObjectData->GetZscale(),oneScale);
			transX->MakeCompleteAffineMatrixInv(Matrix,TransT);
			transX->SetTransformationSteps(	ModelData->GetXscale(),ModelData->GetYscale(),ModelData->GetZscale(),
				ObjectData->GetXscale(),ObjectData->GetYscale(),ObjectData->GetZscale(),
				ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz(),
				VolumeStepX,VolumeStepY,VolumeStepZ);					

			//t1=(double)(ObjectData->GetNx())-1.0;//p
			//t2=(double)(ObjectData->GetNy())-1.0;//p
			//t3=(double)(ObjectData->GetNz())-1.0;//p

			//double tolX=1e-5;	

			t1=(double)(ObjectData->GetNx()-1);
			t2=(double)(ObjectData->GetNy()-1);
			t3=(double)(ObjectData->GetNz()-1);

			Point1=transX->FinalPoint;

			Value2=&MValue;
			Value1=&OValue;
			inv=false;

			tX=ModelData->GetNx();
			tY=ModelData->GetNy();
			tZ=ModelData->GetNz();
			tS=ModelData->GetSliceSize();
		}
		else				
		{
			//-----------------------
			// zwykla transformacja		
			//-----------------------

			Q1=ObjectData->GetDataArray();
			transX=new Transform(RotateCenter,ModelData->GetXscale(),ModelData->GetYscale(),ModelData->GetZscale(),oneScale);
			transX->MakeCompleteAffineMatrix(Matrix,TransT);
			transX->SetTransformationSteps(	ObjectData->GetXscale(),ObjectData->GetYscale(),ObjectData->GetZscale(),
				ModelData->GetXscale(),ModelData->GetYscale(),ModelData->GetZscale(),
				ObjectData->GetNx(),ObjectData->GetNy(),ObjectData->GetNz(),
				VolumeStepX,VolumeStepY,VolumeStepZ);




			//double tolX=1e-5;	

			t1=(double)(ModelData->GetNx()-1);
			t2=(double)(ModelData->GetNy()-1);
			t3=(double)(ModelData->GetNz()-1);


			//t1=(double)(ModelData->GetNx())-1.0;//p
			//t2=(double)(ModelData->GetNy())-1.0;//p
			//t3=(double)(ModelData->GetNz())-1.0;//p

			Point1=transX->FinalPoint;

			Value2=&OValue;
			Value1=&MValue;
			inv=true;

			tX=ObjectData->GetNx();
			tY=ObjectData->GetNy();
			tZ=ObjectData->GetNz();
			tS=ObjectData->GetSliceSize();

		}

		//--zbieram pkty do scatter plotu i histogramow...



		RestX=((tX-1)%VolumeStepX)+1;
		RestY=((tY-1)%VolumeStepY)*tX;
		SSY=(VolumeStepY-1)*tX;
		SSZ=(VolumeStepZ-1)*tS;
		Add1=RestX-VolumeStepX+SSY;
		Add2=RestY+SSZ-SSY;


		////////////////////////////////////
		XStep_x=transX->SX1;	XStep_y=transX->SY1;	XStep_z=transX->SZ1;
		YStep_x=transX->SX2;	YStep_y=transX->SY2;	YStep_z=transX->SZ2;
		ZStep_x=transX->SX3;	ZStep_y=transX->SY3;	ZStep_z=transX->SZ3;
		XBack_x=transX->SXB1;	XBack_y=transX->SYB1;	XBack_z=transX->SZB1;
		YBack_x=transX->SXB2;	YBack_y=transX->SYB2;	YBack_z=transX->SZB2;
		ZBack_x=transX->SXB3;	ZBack_y=transX->SYB3;	ZBack_z=transX->SZB3;
		ZeroYNextZ_x=transX->SYZB1;		ZeroYNextZ_y=transX->SYZB2;		ZeroYNextZ_z=transX->SYZB3;
		ZeroXNextY_x=transX->SXYB1;		ZeroXNextY_y=transX->SXYB2;		ZeroXNextY_z=transX->SXYB3;
		//////////////////////////////////////

		if (tZ<=0) tZ=1; //takie tam
		if (t3<=0) t3=1; //takie tam


		int dd=0;
		if (MisregistrationFunctionMode!=6)
		{

			if (this->parallelComputing==true)
			{
				int t[3]={tX,tY,tZ};
				int T[3]={t1,t2,t3};
				int paddNx=DataPadded->GetNx();
				int paddSS=DataPadded->GetSliceSize();
				unsigned short *paddQ=DataPadded->GetDataArray();



				int **tabs;

				tabs=new int *[numberOfThreads];

				for(k=0;k<this->numberOfThreads;k++)
				{
					tabs[k]=ScatterTabs[k];

				} 
				int *counts;
				#pragma omp parallel  num_threads(numberOfThreadsUsed) private(counts)//private (counts,tabs)//shared(Q1,paddQ)  //num_threads(1)
				{ 
					int tid = omp_get_thread_num();
					unsigned short *t1=Q1;
					unsigned short *t2=paddQ;

					counts=new int[numberOfThreads];
					#pragma omp critical
					for(k=0;k<this->numberOfThreads;k++)counts[k]=0;
					//#pragma omp critical

					#pragma omp for  nowait ordered schedule (guided) private(k) //private (counts)//ordered schedule(dynamic) p
					for ( k=0;k<tZ;k+=VolumeStepZ)
					{
						//int thNr= omp_get_thread_num();
						UpdateHistograms(k, tabs[tid], t1,t2,paddNx,paddSS,Point1,t,T,counts[tid],inv);
					}

					#pragma omp critical
					{

						for(k=0;k<numberOfThreads;k++)
							count+=counts[k];
						delete []counts;
					}

				}

				for(k=0;k<numberOfThreads;k++)
				{
					tabs[k]=NULL;
					//count+=counts[k];
				}
				delete[]tabs;
				//	delete []counts;


				//teoretycznie zebrane trzeba teraz uporzadkowac

				for(i=0;i<this->numberOfThreads;i++)
				{
					for(j=0;j<ScatterSize*ScatterSize;j++)
					{
						ScatterTab[j]+=ScatterTabs[i][j];
					}


					/*	for(j=0;j<ScatterSize;j++)
					{
					ObjectTab[j]+=ObjectTabs[i][j];
					ModelTab[j]+=ModelTabs[i][j];
					}*/


				}
				for(j=0;j<ScatterSize;j++)
					for(i=0;i<ScatterSize;i++)
					{
						ObjectTab[j]+=ScatterTab[ScatterSize*i+j];
						ModelTab[j]+=ScatterTab[ScatterSize*j+i];
					}
			}
			else
			{
				for (k=0;k<tZ;k+=VolumeStepZ)
				{			
					for (j=0;j<tY;j+=VolumeStepY)
					{
						for (i=0;i<tX;i+=VolumeStepX)
						{
							/*if((i==163)&&(j==136)&&(k==212))
							{
							i=i;
							}*/
							if ((Point1[0]>=0.0)&&(Point1[0]<t1)&&
								(Point1[1]>=0.0)&&(Point1[1]<t2)&&
								(Point1[2]>=0.0)&&(Point1[2]<t3))
							{

								/*			
								if ((CountMutual==218)&&(Point1[2]>280))
								{	FILE *Do_opa;
								dd++;
								if (dd==7000)
								{
								dd=dd;
								}
								Do_opa=fopen("c:\\delete.txt","a");fprintf(Do_opa," %d\n",dd);fclose(Do_opa);}
								*/

								*(Value1)=(int)(Interpolate->GetInterpolatedValue(DataPadded,Point1[0],Point1[1],Point1[2]));
								*(Value2)=*(Q1);
								ModelValue=tab1[MValue];
								ObjectValue=tab2[OValue];
								ObjectTab[ObjectValue]++;
								ModelTab[ModelValue]++;
								ScatterTab[ModelValue*ScatterSize+ObjectValue]++;

								if (newMask!=NULL)
								{
									int pos=nSliceSize*k+nNx*j+i;
									if (scatterMaskData[ModelValue*ScatterSize+ObjectValue]!=0)
									{
										newMask[pos]=1;
									}
								}

								count++;
							}

							Q1+=VolumeStepX;
							//transX->SetNextXCoordinate();
							Point1[0]+=XStep_x;
							Point1[1]+=XStep_y;
							Point1[2]+=XStep_z;
						}
						Q1+=Add1;
						//transX->SetZeroXNextYCoordinate();
						Point1[0]+=ZeroXNextY_x;
						Point1[1]+=ZeroXNextY_y;
						Point1[2]+=ZeroXNextY_z;
					}
					Q1+=Add2;
					//transX->SetZeroYNextZCoordinate();
					Point1[0]+=ZeroYNextZ_x;
					Point1[1]+=ZeroYNextZ_y;
					Point1[2]+=ZeroYNextZ_z;
				} 
			}
		}
		else
		{
			FloatEnergy=0;
			double tmpE;
			double tmpE1,tmpE2;
			tmpE1=0;
			tmpE2=0;
			tmpE=0;
			//FILE *Do_opa;

			for (k=0;k<tZ;k+=VolumeStepZ)
			{			
				for (j=0;j<tY;j+=VolumeStepY)
				{

					for (i=0;i<tX;i+=VolumeStepX)
					{

						if ((Point1[0]>=0.0)&&(Point1[0]<t1)&&
							(Point1[1]>=0.0)&&(Point1[1]<t2)&&
							(Point1[2]>=0.0)&&(Point1[2]<t3))
						{

							/*			
							if ((CountMutual==218)&&(Point1[2]>280))
							{	FILE *Do_opa;
							dd++;
							if (dd==7000)
							{
							dd=dd;
							}
							Do_opa=fopen("c:\\delete.txt","a");fprintf(Do_opa," %d\n",dd);fclose(Do_opa);}
							*/

							*(Value2)=*(Q1);
							if (*(Value2)>0)
							{
								*(Value1)=(int)(Interpolate->GetInterpolatedValue(DataPadded,Point1[0],Point1[1],Point1[2]));

								ModelValue=tab1[MValue];
								ObjectValue=tab2[OValue];
								//ObjectTab[ObjectValue]++;
								//ModelTab[ModelValue]++;
								//ScatterTab[ModelValue*ScatterSize+ObjectValue]++;
								tmpE+=(ModelValue-ObjectValue)*(ModelValue-ObjectValue)*0.0001;
								tmpE1+=ModelValue*ModelValue*0.0001;
								tmpE2+=(ObjectValue*ObjectValue)*0.0001;



								count++;
							}
						}


						Q1+=VolumeStepX;
						//transX->SetNextXCoordinate();
						Point1[0]+=XStep_x;
						Point1[1]+=XStep_y;
						Point1[2]+=XStep_z;
					}
					//FloatEnergy+=tmpE;
					//{Do_opa=fopen("c:\\delete.txt","a");fprintf(Do_opa," %lf\n",tmpE);fclose(Do_opa);}
					Q1+=Add1;
					//transX->SetZeroXNextYCoordinate();
					Point1[0]+=ZeroXNextY_x;
					Point1[1]+=ZeroXNextY_y;
					Point1[2]+=ZeroXNextY_z;
				}
				Q1+=Add2;
				//transX->SetZeroYNextZCoordinate();
				Point1[0]+=ZeroYNextZ_x;
				Point1[1]+=ZeroYNextZ_y;
				Point1[2]+=ZeroYNextZ_z;

				if (tmpE1*tmpE2>0)//nie wiem co to!
				{
					FloatEnergy=tmpE/(sqrt(tmpE1)*sqrt(tmpE2));
					count=1;
				}


			}

			if (count!=0) FloatEnergy/=(double)count;

		}


		//------------------------------------------------------------
		if (newMask!=NULL)
		{
			//wyslij maske
			emit SignalSendNewMask (newMask);
			if (scatterMaskData!=NULL) delete []scatterMaskData;
			scatterMaskData=NULL;
			delete[]newMask;
			newMask=NULL;
		}

		// OBLICZANIE MUTUAL INFORMATION
		double tolX=10e-7;
		double tmpf;
		count++;
		double a,b,c;
		switch(MisregistrationFunctionMode)
		{

		case 0:

			FloatEnergy=CompMutual(ScatterTab,ModelTab,ObjectTab,count,&Registration::MIFunction1); //MI
			break;

		case 1:

			FloatEnergy=2.0*CompMutual(ScatterTab,ModelTab,ObjectTab,count,&Registration::MIFunction1)/
				(GetEntropy(ModelTab,count)+GetEntropy(ObjectTab,count)); //NMI1

			break;

		case 2:

			FloatEnergy=CompMutual(ScatterTab,ModelTab,ObjectTab,count,&Registration::MIFunction5)/
				(GetEntropy(ModelTab,count)+GetEntropy(ObjectTab,count)); //NMI2			

			break;

		case 3:

			a=GetEntropy(ModelTab,count);
			b=GetEntropy(ObjectTab,count);
			c=CompMutual(ScatterTab,ModelTab,ObjectTab,count,&Registration::MIFunction5);
			FloatEnergy=-(a+b)/c;
			//FloatEnergy=-(GetEntropy(ModelTab,count)+GetEntropy(ObjectTab,count))
			//	/CompMutual(ScatterTab,ModelTab,ObjectTab,count,&Registration::MIFunction5);//NMI3

			break;


		case 4:

			FloatEnergy=CompMutual(ScatterTab,ModelTab,ObjectTab,count,&Registration::MIFunction3); //NMI4

			break;

		case 5:

			FloatEnergy=CompMutual(ScatterTab,ModelTab,ObjectTab,count,&Registration::MIFunction5); //ENTROPY

			break;





		}





		delete transX;


		// KONIEC OBLICZNIANIA MUTUAL INFORMATION

		//QString tmp;
		//		tmp="Iter:"+" Count:"+" Value:";

		//		tmp="Iter:"+QString::number(Iterations)+" Count:"+QString::number(CountMutual)+" Value:"+QString::number(FloatEnergy);
		//	tmp.sprintf("Iter:%3d Count:%4d Value:%5.5f",Iterations,CountMutual,FloatEnergy);
		//	InfoLabel->setText(tmp);

		//sprintf("Value	(%1.2f/1.00)",Value/100.0);	


		//rysowanie scatter plotu
		if(ScatterPlotVisualizationEnabled==1)
		{
			double Max=0.0;
			double *ScatterTab2=new double[ScatterSize*ScatterSize];


			int bb;
			for (i=0;i<ScatterSize*ScatterSize;i++)
			{
				bb=ScatterTab[i];
				if (bb>0)
					ScatterTab2[i]=log((double)bb);
				else
					ScatterTab2[i]=0;

				if (ScatterTab2[i]>Max) Max=ScatterTab2[i];
			}	

			//if (Max==0.0) Max=1.0;
			double ScaleFactor=(double)(255)/(double)Max;


			unsigned char *dataX;
			dataX=(unsigned char *)Image->bits();
			int x=0;
			unsigned char col;
			for (j=0;j<ScatterSize;j++)
				for (i=0;i<ScatterSize;i++)
				{
					col=Round((double)ScatterTab2[x++]*ScaleFactor);
					*(dataX++)=col;
					*(dataX++)=col;
					*(dataX++)=col;
					dataX++;

				}

				IWidget->repaint();
				delete []ScatterTab2;
		}
		if (PaintTransformedSlicesEnabled==true) PaintTransformedSlices(TransT);
		qApp->processEvents () ;
		//koniec rysowania scatter plotu


		//if (GrabApplicationWindow==true) 
		emit SignalGrabApplicationWidnow();

		return FloatEnergy;

	}
	return 0;


}

//----------------------------------------------------------------------
double Registration::GetEntropy(int *Histogram,int count)
{

	int i;
	double Entropy=0;
	//	double Entropy2=0;
	double tol=10e-6;
	double Count1=1.0/count;
	double p1;


	for (i = 0; i < ScatterSize; i++)
	{
		if(Histogram[i]!=0)
		{
			p1 = (double)Histogram[i]*Count1;
			Entropy-=p1*log(p1);
			//if ((p1<0)||(p1>=1.0))
			///{
			//	p1=p1;
			//}
			//Entropy+=log2tab[(int)(1000000*p1)];


		}
	}

	return Entropy;

}
//----------------------------------------------------------------------


//----------------------------------------------------------------------
double Registration::CompMutual(int *ST,int *MT,int *OT,int count,double (Registration::*funcs) (double p,double p1,double p2))
{
	/*
	//		double TempEnergy;
	double Energy=0;
	double p,p1,p2;
	int *sp=ST;
	int i,j;
	double Count1=1.0/count;
	for (j = 0; j < ScatterSize; j++)
	{
	p1 = (double)MT[j]*Count1;

	for (i = 0; i < ScatterSize; i++)
	{    
	if ((*sp!=0)&&(MT[j]!=0)&&(OT[i]!=0))             
	{
	p = (double)*sp*Count1;
	p2 = (double)OT[i]*Count1;

	Energy+=(this->*funcs)(p,p1,p2);			
	}
	sp++;
	} 
	}

	return Energy;
	*/
	double Energy=0;
	double p,p1,p2;
	int *sp=ST;
	int i,j;
	double Count1=1.0/count;
	for (j = 0; j < ScatterSize; j++)
	{
		p1 = (double)MT[j]*Count1;

		for (i = 0; i < ScatterSize; i++)
		{    
			if ((*sp!=0)&&(MT[j]!=0)&&(OT[i]!=0))             
			{
				p = (double)*sp*Count1;
				p2 = (double)OT[i]*Count1;

				Energy+=(this->*funcs)(p,p1,p2);			
			}
			sp++;
		} 
	}

	return Energy;
}

//FloatEnergy+=p*log(p/(p1*p2));
//	TempEnergy=( p1*log(p1) + p2*log(p2) ) / (p*log(p));
//	Do_opa=fopen("c:\\MI.txt","a");fprintf(Do_opa,"(%lf*%lf + %lf*%lf) / %lf*%lf = %lf\n",
//					p1,log (p1),p2,log(p2),p,log(p),TempEnergy);fclose(Do_opa);
//	FloatEnergy-=TempEnergy;


//  FloatEnergy+=p*log((double)(count)*(*sp)/((double)(ModelTab[i])*(double)(ObjectTab[j])));
//	FloatEnergy -=( p1*log(p1) + p2*log(p2) ) / (p*log(p));
//------------------------------------------------------------

//---------------------------------------------------

void Registration::PrepareVariables(int nr)
{

	int i;

	//		time1=0;
	//		time2=0;

	ScatterSize=(1<<((int)Trans[21]+4));
	TransformationMode=(int)Trans[22];
	InterpolationMode=(int)Trans[23];
	ScatterPlotVisualizationEnabled=(bool)(int)Trans[24];
	MisregistrationFunctionMode=(int)Trans[25];
	Tolerance=pow((double)10,(double)Trans[26]);
	TransXPos=(int)Trans[27];
	TransYPos=(int)Trans[28];
	TransZPos=(int)Trans[29];
	XGraphResolution=(int)Trans[30];
	YGraphResolution=(int)Trans[31];
	ZGraphResolution=(int)Trans[32];
	TransGraphSize=(double)Trans[33];
	RotGraphSize=(double)Trans[34];

	if (ModelData->GetMaxValue()<1) ModelData->FindMinMaxValue();
	if (ObjectData->GetMaxValue()<1) ObjectData->FindMinMaxValue();

	///////////////////////////////////////////////////////
	if (Interpolate!=NULL) delete Interpolate;
	/*
	if(TransformationMode==1)	//transformacja odwrotna
	DataPadded=ObjectData;//PaddingBySize(1,ObjectData);
	else						//transformacja normalna (dziurawa)
	DataPadded=ModelData;//PaddingBySize(1,ModelData);
	*/

	if (((TransformationMode==1)&&(ObjectData->GetNz()==1))||((TransformationMode==0)&&(ModelData->GetNz()==1)))
	{
		if (InterpolationMode==0)
		{
			//2dtrilin
			Interpolate=new InterpolationFunction2DBilinear();
		}
		else if(InterpolationMode==1)
		{
			//2dnearest	
			Interpolate=new InterpolationFunction2DNearest();
		}


	}
	else if (InterpolationMode==0)
	{
		//trilin
		Interpolate=new InterpolationFunctionTrilinear();
	}
	else if(InterpolationMode==1)
	{
		//nearest	
		Interpolate=new InterpolationFunctionNearest();
	}
	else
	{
		//nothing

		Interpolate=new InterpolationFunctionNearest();

	}
	/////////////////////////////////////////////////////////////////////////


	//RotateCenter=new double[3];

	if (Trans[13]==1.0)
	{
		RotateCenter[0]=((double)(ObjectData->GetNx()-1)*(ObjectData->GetXscale()))/2.0;
		RotateCenter[1]=((double)(ObjectData->GetNy()-1)*(ObjectData->GetYscale()))/2.0;
		RotateCenter[2]=((double)(ObjectData->GetNz()-1)*(ObjectData->GetZscale()))/2.0;
	}
	else
	{
		RotateCenter[0]=Trans[14];
		RotateCenter[1]=Trans[15];
		RotateCenter[2]=Trans[16];
	}

	emit SignalSendRotateCenter(RotateCenter);

	VolumeStepX=((int)Trans[18]<(int)(ModelData->GetNx()))?(int)Trans[18]:(int)(ModelData->GetNx());
	VolumeStepY=((int)Trans[19]<(int)(ModelData->GetNy()))?(int)Trans[19]:(int)(ModelData->GetNy());
	VolumeStepZ=((int)Trans[20]<(int)(ModelData->GetNz()))?(int)Trans[20]:(int)(ModelData->GetNz());

	TransTemp=new double[TransSize]; /// tak na zapas

	for(i=0;i<3;i++)
		TransTemp[i+1]=Trans[i];
	for(i=3;i<6;i++)
		TransTemp[i+1]=Trans[i]* to_rad;
	//!!!!!!!!!!!!!!!!!!!1
	for(i=6;i<13;i++)
		TransTemp[i+1]=Trans[i]; 



	// licze ile parametrow optymalizowac
	NumberOfOptimParams=0;
	for(i=35;i<47;i++)
	{
		if (((bool)(int)Trans[i])==true) 
			NumberOfOptimParams++;
	}

	// robie tablice z parametrami
	if (NumberOfOptimParams>0)
	{

		if (ParamTransTab!=NULL) delete ParamTransTab;

		ParamTransTab=new int[NumberOfOptimParams+1];

		int pos=1;
		for(i=35;i<=47;i++)
		{
			if (((bool)(int)Trans[i])==true)
			{
				ParamTransTab[pos++]=i-34; //zeby bylo od 1 do 12 :) 

			}

		}
	}


	//	35 47
	//	int *ParamTransTab;
	//	int NumberOfOptimParams;

	// shear
	/*	
	TransTemp[7]=0;
	TransTemp[8]=0;
	TransTemp[9]=0;

	// scale

	TransTemp[10]=1.0;
	TransTemp[11]=1.0;
	TransTemp[12]=1.0;
	*/
	/*		TransTemp[7]=0.002961;
	TransTemp[8]=-0.017609;
	TransTemp[9]=-0.002838;

	// scale

	TransTemp[10]=0.995021 ;
	TransTemp[11]=1.006149;
	TransTemp[12]=0.958907;
	*/



	//Matrix=new double [16];
	/*
	Ox=1.0/ObjectData->GetXscale();
	Oy=1.0/ObjectData->GetYscale();
	Oz=1.0/ObjectData->GetZscale();
	*/

	if (nr==0)
	{
		DataPadded=ObjectData;//PaddingBySize(1,ObjectData); // zawsze transformacja odwrotna dla wygeneroania volume
	}


	if (nr==1)
	{

		if (Image!=NULL) delete Image;
		//		Image = new QImage(ScatterSize,ScatterSize,QImage::Format_RGB32);
		Image = new QImage(ScatterSize, ScatterSize, QImage::Format_ARGB32);
		IWidget->setValues(Image, ScatterSize, ScatterSize,0.0,0.0);
		IWidget->SetMaskPaintMode(1);

		if (ScatterPlotVisualizationEnabled==true)
		{
			IWidget->show();
			IWidget->raise();
		}
		else
		{
			IWidget->hide();
		}

		//nowe!!!!!!!!!!!!!!

		if(TransformationMode==1)	//transformacja odwrotna
			DataPadded=ObjectData;//PaddingBySize(1,ObjectData);
		else						//transformacja normalna (dziurawa)
			DataPadded=ModelData;//PaddingBySize(1,ModelData);

		ObjectTab=new int[ScatterSize];
		ModelTab=new int[ScatterSize];
		ScatterTab=new int[ScatterSize*ScatterSize];


		for ( i=0;i<numberOfThreads;i++)
		{
			//	ObjectTabs[i]=new int[ScatterSize];
			//	ModelTabs[i]=new int[ScatterSize];
			ScatterTabs[i]=new int[ScatterSize*ScatterSize];
		}



		tab1=new int[ModelData->GetMaxValue()+1];
		tab2=new int[ObjectData->GetMaxValue()+1];
		for(i=0;i<ModelData->GetMaxValue()+1;i++)
			tab1[i]=Round((double)((ScatterSize-1)*i)/(double)ModelData->GetMaxValue());

		for(i=0;i<ObjectData->GetMaxValue()+1;i++)
			tab2[i]=Round((double)((ScatterSize-1)*i)/(double)ObjectData->GetMaxValue());

	}








	//-------------------------------------------------
	if(PaintTransformedSlicesEnabled==true)
	{
		/*	PaletteReg=new PaletteRGBA;

		for(i=0;i<256;i++)
		{

		for( i=0;i<256;i++)
		PaletteReg->R[i]=i;

		for( i=0;i<256;i++)
		PaletteReg->G[i]=i;

		for( i=0;i<256;i++)
		PaletteReg->B[i]=i;
		}	
		*/
		TransversalDataSet = new RawDataSet(ModelData->GetNx(),ModelData->GetNy(),1,1,1,1,0);
		SagittalDataSet = new RawDataSet(ModelData->GetNy(),ModelData->GetNz(),1,1,1,1,0);
		CoronalDataSet = new RawDataSet(ModelData->GetNx(),ModelData->GetNz(),1,1,1,1,0);
	}

	//------------------------------------------------------



	DataPrepared=true;


}

//-------------------------------------------------------------------

void Registration::UnprepareVariables(int nr)
{


	//delete	[]RotateCenter;
	//delete	[]Matrix;

	//	delete  DataPadded;

	if (nr==1)
	{
		delete	[]tab1;
		delete	[]tab2;
		delete	[]ObjectTab;
		delete	[]ModelTab;
		delete	[]ScatterTab;
		tab1=NULL;
		tab2=NULL;
		ObjectTab=NULL;
		ModelTab=NULL;
		ScatterTab=NULL;

		for (int i=0;i<numberOfThreads;i++)
		{
			//delete []ObjectTabs[i];		ObjectTabs[i]=NULL;
			//delete []ModelTabs[i];		ModelTabs[i]=NULL;
			delete []ScatterTabs[i];	ScatterTabs[i]=NULL;
		}

	}

	delete	[]TransTemp;

	//RotateCenter=NULL;
	//Matrix=NULL;
	//	`=NULL;
	TransTemp=NULL;



	///------------------------------
	if(PaintTransformedSlicesEnabled==true)
	{
		delete TransversalDataSet;
		delete SagittalDataSet;
		delete CoronalDataSet;
		//	delete	PaletteReg;
	}
	//-------------------------------
	DataPrepared=false;
}


//-------------------------------------------------------------------

void Registration::GeneratePolyMisregistrationGraphData( VtkWidget *VWidget)
{

	if((ModelData!=NULL)&&(ObjectData!=NULL))
	{

		PrepareVariables(1);
		if (DataPrepared==true)
		{

			double XResolution=(double)XGraphResolution;
			double YResolution=(double)YGraphResolution;

			vtkIdType	newCellPts[3];
			double		newPt[3];
			int			NumberOfPoints=XGraphResolution*YGraphResolution;
			vtkPoints	*newPts=vtkPoints::New();
			vtkFloatArray *Tscalars = vtkFloatArray::New();
			newPts->SetNumberOfPoints(NumberOfPoints);
			int ptId;
			double scalar=1.0;



			double ROTT=RotGraphSize;
			double TRANST=TransGraphSize;


			double XMinT=-TRANST;
			double XMaxT=TRANST;
			double YMinT=-TRANST;
			double YMaxT=TRANST;


			double XMinR=-ROTT* to_rad;
			double XMaxR=ROTT* to_rad;
			double YMinR=-ROTT* to_rad;
			double YMaxR=ROTT* to_rad;




			int i,j;
			double XX,YY;
			double XMin,YMin,XMax,YMax;

			double *TempTrans= new double [TransSize];
			double XRange;
			double YRange;
			double RangeStepX;
			double RangeStepY;
			int ParamX;
			int ParamY;


			double *a=new double[TransSize];


			for(i=0;i<3;i++)
				a[i+1]=Trans[i];
			for(i=3;i<6;i++)
				a[i+1]=Trans[i]* to_rad;

			for(i=6;i<13;i++)
				a[i+1]=Trans[i]; 


			ParamX=TransXPos;
			ParamY=TransYPos;


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
			RangeStepX = XRange /  XResolution;
			RangeStepY = YRange / YResolution;

			for(i=1;i<TransSize;i++)
				TempTrans[i]=a[i];

			ptId=0;

			double MaxZ=-999999999.0;
			double MinZ=999999999.0;

			for(j=0;j<YResolution;j++)
			{
				for(i=0;i<XResolution;i++)
				{

					XX=XMin +(double)i*RangeStepX;
					YY=YMin +(double)j*RangeStepY;
					TempTrans[ParamX]=a[ParamX]+XX;
					TempTrans[ParamY]=a[ParamY]+YY;

					newPt[0] =i;
					newPt[1] =j;
					newPt[2] =ComputeMutualInformation(TempTrans);
					newPts->SetPoint(ptId,newPt);

					Tscalars->InsertTuple(ptId,&scalar); 
					ptId++;
					MaxZ=MAX(MaxZ,newPt[2]);
					MinZ=MIN(MinZ,newPt[2]);
				}
			}

			double Scale=0.5*((XResolution-1))/(MaxZ-MinZ);		


			for(i=0;i<NumberOfPoints;i++)
			{
				newPts->GetPoint(i,newPt);
				newPt[2]=(newPt[2]-MinZ)*Scale;

				newPts->SetPoint(i,newPt);
			}


			for(i=1;i<7;i++)
				TransTemp[i]=a[i];






			//---------------------------------------------------






			int NumberOfTriangles=NumberOfPoints*2;
			vtkCellArray *newPolys = vtkCellArray::New();
			newPolys->Allocate(newPolys->EstimateSize(3,NumberOfTriangles));

			vtkPolyData *output=vtkPolyData::New();
			output->SetPoints(newPts);
			output->SetPolys(newPolys);

			//			vtkCellData		*outputCD=output->GetCellData();


			//			int numNewPts = 0;




			//outputCD->SetScalars(Tscalars); 
			output->GetPointData()->SetScalars(Tscalars); 
			newPts->SetNumberOfPoints(NumberOfPoints);
			output->BuildLinks();
			vtkIdType ID;


			//cellId=0;
			for(i=0;i<XResolution-1;i++)
			{

				for(j=0;j<YResolution-1;j++)
				{
					//Pierwszy Trojkat
					//newPts->GetPoint(j*XResolution+i,newPt);
					newCellPts[0] =j*XGraphResolution+i;
					newCellPts[1] =j*XGraphResolution+i+1;
					newCellPts[2] =(j+1)*XGraphResolution+i;
					ID=newPolys->InsertNextCell(3,newCellPts);


					//cellId++;

					//DrugiTrojkat
					newCellPts[0] =j*XGraphResolution+i+1;
					newCellPts[1] =(j+1)*XGraphResolution+i+1;
					newCellPts[2] =(j+1)*XGraphResolution+i;
					ID=newPolys->InsertNextCell(3,newCellPts);	



				}
			}


			output->BuildCells();
//			//output->Update();
			newPolys->Delete();
			newPts->Delete();


			PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);


			Output1PolygonSurfaceData->GetPN()[0]=XGraphResolution;
			Output1PolygonSurfaceData->GetPScale()[0]=RangeStepX;
			Output1PolygonSurfaceData->GetPN()[1]=XGraphResolution;
			Output1PolygonSurfaceData->GetPScale()[1]=RangeStepX;
			Output1PolygonSurfaceData->GetPN()[2]=(int)MaxZ;
			Output1PolygonSurfaceData->GetPScale()[2]=1.0;



			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("GRAPH");

			emit	SignalSendNewSurface(Output1PolygonSurfaceData);
		}

		UnprepareVariables(1);









	}


}



//-------------------------------------------------------------------


void Registration::Generate3DVolumeMisregistrationGraphData()
{

	if((ModelData!=NULL)&&(ObjectData!=NULL))
	{

		PrepareVariables(1);
		if (DataPrepared==true)
		{



			int XResolution=XGraphResolution;
			int YResolution=YGraphResolution;
			int ZResolution=ZGraphResolution;


			double ROTT=RotGraphSize;
			double TRANST=TransGraphSize;


			double XMinT=-TRANST;
			double XMaxT=TRANST;
			double XMinR=-ROTT* to_rad;
			double XMaxR=ROTT* to_rad;

			double YMinR=-ROTT* to_rad;
			double YMaxR=ROTT* to_rad;
			double YMinT=-TRANST;
			double YMaxT=TRANST;


			double ZMinT=-TRANST;
			double ZMaxT=TRANST;
			double ZMinR=-ROTT* to_rad;
			double ZMaxR=ROTT* to_rad;

			int i,j,k;
			double XX,YY,ZZ;
			double XMin,YMin,ZMin,XMax,YMax,ZMax;

			double *TempTrans= new double [TransSize];
			double XRange;
			double YRange;
			double ZRange;

			double RangeStepX;
			double RangeStepY;
			double RangeStepZ;

			int ParamX;
			int ParamY;
			int ParamZ;


			double *a=new double[TransSize];


			for(i=0;i<3;i++)
				a[i+1]=Trans[i];
			for(i=3;i<6;i++)
				a[i+1]=Trans[i]* to_rad;


			for(i=6;i<13;i++)
				a[i+1]=Trans[i];

			ParamX=TransXPos;
			ParamY=TransYPos;
			ParamZ=TransZPos;


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

			if (ParamZ<4) 
			{
				ZMin=ZMinT;
				ZMax=ZMaxT;
			}
			else
			{
				ZMin=ZMinR;
				ZMax=ZMaxR;
			}

			XRange=XMax-XMin;
			YRange=YMax-YMin;
			ZRange=ZMax-ZMin;			

			RangeStepX = XRange / (double)XResolution;
			RangeStepY = YRange / (double)YResolution;
			RangeStepZ = ZRange / (double)ZResolution;


			for(i=1;i<TransSize;i++)
				TempTrans[i]=a[i];



			double MaxValue=-999999999.0;
			double MinValue=999999999.0;
			double Value;


			double *TempFloatTab=new double[XResolution*YResolution*ZResolution];
			//int XYR=XResolution*YResolution;
			int pos=0;
			for(k=0;k<ZResolution;k++)
			{		
				for(j=0;j<YResolution;j++)
				{
					for(i=0;i<XResolution;i++)
					{

						XX=XMin +(double)i*RangeStepX;
						YY=YMin +(double)j*RangeStepY;
						ZZ=ZMin +(double)k*RangeStepZ;

						TempTrans[ParamX]=a[ParamX]+XX;
						TempTrans[ParamY]=a[ParamY]+YY;
						TempTrans[ParamZ]=a[ParamZ]+ZZ;

						Value=ComputeMutualInformation(TempTrans);

						TempFloatTab[pos++]=Value;


						MaxValue=MAX(MaxValue,Value);
						MinValue=MIN(MinValue,Value);





					}
				}
			}



			RawDataSet *dataX=new RawDataSet(XResolution,YResolution,ZResolution,1.0,1.0,1.0,0);



			dataX->SetRawDataSetName("3D Misreg");


			double Scale=32767.0/(MaxValue-MinValue);
			for(k=0;k<dataX->GetVolumeSize();k++)
				dataX->GetDataArray()[k]=Round((TempFloatTab[k]-MinValue)*Scale);



			delete []TempFloatTab;

			emit SignalInsertRawDataSet(dataX);




		}

		UnprepareVariables(1);









	}


}



//-------------------------------------------------------------------

void Registration::GenerateMisregistrationGraphData(const QString &filename)
{

	if((ModelData!=NULL)&&(ObjectData!=NULL))
	{

		PrepareVariables(1);
		if ((DataPrepared==true)&&(!filename.isEmpty()))
		{
			/*		FILE *Do_opa;

			int i;
			double *TempTrans= new double [7];

			for(i=1;i<7;i++)
			TempTrans[i]=0;
			Do_opa=fopen("c:\\XSEC.dat","w");fprintf(Do_opa,"");fclose(Do_opa);

			int Z=300;
			for (i=-Z;i<=Z;i++)
			{

			TempTrans[3]=(double)i/3;
			Do_opa=fopen("c:\\XSEC.dat","a");fprintf(Do_opa,"%d %lf\n",i, ComputeMutualInformation(TempTrans));fclose(Do_opa);
			}
			*/		





			double ROTT=RotGraphSize;
			double TRANST=TransGraphSize;

			QString NAME="Misregistration graph";

			double XMinT=-TRANST;
			double XMaxT=TRANST;
			double YMinT=-TRANST;
			double YMaxT=TRANST;


			double XMinR=-ROTT* to_rad;
			double XMaxR=ROTT* to_rad;
			double YMinR=-ROTT* to_rad;
			double YMaxR=ROTT* to_rad;

			double XResolution=(double)XGraphResolution;
			double YResolution=(double)YGraphResolution;




			FILE *Do_opa;
			int i,j;
			double XX,YY;
			double XMin,YMin,XMax,YMax;
			double Energy;
			double *TempTrans= new double [TransSize];
			double XRange;
			double YRange;
			double RangeStepX;
			double RangeStepY;
			int ParamX;
			int ParamY;
			int StepY=2;

			double *a=new double[TransSize];

			//const char *name,*name2,*name3;	

			QString filename2;
			QString filename3;
			QString txt;







			filename2=filename+".plt";

			/*	QByteArray adata;
			adata=filename2.totoLocal8Bit();
			name=adata.constData();
			*/
			//name=filename2.toLocal8Bit();
			Do_opa = fopen(filename2.toLocal8Bit(), "w");

			fprintf(Do_opa,"set cntrparam levels 10\n"); 
			fprintf(Do_opa,"set contour base \n");
			fprintf(Do_opa,"set cntrparam bspline \n");
			fprintf(Do_opa,"set cntrparam order 5 \n");
			fprintf(Do_opa,"set nokey \n");
			fprintf(Do_opa,"set hidden3d\n");


			fclose(Do_opa);




			for(i=0;i<3;i++)
				a[i+1]=Trans[i];
			for(i=3;i<6;i++)
				a[i+1]=Trans[i]* to_rad;

			for(i=6;i<13;i++)
				a[i+1]=Trans[i];

			for (ParamX=1;ParamX<7;ParamX++)
			{
				for (ParamY=StepY;ParamY<7;ParamY++)
				{

					filename2=filename+"_"+QString::number(ParamX)+"_"+QString::number(ParamY)+".dat";
					//name=filename2.toLocal8Bit();
					/*QByteArray adata1;
					adata1=filename2.totoLocal8Bit();
					name=adata1.constData();*/

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

					for(i=1;i<TransSize;i++)
						TempTrans[i]=a[i];


					Do_opa = fopen(filename2.toLocal8Bit(), "w");
					for(i=0;i<=XResolution;i++)
					{
						fprintf(Do_opa,"\n# i = %d \n",i);
						for(j=0;j<=YResolution;j++)
						{
							XX=XMin +i*RangeStepX;
							YY=YMin +j*RangeStepY;

							TempTrans[ParamX]=a[ParamX]+XX;
							TempTrans[ParamY]=a[ParamY]+YY;

							//	TransTemp[ParamX]=TempTrans[ParamX];
							//	TransTemp[ParamY]=TempTrans[ParamY];
							//		if (Scaled256==true)
							//			Energy = ComputeMutualInformation256(TempTrans);
							//		else
							Energy = ComputeMutualInformation(TempTrans);
							fprintf(Do_opa,"%lf\n",Energy);      

						}
					}
					fclose(Do_opa);

					filename2=filename+".plt";
					//name=filename2.toLocal8Bit();

					/*adata=filename2.totoLocal8Bit();
					name=adata.constData();*/
					Do_opa = fopen(filename2.toLocal8Bit(), "a");

					fprintf(Do_opa,"\n");



					txt=TransSymbolName(ParamX)+"-"+TransSymbolName(ParamY);
					//name2=txt.totoLocal8Bit();

					//name3=NAME.totoLocal8Bit();
					//					QByteArray adata;
					//QByteArray adata4=txt.totoLocal8Bit();
					//name2=adata4.constData();
					//QByteArray adata5=NAME.totoLocal8Bit();
					//name3=adata5.constData();
					//name2=txt.toLocal8Bit();
					//name3=NAME.toLocal8Bit();

					fprintf(Do_opa, "set title '%s %s' -10,-1.5 \n", NAME.toLocal8Bit(), txt.toLocal8Bit());

					txt=TransSymbolName(ParamY);
					//name2=txt.totoLocal8Bit();
					//QByteArray adata;
					//					QByteArray adata6=txt.toLocal8Bit();
					//					name2=adata6.constData();
					//name2=txt.toLocal8Bit();
					fprintf(Do_opa, "set xlabel '%s'\n", txt.toLocal8Bit());

					txt=TransSymbolName(ParamX);
					//name2=txt.totoLocal8Bit();
					//					QByteArray adata;
					//QByteArray adata7=txt.totoLocal8Bit();
					//name2=adata7.constData();
					//name2=txt.toLocal8Bit();
					fprintf(Do_opa, "set ylabel '%s'\n", txt.toLocal8Bit());

					if (ParamX<4) 
						fprintf(Do_opa,"set ytics ('%4.2f' 0, '0' %4.2f, '%4.2f' %lf)\n",XMin,XResolution/2.0,XMax,XResolution); 
					else
						fprintf(Do_opa,"set ytics ('%4.2f' 0, '0' %lf, '%4.2f' %lf)\n",XMin* to_deg,XResolution/2.0,XMax* to_deg,XResolution);


					if (ParamY<4) 
						fprintf(Do_opa,"set xtics ('%4.2f' 0, '0' %4.2f, '%4.2f' %lf)\n",YMin,YResolution/2.0,YMax,YResolution);
					else
						fprintf(Do_opa,"set xtics ('%4.2f' 0, '0' %4.2f, '%4.2f' %lf)\n",YMin* to_deg,YResolution/2.0,YMax* to_deg,YResolution);


					fprintf(Do_opa,"set view 55,60,1,1.5\n");
					fprintf(Do_opa,"set terminal postscript eps monochrome solid 'Times-Roman' 20\n");

					txt=TransSymbolName(ParamX)+"-"+TransSymbolName(ParamY);
					//name2=txt.totoLocal8Bit();
					///					QByteArray adata;
					//QByteArray adata8=txt.totoLocal8Bit();
					//name2=adata8.constData();

					//name2=txt.toLocal8Bit();
					fprintf(Do_opa, "set output 'Plot-%s.eps' \n", txt.toLocal8Bit());

					filename3=filename+"_"+QString::number(ParamX)+"_"+QString::number(ParamY)+".dat";
					filename3=filename3.replace( QRegExp("/"), "\\"); 
					//name2=filename3.totoLocal8Bit();

					//					QByteArray adata9=filename3.totoLocal8Bit();
					//					name2=adata9.constData();
					//name2=filename3.toLocal8Bit();
					fprintf(Do_opa, "splot '%s' with lines 6\n", filename3.toLocal8Bit());





					for(i=1;i<7;i++)
						TransTemp[i]=a[i];


					fclose(Do_opa);
				}
				StepY++;
			}


		}
		UnprepareVariables(1);
	}

}

//-----------------------------------------------

QString Registration::TransSymbolName(int nr)
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
//-----------------------------------------------

double	Registration::GetFunctionValue(double *TT)
{
	//To jest przeciazona wersja!
	FILE *Do_opa;

	GetRealTransform(TransXX,TT,ParamTransTab,NumberOfOptimParams);

	double Value= ComputeMutualInformation(TransXX);

	emit SignalSendActualTransform (TransXX);

	double add=0;
	if (constraints==true)
	{
		add=getConstraintUpdate(TransXX);
		//		Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%d  mutual= %lfm %lfm\n",CountMutual,Value,add);fclose(Do_opa);

		Value+=add;
	}

	//bool constraints;


	int j;
	for(j=1;j<4;j++)	
		TabVal[CountMutual][j]=TT[j];

	for(j=4;j<7;j++)	
		TabVal[CountMutual][j]=TT[j]* to_deg;

	TabVal[CountMutual][7]=Value;

	/*int i;	for(i=1;i<4;i++)
	{Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%lf\n",TT[i]);fclose(Do_opa);}

	for(i=4;i<7;i++)
	{Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%lf\n",TT[i]*deg);fclose(Do_opa);}
	*/



	QString tmp;
	tmp.sprintf("Iter:%3d Count:%4d Value:%5.5f",Iterations,CountMutual,Value);
	InfoLabel->setText(tmp);
	lastFValue = Value;

	return Value;


}
//-----------------------------------------------

void Registration::ApplyPowellsRegistration()
{

	bool OptimStopped=false;
	if((ModelData!=NULL)&&(ObjectData!=NULL))
	{
		PaintTransformedSlicesEnabled=true;

		CountMutual=0;
		FILE *Do_opa;

		Iterations=0;
		PrepareVariables(1);

		if(NumberOfOptimParams==0)
		{
			QMessageBox::about( 0,"","No Parameters To Optimize");
			OptimizationSuccsessfull=false;
			return;

		}

		double FValue;

		if (TransXX!=NULL) delete TransXX;
		TransXX=new double[TransSize];

		int i;

		int tempol=0;
		for(i=1;i<13;i++)
		{
			if ((i<7)&&(TransTemp[i]!=0)) tempol++;
			TransXX[i]=TransTemp[i];
		}
		//align centers
		if ((Trans[17]==1.0)&&(tempol==0))
		{

			double ModelCenter[3],ObjectCenter[3];


			ModelCenter[0]=ModelData->GetXscale()*(double)(ModelData->GetNx()-1)/2.0;
			ModelCenter[1]=ModelData->GetYscale()*(double)(ModelData->GetNy()-1)/2.0;
			ModelCenter[2]=ModelData->GetZscale()*(double)(ModelData->GetNz()-1)/2.0;

			ObjectCenter[0]=ObjectData->GetXscale()*(double)(ObjectData->GetNx()-1)/2.0;
			ObjectCenter[1]=ObjectData->GetYscale()*(double)(ObjectData->GetNy()-1)/2.0;
			ObjectCenter[2]=ObjectData->GetZscale()*(double)(ObjectData->GetNz()-1)/2.0;

			for(i=0;i<3;i++)
				TransXX[i+1]=ModelCenter[i]-ObjectCenter[i];


		}




		/*Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"przed powellem\n");fclose(Do_opa);
		for(i=1;i<4;i++)
		{Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%lf\n",TransXX[i]);fclose(Do_opa);}

		for(i=4;i<7;i++)
		{Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%lf\n",TransXX[i]*deg);fclose(Do_opa);}


		for(i=7;i<13;i++)
		{Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%lf\n",TransXX[i]);fclose(Do_opa);}*/




		//------------------------------
		// 2003.10.05 - teraz robie tak zeby mozna bylo optymalizowac rozne parametry

		// robie tablice z parametrami do optymalizacji
		double *OptimTrans=new double[NumberOfOptimParams+1];

		GetOptimTransform(OptimTrans,TransXX,ParamTransTab,NumberOfOptimParams);




		//------------------------------

		double temptol=MIN3(
			ModelData->GetXscale(),
			ModelData->GetYscale(),
			ModelData->GetZscale())/100.0;

		OptimizationPowell *Optim=new OptimizationPowell(OptimTrans,NumberOfOptimParams,Tolerance,&Iterations,&FValue,this,temptol); 
		connect(this,SIGNAL(SignalStopOptimization()),Optim,SLOT(SlotStopOptimization())); 

		//
		Optim->StartOptimization();
		OptimStopped=Optim->OptimizationStopped;

		delete Optim;


		GetRealTransform(TransXX,OptimTrans,ParamTransTab,NumberOfOptimParams);
		delete []OptimTrans;

		/*Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"Final MI=%lf \n",ComputeMutualInformation(TransXX));fclose(Do_opa);


		Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"po powellku\n");fclose(Do_opa);

		for(i=1;i<4;i++)
		{Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%lf\n",TransXX[i]);fclose(Do_opa);}

		for(i=4;i<7;i++)
		{Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%lf\n",TransXX[i]*deg);fclose(Do_opa);}

		for(i=7;i<13;i++)
		{Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"%lf\n",TransXX[i]);fclose(Do_opa);}

		*/


		for(i=1;i<13;i++)
			FinalTrans[i]=TransXX[i];









		/*
		/////////////////////////////////////////////////////////
		int GraphXResolution=74;
		int GraphYResolution=74;
		double ROTT=15.0;
		double TRANST=15.0;
		int XP,YP;
		int NrIter=CountMutual-1;
		int ParamX;
		int ParamY;
		int StepY=2;
		double Rangea,RangeHa;
		double Rangeb,RangeHb;
		QString filename,filename2;
		const char *name;
		filename="c:\\pathPowell";


		double Tabelka[74][74];


		////////////////////////////////////////////

		//!!!!!!!!!!!!!!

		int j;
		for (ParamX=1;ParamX<7;ParamX++)
		{
		for (ParamY=StepY;ParamY<7;ParamY++)
		{


		if (ParamX<4) 
		RangeHa=TRANST;
		else
		RangeHa=ROTT;

		if (ParamY<4) 
		RangeHb=TRANST;
		else
		RangeHb=ROTT;

		Rangea=2.0*RangeHa;
		Rangeb=2.0*RangeHb;

		for(i=0;i<74;i++)
		for(j=0;j<74;j++)
		Tabelka[i][j]=0;


		filename2=filename+"_"+QString::number(ParamX)+"_"+QString::number(ParamY)+".dat";
		name=filename2.totoLocal8Bit();


		//Do_opa=fopen("c:\\tmpa.dat","w");fclose(Do_opa);Do_opa=fopen("c:\\tmpa.dat","a");

		for(i=0;i<NrIter;i++)
		{	

		XP=Round(((TabVal[i][ParamX]+RangeHa-TabVal[NrIter][ParamX])*(double)GraphXResolution)/(Rangea));
		YP=Round(((TabVal[i][ParamY]+RangeHb-TabVal[NrIter][ParamY])*(double)GraphYResolution)/(Rangeb));

		//fprintf(Do_opa,"%d %d %lf \n",XP,YP,TabVal[i][7]);	

		if (((XP>=0)&&(XP<GraphXResolution))&&((YP>=0)&&(YP<GraphYResolution)))
		Tabelka[XP][YP]=TabVal[i][7];
		}
		//fclose(Do_opa);

		//nagrywam tabelke na dysk


		Do_opa=fopen(name,"w");

		for(i=0;i<GraphXResolution;i++)
		{
		fprintf(Do_opa,"\n");
		for(j=0;j<GraphYResolution;j++)
		fprintf(Do_opa,"%lf\n",Tabelka[i][j]);

		}
		fclose(Do_opa);
		}
		StepY++;
		}

		*/




		UnprepareVariables(1);
		delete []TransXX;
	}
	if (!OptimStopped) OptimizationSuccsessfull=true;


}

//------------------------------------------------------------

void	Registration::SlotSetViewPos(int Ix,int Iy,int Iz,int Px,int Py,int Pz)
{
	InspX=Ix;
	InspY=Iy;
	InspZ=Iz;
	PosX=Px;
	PosY=Py;
	PosZ=Pz;
}

//------------------------------------------------------------

void	Registration::SlotSet2DSlicesPos(int Px,int Py,int Pz)
{

	PosX=Px;
	PosY=Py;
	PosZ=Pz;
}


//------------------------------------------------------------
void	Registration::SlotSwitchScatterState(int State)
{
	ScatterPlotVisualizationEnabled=!ScatterPlotVisualizationEnabled;


	if (ScatterPlotVisualizationEnabled==true)
	{
		IWidget->show();
	}
	else
	{
		IWidget->hide();
	}
}

void	Registration::GetRealTransform(double *RealTransform,double *OptimTransform,int *ParamTrans,int NrOfParams)
{
	// wpisuje do prawdziwej transformacji parametry zoptymalizowane
	int i;
	for (i=1;i<=NrOfParams;i++)
	{

		RealTransform[ParamTrans[i]]=OptimTransform[i];
	}

}

void	Registration::GetOptimTransform(double *OptimTransform,double *RealTransform,int *ParamTrans,int NrOfParams)
{
	// przepisuje do transformacji optim te parametry ktore chce optymalizowac
	int i;
	for (i=1;i<=NrOfParams;i++)
	{
		OptimTransform[i]=RealTransform[ParamTrans[i]];
	}
}
//---------------------
void	Registration::SlotStopOptimization()
{

	emit SignalStopOptimization();
}
//---------------------
void	Registration::setScatterMaskData(unsigned char *m)
{
	if (scatterMaskData!=NULL)
		delete []scatterMaskData;
	int size=ScatterSize*ScatterSize;

	scatterMaskData=new unsigned char[size];
	int i;

	unsigned int *tt=(unsigned int*)m;

	for(i=0;i<size;i++)
		scatterMaskData[i]=((*tt++)==0xffffffff)?1:0;
	//scatterMaskData[i]=m[i];

}
//----------------------
void	Registration::setScatterMaskData(unsigned char *m,int sx,int sy)
{
	setScatterMaskData(m);
}
//----------------------------
RawDataSet	*Registration::TransformObjectToFinalVolume(RawDataSet *finalData)
{



	if((finalData!=NULL)&&(ObjectData!=NULL))
	{
		int i,j,k;

		PrepareVariables(0);

		Transform *transX=new Transform(RotateCenter,1,1,1,oneScale);

		transX->MakeCompleteAffineMatrixInv(Matrix,TransTemp);



		//RawDataSet	*FinalData = new RawDataSet(ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz(),ModelData->GetXscale(),ModelData->GetYscale(),ModelData->GetZscale(),0);
		//FinalData->ZeroQ();

		//FinalData->SetRawDataSetName(ObjectData->GetRawDataSetName()+"-TO-"+ModelData->GetRawDataSetName());

		unsigned short *TempPointer=finalData->GetDataArray();


		double *Point1;




		transX->SetTransformationSteps(	finalData->GetXscale(),finalData->GetYscale(),finalData->GetZscale(),
			ObjectData->GetXscale(),ObjectData->GetYscale(),ObjectData->GetZscale(),
			ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz());
		Point1=transX->FinalPoint;


		double t1=(double)(ObjectData->GetNx()-1);
		double t2=(double)(ObjectData->GetNy()-1);
		double t3=(double)(ObjectData->GetNz()-1);


		////////////////////////////////////
		XStep_x=transX->SX1;	XStep_y=transX->SY1;	XStep_z=transX->SZ1;
		YStep_x=transX->SX2;	YStep_y=transX->SY2;	YStep_z=transX->SZ2;
		ZStep_x=transX->SX3;	ZStep_y=transX->SY3;	ZStep_z=transX->SZ3;
		XBack_x=transX->SXB1;	XBack_y=transX->SYB1;	XBack_z=transX->SZB1;
		YBack_x=transX->SXB2;	YBack_y=transX->SYB2;	YBack_z=transX->SZB2;
		ZBack_x=transX->SXB3;	ZBack_y=transX->SYB3;	ZBack_z=transX->SZB3;
		ZeroYNextZ_x=transX->SYZB1;		ZeroYNextZ_y=transX->SYZB2;		ZeroYNextZ_z=transX->SYZB3;
		ZeroXNextY_x=transX->SXYB1;		ZeroXNextY_y=transX->SXYB2;		ZeroXNextY_z=transX->SXYB3;
		//////////////////////////////////////



		if (t3<=0) t3=1; //takie tam
		int valtmp;

		if (Interpolate!=NULL) delete Interpolate;
		if (ModelData->GetNz()==1)
		{
			Interpolate=new InterpolationFunction2DBilinear();
		}
		else
		{
			Interpolate=new InterpolationFunctionTrilinear();
		}

		for (k=0;k<finalData->GetNz();k++)
		{		
			for (j=0;j<finalData->GetNy();j++)
			{
				for (i=0;i<finalData->GetNx();i++)
				{	
					if ((Point1[0]>=0.0)&&(Point1[0]<t1)&&
						(Point1[1]>=0.0)&&(Point1[1]<t2)&&
						(Point1[2]>=0.0)&&(Point1[2]<t3))
					{
						valtmp=(int)(Interpolate->GetInterpolatedValue(DataPadded,Point1[0],Point1[1],Point1[2]));

						if (valtmp>0) *(TempPointer)=valtmp;
						//Interpolation3Linear(DataPadded,Point1[0],Point1[1],Point1[2]);
					}
					TempPointer++;	
					//transX->SetNextXCoordinate();
					Point1[0]+=XStep_x;
					Point1[1]+=XStep_y;
					Point1[2]+=XStep_z;
				}
				//transX->SetZeroXNextYCoordinate();
				Point1[0]+=ZeroXNextY_x;
				Point1[1]+=ZeroXNextY_y;
				Point1[2]+=ZeroXNextY_z;
			}
			//transX->SetZeroYNextZCoordinate();
			Point1[0]+=ZeroYNextZ_x;
			Point1[1]+=ZeroYNextZ_y;
			Point1[2]+=ZeroYNextZ_z;

		}


		UnprepareVariables(0);

		delete transX;
		//		delete Point1;

	}
	return NULL;

}

//----------------------------------------------------------
void	Registration::TransformObjectToModelVolume(int value,RawDataSet *d,RawDataSet *obj)
{

	//	FILE *Do_opa;

	if((ModelData!=NULL)&&(ObjectData!=NULL))
	{
		int i,j,k;

		PrepareVariables(0);
		int tvalue;

		Transform *transX=new Transform(RotateCenter,1,1,1,oneScale);

		transX->MakeCompleteAffineMatrixInv(Matrix,FinalTrans);

		RawDataSet	*FinalData;
		if (d==NULL)	
			FinalData = ModelData;
		else
			FinalData=d;
		//FinalData->ZeroQ();

		//FinalData->SetRawDataSetName(ObjectData->GetRawDataSetName()+"-TO-"+ModelData->GetRawDataSetName());

		unsigned short *TempPointer=FinalData->GetDataArray();


		double *Point1;




		transX->SetTransformationSteps(	FinalData->GetXscale(),FinalData->GetYscale(),FinalData->GetZscale(),
			ObjectData->GetXscale(),ObjectData->GetYscale(),ObjectData->GetZscale(),
			ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz());
		Point1=transX->FinalPoint;

		//		Point1[0]+=1.0;
		//		Point1[1]+=1.0;
		//		Point1[2]+=1.0;
		//double tolX=1e-5;	

		double t1=(double)(ObjectData->GetNx()-1);
		double t2=(double)(ObjectData->GetNy()-1);
		double t3=(double)(ObjectData->GetNz()-1);


		////////////////////////////////////
		XStep_x=transX->SX1;	XStep_y=transX->SY1;	XStep_z=transX->SZ1;
		YStep_x=transX->SX2;	YStep_y=transX->SY2;	YStep_z=transX->SZ2;
		ZStep_x=transX->SX3;	ZStep_y=transX->SY3;	ZStep_z=transX->SZ3;
		XBack_x=transX->SXB1;	XBack_y=transX->SYB1;	XBack_z=transX->SZB1;
		YBack_x=transX->SXB2;	YBack_y=transX->SYB2;	YBack_z=transX->SZB2;
		ZBack_x=transX->SXB3;	ZBack_y=transX->SYB3;	ZBack_z=transX->SZB3;
		ZeroYNextZ_x=transX->SYZB1;		ZeroYNextZ_y=transX->SYZB2;		ZeroYNextZ_z=transX->SYZB3;
		ZeroXNextY_x=transX->SXYB1;		ZeroXNextY_y=transX->SXYB2;		ZeroXNextY_z=transX->SXYB3;
		//////////////////////////////////////
		double fvalue;
		RawDataSet *dp=DataPadded;
		unsigned short maxdp=dp->GetMaxValue();
		if (obj!=NULL)
		{
			dp=obj;
		}

		if (t3<=0) t3=1; //takie tam

		if (Interpolate!=NULL) delete Interpolate;
		if (ModelData->GetNz()==1)
		{
			Interpolate=new InterpolationFunction2DBilinear();
		}
		else
		{
			Interpolate=new InterpolationFunctionTrilinear();
		}

		for (k=0;k<FinalData->GetNz();k++)
		{		
			for (j=0;j<FinalData->GetNy();j++)
			{
				for (i=0;i<FinalData->GetNx();i++)
				{	
					if ((Point1[0]>=0.0)&&(Point1[0]<t1)&&
						(Point1[1]>=0.0)&&(Point1[1]<t2)&&
						(Point1[2]>=0.0)&&(Point1[2]<t3))
					{


						fvalue=(Interpolate->GetInterpolatedValue(dp,Point1[0],Point1[1],Point1[2]));

						if (value==-1)
						{
							*(TempPointer)=(i*j*k)%ModelData->GetMaxValue();
						}
						else if (value>0)
						{
							if ((fvalue>maxdp*0.9))
								*(TempPointer)=value;
						}
						else 
						{
							if ((fvalue>maxdp*0.9))
								*(TempPointer)=(unsigned short) fvalue;
						}



						//if (obj==NULL)
						{
							//							if ((fvalue>value*0.9))
							//							{
							//								if (value!=-1)
							//									*(TempPointer)=(unsigned short) fvalue;
							//								else 
							//									*(TempPointer)=value;
							//							}
							//							else
							//							{
							//						
							////*(TempPointer)=((double)rand()*ModelData->GetMaxValue()/2)/(double)RAND_MAX;
							//								//*(TempPointer)=ModelData->GetMaxValue()-tvalue>>2;
							//								//*(TempPointer)=(i*j*k)%ModelData->GetMaxValue();
							//
							//							}
						}
						/*else
						{
						if (fabs((double)(tvalue-value))<1.0) 
						*(TempPointer)=value;


						}*/

					}	//Interpolation3Linear(DataPadded,Point1[0],Point1[1],Point1[2]);

					TempPointer++;	
					//transX->SetNextXCoordinate();
					Point1[0]+=XStep_x;
					Point1[1]+=XStep_y;
					Point1[2]+=XStep_z;
				}
				//transX->SetZeroXNextYCoordinate();
				Point1[0]+=ZeroXNextY_x;
				Point1[1]+=ZeroXNextY_y;
				Point1[2]+=ZeroXNextY_z;
			}
			//transX->SetZeroYNextZCoordinate();
			Point1[0]+=ZeroYNextZ_x;
			Point1[1]+=ZeroYNextZ_y;
			Point1[2]+=ZeroYNextZ_z;

		}


		UnprepareVariables(0);

		delete transX;
		//		delete Point1;
		//	return	FinalData;
	}
	//return NULL;

}
//---------------------------
double Registration::getConstraintUpdate(double *t)
{
	//trans 3,4,5 *180
	//bool constraints;
	//	double constraintRanges[12];
	double val=0;
	double max=0.95;
	double delta;
	double f=0;
	double alpha=0.1;

	if (t[10]>2)
	{
		f=f;
	}
	if (constraints==true)
	{
		int i;
		for(i=0;i<12;i++)
		{
			delta =fabs((double)(trans2[i]-t[i+1]));
			val=delta/constraintRanges[i];
			if (val>max)
			{
				val-=max;
				f+=val*val;
			}

		}


	}
	return f*alpha;



	/*for (i=1;i<=NrOfParams;i++)
	{

	RealTransform[ParamTrans[i]]=OptimTransform[i];
	}*/
}

//---------------------------
void Registration::setConstraints(double *c)
{

	if (c!=NULL)
	{
		constraints=true;
		int i;
		for(i=0;i<12;i++)
		{
			constraintRanges[i]=c[i];
		}

	}
}

//---------------------------
void Registration::SlotSetParalellComputing(bool p,int nr)
{
	this->parallelComputing=p;
	this->numberOfThreadsUsed=nr;

}
//----------------
//void Registration::SlotSetNrOfParallelThreads(int nr)
//{
//	numberOfThreadsUsed=nr;
//}