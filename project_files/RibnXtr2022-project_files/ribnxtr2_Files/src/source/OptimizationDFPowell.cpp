#include "OptimizationDFPowell.h"

OptimizationDFPowell::OptimizationDFPowell(double *_Params,int _NumberOfParameters,double _Tolerance,
										   int *_IterationNumber,double *_FunctionValue,Function *_F)//,double _ZEPS)
{


	//ZEPS=_ZEPS;
	Params=_Params;
	NumberOfParameters=_NumberOfParameters;//poprawka na numerical recipecs!
	Tolerance=_Tolerance;
	IterationNumber=_IterationNumber;
	FunctionValue=_FunctionValue;

	Func=_F;
	//function=funcs;
	//	double a=(*funcs)(1,2,3);
	/*
	int i,j;
	int Lsize=NumberOfParameters+1;

	alpha = new double*[Lsize];
	for (i = 0; i < (Lsize); i++)
	alpha[i] = new double[Lsize];

	for (i = 0; i < (Lsize); i++)
	for (j = 0; j < (Lsize); j++)
	alpha[i][j]=0.0;

	for (i = 0; i < (Lsize); i++)
	alpha[i][i]=1.0;
	*/

}

//------------------------------------------

void OptimizationDFPowell::StartOptimization()
{

	OptimizationStopped=false;
	try
	{
		dfpmin(Params, NumberOfParameters,Tolerance, IterationNumber, FunctionValue);
	}
	catch(...)
	{

	}
	//powell(Params,alpha,NumberOfParameters,Tolerance, IterationNumber,FunctionValue);
}
//---------------------------------------
OptimizationDFPowell::~OptimizationDFPowell()
{
	Params=NULL;
	FunctionValue=NULL;
	Func=NULL;

	/*if (alpha!=NULL) 
	{
	int i;
	for (i = 0; i <= NumberOfParameters; i++)
	delete alpha[i];
	delete[] alpha;
	}*/
}
//---------------------------------------

//-----------------------------------------------

double *OptimizationDFPowell::vector(int nl,int nh)
{

	double *v;
	v=(double *)malloc((unsigned) (nh-nl+1)*sizeof(double));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl;

}
//-----------------------------------------------

double OptimizationDFPowell::func(double TT[])
{

	double Value=Func->GetFunctionValue(TT); 
	emit SignalUpdatedTrans(TT);
	//FILE *Do_opa=fopen("c:\\nr.txt","a");fprintf(Do_opa,"%lf %lf %lf %lf %lf %lf \n %lf \n",Value,TT[1],TT[2],TT[3],TT[4],TT[5],TT[6]);fclose(Do_opa);

	return Value;
}

void OptimizationDFPowell::nrerror(char error_text[])
{

	//	void exit();



	fprintf(stderr,"Numerical Recipes run-time error...\n");

	fprintf(stderr,"%s\n",error_text);

	fprintf(stderr,"...now exiting to system...\n");

	exit(1);

}
//------------------------------------------------------------
void OptimizationDFPowell::free_vector(double *v,int nl, int nh)
{

	free((char*) (v+nl));

}
//------------------------------------------------------------
//------------------------------------------------------------
void OptimizationDFPowell::SlotStopOptimization()
{
	OptimizationStopped=true;
}


//----------------------------------------------------------------
//#include <math.h>
//#include "nrutil.h"

void OptimizationDFPowell::dfpmin(double p[], int n, double gtol, int *iter, double *fret)
//double(*func)(double []), void (*dfunc)(double [], double []))
//	Given a starting point p[1..n] that is a vector of length n, the Broyden-Fletcher-Goldfarb-
//	Shanno variant of Davidon-Fletcher-Powell minimization is performed on a function func, using
//	its gradient as calculated by a routine dfunc. The convergence requirement on zeroing the
//	gradient is input as gtol. Returned quantities are p[1..n] (the location of the minimum),
//	iter (the number of iterations that were performed), and fret (the minimum value of the
//	function). The routine lnsrch is called to perform approximate line minimizations.

{
	//	void lnsrch(int n, double xold[], double fold, double g[], double p[], double x[],
	//		double *f, double stpmax, int *check, double (*func)(double []));
	int check,i,its,j;
	double den,fac,fad,fae,fp,stpmax,sum=0.0,sumdg,sumxi,temp,test;
	//double *dg,*g,*hdg,**hessin,*pnew,*xi;
	dg=vector(1,n);
	g=vector(1,n);
	hdg=vector(1,n);
	hessin=matrix(1,n,1,n);
	pnew=vector(1,n);
	xi=vector(1,n);

	fp=Func->GetFunctionValue(p);	
	///fp=(*func)(p); //Calculate starting function value and gra-
	//Func->CalculateDerivative(g,p);
	Func->CalculateDerivative(g,p);
	//fp=Func->GetLastValue(); //tymczasowy trick

	//(*dfunc)(p,g); //dient,
	for (i=1;i<=n;i++)
	{ //and initialize the inverse Hessian to the
		for (j=1;j<=n;j++) hessin[i][j]=0.0; //unit matrix.
		hessin[i][i]=1.0;
		xi[i] = -g[i]; //Initial line direction.
		sum += p[i]*p[i];
	}
	stpmax=STPMX*FMAX(sqrt(sum),(double)n);
	for (its=1;its<=ITMAX;its++)
	{// Main loop over the iterations.
		*iter=its;
		lnsrch(n,p,fp,g,xi,pnew,fret,stpmax,&check);//,func);
		//The new function evaluation occurs in lnsrch; save the function value in fp for the
		//	next line search. It is usually safe to ignore the value of check.
		fp = *fret;
		for (i=1;i<=n;i++) 
		{
			xi[i]=pnew[i]-p[i]; //Update the line direction,
			p[i]=pnew[i]; //and the current point.
		}
		test=0.0; //Test for convergence on ?x.
		for (i=1;i<=n;i++) 
		{
			temp=fabs(xi[i])/FMAX(fabs(p[i]),1.0);
			if (temp > test) test=temp;
		}
		if (test < TOLX)
		{
			//	FREEALL
			free_vector(xi,1,n);
			free_vector(pnew,1,n); 
			free_matrix(hessin,1,n,1,n);
			free_vector(hdg,1,n);
			free_vector(g,1,n); 
			free_vector(dg,1,n);		
			return;
		}
		for (i=1;i<=n;i++) dg[i]=g[i]; //Save the old gradient,
		Func->CalculateDerivative(g,p);
		//(*dfunc)(p,g); //and get the new gradient.
		test=0.0; //Test for convergence on zero gradient.
		den=FMAX(*fret,1.0);
		for (i=1;i<=n;i++)
		{
			temp=fabs(g[i])*FMAX(fabs(p[i]),1.0)/den;
			if (temp > test) test=temp;
		}
		if (test < gtol) 
		{
			free_vector(xi,1,n);
			free_vector(pnew,1,n); 
			free_matrix(hessin,1,n,1,n);
			free_vector(hdg,1,n);
			free_vector(g,1,n); 
			free_vector(dg,1,n);
			//FREEALL
			return;
		}
		for (i=1;i<=n;i++) dg[i]=g[i]-dg[i]; //Compute difference of gradients,
		for (i=1;i<=n;i++) 
		{ //and difference times current matrix.
			hdg[i]=0.0;
			for (j=1;j<=n;j++) hdg[i] += hessin[i][j]*dg[j];
		}
		fac=fae=sumdg=sumxi=0.0; //Calculate dot products for the denomifor
		for(i=1;i<=n;i++) 
		{// nators.
			fac += dg[i]*xi[i];
			fae += dg[i]*hdg[i];
			sumdg += dg[i]*dg[i];
			sumxi += xi[i]*xi[i];
		}
		if (fac > sqrt(EPS8*sumdg*sumxi)) 
		{ //Skip update if fac not sufficiently posi
			fac=1.0/fac;// tive.
			fad=1.0/fae;
			//The vector that makes BFGS different from DFP:
			for (i=1;i<=n;i++) dg[i]=fac*xi[i]-fad*hdg[i];
			for (i=1;i<=n;i++) 
			{// The BFGS updating formula:
				for (j=i;j<=n;j++) 
				{
					hessin[i][j] += fac*xi[i]*xi[j]
					-fad*hdg[i]*hdg[j]+fae*dg[i]*dg[j];
					hessin[j][i]=hessin[i][j];
				}
			}
		}
		for (i=1;i<=n;i++) 
		{ //Now calculate the next direction to go,
			xi[i]=0.0;
			for (j=1;j<=n;j++) xi[i] -= hessin[i][j]*g[j];
		}
	} //and go back for another iteration.
	nrerror("too many iterations in dfpmin");
	//FREEALL
	free_vector(xi,1,n);
	free_vector(pnew,1,n); 
	free_matrix(hessin,1,n,1,n);
	free_vector(hdg,1,n);
	free_vector(g,1,n); 
	free_vector(dg,1,n);
}
//---------------------------
/*#include <math.h>
#include "nrutil.h"
#define ALF 1.0e-4 Ensures sufficient decrease in function value.
#define TOLX 1.0e-7 Convergence criterion on ?x.*/
void OptimizationDFPowell::lnsrch(int n, double xold[], double fold, double g[], double p[], double x[],
								  double *f, double stpmax, int *check)// double (*func)(double []))
								  //	Given an n-dimensional point xold[1..n], the value of the function and gradient there, fold
								  //	and g[1..n], and a direction p[1..n], finds a new point x[1..n] along the direction p from
								  //	xold where the function func has decreased “sufficiently.” The new function value is returned
								  //	in f. stpmax is an input quantity that limits the length of the steps so that you do not try to
								  //	evaluate the function in regions where it is undefined or subject to overflow. p is usually the
								  //	Newton direction. The output quantity check is false (0) on a normal exit. It is true (1) when
								  //	x is too close to xold. In a minimization algorithm, this usually signals convergence and can
								  //	be ignored. However, in a zero-finding algorithm the calling program should check whether the
								  //	convergence is spurious. Some “difficult” problems may require double precision in this routine.
{
	int i;
	double a,alam,alam2,alamin,b,disc,f2,rhs1,rhs2,slope,sum,temp,
		test,tmplam;
	*check=0;
	for (sum=0.0,i=1;i<=n;i++) sum += p[i]*p[i];
	sum=sqrt(sum);
	if (sum > stpmax)
		for (i=1;i<=n;i++) p[i] *= stpmax/sum;// Scale if attempted step is too big.
	for (slope=0.0,i=1;i<=n;i++)
		slope += g[i]*p[i];
	if (slope >= 0.0) 
	{
		free_vector(xi,1,n);
		free_vector(pnew,1,n); 
		free_matrix(hessin,1,n,1,n);
		free_vector(hdg,1,n);
		free_vector(g,1,n); 
		free_vector(dg,1,n);	
		throw 0;
	}
	//nrerror("Roundoff problem in lnsrch.");
	test=0.0; //Compute ?min.
	for (i=1;i<=n;i++) 
	{
		temp=fabs(p[i])/FMAX(fabs(xold[i]),1.0);
		if (temp > test) test=temp;
	}
	alamin=TOLX/test;
	alam=1.0;// Always try full Newton step first.
	for (;;)
	{ 
		//Start of iteration loop.
		for (i=1;i<=n;i++) x[i]=xold[i]+alam*p[i];
		*f=Func->GetFunctionValue(x);
		//	*f=(*func)(x);
		if (alam < alamin) 
		{ 
			//Convergence on ?x. For zero finding,
			//the calling program should
			//verify the convergence.
			for (i=1;i<=n;i++) x[i]=xold[i];
			*check=1;
			return;
		} else if (*f <= fold+ALF*alam*slope) return;// Sufficient function decrease.
		else 
		{// Backtrack.
			if (alam == 1.0)
				tmplam = -slope/(2.0*(*f-fold-slope));// First time.
			else 
			{ //Subsequent backtracks.
				rhs1 = *f-fold-alam*slope;
				rhs2=f2-fold-alam2*slope;
				a=(rhs1/(alam*alam)-rhs2/(alam2*alam2))/(alam-alam2);
				b=(-alam2*rhs1/(alam*alam)+alam*rhs2/(alam2*alam2))/(alam-alam2);

				if (a == 0.0) tmplam = -slope/(2.0*b);
				else 
				{
					disc=b*b-3.0*a*slope;
					if (disc < 0.0) tmplam=0.5*alam;
					else if (b <= 0.0) tmplam=(-b+sqrt(disc))/(3.0*a);
					else tmplam=-slope/(b+sqrt(disc));
				}
				if (tmplam > 0.5*alam)
					tmplam=0.5*alam;// ? ? 0.5?1.
			}
		}
		alam2=alam;
		f2 = *f;
		alam=FMAX(tmplam,0.1*alam); //? ? 0.1?1.
	} //Try again.
}
//--------------------------------------
double **OptimizationDFPowell::matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;
	/* allocate pointers to rows */
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;
	/* allocate rows and set pointers to them */
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl]) 
	{	
		throw 0;
		//nrerror("allocation failure 2 in matrix()");

	}
	m[nrl] += NR_END;
	m[nrl] -= ncl;
	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
	/* return pointer to array of pointers to rows */
	return m;
}
//--------------------------
void OptimizationDFPowell::free_matrix(double **m, long nrl, long nrh, long ncl, long nch)
/* free a double matrix allocated by matrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
}