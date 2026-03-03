#include "AnisotropicDiffusion.h"

AnisotropicDiffusion::AnisotropicDiffusion(RawDataSet *InputData,LinkedList<DataSet*>	*_DataSetList)
{

	Data=InputData;
	DataList=_DataSetList;
	ConductivityFuncs[0]=&AnisotropicDiffusion::C1;
	ConductivityFuncs[1]=&AnisotropicDiffusion::C2;
	ConductivityFuncs[2]=&AnisotropicDiffusion::C3;
	ConductivityFuncs[3]=&AnisotropicDiffusion::C4;
	ConductivityFuncs[4]=&AnisotropicDiffusion::C5;

}
//---------------------------------------------
AnisotropicDiffusion::~AnisotropicDiffusion()
{
	Data=NULL;
}
//---------------------------------------------
/*void AnisotropicDiffusion::ComputeGradient3D(int x, int y, int z)
{
}
*/
//---------------------------------------------
void AnisotropicDiffusion::FilterDataSet3D(RawDataSet *FinalData,int iter,int cnr,double k0,double a,double dt)
{

	if (Data->CompareWithOtherDataSet(FinalData)==true)
	{


		double coef=0.1/6.0;// lambda=0.1, k=6 (6 neighbors)
		
		double (AnisotropicDiffusion::*func)(double,double);

		if (cnr<0 ) cnr=0; else if (cnr>4) cnr=4;
		func=(this->ConductivityFuncs[cnr]);


		//wyczyszcze na poczatek bo tam bede przechowywal wyniki kolejnych iteracji
		//FinalData->ZeroQ();

		memcpy(FinalData->GetDataArray(),Data->GetDataArray(),Data->GetVolumeSize()*sizeof(unsigned short));

		//trzeba zrobic kopie 

		RawDataSet RData=*Data;
		RawDataSet *TempData=&RData;

		int nx=Data->GetNx();
		int ny=Data->GetNy();
		int nz=Data->GetNz();


		unsigned short *TempDataTab,*FinalDataTab;
		TempDataTab=TempData->GetDataArray();
		FinalDataTab=FinalData->GetDataArray();
		int pos;

		int sx=Data->GetNx();
		int ssize=Data->GetSliceSize();
		//turbo
		// stablicuje sobie fkcje 

		int maxvalue=Data->GetMaxValue();


		double *tab=new double[2*maxvalue];

		double proggress=0;
		double dproggress=100.0/(nz*iter);

		int l;
		for (l=0;l<iter;l++)
		{
			pos=0;
			for (int i=0;i<2*maxvalue;i++)
			{
				tab[i]=(this->*func)((double)i,k0);
			}

			// parallel_for(0,nz, [&](int k) 
			for (int k=0;k<nz;k++)
			{
				int i,j;
				double tmp;
				int grad;
				int val0;
				int nr;
				int pos=k*nx*ny;
				for(j=0;j<ny;j++)
					for(i=0;i<nx;i++)
					{
						nr=0;
						tmp=0;

						//val0=TempData->GetVoxelValue(i,j,k);
						val0=TempDataTab[pos];

						if (i<nx-1)
						{	//x+1
							//grad=TempData->GetVoxelValue(i+1,j,k)-val0;
							grad=TempDataTab[pos+1]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (i >0)
						{	//x-1
							grad=TempDataTab[pos-1]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (j<ny-1)
						{	//y+1
							//grad=TempData->GetVoxelValue(i,j+1,k)-val0;
							grad=TempDataTab[pos+sx]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (j>0)
						{	//y-1
							//grad=TempData->GetVoxelValue(i,j-1,k)-val0;
							grad=TempDataTab[pos-sx]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (k<nz-1)
						{	//z+1
							//grad=TempData->GetVoxelValue(i,j,k+1)-val0;
							grad=TempDataTab[pos+ssize]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (k>0)
						{	//z-1
							//grad=TempData->GetVoxelValue(i,j,k-1)-val0;
							grad=TempDataTab[pos-ssize]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}

						if (nr!=0) coef=1.0/nr;
						tmp*=coef;
						//val0=FinalData->GetVoxelValue(i,j,k)+tmp;
						val0=(int)(FinalDataTab[pos]+tmp);

						if (val0<0) val0=0; else if (val0>65535) val0=65535;

						//FinalData->SetVoxelValue(i,j,k,val0);
						FinalDataTab[pos]=val0;
						pos++;


					}
					proggress+=dproggress;
					emit SignalSendProgress((int)proggress);
			};

			k0+=a*dt;
			if (l<iter-1)
				TempData->MakeCopyOfDataArray(FinalData->GetDataArray());
		}


		TempData=NULL;
		delete []tab;
		tab=NULL;

	}

}
//----------------------------------------------------
void AnisotropicDiffusion::FilterDataSet4D(RawDataSet *FinalData,int iter,int cnr,double k0,double a,double dt)
{
	/*if (DataList!=NULL)
	if (Data->CompareWithOtherDataSet(FinalData)==true)
	{

	int i,j,k,l;
	double tmp;
	int grad;
	int val0;
	double coef;

	//RawDataSet *prevData=NULL;
	//RawDataSet *nextData=NULL;
	unsigned short *prevDataTab=NULL;
	unsigned short *nextDataTab=NULL;
	RawDataSet *tempRawData;
	//----------------------------------

	int DataSetNr=DataList->GetNumberOfItems();	
	for (j=0;j<DataSetNr;j++)
	{
	tempRawData=DataList->GetItemAtPos(j)->GetData()->GetRawData();
	if (Data==tempRawData)
	break;
	}

	if (j>0)
	if (Data->CompareWithOtherDataSet(DataList->GetItemAtPos(j-1)->GetData()->GetRawData()))
	prevDataTab=DataList->GetItemAtPos(j-1)->GetData()->GetRawData()->GetDataArray();

	if (j<DataSetNr-1)
	if (Data->CompareWithOtherDataSet(DataList->GetItemAtPos(j+1)->GetData()->GetRawData()))
	nextDataTab=DataList->GetItemAtPos(j+1)->GetData()->GetRawData()->GetDataArray();

	//----------------------------------

	int nr;
	double (AnisotropicDiffusion::*func)(double,double);
	if (cnr<0 ) cnr=0; else if (cnr>4) cnr=4;
	func=(this->ConductivityFuncs[cnr]);
	//wyczyszcze na poczatek bo tam bede przechowywal wyniki kolejnych iteracji
	//FinalData->ZeroQ();

	memcpy(FinalData->GetDataArray(),Data->GetDataArray(),Data->GetVolumeSize()*sizeof(unsigned short));

	//trzeba zrobic kopie 

	RawDataSet RData=*Data;
	RawDataSet *TempData=&RData;

	int nx=Data->GetNx();
	int ny=Data->GetNy();
	int nz=Data->GetNz();


	unsigned short *TempDataTab,*FinalDataTab;
	TempDataTab=TempData->GetDataArray();
	FinalDataTab=FinalData->GetDataArray();
	int pos;

	int sx=Data->GetNx();
	int ssize=Data->GetSliceSize();
	//turbo
	// stablicuje sobie fkcje 

	int maxvalue=Data->GetMaxValue();


	double *tab=new double[2*maxvalue];

	double proggress=0;
	double dproggress=100.0/(nz*iter);


	for (l=0;l<iter;l++)
	{
	pos=0;
	for (i=0;i<2*maxvalue;i++)
	{
	tab[i]=(this->*func)((double)i,k0);
	}

	for (k=0;k<nz;k++)
	{
	for(j=0;j<ny;j++)
	for(i=0;i<nx;i++)
	{
	nr=0;
	tmp=0;

	val0=TempDataTab[pos];

	if (i<nx-1)
	{	//x+1
	grad=TempDataTab[pos+1]-val0;
	tmp+=grad*tab[abs(grad)];
	nr++;
	}
	if (i >0)
	{	//x-1
	grad=TempDataTab[pos-1]-val0;
	tmp+=grad*tab[abs(grad)];
	nr++;
	}
	if (j<ny)
	{	//y+1
	grad=TempDataTab[pos+sx]-val0;
	tmp+=grad*tab[abs(grad)];
	nr++;
	}
	if (j>0)
	{	//y-1
	grad=TempDataTab[pos-sx]-val0;
	tmp+=grad*tab[abs(grad)];
	nr++;
	}
	if (k<nz-1)
	{	//z+1
	grad=TempDataTab[pos+ssize]-val0;
	tmp+=grad*tab[abs(grad)];
	nr++;
	}
	if (k>0)
	{	//z-1								
	grad=TempDataTab[pos-ssize]-val0;
	tmp+=grad*tab[abs(grad)];
	nr++;
	}

	//time components
	if (prevDataTab!=NULL)
	{	//t-1
	grad=prevDataTab[pos]-val0;
	tmp+=grad*tab[abs(grad)];
	nr++;
	}

	if (nextDataTab!=NULL)
	{	//t+1
	grad=nextDataTab[pos]-val0;
	tmp+=grad*tab[abs(grad)];
	nr++;
	}
	/////////////////////////
	if (nr!=0) coef=1.0/nr;
	tmp*=coef;
	val0=(int)(FinalDataTab[pos]+tmp);
	if (val0<0) val0=0; else if (val0>65535) val0=65535;
	FinalDataTab[pos]=val0;
	pos++;

	}
	proggress+=dproggress;
	emit SignalSendProgress((int)proggress);
	}

	k0+=a*dt;
	/*	if (l<iter-1)
	TempData->MakeCopyOfDataArray(FinalData->GetDataArray());

	QString Nr=(l+1<10)?"0"+QString::number(l+1):QString::number(l+1);
	TempData->SaveRawDataSet("E:\\"+Nr);*/
	/*	}

	TempData=NULL;
	prevDataTab=NULL;
	nextDataTab=NULL;
	delete []tab;
	tab=NULL;

	}*/
	if (Data->CompareWithOtherDataSet(FinalData)==true)
	{

	
		double coef=0.1/6.0;// lambda=0.1, k=6 (6 neighbors)

		double (AnisotropicDiffusion::*func)(double,double);

		if (cnr<0 ) cnr=0; else if (cnr>4) cnr=4;
		func=(this->ConductivityFuncs[cnr]);


		//wyczyszcze na poczatek bo tam bede przechowywal wyniki kolejnych iteracji
		//FinalData->ZeroQ();

		memcpy(FinalData->GetDataArray(),Data->GetDataArray(),Data->GetVolumeSize()*sizeof(unsigned short));

		//trzeba zrobic kopie 
		unsigned short *prevDataTab=NULL;
		unsigned short *nextDataTab=NULL;
		RawDataSet *tempRawData ;
		RawDataSet RData=*Data;
		RawDataSet *TempData=&RData;
		//----------------------------------

		int DataSetNr=DataList->GetNumberOfItems();	
		
		int j;
		for (j=0;j<DataSetNr;j++)
		{
			tempRawData=DataList->GetItemAtPos(j)->GetData()->GetRawData();
			if (Data==tempRawData)
				break;
		}

		if (j>0)
		{
			if (Data->CompareWithOtherDataSet(DataList->GetItemAtPos(j-1)->GetData()->GetRawData()))
				prevDataTab=DataList->GetItemAtPos(j-1)->GetData()->GetRawData()->GetDataArray();
		}		
		else if (Data->CompareWithOtherDataSet(DataList->GetItemAtPos(DataSetNr-1)->GetData()->GetRawData()))
			prevDataTab=DataList->GetItemAtPos(DataSetNr-1)->GetData()->GetRawData()->GetDataArray();

		if (j<DataSetNr-1)
		{
			if (Data->CompareWithOtherDataSet(DataList->GetItemAtPos(j+1)->GetData()->GetRawData()))
				nextDataTab=DataList->GetItemAtPos(0)->GetData()->GetRawData()->GetDataArray();
		}		
		else if (Data->CompareWithOtherDataSet(DataList->GetItemAtPos(j+1)->GetData()->GetRawData()))
			nextDataTab=DataList->GetItemAtPos(0)->GetData()->GetRawData()->GetDataArray();


		//----------------------------------
		int nx=Data->GetNx();
		int ny=Data->GetNy();
		int nz=Data->GetNz();


		unsigned short *TempDataTab,*FinalDataTab;
		TempDataTab=TempData->GetDataArray();
		FinalDataTab=FinalData->GetDataArray();
//		int pos;

		int sx=Data->GetNx();
		int ssize=Data->GetSliceSize();
		//turbo
		// stablicuje sobie fkcje 

		int maxvalue=Data->GetMaxValue();


		double *tab=new double[2*maxvalue];

		double proggress=0;
		double dproggress=100.0/(nz*iter);

		int l;
		for (l=0;l<iter;l++)
		{
			///pos=0;
			for (int i=0;i<2*maxvalue;i++)
			{
				tab[i]=(this->*func)((double)i,k0);
			}

			
			
			//parallel_for(0,nz, [&](int k) 
			for (int k=0;k<nz;k++)
			{
				int i,j;
				double tmp;
				int grad;
				int val0;
				int nr;
				int pos=k*ssize;
				for(j=0;j<ny;j++)
					for(i=0;i<nx;i++)
					{
						nr=0;
						tmp=0;

						//val0=TempData->GetVoxelValue(i,j,k);
						val0=TempDataTab[pos];

						if (i<nx-1)
						{	//x+1
							//grad=TempData->GetVoxelValue(i+1,j,k)-val0;
							grad=TempDataTab[pos+1]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (i >0)
						{	//x-1
							grad=TempDataTab[pos-1]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (j<ny)
						{	//y+1
							//grad=TempData->GetVoxelValue(i,j+1,k)-val0;
							grad=TempDataTab[pos+sx]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (j>0)
						{	//y-1
							//grad=TempData->GetVoxelValue(i,j-1,k)-val0;
							grad=TempDataTab[pos-sx]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (k<nz-1)
						{	//z+1
							//grad=TempData->GetVoxelValue(i,j,k+1)-val0;
							grad=TempDataTab[pos+ssize]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						if (k>0)
						{	//z-1
							//grad=TempData->GetVoxelValue(i,j,k-1)-val0;
							grad=TempDataTab[pos-ssize]-val0;
							//tmp+=grad*(this->*func)(fabs(grad),k0);
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						//--------------

						//time components
						if (prevDataTab!=NULL)
						{	//t-1
							grad=prevDataTab[pos]-val0;
							tmp+=grad*tab[abs(grad)];
							nr++;
						}

						if (nextDataTab!=NULL)
						{	//t+1
							grad=nextDataTab[pos]-val0;
							tmp+=grad*tab[abs(grad)];
							nr++;
						}
						/////////////////////////
						if (nr!=0) coef=1.0/nr;
						tmp*=coef;
						val0=(int)(FinalDataTab[pos]+tmp);
						if (val0<0) val0=0; else if (val0>65535) val0=65535;
						FinalDataTab[pos]=val0;
						pos++;
						//------------------



					}
					proggress+=dproggress;
					emit SignalSendProgress((int)proggress);
			};

			k0+=a*dt;
			if (l<iter-1)
				TempData->MakeCopyOfDataArray(FinalData->GetDataArray());
		}


		TempData=NULL;
		delete []tab;
		tab=NULL;

	}
}
