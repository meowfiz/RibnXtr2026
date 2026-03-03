#ifndef SEG_CRITERION
#define SEG_CRITERION

#include "RawDataSet.h"
#include	<omp.h>
//----------------------------------------------

class SegCriterion
{

public:
	SegCriterion(RawDataSet * _data,int _neighSize, int _neighType, float _neighPercent);
	

	SegCriterion()
	{
		init();
	};

	void init()
	{
		maxThreads = 0;
		//nrOfThreads = 1;
		is2D = false; avoidValueExists = false; seedSet = false;
		euclideanDistance = 0;
		dx2 = dy2 = dz2 = 1;
		data = NULL;
		blockMask = NULL;
		blockValue = 1;
		unblockValue = 2;
		dData = NULL;
		dsizemin1 = 0;
		/*stdDevMin = 0;
		stdDevMax=0;*/
		stdDevDelta = 0;
		stdDevGlobal = 0;
		inputHistogram = NULL;
		nrOfBins = 0;
		normInputHistogram = NULL;
		norm2DNeighHistogram = NULL;
		neigh2DHistogram = NULL;
		//currentHistogram = NULL;
	//	currentHistogramNorm = NULL;
		histSimilarityThreshold = 0;
		histSimilarityThreshold2D=0;

		samplesTab = NULL;
		posTab = NULL;

		currentHistogramNorm = NULL;
		currentHistogram = NULL;

		currentHistogramNorm2D = NULL;
		currentHistogram2D = NULL;
		parallelComputing = true;
		histSimilarityMode = 0;

	};
	~SegCriterion()
	{ 
		if (neighTab != NULL)
		{
			delete[]neighTab;
			neighTab = NULL;
		//	delete[]samplesTab;
			//samplesTab = NULL;
		}
		if (inputHistogram != NULL)delete[]inputHistogram;
		if (normInputHistogram != NULL)delete[]normInputHistogram;
		if (norm2DNeighHistogram != NULL)delete[]norm2DNeighHistogram;
		if (neigh2DHistogram != NULL)delete[]neigh2DHistogram;
		/*if (currentHistogram != NULL)delete[]currentHistogram;
		if (currentHistogramNorm != NULL)delete[]currentHistogramNorm;*/
		

		if (samplesTab != NULL)delete[]samplesTab;
		if (posTab != NULL)delete[]posTab;
		if (currentHistogramNorm != NULL)delete[]currentHistogramNorm;
		if (currentHistogram != NULL)delete[]currentHistogram;
		if (currentHistogramNorm2D != NULL)delete[]currentHistogramNorm2D;
		if (currentHistogram2D != NULL)delete[]currentHistogram2D;

	



	};

	//bool PerformStatTests(int pos);
	bool SegCriterion::PerformStatTests(int pos, bool useInternaTabs=false, unsigned short* samplesTab=NULL, int* posTab = NULL,
		double* currentHistogramNorm = NULL, int* currentHistogram = NULL,
		double* currentHistogramNorm2D = NULL, int* currentHistogram2D = NULL,  int* _neighTab = NULL, double* _normInputHistogram = NULL, double* _norm2DNeighHistogram = NULL);
	void CheckAllVoxelsInVolume(unsigned char* blockMask, unsigned short* volTempData);
	void SetHistSimilarityThreshold(double th) { histSimilarityThreshold = th; };
	void SetHistSimilarityThreshold2D(double th) { histSimilarityThreshold2D = th; };
	double GetHistogramSimilarity(double *baseHist,double* currentHistogramNorm,int size);
	bool CheckVoxelStdDev(unsigned short* samplesTab, int samplesTabSize);
	void GetCurrentHistogram(int* currentHistogram, double* currentHistogramNorm, unsigned short* samplesTab, int samplesTabSize);
	void GetCurrentHistogram2D(int* currentHistogram, double* currentHistogramNorm, int *postab, int samplesTabSize);

	void SetInputHistogram(int* _inputHistogram, int _nrOfBins);
	void SetInput2DHistogram(int* input2Hist);
	void SetHistSimilarityMode(int mode) { histSimilarityMode = mode; };
	
	int maxThreads;
	unsigned short* samplesTab;// = new unsigned short[neighTabSize];
	int* posTab;// = new int[neighTabSize];

	double* currentHistogramNorm;// = new double[nrOfBins];
	int* currentHistogram;// = new int[nrOfBins];

	double* currentHistogramNorm2D;// = new double[nrOfBins * nrOfBins];
	int* currentHistogram2D;// = new int[nrOfBins * nrOfBins];



	int nrOfThreads;
	int* neigh2DHistogram;
	double* norm2DNeighHistogram;
	double* normInputHistogram;

	//double* inputHistogram;
	int* inputHistogram;
	//int* currentHistogram;
	//double* currentHistogramNorm;
	int nrOfBins;

	double histSimilarityThreshold;
	double histSimilarityThreshold2D;
	//double stdDevMin, stdDevMax;
	double stdDevDelta;
	double stdDevGlobal;
	bool getNeighbours;
	//unsigned short* samplesTab;
	//int samplesTabSize;
	float neighPercent;
	int neighSize;
	int neighType;
	int* neighTab;
	int neighTabSize;
	RawDataSet *data;
	bool is2D; 
	bool avoidValueExists;
	unsigned short avoidValue;
	double euclideanDistance;
	double seedPoint;
	double dx2, dy2, dz2;
	int seed[3];
	bool seedSet;
	int dsizemin1;
	unsigned short* dData;

	unsigned char* blockMask;
	int blockValue ;
	int unblockValue;
	double statParams[10];


	unsigned short* dataArray;
	int ss;
	int nx;
	int vsizem1;
	int dataMin;
	int dataMax;
	
	int dataRange; 
	double histScaleFactor;
	bool parallelComputing;
	//bool parallelComputing;
	int histSimilarityMode;

	void SetBlockMask(unsigned char* bm, int bl = 1, int ubl = 2)
	{
		blockMask=bm;
		blockValue=bl;
		unblockValue=ubl;
	};

	/*void SetStdDevRange(double minS, double maxS) 
	{
		stdDevMin = minS; stdDevMax = maxS;
	};*/

	void SetStdDev(double s,double d)
	{
		stdDevGlobal = s;
		stdDevDelta = d;
	};
	void SetParallelComputing(bool mode,int _maxThreads=0)
	{
		parallelComputing = mode;maxThreads = _maxThreads;
	}
//	void GetSamplesTab(int pos);
	int  GetSamplesTab(int pos,unsigned short *tab,int *posTab=NULL,int *neighTab=NULL);



	void SetEuclideanDistanceAndSeedPoint(double dist, int point[3])
	{
		euclideanDistance = dist;
		seed[0] = point[0];
		seed[1] = point[1];
		seed[2] = point[2];
		seedSet = true;
		if (data != NULL)
		{

			dx2 = data->GetXscale() * data->GetXscale();
			dy2 = data->GetYscale() * data->GetYscale();
			dz2 = data->GetZscale() * data->GetZscale();
		}
	};

	void SetRawData(RawDataSet *rawData){ 
		data = rawData; 	dsizemin1 = data->GetVolumeSize() - 1; dData = data->GetDataArray();
	};
	void Set2DMode(bool mode){ is2D = mode; };
	void SetAvoidValue(bool mode, unsigned short val)
	{
		avoidValueExists = mode;
		avoidValue = val;
	
	};
	virtual int CheckPoint(int x, int y, int z, unsigned short *rayTab = NULL, int rayTabSize = 0, int pos = -1,

		bool useInternaTabs = true, unsigned short* _samplesTab = NULL, int* _posTab = NULL,
		double* _currentHistogramNorm = NULL, int* _currentHistogram = NULL,
		double* _currentHistogramNorm2D = NULL, int* _currentHistogram2D = NULL, int *_neighTab=NULL, double* _normInputHistogram=NULL, double* _norm2DNeighHistogram=NULL
	) = 0;

	bool CheckSqDistance(int x, int y, int z,int pos=-1)
	{
		if ((seedSet == true) && (euclideanDistance > 0))
		{

			double dx, dy, dz=0;
			if (pos < 0)
			{
				dx = (x - seed[0]) * (x - seed[0]) * dx2;
				dy = (y - seed[1]) * (y - seed[1]) * dy2;
				dz = (z - seed[2]) * (z - seed[2]) * dz2;
			}
			else if (data!=NULL)
			{
				int pp[3];
				data->GetVoxelXYZPos(pos, pp);
				dx = (pp[0] - seed[0]) * (pp[0] - seed[0]) * dx2;
				dy = (pp[1] - seed[1]) * (pp[1] - seed[1]) * dy2;
				dz = (pp[2] - seed[2]) * (pp[2] - seed[2]) * dz2;
			}
			else
			{
				return false;
			}
			double d = dx + dy + dz;
			if (d < euclideanDistance) return true;
			else return false;
		}
		return true;
	}
};
//-------


class SegCriterionThresholds : public SegCriterion
{
public:
	SegCriterionThresholds(RawDataSet *_data, int _lth, int _rth, int _sureTh, int _thMode, int _neighSize, int neighType, float _neighPercent);
	~SegCriterionThresholds();

	int CheckPoint(int x, int y, int z, unsigned short *rayTab = NULL, int rayTabSize = 0, int pos = -1,
	
	bool useInternaTabs=true, unsigned short* _samplesTab=NULL, int* _posTab = NULL,
		double* _currentHistogramNorm = NULL, int* _currentHistogram = NULL,
		double* _currentHistogramNorm2D = NULL, int* _currentHistogram2D = NULL,  int* _neighTab = NULL, double* _normInputHistogram = NULL, double* _norm2DNeighHistogram = NULL
	);

	int lth, rth;
	
	//, neighType;


//	int neighTabSize;
	//int *neighTab;
	//RawDataSet *data;
	int sureTh;
	int thMode;

};
//-------------------------------------------

class SegCriterionProbabilityMap : public SegCriterion
{
public:
	SegCriterionProbabilityMap(RawDataSet *_data, double *_probabilityTab, double _percentage, double _minPerc, double _factorPerc, int _neighSize, int neighType, float _neighPercent);
	~SegCriterionProbabilityMap();

	int CheckPoint(int x, int y, int z, unsigned short *rayTab = NULL, int rayTabSize = 0,int pos=-1,

		bool useInternaTabs = true, unsigned short* _samplesTab = NULL, int* _posTab = NULL,
		double* _currentHistogramNorm = NULL, int* _currentHistogram = NULL,
		double* _currentHistogramNorm2D = NULL, int* _currentHistogram2D = NULL, int* _neighTab = NULL, double* _normInputHistogram = NULL, double* _norm2DNeighHistogram = NULL
	);



	double *probabilityTab;
	double percentage;
	double minPerc;
	double factorPerc;


	//int neighType;
	//float neighPercent;

	//int neighTabSize;
	//int *neighTab;
	//RawDataSet *data;


};



//---------------------------------
class SegCriterionThresholdsWithRayCheck : public SegCriterion
{
public:
	SegCriterionThresholdsWithRayCheck(RawDataSet *_data, int _lth, int _rth, int _sureTh, int _thMode, int _neighSize, int neighType, float _neighPercent);
	~SegCriterionThresholdsWithRayCheck();

	int CheckPoint(int x, int y, int z, unsigned short *rayTab = NULL, int rayTabSize = 0, int pos = -1,

		bool useInternaTabs = true, unsigned short* _samplesTab = NULL, int* _posTab = NULL,
		double* _currentHistogramNorm = NULL, int* _currentHistogram = NULL,
		double* _currentHistogramNorm2D = NULL, int* _currentHistogram2D = NULL, int* _neighTab = NULL, double* _normInputHistogram = NULL, double* _norm2DNeighHistogram = NULL
	);

	int lth, rth;
	/*, neighType;
	float neighPercent;

	int neighTabSize;
	int *neighTab;*/
	//RawDataSet *data;
	int sureTh;
	int thMode;


};
//--------------------------


//---------------------------------
class SegCriterionSimpleMask : public SegCriterion
{
public:
	SegCriterionSimpleMask(RawDataSet* _data, int _goodValue, int _badValue, unsigned char*_mask, int _neighSize, int neighType, float _neighPercent);
	~SegCriterionSimpleMask();

	int CheckPoint(int x, int y, int z, unsigned short* rayTab = NULL, int rayTabSize = 0, int pos = -1,

		bool useInternaTabs = true, unsigned short* _samplesTab = NULL, int* _posTab = NULL,
		double* _currentHistogramNorm = NULL, int* _currentHistogram = NULL,
		double* _currentHistogramNorm2D = NULL, int* _currentHistogram2D = NULL, int* _neighTab = NULL, double* _normInputHistogram = NULL, double* _norm2DNeighHistogram = NULL
	);


	int goodValue, badValue;
	unsigned char* mask;

	/*int  neighType;
	float neighPercent;

	int neighTabSize;
	int* neighTab;*/
	//RawDataSet *data;



};
//-------------------------------------------

#endif // SEG_CRITERION
