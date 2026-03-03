#ifndef OTHER_COMMON_FUNCTIONS_VTK
#define OTHER_COMMON_FUNCTIONS_VTK
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkEdgeTable.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkMarchingCubes.h>
#include <vtkCharArray.h>
#include <vtkDecimatePro.h>
#include <vtkUnsignedShortArray.h>
#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkGlyph3D.h>
#include <vtkStructuredPoints.h>

#include "FilterDefs.h"
#include <qdebug.h>
#include <stdio.h>
#include <math.h>
#include <qdatetime.h> 
#include <qstring.h>
#include <qapplication.h>
#include <qsettings.h>
#include <qdebug.h>
#include <qpainter.h>
#include <qbytearray.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qimage.h>


#include "RawDataSet.h"
#include "ImageShapes.h"
#include "Transform.h"
#include "SegCriterion.h"
#include "LinkedList.h"
#include "SurfaceCollection.h"

#include <qtreewidget.h>
#include "OtherCommonFunctions.h"


void UpdateProperty(vtkProperty *prop, QString txt, QString &textureName, bool &repeat, QString &groupName, vtkPolyDataMapper *vMapper = NULL);
void WriteProperty(vtkProperty *prop, QString file, QString textureName = "", bool repeat = true, QString groupName = "", vtkPolyDataMapper *vMapper = NULL);
vtkPolyData *GenerateSmoothedSurface(unsigned short thresh, RawDataSet *data, unsigned char *mdata, bool decim = true, bool smooth = true);
void CreateStencilBufferMask(vtkPolyData *polys, RawDataSet *origData, int closingS, unsigned char *backgroundMask, unsigned char *finalMask, int addDilate, int kernelType = 0, SegCriterion *cr = NULL, int fillValue=1);
RawDataSet *CreateStencilBuffer(vtkPolyData *polys, RawDataSet *origData, int closingSize, unsigned char *mask0 = NULL, int addDilate = 0, int kernelType = 0);
double	GetAvgDistance(vtkPolyData *polys);
double	GetMedDistance(vtkPolyData *polys);
void ConstrainedPolyDataSmoothing(vtkPolyData *&polys, int *smoothTab, int nrOfSmoothPts, int iter, double factor);
void		GetLinearInterpolatedPoint(vtkPoints *pts, double *p, int a, int b, double alpha);
double		GetDistance(vtkPoints *pts, int a, int b);
void		CleanPolyData(vtkPolyData *&polys);
int		CleanPolyDataDuplicatedLines(vtkPolyData*& polys);
void		ComputeNormalsPolyData(vtkPolyData *&Polys);
void		SmoothPolyData(vtkPolyData *&Polys, float factor, int iterNumber, float featureAngle = 60.0);
vtkPolyData	*DistanceRefinePolyDataFilter(vtkPolyData *pData, double dist, unsigned char *avoidList, int avoidTabLength, int maxIter = 10);
vtkPolyData	*DistanceRefinePolyDataFilter(vtkPolyData *pData, double dist);
vtkPolyData *IncreaseDensityInLinePolyData(vtkPolyData *pData, double minDist);
int		DistanceRefineTrianglePolyDataFilter(vtkPolyData *pData, double dist);
vtkPolyData *DoubleLinesInLinePolyData(vtkPolyData *pData);
double GetPolyLineLength(vtkPoints* pts);




#endif //OTHER_COMMON_FUNCTIONS_VTK

