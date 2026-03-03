#ifndef ANISOTROPIC_DIFFUSION_H
#define ANISOTROPIC_DIFFUSION_H
	
#include <stdio.h>
#include <memory.h>
#include <qapplication.h>


#include	"RawDataSet.h"
#include	"DataSet.h"
#include	"LinkedList.h"
#include	"OtherCommonFunctions.h"
//#include <ppl.h>
// using namespace Concurrency;
///////////////////////////////////////////////////////
class AnisotropicDiffusion :public QObject
{

	Q_OBJECT

	public:
		AnisotropicDiffusion(RawDataSet *InputData,LinkedList<DataSet*>	*_DataSetList=NULL);
		~AnisotropicDiffusion();

		void FilterDataSet2D(RawDataSet *FinalData,int iter,int cnr=0,double k0=5.0,double a=10.0,double dt=0.05){};
		void FilterDataSet3D(RawDataSet *FinalData,int iter,int cnr=0,double k0=5.0,double a=10.0,double dt=0.05);
		void FilterDataSet4D(RawDataSet *FinalData,int iter,int cnr=0,double k0=5.0,double a=10.0,double dt=0.05);

	private:

		//void ComputeGradient2D(int x, int y){};
		//void ComputeGradient3D(int x, int y, int z);
		//void ComputeGradient4D(int x, int y, int z, int t){};

		//double ComputeVectorLength(int *vectortab, int Dim);

		//original Perona & Malik
		double C1(double s, double K){return 1.0/(1.0+(s*s)/(K*K));}
		double C2(double s, double K){return exp(-(s*s)/(K*K));}
		//Turkey's biweight function
		double C3(double s, double K){if(s>K)return 0;double tmp=1-(s*s)/(K*K);return (0.5*(tmp*tmp));}		
		// Huber'smin-max norm
		double C4(double s, double K){return (fabs(s)>K)?1.0/s:1.0/K;}
		// Weickert
		double C5(double s, double K){if(s>0){double tmp=(K/s);return 1.0-exp(-3.315*tmp*tmp*tmp*tmp);}return 1.0;}
		
		double (AnisotropicDiffusion::*ConductivityFuncs[5])(double,double);


		RawDataSet *Data;
		LinkedList<DataSet*> *DataList;

	signals:

		void			SignalSendProgress(int);


};

#endif //ANISOTROPIC_DIFFUSION
//////////////////////////////////////////////////////
