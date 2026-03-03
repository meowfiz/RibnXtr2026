#include "HeartSimulator.h"

HeartSimulator::HeartSimulator()
{

		rxx=0;
		ryy=0;
		stddevs=NULL;
	

}
//---------------------------------------------
HeartSimulator::~HeartSimulator()
{
	if (stddevs!=NULL)
	{
		delete []stddevs;
		stddevs=NULL;
	}
}
//---------------------------------------------

void HeartSimulator::GenerateDoubleEllipsoid(RawDataSet *data, double *center1,double *center2, double *radius1, double *radius2,int val,bool append)
{

	int nx=data->GetNx();
	int ny=data->GetNy();
	int nz=data->GetNz();

	int i,j,k;
	int tmp;

	unsigned short *tempQ=data->GetDataArray();
	if(append==false)	memset(tempQ, 0, data->GetVolumeSize());

	double sqRadius1[3],sqRadius2[3];

	sqRadius1[0]=radius1[0]*radius1[0];
	sqRadius1[1]=radius1[1]*radius1[1];
	sqRadius1[2]=radius1[2]*radius1[2];

	sqRadius2[0]=radius2[0]*radius2[0];
	sqRadius2[1]=radius2[1]*radius2[1];
	sqRadius2[2]=radius2[2]*radius2[2];

	int pos=0;
	for(k=0;k<nz;k++)
		for(j=0;j<ny;j++)
			for(i=0;i<nx;i++)
			{

				if ((
					(double)((i-center1[0])*(i-center1[0]))/sqRadius1[0]+
					(double)((j-center1[1])*(j-center1[1]))/sqRadius1[1]+
					(double)((k-center1[2])*(k-center1[2]))/sqRadius1[2]<1+1e-6)&&
					(
					(double)((i-center2[0])*(i-center2[0]))/sqRadius2[0]+
					(double)((j-center2[1])*(j-center2[1]))/sqRadius2[1]+
					(double)((k-center2[2])*(k-center2[2]))/sqRadius2[2]>1+1e-6))
					
				{

					tempQ[pos]=val;
				}
				pos++;
			}



}
//-----------------------------------------------------------------
void HeartSimulator::AddGaussianNoise(RawDataSet *data,double amount,double mean, double stddev)
{

	int nx=data->GetNx()-1;
	int ny=data->GetNy()-1;
	int nz=data->GetNz()-1;

	int size=data->GetVolumeSize()*amount;

	int i;
	int x,y,z;
	int ii;
	unsigned short tmp;
	int v;
	if (amount+0.0001<1)
	{
		//srand( (unsigned)time( NULL ) );
		int ii=-rand();
		for (i=0;i<size;i++)
		{
			x=((double)rand()*nx)/(double)RAND_MAX;
			y=((double)rand()*ny)/(double)RAND_MAX;
			z=((double)rand()*nz)/(double)RAND_MAX;
			
			tmp=data->GetVoxelValue(x,y,z);
			v=tmp+(mean+stddev*gasdev (&ii));
			if (v<0) v=0;
			if (v>65535) v=65535;
			data->SetVoxelValue(x,y,z,(unsigned short)v);
			
		}
	}

	else 
	{

		unsigned short *p=data->GetDataArray();
		unsigned short tmp;
		int v;
		for(i=0;i<data->GetVolumeSize();i++)
		{
			tmp=*p;
			v=tmp+(mean+stddev*gasdev (&ii));
			if (v<0) v=0;
			if (v>65535) v=65535;
			*(p++)=v;
		}
	}


}
//-----------------------------------------------------------------
void HeartSimulator::AddRandomSpheres(RawDataSet *data,double minSize, double maxSize, int nrOfSpheres,unsigned short valMin, unsigned short valMax)
{
	//srand( (unsigned)time( NULL ) );
	int nr=0;
	
	int nx=data->GetNx();
	int ny=data->GetNy();
	int nz=data->GetNz();

	int i,j,k;
	int tmp;


	double pos1[3];
	double pos2[3];


	double length;
	double angle1;
	double angle2;
	double d;
	double p[3];
	double dd[3];
	double dl;
	unsigned short value;
	int ip[3];
	double l2;
	do
	{
			pos1[0]=((double)rand()*nx)/(double)RAND_MAX;
			pos1[1]=((double)rand()*ny)/(double)RAND_MAX;
			pos1[2]=((double)rand()*nz)/(double)RAND_MAX;
			length=minSize+((double)rand()*(maxSize-minSize)/(double)RAND_MAX);
			value=valMin+((double)rand()*(valMax-valMin)/(double)RAND_MAX);
			l2=length*length;
			for(k=pos1[2]-length;k<pos1[2]+length;k++)
				for(j=pos1[1]-length;j<pos1[1]+length;j++)
					for(i=pos1[0]-length;i<pos1[0]+length;i++)
					{
						
						if ((double)((i-pos1[0])*(i-pos1[0]))+
							(double)((j-pos1[1])*(j-pos1[1]))+
							(double)((k-pos1[2])*(k-pos1[2]))<l2+1e-6)
						{
			
							if ((i>0)&&(i<nx-1)&&
								(j>0)&&(j<ny-1)&&
								(k>0)&&(k<nz-1))
							{

							

								if (data->GetVoxelValue(i,j,k)!=0)
								{
									data->SetVoxelValue(i,j,k,value);
								}
							}
						}
					}
					


			nr++;
	}while (nr<nrOfSpheres);
}
//------------------------------------------------------------------------
void	HeartSimulator::AddRegularSpheres(RawDataSet *data,int stepx,int stepy,int stepz,int size,unsigned short valMin, unsigned short valMax)
{
	//srand( (unsigned)time( NULL ) );
	
	
	int nx=data->GetNx();
	int ny=data->GetNy();
	int nz=data->GetNz();

	int i,j,k,x,y,z;
	int tmp;


	double pos1[3];
	double pos2[3];


	double length;
	double angle1;
	double angle2;
	double d;
	double p[3];
	double dd[3];
	double dl;
	unsigned short value;
	int ip[3];
	double l2;
	length=size;
	l2=size*size;
	
	for(z=stepz;z<nz-1;z+=stepz)
		for(y=stepy;y<ny-1;y+=stepy)
			for(x=stepx;x<nx-1;x+=stepx)
			{

				value=valMin+((double)rand()*(valMax-valMin)/(double)RAND_MAX);
				for(k=z-length;k<z+length;k++)
				for(j=y-length;j<y+length;j++)
					for(i=x-length;i<x+length;i++)
					{
						
						if ((double)((i-x)*(i-x))+
							(double)((j-y)*(j-y))+
							(double)((k-z)*(k-z))<l2+1e-6)
						{
			
							if ((i>0)&&(i<nx-1)&&
								(j>0)&&(j<ny-1)&&
								(k>0)&&(k<nz-1))
							{

							

								if (data->GetVoxelValue(i,j,k)!=0)
								{
									data->SetVoxelValue(i,j,k,value);
								}
							}
						}
					}


			}
			
	
	
	
	
	
}
//---------------------------------------------------------------------
void HeartSimulator::AddLines(RawDataSet *data,double minSize, double maxSize,double deltaAngle, int nrOfLines,unsigned short valMin, unsigned short valMax)
{

	//srand( (unsigned)time( NULL ) );
	int nr=0;
	
	int nx=data->GetNx();
	int ny=data->GetNy();
	int nz=data->GetNz();

	int i,j,k;
	int tmp;


	double pos1[3];
	double pos2[3];


	double length;
	double angle1;
	double angle2;
	double d;
	double p[3];
	double dd[3];
	double dl;
	unsigned short value;
	int ip[3];
	do
	{
	
		//znajdz poczatek punktu ktory znajduje sie w elipsoidzie
	//	do
	//	{
			pos1[0]=((double)rand()*nx)/(double)RAND_MAX;
			pos1[1]=((double)rand()*ny)/(double)RAND_MAX;
			pos1[2]=((double)rand()*nz)/(double)RAND_MAX;
	//	}while (data->GetVoxelValue((int)pos1[0],(int)pos1[1],(int)pos1[2])==0);

		// wylosuj dlugosc krechy

		length=minSize+((double)rand()*(maxSize-minSize)/(double)RAND_MAX);

		// wylosuj kat krechy

		angle1=((double)rand()*deltaAngle)/(double)RAND_MAX-deltaAngle/2+3.14159/2;
		angle2=((double)rand()*deltaAngle)/(double)RAND_MAX-deltaAngle/2+3.14159/2;
		

		pos2[0]=pos1[0]+length*cos(angle1)*cos(angle2);
		pos2[1]=pos1[1]+length*sin(angle1)*cos(angle2);
		pos2[2]=pos1[2]+length*sin(angle2);
		

		// narysuj kreche
		
		 d=1.0/3.0;
		
	
		double dl=(length>0)?1/length:1;
		dl*=d;


		dd[0]=(pos2[0]-pos1[0])*dl;
		dd[1]=(pos2[1]-pos1[1])*dl;
		dd[2]=(pos2[2]-pos1[2])*dl;

		p[0]=pos1[0];p[1]=pos1[1];p[2]=pos1[2];


		value=valMin+((double)rand()*(valMax-valMin)/(double)RAND_MAX);

		

		for(i=0;i<length*3;i++)
		{
			
			p[0]+=dd[0];
			p[1]+=dd[1];
			p[2]+=dd[2];

			
			ip[0]=(int)p[0];
			ip[1]=(int)p[1];
			ip[2]=(int)p[2];

			if ((ip[0]>0)&&(ip[0]<nx-1)&&
				(ip[1]>0)&&(ip[1]<ny-1)&&
				(ip[2]>0)&&(ip[2]<nz-1))
			{

				if ((ip[0]==69)&&(ip[1]== 100)&&(ip[2]== 128))
				{
					i=i;
				}

				if (data->GetVoxelValue(ip[0],ip[1],ip[2])!=0)
				{
					data->SetVoxelValue(ip[0],ip[1],ip[2],value);
				}
			}

		}


	nr++;
	}while (nr<nrOfLines);




}
//------------------------------------
void HeartSimulator::DefineStdDevs(double stddev, double range, int x,int y)
{
	rxx=x;
	ryy=y;
	if (stddevs!=NULL)
	{
		delete []stddevs;
		stddevs=NULL;
	}
	stddevs=new double[rxx*ryy];
	int i;
	//srand( (unsigned)time( NULL ) );
	for(i=0;i<rxx*ryy;i++)
	{
		stddevs[i]=stddev+((double)rand()*range)/(double)RAND_MAX-range/2;
	}
}
//----------------------------------------
void HeartSimulator::AddSpecialNoise(RawDataSet *data,int rx,int ry,double range,double mean, double stddev,bool changeDevs)
{
	double fact=1.3;

	int nx=data->GetNx()-1;
	int ny=data->GetNy()-1;
	int nz=data->GetNz()-1;

	

	int i,j,k;
	int x,y,z;
	int ii;
	unsigned short tmp;
	int v;

	
	//srand( (unsigned)time( NULL ) );

	int sx0,sy0;
	int sx1,sy1;
	int sizex,sizey;
	//double *stddevs=new double[rx*ry];

	if ((changeDevs==true)||(stddevs==NULL))
		DefineStdDevs( stddev,  range, rx,ry);
	/*

	*/
	//for(i=0;i<rx*ry;i++)
	//{
	//	stddevs[i]=stddev+((double)rand()*range)/32767.0-range/2;
	//}

	double fx=(double)nx*fact/nz;
	double fy=(double)ny*fact/nz;
	int poss=0;
	int xx,yy;
	for(j=0;j<ry;j++)
	{
		for(i=0;i<rx;i++)
		{
			for(k=0;k<=nz;k++)
			{
				sizex=fx*k;
				sizey=fy*k;

				sx0=(sizex*i)/rxx+(nx-sizex)/2.0;
				sy0=(sizey*j)/ryy+(ny-sizey)/2.0;
				sx1=(sizex*(i+1))/rxx+(nx-sizex)/2.0;
				sy1=(sizey*(j+1))/ryy+(ny-sizey)/2.0;


				for(yy=sy0;yy<=sy1;yy++)
					for(xx=sx0;xx<=sx1;xx++)
					{

						if ((xx>0)&&(xx<nx-1)&&(yy>0)&&(yy<ny-1)	)
						{
							//data->SetVoxelValue(xx,yy,k,poss*10);

							tmp=data->GetVoxelValue(xx,yy,k);
							v=tmp+(mean+stddevs[poss]*gasdev (&ii));
							if (v<0) v=0;
							if (v>65535) v=65535;
							data->SetVoxelValue(xx,yy,k,(unsigned short)v);

						}
					}





			}
			poss++;
		}
		
	}

	

}
//------------------------------------------------
void	HeartSimulator::TransVolume(RawDataSet *data, RawDataSet *data1,double *params)
{
	if (data->CompareWithOtherDataSet(data1)==true)
	{


		int i,j,k;
		int max=0;

		int nx=data->GetNx();
		int ny=data->GetNy();
		int nz=data->GetNz();
		double dx=data->GetXscale();
		double dy=data->GetYscale();
		double dz=data->GetZscale();


		double dx1=(dx>0)?1/dx:1;
		double dy1=(dy>0)?1/dy:1;
		double dz1=(dz>0)?1/dz:1;



///////////////////////////////////////////////////////////////////
		double RotateCenter[3]={nx*dx*0.5, ny*dy*0.5, nz*dz*0.5};



//	for (i=0;i<12;i++)
	//		fprintf(plik,"%lf\n",GUIPanel->TEdit[i]->text().toFloat());

		double	Matrix[16];
		/*double	params[14];
		

		for(i=0;i<3;i++)
			TransTemp[i+1]=GUIPanel->TEdit[i]->text().toFloat();
		for(i=3;i<6;i++)
			TransTemp[i+1]=GUIPanel->TEdit[i]->text().toFloat()*rad;
		for(i=6;i<13;i++)
			TransTemp[i+1]=GUIPanel->TEdit[i]->text().toFloat(); 
*/
		Transform *transX=new Transform(RotateCenter,1,1,1);

		transX->MakeCompleteAffineMatrixInv(Matrix,params);

		double zRot=params[6];

	//	RawDataSet	*FinalData = new RawDataSet(data->GetNx(),data->GetNy(),data->GetNz(),data->GetXscale(),data->GetYscale(),data->GetZscale(),0);
		data1->ZeroQ();
		//FinalData->SetRawDataSetName("xx");

		unsigned short *TempPointer=data1->GetDataArray();


		double Point1[3],Point[3];

		double t1=(double)(data->GetNx()-1);
		double t2=(double)(data->GetNy()-1);
		double t3=(double)(data->GetNz()-1);

	
		double t=params[14];
		double f;
		double c;

		for (k=0;k<data1->GetNz();k++)
		{		

			Point[2]=k*dz;
			for (j=0;j<data1->GetNy();j++)
			{
				Point[1]=j*dy;
				Point[0]=0;
				for (i=0;i<data1->GetNx();i++)
				{	
						
					//c=sqrt(		(Point[0]-RotateCenter[0])*(Point[0]-RotateCenter[0]) 
					//		+	(Point[1]-RotateCenter[1])*(Point[1]-RotateCenter[1]));
							//+	(Point[2]-RotateCenter[2])*(Point[2]-RotateCenter[2]));
					//if (c<0.000000001) c=1;
					//f=t/c+(Point[2]-RotateCenter[2])*t/100;
					f=(Point[2]-RotateCenter[2])*t/100;

					params[6]=zRot+f;

					transX->MakeCompleteAffineMatrixInv(Matrix,params);
					transX->MultiplyPoint(Point1,Point);
					Point1[0]*=dx1;Point1[1]*=dy1;Point1[2]*=dz1;
					
					if ((Point1[0]>=0)&&(Point1[0]<t1)&&
						(Point1[1]>=0)&&(Point1[1]<t2)&&
						(Point1[2]>=0)&&(Point1[2]<t3))
							*(TempPointer)=Interpolation3Linear(data,Point1[0],Point1[1],Point1[2]);
							//*(TempPointer)=ObjectData->GetVoxelValue((int)(PointF[0]),(int)(PointF[1]),(int)(PointF[2]));
					TempPointer++;		
					Point[0]+=dx;
				
				}
		
			}


		}

	}



}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
double    ran1 (int      *idum)
{
    static long ix1,
              ix2,
              ix3;
    static double r[98];
    double    temp;
    static int iff = 0;
    int       j;

    if (*idum < 0 || iff == 0) {
	iff = 1;
	ix1 = (IC1 - (*idum)) % M1;
	ix1 = (IA1 * ix1 + IC1) % M1;
	ix2 = ix1 % M2;
	ix1 = (IA1 * ix1 + IC1) % M1;
	ix3 = ix1 % M3;
	for (j = 1; j <= 97; j++) {
	    ix1 = (IA1 * ix1 + IC1) % M1;
	    ix2 = (IA2 * ix2 + IC2) % M2;
	    r[j] = (ix1 + ix2 * RM2) * RM1;
	}
	*idum = 1;
    }
    ix1 = (IA1 * ix1 + IC1) % M1;
    ix2 = (IA2 * ix2 + IC2) % M2;
    ix3 = (IA3 * ix3 + IC3) % M3;
    j = 1 + ((97 * ix3) / M3);
    temp = r[j];
    r[j] = (ix1 + ix2 * RM2) * RM1;
    return temp;
}
//-----------------------
double    gasdev (int      *idum)
{
    static int iset = 0;
    static double gset;
    double    fac,
              r,
              v1,
              v2;

    if (iset == 0) {
	do {
	    v1 = 2.0 * ran1 (idum) - 1.0;
	    v2 = 2.0 * ran1 (idum) - 1.0;
	    r = v1 * v1 + v2 * v2;
	} while (r >= 1.0);
	fac = sqrt (-2.0 * log (r) / r);
	gset = v1 * fac;
	iset = 1;
	return v2 * fac;
    } else {
	iset = 0;
	return gset;
    }
}