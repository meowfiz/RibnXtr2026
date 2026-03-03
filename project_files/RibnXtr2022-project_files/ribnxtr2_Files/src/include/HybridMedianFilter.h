#ifndef	 HYBRID_MEDIAN_FILTER
#define	HYBRID_MEDIAN_FILTER


#include <qobject.h>
#include <qapplication.h>
//#include <vector>
#include <stdio.h>
//#include <stl.h>

#include	"OtherCommonFunctions.h"

class HybridMedianFilter : public QObject
{

 Q_OBJECT

  public:


  		inline int Position(int x,int y,int z)
		{
			return z*sxy+y*sx+x;
		};



  	HybridMedianFilter();
	
	//~HybridMedianFilter();
		
		
    void ApplyMedian(unsigned short *InputArray,unsigned short *OutputArray,int Nx,int Ny,int Nz,int _dim,double Ha,double Ka);
  	
    int aa,bb;
		int sx,sy,sz,sxy;
		int dim;
		unsigned short *Tab0,*Tab1,*Tab2,*Tab3;
		unsigned short *InputArray,*OutputArray;
	//	int M1,M2,M3;
		int Value;
		int Pos1,Pos2,Pos3;
		
		int MedianPlus(int i,int j,int k);
		int MedianCross(int i,int j,int k);
		int MedianNormal(int i,int j,int k);
		int	MedianPlusCenterWeighed(int i,int j,int k, int Cweight);
//		int Median(unsigned short *A,int n);


			
	signals:
	
	void SignalSendProgress(int Progress);
	
};	 	
			
#endif //   HYBRID_MEDIAN_FILTER
