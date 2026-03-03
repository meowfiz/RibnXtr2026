#ifndef RenderWindowInteractor_h
#define RenderWindowInteractor_h

//
// $Id: renderwindowinteractor.h,v 1.2 2000/09/14 13:25:43 jstifter Exp $
//

//#include	"SurfaceCollection.h"
#include "renderwindow.h"

#include <qtimer.h>
#include <qwidget.h>
#include <qdebug.h>
//Added by qt3to4:
#include <qpolygon.h>
#include <QMouseEvent>
#include<qcolor.h>
#include<qdir.h>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkCellPicker.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageData.h>


#include	"LinkedList.h"
//include	"SurfaceData.h"

#include	"qpoint.h"
#include <QMetaType>
//class PolygonSurfaceData;


class RenderWindowInteractor : public RenderWindow
{
	Q_OBJECT

public:

	RenderWindowInteractor(QWidget* parent = 0, const char* name = 0);
	//RenderWindowInteractor(QGLFormat glf, QWidget* parent = 0, const char* name = 0);

	~RenderWindowInteractor();

	void SetLeftButtonFunction(const char* function);
	void SetMiddleButtonFunction(const char* function);
	void SetRightButtonFunction(const char* function);
	void SetShiftLeftButtonFunction(const char* function);

	void SetInteractionActor(vtkActor* actor);

	void SetSecondActorActor(vtkActor* actor);

	void SetActorRotation(double x, double y, double z);
	void SetPlane(double* ebene);
	void SetInteractionTime(int time);
	void SetSensitivity(double t);

	int GetInteractionTime(void);
	vtkActor* GetInteractionActor(void);
	double GetSensitivity();
	const char* GetClassName();

	QImage* grabImage;

	QImage* grabRenderWindow();
	QImage* grabRenderWindowGL();

	QImage* TrickyGrabWindow();
	void	GrabTrickyWindow2();
	void	CleanTrickyWindow();


	int      lastMousePos[2];
	QTimer* stepTimer;

	//LinkedList<PolygonSurfaceData*>* SurfaceDataList;
	void ClearPointArray();

signals:

	void	signalRenderWindowInteractorAltReleased();
	void	signalRenderWindowInteractorReleased(QKeyEvent* event);
	void	mouseReleased();
	void	signalMouseMoved(int x, int y, int button);
	void	signalMouseMoved(int x, int y);
	void	mouseMoved();
	void	signalLeftWithCtrlClicked(int x, int y);
	void	signalLeftDoubleClicked(int x, int y);
	void	signalRightWithCTRLClicked(int x, int y, QMouseEvent* event);
	void	signalRightWithCTRLMoved(int x, int y, QMouseEvent* event);
	void	signalRightWithCTRLReleased(int x, int y, QMouseEvent* event);

	void	signalRightWithAltClicked(int x, int y,QWidget *);
	void	signalRightWithCTRLDoubleClicked(int x, int y);
	void	signalLeftWithAltDoubleClicked(int x, int y);
	void	signalLeftWithModifierDoubleClicked(int x, int y, int mod);



	void	signalSendSurfaceTranslate(double* newP, double* oldP);
	void	signalSendSurfaceZoom(double* vpNormal, double yf);
	void	signalSendSurfaceRoll(double* vpNormal, double angle, double* TempOrig2);
	void	signalActorPlaneMove(double* p0, double* p1);
	void	signalActorPlaneRoll(double *normale, double angle, double* TempOrig2);
	void	signalSurfaceRotate(double xf, double yf, double *viewUp, double *rotateY, double *TempOrigin2);

	void	signalSendPointArray(QPolygon Qarray);

	/*!
	   \brief This signal is emitted, whenever we have rotated an actor.
	   \param xf the angle in degrees around the view-up vector of the current
	   camera.
	   \param yf the angle in degrees around the
	   cross of the viewplane-normal and the view-up vector.
	*/
	void ActorRotated(double xf, double yf);

	/*!
	   \brief This signal is emitted, whenever we have rolled an actor
	   Rolling is rotating in the viewplane of the camera
	   \param angle the angle in degrees around the viewplane-normal of
	   the current camera.
	*/
	void ActorRolled(double angle);

	/*!
	   This signal is emitted, when the interaction is finished.
	   \param x the x coordinate of the pointer
	   \param y the y coordinate of the pointer
	*/
	void Released(int x, int y);


	void CubeUpdated();
	void CubePosUpdated(double*);

	void SendCurrentCameraRotation(double Azimuth, double Elevation, double Roll);



protected:

	void mousePressEvent(QMouseEvent* event);

	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void keyReleaseEvent(QKeyEvent* event);


public slots:

	void	SlotSetInteractionWithCamera();
	void	SlotSetInteractionWithActor();
	void	SlotSetInteractionWithCubeActor(double* _CubeCenter, double* _CubeSize);

	void	CameraUserRotate(double Azimuth, double Elevation, double Roll);

	//void	SetSurfaceDataList(LinkedList<PolygonSurfaceData*>* _SurfaceDataList);

	void	SetInteractionModeToCuttingPlane(bool);

	void	SlotPressMaskDefinition(int x, int y, QMouseEvent* event);
	void	SlotPressMaskMove(int x, int y, QMouseEvent* event);
	void	SlotPressMaskRelease(int x, int y, QMouseEvent* event);

	void	SlotResizeRenderWindow(int x, int y);

	void mainHandPress(int, int, Qt::MouseButton, bool);
	void mainHandRelease(int, int, Qt::MouseButton);
	void mainHandMoved(int, int, Qt::MouseButton);
	void CubeActorZoom(double* currentMousePos, double* lastMousePos);
	void CubeActorZoom();
protected slots:

	void ActorRotate();
	void ActorTranslate();
	void ActorZoom();
	void ActorRoll();
	void ActorPlaneRoll();
	void ActorPlaneMove();

	void CameraRotate();
	void CameraTranslate();
	void CameraZoom();
	void CameraRoll();

	void CubeActorResizeXY();
	void CubeActorResizeZ();


	void CubeActorTranslate();




	void DoNothing();



private:

	void DisplayToWorld(double x, double y, double z,
		double* worldPoint);

	void WorldToDisplay(double x, double y, double z,
		double* displayPoint);

	void InitRenderWindowInteractor();

	vtkCellPicker* cellpicker;

	vtkCamera* currentCamera;
	vtkActor* interactionActor;

	vtkActor* secondActor;

	vtkActor* collisionActor;
	vtkLight* currentLight;


	double viewUp[3];
	double vpNormal[3];
	double rotateY[3];
	double renCenter[3];
	double viewFocus[3];
	double camPos[3];
	double deltaElevation;
	double deltaAzimuth;
	double normale[3];
	double ebenekonstante;

	
	bool     mouseEventActive;
	bool     firstCall;
	double    trackballFactor;
	double    cameraRotation[3];
	int      interactionTime;
	//   int      currentMousePos[2];


	int		MaskPaintMode;
	bool	PointArraySentAlready;
	bool	SendMaskMode;
	QPolygon* PointArray;
	QPoint	Point1, Point2;
	QPoint	PointPos, PointSize;
	int		PointIndex;

	double* CubeCenter;
	double* CubeSize;


	bool	InteractWithCuttingTool;

	char leftButtonFunction[200];
	char middleButtonFunction[200];
	char rightButtonFunction[200];
	char shiftLeftButtonFunction[200];

	QPoint pressPos;
	bool dbl;
};
Q_DECLARE_METATYPE(QMouseEvent*)
#endif  // RenderWindowInteractor_h
