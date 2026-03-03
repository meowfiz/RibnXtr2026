#include "CylinderBoxActor.h"


CylinderBoxActor::CylinderBoxActor(VtkWidget *_VWidget,vtkProperty *_Property)
{
	//OLD AND DIRTY!!!!!!!!!!!!!
	CylinderBoxActorVisible=false;
	
	Property = _Property;
	VWidget = _VWidget;

	vtkCylinderSource *Axe=vtkCylinderSource::New();
	Axe->SetResolution(10);

	CubeH1Actor 	=	vtkActor::New();
	CubeH2Actor 	=	vtkActor::New();
	CubeH3Actor 	=	vtkActor::New();
	CubeH4Actor 	=	vtkActor::New();

	CubeVU1Actor 	=	vtkActor::New();
	CubeVU2Actor 	=	vtkActor::New();
	CubeVU3Actor 	=	vtkActor::New();
	CubeVU4Actor 	=	vtkActor::New();

	CubeVD1Actor 	=	vtkActor::New();
	CubeVD2Actor 	=	vtkActor::New();
	CubeVD3Actor 	=	vtkActor::New();
	CubeVD4Actor 	=	vtkActor::New();




	vtkPolyDataMapper *CubePartMapper	=	vtkPolyDataMapper::New();
		CubePartMapper->SetInputConnection(Axe->GetOutputPort());
	//CubePartMapper->SetInputData(Axe->GetOutput());

	CubeH1Actor->SetMapper(CubePartMapper);
	CubeH2Actor->SetMapper(CubePartMapper);
	CubeH3Actor->SetMapper(CubePartMapper);
	CubeH4Actor->SetMapper(CubePartMapper);

	CubeVU1Actor->SetMapper(CubePartMapper);
	CubeVU2Actor->SetMapper(CubePartMapper);
	CubeVU3Actor->SetMapper(CubePartMapper);
	CubeVU4Actor->SetMapper(CubePartMapper);

	CubeVD1Actor->SetMapper(CubePartMapper);
	CubeVD2Actor->SetMapper(CubePartMapper);
	CubeVD3Actor->SetMapper(CubePartMapper);
	CubeVD4Actor->SetMapper(CubePartMapper);

	CubeH1Actor->SetProperty(Property);
	CubeH2Actor->SetProperty(Property);
	CubeH3Actor->SetProperty(Property);
	CubeH4Actor->SetProperty(Property);

	CubeVU1Actor->SetProperty(Property);
	CubeVU2Actor->SetProperty(Property);
	CubeVU3Actor->SetProperty(Property);
	CubeVU4Actor->SetProperty(Property);

	CubeVD1Actor->SetProperty(Property);
	CubeVD2Actor->SetProperty(Property);
	CubeVD3Actor->SetProperty(Property);
	CubeVD4Actor->SetProperty(Property);


	Center=new double[3];
	Size=new double[3];

	int i;
	for(i=0;i<3;i++)
	{

		Center[i]=0;
		Size[i]=0;
	}
	Radius=1.0;
	

}

//----------------------------------------------------------------

CylinderBoxActor::~CylinderBoxActor()
{


}

//----------------------------------------------------------------
void	CylinderBoxActor::SlotUpdateTransformCylinderBoxActor()
{
	SlotTransformCylinderBoxActor(Center[0],Center[1],Center[2],Size[0],Size[1],Size[2],Radius);

}
//----------------------------------------------------------------
void	CylinderBoxActor::SlotMoveCylinderBoxActor(double *Pos)
{

	SlotTransformCylinderBoxActor(Pos[0],Pos[1],Pos[2],Size[0],Size[1],Size[2],Radius);
}
//----------------------------------------------------------------
void CylinderBoxActor::SlotResizeCyliCylinderBoxActor(double XSize,double YSize,double ZSize)
{

	SlotTransformCylinderBoxActor(	Center[0],Center[1],Center[2],
									XSize,YSize,ZSize,Radius);
}

//------------------------------------------------------------------

void	CylinderBoxActor::SlotTransformCylinderBoxActor(double XPos,double YPos,double ZPos,double XSize,double YSize,double ZSize,double _Radius)
{
		Radius=_Radius;
		Center[0]=XPos;
		Center[1]=YPos;
		Center[2]=ZPos;

		Size[0]=XSize;
		Size[1]=YSize;
		Size[2]=ZSize;

		CubeVD1Actor->SetScale(Radius*2.0,XSize,_Radius*2.0);
		CubeVD2Actor->SetScale(Radius*2.0,YSize,_Radius*2.0);
		CubeVD3Actor->SetScale(Radius*2.0,XSize,_Radius*2.0);
		CubeVD4Actor->SetScale(Radius*2.0,YSize,_Radius*2.0);


		CubeVU1Actor->SetScale(Radius*2.0,XSize,_Radius*2.0);
		CubeVU2Actor->SetScale(Radius*2.0,YSize,_Radius*2.0);
		CubeVU3Actor->SetScale(Radius*2.0,XSize,_Radius*2.0);
		CubeVU4Actor->SetScale(Radius*2.0,YSize,_Radius*2.0);
		
		CubeH1Actor->SetScale(Radius*2.0,ZSize,_Radius*2.0);
		CubeH2Actor->SetScale(Radius*2.0,ZSize,_Radius*2.0);
		CubeH3Actor->SetScale(Radius*2.0,ZSize,_Radius*2.0);
		CubeH4Actor->SetScale(Radius*2.0,ZSize,_Radius*2.0);


	
		
		//Down

		CubeVD1Actor->SetPosition(XPos,YPos-YSize/2.0,ZPos-ZSize/2.0);
		CubeVD1Actor->SetOrientation(0,0,90.0);
		CubeVD2Actor->SetPosition(XPos-XSize/2.0,YPos,ZPos-ZSize/2.0);
		CubeVD2Actor->SetOrientation(0,0,0);
		CubeVD3Actor->SetPosition(XPos,YPos+YSize/2.0,ZPos-ZSize/2.0);
		CubeVD3Actor->SetOrientation(0,0,90.0);
		CubeVD4Actor->SetPosition(XPos+XSize/2.0,YPos,ZPos-ZSize/2.0);
		CubeVD4Actor->SetOrientation(0,0,0);


		//Up
		

		CubeVU1Actor->SetPosition(XPos,YPos-YSize/2.0,ZPos+ZSize/2.0);
		CubeVU1Actor->SetOrientation(0,0,90.0);
		CubeVU2Actor->SetPosition(XPos-XSize/2.0,YPos,ZPos+ZSize/2.0);
		CubeVU2Actor->SetOrientation(0,0,0);
		CubeVU3Actor->SetPosition(XPos,YPos+YSize/2.0,ZPos+ZSize/2.0);
		CubeVU3Actor->SetOrientation(0,0,90.0);
		CubeVU4Actor->SetPosition(XPos+XSize/2.0,YPos,ZPos+ZSize/2.0);
		CubeVU4Actor->SetOrientation(0,0,0);


		//Vertical

		CubeH1Actor->SetPosition(XPos-XSize/2.0,YPos-YSize/2.0,ZPos);
		CubeH1Actor->SetOrientation(90.0,0,0);
		CubeH2Actor->SetPosition(XPos-XSize/2.0,YPos+YSize/2.0,ZPos);
		CubeH2Actor->SetOrientation(90.0,0,0);
		CubeH3Actor->SetPosition(XPos+XSize/2.0,YPos-YSize/2.0,ZPos);
		CubeH3Actor->SetOrientation(90.0,0,0);
		CubeH4Actor->SetPosition(XPos+XSize/2.0,YPos+YSize/2.0,ZPos);
		CubeH4Actor->SetOrientation(90.0,0,0);


		if (CylinderBoxActorVisible==true)	
			VWidget->Interactor->repaint();


}

//-----------------------------------------------------------------

void	CylinderBoxActor::SlotShowCylinderBoxActor()
{

	if (CylinderBoxActorVisible==false)
	{

		VWidget->Renderer->AddActor(CubeVD1Actor);
		VWidget->Renderer->AddActor(CubeVD2Actor);
		VWidget->Renderer->AddActor(CubeVD3Actor);
		VWidget->Renderer->AddActor(CubeVD4Actor);
		VWidget->Renderer->AddActor(CubeVU1Actor);
		VWidget->Renderer->AddActor(CubeVU2Actor);
		VWidget->Renderer->AddActor(CubeVU3Actor);
		VWidget->Renderer->AddActor(CubeVU4Actor);
		VWidget->Renderer->AddActor(CubeH1Actor);
		VWidget->Renderer->AddActor(CubeH2Actor);
		VWidget->Renderer->AddActor(CubeH3Actor);
		VWidget->Renderer->AddActor(CubeH4Actor);

		VWidget->Renderer->ViewToWorld();
		VWidget->Interactor->repaint();
		VWidget->Renderer->ResetCamera();

		CylinderBoxActorVisible=true;
	}


}

//----------------------------------------------------------------

void	CylinderBoxActor::SlotHideCylinderBoxActor()
{
	if (CylinderBoxActorVisible==true)
	{

		VWidget->Renderer->RemoveActor(CubeVD1Actor);
		VWidget->Renderer->RemoveActor(CubeVD2Actor);
		VWidget->Renderer->RemoveActor(CubeVD3Actor);
		VWidget->Renderer->RemoveActor(CubeVD4Actor);
		VWidget->Renderer->RemoveActor(CubeVU1Actor);
		VWidget->Renderer->RemoveActor(CubeVU2Actor);
		VWidget->Renderer->RemoveActor(CubeVU3Actor);
		VWidget->Renderer->RemoveActor(CubeVU4Actor);
		VWidget->Renderer->RemoveActor(CubeH1Actor);
		VWidget->Renderer->RemoveActor(CubeH2Actor);
		VWidget->Renderer->RemoveActor(CubeH3Actor);
		VWidget->Renderer->RemoveActor(CubeH4Actor);
	
		VWidget->Renderer->ViewToWorld();
		VWidget->Interactor->repaint();
		VWidget->Renderer->ResetCamera();

		CylinderBoxActorVisible=false;
	}

}

//----------------------------------------------------------------

void	CylinderBoxActor::SlotUpdate()
{

	VWidget->RepaintWidget();
}
