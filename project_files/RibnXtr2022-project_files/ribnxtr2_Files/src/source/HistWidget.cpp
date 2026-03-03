#include "HistWidget.h"
//Added by qt3to4:
//#include <QWidgetLayout>
#include <QLabel>
//#include <q3Frame>
//#include <QVBoxLayout>



//histogramWidget

//----------------------------------------------------------------

HistWidget::~HistWidget()
{
	HistArray = NULL;
	data = NULL;
}
void HistWidget::SetVolumeOpacityFunctions(vtkPiecewiseFunction *f1, vtkPiecewiseFunction *f2)
{
	volumeScalarOpacityFunction = f1;
	volumeGradientpacityFunction = f2;
	this->IWidget->SetNewTransferFunction(f1, f2);
}
//----------------------------------------------------------------

HistWidget::HistWidget(QList<QSlider2*>* _sliderList, LinkedList<DataSet*>	*_DataSetList, QWidget *parent, QString name, Qt::WindowFlags f) :QWidget(parent, f)//MyDockWidget(name,parent,f)
{
	sliderList = _sliderList;
	volumeScalarOpacityFunction = NULL;
	volumeGradientpacityFunction = NULL;
	data = NULL;

	//TrFunction=new TransferFunction();
	//TrFunction->InsertPoint(0,0, 0,100,100);//ni
	//TrFunction->InsertPoint(1,10, 20,100,100);//ni
	//TrFunction->InsertPoint(2,20, 40,100,100);//ni
	//TrFunction->InsertPoint(3,30, 30,100,100);//ni
	//TrFunction->InsertPoint(4,40, 80,100,100);//ni
	//TrFunction->InsertPoint(5,50, 50,100,100);//ni
	//TrFunction->InsertPoint(6,60, 20,100,100);//ni
	//TrFunction->InsertPoint(7,70, 40,100,100);//ni
	//TrFunction->InsertPoint(8,80, 30,100,100);//ni
	//TrFunction->InsertPoint(9,90, 80,100,100);//ni
	//TrFunction->InsertPoint(10,100, 50,100,100);//ni



	LowerThreshold = 0;
	UpperThreshold = 0;
	LeftY = 0;
	RightY = 0;

	Mode = 1;

	LastMode = Mode;

	factor = 10000;

	DataSetList = _DataSetList;
	MainDataSet = NULL;

	WindowLevel = false;
	StretchPalette = false;

	ScaleSliderRange = 1000;
	YHSize = 200;

	//Creating Sliders ImageWidget AndConnections

	Viewer = false;
	QWidget *widget = this;//new QWidget(this);
						   //this->setWidget(widget);
						   //HLayout=new  QWidgetLayout( this, 1, -1, "HLayout");
	HLayout = new QHBoxLayout(widget);
	HLayout->setSpacing(0);
	HLayout->setMargin(0);
	widget->setLayout(HLayout);
	QWidget *xWidget = new QWidget(widget);
	xWidget->setLayout(new QVBoxLayout());
	//logCheckBox=new QCheckBox(xWidget);
	//logCheckBox->setChecked(true);
	//histGraphMode=1;
	////connect(logCheckBox,SIGNAL(toggled(bool)),this,SLOT(SlotHistGraphMode (bool)));
	//connect(logCheckBox,SIGNAL(toggled(bool)),this,SIGNAL(SignalThreshModeChanged (bool)));

	//xWidget->layout()->addWidget(logCheckBox);
	ScaleSlider = new QSlider2(sliderList ,Qt::Vertical, xWidget);
	xWidget->layout()->addWidget(ScaleSlider);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(0);

	HLayout->addWidget(xWidget);

	QWidget *TempWidget1 = new QWidget(widget);
	//setWidget(TempWidget1);
	QVBoxLayout *qbl_TempWidget1 = new QVBoxLayout;
	qbl_TempWidget1->setSpacing(0);
	qbl_TempWidget1->setMargin(0);
	TempWidget1->setLayout(qbl_TempWidget1);
	HLayout->addWidget(TempWidget1);
	//VLayout=new  QVBoxLayout( TempWidget1);//,1, -1, "VLayout");


	//QWidget *GridCol2 = new QWidget(2,TempWidget1);
	QWidget *GridCol2 = new QWidget(TempWidget1);
	QHBoxLayout *hbl_GridCol2 = new QHBoxLayout;
	hbl_GridCol2->setSpacing(0);
	hbl_GridCol2->setMargin(0);
	GridCol2->setLayout(hbl_GridCol2);
	qbl_TempWidget1->addWidget(GridCol2);




	QFrame*ThFcBox = new QFrame(TempWidget1);
	ThFcBox->setFrameStyle(QFrame::Box | QFrame::Raised);
	ThFcBox->setFixedWidth(120);

	ThFcBox->setLayout(new QVBoxLayout());
	ThFcBox->layout()->setSpacing(1);
	ThFcBox->layout()->setMargin(1);

	//ThFcBox->hide();
	xWidget = new QWidget(ThFcBox);
	xWidget->setLayout(new QVBoxLayout());
	ThFcBox->layout()->addWidget(xWidget);

	logCheckBox = new QCheckBox(xWidget);
	xWidget->layout()->addWidget(logCheckBox);
	logCheckBox->setChecked(true);
	logCheckBox->setText("logScale");
	histGraphMode = 1;
	//connect(logCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(SignalThreshModeChanged(bool)));
	connect(logCheckBox, SIGNAL(toggled(bool)), this, SLOT(SlotHistGraphMode(bool)));



	//revThreshBox


	QWidget* aWidget = new QWidget(xWidget);
	xWidget->layout()->addWidget(aWidget);
	aWidget->setLayout(new QHBoxLayout());
	aWidget->layout()->setSpacing(0);
	aWidget->layout()->setMargin(0);

	maskShowModeCheckBox = new QCheckBox(aWidget);
	aWidget->layout()->addWidget(maskShowModeCheckBox);
	maskShowModeCheckBox->setChecked(false);
	maskShowModeCheckBox->setText("threshMask");


	revThreshBox = new QCheckBox(aWidget);
	aWidget->layout()->addWidget(revThreshBox);
	revThreshBox->setChecked(false);
	revThreshBox->setText("revTh");

	//connect(logCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(SignalThreshModeChanged(bool)));
	connect(maskShowModeCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(SignalThreshModeChanged(bool)));


	volThreshsBox = new QCheckBox(xWidget);
	xWidget->layout()->addWidget(volThreshsBox);
	volThreshsBox->setChecked(false);
	volThreshsBox->setText("volThresh");

	xWidget = new QWidget(ThFcBox);

	ThFcBox->layout()->addWidget(xWidget);

	QGridLayout *qgl_ThFcBox = new QGridLayout;
	qgl_ThFcBox->setSpacing(0);
	qgl_ThFcBox->setMargin(0);
	//ThFcBox->setLayout(qgl_ThFcBox);	

	xWidget->setLayout(qgl_ThFcBox);


	hbl_GridCol2->addWidget(ThFcBox);





	//	ThFcBox->setSpacing(1);


	//	QVBox *ThFcBox = new QVBox(Gb1);
	ThresholdRadioButton = new QRadioButton(xWidget);
	ThresholdRadioButton->setText("Thresh");
	//ThresholdRadioButton->setChecked(true);
	OptimButton = new QPushButton("O", xWidget);
	OptimButton->setMaximumWidth(15);

	qgl_ThFcBox->addWidget(ThresholdRadioButton, 0, 0);
	qgl_ThFcBox->addWidget(OptimButton, 0, 1);

	TransferFunctionRadioButton = new QRadioButton(xWidget);
	TransferFunctionRadioButton->setText("T-Func");
	TransferFunctionRadioButton->setChecked(true);
	qgl_ThFcBox->addWidget(TransferFunctionRadioButton, 1, 0);

	OptimButton2 = new QPushButton("O2", xWidget);
	OptimButton2->setMaximumWidth(15);
	qgl_ThFcBox->addWidget(OptimButton2, 1, 1);

	connect(OptimButton, SIGNAL(clicked()), this, SLOT(GenerateOptimizedPalette()));
	connect(OptimButton2, SIGNAL(clicked()), this, SLOT(GenerateOptimizedPalette2()));



	//	VButtonLayout->addWidget(ThFcBox);
	//	VButtonLayout->addWidget(GenPaletteButton);
	//	VButtonLayout->addWidget(EqualizeButton);

	/*
	GenPaletteButton->setFixedWidth(80);
	EqualizeButton->setFixedWidth(80);
	GenPaletteButton->setFixedHeight(20);
	EqualizeButton->setFixedHeight(20);
	GenPaletteButton->setAutoMask(true);
	EqualizeButton->setAutoMask(true);
	*/


	QFrame *Gb2 = new QFrame(GridCol2);
	Gb2->setFrameStyle(QFrame::Box | QFrame::Raised);
	//	Gb2->setAutoMask(true);
	hbl_GridCol2->addWidget(Gb2);
	QVBoxLayout *VImageLayout = new  QVBoxLayout(Gb2);//,1, -1, "VLayout");
	Gb2->setLayout(VImageLayout);
	VImageLayout->setSpacing(0);
	//VImageLayout->setMargin(0);

	VImageLayout->setMargin(0);

	IWidget = new ImageWidget(Gb2, "Histogram");
	IWidget->show();
	//IWidget->SetMaskPaintMode(1);
	IWidget->SetNewTransferFunction(volumeScalarOpacityFunction, volumeGradientpacityFunction);
	QObject::connect(IWidget, SIGNAL(SignalSendPointArray(QPolygon, double, double)), this, SLOT(SlotSendPointArray(QPolygon, double, double)));;


	VImageLayout->addWidget(IWidget);


	//	QVBox *ThresholdsBox;
	//	QVBox *TransferFunctionBox;

	//--------------------------------------------------
	ThresholdsBox = new QWidget(TempWidget1);

	//ThresholdsBox->setTitle("ZENON");
	QVBoxLayout *vbl_ThresholdsBox = new QVBoxLayout;
	vbl_ThresholdsBox->setSpacing(0);
	vbl_ThresholdsBox->setMargin(0);
	ThresholdsBox->setLayout(vbl_ThresholdsBox);
	//ThresholdsBox->setSpacing(-1);
	ThresholdsBox->setFixedHeight(40);
	//	hbl_GridCol2->addWidget(ThresholdsBox);


	//QWidget *GridCol3 = new QWidget(2,ThresholdsBox);




	GridColTh1 = new QWidget(ThresholdsBox);
	GridColTh1->show();

	vbl_ThresholdsBox->addWidget(GridColTh1);
	QGridLayout *qgl_GridCol3 = new QGridLayout;
	qgl_GridCol3->setSpacing(0);
	qgl_GridCol3->setMargin(0);
	GridColTh1->setLayout(qgl_GridCol3);

	//	GridCol3->setAutoMask(true);
	//GridCol3->setFixedHeight(15);


	Thresh1Label = new QLabel(QString(""), GridColTh1);
	Thresh1Label->setMaximumHeight(15);
	Thresh1Label->setMinimumWidth(1);
	Thresh1Label->setFixedWidth(100);
	Thresh1Slider = new QSlider2(sliderList, Qt::Horizontal, GridColTh1);

	//QWidget *GridCol4 = new QWidget(2,ThresholdsBox);
	//	GridCol4->setAutoMask(true);
	//	GridCol4->setFixedHeight(15);

	Thresh2Label = new QLabel(QString(""), GridColTh1);
	Thresh2Label->setMaximumHeight(15);
	Thresh2Label->setMinimumWidth(1);
	Thresh2Label->setFixedWidth(100);
	Thresh2Slider = new QSlider2(sliderList ,Qt::Horizontal, GridColTh1);



	qgl_GridCol3->addWidget(Thresh1Label, 0, 0);
	qgl_GridCol3->addWidget(Thresh1Slider, 0, 1);
	qgl_GridCol3->addWidget(Thresh2Label, 1, 0);
	qgl_GridCol3->addWidget(Thresh2Slider, 1, 1);


	GridColTh2 = new QWidget(ThresholdsBox);
	GridColTh2->hide();

	vbl_ThresholdsBox->addWidget(GridColTh2);
	qgl_GridCol3 = new QGridLayout;
	qgl_GridCol3->setSpacing(0);
	qgl_GridCol3->setMargin(0);
	GridColTh2->setLayout(qgl_GridCol3);


	Thresh1VolLabel = new QLabel(QString(""), GridColTh2);
	Thresh1VolLabel->setMaximumHeight(15);
	Thresh1VolLabel->setMinimumWidth(1);
	Thresh1VolLabel->setFixedWidth(100);
	Thresh1VolLabel->setText("LevtVolTh");
	Thresh1VolSlider = new QSlider2(sliderList, Qt::Horizontal, GridColTh2);

	//QWidget *GridCol4 = new QWidget(2,ThresholdsBox);
	//	GridCol4->setAutoMask(true);
	//	GridCol4->setFixedHeight(15);

	Thresh2VolLabel = new QLabel(QString(""), GridColTh2);
	Thresh2VolLabel->setMaximumHeight(15);
	Thresh2VolLabel->setMinimumWidth(1);
	Thresh2VolLabel->setFixedWidth(100);
	Thresh2VolLabel->setText("RightVolTh");
	Thresh2VolSlider = new QSlider2(sliderList, Qt::Horizontal, GridColTh2);


	connect(volThreshsBox, SIGNAL(toggled(bool)), this, SLOT(SlotShowThresholdsMode(bool)));



	qgl_GridCol3->addWidget(Thresh1VolLabel, 0, 0);
	qgl_GridCol3->addWidget(Thresh1VolSlider, 0, 1);
	qgl_GridCol3->addWidget(Thresh2VolLabel, 1, 0);
	qgl_GridCol3->addWidget(Thresh2VolSlider, 1, 1);
	//---------------------------------------------------------

	QPushButton *AddNewPointButton;
	QPushButton *ClearAllPointsButton;


	TransferFunctionBox = new QGroupBox(TempWidget1);
	QVBoxLayout *vbl_TransferFunctionBox = new QVBoxLayout;
	vbl_TransferFunctionBox->setSpacing(0);
	vbl_TransferFunctionBox->setMargin(0);
	TransferFunctionBox->setLayout(vbl_TransferFunctionBox);
	TransferFunctionBox->hide();
	//	TransferFunctionBox->setSpacing(-1);
	TransferFunctionBox->setFixedHeight(40);


	//QWidget *TransfBox1 = new QWidget(TransferFunctionBox);
	QWidget *TransfBox1 = new	QWidget(TransferFunctionBox);

	QHBoxLayout *hbl_TransfBox1 = new QHBoxLayout;
	hbl_TransfBox1->setSpacing(0);
	hbl_TransfBox1->setMargin(0);
	TransfBox1->setLayout(hbl_TransfBox1);
	vbl_TransferFunctionBox->addWidget(TransfBox1);


	AddNewPointButton = new QPushButton("Add Point", TransfBox1);
	ClearAllPointsButton = new QPushButton("Clear All Points", TransfBox1);
	hbl_TransfBox1->addWidget(AddNewPointButton);
	hbl_TransfBox1->addWidget(ClearAllPointsButton);



	//---------------------------------------------------------

	//qbl_TempWidget1->addWidget(Gb2);
	qbl_TempWidget1->addWidget(ThresholdsBox);
	qbl_TempWidget1->addWidget(TransferFunctionBox);



	//	VLayout->addWidget(GridCol3);
	//	VLayout->addWidget(GridCol4);







	LeftY = 1.0;
	RightY = 1.0;


	this->SlotSetTransferFunctionMode();
}

//----------------------------------------------------------------

void HistWidget::DeleteData()
{
	if (Viewer == true)
	{
		HistArray = NULL;
		delete	Image;
		delete	TempPaletteArray;
		delete	Bins;
	}
	data = NULL;
	Viewer = false;

	IWidget->DataCleared();

	LowerThreshold = 0;
	UpperThreshold = 0;
	LeftY = 0;
	RightY = 0;

	disconnect(ThresholdRadioButton, SIGNAL(clicked()), this, SLOT(SlotSetThresholdMode()));
	disconnect(TransferFunctionRadioButton, SIGNAL(clicked()), this, SLOT(SlotSetTransferFunctionMode()));
	disconnect(ScaleSlider, SIGNAL(sliderPressed()), ScaleSlider, SLOT(setFocus()));
	disconnect(Thresh1Slider, SIGNAL(sliderPressed()), Thresh1Slider, SLOT(setFocus()));
	disconnect(Thresh2Slider, SIGNAL(sliderPressed()), Thresh2Slider, SLOT(setFocus()));

	disconnect(ScaleSlider, SIGNAL(valueChanged(int)), this, SLOT(RedrawHistogram(int)));
	disconnect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
	disconnect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
	//disconnect(Thresh1Slider,SIGNAL(sliderReleased()),this,SIGNAL(ThresholdsChanged()));
	//disconnect(Thresh2Slider,SIGNAL(sliderReleased()),this,SIGNAL(ThresholdsChanged()));


	disconnect(IWidget, SIGNAL(ImageReleased()), this, SIGNAL(ThresholdsChanged()));
	disconnect(IWidget, SIGNAL(SendPosition(int, int, int)), this, SLOT(SlotSetThresholds(int, int, int)));
	disconnect(IWidget, SIGNAL(SignalSendUserTransferFunction()), this, SIGNAL(SignalSendUserTransferFunction()));


}
//----------------------------------------------------------------

void HistWidget::SlotSetThresholds(int X, int Y, int Button)
{

	if (Mode == 0)
	{
		// tu dla thresholdow

		double YPos;
		if (XHSize == 1) XHSize++;
		if (YHSize == 1) YHSize++;

		unsigned int XPos = Round((double)(X*Range / (XHSize - 1))) + FirstHistValue;
		if (YHSize - Y>10)
			YPos = (double)(Y*1.0 / (YHSize - 1));
		else
			YPos = 1.0;


		if (WindowLevel == false)
		{
			if ((Button == 0) || (Button == 1))
			{
				Thresh1Slider->setValue(XPos);
				LeftY = YPos;
			}
			else if ((Button == 3) || (Button == 4))
			{
				Thresh2Slider->setValue(XPos);
				RightY = YPos;
			}
		}
		else
		{
			//!!!!!!!!! dopisac!

			if ((Button == 0) || (Button == 1))
			{
				disconnect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
				disconnect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));

				Thresh2Slider->setValue(UpperThreshold - XPos);

				connect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
				connect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));

				Thresh1Slider->setValue(XPos + Round((UpperThreshold - XPos) / 2.0));

				LeftY = YPos;
			}
			else if ((Button == 3) || (Button == 4))
			{
				disconnect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
				disconnect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));

				Thresh2Slider->setValue(XPos - LowerThreshold);

				connect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
				connect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));

				Thresh1Slider->setValue(LowerThreshold + Round((XPos - LowerThreshold) / 2.0));



				RightY = YPos;
			}


		}
	}
	else
	{
		// a tu dla transfer functions

		IWidget->RepaintWithCross(X, Y);

	}

	if ((Button == 6) || (Button == 7))
	{
		emit SignalSendPaletteRotationValue(Round((double)(X*255.0 / (XHSize - 1))));
	}

}

//----------------------------------------------------------------
void HistWidget::SlotHistGraphMode(bool val)
{
	if (data != NULL)
	{
		histGraphMode = val;

		Bins = new int[XHSize];
		memset(Bins, 0, (XHSize) * sizeof(int));
		HistArray = data->GetHistogramData();
		int tempAA;
		int i;
		for (i = 0; i <= Range; i++)
		{
			tempAA = Round((double)(i*(XHSize - 1)) / (double)(Range));
			//if (HistArray[i]>Bins[tempAA])
			Bins[tempAA] += HistArray[i + FirstHistValue];

		}
		if (histGraphMode == 1)
		{
			for (i = 0; i < XHSize; i++)
			{
				Bins[i] = log(1 + (double)(Bins[i])) * 100;

			}

		}

		BinsMax = 1;
		BinsMin = INT_MAX;
		unsigned int ii;
		for (ii = 0; ii < XHSize; ii++)
			if (Bins[ii] > BinsMax) BinsMax = Bins[ii];
			else if (BinsMin > Bins[ii]) BinsMin = Bins[ii];


			this->RedrawHistogram(ScaleSlider->value());
	}

}
void HistWidget::InsertNewData(int MainPos)
{
	if (data == NULL)
	{
		connect(ThresholdRadioButton, SIGNAL(clicked()), this, SLOT(SlotSetThresholdMode()));
		connect(TransferFunctionRadioButton, SIGNAL(clicked()), this, SLOT(SlotSetTransferFunctionMode()));
		connect(ScaleSlider, SIGNAL(sliderPressed()), ScaleSlider, SLOT(setFocus()));
		connect(Thresh1Slider, SIGNAL(sliderPressed()), Thresh1Slider, SLOT(setFocus()));
		connect(Thresh2Slider, SIGNAL(sliderPressed()), Thresh2Slider, SLOT(setFocus()));
		connect(ScaleSlider, SIGNAL(valueChanged(int)), this, SLOT(RedrawHistogram(int)));
		connect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
		connect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
		//connect(Thresh1Slider,SIGNAL(sliderReleased()),this,SIGNAL(ThresholdsChanged()));
		//connect(Thresh2Slider,SIGNAL(sliderReleased()),this,SIGNAL(ThresholdsChanged()));
		connect(IWidget, SIGNAL(ImageReleased()), this, SIGNAL(ThresholdsChanged()));
		connect(IWidget, SIGNAL(SendPosition(int, int, int)), this, SLOT(SlotSetThresholds(int, int, int)));
		connect(IWidget, SIGNAL(SignalSendUserTransferFunction()), this, SIGNAL(SignalSendUserTransferFunction()));
	}

	MainDataSet = DataSetList->GetItemAtPos(MainPos)->GetData();
	data = MainDataSet->GetRawData();

	if (data->GetHistogramCalculated() == false)
		data->CalculateHistogram();

	HistMax = data->GetHistMax();
	HistMin = data->GetHistMin();
	FirstHistValue = 0;//data->GetFirstHistValue();
	LastHistValue = data->GetLastHistValue();

	Range = LastHistValue - FirstHistValue;
	if (Range == 0) Range = 1;

	//	QMessageBox::about( 0,"Yo:",QString::number(LastHistValue));
	if (Viewer == true)
	{
		HistArray = NULL;
		delete	Image;
		delete	TempPaletteArray;
		delete	Bins;
	}

	Viewer = false;




	ScaleSlider->setRange(0, ScaleSliderRange);

	ScaleSlider->setValue(0);//(int)(ScaleSliderRange*0.3));// tu moze ten median?!


	Thresh1Slider->setRange(FirstHistValue, LastHistValue);

	Thresh1VolSlider->setRange(FirstHistValue, LastHistValue);

	if (WindowLevel == true)
	{
		Thresh2Slider->setRange(1, Range);
		Thresh1Slider->setValue(FirstHistValue + Round((double)(Range / 2)));
		Thresh2Slider->setValue(Range);

		Thresh2VolSlider->setRange(1, Range);
		Thresh1VolSlider->setValue(FirstHistValue + Round((double)(Range / 2)));
		Thresh2VolSlider->setValue(Range);

	}
	else
	{
		Thresh2Slider->setRange(FirstHistValue, LastHistValue);
		Thresh1Slider->setValue(FirstHistValue);
		Thresh2Slider->setValue(LastHistValue);

		Thresh2VolSlider->setRange(FirstHistValue, LastHistValue);
		Thresh1VolSlider->setValue(FirstHistValue);
		Thresh2VolSlider->setValue(LastHistValue);


	}



	int i;


	//tych linijek nie kapuje!!!!
	int  XX = 5000;

	if (Range >= XX)
		XHSize = XX;
	else
		XHSize = Range;

	if (Range<2) XHSize = 1;
	/// to jest costam





	YHSize = 200;

	if (XHSize>1)
		ScalePalette = (double)(255.0) / (double)(XHSize - 1);
	else
		ScalePalette = 1;
	if (Range>0)
		ScalePalette2 = (double)(255.0) / (double)(Range);
	else
		ScalePalette2 = 1;

	sx = data->GetNx();
	sy = data->GetNy();
	sz = data->GetNz();



	UpperThreshold = LastHistValue;
	LowerThreshold = FirstHistValue;





	TempPaletteArray = new int[Range + 1];
	for (i = 0; i <= Range; i++)
		TempPaletteArray[i] = i;


	//	Image = new QImage(rx,ry,32);


	PalTemp = new PaletteRGBA();
	for (i = 0; i <= 255; i++)
	{
		PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
		PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
		PalTemp->B[i] = MainDataSet->GetPalette()->B[i];

	}







	Max = data->GetHistMax();

	//	Image = new QImage(XHSize,YHSize,32);
	Image = new QImage(XHSize, YHSize, QImage::Format_ARGB32);
	IWidget->setValues(Image, XHSize, YHSize, 0.0, 0.0);
	//Image->fill(Qt::green);

	Viewer = true;

	SlotHistGraphMode(histGraphMode);


	int *tempBins = new int[XHSize];
	memcpy(tempBins, Bins, (XHSize) * sizeof(int));

	int MedianValue = Median(tempBins, XHSize) * 4;

	delete[]tempBins;

	//	QSortInt(Bins,0,XHSize-1);
	//	int MedianValue=Bins[XHSize/2];


	//double XXTemp=(1.0-(double)MedianValue/((double)BinsMax)) *log10( (double)(ScaleSliderRange*factor)+1.0);
	//int ValX=ScaleSliderRange-Round((double)(pow((double)10,(double)XXTemp)-1)/factor);

	disconnect(ScaleSlider, SIGNAL(valueChanged(int)), this, SLOT(RedrawHistogram(int)));
	ScaleSlider->setValue(0);
	connect(ScaleSlider, SIGNAL(valueChanged(int)), this, SLOT(RedrawHistogram(int)));

	//QMessageBox::about( 0,"Yo:",QString::number(MedianValue)+" "+QString::number(BinsMax)+" "+QString::number(ValX));


	//RedrawHistogram(ScaleSlider->value());

	ThreshSelect(0);


}


//----------------------------------------------------------------

//----------------------------------------------------------------
void HistWidget::Update()
{
	//This makes an update of a histogram view - in depend of a Thresholds values
	if (data == NULL) return;
	if (Viewer == true)
		IWidget->repaint();


	QString text1;
	QString text2;


	double lt = Thresh1Slider->value();
	double rt = Thresh2Slider->value();
	double lhv = LastHistValue;
	double rescale = data->dicomRescaleValue;

	if (WindowLevel == true)
	{
		int minV = (data->dicomMinValue > -100000) ? data->dicomMinValue : 0;
		if (minV != 0)
		{
			lt *= rescale;
			rt *= rescale;
			lhv *= rescale;

			lt += minV;


		}
		text1.sprintf("Window(%.2f/%.2f)    ", rt, lhv);
		text2.sprintf("Level(%.2f/%.2f)    ", lt, lhv);
	}
	else
	{
		int minV = (data->dicomMinValue > -100000) ? data->dicomMinValue : 0;
		//if (minV != 0)
		{
			lt *= rescale;
			rt *= rescale;
			lhv *= rescale;

			lt += minV;
			rt += minV;
			lhv += minV;
		}
		text1.sprintf("Upper(%.2f/%.2f)    ", rt, lhv);
		text2.sprintf("Lower(%.2f/%.2f)    ", lt, lhv);
	}

	Thresh2Label->setText(text1);
	Thresh1Label->setText(text2);

}

//----------------------------------------------------------------

void HistWidget::RedrawHistogram(int Scale)
{
	// scales the histogram horizontaly
	//return;
	//if ((Scale == 0) ||(
	if ((BinsMax == 0))
	{
		Update();
		return;
	}
	if (Viewer == true)
	{
		unsigned char *dataX;
		dataX = (unsigned char *)Image->bits();
		bool Step = (data->GetMachineEndian() == 1) ? 1 : 0;
		unsigned int Pos;
		unsigned int t1, t2, t3, t4;;

		unsigned char Col1[3] = { 255, 0, 0 };
		unsigned char	Col2[3] = { 30, 30, 30 };;

		QColor background(Qt::black);
		QColor foreground(Qt::white);

		//	Col1=200;
		//	Col2=100;

		//	QColor frontCol (255 - backgroundColor.red(), 255 - backgroundColor.red(), 255 - backgroundColor.red());

		//		int BinsRange=BinsMax-BinsMin;



		// factor=50000;
		//Scale=ScaleSliderRange-Scale;

		//	double tempa=1.0-log10 ( (double)( Scale*factor)+1.0 )/log10( (double)(ScaleSliderRange*factor)+1.0); // (0-1>


		//double tempM=1.0/(BinsMax*tempa);
		double tempM = 1.0 + 10 * (double)Scale / ScaleSliderRange;
		tempM /= BinsMax;
		//QMessageBox::about( 0,"Yo:",QString::number(tempM));

		unsigned int i, j;

		double tempH;
		int tempCol;


		int LowerThresholdX = LowerThreshold;
		int UpperThresholdX = UpperThreshold;

		/*if (Mode==1)
		{
		LowerThresholdX=FirstHistValue;
		UpperThresholdX=Range+FirstHistValue;


		if (LastMode!=Mode)
		SlotSetFullPalette();


		}*/

		t2 = ((YHSize - 1) * 4 * XHSize) + Step;
		t3 = 4 * XHSize;

		unsigned int LTR, UTR;
		//	if (Mode==0)
		//	{
		LTR = Round((double)(((LowerThresholdX - FirstHistValue)*(XHSize - 1)) / (double)Range));
		UTR = Round((double)(((UpperThresholdX - FirstHistValue)*(XHSize - 1)) / (double)Range));
		//	}
		/*	else
		{

		LTR=0;
		UTR=XHSize-1;
		}
		*/
		if (StretchPalette == true)
		{

			double TmpSC = (XHSize>1) ? (double)Range / (XHSize - 1) : 1;

			double StretchedFactor = (Range>255) ? 255.0 : Range - 1;

			if (UpperThresholdX - LowerThresholdX>0)
				StretchedScalePalette = (double)(StretchedFactor / (UpperThresholdX - LowerThresholdX)*TmpSC);
			else
				StretchedScalePalette = StretchedFactor;


			//poczatek
			if (LTR>0)
				for (i = 0; i<LTR; i++)
				{

					tempH = (double)Bins[i] * tempM;
					if (tempH>1.0)
					{
						tempH = 1.0;
					}
					t4 = Round(tempH*(double)YHSize);
					t1 = t2 + (i << 2);

					Pos = t1;
					for (j = 0; j<t4; j++)
					{
						*(dataX + Pos) = Col1[0];
						*(dataX + Pos + 1) = Col1[1];
						*(dataX + Pos + 2) = Col1[2];;
						Pos -= t3;
					}

					Pos = t1 - t4*t3;
					for (j = t4; j<YHSize; j++)
					{
						*(dataX + Pos) = Col2[0];
						*(dataX + Pos + 1) = Col2[1];
						*(dataX + Pos + 2) = Col2[2];
						Pos -= t3;
					}


				}
			//srodek
			for (i = LTR; i <= UTR; i++)
			{

				tempH = (double)(Bins[i] * tempM);
				if (tempH>1.0) tempH = 1.0;
				tempCol = TempPaletteArray[Round((i - LTR)*StretchedScalePalette)];
				t4 = Round(tempH*(double)YHSize);
				t1 = t2 + (i << 2);

				Pos = t1;
				for (j = 0; j<t4; j++)
				{
					*(dataX + Pos) = PalTemp->B[tempCol];
					*(dataX + Pos + 1) = PalTemp->G[tempCol];
					*(dataX + Pos + 2) = PalTemp->R[tempCol];
					Pos -= t3;
				}

				Pos = t1 - t4*t3;
				for (j = t4; j<YHSize; j++)
				{
					*(dataX + Pos) = Col1[0];
					*(dataX + Pos + 1) = Col1[1];
					*(dataX + Pos + 2) = Col1[2];
					Pos -= t3;
				}

			}

			//koniec
			if (UTR + 1<XHSize)
				for (i = UTR + 1; i<XHSize; i++)
				{

					tempH = (double)(Bins[i] * tempM);
					if (tempH>1.0) tempH = 1.0;
					t4 = Round(tempH*(double)YHSize);
					t1 = t2 + (i << 2);

					Pos = t1;
					for (j = 0; j<t4; j++)
					{
						*(dataX + Pos) = Col1[0];
						*(dataX + Pos + 1) = Col1[1];
						*(dataX + Pos + 2) = Col1[2];
						Pos -= t3;
					}

					Pos = t1 - t4*t3;
					for (j = t4; j<YHSize; j++)
					{
						*(dataX + Pos) = Col2[0];
						*(dataX + Pos + 1) = Col2[1];
						*(dataX + Pos + 2) = Col2[2];
						Pos -= t3;
					}
				}


		}
		else
		{

			//poczatek
			if (LTR>0)
				for (i = 0; i<LTR; i++)
				{

					tempH = (double)Bins[i] * tempM;
					if (tempH>1.0) tempH = 1.0;

					t4 = Round(tempH*(double)YHSize);
					t1 = t2 + (i << 2);

					Pos = t1;
					for (j = 0; j<t4; j++)
					{
						*(dataX + Pos) = Col1[0];
						*(dataX + Pos + 1) = Col1[1];
						*(dataX + Pos + 2) = Col1[2];
						Pos -= t3;
					}

					Pos = t1 - t4*t3;
					for (j = t4; j<YHSize; j++)
					{
						*(dataX + Pos) = Col2[0];
						*(dataX + Pos + 1) = Col2[1];
						*(dataX + Pos + 2) = Col2[2];
						Pos -= t3;
					}
				}
			//srodek
			///			FILE *Do_opa;
			//Do_opa=fopen("c:\\po.txt","a");


			for (i = LTR; i <= UTR; i++)
			{


				tempH = (double)(Bins[i] * tempM);
				if (tempH>1.0) tempH = 1.0;
				tempCol = Round((double)(i)*ScalePalette);

				if ((tempCol<0) || (tempCol>255)) QMessageBox::about(0, "Yo:", QString::number(i));
				//fprintf(Do_opa,"%d ",tempCol);
				t4 = Round(tempH*(double)YHSize);
				t1 = t2 + (i << 2);

				Pos = t1;
				for (j = 0; j<t4; j++)
				{
					*(dataX + Pos) = PalTemp->B[tempCol];
					*(dataX + Pos + 1) = PalTemp->G[tempCol];
					*(dataX + Pos + 2) = PalTemp->R[tempCol];
					Pos -= t3;
				}

				Pos = t1 - t4*t3;
				for (j = t4; j<YHSize; j++)
				{
					*(dataX + Pos) = Col1[0];
					*(dataX + Pos + 1) = Col1[1];
					*(dataX + Pos + 2) = Col1[2];
					Pos -= t3;
				}

			}
			//fprintf(Do_opa,"n");
			//fclose(Do_opa);
			//koniec
			if (UTR + 1<XHSize)
				for (i = UTR + 1; i<XHSize; i++)
				{

					tempH = (double)(Bins[i] * tempM);
					if (tempH>1.0) tempH = 1.0;
					t4 = Round(tempH*(double)YHSize);
					t1 = t2 + (i << 2);

					Pos = t1;
					for (j = 0; j<t4; j++)
					{
						*(dataX + Pos) = Col1[0];
						*(dataX + Pos + 1) = Col1[1];
						*(dataX + Pos + 2) = Col1[2];
						Pos -= t3;
					}

					Pos = t1 - t4*t3;
					for (j = t4; j<YHSize; j++)
					{
						*(dataX + Pos) = Col2[0];
						*(dataX + Pos + 1) = Col2[1];
						*(dataX + Pos + 2) = Col2[2];
						Pos -= t3;
					}
				}

		}

		Update();
		LastMode = Mode;
	}





}
//----------------------------------------------------------------

void HistWidget::ThreshSelect(int value)
{
	//Selecting The New Threshold
	if (Viewer == true)
	{


		if (WindowLevel == true)
		{
			LowerThreshold = Thresh1Slider->value() - Round(Thresh2Slider->value() / 2.0);
			UpperThreshold = Thresh1Slider->value() + Round(Thresh2Slider->value() / 2.0);

			if (LowerThreshold<FirstHistValue) LowerThreshold = FirstHistValue;
			if (LowerThreshold>LastHistValue) LowerThreshold = Range;

			if (UpperThreshold<FirstHistValue) UpperThreshold = FirstHistValue;
			if (UpperThreshold>LastHistValue) UpperThreshold = LastHistValue;
		}
		else
		{
			LowerThreshold = Thresh1Slider->value();
			UpperThreshold = Thresh2Slider->value();


			if (LowerThreshold >= UpperThreshold - 1)
				if (LowerThreshold>FirstHistValue)
				{
					LowerThreshold = UpperThreshold - 1;
					Thresh1Slider->setValue(LowerThreshold);
				}
				else
				{
					UpperThreshold = FirstHistValue + 1;


				}

			//temp
			//	if (LowerThreshold==UpperThreshold==FirstHistValue)
			//		UpperThreshold+=1;


		}

		if (StretchPalette == true)
		{
			RedrawHistogram(ScaleSlider->value());
		}
		else
		{
			int a = Round(ScalePalette2*(LowerThreshold - FirstHistValue));
			int b = Round(ScalePalette2*(UpperThreshold - FirstHistValue));
			int i;


			if (a>0)
				for (i = 0; i <= a - 1; i++)
				{
					PalTemp->R[i] = 0;
					PalTemp->G[i] = 0;
					PalTemp->B[i] = 0;
				}


			for (i = a; i <= b; i++)
			{
				PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
				PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
				PalTemp->B[i] = MainDataSet->GetPalette()->B[i];
			}

			if (b<Range)
				for (i = b + 1; i <= 255; i++)
				{
					PalTemp->R[i] = 0;
					PalTemp->G[i] = 0;
					PalTemp->B[i] = 0;
				}



			Update();
		}
		emit SendThresholds(LowerThreshold, UpperThreshold);
		//emit SignalSendUserTransferFunction(TrFunction);
		RedrawHistogram(ScaleSlider->value());
		emit ThresholdsChanged();
	}

}


void	HistWidget::GiveThresholds()
{
	emit SendThresholds2(Thresh1Slider->value(), Thresh2Slider->value(), TempPaletteArray);
}

//----------------------------------------------------------------



void	HistWidget::SlotSwitchToThresholdMode()
{

	if (WindowLevel == true)
	{

		disconnect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
		disconnect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));


		WindowLevel = false;

		Thresh1Slider->setValue(LowerThreshold);
		Thresh2Slider->setValue(UpperThreshold);



		Update();
		connect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
		connect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));

	}

}

//-----------------------------------------------------

void	HistWidget::SlotSwitchToWindowLevelMode()
{

	if (WindowLevel == false)
	{
		disconnect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
		disconnect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));


		WindowLevel = true;

		Thresh1Slider->setValue(LowerThreshold + Round((UpperThreshold - LowerThreshold) / 2.0));
		Thresh2Slider->setValue(UpperThreshold - LowerThreshold);

		Update();
		connect(Thresh1Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
		connect(Thresh2Slider, SIGNAL(valueChanged(int)), this, SLOT(ThreshSelect(int)));
	}

}

//-------------------------------------------------------


void HistWidget::HistogramEqualization()
{

	//Histogram equalization

	if (Viewer == true)
	{
		int Ni = 0;

		int i;



		for (i = 0; i<Range; i++)
		{
			Ni += HistArray[i];
			TempPaletteArray[i] = Round(((double)(Ni) / (double)data->GetVolumeSize())*Range);

		}

		//	RedrawHistogram();
		RedrawHistogram(ScaleSlider->value());
	}
	emit SendTempPalette(TempPaletteArray);
}

//----------------------------------------------------


void HistWidget::SlotSwitchToStretchedPaletteMode()
{

	if (StretchPalette == false)
	{
		StretchPalette = true;
		if (MainDataSet != NULL)
		{

			int i;
			//	for ( i=0;i<256;i++)
			//	Image->setColor(i,Palette[i]);

			for (i = 0; i <= 255; i++)
			{
				PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
				PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
				PalTemp->B[i] = MainDataSet->GetPalette()->B[i];
			}


			ThreshSelect(0);
			RedrawHistogram(ScaleSlider->value());
		}
	}

}

//----------------------------------------------------


void HistWidget::SlotSwitchToNormalPaletteMode()
{

	if (StretchPalette == true)
	{

		StretchPalette = false;
		if (MainDataSet != NULL)
		{
			ThreshSelect(0);
			RedrawHistogram(ScaleSlider->value());
		}

	}
}


//----------------------------------------------------



void HistWidget::ChangePaletteType()
{
	int i;
	for (i = 0; i <= 255; i++)
	{
		PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
		PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
		PalTemp->B[i] = MainDataSet->GetPalette()->B[i];

	}

	ThreshSelect(0);

	RedrawHistogram(ScaleSlider->value());

}
//----------------------------------------------------
void HistWidget::GenerateOptimizedPalette2()
{
	if (Viewer == true)
	{

		int size = this->volumeScalarOpacityFunction->GetSize();
		if (size>1)
		{
			srand((unsigned)time(NULL));
			//int *pointtab=new int[size];
			int i;
			double max = (data->GetMaxValue()>0) ? data->GetMaxValue() : 1;
			double scale = 255.0 / max;
			double val[4];
			double x;
			double r, g, b;
			vtkSmartPointer <vtkPiecewiseFunction> rf = vtkSmartPointer <vtkPiecewiseFunction>::New();
			vtkSmartPointer <vtkPiecewiseFunction> gf = vtkSmartPointer <vtkPiecewiseFunction>::New();
			vtkSmartPointer <vtkPiecewiseFunction> bf = vtkSmartPointer <vtkPiecewiseFunction>::New();

			for (i = 0; i<size; i++)
			{
				volumeScalarOpacityFunction->GetNodeValue(i, val);
				x = val[0] * scale;
				r = Round(255.0*((double)(rand()) / (double)RAND_MAX));
				g = Round(255.0*((double)(rand()) / (double)RAND_MAX));
				b = Round(255.0*((double)(rand()) / (double)RAND_MAX));
				rf->AddPoint(x, r);
				gf->AddPoint(x, g);
				bf->AddPoint(x, b);
			}

			PaletteRGBA pal;
			for (i = 0; i<255; i++)
			{
				pal.R[i] = Round(rf->GetValue(i));
				pal.G[i] = Round(gf->GetValue(i));
				pal.B[i] = Round(bf->GetValue(i));
			}
			emit SignalSendNewPalette(pal);

		}
	}

}
//-------------------------------------------
void HistWidget::GenerateOptimizedPalette()
{

	if (Viewer == true)
	{
		FILE *czesc;
		int i, j, k;
		int size = LastHistValue + 1;

		double *logHistTab = new double[size];
		double *logAgerageTab = new double[size];
		//	double *logMedianTab=new double [size];

		// zrobic histogram w skali logarytmicznej

		int bb;
		for (i = 0; i<size; i++)
		{
			bb = HistArray[i];
			if (bb>0)
				logHistTab[i] = log((double)bb);
			else
				logHistTab[i] = 0;
		}

		int FilterSize;
		if (size>100)
			FilterSize = Round(size / 100.0);
		else
			FilterSize = 1;
		int TempCounter;

		double *FilterTab = new double[FilterSize * 2 + 1];
		int ipj;
		double tmp2, tmpmax;
		int nr = Round((double)FilterSize / 2.0) + 1;

		//moving average
		for (k = 0; k<1; k++)
			for (i = 0; i<size; i++)
			{
				tmp2 = 0;
				tmpmax = 0;
				TempCounter = 0;
				for (j = -nr; j <= nr; j++)
				{
					ipj = i + j;
					if ((ipj >= 0) && (ipj<size))
					{
						TempCounter++;
						tmp2 += logHistTab[ipj];

					}
				}

				logAgerageTab[i] = tmp2 / (double)(TempCounter);
			}


		// mendian filtering

		//for (k=0;k<2;k++)
		/*		for(i=0;i<size;i++)
		{
		TempCounter=0;
		for (j=-FilterSize;j<=FilterSize;j++)
		if (((i+j)>=0)&&((i+j)<size))
		FilterTab[TempCounter++]=logAgerageTab[i+j];

		logMedianTab[i]=Median (FilterTab,TempCounter-1);

		}
		*/
		//////czesc=fopen("c:\\czesc1.dat","w");for(i=0;i<size;i++)fprintf(czesc,"%d %lf\n",i,(logHistTab[i]));fclose(czesc);
		////czesc=fopen("c:\\czesc2.dat","w");for(i=0;i<size;i++)fprintf(czesc,"%d %lf\n",i,(logAgerageTab[i]));fclose(czesc);
		//		//czesc=fopen("c:\\czesc3.dat","w");for(i=0;i<size;i++)fprintf(czesc,"%d %lf\n",i,(logMedianTab[i]));fclose(czesc);

		// peak finder

		//znajde 256 binow na poczatek
		tHistTab = logAgerageTab;
		int smallSize = 256;
		double *smallHistTab = new double[smallSize];
		memset(smallHistTab, 0, smallSize * sizeof(double));
		int pp;
		double factor = (double)(smallSize - 1) / (double)(size - 1);
		for (i = 0; i<size; i++)
		{
			pp = (int)(i*factor);
			//if (tHistTab[i]>smallHistTab[pp]) smallHistTab[pp]=tHistTab[i];
			/*	if (smallHistTab[pp]>0)
			smallHistTab[pp]=(smallHistTab[pp]+tHistTab[i])/2;
			else
			smallHistTab[pp]=tHistTab[i];*/

			smallHistTab[pp] += tHistTab[i];

		}
		////czesc=fopen("c:\\czesc4.dat","w");for(i=0;i<smallSize;i++)fprintf(czesc,"%d %lf\n",i,smallHistTab[i]);fclose(czesc);

		IntDoubleStruct	**tempdouble = new IntDoubleStruct*[size];
		double p;
		int d = 10;
		double maxdl, maxdr, delta;
		int pos;
		int peaks = 0;
		double tol = 10e-4;

		double maxVal = 0;
		double avg = 0;
		for (i = 0; i<smallSize; i++)
		{
			if (smallHistTab[i]>maxVal) maxVal = smallHistTab[i];
			avg += smallHistTab[i];
		}
		avg /= size;

		double cut = 0.1*maxVal;
		for (i = 0; i<smallSize; i++)
		{
			tempdouble[i] = new IntDoubleStruct;
			tempdouble[i]->IntVar = i;

			if (smallHistTab[i]>cut)
			{
				//sprawdzam z lewej czy jest ten sam znak otoczenia
				maxdl = maxdr = 0;
				int left, right;
				int sign;
				//**
				if (i == 84)
				{
					i = i;
				}
				if (i>1)
				{
					delta = smallHistTab[i - 1] - smallHistTab[i];
					sign = 0; if (delta>tol) sign = 1; else if (delta<-tol) sign = -1;
					left = sign;
					for (j = 2; j<d; j++)
					{
						pos = i - j;
						if (pos >= 0)
						{
							delta = smallHistTab[pos] - smallHistTab[i];
							sign = 0; if (delta>0) sign = 1; else sign = -1;

							if (!((sign == left) || (sign == 0) || ((sign == 1) && (left == 0)) || ((sign == -1) && (left == 0))))
								//if  ((sign!=left)
								//	&&(!(sign*left==0)))
							{
								maxdl = 0;
								left = -2;
								break;
							}
							if (fabs(delta) >maxdl) maxdl = fabs(delta);
						}
					}
				}
				//**
				if (i<smallSize - 1)
				{
					delta = smallHistTab[i + 1] - smallHistTab[i];
					sign = 0; if (delta>tol) sign = 1; else if (delta<-tol) sign = -1;
					right = sign;
					for (j = 2; j<d; j++)
					{
						pos = i + j;
						if (pos<smallSize)
						{

							delta = smallHistTab[pos] - smallHistTab[i];
							//sign =0; if (delta>0) sign=1;else sign=-1;

							//if ((sign!=right)
							//	&&(!(sign*right==0)))
							if (!((sign == right) || (sign == 0) || ((sign == 1) && (right == 0)) || ((sign == -1) && (right == 0))))
							{
								maxdr = 0;
								right = 2;
								break;
							}
							if (fabs(delta) >maxdr) maxdr = fabs(delta);
						}
					}
				}
				//sprawdzam z prawej czy jest skok
				double val = -1;
				if ((left == -1) && (right == -1))	val = 8;
				if ((left == -1) && (right == 0))	val = 7;
				if ((left == -1) && (right == 1))	val = 6;
				if ((left == 0) && (right == -1))	val = 5;
				if ((left == 0) && (right == 0))		val = 4;
				if ((left == 0) && (right == 1))		val = 3;
				if ((left == 1) && (right == -1))	val = 2;
				if ((left == 1) && (right == 0))		val = 1;
				if ((left == 1) && (right == 1))		val = 0;

				tempdouble[i]->DoubleVar = val;
				double p = (maxdr + maxdl);
				if ((p>0.2) && (((peaks == 0) && (val == 2)) ||
					(val == 5) || (val == 8)))
				{
					tempdouble[i]->DoubleVar = val;
					peaks++;
				}
				else
				{
					tempdouble[i]->DoubleVar = 0;
				}
			}
			else
				tempdouble[i]->DoubleVar = 0;

		}

		//tempdouble[140]->DoubleVar=8;
		//tempdouble[30]->DoubleVar=8;
		//tempdouble[120]->DoubleVar=7;
		//tempdouble[10]->DoubleVar=7;

		for (i = 0; i<smallSize - 1; i++)
		{	//for(j=1;j<10;j++)
			double p0 = tempdouble[i]->DoubleVar;
			if (p0>0)
			{
				int pos = 0;
				do
				{
					pos++;

				} while ((tempdouble[i + pos]->DoubleVar>0) && (i + pos<smallSize - 1));
				//while 	((fabs(tempdouble[i+pos]->DoubleVar-p0)<tol)&&(i+pos<smallSize-1));

				if (pos>1)
					for (j = i; j<i + pos + 1; j++)
					{
						tempdouble[j]->DoubleVar = 0;
					}

				tempdouble[i + Round(pos / 2)]->DoubleVar = p0;
				i = i + pos + 1;

			}
		}


		QSortIntDouble(tempdouble, 0, smallSize - 1);
		//czesc=fopen("c:\\czescX.dat","w");for(i=0;i<smallSize;i++)fprintf(czesc,"%d %lf\n",tempdouble[i]->IntVar,(tempdouble[i]->DoubleVar));fclose(czesc);

		//----------------------------------------------

		//-----------------------------------------------------
		int gaussNr;
		for (i = 0; i<smallSize; i++)
		{
			if (tempdouble[smallSize - i - 1]->DoubleVar<10e-4)
			{
				gaussNr = i;
				break;
			}

		}

		//int gaussNr=3;

		//double *gaussParams=new double[gaussNr*3+2+1];


		size = smallSize;
		int Hsize = smallSize;
		int Lsize = gaussNr * 3 + 3;
		nrOfParams = Lsize;

		tHistTab = smallHistTab;



		double	*y = new double[Hsize];
		double	*x = new double[Hsize];
		double	*sig = new double[Hsize];
		int		*ia = new int[Lsize];
		double	*a = new double[Lsize];

		for (i = 1; i<Hsize; i++)
		{
			sig[i] = 1;		x[i] = i;		y[i] = tHistTab[i - 1];
		}

		for (i = 1; i<Lsize; i++)
			ia[i] = 1;

		a[1] = 0.0;
		a[2] = 0;

		double sd = ((double)(size - 2) / (double)(gaussNr - 1));
		for (i = 0; i<gaussNr; i++)
		{
			a[3 * i + 4] = tempdouble[smallSize - i - 1]->IntVar;		//pozycja
			a[3 * i + 3] = ((rand()) / (double)RAND_MAX)* tHistTab[Round(a[3 * i + 4])];
			//	a[3*i+3]=sqrt(tHistTab[Round(a[3*i+4])]/(gaussNr/2.0)+1.0);//maxVal/2.0;		//amplituda
			//  a[3*i+3]=sqrt(tHistTab[Round(a[3*i+4])]/tempdouble[smallSize-i-1]->DoubleVar+1);
			a[3 * i + 5] = 10;
			//	if (tHistTab[Round(a[3*i+4])]/maxVal>0.9)
			//		a[3*i+3]=tHistTab[Round(a[3*i+4])]/(sqrt((double)gaussNr));			
		}

		//a[3*0+3]/=1.2;
		//a[3*1+3]=0.1;
		//a[3*2+3]=7;
		//a[3*2+3]+=20;

		//a[3*2+3]*=2;
		//----------------------------------------------

		//czesc=fopen("c:\\czesc6.dat","w");for(i=1;i<=size;i++)fprintf(czesc,"%d %lf\n",i-1,(GetFunctionValue(i,a)));fclose(czesc);

		//int	  tHistPos;
		//int nrOfParams;

		//czesc=fopen("c:\\czesc5.dat","w");for(i=1;i<Lsize;i++)fprintf(czesc,"%d %lf\n",i,(a[i]));fclose(czesc);

		OptimizationMarquardt *optim = new OptimizationMarquardt(x, y, sig, Hsize, a, ia, Lsize, this);


		double tolx = 10e-4;

		ia[1] = 0; ia[2] = 0;
		for (i = 0; i<gaussNr * 2; i++)
			optim->StartOptimization(tolx);
		//		ia[1]=1;ia[2]=1;
		//		optim->StartOptimization(tolx);

		//ia[1]=1;ia[2]=1;

		//czesc=fopen("c:\\czesc5.dat","a");fprintf(czesc,"\n");for(i=1;i<Lsize;i++)fprintf(czesc,"%d %lf\n",i-1,(a[i]));fclose(czesc);

		//czesc=fopen("c:\\czesc7.dat","w");for(i=1;i<=size;i++)fprintf(czesc,"%d %lf\n",i-1,(GetFunctionValue(i,a)));fclose(czesc);





		// na momencik wrzuce tu normalny histogram
		double *newTab = new double[size];
		memset(newTab, 0, sizeof(int)*size);
		for (i = 0; i<LastHistValue + 1; i++)
		{
			pp = Round(i*factor);
			//if (tHistTab[i]>smallHistTab[pp]) smallHistTab[pp]=tHistTab[i];
			if (pp<256)
			{
				if (newTab[pp]>0)
					newTab[pp] = (newTab[pp] + logHistTab[i]) / 2;
				else
					newTab[pp] = logHistTab[i];
			}
		}

		//zapis oryginalnego histogramu do pliku
		//czesc=fopen("c:\\czesc0.dat","w");for(i=0;i<size-1;i++)fprintf(czesc,"%d %lf\n",i,newTab[i]);fclose(czesc);
		delete[]newTab;

		//zapis poszczegolnych gaussow do pliku
		QString txt;
		for (j = 0; j<gaussNr; j++)
		{
			txt = "c:\\gauss" + QString::number(j) + ".dat";
			//czesc=fopen(txt.toLocal8Bit(),"w");for(i=1;i<=size;i++)fprintf(czesc,"%d %lf\n",i-1,(GetSingleGaussianValue(i,a,j)));fclose(czesc);


		}
		//zapis do pliku punktow odpowiadajacych sigmie dla danego gaussa

		double sigma = 1.0;
		int *pointtab = new int[gaussNr * 2];
		double po, amp, szer;
		int pi;

		//czesc=fopen("c:\\piki.dat","w");
		pos = 0;
		for (j = 0; j<gaussNr; j++)
		{
			po = a[3 * j + 4];
			amp = a[3 * j + 3];
			szer = a[3 * j + 5];

			pi = Round(po - 0.5*sigma*szer);
			if ((pi >= 0) && (pi<256))
			{
				//					fprintf(czesc,"%d %lf\n",pi,GetSingleGaussianValue(pi+1,a,j));
				pointtab[pos++] = pi;
			}

			pi = Round(po + 0.5*sigma*szer);
			if ((pi >= 0) && (pi<256))
			{
				//			fprintf(czesc,"%d %lf\n",pi,GetSingleGaussianValue(pi+1,a,j));
				pointtab[pos++] = pi;
			}

		}
		//	fclose(czesc);
		QSortInt(pointtab, 0, pos - 1);

		/*//czesc=fopen("c:\\piki.dat","w");
		for (j=0;j<gaussNr;j++)
		{
		if ((pointtab[j]>=0)&&(pointtab[j]<256))
		fprintf(czesc,"%d %lf\n",pointtab[j],GetFunctionValue(pointtab[j],a));

		}
		fclose(czesc);*/

		CreateNewPalette(pointtab, pos);

		delete[]pointtab;
		delete optim;
		delete[]a;
		delete[]ia;
		delete[]sig;
		delete[]x;
		delete[]y;
		tHistTab = NULL;
		delete[]FilterTab;
		delete[]logHistTab;
		delete[]logAgerageTab;
		//	delete []logMedianTab;

	}




	/*
	int *TempHistArray=new int[Range];
	int *FilteredHist=new int[Range];

	int FilterSize;
	//		int NNR=100;


	if (Range>100)
	FilterSize=Round(Range/100.0);
	else
	FilterSize=1;
	int TempCounter;

	int *FilterTab=new int[FilterSize*2+1];










	//mooving average


	int tmp2,tmpmax,ipj;
	int nr=Round((double)FilterSize/2.0)+1;

	for (k=0;k<1;k++)
	for (i=0;i<Range;i++)
	{
	tmp2=0;
	tmpmax=0;
	TempCounter=0;
	for (j=-nr;j<=nr;j++)
	{
	ipj=i+j;
	if((ipj >=0)&&(ipj<Range))
	{
	TempCounter++;
	tmp2+=HistArray[ipj];

	}
	}

	FilteredHist[i]=Round((double)tmp2/(double)(TempCounter));
	}


	// mendian filtering

	for (k=0;k<2;k++)
	for(i=0;i<Range;i++)
	{
	TempCounter=0;
	for (j=-FilterSize;j<=FilterSize;j++)
	if (((i+j)>=0)&&((i+j)<Range))
	FilterTab[TempCounter++]=FilteredHist[i+j];

	TempHistArray[i]=Median (FilterTab,TempCounter-1);

	}

	//----------------skala logarytmiczna
	int *TempHistArray=new int[Range];
	int *FilteredHist=new int[Range];

	int bb;
	for (i=0;i<ScatterSize*ScatterSize;i++)
	{
	bb=ScatterTab[i];
	if (bb>0)
	ScatterTab2[i]=log((double)bb);
	else
	ScatterTab2[i]=0;

	if (ScatterTab2[i]>Max) Max=ScatterTab2[i];
	}

	//if (Max==0.0) Max=1.0;
	double ScaleFactor=(double)(255)/(double)Max;






	//czesc=fopen("c:\\czesc1.dat","w");for(i=0;i<Range;i++)fprintf(czesc,"%d %lf\n",i,(HistArray2[i]));fclose(czesc);
	//czesc=fopen("c:\\czesc2.dat","w");for(i=0;i<Range;i++)fprintf(czesc,"%d %lf\n",i,(FilteredHist2[i]));fclose(czesc);
	//czesc=fopen("c:\\czesc3.dat","w");for(i=0;i<Range;i++)fprintf(czesc,"%d %lf\n",i,(TempHistArray2[i]));fclose(czesc);
	//------------------------------------

	//Pick searching
	int *PickPos=new int[200];

	int NumberOfPicks=-1;
	//		int tmp=0;
	int left=0;
	int right=0;

	for(i=1;i<Range-1;i++)
	{
	if(TempHistArray[i]>TempHistArray[i-1])
	left=i;
	else
	if((TempHistArray[i]>TempHistArray[i+1])&&(left!=0))
	right=i;

	if((left!=0)&&(right!=0))
	{
	NumberOfPicks++;
	PickPos[NumberOfPicks]=left+Round((double)(right-left)/2.0);

	//czesc=fopen("c:\\czescX.dat","a");
	fprintf(czesc,"Nr:%d Pos:%d Left=%d Right=%d\n",NumberOfPicks,PickPos[NumberOfPicks],left,right);
	fclose(czesc);
	left=0;
	right=0;


	}


	}
	}
	*/

}


//----------------------------------------------------------------

//----------------------------------------------------------------

void HistWidget::SlotSetThresholdMode()
{

	Mode = 0;
	//TransferFunctionBox->hide();
	//ThresholdsBox->show();
	IWidget->SetTransferFunctionModeOff();

	RedrawHistogram(ScaleSlider->value());

	emit SignalOpacityModeChanged();

}

//----------------------------------------------------------------

void HistWidget::SlotSetTransferFunctionMode()
{

	Mode = 1;
	//	ThresholdsBox->hide();
	//	TransferFunctionBox->show();
	IWidget->SetTransferFunctionModeOn();

	RedrawHistogram(ScaleSlider->value());

	emit SignalOpacityModeChanged();

}

//----------------------------------------------------------------

void HistWidget::SlotSetFullPalette()
{
	int i;
	for (i = 0; i <= 255; i++)
	{
		PalTemp->R[i] = MainDataSet->GetPalette()->R[i];
		PalTemp->G[i] = MainDataSet->GetPalette()->G[i];
		PalTemp->B[i] = MainDataSet->GetPalette()->B[i];
	}
}

//----------------------------------------------------------------

int HistWidget::GetLowerThreshold()
{
	return LowerThreshold;
}
//----------------------------------------------------------------

int HistWidget::GetUpperThreshold()
{
	return UpperThreshold;
}

//----------------------------------------------------------------

double HistWidget::GetLeftY()
{
	return LeftY;
}
//----------------------------------------------------------------

double HistWidget::GetRightY()
{
	return RightY;
}

//----------------------------------------------------------------

void HistWidget::SetLowerThreshold(int value)
{
	LowerThreshold = value;
}
//----------------------------------------------------------------

void HistWidget::SetUpperThreshold(int value)
{
	UpperThreshold = value;
}
//----------------------------------------------------------------

void HistWidget::SetLeftY(double value)
{
	LeftY = value;
}
//----------------------------------------------------------------

void HistWidget::SetRightY(double value)
{
	RightY = value;
}
//----------------------------------------------------------------

ImageWidget *HistWidget::GetImageWidget()
{
	return IWidget;
}

//----------------------------------------------------------------

bool HistWidget::GetStretchPaletteMode()
{
	return StretchPalette;
}
//----------------------------------------------------------------

int HistWidget::GetMaxValue()
{
	return LastHistValue;
}
//----------------------------------------------------------------

int HistWidget::GetMinValue()
{
	return FirstHistValue;
}
//----------------------------------------------------------------
double HistWidget::GetFunctionValue(double pos, double *a)
{
	int i;
	double fac, ex, arg;
	double y = 0.0;
	tHistPos = Round(pos - 1);
	double x = tHistPos;
	for (i = 1; i <= nrOfParams - 3; i += 3)
	{
		arg = (x - a[i + 3]) / a[i + 4];
		ex = exp(-arg*arg);
		y += a[i + 2] * a[i + 2] * ex;
		//	y+=a[i+2]*ex;
	}

	y += a[1] * x + a[2];

	return y;


}
//----------------------------------
double HistWidget::GetSingleGaussianValue(double pos, double *a, double nr)
{
	int i;
	double fac, ex, arg;
	double y = 0.0;
	tHistPos = Round(pos - 1);
	double x = tHistPos;

	for (i = nr * 3 + 1; i <= nr * 3 + 1; i += 3)
	{
		arg = (x - a[i + 3]) / a[i + 4];
		ex = exp(-arg*arg);
		y += a[i + 2] * a[i + 2] * ex;
		//	y+=a[i+2]*ex;
	}

	//y += a[1]*x+a[2];

	return y;
}

void HistWidget::CalculateDerivative(double *dyda, double *a)
{

	int i;
	double fac, ex, arg;

	double x = tHistPos;
	for (i = 1; i <= nrOfParams - 3; i += 3)
	{
		arg = (x - a[i + 3]) / a[i + 4];
		ex = exp(-arg*arg);
		fac = a[i + 2] * a[i + 2] * ex*2.0*arg;
		dyda[i + 2] = 2 * a[i + 2] * ex;
		//fac=a[i+2]*ex*2.0*arg;
		//dyda[i+2]=ex;

		dyda[i + 3] = fac / a[i + 4];
		dyda[i + 4] = fac*arg / a[i + 4];
	}

	dyda[1] = x;
	dyda[2] = 1.0;



}
void HistWidget::PrecalculateVariables(double *Params)
{

}
//-------------------------
void HistWidget::CreateNewPalette(int *postab, int size)
{


	srand((unsigned)time(NULL));
	PaletteRGBA pal;

	int i, j;
	unsigned char r1, g1, b1;
	unsigned char r2, g2, b2;

	r1 = 0;
	g1 = 0;
	b1 = 0;
	int pos = 0;
	double stepR, stepG, stepB;
	double fct;
	int range;
	//GetRandomCol(r1,g1,b1);
	for (i = 0; i <= size; i++)
	{
		if (postab[i] - pos>10)
			GetRandomCol(r2, g2, b2);
		else
		{
			r2 = Round(((double)(rand()) / (double)RAND_MAX) * 10) + r1;
			g2 = Round(((double)(rand()) / (double)RAND_MAX) * 10) + g1;
			b2 = Round(((double)(rand()) / (double)RAND_MAX) * 10) + b1;
			if (r2>255) r2 = 255; if (g2>255) g2 = 255; if (b2>255) b2 = 255;

		}
		if ((postab[i] >= 0) && (postab[i]<256) && (postab[i] != pos))
		{
			range = postab[i] - pos;
			fct = 1.0 / range;
			stepR = (r2 - r1)*fct;
			stepG = (g2 - g1)*fct;
			stepB = (b2 - b1)*fct;
			for (j = 0; j<range; j++)
			{
				pal.R[pos + j] = r1 + Round(stepR*j);
				pal.G[pos + j] = g1 + Round(stepG*j);
				pal.B[pos + j] = b1 + Round(stepB*j);
			}
			pos = postab[i];
			r1 = r2; g1 = g2; b1 = b2;
		}
	}

	GetRandomCol(r2, g2, b2);
	range = 255 - pos;
	fct = 1.0 / range;
	stepR = (r2 - r1)*fct;
	stepG = (g2 - g1)*fct;
	stepB = (b2 - b1)*fct;
	for (j = 0; j<range; j++)
	{
		pal.R[pos + j] = r1 + Round(stepR*j);
		pal.G[pos + j] = g1 + Round(stepG*j);
		pal.B[pos + j] = b1 + Round(stepB*j);
	}



	/*for ( i=0;i<255;i++)
	{
	pal.R[pos+i]=0;
	pal.G[pos+i]=0;
	pal.B[pos+i]=0;
	}*/
	FILE *czesc;
	//czesc=fopen("c:\\r.dat","w");for(i=0;i<255;i++)fprintf(czesc,"%d %d\n",i,pal.R[i]);fclose(czesc);
	//czesc=fopen("c:\\g.dat","w");for(i=0;i<255;i++)fprintf(czesc,"%d %d\n",i,pal.G[i]);fclose(czesc);
	//czesc=fopen("c:\\b.dat","w");for(i=0;i<255;i++)fprintf(czesc,"%d %d\n",i,pal.B[i]);fclose(czesc);

	emit SignalSendNewPalette(pal);



}
//-------------------------
void HistWidget::GetRandomCol(unsigned char &r, unsigned char &g, unsigned char &b)
{
	r = Round(((rand()) / (double)RAND_MAX) * 255);
	g = Round(((rand()) / (double)RAND_MAX) * 255);
	b = Round(((rand()) / (double)RAND_MAX) * 255);


}
//---------------------------
void	HistWidget::SlotSendPointArray(QPolygon a, double x, double y)
{

	double scale = (XHSize > 0) ? 1.0 / XHSize : 1;
	QPolygonF poly;
	for (int i = 0; i < a.size(); i++)
	{
		double x, y;
		x = a.at(i).x()*scale;

		y = 1.0 - a.at(i).y() / 200.0;
		poly.append(QPointF(x, y));
	}

	emit SignalSendOpacityFunctionPointArray(poly, Round(x));
}
//----------------------------
void HistWidget::SetBackgroundColor(QColor col, bool hideBoxSLider)
{
	this->backgroundColor = col;
	RedrawHistogram(ScaleSlider->value());
}

//-----------------------------------------------
void HistWidget::SlotShowThresholdsMode(bool mode)
{
	GridColTh1->setVisible(!mode);
	GridColTh2->setVisible(mode);

}


void	HistWidget::SlotSetCurrentHistogram(RawDataSet *origData, int *histTab, int size)
{
	if (data == NULL) return;
	if (data == origData)
	{
		if (size != XHSize)
		{
			data = data;
		}
		memset(Bins, 0, XHSize * sizeof(int));
		memcpy(Bins, histTab, size * sizeof(int));

		int i;
		//find binsmax


		if (histGraphMode == 1)
		{
			for (i = 0; i<XHSize; i++)
			{
				Bins[i] = log(1 + (double)(Bins[i])) * 100;

			}

		}


		this->BinsMax = 0;
		for (i = 0; i<size; i++)
		{
			if (Bins[i]>BinsMax)	BinsMax = Bins[i];
		}

		//------------------------


		RedrawHistogram(this->ScaleSlider->value());
	}

}