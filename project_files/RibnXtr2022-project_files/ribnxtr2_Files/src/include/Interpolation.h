#ifndef	INTERPOLATION
#define INTERPOLATION

#include "OtherCommonFunctions.h"
#include "RawDataSet.h"


inline double  sinc(double x)
{
	return (x != 0.0) ? sin(M_PI * x) / (M_PI * x) : 1.0;
}

class InterpolationFunc
{
public:
	virtual double GetValue(double *,double)=0;
	int GetSize(){return size;};
protected:
	int size;
};
//------------------------------------
class NearestNeighborInterpolation: public InterpolationFunc
{
public:
	NearestNeighborInterpolation(){size=0;};

	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;
		return (double)(*tab);	
	};
};
//------------------------------------
class LinearInterpolation: public InterpolationFunc
{
public:
	LinearInterpolation(){size=1;};
	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;
		return(tab[0]*(1.0-x)+tab[1]*x);

	};
};
//------------------------------------
class CosineInterpolation: public InterpolationFunc
{
public:
	CosineInterpolation(){size=1;};
	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;

		x = (1.0 - cos(x*M_PI)) / 2;
		return(tab[0]*(1.0-x)+tab[1]*x);

	};
};
//------------------------------------
class Cubic4Interpolation: public InterpolationFunc
{
public:
	Cubic4Interpolation(){size=2;};
	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;

		double A = tab[0];
		double B = tab[1];
		double C = tab[2];
		double D = tab[3];

		double x2 = x * x;
		double a0 = D - C - A + B;
		double a1 = A - B - a0;
		double a2 = C - A;
		double a3 = B;

		double w=a0 * x * x2 + a1 * x2 + a2 * x + a3;

		if (w < 0) return 0; else if (w > 65535) return 65535; else return w;
	};
};
//------------------------------------
class Spline6Interpolation: public InterpolationFunc
{
public:
	Spline6Interpolation(){size=3;};
	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;
		double p0 = tab[0];
		double p1 = tab[1];
		double p2 = tab[2];
		double p3 = tab[3];
		double p4 = tab[4];
		double p5 = tab[5];

		double w = p2 + 0.04166666666 * x * ((p3 - p1) * 16.0 + (p0 - p4) * 2.0
			+ x * ((p3 + p1) * 16.0 - p0 - p2 * 30.0 - p4
			+ x * (p3 * 66.0 - p2 * 70.0 - p4 * 33.0 + p1 * 39.0 + p5 * 7.0- p0 * 9.0
			+ x * (p2 * 126.0 - p3 * 124.0 + p4 * 61.0 - p1 * 64.0 - p5 * 12.0 + p0 * 13.0
			+ x * ((p3 - p2) * 50.0 + (p1 - p4) * 25.0 + (p5 - p0) * 5.0)))));

		if (w < 0) return 0; else if (w > 65535) return 65535; else return w;

	};
};

//------------------------------------
class WelchSincInterpolation: public InterpolationFunc
{
public:
	WelchSincInterpolation(){size=3;size2m1=1.0/(size*size);size2=size<<1;};
	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;

		double  w = 0.0;
		double X;
		int i;
		/// 1-(x/s)^2
		for(i=0;i<size2;i++)
		{
			X=x-i+size-1;
			w+=tab[i]*sinc(X)*(1-(X*X)*size2m1);
		}

		if (w < 0) return 0; else if (w > 65535) return 65535; else return w;
	};
	double size2m1;
	int size2;
};
//------------------------------------
class HannHammingSincInterpolation: public InterpolationFunc
{
public:
	HannHammingSincInterpolation(){size=4;size2=size<<1;};

	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;

		double  w = 0.0;
		double X;
//		double t;
//		double s;
		double p=0.5;

		for(int i = 0; i < size2; i ++)
		{
			X=x-i+size-1;
			w += tab[i] * sinc(X)*(p + (1 - p)*cos(M_PI*X / size));
		}
		if (w < 0) return 0; else if (w > 65535) return 65535; else return w;
	};

	double size2;
};
//-------------------------------------------
class LanczosSincInterpolation: public InterpolationFunc
{
public:
	LanczosSincInterpolation(){size=4;size2=size<<1;};

	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;

		double  w = 0.0;
		double X;
//		double t;
//		double s;
//		double p=0.5;

		for(int i = 0; i < size2; i ++)
		{
			X=x-i+size-1;
			 w+=(double)tab[i]*sinc(X/size) * sinc(X);
		}
		if (w < 0) return 0; else if (w > 65535) return 65535; else return w;
	};

	double size2;
};
//-------------------------------------------
class CatmullRomSplineInterpolation: public InterpolationFunc
{
public:
	CatmullRomSplineInterpolation(){size=2;};

	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;
		double A = tab[0];
		double B = tab[1];
		double C = tab[2];
		double D = tab[3];
		double x2 = x * x;

		double w = 0.5 * ((2.0 * B) + (- A + C) * x
                + (2.0 * A - 5.0 * B + 4.0 * C - D) * x2
                + (- A + 3.0 * B - 3.0 * C + D) * x2 * x);


		if (w < 0) return 0; else if (w > 65535) return 65535; else return w;
	};

	
};
//-------------------------------------------
class HermiteLaurentInterpolation: public InterpolationFunc
{
public:
	HermiteLaurentInterpolation(){size=2;};

	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;
		double A = tab[0];
		double B = tab[1];
		double C = tab[2];
		double D = tab[3];
        double c1 = 0.5f * (C - A);
        double AmB = A - B;
        double c3 = (B - C) + 0.5f * (D - AmB - C);
        double c2 = AmB + c1 - c3;

        double w = ((c3 * x + c2) * x + c1) * x + B;

		if (w < 0) return 0; else if (w > 65535) return 65535; else return w;
	};

};
//-------------------------------------------
class HermiteInterpolation: public InterpolationFunc
{
public:
	HermiteInterpolation(){size=2;tension=1.0;bias=1.0;};

	inline double GetValue(double *tab,double x)
	{
		x=x-(int)x;
		double A = tab[0];
		double B = tab[1];
		double C = tab[2];
		double D = tab[3];
      
 		double m0, m1, mu2, mu3;
        double a0, a1, a2, a3;

		mu2 = x * x;
		mu3 = mu2 * x;
        m0 = (B - A) * (1.0 + bias) * (1.0 - tension) / 2.0;
        m0 += (C - B) * (1.0 - bias) * (1.0 - tension) / 2.0;
        m1 = (C - B) * (1.0 + bias) * (1.0 - tension) / 2.0;
        m1 += (D - C) * (1.0 - bias) * (1.0 - tension) / 2.0;

        a0 = 2.0 * mu3 - 3.0 * mu2 + 1.0;
        a1 = mu3 - 2.0 * mu2 + x;
        a2 = mu3 - mu2;
        a3 = -2.0 * mu3 + 3.0 * mu2;

        double w = a0 * B + a1 * m0 + a2 * m1 + a3 * C;

		if (w < 0) return 0; else if (w > 65535) return 65535; else return w;
	};
	double tension;
	double bias;

};
//------------------------------------
//------------------------------------
//------------------------------------
//------------------------------------
//------------------------------------
class Interpolation
{
public:
	Interpolation(RawDataSet *RawData,InterpolationFunc *f);
	Interpolation(int _nx, int _ny, int _nz, unsigned short *_tab,InterpolationFunc *f);
	~Interpolation();


	double Interpolate1DZ(int ix, int iy, double z);
	double Interpolate3D(double x, double y, double z,unsigned short *_tab=NULL);
	double Interpolate2D(double x, double y,unsigned short *_tab=NULL);

private:
	InterpolationFunc *func;
	int fsize;
	double *tx,*ty,*tz;
	unsigned short *tab0;
	int nx,ny,nz,slicesize;
	int ksize;	
	int volumeSize;

};



#endif //INTERPOLATION
