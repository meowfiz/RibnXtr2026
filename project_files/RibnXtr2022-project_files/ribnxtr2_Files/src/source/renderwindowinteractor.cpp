#include "renderwindowinteractor.h"

#include "vtkMath.h"
#include "vtkLightCollection.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkMath.h"
//Added by qt3to4:
#include <QPolygon>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>


//--------------------------------------------------------------------------
QImage* RenderWindowInteractor::grabRenderWindow()
{
	vtkWindowToImageFilter* Filterek = vtkWindowToImageFilter::New();
	Filterek->SetInput(this->GetRenderWindow());
	Filterek->Update();

	GetRenderWindow()->Render();
	Filterek->Modified();
	vtkImageData* IData = Filterek->GetOutput();
	IData->Modified();

	int size[3];
	IData->GetDimensions(size);

	QImage* Image = new QImage(size[0], size[1], QImage::Format_RGB32);
	unsigned char* pixeldata = Image->bits();//new unsigned char[s*4];
	unsigned char* pixeldataV = (unsigned char*)IData->GetScalarPointer();
	int i, j;
	int step1 = 0;
	int	step2 = (size[0] * (size[1] - 1)) * 3;

	for (j = 0;j < size[1];j++)
	{
		for (i = 0;i < size[0];i++)
		{
			pixeldata[step1++] = pixeldataV[step2 + 2];
			pixeldata[step1++] = pixeldataV[step2 + 1];
			pixeldata[step1++] = pixeldataV[step2];
			step2 += 3;
			step1++;
		}
		step2 -= (2 * size[0]) * 3;
	}

	IData = NULL;


	Filterek->Delete();
	return Image;
}

QImage* RenderWindowInteractor::grabRenderWindowGL()
{

	GetRenderWindow()->Render();
	int size[2];
	size[0] = this->GetRenderWindow()->GetSize()[0];
	size[1] = this->GetRenderWindow()->GetSize()[1];
	unsigned char* pixeldataV = new unsigned char[3 * size[0] * size[1]];

	//	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	//glPixelStorei( GL_PACK_ROW_LENGTH, 0 );
	glReadPixels(0, 0, size[0], size[1], GL_RGB, GL_UNSIGNED_BYTE, pixeldataV);
	//glPopClientAttrib();		




	QImage* Image = new QImage(size[0], size[1], QImage::Format_RGB32);

	unsigned char* pixeldata = Image->bits();
	for (int i = 0, a4 = 4 * size[0] * (size[1] - 1), a3 = 0; i < size[1]; ++i, a4 -= 2 * 4 * size[0]) {
		for (int j = 0; j < size[0]; ++j, a4 += 4, a3 += 3) {
			pixeldata[a4 + 2] = pixeldataV[a3];
			pixeldata[a4 + 1] = pixeldataV[a3 + 1];
			pixeldata[a4 + 0] = pixeldataV[a3 + 2];
			pixeldata[a4 + 3] = 255;
		}
	}
	delete[] pixeldataV;
	/*Image->save("d:\\tmp\\pic.png", "PNG");*/
	return Image;
	return NULL;
}

RenderWindowInteractor::RenderWindowInteractor(QWidget* parent, const char* name) : RenderWindow(parent, name)
{
	PointArray = NULL;poly = PointArray;grabImage = NULL;grabImageR = grabImage;
	PointIndex = 0;
	SendMaskMode = true;
	PointArraySentAlready = true;

	this->InitRenderWindowInteractor();
	this->setMouseTracking(true);

	connect(this, SIGNAL(signalRightWithCTRLClicked(int, int, QMouseEvent*)), this, SLOT(SlotPressMaskDefinition(int, int, QMouseEvent*)));
	connect(this, SIGNAL(signalRightWithCTRLMoved(int, int, QMouseEvent*)), this, SLOT(SlotPressMaskMove(int, int, QMouseEvent*)));
	connect(this, SIGNAL(signalRightWithCTRLReleased(int, int, QMouseEvent*)), this, SLOT(SlotPressMaskRelease(int, int, QMouseEvent*)));

	//this->poly=this->PointArray;
}


//--------------------------------------------------------------------------

//RenderWindowInteractor::RenderWindowInteractor(QGLFormat glf, QWidget* parent, const char* name) : RenderWindow(glf, parent, name)
//{
//	PointArray = NULL;poly = PointArray;;grabImage = NULL;grabImageR = grabImage;
//	PointIndex = 0;
//	SendMaskMode = true;
//	PointArraySentAlready = true;
//
//	this->InitRenderWindowInteractor();
//
//	connect(this, SIGNAL(signalRightWithCTRLClicked(int, int, QMouseEvent*)), this, SLOT(SlotPressMaskDefinition(int, int, QMouseEvent*)));
//	connect(this, SIGNAL(signalRightWithCTRLMoved(int, int, QMouseEvent*)), this, SLOT(SlotPressMaskMove(int, int, QMouseEvent*)));
//	connect(this, SIGNAL(signalRightWithCTRLReleased(int, int, QMouseEvent*)), this, SLOT(SlotPressMaskRelease(int, int, QMouseEvent*)));
//
//
//	// SurfaceDataList=NULL;
//}




//--------------------------------------------------------------------------

RenderWindowInteractor::~RenderWindowInteractor()
{
	delete this->stepTimer;
}



//--------------------------------------------------------------------------

void RenderWindowInteractor::InitRenderWindowInteractor()
{


	InteractWithCuttingTool = false;
	//SurfaceDataList = NULL;

	this->SetLeftButtonFunction(SLOT(CameraRotate()));
	this->SetMiddleButtonFunction(SLOT(CameraTranslate()));
	this->SetRightButtonFunction(SLOT(CameraZoom()));
	this->SetShiftLeftButtonFunction(SLOT(CameraRoll()));

	this->mouseEventActive = false;
	this->SetInteractionTime(50);
	this->SetSensitivity(10.0);

	this->stepTimer = new QTimer();
	//   CHECK_PTR( this->stepTimer );

	this->currentCamera = this->GetRenderer()->GetActiveCamera();
	// CHECK_PTR( this->currentCamera );z

	 // get the light we are using
	vtkLightCollection* lightCollection = this->GetRenderer()->GetLights();
	lightCollection->InitTraversal();
	this->currentLight = lightCollection->GetNextItem();
	//CHECK_PTR( this->currentLight );

	this->interactionActor = NULL;

	this->secondActor = NULL;


	this->collisionActor = NULL;
}







//--------------------------------------------------------------------------

void RenderWindowInteractor::SlotSetInteractionWithActor()
{

	this->SetLeftButtonFunction(SLOT(ActorRotate()));
	this->SetMiddleButtonFunction(SLOT(ActorTranslate()));
	this->SetRightButtonFunction(SLOT(ActorZoom()));
	this->SetShiftLeftButtonFunction(SLOT(ActorRoll()));
	/*
	   \arg  ActorRotate()
	   \arg  ActorTranslate()
	   \arg  ActorZoom()
	   \arg  ActorRoll()
	   \arg  ActorPlaneMove()
	   \arg  ActorPlaneRoll()
	   \arg  CameraRotate()
	   \arg  CameraTranslate()
	   \arg  CameraZoom()
	   \arg  CameraRoll()
	   \arg  DoNothing()

	*/

}

//--------------------------------------------------------------------------

void RenderWindowInteractor::SlotSetInteractionWithCubeActor(double* _CubeCenter, double* _CubeSize)
{

	CubeCenter = _CubeCenter;
	CubeSize = _CubeSize;


	this->SetLeftButtonFunction(NULL);
	this->SetShiftLeftButtonFunction(NULL);
	//	this->SetLeftButtonFunction( SLOT(ActorRotate()) );
	this->SetMiddleButtonFunction(SLOT(CubeActorTranslate()));
	this->SetRightButtonFunction(SLOT(CubeActorZoom()));
	//	this->SetShiftLeftButtonFunction( SLOT(ActorRoll()) );

	/*
	   \arg  ActorRotate()
	   \arg  ActorTranslate()
	   \arg  ActorZoom()
	   \arg  ActorRoll()
	   \arg  ActorPlaneMove()
	   \arg  ActorPlaneRoll()
	   \arg  CameraRotate()
	   \arg  CameraTranslate()
	   \arg  CameraZoom()
	   \arg  CameraRoll()
	   \arg  DoNothing()

	*/

}



//--------------------------------------------------------------------------

void RenderWindowInteractor::SlotSetInteractionWithCamera()
{

	this->SetLeftButtonFunction(SLOT(CameraRotate()));
	this->SetMiddleButtonFunction(SLOT(CameraTranslate()));
	this->SetRightButtonFunction(SLOT(CameraZoom()));
	this->SetShiftLeftButtonFunction(SLOT(CameraRoll()));
	/*
	   \arg  ActorRotate()
	   \arg  ActorTranslate()
	   \arg  ActorZoom()
	   \arg  ActorRoll()
	   \arg  ActorPlaneMove()
	   \arg  ActorPlaneRoll()
	   \arg  CameraRotate()
	   \arg  CameraTranslate()
	   \arg  CameraZoom()
	   \arg  CameraRoll()
	   \arg  DoNothing()

	*/

}



/*!
   \brief Returns the class name
*/

//--------------------------------------------------------------------------

const char* RenderWindowInteractor::GetClassName()
{
	return("RenderWindowInteractor");
}


/*!
   \brief Sets the plane for the actor interaction ActorPlaneMove() and ActorPlaneRoll().

   \param ebene pointer to a double array with the four values, which specifiy a plane (ax+by+cz+d=1).
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::SetPlane(double* ebene)
{

	this->normale[0] = ebene[0];
	this->normale[1] = ebene[1];
	this->normale[2] = ebene[2];
	this->ebenekonstante = ebene[3];

}

/*!
   \brief Sets the sensitivity of all interactions

   \param t the sensitivity, default is 10.0
   \sa GetSensitivity()
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::SetSensitivity(double t)
{
	this->trackballFactor = t;
}

/*!
   \brief Returns the current sensitivity of all interactions

   \sa SetSensitivity()
*/

//--------------------------------------------------------------------------

double RenderWindowInteractor::GetSensitivity()
{
	return this->trackballFactor;
}


/*!
   \brief Sets the function, which will be used, when the left mouse button is pressed

   The function can be:
   \arg  ActorRotate()
   \arg  ActorTranslate()
   \arg  ActorZoom()
   \arg  ActorRoll()
   \arg  ActorPlaneMove()
   \arg  ActorPlaneRoll()
   \arg  CameraRotate()
   \arg  CameraTranslate()
   \arg  CameraZoom()
   \arg  CameraRoll()
   \arg  DoNothing()

   \param function the name of the function, wrapped in a slot, e.g.
   \c SLOT(CameraRotate()). If function is NULL,  DoNothing() will be used.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::SetLeftButtonFunction(const char* function)
{
	if (function == NULL) {
		strncpy(this->leftButtonFunction, SLOT(DoNothing()), 100);
	}
	else {
		strncpy(this->leftButtonFunction, function, 100);
	}
}


/*!
   \brief Sets the function, which will be used, when the middle mouse button is pressed

   The function can be:
   \arg  ActorRotate()
   \arg  ActorTranslate()
   \arg  ActorZoom()
   \arg  ActorRoll()
   \arg  ActorPlaneMove()
   \arg  ActorPlaneRoll()
   \arg  CameraRotate()
   \arg  CameraTranslate()
   \arg  CameraZoom()
   \arg  CameraRoll()
   \arg  DoNothing()

   \param function the name of the function, wrapped in a slot, e.g.
   \c SLOT(CameraRotate()). If function is NULL,  DoNothing() will be used.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::SetMiddleButtonFunction(const char* function)
{
	if (function == NULL) {
		strncpy(this->middleButtonFunction, SLOT(DoNothing()), 100);
	}
	else {
		strncpy(this->middleButtonFunction, function, 100);
	}
}


/*!
   \brief Sets the function, which will be used, when the right mouse button is pressed

   The function can be:
   \arg  ActorRotate()
   \arg  ActorTranslate()
   \arg  ActorZoom()
   \arg  ActorRoll()
   \arg  ActorPlaneMove()
   \arg  ActorPlaneRoll()
   \arg  CameraRotate()
   \arg  CameraTranslate()
   \arg  CameraZoom()
   \arg  CameraRoll()
   \arg  DoNothing()

   \param function the name of the function, wrapped in a slot, e.g.
   SLOT(CameraRotate()). If function is NULL, DoNothing() will be used.
*/


//--------------------------------------------------------------------------

void RenderWindowInteractor::SetRightButtonFunction(const char* function)
{
	if (function == NULL) {
		strncpy(this->rightButtonFunction, SLOT(DoNothing()), 100);
	}
	else {
		strncpy(this->rightButtonFunction, function, 100);
	}
}


/*!
   \brief Sets the function, which will be used, when the left button and the Qt::SHIFT key are pressed

	The function can be:
   \arg  ActorRotate()
   \arg  ActorTranslate()
   \arg  ActorZoom()
   \arg  ActorRoll()
   \arg  ActorPlaneMove()
   \arg  ActorPlaneRoll()
   \arg  CameraRotate()
   \arg  CameraTranslate()
   \arg  CameraZoom()
   \arg  CameraRoll()
   \arg  DoNothing()

   \param function the name of the function, wrapped in a slot, e.g.
   SLOT(CameraRotate()). If function is NULL, DoNothing() will be used.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::SetShiftLeftButtonFunction(const char* function)
{
	if (function == NULL) {
		strncpy(this->shiftLeftButtonFunction, SLOT(DoNothing()), 100);
	}
	else {
		strncpy(this->shiftLeftButtonFunction, function, 100);
	}
}


/*!
   \brief Sets the actor, which will be used in actor interactions.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::SetInteractionActor(vtkActor* actor)
{
	this->interactionActor = actor;

	this->secondActor = NULL;
}


//--------------------------------------------------------------------------



void RenderWindowInteractor::SetSecondActorActor(vtkActor* actor)
{
	this->secondActor = actor;
	if (this->interactionActor != NULL) secondActor->SetOrigin(this->interactionActor->GetOrigin());
}



/*!
   \brief Returns the actor, which is used in actor interactions.
*/

//--------------------------------------------------------------------------

vtkActor* RenderWindowInteractor::GetInteractionActor(void)
{
	return this->interactionActor;
}

/*!
   \brief Sets the point, in actor coordinate system, around which we rotate the
   current actor.

   \b Important: Set first the actor with
   SetInteractionActor() before using this function.
   \param x x of the rotation point
   \param y y of the rotation point
   \param z z of the rotation point
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::SetActorRotation(double x, double y, double z)
{
	assert(this->interactionActor != NULL);

	// set it to the interactionActor
	this->interactionActor->SetOrigin(x, y, z);
}


/*!
   \brief catches the mouse press to react appropriate

   Overriden to catch mousePressEvents and to start an internal
   timer, which calls the appropriate interaction routine.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::mousePressEvent(QMouseEvent* event)
{


	if (this->mouseEventActive)
	{
		return;
	}
	this->update();
	/*	this->setFocus();
		this->clearFocus();*/
	this->setFocus();
	//	qWarning() << "Press TYPE:" << QString::number(event->type());
	if (event->button() & Qt::LeftButton)
	{
		if (event->type() & QEvent::MouseButtonDblClick)
		{

			if (event->modifiers() & Qt::AltModifier)
			{
				emit signalLeftWithAltDoubleClicked(event->x(), this->height() - event->y());
				emit signalLeftWithModifierDoubleClicked(event->x(), this->height() - event->y(), 1);
			}
			else if (event->modifiers() & Qt::ControlModifier)
			{



				emit signalLeftWithModifierDoubleClicked(event->x(), this->height() - event->y(), 2);
			}
			else
			{
				emit signalLeftDoubleClicked(event->x(), this->height() - event->y());
				emit signalLeftWithModifierDoubleClicked(event->x(), this->height() - event->y(), 0);
			}
		}
		else
		{
			if (event->modifiers() & Qt::ShiftModifier)
			{
				// left mouse mouse and Qt::SHIFT button
				this->mouseEventActive = true;
				QObject::connect(this->stepTimer, SIGNAL(timeout()),
					this->shiftLeftButtonFunction);
			}
			else if ((event->modifiers() & Qt::ControlModifier))
			{
				this->mouseEventActive = true;
				QObject::connect(this->stepTimer, SIGNAL(timeout()),
					this->middleButtonFunction);

				emit signalLeftWithCtrlClicked(event->x(), this->height() - event->y());
			}
			else
			{
				// left mouse button without Qt::SHIFT button
				this->mouseEventActive = true;
				QObject::connect(this->stepTimer, SIGNAL(timeout()),
					this->leftButtonFunction);
			}


		}


	}
	else if (event->button() & Qt::MidButton)
	{
		// middle mouse button
		this->mouseEventActive = true;
		QObject::connect(this->stepTimer, SIGNAL(timeout()),
			this->middleButtonFunction);
	}
	else if (event->button() & Qt::RightButton)
	{


		if (event->modifiers() & Qt::ControlModifier)
		{
			if (event->type() & QEvent::MouseButtonDblClick)
			{
				emit signalRightWithCTRLDoubleClicked(event->x(), this->height() - event->y());
			}
			else
			{
				int bs = event->button();
				//	int evstate=event->state() ;
				emit signalRightWithCTRLClicked(event->x(), this->height() - event->y(), event);
			}
		}
		else if (event->modifiers() & Qt::AltModifier)
		{

			int bs = event->button();
			// int evstate=event->state() ;
			emit signalRightWithAltClicked(event->x(), this->height() - event->y(),this);


		}
		else
		{
			// right mouse button
			this->mouseEventActive = true;
			QObject::connect(this->stepTimer, SIGNAL(timeout()),
				this, this->rightButtonFunction);
		}
	}

	if (this->mouseEventActive)
	{
		this->currentMousePos[0] = event->x();
		this->currentMousePos[1] = event->y();
		this->lastMousePos[0] = event->x();
		this->lastMousePos[1] = event->y();
		this->firstCall = true;
		this->stepTimer->start(this->interactionTime);
	}
}


/*!
   \brief Overriden to catch mouseReleaseEvents and to stop the internal timer.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::mouseReleaseEvent(QMouseEvent* event)
{

	emit mouseReleased();
	if (this->mouseEventActive) {
		this->stepTimer->stop();
		this->mouseEventActive = false;
		QObject::disconnect(this->stepTimer, 0, this, 0);
		//	  qWarning() << "Release TYPEX:" << QString::number(event->type());
	}

	emit(Released(event->x(), this->height() - event->y()));

	// qWarning() << "Released TYPE:" << QString::number(event->type());

	emit
		/* int bs=event->button();
	  int evstate=event->state() ;
		  if( event->button() & RightButton )
		 {

			if( event->state() & ControlButton )
			{  */
		if (PointArraySentAlready == false)
			emit signalRightWithCTRLReleased(event->x(), this->height() - event->y(), event);
	// }
 //}


}


/*!
   \brief catches all movements of the mouse

   Overriden to save the mouse coords, which are needed for the interaction
   routines.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::mouseMoveEvent(QMouseEvent* event)
{
	//lastMousePosV2[0]=currentMousePos[0];
	//lastMousePosV2[1]=currentMousePos[1];	
	this->currentMousePos[0] = event->x();
	this->currentMousePos[1] = event->y();

	emit mouseMoved();
	//	if ((event->button()!=Qt::LeftButton)&&(event->button()!=Qt::RightButton)&&(event->button()!=Qt::MidButton))
	{
		emit	signalMouseMoved(currentMousePos[0], currentMousePos[1]);
		//qWarning()<<("aqq");
	}
	emit signalMouseMoved(currentMousePos[0], currentMousePos[1], event->buttons());
	//int bs=event->button();
	//int evstate=event->state() ;
	//   if( event->button() & RightButton ) 
	//   {
	//
	//	  if( event->state() & ControlButton )
	//	  {  
	if (PointArraySentAlready == false)
		emit signalRightWithCTRLMoved(event->x(), this->height() - event->y(), event);
	//	  }
	//  }
}


/*!
   \brief Set the time in milliseconds between two interactions.

   If you have huge
   objects and the interaction is too fast for the rendering, set the value
   bigger.
   Note that on some systems, the timer of qt (QTimer) can not be faster
   than the internal clock, which is around 16 msec (measured on WinNT 4.0).
   \param time the time in milliseconds
*/


//--------------------------------------------------------------------------

void RenderWindowInteractor::SetInteractionTime(int time)
{
	this->interactionTime = time;
}

/*!
   \brief Get the time in milliseconds between two interactions.
*/
int RenderWindowInteractor::GetInteractionTime(void) {
	return this->interactionTime;
}


/*!
   \brief Rotates an actor around a specific point

   The rotation point can be set with SetActorRotation(),
   the actor must be set with SetInteractionActor()
*/

//--------------------------------------------------------------------------


void RenderWindowInteractor::ActorRotate()
{
	double xf;
	double yf;

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	// basic idea: the change in x of the mouse is used to rotate around the
	// view-up vector. the change in y of the mouse is used to rotate around the
	// cross of the viewplane-normal and the view-up vector,

	// first time we do some preprocessing
	if (firstCall) {
		int* size = this->GetRenderWindow()->GetSize();
		double* vp = this->GetRenderer()->GetViewport();

		this->deltaAzimuth = -20.0 / ((vp[2] - vp[0]) * size[0]);
		this->deltaElevation = -20.0 / ((vp[3] - vp[1]) * size[1]);

		// get the rotation axis: cross of view-up and the normal in the viewplane
		this->currentCamera->GetViewUp(this->viewUp);
		this->currentCamera->GetViewPlaneNormal(this->vpNormal);
		vtkMath::Cross(this->vpNormal, this->viewUp, this->rotateY);

		this->firstCall = false;
	}

	// how much to rotate
	xf = (this->currentMousePos[0] - this->lastMousePos[0])
		* this->trackballFactor * deltaAzimuth * (-1);
	yf = (this->currentMousePos[1] - this->lastMousePos[1])
		* this->trackballFactor * deltaElevation;

	/* this->interactionActor->RotateWXYZ( xf, this->viewUp[0], this->viewUp[1],this->viewUp[2] );
	  this->interactionActor->RotateWXYZ( yf, this->rotateY[0], this->rotateY[1],this->rotateY[2] );
	  if (this->secondActor!=NULL)
	  {
		  this->secondActor->RotateWXYZ( xf, this->viewUp[0], this->viewUp[1],this->viewUp[2] );
		  this->secondActor->RotateWXYZ( yf, this->rotateY[0], this->rotateY[1],this->rotateY[2] );
	  }
  */
  //////////////////////////////////////////////////////////

	if ( (InteractWithCuttingTool == true))
	{
		this->interactionActor->RotateWXYZ(xf, this->viewUp[0], this->viewUp[1], this->viewUp[2]);
		this->interactionActor->RotateWXYZ(yf, this->rotateY[0], this->rotateY[1], this->rotateY[2]);
		if (this->secondActor != NULL)
		{
			this->secondActor->RotateWXYZ(xf, this->viewUp[0], this->viewUp[1], this->viewUp[2]);
			this->secondActor->RotateWXYZ(yf, this->rotateY[0], this->rotateY[1], this->rotateY[2]);
		}
	}
	else
	{
		double TempOrig2[3];
		this->interactionActor->GetOrigin(TempOrig2);
		emit signalSurfaceRotate(xf, yf, viewUp, rotateY, TempOrig2);
	}
	//	if (SurfaceDataList->GetNumberOfItems() >= 1)
	//	{
	//		int Nr = SurfaceDataList->GetNumberOfItems();
	//		int i;
	//		PolygonSurfaceData* Data;
	//		/////////////////
	//		double TempOrig[3];
	//		double TempOrig2[3];
	//		/////////////////

	//		this->interactionActor->GetOrigin(TempOrig2);


	//		for (i = 0;i < Nr;i++)
	//		{
	//			Data = SurfaceDataList->GetItemAtPos(i)->GetData();

	//			if (Data->GetTreeItem()->isSelected() == true)
	//			{
	//				//////////////////////////
	//				Data->GetVActor()->GetOrigin(TempOrig);
	//				Data->GetVActor()->SetOrigin(TempOrig2);
	//				/////////////////////////////

	//				Data->GetVActor()->RotateWXYZ(xf, this->viewUp[0], this->viewUp[1], this->viewUp[2]);
	//				Data->GetVActor()->RotateWXYZ(yf, this->rotateY[0], this->rotateY[1], this->rotateY[2]);

	//				//	Data->GetVActor()->SetOrigin(TempOrig);

	//				if (Data->GetCuttingEdgeActor() != NULL)
	//				{
	//					Data->GetCuttingEdgeActor()->SetOrigin(TempOrig2);

	//					Data->GetCuttingEdgeActor()->RotateWXYZ(xf, this->viewUp[0], this->viewUp[1], this->viewUp[2]);
	//					Data->GetCuttingEdgeActor()->RotateWXYZ(yf, this->rotateY[0], this->rotateY[1], this->rotateY[2]);

	//					Data->GetCuttingEdgeActor()->SetOrigin(TempOrig);
	//				}

	//			}
	//			////////


	//			///////
	//		}
	//		/////
	//		/////////
	//	}

	//}


	//////////////////////////////////////////////////////////

	emit ActorRotated(xf, yf);

	// update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];
}


/*!
   \brief Translates the actor on the viewplane

   The actor must be set with SetInteractionActor()
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::ActorTranslate()
{
	double* center;
	double dispCenter[4];
	double newP[4];
	double oldP[4];

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	center = this->interactionActor->GetCenter();
	this->WorldToDisplay(center[0], center[1], center[2], dispCenter);

	this->DisplayToWorld(double(this->currentMousePos[0]),
		double(this->height() - this->currentMousePos[1]),
		dispCenter[2],
		newP);

	this->DisplayToWorld(double(this->lastMousePos[0]),
		double(this->height() - this->lastMousePos[1]),
		dispCenter[2],
		oldP);

	/*
	   this->interactionActor->AddPosition( newP[0]-oldP[0],newP[1]-oldP[1],newP[2]-oldP[2] );
		if (this->secondActor!=NULL)
			this->secondActor->AddPosition( newP[0]-oldP[0],newP[1]-oldP[1],newP[2]-oldP[2] );
	*/

	/////////////////////////////////////////////////////////////
	if ( (InteractWithCuttingTool == true))
	{
		this->interactionActor->AddPosition(newP[0] - oldP[0], newP[1] - oldP[1], newP[2] - oldP[2]);
		if (this->secondActor != NULL)
			this->secondActor->AddPosition(newP[0] - oldP[0], newP[1] - oldP[1], newP[2] - oldP[2]);
	}
	else
	{
		emit signalSendSurfaceTranslate(newP, oldP);
		

		/*if (SurfaceDataList->GetNumberOfItems() >= 1)
		{
			int Nr = SurfaceDataList->GetNumberOfItems();
			int i;
			PolygonSurfaceData* Data;

			for (i = 0;i < Nr;i++)
			{
				Data = SurfaceDataList->GetItemAtPos(i)->GetData();
				if (Data->GetTreeItem()->isSelected() == true)
				{
					Data->GetVActor()->AddPosition(newP[0] - oldP[0], newP[1] - oldP[1], newP[2] - oldP[2]);
					if (Data->GetCuttingEdgeActor() != NULL)
						Data->GetCuttingEdgeActor()->AddPosition(newP[0] - oldP[0], newP[1] - oldP[1], newP[2] - oldP[2]);
				}
			}
		}*/

	}
	/////////////////////////////////////////////////////

		//double *Origin=this->interactionActor->GetOrigin();


	 //  this->interactionActor->SetOrigin( Origin[0]+newP[0]-oldP[0], Origin[1]+newP[1]-oldP[1], Origin[2]+newP[2]-oldP[2] );



	   // update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];
}


/*!
  \brief Zooms the actor on the normal of the viewplane

   The actor must be set with SetInteractionActor()
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::ActorZoom()
{
	double yf;

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	// first time we do some preprocessing
	if (firstCall) {
		this->currentCamera->GetViewPlaneNormal(this->vpNormal);
		firstCall = false;
	}

	yf = double(this->lastMousePos[1] - this->currentMousePos[1]) /
		double(this->height()) * this->trackballFactor * 30;

	/*	this->interactionActor->AddPosition( this->vpNormal[0] * yf,this->vpNormal[1] * yf,this->vpNormal[2] * yf );
		if (this->secondActor!=NULL)
			this->secondActor->AddPosition( this->vpNormal[0] * yf,this->vpNormal[1] * yf,this->vpNormal[2] * yf );
	 */

	 /////////////////////////////////////////////////////////////
	if ( (InteractWithCuttingTool == true))
	{
		this->interactionActor->AddPosition(this->vpNormal[0] * yf, this->vpNormal[1] * yf, this->vpNormal[2] * yf);
		if (this->secondActor != NULL)
			this->secondActor->AddPosition(this->vpNormal[0] * yf, this->vpNormal[1] * yf, this->vpNormal[2] * yf);
	}
	else
	{
		
		emit signalSendSurfaceZoom(this->vpNormal, yf);
		/*if (SurfaceDataList->GetNumberOfItems() >= 1)
		{
			int Nr = SurfaceDataList->GetNumberOfItems();
			int i;
			PolygonSurfaceData* Data;

			for (i = 0;i < Nr;i++)
			{
				Data = SurfaceDataList->GetItemAtPos(i)->GetData();
				if (Data->GetTreeItem()->isSelected() == true)
				{
					Data->GetVActor()->AddPosition(this->vpNormal[0] * yf, this->vpNormal[1] * yf, this->vpNormal[2] * yf);
					if (Data->GetCuttingEdgeActor() != NULL)
						Data->GetCuttingEdgeActor()->AddPosition(this->vpNormal[0] * yf, this->vpNormal[1] * yf, this->vpNormal[2] * yf);
				}
			}
		}*/

	}
	/////////////////////////////////////////////////////

	   // update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];






}


/*!
   \brief Turns an actor on the viewplane of the camera

   The actor can be set with SetInteractionActor()
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::ActorRoll()
{
	double angle;

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	// first time we do some preprocessing
	if (firstCall) {
		this->currentCamera->GetViewPlaneNormal(this->vpNormal);
		this->renCenter[0] = this->GetRenderer()->GetCenter()[0];
		this->renCenter[1] = this->GetRenderer()->GetCenter()[1];
		this->renCenter[2] = this->GetRenderer()->GetCenter()[2];
		firstCall = false;
	}

	int diffX1 = this->currentMousePos[0] - int(this->renCenter[0]);
	int diffY1 = this->currentMousePos[1] - int(this->renCenter[1]);
	int diffX2 = this->lastMousePos[0] - int(this->renCenter[0]);
	int diffY2 = this->lastMousePos[1] - int(this->renCenter[1]);

	double a1 = atan2(double(diffY1), double(diffX1));
	double a2 = atan2(double(diffY2), double(diffX2));
	angle = (a2 - a1) / (2.0 * 3.1415926535) * 360.0 / 10.0 *
		this->trackballFactor;

	/*
	   this->interactionActor->RotateWXYZ( angle, this->vpNormal[0],this->vpNormal[1],this->vpNormal[2] );
	   if (this->secondActor!=NULL)
		   this->secondActor->RotateWXYZ( angle, this->vpNormal[0],this->vpNormal[1],this->vpNormal[2] );
	 */

	 /////////////////////////////////////////////////////////////
	if ( (InteractWithCuttingTool == true))
	{
		this->interactionActor->RotateWXYZ(angle, this->vpNormal[0], this->vpNormal[1], this->vpNormal[2]);
		if (this->secondActor != NULL)
			this->secondActor->RotateWXYZ(angle, this->vpNormal[0], this->vpNormal[1], this->vpNormal[2]);
	}
	else
	{
		//double TempOrig2[3];
		//this->interactionActor->GetOrigin(TempOrig2);
		emit 		signalSendSurfaceRoll( vpNormal,  angle, NULL);
		//void	SendSurfaceRoll(double vpNormal[3], double angle);

		//if (SurfaceDataList->GetNumberOfItems() >= 1)
		//{
		//	int Nr = SurfaceDataList->GetNumberOfItems();
		//	int i;
		//	PolygonSurfaceData* Data;
		//	double TempOrig[3];
		//	double TempOrig2[3];
		//	this->interactionActor->GetOrigin(TempOrig2);

		//	for (i = 0;i < Nr;i++)
		//	{
		//		Data = SurfaceDataList->GetItemAtPos(i)->GetData();




		//		if (Data->GetTreeItem()->isSelected() == true)
		//		{
		//			////////////////////////
		//			Data->GetVActor()->GetOrigin(TempOrig);
		//			Data->GetVActor()->SetOrigin(TempOrig2);
		//			///////////////////////////
		//			Data->GetVActor()->RotateWXYZ(angle, this->vpNormal[0], this->vpNormal[1], this->vpNormal[2]);
		//			if (Data->GetCuttingEdgeActor() != NULL)
		//			{
		//				Data->GetCuttingEdgeActor()->SetOrigin(TempOrig);

		//				Data->GetCuttingEdgeActor()->RotateWXYZ(angle, this->vpNormal[0], this->vpNormal[1], this->vpNormal[2]);
		//				Data->GetCuttingEdgeActor()->SetOrigin(TempOrig2);

		//			}
		//			Data->GetVActor()->SetOrigin(TempOrig);
		//		}

		//	}
		//	delete []TempOrig;
		//}

	}
	/////////////////////////////////////////////////////

	emit ActorRolled(angle);

	// update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];
}


/*!
   \brief Rotates the camera around the point, which was set with SetCameraRotation().

*/

//--------------------------------------------------------------------------



void RenderWindowInteractor::CameraRotate()
{
	double xf;
	double yf;

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	// first time we do some preprocessing
	if (firstCall) {
		int* size = this->GetRenderWindow()->GetSize();
		double* vp = this->GetRenderer()->GetViewport();

		this->deltaAzimuth = -20.0 / ((vp[2] - vp[0]) * size[0]);
		this->deltaElevation = -20.0 / ((vp[3] - vp[1]) * size[1]);
		firstCall = false;
	}

	xf = (this->currentMousePos[0] - this->lastMousePos[0]) * this->deltaAzimuth
		* this->trackballFactor;
	yf = (this->lastMousePos[1] - this->currentMousePos[1]) * this->deltaElevation
		* this->trackballFactor;

	this->currentCamera->Azimuth(xf);
	this->currentCamera->Elevation(yf);
	this->currentCamera->OrthogonalizeViewUp();
	this->GetRenderer()->ResetCameraClippingRange();

	// light follows the camera
	this->currentLight->SetPosition(this->currentCamera->GetPosition());
	this->currentLight->SetFocalPoint(this->currentCamera->GetFocalPoint());

	// update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];

	emit SendCurrentCameraRotation(xf, yf, 0);
}


/*!
   \brief Translates the camera along the viewplane.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::CameraTranslate()
{
	double newP[4];
	double viewFocus[4];
	double viewPoint[3];
	double motionVector[3];

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	this->currentCamera->GetFocalPoint(viewFocus);
	this->WorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2],
		viewFocus);
	double focalDepth = viewFocus[2];

	this->DisplayToWorld(double(this->width() / 2.0 + (this->currentMousePos[0]
		- this->lastMousePos[0])),
		double(this->height() / 2.0 - (this->currentMousePos[1]
			- this->lastMousePos[1])),
		focalDepth,
		newP);

	this->currentCamera->GetFocalPoint(viewFocus);
	this->currentCamera->GetPosition(viewPoint);

	motionVector[0] = this->trackballFactor / 10.0 * (viewFocus[0] - newP[0]);
	motionVector[1] = this->trackballFactor / 10.0 * (viewFocus[1] - newP[1]);
	motionVector[2] = this->trackballFactor / 10.0 * (viewFocus[2] - newP[2]);

	this->currentCamera->SetFocalPoint(motionVector[0] + viewFocus[0],
		motionVector[1] + viewFocus[1],
		motionVector[2] + viewFocus[2]);
	this->currentCamera->SetPosition(motionVector[0] + viewPoint[0],
		motionVector[1] + viewPoint[1],
		motionVector[2] + viewPoint[2]);

	// light follows the camera
	this->currentLight->SetPosition(this->currentCamera->GetPosition());
	this->currentLight->SetFocalPoint(this->currentCamera->GetFocalPoint());

	// update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];
}


/*!
   \brief Zooms the camera along the normal of the viewplane.

*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::CameraZoom()
{
	double yf;

	// do nothing if mouse is still on the same y position
	if (this->currentMousePos[1] == this->lastMousePos[1]) {
		return;
	}

	yf = double(this->currentMousePos[1] - this->lastMousePos[1]) /
		double(this->height()) * this->trackballFactor;
	// if yf < 0, we need to be between 0..1
	yf = pow(1.1, (double)-yf);



	if (this->currentCamera->GetParallelProjection())
	{
		this->currentCamera->
			SetParallelScale(this->currentCamera->GetParallelScale() / yf);


		emit SendCurrentCameraRotation(this->currentCamera->GetParallelScale(), -1, -1);

	}
	else
	{
		//   this->currentCamera->SetViewAngle( this->currentCamera->GetViewAngle()/yf); 
		this->currentCamera->Dolly(yf);
		this->GetRenderer()->ResetCameraClippingRange();

		emit SendCurrentCameraRotation(this->currentCamera->GetViewAngle(), -1, -1);
	}

	qDebug() << "dolly :" << yf;

	//qDebug() << "scale: " << this->currentCamera->GetParallelScale();
//	qDebug() << "Distance: " << this->currentCamera->GetDistance();


	// this->currentCamera->Dolly( yf );
	// this->GetRenderer()->ResetCameraClippingRange();

	 // update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];




}


/*!
   \brief Rotates the camera on the viewplane

   This function corresponds to a roll-operation
   like in airplanes
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::CameraRoll()
{
	double angle;

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	// first time we do some preprocessing
	if (firstCall) {
		this->renCenter[0] = this->GetRenderer()->GetCenter()[0];
		this->renCenter[1] = this->GetRenderer()->GetCenter()[1];
		this->renCenter[2] = this->GetRenderer()->GetCenter()[2];
		firstCall = false;
	}

	// calculate the angle in radians and roll the camera
	int diffX1 = this->currentMousePos[0] - int(this->renCenter[0]);
	int diffY1 = this->currentMousePos[1] - int(this->renCenter[1]);
	int diffX2 = this->lastMousePos[0] - int(this->renCenter[0]);
	int diffY2 = this->lastMousePos[1] - int(this->renCenter[1]);

	double a1 = atan2(double(diffY1), double(diffX1));
	double a2 = atan2(double(diffY2), double(diffX2));
	angle = (a2 - a1) / (2.0 * 3.1415926535) * 360.0 / 10.0 *
		this->trackballFactor;
	this->currentCamera->Roll(angle);

	// light follows the camera
	this->currentLight->SetPosition(this->currentCamera->GetPosition());
	this->currentLight->SetFocalPoint(this->currentCamera->GetFocalPoint());

	// update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];

	emit SendCurrentCameraRotation(0, 0, angle);
}


/*!
   \brief Moves the actor

   Moves the actor, which was set with SetInteractionActor()
   in the plane, which was set with SetPlane().
*/

//--------------------------------------------------------------------------
void RenderWindowInteractor::CameraUserRotate(double Azimuth, double Elevation, double Roll)
{

	this->currentCamera->Azimuth(Azimuth);
	this->currentCamera->Elevation(Elevation);
	this->currentCamera->Roll(Roll);
	this->currentCamera->OrthogonalizeViewUp();
	this->GetRenderer()->ResetCameraClippingRange();

	// light follows the camera
	this->currentLight->SetPosition(this->currentCamera->GetPosition());
	this->currentLight->SetFocalPoint(this->currentCamera->GetFocalPoint());

	// update
	this->update();//updateGL();
}

//----------------------------------------------

void RenderWindowInteractor::ActorPlaneMove()
{

	double* center;
	double dispCenter[4];
	double newP[4];
	double oldP[4];
	double p0[3], p1[3], t;
	double tkonstante = (this->normale[0]) * (this->normale[0]) +
		(this->normale[1]) * (this->normale[1]) +
		(this->normale[2]) * (this->normale[2]);

	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	center = this->interactionActor->GetCenter();
	this->WorldToDisplay(center[0], center[1], center[2], dispCenter);

	this->DisplayToWorld(double(this->currentMousePos[0]),
		double(this->height() - this->currentMousePos[1]),
		dispCenter[2],
		newP);

	this->DisplayToWorld(double(this->lastMousePos[0]),
		double(this->height() - this->lastMousePos[1]),
		dispCenter[2],
		oldP);

	oldP[3] = oldP[0] * (this->normale[0]) +
		oldP[1] * (this->normale[1]) +
		oldP[2] * (this->normale[2]);

	t = (this->ebenekonstante - oldP[3]) / tkonstante;

	p0[0] = oldP[0] + t * (this->normale[0]);
	p0[1] = oldP[1] + t * (this->normale[1]);
	p0[2] = oldP[2] + t * (this->normale[2]);

	newP[3] = newP[0] * (this->normale[0]) +
		newP[1] * (this->normale[1]) +
		newP[2] * (this->normale[2]);

	t = (this->ebenekonstante - newP[3]) / tkonstante;

	p1[0] = newP[0] + t * (this->normale[0]);
	p1[1] = newP[1] + t * (this->normale[1]);
	p1[2] = newP[2] + t * (this->normale[2]);


	/////////////////////////////////////////////////////////////
	if (InteractWithCuttingTool == true)
	{
		this->interactionActor->AddPosition(p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]);
		if (this->secondActor != NULL)
			this->secondActor->AddPosition(p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]);
	}
	else
	{
		emit 		signalActorPlaneMove(p0,p1);;

		/*if (SurfaceDataList->GetNumberOfItems() >= 1)
		{
			int Nr = SurfaceDataList->GetNumberOfItems();
			int i;
			PolygonSurfaceData* Data;

			for (i = 0;i < Nr;i++)
			{
				Data = SurfaceDataList->GetItemAtPos(i)->GetData();
				if (Data->GetTreeItem()->isSelected() == true)
				{
					Data->GetVActor()->AddPosition(p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]);
					if (Data->GetCuttingEdgeActor() != NULL)
						Data->GetCuttingEdgeActor()->AddPosition(p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]);
				}
			}
		}*/

	}
	/////////////////////////////////////////////////////

	   // update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];
}


/*!
   \brief Rolls the actor

   Rolls the actor, which was set with SetInteractionActor()
   in the plane, which was set with SetPlane().
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::ActorPlaneRoll()
{

	double angle;

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	// first time we do some preprocessing
	if (firstCall) {
		this->renCenter[0] = this->GetRenderer()->GetCenter()[0];
		this->renCenter[1] = this->GetRenderer()->GetCenter()[1];
		this->renCenter[2] = this->GetRenderer()->GetCenter()[2];
		firstCall = false;
	}

	int diffX1 = this->currentMousePos[0] - int(this->renCenter[0]);
	int diffY1 = this->currentMousePos[1] - int(this->renCenter[1]);
	int diffX2 = this->lastMousePos[0] - int(this->renCenter[0]);
	int diffY2 = this->lastMousePos[1] - int(this->renCenter[1]);

	double a1 = atan2(double(diffY1), double(diffX1));
	double a2 = atan2(double(diffY2), double(diffX2));
	angle = (a2 - a1) / (2.0 * 3.1415926535) * 360.0 / 10.0 *
		this->trackballFactor;



	///////////////////////////////////////////// 
  /*	this->interactionActor->RotateWXYZ( -angle, this->normale[0],this->normale[1],this->normale[2] );
	  if (this->secondActor!=NULL)
		  this->secondActor->RotateWXYZ( -angle, this->normale[0],this->normale[1],this->normale[2] );
  */	////////////////////////////////////////////
  /////////////////////////////////////////////////////////////
	if ((InteractWithCuttingTool == true))
	{
		this->interactionActor->RotateWXYZ(-angle, this->normale[0], this->normale[1], this->normale[2]);
		if (this->secondActor != NULL)
			this->secondActor->RotateWXYZ(-angle, this->normale[0], this->normale[1], this->normale[2]);

	}
	else
	{
		double TempOrig2[3];
		this->interactionActor->GetOrigin(TempOrig2);
	
		emit signalActorPlaneRoll(normale, angle, TempOrig2);


		//if (SurfaceDataList->GetNumberOfItems() >= 1)
		//{
		//	int Nr = SurfaceDataList->GetNumberOfItems();
		//	int i;
		//	PolygonSurfaceData* Data;

		//	double TempOrig[3];
		//	double TempOrig2[3];

		//	this->interactionActor->GetOrigin(TempOrig2);

		//	for (i = 0;i < Nr;i++)
		//	{
		//		Data = SurfaceDataList->GetItemAtPos(i)->GetData();

		//		Data->GetVActor()->GetOrigin(TempOrig);
		//		Data->GetVActor()->SetOrigin(TempOrig2);

		//		if (Data->GetTreeItem()->isSelected() == true)
		//		{
		//			//////////////////////////
		//			Data->GetVActor()->GetOrigin(TempOrig);
		//			Data->GetVActor()->SetOrigin(TempOrig2);
		//			/////////////////////////////
		//			Data->GetVActor()->RotateWXYZ(-angle, this->normale[0], this->normale[1], this->normale[2]);
		//			if (Data->GetCuttingEdgeActor() != NULL)
		//			{
		//				Data->GetCuttingEdgeActor()->SetOrigin(TempOrig2);

		//				Data->GetCuttingEdgeActor()->RotateWXYZ(-angle, this->normale[0], this->normale[1], this->normale[2]);
		//				Data->GetCuttingEdgeActor()->SetOrigin(TempOrig);

		//			}
		//			Data->GetVActor()->SetOrigin(TempOrig);
		//		}

		//	}

		//}

	}
	/////////////////////////////////////////////////////
		//////////////////////////////////////////////




	   // update
	this->update();//updateGL();

	// save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];

}


/*!
   \brief Does nothing.

   This is needed, when a Button should have no interaction meaning.
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::DoNothing()
{
}


/*!
   \brief Computes the display point defined with \a x, \a y and \a z to world coordinates.

   \param x x of the display point
   \param y y of the display point
   \param z z of the display point
   \param worldPoint the resulting world point (size 4 doubles).
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::DisplayToWorld(double x, double y, double z, double* worldPoint)
{
	this->GetRenderer()->SetDisplayPoint(x, y, z);
	this->GetRenderer()->DisplayToWorld();
	this->GetRenderer()->GetWorldPoint(worldPoint);

	// normalize
	if (worldPoint[3]) {
		worldPoint[0] = worldPoint[0] / worldPoint[3];
		worldPoint[1] = worldPoint[1] / worldPoint[3];
		worldPoint[2] = worldPoint[2] / worldPoint[3];
		worldPoint[3] = 1.0;
	}
}


/*!
   \brief Computes the world point defined with \a x, \a y and \a z to display coordinates.

   \param x x of the world point
   \param y y of the world point
   \param z z of the world point
   \param displayPoint the resulting display point (size 3 doubles).
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::WorldToDisplay(double x, double y, double z, double* displayPoint)
{
	this->GetRenderer()->SetWorldPoint(x, y, z, 1.0);
	this->GetRenderer()->WorldToDisplay();
	this->GetRenderer()->GetDisplayPoint(displayPoint);
}


//--------------------------------------------------------------------------





void RenderWindowInteractor::CubeActorResizeXY()
{

	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1]))
	{
		return;
	}

	double* size = CubeSize;



	if (size[0] - (this->currentMousePos[0] - this->lastMousePos[0]) > 0)
		size[0] -= this->currentMousePos[0] - this->lastMousePos[0];
	if (size[1] - (this->currentMousePos[1] - this->lastMousePos[1]) > 0)
		size[1] -= this->currentMousePos[1] - this->lastMousePos[1];

	emit CubeUpdated();

	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];
	//i update

}

//--------------------------------------------------------------------------

void RenderWindowInteractor::CubeActorResizeZ()
{


	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1]))
	{
		return;
	}

	double* size = CubeSize;

	if (size[2] - (this->currentMousePos[1] - this->lastMousePos[1]) > 0)
		size[2] -= this->currentMousePos[1] - this->lastMousePos[1];

	emit CubeUpdated();

	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];

	//i update
}

//--------------------------------------------------------------------------


void RenderWindowInteractor::CubeActorTranslate()
{
	double* center;
	double dispCenter[4];
	double newP[4];
	double oldP[4];

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	center = CubeCenter;
	this->WorldToDisplay(center[0], center[1], center[2], dispCenter);

	this->DisplayToWorld(double(this->currentMousePos[0]),
		double(this->height() - this->currentMousePos[1]),
		dispCenter[2],
		newP);

	this->DisplayToWorld(double(this->lastMousePos[0]),
		double(this->height() - this->lastMousePos[1]),
		dispCenter[2],
		oldP);



	center[0] = newP[0];
	center[1] = newP[1];
	center[2] = newP[2];



	// update


	//tu trza inny update 
   // this->update();//updateGL();
	emit CubeUpdated();
	//	emit CubePosUpdated(center);

	   // save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];
}


/*!
  \brief Zooms the actor on the normal of the viewplane

   The actor must be set with SetInteractionActor()
*/

//--------------------------------------------------------------------------

void RenderWindowInteractor::CubeActorZoom()
{
	double yf;

	// do nothing if mouse is still on the same pos
	if ((this->currentMousePos[0] == this->lastMousePos[0]) &&
		(this->currentMousePos[1] == this->lastMousePos[1])) {
		return;
	}

	// first time we do some preprocessing
	if (firstCall) {
		this->currentCamera->GetViewPlaneNormal(this->vpNormal);
		firstCall = false;
	}

	yf = double(this->lastMousePos[1] - this->currentMousePos[1]) /
		double(this->height()) * this->trackballFactor * 30;

	/* this->interactionActor->AddPosition( this->vpNormal[0] * yf,
										  this->vpNormal[1] * yf,
										  this->vpNormal[2] * yf );
  */

	double* center;
	center = CubeCenter;



	center[0] += this->vpNormal[0] * yf;
	center[1] += this->vpNormal[1] * yf;
	center[2] += this->vpNormal[2] * yf;

	// update

	 //tu trza inny update 
  //  this->update();//updateGL();
	emit CubeUpdated();
	//	emit CubePosUpdated(center);

	   // save mouse pos
	this->lastMousePos[0] = this->currentMousePos[0];
	this->lastMousePos[1] = this->currentMousePos[1];


}
//----------------------------------
void RenderWindowInteractor::CubeActorZoom(double* currentMousePos, double* lastMousePos)
{
	double yf;

	// do nothing if mouse is still on the same pos
	if ((currentMousePos[0] == lastMousePos[0]) &&
		(currentMousePos[1] == lastMousePos[1])) {
		return;
	}

	// first time we do some preprocessing
	if (firstCall) {
		this->currentCamera->GetViewPlaneNormal(this->vpNormal);
		firstCall = false;
	}

	yf = double(lastMousePos[1] - currentMousePos[1]) /
		double(this->height()) * this->trackballFactor * 30;

	/* this->interactionActor->AddPosition( this->vpNormal[0] * yf,
	this->vpNormal[1] * yf,
	this->vpNormal[2] * yf );
	*/

	/*double *center;
	center = CubeCenter;



	center[0] += this->vpNormal[0] * yf;
	center[1] += this->vpNormal[1] * yf;
	center[2] += this->vpNormal[2] * yf;*/

	// update

	//tu trza inny update 
	//  this->update();//updateGL();
	emit CubeUpdated();
	//	emit CubePosUpdated(center);


}
//--------------------------------------------------------------------------
//
//void RenderWindowInteractor::SetSurfaceDataList(LinkedList<PolygonSurfaceData*>* _SurfaceDataList)
//{
//	SurfaceDataList = _SurfaceDataList;
//}

//--------------------------------------------------------------------------

void RenderWindowInteractor::SetInteractionModeToCuttingPlane(bool Mode)
{

	InteractWithCuttingTool = Mode;
}
//---------------------
void	RenderWindowInteractor::CleanTrickyWindow()
{
	if (PointArray != NULL)
		delete PointArray;
	PointArray = NULL;poly = PointArray;
	if (grabImage != NULL)
	{
		delete grabImage;
		grabImage = NULL;
		grabImageR = NULL;
	}

	PointArraySentAlready = true;
	update();
	//	 qWarning ("trick out");
}
//-----------------------
QImage* RenderWindowInteractor::TrickyGrabWindow()
{
	//	 qWarning ("trick in");
	if (PointArray != NULL) { delete PointArray;poly = PointArray;delete grabImage; };
	PointArray = new QPolygon();poly = PointArray;
	grabImage = grabRenderWindowGL(); grabImageR = grabImage;
	PointIndex = 0;
	return grabImageR;
	PointArraySentAlready = false;

}

void RenderWindowInteractor::GrabTrickyWindow2()
{
	if (grabImage != NULL)delete grabImage;
	grabImage = grabRenderWindowGL();
	grabImageR = grabImage;
}
//---------------------------
void	RenderWindowInteractor::SlotPressMaskDefinition(int x, int y, QMouseEvent* event)
{
	MaskPaintMode = 1;
	QPoint P = event->pos();

	int PointX = P.rx();
	int PointY = P.ry();

	int rx, ry;
	int tx, ty;

	tx = PointX;
	ty = PointY;

	this->setFocus();

	if ((event->button() == Qt::RightButton) && (MaskPaintMode > 0))
	{

		//	if ((tx>=0)&&(tx<ImgSizeX)&&(ty>=0)&&(ty<ImgSizeY))
		{

			if (MaskPaintMode == 1)
			{
				if ((!(event->modifiers() & Qt::AltModifier)) || (PointArraySentAlready == true))
				{
					if (PointArray != NULL) { delete PointArray;poly = PointArray;delete grabImage; };
					PointArray = new QPolygon();poly = PointArray;
					grabImage = grabRenderWindowGL(); grabImageR = grabImage;
					PointIndex = 0;

					*PointArray << QPoint(PointX, PointY);
					PointArraySentAlready = false;
				}
				else
				{
					// dodaj nowy punkt i przemaluj

					if (PointArray != NULL)

					{
						this->repaint();
						PointIndex++;
						//PointArray->resize(PointIndex+1);

						*PointArray << QPoint(Point1.x(), Point1.y());
						//if ((PointX!=PointArray->point(PointIndex-1).rx())||(PointY!=PointArray->point(PointIndex-1).ry()))
						{
							//QPainter *Painter2=new QPainter(this);
							////	Painter2->setPen(CrossColor);
							//Painter2->drawPolyline(*PointArray);
							//delete Painter2;
						}
					}



				}

			}
			else if (MaskPaintMode == 2)//ellipse
			{
				Point1.setX(tx);
				Point1.setY(ty);

			}
			else if (MaskPaintMode == 3)//rectangle
			{
				Point1.setX(tx);
				Point1.setY(ty);
			}

		}
	}




}

//------------------
void	RenderWindowInteractor::SlotPressMaskMove(int x, int y, QMouseEvent* event)
{
	//	emit SignalParentPleaseSendViewID();
	MaskPaintMode = 1;
	QPoint  P = event->pos();
	int PointX = P.rx();
	int PointY = P.ry();

	int rx, ry;

	int tx, ty;

	int ImgPosX = 0;
	int ImgPosY = 0;
	tx = PointX - ImgPosX;
	ty = PointY - ImgPosY;
	int count = 0;
	if (PointArray != NULL)
	{
		count = PointArray->count();
	}




	//QMessageBox::about( 0,"move",QString::number(Button));
	//emit SendMessage(QString::number((int)event->state())+" "+QString::number(tx+ty));
	int buttons = event->buttons();
	int rb = Qt::RightButton;
	if ((event->buttons() & Qt::RightButton) && (MaskPaintMode > 0))
	{


		if (MaskPaintMode == 1)//lasso
		{
			//			if 	((tx>=0)&&(tx<ImgSizeX)&&(ty>=0)&&(ty<ImgSizeY))
			if (PointArray != NULL)

			{




				if ((PointX != PointArray->point(count - 1).rx()) || (PointY != PointArray->point(count - 1).ry()))
				{
					PointIndex++;
					*PointArray << QPoint(PointX, PointY);

					if (PointArray->count() > 1)
					{
						update();
					}

					Point1.setX(PointX);
					Point1.setY(PointY);
				}
			}
		}
		else if (MaskPaintMode > 1)
		{
			this->repaint();
			Point2.setX(tx);
			Point2.setY(ty);

			int x1, y1, x2, y2;
			x1 = Point1.x() + ImgPosX;
			x2 = Point2.x() + ImgPosX;
			y1 = Point1.y() + ImgPosY;
			y2 = Point2.y() + ImgPosY;

			int x, w, y, h;

			if (x1 < x2)
			{
				x = x1;w = x2 - x;
			}
			else
			{
				x = x2;w = x1 - x;
			}

			if (y1 < y2)
			{
				y = y1;h = y2 - y;
			}
			else
			{
				y = y2;h = y1 - y;
			}


			if (event->modifiers() & Qt::ShiftModifier) // rowne
			{


				if (w < h)
				{
					if (y2 <= y1)
						y += (h - w);
					h = w;
				}
				else
				{
					if (x2 <= x1)
						x += (w - h);
					w = h;
				}
			}
			if (event->modifiers() & Qt::AltModifier) // center
			{
				x = x1 - w;
				y = y1 - h;
				w *= 2;
				h *= 2;


			}

			//PointPos.setX(x-ImgPosX);
			//PointPos.setY(y-ImgPosY);
			//PointSize.setX(w);
			//PointSize.setY(h);

			//QPainter *Painter2=new QPainter(this);
			//Painter2->setClipRect ( ImgPosX, ImgPosY,ImgSizeX,ImgSizeY);
			//Painter2->setPen(CrossColor);

			//if (MaskPaintMode==2)//ellipse
			//{
			//	Painter2->drawEllipse(x,y,w,h);
			//}
			//else if	(MaskPaintMode==3)//rectangle
			//{
			//	Painter2->drawRect(x,y,w,h);
			//}
			//delete Painter2;
		}

	}
}
//-------------------
void	RenderWindowInteractor::SlotPressMaskRelease(int x, int y, QMouseEvent* event)
{
	qWarning() << ("Release");
	MaskPaintMode = 1;
	//	QMessageBox::about( 0,"release","");
	//	emit SignalParentPleaseSendViewID();
	this->setFocus();
	int ImgPosX = 0;
	int ImgPosY = 0;
	if ((Qt::RightButton) && (MaskPaintMode > 0))
	{
		double fx, fy;
		fx = 1.0;
		fy = 1.0;
		/*if (SendMaskMode==false)
		{
			fx*=m_dx;
			fy*=m_dy;
		}*/

		int i;

		if ((MaskPaintMode == 1) && !(event->modifiers() & Qt::AltModifier))//lasso
		{
			if (PointArraySentAlready == false)
			{

				int qps = PointArray->size();
				for (i = 0;i < qps;i++)
					PointArray->setPoint(i, (int)(fx * (PointArray->point(i).x() - ImgPosX) + 0.5), this->height() - (int)(fy * (PointArray->point(i).y() - ImgPosY) + 0.5));


				/*QImage img(width(),height(),QImage::Format_RGB32);

				memset(img.bits(),255,width()*height()*4);
				QPainter pt(&img);
				pt.drawPolygon(*PointArray);
				QString file=QDir::current().path()+"/pic.png";
				img.save(file,"PNG");*/

				QPolygon arr(*PointArray);

				if (qps > 3)
					emit signalSendPointArray(arr);

				PointArraySentAlready = true;

				if (SendMaskMode == true)
				{
					/*QPixmap Pixmap(m_rx,m_ry,32);
					Pixmap.fill(Qt::black);
					QPainter *Painter2=new QPainter(&Pixmap);
					Painter2->setBrush(Qt::white);
					Painter2->setPen(1);
					Painter2->drawPolygon(*PointArray,true);
					delete Painter2;
					QImage Image=(Pixmap.convertToImage());*/
					//emit SignalSendMask((unsigned char*)(Image.bits()));
					//emit SignalSendMask((unsigned char*)(Image.convertDepth(8)).bits());
	//				emit	SignalSendMaskImage((Image));

					//Pixmap.save("c:\\pic.png","PNG");
					//Image.save("c:\\pic2.png","PNG");

				}


				// to na chwile!!!!!!

				delete PointArray;
				PointArray = NULL;poly = PointArray;
				if (grabImage != NULL)
				{
					delete grabImage;
					grabImage = NULL;
					grabImageR = NULL;
				}
				//				emit ImageReleased(); //tmp
			}
		}
		//		else if (MaskPaintMode>1)
		//		{
		//			
		//			if  ((PointPos.x()!=-1)&&(PointPos.y()!=-1)&&(PointSize.x()!=-1)&&(PointSize.y()!=-1))
		//			{
		//
		//				int x,y,w,h;
		//				
		//				x=Round((fx*PointPos.x()));
		//				y=Round((fy*PointPos.y()));
		//				w=Round((fx*PointSize.x()));
		//				h=Round((fy*PointSize.y()));
		//
		//				QPixmap Pixmap(this->width(),this->height());
		//				Pixmap.fill(Qt::black);
		//				QPainter *Painter2=new QPainter(&Pixmap);	
		//				Painter2->setBrush(Qt::white);
		//				Painter2->setPen(1);	
		////				Painter2->setPen(CrossColor);
		//
		//				if (MaskPaintMode==2)//ellipse
		//				{
		//					Painter2->drawEllipse(x,y,w,h);
		//				}
		//				else if	(MaskPaintMode==3)//rectangle
		//				{
		//					Painter2->drawRect(x,y,w,h);
		//				}
		//				
		//				
		//				delete Painter2;
		//				QImage Image=Pixmap.convertToImage();
		//				//Image.save("c:\\pic2.png","PNG");
		//				//SendMaskMode
		//				//emit SignalSendMask((unsigned char*)(Image.convertDepth(8)).bits());
		////				emit	SignalSendMaskImage(Image);//(Image.convertDepth(8)));
		//				
		//	//			emit ImageReleased(); //tmp
		//					
		//				PointPos.setX(-1);
		//				PointPos.setY(-1);
		//				PointSize.setX(-1);
		//				PointSize.setY(-1);
		//			}
		//		}


	}






	/*
	if (event->state()&LeftButton)
		{	Button=2;}
	else if
		(event->state()&RightButton)
		{	Button=5;}
	else if
		(event->state()&MidButton)
		{	Button=8;}*/


		//emit ImageReleased(Button);


	QPoint P = event->pos();
	int PointX = P.rx();
	int PointY = P.ry();
}
//-----------------
void	RenderWindowInteractor::SlotResizeRenderWindow(int w, int h)
{
	//ForceResizeGL(w, h);
	update();
}

void RenderWindowInteractor::mainHandPress(int x, int y, Qt::MouseButton button, bool dbl)
{
	QMouseEvent ev(dbl ? QEvent::MouseButtonDblClick : QEvent::MouseButtonPress, QPoint(x, y), button, button, 0);
	mousePressEvent(&ev);
}

void RenderWindowInteractor::mainHandRelease(int x, int y, Qt::MouseButton button)
{
	QMouseEvent ev(QEvent::MouseButtonRelease, QPoint(x, y), button, button, 0);
	mouseReleaseEvent(&ev);
}

void RenderWindowInteractor::mainHandMoved(int x, int y, Qt::MouseButton button)
{
	QMouseEvent ev(QEvent::MouseMove, QPoint(x, y), button, button, 0);
	mouseMoveEvent(&ev);
}
//---------------------------------------------
void RenderWindowInteractor::ClearPointArray()
{
	if (PointArray != NULL)
	{
		PointArray->clear();
	}
}
//--------------------------------
void RenderWindowInteractor::keyReleaseEvent(QKeyEvent* event)
{
	QString txt = event->text();
	qDebug() << "klawisz" << txt;

	if (event->modifiers() & Qt::ControlModifier)
	{
		qDebug() << "puscilem klawisz ctrl renderwindow";
	}

	if (event->modifiers() & Qt::AltModifier)
		// (QApplication::keyboardModifiers() & Qt::AltModifier)
	{
		//emit signalRenderWindowInteractorAltReleased();

	}
	emit signalRenderWindowInteractorReleased(event);
}