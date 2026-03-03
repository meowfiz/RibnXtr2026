#ifndef PARAMS_WIDGET_H
#define PARAMS_WIDGET_H

#include <stdio.h>
#include <math.h>
#include "MyQtObjects.h"
//#include "OtherCommonFunctions.h"
//#include "Geometry.h"
#include <qobject.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qslider.h>
#include <qwidget.h>
//#include "RawDataLayer.h"
#include "LayerGenericFilter.h"
#include <qcombobox.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>



class ParamsWidget : public QWidget
{

	Q_OBJECT

public:

	void UpdateSliderSettings(LayerGenericFilter *filter, int paramNr, int sel, double val, double *address = NULL,int *cAddress=NULL);// , RawDataLayer *l = NULL, int mode = -1);// int *ind, QString paramName, bool sliderTest, double *sliderParams, QString ttext, bool sel);
	void SetBackgroundColor(int r, int g,int b);
	void GetBackGroundColor(int* col) { col[0] = color[0];col[1] = color[1];col[2] = color[2]; };
	double SliderSettingsChanged(LayerGenericFilter *filter = NULL, int paramNr = -1);
	void	SetTopLabel(const QString &, bool visible);
	void	HideWidgetAndTopLabel();

	ParamsWidget(QList<QSlider2*>* _sliderList,QWidget * parent);
	~ParamsWidget();



	//QList<QPushButton2*>* buttonList;
	//QList<QCheckBox2*>* checkBoxList;
	//QList<QComboBox2*>* comboBoxList;
//	QList<QLineEdit2*>* lineEditList;
	QList<QSlider2*>* sliderList;
//	QList<QSpinBox2*>* spinBoxList;

	QCheckBox		*paramsCheckBox;
	QLineEdit		*paramsLineEdit;
	QLabel			*paramsNameLabel;
	QLabel			*paramsLabel;
	QSlider2			*paramsSlider;
	QWidget			*paramsWidget;
	QComboBox		*paramsComboBox;
	QLabel			*paramsTopLabel;

private:
	QString defaultStyle;
	LayerGenericFilter *prevFilter;
	QString prevText;
	int prevParamNr;
	double *valAddres;
	int* colAddres;
	int  color[3];
	QString textForTooltip;
	QLabel tipLabel;
	//RawDataLayer *layer;
	///int filterMode;

protected:
	bool eventFilter(QObject* obj, QEvent* event);
	void mousePressEvent(QMouseEvent* event);

signals:

	void	textChanged(const QString &);
	void	valueChanged(int);
	void	stateChanged(int);
	void	backgroundColorChanged(int);

	public slots:

	void	SlotSelectNewColor();
	void	SlotConnectAll();
	void	SlotDisconnectAll();
	void	SlotComboBoxActivated(int val);
	void	SlotTextChanged();

};




#endif //PARAMS_WIDGET_H
