#ifndef VOLUME_ELASTIC_REGISTRATION_H
#define VOLUME_ELASTIC_REGISTRATION_H

#include	<qapplication.h>
//#include	<q3filedialog.h>
#include	<qstring.h>
#include	<qregexp.h>
#include	 <time.h>

#include	"DistanceMap.h"
#include	"RawDataSet.h"
#include	"Transform.h"
#include	"VtkWidget.h"

#include	"OptimizationPowell.h"
#include	"OptimizationDFPowell.h"
#include	"AbstractFunctionClasses.h"
#include	"OtherCommonFunctions.h"
#include	"TransformBSpline.h"
#include	"DataSet.h"
#include	"LinkedList.h"
#include <omp.h>
	#define FIXED_REST_TO_INT(x)    ((x)& 0xFFFFU)
	#define ROUND_FIXED_TO_INT(x)   (((x)+0x8000)>>16)
	#define LERP(a,l,h)     (WORD) (l+ ROUND_FIXED_TO_INT(((h-l)*a)))

///////////////////////////////////////////////////////



class VolumeElasticRegistration: public QObject,public Function
{
	Q_OBJECT

	public:
		
		VolumeElasticRegistration(ListItem<DataSet*>	*ModelListItem,	ListItem<DataSet*>	*ObjectListItem,FloatGrid3D *_Grid,double maxDisp=0.0,const QString& text="");
		~VolumeElasticRegistration();

		RawDataSet *ApplyTransformation();
		RawDataSet	*GenerateAbsoluteDisplacementDataSet();
		void	Optimize(int _iter=1,int _optimStep=8,bool randomSampling=true,int multiressteps=1,bool fixBounds=true);
		void	Optimize2(int _iter=1,int _optimStep=8,bool randomSampling=true,int multiressteps=1,bool fixBounds=true, int valx=-1);
		void	OptimizeBig(int _iter,int _optimStep,bool _randomSampling,int multiressteps, bool fixBounds);

		
		void UpdateData(int gridNx, int gridNy, int gridNz);

private:
	bool parallelComputing;
	int		numberOfThreads;
	int numberOfThreadsUsed;

	int valX;

	Node *nodes;
	int nrOfNodes;
	double threshold;
	
	int iterx;
	double		maxFactor;
	RawDataSet	*ModelDataScaled;
	RawDataSet	*ModelData;
	RawDataSet	*ObjectData;
	FloatGrid3D *Grid;
	FloatGrid3D *OrigGrid;
	FloatGrid3D *TempGrid;
	BSplineGridTransformation *BSplineTransform;
	double	*derivs;
	double	testD[6];
	bool	calculateDerivs;
	double   FuctionValue;
	//double	*tempderivs;
	double	*transx;
	int		NodX,NodY,NodZ;
	int		*tab1,*tab2,*ObjectTab,*ModelTab,*ScatterTab;
	//double Point[3];
	unsigned short MValue,ModelValue;
	unsigned short OValue,ObjectValue;

	double ToleranceX;
	double pDef[4];
	int   oldNod[3];
	double pFun;
	double maxDisp;
	int nx,ny,nz;
	int nx1,ny1,nz1;
	int mx1,my1,mz1;
	int scatterSize;
	int count;

	double alpha;
	double mMax;
	double mVolMax,mVolMax2;

	int		posXstart,posYstart,posZstart;
	int		posXstop,posYstop,posZstop;
	double	fx,fy,fz;
	double	fx2,fy2,fz2;
	double	tdx,tdy,tdz;
	double	pSx,pSy,pSz;
	double	nnx,nny,nnz;
	int		countsize;
	int		nrofsamples;
	int		optimStep;
	bool	randomSampling;
	int		iter;
	bool	fixBounds;
	int		nrOfParams;
	//double *log2tab;
	double dMax[3],dP[3];
	double fval;

	double maxP,minP;
	int maxNrOfSamples;

	double e0;
	bool tmpbool;
	QString initText;


	RawDataSet		*ScaleDataSet(RawDataSet *data, double factor);

	double			(VolumeElasticRegistration::*GetFValue)(int x,int y,int z,double *);
	void			(VolumeElasticRegistration::*GetDerivatives)(double *Deriv,double *Params);
	void			SetSizeVariables(FloatGrid3D *grid);
	inline void		AddNewPointsToHistTabs(int i,int j,int k, int &count);
	inline void		AddNewPointsToHistTabs2(int i,int j,int k, int &count);
	inline void		AddNewEnergyPoint(int i,int j,int k, int &count,double *energy,double *derivs=NULL);
	void			AddNewEnergyPointSet(int kk, int &count, double *energy, double *drv);
	inline void		AddNewEnergyPointWithoutDerivs(int i,int j,int k, int &count,double *energy);
	 void		  AddNewEnergyPointSetWithoutDerivs(int k, int& count, double* energy);
	 double	GetFunctionValue(double *a);
	void			CalculateDerivative(double *Deriv,double *Params);
	 double	GetFunctionValue(double i,double *Params);//dla marquarda

	//nowe dla calosci
	inline double	GetTotalFunctionValue(int a,int b,int c,double *Params);
	inline void		CalculateTotalDerivative(double *Deriv,double *Params);
	void			CalculateDerivativesAtPoint(int i,int j,int k,double *der,double *drvTempTab,int &count, BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf);
	void			CalculateSingleNodDerivative(double *Deriv,double *Params);
	//	inline double	GetFunctionValue2(double *a);
	void    intit(){};//zaslepka

	inline void		GetTransFromCurrentNode(double *tr, FloatGrid3D *grid);
	inline void		SetTransToCurrentNode(double *tr, FloatGrid3D *grid);
	inline void		SetCurrentNode(int x,int y,int z);
	inline double	GetEnergy(int x,int y,int z,double *params);
	inline double	GetSqDistEnergy(int x,int y,int z,double *params);
	inline double	CompMutual(int *ST,int *MT,int *OT,int count);
	inline double	GetEntropy(int *Histogram,int count,int size);

	double	GetGradI(int i,int j,int k,double smallStep,double fvalue,int view);
	double	GetGradI(int i,int j,int k,BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf);
	void	AddGradPart1(int i,int j,int k, int &count,double *drv,double &drvel);
	void	AddGradPart2(double *der,double *drvTempTab,double smallStep,int view,BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf);



	void	SetParamsFromGridToArray(FloatGrid3D *Grid,double *Params, bool fixbounds);
	void	SetParamsFromArrayToGrid(FloatGrid3D *Grid,double *Params, bool fixbounds);
	void	GetVolumeSpacing(int minX,int maxX,int minY,int maxY,int minZ, int nx,int ny,int slicesize,int step,int &start,int &sy,int &sz);


	double	Interpolation3LinearDoubleIntArithm(RawDataSet *dataX,double x,double y, double z);
	int		GetNodesToOptimization(FloatGrid3D *Grid,double *Params, bool fixbounds,double threshold, Node *&Nodes);


	void	GradientDescentOptimizer(double *_Params,int _NumberOfParameters,double _Tolerance,
										   int *_IterationNumber,double *_FunctionValue);

	double smallStepX;
	double smallStepY;
	double smallStepZ;

	bool stopOptimization; 
	public slots:

	void	SlotStopOptimization();
	void	SlotSetParalellComputing(bool, int);


	signals:

	void			SendMessage(const QString &Message);
	void			SignalSendProgress(int);


};





#endif //VOLUME_ELASTIC_REGISTRATION_H
