#ifndef	SCATTERPLOT_PROCESSING
#define	SCATTERPLOT_PROCESSING

#include	<qimage.h>
#include	<qstring.h>
#include	<qwidget.h>
#include	<stdio.h>
#include	<time.h>

#include	"OtherCommonFunctions.h"
#include	"OptimizationMarquardt.h"




class ScatterPlotProcessing : public QWidget,public Function
{
  Q_OBJECT

  public:


	ScatterPlotProcessing(QImage *img=NULL,QWidget *parent=0,const char *name=0);
	~ScatterPlotProcessing();

	void	SmoothScatterPlot();
	int		FindPeaks(int *tab,int peakNr);
	double	FitGaussians(int *tab,int peakNr,double *startA,double *endA);

	void	ProcessScatterPlot(int peaks,int iterNr,double sigma,int sxx,int syy);

	
	double	GetFunctionValue(double ,double *);
	void	CalculateDerivative(double *Deriv,double *Params);
	void	PrecalculateVariables(double *);
	void	intit(){};
	double	GetSingleGaussianValue(double pos, double *a,int nr);

	void	GenerateSegmentationMasks(double sigma, double *a, int nr,int scx,int scy);

	int sizeX,sizeY;
	int origSizeX,origSizeY;
	int *tab;
	int tHistPosX,tHistPosY;
	int nrOfParams;
	//double chisq;
	double *tmpA;
	double tolerance;
	int gaussParams;
	double minFloat;

	int iterX;
	signals:

	void	SignalSendSegmentationMask(unsigned char *&, int sx,int sy);
	void	SendMessage(const QString &Message);
	


	
};
			
#endif //   SCATTERPLOT_PROCESSING
