#ifndef Filters3D_H
#define Filters3D_H
	
#include <stdio.h>
#include <memory.h>
#include <qapplication.h>
#include "qobject.h"
#include	<omp.h>

#include	"RawDataSet.h"
#include	"DataSet.h"
#include	"LinkedList.h"
#include	"OtherCommonFunctions.h"
#include	"Interpolation.h"
//#include <ppl.h>
// using namespace Concurrency;
///////////////////////////////////////////////////////
class Filters3D : public QObject
{

	Q_OBJECT

	public:
		Filters3D();
		~Filters3D();
		int nrOfThreads;
		void	GaussianSmoothingFiltering(double stddev, int radius, RawDataSet *Input, RawDataSet *_data);
		void CalculateGaussianSlice(unsigned short *inQ, unsigned short *outQ, int n[3], int radius, unsigned short *tab, double *gaussianTab, int k, int view);
		void	MedianFiltering(int radius, int kernelType, int mode, RawDataSet *Input, RawDataSet *_data);
		RawDataSet *Resample(RawDataSet *data, int newN[3], double newS[3], int mode,unsigned char *&maskOut, unsigned char* maskIn = NULL);

	signals:

		void			SignalSendProgress(int);


};

#endif //ANISOTROPIC_DIFFUSION
//////////////////////////////////////////////////////
