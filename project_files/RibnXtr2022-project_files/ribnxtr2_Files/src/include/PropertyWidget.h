#ifndef	PROPERTY_WIDGET
#define	PROPERTY_WIDGET

#include <qgroupbox.h>
#include <vtkProperty.h>
//#include "VtkWidget.h"
#include	"MyDockWidget.h"
#include <qwidget.h>
#include <qimage.h>
#include <qpainter.h>
#include <qslider.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
//#include <q3groupbox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
//#include <QWidget.h>
#include <qcombobox.h>
#include <qtabwidget.h>
//#include <QWidget.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qcolordialog.h>
#include <qdesktopwidget.h>
#include <qapplication.h>
//Added by qt3to4:
#include <QCloseEvent>
#include <stdio.h>
#include <stdio.h>


//class VtkWidget;

class PropertyWidget : public MyDockWidget
{
  Q_OBJECT

  public:
  	
	PropertyWidget(QWidget *parent=0,QString name="",Qt::WindowFlags flag=0);
	~PropertyWidget();


	vtkProperty *OriginalProperty;
	vtkProperty *NewProperty;


	QPushButton *ColorButton;

	QPushButton *AmbientColorButton;
	QPushButton *SpecularColorButton;
	QPushButton *DiffuseColorButton;
	QPushButton *ApplyChangesButton;
	QPushButton *CancelChangesButton;
	QPushButton *SaveAsDefaultButton;


	QSlider *SpecularPowerSlider;
	QSlider *SpecularLightingCoefSlider;
	QSlider *DiffuseLightingCoefSlider;
	QSlider *AmbientLightingCoefSlider;

	QSlider *OpacitySlider;
	QSlider *RedColorSlider;
	QSlider *GreenColorSlider;
	QSlider *BlueColorSlider;


	QLabel *ColRedLabel;
	QLabel *ColGreenLabel;
	QLabel *ColBlueLabel;
	QLabel *SpecularLightLabel;
	QLabel *SpecularPowerLabel;
	QLabel *DiffuseLightLabel;
	QLabel *AmbientLightLabel;
	QLabel *OpacityLabel;

	QRadioButton *FlatRadio;
	QRadioButton *GouraudRadio;
	QRadioButton *PhongRadio;
	QRadioButton *SurfaceRadio;
	QRadioButton *WireframeRadio;
	QRadioButton *PointsRadio;
	QTabWidget 	*qtab;

protected:

	bool	CloseParam;

	void	closeEvent( QCloseEvent * );


signals:

	void Updated();

	void Deleted();

public slots:


	void	SlotSetInputProperty(vtkProperty *Property,const char *name=NULL,bool show=true);
	void	SlotUpdateExternalProperty(vtkProperty *Prop);


private slots:

	void	SlotColorClicked();
	void	SlotSetRedValue(int Value);
	void	SlotSetGreenValue(int Value);
	void	SlotSetBlueValue(int Value);
	void	SlotSpecularClicked();
	void	SlotSpecularValue(int Value);
	void	SlotSpecularPower(int Value);
	void	SlotDiffuseClicked();
	void	SlotDiffuseValue(int Value);
	void	SlotAmbientClicked();
	void	SlotAmbientValue(int Value);
	void	SlotOpacityValue(int Value);
	void	SlotChangeInterpolationMode();
	void	SlotChangeRepresentationMode();
	void	SlotApplyClicked();
	void	SlotCancelClicked();
	void	SlotSaveClicked();



	void	SlotUpdateWidget();

};	 	
			
#endif //   PROPERTY_WIDGET

