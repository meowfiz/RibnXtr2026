#include	"HybridMedianFilter.h"


HybridMedianFilter::HybridMedianFilter()
{


}

void HybridMedianFilter:: ApplyMedian(unsigned short *_InputArray,unsigned short *_OutputArray,int Nx,int Ny,int Nz,int _dim,double Ha,double Ka)
{
	
	
	int i,j,k;
	
	sx=Nx;
	sy=Ny;
	sz=Nz;
	sxy=Nx*Ny; 
	
	dim=_dim;
	InputArray=_InputArray;
	OutputArray=_OutputArray;
	
	Tab0= new unsigned short  [(2*dim+1)*(2*dim+1)];
	
	
	Tab1	=	new	unsigned short[4*dim+5];
	Tab2	=	new	unsigned short[4*dim+5];
	aa= (2*dim+1)*(2*dim+1);
	bb=4*dim+1;
	
   Tab3 = new unsigned short[3];

   double  proc=(double)(100.0/(sx-dim-1) );

	QApplication::setOverrideCursor( Qt::WaitCursor );
	emit SignalSendProgress(0);

/*	QApplication::restoreOverrideCursor(  );
	QApplication::setOverrideCursor( waitCursor );
	*/
	QApplication::sendPostedEvents();
	 	
//	double H=1.0;
//	double K=0.2;
	double gg;
	int abc;
// 	FILE *Do_opa;
//	Do_opa=fopen("c:\\f.txt","a");

	for(i=dim;i<sx-dim;i++)
	{
	   for(j=dim;j<sy-dim;j++)
	   {
          for(k=0;k<sz;k++)
			{

		       Tab3[0]=MedianPlus(i,j,k);
		       Tab3[1]=MedianPlusCenterWeighed(i,j,k,3);
		       Tab3[2]=MedianCross(i,j,k);
		
				//Hybrid median
				gg=(double)(Tab3[0]-2*Tab3[1]+Tab3[2])/(double)(Ha+Ka*(Tab3[0]-Tab3[2])*(Tab3[0]-Tab3[2]));
				abc=Tab3[1]+Round(gg);
				if (abc<0)
					OutputArray[Position(i,j,k)]=0;
				else
					OutputArray[Position(i,j,k)]=abc;
				
			//	fprintf(Do_opa,"%lf \n",gg);
			
			//	 OutputArray[Position(i,j,k)]=Median(Tab3,3);	
			}
			emit SignalSendProgress((int)(proc*i));
		//	QApplication::setGlobalMouseTracking (true);
		/*	QApplication::restoreOverrideCursor(  );	
		QApplication::setOverrideCursor( waitCursor );
	*/	
			






	}
			
		QApplication::restoreOverrideCursor(  );

	}
//	fclose(Do_opa);


}
//---------------------------------------------
// + 
//---------------------------------------------
int HybridMedianFilter::MedianPlus(int i,int j,int k)
{

	int nr=0;
 	int x;
 	
   Tab1[nr]=InputArray[Position(i,j,k)];nr++;
    	
	for (x=1;x<=dim;x++)
	{
	    	Tab1[nr]=InputArray[Position(i+x,j,k)];nr++;
	    	Tab1[nr]=InputArray[Position(i-x,j,k)];nr++;
	    	
	    	Tab1[nr]=InputArray[Position(i,j+x,k)];nr++;
	    	Tab1[nr]=InputArray[Position(i,j-x,k)];nr++;	
	}

	 return Median(Tab1,bb);



}
//---------------------------------------------
// x
//---------------------------------------------
int  HybridMedianFilter::MedianCross(int i,int j,int k)
{
	int nr=0;
 	int x;
 	
   Tab1[nr]=InputArray[Position(i,j,k)];nr++;
    	
	for (x=1;x<=dim;x++)
	{
	
	  Tab1[nr]=InputArray[Position(i+x,j+x,k)];nr++;
	  Tab1[nr]=InputArray[Position(i-x,j-x,k)];nr++;
	  Tab1[nr]=InputArray[Position(i-x,j+x,k)];nr++;
	  Tab1[nr]=InputArray[Position(i+x,j-x,k)];nr++;
	}

   	 return Median(Tab1,bb);


}

//----------------------------------------
// median n-n
//----------------------------------------
int  HybridMedianFilter::MedianNormal(int i,int j,int k)
{
  int nr=0;
   int x,y;


	for (x=-dim;x<=dim;x++)
		for(y=-dim;y<=dim;y++)
	   			{
	   				Tab0[nr]=InputArray[Position(i+x,j+y,k)];nr++;
	   			}
	
     return Median(Tab0,aa);
}


//--------------------------------------
// center weighed + median
//---------------------------------------

int HybridMedianFilter::MedianPlusCenterWeighed(int i,int j,int k, int Cweight)
{

	int nr=0;
 	int x;
 	
	for (x=0;x<Cweight;x++)
	{
		Tab1[nr]=InputArray[Position(i,j,k)];nr++;
	}

    	
	for (x=1;x<=dim;x++)
	{
	    	Tab1[nr]=InputArray[Position(i+x,j,k)];nr++;
	    	Tab1[nr]=InputArray[Position(i-x,j,k)];nr++;
	    	
	    	Tab1[nr]=InputArray[Position(i,j+x,k)];nr++;
	    	Tab1[nr]=InputArray[Position(i,j-x,k)];nr++;	
	}

	 return Median(Tab1,bb+Cweight-1);



}


