#ifndef	REGISTRATION
#define	REGISTRATION



#include	<qmessagebox.h>
#include	<qstring.h>
#include	<qapplication.h>
#include	<qlabel.h>
#include	<qwidget.h>
#include	<qdatetime.h>
#include	<qimage.h>


#include	<stdio.h>
#include	"RawDataSet.h"
#include	"Transform.h"
#include	"OtherCommonFunctions.h"
#include	"View2DSliceWidget.h"

#include	"ImageWidget.h"

#include	"OptimizationPowell.h"
#include	"AbstractFunctionClasses.h"


#include	<vtkMatrix4x4.h>
#include	<vtkCellArray.h>
#include	<vtkFloatArray.h>
#include	<vtkCellData.h>
#include	<vtkPointData.h>
#include	"SurfaceCollection.h"
#include	"VtkWidget.h"

#include <omp.h>

#include <qregexp.h>
//---------------------------------------
	
//----------------------------------------

class Registration  : public QObject,public Function
{
 
	Q_OBJECT

	public:


	Registration(	ListItem<DataSet*>	*_ModelListItem,	ListItem<DataSet*>	*_ObjectListItem,ImageWidget *_IWidget,QLabel *_InfoLabel,double *Trans,
					View *TransversalV=NULL,View *SagittalV=NULL,View *CoronalV=NULL,
					QWidget *parent=0,const char *name=0,Qt::WindowFlags f=0);

	~Registration();
	void  setConstraints(double *);
	double GetFunctionValue(double *);
	void    intit(){};
	InterpolationFunction *Interpolate;

	double lastFValue;
	unsigned char *scatterMaskData;
	ListItem<DataSet*>* ModelListItem;
	ListItem<DataSet*>* ObjectListItem;
	bool oneScale;
	int	TransSize;
	//double *log2tab;

	bool constraints;
	double constraintRanges[13];
	double getConstraintUpdate(double *t);

	double XStep_x,XStep_y,XStep_z;
	double YStep_x,YStep_y,YStep_z;
	double ZStep_x,ZStep_y,ZStep_z;

	double XBack_x,XBack_y,XBack_z;
	double YBack_x,YBack_y,YBack_z;
	double ZBack_x,ZBack_y,ZBack_z;

	double ZeroYNextZ_x,ZeroYNextZ_y,ZeroYNextZ_z;
	double ZeroXNextY_x,ZeroXNextY_y,ZeroXNextY_z;


	int XGraphResolution;
	int YGraphResolution;
	int ZGraphResolution;

	double TransGraphSize;
	double RotGraphSize;
	int	  TransXPos,TransYPos,TransZPos;

	unsigned char RegOpacity;
	RawDataSet	*TransversalDataSet;
	RawDataSet	*SagittalDataSet;
	RawDataSet	*CoronalDataSet;
	PaletteRGBA *PaletteReg;

	//bool	GrabApplicationWindow;

	bool	PaintTransformedSlicesEnabled;
	void	PaintTransformedSlices(double *TransT);
	View	*TransversalView;
	View	*SagittalView;
	View	*CoronalView;

	int		*ParamTransTab;
	int		NumberOfOptimParams;
	double	*TransXX;

	bool	OptimizationSuccsessfull;



	void	GetRealTransform(double *RealTransform,double *OptimTransform,int *ParamTrans,int NrOfParams);
	void	GetOptimTransform(double *OptimTransform,double *RealTransform,int *ParamTrans,int NrOfParams);

	void	ApplyPowellsRegistration();
	double MIFunction1(double p,double p1,double p2){return -p*log(p/(p1*p2));}; //OK
	//double MIFunction2(double p,double p1,double p2){return -2.0*(p*log(p/(p1*p2)))/(p1+p2);}; //ZLE
	double MIFunction3(double p,double p1,double p2){return -p*log(p)-p*log(p/(p1*p2));}; //OK
	double MIFunction4(double p,double p1,double p2){return log(p/(p1*p2))/log(p);}; //ZLE
	double MIFunction5(double p,double p1,double p2){return -p*log(p);}; 
		//return log2tab[(int)(1000000*p)];}; //OK


	double CompMutual(int *ST,int *MT,int *OT,int count, double (Registration::*funcs) (double p,double p1,double p2));
	double GetEntropy(int *Histogram,int count);

	int CountMutual;
	ImageWidget *IWidget;
	QImage *Image;
	QLabel *InfoLabel;
	int Iterations;
	double Tolerance;

	//potem zamienic ewentualnie na bool
	int InterpolationMode;
	int	TransformationMode;
	bool	ScatterPlotVisualizationEnabled;
	int MisregistrationFunctionMode;

	RawDataSet		*TransformObjectToFinalVolume(RawDataSet *finalData);
	RawDataSet		*GenerateScatterPlot();
	RawDataSet		*TransformObjectToModelVolume(bool inv , unsigned char*& outputMask,int interpolMode=0 );
	void			TransformObjectToModelVolume(int value,RawDataSet *d=NULL,RawDataSet *d1=NULL);
	double			ComputeMutualInformation(double *TransT);

	void			TransformPoints(double *TransT,double *pts,int nrOfPts);
	void			PrepareVariables(int nr);
	void			UnprepareVariables(int nr);
	void			GenerateMisregistrationGraphData(const QString &filename);
	void			GeneratePolyMisregistrationGraphData( VtkWidget *VWidget);
	void			Generate3DVolumeMisregistrationGraphData();

	QString			TransSymbolName(int nr);

	void			setScatterMaskData(unsigned char *m);//{scatterMaskData=m;};
	void			setScatterMaskData(unsigned char *m,int sx,int sy);//{scatterMaskData=m;};


	RawDataSet		*ModelData;
	RawDataSet		*ObjectData;
	double *Trans;
	double trans2[12];

	bool			Scaled256;
	int				*tab1,*tab2,*ObjectTab,*ModelTab,*ScatterTab;

	//int  **ObjectTabs,**ModelTabs,
	int **ScatterTabs;
	double			RotateCenter[3],*TransTemp;
	double			Matrix[16];
	RawDataSet		*DataPadded;
//	double			Ox,Oy,Oz;
	bool			DataPrepared;

	double			*FinalTrans;

	int	VolumeStepX,VolumeStepY,VolumeStepZ;

	int ScatterSize;

	int time1, time2;
	int SSY,SSZ,Add1,Add2;
	unsigned char *newMask;
	int nNx,nSliceSize;
	int newMaskSize;
	
	double TabVal[5000][8];

	int PosX,PosY,PosZ;
	int InspX,InspY,InspZ;

	void	UpdateHistograms(int Z, int*scatTab, unsigned short *Q1,unsigned short *paddData,int paddNx,int paddSS, double *Point1,int t[3],int T[3],int &count,bool inv);

	
	private:
		bool parallelComputing;
		int		numberOfThreads;
		int numberOfThreadsUsed;

	public slots:
	
	void	SlotSetViewPos(int Ix,int Iy,int Iz,int Px,int Py,int Pz);
	void	SlotSet2DSlicesPos(int Px,int Py,int Pz);
	void	SlotSwitchScatterState(int State);

	void	SlotStopOptimization();
	void	SlotSetParalellComputing(bool,int);
	void	SlotRegOpacity(int);
	//void	SlotSetNrOfParallelThreads(int);

	signals:

	void	SignalSendNewSurface(PolygonSurfaceData *_SurfaceData);
	void	SignalInsertRawDataSet(RawDataSet *DataX);

	void	SignalGet2DSlicesPos();

	void	SignalStopOptimization();

	void	SignalSendActualTransform(double *);

	void	SignalGrabApplicationWidnow();
	void	SignalSendRotateCenter(double *);
	void	SignalSendNewMask(unsigned char *n);



	
};	 	
			
#endif //   REGISTRATION

