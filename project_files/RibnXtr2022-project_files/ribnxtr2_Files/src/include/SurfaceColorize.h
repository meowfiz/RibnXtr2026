
#ifndef	SURFACE_COLORIZE
#define	SURFACE_COLORIZE


#include "OtherCommonFunctions.h"
#include "SurfaceCollection.h"
#include "SurfaceData.h"
#include "DataSet.h"
#include "RawDataSet.h"
#include "Grid3D.h"

#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkAppendPolyData.h>

#include <stdio.h>


class SurfaceColorize 
{
	public:
	SurfaceColorize();
	~SurfaceColorize();

	void	MapPaletteOnZAxisToMesh(PolygonSurfaceData *meshData,DataSet *data);
	void	MapCorrespondingVolumeToMesh(PolygonSurfaceData *meshData,DataSet *data);
	void	MapTabPaletteToMesh(vtkPolyData *meshData,double *tab,PaletteRGBA *palette);
	void	MapTabPaletteToMesh(PolygonSurfaceData *meshData, double *tab, PaletteRGBA *palette);
};


#endif //SURFACE_COLORIZE

