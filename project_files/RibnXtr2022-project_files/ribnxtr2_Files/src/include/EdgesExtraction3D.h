#ifndef EDGES_EXTRACTION_3D_H
#define EDGES_EXTRACTION_3D_H
	
#include <stdio.h>
#include <memory.h>
#include <qapplication.h>


#include	"RawDataSet.h"
#include	"DataSet.h"
#include	"LinkedList.h"
#include	"OtherCommonFunctions.h"
#include	"TransformBSpline.h"
#include	"Grid3D.h"
#include	"OptimizationPowell.h"

///////////////////////////////////////////////////////
class EdgesExtraction3D :public QObject
{

	Q_OBJECT

	public:
		EdgesExtraction3D(RawDataSet *_rawData);
		~EdgesExtraction3D();

		RawDataSet	*ApplyCanny3DEdgesExtraction(double radius,double stddev,int dim=3);


	private:

		RawDataSet *rawData;
		double	_EPSILON_DERIVATIVE_;
		double	_EPSILON_NORM_;

		void	GetGradient2DTab(double *output,unsigned short *input3D,double *kernel,unsigned short *tmpBuff,int size,int view,int nx,int ny,int nz,int zPos);
		void	Get3DModulus(double *output, double *inX,double *inY,double *inZ,int size);
	
		void	Remove_Gradient_NonMaxima_Slice_2D( double *maxima,	double *gx,double *gy,double *norme,int *bufferDims );
		void	Remove_Gradient_NonMaxima_Slice_3D( double *maxima,double *gx,double *gy,double *gz,double **norme,int *bufferDims );

signals:

		void	SignalSendProgress(int);
		void	SendMessage(const QString &Message);

};

#endif //EDGES_EXTRACTION_3D_H
//////////////////////////////////////////////////////
