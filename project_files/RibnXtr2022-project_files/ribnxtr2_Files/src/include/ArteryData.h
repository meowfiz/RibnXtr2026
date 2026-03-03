#ifndef	ARTERY_DATA_H
#define ARTERY_DATA_H

 
#include <qobject.h>
#include <qapplication.h>
////#include <q3filedialog.h>
//Added by qt3to4:
#include <QTextStream>
#include <qvector3d.h>

#include	<stdio.h>
#include	<math.h>
#include	"DataSet.h"
#include	"RawDataSet.h"
#include	"SurfaceData.h"
#include	"OtherCommonFunctions.h"
#include	"Geometry.h"
#include	"SurfaceCollection.h"
#include	"VtkWidget.h"
#include	"SurfaceProcessing.h"
#include	"PlaneObject.h"
#include	"ByteMaskCollection.h"



 

//class RawDataSet;

//---------------------------------

class ArteryData: public QWidget
{
	Q_OBJECT

		public: 
	ArteryData( PaletteRGBA *_Palette,unsigned char *_veinColor=NULL,int _maxMaskNr=50);
	void	SetVtkWidget( VtkWidget *_vWidget){vWidget=_vWidget;};



	~ArteryData();
	void	ClearMainData(int dataNr){mainData[dataNr]=NULL;mainRawData[dataNr]=NULL;};
	bool	SetMainData(int dataNr,DataSet *_mainData)	;				//{	if (_mainData!=NULL) {mainData=_mainData;return true;} return false;};
	//void	SetMainDataPath(const QString _mainDataPath)		{	mainDataPath=_mainDataPath;};
	//bool	SetMainDataMask1(unsigned char *_mainDataMask1)		{	if (_mainDataMask1!=NULL) {mainDataMask1=_mainDataMask1;return true;} return false;};	
	//void	SetMainDataMask1Path(const QString _mainDataMask1Path)		{	mainDataMask1Path=_mainDataMask1Path;};
	//bool	SetMainDataMask2(unsigned char *_mainDataMask2)		{	if (_mainDataMask2!=NULL) {mainDataMask2=_mainDataMask2;return true;} return false;};	
	//void	SetMainDataMask2Path(const QString _mainDataMask2Path)	{	mainDataMask2Path=_mainDataMask2Path;};
	//bool	SetMainDataMask3(unsigned char *_mainDataMask3)		{	if (_mainDataMask3!=NULL) {mainDataMask3=_mainDataMask3;return true;} return false;};	
	//void	SetMainDataMask3Path(const QString _mainDataMask3Path)		{	mainDataMask3Path=_mainDataMask3Path;};

	bool	SetMainDataMask(int dataNr,int pos,DataSet *_mainData,int nr);
	void	ClearMainDataMask(int dataNr,int pos){mainDataMasks[dataNr][pos]=-1;};
	//bool	SetCropData(RawDataSet *_cropData)	;//				{	if (_cropData!=NULL) {cropData=_cropData;return true;} return false;};
//	void	SetCropDataPath(const QString _cropDataPath)		{	cropDataPath=_cropDataPath;};
//	bool	SetCropDataMask1(unsigned char *_cropDataMask1)		{	if (_cropDataMask1!=NULL) {cropDataMask1=_cropDataMask1;return true;} return false;};	
////	void	SetCropDataMask1Path(const QString _cropDataMask1Path)		{	cropDataMask1Path=_cropDataMask1Path;};
//	bool	SetCropDataMask2(unsigned char *_cropDataMask2)		{	if (_cropDataMask2!=NULL) {cropDataMask2=_cropDataMask2;return true;} return false;};	
////	void	SetCropDataMask2Path(const QString _cropDataMask2Path)		{	cropDataMask2Path=_cropDataMask2Path;};
//	bool	SetCropDataMask3(unsigned char *_cropDataMask3)		{	if (_cropDataMask3!=NULL) {cropDataMask3=_cropDataMask3;return true;} return false;};	
////	void	SetCropDataMask3Path(const QString _cropDataMask3Path)		{	cropDataMask3Path=_cropDataMask3Path;};
	//void	SetCropDataMask(int nr,DataSet *_mainData,int nr);


	DataSet *GetMainRawData(int dataNr){return mainRawData[dataNr];};
	//RawDataSet *GetCropRawData(){return cropRawData;};
	//unsigned char *GetMainDataMask1(){return mainDataMask1;};
	//unsigned char *GetMainDataMask2(){return mainDataMask2;};
	//unsigned char *GetMainDataMask3(){return mainDataMask3;};
	//unsigned char *GetCropData1(){return cropDataMask1;};
	//unsigned char *GetCropData2(){return cropDataMask2;};
	//unsigned char *GetCropData3(){return cropDataMask3;};

	void	SetOriginAndBaseVectors(double *_baseOrigin,double *_baseVectors);
	double *GetBaseOrigin(){return baseOrigin;};
	double *GetBaseVectors(){return baseVectors;};
	double *GetSeedPoint(){return seedPoint;};


	QVector <QVector3D> seeds;
	QVector <QVector3D> ignorePoints;

	QVector <QVector3D> GetSeeds() {
		return seeds;
	};
	void SetSeeds(QVector <QVector3D> x)
	{
		seeds = x;
	};

	void ClearSeeds() {
		seeds.clear();
	};
	QVector <QVector3D> GetIgnorePoints() {
		return ignorePoints;
	};
	void ClearIgnorePoints() {
		ignorePoints.clear();
	};
	void SetIgnorePoints(QVector <QVector3D> x)
	{
		ignorePoints = x;
	};

	

	LinkedList <PolygonSurfaceData*> *GetFrontSurfaces(){return frontSurfaces;};
	LinkedList <PolygonSurfaceData*> *GetTubeSurfaces(){return tubeSurfaces;}


	DataSet *GetMainData(int pos){return mainData[pos];};
	int		GetMainDataMask(int i,int j){return mainDataMasks[i][j];};
	void	SetSeedPoint(double *_seedPoint);


	bool	SetFrontSurfaces(LinkedList <PolygonSurfaceData*> *surfaces);
	bool	SetTubeSurfaces(LinkedList <PolygonSurfaceData*> *surfaces);
	void	ClearFrontSurfaces();
	void	ClearTubeSurfaces();

	void	LoadState(const QString filename,bool *fields);
	void	SaveState(const QString filename);

	void	RemoveAllItemsByClearingPoiners(LinkedList <PolygonSurfaceData*> *list);

	QString	GetMainDataDir(){return mainDataDir;};

	void	SetSelectedPointsNr(int nr){selectedPointsNr=nr;};
	int		GetSelectedPointsNr(){return selectedPointsNr;};

	void	SetSelectedPointsPointer(double *ptr){selectedPoints=ptr;};
	void	SetSelectedPointsExistPointer(bool *ptr){selectedPointsExist=ptr;};
	
	void	SetProgDirPath(QString n){progDirPath=n;};
	QString GetProgDirPath(){return progDirPath;};

	private:

		QString progDirPath;
		


		bool	saveRawDataInfo(DataSet *data,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text);
		bool	saveMaskDataInfo(DataSet *data,int maskNr,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text);
		bool	saveSurfaceInfo(PolygonSurfaceData *pData,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text,bool uniqueName=false,bool modified=false);
		bool	saveSurfaceSet(LinkedList <PolygonSurfaceData*> *surfaces,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text);
		bool	savePoints(int nr, double *pts, bool *existTab,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text);
		bool	savePoints(QVector <QVector3D> *pts, QTextStream &stream, const QString mainPath, const QString text);
		bool	saveNumbers(double *nr, int nrOfItems, int itemCount,QTextStream &stream,const QString text);
		bool	loadPoints(int &nr, double *pts, bool *exTab,QTextStream &stream,const QString mainPath);
		bool ArteryData::loadPoints(QVector <QVector3D> *pts, QTextStream &stream, const QString mainPath);
		bool	 loadSurfaceDataSet(LinkedList <PolygonSurfaceData*> *surfaces,QTextStream &stream,const QString mainPath);

		QString GetAbsPath(QString mainPath,QString relFilePath);
		bool loadNumbers(double *number,  int nrOfItems, int itemCount,QTextStream &stream);

		bool testtt(){return true;};
		DataSet *mainData[2]; 
		DataSet *mainRawData[2];
		int mainDataMasks[2][3];

		 PaletteRGBA *palette;
		 int maxMaskNr;
		 VtkWidget *vWidget;
		 unsigned char *veinColor;
		 QString mainDataDir;
		 //QString calcDir;
		//QString		mainDataPath;
		//unsigned char *mainDataMask1;
		//QString		mainDataMask1Path;
		//unsigned char *mainDataMask2;
		//QString		mainDataMask2Path;		
		//unsigned char *mainDataMask3;
		//QString		mainDataMask3Path;

		//DataSet *cropData;
		//RawDataSet *cropRawData;
		//int cropDataMasks[3];
		//QString		cropDataPath;
		//unsigned char *cropDataMask1;
		///QString		cropDataMask1Path;
		//unsigned char *cropDataMask2;//
		//QString		cropDataMask2Path;
		//unsigned char *cropDataMask3;
		//QString		cropDataMask3Path;


		double baseOrigin[3];
		double baseVectors[9];

		double seedPoint[3];
		double *selectedPoints;//[300];
		bool *selectedPointsExist;
		int selectedPointsNr;



		LinkedList <PolygonSurfaceData*> *frontSurfaces;
		LinkedList <PolygonSurfaceData*> *tubeSurfaces;
		

	//roles

	signals:

		void	SingnalMaskUpdated();
		void	SignalSendDataSet(DataSet *);
		void	SignalSendNewSurface(PolygonSurfaceData *_SurfaceData);
		void	SendMessage(const QString &Message);

};
//-------------

#endif //   ARTERY_DATA_H
