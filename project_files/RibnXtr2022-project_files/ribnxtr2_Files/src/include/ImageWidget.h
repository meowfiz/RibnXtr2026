#ifndef		IMAGE_WIDGET
#define 	IMAGE_WIDGET

#ifndef		VTK
#define VTK
#endif

#include	<qwidget.h>
#include	<qimage.h>
#include	<qpainter.h>
#include	<QPolygon>
#include	<qbrush.h>
#include	<QMutex>
#
//Added by qt3to4:
#include <QPaintEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QKeyEvent>


#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>


#include	"OtherCommonFunctions.h"
#include	"ImageShapes.h"
#if defined (VTK)
#include	<vtkPiecewiseFunction.h>
#endif



class ImageWidget : public QWidget
{
	Q_OBJECT

public:

	ImageWidget(QWidget *pWidget = 0, const char *name = "", Qt::WindowFlags f = 0);
	~ImageWidget();

	virtual void	DataCleared();
#if defined (VTK)
	virtual void	SetNewTransferFunction(vtkPiecewiseFunction *f1, vtkPiecewiseFunction *f2);
	virtual void	SetTransferFunctionModeOn();
	virtual void	SetTransferFunctionModeOff();
	virtual void	PaintTransferFunctions(QImage *Image);
#endif



	virtual void	RepaintWithCross(int X, int Y);
	virtual void	setValues(QImage *pImage, int rx, int ry, double dx, double dy, unsigned char *mask = NULL);

	virtual	void	SetMaskPaintMode(int);
	virtual void	SetGetMaskMode(int);




	virtual	void	SetInspectionWindowSizes(int x, int y, int z);


	virtual	void	SetName(const QString &name);
	QString			GetName() { return Name; };
	virtual	void	SetSliceNumber(int Nr);

	virtual	void	SetPosZ(int value);
	virtual	void	SetCrossColor(QColor Color);
	virtual	void	SetCrossType(int Type);
	int				Getm_rx();
	virtual	void	SetCrossColorValue(int);
	virtual	void	SetInterpolation(int Value);

	QImage *		GetCurrentImage() { return m_Image; };

	/** Opcjonalny mutex: jeśli ustawiony, paintEvent blokuje go na czas malowania (np. podgląd Kinecta z wątku roboczego). */
	void SetPaintMutex(QMutex* m) { m_paintMutex = m; }

	void	SetObjectTexts(QStringList texts) { objectTexts = texts; };
	void	SetPos3D(int pos[3]) { pos3D[0] = pos[0]; pos3D[1] = pos[1]; pos3D[2] = pos[2]; }
	void	SetPos2D(double x, double y) { pos2D[0] = x; pos2D[1] = y; ; }
	void	SetDrawTextsInCorners(bool v) { drawTextsInCorners = v; };
	void	SetVoxelRealValue(double v) { voxelRealValue = v; };
	void	SetFonts(QFont f1, QFont f2) { font1 = f1; font2 = f2; };;
	void	SetPens(QPen f1, QPen f2) { pen1 = f1; pen2 = f2; };;
	void	SetVoxelScaleFactor(double scale) { voxelScaleFactor = scale; };
	bool liveMaskTrace;

	bool ImageExists;
	//

	void	SetShapes(ImageShapes *s) {
		shapes = s;
	};
	ImageShapes *GetShapes() { return shapes; };

	//unsigned char maskOpacity;
	QColor *maskColor;
	QColor *mask2Color;
	unsigned char *colsTab;
	unsigned char *volPointsOpacity;

	void MagnifyignGlassMask(QImage *img);

	void SetCropLines(int *);
	void	SetWarning(bool mode, QString text, QPen pen, QFont font) {
		warningMode = mode; warningPen = pen; warningText = text; warningFont = font;
	};

	unsigned char *getTempMask() { return tmpMask; };
	void SetCropLinesColors(QColor c1, QColor c2) {
		cropLinesColor1 = c1; cropLinesColor2 = c2;
	};

protected:

	bool keyModesEnabled;
	QColor cropLinesColor1, cropLinesColor2;
#if defined (VTK)
	vtkPiecewiseFunction *volumeScalarOpacityFunction;
	vtkPiecewiseFunction *volumeGradientpacityFunction;
#endif
	QString warningText;
	QPen warningPen;
	QFont warningFont;
	bool warningMode;
	double voxelScaleFactor;
	QPen pen1, pen2;
	QFont font1, font2;
	QStringList objectTexts;
	QRect cliprect;
	double imageRescale;
	int rescaleDiffX, rescaleDiffY;
	bool cursonOnLines;
	bool moveLinesMode;
	int nrOfSelectedLine;
	bool showCropLines;


	unsigned char *mask;
	ImageShapes *shapes;

	bool TransferFunctionMode;
	double realXSize, realYSize;
	bool SendMaskMode;
	bool PointArraySentAlready;


	int	OldSrcWidth;
	int	OldSrcHeight;
	int	OldTgtWidth;
	int	OldTgtHeight;
	int	OldInterpolation;

	double	*XFParamTab, *YFParamTab;
	double	*XIParamTab, *YIParamTab;

	int OldXSize, OldYSize;
	QImage *TmpImage;
	QImage *maskImage;
	unsigned char *tmpMask2;
	unsigned char *tmpMask;

	unsigned char timeChar;
	QString Name;

	QImage *m_Image;
	QMutex* m_paintMutex;
	int		m_rx;
	int		m_ry;
	double	m_dx;
	double	m_dy;
	double	m_s;
	bool	Cross;
	double	CrossX, CrossY;
	double	XScale, YScale;
	double	XCScale, YCScale;

	int ImgSizeX;
	int ImgSizeY;
	int ImgPosX;
	int	ImgPosY;
	int pos3D[3];
	double pos2D[2];
	bool drawTextsInCorners;
	double voxelRealValue;
	int TempX, TempY;

	int RX, RY;



	QColor backgroundColor;
	char *ImageMask;

	int InspX, InspY, InspZ;
	int PosZ;
	int SliceNumber;

	QPolygon PointArray;
	QPolygonF fPointArray;
	QPointF lastPoint;
	QPoint	Point1, Point2;
	QPointF Point1f;
	QPoint	PointPos, PointSize;
	bool	PointArrayExists;


	int		MaskPaintMode;
	bool	GetMaskMode;
	//	int		PointIndex;


	bool	sendShape;

	QColor	CrossColor;
	int		CrossColorValue;
	int		CrossType;
	bool	Corner;
	int		Button;
	int		Interpolation;
	bool	InspectionMode;

	bool	BlockRepaint;

	double fx, fy;
	int PointX, PointY;
	int xP1, wP1, yP1, hP1;
	int painterMode;

	int colorButtonMode;
	int borderSize;
	bool drawOpacityMode;


	int cropLines[4];

	bool ellipseMode;
	int ellipseDist;
	bool sendXYMode;
	bool drawDualCross;

	double pressX, pressY;
	QString parentName;

	QPen dualCrossPen1, dualCrossPen2;
	//int pos3D[3];



	virtual void paintEvent(QPaintEvent*);

	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);


	unsigned char *FillMaskWithPolygon(QPolygon *Points, int ScreenX, int ScreenY, int MaskX, int MaskY);

	//	virtual void FillPolygonBorder(const QPointArray &Points,char *Img,int IWidgh,IHeight);
	virtual void	focusOutEvent(QFocusEvent *);


	/*
	void	initializeGL( void );
	void	resizeGL( int w, int h );
	void	paintGL();
	*/


	public slots:
	 
	void	SlotSetDualCrossMode(bool mode, QPen p1, QPen p2) { drawDualCross = mode; dualCrossPen1 = p1; dualCrossPen2 = p2; }
	void	SlotSetSendXYMode(bool mode) { sendXYMode = mode; };
	void	SlotSetEllipseMode(bool mode, int dist) { ellipseMode = mode; ellipseDist = dist; }
	void	SlotSetBackgroundColor(QColor col) { backgroundColor = col; };
	void	SlotImageZoomIn(int val) { imageRescale = 1.0 + val / 1000.0;	repaint(); };//1000 bedzie mial slider co to obsluzy
	void	SlotShowCropLines(bool val);

	void	SlotBlockRepaint(bool block);

#if defined (VTK)
	void	SlotPressTransferFunctionPoint(int x, int y, int Button);
	void	SlotMoveTransferFunctionPoint(int x, int y, int Button);
	void	SlotReleaseTransferFunctionPoint(int x, int y, int Button);
#endif

	void	SlotSetSendMaskMode(bool a);
	void	SlotGetEllipseParams(double x0, double y0, double w, double h);
	void	SlotGetRectParams(double x0, double y0, double w, double h);
	void	SlotGetPolylineParams(QPolygonF *polys);
	void	SlotGetPolygonParams(QPolygonF *polys);
	void	SlotUpdatePolygon(QPolygonF *p);
	void	SetParentName(QString name) { parentName = name; }

	void	SetKeyModesEnabled(bool e) { keyModesEnabled = e; };

signals:
	void	signalRightWithAltPressed(QMouseEvent* event);
	void	signalRightWithAltReleased(QMouseEvent* event);
	void	signalDoubleClicked();
	void	signalSendXY(double x, double y);
	void	signalCropLines(int*);
	void	signalArrowPressed(int);
	void	signalCopyMask();
	void	signalPasteMask();
	//
	//
	void	signalImagePressed();
	void	signalImageReleased();

	//void	SignalSendShape(vtkPoints *pts);
	void	SignalRefreshGui();
	void	SignalSendPointP1(int x, int y);
	void	SignalSendPointP2(int x, int y);
	//
	void	SendPosition(int x, int y, int Button);

	void	SendPosition2(int x, int y, int Button);
	void	ImageReleased();
	void	ImageReleased(int);

	void	SignalSendMask(unsigned char*);
	void	SignalSendMaskImage(QImage img);

	void	SignalParentPleaseSendViewID();// :)


	void	SignalSendUserTransferFunction();
	void		SendMessage(const QString &Message);
	void		SendMessage2(const QString &Message);
	void	SendMessageToLog(const QString &Message);

	void	SignalMoveMask(int);
	void	SignalSendPointArray(QPolygon, double, double);

	void	SignalSendImage(QImage*);
	void	SignalSendShape(QPolygonF *p);


};



#endif // IMAGE_WIDGET
