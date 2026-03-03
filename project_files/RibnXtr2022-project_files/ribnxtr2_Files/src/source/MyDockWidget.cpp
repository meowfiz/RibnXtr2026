

#include "MyDockWidget.h"

#include <QAction>
#include <QtEvents>
#include <QFrame>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QDialog>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPainterPath>
#include <QPushButton>
#include <QHBoxLayout>
#include <QBitmap>
#include <QtDebug>


 void MyDockWidget::mousePressEvent(QMouseEvent* event)
{
	 qDebug() << event->x() << " " << event->y();
	 if (event->y()<15)
	emit closeButtonClicked(this->objectName());
}
void MyDockWidget::slotDockLocationChanged(Qt::DockWidgetArea a)
{
	emit dockLocationChanged(a,this->objectName());
}
//--------------
MyDockWidget::MyDockWidget(const QString &_name, const QString &_title, bool placeOnLeft , QWidget *parent, Qt::WindowFlags flags)
	: QDockWidget(parent, flags)
{
	setAttribute(Qt::WA_AlwaysShowToolTips);
	setObjectName(_name + QLatin1String(" Dock Widget"));
	ww=0;hh=0;
	tmpWidget=new QWidget(this);
	tmpWidget->setFixedHeight(0);
	//QWidget *bar=new MyTitleBar(this);
	//setTitleBarWidget(bar);
	//bar->layout()->SetDefaultConstraint;
	//this->layout()->SetDefaultConstraint;
    //connect(this, SIGNAL(topLevelChanged(bool)), bar, SLOT(updateMask()));
    //connect(this, SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)), bar, SLOT(updateMask()));
	//connect(bar,SIGNAL(showHideContent()),this, SLOT (showHideContent()));

	setWindowTitle(_title);
	setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
	setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
	this->tmpTitleBarWidget=new QWidget(this);
	this->origTitleBarWidget=titleBarWidget();

	scrollArea=new QScrollArea(this);
	scrollArea->setWidgetResizable(true);
	////sArea->setWidget(Info);
	this->setWidget(scrollArea);

	
	//QWidget* titleWidget = new QWidget(this); /* where this a QMainWindow object */
	//setTitleBarWidget( titleWidget );
	/* setObjectName(colorName + QLatin1String(" Dock Widget"));
	setWindowTitle(objectName() + QLatin1String(" [*]"));

	QFrame *swatch = new ColorDock(colorName, this);
	swatch->setFrameStyle(QFrame::Box | QFrame::Sunken);*/

	//    setWidget(swatch);
	wName=_title;

	changeSizeHintsAction = new QAction(tr("Change Size Hints"), this);
	//    connect(changeSizeHintsAction, SIGNAL(triggered()), swatch, SLOT(changeSizeHints()));

	closableAction = new QAction(tr("Closable"), this);
	closableAction->setCheckable(true);
	connect(closableAction, SIGNAL(triggered(bool)), SLOT(changeClosable(bool)));

	movableAction = new QAction(tr("Movable"), this);
	movableAction->setCheckable(true);
	connect(movableAction, SIGNAL(triggered(bool)), SLOT(changeMovable(bool)));

	floatableAction = new QAction(tr("Floatable"), this);
	floatableAction->setCheckable(true);
	connect(floatableAction, SIGNAL(triggered(bool)), SLOT(changeFloatable(bool)));

	verticalTitleBarAction = new QAction(tr("Vertical title bar"), this);
	verticalTitleBarAction->setCheckable(true);
	connect(verticalTitleBarAction, SIGNAL(triggered(bool)),
		SLOT(changeVerticalTitleBar(bool)));

	floatingAction = new QAction(tr("Floating"), this);
	floatingAction->setCheckable(true);
	connect(floatingAction, SIGNAL(triggered(bool)), SLOT(changeFloating(bool)));

	allowedAreasActions = new QActionGroup(this);
	allowedAreasActions->setExclusive(false);

	allowLeftAction = new QAction(tr("Allow on Left"), this);
	allowLeftAction->setCheckable(true);
	connect(allowLeftAction, SIGNAL(triggered(bool)), SLOT(allowLeft(bool)));

	allowRightAction = new QAction(tr("Allow on Right"), this);
	allowRightAction->setCheckable(true);
	connect(allowRightAction, SIGNAL(triggered(bool)), SLOT(allowRight(bool)));

	//allow//topAction = new QAction(tr("Allow on Top"), this);
	//allow//topAction->setCheckable(true);
	//connect(allow//topAction, SIGNAL(triggered(bool)), SLOT(allowTop(bool)));

	//allow//bottomAction = new QAction(tr("Allow on Bottom"), this);
	//allow//bottomAction->setCheckable(true);
	//connect(allow//bottomAction, SIGNAL(triggered(bool)), SLOT(allowBottom(bool)));

	allowedAreasActions->addAction(allowLeftAction);
	allowedAreasActions->addAction(allowRightAction);
	/*allowedAreasActions->addAction(allow//topAction);
	allowedAreasActions->addAction(allow//bottomAction);*/

	areaActions = new QActionGroup(this);
	areaActions->setExclusive(true);

	leftAction = new QAction(tr("Place on Left") , this);
	leftAction->setCheckable(true);
	connect(leftAction, SIGNAL(triggered(bool)), SLOT(placeLeft(bool)));

	rightAction = new QAction(tr("Place on Right") , this);
	rightAction->setCheckable(true);
	connect(rightAction, SIGNAL(triggered(bool)), SLOT(placeRight(bool)));

	/*//topAction = new QAction(tr("Place on Top") , this);
	//topAction->setCheckable(true);
	connect(//topAction, SIGNAL(triggered(bool)), SLOT(placeTop(bool)));

	//bottomAction = new QAction(tr("Place on Bottom") , this);
	//bottomAction->setCheckable(true);
	connect(//bottomAction, SIGNAL(triggered(bool)), SLOT(placeBottom(bool)));*/

	areaActions->addAction(leftAction);
	areaActions->addAction(rightAction);
	//areaActions->addAction(//topAction);
	//areaActions->addAction(//bottomAction);

	connect(movableAction, SIGNAL(triggered(bool)), areaActions, SLOT(setEnabled(bool)));

	connect(movableAction, SIGNAL(triggered(bool)), allowedAreasActions, SLOT(setEnabled(bool)));

	connect(floatableAction, SIGNAL(triggered(bool)), floatingAction, SLOT(setEnabled(bool)));

	connect(floatingAction, SIGNAL(triggered(bool)), floatableAction, SLOT(setDisabled(bool)));
	connect(movableAction, SIGNAL(triggered(bool)), floatableAction, SLOT(setEnabled(bool)));

	tabMenu = new QMenu(this);
	tabMenu->setTitle(tr("Tab into"));
	connect(tabMenu, SIGNAL(triggered(QAction*)), this, SLOT(tabInto(QAction*)));

	splitHMenu = new QMenu(this);
	splitHMenu->setTitle(tr("Split horizontally into"));
	connect(splitHMenu, SIGNAL(triggered(QAction*)), this, SLOT(splitInto(QAction*)));

	splitVMenu = new QMenu(this);
	splitVMenu->setTitle(tr("Split vertically into"));
	connect(splitVMenu, SIGNAL(triggered(QAction*)), this, SLOT(splitInto(QAction*)));

	windowModifiedAction = new QAction(tr("Modified"), this);
	windowModifiedAction->setCheckable(true);
	windowModifiedAction->setChecked(false);
	connect(windowModifiedAction, SIGNAL(toggled(bool)), this, SLOT(setWindowModified(bool)));

	/*menu = new QMenu(wName, this);
	menu->addAction(toggleViewAction());
	QAction *action = menu->addAction(tr("Raise"));
	connect(action, SIGNAL(triggered()), this, SLOT(raise()));
	menu->addAction(changeSizeHintsAction);
	menu->addSeparator();
	menu->addAction(closableAction);
	menu->addAction(movableAction);
	menu->addAction(floatableAction);
	menu->addAction(floatingAction);
	menu->addAction(verticalTitleBarAction);
	menu->addSeparator();
	menu->addActions(allowedAreasActions->actions());
	menu->addSeparator();
	menu->addActions(areaActions->actions());
	menu->addSeparator();
	menu->addMenu(splitHMenu);
	menu->addMenu(splitVMenu);
	menu->addMenu(tabMenu);
	menu->addSeparator();
	menu->addAction(windowModifiedAction);*/

	//connect(menu, SIGNAL(aboutToShow()), this, SLOT(updateContextMenu()));

	connect(this,	SIGNAL(	dockLocationChanged ( Qt::DockWidgetArea)	), this, SLOT	(slotDockLocationChanged(Qt::DockWidgetArea))	);

	if (placeOnLeft == true) placeLeft(true);

	//changeFloatable(true);
}
void	MyDockWidget::setScrollWidget(QWidget *w)
{
	this->scrollArea->setWidget(w);
}
//-----------------
void MyDockWidget::SlotSetTitleBarVisibility(bool on)
{
	if (on==true)
	{
		setTitleBarWidget(this->origTitleBarWidget);
	}
	else
	{
		setTitleBarWidget(this->tmpTitleBarWidget);
	}
}
//---------------
void MyDockWidget::updateContextMenu()
{
	QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
	const Qt::DockWidgetArea area = mainWindow->dockWidgetArea(this);
	const Qt::DockWidgetAreas areas = allowedAreas();

	closableAction->setChecked(features() & QDockWidget::DockWidgetClosable);
	if (windowType() == Qt::Drawer) {
		floatableAction->setEnabled(false);
		floatingAction->setEnabled(false);
		movableAction->setEnabled(false);
		verticalTitleBarAction->setChecked(false);
	} else {
		floatableAction->setChecked(features() & QDockWidget::DockWidgetFloatable);
		floatingAction->setChecked(isWindow());
		// done after floating, to get 'floatable' correctly initialized
		movableAction->setChecked(features() & QDockWidget::DockWidgetMovable);
		verticalTitleBarAction
			->setChecked(features() & QDockWidget::DockWidgetVerticalTitleBar);
	}

	allowLeftAction->setChecked(isAreaAllowed(Qt::LeftDockWidgetArea));
	allowRightAction->setChecked(isAreaAllowed(Qt::RightDockWidgetArea));
	/*allow//topAction->setChecked(isAreaAllowed(Qt::TopDockWidgetArea));
	allow//bottomAction->setChecked(isAreaAllowed(Qt::BottomDockWidgetArea));*/

	if (allowedAreasActions->isEnabled()) {
		allowLeftAction->setEnabled(area != Qt::LeftDockWidgetArea);
		allowRightAction->setEnabled(area != Qt::RightDockWidgetArea);
	/*	allow//topAction->setEnabled(area != Qt::TopDockWidgetArea);
		allow//bottomAction->setEnabled(area != Qt::BottomDockWidgetArea);*/
	}

	leftAction->blockSignals(true);
	rightAction->blockSignals(true);
	//topAction->blockSignals(true);
	//bottomAction->blockSignals(true);

	leftAction->setChecked(area == Qt::LeftDockWidgetArea);
	rightAction->setChecked(area == Qt::RightDockWidgetArea);
	//topAction->setChecked(area == Qt::TopDockWidgetArea);
	//bottomAction->setChecked(area == Qt::BottomDockWidgetArea);

	leftAction->blockSignals(false);
	rightAction->blockSignals(false);
	//topAction->blockSignals(false);
	//bottomAction->blockSignals(false);

	if (areaActions->isEnabled()) {
		leftAction->setEnabled(areas & Qt::LeftDockWidgetArea);
		rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
		//topAction->setEnabled(areas & Qt::TopDockWidgetArea);
		//bottomAction->setEnabled(areas & Qt::BottomDockWidgetArea);
	}

	tabMenu->clear();
	splitHMenu->clear();
	splitVMenu->clear();
	QList<MyDockWidget*> dock_list = mainWindow->findChildren<MyDockWidget*>();
	foreach (MyDockWidget *dock, dock_list) {
		//        if (!dock->isVisible() || dock->isFloating())
		//            continue;
		tabMenu->addAction(dock->objectName());
		splitHMenu->addAction(dock->objectName());
		splitVMenu->addAction(dock->objectName());
	}
}

void MyDockWidget::splitInto(QAction *action)
{
	QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
	QList<MyDockWidget*> dock_list = mainWindow->findChildren<MyDockWidget*>();
	MyDockWidget *target = 0;
	foreach (MyDockWidget *dock, dock_list) {
		if (action->text() == dock->objectName()) {
			target = dock;
			break;
		}
	}
	if (target == 0)
		return;

	Qt::Orientation o = action->parent() == splitHMenu
		? Qt::Horizontal : Qt::Vertical;
	mainWindow->splitDockWidget(target, this, o);
}

void MyDockWidget::tabInto(QAction *action)
{
	QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
	QList<MyDockWidget*> dock_list = mainWindow->findChildren<MyDockWidget*>();
	MyDockWidget *target = 0;
	foreach (MyDockWidget *dock, dock_list) {
		if (action->text() == dock->objectName()) {
			target = dock;
			break;
		}
	}
	if (target == 0)
		return;

	mainWindow->tabifyDockWidget(target, this);
}

void MyDockWidget::contextMenuEvent(QContextMenuEvent *event)
{
	//event->accept();
	//menu->exec(event->globalPos());
}

void MyDockWidget::resizeEvent(QResizeEvent *e)
{
	 if (MyTitleBar *btb = qobject_cast<MyTitleBar*>(titleBarWidget()))
		btb->updateMask();
	
	QDockWidget::resizeEvent(e);
}


void MyDockWidget::allow(Qt::DockWidgetArea area, bool a)
{
	Qt::DockWidgetAreas areas = allowedAreas();
	areas = a ? areas | area : areas & ~area;
	setAllowedAreas(areas);

	if (areaActions->isEnabled()) {
		leftAction->setEnabled(areas & Qt::LeftDockWidgetArea);
		rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
		//topAction->setEnabled(areas & Qt::TopDockWidgetArea);
		//bottomAction->setEnabled(areas & Qt::BottomDockWidgetArea);
	}
}

void MyDockWidget::place(Qt::DockWidgetArea area, bool p)
{
	if (!p) return;

	QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
	mainWindow->addDockWidget(area, this);
	
	if (allowedAreasActions->isEnabled()) {
		allowLeftAction->setEnabled(area != Qt::LeftDockWidgetArea);
		allowRightAction->setEnabled(area != Qt::RightDockWidgetArea);
		/*allow//topAction->setEnabled(area != Qt::TopDockWidgetArea);
		allow//bottomAction->setEnabled(area != Qt::BottomDockWidgetArea);*/
	}
}

//void MyDockWidget::setCustomSizeHint(const QSize &size)
//{
//    if (ColorDock *dock = qobject_cast<ColorDock*>(widget()))
//        dock->setCustomSizeHint(size);
//}

void MyDockWidget::changeClosable(bool on)
{ setFeatures(on ? features() | DockWidgetClosable : features() & ~DockWidgetClosable); }

void MyDockWidget::changeMovable(bool on)
{ setFeatures(on ? features() | DockWidgetMovable : features() & ~DockWidgetMovable); }

void MyDockWidget::changeFloatable(bool on)
{ setFeatures(on ? features() | DockWidgetFloatable : features() & ~DockWidgetFloatable); }

void MyDockWidget::changeFloating(bool floating)
{ setFloating(floating); }

void MyDockWidget::allowLeft(bool a)
{ allow(Qt::LeftDockWidgetArea, a); }

void MyDockWidget::allowRight(bool a)
{ allow(Qt::RightDockWidgetArea, a); }

void MyDockWidget::allowTop(bool a)
{ allow(Qt::TopDockWidgetArea, a); }

void MyDockWidget::allowBottom(bool a)
{ allow(Qt::BottomDockWidgetArea, a); }

void MyDockWidget::placeLeft(bool p)
{ place(Qt::LeftDockWidgetArea, p); }

void MyDockWidget::placeRight(bool p)
{ place(Qt::RightDockWidgetArea, p); }

void MyDockWidget::placeTop(bool p)
{ place(Qt::TopDockWidgetArea, p); }

void MyDockWidget::placeBottom(bool p)
{ place(Qt::BottomDockWidgetArea, p); }

void MyDockWidget::changeVerticalTitleBar(bool on)
{
	this->widget()->setVisible(!this->widget()->isVisible());
	setFeatures(on ? features() | DockWidgetVerticalTitleBar
		: features() & ~DockWidgetVerticalTitleBar);
}
//-----------------
 void MyDockWidget::hideEvent  ( QCloseEvent *event )
{
	//QMessageBox::about( 0,"","hide");
	//nothing
	//event->ignore();
	
}
  void MyDockWidget::closeEvent  ( QCloseEvent *event )
{
	//nothing
	//QMessageBox::about( 0,"","close");
	//showHideContent();
	emit closeButtonClicked(this->objectName());
	event->ignore();
	
}
///---------------
  void	MyDockWidget::setContentVisible(bool on)
  {
	if (on==false)
	{
		//jakos musze schowac tego scrolla
		this->scrollArea->widget()->setVisible(false);
		ww=this->width();
		hh=this->height();
		scrollArea->setFixedHeight(0);
	}
	else
	{
		//pokaz scrolla z powrotem
		scrollArea->widget()->setVisible(true);
		this->setFixedSize(ww,hh);
		scrollArea->setFixedSize(ww,hh);
		scrollArea->setMinimumSize(0,0);		scrollArea->setMaximumSize(16777215,16777215);
		this->setMinimumSize(0,0);		this->setMaximumSize(16777215,16777215);
	}
	//QWidget *tmp=this->scrollArea->widget();
	//this->scrollArea->setWidget(tmpWidget);
	//tmpWidget=tmp;
	//tmp=NULL;

  }
  //--------------------------
 void MyDockWidget::showHideContent()
{
	//this->widget()->setVisible(!this->widget()->isVisible());
	bool vis=scrollArea->widget()->isVisible();
	if (vis==true)
	{
		//jakos musze schowac tego scrolla
		this->scrollArea->widget()->setVisible(!vis);
		ww=this->width();
		hh=this->height();
		scrollArea->setFixedHeight(0);
	}
	else
	{
		//pokaz scrolla z powrotem
		scrollArea->widget()->setVisible(!vis);
		this->setFixedSize(ww,hh);
		scrollArea->setFixedSize(ww,hh);
		scrollArea->setMinimumSize(0,0);		scrollArea->setMaximumSize(16777215,16777215);
		this->setMinimumSize(0,0);		this->setMaximumSize(16777215,16777215);
	}
	//QWidget *tmp=this->scrollArea->widget();
	//this->scrollArea->setWidget(tmpWidget);
	//tmpWidget=tmp;
	//tmp=NULL;
}

MyViewWidget::MyViewWidget(QString _title, QWidget *parent , Qt::WindowFlags flags ) : QWidget(parent, flags)
{
	//setWidget(NULL);
	//this->setDockNestingEnabled(true);
	hide();

	int i;
	for(i=0;i<6;i++)
	{
		this->myWidget[i]=new QWidget(this);
	}

	currentLayout=0;
	//QStyle *styles=new QMotifStyle ();

	setLayout( new QVBoxLayout ());//,1, -1, "AllViewsLayout");
	layout()->setMargin(0);
	layout()->setSpacing(0);


	MainHorizontalSplitter= new 	QSplitter( Qt::Horizontal,this );
	MainVerticalSplitter= new 	QSplitter( Qt::Vertical, this );

	layout()->addWidget(MainVerticalSplitter);
	layout()->addWidget(MainHorizontalSplitter);



	VerticalSplitter1= new 	QSplitter( Qt::Vertical, 0 );
	VerticalSplitter2= new 	QSplitter( Qt::Vertical, 0 );
	HorizontalSplitter1= new 	QSplitter( Qt::Horizontal, 0 );
	HorizontalSplitter2= new 	QSplitter( Qt::Horizontal, 0 );

	//MainHorizontalSplitter->setStyle(styles);
	//MainVerticalSplitter->setStyle(styles);
	//VerticalSplitter1->setStyle(styles);
	//VerticalSplitter2->setStyle(styles);
	//HorizontalSplitter1->setStyle(styles);
	//HorizontalSplitter2->setStyle(styles);

}

//--------------


void	MyViewWidget::setSubWidget(int nr,QWidget *w)
{
	//MyViewWidget
	if ((w!=NULL) &&(nr>=0)&&(nr<6))
	{
		if (this->isVisible()==true)
		{
			FreeAllWidgets();
			delete myWidget[nr];
			myWidget[nr]=w;
			SlotSetLayout(currentLayout);



		}
		else
		{
			delete myWidget[nr];
			myWidget[nr]=w;
		}
	}
}
//--------------------	
QWidget *MyViewWidget::getSubWidget(int nr)
{
	if ((nr>=0)&&(nr<6))
	{
		return myWidget[nr];
	}
	else
	{
		return NULL;
	}
}
//--------------------


//---------------

//private:
//QDockWidget *myWidgets[5];
//int currentLayout;

void MyViewWidget::ShowAndResetFixedSizeOfDockWidgets(char *tab)
{
	/*QWidget *w[6];
	w[0]=VWidget;
	w[1]=GUIPanel;
	w[2]=TransversalView;
	w[3]=SagittalView;
	w[4]=CoronalView;
	w[5]=Histogram;*/

	int i;

	for(i=0;i<5;i++)
	{
		if (tab[i]==1)
		{
			myWidget[i]->show();		
		}
	}
	for(i=0;i<5;i++)
	{
		if (tab[i]==1)
		{
			myWidget[i]->setMinimumSize(0,0);		myWidget[i]->setMaximumSize(16777215,16777215);
		}
	}

}
//----------
void MyViewWidget::FreeAllWidgets()
{


	int nr=5;

	int i;
	/*for(i=0;i<nr;i++)
	{

	mainWindow->removeDockWidget(myWidget[i]);
	}
	for(i=0;i<nr;i++)
	{

	mainWindow->layout()->removeWidget(myWidget[i]);
	}*/

	MainHorizontalSplitter->hide();
	MainVerticalSplitter->hide();
	VerticalSplitter1->setParent(0);
	VerticalSplitter2->setParent(0);
	HorizontalSplitter1->setParent(0);
	HorizontalSplitter2->setParent(0);;

	for(i=0;i<5;i++)
	{
		myWidget[i]->setParent(0);
	}
	/*TransversalView->setParent(0);
	SagittalView->setParent(0);
	CoronalView->setParent(0);
	Histogram->setParent(0);
	VWidget->setParent(0);
	*/


}
//----------
void MyViewWidget::SlotSetLayout(int i)
{


	QWidget *VWidget=myWidget[0];
	QWidget *TransversalView=myWidget[1];
	QWidget *SagittalView=myWidget[2];
	QWidget *CoronalView=myWidget[3];
	QWidget *Histogram=myWidget[4];

	SignalBlockViewWidgets(false);
	const QPoint Point;
	FreeAllWidgets();
	//QList<int> qValue;
	//			qValue.append(500);
	//			qValue.append(100);

	QList<int> qValue;
	QList<int> qValue1;
	QList<int> qValue2;


	int Width=(int)((float)this->width()/100.0);
	int Height=(int)((float)this->height()/100.0);

	int a;
	for (a=0;a<=5;a++)
	{
		//		WidgetMenu->setItemEnabled(a,true);
		//	WidgetMenu->setItemChecked (a,true);

	}


	switch(i)
	{
	case 0:

		MainVerticalSplitter->show();
		VWidget->setParent(MainVerticalSplitter);VWidget->show();
		HorizontalSplitter1->setParent(MainVerticalSplitter);HorizontalSplitter1->show();		
		TransversalView->setParent(HorizontalSplitter1);TransversalView->show();
		SagittalView->setParent(HorizontalSplitter1);SagittalView->show();
		CoronalView->setParent(HorizontalSplitter1);CoronalView->show();
		Histogram->setParent(MainVerticalSplitter);Histogram->show();



		qValue.append(Height*60);
		qValue.append(Height*25);
		qValue.append(Height*15);
		MainVerticalSplitter->setSizes(qValue) ;

		qValue1.append(Width*33);
		qValue1.append(Width*33);
		qValue1.append(Width*33);
		HorizontalSplitter1->setSizes(qValue1) ;


		break;

	case 1:


		MainVerticalSplitter->show();

		HorizontalSplitter1->setParent(MainVerticalSplitter);HorizontalSplitter1->show();		
		TransversalView->setParent(HorizontalSplitter1);
		TransversalView->show();
		SagittalView->setParent(HorizontalSplitter1);
		SagittalView->show();
		CoronalView->setParent(HorizontalSplitter1);
		CoronalView->show();
		VWidget->setParent(MainVerticalSplitter);
		VWidget->show();
		Histogram->setParent(MainVerticalSplitter);
		Histogram->show();



		qValue.append(Height*25);
		qValue.append(Height*60);
		qValue.append(Height*15);

		MainVerticalSplitter->setSizes(qValue) ;

		qValue1.append(Width*33);
		qValue1.append(Width*33);
		qValue1.append(Width*33);
		HorizontalSplitter1->setSizes(qValue1) ;


		break;

	case 2:

		MainHorizontalSplitter->show();

		VerticalSplitter1->setParent(MainHorizontalSplitter);VerticalSplitter1->show();	

		TransversalView->setParent(VerticalSplitter1);TransversalView->show();
		SagittalView->setParent(VerticalSplitter1);SagittalView->show();
		CoronalView->setParent(VerticalSplitter1);CoronalView->show();

		VerticalSplitter2->setParent(MainHorizontalSplitter);VerticalSplitter2->show();		

		VWidget->setParent(VerticalSplitter2);VWidget->show();
		Histogram->setParent(VerticalSplitter2);Histogram->show();



		qValue.append(Width*30);
		qValue.append(Width*70);

		MainHorizontalSplitter->setSizes(qValue);

		qValue1.append(Height*33);
		qValue1.append(Height*33);
		qValue1.append(Height*33);

		VerticalSplitter1->setSizes(qValue1);

		qValue2.append(Height*80);
		qValue2.append(Height*20);

		VerticalSplitter2->setSizes(qValue2);



		break;


	case 3:

		MainHorizontalSplitter->show();
		VerticalSplitter2->setParent(MainHorizontalSplitter);VerticalSplitter2->show();		

		VWidget->setParent(VerticalSplitter2);VWidget->show();
		Histogram->setParent(VerticalSplitter2);Histogram->show();

		VerticalSplitter1->setParent(MainHorizontalSplitter);VerticalSplitter1->show();	

		TransversalView->setParent(VerticalSplitter1);TransversalView->show();
		SagittalView->setParent(VerticalSplitter1);SagittalView->show();
		CoronalView->setParent(VerticalSplitter1);CoronalView->show();



		qValue.append(Width*70);
		qValue.append(Width*30);


		MainHorizontalSplitter->setSizes(qValue);

		qValue1.append(Height*33);
		qValue1.append(Height*33);
		qValue1.append(Height*33);

		VerticalSplitter1->setSizes(qValue1);

		qValue2.append(Height*80);
		qValue2.append(Height*20);

		VerticalSplitter2->setSizes(qValue2);

		break;


	case 4:
		//SlotSetNewLayout(int i)
		MainVerticalSplitter->show();
		HorizontalSplitter1->setParent(MainVerticalSplitter);HorizontalSplitter1->show();		
		TransversalView->setParent(HorizontalSplitter1);TransversalView->show();
		VWidget->setParent(HorizontalSplitter1);VWidget->show();
		HorizontalSplitter2->setParent(MainVerticalSplitter);HorizontalSplitter2->show();
		SagittalView->setParent(HorizontalSplitter2);SagittalView->show();
		CoronalView->setParent(HorizontalSplitter2);CoronalView->show();
		Histogram->setParent(MainVerticalSplitter);Histogram->show();

		qValue.append(Height*42);
		qValue.append(Height*42);
		qValue.append(Height*16);

		MainVerticalSplitter->setSizes(qValue);
		qValue1.append(Width*50);
		qValue1.append(Width*50);
		HorizontalSplitter1->setSizes(qValue1);
		HorizontalSplitter2->setSizes(qValue1);

		break;


	case 5:
		MainVerticalSplitter->show();
		VWidget->setParent(MainVerticalSplitter);VWidget->show();

		/*for (a=0;a<5;a++)
		{
		WidgetMenu->setItemChecked (a,false);
		WidgetMenu->setItemEnabled(a,false);
		}
		WidgetMenu->setItemChecked (5,true);
		WidgetMenu->setItemEnabled(5,false);*/

		break;


	case 6:

		MainVerticalSplitter->show();
		TransversalView->setParent(MainVerticalSplitter);TransversalView->show();
		Histogram->setParent(MainVerticalSplitter);Histogram->show();
		qValue.append(Height*90);
		qValue.append(Height*10);
		MainVerticalSplitter->setSizes(qValue);
		VWidget->hide();

		break;

	case 7:
		MainVerticalSplitter->show();
		SagittalView->setParent(MainVerticalSplitter);SagittalView->show();
		Histogram->setParent(MainVerticalSplitter);Histogram->show();
		qValue.append(Height*90);
		qValue.append(Height*10);
		MainVerticalSplitter->setSizes(qValue);
		VWidget->hide();
		break;


	case 8:

		MainVerticalSplitter->show();
		CoronalView->setParent(MainVerticalSplitter);CoronalView->show();
		Histogram->setParent(MainVerticalSplitter);Histogram->show();
		qValue.append(Height*90);
		qValue.append(Height*10);
		MainVerticalSplitter->setSizes(qValue);
		VWidget->hide();
		break;
	}


	currentLayout=i;
}








QSize MyTitleBar::minimumSizeHint() const
{
    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);
    QSize result(leftPm.width() + rightPm.width(), centerPm.height());
    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar)
        result.transpose();
    return result;
}

MyTitleBar::MyTitleBar(QWidget *parent)
    : QWidget(parent)
{
	//this->Qt::WStyle_NormalBorder
    leftPm = QPixmap("images/titlebarLeft.png");
    centerPm = QPixmap("images/titlebarCenter.png");
    rightPm = QPixmap("images/titlebarRight.png");
}

void MyTitleBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QRect rect = this->rect();

    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);

    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
        QSize s = rect.size();
        s.transpose();
        rect.setSize(s);

        painter.translate(rect.left(), rect.top() + rect.width());
        painter.rotate(-90);
        painter.translate(-rect.left(), -rect.top());
    }

    painter.drawPixmap(rect.topLeft(), leftPm);
    painter.drawPixmap(rect.topRight() - QPoint(rightPm.width() - 1, 0), rightPm);
    QBrush brush(centerPm);
    painter.fillRect(rect.left() + leftPm.width(), rect.top(),
                        rect.width() - leftPm.width() - rightPm.width(),
                        centerPm.height(), centerPm);
}

void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    QRect rect = this->rect();

    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);

    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
        QPoint p = pos;
        pos.setX(rect.left() + rect.bottom() - p.y());
        pos.setY(rect.top() + p.x() - rect.left());

        QSize s = rect.size();
        s.transpose();
        rect.setSize(s);
    }

    const int buttonRight = 7;
    const int buttonWidth = 20;
    int right = rect.right() - pos.x();
    int button = (right - buttonRight)/buttonWidth;
    switch (button) {
        case 0:
            event->accept();
   //         dw->close();
						emit showHideContent();
            break;
        case 1:
            event->accept();
            dw->setFloating(!dw->isFloating());
            break;
        case 2: {
            event->accept();
            QDockWidget::DockWidgetFeatures features = dw->features();
            if (features & QDockWidget::DockWidgetVerticalTitleBar)
                features &= ~QDockWidget::DockWidgetVerticalTitleBar;
            else
                features |= QDockWidget::DockWidgetVerticalTitleBar;
            dw->setFeatures(features);
            break;
        }
        default:
            event->ignore();
            break;
    }
}

void MyTitleBar::updateMask()
{
    QDockWidget *dw = qobject_cast<QDockWidget*>(parent());
    Q_ASSERT(dw != 0);

    QRect rect = dw->rect();
    QPixmap bitmap(dw->size());

    {
        QPainter painter(&bitmap);

        ///initialize to transparent
        painter.fillRect(rect, Qt::color0);

        QRect contents = rect;
        contents.setTopLeft(geometry().bottomLeft());
        contents.setRight(geometry().right());
        contents.setBottom(contents.bottom()-y());
        painter.fillRect(contents, Qt::color1);



        //let's pait the titlebar

        QRect titleRect = this->geometry();

        if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
            QSize s = rect.size();
            s.transpose();
            rect.setSize(s);

            QSize s2 = size();
            s2.transpose();
            titleRect.setSize(s2);

            painter.translate(rect.left(), rect.top() + rect.width());
            painter.rotate(-90);
            painter.translate(-rect.left(), -rect.top());
        }

        contents.setTopLeft(titleRect.bottomLeft());
        contents.setRight(titleRect.right());
        contents.setBottom(rect.bottom()-y());

        QRect rect = titleRect;


        painter.drawPixmap(rect.topLeft(), leftPm.mask());
        painter.fillRect(rect.left() + leftPm.width(), rect.top(),
            rect.width() - leftPm.width() - rightPm.width(),
            centerPm.height(), Qt::color1);
        painter.drawPixmap(rect.topRight() - QPoint(rightPm.width() - 1, 0), rightPm.mask());

        painter.fillRect(contents, Qt::color1);
    }

    dw->setMask(bitmap);
}


void MyTitleBar::resizeEvent(QResizeEvent *e)
{
	e->ignore();
}
