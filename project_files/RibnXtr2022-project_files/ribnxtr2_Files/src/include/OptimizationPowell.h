#ifndef	OPTIMIZATION_POWELL
#define	OPTIMIZATION_POWELL

#include	<qapplication.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"OtherCommonFunctions.h"
#include	"AbstractFunctionClasses.h"

#define ITMAX 200
#define TOL 2.0e-4 
#define CGOLD 0.3819660
//#define ZEPS 1.0e-10
#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define GOLD 1.618034
#define GLIMIT 100.0
#define TINY 1.0e-20
static double sqrarg;
 
#ifndef SQR
#define SQR(a) (sqrarg=(a),sqrarg*sqrarg)
#endif
//class Registration;

class OptimizationPowell   : public QObject
{
 
	 Q_OBJECT

 
	public:


	OptimizationPowell(double *_Params,int _NumberOfParameters,double _Tolerance,
									   int *_IterationNumber,double *_FunctionValue,Function *_F,double _ZEPS=1.0e-10);
	~OptimizationPowell();

	void StartOptimization();

	Function *Func;
		
	double	*Params;
	int		NumberOfParameters;
	double	Tolerance;
	int		*IterationNumber;
	double	*FunctionValue;

	double **alpha;

	//te 2 nizej maja byc zerami
	double	*pcom,*xicom,(*nrfunc)(double[]);
	int		ncom;
 
	double	ax,bx,cx,tol,*xmin;

//	double	(*f)();	/* ANSI: double (*f)(double); */ 

	//POWELL

//	double (*function)(double *);

	void	powell(double *p,double **xi,int n,double ftol,int *iter,double *fret);
	void	linmin(double p[],double xi[],int n,double *fret,double tol);
	void	mnbrak(double *ax,double *bx,double *cx,double *fa,double *fb,double *fc);
	double	brent(double ax,double bx,double cx,double tol,double *xmin);
	double	f1dim(double x);
	double	*vector(int nl,int nh);
	void	nrerror(char error_text[]);
	void	free_vector(double *v,int nl, int nh);
	double	func(double TT[]);


	bool OptimizationStopped;

	void setZEPS(double x){ZEPS=x;};

	private:
		
		double ZEPS;

	//potem zamienic ewentualnie na bool
	public slots:

	void	SlotStopOptimization();

	signals:
	
	void	SignalUpdatedTrans(double *);
	
};	
#endif //   OPTIMIZATION_POWELL
