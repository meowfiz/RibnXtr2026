
#include "SegCriterion.h"
#include "OtherCommonFunctions.h"

SegCriterion::SegCriterion(RawDataSet * _data,int _neighSize, int _neighType, 	float _neighPercent)
{
#pragma omp parallel
	nrOfThreads = omp_get_num_threads();
	init();
	
	data = _data;
	SetRawData(data);//?
	neighSize = _neighSize;
	neighType = _neighType;
	neighPercent = _neighPercent;
	neighTab = NULL;
	neighTabSize = 0;

	dataArray = data->GetDataArray();
	ss = data->GetSliceSize();
	nx = data->GetNx();
	vsizem1 = data->GetVolumeSize() - 1;
	dataMin = data->GetMinValue();
	dataMax = data->GetMaxValue();
//	addTabForHist = { 1,nx,nx + 1,ss,ss + 1,ss + nx,ss + nx + 1 };
	dataRange = dataMax - dataMin;

	if (neighSize > 0)
	{
		if (!is2D)
			neighTab = GetGrowingKernel(neighTabSize, neighSize, data, neighType);
		else
			neighTab = GetGrowingKernel2D(neighTabSize, neighSize, data, neighType);

		samplesTab = new unsigned short[neighTabSize];
		posTab = new int[neighTabSize];
	}
};
//---------------------
bool SegCriterion::PerformStatTests(int pos, bool useInternaTabs, unsigned short* _samplesTab, int* _posTab,
												double* _currentHistogramNorm, int* _currentHistogram,
												double* _currentHistogramNorm2D,int* _currentHistogram2D,
												int *_neighTab,double * _normInputHistogram,double * _norm2DNeighHistogram)
{

	if (useInternaTabs == true)
	{
		_samplesTab = samplesTab;
		_posTab = posTab;
		_currentHistogramNorm = currentHistogramNorm;
		_currentHistogramNorm2D = currentHistogramNorm2D;
		_currentHistogram = currentHistogram;
		_currentHistogram2D = currentHistogram2D;

		_neighTab = neighTab;
		_normInputHistogram = normInputHistogram;
		_norm2DNeighHistogram = norm2DNeighHistogram;

	}

	bool test1 = true;
	bool test2 = true;
	bool test3 = true;
	if (neighTabSize > 0)
	{

		int samplesTabSize = GetSamplesTab(pos, _samplesTab, _posTab,_neighTab);
		if (samplesTabSize > 0)
		{


			test1 = CheckVoxelStdDev(_samplesTab, samplesTabSize);
			
			if (test1 == false)return false;

			if (histSimilarityThreshold > 0)
			{


				GetCurrentHistogram(_currentHistogram, _currentHistogramNorm, _samplesTab, samplesTabSize);
				double sim =  GetHistogramSimilarity(_normInputHistogram, _currentHistogramNorm, nrOfBins);
				if (sim < histSimilarityThreshold)test2 = true; else test2 = false;

			}
			if (test2 == false)return false;
			if (histSimilarityThreshold2D > 0)
			{

		

				GetCurrentHistogram2D(_currentHistogram2D,_currentHistogramNorm2D, _posTab, samplesTabSize);
				double sim = GetHistogramSimilarity(_norm2DNeighHistogram, _currentHistogramNorm2D, nrOfBins * nrOfBins);
				if (sim < histSimilarityThreshold2D)test3 = true; else test3 = false;
			
			}
			if (test3 == false)return false;
		}

	
	}
	return true;(test1 & test2 & test3);
}
//---------------------------------------
void SegCriterion::CheckAllVoxelsInVolume(unsigned char* blockMask, unsigned short* volumeTempData)
{

	//parallelComputing = true;
	int maxThr = (maxThreads > 0)?maxThreads:nrOfThreads;
//#pragma omp parallel
	//#pragma omp for   ordered schedule(guided) 

	if ((parallelComputing == false)||(maxThr<1))
	{
		if (blockMask != NULL)
		{
			//int nrOfThreads = omp_get_num_threads();
			//#pragma omp for   ordered schedule(guided) 
			for (int i = 0; i < data->GetVolumeSize(); i++)
			{
				bool test = false;

				if (blockMask[i] == 1) volumeTempData[i] = 0;
				else if (blockMask[i] == 2) volumeTempData[i] = 2;
				else
				{


					test = (CheckPoint(0, 0, 0, NULL, 0, i));

					volumeTempData[i] = (test == true) ? 2 : 0;
				}


			}
		}
		else
		{
			//#pragma omp for   ordered schedule(guided) 
			for (int i = 0; i < data->GetVolumeSize(); i++)
			{
				bool test = false;
				test = (CheckPoint(0, 0, 0, NULL, 0, i));
				volumeTempData[i] = (test == true) ? 2 : 0;
			}
		}
	}
	else
	{

		//int nrOfThreads = omp_get_num_threads();
	

		
		unsigned short** _samplesTab=new unsigned short*[maxThr];
		int** _posTab = new int* [maxThr];
		double** _currentHistogramNorm = new double* [maxThr];
		int** _currentHistogram = new int* [maxThr];
		double** _currentHistogramNorm2D = new double* [maxThr];
		int** _currentHistogram2D = new int* [maxThr];

		double** _normInputHistogram = new double* [maxThr];
		double** _norm2DNeighHistogram = new double* [maxThr];
		int** _neighTab = new int* [maxThr];

		for (int i = 0; i < maxThr; i++)
		{
			_samplesTab[i] = new unsigned short[neighTabSize];
			_posTab[i] = new int[neighTabSize];
			_currentHistogramNorm2D[i] = new double[nrOfBins * nrOfBins];
			_currentHistogram2D[i] = new int[nrOfBins * nrOfBins];
			_currentHistogramNorm[i] = new double[nrOfBins ];
			_currentHistogram[i] = new int[nrOfBins ];

			//skopiowac zawartosc z orginalow!!!
			_normInputHistogram[i] = new double[nrOfBins];
			_norm2DNeighHistogram[i] = new double[nrOfBins * nrOfBins];
			_neighTab[i] = new int[neighTabSize];

			memcpy(_normInputHistogram[i], normInputHistogram, nrOfBins * sizeof(double));
			memcpy(_norm2DNeighHistogram[i], norm2DNeighHistogram, nrOfBins * nrOfBins* sizeof(double));
			memcpy(_neighTab[i], neighTab, neighTabSize * sizeof(int));
		}

		
		if (blockMask != NULL)
		{
			//#pragma omp parallel 
			{
				//int tid = omp_get_thread_num();
				
#pragma omp parallel  for  ordered schedule (guided) num_threads(maxThr)//nrOfThreads)
				for (int i = 0; i < data->GetVolumeSize(); i++)
				{
					bool test = false;
					int tid = omp_get_thread_num();

					if (blockMask[i] == 1) volumeTempData[i] = 0;
					else if (blockMask[i] == 2) volumeTempData[i] = 2;
					else
					{
						

						test = (CheckPoint(0, 0, 0, NULL, 0, i, false, _samplesTab[tid], _posTab[tid],
							_currentHistogramNorm[tid], _currentHistogram[tid],
							_currentHistogramNorm2D[tid], _currentHistogram2D[tid],
							_neighTab[tid], _normInputHistogram[tid], _norm2DNeighHistogram[tid]));

						volumeTempData[i] = (test == true) ? 2 : 0;
						if (test == true)
						{
							int a = 5;
						}
					}


				}
			}
		}
		else
		{
			#pragma omp parallel 
			{
				int tid = omp_get_thread_num();
				for (int i = 0; i < data->GetVolumeSize(); i++)
				{

					bool test = false;
					test = (CheckPoint(0, 0, 0, NULL, 0, i, false, _samplesTab[tid], _posTab[tid],
						_currentHistogramNorm[tid], _currentHistogram[tid],
						_currentHistogramNorm2D[tid], _currentHistogram2D[tid]));
					volumeTempData[i] = (test == true) ? 2 : 0;
				}
			}
		}

		for (int i = 0; i < maxThr; i++)
		{
			delete[]_samplesTab[i];
			delete[]_posTab[i];
			delete[]_currentHistogramNorm2D[i];
			delete[]_currentHistogram2D[i];
			delete[]_currentHistogramNorm[i];
			delete[]_currentHistogram[i];
			delete[]_normInputHistogram[i];
			delete[]_norm2DNeighHistogram[i];
			delete[]_neighTab[i];
		}
		delete[]_samplesTab;
		delete[]_posTab;
		delete[]_currentHistogramNorm2D;
		delete[]_currentHistogram2D;
		delete[]_currentHistogramNorm;
		delete[]_currentHistogram;
		delete[]_normInputHistogram;
		delete[]_norm2DNeighHistogram;
		delete[]_neighTab;

	}


}
//---------------------------------------

double  SegCriterion::GetHistogramSimilarity(double *baseHistogram,double *currentHistogramNorm,int size)
{
	double val = 0;

	if ((baseHistogram != NULL) && (currentHistogramNorm != NULL))
	{
		int test=omp_get_num_threads();
		//manhattan
		const double* _baseHistogram = baseHistogram;
		const double* _currentHistogramNorm = currentHistogramNorm;
#pragma omp parallel for reduction(+ : val)
		for (int i = 0; i < size;i++)
		{
			val += (histSimilarityMode==0)?(fabs(_baseHistogram[i] - _currentHistogramNorm[i]))
				:
				((_currentHistogramNorm[i] > 0) ? fabs(_baseHistogram[i] - _currentHistogramNorm[i]) : 0)
				;
			//val += (_currentHistogramNorm[i] > 0) ? fabs(_baseHistogram[i] - _currentHistogramNorm[i]) : 0;
		}
	}


	return val;
}
//---------------------------------------
void SegCriterion::GetCurrentHistogram2D(int* currentHistogram, double* currentHistogramNorm, int *posTab, int samplesTabSize)
{
	if ((currentHistogram != NULL) && (posTab != NULL)&&(nrOfBins>0))
	{

		if (samplesTabSize > 0)
		{
			int addTabForHist[7] = { 1, nx, nx + 1, ss, ss + 1, ss + nx, ss + nx + 1 };
			int* hist2d = currentHistogram;
			memset(hist2d, 0, nrOfBins * nrOfBins * sizeof(int));
			int sum = 0;
			for (int i = 0; i < samplesTabSize; i++)
			{
				int pp = posTab[i];
				int val = dataArray[pp];//data->GetVoxelValue(i, j, k);

				int valSc1 = val - dataMin;
				valSc1 *= histScaleFactor;
				
				for (int m = 0;m < 7;m++)
				{
					int dd = pp + addTabForHist[m];
					if (dd < vsizem1)
					{
						//wstawiam do odpowiedniego histogramu w obie storny
						int val2 = dataArray[dd];
						int valSc2 = val2 - dataMin;
						valSc2 *= histScaleFactor;

						//wstawiam obie kombinacje ij i ji
						hist2d[valSc1 * nrOfBins + valSc2]++;
						hist2d[valSc2 * nrOfBins + valSc1]++;
						sum += 2;
					}
				}
			
			}
			double sc = 1.0 / sum;
			for (int i = 0; i < nrOfBins * nrOfBins; i++)
			{
				currentHistogramNorm[i] = currentHistogram[i] * sc;

			}


		}
	}
}
//------------------------------
void SegCriterion::GetCurrentHistogram(int * currentHistogram, double * currentHistogramNorm,unsigned short  *samplesTab,int samplesTabSize)
{
	if( (currentHistogram != NULL)&&(samplesTab!=NULL))
	{
	
		if (samplesTabSize > 0)
		{
			GetHistogram(samplesTab, samplesTabSize, currentHistogram, nrOfBins, dataMin, dataMax, true);

			
			double sc = 1.0 / samplesTabSize;
			for (int i = 0; i < nrOfBins; i++)
			{
				currentHistogramNorm[i] = currentHistogram[i] * sc;
			
			}
		}

	//	currentHistogramNorm
	}

}
//--------------------------------------

void SegCriterion::SetInput2DHistogram(int* input2Hist)
{
	if ((input2Hist != NULL) && (nrOfBins > 0))
	{
		if (norm2DNeighHistogram != NULL) delete[]norm2DNeighHistogram;
		if (neigh2DHistogram != NULL) delete[]neigh2DHistogram;
		if (currentHistogramNorm2D != NULL) delete[]currentHistogramNorm2D;
		if (currentHistogram2D != NULL) delete[]currentHistogram2D;


		norm2DNeighHistogram = new double [nrOfBins * nrOfBins];
		neigh2DHistogram = new int[nrOfBins * nrOfBins];

		currentHistogramNorm2D = new double[nrOfBins * nrOfBins];
		currentHistogram2D = new int[nrOfBins * nrOfBins];

	
		//memset(normInputHistogram, 0, nrOfBins* nrOfBins * sizeof(double));

		int sum = 0;
		for (int i = 0; i < nrOfBins* nrOfBins; i++)
		{
			neigh2DHistogram[i] = input2Hist[i];
			sum += neigh2DHistogram[i];
		}

		if (sum > 0)
		{
			double sc = 1.0 / sum;
			for (int i = 0; i < nrOfBins* nrOfBins; i++)
			{
				norm2DNeighHistogram[i] = neigh2DHistogram[i] * sc;
			}
		}



		
	}
}

void SegCriterion::SetInputHistogram(int* _inputHistogram, int _nrOfBins)
{
	if (inputHistogram != NULL) delete[]inputHistogram;
	if (normInputHistogram != NULL) delete[]normInputHistogram;
	if (currentHistogram != NULL) delete[]currentHistogram;
	if (currentHistogramNorm != NULL) delete[]currentHistogramNorm;

	

	nrOfBins = _nrOfBins;
	if (nrOfBins < 1)nrOfBins = 1;

	inputHistogram = new int[nrOfBins];
	normInputHistogram = new double[nrOfBins];
	currentHistogramNorm = new double[nrOfBins];
	currentHistogram = new int[nrOfBins];


	memset(inputHistogram, 0, nrOfBins * sizeof(int));
	//memset(currentHistogram, 0, nrOfBins * sizeof(int));
	//memset(normInputHistogram, 0, nrOfBins * sizeof(double));
	//memset(currentHistogramNorm, 0, nrOfBins * sizeof(double));
	 histScaleFactor = (double)(nrOfBins - 1) / dataRange;
	//int sum = 0;
	int sum = 0;
	for (int i = 0; i < nrOfBins; i++)
	{
		inputHistogram[i] = _inputHistogram[i];
		sum += inputHistogram[i];
	}

	if (sum > 0)
	{
		double sc = 1.0 / sum;
		for (int i = 0; i < nrOfBins; i++)
		{
			normInputHistogram[i] = inputHistogram[i] * sc;
		}
	}


	/*double* normInputHistogram;
	int* inputHistogram;
	int* currentHistogram;
	int nrOfBins;*/
}
//---------------------------------

bool SegCriterion::CheckVoxelStdDev(unsigned short * samplesTab, int samplesTabSize)
{
	bool test1 = true;

	//teraz test na stddev
	double tol = 1e-5;
	if (fabs(stdDevDelta) > tol)
	{
	//	if (stdDevMax < tol) stdDevMax = data->GetMaxValue();
		
		if (samplesTabSize > 0)
		{
			double _statParams[10];
			GetStatisticsFromUShortTab(samplesTab, samplesTabSize, _statParams, false);
			double stdDev = _statParams[3];

			if ((stdDev> stdDevGlobal-stdDevDelta)&& (stdDev < stdDevGlobal + stdDevDelta))
			//if ((stdDev > stdDevMin) && (stdDev < stdDevMax))
			{
				//return 1;
				test1 = true;
			}
			else
			{
				test1 = false;
			}
		}
	}
	return test1;
}
//----------------------------------------
int SegCriterion::  GetSamplesTab(int pos, unsigned short* samplesTab, int* posTab,int *_neighTab )
{ 
	if (_neighTab == NULL)
	{
		_neighTab = neighTab;
	}
	int size = 0;
	if ((samplesTab != NULL)&&(neighTabSize>0))
	{
		if (posTab != NULL)
		{
			int tmp;
			for (int j = 0; j < neighTabSize; j++)
			{
				tmp = pos + _neighTab[j];
				if ((tmp > dsizemin1) || (tmp < 0))
					continue;
				else
				{
					samplesTab[size] = dData[tmp];
					posTab[size] = tmp;
					
					size++;
				}
			}
		}
		else
		{
			int tmp;
			for (int j = 0; j < neighTabSize; j++)
			{
				tmp = pos + _neighTab[j];
				if ((tmp > dsizemin1) || (tmp < 0))
					continue;
				else
				{
					samplesTab[size] = dData[tmp];
					size++;
				}
			}
		}

	}
	return size;
}
//-----------------------------------------
//void  SegCriterion::GetSamplesTab(int pos)
//{
//
//	if (samplesTab != NULL)
//	{
//		samplesTabSize = 0;
//		int tmp;
//		for (int j = 0; j < neighTabSize; j++)
//		{
//			tmp = pos + neighTab[j];
//			if ((tmp > dsizemin1) || (tmp < 0))
//				break;
//			else
//			{
//				samplesTab[samplesTabSize++] = dData[tmp];
//			}
//		}
//
//	}
//
//}

SegCriterionThresholds::SegCriterionThresholds(RawDataSet *_data, int _lth, int _rth, int _sureTh, int _thMode, int _neighSize, int _neighType, float _neighPercent ):SegCriterion(_data, _neighSize,  _neighType, _neighPercent)
{

	lth = _lth;
	rth = _rth;
	/*neighSize = _neighSize;
	neighType = _neighType;*/
	//neighPercent = _neighPercent;
	data = _data;
	if (_sureTh != 0)
		sureTh = _sureTh;
	else
		sureTh = rth + 200;
	thMode = _thMode;
	// neightype - 0 cube, 1 sphere
//	neighTab = NULL;
	/*neighTabSize = 0;

	if (neighSize > 0)
	{
		if (!is2D)
		neighTab = GetGrowingKernel(neighTabSize, neighSize, data, neighType);
		else
			neighTab = GetGrowingKernel2D(neighTabSize, neighSize, data, neighType);
	}*/

}
//--------------------------------------------------

SegCriterionThresholds::~SegCriterionThresholds()
{
	/*if (neighTab != NULL)
	{
		delete[]neighTab;
		neighTab = NULL;
	}*/
}
//-------------------------------------------------
int SegCriterionThresholds::CheckPoint(int ix, int iy, int iz, unsigned short *tab, int rayTabSize, int pos,

	bool useInternaTabs , unsigned short* _samplesTab , int* _posTab ,
	double* _currentHistogramNorm , int* _currentHistogram ,
	double* _currentHistogramNorm2D , int* _currentHistogram2D, int *_neighTab, double* _normInputHistogram, double* _norm2DNeighHistogram
)
{
	if (CheckSqDistance(ix, iy, iz,pos) == false) return 0;
	int j;
	int val = 0;
	int testval = 0;
	int tmp;
	if (pos==-1)	 pos = data->GetVoxelPos(ix, iy, iz);
	
	

	if ((pos < 0) || (pos > dsizemin1))return 0;
	
	if (blockMask != NULL)
	{
		if (blockMask[pos] == blockValue)
		{
			return 0;
		}
		else if (blockMask[pos] == unblockValue)
		{
			return 1;
		}
	}
	
	if ((avoidValueExists == true) && (dData[pos] == avoidValue)) return 0;

	val = dData[pos];

	bool test = ((val > rth) || (val<lth));	if (thMode == false) test = !test;

	if (neighTabSize > 0)
	{
		bool testX = false;
		for (j = 0; j<neighTabSize; j++)
			{
				tmp = pos + neighTab[j];
				if ((tmp>dsizemin1) || (tmp<0))
					break;
				else
				{
					val = dData[tmp];
					bool test = ((val > rth) || (val<lth));	if (thMode == false) test = !test;
					if (test == true)	testval++;

				}

				
			}
		if (testval >= neighTabSize * neighPercent) testX = true;;
		test = testX;
	}
	
	return (PerformStatTests(pos, useInternaTabs, _samplesTab,  _posTab, _currentHistogramNorm,  _currentHistogram,	 _currentHistogramNorm2D,  _currentHistogram2D,  _neighTab,  _normInputHistogram, _norm2DNeighHistogram) & test);
	//return test;

//	return 0;


	/*for (j = 0; j<neighTabSize; j++)
	{
		tmp = pos + neighTab[j];
		if ((tmp>dsizemin1) || (tmp<0))
			break;
		else
		{
			val = dData[tmp];
			bool test = ((val > rth) || (val<lth));	if (thMode == false) test = !test;
			if (test == true)	testval++;

		}

		if (testval >= neighTabSize*neighPercent) return 1;
	}*/



	return 0;

}
//--------------------------------------



SegCriterionSimpleMask::SegCriterionSimpleMask(RawDataSet* _data, int _goodValue, int _badValue, unsigned char* _mask, int _neighSize, int _neighType, float _neighPercent) :SegCriterion(_data,_neighSize, _neighType, _neighPercent)
{

	
	/*neighSize = _neighSize;
	neighType = _neighType;
	neighPercent = _neighPercent;*/
	data = _data;


	mask = _mask;
	goodValue = _goodValue;
	badValue = _badValue;






	// neightype - 0 cube, 1 sphere
	/*neighTab = NULL;
	neighTabSize = 0;

	if (neighSize > 0)
	{
		if (!is2D)
			neighTab = GetGrowingKernel(neighTabSize, neighSize, data, neighType);
		else
			neighTab = GetGrowingKernel2D(neighTabSize, neighSize, data, neighType);
	}*/

}
//--------------------------------------------------

SegCriterionSimpleMask::~SegCriterionSimpleMask()
{/*
	if (neighTab != NULL)
	{
		delete[]neighTab;
		neighTab = NULL;
	}*/
}
//-------------------------------------------------
int SegCriterionSimpleMask::CheckPoint(int ix, int iy, int iz, unsigned short* tab, int rayTabSize, int pos,

	bool useInternaTabs, unsigned short* _samplesTab, int* _posTab,
	double* _currentHistogramNorm, int* _currentHistogram,
	double* _currentHistogramNorm2D, int* _currentHistogram2D, int *_neighTab, double* _normInputHistogram, double* _norm2DNeighHistogram
)
{
	if (CheckSqDistance(ix, iy, iz, pos) == false) return 0;
	int j;
	int val = 0;
	int testval = 0;
	int tmp;
	if (pos == -1)	 pos = data->GetVoxelPos(ix, iy, iz);
	//int	dsizemin1 = data->GetVolumeSize() - 1;
	//unsigned short* dData = data->GetDataArray();

	if ((pos < 0) || (pos > dsizemin1))return 0;
	if (blockMask != NULL)
	{
		if (blockMask[pos] == blockValue)
		{
			return 0;
		}
		else if (blockMask[pos] == unblockValue)
		{
			return 1;
		}
	}
	if ((avoidValueExists == true) && (dData[pos] == avoidValue)) return 0;


	bool test = false;
	if (mask != NULL)
	{
		if (mask[pos] == goodValue) test = true; // return 1; else return 0;
	}
	//bool test2 = true;
	//bool test1 = true;
	////if (test == true) return 1;
	//if (neighTabSize > 0)
	//{

	//	unsigned short* samplesTab = new unsigned short[neighTabSize];
	//	double* currentHistogramNorm = new double[nrOfBins];
	//	int* currentHistogram = new int[nrOfBins];
	//	int samplesTabSize = GetSamplesTab(pos, samplesTab);
	//	if (samplesTabSize > 0)
	//	{
	//		test1 = CheckVoxelStdDev(samplesTab, samplesTabSize);

	//		if (histSimilarityThreshold > 0)
	//		{


	//			GetCurrentHistogram(currentHistogram, currentHistogramNorm, samplesTab, samplesTabSize);
	//			double sim = GetHistogramSimilarity(currentHistogramNorm);
	//			if (sim < histSimilarityThreshold)test2 = true; else test2 = false;
	//		}
	//	}

	//	delete[]samplesTab;
	//	delete[]currentHistogramNorm;
	//	delete[]currentHistogram;

	//}

	//if ((test2 == true) && (test1 == true) && (test == true)) return true;


	return test;// nie robie statystyk bo morofologia ma uzupalniaæ!!!

	//return (PerformStatTests(pos, useInternaTabs, _samplesTab, _posTab, _currentHistogramNorm, _currentHistogram, _currentHistogramNorm2D, _currentHistogram2D,  _neighTab,  _normInputHistogram,  _norm2DNeighHistogram) & test);
	/*val = dData[pos];

	bool test = ((val > rth) || (val < lth));	if (thMode == false) test = !test;

	if (test == true) return 1;


	for (j = 0; j < neighTabSize; j++)
	{
		tmp = pos + neighTab[j];
		if ((tmp > dsizemin1) || (tmp < 0))
			break;
		else
		{
			val = dData[tmp];
			bool test = ((val > rth) || (val < lth));	if (thMode == false) test = !test;
			if (test == true)	testval++;

		}

		if (testval >= neighTabSize * neighPercent) return 1;
	}*/



	return 0;

}
//--------------------------------------




//-------------------------------------
SegCriterionProbabilityMap::SegCriterionProbabilityMap(RawDataSet *_data, double *_probabilityTab, double _percentage, double _minPerc, double _factorPerc, int _neighSize, int _neighType, float _neighPercent) :SegCriterion(_data,_neighSize, _neighType, _neighPercent)
{



	probabilityTab = _probabilityTab;
	percentage = _percentage;
	minPerc = _minPerc;
	factorPerc = _factorPerc;

	/*neighSize = _neighSize;
	neighType = _neighType;
	neighPercent = _neighPercent;*/
	data = _data;

	// neightype - 0 cube, 1 sphere
	/*neighTab = NULL;
	neighTabSize = 0;

	if (neighSize > 0)
	{
		if (!is2D)
			neighTab = GetGrowingKernel(neighTabSize, neighSize, data, neighType);
		else
			neighTab = GetGrowingKernel2D(neighTabSize, neighSize, data, neighType);
	}*/

}
//--------------------------------------------------

SegCriterionProbabilityMap::~SegCriterionProbabilityMap()
{
	//if (neighTab != NULL)
	//{
	//	delete[]neighTab;
	//	neighTab = NULL;
	//}
}
//-------------------------------------------------
int SegCriterionProbabilityMap::CheckPoint(int ix, int iy, int iz, unsigned short *tab, int rayTabSize, int pos,

	bool useInternaTabs, unsigned short* _samplesTab, int* _posTab,
	double* _currentHistogramNorm, int* _currentHistogram,
	double* _currentHistogramNorm2D, int* _currentHistogram2D, int *_neighTab, double* _normInputHistogram, double* _norm2DNeighHistogram
)
{
	if (CheckSqDistance(ix, iy, iz,pos) == false) return 0;
	int j;
	int val = 0;
	int testval = 0;
	int tmp;
	if (pos == -1)	 pos = data->GetVoxelPos(ix, iy, iz);
	//int	dsizemin1 = data->GetVolumeSize() - 1;
//	unsigned short *dData = data->GetDataArray();

	if ((pos < 0) || (pos > dsizemin1))return 0;
	if (blockMask != NULL)
	{
		if (blockMask[pos] == blockValue)
		{
			return 0;
		}
		else if (blockMask[pos] == unblockValue)
		{
			return 1;
		}
	}
	if ((avoidValueExists == true) && (dData[pos] == avoidValue))
	{
		return 0;
	}
	val = dData[pos];

	bool test = ((probabilityTab[val])*factorPerc > percentage);
	//bool test2 = true;
	//bool test1 = true;
	////if (test == true) return 1;
	//if (neighTabSize > 0)
	//{

	//	unsigned short* samplesTab = new unsigned short[neighTabSize];
	//	double* currentHistogramNorm = new double[nrOfBins];
	//	int* currentHistogram = new int[nrOfBins];
	//	int samplesTabSize = GetSamplesTab(pos, samplesTab);
	//	if (samplesTabSize > 0)
	//	{
	//		test1 = CheckVoxelStdDev(samplesTab, samplesTabSize);

	//		if (histSimilarityThreshold > 0)
	//		{


	//			GetCurrentHistogram(currentHistogram, currentHistogramNorm, samplesTab, samplesTabSize);
	//			double sim = GetHistogramSimilarity(currentHistogramNorm);
	//			if (sim < histSimilarityThreshold)test2 = true; else test2 = false;
	//		}
	//	}

	//	delete[]samplesTab;
	//	delete[]currentHistogramNorm;
	//	delete[]currentHistogram;

	//}
	//if ((test2 == true) && (test1 == true) && (test == true)) return true;
	return (PerformStatTests(pos, useInternaTabs,_samplesTab, _posTab, _currentHistogramNorm, _currentHistogram, _currentHistogramNorm2D, _currentHistogram2D,  _neighTab,  _normInputHistogram,  _norm2DNeighHistogram) & test);

	return 0;

}
//--------------------------------------



SegCriterionThresholdsWithRayCheck::SegCriterionThresholdsWithRayCheck(RawDataSet *_data, int _lth, int _rth, int _sureTh, int _thMode, int _neighSize, int _neighType, float _neighPercent) :SegCriterion(_data,_neighSize, _neighType, _neighPercent)
{

	lth = _lth;
	rth = _rth;
	//neighSize = _neighSize;
	//neighType = _neighType;
	//neighPercent = _neighPercent;
	data = _data;
	if (_sureTh != 0)
		sureTh = _sureTh;
	else
		sureTh = rth + 200;
	thMode = _thMode;
	//// neightype - 0 cube, 1 sphere
	//if (!is2D)
	//	neighTab = GetGrowingKernel(neighTabSize, neighSize, data, neighType);
	//else
	//	neighTab = GetGrowingKernel2D(neighTabSize, neighSize, data, neighType);

}
//--------------------------------------------------

SegCriterionThresholdsWithRayCheck::~SegCriterionThresholdsWithRayCheck()
{
	//if (neighTab != NULL)
	//{
	//	delete[]neighTab;
	//	neighTab = NULL;
	//}
}
//-------------------------------------------------
int SegCriterionThresholdsWithRayCheck::CheckPoint(int ix, int iy, int iz, unsigned short *rayTab, int rayTabSize, int pos,

	bool useInternaTabs, unsigned short* _samplesTab, int* _posTab,
	double* _currentHistogramNorm, int* _currentHistogram,
	double* _currentHistogramNorm2D, int* _currentHistogram2D, int *_neighTab, double* _normInputHistogram, double* _norm2DNeighHistogram
)
{
	if (CheckSqDistance(ix, iy, iz,pos) == false) return 0;

	int i, j;
	int val = 0;
	int testval = 0;
	int tmp;
	if (pos == -1)	 pos = data->GetVoxelPos(ix, iy, iz);
//	int	dsizemin1 = data->GetVolumeSize() - 1;
//	unsigned short *dData = data->GetDataArray();
	int testVal = 1;//ile najpier ma trafic
	int testVal2 = 3;

	// bede biegl wzdluz promienia
	// jelsli trafie na cos co spelnia a potem na cos co nie spelnia - zwracam 1 jesli nie to zero

	if ((pos < 0) || (pos > dsizemin1))return 0;
	if (blockMask != NULL)
	{
		if (blockMask[pos] == blockValue)
		{
			return 0;
		}
		else if (blockMask[pos] == unblockValue)
		{
			return 1;
		}
	}
	if ((avoidValueExists == true) && (dData[pos] == avoidValue)) return 0;

	if (thMode == 0) //to co dobre lezy pomiedzy progami
	{

		int temp = 0;
		int check = 0;
		for (i = 0; i<rayTabSize; i++)
		{
			val = rayTab[i];
			if ((val >= rth) || (val <= lth))
			{
				temp++;
			}
			if (temp == testVal) break;

		}
		temp = 0;
		for (j = i; j<rayTabSize; j++)
		{
			val = rayTab[j];
			if (!((val >= rth) || (val <= lth))) temp++;

			if (temp == testVal2)
			{
				check = 1;
				break;
			}

		}
		return check;



	}
	else //to co dobre lezy poza progami
	{

		int temp = 0;
		int check = 0;
		for (i = 0; i<rayTabSize; i++)
		{
			val = rayTab[i];
			if (!((val >= rth) || (val <= lth)))
			{
				temp++;
			}
			if (temp == testVal) break;

		}
		temp = 0;
		for (j = i; j<rayTabSize; j++)
		{
			val = rayTab[j];
			if (((val >= rth) || (val <= lth))) temp++;

			if (temp == testVal2)
			{
				check = 1;
				break;
			}

		}
		return check;

	}

	return 0;

}