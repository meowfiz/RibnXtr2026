#include	"PropertyWidget.h"
//Added by qt3to4:
#include <QCloseEvent>
#include <QLabel>
//#include <QVBoxLayout>



PropertyWidget::PropertyWidget(QWidget *parent,QString name,Qt::WindowFlags flags):MyDockWidget(name,name,false,parent,flags)
{



////tylko rysowanie
	OriginalProperty=vtkProperty::New();
	NewProperty=vtkProperty::New();
	// bede to ustawial poprzez deepcopy z oryginalu ...

//	setCaption(name);
	//QVBoxLayout* layout1= new QVBoxLayout( this, 10, -1, "layout");	
	QWidget *widget=new QWidget(this);
	
	this->setWidget(widget);
	widget->setLayout(new QVBoxLayout());
	
	qtab=new QTabWidget(widget);
	widget->layout()->addWidget(qtab);
		

	QWidget *PropertyTab = new QWidget(qtab);
	PropertyTab->setLayout(new QVBoxLayout());
	PropertyTab->setMinimumWidth(260);
	PropertyTab->setMinimumHeight(620);

	//QVBoxLayout* MainLayout= new QVBoxLayout( PropertyTab, 10, -1, "MainLayout");	

	qtab->addTab(PropertyTab,"Property Widget");

	//Color Group
	//------------------------------

	QGroupBox *Gb1=new QGroupBox(PropertyTab);
	PropertyTab->layout()->addWidget(Gb1);
	Gb1->setLayout(new QVBoxLayout());
	Gb1->setTitle("Color");

	Gb1->layout()->setMargin(1);
	Gb1->layout()->setSpacing(1);

//	QVBoxLayout* layoutG1= new QVBoxLayout( Gb1,10);	

	QWidget *GridCol = new QWidget( Gb1);
	QGridLayout *gLayout = new QGridLayout();
	GridCol->setLayout(gLayout);
	Gb1->layout()->addWidget(GridCol);
//	GridCol->setAutoMask(true);
	ColorButton = new	QPushButton("Color...", GridCol); gLayout->addWidget(ColorButton, 0, 0);

	//ColorButton->setAutoMask(true);
	QLabel *L00 = new QLabel(GridCol); gLayout->addWidget(L00, 0,1);
 //	L00->setAutoMask(true);

	ColRedLabel = new QLabel("Red	(0/255) ", GridCol); gLayout->addWidget(ColRedLabel,1,0);
 //	ColRedLabel->setAutoMask(true);	
	ColRedLabel->setFixedWidth(120);

	RedColorSlider = new QSlider(Qt::Horizontal, GridCol); gLayout->addWidget(RedColorSlider,1,1);
		
	ColGreenLabel = new QLabel("Green	(0/255) ", GridCol); gLayout->addWidget(ColGreenLabel,2,0);
// 	ColGreenLabel->setAutoMask(true);
	ColGreenLabel->setFixedWidth(120);
	GreenColorSlider = new QSlider(Qt::Horizontal, GridCol); gLayout->addWidget(GreenColorSlider,2,1);
		
	ColBlueLabel = new QLabel("Blue	(0/255)", GridCol); gLayout->addWidget(ColBlueLabel,3,0);
// 	ColBlueLabel->setAutoMask(true);
	ColBlueLabel->setFixedWidth(120);
	BlueColorSlider = new QSlider(Qt::Horizontal, GridCol); gLayout->addWidget(BlueColorSlider,3,1);
		

	//------------------------------

	//Specular Group
	//------------------------------
	
	QWidget *Gb2 = new QWidget(PropertyTab);

	PropertyTab->layout()->addWidget(Gb2);
	Gb2->setLayout(new QVBoxLayout());
	Gb2->layout()->setMargin(10);
//	Gb2->setTitle("Specular");

//	QVBoxLayout* layoutG2= new QVBoxLayout( Gb2,10);	


	QWidget *GridCol2 = new QWidget(Gb2);
	gLayout = new QGridLayout();
	GridCol2->setLayout(gLayout);
		Gb2->layout()->addWidget(GridCol2);
// 	GridCol2->setAutoMask(true);

		SpecularColorButton = new	QPushButton("Specular Color...", GridCol2); gLayout->addWidget(SpecularColorButton,0,0);

	//SpecularColorButton->setAutoMask(true);
		QLabel *L20 = new QLabel(GridCol2); gLayout->addWidget(L20,0,1);
// 	L20->setAutoMask(true);
	
		SpecularLightLabel = new QLabel("Value	(0.0/1.00)", GridCol2); gLayout->addWidget(SpecularLightLabel,1,0);
 //	SpecularLightLabel->setAutoMask(true);
	SpecularLightLabel->setFixedWidth(120);
	SpecularLightingCoefSlider = new QSlider(Qt::Horizontal, GridCol2); gLayout->addWidget(SpecularLightingCoefSlider,1,1);
		
	SpecularPowerLabel = new QLabel("Power	(0.0/1.00)", GridCol2); gLayout->addWidget(SpecularPowerLabel,2,0);
 //	SpecularPowerLabel->setAutoMask(true);
	SpecularPowerLabel->setFixedWidth(120);
	SpecularPowerSlider = new QSlider(Qt::Horizontal, GridCol2); gLayout->addWidget(SpecularPowerSlider,2,1);



	//------------------------------
	

//------------------------------

	//Diffuse Group
	//------------------------------
	
	QGroupBox *Gb3=new QGroupBox(PropertyTab);
PropertyTab->layout()->addWidget(Gb3);
	Gb3->setLayout(new QVBoxLayout());
	Gb3->layout()->setMargin(10);
	Gb3->setTitle("Diffuse");

//	QVBoxLayout* layoutG3= new QVBoxLayout( Gb3,10);	


	QWidget *GridCol3 = new QWidget(Gb3);
	gLayout = new QGridLayout();
	GridCol3->setLayout(gLayout);
	Gb3->layout()->addWidget(GridCol3);
	//GridCol3->setAutoMask(true);

	DiffuseColorButton = new	QPushButton("Diffuse Color...", GridCol3); gLayout->addWidget(DiffuseColorButton,0,0);
	//DiffuseColorButton->setAutoMask(true);
	QLabel *L30=new QLabel(GridCol3);
 	//L30->setAutoMask(true);
	
	DiffuseLightLabel = new QLabel("Value	(0.0/1.00)", GridCol3); gLayout->addWidget(DiffuseLightLabel,1,0);
 //	DiffuseLightLabel->setAutoMask(true);
	DiffuseLightLabel->setFixedWidth(120);
	DiffuseLightingCoefSlider = new QSlider(Qt::Horizontal, GridCol3); gLayout->addWidget(DiffuseLightingCoefSlider,1,1);
	

	//------------------------------



	//Ambient Group
	//------------------------------
	
	QGroupBox *Gb4=new QGroupBox(PropertyTab);
PropertyTab->layout()->addWidget(Gb4);
	Gb4->setLayout(new QVBoxLayout());
//	Gb4->setMargin(10);
	Gb4->setTitle("Ambient");

//	QVBoxLayout* layoutG4= new QVBoxLayout( Gb4,10);	


	QWidget *GridCol4 = new QWidget(Gb4);
	gLayout = new QGridLayout();
	GridCol4->setLayout(gLayout);
Gb4->layout()->addWidget(GridCol4);
// 	GridCol4->setAutoMask(true);

AmbientColorButton = new	QPushButton("Ambient Color...", GridCol4); gLayout->addWidget(AmbientColorButton,0,0);
	//AmbientColorButton->setAutoMask(true);
	QLabel *L40=new QLabel(GridCol4);
 	//L40->setAutoMask(true);
	
	AmbientLightLabel = new QLabel("Value	(0.0/1.00)", GridCol4); gLayout->addWidget(AmbientLightLabel,1,0);
 //	AmbientLightLabel->setAutoMask(true);
	AmbientLightLabel->setFixedWidth(120);
	AmbientLightingCoefSlider = new QSlider(Qt::Horizontal, GridCol4); gLayout->addWidget(AmbientLightingCoefSlider,1,1);
	

	//------------------------------
	//Opacity Group
	//------------------------------
	
	QGroupBox *Gb5=new QGroupBox(PropertyTab);
PropertyTab->layout()->addWidget(Gb5);
	Gb5->setLayout(new QVBoxLayout());
	Gb5->setTitle("Opacity");

//	QVBoxLayout* layoutG5= new QVBoxLayout( Gb5,10);	

	QWidget *GridCol5 = new QWidget(Gb5);
	gLayout = new QGridLayout();
	GridCol5->setLayout(gLayout);
		Gb5->layout()->addWidget(GridCol5);

	//GridCol5->setAutoMask(true);
	
		OpacityLabel = new QLabel("Value	(0.0/1.00)", GridCol5); gLayout->addWidget(OpacityLabel,0,0);
 //	OpacityLabel->setAutoMask(true);
	OpacityLabel->setFixedWidth(120);
	OpacitySlider = new QSlider(Qt::Horizontal, GridCol5); gLayout->addWidget(OpacitySlider,0,1);
	

	//------------------------------

	//------------------------------
	//Interpolation Group
	//------------------------------
	
	QGroupBox *Gb6=new QGroupBox(PropertyTab);
	PropertyTab->layout()->addWidget(Gb6);
	Gb6->setLayout(new QVBoxLayout());
	Gb6->setTitle("Interpolation");

//	QVBoxLayout* layoutG6= new QVBoxLayout( Gb6,10);	

	QWidget *GridCol6 = new QWidget(Gb6);
	//Gb6->layout()->addWidget(GridCol6);

	QButtonGroup *gr1=new QButtonGroup;
	QHBoxLayout *hbl_GridCol6 = new QHBoxLayout;
	hbl_GridCol6->setMargin(1);
	hbl_GridCol6->setSpacing(0);
	GridCol6->setLayout(hbl_GridCol6);	
	Gb6->layout()->addWidget(GridCol6);

	//GridCol6->setLayout(new QHBoxLayout());
 //	GridCol6->setAutoMask(true);

	
	FlatRadio = new QRadioButton(GridCol6);
	GouraudRadio = new QRadioButton(GridCol6);
	PhongRadio = new QRadioButton(GridCol6);
	FlatRadio->setText("Flat");
	GouraudRadio->setText("Gouraud");
	PhongRadio->setText("Phong");
	GridCol6->layout()->addWidget(FlatRadio);
	GridCol6->layout()->addWidget(GouraudRadio);
	GridCol6->layout()->addWidget(PhongRadio);
	//------------------------------


		//------------------------------
	//Representation Group
	//------------------------------
	
	QGroupBox *Gb7=new QGroupBox(PropertyTab);
		PropertyTab->layout()->addWidget(Gb7);
	Gb7->setLayout(new QVBoxLayout());
	Gb7->setTitle("Representation");

//	QVBoxLayout* layoutG7= new QVBoxLayout( Gb7,10);	


	QWidget *GridCol7 = new QWidget(Gb4);

	QButtonGroup *gr2=new QButtonGroup;
	QHBoxLayout *hbl_GridCol7 = new QHBoxLayout;
	hbl_GridCol7->setMargin(1);
	hbl_GridCol7->setSpacing(0);
	GridCol7->setLayout(hbl_GridCol7);	
	Gb7->layout()->addWidget(GridCol7);
	
	//QButtonGroup *GridCol7 = new QButtonGroup(Gb7);
 	//GridCol7->setAutoMask(true);

	
	SurfaceRadio = new QRadioButton(GridCol7);
	WireframeRadio = new QRadioButton(GridCol7);
	PointsRadio = new QRadioButton(GridCol7);
	SurfaceRadio->setText("Surface");
	WireframeRadio->setText("Wireframe");
	PointsRadio->setText("Points");
	GridCol7->layout()->addWidget(SurfaceRadio);
	GridCol7->layout()->addWidget(WireframeRadio);
	GridCol7->layout()->addWidget(PointsRadio);

	//------------------------------
	QWidget *GridCol8 = new QWidget(PropertyTab);
	gLayout = new QGridLayout();
	GridCol8->setLayout(gLayout);
	PropertyTab->layout()->addWidget(GridCol8);
 	//GridCol8->setAutoMask(true);

	ApplyChangesButton = new	QPushButton("Apply", GridCol8); gLayout->addWidget(ApplyChangesButton,0, 0);
	CancelChangesButton = new	QPushButton("Cancel", GridCol8); gLayout->addWidget(CancelChangesButton, 0, 1);
	SaveAsDefaultButton = new	QPushButton("Save as default", GridCol8); gLayout->addWidget(SaveAsDefaultButton, 0, 2);

/*
	MainLayout->addWidget(Gb1);
	MainLayout->addWidget(Gb2);
	MainLayout->addWidget(Gb3);
	MainLayout->addWidget(Gb4);
	MainLayout->addWidget(Gb5);
	MainLayout->addWidget(Gb6);
	MainLayout->addWidget(Gb7);
	MainLayout->addWidget(GridCol8);
*/
	QLabel *L800=new QLabel(PropertyTab);
		PropertyTab->layout()->addWidget(L800);
	//L800->setAutoMask(true);
	L800->setMinimumHeight(15);

	//MainLayout->addWidget(L800);
	



	 SpecularPowerSlider->setRange(0,100);
	 SpecularLightingCoefSlider->setRange(0,100);
	 DiffuseLightingCoefSlider->setRange(0,100);
	 AmbientLightingCoefSlider->setRange(0,100);

	 OpacitySlider->setRange(0,100);
	 RedColorSlider->setRange(0,255);
	 GreenColorSlider->setRange(0,255);
	 BlueColorSlider->setRange(0,255);


	
//	SlotInsertProperty();


	connect(ColorButton,SIGNAL(clicked()),this,SLOT(SlotColorClicked()));
	connect(AmbientColorButton,SIGNAL(clicked()),this,SLOT(SlotAmbientClicked()));
	connect(SpecularColorButton,SIGNAL(clicked()),this,SLOT(SlotSpecularClicked()));
	connect(DiffuseColorButton,SIGNAL(clicked()),this,SLOT(SlotDiffuseClicked()));

	connect(ApplyChangesButton,SIGNAL(clicked()),this,SLOT(SlotApplyClicked()));
	connect(CancelChangesButton,SIGNAL(clicked()),this,SLOT(SlotCancelClicked()));
	connect(SaveAsDefaultButton,SIGNAL(clicked()),this,SLOT(SlotSaveClicked()));
	

	connect(RedColorSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotSetRedValue(int)));
	connect(GreenColorSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotSetGreenValue(int)));
	connect(BlueColorSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotSetBlueValue(int)));
	connect(SpecularLightingCoefSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotSpecularValue(int)));
	connect(SpecularPowerSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotSpecularPower(int)));
	connect(DiffuseLightingCoefSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotDiffuseValue(int)));
	connect(AmbientLightingCoefSlider,SIGNAL(valueChanged(int)),this,SLOT(SlotAmbientValue(int)));
	connect(OpacitySlider,SIGNAL(valueChanged(int)),this,SLOT(SlotOpacityValue(int)));
	
	
	connect(RedColorSlider,SIGNAL(sliderReleased()),this,SIGNAL(Updated()));
	connect(GreenColorSlider,SIGNAL(sliderReleased()),this,SIGNAL(Updated()));
	connect(BlueColorSlider,SIGNAL(sliderReleased()),this,SIGNAL(Updated()));
	connect(SpecularLightingCoefSlider,SIGNAL(sliderReleased()),this,SIGNAL(Updated()));
	connect(SpecularPowerSlider,SIGNAL(sliderReleased()),this,SIGNAL(Updated()));
	connect(DiffuseLightingCoefSlider,SIGNAL(sliderReleased()),this,SIGNAL(Updated()));
	connect(AmbientLightingCoefSlider,SIGNAL(sliderReleased()),this,SIGNAL(Updated()));
	connect(OpacitySlider,SIGNAL(sliderReleased()),this,SIGNAL(Updated()));



	connect(FlatRadio,SIGNAL(clicked()),this,SLOT(SlotChangeInterpolationMode()));
	connect(GouraudRadio,SIGNAL(clicked()),this,SLOT(SlotChangeInterpolationMode()));
	connect(PhongRadio,SIGNAL(clicked()),this,SLOT(SlotChangeInterpolationMode()));

	connect(SurfaceRadio,SIGNAL(clicked()),this,SLOT(SlotChangeRepresentationMode()));
	connect(WireframeRadio,SIGNAL(clicked()),this,SLOT(SlotChangeRepresentationMode()));
	connect(PointsRadio,SIGNAL(clicked()),this,SLOT(SlotChangeRepresentationMode()));


}

PropertyWidget::~PropertyWidget()
{
//	 OriginalProperty->Delete();
	emit Deleted();
}
//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotColorClicked()
{

	QColor BckColor1=QColor(RedColorSlider->value(),GreenColorSlider->value(),BlueColorSlider->value());
	QColor BckColor=QColorDialog::getColor(BckColor1);
	if (BckColor.isValid()==true)
	{	
		RedColorSlider->setValue(BckColor.red());
		GreenColorSlider->setValue(BckColor.green());
		BlueColorSlider->setValue(BckColor.blue());
		emit Updated();
	}

}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotSetRedValue(int Red)
{

	QString text;
	text.sprintf("Red	(%d/255)",Red);	
	ColRedLabel->setText(text);	

	NewProperty->SetColor((double)(RedColorSlider->value()/255.0),
		(double)(GreenColorSlider->value()/255.0),(double)(BlueColorSlider->value()/255.0));

}

//--------------------------------------------
//--------------------------------------------



void PropertyWidget::SlotSetGreenValue(int Green)
{
	QString text;
	text.sprintf("Green	(%d/255)",Green);	
	ColGreenLabel->setText(text);

	NewProperty->SetColor((double)(RedColorSlider->value()/255.0),
		(double)(GreenColorSlider->value()/255.0),(double)(BlueColorSlider->value()/255.0));


}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotSetBlueValue(int Blue)
{
	QString text;
	text.sprintf("Blue	(%d/255)",Blue);	
	ColBlueLabel->setText(text);

	NewProperty->SetColor((double)(RedColorSlider->value()/255.0),
		(double)(GreenColorSlider->value()/255.0),(double)(BlueColorSlider->value()/255.0));

}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotSpecularClicked()
{
	double ColorRGB[3];	
	NewProperty->GetSpecularColor(ColorRGB);
	QColor Bck1Col=QColor((int)(ColorRGB[0]*255.0),(int)(ColorRGB[1]*255.0),(int)(ColorRGB[2]*255.0));
	QColor BckColor=QColorDialog::getColor(Bck1Col);
	if (BckColor.isValid()==true) 
	{
		ColorRGB[0]=BckColor.red()/255.0;
		ColorRGB[1]=BckColor.green()/255.0;
		ColorRGB[2]=BckColor.blue()/255.0;

		NewProperty->SetSpecularColor(ColorRGB);
		emit Updated();
	}
}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotSpecularValue(int Value)
{

	QString text;
	text.sprintf("Value	(%1.2f/1.00)",Value/100.0);	
	SpecularLightLabel->setText(text);


	NewProperty->SetSpecular(Value/100.0);

}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotSpecularPower(int Value)
{

	QString text;
	text.sprintf("Power	(%1.2f/1.00)",Value/100.0);	
	SpecularPowerLabel->setText(text);

	NewProperty->SetSpecular(Value/100.0);

}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotDiffuseClicked()
{
	double ColorRGB[3];	
	NewProperty->GetDiffuseColor(ColorRGB);
	QColor Bck1Col=QColor((int)(ColorRGB[0]*255.0),(int)(ColorRGB[1]*255.0),(int)(ColorRGB[2]*255.0));
	QColor BckColor=QColorDialog::getColor(Bck1Col);
	if (BckColor.isValid()==true) 
	{
		ColorRGB[0]=BckColor.red()/255.0;
		ColorRGB[1]=BckColor.green()/255.0;
		ColorRGB[2]=BckColor.blue()/255.0;

		NewProperty->SetDiffuseColor(ColorRGB);
		emit Updated();
	}

}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotDiffuseValue(int Value)
{
	QString text;
	text.sprintf("Value	(%1.2f/1.00)",Value/100.0);	
	DiffuseLightLabel->setText(text);

	NewProperty->SetDiffuse(Value/100.0);
	
}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotAmbientClicked()
{

	double ColorRGB[3];	
	NewProperty->GetAmbientColor(ColorRGB);
	QColor Bck1Col=QColor((int)(ColorRGB[0]*255.0),(int)(ColorRGB[1]*255.0),(int)(ColorRGB[2]*255.0));
	QColor BckColor=QColorDialog::getColor(Bck1Col);
	if (BckColor.isValid()==true) 
	{
		ColorRGB[0]=BckColor.red()/255.0;
		ColorRGB[1]=BckColor.green()/255.0;
		ColorRGB[2]=BckColor.blue()/255.0;

		NewProperty->SetAmbientColor(ColorRGB);
		emit Updated();
	}

}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotAmbientValue(int Value)
{

	QString text;
	text.sprintf("Value	(%1.2f/1.00)",Value/100.0);	
	AmbientLightLabel->setText(text);

	NewProperty->SetAmbient(Value/100.0);

}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotOpacityValue(int Value)
{
	QString text;
	text.sprintf("Value	(%1.2f/1.00)",Value/100.0);	
	OpacityLabel->setText(text);

	NewProperty->SetOpacity(Value/100.0);


}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotChangeInterpolationMode()
{

	if (FlatRadio->isChecked()==true)		NewProperty->SetInterpolationToFlat();
	if (GouraudRadio->isChecked()==true)	NewProperty->SetInterpolationToGouraud();
	if (PhongRadio->isChecked()==true)		NewProperty->SetInterpolationToPhong();
	emit Updated();
}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotChangeRepresentationMode()
{

	if (SurfaceRadio->isChecked()==true)	NewProperty->SetRepresentationToSurface (); 
	if (WireframeRadio->isChecked()==true)	NewProperty->SetRepresentationToWireframe (); 
	if (PointsRadio->isChecked()==true)		NewProperty->SetRepresentationToPoints ();
	emit Updated();
}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotApplyClicked()
{
	/*
	emit Updated();
		if (VWidget==NULL)
		this->hide();
	else 
	{
		CloseParam=false;
		this->close();
		
	}
	delete this;
*/

	this->hide();	
}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotCancelClicked()
{

//SetOld

	NewProperty->SetSpecularPower(OriginalProperty->GetSpecularPower());
	NewProperty->SetSpecular(OriginalProperty->GetSpecular());
	NewProperty->SetDiffuse(OriginalProperty->GetDiffuse());
	NewProperty->SetAmbient(OriginalProperty->GetAmbient());
	NewProperty->SetOpacity(OriginalProperty->GetOpacity());

	NewProperty->SetColor(OriginalProperty->GetColor());
	NewProperty->SetSpecularColor(OriginalProperty->GetSpecularColor());
	NewProperty->SetDiffuseColor(OriginalProperty->GetDiffuseColor());
	NewProperty->SetAmbientColor(OriginalProperty->GetAmbientColor());

	const char *Interpolation1 = OriginalProperty->GetInterpolationAsString();

	if (strcmp(Interpolation1,"Flat")==0) NewProperty->SetInterpolationToFlat();
	if (strcmp(Interpolation1,"Gouraud")==0) NewProperty->SetInterpolationToGouraud();
	if (strcmp(Interpolation1,"Phong")==0) NewProperty->SetInterpolationToPhong();



	const char *Representation1 = OriginalProperty->GetRepresentationAsString();

	if (strcmp(Representation1,"Surface")==0)	NewProperty->SetRepresentationToSurface();
	if (strcmp(Representation1,"Wireframe")==0)	NewProperty->SetRepresentationToWireframe();
	if (strcmp(Representation1,"Points")==0)	NewProperty->SetRepresentationToPoints();

	emit Updated();
	this->hide();


  }

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotSaveClicked()
{

}

//--------------------------------------------
//--------------------------------------------


void PropertyWidget::SlotSetInputProperty(vtkProperty *Property,const char *name,bool show)
{
		
//	if (name!=NULL) setCaption(name);

	OriginalProperty->DeepCopy(Property);
//	OriginalProperty->Update();

	NewProperty->DeepCopy(Property);
//	NewProperty->Update();



	SpecularPowerSlider->setValue((int)(NewProperty->GetSpecularPower()));
	SpecularLightingCoefSlider->setValue((int)(NewProperty->GetSpecular()*100.0));
	DiffuseLightingCoefSlider->setValue((int)(NewProperty->GetDiffuse()*100.0));
	AmbientLightingCoefSlider->setValue((int)(NewProperty->GetAmbient()*100.0));

	OpacitySlider->setValue((int)(NewProperty->GetOpacity()*100.0));

	double ColorRGB[3];

	NewProperty->GetColor(ColorRGB);

	
	RedColorSlider->setValue((int)(ColorRGB[0]*255.0));
	GreenColorSlider->setValue((int)(ColorRGB[1]*255.0));
	BlueColorSlider->setValue((int)(ColorRGB[2]*255.0));

	const char *Interpolation = NewProperty->GetInterpolationAsString();

	if (strcmp(Interpolation,"Flat")==0)	FlatRadio->setChecked(true);
	if (strcmp(Interpolation,"Gouraud")==0) GouraudRadio->setChecked(true);
	if (strcmp(Interpolation,"Phong")==0) PhongRadio->setChecked(true);

	const char *Representation = NewProperty->GetRepresentationAsString();

	if (strcmp(Representation,"Surface")==0)	SurfaceRadio->setChecked(true);
	if (strcmp(Representation,"Wireframe")==0)	WireframeRadio->setChecked(true);
	if (strcmp(Representation,"Points")==0)		PointsRadio->setChecked(true);

	if (show==true)
	{
		this->show();
		this->raise();
	}

}


//--------------------------------------------
//--------------------------------------------

//--------------------------------------------

void PropertyWidget::SlotUpdateExternalProperty(vtkProperty *Prop)
{

	
	Prop->SetSpecularPower(NewProperty->GetSpecularPower());
	Prop->SetSpecular(NewProperty->GetSpecular());
	Prop->SetDiffuse(NewProperty->GetDiffuse());
	Prop->SetAmbient(NewProperty->GetAmbient());
	Prop->SetOpacity(NewProperty->GetOpacity());

	Prop->SetColor(NewProperty->GetColor());
	Prop->SetSpecularColor(NewProperty->GetSpecularColor());
	Prop->SetDiffuseColor(NewProperty->GetDiffuseColor());
	Prop->SetAmbientColor(NewProperty->GetAmbientColor());

	const char *Interpolation1 = NewProperty->GetInterpolationAsString();
	if (strcmp(Interpolation1,"Flat")==0) Prop->SetInterpolationToFlat();
	if (strcmp(Interpolation1,"Gouraud")==0) Prop->SetInterpolationToGouraud();
	if (strcmp(Interpolation1,"Phong")==0) Prop->SetInterpolationToPhong();

	const char *Representation1 = NewProperty->GetRepresentationAsString();
	if (strcmp(Representation1,"Surface")==0)	Prop->SetRepresentationToSurface();
	if (strcmp(Representation1,"Wireframe")==0)	Prop->SetRepresentationToWireframe();
	if (strcmp(Representation1,"Points")==0)	Prop->SetRepresentationToPoints();


}

//--------------------------------------------

void PropertyWidget::SlotUpdateWidget()
{
//	if (VWidget!=NULL)
//	VWidget->RepaintWidget();
//TANDETA!!!!!!ZMIENIC jak wywalisz SurfaceData
}

//-----------------------------
void PropertyWidget::closeEvent( QCloseEvent *event )
{

	this->hide();
}