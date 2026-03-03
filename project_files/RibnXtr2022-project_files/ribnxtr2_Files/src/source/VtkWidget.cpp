#include "VtkWidget.h"
//Added by qt3to4:
#include <QResizeEvent>
//#include <QVBoxLayout>


//------------------------
VtkWidget::VtkWidget(QWidget* parent, QString name, Qt::WindowFlags flags) :RenderWindowInteractor(parent)//MyDockWidget(name,parent, flags)//,QToolTip()
{
	lastTextActor = NULL;
	vtkMatrix4x4 *ma= vtkMatrix4x4::New();
	ma->Identity();
	double *m=ma->GetData();
	for (int i = 0;i < 16;i++)
	{
		baseMatrix[i] = m[i];
	}

	bckCol[0] = 1;
	bckCol[1] = 1;
	bckCol[2] = 1;
	frontCol[0] = 0;
	frontCol[1] = 0;
	frontCol[2] = 0;
	skyBoxName = "";
	backgroundMode = 1;
	defaultPropertyFilename = "";
	skyBox = false;
	installEventFilter(this);
	trickMode = false;
	logoSizeX = 100;

	//this->setAttribute(Qt::WA_OpaquePaintEvent, true);
	//this->setWakeUpDelay(500);
	resetCameraMode = true;
	refreshMode = true;
	QWidget* widget = this;//=new QWidget(this);
	//this->setWidget(widget);

	//QVBoxLayout* VLayout = new QVBoxLayout(widget);
	//VLayout->setSpacing(0);
	//VLayout->setMargin(0);
	//widget->setLayout(VLayout);

	//	RenderWindow *=new RenderWindow(this);
	//Interactor=new VtkInteractorWidget(this);
	//Interactor= new RenderWindowInteractor(widget);

	//glf.setSampleBuffers(true);
   // glf.setSamples(4);

   //QGLFormat glFormat;                            // 1
   // glFormat.setVersion( 4, 2 );                   // 2
   // glFormat.setProfile( QGLFormat::CoreProfile ); // 3
   // glFormat.setAlpha(true);
 //   CoreGLWidget w( glFormat );      


	Interactor = this;//new RenderWindowInteractor(widget);



	//connect(Interactor, SIGNAL(signalLeftDoubleClicked(int, int)), this, SIGNAL(signalLeftDoubleClicked(int, int)));
	//connect(Interactor, SIGNAL(signalMouseMoved(int, int)), this, SIGNAL(signalMouseMoved(int, int)));
	//connect(Interactor, SIGNAL(signalMouseMoved(int, int, int)), this, SIGNAL(signalMouseMoved(int, int, int)));
	//connect(Interactor,SIGNAL(signalRightWithCTRLClicked(int , int )),this,SIGNAL(signalRightWithCTRLClicked(int , int  )));

//	connect(Interactor, SIGNAL(SendCurrentCameraRotation(double, double, double)), this, SIGNAL(SendCurrentCameraRotation(double, double, double)));

	//	Interactor->SetLeftButtonFunction( SLOT(Interactor->CameraRotate()) );
	//	Interactor->SetMiddleButtonFunction( SLOT(Interactor->CameraTranslate()) );
	//	Interactor->SetRightButtonFunction( SLOT(Interactor->CameraZoom()) );
	//	Interactor->SetShiftLeftButtonFunction( SLOT(Interactor->CameraRoll()) );



	ProjectionMode = 0;//Unparalell

	resetCameraDistance = 0;



	//VLayout->addWidget(Interactor);
	Renderer = Interactor->GetRenderer();


	//SlotChangeBackgroundColor(255,255,255);
	this->setBackgroundColor(bckCol);
	this->setGradientBackgroundColor1(1.0, 1.0, 1.0);
	this->setGradientBackgroundColor2(0.5, 0.5, 0.5);






	//Interactor->GetRenderWindow()->LineSmoothingOn();
	//Interactor->GetRenderWindow()->SetAAFrames(16);


	//connect (Interactor,SIGNAL(mouseMoved()),this,SLOT(mouseMoved()));

	connect(Interactor, SIGNAL(updatedView()), this, SLOT(SlotUpdateCube()));

	for (int i = 0;i < 6;i++)
	{
		skyboxActor[i] = vtkSmartPointer<vtkActor>::New();
		skyboxActor[i]->SetVisibility(0);
		Renderer->AddActor(skyboxActor[i]);
		skyBoxModel = 0;
	}

	//Renderer->GetActiveCamera()->SetViewAngle(40);
	//Renderer->GetActiveCamera()->SetEyeAngle(5.0);
	this->setViewAngle(40.0);
	this->setEyeAngle(5.0);
	this->Renderer->SetNearClippingPlaneTolerance(0.0001);




	backgroundRenderer = vtkSmartPointer<vtkRenderer>::New();
	backgroundRendererLogo = vtkSmartPointer<vtkRenderer>::New();
	//Interactor->GetRenderWindow()->AddRenderer(backgroundRendererLogo);
	vtkSmartPointer<vtkJPEGReader> jpegReader = vtkSmartPointer<vtkJPEGReader>::New();
	QDir Dir;
	QString name2 = Dir.absolutePath() + "/sky/" + "sushi.jpg";
	jpegReader->SetFileName(name2.toLocal8Bit());
	jpegReader->Update();
	imageData = jpegReader->GetOutput();



	// imageMapper = vtkSmartPointer<vtkImageMapper>::New();
	//imageMapper->SetInputData(imageData);

	imageActor = vtkSmartPointer<vtkImageActor>::New();

	imageActor->SetInputData(imageData);

	/*imageActor->SetMapper(imageMapper);
	imageActor->SetPosition(0, 0);
	imageActor->SetPosition2(width() , height());
	imageActor->GetProperty()->SetOpacity(1);*/

	//	 imageMapper->SetColorWindow(255);
	//	 imageMapper->SetColorLevel(127.5);




	backgroundRenderer->InteractiveOff();
	backgroundRenderer->AddActor(imageActor);




	backgroundRenderer->GetActiveCamera()->ParallelProjectionOn();
	backgroundRenderer->SetBackground(bckCol);



	//backgroundRendererLogo->InteractiveOff();
	//backgroundRendererLogo->AddActor(imageActor);
	//backgroundRendererLogo->GetActiveCamera()->ParallelProjectionOn();
	//backgroundRendererLogo->SetBackground(bckCol);


	GenerateCubeSource();

	//	SlotSetCubeVisibility(false);

	Interactor->show();
	SlotResetCamera();


	cubeActor2D->GetProperty()->SetColor(frontCol);

	Interactor->GetRenderWindow()->SetNumberOfLayers(1);
	backgroundRenderer->SetLayer(0);
	Renderer->SetLayer(0);
	//
	bckgRenderer = false;

	//vtkGPUInfoList *l = vtkGPUInfoList::New();
	//l->Probe();
	//int c = l->GetNumberOfGPUs();

	//cout << "There is " << c << " GPU(s)." << endl;
	//int i = 0;
	//while (i<c)
	//{
	//	qDebug() << " GPU " << i << ": " << endl;
	//	vtkGPUInfo *info = l->GetGPUInfo(i);
	//	vtkTypeUInt64 value;
	//	value = info->GetDedicatedVideoMemory();
	//	qDebug() << "  dedicated VRAM=" << value / (1024 * 1024) << " MiB" << endl;
	//	value = info->GetDedicatedSystemMemory();
	//	qDebug() << "  dedicated RAM=" << value / (1024 * 1024) << " MiB" << endl;
	//	value = info->GetSharedSystemMemory();
	//	qDebug() << "  shared RAM=" << value / (1024 * 1024) << " MiB" << endl;
	//	++i;
	//}
	//l->Delete();

	//std::string msg = vtksys::SystemInformation::GetProgramStack(0, 0);
	textActor =vtkTextActor::New();



	//int size = vtkTextureObject::GetMaximumTextureSize(Renderer->GetRenderWindow());

}
//--------------------------------
void	VtkWidget::setTextActor(QString txt, int pos[2], int fontSize, double color[3])
{
	textActor->SetInput(txt.toLocal8Bit());
	int h = height();
	textActor->SetPosition(pos[0],pos[1]);
	textActor->GetTextProperty()->SetFontSize(fontSize);
	textActor->GetTextProperty()->SetColor(color);
	Renderer->AddActor2D(textActor);
}
//--------------------------------
void	VtkWidget::hideTextActor()
{
	Renderer->RemoveActor2D(textActor);
}

//---------------------
void	VtkWidget::AddTextActor(vtkTextActor* actor)
{
	lastTextActor = actor;
	if (actor!=NULL)
	Renderer->AddActor2D(actor);

}
void	VtkWidget::RemoveTextActor(vtkTextActor* actor)
{
	if (actor!=NULL)
	Renderer->RemoveActor2D(actor);

	lastTextActor = NULL;
}
//----------------------
void	VtkWidget::SlotSetSkyboxVisibility(bool val)
{
	skyBox = val;
	if (val == true)
	{
		(skyBoxName);

	}
	else
	{
		for (int i = 0;i < 6;i++)
		{
			/*skyboxActor[i]=vtkSmartPointer<vtkActor>::New();*/
			skyboxActor[i]->SetVisibility(0);
			//Renderer->AddActor(skyboxActor[i]);

		}
	}
	this->SlotResetCamera();
}
//---------------------
void	VtkWidget::SetCurrentSkyboxModel(QString skyName)
{
	//if (nr>5) return ;
	int v = 32768;
	QString names[6] = { "negx","posx","negz","posz","negy","posy" };
	double tab[] = { -v,-v,-v,	-v,v,-v,	-v,-v,v,	-1,0,0, //-x
							v,v,-v,			v,-v,-v,	v,v,v,	1,0,0,	//+x
							v,-v,-v,		-v,-v,-v,v,-v,v,	0,-1,0, //-y

							-v,v,-v,				v,v,-v,	-v,v,v,	0,1,0, //+y

							-v,v,-v,			v,v,-v,-v,-v,-v,	0,0,-1, //-z	

							-v,v,v,				v,v,v,-v,-v,v,	0,0,1, //+z

	};

	int pos = 0;
	//int nr=nr;
	//double *tab1=tab;
	for (int i = 0;i < 6;i++)
	{
		vtkSmartPointer<vtkPlaneSource > aPlane = vtkSmartPointer<vtkPlaneSource>::New();



		aPlane->SetOrigin(tab[pos], tab[pos + 1], tab[pos + 2]);
		aPlane->SetPoint1(tab[pos + 3], tab[pos + 4], tab[pos + 5]);
		aPlane->SetPoint2(tab[pos + 6], tab[pos + 7], tab[pos + 8]);
		aPlane->SetNormal(tab[pos + 9], tab[pos + 10], tab[pos + 11]);


		vtkSmartPointer<vtkTextureMapToPlane> pl = vtkSmartPointer<vtkTextureMapToPlane>::New();
		//pl->SetInputData(aPlane->GetOutput());
		pl->SetInputConnection(aPlane->GetOutputPort());
		pl->SetOrigin(tab[pos], tab[pos + 1], tab[pos + 2]);
		pl->SetPoint1(tab[pos + 3], tab[pos + 4], tab[pos + 5]);
		pl->SetPoint2(tab[pos + 6], tab[pos + 7], tab[pos + 8]);
		pl->SetNormal(tab[pos + 9], tab[pos + 10], tab[pos + 11]);

		pos += 12;
		pl->AutomaticPlaneGenerationOn();
		pl->Update();
		vtkSmartPointer<vtkPolyDataMapper >mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

		mapper->SetInputConnection(pl->GetOutputPort());
		mapper->Update();


		QDir Dir;
		QString name2 = Dir.absolutePath() + "/sky/" + skyName + "/" + names[i] + ".jpg";
		//nname.replace("/", "\\\\");
		QFile file(name2);
		if (file.exists())
		{

			vtkSmartPointer<vtkJPEGReader >imageIn = vtkSmartPointer<vtkJPEGReader>::New();


			imageIn->SetFileName(name2.toLocal8Bit());

			skyBoxName = skyName;
			imageIn->Modified();
			imageIn->Update();


			vtkSmartPointer<vtkTexture >texture = vtkSmartPointer<vtkTexture>::New();
			//texture->SetInputData(imageIn->GetOutput());
			texture->SetInputConnection(imageIn->GetOutputPort());
			texture->EdgeClampOn();
			texture->RepeatOff();
			texture->InterpolateOn();
			texture->PremultipliedAlphaOn();





			skyboxActor[i]->SetMapper(mapper);
			skyboxActor[i]->SetTexture(texture);
			skyboxActor[i]->GetProperty()->SetLighting(false);
			skyboxActor[i]->SetVisibility(1);
			//skyboxActor[i]->GetProperty()->SetOpacity(0.5);

		}


	}
	skyBox = true;
}
//--------------
void	VtkWidget::SlotSetCubeVisibility(bool v)
{
	cubeActor2D->SetVisibility(v);
}
void VtkWidget::SlotUpdateCube()
{


	//bool vis = cubeActor2D->GetVisibility();

	//if (vis == true)
	//{


	//	vtkCamera* cam = Renderer->GetActiveCamera();


	//	double p1[3], p2[3], p3[3];

	//	cam->GetPosition(p1);
	//	cam->GetViewUp(p2);
	//	cam->GetFocalPoint(p3);


	//	vtkCamera* cam2 = backgroundRendererLogo->GetActiveCamera();

	//	cam2->SetPosition(p1);
	//	cam2->SetViewUp(p2);
	//	cam2->SetFocalPoint(p3);

	//	//	this->backgroundRendererLogo->GetActiveCamera()->SetUserTransform(Renderer->GetActiveCamera()->GetUserTransform());
	//	//	backgroundRendererLogo->re

	//	int i;
	//	double p[3];


	//	//rzutuje na wsp ekranowe

	//	//if (this->ProjectionMode==true)
	//	//{
	//	//		Renderer->GetActiveCamera()->ParallelProjectionOff();
	//	////	Renderer->GetActiveCamera()->ParallelProjectionOn();
	//	////	Renderer->GetActiveCamera()->SetParallelScale(40);
	//	//}
	//	/*else
	//	{
	//	Renderer->GetActiveCamera()->ParallelProjectionOff();
	//	}*/

	//	//	this->SlotResetCube();
	//	/*	double bounds[6];
	//	cubePolys->GetBounds(bounds);
	//	double bounds2[6];
	//	double pl[24];
	//	double aspect=(double)this->width()/this->height();
	//	Renderer->GetActiveCamera()->GetFrustumPlanes(aspect,pl);
	//	vtkPlanes *planes=vtkPlanes::New();
	//	planes->SetFrustumPlanes(pl);

	//	vtkClipPolyData *clip=vtkClipPolyData::New();
	//	clip->SetInputData(cubePolys);
	//	clip->SetClipFunction(planes);
	//	clip->GetOutput()->Update();
	//	clip->GetOutput()->GetBounds(bounds2);

	//	if (fabs(	(bounds[0]*bounds[1]*bounds[2]*bounds[3]*bounds[4]*bounds[5])-(bounds2[0]*bounds2[1]*bounds2[2]*bounds2[3]*bounds2[4]*bounds2[5]))>1e-4)
	//	{
	//	i=i;
	//	}
	//	clip->Delete();
	//	planes->Delete();*/


	//	trans->SetInputData(cubePolys);
	//	trans->Update();
	//	cubePolys2D->DeepCopy(trans->GetOutput());
	//	//		cubePolys2D->Update();

	//			/*if (this->ProjectionMode==true)
	//			{
	//			Renderer->GetActiveCamera()->ParallelProjectionOff();
	//			Renderer->GetActiveCamera()->ParallelProjectionOn();
	//			Renderer->GetActiveCamera()->SetParallelScale(40);
	//			}*/

	//			//double min[2]={INT_MAX, INT_MAX};
	//			//double max[2]={-INT_MAX, -INT_MAX};

	//			//vtkPoints *srcPts=cubePolys->GetPoints();
	//			//vtkPoints *dstPts=cubePolys2D->GetPoints();
	//			//int nrPts=srcPts->GetNumberOfPoints();

	//			////szukam granic
	//			//for(i=0;i<nrPts;i++)
	//			//{
	//			//	dstPts->GetPoint(i,p);
	//			//	if  (p[0]>max[0]) max[0]=p[0];
	//			//	if  (p[0]<min[0]) min[0]=p[0];
	//			//	if  (p[1]>max[1]) max[1]=p[1];
	//			//	if  (p[1]<min[1]) min[1]=p[1];
	//			//}
	//			//// przeskaluje zeby sie miescilo w actor2d okienku

	//			//double tol=1e-5;
	//			//double f[2];
	//			//double lP1[2],lP2[2];
	//			//double d=20.0;
	//			//lP1[0]=logoP1[0]+d;lP1[1]=logoP1[1]+d;
	//			//lP2[0]=logoP2[0]-d;lP2[1]=logoP2[1]-d;
	//			//f[0]=max[0]-min[0];	if (fabs(f[0])<tol) f[0]=1e-5;	f[0]=(lP2[0]-lP1[0])/f[0];
	//			//f[1]=max[1]-min[1];	if (fabs(f[1])<tol) f[1]=1e-5;	f[1]=(lP2[1]-lP1[1])/f[1];


	//			////	if (f[0]>f[1]) f[1]=f[0]; else f[0]=f[1];

	//			//for(i=0;i<nrPts;i++)
	//			//{
	//			//	dstPts->GetPoint(i,p);
	//			//	p[0]= (p[0]-min[0])*f[0]+lP1[0];
	//			//	p[1]= (p[1]-min[1])*f[1]+lP1[1];
	//			//	dstPts->SetPoint(i,p);
	//			//}

	//	//		cubePolys2D->Update();



	//	mapper->Update();

	//}
bool vis = cubeActor2D->GetVisibility();

if (vis == true)
{

	int i;
	double p[3];


	//rzutuje na wsp ekranowe

	//if (this->ProjectionMode==true)
	//{
	//		Renderer->GetActiveCamera()->ParallelProjectionOff();
	////	Renderer->GetActiveCamera()->ParallelProjectionOn();
	////	Renderer->GetActiveCamera()->SetParallelScale(40);
	//}
	/*else
	{
	Renderer->GetActiveCamera()->ParallelProjectionOff();
	}*/

	//	this->SlotResetCube();
	/*	double bounds[6];
	cubePolys->GetBounds(bounds);
	double bounds2[6];
	double pl[24];
	double aspect=(double)this->width()/this->height();
	Renderer->GetActiveCamera()->GetFrustumPlanes(aspect,pl);
	vtkPlanes *planes=vtkPlanes::New();
	planes->SetFrustumPlanes(pl);

	vtkClipPolyData *clip=vtkClipPolyData::New();
	clip->SetInputData(cubePolys);
	clip->SetClipFunction(planes);
	clip->GetOutput()->Update();
	clip->GetOutput()->GetBounds(bounds2);

	if (fabs(	(bounds[0]*bounds[1]*bounds[2]*bounds[3]*bounds[4]*bounds[5])-(bounds2[0]*bounds2[1]*bounds2[2]*bounds2[3]*bounds2[4]*bounds2[5]))>1e-4)
	{
	i=i;
	}
	clip->Delete();
	planes->Delete();*/



	//vtkCamera* logoCam = backgroundRendererLogo->GetActiveCamera();
	vtkCamera* cam = Renderer->GetActiveCamera();
	double isNoParallel = cam->GetParallelProjection();
	if (isNoParallel == false) cam->ParallelProjectionOn();
	//logoCam->DeepCopy(cam);

	double posX[3],focal[3];
	cam->GetPosition(posX);
	cam->GetFocalPoint(focal);

	double newPos[3];

	double diff[3];
	for (int j = 0;j < 3;j++)
	{
		diff[j] = focal[j]- posX[j] ;
	}
	normV(diff);
	for (int j = 0;j < 3;j++)
	{
		newPos[j] = focal[j]-diff[j]* resetCameraDistance;
	}

	//resetCameraDistance
	//qDebug() << "camPOs: " << posX[0] << " " << posX[1] << " " << posX[2];

	cam->SetPosition(newPos);
	trans->SetInputData(cubePolys);
	
	trans->Update();
	
	cubePolys2D->DeepCopy(trans->GetOutput());
	cam->SetPosition(posX);
	if (isNoParallel == false)cam->ParallelProjectionOff();
	//		cubePolys2D->Update();

			/*if (this->ProjectionMode==true)
			{
			Renderer->GetActiveCamera()->ParallelProjectionOff();
			Renderer->GetActiveCamera()->ParallelProjectionOn();
			Renderer->GetActiveCamera()->SetParallelScale(40);
			}*/

	double min[2] = { INT_MAX, INT_MAX };
	double max[2] = { -INT_MAX, -INT_MAX };

	vtkPoints* srcPts = cubePolys->GetPoints();
	vtkPoints* dstPts = cubePolys2D->GetPoints();
	int nrPts = srcPts->GetNumberOfPoints();

	//szukam granic
	for (i = 0;i < nrPts;i++)
	{
		dstPts->GetPoint(i, p);
		if (p[0] > max[0]) max[0] = p[0];
		if (p[0] < min[0]) min[0] = p[0];
		if (p[1] > max[1]) max[1] = p[1];
		if (p[1] < min[1]) min[1] = p[1];
	}
	// przeskaluje zeby sie miescilo w actor2d okienku

	double tol = 1e-5;
	double f[2];
	double lP1[2], lP2[2];
	double d = 20.0;
	lP1[0] = logoP1[0] + d;lP1[1] = logoP1[1] + d;
	lP2[0] = logoP2[0] - d;lP2[1] = logoP2[1] - d;
	f[0] = max[0] - min[0];	if (fabs(f[0]) < tol) f[0] = 1e-5;	f[0] = (lP2[0] - lP1[0]) / f[0];
	f[1] = max[1] - min[1];	if (fabs(f[1]) < tol) f[1] = 1e-5;	f[1] = (lP2[1] - lP1[1]) / f[1];


	//	if (f[0]>f[1]) f[1]=f[0]; else f[0]=f[1];

	for (i = 0;i < nrPts;i++)
	{
		dstPts->GetPoint(i, p);
		p[0] = (p[0] - min[0]) * f[0] + lP1[0];
		p[1] = (p[1] - min[1]) * f[1] + lP1[1];
		dstPts->SetPoint(i, p);
	}

	//		cubePolys2D->Update();

//	vtkTransform* trCam = Renderer->GetActiveCamera()->GetModelViewTransformObject();
//
//
//
//	double pp[4], pp1[4];
//	double ptsX[8][3];
//	vtkSmartPointer<vtkPoints> ptsTmp = vtkSmartPointer<vtkPoints>::New();
//	for (int i = 0;i < 8;i++)
//	{
//		cubePolys->GetPoint(i, pp);
//		trCam->MultiplyPoint(pp, pp1);
//		ptsTmp->InsertNextPoint(pp1);
//		for (int j = 0;j < 3;j++)
//		{
//			ptsX[i][j] = pp1[j];
//		}
//	}
//
//	
//	vtkSmartPointer<vtkHexahedron> hex =
//		vtkSmartPointer<vtkHexahedron>::New();
//	hex->GetPointIds()->SetId(0, 0);
//	hex->GetPointIds()->SetId(1, 1);
//	hex->GetPointIds()->SetId(2, 2);
//	hex->GetPointIds()->SetId(3, 3);
//	hex->GetPointIds()->SetId(4, 4);
//	hex->GetPointIds()->SetId(5, 7);
//	hex->GetPointIds()->SetId(6, 6);
//	hex->GetPointIds()->SetId(7, 7);
//
//
//
//	// Add the hexahedron to a cell array
//	vtkSmartPointer<vtkCellArray> hexs =
//		vtkSmartPointer<vtkCellArray>::New();
//	hexs->InsertNextCell(hex);
//
//
//	vtkSmartPointer<vtkUnstructuredGrid> uGrid =
//		vtkSmartPointer<vtkUnstructuredGrid>::New();
//	uGrid->SetPoints(ptsTmp);
//	uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
//
//	
//
//
//
//
//	//output->SetLines(skeletLine);
//
//
////	vtkSmartPointer<vtkMeshQuality> qualityFilter =		vtkSmartPointer<vtkMeshQuality>::New();
//
////	qualityFilter->SetInput(uGrid);
//
//
//	double vol = vtkMeshQuality::HexVolume(uGrid->GetCell(0));
//
//	qDebug() << "cubevol: " << vol;

	mapper->Update();

}
}
//---------
void VtkWidget::SlotChangeBackgroundColor(int R, int G, int B)
{
	Renderer->GradientBackgroundOn();
	Renderer->SetBackground((double)(R / 255.0), (double)(G / 255.0), (double)(B / 255.0));

	R /= 2;B /= 2;G /= 2;
	Renderer->SetBackground2((double)(R / 255.0), (double)(G / 255.0), (double)(B / 255.0));

	//	Renderer->Update();
	//repaint();
}

//-------------------------------------------------------------------

void VtkWidget::RepaintWidget()
{

	if (refreshMode == true) Interactor->repaint();

	//Interactor->SlotResizeRenderWindow(this->width(), this->height());


}

//---------------------------------------------

void VtkWidget::SlotSetInteractorModeToTrackball()
{

	/*
	Interactor->SetButton1Function(vtkQGLInteractorWidget::CameraMode,
	SLOT(TrackballRotateCamera()));
	Interactor->SetButton2Function(vtkQGLInteractorWidget::CameraMode,
	SLOT(TrackballPanCamera()));
	Interactor->SetButton3Function(vtkQGLInteractorWidget::CameraMode,
	SLOT(TrackballDollyCamera()));
	Interactor->SetButton1Qt::CTRLFunction(vtkQGLInteractorWidget::CameraMode,
	SLOT(TrackballSpinCamera()));
	Interactor->SetButton1Function(vtkQGLInteractorWidget::ActorMode,
	SLOT(TrackballRotateActor()));
	Interactor->SetButton2Function(vtkQGLInteractorWidget::ActorMode,
	SLOT(TrackballPanActor()));
	Interactor->SetButton3Function(vtkQGLInteractorWidget::ActorMode,
	SLOT(TrackballDollyActor()));
	Interactor->SetButton1Qt::CTRLFunction(vtkQGLInteractorWidget::ActorMode,
	SLOT(TrackballSpinActor()));
	Interactor->SetButton3Qt::CTRLFunction(vtkQGLInteractorWidget::ActorMode,
	SLOT(TrackballScaleActor()));

	*/


	//  vtkInteractorStyleFlight *Style=vtkInteractorStyleFlight::New();
	//	vtkInteractorStyleTrackballCamera *Style=vtkInteractorStyleTrackballCamera::New();
	// Style->SetMotionStepSize(0.01);
	// Style->SetAngleAccelerationFactor(1.0);
	//Style->SetMotionAccelerationFactor(1);
	//	Interactor->GetInteractor()->SetInteractorStyle(Style);
}

//----------------------------------------------

void VtkWidget::SlotSetInteractorModeToJoystick()
{
	/*
	Interactor->SetButton1Function(vtkQGLInteractorWidget::CameraMode,
	SLOT(JoystickRotateCamera()));
	Interactor->SetButton2Function(vtkQGLInteractorWidget::CameraMode,
	SLOT(JoystickPanCamera()));
	Interactor->SetButton3Function(vtkQGLInteractorWidget::CameraMode,
	SLOT(JoystickDollyCamera()));
	Interactor->SetButton1Qt::CTRLFunction(vtkQGLInteractorWidget::CameraMode,
	SLOT(JoystickSpinCamera()));
	Interactor->SetButton1Function(vtkQGLInteractorWidget::ActorMode,
	SLOT(JoystickRotateActor()));
	Interactor->SetButton2Function(vtkQGLInteractorWidget::ActorMode,
	SLOT(JoystickPanActor()));
	Interactor->SetButton3Function(vtkQGLInteractorWidget::ActorMode,
	SLOT(JoystickDollyActor()));
	Interactor->SetButton1Qt::CTRLFunction(vtkQGLInteractorWidget::ActorMode,
	SLOT(JoystickSpinActor()));
	Interactor->SetButton3Qt::CTRLFunction(vtkQGLInteractorWidget::ActorMode,
	SLOT(JoystickScaleActor()));
	*/

	//	vtkInteractorStyleJoystickCamera *Style=vtkInteractorStyleJoystickCamera::New();
	//	Interactor->GetInteractor()->SetInteractorStyle(Style);
}


//----------------------------------------------

void VtkWidget::SlotSwitchProjectionModes()
{

	ProjectionMode = !ProjectionMode;
	if (ProjectionMode)
	{
		Renderer->GetActiveCamera()->ParallelProjectionOn();
		Renderer->GetActiveCamera()->SetParallelScale(40);
	}
	else
	{
		Renderer->GetActiveCamera()->ParallelProjectionOff();
	}

	SlotResetCamera();
}

//----------------------------------------------
void	 VtkWidget::GetBounds(double* bounds)
{
	int maxx = INT_MAX;
	if (skyBox == true) maxx = 10240;
	bounds[0] = maxx;
	bounds[1] = -maxx;
	bounds[2] = maxx;
	bounds[3] = -maxx;
	bounds[4] = maxx;
	bounds[5] = -maxx;
	double tmpBounds[6];
	double opacity;
	vtkActorCollection* collection = Renderer->GetActors();
	int count = collection->GetNumberOfItems();
	collection->InitTraversal();
	bool any = false;
	int i;
	for (i = 0;i < count;i++)
	{

		vtkActor* actor = collection->GetNextActor();
		opacity = actor->GetProperty()->GetOpacity();
		if ((i < 6)) continue;
		any = true;

		if (opacity > 0)
		{
			actor->GetBounds(tmpBounds);
			if (tmpBounds[0] < bounds[0]) bounds[0] = tmpBounds[0];
			if (tmpBounds[1] > bounds[1]) bounds[1] = tmpBounds[1];
			if (tmpBounds[2] < bounds[2]) bounds[2] = tmpBounds[2];
			if (tmpBounds[3] > bounds[3]) bounds[3] = tmpBounds[3];
			if (tmpBounds[4] < bounds[4]) bounds[4] = tmpBounds[4];
			if (tmpBounds[5] > bounds[5]) bounds[5] = tmpBounds[5];
		}


	}

	vtkVolumeCollection* vCollection = Renderer->GetVolumes();
	count = vCollection->GetNumberOfItems();
	vCollection->InitTraversal();


	for (i = 0;i < count;i++)
	{
		any = true;
		vtkVolume* vol = vCollection->GetNextVolume();
		vol->GetBounds(tmpBounds);
		if (tmpBounds[0] < bounds[0]) bounds[0] = tmpBounds[0];
		if (tmpBounds[1] > bounds[1]) bounds[1] = tmpBounds[1];
		if (tmpBounds[2] < bounds[2]) bounds[2] = tmpBounds[2];
		if (tmpBounds[3] > bounds[3]) bounds[3] = tmpBounds[3];
		if (tmpBounds[4] < bounds[4]) bounds[4] = tmpBounds[4];
		if (tmpBounds[5] > bounds[5]) bounds[5] = tmpBounds[5];
	}

	if (any == false)
	{
		int vm = -100;int vp = 100;
		bounds[0] = vm;bounds[1] = vp;
		bounds[2] = vm;bounds[3] = vp;
		bounds[4] = vm;bounds[5] = vp;
	}

	sizes[0] = bounds[1] - bounds[0];
	sizes[1] = bounds[3] - bounds[2];
	sizes[2] = bounds[5] - bounds[4];


}
//----------------------------------------------

void VtkWidget::SlotResetCube()
{
	double bounds[6];
	//GetBounds(bounds);


	double ssize[3];
	double center[3];
	int i;
	for (i = 0;i < 3;i++)
	{
		ssize[i] = (bounds[i * 2 + 1] - bounds[i * 2]);
		center[i] = bounds[i * 2] + ssize[i] / 2;
	}

	double p[3], fP[3], pIn[4], pOut[4];

	vtkPoints* pts = cubePolys->GetPoints();
	vtkPoints* ptsOrig = cubeSourcePolys->GetPoints();
	for (i = 0;i < pts->GetNumberOfPoints();i++)
	{
		ptsOrig->GetPoint(i, p);

		pIn[0] = p[0];pIn[1] = p[1];pIn[2] = p[2];pIn[3] = 1;
		vtkMatrix4x4::MultiplyPoint(baseMatrix, pIn, pOut);
		p[0] = pOut[0];p[1] = pOut[1];p[2] = pOut[2];

		p[0] *= ssize[0];p[1] *= ssize[1];p[2] *= ssize[2];
		p[0] += center[0];p[1] += center[1];p[2] += center[2];
		pts->SetPoint(i, p);
	}



	Renderer->ViewToWorld();
	if (refreshMode == true) Interactor->repaint();

}
//----------------------------------------------
void VtkWidget::SlotResetCamera()
{
	Interactor->SlotResizeRenderWindow(this->width(), this->height());
	if (resetCameraMode == true)
	{

		//Renderer->ResetCameraClippingRange();
		int i;
		double bounds[6];
		GetBounds(bounds);

		double ssize[3];
		double center[3];
		for (i = 0;i < 3;i++)
		{
			ssize[i] = (bounds[i * 2 + 1] - bounds[i * 2]);
			center[i] = bounds[i * 2] + ssize[i] / 2;
		}

		double p[3], fP[3], pIn[4], pOut[4];;
		//		Renderer->GetActiveCamera()->GetFocalPoint(fP);



		vtkPoints* pts = cubePolys->GetPoints();
		vtkPoints* ptsOrig = cubeSourcePolys->GetPoints();
		for (i = 0;i < pts->GetNumberOfPoints();i++)
		{
			ptsOrig->GetPoint(i, p);

			pIn[0] = p[0];pIn[1] = p[1];pIn[2] = p[2];pIn[3] = 1;
			vtkMatrix4x4::MultiplyPoint(baseMatrix, pIn, pOut);
			p[0] = pOut[0];p[1] = pOut[1];p[2] = pOut[2];

			p[0] *= ssize[0];p[1] *= ssize[1];p[2] *= ssize[2];
			//p[0]-=ssize[0]/2;p[1]-=ssize[1]/2;p[2]-=ssize[2]/2;
			p[0] += center[0];p[1] += center[1];p[2] += center[2];
			pts->SetPoint(i, p);
		}



		//chce zeby sizex przy resecie bylo ilestam - z tego wylicze skale dla x i y


		//s//cale[0]=this->sthis->width()

		//update bounding boxu
		//	double p[3];
		//	vtkCubeSource *src=vtkCubeSource::New();
		//	src->SetBounds(bounds);

		//	vtkExtractEdges *extr=vtkExtractEdges::New();
		//	extr->SetInputData(src->GetOutput());
		//	extr->Update();

		//	//cubePolys=vtkPolyData::New();
		//	//cubePolys->DeepCopy(extr->GetOutput());
		//	//cubePolys->Update();

		//	vtkPolyData *tmpP=extr->GetOutput();
		//
		////	vtkPolyData *tmpP=src->GetOutput();
		//	tm/*pp->Update();*/

		//	vtkPoints *pts=cubePolys->GetPoints();
		//	for(i=0;i<pts->GetNumberOfPoints();i++)
		//	{
		//		tmpP->GetPoint(i,p);
		//		pts->SetPoint(i,p);
		//		

		//	}
		//	
		//	src->Delete();
		//tmpP->Delete();
		Renderer->ResetCamera(bounds);


		double pos[3], focalP[3];
		Renderer->GetActiveCamera()->GetPosition(pos);
		Renderer->GetActiveCamera()->GetFocalPoint(focalP);
		double dd = GetDistance(pos, focalP);


		resetCameraDistance = dd;// Renderer->GetActiveCamera()->GetFocalDistance();

		//	resetCameraDistance = resetCameraDistance;
	}
	Renderer->ViewToWorld();
	if (refreshMode == true) Interactor->repaint();

}


//----------------------------------------------

void VtkWidget::SlotCameraRotation(double Azimuth, double Elevation, double Roll)
{


	Interactor->CameraUserRotate(Azimuth, Elevation, Roll);
}
//---------------------------------
bool VtkWidget::eventFilter(QObject* obj, QEvent* event)
{
	bool okp = lKeyPressed;
	//QKeyEvent *ee=static_cast<QKeyEvent *>(event);
	//
	//if (ee->type()== QEvent::KeyPress)
	//{
	//	lKeyPressed=ee->key()==Qt::Key_L; 
	//}
	//else if (ee->type()== QEvent::KeyRelease)
	//{
	//	lKeyPressed=(ee->key()!=Qt::Key_L); 
	//}

	//
	//
	//if (okp!=lKeyPressed)
	//	qWarning()<<("test "+QString::number(lKeyPressed));

	if (event->type() == QEvent::ToolTip) {                            // Only process tool tip events
		QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);      // Tool tip events come as the type QHelpEvent
		QPoint pos = helpEvent->pos();                                 // Get pos from event (instead of what was passed to maybeTip)

		// Code from maybeTip function. Modified for Qt 4.
		// Rather than tip() use QToolTip::showText() and QToolTip::hideText()
		//
		// For example, convert
		//     tip(r, text);
		// to
		//     QToolTip::showText(helpEvent->globalPos(), text, widget, r);
		//int xx=this->pos().x();
		//int yy=this->pos().y();
		//qWarning ("xx: "+ QString::number(xx)+ " yy: "+ QString::number(yy));	
		QPoint p2(pos.x(), this->height() - pos.y());
		emit signalTolTipActivated(p2);
		event->ignore();

		return true;                                                    // Return true to filter event
	}
	return false;
}
//----------------------
void	VtkWidget::maybeTip(const QPoint& p)
{
	//p.setX(this->height()-p.y());

	QPoint p2(p.x(), this->height() - p.y());
	emit signalTolTipActivated(p2);
};
//---------------------------------
void	VtkWidget::mouseMoved()
{

	if (trickMode == true)
	{
		Interactor->CleanTrickyWindow();
		trickMode = false;
	}

	//	((QToolTip*)(this))->hide();
	disconnect(Interactor, SIGNAL(mouseMoved()), this, SLOT(mouseMoved()));
	Interactor->repaint();
	//((QToolTip*)(this))->show();
}
//-----------
//void	VtkWidget::resizeEvent(QResizeEvent*)
//{
//
//	QWidgetList  list = QApplication::allWidgets();
//	QList<QWidget*>::iterator it(list.begin());         // iterate over the widgets
//
//	QWidget* w;
//	// while ( (w=it.current()) != 0 ) {  // for each widget...
//	//   ++it;
//	for (it = list.begin();it != list.end();++it)
//	{
//		w = *it;
//		w->update();
//	}
//
//
//	//  delete list;                      // delete the list, not the widgets
//
//}

void	VtkWidget::GenerateCubeSource()
{
//	
//	double size = 0.15;
//	vtkCubeSource* src = vtkCubeSource::New();
//	src->SetBounds(-size, size, -size, size, -size, size);
//
//	vtkExtractEdges* extr = vtkExtractEdges::New();
//	extr->SetInputConnection(src->GetOutputPort());
//
//	extr->Update();
//
//
//
//
//
//	cubeSourcePolys = vtkPolyData::New();
//	cubeSourcePolys->DeepCopy(extr->GetOutput());
//	//cubeSourcePolys->Update();
////	cubeSourcePolys->UpdateData();
//
//
//	int errr = cubeSourcePolys->GetNumberOfPoints();
//
//
//	double px[3];
//
//	//-----------------------------------
//	double textScale = size * 0.5;
//	double offset = size;
//	double* bounds = cubeSourcePolys->GetBounds();
//	double cu = bounds[0] + textScale * fabs(0.9 * (bounds[1] - bounds[0]));
//	///double cv = -textScale*fabs(0.5*(bounds[2] + bounds[3]));
//
//	vtkActor* tmpActor = vtkActor::New();
//
//
//
//	vtkPolyData* tmpPolyData;//=vtkPolyData::New();
//	//------------------------------------
//
//	vtkTransformPolyDataFilter* transformFilter = vtkTransformPolyDataFilter::New();
//	vtkTransform* transform = vtkTransform::New();
//	transformFilter->SetTransform(transform);
//
//	vtkVectorText* letter = vtkVectorText::New();
//	vtkAppendPolyData* append = vtkAppendPolyData::New();
//	append->AddInputData(cubeSourcePolys);
//
//	char* letters[6] = { "L","R","P","A","I","S" };
//	//double position[18]={offset,cu,cu,	-offset,-cu,cu,	-cu,offset,cu,	cu,-offset,cu,	cu,-cu,offset,	cu,cu,-offset};
//	//double orientation[18]={90,0,90,		90,0,-90,	90,0,180,	90,0,0,		0,0,-90,	180,0,90};
//
//	double position[18] = { offset,cu,cu,	-offset,-cu,cu,	-cu,offset,cu,	cu,-offset,cu,	cu,cu,offset,	cu,-cu,-offset };
//	double orientation[18] = { 90,0,90,		90,0,-90,	90,0,180,	90,0,0,		0,0,0,	180,0,0 };
//
//
//	int i;
//	for (i = 0;i < 6;i++)
//	{
//		letter->SetText(letters[i]);
//
//		tmpActor->SetScale(textScale);
//		tmpActor->SetPosition(&position[3 * i]);
//		tmpActor->SetOrientation(&orientation[3 * i]);
//		transform->SetMatrix(tmpActor->GetMatrix());
//		transform->Update();
//		//transformFilter->SetInputData(letter->GetOutput());
//		transformFilter->SetInputConnection(letter->GetOutputPort());
//		transformFilter->Update();
//		tmpPolyData = vtkPolyData::New();
//		tmpPolyData->DeepCopy(transformFilter->GetOutput());
//		//		tmpPolyData->Update();
//		append->AddInputData(tmpPolyData);
//		append->Update();
//	}
//
//
//
//	//append->Update();
//
//	cubeSourcePolys->DeepCopy(append->GetOutput());
//	//cubeSourcePolys->Update();
//
//
//
//	/*
//
//	this->XPlusFaceVectorText->Update();
//	double* bounds = this->XPlusFaceVectorText->GetOutput()->GetBounds();
//	double cu = -this->FaceTextScale*fabs(0.5*(bounds[0] + bounds[1]));
//	double cv = -this->FaceTextScale*fabs(0.5*(bounds[2] + bounds[3]));
//
//	this->XPlusFaceActor->SetScale( this->FaceTextScale );
//	this->XPlusFaceActor->SetPosition( offset, cu, cv );
//	this->XPlusFaceActor->SetOrientation( 90 , 0, 90 );
//
//
//	cube->SetXPlusFaceText ( "R");
//	cube->SetXMinusFaceText ("L");
//	cube->SetYPlusFaceText  ("A");
//	cube->SetYMinusFaceText ("P");
//	cube->SetZPlusFaceText  ("I");
//	cube->SetZMinusFaceText ("S");
//	cube->SetXFaceTextRotation (180);
//	cube->SetYFaceTextRotation (180);
//	cube->SetZFaceTextRotation (-90);
//	cube->SetFaceTextScale (0.65);
//	*/
//
//
//	cubePolys = vtkPolyData::New();
//	cubePolys->DeepCopy(cubeSourcePolys);
//	//cubePolys->Update();
//
//	cubePolys2D = vtkPolyData::New();
//	cubePolys2D->DeepCopy(cubePolys);
//	//cubePolys2D->Update();
//
//	trans = vtkTransformCoordinateSystems::New();
//	trans->SetInputCoordinateSystemToWorld();
//	trans->SetOutputCoordinateSystemToDisplay();
//	trans->SetViewport(backgroundRendererLogo);
//
//
//	mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
//	mapper->SetInputData(cubePolys2D);
//	mapper->SetScalarModeToDefault();
//	mapper->Update();
//	cubeActor2D = vtkActor2D::New();
//	cubeActor2D->SetMapper(mapper);
//	logoP1[0] = 50;	logoP1[1] = 50;
//	logoP2[0] = 100;	logoP2[1] = 100;
//	cubeActor2D->SetPosition(logoP1);
//	cubeActor2D->SetPosition2(logoP2);
//	Renderer->AddViewProp(cubeActor2D);
//	cubeActor2D->GetProperty()->SetColor(frontCol);

double size = 0.5;
vtkCubeSource* src = vtkCubeSource::New();
src->SetBounds(-size, size, -size, size, -size, size);

vtkExtractEdges* extr = vtkExtractEdges::New();
extr->SetInputConnection(src->GetOutputPort());

extr->Update();





cubeSourcePolys = vtkPolyData::New();
cubeSourcePolys->DeepCopy(extr->GetOutput());
//cubeSourcePolys->Update();
//	cubeSourcePolys->UpdateData();


int errr = cubeSourcePolys->GetNumberOfPoints();


double px[3];

//-----------------------------------
double textScale = size*0.4;
double offset = size;
double* bounds = cubeSourcePolys->GetBounds();
double cu = bounds[0] + textScale * fabs(0.5 * (bounds[1] - bounds[0]));
///double cv = -textScale*fabs(0.5*(bounds[2] + bounds[3]));

vtkActor* tmpActor = vtkActor::New();



vtkPolyData* tmpPolyData;//=vtkPolyData::New();
//------------------------------------

vtkTransformPolyDataFilter* transformFilter = vtkTransformPolyDataFilter::New();
vtkTransform* transform = vtkTransform::New();
transformFilter->SetTransform(transform);

vtkVectorText* letter = vtkVectorText::New();
vtkAppendPolyData* append = vtkAppendPolyData::New();
append->AddInputData(cubeSourcePolys);

char* letters[6] = { "L","R","P","A","I","S" };
//double position[18]={offset,cu,cu,	-offset,-cu,cu,	-cu,offset,cu,	cu,-offset,cu,	cu,-cu,offset,	cu,cu,-offset};
//double orientation[18]={90,0,90,		90,0,-90,	90,0,180,	90,0,0,		0,0,-90,	180,0,90};

double position[18] = { offset,cu,cu,	-offset,-cu,cu,	-cu,offset,cu,	cu,-offset,cu,	cu,cu,offset,	cu,-cu,-offset };
double orientation[18] = { 90,0,90,		90,0,-90,	90,0,180,	90,0,0,		0,0,0,	180,0,0 };


int i;
for (i = 0;i < 6;i++)
{
	letter->SetText(letters[i]);

	tmpActor->SetScale(textScale);
	tmpActor->SetPosition(&position[3 * i]);
	tmpActor->SetOrientation(&orientation[3 * i]);
	transform->SetMatrix(tmpActor->GetMatrix());
	transform->Update();
	//transformFilter->SetInputData(letter->GetOutput());
	transformFilter->SetInputConnection(letter->GetOutputPort());
	transformFilter->Update();
	tmpPolyData = vtkPolyData::New();
	tmpPolyData->DeepCopy(transformFilter->GetOutput());
	//		tmpPolyData->Update();
	append->AddInputData(tmpPolyData);
	append->Update();
}



//append->Update();

cubeSourcePolys->DeepCopy(append->GetOutput());
//cubeSourcePolys->Update();



/*

this->XPlusFaceVectorText->Update();
double* bounds = this->XPlusFaceVectorText->GetOutput()->GetBounds();
double cu = -this->FaceTextScale*fabs(0.5*(bounds[0] + bounds[1]));
double cv = -this->FaceTextScale*fabs(0.5*(bounds[2] + bounds[3]));

this->XPlusFaceActor->SetScale( this->FaceTextScale );
this->XPlusFaceActor->SetPosition( offset, cu, cv );
this->XPlusFaceActor->SetOrientation( 90 , 0, 90 );


cube->SetXPlusFaceText ( "R");
cube->SetXMinusFaceText ("L");
cube->SetYPlusFaceText  ("A");
cube->SetYMinusFaceText ("P");
cube->SetZPlusFaceText  ("I");
cube->SetZMinusFaceText ("S");
cube->SetXFaceTextRotation (180);
cube->SetYFaceTextRotation (180);
cube->SetZFaceTextRotation (-90);
cube->SetFaceTextScale (0.65);
*/


cubePolys = vtkPolyData::New();
cubePolys->DeepCopy(cubeSourcePolys);
//cubePolys->Update();




cubePolys2D = vtkPolyData::New();
cubePolys2D->DeepCopy(cubePolys);
//cubePolys2D->Update();

trans = vtkTransformCoordinateSystems::New();
trans->SetInputCoordinateSystemToWorld();
trans->SetOutputCoordinateSystemToDisplay();
trans->SetViewport(Renderer);


mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
mapper->SetInputData(cubePolys2D);
mapper->SetScalarModeToDefault();
mapper->Update();
cubeActor2D = vtkActor2D::New();
cubeActor2D->SetMapper(mapper);
logoP1[0] = 0;	logoP1[1] = 0;
logoP2[0] = 100;	logoP2[1] = 100;
cubeActor2D->SetPosition(logoP1);
cubeActor2D->SetPosition2(logoP2);
Renderer->AddViewProp(cubeActor2D);
cubeActor2D->GetProperty()->SetColor(frontCol);
cubeActor2D->GetProperty() ->SetLineWidth(2);
cubeActor2D->GetProperty()->SetLineStipplePattern(344);

//cubeActor2D->GetProperty()->SetOpacity(0.5);

}
//-------------
void	VtkWidget::setBackgroundColor(double* c)
{
	if (c == NULL) return;
	for (int i = 0;i < 3;i++) bckCol[i] = c[i];
	Renderer->SetBackground(bckCol);
}
//-------------
void	VtkWidget::setFrontColor(double* c)
{
	if (c == NULL) return;
	for (int i = 0; i < 3; i++) frontCol[i] = c[i];

	cubeActor2D->GetProperty()->SetColor(frontCol);

}
//-------------------------
//{bkgCol[0]=c[0];bkcgCol[1]=c[1];bkcgCol[2]=c[2];};
void	VtkWidget::setGradientBackgroundColor1(double* c)
{
	if (c == NULL) return;
	for (int i = 0;i < 3;i++) gradCol1[i] = c[i];
	Renderer->SetBackground(gradCol1);

}
//-------------------------//{gradCol1[0]=c[0];gradCol1[1]=c[1];gradCol1[2]=c[2];};
void	VtkWidget::setGradientBackgroundColor2(double* c)
{
	if (c == NULL) return;
	for (int i = 0;i < 3;i++) gradCol2[i] = c[i];

	Renderer->SetBackground2(gradCol2);

}
//-------------------------//{gradCol2[0]=c[0];gradCol2[1]=c[1];gradCol2[2]=c[2];};

void	VtkWidget::getBackgroundColor(double* c)
{
	if (c == NULL) return;
	for (int i = 0;i < 3;i++) c[i] = bckCol[i];


}
//-------------------------//{bkgCol[0]=c[0];bkcgCol[1]=c[1];bkcgCol[2]=c[2];};
void	VtkWidget::getGradientBackgroundColor1(double* c)
{
	if (c == NULL) return;
	for (int i = 0;i < 3;i++) c[i] = gradCol1[i];

}
//-------------------------//{gradCol1[0]=c[0];gradCol1[1]=c[1];gradCol1[2]=c[2];};
void	VtkWidget::getGradientBackgroundColor2(double* c)
{
	if (c == NULL) return;
	for (int i = 0;i < 3;i++) c[i] = gradCol2[i];

}
void	VtkWidget::setEyeAngle(double angle)
{
	eyeAngle = angle;
	Renderer->GetActiveCamera()->SetEyeAngle(eyeAngle);
}
//------------------------
//----------------------------------
void	VtkWidget::setViewAngle(double angle)
{
	viewAngle = angle;
	Renderer->GetActiveCamera()->SetViewAngle(viewAngle);
}
//----------------------------------
double	VtkWidget::getEyeAngle()
{
	return eyeAngle;
}
//----------------------------------
double	VtkWidget::getViewAngle()
{
	return viewAngle;
}
//----------------------------------
//-------------------------//{gradCol2[0]=c[0];gradCol2[1]=c[1];gradCol2[2]=c[2];};
void	VtkWidget::setBackgroundColor(double a, double b, double c) { bckCol[0] = a;bckCol[1] = b;bckCol[2] = c;Renderer->SetBackground(bckCol); };
void	VtkWidget::setGradientBackgroundColor1(double a, double b, double c) { gradCol1[0] = a;gradCol1[1] = b;gradCol1[2] = c;Renderer->SetBackground(gradCol1); };
void	VtkWidget::setGradientBackgroundColor2(double a, double b, double c) { gradCol2[0] = a;gradCol2[1] = b;gradCol2[2] = c;Renderer->SetBackground2(gradCol2); };

void	VtkWidget::SlotSetBackgroundMode(int mode)
{

	if (mode != 5)
	{

		if (bckgRenderer == true)
		{
			Interactor->GetRenderWindow()->RemoveRenderer(backgroundRenderer);
			Interactor->GetRenderWindow()->SetNumberOfLayers(1);
			backgroundRenderer->SetLayer(0);
			Renderer->SetLayer(0);
			bckgRenderer = false;
		}
	}

	switch (mode)
	{
	case 0: //kolor
		SlotSetSkyboxVisibility(false);
		Renderer->GradientBackgroundOff();
		Renderer->SetBackground(bckCol);
		backgroundMode = 0;
		//Interactor->GetRenderWindow()->Render();
//R/=2;B/=2;G/=2;
//Renderer->SetBackground2((double)(R/255.0),(double)(G/255.0),(double)(B/255.0));


		break;

	case 1: //gradient
		SlotSetSkyboxVisibility(false);
		Renderer->GradientBackgroundOn();
		Renderer->SetBackground(gradCol1);
		Renderer->SetBackground2(gradCol2);
		backgroundMode = 1;
		//Interactor->GetRenderWindow()->Render();

		break;

	case 2: //skybox

		SlotSetSkyboxVisibility(true);
		backgroundMode = 2;

		break;

	case 5:

		SlotSetSkyboxVisibility(false);
		if (bckgRenderer == false)

		{
			Interactor->GetRenderWindow()->SetNumberOfLayers(2);
			backgroundRenderer->SetLayer(0);
			Renderer->SetLayer(1);

			Interactor->GetRenderWindow()->AddRenderer(backgroundRenderer);
			bckgRenderer = true;
			//Interactor->GetRenderWindow()->SetNumberOfLayers(2);
		}



		break;



	}
}
//-------------------------

