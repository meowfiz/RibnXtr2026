#ifndef	LAYER_GENERIC_FILTER_H
#define	LAYER_GENERIC_FILTER_H

#include <qwidget.h>

//#include <math.h>
//#include <stdlib.h>

//#include "RawDataSet.h"
#include "OtherCommonFunctions.h"
//#include "DataSet.h"
//#include "GUIPanelWidget.h"
//#include "ImageShapes.h"
//#include "ImageWidget.h"
//#include "RawDataMixerLite.h"

//#include <qlist.h>

// co

#include <omp.h>
//class RawDataMixerLite;
enum WidgetModeIDS { LINEEDIT_MODE, SLIDER_MODE, COMBOBOX_MODE };
class LayerGenericFilter
{
protected:
	int				nrOfThreads;
	int				inverseID;
	int				prevLayerID;

	QString			filterName;
	QString			paramsNames[PARAMS_NR];
	QString			addText[PARAMS_NR];
	QStringList		showTextList[PARAMS_NR];
	//int				showTextStartStop[PARAMS_NR*2];
	double			defaultParams[PARAMS_NR];
	int 			viewMode[PARAMS_NR]; //czy dodac slider przy gui - zmienie na mode - czy slider, czy combobox
	double			minVals[PARAMS_NR]; //min max jesli jest slider
	double			maxVals[PARAMS_NR];//
	double			steps[PARAMS_NR];//step dla slidera - glownie po to, zeby wiedziec czy po calkowitych czy po ulamkach
	int				numberOfParams;


	int				viewType;
	double			mdx, mdy;
	int				mnx, mny;

	unsigned char *mainMask;
	unsigned char* mainMask2;
	int position[2];

	unsigned short mainDataMaxValue;
	QPolygon polyline;
	int globalLeftThreshold;
	int globalRightThreshold;
	//unsigned short	maxValue;
	double			*outParams;
	double			*inParams;
	double nrInParams, nrOutParams;
	int sliceSize;

	double *shaFunc;
	double *higFunc;
	double *midFunc;
	int funSize;
	int brightnessFilterMode;

public:
	 
	void	SetViewType(int t) { viewType = t; };
	void	SetMainMaskData(unsigned char *mask, int x, int y);
	void	SetMainMaskData2(unsigned char* mask);
	LayerGenericFilter();
	~LayerGenericFilter();

	void	SetOutputParams(double *params, int nr) { outParams = params; nrOutParams = nr; };
	void	SetInputParams(double *params, int nr) { inParams = params; nrInParams = nr; };

	void	SetPolyline(QPolygon l) { polyline = l; };;
	void	SetGlobalThresholds(int t1, int t2) { globalLeftThreshold = t1; globalRightThreshold = t2; };
	void	SetMainDataSizes(double _nx, double _ny) { mnx = _nx; mny = _ny; sliceSize = mnx*mny; };
	void	SetMainDataMaxValue(unsigned short val) { mainDataMaxValue = val; };
	void	SetMainDataPixelSizes(double _dx, double _dy) { mdx = _dx; mdy = _dy; };
	void	SetBrightnessTabs(int size, double *hig, double*mid, double *sha) { shaFunc = sha; midFunc = mid; higFunc = hig; funSize = size; }
	void	SetBrightnessFilterMode(int mode) { brightnessFilterMode = mode; };
	//	void	SetMainDataSizes(double _nx, double _ny){ mnx = _nx; mny = _ny; };
	//void	GetShowTextStartStop(int index,int tab[2]);
	QString	GetShowText(int nr, int pos);
	void	AddParam(QString name, double defVal, int mode, double min, double max, double step, int &paramID, QStringList *names = NULL);
	bool	GetSliderParams(int nr, double sParams[3]);
	double	GetDefaultParam(int nr) { return defaultParams[nr]; };
	QString	GetParamName(int nr) { return paramsNames[nr]; };
	QString	GetFilterName() { return filterName; };
	int		GetNumberOfParams() { return numberOfParams; };
//	void	SetMaxValue(unsigned short m) { maxValue = m; };

	int	GetViewMode(int param) { return viewMode[param]; };



	// data - dane raw data zrodlowe
	// slicenr - numer warstwy
	// masks - maska docelowa
	// maskParams - parametry do filtru
	// maskThresholds - progi - lewy i prawy
	// dataTab - poprzednia warstwa

};


//---------------------------------------------
#endif //LAYER_GENERIC_FILTER_H