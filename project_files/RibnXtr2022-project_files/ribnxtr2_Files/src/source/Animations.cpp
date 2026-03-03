#include "Animations.h"
#include "StereoViewWidget.h"

void AnimationText::ShowText()
{
	prevTextActor = vWidget->GetLastActor();
	if (prevTextActor!=NULL)vWidget->RemoveTextActor(prevTextActor);

	if (timeout > 0)
	{
		timer.setSingleShot(true);
		connect(&timer, SIGNAL(timeout()), this, SLOT(SlotTimeout()));
		timer.start(timeout);

	}
	textActor= vtkSmartPointer<vtkTextActor>::New();
	textActor->SetInput(text.toLocal8Bit());
	
	textActor->SetPosition(pos[0], pos[1]);
	textActor->GetTextProperty()->SetFontSize(fontSize);
	textActor->GetTextProperty()->SetColor(color);



	vWidget->AddTextActor(textActor);



}


//--------------
void AnimationText::HideText()
{
	vWidget->RemoveTextActor(textActor);
}
//------------
void AnimationText::SlotTimeout()
{
	HideText();
}
//--------------------------------------------------------------------
AnimationKeyPoint::AnimationKeyPoint(const AnimationKeyPoint& kp)
{

	Copy(kp, true);

}
//---------------------------------------------------------------------
void AnimationKeyPoint::Copy(const	AnimationKeyPoint& kp, bool copyData)
{


	//text = kp.text;



	//CameraSubStep = kp.CameraSubStep;
	//for (int i = 0;i < 4;i++)
	//{
	//	textPos[i] = kp.textPos[i];
	//}

	for (int i = 0;i < 3;i++)
	{
		//textColor[i] = kp.textColor[i];

		CameraPoint[i] = kp.CameraPoint[i];
		CameraViewUpPoint[i] = kp.CameraViewUpPoint[i];
		CameraFocalPoint[i] = kp.CameraFocalPoint[i];
		Background3DColorPoint[i] = kp.Background3DColorPoint[i];
		BackgroundColor1[i] = kp.BackgroundColor1[i];
		BackgroundColor2[i] = kp.BackgroundColor2[i];
		backgroundMode = kp.backgroundMode;
		skyboxName = kp.skyboxName;
		viewAngle = kp.viewAngle;
	}

	LeftVolumeThreshold = kp.LeftVolumeThreshold;
	RightVolumeThreshold = kp.RightVolumeThreshold;
	LeftVolumeOpacity = kp.LeftVolumeOpacity;
	RightVolumeOpacity = kp.RightVolumeOpacity;
	PaletteNr = kp.PaletteNr;

	// surfaces...
	if (copyData == true)
	{
		AnimationKeyPoint(kp.NumberOfSurfaces);
		NumberOfSurfaces = kp.NumberOfSurfaces; //trick 

		int Nr = NumberOfSurfaces;
		int Nr3 = 3 * Nr;

		memcpy(ColorPoints, kp.ColorPoints, Nr3 * sizeof(double));
		memcpy(SpecularColorPoints, kp.SpecularColorPoints, Nr3 * sizeof(double));
		memcpy(DiffuseColorPoints, kp.DiffuseColorPoints, Nr3 * sizeof(double));
		memcpy(AmbientColorPoints, kp.AmbientColorPoints, Nr3 * sizeof(double));
		memcpy(ActorOriginPoints, kp.ActorOriginPoints, Nr3 * sizeof(double));
		memcpy(ActorOrientationPoints, kp.ActorOrientationPoints, Nr3 * sizeof(double));
		memcpy(ActorPositionPoints, kp.ActorPositionPoints, Nr3 * sizeof(double));


		memcpy(OpacityPoints, kp.OpacityPoints, Nr * sizeof(double));
		memcpy(AmbientValuePoints, kp.AmbientValuePoints, Nr * sizeof(double));
		memcpy(SpecularValuePoints, kp.SpecularValuePoints, Nr * sizeof(double));
		memcpy(SpecularPowerPoints, kp.SpecularPowerPoints, Nr * sizeof(double));
		memcpy(DiffuseValuePoints, kp.DiffuseValuePoints, Nr * sizeof(double));
		memcpy(RepresentationPoints, kp.RepresentationPoints, Nr * sizeof(char));




	}
}

AnimationKeyPoint::AnimationKeyPoint(int Nr)
{
	int i;
	CameraSubStep = 0;
	//text = "";

	//int sizes[] = { 50,100,50,0 };
	//for (i = 0;i < 4;i++)
	//{
	//	textPos[i] = sizes[i];
	//}

	for (i = 0;i < 3;i++)
	{
		//textColor[i] = 0;

		CameraPoint[i] = 0;
		CameraViewUpPoint[i] = 0;
		CameraFocalPoint[i] = 0;
		Background3DColorPoint[i] = 0;
		BackgroundColor1[i] = 0;
		BackgroundColor2[i] = 0;
		backgroundMode = 1;
		skyboxName = "";
		viewAngle = 60;
	}

	LeftVolumeThreshold = 0;
	RightVolumeThreshold = 0;
	LeftVolumeOpacity = 0;
	RightVolumeOpacity = 0;
	PaletteNr = 0;

	// surfaces...
	NumberOfSurfaces = -1; //trick 


	AmbientValuePoints = NULL;
	SpecularValuePoints = NULL;
	SpecularPowerPoints = NULL;
	DiffuseValuePoints = NULL;
	OpacityPoints = NULL;
	SpecularColorPoints = NULL;
	DiffuseColorPoints = NULL;
	AmbientColorPoints = NULL;
	ColorPoints = NULL;
	ActorOriginPoints = NULL;
	ActorOrientationPoints = NULL;
	ActorPositionPoints = NULL;

	UpdateNumberOfSurfaces(Nr);




}

//-----------------------------------

AnimationKeyPoint::~AnimationKeyPoint()
{
	if (AmbientValuePoints != NULL)		delete[]AmbientValuePoints;
	if (SpecularValuePoints != NULL)		delete[]SpecularValuePoints;
	if (SpecularPowerPoints != NULL)		delete[]SpecularPowerPoints;
	if (DiffuseValuePoints != NULL)		delete[]DiffuseValuePoints;

	if (OpacityPoints != NULL)			delete[]OpacityPoints;

	if (SpecularColorPoints != NULL)		delete[]SpecularColorPoints;
	if (DiffuseColorPoints != NULL)		delete[]DiffuseColorPoints;
	if (AmbientColorPoints != NULL)		delete[]AmbientColorPoints;

	if (ColorPoints != NULL)				delete[]ColorPoints;



	if (ActorOriginPoints != NULL)		delete[]ActorOriginPoints;
	if (ActorOrientationPoints != NULL)	delete[]ActorOrientationPoints;
	if (ActorPositionPoints != NULL)		delete[]ActorPositionPoints;
}
//-----------------------------------

void AnimationKeyPoint::UpdateNumberOfSurfaces(int Nr)
{
	if (Nr != NumberOfSurfaces)
	{
		////////////
		if (AmbientValuePoints != NULL)		delete[]AmbientValuePoints;
		if (SpecularValuePoints != NULL)		delete[]SpecularValuePoints;
		if (SpecularPowerPoints != NULL)		delete[]SpecularPowerPoints;
		if (DiffuseValuePoints != NULL)		delete[]DiffuseValuePoints;
		///////////////
		if (OpacityPoints != NULL)			delete[]OpacityPoints;
		///////////////////
		if (SpecularColorPoints != NULL)		delete[]SpecularColorPoints;
		if (DiffuseColorPoints != NULL)		delete[]DiffuseColorPoints;
		if (AmbientColorPoints != NULL)		delete[]AmbientColorPoints;
		//////////////////
		if (ColorPoints != NULL)				delete[]ColorPoints;

		if (ActorOriginPoints != NULL)		delete[]ActorOriginPoints;
		if (ActorOrientationPoints != NULL)	delete[]ActorOrientationPoints;
		if (ActorPositionPoints != NULL)		delete[]ActorPositionPoints;
		if (Nr > 0)
		{
			int Nr3 = 3 * Nr;
			NumberOfSurfaces = Nr;
			ColorPoints = new double[Nr3];
			SpecularColorPoints = new double[Nr3];
			DiffuseColorPoints = new double[Nr3];
			AmbientColorPoints = new double[Nr3];


			OpacityPoints = new double[Nr];
			AmbientValuePoints = new double[Nr];
			SpecularValuePoints = new double[Nr];
			SpecularPowerPoints = new double[Nr];
			DiffuseValuePoints = new double[Nr];
			RepresentationPoints = new char[Nr];



			ActorOriginPoints = new double[Nr3];
			ActorOrientationPoints = new double[Nr3];

			ActorPositionPoints = new double[Nr3];

			int i;

			for (i = 0;i < Nr;i++)
			{
				OpacityPoints[i] = 0;
				AmbientValuePoints[i] = 0;
				SpecularValuePoints[i] = 0;
				SpecularPowerPoints[i] = 0;
				DiffuseValuePoints[i] = 0;
				RepresentationPoints[i] = 2;


			}

			for (i = 0;i < Nr3;i++)
			{
				ColorPoints[i] = 0;
				ActorOriginPoints[i] = 0;
				ActorOrientationPoints[i] = 0;
				ActorPositionPoints[i] = 0;
				SpecularColorPoints[i] = 0;
				DiffuseColorPoints[i] = 0;
				AmbientColorPoints[i] = 0;

			}
		}
		else
		{
			NumberOfSurfaces = 0;
			ColorPoints = NULL;
			SpecularColorPoints = NULL;;
			DiffuseColorPoints = NULL;
			AmbientColorPoints = NULL;
			OpacityPoints = NULL;
			AmbientValuePoints = NULL;
			SpecularValuePoints = NULL;
			SpecularPowerPoints = NULL;
			DiffuseValuePoints = NULL;
			RepresentationPoints = NULL;
			ActorOriginPoints = NULL;
			ActorOrientationPoints = NULL;
			ActorPositionPoints = NULL;
		}
	}


}

//-----------------------------------

Animations::Animations(GUIPanelWidget* panel, VtkWidget* _VWidget, vtkVolume* _Volume, HistWidget* _Histogram,
	LinkedList<PolygonSurfaceData*>* _SurfaceDataList, int* _PalNr,
	unsigned char* _PalTab, PaletteRGBA* _Pal,
	int _NumberOfSubSteps)
{
	GuiPanel = panel;
	savedGroupNames = NULL;
	updateVolume = false;
	animOnlyOpacityOfGroups = false;
	grabAnimMode = false;
	VWidget = _VWidget;
	Volume = _Volume;
	Histogram = _Histogram;
	SurfaceDataList = _SurfaceDataList;
	KeyPointList = new LinkedList<AnimationKeyPoint*>;

	PalTab = _PalTab;
	Pal = _Pal;
	NumberOfSubSteps = _NumberOfSubSteps;

	//StereoWidget = NULL;

	PalNr = _PalNr;
	GenerateSequenceAbort = false;
	UpdateCamOnly = false;
	animSurfaceDataList = new LinkedList<PolygonSurfaceData*>;
}

//-----------------------------------

Animations::~Animations(void)
{
	if (animSurfaceDataList->GetNumberOfItems() > 0)
	{
		for (int i = 0;i < animSurfaceDataList->GetNumberOfItems();i++)
		{
			animSurfaceDataList->GetItemAtPos(i)->SetData(new PolygonSurfaceData());
		}
		animSurfaceDataList->DeleteAllItems();
		delete animSurfaceDataList;
	}

}

//-----------------------------------
void Animations::SlotMoveCameraPosAndViewUp(double* PosPoint, double* ViewUpPoint, double* FocalPoint, AnimationKeyPoint* Item,int substep)
{
	if (VWidget != NULL)
	{
		vtkRenderer* renderer = VWidget->Renderer;
		vtkCamera* camera = VWidget->Renderer->GetActiveCamera();
		vtkLightCollection* lightCollection = renderer->GetLights();
		lightCollection->InitTraversal();
		vtkLight* light = lightCollection->GetNextItem();

		camera->SetPosition(PosPoint);
		camera->SetViewUp(ViewUpPoint);
		camera->SetFocalPoint(FocalPoint);

		light->SetPosition(PosPoint);
		light->SetFocalPoint(FocalPoint);

		/*if ((Item->textPos[3] == substep)||(substep==-1))
		{
			if (Item->text.isEmpty())
			{
				VWidget->hideTextActor();
			}
			else
			{
				VWidget->setTextActor(Item->text, Item->textPos, Item->textPos[2], Item->textColor);
			}
		}*/

	}
}

//-----------------------------------
void Animations::SetItemParameters(AnimationKeyPoint* Item)
{

	int i;
	int k;

	Item->PaletteNr = *PalNr;
	Item->CameraSubStep = NumberOfSubSteps;

	double col[3], col1[3], col2[3];
	//	double val;
	//	int tmp;
	double tol = 1e-5;
	vtkCamera* camera = VWidget->Renderer->GetActiveCamera();
	//tmp=VWidget->GetBackgroundMode();
	VWidget->getBackgroundColor(col);
	VWidget->getGradientBackgroundColor1(col1);
	VWidget->getGradientBackgroundColor2(col2);
	Item->viewAngle = VWidget->getViewAngle();
	for (k = 0;k < 3;k++)
	{
		*(Item->CameraPoint + k) = camera->GetPosition()[k];
		*(Item->CameraViewUpPoint + k) = camera->GetViewUp()[k];
		*(Item->CameraFocalPoint + k) = camera->GetFocalPoint()[k];
		Item->Background3DColorPoint[k] = col[k];
		Item->BackgroundColor1[k] = col1[k];
		Item->BackgroundColor2[k] = col2[k];

	}
	Item->backgroundMode = VWidget->GetBackgroundMode();
	Item->skyboxName = VWidget->GetSkyboxName();


	//Item->text = GuiPanel->animTxtLineEdit->text();
	//int pos = 0;
	//for (int i = 0;i < 4;i++) Item->textPos[i] = GuiPanel->animTextPosLineEdit[i]->text().toInt();

	//for (int i = 0;i < 3;i++) Item->textColor[i] = GuiPanel->animTextColorLineEdit[i]->text().toInt();



	//if (Item->backgroundMode==0)	
	//{	//color
	//		
	//	VWidget->getBackgroundColor(col1);

	//	if (!compare2Doubles3(Item->Background3DColorPoint,col1,tol)) VWidget->setBackgroundColor(Item->Background3DColorPoint);
	//		
	//		
	//}
	//else if (Item->backgroundMode==1)
	//{	//gradient
	//	tmp=VWidget->GetBackgroundMode();
	//	VWidget->getGradientBackgroundColor1(col1);
	//	VWidget->getGradientBackgroundColor2(col2);

	//	if (!compare2Doubles3(Item->BackgroundColor1,col1,tol)) VWidget->setGradientBackgroundColor1(Item->BackgroundColor1);
	//	if (!compare2Doubles3(Item->BackgroundColor2,col2,tol)) VWidget->setGradientBackgroundColor2(Item->BackgroundColor2);

	//}
	//else
	//{	//skybox
	//	QString tmpskyName=VWidget->GetSkyboxName();
	//	if(((Item->skyboxName).isEmpty()!=0)&&(tmpskyName.compare(Item->skyboxName)!=0)) VWidget->SetCurrentSkyboxModel(Item->skyboxName);
	//		
	//}

	/*if (tmp!=Item->backgroundMode)
	{
			VWidget->SlotSetBackgroundMode(Item->backgroundMode);
	}*/

	Item->LeftVolumeThreshold = Histogram->GetLowerThreshold();
	Item->RightVolumeThreshold = Histogram->GetUpperThreshold();
	Item->LeftVolumeOpacity = Histogram->GetLeftY();
	Item->RightVolumeOpacity = Histogram->GetRightY();


	int m = 0;

	Item->UpdateNumberOfSurfaces(SurfaceDataList->GetNumberOfItems());

	int Representation = 0;
	const char* Representation1;


	//sprawdzam, ktora powierzchnia z surfacedatalist zmieniona
	//sprawdzam, czy jest na liscie animSurfaceDataList
	//jesli nie dodaje ja do listy
	//jeslitak pobieram parametry i dopisuje do danej pozycji z animSurfaceDataList

	for (i = 0;i < Item->NumberOfSurfaces;i++)
	{



		Representation1 = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetRepresentationAsString();
		if (strcmp(Representation1, "Surface") == 0)	Representation = 2;
		if (strcmp(Representation1, "Wireframe") == 0)	Representation = 1;
		if (strcmp(Representation1, "Points") == 0)	Representation = 0;


		for (k = 0;k < 3;k++)
		{
			*(Item->ColorPoints + m + k) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetColor()[k];
			*(Item->SpecularColorPoints + m + k) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetSpecularColor()[k];
			*(Item->DiffuseColorPoints + m + k) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetDiffuseColor()[k];
			*(Item->AmbientColorPoints + m + k) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetAmbientColor()[k];




			*(Item->ActorOriginPoints + m + k) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVActor()->GetOrigin()[k];
			*(Item->ActorOrientationPoints + m + k) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVActor()->GetOrientation()[k];
			*(Item->ActorPositionPoints + m + k) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVActor()->GetPosition()[k];
		}
		*(Item->OpacityPoints + i) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetOpacity();
		*(Item->AmbientValuePoints + i) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetAmbient();
		*(Item->SpecularValuePoints + i) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetSpecular();
		*(Item->SpecularPowerPoints + i) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetSpecularPower();
		*(Item->DiffuseValuePoints + i) = SurfaceDataList->GetItemAtPos(i)->GetData()->GetVProperty()->GetDiffuse();
		*(Item->RepresentationPoints + i) = Representation;

		m += 3;
	}

}

//-------------------------------------------------------------------

void Animations::SetUpdateMode(bool val)
{
	UpdateCamOnly = val;

}

//-------------------------------------------------------------------

void Animations::InsertNewItem(int Nr)
{
	NumberOfSubSteps = Nr;
	AnimationKeyPoint* Item = new AnimationKeyPoint(SurfaceDataList->GetNumberOfItems());
	SetItemParameters(Item);
	KeyPointList->InsertItem(Item);
}

//-----------------------------------
void Animations::DeleteCurrentItem()
{
	KeyPointList->DeleteItem();
}
//-----------------------------------
void Animations::DeleteAllItems()
{
	KeyPointList->DeleteAllItems();

	if (savedGroupNames != NULL) { delete[]savedGroupNames;savedGroupNames = NULL; }

}
//-----------------------------------
int	Animations::GetNumberOfItems()
{
	return KeyPointList->GetNumberOfItems();
}
//-----------------------------------
int		Animations::GetCurrentItemNumber()
{
	return KeyPointList->GetCurrentItemPos();
}
//-----------------------------------
void Animations::MoveCurrentItemUp()
{
	if (KeyPointList->GetCurrentListItem() != NULL)
		KeyPointList->MoveItemUp();

}

//-----------------------------------
void Animations::MoveCurrentItemDown()
{
	if (KeyPointList->GetCurrentListItem() != NULL)
		KeyPointList->MoveItemDown();

}

//-----------------------------------

void Animations::UpdateAllSubsteps()
{
	int f = GuiPanel->NumberOfSequenceSubStepsBox->value();

	for (int i = 0;i < KeyPointList->GetNumberOfItems();i++)
	{
		KeyPointList->GetItemAtPos(i)->GetData()->CameraSubStep = f;
	}

}

void Animations::UpdateCurrentItem(int Nr)
{
	if (KeyPointList->GetCurrentListItem() != NULL)
	{
		NumberOfSubSteps = Nr;
		SetItemParameters(KeyPointList->GetCurrentListItem()->GetData());
		SetupSceneWithCurrentItemParameters();
	}
}

//-----------------------------------
void Animations::SetNextItem()
{
	if (KeyPointList->GetNumberOfItems() > 0)
	{
		if (KeyPointList->GetCurrentItemPos() < KeyPointList->GetNumberOfItems() - 1)
		{
			KeyPointList->SetNextItem();
			SetupSceneWithCurrentItemParameters();
		}
	}
}

void Animations::SetItemAtPos(int pos)
{
	if (KeyPointList->GetNumberOfItems() > 0)
	{
		if (KeyPointList->GetCurrentItemPos() != pos - 1)
		{
			KeyPointList->SetCurrentItem(pos - 1);
			SetupSceneWithCurrentItemParameters();
		}
	}
}

//-----------------------------------
void Animations::SetPreviousItem()
{
	if (KeyPointList->GetNumberOfItems() > 0)
	{
		if (KeyPointList->GetCurrentItemPos() > 0)
		{
			KeyPointList->SetPreviousItem();
			SetupSceneWithCurrentItemParameters();
		}
	}
}
//-----------------------------------

LinkedList < AnimationText*>* Animations::GenerateTextList()
{
	LinkedList < AnimationText*>* textList = new  LinkedList<AnimationText*>;
	new  LinkedList<AnimationText*>;




	QTextDocument* doc = GuiPanel->subtitleTextEdit->document();
	//QTextBlock tb = doc->findBlockByLineNumber(1); // The second line.
	//QString s = tb.text();


	int nrLines = doc->lineCount();

	for (int i = 0;i < nrLines;i++)
	{
		QTextBlock tb = doc->findBlockByLineNumber(i); // The second line.
		QString line0 = tb.text();
		QStringList fields = line0.split(",", QString::SkipEmptyParts);
		int fCount = fields.count();
		if (fCount >= 9)
		{
			double col[3];
			int pos[2];
			int fSize;
			int timeout;
			QString text;

			double timePos = fields[0].toInt() + fields[1].toDouble();
			pos[0] = fields[2].toInt();
			pos[1] = fields[3].toInt();

			fSize = fields[4].toInt();
			timeout = fields[5].toInt();
			col[0] = fields[6].toDouble();
			col[1] = fields[7].toDouble();
			col[2] = fields[8].toDouble();
			i++;


			if (i < nrLines)
			{

				QTextBlock tb1 = doc->findBlockByLineNumber(i);
				text = tb1.text();

				if (text.isEmpty() == false)
				{
					AnimationText* aText = new AnimationText(VWidget, text, timePos, pos, fSize, col, timeout);
					textList->InsertItem(aText);
				}
			}
		}




	}
	return textList;
}

//-----------------------------------------
void Animations::GenerateAnimationSequence(bool LoopAnimation, int loopVisibility, int series, int* indexActicveTab, int nrOfActive, bool smoothAnim, int startKeyFrame)
{
	LinkedList<AnimationKeyPoint*>* KeyPointListTmp = new LinkedList<AnimationKeyPoint*>;


	int ssize = 0;
	int origCount = KeyPointList->GetNumberOfItems();

	LinkedList < AnimationText*>* textList = GenerateTextList();//trzeba ja skasowac!

	if (LoopAnimation == false)
	{
		for (int i = 0;i < origCount;i++)
		{
			KeyPointListTmp->InsertItem(KeyPointList->GetItemAtPos(i)->GetData());
		}
	}
	else
	{
		ssize = 4;
		for (int i = 0;i < origCount + 2 * ssize;i++)
		{
			int m = ((i - ssize) + (origCount)*ssize) % (origCount);

			KeyPointListTmp->InsertItem(KeyPointList->GetItemAtPos(m)->GetData());
		}
	}

	QStringList groupNames;
	QList<int> groupRepresentatntPos;

	int NrOfSurfaces = SurfaceDataList->GetNumberOfItems();
	if ((animOnlyOpacityOfGroups == true) && (NrOfSurfaces > 0))
	{
		if (savedGroupNames != NULL)
		{
			// muszê na pocz¹tek zadbaæ, ¿eby do pliku z animseq by³a równie¿ zapisywana nazwa grupy!
			//1. znajdz iloœæ grup
			int groupCount = 0;


			for (int i = 0;i < nrOfSavedGroupNames;i++)
			{
				QString currentName = savedGroupNames[i];
				bool exists = false;
				for (int j = 0;j < groupNames.count();j++)
				{
					QString grName = groupNames[j];
					if ((QString::compare(grName, currentName) == 0))
					{
						exists = true;
					}
				}
				if (exists == false)
				{
					groupNames << currentName;
					groupRepresentatntPos << i;
				}

			}


			//2. znajdz pozycjê pierwszego reprezentatnta

			// done

			//3. dla ka¿dej klatki pobierz opacity dla reprezentanta i przenies na grupê

			//trzeba jednak zbudowac nowe Keypointy z jedynie opacity!!!!
			int count = KeyPointListTmp->GetNumberOfItems();
			for (int i = 0;i < count;i++)
			{
				AnimationKeyPoint* kp = KeyPointListTmp->GetItemAtPos(i)->GetData();

				//trzeba zrobic nowy - skopiowac zawartosc i wymienic powierzchnie
				AnimationKeyPoint* kpGroups = new AnimationKeyPoint(NrOfSurfaces);
				kpGroups->Copy(*kp, false);//skopiowane wszystko oprocz danych o powierzchniach - teraz musze dorobic dla nich opacity
				kpGroups->NumberOfSurfaces = NrOfSurfaces;
				//	kpGroups->OpacityPoints = new double[NrOfSurfaces];




				for (int j = 0;j < groupNames.count();j++)
				{

					int gPos = groupRepresentatntPos[j];

					double groupOpacity = kp->OpacityPoints[gPos];
					for (int k = 0;k < NrOfSurfaces;k++)
					{
						QString groupName = SurfaceDataList->GetItemAtPos(k)->GetData()->GetGroupName();
						if ((QString::compare(groupNames[j], groupName) == 0))
						{
							kpGroups->OpacityPoints[k] = groupOpacity;
						}
					}

				}
				KeyPointListTmp->GetItemAtPos(i)->SetData(kpGroups);
			}

		}
	}


	int i, j, k, l, m, n;
	GenerateSequenceAbort = false;
	int NrOfKeyFrames = KeyPointListTmp->GetNumberOfItems();
	if (NrOfKeyFrames == 0) return;



	//int LoopVisibility=params[0];
	//int NrOfTotalLoopSurfaces=params[1];
	//int NrOfFrameSurfaces=params[2];
	//int FrameStepNr=params[3];

	int		activeIndex;
	int		surfacesProcessed = nrOfActive / series;
	double* OpacityTab = new double[NrOfSurfaces];
	int		VisibilityStep = 0;

	//if ((NrOfTotalLoopSurfaces>NrOfSurfaces)||(NrOfTotalLoopSurfaces==0)) NrOfTotalLoopSurfaces=NrOfSurfaces;


	//int MaxVis=NrOfTotalLoopSurfaces/NrOfFrameSurfaces;



	// camera
	vtkSpline** Spline = new vtkSpline * [9]();

	for (j = 0;j < 9;j++)
	{
		Spline[j] = vtkCardinalSpline::New();
	}
	//rest
	int VolNr = 0;
	if (Volume != NULL) VolNr = 772;

	int NumberOfRest = 3 + VolNr + 27 * NrOfSurfaces + 6 + 1;
	vtkPiecewiseFunction** LinearFunc = new vtkPiecewiseFunction * [NumberOfRest]();

	for (j = 0;j < NumberOfRest;j++)
		LinearFunc[j] = vtkPiecewiseFunction::New();


	int NumberOfFrames = 0;
	for (j = 1;j < NrOfKeyFrames;j++)
		NumberOfFrames += KeyPointListTmp->GetItemAtPos(j)->GetData()->CameraSubStep;


	int ss = NrOfSurfaces;
	if (NrOfSurfaces > SurfaceDataList->GetNumberOfItems())
	{
		ss = SurfaceDataList->GetNumberOfItems();
	}
	//if (LoopAnimation==true)
	//{
	//	NrOfKeyFrames++;
	//	NumberOfFrames+=NumberOfSubSteps;
	//}

	//StereoWidget->animMode = true;

	/*
	double	**FTab=new double*[9];
	for (j=0;j<9;j++)
	{
	FTab[j]=new double[NumberOfFrames];
	}
	*/
	///////////////////////////////////////////////

	int pos;

	for (j = 0;j < NrOfKeyFrames;j++)
	{
		l = j;

		/*if(LoopAnimation==true)
		{
			if (j==NrOfKeyFrames-1)
				l=0;
		}*/


		//camera
		pos = 0;
		for (i = 0;i < 3;i++)
		{
			Spline[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->CameraPoint[i]);
			Spline[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->CameraViewUpPoint[i]);
			Spline[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->CameraFocalPoint[i]);

		}

		pos = 0;

		//background
		for (i = 0;i < 3;i++)
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->Background3DColorPoint[i]);
		//background c1
		for (i = 0;i < 3;i++)
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->BackgroundColor1[i]);
		//background c1
		for (i = 0;i < 3;i++)
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->BackgroundColor2[i]);
		//viewAngle
		LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->viewAngle);
		// 3D surfaces
		for (k = 0;k < ss;k++)
		{
			if (KeyPointListTmp->GetItemAtPos(l)->GetData()->NumberOfSurfaces <= k) break;
			//opacity
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->OpacityPoints[k]);

			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->AmbientValuePoints[k]);
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->SpecularValuePoints[k]);
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->SpecularPowerPoints[k]);
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->DiffuseValuePoints[k]);


			//color
			for (i = 0;i < 3;i++)
				LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->ColorPoints[k * 3 + i]);

			for (i = 0;i < 3;i++)
				LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->SpecularColorPoints[k * 3 + i]);
			for (i = 0;i < 3;i++)
				LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->DiffuseColorPoints[k * 3 + i]);
			for (i = 0;i < 3;i++)
				LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->AmbientColorPoints[k * 3 + i]);



			//orientation
			for (i = 0;i < 3;i++)
				LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->ActorOriginPoints[k * 3 + i]);
			//position
			for (i = 0;i < 3;i++)
				LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->ActorOrientationPoints[k * 3 + i]);
			//origin
			for (i = 0;i < 3;i++)
				LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->ActorPositionPoints[k * 3 + i]);

		}


		// volume opacity and thresholds
		if (Volume != NULL)
		{
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->LeftVolumeThreshold);
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->RightVolumeThreshold);
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->LeftVolumeOpacity);
			LinearFunc[pos++]->AddPoint(j, KeyPointListTmp->GetItemAtPos(l)->GetData()->RightVolumeOpacity);

			// volume palette

			for (i = 0;i < 256;i++)
			{
				LinearFunc[pos++]->AddPoint(j, PalTab[KeyPointListTmp->GetItemAtPos(l)->GetData()->PaletteNr * 768 + i]);
				LinearFunc[pos++]->AddPoint(j, PalTab[KeyPointListTmp->GetItemAtPos(l)->GetData()->PaletteNr * 768 + 256 + i]);
				LinearFunc[pos++]->AddPoint(j, PalTab[KeyPointListTmp->GetItemAtPos(l)->GetData()->PaletteNr * 768 + 512 + i]);
			}
		}

	}


	///////////////////////////////////////////////////////////////////

	int SubStep;
	int S = 0;
	int SubStepTmp;
	double t;

	double Point9[9];

	//double Point1[3],Point2[3],Point3[3];
	double* Point1 = Point9;
	double* Point2 = Point9 + 3;
	double* Point3 = Point9 + 6;


	double dist1, dist2, distX;
	double foc1[3], foc2[3], vect[3];
	double pos1[3], pos2[3];

	/*
	vtkSphereSource *Sphere=vtkSphereSource::New();
	Sphere->SetPhiResolution(6);
	Sphere->SetThetaResolution(6);

	*/
	dist1 = -1;
	dist2 = -1;


	//zebrac do tab kamere i wygladzic bo mieszam cos z promieniami

	int countCameraFrames = 0;
	S = 0;
	int* startSplineTab = new int[NrOfKeyFrames];
	for (i = 0;i < NrOfKeyFrames - 1;i++)
	{
		startSplineTab[i] = countCameraFrames;
		SubStep = KeyPointListTmp->GetItemAtPos(i + 1)->GetData()->CameraSubStep;
		SubStepTmp = SubStep;
		if (i == (NrOfKeyFrames - 2) && (LoopAnimation == false)) SubStepTmp++;
		countCameraFrames += SubStepTmp;
	}

	double* splineTab = new double[countCameraFrames * 9];
	memset(splineTab, 0, countCameraFrames * 9 * sizeof(double));
	int sPos = 0;
	for (i = 0;i < NrOfKeyFrames - 1;i++)
	{



		SubStep = KeyPointListTmp->GetItemAtPos(i + 1)->GetData()->CameraSubStep;
		SubStepTmp = SubStep;



		if (i == (NrOfKeyFrames - 2) && (LoopAnimation == false)) SubStepTmp++;
		if (smoothAnim == true)
		{

			pos1[0] = Spline[0]->Evaluate(i);
			pos1[1] = Spline[3]->Evaluate(i);
			pos1[2] = Spline[6]->Evaluate(i);

			pos2[0] = Spline[0]->Evaluate(i + 1);
			pos2[1] = Spline[3]->Evaluate(i + 1);
			pos2[2] = Spline[6]->Evaluate(i + 1);



			foc1[0] = Spline[2]->Evaluate(i);
			foc1[1] = Spline[5]->Evaluate(i);
			foc1[2] = Spline[8]->Evaluate(i);

			foc2[0] = Spline[2]->Evaluate(i + 1);
			foc2[1] = Spline[5]->Evaluate(i + 1);
			foc2[2] = Spline[8]->Evaluate(i + 1);

			dist1 = GetDistance(pos1, foc1);
			dist2 = GetDistance(pos2, foc2);



		}

		for (j = 0;j < SubStepTmp;j++)
		{
			t = i + (double)j / SubStep;

			// camera
			//-------------------------
			pos = 0;
			for (k = 0;k < 3;k++)
			{
				Point1[k] = Spline[pos++]->Evaluate(t);
				Point2[k] = Spline[pos++]->Evaluate(t);
				Point3[k] = Spline[pos++]->Evaluate(t);


				// jesli promien jest taki sam dla obu to costam
				if (smoothAnim == true)
					if ((dist1 > 0) && (fabs(dist1 - dist2) < 1e-5))
					{

						//poprawiam pos
						distX = GetDistance(Point3, Point1);

						vect[0] = Point1[0] - Point3[0];	vect[1] = Point1[1] - Point3[1];	vect[2] = Point1[2] - Point3[2];
						normV(vect);
						Point1[0] = Point3[0] + dist1 * vect[0]; Point1[1] = Point3[1] + dist1 * vect[1]; Point1[2] = Point3[2] + dist1 * vect[2];

					}
			}
			for (int l = 0;l < 9;l++)
			{
				splineTab[sPos++] = Point9[l];
			}


		}
	}
	//----------------------------------------------------------
	if (smoothAnim == true)
	{
		//camera path smoothing
		double stdDev = 2.0;
		int radius = 5;
		int					size2 = 2 * radius + 1;
		double* gaussianKernelTab = new double[size2];
		double* intensityTab = new double[size2];
		GetGaussianKernel(gaussianKernelTab, (double)radius, stdDev);
		double* splineTabTemp = new double[countCameraFrames * 3];
		
		double val;
		int poss;
		/*if (LoopAnimation==true)
		{
			radius=0;
		}*/
		int vvv = countCameraFrames * 9;
		for (i = radius;i < countCameraFrames - radius;i++)
		{

			//zbierz dane do filtrowania w zaleznosci czy loop czy nie

			for (k = 0;k < 3;k++)
			{
				for (j = 0;j < size2;j++)
				{
					poss = ((i + j - radius) * 9 + k + vvv) % vvv;
					intensityTab[j] = splineTab[poss];
				}
				val = Apply1DNormFilterF(gaussianKernelTab, intensityTab, size2);
				splineTabTemp[i * 3 + k] = val;
			}

			// przefiltruj zamien nowe pozycje

		}


		for (i = radius;i < countCameraFrames - radius;i++)
		{
			for (k = 0;k < 3;k++)
			{
				splineTab[(i) * 9 + k] = splineTabTemp[(i * 3 + k)];
			}
		}






		delete[]splineTabTemp;
		delete[]gaussianKernelTab;
		delete[]intensityTab;
		//Q[pos++]=Apply1DNormFilter(gaussianKernelTab,intensityTab,size2);	

	}
	int backMode = 1;
	QString skyName = "";
	double tol = 1e-5;
	int startAnim = startKeyFrame;
	double prevT = -1;
	//---------------------------------------------------------
	do
	{
		S = 0;
	

		vtkProperty* prop;
		vtkActor* actor;
		int oldBackMode = 1;
		//for (i=0;i<NrOfKeyFrames-1;i++)
		int loopSize = origCount;
		if (LoopAnimation == true)loopSize++;


		//sPos = ssize * 9;//+m * 9;

		for (int m = startAnim;m < loopSize - 1;m++)
		{
			GuiPanel->CurrentKeyFrameNrEdit->setText(QString::number(m));
			KeyPointList->SetCurrentItem(m);




			i = m + ssize;

			SubStep = KeyPointListTmp->GetItemAtPos(i + 1)->GetData()->CameraSubStep;
			SubStepTmp = SubStep;


			if (i == (NrOfKeyFrames - 2) && (LoopAnimation == false)) SubStepTmp++;


			AnimationKeyPoint* Item = KeyPointListTmp->GetItemAtPos(i)->GetData();
			backMode = Item->backgroundMode;
			oldBackMode = VWidget->GetBackgroundMode();

			if (m == (loopSize - 2) && (LoopAnimation == true))
			{
				SubStepTmp -= 3;
			}

		
			sPos = startSplineTab[i] * 9;//i * 9;//+m * 9;

			for (j = 0;j < SubStepTmp;j++)
			{


			/*	if (Item->textPos[3] ==j)
				{
					if (Item->text.isEmpty())
					{
						VWidget->hideTextActor();
					}
					else
					{
						VWidget->setTextActor(Item->text, Item->textPos, Item->textPos[2], Item->textColor);
					}
				}*/

				currentT = (double)j / SubStep;
				//--zarzadzanie napisami
				t = i + currentT;

				double  cT = m + currentT;
				for (int k = 0;k < textList->GetNumberOfItems();k++)
				{
					AnimationText *at = textList->GetItemAtPos(k)->GetData();
					double v = at->GetTimeValue();
					if ((prevT < v) && ((cT - v) >=0))
					{
						at->ShowText();
					}


				}

				//--koniec zarzadzania napisami

				prevT = cT;

			

				GuiPanel->CurrentTKeyFrameNrEdit->setText(QString::number(currentT));

				// camera
				//-------------------------
				pos = 0;

				SlotMoveCameraPosAndViewUp(splineTab + sPos, splineTab + sPos + 3, splineTab + sPos + 6,Item,j);
				sPos += 9;
				//-------------------------
				if (UpdateCamOnly == false)
				{
					double c[3], c1[3], c2[3];
					double cOld[3], c1Old[3], c2Old[3];
					//background
					for (l = 0;l < 3;l++)
					{
						c[l] = LinearFunc[l]->GetValue(t);
						c1[l] = LinearFunc[l + 3]->GetValue(t);
						c2[l] = LinearFunc[l + 6]->GetValue(t);

					}
					//jesli klata kluczowa zobacz czy nie trzeba zmienic bckbmode




					//if (backMode==0)	
					{	//color

						VWidget->getBackgroundColor(cOld);

						if (!compare2Doubles3(c, cOld, tol)) VWidget->setBackgroundColor(c);


					}
					//else if (backMode==1)
					{	//gradient

						VWidget->getGradientBackgroundColor1(c1Old);
						VWidget->getGradientBackgroundColor2(c2Old);

						if (!compare2Doubles3(c1Old, c1, tol)) VWidget->setGradientBackgroundColor1(c1);
						if (!compare2Doubles3(c2Old, c2, tol)) VWidget->setGradientBackgroundColor2(c2);

					}

					//tylko dla klatek kluczowych zmieniam backmode albo skyboxa
					if (t - (int)t < 1e-9)
					{
						//int tmp=VWidget->GetBackgroundMode();
						//AnimationKeyPoint *Item=KeyPointListTmp->GetItemAtPos((int)t)->GetData();

						if (backMode == 2)
						{	//skybox
							QString tmpskyName = VWidget->GetSkyboxName();
							if (((Item->skyboxName).isEmpty() == false) && (tmpskyName.compare(Item->skyboxName) != 0)) { VWidget->SetCurrentSkyboxModel(Item->skyboxName); }

						}

						if (backMode != oldBackMode)
						{
							VWidget->SlotSetBackgroundMode(Item->backgroundMode);
						}
					}


					//viewAngle

					double oldViewAngle, viewAngle;
					//oldViewAngle=VWidget->
					oldViewAngle = VWidget->getViewAngle();
					viewAngle = LinearFunc[9]->GetValue(t);
					if (!compare2Doubles(oldViewAngle, viewAngle, tol))
						VWidget->setViewAngle(viewAngle);




					double tol = 1e-5;
					//wydzielam opacity
					

					for (k = 0;k < ss;k++)
					{
						if (KeyPointListTmp->GetItemAtPos(l)->GetData()->NumberOfSurfaces <= k) break;
						pos = 26 * k + 10;
						prop = SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty();
						actor = SurfaceDataList->GetItemAtPos(k)->GetData()->GetVActor();
						double opacity, oldOpacity;
						opacity = LinearFunc[pos++]->GetValue(t);		oldOpacity = prop->GetOpacity();
						if (!compare2Doubles(opacity, oldOpacity, tol))
							prop->SetOpacity(opacity);
						OpacityTab[k] = opacity;

						if ((QString::compare(SurfaceDataList->GetItemAtPos(k)->GetData()->GetPolygonSurfaceName(), "tube 44") == 0))
						{
							k = k;
						}

					}





					if (loopVisibility == 1)
					{
						for (n = 0;n < series;n++)
							for (m = 0;m < surfacesProcessed;m++)
							{
								activeIndex = indexActicveTab[n * surfacesProcessed + m];
								if (SurfaceDataList->GetItemAtPos(activeIndex) != NULL)
									if (m == VisibilityStep)
									{
										//pokaz
										prop->SetOpacity(OpacityTab[activeIndex]);
									}
									else
									{
										//schowaj
										prop->SetOpacity(0.0);
									}


							}

						VisibilityStep++;
						if (VisibilityStep == surfacesProcessed) VisibilityStep = 0;

					}

					if (animOnlyOpacityOfGroups == false)//nie animujê nic poza przezroczystoscia
					{
						pos = 10;
						// 3D surfaces	
						for (k = 0;k < ss;k++)
						{
							if (KeyPointListTmp->GetItemAtPos(l)->GetData()->NumberOfSurfaces <= k) break;
							prop = SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty();
							actor = SurfaceDataList->GetItemAtPos(k)->GetData()->GetVActor();

							int rep, oldRep;
							//double opacity, oldOpacity;
							double ambient, oldAmbient;
							double specular, oldSpecular;
							double specularP, oldSpecularP;
							double diffuse, oldDiffuse;
							double color[3], oldColor[3];
							double specColor[3], oldSpecColor[3];
							double diffColor[3], oldDiffColor[3];
							double ambientColor[3], oldAmbientColor[3];
							double position[3], oldPosition[3];
							double orientation[3], oldOrientation[3];
							double origin[3], oldOrigin[3];

							rep = -1;
							//zmmiana reprezentacji tylko dla klatek kluczowych
							if (t - (int)t < 1e-9)
							{
								rep = *(KeyPointListTmp->GetItemAtPos((int)t)->GetData()->RepresentationPoints + k);
								oldRep = prop->GetRepresentation();
							}

							/*if (rep==2)
								prop->SetRepresentationToSurface();
							else if (Representation==1)
								prop->SetRepresentationToWireframe();
							else if (Representation==0)
								prop->SetRepresentationToPoints();*/
								//}
								//opacity




								//pobieranie
							pos++;// to za opacity skip
								//opacity=LinearFunc[pos++]->GetValue(t);		oldOpacity=prop->GetOpacity();
							ambient = LinearFunc[pos++]->GetValue(t);		oldAmbient = prop->GetAmbient();
							specular = LinearFunc[pos++]->GetValue(t);	oldSpecular = prop->GetSpecular();
							specularP = LinearFunc[pos++]->GetValue(t);	oldSpecularP = prop->GetSpecularPower();
							diffuse = LinearFunc[pos++]->GetValue(t);		oldDiffuse = prop->GetDiffuse();

							for (l = 0;l < 3;l++)	color[l] = LinearFunc[pos++]->GetValue(t);		prop->GetColor(oldColor);
							for (l = 0;l < 3;l++)	specColor[l] = LinearFunc[pos++]->GetValue(t);	prop->GetSpecularColor(oldSpecColor);
							for (l = 0;l < 3;l++)	diffColor[l] = LinearFunc[pos++]->GetValue(t);	prop->GetDiffuseColor(oldDiffColor);
							for (l = 0;l < 3;l++)	ambientColor[l] = LinearFunc[pos++]->GetValue(t);	prop->GetAmbientColor(oldAmbientColor);

							for (l = 0;l < 3;l++)	origin[l] = LinearFunc[pos++]->GetValue(t);		actor->GetOrigin(oldOrigin);
							for (l = 0;l < 3;l++)	orientation[l] = LinearFunc[pos++]->GetValue(t);	actor->GetOrientation(oldOrientation);
							for (l = 0;l < 3;l++)	position[l] = LinearFunc[pos++]->GetValue(t);		actor->GetPosition(oldPosition);
							//sprawdzanie i ew zmiana



						/*	if (!compare2Doubles(opacity,oldOpacity,tol))
								prop->SetOpacity(opacity);*/
							if (!compare2Doubles(ambient, oldAmbient, tol))
								prop->SetAmbient(ambient);
							if (!compare2Doubles(specular, oldSpecular, tol))
								prop->SetSpecular(specular);
							if (!compare2Doubles(specularP, oldSpecularP, tol))
								prop->SetSpecularPower(specularP);
							if (!compare2Doubles(diffuse, oldDiffuse, tol))
								prop->SetDiffuse(diffuse);


							if (!compare2Doubles3(color, oldColor, tol))
								prop->SetColor(color);
							if (!compare2Doubles3(specColor, oldSpecColor, tol))
								prop->SetSpecularColor(specColor);
							if (!compare2Doubles3(diffColor, oldDiffColor, tol))
								prop->SetDiffuseColor(diffColor);
							if (!compare2Doubles3(ambientColor, oldAmbientColor, tol))
								prop->SetAmbientColor(ambientColor);


							if (!compare2Doubles3(position, oldPosition, tol))
								actor->SetPosition(position);
							if (!compare2Doubles3(orientation, oldOrientation, tol))
								actor->SetOrientation(orientation);
							if (!compare2Doubles3(origin, oldOrigin, tol))
								actor->SetOrigin(origin);

							if (rep != -1)
							{
								if (rep != oldRep)
								{
									if (rep == 2)
										prop->SetRepresentationToSurface();
									else if (rep == 1)
										prop->SetRepresentationToWireframe();
									else if (rep == 0)
										prop->SetRepresentationToPoints();
								}
							}
							//color

						}
					}


					// volume opacity and thresholds
					if ((Volume != NULL) && (updateVolume == true))
					{

						Histogram->SetLowerThreshold((int)(LinearFunc[pos++]->GetValue(t)));
						Histogram->SetUpperThreshold((int)(LinearFunc[pos++]->GetValue(t)));
						Histogram->SetLeftY(LinearFunc[pos++]->GetValue(t));
						Histogram->SetRightY(LinearFunc[pos++]->GetValue(t));



						for (k = 0;k < 256;k++)
						{
							Pal->R[k] = (unsigned char)(LinearFunc[pos++]->GetValue(t));
							Pal->G[k] = (unsigned char)(LinearFunc[pos++]->GetValue(t));
							Pal->B[k] = (unsigned char)(LinearFunc[pos++]->GetValue(t));
						}

						SlotSetVolumeOpacity();
						Volume->Update();
					}
				}

				///------------------------------
				//loop visibility






				//-------------------------------


				QString txt, txt2;
				txt = "Animation in progress: ";
				txt2.sprintf("(%d/%d)", S++, NumberOfFrames);
				txt.append(txt2);
				emit SendMessage(txt);

				qApp->processEvents();
				VWidget->Interactor->repaint();
				VWidget->Renderer->ResetCameraClippingRange();
				//	VWidget->Interactor->updateGL();		
				/*	if (StereoWidget && StereoWidget->isVisible()) {
						StereoWidget->repaint();
					}*/
				emit Signal3DFrameGenerated();

				if (GenerateSequenceAbort == true)
				{
					GenerateSequenceAbort = false;
					return;
				}
			}
		}
		startAnim = 0;
	} while ((LoopAnimation == true) && (grabAnimMode == false));


	vtkTextActor *lActor=VWidget->GetLastActor();
	if (lActor != NULL) VWidget->RemoveTextActor(lActor);

	delete[]splineTab;
	delete[]startSplineTab;
	for (j = 0;j < 9;j++)
	{
		Spline[j]->Delete();
		//	delete FTab[j];
	}

	delete Spline;
	//delete FTab;

	for (j = 0;j < NumberOfRest;j++)
		LinearFunc[j]->Delete();
	delete LinearFunc;

	delete[]OpacityTab;
	//StereoWidget->animMode = false;

	for (int i = 0;i < KeyPointListTmp->GetNumberOfItems();i++)
	{
		if (animOnlyOpacityOfGroups == false)//jak true to mozna kasowac
			KeyPointListTmp->GetItemAtPos(i)->SetData(NULL);


	}

	delete KeyPointListTmp;


	textList->DeleteAllItems();


}

//-----------------------------------

void Animations::SlotGenerateSequenceAbort()
{
	GenerateSequenceAbort = true;
}

//-----------------------------------

void Animations::SetupSceneWithCurrentItemParameters()
{

	if (KeyPointList->GetCurrentListItem() != NULL)
	{

		int i, k;

		AnimationKeyPoint* Item = KeyPointList->GetCurrentListItem()->GetData();
		int NrOfSurfaces = Item->NumberOfSurfaces;
		double tol = 1e-5;
		double Point1[3], Point2[3], Point3[3];
		for (i = 0;i < 3;i++)
		{
			Point1[i] = Item->CameraPoint[i];
			Point2[i] = Item->CameraViewUpPoint[i];
			Point3[i] = Item->CameraFocalPoint[i];
		}
		SlotMoveCameraPosAndViewUp(Point1, Point2, Point3,Item);

		double oldViewAngle, viewAngle;
		//oldViewAngle=VWidget->
		oldViewAngle = VWidget->getViewAngle();
		viewAngle = Item->viewAngle;
		if (!compare2Doubles(oldViewAngle, viewAngle, tol))
			VWidget->setViewAngle(viewAngle);

		if (UpdateCamOnly == false)
		{
			//-------------------------
			//for(i=0;i<3;i++)
			//	Point1[i]=Item->Background3DColorPoint[i];
			//VWidget->Renderer->SetBackground(Point1);








			double col1[3], col2[3];



			int tmp = VWidget->GetBackgroundMode();


			if (Item->backgroundMode == 0)
			{	//color

				VWidget->getBackgroundColor(col1);

				if (!compare2Doubles3(Item->Background3DColorPoint, col1, tol)) VWidget->setBackgroundColor(Item->Background3DColorPoint);


			}
			else if (Item->backgroundMode == 1)
			{	//gradient
				tmp = VWidget->GetBackgroundMode();
				VWidget->getGradientBackgroundColor1(col1);
				VWidget->getGradientBackgroundColor2(col2);

				if (!compare2Doubles3(Item->BackgroundColor1, col1, tol)) VWidget->setGradientBackgroundColor1(Item->BackgroundColor1);
				if (!compare2Doubles3(Item->BackgroundColor2, col2, tol)) VWidget->setGradientBackgroundColor2(Item->BackgroundColor2);

			}
			else
			{	//skybox
				QString tmpskyName = VWidget->GetSkyboxName();
				if (((Item->skyboxName).isEmpty() == false) && (tmpskyName.compare(Item->skyboxName) != 0)) VWidget->SetCurrentSkyboxModel(Item->skyboxName);

			}

			if (tmp != Item->backgroundMode)
			{
				VWidget->SlotSetBackgroundMode(Item->backgroundMode);
			}




			// 3D surfaces	
			int ss = NrOfSurfaces;
			if (NrOfSurfaces > SurfaceDataList->GetNumberOfItems())
			{
				ss = SurfaceDataList->GetNumberOfItems();
			}
			for (k = 0;k < ss;k++)
				//for(k=0;k<NrOfSurfaces;k++)
			{

				int Representation = *(Item->RepresentationPoints + k);

				if (Representation == 2)
					SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetRepresentationToSurface();
				else if (Representation == 1)
					SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetRepresentationToWireframe();
				else if (Representation == 0)
					SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetRepresentationToPoints();

				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetOpacity(*(Item->OpacityPoints + k));

				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetAmbient(*(Item->AmbientValuePoints + k));
				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetSpecular(*(Item->SpecularValuePoints + k));
				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetSpecularPower(*(Item->SpecularPowerPoints + k));
				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetDiffuse(*(Item->DiffuseValuePoints + k));

				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetColor(Item->ColorPoints + 3 * k);

				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetSpecularColor(Item->SpecularColorPoints + 3 * k);
				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetDiffuseColor(Item->DiffuseColorPoints + 3 * k);
				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->SetAmbientColor(Item->AmbientColorPoints + 3 * k);

				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVActor()->SetOrigin(Item->ActorOriginPoints + 3 * k);
				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVActor()->SetOrientation(Item->ActorOrientationPoints + 3 * k);
				SurfaceDataList->GetItemAtPos(k)->GetData()->GetVActor()->SetPosition(Item->ActorPositionPoints + 3 * k);


				/*	if ((SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->GetOpacity()>0)&&
						(SurfaceDataList->GetItemAtPos(k)->GetData()->GetVProperty()->GetOpacity()<1.0))
						SlotDepthSortPolygonSurface(SurfaceDataList->GetItemAtPos(k)->GetData());*/

			}




			// volume opacity and thresholds
			if (Volume->GetMapper() != NULL)
			{

				Histogram->SetLowerThreshold(Item->LeftVolumeThreshold);
				Histogram->SetUpperThreshold(Item->RightVolumeThreshold);
				Histogram->SetLeftY(Item->LeftVolumeOpacity);
				Histogram->SetRightY(Item->RightVolumeOpacity);


				for (i = 0;i < 256;i++)
				{
					Pal->R[i] = PalTab[Item->PaletteNr * 768 + i];
					Pal->G[i] = PalTab[Item->PaletteNr * 768 + 256 + i];
					Pal->B[i] = PalTab[Item->PaletteNr * 768 + 512 + i];
				}

				SlotSetVolumeOpacity();
				Volume->Update();

			}


			//if (Item->text.isEmpty()==false)
			//{
			//	//tyu cos trzena wstawic
			//	//tyu cos trzena wstawic
			//	QStringList list;
			//	for (int i = 0;i < 4;i++)
			//		list << QString::number(Item->textPos[i]);
			//	for (int i = 0;i < 3;i++)
			//		list << QString::number(Item->textColor[i]);

			//	//emit SingalSendTextForGui(list);
			//	int pos = 0;
			//	for (int i = 0;i < 4;i++) GuiPanel->animTextPosLineEdit[i]->setText(list[pos++]);

			//	for (int i = 0;i < 3;i++) GuiPanel->animTextColorLineEdit[i]->setText(list[pos++]);


			//	GuiPanel->animTxtLineEdit->setText(Item->text);

			//}



		}

		VWidget->Interactor->repaint();
		VWidget->Renderer->ResetCameraClippingRange();
		//	VWidget->Interactor->updateGL();
		//	emit SignalSendNumberOfSubSteps(Item->CameraSubStep);

		GuiPanel->NumberOfSequenceSubStepsBox->setValue(Item->CameraSubStep);


	}


}
//-----------------------------------
//-----------------------------------
bool Animations::LoadAnimationSequence2(const QString& filename)
{
	if ((!filename.isEmpty()))
	{




		DeleteAllItems();



		QSettings settings(filename, QSettings::IniFormat);

		double tol = 1e-5;



		/*	double cameraPointOld[3],cameraViewUpOld[3],cameraFocalPointOld[3];
			double backColOld[3],backCol1Old[3],backCol2Old[3];
			QString skyboxNameOld;
			int backgroundModeOld;
			double viewUpOld;
			double colorOld[3],specularColorO[3],diffuseColorOld[3],ambientColorOld[3];
			double specularValueOld,specularPowerOld,diffuseValueOld,ambientValueOld;
			double leftVolumeOpacityOld,rightVolumeOpacityOld;
			int	   rightVolumeThresholdOld,leftVolumeThresholdOld;
			int		paletteNrOld;*/



		QString representationOld;


		int i, j, m;
		int nr = 0;//KeyPointList->GetNumberOfItems();;
		int surfaceNr = 0;//SurfaceDataList->GetNumberOfItems();

		settings.beginGroup("Main");

		if (settings.contains("KeyFrameCount")) nr = settings.value("KeyFrameCount").toInt();
		if (settings.contains("SurfacesCount")) surfaceNr = settings.value("SurfacesCount").toInt();
		
		settings.endGroup();

		nrOfSavedGroupNames = surfaceNr;
		if (nrOfSavedGroupNames > 0)
		{
			if (savedGroupNames != NULL) delete[]savedGroupNames;
			savedGroupNames = new QString[surfaceNr];
		}


		AnimationKeyPoint* Item = NULL;
		AnimationKeyPoint* ItemPrev = NULL;
		//settings.beginWriteArray("KeyFrames");
		settings.beginGroup("KeyFrames");
		for (j = 0;j < nr;j++)
		{
			AnimationKeyPoint* Item = new AnimationKeyPoint(surfaceNr);

			if (j > 0) Item->Copy(*KeyPointList->GetItemAtPos(j - 1)->GetData(), true);

			//if (j > 0) ItemPrev = KeyPointList->GetItemAtPos(j - 1)->GetData();
			//settings.setArrayIndex(j);






			QString gName = "KeyFrames_" + QString::number(j);
			settings.beginGroup(gName);

			if (settings.contains("SubSteps")) Item->CameraSubStep = settings.value("SubSteps").toInt();

			

			//dla pierwszej klatki zapisz wszystko dla kolejnych nie

			//camera
			settings.beginGroup("Camera");

			settings.beginGroup("Point");

			if (settings.contains("x")) Item->CameraPoint[0] = settings.value("x").toDouble();
			if (settings.contains("y")) Item->CameraPoint[1] = settings.value("y").toDouble();
			if (settings.contains("z")) Item->CameraPoint[2] = settings.value("z").toDouble();

			settings.endGroup();



			settings.beginGroup("ViewUp");

			if (settings.contains("x")) Item->CameraViewUpPoint[0] = settings.value("x").toDouble();
			if (settings.contains("y")) Item->CameraViewUpPoint[1] = settings.value("y").toDouble();
			if (settings.contains("z")) Item->CameraViewUpPoint[2] = settings.value("z").toDouble();

			settings.endGroup();



			settings.beginGroup("FocalPoint");

			if (settings.contains("x")) Item->CameraFocalPoint[0] = settings.value("x").toDouble();
			if (settings.contains("y")) Item->CameraFocalPoint[1] = settings.value("y").toDouble();
			if (settings.contains("z")) Item->CameraFocalPoint[2] = settings.value("z").toDouble();

			settings.endGroup();


			if (settings.contains("ViewAngle")) Item->viewAngle = settings.value("ViewAngle").toDouble();

			settings.endGroup();
			//background
			settings.beginGroup("Background");

			settings.beginGroup("Color");

			if (settings.contains("R")) Item->Background3DColorPoint[0] = settings.value("R").toDouble();
			if (settings.contains("G")) Item->Background3DColorPoint[1] = settings.value("G").toDouble();
			if (settings.contains("B")) Item->Background3DColorPoint[2] = settings.value("B").toDouble();

			settings.endGroup();


			settings.beginGroup("Color1");

			if (settings.contains("R")) Item->BackgroundColor1[0] = settings.value("R").toDouble();
			if (settings.contains("G")) Item->BackgroundColor1[1] = settings.value("G").toDouble();
			if (settings.contains("B")) Item->BackgroundColor1[2] = settings.value("B").toDouble();

			settings.endGroup();


			settings.beginGroup("Color2");
			if (settings.contains("R")) Item->BackgroundColor2[0] = settings.value("R").toDouble();
			if (settings.contains("G")) Item->BackgroundColor2[1] = settings.value("G").toDouble();
			if (settings.contains("B")) Item->BackgroundColor2[2] = settings.value("B").toDouble();

			settings.endGroup();



			if (settings.contains("Skybox")) Item->skyboxName = settings.value("ViewAngle").toString();
			if (settings.contains("Mode")) Item->backgroundMode = settings.value("Mode").toInt();

			settings.endGroup();



			/*settings.beginGroup("Text");

			if (settings.contains("Text")) Item->text = settings.value("Text").toString();


			settings.beginGroup("Pos");

			if (settings.contains("X")) Item->textPos[0] = settings.value("X").toInt();
			if (settings.contains("Y")) Item->textPos[1] = settings.value("Y").toInt();
			if (settings.contains("Size")) Item->textPos[2] = settings.value("Size").toInt();
			if (settings.contains("Step")) Item->textPos[3] = settings.value("Step").toInt();


			settings.endGroup();

			settings.beginGroup("Color");
			if (settings.contains("R")) Item->textColor[0] = settings.value("R").toDouble();
			if (settings.contains("G")) Item->textColor[1] = settings.value("G").toDouble();
			if (settings.contains("B")) Item->textColor[2] = settings.value("B").toDouble();


			settings.endGroup();*/


			//settings.endGroup();

			 
			//surfaces 

			//settings.beginWriteArray("Surfaces");
			settings.beginGroup("Surfaces");
			for (i = 0;i < Item->NumberOfSurfaces;i++)
			{
				gName = "Surfaces_" + QString::number(i);
				if ((SurfaceDataList->GetNumberOfItems() >= i) && (i == 0))
				{
					if (settings.contains("GroupName")) savedGroupNames[i] = settings.value("GroupName").toDouble();

				}

				m = 3 * i;
				//settings.setArrayIndex(i);
				settings.beginGroup(gName);

				double* color, * specularColor, * diffuseColor, * ambientColor;
			
			
				
				double* actorOrigin, * actorPosition, * actorOrientation;

			




				color = Item->ColorPoints + m;
				specularColor = Item->SpecularColorPoints + m;
				diffuseColor = Item->DiffuseColorPoints + m;
				ambientColor = Item->AmbientColorPoints + m;

				double& opacity = Item->OpacityPoints[i];
				double& specularValue = Item->SpecularValuePoints[i];
				double& specularPower = Item->SpecularPowerPoints[i];
				double& diffuseValue = Item->DiffuseValuePoints[i];
				double& ambientValue = Item->AmbientValuePoints[i];
				char&representation = Item->RepresentationPoints[i];

				actorOrigin = Item->ActorOriginPoints + m;
				actorPosition = Item->ActorPositionPoints + m;
				actorOrientation = Item->ActorOrientationPoints + m;



				settings.beginGroup("Color");
				if (settings.contains("R"))

				{
					color[0] = settings.value("R").toDouble();
				}
				if (settings.contains("G")) color[1] = settings.value("G").toDouble();
				if (settings.contains("B")) color[2] = settings.value("B").toDouble();
				if (settings.contains("opacity"))
				{
					opacity = settings.value("opacity").toDouble();
					if (opacity > 0)
					{
						j = j;
					}
				}

				settings.endGroup();


				settings.beginGroup("Specular");

				if (settings.contains("R")) specularColor[0] = settings.value("R").toDouble();
				if (settings.contains("G")) specularColor[1] = settings.value("G").toDouble();
				if (settings.contains("B")) specularColor[2] = settings.value("B").toDouble();

				if (settings.contains("value")) specularValue = settings.value("value").toDouble();
				if (settings.contains("power")) specularPower = settings.value("power").toDouble();


				settings.endGroup();


				settings.beginGroup("Diffuse");


				if (settings.contains("R")) diffuseColor[0] = settings.value("R").toDouble();
				if (settings.contains("G")) diffuseColor[1] = settings.value("G").toDouble();
				if (settings.contains("B")) diffuseColor[2] = settings.value("B").toDouble();
				if (settings.contains("value")) diffuseValue = settings.value("value").toDouble();



				settings.endGroup();


				settings.beginGroup("Ambient");

				if (settings.contains("R")) ambientColor[0] = settings.value("R").toDouble();
				if (settings.contains("G")) ambientColor[1] = settings.value("G").toDouble();
				if (settings.contains("B")) ambientColor[2] = settings.value("B").toDouble();
				if (settings.contains("value")) ambientValue = settings.value("value").toDouble();



				settings.endGroup();

				settings.beginGroup("Other");

				if (settings.contains("Representation")) representation = settings.value("Representation").toInt();


				settings.endGroup();



				settings.beginGroup("Actor");


				settings.beginGroup("Position");


				if (settings.contains("x")) actorPosition[0] = settings.value("x").toDouble();
				if (settings.contains("y")) actorPosition[1] = settings.value("y").toDouble();
				if (settings.contains("z")) actorPosition[2] = settings.value("z").toDouble();


				settings.endGroup();


				settings.beginGroup("Orientation");
				if (settings.contains("x")) actorOrientation[0] = settings.value("x").toDouble();
				if (settings.contains("y")) actorOrientation[1] = settings.value("y").toDouble();
				if (settings.contains("z")) actorOrientation[2] = settings.value("z").toDouble();

				settings.endGroup();


				settings.beginGroup("Origin");
				if (settings.contains("x")) actorOrigin[0] = settings.value("x").toDouble();
				if (settings.contains("y")) actorOrigin[1] = settings.value("y").toDouble();
				if (settings.contains("z")) actorOrigin[2] = settings.value("z").toDouble();
				settings.endGroup();


				settings.endGroup();


				settings.endGroup();

				
				//settings.endArray();
			}
			settings.endGroup();

			settings.beginGroup("Volume");


			if (settings.contains("LeftOpacity")) Item->LeftVolumeOpacity = settings.value("LeftOpacity").toInt();
			if (settings.contains("RightOpacity")) Item->RightVolumeOpacity = settings.value("RightOpacity").toInt();
			if (settings.contains("LeftThreshold")) Item->LeftVolumeOpacity = settings.value("LeftThreshold").toInt();
			if (settings.contains("RightThreshold")) Item->RightVolumeThreshold = settings.value("RightThreshold").toInt();
			if (settings.contains("PaletteNr")) Item->PaletteNr = settings.value("PaletteNr").toInt();


			settings.endGroup();




			//volume


			settings.endGroup();

			KeyPointList->InsertItem(Item);
		}
		//settings.endArray();
		settings.endGroup();

		
		settings.beginGroup("Subtitles");
		int nrOfItems=settings.value("NumberOfTextItems").toInt();

			for (int j = 0;j < nrOfItems;j++)
			{

			
				QString gName = "Line_" + QString::number(j);
				settings.beginGroup(gName);

				int pos[4];
				int timeInt;
				double t;
				int color[3];

				double timeV;
				timeV = settings.value("Time").toDouble();
				timeInt = (int)timeV;
				t = timeV - timeInt;



				pos[0] = settings.value("TextPosX").toInt();
				pos[1] = settings.value("TextPosY").toInt();
				pos[2] = settings.value("FontSize").toInt();
				pos[3] = settings.value("TimeOut").toInt();
					
				color[0] = settings.value("FontColor_Red").toDouble();
				color[1] = settings.value("FontColor_Green").toDouble();
				color[2] = settings.value("FontColor_Blue").toDouble();
				

				QString textV = settings.value("Text").toString();
				
				



				QString num;

				num += QString::number(timeInt);num += ",";
				num += QString::number(t);num += ",";
				for (int i = 0;i < 4;i++)
				{
					num += QString::number(pos[i]);
					num += ",";
				}
				for (int i = 0;i < 3;i++)
				{
					num += QString::number(color[i]);
					if (i < 2) num += ",";
				}

				GuiPanel->subtitleTextEdit->appendPlainText(num);
				GuiPanel->subtitleTextEdit->appendPlainText(textV);



				settings.endGroup();

			}



			settings.endGroup();

		



		//fclose(plik);



	}
	else
		return false;


}
//------------------------------
bool Animations::SaveAnimationSequence2(const QString& txt)
{
	///save only changes
	//QFile f(txt);
	//if(f.exists())

	QString filename = txt;
	if (filename.lastIndexOf(".animseq2") == -1)
		filename += ".animseq2";

	{
		QSettings settings(filename, QSettings::IniFormat);

		double tol = 1e-5;



		/*	double cameraPointOld[3],cameraViewUpOld[3],cameraFocalPointOld[3];
			double backColOld[3],backCol1Old[3],backCol2Old[3];
			QString skyboxNameOld;
			int backgroundModeOld;
			double viewUpOld;
			double colorOld[3],specularColorO[3],diffuseColorOld[3],ambientColorOld[3];
			double specularValueOld,specularPowerOld,diffuseValueOld,ambientValueOld;
			double leftVolumeOpacityOld,rightVolumeOpacityOld;
			int	   rightVolumeThresholdOld,leftVolumeThresholdOld;
			int		paletteNrOld;*/



		QString representationOld;


		int i, j, m;
		int nr = KeyPointList->GetNumberOfItems();;
		int surfaceNr = SurfaceDataList->GetNumberOfItems();

		settings.beginGroup("Main");
		settings.setValue("KeyFrameCount", nr);
		settings.setValue("SurfacesCount", surfaceNr);
		settings.endGroup();

		AnimationKeyPoint* Item = NULL;
		AnimationKeyPoint* ItemPrev = NULL;
		//settings.beginWriteArray("KeyFrames");
		settings.beginGroup("KeyFrames");
		for (j = 0;j < nr;j++)
		{
			Item = KeyPointList->GetItemAtPos(j)->GetData();
			if (j > 0) ItemPrev = KeyPointList->GetItemAtPos(j - 1)->GetData();
			//settings.setArrayIndex(j);
			QString gName = "KeyFrames_" + QString::number(j);
			settings.beginGroup(gName);
			//dla pierwszej klatki zapisz wszystko dla kolejnych nie

			settings.setValue("SubSteps", Item->CameraSubStep);
			//camera
			settings.beginGroup("Camera");

			if ((j == 0) || ((j > 0) && (!compare2Doubles3(Item->CameraPoint, ItemPrev->CameraPoint, tol))))
			{
				settings.beginGroup("Point");
				settings.setValue("x", Item->CameraPoint[0]);			settings.setValue("y", Item->CameraPoint[1]);			settings.setValue("z", Item->CameraPoint[2]);
				settings.endGroup();
			}

			if ((j == 0) || ((j > 0) && (!compare2Doubles3(Item->CameraViewUpPoint, ItemPrev->CameraViewUpPoint, tol))))
			{
				settings.beginGroup("ViewUp");
				settings.setValue("x", Item->CameraViewUpPoint[0]);		settings.setValue("y", Item->CameraViewUpPoint[1]);		settings.setValue("z", Item->CameraViewUpPoint[2]);
				settings.endGroup();
			}

			if ((j == 0) || ((j > 0) && (!compare2Doubles3(Item->CameraFocalPoint, ItemPrev->CameraFocalPoint, tol))))
			{
				settings.beginGroup("FocalPoint");
				settings.setValue("x", Item->CameraFocalPoint[0]);		settings.setValue("y", Item->CameraFocalPoint[1]);		settings.setValue("z", Item->CameraFocalPoint[2]);
				settings.endGroup();
			}
			if ((j == 0) || ((j > 0) && (!compare2Doubles(Item->viewAngle, ItemPrev->viewAngle, tol))))
				settings.setValue("ViewAngle", Item->viewAngle);

			settings.endGroup();
			//background
			settings.beginGroup("Background");
			if ((j == 0) || ((j > 0) && (!compare2Doubles3(Item->Background3DColorPoint, ItemPrev->Background3DColorPoint, tol))))
			{
				settings.beginGroup("Color");
				settings.setValue("R", Item->Background3DColorPoint[0]);		settings.setValue("G", Item->Background3DColorPoint[1]);			settings.setValue("B", Item->Background3DColorPoint[2]);
				settings.endGroup();
			}
			if ((j == 0) || ((j > 0) && (!compare2Doubles3(Item->BackgroundColor1, ItemPrev->BackgroundColor1, tol))))
			{
				settings.beginGroup("Color1");
				settings.setValue("R", Item->BackgroundColor1[0]);			settings.setValue("G", Item->BackgroundColor1[1]);			settings.setValue("B", Item->BackgroundColor1[2]);
				settings.endGroup();
			}
			if ((j == 0) || ((j > 0) && (!compare2Doubles3(Item->BackgroundColor2, ItemPrev->BackgroundColor2, tol))))
			{
				settings.beginGroup("Color2");
				settings.setValue("R", Item->BackgroundColor2[0]);			settings.setValue("G", Item->BackgroundColor2[1]);			settings.setValue("B", Item->BackgroundColor2[2]);
				settings.endGroup();
			}

			if ((j == 0) || ((j > 0) && (Item->skyboxName.compare(ItemPrev->skyboxName) != 0)))
				settings.setValue("Skybox", Item->skyboxName);
			if ((j == 0) || ((j > 0) && (Item->backgroundMode != ItemPrev->backgroundMode)))
				settings.setValue("Mode", Item->backgroundMode);

			settings.endGroup();


			/*if (Item->text.isEmpty() == false)
			{
				settings.beginGroup("Text");

				settings.setValue("Text", Item->text);

				settings.beginGroup("Pos");
				settings.setValue("X", Item->textPos[0]);
				settings.setValue("Y", Item->textPos[1]);
				settings.setValue("Size", Item->textPos[2]);
				settings.setValue("Step", Item->textPos[3]);
				settings.endGroup();

				settings.beginGroup("Color");
				settings.setValue("R", Item->textColor[0]);			settings.setValue("G", Item->textColor[1]);			settings.setValue("B", Item->textColor[2]);
				settings.endGroup();


				settings.endGroup();
			}*/

			//surfaces 

			//settings.beginWriteArray("Surfaces");
			settings.beginGroup("Surfaces");
			for (i = 0;i < Item->NumberOfSurfaces;i++)
			{
				gName = "Surfaces_" + QString::number(i);
				if ((SurfaceDataList->GetNumberOfItems() >= i)&&(SurfaceDataList->GetNumberOfItems()>0))
					settings.setValue("GroupName", SurfaceDataList->GetItemAtPos(i)->GetData()->GetGroupName());
				m = 3 * i;
				//settings.setArrayIndex(i);
				settings.beginGroup(gName);

				double* color, * specularColor, * diffuseColor, * ambientColor;
				double opacity, specularValue, specularPower, diffuseValue, ambientValue;
				int representation;
				double* actorOrigin, * actorPosition, * actorOrientation;

				double* colorPrev, * specularColorPrev, * diffuseColorPrev, * ambientColorPrev;
				double opacityPrev, specularValuePrev, specularPowerPrev, diffuseValuePrev, ambientValuePrev;
				int representationPrev;
				double* actorOriginPrev, * actorPositionPrev, * actorOrientationPrev;


				if (j > 0)
				{
					colorPrev = ItemPrev->ColorPoints + m;
					specularColorPrev = ItemPrev->SpecularColorPoints + m;
					diffuseColorPrev = ItemPrev->DiffuseColorPoints + m;
					ambientColorPrev = ItemPrev->AmbientColorPoints + m;

					opacityPrev = ItemPrev->OpacityPoints[i];
					specularValuePrev = ItemPrev->SpecularValuePoints[i];
					specularPowerPrev = ItemPrev->SpecularPowerPoints[i];
					diffuseValuePrev = ItemPrev->DiffuseValuePoints[i];
					ambientValuePrev = ItemPrev->AmbientValuePoints[i];
					representationPrev = ItemPrev->RepresentationPoints[i];

					actorOriginPrev = ItemPrev->ActorOriginPoints + m;
					actorPositionPrev = ItemPrev->ActorPositionPoints + m;
					actorOrientationPrev = ItemPrev->ActorOrientationPoints + m;
				}


				color = Item->ColorPoints + m;
				specularColor = Item->SpecularColorPoints + m;
				diffuseColor = Item->DiffuseColorPoints + m;
				ambientColor = Item->AmbientColorPoints + m;

				opacity = Item->OpacityPoints[i];
				specularValue = Item->SpecularValuePoints[i];
				specularPower = Item->SpecularPowerPoints[i];
				diffuseValue = Item->DiffuseValuePoints[i];
				ambientValue = Item->AmbientValuePoints[i];
				representation = Item->RepresentationPoints[i];

				actorOrigin = Item->ActorOriginPoints + m;
				actorPosition = Item->ActorPositionPoints + m;
				actorOrientation = Item->ActorOrientationPoints + m;


				if ((j == 0)
					|| ((j > 0) && (!compare2Doubles3(color, colorPrev, tol)))
					|| ((j > 0) && (!compare2Doubles(opacity, opacityPrev, tol)))
					)
				{
					settings.beginGroup("Color");
					if ((j == 0) || ((j > 0) && (!compare2Doubles3(color, colorPrev, tol))))
					{
						settings.setValue("R", color[0]);				settings.setValue("G", color[1]);				settings.setValue("B", color[2]);
					}
					if ((j == 0) || ((j > 0) && (!compare2Doubles(opacity, opacityPrev, tol))))
					{
						settings.setValue("opacity", opacity);
					}
					settings.endGroup();
				}
				if ((j == 0)
					|| ((j > 0) && (!compare2Doubles3(specularColor, specularColorPrev, tol)))
					|| ((j > 0) && (!compare2Doubles(specularValue, specularValuePrev, tol)))
					|| ((j > 0) && (!compare2Doubles(specularPower, specularPowerPrev, tol)))
					)


				{
					settings.beginGroup("Specular");
					if ((j == 0) || ((j > 0) && (!compare2Doubles3(specularColor, specularColorPrev, tol))))
					{
						settings.setValue("R", specularColor[0]);		settings.setValue("G", specularColor[1]);		settings.setValue("B", specularColor[2]);
					}
					if ((j == 0) || ((j > 0) && (!compare2Doubles(specularValue, specularValuePrev, tol))))
					{
						settings.setValue("value", specularValue);
					}
					if ((j == 0) || ((j > 0) && (!compare2Doubles(specularPower, specularPowerPrev, tol))))
					{
						settings.setValue("power", specularPower);
					}
					settings.endGroup();
				}
				if ((j == 0)
					|| ((j > 0) && (!compare2Doubles3(diffuseColor, diffuseColorPrev, tol)))
					|| ((j > 0) && (!compare2Doubles(diffuseValue, diffuseValuePrev, tol)))
					)
				{
					settings.beginGroup("Diffuse");
					if ((j == 0) || ((j > 0) && (!compare2Doubles3(diffuseColor, diffuseColorPrev, tol))))
					{
						settings.setValue("R", diffuseColor[0]);			settings.setValue("G", diffuseColor[1]);			settings.setValue("B", diffuseColor[2]);
					}
					if ((j == 0) || ((j > 0) && (!compare2Doubles(diffuseValue, diffuseValuePrev, tol))))
						settings.setValue("value", diffuseValue);
					settings.endGroup();
				}
				if ((j == 0)
					|| ((j > 0) && (!compare2Doubles3(ambientColor, ambientColorPrev, tol)))
					|| ((j > 0) && (!compare2Doubles(ambientValue, ambientValuePrev, tol)))
					)
				{
					settings.beginGroup("Ambient");
					if ((j == 0) || ((j > 0) && (!compare2Doubles3(ambientColor, ambientColorPrev, tol))))
					{
						settings.setValue("R", ambientColor[0]);			settings.setValue("G", ambientColor[1]);			settings.setValue("B", ambientColor[2]);
					}
					if ((j == 0) || ((j > 0) && (!compare2Doubles(ambientValue, ambientValuePrev, tol))))
						settings.setValue("value", ambientValue);
					settings.endGroup();
				}
				if ((j == 0) || (representation != representationPrev))
				{
					settings.beginGroup("Other");
					settings.setValue("Representation", representation);
					settings.endGroup();
				}

				if ((j == 0)
					|| ((j > 0) && (!compare2Doubles3(actorOrigin, actorOriginPrev, tol)))
					|| ((j > 0) && (!compare2Doubles3(actorPosition, actorPositionPrev, tol)))
					|| ((j > 0) && (!compare2Doubles3(actorOrientation, actorOrientationPrev, tol)))
					)
				{
					settings.beginGroup("Actor");

					if ((j == 0) || ((j > 0) && (!compare2Doubles3(actorPosition, actorPositionPrev, tol))))
					{
						settings.beginGroup("Position");
						settings.setValue("x", actorPosition[0]);			settings.setValue("y", actorPosition[1]);			settings.setValue("z", actorPosition[2]);
						settings.endGroup();
					}
					if ((j == 0) || ((j > 0) && (!compare2Doubles3(actorPosition, actorPositionPrev, tol))))
					{
						settings.beginGroup("Orientation");
						settings.setValue("x", actorOrientation[0]);			settings.setValue("y", actorOrientation[1]);			settings.setValue("z", actorOrientation[2]);
						settings.endGroup();
					}
					if ((j == 0) || ((j > 0) && (!compare2Doubles3(actorOrigin, actorOriginPrev, tol))))
					{
						settings.beginGroup("Origin");
						settings.setValue("x", actorOrigin[0]);			settings.setValue("y", actorOrigin[1]);			settings.setValue("z", actorOrigin[2]);
						settings.endGroup();
					}

					settings.endGroup();

				}
				settings.endGroup();
			}
			settings.endGroup();
			//settings.endArray();

			int lt, rt, pn;
			double lo, ro;
			int ltPrev, rtPrev, pnPrev;
			double loPrev, roPrev;

			lo = Item->LeftVolumeOpacity;
			ro = Item->RightVolumeOpacity;
			lt = Item->LeftVolumeThreshold;
			rt = Item->RightVolumeThreshold;
			pn = Item->PaletteNr;

			if (j > 0)
			{
				loPrev = ItemPrev->LeftVolumeOpacity;
				roPrev = ItemPrev->RightVolumeOpacity;
				ltPrev = ItemPrev->LeftVolumeThreshold;
				rtPrev = ItemPrev->RightVolumeThreshold;
				pnPrev = ItemPrev->PaletteNr;
			}


			bool comp[5] = { 0 };
			bool anycomp = false;
			if (lo != loPrev) { comp[0] = true;anycomp = true; }
			if (ro != roPrev) { comp[1] = true;anycomp = true; }
			if (lt != ltPrev) { comp[2] = true;anycomp = true; }
			if (rt != rtPrev) { comp[3] = true;anycomp = true; }
			if (pn != pnPrev) { comp[4] = true;anycomp = true; }

			if ((j == 0) || (anycomp == true))
			{
				settings.beginGroup("Volume");
				if ((j == 0) || (comp[0])) settings.setValue("LeftOpacity", lo);
				if ((j == 0) || (comp[1])) settings.setValue("RightOpacity", ro);
				if ((j == 0) || (comp[2])) settings.setValue("LeftThreshold", lt);
				if ((j == 0) || (comp[3])) settings.setValue("RightThreshold", rt);
				if ((j == 0) || (comp[4])) settings.setValue("PaletteNr", pn);
				settings.endGroup();
			}



			//volume


			settings.endGroup();
		}
		//settings.endArray();
		settings.endGroup();

		LinkedList < AnimationText*>* textList = GenerateTextList();

		if (textList->GetNumberOfItems() > 0)
		{
			settings.beginGroup("Subtitles");
			settings.setValue("NumberOfTextItems", textList->GetNumberOfItems());

			for (int j = 0;j < textList->GetNumberOfItems();j++)
			{

				AnimationText* item = textList->GetItemAtPos(j)->GetData();

				QString gName = "Line_" + QString::number(j);
				settings.beginGroup(gName);


				settings.setValue("Time", item->GetTimeValue());
				settings.setValue("TextPosX", item->GetPos()[0]);
				settings.setValue("TextPosY", item->GetPos()[1]);
				settings.setValue("FontSize", item->GetFontSize());
				settings.setValue("TimeOut", item->GetTimeout());
				settings.setValue("FontColor_Red", item->GetColor()[0]);
				settings.setValue("FontColor_Green", item->GetColor()[1]);
				settings.setValue("FontColor_Blue", item->GetColor()[2]);
				settings.setValue("Text", item->GetText());
				settings.endGroup();

			}



			settings.endGroup();

		}

		textList->DeleteAllItems();
			
		

		return 1;
	}
	return 0;
	//zapisuje liczbe klatek
	//zapisuje dane kolejnych klatek kluczowych
	//ale dla drugiej i dalej zapisuje tylko dane zmienione
}
//------------------------------
bool Animations::SaveAnimationSequence(const QString& _filename)
{



	if ((!_filename.isEmpty()) && (KeyPointList->GetNumberOfItems() > 0))
	{


		//const char *name1;	
		QString filename = _filename;
		if (filename.lastIndexOf(".animseq") == -1)
			filename += ".animseq";
		//name1=filename.toLocal8Bit();

		int Nr = KeyPointList->GetNumberOfItems();
		int j, k;

		int SurfaceNr = SurfaceDataList->GetNumberOfItems();
		//FILE *plik;

		//plik=fopen(name1,"w");

		QFile file(filename);
		int i;
		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&file);

			//fprintf(plik,"%d %d\n",Nr,SurfaceNr);
			stream << Nr << " " << SurfaceNr << "\n";
			/// teraz trzeba bedzie zapisac wszystko


			AnimationKeyPoint* Item;
			int m;
			for (j = 0;j < Nr;j++)
			{

				Item = KeyPointList->GetItemAtPos(j)->GetData();
				//substeps

				stream << Item->CameraSubStep << "\n";
				// camera



				for (k = 0;k < 3;k++) { stream << Item->CameraPoint[k] << " "; }	stream << "\n";
				for (k = 0;k < 3;k++) { stream << Item->CameraViewUpPoint[k] << " "; }	stream << "\n";
				for (k = 0;k < 3;k++) { stream << Item->CameraFocalPoint[k] << " "; }	stream << "\n";

				stream << Item->viewAngle << "\n";
				// 3D Background color	

				for (k = 0;k < 3;k++) { stream << Item->Background3DColorPoint[k] << " "; }	stream << "\n";
				for (k = 0;k < 3;k++) { stream << Item->BackgroundColor1[k] << " "; }	stream << "\n";
				for (k = 0;k < 3;k++) { stream << Item->BackgroundColor2[k] << " "; }	stream << "\n";


				//col2
				//skybox
				if (Item->skyboxName.isEmpty() == true)

					stream << "none\n";
				else

					stream << Item->skyboxName << "\n";
				//int backgroundmode

				stream << Item->backgroundMode << "\n";
				// 3D surfaces
				for (i = 0;i < Item->NumberOfSurfaces;i++)
				{
					m = 3 * i;

					QString groupName = SurfaceDataList->GetItemAtPos(i)->GetData()->GetGroupName();

					for (k = 0;k < 3;k++) { stream << Item->ColorPoints[m + k] << " "; }	stream << "\n";
					for (k = 0;k < 3;k++) { stream << Item->SpecularColorPoints[m + k] << " "; }	stream << "\n";
					for (k = 0;k < 3;k++) { stream << Item->DiffuseColorPoints[m + k] << " "; }	stream << "\n";
					for (k = 0;k < 3;k++) { stream << Item->AmbientColorPoints[m + k] << " "; }	stream << "\n";

					stream << Item->OpacityPoints[i] << " ";
					stream << Item->AmbientValuePoints[i] << " ";
					stream << Item->SpecularValuePoints[i] << " ";
					stream << Item->SpecularPowerPoints[i] << " ";
					stream << Item->DiffuseValuePoints[i] << "\n";
					stream << (int)(Item->RepresentationPoints[i]) << "?" + groupName << "\n";

					for (k = 0;k < 3;k++) { stream << Item->ActorOriginPoints[m + k] << " "; }	stream << "\n";
					for (k = 0;k < 3;k++) { stream << Item->ActorOrientationPoints[m + k] << " "; }	stream << "\n";
					for (k = 0;k < 3;k++) { stream << Item->ActorPositionPoints[m + k] << " "; }	stream << "\n";




				}

				// volume opacity and thresholds			
				stream << Item->LeftVolumeOpacity << " ";
				stream << Item->RightVolumeOpacity << " ";
				stream << Item->LeftVolumeThreshold << " ";
				stream << Item->RightVolumeThreshold << "\n";
				stream << Item->PaletteNr << "\n";



			/*	if (Item->text.isEmpty() == false)
				{
					stream << "TEXT_SET" << "\n";
					stream << Item->text << "\n";
					for (k = 0;k < 4;k++) { stream << Item->textPos[k] << " "; }	stream << "\n";
					for (k = 0;k < 3;k++) { stream << Item->textColor[k] << " "; }	stream << "\n";
				}*/


			}






			file.close();
			//fclose(plik);
			return true;
		}
		return false;
	}
	else
		return false;

}

//-----------------------------------
bool Animations::LoadAnimationSequence(const QString& filename)
{



	if ((!filename.isEmpty()))
	{


		//const char *name1;	
		//name1=filename.toLocal8Bit();

		DeleteAllItems();


		//		vtkCamera	*camera=VWidget->Renderer->GetActiveCamera();	

		int Nr, SurfaceNr;
		int j, k;
		//FILE *plik;


	//	plik=fopen(name1,"r");
		//fscanf(plik,"%d %d\n",&Nr,&SurfaceNr);
		QFile file(filename);
		int m, i;
		if (file.open(QIODevice::ReadOnly))
		{
			QTextStream stream(&file);

			QString line;//=stream.readLine();
			QStringList fields;// = line.split( " ");
			line = stream.readLine();
			fields = line.split(" ", QString::SkipEmptyParts);
			Nr = fields[0].toInt();
			SurfaceNr = fields[1].toInt();


			nrOfSavedGroupNames = SurfaceNr;
			if (nrOfSavedGroupNames > 0)
			{
				if (savedGroupNames != NULL) delete[]savedGroupNames;
				savedGroupNames = new QString[SurfaceNr];
			}

			for (j = 0;j < Nr;j++)
			{
				AnimationKeyPoint* Item = new AnimationKeyPoint(SurfaceNr);
				if (j == 0) line = stream.readLine();
				fields = line.split(" ", QString::SkipEmptyParts);
				//substepp
				//fscanf(plik,"%d\n",&Item->CameraSubStep);
				Item->CameraSubStep = fields[0].toInt();
				// camera

			//	fscanf(plik,"%lf %lf %lf\n",&Item->CameraPoint[0],&Item->CameraPoint[1],&Item->CameraPoint[2]);
				//fscanf(plik,"%lf %lf %lf\n",&Item->CameraViewUpPoint[0],&Item->CameraViewUpPoint[1],&Item->CameraViewUpPoint[2]);
			//	fscanf(plik,"%lf %lf %lf\n",&Item->CameraFocalPoint[0],&Item->CameraFocalPoint[1],&Item->CameraFocalPoint[2]);
				line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);
				for (k = 0;k < 3;k++) { Item->CameraPoint[k] = fields[k].toDouble(); }
				line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);
				for (k = 0;k < 3;k++) { Item->CameraViewUpPoint[k] = fields[k].toDouble(); }
				line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);
				for (k = 0;k < 3;k++) { Item->CameraFocalPoint[k] = fields[k].toDouble(); }

				line = stream.readLine();		fields = line.split(" ", QString::SkipEmptyParts);
				Item->viewAngle = fields[0].toDouble();


				// 3D Background color	
				//fscanf(plik,"%lf %lf %lf \n",&Item->Background3DColorPoint[0],&Item->Background3DColorPoint[1],&Item->Background3DColorPoint[2]);
				line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);			for (k = 0;k < 3;k++) { Item->Background3DColorPoint[k] = fields[k].toDouble(); }



				//fscanf(plik,"%lf %lf %lf \n",&Item->BackgroundColor1[0],&Item->BackgroundColor1[1],&Item->BackgroundColor1[2]);
				line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);
				for (k = 0;k < 3;k++) { Item->BackgroundColor1[k] = fields[k].toDouble(); }
				//col1
				//fscanf(plik,"%lf %lf %lf \n",&Item->BackgroundColor2[0],&Item->BackgroundColor2[1],&Item->BackgroundColor2[2]);
				line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);
				for (k = 0;k < 3;k++) { Item->BackgroundColor2[k] = fields[k].toDouble(); }
				//col2
				//skybox
				//char PathName[256];

				//fscanf(plik,"%s\n",PathName);
				//Item->skyboxName=QString::fromLatin1(PathName);
				////int backgroundmode
				line = stream.readLine();

				if (line.compare("none") == 0)
					Item->skyboxName = "";
				else
					Item->skyboxName = line;



				//fscanf(plik,"%d\n",&Item->backgroundMode);
				line = stream.readLine();
				fields = line.split(" ", QString::SkipEmptyParts);		Item->backgroundMode = fields[0].toInt();

				//fscanf(plik,"%s",PathName);
				//FileDefaultDir=QString::fromLatin1(PathName);
					// 3D surfaces
				for (i = 0;i < Item->NumberOfSurfaces;i++)
				{
					m = 3 * i;

					line = stream.readLine(); // bo potem jeszcze czytam na koniec

					//fscanf(plik,"%lf %lf %lf\n",(Item->ColorPoints+m),			(Item->ColorPoints+m+1),		(Item->ColorPoints+m+2));
					fields = line.split(" ", QString::SkipEmptyParts);			for (k = 0;k < 3;k++) { Item->ColorPoints[m + k] = fields[k].toDouble(); }
					line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);			for (k = 0;k < 3;k++) { Item->SpecularColorPoints[m + k] = fields[k].toDouble(); }
					line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);			for (k = 0;k < 3;k++) { Item->DiffuseColorPoints[m + k] = fields[k].toDouble(); }
					line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);			for (k = 0;k < 3;k++) { Item->AmbientColorPoints[m + k] = fields[k].toDouble(); }
					//fscanf(plik,"%lf %lf %lf\n",(Item->SpecularColorPoints+m),	(Item->SpecularColorPoints+m+1),(Item->SpecularColorPoints+m+2));
					//fscanf(plik,"%lf %lf %lf\n",(Item->DiffuseColorPoints+m),	(Item->DiffuseColorPoints+m+1),	(Item->DiffuseColorPoints+m+2));
					//fscanf(plik,"%lf %lf %lf\n",(Item->AmbientColorPoints+m),	(Item->AmbientColorPoints+m+1),(Item->AmbientColorPoints+m+2));

					//fscanf(plik,"%lf %lf %lf %lf %lf\n",(Item->OpacityPoints+k),(Item->AmbientValuePoints+k),(Item->SpecularValuePoints+k),(Item->SpecularPowerPoints+k),(Item->DiffuseValuePoints+k));
					line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);
					Item->OpacityPoints[i] = fields[0].toDouble();
					Item->AmbientValuePoints[i] = fields[1].toDouble();
					Item->SpecularValuePoints[i] = fields[2].toDouble();
					Item->SpecularPowerPoints[i] = fields[3].toDouble();
					Item->DiffuseValuePoints[i] = fields[4].toDouble();

					//fscanf(plik,"%d\n",(Item->RepresentationPoints+k));
					line = stream.readLine();
					fields = line.split("?", QString::SkipEmptyParts);		Item->RepresentationPoints[i] = fields[0].toInt();
					if ((fields.count() > 1) && (j == 0))
					{
						savedGroupNames[i] = fields[1];//zapamietana  grupa tylko dla zerowej klatki animacjibo to globalna informacja
					}


					//fscanf(plik,"%lf %lf %lf\n",(Item->ActorOriginPoints+m),		(Item->ActorOriginPoints+m+1),		(Item->ActorOriginPoints+m+2));
					//fscanf(plik,"%lf %lf %lf\n",(Item->ActorOrientationPoints+m),	(Item->ActorOrientationPoints+m+1),	(Item->ActorOrientationPoints+m+2));
					//fscanf(plik,"%lf %lf %lf\n",(Item->ActorPositionPoints+m),		(Item->ActorPositionPoints+m+1),	(Item->ActorPositionPoints+m+2));
					line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);			for (k = 0;k < 3;k++) { Item->ActorOriginPoints[m + k] = fields[k].toDouble(); }
					line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);			for (k = 0;k < 3;k++) { Item->ActorOrientationPoints[m + k] = fields[k].toDouble(); }
					line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);			for (k = 0;k < 3;k++) { Item->ActorPositionPoints[m + k] = fields[k].toDouble(); }



					//jesli bedize line z tekstem - font available to wczytaj!




				}

				// volume opacity and thresholds

				//fscanf(plik,"%lf %lf %d %d\n",&Item->LeftVolumeOpacity,&Item->RightVolumeOpacity,&Item->LeftVolumeThreshold,&Item->RightVolumeThreshold);
				line = stream.readLine();	fields = line.split(" ", QString::SkipEmptyParts);
				Item->LeftVolumeOpacity = fields[0].toDouble();
				Item->RightVolumeOpacity = fields[1].toDouble();
				Item->LeftVolumeThreshold = fields[2].toInt();
				Item->RightVolumeThreshold = fields[3].toInt();

				// volume palette
				//fscanf(plik,"%d\n",&Item->PaletteNr);

				line = stream.readLine();
				fields = line.split(" ", QString::SkipEmptyParts);		Item->PaletteNr = fields[0].toInt();



				line = stream.readLine();
				//if ((QString::compare(line, "TEXT_SET") == 0))
				//{


				//	//wczytuje teskt
				//	line = stream.readLine();					Item->text = line;
				//	//pozycja tekstu i wielkosc
				//	line = stream.readLine();fields = line.split("?", QString::SkipEmptyParts);					for (k = 0;k < 4;k++)Item->textPos[k] = fields[i].toInt();
				//	//kolor tekstu
				//	line = stream.readLine();fields = line.split("?", QString::SkipEmptyParts);					for (k = 0;k < 3;k++)Item->textColor[k] = fields[i].toInt();

				//	//na koniec!!!
				//	line = stream.readLine();
				//}



				KeyPointList->InsertItem(Item);

			}
			file.close();
		}
		//fclose(plik);

		return true;

	}
	else
		return false;




}

void	Animations::SlotDepthSortPolygonSurface(PolygonSurfaceData* Data)
{
	//	ofstream myFile("c:/out.txt");

	vtkDepthSortPolyData* DepthSort = vtkDepthSortPolyData::New();
	DepthSort->SetInputData(Data->GetVPolyData());
	DepthSort->SetDirectionToBackToFront();
	DepthSort->SetCamera(VWidget->Renderer->GetActiveCamera());
	DepthSort->SortScalarsOn();
	DepthSort->Update();
	vtkPolyData* output = vtkPolyData::New();
	output->DeepCopy(DepthSort->GetOutput());
	////output->Update();
	DepthSort->Delete();

	vtkDataArray* TempData = Data->GetVPolyData()->GetPointData()->GetScalars();
	if (TempData != NULL)
	{
		if ((TempData->GetRange() != NULL) && (TempData->GetLookupTable() != NULL))
		{
			vtkLookupTable* LUT = vtkLookupTable::New();
			LUT->DeepCopy(TempData->GetLookupTable());
			output->GetPointData()->GetScalars()->SetLookupTable(LUT);
			output->Modified();
			LUT->Delete();

		}

	}

	VWidget->Renderer->RemoveActor(Data->GetVActor());
	vtkPolyData* P = Data->GetVPolyData();
	Data->SetVPolyData(output);
	Data->GetVMapper()->SetInputData(output);
	Data->GetVMapper()->Modified();
	Data->GetVActor()->Modified();



	P->Delete();
	//	myFile << "1 : " << endl<< endl<< *P << endl;  

	VWidget->Renderer->AddActor(Data->GetVActor());




}

//-----------------------------------
void Animations::SetNumberOfSubSteps(int _Nr)
{
	NumberOfSubSteps = _Nr;
}

//-----------------------------------
void Animations::SlotSetVolumeOpacity()
{
	if (Volume->GetMapper() != NULL)
	{
		SlotSetVolumeColors();


		vtkPiecewiseFunction* OpacityTransferFunction = Volume->GetProperty()->GetScalarOpacity();
		OpacityTransferFunction->RemoveAllPoints();

		OpacityTransferFunction->AddPoint(Histogram->GetMinValue(), 0.00);
		OpacityTransferFunction->AddPoint(Histogram->GetLowerThreshold(), 0.0);
		OpacityTransferFunction->AddPoint(Histogram->GetLowerThreshold() + 1, Histogram->GetLeftY());
		OpacityTransferFunction->AddPoint(Histogram->GetUpperThreshold() - 1, Histogram->GetRightY());
		OpacityTransferFunction->AddPoint(Histogram->GetUpperThreshold(), 0.0);
		OpacityTransferFunction->AddPoint(Histogram->GetMinValue(), 0.00);


		vtkPiecewiseFunction* GradientTransferFunction = Volume->GetProperty()->GetGradientOpacity();

		//GradientTransferFunction->AddPoint(0,0);
		//GradientTransferFunction->AddPoint(Histogram->GetUpperThreshold(),1.0);

	}

}
///-----------------------------------------------
void Animations::SlotSetVolumeColors()
{
	if (Volume->GetMapper() != NULL)
	{
		int i;
		int Range = Histogram->GetMaxValue();

		vtkColorTransferFunction* ColorTransferFunction = Volume->GetProperty()->GetRGBTransferFunction();

		ColorTransferFunction->RemoveAllPoints();

		if (Histogram->GetStretchPaletteMode() == true)
		{
			int LowerThreshold = Histogram->GetLowerThreshold();
			int UpperThreshold = Histogram->GetUpperThreshold();

			double	StretchedScalePalette = (double)(255.0 / (UpperThreshold - LowerThreshold));
			int Step = Round(1 / StretchedScalePalette);
			if (Step == 0) Step = 1;

			if (LowerThreshold > 0)
			{
				ColorTransferFunction->AddRGBPoint(0, 0, 0, 0);
				ColorTransferFunction->AddRGBPoint(LowerThreshold - 1, 0, 0, 0);
			}
			int TMP;

			for (i = LowerThreshold;i <= UpperThreshold;i += Step)
			{
				TMP = Round((i - LowerThreshold) * StretchedScalePalette);
				//ColorTransferFunction->AddRGBPoint(i,(double)(View3DListItem->GetData()->Palette->R[TMP]/255.0),(double)(View3DListItem->GetData()->Palette->G[TMP]/255.0),(double)(View3DListItem->GetData()->Palette->B[TMP]/255.0));	
				ColorTransferFunction->AddRGBPoint(i, (double)(Pal->R[TMP]) / 255.0, (double)(Pal->G[TMP]) / 255.0, (double)(Pal->B[TMP]) / 255.0);

			}

			if (UpperThreshold + 1 < Range)
			{
				ColorTransferFunction->AddRGBPoint(UpperThreshold + 1, 0, 0, 0);
				ColorTransferFunction->AddRGBPoint(Range, 0, 0, 0);
			}
		}
		else
		{
			double ScalePalette = (double)(Histogram->GetMaxValue()) / (double)(255.0);
			for (i = 0;i < 255;i++)
				ColorTransferFunction->AddRGBPoint((double)i * ScalePalette, (double)(Pal->R[i]) / 255.0, (double)(Pal->G[i]) / 255.0, (double)(Pal->B[i]) / 255.0);
		}
	}

}

//-----------------------------------
double Animations::EvaluateHerimteSpline(double val, double* tab, int Size, bool loop)
{

	if ((val < 0) || (val >= Size)) return 0;

	int x[4];
	double m0, m1;

	int i;

	for (i = 0;i < 4;i++)
	{
		x[i] = (int)(val - 1.0 + (double)i);
		if (x[i] < 0) x[i] = 0;
		else if (x[i] >= Size) x[i] = Size - 1;

	}


	double t = val - (int)val;

	const double alpha = 0.0f;
	double t_2 = t * t;
	double t_3 = t_2 * t;


	m0 = ((1 - alpha) / 2.0f) * ((tab[x[1]] - tab[x[0]]) + (tab[x[2]] - tab[x[1]]));
	m1 = ((1 - alpha) / 2.0f) * ((tab[x[2]] - tab[x[1]]) + (tab[x[3]] - tab[x[2]]));


	return	(((2 * t_3) - (3 * t_2) + 1) * tab[x[1]]) + ((t_3 - (2 * t_2) + t) * m0) +
		((t_3 - t_2) * m1) + (((-2 * t_3) + (3 * t_2)) * tab[x[2]]);

}

//----------------
void Animations::AddSubtitleLine() 
{

	QString txt = GuiPanel->animTxtLineEdit->text();
	if (txt.isEmpty() == false)
	{

		QString num;

		num += GuiPanel->CurrentKeyFrameNrEdit->text();num += ",";
		num += GuiPanel->CurrentTKeyFrameNrEdit->text();num += ",";
		for (int i = 0;i < 4;i++)
		{
			num += GuiPanel->animTextPosLineEdit[i]->text();
			num += ",";
		}
		for (int i = 0;i < 3;i++)
		{
			num += GuiPanel->animTextColorLineEdit[i]->text();
			if (i<2) num += ",";
		}
		
		GuiPanel->subtitleTextEdit->appendPlainText(num);
		GuiPanel->subtitleTextEdit->appendPlainText(txt);
	}





}