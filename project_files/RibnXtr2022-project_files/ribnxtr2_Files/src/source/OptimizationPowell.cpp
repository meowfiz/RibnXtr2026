#include "OptimizationPowell.h"

OptimizationPowell::OptimizationPowell(double *_Params,int _NumberOfParameters,double _Tolerance,
									   int *_IterationNumber,double *_FunctionValue,Function *_F,double _ZEPS)
{

	
		ZEPS=_ZEPS;
		Params=_Params;
		NumberOfParameters=_NumberOfParameters;//poprawka na numerical recipecs!
		Tolerance=_Tolerance;
		IterationNumber=_IterationNumber;
		FunctionValue=_FunctionValue;

		Func=_F;
		//function=funcs;
	//	double a=(*funcs)(1,2,3);


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


}

//------------------------------------------

void OptimizationPowell::StartOptimization()
{
	OptimizationStopped=false;

	powell(Params,alpha,NumberOfParameters,Tolerance, IterationNumber,FunctionValue);
}

//---------------------------------------

OptimizationPowell::~OptimizationPowell()
{
	Params=NULL;
	FunctionValue=NULL;
	Func=NULL;

	if (alpha!=NULL) 
	{
		int i;
		for (i = 0; i <= NumberOfParameters; i++)
			delete alpha[i];
		delete[] alpha;
	}
}

//---------------------------------------
void OptimizationPowell::powell(double p[],double **xi,int n,double ftol,int *iter,double *fret)
{
//	FILE *Do_opa;
	
	int i,ibig,j;

	double t,fptt,fp,del;

	double *pt,*ptt,*xit;
	pt=vector(1,n);
	ptt=vector(1,n);

	xit=vector(1,n);
	*fret=func(p);

	for (j=1;j<=n;j++)
		pt[j]=p[j];

	for (*iter=1;;(*iter)++)
	{
		

		fp=(*fret);
//		FILE *Do_opa;
//	Do_opa=fopen("c:\\powell.txt","a");fprintf(Do_opa,"Iter %d FVal=%lf\n",*iter,fp);fclose(Do_opa);

		ibig=0;
		del=0.0;
		for (i=1;i<=n;i++)
		{
			for (j=1;j<=n;j++)
				xit[j]=xi[j][i];

			fptt=(*fret);
			if (OptimizationStopped==true) 
			{
				free_vector(xit,1,n);
				free_vector(ptt,1,n);
				free_vector(pt,1,n);
				return;
			}
			linmin(p,xit,n,fret,ftol);

			if (fabs(fptt-(*fret)) > del)
			{
				del=fabs(fptt-(*fret));
				ibig=i;

			}

		}
		if ((2.0*fabs(fp-(*fret)) <= ftol*(fabs(fp)+fabs(*fret)))|| (OptimizationStopped==true) )
		{
			free_vector(xit,1,n);
			free_vector(ptt,1,n);
			free_vector(pt,1,n);
			return;
		}
		if (*iter == ITMAX) nrerror("Too many iterations in routine POWELL");

		for (j=1;j<=n;j++)
		{
			ptt[j]=2.0*p[j]-pt[j];
			xit[j]=p[j]-pt[j];
			pt[j]=p[j];

		}

		fptt=func(ptt);
		if (fptt < fp) 
		{
			t=2.0*(fp-2.0*(*fret)+fptt)*SQR(fp-(*fret)-del)-del*SQR(fp-fptt);
			if (t < 0.0) 
			{
				linmin(p,xit,n,fret,ftol);
				for (j=1;j<=n;j++)
					xi[j][ibig]=xit[j];
			}
		}
		
	}

}

 
//-----------------------------------------------

void OptimizationPowell::linmin
(double p[],double xi[],int n,double *fret,double tol)
{

	int j;

	double xx,xmin,fx,fb,fa,bx,ax;

	ncom=n;
	pcom=vector(1,n);
	xicom=vector(1,n);

	for (j=1;j<=n;j++)
	{
		pcom[j]=p[j];
		xicom[j]=xi[j];
	}

	ax=0.0;
	xx=1.0;
	bx=2.0;

	mnbrak(&ax,&xx,&bx,&fa,&fx,&fb);
	*fret=brent(ax,xx,bx,tol,&xmin);

	for (j=1;j<=n;j++)
	{
		xi[j] *= xmin;
		p[j] += xi[j];
	}

	free_vector(xicom,1,n);
	free_vector(pcom,1,n);

}

 //-----------------------------------------------

void OptimizationPowell::mnbrak(double *ax,double *bx,double *cx,double *fa,double *fb,double *fc)

{

	double ulim,u,r,q,fu,dum;
	*fa=f1dim(*ax);
	*fb=f1dim(*bx);

	if (*fb > *fa)
	{
		SHFT(dum,*ax,*bx,dum)
		SHFT(dum,*fb,*fa,dum)
	}

	*cx=(*bx)+GOLD*(*bx-*ax);
	*fc=f1dim(*cx);

	while ((*fb > *fc))
	{
		r=(*bx-*ax)*(*fb-*fc);
		q=(*bx-*cx)*(*fb-*fa);
		u=(*bx)-((*bx-*cx)*q-(*bx-*ax)*r)/(2.0*SIGN(MAX(fabs(q-r),TINY),q-r));
		ulim=(*bx)+GLIMIT*(*cx-*bx);
		if ((*bx-u)*(u-*cx) > 0.0)
		{
			fu=f1dim(u);

			if ((fu < *fc)||(OptimizationStopped==true) )
			{
				*ax=(*bx);
				*bx=u;
				*fa=(*fb);
				*fb=fu;
				return;
			}
			else if (fu > *fb)
			{
				*cx=u;
				*fc=fu;
				return;
			}

			u=(*cx)+GOLD*(*cx-*bx);
			fu=f1dim(u);

		}
		else if ((*cx-u)*(u-ulim) > 0.0)
		{
			fu=f1dim(u);
			if (fu < *fc)
			{
				SHFT(*bx,*cx,u,*cx+GOLD*(*cx-*bx))
				SHFT(*fb,*fc,fu,f1dim(u))
			}

		} 
		else if ((u-ulim)*(ulim-*cx) >= 0.0)
		{
			u=ulim;
			fu=f1dim(u);
		} 
		else
		{
			u=(*cx)+GOLD*(*cx-*bx);
			fu=f1dim(u);
		}

		SHFT(*ax,*bx,*cx,u)
		SHFT(*fa,*fb,*fc,fu)

	}

} 

 //-----------------------------------------------

double  OptimizationPowell::brent(double ax,double bx,double cx,double tol,double *xmin)
{

	int iter;
	double a,b,d=0,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
	double e=0.0;

	a=((ax < cx) ? ax : cx);
	b=((ax > cx) ? ax : cx);
	x=w=v=bx;
	fw=fv=fx=f1dim(x);

	for (iter=1;iter<=ITMAX;iter++)
	{
		xm=0.5*(a+b);
		tol2=2.0*(tol1=tol*fabs(x)+ZEPS);
		if ((fabs(x-xm) <= (tol2-0.5*(b-a)))||(OptimizationStopped==true) )
		{
			*xmin=x;
			return fx;
		}

		if (fabs(e) > tol1) 
		{
			r=(x-w)*(fx-fv);
			q=(x-v)*(fx-fw);
			p=(x-v)*q-(x-w)*r;
			q=2.0*(q-r);
			if (q > 0.0) p = -p;
			q=fabs(q);
			etemp=e;
			e=d;
			if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
				d=CGOLD*(e=(x >= xm ? a-x : b-x));
			else
			{
				d=p/q;
				u=x+d;
				if (u-a < tol2 || b-u < tol2)
					d=SIGN(tol1,xm-x);
			}

		}
		else 
		{
			d=CGOLD*(e=(x >= xm ? a-x : b-x));
		}

		u=(fabs(d) >= tol1 ? x+d : x+SIGN(tol1,d));
		fu=f1dim(u);
		if (fu <= fx) 
		{
			if (u >= x) a=x; else b=x;
			SHFT(v,w,x,u)
			SHFT(fv,fw,fx,fu)

		} 
		else 
		{
			if (u < x) a=u; else b=u;
			if (fu <= fw || w == x)
			{
				v=w;
				w=u;
				fv=fw;
				fw=fu;

			} 
			else if (fu <= fv || v == x || v == w)
			{
				v=u;
				fv=fu;
			}
		}
	}

	nrerror("Too many iterations in BRENT");

	*xmin=x;

	return fx;

}
 
 //-----------------------------------------------

double   OptimizationPowell:: f1dim(double x)
{

	int j;

	double f,*xt;
	xt=vector(1,ncom);
	for (j=1;j<=ncom;j++)
		xt[j]=pcom[j]+x*xicom[j];

	f=func(xt);
	free_vector(xt,1,ncom);
	return f;

}
 //-----------------------------------------------

double *OptimizationPowell::vector(int nl,int nh)
{
	double *v;
	v=(double *)malloc((unsigned) (nh-nl+1)*sizeof(double));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl;
}
 //-----------------------------------------------

double OptimizationPowell::func(double TT[])
{		
	double Value=Func->GetFunctionValue(TT); 
	emit SignalUpdatedTrans(TT);
	//FILE *Do_opa=fopen("c:\\nr.txt","a");fprintf(Do_opa,"%lf %lf %lf %lf %lf %lf \n %lf \n",Value,TT[1],TT[2],TT[3],TT[4],TT[5],TT[6]);fclose(Do_opa);
	return Value;
}

void OptimizationPowell::nrerror(char error_text[])
{
//	void exit();
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}
//------------------------------------------------------------
void OptimizationPowell::free_vector(double *v,int nl, int nh)
{
	free((char*) (v+nl));
}
//------------------------------------------------------------
//------------------------------------------------------------
void OptimizationPowell::SlotStopOptimization()
{
	OptimizationStopped=true;
}

