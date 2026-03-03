
#ifndef	SURFACE_DATA
#define	SURFACE_DATA





#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkAppendPolyData.h>

#include <stdio.h>

class SurfaceData 
{
	public:
	SurfaceData();
	~SurfaceData();

	void Delete();

	vtkActor *VActor;
	vtkPolyData *VPolyData;
	vtkProperty *VProperty;

	vtkAppendPolyData *VAppendPolyData;

	bool		VVisible;

};


#endif //SURFACE_DATA

