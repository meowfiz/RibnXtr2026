#ifndef	VTK_WIDGET
#define	VTK_WIDGET
#include <vtkUnstructuredGrid.h>
#include <vtkHexahedron.h>
//#include <vtkMeshQuality.h>
#include <vtkGPUInfoList.h>
#include <vtkGPUInfo.h>
#include <qsplitter.h>
#include <qwidget.h>
#include <qlayout.h>
//Added by qt3to4:
#include <QResizeEvent>

#include <stdio.h>
#include <qtooltip.h>
#include <qwidget.h>

#include <vtkTextureMapToPlane.h>
#include <vtkImageReader2.h>
#include <vtkPlaneSource.h>
#include <vtkJPEGReader.h>
#include <vtkTexture.h>
#include <vtkExtractEdges.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkCollectionIterator.h>
#include <vtkPolyDataMapper2D.h>
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkInteractorStyleJoystickCamera.h"
#include "vtkInteractorStyleFlight.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackball.h"
#include "vtkInteractorStyleFlight.h"
#include "vtkLightCollection.h"
#include "vtkSmartPointer.h"
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkActor2D.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkCubeSource.h>
#include <vtkProperty2D.h>
#include <vtkTransformCoordinateSystems.h>
#include <vtkSuperquadricSource.h>
#include <vtkImageMapper.h>
//#include <vtkMeshQuality.h>

#include "renderwindowinteractor.h"
//#include "MyDockWidget.h"

#include <vtkAnnotatedCubeActor.h>
#include <vtkVolumeCollection.h>
#include <vtkVectorText.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkAppendPolyData.h>
#include <vtkPlanes.h>
#include <vtkClipPolyData.h>
#include <vtkImageActor.h>
#include <vtkImageMapper.h>
#include "renderwindowinteractor.h"
#include "MyDockWidget.h"
#include <vtkTextActor.h>
#include "Geometry.h"
//#include "LinkedList.h"
//#include "SurfaceCollection.h"
//#include "QListView2DSliceWidget.h"

//class RenderWindowInteractor;
//#include "renderwindow.h"
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingFreeType);



class VtkWidget : public RenderWindowInteractor//MyDockWidget//,QToolTip
{
	Q_OBJECT

public:


	vtkSmartPointer<vtkRenderer> backgroundRenderer;
	vtkSmartPointer<vtkRenderer> backgroundRendererLogo;
	vtkSmartPointer<vtkImageMapper> imageMapper;
	VtkWidget(QWidget* parent = 0, QString name = "", Qt::WindowFlags flags = 0);
	//QVBoxLayout* VLayout;

	vtkTransformCoordinateSystems* trans;
	vtkTransformCoordinateSystems* trans2;

	QString defaultPropertyFilename;
	QString progDirPath;
	vtkSmartPointer<vtkImageData> imageData;
	vtkSmartPointer<vtkImageActor> imageActor;
	bool bckgRenderer;
	vtkPolyData* tempPolys, * cubePolys, * cubeSourcePolys, * cubePolys2D, * cubePolysFlat;
	vtkActor2D* cubeActor2D;
	vtkTextActor* textActor;

	vtkActor* cubeActorFlat;
	vtkSmartPointer<vtkPolyDataMapper2D> mapper;
	vtkSmartPointer<vtkPolyDataMapper> mapper3D;
	vtkSmartPointer<vtkActor >skyboxActor[6];


	bool ProjectionMode;

	double scaleX;
	double logoSizeX;
	bool refreshMode;
	double logoP1[2], logoP2[2];
	double sizes[3];

	bool lKeyPressed;
	double resetCameraDistance;

	//	VtkInteractorWidget* Interactor;

	RenderWindowInteractor* Interactor;
	vtkRenderer* Renderer;

	void	GenerateCubeSource();
	void	SetDefaultPropertyFileName(QString n) { defaultPropertyFilename = n; };
	QString GetDefaultPropertyFileName() { return defaultPropertyFilename; };

	void	SetProgDirPath(QString n) { progDirPath = n; };
	QString GetProgDirPath() { return progDirPath; };


	void	SetResetCameraMode(bool mode) { resetCameraMode = mode; };
	bool		GetResetCameraMode() { return resetCameraMode; };


	void	SetNewBaseMatrix(double* m)
	{
		if (m != NULL)
		{
			for (int i = 0;i < 16;i++)
			{
				baseMatrix[i]= m[i];
			}
		}
	};

	void	SetRefreshMode(bool mode) 
	{
		refreshMode = mode; 
		
	};// SetBlockPaintEvent(!mode);};

	bool		GetRefreshMode() { return refreshMode; };

	void  test(const QRect& rect, const QString& text)
	{
		//		tip(rect,text);
		//		this->
	};

	bool	getTrickMode() { return trickMode; };
	void	setTrickMode(bool t) { trickMode = t; };


	void	setTextActor(QString txt, int pos[2], int fontSize, double color[3]);
	void	hideTextActor();

	void	setBackgroundColor(double* c);//{bkgCol[0]=c[0];bkcgCol[1]=c[1];bkcgCol[2]=c[2];};
	void	setFrontColor(double* c);
	void	setGradientBackgroundColor1(double* c);//{gradCol1[0]=c[0];gradCol1[1]=c[1];gradCol1[2]=c[2];};
	void	setGradientBackgroundColor2(double* c);//{gradCol2[0]=c[0];gradCol2[1]=c[1];gradCol2[2]=c[2];};

	void	setBackgroundColor(double a, double b, double c);//{bkgCol[0]=c[0];bkcgCol[1]=c[1];bkcgCol[2]=c[2];};
	void	setGradientBackgroundColor1(double a, double b, double c);//{gradCol1[0]=c[0];gradCol1[1]=c[1];gradCol1[2]=c[2];};
	void	setGradientBackgroundColor2(double a, double b, double c);//{gradCol2[0]=c[0];gradCol2[1]=c[1];gradCol2[2]=c[2];};

	void	getBackgroundColor(double* c);//{bkgCol[0]=c[0];bkcgCol[1]=c[1];bkcgCol[2]=c[2];};
	void	getGradientBackgroundColor1(double* c);//{gradCol1[0]=c[0];gradCol1[1]=c[1];gradCol1[2]=c[2];};
	void	getGradientBackgroundColor2(double* c);//{gradCol2[0]=c[0];gradCol2[1]=c[1];gradCol2[2]=c[2];};

//	void	SlotSetBackgroundMode(int mode);
	void	setEyeAngle(double angle);
	void	setViewAngle(double angle);
	double	getEyeAngle();
	double	getViewAngle();
	int		GetBackgroundMode() { return backgroundMode; };
	QString GetSkyboxName() { return skyBoxName; };


	void	AddTextActor(vtkTextActor* textActor);
	void	RemoveTextActor(vtkTextActor* textActor);
	vtkTextActor* GetLastActor() { return lastTextActor; };
protected:
	bool eventFilter(QObject* obj, QEvent* event);
private:

	vtkTextActor* lastTextActor;
	double eyeAngle, viewAngle;
	double bckCol[3];
	double frontCol[3];
	double gradCol1[3];
	double gradCol2[3];

	int		skyBoxModel;
	bool	skyBox;
	bool resetCameraMode;
	bool trickMode;
	void	GetBounds(double* bounds);
	QString skyBoxName;
	int		backgroundMode;
	double baseMatrix[16];
	


public slots:




	void	SlotSetCubeVisibility(bool v);
	void	SlotUpdateCube();
	void	SlotChangeBackgroundColor(int R, int G, int B);
	void	RepaintWidget();
	void	SlotSetInteractorModeToTrackball();
	void	SlotSetInteractorModeToJoystick();
	void	SlotSwitchProjectionModes();
	void	SlotResetCamera();
	void	SlotResetCube();


	void	SlotCameraRotation(double Azimuth, double Elevation, double Roll);
	void	SlotSetSkyboxVisibility(bool);
	void	SetCurrentSkyboxModel(QString skyName);
	void	SlotSetBackgroundMode(int mode);




	void	maybeTip(const QPoint& p);
	void	mouseMoved();
	//void	resizeEvent(QResizeEvent*);




signals:

	void signalMouseMoved(int x, int y);
	void signalMouseMoved(int x, int y, int button);
	//	void SendCurrentCameraRotation(double Azimuth, double Elevation, double Roll);
	void signalLeftDoubleClicked(int x, int y);
	void signalRightWitCTRLClicked(int x, int y);
	void signalTolTipActivated(const QPoint& p);


};

#endif //   VTK_WIDGET
