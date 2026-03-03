#ifndef OTHER_IMAGE_FUNCTIONS
#define OTHER_IMAGE_FUNCTIONS
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
#include "DistanceMap.h"

QPolygonF	OpenSeparatedIslandsWithDistanceMap(unsigned short *tab, int ww, int hh, int inputVal, double *params, int *listS, int *listTmp, DistanceMap *distMap, QImage *img, bool test = false);
void  DivideBlobUsingDistanceMap(maskClass *inputBlob, QPolygonF *lines, int morphologyKernelType, int morphologyType, int *listS, int *listTmp, DistanceMap *distMap, bool test = false);


#endif //OTHER_IMAGE_FUNCTIONS

