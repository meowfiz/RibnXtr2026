#ifndef HIST_WIDGET_H
#define HIST_WIDGET_H
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include	<time.h>

#include	<qwidget.h>
#include	<qimage.h>
#include	<qpainter.h>
#include	<qslider.h>
#include	<qlabel.h>

#include	<qcheckbox.h>
//Added by qt3to4:
//#include <QWidgetLayout>
//#include <QVBoxLayout>
#include	<stdio.h>
#include	<qlayout.h>
//#include	<q3valuelist.h>
#include	<qpushbutton.h>
//#include	<QWidget.h>
//#include	<q3groupbox.h>
//#include	<q3frame.h>
#include	<qmessagebox.h>
//#include	<q3vbox.h>
//#include	<QButtonGroup.h>
#include	<qradiobutton.h>

#include	"RawDataSet.h"
#include	"ImageWidget.h"
#include	"DataSet.h"
#include	"MyDockWidget.h"
#include	"MyQtObjects.h"

//#include	"marquardt.h"
#include	"OtherCommonFunctions.h"
#include	"TransferFunction.h"
#include	"OptimizationMarquardt.h"

#include	<vtkPiecewiseFunction.h>

#include <math.h>
#include <qgroupbox.h>
class RawDataSet;

class HistWidget : public QWidget, public Function
{
  Q_OBJECT

  public:
  	
	double	GetFunctionValue(double ,double *);
	void	CalculateDerivative(double *Deriv,double *Params);
	void	PrecalculateVariables(double *);
	void	intit(){};
	double	GetSingleGaussianValue(double pos, double *a,double nr);
	void	CreateNewPalette(int *postab,int size);
	void	GetRandomCol(unsigned char &r,unsigned char &g,unsigned char &b);


	void	SetBackgroundColor(QColor col, bool hideBoxSLider = false);
	QColor backgroundColor;


	HistWidget(QList<QSlider2*>* _sliderList, LinkedList<DataSet*>	*_DataSetList,QWidget *parent=0,QString name="",Qt::WindowFlags f=0);
	~HistWidget(void);

	int GetLowerVolumeThreshold() { return Thresh1VolSlider->value(); };
	int GetUpperVolumeThreshold() { return Thresh2VolSlider->value(); };
	
	int GetLowerThreshold();
	int GetUpperThreshold();
	double GetLeftY();
	double GetRightY(); 
	
	void InsertNewData(int ModelPos);

	QCheckBox *logCheckBox;
	QCheckBox *volThreshsBox;
	QCheckBox* revThreshBox;
	QCheckBox *maskShowModeCheckBox;
	
	void SetLowerThreshold(int value);
	void SetUpperThreshold(int value);
	void SetLeftY(double value);

	void SetRightY(double value); 

	ImageWidget *GetImageWidget();
	void DeleteData();

	bool	GetStretchPaletteMode();
	int		GetMinValue();
	int		GetMaxValue();

	void SetVolumeOpacityFunctions(vtkPiecewiseFunction *f1,vtkPiecewiseFunction *f2);

	//void			HistogramSmoothing(int *tab);
	QSlider2	*Thresh1Slider;
	QSlider2	*Thresh2Slider;
	QSlider2	*Thresh1VolSlider;
	QSlider2	*Thresh2VolSlider;
	QSlider2	*ScaleSlider;
	protected:	

	vtkPiecewiseFunction *volumeScalarOpacityFunction;
	vtkPiecewiseFunction *volumeGradientpacityFunction;
	//TransferFunction *TrFunction;



	

	QRadioButton *ThresholdRadioButton;
	QRadioButton *TransferFunctionRadioButton;

	QWidget *ThresholdsBox;
	QGroupBox *TransferFunctionBox;

	int Mode;
	int LastMode;
/////////////////////////////
	LinkedList<DataSet*>	*DataSetList;

	DataSet					*MainDataSet;
 	PaletteRGBA		*Pal;
	PaletteRGBA		*PalTemp;

//////////////////////////////

	

	RawDataSet *data;
//	Marquardt *HistMarquardt;

	ImageWidget *IWidget;
	QVBoxLayout* VLayout;
	QHBoxLayout* HLayout;

	QPushButton *OptimButton;
	QPushButton *OptimButton2;
	int *Bins;

	QImage 	*Image;

	QLabel  *Thresh1Label;
  	QLabel  *Thresh2Label;

	QLabel  *Thresh1VolLabel;
	QLabel  *Thresh2VolLabel;

	int		*Palette;
	int		*HistArray;
	int		*TempPaletteArray;
  	double	ScalePalette,ScalePalette2;
	double	StretchedScalePalette,StretchedScalePalette2;
	bool	StretchPalette;
  	
	QList<QSlider2*>* sliderList;

	bool WindowLevel;


	int		Max;
	int		MaxTemp;
	int		Range;
	int		HistMax,HistMin;

	int		FirstHistValue;
	int		LastHistValue;
	int		BinsMax,BinsMin;

	int		ScaleSliderRange;

	int		rx,ry,rz;
  	int		sx,sy,sz;
	unsigned int 		XHSize;
	unsigned int		YHSize;

  	short 	view;
	bool Viewer;


	double chisq;
	int Hsize;
	int	Lsize;

	double	*y;
	double	*x;
	double	*sig;
	double	*a;
	int		*ia;


	double factor;
	
	int LowerThreshold;
	int UpperThreshold;

	double LeftY,RightY; 

	double	*tHistTab;
	int		tHistPos;
	int		nrOfParams;

	bool histGraphMode;
	QWidget *GridColTh1, *GridColTh2;

 	private slots:



	void	SlotSetFullPalette();
	void	SlotSetThresholdMode();
	void	SlotSetTransferFunctionMode();
	void	ThreshSelect(int );
	void	RedrawHistogram(int);
	void	ChangePaletteType(void);
	void	GenerateOptimizedPalette();
	void	GenerateOptimizedPalette2();
	void	HistogramEqualization();
	void	SlotSetThresholds(int X,int Y,int Button);
	void	SlotShowThresholdsMode(bool);

	

	public slots:
	void	SlotHistGraphMode(bool);
	void	GiveThresholds();
	void	Update();

	void	SlotSwitchToThresholdMode();
	void	SlotSwitchToWindowLevelMode();

	void	SlotSwitchToStretchedPaletteMode();
	void	SlotSwitchToNormalPaletteMode();
	void	SlotSendPointArray(QPolygon a,double x,double y);
	void	SlotSetCurrentHistogram(RawDataSet *origData, int *histTab, int size);

	signals:
	
	void	SignalThreshModeChanged(bool);
	void	ThresholdsChanged();

	void	SendThresholds(int _LowerThreshold,int _UpperThreshold);
	void	SendThresholds2(int _LowerThreshold,int _UpperThreshold,int *TPaletteArray);
	void	SendTempPalette(int *_TempPaletteArray);

	void	SignalSendUserTransferFunction();
	void	SignalOpacityModeChanged();

	void	SignalSendPaletteRotationValue(unsigned char value);
	void	SignalSendNewPalette(PaletteRGBA _Palette);
	void	SignalSendOpacityFunctionPointArray(QPolygonF p,int mode);
};

#endif //HIST_WIDGET
