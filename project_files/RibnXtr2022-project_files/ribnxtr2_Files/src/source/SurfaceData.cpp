#include "SurfaceData.h"


SurfaceData::SurfaceData()
{
	VVisible=0;
	VActor=NULL;
	VPolyData=NULL;
	VProperty=vtkProperty::New();
	
	




}

void SurfaceData::Delete()
{

	if (VActor!=NULL) VActor->Delete();
	if (VPolyData!=NULL) VPolyData->Delete();
	VVisible=0;
	VActor=NULL;
	VPolyData=NULL;

}


SurfaceData::~SurfaceData()
{
	if (VActor!=NULL) 	VActor->Delete();
	if (VPolyData!=NULL) 	VPolyData->Delete();

	VActor=NULL;
	VPolyData=NULL;
	VProperty->Delete();

	// wstawic kasowanie property widgetu.. pozniej
}
