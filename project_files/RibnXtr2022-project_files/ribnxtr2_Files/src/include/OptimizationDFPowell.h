#ifndef	OPTIMIZATION_DFPOWELL
#define	OPTIMIZATION_DFPOWELL

#include	<qapplication.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"OtherCommonFunctions.h"
#include	"AbstractFunctionClasses.h"

#define ITMAX 200 //Maximum allowed number of iterations.
#define EPS8 3.0e-8 //Machine precision.
#define TOLX (4*EPS8) //Convergence criterion on x values.
#define STPMX 100.0 //Scaled maximum step length allowed in line searches.
#define FREEALL free_vector(xi,1,n);free_vector(pnew,1,n); \
	free_matrix(hessin,1,n,1,n);free_vector(hdg,1,n);free_vector(g,1,n); \
	free_vector(dg,1,n);
#define ALF 1.0e-4 //Ensures sufficient decrease in function value.
#define NR_END 1
#define FREE_ARG char*

static double maxarg1,maxarg2;
#define FMAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
(maxarg1) : (maxarg2))

 //static double sqrarg;
//#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)

/*
#define ITMAX 200
#define TOL 2.0e-4 
#define CGOLD 0.3819660
//#define ZEPS 1.0e-10
#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define GOLD 1.618034
#define GLIMIT 100.0
#define TINY 1.0e-20*/

//static double sqrarg_b;
//#define SQR_B(a) (sqrarg_b=(a),sqrarg_b*sqrarg_b)
 
//class Registration;

class OptimizationDFPowell   : public QObject
{
 
	 Q_OBJECT

 
	public:


	OptimizationDFPowell(double *_Params,int _NumberOfParameters,double _Tolerance,
									   int *_IterationNumber,double *_FunctionValue,Function *_F);
	~OptimizationDFPowell();

	void StartOptimization();

	Function *Func;
		
	double	*Params;
	int		NumberOfParameters;
	double	Tolerance;
	int		*IterationNumber;
	double	*FunctionValue;

	//double **alpha;


	void lnsrch(int n, double xold[], double fold, double g[], double p[], double x[],
			double *f, double stpmax, int *check);

	void dfpmin(double p[], int n, double gtol, int *iter, double *fret);
			//double(*func)(double []), void (*dfunc)(double [], double []))
/*
	//te 2 nizej maja byc zerami
	double	*pcom,*xicom,(*nrfunc)(double[]);
	int		ncom;
 
	double	ax,bx,cx,tol,*xmin;
*/
//	double	(*f)();	/* ANSI: double (*f)(double); */ 

	//POWELL

//	double (*function)(double *);
/*
	void	powell(double *p,double **xi,int n,double ftol,int *iter,double *fret);
	void	linmin(double p[],double xi[],int n,double *fret,double tol);
	void	mnbrak(double *ax,double *bx,double *cx,double *fa,double *fb,double *fc);
	double	brent(double ax,double bx,double cx,double tol,double *xmin);

	double	f1dim(double x);
*/
	//matrix
	double	*vector(int nl,int nh);
	double **matrix(long nrl, long nrh, long ncl, long nch);

	void	nrerror(char error_text[]);
	void	free_vector(double *v,int nl, int nh);
	void	free_matrix(double **m, long nrl, long nrh, long ncl, long nch);
	double	func(double TT[]);


	bool OptimizationStopped;

//	void setZEPS(double x){ZEPS=x;};

	private:
		double *dg,*g,*hdg,**hessin,*pnew,*xi;
		double ZEPS;

	//potem zamienic ewentualnie na bool
	public slots:

	void	SlotStopOptimization();

	signals:
	
	void	SignalUpdatedTrans(double *);
	
};	
#endif //   OPTIMIZATION_DFPOWELL
