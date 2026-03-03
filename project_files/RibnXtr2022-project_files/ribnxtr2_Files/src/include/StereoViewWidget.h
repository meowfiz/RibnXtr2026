//#ifndef STEREO_VIEW_WIDGET
//#define STEREO_VIEW_WIDGET
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//#include <time.h>
//#include <QtOpenGL>
//
//#include <qwidget.h>
//#include <qimage.h>
//#include <qpainter.h>
//#include <qslider.h>
//#include <qlabel.h>
//#include <qtimer.h>
//#include <qsound.h>
//#include <math.h>
//#include "OtherCommonFunctions.h"
////#include "Kinalyzer/menu3d.h" 
////#include "Kinalyzer/kinintegrator.h"
////#include "Kinalyzer/kininter3.h"
////#include "Kinalyzer/timedvars.h"
////#include "VarTrace/vartraceview.h"
//#include <qfont.h>
//#include "StereoViewLayer.h"
//
////struct UpperMenuItem
////{
////	float x0, y0;
////	float x1, y1;
////	float w, h;
////	TimedVars::TimedBool over;
////	TimedVars::TimedBool selected;
////
////	UpperMenuItem(float x = 0, float y = 0, float w = 100, float h = 100)
////		: x0(0), y0(-h), x1(x), y1(y), w(w), h(h)
////	{}
////};
//
//class StereoViewWidget : public QGLWidget
//	//public QWidget
//{
//	Q_OBJECT
//
//public:
//	bool animMode;
//
//	int *box;
//	StereoViewWidget(QWidget *parent = NULL, QGLWidget * shareWidget = 0);
//	~StereoViewWidget();
//	void appendLayer(StereoView::Layer *layer);
//	void prependLayer(StereoView::Layer *layer);
//	void setBufferedViewLayer(StereoView::BufferedViewLayer *layer);
//	void initializeGL();
//	void resizeGL(int w, int h);
//	void paintGL();
//	QImage *getCurrentStereoImage();
//	void grabCurrent(QString filename);
//	//StereoView::KinMenuLayer *getMenuLayer() { return menuLayer; }
//
//	/*
//	following functions use the widgets units of pixels. Caller is responsible of
//	converting real units to widget pixel units. If widget is using the whole screen
//	and position of head is known in mm, relative to center of the screen,
//	then one may compute xPx parameter in setHeadPosition as:
//	widget.setHeadPosition(HeadPosMM*widget.width()/screenWidthMM, ...);
//	Head position, while head is in front of the screen is always positive,
//	raising while distance to the screen raises.
//	/|
//	/ |/x
//	/  /
//	|  /|
//	| +----> z
//	| | |
//	| |y|
//	| V/
//	| /
//	|/
//	*/
//	void setCustomBox(int*b){ box = b; };
//	void setEyesDisparity(float leftRightPx) { setEyesDisparity(-leftRightPx, leftRightPx); }
//	void setEyesDisparity(float leftPx, float rightPx) { eyesShift[0] = leftPx; eyesShift[1] = rightPx; }
//	void setHeadPosition(float xPx, float yPx, float zPx) { headPos[0] = xPx; headPos[1] = yPx; headPos[2] = zPx; }
//
//	//void refreshFrame() { doFrameRefresh = true; }
//
//
//	int	 getStereoMode(){ return stereoMode; };
//	void setStereoMode(int val){ stereoMode = val; qWarning() << ("set stereo mode: %d", val); };
//
//	void setStereoView(int left, int right);
//	int getStereoViewLeft();
//	int getStereoViewRight();
//
//	void updateInputRes(int w, int h);
//	void updateWidget();
//
//	int	 getPainterMode(){ return painterMode; };
//	void setPainterMode(int val){ painterMode = val; };
//
//	void setNewPainterParams(double *params);
//
//	int	 getForceHalfMode(){ return forceHalfMode; };
//	void setForceHalfMode(int val){ forceHalfMode = val; };
//
//	void deleteImg3D();
//
//private:
//	QList<StereoView::Layer *> layersNew;
//	QList<StereoView::Layer *> layersPre;
//	void initNewLayers();
//	QImage *img3D;
//protected:
//	float eyesShift[2];
//	float headPos[3];
//
//	QList<StereoView::Layer *> layers;
//	//StereoView::KinMenuLayer *menuLayer;
//
//	//Menu3D menu;
//	//VarTrace::View *plotsView;
//	//TimedVars::TimedVar<QString> fsInfoText; // full screen info text		
//	//QList<UpperMenuItem *> upMenu;
//	//int menuCameraLastTool;
//	//int menuActorLastTool;
//	//TimedVars::LazyBool upMenuDisplay;
//	//TimedVars::TimedBool mainHandStationary;
//	//QFont fsInfoFont;
//	//Kin::ProducerNetworkUDP *kinProducer;
//	//Kin::DevicesSet *kinSet;
//	//TimedVars::TimedBool frameIsGood;
//	//TimedVars::TimedBool facingCamera;
//	//Kin::Inter3 *kinInter;
//	//Kin::User fakeUser;
//	bool useFakeUser;
//	QSound *sndBip;
//	QSound *sndClick;
//	QSound *sndUnClick;
//
//	void drawRect(float x, float y, float w, float h);
//	void drawRectTextured(float x, float y, float w, float h, GLuint);
//	void drawRectTextured(float x, float y, float w, float h, const QString &);
//
//	/*KinProducer *kinProducer;
//	KinDevicesSet *kinSet;
//	KinInterpreter *kinInter;*/
//	bool doMenuReload;
//	bool doFrameRefresh;
//	int frameno;
//
//	int stereoViewLeft;
//	int stereoViewRight;
//
//	bool forceHalfMode;
//	int 	 newW, newH;
//	double painterParams[500];
//	int painterMode;
//	int prevWidth, prevHeight;
//	//QPixmap cursorPixmap;		
//	GLuint cursorTexId;
//
//	int stereoMode;
//
//	int mousePos[2];
//	bool paintWithMousePointer;
//	int  sourceSize[2];
//	int  posLeft[2];
//	int  posRight[2];
//	GLuint handCursorTex;
//	GLuint handCursorGrabTex;
//
//	double scaleFactorX, scaleFactorY;
//
//
//	//	virtual void paintEvent(QPaintEvent*);
//	void showEvent(QShowEvent *);
//	void hideEvent(QHideEvent *);
//	void closeEvent(QCloseEvent *);
//	void mouseMoveEvent(QMouseEvent *);
//	void mouseReleaseEvent(QMouseEvent *);
//
//	GLfloat rot[3], xOffs[3], yOffs[3], xInc[3];
//	GLuint pbufferList;
//	GLuint dynamicTexture;
//	GLuint cubeTexture;
//	//   int timerId;
//	bool hasDynamicTextureUpdate;
//
//	QTimer repaintTimer;
//
//	QGLPixelBuffer *pbuffer;
//
//signals:
//	void signalWidgetChanged();
//	void signalActorPicker(int x, int y, int mode);
//
//	void mainHandPress(int x, int y, Qt::MouseButton mb, bool dbl);
//	void mainHandRelease(int x, int y, Qt::MouseButton mb);
//	void mainHandMoved(int x, int y, Qt::MouseButton mb);
//	void interactWithCurrentActor();
//	void interactWithCamera();
//	void resetAllActorsTransformations();
//	void generateAnimation();
//	void abortAnimation();
//	public slots:
//	void reloadMenu();
//	void menuShow();
//	void menuHide();
//	void menuNext();
//	void menuSelect();
//	void repaintTimeout();
//	void slotMousePointerMoved(int x, int y);
//	void slotMousePointerMoved(int x, int y, int button);
//	//void leadingUserChanged(Kin::User *);
//};
//
//#endif //STEREO_VIEW_WIDGET
