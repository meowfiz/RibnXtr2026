#include	"GUIPanelWidget.h"
//Added by qt3to4:
#include <QLabel>
#include <QPixmap>
//#include <QVBoxLayout>
//#include <QMenu>

//Just  buttons & some more
GUIPanelWidget::GUIPanelWidget(unsigned char * _PalTab, int _PalNr, QList <QString> *names, QList<QPushButton2*>* _buttonList, QList<QCheckBox2*>* _checkBoxList, QList<QComboBox2*>* _comboBoxList,
	QList<QLineEdit2*>* _lineEditList, QList<QSlider2*>* _sliderList, QList<QSpinBox2*>* _spinBoxList,QWidget *parent,  double iconFactor, QString name) :QObject(parent)
{

	buttonList = _buttonList;
	checkBoxList = _checkBoxList;
	comboBoxList = _comboBoxList;
	lineEditList = _lineEditList;
	sliderList = _sliderList;
	spinBoxList = _spinBoxList;
	//qhBoxLayout=new QHBoxLayout;
	//qvBoxLayout=new QVBoxLayout;
	iconScaleFactor = iconFactor;
	this->paletteNames = names;

	imgMatchOptName[0] = "grid  X";
	imgMatchOptName[1] = "grid  Y";
	imgMatchOptName[2] = "angle range";
	imgMatchOptName[3] = "angle samples";
	imgMatchOptName[4] = "first Sampling X";
	imgMatchOptName[5] = "first Sampling Y";

	imgMatchOptName[6] = "number of Bins";
	imgMatchOptName[7] = "number of Threads";
	imgMatchOptName[8] = "% of rejection";

	for (int i = 0; i < NUMBER_OF_REG_SLIDERS; i++)
	{

		imgMatchOptRange[i * 2] = 1;
		imgMatchOptRange[i * 2 + 1] = 100;
	}



	imgMatchOptRange[2 * 2] = 0;//min angle 0
	imgMatchOptRange[2 * 2 + 1] = 90;//max angle 90

	imgMatchOptRange[2 * 6] = 2;//min nr of Bins 2^2
	imgMatchOptRange[2 * 6 + 1] = 8;//min nr of Bins 2^8

	imgMatchOptRange[2 * 8] = 1;//only one is accepted
	imgMatchOptRange[2 * 8 + 1] = 200;//what is above 200% is rejected


	rDataCounter = 0;
	mDataCounter = 0;
	GenerateDistinctColors();

	UpdateListBlocked=false;
	LastSurfaceItemDeleted=false;
	//this->setMinimumWidth(220);


	PalTab=_PalTab;
	PalNr=_PalNr;
	MaxToleranceValue=0;

	//QWidget *widget=new QWidget();
	//this->setWidget(widget);
	//qtab=new MyDockWidget(0);

	/*QVBoxLayout* layout1= new QVBoxLayout( widget);	
	qtab=new MyDockWidget(widget);


	VolumeDataTab=new MyDockWidget(qtab);
	SurfaceDataTab=new MyDockWidget(qtab);
	ArteryDataTab=new MyDockWidget(qtab);

	qtab->addTab(VolumeDataTab,"&Volume");
	qtab->addTab(SurfaceDataTab,"&Surface");
	qtab->addTab(ArteryDataTab,"&Artery");


*/
	TParams=new double[50]; 
//	layout1->addWidget(qtab);

	//QEvent::NonClientAreaMouseButtonDblClick

	int pos=0;
	int i;
	for(i=0;i< NUMBER_OF_DOCKWIDGETS;i++)
	{
		GUIWidgets[i]=NULL;
	}

	QString style1(" QDockWidget { border: 1px solid transparent; titlebar-close-icon: url(images/blue.png); } QDockWidget::title {     text-align: left;     background-color: #231a98;     padding-left: 35px; }  QDockWidget::close-button, QDockWidget::float-button {     border: 0px solid transparent;     background: transparent;     padding: 0px; }");
	QString style2(" QDockWidget { border: 1px solid transparent; titlebar-close-icon: url(images/green.png); } QDockWidget::title {     text-align: left;    background-color: #1c5105;     padding-left: 35px; }  QDockWidget::close-button, QDockWidget::float-button {     border: 0px solid transparent;     background: transparent;     padding: 0px; }");
	QString style3(" QDockWidget { border: 1px solid transparent; titlebar-close-icon: url(images/red.png); } QDockWidget::title {     text-align: left;      background-color: #812128;     padding-left: 35px; }  QDockWidget::close-button, QDockWidget::float-button {     border: 0px solid transparent;     background: transparent;     padding: 0px; }");
	
	//QString style1(" QDockWidget { border: 0px solid lightgray; titlebar-close-icon: url(blue_on_b3fdb3.png); } QDockWidget::title {     text-align: left;     background: #b3fdb3;     padding-left: 35px; }");
	//QString style2(" QDockWidget { border: 0px solid lightgray; titlebar-close-icon: url(green_on_ffabab.png);} QDockWidget::title {     text-align: left;     background: #ffabab;     padding-left: 35px; }");
	//QString style3(" QDockWidget { border: 0px solid lightgray; titlebar-close-icon: url(red_on_bad7f1.png); } QDockWidget::title {     text-align: left;     background: #bad7f1;     padding-left: 35px; }");
	for(i=0;i<4;i++)
	{
		widgetHistory[i]=NULL;
	}
	GUIWidgets[pos]=new MyDockWidget("Volume Info","Volume Info",false,0);					CreateInfoTab(GUIWidgets[pos]);					GUIWidgets[pos]->setStyleSheet(	style1);	pos++; //0
	GUIWidgets[pos] = new MyDockWidget("DICOM Data", "DICOM Data", 0);				CreateDICOMTab(GUIWidgets[pos]);					GUIWidgets[pos]->setStyleSheet(style1);	pos++; //1

	

	
	GUIWidgets[pos] = new MyDockWidget("Volume Segmentation", "Volume Segmentation", false, parent);		CreateSegmentationTab(GUIWidgets[pos]);			GUIWidgets[pos]->setStyleSheet(style1);	pos++;//2
	GUIWidgets[pos] = new MyDockWidget("Volume Mixer", "Volume Mixer", false, parent);					CreateVolumeMixerTab(GUIWidgets[pos]);			GUIWidgets[pos]->setStyleSheet(style1);	pos++;//3
	GUIWidgets[pos] = new MyDockWidget("Fast Filters", "Fast Filters", true, parent);					CreateFastFiltersTab(GUIWidgets[pos]);			GUIWidgets[pos]->setStyleSheet(style1);	pos++;//4
	GUIWidgets[pos] = new MyDockWidget("Surface Info", "Surface Info", false, parent);				CreateSurfaceTab(GUIWidgets[pos]);				GUIWidgets[pos]->setStyleSheet(style2);	pos++;//5
	GUIWidgets[pos] = new MyDockWidget("Surface Property", "Surface Property", false, parent);				CreateSurfacePropertyTab(GUIWidgets[pos]);		GUIWidgets[pos]->setStyleSheet(style2);	pos++;//6

	
	GUIWidgets[pos] = new MyDockWidget("Surface Processing", "Surface Processing", false, parent);			CreateSurfaceProcessingTab(GUIWidgets[pos]);	GUIWidgets[pos]->setStyleSheet(style2);	pos++;//7
	GUIWidgets[pos] = new MyDockWidget("Volume Vis", "Volume Vis", false, parent);					CreateVolumeVisTab(GUIWidgets[pos]);			GUIWidgets[pos]->setStyleSheet(style1);	pos++;//8
	GUIWidgets[pos] = new MyDockWidget("Artery Info", "Artery Info", false, parent);				CreateArteryGenerationTab(GUIWidgets[pos]);		GUIWidgets[pos]->setStyleSheet(style3);	pos++;//9
	GUIWidgets[pos] = new MyDockWidget("Artery Analyze", "Artery Analyze", false, parent);				CreateArteryAnalyzeTab(GUIWidgets[pos]);		GUIWidgets[pos]->setStyleSheet(style3);	pos++;//10
	GUIWidgets[pos] = new MyDockWidget("Volume Registration", "Volume Registration", false, parent);		CreateRegistrationTab(GUIWidgets[pos]);			GUIWidgets[pos]->setStyleSheet(style1);	pos++;//11
	//GUIWidgets[pos]=new MyDockWidget(QString(pos),"Volume Filtering",parent);			CreateFilteringTab(GUIWidgets[pos]);			GUIWidgets[pos]->setStyleSheet(	style1);	pos++;
	GUIWidgets[pos] = new MyDockWidget("Volume Other Stuff", "Volume Other Stuff", false, parent);			CreatePaletteTab(GUIWidgets[pos]);				GUIWidgets[pos]->setStyleSheet(style1);	pos++;//12
	GUIWidgets[pos] = new MyDockWidget("Surface Registration", "Surface Registration", false, parent);		CreateSurfaceRegistrationTab(GUIWidgets[pos]);	GUIWidgets[pos]->setStyleSheet(style2);	pos++;//13
	GUIWidgets[pos] = new MyDockWidget("Animation", "Animation", false, parent);					CreateSequenceGenerationTab(GUIWidgets[pos]);	GUIWidgets[pos]->setStyleSheet(style2);	pos++;//14

	GUIWidgets[pos] = new MyDockWidget("Logs", "Logs", false, 0);					CreateLogTab(GUIWidgets[pos]);					GUIWidgets[pos]->setStyleSheet(style1);	pos++; //15

	GUIWidgets[pos] = new MyDockWidget("Mask Data", "Mask Data", 0);				CreateMaskTab(GUIWidgets[pos]);					GUIWidgets[pos]->setStyleSheet(style1);	pos++; //16
	GUIWidgets[pos] = new MyDockWidget("Dev Tools", "Dev Tools", false, 0);					CreateDevTab(GUIWidgets[pos]);					GUIWidgets[pos]->setStyleSheet(style1);	pos++; //17

	//for(i=0;i<12;i++)

	/*for(i=0;i<pos;i++)
	{
		GUIWidgets[i]->setFixedWidth(300);
	}*/
	

	SlotCreateFloatingWidgets();

	ScatterSizeCombo->setCurrentIndex(3);

	for(i=0;i< NUMBER_OF_DOCKWIDGETS;i++)
		{
			if (this->GUIWidgets[i]==NULL) break;
			connect(this->GUIWidgets[i],SIGNAL(closeButtonClicked(QString)),this,SLOT(slotChangeDockWidgetVisibility(QString)));
			connect(this->GUIWidgets[i],SIGNAL(dockLocationChanged(Qt::DockWidgetArea,QString )),this,SLOT(slotDockLocationChanged(Qt::DockWidgetArea,QString )));
			//connect(GUIPanel->GUIWidgets[i],SIGNAL(dockLocationChanged (Qt::DockWidgetArea)),GUIPanel,SLOT(slotChangeDockWidgetVisibility(Qt::DockWidgetArea)));
		}

	CreateToolBars();

}
void	GUIPanelWidget::CreateToolBars()
{



	QString txtx[10] = { "I","S","A","P","L","R" ,"User Base","Orientation Cuboid","+","Volume Rendering" };
	cGroup = new QActionGroup(this);
	camToolBar = new QToolBar("CamTools"); camToolBar->setObjectName("camToolBar");
	camToolsMapper = new QSignalMapper(this);

	QLabel *l=new QLabel("Select View: ");
	camToolBar->addWidget(l);
	
	for (int i = 0;i < 10;i++)
	{
		camAction[i] = new QAction(this);
	
		camAction[i]->setCheckable(true);
		camToolsMapper->setMapping(camAction[i], i);
		camToolBar->addAction(camAction[i]);
		if (i == 5)camToolBar->addSeparator();

		if (i <6 ) cGroup->addAction(camAction[i]);

		connect(camAction[i], SIGNAL(triggered()), camToolsMapper, SLOT(map()));
	}
	camToolBar->setIconSize(QSize(20 * iconScaleFactor, 20 * iconScaleFactor));
	QIcon icon;
	QImage buffer, temp;
	QPixmap pix;

	if (buffer.load("images/camerabuttons.png"))
	{

		for (int i = 0;i < 10;i++)
		{

			temp = buffer.copy(i * 20, 0, 20, 20);
			pix = QPixmap::fromImage(temp);
			QIcon icon(pix.scaled(pix.width()* iconScaleFactor,pix.height()*iconScaleFactor));
			camAction[i]->setIcon(icon);


		}
		
		camToolBar->layout()->setMargin(1);

		
		for (int i = 0;i < 10;i++)
			camAction[i]->setToolTip(txtx[i]);
		

	}
	else
	{
		
		for (int i = 0;i < 10;i++)
			camAction[i]->setText(txtx[i]);

	

	}



	((QMainWindow*)(this->parent()))->addToolBar(Qt::TopToolBarArea, camToolBar);



	segToolBar = new QToolBar("SegTools"); segToolBar->setObjectName("SegToolsToolBar");
	segToolBar->setIconSize(QSize(20 * iconScaleFactor, 20 * iconScaleFactor));
//	segToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	
	aGroup=new QActionGroup(this);
	segToolsMapper=new QSignalMapper(this);

	int nr = 12;
	int i;
	



	for(i=0;i<nr;i++)
	{
		segAction[i]=new QAction(this);
		connect(segAction[i],SIGNAL(triggered()),segToolsMapper,SLOT(map()));
		//if (i<nr-3)		
			segToolsMapper->setMapping(segAction[i],i);
		

		segAction[i]->setCheckable(true);
		if (i<nr-6) aGroup->addAction(segAction[i]);

	}
	aGroup = new QActionGroup(this);
	for (i = 0; i<3; i++)
	{
		aGroup->addAction(segAction[i + 6]);
	}
	aGroup = new QActionGroup(this);
	for (i = 0; i<3; i++)
	{
		aGroup->addAction(segAction[i + 9]);
	}
	segAction[11]->setChecked(true);

	segAction[4]->setEnabled(false);
	segAction[5]->setEnabled(false);

	if (buffer.load("images/MaskButtons.png"))
	{

		for( i=0;i<nr;i++)
		{

			temp=buffer.copy(i*20,0,20,20);
			pix=QPixmap::fromImage(temp);
			QIcon icon(pix.scaled(pix.width()* iconScaleFactor, pix.height()* iconScaleFactor));

			//MaskToolButton[i]->setDefaultAction(new QAction(icon,"",MaskButtonGroup));
			segAction[i]->setIcon(icon);


		}
		//segToolBar->setIconSize(QSize(15,15));
		segToolBar->layout()->setMargin(1);
		
		segAction[0]->setToolTip("Point");
		segAction[1]->setToolTip("Rectangle");
		segAction[2]->setToolTip("Ellipse");
		segAction[3]->setToolTip("Lasso");
		segAction[4]->setToolTip("Fill");
		segAction[5]->setToolTip("Magic Wand");
		segAction[6]->setToolTip("Measurements");
		segAction[7]->setToolTip("Segment");
		segAction[8]->setToolTip("Add 3D Shape");
		segAction[9]->setToolTip("Magnifying glass 1");
		segAction[10]->setToolTip("Magnifying glass 2");
		segAction[11]->setToolTip("No Magnifying");

	}
	else
	{
		segAction[0]->setText("Point");
		segAction[1]->setText("Rectangle");
		segAction[2]->setText("Ellipse");
		segAction[3]->setText("Lasso");
		segAction[4]->setText("Fill");
		segAction[5]->setText("Magic Wand");
		segAction[6]->setText("Measurements");
		segAction[7]->setText("Segment");
		segAction[8]->setText("Add 3D Shape");
		segAction[9]->setToolTip("Magnifying glass 1");
		segAction[10]->setToolTip("Magnifying glass 2");
		segAction[11]->setToolTip("No Magnifying");

	}
	
	for(i=0;i<nr;i++)
	{
		segToolBar->addAction(segAction[i]);
		if (i==nr-7)segToolBar->addSeparator();
		if (i == nr - 3)segToolBar->addSeparator();
	}
	
	
	
	MagnifyingGlassSlider = new QSlider2(sliderList,Qt::Horizontal, segToolBar);
	MagnifyingGlassSlider->setFixedWidth(50);
	MagnifyingGlassSlider->setRange(10, 100);
	MagnifyingGlassSlider->setValue(20);
	segToolBar->addWidget(MagnifyingGlassSlider);

	segToolBar->layout()->setSpacing(0);
	segToolBar->layout()->setMargin(0);
	segAction[0]->setChecked(true);

	segAction[6]->setChecked(false);///
	segAction[7]->setChecked(true);


	cropVolumeButton2 = new QPushButton2(buttonList, "Crop&Lines", segToolBar);
	segToolBar->addWidget(cropVolumeButton2);
	segToolBar->addSeparator();
	FillModeCombo = new QComboBox2(comboBoxList,segToolBar);
	FillModeCombo->insertItem(0, "OR   A|B");
	FillModeCombo->insertItem(1, "CLR  A&!B");
	FillModeCombo->insertItem(2, "AND  A&B");
	FillModeCombo->insertItem(3, "NOR  !(A|B)");
	FillModeCombo->insertItem(4, "NAND !(A&B)");
	FillModeCombo->insertItem(5, "XOR  A^B");
	FillModeCombo->insertItem(6, "CLR2 !A&B");
	FillModeCombo->setMinimumWidth(50);
	segToolBar->addSeparator();
	segToolBar->addWidget(FillModeCombo);



	UndoSegmentButton = new QPushButton2(buttonList,"Undo", segToolBar); UndoSegmentButton->setFixedWidth(40);
	RedoSegmentButton = new QPushButton2(buttonList,"Redo", segToolBar); RedoSegmentButton->setFixedWidth(40);
	InvertDuplicateMaskButton = new QPushButton2(buttonList,"Inv", segToolBar); InvertDuplicateMaskButton->setFixedWidth(30);
	
	segToolBar->addWidget(UndoSegmentButton);
	segToolBar->addWidget(RedoSegmentButton);
	segToolBar->addWidget(InvertDuplicateMaskButton);

	OnOffMaskCheckBox = new QCheckBox2(checkBoxList,segToolBar);
	OnOffMaskCheckBox->setText("Show/Hide Barriers");
	OnOffMaskCheckBox->setChecked(true);
	AddBarierPushButton = new QPushButton2(buttonList,"Thickness Mask", segToolBar);
	AddPolygonToMaskPushButton = new QPushButton2(buttonList,"Polygon To Mask", segToolBar);
	AddMaskFromLayerFilterPushButton = new QPushButton2(buttonList,"Filter To Mask", segToolBar);
	ClearMasksPushButton = new QPushButton2(buttonList,"Clr Mask", segToolBar);
	ClearMasks2PushButton = new QPushButton2(buttonList,"Clr Mask2", segToolBar);
	Explode3DPushButton = new QPushButton2(buttonList,"Add LiveSeg Mask", segToolBar);
	ExplodeLiveUpdateCheckBox = new QCheckBox2(checkBoxList,segToolBar);
	ExplodeLiveUpdateCheckBox->setText("Live3D");
	ExplodeLiveUpdateCheckBox->setChecked(true);
	ChangeMaskVisibilitySlider = new QSlider2(sliderList,Qt::Horizontal, segToolBar);
	ChangeMaskVisibilitySlider->setFixedWidth(80);
	ChangeMaskVisibilitySlider->setRange(0, 255);
	ChangeMaskVisibilitySlider->setValue(255);
	QLabel *ll = new QLabel("Visibility:", segToolBar);

	buttonSegToolsActions[0] =segToolBar->addWidget(OnOffMaskCheckBox);
	buttonSegToolsActions[1] = segToolBar->addWidget(ll);
	buttonSegToolsActions[2] = segToolBar->addWidget(ChangeMaskVisibilitySlider);
	buttonSegToolsActions[3] = segToolBar->addWidget(AddBarierPushButton);
	buttonSegToolsActions[4] = segToolBar->addWidget(AddPolygonToMaskPushButton);
	buttonSegToolsActions[5] = segToolBar->addWidget(AddMaskFromLayerFilterPushButton);
	buttonSegToolsActions[6] = segToolBar->addWidget(ClearMasksPushButton);
	buttonSegToolsActions[7] = segToolBar->addWidget(ClearMasks2PushButton);
	buttonSegToolsActions[8] = segToolBar->addWidget(Explode3DPushButton);
	buttonSegToolsActions[9] = segToolBar->addWidget(ExplodeLiveUpdateCheckBox);
	
	





	segToolBar2 = new QToolBar("SegTools"); segToolBar2->setObjectName("SegTools2ToolBar");


	//segAction[nr]->setChecked(true);
	((QMainWindow*)(this->parent()))->addToolBar(Qt::TopToolBarArea, segToolBar);

	((QMainWindow*)(this->parent()))->addToolBar(Qt::TopToolBarArea, segToolBar2);


	nr = 11;



	videoToolBar = new QToolBar("VideoTools"); videoToolBar->setObjectName("VideoToolsToolBar");
	videoToolBar->setIconSize(QSize(20 * iconScaleFactor, 20 * iconScaleFactor));
	videoToolsMapper=new QSignalMapper(this);

	for(i=0;i<nr;i++)
	{
		videoAction[i]=new QAction(this);
		connect(videoAction[i],SIGNAL(triggered()),videoToolsMapper,SLOT(map()));
		if (i<nr-3)		videoToolsMapper->setMapping(videoAction[i],i);
		
	
		videoAction[i]->setCheckable(true);
		//if (i>4) aGroup->addAction(videoAction[i]);

	}
	
	vGroup=new QActionGroup(this);
	for(i=0;i<4;i++)
		vGroup->addAction(videoAction[i]);

	vGroup=new QActionGroup(this);
	for(i=5;i<nr;i++)
		vGroup->addAction(videoAction[i]);
	
	 //videoAction[4]->setEnabled(false);
	//segAction[5]->setEnabled(false);
	QString Rtext[]={"Record","Stop", "Pause", "Play", "Auto Play", "Grab App", "Grab 3D", "Grab Stereo", "Grab Transversal", "Grab Sagittal", "Grab Coronal"};

	if (buffer.load("images/recordbuttons.png"))
	{

		for( i=0;i<nr;i++)
		{

			temp=buffer.copy(i*20,0,20,20);
			pix=QPixmap::fromImage(temp);
			QIcon icon(pix.scaled(pix.width() * iconScaleFactor, pix.height() * iconScaleFactor));

			//MaskToolButton[i]->setDefaultAction(new QAction(icon,"",MaskButtonGroup));
			videoAction[i]->setIcon(icon);

			videoAction[i]->setToolTip(Rtext[i]);
		}
		//segToolBar->setIconSize(QSize(15,15));
		videoToolBar->layout()->setMargin(1);
		

		/*videoAction[0]->setToolTip("Record");
		videoAction[1]->setToolTip("Stop");
		videoAction[2]->setToolTip("Pause");
		videoAction[3]->setToolTip("PLay");
		videoAction[4]->setToolTip("AutoPlay");
		videoAction[5]->setToolTip("Grab App");
		videoAction[6]->setToolTip("Grab 3D");
		videoAction[7]->setToolTip("Grab Stereo");
		videoAction[8]->setToolTip("Grab Transversal");
		videoAction[9]->setToolTip("Grab Sagittal");
		videoAction[10]->setToolTip("Grab Coronal");*/

	}
	else
	{
		for( i=0;i<nr;i++)
		{
			videoAction[i]->setText(Rtext[i]);
		}


	}
	
	for(i=0;i<nr;i++)
	{
		videoToolBar->addAction(videoAction[i]);
		if (i==4)videoToolBar->addSeparator();
	}
	
	
	QLabel* fpsLabel = new QLabel("fps:", videoToolBar);
	videoToolBar->addWidget(fpsLabel);

	fpsComboBox = new QComboBox2(comboBoxList,videoToolBar);
	videoToolBar->addWidget(fpsComboBox);

	int fps[] = { 25,30,50,60,100,120 };

	for (int i = 0;i < 6;i++)
	{
		fpsComboBox->insertItem(i, QString::number(fps[i]));
	}
	videoToolBar->layout()->setSpacing(0);
	videoToolBar->layout()->setMargin(0);
	//segAction[0]->setChecked(true);

	videoAction[6]->setChecked(true);
	

	videoToolBar->addSeparator();



	volumeToolBar = new QToolBar("3D VolumeTools"); volumeToolBar->setObjectName("Volume3DToolBar");
	volumeToolBar->layout()->setSpacing(0);
	volumeToolBar->layout()->setMargin(0);
	volumeToolBar->setIconSize(QSize(20 * iconScaleFactor, 20 * iconScaleFactor));

	QLabel *ll1 = new QLabel("3DProp:", volumeToolBar);



	volumeToolBar->addWidget(ll1);
	//segToolBar->addWidget(ll);
	standardVolOpacityComboBox2 = new QComboBox2(comboBoxList,volumeToolBar);
	volumeToolBar->addWidget(standardVolOpacityComboBox2);



	customButtonsToolBar = new QToolBar("CustomButtonsToolbar"); customButtonsToolBar->setObjectName("CustomButtonsToolbar");
	customButtonsToolBar->layout()->setSpacing(0);
	customButtonsToolBar->layout()->setMargin(0);
	customButtonsToolBar->setIconSize(QSize(20 * iconScaleFactor, 20 * iconScaleFactor));


	ll1 = new QLabel("LastClicked: ", customButtonsToolBar);
	customButtonsToolBar->addWidget(ll1);

	lastclickedButtonNumberLabel = new QLabel("()", customButtonsToolBar);
	customButtonsToolBar->addWidget(lastclickedButtonNumberLabel);

	lastClickButtonQPushButton = new QPushButton(customButtonsToolBar);
	customButtonsToolBar->addWidget(lastClickButtonQPushButton);
	customButtonsToolBar->addSeparator();
	ll1 = new QLabel("Custom: ", customButtonsToolBar);
	customButtonsToolBar->addWidget(ll1);
	for (int i = 0;i < 5;i++)
	{
		customButtons[i] = new QPushButton(customButtonsToolBar);
		customButtonsToolBar->addWidget(customButtons[i]);
	}
	customButtonsToolBar->addSeparator();
	QString t[5] = { "S1","S2","S3","S4","S5" };
	for (int i = 0;i < 5;i++)
	{
		customSetButtons[i] = new QPushButton(customButtonsToolBar);
		customSetButtons[i]->setFixedWidth(20);
		customSetButtons[i]->setText(t[i]);
		customButtonsToolBar->addWidget(customSetButtons[i]);
	}

		//segAction[nr]->setChecked(true);
	((QMainWindow*)(this->parent()))->addToolBar(Qt::TopToolBarArea,videoToolBar);
	((QMainWindow*)(this->parent()))->addToolBar(Qt::TopToolBarArea, volumeToolBar);
	((QMainWindow*)(this->parent()))->addToolBar(Qt::TopToolBarArea, customButtonsToolBar);

	
}
//----------------------------------------------------
void GUIPanelWidget::SlotSetFrameToAnimMissPixelsLabel(int a)
{
	this->frameToAnimMissPixelsLabel->setText("%MissPixels(" + QString::number(a) + "/" + QString::number(this->frameToAnimMissPixelsSlider->maximum()) + ")");
}
//-------------------------------------------------------
void GUIPanelWidget::SlotSetBruteForceSliderLabel(int a)
{
	bruteForceLabel->setText("NrOfSamples(" + QString::number(a) + "/" + QString::number(this->bruteForceSlider->maximum()) + ")");
}
//--------------------
void GUIPanelWidget::SlotSetRangeMatchSlider0(int a)
{
	rangeMatchLabel[0]->setText("minObjSlice(" + QString::number(a) + "/" + QString::number(this->rangeMatchSlider[0]->maximum()) + ")");
}
//-----------------------------------
void GUIPanelWidget::SlotSetRangeMatchSlider1(int a)
{
	rangeMatchLabel[1]->setText("maxObjSlice(" + QString::number(a) + "/" + QString::number(this->rangeMatchSlider[1]->maximum()) + ")");
}
//-----------------------------------------------
void GUIPanelWidget::SlotSetImgMatchOptSlider(int value)
{
	int i;

	QSlider *w = dynamic_cast<QSlider*>(sender());
	for (i = 0; i < NUMBER_OF_REG_SLIDERS; i++)
	{
		if (w == imgMatchOptSlider[i]) break;
	}

	if (i < NUMBER_OF_REG_SLIDERS)
	{
		imgMatchOptLabel[i]->setText(imgMatchOptName[i] + " (" + QString::number(imgMatchOptSlider[i]->value()) + "/" + QString::number(imgMatchOptSlider[i]->maximum()) + ")");


	}

}

//------------------------------------------------------
void	GUIPanelWidget::CreateDICOMTab(MyDockWidget *qtab)
{




	QWidget *Info = new QWidget(qtab);

	qtab->setScrollWidget(Info);
	Info->setLayout(new QVBoxLayout());
	Info->layout()->setSpacing(1);
	Info->layout()->setMargin(1);

	//QSplitter *splitter = new QSplitter(Qt::Vertical, Info);	splitter->setChildrenCollapsible(false);
	//splitter->setLayout(new QVBoxLayout);

	//splitter->setHandleWidth(3);


	//splitter->setCollapsible(false);
	//Info->layout()->addWidget(splitter);
	dicomTabMainWidget= new	QWidget(Info);


	QWidget* Gb1 = dicomTabMainWidget;
	Info->layout()->addWidget(Gb1);
	Gb1->setLayout(new QVBoxLayout);
	Gb1->layout()->setSpacing(1);
	Gb1->layout()->setMargin(1);
	//	Gb1->setTitle("Raw Data Sets");

	QFont font = QFont();
	font.setBold(true);


	QWidget *DICOMBox;
	QWidget *w;
	DICOMBox = new QWidget(Gb1);
	DICOMBox->setLayout(new QHBoxLayout());
	DICOMBox->layout()->setSpacing(1);
	DICOMBox->layout()->setMargin(3);
	Gb1->layout()->addWidget(DICOMBox);
	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	AnalyzeDICOMdirButton = new QPushButton2(buttonList,"Set DICOMdir", w);/*xxx->layout()->addWidget(zzz);*/
	ImportSelectedDICOMButton = new QPushButton2(buttonList,"Import", w);/*xxx->layout()->addWidget(zzz);*/
	w->layout()->addWidget(AnalyzeDICOMdirButton);
	w->layout()->addWidget(ImportSelectedDICOMButton);
	//0----------------------------------	
	DICOMDataTreeWidget = new QTreeWidget(Gb1);
	Gb1->layout()->addWidget(DICOMDataTreeWidget);

	DICOMDataTreeWidget->setColumnCount(3);
	DICOMDataTreeWidget->header()->resizeSection(0, 120);	//	name
	DICOMDataTreeWidget->header()->resizeSection(1, 40);	//	nr
	DICOMDataTreeWidget->header()->resizeSection(2, 64);	//	preview icon
	
	//mixerTreeWidget->header()->setic

	QTreeWidgetItem *headerItem = new QTreeWidgetItem();

	headerItem->setText(0, "Name");
	headerItem->setText(1, "Count");
	headerItem->setText(2, "Preview");
	DICOMDataTreeWidget->setIconSize(QSize(64, 64));

	DICOMDataTreeWidget->setHeaderItem(headerItem);


	DICOMDataTreeWidget->show();
	DICOMDataTreeWidget->setSortingEnabled(false);
	//-----------------------------------------------

	DICOMBox = new QWidget(Gb1);
	DICOMBox->setLayout(new QHBoxLayout());
	DICOMBox->layout()->setSpacing(1);
	DICOMBox->layout()->setMargin(3);
	Gb1->layout()->addWidget(DICOMBox);


	QWidget* ww = new QWidget(DICOMBox);
	ww->setLayout(new QHBoxLayout());
	ww->layout()->setSpacing(1);
	ww->layout()->setMargin(3);
	DICOMBox->layout()->addWidget(ww);


	ImportAllWithErrorsCheckBox = new QCheckBox2(checkBoxList,ww);/*xxx->layout()->addWidget(zzz);*/
	ImportAllWithErrorsCheckBox->setText("Ignore slice distance");
	ImportAllWithErrorsCheckBox->setChecked(FALSE);



	dicomDistTolEdit = new QLineEdit2(lineEditList,ww);

	TextFieldDoubleValidator *vd = new TextFieldDoubleValidator(0, 10000.0, 15, this);
	vd->setNotation(TextFieldDoubleValidator::StandardNotation);
	dicomDistTolEdit->setValidator(vd);
	QLabel *l = new QLabel("Tol: ", ww);

	dicomDistTolEdit->setText("1");
	ww->layout()->addWidget(ImportAllWithErrorsCheckBox);
	ww->layout()->addWidget(l);
	ww->layout()->addWidget(dicomDistTolEdit);

	//	DICOMBox->layout()->addWidget(ImportAllWithErrorsCheckBox);






	DICOMBox = new QWidget(Gb1);
	DICOMBox->setLayout(new QHBoxLayout());
	DICOMBox->layout()->setSpacing(1);
	DICOMBox->layout()->setMargin(3);
	Gb1->layout()->addWidget(DICOMBox);

	InvertDicomPixelRepresentationCheckBox = new QCheckBox2(checkBoxList,DICOMBox);/*xxx->layout()->addWidget(zzz);*/
	InvertDicomPixelRepresentationCheckBox->setText("Invert Pixel Representation");
	InvertDicomPixelRepresentationCheckBox->setChecked(FALSE);
	DICOMBox->layout()->addWidget(InvertDicomPixelRepresentationCheckBox);

	IgnoreWindowLevelValuesCheckBox = new QCheckBox2(checkBoxList,DICOMBox);/*xxx->layout()->addWidget(zzz);*/
	IgnoreWindowLevelValuesCheckBox->setText("Ignore Window/Level ");
	IgnoreWindowLevelValuesCheckBox->setChecked(TRUE);
	DICOMBox->layout()->addWidget(IgnoreWindowLevelValuesCheckBox);



	DICOMBox = new QWidget(Gb1);
	DICOMBox->setLayout(new QHBoxLayout());
	DICOMBox->layout()->setSpacing(1);
	DICOMBox->layout()->setMargin(3);
	Gb1->layout()->addWidget(DICOMBox);


	ShiftValuesToZeroCheckBox = new QCheckBox2(checkBoxList,DICOMBox);/*xxx->layout()->addWidget(zzz);*/
	ShiftValuesToZeroCheckBox->setText("Remove gap");
	ShiftValuesToZeroCheckBox->setChecked(false);
	DICOMBox->layout()->addWidget(ShiftValuesToZeroCheckBox);


	QValidator *ValidatorInt = new QIntValidator(0, 65535, this);



	ShiftValuesLineEdit = new QLineEdit2(lineEditList,DICOMBox);
	DICOMBox->layout()->addWidget(ShiftValuesLineEdit);
	ShiftValuesLineEdit->setText("0");
	ShiftValuesLineEdit->setValidator(ValidatorInt);


}
//------------------------------------------------------
void	GUIPanelWidget::CreateInfoTab(MyDockWidget *qtab)
{
	//QWidget *PaletteTab = new QWidget(qtab);
	//qtab->setScrollWidget(PaletteTab);
	//QLayout* MainLayout = new QVBoxLayout();
	//PaletteTab->setLayout(MainLayout);

		QWidget *Info = new QWidget(qtab);

	qtab->setScrollWidget(Info);
	Info->setLayout(new QVBoxLayout());
	Info->layout()->setSpacing(1);
	Info->layout()->setMargin(1);







	autoLoadLastStateCheckBox = new QCheckBox2(checkBoxList, Info);
	autoLoadLastStateCheckBox->setText("Load on Startup Last Widget State");
	Info->layout()->addWidget(autoLoadLastStateCheckBox);
	autoLoadCheckBoxNr = autoLoadLastStateCheckBox->GetNumber();


	autoRestartAppCheckBox = new QCheckBox2(checkBoxList, Info);
	autoRestartAppCheckBox->setText("Auto Restart App");
	Info->layout()->addWidget(autoRestartAppCheckBox);


	blockFix2DCheckBox = new QCheckBox2(checkBoxList, Info,"blockFix2DCheckBox");
	blockFix2DCheckBox->setText("blockFix2D");
	Info->layout()->addWidget(blockFix2DCheckBox);

	QSplitter *splitter = new QSplitter(Qt::Vertical, Info);	splitter->setChildrenCollapsible(false);
	//splitter->setLayout(new QVBoxLayout);

	splitter->setHandleWidth(3);


	//splitter->setCollapsible(false);
	Info->layout()->addWidget(splitter);


	infoTabMainWidget =new	QWidget(Info);

	QWidget* Gb1 = infoTabMainWidget;//new	QWidget(Info);

	Gb1->setLayout(new QVBoxLayout);
	Gb1->layout()->setSpacing(1);
	Gb1->layout()->setMargin(1);
	//	Gb1->setTitle("Raw Data Sets");

	QFont font = QFont();
	font.setBold(true);






	//0----------------------------------	
	rawDataTreeWidget = new QTreeWidget(Gb1);
	rawDataTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

	Gb1->layout()->addWidget(rawDataTreeWidget);



	rawDataTreeWidget->setColumnCount(6);
	rawDataTreeWidget->header()->resizeSection(0, 32);	//	color
	rawDataTreeWidget->header()->resizeSection(1, 120);	//	name
	rawDataTreeWidget->header()->resizeSection(2, 16);	//	M
	rawDataTreeWidget->header()->resizeSection(3, 16);	//	O
	rawDataTreeWidget->header()->resizeSection(4, 16);	//	I
	rawDataTreeWidget->header()->resizeSection(5, 16);	//	A
	//mixerTreeWidget->header()->setic

	QTreeWidgetItem *headerItem = new QTreeWidgetItem();

	headerItem->setText(0, "C");
	headerItem->setText(1, "Layer Name");
	headerItem->setText(2, "M");
	headerItem->setText(3, "O");
	headerItem->setText(4, "I");
	headerItem->setText(5, "A");

	rawDataTreeWidget->setHeaderItem(headerItem);


	rawDataTreeWidget->show();
	rawDataTreeWidget->setSortingEnabled(false);
	rawDataTreeWidget->setFixedHeight(200);
	//-----------------------------------------------
	QWidget *w;

	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	QLabel *ll = new QLabel(w); ll->setText("DataName:"); w->layout()->addWidget(ll);



	rawDataNameLineEdit = new QLineEdit2(lineEditList,w); w->layout()->addWidget(rawDataNameLineEdit);

	frameMatchingWidget = new	QWidget(Info);
		QWidget * Gb1a = frameMatchingWidget;

	Gb1a->setLayout(new QVBoxLayout);
	Gb1a->layout()->setSpacing(1);
	Gb1a->layout()->setMargin(3);
	//Gb1a->setTitle("Volume Data Info");

	//	QFont font=QFont();
	//font.setBold(true);
	//-------------------------------------------
	//QLabel *L002=new QLabel("Datasets: ",Gb1a);Gb1a->layout()->addWidget(L002);
	//QWidget *w;

	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	InsertItemButton = new QPushButton2(buttonList,w); InsertItemButton->setText("Insert ..."); w->layout()->addWidget(InsertItemButton);
	DeleteItemButton = new QPushButton2(buttonList,w); DeleteItemButton->setText("Delete"); w->layout()->addWidget(DeleteItemButton);
	SaveRawDataSetButton = new QPushButton2(buttonList, w); SaveRawDataSetButton->setText("Save"); w->layout()->addWidget(SaveRawDataSetButton);


	
	//Q3HBox *RawDataSetBox	=	new Q3HBox(Gb1a);Gb1a->layout()->addWidget(RawDataSetBox);


	connect(DeleteItemButton, SIGNAL(clicked()), this, SLOT(SlotDeleteRawDataItem()));
	//	connect(SetNewDataSetButton,SIGNAL(clicked ()),this,SLOT(SlotSetNewDataSet( )));

	//	connect(RawDataListView,SIGNAL(currentChanged ( Q3ListViewItem * )),this,SLOT(SlotCurrentRawDataItemChanged( Q3ListViewItem *)));
	//	connect(RawDataListView,SIGNAL(doubleClicked ( Q3ListViewItem * )),this,SLOT(SlotSetNewDataSetByDoubleClick(Q3ListViewItem *)));


	//connect(rawDataTreeWidget,SIGNAL(itemActivated( QTreeWidgetItem * , int  )),this,SLOT(SlotCurrentRawDataItemChanged( QTreeWidgetItem * , int  )));
	connect(rawDataTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(SlotSetNewDataSetByDoubleClick(QTreeWidgetItem *, int)));



	//Helper=false;




	paletteAndVisWidget = new QWidget(Info);
	Info->layout()->addWidget(paletteAndVisWidget);
	paletteAndVisWidget->setLayout(new QVBoxLayout);
	paletteAndVisWidget->layout()->setSpacing(0);
	paletteAndVisWidget->layout()->setMargin(0);

	//QWidget *
	Gb1PP = new	QWidget(paletteAndVisWidget);
	//Info->layout()->addWidget(Gb1PP);
	splitter->addWidget(Gb1PP);
	Gb1PP->setLayout(new QVBoxLayout);
	Gb1PP->layout()->setSpacing(1);
	Gb1PP->layout()->setMargin(3);
	//Gb1PP->setTitle("Palettes");

	QWidget *PaletteBox = new QWidget(Gb1PP);
	PaletteBox->setLayout(new QHBoxLayout());
	Gb1PP->layout()->addWidget(PaletteBox);
	PaletteBox->layout()->setSpacing(0);
	PaletteBox->layout()->setMargin(0);
	currentPaletteName = new QLabel(Gb1PP);
	Gb1PP->layout()->addWidget(currentPaletteName);

	QLabel *L1p = new QLabel("2D View Palette:", PaletteBox);
	L1p->setFont(font);
	PrevPaletteButton = new QPushButton2(buttonList,"<-", PaletteBox);   
	NextPaletteButton = new QPushButton2(buttonList,"->", PaletteBox);
	InvertPaletteButton = new QPushButton2(buttonList,"Inv", PaletteBox);

	PaletteBox->layout()->addWidget(L1p);
	PaletteBox->layout()->addWidget(PrevPaletteButton);
	PaletteBox->layout()->addWidget(NextPaletteButton);
	PaletteBox->layout()->addWidget(InvertPaletteButton);


	PrevPaletteButton->setFixedWidth(20);
	NextPaletteButton->setFixedWidth(20);
	InvertPaletteButton->setFixedWidth(20);
	connect(PrevPaletteButton, SIGNAL(clicked()), this, SLOT(SlotSetPrevPalette()));
	connect(NextPaletteButton, SIGNAL(clicked()), this, SLOT(SlotSetNextPalette()));
	PaletteCombo = new QComboBox2(comboBoxList,Gb1PP);//, "read-only combo" );
	Gb1PP->layout()->addWidget(PaletteCombo);
	connect(PaletteCombo, SIGNAL(activated(int)), this, SIGNAL(SetNewPalette(int)));


	QWidget *PaletteBox2 = new QWidget(Gb1PP);
	PaletteBox2->setLayout(new QHBoxLayout());
	Gb1PP->layout()->addWidget(PaletteBox2);
	PaletteBox2->layout()->setSpacing(0);
	PaletteBox2->layout()->setMargin(0);

	QLabel *L1R = new QLabel("Volume Palette:", PaletteBox2);
	L1R->setFont(font);
	PrevPaletteButtonR = new QPushButton2(buttonList,"<-", PaletteBox2);
	NextPaletteButtonR = new QPushButton2(buttonList,"->", PaletteBox2);
	InvertPaletteButtonR = new QPushButton2(buttonList,"Inv", PaletteBox2);

	PaletteBox2->layout()->addWidget(L1R);
	PaletteBox2->layout()->addWidget(PrevPaletteButtonR);
	PaletteBox2->layout()->addWidget(NextPaletteButtonR);
	PaletteBox2->layout()->addWidget(InvertPaletteButtonR);

	PrevPaletteButtonR->setFixedWidth(20);
	NextPaletteButtonR->setFixedWidth(20);
	InvertPaletteButtonR->setFixedWidth(20);
	connect(PrevPaletteButtonR, SIGNAL(clicked()), this, SLOT(SlotSetPrevPaletteR()));
	connect(NextPaletteButtonR, SIGNAL(clicked()), this, SLOT(SlotSetNextPaletteR()));
	PaletteComboR = new QComboBox2(comboBoxList,Gb1PP);//, "read-only combo" );
	Gb1PP->layout()->addWidget(PaletteComboR);
	connect(PaletteComboR, SIGNAL(activated(int)), this, SIGNAL(SetNewPaletteR(int)));

	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	//Q3HBox *OpacityBox		= new Q3HBox(Gb1PP);
	Gb1PP->layout()->addWidget(w);

	specialSliceCheckBox= new QCheckBox2(checkBoxList,w); specialSliceCheckBox->setText("Special"); w->layout()->addWidget(specialSliceCheckBox);
	specialSliceComboBox = new QComboBox2(comboBoxList,w); w->layout()->addWidget(specialSliceComboBox);

	specialSliceComboBox->insertItem(0, "AVG");
	specialSliceComboBox->insertItem(1, "MIN");
	specialSliceComboBox->insertItem(2, "MAX");
	specialSliceComboBox->insertItem(3, "MED");
	specialSliceComboBox->insertItem(4, "test1");
	specialSliceComboBox->insertItem(5, "test2");
	specialSliceComboBox->insertItem(6, "fitstHit");
	specialSliceComboBox->insertItem(7, "lastHit");
	specialSliceComboBox->setCurrentIndex(2);
	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	QLabel *la = new QLabel("specialOpacity:", w);
	w->layout()->addWidget(la);

	specialSliceOpacitySlider=new QSlider2(sliderList,Qt::Horizontal, w); w->layout()->addWidget(specialSliceOpacitySlider);
	specialSliceOpacitySlider->setRange(0, 255);//w->layout()->addWidget(OpacitySlider);
	specialSliceOpacitySlider->setValue(255);
	Gb1PP->layout()->addWidget(w);


	specialSliceParam1Slider = new QSlider2(sliderList,Qt::Horizontal, Gb1PP); Gb1PP->layout()->addWidget(specialSliceParam1Slider);
	specialSliceParam1Slider->setRange(0, 10000);//w->layout()->addWidget(OpacitySlider);
	specialSliceParam1Slider->setValue(3000);

	specialSliceParam2Slider = new QSlider2(sliderList,Qt::Horizontal, Gb1PP); Gb1PP->layout()->addWidget(specialSliceParam2Slider);
	specialSliceParam2Slider->setRange(0, 10000);//w->layout()->addWidget(OpacitySlider);
	specialSliceParam2Slider->setValue(0);

	specialSliceParam1Slider->hide();
	specialSliceParam2Slider->hide();

	//QLabel *L3R = new QLabel("Opacity:", w);	w->layout()->addWidget(L3R);
	//OpacitySlider = new QSlider2(sliderList,Qt::Horizontal, w);//, "Opacity");	
	//w->layout()->addWidget(OpacitySlider);

	//OpacitySlider->setRange(0, 255);
	//connect(OpacitySlider, SIGNAL(valueChanged(int)), this, SIGNAL(SendOpacity(int)));
	//connect(OpacitySlider, SIGNAL(sliderPressed()), OpacitySlider, SLOT(setFocus())); //!!!!
	//OpacitySlider->setValue(255);





	// double click

	QWidget *w1;
	//Info->layout()->addWidget(Gb1);
	splitter->addWidget(Gb1);

	w = new QWidget(Gb1a); w->setLayout(new QHBoxLayout());	Gb1a->layout()->addWidget(w); w->layout()->setMargin(1);

	frameMatchingPushButton = new QPushButton2(buttonList,w); frameMatchingPushButton->setText("Frame Matching"); w->layout()->addWidget(frameMatchingPushButton);
	frameStabPushButton = new QPushButton2(buttonList,w); frameStabPushButton->setText("Stab"); w->layout()->addWidget(frameStabPushButton);
	w = new QWidget(Gb1a); w->setLayout(new QHBoxLayout());	Gb1a->layout()->addWidget(w); w->layout()->setMargin(1);
	frameMatchingCheckBox = new QCheckBox2(checkBoxList,w); frameMatchingCheckBox->setText("Sort Frames"); w->layout()->addWidget(frameMatchingCheckBox);
	frameMatchingCheckBox->setMaximumWidth(70);

	frameMatchingStopPushButton = new QPushButton2(buttonList,w); frameMatchingStopPushButton->setText("Stop"); w->layout()->addWidget(frameMatchingStopPushButton);
	frameMatchingStopPushButton->setFixedWidth(40);
	w = new QWidget(Gb1a); w->setLayout(new QVBoxLayout());	Gb1a->layout()->addWidget(w); w->layout()->setMargin(1);



	frameStabilizationPushButton = new QPushButton2(buttonList,w); frameStabilizationPushButton->setText("Frame Stabilization"); w->layout()->addWidget(frameStabilizationPushButton);
	frameStabilizationPushButton->hide();
	splitter->addWidget(Gb1a);




	//---------- opcyjki

	/*QCheckBox *frameToAnimMatchCheckBox[5];
	QSlider *bruteForceSlider;
	QLabel *bruteForceLabel;
	QComboBox *frameToAnimMatchFunctionComboBox;
	QComboBox *frameToAnimMatchPrefilterComboBox;*/

	w1 = new QWidget(Gb1a); w1->setLayout(new QHBoxLayout());	Gb1a->layout()->addWidget(w1); w1->layout()->setMargin(1);

	LoadSnapshotSeqPushButton = new QPushButton2(buttonList,"Load Snapshot Seq", w1);
	w1->layout()->addWidget(LoadSnapshotSeqPushButton);

	LoadSnapshotPushButton = new QPushButton2(buttonList,"Load Snapshot", w1);
	w1->layout()->addWidget(LoadSnapshotPushButton);


	QLabel *label;
	w1 = new QWidget(Gb1a); w1->setLayout(new QHBoxLayout());	Gb1a->layout()->addWidget(w1); w1->layout()->setMargin(1);

	exploreMatchingResultsModeBox = new   QComboBox2(comboBoxList,w1);
	w1->layout()->addWidget(exploreMatchingResultsModeBox);
	exploreMatchingResultsModeBox->setFixedWidth(60);

	exploreMatchingResultsPushButton = new QPushButton2(buttonList,"Explore Results", w1);
	w1->layout()->addWidget(exploreMatchingResultsPushButton);
	w1 = new QWidget(Gb1a); w1->setLayout(new QHBoxLayout());	Gb1a->layout()->addWidget(w1); w1->layout()->setMargin(1);

	label = new QLabel("exp-opacity:", w1);
	w1->layout()->addWidget(label);

	exploreMatchingOpacitySlider = new QSlider2(sliderList,Qt::Horizontal, w1);
	exploreMatchingOpacitySlider->setRange(0, 1000);
	w1->layout()->addWidget(exploreMatchingOpacitySlider);


	w1 = new QWidget(Gb1a); w1->setLayout(new QHBoxLayout());	Gb1a->layout()->addWidget(w1); w1->layout()->setMargin(1);




	
	label = new QLabel("tuning:", w1);
	w1->layout()->addWidget(label);
	tunningThicknessFilterSlider2 = new QSlider2(sliderList,Qt::Horizontal, w1);
	tunningThicknessFilterSlider2->setRange(0, 999);
	w1->layout()->addWidget(tunningThicknessFilterSlider2);
	tunningLabel2 = new QLabel("0.000", w1);
	w1->layout()->addWidget(tunningLabel2);
	tunningLabel2->setFixedWidth(30);




	frameToAnimMatchFrame = new QFrame(Gb1a);
	Gb1a->layout()->addWidget(frameToAnimMatchFrame);
	frameToAnimMatchFrame->setFrameStyle(QFrame::StyledPanel);
	frameToAnimMatchFrame->setLayout(new QVBoxLayout());
	frameToAnimMatchFrame->layout()->setMargin(1);


	frameToAnimMatchFrame2 = new QFrame(Gb1a);
	Gb1a->layout()->addWidget(frameToAnimMatchFrame2);
	frameToAnimMatchFrame2->setFrameStyle(QFrame::StyledPanel);
	frameToAnimMatchFrame2->setLayout(new QVBoxLayout());
	frameToAnimMatchFrame2->layout()->setMargin(1);



	QFrame *framex = frameToAnimMatchFrame;
	QFrame *framex2 = frameToAnimMatchFrame2;
	///QWidget *w2 = new QWidget(framex2); w2->setLayout(new QHBoxLayout());	framex2->layout()->addWidget(w2); w2->layout()->setMargin(1);
	w1 = new QWidget(framex2); w1->setLayout(new QHBoxLayout());	framex2->layout()->addWidget(w1); w1->layout()->setMargin(1);

	frameToAnimMatchCheckBox[9] = new QCheckBox2(checkBoxList,w1);
	frameToAnimMatchCheckBox[9]->setText("remove Outliers");
	frameToAnimMatchCheckBox[9]->setChecked(true);
	w1->layout()->addWidget(frameToAnimMatchCheckBox[9]);

	//framex->hide()

	w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);




	frameToAnimMatchCheckBox[0] = new QCheckBox2(checkBoxList,w1);
	frameToAnimMatchCheckBox[0]->setText("pre Scan");
	frameToAnimMatchCheckBox[0]->setChecked(true);
	w1->layout()->addWidget(frameToAnimMatchCheckBox[0]);





	//void	SlotSetImgMatchOptSlider(QWidget*)
	// mapperX1 = new QSignalMapper(this);

	for (int i = 0; i < NUMBER_OF_REG_SLIDERS; i++)
	{
		if (i != 8)
		{
			w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);
		}
		else
		{
			w1 = new QWidget(framex2); w1->setLayout(new QHBoxLayout());	framex2->layout()->addWidget(w1); w1->layout()->setMargin(1);
		}

		imgMatchOptLabel[i] = new QLabel(w1);

		imgMatchOptLabel[i]->setText(imgMatchOptName[i] + " (" + QString::number(imgMatchOptRange[i * 2]) + "/" + QString::number(imgMatchOptRange[i * 2 + 1]) + ")");

		w1->layout()->addWidget(imgMatchOptLabel[i]);
		imgMatchOptSlider[i] = new QSlider2(sliderList,Qt::Horizontal, framex);
		imgMatchOptSlider[i]->setRange(imgMatchOptRange[i * 2], imgMatchOptRange[i * 2 + 1]);
		imgMatchOptSlider[i]->setValue(imgMatchOptRange[i * 2]);

		connect(imgMatchOptSlider[i], SIGNAL(valueChanged(int)), this, SLOT(SlotSetImgMatchOptSlider(int)));
		//connect(imgMatchOptSlider[i], SIGNAL(sliderReleased()), mapperX1, SLOT(map()));
		//mapperX1->setMapping(imgMatchOptSlider[i], imgMatchOptSlider[i]);
		w1->layout()->addWidget(imgMatchOptSlider[i]);
	}
	//connect(mapperX1, SIGNAL(mapped(QWidget *)), this, SLOT(SlotSetImgMatchOptSlider(QWidget *)));

	w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);
	bruteForceLabel = new QLabel(w1);
	bruteForceLabel->setText("NrOfSamples(100/10000)");
	w1->layout()->addWidget(bruteForceLabel);

	bruteForceSlider = new QSlider2(sliderList,Qt::Horizontal, framex);
	bruteForceSlider->setRange(1, 10000);
	bruteForceSlider->setValue(100);
	w1->layout()->addWidget(bruteForceSlider);
	w1->hide();

	w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);
	frameToAnimMissPixelsLabel = new QLabel(w1);
	//frameToAnimMissPixelsLabel->setText("NrOfSamples(100/1000)");
	w1->layout()->addWidget(frameToAnimMissPixelsLabel);
	frameToAnimMissPixelsSlider = new QSlider2(sliderList,Qt::Horizontal, framex);
	frameToAnimMissPixelsSlider->setRange(0, 100);
	frameToAnimMissPixelsSlider->setValue(0);
	w1->layout()->addWidget(frameToAnimMissPixelsSlider);
	SlotSetFrameToAnimMissPixelsLabel(frameToAnimMissPixelsSlider->value());
	w1->hide();




	//QSlider *rangeMatchSlider[2];
	//QLabel *rangeMatchLabel[2];



	connect(bruteForceSlider, SIGNAL(valueChanged(int)), this, SLOT(SlotSetBruteForceSliderLabel(int)));
	connect(frameToAnimMissPixelsSlider, SIGNAL(valueChanged(int)), this, SLOT(SlotSetFrameToAnimMissPixelsLabel(int)));





	w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);


	// phase one
	//QLabel *label;
	QString names[] = { "tx", "ty", "rot" };
	label = new QLabel(w1); label->setText("phase0"); w1->layout()->addWidget(label);
	for (int i = 1; i <= 3; i++)
	{
		frameToAnimMatchCheckBox[i] = new QCheckBox2(checkBoxList,w1); w1->layout()->addWidget(frameToAnimMatchCheckBox[i]);
		frameToAnimMatchCheckBox[i]->setText(names[i-1]);
	}
	w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);
	//phase two
	label = new QLabel(w1); label->setText("phase1"); w1->layout()->addWidget(label);
	for (int i = 4; i <= 6; i++)
	{
		frameToAnimMatchCheckBox[i] = new QCheckBox2(checkBoxList,w1); w1->layout()->addWidget(frameToAnimMatchCheckBox[i]); 
		frameToAnimMatchCheckBox[i]->setText(names[i - 4]);
	}


	/*frameToAnimMatchCheckBox[1] = new QCheckBox2(checkBoxList,w1);
	frameToAnimMatchCheckBox[1]->setText("TransMatch");
	frameToAnimMatchCheckBox[1]->setChecked(true);
	w1->layout()->addWidget(frameToAnimMatchCheckBox[1]);*/


	//phase two

	int pos = 7;
	/*frameToAnimMatchCheckBox[pos] = new QCheckBox2(checkBoxList,w1);
	frameToAnimMatchCheckBox[pos]->setText("TransRotMatch");
	frameToAnimMatchCheckBox[pos]->setChecked(true);
	w1->layout()->addWidget(frameToAnimMatchCheckBox[pos]);
	pos++;*/


	w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);


	frameToAnimMatchCheckBox[pos] = new QCheckBox2(checkBoxList,w1);
	frameToAnimMatchCheckBox[pos]->setText("parallel");
	frameToAnimMatchCheckBox[pos]->setChecked(true);
	w1->layout()->addWidget(frameToAnimMatchCheckBox[pos]);
	pos++;


	frameToAnimMatchCheckBox[pos] = new QCheckBox2(checkBoxList,w1);
	frameToAnimMatchCheckBox[pos]->setText("show");
	frameToAnimMatchCheckBox[pos]->setChecked(true);
	w1->layout()->addWidget(frameToAnimMatchCheckBox[pos]);
	pos++;



	//[0] - preMatch
	//[1],[2],[3] - phase0;
	//[4],[5],[6] - phase1;
	//[7] -parallel
	//[8] - show
	//[9] - outliers



	//QLabel *label;

	w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);
	label = new QLabel(w1); label->setText("RegFunction:"); w1->layout()->addWidget(label);
	frameToAnimMatchFunctionComboBox = new QComboBox2(comboBoxList,w1); w1->layout()->addWidget(frameToAnimMatchFunctionComboBox);

	frameToAnimMatchFunctionComboBox->insertItem(0, "SSD");
	frameToAnimMatchFunctionComboBox->insertItem(1, "CROSS");
	frameToAnimMatchFunctionComboBox->insertItem(2, "Entropy");
	frameToAnimMatchFunctionComboBox->insertItem(3, "MI");
	frameToAnimMatchFunctionComboBox->insertItem(4, "NMI");

	w1 = new QWidget(framex); w1->setLayout(new QHBoxLayout());	framex->layout()->addWidget(w1); w1->layout()->setMargin(1);
	label = new QLabel(w1); label->setText("PreFilter:"); w1->layout()->addWidget(label);
	frameToAnimMatchPrefilterComboBox = new QComboBox2(comboBoxList,w1); w1->layout()->addWidget(frameToAnimMatchPrefilterComboBox);
	//QComboBox *frameToAnimMatchPrefilterComboBox = new QComboBox2(comboBoxList,framex);;
	
	usePrefilterCheckBox = new QCheckBox2(checkBoxList, w1,"usePrefilterCheckBox"); w1->layout()->addWidget(usePrefilterCheckBox);
	usePrefilterCheckBox->setText("fitler!");
	//usePrefilterCheckBox->setChecked(true);

	//-----------------koniec opcyjek
	//	QWidget *w2 = new QWidget(framex2); w2->setLayout(new QHBoxLayout());	framex2->layout()->addWidget(w2); w2->layout()->setMargin(1);


	w1 = new QWidget(framex2); w1->setLayout(new QHBoxLayout());	framex2->layout()->addWidget(w1); w1->layout()->setMargin(1);
	rangeMatchLabel[0] = new QLabel(w1);
	rangeMatchLabel[0]->setText("minObjSlice(0/255)");
	w1->layout()->addWidget(rangeMatchLabel[0]);
	rangeMatchSlider[0] = new QSlider2(sliderList,Qt::Horizontal, framex2);
	rangeMatchSlider[0]->setRange(0, 255);
	rangeMatchSlider[0]->setValue(0);
	w1->layout()->addWidget(rangeMatchSlider[0]);

	w1 = new QWidget(framex2); w1->setLayout(new QHBoxLayout());	framex2->layout()->addWidget(w1); w1->layout()->setMargin(1);
	rangeMatchLabel[1] = new QLabel(w1);
	rangeMatchLabel[1]->setText("maxObjSlice(255/255)");
	w1->layout()->addWidget(rangeMatchLabel[1]);
	rangeMatchSlider[1] = new QSlider2(sliderList,Qt::Horizontal, framex2);
	rangeMatchSlider[1]->setRange(0, 255);
	rangeMatchSlider[1]->setValue(0);
	w1->layout()->addWidget(rangeMatchSlider[1]);





	connect(rangeMatchSlider[0], SIGNAL(valueChanged(int)), this, SLOT(SlotSetRangeMatchSlider0(int)));
	connect(rangeMatchSlider[1], SIGNAL(valueChanged(int)), this, SLOT(SlotSetRangeMatchSlider1(int)));

	
	//w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	autoWindowLevelCheckBox = new QCheckBox2(checkBoxList,Gb1); Gb1->layout()->addWidget(autoWindowLevelCheckBox);
	autoWindowLevelCheckBox->setText("Auto Window Level Settings");
	autoWindowLevelCheckBox->setChecked(true);


	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	//Q3HBox *DatasetNameBox	=	new Q3HBox(Gb1);Gb1->layout()->addWidget(DatasetNameBox);
	QLabel *L001 = new QLabel("Name: ", w); w->layout()->addWidget(L001);
	LabelFileName = new QLabel(w); w->layout()->addWidget(LabelFileName);
	LabelFileName->setMaximumWidth(120);
	QLabel *L1 = new QLabel("Volume Sizes :", Gb1); Gb1->layout()->addWidget(L1);

	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	//Q3HBox *LabelNBox	=	new Q3HBox(Gb1);
	Gb1->layout()->addWidget(w);

	LabelNx = new QLabel("Nx: ---", w); w->layout()->addWidget(LabelNx);
	LabelNy = new QLabel("Ny: ---", w); w->layout()->addWidget(LabelNy);
	LabelNz = new QLabel("Nz: ---", w); w->layout()->addWidget(LabelNz);



	QLabel *L2 = new QLabel("Voxel  Sizes :", Gb1); Gb1->layout()->addWidget(L2);
	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	//Q3HBox *LabelSBox	=	new Q3HBox(Gb1);Gb1->layout()->addWidget(LabelSBox);
	LabelXscale = new QLabel("Sx: ---", w); w->layout()->addWidget(LabelXscale);
	LabelYscale = new QLabel("Sy: ---", w); w->layout()->addWidget(LabelYscale);
	LabelZscale = new QLabel("Sz: ---", w); w->layout()->addWidget(LabelZscale);



	L1->setFont(font);
	L001->setFont(font);
	L2->setFont(font);


	//***************************************



	//paletteAndVisWidget = new QWidget(Info);
	//Info->layout()->addWidget(paletteAndVisWidget);
	//paletteAndVisWidget->setLayout(new QVBoxLayout);
	//paletteAndVisWidget->layout()->setSpacing(0);
	//paletteAndVisWidget->layout()->setMargin(0);

	////QWidget *
	//Gb1PP = new	QWidget(paletteAndVisWidget);
	////Info->layout()->addWidget(Gb1PP);
	//splitter->addWidget(Gb1PP);
	//Gb1PP->setLayout(new QVBoxLayout);
	//Gb1PP->layout()->setSpacing(1);
	//Gb1PP->layout()->setMargin(3);
	////Gb1PP->setTitle("Palettes");

	//QWidget *PaletteBox = new QWidget(Gb1PP);
	//PaletteBox->setLayout(new QHBoxLayout());
	//Gb1PP->layout()->addWidget(PaletteBox);
	//PaletteBox->layout()->setSpacing(0);
	//PaletteBox->layout()->setMargin(0);
	//QLabel *L1p = new QLabel("Model Palettes:", PaletteBox);
	//L1p->setFont(font);
	//PrevPaletteButton = new QPushButton2(buttonList,"<-", PaletteBox);
	//NextPaletteButton = new QPushButton2(buttonList,"->", PaletteBox);
	//InvertPaletteButton = new QPushButton2(buttonList,"Inv", PaletteBox);

	//PaletteBox->layout()->addWidget(L1p);
	//PaletteBox->layout()->addWidget(PrevPaletteButton);
	//PaletteBox->layout()->addWidget(NextPaletteButton);
	//PaletteBox->layout()->addWidget(InvertPaletteButton);


	//PrevPaletteButton->setFixedWidth(20);
	//NextPaletteButton->setFixedWidth(20);
	//InvertPaletteButton->setFixedWidth(20);
	//connect(PrevPaletteButton, SIGNAL(clicked()), this, SLOT(SlotSetPrevPalette()));
	//connect(NextPaletteButton, SIGNAL(clicked()), this, SLOT(SlotSetNextPalette()));
	//PaletteCombo = new QComboBox2(comboBoxList,Gb1PP);//, "read-only combo" );
	//Gb1PP->layout()->addWidget(PaletteCombo);
	//connect(PaletteCombo, SIGNAL(activated(int)), this, SIGNAL(SetNewPalette(int)));


	//QWidget *PaletteBox2 = new QWidget(Gb1PP);
	//PaletteBox2->setLayout(new QHBoxLayout());
	//Gb1PP->layout()->addWidget(PaletteBox2);
	//PaletteBox2->layout()->setSpacing(0);
	//PaletteBox2->layout()->setMargin(0);

	//QLabel *L1R = new QLabel("Selected Palettes:", PaletteBox2);
	//L1R->setFont(font);
	//PrevPaletteButtonR = new QPushButton2(buttonList,"<-", PaletteBox2);
	//NextPaletteButtonR = new QPushButton2(buttonList,"->", PaletteBox2);
	//InvertPaletteButtonR = new QPushButton2(buttonList,"Inv", PaletteBox2);

	//PaletteBox2->layout()->addWidget(L1R);
	//PaletteBox2->layout()->addWidget(PrevPaletteButtonR);
	//PaletteBox2->layout()->addWidget(NextPaletteButtonR);
	//PaletteBox2->layout()->addWidget(InvertPaletteButtonR);

	//PrevPaletteButtonR->setFixedWidth(20);
	//NextPaletteButtonR->setFixedWidth(20);
	//InvertPaletteButtonR->setFixedWidth(20);
	//connect(PrevPaletteButtonR, SIGNAL(clicked()), this, SLOT(SlotSetPrevPaletteR()));
	//connect(NextPaletteButtonR, SIGNAL(clicked()), this, SLOT(SlotSetNextPaletteR()));
	//PaletteComboR = new QComboBox2(comboBoxList,Gb1PP);//, "read-only combo" );
	//Gb1PP->layout()->addWidget(PaletteComboR);
	//connect(PaletteComboR, SIGNAL(activated(int)), this, SIGNAL(SetNewPaletteR(int)));

	//w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	////Q3HBox *OpacityBox		= new Q3HBox(Gb1PP);
	//Gb1PP->layout()->addWidget(w);
	//QLabel *L3R = new QLabel("Opacity:", w);	w->layout()->addWidget(L3R);
	//OpacitySlider = new QSlider2(sliderList,Qt::Horizontal, w);//, "Opacity");	
	//w->layout()->addWidget(OpacitySlider);

	//OpacitySlider->setRange(0, 255);
	//connect(OpacitySlider, SIGNAL(valueChanged(int)), this, SIGNAL(SendOpacity(int)));
	//connect(OpacitySlider, SIGNAL(sliderPressed()), OpacitySlider, SLOT(setFocus())); //!!!!
	//OpacitySlider->setValue(255);

	//
	//	QWidget *InspectionBox		= new QWidget(Gb1PP);
	//	InspectionBox->setLayout(new QHBoxLayout());
	//	InspectionBox->layout()->setSpacing(1);
	//	InspectionBox->layout()->setMargin(3);
	//	Gb1PP->layout()->addWidget(InspectionBox);
	//
	//	InspectionWindowCheckBox=new QCheckBox2(checkBoxList,InspectionBox);InspectionBox->layout()->addWidget(InspectionWindowCheckBox);
	//	InspectionWindowCheckBox->setText("InspWindow");
	//	InspectionWindowCheckBox->setChecked(false);
	//
	//	InspectionCombo = new QComboBox2(comboBoxList, InspectionBox);//, "read-only combo" );
	//	InspectionBox->layout()->addWidget(InspectionCombo);
	//	//InspectionCombo->setMinimumWidth(30);
	//
	//
	//	InspectionCombo->insertItem(0,"BOX");
	//	InspectionCombo->insertItem(1,"ELLIPSE");
	//	InspectionCombo->insertItem(2,"TRIANGLE");
	//	InspectionCombo->insertItem(3,"CIRCLE");
	//
	//	connect(InspectionCombo,SIGNAL(activated(int)),this,SIGNAL(SignalSendNewInspectionMode(int)));
	//
	//	QWidget *Gb3=new	QWidget(Gb1PP);
	//	
	//	Gb1PP->layout()->addWidget(Gb3);
	////	QVBoxLayout *vbl_Gb3 = new QVBoxLayout;
	//	//vbl_Gb3->setSpacing(1);
	//	Gb3->setLayout(new QVBoxLayout());
	//	Gb3->layout()->setSpacing(1);
	//
	//	QLabel *L4R = new QLabel("Section X Size:",Gb3);		
	//	
	//	InspectionXSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3);
	//	QLabel *L5R = new QLabel("Section Y Size:",Gb3);	
	//	InspectionYSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3);
	//	QLabel *L6R = new QLabel("Section Z Size:",Gb3);	
	//	InspectionZSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3);
	//
	//	Gb3->layout()->addWidget(L4R);
	//	Gb3->layout()->addWidget(InspectionXSizeSlider);
	//	Gb3->layout()->addWidget(L5R);
	//	Gb3->layout()->addWidget(InspectionYSizeSlider);
	//	Gb3->layout()->addWidget(L6R);
	//	Gb3->layout()->addWidget(InspectionZSizeSlider);
	//
	//	connect(InspectionXSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));
	//	connect(InspectionYSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));
	//	connect(InspectionZSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));
	//
	//
	//
	//Q
	Gb1PP->layout()->setMargin(2);
	QFrame *frame = new QFrame(Info);
	Info->layout()->addWidget(frame);
	frame->setFrameStyle(QFrame::StyledPanel);
	frame->setLayout(new QVBoxLayout());
	frame->layout()->setMargin(2);
	frame->layout()->setSpacing(2);
	//Gb1PP->layout()->addWidget(frame);
	int ssize = 256;
	visImage = new ImageWidget(frame);
	visImage->SetShapes(&shapes);



	infoImg = new QImage(ssize, ssize, QImage::Format_ARGB32);
	visImage->setValues(infoImg, ssize, ssize, 0, 0);
	visImage->setFixedHeight(ssize);
	visImage->setFixedWidth(ssize);

	memset(infoImg->bits(), 0, 4 * ssize*ssize);

	//visImage->setGeometry(0,0,128,128);



	frame->layout()->addWidget(visImage);
	visImage->show();
	QString aa, tmp;

	//****************************************

	int i, j, k;

	int sx = 256;
	int sy = 12;
	int sxa=220;
	QImage Image(sx, sy, QImage::Format_Indexed8);
	//QPixmap Pixmap;

	for (k = 0; k<PalNr; k++)
	{
		if (k == 70)
		{
			i = i;
		}

		for (i = 0; i<255; i++)
			Image.setColor(i, qRgb(PalTab[k * 768 + i], PalTab[k * 768 + 256 + i], PalTab[k * 768 + 512 + i]));

		for (i = 0; i<sx; i++)
		for (j = 0; j < sy; j++)
			*(Image.scanLine(j) + i) = (unsigned char)(i);// *(255.0 / 100.0));


		QImage tmpa = Image.convertToFormat(QImage::Format_RGB32);
		
		QImage smaller = tmpa.scaled(sxa, 12);

		//Pixmap.fromImage( tmpa);
		QPixmap pp(sxa, sy);
		QPainter paint(&pp);
		paint.drawImage(0, 0, smaller);

		/*if (k==0)
		{
		QString aaa= qApp->applicationDirPath()+"/tttt1.jpg";
		pp.save(aaa);
		}*/
		tmp = "";
		aa.setNum(k);
		tmp.append(aa);
		PaletteCombo->addItem(QIcon(pp), QString::number(k), Qt::ToolTipRole);
		PaletteComboR->addItem(QIcon(pp), QString::number(k), Qt::ToolTipRole);

		

	}
	connect(PaletteCombo, SIGNAL(highlighted(int)), this, SLOT(SlotShowCurrentPaletteName(int)));
	connect(PaletteComboR, SIGNAL(highlighted(int)), this, SLOT(SlotShowCurrentPaletteName(int)));
	connect(PaletteCombo, SIGNAL(activated(int)), this, SLOT(SlotShowCurrentPaletteName(int)));
	connect(PaletteComboR, SIGNAL(activated(int)), this, SLOT(SlotShowCurrentPaletteName(int)));
	PaletteCombo->setIconSize(QSize(sxa, sy));
	PaletteComboR->setIconSize(QSize(sxa, sy));
	//*******************************************
	//Info->layout()->addWidget(Gb1PP);
	//***************************************
	int nrr = splitter->count();

	QSplitterHandle *handle;
	for (int i = 0; i<nrr; i++)
	{
		handle = splitter->handle(i);
		splitter->setHandleWidth(16);
		QVBoxLayout *layout = new QVBoxLayout(handle);
		layout->setSpacing(4);
		layout->setMargin(4);

		QFrame *line = new QFrame(handle);
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		layout->addWidget(line);
	}
}
//----------------------------------------------------



void	GUIPanelWidget::CreateVolumeVisTab(MyDockWidget *qtab)
{
	QWidget *PaletteTab = new QWidget(qtab);
	qtab->setScrollWidget(PaletteTab);
	QLayout* MainLayout = new QVBoxLayout();
	PaletteTab->setLayout(MainLayout);


	//Q3GroupBox *Gb3=new Q3GroupBox(20,Vertical,PaletteTab);
	QGroupBox *Gb3 = new	QGroupBox(PaletteTab);
	//QVBoxLayout *Gb3l = new QVBoxLayout;
	Gb3->setLayout(new QVBoxLayout);

	//QSlider *planeSliders;
	QGroupBox *b;
	//QHBoxLayout *bl=new QHBoxLayout;

	//QWidget *b;
	QLabel *label;
	int i;
	//QHBox *box;//	=	new QWidget(Gb1);

	QGroupBox *Gb3a = new	QGroupBox(PaletteTab);
	//QVBoxLayout *Gb3l = new QVBoxLayout;
	Gb3a->setLayout(new QVBoxLayout);
	MainLayout->addWidget(Gb3a);
	Gb3a->setTitle("Frustum Seg Mode");
	MainLayout->addWidget(Gb3);






	TextFieldDoubleValidator *Validator2 = new TextFieldDoubleValidator(0.0001, 100.0, 4, this);
	Validator2->setNotation(TextFieldDoubleValidator::StandardNotation);
	QWidget *bBox;
	QLabel *l1;


	QGroupBox *gb4 = new QGroupBox(Gb3a);
	Gb3a->layout()->addWidget(gb4);
	gb4->setLayout(new QVBoxLayout);
	gb4->setTitle("3Dseg");

	gb4->layout()->setSpacing(1);
	gb4->layout()->setMargin(2);
	bBox = new QWidget(gb4); bBox->setLayout(new QHBoxLayout());
	gb4->layout()->addWidget(bBox);
	l1 = new QLabel("Acc:", bBox); bBox->layout()->addWidget(l1);
	cutGeometryComboBox = new QComboBox2(comboBoxList,bBox); bBox->layout()->addWidget(cutGeometryComboBox);
	cutGeometryComboBox->addItem("Low");
	cutGeometryComboBox->addItem("Mid");
	cutGeometryComboBox->addItem("High");
	cutGeometryComboBox->setCurrentIndex(1);

	cutModeComboBox = new QComboBox2(comboBoxList,bBox); bBox->layout()->addWidget(cutModeComboBox);
	cutModeComboBox->addItem("Cut All");
	cutModeComboBox->addItem("Cut Depth Abs");
	cutModeComboBox->addItem("Cut Depth Rel");
	cutModeComboBox->addItem("Cut First Hit");
	cutModeComboBox->addItem("Separate Veins");
	cutModeComboBox->setCurrentIndex(0);

	bBox = new QWidget(gb4);  bBox->setLayout(new QHBoxLayout());
	gb4->layout()->addWidget(bBox);
	l1 = new QLabel("Depth:", bBox); bBox->layout()->addWidget(l1);
	depthCutLineEdit = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(depthCutLineEdit);
	depthCutLineEdit->setText("5.0");
	depthCutLineEdit->setValidator(Validator2);


	bBox = new QWidget(gb4); bBox->setLayout(new QHBoxLayout());

	gb4->layout()->addWidget(bBox);
	l1 = new QLabel("Double Click Mode", bBox); bBox->layout()->addWidget(l1);
	singlePointComboBox = new QComboBox2(comboBoxList,bBox); bBox->layout()->addWidget(singlePointComboBox);
	singlePointComboBox->addItem("Remove Island");
	singlePointComboBox->addItem("Leave Island");
	bBox = new QWidget(gb4); 
	gb4->layout()->addWidget(bBox); bBox->setLayout(new QHBoxLayout());
	l1 = new QLabel("Separate:", bBox); bBox->layout()->addWidget(l1);
	separateSurfacesChceckBox = new QCheckBox2(checkBoxList,bBox); bBox->layout()->addWidget(separateSurfacesChceckBox);
	separateSurfacesChceckBox->setChecked(false);








	frustumSegModeComboBox = new QComboBox2(comboBoxList,Gb3a);
	//InspectionCombo->setMinimumWidth(30);


	frustumSegModeComboBox->addItem("Volumetric Cutter", 0);
	frustumSegModeComboBox->addItem("Volumetric Creator", 1);
	Gb3a->layout()->addWidget(frustumSegModeComboBox);
	frustumSegModeComboBox->setCurrentIndex(0);
	QWidget *box;
	QString tt[6] = { "min X", "max X", "min Y", "max Y", "min Z", "max Z" };



	VolumeCropGroupBox = new QGroupBox(PaletteTab);
	VolumeCropGroupBox->setLayout(new QVBoxLayout);
	VolumeCropGroupBox->setTitle("Volume Clip/Crop");
	MainLayout->addWidget(VolumeCropGroupBox);



	resetCropLines = new QPushButton2(buttonList,"ResetCropLines", VolumeCropGroupBox);
	VolumeCropGroupBox->layout()->addWidget(resetCropLines);
	for (i = 0; i<6; i++)
	{
		box = new QWidget(VolumeCropGroupBox); box->setLayout(new QHBoxLayout());
		label = new QLabel(tt[i], box); label->setFixedWidth(30); box->layout()->addWidget(label);
		planeSliders[i] = new QSlider2(sliderList,Qt::Horizontal, box); box->layout()->addWidget(planeSliders[i]);
		planeSliders[i]->setRange(0, 1);

		VolumeCropGroupBox->layout()->addWidget(box);
	}
	box = new QWidget(VolumeCropGroupBox); box->setLayout(new QHBoxLayout());



	QWidget* xWidget;
	QLabel* xLabel;
	showCropLines2DCheckBox = new QCheckBox2(checkBoxList,"Show CropLines", box);
	showCropLines2DCheckBox->setChecked(false);
	box->layout()->addWidget(showCropLines2DCheckBox);

	autoLinesButton = new QPushButton2(buttonList, "Auto Lines", VolumeCropGroupBox);
	box->layout()->addWidget(autoLinesButton);

	cropVolumeButton = new QPushButton2(buttonList,"Crop Data", VolumeCropGroupBox);
	box->layout()->addWidget(cropVolumeButton);
	VolumeCropGroupBox->layout()->addWidget(box);

	xLabel = new QLabel("Point for Volumetric Joint (CTRL+J)", VolumeCropGroupBox);
	box = new QWidget(VolumeCropGroupBox); box->setLayout(new QHBoxLayout());

	VolumeCropGroupBox->layout()->addWidget(xLabel);
	VolumeCropGroupBox->layout()->addWidget(box);

	
	setP1ForJointButton = new QPushButton2(buttonList, "Set Point1", VolumeCropGroupBox);
	box->layout()->addWidget(setP1ForJointButton);
	setP2ForJointButton = new QPushButton2(buttonList, "Set Point2", VolumeCropGroupBox);
	box->layout()->addWidget(setP2ForJointButton);


	//9999999
	

	xWidget = new QWidget(Gb3);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb3->layout()->addWidget(xWidget);
	xLabel = new QLabel("Opacity Function:", xWidget);
	xWidget->layout()->addWidget(xLabel);
	standardVolOpacityComboBox = new QComboBox2(comboBoxList,xWidget);
	xWidget->layout()->addWidget(standardVolOpacityComboBox);


	xWidget = new QWidget(Gb3);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb3->layout()->addWidget(xWidget);

	standardVolOpacityLineEdit = new QLineEdit2(lineEditList,"default", xWidget);
	xWidget->layout()->addWidget(standardVolOpacityLineEdit);
	addStandardVolOpacityButton = new QPushButton2(buttonList,"Add Function", xWidget);
	xWidget->layout()->addWidget(addStandardVolOpacityButton);
	//---------------------------
	xWidget = new QWidget(Gb3);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb3->layout()->addWidget(xWidget);
	/*writeUpdatedVolOpacityButton=new QPushButton2(buttonList,"Write Updated Func",xWidget);
	xWidget->layout()->addWidget(writeUpdatedVolOpacityButton);*/

	//99999999999



	QString tt1[] = { "Ambient", "Diffuse", "Specular", "OpacityUDist", "SampleDist" ,"AmbientMask","DiffuseMask","SpecularMask"};

	label = new QLabel("Blend Mode:", Gb3);
	Gb3->layout()->addWidget(label);
	volumeBlendComboBox = new QComboBox2(comboBoxList,Gb3);
	Gb3->layout()->addWidget(volumeBlendComboBox);

	volumeBlendComboBox->addItem("Composite", 0);
	volumeBlendComboBox->addItem("Maximum", 1);
	volumeBlendComboBox->addItem("Minimum", 2);
	volumeBlendComboBox->addItem("Additive", 3);


	volumeRenCheckBox[0] = new QCheckBox2(checkBoxList,Gb3);
	volumeRenCheckBox[0]->setText("Volume Shading");
	volumeRenCheckBox[0]->setChecked(true);
	Gb3->layout()->addWidget(volumeRenCheckBox[0]);

	volumeRenCheckBox[1] = new QCheckBox2(checkBoxList,Gb3);
	volumeRenCheckBox[1]->setText("Gradient Opacity");
	volumeRenCheckBox[1]->setChecked(false);
	Gb3->layout()->addWidget(volumeRenCheckBox[1]);

	volumeRenCheckBox[2] = new QCheckBox2(checkBoxList,Gb3);
	volumeRenCheckBox[2]->setText("Volume interpolation");
	volumeRenCheckBox[2]->setChecked(true);
	Gb3->layout()->addWidget(volumeRenCheckBox[2]);

	volumeRenCheckBox[3] = new QCheckBox2(checkBoxList,Gb3);
	volumeRenCheckBox[3]->setText("Auto Sample Dist");
	volumeRenCheckBox[3]->setChecked(true);
	Gb3->layout()->addWidget(volumeRenCheckBox[3]);



	volumeRenCheckBox[4] = new QCheckBox2(checkBoxList, Gb3);
	volumeRenCheckBox[4]->setText("Volume Mask Shading");
	volumeRenCheckBox[4]->setChecked(true);
	Gb3->layout()->addWidget(volumeRenCheckBox[4]);

	volumeRenCheckBox[5] = new QCheckBox2(checkBoxList, Gb3);
	volumeRenCheckBox[5]->setText("Gradient Mask Opacity");
	volumeRenCheckBox[5]->setChecked(false);
	Gb3->layout()->addWidget(volumeRenCheckBox[5]);

	

	for (i = 0; i<5+3; i++)
	{
		box = new QWidget(Gb3); box->setLayout(new QHBoxLayout());
		label = new QLabel(tt1[i], box); label->setFixedWidth(60); box->layout()->addWidget(label);
		volumeRenSettingsSliders[i] = new QSlider2(sliderList,Qt::Horizontal, box); box->layout()->addWidget(volumeRenSettingsSliders[i]);
		volumeRenSettingsSliders[i]->setRange(0, 1000);

		Gb3->layout()->addWidget(box);
	}
	volumeRenSettingsSliders[0]->setValue(400);
	volumeRenSettingsSliders[1]->setValue(600);
	volumeRenSettingsSliders[2]->setValue(200);
	volumeRenSettingsSliders[3]->setValue(1000);
	volumeRenSettingsSliders[4]->setValue(200);
	volumeRenSettingsSliders[5]->setValue(400);
	volumeRenSettingsSliders[6]->setValue(600);
	volumeRenSettingsSliders[7]->setValue(200);

	planeSliders[1]->setValue(1);
	planeSliders[3]->setValue(1);
	planeSliders[5]->setValue(1);
	//b=new QWidget(Gb3);	
	//b=new QGroupBox(Gb3); b->setLayout(new QHBoxLayout);
	
	//QSlider *volumeRenSettingsSliders[6];
	//QCheckBox *volumeRenCheckBox[2];


	//Q3GroupBox *Gb1=new Q3GroupBox(20,Vertical,PaletteTab);

	QGroupBox *gr = new	QGroupBox(PaletteTab);
	Gb3->layout()->addWidget(gr);
	gr->setLayout(new QVBoxLayout());
	gr->layout()->setSpacing(1);
	gr->layout()->setMargin(1);

	//QButtonGroup *polyLineButtonGroup;


	QRadioButton *radio0 = new QRadioButton("Segmentation", gr); gr->layout()->addWidget(radio0);
	QRadioButton *radio1 = new QRadioButton("Create Slice", gr); gr->layout()->addWidget(radio1);
	QRadioButton *radio2 = new QRadioButton("Mixer mode", gr); gr->layout()->addWidget(radio2);


	polyLineButtonGroup = new QButtonGroup();
	polyLineButtonGroup->addButton(radio0, 0);
	polyLineButtonGroup->addButton(radio1, 1);
	polyLineButtonGroup->addButton(radio2, 2);

	radio0->setChecked(true);


	QGroupBox *Gb1 = new	QGroupBox(PaletteTab);
	//QVBoxLayout *Gb1l = new QVBoxLayout;
	Gb1->setLayout(new QVBoxLayout);




	//b=new QWidget(Gb1);	
	//	b=new QGroupBox(Gb1); b->setLayout(new QHBoxLayout);
	generateClippedSurfaceButton = new QPushButton2(buttonList,"Generate Surface", Gb1);	Gb1->layout()->addWidget(generateClippedSurfaceButton);



	//b=new QWidget(Gb1);	
	//b=new QGroupBox(Gb1); b->setLayout(new QHBoxLayout);
	//segmentBrainButton=new QPushButton2(buttonList,"Segment Brain",Gb1);	Gb1->layout()->addWidget(segmentBrainButton);
	//b=new QWidget(Gb1);	
	//	b=new QGroupBox(Gb1); b->setLayout(new QHBoxLayout);
	generateVisibleMaskButton = new QPushButton2(buttonList,"Generate Visible Mask", Gb1); Gb1->layout()->addWidget(generateVisibleMaskButton);
	//b=new QWidget(Gb1);
	//b=new QGroupBox(Gb1); b->setLayout(new QHBoxLayout);
	extractMaskedVolumeButton = new QPushButton2(buttonList,"Extract Masked Volume", Gb1); Gb1->layout()->addWidget(extractMaskedVolumeButton);


	//qtab->addTab(PaletteTab,"&Volume Vis ");
	

	QGroupBox* Gb1a = new	QGroupBox(PaletteTab);
	Gb1a->setLayout(new QVBoxLayout);
	QLabel* tmpL = new QLabel("3DPointer Scale:", Gb1a);Gb1a->layout()->addWidget(tmpL);
	resize3DPointerScale1 = new QSlider2(sliderList, Qt::Horizontal, Gb1a); Gb1a->layout()->addWidget(resize3DPointerScale1);
	resize3DPointerScale2 = new QSlider2(sliderList, Qt::Horizontal, Gb1a); Gb1a->layout()->addWidget(resize3DPointerScale2);
	resize3DPointerScale1->setRange(1, 1000);
	resize3DPointerScale2->setRange(1, 10000);

	MainLayout->addWidget(Gb3);
	MainLayout->addWidget(gr);
	MainLayout->addWidget(Gb1);
	MainLayout->addWidget(Gb1a);
}

void GUIPanelWidget::CreateSegmentationTab(MyDockWidget *qtab)
{
	//QWidget *SegmentationTab = new QWidget(qtab);
	//QVBoxLayout* MainLayout= new QVBoxLayout( SegmentationTab, 10, -1, "MainLayout");	


	//QValidator *ValidatorDouble =new TextFieldDoubleValidator(0,100.0,15,this);
	//QValidator *ValidatorInt =new QIntValidator(0,65535,this);




	////Q3GroupBox *Gb1=new Q3GroupBox(6,Vertical,SegmentationTab);
	//
	//	//Q3GroupBox *Gb1=new Q3GroupBox(20,Vertical,PaletteTab);
	//QGroupBox *Gb1=new	QGroupBox(SegmentationTab);
	////QVBoxLayout *Gb1l = new QVBoxLayout;
	//Gb1->setLayout(new QVBoxLayout());
	//Gb1->setTitle("3D ScanLine");

	//QWidget *PercentBox=new QWidget(Gb1);		
	//QLabel *L1=new QLabel("Percent: ",PercentBox);
	//PercentSpinBox = new QSpinBox2(spinBoxList,0,100,1,PercentBox);
	//PercentSpinBox->setValue(50);

	////	QHBox *NeighbourhoodBox=new QHBox(Gb1);		
	//QLabel *L2=new QLabel("Neighbors: ",PercentBox);
	//NeighSpinBox = new QSpinBox2(spinBoxList,1,50,1,PercentBox);
	//NeighSpinBox->setValue(1);
	//L1->setMinimumWidth(30);
	//PercentSpinBox->setMinimumWidth(30);
	//L2->setMinimumWidth(30);
	//NeighSpinBox->setMinimumWidth(30);


	//QWidget *ScanlineBox=new QWidget(Gb1);	


	//ScanXCheck = new QCheckBox2(checkBoxList,ScanlineBox);
	//ScanXCheck->setText("X: ");
	//ScanXCheck->setChecked(true);

	//ScanYCheck = new QCheckBox2(checkBoxList,ScanlineBox);
	//ScanYCheck->setText("Y: ");
	//ScanYCheck->setChecked(true);

	//ScanZCheck = new QCheckBox2(checkBoxList,ScanlineBox);
	//ScanZCheck->setText("Z: ");
	//ScanZCheck->setChecked(true);

	//ScanLineSegmentButton=new QPushButton2(buttonList,"Segment",ScanlineBox);
	//ScanLineSegmentButton->setMinimumWidth(45);



	////-Thresholding

	////Q3GroupBox *Gb2=new Q3GroupBox(4,Vertical,SegmentationTab);
	//QGroupBox *Gb2=new	QGroupBox(SegmentationTab);
	////QVBoxLayout *Gb2l = new QVBoxLayout;
	//Gb2->setLayout(new QVBoxLayout);
	//Gb2->setTitle("Thresholding");



	////QLabel *L02=new QLabel(Gb2);
	////L02->setAutoMask(true);
	////L02->setFixedHeight(1);
	//QWidget *ThreshBox=new QWidget(Gb2);		

	//ThresholdSegmentButton=new QPushButton2(buttonList,"Segment ",ThreshBox);
	//ThresholdRemoveButton=new QPushButton2(buttonList,"Segment Rest",ThreshBox);
	//ThresholdSegmentButton->setMinimumWidth(30);
	//ThresholdRemoveButton->setMinimumWidth(30);




	////-przyciski	





	////Region Growing




	////Q3GroupBox *Gb4=new Q3GroupBox(20,Vertical,SegmentationTab);
	//	QGroupBox *Gb4=new	QGroupBox(SegmentationTab);
	////QVBoxLayout *Gb4l = new QVBoxLayout;
	//Gb4->setLayout(new QVBoxLayout);
	////Gb4->layout()->setMargin(4);
	//Gb4->setTitle("Segmentation Tools");

	////	QLabel *L4S = new QLabel("Tolerance T:",Gb4);




	////----------------------------------------------

	//QWidget *RegionTolBox 	=	new QWidget(Gb4);
	////RegionTolBox->setSpacing(10);


	//QLabel *Tol=new QLabel("Tol: %:",RegionTolBox);
	//Tol->setFixedWidth(35);
	//ToleranceEdit=new QLineEdit2(lineEditList,RegionTolBox);
	//ToleranceEdit->setValidator(ValidatorDouble);
	////	ToleranceEdit->setMaximumWidth(50);
	//ToleranceEdit->setText("5");
	//QLabel *Tol1=new QLabel(" I:",RegionTolBox);
	//Tol1->setFixedWidth(15);
	//TempPercent=ToleranceEdit->text().toFloat();

	//ToleranceValueEdit=new QLineEdit2(lineEditList,RegionTolBox);
	//ToleranceValueEdit->setValidator(ValidatorInt);
	////	ToleranceValueEdit->setMaximumWidth(50);
	//ToleranceValueEdit->setText("0");




	//ToleranceSlider=new QSlider2(sliderList,-100,100,1,0,Qt::Horizontal, RegionTolBox);
	//ToleranceSlider->setMinimumWidth(40);

	//connect(ToleranceSlider,SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateTolerance(int)));
	//connect(ToleranceSlider,SIGNAL(sliderReleased ()),this,SLOT(SlotUpdateTolerance()));
	//connect(ToleranceEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTolerancePercent(const QString &)));
	//connect(ToleranceValueEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateToleranceValue(const QString &)));





	////		TParams[i]=TEdit[i]->text().toFloat();

	////----------------------------------------------







	////ToleranceSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb4, "Tolerance:");
	////ToleranceSlider->setRange(0,1000);
	////ToleranceSlider->setValue(100);


	//HomogenityCriterionCombo = new QComboBox2(comboBoxList, Gb4, "read-only combo" );



	//HomogenityCriterionCombo->addAction("| X-Seed | < T",0);
	//HomogenityCriterionCombo->addAction("| X-Mean | < T",1);
	//HomogenityCriterionCombo->addAction("T1 <= X <= T2",2);
	//HomogenityCriterionCombo->setCurrentIndex(2);
	///*	HomogenityCriterionCombo->addAction("| X-Median | < T",2);
	//HomogenityCriterionCombo->addAction("T1 <= X <= T2",3);
	//HomogenityCriterionCombo->addAction("| Seed-MeanKer | < T",4);
	//HomogenityCriterionCombo->addAction("| Seed-MedianKer | < T",5);
	////	HomogenityCriterionCombo->addAction(text,4);
	//*/
	//QWidget *ThreshAnd3DBox=new QWidget(Gb4);

	//ThreeDimCheckBox=new QCheckBox2(checkBoxList,ThreshAnd3DBox);
	//ThreeDimCheckBox->setText("3D ");
	//ThreeDimCheckBox->setChecked(true);


	//MaskAppendCheckBox=new QCheckBox2(checkBoxList,ThreshAnd3DBox);
	//MaskAppendCheckBox->setText("App ");
	//MaskAppendCheckBox->setChecked(true);


	//FillSegmentButton=new QPushButton2(buttonList,"Region Growing",ThreshAnd3DBox);
	//FillSegmentButton->setMinimumWidth(70);



	//QWidget *MorphologyBox=new QWidget(Gb4);	
	//DilateMaskButton=new QPushButton2(buttonList,"Dilate",MorphologyBox);
	//ErodeMaskButton=new QPushButton2(buttonList,"Erode",MorphologyBox);
	//QWidget *MorphologyBox2=new QWidget(Gb4);
	//QLabel *LM1 = new QLabel("KSize:",MorphologyBox2);	

	//MorphologyKernelSizeSpinBox=new QSpinBox2(spinBoxList,1,50,1,MorphologyBox2);
	//MorphologyKernelSizeSpinBox->setValue(1);
	//MorphologyKernelSizeSpinBox->setMinimumWidth(28);
	//QLabel *LM2 = new QLabel("KType:",MorphologyBox2);	

	//MorphologyKernelTypeSpinBox=new QSpinBox2(spinBoxList,0,2,1,MorphologyBox2);
	//MorphologyKernelTypeSpinBox->setValue(0);
	//MorphologyKernelTypeSpinBox->setMinimumWidth(28);




	//MorphologyWithCriterionCheckBox=new QCheckBox2(checkBoxList,MorphologyBox2);
	//MorphologyWithCriterionCheckBox->setText("On");
	//MorphologyWithCriterionCheckBox->setMinimumWidth(38);


	//QWidget *MorphologyBox3=new QWidget(Gb4);
	//ClearWithFillCheckBox=new QCheckBox2(checkBoxList,MorphologyBox3);
	//ClearWithFillCheckBox->setText("Clear Fill ");

	//ObjectThresholdCritBox=new QCheckBox2(checkBoxList,MorphologyBox3);
	//ObjectThresholdCritBox->setText("OTC ");

	//FillMaskButton=new QPushButton2(buttonList,"Fill Mask",MorphologyBox3);
	//FillMaskButton->setMinimumWidth(30);


	////---------------------------------------------------------







	//QWidget *FillOptionsBox=new QWidget(Gb4);


	//QLabel *FillTextLabel=new QLabel("Fill Mode: ",FillOptionsBox);

	//FillModeCombo = new QComboBox2(comboBoxList, FillOptionsBox, "read-only combo" );
	//FillModeCombo->addAction("OR   A|B",0);
	//FillModeCombo->addAction("CLR  A&!B",1);
	//FillModeCombo->addAction("AND  A&B",2);
	//FillModeCombo->addAction("NOR  !(A|B)",3);
	//FillModeCombo->addAction("NAND !(A&B)",4);
	//FillModeCombo->addAction("XOR  A^B",5);
	//FillModeCombo->addAction("CLR2 !A&B",6);
	//FillModeCombo->setMinimumWidth(50);
	//QLabel *TMPLabel=new QLabel(" ",FillOptionsBox);

	//FillThroughAllSlices=new QCheckBox2(checkBoxList,FillOptionsBox);
	//FillThroughAllSlices->setText("All Slices");
	//FillThroughAllSlices->setChecked(false);

	//MaskButtonGroup=new Q3HButtonGroup(Gb4);
	//MaskButtonGroup->setExclusive(true);
	//MaskButtonGroup->layout()->setMargin(5);
	//MaskButtonGroup->layout()->setSpacing(1);



	//int Nrm=6;
	//QToolButton **MaskToolButton;
	//MaskToolButton =new QToolButton*[Nrm]; 
	//QImage buffer,temp;
	//QPixmap pix;



	//int i;
	//for (i=0;i<Nrm;i++)
	//{
	//	MaskToolButton[i]=new QToolButton(MaskButtonGroup);
	//	MaskToolButton[i]->setToggleButton(true);
	//}

	////MaskToolButton[0]->setDown(true);
	//MaskButtonGroup->setButton(0);

	//if (buffer.load("MaskButtons.png"))
	//{

	//	for( i=0;i<Nrm;i++)
	//	{

	//		temp=buffer.copy(i*20,0,20,20);
	//		pix.convertFromImage(temp);
	//		MaskToolButton[i]->setPixmap(pix);


	//	}

	//}
	//else
	//{
	//	//for(int AA=0;AA<9;AA++)
	//	//	LayoutMenu->addAction(pix,"Layout "+QString::number(AA),AA);
	//}




	///*
	////Kopiuje ten polygon do rysunku
	//QImage buffer,temp;
	//QPixmap pix;

	//LayoutMenu = new QPopupMenu(this);

	//if (buffer.load("Layouts.png"))
	//{

	//for(int AA=0;AA<9;AA++)
	//{

	//temp=buffer.copy(AA*20,0,20,20);
	//pix.convertFromImage(temp);
	//LayoutMenu->addAction(pix,"Layout "+QString::number(AA),AA);

	//}

	//}
	//else
	//{
	//for(int AA=0;AA<9;AA++)
	//LayoutMenu->addAction(pix,"Layout "+QString::number(AA),AA);
	//}

	//*/
	////----------------------------------------------------------
	////	QGroupBox *Gb5=new QGroupBox(10,Vertical,SegmentationTab);



	////	QLabel *L3S = new QLabel("Mask Opacity :",Gb5);	

	////Q3GroupBox *Gb7=new Q3GroupBox(20,Vertical,SegmentationTab);
	//	QGroupBox *Gb7=new	QGroupBox(SegmentationTab);
	////QVBoxLayout *Gb7l = new QVBoxLayout;
	//Gb7->setLayout(new QVBoxLayout());
	//QWidget *MaskOpacityBox=new QWidget(Gb7);
	//QLabel *L3S = new QLabel("Opacity:",MaskOpacityBox);	

	//MaskOpacitySlider	= new QSlider2(sliderList,Qt::Horizontal, MaskOpacityBox);
	//MaskOpacitySlider->setRange(0,255);
	//MaskOpacitySlider->setValue(255);


	//connect(MaskOpacitySlider,SIGNAL(valueChanged(int)),this,SIGNAL(SendMaskOpacity(int)));


	////Q3GroupBox *Gb6=new Q3GroupBox(2,Horizontal,Gb7);
	//QGroupBox *Gb6=new	QGroupBox(Gb7);
	////QHBoxLayout *Gb6l = new QHBoxLayout;
	//Gb6->setLayout(new QHBoxLayout);
	//Gb6->setTitle("Create new DataSet");
	//NANDButton=new QPushButton2(buttonList,"AND",Gb6);
	//ANDButton=new QPushButton2(buttonList,"NAND",Gb6);




	//NANDButton->setMinimumWidth(30);
	//ANDButton->setMinimumWidth(30);



	//QWidget *UndoRedoSegmentBox=new QWidget(Gb7);
	//UndoSegmentButton=new QPushButton2(buttonList,"Undo",UndoRedoSegmentBox);
	//RedoSegmentButton=new QPushButton2(buttonList,"Redo",UndoRedoSegmentBox);

	//QWidget *LoadSaveMaskBox=new QWidget(Gb7);
	//LoadMaskButton=new QPushButton2(buttonList,"Load Mask",LoadSaveMaskBox);
	//SaveMaskButton=new QPushButton2(buttonList,"Save Mask",LoadSaveMaskBox);


	//CalculateMaskVolumeButton=new QPushButton2(buttonList,"Calculate Mask Volume",Gb7);
	//GenerateMarchingCubesSurfaceButton=new QPushButton2(buttonList,"Generate MC Surface",Gb7);



	//MainLayout->addWidget(Gb1);
	//MainLayout->addWidget(Gb2);

	//MainLayout->addWidget(Gb4);
	//MainLayout->addWidget(Gb6);
	//MainLayout->addWidget(Gb7);

	//Gb1->layout()->setSpacing(3);
	//Gb1->layout()->setMargin(5);
	//Gb2->layout()->setSpacing(3);
	//Gb2->layout()->setMargin(5);
	//Gb4->layout()->setSpacing(3);
	//Gb4->layout()->setMargin(5);
	//Gb6->layout()->setSpacing(3);
	//Gb6->layout()->setMargin(5);

	//Gb7->layout()->setSpacing(3);
	//Gb7->layout()->setMargin(5);

	////	MainLayout->addWidget(MaskOpacityBox);

	////MainLayout->addWidget(UndoRedoSegmentBox);

	////	MainLayout->addWidget(LoadSaveMaskBox);
	////MainLayout->addWidget(CalculateMaskVolumeButton);

	////	MainLayout->addWidget(GenerateMarchingCubesSurfaceButton);




	//qtab->addTab(SegmentationTab,"&Segmentation ");
QWidget *SegmentationTab = new QWidget(qtab);
qtab->setScrollWidget(SegmentationTab);
	QVBoxLayout* MainLayout= new QVBoxLayout( SegmentationTab);//, 10, -1, "MainLayout");	
	SegmentationTab->setLayout(MainLayout);
	MainLayout->setSpacing(1);
	TextFieldDoubleValidator *ValidatorDouble =new TextFieldDoubleValidator(0,100.0,15,this);
	ValidatorDouble->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *ValidatorInt =new QIntValidator(0,65535,this);


	SaveCurrentMaskWithPreviousPathButton = new QPushButton2(buttonList, "SaveMaskWithPreviousPath(unsafe)", SegmentationTab);
	SegmentationTab->layout()->addWidget(SaveCurrentMaskWithPreviousPathButton);

	SaveMaskAsUnsignedCharArrayButton=new QPushButton2(buttonList, "SaveMaskAsUnsignedCharArrayButton", SegmentationTab);
	SegmentationTab->layout()->addWidget(SaveMaskAsUnsignedCharArrayButton);


	LoadMaskAsUnsignedCharArrayButton = new QPushButton2(buttonList, "LoadMaskAsUnsignedCharArrayButton", SegmentationTab);
	SegmentationTab->layout()->addWidget(LoadMaskAsUnsignedCharArrayButton);


	GenerateMaskUsingNeuralSegmentatorButton = new QPushButton2(buttonList, "GenerateMaskUsingNeuralSegmentatorButton", SegmentationTab);
	SegmentationTab->layout()->addWidget(GenerateMaskUsingNeuralSegmentatorButton);

	GenerateMaskUsingJadraSegmentatorButton = new QPushButton2(buttonList, "GenerateMaskUsingJadraSegmentatorButton", SegmentationTab);
	SegmentationTab->layout()->addWidget(GenerateMaskUsingJadraSegmentatorButton);


	//Q3GroupBox *Gb1=new Q3GroupBox(6,Vertical,SegmentationTab);
	QGroupBox *Gb1=new	QGroupBox(SegmentationTab);
	QVBoxLayout *vbl_gb1 = new QVBoxLayout;
	vbl_gb1->setSpacing(1);
	vbl_gb1->setMargin(1);
	Gb1->setLayout(vbl_gb1);	
	MainLayout->addWidget(Gb1);

	Gb1->setTitle("3D ScanLine");

	//QWidget *PercentBox=new QWidget(Gb1);
	QWidget *PercentBox=new	QWidget(Gb1);
	QHBoxLayout *hbl_PercentBox = new QHBoxLayout;
	hbl_PercentBox->setSpacing(0);
	vbl_gb1->setMargin(1);

	PercentBox->setLayout(hbl_PercentBox);
	PercentBox->layout()->setSpacing(1);
	PercentBox->layout()->setMargin(2);

	vbl_gb1->addWidget(PercentBox);
	QLabel *L1=new QLabel("Percent: ",PercentBox);
	PercentSpinBox = new QSpinBox2(spinBoxList,PercentBox);
	PercentSpinBox->setRange(0,100);
	PercentSpinBox->setValue(50);

	//	QHBox *NeighbourhoodBox=new QHBox(Gb1);		
	QLabel *L2=new QLabel("neighbours: ",PercentBox);
	NeighSpinBox = new QSpinBox2(spinBoxList,PercentBox);
	NeighSpinBox->setRange(0,50);
	NeighSpinBox->setValue(1);
	L1->setMinimumWidth(30);
	PercentSpinBox->setMinimumWidth(30);
	L2->setMinimumWidth(30);
	NeighSpinBox->setMinimumWidth(30);

	hbl_PercentBox->addWidget(L1);
	hbl_PercentBox->addWidget(PercentSpinBox);
	hbl_PercentBox->addWidget(L2);
	hbl_PercentBox->addWidget(NeighSpinBox);



	//	QWidget *ScanlineBox=new QWidget(Gb1);	
	QWidget *ScanlineBox=new	QWidget(Gb1);
	vbl_gb1->addWidget(ScanlineBox);
	QHBoxLayout *hbl_ScanlineBox = new QHBoxLayout;
	hbl_ScanlineBox->setSpacing(1);
	hbl_ScanlineBox->setMargin(1);
	ScanlineBox->setLayout(hbl_ScanlineBox);

	ScanXCheck = new QCheckBox2(checkBoxList,ScanlineBox);
	ScanXCheck->setText("X: ");
	ScanXCheck->setChecked(true);

	ScanYCheck = new QCheckBox2(checkBoxList,ScanlineBox);
	ScanYCheck->setText("Y: ");
	ScanYCheck->setChecked(true);

	ScanZCheck = new QCheckBox2(checkBoxList,ScanlineBox);
	ScanZCheck->setText("Z: ");
	ScanZCheck->setChecked(true);

	ScanLineSegmentButton=new QPushButton2(buttonList,"Segment",ScanlineBox);
	ScanLineSegmentButton->setMinimumWidth(45);

	hbl_ScanlineBox->addWidget(ScanXCheck);
	hbl_ScanlineBox->addWidget(ScanYCheck);
	hbl_ScanlineBox->addWidget(ScanZCheck);
	hbl_ScanlineBox->addWidget(ScanLineSegmentButton);

	//-Thresholding

	//Q3GroupBox *Gb2=new Q3GroupBox(4,Vertical,SegmentationTab);
	QGroupBox *Gb2=new	QGroupBox(SegmentationTab);
	QLayout *vbl_Gb2 = new QVBoxLayout();
	

	Gb2->setLayout(vbl_Gb2);
	Gb2->layout()->setSpacing(1);
	Gb2->layout()->setMargin(2);

	Gb2->setTitle("Thresholding");
	MainLayout->addWidget(Gb2);



	//QLabel *L02=new QLabel(Gb2);
	//L02->setAutoMask(true);
	//L02->setFixedHeight(1);

	//QWidget *ThreshBox=new QWidget(Gb2);
	QWidget *ThreshBox=new	QWidget(Gb2);
	QHBoxLayout *hbl_ThreshBox = new QHBoxLayout;
	
	ThreshBox->setLayout(hbl_ThreshBox);
	ThreshBox->layout()->setSpacing(1);
	ThreshBox->layout()->setMargin(2);

	vbl_Gb2->addWidget(ThreshBox);

	ThresholdSegmentButton=new QPushButton2(buttonList,"Segment ",ThreshBox);
	ThresholdRemoveButton=new QPushButton2(buttonList,"Segment Rest",ThreshBox);
	ThresholdSegmentButton->setMinimumWidth(30);
	ThresholdRemoveButton->setMinimumWidth(30);
	hbl_ThreshBox->addWidget(ThresholdSegmentButton);
	hbl_ThreshBox->addWidget(ThresholdRemoveButton);





	//-przyciski	





	//Region Growing




	//Q3GroupBox *Gb4=new Q3GroupBox(20,Vertical,SegmentationTab);

	QGroupBox *Gb4=new	QGroupBox(SegmentationTab);
	QVBoxLayout *vbl_Gb4 = new QVBoxLayout;
	vbl_Gb4->setMargin(1);
	vbl_Gb4->setSpacing(0);
	Gb4->setLayout(vbl_Gb4);

	MainLayout->addWidget(Gb4);
	//Gb4->setInsideMargin(4);
	Gb4->setTitle("Segmentation Tools");

	//	QLabel *L4S = new QLabel("Tolerance T:",Gb4);




	//----------------------------------------------

	//	QWidget *RegionTolBox 	=	new QWidget(Gb4);
	QWidget *RegionTolBox=new	QWidget(Gb4);
	QHBoxLayout *hbl_RegionTolBox = new QHBoxLayout;
	hbl_RegionTolBox->setMargin(1);
	hbl_RegionTolBox->setSpacing(0);
	RegionTolBox->setLayout(hbl_RegionTolBox);

	vbl_Gb4->addWidget(RegionTolBox);
	//RegionTolBox->setSpacing(10);


	QLabel *Tol=new QLabel("Tol: %:",RegionTolBox);
	Tol->setFixedWidth(35);
	ToleranceEdit=new QLineEdit2(lineEditList,RegionTolBox);
	ToleranceEdit->setValidator(ValidatorDouble);
	//	ToleranceEdit->setMaximumWidth(50);
	ToleranceEdit->setText("5");
	QLabel *Tol1=new QLabel(" I:",RegionTolBox);
	Tol1->setFixedWidth(15);
	TempPercent=ToleranceEdit->text().toFloat();

	ToleranceValueEdit=new QLineEdit2(lineEditList,RegionTolBox);
	ToleranceValueEdit->setValidator(ValidatorInt);
	//	ToleranceValueEdit->setMaximumWidth(50);
	ToleranceValueEdit->setText("0");




	ToleranceSlider=new QSlider2(sliderList,Qt::Horizontal, RegionTolBox);
	ToleranceSlider->setRange(-100,100);
	ToleranceSlider->setMinimumWidth(40);

	hbl_RegionTolBox->addWidget(Tol);
	hbl_RegionTolBox->addWidget(ToleranceEdit);
	hbl_RegionTolBox->addWidget(Tol1);
	hbl_RegionTolBox->addWidget(ToleranceValueEdit);
	hbl_RegionTolBox->addWidget(ToleranceSlider);


	connect(ToleranceSlider,SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateTolerance(int)));
	connect(ToleranceSlider,SIGNAL(sliderReleased ()),this,SLOT(SlotUpdateTolerance()));
	connect(ToleranceEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTolerancePercent(const QString &)));
	connect(ToleranceValueEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateToleranceValue(const QString &)));





	//		TParams[i]=TEdit[i]->text().toFloat();

	//----------------------------------------------







	//ToleranceSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb4, "Tolerance:");
	//ToleranceSlider->setRange(0,1000);
	//ToleranceSlider->setValue(100);


	HomogenityCriterionCombo = new QComboBox2(comboBoxList, Gb4 );

	vbl_Gb4->addWidget(HomogenityCriterionCombo);


	HomogenityCriterionCombo->addItem("| X-Seed | < T");
	HomogenityCriterionCombo->addItem("| X-Mean | < T");
	HomogenityCriterionCombo->addItem("T1 <= X <= T2");
	/*	HomogenityCriterionCombo->addAction("| X-Median | < T",2);
	HomogenityCriterionCombo->addAction(3,"T1 <= X <= T2");
	HomogenityCriterionCombo->addAction(4,"| Seed-MeanKer | < T");
	HomogenityCriterionCombo->addAction(5,"| Seed-MedianKer | < T");
	//	HomogenityCriterionCombo->addAction(text,4);
	*/

	HomogenityCriterionCombo->setCurrentIndex(2);
	//QWidget *ThreshAnd3DBox=new QWidget(Gb4);
	QWidget *ThreshAnd3DBox=new	QWidget(Gb4);
	vbl_Gb4->addWidget(ThreshAnd3DBox);
	QHBoxLayout *hbl_ThreshAnd3DBox = new QHBoxLayout;
	hbl_ThreshAnd3DBox->setMargin(1);
	hbl_ThreshAnd3DBox->setSpacing(0);
	ThreshAnd3DBox->setLayout(hbl_ThreshAnd3DBox);

	ThreeDimCheckBox=new QCheckBox2(checkBoxList,ThreshAnd3DBox);
	ThreeDimCheckBox->setText("3D ");
	ThreeDimCheckBox->setChecked(false);


	MaskAppendCheckBox=new QCheckBox2(checkBoxList,ThreshAnd3DBox);
	MaskAppendCheckBox->setText("App ");
	MaskAppendCheckBox->setChecked(true);


	FillSegmentButton=new QPushButton2(buttonList,"Region Growing",ThreshAnd3DBox);
	FillSegmentButton->setMinimumWidth(70);

	hbl_ThreshAnd3DBox->addWidget(ThreeDimCheckBox);
	hbl_ThreshAnd3DBox->addWidget(MaskAppendCheckBox);
	hbl_ThreshAnd3DBox->addWidget(FillSegmentButton);

	//QWidget *MorphologyBox=new QWidget(Gb4);	
	QWidget *MorphologyBox=new	QWidget(Gb4);
	vbl_Gb4->addWidget(MorphologyBox);
	QHBoxLayout *hbl_MorphologyBox = new QHBoxLayout;
	hbl_MorphologyBox->setMargin(1);
	hbl_MorphologyBox->setSpacing(0);
	MorphologyBox->setLayout(hbl_MorphologyBox);

	DilateMaskButton=new QPushButton2(buttonList,"Dilate",MorphologyBox);
	ErodeMaskButton=new QPushButton2(buttonList,"Erode",MorphologyBox);
	InvertMaskButton = new QPushButton2(buttonList,"Invert", MorphologyBox);
	
	hbl_MorphologyBox->addWidget(DilateMaskButton);
	hbl_MorphologyBox->addWidget(ErodeMaskButton);
	hbl_MorphologyBox->addWidget(InvertMaskButton);


	//QWidget *MorphologyBox2=new QWidget(Gb4);
	QWidget *MorphologyBox2=new	QWidget(Gb4);
	vbl_Gb4->addWidget(MorphologyBox2);
	QHBoxLayout *hbl_MorphologyBox2 = new QHBoxLayout;
	hbl_MorphologyBox2->setMargin(1);
	hbl_MorphologyBox2->setSpacing(0);
	MorphologyBox2->setLayout(hbl_MorphologyBox2);	

	QLabel *LM1 = new QLabel("KSize:",MorphologyBox2);	

	MorphologyKernelSizeSpinBox=new QSpinBox2(spinBoxList,MorphologyBox2);
	MorphologyKernelSizeSpinBox->setRange(1,5);
	MorphologyKernelSizeSpinBox->setValue(1);
	MorphologyKernelSizeSpinBox->setMinimumWidth(28);
	QLabel *LM2 = new QLabel("KType:",MorphologyBox2);	

	MorphologyKernelTypeSpinBox=new QSpinBox2(spinBoxList,MorphologyBox2);
	MorphologyKernelTypeSpinBox->setRange(0,1);
	MorphologyKernelTypeSpinBox->setValue(0);
	MorphologyKernelTypeSpinBox->setMinimumWidth(28);

	MorphologyWithCriterionCheckBox=new QCheckBox2(checkBoxList,MorphologyBox2);
	MorphologyWithCriterionCheckBox->setText("On");
	MorphologyWithCriterionCheckBox->setMinimumWidth(38);

	hbl_MorphologyBox2->addWidget(LM1);
	hbl_MorphologyBox2->addWidget(MorphologyKernelSizeSpinBox);
	hbl_MorphologyBox2->addWidget(LM2);
	hbl_MorphologyBox2->addWidget(MorphologyKernelTypeSpinBox);
	hbl_MorphologyBox2->addWidget(MorphologyWithCriterionCheckBox);

	//QWidget *MorphologyBox3=new QWidget(Gb4);

	QWidget *MorphologyBox3=new	QWidget(Gb4);
	vbl_Gb4->addWidget(MorphologyBox3);
	QHBoxLayout *hbl_MorphologyBox3 = new QHBoxLayout;
	hbl_MorphologyBox3->setMargin(1);
	hbl_MorphologyBox3->setSpacing(0);
	MorphologyBox3->setLayout(hbl_MorphologyBox3);	

	ClearWithFillCheckBox=new QCheckBox2(checkBoxList,MorphologyBox3);
	ClearWithFillCheckBox->setText("Clear Fill ");

	ObjectThresholdCritBox=new QCheckBox2(checkBoxList,MorphologyBox3);
	ObjectThresholdCritBox->setText("OTC ");

	FillMaskButton=new QPushButton2(buttonList,"Fill Mask",MorphologyBox3);
	FillMaskButton->setMinimumWidth(30);

	hbl_MorphologyBox3->addWidget(ClearWithFillCheckBox);
	hbl_MorphologyBox3->addWidget(ObjectThresholdCritBox);
	hbl_MorphologyBox3->addWidget(FillMaskButton);
	//---------------------------------------------------------







	//QWidget *FillOptionsBox=new QWidget(Gb4);
	QWidget *FillOptionsBox=new	QWidget(Gb4);
	vbl_Gb4->addWidget(FillOptionsBox);
	QHBoxLayout *hbl_FillOptionsBox = new QHBoxLayout;
	hbl_FillOptionsBox->setMargin(1);
	hbl_FillOptionsBox->setSpacing(0);
	FillOptionsBox->setLayout(hbl_FillOptionsBox);

	QLabel *FillTextLabel=new QLabel("Fill Mode: ",FillOptionsBox);

	FillModeCombo = new QComboBox2(comboBoxList, FillOptionsBox );
	FillModeCombo->addItem( "OR   A|B");
	FillModeCombo->addItem( "CLR  A&!B");
	FillModeCombo->addItem( "AND  A&B");
	FillModeCombo->addItem("NOR  !(A|B)");
	FillModeCombo->addItem( "NAND !(A&B)");
	FillModeCombo->addItem( "XOR  A^B");
	FillModeCombo->addItem("CLR2 !A&B");
	FillModeCombo->setMinimumWidth(50);
	QLabel *TMPLabel=new QLabel(" ",FillOptionsBox);

	FillThroughAllSlices=new QCheckBox2(checkBoxList,FillOptionsBox);
	FillThroughAllSlices->setText("All Slices");
	FillThroughAllSlices->setChecked(false);

	hbl_FillOptionsBox->addWidget(FillTextLabel);
	hbl_FillOptionsBox->addWidget(FillModeCombo);
	hbl_FillOptionsBox->addWidget(TMPLabel);
	hbl_FillOptionsBox->addWidget(FillThroughAllSlices);


	//	MaskButtonGroup=new Q3HButtonGroup(Gb4);
	MaskButtonGroup=new QButtonGroup;

	QGroupBox *MaskButtonGroup1=new	QGroupBox(Gb4);
	vbl_Gb4->addWidget(MaskButtonGroup1);
	QHBoxLayout *hbl_MaskButtonGroup = new QHBoxLayout;
	hbl_MaskButtonGroup->setMargin(0);
	hbl_MaskButtonGroup->setSpacing(0);
	MaskButtonGroup1->setLayout(hbl_MaskButtonGroup);

		MaskButtonGroup->setExclusive(true);
	//	MaskButtonGroup->setInsideMargin(5);
	//	MaskButtonGroup->setInsideSpacing(1);



	int Nrm=6;
	
	MaskToolButton =new QPushButton2*[Nrm]; 
	QImage buffer,temp;
	QPixmap pix;



	int i;
	for (i=0;i<Nrm;i++)
		{
		MaskToolButton[i]=new QPushButton2(buttonList,MaskButtonGroup1);
		MaskToolButton[i]->setCheckable(true);
		//		MaskToolButton[i]->setToggleButton(true);
		hbl_MaskButtonGroup->addWidget(MaskToolButton[i]);
		MaskButtonGroup->addButton(MaskToolButton[i],i);

		}

	MaskToolButton[0]->setChecked(true);

	//	MaskButtonGroup->setButton(0);

	if (buffer.load("images/MaskButtons.png"))
		{

		for( i=0;i<Nrm;i++)
			{

			temp=buffer.copy(i*20,0,20,20);
			pix=QPixmap::fromImage(temp);
			QIcon icon(pix.scaled(pix.width() * iconScaleFactor, pix.height() * iconScaleFactor));

			//MaskToolButton[i]->setDefaultAction(new QAction(icon,"",MaskButtonGroup));
			MaskToolButton[i]->setIcon(pix);


			}
		//PaletteCombo->setIconSize(QSize(90,12));

		}
	else
		{
		//for(int AA=0;AA<9;AA++)
		//	LayoutMenu->addAction(pix,"Layout "+QString::number(AA),AA);
		}




	/*
	//Kopiuje ten polygon do rysunku
	QImage buffer,temp;
	QPixmap pix;

	LayoutMenu = new QPopupMenu(this);

	if (buffer.load("Layouts.png"))
	{

	for(int AA=0;AA<9;AA++)
	{

	temp=buffer.copy(AA*20,0,20,20);
	pix.convertFromImage(temp);
	LayoutMenu->addAction(pix,"Layout "+QString::number(AA),AA);

	}

	}
	else
	{
	for(int AA=0;AA<9;AA++)
	LayoutMenu->addAction(pix,"Layout "+QString::number(AA),AA);
	}

	*/
	//----------------------------------------------------------
	//	QWidget *Gb5=new QWidget(10,Vertical,SegmentationTab);



	//	QLabel *L3S = new QLabel("Mask Opacity :",Gb5);	

	//Q3GroupBox *Gb7=new Q3GroupBox(20,Vertical,SegmentationTab);
	QLabel *lb = new QLabel("ViewMaskOptions:", SegmentationTab);
	MainLayout->addWidget(lb);
	QGroupBox *Gbx = new	QGroupBox(SegmentationTab);
	MainLayout->addWidget(Gbx);
	Gbx->setLayout(new QHBoxLayout);
	Gbx->layout()->setMargin(0);
	Gbx->layout()->setSpacing(0);
	
	

	QString names[] = { "color", "trace", "livetrace" };
	for (int i = 0; i < 3; i++)
	{
		maskModeCheckBox[i] = new QCheckBox2(checkBoxList,names[i],Gbx);
		Gbx->layout()->addWidget(maskModeCheckBox[i]);
	}
	maskModeCheckBox[0]->setVisible(false);//chowam i tak nie uzywam
	
	QGroupBox *Gb7=new	QGroupBox(SegmentationTab);
	MainLayout->addWidget(Gb7);
	QVBoxLayout *vbl_Gb7 = new QVBoxLayout;
	vbl_Gb7->setMargin(0);
	vbl_Gb7->setSpacing(0);
	Gb7->setLayout(vbl_Gb7);


	//QWidget *MaskOpacityBox=new QWidget(Gb7);
	QWidget *MaskOpacityBox=new	QWidget(Gb7);
	vbl_Gb7->addWidget(MaskOpacityBox);
	QHBoxLayout *hbl_MaskOpacityBox = new QHBoxLayout;
	MaskOpacityBox->setLayout(hbl_MaskOpacityBox);

	QLabel *L3S = new QLabel("MaskOpacity:",MaskOpacityBox);	

	MaskOpacitySlider	= new QSlider2(sliderList,Qt::Horizontal, MaskOpacityBox);
	MaskOpacitySlider->setRange(0,255);
	//MaskOpacitySlider->setValue(255);
	hbl_MaskOpacityBox->addWidget(L3S);
	hbl_MaskOpacityBox->addWidget(MaskOpacitySlider);

	connect(MaskOpacitySlider,SIGNAL(valueChanged(int)),this,SIGNAL(SendMaskOpacity(int)));



	MaskOpacityBox = new	QWidget(Gb7);
	vbl_Gb7->addWidget(MaskOpacityBox);
	 hbl_MaskOpacityBox = new QHBoxLayout;
	MaskOpacityBox->setLayout(hbl_MaskOpacityBox);


	L3S = new QLabel("ThreshOpacity:", MaskOpacityBox);

	threshMaskOpacitySlider = new QSlider2(sliderList, Qt::Horizontal, MaskOpacityBox);
	threshMaskOpacitySlider->setRange(0, 255);
	//MaskOpacitySlider->setValue(255);
	hbl_MaskOpacityBox->addWidget(L3S);
	hbl_MaskOpacityBox->addWidget(threshMaskOpacitySlider);


	

	//Q3GroupBox *Gb6=new Q3GroupBox(2,Horizontal,Gb7);

	QGroupBox *Gb6=new	QGroupBox(Gb7);
	vbl_Gb7->addWidget(Gb6);
	QHBoxLayout *hbl_Gb6 = new QHBoxLayout;
	hbl_Gb6->setSpacing(1);
	hbl_Gb6->setMargin(1);
	Gb6->setLayout(hbl_Gb6);

	Gb6->setTitle("Segmentation Tools");
	ClearMaskButton=new QPushButton2(buttonList,"Clear",Gb7);
	vbl_Gb7->addWidget(ClearMaskButton);
	NANDButton=new QPushButton2(buttonList,"AND",Gb6);
	ANDButton=new QPushButton2(buttonList,"NAND",Gb6);


	hbl_Gb6->addWidget(NANDButton);
	hbl_Gb6->addWidget(ANDButton);

	NANDButton->setMinimumWidth(30);
	ANDButton->setMinimumWidth(30);



	//QWidget *UndoRedoSegmentBox=new QWidget(Gb7);
	QWidget *UndoRedoSegmentBox=new	QWidget(Gb7);
	vbl_Gb7->addWidget(UndoRedoSegmentBox);
	QHBoxLayout *hbl_UndoRedoSegmentBox = new QHBoxLayout;
	hbl_UndoRedoSegmentBox->setSpacing(0);
	hbl_UndoRedoSegmentBox->setMargin(0);
	UndoRedoSegmentBox->setLayout(hbl_UndoRedoSegmentBox);

	UndoSegmentButton=new QPushButton2(buttonList,"Undo",UndoRedoSegmentBox);
	RedoSegmentButton=new QPushButton2(buttonList,"Redo",UndoRedoSegmentBox);

	hbl_UndoRedoSegmentBox->addWidget(UndoSegmentButton);
	hbl_UndoRedoSegmentBox->addWidget(RedoSegmentButton);

	//QWidget *LoadSaveMaskBox=new QWidget(Gb7);

	QWidget *LoadSaveMaskBox=new	QWidget(Gb7);
	vbl_Gb7->addWidget(LoadSaveMaskBox);
	QHBoxLayout *hbl_LoadSaveMaskBox = new QHBoxLayout;
	hbl_LoadSaveMaskBox->setSpacing(0);
	hbl_LoadSaveMaskBox->setMargin(0);
	LoadSaveMaskBox->setLayout(hbl_LoadSaveMaskBox);

	LoadMaskButton=new QPushButton2(buttonList,"Load Mask",LoadSaveMaskBox);
	SaveMaskButton=new QPushButton2(buttonList,"Save Mask",LoadSaveMaskBox);

	hbl_LoadSaveMaskBox->addWidget(LoadMaskButton);
	hbl_LoadSaveMaskBox->addWidget(SaveMaskButton);

	CalculateMaskVolumeButton=new QPushButton2(buttonList,"Calculate Mask Volume",Gb7);
	vbl_Gb7->addWidget(CalculateMaskVolumeButton);
	GenerateMarchingCubesSurfaceButton=new QPushButton2(buttonList,"MC Surface",Gb7);
	vbl_Gb7->addWidget(GenerateMarchingCubesSurfaceButton);
	GenerateMarchingCubesDecimSurfaceButton=new QPushButton2(buttonList,"MC-Dec",Gb7);
	vbl_Gb7->addWidget(GenerateMarchingCubesDecimSurfaceButton);


	

	QWidget *xWidget;
	QLabel *xLabel;
	Gb6=new	QGroupBox(Gb7);
	Gb6->setTitle("Set Opacity to Prev");
	vbl_Gb7->addWidget(Gb6);

	Gb6->setLayout(new QVBoxLayout);
	xWidget=new QWidget(Gb6);
	xWidget->setLayout(new QHBoxLayout);
	Gb6->layout()->addWidget(xWidget);
	setOpacityToPrevCheckBox=new QCheckBox2(checkBoxList,xWidget);
	xWidget->layout()->addWidget(setOpacityToPrevCheckBox);
	xLabel=new QLabel("Opacity:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	opacityToPrevSlider	= new QSlider2(sliderList,Qt::Horizontal, xLabel);
	xWidget->layout()->addWidget(opacityToPrevSlider);
	opacityToPrevSlider->setRange(0,255);
	opacityToPrevSlider->setValue(255);
	

	AutoCropWithMaskButton=new QPushButton2(buttonList,"Auto Crop With Mask",Gb7);
	vbl_Gb7->addWidget(AutoCropWithMaskButton);

	GenerateMarchingCubesFilledIslandButton=new QPushButton2(buttonList,"Fill Binary Islands",Gb7);
	vbl_Gb7->addWidget(GenerateMarchingCubesFilledIslandButton);

	GenerateMarchingCubesFilledGreyIslandButton=new QPushButton2(buttonList,"Fill Grey Islands",Gb7);
	vbl_Gb7->addWidget(GenerateMarchingCubesFilledGreyIslandButton);


	xWidget=new QWidget(Gb6);
	xWidget->setLayout(new QHBoxLayout);
	Gb6->layout()->addWidget(xWidget);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	

	xLabel=new QLabel("Min Isle Size:",xWidget);
	//xLabel->setFixedWidth(15);
	xWidget->layout()->addWidget(xLabel);
	FillGreyIslandsMinSizeBox=new QSpinBox2(spinBoxList,xWidget);
	FillGreyIslandsMinSizeBox->setRange(-1,100000000000000000);
	FillGreyIslandsMinSizeBox->setValue(0);
	//FillGreyIslandsMinSizeBox->setFixedWidth(kk);

	xWidget->layout()->addWidget(FillGreyIslandsMinSizeBox);


	vbl_Gb7->addWidget(xWidget);

	//0000000000000000000000000000


	//------------------------------------------------------

	vbl_Gb7->addWidget(new QLabel(Gb7));

	Gb6=new	QGroupBox(Gb7);
	Gb6->setTitle("Segmentation Buffer");
	vbl_Gb7->addWidget(Gb6);
	QVBoxLayout *vbl_Gbx = new QVBoxLayout;
	//vbl_Gbx->setSpacing(1);
	vbl_Gbx->setMargin(1);
	Gb6->setLayout(vbl_Gbx);
	SwapMaskBuffersButton=new QPushButton2(buttonList,"Swap Mask Buffers",Gb6);
	vbl_Gbx->addWidget(SwapMaskBuffersButton);

	
	ClearMask2Button=new QPushButton2(buttonList,"Clear",Gb6);
	vbl_Gbx->addWidget(ClearMask2Button);






	MaskOpacityBox=new	QWidget(Gb7);
	vbl_Gb7->addWidget(MaskOpacityBox);
	hbl_MaskOpacityBox = new QHBoxLayout;
	MaskOpacityBox->setLayout(hbl_MaskOpacityBox);

	L3S = new QLabel("Opacity:",MaskOpacityBox);	

	MaskBufferOpacitySlider	= new QSlider2(sliderList,Qt::Horizontal, MaskOpacityBox);
	MaskBufferOpacitySlider->setRange(0,255);
	MaskBufferOpacitySlider->setValue(255);
	hbl_MaskOpacityBox->addWidget(L3S);
	hbl_MaskOpacityBox->addWidget(MaskBufferOpacitySlider);

	connect(MaskBufferOpacitySlider,SIGNAL(valueChanged(int)),this,SIGNAL(SendMaskBufferOpacity(int)));





	QGroupBox *Gb6x=new	QGroupBox(Gb6);
	vbl_Gbx->addWidget(Gb6x);
	//vbl_Gb7->addWidget(Gb6);
	hbl_Gb6 = new QHBoxLayout;
	hbl_Gb6->setSpacing(1);
	hbl_Gb6->setMargin(1);
	Gb6x->setLayout(hbl_Gb6);
	CopyMaskToBufferButton=new QPushButton2(buttonList,"Copy To Buffer",Gb6x);
	BooleanMaskToBufferButton=new QPushButton2(buttonList,"Bool To Buffer",Gb6x);
	hbl_Gb6->addWidget(CopyMaskToBufferButton);
	hbl_Gb6->addWidget(BooleanMaskToBufferButton);
	


	Gb6x=new	QGroupBox(Gb6);
	vbl_Gbx->addWidget(Gb6x);
	//vbl_Gb7->addWidget(Gb6);
	hbl_Gb6 = new QHBoxLayout;
	hbl_Gb6->setSpacing(1);
	hbl_Gb6->setMargin(1);
	Gb6x->setLayout(hbl_Gb6);
	CopyBufferToMaskButton=new QPushButton2(buttonList,"Copy B To Mask",Gb6x);
	BooleanBufferToMaskButton=new QPushButton2(buttonList,"Bool B To Mask",Gb6x);
	hbl_Gb6->addWidget(CopyBufferToMaskButton);
	hbl_Gb6->addWidget(BooleanBufferToMaskButton);

	Gb6x=new	QGroupBox(Gb6);
	vbl_Gbx->addWidget(Gb6x);
	//vbl_Gb7->addWidget(Gb6);
	hbl_Gb6 = new QHBoxLayout;
	hbl_Gb6->setSpacing(1);
	hbl_Gb6->setMargin(1);
	Gb6x->setLayout(hbl_Gb6);
	UndoSegment2Button=new QPushButton2(buttonList,"Undo2",Gb6x);
	RedoSegment2Button=new QPushButton2(buttonList,"Redo2",Gb6x);
	hbl_Gb6->addWidget(UndoSegment2Button);
	hbl_Gb6->addWidget(RedoSegment2Button);



	IslandsVolumeFromBuffersButton=new QPushButton2(buttonList,"Buffer Islands Volume",Gb6);
	vbl_Gbx->addWidget(IslandsVolumeFromBuffersButton);
	vbl_Gb7->addWidget(new QLabel(Gb7));

	//Gb6x=new	QGroupBox(Gb6);
	//vbl_Gbx->addWidget(Gb6x);
	////vbl_Gb7->addWidget(Gb6);
	//hbl_Gb6 = new QHBoxLayout;
	//hbl_Gb6->setSpacing(1);
	//hbl_Gb6->setMargin(1);
	//Gb6x->setLayout(hbl_Gb6);
	//VolDilateButton=new QPushButton2(buttonList,"Dilate Vol",Gb6x);
	//VolErodeButton=new QPushButton2(buttonList,"Erode Vol",Gb6x);
	//hbl_Gb6->addWidget(VolDilateButton);
	//hbl_Gb6->addWidget(VolErodeButton);


	Gb6=new	QGroupBox(Gb7);
	Gb6->setTitle("Volume Morphology");
	vbl_Gb7->addWidget(Gb6);
	Gb6->setLayout(new QVBoxLayout);
	Gb6->layout()->setSpacing(1);
	Gb6->layout()->setMargin(1);
	xWidget=new QWidget(Gb6);
	xWidget->setLayout(new QHBoxLayout);
	Gb6->layout()->addWidget(xWidget);

	xLabel=new QLabel("Type:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	volMorphTypeComboBox=new QComboBox2(comboBoxList,xWidget);
	volMorphTypeComboBox->addItem("CUBE");
	volMorphTypeComboBox->addItem("SPHERE");
	volMorphTypeComboBox->setCurrentIndex(1);
	xWidget->layout()->addWidget(volMorphTypeComboBox);
	xLabel=new QLabel("Size:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	volMorphSizeSpinBox=new QSpinBox2(spinBoxList,xWidget);
	volMorphSizeSpinBox->setRange(1,100);
	volMorphSizeSpinBox->setValue(1);
	xWidget->layout()->addWidget(volMorphSizeSpinBox);

	xWidget=new QWidget(Gb6);
	xWidget->setLayout(new QHBoxLayout);
	Gb6->layout()->addWidget(xWidget);
	VolDilateButton=new QPushButton2(buttonList,"Dilate Vol",Gb6x);
	xWidget->layout()->addWidget(VolDilateButton);
	VolErodeButton=new QPushButton2(buttonList,"Erode Vol",Gb6x);
	xWidget->layout()->addWidget(VolErodeButton);


	//QSpinBox *volMorphInSpinBox,  *volMorphBackSpinBox,*volMorphMaxIterSpinBox;

	int kk=35;
	//int kk1=20;
	xWidget=new QWidget(Gb6);
	xWidget->setLayout(new QHBoxLayout);
	Gb6->layout()->addWidget(xWidget);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	

	xLabel=new QLabel("In:",xWidget);
	xLabel->setFixedWidth(15);
	xWidget->layout()->addWidget(xLabel);
	volMorphInSpinBox=new QSpinBox2(spinBoxList,xWidget);
	volMorphInSpinBox->setRange(-1,65535);
	volMorphInSpinBox->setValue(-1);
	volMorphInSpinBox->setFixedWidth(kk);

	xWidget->layout()->addWidget(volMorphInSpinBox);


	xLabel=new QLabel("Back:",xWidget);
	xLabel->setFixedWidth(25);
	xWidget->layout()->addWidget(xLabel);
	volMorphBackSpinBox=new QSpinBox2(spinBoxList,xWidget);
	volMorphBackSpinBox->setRange(-1,65535);
	volMorphBackSpinBox->setValue(-1);
	volMorphBackSpinBox->setFixedWidth(kk);
	xWidget->layout()->addWidget(volMorphBackSpinBox);

	xLabel=new QLabel("Iter:",xWidget);
		xLabel->setFixedWidth(30);
	xWidget->layout()->addWidget(xLabel);
	volMorphMaxIterSpinBox=new QSpinBox2(spinBoxList,xWidget);
	volMorphMaxIterSpinBox->setRange(0,65535);
	volMorphMaxIterSpinBox->setValue(1);
	volMorphMaxIterSpinBox->setFixedWidth(kk);
	xWidget->layout()->addWidget(volMorphMaxIterSpinBox);

	xWidget=new QWidget(Gb6);

	xWidget->setLayout(new QHBoxLayout);
	//	xWidget->layout()->setSpacing(1);
	//xWidget->layout()->setMargin(1);
	Gb6->layout()->addWidget(xWidget);
	xLabel=new QLabel("Bounds:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	volMorphBoundsCheckBox=new QCheckBox2(checkBoxList,xWidget);
	xWidget->layout()->addWidget(volMorphBoundsCheckBox);
		
	//xWidget=new QWidget(Gb6);
	//xWidget->setLayout(new QHBoxLayout);
	Gb6->layout()->addWidget(xWidget);
	xLabel=new QLabel("UseMask:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	volUseMaskCheckBox=new QCheckBox2(checkBoxList,xWidget);
	xWidget->layout()->addWidget(volUseMaskCheckBox);

	


	/*
		FillModeCombo = new QComboBox2(comboBoxList, FillOptionsBox );
	FillModeCombo->addAction(0,"OR   A|B");
	FillModeCombo->addAction(1,"CLR  A&!B");
	FillModeCombo->addAction(2,"AND  A&B");
	FillModeCombo->addAction(3,"NOR  !(A|B)");
	FillModeCombo->addAction(4,"NAND !(A&B)");
	FillModeCombo->addAction(5,"XOR  A^B");
	FillModeCombo->addAction(6,"CLR2 !A&B");
	*/




	Gb6=new	QGroupBox(Gb7);
	Gb6->setTitle("Sphere Shoot Segmentation");
	vbl_Gb7->addWidget(Gb6);
	vbl_Gbx = new QVBoxLayout;
	//vbl_Gbx->setSpacing(1);
	vbl_Gbx->setMargin(1);
	Gb6->setLayout(vbl_Gbx);
	SphereShootSegmentationButton=new QPushButton2(buttonList,"Sphere Segment!",Gb6);
	vbl_Gbx->addWidget(SphereShootSegmentationButton);

	Gb6x=new	QGroupBox(Gb6);
	vbl_Gbx->addWidget(Gb6x);


	//vbl_Gb7->addWidget(Gb6);


	hbl_Gb6 = new QHBoxLayout;
	hbl_Gb6->setSpacing(1);
	hbl_Gb6->setMargin(1);
	Gb6x->setLayout(hbl_Gb6);

	LM1 = new QLabel("Iters:",Gb6x);	
	hbl_Gb6->addWidget(LM1);
	SphereShootItersSpinBox=new QSpinBox2(spinBoxList,Gb6x);
	SphereShootItersSpinBox->setRange(0,100);
	SphereShootItersSpinBox->setValue(0);
	hbl_Gb6->addWidget(SphereShootItersSpinBox);


	Gb6x=new	QGroupBox(Gb6);
	vbl_Gbx->addWidget(Gb6x);
	hbl_Gb6 = new QHBoxLayout;
	hbl_Gb6->setSpacing(1);
	hbl_Gb6->setMargin(1);
	Gb6x->setLayout(hbl_Gb6);
	LM1 = new QLabel("Smallest:",Gb6x);	
	hbl_Gb6->addWidget(LM1);
	SphereShootMinSizeSpinBox=new QSpinBox2(spinBoxList,Gb6x);
	SphereShootMinSizeSpinBox->setRange(0,100);
	SphereShootMinSizeSpinBox->setValue(5);
	hbl_Gb6->addWidget(SphereShootMinSizeSpinBox);

	//MainLayout->addWidget(Gb1);
	//MainLayout->addWidget(Gb2);

	//MainLayout->addWidget(Gb4);
	//MainLayout->addWidget(Gb6);
	//MainLayout->addWidget(Gb7);

	//Gb1->setContentsMargins(5
	/*Gb1->setInsideSpacing(3);
	Gb1->setInsideMargin(5);
	Gb2->setInsideSpacing(3);
	Gb2->setInsideMargin(5);
	Gb4->setInsideSpacing(3);
	Gb4->setInsideMargin(5);
	Gb6->setInsideSpacing(3);
	Gb6->setInsideMargin(5);

	Gb7->setInsideSpacing(3);
	Gb7->setInsideMargin(5);*/

	//	MainLayout->addWidget(MaskOpacityBox);

	//MainLayout->addWidget(UndoRedoSegmentBox);

	//	MainLayout->addWidget(LoadSaveMaskBox);
	//MainLayout->addWidget(CalculateMaskVolumeButton);

	//	MainLayout->addWidget(GenerateMarchingCubesSurfaceButton);




	//qtab->addTab(SegmentationTab,"&Segmentation ");
}


//----------------------------------------------------

void GUIPanelWidget::SlotSetNewCameraRotations(double _Azimuth, double _Elevation,double _Roll)
{
	//return;
	Azimuth = _Azimuth;
	Elevation = _Elevation;
	Roll = _Roll;
	 

}
void GUIPanelWidget::SlotShowCurrentCameraAngles()
{
	TransformCameraAzimuthEdit->setText(QString::number(Azimuth));
TransformCameraElevationEdit->setText(QString::number(Elevation));
TransformCameraRollEdit->setText(QString::number(Roll));
}
//-----------------------------------

void GUIPanelWidget::SlotShowInfo(int Nx,int Ny,int Nz,double Xscale,double Yscale,double Zscale,const QString &name )
{
	QString Ntemp,Txt,tt;

	LabelFileName->setText(name);

	Ntemp.setNum(Nx);
	Txt="Nx: ";
	Txt.append( Ntemp);
	LabelNx->setText(Txt);


	Ntemp.setNum(Ny);
	Txt="Ny: ";
	Txt.append( Ntemp);
	LabelNy->setText(Txt);

	Ntemp.setNum(Nz);
	Txt="Nz: ";
	Txt.append( Ntemp);
	LabelNz->setText(Txt);


	Ntemp.setNum(Xscale);
	Txt="Sx: ";
	Txt.append( Ntemp);
	Txt.truncate(10);
	//	SdvX->setText(Ntemp);
	//	Txt.append( " mm");
	LabelXscale->setText(Txt);


	Ntemp.setNum(Yscale);
	Txt="Sy: ";
	Txt.append( Ntemp);
	Txt.truncate(10);
	//	SdvY->setText(Ntemp);
	//	Txt.append( " mm");
	LabelYscale->setText(Txt);


	Ntemp.setNum(Zscale);
	Txt="Sz: ";
	Txt.append( Ntemp);
	Txt.truncate(10);
	//	SdvZ->setText(Ntemp);
	//	Txt.append( " mm");
	LabelZscale->setText(Txt);


	//int n[3]={Nx,Ny,Nz};
	/*int n[3]={10000,10000,10000 };
	int pos=0;
	int i;
	for(i=0;i<3;i++)
	{	
		planeSliders[pos]->setRange(0,n[i]-1);
		planeSliders[pos++]->setValue(0);

		planeSliders[pos]->setRange(1,n[i]);
		planeSliders[pos++]->setValue(n[i]);
	}

	*/
	int n[3] = { Nx,Ny,Nz };
	//data->GetN(n);
	int bounds[] = { 0,n[0] - 1,0,n[1] - 1,0,n[2] - 1 };

	
	for (int i = 0; i < 6; i++)
	{
		int val = n[i / 2];
		planeSliders[i]->setRange(0, val);
	}

	for (int i = 0; i < 6; i++)
	{
		planeSliders[i]->setValue(bounds[i]);
	}

	double bb[6];
	for (int i = 0; i < 6; i++)
	{
		bb[i] = planeSliders[i]->value();
	}

}

//-------------------------------------------------------------

void  GUIPanelWidget::GetMarchingCubesSettings()
{
	emit SendMarchingCubesSettings(GaussCheck->isChecked(),
		(SdvX->text()).toFloat(),(SdvY->text()).toFloat(),(SdvZ->text()).toFloat(),
		DecimateCheck->isChecked(),(Tred->text()).toFloat(),SmoothingCheck->isChecked(),ConnectCheck->isChecked());


}

//-------------------------------------------------------------

void GUIPanelWidget::CreatePaletteTab(MyDockWidget *qtab)
{

//	QWidget *PaletteTab = new QWidget(qtab);
//	QVBoxLayout* MainLayout= new QVBoxLayout( PaletteTab, 10, -1, "MainLayout");	
//
//
//
//
//
//
//
//
//	//---------------------------------
//
//
//
//
//	//Q3GroupBox *Gb3=new Q3GroupBox(10,Vertical,PaletteTab);
//	QGroupBox *Gb3=new	QGroupBox(PaletteTab);
//	Gb3->setLayout(new QVBoxLayout);
//
//	QLabel *L4R = new QLabel("Section X Size:",Gb3);	
//	InspectionXSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3, "InspectionXSizeSlider");
//	QLabel *L5R = new QLabel("Section Y Size:",Gb3);	
//	InspectionYSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3, "InspectionYSizeSlider");
//	QLabel *L6R = new QLabel("Section Z Size:",Gb3);	
//	InspectionZSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3, "InspectionZSizeSlider");
//
//
//	connect(InspectionXSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));
//	connect(InspectionYSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));
//	connect(InspectionZSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));
//
//
//	QWidget *Gb31=new QWidget(Gb3);
//	CropDatasetButton   =	new    QPushButton2(buttonList,"Crop",Gb31);
//	RemovePartButton =	new    QPushButton2(buttonList,"Fill Part",Gb31);
//	FlipXButton =	new    QPushButton2(buttonList,"FlipX",Gb31);
//
//	QGroupBox *Gb32=new QGroupBox(Gb3);
//	Gb32->setLayout(new QVBoxLayout);
//
//	ModelObjectCombineButton=	new    QPushButton2(buttonList,"Model Object Fussion",Gb32);
//	ModelObjectDistanceMapButton=	new    QPushButton2(buttonList,"Model Object DistMap",Gb32);
//	DeleteDistanceMapButton=	new    QPushButton2(buttonList,"Delete DistMap",Gb32);
//
//
//
//
//	QValidator *ValidatorInt= new QIntValidator(0,65535,this);	
//	QWidget *DistThreshBox=new QWidget(Gb32);
//
//	QLabel *LThreshLabel=new QLabel("LTh:",DistThreshBox);
//	LThreshLineEdit=new QLineEdit2(lineEditList,DistThreshBox);
//	LThreshLineEdit->setValidator(ValidatorInt);
//	LThreshLineEdit->setText("0");
//
//	QLabel *RThreshLabel=new QLabel("RTh:",DistThreshBox);
//	RThreshLineEdit=new QLineEdit2(lineEditList,DistThreshBox);
//	RThreshLineEdit->setValidator(ValidatorInt);
//	RThreshLineEdit->setText("0");
//
//	//
//
//
//
//	GenerateThresholdedMCSurfaceButton = new QPushButton2(buttonList,"Generate Th Surface",Gb32);
//	QWidget *AverageGridBox=new QWidget(Gb32);
//	AverageDataSetsButton = new QPushButton2(buttonList,"Average",AverageGridBox);
//	QLabel *Lxx=new QLabel(" Amount:",AverageGridBox);
//	QValidator *Validatordd =new TextFieldDoubleValidator(0.001,1.0,20,this);
//	AverageAmountEdit=new QLineEdit2(lineEditList,AverageGridBox);
//	AverageAmountEdit->setText("0.0");
//	AverageAmountEdit->setValidator(Validatordd);
//
//	AverageDataSetsButton->setMinimumWidth(60);
//
//	//
//
//	CreateSliceThroughPolylineCheckBox =new QCheckBox2(checkBoxList,Gb3);
//	CreateSliceThroughPolylineCheckBox->setText("Create Slice Through PolyLine");
//	CreateSliceThroughPolylineCheckBox->setChecked(false);	
//
//
//	CropDatasetButton->setMinimumWidth(30);
//	RemovePartButton->setMinimumWidth(30);
//
//
////-------------------------------
//
//	
//	QGroupBox *Gb33=new QGroupBox(Gb3);Gb33->setLayout(new QVBoxLayout);
//	Gb33->setTitle("Cycle DataSets");
//
//	QWidget *CycleBox=new QWidget(Gb33);
//
//	StartCycleDatasetViewsButton =	new    QPushButton2(buttonList,"Start",CycleBox);
//	PauseCycleDatasetViewsButton =	new    QPushButton2(buttonList,"Pause",CycleBox);
//	StopCycleDataSetViewsButton =	new    QPushButton2(buttonList,"Stop",CycleBox);
//	StartCycleDatasetViewsButton->setMinimumWidth(40);
//	StopCycleDataSetViewsButton->setMinimumWidth(40);
//	PauseCycleDatasetViewsButton->setMinimumWidth(40);
//
//	QWidget *CycleBox2=new QWidget(Gb33);
//	QLabel *LL1=new QLabel("Nr:",CycleBox2);
//	NrOfCycleSetsBox = new QSpinBox2(spinBoxList,1,511,1,CycleBox2);
//	NrOfCycleSetsBox->setValue(1);
//	NrOfCycleSetsBox->setMaximumWidth(35);
//	QValidator *Validator =new QIntValidator(1,10000,this);
//	QLabel *TCycleLabel = new QLabel("t: ",CycleBox2);
//	CyclePeriodEdit=new QLineEdit2(lineEditList,CycleBox2);
//	CyclePeriodEdit->setValidator(Validator);
//	//CyclePeriodEdit->setMaximumWidth(100);
//	CyclePeriodEdit->setText("80");
//	CycleGrabCheckBox= new QCheckBox2(checkBoxList,CycleBox2);
//	CycleGrabCheckBox->setText("grab");
//
//
//	
//
//	//QHBox *Gb31=new QHBox(Gb3);
//	//	QHBox *SizeBox=new QHBox(Gb2);
//	//RemovePartButton =	new    QPushButton2(buttonList,"Fill Part",Gb31);
//	//FlipXButton =	new    QPushButton2(buttonList,"FlipX",Gb31);
//
////--------------------------------
//
//
//	qtab->addTab(PaletteTab,"&Other ");
//
//
//	//	 QString aa,tmp;
//
//
//
//
//	MainLayout->addWidget(Gb3);

QWidget *PaletteTab = new QWidget(qtab);
qtab->setScrollWidget(PaletteTab);
	//QVBoxLayout* MainLayout= new QVBoxLayout( PaletteTab, 10, -1, "MainLayout");	

	QVBoxLayout* MainLayout= new QVBoxLayout( PaletteTab);//, 10, -1, "MainLayout");	
	MainLayout->setSpacing(1);
	PaletteTab->setLayout(MainLayout);




	QLabel *l1;
	QGroupBox *Gb3 = new	QGroupBox(PaletteTab);
	//---------------------------------
	Gb3->setLayout(new QVBoxLayout());
	Gb3->layout()->setSpacing(1);
	
	QString kNames[] = { "angle", "minDepth", "maxDepth", "lightThresh", "xmin", "xmax", "ymin", "ymax", "bckgZoom", "exposure", "focus", "wb", "minBlobSize", "maxBlobSize" };
	for (int i = 0; i < NUMBER_OF_KIN_SLIDERS; i++)
	{
		//l1 = new QLabel("Kinect Param: "+QString::number(i), Gb3);
		l1 = new QLabel(kNames[i], Gb3);
		kinectSlider[i] = new   QSlider2(sliderList,Qt::Horizontal, Gb3);
		Gb3->layout()->addWidget(l1);
		Gb3->layout()->addWidget(kinectSlider[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		kinectLabel[i]=new QLabel(".. " + QString::number(i), Gb3);
		Gb3->layout()->addWidget(kinectLabel[i]);
	
	}

	kinectSlider[0]->setMinimum(-30);
	kinectSlider[0]->setMaximum(30);
	kinectSlider[0]->setValue(0);
	MainLayout->addWidget(Gb3);


	kinectSlider[1]->setMinimum(0); kinectSlider[1]->setMaximum(4000); kinectSlider[1]->setValue(0);
	kinectSlider[2]->setMinimum(0); kinectSlider[2]->setMaximum(4000); kinectSlider[2]->setValue(10000);
	kinectSlider[3]->setMinimum(1); kinectSlider[3]->setMaximum(255); kinectSlider[2]->setValue(128);

	//kinectSlider[8]->setMinimum(-100); kinectSlider[3]->setMaximum(255); kinectSlider[2]->setValue(128);

	for (int i = 4; i < NUMBER_OF_KIN_SLIDERS; i++)
	{
		kinectSlider[i]->setMinimum(0); kinectSlider[i]->setMaximum(10000); kinectSlider[1]->setValue(0);
	}

	//kinectSlider[8]->setMinimum(0); kinectSlider[8]->setMaximum(10000); kinectSlider[8]->setValue(5000);

	QWidget *w1;
	QLabel *label;
	w1 = new QWidget(Gb3); w1->setLayout(new QHBoxLayout());	Gb3->layout()->addWidget(w1); w1->layout()->setMargin(1);
	label = new QLabel(w1); label->setText("PreFilter:"); w1->layout()->addWidget(label);
	kinectImagePrefilterComboBox = new QComboBox2(comboBoxList,w1); w1->layout()->addWidget(kinectImagePrefilterComboBox);


	w1 = new QWidget(Gb3); w1->setLayout(new QHBoxLayout());	Gb3->layout()->addWidget(w1); w1->layout()->setMargin(1);


	// phase one
	//QLabel *label;
	QString names[] = { "Kinect", "Filtering","VolumeMask","Time","useBackground","Blobs","Rgb","Stop","FlipYCam","KinXFilip","KinYFlip","KinDFlip","KinDTop" };
	//label = new QLabel(w1); label->setText("phase0"); w1->layout()->addWidget(label);
	for (int i = 0; i < NUMBER_OF_KIN_CHECKBOXES; i++)
	{
		kinectOptionsCheckBox[i] = new QCheckBox2(checkBoxList,w1); w1->layout()->addWidget(kinectOptionsCheckBox[i]);
		kinectOptionsCheckBox[i]->setText(names[i]);
		if (i % 2==0)
		{
			w1 = new QWidget(Gb3); w1->setLayout(new QHBoxLayout());	Gb3->layout()->addWidget(w1); w1->layout()->setMargin(1);

		}
	}


	

	w1 = new QWidget(Gb3); w1->setLayout(new QHBoxLayout());	Gb3->layout()->addWidget(w1); w1->layout()->setMargin(1);
	kinectGrabBackgroundFrameButton = new QPushButton2(buttonList,"GetBackground", w1); w1->layout()->addWidget(kinectGrabBackgroundFrameButton);
	w1 = new QWidget(Gb3); w1->setLayout(new QHBoxLayout());	Gb3->layout()->addWidget(w1); w1->layout()->setMargin(1);
	kinectFrustumButton = new QPushButton2(buttonList,"FrustumBox", w1); w1->layout()->addWidget(kinectFrustumButton);
	w1 = new QWidget(Gb3); w1->setLayout(new QHBoxLayout());	Gb3->layout()->addWidget(w1); w1->layout()->setMargin(1);
	kinectCalibrateButton = new QPushButton2(buttonList,"CalibrateDepthMap", w1); w1->layout()->addWidget(kinectCalibrateButton);
	w1 = new QWidget(Gb3); w1->setLayout(new QHBoxLayout());	Gb3->layout()->addWidget(w1); w1->layout()->setMargin(1);
	kinectGrabBackgroundDepthButton = new QPushButton2(buttonList,"GrabDepthMap", w1); w1->layout()->addWidget(kinectGrabBackgroundDepthButton);

	

	

	//Q3GroupBox *Gb3=new Q3GroupBox(10,Vertical,PaletteTab);
	Gb3=new	QGroupBox(PaletteTab);
	

//	QVBoxLayout *vbl_Gb3 = new QVBoxLayout;
	//vbl_Gb3->setSpacing(1);
	Gb3->setLayout(new QVBoxLayout());
	Gb3->layout()->setSpacing(1);

	QLabel *L4R = new QLabel("Section X Size:",Gb3);		
	
	InspectionXSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3);
	QLabel *L5R = new QLabel("Section Y Size:",Gb3);	
	InspectionYSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3);
	QLabel *L6R = new QLabel("Section Z Size:",Gb3);	
	InspectionZSizeSlider	= new QSlider2(sliderList,Qt::Horizontal, Gb3);

	Gb3->layout()->addWidget(L4R);
	Gb3->layout()->addWidget(InspectionXSizeSlider);
	Gb3->layout()->addWidget(L5R);
	Gb3->layout()->addWidget(InspectionYSizeSlider);
	Gb3->layout()->addWidget(L6R);
	Gb3->layout()->addWidget(InspectionZSizeSlider);

	connect(InspectionXSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));
	connect(InspectionYSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));
	connect(InspectionZSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotChangeInspectionWindowSize(int)));


	//QWidget *Gb31=new QWidget(Gb3);
	QWidget *Gb31=new	QWidget(Gb3);
	Gb3->layout()->addWidget(Gb31);
	QHBoxLayout *hbl_Gb31 = new QHBoxLayout;
	hbl_Gb31->setSpacing(0);
	hbl_Gb31->setMargin(1);
	Gb31->setLayout(hbl_Gb31);
	CropDatasetButton   =	new    QPushButton2(buttonList,"Crop",Gb31);
	RemovePartButton =	new    QPushButton2(buttonList,"Fill Part",Gb31);
	FlipXButton =	new    QPushButton2(buttonList,"FlipX",Gb31);

	hbl_Gb31->addWidget(CropDatasetButton);
	hbl_Gb31->addWidget(RemovePartButton);
	hbl_Gb31->addWidget(FlipXButton);

	//Q3GroupBox *Gb32=new Q3GroupBox(6,Vertical,Gb3);
	QGroupBox *Gb32=new	QGroupBox(Gb3);
	Gb3->layout()->addWidget(Gb32);
	QVBoxLayout *hbl_Gb32 = new QVBoxLayout;
	hbl_Gb32->setSpacing(1);
	hbl_Gb32->setMargin(1);
	Gb32->setLayout(hbl_Gb32);


	ModelObjectCombineButton=	new    QPushButton2(buttonList,"Model Object Fussion",Gb32);
	ModelObjectDistanceMapButton=	new    QPushButton2(buttonList,"Model Object DistMap",Gb32);
	DeleteDistanceMapButton=	new    QPushButton2(buttonList,"Delete DistMap",Gb32);
	hbl_Gb32->addWidget(ModelObjectCombineButton);
	hbl_Gb32->addWidget(ModelObjectDistanceMapButton);
	hbl_Gb32->addWidget(DeleteDistanceMapButton);

	QValidator *ValidatorInt= new QIntValidator(0,65535,this);	
	//QWidget *DistThreshBox=new QWidget(Gb32);
	QWidget *DistThreshBox=new	QWidget(Gb32);
	QHBoxLayout *hbl_DistThreshBox = new QHBoxLayout;
	hbl_DistThreshBox->setSpacing(0);
	hbl_DistThreshBox->setMargin(1);
	DistThreshBox->setLayout(hbl_DistThreshBox);
	hbl_Gb32->addWidget(DistThreshBox);

	QLabel *LThreshLabel=new QLabel("LTh:",DistThreshBox);
	LThreshLineEdit=new QLineEdit2(lineEditList,DistThreshBox);
	LThreshLineEdit->setValidator(ValidatorInt);
	LThreshLineEdit->setText("0");

	QLabel *RThreshLabel=new QLabel("RTh:",DistThreshBox);
	RThreshLineEdit=new QLineEdit2(lineEditList,DistThreshBox);
	RThreshLineEdit->setValidator(ValidatorInt);
	RThreshLineEdit->setText("0");

	hbl_DistThreshBox->addWidget(LThreshLabel);
	hbl_DistThreshBox->addWidget(LThreshLineEdit);

	hbl_DistThreshBox->addWidget(RThreshLabel);
	hbl_DistThreshBox->addWidget(RThreshLineEdit);

	QPushButton2 *setMemorizetThresholdsButton;

	setMemorizedThresholdsButton = new QPushButton2(buttonList, "SetThresholds", Gb32);
	hbl_DistThreshBox->addWidget(setMemorizedThresholdsButton);
	//



	GenerateThresholdedMCSurfaceButton = new QPushButton2(buttonList,"Generate Th Surface",Gb32);
	//QWidget *AverageGridBox=new QWidget(Gb32);
	hbl_Gb32->addWidget(GenerateThresholdedMCSurfaceButton);
	QWidget *AverageGridBox=new	QWidget(Gb32);
	QHBoxLayout *hbl_AverageGridBox = new QHBoxLayout;
	hbl_AverageGridBox->setSpacing(0);
	hbl_AverageGridBox->setMargin(1);
	AverageGridBox->setLayout(hbl_AverageGridBox);
	hbl_Gb32->addWidget(AverageGridBox);

	AverageDataSetsButton = new QPushButton2(buttonList,"Average",AverageGridBox);
	QLabel *Lxx=new QLabel(" Amount:",AverageGridBox);
	TextFieldDoubleValidator *Validatordd =new TextFieldDoubleValidator(0.001,1.0,20,this);
	Validatordd->setNotation(TextFieldDoubleValidator::StandardNotation);
	AverageAmountEdit=new QLineEdit2(lineEditList,AverageGridBox);
	AverageAmountEdit->setText("0.0");
	AverageAmountEdit->setValidator(Validatordd);

	AverageDataSetsButton->setMinimumWidth(60);

	//

	CreateSliceThroughPolylineCheckBox =new QCheckBox2(checkBoxList,Gb3);
	Gb3->layout()->addWidget(CreateSliceThroughPolylineCheckBox);
	CreateSliceThroughPolylineCheckBox->setText("Create Slice Through PolyLine");
	CreateSliceThroughPolylineCheckBox->setChecked(false);	
	CreateSliceThroughPolylineCheckBox->hide();



	CropDatasetButton->setMinimumWidth(30);
	RemovePartButton->setMinimumWidth(30);

	hbl_AverageGridBox->addWidget(AverageDataSetsButton);
	hbl_AverageGridBox->addWidget(Lxx);
	hbl_AverageGridBox->addWidget(AverageAmountEdit);

	//-------------------------------


	//Q3GroupBox *Gb33=new Q3GroupBox(6,Vertical,Gb3);
	QGroupBox *Gb33=new	QGroupBox(Gb3);
	QVBoxLayout *vbl_Gb33 = new QVBoxLayout;
	Gb33->setLayout(vbl_Gb33);	
	Gb33->layout()->setSpacing(0);
	Gb33->layout()->setMargin(1);

	Gb3->layout()->addWidget(Gb33);

	Gb33->setWindowTitle("Cycle DataSets");

	//QWidget *CycleBox=new QWidget(Gb33);
	QWidget *CycleBox=new	QWidget(Gb33);
	QHBoxLayout *hbl_CycleBox = new QHBoxLayout;
	CycleBox->setLayout(hbl_CycleBox);

	vbl_Gb33->addWidget(CycleBox);

	StartCycleDatasetViewsButton =	new    QPushButton2(buttonList,"Start",CycleBox);
	PauseCycleDatasetViewsButton =	new    QPushButton2(buttonList,"Pause",CycleBox);
	StopCycleDataSetViewsButton =	new    QPushButton2(buttonList,"Stop",CycleBox);
	StartCycleDatasetViewsButton->setMinimumWidth(40);
	StopCycleDataSetViewsButton->setMinimumWidth(40);
	PauseCycleDatasetViewsButton->setMinimumWidth(40);

	hbl_CycleBox->addWidget(StartCycleDatasetViewsButton);
	hbl_CycleBox->addWidget(PauseCycleDatasetViewsButton);
	hbl_CycleBox->addWidget(StopCycleDataSetViewsButton);

	//	QWidget *CycleBox2=new QWidget(Gb33);
	QWidget *CycleBox2=new	QWidget(Gb33);
	QHBoxLayout *hbl_CycleBox2 = new QHBoxLayout;
	hbl_CycleBox2->setSpacing(0);
	hbl_CycleBox2->setMargin(1);
	CycleBox2->setLayout(hbl_CycleBox2);
	vbl_Gb33->addWidget(CycleBox2);

	QLabel *LL1=new QLabel("Nr:",CycleBox2);
	NrOfCycleSetsBox = new QSpinBox2(spinBoxList,CycleBox2);
	NrOfCycleSetsBox->setRange(1,511);
	NrOfCycleSetsBox->setValue(1);
	NrOfCycleSetsBox->setMaximumWidth(35);
	QValidator *Validator =new QIntValidator(1,10000,this);
	QLabel *TCycleLabel = new QLabel("t: ",CycleBox2);
	CyclePeriodEdit=new QLineEdit2(lineEditList,CycleBox2);
	CyclePeriodEdit->setValidator(Validator);
	//CyclePeriodEdit->setMaximumWidth(100);
	CyclePeriodEdit->setText("80");
	CycleGrabCheckBox= new QCheckBox2(checkBoxList,CycleBox2);
	CycleGrabCheckBox->setText("grab");

	hbl_CycleBox2->addWidget(LL1);
	hbl_CycleBox2->addWidget(NrOfCycleSetsBox);
	hbl_CycleBox2->addWidget(TCycleLabel);
	hbl_CycleBox2->addWidget(CyclePeriodEdit);
	hbl_CycleBox2->addWidget(CycleGrabCheckBox);


	//QHBox *Gb31=new QHBox(Gb3);
	//	QHBox *SizeBox=new QHBox(Gb2);
	//RemovePartButton =	new    QPushButton2(buttonList,"Fill Part",Gb31);
	//FlipXButton =	new    QPushButton2(buttonList,"FlipX",Gb31);

	//--------------------------------


	//qtab->addTab(PaletteTab,"&Other ");


	//	 QString aa,tmp;




	MainLayout->addWidget(Gb3);

}


//-------------------------------------------------------------

void GUIPanelWidget::SlotSetInspectionRanges(int X,int Y,int Z)
{

	InspectionXSizeSlider->setRange(2,X);
	InspectionYSizeSlider->setRange(2,Y);
	InspectionZSizeSlider->setRange(2,Z);

	InspectionXSizeSlider->setValue((int)(0.5*X));
	InspectionYSizeSlider->setValue((int)(0.5*Y));
	InspectionZSizeSlider->setValue((int)(0.5*Z));
}
//-------------------------------------------------------------

void GUIPanelWidget::SlotSetInspectionRanges(int X,int Y,int Z,int val1,int val2,int val3)
{

	InspectionXSizeSlider->setRange(2,X);
	InspectionYSizeSlider->setRange(2,Y);
	InspectionZSizeSlider->setRange(2,Z);

	InspectionXSizeSlider->setValue(val1);
	InspectionYSizeSlider->setValue(val2);
	InspectionZSizeSlider->setValue(val3);
}

//-------------------------------------------------------------

void GUIPanelWidget::SlotChangeActualSurface(int Nr)
{

	//SurfaceNameEdit->setText(SurfaceListBox->itemText(Nr));
	emit SendNewActualSurface(Nr);
}

//-------------------------------------------------------------

void GUIPanelWidget::SlotSetNewName( const QString &Name)
{

	//SurfaceListBox->changeItem(Name,SurfaceListBox->currentIndex());
}


//-------------------------------------------------------------

void GUIPanelWidget::SlotSetPrevPaletteR()
{

	if (PaletteComboR->currentIndex()>0) PaletteComboR->setCurrentIndex(PaletteComboR->currentIndex()-1);

	emit SetNewPaletteR(PaletteComboR->currentIndex());
}

//-------------------------------------------------------------

void GUIPanelWidget::SlotSetNextPaletteR()
{

	if (PaletteComboR->currentIndex()<PalNr) PaletteComboR->setCurrentIndex(PaletteComboR->currentIndex()+1);

	emit SetNewPaletteR(PaletteComboR->currentIndex());
}

//-------------------------------------------------------------

void GUIPanelWidget::SlotSetPaletteItemR(int Item)
{

	if ((Item>=0)&&(Item<PalNr))
		PaletteComboR->setCurrentIndex(Item);

}


//-------------------------------------------------------------

void GUIPanelWidget::SlotSetPrevPalette()
{

	if (PaletteCombo->currentIndex()>0) PaletteCombo->setCurrentIndex(PaletteCombo->currentIndex()-1);

	emit SetNewPalette(PaletteCombo->currentIndex());
}

//-------------------------------------------------------------

void GUIPanelWidget::SlotSetNextPalette()
{

	if (PaletteCombo->currentIndex()<PalNr) PaletteCombo->setCurrentIndex(PaletteCombo->currentIndex()+1);

	emit SetNewPalette(PaletteCombo->currentIndex());
}

//-------------------------------------------------------------

void GUIPanelWidget::SlotSetPaletteItem(int Item)
{

	if ((Item>=0)&&(Item<PalNr))
		PaletteCombo->setCurrentIndex(Item);

}

//-------------------------------------------------------------

void GUIPanelWidget::SlotSetNewOpacity()
{
	emit SendOpacity((unsigned char )OpacitySlider->value());
}
//--------------------------------------------------
void GUIPanelWidget::SlotSetNewMaskOpacity()
{

	emit SendMaskOpacity((unsigned char )MaskOpacitySlider->value());

}

//---------------------------------------------------------------

void GUIPanelWidget::SlotChangeInspectionWindowSize(int a)
{

	emit SignalSendInspectionWindowSize(InspectionXSizeSlider->value(),InspectionYSizeSlider->value(),InspectionZSizeSlider->value());

}

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//------------ RAW DATA SET ITEMS -------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------

//void GUIPanelWidget:: SlotInsertRawDataItem(const QString &name)
//{
//
//	QListViewItem *Item=new QListViewItem( RawDataListView,RawDataListView->currentIndex(),name,"o","+","+","-","-" );
//
//	RawDataListView->setCurrentIndex(RawDataListView->currentIndex()->nextSibling());
//	RawDataListView->setSelected(RawDataListView->currentIndex(),true); 		
//
//
//	//	if (InsertAsModelCheckBox->isChecked())
//	//	{
//	CurrentItem=Item;
//	ModelItem=CurrentItem;
//	View3DItem=CurrentItem;
//	emit	SignalSetNew3DViewDataSet();
//	//InspectionItem=NULL;
//	View2DItems=NULL;
//	InspectionItems=NULL;
//	SlotCurrentRawDataItemChanged(ModelItem);
//	//	}
//	//	else
//	//	{
//
//	//tu trzeba dopisac przypadek kiedy wstawiany zbior nie staje sie automatycznie modelem
//
//	// w sumie chyba nic nie musze pisac...i git
//	//	}
//
//	SlotUpdateItemState();
//
//}

////---------------------------------------------------------------
//
void GUIPanelWidget:: SlotDeleteRawDataItem()
{
	emit SignalDeleteRawDataItem(this->rawDataTreeWidget->currentItem());


}
//
////---------------------------------------------------------------
//
//
////---------------------------------------------------------------
//
//void GUIPanelWidget:: SlotSetNewDataSet( )
//{
//
//	this->emit SignalSetNewDataSet(item);
//
//
//}
//
//
////---------------------------------------------------------------
//
//
//void GUIPanelWidget::SlotSetNewDataSetByDoubleClick(QListViewItem *Item )
//{
//
//	if (CurrentItem!=Item)
//	{
//
//
//
//		emit SignalSetNewDataSet(); 
//		CurrentItem=Item;
//		ModelItem=CurrentItem;
//		//		InspectionItem=NULL;//noew
//
//
//
//		SlotUpdateItemState();
//	}
//
//
//}
//
//
////---------------------------------------------------------------
//
//
//void GUIPanelWidget::SlotListRightMouseButtonRMB( QListViewItem* Item, const QPoint & point, int )
//{
//	int i;
//	bool Ena;
//
//	if( Item )
//	{
//		(Item==ModelItem)?ItemAttributesMenu->setItemChecked(0,true):ItemAttributesMenu->setItemChecked(0,false);
//
//		Ena=(Item!=ModelItem)?true:false;
//
//		ItemAttributesMenu->setItemEnabled(1,Ena);
//
//		Ena=(Item->text(1)=="C")?true:false;
//		//	ItemAttributesMenu->setItemEnabled(3,Ena);	
//		for(i=0;i<NumberOfCompatibleItems*2;i++)
//			ItemAttributesMenu->setItemEnabled(3+i,Ena);
//
//		(Item==ObjectItem)?ItemAttributesMenu->setItemChecked(1,true):ItemAttributesMenu->setItemChecked(1,false);
//		(Item==View3DItem)?ItemAttributesMenu->setItemChecked(2,true):ItemAttributesMenu->setItemChecked(2,false);
//		//	(Item==InspectionItem)?ItemAttributesMenu->setItemChecked(3,true):ItemAttributesMenu->setItemChecked(3,false);
//
//		for(i=0;i<NumberOfCompatibleItems;i++)
//		{
//			(Item==(View2DItems[i].Item))?ItemAttributesMenu->setItemChecked(3+i,true):ItemAttributesMenu->setItemChecked(3+i,false);
//			(Item==(InspectionItems[i].Item))?ItemAttributesMenu->setItemChecked(3+i+NumberOfCompatibleItems,true):ItemAttributesMenu->setItemChecked(3+i+NumberOfCompatibleItems,false);
//
//		}
//		ItemAttributesMenu->popup( point );
//		ItemAttributesMenu->show();
//	}
//
//}
//
//
////---------------------------------------------------------------
//
//
//void GUIPanelWidget::SlotUpdateItemState()
//{
//
//	// w razie zmiany main lub 
//	// w razie dodania/usuniecia 
//	// sprawdz czy mozliwe sa View2 i inspection i zaznacz ew main
//
//	emit SignalGetModelCompatibilityArray();
//
//
//}
//
////---------------------------------------------------------------
//
//void GUIPanelWidget::SlotSetAttributesToItemsCompatibleWithModel(int *CompatibilityArray)
//{
//
//	if(RawDataListView-> firstChild()!=NULL)
//	{
//
//		QListViewItem *Item=RawDataListView->selectedItem();
//
//		QListViewItem *StepItem=RawDataListView-> firstChild();
//
//		int i;
//		int pos=0;
//		int TEMP=0;
//		NumberOfCompatibleItems=0;
//		while(TEMP==0)
//		{
//			if (ObjectItem==Item)
//			{
//				ObjectItem=NULL;
//				Item->setText(2,"-");
//			}
//
//			if(StepItem!=ModelItem)
//			{
//				if(CompatibilityArray[pos++])
//				{
//					StepItem->setText(1,"C");
//					StepItem->setText(4,"+");
//					StepItem->setText(5,"+");
//					NumberOfCompatibleItems++;
//				}
//				else
//				{
//					StepItem->setText(1,"+");
//					StepItem->setText(4,"-");
//					StepItem->setText(5,"-");
//				}
//
//			}
//			else
//			{
//				pos++;
//				StepItem->setText(1,"o");
//				StepItem->setText(3,"o");
//				StepItem->setText(4,"-");
//				StepItem->setText(5,"-");
//			}
//
//			(StepItem==View3DItem)?StepItem->setText(3,"o"):StepItem->setText(3,"+");
//			if(StepItem!=RawDataListView->lastItem()) 
//				StepItem=StepItem->nextSibling();
//			else
//				TEMP=1;
//
//		}	
//
//		//trzeba zrobic updaet menusow
//		ItemAttributesMenu->clear();
//		ItemAttributesMenu->addAction("Main/Model",0);
//		ItemAttributesMenu->addAction("Object",1);
//		ItemAttributesMenu->addAction("View 3D",2);
//
//
//		if (View2DItems!=NULL) 	
//			delete View2DItems;
//		View2DItems=new View2DItemArray[NumberOfCompatibleItems];
//
//
//		if (InspectionItems!=NULL) 	
//			delete InspectionItems;
//		InspectionItems=new View2DItemArray[NumberOfCompatibleItems];
//
//
//		if (NumberOfCompatibleItems>0)
//		{
//
//			//ItemAttributesMenu->addAction("InspWindow",3);
//			QString temp1,temp2;
//			for(i=1;i<=NumberOfCompatibleItems;i++)
//			{
//				temp1="View 2D nr.";
//				temp2.setNum(i);
//				temp1.append(temp2);
//				ItemAttributesMenu->addAction(temp1,2+i);
//				View2DItems[i-1].Item=NULL;
//
//
//			}
//
//			for(i=1;i<=NumberOfCompatibleItems;i++)
//			{
//				temp1="Insp View nr.";
//				temp2.setNum(i);
//				temp1.append(temp2);
//				ItemAttributesMenu->addAction(temp1,2+i+NumberOfCompatibleItems);
//				InspectionItems[i-1].Item=NULL;
//
//
//			}
//		}
//	}
//}
//
//
////---------------------------------------------------------------
//
//void GUIPanelWidget::SlotChangeAtributes(int Pos)
//{
//
//	//tu trzeba bedzie powysylac troszke sygnalow...do main window zeby pozmieniac role zbiorow danych...
//	int nr=ItemAttributesMenu->count();
//	bool ch[10];
//	for(int i=0;i<nr;i++)
//	{
//		ch[i]=ItemAttributesMenu->isItemChecked(i);
//	}
//
//	if((ItemAttributesMenu->isItemChecked(Pos)))
//	{
//		QListViewItem *Item=RawDataListView->currentIndex();
//
//		switch(Pos)
//		{
//		case 0:
//
//			ModelItem=Item;
//			SlotSetNewDataSet();
//
//			break;
//
//		case 1:
//			if(ObjectItem!=NULL) ObjectItem->setText(2,"+");
//			ObjectItem=Item ;
//			Item->setText(2,"o");
//
//
//			emit	SignalSetNewObjectDataSet();
//
//			break;
//
//		case 2:
//			if(View3DItem!=NULL) View3DItem->setText(3,"+");
//			View3DItem=Item;
//			Item->setText(3,"o");
//
//
//			emit	SignalSetNew3DViewDataSet();
//
//
//
//
//			break;
//
//			/*	case 3:
//			if(InspectionItem!=NULL) InspectionItem->setText(4,"+");
//			InspectionItem=RawDataListView->selectedItem () ;
//			Item->setText(4,"o");
//			emit	SignalSetNewInspectionDataSet();
//
//			break;
//			*/
//
//
//
//
//
//		}
//
//
//
//		if ((Pos>2)&&(Pos<2+NumberOfCompatibleItems+1)) //to w sumie jest nie potrzebne
//		{
//			if(View2DItems[Pos-3].Item!=NULL) View2DItems[Pos-3].Item->setText(5,"+");
//
//
//
//			int i;
//			for(i=0;i<NumberOfCompatibleItems;i++)
//				if(ItemAttributesMenu->isItemChecked(i+3))
//				{
//					ItemAttributesMenu->setItemChecked(Pos,false);
//					View2DItems[i].Item=NULL;
//
//
//				}
//
//				QString TempS;
//				TempS.setNum(Pos-2); //bo numerki wyswietlam od 1
//				View2DItems[Pos-3].Item=Item;
//				Item->setText(5,TempS);
//
//				// teraz trzeba przeleciec wszsytkie zbiory i wpisac pozycje na ktorych znajduja sie kolejne
//				// liczby do tablicy
//				emit	SignalSetNew2DViews(SlotCreate2DViewsArray());
//		}
//
//
//		int p=NumberOfCompatibleItems+3;
//
//		if ((Pos>=2+NumberOfCompatibleItems+1)) //to w sumie jest nie potrzebne
//		{
//			if(InspectionItems[Pos-p].Item!=NULL) InspectionItems[Pos-p].Item->setText(4,"+");
//
//
//
//			int i;
//			for(i=0;i<NumberOfCompatibleItems;i++)
//				if(ItemAttributesMenu->isItemChecked(i+p))
//				{
//					ItemAttributesMenu->setItemChecked(Pos,false);
//					InspectionItems[i].Item=NULL;
//
//
//				}
//
//				QString TempS;
//				TempS.setNum(Pos-p+1); //bo numerki wyswietlam od 1
//				InspectionItems[Pos-p].Item=Item;
//				Item->setText(4,TempS);
//
//				// teraz trzeba przeleciec wszsytkie zbiory i wpisac pozycje na ktorych znajduja sie kolejne
//				// liczby do tablicy
//
//
//
//				emit	SignalSetNewInspectionWindows(SlotCreateInspectionWindowsArray());
//
//
//		}
//
//
//
//
//
//	}
//
//}
//
//
////---------------------------------------------------------------
//
//int * GUIPanelWidget::SlotCreate2DViewsArray()
//{
//
//
//	int i;
//	//tu powstaje tablica z numerkami zbiorow do 2dviewsow
//	int *TempTab =new int [NumberOfCompatibleItems+1];
//	TempTab[0]=NumberOfCompatibleItems;
//
//	//czyszczenie
//	for(i=1;i<=NumberOfCompatibleItems;i++)
//		TempTab[i]=-1;
//
//	//wstawianie wlasciwych wartosci
//
//	//		QListViewItem *Item=RawDataListView->selectedItem();
//
//	QListViewItem *StepItem=RawDataListView-> firstChild();
//
//
//	int pos=0;
//	int TEMP=0;
//	int nr;
//	while(TEMP==0)
//	{
//
//		nr=StepItem->text(5).toInt();
//
//		if (nr!=0) TempTab[nr]=pos;
//
//
//
//		if(StepItem!=RawDataListView->lastItem()) 
//			StepItem=StepItem->nextSibling();
//		else
//			TEMP=1;
//		pos++;
//	}	
//
//
//	return TempTab;
//
//
//}
//
//
//
////---------------------------------------------------------------
//
//int * GUIPanelWidget::SlotCreateInspectionWindowsArray()
//{
//
//
//	int i;
//	//tu powstaje tablica z numerkami zbiorow do 2dviewsow
//	int *TempTab =new int [NumberOfCompatibleItems+1];
//	TempTab[0]=NumberOfCompatibleItems;
//
//	//czyszczenie
//	for(i=1;i<=NumberOfCompatibleItems;i++)
//		TempTab[i]=-1;
//
//	//wstawianie wlasciwych wartosci
//
//	//		QListViewItem *Item=RawDataListView->selectedItem();
//
//	QListViewItem *StepItem=RawDataListView-> firstChild();
//
//
//	int pos=0;
//	int TEMP=0;
//	int nr;
//	while(TEMP==0)
//	{
//
//		nr=StepItem->text(4).toInt();
//
//		if (nr!=0) TempTab[nr]=pos;
//
//
//
//		if(StepItem!=RawDataListView->lastItem()) 
//			StepItem=StepItem->nextSibling();
//		else
//			TEMP=1;
//		pos++;
//	}	
//
//
//	return TempTab;
//
//
//}

void	GUIPanelWidget::GenerateDistinctColors()
{

	int colPos = 0;
	unsigned char r, g, b;
	do
	{
		r = ((unsigned char*)(indexcolors+colPos))[3];
		g = ((unsigned char*)(indexcolors+colPos))[2];
		b = ((unsigned char*)(indexcolors+colPos))[1];
		distinctColors[colPos++].setRgb(r, g, b);


	} while (colPos<260);
	
	//distinctColors[colPos++].setNamedColor("#FFFFFF");	//	white
	//distinctColors[colPos++].setNamedColor("#000000");	//	black

	//distinctColors[colPos++].setNamedColor("#FF0000");	//	red
	//distinctColors[colPos++].setNamedColor("#00FF00");	//	green
	//distinctColors[colPos++].setNamedColor("#0000FF");	//	blue

	//distinctColors[colPos++].setNamedColor("#FFFF00");	//	yellow
	//distinctColors[colPos++].setNamedColor("#FF00FF");	//	magenta
	//distinctColors[colPos++].setNamedColor("#00FFFF");	//	cyan


	////distinctColors[colPos++].setNamedColor("#FF8080");	//	Pink
	//////distinctColors[colPos++].setNamedColor("#808080");	//	Gray
	////distinctColors[colPos++].setNamedColor("#800000");	//	Brown
	////distinctColors[colPos++].setNamedColor("#FF8080");	//	Orange

	////distinctColors[colPos++].setNamedColor("#FFB300"); //Vivid Yellow
	////distinctColors[colPos++].setNamedColor("#803E75"); //Strong Purple
	////distinctColors[colPos++].setNamedColor("#FF6800"); //Vivid Orange
	////distinctColors[colPos++].setNamedColor("#A6BDD7"); //Very Light Blue
	////distinctColors[colPos++].setNamedColor("#C10020"); //Vivid Red
	////distinctColors[colPos++].setNamedColor("#CEA262"); //Grayish Yellow
	//////distinctColors[colPos++].setNamedColor("#817066"); //Medium Gray

	//////The following will not be good for people with defective color vision
	////distinctColors[colPos++].setNamedColor("#007D34"); //Vivid Green
	////distinctColors[colPos++].setNamedColor("#F6768E"); //Strong Purplish Pink
	////distinctColors[colPos++].setNamedColor("#00538A"); //Strong Blue
	////distinctColors[colPos++].setNamedColor("#FF7A5C"); //Strong Yellowish Pink
	////distinctColors[colPos++].setNamedColor("#53377A"); //Strong Violet
	////distinctColors[colPos++].setNamedColor("#FF8E00"); //Vivid Orange Yellow
	////distinctColors[colPos++].setNamedColor("#B32851"); //Strong Purplish Red
	////distinctColors[colPos++].setNamedColor("#F4C800"); //Vivid Greenish Yellow
	////distinctColors[colPos++].setNamedColor("#7F180D"); //Strong Reddish Brown
	////distinctColors[colPos++].setNamedColor("#93AA00"); //Vivid Yellowish Green
	////distinctColors[colPos++].setNamedColor("#593315"); //Deep Yellowish Brown
	////distinctColors[colPos++].setNamedColor("#F13A13"); //Vivid Reddish Orange
	////distinctColors[colPos++].setNamedColor("#232C16"); //Dark Olive Green


	////reszta - bez pomyslu
	////do
	////{
	////	distinctColors[colPos++].setHsvF(((rand()) / (double)RAND_MAX) ,  ((rand()) / (double)RAND_MAX) ,  ((rand()) / (double)RAND_MAX));


	////} while (colPos<260);
	//int num_colors = 260 - colPos;
	//double hue, lightness, saturation;
	//for (double i = 0; i < 100; i += 100.0 / num_colors)
	//{
	//	hue = i / 100.0;

	//	lightness = (50 + ((rand()) / (double)RAND_MAX) * 10) / 100.0;
	//	saturation = (20 + ((rand()) / (double)RAND_MAX) * 80) / 100.0;

	//	distinctColors[colPos++].setHsvF(hue, lightness, saturation);
	//}

	//distinctColorIcons


	//pomysl na ladne kolko 8x8

	unsigned char tab[] = {
		9, 9, 1, 0, 0, 1, 9, 9,
		9, 2, 3, 4, 4, 3, 2, 9,
		1, 3, 4, 4, 4, 4, 3, 1,
		0, 4, 4, 4, 4, 4, 4, 0,
		0, 4, 4, 4, 4, 4, 4, 0,
		1, 3, 4, 4, 4, 4, 3, 1,
		9, 2, 3, 4, 4, 3, 2, 9,
		9, 9, 1, 0, 0, 1, 9, 9 };


	int i, j;
	for (i = 0; i<260; i++)
	{

		int r, g, b;
		r = distinctColors[i].red();
		g = distinctColors[i].green();
		b = distinctColors[i].blue();

		int sx = 8;
		int sy = 8;

		QImage image(sx, sy, QImage::Format_Indexed8);
		QImage image2(sx, sy, QImage::Format_Indexed8);
		//QImage image,image2;
		//image.create(sx,sy,8,256);
		//image2.create(sx,sy,8,256);

		//for (  i=0;i<255;i++)
		image.setColor(0, qRgb(r, g, b));
		image.setColor(1, qRgb(0, 0, 0));



		image2.setColor(0, qRgb(r, g, b));

		unsigned char aa = 127;//63;
		unsigned char bb = 127;//127;
		unsigned char cc = 0;//191;

		image2.setColor(1, qRgb(0, 0, 0));
		image2.setColor(2, qRgb(aa, aa, aa));
		image2.setColor(3, qRgb(bb, bb, bb));
		image2.setColor(4, qRgb(cc, cc, cc));
		image2.setColor(5, qRgb(255, 255, 255));
		image2.setColor(10, qRgba(255, 255, 255, 255));

		{
			int i, j;
			int pos = 0;
			for (i = 0; i<sx; i++)
			for (j = 0; j<sy; j++)
			{
				if ((i == 0) || (i == sx - 1) || (j == 0) || (j == sy - 1))*(image.scanLine(j) + i) = 1;
				else*(image.scanLine(j) + i) = 0;

				/*	if((i==0)||(i==sx-1)||(j==0)||(j==sy-1))*(image2.scanLine(j)+i)=1;
				else*(image2.scanLine(j)+i)=0;*/

				if (tab[pos] == 4) image2.bits()[pos] = 0;
				else image2.bits()[pos] = tab[pos] + 1;
				pos++;

			}
		}
		QPixmap pixmap(sx, sy);
		//pixmap.fromImage(image);

		QPainter paint(&pixmap);
		paint.drawImage(0, 0, image);

		distinctColorIconsRect[i] = QIcon(pixmap);
		//QPixmap pixmap2;
		//pixmap2.fromImage(image2);
		QPixmap pixmap2(sx, sy);
		QPainter paint2(&pixmap2);
		paint2.drawImage(0, 0, image2);

		distinctColorIconsCirc[i] = QIcon(pixmap2);

	}

}

//--------------------------------------------------------------------
void	GUIPanelWidget::SlotInsertMaskDataItem(ByteMask *byteMask,int col)
{
	QTreeWidgetItem *item = new QTreeWidgetItem();
//	item->setFlags(item->flags() | Qt::ItemIsEditable);

	QString name = byteMask->GetMaskName();
	//item->setText(1,QString::number(opacity,'g',2));
	item->setText(0, name);
	item->setIcon(1, this->distinctColorIconsCirc[col+1]);
	this->masksTreeWidget->insertTopLevelItem(0, item);
	byteMask->SetTreeItem(item);
}
//-----------------------------------------------
void	GUIPanelWidget::SlotInsertRawDataItem(DataSet *dataset, LinkedList <DataSet *> *dataSetList)
{


	QTreeWidgetItem *item = new QTreeWidgetItem();
	//item->setFlags(item->flags() | Qt::ItemIsEditable);

	QString name = dataset->GetRawData()->GetRawDataSetName();
	//double opacity=layer->GetBlendingOpacity();




	item->setIcon(0, this->distinctColorIconsRect[rDataCounter]);//layer visibility - on
	item->setIcon(2, emptyIcon);//filter visibility -off

	//item->setText(1,QString::number(opacity,'g',2));
	item->setText(1, name);
	//item->setText(3,QString::number(mode,'g',2));

	dataset->SetTreeWidgetItem(item, rDataCounter);

	this->rawDataTreeWidget->insertTopLevelItem(0, item);
	//rawDataTreeWidget->InsertItem(layer);
	rDataCounter = (rDataCounter + 1) % 255;
}
//------------------------------------------------------
void	GUIPanelWidget::SlotSetNewDataSetByDoubleClick(QTreeWidgetItem * item, int)
{

	this->emit SignalSetNewDataSet(item);

}
//------------------------------------------------
void	GUIPanelWidget::SlotRawTreeWidgetItemClicked(QTreeWidgetItem * item, int pos, int mode)
{

	if (item != NULL)
	{
		if (mode == 0)
			item->setIcon(pos, emptyIcon);
		else
			item->setIcon(pos, distinctColorIconsCirc[mode]);
	}

}
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//------------ FILTERING ITEMS ----------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//--------------------------------------------------------------- 




//---------------------------------------------------------

//void GUIPanelWidget::CreateFilteringTab(MyDockWidget *qtab)
//{
//
//	
//	HelperFilt=false;
//
//	QWidget *FilteringTab = new QWidget(qtab);
//	qtab->setScrollWidget(FilteringTab);
//	QVBoxLayout* MainLayout= new QVBoxLayout( FilteringTab);//, 10, -1, "MainLayout");	
//	FilteringTab->setLayout(MainLayout);
//
//	//Q3GroupBox *Gb1=new Q3GroupBox(2,Vertical,FilteringTab);	
//	QGroupBox *Gb1=new	QGroupBox(FilteringTab);
//	QVBoxLayout *vbl_Gb1 = new QVBoxLayout;
//	Gb1->setLayout(vbl_Gb1);	
//	Gb1->setTitle("Available Filters");
//	AvailableFiltersListView = new QListView( Gb1 );
//	AddFilterButton=new QPushButton2(buttonList,"Insert Filter",Gb1);
//	vbl_Gb1->addWidget(AvailableFiltersListView);
//	vbl_Gb1->addWidget(AddFilterButton);
//
//
//	//Q3GroupBox *Gb2=new Q3GroupBox(5,Vertical,FilteringTab);
//	QGroupBox *Gb2=new	QGroupBox(FilteringTab);
//	QVBoxLayout *vbl_Gb2 = new QVBoxLayout;
//	Gb2->setLayout(vbl_Gb2);	
//	Gb2->setWindowTitle("Filtering Pipeline");
//	FilteringPipelineListView = new QListView( Gb2 );
//	vbl_Gb2->addWidget(FilteringPipelineListView);
//
//
//
//	//QWidget *Gb21=new QWidget(Gb2);
//	QWidget *Gb21=new	QWidget(Gb2);
//	QHBoxLayout *hbl_Gb21 = new QHBoxLayout;
//	hbl_Gb21->setMargin(1);
//
//	Gb21->setLayout(hbl_Gb21);
//	vbl_Gb2->addWidget(Gb21);
//	MoveUpSelectedFilterButton   =	new    QPushButton2(buttonList,"Move Up",Gb21);
//	MoveDownSelectedFilterButton =	new    QPushButton2(buttonList,"Move Down",Gb21);
//
//
//	RemoveFilterButton = new QPushButton2(buttonList,"Remove Filter",Gb2);		
//	RemoveAllFiltersButton = new QPushButton2(buttonList,"Remove All Filters",Gb2);
//
//	ApplyFilteringButton = new QPushButton2(buttonList,"Apply Filtering",Gb2); 
//
//	hbl_Gb21->addWidget(MoveUpSelectedFilterButton);
//	hbl_Gb21->addWidget(MoveDownSelectedFilterButton);
//	vbl_Gb2->addWidget(RemoveFilterButton);
//	vbl_Gb2->addWidget(RemoveAllFiltersButton);
//	vbl_Gb2->addWidget(ApplyFilteringButton);
//
//
//
//		AvailableFiltersListView->addColumn("Filters List");
//		AvailableFiltersListView->setSorting(-1);
//
//		FilteringPipelineListView->addColumn("Pipeline");
//		FilteringPipelineListView->setSorting(-1);
//
//
//	/* 	QGroupBox *Gb1=new QGroupBox(2,Vertical,FilteringTab);		
//	Gb1->setTitle("Hybrid Median Info");
//	Gb1->setMaximumHeight(150);	
//
//	HybridMedianFilter   =	new    QPushButton2(buttonList,"Hybrid Median Filter",Gb1);
//
//	QHBox *HybridBox=new QHBox(Gb1);		
//	QLabel *HLab=new QLabel(" H:",HybridBox);
//	HEdit=new QLineEdit2(lineEditList,HybridBox);
//	HEdit->setText("1.0");
//	QLabel *KLab=new QLabel(" K:",HybridBox);
//	KEdit=new QLineEdit2(lineEditList,HybridBox);
//	KEdit->setText("1.0");
//
//	*/	
//
//	connect(AvailableFiltersListView,SIGNAL(doubleClicked  ( QListViewItem * )),this,SLOT(SlotSelectCurrentAvailableFilter(QListViewItem *)));
//	connect(FilteringPipelineListView,SIGNAL(doubleClicked  ( QListViewItem * )),this,SLOT(SlotEditFilterFromPipeline(QListViewItem *)));
//
//	connect(RemoveFilterButton,SIGNAL(clicked ()),this,SLOT(SlotRemoveFilterItem()));
//	connect(RemoveAllFiltersButton,SIGNAL(clicked ()),this,SLOT(SlotRemoveAllFilterItems()));
//
//	connect(MoveUpSelectedFilterButton,SIGNAL(clicked ()),this,SLOT(SlotMoveFilterItemUp()));
//	connect(MoveDownSelectedFilterButton,SIGNAL(clicked ()),this,SLOT(SlotMoveFilterItemDown()));
//
//
//
//
//
//
//
//	connect(FilteringPipelineListView,SIGNAL(currentChanged  ( QListViewItem * )),this,SLOT(SlotCurrentFiltersPipelineItemChanged( QListViewItem *)));
//
//
//
//	MainLayout->addWidget(Gb1);
//	MainLayout->addWidget(Gb2);
//
//	//qtab->addTab(FilteringTab,"&Filtering ");
//
//
//
//
//}
//
//
////---------------------------------------------------------------
//
//
//void GUIPanelWidget::SlotAddFilterToListBox(const QString &Name)
//{
//	//	AvailableFiltersListView->addAction(Name);
//
//	QListViewItem *Item=new QListViewItem( AvailableFiltersListView,AvailableFiltersListView->currentIndex(),Name);
//	AvailableFiltersListView->setCurrentIndex(AvailableFiltersListView->currentIndex()->nextSibling());
//	AvailableFiltersListView->setSelected(AvailableFiltersListView->currentIndex(),true); 
//	
//	//QListViewItem *Item=new QListViewItem( RawDataListView,RawDataListView->currentIndex(),name,"o","+","+","-","-" );
//
//
//	//RawDataListView->setCurrentIndex(RawDataListView->currentIndex()->nextSibling());
//	//RawDataListView->setSelected(RawDataListView->currentIndex(),true); 		
//}
//
////-------------------------------------------------------------------
//
//void GUIPanelWidget:: SlotSelectCurrentAvailableFilter(QListViewItem * )
//{
//
//	if(AvailableFiltersListView->currentIndex()!=NULL)
//	{
//		QListViewItem *Item=AvailableFiltersListView->currentIndex();
//		QListViewItem *StepItem=AvailableFiltersListView->firstChild();
//
//		int pos=0;
//
//		while(Item!=StepItem)
//		{
//			StepItem=StepItem->nextSibling();
//			pos++;
//		}
//
//		Item=FilteringPipelineListView->currentIndex();
//		StepItem=FilteringPipelineListView->firstChild();
//
//
//
//		int pos2=0;
//
//		if (Item!=NULL)
//			while(Item!=StepItem)
//			{
//				StepItem=StepItem->nextSibling();
//				pos2++;
//			}
//
//
//
//			emit SignalOpenAddFilterDialog(pos,pos2);
//	}
//
//}
//
////---------------------------------------------------------------
//
//
//void	GUIPanelWidget:: SlotAddFilterToFilteringPipeline()
//{
//
//	QListViewItem *Item=new QListViewItem( FilteringPipelineListView,FilteringPipelineListView->currentIndex(),AvailableFiltersListView->currentIndex()->text(0));
//	FilteringPipelineListView->setCurrentIndex(FilteringPipelineListView->currentIndex()->nextSibling());
//	FilteringPipelineListView->setSelected(FilteringPipelineListView->currentIndex(),true); 
//
//
//
//
//}
////---------------------------------------------------------------
//
//void	GUIPanelWidget:: SlotMoveFilterItemDown()
//{
//
//	QListViewItem *Item=FilteringPipelineListView->currentIndex();
//
//
//	QListViewItem *NextItem=Item->nextSibling();
//
//	if ((NextItem!=NULL)&&(NextItem!=Item))
//	{
//		Item->moveItem(NextItem);
//		emit	SignalMoveFilterItemDown();
//
//	}
//}
//
////---------------------------------------------------------------
//
//void	GUIPanelWidget:: SlotMoveFilterItemUp()
//{
//	//to jest bez sensu!
//	QListViewItem *Item=FilteringPipelineListView->currentIndex();
//
//	QListViewItem *StepItem=FilteringPipelineListView->firstChild();
//
//	if(StepItem!=Item)
//	{
//		while(Item!=StepItem->nextSibling())
//		{
//			StepItem=StepItem->nextSibling();
//		}
//
//
//		if ((StepItem!=NULL))
//		{
//			StepItem->moveItem(Item);
//			emit	SignalMoveFilterItemUp();
//		}
//
//	}
//}
//
////---------------------------------------------------------------
//
//void	GUIPanelWidget:: SlotRemoveFilterItem()
//{
//
//	HelperFilt=true;
//	emit SignalRemoveFilterItem();
//
//	FilteringPipelineListView->removeItem( FilteringPipelineListView->currentIndex());
//	FilteringPipelineListView->setSelected(FilteringPipelineListView->currentIndex(),true); 
//
//	HelperFilt=false;
//
//
//
//
//
//
//
//
//}
////---------------------------------------------------------------
//
//void	GUIPanelWidget:: SlotRemoveAllFilterItems()
//{
//
//	HelperFilt=true;
//	while(FilteringPipelineListView->currentIndex()!=NULL)
//	{
//		emit SignalRemoveFilterItem();
//		FilteringPipelineListView->removeItem( FilteringPipelineListView->currentIndex());
//		FilteringPipelineListView->setSelected(FilteringPipelineListView->currentIndex(),true); 
//	}
//
//	HelperFilt=false;
//
//
//
//
//
//
//
//
//}
////---------------------------------------------------------------
//
//void	GUIPanelWidget::SlotEditFilterFromPipeline(QListViewItem *)
//{															   
//
//	QListViewItem *Item=FilteringPipelineListView->currentIndex();
//	QListViewItem *StepItem=FilteringPipelineListView->firstChild();
//
//	int pos=0;
//
//	while(Item!=StepItem)
//	{
//		StepItem=StepItem->nextSibling();
//		pos++;
//	}
//
//
//	emit SignalOpenUpdateFilterDialog(pos);
//}
//
//
//
//
////--------------------------------------------------------------------
//
//void GUIPanelWidget:: SlotCurrentFiltersPipelineItemChanged(QListViewItem * )
//{
//
//	if((FilteringPipelineListView->currentIndex()!=NULL)&&(HelperFilt==false))
//	{
//		QListViewItem *Item=FilteringPipelineListView->currentIndex();
//		QListViewItem *StepItem=FilteringPipelineListView-> firstChild();
//
//		int pos=0;
//
//		while(Item!=StepItem)
//		{
//			StepItem=StepItem->nextSibling();
//			pos++;
//		}
//
//		emit SignalSetCurrentPipelineFilter(pos);
//	}
//
//}
//
//
//
//



//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//------------ Registration			  ---------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//--------------------------------------------------------------- 
void GUIPanelWidget::SlotShowTransformWidget()
{
	this->TransformWidget->show();
	this->TransformWidget->raise();
}
//---------------------
void GUIPanelWidget::CreateRegistrationTab(MyDockWidget *qtab)
{


//	QValidator *Validator =new TextFieldDoubleValidator(-9999999,9999999,20,this);
//
//
//	QWidget *Info = new QWidget(qtab);
//	QVBoxLayout* MainLayout= new QVBoxLayout( Info, 10, -1, "MainLayout");	
//
//	QGroupBox *Gb1=new QGroupBox(Info);Gb1->setLayout(new QVBoxLayout);
//	Gb1->setTitle("Transformation");
//
//	Gb1->layout()->setSpacing(3);
//	Gb1->layout()->setMargin(5);
//
//	QWidget *LoadSaveTransBox =new QWidget(Gb1);
//	LoadTransformationButton=new QPushButton2(buttonList,"Load Trans",LoadSaveTransBox);
//	SaveTransformationButton=new QPushButton2(buttonList,"Save Trans",LoadSaveTransBox);
//
//
//
//	
//
//	TransformWidget=new QWidget();
//
//	ShowTransformWidgetButton=new  QPushButton2(buttonList,"Transformation Panel",Gb1);
//	connect(ShowTransformWidgetButton,SIGNAL(clicked()),TransformWidget,SLOT(show()));
//
//	QVBoxLayout* TransformLayout= new QVBoxLayout( TransformWidget, 10, -1);
//	QGroupBox *TransformGroupBox=new QGroupBox(TransformWidget);TransformGroupBox->setLayout(new QVBoxLayout);
//	TransformGroupBox->setTitle("Transformation Parameters");
//	TransformLayout->addWidget(TransformGroupBox);
//
//
//
//	QLabel		*TLabel[13];
//
//
//
//	QWidget* OutsideBox[14];
//	QWidget		*Box[14]; 
//
//	int i;
//	QString NrText="0.0";
//	for(i=0;i<13;i++)
//	{
//
//		OutsideBox[i]=new QWidget(TransformGroupBox);
//		if (i<12)
//		{
//			TCheckBox[i]=new QCheckBox2(checkBoxList,OutsideBox[i]);
//			TCheckBox[i]->setChecked(true);	
//		}
//
//		Box[i]	=	new QWidget(OutsideBox[i]);
//		Box[i]->setSpacing(10);
//
//
//
//
//
//
//
//		if (i==12)	ResetTransformationButton=new QPushButton2(buttonList,"Reset",Box[i]);
//
//		TLabel[i]=new QLabel(Box[i]);
//		TEdit[i]=new QLineEdit2(lineEditList,Box[i]);
//		TEdit[i]->setValidator(Validator);
//		TEdit[i]->setMaximumWidth(90);
//
//		if (i==9) NrText="1.0";
//		TEdit[i]->setText(NrText);
//
//		TSlider[i]=new QSlider2(sliderList,-100,100,1,0,Qt::Horizontal, Box[i]);
//		connect(TSlider[i],SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateTransformation(int)));
//		connect(TSlider[i],SIGNAL(sliderReleased ()),this,SLOT(SlotUpdateTransformation()));
//		connect(TEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTransformation(const QString &)));
//
//		if (i<12) connect(TCheckBox[i],SIGNAL(toggled(bool)),Box[i],SLOT(setEnabled (bool)));
//
//	}
//
//	ResetTransformationButton->setMinimumWidth(30);
//
//	connect(ResetTransformationButton,SIGNAL(clicked ()),this,SLOT(SlotResetTransformation()));
//
//
//	TLabel[0]->setText("Tr X:");
//	TLabel[1]->setText("Tr Y:");
//	TLabel[2]->setText("Tr Z:");
//
//	TLabel[3]->setText("Rt X:");
//	TLabel[4]->setText("Rt Y:");
//	TLabel[5]->setText("Rt Z:");
//
//
//	TLabel[6]->setText("Sh X:");
//	TLabel[7]->setText("Sh Y:");
//	TLabel[8]->setText("Sh Z:");
//
//	TLabel[9]->setText("Sc X:");
//	TLabel[10]->setText("Sc Y:");
//	TLabel[11]->setText("Sc Z:");
//
//	TLabel[12]->setText("Range:");
//	TEdit[12]->setText("10");
//
//	for(i=0;i<13;i++)
//		TParams[i]=TEdit[i]->text().toFloat();
//
//	AlignCentersCheckBox= new QCheckBox2(checkBoxList,TransformGroupBox);
//	AlignCentersCheckBox->setText("Align Center");
//	AlignCentersCheckBox->setChecked(true);	
//
//	//QHBox *TransGroup=new QHBox(TransformGroupBox);
//
//
//
//	QLabel *CenterLabel=new QLabel("Rotation Center:",TransformGroupBox);
//
//
//	QWidget *CenterBox =	new QWidget(TransformGroupBox);
//	AutoCenterCheckBox= new QCheckBox2(checkBoxList,CenterBox);
//	AutoCenterCheckBox->setText("Auto");
//	AutoCenterCheckBox->setChecked(true);
//
//
//	Box[13]	=	new QWidget(CenterBox);
//
//
//
//
//
//
//
//	for(i=13;i<16;i++)
//	{
//		TEdit[i]=new QLineEdit2(lineEditList,Box[13]);
//		TEdit[i]->setValidator(Validator);
//		TEdit[i]->setMaximumWidth(50);
//		TEdit[i]->setText("0.0");
//	}
//
//	Box[13]->setEnabled (false);
//	connect(AutoCenterCheckBox,SIGNAL(toggled(bool)),Box[13],SLOT(setDisabled (bool)));
//
//
//	//QPushButton	*SetRigidTransformationModeButton;
//	//QPushButton	*SetAffineTransformationModeButton;
//	//TCheckBox
//	//--------------------------------------------------------
//	//--------------------------------------------------------
//	//--------------------------------------------------------
//	//--------------------------------------------------------
//	//--------------------------------------------------------
//	//--------------------------------------------------------
//	//--------------------------------------------------------
//	//--------------------------------------------------------
//	//--------------------------------------------------------
//	QWidget *TransTypeBox =	new QWidget(TransformGroupBox);
//	SetRigidTransformationModeButton=new QPushButton2(buttonList,"Rigid",TransTypeBox);
//	SetAffineTransformationModeButton=new QPushButton2(buttonList,"Affine",TransTypeBox);
//
//	connect(SetRigidTransformationModeButton,SIGNAL(clicked()),this,SLOT(SlotSetTransformationModeToRigid()));
//	connect(SetAffineTransformationModeButton,SIGNAL(clicked()),this,SLOT(SlotSetTransformationModeToAffine()));
//
//	//	void	SlotSetTransformationModeToAffine();
//	//	void	SlotSetTransformationModeToRigid();
//
//
//
//
//
//
//
//	VolumeTransformButton=new QPushButton2(buttonList,"Transform Volume",Gb1);
//
//
//
//
//	
//	///////////////////////////////////////////////////
//
//	QGroupBox *Gb2=new QGroupBox(Info);Gb2->setLayout(new QVBoxLayout);
//	Gb2->setTitle("Optimization");
//	Gb2->layout()->setSpacing(3);
//	Gb2->layout()->setMargin(5);
//	//	Gb2->setMinimumHeight(265);
//
//
//	MisregistrationFunctionCombo= new QComboBox2(comboBoxList, Gb2, "read-only combo" );
//
//	MisregistrationFunctionCombo->addAction("I(A,B) = H(A)+H(B)-H(A,B)",0);
//	MisregistrationFunctionCombo->addAction("NMI1(A,B) = 2I(A,B)/(H(A)+H(B))",1);
//	MisregistrationFunctionCombo->addAction("NMI2(A,B) = H(A,B)/(H(A)+H(B))",2);
//	MisregistrationFunctionCombo->addAction("NMI3(A,B) = -(H(A)+H(B))/H(A,B)",3);
//	MisregistrationFunctionCombo->addAction("NMI4(A,B) = H(A,B) - I(A,B)",4);
//	MisregistrationFunctionCombo->addAction("Entropy = H(A,B)",5);
//	MisregistrationFunctionCombo->addAction("SSD",6);
//	MisregistrationFunctionCombo->setMinimumWidth(100);
//
//
//
//
//	QWidget *InterpolationBox=new QWidget(Gb2);
//	QLabel *InterpolationLabel = new QLabel("Interpolation:",InterpolationBox);	
//	InterpolationCombo = new QComboBox2(comboBoxList, InterpolationBox, "read-only combo" );
//	InterpolationCombo->addAction("TriLinear",0);
//	InterpolationCombo->addAction("Nearest",1);
//
//	QWidget *RegStepBox=new QWidget(Gb2);	
//	//	QLabel *RegStepL0=new QLabel("Spc:",RegStepBox);
//
//	QLabel *RegStepL1=new QLabel(" X:",RegStepBox);
//	RegStepXBox = new QSpinBox2(spinBoxList,1,511,1,RegStepBox);
//	RegStepXBox->setValue(1);
//	RegStepXBox->setMaximumWidth(40);
//
//	QLabel *RegStepL2=new QLabel(" Y:",RegStepBox);
//	RegStepYBox = new QSpinBox2(spinBoxList,1,511,1,RegStepBox);
//	RegStepYBox->setValue(1);
//	RegStepYBox->setMaximumWidth(40);
//
//	QLabel *RegStepL3=new QLabel("Z:",RegStepBox);
//	RegStepZBox = new QSpinBox2(spinBoxList,1,511,1,RegStepBox);
//	RegStepZBox->setValue(1);
//	RegStepZBox->setMaximumWidth(40);
//
//
//
//	QWidget *ScatterBox=new QWidget(Gb2);
//
//	ScatterPlotVisualizationCheckBox= new QCheckBox2(checkBoxList,ScatterBox);
//	//ScatterPlotVisualizationCheckBox->setText("Show:");
//	ScatterPlotVisualizationCheckBox->setChecked(false);
//	ScatterPlotVisualizationCheckBox->setMaximumWidth(40);
//
//
//	ScatterPlotButton=new QPushButton2(buttonList,"ScatterPlot",ScatterBox);
//	ScatterPlotButton->setMaximumWidth(60);
//
//	QLabel *ScatterLabel = new QLabel(" Size:",ScatterBox);	
//
//	ScatterSizeCombo = new QComboBox2(comboBoxList, ScatterBox, "read-only combo" );
//	ScatterSizeCombo->addAction("16",0);
//	ScatterSizeCombo->addAction("32",1);
//	ScatterSizeCombo->addAction("64",2);
//	ScatterSizeCombo->addAction("128",3);
//	ScatterSizeCombo->addAction("256",4);
//	ScatterSizeCombo->addAction("512",5);
//
//	QWidget *ToleranceBox=new QWidget(Gb2);
//
//	ReversedTransformCheckBox= new QCheckBox2(checkBoxList,ToleranceBox);
//	ReversedTransformCheckBox->setText("RevTrans ");
//	ReversedTransformCheckBox->setChecked(false);	
//
//	QLabel *ToleranceLabel = new QLabel("Tol(10^):",ToleranceBox);	
//	ToleranceSpinBox = new QSpinBox2(spinBoxList,-100,100,1,ToleranceBox);
//	ToleranceSpinBox->setValue(-3);
//	ToleranceSpinBox->setMinimumWidth(10);
//
//	MissregInfoLabel = new QLabel("Iter:---  Count:----  Value:--------",Gb2);
//
//
//	QWidget *StartStopOptimBox =new QWidget(Gb2);
//	StartRegistrationButton=new QPushButton2(buttonList,"Start",StartStopOptimBox);
//	StopRegistrationButton=new QPushButton2(buttonList,"Stop",StartStopOptimBox);
//
//
//	//
//
//
//	QWidget *ResolBox=new QWidget(Gb2);
//	QLabel *ResolLabel = new QLabel("Res: ",ResolBox);	
//
//	ResolXSpinBox = new QSpinBox2(spinBoxList,8,1024,1,ResolBox);
//	ResolXSpinBox->setValue(74);
//	ResolXSpinBox->setMinimumWidth(20);
//	ResolYSpinBox = new QSpinBox2(spinBoxList,8,1024,1,ResolBox);
//	ResolYSpinBox->setValue(74);
//	ResolYSpinBox->setMinimumWidth(20);
//	ResolZSpinBox = new QSpinBox2(spinBoxList,8,1024,1,ResolBox);
//	ResolZSpinBox->setValue(74);
//	ResolZSpinBox->setMinimumWidth(20);
//
//	QWidget *SizeBox=new QWidget(Gb2);
//	QLabel *TTransSizeLabel = new QLabel("Trans: ",SizeBox);	
//	TTransSizeEdit=new QLineEdit2(lineEditList,SizeBox);
//	TTransSizeEdit->setValidator(Validator);
//	TTransSizeEdit->setMaximumWidth(50);
//	TTransSizeEdit->setText("15.0");
//
//	QLabel *TRotSizeLabel = new QLabel("Rot: ",SizeBox);	
//	TRotSizeEdit=new QLineEdit2(lineEditList,SizeBox);
//	TRotSizeEdit->setValidator(Validator);
//	TRotSizeEdit->setMaximumWidth(50);
//	TRotSizeEdit->setText("15.0");
//
/////////////////////////////
//
//	///-----------------
//	ShowVolumeRegistrationGraphButton=new  QPushButton2(buttonList,"Generate Graphs ...",Gb2);
//	VolumeRegistrationGraphWidget=new QWidget();
//	VolumeRegistrationGraphWidget->setCaption("Generate Graphs Panel");
//	connect(ShowVolumeRegistrationGraphButton,SIGNAL(clicked()),VolumeRegistrationGraphWidget,SLOT(show()));
//
//	QVBoxLayout* VolumeRegistrationGraphLayout= new QVBoxLayout( VolumeRegistrationGraphWidget, 10, -1);
//	QGroupBox *VolumeRegistrationGraphGroupBox=new QGroupBox(VolumeRegistrationGraphWidget);VolumeRegistrationGraphGroupBox->setLayout(new QVBoxLayout);
//	VolumeRegistrationGraphLayout->addWidget(VolumeRegistrationGraphGroupBox);	
//	//---------------------
////////////////////////////
//
//
//
//
//
//	QWidget *GraphBox=new QWidget(VolumeRegistrationGraphGroupBox);
//	GraphXSpinBox = new QSpinBox2(spinBoxList,1,6,1,GraphBox);
//	GraphXSpinBox->setValue(1);
//	GraphXSpinBox->setMinimumWidth(28);
//	GraphYSpinBox = new QSpinBox2(spinBoxList,1,6,1,GraphBox);
//	GraphYSpinBox->setValue(2);
//	GraphYSpinBox->setMinimumWidth(28);
//	GraphZSpinBox = new QSpinBox2(spinBoxList,1,6,1,GraphBox);
//	GraphZSpinBox->setValue(3);
//	GraphZSpinBox->setMinimumWidth(28);
//	Generate3DMisregPolyGraphButton=new QPushButton2(buttonList,"3DVol",GraphBox);
//	GeneratePolyMisregistrationGraphButton=new QPushButton2(buttonList,"2DPoly",GraphBox);
//	Generate3DMisregPolyGraphButton->setMinimumWidth(35);
//	GeneratePolyMisregistrationGraphButton->setMinimumWidth(35);
//
//	QWidget *GraphBox2=new QWidget(VolumeRegistrationGraphGroupBox);
//
//
//	Generate6DRandomGraphButton=new QPushButton2(buttonList,"6DRandom",GraphBox2);
//	GenerateMisregistrationGraphsButton=new QPushButton2(buttonList,"Gnuplot2DGraphs",GraphBox2);
//	Generate6DRandomGraphButton->setMinimumWidth(45);
//	GenerateMisregistrationGraphsButton->setMinimumWidth(95);
//
//
//	connect(ScatterPlotButton,SIGNAL(clicked ()),this,SLOT(SlotScatterPlotButtonPressed()));
//	connect(VolumeTransformButton,SIGNAL(clicked ()),this,SLOT(SlotVolumeTransformButtonPressed()));
//	connect(StartRegistrationButton,SIGNAL(clicked ()),this,SLOT(SlotStartRegistrationButtonPressed()));
//
//
//	connect(GenerateMisregistrationGraphsButton,SIGNAL(clicked ()),this,SLOT(SlotGenerateMisregistrationGraphsButtonPressed()));
//	connect(GeneratePolyMisregistrationGraphButton,SIGNAL(clicked ()),this,SLOT(SlotGeneratePolyGraphButtonPressed()));
//	connect(Generate6DRandomGraphButton,SIGNAL(clicked ()),this,SLOT(SlotGenerate6DGraphButtonPressed()));
//	connect(Generate3DMisregPolyGraphButton,SIGNAL(clicked ()),this,SLOT(SlotGenerate3DVolumeGraphButtonPressed()));
//
//
///////////
//	///////////////
//	//----------------------------------------------------------
//
//	QGroupBox *GbX=new QGroupBox(Info);GbX->setLayout(new QVBoxLayout);
//	GbX->setTitle("Feature Point Registration");
//	GbX->layout()->setSpacing(3);
//	GbX->layout()->setMargin(5);
//
//
//
//	FeaturePointsWidget=new QWidget();
//	FeaturePointsWidget->setCaption("Feature Points Panel");
//	ShowFeaturePointsWidgetButton=new  QPushButton2(buttonList,"Feature Points Panel",GbX);
//	connect(ShowFeaturePointsWidgetButton,SIGNAL(clicked()),FeaturePointsWidget,SLOT(show()));
//
//	QVBoxLayout* FeaturePointsLayout= new QVBoxLayout( FeaturePointsWidget, 10, -1);
//	QGroupBox *FeaturePointsGroupBox=new QGroupBox(FeaturePointsWidget);FeaturePointsGroupBox->setLayout(new QVBoxLayout);
//	FeaturePointsGroupBox->setTitle("Feature Points Editor");
//	FeaturePointsLayout->addWidget(FeaturePointsGroupBox);
//	//////////////////////////////////
//	////////////////////////////////////
////----------
//
//
//	QWidget *FeaturePointBox=new QWidget(FeaturePointsGroupBox);
//
//	QGroupBox *ModelBox=new QGroupBox(FeaturePointBox);ModelBox->setLayout(new QVBoxLayout());	
//	ModelBox->setTitle("Model Points");
//	ModelListBox=new Q3ListBox(ModelBox);
//
//	QWidget *FeaturePointBoxM=new QWidget(ModelBox);
//
//	AddModelPointPushButton=new QPushButton2(buttonList,"Add",FeaturePointBoxM);
//	ClearModelPointList=new QPushButton2(buttonList,"ClrAll",FeaturePointBoxM);
//
//	int buttonsize=30;
//
//	ModelBox->setMinimumWidth(80);
//	ModelListBox->setMinimumWidth(buttonsize);
//	ModelListBox->setMinimumHeight(20);
//	AddModelPointPushButton->setFixedWidth(buttonsize);
//	ClearModelPointList->setFixedWidth(buttonsize);
//
//
//
//	QGroupBox *ObjectBox=new QGroupBox(FeaturePointBox);	ObjectBox->setLayout(new QVBoxLayout);
//	ObjectBox->setTitle("Object Points");
//	ObjectListBox=new Q3ListBox(ObjectBox);
//
//	QWidget *FeaturePointBoxO=new QWidget(ObjectBox);
//
//	AddObjectPointPushButton=new QPushButton2(buttonList,"Add",FeaturePointBoxO);
//	ClearObjectPointList=new QPushButton2(buttonList,"ClrAll",FeaturePointBoxO);
//
//	ObjectBox->setMinimumWidth(80);
//	ObjectListBox->setMinimumWidth(buttonsize);
//	ObjectListBox->setMinimumHeight(20);
//
//	AddObjectPointPushButton->setFixedWidth(buttonsize);
//	ClearObjectPointList->setFixedWidth(buttonsize);
//
//
//	StartFeaturePointOptimization=new QPushButton2(buttonList,"Start Optimization",GbX);
//
//
//
//	////////////////////////////////////////////////////////
//	////////////////////////////////////
//	////////////////////////////////////
//
//
//	QGroupBox *GbY=new QGroupBox(Info);GbY->setLayout(new QVBoxLayout);
//	GbY->setTitle("Elastic Registration");
//	GbY->layout()->setSpacing(3);
//	GbY->layout()->setMargin(5);
//
//
//	QWidget *ElasticRegistrationBox =new QWidget(GbY);
//	StartElasticVolumeRegistrationButton=new QPushButton2(buttonList,"Start",ElasticRegistrationBox);
//	StopElasticVolumeRegistrationButton=new QPushButton2(buttonList,"Stop",ElasticRegistrationBox);
//
//	//QHBox *ElasticBox1=new QHBox(GbY);
//	ApplyElasticVolumeTransformation=new QPushButton2(buttonList,"Transform",GbY);
//	//ApplyElasticVolumeTransformation->setMinimumWidth(40);
//	
//
//
//	///-----------------
//	ShowVolumeElasticRegistrationPanelButton=new  QPushButton2(buttonList,"Parameters",GbY);
//
//	VolumeElasticRegistrationWidget=new QWidget();
//	VolumeElasticRegistrationWidget->setCaption("Volume Elastic Registration Panel");
//	connect(ShowVolumeElasticRegistrationPanelButton,SIGNAL(clicked()),VolumeElasticRegistrationWidget,SLOT(show()));
//
//	QVBoxLayout* VolumeElasticRegistrationLayout= new QVBoxLayout( VolumeElasticRegistrationWidget, 10, -1);
//	QGroupBox *VolumeElasticRegistratioGroupBox=new QGroupBox(VolumeElasticRegistrationWidget);VolumeElasticRegistratioGroupBox->setLayout(new QVBoxLayout);
//	//VolumeElasticRegistratioGroupBox->setTitle("Feature Points Editor");
//	VolumeElasticRegistrationLayout->addWidget(VolumeElasticRegistratioGroupBox);	
//	
//	
//	//---------------------
//	QWidget *GridSizeXXBox=new QWidget(VolumeElasticRegistratioGroupBox);	
//	
//	
//	
//	QLabel *MultiResolutionStepL4=new QLabel("Multi-Res Step:",GridSizeXXBox);
//	MultiResolutionStepBox = new QSpinBox2(spinBoxList,1,512,1,GridSizeXXBox);
//	MultiResolutionStepBox->setValue(4);
//	MultiResolutionStepBox->setMaximumWidth(40);
//
//	ElasticUpdateGridBox=new QCheckBox2(checkBoxList,GridSizeXXBox);
//	ElasticUpdateGridBox->setText("Update");
//	ElasticUpdateGridBox->setChecked(false);
//
//
//	QWidget *GridSizeBox=new QWidget(VolumeElasticRegistratioGroupBox);	
//	//	QLabel *GridSizeL0=new QLabel("Spc:",GridSizeBox);
//
//	QLabel *GridSizeL1=new QLabel(" X:",GridSizeBox);
//	GridSizeX1Box = new QSpinBox2(spinBoxList,2,512,1,GridSizeBox);
//	GridSizeX1Box->setValue(8);
//	GridSizeX1Box->setMaximumWidth(40);
//
//	QLabel *GridSizeL2=new QLabel(" Y:",GridSizeBox);
//	GridSizeY1Box = new QSpinBox2(spinBoxList,2,512,1,GridSizeBox);
//	GridSizeY1Box->setValue(8);
//	GridSizeY1Box->setMaximumWidth(40);
//
//	QLabel *GridSizeL3=new QLabel("Z:",GridSizeBox);
//	GridSizeZ1Box = new QSpinBox2(spinBoxList,2,512,1,GridSizeBox);
//	GridSizeZ1Box->setValue(8);
//	GridSizeZ1Box->setMaximumWidth(40);
//	///////////////
//	////////////////
//	
//	QWidget *OptimParamsBox=new QWidget(VolumeElasticRegistratioGroupBox);
//
//	QLabel *ElasticIterL3=new QLabel("Iter:",OptimParamsBox);
//	ElasticIterBox = new QSpinBox2(spinBoxList,1,512,1,OptimParamsBox);
//	ElasticIterBox->setValue(1);
//	ElasticIterBox->setMaximumWidth(40);
//
//	QLabel *ElasticStepL4=new QLabel("Step:",OptimParamsBox);
//	ElasticStepBox = new QSpinBox2(spinBoxList,1,512,1,OptimParamsBox);
//	ElasticStepBox->setValue(2);
//	ElasticStepBox->setMaximumWidth(40);
//
//	QWidget *OptimParams2Box=new QWidget(VolumeElasticRegistratioGroupBox);
//
//	ElasticRandomSamplingBox=new QCheckBox2(checkBoxList,OptimParams2Box);
//	ElasticRandomSamplingBox->setText("Rand");
//	ElasticRandomSamplingBox->setChecked(false);
//
//	ElasticRegistrationFixedBoundsBox=new QCheckBox2(checkBoxList,OptimParams2Box);
//	ElasticRegistrationFixedBoundsBox->setText("Fixed Bounds");
//	ElasticRegistrationFixedBoundsBox->setChecked(true);
//
//
//	MainLayout->addWidget(Gb1);
//	MainLayout->addWidget(Gb2);
//	MainLayout->addWidget(GbX);
//	MainLayout->addWidget(GbY);
//
//
//
//
//	qtab->addTab(Info,"&Registration ");



TextFieldDoubleValidator *Validator =new TextFieldDoubleValidator(-9999999,9999999,20,this);
Validator->setNotation(TextFieldDoubleValidator::StandardNotation);

	QWidget *Info = new QWidget(qtab);
	qtab->setScrollWidget(Info);
	QVBoxLayout* MainLayout= new QVBoxLayout( Info);//, 10, -1, "MainLayout");	
	Info->setLayout(MainLayout);


	 frameStabilizationCopyPushButton = new QPushButton2(buttonList, "TransversalStabilization", Info);
	 MainLayout->addWidget(frameStabilizationCopyPushButton);
	//Q3GroupBox *Gb1=new Q3GroupBox(20,Vertical,Info);
	QGroupBox *Gb1=new	QGroupBox(Info);
	QVBoxLayout *vbl_Gb1 = new QVBoxLayout;
	Gb1->setLayout(vbl_Gb1);
	vbl_Gb1->setSpacing(1);
	Gb1->setWindowTitle("Transformation");
	MainLayout->addWidget(Gb1);
	//	Gb1->setInsideSpacing(3);
	//	Gb1->setInsideMargin(5);

	//QWidget *LoadSaveTransBox =new QWidget(Gb1);
	QWidget *LoadSaveTransBox=new	QWidget(Gb1);
	QHBoxLayout *hbl_LoadSaveTransBox = new QHBoxLayout;
	LoadSaveTransBox->setLayout(hbl_LoadSaveTransBox);
	hbl_LoadSaveTransBox->setSpacing(1);
	hbl_LoadSaveTransBox->setMargin(1);
	vbl_Gb1->addWidget(LoadSaveTransBox);
	LoadTransformationButton=new QPushButton2(buttonList,"Load Trans",LoadSaveTransBox);
	SaveTransformationButton=new QPushButton2(buttonList,"Save Trans",LoadSaveTransBox);

	hbl_LoadSaveTransBox->addWidget(LoadTransformationButton);
	hbl_LoadSaveTransBox->addWidget(SaveTransformationButton);


	TransformWidget=new QWidget();

	ShowTransformWidgetButton=new  QPushButton2(buttonList,"Transformation Panel",Gb1);
	connect(ShowTransformWidgetButton,SIGNAL(clicked()),this,SLOT(SlotShowTransformWidget()));

	QVBoxLayout* TransformLayout= new QVBoxLayout( TransformWidget);//, 10, -1);
	TransformWidget->setLayout(TransformLayout);
	vbl_Gb1->addWidget(ShowTransformWidgetButton);

	//Q3GroupBox *TransformGroupBox=new Q3GroupBox(30,Vertical,TransformWidget);
	QGroupBox *TransformGroupBox=new	QGroupBox(TransformWidget);
	QVBoxLayout *vbl_TransformGroupBox = new QVBoxLayout;
	TransformGroupBox->setLayout(vbl_TransformGroupBox);	


	TransformGroupBox->setWindowTitle("Transformation Parameters");
	TransformLayout->addWidget(TransformGroupBox);



	QLabel		*TLabel[13];



	//QWidget* OutsideBox[14];
	QWidget *OutsideBox[14];
	QHBoxLayout *hbl_OutsideBox[14];


	//	QWidget		*Box[14]; 
	QWidget *Box[14];
	QHBoxLayout *hbl_Box[14];


	int i;
	QString NrText="0.0";
	for(i=0;i<13;i++)
		{

		//OutsideBox[i]=new QWidget(TransformGroupBox);
		OutsideBox[i]=new	QWidget(TransformGroupBox);
		vbl_TransformGroupBox->addWidget(OutsideBox[i]);
		hbl_OutsideBox[i] = new QHBoxLayout;
		OutsideBox[i]->setLayout(hbl_OutsideBox[i]);
		hbl_OutsideBox[i]->setSpacing(1);
		hbl_OutsideBox[i]->setMargin(1);


		if (i<12)
			{
			TCheckBox[i]=new QCheckBox2(checkBoxList,OutsideBox[i]);
			TCheckBox[i]->setChecked(true);	
			hbl_OutsideBox[i]->addWidget(TCheckBox[i]);
			}

		//Box[i]	=	new QWidget(OutsideBox[i]);
		Box[i]=new	QWidget(OutsideBox[i]);
		hbl_OutsideBox[i]->addWidget(Box[i]);
		hbl_Box[i] = new QHBoxLayout;
		Box[i]->setLayout(hbl_Box[i]);
		hbl_Box[i]->setSpacing(1);
		hbl_Box[i]->setMargin(1);


		//Box[i]->setSpacing(10);







		if (i==12)	
			{
			ResetTransformationButton=new QPushButton2(buttonList,"Reset",Box[i]);
			hbl_Box[i]->addWidget(ResetTransformationButton);
			}
		TLabel[i]=new QLabel(Box[i]);
		TEdit[i]=new QLineEdit2(lineEditList,Box[i]);
		TEdit[i]->setValidator(Validator);
		TEdit[i]->setMaximumWidth(90);


		if (i==9) NrText="1.0";
		TEdit[i]->setText(NrText);

		TSlider[i]=new QSlider2(sliderList,Qt::Horizontal, Box[i]);
		TSlider[i]->setRange(-100,100);
		connect(TSlider[i],SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateTransformation(int)));
		connect(TSlider[i],SIGNAL(sliderReleased ()),this,SLOT(SlotUpdateTransformation()));
		connect(TEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTransformation(const QString &)));


		hbl_Box[i]->addWidget(TLabel[i]);
		hbl_Box[i]->addWidget(TEdit[i]);
		hbl_Box[i]->addWidget(TSlider[i]);

		if (i<12) connect(TCheckBox[i],SIGNAL(toggled(bool)),Box[i],SLOT(setEnabled (bool)));

		}

	ResetTransformationButton->setMinimumWidth(30);

	connect(ResetTransformationButton,SIGNAL(clicked ()),this,SLOT(SlotResetTransformation()));


	TLabel[0]->setText("Tr X:");
	TLabel[1]->setText("Tr Y:");
	TLabel[2]->setText("Tr Z:");

	TLabel[3]->setText("Rt X:");
	TLabel[4]->setText("Rt Y:");
	TLabel[5]->setText("Rt Z:");


	TLabel[6]->setText("Sh X:");
	TLabel[7]->setText("Sh Y:");
	TLabel[8]->setText("Sh Z:");

	TLabel[9]->setText("Sc X:");
	TLabel[10]->setText("Sc Y:");
	TLabel[11]->setText("Sc Z:");

	TLabel[12]->setText("Range:");
	TEdit[12]->setText("10");

	for(i=0;i<13;i++)
		TParams[i]=TEdit[i]->text().toFloat();

	AlignCentersCheckBox= new QCheckBox2(checkBoxList,TransformGroupBox);
	AlignCentersCheckBox->setText("Align Center");
	AlignCentersCheckBox->setChecked(true);	
	vbl_TransformGroupBox->addWidget(AlignCentersCheckBox);
	//QHBox *TransGroup=new QHBox(TransformGroupBox);



	QLabel *CenterLabel=new QLabel("Rotation Center:",TransformGroupBox);
	vbl_TransformGroupBox->addWidget(CenterLabel);

	//QWidget *CenterBox =	new QWidget(TransformGroupBox);
	QWidget *CenterBox=new	QWidget(TransformGroupBox);
	QHBoxLayout *hbl_CenterBox = new QHBoxLayout;
	CenterBox->setLayout(hbl_CenterBox);

	AutoCenterCheckBox= new QCheckBox2(checkBoxList,CenterBox);
	AutoCenterCheckBox->setText("Auto");
	AutoCenterCheckBox->setChecked(true);

	vbl_TransformGroupBox->addWidget(CenterBox);
	//Box[13]	=	new QWidget(CenterBox);
	Box[13]=new	QWidget(CenterBox);
	QHBoxLayout *hbl_Box13 = new QHBoxLayout;
	Box[13]->setLayout(hbl_Box13);
	hbl_CenterBox->addWidget(AutoCenterCheckBox);
	hbl_CenterBox->addWidget(Box[13]);




	for(i=13;i<16;i++)
		{
		TEdit[i]=new QLineEdit2(lineEditList,Box[13]);
		hbl_Box13->addWidget(TEdit[i]);
		TEdit[i]->setValidator(Validator);
		TEdit[i]->setMaximumWidth(50);
		TEdit[i]->setText("0.0");
		}

	Box[13]->setEnabled (false);
	connect(AutoCenterCheckBox,SIGNAL(toggled(bool)),Box[13],SLOT(setDisabled (bool)));


	//QPushButton	*SetRigidTransformationModeButton;
	//QPushButton	*SetAffineTransformationModeButton;
	//TCheckBox
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	//--------------------------------------------------------
	//QWidget *TransTypeBox =	new QWidget(TransformGroupBox);
	QWidget *TransTypeBox=new	QWidget(TransformGroupBox);
	QHBoxLayout *hbl_TransTypeBox= new QHBoxLayout;
	TransTypeBox->setLayout(hbl_TransTypeBox);
	vbl_TransformGroupBox->addWidget(TransTypeBox);

	SetRigidTransformationModeButton=new QPushButton2(buttonList,"Rigid",TransTypeBox);
	SetAffineTransformationModeButton=new QPushButton2(buttonList,"Affine",TransTypeBox);

	hbl_TransTypeBox->addWidget(SetRigidTransformationModeButton);
	hbl_TransTypeBox->addWidget(SetAffineTransformationModeButton);

	connect(SetRigidTransformationModeButton,SIGNAL(clicked()),this,SLOT(SlotSetTransformationModeToRigid()));
	connect(SetAffineTransformationModeButton,SIGNAL(clicked()),this,SLOT(SlotSetTransformationModeToAffine()));
	SlotSetTransformationModeToRigid();
	//	void	SlotSetTransformationModeToAffine();
	//	void	SlotSetTransformationModeToRigid();




	QWidget *xbg=new QWidget(Gb1);
	xbg->setLayout(new QHBoxLayout());
	xbg->layout()->setSpacing(0);
	xbg->layout()->setMargin(1);


	VolumeTransformButton=new QPushButton2(buttonList,"Transform Volume",xbg);
	xbg->layout()->addWidget(VolumeTransformButton);




	invVolAffineCheckBox = new QCheckBox2(checkBoxList,xbg);
	invVolAffineCheckBox->setText("inv");
	invVolAffineCheckBox->setChecked(true);
	xbg->layout()->addWidget(invVolAffineCheckBox);


	SurfaceTransformButton = new QPushButton2(buttonList, "Transform Surfaces", Gb1);
	Gb1->layout()->addWidget(SurfaceTransformButton);


		Gb1->layout()->addWidget(xbg);

	//VolumeTransformButton=new QPushButton2(buttonList,"Transform Volume",Gb1);
	//vbl_Gb1->addWidget(VolumeTransformButton);




	///////////////////////////////////////////////////

	//Q3GroupBox *Gb2=new Q3GroupBox(20,Vertical,Info);

	QGroupBox *Gb2=new	QGroupBox(Info);
	QVBoxLayout *vbl_Gb2 = new QVBoxLayout;
	Gb2->setLayout(vbl_Gb2);	
	vbl_Gb2->setSpacing(1);
	MainLayout->addWidget(Gb2);

	Gb2->setWindowTitle("Optimization");
	//	Gb2->setInsideSpacing(3);
	//	Gb2->setInsideMargin(5);
	//	Gb2->setMinimumHeight(265);

	
	QWidget *w=new QWidget(Gb2);
	Gb2->layout()->addWidget(w);
	w->setLayout(new QHBoxLayout());
	w->layout()->setMargin(1);
	w->layout()->setSpacing(1);




	QLabel *label = new QLabel(w);
	label->setText("opacity:");
	w->layout()->addWidget(label);
	affineRegistrationSlider = new QSlider2(sliderList,Qt::Horizontal, w);
	affineRegistrationSlider->setRange(0, 255);
	affineRegistrationSlider->setValue(127);
	w->layout()->addWidget(affineRegistrationSlider);


	w = new QWidget(Gb2);
	Gb2->layout()->addWidget(w);
	w->setLayout(new QHBoxLayout());
	w->layout()->setMargin(1);
	w->layout()->setSpacing(1);
	parallelThreadSlider=new QSlider2(sliderList,Qt::Horizontal,w);
	w->layout()->addWidget(parallelThreadSlider);
	parallelThreadLabel=new QLabel(w);
	w->layout()->addWidget(parallelThreadLabel);
	connect(parallelThreadSlider, SIGNAL(valueChanged ( int )), this, SLOT(SlotParallelSliderValueChanged(int)));

	parallelRegistrationCheckBox=new QCheckBox2(checkBoxList,Gb2);
	parallelRegistrationCheckBox->setChecked(true);
	parallelRegistrationCheckBox->setText("Compute Parallel");
	vbl_Gb2->addWidget(parallelRegistrationCheckBox);



	MisregistrationFunctionCombo= new QComboBox2(comboBoxList, Gb2 );

	MisregistrationFunctionCombo->addItem( "I(A,B) = H(A)+H(B)-H(A,B)");
	MisregistrationFunctionCombo->addItem( "NMI1(A,B) = 2I(A,B)/(H(A)+H(B))");
	MisregistrationFunctionCombo->addItem( "NMI2(A,B) = H(A,B)/(H(A)+H(B))");
	MisregistrationFunctionCombo->addItem( "NMI3(A,B) = -(H(A)+H(B))/H(A,B)");
	MisregistrationFunctionCombo->addItem( "NMI4(A,B) = H(A,B) - I(A,B)");
	MisregistrationFunctionCombo->addItem( "Entropy = H(A,B)");
	MisregistrationFunctionCombo->addItem( "SSD");
	MisregistrationFunctionCombo->setMinimumWidth(100);
	vbl_Gb2->addWidget(MisregistrationFunctionCombo);



	//QWidget *InterpolationBox=new QWidget(Gb2);
	QWidget *InterpolationBox=new	QWidget(Gb2);
	QHBoxLayout *hbl_InterpolationBox = new QHBoxLayout;
	hbl_InterpolationBox->setSpacing(0);
	InterpolationBox->setLayout(hbl_InterpolationBox);
	vbl_Gb2->addWidget(InterpolationBox);

	QLabel *InterpolationLabel = new QLabel("Interpolation:",InterpolationBox);	
	InterpolationCombo = new QComboBox2(comboBoxList, InterpolationBox );
	InterpolationCombo->addItem("TriLinear");
	InterpolationCombo->addItem("Nearest");
	hbl_InterpolationBox->addWidget(InterpolationLabel);
	hbl_InterpolationBox->addWidget(InterpolationCombo);


	//QWidget *RegStepBox=new QWidget(Gb2);	
	QWidget *RegStepBox=new	QWidget(Gb2);
	QHBoxLayout *hbl_RegStepBox = new QHBoxLayout;
	hbl_RegStepBox->setSpacing(0);
	hbl_RegStepBox->setMargin(1);
	RegStepBox->setLayout(hbl_RegStepBox);
	vbl_Gb2->addWidget(RegStepBox);
	//	QLabel *RegStepL0=new QLabel("Spc:",RegStepBox);

	QLabel *RegStepL1=new QLabel(" X:",RegStepBox);
	RegStepXBox = new QSpinBox2(spinBoxList,RegStepBox);
	RegStepXBox->setRange(1,511);
	RegStepXBox->setValue(4);
	RegStepXBox->setMaximumWidth(40);

	QLabel *RegStepL2=new QLabel(" Y:",RegStepBox);
	RegStepYBox = new QSpinBox2(spinBoxList,RegStepBox);
	RegStepYBox->setRange(1,511);
	RegStepYBox->setValue(4);
	RegStepYBox->setMaximumWidth(40);

	QLabel *RegStepL3=new QLabel("Z:",RegStepBox);
	RegStepZBox = new QSpinBox2(spinBoxList,RegStepBox);
	RegStepZBox->setRange(1,511);
	RegStepZBox->setValue(4);
	RegStepZBox->setMaximumWidth(40);

	hbl_RegStepBox->addWidget(RegStepL1);
	hbl_RegStepBox->addWidget(RegStepXBox);
	hbl_RegStepBox->addWidget(RegStepL2);
	hbl_RegStepBox->addWidget(RegStepYBox);
	hbl_RegStepBox->addWidget(RegStepL3);
	hbl_RegStepBox->addWidget(RegStepZBox);

	//QWidget *ScatterBox=new QWidget(Gb2);
	QWidget *ScatterBox=new	QWidget(Gb2);
	QHBoxLayout *hbl_ScatterBox = new QHBoxLayout;
	hbl_ScatterBox->setSpacing(0);
	hbl_ScatterBox->setMargin(1);
	ScatterBox->setLayout(hbl_ScatterBox);
	vbl_Gb2->addWidget(ScatterBox);
	ScatterPlotVisualizationCheckBox= new QCheckBox2(checkBoxList,ScatterBox);
	//ScatterPlotVisualizationCheckBox->setText("Show:");
	ScatterPlotVisualizationCheckBox->setChecked(false);
	ScatterPlotVisualizationCheckBox->setMaximumWidth(40);


	ScatterPlotButton=new QPushButton2(buttonList,"ScatterPlot",ScatterBox);
	ScatterPlotButton->setMaximumWidth(60);

	QLabel *ScatterLabel = new QLabel(" Size:",ScatterBox);	

	ScatterSizeCombo = new QComboBox2(comboBoxList, ScatterBox );

	hbl_ScatterBox->addWidget(ScatterPlotVisualizationCheckBox);
	hbl_ScatterBox->addWidget(ScatterPlotButton);
	hbl_ScatterBox->addWidget(ScatterLabel);
	hbl_ScatterBox->addWidget(ScatterLabel);
	hbl_ScatterBox->addWidget(ScatterSizeCombo);

	ScatterSizeCombo->addItem("16");
	ScatterSizeCombo->addItem("32");
	ScatterSizeCombo->addItem("64");
	ScatterSizeCombo->addItem("128");
	ScatterSizeCombo->addItem("256");
	ScatterSizeCombo->addItem("512");

	//	QWidget *ToleranceBox=new QWidget(Gb2);
	QWidget *ToleranceBox=new	QWidget(Gb2);
	QHBoxLayout *hbl_ToleranceBox = new QHBoxLayout;
	hbl_ToleranceBox->setSpacing(0);
	hbl_ToleranceBox->setMargin(1);
	ToleranceBox->setLayout(hbl_ToleranceBox);
	vbl_Gb2->addWidget(ToleranceBox);
	ReversedTransformCheckBox= new QCheckBox2(checkBoxList,ToleranceBox);
	ReversedTransformCheckBox->setText("RevTrans ");
	ReversedTransformCheckBox->setChecked(false);	

	QLabel *ToleranceLabel = new QLabel("Tol(10^):",ToleranceBox);	
	ToleranceSpinBox = new QSpinBox2(spinBoxList,ToleranceBox);
	ToleranceSpinBox->setRange(-100,100);
	ToleranceSpinBox->setValue(-3);
	ToleranceSpinBox->setMinimumWidth(10);
	hbl_ToleranceBox->addWidget(ReversedTransformCheckBox);	
	hbl_ToleranceBox->addWidget(ToleranceLabel);
	hbl_ToleranceBox->addWidget(ToleranceSpinBox);
	MissregInfoLabel = new QLabel("Iter:---  Count:----  Value:--------",Gb2);
	vbl_Gb2->addWidget(MissregInfoLabel);

	//QWidget *StartStopOptimBox =new QWidget(Gb2);
	QWidget *StartStopOptimBox=new	QWidget(Gb2);
	QHBoxLayout *hbl_StartStopOptimBox = new QHBoxLayout;
	hbl_StartStopOptimBox->setSpacing(0);
	StartStopOptimBox->setLayout(hbl_StartStopOptimBox);
	vbl_Gb2->addWidget(StartStopOptimBox);
	vbl_Gb2->setSpacing(0);
	vbl_Gb2->setMargin(1);

	StartRegistrationButton=new QPushButton2(buttonList,"Start",StartStopOptimBox);
	StopRegistrationButton=new QPushButton2(buttonList,"Stop",StartStopOptimBox);

	hbl_StartStopOptimBox->addWidget(StartRegistrationButton);
	hbl_StartStopOptimBox->addWidget(StopRegistrationButton);
	//


	//QWidget *ResolBox=new QWidget(Gb2);
	QWidget *ResolBox=new	QWidget(Gb2);
	QHBoxLayout *hbl_ResolBox = new QHBoxLayout;
	hbl_ResolBox->setSpacing(0);
	hbl_ResolBox->setMargin(1);
	ResolBox->setLayout(hbl_ResolBox);
	vbl_Gb2->addWidget(ResolBox);
	QLabel *ResolLabel = new QLabel("Res: ",ResolBox);	

	ResolXSpinBox = new QSpinBox2(spinBoxList,ResolBox);
	ResolXSpinBox->setValue(74);
	ResolXSpinBox->setMinimumWidth(20);
	ResolYSpinBox = new QSpinBox2(spinBoxList,ResolBox);
	ResolYSpinBox->setValue(74);
	ResolYSpinBox->setMinimumWidth(20);
	ResolZSpinBox = new QSpinBox2(spinBoxList,ResolBox);
	ResolZSpinBox->setValue(74);
	ResolZSpinBox->setMinimumWidth(20);

	ResolXSpinBox->setRange(8,1024);
	ResolYSpinBox->setRange(8,1024);
	ResolZSpinBox->setRange(8,1024);
	hbl_ResolBox->addWidget(ResolLabel);
	hbl_ResolBox->addWidget(ResolXSpinBox);
	hbl_ResolBox->addWidget(ResolYSpinBox);
	hbl_ResolBox->addWidget(ResolZSpinBox);
	hbl_ResolBox->addWidget(ResolXSpinBox);
	//QWidget *SizeBox=new QWidget(Gb2);
	QWidget *SizeBox=new	QWidget(Gb2);
	QHBoxLayout *hbl_SizeBox = new QHBoxLayout;
	hbl_SizeBox->setSpacing(0);
	hbl_SizeBox->setMargin(1);
	SizeBox->setLayout(hbl_SizeBox);
	vbl_Gb2->addWidget(SizeBox);
	QLabel *TTransSizeLabel = new QLabel("Trans: ",SizeBox);	
	TTransSizeEdit=new QLineEdit2(lineEditList,SizeBox);
	TTransSizeEdit->setValidator(Validator);
	TTransSizeEdit->setMaximumWidth(50);
	TTransSizeEdit->setText("15.0");

	QLabel *TRotSizeLabel = new QLabel("Rot: ",SizeBox);	
	TRotSizeEdit=new QLineEdit2(lineEditList,SizeBox);
	TRotSizeEdit->setValidator(Validator);
	TRotSizeEdit->setMaximumWidth(50);
	TRotSizeEdit->setText("15.0");
	hbl_SizeBox->addWidget(TTransSizeLabel);
	hbl_SizeBox->addWidget(TTransSizeEdit);
	hbl_SizeBox->addWidget(TRotSizeLabel);
	hbl_SizeBox->addWidget(TRotSizeEdit);
	///////////////////////////

	///-----------------
	ShowVolumeRegistrationGraphButton=new  QPushButton2(buttonList,"Generate Graphs ...",Gb2);
	vbl_Gb2->addWidget(ShowVolumeRegistrationGraphButton);
	VolumeRegistrationGraphWidget=new QWidget();
	VolumeRegistrationGraphWidget->setWindowTitle("Generate Graphs Panel");
	connect(ShowVolumeRegistrationGraphButton,SIGNAL(clicked()),VolumeRegistrationGraphWidget,SLOT(show()));

	QVBoxLayout* VolumeRegistrationGraphLayout= new QVBoxLayout( VolumeRegistrationGraphWidget);//, 10, -1);
	VolumeRegistrationGraphWidget->setLayout(VolumeRegistrationGraphLayout);
	//QGroupBox *Gb4=new	QGroupBox(SegmentationTab);


	//Q3GroupBox *VolumeRegistrationGraphGroupBox=new Q3GroupBox(30,Vertical,VolumeRegistrationGraphWidget);
	QGroupBox *VolumeRegistrationGraphGroupBox=new	QGroupBox(VolumeRegistrationGraphWidget);
	QVBoxLayout *vbl_VolumeRegistrationGraphGroupBox = new QVBoxLayout;
	VolumeRegistrationGraphGroupBox->setLayout(vbl_VolumeRegistrationGraphGroupBox);	
	VolumeRegistrationGraphLayout->addWidget(VolumeRegistrationGraphGroupBox);

	//---------------------
	//////////////////////////





	//	QWidget *GraphBox=new QWidget(VolumeRegistrationGraphGroupBox);
	QGroupBox *GraphBox=new	QGroupBox(VolumeRegistrationGraphGroupBox);
	QHBoxLayout *hbl_GraphBox = new QHBoxLayout;
	GraphBox->setLayout(hbl_GraphBox);
	vbl_VolumeRegistrationGraphGroupBox->addWidget(GraphBox);
	GraphXSpinBox = new QSpinBox2(spinBoxList,GraphBox);
	GraphXSpinBox->setValue(1);
	GraphXSpinBox->setMinimumWidth(28);
	GraphYSpinBox = new QSpinBox2(spinBoxList,GraphBox);
	GraphYSpinBox->setValue(2);
	GraphYSpinBox->setMinimumWidth(28);
	GraphZSpinBox = new QSpinBox2(spinBoxList,GraphBox);
	GraphZSpinBox->setValue(3);
	GraphZSpinBox->setMinimumWidth(28);

	GraphXSpinBox->setRange(1,6);
	GraphYSpinBox->setRange(1,6);
	GraphZSpinBox->setRange(1,6);

	Generate3DMisregPolyGraphButton=new QPushButton2(buttonList,"3DVol",GraphBox);
	GeneratePolyMisregistrationGraphButton=new QPushButton2(buttonList,"2DPoly",GraphBox);
	Generate3DMisregPolyGraphButton->setMinimumWidth(35);
	GeneratePolyMisregistrationGraphButton->setMinimumWidth(35);

	hbl_GraphBox->addWidget(GraphXSpinBox);
	hbl_GraphBox->addWidget(GraphYSpinBox);
	hbl_GraphBox->addWidget(GraphZSpinBox);
	hbl_GraphBox->addWidget(Generate3DMisregPolyGraphButton);
	hbl_GraphBox->addWidget(GeneratePolyMisregistrationGraphButton);

	//QWidget *GraphBox2=new QWidget(VolumeRegistrationGraphGroupBox);
	QGroupBox *GraphBox2=new	QGroupBox(VolumeRegistrationGraphGroupBox);
	QHBoxLayout *hbl_GraphBox2 = new QHBoxLayout;
	GraphBox2->setLayout(hbl_GraphBox2);
	vbl_VolumeRegistrationGraphGroupBox->addWidget(GraphBox2);

	Generate6DRandomGraphButton=new QPushButton2(buttonList,"6DRandom",GraphBox2);
	GenerateMisregistrationGraphsButton=new QPushButton2(buttonList,"Gnuplot2DGraphs",GraphBox2);
	Generate6DRandomGraphButton->setMinimumWidth(45);
	GenerateMisregistrationGraphsButton->setMinimumWidth(95);

	hbl_GraphBox->addWidget(Generate6DRandomGraphButton);
	hbl_GraphBox->addWidget(GenerateMisregistrationGraphsButton);

	connect(ScatterPlotButton,SIGNAL(clicked ()),this,SLOT(SlotScatterPlotButtonPressed()));
	connect(VolumeTransformButton,SIGNAL(clicked ()),this,SLOT(SlotVolumeTransformButtonPressed()));
	connect(StartRegistrationButton,SIGNAL(clicked ()),this,SLOT(SlotStartRegistrationButtonPressed()));


	connect(GenerateMisregistrationGraphsButton,SIGNAL(clicked ()),this,SLOT(SlotGenerateMisregistrationGraphsButtonPressed()));
	connect(GeneratePolyMisregistrationGraphButton,SIGNAL(clicked ()),this,SLOT(SlotGeneratePolyGraphButtonPressed()));
	connect(Generate6DRandomGraphButton,SIGNAL(clicked ()),this,SLOT(SlotGenerate6DGraphButtonPressed()));
	connect(Generate3DMisregPolyGraphButton,SIGNAL(clicked ()),this,SLOT(SlotGenerate3DVolumeGraphButtonPressed()));


	/////////
	///////////////
	//----------------------------------------------------------

	//Q3GroupBox *GbX=new Q3GroupBox(20,Vertical,Info);
	QGroupBox *GbX=new	QGroupBox(Info);
	QVBoxLayout *vbl_GbX = new QVBoxLayout;
	GbX->setLayout(vbl_GbX);	
	MainLayout->addWidget(GbX);
	GbX->setWindowTitle("Feature Point Registration");
	//	GbX->setInsideSpacing(3);
	//	GbX->setInsideMargin(5);



	//FeaturePointsWidget=new QWidget();
	//FeaturePointsWidget->setWindowTitle("Feature Points Panel");
	//ShowFeaturePointsWidgetButton=new  QPushButton2(buttonList,"Feature Points Panel",GbX);
	//vbl_GbX->addWidget(ShowFeaturePointsWidgetButton);	
	//connect(ShowFeaturePointsWidgetButton,SIGNAL(clicked()),FeaturePointsWidget,SLOT(show()));

	////	QVBoxLayout* FeaturePointsLayout= new QVBoxLayout( FeaturePointsWidget, 10, -1);
	////	Q3GroupBox *FeaturePointsGroupBox=new Q3GroupBox(30,Vertical,FeaturePointsWidget);
	//QVBoxLayout* FeaturePointsLayout= new QVBoxLayout( FeaturePointsWidget);//, 10, -1);
	//FeaturePointsWidget->setLayout(FeaturePointsLayout);

	//QGroupBox *FeaturePointsGroupBox=new	QGroupBox(FeaturePointsWidget);
	//QVBoxLayout *vbl_FeaturePointsGroupBox = new QVBoxLayout;
	//FeaturePointsGroupBox->setLayout(vbl_FeaturePointsGroupBox);	
	//FeaturePointsGroupBox->layout()->setSpacing(1);
	//FeaturePointsGroupBox->layout()->setMargin(2);

	//FeaturePointsLayout->addWidget(FeaturePointsGroupBox);

	//FeaturePointsGroupBox->setWindowTitle("Feature Points Editor");
	////////////////////////////////////
	//////////////////////////////////////
	////----------


	////QWidget *FeaturePointBox=new QWidget(FeaturePointsGroupBox);
	//QWidget *FeaturePointBox=new	QWidget(FeaturePointsGroupBox);
	//QHBoxLayout *hbl_FeaturePointBox = new QHBoxLayout;
	//hbl_FeaturePointBox->setMargin(0);
	//hbl_FeaturePointBox->setSpacing(0);
	//FeaturePointBox->setLayout(hbl_FeaturePointBox);
	//vbl_FeaturePointsGroupBox->addWidget(FeaturePointBox);

	////Q3GroupBox *ModelBox=new Q3GroupBox(20,Vertical,FeaturePointBox);
	//QGroupBox *ModelBox=new	QGroupBox(FeaturePointBox);
	//QVBoxLayout *hbl_ModelBox = new QVBoxLayout;
	//hbl_ModelBox->setMargin(0);
	//hbl_ModelBox->setSpacing(0);
	//ModelBox->setLayout(hbl_ModelBox);
	//hbl_FeaturePointBox->addWidget(ModelBox);

	//ModelBox->setTitle("Model Points");
	//ModelListBox=new Q3ListBox(ModelBox);
	//hbl_ModelBox->addWidget(ModelListBox);

	////QWidget *FeaturePointBoxM=new QWidget(ModelBox);

	//QGroupBox *FeaturePointBoxM=new	QGroupBox(Gb1);
	//QHBoxLayout *hbl_FeaturePointBoxM = new QHBoxLayout;
	//hbl_FeaturePointBoxM->setMargin(0);
	//hbl_FeaturePointBoxM->setSpacing(0);
	//FeaturePointBoxM->setLayout(hbl_FeaturePointBoxM);
	//hbl_ModelBox->addWidget(FeaturePointBoxM);

	//AddModelPointPushButton=new QPushButton2(buttonList,"Add",FeaturePointBoxM);
	//ClearModelPointList=new QPushButton2(buttonList,"ClrAll",FeaturePointBoxM);
	//hbl_FeaturePointBoxM->addWidget(AddModelPointPushButton);
	//hbl_FeaturePointBoxM->addWidget(ClearModelPointList);

	//int buttonsize=30;

	//ModelBox->setMinimumWidth(80);
	//ModelListBox->setMinimumWidth(buttonsize);
	//ModelListBox->setMinimumHeight(20);
	//AddModelPointPushButton->setFixedWidth(buttonsize);
	//ClearModelPointList->setFixedWidth(buttonsize);



	//Q3GroupBox *ObjectBox=new Q3GroupBox(20,Vertical,FeaturePointBox);	

	//QGroupBox *ObjectBox=new	QGroupBox(FeaturePointBox);
	//QVBoxLayout *vbl_ObjectBox = new QVBoxLayout;
	//vbl_ObjectBox->setMargin(0);
	//vbl_ObjectBox->setSpacing(0);
	//ObjectBox->setLayout(vbl_ObjectBox);	
	//hbl_FeaturePointBox->addWidget(ObjectBox);

	//ObjectBox->setTitle("Object Points");
	//ObjectListBox=new Q3ListBox(ModelBox);

	////QWidget *FeaturePointBoxO=new QWidget(ObjectBox);
	//QWidget *FeaturePointBoxO=new	QWidget(ObjectBox);
	//QHBoxLayout *hbl_FeaturePointBoxO = new QHBoxLayout;
	//hbl_FeaturePointBoxO->setMargin(0);
	//hbl_FeaturePointBoxO->setSpacing(0);
	//FeaturePointBoxO->setLayout(hbl_FeaturePointBoxO);

	//vbl_ObjectBox->addWidget(ObjectListBox);
	//vbl_ObjectBox->addWidget(FeaturePointBoxO);

	//AddObjectPointPushButton=new QPushButton2(buttonList,"Add",FeaturePointBoxO);
	//ClearObjectPointList=new QPushButton2(buttonList,"ClrAll",FeaturePointBoxO);

	//hbl_FeaturePointBoxO->addWidget(AddObjectPointPushButton);
	//hbl_FeaturePointBoxO->addWidget(ClearObjectPointList);

	//ObjectBox->setMinimumWidth(80);
	//ObjectListBox->setMinimumWidth(buttonsize);
	//ObjectListBox->setMinimumHeight(20);

	//AddObjectPointPushButton->setFixedWidth(buttonsize);
	//ClearObjectPointList->setFixedWidth(buttonsize);


	//StartFeaturePointOptimization=new QPushButton2(buttonList,"Start Optimization",GbX);

	//vbl_GbX->addWidget(StartFeaturePointOptimization);

	////////////////////////////////////////////////////////
	////////////////////////////////////
	////////////////////////////////////


	//Q3GroupBox *GbY=new Q3GroupBox(20,Vertical,Info);
	QGroupBox *GbY=new	QGroupBox(Info);
	QVBoxLayout *vbl_GbY = new QVBoxLayout;
	GbY->setLayout(vbl_GbY);	
	GbY->layout()->setSpacing(1);
	GbY->layout()->setMargin(2);

	MainLayout->addWidget(GbY);

	GbY->setTitle("Elastic Registration");
	//	GbY->setInsideSpacing(3);
	//	GbY->setInsideMargin(5);


	//QWidget *ElasticRegistrationBox =new QWidget(GbY);
	QWidget *ElasticRegistrationBox=new	QWidget(Gb1);
	QHBoxLayout *hbl_ElasticRegistrationBox = new QHBoxLayout;

	ElasticRegistrationBox->setLayout(hbl_ElasticRegistrationBox);
	
	ElasticRegistrationBox->layout()->setSpacing(1);
	ElasticRegistrationBox->layout()->setMargin(2);

	StartElasticVolumeRegistrationButton=new QPushButton2(buttonList,"Start",ElasticRegistrationBox);
	StopElasticVolumeRegistrationButton=new QPushButton2(buttonList,"Stop",ElasticRegistrationBox);
	hbl_ElasticRegistrationBox->addWidget(StartElasticVolumeRegistrationButton);
	hbl_ElasticRegistrationBox->addWidget(StopElasticVolumeRegistrationButton);

	//QHBox *ElasticBox1=new QHBox(GbY);
	ApplyElasticVolumeTransformation=new QPushButton2(buttonList,"Transform",GbY);
	//ApplyElasticVolumeTransformation->setMinimumWidth(40);



	///-----------------
	ShowVolumeElasticRegistrationPanelButton=new  QPushButton2(buttonList,"Parameters",GbY);

	vbl_GbY->addWidget(ElasticRegistrationBox);
	vbl_GbY->addWidget(ApplyElasticVolumeTransformation);
	vbl_GbY->addWidget(ShowVolumeElasticRegistrationPanelButton);

	VolumeElasticRegistrationWidget=new QWidget();
	VolumeElasticRegistrationWidget->setWindowTitle("Volume Elastic Registration Panel");
	connect(ShowVolumeElasticRegistrationPanelButton,SIGNAL(clicked()),VolumeElasticRegistrationWidget,SLOT(show()));

	//QVBoxLayout* VolumeElasticRegistrationLayout= new QVBoxLayout( VolumeElasticRegistrationWidget, 10, -1);
	//Q3GroupBox *VolumeElasticRegistratioGroupBox=new Q3GroupBox(30,Vertical,VolumeElasticRegistrationWidget);
	QVBoxLayout* VolumeElasticRegistrationLayout= new QVBoxLayout( VolumeElasticRegistrationWidget);//, 10, -1);
	VolumeElasticRegistrationLayout->setSpacing(1);
	VolumeElasticRegistrationWidget->setLayout(VolumeElasticRegistrationLayout);

	QGroupBox *VolumeElasticRegistratioGroupBox=new	QGroupBox(VolumeElasticRegistrationWidget);
	QVBoxLayout *vbl_VolumeElasticRegistratioGroupBox = new QVBoxLayout;
	vbl_VolumeElasticRegistratioGroupBox->setSpacing(1);
	vbl_VolumeElasticRegistratioGroupBox->setMargin(1);
	VolumeElasticRegistratioGroupBox->setLayout(vbl_VolumeElasticRegistratioGroupBox);	
	VolumeElasticRegistrationLayout->addWidget(VolumeElasticRegistratioGroupBox);

	//VolumeElasticRegistratioGroupBox->setTitle("Feature Points Editor");



	//---------------------
	//QWidget *GridSizeXXBox=new QWidget(VolumeElasticRegistratioGroupBox);	
	QWidget *GridSizeXXBox=new	QWidget(VolumeElasticRegistratioGroupBox);
	QHBoxLayout *hbl_GridSizeXXBox = new QHBoxLayout;
	hbl_GridSizeXXBox->setSpacing(1);
	GridSizeXXBox->setLayout(hbl_GridSizeXXBox);
	vbl_VolumeElasticRegistratioGroupBox->addWidget(GridSizeXXBox);


	QLabel *MultiResolutionStepL4=new QLabel("Multi-Res Step:",GridSizeXXBox);
	MultiResolutionStepBox = new QSpinBox2(spinBoxList,GridSizeXXBox);
	MultiResolutionStepBox->setRange(1,512);
	MultiResolutionStepBox->setValue(4);
	MultiResolutionStepBox->setMaximumWidth(40);

	ElasticUpdateGridBox=new QCheckBox2(checkBoxList,GridSizeXXBox);
	ElasticUpdateGridBox->setText("Update");
	ElasticUpdateGridBox->setChecked(false);
	hbl_GridSizeXXBox->addWidget(MultiResolutionStepL4);
	hbl_GridSizeXXBox->addWidget(MultiResolutionStepBox);
	hbl_GridSizeXXBox->addWidget(ElasticUpdateGridBox);

	//	QWidget *GridSizeBox=new QWidget(VolumeElasticRegistratioGroupBox);	
	QWidget *GridSizeBox=new	QWidget(VolumeElasticRegistratioGroupBox);
	QHBoxLayout *hbl_GridSizeBox = new QHBoxLayout;
	hbl_GridSizeBox->setSpacing(1);
	GridSizeBox->setLayout(hbl_GridSizeBox);
	vbl_VolumeElasticRegistratioGroupBox->addWidget(GridSizeBox);
	//	QLabel *GridSizeL0=new QLabel("Spc:",GridSizeBox);

	QLabel *GridSizeL1=new QLabel(" X:",GridSizeBox);
	GridSizeX1Box = new QSpinBox2(spinBoxList,GridSizeBox);
	GridSizeX1Box->setRange(2,512);
	GridSizeX1Box->setValue(8);
	GridSizeX1Box->setMaximumWidth(40);

	QLabel *GridSizeL2=new QLabel(" Y:",GridSizeBox);
	GridSizeY1Box = new QSpinBox2(spinBoxList,GridSizeBox);
	GridSizeY1Box->setRange(2,512);
	GridSizeY1Box->setValue(8);
	GridSizeY1Box->setMaximumWidth(40);

	QLabel *GridSizeL3=new QLabel("Z:",GridSizeBox);
	GridSizeZ1Box = new QSpinBox2(spinBoxList,GridSizeBox);
	GridSizeZ1Box->setValue(8);
	GridSizeZ1Box->setMaximumWidth(40);
	GridSizeZ1Box->setRange(2,512);
	///////////////
	hbl_GridSizeBox->addWidget(GridSizeL1);
	hbl_GridSizeBox->addWidget(GridSizeX1Box);
	hbl_GridSizeBox->addWidget(GridSizeL2);
	hbl_GridSizeBox->addWidget(GridSizeY1Box);
	hbl_GridSizeBox->addWidget(GridSizeL3);
	hbl_GridSizeBox->addWidget(GridSizeZ1Box);
	////////////////

	//	QWidget *OptimParamsBox=new QWidget(VolumeElasticRegistratioGroupBox);
	QWidget *OptimParamsBox=new	QWidget(VolumeElasticRegistratioGroupBox);
	QHBoxLayout *hbl_OptimParamsBox = new QHBoxLayout;
	hbl_OptimParamsBox->setSpacing(1);
	OptimParamsBox->setLayout(hbl_OptimParamsBox);
	vbl_VolumeElasticRegistratioGroupBox->addWidget(OptimParamsBox);

	QLabel *ElasticIterL3=new QLabel("Iter:",OptimParamsBox);
	ElasticIterBox = new QSpinBox2(spinBoxList,OptimParamsBox);
	ElasticIterBox->setValue(1);
	ElasticIterBox->setMaximumWidth(40);

	QLabel *ElasticStepL4=new QLabel("Step:",OptimParamsBox);
	ElasticStepBox = new QSpinBox2(spinBoxList,OptimParamsBox);
	ElasticStepBox->setValue(2);
	ElasticStepBox->setMaximumWidth(40);

	ElasticIterBox->setRange(1,512);
	ElasticStepBox->setRange(1,512);

	hbl_OptimParamsBox->addWidget(ElasticIterL3);
	hbl_OptimParamsBox->addWidget(ElasticIterBox);
	hbl_OptimParamsBox->addWidget(ElasticStepL4);
	hbl_OptimParamsBox->addWidget(ElasticStepBox);


	//QWidget *OptimParams2Box=new QWidget(VolumeElasticRegistratioGroupBox);
	QWidget *OptimParams2Box=new	QWidget(VolumeElasticRegistratioGroupBox);
	QHBoxLayout *hbl_OptimParams2Box = new QHBoxLayout;
	hbl_OptimParams2Box->setSpacing(1);
	OptimParams2Box->setLayout(hbl_OptimParams2Box);
	vbl_VolumeElasticRegistratioGroupBox->addWidget(OptimParams2Box);

	ElasticRandomSamplingBox=new QCheckBox2(checkBoxList,OptimParams2Box);
	ElasticRandomSamplingBox->setText("Rand");
	ElasticRandomSamplingBox->setChecked(false);

	ElasticRegistrationFixedBoundsBox=new QCheckBox2(checkBoxList,OptimParams2Box);
	ElasticRegistrationFixedBoundsBox->setText("Fixed Bounds");
	ElasticRegistrationFixedBoundsBox->setChecked(true);

	hbl_OptimParams2Box->addWidget(ElasticRandomSamplingBox);
	hbl_OptimParams2Box->addWidget(ElasticRegistrationFixedBoundsBox);

	//MainLayout->addWidget(Gb1);
	//MainLayout->addWidget(Gb2);
	///MainLayout->addWidget(GbX);
	//MainLayout->addWidget(GbY);




	//qtab->addTab(Info,"&Registration ");
}

//----------------------------------------------------------

void	GUIPanelWidget::SlotSetTransformationModeToRigid()
{
	int i;
	for(i=0;i<6;i++)
	{
		TCheckBox[i]->setChecked(true);
	}
	for(i=6;i<12;i++)
	{
		TCheckBox[i]->setChecked(false);
	}


}

//----------------------------------------------------------

void	GUIPanelWidget::SlotSetTransformationModeToAffine()
{
	int i;
	for(i=0;i<12;i++)
	{
		TCheckBox[i]->setChecked(true);
	}
}
//------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------

void	GUIPanelWidget::SlotUpdateTolerance(int)
{


	//double temp;
	double temp2;
	int temp3;
	QString tempString;

	double Step=5.0;//5procent

	if (ToleranceSlider->value()!=0)
	{
		//temp=ToleranceEdit->text().toFloat();

		temp2=TempPercent+((double)ToleranceSlider->value()*Step)/100.0;
		temp3=Round(((double)MaxToleranceValue*temp2)/100.0);





		disconnect(ToleranceEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTolerancePercent(  const QString & )));
		disconnect(ToleranceValueEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateToleranceValue(  const QString & )));

		if ((temp2>0)&&(temp2<=100.0))
		{
			tempString.setNum(temp2);
		}
		else if (temp2<=0)
		{
			tempString.setNum(0.0);
		}
		else
		{
			tempString.setNum(100.0);
		}
		ToleranceEdit->setText(tempString);




		if ((temp3>0)&&(temp3<=MaxToleranceValue))
		{
			tempString.setNum(temp3);
		}	
		else if (temp3<=0)
		{
			tempString.setNum(0);
		}
		else
		{
			tempString.setNum(MaxToleranceValue);
		}
		ToleranceValueEdit->setText(tempString);


		connect(ToleranceEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTolerancePercent(  const QString & )));
		connect(ToleranceValueEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateToleranceValue(  const QString & )));


	}

}

//--------------------------------------------------

void	GUIPanelWidget::SlotUpdateTolerance()
{

	if(ToleranceSlider->value()!=0)
	{
		disconnect(ToleranceSlider,SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateTolerance(int)));	
		ToleranceSlider->setValue(0);
		TempPercent=ToleranceEdit->text().toFloat();


		connect(ToleranceSlider,SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateTolerance(int)));

	}





}

//--------------------------------------------------

void	GUIPanelWidget::SlotUpdateTolerancePercent(const QString &)
{



	double temp;

	int temp3;
	QString tempString;

	TempPercent=ToleranceEdit->text().toFloat();
	temp=TempPercent;

	//	temp2=TParams[i]+((double)ToleranceSlider->value()*Step)/100.0;

	temp3=Round(((double)MaxToleranceValue*temp)/100.0);





	//		disconnect(ToleranceEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTolerancePercent(  const QString & )));
	disconnect(ToleranceValueEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateToleranceValue(  const QString & )));

	if ((temp3>0)&&(temp3<=MaxToleranceValue))
	{
		tempString.setNum(temp3);
	}	
	else if (temp3<=0)
	{
		tempString.setNum(0);
	}
	else
	{
		tempString.setNum(MaxToleranceValue);
	}
	ToleranceValueEdit->setText(tempString);

	//		connect(ToleranceEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTolerancePercent(  const QString & )));
	connect(ToleranceValueEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateToleranceValue(  const QString & )));


}

//--------------------------------------------------

void	GUIPanelWidget::SlotUpdateToleranceValue()
{
	SlotUpdateTolerancePercent(ToleranceEdit->text());


}
//--------------------------------------------------

void	GUIPanelWidget::SlotUpdateToleranceValue(const QString &)
{


	double temp;

	double temp2;
	QString tempString;

	TempPercent=ToleranceEdit->text().toFloat();
	temp=TempPercent;


	temp2=(100.0*temp)/(double)MaxToleranceValue;





	disconnect(ToleranceEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTolerancePercent(  const QString & )));
	//		disconnect(ToleranceValueEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateToleranceValue(  const QString & )));

	if ((temp2>0)&&(temp2<=100.0))
	{
		tempString.setNum(temp2);
	}
	else if (temp2<=0)
	{
		tempString.setNum(0.0);
	}
	else
	{
		tempString.setNum(100.0);
	}
	ToleranceEdit->setText(tempString);

	//		tempString.setNum(temp3);
	//		ToleranceValueEdit->setText(tempString);		

	connect(ToleranceEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTolerancePercent(  const QString & )));
	//		connect(ToleranceValueEdit,SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateToleranceValue(  const QString & )));

}

//--------------------------------------------------

//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------
void GUIPanelWidget::SlotUpdateTransformation(int empty)
{

	int i;
	double temp;
	QString tempString;
	//	if (TSlider[6]->value()==0)
	for(i=0;i<13;i++)
	{
		if (TSlider[i]->value()!=0)
		{

			if (i!=12)
				temp=TParams[i]+((double)TSlider[i]->value()*TParams[12])/100.0;
			else
			{

				temp=TParams[i]+(double)TSlider[i]->value();

				if (temp<0) temp=0;
			}

			tempString.setNum(temp);

			disconnect(TEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTransformation(  const QString & )));
			TEdit[i]->setText(tempString);		
			connect(TEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateTransformation(  const QString & )));

		}
	}
	/*	else
	{
	i=6;

	temp=TParams[i]+(double)TSlider[i]->value();
	tempString.setNum(temp);
	TEdit[i]->setText(tempString);

	}
	*/


}


//----------------------------------------------
 

void GUIPanelWidget::SlotUpdateTransformation()
{
	int i;
	for(i=0;i<13;i++)
	{
		if(TSlider[i]->value()!=0)
		{
			disconnect(TSlider[i],SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateTransformation(int)));	
			TSlider[i]->setValue(0);
			connect(TSlider[i],SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateTransformation(int)));

			TParams[i]=TEdit[i]->text().toFloat();
		}
	}
}
//----------------------------------------------


void GUIPanelWidget::SlotUpdateTransformation( const QString &Empty)
{

	int i;
	for(i=0;i<13;i++)
	{
		TParams[i]=TEdit[i]->text().toFloat();
	}

}


//----------------------------------------------


void GUIPanelWidget::SlotScatterPlotButtonPressed()
{
	SlotUpdateTransformationParameters();
	emit SignalSendTransformation(TParams);
}

//----------------------------------------------

void GUIPanelWidget::SlotVolumeTransformButtonPressed()
{

	SlotUpdateTransformationParameters();
	emit SignalSendTransformation2(TParams);


}
void GUIPanelWidget::SlotSurfacesTransformButtonPressed()
{

	SlotUpdateTransformationParameters();
	emit SignalSendTransformationForSurfaces(TParams);


}

//----------------------------------------------

void GUIPanelWidget::SlotGenerateMisregistrationGraphsButtonPressed()
{

	SlotUpdateTransformationParameters();
	emit SignalSendTransformation3(TParams);


}

//----------------------------------------------

void GUIPanelWidget::SlotStartRegistrationButtonPressed()
{


	SlotUpdateTransformationParameters();
	emit SignalSendTransformation4(TParams);


}


//----------------------------------------------

void GUIPanelWidget::SlotGeneratePolyGraphButtonPressed()
{


	SlotUpdateTransformationParameters();
	emit SignalSendTransformation5(TParams);


}


//----------------------------------------------


void	GUIPanelWidget::SlotGenerate3DVolumeGraphButtonPressed()
{
	SlotUpdateTransformationParameters();
	emit SignalSendTransformation6(TParams);
}


//----------------------------------------------

void	GUIPanelWidget::SlotGenerate6DGraphButtonPressed()
{
	SlotUpdateTransformationParameters();
	emit SignalSendTransformation7(TParams);
}


//----------------------------------------------




//----------------------------------------------

void GUIPanelWidget::SlotInsertNewTransformation(double *TransX)
{
	int i;
	QString nr;
	double a;
	for(i=0;i<12;i++)
	{	
		a=((i>2)&&(i<6))? to_deg:1.0;
		nr.setNum(TransX[i+1]*a);
		TEdit[i]->setText(nr);
	}




}

//----------------------------------------------

void GUIPanelWidget::SlotInsertNewTransformation2(double *TransX)
{
	int i;
	QString nr;

	for(i=0;i<12;i++)
	{	

		nr.setNum(TransX[i+1]);
		TEdit[i]->setText(nr);
	}


	if (TransX[13]!=0)
	{
		for(i=13;i<16;i++)
		{
			nr.setNum(TransX[i+1]);
			TEdit[i]->setText(nr);
		}

		AutoCenterCheckBox->setChecked(false);
	}
	else
		AutoCenterCheckBox->setChecked(true);


}

//-------------------------------------------------------

void GUIPanelWidget::SlotUpdateTransformationParameters()
{
	int i;
	TParams[13]=AutoCenterCheckBox->isChecked();
	for(i=14;i<17;i++)
		TParams[i]=TEdit[i-1]->text().toFloat();


	TParams[17]=AlignCentersCheckBox->isChecked();

	TParams[18]=RegStepXBox->value();
	TParams[19]=RegStepYBox->value();
	TParams[20]=RegStepZBox->value();
	TParams[21]=ScatterSizeCombo->currentIndex(); //scattertab
	TParams[22]=ReversedTransformCheckBox->isChecked(); //Reversed Transformation
	TParams[23]=InterpolationCombo->currentIndex(); // Interpolation
	TParams[24]=ScatterPlotVisualizationCheckBox->isChecked(); // Interpolation
	TParams[25]=MisregistrationFunctionCombo->currentIndex(); // Misregistration Function
	TParams[26]=ToleranceSpinBox->value(); // tolerance 
	TParams[27]=GraphXSpinBox->value();	 //X parameter of misregistration graph
	TParams[28]=GraphYSpinBox->value();  //Y parameter of misregistration graph
	TParams[29]=GraphZSpinBox->value();  //Z parameter of misregistration graph
	TParams[30]=ResolXSpinBox->value();  //X resolution of misregistration graph
	TParams[31]=ResolYSpinBox->value();  //Y resolution of misregistration graph
	TParams[32]=ResolZSpinBox->value();  //Z resolution of misregistration graph

	TParams[33]=TTransSizeEdit->text().toFloat();  //Y resolution of misregistration graph
	TParams[34]=TRotSizeEdit->text().toFloat();  //Z resolution of misregistration graph


	// which parameters are to be optimized
	for(i=0;i<12;i++)
		TParams[35+i]=TCheckBox[i]->isChecked();



}

//---------------------------------------------------------------

void GUIPanelWidget::SlotResetTransformation()
{
	double *aaa=new double[13];

	double nr=0.0;
	for(int i=0;i<13;i++)
	{
		if (i==10) nr=1.0;
		aaa[i]=nr;
	}
	SlotInsertNewTransformation(aaa);
}
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//------------ SURFACE DATA SET ITEMS ---------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//--------------------------------------------------------------- 
void	GUIPanelWidget::CreateSurfacePropertyTab(MyDockWidget *qtab)
{
	TextFieldDoubleValidator *validator =new TextFieldDoubleValidator(-1000000,1000000.0,4,this);
	validator->setNotation(TextFieldDoubleValidator::StandardNotation);
	TextFieldDoubleValidator *validatorDeg =new TextFieldDoubleValidator(-180,180,4,this);
	validatorDeg->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *validatorInt =new QIntValidator(0,10000000,this);
	TextFieldDoubleValidator *validatorNormal =new TextFieldDoubleValidator(-1,1,4,this);
	validatorNormal->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *validatorIter =new QIntValidator(1,100,this);
	QWidget *xWidget;
	QLabel *xLabel;
	QGroupBox *gBox;

	QWidget *Info = new QWidget(qtab);
	qtab->setScrollWidget(Info);
	QVBoxLayout* MainLayout= new QVBoxLayout( Info);//, 10, -1);
	Info->setLayout(MainLayout);
	MainLayout->setSpacing(1);
	QGroupBox *Gb1=new	QGroupBox(Info);
	QVBoxLayout *vbl_Gb1 = new QVBoxLayout;
	Gb1->setLayout(vbl_Gb1);	
	Gb1->layout()->setSpacing(1);
	Gb1->layout()->setMargin(2);
	MainLayout->addWidget(Gb1);


	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	xLabel=new QLabel("Shader:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	shaderSelectComboBox=new QComboBox2(comboBoxList,xWidget);
	shaderSelectComboBox->addItem(0,"none");
	xWidget->layout()->addWidget(shaderSelectComboBox);

	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	setShaderToSurfacesButton=new QPushButton2(buttonList,"Apply Shader",xWidget);
	xWidget->layout()->addWidget(setShaderToSurfacesButton);
	showShadersCheckBox=new QCheckBox2(checkBoxList,"Use Shaders",xWidget);
	xWidget->layout()->addWidget(showShadersCheckBox);
	showShadersCheckBox->setChecked(true);




	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	xLabel=new QLabel("Property:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	standardPropertyComboBox=new QComboBox2(comboBoxList,xWidget);
	xWidget->layout()->addWidget(standardPropertyComboBox);
	setStandardPropertyToSurfacesButton=new QPushButton2(buttonList,"Apply",xWidget);
	xWidget->layout()->addWidget(setStandardPropertyToSurfacesButton);

	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	
	standardPropertyLineEdit=new QLineEdit2(lineEditList,"default",xWidget);
	xWidget->layout()->addWidget(standardPropertyLineEdit);
	AddStandardPropertyButton=new QPushButton2(buttonList,"Add Property",xWidget);
	xWidget->layout()->addWidget(AddStandardPropertyButton);
		//---------------------------
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	writeUpdatedPropsButton=new QPushButton2(buttonList,"Write Updated Props",xWidget);
	xWidget->layout()->addWidget(writeUpdatedPropsButton);
		//---------------------------
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	depthPeelingChceckBox=new QCheckBox2(checkBoxList,"Depht Peeling",xWidget);
	xWidget->layout()->addWidget(depthPeelingChceckBox);
	lineAntialiasingCheckBox=new QCheckBox2(checkBoxList,"Line Antialiasing",xWidget);
	xWidget->layout()->addWidget(lineAntialiasingCheckBox);
	lineAntialiasingCheckBox->hide();


	shadowsCheckBox = new QCheckBox2(checkBoxList,"Shadows", xWidget);
	xWidget->layout()->addWidget(shadowsCheckBox);
	
		
	//---------------------------
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	xLabel=new QLabel("Concide:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	resolveConcidentGeometryComboBox=new QComboBox2(comboBoxList,xWidget);
	xWidget->layout()->addWidget(resolveConcidentGeometryComboBox);
	resolveConcidentGeometryComboBox->addItem("Off",0);
	resolveConcidentGeometryComboBox->addItem("PolygonOffset",1);
	resolveConcidentGeometryComboBox->addItem("ShiftZBuffer",2);

	applyResolveSettingsButton=new QPushButton2(buttonList,"Apply",xWidget);
	xWidget->layout()->addWidget(applyResolveSettingsButton);

	
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);


	xLabel=new QLabel("P0:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	resolveParams[0]=new QLineEdit2(lineEditList,"0",xWidget);
	resolveParams[0]->setValidator(validator);
	xWidget->layout()->addWidget(resolveParams[0]);
	
	xLabel=new QLabel("P1:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	resolveParams[1]=new QLineEdit2(lineEditList,"1",xWidget);
	resolveParams[1]->setValidator(validator);
	xWidget->layout()->addWidget(resolveParams[1]);

	xLabel=new QLabel("P2:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	resolveParams[2]=new QLineEdit2(lineEditList,"2",xWidget);
	resolveParams[2]->setValidator(validator);
	xWidget->layout()->addWidget(resolveParams[2]);
	

	//xWidget->hide();
	

	
		//---------------------------
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	testTextureButton=new QPushButton2(buttonList,"Texture Test",xWidget);
	xWidget->layout()->addWidget(testTextureButton);

	
	xLabel=new QLabel("Scale:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	scaleTextureLineEdit=new QLineEdit2(lineEditList,"30",xWidget);
	//scaleTextureLineEdit->setValidator(validator);
	xWidget->layout()->addWidget(scaleTextureLineEdit);
	
	xLabel=new QLabel("Angle:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	angleTextureLineEdit=new QLineEdit2(lineEditList,"0.0",xWidget);
	angleTextureLineEdit->setValidator(validatorDeg);
	xWidget->layout()->addWidget(angleTextureLineEdit);


	//---------------------------
	//QCheckBox *normXTexCheckBox,*normYTexCheckBox;
	//QTextEdit *scaleXTextureLineEdit,*scaleYTextureLineEdit;
	//QTextEdit *centerXTexLineEdit,*centerYTexLineEdit;

	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);

	repeatTexCheckBox=new QCheckBox2(checkBoxList,xWidget);
	repeatTexCheckBox->setText("repeat");
	repeatTexCheckBox->setChecked(true);
	xWidget->layout()->addWidget(repeatTexCheckBox);

	normXTexCheckBox=new QCheckBox2(checkBoxList,xWidget);
	normXTexCheckBox->setText("normX");
	normXTexCheckBox->setChecked(true);
	xWidget->layout()->addWidget(normXTexCheckBox);
	normYTexCheckBox=new QCheckBox2(checkBoxList,xWidget);

	normYTexCheckBox->setText("normY");
	normYTexCheckBox->setChecked(true);
	xWidget->layout()->addWidget(normYTexCheckBox);

	//-------------------------------
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);


	xLabel=new QLabel("Scale X:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	scaleXTextureLineEdit=new QLineEdit2(lineEditList,"1.0",xWidget);
	//scaleXTextureLineEdit->setValidator(validator);
	xWidget->layout()->addWidget(scaleXTextureLineEdit);
	xLabel=new QLabel("Scale Y:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	scaleYTextureLineEdit=new QLineEdit2(lineEditList,"1.0",xWidget);
	//scaleYTextureLineEdit->setValidator(validator);
	xWidget->layout()->addWidget(scaleYTextureLineEdit);


	//-------------------------------
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);

	xLabel=new QLabel("Tex OX:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	centerXTexLineEdit=new QLineEdit2(lineEditList,"0.5",xWidget);
	centerXTexLineEdit->setValidator(validator);
	xWidget->layout()->addWidget(centerXTexLineEdit);
	xLabel=new QLabel("Tex OY:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	centerYTexLineEdit=new QLineEdit2(lineEditList,"0.5",xWidget);
	centerYTexLineEdit->setValidator(validator);
	xWidget->layout()->addWidget(centerYTexLineEdit);

		//---------------------------
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);

	xLabel=new QLabel("CPoint:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	startPointTextureLineEdit=new QLineEdit2(lineEditList,"0",xWidget);
	startPointTextureLineEdit->setValidator(validatorInt);
	xWidget->layout()->addWidget(startPointTextureLineEdit);
	findClosestPointTo3DPointerPushButton=new QPushButton2(buttonList,"Find",xWidget);
	findClosestPointTo3DPointerPushButton->setFixedWidth(30);
	xWidget->layout()->addWidget(findClosestPointTo3DPointerPushButton);

	randomTexSeedCheckBox=new QCheckBox2(checkBoxList,"Random",xWidget);
	randomTexSeedCheckBox->setChecked(true);
	xWidget->layout()->addWidget(randomTexSeedCheckBox);

	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);


	xLabel=new QLabel("Orient:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	orientTextureLineEdit[0]=new QLineEdit2(lineEditList,"0",xWidget);	orientTextureLineEdit[0]->setValidator(validatorNormal);	xWidget->layout()->addWidget(orientTextureLineEdit[0]);
	orientTextureLineEdit[1]=new QLineEdit2(lineEditList,"1",xWidget);	orientTextureLineEdit[1]->setValidator(validatorNormal);	xWidget->layout()->addWidget(orientTextureLineEdit[1]);
	orientTextureLineEdit[2]=new QLineEdit2(lineEditList,"2",xWidget);	orientTextureLineEdit[2]->setValidator(validatorNormal);	xWidget->layout()->addWidget(orientTextureLineEdit[2]);
	orientTextureLineEdit[0]->setText("0");
	orientTextureLineEdit[1]->setText("0");
	orientTextureLineEdit[2]->setText("1");


	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	randTextureOrientationButton=new QPushButton2(buttonList,"Random Tex3D Orientation",xWidget);
	xWidget->layout()->addWidget(randTextureOrientationButton);

	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	textureAnimButton=new QPushButton2(buttonList,"Tex Anim Test",xWidget);
	xWidget->layout()->addWidget(textureAnimButton);


	
	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
		xLabel=new QLabel("dx:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	dxTexAnimLineEdit=new QLineEdit2(lineEditList,"0.1",xWidget);
	dxTexAnimLineEdit->setValidator(validator);
	xWidget->layout()->addWidget(dxTexAnimLineEdit);
		xLabel=new QLabel("dy:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	dyTexAnimLineEdit=new QLineEdit2(lineEditList,"0.1",xWidget);
	dyTexAnimLineEdit->setValidator(validator);
	xWidget->layout()->addWidget(dyTexAnimLineEdit);
		xLabel=new QLabel("msec:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	msecTexAnimLineEdit=new QLineEdit2(lineEditList,"30",xWidget);
	msecTexAnimLineEdit->setValidator(validatorInt);
	xWidget->layout()->addWidget(msecTexAnimLineEdit);
	

	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	xLabel=new QLabel("Iter:",xWidget);
	xWidget->layout()->addWidget(xLabel);
	iterTextureLineEdit=new QLineEdit2(lineEditList,"1",xWidget);
	iterTextureLineEdit->setValidator(validatorIter);
	xWidget->layout()->addWidget(iterTextureLineEdit);

	xWidget=new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	testTextureSelectComboBox = new QComboBox2(comboBoxList, xWidget );
	xWidget->layout()->addWidget(testTextureSelectComboBox);

	
	testTextureModeComboBox = new QComboBox2(comboBoxList, xWidget );
	xWidget->layout()->addWidget(testTextureModeComboBox);
	testTextureModeComboBox->addItem("Random",0);
	testTextureModeComboBox->addItem("Marble look",1);
	testTextureModeComboBox->addItem("Sth Spherlical",2);
	testTextureModeComboBox->addItem("Angle trace linear",3);
	testTextureModeComboBox->addItem("Angle trace radial",4);
	testTextureModeComboBox->addItem("Dihedral trace test",5);
	testTextureModeComboBox->addItem("tex map to cylinder",6);
	testTextureModeComboBox->addItem("tex map to plane",7);
	testTextureModeComboBox->addItem("tex map to sphere",8);
	testTextureModeComboBox->addItem("triangle map vtk",9);
	testTextureModeComboBox->setCurrentIndex(3);


}
//-----------------------------------------
void GUIPanelWidget::CreateMaskTab(MyDockWidget *qtab)
{



	QWidget *Info = new QWidget(qtab);
	qtab->setScrollWidget(Info);
	QVBoxLayout* MainLayout = new QVBoxLayout(Info);//, 10, -1);
	Info->setLayout(MainLayout);
	MainLayout->setSpacing(1);
	QGroupBox *Gb1 = new	QGroupBox(Info);
	QVBoxLayout *vbl_Gb1 = new QVBoxLayout;
	Gb1->setLayout(vbl_Gb1);
	Gb1->layout()->setSpacing(1);
	Gb1->layout()->setMargin(2);
	MainLayout->addWidget(Gb1);
	Gb1->setTitle("Masks Info");

	QWidget *w;
	QLabel *l;


	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(w);
	riceButton = new QPushButton2(buttonList,"Rice", w);
	w->layout()->addWidget(riceButton);
	ttwButton = new QPushButton2(buttonList,"ttw", w);
	w->layout()->addWidget(ttwButton);
	


	//QSlider *MaskWeightsSlider1, *MaskWeightsSlider2;
	l = new QLabel("use MainMask for:", w);
	Gb1->layout()->addWidget(l);
	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout()); Gb1->layout()->addWidget(w);
	useMaskFor3DViewCheckBox = new QCheckBox2(checkBoxList,Gb1);
	useMaskFor3DViewCheckBox->setText("3DVis");
	w->layout()->addWidget(useMaskFor3DViewCheckBox);
	useMaskForBarrierCheckBox = new QCheckBox2(checkBoxList,Gb1);
	useMaskForBarrierCheckBox->setText("Barrier");
	w->layout()->addWidget(useMaskForBarrierCheckBox);

	useFrustomOnLiveMaskCheckBox = new QCheckBox2(checkBoxList,Gb1);
	useFrustomOnLiveMaskCheckBox->setText("liveFrustum");
	w->layout()->addWidget(useFrustomOnLiveMaskCheckBox);
	
	
	
	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(w);
	l = new QLabel("BlocklMaskOpacity: ", w); w->layout()->addWidget(l);
	BlockMaskOpacitySlider = new QSlider2(sliderList,Qt::Horizontal, w); w->layout()->addWidget(BlockMaskOpacitySlider);
	BlockMaskOpacitySlider->setRange(0, 255);
	BlockMaskOpacitySlider->setValue(255);

	//maskopacity3DWidget = new QWidget(Gb1);
	w =  new QWidget(Gb1);; w->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(w);
	
	l3dMasksOpacityLabel = new QLabel("3DMasksOpacity: ", w); w->layout()->addWidget(l3dMasksOpacityLabel);
	CurrentMask3DOpacitySlider = new QSlider2(sliderList,Qt::Horizontal, w); w->layout()->addWidget(CurrentMask3DOpacitySlider);
	CurrentMask3DOpacitySlider->setRange(0, 255);
	CurrentMask3DOpacitySlider->setValue(255);



	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(w);
	liveMaskOpacityLabel = new QLabel("LiveMaskOpacity: ", w); w->layout()->addWidget(liveMaskOpacityLabel);
	MaskWeightsSlider1 = new QSlider2(sliderList,Qt::Horizontal, w); w->layout()->addWidget(MaskWeightsSlider1);
	MaskWeightsSlider1->setRange(0, 255);
	MaskWeightsSlider1->setValue(255);

	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(w);
	removeCurrentLiveAddFromMainMaskCheckBox = new QCheckBox2(checkBoxList,"RemoveLiveAddFromMainMask", w); w->layout()->addWidget(removeCurrentLiveAddFromMainMaskCheckBox);

	/*l = new QLabel("Weight2: ", w); w->layout()->addWidget(l);
	MaskWeightsSlider2 = new QSlider2(sliderList,Qt::Horizontal, w); w->layout()->addWidget(MaskWeightsSlider2);
	MaskWeightsSlider2->setRange(0, 255);
	MaskWeightsSlider2->setValue(255);
	MaskWeightsSlider2->hide();*/





	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(w);
	CurrentMaskNameLineEdit = new QLineEdit2(lineEditList,"default", w);
	w->layout()->addWidget(CurrentMaskNameLineEdit);
	
	

	
	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(w);
	GenerateMaskStatsButton = new QPushButton2(buttonList," Stats", w);
	w->layout()->addWidget(GenerateMaskStatsButton);


	GenerateSelMultiMaskStatsButton	= new QPushButton2(buttonList, "Multi Stats", w);
	w->layout()->addWidget(GenerateSelMultiMaskStatsButton);
	l = new QLabel("Source:", w);
	w->layout()->addWidget(l);

	//QComboBox *m2ComboBox;

	maskStatsSourceComboBox = new QComboBox2(comboBoxList,w);
	w->layout()->addWidget(maskStatsSourceComboBox);
	maskStatsSourceComboBox->addItem("Live");
	maskStatsSourceComboBox->addItem("Main");
	maskStatsSourceComboBox->addItem("Selected");
	maskStatsSourceComboBox->setCurrentIndex(0);






	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(w);



	
	fitMinEllipsoidButton = new QPushButton2(buttonList,"Fit MinVol Ellipsoid", w);
	w->layout()->addWidget(fitMinEllipsoidButton);



	
	Gb1->layout()->addWidget(w);
	AddCurrentMaskForTreeButton = new QPushButton2(buttonList,"Add Mask", w);
	w->layout()->addWidget(AddCurrentMaskForTreeButton);

	m2ComboBox = new QComboBox2(comboBoxList,w);
	w->layout()->addWidget(m2ComboBox);
	m2ComboBox->addItem("NotM2");
	m2ComboBox->addItem("AddToM2");
	m2ComboBox->addItem("OnlyM2");
	m2ComboBox->setCurrentIndex(1);
	Gb1->layout()->addWidget(w);


	
	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());
	AddLiveMaskForTreeButton = new QPushButton2(buttonList,"Add Live", w);
	w->layout()->addWidget(AddLiveMaskForTreeButton);
	AddLiveMutliMaskForTreeButton = new QPushButton2(buttonList, "Add Live All", w);

	w->layout()->addWidget(AddLiveMutliMaskForTreeButton);
	Gb1->layout()->addWidget(w);


	//QComboBox *m2ComboBox;

	
	/*AddCurrentAddToMask2CheckBox = new QCheckBox2(checkBoxList,"M2", w);
	w->layout()->addWidget(AddCurrentAddToMask2CheckBox);
	AddCurrentAddToMask2CheckBox->setChecked(true);*/
	UseMasksForBarrierButton = new QPushButton2(buttonList,"Use Masks For MainMask", Gb1);
	Gb1->layout()->addWidget(UseMasksForBarrierButton);

	ShowMasksFromTreeButton = new QPushButton2(buttonList,"Show Masks from Tree Widget", Gb1);
	Gb1->layout()->addWidget(ShowMasksFromTreeButton);

	GenerateSurfacesFromMasksButton = new QPushButton2(buttonList,"GenerateSurfeces", Gb1);
	Gb1->layout()->addWidget(GenerateSurfacesFromMasksButton);

	GenerateGlyphsFromCurrentVol2PushButton = new QPushButton2(buttonList,"GenerateGlyphs", Gb1);
	Gb1->layout()->addWidget(GenerateGlyphsFromCurrentVol2PushButton);

	QWidget *ws = new QWidget(Gb1);
	Gb1->layout()->addWidget(ws);
	ws->setLayout(new QHBoxLayout());
	ws->layout()->setSpacing(1);
	ws->layout()->setMargin(1);
	QLabel *lla = new QLabel("Set Color:", ws);
	ws->layout()->addWidget(lla);
	ChangeVolMaskColorComboBox = new QComboBox2(comboBoxList,Gb1);
	ws->layout()->addWidget(ChangeVolMaskColorComboBox);

	int sx = 256;
	int sy = 12;
	int sxa = 150;
	QImage Image(sx, sy, QImage::Format_Indexed8);
	//QPixmap Pixmap;
	for (int i = 0; i < 256; i++)
	{
		double col[3];
		QColor cc;
		cc = distinctColors[i + 1];
		col[0] = cc.red() ;
		col[1] = cc.green() ;
		col[2] = cc.blue();
		if (i == 0)
		{
		col[0] = 0;
		col[1] = 0;
		col[2] = 0;
		}
		Image.setColor(i, qRgb(col[0], col[1], col[2]));
		
	}

		
	for (int k = 0; k<256; k++)
	{
	

		

		for (int i = 0; i<sx; i++)
			for (int j = 0; j < sy; j++)
				*(Image.scanLine(j) + i) = (unsigned char)(k);// *(255.0 / 100.0));


		QImage tmpa = Image.convertToFormat(QImage::Format_RGB32);

		QImage smaller = tmpa.scaled(sxa, 12);

		//Pixmap.fromImage( tmpa);
		QPixmap pp(sxa, sy);
		QPainter paint(&pp);
		paint.drawImage(0, 0, smaller);

		/*if (k==0)
		{
		QString aaa= qApp->applicationDirPath()+"/tttt1.jpg";
		pp.save(aaa);
		}*/
		QString 	tmp = "";
		QString aa;
		aa.setNum(k);
		tmp.append(aa);
		ChangeVolMaskColorComboBox->addItem(QIcon(pp), QString::number(k), Qt::ToolTipRole);
		//PaletteComboR->addItem(QIcon(pp), QString::number(k), Qt::ToolTipRole);



	}
	ChangeVolMaskColorComboBox->setIconSize(QSize(sxa, sy));

	
	ClearVolMaskButton = new QPushButton2(buttonList,"Clear Masks From 3D", Gb1);
	Gb1->layout()->addWidget(ClearVolMaskButton);

	DeleteVolMaskButton = new QPushButton2(buttonList,"Delete Masks", Gb1);
	Gb1->layout()->addWidget(DeleteVolMaskButton);
	
	

	QStringList ll;

	//ll.append("");
	//ll.append("default");
	masksTreeWidget = new QTreeWidget(Gb1);
	//masksTreeWidget->insertTopLevelItem(0, new QTreeWidgetItem(ll));//QStringList("default")));
	masksTreeWidget->setColumnCount(3);
	masksTreeWidget->header()->resizeSection(0, 155);
	masksTreeWidget->header()->resizeSection(1, 14);
	masksTreeWidget->header()->resizeSection(2, 20);
	masksTreeWidget->show();
	Gb1->layout()->addWidget(masksTreeWidget);
	masksTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

	QWidget *LoadSaveTransGridBox = new QWidget(Gb1); LoadSaveTransGridBox->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(LoadSaveTransGridBox);
	LoadMaskForTreeButton = new QPushButton2(buttonList,"Load ", LoadSaveTransGridBox); LoadSaveTransGridBox->layout()->addWidget(LoadMaskForTreeButton);
	SaveMaskForTreeButton = new QPushButton2(buttonList,"Save ", LoadSaveTransGridBox); LoadSaveTransGridBox->layout()->addWidget(SaveMaskForTreeButton);

	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w);
	CropAirSpacesPushButton= new QPushButton2(buttonList,"AirSpaces ", w); w->layout()->addWidget(CropAirSpacesPushButton);
	

	GetMiddleVolumeBetweenTwoMasksPushButton = new QPushButton2(buttonList,"GetMiddleMask", w); Gb1->layout()->addWidget(GetMiddleVolumeBetweenTwoMasksPushButton);



	GenerateTestTubesButton = new QPushButton2(buttonList, "GenTestTubes", w); Gb1->layout()->addWidget(GenerateTestTubesButton);
	QString txts[] = { "AutoAir-Right", "AutoBones-Left", "AutoCrop","tmp" };
	for (int i = 0; i < 4; i++)
	{
		w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w);
		AirspacesOptionsCheckBox[i] = new QCheckBox2(checkBoxList,Gb1);
		
		AirspacesOptionsCheckBox[i]->setText(txts[i]);
	
		AirspacesOptionsButton[i] = new QPushButton2(buttonList,"Get", Gb1);
		AirspacesOptionsButton[i]->setFixedWidth(20);
		w->layout()->addWidget(AirspacesOptionsButton[i]);
		AirspacesLineEdit[i] = new QLineEdit2(lineEditList,Gb1);
		w->layout()->addWidget(AirspacesLineEdit[i]);
		AirspacesLineEdit[i]->setFixedWidth(100);
		w->layout()->addWidget(AirspacesOptionsCheckBox[i]);
	}
	//	AutoThresholdForAirspacesCheckBox = new QCheckBox2(checkBoxList,Gb1);
	//	AutoThresholdForAirspacesCheckBox->setChecked(true);
	//	AutoThresholdForAirspacesCheckBox->setText("Th-Auto/Right");
	//w->layout()->addWidget(AutoThresholdForAirspacesCheckBox);





//	SaveMaskForTreeButton = new QPushButton2(buttonList,"Save ", LoadSaveTransGridBox); LoadSaveTransGridBox->layout()->addWidget(SaveMaskForTreeButton);

}
//-----------------------------------------
void GUIPanelWidget::CreateSurfaceTab(MyDockWidget *qtab)
{
	TextFieldDoubleValidator *validator = new TextFieldDoubleValidator(-1000000, 1000000.0, 4, this);
	validator->setNotation(TextFieldDoubleValidator::StandardNotation);
	TextFieldDoubleValidator *validatorDeg = new TextFieldDoubleValidator(-180, 180, 4, this);
	validatorDeg->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *validatorInt = new QIntValidator(0, 10000000, this);
	TextFieldDoubleValidator *validatorNormal = new TextFieldDoubleValidator(-1, 1, 4, this);
	validatorNormal->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *validatorIter = new QIntValidator(1, 100, this);
	QWidget *xWidget;
	QLabel *xLabel;
	QGroupBox *gBox;



	QWidget *Info = new QWidget(qtab);



	qtab->setScrollWidget(Info);
	QVBoxLayout* MainLayout= new QVBoxLayout( Info);//, 10, -1);
	Info->setLayout(MainLayout);
	MainLayout->setSpacing(1);
	QGroupBox *Gb1=new	QGroupBox(Info);
	QVBoxLayout *vbl_Gb1 = new QVBoxLayout;
	Gb1->setLayout(vbl_Gb1);	
	Gb1->layout()->setSpacing(1);
	Gb1->layout()->setMargin(2);
	MainLayout->addWidget(Gb1);
	Gb1->setTitle("Surface Datasets Info");


	//	QWidget *DatasetNameBox	=	new QWidget(Gb1);
	QWidget *DatasetNameBox=new	QWidget(Gb1);
	QHBoxLayout *hbl_DatasetNameBox = new QHBoxLayout;
	hbl_DatasetNameBox->layout()->setSpacing(1);
	hbl_DatasetNameBox->layout()->setMargin(2);
	DatasetNameBox->setLayout(hbl_DatasetNameBox);
	
	vbl_Gb1->addWidget(DatasetNameBox);
	QLabel *L001=new QLabel("Name: ",DatasetNameBox);
	LabelSurfaceFileName=new QLabel(DatasetNameBox);
	LabelSurfaceFileName->setMaximumWidth(120);
	hbl_DatasetNameBox->addWidget(L001);
	hbl_DatasetNameBox->addWidget(LabelSurfaceFileName);

	QLabel *L1=new QLabel("Volume Sizes :",Gb1);	
	vbl_Gb1->addWidget(L1);

	QWidget *LabelNBox=new	QWidget(Gb1);
	QHBoxLayout *hbl_LabelNBox = new QHBoxLayout;
	LabelNBox->setLayout(hbl_LabelNBox);
	LabelNBox->layout()->setSpacing(1);
	LabelNBox->layout()->setMargin(2);
	
	vbl_Gb1->addWidget(LabelNBox);
	
	LabelSurfaceNx=new QLabel("Nx: ",LabelNBox);
	LabelSurfaceNy=new QLabel("Ny: ",LabelNBox);
	LabelSurfaceNz=new QLabel("Nz: ",LabelNBox);
	QLabel *L2=new QLabel("Voxel  Sizes :",Gb1);
	vbl_Gb1->addWidget(L2);
	hbl_LabelNBox->addWidget(LabelSurfaceNx);
	hbl_LabelNBox->addWidget(LabelSurfaceNy);
	hbl_LabelNBox->addWidget(LabelSurfaceNz);

	//	QWidget *LabelSBox	=	new QWidget(Gb1);
	QWidget *LabelSBox=new	QWidget(Gb1);
	QHBoxLayout *hbl_LabelSBox = new QHBoxLayout;
	LabelSBox->setLayout(hbl_LabelSBox);
		LabelSBox->layout()->setSpacing(1);
	LabelSBox->layout()->setMargin(2);

	vbl_Gb1->addWidget(LabelSBox);
	LabelSurfaceXscale=new QLabel("Sx: ",LabelSBox);
	LabelSurfaceYscale=new QLabel("Sy: ",LabelSBox);
	LabelSurfaceZscale=new QLabel("Sz: ",LabelSBox);

	hbl_LabelSBox->addWidget(LabelSurfaceXscale);
	hbl_LabelSBox->addWidget(LabelSurfaceYscale);
	hbl_LabelSBox->addWidget(LabelSurfaceZscale);
	///////////////////////////////////////////////
	//Q3GroupBox *Gb2X=new Q3GroupBox(5,Vertical,Gb1);

	sufreceInfoTabGroupBox=new	QGroupBox(Gb1);
	QGroupBox* Gb2X = sufreceInfoTabGroupBox;//new	QGroupBox(Gb1);
	QVBoxLayout *vbl_Gb2X = new QVBoxLayout;
	Gb2X->setLayout(vbl_Gb2X);	
		Gb2X->layout()->setSpacing(1);
	Gb2X->layout()->setMargin(2);

	vbl_Gb1->addWidget(Gb2X);
	//QWidget *PropBox1	=	new QWidget(Gb2X);
	
	
	
	
	QWidget *PropBox1=new	QWidget(Gb2X);
	QHBoxLayout *hbl_PropBox1 = new QHBoxLayout;
	PropBox1->setLayout(hbl_PropBox1);
	PropBox1->layout()->setSpacing(1);
	PropBox1->layout()->setMargin(2);
	LabelSurfaceNumberOfPoints2=new QLabel("PointsNr: ",PropBox1);
	LabelSurfaceNumberOfPolys2=new QLabel("PolysNr: ",PropBox1);
	hbl_PropBox1->addWidget(LabelSurfaceNumberOfPoints2);
	hbl_PropBox1->addWidget(LabelSurfaceNumberOfPolys2);

	OpenSurfacePropertyWidgetButton2=new QPushButton2(buttonList,"Property...",Gb2X);


	CombineAllSurfacesButton=new QPushButton2(buttonList,"Combine Surfaces",Gb2X);


	//------------------
	visNormalsWidget=new QWidget(Gb1);
	xWidget = visNormalsWidget;
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);

	WhichSurfaceIsCurrentButton=new QPushButton2(buttonList,"VisCurrent",xWidget);
	xWidget->layout()->addWidget(WhichSurfaceIsCurrentButton);

	showNormalsButton=new QPushButton2(buttonList,"Vis Normals",xWidget);
	xWidget->layout()->addWidget(showNormalsButton);
	xLabel=new QLabel("Scale N",xWidget);
	xWidget->layout()->addWidget(xLabel);
	scaleNormalsLineEdit=new QLineEdit2(lineEditList,"1.0",xWidget);
	//scaleNormalsLineEdit->setValidator(validator);
	xWidget->layout()->addWidget(scaleNormalsLineEdit);






	/////////////////////////////////////////////////
	vbl_Gb2X->addWidget(PropBox1);
	vbl_Gb2X->addWidget(OpenSurfacePropertyWidgetButton2);
	vbl_Gb2X->addWidget(CombineAllSurfacesButton);
	vbl_Gb2X->addWidget(WhichSurfaceIsCurrentButton);

	QLabel *L01=new QLabel(Gb1);
	vbl_Gb1->addWidget(L01);
	QFont font=QFont();
	font.setBold(true);

	L1->setFont(font);
	L001->setFont(font);
	L2->setFont(font);

	

		//------------------
	xWidget=new QWidget(Gb2X);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb2X->layout()->addWidget(xWidget);

	setSurfaceGroupButton=new QPushButton2(buttonList,"Set Group ...",xWidget);
	xWidget->layout()->addWidget(setSurfaceGroupButton);
	saveAllSelectedSurfacesButton = new QPushButton2(buttonList, "Save Selected Surfaces ...", xWidget);
	xWidget->layout()->addWidget(saveAllSelectedSurfacesButton);


	//QLabel *L002=new QLabel("Datasets: ",Gb1);
	//	vbl_Gb1->addWidget(L002);
//	SurfaceDataListView =new QListView(0);
	//	vbl_Gb1->addWidget(SurfaceDataListView);

	surfaceTreeWidget=new QTreeWidget(Gb2X);
	connect(surfaceTreeWidget,SIGNAL(itemActivated( QTreeWidgetItem * , int  )),this,SLOT(SlotUpdateTreeWidgetState( QTreeWidgetItem * , int  )));
//	connect(surfaceTreeWidget, SIGNAL( itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT (		SlotSurfaceTreeWidgetDoubleClicked( QTreeWidgetItem * , int  )	));
	Gb2X->layout()->addWidget(surfaceTreeWidget);
	surfaceTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

	QStringList ll;

	//ll.append("");
	ll.append("default");
	surfaceTreeWidget->insertTopLevelItem(0,new QTreeWidgetItem(ll));//QStringList("default")));
	surfaceTreeWidget->setColumnCount(3);
	surfaceTreeWidget->header()->resizeSection(0,155);
	surfaceTreeWidget->header()->resizeSection(1,14);
	surfaceTreeWidget->header()->resizeSection(2,20);
	//surfaceTreeWidget->setColumnWidth(1,2);
	 //QStringList columns;
	/// columns << "Name" << "C" << "M" << "O" << "V";
    //QTreeWidgetItem* item=new QTreeWidgetItem(surfaceTreeWidget,columns);
	// surfaceTreeWidget->addTopLevelItem(item);
	surfaceTreeWidget->show();


//	SurfaceDataListView->addColumn("Name",SurfaceDataListView->width()-14);

	/*SurfaceDataListView->addColumn("C",14);
	SurfaceDataListView->addColumn("M",14);
	SurfaceDataListView->addColumn("O",14);
	SurfaceDataListView->addColumn("V",14);


	SurfaceDataListView->setSelectionMode(QListView::Extended );
	SurfaceDataListView->setColumnWidthMode ( 1, QListView::Maximum ) ;

	SurfaceDataListView->setAllColumnsShowFocus ( true );
	SurfaceDataListView->setSorting(-1);
*/
	surfaceTreeWidget->setSortingEnabled(false);
	QWidget *SurfaceDataSetBox	=	new QWidget(Gb2X);
	SurfaceDataSetBox->setLayout(new QHBoxLayout());
	Gb2X->layout()->addWidget(SurfaceDataSetBox);
	CollapseAllSurfaceItemsButton = new QPushButton2(buttonList, "CollapseAll", SurfaceDataSetBox);
	InsertSurfaceItemButton = new QPushButton2(buttonList, "Insert ...", SurfaceDataSetBox);

	DeleteSurfaceItemButton=new QPushButton2(buttonList,"Delete",SurfaceDataSetBox);
	


	SurfaceDataSetBox->layout()->addWidget(CollapseAllSurfaceItemsButton);
	SurfaceDataSetBox->layout()->addWidget(InsertSurfaceItemButton);
	SurfaceDataSetBox->layout()->addWidget(DeleteSurfaceItemButton);

	SurfaceDataSetBox	=	new QWidget(Gb2X);
	Gb2X->layout()->addWidget(SurfaceDataSetBox);
	QLabel *L002=new QLabel("Opacity: ",SurfaceDataSetBox);
	surfaceOpacitySlider	= new QSlider2(sliderList,Qt::Horizontal, SurfaceDataSetBox);
	surfaceOpacitySlider->setRange(0,255);
	surfaceOpacitySlider->setValue(255);


	setCurrentPropertyBox=new QCheckBox2(checkBoxList,SurfaceDataSetBox);
	setCurrentPropertyBox->setText("Show");
	setCurrentPropertyBox->setChecked(false);
	setCurrentPropertyBox->hide();

	SurfaceDataSetBox->setLayout(new QHBoxLayout());
	SurfaceDataSetBox->layout()->addWidget(L002);
	SurfaceDataSetBox->layout()->addWidget(surfaceOpacitySlider);
	SurfaceDataSetBox->layout()->addWidget(setCurrentPropertyBox);


	//connect(DeleteSurfaceItemButton,SIGNAL(clicked()),this,SLOT(SlotDeletePolygonSurfaceDataItem()));

//	connect(SurfaceDataListView,SIGNAL(currentChanged ( QListViewItem * )),this,SLOT(SlotCurrentPolygonSurfaceDataItemChanged( QListViewItem *)));

	Helper2=false;


	// double click

//	connect(SurfaceDataListView,SIGNAL(	selectionChanged ()),this,SLOT(SlotSurfaceListSelectionChanged( )));

	
	transSurfacesWidget = new QWidget(Gb2X);
	Gb2X->layout()->addWidget(transSurfacesWidget);
	QWidget *Gb3	= transSurfacesWidget;
	//	Gb3->setTitle("Transformation Operatins");
	QLabel *TransL1=new QLabel(" Trans:",Gb3);
	Gb3->setLayout(new QHBoxLayout());
	ApplyActorTransformationButton = new QPushButton2(buttonList, "Apply ",Gb3);
	ResetActorTransformationButton = new QPushButton2(buttonList, "Reset ",Gb3);
	ApplyActorTransformationButton->setMinimumWidth(40);
	ApplyActorTransformationButton->setMinimumWidth(40);

	Gb3->layout()->addWidget(TransL1);
	Gb3->layout()->addWidget(ResetActorTransformationButton);
	Gb3->layout()->addWidget(ApplyActorTransformationButton);


	
		QGroupBox* Gb3a = new QGroupBox(Info);//300,Vertical,Info);
		Gb3a->setLayout(new QVBoxLayout);
		Gb3a->setTitle("Surface Tools");

		QWidget *Box22 = new QWidget(Gb3); Box22->setLayout(new QHBoxLayout);
		Gb3a->layout()->addWidget(Box22);

		SetRandomColorsToSelectedSurfacesButton = new QPushButton2(buttonList, "Set Random Color", Box22); Box22->layout()->addWidget(SetRandomColorsToSelectedSurfacesButton);
		SetRandomColorsToSelectedSurfacesButton->setMinimumWidth(40);
		Box22 = new QWidget(Gb3); Box22->setLayout(new QHBoxLayout);
		Gb3a->layout()->addWidget(Box22);
		//QPushButton *GenerateSphericalPointsCutsBuutton;
		GenerateSphericalPointsCutsBuutton = new QPushButton2(buttonList, "Generate Spherical Points Cuts", Box22); Box22->layout()->addWidget(GenerateSphericalPointsCutsBuutton);
		GenerateSphericalPointsCutsBuutton->setMinimumWidth(40);
	


	MainLayout->addWidget(Gb1);
	MainLayout->addWidget(Gb3a);
	//qtab->addTab(Info,"&Data");

	/*

	ItemAttributesMenu=new QPopupMenu(RawDataListView);

	connect(ItemAttributesMenu,SIGNAL(activated ( int )),this,SLOT(SlotChangeAtributes(int)));
	connect(RawDataListView, SIGNAL( rightButtonPressed( QListViewItem *, const QPoint& , int ) ),
	this, SLOT( SlotListRightMouseButtonRMB( QListViewItem *, const QPoint &, int ) ) );



	ModelItem=NULL;
	ObjectItem=NULL;
	View3DItem=NULL;
	InspectionItem=NULL;
	View2DItems=NULL;
	*/
}

//--------------------------------------------------------

void GUIPanelWidget:: SlotInsertPolygonSurfaceDataItem(const QString &name)
{

	//QListViewItem *Item=new QListViewItem( SurfaceDataListView,SurfaceDataListView->currentIndex(),name,"","+","+","+" );

	//SlotSetSurfaceColorPresentation(Item, 0,0,0);
	//SurfaceDataListView->setCurrentIndex(SurfaceDataListView->currentIndex()->nextSibling());

	//SurfaceDataListView->clearSelection();	
	//SurfaceDataListView->setSelected(SurfaceDataListView->currentIndex(),true); 

	//

	//CurrentPolygonSurfaceDataItem=Item;
	////dodatek
	//SlotCurrentPolygonSurfaceDataItemChanged(CurrentPolygonSurfaceDataItem);
}
//---------------------------------------------------
void GUIPanelWidget:: SlotInsertPolygonSurfaceDataItemAtTheEnd(const QString &name)
{
	////SurfaceDataListView->setCurrentIndex(SurfaceDataListView->lastItem());
	//QListViewItem *Item=new QListViewItem( SurfaceDataListView,SurfaceDataListView->lastItem(),name,"","+","+","+" );

	//SlotSetSurfaceColorPresentation(Item, 0,0,0);
	//SurfaceDataListView->setCurrentIndex(SurfaceDataListView->lastItem());

	//SurfaceDataListView->clearSelection();	
	//SurfaceDataListView->setSelected(SurfaceDataListView->currentIndex(),true); 

	//

	//CurrentPolygonSurfaceDataItem=Item;
	////dodatek
	//SlotCurrentPolygonSurfaceDataItemChanged(CurrentPolygonSurfaceDataItem);
}
//--------------------------------------------------------

void GUIPanelWidget:: SlotInsertPolygonSurfaceDataItemAndBack(const QString &name)
{
	//QListViewItem *Item=new QListViewItem( SurfaceDataListView,SurfaceDataListView->currentIndex(),name,"","+","+","+" );
	//SlotSetSurfaceColorPresentation(Item, 0,0,0);
	//// o jeden w gore jak sie da
	//QListViewItem *StepItem=SurfaceDataListView->firstChild();

	//if((StepItem!=Item)&&(LastSurfaceItemDeleted==false))
	//{
	//	while(Item!=StepItem->nextSibling())
	//	{
	//		StepItem=StepItem->nextSibling();
	//	}

	//	if ((StepItem!=NULL))
	//	{
	//		StepItem->moveItem(Item);

	//	}
	//}
	////////////////////////////////////////////	
	//SurfaceDataListView->setCurrentIndex(Item);
	//SurfaceDataListView->clearSelection();	
	//SurfaceDataListView->setSelected(SurfaceDataListView->currentIndex(),true); 
	//CurrentPolygonSurfaceDataItem=Item;
	////dodatek
	//SlotCurrentPolygonSurfaceDataItemChanged(CurrentPolygonSurfaceDataItem);

}

//----------------------------------

void GUIPanelWidget::SlotBlockUpdateSelectionList(bool val)
{
	UpdateListBlocked=val;
}
//--------------------------------------------------------


void GUIPanelWidget::SlotShowPolygonSurfaceDatasetInfo(int Nx,int Ny,int Nz,double Xscale,double Yscale,double Zscale,const QString &name,int Nr1,int Nr2,double Or1,double Or2,double Or3)
{
	QString Ntemp,Txt,tt;

	LabelSurfaceFileName->setText(name);

	Ntemp.setNum(Nx);
	Txt="Nx: ";
	Txt.append( Ntemp);
	LabelSurfaceNx->setText(Txt);


	Ntemp.setNum(Ny);
	Txt="Ny: ";
	Txt.append( Ntemp);
	LabelSurfaceNy->setText(Txt);

	Ntemp.setNum(Nz);
	Txt="Nz: ";
	Txt.append( Ntemp);
	LabelSurfaceNz->setText(Txt);


	Ntemp.setNum(Xscale);
	Txt="Xscale: ";
	Txt.append( Ntemp);
	Txt.truncate(10);
	//	Txt.append( " mm");
	LabelSurfaceXscale->setText(Txt);


	Ntemp.setNum(Yscale);
	Txt="Yscale: ";
	Txt.append( Ntemp);
	Txt.truncate(10);
	//	Txt.append( " mm");
	LabelSurfaceYscale->setText(Txt);


	Ntemp.setNum(Zscale);
	Txt="Zscale: ";
	Txt.append( Ntemp);
	Txt.truncate(10);
	//	Txt.append( " mm");
	LabelSurfaceZscale->setText(Txt);



	Ntemp.setNum(Nr1);
	Txt="PointsNr: ";
	Txt.append( Ntemp);
	LabelSurfaceNumberOfPoints->setText(Txt);
	LabelSurfaceNumberOfPoints2->setText(Txt);

	Ntemp.setNum(Nr2);
	Txt="PolysNr: ";
	Txt.append( Ntemp);
	LabelSurfaceNumberOfPolys->setText(Txt);
	LabelSurfaceNumberOfPolys2->setText(Txt);


	SurfaceOriginXEdit->setText(QString::number(Or1));
	SurfaceOriginYEdit->setText(QString::number(Or2));
	SurfaceOriginZEdit->setText(QString::number(Or3));

}

void	GUIPanelWidget::SlotSetSurfaceOrigin(double *Origin)
{
	SurfaceOriginXEdit->setText(QString::number(Origin[0]));
	SurfaceOriginYEdit->setText(QString::number(Origin[1]));
	SurfaceOriginZEdit->setText(QString::number(Origin[2]));

}
/*

void	SlotPolygonSurfaceDataItem();
void	SlotCurrentPolygonSurfaceDataItemChanged(QListViewItem * );
void	SlotSetNewPolygonSurfaceDataSet( );
void	SlotSetNewPolygonSurfaceDataSetByDoubleClick(QListViewItem *Item );

CurrentPolygonSurfaceDataItem
ActivePolygonSurfaceItem

Helper2

SignalSetCurrentPolygonSurfaceDataSet
SignalSetNewPolygonSurfaceDataSet
*/
//---------------------------------------------------------------



void GUIPanelWidget:: SlotDeletePolygonSurfaceDataItem()
{
//	Helper2=true;
//	emit SignalDeletePolygonSurfaceDataSet();

//	CurrentPolygonSurfaceDataItem=SurfaceDataListView->currentIndex();
//	if (CurrentPolygonSurfaceDataItem==SurfaceDataListView->lastItem())
//		LastSurfaceItemDeleted=true;
//	else
//		LastSurfaceItemDeleted=false;
//
//	SurfaceDataListView->removeItem( CurrentPolygonSurfaceDataItem);
//	SurfaceDataListView->setSelected(SurfaceDataListView->currentIndex(),true); 
////	Helper2=false;
//
//	//nowe
//	if(SurfaceDataListView->currentIndex()!=NULL)
//	{
//		CurrentPolygonSurfaceDataItem=SurfaceDataListView->currentIndex();
////		SlotCurrentPolygonSurfaceDataItemChanged(CurrentPolygonSurfaceDataItem);
//	}
//

}

//---------------------------------------------------------------

void GUIPanelWidget:: SlotChangeCurrentPolygonSurfaceDataItem(int pos)
{

	//if((SurfaceDataListView->currentIndex()!=NULL)&&(Helper2==false))
	//{
	//	QListViewItem *Item=SurfaceDataListView->currentIndex();
	//	QListViewItem *StepItem=SurfaceDataListView-> firstChild();

	//	int pos2=0;

	//	while(pos2!=pos)
	//	{
	//		StepItem=StepItem->nextSibling();
	//		pos2++;
	//	}

	//	//ustaw na current 

	//	SurfaceDataListView->setCurrentIndex(StepItem);
	//	SurfaceDataListView->ensureItemVisible(StepItem);
	//	 
	//	StepItem->setSelected(true);


	//	//emit SignalSetCurrentPolygonSurfaceDataSet(pos);
	//}

}
//-----------------------------------
void GUIPanelWidget:: SlotChangeCurrentPolygonSurfaceDataItem(int pos,bool sel)
{

	//if((SurfaceDataListView->currentIndex()!=NULL)&&(Helper2==false))
	//{
	//	QListViewItem *Item=SurfaceDataListView->currentIndex();
	//	QListViewItem *StepItem=SurfaceDataListView-> firstChild();

	//	int pos2=0;

	//	while(pos2!=pos)
	//	{
	//		StepItem=StepItem->nextSibling();
	//		pos2++;
	//	}

	//	//ustaw na current 

	//	SurfaceDataListView->setCurrentIndex(StepItem);
	//	SurfaceDataListView->ensureItemVisible(StepItem);
	//	 
	//	StepItem->setSelected(sel);


	//	//emit SignalSetCurrentPolygonSurfaceDataSet(pos);
	//}

}
//---------------------------------------------------------------



/*

void GUIPanelWidget::SlotListPolygonSurfaceRightMouseButtonRMB( QListViewItem* Item, const QPoint & point, int )
{
int i;
bool Ena;

if( Item )
{
(Item==ModelItem)?ItemAttributesMenu->setItemChecked(0,true):ItemAttributesMenu->setItemChecked(0,false);

Ena=(Item!=ModelItem)?true:false;

ItemAttributesMenu->setItemEnabled(1,Ena);

Ena=(Item->text(1)=="C")?true:false;
ItemAttributesMenu->setItemEnabled(3,Ena);	
for(i=0;i<NumberOfCompatibleItems;i++)
ItemAttributesMenu->setItemEnabled(4+i,Ena);

(Item==ObjectItem)?ItemAttributesMenu->setItemChecked(1,true):ItemAttributesMenu->setItemChecked(1,false);
(Item==View3DItem)?ItemAttributesMenu->setItemChecked(2,true):ItemAttributesMenu->setItemChecked(2,false);
(Item==InspectionItem)?ItemAttributesMenu->setItemChecked(3,true):ItemAttributesMenu->setItemChecked(3,false);

for(i=0;i<NumberOfCompatibleItems;i++)
(Item==(View2DItems[i].Item))?ItemAttributesMenu->setItemChecked(4+i,true):ItemAttributesMenu->setItemChecked(4+i,false);

ItemAttributesMenu->popup( point );
ItemAttributesMenu->show();
}

}


//---------------------------------------------------------------
*/

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//------------ MESH OPERATIONS  ---------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//--------------------------------------------------------------- 

//--------------------------------------------------------------- 

void GUIPanelWidget::CreateSurfaceProcessingTab(MyDockWidget *qtab)
{

	TextFieldDoubleValidator *Validator =new TextFieldDoubleValidator(0.0,1.0,15,this);
	Validator->setNotation(TextFieldDoubleValidator::StandardNotation);
	TextFieldDoubleValidator *Validator1 =new TextFieldDoubleValidator(-9999.0,		9999.0, 4,this);
	Validator1->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *Validator2 =new QIntValidator(1,10000,this);


	QWidget *Info = new QWidget(qtab);
	qtab->setScrollWidget(Info);
	QLayout* MainLayout = new QVBoxLayout();
	Info->setLayout(MainLayout);

	QGroupBox *Gb1=new QGroupBox(Info);//20,Vertical,Info);
	Gb1->setLayout(new QVBoxLayout);
	Gb1->setTitle("Surface Cutting");

	QWidget *CuttBox1	=	new QWidget(Gb1);
	CuttBox1->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(CuttBox1);

	QLabel *L1=new QLabel("Cutter:",CuttBox1);	
	CuttBox1->layout()->addWidget(L1);
	CutterCombo = new QComboBox2(comboBoxList, CuttBox1);
	CuttBox1->layout()->addWidget(CutterCombo);
	CutterCombo->addItem("Plane");
	CutterCombo->addItem("Sphere");
	//	CutterCombo->addAction("Circle Saw",2);


	ShowHideCutterButton = new QPushButton2(buttonList,"Show/Hide", CuttBox1); CuttBox1->layout()->addWidget(ShowHideCutterButton);
	
	ShowHideCutterButton->setMinimumWidth(30);

	QWidget *CuttBox2 = new QWidget(Gb1); CuttBox2->setLayout(new QHBoxLayout());
	

	Gb1->layout()->addWidget(CuttBox2);
	QLabel *L1R = new QLabel("Size:", CuttBox2);	CuttBox2->layout()->addWidget(L1R);
	
	CutterSizeSlider = new QSlider2(sliderList,Qt::Horizontal, CuttBox2);	CuttBox2->layout()->addWidget(CutterSizeSlider);

	CutterSizeSlider->setRange(1,100);
	CutterSizeSlider->setValue(50);

	QWidget *PropBox2x = new QWidget(Gb1); PropBox2x->setLayout(new QHBoxLayout());
	

	Gb1->layout()->addWidget(PropBox2x);
	CuttSurfaceButton = new QPushButton2(buttonList,"Cut Surface", PropBox2x); PropBox2x->layout()->addWidget(CuttSurfaceButton);
	


	PropBox2x	=	new QWidget(Gb1);
	PropBox2x->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(PropBox2x);
	SegmentCuttButton = new QPushButton2(buttonList,"SegCut", PropBox2x); SegmentCuttButton->setMinimumWidth(50); PropBox2x->layout()->addWidget(SegmentCuttButton);
	UndoSegCuttButton = new QPushButton2(buttonList,"Undo", PropBox2x); UndoSegCuttButton->setMinimumWidth(50); PropBox2x->layout()->addWidget(UndoSegCuttButton);
	RedoSegCuttButton = new QPushButton2(buttonList,"Redo", PropBox2x); RedoSegCuttButton->setMinimumWidth(50); PropBox2x->layout()->addWidget(RedoSegCuttButton);

	//UndoSegCuttButton->setAccel( Qt::CTRL + Qt::Key_Z);
	//RedoSegCuttButton->setAccel( Qt::CTRL + Qt::Key_Y);



	CreateBoundaryDataButton=new QPushButton2(buttonList,"Create Boundary Data",Gb1);
	Gb1->layout()->addWidget(CreateBoundaryDataButton);
	///////////////////////////////////////////////
	QGroupBox *PropBox1 = new QGroupBox(Gb1); PropBox1->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(PropBox1);
	QWidget *PropBox2 = new QWidget(PropBox1); PropBox2->setLayout(new QHBoxLayout());

	Gb1->layout()->addWidget(PropBox2);
	LabelSurfaceNumberOfPoints = new QLabel("PointsNr: ", PropBox2); PropBox2->layout()->addWidget(LabelSurfaceNumberOfPoints);
	LabelSurfaceNumberOfPolys = new QLabel("PolysNr: ", PropBox2); PropBox2->layout()->addWidget(LabelSurfaceNumberOfPolys);
	OpenSurfacePropertyWidgetButton = new QPushButton2(buttonList,"Property...", PropBox1); PropBox1->layout()->addWidget(OpenSurfacePropertyWidgetButton);
	Gb1->layout()->addWidget(OpenSurfacePropertyWidgetButton);
	SmoothAndDecimSurfcaceButton = new QPushButton2(buttonList,"Smooth And Decim", PropBox1); PropBox1->layout()->addWidget(SmoothAndDecimSurfcaceButton);
	Gb1->layout()->addWidget(SmoothAndDecimSurfcaceButton);

//---------------------
	QGroupBox *Gb11=new QGroupBox(Info);//4,Vertical,Info);
	Gb11->setLayout(new QVBoxLayout);
	Gb11->setTitle("Grid Surface Deformation");
	Gb11->layout()->setSpacing(3);
	Gb11->layout()->setMargin(5);

	QWidget *MultiGridDeformBox = new QWidget(Gb11); MultiGridDeformBox->setLayout(new QHBoxLayout());
	Gb11->layout()->addWidget(MultiGridDeformBox); 

	QLabel *Lxx = new QLabel(" Sub:", MultiGridDeformBox); MultiGridDeformBox->layout()->addWidget(Lxx);
	MultiGridDeformSubFramesBox = new QSpinBox2(spinBoxList,MultiGridDeformBox); MultiGridDeformSubFramesBox->setRange(0, 100); MultiGridDeformBox->layout()->addWidget(MultiGridDeformSubFramesBox);
	MultiGridDeformSubFramesBox->setValue(0);
	MultiGridDeformSubFramesBox->setMinimumWidth(30);
	ElasticLoopBox = new QCheckBox2(checkBoxList,MultiGridDeformBox); MultiGridDeformBox->layout()->addWidget(ElasticLoopBox);
	ElasticLoopBox->setText("Loop");
	ElasticLoopBox->setChecked(true);

	MultiGridDeformColorizeBox = new QCheckBox2(checkBoxList,MultiGridDeformBox); MultiGridDeformBox->layout()->addWidget(MultiGridDeformColorizeBox);
	MultiGridDeformColorizeBox->setText("Colorize");
	MultiGridDeformColorizeBox->setChecked(true);

	QWidget *MultiGridDeformBox2 = new QWidget(Gb11); MultiGridDeformBox2->setLayout(new QHBoxLayout());
	Gb11->layout()->addWidget(MultiGridDeformBox2);
	//QLabel *Lxy=new QLabel("Grid Deform ",MultiGridDeformBox2);
	MultiGridDeformButton = new QPushButton2(buttonList,"Surfaces", MultiGridDeformBox2); MultiGridDeformBox2->layout()->addWidget(MultiGridDeformButton);
	MultiGridDeformButton->setMinimumWidth(50);
	MultiGridDeformLinesButton = new QPushButton2(buttonList,"Lines", MultiGridDeformBox2); MultiGridDeformBox2->layout()->addWidget(MultiGridDeformLinesButton);
	MultiGridDeformLinesButton->setMinimumWidth(50);
	MultiGridDeformArrowsButton = new QPushButton2(buttonList,"Arrows", MultiGridDeformBox2); MultiGridDeformBox2->layout()->addWidget(MultiGridDeformArrowsButton);
	MultiGridDeformArrowsButton->setMinimumWidth(40);
	MultiGridDeformLinePathsButton = new QPushButton2(buttonList,"P", MultiGridDeformBox2); MultiGridDeformBox2->layout()->addWidget(MultiGridDeformLinePathsButton);
	MultiGridDeformLinePathsButton->setMinimumWidth(10);

	QWidget *MultiGridDeformBox3 = new QWidget(Gb11); MultiGridDeformBox3->setLayout(new QHBoxLayout());
	Gb11->layout()->addWidget(MultiGridDeformBox3);
	VectorDeformationComponentCombo = new QComboBox2(comboBoxList,MultiGridDeformBox3); MultiGridDeformBox3->layout()->addWidget(VectorDeformationComponentCombo);
	VectorDeformationComponentCombo->addItem("Vector");
	VectorDeformationComponentCombo->addItem("Normal");
	VectorDeformationComponentCombo->addItem("Tangential");
	MultiGridDeformRelativeBox = new QCheckBox2(checkBoxList,MultiGridDeformBox3); MultiGridDeformBox3->layout()->addWidget(MultiGridDeformRelativeBox);
	MultiGridDeformRelativeBox->setText("Rel");
	MultiGridDeformRelativeBox->setChecked(false);
	MultiGridTransBox = new QCheckBox2(checkBoxList,MultiGridDeformBox3); MultiGridDeformBox3->layout()->addWidget(MultiGridTransBox);
	MultiGridTransBox->setText("TR");
	MultiGridTransBox->setChecked(false);

	QWidget *MultiGridDeformBox4 = new QWidget(Gb11); MultiGridDeformBox4->setLayout(new QHBoxLayout());
Gb11->layout()->addWidget(MultiGridDeformBox4);
MultiGridAutoMaxDistBox = new QCheckBox2(checkBoxList,MultiGridDeformBox4); MultiGridDeformBox4->layout()->addWidget(MultiGridAutoMaxDistBox);
	MultiGridAutoMaxDistBox->setText("AutoMax");
	MultiGridAutoMaxDistBox->setChecked(true);
	//connect(MultiGridAutoMaxDistBox,SIGNAL(toggled (bool)),MultiGridMaxDistEdit,SLOT(setEnabled ( bool )));
	TextFieldDoubleValidator *Validatordd =new TextFieldDoubleValidator(1e-20,1e+20,20,this);
	Validatordd->setNotation(TextFieldDoubleValidator::StandardNotation);
	MultiGridMaxDistEdit = new QLineEdit2(lineEditList,MultiGridDeformBox4); MultiGridDeformBox4->layout()->addWidget(MultiGridMaxDistEdit);
	MultiGridMaxDistEdit->setText("0.0");
	MultiGridMaxDistEdit->setValidator(Validatordd);
	MultiGridMaxDistEdit->setEnabled(false);
	connect(MultiGridAutoMaxDistBox,SIGNAL(toggled(bool)),MultiGridMaxDistEdit,SLOT(setDisabled (bool)));



	//MultiGridDeformSubFramesEdit=new QLineEdit2(lineEditList,MultiGridDeformBox);
	//MultiGridDeformSubFramesEdit->setText("1.0");
	//MultiGridDeformAmountEdit->setValidator(Validatordd);


	/////////////////////////////////////////////////
	QGroupBox *Gb2=new QGroupBox(Info);//300,Vertical,Info);
	Gb2->setLayout(new QVBoxLayout);
	Gb2->setTitle("Surface Filtering");

	DepthSortSurfaceButton = new QPushButton2(buttonList, "Depth Sort",Gb2);
	Gb2->layout()->addWidget(DepthSortSurfaceButton);

	QWidget *Box1 = new QWidget(Gb2); Box1->setLayout(new QHBoxLayout());
	Gb2->layout()->addWidget(Box1);
	
	DecimateSurfaceButton = new QPushButton2(buttonList,"Decimate", Box1); Box1->layout()->addWidget(DecimateSurfaceButton);
	DecimateSurfaceButton->setMinimumWidth(30);
	DecimateValueEdit = new QLineEdit2(lineEditList,Box1); Box1->layout()->addWidget(DecimateValueEdit);
	DecimateValueEdit->setText("0.5");
	DecimateValueEdit->setValidator(Validator);

	DecimateErrorEdit = new QLineEdit2(lineEditList,Box1); Box1->layout()->addWidget(DecimateErrorEdit);
	DecimateErrorEdit->setText("0.002");
	DecimateErrorEdit->setValidator(Validator1);


	QWidget *Box3 = new QWidget(Gb2); Box3->setLayout(new QHBoxLayout());
	Gb2->layout()->addWidget(Box3);
	SmoothSurfaceButton = new QPushButton2(buttonList,"Smooth", Box3); Box3->layout()->addWidget(SmoothSurfaceButton);
	SmoothSurfaceButton->setMinimumWidth(60);
	QLabel *SL1 = new QLabel(" F:", Box3); Box3->layout()->addWidget(SL1);
	SmoothFactorValueEdit = new QLineEdit2(lineEditList,Box3); Box3->layout()->addWidget(SmoothFactorValueEdit);
	SmoothFactorValueEdit->setText("0.01");
	SmoothFactorValueEdit->setValidator(Validator);
	QLabel *SL2 = new QLabel(" Iter:", Box3); Box3->layout()->addWidget(SL2);
	SmoothIterValueEdit = new QLineEdit2(lineEditList,Box3); Box3->layout()->addWidget(SmoothIterValueEdit);
	SmoothIterValueEdit->setText("100");
	SmoothIterValueEdit->setValidator(Validator2);


	QWidget *BoxX = new QWidget(Gb2); BoxX->setLayout(new QHBoxLayout());
		Gb2->layout()->addWidget(BoxX);
		LargestSurfaceOnlyButton = new QPushButton2(buttonList,"Keep Largest", BoxX); BoxX->layout()->addWidget(LargestSurfaceOnlyButton);
		ClosestToPointerSurfaceOnlyButton = new QPushButton2(buttonList,"Keep Closest", BoxX); BoxX->layout()->addWidget(ClosestToPointerSurfaceOnlyButton);
	

		BoxX = new QWidget(Gb2); BoxX->setLayout(new QHBoxLayout());
	Gb2->layout()->addWidget(BoxX);
	extractSeparatedSurfacesButton = new QPushButton2(buttonList,"Separate All", BoxX); BoxX->layout()->addWidget(extractSeparatedSurfacesButton);

	QWidget *Box2=new QWidget(Gb2);
		Gb2->layout()->addWidget(Box2);
	DeformSurfaceButton = new QPushButton2(buttonList, "Deform",Box2);
	DeformSurfaceButton->setMinimumWidth(40);
	DeformValueEdit=new QLineEdit2(lineEditList,Box2);
	DeformValueEdit->setText("50.0");
	DeformValueEdit->setValidator(Validator1);

	//-------------------------------------------

	//QGroupBox *GbTransform=new QGroupBox(Gb2);//5,Vertical,Gb2);
	//Gb2->layout()->addWidget(GbTransform);
	//GbTransform->setLayout(new QVBoxLayout);
	//GbTransform->layout()->setSpacing(3);
	//GbTransform->layout()->setMargin(5);
	QWidget *TransformBox = new QWidget(Gb2); TransformBox->setLayout(new QHBoxLayout());
	Gb2->layout()->addWidget(TransformBox);
	
	TransformSurfaceButton = new QPushButton2(buttonList,"Transform", TransformBox); TransformBox->layout()->addWidget(TransformSurfaceButton);
	///-----------------
	ShowTransformSurfaceParamsButton = new  QPushButton2(buttonList,"Parameters", TransformBox); TransformBox->layout()->addWidget(ShowTransformSurfaceParamsButton);
	TransformSurfaceParamsWidget=new QWidget();
//	TransformSurfaceParamsWidget->setCaption("Surface Transformation Panel");
	connect(ShowTransformSurfaceParamsButton,SIGNAL(clicked()),TransformSurfaceParamsWidget,SLOT(show()));
	TransformSurfaceParamsWidget->setLayout(new QVBoxLayout());
//	QVBoxLayout* TransformSurfaceParamsLayout= new QVBoxLayout( TransformSurfaceParamsWidget, 10, -1);
	QGroupBox *TransformSurfaceParamsGroupBox=new QGroupBox(TransformSurfaceParamsWidget);

	TransformSurfaceParamsGroupBox->setLayout(new QVBoxLayout);
	TransformSurfaceParamsWidget->layout()->addWidget(TransformSurfaceParamsGroupBox);	
	//---------------------

	
	SetSurfaceOriginButton= new QPushButton2(buttonList, "Set Origin",TransformSurfaceParamsGroupBox);TransformSurfaceParamsGroupBox->layout()->addWidget(SetSurfaceOriginButton);
	TransformSurfaceButton->setMinimumWidth(40);
	SetSurfaceOriginButton->setMinimumWidth(50);
	computeSurfaceCenterButton= new QPushButton2(buttonList, "Calculate Center",TransformSurfaceParamsGroupBox);TransformSurfaceParamsGroupBox->layout()->addWidget(computeSurfaceCenterButton);
	

	QWidget *ScaleBox = new QWidget(TransformSurfaceParamsGroupBox); ScaleBox->setLayout(new QHBoxLayout);

	TransformSurfaceParamsGroupBox->layout()->addWidget(ScaleBox); 
	QLabel *ScaleL1 = new QLabel("Scale X:", ScaleBox); ScaleBox->layout()->addWidget(ScaleL1);
	SurfaceScaleXEdit = new QLineEdit2(lineEditList,ScaleBox); ScaleBox->layout()->addWidget(SurfaceScaleXEdit);
	SurfaceScaleXEdit->setText("1.0");
	SurfaceScaleXEdit->setValidator(Validator1);

	QLabel *ScaleL2 = new QLabel(" Y:", ScaleBox); ScaleBox->layout()->addWidget(ScaleL2);
	SurfaceScaleYEdit = new QLineEdit2(lineEditList,ScaleBox); ScaleBox->layout()->addWidget(SurfaceScaleYEdit);
	SurfaceScaleYEdit->setText("1.0");
	SurfaceScaleYEdit->setValidator(Validator1);

	QLabel *ScaleL3 = new QLabel(" Z:", ScaleBox); ScaleBox->layout()->addWidget(ScaleL3);
	SurfaceScaleZEdit = new QLineEdit2(lineEditList,ScaleBox); ScaleBox->layout()->addWidget(SurfaceScaleZEdit);
	SurfaceScaleZEdit->setText("1.0");
	SurfaceScaleZEdit->setValidator(Validator1);


	QWidget *ShearBox = new QWidget(TransformSurfaceParamsGroupBox); ShearBox->setLayout(new QHBoxLayout);
	TransformSurfaceParamsGroupBox->layout()->addWidget(ShearBox);
	QLabel *ShearL1 = new QLabel("Shear X:", ShearBox); ShearBox->layout()->addWidget(ShearL1);
	SurfaceShearXEdit = new QLineEdit2(lineEditList,ShearBox); ShearBox->layout()->addWidget(SurfaceShearXEdit);
	SurfaceShearXEdit->setText("0.0");
	SurfaceShearXEdit->setValidator(Validator1);

	QLabel *ShearL2 = new QLabel(" Y:", ShearBox); ShearBox->layout()->addWidget(ShearL2);
	SurfaceShearYEdit = new QLineEdit2(lineEditList,ShearBox); ShearBox->layout()->addWidget(SurfaceShearYEdit);
	SurfaceShearYEdit->setText("0.0");
	SurfaceShearYEdit->setValidator(Validator1);

	QLabel *ShearL3 = new QLabel(" Z:", ShearBox); ShearBox->layout()->addWidget(ShearL3);
	SurfaceShearZEdit = new QLineEdit2(lineEditList,ShearBox); ShearBox->layout()->addWidget(SurfaceShearZEdit);
	SurfaceShearZEdit->setText("0.0");
	SurfaceShearZEdit->setValidator(Validator1);



	QWidget *OriginBox = new QWidget(TransformSurfaceParamsGroupBox); OriginBox->setLayout(new QHBoxLayout);
	TransformSurfaceParamsGroupBox->layout()->addWidget(OriginBox);
	QLabel *OriginL1 = new QLabel("Origin X:", OriginBox); OriginBox->layout()->addWidget(OriginL1);
	SurfaceOriginXEdit = new QLineEdit2(lineEditList,OriginBox); OriginBox->layout()->addWidget(SurfaceOriginXEdit);
	SurfaceOriginXEdit->setText("0.0");
	SurfaceOriginXEdit->setValidator(Validator1);

	QLabel *OriginL2 = new QLabel(" Y:", OriginBox); OriginBox->layout()->addWidget(OriginL2);
	SurfaceOriginYEdit = new QLineEdit2(lineEditList,OriginBox); OriginBox->layout()->addWidget(SurfaceOriginYEdit);
	SurfaceOriginYEdit->setText("0.0");
	SurfaceOriginYEdit->setValidator(Validator1);

	QLabel *OriginL3 = new QLabel(" Z:", OriginBox); OriginBox->layout()->addWidget(OriginL3);
	SurfaceOriginZEdit = new QLineEdit2(lineEditList,OriginBox); OriginBox->layout()->addWidget(SurfaceOriginZEdit);
	SurfaceOriginZEdit->setText("0.0");
	SurfaceOriginZEdit->setValidator(Validator1);

	//-------------------------------------------
	QWidget *SphereBox = new QWidget(Gb2); SphereBox->setLayout(new QHBoxLayout);
		Gb2->layout()->addWidget(SphereBox);
		SphereGenerationButton = new QPushButton2(buttonList,"Sphere", SphereBox); SphereBox->layout()->addWidget(SphereGenerationButton);
	SphereGenerationButton->setMinimumWidth(40);

	QLabel *SphL1 = new QLabel(" Phi:", SphereBox); SphereBox->layout()->addWidget(SphL1);
	SpherePhiEdit = new QLineEdit2(lineEditList,SphereBox); SphereBox->layout()->addWidget(SpherePhiEdit);
	SpherePhiEdit->setText("10.0");
	SpherePhiEdit->setValidator(Validator1);


	QLabel *SphL2 = new QLabel(" Theta:", SphereBox); SphereBox->layout()->addWidget(SphL2);
	SphereThetaEdit = new QLineEdit2(lineEditList,SphereBox); SphereBox->layout()->addWidget(SphereThetaEdit);
	SphereThetaEdit->setText("10.0");
	SphereThetaEdit->setValidator(Validator1);
	//-----------------------------------------

	QWidget *ButtonBox1 = new QWidget(Gb2); ButtonBox1->setLayout(new QHBoxLayout);
		Gb2->layout()->addWidget(ButtonBox1);
		FindMaxDistanceButton = new QPushButton2(buttonList,"MaxDistance", ButtonBox1); ButtonBox1->layout()->addWidget(FindMaxDistanceButton);
	//-----------------------------------------

	
	TransSurfacePointButton = new QPushButton2(buttonList,"Trans Point", ButtonBox1); ButtonBox1->layout()->addWidget(TransSurfacePointButton);
	TransSurfacePointButton->setMinimumWidth(30);



	QWidget *ButtonBox1a = new QWidget(Gb2); ButtonBox1a->setLayout(new QHBoxLayout);
	Gb2->layout()->addWidget(ButtonBox1a);
	PointNumberTextEdit = new QLineEdit2(lineEditList,ButtonBox1a); ButtonBox1a->layout()->addWidget(PointNumberTextEdit);
	PointNumberTextEdit->setText("0");
	QValidator *Validator3 = new QIntValidator(1, 99999999, this);
	PointNumberTextEdit->setValidator(Validator3);


	
	TransSurfacePointSlider	= new QSlider2(sliderList,Qt::Horizontal, ButtonBox1a);
	ButtonBox1a->layout()->addWidget(TransSurfacePointSlider);
	TransSurfacePointSlider->setRange(0,0);
	TransSurfacePointSlider->setValue(0);
	connect(TransSurfacePointSlider,SIGNAL(sliderPressed ()),TransSurfacePointSlider,SLOT(setFocus()));

	QWidget *Box22 = new QWidget(Gb2); Box22->setLayout(new QHBoxLayout);
		Gb2->layout()->addWidget(Box22);

		ComputeNormalsButton = new QPushButton2(buttonList,"Compute Normals", Box22); Box22->layout()->addWidget(ComputeNormalsButton);
		CleanPolyDataButton = new QPushButton2(buttonList,"Clean", Box22); Box22->layout()->addWidget(CleanPolyDataButton);
	CleanPolyDataButton->setMinimumWidth(40);


	//QGroupBox *Gb3=new QGroupBox(Info);//300,Vertical,Info);
	//Gb3->setLayout(new QVBoxLayout);
	//Gb3->setTitle("Surface Tools");

	//Box22 = new QWidget(Gb3); Box22->setLayout(new QHBoxLayout);
	//Gb3->layout()->addWidget(Box22);

	//SetRandomColorsToSelectedSurfacesButton = new QPushButton2(buttonList,"Set Random Color", Box22); Box22->layout()->addWidget(SetRandomColorsToSelectedSurfacesButton);
	//SetRandomColorsToSelectedSurfacesButton->setMinimumWidth(40);
	//Box22 = new QWidget(Gb3); Box22->setLayout(new QHBoxLayout);
	//Gb3->layout()->addWidget(Box22);
	////QPushButton *GenerateSphericalPointsCutsBuutton;
	//GenerateSphericalPointsCutsBuutton = new QPushButton2(buttonList,"Generate Spherical Points Cuts", Box22); Box22->layout()->addWidget(GenerateSphericalPointsCutsBuutton);
	//GenerateSphericalPointsCutsBuutton->setMinimumWidth(40);



	ExtractLinesFromDataButton = new QPushButton2(buttonList, "Extract Lines", Gb2); Gb2->layout()->addWidget(ExtractLinesFromDataButton);
	GenerateDistanceChartButton = new QPushButton2(buttonList, "Generate Distance Chart", Gb2); Gb2->layout()->addWidget(GenerateDistanceChartButton);



	MainLayout->addWidget(Gb1);
	MainLayout->addWidget(Gb11);
	MainLayout->addWidget(PropBox1);
	MainLayout->addWidget(Gb2);
	//MainLayout->addWidget(Gb3);

	Gb1->layout()->setSpacing(3);
	Gb1->layout()->setMargin(5);
	Gb2->layout()->setSpacing(3);
	Gb2->layout()->setMargin(5);

	//qtab->addTab(Info,"&Surface Processing");
}

//--------------------------------------------------------------------

void GUIPanelWidget::CreateSurfaceRegistrationTab(MyDockWidget *qtab)
{

	TextFieldDoubleValidator *Validator1 =new TextFieldDoubleValidator(-99999999.0,99999999,15,this);
	Validator1->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *Validator2 =new TextFieldDoubleValidator(0.0001,100.0,4,this);
	//	QValidator *ValidatorInt =new QIntValidator(0,512,this);

	QWidget *Info = new QWidget(qtab);
	qtab->setScrollWidget(Info);
	QLayout* MainLayout = new QVBoxLayout();
	Info->setLayout(MainLayout);

	QGroupBox *Gb1=new QGroupBox(Info);
	Gb1->setLayout(new QVBoxLayout);
	Gb1->layout()->setSpacing(0);
	Gb1->layout()->setMargin(1);

	Gb1->setTitle("Surface Matching");


	GenerateDistanceMapButton=new QPushButton2(buttonList,"DistanceMap",Gb1);	
	Gb1->layout()->addWidget(GenerateDistanceMapButton);

	GenerateDistanceMapWithAutoCropButton = new QPushButton2(buttonList, "GenerateDistanceMapWithAutoCropButton", Gb1);
	Gb1->layout()->addWidget(GenerateDistanceMapWithAutoCropButton);

	
	int ss=128;

	QWidget *DistStepBox = new QWidget(Gb1);	DistStepBox->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(DistStepBox);
	//	QLabel *DistStepL0=new QLabel("Spc:",DistStepBox);

	QLabel *DistStepL1 = new QLabel(" X:", DistStepBox); DistStepBox->layout()->addWidget(DistStepL1);
	DistStepXBox = new QSpinBox2(spinBoxList,DistStepBox); DistStepXBox->setRange(1, 512); DistStepBox->layout()->addWidget(DistStepXBox);
	DistStepXBox->setValue(ss);
	DistStepXBox->setMaximumWidth(40);

	QLabel *DistStepL2 = new QLabel(" Y:", DistStepBox); DistStepBox->layout()->addWidget(DistStepL2);
	DistStepYBox = new QSpinBox2(spinBoxList,DistStepBox); DistStepYBox->setRange(1, 512); DistStepBox->layout()->addWidget(DistStepYBox);
	DistStepYBox->setValue(ss);
	DistStepYBox->setMaximumWidth(40);

	QLabel *DistStepL3 = new QLabel("Z:", DistStepBox); DistStepBox->layout()->addWidget(DistStepL3);
	DistStepZBox = new QSpinBox2(spinBoxList,DistStepBox); DistStepZBox->setRange(1, 512); DistStepBox->layout()->addWidget(DistStepZBox);
	DistStepZBox->setValue(ss);
	DistStepZBox->setMaximumWidth(40);


	
	
	QWidget *xbg = new QWidget(Gb1);
	xbg->setLayout(new QHBoxLayout());
	xbg->layout()->setSpacing(0);
	xbg->layout()->setMargin(1);


	ApplyAffineTransformationFromVolumePanelButton=new QPushButton2(buttonList,"Affine Vol Trans",xbg);
	xbg->layout()->addWidget(ApplyAffineTransformationFromVolumePanelButton);

	invAffineCheckBox = new QCheckBox2(checkBoxList,xbg);
	invAffineCheckBox->setText("inv");
	invAffineCheckBox->setChecked(true);
	xbg->layout()->addWidget(invAffineCheckBox);
	Gb1->layout()->addWidget(xbg);
	connect(ApplyAffineTransformationFromVolumePanelButton,SIGNAL(clicked ()),this,SLOT(SlotSurfacesTransformButtonPressed()));
	//Gb1->layout()->addWidget(ApplyAffineTransformationFromVolumePanelButton);

	ApplySurfaceMatchingButton=new QPushButton2(buttonList,"SurfaceMatching",Gb1);
	Gb1->layout()->addWidget(ApplySurfaceMatchingButton);
	QWidget *PercentBox = new QWidget(Gb1);		PercentBox->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(PercentBox);
	QLabel *L1 = new QLabel("Percent: ", PercentBox); PercentBox->layout()->addWidget(L1);
	PointPercentSpinBox = new QSpinBox2(spinBoxList,PercentBox); PointPercentSpinBox->setRange(0, 100); PercentBox->layout()->addWidget(PointPercentSpinBox);
	PointPercentSpinBox->setValue(100);


	BoundingBoxPointsCheckBox = new QCheckBox2(checkBoxList,PercentBox); PercentBox->layout()->addWidget(BoundingBoxPointsCheckBox);
	BoundingBoxPointsCheckBox->setText("Only BB");
	BoundingBoxPointsCheckBox->setChecked(false);

	QWidget* maxValBox = new QWidget(Gb1);		maxValBox->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(maxValBox);
	QLabel* L1x = new QLabel("MaxOverride: ", maxValBox); maxValBox->layout()->addWidget(L1x);
	maxValueOverrideForDistanceMapLineEdit = new QLineEdit2(lineEditList, maxValBox);
	maxValBox->layout()->addWidget(maxValueOverrideForDistanceMapLineEdit);
	

	GenerateSurfaceDistMapButton=new QPushButton2(buttonList,"Map Surface Diff",Gb1);
		Gb1->layout()->addWidget(GenerateSurfaceDistMapButton);

	ApplySufaceMatchingGenerateGraphsButton=new QPushButton2(buttonList,"GenerateGraphs",Gb1);
			Gb1->layout()->addWidget(ApplySufaceMatchingGenerateGraphsButton);



	ShowHide3DSectionButton=new QPushButton2(buttonList,"Show/Hide 3D Section",Gb1);
			Gb1->layout()->addWidget(ShowHide3DSectionButton);



			QWidget *GridCol = new QWidget(Gb1); GridCol->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(GridCol);
			//Gb1->layout()->addWidget(Q3VButtonGroup);
	//GridCol->setTitle("Grab Options");
	QRadioButton *NoneGrabRadioButton = new QRadioButton(GridCol); GridCol->layout()->addWidget(NoneGrabRadioButton);
	GrabAppOnRegRadioButton = new QRadioButton(GridCol); GridCol->layout()->addWidget(GrabAppOnRegRadioButton);
	Grab3DWindowOnRegRadioButton = new QRadioButton(GridCol); GridCol->layout()->addWidget(Grab3DWindowOnRegRadioButton);

	NoneGrabRadioButton->setText("Don't Grab");
	GrabAppOnRegRadioButton->setText("Grab Application");
	Grab3DWindowOnRegRadioButton->setText("Grab 3D Window");

	NoneGrabRadioButton->setChecked(true);


	/////////////////////


	QGroupBox *FFDGroupBox=new QGroupBox(Info);
	FFDGroupBox->setLayout(new QVBoxLayout);
	FFDGroupBox->layout()->setSpacing(0);
	FFDGroupBox->layout()->setMargin(1);

	FFDGroupBox->setTitle("Elastic Registration");

	QWidget *GridSizeBox = new QWidget(FFDGroupBox); GridSizeBox->setLayout(new QHBoxLayout());
		FFDGroupBox->layout()->addWidget(GridSizeBox);
	//	QLabel *GridSizeL0=new QLabel("Spc:",GridSizeBox);

		QLabel *GridSizeL1 = new QLabel(" X:", GridSizeBox); GridSizeBox->layout()->addWidget(GridSizeL1);
		GridSizeXBox = new QSpinBox2(spinBoxList,GridSizeBox); GridSizeXBox->setRange(2, 512); GridSizeBox->layout()->addWidget(GridSizeXBox);
	GridSizeXBox->setValue(4);
	GridSizeXBox->setMaximumWidth(40);

	QLabel *GridSizeL2 = new QLabel(" Y:", GridSizeBox); GridSizeBox->layout()->addWidget(GridSizeL2);
	GridSizeYBox = new QSpinBox2(spinBoxList,GridSizeBox); GridSizeYBox->setRange(2, 512); GridSizeBox->layout()->addWidget(GridSizeYBox);
	GridSizeYBox->setValue(4);
	GridSizeYBox->setMaximumWidth(40);

	QLabel *GridSizeL3 = new QLabel("Z:", GridSizeBox); GridSizeBox->layout()->addWidget(GridSizeL3);
	GridSizeZBox = new QSpinBox2(spinBoxList,GridSizeBox); GridSizeZBox->setRange(2, 512); GridSizeBox->layout()->addWidget(GridSizeZBox);
	GridSizeZBox->setValue(4);
	GridSizeZBox->setMaximumWidth(40);

	QWidget *SurfaceElsasticOptimBox = new QWidget(FFDGroupBox); SurfaceElsasticOptimBox->setLayout(new QHBoxLayout());
	FFDGroupBox->layout()->addWidget(SurfaceElsasticOptimBox);
	ApplyElasticSurfaceMatchingButton = new QPushButton2(buttonList,"Start!", SurfaceElsasticOptimBox); SurfaceElsasticOptimBox->layout()->addWidget(ApplyElasticSurfaceMatchingButton);

	QLabel *ElasticIterL34 = new QLabel(" Iter:", SurfaceElsasticOptimBox); SurfaceElsasticOptimBox->layout()->addWidget(ElasticIterL34);
	SurfaceElasticIterBox = new QSpinBox2(spinBoxList,SurfaceElsasticOptimBox); SurfaceElasticIterBox->setRange(1, 512); SurfaceElsasticOptimBox->layout()->addWidget(SurfaceElasticIterBox);
	SurfaceElasticIterBox->setValue(1);

	SurfaceElasticUpdateGridBox = new QCheckBox2(checkBoxList,SurfaceElsasticOptimBox); SurfaceElsasticOptimBox->layout()->addWidget(SurfaceElasticUpdateGridBox);
	SurfaceElasticUpdateGridBox->setText("Update");
	SurfaceElasticUpdateGridBox->setChecked(false);

	ApplyElasticSurfaceMatchingButton->setMinimumWidth(50);
//	SurfaceElasticUpdateGridBox->setMinimumWidth(40);
	SurfaceElasticIterBox->setMaximumWidth(30);



	
	QWidget *GridMatchingBox = new QWidget(FFDGroupBox); GridMatchingBox->setLayout(new QHBoxLayout());
	FFDGroupBox->layout()->addWidget(GridMatchingBox);
	Create3DGridPolyDataButton = new QPushButton2(buttonList,"Create Grid", GridMatchingBox); GridMatchingBox->layout()->addWidget(Create3DGridPolyDataButton);

//	Create3DGridPolyDataButton->setMinimumWidth(50);
//	Clear3DGridPolyDataButton->setMinimumWidth(40);
	QLabel *GridSizeL21 = new QLabel(" Density:", GridMatchingBox); GridMatchingBox->layout()->addWidget(GridSizeL21);
	GridDensityBox = new QSpinBox2(spinBoxList,GridMatchingBox); GridDensityBox->setRange(1, 512); GridMatchingBox->layout()->addWidget(GridDensityBox);
	GridDensityBox->setValue(5);
	GridDensityBox->setMaximumWidth(40);


	QWidget *LoadSaveTransGridBox = new QWidget(FFDGroupBox); LoadSaveTransGridBox->setLayout(new QHBoxLayout());
	FFDGroupBox->layout()->addWidget(LoadSaveTransGridBox);
	LoadGrid3DButton = new QPushButton2(buttonList,"Load Grid", LoadSaveTransGridBox); LoadSaveTransGridBox->layout()->addWidget(LoadGrid3DButton);
	SaveGrid3DButton = new QPushButton2(buttonList,"Save Grid", LoadSaveTransGridBox); LoadSaveTransGridBox->layout()->addWidget(SaveGrid3DButton);
	Clear3DGridPolyDataButton = new QPushButton2(buttonList,"Clear", LoadSaveTransGridBox); LoadSaveTransGridBox->layout()->addWidget(Clear3DGridPolyDataButton);
	Clear3DGridPolyDataButton->setMinimumWidth(40);
	LoadGrid3DButton->setMinimumWidth(60);
	SaveGrid3DButton->setMinimumWidth(60);

	QWidget *TransGridBox = new QWidget(FFDGroupBox); TransGridBox->setLayout(new QHBoxLayout());
	FFDGroupBox->layout()->addWidget(TransGridBox);

	ElasticTransformationButton = new QPushButton2(buttonList,"Transform", TransGridBox); TransGridBox->layout()->addWidget(ElasticTransformationButton);

	QLabel *Lxx = new QLabel(" Amount:", TransGridBox); TransGridBox->layout()->addWidget(Lxx);

	TextFieldDoubleValidator *Validatordd =new TextFieldDoubleValidator(0.001,1.0,20,this);
	Validatordd->setNotation(TextFieldDoubleValidator::StandardNotation);
	ElasticTransAmountEdit = new QLineEdit2(lineEditList,TransGridBox); TransGridBox->layout()->addWidget(ElasticTransAmountEdit);
	ElasticTransAmountEdit->setText("1.0");
	ElasticTransAmountEdit->setValidator(Validatordd);

	QWidget *sthBox = new QWidget(FFDGroupBox); sthBox->setLayout(new QHBoxLayout());
	FFDGroupBox->layout()->addWidget(sthBox);
	ReverseGridButton = new QPushButton2(buttonList,"Reverse Grid", sthBox); sthBox->layout()->addWidget(ReverseGridButton);
	GenerateTestGridButton = new QPushButton2(buttonList,"Test", sthBox); sthBox->layout()->addWidget(GenerateTestGridButton);

	QWidget *sth2Box = new QWidget(FFDGroupBox); sth2Box->setLayout(new QHBoxLayout());
	FFDGroupBox->layout()->addWidget(sth2Box);
	ScaleGridButton = new QPushButton2(buttonList,"Scale Grid", sth2Box); sth2Box->layout()->addWidget(ScaleGridButton);
	//////////////////////////////////////////////////////////////////////////////

///-----------------
	ShowBoundingBoxParamsButton=new  QPushButton2(buttonList,"Bounding Box Params",Gb1);
		Gb1->layout()->addWidget(ShowBoundingBoxParamsButton);
	BoundingBoxParamsWidget=new QWidget();
//	BoundingBoxParamsWidget->setName("Bounding Box Params Panel");
	BoundingBoxParamsWidget->setLayout(new QVBoxLayout());
	connect(ShowBoundingBoxParamsButton,SIGNAL(clicked()),BoundingBoxParamsWidget,SLOT(show()));

	//QVBoxLayout* BoundingBoxParamsLayout= new QVBoxLayout( BoundingBoxParamsWidget, 10, -1);

	QGroupBox *BoundingBoxParamsGroupBox=new QGroupBox(BoundingBoxParamsWidget);
	BoundingBoxParamsGroupBox->setLayout(new QVBoxLayout);
	BoundingBoxParamsWidget->layout()->addWidget(BoundingBoxParamsGroupBox);	
	
	
	//---------------------

	
	 
	TextFieldDoubleValidator *Validator =new TextFieldDoubleValidator(0.0000001,9999999,20,this);
	Validator->setNotation(TextFieldDoubleValidator::StandardNotation);


	QGroupBox *TransformGroupBox = new QGroupBox(BoundingBoxParamsGroupBox); TransformGroupBox->setLayout(new QHBoxLayout());
	BoundingBoxParamsGroupBox->layout()->addWidget(TransformGroupBox);
	
	TransformGroupBox->setTitle("Bounding Box Sizes");
	//	TransformLayout->addWidget(TransformGroupBox);

	QLabel		*BBoxLabel[4];



	QWidget* OutsideBox[4];
	QWidget	*Box[4]; 

	int i;
	QString NrText="0.0";

	for(i=0;i<4;i++)
	{

		OutsideBox[i] = new QWidget(TransformGroupBox); 
		TransformGroupBox->layout()->addWidget(OutsideBox[i]);
		Box[i]	=new QWidget(OutsideBox[i]);
//		Box[i]->setSpacing(5);

		BBoxLabel[i]=new QLabel(Box[i]);
		BBoxEdit[i]=new QLineEdit2(lineEditList,Box[i]);
		BBoxEdit[i]->setValidator(Validator);
		BBoxEdit[i]->setMaximumWidth(50);


		if (i==9) NrText="1.0";
		BBoxEdit[i]->setText(NrText);

		//BBoxSlider[i]=new QSlider2(sliderList,-100,100,1,0,Qt::Horizontal, Box[i]);
		BBoxSlider[i] = new QSlider2(sliderList,Qt::Horizontal, Box[i]);
		BBoxSlider[i]->setRange(-100, 100);
		BBoxSlider[i]->setValue(0);

		if (i<12) connect(TCheckBox[i],SIGNAL(toggled(bool)),Box[i],SLOT(setEnabled (bool)));

	}

	BBoxLabel[0]->setText("Bx:");
	BBoxLabel[1]->setText("By:");
	BBoxLabel[2]->setText("Bz:");
	BBoxLabel[3]->setText("Range:");
	BBoxEdit[3]->setText("10.0");

	for(i=0;i<3;i++)
		BBoxParams[i]=BBoxEdit[i]->text().toFloat();
	////////////////////////////////////////////////////////////////////////////

	for(i=0;i<4;i++)
	{
		connect(BBoxSlider[i],SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateBBoxSizes(int)));
		connect(BBoxSlider[i],SIGNAL(sliderReleased ()),this,SLOT(SlotUpdateBBoxSizes()));
		connect(BBoxEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateBBoxSizes(const QString &)));

	}


	MainLayout->addWidget(Gb1);

	MainLayout->addWidget(FFDGroupBox);
//	MainLayout->addWidget(ShowBoundingBoxParamsButton);

//	qtab->addTab(Info,"&Registration");

}

//--------------------------------------------------------------------

void GUIPanelWidget::CreateSequenceGenerationTab(MyDockWidget *qtab)
{

	TextFieldDoubleValidator *Validator1 =new TextFieldDoubleValidator(-99999999.0,99999999,15,this);
	Validator1->setNotation(TextFieldDoubleValidator::StandardNotation);
	QWidget *Info = new QWidget(qtab);
	qtab->setScrollWidget(Info);
	QLayout* MainLayout = new QVBoxLayout();	Info->setLayout(MainLayout);

	QGroupBox *Gb1=new QGroupBox(Info);
	Gb1->setLayout(new QVBoxLayout);

	Gb1->setTitle("Animation Generator");

	//--------------------------------

	animOnlyOpacityOfGroupsCheckBox= new QCheckBox2(checkBoxList, Gb1);
	Gb1->layout()->addWidget(animOnlyOpacityOfGroupsCheckBox);
	animOnlyOpacityOfGroupsCheckBox->setText("Animate Only Opacity Of Groups");

	


	updateVolumeCheckBox = new QCheckBox2(checkBoxList, Gb1);
	Gb1->layout()->addWidget(updateVolumeCheckBox);
	updateVolumeCheckBox->setText("Update Volume");

	QGroupBox *TransformBox = new QGroupBox(Gb1); 
	Gb1->layout()->addWidget(TransformBox);
	TransformBox->setLayout(new QVBoxLayout);

	//	QHBox *TransformBox=new QVBox(Gb2X);
	TransformBox->layout()->setSpacing(3);
	TransformBox->layout()->setMargin(5);

	QWidget *AzimuthBox = new QWidget(TransformBox); AzimuthBox->setLayout(new QHBoxLayout());
	TransformBox->layout()->addWidget(AzimuthBox);
	QLabel *AzimuthL1 = new QLabel("Azimuth:", AzimuthBox); AzimuthBox->layout()->addWidget(AzimuthL1);
	TransformCameraAzimuthEdit = new QLineEdit2(lineEditList,AzimuthBox); AzimuthBox->layout()->addWidget(TransformCameraAzimuthEdit);
	TransformCameraAzimuthEdit->setText("0.0");
	TransformCameraAzimuthEdit->setValidator(Validator1);


	QWidget *ElevationBox = new QWidget(TransformBox);	 ElevationBox->setLayout(new QHBoxLayout());
	TransformBox->layout()->addWidget(ElevationBox);
	QLabel *ElevationL1 = new QLabel("Elevation:", ElevationBox); ElevationBox->layout()->addWidget(ElevationL1);
	TransformCameraElevationEdit = new QLineEdit2(lineEditList,ElevationBox); ElevationBox->layout()->addWidget(TransformCameraElevationEdit);
	TransformCameraElevationEdit->setText("0.0");
	TransformCameraElevationEdit->setValidator(Validator1);


	QWidget *RollBox = new QWidget(TransformBox); RollBox->setLayout(new QHBoxLayout());
	TransformBox->layout()->addWidget(RollBox);
	QLabel *RollL1 = new QLabel(" Roll:", RollBox); RollBox->layout()->addWidget(RollL1);
	TransformCameraRollEdit = new QLineEdit2(lineEditList,RollBox); RollBox->layout()->addWidget(TransformCameraRollEdit);
	TransformCameraRollEdit->setText("0.0");
	TransformCameraRollEdit->setValidator(Validator1);


	showCameraAnglesButton= new QPushButton2(buttonList, "ShowCurrentCamAnlges", TransformBox); TransformBox->layout()->addWidget(showCameraAnglesButton);
	connect(showCameraAnglesButton, SIGNAL(clicked()), this, SLOT(SlotShowCurrentCameraAngles()));

	AzimuthL1->setMinimumWidth(70);
	ElevationL1->setMinimumWidth(70);
	RollL1->setMinimumWidth(90);

	QWidget *AnimBoxX = new QWidget(TransformBox); AnimBoxX->setLayout(new QHBoxLayout());
		TransformBox->layout()->addWidget(AnimBoxX);

		TransformCameraButton = new QPushButton2(buttonList,"RotCam", AnimBoxX); AnimBoxX->layout()->addWidget(TransformCameraButton);
		TransformCameraAndGrabButton = new QPushButton2(buttonList,"Rot&Grab", AnimBoxX); AnimBoxX->layout()->addWidget(TransformCameraAndGrabButton);
		Grab3DWindowButton = new QPushButton2(buttonList,"Grab3D", AnimBoxX); AnimBoxX->layout()->addWidget(Grab3DWindowButton);


		AnimBoxX = new QWidget(TransformBox); AnimBoxX->setLayout(new QHBoxLayout());
		TransformBox->layout()->addWidget(AnimBoxX);

		CopyToClipBoardCameraButton = new QPushButton2(buttonList, "CopyCamSettings", AnimBoxX); AnimBoxX->layout()->addWidget(CopyToClipBoardCameraButton);
		PasteFromClipboardCameraButton = new QPushButton2(buttonList, "PasteCamSettings", AnimBoxX); AnimBoxX->layout()->addWidget(PasteFromClipboardCameraButton);


	TransformCameraButton->setMinimumWidth(40);
	TransformCameraAndGrabButton->setMinimumWidth(40);
	Grab3DWindowButton->setMinimumWidth(40);

	QGroupBox *AnimationBox=new QGroupBox(Gb1);
		Gb1->layout()->addWidget(AnimationBox);

	AnimationBox->setLayout(new QVBoxLayout);
	AnimationBox->setTitle("Animation");

	AnimationBox->layout()->setSpacing(3);
	AnimationBox->layout()->setMargin(5);


	///---------------------------------------------------------

	QWidget *AnimBox1 = new QWidget(AnimationBox); AnimBox1->setLayout(new QHBoxLayout());
		AnimationBox->layout()->addWidget(AnimBox1);
		QLabel *Label1 = new QLabel("KeyFr: ", AnimBox1); AnimBox1->layout()->addWidget(Label1);
		NumberOfAllKeyFramesLabel = new QLabel( "0", AnimBox1); AnimBox1->layout()->addWidget(NumberOfAllKeyFramesLabel);
		QLabel *Label2 = new QLabel("Current: ", AnimBox1); AnimBox1->layout()->addWidget(Label2);
		CurrentKeyFrameNrEdit = new QLineEdit2(lineEditList, "0", AnimBox1); AnimBox1->layout()->addWidget(CurrentKeyFrameNrEdit);CurrentKeyFrameNrEdit->setFixedWidth(30);

		Label2 = new QLabel("t: ", AnimBox1); AnimBox1->layout()->addWidget(Label2);
		CurrentTKeyFrameNrEdit = new QLineEdit2(lineEditList,"0", AnimBox1); AnimBox1->layout()->addWidget(CurrentTKeyFrameNrEdit);CurrentTKeyFrameNrEdit->setFixedWidth(30);

	UpdateCamOnlyCheckBox=new QCheckBox2(checkBoxList,AnimationBox);
		AnimationBox->layout()->addWidget(UpdateCamOnlyCheckBox);
	UpdateCamOnlyCheckBox->setText("Update Camera Only");
	UpdateCamOnlyCheckBox->setChecked(false);




	QWidget *AnimBox1a = new QWidget(AnimationBox); AnimBox1a->setLayout(new QHBoxLayout());
		AnimationBox->layout()->addWidget(AnimBox1a);
		GoToKeyFrameItemButton = new QPushButton2(buttonList,"Go To:", AnimBox1a); AnimBox1a->layout()->addWidget(GoToKeyFrameItemButton);
		NumberOfKeyFrameItemBox = new QSpinBox2(spinBoxList,AnimBox1a); NumberOfKeyFrameItemBox->setRange(1, 512); AnimBox1a->layout()->addWidget(NumberOfKeyFrameItemBox);
	
	NumberOfKeyFrameItemBox->setValue(1);



	QWidget *AnimBox2 = new QWidget(AnimationBox); AnimBox2->setLayout(new QHBoxLayout());
		AnimationBox->layout()->addWidget(AnimBox2);
		SetPreviousKeyFrameItemButton = new QPushButton2(buttonList,"Prev", AnimBox2); AnimBox2->layout()->addWidget(SetPreviousKeyFrameItemButton);
		SetNextKeyFrameItemButton = new QPushButton2(buttonList,"Next", AnimBox2); AnimBox2->layout()->addWidget(SetNextKeyFrameItemButton);




		QWidget *AnimBox3 = new QWidget(AnimationBox); AnimBox3->setLayout(new QHBoxLayout());
			AnimationBox->layout()->addWidget(AnimBox3);
			MoveUpKeyFrameItemButton = new QPushButton2(buttonList,"Move <-", AnimBox3); AnimBox3->layout()->addWidget(MoveUpKeyFrameItemButton);
			MoveDownKeyFrameItemButton = new QPushButton2(buttonList,"Move ->", AnimBox3); AnimBox3->layout()->addWidget(MoveDownKeyFrameItemButton);

			QWidget *AnimBox4 = new QWidget(AnimationBox); AnimBox4->setLayout(new QHBoxLayout());
			AnimationBox->layout()->addWidget(AnimBox4);
			InsertNewKeyFrameItemButton = new QPushButton2(buttonList,"Insert", AnimBox4); AnimBox4->layout()->addWidget(InsertNewKeyFrameItemButton);
			UpdateKeyFrameItemButton = new QPushButton2(buttonList,"Update Curr", AnimBox4);	 AnimBox4->layout()->addWidget(UpdateKeyFrameItemButton);


	QWidget *AnimBox4a = new QWidget(AnimationBox); AnimBox4a->setLayout(new QHBoxLayout());
			AnimationBox->layout()->addWidget(AnimBox4a);
			QLabel *StepsSeqL = new QLabel(" SubSteps: ", AnimBox4a); AnimBox4a->layout()->addWidget(StepsSeqL);

			NumberOfSequenceSubStepsBox = new QSpinBox2(spinBoxList,AnimBox4a); NumberOfSequenceSubStepsBox->setRange(1, 512); AnimBox4a->layout()->addWidget(NumberOfSequenceSubStepsBox);
	NumberOfSequenceSubStepsBox->setValue(30);


	
	updateAllSubstepsButton = new QPushButton2(buttonList, "UpdAll", AnimBox4a); AnimBox4a->layout()->addWidget(updateAllSubstepsButton);




	QWidget* tt;
	QLabel* l;


	tt=new   QWidget(AnimationBox); tt->setLayout(new QHBoxLayout());	AnimationBox->layout()->addWidget(tt);
	l = new QLabel("Text: ", tt); tt-> layout()->addWidget(l);
	animTxtLineEdit = new QLineEdit2(lineEditList, tt); tt->layout()->addWidget(animTxtLineEdit);

	tt = new   QWidget(AnimationBox); tt->setLayout(new QHBoxLayout());	AnimationBox->layout()->addWidget(tt);
	//l = new QLabel("pos: ", tt); tt->layout()->addWidget(l);

	QString ttt[] = { "x:","y:","size:", "step:" };
	for (int i = 0;i < 4;i++)
	{
		l = new QLabel(ttt[i], tt); tt->layout()->addWidget(l);
		animTextPosLineEdit[i] = new QLineEdit2(lineEditList, tt); tt->layout()->addWidget(animTextPosLineEdit[i]);animTextPosLineEdit[i]->setFixedWidth(30);
	}
	animTextPosLineEdit[0]->setText("0");
	animTextPosLineEdit[1]->setText("0");
	animTextPosLineEdit[2]->setText("10");

	tt = new   QWidget(AnimationBox); tt->setLayout(new QHBoxLayout());	AnimationBox->layout()->addWidget(tt);
	QString ttt1[] = { "R:","G:","B:" };
	for (int i = 0;i < 3;i++)
	{
		l = new QLabel(ttt1[i], tt); tt->layout()->addWidget(l);
		animTextColorLineEdit[i] = new QLineEdit2(lineEditList, tt); tt->layout()->addWidget(animTextColorLineEdit[i]);
		animTextColorLineEdit[i]->setText("0");
	}



	addSubtiutleLineButton = new QPushButton2(buttonList, "Add Subtitle line", AnimationBox);	AnimationBox->layout()->addWidget(addSubtiutleLineButton);
	subtitleTextEdit = new QPlainTextEdit(AnimationBox);AnimationBox->layout()->addWidget(subtitleTextEdit);

	QWidget *AnimBox5 = new QWidget(AnimationBox); AnimBox5->setLayout(new QHBoxLayout());
			AnimationBox->layout()->addWidget(AnimBox5);
			RemoveKeyFrameItemButton = new QPushButton2(buttonList,"Remove Curr", AnimBox5);	 AnimBox5->layout()->addWidget(RemoveKeyFrameItemButton);
			RemoveAllKeyFrameItemButton = new QPushButton2(buttonList,"Remove All", AnimBox5);	 AnimBox5->layout()->addWidget(RemoveAllKeyFrameItemButton);


			QWidget *AnimBox6 = new QWidget(AnimationBox); AnimBox6->setLayout(new QHBoxLayout());
			AnimationBox->layout()->addWidget(AnimBox6);
			GenerateAnimationButton = new QPushButton2(buttonList,"Animate", AnimBox6);	AnimBox6->layout()->addWidget(GenerateAnimationButton);
			GenerateAnimationFromCurrentKeyframeButton = new QPushButton2(buttonList, "AnimCurr", AnimBox6);	AnimBox6->layout()->addWidget(GenerateAnimationFromCurrentKeyframeButton);
			GrabAnimationButton = new QPushButton2(buttonList,"Grab Anim", AnimBox6);	AnimBox6->layout()->addWidget(GrabAnimationButton);

			QWidget *AnimBox7 = new QWidget(AnimationBox); AnimBox7->setLayout(new QHBoxLayout());
			AnimationBox->layout()->addWidget(AnimBox7);
			LoopAnimationCheckBox = new QCheckBox2(checkBoxList,AnimBox7); AnimBox7->layout()->addWidget(LoopAnimationCheckBox);
	LoopAnimationCheckBox->setText("Loop");
	LoopAnimationCheckBox->setChecked(false);
	smoothAnimationCheckBox = new QCheckBox2(checkBoxList,AnimBox7); AnimBox7->layout()->addWidget(smoothAnimationCheckBox);
	smoothAnimationCheckBox->setText("Smooth");
	smoothAnimationCheckBox->setChecked(true);
	AbortAnimationButton = new QPushButton2(buttonList,"Abort", AnimBox7);	AnimBox7->layout()->addWidget(AbortAnimationButton);

	AnimBox7 = new QWidget(AnimationBox); AnimBox7->setLayout(new QHBoxLayout());
	AnimationBox->layout()->addWidget(AnimBox7);


	
	loadAnimPushButton = new QPushButton2(buttonList,"Load", AnimBox7);	AnimBox7->layout()->addWidget(loadAnimPushButton);
	saveAnimPushButton = new QPushButton2(buttonList,"Save", AnimBox7);	AnimBox7->layout()->addWidget(saveAnimPushButton);


	//-----------------------

	QGroupBox* testBox = new QGroupBox(Gb1);
	Gb1->layout()->addWidget(testBox);
	testBox->setLayout(new QVBoxLayout);
	testBox->setTitle("Special Functions:");

	testBox->layout()->setSpacing(3);
	testBox->layout()->setMargin(5);



	functionForGenAnimComboBox = new QComboBox2(comboBoxList,testBox);	testBox->layout()->addWidget(functionForGenAnimComboBox);


	QWidget* testB1 = new QWidget(testBox); testB1->setLayout(new QHBoxLayout());	testBox->layout()->addWidget(testB1);

	QLabel* lab;
	for (int i = 0;i < 4;i++)
	{
		lab = new QLabel(QString::number(i)+":", testB1);testB1->layout()->addWidget(lab);
		functionForGenAnimLineEdit[i] = new QLineEdit2(lineEditList,"0", testB1);testB1->layout()->addWidget(functionForGenAnimLineEdit[i]);functionForGenAnimLineEdit[i]->setFixedWidth(30);
	}
	functionForGenAnimLineEdit[0]->setText("4");
	functionForGenAnimLineEdit[1]->setText("50");
	functionForGenAnimLineEdit[2]->setText("4");

	//--------------------------

	QGroupBox *AnimationBox2=new QGroupBox(Gb1);
	Gb1->layout()->addWidget(AnimationBox2);
	AnimationBox2->setLayout(new QVBoxLayout);
	AnimationBox2->setTitle("Visibiliy:");

	AnimationBox2->layout()->setSpacing(3);
	AnimationBox2->layout()->setMargin(5);


	QWidget *AnimBox8 = new QWidget(AnimationBox2); AnimBox8->setLayout(new QHBoxLayout());
			AnimationBox2->layout()->addWidget(AnimBox8);
			LoopVisibilityChechBox = new QCheckBox2(checkBoxList,AnimBox8);	AnimBox8->layout()->addWidget(LoopVisibilityChechBox);
	//LoopVisibilityChechBox->setText("LV");
	LoopVisibilityChechBox->setChecked(false);

//	QLabel *Labelxx1				=	new QLabel("Nr:",AnimBox8); //number of total loop surfaces

//	NrOfTotalLoopSurfacesBox = new QSpinBox2(spinBoxList,1,511,1,AnimBox8);
//	NrOfTotalLoopSurfacesBox->setMinValue(0);
//	NrOfTotalLoopSurfacesBox->setValue(0);

	QLabel *Labelxx2 = new QLabel("St:", AnimBox8); //number of frame-surfaces
	AnimBox8->layout()->addWidget(Labelxx2);
	NrOfFrameSurfacesBox = new QSpinBox2(spinBoxList,AnimBox8); NrOfFrameSurfacesBox->setRange(1, 512);	AnimBox8->layout()->addWidget(NrOfFrameSurfacesBox);
	
	NrOfFrameSurfacesBox->setValue(1);

//	QLabel *Labelxx3				=	new QLabel("FSt:",AnimBox8); //frame step
//	FrameStepNr = new QSpinBox2(spinBoxList,1,511,1,AnimBox8);
//	FrameStepNr->setMinValue(1);
//	FrameStepNr->setValue(1);

	//LoopAnimationCheckBox->setMinimumWidth(20);
//	NrOfTotalLoopSurfacesBox->setMinimumWidth(20);
//	NrOfFrameSurfacesBox->setMinimumWidth(20);
//	FrameStepNr->setMinimumWidth(20);
	//-----------------------------------------


	SetPreviousKeyFrameItemButton->setMinimumWidth(70);
	SetNextKeyFrameItemButton->setMinimumWidth(70);
	MoveUpKeyFrameItemButton->setMinimumWidth(70);
	MoveDownKeyFrameItemButton->setMinimumWidth(70);
	InsertNewKeyFrameItemButton->setMinimumWidth(70);
	UpdateKeyFrameItemButton->setMinimumWidth(70);
	RemoveKeyFrameItemButton->setMinimumWidth(70);
	RemoveAllKeyFrameItemButton->setMinimumWidth(70);
	GenerateAnimationButton->setMinimumWidth(70);
	GrabAnimationButton->setMinimumWidth(70);

	///////////////////////////////////////////////////////

	QWidget *ButtonBox2 = new QWidget(TransformBox); ButtonBox2->setLayout(new QHBoxLayout());
		TransformBox->layout()->addWidget(ButtonBox2);



		Window3DSizeCombo = new QComboBox2(comboBoxList,ButtonBox2);	ButtonBox2->layout()->addWidget(Window3DSizeCombo);



	//	Window3DSizeCombo->addAction("48  x  48",0);
	Window3DSizeCombo->addItem("128 x 128");
	Window3DSizeCombo->addItem("256 x 256");
	Window3DSizeCombo->addItem("384 x 384");
	Window3DSizeCombo->addItem("480 x 480");
	Window3DSizeCombo->addItem("512 x 512");
	Window3DSizeCombo->addItem("600  x 600");


	Window3DSizeCombo->setMinimumWidth(50);



	QGroupBox *LightBox = new QGroupBox(Gb1); LightBox->setLayout(new QHBoxLayout());
		Gb1->layout()->addWidget(LightBox);

	//LightBox->setLayout(new QVBoxLayout);
	LightBox->setTitle("Lights");

	LightBox->layout()->setSpacing(3);
	LightBox->layout()->setMargin(5);

	ShowHideLightsButton = new QPushButton2(buttonList,"Show/Hide Lights", LightBox);	LightBox->layout()->addWidget(ShowHideLightsButton);
	LightBox->layout()->addWidget(ShowHideLightsButton);
	QWidget *LightBox1 = new QWidget(LightBox); LightBox1->setLayout(new QHBoxLayout());
	LightBox->layout()->addWidget(LightBox1);
	QLabel *LightsLabel = new QLabel("Intensity:", LightBox1); LightBox1->layout()->addWidget(LightsLabel);

	LightIntensitySlider = new QSlider2(sliderList,Qt::Horizontal, LightBox1); LightBox1->layout()->addWidget(LightIntensitySlider);
	LightIntensitySlider->setRange(0,100);
	LightIntensitySlider->setValue(100);
	LightIntensityLabel = new QLabel("1.0", LightBox1); LightBox1->layout()->addWidget(LightIntensityLabel);
	LightIntensityLabel->setFixedWidth(25);
	connect(LightIntensitySlider,SIGNAL(valueChanged(int)),this,SLOT(SlotUpdateLightIntensityLabel(int)));


	//QLineEdit *explodeRestRotLineEdit;

	TextFieldDoubleValidator *scaleValidator =new TextFieldDoubleValidator(0,100000.0,15,this);
	scaleValidator->setNotation(TextFieldDoubleValidator::StandardNotation);
	
	LightBox = new QGroupBox(Gb1); LightBox->setLayout(new QVBoxLayout());
	Gb1->layout()->addWidget(LightBox);
	//LightBox->setLayout(new QVBoxLayout);
	LightBox->setTitle("Multi Transforms");
	LightBox->layout()->setSpacing(3);
	LightBox->layout()->setMargin(5);
	
	ButtonBox2 = new QWidget(LightBox); ButtonBox2->setLayout(new QHBoxLayout());
	LightBox->layout()->addWidget(ButtonBox2);
	
	explodeSliderScaleLineEdit = new QLineEdit2(lineEditList,ButtonBox2); ButtonBox2->layout()->addWidget(explodeSliderScaleLineEdit);
	explodeSliderScaleLineEdit->setText("1.0");
	explodeSliderScaleLineEdit->setFixedWidth(45);
	explodeSliderScaleLineEdit->setValidator(scaleValidator);
	
	explodeRestSlider = new QSlider2(sliderList,Qt::Horizontal, ButtonBox2); ButtonBox2->layout()->addWidget(explodeRestSlider);
	explodeRestSlider->setMinimum(0);
	explodeRestSlider->setMaximum(100);
	explodeRestSlider->setValue(0);

	ButtonBox2 = new QWidget(LightBox); ButtonBox2->setLayout(new QHBoxLayout());
	LightBox->layout()->addWidget(ButtonBox2);
	
	int j;
	for(j=0;j<3;j++)
	{
		explodeRestRotLineEdit[j] = new QLineEdit2(lineEditList,ButtonBox2); ButtonBox2->layout()->addWidget(explodeRestRotLineEdit[j]);
		explodeRestRotLineEdit[j]->setText("0.0");
		explodeRestRotLineEdit[j]->setFixedWidth(45);
		explodeRestRotLineEdit[j]->setValidator(scaleValidator);
	}


	//--------------------------------
	MainLayout->addWidget(Gb1);
	//qtab->addTab(Info,"&Animation");

}
//--------------------------------------------------------

void	GUIPanelWidget::SlotUpdateLightIntensityLabel(int value)
{

	LightIntensityLabel->setText(QString::number((double)value/100));

}

//--------------------------------------------------------

void	GUIPanelWidget::SlotFindSelectedDICOMPosition()
{															   

	//QListViewItem *Item=DICOMDirListView->currentIndex();
	//if (Item!=NULL)
	//{
	//	QListViewItem *StepItem=DICOMDirListView->firstChild();

	//	int pos=0;

	//	while(Item!=StepItem)
	//	{
	//		StepItem=StepItem->nextSibling();
	//		pos++;
	//	}

	//	emit SignalSelectedDICOMAtPos(pos);
	//}
}




//--------------------------------------------------
void GUIPanelWidget::SlotUpdateBBoxSizes(int empty)
{

	int i;
	double temp;
	QString tempString;
	for(i=0;i<4;i++)
	{
		if (BBoxSlider[i]->value()!=0)
		{


			if (i!=3)
				temp=BBoxParams[i]+((double)BBoxSlider[i]->value()*BBoxParams[3])/100.0;
			else
			{

				temp=BBoxParams[i]+(double)BBoxSlider[i]->value();


			}

			if (temp<0) temp=0;
			//tozmienie!


			tempString.setNum(temp);

			disconnect(BBoxEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateBBoxSizes(  const QString & )));
			BBoxEdit[i]->setText(tempString);		
			connect(BBoxEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateBBoxSizes(  const QString & )));


		}
	}

	double Ftemp[3];
	for(i=0;i<3;i++)
		Ftemp[i]=BBoxEdit[i]->text().toFloat();

	emit	SignalSendNewBBoxSize(Ftemp);

}


//----------------------------------------------


void GUIPanelWidget::SlotUpdateBBoxSizes()
{
	int i;
	for(i=0;i<4;i++)
	{
		if(BBoxSlider[i]->value()!=0)
		{
			disconnect(BBoxSlider[i],SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateBBoxSizes(int)));	
			BBoxSlider[i]->setValue(0);
			connect(BBoxSlider[i],SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateBBoxSizes(int)));

			BBoxParams[i]=BBoxEdit[i]->text().toFloat();

		}
	}

	emit	SignalSendNewBBoxSize(BBoxParams);
}
//----------------------------------------------


void GUIPanelWidget::SlotUpdateBBoxSizes( const QString &Empty)
{

	int i;
	for(i=0;i<4;i++)
		BBoxParams[i]=BBoxEdit[i]->text().toFloat();

	emit	SignalSendNewBBoxSize(BBoxParams);


}


//----------------------------------------------
void GUIPanelWidget::SlotSetNewBBoxSizes(double *Sizes)
{

	int i;
	QString nr;

	for(i=0;i<3;i++)
	{	
		nr.setNum(Sizes[i]);
		disconnect(BBoxEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateBBoxSizes(  const QString & )));

		BBoxEdit[i]->setText(nr);
		connect(BBoxEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateBBoxSizes(  const QString & )));

	}

	double Range=MAX3(Sizes[0],Sizes[1],Sizes[2]);

	nr.setNum(Range);

	BBoxEdit[3]->setText(nr);

}
//---------------------------------------------------
void GUIPanelWidget::SlotCreateFloatingWidgets()
{

	
	//------------------------------------

	//DeformableSegmentationWidget=new QWidget();
	//DeformableSegmentationWidget->setCaption("Deformable Segmentation Panel");

	//QVBoxLayout	*DeformableSegmentationLayout= new QVBoxLayout( DeformableSegmentationWidget, 10, -1);
	//QGroupBox	*DeformableSegmentationGroupBox=new QGroupBox(DeformableSegmentationWidget);
	//DeformableSegmentationGroupBox->setLayout(new QVBoxLayout);
	//DeformableSegmentationLayout->addWidget(DeformableSegmentationGroupBox);	
	//
	//QWidget *XXBox=new QWidget(DeformableSegmentationGroupBox);	
	//QValidator *ValidatorDouble =new TextFieldDoubleValidator(0,100.0,15,this);

	//QLabel *Shape1Label=new QLabel("Sh1: ",XXBox);
	//Shape1LineEdit=new QLineEdit2(lineEditList,XXBox);
	//Shape1LineEdit->setValidator(ValidatorDouble);


	//QLabel *Shape2Label=new QLabel("Sh2: ",XXBox);
	//Shape2LineEdit=new QLineEdit2(lineEditList,XXBox);
	//Shape2LineEdit->setValidator(ValidatorDouble);


	//QLabel *Intensity1Label=new QLabel("Int1: ",XXBox);
	//Intensity1LineEdit=new QLineEdit2(lineEditList,XXBox);
	//Intensity1LineEdit->setValidator(ValidatorDouble);


	//QLabel *Intensity2Label=new QLabel("Int2: ",XXBox);
	//Intensity2LineEdit=new QLineEdit2(lineEditList,XXBox);
	//Intensity2LineEdit->setValidator(ValidatorDouble);
	//


	//QWidget *XXBox2=new QWidget(DeformableSegmentationGroupBox);	

	//QLabel *DensityLabel=new QLabel("Grid Size:",XXBox2);
	//DensitySpinBox = new QSpinBox2(spinBoxList,4,512,1,XXBox2);
	//DensitySpinBox->setValue(30);

	//Shape1LineEdit->setMinimumWidth(50);
	//Shape2LineEdit->setMinimumWidth(50);
	//Intensity1LineEdit->setMinimumWidth(50);
	//Intensity2LineEdit->setMinimumWidth(50);


	//Shape1LineEdit->setText("0.1");
	//Shape2LineEdit->setText("1.0");	
	//Intensity1LineEdit->setText("0.5");
	//Intensity2LineEdit->setText("1.0");
	//ApplyDeformableSegmentationButton= new QPushButton2(buttonList,"Apply Segmentation", XXBox2);
	
	DeformableSegmentationWidget=new QWidget();
	DeformableSegmentationWidget->setWindowTitle("Deformable Segmentation Panel");

	//QVBoxLayout	*DeformableSegmentationLayout= new QVBoxLayout( DeformableSegmentationWidget, 10, -1);
	//Q3GroupBox	*DeformableSegmentationGroupBox=new Q3GroupBox(30,Vertical,DeformableSegmentationWidget);

	QVBoxLayout* DeformableSegmentationLayout= new QVBoxLayout( DeformableSegmentationWidget);//, 10, -1);
	DeformableSegmentationWidget->setLayout(DeformableSegmentationLayout);
	QGroupBox *DeformableSegmentationGroupBox=new	QGroupBox(DeformableSegmentationWidget);
	QVBoxLayout *vbl_DeformableSegmentationGroupBox = new QVBoxLayout;
	DeformableSegmentationGroupBox->setLayout(vbl_DeformableSegmentationGroupBox);	
	DeformableSegmentationLayout->addWidget(DeformableSegmentationGroupBox);




	//QWidget *XXBox=new QWidget(DeformableSegmentationGroupBox);
	QGroupBox *XXBox=new	QGroupBox(DeformableSegmentationGroupBox);
	QHBoxLayout *hbl_XXBox = new QHBoxLayout;
	XXBox->setLayout(hbl_XXBox);
	vbl_DeformableSegmentationGroupBox->addWidget(XXBox);
	TextFieldDoubleValidator *ValidatorDouble =new TextFieldDoubleValidator(0,100.0,15,this);
	ValidatorDouble->setNotation(TextFieldDoubleValidator::StandardNotation);
	QLabel *Shape1Label=new QLabel("Sh1: ",XXBox);
	Shape1LineEdit=new QLineEdit2(lineEditList,XXBox);
	Shape1LineEdit->setValidator(ValidatorDouble);


	QLabel *Shape2Label=new QLabel("Sh2: ",XXBox);
	Shape2LineEdit=new QLineEdit2(lineEditList,XXBox);
	Shape2LineEdit->setValidator(ValidatorDouble);


	QLabel *Intensity1Label=new QLabel("Int1: ",XXBox);
	Intensity1LineEdit=new QLineEdit2(lineEditList,XXBox);
	Intensity1LineEdit->setValidator(ValidatorDouble);


	QLabel *Intensity2Label=new QLabel("Int2: ",XXBox);
	Intensity2LineEdit=new QLineEdit2(lineEditList,XXBox);
	Intensity2LineEdit->setValidator(ValidatorDouble);


	hbl_XXBox->addWidget(Shape1Label);
	hbl_XXBox->addWidget(Shape1LineEdit);
	hbl_XXBox->addWidget(Shape2Label);
	hbl_XXBox->addWidget(Shape2LineEdit);
	hbl_XXBox->addWidget(Intensity1Label);
	hbl_XXBox->addWidget(Intensity1LineEdit);
	hbl_XXBox->addWidget(Intensity2Label);
	hbl_XXBox->addWidget(Intensity2LineEdit);


	//QWidget *XXBox2=new QWidget(DeformableSegmentationGroupBox);	
	QGroupBox *XXBox2=new	QGroupBox(DeformableSegmentationGroupBox);
	QHBoxLayout *hbl_XXBox2 = new QHBoxLayout;
	XXBox2->setLayout(hbl_XXBox2);
	vbl_DeformableSegmentationGroupBox->addWidget(XXBox2);

	QLabel *DensityLabel=new QLabel("Grid Size:",XXBox2);
	DensitySpinBox = new QSpinBox2(spinBoxList,XXBox2);
	DensitySpinBox->setRange(4,512);
	DensitySpinBox->setValue(30);

	hbl_XXBox2->addWidget(DensityLabel);
	hbl_XXBox2->addWidget(DensitySpinBox);

	Shape1LineEdit->setMinimumWidth(50);
	Shape2LineEdit->setMinimumWidth(50);
	Intensity1LineEdit->setMinimumWidth(50);
	Intensity2LineEdit->setMinimumWidth(50);


	Shape1LineEdit->setText("0.1");
	Shape2LineEdit->setText("1.0");	
	Intensity1LineEdit->setText("0.5");
	Intensity2LineEdit->setText("1.0");
	ApplyDeformableSegmentationButton= new QPushButton2(buttonList,"Apply Segmentation", XXBox2);

	hbl_XXBox2->addWidget(ApplyDeformableSegmentationButton);

}
//---------------------------------------------------
void GUIPanelWidget::SlotInsertNewSurfaceTreeItem(QString name, QString groupName, PolygonSurfaceData *dataXX,int mode)
{

	//znajdz grupe - jesli nie ma stworz, jesli pusta wrzuc do grupy default
	//int i=0;
	int nr=surfaceTreeWidget-> topLevelItemCount();
	QString tmpName;
	bool newGroup=true;
	int groupPos=0;
	if (!(QString::compare(groupName,QString(""))==0))
	{
		for(int i=0;i<nr;i++)
		{
			tmpName=surfaceTreeWidget->topLevelItem(i)->text(0);
			if ((QString::compare(groupName,tmpName)==0))
			{
				newGroup=false;
				groupPos=i;
				break;
			}

		}
	}
	else
	{
		//wstaw do default
		groupPos=0;
		newGroup=false;


	}
	if (newGroup==true)
	{
		QStringList ll;
	
		//ll.append("");
		ll.append(groupName);
		surfaceTreeWidget->insertTopLevelItem(nr,new QTreeWidgetItem(ll));
		groupPos=nr;
	}

	QTreeWidgetItem *parentItem=surfaceTreeWidget->topLevelItem(groupPos);
	QTreeWidgetItem *item=new QTreeWidgetItem(parentItem);
	//item->setFlags
	double opacity=dataXX->GetVProperty()->GetOpacity();
	item->setText(2,QString::number(opacity,'g',2));
	item->setText(0,name);
	double col[3];
	



	dataXX->GetVProperty()->GetColor(col);
	int sx=8;
	int sy=8;
	//QImage image;
	//image.create(sx,sy,8,256);
	QImage image(sx, sy, QImage::Format_Indexed8);
		//for (  i=0;i<255;i++)
	image.setColor(0,qRgb(255*col[0],255*col[1],255*col[2]));
	image.setColor(1,qRgb(0,0,0));

	int i,j;
	for (i=0;i<sx;i++)
		for (j=0;j<sy;j++)
		{
			if((i==0)||(i==sx-1)||(j==0)||(j==sy-1))*(image.scanLine(j)+i)=1;
			else*(image.scanLine(j)+i)=0;
		}	

	//QPixmap pixmap(image);
		QPixmap pixmap(sx, sy);
		//pixmap.fromImage(image);

		QPainter paint(&pixmap);
		paint.drawImage(0, 0, image);
	item->setIcon(1,QIcon(pixmap));


	//mode - 0 - after current
	//mode - 1 - end
	//mode - 2 - before current
	if (mode==0)
	{
		parentItem->addChild(item);
	}else
	if (mode==1)
	{
		parentItem->insertChild(parentItem->childCount(),item);
	}
	else if (mode==2)
	{
		parentItem->addChild(item); //do poprawioenia
	}
	

	dataXX->SetTreeItem(item);
		//surfaceTreeWidget
}
void GUIPanelWidget::SlotSetNewTreeItemColor(QTreeWidgetItem *item, double col[4])
{
	int sx=8;
	int sy=8;
	//QImage image;
	//image.create(sx,sy,8,256);
	QImage image(sx, sy, QImage::Format_Indexed8);
		//for (  i=0;i<255;i++)
	image.setColor(0,qRgb(255*col[0],255*col[1],255*col[2]));
	image.setColor(1,qRgb(0,0,0));

	int i,j;
	for (i=0;i<sx;i++)
		for (j=0;j<sy;j++)
		{
			if((i==0)||(i==sx-1)||(j==0)||(j==sy-1))*(image.scanLine(j)+i)=1;
			else*(image.scanLine(j)+i)=0;
		}	
		QPixmap pixmap(sx, sy);
		//pixmap.fromImage(image);

		QPainter paint(&pixmap);
		paint.drawImage(0, 0, image);
	
	item->setIcon(1,QIcon(pixmap));
	item->setText(2,QString::number(col[3],'g',2));
}
//---------------------------------------------------

//----------------------
void GUIPanelWidget::SlotSetSurfaceColorPresentation(QTreeWidgetItem *item, double r,double g, double b,double alpha)
{
	int sx=8;
	int sy=8;
	QImage Image;
	//Image.create(sx,sy,8,256);
	QImage image(sx, sy, QImage::Format_Indexed8);
	QPixmap pixmap;
		//for (  i=0;i<255;i++)
	Image.setColor(0,qRgb(255*r,255*g,255*b));
	Image.setColor(1,qRgb(0,0,0));

	int i,j;
	for (i=0;i<sx;i++)
		for (j=0;j<sy;j++)
		{
			if((i==0)||(i==sx-1)||(j==0)||(j==sy-1))*(Image.scanLine(j)+i)=1;
			else*(Image.scanLine(j)+i)=0;
		}

	pixmap.convertFromImage( Image);
	item->setIcon(1,QIcon(pixmap));
	item->setText(2,QString::number(alpha,'g',2));
}

void GUIPanelWidget::SlotSetSurfaceOpacityPresentation(QTreeWidgetItem *item, double alpha)
{
	
	item->setText(2,QString::number(alpha,'g',2));
}
//---------------
//
//void	GUIPanelWidget::SlotSetMixerItemMixerMode (QTreeWidgetItem *item, bool mode)
//{
//	
//	if (mode==false)//jest dzieckiem ma zostac parentem
//	{
//		item->setIcon(1,emptyIcon);
//	}
//	else
//	{
//	
//		item->setIcon(1,mixerIcons[4]);
//	
//
//	}
//		
//}

////-----------------------------
//void GUIPanelWidget::SlotSetMixerBlendOpacityPresentation(QTreeWidgetItem *item, double alpha)
//{
//	item->setText(4,QString::number(alpha,'g',2));
//}
//
/////--------------------------------
//void GUIPanelWidget::SlotSetMixerBlendModePresentation(QTreeWidgetItem *item, int mode)
//{
//	item->setText(3,QString::number(mode,'g',2));
//}
//----------------------------------
void GUIPanelWidget::SlotSetSurfaceColorPresentation(int pos, double r,double g, double b)
{
	
//QListViewItem *Item=->currentIndex();
	//	QListViewItem *item=SurfaceDataListView-> firstChild();

	//	int posx=0;

	//	while((item!=NULL)&&(posx!=pos))
	//	{
	//		item=item->nextSibling();
	//		posx++;
	//	}

	//	


	//int sx=8;
	//int sy=8;
	//QImage Image;
	//Image.create(sx,sy,8,256);
	//QPixmap pixmap;
	//	//for (  i=0;i<255;i++)
	//Image.setColor(0,qRgb(255*r,255*g,255*b));
	//Image.setColor(1,qRgb(0,0,0));

	//int i,j;
	//for (i=0;i<sx;i++)
	//	for (j=0;j<sy;j++)
	//	{
	//		if((i==0)||(i==sx-1)||(j==0)||(j==sy-1))*(Image.scanLine(j)+i)=1;
	//		else*(Image.scanLine(j)+i)=0;
	//	}

	//pixmap.convertFromImage( Image);
	//item->setPixmap(1,pixmap);
}
//---------------------
void GUIPanelWidget::CreateArteryGenerationTab(MyDockWidget *qtab)
{

	int sx=8;
	int sy=8;
	/*QImage Image;
	Image.create(sx,sy,8,256);*/
	QImage Image(sx, sy, QImage::Format_Indexed8);
	int r,g,b;
	int i,j;

		//for (  i=0;i<255;i++)
	r=255;g=0;b=0;
	Image.setColor(0,qRgb(r,g,b));
	for (i=0;i<sx;i++)	for (j=0;j<sy;j++)	*(Image.scanLine(j)+i)=0;
	pixmapRed.convertFromImage( Image);
	r=0;g=255;b=0;
	Image.setColor(0,qRgb(r,g,b));
	for (i=0;i<sx;i++)	for (j=0;j<sy;j++)	*(Image.scanLine(j)+i)=0;
	pixmapGreen.convertFromImage( Image);


	QWidget *Info = new QWidget(qtab);
	qtab->setScrollWidget(Info);
	QLayout* MainLayout = new QVBoxLayout();	Info->setLayout(MainLayout);
	loadLargestDicomSetButton=new QPushButton2(buttonList,"Load Largest Dicom Set in Dir",Info);
	MainLayout->addWidget(loadLargestDicomSetButton);
	QWidget* tmpBox;
	QLabel* tmpLabel;
	QWidget* gb0=new QWidget(Info);//30,Vertical,Info);
	gb0->setLayout(new QVBoxLayout);
	gb0->layout()->setSpacing(0);
	gb0->layout()->setMargin(1);
	MainLayout->addWidget(gb0);


	//QComboBox* MedDefsComboBox;
	tmpLabel = new QLabel("MedDefs:", gb0); gb0->layout()->addWidget(tmpLabel);
	MedDefsComboBox=new   QComboBox2(comboBoxList, gb0);
	gb0->layout()->addWidget(MedDefsComboBox);
	

	
	
	gb0 = new QWidget(Info);//30,Vertical,Info);
	gb0->setLayout(new QVBoxLayout);
	gb0->layout()->setSpacing(0);
	gb0->layout()->setMargin(1);
	MainLayout->addWidget(gb0);

	tmpBox = new QWidget(gb0);	tmpBox->setLayout(new QHBoxLayout());
	gb0->layout()->addWidget(tmpBox);
	//tmpBox->setSpacing(1);


	QGroupBox *gb01 = new QGroupBox(tmpBox); tmpBox->layout()->addWidget(gb01);
	gb01->setLayout(new QVBoxLayout);
	QGroupBox *gb1 = new QGroupBox(tmpBox); tmpBox->layout()->addWidget(gb1);
	gb1->setLayout(new QVBoxLayout);
	//QGroupBox *gb0x=new QGroupBox(30,Horizontal,Info);

//	gb0x->setFlat(true);


	
	gb1->layout()->setSpacing(0);
	gb1->layout()->setMargin(0);


	arteryStateButtonGroup=new QButtonGroup();
	arteryShowButtonGroup=new QButtonGroup();
	arteryClearButtonGroup=new	QButtonGroup();
	gb01->layout()->setMargin(0);
	gb01->layout()->setSpacing(0);
	gb01->setFlat(true);
	gb01->hide();
	for(i=0;i<10;i++)
	{
		tmpBox = new QWidget(gb01);	tmpBox->setLayout(new QHBoxLayout());
		gb01->layout()->addWidget(tmpBox);
		//tmpBox->setSpacing(0);
		showArteryObjectsButton[i] = new QPushButton2(buttonList,"Show", tmpBox); tmpBox->layout()->addWidget(showArteryObjectsButton[i]);
		showArteryObjectsButton[i]->setDisabled(true);
		arteryShowButtonGroup->addButton(showArteryObjectsButton[i],i);
		showArteryObjectsButton[i]->setFixedWidth(35);
	}
	//arteryShowButtonGroup->setFixedWidth(37);
	int taab[13]={1,1,0,0,1,1,0,0,1,1,0,1,1};
	for(i=0;i<13;i++)
	{
		tmpBox = new QWidget(gb1);	tmpBox->setLayout(new QHBoxLayout());
		gb1->layout()->addWidget(tmpBox);
		//t//mpBox->setSpacing(2);
		
		
		
		arteryStateLabels[i] = new QLabel(tmpBox); tmpBox->layout()->addWidget(arteryStateLabels[i]);
		arteryStateLabels[i]->setFixedWidth(10);
		arteryStateLabels[i]->setPixmap(pixmapRed);
		arterySetButtons[i] = new QPushButton2(buttonList,"Set", tmpBox); tmpBox->layout()->addWidget(arterySetButtons[i]);
		arterySetButtons[i]->setFixedWidth(20);
		arteryClearButtons[i] = new QPushButton2(buttonList,"Clr", tmpBox); tmpBox->layout()->addWidget(arteryClearButtons[i]);
		arteryClearButtons[i]->setFixedWidth(20);
		arteryStateButtonGroup->addButton(arterySetButtons[i],i);
		arteryClearButtonGroup->addButton(arteryClearButtons[i],i);
		arteryNameLabels[i] = new QLabel(tmpBox); tmpBox->layout()->addWidget(arteryNameLabels[i]);
		arteryNameLabels[i]->setMinimumWidth(50);
		if (taab[i]==0)tmpBox->hide();
		
	}

	arteryNameLabels[0]->setText("Data");
	arteryNameLabels[1]->setText("Data Mask 1");
	arteryNameLabels[2]->setText("Data Mask 2");
	arteryNameLabels[3]->setText("Data Mask 3");
	arteryNameLabels[4]->setText("Data Crop");
	arteryNameLabels[5]->setText("Crop Mask");
	arteryNameLabels[6]->setText("Aneurysm Mask");
	arteryNameLabels[7]->setText("Rest Mask");
	arteryNameLabels[8]->setText("TMP Surfaces");
	arteryNameLabels[9]->setText("Tube Surfaces");
	arteryNameLabels[10]->setText("Base Vectors");
	arteryNameLabels[11]->setText("Seed Point");
	arteryNameLabels[12]->setText("Ignore Points");

	QGroupBox* newGb = new QGroupBox(Info);
	seedsGroupBox = newGb;
	newGb->setLayout(new QVBoxLayout);
	newGb->layout()->setSpacing(1);
	newGb->layout()->setMargin(2);
	MainLayout->addWidget(newGb);
	QWidget* tWidget;
	tWidget = new QWidget(newGb); tWidget->setLayout(new QHBoxLayout());newGb->layout()->addWidget(tWidget);




	//tmpBox=new QWidget(gb0);gb0->layout()->addWidget(tmpBox);
	//tmpBox->setMinimumWidth(50);
	loadSeedsPushButton = new QPushButton2(buttonList, "Load Seeds", tWidget);
	tWidget->layout()->addWidget(loadSeedsPushButton);
	//	loadArteryDataButton->setFixedWidth(45);
	saveSeedsPushButton = new QPushButton2(buttonList, "Save Seeds", tWidget);
	tWidget->layout()->addWidget(saveSeedsPushButton);
	tWidget = new QWidget(newGb); tWidget->setLayout(new QHBoxLayout());newGb->layout()->addWidget(tWidget);
	//QGroupBox *gb2a = new QGroupBox(Info);
	//gb2a->setLayout(new QHBoxLayout);
	//QPushButton *addSeedPushButton;
	//QPushButton *clearSeedsPushButton;
	addSeedPushButton = new QPushButton2(buttonList, "add seed", tWidget);
	tWidget->layout()->addWidget(addSeedPushButton);
	clearSeedsPushButton = new QPushButton2(buttonList, "clear seeds", tWidget);
	tWidget->layout()->addWidget(clearSeedsPushButton);


	//QGroupBox *newGb = new QGroupBox(Info);
	//newGb->setLayout(new QVBoxLayout);
	//MainLayout->addWidget(newGb);
	////tmpBox=new QWidget(gb0);gb0->layout()->addWidget(tmpBox);
	////tmpBox->setMinimumWidth(50);
	//loadSeedsPushButton = new QPushButton2(buttonList,"Load Seeds", newGb);
	//newGb->layout()->addWidget(loadSeedsPushButton);
	////	loadArteryDataButton->setFixedWidth(45);
	//saveSeedsPushButton = new QPushButton2(buttonList,"Save Seeds", newGb);
	//newGb->layout()->addWidget(saveSeedsPushButton);

	//QGroupBox *gb2a = new QGroupBox(Info);
	//gb2a->setLayout(new QHBoxLayout);
	////QPushButton *addSeedPushButton;
	////QPushButton *clearSeedsPushButton;
	//addSeedPushButton = new QPushButton2(buttonList,"add seed", gb2a);
	//gb2a->layout()->addWidget(addSeedPushButton);
	//clearSeedsPushButton = new QPushButton2(buttonList,"clear seeds", gb2a);
	//gb2a->layout()->addWidget(clearSeedsPushButton);
	//MainLayout->addWidget(gb2a);
	//tmpBox=new QHBox(gb1);
	QGroupBox *gb2 = new QGroupBox(Info);
	gb2->setLayout(new QVBoxLayout);
	MainLayout->addWidget(gb2);
	//tmpBox=new QWidget(gb0);gb0->layout()->addWidget(tmpBox);
	//tmpBox->setMinimumWidth(50);
	loadArteryDataButton = new QPushButton2(buttonList,"Load State", gb2);
	gb2->layout()->addWidget(loadArteryDataButton);
//	loadArteryDataButton->setFixedWidth(45);
	saveArteryDataButton = new QPushButton2(buttonList,"Save State", gb2);
	gb2->layout()->addWidget(saveArteryDataButton);
//	saveArteryDataButton->setFixedWidth(45);
	

	gb2=new QGroupBox(Info);
	gb2->setLayout(new QVBoxLayout);
	gb2->setTitle("Base Vectors");
	
	gb2->layout()->setSpacing(1);
	gb2->layout()->setMargin(2);

	MainLayout->addWidget(gb2);
	//MainLayout->addWidget(gb0x);

	QWidget *bBox;
	QLabel *l1;
	bBox = new QWidget(gb2); bBox->setLayout(new QHBoxLayout());
	gb2->layout()->addWidget(bBox);


	setBaseButton = new QPushButton2(buttonList,"Set Base Vectors", bBox); bBox->layout()->addWidget(setBaseButton);
	resetBaseButton = new QPushButton2(buttonList,"Reset", bBox); bBox->layout()->addWidget(resetBaseButton);
	resetBaseButton->setMinimumWidth(20);
	
	bBox = new QWidget(gb2); bBox->setLayout(new QHBoxLayout());
	gb2->layout()->addWidget(bBox);
	l1 = new QLabel("Origin: ", bBox); bBox->layout()->addWidget(l1);
	v0Edit[0] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v0Edit[0]);
	v0Edit[1] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v0Edit[1]);
	v0Edit[2] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v0Edit[2]);
	v0Edit[0]->setText("0.0");
	v0Edit[1]->setText("0.0");
	v0Edit[2]->setText("0.0");

	bBox = new QWidget(gb2); bBox->setLayout(new QHBoxLayout());
	gb2->layout()->addWidget(bBox);
	l1 = new QLabel("V1: ", bBox); bBox->layout()->addWidget(l1);
	v1Edit[0] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v1Edit[0]);
	v1Edit[1] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v1Edit[1]);
	v1Edit[2] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v1Edit[2]);
	v1Edit[0]->setText("1.0");
	v1Edit[1]->setText("0.0");
	v1Edit[2]->setText("0.0");

	bBox = new QWidget(gb2); bBox->setLayout(new QHBoxLayout());
	gb2->layout()->addWidget(bBox);
	l1 = new QLabel("V2: ", bBox); bBox->layout()->addWidget(l1);
	v2Edit[0] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v2Edit[0]);
	v2Edit[1] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v2Edit[1]);
	v2Edit[2] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v2Edit[2]);
	v2Edit[0]->setText("0.0");
	v2Edit[1]->setText("1.0");
	v2Edit[2]->setText("0.0");

	bBox = new QWidget(gb2); bBox->setLayout(new QHBoxLayout());
	gb2->layout()->addWidget(bBox);
	l1 = new QLabel("V3: ", bBox); bBox->layout()->addWidget(l1);
	v3Edit[0] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v3Edit[0]);
	v3Edit[1] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v3Edit[1]);
	v3Edit[2] = new QLineEdit2(lineEditList,bBox); bBox->layout()->addWidget(v3Edit[2]);
	v3Edit[0]->setText("0.0");
	v3Edit[1]->setText("0.0");
	v3Edit[2]->setText("1.0");

	for(i=0;i<3;i++)
	{
		v0Edit[i]->setMinimumWidth(20);
		v1Edit[i]->setMinimumWidth(20);
		v2Edit[i]->setMinimumWidth(20);
		v3Edit[i]->setMinimumWidth(20);
	
	}




	bBox = new QWidget(gb2); bBox->setLayout(new QHBoxLayout());
	gb2->layout()->addWidget(bBox);
	//QPushButton *p4Button[4];
	//QPushButton *p4PlaneButton;

	p4Button[0]=new QPushButton2(buttonList,"LU",bBox);
	p4Button[1]=new QPushButton2(buttonList,"PU",bBox);	
	p4Button[2]=new QPushButton2(buttonList,"LO",bBox);	
	p4Button[3]=new QPushButton2(buttonList,"PO",bBox);	
	p4Button[0]->setMinimumWidth(10);
	p4Button[1]->setMinimumWidth(10);
	p4Button[2]->setMinimumWidth(10);
	p4Button[3]->setMinimumWidth(10);
	p4PlaneButton=new QPushButton2(buttonList,"!",bBox);
	p4PlaneButton->setMinimumWidth(10);
	bBox->hide();
	

	QGroupBox *gb3=new QGroupBox(Info);
	gb3->setLayout(new QVBoxLayout);
	
	gb3->layout()->setSpacing(1);
	gb3->layout()->setMargin(2);
	gb3->setTitle("Seed Point");
	MainLayout->addWidget(gb3);
	bBox = new QWidget(gb3); bBox->setLayout(new QHBoxLayout());
	gb3->layout()->addWidget(bBox);
	
	l1 = new QLabel(" ", bBox); bBox->layout()->addWidget(l1);
	seedLabel[0] = new QLabel(bBox); bBox->layout()->addWidget(seedLabel[0]);
	l1 = new QLabel(" ", bBox); bBox->layout()->addWidget(l1);
	seedLabel[1] = new QLabel(bBox); bBox->layout()->addWidget(seedLabel[1]);
	l1 = new QLabel(" ", bBox); bBox->layout()->addWidget(l1);
	seedLabel[2] = new QLabel(bBox); bBox->layout()->addWidget(seedLabel[2]);
	seedLabel[0]->setText("0.0");
	seedLabel[1]->setText("0.0");
	seedLabel[2]->setText("0.0");
	
	

	//
//	PaletteCombo = new QComboBox2(comboBoxList, Gb1PP, "read-only combo" );



	

	QGroupBox *gb5=new QGroupBox(Info);
	gb5->setLayout(new QVBoxLayout);
	
	gb5->layout()->setSpacing(1);
	gb5->layout()->setMargin(2);
	gb5->setTitle("TUBES");
	bBox = new QWidget(gb5); bBox->setLayout(new QHBoxLayout());
	gb5->layout()->addWidget(bBox);
	MainLayout->addWidget(gb5);
	generateFrontTreeButton = new QPushButton2(buttonList,"Tubes", bBox); bBox->layout()->addWidget(generateFrontTreeButton);
	generateFrontTreeButton->setFixedWidth(35);

	

		//seedPointZBox->setMaximumWidth(40);
	testSkeletonButton = new QPushButton2(buttonList,"Params", bBox);	bBox->layout()->addWidget(testSkeletonButton);
	testSkeletonButton->setMaximumWidth(40);

	savePointsToFileButton = new QPushButton2(buttonList,"SavePts", bBox);	bBox->layout()->addWidget(savePointsToFileButton);
	savePointsToFileButton->setMaximumWidth(40);
	//QPushButton *savePointsToFileButton;

	QGroupBox *Gb1PP=new	QGroupBox(Info);
	gb5->layout()->addWidget(Gb1PP);
	
	Gb1PP->setLayout(new QHBoxLayout);
	Gb1PP->layout()->setSpacing(1);
	Gb1PP->layout()->setMargin(3);
	Gb1PP->setTitle("Temp Surfaces");
	//bBox=new QWidget(gb5);gb5->layout()->addWidget(bBox);
	



	showTmpButton=new QPushButton2(buttonList,"Show",Gb1PP);Gb1PP->layout()->addWidget(	showTmpButton);
	showTmpButton->setMaximumWidth(40);
	hideTmpButton=new QPushButton2(buttonList,"Hide",Gb1PP);Gb1PP->layout()->addWidget(	hideTmpButton);	
	hideTmpButton->setMaximumWidth(40);
	deleteTmpButton=new QPushButton2(buttonList,"Delete",Gb1PP);	Gb1PP->layout()->addWidget(	deleteTmpButton);
	deleteTmpButton->setMaximumWidth(40);

	
	
	


	//MainLayout->addWidget(gb4);
	
	MainLayout->setSpacing(1);


	//qtab->addTab(Info,"&Artery Data");
}

void	GUIPanelWidget::SlotSetArteryButtonState(int nr,bool state)
{


	if (state==true)
	{
		arteryStateLabels[nr]->setPixmap(pixmapGreen);
		if (nr<10)	showArteryObjectsButton[nr]->setEnabled(true);
	}
	else  
	{
		arteryStateLabels[nr]->setPixmap(pixmapRed);
		if (nr<10) showArteryObjectsButton[nr]->setEnabled(false);

	}
}
//------------------------
void GUIPanelWidget::CreateArteryAnalyzeTab(MyDockWidget *qtab)
{

	/*QSpinBox *seedPointXBox,*seedPointYBox,*seedPointZBox;
	QSpinBox *cropSizeXBox,*cropSizeYBox,*cropSizeZBox;
	QSpinBox *maxSeedsBox,*maxDistBox,*maxPtsInOneBox,*firstThreshBox,*lastThreshBox;

	QPushButton *skeletonFromThreshButton,*skeletonFromMaskButton;
	QPushButton *getFirstLineButton,*getSecondLineButton;
	QLineEdit *aFirstLineEdit,*bFirstLineEdit,*cFirstLineEdit;
	QLineEdit *aSecondLineEdit,*bSecondLineEdit,*cSecondLineEdit;
	QPushButton *calculateLineAngleButton;
	QLineEdit *lineAngleEdit;
*/

	TextFieldDoubleValidator *Validator1 =new TextFieldDoubleValidator(-99999999.0,99999999,15,this);
	Validator1->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *Validator2 =new TextFieldDoubleValidator(0.0001,100.0,4,this);
	//	QValidator *ValidatorInt =new QIntValidator(0,512,this);

	QWidget *Info = new QWidget(qtab);
	qtab->setScrollWidget(Info);
	QLayout* MainLayout= new QVBoxLayout( );	
	Info->setLayout(MainLayout);

	QGroupBox *Gb1=new QGroupBox(Info);
	QGroupBox *Gb2=new QGroupBox(Info);
	Gb1->setLayout(new QVBoxLayout);
	Gb2->setLayout(new QVBoxLayout);
	//Gb1->setTitle("Artery Analyze");

	int ss=128;
	int ww=500;
	//int maxSeeds=50000;
	int maxDist=2000;
	//int maxPtsInOne=1000;
	int firstThresh=10;
	int lastThresh=2;
	int tumorIter=10;
	int skelSmoothIter=30;
	int skelWeight=10;
	

	QWidget *seedPospBox = new QWidget(Gb1); seedPospBox->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(seedPospBox);
	//	QLabel *DistStepL0=new QLabel("Spc:",DistStepBox);
	QLabel *l1 = new QLabel("S:", seedPospBox); seedPospBox->layout()->addWidget(l1);
	QLabel *DistStepL1 = new QLabel(" X", seedPospBox); seedPospBox->layout()->addWidget(DistStepL1);
	seedPointXBox = new QSpinBox2(spinBoxList,seedPospBox); seedPointXBox->setRange(1, 512); seedPospBox->layout()->addWidget(seedPointXBox);
	seedPointXBox->setValue(ss);
	seedPointXBox->setMaximumWidth(40);

	QLabel *DistStepL2 = new QLabel(" Y", seedPospBox); seedPospBox->layout()->addWidget(DistStepL2);
	seedPointYBox = new QSpinBox2(spinBoxList,seedPospBox); seedPointYBox->setRange(1, 512); seedPospBox->layout()->addWidget(seedPointYBox);
	seedPointYBox->setValue(ss);
	seedPointYBox->setMaximumWidth(40);

	QLabel *DistStepL3 = new QLabel("Z", seedPospBox); seedPospBox->layout()->addWidget(DistStepL3);
	seedPointZBox = new QSpinBox2(spinBoxList,seedPospBox); seedPointZBox->setRange(1, 512); seedPospBox->layout()->addWidget(seedPointZBox);
	seedPointZBox->setValue(ss);
	seedPointZBox->setMaximumWidth(40);


	
	QWidget *seedPospBox2 = new QWidget(Gb1);	seedPospBox2->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(seedPospBox2);
	//	QLabel *DistStepL0=new QLabel("Spc:",DistStepBox);
	l1 = new QLabel("W:", seedPospBox2); seedPospBox2->layout()->addWidget(l1);
	DistStepL1 = new QLabel(" X", seedPospBox2); seedPospBox2->layout()->addWidget(DistStepL1);
	cropSizeXBox = new QSpinBox2(spinBoxList,seedPospBox2); cropSizeXBox->setRange(1, 512); seedPospBox2->layout()->addWidget(cropSizeXBox);
	cropSizeXBox->setValue(ww);
	cropSizeXBox->setMaximumWidth(40);


	DistStepL2 = new QLabel(" Y", seedPospBox2); seedPospBox2->layout()->addWidget(DistStepL2);
	cropSizeYBox = new QSpinBox2(spinBoxList,seedPospBox2); cropSizeYBox->setRange(1, 512); seedPospBox2->layout()->addWidget(cropSizeYBox);
	cropSizeYBox->setValue(ww);
	cropSizeYBox->setMaximumWidth(40);

	DistStepL3 = new QLabel("Z", seedPospBox2); seedPospBox2->layout()->addWidget(DistStepL3);
	cropSizeZBox = new QSpinBox2(spinBoxList,seedPospBox2); cropSizeZBox->setRange(1, 512); seedPospBox2->layout()->addWidget(cropSizeZBox);
	cropSizeZBox->setValue(ww);
	cropSizeZBox->setMaximumWidth(40);



	//	QSpinBox *maxSeedsBox,*maxDistBox,*maxPtsInOneBox,*firstThreshBox,*lastBoxThresh;
	//	int maxSeeds=50000;
	//int maxDist=80;
	//int maxPtsInOne=1000;
	//int firstThresh=5;
	//int lastThresh=5;
	QWidget *b=new QWidget(Gb1);		
	Gb1->layout()->addWidget(b);
	//l1=new QLabel("Max Seeds: ",b);
	//maxSeedsBox = new QSpinBox2(spinBoxList,0,maxSeeds*10,1,b);
	//maxSeedsBox->setValue(maxSeeds);
	
	b = new QWidget(Gb1);		b->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("Max Dist: ", b); b->layout()->addWidget(l1);
	maxDistBox = new QSpinBox2(spinBoxList,b); maxDistBox->setRange(0, 10000); b->layout()->addWidget(maxDistBox);
	maxDistBox->setValue(maxDist);





	/*b=new QHBox(Gb1);		
	l1=new QLabel("MaxPts InOne: ",b);
	maxPtsInOneBox = new QSpinBox2(spinBoxList,0,maxPtsInOne*10,1,b);
	maxPtsInOneBox->setValue(maxPtsInOne);*/

	b = new QWidget(Gb1);		b->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("Min step1: ", b); b->layout()->addWidget(l1);
	firstThreshBox = new QSpinBox2(spinBoxList,b); firstThreshBox->setRange(0, 65535); b->layout()->addWidget(firstThreshBox);
	firstThreshBox->setValue(firstThresh);

	b = new QWidget(Gb1);		b->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("Min step2: ", b); b->layout()->addWidget(l1);
	lastThreshBox = new QSpinBox2(spinBoxList,b); lastThreshBox->setRange(0, 65535); b->layout()->addWidget(lastThreshBox);
	lastThreshBox->setValue(lastThresh);

		/*QPushButton *,*;
	QPushButton *,*;
	QLineEdit *,*bFirstLineEdit,*cFirstLineEdit;
	QLineEdit *aSecondLineEdit,*bSecondLineEdit,*cSecondLineEdit;
	QPushButton *calculateLineAngleButton;
	QLineEdit *lineAngleEdit;*/

	b = new QWidget(Gb1);		b->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(b);
	///l1=new QLabel("Min step2: ",b);
	l1 = new QLabel("TSeed: ", b); b->layout()->addWidget(l1);
	startTumorBox = new QCheckBox2(checkBoxList,b); b->layout()->addWidget(startTumorBox);
	//startTumorBox->setText("TumorSeed");
	startTumorBox->setChecked(false);

	b = new QWidget(Gb1);	b->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("MedianT: ", b); b->layout()->addWidget(l1);
	medianTumorBox = new QCheckBox2(checkBoxList,b); b->layout()->addWidget(medianTumorBox);
	//medianTumorBox->setText("MedianTumor");
	medianTumorBox->setChecked(false);
	l1 = new QLabel("Iter: ", b); b->layout()->addWidget(l1);
	tumorIterBox = new QSpinBox2(spinBoxList,b); tumorIterBox->setRange(0, 1000); b->layout()->addWidget(tumorIterBox);
	tumorIterBox->setValue(tumorIter);


	b = new QWidget(Gb1);	b->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("SmoothSkel: ", b); b->layout()->addWidget(l1);
	smoothSkeletonBox = new QCheckBox2(checkBoxList,b); b->layout()->addWidget(smoothSkeletonBox);
	//smoothSkeletonBox->setText("SmoothSkel");
	smoothSkeletonBox->setChecked(true);
	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("Iter: ", b); b->layout()->addWidget(l1);
	skelSmoothIterBox = new QSpinBox2(spinBoxList,b); skelSmoothIterBox->setRange(1, 1000); b->layout()->addWidget(skelSmoothIterBox);
	skelSmoothIterBox->setValue(skelSmoothIter);
	//skelSmoothIterBox->setFixedWidth(20);



	l1 = new QLabel("W: ", b); b->layout()->addWidget(l1);
	skelSmoothWeightBox = new QSpinBox2(spinBoxList,b); skelSmoothWeightBox->setRange(0, 1000); b->layout()->addWidget(skelSmoothWeightBox);
	skelSmoothWeightBox->setValue(skelWeight);
	//skelSmoothWeightBox->setFixedWidth(20);



//	QSpinBox *,,;
//	QCheckBox *,*,*;


	b = new QWidget(Gb1);	b->setLayout(new QHBoxLayout());
	Gb1->layout()->addWidget(b);
	generateSkeletonButton = new QPushButton2(buttonList,"Tree", b); b->layout()->addWidget(generateSkeletonButton);
	generateSkeletonButton->setFixedWidth(35);
	skeletonButtonGroup=new QButtonGroup();
	skeletonButtonGroup->addButton(generateSkeletonButton,0);
	skeletonButtonGroup->addButton(generateFrontTreeButton,1);
	cropDataButton=new QPushButton2(buttonList,"Crop",b);
		cropDataButton->setMaximumWidth(40);

		b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
		Gb1->layout()->addWidget(b);
	
		pButton[0] = new QPushButton2(buttonList,"P-1", b);	 b->layout()->addWidget(pButton[0]);
		pButton[1] = new QPushButton2(buttonList,"P", b); b->layout()->addWidget(pButton[1]);
		pButton[2] = new QPushButton2(buttonList,"P+1", b); b->layout()->addWidget(pButton[2]);

	pButton[0]->setMinimumWidth(20);
	pButton[1]->setMinimumWidth(20);
	pButton[2]->setMinimumWidth(20);

	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	getFirstLineButton = new QPushButton2(buttonList,"GetLine 1", b);	 b->layout()->addWidget(getFirstLineButton);
	getSecondLineButton = new QPushButton2(buttonList,"GetLine 2", b);	 b->layout()->addWidget(getSecondLineButton);

	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("A1: ", b); b->layout()->addWidget(l1);
	aFirstLineEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(aFirstLineEdit);
	aFirstLineEdit->setText("0.0");
	aFirstLineEdit->setValidator(Validator1);
	l1 = new QLabel("A2: ", b); b->layout()->addWidget(l1);
	aSecondLineEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(aSecondLineEdit);
	aSecondLineEdit->setText("0.0");
	aSecondLineEdit->setValidator(Validator1);

	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("B1: ", b); b->layout()->addWidget(l1);
	bFirstLineEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(bFirstLineEdit);
	bFirstLineEdit->setText("0.0");
	bFirstLineEdit->setValidator(Validator1);
	l1 = new QLabel("B2: ", b); b->layout()->addWidget(l1);
	bSecondLineEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(bSecondLineEdit);
	bSecondLineEdit->setText("0.0");
	bSecondLineEdit->setValidator(Validator1);

	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("C1: ", b); b->layout()->addWidget(l1);
	cFirstLineEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(cFirstLineEdit);
	cFirstLineEdit->setText("0.0");
	cFirstLineEdit->setValidator(Validator1);
	l1 = new QLabel("C2: ", b); b->layout()->addWidget(l1);
	cSecondLineEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(cSecondLineEdit);
	cSecondLineEdit->setText("0.0");
	cSecondLineEdit->setValidator(Validator1);


	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("AnglesL1: ", b); b->layout()->addWidget(l1);
	l1AnglesEdit[0] = new QLineEdit2(lineEditList,b); b->layout()->addWidget(l1AnglesEdit[0]);
	l1AnglesEdit[1] = new QLineEdit2(lineEditList,b); b->layout()->addWidget(l1AnglesEdit[1]);
	l1AnglesEdit[2] = new QLineEdit2(lineEditList,b); b->layout()->addWidget(l1AnglesEdit[2]);
	l1AnglesEdit[0]->setText("0.0");
	l1AnglesEdit[1]->setText("0.0");
	l1AnglesEdit[2]->setText("0.0");

	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("AnglesL2: ", b); b->layout()->addWidget(l1);
	l2AnglesEdit[0] = new QLineEdit2(lineEditList,b); b->layout()->addWidget(l2AnglesEdit[0]);
	l2AnglesEdit[1] = new QLineEdit2(lineEditList,b); b->layout()->addWidget(l2AnglesEdit[1]);
	l2AnglesEdit[2] = new QLineEdit2(lineEditList,b); b->layout()->addWidget(l2AnglesEdit[2]);
	l2AnglesEdit[0]->setText("0.0");
	l2AnglesEdit[1]->setText("0.0");
	l2AnglesEdit[2]->setText("0.0");


	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	calculateLineAngleButton = new QPushButton2(buttonList,"Get Angle", b);	 b->layout()->addWidget(calculateLineAngleButton);
	l1 = new QLabel("Anlge: ", b); b->layout()->addWidget(l1);
	lineAngleEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(lineAngleEdit);
	lineAngleEdit->setText("0.0");

	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	calculateParamsButton = new QPushButton2(buttonList,"Calculate Params", b);	 b->layout()->addWidget(calculateParamsButton);


	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); 
	Gb1->layout()->addWidget(b);
	l1 = new QLabel("Volume: ", b); b->layout()->addWidget(l1);
	meshVolumeEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(meshVolumeEdit);
	meshVolumeEdit->setText("0.0");
	l1 = new QLabel("Area: ", b); b->layout()->addWidget(l1);
	meshAreaEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(meshAreaEdit);
	meshAreaEdit->setText("0.0");


	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); Gb1->layout()->addWidget(b);
	
	l1 = new QLabel("Length: ", b); b->layout()->addWidget(l1);
	skelLengthEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(skelLengthEdit);
	skelLengthEdit->setText("0.0");
	l1 = new QLabel("AvgDi: ", b); b->layout()->addWidget(l1);
	avgDiEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(avgDiEdit);
	avgDiEdit->setText("0.0");

	b = new QWidget(Gb1); b->setLayout(new QHBoxLayout()); Gb1->layout()->addWidget(b);
	
	l1 = new QLabel("WK: ", b); b->layout()->addWidget(l1);
	skelWKEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(skelWKEdit);
	skelWKEdit->setText("0.0");
	l1 = new QLabel("WO: ", b); b->layout()->addWidget(l1);
	skelWOEdit = new QLineEdit2(lineEditList,b); b->layout()->addWidget(skelWOEdit);
	skelWOEdit->setText("0.0");




//	QLineEdit *meshVolumeEdit,*skelLengthEdit,*skelWKEdit,*skelWOEdit,*meshAreaEdit,*avgDiEdit;


	//QPushButton *pButton[3];
	//QLineEdit	*v1Edit[3],*v1yEdit[3],*v1zEdit[3];
	//
	//QLineEdit	*l1AnglesEdit[3], *l2AnglesEdit[3];





	//b=new QHBox(Gb2);


	//setBaseButton=new QPushButton2(buttonList,"Set Base Vectors",b);
	//resetBaseButton=new QPushButton2(buttonList,"Reset",b);
	//resetBaseButton->setMinimumWidth(20);
	//b=new QHBox(Gb2);
	//
	//pButton[0]=new QPushButton2(buttonList,"P-1",b);	
	//pButton[1]=new QPushButton2(buttonList,"P",b);
	//pButton[2]=new QPushButton2(buttonList,"P+1",b);

	//pButton[0]->setMinimumWidth(20);
	//pButton[1]->setMinimumWidth(20);
	//pButton[2]->setMinimumWidth(20);

	//b=new QHBox(Gb2);
	//l1=new QLabel("V1: ",b);
	//v1Edit[0]=new QLineEdit2(lineEditList,b);
	//v1Edit[1]=new QLineEdit2(lineEditList,b);
	//v1Edit[2]=new QLineEdit2(lineEditList,b);
	//v1Edit[0]->setText("1.0");
	//v1Edit[1]->setText("0.0");
	//v1Edit[2]->setText("0.0");

	//b=new QHBox(Gb2);
	//l1=new QLabel("V2: ",b);
	//v2Edit[0]=new QLineEdit2(lineEditList,b);
	//v2Edit[1]=new QLineEdit2(lineEditList,b);
	//v2Edit[2]=new QLineEdit2(lineEditList,b);
	//v2Edit[0]->setText("0.0");
	//v2Edit[1]->setText("1.0");
	//v2Edit[2]->setText("0.0");

	//b=new QHBox(Gb2);
	//l1=new QLabel("V3: ",b);
	//v3Edit[0]=new QLineEdit2(lineEditList,b);
	//v3Edit[1]=new QLineEdit2(lineEditList,b);
	//v3Edit[2]=new QLineEdit2(lineEditList,b);
	//v3Edit[0]->setText("0.0");
	//v3Edit[1]->setText("0.0");
	//v3Edit[2]->setText("1.0");



	//b=new QHBox(Gb2);
	////QPushButton *p4Button[4];
	////QPushButton *p4PlaneButton;

	//p4Button[0]=new QPushButton2(buttonList,"LU",b);
	//p4Button[1]=new QPushButton2(buttonList,"PU",b);	
	//p4Button[2]=new QPushButton2(buttonList,"LO",b);	
	//p4Button[3]=new QPushButton2(buttonList,"PO",b);	
	//p4Button[0]->setMinimumWidth(15);
	//p4Button[1]->setMinimumWidth(15);
	//p4Button[2]->setMinimumWidth(15);
	//p4Button[3]->setMinimumWidth(15);
	//p4PlaneButton=new QPushButton2(buttonList,"!",b);
	//p4PlaneButton->setMinimumWidth(15);

	int dest=40;
	double minArteryLength=4.0;

	bool tubeParams[7];
	tubeParams[0]=false;		//generate orig skelton
	tubeParams[1]=false;		//generate orig tubes
	tubeParams[2]=true;		//generate skeletons
	tubeParams[3]=true;		//generate tubes
	tubeParams[4]=true;	//optimize tubes
	tubeParams[5]=true;		//generate joints
	tubeParams[6]=false;		//generate front surfaces
	//genArteryOptionCheckBox[i]=new QCheckBox2(checkBoxList,b);	
	//minSkelLengthLineEdit->setValidator(validD);
	Gb1->layout()->setSpacing(3);
	Gb1->layout()->setMargin(5);
	Gb1->layout()->setSpacing(3);
	Gb1->layout()->setMargin(5);

	Gb2->layout()->setSpacing(3);
	Gb2->layout()->setMargin(5);
	Gb2->layout()->setSpacing(3);
	Gb2->layout()->setMargin(5);


	QString chNames[10]={"generate orig skelton","generate orig tubes","generate skeletons","generate tubes","optimize tubes","generate joints","generate front surfaces","tmp surfaces","improveLargeCenterlines","GenTubeGraphsOn"};  
	for(int i=0;i<10;i++)
	{
		b=new QWidget(Gb2);Gb2->layout()->addWidget(b);
		b->setLayout(new QHBoxLayout());
		b->layout()->setAlignment(Qt::AlignLeft);
		b->layout()->setSpacing(2);
		
		genArteryOptionCheckBox[i] = new QCheckBox2(checkBoxList,b); b->layout()->addWidget(genArteryOptionCheckBox[i]);
		genArteryOptionCheckBox[i]->setChecked(tubeParams[i]);
		l1 = new QLabel(chNames[i], b); b->layout()->addWidget(l1);

	}

	b=new QWidget(Gb2);	Gb2->layout()->addWidget(b);
	b->setLayout(new QHBoxLayout());
	b->layout()->setAlignment(Qt::AlignLeft);
	b->layout()->setSpacing(2);
		
	TextFieldDoubleValidator *validD =new TextFieldDoubleValidator(0.001,100,5,this);
	validD->setNotation(TextFieldDoubleValidator::StandardNotation);
	QValidator *validI =new QIntValidator(3,100,this);

	//genArteryOptionCheckBox[i]=new QCheckBox2(checkBoxList,b);
	l1 = new QLabel("MinSkelLen [mm]:", b);; b->layout()->addWidget(l1);
	minSkelLengthLineEdit = new QLineEdit2(lineEditList,QString::number(minArteryLength), b); b->layout()->addWidget(minSkelLengthLineEdit);
	minSkelLengthLineEdit->setValidator(validD);
	b=new QWidget(Gb2);	Gb2->layout()->addWidget(b);
	b->setLayout(new QHBoxLayout());
	b->layout()->setAlignment(Qt::AlignLeft);
	b->layout()->setSpacing(2);
	
	l1 = new QLabel("Tube Dest:", b); b->layout()->addWidget(l1);
	tubeDestLineEdit = new QLineEdit2(lineEditList,QString::number(dest), b); b->layout()->addWidget(tubeDestLineEdit);
	tubeDestLineEdit->setValidator(validI);




	



	
	
	

//	ElasticTransAmountEdit=new QLineEdit2(lineEditList,TransGridBox);
//	ElasticTransAmountEdit->setText("1.0");
//	ElasticTransAmountEdit->setValidator(Validatordd);
//
//	GenerateDistanceMapButton=new QPushButton2(buttonList,"DistanceMap",Gb1);	
//
//
//	int ss=128;
//
//	QHBox *DistStepBox=new QHBox(Gb1);	
//	//	QLabel *DistStepL0=new QLabel("Spc:",DistStepBox);
//
//	QLabel *DistStepL1=new QLabel(" X:",DistStepBox);
//	DistStepXBox = new QSpinBox2(spinBoxList,1,512,1,DistStepBox);
//	DistStepXBox->setValue(ss);
//	DistStepXBox->setMaximumWidth(40);
//
//	QLabel *DistStepL2=new QLabel(" Y:",DistStepBox);
//	DistStepYBox = new QSpinBox2(spinBoxList,1,512,1,DistStepBox);
//	DistStepYBox->setValue(ss);
//	DistStepYBox->setMaximumWidth(40);
//
//	QLabel *DistStepL3=new QLabel("Z:",DistStepBox);
//	DistStepZBox = new QSpinBox2(spinBoxList,1,512,1,DistStepBox);
//	DistStepZBox->setValue(ss);
//	DistStepZBox->setMaximumWidth(40);
//
//
//
//
//	ApplySurfaceMatchingButton=new QPushButton2(buttonList,"SurfaceMatching",Gb1);
//
//	QHBox *PercentBox=new QHBox(Gb1);		
//	QLabel *L1=new QLabel("Percent: ",PercentBox);
//	PointPercentSpinBox = new QSpinBox2(spinBoxList,0,100,1,PercentBox);
//	PointPercentSpinBox->setValue(100);
//
//
//	BoundingBoxPointsCheckBox = new QCheckBox2(checkBoxList,PercentBox);
//	BoundingBoxPointsCheckBox->setText("Only BB");
//	BoundingBoxPointsCheckBox->setChecked(false);
//
//
//
//
//	ApplySufaceMatchingGenerateGraphsButton=new QPushButton2(buttonList,"GenerateGraphs",Gb1);
//
//
//
//	ShowHide3DSectionButton=new QPushButton2(buttonList,"Show/Hide 3D Section",Gb1);
//
//
//
//	QVButtonGroup *GridCol = new QVButtonGroup(Gb1);
//	GridCol->setTitle("Grab Options");
//	QRadioButton *NoneGrabRadioButton = new QRadioButton(GridCol);
//	GrabAppOnRegRadioButton = new QRadioButton(GridCol);
//	Grab3DWindowOnRegRadioButton = new QRadioButton(GridCol);
//
//	NoneGrabRadioButton->setText("Don't Grab");
//	GrabAppOnRegRadioButton->setText("Grab Application");
//	Grab3DWindowOnRegRadioButton->setText("Grab 3D Window");
//
//	NoneGrabRadioButton->setChecked(true);
//
//
//	/////////////////////
//
//
//	QGroupBox *FFDGroupBox=new QGroupBox(30,Vertical,Info);
//	FFDGroupBox->setTitle("Elastic Registration");
//
//	QHBox *GridSizeBox=new QHBox(FFDGroupBox);	
//	//	QLabel *GridSizeL0=new QLabel("Spc:",GridSizeBox);
//
//	QLabel *GridSizeL1=new QLabel(" X:",GridSizeBox);
//	GridSizeXBox = new QSpinBox2(spinBoxList,2,512,1,GridSizeBox);
//	GridSizeXBox->setValue(4);
//	GridSizeXBox->setMaximumWidth(40);
//
//	QLabel *GridSizeL2=new QLabel(" Y:",GridSizeBox);
//	GridSizeYBox = new QSpinBox2(spinBoxList,2,512,1,GridSizeBox);
//	GridSizeYBox->setValue(4);
//	GridSizeYBox->setMaximumWidth(40);
//
//	QLabel *GridSizeL3=new QLabel("Z:",GridSizeBox);
//	GridSizeZBox = new QSpinBox2(spinBoxList,2,512,1,GridSizeBox);
//	GridSizeZBox->setValue(4);
//	GridSizeZBox->setMaximumWidth(40);
//
//	QHBox *SurfaceElsasticOptimBox=new QHBox(FFDGroupBox);
//	ApplyElasticSurfaceMatchingButton=new QPushButton2(buttonList,"Start!",SurfaceElsasticOptimBox);
//
//	QLabel *ElasticIterL34=new QLabel(" Iter:",SurfaceElsasticOptimBox);
//	SurfaceElasticIterBox = new QSpinBox2(spinBoxList,1,512,1,SurfaceElsasticOptimBox);
//	SurfaceElasticIterBox->setValue(1);
//
//	SurfaceElasticUpdateGridBox=new QCheckBox2(checkBoxList,SurfaceElsasticOptimBox);
//	SurfaceElasticUpdateGridBox->setText("Update");
//	SurfaceElasticUpdateGridBox->setChecked(false);
//
//	ApplyElasticSurfaceMatchingButton->setMinimumWidth(50);
////	SurfaceElasticUpdateGridBox->setMinimumWidth(40);
//	SurfaceElasticIterBox->setMaximumWidth(30);
//
//
//
//	
//	QHBox *GridMatchingBox=new QHBox(FFDGroupBox);
//	Create3DGridPolyDataButton=new QPushButton2(buttonList,"Create Grid",GridMatchingBox);
//
////	Create3DGridPolyDataButton->setMinimumWidth(50);
////	Clear3DGridPolyDataButton->setMinimumWidth(40);
//	QLabel *GridSizeL21=new QLabel(" Density:",GridMatchingBox);
//	GridDensityBox = new QSpinBox2(spinBoxList,1,512,1,GridMatchingBox);
//	GridDensityBox->setValue(5);
//	GridDensityBox->setMaximumWidth(40);
//
//
//	QHBox *LoadSaveTransGridBox=new QHBox(FFDGroupBox);
//	LoadGrid3DButton = new QPushButton2(buttonList,"Load Grid",LoadSaveTransGridBox);
//	SaveGrid3DButton = new QPushButton2(buttonList,"Save Grid",LoadSaveTransGridBox);
//	Clear3DGridPolyDataButton=new QPushButton2(buttonList,"Clear",LoadSaveTransGridBox);
//	Clear3DGridPolyDataButton->setMinimumWidth(40);
//	LoadGrid3DButton->setMinimumWidth(60);
//	SaveGrid3DButton->setMinimumWidth(60);
//
//	QHBox *TransGridBox=new QHBox(FFDGroupBox);
//
//
//	ElasticTransformationButton = new QPushButton2(buttonList,"Transform",TransGridBox);
//
//	QLabel *Lxx=new QLabel(" Amount:",TransGridBox);
//	QValidator *Validatordd =new TextFieldDoubleValidator(0.001,1.0,20,this);
//	ElasticTransAmountEdit=new QLineEdit2(lineEditList,TransGridBox);
//	ElasticTransAmountEdit->setText("1.0");
//	ElasticTransAmountEdit->setValidator(Validatordd);
//
//	QHBox *sthBox=new QHBox(FFDGroupBox);
//	ReverseGridButton = new QPushButton2(buttonList,"Reverse Grid",sthBox);
//	GenerateTestGridButton = new QPushButton2(buttonList,"Test",sthBox);
//
//	QHBox *sth2Box=new QHBox(FFDGroupBox);
//	ScaleGridButton = new QPushButton2(buttonList,"Scale Grid",sth2Box);
//	//////////////////////////////////////////////////////////////////////////////
//
/////-----------------
//	ShowBoundingBoxParamsButton=new  QPushButton2(buttonList,"Bounding Box Params",Gb1);
//
//	BoundingBoxParamsWidget=new QWidget();
//	BoundingBoxParamsWidget->setName("Bounding Box Params Panel");
//	connect(ShowBoundingBoxParamsButton,SIGNAL(clicked()),BoundingBoxParamsWidget,SLOT(show()));
//
//	QVBoxLayout* BoundingBoxParamsLayout= new QVBoxLayout( BoundingBoxParamsWidget, 10, -1);
//	QGroupBox *BoundingBoxParamsGroupBox=new QGroupBox(30,Vertical,BoundingBoxParamsWidget);
//	BoundingBoxParamsLayout->addWidget(BoundingBoxParamsGroupBox);	
//	
//	
//	//---------------------
//
//	
//
//	QValidator *Validator =new TextFieldDoubleValidator(0.0000001,9999999,20,this);
//
//
//
//	QGroupBox *TransformGroupBox=new QGroupBox(30,Vertical,BoundingBoxParamsGroupBox);
//	TransformGroupBox->setTitle("Bounding Box Sizes");
//	//	TransformLayout->addWidget(TransformGroupBox);
//
//	QLabel		*BBoxLabel[4];
//
//
//
//	QHBox* OutsideBox[4];
//	QHBox	*Box[4]; 
//
//	int i;
//	QString NrText="0.0";
//
//	for(i=0;i<4;i++)
//	{
//
//		OutsideBox[i]=new QHBox(TransformGroupBox);
//		Box[i]	=new QHBox(OutsideBox[i]);
//		Box[i]->setSpacing(5);
//
//		BBoxLabel[i]=new QLabel(Box[i]);
//		BBoxEdit[i]=new QLineEdit2(lineEditList,Box[i]);
//		BBoxEdit[i]->setValidator(Validator);
//		BBoxEdit[i]->setMaximumWidth(50);
//
//
//		if (i==9) NrText="1.0";
//		BBoxEdit[i]->setText(NrText);
//
//		BBoxSlider[i]=new QSlider2(sliderList,-100,100,1,0,Qt::Horizontal, Box[i]);
//
//		if (i<12) connect(TCheckBox[i],SIGNAL(toggled(bool)),Box[i],SLOT(setEnabled (bool)));
//
//	}
//
//	BBoxLabel[0]->setText("Bx:");
//	BBoxLabel[1]->setText("By:");
//	BBoxLabel[2]->setText("Bz:");
//	BBoxLabel[3]->setText("Range:");
//	BBoxEdit[3]->setText("10.0");
//
//	for(i=0;i<3;i++)
//		BBoxParams[i]=BBoxEdit[i]->text().toFloat();
//	////////////////////////////////////////////////////////////////////////////
//
//	for(i=0;i<4;i++)
//	{
//		connect(BBoxSlider[i],SIGNAL(valueChanged (int)),this,SLOT(SlotUpdateBBoxSizes(int)));
//		connect(BBoxSlider[i],SIGNAL(sliderReleased ()),this,SLOT(SlotUpdateBBoxSizes()));
//		connect(BBoxEdit[i],SIGNAL( textChanged ( const QString & )),this,SLOT(SlotUpdateBBoxSizes(const QString &)));
//
//	}


	MainLayout->addWidget(Gb1);
	MainLayout->addWidget(Gb2);

//	MainLayout->addWidget(FFDGroupBox);
//	MainLayout->addWidget(ShowBoundingBoxParamsButton);

	//qtab->addTab(Info,"&Artery Analyze");

}
//--------
int GUIPanelWidget::findMyWidgetPos(MyDockWidget *w)
{
	int i;
	bool bad=false;


	int found=-1;
	for(i=0;i< NUMBER_OF_DOCKWIDGETS;i++)
	{
		if (GUIWidgets[i]==w)
		{
			found=i;
			break;
		}
	}
	return found;

}
//-----


void GUIPanelWidget::slotChangeDockWidgetVisibility(QString name)
{
		
	int i;
	bool bad=false;
	int max= NUMBER_OF_DOCKWIDGETS;
	for(i=0;i< NUMBER_OF_DOCKWIDGETS;i++)
	{
		if (GUIWidgets[i]==NULL)
		{
			bad=true;
			max=i;
			break;
			
		}
	}



	
	int pos=-1;
	for(i=0;i<max;i++)
	{
		if (name==GUIWidgets[i]->objectName())
		{
			pos=i;
			break;
		}
	}

	MyDockWidget *actWidget=GUIWidgets[pos];
	//wiem ktory klinkalem
	//teraz sprawdzam czy jest jakis lewy albo prawy - jesli nul to sprawdze czy ktorys ma flage left||right i jest visible - jesli tak to go ustawie na last
	QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent());
	const Qt::DockWidgetArea lArea=Qt::LeftDockWidgetArea;
	const Qt::DockWidgetArea rArea=Qt::RightDockWidgetArea;
	 Qt::DockWidgetArea actArea = mainWindow->dockWidgetArea(actWidget);

	int posCurrent=0;
	int posPrevious=0;

	if (actArea==lArea)
	{
		posCurrent=0;
		posPrevious=1;
	}
	else if(actArea=rArea)
	{
		posCurrent=2;
		posPrevious=3;
	}
	else return;//niezadokowany

	

	//	//lastRightDockWidget=NULL;
	////lastLeftDockWidget=NULL;
	
	//sprawdzam czy docka nie przerzucilem na druga strone - last

	if (widgetHistory[posPrevious]!=NULL)
	{
		if ((mainWindow->dockWidgetArea(widgetHistory[posPrevious])!=actArea)) widgetHistory[posPrevious]=NULL;
	}
	
	//czyli jesli przerzucilem na druga strone dock widget i byl on zaznaczony to go czyszcze i jade dalej


	//jesli prev jest null (na poczatku lub po rpzerzuceniu) to sprawdzam czy jest jakis inny prawdziwy i ustalam
	if (widgetHistory[posPrevious]==NULL)
	{
		for(i=0;i<max;i++)
		{
			if ((mainWindow->dockWidgetArea(GUIWidgets[i])==actArea)&&(GUIWidgets[i]->isContentVisible()==true))
			{
				widgetHistory[posPrevious]=GUIWidgets[i];
				widgetHistory[posCurrent]=GUIWidgets[i];
				break;
			}
		}
	}
	//jesli nic nie jest zapalone - TODO






	//jesli l jest nadal null to oznacza ze nie ma zadnego po danej stronie

	//jezeli kliknalem cos co jest widoczne znaczy trzeba schowac i pokazac stary (jesli jest) - jak nie nic nie zmieniam


	bool vis=actWidget->isContentVisible();


	if ((vis==true)) 
	{
		if(actWidget==widgetHistory[posPrevious])
		{
			//do nothing
		}
		else if (actWidget==widgetHistory[posCurrent])
		{
			//switch current and previous
			MyDockWidget *tmpW;
			tmpW=widgetHistory[posPrevious];
			widgetHistory[posPrevious]=widgetHistory[posCurrent];
			widgetHistory[posCurrent]=tmpW;
			
		}
		else
		{
			//not sure
			widgetHistory[posCurrent]=actWidget;
			widgetHistory[posPrevious]=actWidget;
		}


	}
	else
	{
		if(actWidget==widgetHistory[posPrevious])
		{
			//switch current and previous
			MyDockWidget *tmpW;
			tmpW=widgetHistory[posPrevious];
			widgetHistory[posPrevious]=widgetHistory[posCurrent];
			widgetHistory[posCurrent]=tmpW;
		}
		else //(act!previous)
		{
			widgetHistory[posPrevious]=widgetHistory[posCurrent];
			widgetHistory[posCurrent]=actWidget;
		
		}

	}
	
	
	//if (vis==false)
	{
		//zmieniam visibility wszystkich z danej strony i zostawiam tylko act -

	

		for(i=0;i<max;i++)
		{
			if (mainWindow->dockWidgetArea(GUIWidgets[i])==actArea)
			{
				this->GUIWidgets[i]->hide();
			}
		}
		for(i=0;i<max;i++)
		{
			if (mainWindow->dockWidgetArea(GUIWidgets[i])==actArea)
			{
				if (GUIWidgets[i]!=widgetHistory[posCurrent])
					GUIWidgets[i]->setContentVisible(false);
				else
					GUIWidgets[i]->setContentVisible(true);
			}
		}
		
		for(i=0;i<max;i++)
		{
			if (mainWindow->dockWidgetArea(GUIWidgets[i])==actArea)
			{
				GUIWidgets[i]->show();
			}
		}

	
		
	}

	
	

}
//----------------------------
void	GUIPanelWidget::slotDockLocationChanged(Qt::DockWidgetArea,QString name)
{

	int i;
	bool bad=false;
	int max= NUMBER_OF_DOCKWIDGETS;
	for(i=0;i< NUMBER_OF_DOCKWIDGETS;i++)
	{
		if (GUIWidgets[i]==NULL)
		{
			bad=true;
			max=i;
			break;
		
		}
	}



	
	int pos=-1;
	for(i=0;i<max;i++)
	{
		if (name==GUIWidgets[i]->objectName())
		{
			pos=i;
			break;
		}
	}


	MyDockWidget *actWidget=GUIWidgets[pos];
	//wiem ktory klinkalem
	//teraz sprawdzam czy jest jakis lewy albo prawy - jesli nul to sprawdze czy ktorys ma flage left||right i jest visible - jesli tak to go ustawie na last
	QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parent());
	const Qt::DockWidgetArea lArea=Qt::LeftDockWidgetArea;
	const Qt::DockWidgetArea rArea=Qt::RightDockWidgetArea;
	 Qt::DockWidgetArea actArea = mainWindow->dockWidgetArea(actWidget);

	int posCurrent=0;
	int posPrevious=0;
	if (actArea==lArea)
	{
		posCurrent=0;
		posPrevious=1;
	}
	else if(actArea=rArea)
	{
		posCurrent=2;
		posPrevious=3;
	}
	else return;//niezadokowany

	 //trzeba rozwazyc przypadki-

	 //jezeli jest zminimalizowany - sprawdzam tylko aktualna strefe
	 int nrOfItemsInCurrentArea=0;
	 for(i=0;i<max;i++)
	 {
		 if ( mainWindow->dockWidgetArea(GUIWidgets[i])==actArea) nrOfItemsInCurrentArea++;
	 }

	 if (actWidget->isContentVisible()==false)
	 {
	 
		//jezeli jest juz jakis w aktualnej strefie to zachowuje sie tak jakbym kliknal ten przycisk  slotChangeDockWidgetVisibility(name) 
		 //zmiana jednak nic nie zrobie - wrzuce zminimalizowan

		 if (nrOfItemsInCurrentArea>1)
		 {
			//slotChangeDockWidgetVisibility(name);
		 }
		 else
		 {
			widgetHistory[posPrevious]=NULL;
			widgetHistory[posCurrent]=NULL;


			actWidget->setContentVisible(true);
		//jezeli jest on jedyny to trzeba go zmaksymalizowac i ustawic prev i curr na null

		 }
	 }
	 else
	 {
		//jezeli jests zmaksymalizowany - sprawdzam obie

		//w aktualnej strefie - moze zminimalizuje i dopale klikniecie? slotChangeDockWidgetVisibility(name)
		
		 actWidget->setContentVisible(false);
		 	qApp->processEvents();
		 slotChangeDockWidgetVisibility(name);
		 //nie wiem co zrobic zeby zwinelo poprzednika
		


		
		
		 //slotChangeDockWidgetVisibility(name);
		 


		// w strefie przeciwnej - musze odpalic klikniecie dla lastclicked // niestety jak  narazie udalo mi sie to zrobic tylko na ostatnim bo inaczej gui syfi
		if (actArea==rArea)
		{
			posCurrent=0;
			posPrevious=1;
			actArea=lArea;
		}
		else if(actArea=lArea)
		{
			posCurrent=2;
			posPrevious=3;
			actArea=rArea;
		}
		actWidget=widgetHistory[posPrevious];
		if (actWidget==NULL)
		{
			for(i=max-1;i>=0;i--)
			{
				if(mainWindow->dockWidgetArea(GUIWidgets[i])==actArea)
				{
					actWidget=GUIWidgets[i];
					break;
				}

			}
		}
		
		if (actWidget!=NULL)
		{
			//// actWidget->setContentVisible(true);
			widgetHistory[posPrevious]=NULL;
			widgetHistory[posCurrent]=NULL;
			slotChangeDockWidgetVisibility(actWidget->objectName());
			
			
		}

		 
	}


}
void	GUIPanelWidget::SlotSurfaceTreeWidgetDoubleClicked( QTreeWidgetItem * item, int column )
{

	if ((item!=NULL))//&&(column==0))
	{

		int childNr=item->childCount();
		
		for(int  i=0;i<childNr;i++)
		{
			item->child(i)->setSelected(true);
		}
	}
}

void	GUIPanelWidget::SlotUpdateTreeWidgetState( QTreeWidgetItem *item, int column )
{
	//sprawdzam czy ma dzieci,
	//jesli jest zaznaczony zaznacz wszystkie
	//jesli jest odznaczony odznacz wszystkie
	/*int cc=item->childCount();
	{
		if (cc>0)
		{
			bool sel=item->isSelected();
			surfaceTreeWidget->expandItem(item);
			
			for(int i=0;i<cc;i++)
			{
				item->child(i)->setSelected(i);
			}
		}
	}*/
}


void	GUIPanelWidget::SlotParallelSliderValueChanged(int val)
{
	parallelThreadLabel->setText(" "+QString::number(val));
}


void	GUIPanelWidget::SlotSetMixerItemMixerMode(QTreeWidgetItem *item, bool mode)
{

	if (mode == false)//jest dzieckiem ma zostac parentem
	{
		item->setIcon(2, emptyIcon);
	}
	else
	{

		item->setIcon(2, mixerIcons[4]);


	}

}

//-----------------------------
void GUIPanelWidget::SlotSetMixerBlendOpacityPresentation(QTreeWidgetItem *item, double alpha)
{
	item->setText(5, QString::number(alpha, 'g', 2));
}

///--------------------------------
void GUIPanelWidget::SlotSetMixerBlendModePresentation(QTreeWidgetItem *item, int mode)
{
	item->setText(4, QString::number(mode, 'g', 2));
}
//----------------------------------

//------------------------

//--------
void	GUIPanelWidget::CreateFastFiltersTab(MyDockWidget *qtab)
{
	QWidget *Info = new QWidget(qtab);

	qtab->setScrollWidget(Info);
	Info->setLayout(new QVBoxLayout());
	Info->layout()->setSpacing(1);
	Info->layout()->setMargin(1);

	QSplitter *splitter = new QSplitter(Qt::Vertical, Info);
	splitter->setChildrenCollapsible(false);
	splitter->setHandleWidth(3);






	QWidget *xWidget;
	QWidget *xWidget1;
	QLabel *xLabel;

	QWidget *w;
	QWidget *b;
	QLabel *label;
	//-------------------------------------------------
	

	partitionSkeletonWidget = new QFrame(Info);
	partitionSkeletonWidget->setLayout(new QVBoxLayout);
	partitionSkeletonWidget->layout()->setSpacing(1);
	partitionSkeletonWidget->layout()->setMargin(1);
	Info->layout()->addWidget(partitionSkeletonWidget);



	partitionSkeletonButton = new QPushButton2(buttonList,"PartitionSkeleton", partitionSkeletonWidget); partitionSkeletonWidget->layout()->addWidget(partitionSkeletonButton);
	xWidget = new QWidget(partitionSkeletonWidget);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	partitionSkeletonWidget->layout()->addWidget(xWidget);


	

	label = new QLabel("prRadius: ",xWidget); xWidget->layout()->addWidget(label);
	newTubeParams[0] = new QLineEdit2(lineEditList,"1.0", xWidget); xWidget->layout()->addWidget(newTubeParams[0]);
	label = new QLabel("thinningFactor: ", xWidget); xWidget->layout()->addWidget(label);
	newTubeParams[1] = new QLineEdit2(lineEditList,"1.0", xWidget); xWidget->layout()->addWidget(newTubeParams[1]);


	xWidget = new QWidget(partitionSkeletonWidget);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	partitionSkeletonWidget->layout()->addWidget(xWidget);


	label = new QLabel("radiusThresh: ", xWidget); xWidget->layout()->addWidget(label);
	newTubeParams[2] = new QLineEdit2(lineEditList,"0.0", xWidget); xWidget->layout()->addWidget(newTubeParams[2]);
	label = new QLabel("collisionDist(<1): ", xWidget); xWidget->layout()->addWidget(label);
	newTubeParams[3] = new QLineEdit2(lineEditList,"1.0", xWidget); xWidget->layout()->addWidget(newTubeParams[3]);






	xWidget = new QWidget(partitionSkeletonWidget);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	partitionSkeletonWidget->layout()->addWidget(xWidget);


	label = new QLabel("minSkelFree: ", xWidget); xWidget->layout()->addWidget(label);
	newTubeParams[4] = new QLineEdit2(lineEditList,"4.0", xWidget); xWidget->layout()->addWidget(newTubeParams[4]);
	label = new QLabel("minSkelInner: ", xWidget); xWidget->layout()->addWidget(label);
	newTubeParams[5] = new QLineEdit2(lineEditList,"4.0", xWidget); xWidget->layout()->addWidget(newTubeParams[5]);





	xWidget = new QWidget(partitionSkeletonWidget);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	partitionSkeletonWidget->layout()->addWidget(xWidget);


	label = new QLabel("minSkelLoop: ", xWidget); xWidget->layout()->addWidget(label);
	newTubeParams[6] = new QLineEdit2(lineEditList,"4.0", xWidget); xWidget->layout()->addWidget(newTubeParams[6]);
	label = new QLabel("RadAvgMedFactor ", xWidget); xWidget->layout()->addWidget(label);
	newTubeParams[7] = new QLineEdit2(lineEditList,"4.0", xWidget); xWidget->layout()->addWidget(newTubeParams[7]);



	xWidget = new QWidget(partitionSkeletonWidget);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	partitionSkeletonWidget->layout()->addWidget(xWidget);

	//QCheckBox2* veinRGBCheckBox;
	//QLineEdit2* veinRGBLineEdits[3];


	veinRGBCheckBox = new QCheckBox2(checkBoxList, xWidget, "veinRGBCheckBox");
	veinRGBCheckBox->setText("colForTubes");
	veinRGBCheckBox->setChecked(false);
	xWidget->layout()->addWidget(veinRGBCheckBox);
	QString c[3] = { "R:","G:","B:" };
	for (int i = 0;i < 3;i++)
	{
		label = new QLabel(c[i], xWidget); xWidget->layout()->addWidget(label);
		veinRGBLineEdits[i] = new QLineEdit2(lineEditList, "0",xWidget, "veinRGBLineEdits" + QString::number(i)); xWidget->layout()->addWidget(veinRGBLineEdits[i]);
	}


	cleanNonTubeSurfacesButton = new QPushButton2(buttonList,"CleanTubeSet", partitionSkeletonWidget); partitionSkeletonWidget->layout()->addWidget(cleanNonTubeSurfacesButton);

	copyToClipboard3DDepthMapButton = new QPushButton2(buttonList, "GrabDepthMap", partitionSkeletonWidget); partitionSkeletonWidget->layout()->addWidget(copyToClipboard3DDepthMapButton);


	xWidget = new QWidget(partitionSkeletonWidget);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	partitionSkeletonWidget->layout()->addWidget(xWidget);
	
	label = new QLabel("maxRadius: ", xWidget); xWidget->layout()->addWidget(label);
	maxRadiusToColorizeEdit = new QLineEdit2(lineEditList,"0.0", xWidget); xWidget->layout()->addWidget(maxRadiusToColorizeEdit);


	xWidget = new QWidget(partitionSkeletonWidget);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	partitionSkeletonWidget->layout()->addWidget(xWidget);
	decolorizeTubeSurfacesWithoutRolesButton = new QPushButton2(buttonList, "DecolorizeNoRoles", xWidget); xWidget->layout()->addWidget(decolorizeTubeSurfacesWithoutRolesButton);
	xWidget = new QWidget(partitionSkeletonWidget);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	partitionSkeletonWidget->layout()->addWidget(xWidget);
	colorizeTubesButton = new QPushButton2(buttonList, "ColTubeByRadius", xWidget); xWidget->layout()->addWidget(colorizeTubesButton);
	colorizeErrorTubesButton = new QPushButton2(buttonList, "ColErrors", xWidget); xWidget->layout()->addWidget(colorizeErrorTubesButton);


	errorTubeModeCheckBox = new QCheckBox2(checkBoxList, xWidget);
	errorTubeModeCheckBox->setText("Mode");
	errorTubeModeCheckBox->setChecked(false);
	xWidget->layout()->addWidget(errorTubeModeCheckBox);

	getTubesStatsButton = new QPushButton2(buttonList, "GetTubesStats", partitionSkeletonWidget); partitionSkeletonWidget->layout()->addWidget(getTubesStatsButton);

	getOrientedSizesButton = new QPushButton2(buttonList, "GetOrientedSizes", partitionSkeletonWidget); partitionSkeletonWidget->layout()->addWidget(getOrientedSizesButton);

	xWidget1 = new QWidget(Info);
	xWidget1->setLayout(new QVBoxLayout);
	xWidget1->layout()->setSpacing(1);
	xWidget1->layout()->setMargin(1);
	Info->layout()->addWidget(xWidget1);


	thicknesLinesTreeWidget = new QTreeWidget(xWidget1);
	thicknesLinesTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	xWidget1->layout()->addWidget(thicknesLinesTreeWidget);


	lineNameLineEdit = new QLineEdit2(lineEditList,xWidget1);
	xWidget1->layout()->addWidget(lineNameLineEdit);


	xWidget = new QWidget(Info);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	xWidget1->layout()->addWidget(xWidget);





	memorizeUpperLinePushButton = new QPushButton2(buttonList,"MemUpperLine", xWidget); xWidget->layout()->addWidget(memorizeUpperLinePushButton);
	memorizeLowerLinePushButton = new QPushButton2(buttonList,"MemLowerLine", xWidget); xWidget->layout()->addWidget(memorizeLowerLinePushButton);

	xWidget = new QWidget(Info);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	xWidget1->layout()->addWidget(xWidget);

	calculateStatsBetweenLinesPushButton = new QPushButton2(buttonList,"CalculateStats", xWidget); xWidget->layout()->addWidget(calculateStatsBetweenLinesPushButton);
	deleteSelectedLinesPushButton = new QPushButton2(buttonList,"DelLines", xWidget); xWidget->layout()->addWidget(deleteSelectedLinesPushButton);

	//----------------------------


	

	FastFiltersGroupBox = new QGroupBox(Info);
	FastFiltersGroupBox->setLayout(new QVBoxLayout);
	FastFiltersGroupBox->setTitle("Fast Filters:");
	Info->layout()->addWidget(FastFiltersGroupBox);




	xWidget = new QWidget(FastFiltersGroupBox);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	FastFiltersGroupBox->layout()->addWidget(xWidget);



	disableRawDataMixerCheckBox2 = new QCheckBox2(checkBoxList,xWidget);
	disableRawDataMixerCheckBox2->setText("Disable Mixer");
	disableRawDataMixerCheckBox2->setChecked(false);
	xWidget->layout()->addWidget(disableRawDataMixerCheckBox2);
	/*for (int i = 0; i < 3; i++)
	{
		disableSeparatedMixersCheckBox[i] = new QCheckBox2(checkBoxList,xWidget);
		disableSeparatedMixersCheckBox[i]->setText(QString::number(i));
		disableSeparatedMixersCheckBox[i]->setChecked(true);
		xWidget->layout()->addWidget(disableSeparatedMixersCheckBox[i]);
	}*/


	


	fastFiltersComboBox = new QComboBox2(comboBoxList,xWidget);
	xWidget->layout()->addWidget(fastFiltersComboBox);
	xWidget->setFixedWidth(250);
	/*fastFiltersComboBox = new QComboBox2(comboBoxList,xWidget);
	xWidget->layout()->addWidget(fastFiltersComboBox);*/

	applyCurrentFastFilterPushButton = new QPushButton2(buttonList,"On", xWidget);
	xWidget->layout()->addWidget(applyCurrentFastFilterPushButton);

	forceRecalculateFilterPushButton = new QPushButton2(buttonList, "Recalc", xWidget);
	forceRecalculateFilterPushButton->setFlag(0, 1);// tylko taka flaga pozwala na growing 3d
	xWidget->layout()->addWidget(forceRecalculateFilterPushButton);



	apply3DFastFilterPushButton = new QPushButton2(buttonList,"Gen3D", xWidget);
	xWidget->layout()->addWidget(apply3DFastFilterPushButton);
	apply3DFastFilterPushButton->setEnabled(false);





	xWidget = new QWidget(FastFiltersGroupBox);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	FastFiltersGroupBox->layout()->addWidget(xWidget);
	updateFastFilterPushButton = new QPushButton2(buttonList,"Update Filter!", xWidget);
	xWidget->layout()->addWidget(updateFastFilterPushButton);
	updateFastFilterPushButton->setEnabled(true);
	///apply3DFastFilterPushButton->hide();
	//applyCurrentFastFilterPushButton->hide();

	//QValidator *vd = new TextFieldDoubleValidator(0, 10000.0, 15, this);
	for (int i = 0; i<PARAMS_NR; i++)
	{
		fastFilterParamsWidgets[i] = new ParamsWidget(sliderList,FastFiltersGroupBox);
		fastFilterParamsWidgets[i]->paramsCheckBox->hide();

	}




	/*OCTGroupBox = new QGroupBox(Info);
	OCTGroupBox->setLayout(new QVBoxLayout);
	OCTGroupBox->setTitle("OCT:");
	Info->layout()->addWidget(OCTGroupBox);
	xWidget = new QWidget(OCTGroupBox);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	OCTGroupBox->layout()->addWidget(xWidget);*/

	//dopplerCalcGroupBox = new QPushButton2(buttonList, "DopplerCalc", xWidget);
	//xWidget->layout()->addWidget(dopplerCalcGroupBox);




	MasksAndParamsGroupBox = new QGroupBox(Info);
	MasksAndParamsGroupBox->setLayout(new QVBoxLayout);
	//MasksAndParamsGroupBox->setTitle(":");
	Info->layout()->addWidget(MasksAndParamsGroupBox);



	xWidget = new QWidget(MasksAndParamsGroupBox);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	MasksAndParamsGroupBox->layout()->addWidget(xWidget);

	dopplerCalcGroupBox = new QPushButton2(buttonList, "DopplerCalc", xWidget);
	xWidget->layout()->addWidget(dopplerCalcGroupBox);

	

	QLabel* llx;
		
	llx = new QLabel(xWidget); llx->setText("ScX"); xWidget->layout()->addWidget(llx);
	editDopplerX = new QLineEdit2(lineEditList, xWidget); xWidget->layout()->addWidget(editDopplerX);editDopplerX->setText("1.0");
	llx = new QLabel(xWidget); llx->setText("ScY"); xWidget->layout()->addWidget(llx);
	editDopplerY = new QLineEdit2(lineEditList, xWidget); xWidget->layout()->addWidget(editDopplerY);;editDopplerY->setText("1.0");

	xWidget = new QWidget(MasksAndParamsGroupBox);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	MasksAndParamsGroupBox->layout()->addWidget(xWidget);
	GenrateMasksAndSurfacesButton = new QPushButton2(buttonList, "Masks and Surfaces", xWidget);
	xWidget->layout()->addWidget(GenrateMasksAndSurfacesButton);





	GenerateMedResultsButton = new QPushButton2(buttonList, "CalculateMed", xWidget);
	xWidget->layout()->addWidget(GenerateMedResultsButton);


	xWidget = new QWidget(MasksAndParamsGroupBox);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	MasksAndParamsGroupBox->layout()->addWidget(xWidget);

	ShowAllMasksButton = new QPushButton2(buttonList, "Show All Masks", xWidget);
	xWidget->layout()->addWidget(ShowAllMasksButton);


	xWidget = new QWidget(MasksAndParamsGroupBox);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	MasksAndParamsGroupBox->layout()->addWidget(xWidget);
	LoadStateButton = new QPushButton2(buttonList, "LoadState", xWidget);
	xWidget->layout()->addWidget(LoadStateButton);
	SaveStateButton = new QPushButton2(buttonList, "SaveState", xWidget);
	xWidget->layout()->addWidget(SaveStateButton);

	//-------------------------------------------------

	int nrrx = splitter->count();

	QSplitterHandle *handle;
	for (int i = 0; i<nrrx; i++)
	{
		handle = splitter->handle(i);
		splitter->setHandleWidth(16);
		QVBoxLayout *layout = new QVBoxLayout(handle);
		layout->setSpacing(4);
		layout->setMargin(4);

		QFrame *line = new QFrame(handle);
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		layout->addWidget(line);



	}




}
//-----------------------------------------------
void	GUIPanelWidget::CreateVolumeMixerTab(MyDockWidget *qtab)
{

	QWidget *Info = new QWidget(qtab);

	qtab->setScrollWidget(Info);

	QImage buffer, temp;
	QPixmap pix;

	if (buffer.load("images/layermixericons.png"))
	{
		int nr = 5;

		for (int i = 0; i<nr; i++)
		{

			temp = buffer.copy(i * 16, 0, 16, 12);
			pix = QPixmap::fromImage(temp);
			mixerIcons[i].addPixmap(pix);
		}

	}




	Info->setLayout(new QVBoxLayout());
	Info->layout()->setSpacing(1);
	Info->layout()->setMargin(1);

	QSplitter *splitter = new QSplitter(Qt::Vertical, Info);
	splitter->setChildrenCollapsible(false);
	//splitter->setLayout(new QVBoxLayout);

	splitter->setHandleWidth(3);

	QWidget *xWidget;
	QLabel *xLabel;

	QWidget *w;
	QWidget *b;
	QLabel *label;






	//--------------------------
	GbSnapshot = new	QWidget(Info);
	GbSnapshot->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	GbSnapshot->setLayout(new QVBoxLayout);
	GbSnapshot->layout()->setSpacing(1);
	GbSnapshot->layout()->setMargin(3);
	
	
	xWidget = new QWidget(GbSnapshot);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	GbSnapshot->layout()->addWidget(xWidget);

	disableRawDataMixerCheckBox = new QCheckBox2(checkBoxList,xWidget);
	disableRawDataMixerCheckBox->setText("Disable Mixer");
	disableRawDataMixerCheckBox->setChecked(false);
	xWidget->layout()->addWidget(disableRawDataMixerCheckBox);
	
	for (int i = 0; i < 3; i++)
	{
		disableSeparatedMixersCheckBox[i] = new QCheckBox2(checkBoxList,xWidget);
		disableSeparatedMixersCheckBox[i]->setText(QString::number(i));
		disableSeparatedMixersCheckBox[i]->setChecked(true);
		xWidget->layout()->addWidget(disableSeparatedMixersCheckBox[i]);
	}




	xWidget = new QWidget(GbSnapshot);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	GbSnapshot->layout()->addWidget(xWidget);

	LoadSnapshotForThicknessPushButton = new QPushButton2(buttonList,"Load Snapshot", xWidget);
	xWidget->layout()->addWidget(LoadSnapshotForThicknessPushButton);
	autoLoadSnapshotCheckBox = new QCheckBox2(checkBoxList,"auto update", xWidget);
	autoLoadSnapshotCheckBox->setChecked(false);
	xWidget->layout()->addWidget(autoLoadSnapshotCheckBox);
	//--------------------------
	xWidget->setMinimumHeight(20);



	xWidget = new QWidget(GbSnapshot);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	GbSnapshot->layout()->addWidget(xWidget);



	thicknessFilterComboBox = new QComboBox2(comboBoxList,xWidget);
	xWidget->layout()->addWidget(thicknessFilterComboBox);
	xWidget->hide();

	xWidget = new QWidget(GbSnapshot);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);



	label = new QLabel("histogram mode:", GbSnapshot);
	GbSnapshot->layout()->addWidget(label);
	histogramModeSlider =new QSlider2(sliderList,Qt::Horizontal, GbSnapshot);
	histogramModeSlider->setRange(0, 3);
	histogramModeSlider->setValue(3);
	GbSnapshot->layout()->addWidget(histogramModeSlider);
	
	tunningThicknessFilterSlider = new QSlider2(sliderList,Qt::Horizontal, xWidget);
	tunningThicknessFilterSlider->setRange(0, 999);
	xWidget->layout()->addWidget(tunningThicknessFilterSlider);




	tunningLabel = new QLabel("0.000", xWidget);
	xWidget->layout()->addWidget(tunningLabel);
	tunningLabel->setFixedWidth(30);
	xWidget->hide();
	GbSnapshot->layout()->addWidget(xWidget);

	//splitter->setCollapsible(false);
	Info->layout()->addWidget(splitter);

	QWidget *Gb1 = new	QWidget(Info);
	Gb1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
	Gb1->setLayout(new QVBoxLayout);
	Gb1->layout()->setSpacing(1);
	Gb1->layout()->setMargin(3);
	//Gb1->setTitle("Volume Mixer");


	int i;

	/*
	QGroupBox *Gb1=new	QGroupBox(Info);
	Gb3a->setLayout(new QVBoxLayout);
	Info->layout()->addWidget(Gb3a);
	Gb3a->setTitle("Volume Mixer");*/




	//ignoreCompatibilityMixerCheckBox=new QCheckBox2(checkBoxList,Gb3a);
	//ignoreCompatibilityMixerCheckBox->setText("Ignore Compatibility");
	//Gb3a->layout()->addWidget(ignoreCompatibilityMixerCheckBox);
	//*****************************************

	xWidget = new QWidget(Gb1);
	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);
	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);
	xLabel = new QLabel("Property:", xWidget);
	xWidget->layout()->addWidget(xLabel);

	filterCompositionsComboBox = new QComboBox2(comboBoxList,xWidget);
	xWidget->layout()->addWidget(filterCompositionsComboBox);

	enableFastFilterCheckBox = new QCheckBox2(checkBoxList,xWidget);
	xWidget->layout()->addWidget(enableFastFilterCheckBox);
	ApplyFilterCompositionButton = new QPushButton2(buttonList,"Apply", xWidget);
	xWidget->layout()->addWidget(ApplyFilterCompositionButton);

	xWidget = new QWidget(Gb1);	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);

	FilterCompositionNameLineEdit = new QLineEdit2(lineEditList,"default", xWidget);
	xWidget->layout()->addWidget(FilterCompositionNameLineEdit);

	AddFilterCompositionButton = new QPushButton2(buttonList,"Add Property", xWidget);
	xWidget->layout()->addWidget(AddFilterCompositionButton);

	xWidget = new QWidget(Gb1);	xWidget->setLayout(new QHBoxLayout);
	xWidget->layout()->setSpacing(1);	xWidget->layout()->setMargin(1);
	Gb1->layout()->addWidget(xWidget);

	autoUpdateCurrentFilterCheckBox = new QCheckBox2(checkBoxList,"Auto Update:", xWidget);
	autoUpdateCurrentFilterCheckBox->setChecked(false);

	xWidget->layout()->addWidget(autoUpdateCurrentFilterCheckBox);

	//*****************************************


	mixerTreeWidget = new QTreeWidget(Gb1);
	Gb1->layout()->addWidget(mixerTreeWidget);
	//ll.append("default");
	//mixerTreeWidget->insertTopLevelItem(0,new QTreeWidgetItem(ll));//QStringList("default")));
	mixerTreeWidget->setColumnCount(6);
	mixerTreeWidget->header()->resizeSection(0, 32);
	mixerTreeWidget->header()->resizeSection(1, 38);
	mixerTreeWidget->header()->resizeSection(2, 110);
	mixerTreeWidget->header()->resizeSection(3, 16);
	mixerTreeWidget->header()->resizeSection(4, 16);
	mixerTreeWidget->header()->resizeSection(5, 20);

	//mixerTreeWidget->header()->setic

	QTreeWidgetItem *headerItem = new QTreeWidgetItem();
	headerItem->setText(0, "C");
	headerItem->setIcon(1, mixerIcons[0]);
	headerItem->setText(2, "Layer Name");
	headerItem->setText(3, "F");
	headerItem->setText(4, "B");
	headerItem->setText(5, "O");

	mixerTreeWidget->setHeaderItem(headerItem);


	mixerTreeWidget->show();
	mixerTreeWidget->setSortingEnabled(false);

	splitter->addWidget(GbSnapshot);
	splitter->addWidget(Gb1);

	mixerParamsWidget = new	QWidget(Info);
	mixerParamsWidget->setLayout(new QVBoxLayout);
	Info->layout()->addWidget(mixerParamsWidget);
	splitter->addWidget(mixerParamsWidget);
	//mixerParamsWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
	

	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	QLabel *ll = new QLabel(w); ll->setText("Layer Name:"); w->layout()->addWidget(ll);
	rawDataLayerNameLineEdit = new QLineEdit2(lineEditList,w); w->layout()->addWidget(rawDataLayerNameLineEdit);



	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	mixerGenerateMixedVolumePushButton = new QPushButton2(buttonList,w); mixerGenerateMixedVolumePushButton->setText("Generate Mixed Data"); w->layout()->addWidget(mixerGenerateMixedVolumePushButton);

	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	mixerCreateLayerPushButton = new QPushButton2(buttonList,w); mixerCreateLayerPushButton->setText("Create Layer"); w->layout()->addWidget(mixerCreateLayerPushButton);


	w = new QWidget(Gb1); w->setLayout(new QHBoxLayout());	Gb1->layout()->addWidget(w); w->layout()->setMargin(1);
	mixerUpdate3DVolumePushButton = new QPushButton2(buttonList,w); mixerUpdate3DVolumePushButton->setText("Generate 3D Preview"); w->layout()->addWidget(mixerUpdate3DVolumePushButton);


	w = new QWidget(mixerParamsWidget); w->setLayout(new QHBoxLayout());	mixerParamsWidget->layout()->addWidget(w); w->layout()->setMargin(1);
	mixerChildModePushButton = new QPushButton2(buttonList,w); mixerChildModePushButton->setText("Switch Child Mode"); w->layout()->addWidget(mixerChildModePushButton);


	w = new QWidget(mixerParamsWidget); w->setLayout(new QHBoxLayout());	mixerParamsWidget->layout()->addWidget(w); w->layout()->setMargin(1);
	mixerMoveUpPushButton = new QPushButton2(buttonList,w); mixerMoveUpPushButton->setText("Move Up"); w->layout()->addWidget(mixerMoveUpPushButton);
	mixerMoveDownPushButton = new QPushButton2(buttonList,w); mixerMoveDownPushButton->setText("Move Down"); w->layout()->addWidget(mixerMoveDownPushButton);
	w->hide();
	w = new QWidget(mixerParamsWidget); w->setLayout(new QHBoxLayout());	mixerParamsWidget->layout()->addWidget(w); w->layout()->setMargin(1);
	mixerRemovePushButton = new QPushButton2(buttonList,w); mixerRemovePushButton->setText("Remove"); w->layout()->addWidget(mixerRemovePushButton);
	w->hide();

	//w=new QWidget(mixerParamsWidget); w->setLayout(new QHBoxLayout());	mixerParamsWidget->layout()->addWidget(w);w->layout()->setMargin(1);
	//mixerCreateMixedLayerPushButton=new QPushButton2(buttonList,w);mixerCreateMixedLayerPushButton->setText("Mixed Layer"); w->layout()->addWidget(mixerCreateMixedLayerPushButton);

	QFont font = QFont();
	font.setBold(true);
	QWidget *w0 = new QWidget(mixerParamsWidget); w0->setLayout(new QVBoxLayout());	mixerParamsWidget->layout()->addWidget(w0); w0->layout()->setMargin(1);
	label = new QLabel(w0); w0->layout()->addWidget(label);
	label->setText("Main:");
	label->setFont(font);

	w = new QWidget(w0); w->setLayout(new QHBoxLayout());	w0->layout()->addWidget(w); w->layout()->setMargin(1);
	label = new QLabel(w); w->layout()->addWidget(label);
	label->setText("Source:");
	label->setFixedWidth(50);
	mixerLayerSourceComboBox = new QComboBox2(comboBoxList,w); w->layout()->addWidget(mixerLayerSourceComboBox);



	connect(mixerMoveUpPushButton, SIGNAL(clicked()), this, SLOT(SlotMoveUpMixerItems()));
	connect(mixerMoveDownPushButton, SIGNAL(clicked()), this, SLOT(SlotMoveDownMixerItems()));
	

	


	mixerFilterWidget = new QWidget(mixerParamsWidget);

	mixerFilterWidget->setLayout(new QVBoxLayout());

	mixerFilterWidget->layout()->setSpacing(1);
	mixerFilterWidget->layout()->setMargin(0);


	mixerParamsWidget->layout()->addWidget(mixerFilterWidget);

	QWidget *w1 = new QWidget(mixerFilterWidget); w1->setLayout(new QVBoxLayout());	mixerFilterWidget->layout()->addWidget(w1); w1->layout()->setMargin(1);
	label = new QLabel(w1); w1->layout()->addWidget(label);
	label->setText("Filters:");
	label->setFont(font);

	QWidget *w2 = new QWidget(mixerFilterWidget); w2->setLayout(new QVBoxLayout());	mixerFilterWidget->layout()->addWidget(w2); w2->layout()->setMargin(1);
	label = new QLabel(w2); w2->layout()->addWidget(label);
	label->setText("Masks:");
	label->setFont(font);

	for (int i = 0; i < PARAMS_NR; i++)
	{
		QWidget *wt = NULL;
		if ((i ==  4) || (i == 5))
		{
			wt = w1;
		}
		else if (i == 9)
		{
			wt = w2;
		}
		else
		{
			wt = w0;
		}
		mainLayerParamsWidgets[i] = new ParamsWidget(sliderList,wt);
	}
	//FILTERS
	


//	w = new QWidget(mixerFilterWidget); w->setLayout(new QHBoxLayout());	mixerFilterWidget->layout()->addWidget(w); w->layout()->setMargin(1);
	mixerFilterModeComboBox = new QComboBox2(comboBoxList,w1); w1->layout()->addWidget(mixerFilterModeComboBox);

	int nrr = 0;
	

	for (int i = 0; i<PARAMS_NR; i++)
	{
		mixerFilterParamsWidgets[i] = new ParamsWidget(sliderList,w1);
	}

	//MASKS

	w = new QWidget(w2); w->setLayout(new QHBoxLayout());	w2->layout()->addWidget(w); w->layout()->setMargin(1);
	/*mixerMaskModeComboBox=new QComboBox2(comboBoxList,w); w->layout()->addWidget(mixerMaskModeComboBox);*/

	label = new QLabel(w); w->layout()->addWidget(label);
	label->setText("Source:");
	label->setFont(font);


	mixerLayerMaskSourceComboBox = new QComboBox2(comboBoxList,w); w->layout()->addWidget(mixerLayerMaskSourceComboBox);

	nrr = 0;


	//w = new QWidget(mixerFilterWidget); w->setLayout(new QHBoxLayout());	mixerFilterWidget->layout()->addWidget(w); w->layout()->setMargin(1);
	mixerMaskFilterModeComboBox = new QComboBox2(comboBoxList,w2); w2->layout()->addWidget(mixerMaskFilterModeComboBox);



	for (int i = 0; i<PARAMS_NR; i++)
	{
		mixerMaskFilterParamsWidgets[i] = new ParamsWidget(sliderList,w2);
		
	}


	int nrrx = splitter->count();

	QSplitterHandle *handle;
	for (int i = 0; i<nrrx; i++)
	{
		handle = splitter->handle(i);
		splitter->setHandleWidth(16);
		QVBoxLayout *layout = new QVBoxLayout(handle);
		layout->setSpacing(4);
		layout->setMargin(4);

		QFrame *line = new QFrame(handle);
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		layout->addWidget(line);
	}
}
////-----------------------------------------------
void	GUIPanelWidget::SlotInsertNewVolumeMixerItem(RawDataLayer *layer, LinkedList <RawDataLayer *> *rawDataLayerList)
{



	QTreeWidgetItem *item = new QTreeWidgetItem();
	//item->setFlags(item->flags() | Qt::ItemIsEditable);

	QString name = layer->GetName();
	double opacity = layer->GetMainParam(0);//layer->GetBlendingOpacity();
	int mode = layer->GetMainParam(3);//layer->GetBlendingMode();

	item->setIcon(0, this->distinctColorIconsCirc[mDataCounter]);//layer visibility - on
	item->setIcon(1, mixerIcons[0]);//layer visibility - on
	item->setIcon(3, emptyIcon);//filter visibility -off

	item->setText(5, QString::number(opacity, 'g', 2));
	item->setText(2, name);
	item->setText(4, QString::number(mode, 'g', 2));

	layer->SetTreeWidgetItem(item, mDataCounter);

	this->mixerTreeWidget->insertTopLevelItem(0, item);

	//layer->GetTreeWidgetItem()
	//rawDataLayerList->InsertItem(layer);
	mDataCounter = (mDataCounter + 1) % 255;
}
//--------------------------------

void	GUIPanelWidget::SetLayerFilterVisibilityIcons(bool layerVis, bool filterVis, QTreeWidgetItem *itemx)
{
	//	mixerLayerEnableCheckBox->isChecked(),mixerFilterEnableCheckBox->isChecked());
	QTreeWidgetItem *item = NULL;
	if (itemx == NULL)
	{
		QList<QTreeWidgetItem *> selectedItems = mixerTreeWidget->selectedItems();

		if (selectedItems.isEmpty())
			return;

		int row = mixerTreeWidget->indexOfTopLevelItem(selectedItems.first());
		if (row == mixerTreeWidget->topLevelItemCount() - 1)//najnizsza warstwa nic nie mozna z nia robic
			return;

		QTreeWidgetItem *itemAbove;
		QListIterator<QTreeWidgetItem *> it(selectedItems);
		QTreeWidgetItem *currentItem;
		item = it.next();
	}
	else
	{
		item = itemx;
	}

	if (layerVis == true) item->setIcon(1, mixerIcons[0]); else item->setIcon(1, mixerIcons[1]);
	if (filterVis == true) item->setIcon(3, mixerIcons[3]); else item->setIcon(3, emptyIcon);

}
//---------------------------------------------------
void	GUIPanelWidget::SetLayerVisibilityIcon(QTreeWidgetItem *currentItem, bool layerVis)
{
	//	mixerLayerEnableCheckBox->isChecked(),mixerFilterEnableCheckBox->isChecked());



	if (layerVis == true) currentItem->setIcon(1, mixerIcons[0]); else currentItem->setIcon(1, mixerIcons[1]);
	//if (filterVis==true) currentItem->setIcon(2,mixerIcons[3]); else currentItem->setIcon(2,emptyIcon);

}
//--------------------------------
void	GUIPanelWidget::SlotSwitchChildMode(bool childMode)
{




	//dodam albo usune strzalke - inaczej bez sensu!!!


	QList<QTreeWidgetItem *> selectedItems = mixerTreeWidget->selectedItems();

	if (selectedItems.isEmpty())
		return;

	int row = mixerTreeWidget->indexOfTopLevelItem(selectedItems.first());
	if (row == 0)
		return;

	QTreeWidgetItem *itemAbove;
	QListIterator<QTreeWidgetItem *> it(selectedItems);
	QTreeWidgetItem *currentItem;
	currentItem = it.next();
	if (childMode == false)//jest dzieckiem ma zostac parentem
	{

		////znajdz jego parenta i wstaw go jako top level item pod niego
		//itemAbove=currentItem->parent();
		//row = mixerTreeWidget->indexOfTopLevelItem( itemAbove );
		//mixerTreeWidget->insertTopLevelItem( row + 1  , currentItem );
		currentItem->setIcon(2, emptyIcon);
	}
	else
	{
		//itemAbove = mixerTreeWidget->takeTopLevelItem( row - 1 );
		////itemAbove->insertChild(itemAbove->childCount(),currentItem);
		//itemAbove->addChild(currentItem);
		currentItem->setIcon(2, mixerIcons[2]);


	}

}
//----------------------------------------
void	GUIPanelWidget::SlotMoveUpMixerItems()
{

	QList<QTreeWidgetItem *> selectedItems = mixerTreeWidget->selectedItems();

	if (selectedItems.isEmpty())
		return;

	// If selection has the first line the items are on the top -> return
	int row = mixerTreeWidget->indexOfTopLevelItem(selectedItems.first());
	if (row == 0)
		return;

	QTreeWidgetItem *itemAbove;
	QListIterator<QTreeWidgetItem *> it(selectedItems);

	while (it.hasNext())
	{
		row = mixerTreeWidget->indexOfTopLevelItem(it.next());
		itemAbove = mixerTreeWidget->takeTopLevelItem(row - 1);
		mixerTreeWidget->insertTopLevelItem(row, itemAbove);
	}


}
//-----------------------------------
void	GUIPanelWidget::SlotMoveDownMixerItems()
{
	QList<QTreeWidgetItem *> selectedItems = mixerTreeWidget->selectedItems();

	if (selectedItems.isEmpty())
		return;

	// If selection has the first line the items are on the top -> return
	int row = mixerTreeWidget->indexOfTopLevelItem(selectedItems.last());
	if (row == mixerTreeWidget->topLevelItemCount() - 1)
		return;

	QTreeWidgetItem *itemBelow;

	row = mixerTreeWidget->indexOfTopLevelItem(selectedItems.last());
	itemBelow = mixerTreeWidget->takeTopLevelItem(row + 1);
	mixerTreeWidget->insertTopLevelItem(row + 1 - selectedItems.length(), itemBelow);

	mixerTreeWidget->scrollToItem(selectedItems.last());
}
//-----------------------------------
void	GUIPanelWidget::SlotRemoveMixerItems()
{


}
//-----------------------------------
void	GUIPanelWidget::SlotShowCurrentPaletteName(int nr)
{
	currentPaletteName->setText(this->paletteNames->at(nr));
}


//----------------------------------------------------

void	GUIPanelWidget::CreateDevTab(MyDockWidget* qtab)
{
	devTabMainWidget = new QWidget(qtab);
	QWidget* PaletteTab = devTabMainWidget;
	qtab->setScrollWidget(PaletteTab);
	QLayout* MainLayout = new QVBoxLayout();


	PaletteTab->setLayout(MainLayout);

	QWidget* vWidget = new	QWidget(PaletteTab);
	vWidget->setLayout(new QHBoxLayout());
	MainLayout->addWidget(vWidget);

	QLabel* label;
	label = new QLabel(vWidget);vWidget->layout()->addWidget(label);
	label->setText("Volume Scale Tunning");
	
	TextFieldDoubleValidator* scaleValidator = new TextFieldDoubleValidator(0, 100000.0, 15, this);
;
	for (int j = 0;j < 3;j++)
	{
		scaleDevLineEdit[j] = new QLineEdit2(lineEditList, vWidget); vWidget->layout()->addWidget(scaleDevLineEdit[j]);
		scaleDevLineEdit[j]->setText("1.0");
		scaleDevLineEdit[j]->setFixedWidth(45);
		scaleDevLineEdit[j]->setValidator(scaleValidator);
	}

	vWidget = new	QWidget(PaletteTab);
	vWidget->setLayout(new QHBoxLayout());
	MainLayout->addWidget(vWidget);
	applyNewDevScaleButton = new QPushButton2(buttonList, "Apply New Scale", vWidget);vWidget->layout()->addWidget(applyNewDevScaleButton);
	resetOriginalScaleButton = new QPushButton2(buttonList, "ResetScale", vWidget);vWidget->layout()->addWidget(resetOriginalScaleButton);





}
//---------------------------------
void	GUIPanelWidget::CreateLogTab(MyDockWidget *qtab)
{

	logTabMainWidget = new QWidget(qtab);


	QWidget* PaletteTab = logTabMainWidget;
	qtab->setScrollWidget(PaletteTab);
	QLayout* MainLayout = new QVBoxLayout();
	
	
	PaletteTab->setLayout(MainLayout);

	QWidget* vWidget;
	//-------------------------------- JOB3
	QLabel* lab;
	//QLineEdit2* job3ScriptLineEdit;
	//QLineEdit2* job3FilePathSetLineEdit;
	//QLineEdit2* job3DestinationPathLineEdit;

	//QPushButton2* selectFilesetButton;
	//QPushButton2* selectScriptButton;
	//QPushButton2* selectDestinationPathButton;

	QGroupBox* job3GroupBox;
	job3GroupBox = new QGroupBox(PaletteTab);
	job3GroupBox->setLayout(new QVBoxLayout);
	job3GroupBox->setTitle("Job3:");
	MainLayout->layout()->addWidget(job3GroupBox);
	job3GroupBox->layout()->setSpacing(1);
	job3GroupBox->layout()->setMargin(2);
	//line1
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	createDirForJob3Button = new QPushButton2(buttonList, "SetFileDirForJob3", PaletteTab);	vWidget->layout()->addWidget(createDirForJob3Button);
	lab = new QLabel("sufix:", PaletteTab);	vWidget->layout()->addWidget(lab);
	sufixJob3LineEdit = new QLineEdit2(lineEditList, PaletteTab,"sufixJob3LineEdit");	vWidget->layout()->addWidget(sufixJob3LineEdit);

	//line1a

	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);


	subdirsJob3CheckBox = new QCheckBox2(checkBoxList, PaletteTab, "subdirsJob3CheckBox");
	subdirsJob3CheckBox->setText("includeSubdirs");	subdirsJob3CheckBox->setChecked(false);vWidget->layout()->addWidget(subdirsJob3CheckBox);
	onlyLatestFileInSubdirsJob3CheckBox = new QCheckBox2(checkBoxList, PaletteTab, "onlyLatestFileInSubdirsJob3CheckBox");
	onlyLatestFileInSubdirsJob3CheckBox->setText("onlyLatestInSubdir");	onlyLatestFileInSubdirsJob3CheckBox->setChecked(false);vWidget->layout()->addWidget(onlyLatestFileInSubdirsJob3CheckBox);




	//line2
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	selectFilesetButton = new QPushButton2(buttonList, "SelectFileset", PaletteTab);	vWidget->layout()->addWidget(selectFilesetButton);
	job3FilePathSetLineEdit = new QLineEdit2(lineEditList, PaletteTab,"job3FilePathSetLineEdit");	vWidget->layout()->addWidget(job3FilePathSetLineEdit);
	//line3
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	selectScriptButton = new QPushButton2(buttonList, "SelectScript", PaletteTab);	vWidget->layout()->addWidget(selectScriptButton);
	job3ScriptLineEdit = new QLineEdit2(lineEditList, PaletteTab, "job3ScriptLineEdit");	vWidget->layout()->addWidget(job3ScriptLineEdit);
	//line3
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	selectDestinationPathButton = new QPushButton2(buttonList, "DestinationPath", PaletteTab);	vWidget->layout()->addWidget(selectDestinationPathButton);
	job3DestinationPathLineEdit = new QLineEdit2(lineEditList, PaletteTab, "job3DestinationPathLineEdit");	vWidget->layout()->addWidget(job3DestinationPathLineEdit);

	//line4
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	selectAffineTransPathButton = new QPushButton2(buttonList, "AffineTransPath", PaletteTab);	vWidget->layout()->addWidget(selectAffineTransPathButton);
	job3AffineTransPathLineEdit = new QLineEdit2(lineEditList, PaletteTab, "job3AffineTransPathLineEdit");	vWidget->layout()->addWidget(job3AffineTransPathLineEdit);


	


	//line5
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	lab = new QLabel("RawParams:", PaletteTab);	vWidget->layout()->addWidget(lab);
	job3RawFileParamsLineEdit = new QLineEdit2(lineEditList, PaletteTab, "job3RawFileParamsLineEdit");	vWidget->layout()->addWidget(job3RawFileParamsLineEdit);
	logRawDataCheckBox = new QCheckBox2(checkBoxList, PaletteTab, "logRawDataCheckBox");
	logRawDataCheckBox->setText("log");	logRawDataCheckBox->setChecked(false);vWidget->layout()->addWidget(logRawDataCheckBox);
	//QCheckBox2 logRawDataCheckBox;

	//line6
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	lab = new QLabel("LoadByIndex:", PaletteTab);	vWidget->layout()->addWidget(lab);
	job3IndexListLineEdit = new QLineEdit2(lineEditList, PaletteTab, "job3IndexListLineEdit");	vWidget->layout()->addWidget(job3IndexListLineEdit);
	//QLineEdit2* job3IndexListLineEdit;


	inverseIndexListCheckBox = new QCheckBox2(checkBoxList, PaletteTab, "inverseIndexListCheckBox");vWidget->layout()->addWidget(inverseIndexListCheckBox);
	inverseIndexListCheckBox->setText("inv");
	//line7
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);

	/*QCheckBox2* job3TestModeCheckBox;
	QCheckBox2* job3DataNumberingCheckbox;*/


	job3TestModeCheckBox = new QCheckBox2(checkBoxList,PaletteTab,"job3TestModeCheckBox");
	job3TestModeCheckBox->setText("TestMode");	job3TestModeCheckBox->setChecked(false);vWidget->layout()->addWidget(job3TestModeCheckBox);
	job3DataNumberingCheckbox = new QCheckBox2(checkBoxList,PaletteTab, "job3DataNumberingCheckbox");
	job3DataNumberingCheckbox->setText("CustomName");	job3DataNumberingCheckbox->setChecked(false);vWidget->layout()->addWidget(job3DataNumberingCheckbox);
	job3CustomNameLineEdit =new QLineEdit2(lineEditList, PaletteTab, "job3CustomNameLineEdit");	vWidget->layout()->addWidget(job3CustomNameLineEdit);

	//line8
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	lab = new QLabel("regModelNr:", PaletteTab);	vWidget->layout()->addWidget(lab);

	job3RegModelDataNumberLineEdit = new QLineEdit2(lineEditList, PaletteTab, "job3RegModelDataNumberLineEdit");	vWidget->layout()->addWidget(job3RegModelDataNumberLineEdit);
	
	
	
	findTheBestCandidateForModelButton = new QPushButton2(buttonList, "FindModelCandidate", PaletteTab);	vWidget->layout()->addWidget(findTheBestCandidateForModelButton);

	genStabCheckBox = new QCheckBox2(checkBoxList, PaletteTab, "genStabCheckBox");vWidget->layout()->addWidget(genStabCheckBox);
	genStabCheckBox->setText("stab");

	lab = new QLabel("medThresh:", PaletteTab);	vWidget->layout()->addWidget(lab);
	medFactorJob3LineEdit = new QLineEdit2(lineEditList, PaletteTab, "medFactorJob3LineEdit");	vWidget->layout()->addWidget(medFactorJob3LineEdit);
	
	
	
	//line9
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	lab = new QLabel("minModelSlice:", PaletteTab);	vWidget->layout()->addWidget(lab);


	minSliceModelLineEdit = new QLineEdit2(lineEditList, PaletteTab, "minSliceModelLineEdit");	vWidget->layout()->addWidget(minSliceModelLineEdit);

	
	lab = new QLabel("minMaxSlice:", PaletteTab);	vWidget->layout()->addWidget(lab);
	maxSliceModelLineEdit = new QLineEdit2(lineEditList, PaletteTab, "maxSliceModelLineEdit");	vWidget->layout()->addWidget(maxSliceModelLineEdit);

	//line10
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	lab = new QLabel("minModelBlockSize:", PaletteTab);	vWidget->layout()->addWidget(lab);
	minModelBlockSizeLineEdit= new QLineEdit2(lineEditList, PaletteTab, "minModelBlockSizeLineEdit");	vWidget->layout()->addWidget(minModelBlockSizeLineEdit);
	//line11
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);
	applyScriptForFob3Button = new QPushButton2(buttonList, "ApplyScript(Job3)", PaletteTab);	vWidget->layout()->addWidget(applyScriptForFob3Button);


	applyTempButton = new QPushButton2(buttonList, "applyTempButton", PaletteTab);	vWidget->layout()->addWidget(applyTempButton);
	
	//line12
	vWidget = new	QWidget(job3GroupBox);	vWidget->setLayout(new QHBoxLayout());	job3GroupBox->layout()->addWidget(vWidget);


	
	job3SegmentatorModeCheckBox = new QCheckBox2(checkBoxList, PaletteTab, "job3SegmentatorModeCheckBox");
	job3SegmentatorModeCheckBox->setText("UseJob3ButtonAsDopplerSegmentator");	job3TestModeCheckBox->setChecked(false);vWidget->layout()->addWidget(job3SegmentatorModeCheckBox);

	//----------------------------------------
	vWidget = new	QWidget(PaletteTab);
	vWidget->setLayout(new QHBoxLayout());
	MainLayout->addWidget(vWidget);
	generateRandomPointsAndPrepareJob2Button = new QPushButton2(buttonList, "Create Job2", PaletteTab);
	


	
	vWidget->layout() ->addWidget(generateRandomPointsAndPrepareJob2Button);

	QLabel* L1 = new QLabel("Samples: ", vWidget);
	vWidget->layout()->addWidget(L1);
	nrOfRandbomSamplesSpinBox = new QSpinBox2(spinBoxList, vWidget);
	vWidget->layout()->addWidget(nrOfRandbomSamplesSpinBox);
	nrOfRandbomSamplesSpinBox->setRange(1, 1000);
	nrOfRandbomSamplesSpinBox->setValue(50);

	
	analyzeForLatestArteryDataFilesInSubdirectoriesButton = new QPushButton2(buttonList, "Create Job", PaletteTab);
	MainLayout->addWidget(analyzeForLatestArteryDataFilesInSubdirectoriesButton);
	

	startJobButton = new QPushButton2(buttonList, "Start Job", PaletteTab);
	MainLayout->addWidget(startJobButton);

	logTextEdit = new QPlainTextEdit(PaletteTab);
	MainLayout->addWidget(logTextEdit);
	clearLogPushButton = new QPushButton2(buttonList,"Clear",PaletteTab);

	connect(clearLogPushButton, SIGNAL(clicked()), logTextEdit, SLOT(clear()));

	MainLayout->addWidget(clearLogPushButton);

}

