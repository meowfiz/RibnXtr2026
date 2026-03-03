#include "ParamsWidget.h"



ParamsWidget::ParamsWidget( QList<QSlider2*>* _sliderList , QWidget *parent) //: QWidget(parent)
{
	color[0] = -1;
	color[1] = -1;
	color[2] = -1;

	sliderList = _sliderList;
	installEventFilter(this);
	setMouseTracking(true);
	defaultStyle = styleSheet();
	//setAttribute(Qt::WA_AlwaysShowToolTips);
	valAddres = NULL;
	colAddres = NULL;
	QValidator *vd = new QRegExpValidator(QRegExp("^\-?\d\.?\d*(e\-?\d*)?$", Qt::CaseInsensitive));
	//mixerMaskFilterParamsWidgets[i] = new QWidget(mixerFilterWidget);

	this->setLayout(new QVBoxLayout());
	parent->layout()->addWidget(this);
	this->layout()->setMargin(0);

	paramsTopLabel = new QLabel(this);
	QFont font = QFont();
	font.setBold(true);
	paramsTopLabel->setFont(font);
	this->layout()->addWidget(paramsTopLabel);
	paramsTopLabel->hide();

	/*QColor color=qRgb(
		(rand()*255.0) / (double)RAND_MAX,
		(rand() * 255.0) / (double)RAND_MAX,
		(rand() * 255.0) / (double)RAND_MAX);
	std:string clr = "background-color: rgb(" + std::to_string(color.red()) + "," + std::to_string(color.green()) + "," + std::to_string(color.blue()) + "); border: 0px;";
	setStyleSheet(clr.c_str());*/

	QWidget *w = new QWidget(this);
	this->layout()->addWidget(w);
	w->setLayout(new QHBoxLayout());
	w->layout()->setMargin(1);

	//w->layout()->addWidget(paramsCheckBox);


	paramsCheckBox = new QCheckBox(w);
	w->layout()->addWidget(paramsCheckBox);

	paramsLabel = new QLabel(w);
	paramsLabel->setFixedWidth(70);

	w->layout()->addWidget(paramsLabel);
	//paramsLabel->setText("Pi" + QString::number(i) + ":");


	paramsLineEdit = new QLineEdit(w);
	//	paramsLineEdit->setValidator(vd);
	w->layout()->addWidget(paramsLineEdit);
	paramsLineEdit->setText("0.0");
	paramsLineEdit->setFixedWidth(40);

	paramsNameLabel = new QLabel(w);
	w->layout()->addWidget(paramsNameLabel);



	paramsSlider = new QSlider2(sliderList,Qt::Horizontal, w);
	paramsSlider->setFlag(0, 1);//ten jest do segmentation!!!
	paramsSlider->setRange(0, 100000);
	w->layout()->addWidget(paramsSlider);

	paramsComboBox = new QComboBox(w);
	w->layout()->addWidget(paramsComboBox);
	paramsComboBox->hide();

	SlotConnectAll();
	this->setVisible(false);
}

//---------------------------------------
ParamsWidget::~ParamsWidget()
{

}
void	ParamsWidget::SetTopLabel(const QString &text, bool visible)
{
	paramsTopLabel->setText(text);
	paramsTopLabel->setVisible(visible);

}
void	ParamsWidget::HideWidgetAndTopLabel()
{
	paramsTopLabel->hide();
	this->setVisible(false);
}
void ParamsWidget::SlotTextChanged()
{
	emit textChanged("");
	

}
//------------------------------------------------------------
void ParamsWidget::SlotConnectAll()
{

	//connect(paramsLineEdit, SIGNAL(returnPressed()), this, SLOT(SlotTextChanged()));
	connect(paramsLineEdit, SIGNAL(editingFinished()), this, SLOT(SlotTextChanged()));
	//	connect(paramsLineEdit, SIGNAL(textChanged(const QString &)), this, SIGNAL(textChanged(const QString &)));
	connect(paramsSlider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
	connect(paramsCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(stateChanged(int)));
	connect(paramsComboBox, SIGNAL(activated(int)), this, SLOT(SlotComboBoxActivated(int)));

}
//------------------------------------------------------------
void ParamsWidget::SlotDisconnectAll()
{
	disconnect(paramsLineEdit, SIGNAL(editingFinished()), this, SLOT(SlotTextChanged()));
	//disconnect(paramsLineEdit, SIGNAL(textChanged(const QString &)), this, SIGNAL(textChanged(const QString &)));
	disconnect(paramsSlider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
	disconnect(paramsCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(stateChanged(int)));
	disconnect(paramsComboBox, SIGNAL(activated(int)), this, SLOT(SlotComboBoxActivated(int)));

}
//-----------------------------------------------------------
void ParamsWidget::SlotComboBoxActivated(int val)
{
	paramsSlider->setValue(val);
}
//------------------------------------------------
void ParamsWidget::SetBackgroundColor(int r, int g, int b)
{
	if (r < 0)
	{
		setStyleSheet(defaultStyle);
}
	else
	{

		color[0] = r;
		color[1] = g;
		color[2] = b;
		QColor colorx = qRgb(r, g, b);
		/*	(rand() * 255.0) / (double)RAND_MAX,
			(rand() * 255.0) / (double)RAND_MAX,
			(rand() * 255.0) / (double)RAND_MAX);*/
	std:string clr = "background-color: rgb(" + std::to_string(colorx.red()) + "," + std::to_string(colorx.green()) + "," + std::to_string(colorx.blue()) + "); border: 0px;";
		setStyleSheet(clr.c_str());
	}
}
//-----------------------------
void ParamsWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() & Qt::RightButton)
	{
		int x = event->x();
		int y = this->height() - event->y();
		QPoint pt = mapToGlobal(QPoint(x, height() - y));
		int xx = pt.x();
		int yy = pt.y();
		QMenu* pMenu = new QMenu(this);
		QSignalMapper* sMapper = new QSignalMapper(this);
		QAction* act;
		pMenu->addAction(act = new QAction("Change Color ...", this));
		connect(act, SIGNAL(triggered()), this, SLOT(SlotSelectNewColor()));
		pMenu->popup(QPoint(xx, yy));
		//QMenu* arteryMenu = new QMenu("Object Selection", this);

		//connect(arteryMenu,SIGNAL(activated(int)),this,SLOT(SlotSetSurfaceRole(int)));
		//connect(arteryMenu, SIGNAL(triggered(QAction*)), this, SLOT(SlotSetSurfaceRole(QAction*)));
		
	}
}
//-----------------------------
void ParamsWidget::SlotSelectNewColor()
{
	QColor col1 = qRgb(color[0], color[1], color[2]);
	QColor col2 = QColorDialog::getColor(col1);
	if (col2.isValid() == true)
	{
		//col1 = col2;
		color[0] = col2.red();color[1] = col2.green();color[2] = col2.blue();

		SetBackgroundColor(color[0], color[1], color[2]);
		emit backgroundColorChanged(0);
		

	}
}
//---------------------------------
void ParamsWidget::UpdateSliderSettings(LayerGenericFilter *filter, int paramNr, int sel, double val, double *address,int *colAddress)// , RawDataLayer *l, int fmode )// int *ind, QString paramName, bool sliderTest, double *sliderParams, QString ttext, bool sel)
{
	SlotDisconnectAll();
	valAddres = address;
	colAddres = colAddress;
	//layer = l;
	//filterMode = fmode;
	
	//int ind[2];
	double sliderParams[3];
	//QString paramName;
	//filter->GetShowTextStartStop(paramNr, ind);
	QString ttext = filter->GetParamName(paramNr);
	//int v = this->paramsLineEdit->text().toInt() + ind[0];
	//	paramName = filter->GetShowText(paramNr,0);
	bool sliderTest = filter->GetSliderParams(paramNr, sliderParams);
	//bool sel = layer->GetFilterParamForGui(i);
	//double val = layer->GetFilterParam(i);
	prevParamNr = paramNr;

	if (sliderTest)
	{


		this->paramsCheckBox->setChecked(sel);

		double maxVal;
		if (sliderParams[2]>0)
		{
			maxVal = (sliderParams[1] - sliderParams[0]) / sliderParams[2];
			this->paramsSlider->setRange(0, maxVal);


		}
		else
		{
			maxVal = 100000;
			this->paramsSlider->setRange(0, maxVal);
		}

		//set slider value based on  lineedit settings

		this->paramsLineEdit->setText(QString::number(val));


		double minVal = 0;

		double sliderRange = maxVal;
		double paramsRange = sliderParams[1] - sliderParams[0];
		double sliderVal;
		if (paramsRange>0)
		{
			sliderVal = (val - sliderParams[0])*sliderRange / paramsRange;
			this->paramsSlider->setValue(sliderVal);
		}
		else
		{
			sliderVal = val;
			this->paramsSlider->setValue(val);
		}



		//sliderValues[i] = sliderVal; - nie wiem czy to do czegos potrzebne

		this->paramsSlider->setVisible(true);
	}
	else
	{
		//schowaj slider
		this->paramsSlider->setVisible(false);
	}
	int mode = filter->GetViewMode(paramNr);
	if (mode == 0)
	{
		//no slider
		paramsComboBox->hide();
		paramsSlider->hide();

	}
	//
	else if (mode == 1)
	{
		//slider

		//this->paramsLineEdit->hide();
		this->paramsLabel->show();
		paramsLineEdit->show();
		paramsNameLabel->show();
		paramsLabel->show();
		paramsComboBox->hide();

		textForTooltip = filter->GetShowText(paramNr, val);
		this->paramsNameLabel->setText(": " + textForTooltip);
		
		//setToolTip(textForTooltip);
	}
	if (filter->GetViewMode(paramNr) == 2)
	{
		//zrobie comboboxa
		paramsComboBox->show();
		this->paramsComboBox->clear();

		double sParams[3];
		filter->GetSliderParams(paramNr, sParams);

		int nrr = sParams[1] + 1;//maxvalue+1

		for (int i = 0; i < nrr; i++)
		{
			QString txt = filter->GetShowText(paramNr, i);
			paramsComboBox->addItem(txt, i);
		}
		paramsComboBox->setCurrentIndex(val);
		paramsLineEdit->hide();
		paramsNameLabel->hide();
		paramsSlider->hide();
		paramsLabel->show();
	}
	else
	{

		paramsComboBox->hide();
	}





	bool test = false;
	if ((((int)sliderParams[2]) == 1))//&& (ind[0] >= 0) && (ind[1] >= 0)) //jesli steps dla slidera jest ==1 i zdefiniowalem nazwy
	{
		//get param name

		// ((sliderParams[1] - sliderParams[0]) )== (ind[1] - ind[0]))
		{

			//this->paramsLineEdit->hide();
			//this->paramsLabel->show();
			test = true;



			//ttext+= ": "+paramName;
			this->paramsNameLabel->setText(": " + filter->GetShowText(paramNr, val));

			
			//mixerFilterParamsWidgets[i]->setVisible(true);
		}
	}


	this->paramsLabel->setText(ttext);

	textForTooltip = ttext;
	setToolTip(textForTooltip);

	this->setVisible(true);
	SlotConnectAll();

	prevFilter = filter;
	prevText = paramsLineEdit->text();
}
//------------------------------------
double ParamsWidget::SliderSettingsChanged(LayerGenericFilter *filter, int paramNr)
{
	SlotDisconnectAll();
	double val = 0;
	double sliderParams[3];
	if ((filter == NULL) && (paramNr == -1) && (valAddres != NULL))
	{
		filter = prevFilter;
		paramNr = prevParamNr;
		if (filter->GetSliderParams(paramNr, sliderParams))
		{
			double vv = *valAddres;
			//paramsLineEdit->text().toDouble();
			paramsLineEdit->setText(QString::number(vv));


			if (colAddres != NULL)
			{
				if (colAddres[0]>=0)
				this->SetBackgroundColor(colAddres[0], colAddres[1], colAddres[2]);
			}
			if ((vv >= sliderParams[0]) && (vv <= sliderParams[1]))
			{
				//miesci sie we wlascywych wartosciach
				val = vv;
				double maxVal = 0;
				if (sliderParams[2]>0)
				{
					maxVal = (sliderParams[1] - sliderParams[0]) / sliderParams[2];

				}
				else
				{
					maxVal = 100000;

				}

				double minVal = 0;
				double sliderRange = maxVal;


				double paramsRange = sliderParams[1] - sliderParams[0];
				double pr = (paramsRange > 0) ? paramsRange : 1;
				int sliderVal = ((val - sliderParams[0])*sliderRange) / pr;
				paramsSlider->setValue(sliderVal);

				if (filter->GetViewMode(paramNr) == 2)
				{
					paramsComboBox->setCurrentIndex(val);
				}


			}
		}

	}
	else
		if (filter->GetSliderParams(paramNr, sliderParams))
		{

			//wylicz na podstawie slidera wartosc 
			double maxVal;
			double sliderVal = paramsSlider->value();
			int mm0 = paramsSlider->minimum();
			int mm1 = paramsSlider->maximum();
			if (sliderParams[2]>0)
			{
				maxVal = (sliderParams[1] - sliderParams[0]) / sliderParams[2];
				paramsSlider->setRange(0, maxVal);
			}
			else
			{
				maxVal = 100000;
				paramsSlider->setRange(0, maxVal);
			}

			double minVal = 0;
			double sliderRange = maxVal;
			double paramsRange = sliderParams[1] - sliderParams[0];

			val = sliderParams[0] + sliderVal*(paramsRange / sliderRange);



			if (filter->GetViewMode(paramNr) == 2)
			{
				paramsComboBox->setCurrentIndex(val);
			}

		/*	int col[3], col2[3];
			filter->getPaGetMainParamsColors(i, col);*/


			if ((prevFilter == filter) && (prevText.compare(paramsLineEdit->text()) != 0))
			{

				//tekst ró¿ne zrobiê update - to oznacza ze wpisalem tekst recznie a nie sliderem

				double vv = paramsLineEdit->text().toDouble();

				if ((vv >= sliderParams[0]) && (vv <= sliderParams[1]))
				{
					//miesci sie we wlascywych wartosciach
					val = vv;
					//przesune jeszcze slider
					double pr = (paramsRange > 0) ? paramsRange : 1;
					int sliderVal = ((val - sliderParams[0])*sliderRange) / pr;
					paramsSlider->setValue(sliderVal);
				}


			}
			//else
			//{
			paramsLineEdit->setText(QString::number(val));
			//}

			val = paramsLineEdit->text().toDouble();;

			//layer->SetFilterParam(i, mixerFilterParamsLineEdits[i]->text().toDouble());


			//int ind[2];
			QString paramName;
			//filter->GetShowTextStartStop(paramNr, ind);
			QString ttext = filter->GetParamName(paramNr);

			bool test = false;

			if ((((int)sliderParams[2]) == 1))// && (ind[0] >= 0) && (ind[1] >= 0)) //jesli steps dla slidera jest ==1 i zdefiniowalem nazwy
			{
				//get param name

				//if ((sliderParams[1] - sliderParams[0]) == (ind[1] - ind[0]))
				{

					/*	paramsLineEdit->hide();
					paramsNameLabel->show();*/
					test = true;

					int v = paramsLineEdit->text().toInt();// +ind[0];
					paramName = filter->GetShowText(paramNr, v);

					//ttext+= ": "+paramName;
					paramsNameLabel->setText(": " + paramName);
					//setToolTip(paramName);
					textForTooltip = filter->GetShowText(paramNr, val);
			setToolTip(textForTooltip);
					//paramsNameLabel->setToolTipDuration(100);


					//mixerFilterParamsWidgets[i]->setVisible(true);
				}
			}


			int mode = filter->GetViewMode(paramNr);
			if (mode == 0)
			{
				//no slider
				paramsComboBox->hide();
				paramsSlider->hide();

			}
			//
			else if (mode == 1)
			{
				//slider

				//this->paramsLineEdit->hide();
				this->paramsLabel->show();
				//this->paramsNameLabel->setText(": " + paramName);
			}
			if (filter->GetViewMode(paramNr) == 2)
			{
				//zrobie comboboxa
				paramsComboBox->show();

				paramsLineEdit->hide();
				paramsNameLabel->hide();
				paramsSlider->hide();
				paramsLabel->show();
			}
			else
			{

				paramsComboBox->hide();
			}

		}
	SlotConnectAll();
	prevFilter = filter;
	prevText = paramsLineEdit->text();
	return val;

}

bool ParamsWidget::eventFilter(QObject* obj, QEvent* event)
{

	if (event->type() == QEvent::ToolTip) {                            // Only process tool tip events
		QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);      // Tool tip events come as the type QHelpEvent
		QPoint pos = helpEvent->pos();                                 // Get pos from event (instead of what was passed to maybeTip)

		QPoint p2(pos.x(), pos.y());

		QPoint pt = this->mapToGlobal(QPoint(pos.x(),  pos.y()));
		////emit signalTolTipActivated(p2);
		////event->ignore();
		//static QLabel tipLabel;
		//static int index = 0;
		//tipLabel.show();
		///*if (!tipLabel)
		//	foreach(QWidget * widget, QApplication::allWidgets())*/
		//	/*if (widget->inherits("QTipLabel"))
		//		tipLabel = qobject_cast</QLabel>< QLabel>(widget);*/

		////if (tipLabel)
		////{
		////	tipLabel.setText("AAAAAAAAAAAAAAAAAAAAAAAAAA");
		////	/*int k = (index > 7) ? 14 - index : index;
		////	index += (index & lt;14) ? 1 : -14;*/
		////	int k = 10;
		////	QColor c = QColor(255, 128 + k * 16, k * 20);
		////	QPalette pal = tipLabel.palette();
		////	pal.setColor(QPalette::Background, c);
		////	tipLabel.setPalette(pal);
		////}

		//QTimer::singleShot(&tipLabel ? 100 : 1000, this, SLOT(beat()));
		//QString textForTooltip = "aaaaaaaaaaaaaaaaaaaaa";
		//QPoint p(0, 0);
		setToolTip(textForTooltip);
		QToolTip::showText(pt, textForTooltip);
	///*	QToolTip tip;
	//	tip.showText("")*/
		return true;                                                    // Return true to filter event
	}
	return false;
}