#ifndef	IMAGE_SET_REGISTRATION_H
#define	IMAGE_SET_REGISTRATION_H



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
#include	"RawDataMixerLite.h"

#include <omp.h>

#include <qregexp.h>

#define NROFPARAMS 10

class ImageSetRegistration : public QObject,public Function
{
	Q_OBJECT

	public:	
	
	
	bool removeOutliers;
	int resultNrSlices;
	int currentSlice;
	int paramsReg[NUMBER_OF_REG_SLIDERS];
	int actualBins;
	bool tabsInitialized;
	int scatterSizeID;

	int hist1[256];
	int hist2[256];
	int scatter[65536];

	double stats1[16];
	double stats2[16];
	double statsMutual[16];
	ImageSetRegistration(RawDataSet *_ModelData, RawDataSet *_ObjectData);
	ImageSetRegistration(DataSet *_ModelDataSet, DataSet *_ObjectDataSet);
	~ImageSetRegistration();

	void SetGhostData(RawDataSet *data){ ghostData = data; };
	void	SetMatchSingleFrameWithModelParams(bool *chcks, QString prefilterName, int nrOfSamples, int functionNr, int nr[2],  int *params, int nrMissPixels,
		View *TransversalV = NULL, bool paintView = false, PaletteRGBA *pal=NULL,unsigned char opacity=128);

	void	ApplyRegistration();
	void	ApplyRegistration2();
	void	ApplyStabilization(double medThresh=0);
	void	ApplyStabilization2();
	void	MatchSingleFrameWithModel(int sliceNr,bool sort);
	void	MatchCenters(double *tr,double *c);
	void	MatchCenters(int posSrc,int posDst, double *center);
	double	GetFunctionValue(double *tr);
	double	GetFunctionValue2(double *tr);
	void	SetData(RawDataSet *s,RawDataSet *d, RawDataSet *m);
	double	GetSqDistSimilarityFunction(double *tr,RawDataSet *distData, double scaleFactor,
														RawDataSet *edgeData,bool inverse);
	void	SetColorData(RawDataSet *s,RawDataSet *d);
	void	FindBestFittingCandidats();
	double	CalculateSumOfSquareDifferencesSimilarity(int posSrc,int posDst, double *tr);
	double	CalculateSumOfSquareDifferencesSimilarity(RawDataSet *src, RawDataSet *dst, int sliceNr,double *tr,bool inv);	
	double  CalculateSumOfSquareDifferencesSimilarityGrayImg(RawDataSet *src, RawDataSet *dst,double *tr,bool inv);
	void	SetShapesAndVisImage(ImageShapes 	*s, ImageWidget *w);
	void GetSimTab(double* simTab,double *stabTab=NULL);
	//void	SetPaintTransformedSlicesEnabled(bool m){ paintTransformedSlicesEnabled = m; };
private:
	RawDataSet   *ghostData;
	bool Update2DStatsParallel(int bins, int *hist1, int *hist2, int *scatter, double *statsScatter, int *bounds, unsigned int*maskImgBits,
		RawDataSet *modelData, RawDataSet *objectData, int **tmpTab, int &count, unsigned short *result = NULL,
		bool useTrans = false, double *trans = NULL, bool invTrans = false, double *rCenter = NULL, int samplingX=1, int samplingY=1);


	void UpdateHistograms(int y, int*scatTab, RawDataSet *modelData, RawDataSet *objectData, int x1, int x2, int stepx, double *params,  double scale1, double scale2, double m1Inv, double m2Inv, unsigned int*maskImgBits, int bins);
	InterpolationFunction2DBilinear *interpolation ;
	Transform* transformation ;

	bool parallelComputing;
	int		numberOfThreads;
	int numberOfThreadsUsed;
	int **scatterTabs;
	double **paramsTabs;

	unsigned char regOpacity;
	RawDataSet	*transversalDataSet;
	
	PaletteRGBA *paletteReg;

	bool	paintTransformedSlicesEnabled;
	void	PaintTransformedSlices(double *TransT);
	View	*transversalView;


	int objSliceRange[2];
	bool chcks[10];
	QString prefilterName;
	int nrOfSamples;
	int functionNr;

	InterpolationFunction *intrp;
	double intrpVal;
	RawDataSet *modelData,*objectData;
	RawDataSet *srcData,*dstData;
	//RawDataSet *distData;
	RawDataSet *edgeSrcData;
	RawDataSet *edgeDstData;
	RawDataSet *srcDistData, *dstDistData;
	RawDataSet *colSrcData,*colDstData;
	double srcScaleDist,dstScaleDist;
	RawDataSet *tempData;
	double t[15];
	double matrix[16];
	Transform *trans;
    double rCenter[3];
	int sliceNumber;

	ImageShapes *shapes;
	ImageWidget *visWidget;
	
	int scatterSize;
	double maxFound;
	int bounds[4];
	unsigned int *imgMaskBits;
	RawDataSet *filteredModel;
	RawDataSet *filteredObject;
	int posTab[10];
	int numOfParams;
//	int scatterSizeID;

//	int hist1[256];
//	int hist2[256];
//	int scatter[65536];

//	double stats1[16];
//	double stats2[16];
//	double statsMutual[16];
	double maxFactor;
	int count;
	int functionNumber;
	int minCount;
	int nrMissPixels;

	bool optimizationStopped;

	double XStep_x, XStep_y, XStep_z;
	double YStep_x, YStep_y, YStep_z;
	double ZStep_x, ZStep_y, ZStep_z;

	double XBack_x, XBack_y, XBack_z;
	double YBack_x, YBack_y, YBack_z;
	double ZBack_x, ZBack_y, ZBack_z;

	double ZeroYNextZ_x, ZeroYNextZ_y, ZeroYNextZ_z;
	double ZeroXNextY_x, ZeroXNextY_y, ZeroXNextY_z;


	int counter;
	unsigned int totalTime, totalTime1, totalTime2, totalTime3, totalTime4;
	
	//----------------------------------------

	void  TransformSlice(RawDataSet *src, RawDataSet *dst, int sliceNr,double *trans,bool inv=true);
	void  TransformSlice2(RawDataSet *src, RawDataSet *dst, int sliceNr, double *trans, bool inv = true);

	void	AddSqDistEnergy(int j, double& energy, int& count, RawDataSet* src, RawDataSet* dst,int nr);

	double GetDistanceFromRawData(RawDataSet *dist, double x,double y, double slice);
    void  PrepareTrans(double *t0,double *t1);   
	double GetFunctionValueDist(double *tr);
	void  GetPercentDistMaskData(RawDataSet *distMap,RawDataSet *edgeData, RawDataSet *maskData, double percent,double *tr);
	RawDataSet *CreateBlendingDataSet(RawDataSet *d);

	RawDataSet *oSliceData,*mSliceData;
	
	bool grayMode;

	bool useGlobalMax;
	double globalMaxValue;

	  void    intit(){};

signals:
		void	SignalSendRawDataSet(RawDataSet *);
		void	SignalSendActualTransform(double *);

	    void	SendMessage(const QString &Message);
		void	SignalStopOptimization();
		void SendMessageToLog(const QString& Message);


		public slots:

		void SlotStopOptimization();
		void	SlotSetParalellComputing(bool, int);
};


//---------------------------------------
/*	class InterpolationFunction
	{

		public:
			InterpolationFunction(){};

			virtual	double GetInterpolatedValue(RawDataSet *data,double X,double Y, double Z)=0;
	

	};
//-------------------------------
	class InterpolationFunctionTrilinear : public InterpolationFunction
	{
		public:

			double GetInterpolatedValue(RawDataSet *data,double X,double Y, double Z)
			{
				
				return Interpolation3Linear(data,X,Y,Z);
			};
	
	};

//-----------------------------------
	class InterpolationFunctionNearest : public InterpolationFunction
	{
		public:

			double GetInterpolatedValue(RawDataSet *data,double X,double Y, double Z)
			{
				
				return data->GetVoxelValue((int)(X+0.5),(int)(Y+0.5),(int)(Z+0.5));
			};
	
	};

	class InterpolationFunction2DNearest : public InterpolationFunction
	{
		public:

			double GetInterpolatedValue(RawDataSet *data,double X,double Y, double Z)
			{	
				return data->GetDataArray()[(int)(X+0.5)+(int)(Y+0.5)*data->GetNx()];
			};
	
	};


	class InterpolationFunction2DBilinear : public InterpolationFunction
	{
		public:

			double GetInterpolatedValue(RawDataSet *data,double X,double Y, double Z)
			{	
				int xx,yy,x11,x12,x21,x22;
				double y1,y2;
				double dx,dy;
				xx=(int)X;
				yy=(int)Y;
				dx=X-xx;
				dy=Y-yy;
				int nx=data->GetNx();
				int pos=xx+yy*nx;

				unsigned short *tab=data->GetDataArray();
			
				x11=tab[pos];
				x12=tab[pos+1];
				x21=tab[pos+nx];
				x22=tab[pos+nx+1];

				y1=		x11*(1.0-dx)+x12*dx;
				y2=		x21*(1.0-dx)+x21*dx;
				return	y1 *(1.0-dy)+y2 *dy;

			};
	
	};

//----------------------------------------

class Registration  : public QObject,public Function
{
 
	Q_OBJECT

	public:


	Registration(	ListItem<DataSet*>	*ModelListItem,	ListItem<DataSet*>	*ObjectListItem,ImageWidget *_IWidget,QLabel *_InfoLabel,double *Trans,
					View *TransversalV=NULL,View *SagittalV=NULL,View *CoronalV=NULL,
					QWidget *parent=0,const char *name=0,WFlags f=0);

	~Registration();
	double GetFunctionValue(double *);
	void    intit(){};
	InterpolationFunction *Interpolate;

	unsigned char *scatterMaskData;

	int	TransSize;
	//double *log2tab;

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


	RawDataSet		*GenerateScatterPlot();
	RawDataSet		*TransformObjectToModelVolume();
	double			ComputeMutualInformation(double *TransT);

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

	bool			Scaled256;
	int				*tab1,*tab2,*ObjectTab,*ModelTab,*ScatterTab;
	double			*RotateCenter,*TransTemp;
	double			*Matrix;
	RawDataSet		*DataPadded;
//	double			Ox,Oy,Oz;
	bool			DataPrepared;

	double			*FinalTrans;

	int	VolumeStepX,VolumeStepY,VolumeStepZ;

	int ScatterSize;

	int time1, time2;

	double TabVal[5000][8];

	int PosX,PosY,PosZ;
	int InspX,InspY,InspZ;

	public slots:
	
	void	SlotSetViewPos(int Ix,int Iy,int Iz,int Px,int Py,int Pz);
	void	SlotSet2DSlicesPos(int Px,int Py,int Pz);
	void	SlotSwitchScatterState(int State);

	void	SlotStopOptimization();

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
*/			
#endif //   IMAGE_SET_REGISTRATION_H

