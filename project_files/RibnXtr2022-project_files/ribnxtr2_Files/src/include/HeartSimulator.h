#ifndef HEART_SIMULATOR_H
#define HEART_SIMULATOR_H
	
#include <stdio.h>
#include <memory.h>
#include <qapplication.h>
#include <time.h>



#include	"RawDataSet.h"
#include	"DataSet.h"
#include	"LinkedList.h"
#include	"OtherCommonFunctions.h"
#include	"Transform.h"

#define M1 259200
#define IA1 7141
#define IC1 54773
#define RM1 (1.0/M1)
#define M2 134456
#define IA2 8121
#define IC2 28411
#define RM2 (1.0/M2)
#define M3 243000
#define IA3 4561
#define IC3 51349


/*#undef M1
#undef IA1
#undef 
#undef RM1
#undef M2
#undef IA2
#undef IC2
#undef RM2
#undef M3
#undef IA3
#undef IC3
*/

///////////////////////////////////////////////////////
class HeartSimulator :public QObject
{

	Q_OBJECT

	public:
		HeartSimulator();
		~HeartSimulator();

		void	GenerateDoubleEllipsoid(RawDataSet *data, double *center1,double *center2, double *radius1, double *radius2,int val,bool append=true);
		void	AddGaussianNoise(RawDataSet *data,double amount,double mean, double stddev);
		void	AddLines(RawDataSet *data,double minSize, double maxSize,double deltaAngle, int nrOfLines,unsigned short valMin, unsigned short valMax);
		void	AddSpecialNoise(RawDataSet *data,int rx,int ry,double amount,double mean, double stddev,bool changeDevs);

		void	AddRandomSpheres(RawDataSet *data,double minSize, double maxSize,int nr,unsigned short valMin, unsigned short valMax);
		void	AddRegularSpheres(RawDataSet *data,int stepx,int stepy,int stepz,int size,unsigned short valMin, unsigned short valMax);

		void	TransVolume(RawDataSet *src, RawDataSet *dst,double *params);
		void	DefineStdDevs(double stddev, double range, int x,int y);
		//void FilterDataSet2D(RawDataSet *FinalData,int iter,int cnr=0,double k0=5.0,double a=10.0,double dt=0.05){};
		//void FilterDataSet3D(RawDataSet *FinalData,int iter,int cnr=0,double k0=5.0,double a=10.0,double dt=0.05);
		//void FilterDataSet4D(RawDataSet *FinalData,int iter,int cnr=0,double k0=5.0,double a=10.0,double dt=0.05);

	private:

	
		int rxx,ryy;
		double *stddevs;

		RawDataSet *Data;
	//	LinkedList<DataSet*> *DataList;

	signals:

		void			SignalSendProgress(int);


};
	
//-----------------------------------------------
double    ran1 (int      *idum);
double    gasdev (int      *idum);

#endif //HEART_SIMULATOR_H
//////////////////////////////////////////////////////
