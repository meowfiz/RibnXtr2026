#include "Interpolation.h"

Interpolation::Interpolation(RawDataSet *Data,InterpolationFunc *f)
{
		func=f;
		fsize=func->GetSize();
		ksize=fsize<<1;

	    nx=Data->GetNx();
		ny=Data->GetNy();
		nz=Data->GetNz();
		slicesize=Data->GetSliceSize();
		tab0=Data->GetDataArray();
		volumeSize = Data->GetVolumeSize();

		tx=NULL;
		ty=NULL;
		tz=NULL;
		if (fsize>0)
		{
			tx=new double[ksize];
			ty=new double[ksize];
			tz=new double[ksize];
		}
		

}

Interpolation::Interpolation(int _nx, int _ny, int _nz, unsigned short *_tab,InterpolationFunc *f)
{
		func=f;
		fsize=func->GetSize();
		ksize=fsize<<1;

	    nx=_nx;
		ny=_ny;
		nz=_nz;
		slicesize=nx*ny;
		tab0=_tab;
		volumeSize = nx * ny * nz;

		tx=NULL;
		ty=NULL;
		tz=NULL;

		if (fsize>0)
		{
			tx=new double[ksize];
			ty=new double[ksize];
			tz=new double[ksize];
		}
	
}
//--------------------------------------------------------
Interpolation::~Interpolation()
{
	if (tx!=NULL)
		delete []tx;
	if (ty!=NULL)
		delete []ty;	
	if (tz!=NULL)
		delete []tz;
	
	tx=NULL;
	ty=NULL;
	tz=NULL;
}
//--------------------------------------------------------

//--------------------------------------------------------
double Interpolation::Interpolate3D(double x, double y, double z,unsigned short *_tab)
{

	unsigned short *tab=(_tab==NULL)?tab0:_tab;

	if (fsize==0)
		return  *(tab+(int)(z+0.5)*slicesize + (int)(y+0.5)*nx + (int)(x+0.5));
	else
	{
		
		int k;
		int j;

		int step=slicesize;
		int posZ = ((int)z - fsize + 1)*slicesize;

		//if ((posZ<0)||(posZ>)

		//tab+=((int)z-fsize+1)*slicesize;
	
		for(k=0;k<ksize;k++)
		{
			if ((posZ >= 0) && (posZ < volumeSize - slicesize))
				tz[k] = Interpolate2D(x, y, tab + posZ);
			else
				tz[k] = 0;

			//tab+=step;
			posZ += step;
		}
		return func->GetValue(tz,z);
		

	}

	return 0;
}
//--------------------------------------------------------
double Interpolation::Interpolate2D(double x, double y,unsigned short * _tab)
{
	int step;
	//unsigned short *tab=(tab0==NULL)?tab0:_tab;
	unsigned short *tab = (_tab == NULL) ? tab0 : _tab;
	if (fsize==0)
		return *(tab+(int)(y+0.5)*nx + (int)(x+0.5));
	else
	{
		//if (fsize == 1)
		//{
		//	
		//	int xI = (int)xI;
		//	int yI = (int)yI;
		//	int pos = (int)yI*nx + xI;

		//	if ((xI<nx-1)&&(yI<ny-1))
		//	//if ((pos >= 0) && (pos < slicesize-1))
		//	{
		//
		//		tab += pos;
		//		step = nx - 1;
		//		tx[0] = *(tab++);
		//		tx[1] = *(tab);
		//		ty[0] = func->GetValue(tx, x);
		//		tab += step;
		//		tx[0] = *(tab++);
		//		tx[1] = *(tab);
		//		ty[1] = func->GetValue(tx, x);
		//		return func->GetValue(ty, y);
		//	}
		//	else return 0;

		//}
		//else
		{
			int i,j;

			int step=nx-ksize;
			int pos = ((int)y - fsize + 1)*nx + ((int)x - fsize + 1);
			//tab+=posY;
			for(j=0;j<ksize;j++)
			{
				for (i = 0; i < ksize; i++)
				{
					if ((pos >= 0) && (pos < slicesize))
						tx[i] = *(tab + pos);
					else
						tx[i] = 0;
					pos++;
				}
				ty[j]=func->GetValue(tx,x);
				//tab+=step;
				pos += step;
			}
			return func->GetValue(ty,y);
		}
	}
	return 0;
}
//--------------------------------------------------------
//--------------------------------------------------------
double Interpolation::Interpolate1DZ(int ix, int iy, double z)
{
	//wersja specjalna tylko interpolacja w osi Z
	unsigned short *tab=tab0;

	if (fsize==0)
		return  *(tab+(int)(z+0.5)*slicesize + iy*nx + ix);
	else
	{
		int k;
		int j;

		int step=slicesize;
		int pos = ((int)z - fsize + 1) * slicesize + iy * nx + ix;
		//tab+=;

		for(k=0;k<ksize;k++)
		{
			if ((pos<0)||(pos>=volumeSize))
			{
				tz[k] = 0;
			}
			else
			{
				tz[k] = tab[pos];
				pos += step;
			}
			
			//tab+=step;
		}
		return func->GetValue(tz,z);
	}
	return 0;
}
//--------------------------------------------------------
//--------------------------------------------------------