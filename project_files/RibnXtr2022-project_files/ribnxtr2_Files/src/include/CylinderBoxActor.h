#ifndef	CYLINDER_BOX_ACTOR
#define	CYLINDER_BOX_ACTOR



#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkDataSetWriter.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkCylinderSource.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <qapplication.h>

#include "VtkWidget.h"

class CylinderBoxActor : public QObject
{

	Q_OBJECT
  
public:

	CylinderBoxActor(VtkWidget *_VWidget,vtkProperty *_Property);
	~CylinderBoxActor();


  	vtkActor *CubeH1Actor,*CubeH2Actor,*CubeH3Actor,*CubeH4Actor;
	vtkActor *CubeVU1Actor,*CubeVU2Actor,*CubeVU3Actor,*CubeVU4Actor;
	vtkActor *CubeVD1Actor,*CubeVD2Actor,*CubeVD3Actor,*CubeVD4Actor;
	bool CylinderBoxActorVisible;

	
	VtkWidget *VWidget;
	vtkProperty *Property;

	double *Center;
	double *Size;
	double Radius;

  public slots:

	void	SlotTransformCylinderBoxActor(double XPos,double YPos,double ZPos,double XSize,double YSize,double ZSize,double Radius);
	void	SlotUpdateTransformCylinderBoxActor();
	void	SlotShowCylinderBoxActor();
	void	SlotHideCylinderBoxActor();
	void	SlotUpdate();
	void	SlotResizeCyliCylinderBoxActor(double XSize,double YSize,double ZSize);
	void	SlotMoveCylinderBoxActor(double *Pos);
};	 	
			
#endif //   CYLINDER_BOX_ACTOR

