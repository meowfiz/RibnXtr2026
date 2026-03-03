#ifndef	OPTIMIZATION_MARQUARDT
#define	OPTIMIZATION_MARQUARDT

#include	<qapplication.h>
#include	<stdio.h>
#include	"OtherCommonFunctions.h"
#include	"AbstractFunctionClasses.h"

#include <armadillo>

using namespace arma;
class Derivatives
{
	
	public:

		void	CalculateDerivatives(double *Derivative,double X,double Y,double Z, double DistX,double DistY,double DistZ,double *Trans);
		void	CalculateTrigonometricParams(double *Trans);

	private:

		double	 sx,sy,sz,cx,cy,cz;
		double 	xtr1,xtr2,xtr3,xtr4,xtr5,xtr6,xtr7,xtr8,xtr9;
		double 	ytr1,ytr2,ytr3,ytr4,ytr5,ytr6,ytr7,ytr8,ytr9;
		double 	ztr1,ztr2,ztr3,ztr4,ztr5,ztr6,ztr7,ztr8,ztr9;
};


class OptimizationMarquardt : public QWidget
{
 
	 Q_OBJECT
	
public:

	OptimizationMarquardt(double *x, double *y, double *sig, int NPoint, double *a,int *ia,int ma,Function *_F);
	~OptimizationMarquardt(void);

	void StartOptimization(double tol=1e-6);

	Function	*Func;
	

	bool ExitEnabled;


	double *XValues;
	double *YValues;
	double *SigmaValues;
	int		NPoint;
	double *Params;
	int		*BlockParams;
	int		NParams;
		
	double	chisq;
	double 	swap;

	private:
	
	//double	**alpha;
	//double	**covar;
	
		mat alpha;
		mat covar;

	void	mrqmin(double x[], double y[], double sig[], int ndata, double a[], int ia[],int ma,  double *chisq, long double *alamda);	
	void 	mrqcof(double x[], double y[], double sig[], int ndata, double a[], int ia[],int ma, mat &alpha, vec &beta, double *chisq);
	void 	covsrt(mat &covar, int ma, int ia[], int mfit);	
	void 	gaussj(mat &a, int n, mat &b, int m);


	public slots:

	void	SlotStopOptimization();

	signals:
	void	Updated();

};

#endif //   OPTIMIZATION_MARQUARDT
