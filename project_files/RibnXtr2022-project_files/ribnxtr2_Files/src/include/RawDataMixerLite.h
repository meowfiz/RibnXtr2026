#ifndef	RAWDATAMIXERLITE_H
#define	RAWDATAMIXERLITE_H

#include <qobject.h>
#include <qstring.h>

#include <math.h>
#include <stdlib.h>

#include "RawDataSet.h"
#include "OtherCommonFunctions.h"
#include "DataSet.h"
#include "GUIPanelWidget.h"
#include "RawDataLayer.h"
#include "LayerMaskFilters.h"
#include "LayerFilters.h"

#include "ImageShapes.h"

#include <qelapsedtimer.h>


class RawDataLayer;
class LayerFilter;
class LayerMaskFilter;
//---------------------------------
class RawDataMixerLite : public QObject
{
	Q_OBJECT

protected:

	double paramsX[PARAMS_NR];
	QString paramsXNames[PARAMS_NR];

	QString objName;
	bool uShortInputMode;
	QTreeWidgetItem *lastModifiedLayerItem;
	ImageShapes *shapes;
	ImageWidget *iWidget;
	//	bool layerOptimize;

	QTime current_time;
	QTime last_time;

	int elapsed_time;
	double fps;
	int fpsCount, fpsSum;
	int counter;

	bool somethingToMix;

	bool noInputDataSetListMode;

	int startOptID;

	int			numberOfLayerMods;
	bool		childModeOpt;
	int			minLayerMods;
	int			sliceNrOpt;

	int viewType;

	bool externalLayerList;

	LinkedList <RawDataLayer *> *rawDataLayerList;
	LinkedList<DataSet*>	*dataSetList;
	ListItem<DataSet*>		*mainListItem;
	int n[3];
	double scale[3];

	DataSet *mainDataSet;
	int mainListItemPos;
	RawDataLayer *mainLayer;
	unsigned short *dataView;
	unsigned short *dataViewTmp;


	RawDataSet *dataToFilter;
	unsigned char *mmask;
	unsigned char* mmask2;
	unsigned short *childBuffer;

	unsigned short *childBufferOpt;
	unsigned short *dataBufferOpt;

	unsigned short *uShortTmpArray;

	unsigned char *mainBigMask;
	unsigned char* mainBigMask2;

	//int size[3];
	int sliceSize;
	//int Nx,Ny,Nz;;

	int nX, nY, nZ;
	double dX, dY, dZ;

	double *shaFunc;
	double *higFunc;
	double *midFunc;
	int funSize;

	int createdLayerCounter;
	unsigned short (RawDataMixerLite::*GetSliceValue)(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);

	unsigned short GetSliceValue0(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);
	unsigned short GetSliceValue1(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);
	unsigned short GetSliceValue2(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);

	unsigned char GetSliceValueMask0(unsigned char *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);
	unsigned char GetSliceValueMask1(unsigned char *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);
	unsigned char GetSliceValueMask2(unsigned char *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);

	//mixing functions

	//unsigned short MixFunction0(unsigned short a, unsigned short b) { return b; };																					//normal
	//unsigned short MixFunction1(unsigned short a, unsigned short b) { return (a + b) >> 2; };																		//average
	//unsigned short MixFunction2(unsigned short a, unsigned short b) { int val = (a*b) >> 8; return val;/*(val<256)?val:255;*/ };									//multiply
	//unsigned short MixFunction3(unsigned short a, unsigned short b) { return 255 - ((255 - a) * (255 - b) >> 8); };														//screen
	//unsigned short MixFunction4(unsigned short a, unsigned short b) { return (a<b) ? a : b; };																			//darken
	//unsigned short MixFunction5(unsigned short a, unsigned short b) { return (a>b) ? a : b; };																			//lighten
	//unsigned short MixFunction6(unsigned short a, unsigned short b) { return abs(a - b); };																			//difference
	//unsigned short MixFunction7(unsigned short a, unsigned short b) { return a + b<255 ? a + b : 255; };																	//add mode
	//unsigned short MixFunction8(unsigned short a, unsigned short b) { return (a<128) ? (a*b) >> 7 : 255 - ((255 - a)*(255 - b) >> 7); };											//overlay mode
	//unsigned short MixFunction9(unsigned short a, unsigned short b) { return (b<128) ? (a*b) >> 7 : 255 - ((255 - b)*(255 - a) >> 7); };											//hard light
	//unsigned short MixFunction10(unsigned short a, unsigned short b) { unsigned char c = a*b >> 8; return c + a * (255 - ((255 - a)*(255 - b) >> 8) - c) >> 8; };			//soft light
	//unsigned short MixFunction11(unsigned short a, unsigned short b) { if (b == 255) return 255;   unsigned short c = (a << 8) / (255 - b);  if (c > 255) return 255; else return c; }; //color dodge mode
	//unsigned short MixFunction12(unsigned short a, unsigned short b) { if (b == 0)  return 0;  short c = 255 - (((255 - a) << 8) / b); if (c < 0) return 0; else return c; }; //color burn mode
	//unsigned short MixFunction13(unsigned short a, unsigned short b) { if (b == 255) return 255; unsigned short c = a*a / (255 - b); if (c > 255) return 255; else return c; }; //reflect mode
	//unsigned short MixFunction14(unsigned short a, unsigned short b) { return (b>0) ? b : a; }; ///replace oprocz zero
	//unsigned short MixFunction15(unsigned short a, unsigned short b) { return (a>0) ? a : b; }; ///replace oprocz zero



	unsigned short MixFunction0(unsigned short a, unsigned short b) { return b; };																					//normal
	unsigned short MixFunction1(unsigned short a, unsigned short b) { return (a + b) >> 1; };																		//average
	unsigned short MixFunction2(unsigned short a, unsigned short b) { int val = (a*b) >> 8; return val;/*(val<256)?val:255;*/ };									//multiply
	unsigned short MixFunction3(unsigned short a, unsigned short b) { return 65535 - ((65535 - a) * (65535 - b) >> 16); };														//screen
	unsigned short MixFunction4(unsigned short a, unsigned short b) { return (a<b) ? a : b; };																			//darken
	unsigned short MixFunction5(unsigned short a, unsigned short b) { return (a>b) ? a : b; };																			//lighten
	unsigned short MixFunction6(unsigned short a, unsigned short b) { return abs(a - b); };																			//difference
	unsigned short MixFunction7(unsigned short a, unsigned short b) { return a + b<65535 ? a + b : 65535; };																	//add mode
	unsigned short MixFunction8(unsigned short a, unsigned short b) { return (a<32768) ? (a*b) >> 15 : 65535 - ((65535 - a)*(65535 - b) >> 15); };											//overlay mode
	unsigned short MixFunction9(unsigned short a, unsigned short b) { return (b<32768) ? (a*b) >> 15 : 65535 - ((65535 - b)*(65535 - a) >> 15); };											//hard light
	unsigned short MixFunction10(unsigned short a, unsigned short b) { unsigned char c = a*b >> 16; return c + a * (65535 - ((65535 - a)*(65535 - b) >> 16) - c) >> 16; };			//soft light
	unsigned short MixFunction11(unsigned short a, unsigned short b) { if (b == 65535) return 65535;   unsigned short c = (a << 16) / (65535 - b);  if (c > 65535) return 65535; else return c; }; //color dodge mode
	unsigned short MixFunction12(unsigned short a, unsigned short b) { if (b == 0)  return 0;  short c = 65535 - (((65535 - a) << 16) / b); if (c < 0) return 0; else return c; }; //color burn mode
	unsigned short MixFunction13(unsigned short a, unsigned short b) { if (b == 65535) return 65535; unsigned short c = a*a / (65535 - b); if (c > 65535) return 65535; else return c; }; //reflect mode
	unsigned short MixFunction14(unsigned short a, unsigned short b) { return (b>0) ? b : a; }; ///replace oprocz zero
	unsigned short MixFunction15(unsigned short a, unsigned short b) { return (a>0) ? a : b; }; ///replace oprocz zero

	unsigned short (RawDataMixerLite::*MixFunction[20])(unsigned short a, unsigned short b);
	void countFps();



	bool			filterModeChanged;
	bool			maskModeChanged;

	unsigned short *uShortBuffers[3];//trzeci dodaje jako bufor zapamietywania - czasem potrzeba miec dostep do wyniku posredniego poza filtrem!
	int *intBuffers[2];

	unsigned char *uCharBuffers[2];

	int position[2];

	QPolygon polygon;
	int globalLeftThreshold;
	int globalRightThreshold;



	LinkedList <LayerMaskFilter*> *layerMaskFilterList;
	LinkedList <LayerFilter*> *layerFilterList;

	QElapsedTimer *tm;
	int timeStamp;

public:


	void setElapsedTimer(QElapsedTimer *timer) { tm = timer; };
	int getTimeStamp() { return timeStamp;; }

	unsigned char *uCharTab;
	unsigned short *uShortTab;
	double *doubleTab;

	int uCharTabSize;
	int uShortTabSize;
	int doubleTabSize;




	LinkedList <RawDataLayer *>		*GetLayerList() { return rawDataLayerList; };
	LinkedList <LayerMaskFilter*>	*GetLayerMaskFilterList() { return layerMaskFilterList; };
	LinkedList <LayerFilter*>		*GetLayerFilterList() { return layerFilterList; };


	unsigned short* GetRememberBuffer() { return uShortBuffers[2]; };

	void setNoInputDataSetListMode(bool mode) { noInputDataSetListMode = mode; };

	void setObjName(QString n) { objName = n; };
	int  getSliceSize() { return nX*nY; };
	bool SetMainDataRawDataArray(void  *array);

	bool GetuShortInputMode() { return this->uShortInputMode; };
	void SetuShortInputMode(int m) { uShortInputMode = m; };

	RawDataMixerLite(LinkedList<DataSet*>	*_dataSetList, int _viewtype = 0, LinkedList <RawDataLayer *> *rawDataLayerList = NULL);
	virtual ~RawDataMixerLite();
	int GetNumberOfLayers() { return (rawDataLayerList != NULL) ? rawDataLayerList->GetNumberOfItems() : 0; };

	void UpdateMainData(ListItem<DataSet*>		*_mainListItem, bool createBuffers = true);

	void SetShapesAndVisImage(ImageShapes 	*s, ImageWidget *w);



	bool getSomethingToMix() { return somethingToMix; };

	DataSet *GetMainDataSet() { return mainDataSet; };

	bool			SaveLayerSettings(const QString & text);
	bool			LoadLayerSettings(const QString & text, bool updateGUI = false);

	bool inverseOpacity;


	void	SetShapes(ImageShapes *s) { shapes = s; };
	void	UpdateMaxValueForFilters(unsigned short maxValue);
	void	UpdateBuffers();

	bool	CreateNoInputDataSetListData(int nx, int ny, double dx, double dy);

	void	ClearShapes();
	void		SetFilterParamBasedOnNames(QString filterName, QString paramname, double val);
	public slots:

	void			SlotExternalUpdate() { emit signalParamsChanged(); };

	RawDataSet			*SlotGenerateMixedData();
	unsigned short	*SlotGenerateMixedSliceImage(int sliceNumber, double *inParamsX = NULL, int nrInParamsX = 0, double *outParamsX = NULL, int nrOutParamsX = 0);

	void			SlotSetTransversalPos(int, int);
	void			SlotUpdateBigMaskData(unsigned char *bigMask);
	void			SlotUpdateBigMaskData2(unsigned char* bigMask2);
	void			SlotGetPolyLine(QPolygon p);
	void			SlotSetGlobalThresholds(int t1, int t2);

signals:

	void			signalUpdateGUIAfterLoadingLayersFromFile();
	void			signalSendNewRawDataSet(RawDataSet *);

	void			signalParamsChanged();
	void			SendMessage(const QString &Message);
	void			signalUpdateNewVolumeMixerItem();
	void			redrawed();


};


bool FilterDataSet(RawDataSet *input, RawDataSet *output, QString filterName);
static RawDataMixerLite *mixer_pointer = NULL;
// static int mixer_data_type=1; // 1 unsigned short, 0 unsigned char
#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_Initialization(const char * p_plikKonfiguracja, int p_width, int p_height, double dx, double dy, bool mode);


#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_Initialization2(const  char * p_plikKonfiguracja, LinkedList<DataSet*>	*_dataSetList, int mainDataNr = 0);


#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_Filtruj(void * imgIN, void * imgOUT, int maxVal = 0, double *inParamsX = NULL, int nrInParamsX = 0, double *outParamsX = NULL, int nrOutParamsX = 0);


#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_SetShapesAndVisImage(ImageShapes 	*s, ImageWidget *w);

#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_Free(void);

#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_Free2(void);

#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_CurrentSliceSize(void);


#endif //RAWDATAMIXERLITE_H
