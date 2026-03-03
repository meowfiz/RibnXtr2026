#ifndef STEREOVIEWLAYER
#define STEREOVIEWLAYER 1

#include <QObject>
#include <QtOpenGL>
#include <QSizeF>
#include <QRectF>
#include <QSound>
//#include "Kinalyzer/objmath.h"
//#include "Kinalyzer/timedvars.h"
//#include "Kinalyzer/kininter3.h"

namespace StereoView {

struct LayerSetup {	
	float zZero;  // positive: in front of the screen, negative: to back of the screen
	float zNear;
	float zFar;
	bool visible;  // visibility flag
	bool flat;     // setup orthographic perspective
	               // if !flat: origin(0,0) <-> frame center==size/2
	bool osd;      // sticking to users view of the screen
	bool clip;
	bool fit;

	LayerSetup() 
		: zZero(0),
		zFar(20),
		zNear(10),
		visible(true),
		flat(false),
		osd(false),
		clip(false),
		fit(false)
	{}
};

class Layer : public QObject {
	Q_OBJECT
public:
	QRectF renderFrame[2];
	QRectF screenFrame;
	QSizeF screenSize;
	QGLWidget *ctx;

	Layer(QObject *parent = 0) : QObject(parent), ctx(NULL) { }	
	LayerSetup& getSetup() { return setup; }
	void setSetup(LayerSetup &setup) { this->setup = setup; }
	virtual void initGL() { };
	void onResize(float width, float height) { this->width = width; this->height = height; } 
	virtual void paintGLBegin() { } ;
	virtual void paintGL(int eye) = 0; // eye parameter may be used by layer if it wants to destinguish between eyes; but it's up to StereoViewWidget to setup apropriate camera perspective
	virtual void paintGLEnd() { };		
	virtual QSizeF getSize() { return QSizeF(1, 1); }
	//virtual bool onUserUpdate(Kin::User *u, bool fake) { return false; }

	// map from screen space to local space; screen space in chosen dimension spans [from,to]
	float xToLocal(float x, float from = -1, float to = 1);
//	float xToLocal(Kin::Hand *, float from = -1, float to = 1);
	float yToLocal(float y, float from = -1, float to = 1);
	//float yToLocal(Kin::Hand *, float from = -1, float to = 1);
protected:
	float width, height;
	LayerSetup setup;	
};

class TestLayer : public Layer {
	Q_OBJECT
public:
	TestLayer(QObject *parent = 0);
	void paintGL(int eye);
	QSizeF getSize() { return QSizeF(1.8, 1); }
};

class TestLayer3D : public Layer {
	Q_OBJECT
public:
	TestLayer3D(QObject *parent = 0);
	void paintGL(int eye);
	QSizeF getSize();
};

class BufferedViewLayer : public Layer {
	Q_OBJECT
public:
	GLuint texView[2]; // 0:left, 1:right
	bool switchSides;
	char *frameBuf[2];
	int frameBufW, frameBufH;	
	int doFrameRefresh;

	BufferedViewLayer(QObject *parent = 0);
	~BufferedViewLayer();
	void initGL();
	void paintGL(int eye);
	QSizeF getSize() { return QSizeF(frameBufW, frameBufH); }
};
//
//class KinMenuItem : public QObject {
//	Q_OBJECT
//public:
//	bool checkable;
//	int checkGroup;
//	float xp, yp;
//	int border;
//	QString label;
//	GLuint tex;	
//	
////	TimedVars::TimedEvent clicked;
//	bool clickBlock;
////	TimedVars::TimedBool over;
//	//TimedVars::TimedBool checked;
//
//	KinMenuItem(QObject *parent = NULL);
//	/*
//	border:
//		0: up
//		1: right
//		2: bottom
//		3: left
//	*/
//	KinMenuItem(QString label, QString texPath, float xp, float py, int border = 0, bool checkable = true, int group = 0);
//	float x(float visible = 1);
//	float y(float visible = 1);
//	void click();
//signals:
//	void triggered();
//};
//
//enum CursorType {
//	CUR_HIDDEN,
//	CUR_GRAB,
//	CUR_GRABBED,
//	CUR_PICK
//};
//
//class HandCursor {
//public:
//	//TimedVars::LazyBool visible;
//	CursorType type;
//	float x, y;
//	int side;	
//	
////	HandCursor() : visible(false, 600), side(1), x(0), y(0) { type = CUR_HIDDEN; }	
//};
//
//class KinMenuLayer : public Layer {
//	Q_OBJECT
//public:	
//	bool paintLabels;
//	BufferedViewLayer *view;
//	bool pickedActor;
//	//TimedVars::TimedVar<QString> infoText;
//	KinMenuItem *itReset, *itPlay, *itStop, *itHold;
//
//	KinMenuLayer(QObject *parent = 0);
//	~KinMenuLayer();
//	void initGL();
//	void paintGL(int eye);	
//	QSizeF getSize();
////	bool onUserUpdate(Kin::User *u, bool fake);
//
//	QList<KinMenuItem *> items;
//protected:
//	HandCursor cursors[2];
//	void paintCursor(HandCursor *, float size);
//	QFont infoFont;
//	QFont noUserInfoFont;
//	void selectItem(KinMenuItem *item);
//
//	//bool menuHandUpdate(Kin::Hand *h, Kin::Hand *hOther);
//	//bool viewHandUpdate(Kin::Hand *h, Kin::Hand *hOther);
//private:
//	KinMenuItem *itCamera, *itActor, *itMove, *itRotate, *itCameraTool, *itActorTool;
//	QSound *sndBip;
//	QSound *sndClick;
//	QSound *sndUnClick;
//	//TimedVars::LazyBool menuVisible;
//	//TimedVars::TimedBool handStationary;
////	TimedVars::TimedBool handOtherStationary;
//	//TimedVars::TimedBool toolUsing;
//	//TimedVars::TimedBool zoomMode;	
//	bool userActive;
//	float zoomDistStart;
//	int mainHand, otherHand;
//	void setCursorMode(CursorType t) { for (int i = 0; i < 2; ++i) cursors[i].type = t; }
//signals:
//	void mainHandPress(int x, int y, Qt::MouseButton mb, bool dbl);
//	void mainHandRelease(int x, int y, Qt::MouseButton mb);
//	void mainHandMoved(int x, int y, Qt::MouseButton mb);
//	void interactWithCurrentActor();
//	void interactWithCamera();	
//public slots:
////	void leadingUserChanged(Kin::User *);
//};

class TooltipsLayer : public Layer {
	// osd, flat
};

class LassoSelectorLayer : public Layer {

};

}

#endif