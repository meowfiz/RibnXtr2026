#ifndef TEXTURE_SEGMENTATION_H
#define TEXTURE_SEGMENTATION_H
	
#include <stdio.h>
#include <memory.h>
#include <qapplication.h>
#include <math.h>


#include	"RawDataSet.h"
#include	"DataSet.h"
#include	"LinkedList.h"
#include	"OtherCommonFunctions.h"
#include	"TransformBSpline.h"
#include	"Grid3D.h"
#include	"OptimizationPowell.h"
#include	"OptimizationDFPowell.h"
#include	"EdgesExtraction3D.h"
#include	"DistanceMap.h"

#define FMAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
(maxarg1) : (maxarg2))
#define ALF 1.0e-4 
//#define TOLX (4*EPS)
///////////////////////////////////////////////////////
class TextureSegmentation :public QObject, public Function
{

	Q_OBJECT

	public:

		TextureSegmentation(RawDataSet *_rawData,unsigned char *_maskData, RawDataSet *_distanceMapRawData=NULL);
		~TextureSegmentation();

		void	GetIntensityProbabilityMap(RawDataSet *_data);
		RawDataSet*	ExtractEdgesMap(int radius,double stddev,int dim);
			
		unsigned char *DeformationBasedSegmetation(FloatGrid3D *_Grid,int grids,double s1,double s2,double intens1,double intens2);
		unsigned char *DeformationBasedSegmetation2(FloatGrid3D *_Grid,int grids,double s1,double s2,double intens1,double intens2);
		unsigned char *DeformationBasedSegmetationX(FloatGrid3D *_Grid,int grids,double s1,double s2,double intens1,double intens2);

	private:
		double mmx;
		Node *nodes;
		int nrOfNodes;

		void	GradientDescentOptimizer(double *params0,int nrOfParams,double tol,int *iter,double *fval);
		void	SetParamsFromArrayToGrid(FloatGrid3D *Grid,double *Params, bool fixbounds);
		void	SetParamsFromGridToArray(FloatGrid3D *Grid,double *Params, bool fixbounds);
		int	GetNodesToOptimization(FloatGrid3D *Grid, bool fixbounds,int threshold,Node *&Nodes);
		void	CalculateTotalDerivative(double *Deriv,double *Params);
		double	GetTotalFunctionValue(int a,int b,int c,double *Params);



		int		GetNodesToOptimization(FloatGrid3D *Grid,double *Params, bool fixbounds,double threshold,Node *&Nodes);

		int		GetNrOfMaskVoxels();
		void	GetMaskTab(unsigned short *_maskTab,int _numberOfVoxels);
		double	GetIntensityProbability(unsigned short intensity,int max,double sigma);
		void	GetIntensityProbabilityTab(double *tab,int nr,double &min,double &max,double sigma);
		void	GetExpTab(double *_Tab,int number,double sigma,double v);
		

		unsigned char	*maskData;
		unsigned char	*boundaryMaskData;
		unsigned short	*maskTab;
		

		int				numberOfMaskVoxels;
		RawDataSet		*distanceMapRawData;
		RawDataSet		*rawData;
		RawDataSet		*intensityProbabilityRawData;
		RawDataSet		*implicitDistanceDataSet;
		double			*expTab;
		double			*intensityProbabilityTab;
		int				expTabLength;
		double			intensityFactor,intensityFactor2;
		double pDef[4];
		int   oldNod[3];
		double pFun;
		double Step[3];
		double maxDisp;
		double tE[3];
		//deformations
		int nx,ny,nz;
		double *transx;
		int nx1,ny1,nz1;
		int mx1,my1,mz1;
		int scatterSize;
		int count;
		bool fixBounds;
		FloatGrid3D *Grid;
		BSplineGridTransformation *BSplineTransform;

		int		NodX,NodY,NodZ;
		int		posXstart,posYstart,posZstart;
		int		posXstop,posYstop,posZstop;
		double	fx,fy,fz;
		double	tdx,tdy,tdz;
		double	pSx,pSy,pSz;
		double	nnx,nny,nnz;
		int		countsize;
		int		nrofsamples;
		int		optimStep;
		bool	randomSampling;
		int		iter;
		double	factorIntensity;
		double	factorRaw;
		double	factorDist;
		double	factorDistImplicit;
	
		double	alpha;
		double derivs[4];

		double minP,maxP;
		double smallStepX;
		double smallStepY;
		double smallStepZ;

		double	shape1,shape2,intense1,intense2;
		int		gridSize;
		double	GetGradI(int i,int j,int k,double smallStep,int view,RawDataSet *data);
		double 	GetFunctionValue(double *params);
		void	CalculateDerivative(double *Deriv,double *Params);
		double	(TextureSegmentation::*GetFValue)(int x,int y,int z,double *);
		void	(TextureSegmentation::*GetDerivatives)(double *Deriv,double *Params);
		
		double	GetNewEnergy(int x,int y,int z, double *params);
		void	AddNewEnergyPointWithoutDerivs(int i,int j,int k, int &count,int &countB,double *energy);
		void	CalculateSingleNodDerivative(double *Deriv,double *Params);
		void	AddNewEnergyPoint(int i,int j,int k, int &count, int &countB,double *energy,double *drv);


		void    intit(){};
		double	GetEnergy(int x,int y,int z,double *tmp);
		void	UpdateEnergy(int x,int y,int z,double &energy,int &count,double &boundEnergy,int &boundCount,double &intensityEnergy);
		void	SetTransToCurrentNode(double *tr);
		void	GetTransFromCurrentNode(double *tr);

		void	SetTransToCurrentNode(double *tr, FloatGrid3D *grid);
		void	GetTransFromCurrentNode(double *tr, FloatGrid3D *grid);

		void	SetCurrentNode(int x,int y,int z);
		int		CheckNumberOfPointsInsideSection(int x,int y,int z);
		void	GetDeformationBounds(double *bounds);
		bool	CreateNewMask(double *bounds);

		int		GetNumberOfMaskVoxelNeighbors(unsigned char *data, double *point);
		bool	Check3LinearMaskVoxelNeighborhood(unsigned char *data, double *point);

		void	UpdateBoundaryMaskData(unsigned char *mask, unsigned char *boundaryMask);
		void	BinaryMorphology(unsigned char *source, unsigned char *destination, bool mode);


		void	lnsrch(int n, double xold[], double fold, double g[], double p[], double x[],
								  double *f, double stpmax, int *check);
	signals:

		void			SignalSendProgress(int);
		void			SendMessage(const QString &Message);
		void			SignalSendNewMask(const unsigned char *mask);
		void			SignalSendRawDataSet(RawDataSet *);
};

#endif //TEXTURE_SEGMENTATION_H
//////////////////////////////////////////////////////
