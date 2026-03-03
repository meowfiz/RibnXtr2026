//#pragma message("RibnXtr2023: main.cpp CHANGED 2026-02-27")
//#pragma message("SECOND BUILD TEST")
#include <qapplication.h>
//#include <qplatinumstyle.h>
//#include <qcleanlooksstyle.h>
//#include <qwindowsstyle.h>
//#include <qplastiquestyle.h>
//#include <qmacstyle.h>
#include <stdlib.h>
#include <qtextcodec.h>
#include <qfont.h>
#include <qstring.h>
#include <QSurfaceFormat>
///#include <QOpenGLFunctions>
//Rudy tu byl

//#include "ProgWindow.h" 
//#include <QVTKOpenGLWidget.h>
#include "MainWindow.h"
#include "PropertyWidget.h"
#include <vtkProperty.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <QCryptographicHash>

//#include <QTextStream.h>
#include "Iphlpapi.h"
#include <vtkGenericOpenGLRenderWindow.h>
//#include <vtkOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkCommand.h"
#include "vtkBoxWidget.h"
#include "vtkTransform.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "QVTKOpenGLNativeWidget.h"
#include "VtkWidget.h"
#include "renderwindow.h"
#include "renderwindowinteractor.h"
#include <vtkLight.h>

#include "RawDataSet.h"
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkStructuredPoints.h>
#include <vtkSphereSource.h>
#include <QVTKRenderWidget.h>
#include <renderwindow.h>
#include <renderwindowinteractor.h>
//#include <QVTKOpenGLWidget.h>
#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

QString getMacAddress();

class testWidget : public QVTKOpenGLNativeWidget
{
public:
	testWidget(QWidget* parent = 0);
	vtkRenderer* GetRenderer() {
		return renderer;
	};
	vtkGenericOpenGLRenderWindow* GetRenderWindow() {
		return renderWindow;
	}

private:
	vtkRenderer* renderer;
	vtkGenericOpenGLRenderWindow* renderWindow;
	;
protected:

		//virtual void resizeGL(int w, int h);
		//virtual void initializeGL();
	virtual void paintEvent(QPaintEvent*event)
	{
		QVTKOpenGLNativeWidget::paintEvent(event);
		this->renderWindow->Render();
	};
};

testWidget::testWidget(QWidget* parent) :QVTKOpenGLNativeWidget()
{
	this->renderer = vtkRenderer::New();
	renderWindow = vtkGenericOpenGLRenderWindow::New();
	renderWindow->AddRenderer(renderer);
	this->setRenderWindow(renderWindow);

	vtkLight*light = vtkLight::New();

	this->renderer->AddLight(light);
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
	renderer->ResetCamera();
	renderWindow->Render();


	
	renderWindow->SetMultiSamples(0);
	renderWindow->SetGlobalMaximumNumberOfMultiSamples(0);

//	show();
	//interactor()->Disable();
}


int main(int argc, char** argv)
{
	
	//MessageBoxA(nullptr, "MAIN STARTaaaa", "RibnXtr2023", MB_OK | MB_TOPMOST);
	//std::cout <<"txt";
	vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat(true));
	QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QApplication app(argc, argv); //Windows-1250

//	QWidget* w = new QWidget();
//	w->setLayout(new QVBoxLayout);
//	//testWidget* widget = new testWidget(w);
//	
//	//RenderWindow* widget = new RenderWindow(w);
//	VtkWidget * widget = new VtkWidget(w);
//	w->layout()->addWidget(widget);
//	widget->show();
//	w->showMaximized();
//
//	QWidget* w2 = new QWidget();
//	w2->setLayout(new QVBoxLayout);
//	
////	widget->blockPaintEvent = true;
//	
//	w2->layout()->addWidget(w);
//
//	w->setParent(w2);
////	widget->blockPaintEvent = false;
//	w2->show();
//
//	//
//	////qputenv("QT_OPENGL", "angle");
//	////QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
//
//
//	//
//
//	return app.exec();







	//qDebug("t1");
	//QStringList arguments = qApp->arguments();
	//qDebug("t1a");

	//return 0;
	//	QApplication app(argc, argv);
	////	QVTKOpenGLNativeWidget widget1;
	////	vtkNew<vtkGenericOpenGLRenderWindow> window0x;
	////	widget1.setRenderWindow(window0x);
	//////	widget1.show();
	////
	////
	////
	////	while (!widget1.isValid()) {
	////		app.processEvents();
	////	}
	////
	////
	//	/*vtkNew<vtkGenericOpenGLRenderWindow> window0;
	//	widget1.setRenderWindow(window0);
	//	widget1.show();
	//	while (!widget1.isValid()) {
	//		app.processEvents();
	//	}
	//*/
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	////	testWidget* widget = new testWidget();
	//	VtkWidget* widget = new VtkWidget();
	//
	//	//RenderWindow* widget = new RenderWindow();
	//	//RenderWindowInteractor* widget = new RenderWindowInteractor();
	//	/*while (!widget->isValid()) {
	//		app.processEvents();
	//	}*/
	//	//vtkNew<vtkGenericOpenGLRenderWindow> window0;
	//	//widget.setRenderWindow(window0);
	//	//widget.show();
	//	//app.processEvents();
	//	
	//
	//	//vtkNew<vtkRenderer> ren;
	//	/*ren->SetGradientBackground(1);
	//	ren->SetBackground2(0.7, 0.7, 0.7);
	//	ren->SetUseDepthPeeling(false);
	//	ren->SetMaximumNumberOfPeels(100);
	//	ren->SetOcclusionRatio(0.1);
	//	window0->SetAlphaBitPlanes(1);
	//	window0->SetMultiSamples(4);*/
	//
	//	//vtkNew<vtkSphereSource> sphere;
	//	//vtkNew<vtkPolyDataMapper> mapper;
	//	//mapper->SetInputConnection(sphere->GetOutputPort());
	//	//vtkNew<vtkActor> actor;
	//	//actor->SetMapper(mapper);
	//	////ren->AddActor(actor);
	//	//widget->Interactor->GetRenderer()->AddActor(actor);
	//	////widget.update();
	//	////app.processEvents();
	//	app.setActiveWindow(widget);
	//	widget->show();
	//
	//
	//
	//
	//
	//
	//
	//
	//	RawDataSet* Data3D = new RawDataSet();
	//
	//	
	//	if (Data3D->LoadRawDataSet2("D:/nifti.rdata") == true)
	//	{
	//		vtkStructuredPoints* VolumeStructuredPoints = vtkStructuredPoints::New();
	//		VolumeStructuredPoints->SetSpacing(Data3D->GetXscale(), Data3D->GetYscale(), Data3D->GetZscale());
	//		VolumeStructuredPoints->SetExtent(0, Data3D->GetNx() - 1, 0, Data3D->GetNy() - 1, 0, Data3D->GetNz() - 1);
	//
	//		VolumeStructuredPoints->AllocateScalars(VTK_UNSIGNED_SHORT, 2);
	//
	//		//VolumeStructuredPoints->SetNumberOfScalarComponents(1);
	//
	//		vtkUnsignedShortArray* Array = vtkUnsignedShortArray::New();
	//
	//		Array->SetVoidArray(Data3D->GetDataArray(), Data3D->GetVolumeSize(), 1);
	//
	//		VolumeStructuredPoints->GetPointData()->SetScalars(Array);
	//
	//
	//		vtkColorTransferFunction* colorFun = vtkColorTransferFunction::New();
	//		vtkPiecewiseFunction* opacityFun = vtkPiecewiseFunction::New();
	//		double opacityWindow = 4096;
	//		double opacityLevel = 2048;
	//		int blendType = 0;
	//		int clip = 0;
	//		double reductionFactor = 1.0;
	//		vtkVolumeProperty* property = vtkVolumeProperty::New();
	//		property->IndependentComponentsOn();
	//		property->SetColor(colorFun);
	//		property->SetScalarOpacity(opacityFun);
	//		property->SetInterpolationTypeToLinear();
	//		opacityFun->AddSegment(opacityLevel - 0.5 * opacityWindow, 0.0,
	//			opacityLevel + 0.5 * opacityWindow, 1.0);
	//
	//
	//
	//		vtkVolume* Volume = vtkVolume::New();
	//
	//		vtkOpenGLGPUVolumeRayCastMapper* VolumeMapper = vtkOpenGLGPUVolumeRayCastMapper::New();
	//		VolumeMapper->SetMaxMemoryInBytes(2.04e+9);
	//		//((vtkVolumeTextureMapper3D*)VolumeMapper)->SetBlendModeToComposite();
	//		VolumeMapper->SetBlendModeToComposite();
	//		//	VolumeMapper->SetMaximumImageSampleDistance(4.0);
	//		//	VolumeMapper->UseJitteringOn();
	//
	//			//	((vtkVolumeTextureMapper3D*)VolumeMapper)->set
	//		VolumeMapper->SetPartitions(2, 2, 2);
	//		Volume->SetMapper(VolumeMapper);
	//		VolumeMapper->SetInputData(VolumeStructuredPoints);
	//
	//		Volume->SetProperty(property);
	//		VolumeMapper->SetBlendModeToComposite();
	//		property->ShadeOn();
	//		widget->GetRenderer()->AddVolume(Volume);
	//		widget->repaint();
	//	}
	//
	//	return app.exec();

		//vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
		//iren->SetRenderWindow(widget->Interactor->GetRenderWindow());
		//vtkInteractorStyleTrackballCamera* style =
		//	vtkInteractorStyleTrackballCamera::New();
		//iren->SetInteractorStyle(style);
		//iren->Initialize();

		//const char* aa = window0->GetRenderingBackend();



		//int size = vtkTextureObject::GetMaximumTextureSize(window0);

		//window0->AddRenderer(ren);


	//	iren->Start();


		//// Next we create an instance of vtkConeSource and set some of its
		// // properties. The instance of vtkConeSource "cone" is part of a
		// // visualization pipeline (it is a source process object); it produces data
		// // (output type is vtkPolyData) which other filters may process.
		// //
		//vtkConeSource* cone = vtkConeSource::New();
		//cone->SetHeight(3.0);
		//cone->SetRadius(1.0);
		//cone->SetResolution(10);

		////
		//// In this example we terminate the pipeline with a mapper process object.
		//// (Intermediate filters such as vtkShrinkPolyData could be inserted in
		//// between the source and the mapper.)  We create an instance of
		//// vtkPolyDataMapper to map the polygonal data into graphics primitives. We
		//// connect the output of the cone source to the input of this mapper.
		////
		//vtkPolyDataMapper* coneMapper = vtkPolyDataMapper::New();
		//coneMapper->SetInputConnection(cone->GetOutputPort());

		////
		//// Create an actor to represent the cone. The actor orchestrates rendering
		//// of the mapper's graphics primitives. An actor also refers to properties
		//// via a vtkProperty instance, and includes an internal transformation
		//// matrix. We set this actor's mapper to be coneMapper which we created
		//// above.
		////
		//vtkActor* coneActor = vtkActor::New();
		//coneActor->SetMapper(coneMapper);

		////
		//// Create the Renderer and assign actors to it. A renderer is like a
		//// viewport. It is part or all of a window on the screen and it is
		//// responsible for drawing the actors it has.  We also set the background
		//// color here.
		////
		//vtkRenderer* ren1 = vtkRenderer::New();
		//ren1->AddActor(coneActor);
		//ren1->SetBackground(0.1, 0.2, 0.4);

		////
		//// Finally we create the render window which will show up on the screen.
		//// We put our renderer into the render window using AddRenderer. We also
		//// set the size to be 300 pixels by 300.
		////
		//vtkGenericOpenGLRenderWindow* renWin = vtkGenericOpenGLRenderWindow::New();
		//renWin->AddRenderer(ren1);
		//renWin->SetSize(300, 300);

		////
		//// Now we loop over 360 degrees and render the cone each time.
		////
		//int i;
		//for (i = 0; i < 360; ++i)
		//{
		//	// render the image
		//	renWin->Render();
		//	// rotate the active camera by one degree
		//	ren1->GetActiveCamera()->Azimuth(1);
		//}

		////
		//// Free up any objects we created. All instances in VTK are deleted by
		//// using the Delete() method.
		////
		//cone->Delete();
		//coneMapper->Delete();
		//coneActor->Delete();
		//ren1->Delete();
		//renWin->Delete();
		//// VTK/Qt wedded


		// Set up action signals and slots





	//	{
	//	QString txt = "BLABLABLA";
	//	//QByteArray bytes=txt;
	//	QCryptographicHash *hash = new QCryptographicHash(QCryptographicHash::Md5);
	//	hash->addData(txt.toLocal8Bit());
	//	QByteArray result = hash->result();
	//	QString newTxt = QString::fromLatin1(result.data()).toLocal8Bit();
	//
	//	byte *testx=(byte*)(result.data());
	//	QFile ff("d:\\sss");
	//	ff.open(QIODevice::WriteOnly);
	//	for(int i=0;i<newTxt.length();i++)
	//	{
	//		QTextStream stream( &ff );
	//		stream << (int)testx[i] <<",";	
	//	}
	//	ff.close();
	//}
	//
	//53,199,129,97,74,148,248,246,86,121,88,195,213,28,18,131









	char masterTab[] = { 53,199,129,97,74,148,248,246,86,121,88,195,213,28,18,131,'\0' };

	QString masterString = (const char*)masterTab;//"BLABLABLA";



	//MOJ C860008C4426

	//DOROTA 28C2DD5713C3
	int modex = 0;
	QString mac = getMacAddress();
	//mac = "309C23436FCB";//ewelina nowy komp
	//mac = "E4E749519B54";
	//mac = "80FA5B673F56";//ewelina1
//	mac = "185680E5C838";//ewelina2
	//mac = "107B441A1F09";//big -ewelina
	//mac = "00D861FA7209";//ciucias - duzy komp profa
	//mac = "F49634A0E88F";//kuba
	//mac = "309C23141196";//kuba2
	//mac = "02004C4F4F50";
	//qWarning()<<mac;
	if (mac.compare(QString("C860008C4426")) == 0)
	{
		modex = 1;
	}
	if (mac.compare(QString("28C2DD5713C2")) == 0)
	{
		modex = 2;//dorota
	}


	//mac = "C860008C4426"; 
	QString txt = "RibnXtr" + mac + "RibnXtr";
	//QByteArray bytes=txt;
	QCryptographicHash* hash = new QCryptographicHash(QCryptographicHash::Md5);
	hash->addData(txt.toLocal8Bit());
	QByteArray result = hash->result();
	QString newTxt = QString::fromLatin1(result.data()).toLocal8Bit();

	//if (newTxt.compare(QString("C860008C4426")) == 0)
	//{
	   // modex = 1;
	//}
	//if (newTxt.compare(QString("?`ďż˝Ra?Zxďż˝")) == 0)
	//{
	   // modex = 2;//dorota
	//}




   //byte *testx=(byte*)(result.data());
   //QFile ff("sss");
   //ff.open(QIODevice::WriteOnly);
   //for(int i=0;i<newTxt.length();i++)
   //{
   //	QTextStream stream( &ff );
   //	stream << (int)testx[i] <<",";	
   //}
   //ff.close();

	delete hash;
	//for(int i=0;i<1000;i++)cout << "Aaaaaaaaaaaaaaa";
	QFile file(".code");
	QFileInfo fileInfo(file.fileName());

	QString t = fileInfo.absoluteFilePath();

	QString filename2(fileInfo.absoluteFilePath());
	//qDebug() << filename2;
	//std::cout <<"txt1";
	bool test = false;

	if (file.open(QIODevice::ReadOnly))
	{
		QString pass = file.readAll();

		if (newTxt.compare(pass) == 0)//haslo sie zgadza

		{
			test = true;

		}
		else   //if (masterString.compare(pass)==0) //daj nowe haslo i zapisz
		{


			// QByteArray bytes2=pass;
			QCryptographicHash* hash2 = new QCryptographicHash(QCryptographicHash::Md5);
			hash2->addData(pass.toLocal8Bit());
			QByteArray result2 = hash2->result();
			delete hash2;
			QString newTxt2 = result2.data();

			if (newTxt2.compare(masterString) == 0)
			{


				file.close();
				// const char *taba =;
				file.open(QIODevice::WriteOnly);

				//zapisz nowe
				//QTextStream out(&file);
				//out << newTxt;
				file.write(newTxt.toUtf8());
				test = true;
			}

		}

		file.close();
	}

	if (test == false) return 0;
	//auto format = QVTKOpenGLWidget::defaultFormat();
	//format.setProfile(QSurfaceFormat::CompatibilityProfile);
	//QSurfaceFormat::setDefaultFormat(format);





	//qputenv("QT_OPENGL", "angle");
	//QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);







	//UTF-8
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));//Windows - 1250"));
	/* QGLFormat glf = QGLFormat::defaultFormat();
	   glf.setSampleBuffers(true);
	   glf.setSamples(2);
	   QGLFormat::setDefaultFormat(glf);*/
	   //QStyle *WS=new QWindowsStyle();//QCleanlooksStyle ();

		   //QPlastiqueStyle();//
	   //app.setStyle(WS);
	   // app.setStyleSheet("QSplitter::handle { background-color: black }");
	   // app.setStyleSheet("QSplitter::

	   //	QFont f( "Helvetica", 8, QFont::Light );
		//   f.setStyleHint( QFont::Helvetica );
		//   mama.setFont( f );






	app.setStyle(QStyleFactory::create("Fusion"));
	QPalette palette;
	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Base, QColor(15, 15, 15));
	palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	palette.setColor(QPalette::ToolTipBase, Qt::white);
	palette.setColor(QPalette::ToolTipText, Qt::white);
	palette.setColor(QPalette::Text, Qt::white);
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::BrightText, Qt::red);
	//pallette.setColor(QPalette::Hover, Qt::red);
	palette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
	palette.setColor(QPalette::HighlightedText, Qt::black);
	palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
	palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
	app.setPalette(palette);



	MainWindow pWindow(modex);
	app.setActiveWindow(&pWindow);




	//w.show();

	QString filename;
	int nrOfArgs = argc;
	if (nrOfArgs > 1)
	{
		int i;
		for (i = 1;i < nrOfArgs;i++)
		{
			filename = argv[i];
			pWindow.SlotLoadUniversalData(filename);
			//  if ( !filename.endsWith( ".cht" ) )
			  //    filename = QString::null;
		}
	}

	//QString style1(" QSplitter::handle {bacground: black};");
	//app.setStyleSheet(style1);
	//app.setStyleSheet(QString("QWidget {background: darkgray; color: white;}"));
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));


	//--------------------------------------


	return app.exec();

}

QString getMacAddress()
{
	QString macaddress = "??:??:??:??:??:??";
	//#ifdef Q_WS_WIN
	PIP_ADAPTER_INFO pinfo = NULL;

	unsigned long len = 0;
	unsigned long nError;

	if (pinfo != NULL)
		delete (pinfo);

	nError = GetAdaptersInfo(pinfo, &len);	//Have to do it 2 times?

	if (nError != 0)
	{
		pinfo = (PIP_ADAPTER_INFO)malloc(len);
		nError = GetAdaptersInfo(pinfo, &len);
	}

	if (nError == 0)
		macaddress.sprintf("%02X%02X%02X%02X%02X%02X", pinfo->Address[0], pinfo->Address[1], pinfo->Address[2], pinfo->Address[3], pinfo->Address[4], pinfo->Address[5]);
	//#endif
	return macaddress;
}