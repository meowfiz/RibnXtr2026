#ifndef DISTANCE_MAP_H
#define DISTANCE_MAP_H


#include <stdio.h>
#include <math.h>
#include <qstring.h>
#include <qfile.h>

#include	"RawDataSet.h"
#include	"OtherCommonFunctions.h"
#include	"SurfaceCollection.h"
#include	"LinkedList.h"


class RawDataSet;

class DistanceMap  
{
		double   Xscale,Yscale,Zscale;
	public:

		bool	Get2DPos(int x, int y, int&x1, int&x2);

		DistanceMap(vtkPoints *pts,double *Pos,int *Size,double *Scale);
		DistanceMap(LinkedList<PolygonSurfaceData*>	*SurfaceDataList,double *Pos,int *Size,double *Scale);
		DistanceMap(RawDataSet	*Data,int _LThreshold,int _RThreshold,unsigned char *_Mask=NULL,unsigned char *clearMask=NULL);

		DistanceMap();	
		~DistanceMap();

		void GetScale(double scale[3]){scale[0]=Xscale;	scale[1]=Yscale;	scale[2]=Zscale;};
	RawDataSet	*Generate2DSignedDistanceMapVolume(bool createVolume = true);
	RawDataSet	*Generate3DUnsignedDistanceMapVolume(bool clear=false);
	RawDataSet	*Generate2DUnsignedDistanceMapVolume(bool createVolume=true);


	bool	PrecalucateDistanceMap(void);
	void	SetVoxelSize(double xs,double ys,double zs);
	void	GetDistanceValues(double x,double y,double z,double &Distance,double &DistX,double &DistY,double &DistZ);
	double	GetDistanceValue(double x,double y,double z);
	void	SaveDistanceMap( const QString &filename );
	void	LoadDistanceMap( const QString &filename );

	double GetMaximumDistance()
	{ 
		double xx = Nx * Xscale;
		double yy = Ny * Yscale;
		double zz = Nz * Zscale;
		maximumDistance = sqrt(xx * xx + yy * yy + zz * zz)*0.01;
		
		return maximumDistance; 
	
	};
	int GetNx(){return Nx;};
	int GetNy(){return Ny;};
	int GetNz(){return Nz;};

	double GetX0M(){return x0M;};
	double GetY0M(){return y0M;};
	double GetZ0M(){return z0M;};
	
	int		GetLThreshold(){return 	LThreshold;};
	int		GetRThreshold(){return  RThreshold;};
	int		GetNumberOfSeeds(){return  numberOfSeeds;};
	const RawDataSet		*GetRawData(){return 	RawData;};
	int		GetZeroLevel(){return  ZeroLevel;};
	RawDataSet *Create3DDistanceMapRawDataSet(bool clear=false);
	RawDataSet* Create2DDistanceMapRawDataSet(bool clear = false);
	RawDataSet* CreateDistanceMapRawDataSet(bool clear = false);
	double	Create3DDistanceMapRawDataSet(RawDataSet *dataX);
	double	Generate3DUnsignedDistanceMapVolume(RawDataSet *rdata);

	double	GetRangeBack(){return rangeback;};

	private:
		bool is2D;
	vtkPoints *sPoints;
	unsigned char *Mask;
	int ZeroLevel;

	RawDataSet *RawData;

	PolygonSurfaceData *SurfaceData;
	LinkedList<PolygonSurfaceData*>	*SurfaceDataList;

	int		LThreshold,RThreshold;

	double 	XSQscale,YSQscale,ZSQscale;

	double	XReal,YReal,ZReal;
	
	double	*sqtab_X,*sqtab_Y,*sqtab_Z;
	double	*sqtabPX,*sqtabPY,*sqtabPZ;
	double	*sqtabMX,*sqtabMY,*sqtabMZ;

	int		sqtablength;
	double	max;
	double maximumDistance;
	double	rangeback;
	
	double	x0,y0,z0;
	double	x0M,y0M,z0M;
	int 	Nx,Ny,Nz,Nxy;
	int		N2x,N2y,N2z,N2xy;
	bool	EmptyInit;
	short	infinity;
	short	infp1;

	int		numberOfSeeds;
	unsigned char *clearMask;
	 
	
	int				TempArraySize;
	int				ArraySize;





	short			*TempDistanceVectorArrayX;
	short			*TempDistanceVectorArrayY;
	short			*TempDistanceVectorArrayZ;
	
	short			*DistanceVectorArrayX;
	short			*DistanceVectorArrayY;
	short			*DistanceVectorArrayZ;


	int		FillTempDistArrayXWithPolyDataPoints(short *Array);
	int		FillTempDistArrayXWithRawDataPoints(short *Array);
	int		FillTempDistArrayXWithMaskPoints(short *Array);


	
	void	CreateOutputArrays();
	void	CalculateDistance();	
	void	CalculateDistanceArrays();
	void	CalculateForwardUp();
	void	CalculateBackwardDown();

	void	CalculateForwardUp2D();
	void	CalculateBackwardDown2D();

	void	FindBoundaries(short *tab,short *original);
	void	FindBoundaries3D(short *tab,short *original);

	void	Calculate2DDistanceArrays();
//	RawDataSet *Create2DDistanceMapRawDataSet(bool sign=true);


	//double	*FloatSignedMap;



	inline double	 Interpolation3Linear(unsigned short *Q,double X,double Y, double Z);
	inline double	 Interpolation3Linear(short *Q,double X,double Y, double Z);

};


#endif //DISTANCE_MAP_H
