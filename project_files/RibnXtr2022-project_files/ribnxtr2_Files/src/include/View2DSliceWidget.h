#ifndef VIEW_H
#define VIEW_H

//#ifndef		VTK
//#define VTK
//#endif
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include	<qwidget.h>
#include	<qimage.h>
#include	<qpainter.h>
#include	<qslider.h>
#include	<qlabel.h>
#include	<qlayout.h>
//#include	<q3vbox.h>
//#include	<q3hbox.h>

//#include	<q3frame.h>
#include <QPolygon>
#include	<qdatetime.h>
//#include <Q3VBoxLayout>

#include	<stdio.h>

#include	"RawDataSet.h"
#include	"ImageWidget.h"
#include	"DataSet.h"
#include	"LinkedList.h"
#include	"OtherCommonFunctions.h"
#include	"LinkedList.h"

// --------------
#include	"RawDataMixerLite.h"
#include	"ImageShapes.h"
#if defined (VTK)
#include	<vtkColorTransferFunction.h>
#endif
#include <omp.h>
// --------------
#define MAX_HIST_IMG_WIDTH 1280

class RawDataSet;

class View : public QWidget// MyDockWidget
{
	Q_OBJECT

public:


	View(QList<QSlider2*>* _sliderList,LinkedList<DataSet*> *_DataSetList, QWidget *parent = 0, QString name = "", Qt::WindowFlags flags = 0);

	//View(QWidget *parent=0,const char *name=0);				
	~View(void);

	void	InsertNewData(int ModelPos, short _view, QString _Title = "");

	void	InsertMaskData(unsigned char *MaskData);
	void	InsertMask2Data(unsigned char *MaskData);

	void	CreateSpecialSlice();
	void	PaintSpecialSlice();


	void	PaintMainDataSetSice();
	bool	PaintMixedDataSlice();
	void	PaintMainDataSetMaskSlice();
	void	PaintMainDataSetMask2Slice();
	void	PaintCorrespondingDataSetSlices();
	void	PaintInspectionWindow();
	void	PaintVolMaskSlice();

	void	PaintRegisteredSlices(RawDataSet *RegisteredSlice, PaletteRGBA *PaletteReg, unsigned char Opacity);
	void	UpdatRegSlice(RawDataSet *RegisteredSlice, PaletteRGBA *PaletteReg, unsigned char Opacity);

	void	SetBackgroundColor(QColor col, bool hideBoxSLider = false);
	void	SetObjectTexts(QStringList texts) { IWidget->SetObjectTexts(texts); };
	void	SetCropPlanes(int *);
	unsigned char *GetMaskTable() { return maskTable; };
	unsigned char *GetMaskTable1() { return maskTable1; };

	bool locked;
	ImageShapes 	*GetShapes() { return &shapes; };
	//-----------------------------------------
	bool repaintSpecialSlice;
	bool paintSpecialSliceEnabled;
	unsigned char specialSliceOpacity;
	unsigned short *specialSliceData;
	int specialSliceMode;

	ImageShapes				shapes;

	LinkedList<DataSet*>	*DataSetList;

	DataSet					*MainDataSet;
	//	DataSet					*InspectionDataSet;

	int						NumberOfInspectionWindows;
	int						*InspectionWindowsArray;

	int						*Views2DArray;
	int						NumberOf2DViews;


	int *intBuffer0;
	int *intBuffer1;
	unsigned char *maskTable;
	unsigned char *maskTable1;
	//----------------------------------	-------
	int		FirstHistValue;
	int		LastHistValue;

	int pictureIndex;
	unsigned char	*MaskData;
	unsigned char	*Mask2Data;
	bool	ShowMask;
	bool	ShowMask2;
	//	bool	RegisteredDataExists;

	double 	dx, dy;
	int  	rx, ry, rz;
	int		sx, sy, sz, sxy;

	int		PosX, PosY;
	int		InspX, InspY, InspZ;

	//	unsigned char *InspectionMask;

	int		CrossX, CrossY;
	int		CrossType;

	PaletteRGBA		*Pal;
	PaletteRGBA		*PalTemp;

	//	PaletteRGBA		*PalR;
	PaletteRGBA		*PalTempR;

	int PosZ;

	unsigned char thresholdMaskOpacity;
	unsigned char MaskOpacity;
	unsigned char Mask2Opacity;

	QColor MaskColor;
	QColor Mask2Color;
	QColor MaskColorX;
	QColor MaskColorX2;


	bool	StretchPalette;

	double	StretchedScalePalette;

	QString		Title;
	bool		Viewer;
	short 		view;
	RawDataSet	*data;//,*dataR;
	QImage 		*Image;
	QImage		*ImageR;
	//	QImage		*ImageM;
	//	bool		*MaskTable;
	QSlider2		*Slider;
	QLabel		*Label;



	QLabel *NameLabel;
	QLabel *SliceLabel;
	QLabel *PositionLabel;
	QLabel *SizesLabel;
	int *histTab;
	double scaleHistFactor;
	int histTabSize;

	ImageWidget *IWidget;
	QVBoxLayout	*VLayout;

	int		SliceNumber;
	int		Range;
	double	ScalePalette;
	//	double	ScalePaletteR;
	int		*TempPaletteArray;
	int* TempPaletteArrayForMaskView;
	int tmpLth, tmpUth;
	int		LTresh, UTresh;
	int		col;

	bool	InspectionWindow;
	int		InspectionMode;
	int		TempInspX, TempInspY, TempInspZ;

	QString nameX;

	void	SmoothFilterImageTab(unsigned char *image, int W, int H);
	void	SetCyclePos(int i) { cyclePos = i; SlotSetOpacity(); };
	void	SetCycleList(int *List, int size) { cycleList = List; cycleListSize = size; cyclePos = 0; };
	void	SetRawDataMixer(RawDataMixerLite *m);
	int crossection;
	void	GetTransHistTab(unsigned short *data, int size);

	void	SetCropPlanesVisibility(bool sel);
	int bounds3d[6];
#if defined (VTK)
	void SetVolumeStructuredPoints(vtkStructuredPoints *volPts, vtkColorTransferFunction		*cols);
#endif
	void SetBlockMixer(bool mode) { blockMixer = mode; };

	void	SetZoomMode(bool mode) { zoomMode = mode; };
	int counter;
	void	SetThresholdColors(QColor lower, QColor upper) {
		lowerColor = lower; upperColor = upper;
	}

	void	SetCalculateHistogram(bool a) { calculateHistogram = a; };
	bool	GetCalculateHistogram() { return calculateHistogram; };
	void	SetTextsInCorners(bool v) { IWidget->SetDrawTextsInCorners(v); };
	void	SetVoxelScaleFactor(double scale) { IWidget->SetVoxelScaleFactor(scale); };
	void	SetExternalImageUpdateMode(bool  mode) { externalImageUpdateMode = mode; };
	qint64	GetUpdatedTimeID() { return updateTimeID; };
	void	SetWarning(bool mode, QString text, QPen pen, QFont font) {
		IWidget->SetWarning(mode, text, pen, font);
	};
	void	SetViewID(int id) { viewID = id; };
	int*GetBounds2d() { return bounds2d; };

	void SetCropLinesColors(QColor c1, QColor c2) {
		IWidget->SetCropLinesColors(c1, c2);
	};

private:

	int revThMode;

	QList<QSlider2*>* sliderList;
	int bounds2d[4];
	bool	externalImageUpdateMode;
	QColor upperColor, lowerColor;
	int threshColorMode;

	void wheelEvent(QWheelEvent *event);
	
	unsigned char colsTab[768];

	bool magnyfyingModeOn;
	QFrame *Box;
	int numberOfThreads;
	int sendPolyLineMode;
	RawDataMixerLite *mixer;
	bool isSetSliceBlocked;
	int *cycleList;
	int  cyclePos;
	int	 cycleListSize;
	int ButtonMode;
	QString	pictureName;
	bool	autoGrab;
	unsigned short specialSliceMaxValue;
	//QStringList objectTexts;

	//int pos3D[3];
	int threshMode;
#if defined (VTK)
	vtkPoints *pts;
#endif
	bool calculateHistogram;
	bool blockMixer;

	unsigned short (View::*GetProjectionValue)(unsigned short *tab, int size);

	unsigned short GetAvgProjection(unsigned short *tab, int size);
	unsigned short GetMinProjection(unsigned short *tab, int size);
	unsigned short GetMaxProjection(unsigned short *tab, int size);
	unsigned short GetMedProjection(unsigned short *tab, int size);
	unsigned short GetTest1dProjection(unsigned short *tab, int size);
	unsigned short GetTest2dProjection(unsigned short *tab, int size);
	unsigned short GetFirstHitProjection(unsigned short* tab, int size);
	unsigned short GetLastHitProjection(unsigned short* tab, int size);

	void	GetSpecialRay(unsigned short *ray, int rayLen, unsigned short *tab, int x, int y, int nx, int ny, int nz, int ss, bool *validate);
	unsigned short (View::*GetSliceValue)(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);
	unsigned short GetSliceValue0(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);
	unsigned short GetSliceValue1(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);
	unsigned short GetSliceValue2(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate);



	void SetColorAndMask(int cc, int LowerThreshold, int UpperThreshold, unsigned  char maskR, unsigned  char maskG, unsigned  char maskB, unsigned  char maskR2, unsigned  char maskG2, unsigned  char maskB2, unsigned int* dataXI, int pos2D);
	void SetColor2(int cc, int LowerThreshold, int UpperThreshold, unsigned  char* colLow, unsigned  char* colUp, unsigned int* dataXI, int pos2D);

	bool zoomMode;
	int specialParam1;
	int specialParam2;

	bool ellipseMode;
	int ellipseDist;
	int viewID;
	void repaint2();
	int pressedX, pressedY;

protected:

#if defined (VTK)
	vtkStructuredPoints *volumePoints;
	vtkColorTransferFunction *volColors;
#endif
	unsigned char volPointsOpacity;


	qint64 updateTimeID;

	bool wheelOn;

	private slots:


	virtual void updateSlice();



	public slots:

	void	SlotChangeRevThreshMode(int mode);
	void	SlotlRightWithAltPressed(QMouseEvent* event);
	void	SlotlRightWithAltReleased(QMouseEvent* event);
	void	SlotSetSpecialSliceParam1(int v) { specialParam1 = v;	CreateSpecialSlice(); updateSlice(); };
	void	SlotSetSpecialSliceParam2(int v) { specialParam2 = v; CreateSpecialSlice(); updateSlice(); };
	void	SetFonts(QFont f1, QFont f2) { IWidget->SetFonts(f1, f2); };
	void	SetPens(QPen f1, QPen f2) { IWidget->SetPens(f1, f2); };;
	void	SlotSetSendXYMode(bool mode) { IWidget->SlotSetSendXYMode(mode); }; 
	void	SlotSetDualCrossMode(bool mode, QPen p1, QPen p2) { IWidget->SlotSetDualCrossMode(mode, p1, p2); }; 
	void	SlotSetEllipseMode(bool mode, int dist) { ellipseMode = mode; ellipseDist = dist; IWidget->SlotSetEllipseMode(mode, dist); };
	void	SlotSetMouseWheelOn(bool mode) { wheelOn = mode; };
	void	SlotSetThreshColorMode(int mode);
	void	SliceSelect(int);
	void	SlotSetVolumeOpacity(int opacity);
	void	SlotUpdateCropPlanes(int *);
	void	SlotCopyMask();
	void	SlotPasteMask();
	void	SlotRedrawWidget();
	void	SlotChangeThreshMode(bool);
	void	SlotSwitchToStretchedPaletteMode();
	void	SlotSwitchToNormalPaletteMode();
	void	SlotSetOpacity();
	void	SlotSetMaskOpacity(int _MaskOpacity);
	void	SlotSetThresholdMaskOpacity(int _MaskOpacity);
	void	SlotSetMask2Opacity(int _MaskOpacity);

	void	DeleteData();
	void	ChangePalette(int LowerThreshold, int UpperThreshold);
	void	ChangeTempPaletteArray(int *_TPaletteArray);
	void	PaintCross(int x, int y);
	void	FindCoordinates(int x, int y, int Button);
	void	FindCoordinates2(int x, int y, int Button);
	void	SetSlice(int x, int y, int z);
	void	SlotBlockSetSlice(bool block) { isSetSliceBlocked = block; };;
	void	ChangePaletteType(void);
	void	ChangePaletteTypeR(void);



	void	SlotSetCrossColor(QColor CrossColor);
	void	SlotSetMaskColor(QColor	MaskColor);
	void	SlotSetMask2Color(QColor	MaskColor);
	void	SlotSetMaskColorX(QColor	MaskColor);
	void	SlotSetMaskColorX2(QColor	MaskColor);
	void	SlotSetCrossType(int CrossType);
	void	SlotSetCrossMode(int CrossType);
	//	void	SlotInspectionWindowModeOn();
	//	void	SlotInspectionWindowModeOff();
	void	SlotSwitchInspectionMode();
	void	SetInspectionWindowSize(int, int, int);
	void	SlotSetInterpolation(int Interpolation);


	void	SlotInsertNewInspection(int InspectionPos);
	void	SlotInsertNew2DViews(int *Views2D);

	void	SlotInsertNewInspectionWindows(int *InspW);

	void	SlotConnectSendCoordsAndUpdate();
	void	SlotDisconnectSendCoordsAndUpdate();

	void	SlotSetButtonMode(int);
	void	SlotUpdateCrossPointer(int x, int y, int z, int Button);

	void	SlotBlockRepaint(bool);


	void	SlotSendViewID();
	void	SlotMoveMask(int);

	void	SlotSwitchSendPointArray(int);
	void	SlotSendPointArray(QPolygon, double, double);
	void	SlotSwitchAutoGrab(QString filename);
	void	SlotGrabImage();





	void	SlotSetSpecialSliceMode(int mode);
	void	SlotSetSpecialSliceOpacity(int opacity);
	void	SlotSetSpecialSliceVisibility(int v);
	void	SlotSendShape(QPolygonF *p);

	void	SlotSliderFocus(int val);




	//virtual void keyPressEvent(QKeyEvent* event);
	//SignalSendPointArray
signals:
	void	signalRightWithAltClicked(int x, int y, QWidget*);
	void	signalSendViewUserID(int);
	void	signalSelectSliceChanged();
	void	signalWidgetRedrawed();
	void	 signalNewBounds3D(int *);
	void	signalSend2DMask(unsigned char*);

	void	SendCoordinates(int x, int y, int z);
	void	SendCoordinates2(int x, int y, int z, int Button);
	void	SendCoordinatesWithButton(int x, int y, int z, int Button);
	void	SendViewId(int, int);
	void	SignalSendButton(int);
	void	SignalSendButtonAndView(int, int);

	void	SignalSendIWidgetMask(unsigned char *);
	void	SignalSendIWidgetMaskImage(QImage);
	void	SendMessage(const QString &Message);
	void	SignalMoveMask(int, int);
	void	SignalSendPointArray(QPolygon, double, double, int, int);
#if defined (VTK)
	void	SignalSendVtkPoints(vtkPoints *pts);
#endif

	void	signalMaskDataInserted(unsigned char *);

	void	signalSendCurrentHistogram(RawDataSet *, int *histTab, int size);
	void	SendMessageToLog(const QString &Message);

};

#endif //VIEW_H
