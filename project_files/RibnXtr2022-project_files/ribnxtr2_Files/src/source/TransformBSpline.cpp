
#include "TransformBSpline.h"

BSplineGridTransformation::BSplineGridTransformation(FloatGrid3D *_Grid,double _Nx,double _Ny,double _Nz,double _minx, double _miny,double _minz)
{
	Grid=_Grid;
	Nx=_Nx;
	Ny=_Ny;
	Nz=_Nz;
	GridNx=Grid->GetNx();
	GridNy=Grid->GetNy();
	GridNz=Grid->GetNz();

	BSpline[0]=&BSplineGridTransformation::B0;
	BSpline[1]=&BSplineGridTransformation::B1;
	BSpline[2]=&BSplineGridTransformation::B2;
	BSpline[3]=&BSplineGridTransformation::B3;

	BSplineDeriv[0]=&BSplineGridTransformation::dB0;
	BSplineDeriv[1]=&BSplineGridTransformation::dB1;
	BSplineDeriv[2]=&BSplineGridTransformation::dB2;
	BSplineDeriv[3]=&BSplineGridTransformation::dB3;

	minx=_minx;
	miny=_miny;
	minz=_minz;

	N1x=(double)(GridNx-1)/Nx;
	N1y=(double)(GridNy-1)/Ny;
	N1z=(double)(GridNz-1)/Nz;

	xScale=1.0/N1x;
	yScale=1.0/N1y;
	zScale=1.0/N1z;

	xSc=1.0;
	ySc=1.0;
	zSc=1.0;

	GridNx2=GridNx+2;
	GridNy2=GridNy+2;
	GridNz2=GridNz+2;

	SplineTab=new double [4*1000];
	SplineDerivTab=new double [4*1000];
	int i,j;
	int pos=0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<1000;j++)
		{
			SplineTab[pos]=(this->*BSpline[i])((double)j*0.001);
			SplineDerivTab[pos++]=(this->*BSplineDeriv[i])((double)j*0.001);
		}

	}	

}
//---------------------------------
void BSplineGridTransformation::SetupForwardAndBackwardScalingFactors(double fx,double fy,double fz,double bx,double by,double bz)
{
	N1x=(double)(GridNx-1)/fx;
	N1y=(double)(GridNy-1)/fy;
	N1z=(double)(GridNz-1)/fz;

	xScale=bx/(double)(GridNx-1);
	yScale=by/(double)(GridNy-1);
	zScale=bz/(double)(GridNz-1);

	xSc=fx/bx;
	ySc=fy/by;
	zSc=fz/bz;
}

//------------------------------------------------
BSplineGridTransformation::~BSplineGridTransformation()
{
	Grid=NULL;
	if (SplineTab!=NULL)
	{
		delete []SplineTab;
		SplineTab=NULL;
		delete []SplineDerivTab;
		SplineDerivTab=NULL;

	}
}

void BSplineGridTransformation::TransformPoint(double *PIn,double *POut,double *def)
{
	Deformation(PIn,POut);

	if (def!=NULL)
	{def[0]=POut[0];def[1]=POut[1];def[2]=POut[2];}

	POut[0]+=PIn[0];
	POut[1]+=PIn[1];
	POut[2]+=PIn[2];


}
//---------------------------------
void BSplineGridTransformation::TransformPoint(double *PIn,double *POut,double amount,double *def)
{
	Deformation(PIn,POut);

	if (def!=NULL)
	{def[0]=POut[0]*amount;def[1]=POut[1]*amount;def[2]=POut[2]*amount;} //zmiana

	POut[0]=PIn[0]+POut[0]*amount;
	POut[1]=PIn[1]+POut[1]*amount;
	POut[2]=PIn[2]+POut[2]*amount;


}

//--------------------------------------------------------
void BSplineGridTransformation::DeformationOrig(double xx,double yy,double zz,double *def0)
{
	int i,j,k;
	int I,J,K;
	int it,jt,kt;
	double tmp;
	double sk,sj;
	int pos;
	int pz,py,px;
	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();
	int		maxX,maxY,maxZ;

	def0[0]=0;				def0[1]=0;				def0[2]=0;
	int l=(int)(xx);		int m=(int)(yy);		int n=(int)(zz);
	double u= xx-(int)xx;	double v= yy-(int)yy;	double w= zz-(int)zz;
	int uu=(int)(u*1000);	int vv=(int)(v*1000);	int ww=(int)(w*1000);
	it=uu;	jt=vv;	kt=ww;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	maxZ=(n+3<GridNz2)?4:GridNz2-n;

	int II=l;
	int JJ=m;
	K=n;
	pz=K*Nxy2;
	int JJJ=JJ*Nx2;

	for (k=0;k<maxZ;k++)
	{
		sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
		for(j=0;j<maxY;j++)
		{
			sj=sk*SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				tmp=sj*SplineTab[it];
				if (px<0)
				{				
					px=px;
				}
				def0[0]+=tmp*Grid->GetDXNP(px);
				def0[1]+=tmp*Grid->GetDYNP(px);
				def0[2]+=tmp*Grid->GetDZNP(px);
				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
		jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
	}

}

//-------------------------------------------
void BSplineGridTransformation::TransformPoint(int x,int y, int z,double *POut)
{
	int		i,j,k;
	int		I,J,K;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[3];
	double	def0[3];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	gridPos[0]=x*N1x;				gridPos[1]=y*N1y;				gridPos[2]=z*N1z;
	def0[0]=0;						def0[1]=0;						def0[2]=0;
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		int n=(int)(gridPos[2]);
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			double w=gridPos[2]-n;
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			int ww=(int)(w*1000);
	int it=uu;						int jt=vv;						int kt=ww;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	maxZ=(n+3<GridNz2)?4:GridNz2-n;

	int II=l;	int JJ=m;	
	K=n;	pz=K*Nxy2;	int JJJ=JJ*Nx2;

	for (k=0;k<maxZ;k++)
	{
		sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
		for(j=0;j<maxY;j++)
		{
			sj=sk*SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				tmp=sj*SplineTab[it];
				def0[0]+=tmp*Grid->GetDXNP(px);
				def0[1]+=tmp*Grid->GetDYNP(px);
				def0[2]+=tmp*Grid->GetDZNP(px);
				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
		jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
	}

	//-------------------------
	//	POut[0]=def0[0]*xScale+x;		POut[1]=def0[1]*yScale+y;		POut[2]=def0[2]*zScale+z;
	POut[0]=(def0[0]+gridPos[0])*xScale;		POut[1]=(def0[1]+gridPos[1])*yScale;		POut[2]=(def0[2]+gridPos[2])*zScale;	

	/*double in[3];
	in[0]=(double)x;
	in[1]=(double)y;
	in[2]=(double)z;
	Deformation(in,POut);

	POut[0]+=in[0];
	POut[1]+=in[1];
	POut[2]+=in[2];*/

}
//---------------------------------------------
void BSplineGridTransformation::TransformPoint(int x,int y, int z,int *POut)
{

	/*	double xx=N1x*x;
	double yy=N1y*y;
	double zz=N1z*z;


	int IX=(int)xx;
	int IY=(int)yy;
	int IZ=(int)zz;

	xx-=(double)IX;
	yy-=(double)IY;
	zz-=(double)IZ;

	double def[3];
	Deformation(IX,IY,IZ,xx,yy,zz,def);

	POut[0]=x+(int)(def[0]*Nx);
	POut[1]=y+(int)(def[1]*Ny);
	POut[2]=z+(int)(def[2]*Nz);
	*/
	double in[3];
	in[0]=(double)x;
	in[1]=(double)y;
	in[2]=(double)z;
	double pout[3];
	Deformation(in,pout);

	POut[0]=in[0]+pout[0];
	POut[1]=in[1]+pout[1];
	POut[2]=in[2]+pout[2];

}
//------------------------------------
void BSplineGridTransformation::ScaleGrid(FloatGrid3D *_Grid)
{

	// generuje pole deformacji dla nowego gridu wg gridu orginalnego
	int i,j,k;

	double xx=(double)(Grid->GetNx()-1)/(_Grid->GetNx()-1);
	double yy=(double)(Grid->GetNy()-1)/(_Grid->GetNy()-1);
	double zz=(double)(Grid->GetNz()-1)/(_Grid->GetNz()-1);
	double pdef[3];

	int t1=Grid->GetNx();
	int t2=_Grid->GetNx();
	double tt;


	double xx1=1.0/xx;
	double yy1=1.0/yy;
	double zz1=1.0/zz;

	double x,y,z;
	for(k=0;k<_Grid->GetNz();k++)
		for(j=0;j<_Grid->GetNy();j++)
			for(i=0;i<_Grid->GetNx();i++)
			{
				x=(double)i*xx;
				y=(double)j*yy;
				z=(double)k*zz;
				DeformationOrig(x,y,z,pdef);
				pdef[0]*=xx1;pdef[1]*=yy1;pdef[2]*=zz1;
				_Grid->SetDx(i,j,k,pdef[0]);
				_Grid->SetDy(i,j,k,pdef[1]);
				_Grid->SetDz(i,j,k,pdef[2]);


			}
			//uzupelnie tym co juz bylo zz
			double e=1e-6;

			if ((fabs((double)(Grid->GetNx()-_Grid->GetNx()))<e)&&
				(fabs((double)(Grid->GetNy()-_Grid->GetNy()))<e)&&
				(fabs((double)(Grid->GetNz()-_Grid->GetNz()))<e))
				for(k=0;k<Grid->GetNz();k++)
					for(j=0;j<Grid->GetNy();j++)
						for(i=0;i<Grid->GetNx();i++)
						{
							x=Round((double)i*xx1);
							y=Round((double)j*yy1);
							z=Round((double)k*zz1);
							tt=Grid->GetDX(i,j,k);
							_Grid->SetDx(x,y,z,tt*xx1);
							tt=Grid->GetDY(i,j,k);
							_Grid->SetDy(x,y,z,tt*xx1);
							tt=Grid->GetDZ(i,j,k);
							_Grid->SetDz(x,y,z,tt*xx1);

						}
}

//------------------------------------
void BSplineGridTransformation::DeformGrid(FloatGrid3D *_Grid)
{
	//cos popieprzone
	// generuje pole deformacji dla nowego gridu wg gridu orginalnego
	int i,j,k;

	double xx=(double)(Grid->GetNx()-1)/(_Grid->GetNx()-1);
	double yy=(double)(Grid->GetNy()-1)/(_Grid->GetNy()-1);
	double zz=(double)(Grid->GetNz()-1)/(_Grid->GetNz()-1);
	double pdef[3];

	int t1=Grid->GetNx();
	int t2=_Grid->GetNx();
	double tt;


	double xx1=1.0/xx;
	double yy1=1.0/yy;
	double zz1=1.0/zz;

	FloatGrid3D GGrid=*_Grid;
	double x0,y0,z0;

	double x,y,z;
	for(k=0;k<_Grid->GetNz();k++)
		for(j=0;j<_Grid->GetNy();j++)
			for(i=0;i<_Grid->GetNx();i++)
			{
				if ((i>0)&&(j>0)&&(k>0))
				{
					x=x;
				}
				x0=GGrid.GetDX(i,j,k);
				y0=GGrid.GetDY(i,j,k);
				z0=GGrid.GetDZ(i,j,k);
				x=(double)(i+x0)*xx;
				y=(double)(j+y0)*yy;
				z=(double)(k+z0)*zz;
				DeformationOrig(x,y,z,pdef);

				pdef[0]*=xx1;pdef[1]*=yy1;pdef[2]*=zz1;
				pdef[0]+=x0;pdef[1]+=y0;pdef[2]+=z0;
				_Grid->SetDx(i,j,k,pdef[0]);
				_Grid->SetDy(i,j,k,pdef[1]);
				_Grid->SetDz(i,j,k,pdef[2]);
			}
}
//----------------------------------------------------
double BSplineGridTransformation::InvertGrid()
{
	int		iter=4;
	int		iterBig=4;
	double	spacing=1.0;
	double	errStop=1e-6;
	double	tol=1e-9;


	double value;
	double value2;
	double factor;
	double x0,y0,z0;
	double xx,yy,zz;

	int i,j,k,l,m,n;
	double x,y,z;
	double pdef[3];
	int nPos[3];

	FloatGrid3D *gridBack=new FloatGrid3D(Grid->GetNx(),Grid->GetNy(),Grid->GetNz());
	gridBack->Clear();
	BSplineGridTransformation *transBack=new BSplineGridTransformation(gridBack,Grid->GetNx(),Grid->GetNy(),Grid->GetNz(),0,0,0);

	// proponowana wersja deformacji - first guess
	for(k=0;k<Grid->GetNz();k++)
	{
		for(j=0;j<Grid->GetNy();j++)
		{
			for(i=0;i<Grid->GetNx();i++)
			{
				x=(double)i;
				y=(double)j;
				z=(double)k;
				DeformationOrig(x,y,z,pdef);

				nPos[0]=Round(x+pdef[0]);
				nPos[1]=Round(y+pdef[1]);
				nPos[2]=Round(z+pdef[2]);

				gridBack->SetDx(nPos[0],nPos[1],nPos[2],-pdef[0]);
				gridBack->SetDy(nPos[0],nPos[1],nPos[2],-pdef[1]);
				gridBack->SetDz(nPos[0],nPos[1],nPos[2],-pdef[2]);
			}
		}
	}

	//policze sredni blad kwadratowy 
	double fDef[3],bDef[3],bDefNew[3],fDefNew;
	double err, tmperr,tmp;
	int samePoint;
	double	step;
	double oStep;
	double startErr;
	double tmpB;
	double bDef0[3];
	bool changes;
	double bGridDef[3];
	bool badFirstGuess;
	int nrOfUpdates;

	for (int zz=0;zz<iterBig;spacing/=2.0,zz++)
	for(k=0;k<Grid->GetNz();k++)
	{
		for(j=0;j<Grid->GetNy();j++)
		{
			for(i=0;i<Grid->GetNx();i++)
			{

				//licze startowy blad dla danego pktu
				nrOfUpdates=0;
				badFirstGuess=false;
				transBack->DeformationOrig(i,j,k,bDef);

				// nie miesci sie w kuboidzie
				if ((!(bDef[0]+i<0))&&(bDef[0]+i<Grid->GetNx())&&
					(!(bDef[1]+j<0))&&(bDef[1]+j<Grid->GetNy())&&
					(!(bDef[2]+k<0))&&(bDef[2]+k<Grid->GetNz()))
				{
					// forward transformation 
					this->DeformationOrig(i+bDef[0],j+bDef[1],k+bDef[2],fDef);
					// error calculation
					tmp=0;
					for (l=0;l<3;l++)
					{
						tmp+=(fDef[l]+bDef[l])*(fDef[l]+bDef[l]);
						bDef0[l]=bDef[l];
					}
					err=sqrt(tmp);
					startErr=err;
				}
				else
				{
					// sytuacja w ktorej first guess nie dziala
					badFirstGuess=true;
					err=10000;
					startErr=err;
				}

				step=spacing;
				for (l=0;l<3;l++)
				{
					bGridDef[l]=gridBack->GetD(l,i,j,k);
					bDefNew[l]=bGridDef[l];
				}

				for(m=0;m<iter;m++)
				{	
					changes=false;
					/*for(n=0;n<3;n++)
					{
						for (oStep=-step;oStep<2*step;oStep+=2*step)
						{	
							//ustalam nowa pozycje

							//chwilowa modyfikacja siatki gridu
							bGridDef[n]+=oStep;
							gridBack->SetD(n,i,j,k,bGridDef[n]);
							//pozycja punktu po transformacji odw 
							transBack->DeformationOrig(i,j,k,bDef);

							//obliczam pozycje punktu po zastosowaniu trans
							// warunek czy punkt trafil w grid
							if ((!(bDef[0]+i<0))&&(bDef[0]+i<Grid->GetNx())&&
								(!(bDef[1]+j<0))&&(bDef[1]+j<Grid->GetNy())&&
								(!(bDef[2]+k<0))&&(bDef[2]+k<Grid->GetNz()))
							{
								this->DeformationOrig(i+bDef[0],j+bDef[1],k+bDef[2],fDef);
								tmp=0;
								for (l=0;l<3;l++)
									tmp+=(fDef[l]+bDef[l])*(fDef[l]+bDef[l]);
								tmperr=sqrt(tmp);
								//sprawdzam czy lepiej jak tak to zapamietuje
								if (err-tmperr>tol)
								{
									err=tmperr;
									for (l=0;l<3;l++)
										bDefNew[l]=bGridDef[l];
									changes=true;
									nrOfUpdates++;

								}
							}

							//bDef[n]=tmpB;//wracam z powrotem
							bGridDef[n]-=oStep;
							gridBack->SetD(n,i,j,k,bGridDef[n]);
						}

						
					}
					*/

					//----------------------------------------
					for (double kStep=-step;kStep<2*step;kStep+=2*step)
						for (double jStep=-step;jStep<2*step;jStep+=2*step)
							for (double iStep=-step;iStep<2*step;iStep+=2*step)
							{

									//chwilowa modyfikacja siatki gridu
									bGridDef[0]+=iStep;		bGridDef[1]+=jStep;		bGridDef[2]+=kStep;
									gridBack->SetDx(i,j,k,bGridDef[0]);		gridBack->SetDy(i,j,k,bGridDef[1]);		gridBack->SetDz(i,j,k,bGridDef[2]);

									//pozycja punktu po transformacji odw 
									transBack->DeformationOrig(i,j,k,bDef);

									//obliczam pozycje punktu po zastosowaniu trans
									// warunek czy punkt trafil w grid
									if ((!(bDef[0]+i<0))&&(bDef[0]+i<Grid->GetNx())&&
										(!(bDef[1]+j<0))&&(bDef[1]+j<Grid->GetNy())&&
										(!(bDef[2]+k<0))&&(bDef[2]+k<Grid->GetNz()))
									{
										this->DeformationOrig(i+bDef[0],j+bDef[1],k+bDef[2],fDef);
										tmp=0;
										for (l=0;l<3;l++)
											tmp+=(fDef[l]+bDef[l])*(fDef[l]+bDef[l]);
										tmperr=sqrt(tmp);
										//sprawdzam czy lepiej jak tak to zapamietuje
										if (err-tmperr>tol)
										{
											err=tmperr;
											for (l=0;l<3;l++)
												bDefNew[l]=bGridDef[l];
											changes=true;
											nrOfUpdates++;

										}
									}

									//bDef[n]=tmpB;//wracam z powrotem
									bGridDef[0]-=iStep;		bGridDef[1]-=jStep;		bGridDef[2]-=kStep;
									gridBack->SetDx(i,j,k,bGridDef[0]);		gridBack->SetDy(i,j,k,bGridDef[1]);		gridBack->SetDz(i,j,k,bGridDef[2]);


							}
					//----------------------------------------

					step/=2.0;
					if (changes==true)
						//if ((startErr-err)>tol)
						for (l=0;l<3;l++)
						{
							bGridDef[l]=bDefNew[l];
							gridBack->SetD(l,i,j,k,bGridDef[l]);		
						}

						if (err<errStop)
						{
							/*if (!(err<0))
							{
								//for (l=0;l<3;l++)
								//	gridBack->SetD(l,i,j,k,0);

							}
							else*/
								break;
						}
				}


				if (changes==true)
					for (l=0;l<3;l++)
					{
						gridBack->SetD(l,i,j,k,bGridDef[l]);
					}

					transBack->DeformationOrig(i,j,k,bDef);
					this->DeformationOrig(i+bDef[0],j+bDef[1],k+bDef[2],fDef);
					tmp=0;
					for (l=0;l<3;l++)
						tmp+=(fDef[l]+bDef[l])*(fDef[l]+bDef[l]);
					err=sqrt(tmp);

			}
		}
	}

	


	double sum=0.0;
	int index=0;
	for(k=0;k<Grid->GetNz();k++)
		for(j=0;j<Grid->GetNy();j++)
			for(i=0;i<Grid->GetNx();i++)
			{
				transBack->DeformationOrig(i,j,k,bDef);
				if ((!(bDef[0]+i<0))&&(bDef[0]+i<Grid->GetNx())&&
					(!(bDef[1]+j<0))&&(bDef[1]+j<Grid->GetNy())&&
					(!(bDef[2]+k<0))&&(bDef[2]+k<Grid->GetNz()))
				{
					this->DeformationOrig(i+bDef[0],j+bDef[1],k+bDef[2],fDef);
					for (l=0;l<3;l++)
					{
						sum+=(fDef[l]+bDef[l])*(fDef[l]+bDef[l]);
						index++;
					}
				}

			}

			
			sum=(index>0)?sum/index:0;



			Grid->Copy(*gridBack);
			delete transBack;
			delete gridBack;

			return sum;
}





//-------------------------------------------------
double BSplineGridTransformation::GetDerivAtPoint(double xx,double yy,double zz, int px,int py,int pz)
{
	//xx,yy,zz - pkt we wspolrzednych gridu
	//px,py,pz - nod gridu po ktorym liczyc mam pochodne
	//deriv    - tu trzymam wynik
	int l=(int)(xx);		int m=(int)(yy);		int n=(int)(zz);
	double u= xx-(int)xx;	double v= yy-(int)yy;	double w= zz-(int)zz;
	int uu=(int)(u*1000);	int vv=(int)(v*1000);	int ww=(int)(w*1000);
	int lx=px-l;			int my=py-m;			int nz=pz-n;
	int lxx=lx*1000;		int myy=my*1000;		int nzz=nz*1000;

	double deriv=0;

	if (( lx>=0 && lx<4 )&&( my>=0 && my<4 )&&( nz>=0 && nz<4 ))
		deriv= SplineTab[lxx+uu]*SplineTab[myy+vv]*SplineTab[nzz+ww];

	return deriv;

}
//----------------------------
double BSplineGridTransformation::GetDerivAtPoint(int x,int y,int z, int px,int py,int pz)
{
	double xx,yy,zz;	
	xx=(x-minx)*N1x;	yy=(y-miny)*N1y;	zz=(z-minz)*N1z;

	return GetDerivAtPoint(xx,yy,zz, px, py, pz);
	//	deriv[0]*=xScale;	deriv[1]*=yScale;	deriv[2]*=zScale;
}
//---------------------------
void BSplineGridTransformation::TransformPont(int i,int j,int k,double *Point,double x,double y,double z,int NodX,int NodY,int NodZ)
{

	double in[3];
	double nod[3];
	in[0]=(double)i;
	in[1]=(double)j;
	in[2]=(double)k;
	double P[3];
	double def0[3];

	// na chwile movnac noda do nowej pozycji
	int pos=Grid->GetPos(NodX,NodY,NodZ);
	nod[0]=Grid->GetDXNP(pos);	nod[1]=Grid->GetDYNP(pos);	nod[2]=Grid->GetDZNP(pos);
	Grid->SetDx(pos,nod[0]+x);	Grid->SetDy(pos,nod[1]+y);	Grid->SetDz(pos,nod[2]+z);
	//------------------

	P[0]=in[0]*N1x;	P[1]=in[1]*N1y;	P[2]=in[2]*N1z;
	DeformationOrig(P[0],P[1],P[2],def0);

	/*
	PDef[0]=def0[0]*xScale;
	PDef[1]=def0[1]*yScale;
	PDef[2]=def0[2]*zScale;

	//---------------
	Deformation(in,Point);
	*/



	Grid->SetDx(pos,nod[0]);	Grid->SetDy(pos,nod[1]);	Grid->SetDz(pos,nod[2]);
	// przywrocic noda do pozycji pierwotnej
	//	Point[0]+=in[0];
	//	Point[1]+=in[1];
	//	Point[2]+=in[2];
	Point[0]=(def0[0]+P[0])*xScale;		
	Point[1]=(def0[1]+P[1])*yScale;		
	Point[2]=(def0[2]+P[2])*zScale;	

}
//--------------------------------------------------------------------
//- to ma byc najbardziej kompletna i w miare umiejetnosci 
//- zoptymalozwanan transformacja wraz z pochodnymi dla rejestracji
//--------------------------------------------------------------------
void BSplineGridTransformation::TransformPoint(	int *dataPos,double *def,bool calcDerivs,int *nodPos,
											   double *ddDdP,double *ddDdPL, double *ddDdQ,
											   double *ddDPdQ)//,double *ddDydQ,double *ddDzdQ)
{

	int		i,j,k;
	int		I,J,K;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[3];
	double	def0[3];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	gridPos[0]=(dataPos[0])*N1x;	gridPos[1]=(dataPos[1])*N1y;	gridPos[2]=(dataPos[2])*N1z;
	def0[0]=0;						def0[1]=0;						def0[2]=0;
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		int n=(int)(gridPos[2]);
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			double w=gridPos[2]-n;
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			int ww=(int)(w*1000);
	int it=uu;						int jt=vv;						int kt=ww;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	maxZ=(n+3<GridNz2)?4:GridNz2-n;

	int II=l;	int JJ=m;	
	K=n;	pz=K*Nxy2;	int JJJ=JJ*Nx2;

	if (!calcDerivs)
	{
		//licze tylko deformacje
		for (k=0;k<maxZ;k++)
		{
			sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
			for(j=0;j<maxY;j++)
			{
				sj=sk*SplineTab[jt];	I=II;	px=py+I;
				for(i=0;i<maxX;i++)
				{
					tmp=sj*SplineTab[it];
					def0[0]+=tmp*Grid->GetDXNP(px);
					def0[1]+=tmp*Grid->GetDYNP(px);
					def0[2]+=tmp*Grid->GetDZNP(px);
					it+=1000;	I++;	px++;
				}
				it=uu;	jt+=1000;	J++;	py+=Nx2;
			}
			jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
		}
	}
	else
	{
		double tol=1e-8;
		double tmp2;
		double sq;
		double gpx,gpy,gpz;
		ddDdP[0]=0;ddDdP[1]=0;ddDdP[2]=0;
		ddDdP[3]=0;ddDdP[4]=0;ddDdP[5]=0;
		ddDdP[6]=0;ddDdP[7]=0;ddDdP[8]=0;
		//licze wszystko do pochodnych takze
		double ux=N1x;//N1x;//(double)(GridNx-1);//N1x=(double)(GridNx-1)/Nx;
		double uy=N1y;//N1y;//(double)(GridNy-1);
		double uz=N1z;//N1z;//(double)(GridNz-1);
		//double tmpfact=1.0;
		for (k=0;k<maxZ;k++)
		{
			sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
			for(j=0;j<maxY;j++)
			{
				sj=sk*SplineTab[jt];	I=II;	px=py+I;
				for(i=0;i<maxX;i++)
				{
					gpx=Grid->GetDXNP(px);		gpy=Grid->GetDYNP(px);		gpz=Grid->GetDZNP(px);
					tmp=sj*SplineTab[it];
					def0[0]+=tmp*gpx;			def0[1]+=tmp*gpy;			def0[2]+=tmp*gpz;

					//test
					gpx*=xScale;gpy*=yScale;gpz*=zScale;
					//ddDdP_X
					tmp=SplineDerivTab[it]*SplineTab[jt]*SplineTab[kt]*ux;
					ddDdP[0]+=tmp*gpx;			ddDdP[1]+=tmp*gpy;			ddDdP[2]+=tmp*gpz;
					//ddDdP_Y
					tmp=SplineTab[it]*SplineDerivTab[jt]*SplineTab[kt]*uy;
					ddDdP[3]+=tmp*gpx;			ddDdP[4]+=tmp*gpy;			ddDdP[5]+=tmp*gpz;
					//ddDdP_Z
					tmp=SplineTab[it]*SplineTab[jt]*SplineDerivTab[kt]*uz;
					ddDdP[6]+=tmp*gpx;			ddDdP[7]+=tmp*gpy;			ddDdP[8]+=tmp*gpz;
					//----------------------

					it+=1000;	I++;	px++;
				}
				it=uu;	jt+=1000;	J++;	py+=Nx2;
			}
			jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
		}

		if (ddDdPL!=NULL)
		{
			// dlugosc wektorow ddDdPPL
			//	ddDdPL[0]=sqrt(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]+ddDdP[2]*ddDdP[2]);
			//	ddDdPL[1]=sqrt(ddDdP[3]*ddDdP[3]+ddDdP[4]*ddDdP[4]+ddDdP[5]*ddDdP[5]);
			//	ddDdPL[2]=sqrt(ddDdP[6]*ddDdP[6]+ddDdP[7]*ddDdP[7]+ddDdP[8]*ddDdP[8]);

			// dlugosc wektorow ddDdPPL ^2
			ddDdPL[0]=(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]+ddDdP[2]*ddDdP[2]);
			ddDdPL[1]=(ddDdP[3]*ddDdP[3]+ddDdP[4]*ddDdP[4]+ddDdP[5]*ddDdP[5]);
			ddDdPL[2]=(ddDdP[6]*ddDdP[6]+ddDdP[7]*ddDdP[7]+ddDdP[8]*ddDdP[8]);
		}

		//double *ddDdP,		- done
		//double ddDdQ,		- done
		//double *ddDPdQ,	- done
		//--------------------------------
		//		ddDddQ
		//--------------------------------
		int lx=nodPos[0]-l;		int my=nodPos[1]-m;		int nz=nodPos[2]-n;
		*ddDdQ=0;
		ddDPdQ[0]=0;				ddDPdQ[1]=0;			ddDPdQ[2]=0;
		if (( lx>=0 && lx<4 )&&( my>=0 && my<4 )&&( nz>=0 && nz<4 ))
		{

			int sx=lx*1000+uu;			int sy=my*1000+vv;			int sz=nz*1000+ww;
			*ddDdQ= SplineTab[sx]*SplineTab[sy]*SplineTab[sz];

			if (ddDPdQ!=NULL)
			{
				//ddDPdQ[0]=0;				ddDPdQ[1]=0;			ddDPdQ[2]=0;
				ddDPdQ[0]=SplineDerivTab[sx]*SplineTab[sy]*SplineTab[sz]*ux;
				ddDPdQ[1]=SplineTab[sx]*SplineDerivTab[sy]*SplineTab[sz]*uy;
				ddDPdQ[2]=SplineTab[sx]*SplineTab[sy]*SplineDerivTab[sz]*uz;
			}
		}
		//---------------------------------
	}


	//-------------------------
	//	def[0]=def0[0]*xScale+dataPos[0];		def[1]=def0[1]*yScale+dataPos[1];		def[2]=def0[2]*zScale+dataPos[2];
	def[0]=(def0[0]+gridPos[0])*xScale;		def[1]=(def0[1]+gridPos[1])*yScale;		def[2]=(def0[2]+gridPos[2])*zScale;	

}
//-----------------------------
void BSplineGridTransformation::TransformPoint(	int *nodPos,int *dataPos,double *def, double *ddDdQ)
{

	int		i,j,k;
	int		I,J,K;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[3];
	double	def0[3];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	gridPos[0]=(dataPos[0])*N1x;	gridPos[1]=(dataPos[1])*N1y;	gridPos[2]=(dataPos[2])*N1z;
	def0[0]=0;						def0[1]=0;						def0[2]=0;
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		int n=(int)(gridPos[2]);
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			double w=gridPos[2]-n;
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			int ww=(int)(w*1000);
	int it=uu;						int jt=vv;						int kt=ww;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	maxZ=(n+3<GridNz2)?4:GridNz2-n;

	int II=l;	int JJ=m;	
	K=n;	pz=K*Nxy2;	int JJJ=JJ*Nx2;


	//licze tylko deformacje
	for (k=0;k<maxZ;k++)
	{
		sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
		for(j=0;j<maxY;j++)
		{
			sj=sk*SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				tmp=sj*SplineTab[it];
				def0[0]+=tmp*Grid->GetDXNP(px);
				def0[1]+=tmp*Grid->GetDYNP(px);
				def0[2]+=tmp*Grid->GetDZNP(px);
				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
		jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
	}


	//--------------------------------
	//		ddDddQ
	//--------------------------------
	int lx=nodPos[0]-l;		int my=nodPos[1]-m;		int nz=nodPos[2]-n;
	*ddDdQ=0;
	if (( lx>=0 && lx<4 )&&( my>=0 && my<4 )&&( nz>=0 && nz<4 ))
	{

		int sx=lx*1000+uu;			int sy=my*1000+vv;			int sz=nz*1000+ww;
		*ddDdQ= SplineTab[sx]*SplineTab[sy]*SplineTab[sz];

	}


	def[0]=(def0[0]+gridPos[0])*xScale;		def[1]=(def0[1]+gridPos[1])*yScale;		def[2]=(def0[2]+gridPos[2])*zScale;	

}
//-----------------------------------------------------
void BSplineGridTransformation::TransformPoint(	int *dataPos,double *def,int *nodPos,double *ddDdPL)
{

	int		i,j,k;
	int		I,J,K;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[3];
	double	def0[3];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	gridPos[0]=(dataPos[0])*N1x;	gridPos[1]=(dataPos[1])*N1y;	gridPos[2]=(dataPos[2])*N1z;
	def0[0]=0;						def0[1]=0;						def0[2]=0;
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		int n=(int)(gridPos[2]);
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			double w=gridPos[2]-n;
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			int ww=(int)(w*1000);
	int it=uu;						int jt=vv;						int kt=ww;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	maxZ=(n+3<GridNz2)?4:GridNz2-n;

	int II=l;	int JJ=m;	
	K=n;	pz=K*Nxy2;	int JJJ=JJ*Nx2;



	double tol=1e-8;
	double tmp2;
	double sq;
	double gpx,gpy,gpz;
	double ddDdP[9];
	ddDdP[0]=0;ddDdP[1]=0;ddDdP[2]=0;
	ddDdP[3]=0;ddDdP[4]=0;ddDdP[5]=0;
	ddDdP[6]=0;ddDdP[7]=0;ddDdP[8]=0;
	//licze wszystko do pochodnych takze
	double ux=N1x;
	double uy=N1y;
	double uz=N1z;

	for (k=0;k<maxZ;k++)
	{
		sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
		for(j=0;j<maxY;j++)
		{
			sj=sk*SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				gpx=Grid->GetDXNP(px);		gpy=Grid->GetDYNP(px);		gpz=Grid->GetDZNP(px);
				tmp=sj*SplineTab[it];
				def0[0]+=tmp*gpx;			def0[1]+=tmp*gpy;			def0[2]+=tmp*gpz;

				//test
				gpx*=xScale;gpy*=yScale;gpz*=zScale;
				//ddDdP_X
				tmp=SplineDerivTab[it]*SplineTab[jt]*SplineTab[kt]*ux;
				ddDdP[0]+=tmp*gpx;			ddDdP[1]+=tmp*gpy;			ddDdP[2]+=tmp*gpz;
				//ddDdP_Y
				tmp=SplineTab[it]*SplineDerivTab[jt]*SplineTab[kt]*uy;
				ddDdP[3]+=tmp*gpx;			ddDdP[4]+=tmp*gpy;			ddDdP[5]+=tmp*gpz;
				//ddDdP_Z
				tmp=SplineTab[it]*SplineTab[jt]*SplineDerivTab[kt]*uz;
				ddDdP[6]+=tmp*gpx;			ddDdP[7]+=tmp*gpy;			ddDdP[8]+=tmp*gpz;
				//----------------------

				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
		jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
	}

	if (ddDdPL!=NULL)
	{
		// dlugosc wektorow ddDdPPL ^2
		ddDdPL[0]=(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]+ddDdP[2]*ddDdP[2]);
		ddDdPL[1]=(ddDdP[3]*ddDdP[3]+ddDdP[4]*ddDdP[4]+ddDdP[5]*ddDdP[5]);
		ddDdPL[2]=(ddDdP[6]*ddDdP[6]+ddDdP[7]*ddDdP[7]+ddDdP[8]*ddDdP[8]);
	}
	//-------------------------
	def[0]=(def0[0]+gridPos[0])*xScale;		def[1]=(def0[1]+gridPos[1])*yScale;		def[2]=(def0[2]+gridPos[2])*zScale;	

}

//------------
//-----------
//--------
//------------------------------------
void BSplineGridTransformation::ScaleGrid2D(FloatGrid3D *_Grid)
{

	// generuje pole deformacji dla nowego gridu wg gridu orginalnego
	int i,j;

	double xx=(double)(Grid->GetNx()-1)/(_Grid->GetNx()-1);
	double yy=(double)(Grid->GetNy()-1)/(_Grid->GetNy()-1);
	double pdef[2];

	int t1=Grid->GetNx();
	int t2=_Grid->GetNx();
	double tt;


	double xx1=1.0/xx;
	double yy1=1.0/yy;

	double x,y,z;
		for(j=0;j<_Grid->GetNy();j++)
			for(i=0;i<_Grid->GetNx();i++)
			{
				x=(double)i*xx;
				y=(double)j*yy;

				DeformationOrig(x,y,pdef);
				pdef[0]*=xx1;pdef[1]*=yy1;
				_Grid->SetDx(i,j,0,pdef[0]);
				_Grid->SetDy(i,j,0,pdef[1]);
				_Grid->SetDz(i,j,0,0);


			}
			//uzupelnie tym co juz bylo zz
			double e=1e-6;

			if ((fabs((double)(Grid->GetNx()-_Grid->GetNx()))<e)&&
				(fabs((double)(Grid->GetNy()-_Grid->GetNy()))<e))
				
					for(j=0;j<Grid->GetNy();j++)
						for(i=0;i<Grid->GetNx();i++)
						{
							x=Round((double)i*xx1);
							y=Round((double)j*yy1);
							
							tt=Grid->GetDX(i,j,0);
							_Grid->SetDx(x,y,0,tt*xx1);
							tt=Grid->GetDY(i,j,0);
							_Grid->SetDy(x,y,0,tt*xx1);
						

						}
}
//-----------------
void BSplineGridTransformation::TransformPoint2D(	int *dataPos,double *def,int *nodPos,double *ddDdPL)
{

	int		i,j;
	int		I,J;
	int		maxX,maxY;
	double	tmp;
	double	sk,sj;
	int		pos,py,px;
	double	gridPos[3];
	double	def0[3];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	gridPos[0]=(dataPos[0])*N1x;	gridPos[1]=(dataPos[1])*N1y;	
	def0[0]=0;						def0[1]=0;						
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			
	int it=uu;						int jt=vv;						
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	

	int II=l;	int JJ=m;	
	int JJJ=JJ*Nx2;



	double tol=1e-8;
	double tmp2;
	double sq;
	double gpx,gpy;
	double ddDdP[4];
	ddDdP[0]=0;ddDdP[1]=0;ddDdP[2]=0;ddDdP[3]=0;
	//licze wszystko do pochodnych takze
	double ux=N1x;
	double uy=N1y;
	
		int pz=Nxy2;
		J=JJ;py=JJJ+pz;
		for(j=0;j<maxY;j++)
		{

			sj=SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				gpx=Grid->GetDXNP(px);		gpy=Grid->GetDYNP(px);	
				tmp=sj*SplineTab[it];
				def0[0]+=tmp*gpx;			def0[1]+=tmp*gpy;			

				//test
				gpx*=xScale;gpy*=yScale;
				//ddDdP_X
				tmp=SplineDerivTab[it]*SplineTab[jt]*ux;
				ddDdP[0]+=tmp*gpx;			ddDdP[1]+=tmp*gpy;			
				//ddDdP_Y
				tmp=SplineTab[it]*SplineDerivTab[jt]*uy;
				ddDdP[2]+=tmp*gpx;			ddDdP[3]+=tmp*gpy;			
				//ddDdP_Z
				
				//----------------------

				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
	

	if (ddDdPL!=NULL)
	{
		// dlugosc wektorow ddDdPPL ^2
			ddDdPL[0]=(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]);
			ddDdPL[1]=(ddDdP[2]*ddDdP[2]+ddDdP[3]*ddDdP[3]);
	}
	//-------------------------
	def[0]=(def0[0]+gridPos[0])*xScale;		def[1]=(def0[1]+gridPos[1])*yScale;		

}
//-------------------------
void BSplineGridTransformation::TransformPoint2D(	int *dataPos,double *def,bool calcDerivs,int *nodPos,
											   double *ddDdP,double *ddDdPL, double *ddDdQ,
											   double *ddDPdQ)//,double *ddDydQ,double *ddDzdQ)
{

	int		i,j;
	int		I,J;
	int		maxX,maxY;
	double	tmp;
	double	sk,sj;
	int		pos,py,px;
	double	gridPos[2];
	double	def0[2];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	gridPos[0]=(dataPos[0])*N1x;	gridPos[1]=(dataPos[1])*N1y;	
	def0[0]=0;						def0[1]=0;						
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			
	int it=uu;						int jt=vv;						
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	

	int II=l;	int JJ=m;	
	int JJJ=JJ*Nx2;

	if (!calcDerivs)
	{
		//licze tylko deformacje
			J=JJ;py=JJ;
			for(j=0;j<maxY;j++)
			{
				sj=SplineTab[jt];	I=II;	px=py+I;
				for(i=0;i<maxX;i++)
				{
					tmp=sj*SplineTab[it];
					def0[0]+=tmp*Grid->GetDXNP(px);
					def0[1]+=tmp*Grid->GetDYNP(px);
					
					it+=1000;	I++;	px++;
				}
				it=uu;	jt+=1000;	J++;	py+=Nx2;
			}
		
	}
	else
	{
		double tol=1e-8;
		double tmp2;
		double sq;
		double gpx,gpy,gpz;
		ddDdP[0]=0;ddDdP[1]=0;ddDdP[2]=0;
		ddDdP[3]=0;ddDdP[4]=0;
		//licze wszystko do pochodnych takze
		double ux=N1x;//N1x;//(double)(GridNx-1);//N1x=(double)(GridNx-1)/Nx;
		double uy=N1y;//N1y;//(double)(GridNy-1);
		double uz=N1z;//N1z;//(double)(GridNz-1);
		//double tmpfact=1.0;
		//J=JJ;py=JJ;
			int pz=Nxy2;
	J=JJ;py=JJ+pz;
			for(j=0;j<maxY;j++)
			{
				sj=SplineTab[jt];	I=II;	px=py+I;
				for(i=0;i<maxX;i++)
				{
					gpx=Grid->GetDXNP(px);		gpy=Grid->GetDYNP(px);		
					tmp=sj*SplineTab[it];
					def0[0]+=tmp*gpx;			def0[1]+=tmp*gpy;			

					//test
					gpx*=xScale;gpy*=yScale;
					//ddDdP_X
					tmp=SplineDerivTab[it]*SplineTab[jt]*ux;
					ddDdP[0]+=tmp*gpx;			ddDdP[1]+=tmp*gpy;			
					//ddDdP_Y
					tmp=SplineTab[it]*SplineDerivTab[jt]*uy;
					ddDdP[2]+=tmp*gpx;			ddDdP[3]+=tmp*gpy;			
				
					//----------------------

					it+=1000;	I++;	px++;
				}
				it=uu;	jt+=1000;	J++;	py+=Nx2;
			}
	

		if (ddDdPL!=NULL)
		{
			// dlugosc wektorow ddDdPPL
			//	ddDdPL[0]=sqrt(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]+ddDdP[2]*ddDdP[2]);
			//	ddDdPL[1]=sqrt(ddDdP[3]*ddDdP[3]+ddDdP[4]*ddDdP[4]+ddDdP[5]*ddDdP[5]);
			//	ddDdPL[2]=sqrt(ddDdP[6]*ddDdP[6]+ddDdP[7]*ddDdP[7]+ddDdP[8]*ddDdP[8]);

			// dlugosc wektorow ddDdPPL ^2
			ddDdPL[0]=(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]);
			ddDdPL[1]=(ddDdP[2]*ddDdP[2]+ddDdP[3]*ddDdP[3]);
		}

		//double *ddDdP,		- done
		//double ddDdQ,		- done
		//double *ddDPdQ,	- done
		//--------------------------------
		//		ddDddQ
		//--------------------------------
		int lx=nodPos[0]-l;		int my=nodPos[1]-m;		
		*ddDdQ=0;
		ddDPdQ[0]=0;				ddDPdQ[1]=0;			
		if (( lx>=0 && lx<4 )&&( my>=0 && my<4 ))
		{

			int sx=lx*1000+uu;			int sy=my*1000+vv;		
			*ddDdQ= SplineTab[sx]*SplineTab[sy];

			if (ddDPdQ!=NULL)
			{
				//ddDPdQ[0]=0;				ddDPdQ[1]=0;			ddDPdQ[2]=0;
				ddDPdQ[0]=SplineDerivTab[sx]*SplineTab[sy]*ux;
				ddDPdQ[1]=SplineTab[sx]*SplineDerivTab[sy]*uy;
				
			}
		}
		//---------------------------------
	}


	//-------------------------
	//	def[0]=def0[0]*xScale+dataPos[0];		def[1]=def0[1]*yScale+dataPos[1];		def[2]=def0[2]*zScale+dataPos[2];
	def[0]=(def0[0]+gridPos[0])*xScale;		def[1]=(def0[1]+gridPos[1])*yScale;	

}
void BSplineGridTransformation::TransformPont2D(int i,int j,double *Point,double x,double y,int NodX,int NodY,int NodZ)
{

	double in[2];
	double nod[2];
	in[0]=(double)i;
	in[1]=(double)j;
	
	double P[2];
	double def0[2];

	// na chwile movnac noda do nowej pozycji
	int pos=Grid->GetPos(NodX,NodY,0);
	nod[0]=Grid->GetDXNP(pos);	nod[1]=Grid->GetDYNP(pos);	
	Grid->SetDx(pos,nod[0]+x);	Grid->SetDy(pos,nod[1]+y);	
	//------------------

	P[0]=in[0]*N1x;	P[1]=in[1]*N1y;	
	DeformationOrig(P[0],P[1],def0);

	/*
	PDef[0]=def0[0]*xScale;
	PDef[1]=def0[1]*yScale;
	PDef[2]=def0[2]*zScale;

	//---------------
	Deformation(in,Point);
	*/



	Grid->SetDx(pos,nod[0]);	Grid->SetDy(pos,nod[1]);	Grid->SetDz(pos,0);
	// przywrocic noda do pozycji pierwotnej
	//	Point[0]+=in[0];
	//	Point[1]+=in[1];
	//	Point[2]+=in[2];
	Point[0]=(def0[0]+P[0])*xScale;		
	Point[1]=(def0[1]+P[1])*yScale;		
	Point[2]=0;	

}
//--------------------------------------
void BSplineGridTransformation::DeformationOrig(double xx,double yy,double *def0)
{
	int i,j;
	int I,J;
	int it,jt;
	double tmp;
	double sk,sj;
	int pos;
	int py,px;
	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();
	int		maxX,maxY;

	def0[0]=0;				def0[1]=0;			
	int l=(int)(xx);		int m=(int)(yy);		
	double u= xx-(int)xx;	double v= yy-(int)yy;	
	int uu=(int)(u*1000);	int vv=(int)(v*1000);	
	it=uu;	jt=vv;	;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;

	int II=l;
	int JJ=m;

	int JJJ=JJ*Nx2;
	int pz=Nxy2;
	J=JJ;py=JJJ+pz;

	double ppx,ppy;
	
	for(j=0;j<maxY;j++)
	{
		sj=SplineTab[jt];	I=II;	px=py+I;
		for(i=0;i<maxX;i++)
		{
			tmp=sj*SplineTab[it];
			if (px<0)
			{				
				px=px;
			}
			ppx=Grid->GetDXNP(px);
			ppy=Grid->GetDYNP(px);
			if ((fabs(ppx)>0) || (fabs(ppy)>0))
			{
				px=px;
			}
			def0[0]+=tmp*ppx;
			def0[1]+=tmp*ppy;
				
			it+=1000;	I++;	px++;
		}
		it=uu;	jt+=1000;	J++;	py+=Nx2;
	}
	

}

//---------------------------------
void BSplineGridTransformation::TransformPoint2D(int x,int y, double *POut)
{
	double tol=1e-8;
	int		i,j;
	int		I,J;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[2];
	double	def0[2];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	gridPos[0]=x*N1x;				gridPos[1]=y*N1y;				
	def0[0]=0;						def0[1]=0;						
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			
	int it=uu;						int jt=vv;						
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	

	int II=l;	int JJ=m;	
		int JJJ=JJ*Nx2;


		if ((x==60)&&(y==0))
		{
			x=x;

		}

	 pz=Nxy2;
	J=JJ;py=JJJ+pz;
	
		for(j=0;j<maxY;j++)
		{
			sj=SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				tmp=sj*SplineTab[it];
				def0[0]+=tmp*Grid->GetDXNP(px);
				def0[1]+=tmp*Grid->GetDYNP(px);
				
				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
	

	//-------------------------
	//	POut[0]=def0[0]*xScale+x;		POut[1]=def0[1]*yScale+y;		POut[2]=def0[2]*zScale+z;
	POut[0]=(def0[0]+gridPos[0])*xScale;		POut[1]=(def0[1]+gridPos[1])*yScale;	

	
	/*double in[3];
	in[0]=(double)x;
	in[1]=(double)y;
	in[2]=(double)z;
	Deformation(in,POut);

	POut[0]+=in[0];
	POut[1]+=in[1];
	POut[2]+=in[2];*/

}

void BSplineGridTransformation::TransformPoint2D(double x,double y, double *POut)
{
	double tol=1e-8;
	int		i,j;
	int		I,J;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[2];
	double	def0[2];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	gridPos[0]=x*N1x;				gridPos[1]=y*N1y;				
	def0[0]=0;						def0[1]=0;						
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			
	int it=uu;						int jt=vv;						
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	

	int II=l;	int JJ=m;	
		int JJJ=JJ*Nx2;


		if ((x==60)&&(y==0))
		{
			x=x;

		}

	 pz=Nxy2;
	J=JJ;py=JJJ+pz;
	
		for(j=0;j<maxY;j++)
		{
			sj=SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				tmp=sj*SplineTab[it];
				def0[0]+=tmp*Grid->GetDXNP(px);
				def0[1]+=tmp*Grid->GetDYNP(px);
				
				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
	

	//-------------------------
	//	POut[0]=def0[0]*xScale+x;		POut[1]=def0[1]*yScale+y;		POut[2]=def0[2]*zScale+z;
	POut[0]=(def0[0]+gridPos[0])*xScale;		POut[1]=(def0[1]+gridPos[1])*yScale;	

	
	/*double in[3];
	in[0]=(double)x;
	in[1]=(double)y;
	in[2]=(double)z;
	Deformation(in,POut);

	POut[0]+=in[0];
	POut[1]+=in[1];
	POut[2]+=in[2];*/

}

//------------------------------------------------------------------------------------------------------------------------------


void BSplineGridTransformation::TransformPoint(	double *PIn,double *def,bool calcDerivs,int *nodPos,
											   double *ddDdP,double *ddDdPL, double *ddDdQ,
											   double *ddDPdQ)//,double *ddDydQ,double *ddDzdQ)
{

	int		i,j,k;
	int		I,J,K;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[3];
	double	def0[3];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	//gridPos[0]=(dataPos[0])*N1x;	gridPos[1]=(dataPos[1])*N1y;	gridPos[2]=(dataPos[2])*N1z;
/// gridPos mam zamienic na xx,yy,zz
	gridPos[0]=(PIn[0]-minx)*N1x;//(PIn[0]-minx)*(GridNx-1)/Nx; 
	gridPos[1]=(PIn[1]-miny)*N1y;//(PIn[1]-miny)*(GridNy-1)/Ny;
	gridPos[2]=(PIn[2]-minz)*N1z;//(PIn[2]-minz)*(GridNz-1)/Nz;

	def0[0]=0;						def0[1]=0;						def0[2]=0;
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		int n=(int)(gridPos[2]);
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			double w=gridPos[2]-n;
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			int ww=(int)(w*1000);
	int it=uu;						int jt=vv;						int kt=ww;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	maxZ=(n+3<GridNz2)?4:GridNz2-n;

	int II=l;	int JJ=m;	
	K=n;	pz=K*Nxy2;	int JJJ=JJ*Nx2;

	if (!calcDerivs)
	{
		//licze tylko deformacje
		for (k=0;k<maxZ;k++)
		{
			sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
			for(j=0;j<maxY;j++)
			{
				sj=sk*SplineTab[jt];	I=II;	px=py+I;
				for(i=0;i<maxX;i++)
				{
					tmp=sj*SplineTab[it];
					def0[0]+=tmp*Grid->GetDXNP(px);
					def0[1]+=tmp*Grid->GetDYNP(px);
					def0[2]+=tmp*Grid->GetDZNP(px);
					it+=1000;	I++;	px++;
				}
				it=uu;	jt+=1000;	J++;	py+=Nx2;
			}
			jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
		}
	}
	else
	{
		double tol=1e-8;
		double tmp2;
		double sq;
		double gpx,gpy,gpz;
		ddDdP[0]=0;ddDdP[1]=0;ddDdP[2]=0;
		ddDdP[3]=0;ddDdP[4]=0;ddDdP[5]=0;
		ddDdP[6]=0;ddDdP[7]=0;ddDdP[8]=0;
		//licze wszystko do pochodnych takze
		double ux=N1x;//N1x;//(double)(GridNx-1);//N1x=(double)(GridNx-1)/Nx;
		double uy=N1y;//N1y;//(double)(GridNy-1);
		double uz=N1z;//N1z;//(double)(GridNz-1);
		//double tmpfact=1.0;
		for (k=0;k<maxZ;k++)
		{
			sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
			for(j=0;j<maxY;j++)
			{
				sj=sk*SplineTab[jt];	I=II;	px=py+I;
				for(i=0;i<maxX;i++)
				{
					gpx=Grid->GetDXNP(px);		gpy=Grid->GetDYNP(px);		gpz=Grid->GetDZNP(px);
					tmp=sj*SplineTab[it];
					def0[0]+=tmp*gpx;			def0[1]+=tmp*gpy;			def0[2]+=tmp*gpz;

					//test
					gpx*=xScale;gpy*=yScale;gpz*=zScale;
					//ddDdP_X
					tmp=SplineDerivTab[it]*SplineTab[jt]*SplineTab[kt]*ux;
					ddDdP[0]+=tmp*gpx;			ddDdP[1]+=tmp*gpy;			ddDdP[2]+=tmp*gpz;
					//ddDdP_Y
					tmp=SplineTab[it]*SplineDerivTab[jt]*SplineTab[kt]*uy;
					ddDdP[3]+=tmp*gpx;			ddDdP[4]+=tmp*gpy;			ddDdP[5]+=tmp*gpz;
					//ddDdP_Z
					tmp=SplineTab[it]*SplineTab[jt]*SplineDerivTab[kt]*uz;
					ddDdP[6]+=tmp*gpx;			ddDdP[7]+=tmp*gpy;			ddDdP[8]+=tmp*gpz;
					//----------------------

					it+=1000;	I++;	px++;
				}
				it=uu;	jt+=1000;	J++;	py+=Nx2;
			}
			jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
		}

		if (ddDdPL!=NULL)
		{
			// dlugosc wektorow ddDdPPL
			//	ddDdPL[0]=sqrt(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]+ddDdP[2]*ddDdP[2]);
			//	ddDdPL[1]=sqrt(ddDdP[3]*ddDdP[3]+ddDdP[4]*ddDdP[4]+ddDdP[5]*ddDdP[5]);
			//	ddDdPL[2]=sqrt(ddDdP[6]*ddDdP[6]+ddDdP[7]*ddDdP[7]+ddDdP[8]*ddDdP[8]);

			// dlugosc wektorow ddDdPPL ^2
			ddDdPL[0]=(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]+ddDdP[2]*ddDdP[2]);
			ddDdPL[1]=(ddDdP[3]*ddDdP[3]+ddDdP[4]*ddDdP[4]+ddDdP[5]*ddDdP[5]);
			ddDdPL[2]=(ddDdP[6]*ddDdP[6]+ddDdP[7]*ddDdP[7]+ddDdP[8]*ddDdP[8]);
		}

		//double *ddDdP,		- done
		//double ddDdQ,		- done
		//double *ddDPdQ,	- done
		//--------------------------------
		//		ddDddQ
		//--------------------------------
		int lx=nodPos[0]-l;		int my=nodPos[1]-m;		int nz=nodPos[2]-n;
		*ddDdQ=0;
		ddDPdQ[0]=0;				ddDPdQ[1]=0;			ddDPdQ[2]=0;
		if (( lx>=0 && lx<4 )&&( my>=0 && my<4 )&&( nz>=0 && nz<4 ))
		{

			int sx=lx*1000+uu;			int sy=my*1000+vv;			int sz=nz*1000+ww;
			*ddDdQ= SplineTab[sx]*SplineTab[sy]*SplineTab[sz];

			if (ddDPdQ!=NULL)
			{
				//ddDPdQ[0]=0;				ddDPdQ[1]=0;			ddDPdQ[2]=0;
				ddDPdQ[0]=SplineDerivTab[sx]*SplineTab[sy]*SplineTab[sz]*ux;
				ddDPdQ[1]=SplineTab[sx]*SplineDerivTab[sy]*SplineTab[sz]*uy;
				ddDPdQ[2]=SplineTab[sx]*SplineTab[sy]*SplineDerivTab[sz]*uz;
			}
		}
		//---------------------------------
	}


	//-------------------------
	//	def[0]=def0[0]*xScale+dataPos[0];		def[1]=def0[1]*yScale+dataPos[1];		def[2]=def0[2]*zScale+dataPos[2];
//	def[0]=(def0[0]+gridPos[0])*xScale;		def[1]=(def0[1]+gridPos[1])*yScale;		def[2]=(def0[2]+gridPos[2])*zScale;	
	def[0]=(def0[0])*xScale+PIn[0];		def[1]=(def0[1])*yScale+PIn[1];		def[2]=(def0[2])*zScale+PIn[2];	

}
//-----------------------------
void BSplineGridTransformation::TransformPoint(	int *nodPos,double *PIn,double *def, double *ddDdQ)
{

	int		i,j,k;
	int		I,J,K;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[3];
	double	def0[3];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	//gridPos[0]=(dataPos[0])*N1x;	gridPos[1]=(dataPos[1])*N1y;	gridPos[2]=(dataPos[2])*N1z;
	gridPos[0]=(PIn[0]-minx)*N1x;//(PIn[0]-minx)*(GridNx-1)/Nx; 
	gridPos[1]=(PIn[1]-miny)*N1y;//(PIn[1]-miny)*(GridNy-1)/Ny;
	gridPos[2]=(PIn[2]-minz)*N1z;//(PIn[2]-minz)*(GridNz-1)/Nz;

	def0[0]=0;						def0[1]=0;						def0[2]=0;
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		int n=(int)(gridPos[2]);
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			double w=gridPos[2]-n;
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			int ww=(int)(w*1000);
	int it=uu;						int jt=vv;						int kt=ww;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	maxZ=(n+3<GridNz2)?4:GridNz2-n;

	int II=l;	int JJ=m;	
	K=n;	pz=K*Nxy2;	int JJJ=JJ*Nx2;


	//licze tylko deformacje
	for (k=0;k<maxZ;k++)
	{
		sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
		for(j=0;j<maxY;j++)
		{
			sj=sk*SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				tmp=sj*SplineTab[it];
				def0[0]+=tmp*Grid->GetDXNP(px);
				def0[1]+=tmp*Grid->GetDYNP(px);
				def0[2]+=tmp*Grid->GetDZNP(px);
				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
		jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
	}


	//--------------------------------
	//		ddDddQ
	//--------------------------------
	int lx=nodPos[0]-l;		int my=nodPos[1]-m;		int nz=nodPos[2]-n;
	*ddDdQ=0;
	if (( lx>=0 && lx<4 )&&( my>=0 && my<4 )&&( nz>=0 && nz<4 ))
	{

		int sx=lx*1000+uu;			int sy=my*1000+vv;			int sz=nz*1000+ww;
		*ddDdQ= SplineTab[sx]*SplineTab[sy]*SplineTab[sz];

	}


//	def[0]=(def0[0]+gridPos[0])*xScale;		def[1]=(def0[1]+gridPos[1])*yScale;		def[2]=(def0[2]+gridPos[2])*zScale;	
			def[0]=(def0[0])*xScale+PIn[0];		def[1]=(def0[1])*yScale+PIn[1];		def[2]=(def0[2])*zScale+PIn[2];		

}
//-----------------------------------------------------
void BSplineGridTransformation::TransformPoint(double *PIn,double *def,int *nodPos,double *ddDdPL)
{

	int		i,j,k;
	int		I,J,K;
	int		maxX,maxY,maxZ;
	double	tmp;
	double	sk,sj;
	int		pos,pz,py,px;
	double	gridPos[3];
	double	def0[3];

	int Nxy2=Grid->GetNxy2();
	int Nx2=Grid->GetNx2();

	//gridPos[0]=(dataPos[0])*N1x;	gridPos[1]=(dataPos[1])*N1y;	gridPos[2]=(dataPos[2])*N1z;

	gridPos[0]=(PIn[0]-minx)*N1x;//(PIn[0]-minx)*(GridNx-1)/Nx; 
	gridPos[1]=(PIn[1]-miny)*N1y;//(PIn[1]-miny)*(GridNy-1)/Ny;
	gridPos[2]=(PIn[2]-minz)*N1z;//(PIn[2]-minz)*(GridNz-1)/Nz;

	def0[0]=0;						def0[1]=0;						def0[2]=0;
	int l=(int)(gridPos[0]);		int m=(int)(gridPos[1]);		int n=(int)(gridPos[2]);
	double u=gridPos[0]-l;			double v=gridPos[1]-m;			double w=gridPos[2]-n;
	int uu=(int)(u*1000);			int vv=(int)(v*1000);			int ww=(int)(w*1000);
	int it=uu;						int jt=vv;						int kt=ww;
	maxX=(l+3<GridNx2)?4:GridNx2-l;	maxY=(m+3<GridNy2)?4:GridNy2-m;	maxZ=(n+3<GridNz2)?4:GridNz2-n;

	int II=l;	int JJ=m;	
	K=n;	pz=K*Nxy2;	int JJJ=JJ*Nx2;



	double tol=1e-8;
	double tmp2;
	double sq;
	double gpx,gpy,gpz;
	double ddDdP[9];
	ddDdP[0]=0;ddDdP[1]=0;ddDdP[2]=0;
	ddDdP[3]=0;ddDdP[4]=0;ddDdP[5]=0;
	ddDdP[6]=0;ddDdP[7]=0;ddDdP[8]=0;
	//licze wszystko do pochodnych takze
	double ux=N1x;
	double uy=N1y;
	double uz=N1z;

	for (k=0;k<maxZ;k++)
	{
		sk=SplineTab[kt];	J=JJ;	py=pz+JJJ;
		for(j=0;j<maxY;j++)
		{
			sj=sk*SplineTab[jt];	I=II;	px=py+I;
			for(i=0;i<maxX;i++)
			{
				gpx=Grid->GetDXNP(px);		gpy=Grid->GetDYNP(px);		gpz=Grid->GetDZNP(px);
				tmp=sj*SplineTab[it];
				def0[0]+=tmp*gpx;			def0[1]+=tmp*gpy;			def0[2]+=tmp*gpz;

				//test
				gpx*=xScale;gpy*=yScale;gpz*=zScale;
				//ddDdP_X
				tmp=SplineDerivTab[it]*SplineTab[jt]*SplineTab[kt]*ux;
				ddDdP[0]+=tmp*gpx;			ddDdP[1]+=tmp*gpy;			ddDdP[2]+=tmp*gpz;
				//ddDdP_Y
				tmp=SplineTab[it]*SplineDerivTab[jt]*SplineTab[kt]*uy;
				ddDdP[3]+=tmp*gpx;			ddDdP[4]+=tmp*gpy;			ddDdP[5]+=tmp*gpz;
				//ddDdP_Z
				tmp=SplineTab[it]*SplineTab[jt]*SplineDerivTab[kt]*uz;
				ddDdP[6]+=tmp*gpx;			ddDdP[7]+=tmp*gpy;			ddDdP[8]+=tmp*gpz;
				//----------------------

				it+=1000;	I++;	px++;
			}
			it=uu;	jt+=1000;	J++;	py+=Nx2;
		}
		jt=vv;	kt+=1000;	K++;	pz+=Nxy2;
	}

	if (ddDdPL!=NULL)
	{
		// dlugosc wektorow ddDdPPL ^2
		ddDdPL[0]=(ddDdP[0]*ddDdP[0]+ddDdP[1]*ddDdP[1]+ddDdP[2]*ddDdP[2]);
		ddDdPL[1]=(ddDdP[3]*ddDdP[3]+ddDdP[4]*ddDdP[4]+ddDdP[5]*ddDdP[5]);
		ddDdPL[2]=(ddDdP[6]*ddDdP[6]+ddDdP[7]*ddDdP[7]+ddDdP[8]*ddDdP[8]);
	}
	//-------------------------
	//def[0]=(def0[0]+gridPos[0])*xScale;		def[1]=(def0[1]+gridPos[1])*yScale;		def[2]=(def0[2]+gridPos[2])*zScale;	
	def[0]=(def0[0])*xScale+PIn[0];		def[1]=(def0[1])*yScale+PIn[1];		def[2]=(def0[2])*zScale+PIn[2];	
}

//------------
//-----------
//--------

