#include "OptimizationMarquardt.h"

OptimizationMarquardt::OptimizationMarquardt(double *_x, double *_y, double *_sig, int _NPoint, double *_a,int *_ia,int _ma,Function *_F)
{

	ExitEnabled=false;

	XValues=_x;
	YValues=_y;
	SigmaValues=_sig;
	NPoint=_NPoint;
	Params=_a;
	BlockParams=_ia;
	NParams=_ma;

	Func=_F;



	int i,j;


	

	int Lsize=NParams;
	alpha = mat(Lsize, Lsize,		fill::zeros);
	covar= mat(Lsize, Lsize, fill::zeros);
	//arma::mat M = arma::mat(Lsize, LSize);
	/*alpha = new double*[Lsize];
	for (i = 0; i < (Lsize); i++)
		alpha[i] = new double[Lsize];

	covar = new double*[Lsize];
	for (i = 0; i < (Lsize); i++)
		covar[i] = new double[Lsize];

	for (i = 0; i < (Lsize); i++)
		for (j = 0; j < (Lsize); j++)
		{
			covar[i][j]=0;
			alpha[i][j]=0;
		}*/

}
//-------------------------------------------------
OptimizationMarquardt::~OptimizationMarquardt(void)
{
	//int i;
	//int Lsize=NParams;
	////alpha = new double*[Lsize];
	//for (i = 0; i < (Lsize); i++)
	//	delete []alpha[i];//
	//delete []alpha;
	////= new double[Lsize];

	////covar = new double*[Lsize];
	//for (i = 0; i < (Lsize); i++)
	//	delete []covar[i];
	//delete []covar;
		//covar[i] = new double[Lsize];

}
//---------------------------------------------
void OptimizationMarquardt::StartOptimization(double tol)
{

	int i;
	//		int NrIter;
	chisq=0;
	long double alamda=-0.01;  
	double oldchisq=0;
	int counter=0;

	//double tol=1e-6;
	for(i=0;;i++)
	{
		double valx = fabs(oldchisq - chisq);
		

		//if ((fabs(oldchisq-chisq)<tol))
		 if ((chisq==0)||(valx/chisq<0.001)||(valx<tol))
			counter++;
		else
			counter=0;
		oldchisq=chisq;



		//Stopping criterion
		if (counter>3)
		{
			alamda=0;
			

			mrqmin(XValues,YValues,SigmaValues,NPoint-1,Params,BlockParams,NParams,&chisq,&alamda);		
			emit Updated();
			break;		
		}

		mrqmin(XValues,YValues,SigmaValues,NPoint-1,Params,BlockParams,NParams,&chisq,&alamda);		
		emit Updated();
	}
}
//---------------------------------------------------
void OptimizationMarquardt::mrqmin(double x[], double y[], double sig[], int ndata, double a[]
, int ia[],int map1, double *chisq, long double *alamda)
{	
	//qDebug() << "in mrqmin  ";
	int ma=map1-1;
	int i,j,k,l;
	static int mfit;
	//static 
	static	double ochisq;
	//*atry, * beta, * da, ** oneda;

	static double* atry = NULL;
	static vec  beta, da;
	static mat oneda;

	if (*alamda < 0.0)
	{
		//atry = vec(ma + 1);
		beta = vec(ma + 1);
		da = vec(ma + 1);
		atry=new  double[ma+1];
		/*beta=new  double[ma+1];
		da=new  double[ma+1];*/
		for (mfit=0,j=1;j<=ma;j++)
			if (ia[j]) mfit++;		

		oneda = mat(ma + 1, ma + 1);
		/*oneda = new double*[ma+1];
		for (i = 0; i < (ma+1); i++)
			oneda[i] = new double[ma+1];	*/
		for (i = 0; i < (ma + 1); i++)
		{

		}

		*alamda=0.0001;

		mrqcof(x,y,sig,ndata,a,ia,ma,alpha,beta,chisq);

		ochisq=(*chisq);
		for (j=1;j<=ma;j++) atry[j]=a[j];
	}

	//qDebug() << "in mrqmin  1";
	for (j=1;j<=mfit;j++)
	{
		//qDebug() << "j "<<j;
		//qDebug() << "in mrqmin  11";
		//covar(i, j) = alpha(i, j);
		for (k = 1;k <= mfit;k++)covar(j, k) = alpha(j, k);//covar[j][k] = alpha[j][k];
		//qDebug() << "in mrqmin  12";
		covar(j, j) = alpha(j, j) * (1.0 + (*alamda));;
		//covar[j][j]=alpha[j][j]*(1.0+(*alamda));
		//oneda[j][1]=beta[j];
		//qDebug() << "in mrqmin  13 " <<j;
		oneda(j, 1) = beta(j);
	}
	
	//try
	//{
	//qDebug() << "in mrqmin  2";
		gaussj(covar,mfit,oneda,1);
	//}
	//catch(...)
	//{
		//i=i;
	//}



	if (ExitEnabled==true) return;
	//qDebug() << "in mrqmin  3";
	for (j = 1;j <= mfit;j++) da[j] = oneda(j, 1);//oneda[j][1];
	if (*alamda == 0.0)
	{
		covsrt(covar,ma,ia,mfit);
		covsrt(alpha,ma,ia,mfit);

		/*for (i = 0; i < (ma+1); i++)
			delete oneda[i];

		delete[]oneda;
		delete[] da;
		delete[] atry;
		delete[] beta;*/
		//delete[] atry;//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
		return;
	}

	for (j=0,l=1;l<=ma;l++)
		if (ia[l]) atry[l]=a[l]+da[++j];

	//qDebug() << "in mrqmin  4";
	mrqcof(x,y,sig,ndata,atry,ia,ma,covar,da,chisq);

	//qDebug() << "in mrqmin  5";
	if (*chisq < ochisq)
	{
		*alamda *= (double)(1/10.0);
		ochisq=(*chisq);
		for (j=1;j<=mfit;j++)
		{
			for (k=1;k<=mfit;k++) alpha(j,k) = covar(j,k);//alpha[j][k]=covar[j][k];
			beta[j]=da[j];
		}
		for (l=1;l<=ma;l++) a[l]=atry[l];
	}
	else
	{
		*alamda *= 10.0;
		*chisq=ochisq;
	}
	//qDebug() << "out mrqmin  ";
}
///-----------------------------------------------------------
//------------------------------------------------------------------
void OptimizationMarquardt::mrqcof(double x[], double y[], double sig[], int ndata, double a[],int ia[],int ma, mat &alpha, vec &beta, double *chisq)
{
	//qDebug() << "a " ;
	int i,j,k,l,m,mfit=0;
	double ymod, wt, sig2i, dy,*dyda;

	//vec dyda = vec(ma + 1);
	dyda=new double[ma+1];

	for (j=1;j<=ma;j++)
		if (ia[j]) mfit++;
	for (j=1;j<=mfit;j++) {
		for (k=1;k<=j;k++) alpha(j,k)=0.0;
		beta[j]=0.0;
	}
	*chisq=0.0;
	//qDebug() << "b ";
	//	deriv->CalculateTrigonometricParams(a);
	//	trans->CalculateTrigonometricFunctions(a);
	Func->PrecalculateVariables(a);

	for (i=1;i<=ndata;i++)
	{

		ymod=Func->GetFunctionValue(x[i],a);
		Func->CalculateDerivative(dyda,a);
		//OptimizeFunction(x[i],a,&ymod,dyda,dist,deriv,trans);
		sig2i=1.0/(sig[i]*sig[i]);
		dy=y[i]-ymod;
		for (j=0,l=1;l<=ma;l++) 
		{
			if (ia[l]) {
				wt=dyda[l]*sig2i;
				for (j++,k=0,m=1;m<=l;m++)
					if (ia[m]) alpha(j,++k) += wt * dyda[m];//alpha[j][++k] += wt*dyda[m];
				beta[j] += dy*wt;
			}
		}
		*chisq += dy*dy*sig2i;
	}
	//qDebug() << "c ";
	/*if ((*chisq<29))
	{
		j=j;
	}*/
	//FILE *Do_opa;	
	//	Do_opa=fopen("c:\\mrq.txt","a");fprintf(Do_opa,"%lf\n",*chisq);fclose(Do_opa);
	//qDebug() << "d ";
	for (j=2;j<=mfit;j++)
		for (k=1;k<j;k++) alpha(k,j) = alpha(j,k);//alpha[k][j]=alpha[j][k];

	delete[] dyda;

}
//------------------------------------------------------------------
void OptimizationMarquardt::covsrt(mat &covar, int ma, int ia[], int mfit)
{
	//qDebug() << "in covsrt ";

	int i,j,k;
	double swap;
	for (i = mfit + 1;i <= ma;i++)

		for (j = 1;j <= i;j++) covar(i, j) = covar(j, i) = 0;//covar[i][j]=covar[j][i]=0.0;
	k=mfit;
	for (j=ma;j>=1;j--)
	{
		if (ia[j]) 
		{
			for (i=1;i<=ma;i++) SWAP(covar(i,k), covar(i,j))//SWAP(covar[i][k],covar[i][j])
				for (i=1;i<=ma;i++) SWAP(covar(k,i), covar(j,i))//SWAP(covar[k][i],covar[j][i])
					k--;
		}
	}
	//qDebug() << "out covsrt ";

}
//------------------------------------------------------------------
void OptimizationMarquardt::gaussj(mat &a, int n, mat &b, int m)
{

	//qDebug() << "in gauss ";
	//int *indxc,*indxr,*ipiv;
	vec indxc, indxr, ipiv;
	int i,icol,irow,j,k,l,ll;
	double big,dum,pivinv;



	indxc=vec(n+1);
	indxr=vec(n+1);
	ipiv=vec(n+1);



	for (j=1;j<=n;j++) ipiv[j]=0;
	for (i=1;i<=n;i++) 
	{
		big=0.0;
		for (j=1;j<=n;j++)
			if (ipiv[j] != 1)
				for (k=1;k<=n;k++) 
				{
					if (ipiv[k] == 0)
					{
						if (fabs(a(j,k)) >= big) 
						{
							big=fabs(a(j,k));
							irow=j;
							icol=k;
						}
					} else if (ipiv[k] > 1)
					{

						ExitEnabled=true;
						return;
					}		

				}
				++(ipiv[icol]);
				if (irow != icol) 
				{
					for (l=1;l<=n;l++) SWAP(a(irow,l),a(icol,l))

						for (l=1;l<=m;l++) SWAP(b(irow,l),b(icol,l))
				}
				indxr[i]=irow;
				indxc[i]=icol;
				if (a(icol,icol) == 0.0) 
				{
					ExitEnabled=true;
					return;
				}	
				pivinv=1.0/a(icol,icol);
				a(icol,icol)=1.0;
				for (l=1;l<=n;l++) a(icol,l) *= pivinv;

				for (l=1;l<=m;l++) 
				{	
					b(icol,l) *= pivinv;
				}

				for (ll=1;ll<=n;ll++)
					if (ll != icol)
					{
						dum=a(ll,icol);
						a(ll,icol)=0.0;
						for (l=1;l<=n;l++) a(ll,l) -= a(icol,l)*dum;
						for (l=1;l<=m;l++) b(ll,l) -= b(icol,l)*dum;
					}
	}
	for (l=n;l>=1;l--) 
	{
		if (indxr[l] != indxc[l])
			for (k=1;k<=n;k++)
				SWAP(a(k,indxr[l]),a(k,indxc[l]));
	}    


	/*delete[] ipiv;
	delete[] indxr;
	delete[] indxc;*/

	//qDebug() << "out gauss ";
}
//------------------------------------------------------------------
void OptimizationMarquardt::SlotStopOptimization()
{
	ExitEnabled=true;
}
//------------------------------------------------------------------
void	Derivatives::CalculateDerivatives(double *Derivative,double X,double Y,double Z,double DistX,double DistY,double DistZ,double *Trans)
{

	// first 3 derivatives are computed from a distance map vectors
	Derivative[1]=DistX;
	Derivative[2]=DistY;
	Derivative[3]=DistZ;

	//other 3 are computed - by multyplying derivatives of rotation matrix ,source point&   distance map vectors
	Derivative[4]=(X*xtr1+Y*xtr2+Z*xtr3)*DistX
		+(X*xtr4+Y*xtr5+Z*xtr6)*DistY
		+(X*xtr7+Y*xtr8+Z*xtr9)*DistZ;	



	Derivative[5]= (X*ytr1+Y*ytr2+Z*ytr3)*DistX
		+(X*ytr4+Y*ytr5+Z*ytr6)*DistY
		+(X*ytr7+Y*ytr8+Z*ytr9)*DistZ;

	Derivative[6]=	 (X*ztr1+Y*ztr2+Z*ztr3)*DistX
		+(X*ztr4+Y*ztr5+Z*ztr6)*DistY
		+(X*ztr7+Y*ztr8+Z*ztr9)*DistZ;

}
//------------------------------------------------------
void	Derivatives::CalculateTrigonometricParams(double *Trans)
{

	// precomputing trigonometric functions - to speed up calculations
	sx=(double)sin(Trans[4]);cx=(double)cos(Trans[4]);
	sy=(double)sin(Trans[5]);cy=(double)cos(Trans[5]);
	sz=(double)sin(Trans[6]);cz=(double)cos(Trans[6]);

	/*	xtr1=(cx*sy*cz+sx*sz); 
	xtr2=(cx*sy*sz-sx*cz);
	xtr3=cx*cy;
	xtr4=(-sx*sy*cz+cx*sz);
	xtr5=(-sx*sy*sz-cx*cz);
	xtr6=-sx*cy;


	ytr1=(-sy*cz);
	ytr2=(-sy*sz);
	ytr3=(-cy);
	ytr4=(sx*cy*cz);
	ytr5=(sx*cy*sz);
	ytr6=(-sx*sy);
	ytr7=(cx*cy*cz);
	ytr8=(cx*cy*sz);
	ytr9=(-cx*sy);

	ztr1=(-cy*sz);
	ztr2=(cy*cz);
	ztr3=(-sx*sy*sz-cx*cz);
	ztr4=(sx*sy*cz-cx*sz);
	ztr5=(-cx*sy*sz+sx*cz);
	ztr6=(cx*sy*cz+sx*sz);
	*/
	/*	
	sz*cx*sy                -sz*sx                sz*cx*cy
	cz*cx*sy                -cz*sx                cz*cx*cy
	-sx*sy               	 -cx                -sx*cy


	-cz*sy+sz*sx*cy                0                -cz*cy-sz*sx*sy 
	sz*sy+cz*sx*cy                0                sz*cy-cz*sx*sy
	cx*cy                0                -cx*sy



	-sz*cy+cz*sx*sy                cz*cx                sz*sy+cz*sx*cy
	-cz*cy-sz*sx*sy                -sz*cx                cz*sy-sz*sx*cy

	*/

	//ZXYP


	xtr1=sz*cx*sy;
	xtr2=-sz*sx;
	xtr3=sz*cx*cy;
	xtr4=cz*cx*sy;
	xtr5=-cz*sx;
	xtr6=cz*cx*cy;
	xtr7=-sx*sy;
	xtr8=-cx;
	xtr9=-sx*cy;

	ytr1=-cz*sy+sz*sx*cy;
	ytr2=0;
	ytr3= -cz*cy-sz*sx*sy;
	ytr4=sz*sy+cz*sx*cy ;
	ytr5=0;
	ytr6=sz*cy-cz*sx*sy;
	ytr7=cx*cy;
	ytr8=0;
	ytr9=-cx*sy;

	ztr1=-sz*cy+cz*sx*sy;
	ztr2=cz*cx;
	ztr3=sz*sy+cz*sx*cy;
	ztr4=-cz*cy-sz*sx*sy;
	ztr5=-sz*cx;
	ztr6=cz*sy-sz*sx*cy;
	ztr7=0;
	ztr8=0;
	ztr9=0;


}
