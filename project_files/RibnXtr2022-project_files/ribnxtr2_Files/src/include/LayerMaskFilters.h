#ifndef	LAYER_MASK_FUILTERS_H
#define	LAYER_MASK_FUILTERS_H

#include <qwidget.h>

#include <math.h>
#include <stdlib.h>

#include "RawDataSet.h"
#include "OtherCommonFunctions.h"
#include "DataSet.h"
#include "GUIPanelWidget.h"
#include "ImageShapes.h"
#include "ImageWidget.h"
#include "RawDataMixerLite.h"
#include "LayerGenericFilter.h"

#include <qlist.h>
//#define PARAMS_NR 50
// co

#include <omp.h>
class RawDataMixerLite;

class LayerMaskFilter :public QObject, public LayerGenericFilter
{
	Q_OBJECT 
	//filtr maskowy ktory ma nic nie  zmieniac w wyswietlaniu ma miec wartosci 255 w mask0
protected:
	//int				nrOfThreads;
	//int				inverseID;

	//QString			filterName;
	//QString			paramsNames[PARAMS_NR];
	//QString			addText[PARAMS_NR];
	//QStringList		showTextList;
	//int				showTextStartStop[PARAMS_NR*2];
	//double			defaultParams[PARAMS_NR];
	//bool			paramsSliders[PARAMS_NR]; //czy dodac slider przy gui
	//double			minVals[PARAMS_NR]; //min max jesli jest slider
	//double			maxVals[PARAMS_NR];//
	//double			steps[PARAMS_NR];//step dla slidera - glownie po to, zeby wiedziec czy po calkowitych czy po ulamkach
	//int				numberOfParams;

	////ImageShapes		*shapes;
	////QImage			*visImage;

	////ImageWidget		*visImageWidget;
	//int				viewType;
	//double			mdx, mdy;
	//int				mnx, mny;
	////unsigned short	maxValue;

	//unsigned char *mainMask;
	//int position[2];

	//unsigned short mainDataMaxValue;


public:
	//	void	SetViewType(int t){ viewType = t; };
	//	void	SetMainMaskData(unsigned char *mask, int x,int y);
	////	void	SetMaxValue(unsigned short m){maxValue=m;};
	//	//unsigned short		GetMaxValue(){ return mainDataMaxValue; };

	LayerMaskFilter();
	~LayerMaskFilter();

	//void	SetShapes(ImageShapes *s){		shapes=s;	};
	//void	SetVisImageWidget(ImageWidget *s);//{	if(s!=NULL) visImage=s;	};
	/*void	SetMainDataMaxValue(unsigned short val){ mainDataMaxValue = val; };
	void	SetMainDataPixelSizes(double _dx, double _dy){ mdx = _dx; mdy = _dy; };
	void	SetMainDataSizes(double _nx, double _ny){ mnx = _nx; mny = _ny; };
	void	GetShowTextStartStop(int index,int tab[2]);
	QString	GetShowText(int pos);
	void	AddParam(QString name, double defVal, bool slider, double min, double max, double step, int &paramID,QStringList *names=NULL);
	bool	GetSliderParams (int nr,double sParams[3]);
	double	GetDefaultParam(int nr){return defaultParams[nr];};
	QString	GetParamName(int nr){return paramsNames[nr];};
	QString	GetFilterName(){return filterName;};
	int		GetNumberOfParams(){return numberOfParams;};*/


	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL) = 0;

	// data - dane raw data zrodlowe
	// slicenr - numer warstwy
	// masks - maska docelowa
	// maskParams - parametry do filtru
	// maskThresholds - progi - lewy i prawy
	// dataTab - poprzednia warstwa
signals:
	void	SendMessage(const QString &Message);
};


//---------------------------------------------

class LayerMaskThresholdFilter : public LayerMaskFilter
{
	//int leftThresholdID,rightThresholdID;
	int thresholdLeftID, thresholdRightID;
public:


	LayerMaskThresholdFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};
class LayerMaskEmptyFilter : public LayerMaskFilter
{
	//int leftThresholdID,rightThresholdID;
	//int thresholdLeftID, thresholdRightID;
public:


	LayerMaskEmptyFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};
//----------------------------------------

class LayerMaskIslandFilter : public LayerMaskFilter
{
	int fontSizeID;
	int showInfoPlaceID;
	int globalXCorrectionID, globalYCorrectionID;
	int minAreaID, maxAreaID, minWidthID, maxWidthID, minHeightID, maxHeightID;
	int leftThresholdID, rightThresholdID;
	int shapeID, toleranceID;
	int rotateID;

	int showOrigBoundsID;
	int showFittedBoundsID;
	int showFittedAxesID;
	int showFittedEllipseID;
	int showContourPointsID;

	int morphologyTypeID;
	int morphologyKernelTypeID;
	int drawFontsID;
	int infoTypeID;
	int cleanShapesID;
	int useStartShapesID;

public:

	LayerMaskIslandFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};
//----------------------------------------
class LayerMaskIntensityAnalysisFilter : public LayerMaskFilter
{
	bool test;
	int leftThresholdID, rightThresholdID;
	/*int shapeID, toleranceID;
	int rotateID;

	int showOrigBoundsID;
	int showFittedBoundsID;
	int showFittedAxesID;
	int showFittedEllipseID;
	int showContourPointsID;

	int morphologyTypeID;
	int morphologyKernelTypeID;
	int drawFontsID;
	int infoTypeID;
	int cleanShapesID;
	int useStartShapesID;*/
	int nrOfRegion1ID;
	int nrOfRegion2ID;

	int updateRegion1ID;
	int updateRegion2ID;

	int cleanShapesID;

	int useThresholdsID;
	int useHistLogScaleID;

	int showRegionInfoID;
	int showInfoPlaceID;

	int hist1[256];
	int hist2[256];

	double stats1[16];
	double stats2[16];
	bool stats1Calculated;
	bool stats2Calculated;
	int showRealRegionBoundsID;

	//bool UpdateStats(int *hist,double *stats,QPolygonF poly,unsigned char *mask,int nx,int ny,unsigned short *tab,int **tmpTab,int &count,bool showBounds=false);
	//void DrawHistogram(int *hist, int startY,int backClean, int height, bool logScale,QColor backC,QColor frontC);

public:

	LayerMaskIntensityAnalysisFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};
//--------------------------

//----------------------------
class LayerMaskThicknessAnalysisFilter : public LayerMaskFilter
{
	int testID;
	int calculateIntensityStatsID;
	int leftIntensityStatsThreshID;
	int rightIntensityStatsThreshID;

	int lineUpperColorID;
	int lineCenterColorID;
	int lineSmoothed1ColorID;
	int lineSmoothed2ColorID;
	int lineLowerColorID;
	int lineSegmentsColorID;
	int textColorID;

	int fillColorID;
	int fillOpacityID;


	int improveCollisionID;
	int medianID;
	int medianFactorID;
	int medianKernelID;

	int samplingThicknessID;
	int leftCutID;
	int rightCutID;

	int removeCollisionsID;
	int removeAllCollisionsID;
	int useProbabilityDensityMapID;
	//int probabilityDensityPercentageID;

	int probabilityDensityPercentageCoarseID;
	int probabilityDensityPercentageFineID;
	int sigmaID;
	int intensityFactorID;

	int autoThresholdID;
	int maxDistanceID;
	int maxDistanceRealDistModeID;
	int orientationID, positionID, blendWidthID;
	int leftThresholdID, rightThresholdID;

	int firstSmoothingIterID, secondSmoothingID;
	int smoothingDxID, smoothingDyID;
	int factorID;
	int updateCenterID;
	int samplingID;
	int infoTypeID;

	int showOrigLineID, showSmoothed1ID, showSmoothed2ID, showLinesID;
	int updateCenterFactorID;

	int cleanShapesID;
	int fontSizeID;

	int useBigMaskID;
	int bigMaskModeID;

	int showFront1ID;
	int showFront2ID;
	int textXPosID;
	int textYPosID;
	int closingBinaryProbMapID;

	double *expTab;
	unsigned short expMax;
	double sigma;
	double intensityFactor;
	unsigned short *samplingTabIndex;
	int samplesCount1;
	unsigned short *samplingTab1;

public:

	LayerMaskThicknessAnalysisFilter();
	virtual ~LayerMaskThicknessAnalysisFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};


//----------------------------------------

class LayerMaskLinearBlendFilter : public LayerMaskFilter
{
	int orientationID, positionID, blendWidthID;

public:

	LayerMaskLinearBlendFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};


class LayerMaskPointShapeFilter : public LayerMaskFilter
{
	int sizeXID;
	int sizeYID;
	int typeID;
	int angleID;
	int showBoundsID;
	int bRID, bGID, bBID;

public:

	LayerMaskPointShapeFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};

class LayerMaskCheckedFilter : public LayerMaskFilter
{
	int sizeXID, sizeYID, blendWidthID;
public:

	LayerMaskCheckedFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};

//--------------------------------
class LayerMaskAdjacentRegionFilter : public LayerMaskFilter
{
	int minAreaID, maxAreaID, minWidthID, maxWidthID, minHeightID, maxHeightID;
	int leftThreshold1ID, rightThreshold1ID;
	int leftThreshold2ID, rightThreshold2ID;


	int r1ID, g1ID, b1ID;
	int r2ID, g2ID, b2ID;


	//int showOrigBoundsID;
	//int showFittedBoundsID;
	//int showFittedAxesID;
	//int showFittedEllipseID;
	int showContour1PointsID;
	int showContour2PointsID;

	int morphologyTypeID;
	int morphologyKernelTypeID;
	int drawFontsID;
	int infoTypeID;
	int cleanShapesID;
	//int useStartShapesID;
public:

	LayerMaskAdjacentRegionFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};

//------------------------------
class LayerMaskGaussianBlurFilter : public LayerMaskFilter
{
	int radiusID, stddevID, amountID;
	//int useStartShapesID;
public:

	LayerMaskGaussianBlurFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};

//---------------------------------------------
class LayerMaskProbabilityAdjacentRegionFilter : public LayerMaskFilter
{
	int minArea1ID, maxArea1ID, minWidth1ID, maxWidth1ID, minHeight1ID, maxHeight1ID;
	int minArea2ID, maxArea2ID, minWidth2ID, maxWidth2ID, minHeight2ID, maxHeight2ID;

	int onlyAdjacentID;

	double *expTab;
	unsigned short expMax;
	double sigma;
	double intensityFactor;


	int r1ID, g1ID, b1ID;
	int r2ID, g2ID, b2ID;

	int probabilityFilter1ID;
	int probabilityFilter2ID;
	int probabilityDensityPercentage1ID;
	int probabilityDensityPercentage2ID;
	int samplingTypeID;


	int currentShapeEditID;
	int useAsMaskID;

	int showContour1PointsID;
	int showContour2PointsID;

	int morphologyTypeID;
	int morphologyKernelTypeID;
	int drawFontsID;
	int infoTypeID;
	int cleanShapesID;
	int adjacentDistID;
	int showSourceLinesID;
	//double *intensityProbabilityTab1;
	//double *intensityProbabilityTab2;
	unsigned short *samplingTab1;
	unsigned short *samplingTab2;
	int samplesCount1, samplesCount2;
	int updateProbabilityTabsID;
	int fontSizeID;

public:

	LayerMaskProbabilityAdjacentRegionFilter();
	~LayerMaskProbabilityAdjacentRegionFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};
//-------------------------------------
class LayerMaskExplodeSegmentationRegionFilter : public LayerMaskFilter
{

	int dest3DID;
	int medianID;
	int medianFactorID;
	int medianKernelID;
	int medianKernel3DID;
	int medianThresholdID;
	int probOrThreshID;

	int r1ID, g1ID, b1ID;

	int segOpacityID;

	int threshold1ID, threshold2ID;
	int dilateDistID;
	int probabilityFilter1ID;
	int useBigMaskID;
	int bigMaskModeID;

	int onlyShorteningID;
	int ellipseRayDestID;
	int probabilityDensityPercentageCoarseID;
	int probabilityDensityPercentageFineID;
	int sigmaID;
	int intensityFactorID;
	int radiusXID, radiusYID, radiusZID, angleID;
	int samplingTypeID;
	int calculateStatsID;
	int showInfoPlaceID;

	int destID;
	int maxDistID;

	int useAsMaskID;
	int showContour1PointsID;


	int morphologyTypeID;
	int morphologyKernelTypeID;
	int drawFontsID;
	int infoTypeID;

	int showSourceLinesID;
	double *intensityProbabilityTab1;

	unsigned short *samplingTab1;
	unsigned short *samplingTabIndex;
	int samplesCount;
	int updateProbabilityTabsID;
	int fontSizeID;

	int showMaskID;

	int autoThresholdID;
	int sampling3DModeID;
	int closingSizeID;

public:
	int dest3D;
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

	unsigned short *getSamplingTab() { return samplingTab; };
	int getCount() { return samplesCount; };
	LayerMaskExplodeSegmentationRegionFilter();
	~LayerMaskExplodeSegmentationRegionFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};
//-------------------------------------
class LayerMaskGrowingRegionFilter : public LayerMaskFilter
{


	int medianID;
	int medianFactorID;
	int medianKernelID;
	int medianKernel3DID;
	
	int medianThresholdID;
	int probOrThreshID;

	int r1ID, g1ID, b1ID;
	int r2ID, g2ID, b2ID;
	int segOpacityID;

	int threshold1ID, threshold2ID;
	int dilateDistID;
	int probabilityFilter1ID;
	int useBigMaskID;
	int bigMaskModeID;

	int onlyShorteningID;
	int ellipseRayDestID;
	int probabilityDensityPercentageCoarseID;
	int probabilityDensityPercentageFineID;
	int sigmaID;
	int intensityFactorID;
	int radiusXID, radiusYID, radiusZID, angleID;
	int samplingTypeID;
	int calculateStatsID;
	int showInfoPlaceID;

	int destID;
	int maxDistID;

	int useAsMaskID;
	int showContour1PointsID;


	int morphologyTypeID;
	int morphologyKernelTypeID;
	int drawFontsID;
	int infoTypeID;

	int showSourceLinesID;
	double *intensityProbabilityTab1;


	int *frontTabIn, *frontTabOut;
	int frontSizeMax;

	unsigned short *samplingTab1;
	unsigned short *samplingTabIndex;
	int samplesCount;
	int updateProbabilityTabsID;
	int fontSizeID;

	int showMaskID;

	int autoThresholdID;
	int sampling3DModeID;
	int closingSizeID;
	int showFrontID;


public:

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
	


	unsigned short *getSamplingTab() { return samplingTab; };
	int getCount() { return samplesCount; };
	LayerMaskGrowingRegionFilter();
	~LayerMaskGrowingRegionFilter();
	virtual bool	ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *maskParams, int **intBuffers, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *dataTab = NULL);
};
//-------------------------------------


void initLayerMaskFilters(LinkedList <LayerMaskFilter*> *layerMaskFilterList);

#endif //LAYER_MASK_FUILTERS_H
