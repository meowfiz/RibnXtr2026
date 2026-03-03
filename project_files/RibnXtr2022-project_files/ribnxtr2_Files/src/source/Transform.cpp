
#include "Transform.h"





//Makes a list of object points

//------------------------------------------------------------------------

Transform::Transform(PolygonSurfaceData *ObjectSurfaceData, double *_RCenter, int ArithmeticCenter, bool _oneScale)
{

	oneScale = _oneScale;

	IndexedArray = NULL;
	int i;
	ArrayExists = 0;
	RCenter = _RCenter;


	double pPoint1[3];
	pPoints1 = ObjectSurfaceData->GetVPolyData()->GetPoints();
	NPoint = pPoints1->GetNumberOfPoints();



	//	double rx=0;
	//	double ry=0;
	//	double rz=0;


	sizex = ObjectSurfaceData->GetPN()[0];
	sizey = ObjectSurfaceData->GetPN()[1];
	sizez = ObjectSurfaceData->GetPN()[2];


	if (ArithmeticCenter == 0)
	{
		double min[3], max[3];
		min[0] = 10000;
		min[1] = 10000;
		min[2] = 10000;
		max[0] = -10000;
		max[1] = -10000;
		max[2] = -10000;
		double tempA[3];

		//Find Center of the objectus
		for (i = 0; i<NPoint; i++)
		{
			pPoints1->GetPoint(i, pPoint1);

			tempA[0] = pPoint1[0];
			tempA[1] = pPoint1[1];
			tempA[2] = pPoint1[2];

			if (tempA[0]>max[0]) max[0] = tempA[0];
			if (tempA[1]>max[1]) max[1] = tempA[1];
			if (tempA[2]>max[2]) max[2] = tempA[2];
			if (tempA[0]<min[0]) min[0] = tempA[0];
			if (tempA[1]<min[1]) min[1] = tempA[1];
			if (tempA[2]<min[2]) min[2] = tempA[2];
		}

		RCenter[0] = (max[0] - min[0]) / 2.0 + min[0];
		RCenter[1] = (max[1] - min[1]) / 2.0 + min[1];
		RCenter[2] = (max[2] - min[2]) / 2.0 + min[2];
	}
	else if (ArithmeticCenter == 1)
	{
		RCenter[0] = (double)(ObjectSurfaceData->GetPN()[0] - 1)*(ObjectSurfaceData->GetPScale()[0]) / 2.0;
		RCenter[1] = (double)(ObjectSurfaceData->GetPN()[1] - 1)*(ObjectSurfaceData->GetPScale()[1]) / 2.0;
		RCenter[2] = (double)(ObjectSurfaceData->GetPN()[2] - 1)*(ObjectSurfaceData->GetPScale()[2]) / 2.0;

	}


	//	FILE *Do_opa;
	//	Do_opa=fopen("c:\\mrq.txt","a");fprintf(Do_opa,"Liczba pktow obiektu: %d \n",NPoint);fclose(Do_opa);



}
//0--------------------------------------------------------------------------
Transform::Transform(double *_RCenter, double _ModelVoxelSizeX, double _ModelVoxelSizeY, double _ModelVoxelSizeZ, bool _oneScale)
{

	oneScale = _oneScale;
	RCenter = _RCenter;

	IndexedArray = NULL;

	ModelVoxelSizeX = _ModelVoxelSizeX;
	ModelVoxelSizeY = _ModelVoxelSizeY;
	ModelVoxelSizeZ = _ModelVoxelSizeZ;

	FinalPoint = new double[3];
	FinalPoint[0] = 0.0;
	FinalPoint[1] = 0.0;
	FinalPoint[2] = 0.0;
	//PosX=0;
	//PosY=0;
	//PosZ=0;
	//default bez shear (to samo dopisac pozniej dla rotacji)
	xsr1 = 1;
	xsr2 = 0;
	xsr3 = 0;

	ysr1 = 0;
	ysr2 = 1;
	ysr3 = 0;

	zsr1 = 0;
	zsr2 = 0;
	zsr3 = 1;



}
//------------------------------------------------------------------------

void Transform::CreateIndexedArray(double _ModelVoxelSizeX, double _ModelVoxelSizeY, double _ModelVoxelSizeZ)
{



	double pPoint1[3];
	double tempA[3];

	ModelVoxelSizeX = _ModelVoxelSizeX;
	ModelVoxelSizeY = _ModelVoxelSizeY;
	ModelVoxelSizeZ = _ModelVoxelSizeZ;

	if (ArrayExists == 1) delete[]IndexedArray;
	IndexedArray = new double[(NPoint + 1) * 3];
	//Calculate Indexed Array
	int i;
	int TEmp = 0;
	for (i = 0; i<NPoint; i++)
	{
		pPoints1->GetPoint(i, pPoint1);

		tempA[0] = pPoint1[0] - RCenter[0];
		tempA[1] = pPoint1[1] - RCenter[1];
		tempA[2] = pPoint1[2] - RCenter[2];

		IndexedArray[TEmp++] = tempA[0];
		IndexedArray[TEmp++] = tempA[1];
		IndexedArray[TEmp++] = tempA[2];

	}
	ArrayExists = 1;

}



Transform::~Transform()
{
	if (IndexedArray != NULL) delete[] IndexedArray; //2003.11.22 -nie mam pojecia jakim cudem to dzialalo do tej pory
	//	delete[] FinalPoint;
}



//------------------------------------------------------------------------

void Transform::ApplyTransformation(int PointNumber, double *Trans, int &x1, int &y1, int &z1,
	double &rx1, double &ry1, double &rz1)
{

	// used to compute a coordinates with a new Transform parameters
	PointNumber -= 1;   //for numrerical recipies (points from 1 .. n )


	int p = PointNumber * 3;

	rx1 = IndexedArray[p++];
	ry1 = IndexedArray[p++];
	rz1 = IndexedArray[p];


	x1 = Round((rx1*xtr1 + ry1*xtr2 + rz1*xtr3 + Trans[1] + RCenter[0]) / ModelVoxelSizeX);
	y1 = Round((rx1*ytr1 + ry1*ytr2 + rz1*ytr3 + Trans[2] + RCenter[1]) / ModelVoxelSizeY);
	z1 = Round((rx1*ztr1 + ry1*ztr2 + rz1*ztr3 + Trans[3] + RCenter[2]) / ModelVoxelSizeZ);


}

void Transform::ApplyTransformationFloat(int PointNumber, double *Trans, double &x1, double &y1, double &z1,
	double &rx1, double &ry1, double &rz1)
{

	// used to compute a coordinates with a new Transform parameters
	PointNumber -= 1;   //for numrerical recipies (points from 1 .. n )



	int p = PointNumber * 3;

	rx1 = IndexedArray[p++];
	ry1 = IndexedArray[p++];
	rz1 = IndexedArray[p];


	x1 = (double)((rx1*xtr1 + ry1*xtr2 + rz1*xtr3 + Trans[1] + RCenter[0]) / ModelVoxelSizeX);
	y1 = (double)((rx1*ytr1 + ry1*ytr2 + rz1*ytr3 + Trans[2] + RCenter[1]) / ModelVoxelSizeY);
	z1 = (double)((rx1*ztr1 + ry1*ztr2 + rz1*ztr3 + Trans[3] + RCenter[2]) / ModelVoxelSizeZ);


}

void Transform::ApplyTransformationReal(int PointNumber, double *Trans, double &x1, double &y1, double &z1)
{

	// used to compute a coordinates with a new Transform parameters
	PointNumber -= 1;   //for numrerical recipies (points from 1 .. n )



	int p = PointNumber * 3;

	rx1 = IndexedArray[p++];
	ry1 = IndexedArray[p++];
	rz1 = IndexedArray[p];


	x1 = (double)((rx1*xtr1 + ry1*xtr2 + rz1*xtr3 + Trans[1] + RCenter[0]));
	y1 = (double)((rx1*ytr1 + ry1*ytr2 + rz1*ytr3 + Trans[2] + RCenter[1]));
	z1 = (double)((rx1*ztr1 + ry1*ztr2 + rz1*ztr3 + Trans[3] + RCenter[2]));


}


int  *Transform::ApplyTransformation(double *Point, double *Trans)
{

	int *Point1 = new int[3];

	Point1[0] = Round((Point[0] * xtr1 + Point[1] * xtr2 + Point[2] * xtr3 + Trans[1] + RCenter[0]) / ModelVoxelSizeX);
	Point1[1] = Round((Point[0] * ytr1 + Point[1] * ytr2 + Point[2] * ytr3 + Trans[2] + RCenter[1]) / ModelVoxelSizeY);
	Point1[2] = Round((Point[0] * ztr1 + Point[1] * ztr2 + Point[2] * ztr3 + Trans[3] + RCenter[2]) / ModelVoxelSizeZ);

	return Point1;

}

double  *Transform::ApplyTransformationFloat(double *Point, double *Trans)
{

	double *Point1 = new double[3];

	Point1[0] = (Point[0] * xtr1 + Point[1] * xtr2 + Point[2] * xtr3 + Trans[1] + RCenter[0]) / ModelVoxelSizeX;
	Point1[1] = (Point[0] * ytr1 + Point[1] * ytr2 + Point[2] * ytr3 + Trans[2] + RCenter[1]) / ModelVoxelSizeY;
	Point1[2] = (Point[0] * ztr1 + Point[1] * ztr2 + Point[2] * ztr3 + Trans[3] + RCenter[2]) / ModelVoxelSizeZ;

	return Point1;

}

void Transform::ApplyTransformationFloat(double *Point, double *Point1, double *Trans)
{



	Point1[0] = (Point[0] * xtr1 + Point[1] * xtr2 + Point[2] * xtr3 + Trans[1] + RCenter[0]) / ModelVoxelSizeX;
	Point1[1] = (Point[0] * ytr1 + Point[1] * ytr2 + Point[2] * ytr3 + Trans[2] + RCenter[1]) / ModelVoxelSizeY;
	Point1[2] = (Point[0] * ztr1 + Point[1] * ztr2 + Point[2] * ztr3 + Trans[3] + RCenter[2]) / ModelVoxelSizeZ;



}

void Transform::ApplyTransformationF(double *Point, double *Point1, double *Trans)
{



	Point1[0] = (Point[0] * xtr1 + Point[1] * xtr2 + Point[2] * xtr3 + Trans[1] + RCenter[0]);
	Point1[1] = (Point[0] * ytr1 + Point[1] * ytr2 + Point[2] * ytr3 + Trans[2] + RCenter[1]);
	Point1[2] = (Point[0] * ztr1 + Point[1] * ztr2 + Point[2] * ztr3 + Trans[3] + RCenter[2]);



}


double  *Transform::ApplyTransformationF(double *Point, double *Trans)
{

	double *Point1 = new double[3];

	Point1[0] = (Point[0] * xtr1 + Point[1] * xtr2 + Point[2] * xtr3 + Trans[1] + RCenter[0]);
	Point1[1] = (Point[0] * ytr1 + Point[1] * ytr2 + Point[2] * ytr3 + Trans[2] + RCenter[1]);
	Point1[2] = (Point[0] * ztr1 + Point[1] * ztr2 + Point[2] * ztr3 + Trans[3] + RCenter[2]);

	return Point1;

}

//----------------------------------------------------------------------
//-----------------Right Handed-----------------------------------------
//-------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void	Transform::CalculateTrigonometricFunctions(double *Trans)
{

	sx = (double)sin(Trans[4]); cx = (double)cos(Trans[4]);
	sy = (double)sin(Trans[5]); cy = (double)cos(Trans[5]);
	sz = (double)sin(Trans[6]); cz = (double)cos(Trans[6]);

	//right!!! XYZP

	/*tr1=cy*cz;
	xtr2=cy*sz;
	xtr3=-sy;

	ytr1=(sx*sy*cz-cx*sz);
	ytr2=(sx*sy*sz+cx*cz);
	ytr3=sx*cy;

	ztr1=(cx*sy*cz+sx*sz);
	ztr2=(cx*sy*sz-sx*cz);
	ztr3=cx*cy;
	//*/

	//left coordinate system YXZP
	/*	xtr1=cy*cz-sx*sy*sz;
	xtr2=cy*sz+sx*sy*cz;
	xtr3=-cx*sy;

	ytr1=-cx*sz;
	ytr2=cx*cz;
	ytr3=sx;

	ztr1=sy*cz+sx*cy*sz;
	ztr2=sy*sz-sx*cy*cz;
	ztr3=cx*cy;
	//*/

	//VTK  ZXYP	
	xtr1 = cz*cy + sz*sx*sy;
	xtr2 = sz*cx;
	xtr3 = -cz*sy + sz*sx*cy;

	ytr1 = -sz*cy + cz*sx*sy;
	ytr2 = cz*cx;
	ytr3 = sz*sy + cz*sx*cy;

	ztr1 = cx*sy;
	ztr2 = -sx;
	ztr3 = cx*cy;

	//*/
}

//----------------------------------------------------------------------

void	Transform::MakeMatrix(double *_Matrix, double *Trans)
{
	Matrix = _Matrix;
	//Make Trans matrix for a vtk object
	double rx = RCenter[0];
	double ry = RCenter[1];
	double rz = RCenter[2];
	Matrix[0] = xtr1;	Matrix[1] = xtr2;	Matrix[2] = xtr3;		Matrix[3] = -xtr1*rx - xtr2*ry - xtr3*rz + rx + Trans[1];
	Matrix[4] = ytr1;	Matrix[5] = ytr2;	Matrix[6] = ytr3;		Matrix[7] = -ytr1*rx - ytr2*ry - ytr3*rz + ry + Trans[2];
	Matrix[8] = ztr1;	Matrix[9] = ztr2;	Matrix[10] = ztr3;	Matrix[11] = -ztr1*rx - ztr2*ry - ztr3*rz + rz + Trans[3];
	Matrix[12] = 0;	Matrix[13] = 0;	Matrix[14] = 0;		Matrix[15] = 1;
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

void	Transform::CalculateTrigonometricFunctionsInv(double *Trans)
{

	sx = (double)sin(Trans[4]); cx = (double)cos(Trans[4]);
	sy = (double)sin(Trans[5]); cy = (double)cos(Trans[5]);
	sz = (double)sin(Trans[6]); cz = (double)cos(Trans[6]);


	//right
	/*	xtr1=cz*cy;
	xtr2=sx*sy*cz-cx*sz;
	xtr3=cx*sy*cz+sx*sz;

	ytr1=sz*cy;
	ytr2=sx*sy*sz+cx*cz;
	ytr3=cx*sy*sz-sx*cz;

	ztr1=-sy;
	ztr2=cy*sx;
	ztr3=cy*cx;

	//*/

	//left coordinate system YXZP

	/*	xtr1=cy*cz-sx*sy*sz;
	xtr2=-cx*sz;
	xtr3=sy*cz+sx*cy*sz;

	ytr1=cy*sz+sx*sy*cz;
	ytr2=cx*cz;
	ytr3=sy*sz-sx*cy*cz;

	ztr1=-cx*sy;
	ztr2=sx;
	ztr3=cx*cy;

	//*/

	//VTK  ZXYP	

	xtr1 = cz*cy + sz*sx*sy;
	xtr2 = -sz*cy + cz*sx*sy;
	xtr3 = cx*sy;

	ytr1 = sz*cx;
	ytr2 = cz*cx;
	ytr3 = -sx;

	ztr1 = -cz*sy + sz*sx*cy;
	ztr2 = sz*sy + cz*sx*cy;
	ztr3 = cx*cy;
	//*/



}

//----------------------------------------------------------------------

void	Transform::MakeMatrixInv(double *_Matrix, double *Trans)
{


	Matrix = _Matrix;

	double rtx = RCenter[0] + Trans[1];
	double rty = RCenter[1] + Trans[2];
	double rtz = RCenter[2] + Trans[3];

	Matrix[0] = xtr1;	Matrix[1] = xtr2;	Matrix[2] = xtr3;		Matrix[3] = -rtx*xtr1 - rty*xtr2 - rtz*xtr3 + RCenter[0];
	Matrix[4] = ytr1;	Matrix[5] = ytr2;	Matrix[6] = ytr3;		Matrix[7] = -rtx*ytr1 - rty*ytr2 - rtz*ytr3 + RCenter[1];
	Matrix[8] = ztr1;	Matrix[9] = ztr2;	Matrix[10] = ztr3;	Matrix[11] = -rtx*ztr1 - rty*ztr2 - rtz*ztr3 + RCenter[2];
	Matrix[12] = 0;	Matrix[13] = 0;	Matrix[14] = 0;		Matrix[15] = 1;

}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//------------------AFFINE TRANSFORMATION!------------------------------
//-----------------------2003.10.03-------------------------------------
//----------------------------------------------------------------------


void	Transform::CalculateShearParameters1(double *Trans)
{

	//x->y
	//y->z
	//z->x	
	double sh1 = Trans[7];
	double sh2 = 0;
	double sh3 = 0;
	double sh4 = Trans[8];
	double sh5 = Trans[9];
	double sh6 = 0;

	xsr1 = 1 + sh1*sh3;
	xsr2 = sh1;
	xsr3 = sh1*sh4 + sh2;

	ysr1 = sh3;
	ysr2 = 1;
	ysr3 = sh4;

	zsr1 = sh5 + sh3*sh5*sh1 + sh3*sh6;
	zsr2 = sh5*sh1 + sh6;
	zsr3 = 1 + sh4*sh5*sh1 + sh4*sh6 + sh5*sh2;
}


//--------------------------------------------------

void	Transform::CalculateShearParametersInv1(double *Trans)
{

	//x->y
	//y->z
	//z->x
	double sh1 = Trans[7];
	double sh2 = 0;
	double sh3 = 0;
	double sh4 = Trans[8];
	double sh5 = Trans[9];
	double sh6 = 0;



	xsr1 = 1 + sh5*sh2;
	xsr2 = -sh1 + sh2*sh6;
	xsr3 = -sh2;

	ysr1 = -sh3 - sh5*sh3*sh2 + sh5*sh4;
	ysr2 = 1 + sh1*sh3 - sh6*sh3*sh2 + sh4*sh6;
	ysr3 = sh3*sh2 - sh4;

	zsr1 = -sh5;
	zsr2 = -sh6;
	zsr3 = 1;



}

//------------------------------------------------

void	Transform::CalculateShearParameters(double *Trans)
{

	// x-> (y,z=sh1)
	// y-> (x,z=sh2)
	// z-> (x,y=sh3)


	double sh1 = Trans[7];
	double sh2 = Trans[8];
	double sh3 = Trans[9];


	//VTK  ZXYP	
	xsr1 = 1 + sh1*sh2;
	xsr2 = sh1;
	xsr3 = sh1*sh2 + sh1;

	ysr1 = sh2;
	ysr2 = 1;
	ysr3 = sh2;

	zsr1 = sh3 + (sh3*sh1 + sh3)*sh2;
	zsr2 = sh3*sh1 + sh3;
	zsr3 = 1 + (sh3*sh1 + sh3)*sh2 + sh3*sh1;

	/*
	xsr1=1+sh1*sh3;
	xsr2=sh1;
	xsr3=sh1*sh4+sh2;

	ysr1=sh3;
	ysr2=1;
	ysr3=sh4;

	zsr1=sh5+sh3*sh5*sh1+sh3*sh6;
	zsr2=sh5*sh1+sh6;
	zsr3=1+sh4*sh5*sh1+sh4*sh6+sh5*sh2;

	*/

}

//----------------------------------------------------------------------

void	Transform::CalculateShearParametersInv(double *trans)
{
	// x-> (y,z=sh1)
	// y-> (x,z=sh2)
	// z-> (x,y=sh3)


	double sh1 = trans[7];
	double sh2 = trans[8];
	double sh3 = trans[9];


	//VTK  ZXYP	
	xsr1 = sh3*sh1 + 1;
	xsr2 = -sh1 + sh3*sh1;
	xsr3 = -sh1;

	ysr1 = -sh2 - (sh1*sh2 - sh2)*sh3;
	ysr2 = 1 + sh1*sh2 - (sh1*sh2 - sh2) *sh3;
	ysr3 = sh1*sh2 - sh2;

	zsr1 = -sh3;
	zsr2 = -sh3;
	zsr3 = 1;

	/*
	xsr1=1+sh5*sh2;
	xsr2=-sh1+sh2*sh6;
	xsr3=-sh2;

	ysr1=-sh3-sh5*sh3*sh2+sh5*sh4;
	ysr2=1+sh1*sh3-sh6*sh3*sh2+sh4*sh6;
	ysr3=sh3*sh2-sh4;

	zsr1=-sh5;
	zsr2=-sh6;
	zsr3=1;
	*/


}


//----------------------------------------------------------------------


void	Transform::ScaleRotMatrix(double *Trans)
{


	//xtr1*=Trans[10];
	//xtr2*=Trans[10];
	//xtr3*=Trans[10];

	//ytr1*=Trans[11];
	//ytr2*=Trans[11];
	//ytr3*=Trans[11];

	//ztr1*=Trans[12];
	//ztr2*=Trans[12];
	//ztr3*=Trans[12];

	double tol = 1e-5;

	if (oneScale == false)
	{
		//if (fabs(Trans[10])>tol)
		//{
		xtr1 *= Trans[10];
		xtr2 *= Trans[10];
		xtr3 *= Trans[10];
		/*}
		else
		{
		xtr1=INT_MAX;
		xtr2=INT_MAX;
		xtr3=INT_MAX;
		}*/

		//if (fabs(Trans[11])>tol)
		//{
		ytr1 *= Trans[11];
		ytr2 *= Trans[11];
		ytr3 *= Trans[11];
		/*}
		else
		{
		ytr1=INT_MAX;
		ytr2=INT_MAX;
		ytr3=INT_MAX;
		}*/

		//if (fabs(Trans[12])>tol)
		//{
		ztr1 *= Trans[12];
		ztr2 *= Trans[12];
		ztr3 *= Trans[12];
		/*}
		else
		{
		ztr1=INT_MAX;
		ztr2=INT_MAX;
		ztr3=INT_MAX;
		}*/
	}
	else
	{
		//if (fabs(Trans[10])<tol)
		//{
		xtr1 *= Trans[10];
		xtr2 *= Trans[10];
		xtr3 *= Trans[10];
		ytr1 *= Trans[10];
		ytr2 *= Trans[10];
		ytr3 *= Trans[10];
		ztr1 *= Trans[10];
		ztr2 *= Trans[10];
		ztr3 *= Trans[10];
		/*}
		else
		{
		xtr1=INT_MAX;
		xtr2=INT_MAX;
		xtr3=INT_MAX;
		ytr1=INT_MAX;
		ytr2=INT_MAX;
		ytr3=INT_MAX;
		ztr1=INT_MAX;
		ztr2=INT_MAX;
		ztr3=INT_MAX;
		}*/
	}



}

//-----------------------------------------------------

void	Transform::ScaleRotMatrixInv(double *Trans)
{
	double tol = 1e-5;

	if (oneScale == false)
	{
		if (fabs(Trans[10])>tol)
		{
			xtr1 /= Trans[10];
			xtr2 /= Trans[10];
			xtr3 /= Trans[10];
		}
		else
		{
			xtr1 = INT_MAX;
			xtr2 = INT_MAX;
			xtr3 = INT_MAX;
		}

		if (fabs(Trans[11])>tol)
		{
			ytr1 /= Trans[11];
			ytr2 /= Trans[11];
			ytr3 /= Trans[11];
		}
		else
		{
			ytr1 = INT_MAX;
			ytr2 = INT_MAX;
			ytr3 = INT_MAX;
		}

		if (fabs(Trans[12])>tol)
		{
			ztr1 /= Trans[12];
			ztr2 /= Trans[12];
			ztr3 /= Trans[12];
		}
		else
		{
			ztr1 = INT_MAX;
			ztr2 = INT_MAX;
			ztr3 = INT_MAX;
		}
	}
	else
	{
		if (fabs(Trans[10])>tol)
		{
			xtr1 /= Trans[10];
			xtr2 /= Trans[10];
			xtr3 /= Trans[10];
			ytr1 /= Trans[10];
			ytr2 /= Trans[10];
			ytr3 /= Trans[10];
			ztr1 /= Trans[10];
			ztr2 /= Trans[10];
			ztr3 /= Trans[10];
		}
		else
		{
			xtr1 = INT_MAX;
			xtr2 = INT_MAX;
			xtr3 = INT_MAX;
			ytr1 = INT_MAX;
			ytr2 = INT_MAX;
			ytr3 = INT_MAX;
			ztr1 = INT_MAX;
			ztr2 = INT_MAX;
			ztr3 = INT_MAX;
		}
	}


}
//----------------------------------------------------------------------
void	Transform::MakeAffineMatrix(double *_Matrix, double *Trans)
{
	Matrix = _Matrix;
	double origx = -RCenter[0];
	double origy = -RCenter[1];
	double origz = -RCenter[2];

	double origtransx = RCenter[0] + Trans[1];
	double origtransy = RCenter[1] + Trans[2];
	double origtransz = RCenter[2] + Trans[3];

	Matrix[0] = xsr1*xtr1 + xsr2*ytr1 + xsr3*ztr1;
	Matrix[1] = xsr1*xtr2 + xsr2*ytr2 + xsr3*ztr2;
	Matrix[2] = xsr1*xtr3 + xsr2*ytr3 + xsr3*ztr3;
	Matrix[3] = (xsr1*xtr1 + xsr2*ytr1 + xsr3*ztr1)*origx + (xsr1*xtr2 + xsr2*ytr2 + xsr3*ztr2)*origy + (xsr1*xtr3 + xsr2*ytr3 + ztr3*xsr3)*origz + origtransx;

	Matrix[4] = ysr1*xtr1 + ysr2*ytr1 + ysr3*ztr1;
	Matrix[5] = ysr1*xtr2 + ysr2*ytr2 + ysr3*ztr2;
	Matrix[6] = ysr1*xtr3 + ysr2*ytr3 + ysr3*ztr3;
	Matrix[7] = (ysr1*xtr1 + ysr2*ytr1 + ysr3*ztr1)*origx + (ysr1*xtr2 + ysr2*ytr2 + ysr3*ztr2)*origy + (ysr1*xtr3 + ysr2*ytr3 + ztr3*ysr3)*origz + origtransy;

	Matrix[8] = zsr1*xtr1 + zsr2*ytr1 + zsr3*ztr1;
	Matrix[9] = zsr1*xtr2 + zsr2*ytr2 + zsr3*ztr2;
	Matrix[10] = zsr1*xtr3 + zsr2*ytr3 + zsr3*ztr3;
	Matrix[11] = (zsr1*xtr1 + zsr2*ytr1 + ztr1*zsr3)*origx + (zsr1*xtr2 + zsr2*ytr2 + zsr3*ztr2)*origy + (zsr1*xtr3 + zsr2*ytr3 + ztr3*zsr3)*origz + origtransz;

	Matrix[12] = 0;
	Matrix[13] = 0;
	Matrix[14] = 0;
	Matrix[15] = 1;
}
//----------------------------------------------------------------------

void	Transform::MakeAffineMatrixInv(double *_Matrix, double *Trans)
{
	Matrix = _Matrix;
	double origx = -RCenter[0];
	double origy = -RCenter[1];
	double origz = -RCenter[2];

	double origtransx = RCenter[0] + Trans[1];
	double origtransy = RCenter[1] + Trans[2];
	double origtransz = RCenter[2] + Trans[3];




	Matrix[0] = xsr1*xtr1 + ysr1*xtr2 + zsr1*xtr3;
	Matrix[1] = xtr1*xsr2 + xtr2*ysr2 + xtr3*zsr2;
	Matrix[2] = xtr1*xsr3 + xtr2*ysr3 + xtr3*zsr3;
	Matrix[3] = -(xsr1*xtr1 + ysr1*xtr2 + zsr1*xtr3)*origtransx - (xtr1*xsr2 + xtr2*ysr2 + xtr3*zsr2)*origtransy - (xtr1*xsr3 + xtr2*ysr3 + xtr3*zsr3)*origtransz - origx;

	Matrix[4] = ytr1*xsr1 + ytr2*ysr1 + ytr3*zsr1;
	Matrix[5] = xsr2*ytr1 + ysr2*ytr2 + zsr2*ytr3;
	Matrix[6] = ytr1*xsr3 + ytr2*ysr3 + ytr3*zsr3;
	Matrix[7] = -(ytr1*xsr1 + ytr2*ysr1 + ytr3*zsr1)*origtransx - (xsr2*ytr1 + ysr2*ytr2 + zsr2*ytr3)*origtransy - (ytr1*xsr3 + ytr2*ysr3 + ytr3*zsr3)*origtransz - origy;

	Matrix[8] = ztr1*xsr1 + ztr2*ysr1 + ztr3*zsr1;
	Matrix[9] = ztr1*xsr2 + ztr2*ysr2 + ztr3*zsr2;
	Matrix[10] = xsr3*ztr1 + ysr3*ztr2 + ztr3*zsr3;
	Matrix[11] = -(ztr1*xsr1 + ztr2*ysr1 + ztr3*zsr1)*origtransx - (ztr1*xsr2 + ztr2*ysr2 + ztr3*zsr2)*origtransy - (xsr3*ztr1 + ysr3*ztr2 + ztr3*zsr3)*origtransz - origz;




	Matrix[12] = 0;
	Matrix[13] = 0;
	Matrix[14] = 0;
	Matrix[15] = 1;



}

//----------------------------------------------------------------------


void	Transform::MakeCompleteAffineMatrix(double *_Matrix, double *Trans)
{

	CalculateTrigonometricFunctions(Trans);
	ScaleRotMatrix(Trans);

	//---------------------------------
	//x->y
	//y->z
	//z->x	
	CalculateShearParameters1(Trans);
	//---------------------------------

	//---------------------------------
	// x-> (y,z=sh1)
	// y-> (x,z=sh2)
	// z-> (x,y=sh3)
	//CalculateShearParameters(Trans);
	//----------------------------------------
	MakeAffineMatrix(_Matrix, Trans);

}

//----------------------------------------------------------------------


void	Transform::MakeCompleteAffineMatrixInv(double *_Matrix, double *Trans)
{

	CalculateTrigonometricFunctionsInv(Trans);
	ScaleRotMatrixInv(Trans);

	//---------------------------------
	//x->y
	//y->z
	//z->x	
	CalculateShearParametersInv1(Trans);
	//---------------------------------

	//---------------------------------
	// x-> (y,z=sh1)
	// y-> (x,z=sh2)
	// z-> (x,y=sh3)
	//CalculateShearParametersInv(Trans);
	//----------------------------------------

	MakeAffineMatrixInv(_Matrix, Trans);

}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

void *Transform::MultiplyPoint(double *Point1, double *Point)
{


	Point1[0] = (Point[0] * Matrix[0] + Point[1] * Matrix[1] + Point[2] * Matrix[2] + Matrix[3]);
	Point1[1] = (Point[0] * Matrix[4] + Point[1] * Matrix[5] + Point[2] * Matrix[6] + Matrix[7]);
	Point1[2] = (Point[0] * Matrix[8] + Point[1] * Matrix[9] + Point[2] * Matrix[10] + Matrix[11]);

	return Point1;
}

//-----------------------------------------------------------

void	Transform::TransformPoint(double *PIn, double *POut, double *def)
{

	//	double c=sqrt(		(PIn[0]-RCenter[0])*(PIn[0]-RCenter[0]) 
	//					+	(PIn[1]-RCenter[1])*(PIn[1]-RCenter[1]));
	//	if (c<0.000000001) c=1;

	//	double  f=tr[14]/c+(PIn[2]-RCenter[2])*tr[14]/100;
	double	f = (PIn[2] - RCenter[2])*tr[14] / 100;
	double zRot = tr[6];
	tr[6] = zRot + f;

	MakeCompleteAffineMatrix(Matrix, tr);
	tr[6] = zRot;
	//transX->MultiplyPoint(Point1,Point);


	POut[0] = (PIn[0] * Matrix[0] + PIn[1] * Matrix[1] + PIn[2] * Matrix[2] + Matrix[3]);
	POut[1] = (PIn[0] * Matrix[4] + PIn[1] * Matrix[5] + PIn[2] * Matrix[6] + Matrix[7]);
	POut[2] = (PIn[0] * Matrix[8] + PIn[1] * Matrix[9] + PIn[2] * Matrix[10] + Matrix[11]);

	if (def != NULL)
	{
		def[0] = POut[0] - PIn[0];
		def[1] = POut[1] - PIn[1];
		def[2] = POut[2] - PIn[2];
	}

}
//----------------------------------------------
void	Transform::TransformPoint(double *PIn, double *POut, double amount, double *def)
{
	//double c=sqrt(		(PIn[0]-RCenter[0])*(PIn[0]-RCenter[0]) 
	//				+	(PIn[1]-RCenter[1])*(PIn[1]-RCenter[1]));
	//if (c<0.000000001) c=1;

	//double  f=tr[14]/c+(PIn[2]-RCenter[2])*tr[14]/100;
	double	f = (PIn[2] - RCenter[2])*tr[14] / 100;
	double zRot = tr[6];
	tr[6] = zRot + f;

	MakeCompleteAffineMatrix(Matrix, tr);
	tr[6] = zRot;

	POut[0] = (PIn[0] * Matrix[0] + PIn[1] * Matrix[1] + PIn[2] * Matrix[2] + Matrix[3]);
	POut[1] = (PIn[0] * Matrix[4] + PIn[1] * Matrix[5] + PIn[2] * Matrix[6] + Matrix[7]);
	POut[2] = (PIn[0] * Matrix[8] + PIn[1] * Matrix[9] + PIn[2] * Matrix[10] + Matrix[11]);


	//Deformation(PIn,POut);

	//if (def!=NULL)
	//{def[0]=POut[0];def[1]=POut[1];def[2]=POut[2];}

	if (amount<0.9999)
	{
		POut[0] = PIn[0] + (POut[0] - PIn[0])*amount;
		POut[1] = PIn[1] + (POut[1] - PIn[1])*amount;
		POut[2] = PIn[2] + (POut[2] - PIn[2])*amount;
	}

	if (def != NULL)
	{
		def[0] = POut[0] - PIn[0];
		def[1] = POut[1] - PIn[1];
		def[2] = POut[2] - PIn[2];
	}

}
//volume transformation

double *Transform::TransformPresettedPoint(void)
{

	double *Point1 = new double[3];
	Point1[0] = (TX1 + TY1 + TZ1 + Matrix[3]);
	Point1[1] = (TX2 + TY2 + TZ2 + Matrix[7]);
	Point1[2] = (TX3 + TY3 + TZ3 + Matrix[11]);
	return Point1;

}
//-----------------------------------------------
void	Transform::SetNewXCoordinate(double Coord)
{
	TX1 = (double)Coord*xtr1;
	TX2 = (double)Coord*ytr1;
	TX3 = (double)Coord*ztr1;
}
//-----------------------------------------------
void	Transform::SetNewYCoordinate(double Coord)
{
	TY1 = (double)Coord*xtr2;
	TY2 = (double)Coord*ytr2;
	TY3 = (double)Coord*ztr2;
}
//-----------------------------------------------
void	Transform::SetNewZCoordinate(double Coord)
{
	TZ1 = (double)Coord*xtr3;
	TZ2 = (double)Coord*ytr3;
	TZ3 = (double)Coord*ztr3;
}



//-----------------------------------------------
//-----------------------------------------------

void	Transform::SetNextXCoordinate()
{
	FinalPoint[0] += SX1;
	FinalPoint[1] += SY1;
	FinalPoint[2] += SZ1;
	//	PosX++;
}
//-----------------------------------------------
void	Transform::SetNextYCoordinate()
{
	FinalPoint[0] += SX2;
	FinalPoint[1] += SY2;
	FinalPoint[2] += SZ2;
	//	PosY++;
}
//-----------------------------------------------
void	Transform::SetNextZCoordinate()
{
	FinalPoint[0] += SX3;
	FinalPoint[1] += SY3;
	FinalPoint[2] += SZ3;
	//	PosZ++;
}
void	Transform::SetTransformationSteps(double X, double Y, double Z, int MaxX, int MaxY, int MaxZ)
{

	//		FILE *Do_opa;

	SX1 = X*xtr1;
	SX2 = Y*xtr2;
	SX3 = Z*xtr3;

	SY1 = X*ytr1;
	SY2 = Y*ytr2;
	SY3 = Z*ytr3;

	SZ1 = X*ztr1;
	SZ2 = Y*ztr2;
	SZ3 = Z*ztr3;


	SXB1 = SX1*(double)MaxX;
	SXB2 = SX2*(double)MaxY;
	SXB3 = SX3*(double)MaxZ;
	SYB1 = SY1*(double)MaxX;
	SYB2 = SY2*(double)MaxY;
	SYB3 = SY3*(double)MaxZ;
	SZB1 = SZ1*(double)MaxX;
	SZB2 = SZ2*(double)MaxY;
	SZB3 = SZ3*(double)MaxZ;

	SXYB1 = SXB1 + SX2;
	SXYB2 = SYB1 + SY2;
	SXYB3 = SZB1 + SZ2;


	SYZB1 = SXB2 + SX3;
	SYZB2 = SYB2 + SY3;
	SYZB3 = SZB2 + SZ3;




	FinalPoint[0] = Matrix[3];
	FinalPoint[1] = Matrix[7];
	FinalPoint[2] = Matrix[11];
}
//--------------------------------------------------------------------

void	Transform::SetTransformationSteps(double X, double Y, double Z, double XO, double YO, double ZO, int MaxX, int MaxY, int MaxZ)
{

	//dostosowane do affine

	SX1 = (X*Matrix[0]) / XO;
	SX2 = (Y*Matrix[1]) / XO;
	SX3 = (Z*Matrix[2]) / XO;

	SY1 = (X*Matrix[4]) / YO;
	SY2 = (Y*Matrix[5]) / YO;
	SY3 = (Z*Matrix[6]) / YO;

	SZ1 = (X*Matrix[8]) / ZO;
	SZ2 = (Y*Matrix[9]) / ZO;
	SZ3 = (Z*Matrix[10]) / ZO;




	SXB1 = -SX1*(double)MaxX;
	SXB2 = -SX2*(double)MaxY;
	SXB3 = -SX3*(double)MaxZ;
	SYB1 = -SY1*(double)MaxX;
	SYB2 = -SY2*(double)MaxY;
	SYB3 = -SY3*(double)MaxZ;
	SZB1 = -SZ1*(double)MaxX;
	SZB2 = -SZ2*(double)MaxY;
	SZB3 = -SZ3*(double)MaxZ;



	SXYB1 = SXB1 + SX2;
	SXYB2 = SYB1 + SY2;
	SXYB3 = SZB1 + SZ2;


	SYZB1 = SXB2 + SX3;
	SYZB2 = SYB2 + SY3;
	SYZB3 = SZB2 + SZ3;




	FinalPoint[0] = Matrix[3] / XO;
	FinalPoint[1] = Matrix[7] / YO;
	FinalPoint[2] = Matrix[11] / ZO;

}

//--------------------------------------------------------------------------------------------------------------

void	Transform::SetTransformationSteps(double X, double Y, double Z, double XO, double YO, double ZO,
	int MaxX, int MaxY, int MaxZ, int StepX, int StepY, int StepZ)
{



	//dostosowane do affine

	SX1 = (X*Matrix[0]) / XO;
	SX2 = (Y*Matrix[1]) / XO;
	SX3 = (Z*Matrix[2]) / XO;

	SY1 = (X*Matrix[4]) / YO;
	SY2 = (Y*Matrix[5]) / YO;
	SY3 = (Z*Matrix[6]) / YO;

	SZ1 = (X*Matrix[8]) / ZO;
	SZ2 = (Y*Matrix[9]) / ZO;
	SZ3 = (Z*Matrix[10]) / ZO;




	SX1 *= (double)StepX;
	SX2 *= (double)StepY;
	SX3 *= (double)StepZ;

	SY1 *= (double)StepX;
	SY2 *= (double)StepY;
	SY3 *= (double)StepZ;

	SZ1 *= (double)StepX;
	SZ2 *= (double)StepY;
	SZ3 *= (double)StepZ;






	SXB1 = -SX1*(double)(int)(((MaxX - 1) / StepX) + 1);
	SXB2 = -SX2*(double)(int)(((MaxY - 1) / StepY) + 1);
	SXB3 = -SX3*(double)(int)(((MaxZ - 1) / StepZ) + 1);
	SYB1 = -SY1*(double)(int)(((MaxX - 1) / StepX) + 1);
	SYB2 = -SY2*(double)(int)(((MaxY - 1) / StepY) + 1);
	SYB3 = -SY3*(double)(int)(((MaxZ - 1) / StepZ) + 1);
	SZB1 = -SZ1*(double)(int)(((MaxX - 1) / StepX) + 1);
	SZB2 = -SZ2*(double)(int)(((MaxY - 1) / StepY) + 1);
	SZB3 = -SZ3*(double)(int)(((MaxZ - 1) / StepZ) + 1);


	SXYB1 = SXB1 + SX2;
	SXYB2 = SYB1 + SY2;
	SXYB3 = SZB1 + SZ2;


	SYZB1 = SXB2 + SX3;
	SYZB2 = SYB2 + SY3;
	SYZB3 = SZB2 + SZ3;




	FinalPoint[0] = Matrix[3] / XO;
	FinalPoint[1] = Matrix[7] / YO;
	FinalPoint[2] = Matrix[11] / ZO;







}


//-----------------------------------------------
void	Transform::SetZeroXCoordinate()
{
	FinalPoint[0] += SXB1;
	FinalPoint[1] += SYB1;
	FinalPoint[2] += SZB1;
	//PosX=0;
}
//-----------------------------------------------
void	Transform::SetZeroYCoordinate()
{
	FinalPoint[0] += SXB2;
	FinalPoint[1] += SYB2;
	FinalPoint[2] += SZB2;
	//PosY=0;
}
//-----------------------------------------------
void	Transform::SetZeroZCoordinate()
{
	FinalPoint[0] += SXB3;
	FinalPoint[1] += SYB3;
	FinalPoint[2] += SZB3;



	//PosZ=0;
}
//-----------------------------------------------
//-----------------------------------------------
//-----------------------------------------------
void	Transform::SetZeroYNextZCoordinate()
{
	FinalPoint[0] += SYZB1;
	FinalPoint[1] += SYZB2;
	FinalPoint[2] += SYZB3;
	//PosZ=0;
	//PosY++;
}
//-----------------------------------------------
void	Transform::SetZeroXNextYCoordinate()
{
	FinalPoint[0] += SXYB1;
	FinalPoint[1] += SXYB2;
	FinalPoint[2] += SXYB3;



	//PosY=0;
	//PosX++;
}

/////////////////////////////////////////////////////////////////////////





