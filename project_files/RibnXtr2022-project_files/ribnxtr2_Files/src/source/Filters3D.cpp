#include "Filters3D.h"

Filters3D::Filters3D()
{

#pragma omp parallel
	nrOfThreads =   omp_get_num_threads();

}
//---------------------------------------------
Filters3D::~Filters3D()
{
	
}
//---------------------------------------------
/*void AnisotropicDiffusion::ComputeGradient3D(int x, int y, int z)
{
}
*/


void	Filters3D::MedianFiltering(int radius, int kernelType, int mode, RawDataSet *Input, RawDataSet *_data)
{
	if ((Input != NULL) && (_data != NULL))
	{

		int kernelSize = 0;
		int tmpPos;
		int count;
		//int radius = 1;
		//int type = 0;
		//RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
		int *kernel = GetGrowingKernel(kernelSize, radius, Input, kernelType);
		//delete data;

	


		int pos = 0;
		//int l;

		double stddev, mean;
		double localMean, localStddev;
		//double nx1d=1.0/nx;

		double kVal, fVal;
		double cVal;
		double cMinVal;
		double finalVal;
		int chgs;
		int tmpPos2;

		chgs = 0;
		pos = 0;

		int volSize = Input->GetVolumeSize();
		int dsizemin1 = volSize - 1;
		unsigned short *tab = Input->GetDataArray();
		unsigned short *usData0 = _data->GetDataArray();


		//nrOfThreads = omp_get_num_threads();
		unsigned short **tabz;

		tabz = new unsigned short *[nrOfThreads];

		for (int i = 0; i<nrOfThreads; i++)
		{
			tabz[i] = new unsigned short[kernelSize];
		}

		//int tid = omp_get_thread_num();
		
		int progress = 0;
		//double prevProgress = 0;
		int inc =  volSize/100;
		double mul = 100.0 / volSize;

#pragma omp parallel  for  ordered schedule (guided) num_threads(nrOfThreads)// reduction(+:progress)
//#pragma omp parallel  num_threads(nrOfThreads)// private(counts)
		for (int j = 0; j < volSize; j++)
		{

		
			int tmpSize;
			int tid = omp_get_thread_num();
			GetKernelIntesitiesToTab(tab, tabz[tid], tmpSize, kernel, kernelSize, j, dsizemin1);
			if (tmpSize > 1)
			{
				switch (mode)
				{
				case 0:
					usData0[j] = Median(tabz[tid], tmpSize - 1);
					break;
				case 1:
					usData0[j] = Minimum(tabz[tid], tmpSize - 1);
					break;
				case 2:
					usData0[j] = Maximum(tabz[tid], tmpSize - 1);
					break;

				}
			}
			else
			{
				usData0[j] = tab[j];
			}

			//#pragma omp critical  
			{
				#pragma omp atomic
				progress++;

				//#pragma omp master
				//if ((int)progress != (int)prevProgress)
//#pragma omp master
				//if ((progress + 1) % inc == 0)
				//{
				//	emit this->SignalSendProgress((int)(progress*mul));
				//	//cout << (int)(progress*mul) <<endl;
				//}

				//#pragma omp atomic
				//prevProgress = progress;

			}


		}

		for (int i = 0; i<nrOfThreads; i++)
		{
			delete[]tabz[i]; tabz[i] = NULL;
		}
		delete[]tabz;
		delete[]kernel;
	


	}

	
}
//---------------------------------------------
void	Filters3D::GaussianSmoothingFiltering(double stddev, int radius, RawDataSet *Input, RawDataSet *_data)
{

	if ((Input != NULL) && (_data != NULL))
	{

		//radius=50.0;
		//stddev=1;

		int					size2 = 2 * radius + 1;
		double				*gaussianKernelTab = new double[size2];
		unsigned short		*intensityTab = new unsigned short[size2];
		GetGaussianKernel(gaussianKernelTab, (double)radius, stddev);

		//transversal 1d gauss

		RawDataSet rData = *Input;
		RawDataSet *tempData = &rData;

		unsigned short *buff0 = _data->GetDataArray();
		unsigned short *buff1 = new unsigned short[_data->GetVolumeSize()];

		unsigned short *Q;
		int i, j, k;
		int pos;
		unsigned short **tabz;
		//int nrOfThreads = 1;

		tabz = new unsigned short *[nrOfThreads];
		for (i = 0; i < nrOfThreads; i++)
		{
			tabz[i] = new unsigned short[size2];
		}




			int n[3];
			_data->GetN(n);
			int sliceSize = n[0] * n[1];


			Q = Input->GetDataArray();
			pos = 0;
//#pragma omp parallel for  ordered schedule (guided) 
//#pragma omp parallel  num_threads(nrOfThreads) //private(counts)

			int volSize = Input->GetVolumeSize();
			int inc = (n[2] *3) / 100;
			double mul = 100.0 / (n[2] *3);
			int progress = 0;

#pragma omp parallel  for  ordered schedule (guided) num_threads(nrOfThreads) 
			for (int k = 0; k < n[2]; k++)
			{
				int  tid = omp_get_thread_num();
				CalculateGaussianSlice(Q, buff0, n, radius, tabz[tid], gaussianKernelTab, k, 0);
				#pragma omp atomic
				progress++;
			//	if ((progress + 1) % inc == 0) { emit this->SignalSendProgress((int)(progress*mul)); }
				
			}
			//tempData2=tempData;	tempData=_data;	_data=tempData2;	tempData2=NULL;

			//Q=buff1;
			pos = 0;
//#pragma omp parallel for  ordered schedule (guided) 
//#pragma omp parallel  num_threads(nrOfThreads)
#pragma omp parallel  for  ordered schedule (guided) num_threads(nrOfThreads) 
			for (int k = 0; k < n[2]; k++)
			{
				int  tid = omp_get_thread_num();
				CalculateGaussianSlice(buff0, buff1, n, radius, tabz[tid], gaussianKernelTab, k, 1);
				#pragma omp atomic
				progress++;
			//	if ((progress + 1) % inc == 0) { emit this->SignalSendProgress((int)(progress*mul)); }

				
			}
			//	tempData2=tempData;	tempData=_data;	_data=tempData2;	tempData2=NULL;	Q=_data->GetDataArray();

			if (_data->GetNz() > radius)
			{
				Q = _data->GetDataArray();
				pos = 0;
//#pragma omp parallel for  ordered schedule (guided) 
//#pragma omp parallel  num_threads(nrOfThreads)
#pragma omp parallel  for  ordered schedule (guided) num_threads(nrOfThreads) 
				for (int k = 0; k < n[2]; k++)
				{
					int  tid = omp_get_thread_num();
					CalculateGaussianSlice(buff1, buff0, n, radius, tabz[tid], gaussianKernelTab, k, 2);
					#pragma omp atomic
					progress++;
					//if ((progress + 1) % inc == 0) { emit this->SignalSendProgress((int)(progress*mul)); }

					
				}

			}
			else
			{
				memcpy(_data->GetDataArray(), tempData->GetDataArray(), _data->GetVolumeSize() * sizeof(unsigned short));
			}



			tempData = NULL;

			Q = NULL;
			delete[]gaussianKernelTab;
			delete[]intensityTab;
			delete[]buff1;
		
		for (i = 0; i < nrOfThreads; i++)
		{
			delete[]tabz[i]; tabz[i] = NULL;
		}
		delete[]tabz;
	}
	
}
//-------------------------------------------------
void Filters3D::CalculateGaussianSlice(unsigned short *inQ, unsigned short *outQ, int n[3], int radius, unsigned short *tab, double *gaussianTab, int k, int view)
{
	int		size2 = 2 * radius + 1;
	int sliceSize = n[0] * n[1];
	for (int j = 0; j < n[1]; j++)
	{


		for (int i = 0; i <n[0]; i++)
		{

			GetIntensityTab1D(inQ, n[0], n[1], n[2], view, radius, tab, i, j, k);

			int pos = k*sliceSize + j*n[0] + i;
			outQ[pos] = Apply1DNormFilter(gaussianTab, tab, size2);
		}

	}

}

//---------------------------------------------------------------------------------
RawDataSet *Filters3D::Resample(RawDataSet *data, int newN[3], double newS[3], int mode, unsigned char*& maskOut, unsigned char* maskIn)
{
	RawDataSet *nData = new RawDataSet(newN[0], newN[1], newN[2], newS[0], newS[1], newS[2]);
	if (maskIn != NULL)
	{
		if (maskOut != NULL)
		{
			delete[]maskOut;
		}
			maskOut = new unsigned char[nData->GetVolumeSize()];
			memset(maskOut, 0, nData->GetVolumeSize());
		
	}
	double origin[3];
	data->GetOrigin(origin);
	nData->SetOrigin(origin);
	nData->dicomMinValue = data->dicomMinValue;
	nData->dicomLevelValue = data->dicomLevelValue;
	nData->dicomWindowWidthValue = data->dicomWindowWidthValue;
	nData->dicomRescaleValue = data->dicomRescaleValue;

	RawDataSet *DataSetFiltered = nData;
	RawDataSet *Input = data;

	if ((DataSetFiltered->GetNx() == Input->GetNx())
		&& (DataSetFiltered->GetNy() == Input->GetNy())
		&& (DataSetFiltered->GetNz() == Input->GetNz()))
	{

		memcpy(DataSetFiltered->GetDataArray(), Input->GetDataArray(), Input->GetVolumeSize() * sizeof(unsigned	short));
		//emit SignalSendProgress(100);
		if (maskOut != NULL)
		{
			memcpy(maskOut, maskIn, Input->GetVolumeSize() * sizeof(unsigned	char));
		}

	}
	else
	{


		//double XX, YY, ZZ;


		double SizeX = ((double)Input->GetNx() - 1) / ((double)DataSetFiltered->GetNx() - 1);
		double SizeY = ((double)Input->GetNy() - 1) / ((double)DataSetFiltered->GetNy() - 1);
		double SizeZ = ((double)Input->GetNz() - 1) / ((double)DataSetFiltered->GetNz() - 1);


		InterpolationFunc *Func;

		int	interpolation = mode;

		switch (interpolation)
		{
		case 0:
			Func = new NearestNeighborInterpolation();
			break;
		case 1:
			Func = new LinearInterpolation();
			break;
		case 2:
			Func = new CosineInterpolation();
			break;
		case 3:
			Func = new Cubic4Interpolation();
			break;
		case 4:
			Func = new Spline6Interpolation();
			break;
		case 5:
			Func = new CatmullRomSplineInterpolation();
			break;
		case 6:
			Func = new HermiteInterpolation();
			break;
		case 7:
			Func = new HermiteLaurentInterpolation();
			break;
		case 8:
			Func = new WelchSincInterpolation();
			break;
		case 9:
			Func = new HannHammingSincInterpolation();
			break;
		case 10:
			Func = new LanczosSincInterpolation();
			break;



		}

		Interpolation *Interp = new Interpolation(Input, Func);
		double proc = (double)(100.0 / (DataSetFiltered->GetNz() - 1));

		unsigned short *QQ = DataSetFiltered->GetDataArray();
		// tylko inteprolacja w	Z
		int maskPos = 0;
		if ((DataSetFiltered->GetNx() == Input->GetNx()) && (DataSetFiltered->GetNy() == Input->GetNy()))
		{
			
			double dk = 0;
			for (int k = 0; k < DataSetFiltered->GetNz(); k++)
			{
				double dj = 0;
				for (int j = 0; j < DataSetFiltered->GetNy(); j++)
				{
					double di = 0;
					for (int i = 0; i < DataSetFiltered->GetNx(); i++)
					{
						
						*(QQ++) = Interp->Interpolate1DZ(di, dj, dk);
						if (maskOut != NULL)
						{
							maskOut[maskPos++] = maskIn[Input->GetVoxelPos(di, dj, dk)];
						}
						
						di += SizeX;
					}
					dj += SizeY;
				}
				dk += SizeZ;
			
			//	emit SignalSendProgress((int)(proc*k));
			}
		}
		else
		{
			if (DataSetFiltered->GetNz() == Input->GetNz())
			{
				double dk = 0;
				int sliceSize = Input->GetSliceSize();
				unsigned short *tab2D = Input->GetDataArray();
				
				for (int k = 0; k < DataSetFiltered->GetNz(); k++)
				{
					//qDebug() << k;
					double dj = 0;
					if (k == 49) {
						k = k;
					}
					for (int j = 0; j < DataSetFiltered->GetNy(); j++)
					{
						double di = 0;
						for (int i = 0; i < DataSetFiltered->GetNx(); i++)
						{

							*(QQ++) = Interp->Interpolate2D(di, dj, tab2D);
							if (maskOut != NULL)
							{
								maskOut[maskPos++] = maskIn[Input->GetVoxelPos(di, dj, dk)];
							}
							di += SizeX;
						}
						dj += SizeY;
					}
					
					dk += SizeZ;
					tab2D += sliceSize;
					//	emit SignalSendProgress((int)(proc*k));
				}
			}
			else
			{
				double dk = 0;
				for (int k = 0; k < DataSetFiltered->GetNz(); k++)
				{
					double dj = 0;
					for (int j = 0; j < DataSetFiltered->GetNy(); j++)
					{
						double di = 0;
						for (int i = 0; i < DataSetFiltered->GetNx(); i++)
						{

							*(QQ++) = Interp->Interpolate3D(di, dj, dk);
							if (maskOut != NULL)
							{
								maskOut[maskPos++] = maskIn[Input->GetVoxelPos(di, dj, dk)];
							}
							di += SizeX;
						}
						dj += SizeY;
					}
					dk += SizeZ;

					//	emit SignalSendProgress((int)(proc*k));
				}
			}
		}
		delete Func;
		delete Interp;
	}
	return nData;






}