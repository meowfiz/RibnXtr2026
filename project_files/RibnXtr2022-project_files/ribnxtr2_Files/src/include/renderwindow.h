#ifndef _RenderWindow_H_
#define _RenderWindow_H_

//
// $Id: renderwindow.h,v 1.2 2000/09/14 13:25:43 jstifter Exp $
//
//#include <glew.h>
//#include "StereoViewWidget.h"

//#include <QtOpenGL>
#include <qwidget.h>
#include <qimage.h>
#include <qpainter.h>
#include <qslider.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qsound.h>
#include <math.h>
#include <qapplication.h>
#include <qfiledialog.h>
#include <qthread.h>
#include <qthread.h>
#include <qdebug.h>


//#include <qgl.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <QTime>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkTextureObject.h>
#include <vtkProperty.h>
#include <vtkLight.h>
#include <qtimer.h>
#include <qpolygon.h>
//#include "OtherCommonFunctions.h"

#include <QVTKRenderWidget.h>
#include <QVTKOpenGLWidget.h>
//#include <ppl.h>
//using namespace Concurrency;

//class StereoViewWidget;
//namespace StereoView {
//	class BufferedViewLayer;
//};



class RenderWindow : public QVTKOpenGLNativeWidget {

	Q_OBJECT

public:
	QPolygon* poly;
	QImage* grabImageR;
	int* box;

	//RenderWindow(QGLFormat glf, QWidget* parent = 0, const char* name = 0);
	RenderWindow(QWidget* parent = 0, const char* name = 0);

	~RenderWindow();

	vtkRenderer* GetRenderer();
	vtkGenericOpenGLRenderWindow* GetRenderWindow();
	QImage* grabRenderWindow(bool buffer = false);
	void grabRenderWindow2(QImage* img, bool buffer);
	void grabRenderWindow2Box(QImage* img, int sw, int sh, bool side, bool half, bool buffer, bool interlaced = false, int* box = NULL);

	//void	ForceResizeGL(int w, int h);
	//void SetSideBySideWidget(StereoViewWidget* w);
	//   void SetD3DWidget(StereoViewD3DWidget *w){stereoD3DWidget=w;};;
	void countFps();
	//GLuint leftTexture;
	//GLuint rightTexture;
	int GetCount() { return counter; };

	void SetBlockPaintPolygon(bool p) {
		blockPaintPolygon = p;
		//this->setAutoBufferSwap(p);
		
	};

	void SetBox(int* b) { box = b; }
	bool blockPaintEvent;
	void SetBlockPaintEvent(bool b) { blockPaintEvent = b; 	};

protected:

	//virtual void resizeGL(int w, int h);
	//virtual void initializeGL();
	virtual void paintEvent(QPaintEvent*);

	int      currentMousePos[2];
private:
	
	bool blockPaintPolygon;
	int fpsCount, fpsSum;
	int counter;
	void InitRenderWindow();
	bool initNeeded;
	vtkRenderer* renderer;
	vtkGenericOpenGLRenderWindow* renderWindow;
	vtkLight* light;
	int wi, he;
	//StereoViewWidget* stereoWidget;
	//StereoView::BufferedViewLayer* stereoLayer;
	// StereoViewD3DWidget *stereoD3DWidget;
	QImage* img;
	QImage* frameSrcImg, * frameDstImg;
	// QImage *imgTmp;


	QTime current_time;
	QTime last_time;
	int elapsed_time;
	double fps; // current fps in the loop, depending on the "work time" required during the paintGL call
				//	void paintGl(){ };

private slots:
	//virtual void updateGL2();

signals:
	void updatedView();
	void sendFps(double fps, double avg);
};

#endif // _RenderWindow_H_
