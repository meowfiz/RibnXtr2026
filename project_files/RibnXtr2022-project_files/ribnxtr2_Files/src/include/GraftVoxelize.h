#ifndef GRAFT_VOXELIZE
#define GRAFT_VOXELIZE


#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<math.h>
#include 	<string.h>

#include	<vtkPoints.h>
#include	<vtkDataArray.h>
#include	<vtkClipPolyData.h>
#include 	<vtkPlaneSource.h>
#include	<vtkAppendPolyData.h>
#include	<vtkPolyDataConnectivityFilter.h>
#include	<vtkFeatureEdges.h>

#include	<vtkTriangleFilter.h>
#include	<vtkInterpolatingSubdivisionFilter.h> 
#include	<vtkRuledSurfaceFilter.h>
//#include	<vtkPolyDataToPolyDataFilter.h> 
#include	<vtkLinearSubdivisionFilter.h> 
#include	<vtkCleanPolyData.h> 
#include	<vtkDelaunay2D.h> 
#include	<vtkSurfaceReconstructionFilter.h> 
#include	<vtkDataSetTriangleFilter.h> 
#include	<vtkCellArray.h>
#include	<vtkContourGrid.h> 

#include	<vtkTransform.h>

#include	<vtkTransformPolyDataFilter.h> 
#include	<vtkStripper.h>
#include	<vtkCellArray.h>
#include	<vtkLine.h>
#include	<vtkDataArray.h> 
#include	<vtkGenericCell.h>
#include	<vtkFloatArray.h>

#include	<vtkPolygon.h>

#include	<vtkOBBTree.h> 
#include	<vtkContourGrid.h> 

#include	<vtkPolyData.h>
#include	<vtkTriangle.h>
#include	<vtkPointData.h>

#include	"PlaneObject.h"
#include	"RawDataSet.h"
#include	"OtherCommonFunctions.h"
#include	"SurfaceProcessing.h"

#include	"LinkedList.h"
#include	"Geometry.h"


#include 	<qapplication.h>
//#include 	<q3filedialog.h>
#include 	<qcursor.h>



class GraftVoxelize: public QObject
{
	
	Q_OBJECT
	
	public:

		GraftVoxelize();
	
		~GraftVoxelize(void);
	
	int miX,miY,miZ;
	vtkPolyData		*FillPolydataHolesWithCrenellations(vtkPolyData *Input);
	vtkPolyData		*FillPolydataHoles(vtkPolyData *Input);
	vtkPolyData		*FillPolydataPlanarHoles(vtkPolyData *Input);

	vtkPolyData		*FillPolydataHolesMaxVolume(vtkPolyData *Input);

	vtkPolyData		*CloseGraftSurface(vtkPolyData *Input  ,int percentage, double precision, int *NumberOfPlanes,double *PlanePoints);

	vtkCellArray	*FillPlanarHole(vtkPoints *Points, int *HoleIndexPoints,int NumberOfPoints);

	RawDataSet		*CreateVolumeFormSurface(QString filename,vtkPolyData *Input,RawDataSet *Data);
	RawDataSet		*CreateVolumeFormSurface(vtkPolyData *Input,int sx,int sy,int sz);

	signals:

	void	SendMessage(const QString &Message);
};


#endif //GRAFT_VOXELIZE
