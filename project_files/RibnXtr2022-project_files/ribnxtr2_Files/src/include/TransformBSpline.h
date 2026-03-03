#ifndef BSPLINE_TRANSFORM
#define BSPLINE_TRANSFORM


//#include	"OtherCommonFunctions.h"
#include	"Grid3D.h"

	class BSplineGridTransformation
{
	public:

		BSplineGridTransformation(FloatGrid3D *_Grid,double _Nx,double _Ny,double _Nz,double _minx=0.0, double _miny=0.0,double _minz=0.0);
		~BSplineGridTransformation();

		void SetupForwardAndBackwardScalingFactors(double fx,double sy,double fz,double bx,double by,double bz);
		void TransformPoint(double *PIn,double *POut,double *def=NULL);
		void TransformPoint(double *PIn,double *POut,double amount,double *def=NULL);
		void TransformPoint(int x,int y, int z,int *POut);
		void TransformPoint(int x,int y, int z,double *POut);

		void TransformPont(int i,int j,int k,double *Point,double x,double y,double z,int NodX,int NodY,int NodZ);
		void TransformPoint(int *dataPos,double *def,int *nodPos,double *ddDdPL);
		// specjalna dla rejestracji zbioru
		void TransformPoint(	int *dataPos,double *def,bool calcDerivs=false,int *nodPos=NULL,
								double *ddDdP=NULL, double *ddDdPL=0,double *ddDdQ=NULL,
								double *ddDPdQ=NULL);//,double *ddDyQ=NULL,double *ddDzQ=NULL);

		void TransformPoint(	int *nodPos,int *dataPos,double *def, double *ddDdQ);


void TransformPoint(double *PIn,double *def,int *nodPos,double *ddDdPL);
void TransformPoint(	int *nodPos,double *PIn,double *def, double *ddDdQ);
void TransformPoint(	double *PIn,double *def,bool calcDerivs,int *nodPos,
											   double *ddDdP,double *ddDdPL, double *ddDdQ,
											   double *ddDPdQ);


		inline void Deformation(double *PIn,double *PDef);
		void DeformationOrig(double x,double y,double z,double *PDef);
		void ScaleGrid(FloatGrid3D *_Grid);
		void DeformGrid(FloatGrid3D *_Grid);
		double InvertGrid();


		void ScaleGrid2D(FloatGrid3D *_Grid);
		void TransformPoint2D(int *dataPos,double *def,int *nodPos,double *ddDdPL);

		void TransformPoint2D(	int *dataPos,double *def,bool calcDerivs=false,int *nodPos=NULL,
								double *ddDdP=NULL, double *ddDdPL=0,double *ddDdQ=NULL,
								double *ddDPdQ=NULL);//,double *ddDyQ=NULL,double *ddDzQ=NULL);
		void TransformPont2D(int i,int j,double *Point,double x,double y,int NodX,int NodY,int NodZ);
		void DeformationOrig(double xx,double yy,double *def0);
		void TransformPoint2D(int x,int y, double *POut);
		void TransformPoint2D(double x,double y, double *POut);
		//	inline void Deformation(int x,int y,int z,int dx,int dy, int dz,double *PDef);


	
		inline double GetFNx(){return Nx;}
		inline double GetFNy(){return Ny;}
		inline double GetFNz(){return Nz;}

		double *SplineTab;	
		double *SplineDerivTab;

		inline double GetGxDivNx(){return N1x;};
		inline double GetGyDivNy(){return N1y;};
		inline double GetGzDivNz(){return N1z;};


		double GetDerivAtPoint(double x,double y,double z,  int px,int py,int pz);
		double GetDerivAtPoint(int x,int y,int z, int px,int py,int pz);

		inline FloatGrid3D* GetGrid(){return Grid;};
		

	private:

		FloatGrid3D *Grid;
		int GridNx,GridNy,GridNz;
		double Nx,Ny,Nz; //rozmiar bb powierzchni 
		double minx,miny,minz;

		double N1x,N1y,N1z;
		double xScale,yScale,zScale;
		int GridNx2,GridNy2,GridNz2;
		double xSc,ySc,zSc;


		double B0(double u){return (1.0-u)*(1.0-u)*(1-u)/6.0;}
		double B1(double u){return (3.0*u*u*u-6.0*u*u+4.0)/6.0;}
		double B2(double u){return (-3.0*u*u*u +3.0*u*u +3.0*u+1.0)/6.0;}
		double B3(double u){return u*u*u/6.0;}

		double dB0(double u){return -(1-u)*(1-u)/2.0;}
		double dB1(double u){return (3.0*u*u-2*u)/2.0;}
		double dB2(double u){return (-3.0*u*u +2.0*u +u)/2.0;}
		double dB3(double u){return u*u/2.0;}

		double (BSplineGridTransformation::*BSpline[4])(double);
		double (BSplineGridTransformation::*BSplineDeriv[4])(double);
};
//------------------------------
inline void BSplineGridTransformation::Deformation(double *PIn,double *PDef)
{

	double P[3];
	P[0]=(PIn[0]-minx)*N1x;//(PIn[0]-minx)*(GridNx-1)/Nx; 
	P[1]=(PIn[1]-miny)*N1y;//(PIn[1]-miny)*(GridNy-1)/Ny;
	P[2]=(PIn[2]-minz)*N1z;//(PIn[2]-minz)*(GridNz-1)/Nz;
	//P[0]=(PIn[0])*N1x;//(PIn[0]-minx)*(GridNx-1)/Nx; 
	//P[1]=(PIn[1])*N1y;//(PIn[1]-miny)*(GridNy-1)/Ny;
	//P[2]=(PIn[2])*N1z;//(PIn[2]-minz)*(GridNz-1)/Nz;
	double def0[3];

	int a=1;
	if ((P[0]<0)||(P[1]<0)||(P[2]<0))
	{
		a=a;
	}
	DeformationOrig(P[0],P[1],P[2],def0);


	PDef[0]=def0[0]*xScale;
	PDef[1]=def0[1]*yScale;
	PDef[2]=def0[2]*zScale;

}


#endif //BSPLINE_TRANSFORM
