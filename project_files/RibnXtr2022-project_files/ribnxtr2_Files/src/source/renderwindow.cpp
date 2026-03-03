#include "renderwindow.h"

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
//#include "VarTrace\vartracestore.h"

void RenderWindow::countFps()
{
	last_time = current_time;
	current_time = current_time.currentTime();
	elapsed_time = (current_time.second() * 1000 + current_time.msec()) - (last_time.second() * 1000 + last_time.msec()); // /!\ max = 59 seconds 999 ms
	this->fps = 1000.0 / elapsed_time;

	if (fps < 1)
	{
		fpsCount = 0;
		fpsSum = 0;
	}
	fpsCount++;
	fpsSum += fps;

	emit sendFps(fps, double(fpsSum) / fpsCount);
}
//void RenderWindow::paintEvent(QPaintEvent* event)
//
//}
////-----------
void RenderWindow::paintEvent(QPaintEvent* event)
{
	/*QVTKOpenGLNativeWidget::paintEvent(event);
	this->renderWindow->Render();
	return;*/

	if (blockPaintEvent == false)
	{
		counter++;
		emit updatedView();
		blockPaintEvent = true;
		
		if (this->poly == NULL)
		{
			
			QVTKOpenGLNativeWidget::paintEvent(event);
			
			this->renderWindow->Render();
		//	qDebug() << "2";

		}
		else
		{
		
			{
				
				QVTKOpenGLNativeWidget::paintEvent(event);
				
				QPen pen(1);
				pen.setWidth(1);
				pen.setStyle(Qt::SolidLine);
				pen.setColor(Qt::red);
				QPainter Painter2;//(QWidget*)parent());
				Painter2.begin(this);
				Painter2.setPen(pen);
				//grabImageR->save("d:\\tmp\\pic.png", "PNG");
				int pos = poly->count();
				if (grabImageR != NULL)
					Painter2.drawImage(0, 0, *grabImageR);
				if (pos > 1)
				{
					Painter2.drawPolyline(*poly);
				}

			

				Painter2.end();
			

			}


		}
		//qDebug() << "VWidget" << "-out";
		blockPaintEvent = false;
	}
	else
	{
	//qDebug() << "4";
 		QPainter Painter2;//(QWidget*)parent());
		Painter2.begin(this);
		if (grabImageR != NULL)
		Painter2.drawImage(0, 0, *grabImageR);
		Painter2.end();
 }
}
/*! \class RenderWindow
\brief Combines a QGLWidget of \a qt with a vtkRenderWindow of \a vtk.

This class combines the great GUI toolkit qt (<a
href="http://www.trolltech.no">www.troll.no</a>) with the great visualization toolkit vtk
(<a href="http://www.kitware.com/vtk">www.kitware.com/vtk</a>).

\version $Revision: 1.3 $
\author Jan Stifter <a href="mailto:j.stifter@medres.ch">j.stifter@medres.ch</a>.
Original idea and code from Tom Radcliffe
<a href="mailto:radcliffe@igotechnologies.com">radcliffe@igotechnologies.com</a>.
\date $Date: 2000/09/14 14:56:10 $
\par Thanks to:
Kuno Meyer <meyerk@student.ethz.ch>,
Marcel van Straten <streets@mediaport.org>
Kent Vander Velden <graphix@iastate.edu>

\par History:
\code
$Log: renderwindow.cpp,v $
Revision 1.3  2000/09/14 14:56:10  jstifter
Changes proposed from Kent Vander Velden <graphix@iastate.edu>
CameraRotate, CameraTranslate, CameraZoom problems fixed (mostly due to
incorrect clipping)

Revision 1.2  2000/09/14 13:25:43  jstifter
Added Support for *parent, *name in the constructors

Revision 1.1.1.1  2000/08/15 09:36:39  jstifter
first cvs import

\endcode

*/


/*!
\brief Constructs a new RenderWindow

The new RenderWindow has a vtkRenderWindow, a
vtkRenderer and a vtkLight initialized
\param glf the QGLFormat, which will be passed directly to the underlying QGLWidget
\param parent directly passed to QGLWidget
\param name directly passed to QGLWidget
*/
//RenderWindow::RenderWindow(QGLFormat glf, QWidget* parent, const char* name)
//	: QGLWidget(glf, parent) {
//	blockPaintEvent = false;
//	box = NULL;
//	frameSrcImg = NULL;
//
//	frameDstImg = NULL;
//	fpsCount = 0;
//	fpsSum = 0;
//	//stereoWidget = NULL;
//	//	stereoD3DWidget=NULL;
//	this->InitRenderWindow();
//	setAutoFillBackground(true);
//	counter = 0;
//	img = NULL;
//	GetRenderWindow()->StereoCapableWindowOn();
//	//stereoLayer = new StereoView::BufferedViewLayer();
//	blockPaintPolygon = true;
//}

/*!
\brief Constructs a new RenderWindow

The new RenderWindow has a vtkRenderWindow, a
vtkRenderer and a vtkLight initialized
\param parent directly passed to QGLWidget
\param name directly passed to QGLWidget
*/
RenderWindow::RenderWindow(QWidget* parent, const char* name) : QVTKOpenGLNativeWidget(parent)
{
	grabImageR = NULL;
	poly = NULL;
	blockPaintEvent = false;
	box = NULL;
	this->InitRenderWindow();
	//	stereoWidget = NULL;
		//setAutoFillBackground(true);
	counter = 0;
	img = NULL;
	frameSrcImg = NULL;
	//stereoLayer = new StereoView::BufferedViewLayer();
	blockPaintPolygon = true;
	//interactor()->Disable();
	
}

//void RenderWindow::SetSideBySideWidget(StereoViewWidget* w)
//{
//	StereoViewWidget* old = stereoWidget;
//	stereoWidget = w;
//	if (stereoWidget == NULL) return;
//	if (stereoWidget != old) {
//		stereoWidget->setBufferedViewLayer(stereoLayer);
//	}
//};

/*!
\brief Initialize the RenderWindow

This method is called from the two Constructors
*/
void RenderWindow::InitRenderWindow() {


	this->renderer = vtkRenderer::New();
	renderWindow = vtkGenericOpenGLRenderWindow::New();
	renderWindow->AddRenderer(renderer);
	this->setRenderWindow(renderWindow);

	this->light = vtkLight::New();

	this->renderer->AddLight(this->light);

	

	//show();

	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(5);
	sphereSource->Update();
	vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputData(sphereSource->GetOutput());

	vtkSmartPointer<vtkActor> sphere = vtkSmartPointer<vtkActor>::New();
	sphere->SetMapper(sphereMapper);
	renderer->AddViewProp(sphere);
	sphere->GetProperty()->SetOpacity(0);
	this->initNeeded = false;
	
	renderer->ResetCamera();
	renderWindow->Render();

	//renderWindow->SetAlphaBitPlanes(1);
	renderWindow->SetMultiSamples(0);
	renderWindow->SetGlobalMaximumNumberOfMultiSamples(0);

	renderer->RemoveViewProp(sphere);
	renderer->ResetCamera();
	renderWindow->Render();
}

/*!
\brief Destructs the RenderWindow.
*/
RenderWindow::~RenderWindow() {
	this->renderer->Delete();
	this->renderWindow->Delete();
	if (img != NULL) delete img;
}

/*!
\brief returns the vtkRenderer used
*/
vtkRenderer* RenderWindow::GetRenderer() {
	return this->renderer;
}

/*!
\brief returns the vtkRenderWindow used
*/
vtkGenericOpenGLRenderWindow* RenderWindow::GetRenderWindow() {
	return this->renderWindow;
}

/*!
\brief Initialize the OpenGL Window
*/
//void RenderWindow::initializeGL()
//{
//	//glEnable(GL_MULTISAMPLE); 
//	//GLint bufs; 
//	//GLint samples; 
//	//glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs); 
//	//glGetIntegerv(GL_SAMPLES, &samples); 
//	//qDebug("Have %d buffers and %d samples", bufs, samples);
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glColor3f(1.0, 0.0, 0.0);
//
//
//
//
//}

/*!
\brief Resize the OpenGL Window
*/
//void RenderWindow::resizeGL(int w, int h) {
//
//
//	wi = w; he = h;
//	//  gluOrtho2D( 0.0, w, 0.0, h );
//	// glMatrixMode( GL_MODELVIEW );
//	//QTimer::singleShot( 1, this, SLOT(updateGL2()) );
//	updateGL2();
//
//
//}
////---------------------------
//void RenderWindow::updateGL2()
//{
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0.0, wi, 0.0, he);
//	glMatrixMode(GL_MODELVIEW);
//	//updateGL();
//	//update();
//}

/*!
\brief Render the scene
*/
//void RenderWindow::paintGL( )
//{
//	
//   // Get the native window ID and pass it
//   // to the VTK render window
//   // before we render for the first time...
//  
//   
//
//}
//----------------------------------
//void	RenderWindow::ForceResizeGL(int w, int h)
//{
//	resizeGL(w, h);
//	update();
//}
//----------------------------
void RenderWindow::grabRenderWindow2(QImage* img, bool buffer)
{
	/*vtkSmartPointer<vtkWindowToImageFilter> Filterek = vtkSmartPointer<vtkWindowToImageFilter>::New();
	Filterek->SetInput(this->GetRenderWindow());
	Filterek->SetInputBufferTypeToRGBA();
	if (buffer == false) Filterek->ReadFrontBufferOff();

	Filterek->Update();

	Filterek->Modified();
	vtkSmartPointer<vtkImageData> IData = Filterek->GetOutput();
	IData->Modified();

	int size[3];
	IData->GetDimensions(size);

	resizeBilinearVTKtoQt((int*)IData->GetScalarPointer(), size[0], size[1], (int*)img->bits(), img->width(), img->height());
*/



}
//--------------
QImage* RenderWindow::grabRenderWindow(bool buffer)
{
	vtkWindowToImageFilter* Filterek = vtkWindowToImageFilter::New();
	Filterek->SetInput(this->GetRenderWindow());
	Filterek->SetInputBufferTypeToRGBA();
	if (buffer == false) Filterek->ReadFrontBufferOff();
	Filterek->Update();


	//GetRenderWindow()->Render();
	Filterek->Modified();
	vtkImageData* IData = Filterek->GetOutput();
	IData->Modified();

	int size[3];
	IData->GetDimensions(size);

	if (img == NULL)
		img = new QImage(size[0], size[1], QImage::Format_RGB32);
	else if ((img->width() != size[0]) || (img->height() != size[1]))
	{
		delete img;
		img = new QImage(size[0], size[1], QImage::Format_RGB32);
	}
	unsigned char* pixeldata = img->bits();//new unsigned char[s*4];
	unsigned char* pixeldataV = (unsigned char*)IData->GetScalarPointer();
	int i, j;
	int step1 = 0;
	int	step2 = (size[0] * (size[1] - 1)) * 4;

	for (int j = 0; j < size[1]; j++)
		//parallel_for((size[0]*(size[1]-1))*4, 0,-(2*size[0])*4, [&](int j) 
	{
		for (i = 0; i < size[0]; i++)
			//parallel_for(0, size[0], [&](int i) 
		{
			pixeldata[step1++] = pixeldataV[step2 + 2];
			pixeldata[step1++] = pixeldataV[step2 + 1];
			pixeldata[step1++] = pixeldataV[step2];
			step2 += 4;
			step1++;
		}//);

		 //memcpy(pixeldata+step1,pixeldataV+step2,size[0]*4);
		 //step1+=size[0]*4;
		 //step2-=size[0]*4;
		step2 -= (2 * size[0]) * 4;
	}//);


	int lineLen = size[0] * 4;
	int sliceSize = size[0] * size[1] * 4;
	int ssMLlen = sliceSize - lineLen;


	for (int j = 0; j < sliceSize; j += lineLen)
		//parallel_for(0,sliceSize,lineLen, [&](int j) 
	{
		int p1, p2;
		for (int i = 0; i < size[0] * 4; i += 4)
		{

			p1 = j + i;
			p2 = ssMLlen - j + i + 2;
			/*	pixeldata[p1++]=pixeldataV[p2--];
			pixeldata[p1++]=pixeldataV[p2--];
			pixeldata[p1]=pixeldataV[p2];*/

			pixeldata[p1++] = pixeldataV[p2--];
			pixeldata[p1++] = pixeldataV[p2--];
			pixeldata[p1] = pixeldataV[p2];
		}


	}//);
	 //memcpy(pixeldata,pixeldataV,size[0]*size[1]*4);

	IData = NULL;


	Filterek->Delete();
	return img;
}
//--------
void RenderWindow::grabRenderWindow2Box(QImage* img, int sw, int sh, bool side, bool half, bool buffer, bool interlaced, int* box)
{

	//	if (box == NULL)
	//
	//	{
	//
	//		int size[2];
	//		size[0] = this->GetRenderWindow()->GetSize()[0];
	//		size[1] = this->GetRenderWindow()->GetSize()[1];
	//
	//
	//
	//		if (size[0] * size[1] != stereoLayer->frameBufW * stereoLayer->frameBufH) {
	//			delete[] stereoLayer->frameBuf[1];
	//			delete[] stereoLayer->frameBuf[0];
	//			stereoLayer->frameBuf[0] = new char[(size[0] + 2) * (size[1] + 2) * 3]; // +2 cols and +2 rows for optional padding
	//			stereoLayer->frameBuf[1] = new char[(size[0] + 2) * (size[1] + 2) * 3];
	//			stereoLayer->frameBufW = size[0];
	//			stereoLayer->frameBufH = size[1];
	//		}
	//
	//		int ch = side ? 1 : 0;
	//
	//
	//#ifdef WIN32
	//		if (buffer == true)
	//			glReadBuffer(static_cast<GLenum>(((vtkWin32OpenGLRenderWindow*)(GetRenderWindow()))->GetFrontLeftBuffer()));
	//		else
	//			glReadBuffer(static_cast<GLenum>(((vtkWin32OpenGLRenderWindow*)(GetRenderWindow()))->GetFrontRightBuffer()));
	//#else
	//		if (buffer == true)
	//			glReadBuffer(static_cast<GLenum>(((vtkXOpenGLRenderWindow*)(GetRenderWindow()))->GetFrontLeftBuffer()));
	//		else
	//			glReadBuffer(static_cast<GLenum>(((vtkXOpenGLRenderWindow*)(GetRenderWindow()))->GetFrontRightBuffer()));
	//#endif
	//		//glCopyTexImage2D(stereoWidget->texView[ch], 0, GL_RGB, 0, 0, size[0], size[1], 0);
	//
	//		glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
	//		glPixelStorei(GL_PACK_ALIGNMENT, 4);
	//		glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	//		glReadPixels(0, 0, size[0], size[1], GL_RGB, GL_UNSIGNED_BYTE, stereoLayer->frameBuf[ch]);
	//		glPopClientAttrib();
	//		stereoLayer->doFrameRefresh = true;
	//	}
	//	else
	//	{
	//		int x0 = box[0];
	//		int y0 = box[1];
	//
	//		int w1 = box[2];
	//		int h1 = box[3];
	//
	//		int size[2];
	//		size[0] = this->GetRenderWindow()->GetSize()[0];
	//		size[1] = this->GetRenderWindow()->GetSize()[1];
	//		if ((w1 > 0) && (w1 < size[0])) size[0] = w1;//dop poprawki pozniejk
	//		if ((h1 > 0) && (h1 < size[0])) size[1] = h1;
	//
	//
	//
	//		if (size[0] * size[1] != stereoLayer->frameBufW * stereoLayer->frameBufH) {
	//			delete[] stereoLayer->frameBuf[1];
	//			delete[] stereoLayer->frameBuf[0];
	//			stereoLayer->frameBuf[0] = new char[(size[0] + 2) * (size[1] + 2) * 3]; // +2 cols and +2 rows for optional padding
	//			stereoLayer->frameBuf[1] = new char[(size[0] + 2) * (size[1] + 2) * 3];
	//			stereoLayer->frameBufW = size[0];
	//			stereoLayer->frameBufH = size[1];
	//		}
	//
	//		int ch = side ? 1 : 0;
	//
	//
	//#ifdef WIN32
	//		if (buffer == true)
	//			glReadBuffer(static_cast<GLenum>(((vtkWin32OpenGLRenderWindow*)(GetRenderWindow()))->GetFrontLeftBuffer()));
	//		else
	//			glReadBuffer(static_cast<GLenum>(((vtkWin32OpenGLRenderWindow*)(GetRenderWindow()))->GetFrontRightBuffer()));
	//#else
	//		if (buffer == true)
	//			glReadBuffer(static_cast<GLenum>(((vtkXOpenGLRenderWindow*)(GetRenderWindow()))->GetFrontLeftBuffer()));
	//		else
	//			glReadBuffer(static_cast<GLenum>(((vtkXOpenGLRenderWindow*)(GetRenderWindow()))->GetFrontRightBuffer()));
	//#endif
	//		//glCopyTexImage2D(stereoWidget->texView[ch], 0, GL_RGB, 0, 0, size[0], size[1], 0);
	//
	//		glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
	//		glPixelStorei(GL_PACK_ALIGNMENT, 4);
	//		glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	//		glReadPixels(x0, y0, size[0], size[1], GL_RGB, GL_UNSIGNED_BYTE, stereoLayer->frameBuf[ch]);
	//		glPopClientAttrib();
	//		stereoLayer->doFrameRefresh = true;
	//
	//
	//	}
}
