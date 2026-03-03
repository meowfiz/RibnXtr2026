#ifndef TRANSFORMPOINT_H
#define TRANSFORMPOINT_H

#include	<stdio.h>
#include	<math.h>
#include	<stdlib.h>

#include	<vtkPolyData.h>
#include	<vtkPoints.h>

#include	"SurfaceCollection.h"
#include	"RawDataSet.h"
#include	"OtherCommonFunctions.h"



//class RawDataSet;
class PolygonSurfaceData;

class Transform
{
public:


	//------------------------------------------------------------------------


	inline int Position2(int x, int y)
	{
		return x * 3 + y;
	};


	Transform(PolygonSurfaceData *ObjectSurfaceData, double *_RCenter, int ArithmeticCenter, bool _oneScale = false);
	Transform(double *_RCenter, double _ModelVoxelSizeX, double _ModelVoxelSizeY, double _ModelVoxelSizeZ, bool _oneScale = false);
	~Transform(void);

	void	ApplyTransformation(int PointNumber, double *Trans, int &x1, int &y1, int &z1, double &rx1, double &ry1, double &rz1);
	void	ApplyTransformationFloat(int PointNumber, double *Trans, double &x1, double &y1, double &z1, double &rx1, double &ry1, double &rz1);

	int		*ApplyTransformation(double *Point, double *Trans);
	double	*ApplyTransformationF(double *Point, double *Trans);
	double	*ApplyTransformationFloat(double *Point, double *Trans);
	void	ApplyTransformationFloat(double *Point, double *Point1, double *Trans);
	void	ApplyTransformationF(double *Point, double *Point1, double *Trans);

	void	ApplyTransformationReal(int PointNumber, double *Trans, double &x1, double &y1, double &z1);

	void	CreateIndexedArray(double _ModelVoxelSizeX, double _ModelVoxelSizeY, double _ModelVoxelSizeZ);

	void	CalculateTrigonometricFunctions(double *Trans);
	void	MakeMatrix(double *Matrix, double *Trans);
	void	CalculateTrigonometricFunctionsInv(double *Trans);
	void	MakeMatrixInv(double *Matrix, double *Trans);

	void	*MultiplyPoint(double *Point1, double *Point);
	void	TransformPoint(double *PIn, double *POut, double *def = NULL);
	void	TransformPoint(double *PIn, double *POut, double amount, double *def = NULL);
	double	*TransformPresettedPoint(void);


	void	CalculateShearParameters(double *Trans);
	void	CalculateShearParametersInv(double *Trans);


	void	CalculateShearParameters1(double *Trans);
	void	CalculateShearParametersInv1(double *Trans);

	void	MakeAffineMatrix(double *Matrix, double *Trans);
	void	MakeAffineMatrixInv(double *Matrix, double *Trans);

	void	ScaleRotMatrix(double *Trans);
	void	ScaleRotMatrixInv(double *Trans);


	void	MakeCompleteAffineMatrix(double *_Matrix, double *Trans);
	void	MakeCompleteAffineMatrixInv(double *_Matrix, double *Trans);

	void	SetTransformationSteps(double X, double Y, double Z, int MaxX, int MaxY, int MaxZ);
	void	SetTransformationSteps(double X, double Y, double Z, double XO, double YO, double ZO, int MaxX, int MaxY, int MaxZ);
	void	SetTransformationSteps(double X, double Y, double Z, double XO, double YO, double ZO,
		int MaxX, int MaxY, int MaxZ, int StepX, int StepY, int StepZ);

	void	SetNewXCoordinate(double Point);
	void	SetNewYCoordinate(double Point);
	void	SetNewZCoordinate(double Point);
	void	SetNextXCoordinate();
	void	SetNextYCoordinate();
	void	SetNextZCoordinate();
	void	SetZeroXCoordinate();
	void	SetZeroYCoordinate();
	void	SetZeroZCoordinate();


	//-----------------------------------------------
	void	SetZeroYNextZCoordinate();
	void	SetZeroXNextYCoordinate();

	double	SX1, SX2, SX3;
	double	SY1, SY2, SY3;
	double	SZ1, SZ2, SZ3;


	double	SXB1, SXB2, SXB3;
	double	SYB1, SYB2, SYB3;
	double	SZB1, SZB2, SZB3;

	double	SXYB1, SXYB2, SXYB3;
	double	SYZB1, SYZB2, SYZB3;

	//int		PosX,PosY,PosZ;

	bool	oneScale;

	double   *FinalPoint;



	double	rx1, ry1, rz1;
	double *Matrix;


	double	TX1, TX2, TX3;
	double	TY1, TY2, TY3;
	double	TZ1, TZ2, TZ3;


	vtkPolyData *ObjectPolyData;
	bool ArrayExists;
	vtkPoints *pPoints1;
	int		NPoint;
	int		sizex, sizey, sizez;
	double	ModelVoxelSizeX, ModelVoxelSizeY, ModelVoxelSizeZ;
	double	*RCenter;
	double	*IndexedArray;
	double	*Trans;
	double	RotateCenterX, RotateCenterY, RotateCenterZ;
	double 	ModelCenterX, ModelCenterY, ModelCenterZ;


	double   sx, sy, sz, cx, cy, cz;
	// dla macierzy rotacji
	double 	xtr1, ytr1, ztr1;
	double 	xtr2, ytr2, ztr2;
	double 	xtr3, ytr3, ztr3;

	// dla macierzy shear
	double 	xsr1, ysr1, zsr1;
	double 	xsr2, ysr2, zsr2;
	double 	xsr3, ysr3, zsr3;

	double *tr;

};



#endif //TRANSFORMPOINT_H
