//pierNze podejscie
#include "DistanceMap.h"



//---------------------------------------------------------------------------------
DistanceMap::DistanceMap(vtkPoints *pts,double *Pos,
						 int *Size,double *Scale)
{
	is2D = (Size[2] > 1) ? false : true;
	maximumDistance = 0;
	sPoints=NULL;
	LThreshold=0;
	RThreshold=0;
	Mask=NULL;
	RawData=NULL;
	SurfaceDataList=NULL;;
	SurfaceData=NULL;;
	sPoints=pts;

	Xscale=Scale[0];
	Yscale=Scale[1];
	Zscale=Scale[2];
	Nx=Size[0];
	Ny=Size[1];
	Nz=Size[2];
	x0=Pos[0];
	y0=Pos[1];
	z0=Pos[2];

	//----------------------
	XSQscale=Xscale*Xscale;
	YSQscale=Yscale*Yscale;
	ZSQscale=Zscale*Zscale;

	Nxy=Nx*Ny;

	x0M=x0/Scale[0];
	y0M=y0/Scale[1];
	z0M=z0/Scale[2];

	N2x=Nx+2;
	N2y=Ny+2;
	N2z=Nz+2;

	N2xy=N2x*N2y;

	ArraySize=Nx*Ny*Nz;

	TempArraySize=N2x*N2y*N2z;

	XReal=Xscale*Nx;
	YReal=Yscale*Ny;
	ZReal=Zscale*Nz;

	EmptyInit=false;
	numberOfSeeds=0;

	infinity=(Nx>Ny)?Nx:Ny;
	infinity=(Nz>infinity)?Nz:infinity;

	DistanceVectorArrayX=NULL;
	DistanceVectorArrayY=NULL;
	DistanceVectorArrayZ=NULL;

	DistanceVectorArrayX=NULL;
	DistanceVectorArrayY=NULL;
	DistanceVectorArrayZ=NULL;


}
DistanceMap::DistanceMap(LinkedList<PolygonSurfaceData*> *_SurfaceDataList,double *Pos,
						 int *Size,double *Scale)
{
	maximumDistance = 0;
	is2D = (Size[2] > 1) ? false : true;
	sPoints=NULL;
	LThreshold=0;
	RThreshold=0;
	Mask=NULL;
	RawData=NULL;
	SurfaceDataList=_SurfaceDataList;
	SurfaceData=SurfaceDataList->GetCurrentListItem()->GetData();

	Xscale=Scale[0];
	Yscale=Scale[1];
	Zscale=Scale[2];
	Nx=Size[0];
	Ny=Size[1];
	Nz=Size[2];
	x0=Pos[0];
	y0=Pos[1];
	z0=Pos[2];

	//----------------------
	XSQscale=Xscale*Xscale;
	YSQscale=Yscale*Yscale;
	ZSQscale=Zscale*Zscale;

	Nxy=Nx*Ny;

	x0M=x0/Scale[0];
	y0M=y0/Scale[1];
	z0M=z0/Scale[2];

	N2x=Nx+2;
	N2y=Ny+2;
	N2z=Nz+2;

	N2xy=N2x*N2y;

	ArraySize=Nx*Ny*Nz;

	TempArraySize=N2x*N2y*N2z;

	XReal=Xscale*Nx;
	YReal=Yscale*Ny;
	ZReal=Zscale*Nz;

	EmptyInit=false;
	numberOfSeeds=0;

	infinity=(Nx>Ny)?Nx:Ny;
	infinity=(Nz>infinity)?Nz:infinity;

	DistanceVectorArrayX=NULL;
	DistanceVectorArrayY=NULL;
	DistanceVectorArrayZ=NULL;

	DistanceVectorArrayX=NULL;
	DistanceVectorArrayY=NULL;
	DistanceVectorArrayZ=NULL;


}

DistanceMap::DistanceMap(RawDataSet	*Data,int _LThreshold,int _RThreshold,unsigned char *_Mask,unsigned char *_clearMask)
{
	maximumDistance = 0;
	sPoints=NULL;
	clearMask=_clearMask;
	LThreshold=_LThreshold;
	RThreshold=_RThreshold;
	RawData=Data;
	Mask=_Mask;
	SurfaceDataList=NULL;
	SurfaceData=NULL;

	Xscale=Data->GetXscale();
	Yscale=Data->GetYscale();
	Zscale=Data->GetZscale();
	Nx=Data->GetNx();
	Ny=Data->GetNy();
	Nz=Data->GetNz();
	x0=0;
	y0=0;
	z0=0;

	//----------------------
	XSQscale=Xscale*Xscale;
	YSQscale=Yscale*Yscale;
	ZSQscale=Zscale*Zscale;

	Nxy=Nx*Ny;

	x0M=0;
	y0M=0;
	z0M=0;

	N2x=Nx+2;
	N2y=Ny+2;
	N2z=Nz+2;

	N2xy=N2x*N2y;

	ArraySize=Nx*Ny*Nz;

	TempArraySize=N2x*N2y*N2z;

	XReal=Xscale*Nx;
	YReal=Yscale*Ny;
	ZReal=Zscale*Nz;

	EmptyInit=false;
	numberOfSeeds=0;

	infinity=(Nx>Ny)?Nx:Ny;
	infinity=(Nz>infinity)?Nz:infinity;

	DistanceVectorArrayX=NULL;
	DistanceVectorArrayY=NULL;
	DistanceVectorArrayZ=NULL;

	DistanceVectorArrayX=NULL;
	DistanceVectorArrayY=NULL;
	DistanceVectorArrayZ=NULL;

}
//---------------------------------------------------------------------------------
DistanceMap::DistanceMap()
{
	sPoints=NULL;
	EmptyInit=true;
	infinity=10000;
}

//---------------------------------------------------------------------------------

DistanceMap::~DistanceMap()
{
	//Removing object arrays
	if (DistanceVectorArrayX!=NULL) 	delete[] DistanceVectorArrayX;
	if (DistanceVectorArrayY!=NULL) 	delete[] DistanceVectorArrayY;
	if (DistanceVectorArrayZ!=NULL) 	delete[] DistanceVectorArrayZ;

	DistanceVectorArrayX=NULL;
	DistanceVectorArrayY=NULL;
	DistanceVectorArrayZ=NULL;
	RawData=NULL;
	SurfaceDataList=NULL;
	Mask=NULL;


}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void DistanceMap::SetVoxelSize(double xs,double ys,double zs)
{
	// Change size of the voxel
	Xscale=xs;
	Yscale=ys;
	Zscale=zs;
	XSQscale=xs*xs;
	YSQscale=ys*ys;
	ZSQscale=zs*zs;

}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
void DistanceMap::CalculateForwardUp(void)
{

	int x,y,z;
	double a,b,c,d,min;

	//	FILE *Do_opa;

	//forward up pass of distance map calculation
	short *tX0,*tX1,*tX2,*tX3,*tX4;
	short *tY0,*tY1,*tY2,*tY3,*tY4;
	short *tZ0,*tZ1,*tZ2,*tZ3,*tZ4;
	int yT,zT;
	int t0,t1,t2,t3,t4;
	zT=0;
	yT=0;
	for (z=1;z<=Nz;z++)
	{
		zT+=N2xy;
		yT=0;
		for (y=1;y<=Ny;y++)
		{
			yT+=N2x;

			t0=zT		+	yT;			//(x,y,z)
			t1=t0		-	1;			//(x-1,y,z)
			t2=t0		-	N2x;		//(x,y-1,z)
			t3=t0		-	N2xy;		//(x-1,y,z-1)
			t4=t0		+	Nx+1;		//(x+1,y,z)

			tX0=TempDistanceVectorArrayX+t0;tX1=TempDistanceVectorArrayX+t1;tX2=TempDistanceVectorArrayX+t2;tX3=TempDistanceVectorArrayX+t3;tX4=TempDistanceVectorArrayX+t4;
			tY0=TempDistanceVectorArrayY+t0;tY1=TempDistanceVectorArrayY+t1;tY2=TempDistanceVectorArrayY+t2;tY3=TempDistanceVectorArrayY+t3;tY4=TempDistanceVectorArrayY+t4;
			tZ0=TempDistanceVectorArrayZ+t0;tZ1=TempDistanceVectorArrayZ+t1;tZ2=TempDistanceVectorArrayZ+t2;tZ3=TempDistanceVectorArrayZ+t3;tZ4=TempDistanceVectorArrayZ+t4;
			for(x=1;x<=Nx;x++)	/*right*/
			{

				tX0++;tX1++;tX2++;tX3++;
				tY0++;tY1++;tY2++;tY3++;
				tZ0++;tZ1++;tZ2++;tZ3++;

				a=sqtab_X[*tX0+infp1]	+	sqtab_Y[*tY0+infp1]	+	sqtab_Z[*tZ0+infp1];
				b=sqtabPX[*tX1+infp1]	+	sqtab_Y[*tY1+infp1]	+	sqtab_Z[*tZ1+infp1];
				c=sqtab_X[*tX2+infp1]	+	sqtabPY[*tY2+infp1]	+	sqtab_Z[*tZ2+infp1];
				d=sqtab_X[*tX3+infp1]	+	sqtab_Y[*tY3+infp1]	+	sqtabPZ[*tZ3+infp1];

				min=(a<b)?a:b;
				min=(c<min)?c:min;
				min=(d<min)?d:min;

				if (min==b)
				{
					*tX0=*tX1+1;
					*tY0=*tY1;
					*tZ0=*tZ1;
				}
				else if (min==c)
				{
					*tX0=*tX2;
					*tY0=*tY2+1;
					*tZ0=*tZ2;	
				}
				else if (min==d)
				{
					*tX0=*tX3;
					*tY0=*tY3;
					*tZ0=*tZ3+1;
				}
			}

			for( x=Nx;x>=1;x--)	
			{
				a=sqtab_X[*tX0+infp1]+sqtab_Y[*tY0+infp1]+sqtab_Z[*tZ0+infp1];
				b=sqtabMX[*tX4+infp1]+sqtab_Y[*tY4+infp1]+sqtab_Z[*tZ4+infp1];

				if (b<a)
				{
					*tX0=*tX4-1;
					*tY0=*tY4;
					*tZ0=*tZ4;	 
				}
				tX0--;tY0--;tZ0--;
				tX4--;tY4--;tZ4--;
			}
		}
	}
}

//---------------------------------------------------------------------------------

void DistanceMap::CalculateBackwardDown(void)
{
	double a,b,c,d,min;
	int x,y,z;
	//	FILE *Do_opa;
	//backward down pass of distance map calculation
	short *tX0,*tX1,*tX2,*tX3,*tX4;
	short *tY0,*tY1,*tY2,*tY3,*tY4;
	short *tZ0,*tZ1,*tZ2,*tZ3,*tZ4;
	int yT,zT;
	int t0,t1,t2,t3,t4;
	int yTtemp=N2x*(Ny+1);
	int zTemp=N2xy*(Nz+1);
	zT=zTemp;

	for (z=Nz;z>=1;z--)
	{
		zT-=N2xy;
		yT=yTtemp;
		for (y=Ny;y>=1;y--)
		{
			yT-=N2x;
			t0=zT		+	yT+1+Nx;		//(x,y,z)
			t1=t0		+	1;				//(x+1,y,z)
			t2=t0		+	N2x;			//(x,y+1,z)
			t3=t0		+	N2xy;			//(x,y,z+1)
			t4=zT		+	yT		;		//(x-1,y,z)

			tX0=TempDistanceVectorArrayX+t0;tX1=TempDistanceVectorArrayX+t1;tX2=TempDistanceVectorArrayX+t2;tX3=TempDistanceVectorArrayX+t3;tX4=TempDistanceVectorArrayX+t4;
			tY0=TempDistanceVectorArrayY+t0;tY1=TempDistanceVectorArrayY+t1;tY2=TempDistanceVectorArrayY+t2;tY3=TempDistanceVectorArrayY+t3;tY4=TempDistanceVectorArrayY+t4;
			tZ0=TempDistanceVectorArrayZ+t0;tZ1=TempDistanceVectorArrayZ+t1;tZ2=TempDistanceVectorArrayZ+t2;tZ3=TempDistanceVectorArrayZ+t3;tZ4=TempDistanceVectorArrayZ+t4;
			for(x=Nx;x>=1;x--)
			{
				tX0--;tX1--;tX2--;tX3--;
				tY0--;tY1--;tY2--;tY3--;
				tZ0--;tZ1--;tZ2--;tZ3--;

				a=sqtab_X[*tX0+infp1]	+	sqtab_Y[*tY0+infp1]	+	sqtab_Z[*tZ0+infp1];
				b=sqtabMX[*tX1+infp1]	+	sqtab_Y[*tY1+infp1]	+	sqtab_Z[*tZ1+infp1];
				c=sqtab_X[*tX2+infp1]	+	sqtabMY[*tY2+infp1]	+	sqtab_Z[*tZ2+infp1];
				d=sqtab_X[*tX3+infp1]	+	sqtab_Y[*tY3+infp1]	+	sqtabMZ[*tZ3+infp1];

				min=(a<b)?a:b;
				min=(c<min)?c:min;
				min=(d<min)?d:min;

				if (min==b)
				{
					*tX0=*tX1-1;
					*tY0=*tY1;
					*tZ0=*tZ1;
				}
				else if (min==c)
				{
					*tX0=*tX2;
					*tY0=*tY2-1;
					*tZ0=*tZ2;			
				}
				else if (min==d)
				{
					*tX0=*tX3;
					*tY0=*tY3;
					*tZ0=*tZ3-1;
				}
			}
			for( x=1;x<=Nx;x++)	
			{
				a=sqtab_X[*tX0+infp1]+sqtab_Y[*tY0+infp1]+sqtab_Z[*tZ0+infp1];
				b=sqtabPX[*tX4+infp1]+sqtab_Y[*tY4+infp1]+sqtab_Z[*tZ4+infp1];
				if (b<a)
				{
					*tX0=*tX4+1;
					*tY0=*tY4;
					*tZ0=*tZ4;	 
				}
				tX0++;tY0++;tZ0++;
				tX4++;tY4++;tZ4++;
			}
		}
	}
}
//---------------------------------------------------------------------------------
void DistanceMap::CreateOutputArrays()
{
	for (int z = 0;z < TempArraySize;z++)

	{
		if (TempDistanceVectorArrayY[z] != 1024)
		{
			z = z;
		}
	}
	int j,k;

	int a,b;

	//----------First
	DistanceVectorArrayX=new short[ArraySize];
	a=0;
	b=N2x+N2xy+1;
	int g=2*N2x;

	for (k = 0; k < Nz; k++)
	{
		for (j = 0; j < Ny; j++)
		{
			memcpy(DistanceVectorArrayX+a,TempDistanceVectorArrayX+b,Nx*sizeof(short));	
			a+=Nx;
			b+=N2x;
		}
		b+=g;
	}	
	delete[] TempDistanceVectorArrayX;

	//----------Second
	DistanceVectorArrayY=new short[ArraySize];
	a=0;
	b=N2x+N2xy+1;

	for (k = 0; k < Nz; k++)
	{
		for (j = 0; j < Ny; j++)
		{
			memcpy(DistanceVectorArrayY+a,TempDistanceVectorArrayY+b,Nx*sizeof(short));	
			a+=Nx;
			b+=N2x;
		}
		b+=g;
	}	
	delete[] TempDistanceVectorArrayY;

	//----------Third
	if (is2D == false)
	{
		DistanceVectorArrayZ = new short[ArraySize];
		a = 0;
		b = N2x + N2xy + 1;

		for (k = 0; k < Nz; k++)
		{
			for (j = 0; j < Ny; j++)
			{
				memcpy(DistanceVectorArrayZ + a, TempDistanceVectorArrayZ + b, Nx * sizeof(short));
				a += Nx;
				b += N2x;
			}
			b += g;
		}
		delete[] TempDistanceVectorArrayZ;
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------------------------
inline double  DistanceMap::Interpolation3Linear(unsigned short *Q,double X,double Y, double Z)
{
	int				x1,y1,z1,p;

	unsigned short	d0,d1,d2,d3,d4,d5,d6,d7;
	double			d,x,y,z,d8,d9,d10,d11,d12,d13;

	x1=(int)X;
	y1=(int)Y;
	z1=(int)Z;

	x=X-(double)x1;
	y=Y-(double)y1;
	z=Z-(double)z1;

	p=z1*Nxy+y1*Nx+x1;

	d0=*(Q+p);
	d1=*(Q+p+1);
	d2=*(Q+p+Nxy);
	d3=*(Q+p+Nxy+1);
	d4=*(Q+p+Nx);
	d5=*(Q+p+Nx+1);
	d6=*(Q+p+Nxy+Nx);
	d7=*(Q+p+Nxy+Nx+1);

	d8=(double)d0+z*(double)(d2-d0);
	d9=(double)d1+z*(double)(d3-d1);
	d10=(double)d4+z*(double)(d6-d4);
	d11=(double)d5+z*(double)(d7-d5);
	d12=d8+x*(d9-d8);
	d13=d10+x*(d11-d10);

	d=d12+y*(d13-d12);

	return d;
	//	return ((unsigned short)Round(d));

}

//-------------------------------------------------------------------

inline double  DistanceMap::Interpolation3Linear(short *Q,double X,double Y, double Z)
{

	int				x1,y1,z1,p;
	short	d0,d1,d2,d3,d4,d5,d6,d7;
	double			d,x,y,z,d8,d9,d10,d11,d12,d13;

	x1=(int)X;
	y1=(int)Y;
	z1=(int)Z;

	x=X-(double)x1;
	y=Y-(double)y1;
	z=Z-(double)z1;

	p=z1*Nxy+y1*Nx+x1;

	d0=*(Q+p);
	d1=*(Q+p+1);
	d2=*(Q+p+Nxy);
	d3=*(Q+p+Nxy+1);
	d4=*(Q+p+Nx);
	d5=*(Q+p+Nx+1);
	d6=*(Q+p+Nxy+Nx);
	d7=*(Q+p+Nxy+Nx+1);

	d8=(double)d0+z*(double)(d2-d0);
	d9=(double)d1+z*(double)(d3-d1);
	d10=(double)d4+z*(double)(d6-d4);
	d11=(double)d5+z*(double)(d7-d5);
	d12=d8+x*(d9-d8);
	d13=d10+x*(d11-d10);

	d=d12+y*(d13-d12);

	return d;
	//	return ((short)Round(d));

}

//-------------------------------------------------------------------
//--------------------------------------------------------
//---------------------------------------------------------------------------------
double	DistanceMap::GetDistanceValue(double x,double y,double z)
{
	double Distance;
	double DistX,DistY,DistZ;

	GetDistanceValues(x,y,z,Distance,DistX,DistY,DistZ);
	return Distance;

}
//-------------------------------------------------------------------
void	DistanceMap::GetDistanceValues(double x,double y,double z,double &Distance,double &DistX,double &DistY,double &DistZ)
{

	//	double rbck;
	//	double aa,bb,cc;
	//======================================
	//Cofam sie  do poczatku kostki...
	x-=x0M;y-=y0M;z-=z0M;
	//======================================
	//FILE *Do_opa;

	//	Do_opa=fopen("c:\\marq.txt","a");fprintf(Do_opa,"IN \n");fclose(Do_opa);

	//double min=(Xscale<Yscale)?Xscale:Yscale;
	//min=(Zscale<min)?Zscale:min;

	if ((x<(double)Nx-1)&&(x>=0.0)&&(y<(double)Ny-1)&&(y>=0.0)&&(z<(double)Nz-1)&&(z>=0.0))
	{

		DistX=Interpolation3Linear(DistanceVectorArrayX,x,y,z)*Xscale;
		//	DistX=aa;
		DistY=Interpolation3Linear(DistanceVectorArrayY,x,y,z)*Yscale;
		//	DistY=bb*rbck;
		DistZ=Interpolation3Linear(DistanceVectorArrayZ,x,y,z)*Zscale;
		//	DistZ=cc*rbck;
		Distance=sqrt(DistX*DistX+DistY*DistY+DistZ*DistZ);
	}
	else
	{	

		double qx,qy,qz;

		if ((x>=0.0)&&(x<(double)Nx-1.0))
			qx=x;
		else
			if (x<0.0)
				qx=0.0;
			else
				qx=(double)Nx-1.0001;


		if ((y>=0.0)&&(y<(double)Ny-1.0))
			qy=y;
		else
			if (y<0.0)
				qy=0.0;
			else
				qy=(double)Ny-1.0001;

		if ((z>=0.0)&&(z<(double)Nz-1.0))
			qz=z;
		else
			if (z<0.0)
				qz=0.0;
			else
				qz=(double)Nz-1.0001;

		DistX=(Interpolation3Linear(DistanceVectorArrayX,qx,qy,qz)+(x-qx))*Xscale;
		DistY=(Interpolation3Linear(DistanceVectorArrayY,qx,qy,qz)+(y-qy))*Yscale;
		DistZ=(Interpolation3Linear(DistanceVectorArrayZ,qx,qy,qz)+(z-qz))*Zscale;
		Distance=sqrt(DistX*DistX+DistY*DistY+DistZ*DistZ);


	}

}

//---------------------------------------------------------------------------------

void	DistanceMap::SaveDistanceMap( const QString &filename )
{

	if (!filename.isEmpty()) 
	{ 
		//const char *name;	
		//const char *name2;
		QString filename2,filename3;	
		FILE *plik;


		filename2=filename;

		if (filename2.lastIndexOf(".dist")==-1)
			filename2+=".dist";

		QString filenameT;

	//	name=filename2.toLocal8Bit();
		filename3=filename2+".header";


		//name2=filename3.toLocal8Bit();

		plik = fopen(filename3.toLocal8Bit(), "w");
		fprintf(plik,"%d %d %d\n%lf %lf %lf\n%d %d %d\n%lf",Nx,Ny,Nz,Xscale,Yscale,Zscale,x0,y0,z0,rangeback);
		fclose(plik);



		//		plik=fopen(name,"wb");
		//		fwrite( DistanceArray,sizeof(unsigned short),ArraySize,plik);
		//		fclose(plik);

		filenameT=filename2+".x";
		plik=fopen(filenameT.toLocal8Bit(),"wb");
		fwrite( DistanceVectorArrayX,sizeof(short),ArraySize,plik);
		fclose(plik);

		filenameT=filename2+".y";
		plik=fopen(filenameT.toLocal8Bit(),"wb");
		fwrite( DistanceVectorArrayY,sizeof(short),ArraySize,plik);
		fclose(plik);

		filenameT=filename2+".z";
		plik=fopen(filenameT.toLocal8Bit(),"wb");
		fwrite( DistanceVectorArrayZ,sizeof(short),ArraySize,plik);
		fclose(plik);
	}	
}

//---------------------------------------------------------------------------------

void	DistanceMap::LoadDistanceMap( const QString &filename )
{

	QFile f( filename );
	QString filenameT;

	FILE *plik;
	QString tempX;
	tempX=filename+".header";
	plik=fopen(tempX.toLocal8Bit(),"rt");
	fscanf(plik,"%d %d %d %lf %lf %lf %lf %lf %lf %lf",&Nx,&Ny,&Nz,&Xscale,&Yscale,&Zscale,&x0,&y0,&z0,&rangeback);
	fclose(plik);

	Nxy=Nx*Ny;
	XSQscale=Xscale*Xscale;YSQscale=Yscale*Yscale;ZSQscale=Zscale*Zscale;
	ArraySize=Nx*Ny*Nz;


	if ( !f.open( QIODevice::ReadOnly ) ) return;

	if (DistanceVectorArrayX!=NULL)
	{
		//	delete[] DistanceArray;
		delete[] DistanceVectorArrayX;
		delete[] DistanceVectorArrayY;
		delete[] DistanceVectorArrayZ;
	}

	DistanceVectorArrayX=new short[ArraySize];
	DistanceVectorArrayY=new short[ArraySize];
	DistanceVectorArrayZ=new short[ArraySize];

	filenameT=filename+".x";
	plik=fopen(filenameT.toLocal8Bit(),"rb");
	fread( DistanceVectorArrayX,sizeof(short),ArraySize,plik);
	fclose(plik);

	filenameT=filename+".y";
	plik=fopen(filenameT.toLocal8Bit(),"rb");
	fread( DistanceVectorArrayY,sizeof(short),ArraySize,plik);
	fclose(plik);

	filenameT=filename+".z";
	plik=fopen(filenameT.toLocal8Bit(),"rb");
	fread( DistanceVectorArrayZ,sizeof(short),ArraySize,plik);
	fclose(plik);

}
//---------------------------------------------------------------------------------
int DistanceMap::FillTempDistArrayXWithPolyDataPoints(short *Array)
{
	//zwraca liczbe punktow w tablicy;
	int DistDataPoints=0;

	double XS=1.0/Xscale;
	double YS=1.0/Yscale;
	double ZS=1.0/Zscale;
	int xx,yy,zz;
	double pPoint[3];
	int i;
	vtkPoints *pPoints=NULL;;
	if (SurfaceData!=NULL)
	pPoints=SurfaceData->GetVPolyData()->GetPoints();
	else if (sPoints!=NULL)
	pPoints=sPoints;


	if (pPoints!=NULL)
	{
		int pIndex=pPoints->GetNumberOfPoints();


		for (i=0;i<pIndex;i++)
		{
			pPoints->GetPoint(i,pPoint);

			// cofam kazdy punkt do poczatku kostki
			xx =Round((double) ((pPoint[0]-x0)*XS))+1;
			yy =Round((double) ((pPoint[1]-y0)*YS))+1;
			zz =Round((double) ((pPoint[2]-z0)*ZS))+1;

			// tu trzeba wstawic cube obraniczjacy a nie caly!
			if ((xx>=1)&&(xx<Nx+1)&&(yy>=1)&&(yy<Ny+1)&&(zz>=1)&&(zz<Nz+1))
			{			
				Array[zz*N2xy+yy*N2x+xx]=0;
				DistDataPoints++;
			}
		}

		return DistDataPoints;
	}
	else
		return NULL;
}
//--------------------------------------------
int  DistanceMap::FillTempDistArrayXWithRawDataPoints(short *Array)
{
	int DistDataPoints=0;
	int i,j,k;

	//	int pos=0;
	unsigned short *Q=RawData->GetDataArray();

	for(k=0;k<RawData->GetNz();k++)
		for(j=0;j<RawData->GetNy();j++)
			for(i=0;i<RawData->GetNx();i++)
			{
				if (((*Q)>=LThreshold)&&((*Q)<=RThreshold))
				{
					Array[(k+1)*N2xy+(j+1)*N2x+(i+1)]=0;
					DistDataPoints++;
				}
				Q++;
			}

			return DistDataPoints;
}

//--------------------------------------------
int  DistanceMap::FillTempDistArrayXWithMaskPoints(short *Array)
{
	int DistDataPoints=0;
	int i,j,k;

	//	int pos=0;
	unsigned char *Q=Mask;

	for(k=0;k<RawData->GetNz();k++)
		for(j=0;j<RawData->GetNy();j++)
			for(i=0;i<RawData->GetNx();i++)
			{
				if (*Q>0)
				{
					Array[(k+1)*N2xy+(j+1)*N2x+(i+1)]=0;
					DistDataPoints++;
				}

				Q++;
			}

			return DistDataPoints;
}

//--------------------------------------------
bool  DistanceMap::PrecalucateDistanceMap()
{
	if (is2D == false)
	{
		//		FILE *Do_opa;
		int i;

		infp1 = infinity + 1;
		TempDistanceVectorArrayX = new short[TempArraySize];
		TempDistanceVectorArrayY = new short[TempArraySize];
		TempDistanceVectorArrayZ = new short[TempArraySize];
		for (i = 0;i < N2xy;i++)
			TempDistanceVectorArrayX[i] = infinity;
		int tempol = N2xy;
		for (i = 0;i <= Nz;i++)
		{
			memcpy(TempDistanceVectorArrayX + tempol, TempDistanceVectorArrayX, N2xy * sizeof(short));
			tempol += N2xy;
		}

		int DistDataPoints;
		if (RawData == NULL)
			DistDataPoints = FillTempDistArrayXWithPolyDataPoints(TempDistanceVectorArrayX);
		else
		{
			if (Mask == NULL)
				DistDataPoints = FillTempDistArrayXWithRawDataPoints(TempDistanceVectorArrayX);
			else
				DistDataPoints = FillTempDistArrayXWithMaskPoints(TempDistanceVectorArrayX);

		}
		memcpy(TempDistanceVectorArrayY, TempDistanceVectorArrayX, TempArraySize * sizeof(short));
		memcpy(TempDistanceVectorArrayZ, TempDistanceVectorArrayX, TempArraySize * sizeof(short));

		numberOfSeeds = DistDataPoints;
		if (DistDataPoints > 0)
		{
			//	int infinity1=infinity+1;
			sqtab_X = new double[2 * infp1 + 1];
			sqtabPX = new double[2 * infp1 + 1];
			sqtabMX = new double[2 * infp1 + 1];

			sqtab_Y = new double[2 * infp1 + 1];
			sqtabPY = new double[2 * infp1 + 1];
			sqtabMY = new double[2 * infp1 + 1];

			sqtab_Z = new double[2 * infp1 + 1];
			sqtabPZ = new double[2 * infp1 + 1];
			sqtabMZ = new double[2 * infp1 + 1];

			double tmp0;
			double tmp1;
			double tmp2;
			int pos = 0;
			for (i = -infp1;i <= infp1;i++)
			{
				tmp0 = (i - 1) * (i - 1);
				tmp1 = i * i;
				tmp2 = (i + 1) * (i + 1);

				sqtabMX[pos] = tmp0 * XSQscale;
				sqtab_X[pos] = tmp1 * XSQscale;
				sqtabPX[pos] = tmp2 * XSQscale;

				sqtabMY[pos] = tmp0 * YSQscale;
				sqtab_Y[pos] = tmp1 * YSQscale;
				sqtabPY[pos] = tmp2 * YSQscale;

				sqtabMZ[pos] = tmp0 * ZSQscale;
				sqtab_Z[pos] = tmp1 * ZSQscale;
				sqtabPZ[pos] = tmp2 * ZSQscale;

				pos++;
				//	Do_opa=fopen("c:\\dd.txt","a");fprintf(Do_opa,"%d (%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf) \n",i,sqtabMX[i],sqtabPX[i],sqtab_X[i],sqtabMY[i],sqtabPY[i],sqtab_Y[i],sqtabMZ[i],sqtabPZ[i],sqtab_Z[i]);fclose(Do_opa);

			}

			CalculateDistance();

			delete	sqtab_X;
			delete	sqtab_Y;
			delete	sqtab_Z;
			delete[]sqtabPX;
			delete[]sqtabPY;
			delete[]sqtabPZ;
			delete[]sqtabMX;
			delete[]sqtabMY;
			delete[]sqtabMZ;

			sqtab_X = NULL;
			sqtab_Y = NULL;
			sqtab_Z = NULL;
			sqtabPX = NULL;
			sqtabPY = NULL;
			sqtabPZ = NULL;
			sqtabMX = NULL;
			sqtabMY = NULL;
			sqtabMZ = NULL;
			return true;
		}
		else
		{
			// cuboid pusty - zrob z tym cos Michalek :)

		}


		delete[] TempDistanceVectorArrayX;TempDistanceVectorArrayX = NULL;
		delete[]TempDistanceVectorArrayY; TempDistanceVectorArrayX = NULL;
		delete[]TempDistanceVectorArrayZ;TempDistanceVectorArrayX = NULL;

		return false;
	}
	else
	{
	//		FILE *Do_opa;
	int i;

	infp1 = infinity + 1;
	TempDistanceVectorArrayX = new short[TempArraySize];
	TempDistanceVectorArrayY = new short[TempArraySize];
	
	for (i = 0;i < N2xy;i++)
		TempDistanceVectorArrayX[i] = infinity;
	int tempol = N2xy;
	for (i = 0;i <= Nz;i++)
	{
		memcpy(TempDistanceVectorArrayX + tempol, TempDistanceVectorArrayX, N2xy * sizeof(short));
		tempol += N2xy;
	}

	int DistDataPoints;
	if (RawData == NULL)
		DistDataPoints = FillTempDistArrayXWithPolyDataPoints(TempDistanceVectorArrayX);
	else
	{
		if (Mask == NULL)
			DistDataPoints = FillTempDistArrayXWithRawDataPoints(TempDistanceVectorArrayX);
		else
			DistDataPoints = FillTempDistArrayXWithMaskPoints(TempDistanceVectorArrayX);

	}
	memcpy(TempDistanceVectorArrayY, TempDistanceVectorArrayX, TempArraySize * sizeof(short));
	
	
	numberOfSeeds = DistDataPoints;
	if (DistDataPoints > 0)
	{
		//	int infinity1=infinity+1;
		sqtab_X = new double[2 * infp1 + 1];
		sqtabPX = new double[2 * infp1 + 1];
		sqtabMX = new double[2 * infp1 + 1];

		sqtab_Y = new double[2 * infp1 + 1];
		sqtabPY = new double[2 * infp1 + 1];
		sqtabMY = new double[2 * infp1 + 1];

		

		double tmp0;
		double tmp1;
		double tmp2;
		int pos = 0;
		for (i = -infp1;i <= infp1;i++)
		{
			tmp0 = (i - 1) * (i - 1);
			tmp1 = i * i;
			tmp2 = (i + 1) * (i + 1);

			sqtabMX[pos] = tmp0 * XSQscale;
			sqtab_X[pos] = tmp1 * XSQscale;
			sqtabPX[pos] = tmp2 * XSQscale;

			sqtabMY[pos] = tmp0 * YSQscale;
			sqtab_Y[pos] = tmp1 * YSQscale;
			sqtabPY[pos] = tmp2 * YSQscale;

			

			pos++;
			//	Do_opa=fopen("c:\\dd.txt","a");fprintf(Do_opa,"%d (%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf) \n",i,sqtabMX[i],sqtabPX[i],sqtab_X[i],sqtabMY[i],sqtabPY[i],sqtab_Y[i],sqtabMZ[i],sqtabPZ[i],sqtab_Z[i]);fclose(Do_opa);

		}

		CalculateDistance();

		delete	sqtab_X;
		delete	sqtab_Y;
		
		delete[]sqtabPX;
		delete[]sqtabPY;
		
		delete[]sqtabMX;
		delete[]sqtabMY;
		

		sqtab_X = NULL;
		sqtab_Y = NULL;
		
		sqtabPX = NULL;
		sqtabPY = NULL;
		
		sqtabMX = NULL;
		sqtabMY = NULL;
		
		return true;
	}
	else
	{
		// cuboid pusty - zrob z tym cos Michalek :)

	}


	delete[] TempDistanceVectorArrayX;TempDistanceVectorArrayX = NULL;
	delete[]TempDistanceVectorArrayY; TempDistanceVectorArrayX = NULL;
	

	return false;

	}
}
//---------------------------------------------------
double	DistanceMap::Generate3DUnsignedDistanceMapVolume(RawDataSet *rdata)
{
	//		FILE *Do_opa;
	int i;

	infp1=infinity+1;
	TempDistanceVectorArrayX=new short[TempArraySize];
	TempDistanceVectorArrayY=new short[TempArraySize];
	TempDistanceVectorArrayZ=new short[TempArraySize];
	for (i=0;i<N2xy;i++)
		TempDistanceVectorArrayX[i]=infinity;
	int tempol=N2xy;
	for(i=0;i<=Nz;i++)
	{
		memcpy(TempDistanceVectorArrayX+tempol,TempDistanceVectorArrayX,N2xy*sizeof(short));
		tempol+=N2xy;	
	}

	int DistDataPoints;
	if (RawData==NULL)
		DistDataPoints=FillTempDistArrayXWithPolyDataPoints(TempDistanceVectorArrayX);
	else
	{
		if (Mask==NULL)
			DistDataPoints=FillTempDistArrayXWithRawDataPoints(TempDistanceVectorArrayX);
		else
			DistDataPoints=FillTempDistArrayXWithMaskPoints(TempDistanceVectorArrayX);

	}
	FindBoundaries3D(TempDistanceVectorArrayZ,TempDistanceVectorArrayX);
	memcpy(TempDistanceVectorArrayX,TempDistanceVectorArrayZ,TempArraySize*sizeof(short));
	memcpy(TempDistanceVectorArrayY,TempDistanceVectorArrayZ,TempArraySize*sizeof(short));

	numberOfSeeds=DistDataPoints;
	if (DistDataPoints>0)
	{
		//	int infinity1=infinity+1;
		sqtab_X	=	new double[2*infp1+1];
		sqtabPX	=	new double[2*infp1+1];
		sqtabMX	=	new double[2*infp1+1];

		sqtab_Y	=	new double[2*infp1+1];
		sqtabPY	=	new double[2*infp1+1];
		sqtabMY	=	new double[2*infp1+1];

		sqtab_Z	=	new double[2*infp1+1];
		sqtabPZ	=	new double[2*infp1+1];
		sqtabMZ	=	new double[2*infp1+1];

		double tmp0;
		double tmp1;
		double tmp2;
		int pos=0;
		for(i=-infp1;i<=infp1;i++)
		{
			tmp0=(i-1)*(i-1);
			tmp1=i*i;
			tmp2=(i+1)*(i+1);

			sqtabMX[pos]	=tmp0*XSQscale;
			sqtab_X[pos]	=tmp1*XSQscale;
			sqtabPX[pos]	=tmp2*XSQscale;

			sqtabMY[pos]	=tmp0*YSQscale;
			sqtab_Y[pos]	=tmp1*YSQscale;
			sqtabPY[pos]	=tmp2*YSQscale;

			sqtabMZ[pos]	=tmp0*ZSQscale;
			sqtab_Z[pos]	=tmp1*ZSQscale;
			sqtabPZ[pos]	=tmp2*ZSQscale;

			pos++;
			//	Do_opa=fopen("c:\\dd.txt","a");fprintf(Do_opa,"%d (%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf) \n",i,sqtabMX[i],sqtabPX[i],sqtab_X[i],sqtabMY[i],sqtabPY[i],sqtab_Y[i],sqtabMZ[i],sqtabPZ[i],sqtab_Z[i]);fclose(Do_opa);

		}

		CalculateDistanceArrays();

		delete	sqtab_X;
		delete	sqtab_Y;
		delete	sqtab_Z;
		delete	[]sqtabPX;
		delete	[]sqtabPY;
		delete	[]sqtabPZ;
		delete	[]sqtabMX;
		delete	[]sqtabMY;
		delete	[]sqtabMZ;

		sqtab_X=NULL;
		sqtab_Y=NULL;
		sqtab_Z=NULL;
		sqtabPX=NULL;
		sqtabPY=NULL;
		sqtabPZ=NULL;
		sqtabMX=NULL;
		sqtabMY=NULL;
		sqtabMZ=NULL;

		return Create3DDistanceMapRawDataSet(rdata);

	}
	else
	{
		return 1;
		// cuboid pusty - zrob z tym cos Michalek :)
	}

}
//--
RawDataSet* DistanceMap::Generate3DUnsignedDistanceMapVolume(bool clear)
{

	//		FILE *Do_opa;
	int i;

	infp1=infinity+1;
	TempDistanceVectorArrayX=new short[TempArraySize];
	TempDistanceVectorArrayY=new short[TempArraySize];
	TempDistanceVectorArrayZ=new short[TempArraySize];
	for (i=0;i<N2xy;i++)
		TempDistanceVectorArrayX[i]=infinity;
	int tempol=N2xy;
	for(i=0;i<=Nz;i++)
	{
		memcpy(TempDistanceVectorArrayX+tempol,TempDistanceVectorArrayX,N2xy*sizeof(short));
		tempol+=N2xy;	
	}

	int DistDataPoints;
	if (RawData==NULL)
		DistDataPoints=FillTempDistArrayXWithPolyDataPoints(TempDistanceVectorArrayX);
	else
	{
		if (Mask==NULL)
			DistDataPoints=FillTempDistArrayXWithRawDataPoints(TempDistanceVectorArrayX);
		else
			DistDataPoints=FillTempDistArrayXWithMaskPoints(TempDistanceVectorArrayX);

	}
	FindBoundaries3D(TempDistanceVectorArrayZ,TempDistanceVectorArrayX);
	memcpy(TempDistanceVectorArrayX,TempDistanceVectorArrayZ,TempArraySize*sizeof(short));
	memcpy(TempDistanceVectorArrayY,TempDistanceVectorArrayZ,TempArraySize*sizeof(short));

	numberOfSeeds=DistDataPoints;
	if (DistDataPoints>0)
	{
		//	int infinity1=infinity+1;
		sqtab_X	=	new double[2*infp1+1];
		sqtabPX	=	new double[2*infp1+1];
		sqtabMX	=	new double[2*infp1+1];

		sqtab_Y	=	new double[2*infp1+1];
		sqtabPY	=	new double[2*infp1+1];
		sqtabMY	=	new double[2*infp1+1];

		sqtab_Z	=	new double[2*infp1+1];
		sqtabPZ	=	new double[2*infp1+1];
		sqtabMZ	=	new double[2*infp1+1];

		double tmp0;
		double tmp1;
		double tmp2;
		int pos=0;
		for(i=-infp1;i<=infp1;i++)
		{
			tmp0=(i-1)*(i-1);
			tmp1=i*i;
			tmp2=(i+1)*(i+1);

			sqtabMX[pos]	=tmp0*XSQscale;
			sqtab_X[pos]	=tmp1*XSQscale;
			sqtabPX[pos]	=tmp2*XSQscale;

			sqtabMY[pos]	=tmp0*YSQscale;
			sqtab_Y[pos]	=tmp1*YSQscale;
			sqtabPY[pos]	=tmp2*YSQscale;

			sqtabMZ[pos]	=tmp0*ZSQscale;
			sqtab_Z[pos]	=tmp1*ZSQscale;
			sqtabPZ[pos]	=tmp2*ZSQscale;

			pos++;
			//	Do_opa=fopen("c:\\dd.txt","a");fprintf(Do_opa,"%d (%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf) \n",i,sqtabMX[i],sqtabPX[i],sqtab_X[i],sqtabMY[i],sqtabPY[i],sqtab_Y[i],sqtabMZ[i],sqtabPZ[i],sqtab_Z[i]);fclose(Do_opa);

		}

		CalculateDistanceArrays();

		delete	sqtab_X;
		delete	sqtab_Y;
		delete	sqtab_Z;
		delete	[]sqtabPX;
		delete	[]sqtabPY;
		delete	[]sqtabPZ;
		delete	[]sqtabMX;
		delete	[]sqtabMY;
		delete	[]sqtabMZ;

		sqtab_X=NULL;
		sqtab_Y=NULL;
		sqtab_Z=NULL;
		sqtabPX=NULL;
		sqtabPY=NULL;
		sqtabPZ=NULL;
		sqtabMX=NULL;
		sqtabMY=NULL;
		sqtabMZ=NULL;

		RawDataSet *dataX = new RawDataSet(Nx, Ny, Nz, Xscale, Yscale, Zscale);
		Create3DDistanceMapRawDataSet(dataX);
		return dataX;
		//return Create3DDistanceMapRawDataSet( clear);
	}
	else
	{
		return NULL;
		// cuboid pusty - zrob z tym cos Michalek :)
	}


}
//----------------------------------------------
//-----------------------------------
RawDataSet* DistanceMap::Generate2DSignedDistanceMapVolume(bool createVolume)
{

	//		FILE *Do_opa;
	int i;

	infp1=infinity+1;
	TempDistanceVectorArrayX=new short[TempArraySize];
	TempDistanceVectorArrayY=new short[TempArraySize];
	TempDistanceVectorArrayZ=new short[TempArraySize]; // tu bede trzymal oryginal
	for (i=0;i<N2xy;i++)
		TempDistanceVectorArrayX[i]=infinity;
	int tempol=N2xy;
	for(i=0;i<=Nz;i++)
	{
		memcpy(TempDistanceVectorArrayX+tempol,TempDistanceVectorArrayX,N2xy*sizeof(short));
		tempol+=N2xy;	
	}

	int DistDataPoints;
	if (RawData==NULL)
		DistDataPoints=FillTempDistArrayXWithPolyDataPoints(TempDistanceVectorArrayX);
	else
	{
		if (Mask==NULL)
			DistDataPoints=FillTempDistArrayXWithRawDataPoints(TempDistanceVectorArrayX);
		else
			DistDataPoints=FillTempDistArrayXWithMaskPoints(TempDistanceVectorArrayX);

	}
	/////////////////
	//stworze sobie ta trzecia w innym celu zeby trzymac tam oryginal
	memcpy(TempDistanceVectorArrayZ,TempDistanceVectorArrayX,TempArraySize*sizeof(short));
	// x i y beda zmodyfikowane zeby zawierac tylko obrys
	FindBoundaries(TempDistanceVectorArrayX,TempDistanceVectorArrayZ);
	memcpy(TempDistanceVectorArrayY,TempDistanceVectorArrayX,TempArraySize*sizeof(short));
	////////////////

	numberOfSeeds=DistDataPoints;
	if (DistDataPoints>0)
	{
		//	int infinity1=infinity+1;
		sqtab_X	=	new double[2*infp1+1];
		sqtabPX	=	new double[2*infp1+1];
		sqtabMX	=	new double[2*infp1+1];

		sqtab_Y	=	new double[2*infp1+1];
		sqtabPY	=	new double[2*infp1+1];
		sqtabMY	=	new double[2*infp1+1];
		//	
		sqtab_Z	=	new double[2*infp1+1];
		sqtabPZ	=	new double[2*infp1+1];
		sqtabMZ	=	new double[2*infp1+1];
		//

		double tmp0;
		double tmp1;
		double tmp2;
		int pos=0;
		for(i=-infp1;i<=infp1;i++)
		{
			tmp0=(i-1)*(i-1);
			tmp1=i*i;
			tmp2=(i+1)*(i+1);

			sqtabMX[pos]	=tmp0*XSQscale;
			sqtab_X[pos]	=tmp1*XSQscale;
			sqtabPX[pos]	=tmp2*XSQscale;

			sqtabMY[pos]	=tmp0*YSQscale;
			sqtab_Y[pos]	=tmp1*YSQscale;
			sqtabPY[pos]	=tmp2*YSQscale;

			sqtabMZ[pos]	=tmp0*ZSQscale;
			sqtab_Z[pos]	=tmp1*ZSQscale;
			sqtabPZ[pos]	=tmp2*ZSQscale;

			pos++;

		}
		Calculate2DDistanceArrays();
		//memcpy(TempDistanceVectorArrayZ,TempDistanceVectorArrayX,TempArraySize*sizeof(short));
		//CalculateDistance();
		/*


		*/


		delete	sqtab_X;
		delete	sqtab_Y;
		delete	sqtab_Z;
		delete	[]sqtabPX;
		delete	[]sqtabPY;
		delete	[]sqtabPZ;
		delete	[]sqtabMX;
		delete	[]sqtabMY;
		delete	[]sqtabMZ;

		sqtab_X=NULL;
		sqtab_Y=NULL;
		sqtab_Z=NULL;
		sqtabPX=NULL;
		sqtabPY=NULL;
		sqtabPZ=NULL;
		sqtabMX=NULL;
		sqtabMY=NULL;
		sqtabMZ=NULL;
		RawDataSet *vol = NULL;
		if (createVolume == true) vol = Create2DDistanceMapRawDataSet(); else CreateOutputArrays();
		return vol;
		


	}
	else
	{
		return NULL;
		// cuboid pusty - zrob z tym cos Michalek :)
	}

}
//----------------------------------------------
void DistanceMap::FindBoundaries(short *tab,short *original)
{

	int i,j,k;
	int zz=0;
	short *ts,*td;
	int p;
	int pp;

	//+czyszczenie
	int tx,ty;
	bool count;

	for(k=1;k<Nz+1;k++)
	{
		zz+=N2xy;
		td=tab+zz;
		ts=original+zz;
		//p=0;


		// dziadosto zeby nie bylo skopanych sliceow
		count=false;
		for (ty=1;ty<=Ny;ty++)
		{
			for(tx=1;tx<=Nx;tx++)
			{
				p=N2x* ty + tx;
				if (ts[p]==0) 
				{
					count=true;
					break;
				}
			}
			if (count==true) break;
		}

		if (count==true)
			for(j=1;j<Ny+1;j++)
			{
				for(i=1;i<Nx+1;i++)
				{
					p=N2x* j + i;
					pp=ts[p];
					if (pp==0)
						if (!(
							(ts[p-1]		!=pp)||
							(ts[p+1]		!=pp)||
							(ts[p+N2x]		!=pp)||
							(ts[p-N2x]		!=pp)||
							(ts[p+1+N2x]	!=pp)||
							(ts[p+1-N2x]	!=pp)||
							(ts[p-1+N2x]	!=pp)||
							(ts[p-1-N2x]	!=pp)))
						{
							td[p]=infinity;
						}
						//	p++;
				}

			}
		else
		{
			// dziadosto zeby nie bylo skopanych sliceow
			for (ty=1;ty<=Ny;ty++)
				for(tx=1;tx<=Nx;tx++)
				{
					p=N2x* ty + tx;
					td[p]=0;
				}
		}

	}
	ts=NULL;
	td=NULL;
}
//----------------------------------
//----------------------------------------------
void DistanceMap::FindBoundaries3D(short *tab,short *original)
{

	if ((tab!=NULL)&&(original!=NULL))
	{
		short *ts,*td;
		td=tab;
		ts=original;

		//memset(boundaryMask,0,rawData->GetVolumeSize());
		memcpy(tab,original,N2x*N2y*N2z*sizeof(short));
		int i,j,k;


		int p,pp;
		for(k=1;k<=Nz;k++)
			for(j=1;j<=Ny;j++)
				for(i=1;i<=Nx;i++)
				{
					p=N2xy*k+N2x* j + i; 
					pp=ts[p];
					if (pp==0)
						if (!(
							(ts[p-1]		!=pp)||
							(ts[p+1]		!=pp)||
							(ts[p+N2x]		!=pp)||
							(ts[p-N2x]		!=pp)||
							(ts[p+N2xy]	!=pp)||
							(ts[p-N2xy]	!=pp)))

						{
							td[p]=infinity;
						}

				}
				ts=NULL;
				td=NULL;
	}

	/*
	int i,j,k;
	int zz=0;
	short *ts,*td;
	int p;
	int pp;

	//+czyszczenie
	int tx,ty;
	bool count;

	for(k=1;k<Nz+1;k++)
	{
	zz+=N2xy;
	td=tab+zz;
	ts=original+zz;
	//p=0;


	// dziadosto zeby nie bylo skopanych sliceow
	count=false;
	for (ty=1;ty<=Ny;ty++)
	{
	for(tx=1;tx<=Nx;tx++)
	{
	p=N2x* ty + tx;
	if (ts[p]==0) 
	{
	count=true;
	break;
	}
	}
	if (count==true) break;
	}

	//		for(j=1;j<Ny+1;j++)
	{
	for(i=1;i<Nx+1;i++)
	{
	p=N2x* j + i;
	pp=ts[p];
	if (pp==0)
	if (!(
	(ts[p-1]		!=pp)||
	(ts[p+1]		!=pp)||
	(ts[p+N2x]		!=pp)||
	(ts[p-N2x]		!=pp)||
	(ts[p+N2xy]		!=pp)||
	(ts[p-N2xy]		!=pp)

	))
	{
	td[p]=infinity;
	}
	//	p++;
	}

	}
	else
	{
	// dziadosto zeby nie bylo skopanych sliceow
	for (ty=1;ty<=Ny;ty++)
	for(tx=1;tx<=Nx;tx++)
	{
	p=N2x* ty + tx;
	td[p]=0;
	}
	}

	}
	ts=NULL;
	td=NULL;

	*/


}
//----------------------------------
void DistanceMap::Calculate2DDistanceArrays()
{
	CalculateForwardUp2D();
	CalculateBackwardDown2D();

}
//-------------------------------------------
RawDataSet* DistanceMap::CreateDistanceMapRawDataSet(bool clear )
{
	if (is2D == false)
	{
		return Create3DDistanceMapRawDataSet(clear);
	}
	else
	{
		return Create2DDistanceMapRawDataSet(clear);
	}
	return NULL;
}
RawDataSet* DistanceMap::Create2DDistanceMapRawDataSet(bool clear)
{
	//RawDataSet *dataX=new RawDataSet(Nx,Ny,Nz,Xscale,Yscale,Zscale);

	double* Tab = new double[ArraySize];

	int i, j, k;

	//QString name;
	//name="Distance Map";
	//double Dist;
	//dataX->GetRawDataSetName()=name+"-dist";
	//double max=0;
	int zz = 0;
	short* tx, * ty, * tz;
	int p;
	//	int pp;

	double DistX, DistY, Distance;
	int pos = 0;
	double Min = 0, Max = 0;
	zz = 0;
	//zrobic tablice doubleowych dystansow wlacznie ze zmiana znakow
	for (k = 0;k < Nz;k++)
	{
		//zz+=N2xy;
		tx = DistanceVectorArrayX;
		ty = DistanceVectorArrayY;
	

		/*	zz+=N2xy;
			tx=TempDistanceVectorArrayX+zz;
			ty=TempDistanceVectorArrayY+zz;
			tz=TempDistanceVectorArrayZ+zz;*/

		for (j = 0;j < Ny;j++)
			for (i = 0;i < Nx;i++)
			{
				//mask
				if ((clearMask != NULL) && (clear == true) && (clearMask[pos] == 0))
				{
					Distance = 0;
					Min = 0;
					Tab[pos++] = -1;
				}
				else
				{
					p = N2x * (j + 1) + i + 1;
					//if ((p>0)&&(p<TempArraySize))
					{
						DistX = tx[pos] * Xscale;
						DistY = ty[pos] * Yscale;
						
						Distance = sqrt(DistX * DistX + DistY * DistY );

						Tab[pos++] = Distance;

						if (Distance > Max) Max = Distance;
						else if (Distance < Min) Min = Distance;
					}
				}

			}
	}


	maximumDistance = Max;
	double factor = Max - Min;
	if (clear == false)
		factor = (factor != 0) ? factor = (double)65535 / factor : factor = 1;
	else
		factor = (factor != 0) ? factor = (double)65534 / factor : factor = 1;
	rangeback = 1.0 / factor;
	RawDataSet* dataX = new RawDataSet(Nx, Ny, Nz, Xscale, Yscale, Zscale);
	unsigned short* Q = dataX->GetDataArray();
	bool count;
	pos = 0;
	// for (i=0;i<dataX->GetVolumeSize();i++)
	//		Q[i]=(Tab[i+pos]-Min)*factor;
	dataX->ZeroQ();
	if (clear == false)
	{
		for (i = 0;i < dataX->GetVolumeSize();i++)
			*(Q++) = (Tab[i] - Min) * factor;
	}
	else
	{
		for (i = 0;i < dataX->GetVolumeSize();i++)
		{
			if (Tab[i] < 0)
			{

				Q[i] = 0;
			}

			else
			{
				Q[i] = (Tab[i] - Min) * factor + 1;
			}
		}
	}

	
	ZeroLevel = abs(Min * factor);

	Q = NULL;
	delete[]Tab;
	Tab = NULL;
	return dataX;
}
//------------------------------------------
//-------------------------------------------
RawDataSet *DistanceMap::Create3DDistanceMapRawDataSet(bool clear)
{
	//RawDataSet *dataX=new RawDataSet(Nx,Ny,Nz,Xscale,Yscale,Zscale);

	double *Tab=new double[ArraySize];

	int i,j,k;

	//QString name;
	//name="Distance Map";
	//double Dist;
	//dataX->GetRawDataSetName()=name+"-dist";
	//double max=0;
	int zz=0;
	short *tx,*ty,*tz;
	int p;
	//	int pp;

	double DistX,DistY,DistZ,Distance;
	int pos=0;
	double Min=0,Max=0;
	zz=0;
	//zrobic tablice doubleowych dystansow wlacznie ze zmiana znakow
	for(k=0;k<Nz;k++)
	{
		//zz+=N2xy;
		tx=DistanceVectorArrayX;
		ty=DistanceVectorArrayY;
		tz=DistanceVectorArrayZ;

	/*	zz+=N2xy;
		tx=TempDistanceVectorArrayX+zz;
		ty=TempDistanceVectorArrayY+zz;
		tz=TempDistanceVectorArrayZ+zz;*/

		for(j=0;j<Ny;j++)
			for(i=0;i<Nx;i++)
			{
				//mask
				if ((clearMask!=NULL)&&(clear==true)&&(clearMask[pos]==0))
				{
					Distance=0; 
					Min=0;
					Tab[pos++]=-1;
				}
				else
				{
					p=N2x* (j+1) + i+1;
					//if ((p>0)&&(p<TempArraySize))
					{
						DistX=tx[pos]*Xscale;
						DistY=ty[pos]*Yscale;
						DistZ=tz[pos]*Zscale;
						Distance=sqrt(DistX*DistX+DistY*DistY+DistZ*DistZ);

						Tab[pos++]=Distance;

						if (Distance>Max) Max=Distance;
						else if (Distance<Min) Min=Distance;
					}
				}

			}
	}


	//delete []TempDistanceVectorArrayX;
	//delete []TempDistanceVectorArrayY;
	//delete []TempDistanceVectorArrayZ;
	maximumDistance = Max;
	double factor=Max-Min;
	if (clear==false)
	factor=(factor!=0)?factor=(double)65535/factor:factor=1;
	else
	factor=(factor!=0)?factor=(double)65534/factor:factor=1;
	rangeback=1.0/factor;
	RawDataSet *dataX=new RawDataSet(Nx,Ny,Nz,Xscale,Yscale,Zscale);
	unsigned short *Q=dataX->GetDataArray();
	bool count;
	pos=0;
	// for (i=0;i<dataX->GetVolumeSize();i++)
	//		Q[i]=(Tab[i+pos]-Min)*factor;
	dataX->ZeroQ();
	if (clear==false)
	{
		for(i=0;i<dataX->GetVolumeSize();i++)
			*(Q++)=(Tab[i]-Min)*factor;
	}
	else
	{
		for(i=0;i<dataX->GetVolumeSize();i++)
		{
			if (Tab[i]<0)
			{

				Q[i]=0;
			}
			
			else
			{
				Q[i]=(Tab[i]-Min)*factor+1;
			}
		}
	}

	/*	for (k=0;k<dataX->GetNz();k++)
	{
	// sprawdzic czy wszystkie pixle w danym sliceu sa zerem
	count=false;
	for (i=0;i<dataX->GetSliceSize();i++)
	if (Tab[pos+i]!=0)
	{
	count=true;
	break;
	}


	if (count==true)
	for (i=0;i<dataX->GetSliceSize();i++)
	Q[pos+i]=(Tab[i+pos]-Min)*factor;
	else
	for (i=0;i<dataX->GetSliceSize();i++)
	Q[pos+i]=0;


	//Q+=dataX->GetSliceSize();
	pos+=dataX->GetSliceSize();
	}
	*/
	ZeroLevel=abs(Min*factor);

	Q=NULL;
	delete []Tab;
	Tab=NULL;
	return dataX;
}
//------------------------------------------
double DistanceMap::Create3DDistanceMapRawDataSet(RawDataSet *dataX)
{
	//RawDataSet *dataX=new RawDataSet(Nx,Ny,Nz,Xscale,Yscale,Zscale);
	if ((dataX->CompareWithOtherDataSet(RawData))==false) return 1;

	double *Tab=new double[ArraySize];

	int i,j,k;

	//QString name;
	//name="Distance Map";
	//double Dist;
	//dataX->GetRawDataSetName()=name+"-dist";
	//double max=0;
	int zz=0;
	short *tx,*ty,*tz;
	int p;
	//	int pp;

	double DistX,DistY,DistZ,Distance;
	int pos=0;
	double Min=0,Max=0;

	//zrobic tablice doubleowych dystansow wlacznie ze zmiana znakow
	for(k=0;k<Nz;k++)
	{
		zz+=N2xy;
		tx=TempDistanceVectorArrayX+zz;
		ty=TempDistanceVectorArrayY+zz;
		tz=TempDistanceVectorArrayZ+zz;

		for(j=0;j<Ny;j++)
			for(i=0;i<Nx;i++)
			{
				p=N2x* (j+1) + i+1;
				DistX=tx[p]*Xscale;
				DistY=ty[p]*Yscale;
				DistZ=tz[p]*Zscale;
				Distance=sqrt(DistX*DistX+DistY*DistY+DistZ*DistZ);

				Tab[pos++]=Distance;

				if (Distance>Max) Max=Distance;
				else if (Distance<Min) Min=Distance;

			}
	}


	delete []TempDistanceVectorArrayX;
	delete []TempDistanceVectorArrayY;
	delete []TempDistanceVectorArrayZ;
	maximumDistance = Max;
	double factor=Max-Min;
	factor=(factor!=0)?factor=(double)65535/factor:factor=1;
	rangeback=1.0/factor;
	//RawDataSet *dataX=new RawDataSet(Nx,Ny,Nz,Xscale,Yscale,Zscale);

	unsigned short *Q=dataX->GetDataArray();
	bool count;
	pos=0;

	dataX->ZeroQ();
	for(i=0;i<dataX->GetVolumeSize();i++)
		*(Q++)=(Tab[i]-Min)*factor;

	ZeroLevel=abs(Min*factor);

	Q=NULL;
	delete []Tab;
	Tab=NULL;
	return 1.0/factor;
	//return dataX;
}
//------------------------------------------
//RawDataSet *DistanceMap::Create2DDistanceMapRawDataSet(bool sign)
//{
//	//RawDataSet *dataX=new RawDataSet(Nx,Ny,Nz,Xscale,Yscale,Zscale);
//
//	double *Tab=new double[ArraySize];
//
//	int i,j,k;
//
//	//QString name;
//	//name="Distance Map";
//	//double Dist;
//	//dataX->GetRawDataSetName()=name+"-dist";
//	//double max=0;
//	int zz=0;
//	short *tx,*ty,*tz;
//	int p;
//	//	int pp;
//
//	double DistX,DistY,Distance;
//	int pos=0;
//	double Min=0,Max=0;
//
//	//zrobic tablice doubleowych dystansow wlacznie ze zmiana znakow
//	for(k=0;k<Nz;k++)
//	{
//		zz+=N2xy;
//		tx=TempDistanceVectorArrayX+zz;
//		ty=TempDistanceVectorArrayY+zz;
//		tz=TempDistanceVectorArrayZ+zz;
//
//		for(j=0;j<Ny;j++)
//			for(i=0;i<Nx;i++)
//			{
//				p=N2x* (j+1) + i+1;
//				DistX=tx[p]*Xscale;
//				DistY=ty[p]*Yscale;
//				Distance=sqrt(DistX*DistX+DistY*DistY);
//				if ((tz[p]!=0)&&(sign==true)) 
//					Distance*=-1.0;
//				Tab[pos++]=Distance;
//
//				if (Distance>Max) Max=Distance;
//				else if (Distance<Min) Min=Distance;
//
//			}
//	}
//
//
//	delete []TempDistanceVectorArrayX;
//	delete []TempDistanceVectorArrayY;
//	delete []TempDistanceVectorArrayZ;
//
//	double factor=Max-Min;
//
//	factor=(factor!=0)?factor=(double)65535/factor:factor=1;
//	rangeback=1.0/factor;
//	//FILE *Do_opa;
//	//Do_opa=fopen("c:\\maxy.txt","a");fprintf(Do_opa,"%lf \n",factor);fclose(Do_opa);
//
//	RawDataSet *dataX=new RawDataSet(Nx,Ny,Nz,Xscale,Yscale,Zscale);
//	unsigned short *Q=dataX->GetDataArray();
//	bool count;
//	pos=0;
//	// for (i=0;i<dataX->GetVolumeSize();i++)
//	//		Q[i]=(Tab[i+pos]-Min)*factor;
//	for (k=0;k<dataX->GetNz();k++)
//	{
//		// sprawdzic czy wszystkie pixle w danym sliceu sa zerem
//		count=false;
//		for (i=0;i<dataX->GetSliceSize();i++)
//			if (Tab[pos+i]!=0)
//			{
//				count=true;
//				break;
//			}
//
//
//			if (count==true)
//				for (i=0;i<dataX->GetSliceSize();i++)
//					Q[pos+i]=(Tab[i+pos]-Min)*factor;
//			else
//				for (i=0;i<dataX->GetSliceSize();i++)
//					Q[pos+i]=0;
//
//
//			//Q+=dataX->GetSliceSize();
//			pos+=dataX->GetSliceSize();
//	}
//
//	ZeroLevel=abs(Min*factor);
//
//	Q=NULL;
//	delete []Tab;
//	Tab=NULL;
//	return dataX;
//}
////----------------------------------
void DistanceMap::CalculateForwardUp2D()
{
	int x,y,z;
	double a,b,c,min;

	//	FILE *Do_opa;

	//forward up pass of distance map calculation
	short *tX0,*tX1,*tX2,*tX4;
	short *tY0,*tY1,*tY2,*tY4;

	int yT,zT;
	int t0,t1,t2,t4;
	zT=0;
	yT=0;

	for (z=1;z<=Nz;z++)
	{

		zT+=N2xy;
		yT=0;
		for (y=1;y<=Ny;y++)
		{
			yT+=N2x;

			t0=zT		+	yT;			//(x,y,z)
			t1=t0		-	1;			//(x-1,y,z)
			t2=t0		-	N2x;		//(x,y-1,z)
			t4=t0		+	Nx+1;		//(x+1,y,z)

			tX0=TempDistanceVectorArrayX+t0;tX1=TempDistanceVectorArrayX+t1;tX2=TempDistanceVectorArrayX+t2;tX4=TempDistanceVectorArrayX+t4;
			tY0=TempDistanceVectorArrayY+t0;tY1=TempDistanceVectorArrayY+t1;tY2=TempDistanceVectorArrayY+t2;tY4=TempDistanceVectorArrayY+t4;
			for(x=1;x<=Nx;x++)	/*right*/
			{

				tX0++;tX1++;tX2++;
				tY0++;tY1++;tY2++;


				a=sqtab_X[*tX0+infp1]	+	sqtab_Y[*tY0+infp1];
				b=sqtabPX[*tX1+infp1]	+	sqtab_Y[*tY1+infp1];
				c=sqtab_X[*tX2+infp1]	+	sqtabPY[*tY2+infp1];

				min=(a<b)?a:b;
				min=(c<min)?c:min;

				if (min==b)
				{
					*tX0=*tX1+1;
					*tY0=*tY1;

				}
				else if (min==c)
				{
					*tX0=*tX2;
					*tY0=*tY2+1;
				}

			}

			for( x=Nx;x>=1;x--)	
			{
				a=sqtab_X[*tX0+infp1]+sqtab_Y[*tY0+infp1];
				b=sqtabMX[*tX4+infp1]+sqtab_Y[*tY4+infp1];

				if (b<a)
				{
					*tX0=*tX4-1;
					*tY0=*tY4;	 
				}
				tX0--;tY0--;
				tX4--;tY4--;
			}
		}
	}

//	int nx = Nx;
//	int ny = Ny;
//	int n2x = nx + 2;
//
//	short* tempDistanceVectorArrayX = TempDistanceVectorArrayX;
//	short* tempDistanceVectorArrayY = TempDistanceVectorArrayY;
//	//int zT=0;
//#pragma omp  for  nowait ordered schedule (guided)
//	for (int y = 1; y <= ny; y++)
//	{
//
//		double a, b, c, min;
//		short* tX0, * tX1, * tX2, * tX4;
//		short* tY0, * tY1, * tY2, * tY4;
//
//		int yT, zT;
//		int t0, t1, t2, t4;
//
//		yT = n2x * y;
//
//		//yT+=n2x;
//
//		t0 = yT;			//(x,y,z)
//		t1 = t0 - 1;			//(x-1,y,z)
//		t2 = t0 - n2x;		//(x,y-1,z)
//		t4 = t0 + nx + 1;		//(x+1,y,z)
//
//		tX0 = tempDistanceVectorArrayX + t0; tX1 = tempDistanceVectorArrayX + t1; tX2 = tempDistanceVectorArrayX + t2; tX4 = tempDistanceVectorArrayX + t4;
//		tY0 = tempDistanceVectorArrayY + t0; tY1 = tempDistanceVectorArrayY + t1; tY2 = tempDistanceVectorArrayY + t2; tY4 = tempDistanceVectorArrayY + t4;
//		for (int x = 1; x <= nx; x++)	/*right*/
//		{
//
//			tX0++; tX1++; tX2++;
//			tY0++; tY1++; tY2++;
//
//
//			a = sqtab_X[*tX0 + infp1] + sqtab_Y[*tY0 + infp1];
//			b = sqtabPX[*tX1 + infp1] + sqtab_Y[*tY1 + infp1];
//			c = sqtab_X[*tX2 + infp1] + sqtabPY[*tY2 + infp1];
//
//			min = (a < b) ? a : b;
//			min = (c < min) ? c : min;
//
//			if (min == b)
//			{
//				*tX0 = *tX1 + 1;
//				*tY0 = *tY1;
//
//			}
//			else if (min == c)
//			{
//				*tX0 = *tX2;
//				*tY0 = *tY2 + 1;
//			}
//
//		}
//
//		for (int x = nx; x >= 1; x--)
//		{
//			a = sqtab_X[*tX0 + infp1] + sqtab_Y[*tY0 + infp1];
//			b = sqtabMX[*tX4 + infp1] + sqtab_Y[*tY4 + infp1];
//
//			if (b < a)
//			{
//				*tX0 = *tX4 - 1;
//				*tY0 = *tY4;
//			}
//			tX0--; tY0--;
//			tX4--; tY4--;
//		}
//	}
}

//---------------------------------------------------------------------------------

void DistanceMap::CalculateBackwardDown2D()
{
//	int nx = Nx;
//	int ny = Ny;
//	int n2x = nx + 2;
//
//	short* tempDistanceVectorArrayX = TempDistanceVectorArrayX;
//	short* tempDistanceVectorArrayY = TempDistanceVectorArrayY;
//#pragma omp  for  nowait ordered schedule (guided)
//	for (int y = ny; y >= 1; y--)
//	{
//		double a, b, c, min;
//
//		short* tX0, * tX1, * tX2, * tX4;
//		short* tY0, * tY1, * tY2, * tY4;
//
//		int yT;
//		int t0, t1, t2, t4;
//
//		yT = n2x * y;
//		//yT-=n2x;
//		t0 = yT + 1 + nx;		//(x,y,z)
//		t1 = t0 + 1;				//(x+1,y,z)
//		t2 = t0 + n2x;			//(x,y+1,z)
//
//		t4 = yT;		//(x-1,y,z)
//
//		tX0 = tempDistanceVectorArrayX + t0; tX1 = tempDistanceVectorArrayX + t1; tX2 = tempDistanceVectorArrayX + t2; tX4 = tempDistanceVectorArrayX + t4;
//		tY0 = tempDistanceVectorArrayY + t0; tY1 = tempDistanceVectorArrayY + t1; tY2 = tempDistanceVectorArrayY + t2; tY4 = tempDistanceVectorArrayY + t4;
//		for (int x = nx; x >= 1; x--)
//		{
//			tX0--; tX1--; tX2--;
//			tY0--; tY1--; tY2--;
//
//			a = sqtab_X[*tX0 + infp1] + sqtab_Y[*tY0 + infp1];
//			b = sqtabMX[*tX1 + infp1] + sqtab_Y[*tY1 + infp1];
//			c = sqtab_X[*tX2 + infp1] + sqtabMY[*tY2 + infp1];
//
//			min = (a < b) ? a : b;
//			min = (c < min) ? c : min;
//
//			if (min == b)
//			{
//				*tX0 = *tX1 - 1;
//				*tY0 = *tY1;
//			}
//			else if (min == c)
//			{
//				*tX0 = *tX2;
//				*tY0 = *tY2 - 1;
//			}
//
//		}
//		for (int x = 1; x <= nx; x++)
//		{
//			a = sqtab_X[*tX0 + infp1] + sqtab_Y[*tY0 + infp1];
//			b = sqtabPX[*tX4 + infp1] + sqtab_Y[*tY4 + infp1];
//			if (b < a)
//			{
//				*tX0 = *tX4 + 1;
//				*tY0 = *tY4;
//
//			}
//			tX0++; tY0++;
//			tX4++; tY4++;
//		}
//	}
	double a,b,c,min;
	int x,y,z;
	//	FILE *Do_opa;
	//backward down pass of distance map calculation
	short *tX0,*tX1,*tX2,*tX4;
	short *tY0,*tY1,*tY2,*tY4;

	int yT,zT;
	int t0,t1,t2,t4;
	int yTtemp=N2x*(Ny+1);
	int zTemp=N2xy*(Nz+1);
	zT=zTemp;

	for (z=Nz;z>=1;z--)
	{
		zT-=N2xy;
		yT=yTtemp;
		for (y=Ny;y>=1;y--)
		{
			yT-=N2x;
			t0=zT		+	yT+1+Nx;		//(x,y,z)
			t1=t0		+	1;				//(x+1,y,z)
			t2=t0		+	N2x;			//(x,y+1,z)

			t4=zT		+	yT		;		//(x-1,y,z)

			tX0=TempDistanceVectorArrayX+t0;tX1=TempDistanceVectorArrayX+t1;tX2=TempDistanceVectorArrayX+t2;tX4=TempDistanceVectorArrayX+t4;
			tY0=TempDistanceVectorArrayY+t0;tY1=TempDistanceVectorArrayY+t1;tY2=TempDistanceVectorArrayY+t2;tY4=TempDistanceVectorArrayY+t4;
			for(x=Nx;x>=1;x--)
			{
				tX0--;tX1--;tX2--;
				tY0--;tY1--;tY2--;

				a=sqtab_X[*tX0+infp1]	+	sqtab_Y[*tY0+infp1]	;
				b=sqtabMX[*tX1+infp1]	+	sqtab_Y[*tY1+infp1]	;
				c=sqtab_X[*tX2+infp1]	+	sqtabMY[*tY2+infp1]	;

				min=(a<b)?a:b;
				min=(c<min)?c:min;

				if (min==b)
				{
					*tX0=*tX1-1;
					*tY0=*tY1;
				}
				else if (min==c)
				{
					*tX0=*tX2;
					*tY0=*tY2-1;		
				}

			}
			for( x=1;x<=Nx;x++)	
			{
				a=sqtab_X[*tX0+infp1]+sqtab_Y[*tY0+infp1];
				b=sqtabPX[*tX4+infp1]+sqtab_Y[*tY4+infp1];
				if (b<a)
				{
					*tX0=*tX4+1;
					*tY0=*tY4;

				}
				tX0++;tY0++;
				tX4++;tY4++;
			}
		}
	}
}

//--------------------------------------------------------------------

void DistanceMap::CalculateDistance()
{
	CalculateDistanceArrays();
	CreateOutputArrays();
}
//--------------------------------------
void DistanceMap::CalculateDistanceArrays()
{
	if (is2D == false)
	{


		CalculateForwardUp();
		CalculateBackwardDown();
		CalculateForwardUp();
	}
	else
	{
		CalculateForwardUp2D();
		CalculateBackwardDown2D();
	}

}
//------------------------------------
//-----------------------------------
RawDataSet *DistanceMap::Generate2DUnsignedDistanceMapVolume(bool createVolume)
{

	//		FILE *Do_opa;
	int i;

	infp1=infinity+1;
	TempDistanceVectorArrayX=new short[TempArraySize];
	TempDistanceVectorArrayY=new short[TempArraySize];
	TempDistanceVectorArrayZ=new short[TempArraySize]; // tu bede trzymal oryginal
	for (i=0;i<N2xy;i++)
		TempDistanceVectorArrayX[i]=infinity;
	int tempol=N2xy;
	for(i=0;i<=Nz;i++)
	{
		memcpy(TempDistanceVectorArrayX+tempol,TempDistanceVectorArrayX,N2xy*sizeof(short));
		tempol+=N2xy;	
	}

	int DistDataPoints;
	if (RawData==NULL)
		DistDataPoints=FillTempDistArrayXWithPolyDataPoints(TempDistanceVectorArrayX);
	else
	{
		if (Mask==NULL)
			DistDataPoints=FillTempDistArrayXWithRawDataPoints(TempDistanceVectorArrayX);
		else
			DistDataPoints=FillTempDistArrayXWithMaskPoints(TempDistanceVectorArrayX);

	}
	/////////////////
	//stworze sobie ta trzecia w innym celu zeby trzymac tam oryginal

	/*for(i=0;i<TempArraySize;i++)
	{
	TempDistanceVectorArrayX[i]=((TempDistanceVectorArrayX[i])>0)?0:infinity;
	}*/

	memcpy(TempDistanceVectorArrayZ,TempDistanceVectorArrayX,TempArraySize*sizeof(short));
	// x i y beda zmodyfikowane zeby zawierac tylko obrys
	//FindBoundaries(TempDistanceVectorArrayX,TempDistanceVectorArrayZ);
	memcpy(TempDistanceVectorArrayY,TempDistanceVectorArrayX,TempArraySize*sizeof(short));
	////////////////

	numberOfSeeds=DistDataPoints;
	if (DistDataPoints>0)
	{
		//	int infinity1=infinity+1;
		sqtab_X	=	new double[2*infp1+1];
		sqtabPX	=	new double[2*infp1+1];
		sqtabMX	=	new double[2*infp1+1];

		sqtab_Y	=	new double[2*infp1+1];
		sqtabPY	=	new double[2*infp1+1];
		sqtabMY	=	new double[2*infp1+1];
		//	
		sqtab_Z	=	new double[2*infp1+1];
		sqtabPZ	=	new double[2*infp1+1];
		sqtabMZ	=	new double[2*infp1+1];
		//

		double tmp0;
		double tmp1;
		double tmp2;
		int pos=0;
		for(i=-infp1;i<=infp1;i++)
		{
			tmp0=(i-1)*(i-1);
			tmp1=i*i;
			tmp2=(i+1)*(i+1);

			sqtabMX[pos]	=tmp0*XSQscale;
			sqtab_X[pos]	=tmp1*XSQscale;
			sqtabPX[pos]	=tmp2*XSQscale;

			sqtabMY[pos]	=tmp0*YSQscale;
			sqtab_Y[pos]	=tmp1*YSQscale;
			sqtabPY[pos]	=tmp2*YSQscale;

			sqtabMZ[pos]	=tmp0*ZSQscale;
			sqtab_Z[pos]	=tmp1*ZSQscale;
			sqtabPZ[pos]	=tmp2*ZSQscale;

			pos++;

		}
		Calculate2DDistanceArrays();
		//memcpy(TempDistanceVectorArrayZ,TempDistanceVectorArrayX,TempArraySize*sizeof(short));
		//CalculateDistance();



		delete	sqtab_X;
		delete	sqtab_Y;
		delete	sqtab_Z;
		delete	[]sqtabPX;
		delete	[]sqtabPY;
		delete	[]sqtabPZ;
		delete	[]sqtabMX;
		delete	[]sqtabMY;
		delete	[]sqtabMZ;

		sqtab_X=NULL;
		sqtab_Y=NULL;
		sqtab_Z=NULL;
		sqtabPX=NULL;
		sqtabPY=NULL;
		sqtabPZ=NULL;
		sqtabMX=NULL;
		sqtabMY=NULL;
		sqtabMZ=NULL;

		RawDataSet *vol = NULL;
		if (createVolume==true) vol=Create2DDistanceMapRawDataSet(false); else CreateOutputArrays();
		return vol;

	}
	else
	{
		return NULL;
		// cuboid pusty - zrob z tym cos Michalek :)
	}

	//		FILE *Do_opa;
	/*int i;

	infp1=infinity+1;
	TempDistanceVectorArrayX=new short[TempArraySize];
	TempDistanceVectorArrayY=new short[TempArraySize];
	TempDistanceVectorArrayZ=new short[TempArraySize];
	for (i=0;i<N2xy;i++)
	TempDistanceVectorArrayX[i]=infinity;
	int tempol=N2xy;
	for(i=0;i<=Nz;i++)
	{
	memcpy(TempDistanceVectorArrayX+tempol,TempDistanceVectorArrayX,N2xy*sizeof(short));
	tempol+=N2xy;	
	}

	int DistDataPoints;
	if (RawData==NULL)
	DistDataPoints=FillTempDistArrayXWithPolyDataPoints(TempDistanceVectorArrayX);
	else
	{
	if (Mask==NULL)
	DistDataPoints=FillTempDistArrayXWithRawDataPoints(TempDistanceVectorArrayX);
	else
	DistDataPoints=FillTempDistArrayXWithMaskPoints(TempDistanceVectorArrayX);

	}
	FindBoundaries3D(TempDistanceVectorArrayZ,TempDistanceVectorArrayX);
	memcpy(TempDistanceVectorArrayX,TempDistanceVectorArrayZ,TempArraySize*sizeof(short));
	memcpy(TempDistanceVectorArrayY,TempDistanceVectorArrayZ,TempArraySize*sizeof(short));

	numberOfSeeds=DistDataPoints;
	if (DistDataPoints>0)
	{
	//	int infinity1=infinity+1;
	sqtab_X	=	new double[2*infp1+1];
	sqtabPX	=	new double[2*infp1+1];
	sqtabMX	=	new double[2*infp1+1];

	sqtab_Y	=	new double[2*infp1+1];
	sqtabPY	=	new double[2*infp1+1];
	sqtabMY	=	new double[2*infp1+1];

	sqtab_Z	=	new double[2*infp1+1];
	sqtabPZ	=	new double[2*infp1+1];
	sqtabMZ	=	new double[2*infp1+1];

	double tmp0;
	double tmp1;
	double tmp2;
	int pos=0;
	for(i=-infp1;i<=infp1;i++)
	{
	tmp0=(i-1)*(i-1);
	tmp1=i*i;
	tmp2=(i+1)*(i+1);

	sqtabMX[pos]	=tmp0*XSQscale;
	sqtab_X[pos]	=tmp1*XSQscale;
	sqtabPX[pos]	=tmp2*XSQscale;

	sqtabMY[pos]	=tmp0*YSQscale;
	sqtab_Y[pos]	=tmp1*YSQscale;
	sqtabPY[pos]	=tmp2*YSQscale;

	sqtabMZ[pos]	=tmp0*ZSQscale;
	sqtab_Z[pos]	=tmp1*ZSQscale;
	sqtabPZ[pos]	=tmp2*ZSQscale;

	pos++;
	//	Do_opa=fopen("c:\\dd.txt","a");fprintf(Do_opa,"%d (%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf) \n",i,sqtabMX[i],sqtabPX[i],sqtab_X[i],sqtabMY[i],sqtabPY[i],sqtab_Y[i],sqtabMZ[i],sqtabPZ[i],sqtab_Z[i]);fclose(Do_opa);

	}

	CalculateDistanceArrays();

	delete	sqtab_X;
	delete	sqtab_Y;
	delete	sqtab_Z;
	delete	[]sqtabPX;
	delete	[]sqtabPY;
	delete	[]sqtabPZ;
	delete	[]sqtabMX;
	delete	[]sqtabMY;
	delete	[]sqtabMZ;

	sqtab_X=NULL;
	sqtab_Y=NULL;
	sqtab_Z=NULL;
	sqtabPX=NULL;
	sqtabPY=NULL;
	sqtabPZ=NULL;
	sqtabMX=NULL;
	sqtabMY=NULL;
	sqtabMZ=NULL;

	return Create3DDistanceMapRawDataSet();
	}
	else
	{
	return NULL;
	// cuboid pusty - zrob z tym cos Michalek :)
	}*/

}

bool	DistanceMap::Get2DPos(int x, int y, int&x1, int&y1)
{

	if (DistanceVectorArrayX != NULL)
	{
		int pos = x + y*Nx;
		if ((pos >= 0) && (x < Nx) && (y < Ny))
		{
			x1 = DistanceVectorArrayX[pos];
			y1 = DistanceVectorArrayY[pos];
			return true;
		}
		else return false;
	}
	else return false;
}