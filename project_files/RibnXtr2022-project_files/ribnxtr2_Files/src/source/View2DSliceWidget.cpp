#include "View2DSliceWidget.h"
#include <QLabel>
#include <QPolygon>
#include <QFrame>


//View Widget

//----------------------------------------------------------------

View::~View()
{
	DeleteData();
	if (IWidget != NULL) delete IWidget;
	IWidget = NULL;

	if (specialSliceData != NULL) delete[]specialSliceData;

	if (histTab != NULL) delete[]histTab;
}


//--------------------------------------------------
//--------------------------------------------------
View::View(QList<QSlider2*>* _sliderList,LinkedList<DataSet*>	*_DataSetList, QWidget *parent, QString name, Qt::WindowFlags flags) :QWidget(parent, flags)
{
	sliderList = _sliderList;




#pragma omp parallel

	numberOfThreads = omp_get_num_threads();
	threshColorMode = 0;
	revThMode = 0;
	externalImageUpdateMode = false;
	//	pos3D[0] = 0; pos3D[1] = 0;  pos3D[2] = 0;
	viewID = 0;
	counter = 0;
	zoomMode = false;
	setObjectName(name);
	thresholdMaskOpacity = 128;
	PosX = 0;
	PosY = 0;
	ellipseMode = false;
	ellipseDist = 0;
	repaintSpecialSlice = false;

	wheelOn = true;

	locked = false;
	blockMixer = false;
#if defined (VTK)
	volumePoints = NULL;
	volColors = NULL;
#endif
	volPointsOpacity = 255;
	magnyfyingModeOn = false;
	intBuffer0 = NULL;
	intBuffer1 = NULL;
	maskTable = NULL;
	maskTable1 = NULL;

	histTabSize = 0;

	histTab = NULL;
	calculateHistogram = true;

	sendPolyLineMode = 0;
	specialSliceData = NULL;
	specialSliceMaxValue = 0;
	specialSliceMode = 0;
	paintSpecialSliceEnabled = false;
	this->specialSliceOpacity = 255;
	mixer = NULL;
	threshMode = 0;

	//this->setTitle(name);
	Image = NULL;
	ImageR = NULL;
	cycleList = NULL;
	cyclePos = -1;
	cycleListSize = 0;
	TempPaletteArray = NULL;
	TempPaletteArrayForMaskView = NULL;
	PalTemp = NULL;

	isSetSliceBlocked = false;
	ButtonMode = 0;
	DataSetList = _DataSetList;
	PosZ = 0;
	//	InspectionDataSet=NULL;
	MainDataSet = NULL;
	Views2DArray = NULL;
	InspectionWindowsArray = NULL;

	CrossX = 0;
	CrossY = 0;
	CrossType = 1;
	TempInspX = 1;
	TempInspY = 1;
	TempInspZ = 1;
	ShowMask = false;
	ShowMask2 = false;
	MaskOpacity = 255;
	Mask2Opacity = 255;

	//	InspectionMask=NULL;
	//	InspectionMode=2;
	//	InspectionWindow=false;
#if defined (VTK)
	pts = vtkPoints::New();
#endif
	StretchPalette = false;

	QWidget *widget = this;/*new QWidget(this);
						   this->setWidget(widget);*/

	QVBoxLayout *MainLayout = new QVBoxLayout(widget);
	MainLayout->setSpacing(1);
	MainLayout->setMargin(1);

	this->setLayout(MainLayout);

	IWidget = new ImageWidget(widget, "ImageWidget");
	IWidget->maskColor = &MaskColor;
	IWidget->mask2Color = &Mask2Color;
	IWidget->volPointsOpacity = &volPointsOpacity;
	IWidget->colsTab = colsTab;

	IWidget->SetShapes(&shapes);
	MainLayout->addWidget(IWidget);
	IWidget->show();


	QFont font = QFont();
	font.setBold(true);

	//MainLayout->addWidget(IWidget);
	//Q3HBox *Box=new Q3HBox(this);
	Box = new	QFrame/*(1,Vertical,*/(widget);
	MainLayout->addWidget(Box);
	QHBoxLayout *hbl_Box = new QHBoxLayout;
	hbl_Box->setSpacing(0);
	hbl_Box->setMargin(0);
	Box->setLayout(hbl_Box);

	hbl_Box->setSpacing(0);

	//Box->setFrameStyle( Q3Frame::Panel | Q3Frame::Raised );
	Box->setFrameStyle(QFrame::Panel | QFrame::Raised | QFrame::HLine);

	Box->setFixedHeight(18);
	//Box->setMinimumWidth(1);
	//Box->setSpacing(0);
	//	Box->setAutoMask(true);

	NameLabel = new QLabel(Box);
	NameLabel->setFixedHeight(15);
	NameLabel->setMinimumWidth(1);
	NameLabel->setMaximumWidth(80);
	NameLabel->setAlignment(Qt::AlignCenter);
	NameLabel->setFont(font);
	hbl_Box->addWidget(NameLabel);
	QLabel *Lab1 = new QLabel(Box);
	Lab1->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	Lab1->setFixedHeight(12);
	Lab1->setFixedWidth(2);
	//	Lab1->setAutoMask(true);
	hbl_Box->addWidget(Lab1);

	SizesLabel = new QLabel(Box);
	SizesLabel->setFixedHeight(15);
	SizesLabel->setMinimumWidth(1);
	SizesLabel->setMaximumWidth(50);
	//	SizesLabel->setAutoMask(true);
	SizesLabel->setAlignment(Qt::AlignCenter);
	hbl_Box->addWidget(SizesLabel);
	QLabel *Lab2 = new QLabel(Box);
	Lab2->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	Lab2->setFixedHeight(12);
	Lab2->setFixedWidth(2);
	//	Lab2->setAutoMask(true);
	hbl_Box->addWidget(Lab2);

	SliceLabel = new QLabel(Box);
	SliceLabel->setFixedHeight(15);
	SliceLabel->setMinimumWidth(1);
	SliceLabel->setMaximumWidth(55);
	SliceLabel->setAlignment(Qt::AlignCenter);
	hbl_Box->addWidget(SliceLabel);
	MaskColor.setRgb(255, 255, 0);
	Mask2Color.setRgb(0, 255, 255);
	MaskColorX.setRgb(255, 0, 0);
	MaskColorX2.setRgb(0, 0, 255);


	Slider = new QSlider2(sliderList,Qt::Horizontal, this);
	MainLayout->addWidget(Slider);

	Viewer = false;
	connect(Slider, SIGNAL(valueChanged(int)), this, SLOT(SliceSelect(int)));
	connect(Slider, SIGNAL(sliderPressed()), Slider, SLOT(setFocus()));
	connect(IWidget, SIGNAL(ImageReleased()), Slider, SLOT(setFocus()));//2003.09.22


	connect(IWidget, SIGNAL(SignalRefreshGui()), this, SLOT(SlotRedrawWidget()));

	connect(IWidget, SIGNAL(SendPosition(int, int, int)), this, SLOT(FindCoordinates(int, int, int)));
	connect(IWidget, SIGNAL(SendPosition2(int, int, int)), this, SLOT(FindCoordinates2(int, int, int)));

	connect(IWidget, SIGNAL(signalRightWithAltReleased(QMouseEvent*)), this, SLOT(SlotlRightWithAltReleased(QMouseEvent*)));
	connect(IWidget, SIGNAL(signalRightWithAltPressed(QMouseEvent*)), this, SLOT(SlotlRightWithAltPressed(QMouseEvent*)));


	connect(IWidget, SIGNAL(SignalParentPleaseSendViewID()), this, SLOT(SlotSendViewID()));
	//connect(IWidget,SIGNAL(SignalSendMask(unsigned char*)),this,SIGNAL(SignalSendIWidgetMask(unsigned char *)));
	connect(IWidget, SIGNAL(SignalSendMaskImage(QImage)), this, SIGNAL(SignalSendIWidgetMaskImage(QImage)));


	connect(IWidget, SIGNAL(SendMessage(const QString &)), this, SIGNAL(SendMessage(const QString &)));
	connect(IWidget, SIGNAL(SendMessageToLog(const QString &)), this, SIGNAL(SendMessageToLog(const QString &)));


	connect(IWidget, SIGNAL(SignalMoveMask(int)), this, SLOT(SlotMoveMask(int)));

	connect(IWidget, SIGNAL(signalCopyMask()), this, SLOT(SlotCopyMask()));
	connect(IWidget, SIGNAL(signalPasteMask()), this, SLOT(SlotPasteMask()));

	connect(IWidget, SIGNAL(signalCropLines(int*)), this, SLOT(SlotUpdateCropPlanes(int*)));

	pictureIndex = 0;
	autoGrab = 0;
	MaskColor.setAlpha(MaskOpacity);
	Mask2Color.setAlpha(Mask2Opacity);
	connect(IWidget, SIGNAL(signalArrowPressed(int)), this, SLOT(SlotSliderFocus(int)));




}


//--------------------------------------------------

void View::DeleteData()
{
	if (Viewer == true) delete	Image;
	if (ImageR != NULL) delete ImageR;
	if (TempPaletteArray != NULL) delete[]TempPaletteArray;
	if (TempPaletteArrayForMaskView != NULL) delete[]TempPaletteArrayForMaskView;
	if (PalTemp != NULL) delete PalTemp;
	//	if (InspectionMask!=NULL) delete []InspectionMask;
	/*	if (MaskData!=NULL)
	{
	delete []MaskData;
	MaskData=NULL;
	}
	*/	//if (IWidget!=NULL) delete IWidget;

	IWidget->DataCleared();
	Viewer = false;
	data = NULL;
	Image = NULL;
	ImageR = NULL;
	TempPaletteArray = NULL;
	TempPaletteArrayForMaskView = NULL;
	PalTemp = NULL;

	MainDataSet = NULL;

	//	InspectionDataSet=NULL;

	if (InspectionWindowsArray != NULL) delete[]InspectionWindowsArray;
	if (Views2DArray != NULL) delete[]Views2DArray;

	Views2DArray = NULL;
	InspectionWindowsArray = NULL;

	if (specialSliceData != NULL) delete[]specialSliceData;
	specialSliceData = NULL;

	if (intBuffer0 != NULL) delete[]intBuffer0; intBuffer0 = NULL;
	if (intBuffer1 != NULL) delete[]intBuffer1; intBuffer1 = NULL;
	if (maskTable != NULL) delete[]maskTable; maskTable = NULL;
	if (maskTable1 != NULL) delete[]maskTable1; maskTable1 = NULL;



}
//--------------------------------------------------



void	View::InsertNewData(int MainPos, short _view, QString _Title)
{

	//	disconnect(IWidget,SIGNAL(SendPosition(int,int,int)),this,SLOT(FindCoordinates(int,int,int)));

	NameLabel->setText(_Title);
	QString Info;

	ShowMask = false;
	MaskData = NULL;
	ShowMask2 = false;
	Mask2Data = NULL;
	//	InspectionDataSet=NULL;

	if (InspectionWindowsArray != NULL) delete InspectionWindowsArray;
	if (Views2DArray != NULL) delete Views2DArray;

	Views2DArray = NULL;
	InspectionWindowsArray = NULL;
	//Inserting a new data

	if (specialSliceData != NULL) delete[]specialSliceData;
	specialSliceData = NULL;

	int i;
	if (Viewer == true)
	{
		delete	Image;
		delete	ImageR;
		delete[]TempPaletteArray;
		delete[]TempPaletteArrayForMaskView;
		delete  PalTemp;

	}

	Viewer = false;

	MainDataSet = DataSetList->GetItemAtPos(MainPos)->GetData();
	data = MainDataSet->GetRawData();
	//	if ((data->GetMaxValue()==0)) data->FindMinMaxValue();
	//	Range=data->GetMaxValue();
	/////////////////////////////////////
	if (data->GetHistogramCalculated() == false)
		data->CalculateHistogram();


	if (histTab != NULL) delete[]histTab;
	int nrr = data->GetLastHistValue();// +1;
									   //	qDebug() << objectName() << " " << "last hist val:" << nrr;
									   //if (nrr>MAX_HIST_IMG_WIDTH)  nrr = MAX_HIST_IMG_WIDTH;
	histTab = new int[nrr];
	double dd = data->GetLastHistValue();
	if (dd == 0) dd = 1;
	scaleHistFactor = ((double)(nrr - 1)) / dd;
	histTabSize = nrr;

	FirstHistValue = 0;//data->GetFirstHistValue();
	LastHistValue = data->GetLastHistValue();

	if (LastHistValue == FirstHistValue) LastHistValue++;

	Range = LastHistValue - FirstHistValue;
	LTresh = FirstHistValue;
	UTresh = LastHistValue;
	//////////////////////////////////////	


	ScalePalette = (double)(255.0) / (double)(Range);
	view = _view;
	sx = data->GetNx();
	sy = data->GetNy();
	sz = data->GetNz();
	sxy = sx*sy;

	crossection = _view;

	switch (view)
	{
	case 0:
		rx = sx; ry = sy; rz = sz;
		dx = data->GetXscale(); dy = data->GetYscale();
		IWidget->SetName("Transversal");

		break;

	case 1:
		rx = sx; ry = sz; rz = sy;
		dx = data->GetXscale(); dy = data->GetZscale();
		IWidget->SetName("Coronal");

		break;

	case 2:
		rx = sy; ry = sz; rz = sx;
		dx = data->GetYscale(); dy = data->GetZscale();
		IWidget->SetName("Sagittal");

		break;
	}
	IWidget->SetParentName(objectName());
	if (intBuffer0 != NULL) delete[]intBuffer0; intBuffer0 = NULL;
	if (intBuffer1 != NULL) delete[]intBuffer1; intBuffer1 = NULL;
	if (maskTable != NULL) delete[]maskTable; maskTable = NULL;
	if (maskTable1 != NULL) delete[]maskTable1; maskTable1 = NULL;


	intBuffer0 = new int[rx*ry];
	intBuffer1 = new int[rx*ry];
	maskTable = new unsigned char[rx*ry * 3];//bo bedzie tu mask data2 oraz mask table
	maskTable1 = new unsigned char[rx*ry];
	memset(maskTable, 0, rx*ry * 3);
	memset(maskTable1, 0, rx*ry);

	InspX = (int)(TempInspX / 2.0);
	InspY = (int)(TempInspY / 2.0);
	InspZ = (int)(TempInspZ / 2.0);

	Info.sprintf(" %dx%d", rx, ry);
	SizesLabel->setText(Info);

	TempPaletteArray = new int[LastHistValue + 1];
	TempPaletteArrayForMaskView = new int [LastHistValue + 1];
	for (i = 0; i <= Range; i++)
	{
		TempPaletteArray[i] = i;
		TempPaletteArrayForMaskView[i] = i;
	}



	

	Image = new QImage(rx, ry, QImage::Format_ARGB32);
	ImageR = new QImage(rx, ry, QImage::Format_ARGB32);

	PalTemp = new PaletteRGBA();
	for (i = 0; i <= 255; i++)
	{
		PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
		PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
		PalTemp->B[i] = MainDataSet->GetPalette()->B[i];

	}

	disconnect(Slider, SIGNAL(valueChanged(int)), this, SLOT(SliceSelect(int)));
	Slider->setRange(0, rz - 1);
	Slider->setValue(rz / 2);
	SliceNumber = rz / 2;
	connect(Slider, SIGNAL(valueChanged(int)), this, SLOT(SliceSelect(int)));

	IWidget->setValues(Image, rx, ry, dx, dy, maskTable);

	Viewer = true;
	ChangePalette(FirstHistValue, LastHistValue);
	IWidget->show();
}


//--------------------------------------------------
//--------------------------------------------------


void View::updateSlice()
{
	if (locked == false)
	{
		//qDebug() << "redraw nr:" << counter++;
		locked = true;
		if (Viewer == true)
		{	//if (shapes.polygonList.count()>0)
			//qDebug () <<"pos:"+QString::number(shapes.polygonList.at(0).at(0).x())<<" , "<<QString::number(shapes.polygonList.at(0).at(0).y());

			shapes.clickedLeftPoint.setX(CrossX);
			shapes.clickedLeftPoint.setY(CrossY);
			QString Info;
			bool test = true;
			if (externalImageUpdateMode == false)
			{
				if ((mixer != NULL))
				{
					bool test1 = false;
					if ((mixer->getSomethingToMix() == true))
					{
						if (mixer->GetNumberOfLayers() > 1)
						{
							if (blockMixer == false)
							{
								test = PaintMixedDataSlice();
								test1 = true;
							}
						}

					}


					if (test1 == false) PaintMainDataSetSice();
				}
				else
					//if(test == false)
				{
					PaintMainDataSetSice();
				}
				PaintCorrespondingDataSetSlices();
				PaintInspectionWindow();



				PaintMainDataSetMask2Slice();
				//if (magnyfyingModeOn==false)
				PaintMainDataSetMaskSlice();


				PaintSpecialSlice();

				PaintVolMaskSlice();
			}
			Info.sprintf(" %d/%d ", SliceNumber, rz - 1);
			SliceLabel->setText(Info);
			disconnect(Slider, SIGNAL(valueChanged(int)), this, SLOT(SliceSelect(int)));


			IWidget->SetSliceNumber(SliceNumber);
			connect(Slider, SIGNAL(valueChanged(int)), this, SLOT(SliceSelect(int)));

			IWidget->update();
			if (autoGrab == true) SlotGrabImage();

		}
		locked = false;



		//		//lastSnapshotTime1=time1;

		updateTimeID = QDateTime::currentMSecsSinceEpoch();
		emit signalWidgetRedrawed();
	}
	else
	{
		rz = rz;
	}
}
//--------------------------------------------------


void View::UpdatRegSlice(RawDataSet *RegisteredSlice, PaletteRGBA *PaletteReg, unsigned char Opacity)
{
	if (Viewer == true)
	{
		QString Info;
		PaintMainDataSetSice();
		//PaintCorrespondingDataSetSlices();

		PaintRegisteredSlices(RegisteredSlice, PaletteReg, Opacity);

		Info.sprintf(" %d/%d ", SliceNumber, rz - 1);
		SliceLabel->setText(Info);

		IWidget->SetSliceNumber(SliceNumber);
		repaint2();

	}

}

//--------------------------------------------------
//--------------------------------------------------

void View::SliceSelect(int SliceNr)
{
	//2003.09.22
	QString objName = objectName();
	if (SliceNr != SliceNumber)
	{
		//Changing a viewed slice
		SliceNumber = SliceNr;


		updateSlice();
		FindCoordinates(PosX, PosY, 0);
		repaint2();
		emit signalSelectSliceChanged();
	}
}
//-------------------------------------------------
void View::ChangePalette(int LowerThreshold, int UpperThreshold)
{


	//Changes a palette - depends on threshold values
	if (Viewer == true)
	{
		shapes.thresholds[0] = LowerThreshold; shapes.thresholds[1] = UpperThreshold;
		//if (threshMode==0)
		{
			int i;
			if (StretchPalette == true)
			{
				StretchedScalePalette = (double)(255.0 / (UpperThreshold - LowerThreshold));

				if (LowerThreshold>0)
					for (i = 0; i<LowerThreshold; i++)
						TempPaletteArray[i] = 0;

				for (i = LowerThreshold; i <= UpperThreshold; i++)
					TempPaletteArray[i] = Round((i - LowerThreshold)*StretchedScalePalette);

				if (UpperThreshold + 1<Range)
					for (i = UpperThreshold + 1; i <= Range; i++)
						TempPaletteArray[i] = 255;

				LTresh = LowerThreshold;
				UTresh = UpperThreshold;


				for (i = 0; i <= 255; i++)
				{
					PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
					PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
					PalTemp->B[i] = MainDataSet->GetPalette()->B[i];
				}
				//updateSlice();

			}
			else
			{
				int a = Round(ScalePalette*(LowerThreshold));//-FirstHistValue));
				int b = Round(ScalePalette*(UpperThreshold));//-FirstHistValue));


				for (i = 0; i <= this->Range; i++)
				{
					TempPaletteArray[i] = Round(ScalePalette*(i));
				}

				for (i = 0; i <= 255; i++)
				{
					PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
					PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
					PalTemp->B[i] = MainDataSet->GetPalette()->B[i];
				}
				LTresh = LowerThreshold;
				UTresh = UpperThreshold;

			}
		}

		updateSlice();
		repaint2();
	}
}
//--------------------------------------------------
//--------------------------------------------------
void View::ChangeTempPaletteArray(int *_TPaletteArray)
{
	//paskudnie ale tymczasowo
	//delete TempPaletteArray;
	TempPaletteArray = _TPaletteArray;
	updateSlice();
	repaint2();
}
//--------------------------------------------------
//--------------------------------------------------
void View::PaintCross(int x, int y)
{
	disconnect(Slider, SIGNAL(valueChanged(int)), this, SLOT(SliceSelect(int)));
	CrossX = x;
	CrossY = y;
	updateSlice();
	/*QString aa="PaintCross"+QString(" ")+name();
	qWarning(aa);*/

	if (DataSetList->GetCurrentListItem() != NULL)
	{
		DataSet *TData = DataSetList->GetCurrentListItem()->GetData();
		if ((CrossType == 2) && (TData->GetInspectionEnabled() == true))
		{
			int tmp = 0;

			switch (TData->GetInspectionMode())
			{
			case 0:tmp = 6; break;
			case 1:tmp = 2; break;
			case 2:tmp = 4; break;
			case 3:tmp = 3; break;
			}


			SlotSetCrossType(tmp);
			IWidget->RepaintWithCross(x, y);
			//SlotSetCrossType(CrossType);
		}
		else
			IWidget->RepaintWithCross(x, y);
	}
	else
		IWidget->RepaintWithCross(x, y);

	Slider->setValue(SliceNumber);


	connect(Slider, SIGNAL(valueChanged(int)), this, SLOT(SliceSelect(int)));
	update();



}


//--------------------------------------------------

void  View::SlotlRightWithAltReleased(QMouseEvent* event)
{

	if (event->button() & Qt::RightButton)
		if (event->modifiers() & Qt::AltModifier)
		{
			if (
				(pressedX == event->x())
				&&
				(pressedY == event->y())
				)
			emit signalRightWithAltClicked(event->x(), this->height() - event->y(), this);
		}
}

//--------------------------------------------------
void  View::SlotlRightWithAltPressed(QMouseEvent* event)
{

	if (event->button() & Qt::RightButton)
	if (event->modifiers() & Qt::AltModifier)
	{
		pressedX = event->x();
		pressedY = event->y();

		//emit signalRightWithAltClicked(event->x(), this->height() - event->y(), this);
	}
}

void  View::FindCoordinates(int x, int y, int Button)
{


	//QMessageBox::about( 0,"nr",QString::number(x)+" "+QString::number(y)+" Find Boing");
	if (MainDataSet != NULL)
	{
		bool test = true;
		if (ellipseMode == true)//tu sprawdze czy punkt jest w elipsie jak nie to nie przerywam
		{
			//rx,ry

			double c[2] = { rx*0.5, ry*0.5 };

			double xx = x - c[0];
			double yy = y - c[1];

			double aa = (xx*xx) / (c[0] * c[0]);
			double bb = (yy*yy) / (c[1] * c[1]);

			if (aa + bb > 1)
			{

				double x1, y1;
				//	double dist = DistancePointEllipse(c[0], c[1], xx, yy, x1, y1);
				//projection onto ellipse
				double angle = atan2(xx, yy);
				double o1 = c[1] * cos(angle);
				double o2 = c[0] * sin(angle);
				double k1 = sqrt(o1 * o1 + o2*o2);
				double k = 1.0 / k1;
				double nx = k*c[0] * c[1] * sin(angle) + c[0];
				double ny = k*c[0] * c[1] * cos(angle) + c[1];



				qDebug() << xx << " " << yy << nx << " " << ny;
				x = nx;
				y = ny;
			}

		}

		if (test == true)
		{
			int X = 0, Y = 0, Z = 0;
			switch (view)
			{
			case 0:
				X = x;
				Y = y;
				Z = SliceNumber;


				break;

			case 1:
				X = x;
				Y = sy - SliceNumber - 1;
				Z = sz - y - 1;



				break;

			case 2:
				X = SliceNumber;
				Y = x;
				Z = sz - y - 1;
				break;

			}

			//shapes.clickedLeftPoint.setX(X);
			//shapes.clickedLeftPoint.setY(Y);

			PosX = x;
			PosY = y;
			DataSet *TData = DataSetList->GetCurrentListItem()->GetData();
			TData->SetPosXYZ(X, Y, Z);

			emit SendViewId(view, SliceNumber);
			// 	emit signalSendViewUserID(viewID);
			//	emit SendCoordinatesWithButton(X,Y,Z,Button);
			emit SignalSendButton(Button);
			emit SignalSendButtonAndView(Button, view);

			if ((Button >= 3) && (Button <= 5) && (ButtonMode == 1))
			{
				//nie zmieniaj innych sliceow!
			}
			else
			{
				emit SendCoordinates(X, Y, Z);
				//		SlotDisconnectSendCoordsAndUpdate();
			}


		}

	}
}


//--------------------------------------------------
void  View::FindCoordinates2(int x, int y, int Button)
{
	//QMessageBox::about( 0,"nr",QString::number(x)+" "+QString::number(y)+" Find Boing");
	if (MainDataSet != NULL)
	{
		int X = 0, Y = 0, Z = 0;
		switch (view)
		{
		case 0:
			X = x;
			Y = y;
			Z = SliceNumber;

			break;

		case 1:
			X = x;
			Y = sy - SliceNumber - 1;
			Z = sz - y - 1;
			break;

		case 2:
			X = SliceNumber;
			Y = x;
			Z = sz - y - 1;
			break;

		}



		emit SendCoordinates2(X, Y, Z, Button);
		//		SlotDisconnectSendCoordsAndUpdate();





	}
}


//--------------------------------------------------
//--------------------------------------------------


void View::SetSlice(int x, int y, int z)
{
	QString objName = objectName();
	/*if (objectName().compare("Fusion") == 0)
	{
	int i = 0;
	}*/
	int pos3D[3];
	pos3D[0] = x; pos3D[1] = y; pos3D[2] = z;
	if ((MainDataSet != NULL) && (data != NULL))
	{
		int voxelVal = data->GetVoxelValue(pos3D[0], pos3D[1], pos3D[2]);
		double voxelValD = data->dicomMinValue + voxelVal*data->dicomRescaleValue;
		IWidget->SetPos3D(pos3D);
		IWidget->SetVoxelRealValue(voxelValD);

	}
	if (isSetSliceBlocked != true)
	{

		int a = 0, b = 0;

		switch (view)
		{
		case 0:
			PosX = x;			PosY = y;				PosZ = z;
			a = x;			b = y;
			break;
		case 1:
			PosX = x;			PosY = sz - z - 1;		PosZ = sy - y - 1;
			a = x;			b = sz - z - 1;
			break;
		case 2:

			PosX = y;	PosY = sz - z - 1;		PosZ = x;
			a = y;		b = sz - z - 1;

			//PosX=sy-y-1;	PosY=sz-z-1;		PosZ=x;
			//a=sy-y-1;		b=sz-z-1;
			break;
		}
		//	QString aa="SetSlice"+QString(" ")+name()+QString(" ")+QString::number(CrossType);
		//qWarning(aa);

		SliceNumber = PosZ;
		IWidget->SetPosZ(PosZ);

		double xd = PosX*this->dx;
		double yd = PosY*this->dy;
		IWidget->SetPos2D(xd, yd);

		SlotSetCrossType(CrossType);
		PaintCross(a, b);

	}

}

//--------------------------------------------------
//--------------------------------------------------

void View::ChangePaletteType()
{

	int i;
	if (StretchPalette == true)
		for (i = 0; i <= 255; i++)
		{
			PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
			PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
			PalTemp->B[i] = MainDataSet->GetPalette()->B[i];
		}

	ChangePalette(LTresh, UTresh);
	PaintCross(CrossX, CrossY);
}
//--------------------------------------------------
void View::ChangePaletteTypeR()
{
	PaintCross(CrossX, CrossY);
}
//--------------------------------------------------
void View::SlotSetCrossColor(QColor CrossColor)
{


	unsigned int *a = new unsigned int;
	unsigned char *b = (unsigned char *)a;
	b[0] = CrossColor.blue();
	b[1] = CrossColor.green();
	b[2] = CrossColor.red();
	IWidget->SetCrossColorValue(*a);
	IWidget->SetCrossColor(CrossColor);
	PaintCross(CrossX, CrossY);
}
//--------------------------------------------------
void	View::SlotSetMaskColor(QColor	_MaskColor)
{
	MaskColor = _MaskColor;

	PaintCross(CrossX, CrossY);
}
//--------------------------------------------------
void	View::SlotSetMask2Color(QColor	_MaskColor)
{
	Mask2Color = _MaskColor;
	PaintCross(CrossX, CrossY);
}
//------------------
void	View::SlotSetMaskColorX(QColor	_MaskColor)
{
	MaskColorX = _MaskColor;
	PaintCross(CrossX, CrossY);
}
//------------------
void	View::SlotSetMaskColorX2(QColor	_MaskColor)
{
	MaskColorX2 = _MaskColor;
	PaintCross(CrossX, CrossY);
}
//--------------------------------------------------
void View::SlotSetCrossType(int CrossType)
{
	/*QString aa=name()+QString("InVIEW SetCrossType")+QString::number(CrossType);
	qWarning(aa);*/
	IWidget->SetCrossType(CrossType);
}
//--------------------------------------------------
void View::SlotSwitchToStretchedPaletteMode()
{
	if (StretchPalette == false)
	{
		StretchPalette = true;
		ChangePalette(LTresh, UTresh);
	}
}
//----------------------------------------------------
void View::SlotSwitchToNormalPaletteMode()
{
	if (StretchPalette == true)
	{
		int i;
		for (i = 0; i <= Range; i++)
			TempPaletteArray[i] = i;

		StretchPalette = false;
		ChangePalette(LTresh, UTresh);
	}
}
//------------------------------------------------------
void View::SlotSetInterpolation(int Interpolation)
{
	IWidget->SetInterpolation(Interpolation);
	PaintCross(CrossX, CrossY);
}
//-------------------------------------------------------
void	View::SlotSetOpacity()
{
	updateSlice();
	repaint2();
}

//--------------------------------------------------------
void	View::SlotSetMaskOpacity(int _MaskOpacity)
{
	MaskOpacity = _MaskOpacity;
	MaskColor.setAlpha(MaskOpacity);
	//IWidget->maskOpacity = MaskOpacity;
	updateSlice();

	repaint2();

}
void	View::SlotSetThresholdMaskOpacity(int _MaskOpacity)
{
	thresholdMaskOpacity = _MaskOpacity;
	//MaskColor.setAlpha(MaskOpacity);
	//IWidget->maskOpacity = MaskOpacity;
	updateSlice();

	repaint2();

}


//---------------------------------------------------------
void	View::SlotSetMask2Opacity(int _MaskOpacity)
{
	Mask2Opacity = _MaskOpacity;
	Mask2Color.setAlpha(Mask2Opacity);
	updateSlice();
	repaint2();
}
//---------------------------------------------------------
void	View::SlotSwitchInspectionMode()
{
	/*QString aa="SlotSwitchInspectionMode"+QString(" ")+name();
	qWarning(aa);*/
	SlotSetCrossType(CrossType);
	if (data != NULL)		PaintCross(CrossX, CrossY);
}
//--------------------------------------------------
void	View::SlotSetCrossMode(int Mode)
{/*
 QString aa="SlotSetCrossMode"+QString(" ")+name();
 qWarning(aa);*/
	CrossType = Mode;
	SlotSetCrossType(CrossType);
	PaintCross(CrossX, CrossY);

	// ta funkcja sie rozni od poprzedniej (set cross mode) 
	// tym ze ona permanentnie zmienia rodzaj crossa
}
//--------------------------------------------------
void	View::SetInspectionWindowSize(int x, int y, int z)
{
	TempInspX = x;
	TempInspY = y;
	TempInspZ = z;

	InspX = (int)(TempInspX / 2.0);
	InspY = (int)(TempInspY / 2.0);
	InspZ = (int)(TempInspZ / 2.0);

	IWidget->SetInspectionWindowSizes(InspX, InspY, InspZ);

	int TWidth = 2 * InspX + 1;
	int THeight = 2 * InspY + 1;

	unsigned char *InspectionMask = new unsigned char[TWidth*THeight];
	int RX2, RY2;
	double temp;
	double temp2;
	unsigned char *TempInspMask = InspectionMask;
	DataSet *TData = DataSetList->GetCurrentListItem()->GetData();

	switch (TData->GetInspectionMode())
	{

	case 0:
		//porostokat
		memset(InspectionMask, 255, TWidth*THeight);
		break;

	case 1:
		// elipsa
		RX2 = InspX;
		RY2 = InspY;
		memset(InspectionMask, 0, THeight*TWidth); //maluje na 0 calosc
		DrawEllipseUCHAR(TempInspMask, TWidth, THeight, InspX, InspY, RX2, RY2, 255); //rysuje elipse 255
		ScanlineXFillArrayUCHAR(TempInspMask, TWidth, THeight, 255, 255);  // wypelniam srodek elipsy - wypelniam kolorem 255 to co jest od 255 z lewej do 255 z prawej
		SmoothFilterImageTab(TempInspMask, TWidth, THeight);
		break;

	case 2:
		//trojkacik
		temp = 2.0*(double)InspY / (double)InspX;
		temp2 = 2.0*InspY;
		memset(InspectionMask, 0, THeight*TWidth); //maluje na 0 calosc
		DrawLineUCHAR(TempInspMask, TWidth, THeight, InspX, 0, TWidth - 1, THeight - 1, 255);
		DrawLineUCHAR(TempInspMask, TWidth, THeight, 0, THeight - 1, TWidth - 1, THeight - 1, 255);
		DrawLineUCHAR(TempInspMask, TWidth, THeight, 0, THeight - 1, InspX, 0, 255);
		ScanlineXFillArrayUCHAR(TempInspMask, TWidth, THeight, 255, 255);  // wypelniam srodek trojkata - wypelniam kolorem 255 to co jest od 255 z lewej do 255 z prawej
		SmoothFilterImageTab(TempInspMask, TWidth, THeight);
		break;

	case 3:
		// kolko
		temp = ((double)InspX*dx<(double)InspY*dy) ? InspX : (double)InspY*dy / dx;
		temp2 = ((double)InspY*dy<(double)InspX*dx) ? InspY : (double)InspX*dx / dy;
		memset(InspectionMask, 0, THeight*TWidth); //maluje na 0 calosc
		DrawEllipseUCHAR(TempInspMask, TWidth, THeight, InspX, InspY, Round(temp), Round(temp2), 255); //rysuje elipse 255
		ScanlineXFillArrayUCHAR(TempInspMask, TWidth, THeight, 255, 255);  // wypelniam srodek elipsy - wypelniam kolorem 255 to co jest od 255 z lewej do 255 z prawej
		SmoothFilterImageTab(TempInspMask, TWidth, THeight);
		break;
	}


	switch (view)
	{
	case 0:
		if (TData->GetInspectionArray0() != NULL) TData->DeleteInspectionArray0();
		TData->SetInspectionArray0(InspectionMask);
		break;

	case 1:
		if (TData->GetInspectionArray1() != NULL)	TData->DeleteInspectionArray1();
		TData->SetInspectionArray1(InspectionMask);
		break;

	case 2:
		if (TData->GetInspectionArray2() != NULL) TData->DeleteInspectionArray2();
		TData->SetInspectionArray2(InspectionMask);
		break;
	}
	if (data != NULL) PaintCross(CrossX, CrossY);

}
//---------------------------------------------------------
void	View::InsertMaskData(unsigned char *_MaskData)
{
	MaskData = NULL;
	MaskData = _MaskData;
	ShowMask = true;
	updateSlice();
	emit signalMaskDataInserted(MaskData);
	IWidget->update();
}
//----------------------------------------
void	View::InsertMask2Data(unsigned char *_MaskData)
{
	Mask2Data = NULL;
	Mask2Data = _MaskData;
	ShowMask2 = true;
	updateSlice();
	repaint2();
}
//---------------------------------------------------------
bool	View::PaintMixedDataSlice()
{
	if ((mixer != NULL) && (MainDataSet == mixer->GetMainDataSet()))
	{

		QColor colUpper(upperColor);
		QColor colLower(lowerColor);
		if (threshColorMode == 0)
		{
			colUpper = QColor(PalTemp->R[255], PalTemp->G[255], PalTemp->B[255]);
			colLower = QColor(PalTemp->R[0], PalTemp->G[0], PalTemp->B[0]);
		}
		unsigned char colUp[3] = { colUpper.red(),colUpper.green(),colUpper.blue() };
		unsigned char colLow[3] = { colLower.red(),colLower.green(),colLower.blue() };



		int LowerThreshold = LTresh;
		int UpperThreshold = UTresh;

		if (UpperThreshold - LowerThreshold == 1) LowerThreshold = UpperThreshold;
		int i, j;
		unsigned char *dataX;
		RawDataSet *dataT = data;
		//	(dataT->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 : dataX = (unsigned char *)Image->bits();
		dataX = (unsigned char *)Image->bits();
		int cc;
		int pos[3] = { PosX, PosY, PosZ };
		unsigned short *ddata = mixer->SlotGenerateMixedSliceImage(SliceNumber);


		for (int i = 0; i < rx*ry; i++)
		{
			if (ddata[i] > data->GetMaxValue())
			{
				i = i;
			}
		}
		unsigned int* dataXI = (unsigned int*)Image->bits();
		unsigned short *savedBuffer = mixer->GetRememberBuffer();
		if ((calculateHistogram == true) && (histTab != NULL))
		{
			GetTransHistTab(ddata, rx*ry);
		}
		unsigned char maskR = MaskColorX.red();
		unsigned char maskG = MaskColorX.green();
		unsigned char maskB = MaskColorX.blue();

		unsigned char maskR2 = MaskColorX2.red();
		unsigned char maskG2 = MaskColorX2.green();
		unsigned char maskB2 = MaskColorX2.blue();
		unsigned char r, g, b;
		int ppos = 0;
		if (ddata == NULL) return false;
		if ((threshMode == 1) && ((LTresh>FirstHistValue) || ((UTresh<LastHistValue))))
		{

			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.blue() - *dataXM)<<8)*MaskOpacity)>>16);
			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.green() - *dataXM)<<8)*MaskOpacity)>>16);
			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.red() - *dataXM)<<8)*MaskOpacity)>>16);
			int opacity = thresholdMaskOpacity;
			unsigned char maskVal;


			switch (view)
			{


			case 0:
#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sy; j++)
				{
					for (int i = 0; i < sx; i++)
					{
						int pos2D = i + j * sx;
						int cc = ddata[pos2D];//dataT->GetVoxelValue(i, j, SliceNumber);//-FirstHistValue;
																	 //col=((cc<LowerThreshold)||(cc>UpperThreshold))?0:TempPaletteArray[cc];
																	 //col=TempPaletteArray[cc];


						

						SetColorAndMask(cc, LowerThreshold, UpperThreshold, maskR, maskG, maskB, maskR2, maskG2, maskB2, dataXI, pos2D);


					}
				}
				break;
			case 1:
#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sz; j++)
				{
					for (int i = 0; i < sx; i++)
					{
						int pos2D = i + j * sx;
						cc = ddata[pos2D];//dataT->GetVoxelValue(i, sy - SliceNumber - 1, sz - j - 1);//-FirstHistValue;

					

						SetColorAndMask(cc, LowerThreshold, UpperThreshold, maskR, maskG, maskB, maskR2, maskG2, maskB2, dataXI, pos2D);

					}
				}
				break;
			case 2:
#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sz; j++)
				{
					for (int i = 0; i < sy; i++)
					{
						int pos2D = i + j * sy;
						cc = ddata[pos2D];//dataT->GetVoxelValue(SliceNumber, i, sz - j - 1);//-FirstHistValue;

						

						SetColorAndMask(cc, LowerThreshold, UpperThreshold, maskR, maskG, maskB, maskR2, maskG2, maskB2, dataXI, pos2D);

					}
				}
				break;
			}
		}
		else
		{
			int cc1;
			switch (view)
			{

			case 0:
				for (j = 0; j<sy; j++)
					for (i = 0; i<sx; i++)
					{
						//cc=dataT->GetVoxelValue(i,j,SliceNumber);//-FirstHistValue;
						cc1 = savedBuffer[ppos];
						cc = ddata[ppos++];
						col = TempPaletteArray[cc];
						if (cc1 < LowerThreshold) { r = colLow[0]; g = colLow[1]; b = colLow[2]; }
						else if (cc1>UpperThreshold) { r = colUp[0]; g = colUp[1]; b = colUp[2]; }
						else { r = PalTemp->R[col]; g = PalTemp->G[col]; b = PalTemp->B[col]; }
						*(dataX++) = b;						*(dataX++) = g;						*(dataX++) = r; *(dataX++) = 255;
						//	dataX++;
					}
				break;
			case 1:
				for (j = 0; j<sz; j++)
					for (i = 0; i<sx; i++)
					{
						//cc=dataT->GetVoxelValue(i,sy-SliceNumber-1,sz-j-1);//-FirstHistValue;
						cc1 = savedBuffer[ppos];
						cc = ddata[ppos++];
						col = TempPaletteArray[cc];
						if (cc1 <LowerThreshold) { r = colLow[0]; g = colLow[1]; b = colLow[2]; }
						else if (cc1>UpperThreshold) { r = colUp[0]; g = colUp[1]; b = colUp[2]; }
						else { r = PalTemp->R[col]; g = PalTemp->G[col]; b = PalTemp->B[col]; }
						*(dataX++) = b;						*(dataX++) = g;						*(dataX++) = r; *(dataX++) = 255;
						//dataX++;
					}
				break;
			case 2:
				for (j = 0; j<sz; j++)
					for (i = 0; i<sy; i++)
					{
						//cc=dataT->GetVoxelValue(SliceNumber,i,sz-j-1);//-FirstHistValue;
						cc1 = savedBuffer[ppos];
						cc = ddata[ppos++];
						col = TempPaletteArray[cc];
						if (cc1 < LowerThreshold) { r = colLow[0]; g = colLow[1]; b = colLow[2]; }
						else if (cc1>UpperThreshold) { r = colUp[0]; g = colUp[1]; b = colUp[2]; }
						else { r = PalTemp->R[col]; g = PalTemp->G[col]; b = PalTemp->B[col]; }
						*(dataX++) = b;						*(dataX++) = g;						*(dataX++) = r; *(dataX++) = 255;
						//dataX++;
					}
				break;
			}
		}

		return true;
	}
	return false;
}
//----------------------------------

void View::SetColorAndMask(int cc, int LowerThreshold, int UpperThreshold, unsigned  char maskR, unsigned  char maskG, unsigned  char maskB, unsigned  char maskR2, unsigned  char maskG2, unsigned  char maskB2,unsigned int* dataXI, int pos2D)
{
	dataXI += pos2D;
	int opacity = thresholdMaskOpacity;
	unsigned char* dataX = (unsigned char*)dataXI;

	unsigned char r, g, b;
	unsigned char col = TempPaletteArrayForMaskView[cc];


	//LowerThreshold = tmpLth;
	//UpperThreshold = tmpUth;

	

	unsigned char maskVal = (revThMode==0)?
		(((cc < LowerThreshold) || (cc > UpperThreshold)) ? 0 : 1)	:

		(((cc < LowerThreshold) || (cc > UpperThreshold)) ? ((cc < LowerThreshold)?2:1 ): 0) 	;
	r = PalTemp->R[col];		g = PalTemp->G[col];		b = PalTemp->B[col];



	if (maskVal == 1)
	{
		*(dataX++) = b + (unsigned char)((((unsigned int)(maskB - b) << 8) * opacity) >> 16);//PalTemp->B[col];
		*(dataX++) = g + (unsigned char)((((unsigned int)( maskG - g) << 8) * opacity) >> 16);//PalTemp->G[col];
		*(dataX++) = r + (unsigned char)((((unsigned int)( maskR - r) << 8) * opacity) >> 16);//PalTemp->R[col];
		
	}
	else if (maskVal == 2)
	{
		*(dataX++) = b + (unsigned char)((((unsigned int)( maskB2 - b) << 8) * opacity) >> 16);//PalTemp->B[col];
		*(dataX++) = g + (unsigned char)((((unsigned int)( maskG2 - g) << 8) * opacity) >> 16);//PalTemp->G[col];
		*(dataX++) = r + (unsigned char)((((unsigned int)( maskR2 - r) << 8) * opacity) >> 16);//PalTemp->R[col];

	}
	else
	{
		*(dataX++) = b;
		*(dataX++) = g;
		*(dataX++) = r;
		
	}
}
//--------------------------------

void View::SetColor2(int cc, int LowerThreshold, int UpperThreshold, unsigned  char * colLow, unsigned  char * colUp, unsigned int* dataXI, int pos2D)
{
	dataXI += pos2D;
	int opacity = thresholdMaskOpacity;
	unsigned char* dataX = (unsigned char*)dataXI;
	unsigned char r, g, b;

	unsigned char col = TempPaletteArray[cc];
	if (cc < LowerThreshold) { r = colLow[0]; g = colLow[1]; b = colLow[2]; }
	else if (cc > UpperThreshold) { r = colUp[0]; g = colUp[1]; b = colUp[2]; }
	else { r = PalTemp->R[col]; g = PalTemp->G[col]; b = PalTemp->B[col]; }
	*(dataX++) = b;						*(dataX++) = g;						*(dataX++) = r;
	//dataX++;

}
//------------------------
void	View::PaintMainDataSetSice()
{
	QColor colUpper(upperColor);
	QColor colLower(lowerColor);
	if (threshColorMode == 0)
	{
		colUpper = QColor(PalTemp->R[255], PalTemp->G[255], PalTemp->B[255]);
		colLower = QColor(PalTemp->R[0], PalTemp->G[0], PalTemp->B[0]);
	}
	unsigned char colUp[3] = { colUpper.red(),colUpper.green(),colUpper.blue() };
	unsigned char colLow[3] = { colLower.red(),colLower.green(),colLower.blue() };

	RawDataSet *dataT;
	if ((cycleList != NULL) && (cycleListSize>1) && (cyclePos >= 0) && (cyclePos<cycleListSize))
	{
		if ((DataSetList->GetItemAtPos(cycleList[cyclePos])) != NULL)
			dataT = DataSetList->GetItemAtPos(cycleList[cyclePos])->GetData()->GetRawData();
		else
			dataT = data;

	}
	else
		dataT = data;

	int LowerThreshold = LTresh;
	int UpperThreshold = UTresh;

	if (UpperThreshold - LowerThreshold == 1) LowerThreshold = UpperThreshold;
	int i, j;


	/*if ((calculateHistogram == true) && (histTab != NULL))
	{
	GetTransHistTab(dataT->GetDataArray() + SliceNumber*sx*sy, sx*sy);
	}*/

	unsigned char *dataX;
	//(dataT->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 : dataX = (unsigned char *)Image->bits();
	dataX = (unsigned char *)Image->bits();

	unsigned int *dataXI= (unsigned int*)Image->bits();
	
	int cc;
	unsigned char r, g, b;



	unsigned char maskR = MaskColorX.red();
	unsigned char maskG = MaskColorX.green();
	unsigned char maskB = MaskColorX.blue();

	unsigned char maskR2 = MaskColorX2.red();
	unsigned char maskG2 = MaskColorX2.green();
	unsigned char maskB2 = MaskColorX2.blue();

	if (dataT->GetRGBExists() == false)
		//if (StretchPalette==true)
		//if((threshMode==0)&&(LTresh>FirstHistValue)||(UTresh<LastHistValue))


	


		if ((threshMode == 1) && ((LTresh>FirstHistValue) || ((UTresh<LastHistValue))))
		{

			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.blue() - *dataXM)<<8)*MaskOpacity)>>16);
			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.green() - *dataXM)<<8)*MaskOpacity)>>16);
			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.red() - *dataXM)<<8)*MaskOpacity)>>16);
			
			unsigned char maskVal;
			int opacity = thresholdMaskOpacity;


			switch (view)
			{


			case 0:
#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sy; j++)
				{
					for (int i = 0; i < sx; i++)
					{
						int cc = dataT->GetVoxelValue(i, j, SliceNumber);//-FirstHistValue;
																	 //col=((cc<LowerThreshold)||(cc>UpperThreshold))?0:TempPaletteArray[cc];
																	 //col=TempPaletteArray[cc];


						int pos2D = i + j * sx;

						SetColorAndMask(cc, LowerThreshold, UpperThreshold, maskR, maskG, maskB, maskR2, maskG2, maskB2, dataXI, pos2D);

						
					}
				}
				break;
			case 1:
#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sz; j++)
				{
					for (int i = 0; i < sx; i++)
					{
						cc = dataT->GetVoxelValue(i, sy - SliceNumber - 1, sz - j - 1);//-FirstHistValue;

						int pos2D = i + j * sx;

						SetColorAndMask(cc, LowerThreshold, UpperThreshold, maskR, maskG, maskB, maskR2, maskG2, maskB2, dataXI, pos2D);
						
					}
				}
				break;
			case 2:
#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sz; j++)
				{
					for (int i = 0; i < sy; i++)
					{
						cc = dataT->GetVoxelValue(SliceNumber, i, sz - j - 1);//-FirstHistValue;

						int pos2D = i + j * sy;

						SetColorAndMask(cc, LowerThreshold, UpperThreshold, maskR, maskG, maskB, maskR2, maskG2, maskB2, dataXI, pos2D);
						
					}
				}
				break;
			}
		}
		else
		{
			unsigned char r, g, b;
			switch (view)
			{

			case 0:

#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sy; j++)
				{
					for (int i = 0; i < sx; i++)
					{
						int cc = dataT->GetVoxelValue(i, j, SliceNumber);
						int pos2D = i + j * sx;

						SetColor2(cc, LowerThreshold, UpperThreshold, colLow, colUp,  dataXI, pos2D);
						
					}
				}
				break;
			case 1:
#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sz; j++)
				{
					for (int i = 0; i < sx; i++)
					{
						cc = dataT->GetVoxelValue(i, sy - SliceNumber - 1, sz - j - 1);//-FirstHistValue;

						int pos2D = i + j * sx;

						SetColor2(cc, LowerThreshold, UpperThreshold, colLow, colUp, dataXI, pos2D);

					}
				}
				break;
			case 2:
#pragma omp  for  nowait ordered schedule (guided)
				for (int j = 0; j < sz; j++)
				{
					for (int i = 0; i < sy; i++)
					{
						cc = dataT->GetVoxelValue(SliceNumber, i, sz - j - 1);//-FirstHistValue;

						int pos2D = i + j * sy;


						SetColor2(cc, LowerThreshold, UpperThreshold, colLow, colUp, dataXI, pos2D);

					}
				}
				break;
			}
		}
	else
	{
		int pos;
		//RGB-temp
		int Mode = dataT->GetMode();
		// ushort,ushort,ushort
		if (Mode == 3)
		{
			switch (view)
			{
			case 0:
				for (j = 0; j<sy; j++)
					for (i = 0; i<sx; i++)
					{
						pos = dataT->GetVoxelPos(i, j, SliceNumber);
						*(dataX++) = (unsigned char)*(dataT->GetBChannelArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetGChannelArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetRChannelArray() + pos);
						dataX++;
					}
				break;
			case 1:
				for (j = 0; j<sz; j++)
					for (i = 0; i<sx; i++)
					{
						pos = dataT->GetVoxelPos(i, sy - SliceNumber - 1, sz - j - 1);
						*(dataX++) = (unsigned char)*(dataT->GetBChannelArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetGChannelArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetRChannelArray() + pos);
						dataX++;
					}
				break;
			case 2:
				for (j = 0; j<sz; j++)
					for (i = 0; i<sy; i++)
					{
						pos = dataT->GetVoxelPos(SliceNumber, i, sz - j - 1);
						*(dataX++) = (unsigned char)*(dataT->GetBChannelArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetGChannelArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetRChannelArray() + pos);
						dataX++;
					}
				break;
			}
		}
		else if (Mode == 4)
		{
			//ushort,uchar,uchar
			switch (view)
			{
			case 0:

				for (j = 0; j<sy; j++)
					for (i = 0; i<sx; i++)
					{
						pos = dataT->GetVoxelPos(i, j, SliceNumber);
						*(dataX++) = *(dataT->GetBChannelUCHARArray() + pos);
						*(dataX++) = *(dataT->GetGChannelUCHARArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetRChannelArray() + pos);
						dataX++;
					}
				break;
			case 1:
				for (j = 0; j<sz; j++)
					for (i = 0; i<sx; i++)
					{
						pos = dataT->GetVoxelPos(i, sy - SliceNumber - 1, sz - j - 1);
						*(dataX++) = *(dataT->GetBChannelUCHARArray() + pos);
						*(dataX++) = *(dataT->GetGChannelUCHARArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetRChannelArray() + pos);
						dataX++;
					}
				break;
			case 2:
				for (j = 0; j<sz; j++)
					for (i = 0; i<sy; i++)
					{
						pos = dataT->GetVoxelPos(SliceNumber, i, sz - j - 1);
						*(dataX++) = *(dataT->GetBChannelUCHARArray() + pos);
						*(dataX++) = *(dataT->GetGChannelUCHARArray() + pos);
						*(dataX++) = (unsigned char)*(dataT->GetRChannelArray() + pos);
						dataX++;
					}
				break;
			}
		}
	}
}
//---------------------------------------------------------
#if defined (VTK)
void View::SetVolumeStructuredPoints(vtkStructuredPoints *volPts, vtkColorTransferFunction		*cols)
{
	volumePoints = volPts;
	volColors = cols;

	double rgb[3];
	int pos = 0;
	int cls = volColors->GetNumberOfAvailableColors();


	for (int i = 0; i < 256; i++)
	{

		volColors->GetColor(i, rgb);
		colsTab[pos++] = 255.0*rgb[0];	colsTab[pos++] = 255.0*rgb[1];	colsTab[pos++] = 255.0* rgb[2];

	}

	//updateSlice();

	//repaint2();

};
#endif
//-------------------------------------------------------------
void	View::SlotSetVolumeOpacity(int opacity)
{
	volPointsOpacity = opacity;
	updateSlice();
};
//-----------------------------------
void	View::PaintVolMaskSlice()
{
#if defined (VTK)

	if ((volPointsOpacity > 0) && (volumePoints != NULL) && (volColors != NULL))
	{
		int bounds[6];
		volumePoints->GetExtent(bounds);

		int newN[3] = { bounds[1] + 1,bounds[3] + 1,bounds[5] + 1 };
		int n[3];
		data->GetN(n);
		if (newN[0] * newN[1] * newN[2] == n[0] * n[1] * n[2])
		{

			unsigned short  *ptr = (unsigned short *)(volumePoints->GetScalarPointer(0, 0, 0));

			int i, j, il = 0;
		//	unsigned char *dataX, *dataXM;
			//(data->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 : dataX = (unsigned char *)Image->bits();
			//dataXM = (unsigned char *)Image->bits();
			//dataXM = dataX;

			int slicesize = data->GetSliceSize();
			int nx = data->GetNx();
			int nj, slconst;
			int sz1 = sz - 1;
			int sy1 = sy - 1;

			unsigned char *maskTableX = maskTable + 2 * rx*ry;

			unsigned char	*dataXB1 = maskTableX;
			unsigned char	*dataXB2 = maskTableX;

			//	char	*MaskTable = new char[rx*ry];
			/*	unsigned char	*dataXB1 = maskTable1;
			unsigned char	*dataXB2 = maskTable1;*/
			unsigned short val;
			int counter = 0;

			int hist[256] = { 0 };

			switch (view)
			{
			case 0:
				slconst = SliceNumber*slicesize;
				for (j = 0; j < sy; j++)
				{
					nj = slconst + (nx*j);
					for (i = 0; i < sx; i++)
					{
						val = *(ptr + (nj + i) * 2 + 1);

						if (val < 256)	hist[val] ++;


						*(dataXB1++) = ((val > 0) && (val < 256)) ? (unsigned char)val : 0;

						if (((val > 0) && (val < 256)))

							counter++;
					}
				}
				il = sx*sy;
				break;
			case 1:
				slconst = (sy - SliceNumber - 1)*nx;
				for (j = 0; j < sz; j++)
				{
					nj = ((sz1 - j)*slicesize + slconst);
					for (i = 0; i < sx; i++)
					{
						val = *(ptr + (nj + i) * 2 + 1);

						if (val < 256) hist[val] ++;
						//*(dataXB1++) = (*(MaskData + nj + i) == 1) ? 1 : 0;

						*(dataXB1++) = ((val > 0) && (val < 256)) ? (unsigned char)val : 0;
						if (((val > 0) && (val < 256))) counter++;
					}
				}
				il = sz*sx;
				break;
			case 2:
				for (j = 0; j < sz; j++)
				{
					nj = ((sz1 - j)*slicesize + SliceNumber);
					for (i = 0; i < sy; i++)
					{
						//*(dataXB1++) = (*(MaskData + nj + (i)*nx) == 1) ? 1 : 0;
						val = *(ptr + (nj + (i)*nx) * 2 + 1);

						if (val < 256)hist[val] ++;

						*(dataXB1++) = ((val > 0) && (val < 256)) ? (unsigned char)val : 0;
						if (((val > 0) && (val < 256))) counter++;

					}
				}
				il = sz*sy;
				break;
			}


			//

			//qDebug() << this->objectName() << " : " << counter <<  " nr of vals: " ;
			//int sum = 0;
			//for (int s = 1; s < 256; s++)
			//{

			//	sum += hist[s];
			//	if (hist[s] > 0)
			//	{
			//		qDebug() << "val: " << s << " count: " << hist[s];
			//	}

			//}
			//unsigned char col;
			//for (i = 0; i<il; i++)
			//{
			//	if (*dataXB2>0)
			//	{
			//		col = (*dataXB2)*3;
			//		*(dataXM++) += (unsigned char)((((unsigned int)(colsTab[col+2] - *dataXM) << 8)*volPointsOpacity) >> 16);
			//		*(dataXM++) += (unsigned char)((((unsigned int)(colsTab[col+1] - *dataXM) << 8)*volPointsOpacity) >> 16);
			//		*(dataXM++) += (unsigned char)((((unsigned int)(colsTab[col] - *dataXM) << 8)*volPointsOpacity) >> 16);

			///*			*(dataXM++) += (unsigned char)((((unsigned int)(255 - *dataXM) << 8)*volPointsOpacity) >> 16);
			//		*(dataXM++) += (unsigned char)((((unsigned int)(127 - *dataXM) << 8)*volPointsOpacity) >> 16);
			//		*(dataXM++) += (unsigned char)((((unsigned int)(127 - *dataXM) << 8)*volPointsOpacity) >> 16);*/
			//	}
			//	else
			//		dataXM += 3;

			//	dataXM++; // skip alpha
			//	dataXB2++;
			//}
		}
		else
		{

			//cout << "jakis problemik" << endl;
		}
	}
#endif
}
//------------------------------------------
void	View::PaintMainDataSetMaskSlice()
{
	if ((ShowMask == true)&&(MaskData!=NULL)) //lub opacity jest =0 - dopisz!
	{
		int i, j, il = 0;
		unsigned char *dataX, *dataXM;
		//(data->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 : dataX = (unsigned char *)Image->bits();
		dataX = (unsigned char *)Image->bits();
		dataXM = dataX;

		int slicesize = data->GetSliceSize();
		int nx = data->GetNx();
		int nj, slconst;
		int sz1 = sz - 1;
		int sy1 = sy - 1;

		//	char	*MaskTable = new char[rx*ry];
		unsigned char	*dataXB1 = maskTable;
		unsigned char	*dataXB2 = maskTable;
		switch (view)
		{
		case 0:
			slconst = SliceNumber*slicesize;
			for (j = 0; j<sy; j++)
			{
				nj = slconst + nx*j;
				for (i = 0; i<sx; i++)
					*(dataXB1++) = (*(MaskData + nj + i) == 1) ? 1 : 0;
			}
			il = sx*sy;
			break;
		case 1:
			slconst = (sy - SliceNumber - 1)*nx;
			for (j = 0; j<sz; j++)
			{
				nj = (sz1 - j)*slicesize + slconst;
				for (i = 0; i<sx; i++)
					*(dataXB1++) = (*(MaskData + nj + i) == 1) ? 1 : 0;
			}
			il = sz*sx;
			break;
		case 2:
			for (j = 0; j<sz; j++)
			{
				nj = (sz1 - j)*slicesize + SliceNumber;
				for (i = 0; i<sy; i++)
					*(dataXB1++) = (*(MaskData + nj + (i)*nx) == 1) ? 1 : 0;
			}
			il = sz*sy;
			break;
		}


	}
}



//---------------------------------------------------------
void	View::PaintMainDataSetMask2Slice()
{
	if ((ShowMask2 == true)&&(Mask2Data !=NULL)) //lub opacity jest =0 - dopisz!
	{
		int i, j, il = 0;
		unsigned char *dataX, *dataXM;
		//(data->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 : dataX = (unsigned char *)Image->bits();
		dataX = (unsigned char *)Image->bits();
		dataXM = dataX;

		int slicesize = data->GetSliceSize();
		int nx = data->GetNx();
		int nj, slconst;
		int sz1 = sz - 1;
		int sy1 = sy - 1;

		//	char	*MaskTable = new char[rx*ry];
		unsigned char *maskTableX = maskTable + rx*ry;

		unsigned char	*dataXB1 = maskTableX;
		unsigned char	*dataXB2 = maskTableX;
		switch (view)
		{
		case 0:
			slconst = SliceNumber*slicesize;
			for (j = 0; j<sy; j++)
			{
				nj = slconst + nx*j;
				for (i = 0; i<sx; i++)
					*(dataXB1++) = (*(Mask2Data + nj + i) > 1) ? 1 : 0;
			}
			il = sx*sy;
			break;
		case 1:
			slconst = (sy - SliceNumber - 1)*nx;
			for (j = 0; j<sz; j++)
			{
				nj = (sz1 - j)*slicesize + slconst;
				for (i = 0; i<sx; i++)
					*(dataXB1++) = (*(Mask2Data + nj + i) > 1) ? 1 : 0;
			}
			il = sz*sx;
			break;
		case 2:
			for (j = 0; j<sz; j++)
			{
				nj = (sz1 - j)*slicesize + SliceNumber;
				for (i = 0; i<sy; i++)
					*(dataXB1++) = (*(Mask2Data + nj + (i)*nx) > 1) ? 1 : 0;
			}
			il = sz*sy;
			break;
		}

		//for (i = 0; i<il; i++)
		//{
		//	if (*dataXB2 == 1)
		//	{
		//		*(dataXM++) += (unsigned char)((((unsigned int)(*dataXB2*Mask2Color.blue() - *dataXM) << 8)*Mask2Opacity) >> 16);
		//		*(dataXM++) += (unsigned char)((((unsigned int)(*dataXB2*Mask2Color.green() - *dataXM) << 8)*Mask2Opacity) >> 16);
		//		*(dataXM++) += (unsigned char)((((unsigned int)(*dataXB2*Mask2Color.red() - *dataXM) << 8)*Mask2Opacity) >> 16);
		//	}
		//	else
		//		dataXM += 3;

		//	dataXM++; // skip alpha
		//	dataXB2++;
		//}
		//delete[]MaskTable;
	}
}

//---------------------------------------------------------
void	View::PaintRegisteredSlices(RawDataSet *RegisteredSlice, PaletteRGBA *PaletteReg, unsigned char Opacity)
{
	if ((RegisteredSlice != NULL) && (PaletteReg != NULL) && (Opacity>0))
		if ((RegisteredSlice->GetNx() == rx) && (RegisteredSlice->GetNy() == ry))
		{
			int i, j, il;
			RawDataSet *dataR;
			double ScalePaletteR;
			PaletteRGBA		*PalR;
			unsigned char *dataX, *dataXR, *dataXRT;
			dataR = RegisteredSlice;
			PalR = PaletteReg;
			ScalePaletteR = (double)(255.0) / (double)(dataR->GetLastHistValue());//-dataR->GetFirstHistValue());
																				  //	(data->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 : dataX = (unsigned char *)Image->bits();
																				  //	(data->GetMachineEndian() == 1) ? dataXR = (unsigned char *)ImageR->bits() + 1 : dataXR = (unsigned char *)ImageR->bits();
			dataX = (unsigned char *)Image->bits();
			dataXR = (unsigned char *)ImageR->bits();
			dataXRT = dataXR;

			switch (view)
			{
			case 0:	il = sx*sy; break;
			case 1:	il = sz*sx; break;
			case 2:	il = sz*sy; break;
			}

			for (j = 0; j<ry; j++)
				for (i = 0; i<rx; i++)
				{
					col = Round(dataR->GetVoxelValueFromZeroValue(i, j, 0)*ScalePaletteR);
					*(dataXR++) = PalR->B[col];
					*(dataXR++) = PalR->G[col];
					*(dataXR++) = PalR->R[col];
					dataXR++;
				}

			for (i = 0; i<il; i++)
			{
				*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*Opacity) >> 16);
				*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*Opacity) >> 16);
				*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*Opacity) >> 16);

				dataX++; // skip alpha
				dataXRT++;
			}
		}


}

//---------------------------------------------------------

void	View::PaintCorrespondingDataSetSlices()
{
	//	FILE *Do_opa;
	bool modeZeroTransparent = false;
	if (Views2DArray != NULL)
	{
		int i, j, il, item;

		RawDataSet *dataR;
		double ScalePaletteR;
		PaletteRGBA		*PalR;
		unsigned char Opacity;
		unsigned char *dataX, *dataXR, *dataXRT;

		for (item = 1; item <= NumberOf2DViews; item++)
		{
			if (Views2DArray[item] >= 0)
				if (DataSetList->GetItemAtPos(Views2DArray[item]) != NULL)
				{

					Opacity = DataSetList->GetItemAtPos(Views2DArray[item])->GetData()->GetOpacity();

					if (Opacity>0)
					{

						dataR = DataSetList->GetItemAtPos(Views2DArray[item])->GetData()->GetRawData();
						//	(data->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 : dataX = (unsigned char *)Image->bits();
						//	(data->GetMachineEndian() == 1) ? dataXR = (unsigned char *)ImageR->bits() + 1 : dataXR = (unsigned char *)ImageR->bits();
						dataX = (unsigned char *)Image->bits();
						dataXR = (unsigned char *)ImageR->bits();
						dataXRT = dataXR;

						if (dataR->GetRGBExists() == false)
						{
							PalR = DataSetList->GetItemAtPos(Views2DArray[item])->GetData()->GetPalette();
							ScalePaletteR = (double)(255.0) / (double)(dataR->GetLastHistValue());//-dataR->GetFirstHistValue());



							switch (view)
							{
							case 0:
								for (j = 0; j<sy; j++)
									for (i = 0; i<sx; i++)
									{
										col = Round(dataR->GetVoxelValueFromZeroValue(i, j, SliceNumber)*ScalePaletteR);
										*(dataXR++) = PalR->B[col];
										*(dataXR++) = PalR->G[col];
										*(dataXR++) = PalR->R[col];
										dataXR++;
									}
								il = sx*sy;
								break;

							case 1:
								for (j = 0; j<sz; j++)
									for (i = 0; i<sx; i++)
									{
										col = Round(dataR->GetVoxelValueFromZeroValue(i, sy - SliceNumber - 1, sz - j - 1)*ScalePaletteR);
										*(dataXR++) = PalR->B[col];
										*(dataXR++) = PalR->G[col];
										*(dataXR++) = PalR->R[col];
										dataXR++;
									}
								il = sz*sx;
								break;

							case 2:
								for (j = 0; j<sz; j++)
									for (i = 0; i<sy; i++)
									{
										col = Round(dataR->GetVoxelValueFromZeroValue(SliceNumber, i, sz - j - 1)*ScalePaletteR);
										*(dataXR++) = PalR->B[col];
										*(dataXR++) = PalR->G[col];
										*(dataXR++) = PalR->R[col];
										dataXR++;
									}
								il = sz*sy;
								break;
							}
						}
						else
						{
							int pos;
							//RGB-temp
							int Mode = dataR->GetMode();
							// ushort,ushort,ushort
							if (Mode == 3)
							{
								switch (view)
								{
								case 0:
									for (j = 0; j<sy; j++)
										for (i = 0; i<sx; i++)
										{
											pos = dataR->GetVoxelPos(i, j, SliceNumber);
											*(dataXR++) = (unsigned char)*(dataR->GetBChannelArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetGChannelArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetRChannelArray() + pos);
											dataXR++;
										}
									il = sx*sy;
									break;
								case 1:
									for (j = 0; j<sz; j++)
										for (i = 0; i<sx; i++)
										{
											pos = dataR->GetVoxelPos(i, sy - SliceNumber - 1, sz - j - 1);
											*(dataXR++) = (unsigned char)*(dataR->GetBChannelArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetGChannelArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetRChannelArray() + pos);
											dataXR++;
										}
									il = sz*sx;
									break;
								case 2:
									for (j = 0; j<sz; j++)
										for (i = 0; i<sy; i++)
										{
											pos = dataR->GetVoxelPos(SliceNumber, i, sz - j - 1);
											*(dataXR++) = (unsigned char)*(dataR->GetBChannelArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetGChannelArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetRChannelArray() + pos);
											dataXR++;
										}
									il = sz*sy;
									break;
								}
							}
							else if (Mode == 4)
							{
								//ushort,uchar,uchar
								switch (view)
								{
								case 0:

									for (j = 0; j<sy; j++)
										for (i = 0; i<sx; i++)
										{
											pos = dataR->GetVoxelPos(i, j, SliceNumber);
											*(dataXR++) = *(dataR->GetBChannelUCHARArray() + pos);
											*(dataXR++) = *(dataR->GetGChannelUCHARArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetRChannelArray() + pos);
											dataXR++;
										}
									il = sx*sy;
									break;
								case 1:
									for (j = 0; j<sz; j++)
										for (i = 0; i<sx; i++)
										{
											pos = dataR->GetVoxelPos(i, sy - SliceNumber - 1, sz - j - 1);
											*(dataXR++) = *(dataR->GetBChannelUCHARArray() + pos);
											*(dataXR++) = *(dataR->GetGChannelUCHARArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetRChannelArray() + pos);
											dataXR++;
										}
									il = sz*sx;
									break;
								case 2:
									for (j = 0; j<sz; j++)
										for (i = 0; i<sy; i++)
										{
											pos = dataR->GetVoxelPos(SliceNumber, i, sz - j - 1);
											*(dataXR++) = *(dataR->GetBChannelUCHARArray() + pos);
											*(dataXR++) = *(dataR->GetGChannelUCHARArray() + pos);
											*(dataXR++) = (unsigned char)*(dataR->GetRChannelArray() + pos);
											dataXR++;
										}
									il = sz*sy;
									break;
								}
							}
						}
						if (modeZeroTransparent == true)
						{
							unsigned char r, g, b;
							for (i = 0; i<il; i++)
							{
								r = *(dataXRT++);
								g = *(dataXRT++);
								b = *(dataXRT++);

								if (r + g + b == 0)
								{
									*(dataX++) += (unsigned char)((((unsigned int)(r - *dataX) << 8)*Opacity) >> 16);
									*(dataX++) += (unsigned char)((((unsigned int)(g - *dataX) << 8)*Opacity) >> 16);
									*(dataX++) += (unsigned char)((((unsigned int)(b - *dataX) << 8)*Opacity) >> 16);
								}
								else
								{
									*(dataX++) = r;
									*(dataX++) = g;
									*(dataX++) = b;
								}
								dataX++; // skip alpha
								dataXRT++;
							}
						}

						else
						{
							for (i = 0; i<il; i++)
							{
								*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*Opacity) >> 16);
								*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*Opacity) >> 16);
								*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*Opacity) >> 16);

								dataX++; // skip alpha
								dataXRT++;
							}
						}
					}
				}
		}
	}

}



//---------------------------------------------------------

void	View::PaintInspectionWindow()
{
	if (InspectionWindowsArray != NULL)
	{
		int i, j, item;
		unsigned char *dataX;
		RawDataSet *dataR;
		DataSet *TData;
		int X = 0, Y = 0, Z = 0;
		int IX = 0, IY = 0, IZ = 0;
		unsigned char Opacity;


		for (item = 1; item <= NumberOfInspectionWindows; item++)
		{
			if (InspectionWindowsArray[item] >= 0)
			{
				if (DataSetList->GetItemAtPos(InspectionWindowsArray[item]) != NULL)
				{
					TData = DataSetList->GetItemAtPos(InspectionWindowsArray[item])->GetData();
					PaletteRGBA *Palette = TData->GetPalette();
					if (TData->GetInspectionEnabled())
					{
						unsigned char *TempInspMask = NULL;
						switch (view)
						{
						case 0:
							X = TData->GetPosX();
							Y = TData->GetPosY();
							Z = TData->GetPosZ();
							IX = TData->GetInspX();
							IY = TData->GetInspY();
							IZ = TData->GetInspZ();
							TempInspMask = TData->GetInspectionArray0();
							break;


						case 1:
							X = TData->GetPosX();
							Y = TData->GetRawData()->GetNz() - TData->GetPosZ() - 1;
							Z = TData->GetRawData()->GetNy() - TData->GetPosY() - 1;
							IX = TData->GetInspX();
							IY = TData->GetInspZ();
							IZ = TData->GetInspY();
							TempInspMask = TData->GetInspectionArray1();
							break;

						case 2:
							//X=TData->GetRawData()->GetNy()-TData->GetPosY()-1;
							X = TData->GetPosY();
							Y = TData->GetRawData()->GetNz() - TData->GetPosZ() - 1;

							Z = TData->GetPosX();
							IX = TData->GetInspY();
							IY = TData->GetInspZ();
							IZ = TData->GetInspX();
							TempInspMask = TData->GetInspectionArray2();
							break;
						}

						//if((abs(Z-SliceNumber)<=IZ))
						{
							dataR = TData->GetRawData();
							double ScalePaletteR = (double)(255.0) / (double)(dataR->GetLastHistValue());//-dataR->GetFirstHistValue());
																										 //(data->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 : dataX = (unsigned char *)Image->bits();
							dataX = (unsigned char *)Image->bits();

							//	int IX,IY;
							int xxr0, xxr1, yyr0, yyr1;
							xxr0 = ((X - IX)>0) ? X - IX : 0;
							yyr0 = ((Y - IY)>0) ? Y - IY : 0;
							xxr1 = ((X + IX)<rx) ? X + IX : rx - 1;
							yyr1 = ((Y + IY)<ry) ? Y + IY : ry - 1;
							//InspectionMask

							int XSL, YST, XSR;

							XSL = ((X - IX) >= 0) ? 0 : IX - X;
							YST = ((Y - IY) >= 0) ? 0 : IY - Y;
							XSR = ((X + IX)<rx) ? 0 : IX - (rx - 1 - X);
							int Step = XSL + XSR;
							//unsigned char *TempInspMask=InspectionMask;
							TempInspMask += (IX * 2 + 1)*YST + XSL; //maske zaczne czytac dopiero od odpow rzadku
							switch (view)
							{
							case 0:
								dataX += 4 * (yyr0*rx);

								for (j = yyr0; j <= yyr1; j++)
								{
									dataX += 4 * xxr0;
									for (i = xxr0; i <= xxr1; i++)
									{
										Opacity = *(TempInspMask++);
										if (Opacity != 0)
										{
											col = Round(dataR->GetVoxelValueFromZeroValue(i, j, SliceNumber)*ScalePaletteR);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->B[col] - *dataX) << 8)*Opacity) >> 16);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->G[col] - *dataX) << 8)*Opacity) >> 16);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->R[col] - *dataX) << 8)*Opacity) >> 16);
											dataX++;
										}
										else
										{
											dataX += 4;
										}
									}
									dataX += 4 * (rx - xxr1 - 1);
									TempInspMask += Step;
								}
								break;
							case 1:
								dataX += 4 * (yyr0*rx);
								for (j = yyr0; j <= yyr1; j++)
								{
									dataX += 4 * xxr0;
									for (i = xxr0; i <= xxr1; i++)
									{
										Opacity = *(TempInspMask++);
										if (Opacity != 0)
										{
											col = Round(dataR->GetVoxelValueFromZeroValue(i, sy - SliceNumber - 1, sz - j - 1)*ScalePaletteR);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->B[col] - *dataX) << 8)*Opacity) >> 16);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->G[col] - *dataX) << 8)*Opacity) >> 16);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->R[col] - *dataX) << 8)*Opacity) >> 16);
											dataX++;
										}
										else
										{
											dataX += 4;
										}
									}
									dataX += 4 * (rx - xxr1 - 1);
									TempInspMask += Step;
								}
								break;

							case 2:
								dataX += 4 * (yyr0*rx);
								for (j = yyr0; j <= yyr1; j++)
								{
									dataX += 4 * xxr0;
									for (i = xxr0; i <= xxr1; i++)
									{
										Opacity = *(TempInspMask++);
										if (Opacity != 0)
										{
											col = Round(dataR->GetVoxelValueFromZeroValue(SliceNumber, i, sz - j - 1)*ScalePaletteR);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->B[col] - *dataX) << 8)*Opacity) >> 16);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->G[col] - *dataX) << 8)*Opacity) >> 16);
											*(dataX++) += (unsigned char)((((unsigned int)(Palette->R[col] - *dataX) << 8)*Opacity) >> 16);
											dataX++;
										}
										else
										{
											dataX += 4;
										}
									}
									dataX += 4 * (rx - xxr1 - 1);
									TempInspMask += Step;
								}
								break;
							}
						}
					}
				}
			}

		}
	}

}
//---------------------------------------------------------
void	View::SlotInsertNewInspection(int InspectionPos)
{
	//InspectionDataSet=DataSetList->GetItemAtPos(InspectionPos)->GetData();
	IWidget->SetInspectionWindowSizes(InspX, InspY, InspZ);
}
//---------------------------------------------------------
void	View::SlotInsertNewInspectionWindows(int *InspW)
{
	int i;
	if (InspectionWindowsArray != NULL) delete[]InspectionWindowsArray;
	NumberOfInspectionWindows = InspW[0];
	InspectionWindowsArray = new int[NumberOfInspectionWindows + 1];
	for (i = 0; i <= NumberOfInspectionWindows; i++)
		InspectionWindowsArray[i] = InspW[i];

	updateSlice();
	repaint2();

}
//---------------------------------------------------------
void	View::SlotInsertNew2DViews(int *Views2D)
{
	int i;
	if (Views2DArray != NULL) delete[]Views2DArray;
	NumberOf2DViews = Views2D[0];
	Views2DArray = new int[NumberOf2DViews + 1];
	for (i = 0; i <= NumberOf2DViews; i++)
		Views2DArray[i] = Views2D[i];
	updateSlice();

}
//---------------------------------------------------------
void	View::SlotConnectSendCoordsAndUpdate()
{
	connect(this, SIGNAL(SendCoordinatesWithButton(int, int, int, int)), this, SLOT(SlotUpdateCrossPointer(int, int, int, int)));
}
//---------------------------------------------------------
void	View::SlotDisconnectSendCoordsAndUpdate()
{
	disconnect(this, SIGNAL(SendCoordinatesWithButton(int, int, int, int)), this, SLOT(SlotUpdateCrossPointer(int, int, int, int)));
}
//---------------------------------------------------------
void	View::SlotUpdateCrossPointer(int x, int y, int z, int Button)
{
	//QString aa="SlotUpdateCrossPointer"+QString(" ")+name();
	//qWarning(aa);
	//QMessageBox::about( 0,"nr",QString::number(x)+" "+QString::number(y)+" Boing");
	if ((Button >= 3) && (Button <= 5) && (ButtonMode == 1))
	{
		if (Button == 3)
		{
			int tmp = CrossType;
			CrossType = 0;
			SetSlice(x, y, z);
			CrossType = tmp;
		}
	}
	else
	{
		SlotSetCrossType(CrossType);
		SetSlice(x, y, z);

	}
}
//SendCoordinatesWithButton(X,Y,Z,Button);
//---------------------------------------------------------
void	View::SlotSetButtonMode(int Mode)
{
	ButtonMode = Mode;

	if (ButtonMode == 0)
	{
		IWidget->SetMaskPaintMode(0);
		IWidget->SetGetMaskMode(false);
	}
	else //if (ButtonMode>0)
	{
		IWidget->SetMaskPaintMode(Mode);
		IWidget->SetGetMaskMode(true);
	}
	/*else if ((ButtonMode==2)||(ButtonMode==3))
	{
	IWidget->SetMaskPaintMode(Mode);
	IWidget->SetGetMaskMode(false);
	}*/
}
//---------------------------------------------------------
void	View::SlotSendViewID()
{
	emit SendViewId(view, SliceNumber);

	emit signalSendViewUserID(viewID);
}
//----------------------------------------------------------
void	View::SlotBlockRepaint(bool block)
{
	if (IWidget != NULL) IWidget->SlotBlockRepaint(block);
}
//--------------------------------------------
void View::SmoothFilterImageTab(unsigned char *image, int W, int H)
{
	//GAUSS 5x5
	int Size = 2;
	int MASK[25] = { 2, 4, 5, 4, 2, 4, 9, 12, 9, 4, 5, 12, 15, 12, 5, 4, 9, 12, 9, 4, 2, 4, 5, 4, 2 };
	double dzielnik = 1.0 / 159.0;

	int SizeReal = Size * 2 + 1;
	int Length = SizeReal*SizeReal;
	unsigned char *Tab = new unsigned char[Length];
	unsigned char *tmp = Padding2D_Unsigned_Char(image, W, H, Size, 0);

	int Th = H + Size * 2;
	int Tw = W + Size * 2;

	// tu trzeba obrazek przefiltrowac...
	int i, j, k;
	int value;
	int pos = 0;

	for (j = 0; j<H; j++)
		for (i = 0; i<W; i++)
		{
			//pobrac otoczenie
			GetIntensityTab(tmp, Tw, Th, 3, 0, Size, Tab, (j + Size)*Tw + i + Size);
			//i przefiltrowac
			value = 0;
			for (k = 0; k<Length; k++)
				value += MASK[k] * Tab[k];
			if (value<0) value = 0;
			image[pos++] = (int)(value*dzielnik + 0.5);
		}
	delete[]Tab;
	delete[]tmp;

}
//-------------------------------------
void	View::SlotMoveMask(int direction)
{
	emit SignalMoveMask(crossection, direction);
}
//---------------------------------------
void	View::SlotSwitchSendPointArray(int  a)
{
	sendPolyLineMode = a;

	if (a != 0)
		QObject::connect(IWidget, SIGNAL(SignalSendPointArray(QPolygon, double, double)), this, SLOT(SlotSendPointArray(QPolygon, double, double)));
	else
		QObject::disconnect(IWidget, SIGNAL(SignalSendPointArray(QPolygon, double, double)), this, SLOT(SlotSendPointArray(QPolygon, double, double)));
	IWidget->SlotSetSendMaskMode(a);
	//SignalSendPointArray
}
//------------------------------------
void	View::SlotSendPointArray(QPolygon a, double x, double y)
{

	emit SignalSendPointArray(a, x, y, view, sendPolyLineMode);
}
//-----------------------------------
void	View::SlotSwitchAutoGrab(QString filename)
{
	autoGrab = !autoGrab;
	pictureIndex = 0;
	pictureName = filename;
}
//--------------
void	View::SlotSendShape(QPolygonF *polygon)
{
#if defined (VTK)
	pts->Reset();
	int nr = polygon->count();
	int i;
	double origin[3];
	double point[3];
	double p[2];
	double scale[3];
	data->GetScale(scale);
	data->GetOrigin(origin);
	pts->SetNumberOfPoints(nr);


	switch (view)
	{

	case 0:

		for (i = 0; i<nr; i++)
		{
			p[0] = polygon->at(i).x();
			p[1] = polygon->at(i).y();

			point[0] = origin[0] + p[0];
			point[1] = origin[1] + p[1];
			point[2] = origin[2] + SliceNumber*scale[2];
			pts->SetPoint(i, point);

		}
		/*	rx=sx;ry=sy;rz=sz;
		dx=data->GetXscale();dy=data->GetYscale();
		IWidget->SetName("Transversal");*/

		break;

	case 1:

		for (i = 0; i<nr; i++)
		{
			p[0] = polygon->at(i).x();
			p[1] = polygon->at(i).y();

			point[0] = origin[0] + p[0];
			point[1] = origin[1] + (sy - SliceNumber - 1)*scale[1];
			point[2] = origin[2] + (sz - 1)*scale[2] - p[1];
			pts->SetPoint(i, point);

		}
		/*	rx=sx;ry=sz;rz=sy;
		dx=data->GetXscale();dy=data->GetZscale();
		IWidget->SetName("Coronal");*/

		break;

	case 2:
		for (i = 0; i<nr; i++)
		{
			p[0] = polygon->at(i).x();
			p[1] = polygon->at(i).y();

			point[0] = origin[0] + SliceNumber*scale[0];
			point[1] = origin[1] + p[0];
			point[2] = origin[2] + (sz - 1)*scale[2] - p[1];
			pts->SetPoint(i, point);

		}
		/*	rx=sy;ry=sz;rz=sx;
		dx=data->GetYscale();dy=data->GetZscale();
		IWidget->SetName("Sagittal");*/

		break;
	}
	emit SignalSendVtkPoints(pts);
#endif
}
void	View::SlotGrabImage()
{
	QString name;
	pictureIndex++;
	name = pictureName;
	if (pictureIndex<1000000) name += "0";
	if (pictureIndex<100000) name += "0";
	if (pictureIndex<10000) name += "0";
	if (pictureIndex<1000) name += "0";
	if (pictureIndex<100) name += "0";
	if (pictureIndex<10) name += "0";
	name += QString::number(pictureIndex++);
	Image->save(name + ".png", "PNG");

}
void View::SlotChangeThreshMode(bool mode)
{
	this->threshMode = mode;
	//zapamietaj palete!!!
	tmpLth = LTresh;
	tmpUth = UTresh;
	for (int i = 0;i < LastHistValue + 1;i++)
	{
		TempPaletteArrayForMaskView[i] = TempPaletteArray[i];
	}


	updateSlice();
}
//---------
void	View::SetRawDataMixer(RawDataMixerLite *m)
{
	mixer = m;
}

//-------------------------------------
void	View::SlotSetSpecialSliceVisibility(int v)
{
	paintSpecialSliceEnabled = v;

	SlotSetOpacity();

}
//---------------------
void	View::SlotSetSpecialSliceMode(int mode)
{

	if (mode != specialSliceMode)
	{
		specialSliceMode = mode;
		CreateSpecialSlice();
	}


	SlotSetOpacity();
}
//---------------------------------
void	View::SlotSetSpecialSliceOpacity(int opacity)
{
	specialSliceOpacity = opacity;
	SlotSetOpacity();
}
//--------------------------------------------
void	View::CreateSpecialSlice()
{

	if ((paintSpecialSliceEnabled == true) && (specialSliceOpacity > 0))
	{
		if (specialSliceData != NULL) delete[]specialSliceData;

		int NX, NY, NZ;


		switch (view)
		{

		case 0:
			NX = sx;	NY = sy; NZ = sz;
			GetSliceValue = &View::GetSliceValue0;
			break;

		case 1:
			NX = sx;	NY = sz;	NZ = sy;
			GetSliceValue = &View::GetSliceValue1;
			break;

		case 2:
			NX = sy;	NY = sz;	NZ = sx;
			GetSliceValue = &View::GetSliceValue2;
			break;
		}

		switch (specialSliceMode)
		{
		case 0:
			GetProjectionValue = &View::GetAvgProjection;
			break;

		case 1:
			GetProjectionValue = &View::GetMinProjection;
			break;

		case 2:
			GetProjectionValue = &View::GetMaxProjection;
			break;

		case 3:
			GetProjectionValue = &View::GetMedProjection;
			break;
		case 4:
			GetProjectionValue = &View::GetTest1dProjection;
			break;
		case 5:
			GetProjectionValue = &View::GetTest2dProjection;
			break;
		case 6:
			GetProjectionValue = &View::GetFirstHitProjection;
			break;	
		case 7:
				GetProjectionValue = &View::GetLastHitProjection;
				break;
		

		default:
			GetProjectionValue = &View::GetMaxProjection;

		}





		int i, j, k;







		unsigned short *tab = new unsigned short[NZ];
		int sliceSize = sx*sy;

		specialSliceData = new unsigned short[NX*NY];
		bool v;
		int pos = 0;
		//int avg;


		if (numberOfThreads < 2)
		{
			for (j = 0; j < NY; j++)
			{
				for (i = 0; i < NX; i++)
				{

					for (k = 0; k < NZ; k++)
					{
						tab[k] = (this->*GetSliceValue)(data->GetDataArray(), i, j, k, sx, sy, sz, sliceSize, NULL);
						//avg+=tab[k];
					}

					specialSliceData[pos++] = (this->*GetProjectionValue)(tab, NZ);//(double)NZ;

				}
			}
		}
		else
		{
			unsigned short **tabz;

#pragma omp parallel private (tabz) 
			{
				unsigned short *datatab = data->GetDataArray();
				tabz = new unsigned short *[numberOfThreads];
#pragma omp critical
				{
					for (i = 0; i < numberOfThreads; i++)
					{
						tabz[i] = new unsigned short[NZ];
					}
				}
				int tid = omp_get_thread_num();
#pragma omp  for nowait ordered schedule (guided) 

				for (int l = 0; l < NX*NY; l++)
				{
					int xx, yy;
					yy = l / NX;
					xx = l - yy*NX;
					//int thNr= omp_get_thread_num();
					GetSpecialRay(tabz[tid], NZ, datatab, xx, yy, sx, sy, sz, sliceSize, NULL);
					specialSliceData[l] = (this->*GetProjectionValue)(tabz[tid], NZ);

				}


#pragma omp critical
				{
					for (i = 0; i < numberOfThreads; i++)
					{
						delete[]tabz[i]; tabz[i] = NULL;
					}
					delete[]tabz;
				}
			}
		}
		specialSliceMaxValue = 0;
		for (i = 0; i < NX*NY; i++)
		{
			if (specialSliceData[i] > specialSliceMaxValue) specialSliceMaxValue = specialSliceData[i];
		}
		if (specialSliceMaxValue == 0) specialSliceMaxValue = 1;
		delete[]tab;
	}


}
void	View::GetSpecialRay(unsigned short *ray, int rayLen, unsigned short *tab, int x, int y, int nx, int ny, int nz, int ss, bool *validate)
{
	for (int i = 0; i<rayLen; i++)
	{
		ray[i] = (this->*GetSliceValue)(tab, x, y, i, nx, ny, nz, ss, validate);
	}
}
//-------------------------
void	View::PaintSpecialSlice()
{
	//	FILE *Do_opa;
	bool modeZeroTransparent = false;


	if ((paintSpecialSliceEnabled == true) && (specialSliceOpacity>0))
	{

		if (specialSliceData == NULL)	CreateSpecialSlice();
		int ppos = 0;
		int i, j, il, item;

		RawDataSet *dataR;
		double ScalePaletteR;
		PaletteRGBA		*PalR;
		unsigned char Opacity;
		unsigned char *dataX, *dataXR, *dataXRT;


		dataR = data;
		//(data->GetMachineEndian() == 1) ? dataX = (unsigned char *)Image->bits() + 1 :
		dataX = (unsigned char *)Image->bits();
		//(data->GetMachineEndian() == 1) ? dataXR = (unsigned char *)ImageR->bits() + 1 :
		dataXR = (unsigned char *)ImageR->bits();
		dataXRT = dataXR;


		PalR = this->MainDataSet->GetPalette();
		ScalePaletteR = (double)(255.0) / (double)(specialSliceMaxValue);//-dataR->GetFirstHistValue());
		int cc;

		int pos = 0;

		int LowerThreshold = LTresh;
		int UpperThreshold = UTresh;

		if (UpperThreshold - LowerThreshold == 1) LowerThreshold = UpperThreshold;
		unsigned char r, g, b;

		if ((threshMode == 1) && ((LTresh>FirstHistValue) || ((UTresh<LastHistValue))))
		{

			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.blue() - *dataXM)<<8)*MaskOpacity)>>16);
			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.green() - *dataXM)<<8)*MaskOpacity)>>16);
			//	*(dataXM++)+=(unsigned char)((((unsigned int)(*dataXB2*MaskColorX.red() - *dataXM)<<8)*MaskOpacity)>>16);
			int opacity = thresholdMaskOpacity;
			unsigned char maskVal;

			switch (view)
			{


			case 0:
				for (j = 0; j<sy; j++)
					for (i = 0; i<sx; i++)
					{
						//cc=dataT->GetVoxelValue(i,j,SliceNumber);//-FirstHistValue;
						cc = specialSliceData[ppos++];
						//col=((cc<LowerThreshold)||(cc>UpperThreshold))?0:TempPaletteArray[cc];
						//col=TempPaletteArray[cc];
						col = TempPaletteArray[cc];
						maskVal = ((cc<LowerThreshold) || (cc>UpperThreshold)) ? 0 : 1;
						r = PalTemp->R[col];		g = PalTemp->G[col];		b = PalTemp->B[col];
						if (maskVal == 1)
						{
							*(dataXR++) = b + (unsigned char)((((unsigned int)(maskVal*MaskColorX.blue() - b) << 8)*opacity) >> 16);//PalTemp->B[col];
							*(dataXR++) = g + (unsigned char)((((unsigned int)(maskVal*MaskColorX.green() - g) << 8)*opacity) >> 16);//PalTemp->G[col];
							*(dataXR++) = r + (unsigned char)((((unsigned int)(maskVal*MaskColorX.red() - r) << 8)*opacity) >> 16);//PalTemp->R[col];
							dataXR++;
						}
						else
						{
							*(dataXR++) = b;
							*(dataXR++) = g;
							*(dataXR++) = r;
							dataXR++;
						}
					}
				break;
			case 1:
				for (j = 0; j<sz; j++)
					for (i = 0; i<sx; i++)
					{
						//cc=dataT->GetVoxelValue(i,sy-SliceNumber-1,sz-j-1);//-FirstHistValue;
						cc = specialSliceData[ppos++];
						col = TempPaletteArray[cc];
						maskVal = ((cc<LowerThreshold) || (cc>UpperThreshold)) ? 0 : 1;
						r = PalTemp->R[col];		g = PalTemp->G[col];		b = PalTemp->B[col];
						if (maskVal == 1)
						{
							*(dataXR++) = b + (unsigned char)((((unsigned int)(maskVal*MaskColorX.blue() - b) << 8)*opacity) >> 16);//PalTemp->B[col];
							*(dataXR++) = g + (unsigned char)((((unsigned int)(maskVal*MaskColorX.green() - g) << 8)*opacity) >> 16);//PalTemp->G[col];
							*(dataXR++) = r + (unsigned char)((((unsigned int)(maskVal*MaskColorX.red() - r) << 8)*opacity) >> 16);//PalTemp->R[col];
							dataXR++;
						}
						else
						{
							*(dataXR++) = b;
							*(dataXR++) = g;
							*(dataXR++) = r;
							dataXR++;
						}
					}
				break;
			case 2:
				for (j = 0; j<sz; j++)
					for (i = 0; i<sy; i++)
					{
						//cc=dataT->GetVoxelValue(SliceNumber,i,sz-j-1);//-FirstHistValue;
						cc = specialSliceData[ppos++];
						col = TempPaletteArray[cc];
						maskVal = ((cc<LowerThreshold) || (cc>UpperThreshold)) ? 0 : 1;
						r = PalTemp->R[col];		g = PalTemp->G[col];		b = PalTemp->B[col];
						if (maskVal == 1)
						{
							*(dataXR++) = b + (unsigned char)((((unsigned int)(maskVal*MaskColorX.blue() - b) << 8)*opacity) >> 16);//PalTemp->B[col];
							*(dataXR++) = g + (unsigned char)((((unsigned int)(maskVal*MaskColorX.green() - g) << 8)*opacity) >> 16);//PalTemp->G[col];
							*(dataXR++) = r + (unsigned char)((((unsigned int)(maskVal*MaskColorX.red() - r) << 8)*opacity) >> 16);//PalTemp->R[col];
							dataXR++;
						}
						else
						{
							*(dataXR++) = b;
							*(dataXR++) = g;
							*(dataXR++) = r;
							dataXR++;
						}
					}
				break;
			}
		}
		else

		{
			switch (view)
			{
			case 0:
				for (j = 0; j < sy; j++)
					for (i = 0; i < sx; i++)
					{
						//col = Round(specialSliceData[pos++] * ScalePaletteR);
						cc = specialSliceData[ppos++];
						col = ((cc < LowerThreshold) || (cc > UpperThreshold)) ? ((cc < LowerThreshold) ? 0 : 255) : TempPaletteArray[cc];
						*(dataXR++) = PalR->B[col];
						*(dataXR++) = PalR->G[col];
						*(dataXR++) = PalR->R[col];
						dataXR++;
					}
				il = sx*sy;
				break;

			case 1:
				for (j = 0; j < sz; j++)
					for (i = 0; i < sx; i++)
					{
						//col = Round(specialSliceData[pos++] * ScalePaletteR);
						cc = specialSliceData[ppos++];
						col = ((cc < LowerThreshold) || (cc > UpperThreshold)) ? ((cc < LowerThreshold) ? 0 : 255) : TempPaletteArray[cc];
						*(dataXR++) = PalR->B[col];
						*(dataXR++) = PalR->G[col];
						*(dataXR++) = PalR->R[col];
						dataXR++;
					}
				il = sz*sx;
				break;

			case 2:
				for (j = 0; j < sz; j++)
					for (i = 0; i < sy; i++)
					{
						//col = Round(specialSliceData[pos++] * ScalePaletteR);
						cc = specialSliceData[ppos++];
						col = ((cc < LowerThreshold) || (cc > UpperThreshold)) ? ((cc < LowerThreshold) ? 0 : 255) : TempPaletteArray[cc];
						*(dataXR++) = PalR->B[col];
						*(dataXR++) = PalR->G[col];
						*(dataXR++) = PalR->R[col];
						dataXR++;
					}
				il = sz*sy;
				break;
			}
		}




		for (i = 0; i<il; i++)
		{
			*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*specialSliceOpacity) >> 16);
			*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*specialSliceOpacity) >> 16);
			*(dataX++) += (unsigned char)((((unsigned int)(*(dataXRT++) - *dataX) << 8)*specialSliceOpacity) >> 16);

			dataX++; // skip alpha
			dataXRT++;
		}




	}


}

//---------------------------------------
unsigned short View::GetSliceValue0(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//	data->GetVoxelValue(x,y,sliceNumber);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((x<0) || (x>nx - 1) || (y<0) || (y>ny - 1) || (sliceNumber<0) || (sliceNumber>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[sliceNumber*ss + y*nx + x];

}
//---------------------------------------
unsigned short View::GetSliceValue1(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//cc=dataT->GetVoxelValue(x,sy-SliceNumber-1,sz-y-1);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((x<0) || (x>nx - 1) || ((ny - sliceNumber - 1)<0) || ((ny - sliceNumber - 1)>ny - 1) || ((nz - y - 1)<0) || ((nz - y - 1)>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[(nz - y - 1)*ss + (ny - sliceNumber - 1)*nx + x];
}
//---------------------------------------
unsigned short View::GetSliceValue2(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//	cc=dataT->GetVoxelValue(SliceNumber,x,sz-y-1);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((sliceNumber<0) || (sliceNumber>nx - 1) || (x<0) || (x>ny - 1) || ((nz - y - 1)<0) || ((nz - y - 1)>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[(nz - y - 1)*ss + x*nx + sliceNumber];
}
//---------------------------------------

unsigned short View::GetFirstHitProjection(unsigned short* tab, int size)
{
	//specialParam1

	if (size <= 0) return 0;
	int max = data->GetMaxValue();
	//if (max <= 0) max = 0;
	int th = (specialParam1 * 0.0001) * max;

	int val = -1;
	int i;
	int avg = 0;
	for (i = 0; i < size; i++)
	{
		if (tab[i] >= th)
		{
			val = tab[i];
			break;
		}
	}
	if (val < 0)
		val = 0;//tab[0];
	return val;
}
//--------------------------
unsigned short View::GetLastHitProjection(unsigned short* tab, int size)
{

	if (size <= 0) return 0;
	int max = data->GetMaxValue();
	//if (max <= 0) max = 0;
	int th = (specialParam1 * 0.0001) * max;

	int val = -1;
	int i;
	int avg = 0; 
	for (i = size-1; i >= 0; i--)
	{
		if (tab[i] >= th)
		{
			val = tab[i];
			break;
		}
	}
	if (val < 0)
		val = 0;//tab[size - 1];
	return val;
}
//----------------------------------
unsigned short View::GetAvgProjection(unsigned short *tab, int size)
{
	if (size <= 0) return 0;

	int i;
	int avg = 0;
	for (i = 0; i<size; i++)
	{
		avg += tab[i];
	}

	return avg / (1.0*size);
}
//-----------------
unsigned short View::GetMinProjection(unsigned short *tab, int size)
{
	if (size <= 0) return 0;

	int i;
	int min = tab[0];
	for (i = 1; i<size; i++)
	{
		if (tab[i]<min) min = tab[i];
	}

	return min;
}
//-----------------
unsigned short View::GetMaxProjection(unsigned short *tab, int size)
{
	if (size <= 0) return 0;

	int i;
	int max = tab[0];
	for (i = 1; i<size; i++)
	{
		if (tab[i]>max) max = tab[i];
	}

	return max;
}
//-----------------
unsigned short View::GetMedProjection(unsigned short *tab, int size)
{
	QSortUnsignedShort(tab, 0, size - 1);
	return tab[size / 2];
}
//-----------------

//-----------------
unsigned short View::GetTest1dProjection(unsigned short *tab, int size)
{
	double mean = 0;
	double mean2 = 0;

	double min = INT_MAX;
	double max = -INT_MAX;
	double stdDev;
	for (int i = 0; i < size; i++)
	{

		if (tab[i] < min) min = tab[i];
		if (tab[i]>max)max = tab[i];
		mean += tab[i];
		mean2 += tab[i] * tab[i];

	}

	mean /= size;
	mean2 /= size;

	stdDev = sqrt(mean2 - mean*mean);


	double scalex = (1 + specialParam1) / 1000.0;

	return stdDev*scalex;
}

//-----------------
unsigned short View::GetTest2dProjection(unsigned short *tab, int size)
{
	double mean = 0;
	double mean2 = 0;

	double min = INT_MAX;
	double max = -INT_MAX;
	double stdDev;
	for (int i = 0; i < size; i++)
	{

		if (tab[i] < min) min = tab[i];
		if (tab[i]>max)max = tab[i];
		mean += tab[i];
		mean2 += tab[i] * tab[i];

	}

	mean /= size;
	mean2 /= size;

	stdDev = sqrt(mean2 - mean*mean);


	double scalex = (1 + specialParam1) / 1000.0;



	//double scalex2 = (specialParam2 - 5000)*0.001;
	double result = MAX(max, stdDev*scalex);
	return result;
}

//-----------------

//-----------------
void View::GetTransHistTab(unsigned short *d, int size)
{
	if ((histTab != NULL) && (d != NULL))
	{
		int histSize = histTabSize;
		memset(histTab, 0, histSize * sizeof(int));


		for (int i = 0; i<size; i++)
		{
			int val = (int)(d[i] * scaleHistFactor);
			if (val<histSize)		histTab[val]++;
		}

		emit signalSendCurrentHistogram(data, histTab, histSize);
	}



}
//-------------
void	View::SlotSetThreshColorMode(int mode)
{
	threshColorMode = mode;
	updateSlice();
}
//-------------
void View::SlotRedrawWidget()
{
	//QCoreApplication::processEvents();
	updateSlice();
}
//-------------
void View::SetBackgroundColor(QColor col, bool hideBoxSLider)
{
	QPalette Pal(palette());
	// set black background
	Pal.setColor(QPalette::Background, col);
	this->setAutoFillBackground(true);
	this->setPalette(Pal);
	this->show();

	Box->setVisible(!hideBoxSLider);
	Slider->setVisible(!hideBoxSLider);
	IWidget->SlotSetBackgroundColor(col);
}

void View::SlotCopyMask()
{

	//maskTable1
	if (maskTable1 != NULL) //lub opacity jest =0 - dopisz!
	{
		SendMessage(IWidget->GetName() + " - Mask Copied");
		int i, j, il = 0;

		int slicesize = data->GetSliceSize();
		int nx = data->GetNx();
		int nj, slconst;
		int sz1 = sz - 1;
		int sy1 = sy - 1;

		//	char	*MaskTable = new char[rx*ry];
		unsigned char	*dataXB1 = maskTable1;

		switch (view)
		{
		case 0:
			slconst = SliceNumber*slicesize;
			for (j = 0; j < sy; j++)
			{
				nj = slconst + nx*j;
				for (i = 0; i < sx; i++)
					*(dataXB1++) = (*(MaskData + nj + i) == 1) ? 1 : 0;
			}
			il = sx*sy;
			break;
		case 1:
			slconst = (sy - SliceNumber - 1)*nx;
			for (j = 0; j < sz; j++)
			{
				nj = (sz1 - j)*slicesize + slconst;
				for (i = 0; i < sx; i++)
					*(dataXB1++) = (*(MaskData + nj + i) == 1) ? 1 : 0;
			}
			il = sz*sx;
			break;
		case 2:
			for (j = 0; j < sz; j++)
			{
				nj = (sz1 - j)*slicesize + SliceNumber;
				for (i = 0; i < sy; i++)
					*(dataXB1++) = (*(MaskData + nj + (i)*nx) == 1) ? 1 : 0;
			}
			il = sz*sy;
			break;




		}

	}


}
////--------------------
void View::SlotPasteMask()
{
	if (maskTable1 != NULL) //lub opacity jest =0 - dopisz!
	{
		SendMessage(IWidget->GetName() + " - Mask Pasted");
		emit signalSend2DMask(maskTable1);
	}


}
//------------------
//------------------------------
void	View::SlotSliderFocus(int val)
{
	if (val != 0)
	{
		Slider->setValue(Slider->value() + val);
	}
	//	Slider->setFocus();
}

void	View::SetCropPlanes(int  *bounds)
{

	//int bounds2d[4];



	if (bounds != NULL)
	{

		for (int i = 0; i < 6; i++)
		{
			bounds3d[i] = bounds[i];
		}

		switch (view)
		{
		case 0:

			bounds2d[0] = bounds[0];//xmin
			bounds2d[1] = bounds[1];//xmax
			bounds2d[2] = bounds[2];//ymin
			bounds2d[3] = bounds[3];//ymax

									//X = x;
									//Y = y;
									//Z = SliceNumber;


			break;

		case 1:

			bounds2d[0] = bounds[0];//xmin
			bounds2d[1] = bounds[1];//xmax
			bounds2d[2] = sz - bounds[5];//zmin
			bounds2d[3] = sz - bounds[4];//zmax


										 //X = x;
										 //Y = sy - SliceNumber - 1;
										 //Z = sz - y - 1;



			break;

		case 2:

			bounds2d[0] = bounds[2];//ymin
			bounds2d[1] = bounds[3];//ymax
			bounds2d[2] = sz - bounds[5];//zmin
			bounds2d[3] = sz - bounds[4];//zmax
										 //X = SliceNumber;
										 //Y = x;
										 //Z = sz - y - 1;
			break;

		}

		cout << view << ": ";
		for (int i = 0; i < 4; i++)
		{
			cout << bounds2d[i] << " ";
		}
		cout << std::endl;

		IWidget->SetCropLines(bounds2d);
		repaint2();
	}

}

void	View::SetCropPlanesVisibility(bool sel)
{
	if (IWidget != NULL)
	{
		IWidget->SlotShowCropLines(sel);
		repaint2();
	}
}
//-----------------------------------------------------------------

void	View::SlotUpdateCropPlanes(int *_bounds2d)
{

	bounds2d[0] = _bounds2d[0];
	bounds2d[1] = _bounds2d[1];
	bounds2d[2] = _bounds2d[2];
	bounds2d[3] = _bounds2d[3];

	int *bounds = bounds3d;

	switch (view)
	{
	case 0:

		bounds[0] = bounds2d[0]; //xmin
		bounds[1] = bounds2d[1]; //xmax
		bounds[2] = bounds2d[2]; //ymin
		bounds[3] = bounds2d[3]; //ymax

								 //X = x;
								 //Y = y;
								 //Z = SliceNumber;


		break;

	case 1:

		bounds[0] = bounds2d[0];//xmin
		bounds[1] = bounds2d[1];//xmax
		bounds[5] = sz - bounds2d[2];//zmin
		bounds[4] = sz - bounds2d[3];//zmax


									 //X = x;
									 //Y = sy - SliceNumber - 1;
									 //Z = sz - y - 1;



		break;

	case 2:

		bounds[2] = bounds2d[0];//ymin
		bounds[3] = bounds2d[1];//ymax
		bounds[5] = sz - bounds2d[2];//zmin
		bounds[4] = sz - bounds2d[3];//zmax
									 //X = SliceNumber;
									 //Y = x;
									 //Z = sz - y - 1;
		break;

	}
	//qDebug() << "sz " << sz << " bounds2d[2] " << bounds2d[2] << " bounds2d[3] " << bounds2d[3];
	qDebug() << "2d " " " << bounds2d[0] << " " << bounds2d[1] << " " << bounds2d[2] << " " << bounds2d[3];;

	qDebug() << "view " << view << " 3d: " << bounds[0] << " " << bounds[1] << " " << bounds[2] << " " << bounds[3] << " " << bounds[4] << " " << bounds[5];
	//qDebug() << "";
	emit signalNewBounds3D(bounds3d);
}
//---------------------------
void View::wheelEvent(QWheelEvent *event)
{
	if ((zoomMode == false) && (wheelOn == true))
	{
		QPoint numDegrees = event->angleDelta();

		int v = (numDegrees.ry()) > 0 ? 1 : -1;
		Slider->setValue(Slider->value() + v);
		emit SendMessage(IWidget->GetName() + " " + QString::number(numDegrees.rx()) + " - " + QString::number(numDegrees.ry()));
	}

}
//----------------------------------

//------------------------
void View::repaint2()
{
	IWidget->repaint();
	//this->repaint();
}

//------------------------
void View::SlotChangeRevThreshMode(int val)
{
	if (val == Qt::Unchecked)
	{
		revThMode = 0;
	}
	else if (val = Qt::Checked)
	{

		revThMode = 1;
	}
	updateSlice();
	
}