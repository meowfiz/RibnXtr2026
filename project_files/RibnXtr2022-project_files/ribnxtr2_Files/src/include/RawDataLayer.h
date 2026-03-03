#ifndef	RAWDATALAYER_H
#define	RAWDATALAYER_H

#include <qwidget.h>
#include <qtextstream.h>
#include <math.h>
#include <stdlib.h>

#include "RawDataSet.h"
#include "OtherCommonFunctions.h"
#include "DataSet.h"
//#include "GUIPanelWidget.h"
#include "LayerFilters.h"
#include "LayerMaskFilters.h"
#include "LinkedList.h"
#include "LayerGenericFilter.h"
// LinkedList <LayerFilter*> layerFilterList;
// co
#define NR_FILTER_PARAMS 100

class LayerMaskFilter;
class LayerFilter;


#define WRITE_PARAM(p1,p2)(stream << p1 << p2)
enum  mainIDS { OpacityID, LayerEnableID, LayerSourceMixedID, ModesID, FilterEnableID, FilterInverseID, MultiplierID, AdditionID, AutoNormalizeID, MaskFilterEnableID, BrightnessFilterModeID, RememberBufferID };
class RawDataLayer// :public LayerGenericFilter
{


private:
	QStringList mainFilterNames;
	double	defaultMainParams[NR_FILTER_PARAMS];
	int			maskFilterMode;
	int				filterMode;
	int dataSetNumber;
	int maskNumber;

	QString name;
	//DataSet *dataSet;

	//unsigned short *prevBuffer;
	//LinkedList<DataSet*>	*dataSetList
	//	int mainDataSize;
	int	tmp;
	//	int blendingMode;
	//	double blendingOpacity;
	//	double blendingMultiplier;
	//	double blendingAddition;

	//double blendingParamsTab[20];
	QTreeWidgetItem *treeItem;



	//	bool		isMaskEnabled;
	//	bool		isMaskInverted;
	//	int			thresholds[NR_FILTER_PARAMS]; //threshold 2 ==reverse

	int			mainParamsColors[NR_FILTER_PARAMS*3];
	int			filterParamsParamsColors[NR_FILTER_PARAMS * 3];
	int			maskFilterParamsColors[NR_FILTER_PARAMS * 3];

	double		maskFilterParams[NR_FILTER_PARAMS];
	bool		maskFilterParamForGui[NR_FILTER_PARAMS];
	double		mainParams[NR_FILTER_PARAMS];
	//QString		mainParamsNames[NR_FILTER_PARAMS];
	bool		mainParamForGui[NR_FILTER_PARAMS];
	//unsigned char *maskBuffer;

	unsigned char   *maskBuffer[3];

	//	bool			isFilterEnabled;
	double			filterParams[NR_FILTER_PARAMS];
	bool			filterParamForGui[NR_FILTER_PARAMS];
	RawDataSet		*filterBufferData[3];

	unsigned short origMaxValue;


	//bool		isAutoNormalizeEnabled;
	//bool		isFilterInverted;
	//bool		isLayerEnabled;

	bool		isChild;
	bool		isParrentEnabled;
	bool		isMixerMode;

	//	bool		isMixedLayer;
	//	int			nx,ny;
	int			dataCounter;



	LinkedList <LayerMaskFilter*> *layerMaskFilterList;
	LinkedList <LayerFilter*> *layerFilterList;



public:

	RawDataLayer(LinkedList <LayerMaskFilter*> *_layerMaskFilterList, LinkedList <LayerFilter*> *_layerFilterList);
	RawDataLayer(LinkedList <LayerMaskFilter*> *_layerMaskFilterList, LinkedList <LayerFilter*> *_layerFilterList, int _nx, int _ny, int _nz, int _dataSetNumber = -1);//default prev layer
	~RawDataLayer();

	void	CreateBuffers(int _nx, int _ny, int _nz);

	int getTmp() { return tmp; };
	void setTmp(int t) { tmp = t; };


	int		GetDataOrigMaxValue() { return origMaxValue; };
	void	SetDataOrigMaxValue(int nr) { origMaxValue = nr; };

	int		GetDataCounter() { return dataCounter; };
	void	SetDataCounter(int nr) { dataCounter = nr; };


	int		GetDataSetNumber() { return dataSetNumber; };
	void	SetDataSetNumber(int nr) { dataSetNumber = nr; };

	int		GetMaskNumber() { return maskNumber; };
	void	SetMaskNumber(int nr) { maskNumber = nr; };

	QTreeWidgetItem *GetTreeWidgetItem() { return treeItem; };;
	void	SetTreeWidgetItem(QTreeWidgetItem *t, int m) { treeItem = t; dataCounter = m; };

	//void	SetBlendingMode(int b){blendingMode=b;};
	//void	SetBlendingOpacity(double o){blendingOpacity=o;};

	//double	GetBlendingOpacity(){return blendingOpacity;};
	//int		GetBlendingMode(){return blendingMode;};
	//double *GetBlendingParamsTab(){return blendingParamsTab;};

	//void	SetBlendingMultiplier(double b){blendingMultiplier=b;};
	//double	GetBlendingMultiplier(){return blendingMultiplier;};


	//void	SetBlendingAddition(double b){blendingAddition=b;};
	//double	GetBlendingAddition(){return blendingAddition;};

	QString GetName() { return name; };
	void	SetName(QString n) { name = n; };



	/*	void	SetAutoNormalize(bool t){isAutoNormalizeEnabled=t;};;
	bool	GetAutoNormalize(){return isAutoNormalizeEnabled;};

	void	SetMaskEnabled(bool t);
	bool	GetMaskEnabled(){return isMaskEnabled;};


	void	SetMixedLayer(bool t){isMixedLayer=t;};;
	bool	GetMixedLayer(){return isMixedLayer;};*/

	void	SetMainParamsColors(int nr, int col[3])
	{		mainParamsColors[nr * 3] = col[0];		mainParamsColors[nr * 3+1] = col[1];		mainParamsColors[nr * 3+2] = col[2];	};
	void	GetMainParamsColors(int nr, int *col)
	{		col[0]=	mainParamsColors[nr * 3]  ;		col[1]=mainParamsColors[nr * 3 + 1] ;		col[2]=mainParamsColors[nr * 3 + 2] ;	};

	void	SetFilterParamsColors(int nr, int col[3])
	{		filterParamsParamsColors[nr * 3] = col[0];		filterParamsParamsColors[nr * 3 + 1] = col[1];		filterParamsParamsColors[nr * 3 + 2] = col[2];	};
	void	GetFilterParamsColors(int nr, int* col)
	{		col[0] = filterParamsParamsColors[nr * 3];		col[1] = filterParamsParamsColors[nr * 3 + 1];		col[2] = filterParamsParamsColors[nr * 3 + 2];	};

	void	SetMaskFilterParamsColors(int nr, int col[3])
	{		maskFilterParamsColors[nr * 3] = col[0];		maskFilterParamsColors[nr * 3 + 1] = col[1];		maskFilterParamsColors[nr * 3 + 2] = col[2];	};
	void	GetMaskFilterParamsColors(int nr, int* col)
	{		col[0] = maskFilterParamsColors[nr * 3];		col[1] = maskFilterParamsColors[nr * 3 + 1];		col[2] = maskFilterParamsColors[nr * 3 + 2];	};



	void	SetMainParam(int nr, double val) { mainParams[nr] = val; };
	double	GetMainParam(int nr) { return mainParams[nr]; };
	double	*GetMainParams() { return mainParams; };
	void	SetMainParamForGui(int nr, bool val) { mainParamForGui[nr] = val; };
	bool	GetMainParamForGui(int nr) { return mainParamForGui[nr]; };


	int		GetNumberOfMainParams() { return mainFilterNames.count(); };


	void	SetFilterParamForGui(int nr, bool val) { filterParamForGui[nr] = val; };
	bool	GetFilterParamForGui(int nr) { return filterParamForGui[nr]; };
	bool	*GetFilterParamsForGui() { return filterParamForGui; };

	void	SetMaskFilterParam(int nr, double val) { maskFilterParams[nr] = val; };
	double	GetMaskFilterParam(int nr) { return maskFilterParams[nr]; };
	double	*GetMaskFilterParams() { return maskFilterParams; };
	void	SetMaskFilterParamForGui(int nr, bool val) { maskFilterParamForGui[nr] = val; };
	bool	GetMaskFilterParamForGui(int nr) { return maskFilterParamForGui[nr]; };
	bool	*GetMaskFilterParamsForGui() { return maskFilterParamForGui; };


	void	SetMaskFilterMode(int i) { maskFilterMode = i; };
	int		GetMaskFilterMode() { return maskFilterMode; };

	//	void	SetMaskThreshold(int nr,int val){thresholds[nr]=val;};
	//	int	GetMaskThreshold(int nr){return thresholds[nr];};

	//int		*GetMaskThresholds(){return thresholds;};

	/*
	void	SetFilterEnabled(bool t);
	bool	GetFilterEnabled(){return isFilterEnabled;};

	void	SetFilterInverted(bool t){isFilterInverted=t;};
	bool	GetFilterInverted(){return isFilterInverted;};

	void	SetMaskInverted(bool t){isMaskInverted=t;};
	bool	GetMaskInverted(){return isMaskInverted;};



	void	SetLayerEnabled(bool t){isLayerEnabled=t;};
	bool	GetLayerEnabled(){return isLayerEnabled;};*/

	bool	GetLayerEnabled() { return mainParams[LayerEnableID]; };
	double	GetBlendingOpacity() { return mainParams[OpacityID]; };
	bool	GetMaskEnabled() { return mainParams[MaskFilterEnableID]; };
	int		GetBlendingMode() { return mainParams[ModesID]; };
	double	GetBlendingMultiplier() { return mainParams[MultiplierID]; };
	double	GetBlendingAddition() { return mainParams[AdditionID]; };
	bool	GetAutoNormalize() { return mainParams[AutoNormalizeID]; };
	bool	GetFilterEnabled() { return mainParams[FilterEnableID]; };
	bool	GetFilterInverted() { return mainParams[FilterInverseID]; };
	bool	GetMixedLayer() { return mainParams[LayerSourceMixedID]; };
	int		GetBrightnessFilterMode() { return mainParams[BrightnessFilterModeID]; };
	int		GetRememberBuffer() { return mainParams[RememberBufferID]; };


	void	SetLayerEnabled(bool v) { mainParams[LayerEnableID] = v; };
	void	SetBlendingOpacity(double v) { mainParams[OpacityID] = v; };
	void	SetMaskEnabled(bool v) { mainParams[MaskFilterEnableID] = v; };
	void	SetBlendingMode(int v) { 
		mainParams[ModesID];
	};
	void	SetBlendingMultiplier(double v) { mainParams[MultiplierID] = v; };
	void	SetBlendingAddition(double v) { mainParams[AdditionID] = v; };
	void	SetAutoNormalize(bool v) { mainParams[AutoNormalizeID] = v; };
	void	SetFilterEnabled(bool v) { mainParams[FilterEnableID] = v; };
	void	SetFilterInverted(bool v) { mainParams[FilterInverseID] = v; };
	void	SetMixedLayer(bool v) { mainParams[LayerSourceMixedID] = v; };
	void	SetBrightnessFilterMode(int v) { mainParams[BrightnessFilterModeID] = v; };
	void	SetRememberBuffer(int v) { mainParams[RememberBufferID] = v; };


	//void	SetBlendingAddition(double b){blendingAddition=b;};
	//double	GetBlendingAddition(){return blendingAddition;};

	double GetParam(int type, int nr)
	{
		double val = 0;
		switch (type)
		{
		case 0:
			val = GetMainParam(nr);
			break;
		case 1:
			val = GetFilterParam(nr);
			break;
		case 2:
			val = GetMaskFilterParam(nr);
			break;
		};
		return val;
	};
	//////--------------------
	double *GetParamsAddress(int type, int nr)
	{
		double *address = 0;
		switch (type)
		{
		case 0:
			address = &mainParams[nr];
			break;
		case 1:
			address = &filterParams[nr];
			break;
		case 2:
			address = &maskFilterParams[nr];
			break;
		};
		return address;
	};
	//////--------------------
	int* GetColorsAddress(int type, int nr)
	{
		int* address = 0;
		switch (type)
		{
		case 0:
			address = &mainParamsColors[nr*3];
			break;
		case 1:
			address = &filterParamsParamsColors[nr*3];
			break;
		case 2:
			address = &maskFilterParamsColors[nr*3];
			break;
		};
		return address;
	};



	void	SetFilterParam(int nr, double val) { filterParams[nr] = val; };
	double	GetFilterParam(int nr) { return filterParams[nr]; };
	double *GetFilterParams() { return filterParams; };

	unsigned char *getMaskBuffer(int nr) { return maskBuffer[nr]; };
	RawDataSet *getFilterBufferData(int nr) { return filterBufferData[nr]; };

	void	SetFilterMode(int i) { filterMode = i; };
	int		GetFilterMode() { return filterMode; };

	void	SetLayerChildMode(bool t) { isChild = t; };
	bool	GetLayerChildMode() { return isChild; };


	void	SetLayerMixerMode(bool t) { isMixerMode = t; };
	bool	GetLayerMixerMode() { return isMixerMode; };

	void	SetParentEnabledMode(bool t) { isParrentEnabled = t; };
	bool	GetParentEnabledMode() { return isParrentEnabled; };


	void		writeLayerParams(QTextStream &stream);
	bool		readLayerParams(QTextStream &stream);
	void		writeParam(QTextStream &stream, QString p1, QString p2, QString p3 = "",QString red="", QString green = "", QString blue = "")
	{ 
		QString txt = p1 + ": " + p2;
		//if (p3.isEmpty())stream << p1 + ": " << p2 << "\n"; else stream << p1 + ": " << p2 << ": " << p3 << "\n";
		if (!p3.isEmpty())
		{
			txt += ": " + p3;
		}
		if (!red.isEmpty())
		{
			txt+= ": " + red+ ": " + green+ ": " + blue;
		}
		stream << txt << "\n";
	};

	bool		compareText(QString a, QString b) { return ((a.compare(b) == 0)) ? true : false; }; //true jak takie same, false jak nie



};

#endif //RAWDATALAYER_H
