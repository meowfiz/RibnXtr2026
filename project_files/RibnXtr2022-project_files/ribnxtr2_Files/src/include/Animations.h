#ifndef	ANIMATION_H
#define	ANIMATION_H

#include <qwidget.h>
#include <qtextdocument.h>


#include <math.h>
#include <stdlib.h>

#include <vtkVolume.h>
#include <vtkLightCollection.h>
#include <vtkLight.h>
#include <vtkSpline.h>
#include <vtkCardinalSpline.h>
#include <vtkKochanekSpline.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkDepthSortPolyData.h>



#include "LinkedList.h"
#include "VtkWidget.h"
#include "HistWidget.h"
#include "SurfaceCollection.h"
#include "GUIPanelWidget.h"

//class StereoViewWidget;

class AnimationText :public QObject
{

	Q_OBJECT

	public:
		AnimationText(VtkWidget* vW, QString txt, double time,int _pos[2], int fs, double _color[3],double _timeout=0,vtkTextActor* pActor = NULL)
		{
			vWidget = vW; 
			timeV = time;
			prevTextActor = pActor;
			text = txt;
			fontSize = fs;
			pos[0] = _pos[0];	pos[1] = _pos[1];
			color[0] = _color[0];color[1] = _color[1];color[2] = _color[2];
			timeout = _timeout;
		
		};
		void ShowText();
		void HideText();
		void SetPrevActor(vtkTextActor* pActor) { prevTextActor = pActor; };
		vtkTextActor* GetTextActor() { return textActor; };


		double  GetTimeValue() { return timeV; };
		double* GetColor() { return color; };
		int* GetPos() { return pos; };
		int GetTimeout() { return timeout; };
		QString GetText() { return text; };
		int GetFontSize() { return fontSize; };

	private:
		vtkTextActor *prevTextActor;
		vtkSmartPointer < vtkTextActor> textActor;
		QTimer timer;
	
	VtkWidget* vWidget;
	int pos[2];
	int timeout;
	double color[3];
	double timeV;// calosc keyframe nr, ulamek % pomiedzy klatkami
	QString text;
	int fontSize;

public slots:
	void SlotTimeout();
	

};

class AnimationKeyPoint
{

	public:
		AnimationKeyPoint(int Nr);
		~AnimationKeyPoint();
		AnimationKeyPoint(const AnimationKeyPoint&);


		void AnimationKeyPoint::Copy(const	AnimationKeyPoint& Data, bool copydata);
		void	UpdateNumberOfSurfaces(int Nr);

			// camera
	/*	QString text;
		int textPos[4];
		double textColor[3];*/


		int		CameraSubStep;
		double	CameraPoint[3];
		double 	CameraViewUpPoint[3];
		double	CameraFocalPoint[3]; 
		double 	Background3DColorPoint[3];
		
		int		backgroundMode;
		double	BackgroundColor1[3];
		double	BackgroundColor2[3];
		QString skyboxName;

		double viewAngle;


		int	LeftVolumeThreshold;
		int	RightVolumeThreshold;
		double 	LeftVolumeOpacity;
		double 	RightVolumeOpacity;

		// surfaces...
		int	NumberOfSurfaces;

		char	*RepresentationPoints;

		double	*ColorPoints;
		double	*SpecularColorPoints;
		double	*DiffuseColorPoints;
		double	*AmbientColorPoints;

		double	*OpacityPoints;

		double	*SpecularValuePoints;
		double	*SpecularPowerPoints;
		double	*DiffuseValuePoints;
		double	*AmbientValuePoints;

	


		double	*ActorOriginPoints;
		double	*ActorOrientationPoints;
		double	*ActorPositionPoints;

		// volume palette
		int	PaletteNr;

};


class Animations : public QObject
{

	Q_OBJECT

public:

	Animations(GUIPanelWidget* panel,VtkWidget* _VWidget, vtkVolume* _Volume, HistWidget* _Histogram,
		LinkedList<PolygonSurfaceData*>	*_SurfaceDataList,int *_PalNr,
		 unsigned char *_PalTab,PaletteRGBA *_Pal,int NumberOfSubSteps=30);
	~Animations(void);
	

	void	AddSubtitleLine();

	void	InsertNewItem(int Nr);
	void	DeleteCurrentItem();
	void	DeleteAllItems();
	void	MoveCurrentItemUp();
	void	MoveCurrentItemDown();
	void	UpdateCurrentItem(int Nr);
	void	UpdateAllSubsteps();
	void	SetNextItem(); 
	void	SetPreviousItem(); 
	void	SetItemAtPos(int pos);
	void	GenerateAnimationSequence(bool LoopAnimation,int loopVisibility,int series,int *indexActicveTab,int nrOfActive,bool smoothAnim=true, int startKeyFrame=0);

	bool	SaveAnimationSequence(const QString &filename);
	bool	LoadAnimationSequence(const QString &filename);

	bool	SaveAnimationSequence2(const QString &filename);
	bool	LoadAnimationSequence2(const QString &filename);
	
	
	void	SlotMoveCameraPosAndViewUp(double *PosPoint,double *ViewUpPoint,double *FocalPoint, AnimationKeyPoint* Item, int substep=-1);
	void	SetNumberOfSubSteps(int _Nr);

	int		GetNumberOfItems();
	int		GetCurrentItemNumber();

	void	SetUpdateMode(bool val);
	double	EvaluateHerimteSpline(double val,double *tab,int Size,bool loop);

	//void	SetStereoViewWidget(StereoViewWidget *w) { StereoWidget = w; }
	void	SetGrabAnimMode(bool mode) { grabAnimMode = mode; };
	void	SetAnimOnlyOpacityOfGroups(bool a) { animOnlyOpacityOfGroups = a; };
	void	SetUpdateVolume(bool a) { updateVolume = a; };




private:

	LinkedList < AnimationText*>* GenerateTextList();

	double currentT;
	GUIPanelWidget* GuiPanel;
	int nrOfSavedGroupNames;
	QString* savedGroupNames;
	bool	updateVolume;
	bool	animOnlyOpacityOfGroups;
	bool	grabAnimMode;
	bool	UpdateCamOnly;
	void	SetItemParameters(AnimationKeyPoint *Item);
	void	SlotSetVolumeOpacity();
	void	SlotSetVolumeColors();
	void	SetupSceneWithCurrentItemParameters();

	LinkedList<AnimationKeyPoint*>	*KeyPointList;
	LinkedList<PolygonSurfaceData*>	*SurfaceDataList;
	LinkedList<PolygonSurfaceData*>	*animSurfaceDataList;

	VtkWidget		*VWidget;
	//StereoViewWidget *StereoWidget;
	HistWidget		*Histogram;
	vtkVolume		*Volume;
	int				*PalNr;
	int				NumberOfSubSteps;
	unsigned char	*PalTab;
	PaletteRGBA		*Pal;
	bool			GenerateSequenceAbort;



public slots:
	
	
	void	SlotGenerateSequenceAbort();
	void	SlotDepthSortPolygonSurface(PolygonSurfaceData *Data);

signals:

//	void SignalSendNumberOfSubSteps(int);
	void SendMessage(const QString &);
	void Signal3DFrameGenerated();
//	void SingalSendTextForGui(const QStringList list);
		


	
};

#endif //ANIMATION_H
