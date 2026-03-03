#ifndef	LAYER_FUILTERS_H
#define	LAYER_FUILTERS_H

#include <QDebug>
#include <qwidget.h>
#include <qpolygon.h>
#include <qtextstream.h>
#include <qvector.h>
#include <list>
#include <math.h>
#include <stdlib.h>

#include "RawDataSet.h"
#include "OtherCommonFunctions.h"
#include "DataSet.h"
#include "GUIPanelWidget.h"

#include "Transform.h"
#include "LinkedList.h"
#include "ImageShapes.h"
#include "ImageWidget.h"
#include "ctmf.h"
#include "qx_basic.h" 
#include "qx_constant_time_bilateral_filter_published.h"

#include "RawDataMixerLite.h"
#include <omp.h>
#include "LayerGenericFilter.h"
using namespace std;

#if defined (USG)
#include <SCPP.h>
#endif


class RawDataMixerLite;
// c

class lFilterColumn
{
	//private:
public:
	double *tab;
	int size;
	int count;
	double sum;
	double sum2;
	lFilterColumn *next;

	lFilterColumn(int _size) { size = _size; tab = new double[size]; };
	~lFilterColumn() { delete[]tab; };

};

struct lFilterColumn2
{
	//private:
	//public:
	//	double *tab;
	//int size;
	int count;
	long sum;
	long sum2;
	lFilterColumn2 *next;

	//lFilterColumn(int _size){	size=_size; tab=new double[size];	};
	//~lFilterColumn(){delete []tab;};

};
class LayerFilter : public QObject,public LayerGenericFilter
{

	Q_OBJECT
	//#define PARAMS_NR 50

protected:
	//int		nrOfThreads;
	//int		prevLayerID;

	//QString filterName;
	//QString		paramsNames[PARAMS_NR];
	//QString		addText[PARAMS_NR];
	//QStringList	showTextList;
	//int			showTextStartStop[PARAMS_NR*2];
	//double		defaultParams[PARAMS_NR];
	//bool		paramsSliders[PARAMS_NR]; //czy dodac slider przy gui
	//double		minVals[PARAMS_NR]; //min max jesli jest slider
	//double		maxVals[PARAMS_NR];//
	//double		steps[PARAMS_NR];//step dla slidera - glownie po to, zeby wiedziec czy po calkowitych czy po ulamkach
	//int			numberOfParams;
	//
	void		invertData(unsigned short *data, int size, int maxValue);
	void		updateBrightness(unsigned short *input, unsigned short *data, int size, int maxValue, bool invData, int bMode);

	//unsigned short	maxValue;

	//unsigned char *mainMask;
	//int sliceSize;
	//int position[2];
	void		ApplyWeightMap(unsigned short *inTab, unsigned short *filter, unsigned short *outTab, unsigned short *weightMap, unsigned short maxWeightVal, int size, double amount, bool invertWeights);
	//unsigned short		GetMaxValue();
	//QPolygon polyline;
	//int globalLeftThreshold;
	//int globalRightThreshold;

	/*	ImageShapes *shapes;
	QImage			*visImage;
	ImageWidget		*visImageWidget;*/
	/*int				viewType;
	double			mdx,mdy;
	int				mnx, mny;
	double			*outParams;
	double			*inParams;
	double nrInParams, nrOutParams;
	unsigned short mainDataMaxValue;*/

public:
	//	void	SetViewType(int t){ viewType = t; };
	//	void	SetMainDataMaxValue(unsigned short val){ mainDataMaxValue = val; };
	//	void	SetMainDataPixelSizes(double _dx,double _dy){mdx=_dx;mdy=_dy;};
	//	void	SetMainDataSizes(double _nx, double _ny){ mnx = _nx; mny = _ny; sliceSize = mnx*mny; };
	//	void	SetMaxValue(unsigned short m){maxValue=m;};
	////	unsigned short		GetMaxValue(){ return maxValue;}; 

	LayerFilter();
	virtual ~LayerFilter();

	//	void	SetShapes(ImageShapes *s){		shapes=s;	};
	//void	SetVisImageWidget(ImageWidget *s);

	/*	void	SetOutputParams(double *params, int nr){ outParams = params; nrOutParams = nr; };
	void	SetInputParams(double *params, int nr){ inParams = params; nrInParams = nr; };
	*/

	/*void	SetPolyline(QPolygon l){polyline=l;};;
	void	SetGlobalThresholds(int t1,int t2){globalLeftThreshold=t1;globalRightThreshold=t2;};
	*/

	//void	SetMainMaskData(unsigned char *mask, int x,int y);
	//void	GetShowTextStartStop(int index,int tab[2]);
	//QString	GetShowText(int pos);

	//void	AddParam(QString name, double defVal, bool slider, double min, double max, double step, int &paramID,QStringList *names=NULL);
	//
	//bool	GetSliderParams (int nr,double sParams[3]);

	////void	SetMaskParam(int nr,double val){ params[nr]=val;};
	//double	GetDefaultParam(int nr){return defaultParams[nr];};
	////double *Getparams(){return params;};
	//
	//QString	GetParamName(int nr){return paramsNames[nr];};
	//QString	GetFilterName(){return filterName;};

	//int		GetNumberOfParams(){return numberOfParams;};


	//apply filter - return 0 - failed 1- succeed, 2-succed mask utilized
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL) = 0;

	//data slicenr - dane zrodlowe do filtracji
	//usData0 - bufor docelowy
	//usData1 - bufor zapasowy
	//params thhreshods - parametry filtrow
	//datatab - chce zeby to byl bufor poprzedniej warstwy - jesli potrzebny
	//src, dst - tymczasowo bufory uchar - docelowo ma byc to typ danych
	//prevImg - chce wywalic
	//mask - maska do filtra



	//template <class TYPE>
	//void	temporalAveraging(RawDataSet *data, int sliceNr, int radius, double scale, TYPE *dest) ;
	//void	temporalAveraging(RawDataSet *data, int sliceNr, int radius, double scale, unsigned char *dest) ;
	//void	temporalAveraging(RawDataSet *data, int sliceNr, int radius, double scale, double *dest) ;
	//void	temporalAveraging(RawDataSet *data, int sliceNr, int radius, double scale, unsigned short *dest) ;
signals:
	void	SendMessage(const QString &Message);

};


//---------------------------------------------

class LayerEmptyFilter : public LayerFilter
{

public:

	LayerEmptyFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
class LayerThresholdFilter : public LayerFilter
{
	int threshLowerID;
	int threshUpperID;
	int invThreshID;

public:

	LayerThresholdFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//--------------------------------
class LayerAbsUcharFilter : public LayerFilter
{

	int centerValID;
	int rescaleID;

public:

	LayerAbsUcharFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//------------------------
class LayerAbsUShortFilter : public LayerFilter
{

	int centerValID;
	int rescaleID;

public:

	LayerAbsUShortFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//------------------------
class LayerDistanceTransformFilter : public LayerFilter
{
	//#define INF 1E9

	int thresholdLeftID;
	int thresholdRightID;
	int inverseThreshID;
	int useMaskSettingsID;
	int fillGapsFromDistanceID;
	int testID;
	//int scaleDistanceID;
	int scaleFactorID;

	int nx, ny;
	int n2x, n2y;
	int ss2;
	int ss;
	int infinity;
	int infp1;
	int maxSq;

	int *sqtab_X, *sqtab_Y, *sqtabMX, *sqtabMY, *sqtabPX, *sqtabPY;
	float *sqrtTabNorm;

	short *tempDistanceVectorArrayX, *tempDistanceVectorArrayY;

	void CalculateDistanceArrays2D();
	void CalculateForwardUp2D();
	void CalculateBackwardDown2D();

public:

	LayerDistanceTransformFilter();
	~LayerDistanceTransformFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//-------------------------------

//------------------------------------------------
class LayerStatisticalFilter1 : public LayerFilter
{
	int radiusID, kernelTypeID, iterID;

private:

	double K1(double m2, double s2, double mn2, double sn2) { return (1 - m2*s2) / (s2*(1 + sn2)); }
	double K2(double m2, double s2, double mn2, double sn2) { return s2 / (m2*sn2 + s2); }
	double K3(double m2, double s2, double mn2, double sn2) { return (s2 - sn2) / s2; }

	double (LayerStatisticalFilter1::*KFuncs[5])(double m2, double s2, double sn2, double mn2);

public:



	LayerStatisticalFilter1();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//--------------------------------
class LayerStatisticalFilter1Faster : public LayerFilter
{
	int radiusID, optimID;

public:

	LayerStatisticalFilter1Faster();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//--------------------------------
class LayerStatisticalFilter1Faster2 : public LayerFilter
{
	int radiusID, iterID, alphaID, maskPrevThreshID, useWeightMapID, amountID, invertWeightsID;
public:
	LayerStatisticalFilter1Faster2();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//--------------------------------------
class LayerStatisticalFilterDualCross : public LayerFilter
{
	int radiusID, modeID;
public:
	LayerStatisticalFilterDualCross();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//----------------------------------------------------------
class LayerStatisticalDirectionalFilter : public LayerFilter
{
	int radius1ID, radius2ID, finalValueID, rotPartsID, minFuncID, typeID, maskPrevThreshID;
	int finalValModeID;
	int stdDevThreshID;
	int valThreshID;
	int superTestID;
	int testWeightID;
	int testWeight1ID;
	int startAngleID;

private:

	double K1(double m2, double s2, double mn2, double sn2) { return (1 - m2*s2) / (s2*(1 + sn2)); }
	double K2(double m2, double s2, double mn2, double sn2) { return s2 / (m2*sn2 + s2); }
	double K3(double m2, double s2, double mn2, double sn2) { return (s2 - sn2) / s2; }
	double K4(double m2, double s2, double mn2, double sn2) { return (s2 / m2); }
	double K5(double m2, double s2, double mn2, double sn2) { return (s2); }

	double K1m(double m2, double s2, double mn2, double sn2) { return -(1 - m2*s2) / (s2*(1 + sn2)); }
	double K2m(double m2, double s2, double mn2, double sn2) { return -s2 / (m2*sn2 + s2); }
	double K3m(double m2, double s2, double mn2, double sn2) { return -(s2 - sn2) / s2; }
	double K4m(double m2, double s2, double mn2, double sn2) { return -(s2 / m2); }
	double K5m(double m2, double s2, double mn2, double sn2) { return -(s2); }

	double (LayerStatisticalDirectionalFilter::*KFuncs[20])(double m2, double s2, double sn2, double mn2);


public:
	LayerStatisticalDirectionalFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//--------------------------------------
class LayerStatisticalHomogeneousMaskFilter : public LayerFilter
{
private:

	int iterID;

public:

	LayerStatisticalHomogeneousMaskFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//--------------------------

class LayerMedianFast2DFilter : public LayerFilter
{
	int radiusID;

public:

	LayerMedianFast2DFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
class LayerHistogramLineViewFilter :public LayerFilter
{

	int orientationID;
	int sizeID;
	int r1ID, g1ID, b1ID;
	int followMouseID;
	int centerHistID;
	int showTextID;
	int logScaleID;
public:
	LayerHistogramLineViewFilter();
		virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);

};
//-------------------------
class LayerMedianFilter : public LayerFilter
{
	int radiusID, kernelTypeID, modeID;

public:

	LayerMedianFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
	int radius, type, mode;
};
//------------------------
class LayerConvolutionPredefinedFilter : public LayerFilter
{
	int filterNrID;
	QVector <int> filterStart;
	FilterDefs defs;

public:

	LayerConvolutionPredefinedFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
	//int radius, type, mode;
};
//---------------------------------------------------
class LayerMeasureFilter1 : public LayerFilter
{
	int radiusID;
	int sigmaID;
	int cleanShapesID;


	double *expTab;
	unsigned short expMax;
	double sigma;
	double intensityFactor;
	int intensityFactorID;
	unsigned short *samplingTab1;
	unsigned short *samplingTabIndex;
	int samplesCount1;

public:

	LayerMeasureFilter1();
	~LayerMeasureFilter1();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//-----------------------
class LayerBilateralFilter : public LayerFilter
{
	int iterID;
	int sigma_spatialID;
	int sigma_rangeID;
	int textureID;

public:

	LayerBilateralFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//-----------------------
class LayerGaussianFilter : public LayerFilter
{
	int radiusID, stddevID, useWeightMapID, amountID, invertWeightsID;
public:

	double radius;
	double stddev;

	LayerGaussianFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//-----------------------
class LayerEmptyResampleFilter : public LayerFilter
{
	int nxID, nyID, nzID, modeID;
public:
	int nx, ny, nz, mode;


	LayerEmptyResampleFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
///-------------------------
class LayerUnsharpMaskFilter : public LayerFilter
{
	int radiusID, stddevID, amountID, thresholdID;
public:

	LayerUnsharpMaskFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//-------------------------

class LayerAnisotropicDiffusionFilter : public LayerFilter
{
	int cfunctionID, iterID, k0ID, aID, dtID;
	int scalerID;
private:

	double C1(double s, double K) { return 1.0 / (1.0 + (s*s) / (K*K)); }
	double C2(double s, double K) { return exp(-(s*s) / (K*K)); }
	//Turkey's biweight function
	double C3(double s, double K) { if (s>K)return 0; double tmp = 1 - (s*s) / (K*K); return (0.5*(tmp*tmp)); }
	// Huber'smin-max norm
	double C4(double s, double K) { return (fabs(s)>K) ? 1.0 / s : 1.0 / K; }
	// Weickert
	double C5(double s, double K) { if (s>0) { double tmp = (K / s); return 1.0 - exp(-3.315*tmp*tmp*tmp*tmp); }return 1.0; }
	double (LayerAnisotropicDiffusionFilter::*ConductivityFuncs[5])(double, double);


public:

	LayerAnisotropicDiffusionFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//--------------------------

class LayerHighPassFilter : public LayerFilter
{
	int radiusID, stddevID, centerID, fabsID;

public:

	LayerHighPassFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};


class LayerSapheniaFilter : public LayerFilter
{

private:
#if defined (USG)	
	SCPPHandle scppHandle; // A handle to a SCPP instance
	SCPPError scppStatus; // An error code variable
	int nParameterBlocks; // Number of parameter blocks in parameter file
	const char **parameterBlockNames; // Parameter block names in parameter file
	const int *nSettings; // Number of settings in each parameter block
						  //char *parameterFile;

	int hgh, wid; // Image height and width
				  //char *type;//; // Image pixel type (just an example)
#define ERR_MSG_LEN 2048
	char errorMsg[ERR_MSG_LEN]; // Error Message from SCPPCreateEx()
	int blockNbr;
	int setting;

	TSCPPProcessingInfo processingInfo;
#endif

	bool success;

public:




	LayerSapheniaFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};


//-------------------------------------------

class LayerMixerLiteFilter : public LayerFilter
{
	bool init;
public:
	LayerMixerLiteFilter();
	~LayerMixerLiteFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};

//------------------------------------

class LayerMutualIntensityAnalysisFilter : public LayerFilter
{
	bool test;

	int functionID;

	int translateXID;
	int translateYID;
	int rotateID;

	int viewID;

	int mutualHistSizeID;
	int scatterMultiplyID;
	int scatterUseThresholdsID;
	int useBiggerMaxID;
	int invertThreshodsID;
	int leftThresholdID, rightThresholdID;

	int useHistLogScaleID;

	int showRegionInfoID;
	int showInfoPlaceID;

	int showMutualStatsID;

	int showRealRegionBoundsID;

	int sliceNumberID;
	int showReg1ID;
	int showReg2ID;

	int scatterSizeID;

	int hist1[256];
	int hist2[256];
	int scatter[65536];

	double stats1[16];
	double stats2[16];
	double statsMutual[16];

	bool stats1Calculated;
	bool stats2Calculated;




public:

	LayerMutualIntensityAnalysisFilter();
	//~LayerMutualIntensityAnalysisFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);
};
//------------------------------
class LayerGradientTestFilter : public LayerFilter
{

	int centerValID;
	int rescaleID;
	int invertID;
	int directionID;


public:

	LayerGradientTestFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//------------------------


class LayerMagnifyFilter : public LayerFilter
{
	int scaleID, interpolID;
	int useCenterID;
	int posXID, posYID;
	double t[15];
	double matrix[16];
	//Transform *trans;
	double rCenter[3];


public:

	LayerMagnifyFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;

	void TransformLine(int j, unsigned short *tab, unsigned short *dest, Transform *trans, InterpolationFunction *intrp, int *n);
};
//-------------------------




class LayerGrowingRegionFilter : public LayerFilter
{
	int minEndPointFrontSizeID;
	int connectCloseEndPointsID;
	int histSimilarityModeID;
	int histSamplingSizeID;
	int histSimilarityThreshold2DID;
	int nrOfHistBinsID;
	int histSimilarityThresholdID;
	/*int minStdDevID;
	int maxStdDevID;*/
	int deltaStdDevID;

	int fillGapsID;
	int connectNewSkeletonlWithJointsID;
	int generateEndPointsBallsID;
	int generateTracingSkeletonsID;

	int dontRemoveDistMapID;
	int advGlyphID;
	int breakGlyphID;
	int multiSeedID;
	int GrowFromPointID;
	int useOnlyClickedPointForGrowingID;
	int medianID;
	int medianFactorID;
	int medianKernelID;
	int medianKernel3DID;
	int medianThresholdID;
	int probOrThreshID;

	int r1ID, g1ID, b1ID;
	int r2ID, g2ID, b2ID;
	int segOpacityID;

	int thresholdAdd1ID;
	int thresholdAdd2ID;
	int threshold1ID, threshold2ID;
	int dilateSizeID;
	int closingSizeID;
	int probabilityFilter1ID;
	int useBigMaskID;
	int bigMaskModeID;

	int showDebugID;
	int ellipseRayDestID;
	int probabilityDensityPercentageCoarseID;
	int probabilityDensityPercentageFineID;
	int sigmaID;
	int intensityFactorID;
	int sphereRadiusXID;
	int segmentModeID;
	int radiusXID, radiusYID, radiusZID, angleID;
	int samplingTypeID;
	int calculateStatsID;
	int showInfoPlaceID;
	int drawHistogramID;
	int destID;
	int maxDistID;
	int multiplyID;
	int useAsMaskID;
	int mutliFrontID;
	int drawModeID;
	int minLeafLengthID;


	int morphologyTypeID;
	int morphologyKernelTypeID;
	int morph2DID;
	int drawFontsID;
	int infoTypeID;
	//int kernelSizeID;
	int morphKernelSizeID;

	int showSourceLinesID;


	int euclideanDistanceID;

	double *intensityProbabilityTab1;

	

	int *frontTabIn, *frontTabOut, *frontTabOut2,*frontTabOld;
	unsigned short *mask0;
	unsigned char *tempMaskTab;
	int mSize;
	int frontSizeMax;

	unsigned short *samplingTab1;
	unsigned short *samplingTabIndex;
	int samplesCount;
	int updateProbabilityTabsID;
	int fontSizeID;

	int showMaskID;

	int autoThresholdID;
	int sampling3DModeID;
	//int closingSizeID;
	int showFrontID;
	int mcValueID;
	int dontCalculate2DID;
	int minVolumeToGenPointID;


	int blockSmallFrontsID;
	int nrOfRepetitionsBeforeBlockID;
	int minSizeToBlockID;
	int maxSizeToBlockID;
	int maxVolumeID;
	int findEndPointID;
	int endPointRadiusID;


	int angleTestDistID;
	int angleTestAngleID;
	int kernelSkelSearchSizeID;
	int cutSkelFromBeginingID;
	int cutSkelThreshID;
	int useGlobalThresholdsID;
	int useMasksForAneurysmInitID;

	ParentAndAvoidData parentAndAvoid[256];

public:
	int minEndPointFrontSize;
	bool connectCloseEndPoints;
	int morphKernelSize;
	int histSamplingSize;
	bool morph2D;
	int nrOfHistBins;
	double  histSimilarityThreshold2D;
	double  histSimilarityThreshold;
	int histSimilarityMode;
	//double minStdDev;
	//double maxStdDev;
	double deltaStdDev;
	bool fillGaps;

	bool useGlobalThresholds;
	int segmentMode;
	double euclideanDistance;
	bool connectNewSkeletonlWithJoints;
	bool generateEndPointsBalls;
	bool generateTracingSkeletons;

	bool dontRemoveDistMap;
	bool advGlyph;
	
	int  drawMode;
	bool breakGlyph;
	double endPointRadius;
	double maxVolume;
	bool useBigMask;
	int bigMaskMode;
	double exportParams[10];
	/*bool median 0
	int medianKernel 1
	bool medianFactor 2
	bool onlyShortening 3
	int medianThreshold 4
	int dilate dist!!! 5
	int dilate type!!! 6*/
	int sampling3DMode;
	bool equalRadius;
	double radius[3];
	unsigned short *samplingTab;
	double *expTab;
	unsigned short expMax;
	double sigma;
	double intensityFactor;
	double perc;
	int leftTh;
	int rightTh;
	bool revth;
	bool mode;
	double maxDist;
	bool multiFront;
	bool showDebug;
	bool growFromPoint;
	int useOnlyClickedPointForGrowing;
	int useMasksForAneurysmInit;

	bool blockSmallFronts;
	int nrOfRepetitionsBeforeBlock;
	int minSizeToBlock;
	int dilateSize;
	int closingSize;
	int maxSizeToBlock;
	int multiSeed;
	bool findEndPoint;

	double minLeafLength;


	unsigned short *getSamplingTab() { return samplingTab; };
	int getCount() { return samplesCount; };
	LayerGrowingRegionFilter();
	~LayerGrowingRegionFilter();
	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;

};
//-------------------------------------
class IntensityFunctionFilter : public LayerFilter
{
	int functionID;
	int param1ID;
	int param2ID;
	int gammaID;
	int scaleID;
	int drawHistogramID;
	int viewID;


public:

	~IntensityFunctionFilter();
	IntensityFunctionFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};

class IntensitySimpleFilter : public LayerFilter
{

	int brightnessID;
	int contrastID;
	int shadowsID;
	int highlightsID;
	int midtonesID;
	int drawHistogramID;
	int viewID;
	int centerValueID;

	int useGlobalThresholdsID;

public:

	~IntensitySimpleFilter();
	IntensitySimpleFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//---------------------------
class BrightnessContrastFilter : public LayerFilter
{

	int brightnessID;
	int contrastID;
	int useGlobalThresholdsID;
	/*int shadowsID;
	int highlightsID;
	int midtonesID;
	int drawHistogramID;
	int viewID;
	int centerValueID;*/



public:


	BrightnessContrastFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//------------------------------
class CannyEdgeDetectorFilter : public LayerFilter
{
	int radiusID, stddevID;
	int epsilonDerivativeID;
	int epsilonNormID;
	int scaleID;



	/*int brightnessID;
	int contrastID;
	int shadowsID;
	int highlightsID;
	int midtonesID;
	int drawHistogramID;
	int viewID;*/



public:

	double radius;
	double stddev;

	~CannyEdgeDetectorFilter();
	CannyEdgeDetectorFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};

//---------------------------
class HistogramEqualizationFilter : public LayerFilter
{
	int threshID;
	int modeID;
	int valModeID;



	/*int brightnessID;
	int contrastID;
	int shadowsID;
	int highlightsID;
	int midtonesID;
	int drawHistogramID;
	int viewID;*/



public:

	double radius;
	double stddev;


	HistogramEqualizationFilter();

	virtual int ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool invese = false, unsigned char *src = NULL, unsigned char *dst = NULL, unsigned short *prevImg = NULL, unsigned char *mask = NULL, int **intBuffers = NULL);;
};
//--------------------------------------------------------------








class LayerMorphologyFilter : public LayerFilter
{
	int probOrThreshID;
	int histSimilarityModeID;
	int histSamplingSizeID;
	int histSimilarityThreshold2DID;
	int nrOfHistBinsID;
	int histSimilarityThresholdID;

	int deltaStdDevID;

	int segOpacityID;

	int thresholdAdd1ID;
	int thresholdAdd2ID;
	int threshold1ID, threshold2ID;
	int dilateSizeID;
	int closingSizeID;
	int probabilityFilter1ID;
	int autoThresholdID;
	int useGlobalThresholdsID;
	int showDebugID;

	int probabilityDensityPercentageCoarseID;
	int probabilityDensityPercentageFineID;
	int sigmaID;
	int intensityFactorID;


	int samplingTypeID;
	int calculateStatsID;

	int morphologyTypeID;
	int morphologyKernelTypeID;
	int morph2DID;
	int drawFontsID;
	int infoTypeID;
	//int kernelSizeID;
	int morphKernelSizeID;


	double* intensityProbabilityTab1;


	unsigned short* samplingTab1;
	unsigned short* samplingTabIndex;
	int samplesCount;
	int updateProbabilityTabsID;



public:
	
	int probOrThresh;
	int autoThreshold;
	int useGlobalThresholds;
	int morphKernelSize;
	int histSamplingSize;
	bool morph2D;
	int nrOfHistBins;
	double  histSimilarityThreshold2D;
	double  histSimilarityThreshold;
	int histSimilarityMode;

	double deltaStdDev;




	
	
	int sampling3DMode;
	
	unsigned short* samplingTab;
	double* expTab;
	unsigned short expMax;
	double sigma;
	double intensityFactor;
	double perc;
	int leftTh;
	int rightTh;
	bool revth;
	bool mode;
	
	bool showDebug;
	bool growFromPoint;
	


	unsigned short* getSamplingTab() { return samplingTab; };
	int getCount() { return samplesCount; };
	LayerMorphologyFilter();
	~LayerMorphologyFilter();
	virtual int ApplyFiltering(RawDataMixerLite* mixer, unsigned short*& usData0, unsigned short*& usData1, double* params, ImageShapes* shapes, ImageWidget* iWidget, unsigned short* tab, bool invese = false, unsigned char* src = NULL, unsigned char* dst = NULL, unsigned short* prevImg = NULL, unsigned char* mask = NULL, int** intBuffers = NULL);;

};











void  initLayerFilters(LinkedList <LayerFilter*> *layerFilterList);



#endif //LAYER_FUILTERS_H
