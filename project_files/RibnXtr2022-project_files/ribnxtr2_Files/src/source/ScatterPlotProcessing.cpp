#include "ScatterPlotProcessing.h"

ScatterPlotProcessing::~ScatterPlotProcessing()
{
	if (tab!=NULL) delete []tab;
}
//----------------------------------------------
ScatterPlotProcessing::ScatterPlotProcessing(QImage *img,QWidget *parent,const char *name):QWidget(parent)
{

	 gaussParams=6;
	tolerance=1e-2;
	int pos;
	pos=0;
	sizeX=64;
	sizeY=64;


	origSizeX=img->width();
	origSizeY=img->height();

	double sx=(double)sizeX/img->width();
	double sy=(double)sizeY/img->height();

	tab=new int[sizeX*sizeY];
	memset(tab,0,sizeX*sizeY*sizeof(int));
	int i,j,k;

	unsigned char *dataX;
	//(dataT->GetMachineEndian()==1)?dataX=(unsigned char *)Image->bits()+1:dataX=(unsigned char *)Image->bits() ;
	dataX=(unsigned char *)img->bits() ;


	int xx,yy;
	int val;
	for (j=0;j<img->height();j++)
	{
		for(i=0;i<img->width();i++)
		{

			val=*(dataX++);
			dataX++;
			dataX++;
			dataX++;

			xx=Round(i*sx);
			yy=Round(j*sy);

			if ((xx<sizeX)&&(yy<sizeY))
			{
				tab[yy*sizeX+xx]+=val;
			}
		}
	}

	

	


}
//------------------------------------
int ScatterPlotProcessing::FindPeaks(int *tab2,int peakNr)
{

	if (tab!=NULL)	
	{

		int range=1;
		int peaks=peakNr;

		int pos;
		int i,j,k,l;
		int wsp;
		int nr=0;
		int val,val0;
		int dxl,dxr,dyl,dyr;

		int px,py;
		double *tmpTab=new double[sizeX*sizeY];
		for(j=0;j<sizeX;j++)
			for(i=0;i<sizeX;i++)
			{
				wsp=0;
				val0=tab[sizeY*j+i];
				dxl=dxr=dyl=dyr=0;
				//for(k=-range;k<=range;k++)
				for(l=-range;l<=range;l++)
				{
					//left
					px=i-l;
					py=j;
					if ((px>=0)&&(px<sizeX)&&(py>=0)&&(py<sizeY))
					{
						val=val0-tab[sizeY*py+px];
						if (val>dxl) dxl=val;
					}						
					//right
					px=i+l;
					py=j;
					if ((px>=0)&&(px<sizeX)&&(py>=0)&&(py<sizeY))
					{
						val=val0-tab[sizeY*py+px];
						if (val>dxr) dxr=val;
					}	
					//down
					px=i;
					py=j-1;
					if ((px>=0)&&(px<sizeX)&&(py>=0)&&(py<sizeY))
					{
						val=val0-tab[sizeY*py+px];
						if (val>dyl) dyl=val;
					}						
					//up
					px=i;
					py=j+1;
					if ((px>=0)&&(px<sizeX)&&(py>=0)&&(py<sizeY))
					{
						val=val0-tab[sizeY*py+px];
						if (val>dyr) dyr=val;
					}	

				}

				int nrr=0;
				if (dxr>0)	nrr++;
				if (dxl>0)	nrr++;
				if (dyl>0)	nrr++;
				if (dyr>0)	nrr++;

				//if ((dxr>0)&&(dxl>0)&&(dyl>0)&&(dyr>0))
				if (dxr>1)
				{
					tmpTab[sizeY*j+i]=dxr+dyr+dxl+dyl;
					nr++;
				}
				else 
					tmpTab[sizeY*j+i]=0;



			}

			int chg=0;
			do
			{

				chg=0;
				//find peak
				for(j=0;j<sizeY;j++)
				{
					for (i=0;i<sizeX;i++)
					{
						if (tmpTab[sizeX*j+i]>0)
							for(k=-1;k<=1;k++)
								for(l=-1;l<=1;l++)
								{
									px=i+k;
									py=j+l;
									if (!((k==0)&&(l==0)))
									{
										if ((px>=0)&&(px<sizeX)&&(py>=0)&&(py<sizeY))
										{
											if (tmpTab[sizeX*py+px]>0)
											{
												tmpTab[sizeX*py+px]=0;
												chg++;
												break;
											}
										}
									}
								}

					}
				}


			}while(chg>0);


			//losowych n
			int nrr=0;
			// sprawdz ile ich jest
			for(j=0;j<sizeX*sizeY;j++)
			{
				if (tmpTab[j]>0) nrr++;
			}

			// i losuj pokolei tyle co trzeba

			int los;

			//int *tab=new int[2*peaks];
			srand( (unsigned)time( NULL ) );
			//int chg;
			for(k=0;k<nrr;k++)
			{

				los=(int)((rand()/(double)RAND_MAX)*nrr);
				pos=0;
				chg=1;
				for (j=0;j<sizeY;j++)
				{
					for (i=0;i<sizeX;i++)
					{
						if (tmpTab[j*sizeX+i]>0) pos++;
						if (pos==los)
						{
							tmpTab[j*sizeX+i]=0;
							nrr--;

							tab2[(peakNr-peaks)*2]=i;
							tab2[(peakNr-peaks)*2+1]=j;

							peaks--;
							chg=0;
							break;
						}
						if (peaks==0) break;
						if (chg==0) break;
					}
					if (peaks==0) break;
					if (chg==0) break;
				}
				if (peaks==0) break;
			}

			if (peaks!=0)
			{
				// w razie czego cos trzeba z tym zrobic
				i=i;
			}
			//

			FILE *Do_opa;
			Do_opa=fopen("c:\\sct2.dat","w");

			pos=0;
			for (j=0;j<sizeY;j++)
			{
				fprintf(Do_opa,"\n# j = %lf \n",j);
				for(i=0;i<sizeX;i++)
				{
					fprintf(Do_opa,"%lf\n",tmpTab[pos++]);   
				}
			}
			fclose(Do_opa);

			delete []tmpTab;

			return peaks;

	}
	return 0;
}
//-------------------------------------
double ScatterPlotProcessing::FitGaussians(int *peaks,int peakNr,double *startA,double *endA)
{
	double f=0;
	int size=sizeX*sizeY;
	int Hsize= size+1;
	//int Lsize=peakNr*5+1;
	
	int Lsize=peakNr*gaussParams+1;
	nrOfParams=Lsize;
	int gaussNr=peakNr;
	int i,j,k;

	double tmpf;
	double max=0;
	double *fTab=new double[sizeX*sizeY];
	for(i=0;i<sizeX*sizeY;i++)
	{
		if (tab[i]>max) max=tab[i];
	}

	if (max>0)
		for(i=0;i<sizeX*sizeY;i++)
		{
			fTab[i]=(double)tab[i]/max;
		}

		if (iterX==0)
		{
			FILE *Do_opa;
			Do_opa=fopen("c:\\sct.dat","w");
	
			int pos=0;
			for (j=0;j<sizeY;j++)
			{
				fprintf(Do_opa,"\n# j = %d \n",j);
				for(i=0;i<sizeX;i++)
				{
					fprintf(Do_opa,"%lf\n",fTab[pos++]);   
				}
			}
			fclose(Do_opa);
			iterX=1;
		}



		double	*y=new double[Hsize];     
		double	*x=new double[Hsize];     
		double	*sig=new double[Hsize]; 
		int		*ia=new int[Lsize];
		double	*a=new double[Lsize];


		for (i=1;i<Hsize;i++)
		{
			sig[i]=1;		x[i]=i;		y[i]=fTab[i-1];
		}

		for (i=1;i<Lsize;i++)
			ia[i]=1;


		double sd=((double)(size-2)/(double)(gaussNr-1));
		int ii;
		for (i=0;i<gaussNr;i++)
		{
			ii=gaussParams*i;
			startA[ii+1]=a[ii+1]=peaks[i*2];		//px
			startA[ii+2]=a[ii+2]=peaks[i*2+1];		//py
			startA[ii+3]=a[ii+3]=10;				//sx
			startA[ii+4]=a[ii+4]=10;				//sy
			startA[ii+5]=a[ii+5]=sqrt(0.5+(rand()/(2*RAND_MAX))*(fTab[peaks[i*2+1]*sizeX+peaks[i*2]]));		//ampl
			if (gaussParams>5)startA[ii+6]=a[ii+6]=0.0;							//ro

		}


		//---------------------------------------

		OptimizationMarquardt *optim=new OptimizationMarquardt(x,y,sig,Hsize,a,ia,Lsize,this);


		FILE *czesc;
		czesc=fopen("c:\\mrq.txt","a");
		fprintf(czesc,"start %\n");
		fclose(czesc);

		for (i=1;i<Lsize;i++)
			ia[i]=0;

		for (i=0;i<gaussNr;i++)
		{
			ii=gaussParams*i;
			ia[ii+1]=1;		//amp
			ia[ii+2]=1;		//px
			ia[ii+3]=1;		//py
			ia[ii+4]=1;		//sx
			ia[ii+5]=1;		//sy
			if(gaussParams>5)	ia[ii+6]=1;		//ro
		}

		double tolx=10e-4;
		for(i=0;i<gaussNr;i++)
			optim->StartOptimization(tolx);

		/*tmpf=optim->chisq;
		//FILE *czesc;
		czesc=fopen("c:\\mrq.txt","a");
		fprintf(czesc,"dupa %\n");
		fclose(czesc);
	

		for (i=0;i<gaussNr;i++)
		{
			ia[5*i+5]=1;		//amp
			ia[5*i+1]=1;		//px
			ia[5*i+2]=1;		//py
			ia[5*i+3]=1;		//sx
			ia[5*i+4]=1;		//sy
		}
		for(i=0;i<gaussNr;i++)
			optim->StartOptimization(tolx);*/
		
		for (i=1;i<Lsize;i++)
			endA[i]=a[i];

		
		f=optim->chisq;
		//if(tmpf-f<tolx)
		//	f=INT_MAX;

		//-------------------------------------

		delete optim;
		delete []a;
		delete []ia;
		delete []sig;
		delete []x;
		delete []y;
		delete []fTab;
		//delete []tmpA;
		return f;

}
//--------------------------------------
void ScatterPlotProcessing::SmoothScatterPlot()
{



	if (tab!=NULL)	
	{

		int radius=2;
		double stddev=1.5;

		int					size2=2*radius+1;
		double				*gaussianKernelTab=new double [size2];
		unsigned short		*intensityTab=new unsigned short[size2];
		GetGaussianKernel(gaussianKernelTab,(double)radius,stddev);

		int i,j,k;
		int pos;
		//transversal 1d gauss

		RawDataSet *rData=new RawDataSet(sizeX,sizeY,1,1,1,1);
		RawDataSet *_data=new RawDataSet();
		_data->Copy(*rData,false);

		unsigned short *Q=rData->GetDataArray();
		for(i=0;i<rData->GetVolumeSize();i++)
			Q[i]=tab[i];

		//RawDataSet *tempData=&rData;
		//RawDataSet *tempData2;

		Q=_data->GetDataArray();
		pos=0;
		for(j=0;j<_data->GetNy();j++)
		{
			for(i=0;i<_data->GetNx();i++)
			{
				GetIntensityTab1D(rData,i,j,0,0,intensityTab,radius);
				Q[pos++]=Apply1DNormFilter(gaussianKernelTab,intensityTab,size2);		
			}
		}

		//tempData2=tempData;	tempData=_data;	_data=tempData2;	tempData2=NULL;

		Q=rData->GetDataArray();
		pos=0;
		{
			for(j=0;j<_data->GetNy();j++)
				for(i=0;i<_data->GetNx();i++)
				{
					GetIntensityTab1D(_data,i,j,0,1,intensityTab,radius);
					Q[pos++]=Apply1DNormFilter(gaussianKernelTab,intensityTab,size2);		
				}
		}



		Q=rData->GetDataArray();
		for(i=0;i<rData->GetVolumeSize();i++)
			tab[i]=Q[i];

		Q=NULL;

		delete _data;
		delete rData;
		delete []gaussianKernelTab;
		delete []intensityTab;

	}
}
//----------------------------------------
//----------------------------------------------------------------
double ScatterPlotProcessing::GetFunctionValue(double pos,double *a)
{
	int i;

	double fac,ex,arg;
	double f=0.0;
	double px,py,sx,sy,amp,r;

	int p=Round(pos-1);

	tHistPosX=(p%sizeX);
	tHistPosY=(p-tHistPosX)/sizeX;


	double x=tHistPosX;
	double y=tHistPosY;
	if (gaussParams==5)
	{
		for (i=1;i<nrOfParams;i+=gaussParams)
		{
			
			px=a[i];
			py=a[i+1];
			sx=a[i+2];
			sy=a[i+3];
			amp=a[i+4];

			if ((fabs(sx)>tolerance)&&(fabs(sy)>tolerance))
			{
				arg=((x-px)*(x-px)/(sx*sx)+(y-py)*(y-py)/(sy*sy));
				f+=amp*amp*exp(-arg);
			}
			else
			{
				f=0;
				break;	
			}

		}
	}
	else
	{
		double aa,bb;
		double sinr,cosr;
		double xmpx,ympy;
		for (i=1;i<nrOfParams;i+=gaussParams)
		{
			px=a[i];
			py=a[i+1];
			sx=a[i+2];
			sy=a[i+3];
			amp=a[i+4];
			r=a[i+5];

	
			if ((fabs(sx)>tolerance)&&(fabs(sy)>tolerance))
			{
				xmpx=x-px;ympy=y-py;
				sinr=sin(r);cosr=cos(r);
				aa= (cosr*xmpx + sinr*ympy)/sx;		aa*=aa;
				bb=(-sinr*xmpx + cosr*ympy)/sy;		bb*=bb;
				arg=aa+bb;
				//arg=((x-px)*(x-px)/(sx*sx)+(y-py)*(y-py)/(sy*sy));
				f+=amp*amp*exp(-arg);

			}
			else
			{
				f=0;
				break;	
			}
		}
	}

	
//FILE *czesc;czesc=fopen("c:\\mrq.txt","a");fprintf(czesc,"%d %lf\n",(int)pos,f);fclose(czesc);

	return (double)f;


}
//----------------------------------
double ScatterPlotProcessing::GetSingleGaussianValue(double pos, double *a,int nr)
{
	int i;
	double fac,ex,arg;
	double f=0.0;
	double px,py,sx,sy,amp,r;

	int p=Round(pos-1);

	tHistPosX=(p%sizeX);
	tHistPosY=(p-tHistPosX)/sizeX;


	double x=tHistPosX;
	double y=tHistPosY;
	if (gaussParams==5)
	{
		for (i=5*nr+1;i<5*(nr+1)+1;i+=5)
		{
			px=a[i];
			py=a[i+1];
			sx=a[i+2];
			sy=a[i+3];
			amp=a[i+4];
			if ((fabs(sx)>tolerance)&&(fabs(sy)>tolerance))
			{
				arg=((x-px)*(x-px)/(sx*sx)+(y-py)*(y-py)/(sy*sy));
				f=amp*amp*exp(-arg);
			}
			else
			{
				f=0;
				break;	
			}

		}
	}
	else
	{
		double aa,bb;
		double sinr,cosr;
		double xmpx,ympy;

		for (i=6*nr+1;i<6*(nr+1)+1;i+=6)
		{
			px=a[i];
			py=a[i+1];
			sx=a[i+2];
			sy=a[i+3];
			amp=a[i+4];
			r=a[i+5];
			if ((fabs(sx)>tolerance)&&(fabs(sy)>tolerance))
			{
				xmpx=x-px;ympy=y-py;
				sinr=sin(r);cosr=cos(r);
				aa= (cosr*xmpx + sinr*ympy)/sx;		aa*=aa;
				bb=(-sinr*xmpx + cosr*ympy)/sy;		bb*=bb;
				arg=aa+bb;

				f=amp*amp*exp(-arg);
			}
			else
			{
				f=0;
				break;	
			}

		}
	}


	return (double)f;
}
//----------------------------------
void ScatterPlotProcessing::CalculateDerivative(double *dyda,double *a)
{

	int i;

	double fac,ex,arg;
	double f=0.0;
	double px,py,sx,sy,amp,r;
	double xx;
	double yy;
	double xy;
	double ror;
	double aaro;

	double x=tHistPosX;
	double y=tHistPosY;
	if (gaussParams==5)
	{
		for (i=1;i<nrOfParams;i+=gaussParams)
		{
			px=a[i];
			py=a[i+1];
			sx=a[i+2];
			sy=a[i+3];
			amp=a[i+4];

			if ((fabs(sx)>tolerance)&&(fabs(sy)>tolerance))
			{
				arg=((x-px)*(x-px)/(sx*sx)+(y-py)*(y-py)/(sy*sy));
				ex=exp(-arg);
				xx=2.0*(x-px)/(sx*sx);
				yy=2.0*(y-py)/(sy*sy);


				dyda[i]=amp*amp*ex*xx;				//px
				dyda[i+1]=amp*amp*ex*yy;			//py
				dyda[i+2]=dyda[i]*(x-px)/sx;	//sx
				dyda[i+3]=dyda[i+1]*(y-py)/sy;	//sy
				dyda[i+4]=2*amp*ex;					//amp
			}
			else
			{
				dyda[i]=0;						//px
				dyda[i+1]=0;					//py
				dyda[i+2]=0;					//sx
				dyda[i+3]=0;					//sy
				dyda[i+4]=0;					//amp
				dyda[i+5]=0;					//r
			}

		}
	}
	else
	{
		double aa,bb,a1,b1;
		double sinr,cosr;
		double xmpx,ympy;
		double aae;
		for (i=1;i<nrOfParams;i+=gaussParams)
		{
			px=a[i];
			py=a[i+1];
			sx=a[i+2];
			sy=a[i+3];
			amp=a[i+4];
			r=a[i+5];
			if ((fabs(sx)>tolerance)&&(fabs(sy)>tolerance))
			{

				xmpx=x-px;ympy=y-py;
				sinr=sin(r);cosr=cos(r);
				a1= cosr*xmpx + sinr*ympy;		aa=2*a1/(sx*sx);
				b1=-sinr*xmpx + cosr*ympy;		bb=2*b1/(sy*sy);
				arg=aa*a1/2+bb*b1/2;
				ex=exp(-arg);
				aae=amp*amp*ex;


				dyda[i]=aae*(aa*cosr+bb*sinr);								//px
				dyda[i+1]=aae*(aa*sinr+bb*cosr);							//py
				dyda[i+2]=aae*(aa*a1/sx);										//sx
				dyda[i+3]=aae*(bb*b1/sy);										//sy
				dyda[i+4]=2*amp*ex;												//amp
				dyda[i+5]=aae*(-aa*(-sinr*xmpx+cosr*ympy)-bb*(-cosr*xmpx-sinr*ympy));	//r
			}
			else
			{
				dyda[i]=0;						//px
				dyda[i+1]=0;					//py
				dyda[i+2]=0;					//sx
				dyda[i+3]=0;					//sy
				dyda[i+4]=0;					//amp
				dyda[i+5]=0;					//r
			}

		}
	}


	/*
	int i;
	double fac,ex,arg;

	double x=tHistPos;
	for (i=1;i<=nrOfParams-3;i+=3)
	{
	arg=(x-a[i+3])/a[i+4];
	ex=exp(-arg*arg);
	fac=a[i+2]*a[i+2]*ex*2.0*arg;
	dyda[i+2]=2*a[i+2]*ex;
	//fac=a[i+2]*ex*2.0*arg;
	//dyda[i+2]=ex;

	dyda[i+3]=fac/a[i+4];
	dyda[i+4]=fac*arg/a[i+4];
	}

	dyda[1]=x;
	dyda[2]=1.0;
	*/


}
//---------------------------------
void ScatterPlotProcessing::PrecalculateVariables(double *Params)
{

}
//----------------------------------
void	ScatterPlotProcessing::GenerateSegmentationMasks(double sigma, double *a,int nr,int scatX,int scatY)
{

	int i,j,k;
	QString txt;
	int pos;
	FILE *czesc;

	double scx,scy;
	origSizeX=scatX;
	origSizeY=scatY;
	scx=(double)sizeX/origSizeX;
	scy=(double)sizeY/origSizeY;
	int scatterSize=origSizeX*origSizeY;

	unsigned char *tab;
	double px,py,sx,sy,amp;
	double xx,yy;
	double ii,jj;

	unsigned char **masks=new unsigned char*[nr];
	int nrr;
	for (k=0;k<nr;k++)
	{

		px=a[k*5+1];
		py=a[k*5+2];
		sx=a[k*5+3]*0.5;
		sy=a[k*5+4]*0.5;
		amp=a[k*5+5];
		
		if (amp>0)
		{
			nrr=0;
			tab=new unsigned char[scatterSize];
			memset(tab,0,scatterSize*sizeof(unsigned char));
			
			for(j=0;j<origSizeY;j++)
			{
				jj=j*scy;
				yy=((jj-py)*(jj-py))/(sy*sy);
				for(i=0;i<origSizeX;i++)
				{
					ii=i*scx;
					xx=((ii-px)*(ii-px))/(sx*sx);
					if (xx+yy<1)
					{
						tab[origSizeX*j+i]=1;
						nrr++;
					}
				}
			}
			if (nrr>0)
			{			
				masks[k]=tab;
			}
			else
			{
				delete []tab;
				masks[k]=NULL;
			}


		}
		else
			masks[k]=NULL;


		/*

		txt="c:\\mask_"+QString::number(k)+".dat";	
		czesc=fopen(txt.toLocal8Bit(),"w");

		pos=0;
		for (j=0;j<sizeY;j++)
		{
			fprintf(czesc,"\n# j = %lf \n",j);
			for(i=0;i<sizeX;i++)
			{
				fprintf(czesc,"%d\n",tab[origSizeX*j+i]);   
			}
		}

		fclose(czesc);


	
		delete []tab;
		*/


	}

	//--------------------------------------------

	//bede rozsylal parki

	unsigned char *m;

	//for(i=0;i<nr;i++)
	for(j=0;j<nr;j++)
	{
		if (masks[j]!=NULL)
		{
			m=new unsigned char[scatterSize];
			for(i=0;i<scatterSize;i++)
				m[i]=masks[j][i];

			emit SignalSendSegmentationMask(m, origSizeX,origSizeY);
			//paskudztwo bo trzeba to kasowac na zewnatrz!!!!
		}
	}

	//--------------------------------------------

	for(i=0;i<nr;i++)
	{
		if (masks[i]!=NULL) delete []masks[i];
	}
	delete []masks;



	//emit SignalSendSegmentationMask(unsigned char *&, int sx,int sy)
}
//---------------------------------------------
void ScatterPlotProcessing::ProcessScatterPlot(int peaks,int iterNr,double sigma,int sxx,int syy)
{
	if (tab!=NULL)
	{
		int pos;
		int i,j,k;
		
		int *peakTab=new int[peaks*gaussParams];
		int Lsize=peaks*gaussParams+1;
		double *tmpStartA=new double[Lsize];
		double *tmpEndA=new double[Lsize];

		double *startA=new double[Lsize];
		double *endA=new double[Lsize];

		double tmpChisq=INT_MAX;
		double chisq=tmpChisq;
		//QString txt;
		iterX=0;
		for(i=0;i<iterNr;i++)
		{
			peaks-=FindPeaks(peakTab,peaks);
			tmpChisq=FitGaussians(peakTab,peaks,tmpStartA,tmpEndA);
			
			if (tmpChisq<chisq)
			{
				chisq=tmpChisq;
				for (j=1;j<Lsize;j++)
				{
					startA[j]=tmpStartA[j];
					endA[j]=tmpEndA[j];

				}
			}

			emit SendMessage("(" + QString::number(i) + "/"+QString::number(iterNr-1)+")"+"best="+QString::number(chisq));

		}

		//-------------------------******
		//--------------------------------------
		QString txt;
		//int pos;
		FILE *czesc;
	
		for (k=0;k<peaks;k++)
		{
			txt="c:\\gauss2D"+QString::number(k)+".dat";	
			czesc=fopen(txt.toLocal8Bit(),"w");

			pos=0;
			for (j=0;j<sizeY;j++)
			{
				fprintf(czesc,"\n# j = %lf \n",j);
				for(i=0;i<sizeX;i++)
				{
					fprintf(czesc,"%lf\n",GetSingleGaussianValue(pos++,endA,k));   
				}
			}

			fclose(czesc);


		}

		czesc=fopen("c:\\start2d.dat","w");
		pos=0;
		for (j=0;j<sizeY;j++)
		{
			fprintf(czesc,"\n# j = %lf \n",j);
			for(i=0;i<sizeX;i++)
			{
				fprintf(czesc,"%lf\n",GetFunctionValue(pos++,startA));   
			}
		}

		fclose(czesc);


		czesc=fopen("c:\\2DStartParams.dat","w");for(i=1;i<Lsize;i++)fprintf(czesc,"%d %lf\n",i,(startA[i]));fclose(czesc);

		czesc=fopen("c:\\end2d.dat","w");
		pos=0;
		for (j=0;j<sizeY;j++)
		{
			fprintf(czesc,"\n# j = %lf \n",j);
			for(i=0;i<sizeX;i++)
			{
				fprintf(czesc,"%lf\n",GetFunctionValue(pos++,endA));   
			}
		}

		fclose(czesc);

		czesc=fopen("c:\\2DEndParams.dat","w");fprintf(czesc,"\n");for(i=1;i<Lsize;i++)fprintf(czesc,"%d %lf\n",i,(endA[i]));fclose(czesc);

		//-------------------------******





		//GenerateSegmentationMasks(sigma, endA,peaks,sxx,syy);

		delete []tmpStartA;
		delete []tmpEndA;
		delete []startA;
		delete []endA;
		delete []peakTab;
	}
}