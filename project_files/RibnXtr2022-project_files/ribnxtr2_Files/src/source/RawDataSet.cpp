
#include "RawDataSet.h"
//Added by qt3to4:
//#include <QTextStream>

RawDataSet::RawDataSet(int _n[3], double _scale[3], double _origin[3], double gantry , bool Allocate )
{
	ini(_n[0], _n[1], _n[2], _scale[0], _scale[1], _scale[2],  gantry,  Allocate);
	origin[0] = _origin[0];origin[1] = _origin[1];origin[2] = _origin[2];
}
RawDataSet::RawDataSet(	int	N1,	int	N2,	int	N3,	double dx, double	dy,	double dz,double gantry,bool Allocate)
{
	ini(	N1, 	N2, 	N3,  dx, 	dy,  dz,  gantry,  Allocate);

}
void RawDataSet::ini(int N1, int N2, int N3, double dx , double dy , double dz , double gantry, bool Allocate)
{
	groupName = "";
	RawDataSetPath = "";
	origin[0] = 0;	origin[1] = 0;	origin[2] = 0;
	Nx = N1;  Ny = N2;	Nz = N3;

	Xscale = dx;Yscale = dy;Zscale = dz;

	SliceSize = Nx * Ny;
	GantryAngle = gantry;
	VolumeSize = SliceSize * Nz;
	MinValue = INT_MAX;MaxValue = 0;
	HistMax = 0;HistMin = INT_MAX;
	HistogramCalculated = false;
	RawDataSetName = "";
	HistogramData = NULL;

	FirstHistValue = 0;
	LastHistValue = 0;

	qArray = NULL; //26.11.2003 -	no i znowu nie mam pojecia czemu to	qrwa dzialalo!
	if (Allocate == true)	qArray = new unsigned	short[VolumeSize];


	TestEndian();

	RGBExists = false;

	RChannelArray = NULL;
	GChannelArray = NULL;
	BChannelArray = NULL;
	BChannelUCHARArray = NULL;
	GChannelUCHARArray = NULL;

	dicomMinValue = 0;
	dicomWindowWidthValue = 4096;
	dicomMinValue = -100000;
	dicomRescaleValue = 1.0;
}
//--------------------------------------------------------------------
RawDataSet::RawDataSet(const RawDataSet	&Data)
{
	
	Copy(Data,true);

}
//---------------------------------------------------------------------
void	RawDataSet::initRGBArrays()
{
	if (RGBExists==false)
	{
		RChannelArray=this->GetDataArray();
		BChannelUCHARArray=new unsigned char [VolumeSize];
		memset(BChannelUCHARArray,0,VolumeSize);
		GChannelUCHARArray=new unsigned char [VolumeSize];
		memset(GChannelUCHARArray,0,VolumeSize);
		RGBExists=true;
		Mode=4;

	}
	


}
//-------------------------------------------------------------------------------

void RawDataSet::Copy(const	RawDataSet &Data,bool copydata)
{
	groupName = Data.groupName;
	RawDataSetPath="";
	origin[0]=Data.origin[0];	origin[1]=Data.origin[1];	origin[2]=Data.origin[2];
	Nx = Data.Nx;  Ny =	Data.Ny;  Nz = Data.Nz;
	Xscale=Data.Xscale;Yscale=Data.Yscale;Zscale=Data.Zscale;

	SliceSize  =Data.SliceSize;
	GantryAngle=Data.GantryAngle;
	VolumeSize =Data.VolumeSize;
	HistogramCalculated=false;
	HistogramData=NULL;


	RawDataSetName=Data.RawDataSetName;

	TestEndian();

	if (copydata==true)
	{
		//TestEndian();
		if (Data.qArray!=NULL)
		{
			qArray=new unsigned	short[VolumeSize];
			memcpy(qArray,Data.qArray,VolumeSize*sizeof(unsigned short));
		}


		if (Data.HistogramData!=NULL)
		{	
			FirstHistValue=Data.FirstHistValue;
			LastHistValue=Data.LastHistValue;
			MinValue=Data.MinValue;MaxValue=Data.MaxValue;
			HistMax=Data.HistMax;HistMin=Data.HistMin;
			HistogramCalculated=Data.HistogramCalculated;
			HistogramData=new int [MaxValue+1];
			memcpy(HistogramData,Data.HistogramData, (MaxValue)*sizeof(int));

		}

	}
	else
	{
		MinValue=INT_MAX;MaxValue=0;
		HistMax=0;HistMin=INT_MAX;

		FirstHistValue=0;
		LastHistValue=0;

		qArray=new unsigned	short[VolumeSize];
		HistogramCalculated=false;
		HistogramData=NULL;
	}


	dicomMinValue = Data.dicomMinValue;
	dicomLevelValue = Data.dicomLevelValue;
	dicomWindowWidthValue = Data.dicomWindowWidthValue;
	dicomRescaleValue = Data.dicomRescaleValue;

	RGBExists=false;
	RChannelArray=NULL;
	GChannelArray=NULL;
	BChannelArray=NULL;
	BChannelUCHARArray=NULL;
	GChannelUCHARArray=NULL;
}
//---------------------------------------------------
RawDataSet::RawDataSet()
{
	RawDataSetPath="";
	origin[0]=0;	origin[1]=0;	origin[2]=0;
	qArray=NULL;
	Nx = 0;	 Ny	= 0;  Nz = 0;
	Xscale=0;Yscale=0;Zscale=0;
	SliceSize  = 0;
	GantryAngle=0;
	VolumeSize = 0;
	MinValue=INT_MAX;MaxValue=0;
	FirstHistValue=0;
	LastHistValue=0;
	HistMax=0;HistMin=INT_MAX;
	HistogramData=NULL;
	RGBExists=false;
	RChannelArray=NULL;
	GChannelArray=NULL;
	BChannelArray=NULL;
	BChannelUCHARArray=NULL;
	GChannelUCHARArray=NULL;

	dicomMinValue = 0;
	dicomWindowWidthValue = 4096;
	dicomMinValue = -100000;
	dicomRescaleValue = 1.0;
}
//-----------------------------------------------------------------------
RawDataSet::~RawDataSet(void)
{

	if (qArray!=NULL) delete []qArray;
	if (HistogramData!=NULL) delete	[]HistogramData;
	//26.11.2003
	qArray=NULL;
	HistogramData=NULL;
	///

	Nx = 0;	 Ny	= 0;  Nz = 0;
	Xscale=0;Yscale=0;Zscale=0;
	SliceSize  = 0;
	GantryAngle=0;
	VolumeSize = 0;
	MinValue=0;MaxValue=0;
	FirstHistValue=0;
	LastHistValue=0;


	//if (RChannelArray!=NULL)			delete []RChannelArray;
	if (GChannelArray!=NULL)			delete []GChannelArray;
	if (BChannelArray!=NULL)			delete []BChannelArray;
	if (BChannelUCHARArray!=NULL)	delete []BChannelUCHARArray;
	if (GChannelUCHARArray!=NULL)	delete []GChannelUCHARArray;


	//26.11.2003
	RChannelArray=NULL;
	GChannelArray=NULL;
	BChannelArray=NULL;
	BChannelUCHARArray=NULL;
	GChannelUCHARArray=NULL;
	////

}
//--------------------------------------------------------------------------------
/*
int	RawDataSet::GetVoxelPos(int	x, int y, int z	)
{
return z*SliceSize + y*Nx +	x;
}
//--------------------------------------------------------------------------------

unsigned short RawDataSet::GetVoxelValue(int x,	int	y, int z)
{
return *(qArray	+ z*SliceSize +	y*Nx + x);
}




//--------------------------------------------------------------------------------

unsigned short RawDataSet::GetVoxelValueFromZeroValue(int x, int y,	int	z)
{
return *(qArray	+ z*SliceSize +	y*Nx + x)-FirstHistValue;
}

//--------------------------------------------------------------------------------
*/
void RawDataSet::SetVoxelValue(int x, int y, int z,	unsigned short value)
{
	*(qArray + z*SliceSize + y*Nx +	x) = value;
}


//--------------------------------------------------------------------------------
void	RawDataSet::FindCircularMinMax(int *minMax)
{
	if (minMax != NULL)
	{
		if (qArray != NULL)
		{
			int min = INT_MAX;
			int max = -INT_MAX;


			int rx = (Nx - 4) / 2;
			int ry = (Ny - 4) / 2;
			int centerX = (Nx / 2);
			int centerY = (Ny / 2);
			int pos = 0;
			double tol = 1e-3;
			double x, y;
			int sizex2 = rx*rx;
			int sizey2 = ry*ry;
			for (int k = 0; k < Nz; k++)
			{
				for (int j = 0; j < Ny; j++)
				{
					y = j - centerY;

					for (int i = 0; i < Nx; i++)
					{
						x = i - centerX;
						if (((x*x) / sizex2 + (y*y) / sizey2 - 1 < tol))
						{
							if (max < qArray[pos])	max = qArray[pos];
							if (min > qArray[pos]) min = qArray[pos];
							//qArray[pos] = (MaxValue - MinValue) / 2;// (i*j) % (MaxValue - 1);
						}
						pos++;
					}
				}
			}

			minMax[0] = min;
			minMax[1] = max;
			//LastHistValue = MaxValue;
			//if (MinValue == MaxValue) MaxValue++;
		}
	}
}
//;---------------------------
void RawDataSet::FindMinMaxValue()
{
	if (qArray!=NULL)
	{
		MinValue=MaxValue=qArray[0];

		for(int	i=0;i<VolumeSize;i++)
		{
			if (MaxValue<qArray[i])	MaxValue=qArray[i];
			if	(MinValue>qArray[i]) MinValue=qArray[i];
		}

		LastHistValue=MaxValue;	
		if (MinValue==MaxValue) MaxValue++;
	}
}

//--------------------------------------------------------------------------------

void RawDataSet::ConvertFromSignedData(bool clear,int minVal)
{

	signed short	*Q2=(signed	short *) qArray;

	MinValue=MaxValue=Q2[0];

	int	i;
	for( i=0;i<VolumeSize;i++)
	{
		//if (MaxValue<Q2[i]) MaxValue=Q2[i];
		//else if 
		if	(MinValue>Q2[i]) MinValue=Q2[i];
	}

	
	if (MinValue<0)
	if (clear==true)	
	{
		for	(i = 0;	i <	VolumeSize;	i++)  
			qArray[i]=(Q2[i]<0)?0:Q2[i];
			//qArray[i]=(Q2[i]-minVal<0)?0:Q2[i]-minVal;
	}
	else
	{
		for	(i = 0;	i <	VolumeSize;	i++)  
			qArray[i]=Q2[i]-minVal-MinValue;
	}

	FindMinMaxValue();	
}


//--------------------------------------------------------------------------------

void RawDataSet::SwapData()
{
	for	(int i=0;i<VolumeSize;i++)
		swap(qArray+i);
}

//--------------------------------------------------------------------------------

void RawDataSet::swap(unsigned short *a)
{
	/*union	tag
	{
	unsigned short word;
	unsigned char  byte[2];
	} tag;

	unsigned char b;

	tag.word=*a;
	b=tag.byte[0];
	tag.byte[0]=tag.byte[1];
	tag.byte[1]=b;

	*a=tag.word;
	*/
	*a=(((*a)<<8)|((*a)>>8));

}

//--------------------------------------------------------------------------------
unsigned short RawDataSet::swap(unsigned short a)
{
	union tag
	{
		unsigned short word;
		unsigned char  byte[2];
	} tag;

	unsigned char b;

	tag.word=a;
	b=tag.byte[0];
	tag.byte[0]=tag.byte[1];
	tag.byte[1]=b;

	return tag.word;
}

//--------------------------------------------------------------------------------

void RawDataSet::ZeroQ()
{

	memset(qArray, 0, VolumeSize * sizeof(unsigned short));


}


//--------------------------------------------------------------------------------
bool	RawDataSet::SaveRGBRawDataSet(const QString &filename)
{
	if (!filename.isEmpty()) 
	{
		if (RGBExists==true)
		{
			//const char *name1;	
			//const char *name2;
			//QString	filenameR,filename2;	
			QString temp;
			QString tt;
			FILE *plik;

			tt=filename;
			if(tt.mid(tt.length()-4,4)==".dat")
				tt.remove(tt.length()-4,4);
			//zapisac czerwony
			temp=tt+".dat";
			SaveRawDataSet(	temp,MachineEndian,qArray);
			// zapisac zielony

			if (GChannelArray!=NULL)
			{
				temp=tt+".dat.UCHARG";
				//SaveRawDataSet(	temp,MachineEndian,GChannelArray);
				unsigned char *tempQ=new unsigned char[VolumeSize];
				int i;
				for (i=0;i< VolumeSize;i++)
					*(tempQ+i)=(char)*(GChannelArray+i);    
				//const char *name1;	
				FILE *plik;
				//name1=temp.toLocal8Bit();
				plik = fopen(temp.toLocal8Bit(), "wb");
				fwrite( tempQ,sizeof(unsigned char),VolumeSize,plik);
				fclose(plik);
				delete []tempQ;

			}
			else if (GChannelUCHARArray!=NULL)
			{
				temp=tt+".dat.UCHARG";
				SaveRawDataSet(	temp,MachineEndian,GChannelUCHARArray);


			}
			//zapisac niebieski
			if (BChannelArray!=NULL)
			{
				temp=tt+".dat.UCHARB";
				//SaveRawDataSet(	temp,MachineEndian,BChannelArray);
				//temp=tt+".dat.UCHARG";
				//SaveRawDataSet(	temp,MachineEndian,GChannelArray);
				unsigned char *tempQ=new unsigned char[VolumeSize];
				int i;
				for (i=0;i< VolumeSize;i++)
					*(tempQ+i)=(char)*(BChannelArray+i);    
				//const char *name1;	
				FILE *plik;
				//name1=temp.toLocal8Bit();
				plik = fopen(temp.toLocal8Bit(), "wb");
				fwrite( tempQ,sizeof(unsigned char),VolumeSize,plik);
				fclose(plik);
				delete []tempQ;

			}
			else if (BChannelUCHARArray!=NULL)
			{
				temp=tt+".dat.UCHARB";
				SaveRawDataSet(	temp,MachineEndian,BChannelUCHARArray);


			}

		}

	}
	return false;
}
//--------------------------------------------------------------------------------
bool	RawDataSet::SaveRGBDataAs3DataSets(const QString &filename)
{

	if (!filename.isEmpty()) 
	{
		if (RGBExists==true)
		{
		//	const char *name1;	
			//const char *name2;
			//QString	filenameR,filename2;	
			QString temp;
			QString tt;
			FILE *plik;

			tt=filename;
			if(tt.mid(tt.length()-4,4)==".dat")
				tt.remove(tt.length()-4,4);
			//zapisac czerwony
			temp=tt+".dat";
			SaveRawDataSet(	temp,MachineEndian,qArray);
			// zapisac zielony
 
			if (GChannelArray!=NULL)
			{
				temp=tt+".dat.G";
				SaveRawDataSet(	temp,MachineEndian,GChannelArray);

			}
			else if (GChannelUCHARArray!=NULL)
			{
				temp=tt+".dat.UCHARG";
				SaveRawDataSet(	temp,MachineEndian,GChannelUCHARArray);


			}
			//zapisac niebieski
			if (BChannelArray!=NULL)
			{
				temp=tt+".dat.B";
				SaveRawDataSet(	temp,MachineEndian,BChannelArray);

			}
			else if (BChannelUCHARArray!=NULL)
			{
				temp=tt+".dat.UCHARB";
				SaveRawDataSet(	temp,MachineEndian,BChannelUCHARArray);


			}

		}

	}
	return false;

}
//---------------------------------------------------------------------------------
bool	RawDataSet::SaveRawDataSet(	const QString &filename	,int Endian,unsigned char *tQ)
{
	if (!filename.isEmpty()) 
	{
		//const char *name1;	
		//const char *name2;
		QString	filename1,filename2;	
		FILE *plik;

		filename2=filename;
		if (filename2.lastIndexOf(".dat")==-1)
			filename2+=".dat";
		//name1=filename2.toLocal8Bit();
		filename1=filename2+".header";
		//name2=filename1.toLocal8Bit();		

		if (Endian==2) Endian=MachineEndian;

		plik = fopen(filename1.toLocal8Bit(), "w");
		fprintf(plik,"%d %d	\n%lf% f	%lf\n%d %lf",Nx,Ny,Xscale,Yscale,Zscale,Endian,GantryAngle);
		fclose(plik);


		plik = fopen(filename2.toLocal8Bit(), "wb");

		unsigned char *tempQ=new unsigned char[VolumeSize];

		int	i;	
		for	(i=0;i<	VolumeSize;i++)
			*(tempQ+i)=*(tQ+i);	 
		fwrite(	tempQ,sizeof(unsigned char),VolumeSize,plik);

		fclose(plik);
		delete []tempQ;


		return true;


	}
	else
	{
		return false;
	}


}
//---------------------------------------------------------------------------------
bool	RawDataSet::SaveRawDataSet(	const QString &filename	,int Endian,unsigned short *tQ)
{
	if (!filename.isEmpty()) 
	{
		//const char *name1;	
		//const char *name2;
		QString	filename1,filename2;	
		FILE *plik;

		filename2=filename;
		if (filename2.lastIndexOf(".dat")==-1)
			filename2+=".dat";
		//name1=filename2.toLocal8Bit();
		filename1=filename2+".header";
		//name2=filename1.toLocal8Bit();		

		if (Endian==2) Endian=MachineEndian;

		plik = fopen(filename1.toLocal8Bit(), "w");
		fprintf(plik,"%d %d	\n%lf %lf	%lf\n%d %lf",Nx,Ny,Xscale,Yscale,Zscale,Endian,GantryAngle);
		fclose(plik);


		plik = fopen(filename2.toLocal8Bit(), "wb");

		if (MachineEndian!=(bool)Endian)
		{

			unsigned short *tempQ=new unsigned short[VolumeSize];
			int	i;	
			for	(i=0;i<	VolumeSize;i++)
				*(tempQ+i)=swap(*(tQ+i));	 

			fwrite(	tempQ,sizeof(unsigned short),VolumeSize,plik);
			delete []tempQ;
		}
		else
		{
			fwrite(	tQ,sizeof(unsigned short),VolumeSize,plik);
		}

		fclose(plik);


		return true;


	}
	else
	{
		return false;
	}


}
//--------------------------------------------------------------------
bool	RawDataSet::SaveRawDataSet(	const QString &filename	,int Endian)
{
	if (!filename.isEmpty()) 
	{
		//const char *name1;	
		//const char *name2;
		QString	filename1,filename2;	
		FILE *plik;

		filename2=filename;
		if (filename2.lastIndexOf(".dat")==-1)
			filename2+=".dat";
		//name1=filename2.toLocal8Bit();
		filename1=filename2+".header";
		//name2=filename1.toLocal8Bit();		

		if (Endian==2) Endian=MachineEndian;

		plik = fopen(filename1.toLocal8Bit(), "w");
		fprintf(plik,"%d %d	\n%lf %lf	%lf\n%d %lf",Nx,Ny,Xscale,Yscale,Zscale,Endian,GantryAngle);
		fclose(plik);


		plik = fopen(filename2.toLocal8Bit(), "wb");

		if (MachineEndian!=(bool)Endian)
		{

			unsigned short *tempQ=new unsigned short[VolumeSize];
			int	i;	
			for	(i=0;i<	VolumeSize;i++)
				*(tempQ+i)=swap(*(qArray+i));	 

			fwrite(	tempQ,sizeof(unsigned short),VolumeSize,plik);
			delete []tempQ;
		}
		else
		{
			fwrite(	qArray,sizeof(unsigned short),VolumeSize,plik);
		}

		fclose(plik);


		return true;


	}
	else
	{
		return false;
	}


}


//--------------------------------------------------------------------------------
bool	RawDataSet::LoadRawDataSet(int n[3],double s[3],int depth,const QString &filename, int maxVal , int skip,double *th,bool logData)
{
#pragma omp parallel
nrOfThreads = omp_get_num_threads();


	if (!filename.isEmpty()) 
	{
		RawDataSetPath=filename;
		QString filename1,filename2;
		filename2=filename;
		if (filename2.lastIndexOf(".raw")==-1)
			filename2+=".raw";

		if (n[0]==0 || n[1]==0) return false;

		QFile f(filename);
		if ( f.open( QIODevice::ReadOnly ) ) 
		{
			//f.read(( char*) qArray,VolumeSize*sizeof(unsigned short));
			//f.close();
			double* minT = new double[nrOfThreads];//maxy tyle watkow zakladam
			double* maxT = new double[nrOfThreads];
			

		//if (n[2]==0)
		{
			//estimate size
			//n[2]=f.size()/(n[0]*n[1]);
			switch (depth)
			{
				case 8: //bit
				
				break;
					//do nothing
				case 16: //16bit

					n[2]/=2;
				break ;

				case 24: //24bit
					n[2]/=3;
				break;


				case 32: //32bit
					n[2]/=4;
				break; 

				case 320: //32bit
					//n[2] /= 4;
					break;
				default:

				n[2]/=2; //16bit
			}
		}

		if (n[2]==0) return false;

		Nx=n[0];
		Ny=n[1];
		Nz=n[2];
		Xscale=s[0];
		Yscale=s[1];
		Zscale=s[2];

		HistogramCalculated=false;
		SliceSize  = Nx	* Ny;
		VolumeSize = SliceSize * Nz;
		MinValue=0;
		MaxValue=0;
	

		qArray=new unsigned	short[VolumeSize];
		double minF = INT_MAX;
		double maxF = -INT_MAX;
		unsigned char *tab=NULL;

		float* ftab = NULL;
		float* ftabSkip;
		double rangeF;
		double scaleFactor;
		double level, window;
		int pos;

		int maxSize = 32767;
		if (maxVal > 0) maxSize = maxVal;

		switch (depth)
		{
				case 8: //bit

					tab=new unsigned char [VolumeSize];
					f.read(( char*) tab,VolumeSize);

					for(int i=0;i<VolumeSize;i++)
					{
						qArray[i]=tab[i];
					}
					delete []tab;
					tab=NULL;
				
				break;
					//do nothing
				case 16: //16bit

					f.read(( char*) qArray,VolumeSize*sizeof(unsigned short));
				break ;

				case 24: //24bit //tylko red
					 tab=new unsigned char [VolumeSize*3];
					f.read(( char*) tab,VolumeSize);

					if (logData == false)
					{
#pragma omp  for  nowait ordered schedule (guided)
						for (int i = 0;i < VolumeSize;i++)
						{
							qArray[i] = tab[i * 3];
						}
					}
					else
					{
#pragma omp  for  nowait ordered schedule (guided)
						for (int i = 0;i < VolumeSize;i++)
						{
							qArray[i] = (tab[i*3]>0)?log(tab[i * 3]):0;
						}
					}
					delete []tab;
					tab=NULL;
				break;


				case 32: //32bit //tylko red
					tab=new unsigned char [VolumeSize*4];
					f.read(( char*) tab,VolumeSize);

					if (logData == false)
					{
#pragma omp  for  nowait ordered schedule (guided)
						for (int i = 0;i < VolumeSize;i++)
						{
							qArray[i] = tab[i * 4];
						}
					}
					else
					{
#pragma omp  for  nowait ordered schedule (guided)
						for (int i = 0;i < VolumeSize;i++)
						{
							qArray[i] = qArray[i] = (tab[i * 4] > 0) ? log(tab[i * 4]) : 0;
						}
					}
					delete []tab;
					tab=NULL;
				break; 

				case 320: //32bit float
					ftab = new float[VolumeSize + skip];
					ftabSkip = ftab + skip;
					//tab = new unsigned char[VolumeSize * 4];
					f.read((char*)ftab, (VolumeSize+skip) * sizeof(float));
#pragma omp parallel
					{
						//wczytuje do tablicy float tmp - znajduje max min - skaluje dodaje dicom data i lece
						int tid = omp_get_thread_num();
						
						if (logData == true)
						{
#pragma omp  for  nowait ordered schedule (guided)
							for (int i = 0; i < VolumeSize; i++)
							{
								
								ftabSkip[i] = (ftabSkip[i]>0)?log(ftabSkip[i]):0;
							}
						}

						//find min Max

						for (int i = 0;i < nrOfThreads;i++)
						{

							minT[i] = ftabSkip[0];
							maxT[i] = ftabSkip[0];
						}

						pos = 0;
#pragma omp  for  nowait ordered schedule (guided)
						for (int i = 0; i < VolumeSize; i++)
						{
							float val = ftabSkip[i ];

							if ((th != NULL) && ((val < th[0]) || (val > th[1])))
							{
								if (val < th[0]) val = th[0];
								if (val > th[1]) val = th[1];
							}
								if (val >1000)
								{
									val = val;
								}

							if (val < minT[tid])	minT[tid] = val;
							if (val > maxT[tid])	maxT[tid] = val;
							//pos++;

							//qArray[i] = tab[i * 4];
						}
					}

					for (int i = 0;i < nrOfThreads;i++)
					{
						if (minT[i]<minF)minF=minT[i];
						if (maxT[i]>maxF)maxF=maxT[i];
					}
			

					 rangeF = (maxF - minF);
					if (rangeF == 0) rangeF = 1;

					
					 scaleFactor = (double)maxSize / rangeF;

					  pos = 0;
					
					 
//#pragma omp  for  nowait ordered schedule (guided)		
#pragma omp  for  nowait ordered schedule (guided)
					 for (int i = 0; i < VolumeSize; i++)
					 { 
						float val = ftabSkip[i ];

						if (th != NULL)
						{
							if (val < th[0])val = minF;
							if (val > th[1])val = maxF;
						}
						qArray[i] = (val - minF) * scaleFactor;
						/*if (qArray[i] > 50000)
						{
							int b = 1;
						}*/
						
					}



					 level = 0.5 * (maxF + minF);// 0.5*(dMax + dMin);
					 window = rangeF;//(dMax - dMin);

					dicomLevelValue = level;
					dicomWindowWidthValue = window;

					dicomMinValue = minF;
					//scale = scale / 16384.0;
					dicomRescaleValue = 1.0/ scaleFactor;


					delete[]ftab;
					tab = NULL;
					break;

				default:

				f.read(( char*) qArray,VolumeSize*sizeof(unsigned short));
			}


		/*QFile file2(filename2);
		if ( file2.open( QIODevice::ReadOnly ) ) 
		{
			file2.read(( char*) qArray,VolumeSize*sizeof(unsigned short));
			file2.close();
		}
		else
		{
			return false;
		}*/
		
		QString	tt;
		tt=filename;

		if(tt.mid(tt.length()-6,6)==".rdata")
			tt.remove(tt.length()-6,6);


		RawDataSetName=tt;

			pos=RawDataSetName.lastIndexOf("/");
		if (pos>=0)
			RawDataSetName.remove(0,pos+1);


		CalculateHistogram();

		f.close();

		delete[]minT;
		delete[]maxT;

		return true;




		}
		return false;

	}
	return false;

}

bool	RawDataSet::LoadRawDataSet2(const QString &filename)
{

	if (!filename.isEmpty()) 
	{

		RawDataSetPath=filename;
		QString filename1,filename2;
		filename2=filename;
		if (filename2.lastIndexOf(".rdata")==-1)
			filename2+=".rdata";
	
		filename1=filename2+".header";
		int count=0;
		QString filename3 = filename2 + ".float";

	//stream << "SIZES:\n" << Nx << Ny << Nz << "\n";
	//		stream << "VOXEL_DIMS:\n" << Xscale << Yscale << Zscale << "\n";
	//		stream << "START_POS:\n" << origin[0] << origin[1] << origin[2] << "\n";
		QFile file(filename1);
	
		if ( file.open( QIODevice::ReadOnly ) ) 
		{
			QTextStream stream( &file );

			QString line;//=stream.readLine();
			QStringList fields;// = line.split( " ");

			do 
			{
				line=stream.readLine();
				if (QString::compare(line,QString("SIZES:"))==0)
				{
					line=stream.readLine();
					fields=line.split(" ", QString::SkipEmptyParts);
					count=fields.count();
					Nx=fields[0].toInt();
					Ny=fields[1].toInt();
					Nz=fields[2].toInt();

					if (Nx*Ny*Nz==0) return false;

				}
				else
					if (QString::compare(line,QString("VOXEL_DIMS:"))==0)
				{
					line=stream.readLine();
					fields=line.split(" ", QString::SkipEmptyParts);
					Xscale=fields[0].toDouble();
					Yscale=fields[1].toDouble();
					Zscale=fields[2].toDouble();

					if (Xscale*Yscale*Zscale ==0) return false;
				}
				else
				if (QString::compare(line, QString("START_POS:")) == 0)
				{
					line = stream.readLine();
					fields = line.split(" ", QString::SkipEmptyParts);
					origin[0] = fields[0].toDouble();
					origin[1] = fields[1].toDouble();
					origin[2] = fields[2].toDouble();
				}
				else
				if (QString::compare(line, QString("DICOM_THRESHOLDS:")) == 0)
				{
					line = stream.readLine();
					fields = line.split(" ", QString::SkipEmptyParts);
					//this->dicomLeftThreshold = fields[0].toInt();
					//this->dicomRightThreshold = fields[1].toInt();

					dicomMinValue = fields[0].toDouble();
					dicomLevelValue = fields[1].toDouble();
					dicomWindowWidthValue = fields[2].toDouble();
					if (fields.count() > 3)
					{
						dicomRescaleValue = fields[3].toDouble();
					}

				}


			}while	( !stream.atEnd() );

			/*if (QString::compare(line,QString("SIZES:"))==0)
			{
				do
				{
					line=stream.readLine();
					fields.split(" ",line
					if (!(QString::compare(line,QString(""))==0))
					{
						veinStringTab[veinNr++]=line;
					}
				}while	(( !stream.atEnd() )&&(QString::compare(line,QString("symetric:"))!=0));

				if (QString::compare(line,QString("symetric:"))==0)
				{
					veinNr--;
					do
					{
					line=stream.readLine();
					if (!(QString::compare(line,QString(""))==0))
					{
						veinStringTab[veinNr++]=line+"_left";
						veinStringTab[veinNr++]=line+"_right";

					}
					}while	( !stream.atEnd() );
				}
			}*/
		}


		

		
		HistogramCalculated=false;
		SliceSize  = Nx	* Ny;
		VolumeSize = SliceSize * Nz;
		MinValue=0;
		MaxValue=0;
		//	QMessageBox::about(	0,"DICOM",QString::number(Mode));

		//if ((_Nx!=0)&&((Nx!=_Nx)||(Ny!=_Ny)||(Nz!=_Nz))) return	false;
		QFile file3(filename3);
		int question = 0;
		if (file3.open(QIODevice::ReadOnly))
		{

			QMessageBox msgBox;
			QPushButton *bitButton[2];
			bitButton[0] = msgBox.addButton(QObject::tr("load ushort"), QMessageBox::ActionRole);;
			bitButton[1] = msgBox.addButton(QObject::tr("load fload"), QMessageBox::ActionRole);;
		
			msgBox.setText("Float data available:");
			msgBox.exec();



			if (msgBox.clickedButton() == bitButton[0]) { question = 0; }
			else
				if (msgBox.clickedButton() == bitButton[1]) { question = 1; }

			file3.close();
				
		}

		if (question == 0)
		{
			qArray = new unsigned	short[VolumeSize];
			//fread( qArray,sizeof(unsigned short),VolumeSize,plik);
			//fclose(plik);
			QFile file2(filename2);
			if (file2.open(QIODevice::ReadOnly))
			{
				file2.read((char*)qArray, VolumeSize * sizeof(unsigned short));
				file2.close();
			}
			else
			{
				return false;
			}

			

		}
		else
		{
			if (file3.open(QIODevice::ReadOnly))
			{
				float *tab = new float[VolumeSize];
				file3.read((char*)tab, VolumeSize * sizeof(float));
				double minValue = INT_MAX;
				double maxValue = -INT_MAX;

				for (int i = 0; i < VolumeSize; i++)
				{

					if (tab[i] < minValue) minValue = tab[i];
					if (tab[i] > maxValue) maxValue = tab[i];
				}
				qArray = new unsigned	short[VolumeSize];
				
				
				long double scale = (maxValue - minValue);
				if (scale > 0) scale = 16384.0 / scale;

				for (int i = 0; i < VolumeSize; i++)
				{
					qArray[i] = (tab[i] - minValue)*scale;
				}
				double level = 0.5*(maxValue + minValue);// 0.5*(dMax + dMin);
				double window = maxValue - minValue;//(dMax - dMin);

				dicomLevelValue = level;
				dicomWindowWidthValue = window;

				dicomMinValue = minValue;
				 scale = scale / 16384.0;
				dicomRescaleValue = scale;
				delete[]tab;
				file3.close();
			}
			else
			{
				return false;
			}

		}
		QString	tt;
		tt = filename;

		if (tt.mid(tt.length() - 6, 6) == ".rdata")
			tt.remove(tt.length() - 6, 6);


		RawDataSetName = tt;

		int	pos = RawDataSetName.lastIndexOf("/");
		if (pos >= 0)
			RawDataSetName.remove(0, pos + 1);
		CalculateHistogram();
		return true;


	}


 return false;
}
//--------------------------------------------------------------------------------

bool	RawDataSet::SaveRawDataSet2(const QString &filename)
{
	if (!filename.isEmpty()) 
	{
		/*const char *name1;	
		const char *name2;
		QString	filename1,filename2;	
		FILE *plik;

		filename2=filename;
		if (filename2.lastIndexOf(".dat")==-1)
			filename2+=".dat";
		name1=filename2.toLocal8Bit();
		filename1=filename2+".header";
		name2=filename1.toLocal8Bit();		

		if (Endian==2) Endian=MachineEndian;

		plik=fopen(name2,"w");
		fprintf(plik,"%d %d	\n%lf %lf	%lf\n%d %lf",Nx,Ny,Xscale,Yscale,Zscale,Endian,GantryAngle);
		fclose(plik);


		plik=fopen(name1,"wb");

		if (MachineEndian!=(bool)Endian)
		{

			unsigned short *tempQ=new unsigned short[VolumeSize];
			int	i;	
			for	(i=0;i<	VolumeSize;i++)
				*(tempQ+i)=swap(*(qArray+i));	 

			fwrite(	tempQ,sizeof(unsigned short),VolumeSize,plik);
			delete []tempQ;
		}
		else
		{
			fwrite(	qArray,sizeof(unsigned short),VolumeSize,plik);
		}

		fclose(plik);*/

		
		QString	filename1,filename2;	
		FILE *plik;

		filename2=filename;
		if (filename2.lastIndexOf(".rdata")==-1)
			filename2+=".rdata";
		filename1=filename2+".header";

		RawDataSetPath=filename2;

		
		QFile file( filename1);
		if ( file.open( QIODevice::WriteOnly ) ) 
		{
			QTextStream stream( &file );
			stream << "SIZES:\n" << Nx <<" "<< Ny <<" "<< Nz << "\n\n";
			stream << "VOXEL_DIMS:\n" << Xscale <<" "<< Yscale <<" "<< Zscale << "\n\n";
			stream << "START_POS:\n" << origin[0] <<" "<< origin[1] <<" "<< origin[2] << "\n\n";
		
			if (dicomMinValue > -100000)
			{
				//stream << "DICOM_THRESHOLDS:\n" << this->dicomLeftThreshold <<" " << this->dicomRightThreshold << "\n\n";
				stream << "DICOM_THRESHOLDS:\n" << this->dicomMinValue << " " << this->dicomLevelValue << " " << this->dicomWindowWidthValue <<  " " << this->dicomRescaleValue; "\n\n";
			}
			
			//for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
			//	stream << *it << "\n";
			file.close();
		}
		else
		{
			return false;
		}

		QFile file2(filename2);
		if ( file2.open( QIODevice::WriteOnly ) ) 
		{

			file2.write((const char *)qArray,VolumeSize*sizeof(unsigned short));
			file2.close();
		}
		else
		{
			return false;
		}
		

		return true;


	}
	else
	{
		return false;
	}


}
//--------------------------------------------------------------------------------
bool	RawDataSet::LoadRawDataSet(	const QString &filename	,int _Nx,int _Ny, int _Nz )
{


	if(!filename.isEmpty())
	{

		if (qArray!=NULL) delete []qArray;

		HistogramCalculated=false;


		//const char *name1;	
		//const char *name2;
		QString	filename1,filename2;	
		FILE *plik;


		filename2=filename;
		if (filename2.lastIndexOf(".dat")==-1)
			filename2+=".dat";
		//name1=filename2.toLocal8Bit();
		filename1=filename2+".header";
		//name2=filename1.toLocal8Bit();	



		plik = fopen(filename1.toLocal8Bit(), "rt");
		if (plik==NULL)	return false;	
		fscanf(plik,"%d	%d %lf %lf %lf	%d %lf",&Nx,&Ny,&Xscale,&Yscale,&Zscale,&FileEndian,&GantryAngle);

		int	TMode;
		Mode=((fscanf(plik,"%d",&TMode))!=EOF) ?TMode:0;
		fclose(plik);


		if ((Nx<=0)||(Ny<=0)||(Xscale<=0)||(Yscale<=0)||(Zscale<=0)) return	false;


		if (!((FileEndian==0)||(FileEndian==1))) FileEndian=0;

		int	 length;

		QFile f1( filename2	);
		f1.open( QIODevice::Unbuffered	| QIODevice::ReadOnly );
		length=f1.size();
		f1.close();

		/*plik=fopen(name1,"rb");
		if (plik==NULL)	return false;

		fseek(plik,	0L,	SEEK_END);
		length = ftell(plik);
		fclose(plik);
		*/
		Nz=length/(sizeof(unsigned short)*Nx*Ny);		

		if (Nz<=0) return false;

		SliceSize  = Nx	* Ny;
		VolumeSize = SliceSize * Nz;
		MinValue=0;
		MaxValue=0;
		//	QMessageBox::about(	0,"DICOM",QString::number(Mode));

		if ((_Nx!=0)&&((Nx!=_Nx)||(Ny!=_Ny)||(Nz!=_Nz))) return	false;



		plik = fopen(filename2.toLocal8Bit(), "rb");

		qArray=new unsigned	short[VolumeSize];
		fread( qArray,sizeof(unsigned short),VolumeSize,plik);
		fclose(plik);




		TestEndian();
		if (MachineEndian!=FileEndian)
			SwapData();


		//Wstaw	jako nazwe nazwe pliku
		QString	tt;
		tt=filename;

		if(tt.mid(tt.length()-4,4)==".dat")
			tt.remove(tt.length()-4,4);


		RawDataSetName=tt;

		int	pos=RawDataSetName.lastIndexOf("/");
		if (pos>=0)
			RawDataSetName.remove(0,pos+1);


		CalculateHistogram();

		if (Mode==3)
		{

			QString	filenameG,filenameB;

			filenameG=filename2+".G";
			filenameB=filename2+".B";

			//name1=filenameG.toLocal8Bit();
			//name2=filenameB.toLocal8Bit();
			//test
			plik = fopen(filenameG.toLocal8Bit(), "rb");
			if (plik==NULL)	return false;
			fclose(plik);

			plik = fopen(filenameB.toLocal8Bit(), "rb");
			if (plik==NULL)	return false;
			fclose(plik);
			//koniec testu

			plik = fopen(filenameG.toLocal8Bit(), "rb");
			GChannelArray=new unsigned short[VolumeSize];
			fread( GChannelArray,sizeof(unsigned short),VolumeSize,plik);
			fclose(plik);

			plik = fopen(filenameB.toLocal8Bit(), "rb");
			BChannelArray=new unsigned short[VolumeSize];
			fread( BChannelArray,sizeof(unsigned short),VolumeSize,plik);
			fclose(plik);

			RChannelArray=qArray;

			RGBExists=true;



		}
		else if	(Mode==4)
		{
			//BGetGChannelArray()UCHAR

			QString	filenameG,filenameB;

			filenameG=filename2+".UCHARG";
			filenameB=filename2+".UCHARB";

			//name1=filenameG.toLocal8Bit();
			//name2=filenameB.toLocal8Bit();
			//test
			plik = fopen(filenameG.toLocal8Bit(), "rb");
			if (plik==NULL)	return false;
			fclose(plik);

			plik = fopen(filenameB.toLocal8Bit(), "rb");
			if (plik==NULL)	return false;
			fclose(plik);
			//koniec testu

			plik = fopen(filenameG.toLocal8Bit(), "rb");
			GChannelUCHARArray=new unsigned	char[VolumeSize];
			fread( GChannelUCHARArray,sizeof(unsigned char),VolumeSize,plik);
			fclose(plik);

			plik = fopen(filenameB.toLocal8Bit(), "rb");
			BChannelUCHARArray=new unsigned	char[VolumeSize];
			fread( BChannelUCHARArray,sizeof(unsigned char),VolumeSize,plik);
			fclose(plik);

			RChannelArray=qArray;

			RGBExists=true;
		}





		return true;


	}
	else
	{
		return false;
	}


}

//--------------------------------------------------------------------------------
void	RawDataSet::TestEndian()
{
	// testing endians
	union	tag
	{
		unsigned short word;
		unsigned char  byte[2];
	}	tag;

	tag.word=1;
	if (tag.byte[1]==1) MachineEndian=1;	//big endian machine
	else MachineEndian=0;					//little endian	machine
}


//--------------------------------------------------------------------------------

void	RawDataSet::CalculateHistogram()
{


	//if ((MaxValue==0)) 
		FindMinMaxValue();
	if (HistogramData!=NULL) delete	[]HistogramData;
	HistogramData=new int [MaxValue+1];

	memset(HistogramData, 0, (MaxValue+1)*sizeof(int));
	int	i;

	for	( i=0;i<VolumeSize;i++)
		HistogramData[*(qArray+i)]++;

	//HistMax=HistogramData[0];
	for	(i=0;i<=MaxValue;i++)
		if (HistogramData[i]>HistMax) HistMax=HistogramData[i];
		else if	(HistMin>HistogramData[i]) HistMin=HistogramData[i];



		for	(i=0;i<=MaxValue;i++)
			if (HistogramData[i]>0)	break;

		FirstHistValue=i;
		HistogramCalculated=true;

		

}

//------------------------------------------------------------

void	RawDataSet::SetRawDataSetName(const	QString	&name)
{

	RawDataSetName=name;

}


//------------------------------------------------------------

bool	RawDataSet::CompareWithOtherDataSet(RawDataSet *Data2)
{
	if (Data2==NULL) return false;
	return((Data2->GetNx()==Nx)&&(Data2->GetNy()==Ny)&&(Data2->GetNz()==Nz)
		&&(Data2->GetXscale()==Xscale)&&(Data2->GetYscale()==Yscale)&&(Data2->GetZscale()==Zscale))?1:0;

}
//---------------------------------------
bool	RawDataSet::SavePngSlices(	const QString &filename)
{
	if ((!filename.isEmpty())&&(GChannelUCHARArray!=NULL)&& (BChannelUCHARArray!=NULL))
	{
		QString name;
		QString number;
		QString	tt;
		tt=filename;

		if(tt.mid(tt.length()-4,4)==".png")
			tt.remove(tt.length()-4,4);

		QImage Image(Nx, Ny, QImage::Format_ARGB32);
		unsigned char *pixeldata=Image.bits();//new unsigned char[s*4];
		unsigned short *r=this->GetDataArray();
		unsigned char *g=this->GChannelUCHARArray;
		unsigned char *b=this->BChannelUCHARArray;
		int i,j;
	
		int pos=0;
		int step1;
		for(j=0;j<Nz;j++)
		{	
			step1=0;
			for(i=0;i<SliceSize;i++)
			{
				pixeldata[step1++]=b[pos];
				pixeldata[step1++]=g[pos];
				pixeldata[step1++]=r[pos];
				step1++;
				pos++;
			}

			number="";
			if(j<100000) number+="0";
			if(j<10000) number+="0";
			if(j<1000) number+="0";
			if(j<100) number+="0";
			if(j<10) number+="0";
			number+=QString::number(j);
			Image.save(tt+number+".png","PNG");
	
		}
		return true;
	}
	return false;
}
//----------
void	RawDataSet::GetVoxelXYZPos(int pos, int p[3])
{
		p[2]=(int) (pos/SliceSize);
		p[1]=(int) ((pos-p[2]*SliceSize)/Nx);
		p[0]=(int)	(pos-p[2]*SliceSize)-Nx*p[1];
}
//----------
void	RawDataSet::GetVoxelRealPos(int pos, double p[3])
{


	p[2] = (int)(pos / SliceSize);
	p[1] = (int)((pos - p[2] * SliceSize) / Nx);
	p[0] = (int)(pos - p[2] * SliceSize) - Nx*p[1];


	p[0] = origin[0] + p[0] * Xscale;
	p[1] = origin[1] + p[1] * Yscale;
	p[2] = origin[2] + p[2] * Zscale;

}