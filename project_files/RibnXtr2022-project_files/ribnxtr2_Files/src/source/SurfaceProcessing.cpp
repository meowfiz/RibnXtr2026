
#include "SurfaceProcessing.h"

//-------------------------------------------------------

SurfaceProcessing::SurfaceProcessing(VtkWidget *_VWidget)
{
	 
	VWidget=_VWidget;
	CuttingMode=0; //CuttingPlane
	CuttingToolVisible=false;
	ObjectSurfaceData=NULL;
	PreviousCuttingToolActor=NULL;
	CurrentCuttingToolActor=NULL;
	CutingToolSize=50.0;


	CurrentPointNumber=-1;

	enableAffineTransformation=false;
	affineTransform=NULL; 

	//Create Actors....

	//Plane
	vtkPlaneSource *Plane=vtkPlaneSource::New();
	PlaneActor=vtkActor::New();
	vtkPolyDataMapper *PlaneMapper= vtkPolyDataMapper::New();
	PlaneMapper->SetInputConnection(Plane->GetOutputPort());
	PlaneActor->SetMapper(PlaneMapper);
	PlaneActor->SetScale(CutingToolSize);

	vtkProperty *P=vtkProperty::New();
	P->SetDiffuseColor(0.4,0.4,0.7);
	PlaneActor->SetProperty(P);

	vtkProperty *P1=vtkProperty::New();
	P1->SetDiffuseColor(0.7,0.3,0.4);
	PlaneActor->SetBackfaceProperty(P1);


	//Sphere

	vtkSphereSource *Sphere=vtkSphereSource::New();
	Sphere->SetPhiResolution(30);
	Sphere->SetThetaResolution(30);
	SphereActor=vtkActor::New();
	vtkPolyDataMapper *SphereMapper= vtkPolyDataMapper::New();
	SphereMapper->SetInputConnection(Sphere->GetOutputPort());
	SphereActor->SetMapper(SphereMapper);
	SphereActor->SetScale(CutingToolSize);

	SphereActor->SetProperty(P);
	SphereActor->SetBackfaceProperty(P1);

	//CircleSaw



	vtkCylinderSource *Cylinder=vtkCylinderSource::New();
	Cylinder->SetResolution(50);
	CylinderActor=vtkActor::New();
	vtkPolyDataMapper *CylinderMapper= vtkPolyDataMapper::New();
	CylinderMapper->SetInputConnection(Cylinder->GetOutputPort());
	CylinderActor->SetMapper(CylinderMapper);
	CylinderActor->SetScale(CutingToolSize,0.0,CutingToolSize);




	CuttingToolMatrix=vtkMatrix4x4::New();

	//	VWidget->Renderer->AddActor(PlaneActor);
	//	VWidget->Renderer->AddActor(SphereActor);
	//	VWidget->Renderer->AddActor(CylinderActor);
	connect(&RTimer,SIGNAL( SendMessage(const QString &)),this,SIGNAL( SendMessage(const QString &)));

}

//-------------------------------------------------------

SurfaceProcessing::~SurfaceProcessing()
{

}

//-------------------------------------------------------

void	SurfaceProcessing::SlotSetCurrentSurface(PolygonSurfaceData *_SurfaceData)
{
	ObjectSurfaceData=_SurfaceData;
	CurrentPointNumber=-1;
}

//-------------------------------------------------------

void	SurfaceProcessing::SlotSetCuttingMode(int Mode)
{


	if (!CuttingToolVisible)
	{
		CuttingMode=Mode;
		//	SlotShowHideCuttingTool(); //pokaz ??????????
	}
	else
	{
		SlotShowHideCuttingTool(); //schowaj
		CuttingMode=Mode; //zmien
		//	CurrentCuttingToolActor->GetMatrix(CuttingToolMatrix);
		SlotShowHideCuttingTool(); //pokaz

	}




}

//-------------------------------------------------------

void	SurfaceProcessing::SlotSetCuttingToolSize(int Size)
{

	if (ObjectSurfaceData!=NULL)
	{
		double bounds[6];

		ObjectSurfaceData->GetVPolyData()->GetBounds(bounds);


		double maxsize[3];

		int i;
		for(i=0;i<3;i++)
			maxsize[i]=fabs(bounds[i*2+1]-bounds[i*2]);


		double max=MAX3(maxsize[0],maxsize[1],maxsize[2])*2;
		double min=MIN3(maxsize[0],maxsize[1],maxsize[2])/2000;

		double RealSize=min+((max-min)*(double)Size)/100.0;


		CutingToolSize=RealSize;
		PlaneActor->SetScale(CutingToolSize);
		SphereActor->SetScale(CutingToolSize*0.01);
		CylinderActor->SetScale(CutingToolSize,0.0,CutingToolSize);
		VWidget->RepaintWidget();
	}


}

//-------------------------------------------------------

void	SurfaceProcessing::SlotShowHideCuttingTool()
{


	switch (CuttingMode)
	{


	case 0: //plane

		if (CuttingToolVisible)
			PreviousCuttingToolActor=PlaneActor;
		else
			CurrentCuttingToolActor=PlaneActor;


		break;


	case 1: //sphere

		if (CuttingToolVisible)
			PreviousCuttingToolActor=SphereActor;
		else
			CurrentCuttingToolActor=SphereActor;

		break;


	case 2: // circle saw

		if (CuttingToolVisible)
			PreviousCuttingToolActor=CylinderActor;
		else
			CurrentCuttingToolActor=CylinderActor;
		break;

	}


	CuttingToolVisible=!CuttingToolVisible;

	if((CurrentCuttingToolActor!=NULL)&&(PreviousCuttingToolActor!=NULL)&&(CurrentCuttingToolActor!=PreviousCuttingToolActor))
	{

		double OrientationX[3],PositionX[3];
		//double *OrientationX=new double[3];
		//double *PositionX=new double[3];
		int i;

		for(i=0;i<3;i++)
		{
			OrientationX[i]=PreviousCuttingToolActor->GetOrientation()[i];
			PositionX[i]=PreviousCuttingToolActor->GetPosition()[i];
		}

		CurrentCuttingToolActor->SetOrientation(OrientationX);
		CurrentCuttingToolActor->SetPosition(PositionX);


	}


	if (CuttingToolVisible)
	{
		VWidget->Renderer->AddActor(CurrentCuttingToolActor);
		emit SignalSendCuttingActor();
	}
	else
	{
		VWidget->Renderer->RemoveActor(PreviousCuttingToolActor);
	}

	VWidget->RepaintWidget();
}

//-------------------------------------------------------

void	SurfaceProcessing::SlotCutSurface()
{

	//	FILE *Do_opa;
	//	Do_opa=fopen("c:\\cutt.txt","a");fprintf(Do_opa,"YO\n");fclose(Do_opa);

	if ((ObjectSurfaceData!=NULL)&&(CuttingToolVisible))
	{
		vtkImplicitFunction *CuttingFunction;
		//przetransformowac trzeba ...
		SlotApplyActorTransformation();
		vtkPolyData *Input=ObjectSurfaceData->GetVPolyData();
		vtkClipPolyData *Cutter		=	 vtkClipPolyData::New();
		//double *temp=new double[3];
		//double *temp1=new double[3];
		//double *temp2=new double[3];
		//double *Normal= new double[4];
		//double *Normal2= new double[4];
		double temp[3],temp1[3],temp2[3],Normal[3],Normal2[3];
		vtkMatrix4x4 *Matrix   =	vtkMatrix4x4::New();
		vtkPlane *CuttingPlane	= vtkPlane::New();
		vtkSphere *CuttingSphere	= vtkSphere::New();

		if (CuttingMode==0)//plane
		{

			PlaneActor->GetOrientation(temp);
			PlaneActor->GetOrigin(temp1);
			PlaneActor->GetPosition(temp2);
			temp2[0]+=temp1[0];		temp2[1]+=temp1[1];		temp2[2]+=temp1[2];
			Normal[0]=0;		Normal[1]=0;		Normal[2]=1;	

			PlaneActor->GetMatrix(Matrix);
			Matrix->MultiplyPoint (Normal,Normal2);
			temp[0]=Normal2[0];		temp[1]=Normal2[1];		temp[2]=Normal2[2];

			CuttingPlane->SetOrigin(temp2);
			CuttingPlane->SetNormal(temp);
			Cutter->SetClipFunction(CuttingPlane);
			CuttingFunction=CuttingPlane;
		}	
		else if (CuttingMode==1)//sphere
		{

			SphereActor->GetOrigin(temp1);
			SphereActor->GetPosition(temp2);
			temp2[0]+=temp1[0];		temp2[1]+=temp1[1];		temp2[2]+=temp1[2];

			double SSize=SphereActor->GetScale()[0]/2.0;
			CuttingSphere->SetCenter(temp2);
			CuttingSphere->SetRadius(SSize);
			Cutter->SetClipFunction(CuttingSphere);
			CuttingFunction=CuttingSphere;

		

		}
		Cutter->SetInputData(Input);
		Cutter->SetValue(0.0);
		Cutter->GenerateClippedOutputOn();
		Cutter->Update();
		vtkPolyData *Output1=vtkPolyData::New();
		vtkPolyData *Output2=vtkPolyData::New();

		Output2->DeepCopy(Cutter->GetOutput());
		//Output2->Update();
		Output1->DeepCopy(Cutter->GetClippedOutput());
		//Output1->Update();
		Cutter->Delete();

		Matrix->Delete();
		//delete	[]Normal2;
	//	delete	[]Normal;

		int nr1=Output1->GetNumberOfPolys();
		int nr2=Output2->GetNumberOfPolys();

		if(((nr1>0)&&(nr2>0)))
		{

			vtkPolyData *Zupa=GenerateCuttingBoundary(Output2,CuttingFunction);
			vtkAppendPolyData *Append1=vtkAppendPolyData::New();
			Append1->AddInputData(Zupa);

			vtkAppendPolyData *Append2=vtkAppendPolyData::New();
			Append2->AddInputData(Zupa);

			if (ObjectSurfaceData->GetCuttingEdgePolyData()!=NULL)
			{

				Cutter		=	 vtkClipPolyData::New();
				Cutter->SetClipFunction(CuttingFunction);
				Cutter->SetInputData(ObjectSurfaceData->GetCuttingEdgePolyData());
				Cutter->SetValue(0.0);
				Cutter->GenerateClippedOutputOn();

				Cutter->Update();

				vtkPolyData *Output1Edge=vtkPolyData::New();
				vtkPolyData *Output2Edge=vtkPolyData::New();

				Output2Edge->DeepCopy(Cutter->GetOutput());
//				Output2Edge->Update();
				Output1Edge->DeepCopy(Cutter->GetClippedOutput());
		//		Output1Edge->Update();
				Cutter->Delete();

				if (Output2Edge->GetNumberOfCells()>0)
				{
					Append2->AddInputData(Output2Edge);
				}

				if (Output1Edge->GetNumberOfCells()>0)
				{
					Append1->AddInputData(Output1Edge);
				}
			}

			vtkCleanPolyData *Clean1 = vtkCleanPolyData::New();
			Clean1->SetInputData(Output1);
			Clean1->PointMergingOn ();
			Clean1->ConvertStripsToPolysOn ();
			Clean1->ConvertLinesToPointsOn ();
			Clean1->ConvertPolysToLinesOff ();
			Clean1->SetTolerance(1e-5);
			Clean1->Update();

			vtkCleanPolyData *Clean2 = vtkCleanPolyData::New();
			Clean2->SetInputData(Output2);
			Clean2->PointMergingOn ();
			Clean2->ConvertStripsToPolysOn ();
			Clean2->ConvertLinesToPointsOn ();
			Clean2->ConvertPolysToLinesOff ();
			Clean2->SetTolerance(1e-5);
			Clean2->Update();

			PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(Clean1->GetOutput(),VWidget);
			PolygonSurfaceData *Output2PolygonSurfaceData=new PolygonSurfaceData(Clean2->GetOutput(),VWidget);

			Output1PolygonSurfaceData->ReplaceCuttingEdgePolyData(Append1->GetOutput());
			Output2PolygonSurfaceData->ReplaceCuttingEdgePolyData(Append2->GetOutput());

			int i;

			for(i=0;i<3;i++)
			{
				Output1PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
				Output2PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
				Output1PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];
				Output2PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];

			}

			Output1PolygonSurfaceData->GetVProperty()->DeepCopy(ObjectSurfaceData->GetVProperty());
			Output2PolygonSurfaceData->GetVProperty()->DeepCopy(ObjectSurfaceData->GetVProperty());

			ObjectSurfaceData->SetHideWithoutRefresh(true);

			Output1PolygonSurfaceData->ComputeGeometricOrigin();
			Output2PolygonSurfaceData->ComputeGeometricOrigin();

			QString name;
			name=ObjectSurfaceData->GetPolygonSurfaceName();
			name.append(".R.");
			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
			name=ObjectSurfaceData->GetPolygonSurfaceName();
			name.append(".O.");
			Output2PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);

			//emit	SignalDeleteCurrentSurface();
			Output1PolygonSurfaceData->SetHideWithoutRefresh(true);
			emit	SignalSendNewSurfaceAfterDelete(Output1PolygonSurfaceData);
			Output1PolygonSurfaceData->SetHideWithoutRefresh(false);
			emit	SignalSendNewSurfaceAfterDelete(Output2PolygonSurfaceData);

		}
		else
		{

			//pokasuj troche
		}

		CuttingPlane->Delete();
		CuttingSphere->Delete();
	}

}
//---------------------------------------------------
void SurfaceProcessing::GetCuttingSphereParams(double *center,double &size)
{
	double temp1[3],temp2[3];

	SphereActor->GetOrigin(temp1);
	SphereActor->GetPosition(temp2);
	temp2[0]+=temp1[0];		temp2[1]+=temp1[1];		temp2[2]+=temp1[2];
	center[0]=temp2[0];		center[1]=temp2[1];		center[2]=temp2[2]; 
	size=SphereActor->GetScale()[0]/2.0;
}
//-------------------------------------------------------
vtkPolyData	*SurfaceProcessing::GenerateCuttingBoundary(vtkPolyData *PData,vtkImplicitFunction *Func)
{

	//robimy kraw�zie
	vtkFeatureEdges *edges=vtkFeatureEdges::New();
	edges->SetInputData(PData);
	edges->BoundaryEdgesOn();
	edges->FeatureEdgesOff();
	edges->ManifoldEdgesOff();
	edges->NonManifoldEdgesOff ();
	edges->Update();

	//czycimy
	vtkCleanPolyData *boundaryClean = vtkCleanPolyData::New();
	boundaryClean->SetInputData(edges->GetOutput());
	boundaryClean->PointMergingOn ();
	boundaryClean->Update();
	vtkPolyData *XXPoly=boundaryClean->GetOutput();
//	XXPoly->Update();
	XXPoly->BuildLinks();


	//wywalamy te krawedzie co nie naleza do fkcji tnacej
	vtkPoints *pPointsX=XXPoly->GetPoints();
	int i;
	int NumberOfPoints=XXPoly->GetNumberOfPoints();
	double PointX1[3],PointX2[3];
	int NumberOfLines=XXPoly->GetNumberOfCells();
	vtkPoints			*LinePoints;
	vtkGenericCell		*LineCell=vtkGenericCell::New();

	int punkt0ID,punkt1ID;
	int NumberOfDeletedCells=0;

	double Prec;;
	Prec= (Func->IsA("vtkSphere"))?5:0.1;

	for (i=0;i<NumberOfLines-1;i++)
	{

		XXPoly->GetCell(i,LineCell);
		LinePoints=LineCell->GetPoints();
		punkt0ID=LineCell->GetPointId(0);
		punkt1ID=LineCell->GetPointId(1);



		pPointsX->GetPoint(punkt0ID,PointX1);
		pPointsX->GetPoint(punkt1ID,PointX2);

		double a=fabs(Func->FunctionValue(PointX1[0],PointX1[1],PointX1[2]));
		double b=fabs(Func->FunctionValue(PointX2[0],PointX2[1],PointX2[2]));
		if( (a>Prec)||(b>Prec))
		{
			XXPoly->DeleteCell(i);
			if (a>Prec)	XXPoly->DeletePoint(punkt0ID);
			if (b>Prec)	XXPoly->DeletePoint(punkt1ID);
			NumberOfDeletedCells++;
		}
	}
	//-----------------------
	//przebudowa meshu ....

	vtkPoints *newPts=vtkPoints::New();
	newPts->SetNumberOfPoints(NumberOfPoints);

	vtkCellArray *newPolys = vtkCellArray::New();
	newPolys->Allocate(newPolys->EstimateSize(2,NumberOfLines-NumberOfDeletedCells));

	vtkPolyData *output=vtkPolyData::New();
	output->SetPoints(newPts);
	output->SetLines(newPolys);

	vtkCellData		*outputCD=output->GetCellData();

	vtkIdType*cells, cellId,*pts;
	vtkIdType newCellPts[3];
	int numNewPts = 0;
	int ptId;
	vtkIdType  ncells;

	//ustawwiam tablice tumczasowa na -1
	vtkIdType *map = new vtkIdType[NumberOfPoints];
	for (i=0; i < NumberOfPoints; i++)
		map[i] = -1;

	// tworze tablice indexow
	for (ptId=0; ptId < NumberOfPoints; ptId++)
	{
		XXPoly->GetPointCells(ptId,ncells,cells);
		if ( ncells > 0 )
			map[ptId] = numNewPts++;
	}

	vtkFloatArray *Tscalars = vtkFloatArray::New();
	outputCD->SetScalars(Tscalars); 
	newPts->SetNumberOfPoints(numNewPts);

	//tworze nowa tablice pktow

	for (ptId=0; ptId < NumberOfPoints; ptId++)
	{
		if ( map[ptId] > -1 )
			newPts->SetPoint(map[ptId],pPointsX->GetPoint(ptId));
	}
	output->BuildLinks();


	vtkIdType np;
	vtkIdType ID;
	double scalar=2.0;
	//buduje linie
	vtkIdList* pointList = vtkIdList::New();
	for (cellId=0; cellId < NumberOfLines-1; cellId++)
	{
		if ( XXPoly->GetCellType(cellId) == VTK_LINE ) // non-null element
		{
			XXPoly->GetCellPoints(cellId, pointList);
			for (i=0; i < 2; i++)
				newCellPts[i] = map[pointList->GetId(i)];

			ID=newPolys->InsertNextCell(2,newCellPts);	
			Tscalars->InsertTuple(ID,&scalar); 
		}
	}

	output->BuildCells();
//	//output->Update();
	newPolys->Delete();
	newPts->Delete();
	delete [] map;
	boundaryClean->Delete();
	edges->Delete();
	return output;
}
//-------------------------------------------------------
void	SurfaceProcessing::SlotSetSurfacePoint(int Nr)
{
	if (ObjectSurfaceData!=NULL)
	{
		CurrentPointNumber=Nr;
		CuttingToolVisible=true;
		SlotSetCuttingMode(1); //USTAW KULKE
		vtkPoints *Pts=ObjectSurfaceData->GetVPolyData()->GetPoints();
		int NrOfPts=Pts->GetNumberOfPoints();
		double PointX1[3]; 
		Pts->GetPoint(CurrentPointNumber,PointX1);
		CurrentCuttingToolActor->SetPosition(PointX1);
		QString txt,txt2;
		txt="Selected point: ";
		txt2.sprintf("(%d/%d) = [%lf %lf %lf]",CurrentPointNumber,NrOfPts-1,PointX1[0],PointX1[1],PointX1[2] );
		txt.append(txt2);
		emit SendMessage(txt);
		VWidget->RepaintWidget();
	}
}
//---------------------------------------
void	SurfaceProcessing::SlotTransformSurfacePoint()
{
	if(CurrentPointNumber>=0)
	{
		vtkPoints *Pts=ObjectSurfaceData->GetVPolyData()->GetPoints();
		double PointX1[3]; 
		CurrentCuttingToolActor->GetPosition(PointX1);
		Pts->SetPoint(CurrentPointNumber,PointX1);
		ObjectSurfaceData->GetVPolyData()->SetPoints(Pts);
		ObjectSurfaceData->RebuildSurface();
	}
}
//---------------------------------------
void SurfaceProcessing::SlotResetActorTransformation()
{
	if (ObjectSurfaceData!=NULL)
	{
		double Point[3];
		Point[0]=0;Point[1]=0;Point[2]=0;
		ObjectSurfaceData->GetVActor()->SetOrientation(Point);
		ObjectSurfaceData->GetVActor()->SetPosition(Point);
		if (ObjectSurfaceData->GetCuttingEdgePolyData()!=NULL)
		{
			ObjectSurfaceData->GetCuttingEdgeActor()->SetOrientation(Point);
			ObjectSurfaceData->GetCuttingEdgeActor()->SetPosition(Point);
		}
		VWidget->RepaintWidget();
	}
}
//---------------------------------------
void SurfaceProcessing::SlotApplyActorTransformation()
{
	if (ObjectSurfaceData!=NULL)
	{




		vtkTransform *tr = vtkTransform::New();
		tr->SetMatrix(ObjectSurfaceData->GetVActor()->GetMatrix());
		tr->Update();

		vtkTransformPolyDataFilter *transX = vtkTransformPolyDataFilter::New();

		transX->SetTransform(tr);//lTrans->MakeTransform());
		transX->SetInputData(ObjectSurfaceData->GetVPolyData());
		transX->Update();

		vtkPolyData *pp = vtkPolyData::New();
		pp->DeepCopy(transX->GetOutput());


		ObjectSurfaceData->SetVPolyData(transX->GetOutput());




//
//		vtkPolyData *TempPolyDataX;
//		vtkTransform *Transform=vtkTransform::New();
//		Transform->SetMatrix(ObjectSurfaceData->GetVActor()->GetMatrix());
//		vtkTransformPolyDataFilter *TransformFilter=vtkTransformPolyDataFilter::New();
//		TransformFilter->SetInputData(ObjectSurfaceData->GetVPolyData());
//		TransformFilter->SetTransform(Transform);
//
//		TempPolyDataX=ObjectSurfaceData->GetVPolyData();
//		ObjectSurfaceData->SetVPolyData(TransformFilter->GetOutput());
////		ObjectSurfaceData->GetVPolyData()->Update();
//		TempPolyDataX->Delete();
//
//		if (ObjectSurfaceData->GetCuttingEdgePolyData()!=NULL)
//		{
//			vtkTransformPolyDataFilter *TransformFilter2=vtkTransformPolyDataFilter::New();
//			TransformFilter2->SetInputData(ObjectSurfaceData->GetCuttingEdgePolyData());
//			TransformFilter2->SetTransform(Transform);
//
//			TempPolyDataX=ObjectSurfaceData->GetCuttingEdgePolyData();
//			ObjectSurfaceData->SetCuttingEdgePolyData(TransformFilter2->GetOutput());
////			ObjectSurfaceData->GetCuttingEdgePolyData()->Update();
//			TempPolyDataX->Delete();
//		}
//		Transform->Delete();
	}
}

//---------------------------------------

void SurfaceProcessing::SlotApplyActorTransformationAndRebuild()
{
	if (ObjectSurfaceData!=NULL)
	{
		//		FILE *Do_opa;
		//Do_opa=fopen("c:\\cutt.txt","a");fprintf(Do_opa,"YO\n");fclose(Do_opa);

		SlotApplyActorTransformation();
		ObjectSurfaceData->RebuildSurface();
		ObjectSurfaceData->ComputeGeometricOrigin();
		//emit SignalSendNewSurfaceOrigin(PointX1); - to trzeba dodac
		VWidget->RepaintWidget();
	}
}
//----------------------------------------
void	SurfaceProcessing::	CalculateArrowPoint(double *q, double *p,double *r,double *s,double rc,double rs)
{
	q[0]=p[0]+rc*r[0]+rs*s[0];
	q[1]=p[1]+rc*r[1]+rs*s[1];
	q[2]=p[2]+rc*r[2]+rs*s[2];

}
//----------------------------------------
//----------------------------------------
void	SurfaceProcessing::ApplyFFDSurfaceDeformationLines(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
{
	if ((_Grid!=NULL)&&(Polys!=NULL))
	{
		FloatGrid3D GGrid=*_Grid;
		FloatGrid3D *Grid=&GGrid;
		double *colTab;
		double	Point[3];
		double	TPoint[3];
		double	Point1[3];
		vtkPoints *Pts=Polys->GetPoints();
		int numPts=Polys->GetNumberOfPoints();
		vtkDataArray *normals=Polys->GetPointData()->GetNormals();
		double	v[3];
		double	normal[3];
		int i;//,j;
		BSplineGridTransformation *BSplineTransform=new BSplineGridTransformation(Grid,nn[0],nn[1],nn[2],min[0],min[1],min[2]);
		if (palette!=NULL) colTab=new double[numPts*2];

		double def[3];
		double maxx=0;
		double minx=INT_MAX;

		vtkPoints *newPts=vtkPoints::New();


		int pos=0;
		int pos2=0;

		vtkPoints *xPoints=NULL;
		if (startPolys!=NULL)	xPoints=startPolys->GetPoints();

		for (i=0;i<numPts;i++)
		{
			Pts->GetPoint(i,TPoint);//pobieram kolejny pkt meshu
			if (enableAffineTransformation==false)
				BSplineTransform->TransformPoint(TPoint,Point,amount,def);
			else
				affineTransform->TransformPoint(TPoint,Point,amount,def);

			if (vcomponent!=0)
			{
				normals->GetTuple(i,normal);
				GetVectorComponents(TPoint,Point, normal,v,vcomponent);
				Point[0]=v[0];Point[1]=v[1];Point[2]=v[2];
			}

			if (startPolys==NULL)
			{
				newPts->InsertNextPoint(TPoint);
			}
			else
			{
			
				xPoints->GetPoint(i,Point1);
				xPoints->SetPoint(i,Point);

				//Point1[0]=startPoints[pos2];	startPoints[pos2++]=Point[0];
				//Point1[1]=startPoints[pos2];	startPoints[pos2++]=Point[1];
				//Point1[2]=startPoints[pos2];	startPoints[pos2++]=Point[2];
				
				
				newPts->InsertNextPoint(Point1);
			}

			newPts->InsertNextPoint(Point);	
			if (palette !=NULL)	
			{
				colTab[pos++]=sqrt(def[0]*def[0]+def[1]*def[1]+def[2]*def[2]);
				colTab[pos++]=colTab[pos-1];
			}
		}

		double tol=1e-5;
		vtkCellArray *elems = vtkCellArray::New();
		NewPolys->SetPoints(newPts);
		if (fabs(amount)>tol)
		{

			vtkIdType newCellPts[2];
			vtkIdType ID;

			for(i=0;i<newPts->GetNumberOfPoints();i+=2)
			{
				newCellPts[0] =i;	newCellPts[1] =i+1;
				ID=elems->InsertNextCell(2,newCellPts);
			}
			NewPolys->SetLines(elems);
		}
		else
		{
			vtkIdType newCellPts[1];
			vtkIdType ID;

			for(i=0;i<newPts->GetNumberOfPoints();i+=2)
			{
				newCellPts[0] =i;		ID=elems->InsertNextCell(1,newCellPts);
				newCellPts[0] =i+1;		ID=elems->InsertNextCell(1,newCellPts);
			}
			NewPolys->SetVerts(elems);
		}

		NewPolys->BuildCells();
		//NewPolys->Update();

		if (palette !=NULL)
		{
			//find min and max
			minx=0;

			if (max==0)
			{
				for (i=0;i<numPts;i++)	if (colTab[i]>maxx) 	maxx=colTab[i];
			}				
			else
				maxx=max;

			double div=maxx-minx;
			if (maxx-minx==0) div=255.0; else div=255.0/div;
			//scale tab

			for (i=0;i<numPts*2;i++)
			{
				colTab[i]=(colTab[i]-minx)*div;
				if (colTab[i]>255) colTab[i]=255.0;
			}

			SurfaceColorize *sCol=new SurfaceColorize();
			sCol->MapTabPaletteToMesh(NewPolys,colTab,palette);
			delete sCol;
			delete []colTab;
		}
		delete BSplineTransform;
		Grid=NULL;
		if (xPoints!=NULL) xPoints->Delete();
	}
}
//----------------------------------------
void	SurfaceProcessing::ApplyFFDSurfaceDeformationArrows(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
{
	
	if ((_Grid!=NULL)&&(Polys!=NULL))
	{
		FloatGrid3D GGrid=*_Grid;
		FloatGrid3D *Grid=&GGrid;
		double *colTab;


		vtkPoints *Pts=Polys->GetPoints();
		int numPts=Polys->GetNumberOfPoints();
		int i,j;

		vtkDataArray *normals=NULL;
		//normals=Polys->GetPointData()->GetNormals();
//---------------------------------------------------------------	
		vtkPoints *xPoints=NULL;
		if (startPolys!=NULL)
		{	
			xPoints=startPolys->GetPoints();
			if (vcomponent!=0)
				normals=startPolys->GetPointData()->GetNormals();
		}
		else
			normals=Polys->GetPointData()->GetNormals();
//------------------------------------------------------------	

		double	v[3];
		double	normal[3];

		BSplineGridTransformation *BSplineTransform=new BSplineGridTransformation(Grid,nn[0],nn[1],nn[2],min[0],min[1],min[2]);
		if (palette!=NULL) colTab=new double[numPts*6];
		double def[3];
		double maxx=0;
		double minx=INT_MAX;

		vtkPoints *newPts=vtkPoints::New();
		int pos=0;
		double dist;

		double radius;
		double pmp1[3],p2mp1[3],r[3],s[3],q[3],p[3],p1[3],p2[3];
		double Point1[3];
		double factor=0.5;
		double tol=1e-5; 
		double tmp;

		int pos2=0;
		for (i=0;i<numPts;i++)
		{
			Pts->GetPoint(i,p1);//pobieram kolejny pkt meshu
			
			if (enableAffineTransformation==false)
				BSplineTransform->TransformPoint(p1,p2,amount,def);
			else
				affineTransform->TransformPoint(p1,p2,amount,def);
			if(vcomponent!=0)
			{
				normals->GetTuple(i,normal);
				if (startPolys==NULL)
				{
					GetVectorComponents(p1,p2, normal,v,vcomponent);
				}
				else
				{	
					xPoints->GetPoint(i,p1);
					GetVectorComponents(p1,p2, normal,v,vcomponent);
					xPoints->SetPoint(i,p2);
				}
				p2[0]=v[0];p2[1]=v[1];p2[2]=v[2];
			}
			else
			{
				if (startPolys!=NULL)
				{
					xPoints->GetPoint(i,p1);
					xPoints->SetPoint(i,p2);
				}
			}
			newPts->InsertNextPoint(p1);	
			newPts->InsertNextPoint(p2);
			//--------------------------------------------------------



			//trzeba dodac 4 wlasciwe pkty
			//---------------------------------------------
			// pkt gdzie strzalka ma dol
			p[0]=p1[0]+factor*(p2[0]-p1[0]);
			p[1]=p1[1]+factor*(p2[1]-p1[1]);
			p[2]=p1[2]+factor*(p2[2]-p1[2]);
			//
			pmp1 [0]=p[0]- p1[0];	pmp1 [1]= p[1]-p1[1];	pmp1 [2]= p[2]-p1[2];
			p2mp1[0]=p2[0]-p1[0];	p2mp1[1]=p2[1]-p1[1];	p2mp1[2]=p2[2]-p1[2];
			//
			CROSS(r,pmp1,p2mp1);
			CROSS(s,r,p2mp1);
			// jeszcze je znormalizowac trzeba
			tmp=sqrt(DOT(r,r));
			if (tmp>0){tmp=1.0/tmp; r[0]*=tmp;r[1]*=tmp;r[2]*=tmp;};
			tmp=sqrt(DOT(s,s));
			if (tmp>0){tmp=1.0/tmp; s[0]*=tmp;s[1]*=tmp;s[2]*=tmp;};

			//------------------------------

			radius=0.05*sqrt(p2mp1[0]*p2mp1[0]+p2mp1[1]*p2mp1[1]+p2mp1[2]*p2mp1[2]);//??
			if (radius<tol)
				radius=0.01;

			CalculateArrowPoint(q, p,r,s, radius, 0);	newPts->InsertNextPoint(q);
			CalculateArrowPoint(q, p,r,s, 0, radius);	newPts->InsertNextPoint(q);
			CalculateArrowPoint(q, p,r,s, -radius, 0);	newPts->InsertNextPoint(q);
			CalculateArrowPoint(q, p,r,s, 0, -radius);	newPts->InsertNextPoint(q);

			//-------------------------------------------
			if (palette !=NULL)	
			{
				colTab[pos++]=dist=sqrt(def[0]*def[0]+def[1]*def[1]+def[2]*def[2]);
				colTab[pos++]=dist;
				// to te 4 gratisy			
				colTab[pos++]=dist;
				colTab[pos++]=dist;
				colTab[pos++]=dist;
				colTab[pos++]=dist;
				//trzeba pokolorowac 4 pkty tak jak poprzednie
			}
		}

		if ((startPolys!=NULL)&&(vcomponent!=0))
		{
			// przelicz normalne w startpolys
			//SlotComputeNormals(startPolys);
			SlotRecomputeNormals(startPolys);
		}
		//Polys->SetPoints(Pts);

		vtkCellArray *elems = vtkCellArray::New();

		NewPolys->SetPoints(newPts);
		if (fabs(amount)>tol)
		{

			vtkIdType newCellPts[2];
			vtkIdType ID;

			for(i=0;i<newPts->GetNumberOfPoints();i+=6)//bo 4 dodalem
			{
				newCellPts[0] =i;
				newCellPts[1] =i+1;
				ID=elems->InsertNextCell(2,newCellPts);
			}
			NewPolys->SetLines(elems);

			//trojkaty do strzealek
			//------------------------
			vtkCellArray *arrowTriangles= vtkCellArray::New();
			vtkIdType newCellPts2[3];

			for(i=1;i<newPts->GetNumberOfPoints();i+=6)//bo 4 dodalem
			{
				/*
				// wrsja na 2 trojkaty
				newCellPts2[0] =i;		newCellPts2[1] =i+1;	newCellPts2[2] =i+3;	ID=arrowTriangles->InsertNextCell(3,newCellPts2);
				newCellPts2[0] =i;		newCellPts2[1] =i+2;	newCellPts2[2] =i+4;	ID=arrowTriangles->InsertNextCell(3,newCellPts2);
				*/
				//werska na 4 trojkaty
				newCellPts2[0] =i;		newCellPts2[1] =i+1;	newCellPts2[2] =i+2;	ID=arrowTriangles->InsertNextCell(3,newCellPts2);
				newCellPts2[0] =i;		newCellPts2[1] =i+2;	newCellPts2[2] =i+3;	ID=arrowTriangles->InsertNextCell(3,newCellPts2);
				newCellPts2[0] =i;		newCellPts2[1] =i+3;	newCellPts2[2] =i+4;	ID=arrowTriangles->InsertNextCell(3,newCellPts2);
				newCellPts2[0] =i;		newCellPts2[1] =i+4;	newCellPts2[2] =i+1;	ID=arrowTriangles->InsertNextCell(3,newCellPts2);
			}

			NewPolys->SetPolys(arrowTriangles);
			//------------------------
		}
		else
		{
			vtkIdType newCellPts[1];
			vtkIdType ID;

			for(i=0;i<newPts->GetNumberOfPoints();i+=2)
			{
				newCellPts[0] =i;
				ID=elems->InsertNextCell(1,newCellPts);
				newCellPts[0] =i+1;
				ID=elems->InsertNextCell(1,newCellPts);
			}
			NewPolys->SetVerts(elems);
		}
		//trzeba dodac po 2 trojkaty na 1 kreche

		NewPolys->BuildCells();
		//NewPolys->Update();

		if (palette !=NULL)
		{
			//find min and max
			minx=0;
			if (max==0)
			{
				for (i=0;i<numPts;i++)	if (colTab[i]>maxx) 	maxx=colTab[i];
			}				
			else
				maxx=max;

			double div=maxx-minx;
			if (maxx-minx==0) div=255.0; else div=255.0/div;
			//scale tab

			for (i=0;i<newPts->GetNumberOfPoints();i++)
			{			
				colTab[i]=(colTab[i]-minx)*div;
				if (colTab[i]>255) colTab[i]=255.0;
			}
			//

			SurfaceColorize *sCol=new SurfaceColorize();
			sCol->MapTabPaletteToMesh(NewPolys,colTab,palette);
			delete sCol;
			delete []colTab;
		}

		delete BSplineTransform;
		Grid=NULL;

		//if (xPoints!=NULL) xPoints->Delete();
	//	if ((vcomponent!=0)&& (startPoints!=NULL)) normals->Delete();
	}

}
//----------------------------------------
//----------------------------------------
void SurfaceProcessing::ApplyFFDDeformation(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
{
	if ((_Grid!=NULL)&&(Polys!=NULL))
	{
		FloatGrid3D GGrid=*_Grid;
		FloatGrid3D *Grid=&GGrid;
		double *colTab=NULL;
		double	Point[3],Point1[3];
		double	TPoint[3];
		double	Point2[3];

		//vtkDataArray *normals=Polys->GetPointData()->GetNormals();
		double	n[3];
		double	v[3];
		double	normal[3];

	
		vtkDataArray *normals=NULL;
		normals=Polys->GetPointData()->GetNormals();
		
		vtkPoints *xPoints=NULL;
		if (startPolys!=NULL)
		{	
			xPoints=startPolys->GetPoints();
			if (vcomponent!=0)
				normals=startPolys->GetPointData()->GetNormals();
		}
		else
			normals=Polys->GetPointData()->GetNormals();

	


		vtkPoints *Pts=Polys->GetPoints();
		int numPts=Polys->GetNumberOfPoints();
		int i;//,j;

		BSplineGridTransformation *BSplineTransform=new BSplineGridTransformation(Grid,nn[0],nn[1],nn[2],min[0],min[1],min[2]);
		if (palette!=NULL) colTab=new double[numPts];
		// deformations
		double def[3];
		double dist;
		double maxx=0;
		double minx=INT_MAX;
		int pos2=0;
		/*if ((amount >=1.0)||(amount<0))
			for (i=0;i<numPts;i++)
			{
				Pts->GetPoint(i,TPoint);//pobieram kolejny pkt meshu
				BSplineTransform->TransformPoint(TPoint,Point,def);
				Pts->SetPoint(i,Point);
				if (vcomponent==0)
				{
					if (palette !=NULL)	
						colTab[i]=dist=sqrt(def[0]*def[0]+def[1]*def[1]+def[2]*def[2]);
				}
				else
				{
					//normals->GetTuple(i,normal);
					//GetVectorComponents(TPoint,Point, normal,v,vcomponent);
					//Pts->SetPoint(i,v);
					normals->GetTuple(i,normal);
					GetVectorComponents(TPoint,Point, normal,v,vcomponent);
					
					if (palette !=NULL)	
					{
						Point2[0]=v[0]-TPoint[0];
						Point2[1]=v[1]-TPoint[1];
						Point2[2]=v[2]-TPoint[2];
						colTab[i]=dist=sqrt(Point2[0]*Point2[0]+Point2[1]*Point2[1]+Point2[2]*Point2[2]);
					}
				}
				
			}
		else*/
			for (i=0;i<numPts;i++)
			{
				Pts->GetPoint(i,TPoint);//pobieram kolejny pkt meshu
				//BSplineTransform->TransformPoint(TPoint,Point,amount,def);
				
				if (enableAffineTransformation==false)
					BSplineTransform->TransformPoint(TPoint,Point,amount,def);
				else
				{
		//			TPoint[0]=12;
		//			TPoint[1]=0;
		//			TPoint[2]=0;
					affineTransform->TransformPoint(TPoint,Point,amount,def);
				}
				Pts->SetPoint(i,Point);

				//if (palette !=NULL)	
				{
					//--------------------------------------------------
					//--------------------------------------------------
					//Pts->GetPoint(i,p1);//pobieram kolejny pkt meshu
					//BSplineTransform->TransformPoint(p1,p2,amount,def);
					if(vcomponent!=0)
					{
						normals->GetTuple(i,normal);
						if (startPolys==NULL)
						{
							GetVectorComponents(TPoint,Point, normal,v,vcomponent);
							Point2[0]=v[0]-TPoint[0];
							Point2[1]=v[1]-TPoint[1];
							Point2[2]=v[2]-TPoint[2];
						}
						else
						{	
							xPoints->GetPoint(i,TPoint);
							GetVectorComponents(TPoint,Point, normal,v,vcomponent);
							Point2[0]=v[0]-TPoint[0];
							Point2[1]=v[1]-TPoint[1];
							Point2[2]=v[2]-TPoint[2];
							xPoints->SetPoint(i,Point);
							
						}
						Pts->SetPoint(i,v);// to po to aby powierzchnia przedstawiala tylko 1 skladowa przemieszczen
					
					}
					else
					{
						//Pts->SetPoint(i,Point);//chowam na chwile
						if (startPolys!=NULL)
						{
							xPoints->GetPoint(i,TPoint);
							Point2[0]=Point[0]-TPoint[0];
							Point2[1]=Point[1]-TPoint[1];
							Point2[2]=Point[2]-TPoint[2];
							xPoints->SetPoint(i,Point);
						}
						else
						{
							Point2[0]=def[0];
							Point2[1]=def[1];
							Point2[2]=def[2];
						}
			}
			if (palette !=NULL)	 colTab[i]=dist=sqrt(Point2[0]*Point2[0]+Point2[1]*Point2[1]+Point2[2]*Point2[2]);
			//--------------------------------------------------
			//--------------------------------------------------


					/*if (startPolys==NULL)
					{
						if (vcomponent!=0)
						{
							normals->GetTuple(i,normal);
							GetVectorComponents(TPoint,Point, normal,v,vcomponent);
							Point2[0]=v[0]-TPoint[0];
							Point2[1]=v[1]-TPoint[1];
							Point2[2]=v[2]-TPoint[2];
							colTab[i]=dist=sqrt(Point2[0]*Point2[0]+Point2[1]*Point2[1]+Point2[2]*Point2[2]);

						}
						else
							colTab[i]=dist=sqrt(def[0]*def[0]+def[1]*def[1]+def[2]*def[2]);
					}
					else
					{
										
						xPoints->GetPoint(i,Point1);
						Point2[0]=Point1[0]-Point[0];
						Point2[1]=Point1[1]-Point[1];
						Point2[2]=Point1[2]-Point[2];

						xPoints->SetPoint(i,Point);
					

						colTab[i]=dist=sqrt(Point2[0]*Point2[0]+Point2[1]*Point2[1]+Point2[2]*Point2[2]);
					}*/
				}
			}
/*
					for(j=0;j<numPts;j++)
					{
						Pts->GetPoint(j,Point);
						BSplineTransform->TransformPoint(Point,Point1,def);

						Point2[0]=startPoints[pos2]-Point1[0];	startPoints[pos2++]=Point1[0];
						Point2[1]=startPoints[pos2]-Point1[1];	startPoints[pos2++]=Point1[1];
						Point2[2]=startPoints[pos2]-Point1[2];	startPoints[pos2++]=Point1[2];

						dist=sqrt(Point2[0]*Point2[0]+Point2[1]*Point2[1]+Point2[2]*Point2[2]);
						if (dist>maxDist) maxDist=dist;

					}
*/

			Polys->SetPoints(Pts);
			if ((startPolys!=NULL)&&(vcomponent!=0))
			{
				// przelicz normalne w startpolys
//				SlotComputeNormals(startPolys);
				SlotRecomputeNormals(startPolys);
			}
			if (palette !=NULL)
			{

				//find min and max
				minx=0;
				if (max==0)
				{
					for (i=0;i<numPts;i++)	if (colTab[i]>maxx) 	maxx=colTab[i];
				}				
				else
					maxx=max;

				double div=maxx-minx;
				if (maxx-minx==0) div=255.0; else div=255.0/div;
				//scale tab

				for (i=0;i<numPts;i++)
				{
					colTab[i]=(colTab[i]-minx)*div;
					if (colTab[i]>255) colTab[i]=255.0;
				}
				//

				SurfaceColorize *sCol=new SurfaceColorize();

				sCol->MapTabPaletteToMesh(Polys,colTab,palette);
				delete sCol;
				delete []colTab;
			}

			delete BSplineTransform;
			Grid=NULL;
//			if (xPoints!=NULL) xPoints->Delete();


	}
}
//---------------------------------------
void SurfaceProcessing::SlotApplyFFDDeformation(FloatGrid3D *_Grid,double amount,PaletteRGBA *palette,double max)
{
	if ((ObjectSurfaceData!=NULL)&&(_Grid!=NULL))
	{

		double min[3],nn[3];
		CalculateMinAndSizeOfPolys(ObjectSurfaceData->GetVPolyData(),ObjectSurfaceData->GetPN(),ObjectSurfaceData->GetPScale(),min, nn);

		ApplyFFDDeformation(_Grid,ObjectSurfaceData->GetVPolyData(),amount,min,nn,palette,max);
		ObjectSurfaceData->RebuildSurface();
		ObjectSurfaceData->GetPolygonSurfaceName()+="-ffd";
		VWidget->RepaintWidget();
	}

}
//----------------------------------------------
void SurfaceProcessing::ApplyFFDDeformation1(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
{
	NewPolys->DeepCopy(Polys);

	//	double min[3],nn[3];
	//	CalculateMinAndSizeOfPolys(NewPolys,N,S,min, nn);
	ApplyFFDDeformation(_Grid,NewPolys,amount,min,nn,palette,max,startPolys,vcomponent);

	//ApplyFFDDeformation(_Grid,NewPolys,amount,N,S,palette);
}
//--------------------------------------------------
//PolygonSurfaceData *SurfaceProcessing::ApplyFFDDeformation1b(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
//{
//	NewPolys->DeepCopy(Polys);
//
//	//	double min[3],nn[3];
//	//	CalculateMinAndSizeOfPolys(NewPolys,N,S,min, nn);
//	return ApplyFFDDeformation2(_Grid,NewPolys,amount,min,nn,palette,max,startPolys,vcomponent);
//
//	//ApplyFFDDeformation(_Grid,NewPolys,amount,N,S,palette);
//}
//-----------------------------------------------
//-----------------------------------------------
void SurfaceProcessing::ApplyGridBasedProcessing(void (SurfaceProcessing::*funcs)(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent),
												 FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent, double maxDisp)
{
	//	int max=0;

	if ((Polys!=NULL)&(_Grid!=NULL))
	{
		vtkPolyData *NewPolys=vtkPolyData::New();
		//NewPolys->DeepCopy(Polys);
		//vtkPoints *p=startPolys->GetPoints();


		(this->*funcs)(_Grid,Polys,NewPolys,amount,min,nn,palette,max,startPolys,vcomponent);
		// policzyc normalne i wymienic 
		if (NewPolys->GetNumberOfPolys()>0) 
			SlotComputeNormals(	NewPolys);
		//------------------------------------
		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(NewPolys,VWidget);

		if (name=="")
			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("def_surface");
		else
			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
 
		//	Output1PolygonSurfaceData->RebuildSurface();
		vtkDataArray *TempData=NewPolys->GetPointData()->GetScalars();
		vtkMapper *VMapper=Output1PolygonSurfaceData->GetVMapper();
		if (TempData!=NULL)
			if ((TempData->GetRange()!=NULL)&&(TempData->GetLookupTable()!=NULL))
			{
				double *nr1;	nr1=TempData->GetRange();
				if (nr1!=0)	{VMapper->ScalarVisibilityOn();	VMapper->SetScalarRange(nr1[0],nr1[1]);	}
				else VMapper->ScalarVisibilityOff();
				nr1=NULL;
			}

			emit	SignalSendNewSurface(Output1PolygonSurfaceData);

			//VWidget->RepaintWidget();


	}

}
//-------------------------------------------------
PolygonSurfaceData * SurfaceProcessing::ApplyGridBasedProcessing2(void(SurfaceProcessing::*funcs)(FloatGrid3D *_Grid,vtkPolyData *Polys,vtkPolyData *NewPolys,double amount,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent),
												 FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent, double maxDisp)
{
	//	int max=0;

	if ((Polys!=NULL)&(_Grid!=NULL))
	{
		vtkPolyData *NewPolys=vtkPolyData::New();
		//NewPolys->DeepCopy(Polys);
		//vtkPoints *p=startPolys->GetPoints();


		(this->*funcs)(_Grid,Polys,NewPolys,amount,min,nn,palette,max,startPolys,vcomponent);
		// policzyc normalne i wymienic 
		if (NewPolys->GetNumberOfPolys()>0) 
			SlotComputeNormals(	NewPolys);
		//------------------------------------
		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(NewPolys,VWidget);

		if (name=="")
			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("def_surface");
		else
			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
 
		//	Output1PolygonSurfaceData->RebuildSurface();
		vtkDataArray *TempData=NewPolys->GetPointData()->GetScalars();
		vtkMapper *VMapper=Output1PolygonSurfaceData->GetVMapper();
		if (TempData!=NULL)
			if ((TempData->GetRange()!=NULL)&&(TempData->GetLookupTable()!=NULL))
			{
				double *nr1;	nr1=TempData->GetRange();
				if (nr1!=0)	{VMapper->ScalarVisibilityOn();	VMapper->SetScalarRange(nr1[0],nr1[1]);	}
				else VMapper->ScalarVisibilityOff();
				nr1=NULL;
			}

			//emit	SignalSendNewSurface(Output1PolygonSurfaceData);
			return Output1PolygonSurfaceData;
			//VWidget->RepaintWidget();


	}
  return NULL;

}
//----------------------------------------------
PolygonSurfaceData * SurfaceProcessing::SlotApplyFFDDeformation2(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
{
	return ApplyGridBasedProcessing2(&SurfaceProcessing::ApplyFFDDeformation1,_Grid,Polys,amount,name,min,nn,palette,max,startPolys,vcomponent);
}

void SurfaceProcessing::SlotApplyFFDDeformation(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
{
	ApplyGridBasedProcessing(&SurfaceProcessing::ApplyFFDDeformation1,_Grid,Polys,amount,name,min,nn,palette,max,startPolys,vcomponent);
}
//-----------------------------------------------------------------
void SurfaceProcessing::SlotApplyFFDSurfaceDeformationArrows(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
{

	ApplyGridBasedProcessing(&SurfaceProcessing::ApplyFFDSurfaceDeformationArrows,_Grid,Polys,amount,name,min,nn,palette,max,startPolys,vcomponent);
}
//-----------------------------------------------------
void SurfaceProcessing::SlotApplyFFDSurfaceDeformationLines(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent)
{
	ApplyGridBasedProcessing(&SurfaceProcessing::ApplyFFDSurfaceDeformationLines,_Grid,Polys,amount,name,min,nn,palette,max,startPolys,vcomponent);
}
//--------------------------------------------------
void SurfaceProcessing::SlotChlebiejDeformation1(double Factor)
{


	if ((ObjectSurfaceData!=NULL))//&&(CurrentCuttingToolActor!=NULL))
	{


		double	Point[3];
		//		double	Point2[3];
		double	TPoint[3];
		//		double	DeltaPoint[3];
		//		double	Length;

		//CurrentCuttingToolActor->GetPosition(Point);

		vtkPoints *Pts=ObjectSurfaceData->GetVPolyData()->GetPoints();

		int numPts=ObjectSurfaceData->GetVPolyData()->GetNumberOfPoints();
		int i;//,j;


		double bounds[6];

		ObjectSurfaceData->GetVPolyData()->GetBounds(bounds);
		double nnx=fabs(bounds[1]-bounds[0]);
		double nny=fabs(bounds[3]-bounds[2]);
		double nnz=fabs(bounds[5]-bounds[4]);

		int GridNx=10;
		int GridNy=10;
		int GridNz=10;
		int x,y,z;
		FloatGrid3D *Grid=new FloatGrid3D(GridNx,GridNy,GridNz);		
		BSplineGridTransformation *BSplineTransform=new BSplineGridTransformation(Grid,nnx,nny,nnz,bounds[0],bounds[2],bounds[4]);

		// deformations

		srand( (unsigned)time( NULL ) );
		double gg=1.0/Factor;
		double a,b,c;
		for(z=0;z<GridNz;z++)
			for(y=0;y<GridNy;y++)
				for(x=0;x<GridNx;x++)
				{
					a=(-nnx/20+(double)rand()*(nnx/10)/(double)RAND_MAX)*gg;
					b=(-nny/20+(double)rand()*(nny/10)/(double)RAND_MAX)*gg;
					c=(-nnz/20+(double)rand()*(nnz/10)/(double)RAND_MAX)*gg;

					Grid->SetDx(x,y,z,a);
					Grid->SetDy(x,y,z,b);
					Grid->SetDz(x,y,z,c);


				}


				for (i=0;i<numPts;i++)
				{

					Pts->GetPoint(i,TPoint);//pobieram kolejny pkt meshu
				//	BSplineTransform->TransformPoint(TPoint,Point);
					if (enableAffineTransformation==false)
						BSplineTransform->TransformPoint(TPoint,Point);
					else
						affineTransform->TransformPoint(TPoint,Point);
					Pts->SetPoint(i,Point);
				}

				delete BSplineTransform;
				delete Grid;




				/*
				for (i=0;i<numPts;i++)
				{

				Pts->GetPoint(i,TPoint);//pobieram kolejny pkt meshu
				for(j=0;j<3;j++)	//obliczam wektorek laczacy 2 punkty
				Point2[j]=TPoint[j]-Point[j];

				Length=sqrt(Point2[0]*Point2[0]+Point2[1]*Point2[1]+Point2[2]*Point2[2]);
				*/
				/*if (Length<Factor) //deformacja kula
				{
				for(j=0;j<3;j++)
				{
				DeltaPoint[j]=Point2[j]/Length;
				//	TPoint[j]+=DeltaPoint[j]*Factor;
				TPoint[j]+=DeltaPoint[j]*(Factor-Length);
				}
				Pts->SetPoint(i,TPoint);
				}
				*/
				/*			for(j=0;j<3;j++)
				{
				DeltaPoint[j]=Point2[j]/Length;
				TPoint[j]+=DeltaPoint[j]*Factor;
				}


				Pts->SetPoint(i,TPoint);
				}
				*/




				ObjectSurfaceData->GetVPolyData()->SetPoints(Pts);

				ObjectSurfaceData->RebuildSurface();
				VWidget->RepaintWidget();



	}

}
//-------------------------------------------------------

void	SurfaceProcessing::ReplaceCurrentSurface(vtkPolyData *Input, bool GeomOrig, bool AppendName,const QString &text)
{


	PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(Input,VWidget);

	int i;
	for(i=0;i<3;i++)
	{
		Output1PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
		Output1PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];
	}

	Output1PolygonSurfaceData->GetVProperty()->DeepCopy(ObjectSurfaceData->GetVProperty());
	ObjectSurfaceData->SetHideWithoutRefresh(true);


	//CENTER
	if (GeomOrig)
		Output1PolygonSurfaceData->ComputeGeometricOrigin();
	else
		Output1PolygonSurfaceData->SetCenterOrigin();


	//NAME
	QString name;
	if (AppendName)
	{
		name=ObjectSurfaceData->GetPolygonSurfaceName();
		name.append(text);
	}
	else
	{
		name=text;
	}
	Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);



	emit	SignalDeleteCurrentSurface();
	emit	SignalSendNewSurfaceAfterDelete(Output1PolygonSurfaceData);



}


//-------------------------------------------------------
void	SurfaceProcessing::SlotTransformAffineSurface(double *trans,bool inv)
{
	if (ObjectSurfaceData!=NULL)
	{
		
		
		double rotateCenter[3];
		rotateCenter[0]=trans[14];
		rotateCenter[1]=trans[15];
		rotateCenter[2]=trans[16];
		double matrix[16];
		Transform *transX=new Transform(rotateCenter,1,1,1,false);
		if (inv==false)
			transX->MakeCompleteAffineMatrix(matrix,trans);
		else
			transX->MakeCompleteAffineMatrixInv(matrix,trans);

		double Point[3];
		double Point1[3];
		double PointF[3];
	
		int i;
		vtkPoints *pts=vtkPoints::New();
		vtkPoints *origPoints=ObjectSurfaceData->GetVPolyData()->GetPoints();
		pts->DeepCopy(origPoints);

		int nrOfPts=pts->GetNumberOfPoints();
		for(i=0;i<nrOfPts;i++)
		{
			origPoints->GetPoint(i,Point);

			
			transX->MultiplyPoint(Point1,Point);

			pts->SetPoint(i,Point1);

		}
		delete transX;

		ObjectSurfaceData->GetVPolyData()->SetPoints(pts);

		ObjectSurfaceData->RebuildSurface();
		VWidget->RepaintWidget();


			//PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(Clean1->GetOutput(),VWidget);

			////--------------------------
			//for(j=0;j<3;j++)
			//{
			//	Output1PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
			//	Output1PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];
			//}
			//Output1PolygonSurfaceData->GetVProperty()->DeepCopy(ObjectSurfaceData->GetVProperty());
			//ObjectSurfaceData->SetHideWithoutRefresh(true);
			//Output1PolygonSurfaceData->ComputeGeometricOrigin();
			//Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
			//Output1PolygonSurfaceData->SetGroupName("SphereCuts");

	}
}
//-----------------------------------------------------------------
void	SurfaceProcessing::SlotSmoothSurface(double Factor, int IterNumber)
{

	if (ObjectSurfaceData!=NULL)
	{

		/*
			vtkWindowedSincPolyDataFilter*SmoothFilter  =vtkWindowedSincPolyDataFilter::New();
		SmoothFilter->SetInputData(ObjectSurfaceData->GetVPolyData());
		SmoothFilter->SetNumberOfIterations( IterNumber );
		SmoothFilter->SetPassBand(Factor);
		*/

		vtkSmoothPolyDataFilter *SmoothFilter  = vtkSmoothPolyDataFilter::New();
		SmoothFilter->SetInputData(ObjectSurfaceData->GetVPolyData());
		SmoothFilter->SetNumberOfIterations( IterNumber );
		SmoothFilter->SetRelaxationFactor( Factor );
		SmoothFilter->SetFeatureAngle( 60.0 );
		SmoothFilter->FeatureEdgeSmoothingOff();
		SmoothFilter->BoundarySmoothingOff();
		SmoothFilter->SetConvergence( 0 );


		//vtkPolyDataNormals *Normals=vtkPolyDataNormals::New();

		//Normals->SetInputData(SmoothFilter->GetOutput());


		//vtkPolyData *output=Normals->GetOutput();
		////output->Update();


		SmoothFilter->Update();
		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(SmoothFilter->GetOutput());
//		//output->Update();	
		SmoothFilter->Delete();

		//	vtkPolyData *output=SmoothFilter->GetOutput();
		//	//output->Update();

		ReplaceCurrentSurface(output,true, true,"-sm");

	}

}

//-------------------------------------------------------
void	SurfaceProcessing::SlotExtractSeparatedSurfaces()
{
	//daj opacity zero dla aktualnej i walnij wszystkie wyniki dla connectivity filter i wyslicj
	if(ObjectSurfaceData!=NULL)
	{

		double opacity=ObjectSurfaceData->GetVProperty()->GetOpacity();
		ObjectSurfaceData->GetVProperty()->SetOpacity(0);
		vtkSmartPointer<vtkPolyDataConnectivityFilter> filter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();	
			//vtkPolyDataConnectivityFilter *VtkConnectivityFilter = vtkPolyDataConnectivityFilter::New();	
		filter->SetInputData( ObjectSurfaceData->GetVPolyData() );
		filter->SetExtractionModeToSpecifiedRegions();
		filter->Update();
		int nr=filter->GetNumberOfExtractedRegions();
		int i;
		PolygonSurfaceData *o2;
		QString name=ObjectSurfaceData->GetPolygonSurfaceName()+"_";
		vtkProperty *prop=ObjectSurfaceData->GetVProperty();
		vtkPolyData *pp;
		filter->AddSpecifiedRegion(0);				
		filter->Update();
		int nrOfRegions=filter->GetNumberOfExtractedRegions();
						
		for(i=0;i<nrOfRegions;i++)
		{
			if (i>0) filter->DeleteSpecifiedRegion(i-1);
			filter->AddSpecifiedRegion(i);
			filter->Update();
		
			pp=vtkPolyData::New();	pp->DeepCopy(filter->GetOutput());	/*pp->Update();*/
			if (pp!=NULL)
			{
				o2=new PolygonSurfaceData(pp,VWidget);	o2->SetPolygonSurfaceDataSetName(name+QString::number(i));	o2->GetVActor()->GetProperty()->DeepCopy(prop);;	o2->ComputeGeometricOrigin();
				o2->GetVProperty()->SetOpacity(opacity);
				emit	SignalSendNewSurface(o2);
			}
		}
		//VtkConnectivityFilter->Delete();

	}
}
//-------------------------------------------
void	SurfaceProcessing::SlotClosestToPointerSurfaceOnly()
{

	if ((ObjectSurfaceData!=NULL)&&(CuttingToolVisible))
	{
		double	Point[3];
		CurrentCuttingToolActor->GetPosition(Point);



		vtkPolyDataConnectivityFilter *VtkConnectivityFilter = vtkPolyDataConnectivityFilter::New();	
		VtkConnectivityFilter->SetInputData( ObjectSurfaceData->GetVPolyData() );
		VtkConnectivityFilter->SetExtractionModeToClosestPointRegion () ;
		VtkConnectivityFilter->SetClosestPoint(Point[0],Point[1],Point[2]);	


		VtkConnectivityFilter->Update();
		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(VtkConnectivityFilter->GetOutput());
//		//output->Update();	
		VtkConnectivityFilter->Delete();

		//	vtkPolyData *output=VtkConnectivityFilter->GetOutput();
		//	//output->Update();
		ReplaceCurrentSurface(output,true, true,"-l");


	}






}

//-------------------------------------------------------

void	SurfaceProcessing::SlotLargestSurfaceOnly()
{

	if (ObjectSurfaceData!=NULL)
	{


		vtkPolyDataConnectivityFilter *VtkConnectivityFilter = vtkPolyDataConnectivityFilter::New();	
		VtkConnectivityFilter->SetInputData( ObjectSurfaceData->GetVPolyData() );



		VtkConnectivityFilter->SetExtractionModeToLargestRegion();

		VtkConnectivityFilter->Update();

		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(VtkConnectivityFilter->GetOutput());
//		//output->Update();	
		VtkConnectivityFilter->Delete();

		//vtkPolyData *output=VtkConnectivityFilter->GetOutput();
		////output->Update();



		ReplaceCurrentSurface(output,true, true,"-l");


	}
}

//---------------------------------------------------------
void	SurfaceProcessing::SlotSmoothAndDecimSurfcace(double Factor, int IterNumber,double Value,double Error)
{
	if (ObjectSurfaceData!=NULL)
	{




		vtkSmoothPolyDataFilter *SmoothFilter  = vtkSmoothPolyDataFilter::New();
		SmoothFilter->SetInputData(ObjectSurfaceData->GetVPolyData());
		SmoothFilter->SetNumberOfIterations( IterNumber );
		SmoothFilter->SetRelaxationFactor( Factor );
		SmoothFilter->SetFeatureAngle( 60.0 );
		SmoothFilter->FeatureEdgeSmoothingOff();
		SmoothFilter->BoundarySmoothingOff();
		SmoothFilter->SetConvergence( 0 );
		SmoothFilter->Update();	



		vtkDecimatePro *decimate=vtkDecimatePro::New();
		decimate->SetInputData(SmoothFilter->GetOutput());
		decimate->SetTargetReduction(Value);
		decimate->PreserveTopologyOn();	
		decimate->SetMaximumError(Error);
		decimate->SplittingOff(); //pro
		decimate->SetFeatureAngle  (60.0); //pro
		decimate->Update();

		vtkCleanPolyData *Clean2 = vtkCleanPolyData::New();
		Clean2->SetInputData(decimate->GetOutput());
		Clean2->PointMergingOn ();
		Clean2->ConvertStripsToPolysOn ();
		Clean2->ConvertLinesToPointsOn ();
		Clean2->ConvertPolysToLinesOff ();
		Clean2->SetTolerance(1e-5);
		Clean2->Update();

		vtkSmoothPolyDataFilter *SmoothFilter2  = vtkSmoothPolyDataFilter::New();
		SmoothFilter2->SetInputData(Clean2->GetOutput());
		SmoothFilter2->SetNumberOfIterations( IterNumber );
		SmoothFilter2->SetRelaxationFactor( Factor );
		SmoothFilter2->SetFeatureAngle( 60.0 );
		SmoothFilter2->FeatureEdgeSmoothingOff();
		SmoothFilter2->BoundarySmoothingOff();
		SmoothFilter2->SetConvergence( 0 );
		SmoothFilter2->Update();	


		vtkPolyDataNormals *Normals=vtkPolyDataNormals::New();
		Normals->SetInputData(SmoothFilter2->GetOutput());
		Normals->Update();

		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(Normals->GetOutput());
//		//output->Update();

		Normals->Delete();
		SmoothFilter2->Delete();
		Clean2->Delete();
		decimate->Delete();
		SmoothFilter->Delete();

		ReplaceCurrentSurface(output,true, true,"-dc");


	}
}
//--------------------------------------------------------

void	SurfaceProcessing::SlotDecimateSurface(double Value,double Error)
{

	if (ObjectSurfaceData!=NULL)
	{


		/*
		vtkQuadricClustering *decimate=vtkQuadricClustering::New();
		decimate->SetInputData(ObjectSurfaceData->GetVPolyData());
		decimate->UseInputPointsOn ();
		decimate->SetNumberOfXDivisions(128);
		decimate->SetNumberOfYDivisions(128);
		decimate->SetNumberOfZDivisions(128);
		*/

		vtkDecimatePro *decimate=vtkDecimatePro::New();
		decimate->SetInputData(ObjectSurfaceData->GetVPolyData());
		decimate->SetTargetReduction(Value);
		decimate->PreserveTopologyOn();	
		decimate->SetMaximumError(Error);
		decimate->SplittingOff(); //pro
		decimate->SetFeatureAngle  (60.0); //pro
		decimate->Update();

		/*
		vtkDecimate *decimate=vtkDecimate::New();
		decimate->SetInputData(ObjectSurfaceData->GetVPolyData());
		decimate->PreserveEdgesOff(); //normal
		decimate->SetInitialFeatureAngle  (55.0);//normal
		decimate->SetMaximumFeatureAngle  (65.0);//normal
		decimate->SetMaximumIterations   (300);//normal
		decimate->SetInitialError (0.001);//normal
		decimate->SetErrorIncrement(0.005);//normal
		decimate->SetTargetReduction(Value);

		*/

		vtkCleanPolyData *Clean2 = vtkCleanPolyData::New();
		Clean2->SetInputData(decimate->GetOutput());
		Clean2->PointMergingOn ();
		Clean2->ConvertStripsToPolysOn ();
		Clean2->ConvertLinesToPointsOn ();
		Clean2->ConvertPolysToLinesOff ();
		Clean2->SetTolerance(1e-5);
		Clean2->Update();

		//vtkPolyDataNormals *Normals=vtkPolyDataNormals::New();
		//Normals->SetInputData(decimate->GetOutput());
		//vtkPolyData *output=Normals->GetOutput();
		////output->Update();


		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(Clean2->GetOutput());
//		//output->Update();	
		Clean2->Delete();
		decimate->Delete();

		//vtkPolyData *output=Clean2->GetOutput();
		////output->Update();

		ReplaceCurrentSurface(output,true, true,"-dc");

	}
}
//---------------------------------------------------------

void	SurfaceProcessing::SlotDepthSortSurface()
{

	if (ObjectSurfaceData!=NULL)
	{

		vtkDepthSortPolyData *DepthSort = vtkDepthSortPolyData::New();
		DepthSort->SetInputData(ObjectSurfaceData->GetVPolyData());
		DepthSort->SetDirectionToBackToFront();
		DepthSort->SetCamera(VWidget->Renderer->GetActiveCamera());
		DepthSort->SortScalarsOn();
		DepthSort->Update();

		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(DepthSort->GetOutput());
//		//output->Update();	
		DepthSort->Delete();

		/*	vtkDataArray *TempData=ObjectSurfaceData->GetVPolyData()->GetPointData()->GetScalars();
		if (TempData!=NULL)
		{
		double *nr1=new double[2];
		nr1=TempData->GetRange();
		if (nr1!=NULL)
		{
		vtkIntArray *scalars1 = vtkIntArray::New();
		vtkLookupTable *LUT = vtkLookupTable::New();
		scalars1->DeepCopy(ObjectSurfaceData->GetVPolyData()->GetPointData()->GetScalars());
		LUT->DeepCopy(ObjectSurfaceData->GetVPolyData()->GetPointData()->GetScalars()->GetLookupTable());
		scalars1->SetLookupTable(LUT);	
		output->GetPointData()->SetScalars(scalars1);
		}
		}
		*/	
		vtkDataArray *TempData=ObjectSurfaceData->GetVPolyData()->GetPointData()->GetScalars();
		if (TempData!=NULL)
		{
			if ((TempData->GetRange()!=NULL)&&(TempData->GetLookupTable()!=NULL))
			{
				vtkLookupTable *LUT =vtkLookupTable::New();
				LUT->DeepCopy(TempData->GetLookupTable());
				output->GetPointData()->GetScalars()->SetLookupTable(LUT);	
				output->Modified();
				LUT->Delete();

			}

		}
		ReplaceCurrentSurface(output,true, true,"-n");
	}
}

//---------------------------------------------------------
void	SurfaceProcessing::SlotCleanPolyData()
{

	if (ObjectSurfaceData!=NULL)
	{
		//ofstream myFile("c:/out.txt");	
		//myFile << "1: " << endl<< endl<< *(ObjectSurfaceData->GetVPolyData()) << endl;

		vtkCleanPolyData *Clean2 = vtkCleanPolyData::New();
		Clean2->SetInputData(ObjectSurfaceData->GetVPolyData());
		Clean2->PointMergingOn ();
		Clean2->ConvertStripsToPolysOn ();
		Clean2->ConvertLinesToPointsOn ();
		Clean2->ConvertPolysToLinesOff ();
		Clean2->SetTolerance(1e-5);
		Clean2->Update();


		/// TAK TRZEBA ZAWSZE!!!!!!!!!!!!!1
		/// IDE JESC I RESZTE POPOPRAWIAM!
		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(Clean2->GetOutput());
//		//output->Update();	
		Clean2->Delete();

		//myFile << "out: " << endl<< endl<< *(output) << endl;

		//myFile << "poly: " << endl<< endl<< *(ObjectSurfaceData->GetVPolyData()) << endl;

		//		 //output->Update();


		ReplaceCurrentSurface(output,true, true,"-n");
		//myFile << "3: " << endl<< endl<< *(ObjectSurfaceData->GetVPolyData()) << endl;




	}
}

//---------------------------------------------------------
void	SurfaceProcessing::ExtractLinesFromSurface()
{
	if (ObjectSurfaceData != NULL)
	{
		vtkPolyData* TempPoly = ObjectSurfaceData->GetVPolyData();
		
		int nlines = TempPoly->GetNumberOfLines();
	

		
		vtkCellArray* newPolys = vtkCellArray::New();
		vtkIdType newCellPts[2];

		vtkIdType ID;
		if (nlines > 0)
		{
			vtkPoints* pts = GetSkeletalPoints(TempPoly);//mam juz punkty teraz kreski

			int nrOfPts = pts->GetNumberOfPoints();
			if (nrOfPts > 1)
			{

				for (int i = 0; i < nrOfPts - 1; i++)
				{
					newCellPts[0] = i;
					newCellPts[1] = i + 1;
					ID = newPolys->InsertNextCell(2, newCellPts);
				}
				vtkPolyData* output = vtkPolyData::New();
				output->SetPoints(pts);
				output->SetLines(newPolys);


				output->BuildCells();
			

				PolygonSurfaceData* Output1PolygonSurfaceData = new PolygonSurfaceData(output, VWidget);
				Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(ObjectSurfaceData->GetPolygonSurfaceName() + "_line");
				Output1PolygonSurfaceData->SetGroupName("lines");
				Output1PolygonSurfaceData->GetVProperty()->SetRepresentationToWireframe();

				
				for (int i = 0; i < 3; i++)
				{
					Output1PolygonSurfaceData->GetPN()[i] = ObjectSurfaceData->GetPN()[i];
					Output1PolygonSurfaceData->GetPScale()[i] = ObjectSurfaceData->GetPScale()[i];
				}


				emit	SignalSendNewSurface(Output1PolygonSurfaceData);
				


			}




		}


		/*PolygonSurfaceData* Output1PolygonSurfaceData = new PolygonSurfaceData(glyph->GetOutput(), VWidget);


		Output1PolygonSurfaceData->GetPN()[0] = data->GetNx();
		Output1PolygonSurfaceData->GetPScale()[0] = scaleX;
		Output1PolygonSurfaceData->GetPN()[1] = data->GetNy();
		Output1PolygonSurfaceData->GetPScale()[1] = scaleY;
		Output1PolygonSurfaceData->GetPN()[2] = 255;
		Output1PolygonSurfaceData->GetPScale()[2] = scales[2];



		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("balls");

		emit	SignalSendNewSurface(Output1PolygonSurfaceData);*/
	}
}
//------------------------------
void	SurfaceProcessing::SlotComputeNormals()
{

	if (ObjectSurfaceData!=NULL)
	{

		vtkPolyDataNormals *Normals=vtkPolyDataNormals::New();
		Normals->SplittingOff ();
		Normals->ComputePointNormalsOn ();
		Normals->NonManifoldTraversalOn  () ;
		Normals->ComputeCellNormalsOff ();
		Normals->SetInputData(ObjectSurfaceData->GetVPolyData());
		Normals->Update();
		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(Normals->GetOutput());
//		//output->Update();	
		Normals->Delete();

		////////////////////////////
		vtkDataArray *TempData=ObjectSurfaceData->GetVPolyData()->GetPointData()->GetScalars();
		if (TempData!=NULL)
			if ((TempData->GetRange()!=NULL)&&(TempData->GetLookupTable()!=NULL))
			{
				ObjectSurfaceData->GetVPolyData()->GetPointData()->SetScalars(NULL);
				output->GetPointData()->SetScalars(TempData);
				/*double *nr1;
				nr1=TempData->GetRange();
				if (nr1!=0)
				{
				VMapper->ScalarVisibilityOn();
				VMapper->SetScalarRange(nr1[0],nr1[1]);
				}
				else 
				VMapper->ScalarVisibilityOff();

				nr1=NULL;*/
				//output->GetPointData()->SetScalars()
				//	vtkDataArray *tempArray=vtkDataArray::DeepCopy(TempData);
			}



			////////////////////////


			ReplaceCurrentSurface(output,true, true,"-n");

	}
}


//-------------------------------------------------------
void	SurfaceProcessing::SlotCreateSpheresMapFromPictureData(RawDataSet *data, double *scales)
{
	if (data!=NULL)
	{
		double XResolution=(double)data->GetNx();
		double YResolution=(double)data->GetNy();
	
		int i,j;
		vtkIdType	newCellPts[3];
		double		newPt[3];
		int			NumberOfPoints=data->GetSliceSize();
		vtkPoints	*newPts=vtkPoints::New();
		//vtkFloatArray *Tscalars = vtkFloatArray::New();
		//newPts->SetNumberOfPoints(NumberOfPoints);
		int ptId=0;
		double scalar=1.0;
		
		
		unsigned short *aa=data->GetDataArray();
		int max=data->GetMaxValue();
		if (max==0) max=1; 

		double scaleX=scales[0];
		double scaleY=scales[1];
		double scaleZ=scales[2]*255/max;


		//robie punkty
		int pos=0;
		for(j=0;j<YResolution;j++)
		{
			for(i=0;i<XResolution;i++)
			{

				if (aa[pos]!=0)
				{
					newPt[0] =(XResolution-1-i)*scaleX;
					newPt[1] =j*scaleY;
					newPt[2] =aa[pos]*scaleZ;
					newPts->InsertNextPoint(newPt);

					//Tscalars->InsertTuple(ptId,&scalar); 
					ptId++;
				}
				pos++;
				
			}

		}
		vtkSphereSource *Sphere=vtkSphereSource::New();
		Sphere->SetPhiResolution(10);
		Sphere->SetThetaResolution(10);

		double BallSize=data->GetNx()*scaleX/40;


		vtkUnstructuredGrid *ugrid=vtkUnstructuredGrid::New();
		ugrid->SetPoints(newPts);
		vtkGlyph3D *glyph=vtkGlyph3D::New();
		glyph->SetInputData(ugrid);
//		glyph->SetSource(Sphere->GetOutput());
		glyph->SetScaleFactor(BallSize);

		
		

		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(glyph->GetOutput(),VWidget);
		

		Output1PolygonSurfaceData->GetPN()[0]=data->GetNx();
		Output1PolygonSurfaceData->GetPScale()[0]=scaleX;
		Output1PolygonSurfaceData->GetPN()[1]=data->GetNy();
		Output1PolygonSurfaceData->GetPScale()[1]=scaleY;
		Output1PolygonSurfaceData->GetPN()[2]=255;
		Output1PolygonSurfaceData->GetPScale()[2]=scales[2];


		
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("balls");
	
		emit	SignalSendNewSurface(Output1PolygonSurfaceData);
	}


}
//--------------------------------------------------
void	SurfaceProcessing::SlotCreatePolygonalCut3DData(RawDataSet  *data, RawDataSet *newData, int view, vtkPoints *pts,double *dd,PaletteRGBA *palette)
{
	if ((data==NULL)||(newData==NULL)||(pts==NULL)) return;

	int newx=newData->GetNx();
	int newy=newData->GetNy();
	int i,j,k;
	//RawDataSet(newx,newy,1,dxx,dz,1.0);
	double fPos;
	double xf,yf,zf;
	double tmpz;
	double dxx=newData->GetXscale();

	int n[3];
	data->GetN(n);

	vtkIdType	newCellPts[3];
	double		newPt[3];
	int			NumberOfPoints=newData->GetSliceSize();
	vtkPoints	*newPts=vtkPoints::New();
	vtkFloatArray *Tscalars = vtkFloatArray::New();
	newPts->SetNumberOfPoints(NumberOfPoints);
	int ptId=0;
	double scalar=1.0;
		
	double origin[3];
	data->GetOrigin(origin);
	double scale[3];
	data->GetScale(scale);
	double max=data->GetMaxValue();
	double factor=255.0/max;
	double point[3];
	double scale1[3]={1.0/scale[0],1.0/scale[1],1.0/scale[2]};
	for (i=0;i<newx;i++)
	{		
		

		//fPos=dxx*i;
		pts->GetPoint(i,point);
		//xf=pts[0]->GetValue(fPos);
		//yf=vals[1]->GetValue(fPos);
		xf=point[0];
		yf=point[1];

		switch (view)
		{
		case 0:
			for (k=0;k<newy;k++)
			{
				tmpz=(k<newy-1)?(double)k:k-0.00001;		
				//zf=tmpz*dd[2];
				newPt[0] =xf*dd[0]+origin[0];
				newPt[1] =yf*dd[1]+origin[1];
				newPt[2] =tmpz*dd[2]+origin[2];
				newPts->SetPoint(ptId,newPt);

				scalar=newData->GetVoxelValue(i,newy-k-1,0)*factor;
				Tscalars->InsertTuple(ptId,&scalar); 
				ptId++;

			//	d->SetVoxelValue(i,newy-k-1,0,Interpolation3Linear(data,xf,yf,tmpz));
			}
			break;

		case 1:
			for (k=0;k<newy;k++)//(datax,xf,nz-tmpz-1.0,yf));
			{
				tmpz=(k<newy-1)?(double)k:k-0.00001;	
				//zf=tmpz*scale[2];
				newPt[0] =xf*dd[0]+origin[0];
				newPt[1] =(n[1]-tmpz-1.0)*dd[2]+origin[1];
				newPt[2] =(n[2]-yf-1)*dd[1]+origin[2];
				newPts->SetPoint(ptId,newPt);
					scalar=newData->GetVoxelValue(i,newy-k-1,0)*factor;
				Tscalars->InsertTuple(ptId,&scalar); 
				ptId++;
				//d->SetVoxelValue(i,newy-k-1,0,Interpolation3Linear(data,xf,n[2]-tmpz-1.0,n[1]-yf-1.0));
			}
			break;

		case 2:
			for (k=0;k<newy;k++)
			{
				tmpz=(k<newy-1)?(double)k:k-0.00001;	
				newPt[0] =tmpz*dd[2]+origin[0];
				newPt[1] =xf*dd[0]+origin[1];
				newPt[2] =(n[2]-yf-1)*dd[1]+origin[2];
				newPts->SetPoint(ptId,newPt);
					scalar=newData->GetVoxelValue(i,newy-k-1,0)*factor;
				Tscalars->InsertTuple(ptId,&scalar); 
				ptId++;
				//d->SetVoxelValue(i,newy-k-1,0,Interpolation3Linear(data,tmpz,n[0]-xf-1.0,n[1]-yf-1.0));
			}
			break;
		}

		//tmp+=dxx;

	}
	
	//robie trojkaty
		int NumberOfTriangles=NumberOfPoints*2;
		vtkCellArray *newPolys = vtkCellArray::New();
		newPolys->Allocate(newPolys->EstimateSize(3,NumberOfTriangles));

		vtkPolyData *output=vtkPolyData::New();
		output->SetPoints(newPts);
		output->SetPolys(newPolys);


		//outputCD->SetScalars(Tscalars); 
		output->GetPointData()->SetScalars(Tscalars); 
		newPts->SetNumberOfPoints(NumberOfPoints);
		output->BuildLinks();
		vtkIdType ID;


		//cellId=0;
		int pos=0;
		for(j=0;j<newx-1;j++)
		{

			
			for(i=0;i<newy-1;i++)
			{
				//Pierwszy Trojkat
				//newPts->GetPoint(j*XResolution+i,newPt);
				newCellPts[0] =pos;//j*XGraphResolution+i;
				newCellPts[1] =pos+1;//j*XGraphResolution+i+1;
				newCellPts[2] =pos+newy;
				ID=newPolys->InsertNextCell(3,newCellPts);
	

				//cellId++;

				//DrugiTrojkat
				newCellPts[0] =pos+1;
				newCellPts[1] =pos+newy+1;
				newCellPts[2] =pos+newy;
				ID=newPolys->InsertNextCell(3,newCellPts);	
				pos++;


			}
			pos++;
		}


		//kolorowanka paleta
		//kolorowanka paleta
		if (palette!=NULL)
		{
		

			vtkLookupTable *lut = vtkLookupTable::New();
			lut->SetAlphaRange(0,256);
			lut->SetNumberOfColors(256);
			lut->SetTableRange(0, 256);
			for (i=0;i<256;i++)
			{  lut->SetTableValue(i,(double)(palette->R[i]/255.0),(double)(palette->G[i]/255.0),	(double)(palette->B[i]/255.0),1.0);}
			lut->Build();
			Tscalars->SetLookupTable(lut);


		
		}

		output->BuildCells();
//		//output->Update();
		newPolys->Delete();
		newPts->Delete();


		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
		
		
		Output1PolygonSurfaceData->GetPN()[0]=data->GetNx();
		Output1PolygonSurfaceData->GetPScale()[0]=data->GetXscale();
		Output1PolygonSurfaceData->GetPN()[1]=data->GetNy();
		Output1PolygonSurfaceData->GetPScale()[1]=data->GetYscale();
		Output1PolygonSurfaceData->GetPN()[2]=data->GetNz();
		Output1PolygonSurfaceData->GetPScale()[2]=data->GetZscale();

		
		
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("test");
	


		vtkDataArray *TempData2=output->GetPointData()->GetScalars();
	vtkMapper *VMapper2=Output1PolygonSurfaceData->GetVMapper();
	if (TempData2!=NULL)
		if ((TempData2->GetRange()!=NULL)&&(TempData2->GetLookupTable()!=NULL))
		{
			double *nr2;	nr2=TempData2->GetRange();
			if (nr2!=0)	{VMapper2->ScalarVisibilityOn();	VMapper2->SetScalarRange(nr2[0],nr2[1]);	}
			else VMapper2->ScalarVisibilityOff();
			nr2=NULL;
		}

		emit	SignalSendNewSurface(Output1PolygonSurfaceData);


}
//-----------------------------------------
void	SurfaceProcessing::SlotCreateHeightMapFromPictureData(RawDataSet *data, double *scales,PaletteRGBA *palette)
{
	if (data!=NULL)
	{
		double XResolution=(double)data->GetNx();
		double YResolution=(double)data->GetNy();
	
		int i,j;
		vtkIdType	newCellPts[3];
		double		newPt[3];
		int			NumberOfPoints=data->GetSliceSize();
		vtkPoints	*newPts=vtkPoints::New();
		vtkFloatArray *Tscalars = vtkFloatArray::New();
		newPts->SetNumberOfPoints(NumberOfPoints);
		int ptId=0;
		double scalar=1.0;
		
		
		unsigned short *aa=data->GetDataArray();
		int max=data->GetMaxValue();
		if (max==0) max=1; 

		double scaleX=scales[0];
		double scaleY=scales[1];
		double scaleZ=scales[2]*255/max;


		//robie punkty
		for(j=0;j<YResolution;j++)
		{
			for(i=0;i<XResolution;i++)
			{

			
				
				newPt[0] =(XResolution-1-i)*scaleX;
				newPt[1] =j*scaleY;
				newPt[2] =*(aa++)*scaleZ;
				newPts->SetPoint(ptId,newPt);

				Tscalars->InsertTuple(ptId,&scalar); 
				ptId++;
			//	MaxZ=MAX(MaxZ,newPt[2]);
			//	MinZ=MIN(MinZ,newPt[2]);
			}
		}
		//robie trojkaty
		int NumberOfTriangles=NumberOfPoints*2;
		vtkCellArray *newPolys = vtkCellArray::New();
		newPolys->Allocate(newPolys->EstimateSize(3,NumberOfTriangles));

		vtkPolyData *output=vtkPolyData::New();
		output->SetPoints(newPts);
		output->SetPolys(newPolys);


		//outputCD->SetScalars(Tscalars); 
		output->GetPointData()->SetScalars(Tscalars); 
		newPts->SetNumberOfPoints(NumberOfPoints);
		output->BuildLinks();
		vtkIdType ID;


		//cellId=0;
		int pos=0;
		for(j=0;j<data->GetNy()-1;j++)
		{

			
			for(i=0;i<data->GetNx()-1;i++)
			{
				//Pierwszy Trojkat
				//newPts->GetPoint(j*XResolution+i,newPt);
				newCellPts[0] =pos;//j*XGraphResolution+i;
				newCellPts[1] =pos+1;//j*XGraphResolution+i+1;
				newCellPts[2] =pos+data->GetNx();
				ID=newPolys->InsertNextCell(3,newCellPts);
	

				//cellId++;

				//DrugiTrojkat
				newCellPts[0] =pos+1;
				newCellPts[1] =pos+data->GetNx()+1;
				newCellPts[2] =pos+data->GetNx();
				ID=newPolys->InsertNextCell(3,newCellPts);	
				pos++;


			}
			pos++;
		}


		//kolorowanka paleta
		if (palette!=NULL)
		{
			int		nPoint=newPts->GetNumberOfPoints();

			double point1[3];
			vtkFloatArray *scalars1 = vtkFloatArray::New();
			scalars1->Allocate(nPoint);
			
		
			//double max=0;
			double t;
			int t1;
			aa=data->GetDataArray();
			for (i=0;i<nPoint;i++)
			{
				newPts->GetPoint(i,point1);
				t=(int)(*(aa++)*255.0/max);
				scalars1->InsertValue(i,t);
				//if (t>max) max=t;
			}
			t=max/255.0;

			vtkLookupTable *lut = vtkLookupTable::New();
			lut->SetAlphaRange(0,256);
			lut->SetNumberOfColors(256);
			lut->SetTableRange(0, 256);
			for (i=0;i<256;i++)
			{t1=i;   lut->SetTableValue(i,(double)(palette->R[t1]/255.0),(double)(palette->G[t1]/255.0),	(double)(palette->B[t1]/255.0),1.0);}
			lut->Build();
			scalars1->SetLookupTable(lut);


			output->GetPointData()->SetScalars(scalars1);
		}

		output->BuildCells();
//		//output->Update();
		newPolys->Delete();
		newPts->Delete();


		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
		

		Output1PolygonSurfaceData->GetPN()[0]=data->GetNx();
		Output1PolygonSurfaceData->GetPScale()[0]=scaleX;
		Output1PolygonSurfaceData->GetPN()[1]=data->GetNy();
		Output1PolygonSurfaceData->GetPScale()[1]=scaleY;
		Output1PolygonSurfaceData->GetPN()[2]=255;
		Output1PolygonSurfaceData->GetPScale()[2]=scales[2];


		
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("GRAPH");
	
		emit	SignalSendNewSurface(Output1PolygonSurfaceData);
	}

}
//-------------------------------------------------------------------------------
void	SurfaceProcessing::SlotGenerateMarchingCubesSurface(RawDataSet *Data,int lt,int rt)
{
	if (Data!=NULL)
	{

		vtkStructuredPoints *StructuredPoints = vtkStructuredPoints::New();
		StructuredPoints->SetSpacing(Data->GetXscale(),Data->GetYscale(), Data->GetZscale());
		//StructuredPoints->SetScalarType( VTK_UNSIGNED_SHORT);
		StructuredPoints->AllocateScalars(VTK_UNSIGNED_SHORT, 1);
		StructuredPoints->SetExtent( 0, Data->GetNx()-1, 0, Data->GetNy()-1, 0, Data->GetNz()-1 );
		//StructuredPoints->SetNumberOfScalarComponents(1);
		
		double origin[3];
		Data->GetOrigin(origin);
		StructuredPoints->SetOrigin(origin);

		vtkUnsignedShortArray *Array=vtkUnsignedShortArray::New();
		Array->SetVoidArray(Data->GetDataArray(),Data->GetVolumeSize(),1);
		StructuredPoints->GetPointData()->SetScalars(Array);

		vtkMarchingCubes *MarchingCubes = vtkMarchingCubes::New();
		MarchingCubes->SetInputData(StructuredPoints);
		MarchingCubes->ComputeScalarsOff();

		MarchingCubes->ComputeNormalsOn();

		if ((lt>Data->GetMinValue())&&(rt==Data->GetMaxValue()))
		{
			//tylko lewy
			MarchingCubes->SetValue( 0,lt);

		}
		else if((lt==Data->GetMinValue())&&(rt<Data->GetMaxValue()))
		{
			// tylko prawy
			MarchingCubes->SetValue( 0,rt);
		}
		else if((lt==Data->GetMinValue())&&(rt<Data->GetMaxValue()))
		{
			// oba
			MarchingCubes->SetValue( 0,lt);
			MarchingCubes->SetValue( 1,rt);

		}
		else if (lt!=rt)
		{
			//tylko 2 wartosci
			MarchingCubes->SetValue(0,(double)(rt-lt)/2);
		}
		else 
			return;

		//MarchingCubes->GenerateValues (2, double rangeStart, double rangeEnd)
		MarchingCubes->Update();
		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(MarchingCubes->GetOutput());
//		//output->Update();	

		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
		//------------------------------
		MarchingCubes->SetInputData(NULL);
		StructuredPoints->GetPointData()->SetScalars(NULL);
		StructuredPoints->Delete();
		Array->SetVoidArray(NULL,0,1);
		Array->Delete();
		MarchingCubes->Delete();
		//-----------------------------
		int i;
		//for(i=0;i<3;i++)
		//{
		Output1PolygonSurfaceData->GetPN()[0]=Data->GetNx();
		Output1PolygonSurfaceData->GetPN()[1]=Data->GetNy();
		Output1PolygonSurfaceData->GetPN()[2]=Data->GetNz();
		Output1PolygonSurfaceData->GetPScale()[0]=Data->GetXscale();
		Output1PolygonSurfaceData->GetPScale()[1]=Data->GetYscale();
		Output1PolygonSurfaceData->GetPScale()[2]=Data->GetZscale();
		//}

		Output1PolygonSurfaceData->SetCenterOrigin();
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("s_"+Data->GetRawDataSetName());
		emit	SignalSendNewSurface(Output1PolygonSurfaceData);


	}

}
//-------------------------------------------------------
void	SurfaceProcessing::SlotGenerateNewMarchingCubesSurface(unsigned char *MaskData,int *N,double *Scale, double *origin,const QString&name,	bool	repaintWithoutReset,bool isSegCutted,double *color)
{

	if (MaskData!=NULL)
	{
		vtkStructuredPoints *StructuredPoints = vtkStructuredPoints::New();
		StructuredPoints->SetSpacing( Scale[0],Scale[1], Scale[2]);
	//	StructuredPoints->SetScalarType( VTK_CHAR);
		StructuredPoints->AllocateScalars(VTK_CHAR, 1);
		StructuredPoints->SetExtent( 0, N[0]-1, 0, N[1]-1, 0, N[2]-1 );
		//StructuredPoints->SetNumberOfScalarComponents(1);
		StructuredPoints->SetOrigin(origin);
		//StructuredPoints->set

		vtkCharArray *Array=vtkCharArray::New();
		Array->SetVoidArray(MaskData,N[0]*N[1]*N[2],1);
		StructuredPoints->GetPointData()->SetScalars(Array);

		//vtkDiscreteMarchingCubes *MarchingCubes=vtkDiscreteMarchingCubes::New();
		vtkMarchingCubes *MarchingCubes = vtkMarchingCubes::New();
		MarchingCubes->SetInputData(StructuredPoints);
		MarchingCubes->ComputeScalarsOff();
		MarchingCubes->SetValue( 0,0.01);
		MarchingCubes->ComputeNormalsOn();
		MarchingCubes->Update();
		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(MarchingCubes->GetOutput());
//		//output->Update();	

		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
		//------------------------------
		MarchingCubes->SetInputData(NULL);
		StructuredPoints->GetPointData()->SetScalars(NULL);
		StructuredPoints->Delete();
		Array->SetVoidArray(NULL,0,1);
		Array->Delete();
		MarchingCubes->Delete();
		//-----------------------------
		int i;
		for(i=0;i<3;i++)
		{
			Output1PolygonSurfaceData->GetPN()[i]=N[i];
			Output1PolygonSurfaceData->GetPScale()[i]=Scale[i];
		}

		Output1PolygonSurfaceData->SetCenterOrigin();
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
		Output1PolygonSurfaceData->SetRepaintWithoutReset(repaintWithoutReset);
		Output1PolygonSurfaceData->SetIsSegCutted(isSegCutted);


		if (color!=NULL)
		{
			Output1PolygonSurfaceData->GetVActor()->GetProperty()->SetOpacity(color[3]);
			Output1PolygonSurfaceData->GetVActor()->GetProperty()->SetColor(color);
		}

		emit	SignalSendNewSurface(Output1PolygonSurfaceData);


	}
}
//-----------------------------------
PolygonSurfaceData*	SurfaceProcessing::GenerateMarchingCubesSmDecSurface(unsigned char *MaskData,int *N, double *Scale, double *origin,bool smoothOn1,bool decimOn, bool smoothOn2, int smoothIterations, double relaxionsFactor, double smoothAngle, double featureAngle, double targetred)
{

	if (MaskData != NULL)
	{/*
		int smoothIterations = 100;
		double relaxionsFactor = 0.01;
		double smoothAngle = 60.0;
		double featureAngle = 60.0;
		double targetred = 0.3;*/

		vtkSmartPointer<vtkStructuredPoints> structuredPoints = vtkSmartPointer<vtkStructuredPoints>::New();
		structuredPoints->SetSpacing(Scale[0], Scale[1], Scale[2]);
		//structuredPoints->SetScalarType( VTK_CHAR);
		structuredPoints->AllocateScalars(VTK_CHAR, 1);
		structuredPoints->SetExtent(0, N[0] - 1, 0, N[1] - 1, 0, N[2] - 1);
		//structuredPoints->SetNumberOfScalarComponents(1);
		structuredPoints->SetOrigin(origin);
		//StructuredPoints->set

		//vtkSmartPointer<vtkCharArray> marray=vtkSmartPointer<vtkCharArray>::New();
		vtkCharArray *marray = vtkCharArray::New();
		marray->SetVoidArray(MaskData, N[0] * N[1] * N[2], 1);

		structuredPoints->GetPointData()->SetScalars(marray);
		vtkSmartPointer<vtkDataObject> nextInput = vtkSmartPointer<vtkDataObject>::New();
		//vtkDiscreteMarchingCubes *MarchingCubes=vtkDiscreteMarchingCubes::New();
		vtkSmartPointer<vtkMarchingCubes> marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
		//vtkMarchingCubes *marchingCubes = vtkMarchingCubes::New();
		marchingCubes->SetInputData(structuredPoints);
		marchingCubes->ComputeScalarsOff();
		marchingCubes->SetValue(0, 0.5);
		marchingCubes->ComputeNormalsOn();
		marchingCubes->Update();
		nextInput = marchingCubes->GetOutput();
		

		if (smoothOn1)
		{
			vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
			smoother->SetInputData(nextInput);
			smoother->SetNumberOfIterations(smoothIterations);
			smoother->SetRelaxationFactor(relaxionsFactor);
			smoother->SetFeatureAngle(smoothAngle);
			smoother->FeatureEdgeSmoothingOff();
			smoother->SetConvergence(0);
			smoother->Update();
			nextInput = smoother->GetOutput();
		}


		vtkSmartPointer<vtkDecimatePro> decimate = vtkSmartPointer<vtkDecimatePro>::New();
		//decimate->SetInputData(smoother->GetOutput());
		if (decimOn == true)
		{
			decimate->SetInputData(nextInput);
			decimate->SetFeatureAngle(featureAngle);
			decimate->SetMaximumError(1);
			decimate->PreserveTopologyOn();
			decimate->SetTargetReduction(targetred);
			decimate->SplittingOff(); //pro
			decimate->Update();
			nextInput = decimate->GetOutput();
		}

		if (smoothOn2)
		{
			vtkSmartPointer<vtkSmoothPolyDataFilter> smoother2 = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
			smoother2->SetInputData(nextInput);
			smoother2->SetNumberOfIterations(smoothIterations);
			smoother2->SetRelaxationFactor(relaxionsFactor);
			smoother2->SetFeatureAngle(smoothAngle);
			smoother2->FeatureEdgeSmoothingOff();
			smoother2->SetConvergence(0);
			smoother2->Update();
			nextInput = smoother2->GetOutput();
		}


		vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
		normals->SetInputData(nextInput);
		normals->SetFeatureAngle(featureAngle);
		normals->Update();



		vtkPolyData *output = vtkPolyData::New();
		output->DeepCopy(normals->GetOutput());
		//		//output->Update();	

		PolygonSurfaceData *Output1PolygonSurfaceData = new PolygonSurfaceData(output, VWidget);
		//------------------------------
		marchingCubes->SetInputData(NULL);
		structuredPoints->GetPointData()->SetScalars(NULL);
		//structuredPoints->Delete();
		marray->SetVoidArray(NULL, 0, 1);
		marray->Delete();
		//marchingCubes->Delete();
		//-----------------------------
		int i;
		for (i = 0; i<3; i++)
		{
			Output1PolygonSurfaceData->GetPN()[i] = N[i];
			Output1PolygonSurfaceData->GetPScale()[i] = Scale[i];
		}

		Output1PolygonSurfaceData->SetCenterOrigin();
		return	Output1PolygonSurfaceData;


		 
	}
	return NULL;
}

//----------------------------------------
void	SurfaceProcessing::SlotGenerateNewMarchingCubesDecimSurface(unsigned char *MaskData,int *N,double *Scale, double *origin,const QString&name,	bool	repaintWithoutReset,bool isSegCutted)
{

	if (MaskData!=NULL)
	{
		int smoothIterations=100;
		double relaxionsFactor=0.01;
		double smoothAngle=60.0;
		double featureAngle=60.0;
		double targetred=0.3;

		vtkSmartPointer<vtkStructuredPoints> structuredPoints = vtkSmartPointer<vtkStructuredPoints>::New();
		structuredPoints->SetSpacing( Scale[0],Scale[1], Scale[2]);
		//structuredPoints->SetScalarType( VTK_CHAR);
		structuredPoints->AllocateScalars(VTK_CHAR, 1);
		structuredPoints->SetExtent( 0, N[0]-1, 0, N[1]-1, 0, N[2]-1 );
		//structuredPoints->SetNumberOfScalarComponents(1);
		structuredPoints->SetOrigin(origin);
		//StructuredPoints->set

		//vtkSmartPointer<vtkCharArray> marray=vtkSmartPointer<vtkCharArray>::New();
		vtkCharArray *marray=vtkCharArray::New();
		marray->SetVoidArray(MaskData,N[0]*N[1]*N[2],1);

		structuredPoints->GetPointData()->SetScalars(marray);

		//vtkDiscreteMarchingCubes *MarchingCubes=vtkDiscreteMarchingCubes::New();
		vtkSmartPointer<vtkMarchingCubes> marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
		//vtkMarchingCubes *marchingCubes = vtkMarchingCubes::New();
		marchingCubes->SetInputData(structuredPoints);
		marchingCubes->ComputeScalarsOff();
		marchingCubes->SetValue( 0,0.5);
		marchingCubes->ComputeNormalsOn();
		marchingCubes->Update();


		vtkSmartPointer<vtkSmoothPolyDataFilter> smoother =   vtkSmartPointer<vtkSmoothPolyDataFilter>::New(); 
        smoother->SetInputData(marchingCubes->GetOutput()); 
        smoother->SetNumberOfIterations(smoothIterations); 
        smoother->SetRelaxationFactor(relaxionsFactor); 
        smoother->SetFeatureAngle(smoothAngle); 
        smoother->FeatureEdgeSmoothingOff(); 
        smoother->SetConvergence(0); 
        smoother->Update(); 


		vtkSmartPointer<vtkDecimatePro> decimate =  vtkSmartPointer<vtkDecimatePro>::New(); 
		decimate->SetInputData(smoother->GetOutput()); 
        decimate->SetFeatureAngle(60.0); 
        decimate->SetMaximumError(1); 
        decimate->SetTargetReduction(targetred); 
		decimate->Update();

		vtkSmartPointer<vtkSmoothPolyDataFilter> smoother2 =   vtkSmartPointer<vtkSmoothPolyDataFilter>::New(); 
        smoother2->SetInputData(decimate->GetOutput()); 
        smoother2->SetNumberOfIterations(smoothIterations); 
        smoother2->SetRelaxationFactor(relaxionsFactor); 
        smoother2->SetFeatureAngle(smoothAngle); 
        smoother2->FeatureEdgeSmoothingOff(); 
        smoother2->SetConvergence(0); 
        smoother2->Update(); 

        vtkSmartPointer<vtkPolyDataNormals> normals  = vtkSmartPointer<vtkPolyDataNormals>::New();
         normals->SetInputData(smoother2->GetOutput()); 
         normals->SetFeatureAngle(featureAngle); 
         normals->Update(); 



		vtkPolyData *output=vtkPolyData::New();
		output->DeepCopy(normals->GetOutput());
//		//output->Update();	

		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
		//------------------------------
		marchingCubes->SetInputData(NULL);
		structuredPoints->GetPointData()->SetScalars(NULL);
		//structuredPoints->Delete();
		marray->SetVoidArray(NULL,0,1);
		marray->Delete();
		//marchingCubes->Delete();
		//-----------------------------
		int i;
		for(i=0;i<3;i++)
		{
			Output1PolygonSurfaceData->GetPN()[i]=N[i];
			Output1PolygonSurfaceData->GetPScale()[i]=Scale[i];
		}

		Output1PolygonSurfaceData->SetCenterOrigin();
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
		Output1PolygonSurfaceData->SetRepaintWithoutReset(repaintWithoutReset);
		Output1PolygonSurfaceData->SetIsSegCutted(isSegCutted);
		emit	SignalSendNewSurface(Output1PolygonSurfaceData);


	}
}


//----------------------------------------
void SurfaceProcessing::SlotUnifyTriangleOrientation()
{

	//	QMessageBox::about( 0,"nr","qq1");


	if ((ObjectSurfaceData!=NULL))
	{




		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();
		vtkCellArray *Polys = TempPoly->GetPolys();
		int NumberOfTriangles=Polys->GetNumberOfCells();

		vtkPoints *newPoints=vtkPoints::New();
		newPoints->DeepCopy(TempPoly->GetPoints());


		//		vtkPolyData *newPolyData=vtkPolyData::New();
		vtkCellArray *newPolys = vtkCellArray::New();
		newPolys->Allocate(newPolys->EstimateSize(3,NumberOfTriangles));



		int i,j;
		QString TMP="";

		//		int nrpts=3;

		//int *Triangle=new int[3];
		vtkIdType Triangle[3];
		vtkGenericCell		*TriangleCell=vtkGenericCell::New();

		//		int punkt0,punkt1;


		int *NeighborTab=new int[NumberOfTriangles*3];

		int *TriangleTab=new int[NumberOfTriangles*3];


		memset(NeighborTab,0,NumberOfTriangles*3*sizeof(int));
		memset(TriangleTab,0,NumberOfTriangles*3*sizeof(int));

		//zbieram trojkaciki do tablicy

		int pos=0;
		for(i=0;i<NumberOfTriangles;i++)
		{
			TempPoly->GetCell(i,TriangleCell);
			for(j=0;j<3;j++)
				TriangleTab[pos+j]=TriangleCell->GetPointId(j);

			pos+=3;
		}


		//znajdz sasiadow kazdego trojkata...
		int tmp=0;
		int k,l;
		int tmp2;
		int pos2=0;
		pos =0;
		int NrOfNeighbors;
		for(i=0;i<NumberOfTriangles;i++)
		{
			NrOfNeighbors=0;
			pos2=0;
			for(j=0;j<NumberOfTriangles;j++)
			{

				if (i!=j)
				{
					tmp2=0;
					for(k=0;k<3;k++)
						for(l=0;l<3;l++)
							if (TriangleTab[pos+k]==TriangleTab[pos2+l]) tmp2++;

					if (tmp2==2) 
					{
						NeighborTab[tmp++]=j;//sasiad!
						NrOfNeighbors++;
						//TMP+=QString::number(j)+" ";
					}
					if (NrOfNeighbors==3)
					{	
						//TMP+="\n";
						break;
					}
				}


				pos2+=3;
			}
			//	if (j==NumberOfTriangles)
			//	{
			//		QMessageBox::about( 0,"ERROR","Mesh not closed");
			//		return;
			//	}



			pos+=3;

		}
		//QMessageBox::about( 0,"",TMP);
		//no to chyba mamy sasiadow


		//teraz bedziemy odwracac te co rozne od elementu nr1;

		int starttrig = 1;

		int *Oriented=new int[NumberOfTriangles];
		memset(Oriented,0,NumberOfTriangles*sizeof(int));
		Oriented[starttrig]=1; // znaczy tylko tyle,ze pierwszy trojkat ma dobra orietnacje a reszta ma sie dostosowac


		int *tab1=new int[NumberOfTriangles];
		int *tab2=new int[NumberOfTriangles];

		int ltab1=0;
		int ltab2=0;

		//int pos1=0;
		//int pos2=0;

		int tt;

		tab1[ltab1++]=starttrig;

		int TEMP,tX;


		//int cnt = 1;
		int end = 0;
		int nt;

		int numberofflipped=0;

		while (!end)
		{
			end = 1;
			for (i = 0; i < ltab1; i++)
			{
				tt = tab1[i];//GetTriangle(list1.Get(i)); //numer analizowanego trojkata
				for (k = 0; k < 3; k++)
				{
					nt = NeighborTab[tt*3+k];//tt.NBTrigNum (k); // NeighbourTrig(list1.Get(i),k);
					if (Oriented[nt] == 0)
					{		


						if(CompareOrientationOf2Triangles(TriangleTab,tt,nt))
						{

							tX=nt*3;
							TEMP=TriangleTab[tX];
							TriangleTab[tX]=TriangleTab[tX+2];
							TriangleTab[tX+2]=TEMP;


							numberofflipped++;
						}
						Oriented[nt]=1;


						tab2[ltab2++]=nt;

						//cnt++;
						end = 0;
					}
				}
			}


			ltab1=0;

			for (i = 0; i < ltab2; i++)
			{
				tab1[ltab1++]=tab2[i];
			}

			ltab2=0;

		}

		//QMessageBox::about( 0,"",TMP);

		delete []tab1;
		delete []tab2;
		delete []Oriented;



		delete []NeighborTab;

		//to skasuje jak zrobie update trojkatow!
		pos=0;
		
		vtkIdType nptts,*ptts[3];
		vtkIdList *pointList=vtkIdList::New();
		for(i=0;i<NumberOfTriangles;i++)
		{

			//for(j=0;j<3;j++)
			//	Triangle[j]=TriangleTab[pos+j];

			//newPolys->InsertNextCell(3,Triangle);
			//ObjectSurfaceData->GetVPolyData()->GetPointer()[
			//ObjectSurfaceData->GetVPolyData()->GetPolys()->GetCell(i,ptts);
			ObjectSurfaceData->GetVPolyData()->GetPolys()->GetCell(i, pointList);
			for (j = 0;j < 3;j++)

				pointList->SetId(j, TriangleTab[pos + j]);
				//ptts[j]=TriangleTab[pos+j];
			pos+=3;
		}


	//	ObjectSurfaceData->GetVPolyData()->SetPolys(newPolys);
		//TempPoly->SetPolys(newPolys);
	//	newPolys->Delete();

		ObjectSurfaceData->RebuildSurface();


		delete []TriangleTab;

		QMessageBox::about( 0,"Changed:",QString::number(numberofflipped));



	}


}
//----------------------------------------

bool SurfaceProcessing :: CompareOrientationOf2Triangles(int *Tab1,int pos1,int pos2) 
{
	//triangles have not same orientation!!!
	int i, j;
	pos1*=3;
	pos2*=3;

	for(i = 0; i <3; i++)
	{
		for(j = 0; j <3; j++)
		{
			if ((Tab1[pos1+(i+1)%3]==Tab1[pos2+(j+1)%3]) &&(Tab1[pos1+i]==Tab1[pos2+j]))
			{return true;}
		}
	}

	return false;      
}

//----------------------------------------

void SurfaceProcessing::SlotFlipTriangleOrientation()
{

	if ((ObjectSurfaceData!=NULL))
	{

		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();
		vtkCellArray *Polys = TempPoly->GetPolys();
		int NumberOfTriangles=Polys->GetNumberOfCells();

		vtkCellArray *newPolys = vtkCellArray::New();
		newPolys->Allocate(newPolys->EstimateSize(3,NumberOfTriangles));



		int i,j;
		QString TMP="";

		//		int nrpts=3;
		int *TriangleTab=new int[NumberOfTriangles*3];
		memset(TriangleTab,0,NumberOfTriangles*3*sizeof(int));

		//int *Triangle=new int[3];
		vtkIdType Triangle[3];

		vtkGenericCell		*TriangleCell=vtkGenericCell::New();

		for(i=0;i<NumberOfTriangles;i++)
		{
			TempPoly->GetCell(i,TriangleCell);
			for(j=0;j<3;j++)
				Triangle[j]=TriangleCell->GetPointId(2-j);

			//QSortInt(Triangle,0,2);

			newPolys->InsertNextCell(3,Triangle);
		}


		TempPoly->SetPolys(newPolys);
		newPolys->Delete();

		ObjectSurfaceData->RebuildSurface();

	}
}
//----------------------------------------
void	SurfaceProcessing::SlotFillPolyDataHolesAsPlanar()
{
	if ((ObjectSurfaceData!=NULL))
	{
		RTimer.Start("Filling Holes");
		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();
		vtkPolyData *NewPolyData=FillPolyDataPlanarHolesWithMinimalArea(TempPoly);

		if (NewPolyData!=NULL)
		{
//			NewPolyData->Update();
			ReplaceCurrentSurface(NewPolyData,true, true,"-closed");
		}
		RTimer.Stop();

	}
}

//---------------------------------------------------
void	SurfaceProcessing::SlotFillPolyDataExtractedPlanarHoles()
{
	if ((ObjectSurfaceData!=NULL))
	{

		RTimer.Start("Filling Holes");

		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();
		vtkPolyData *NewPolyData= FillPolyDataExtractedPlanarHoles( TempPoly);
		if (NewPolyData!=NULL)
		{
//			NewPolyData->Update();
			ReplaceCurrentSurface(NewPolyData,true, true,"-closed");
		}
		RTimer.Stop();
	}
}
//----------------------------------------
void	SurfaceProcessing::SlotFillPolyDataHoles()
{
	if ((ObjectSurfaceData!=NULL))
	{
		RTimer.Start("Filling Holes");
		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();
		vtkPolyData *NewPolyData= FillPolyDataHolesWithMinimalArea( TempPoly);
		if (NewPolyData!=NULL)
		{
//			NewPolyData->Update();
			ReplaceCurrentSurface(NewPolyData,true, true,"-closed");
		}
		RTimer.Stop();
	}
}
//---------------------------------------------------

//----------------------------------------
void	SurfaceProcessing::SlotFillPolyDataHolesMaxVolume()
{

	if ((ObjectSurfaceData!=NULL))
	{


		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();
		GraftVoxelize *GV=new GraftVoxelize();

		RTimer.Start("Filling Holes");


		vtkPolyData *NewPolyData= GV->FillPolydataHolesMaxVolume( TempPoly);
		if (NewPolyData!=NULL)
		{
//			NewPolyData->Update();
			ReplaceCurrentSurface(NewPolyData,true, true,"-closed");
		}
		RTimer.Stop();
	}
}

//----------------------------------------
void	SurfaceProcessing::SlotFillPolyDataHolesWithCrenellations()
{

	if ((ObjectSurfaceData!=NULL))
	{
		RTimer.Start("Filling Holes");
		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();

		vtkPolyData *NewPolyData= FillPolyDataHolesWithCrenellations( TempPoly);
		if (NewPolyData!=NULL)
		{
//			NewPolyData->Update();
			ReplaceCurrentSurface(NewPolyData,true, true,"-closed");
		}
		RTimer.Stop();
	}
}

//---------------------------------------------------
double	SurfaceProcessing::GetArteryArea(int density)
{

	if ((ObjectSurfaceData!=NULL))
	{
	
		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();
		vtkCellArray *newPolys		=	TempPoly->GetPolys();
		vtkPoints *InputPoints=TempPoly->GetPoints();
		int NumberOfPolys=TempPoly->GetNumberOfCells();

		vtkIdType Triangle[3];

		double P1[3],P2[3],P3[3];

		vtkGenericCell		*TriangleCell=vtkGenericCell::New();
		int i,j;
		int type;
		
		int capsTriangles=2*density;
		int triangles=0;


		for(i=0;i<NumberOfPolys;i++)
		{
			if (TempPoly->GetCellType(i)==5)
				triangles++;
		}
		
		int pos=0;
		triangles-=2*density;//nie biore pod uwage trojkatow zamykajacych rurki;

		double area=0;
		vtkTriangle *tr=vtkTriangle::New();
		for(i=0;i<NumberOfPolys;i++)
		{
			type=TempPoly->GetCellType(i);
			if (type==5)// to chyba znaczy ze trojkat
			{
				pos++;
				TempPoly->GetCell(i,TriangleCell);



				for(j=0;j<3;j++)
					Triangle[j]=TriangleCell->GetPointId(j);

				InputPoints->GetPoint(Triangle[0],P1);
				InputPoints->GetPoint(Triangle[1],P2);
				InputPoints->GetPoint(Triangle[2],P3);

				area+=tr->TriangleArea(P1,P2,P3);
				if (pos==triangles) break;

			}
		}

		tr->Delete();
		
		return area;
	}
	return 0;

}
//-----------------------------------------------
double	SurfaceProcessing::CalculateMeshVolume()
{

	if ((ObjectSurfaceData!=NULL))
	{
		FILE *Do_opa;
		//Do_opa=fopen("c:\\vol.txt","w");fprintf(Do_opa,"");fclose(Do_opa);
		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();

		vtkCellArray *newPolys		=	TempPoly->GetPolys();
		vtkPoints *InputPoints=TempPoly->GetPoints();
		int NumberOfPolys=TempPoly->GetNumberOfCells();

		int nrPolys=TempPoly->GetNumberOfPolys();
		vtkIdType Triangle[3];

		double P1[3],P2[3],P3[3];

		vtkGenericCell		*TriangleCell=vtkGenericCell::New();
		int i,j;
		double Volume=0;
		int testX;

		int type;
		for(i=0;i<NumberOfPolys;i++)
		{
			type=TempPoly->GetCellType(i);
			if (type==5)// to chyba znaczy ze trojkat
			{
				TempPoly->GetCell(i,TriangleCell);




				for(j=0;j<3;j++)
					Triangle[j]=TriangleCell->GetPointId(j);

				InputPoints->GetPoint(Triangle[0],P1);
				InputPoints->GetPoint(Triangle[1],P2);
				InputPoints->GetPoint(Triangle[2],P3);

				Volume+=(	-P3[0]*P2[1]*P1[2]
				+P2[0]*P3[1]*P1[2]
				+P3[0]*P1[1]*P2[2]
				-P1[0]*P3[1]*P2[2]
				-P2[0]*P1[1]*P3[2]
				+P1[0]*P2[1]*P3[2]);
			}
			else
			{
					vtkIdList *pointList=vtkIdList::New();
				TempPoly->GetCellPoints(i,pointList);
				testX=pointList->GetNumberOfIds();
					i=i;
			}
		}



		Volume/=6.0;

	//	QMessageBox::about( 0,"Volume:",QString::number(Volume));
	
		//Do_opa=fopen("c:\\vol.txt","a");fprintf(Do_opa,"%lf\n",Volume);fclose(Do_opa);
		
		return Volume;
	}
	return 0;
}
//-----------------------------------

//-----------------------------------
//---------------------------------------------------
void	SurfaceProcessing::SlotSphereDataSet(int PhiResolution,int ThetaResolution)
{

	vtkSphereSource *Sphere=vtkSphereSource::New();
	Sphere->SetPhiResolution(PhiResolution);
	Sphere->SetThetaResolution(ThetaResolution);


	vtkTransform *Transform=vtkTransform::New();



	if ((ObjectSurfaceData!=NULL))
	{
		PolygonSurfaceData *tmp=ObjectSurfaceData;
		Transform->Translate(tmp->GetOrigin()[0],tmp->GetOrigin()[1],tmp->GetOrigin()[2]);
	}	
	Transform->Scale(10.0,10.0,10.0);
	vtkTransformPolyDataFilter *TransformFilter=vtkTransformPolyDataFilter::New();
	TransformFilter->SetInputData(Sphere->GetOutput());
	TransformFilter->SetTransform(Transform);
	Sphere->Update();

	vtkPolyData *Oj=TransformFilter->GetOutput();
//	Oj->Update();

	PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(Oj,VWidget);

	Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("Sphere");

	//	Output1PolygonSurfaceData->ComputeGeometricOrigin();

	if ((ObjectSurfaceData!=NULL))
	{	
		PolygonSurfaceData *tmp=ObjectSurfaceData;
		Output1PolygonSurfaceData->GetOrigin()[0]=tmp->GetOrigin()[0];
		Output1PolygonSurfaceData->GetOrigin()[1]=tmp->GetOrigin()[1];
		Output1PolygonSurfaceData->GetOrigin()[2]=tmp->GetOrigin()[2];
	}
	else
	{
		Output1PolygonSurfaceData->GetOrigin()[0]=0.0;
		Output1PolygonSurfaceData->GetOrigin()[1]=0.0;
		Output1PolygonSurfaceData->GetOrigin()[2]=0.0;
	}



	emit	SignalSendNewSurface(Output1PolygonSurfaceData);




}

//------------------------------------------------

void	SurfaceProcessing::SlotFindMaxDistance()
{

	if ((ObjectSurfaceData!=NULL))
	{

		double	Point1[3];
		double	Point2[3];




		vtkPoints *Pts=ObjectSurfaceData->GetVPolyData()->GetPoints();

		int numPts=ObjectSurfaceData->GetVPolyData()->GetNumberOfPoints();
		int i,j;

		double MaxDistance=0;



		double dist;
		for (j=0;j<numPts;j++)
		{
			Pts->GetPoint(j,Point1);

			for (i=0;i<numPts;i++)
			{
				Pts->GetPoint(i,Point2);

				dist=	sqrt((Point2[0]-Point1[0])*(Point2[0]-Point1[0])+
					(Point2[1]-Point1[1])*(Point2[1]-Point1[1])+
					(Point2[2]-Point1[2])*(Point2[2]-Point1[2]));
				if (dist>MaxDistance) MaxDistance=dist;

			}


		}

		QMessageBox::about( 0,"Max Distance:",QString::number(MaxDistance));



	}
}

//------------------------------------------------

void	SurfaceProcessing::SlotTransformPolygonalData(double ScaleX,double ScaleY,double ScaleZ,
													  double ShearX,double ShearY,double ShearZ,
													  double OriginX,double OriginY,double OriginZ)
{
	if ((ObjectSurfaceData!=NULL))
	{


		//		double	Point[3];
		double *RotateCenter=new double[3];
		double *Matrix=new double [16];

		RotateCenter[0]=OriginX;
		RotateCenter[1]=OriginY;
		RotateCenter[2]=OriginZ;


		double *TransT=new double[13];

		TransT[1]=0;
		TransT[2]=0;
		TransT[3]=0;
		TransT[4]=0;
		TransT[5]=0;
		TransT[6]=0;
		TransT[7]=ShearX;
		TransT[8]=ShearY;
		TransT[9]=ShearZ;
		TransT[10]=ScaleX;
		TransT[11]=ScaleY;
		TransT[12]=ScaleZ;

		Transform *transX=new Transform(RotateCenter,1,1,1);

		transX->CalculateTrigonometricFunctions(TransT);
		transX->ScaleRotMatrix(TransT);
		transX->CalculateShearParameters1(TransT);
		transX->MakeAffineMatrix(Matrix,TransT);

		//	transX->MakeCompleteAffineMatrix(Matrix,TransT);

		//Normal
		double *Point=new double[3];
		double *Point1=new double[3];





		//transX->MultiplyPoint(Point1,Point); point1-dest point-source

		//vtkPoints *Pts=ObjectSurfaceData->GetVPolyData()->GetPoints();
		/*	PolygonSurfaceData *tmp=ObjectSurfaceData;


		vtkTransform *Transform=vtkTransform::New();
		Transform->Translate(-tmp->GetOrigin()[0],-tmp->GetOrigin()[1],-tmp->GetOrigin()[2]);

		Transform->Scale(ScaleX,ScaleY,ScaleZ);
		//	Transform->Translate(tmp->GetOrigin()[0],tmp->GetOrigin()[1],tmp->GetOrigin()[2]);

		vtkTransformPolyDataFilter *TransformFilter=vtkTransformPolyDataFilter::New();
		TransformFilter->SetInputData(ObjectSurfaceData->GetVPolyData());
		TransformFilter->SetTransform(Transform);

		*/

		/*	int numPts=ObjectSurfaceData->GetVPolyData()->GetNumberOfPoints();
		int i;

		for (i=0;i<numPts;i++)
		{

		Pts->GetPoint(i,Point);

		Point[0]*=ScaleX;
		Point[1]*=ScaleY;
		Point[2]*=ScaleZ;
		Pts->SetPoint(i,Point);
		}

		*/

		//vtkPolyData *output=TransformFilter->GetOutput();
		////output->Update();

		//ReplaceCurrentSurface(output,true, true,"-Rescaled");

		vtkPoints *Pts=ObjectSurfaceData->GetVPolyData()->GetPoints();

		int numPts=ObjectSurfaceData->GetVPolyData()->GetNumberOfPoints();
		int i;

		double t=0.0;
		double f;
		double c;
		double *cent=ObjectSurfaceData->GetOrigin();
		for (i=0;i<numPts;i++)
		{

			Pts->GetPoint(i,Point);

			c=sqrt((Point[0]-cent[0])*(Point[0]-cent[0])+(Point[1]-cent[1])*(Point[1]-cent[1]));
			if (c<0.000000001) c=1;
			//f=t*Point[2]/c;
			f=t/c+(Point[2]-cent[2])*t/100;
			TransT[6]=f;


/*
	c=sqrt(		(Point[0]-RotateCenter[0])*(Point[0]-RotateCenter[0]) 
							+	(Point[1]-RotateCenter[1])*(Point[1]-RotateCenter[1]));
							//+	(Point[2]-RotateCenter[2])*(Point[2]-RotateCenter[2]));
					if (c<0.000000001) c=1;
					f=t/c+(Point[2]-RotateCenter[2])*t/100;

*/

			transX->CalculateTrigonometricFunctions(TransT);
			transX->ScaleRotMatrix(TransT);
			transX->CalculateShearParameters1(TransT);
			transX->MakeAffineMatrix(Matrix,TransT);


			
			transX->MultiplyPoint(Point1,Point);
			Pts->SetPoint(i,Point1);
		}

		ObjectSurfaceData->GetVPolyData()->SetPoints(Pts);

		ObjectSurfaceData->RebuildSurface();
		VWidget->RepaintWidget();

		delete []Point;
		delete []Point1;
		delete []RotateCenter;
		delete []Matrix;
		delete []TransT;
	}

}
//-------------------------------------------------
void	SurfaceProcessing::SlotSetNewSurfaceOrigin()
{

	if (CuttingToolVisible)
	{
		double PointX1[3]; 
		CurrentCuttingToolActor->GetPosition(PointX1);
		emit SignalSendNewSurfaceOrigin(PointX1);
	}

}
void	SurfaceProcessing::	SlotCalculateNewSurfaceOrigin( LinkedList <PolygonSurfaceData*> *SurfaceDataList)
{

	if(SurfaceDataList->GetNumberOfItems()>=1)
	{
		
		int Nr=SurfaceDataList->GetNumberOfItems();
		int i;
		PolygonSurfaceData *Data;
		
		double totalBounds[6]={INT_MAX,-INT_MAX,INT_MAX,-INT_MAX,INT_MAX,-INT_MAX};
		double tempBounds[6];

		
		
		for (i=0;i<Nr;i++)
		{
			Data=SurfaceDataList->GetItemAtPos(i)->GetData();
			if (Data->GetTreeItem()->isSelected()==true)
			{
				Data->GetVPolyData()->GetBounds(tempBounds);
				if(tempBounds[0]<totalBounds[0]) totalBounds[0]=tempBounds[0]; if (tempBounds[1]>totalBounds[1]) totalBounds[1]=tempBounds[1];
				if(tempBounds[2]<totalBounds[2]) totalBounds[2]=tempBounds[2]; if (tempBounds[3]>totalBounds[3]) totalBounds[3]=tempBounds[3];
				if(tempBounds[4]<totalBounds[4]) totalBounds[4]=tempBounds[4]; if (tempBounds[5]>totalBounds[5]) totalBounds[5]=tempBounds[5];
			}

		}

		double origin[3]={(totalBounds[0]+totalBounds[1])*0.5,(totalBounds[2]+totalBounds[3])*0.5,(totalBounds[4]+totalBounds[5])*0.5};

		emit SignalSendNewSurfaceOrigin(origin);
	}

}
//----------------------
void	SurfaceProcessing::SlotGenerateSphericalPointsCuts( LinkedList <PolygonSurfaceData*> *sDataList,QStringList namesList,QVector <double> v, double radius)
{
		//vtkImplicitFunction* cuttingFunction = vtkImplicitFunction::New();
		vtkSmartPointer <vtkSphere> sphere=vtkSmartPointer <vtkSphere>::New();
		int nrOfCuts=v.count()/3;
		vtkSmartPointer <vtkClipPolyData> cutter=vtkSmartPointer <vtkClipPolyData>::New();	
		int i,j;
		double center[3];
		sphere->SetRadius(radius);
		QString name;

		//zmontuje source do ciecia - wszystkie nie VIS POINTS widoczne

		vtkAppendPolyData *Append1=vtkAppendPolyData::New();
		int nr=sDataList->GetNumberOfItems();
	

		PolygonSurfaceData *sData;

		for (i=0;i<nr;i++)
		{
			sData=sDataList->GetItemAtPos(i)->GetData();
			if (sData!=NULL)
			{
				if (	(sData->GetVProperty()->GetOpacity()>0))
				{
					if (sData->GetGroupName().compare("VIS POINTS")!=0)
					{
						Append1->AddInputData(sData->GetVPolyData());
					}
				}
			}
		}

		vtkSmartPointer <vtkPolyData > pData=Append1->GetOutput();
//		//pData->Update();
		cutter->SetInputData(pData);
		cutter->SetClipFunction(sphere);
		cutter->SetValue(0.0);
		cutter->GenerateClippedOutputOn();

		for(i=0;i<nrOfCuts;i++)
		{
				
			sphere->SetCenter(v.at(i*3),v.at(i*3+1),v.at(i*3+2));
			name=namesList.at(i);
			cutter->Update();
			vtkSmartPointer <vtkPolyData> pDatax=vtkSmartPointer <vtkPolyData>::New();
			pDatax->DeepCopy(cutter->GetClippedOutput());
//			pDatax->Update();


			vtkSmartPointer<vtkCleanPolyData >Clean1 = vtkSmartPointer<vtkCleanPolyData >::New();
			Clean1->SetInputData(pDatax);
			Clean1->PointMergingOn ();
			Clean1->ConvertStripsToPolysOn ();
			Clean1->ConvertLinesToPointsOn ();
			Clean1->ConvertPolysToLinesOff ();
			Clean1->SetTolerance(1e-5);
			Clean1->Update();


			PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(Clean1->GetOutput(),VWidget);

			//--------------------------
			for(j=0;j<3;j++)
			{
				Output1PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
				Output1PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];
			}
			Output1PolygonSurfaceData->GetVProperty()->DeepCopy(ObjectSurfaceData->GetVProperty());
			ObjectSurfaceData->SetHideWithoutRefresh(true);
			Output1PolygonSurfaceData->ComputeGeometricOrigin();
			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
			Output1PolygonSurfaceData->SetGroupName("SphereCuts");


			emit	SignalSendNewSurface(Output1PolygonSurfaceData);

			
		}
		

}
//----------------------
void	SurfaceProcessing::SlotCombineAllSurfaces( LinkedList <PolygonSurfaceData*> *DataSetList)
{
	//	QMessageBox::about( 0,"nr","qq1");
	vtkAppendPolyData *Append1=vtkAppendPolyData::New();
	int Nr=DataSetList->GetNumberOfItems();
	int i;

	PolygonSurfaceData *Data;
	PolygonSurfaceData *sData = NULL;

	for (i=0;i<Nr;i++)
	{
		Data=DataSetList->GetItemAtPos(i)->GetData();
		if (Data!=NULL)
			if (Data->GetTreeItem()->isSelected()==true)	
				Append1->AddInputData(Data->GetVPolyData());
		if (sData == NULL)
		{
			sData = Data;
		}
	}



	if (sData != NULL)
	{
		//vtkPolyData *Output = Append1->GetOutputPort();


		//int nrr = Output->GetNumberOfPoints();
		//	//output->Update();
		vtkCleanPolyData *Clean1 = vtkCleanPolyData::New();
		Clean1->SetInputConnection(Append1->GetOutputPort());
		Clean1->PointMergingOn();
		Clean1->ConvertStripsToPolysOn();
		Clean1->ConvertLinesToPointsOn();
		Clean1->ConvertPolysToLinesOff();
		Clean1->SetTolerance(1e-5);
		Clean1->Update();

		PolygonSurfaceData *Output1PolygonSurfaceData = new PolygonSurfaceData(Clean1->GetOutput(), VWidget);

	//--------------------------

	

		if (sData != NULL)
		{


			for (i = 0; i < 3; i++)
			{
				//Output1PolygonSurfaceData->GetPN()[i] = ObjectSurfaceData->GetPN()[i];
				Output1PolygonSurfaceData->GetPScale()[i] = sData->GetPScale()[i];
			}
			Output1PolygonSurfaceData->GetVProperty()->DeepCopy(sData->GetVProperty());
		}
	
	
		Output1PolygonSurfaceData->SetHideWithoutRefresh(true);
		Output1PolygonSurfaceData->ComputeGeometricOrigin();
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("Combined Surface");
		emit	SignalSendNewSurface(Output1PolygonSurfaceData);
	}
}

//----------------------

void	SurfaceProcessing::SlotCreateBoundaryDataFromSurface( )
{

	if (ObjectSurfaceData!=NULL)
	{
		if (ObjectSurfaceData->GetCuttingEdgePolyData()!=NULL)
		{
			vtkPolyData *dataX=vtkPolyData::New();
			dataX->DeepCopy(ObjectSurfaceData->GetCuttingEdgePolyData());
//			dataX->Update();


			PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(dataX,VWidget);

			int i;
			for(i=0;i<3;i++)
			{
				Output1PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
				Output1PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];
			}
			Output1PolygonSurfaceData->ComputeGeometricOrigin();
			QString name;
			name=ObjectSurfaceData->GetPolygonSurfaceName();
			name.append(".bound");
			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
			emit	SignalSendNewSurface(Output1PolygonSurfaceData);

		}
	}

}



//----------------------

void	SurfaceProcessing::SlotShowWhichSurfaceIsCurrent()
{

	if (ObjectSurfaceData!=NULL)
	{


		int NumberOfSamplePoints=30;
		int Nr;

		vtkPoints *Pts=ObjectSurfaceData->GetVPolyData()->GetPoints();
		int NrOfPts=Pts->GetNumberOfPoints();
		double PointX1[3]; 


		if (NrOfPts>0)
		{
			int tempmopde=CuttingMode;
			bool tempvis=CuttingToolVisible;
			double tempsize=CutingToolSize;





			double bounds[6];
			ObjectSurfaceData->GetVPolyData()->GetBounds(bounds);
			double maxsize[3];
			int i;
			for(i=0;i<3;i++)
				maxsize[i]=fabs(bounds[i*2+1]-bounds[i*2]);


			double newSize=MIN3(maxsize[0],maxsize[1],maxsize[2])/5;

			if (tempvis==false) SlotShowHideCuttingTool();
			SlotSetCuttingMode(1); //USTAW KULKE

			SphereActor->SetScale(newSize);

			QTime timer;


			for(i=0;i<NumberOfSamplePoints;i++)
			{

				Nr=(int) (rand()*(NrOfPts-1))/(double)RAND_MAX;
				Pts->GetPoint(Nr,PointX1);
				CurrentCuttingToolActor->SetPosition(PointX1);
				VWidget->RepaintWidget();

				timer.restart();
				while(timer.elapsed()<3)
				{

				}
				//	timer.stop();

			}

			// przywroc zapamietane ustawienia
			if (tempvis==false) SlotShowHideCuttingTool();

			SphereActor->SetScale(tempsize);

			SlotSetCuttingMode(tempmopde);
			VWidget->RepaintWidget();


		}
	}

}


void	SurfaceProcessing::SlotCreateGridPolyData(int nx,int ny,int nz,int de)
{


	double	Point[3];

	double nnx;
	double nny;
	double nnz;
	double minx;
	double miny;
	double minz;
	int x,y,z;//,d;
	int i;//,j;

	if (ObjectSurfaceData!=NULL)
	{

		double bounds[6];

		ObjectSurfaceData->GetVPolyData()->GetBounds(bounds);
		nnx=fabs(bounds[1]-bounds[0]);
		nny=fabs(bounds[3]-bounds[2]);
		nnz=fabs(bounds[5]-bounds[4]);
		minx=bounds[0];
		miny=bounds[2];
		minz=bounds[4];
	}
	else
	{
		nnx=100;
		nny=100;
		nnz=100;
		minx=0;
		miny=0;
		minz=0;

	}

	//		int NumberOfPoints=nx*ny*nz*de*de*de;

	vtkPoints *newPts=vtkPoints::New();
	//newPts->SetNumberOfPoints(NumberOfPoints);
	int sx=(nx-1)*de+1;
	int sy=(ny-1)*de+1;
	int sz=(nz-1)*de+1;

	for(z=0;z<sz;z++)
		for(y=0;y<sy;y++)
			for(x=0;x<sx;x++)
			{
				Point[0]=minx+(nnx*x)/((nx-1)*de);
				Point[1]=miny+(nny*y)/((ny-1)*de);
				Point[2]=minz+(nnz*z)/((nz-1)*de);
				newPts->InsertNextPoint(Point);
			}

			// pododawac liijki do meshu
			vtkCellArray *newPolys = vtkCellArray::New();
			vtkIdType newCellPts[2];
			///////////////////////////////////////////////
			// po xach 
			vtkIdType ID;
			int pos=0;
			for(z=0;z<nz;z++)
			{
				for(y=0;y<ny;y++)
				{
					for(x=0;x<sx-1;x++)
					{
						//x
						newCellPts[0] =pos;
						newCellPts[1] =pos+1;
						ID=newPolys->InsertNextCell(2,newCellPts);

						pos++;

					}
					pos++;
					pos+=(de-1)*sx;
				}
				pos+=sx*sy*(de-1)-(de-1)*sx;
			}

			///////////////////////////////////////////////		
			// po y-ach
			pos=0;
			for(z=0;z<nz;z++)
			{
				for(y=0;y<sy-1;y++)
				{
					for(x=0;x<nx;x++)
					{
						newCellPts[0] =pos;
						newCellPts[1] =pos+sx;
						ID=newPolys->InsertNextCell(2,newCellPts);
						pos+=de;
					}
					pos-=(de-1);
				}
				pos+=sx+(de-1)*sx*sy;


			}

			// po z-ach
			pos=0;
			for(z=0;z<sz-1;z++)
			{
				for(y=0;y<ny;y++)
				{
					for(x=0;x<nx;x++)
					{
						newCellPts[0] =pos;
						newCellPts[1] =pos+sx*sy;
						ID=newPolys->InsertNextCell(2,newCellPts);

						pos+=de;

					}
					pos-=(de-1);
					pos+=sx*(de-1);

				}
				pos-=sx*(de-1);
				//pos+=sx*sy;
			}


			vtkPolyData *output=vtkPolyData::New();
			output->SetPoints(newPts);
			output->SetLines(newPolys);


			output->BuildCells();
//			//output->Update();

			PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
			Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("grid");

			if (ObjectSurfaceData!=NULL)
				for(i=0;i<3;i++)
				{
					Output1PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
					Output1PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];
				}


				emit	SignalSendNewSurface(Output1PolygonSurfaceData);




}
//----------------------


vtkPolyData	*SurfaceProcessing::SlotCreateGridPolyData2(int nx,int ny,int nz,int de)
{

	if (ObjectSurfaceData!=NULL)
	{

		double	Point[3];
		double bounds[6];

		ObjectSurfaceData->GetVPolyData()->GetBounds(bounds);
		double nnx=fabs(bounds[1]-bounds[0]);
		double nny=fabs(bounds[3]-bounds[2]);
		double nnz=fabs(bounds[5]-bounds[4]);
		double minx=bounds[0];
		double miny=bounds[2];
		double minz=bounds[4];

		int x,y,z;//,d;
		//		int NumberOfPoints=nx*ny*nz*de*de*de;

		vtkPoints *newPts=vtkPoints::New();
		//newPts->SetNumberOfPoints(NumberOfPoints);
		int sx=(nx-1)*de+1;
		int sy=(ny-1)*de+1;
		int sz=(nz-1)*de+1;

		for(z=0;z<sz;z++)
			for(y=0;y<sy;y++)
				for(x=0;x<sx;x++)
				{
					Point[0]=minx+(nnx*x)/((nx-1)*de);
					Point[1]=miny+(nny*y)/((ny-1)*de);
					Point[2]=minz+(nnz*z)/((nz-1)*de);
					newPts->InsertNextPoint(Point);
				}

				// pododawac liijki do meshu
				vtkCellArray *newPolys = vtkCellArray::New();
				vtkIdType newCellPts[2];
				///////////////////////////////////////////////
				// po xach 
				vtkIdType ID;
				int pos=0;
				for(z=0;z<nz;z++)
				{
					for(y=0;y<ny;y++)
					{
						for(x=0;x<sx-1;x++)
						{
							//x
							newCellPts[0] =pos;
							newCellPts[1] =pos+1;
							ID=newPolys->InsertNextCell(2,newCellPts);

							pos++;

						}
						pos++;
						pos+=(de-1)*sx;
					}
					pos+=sx*sy*(de-1)-(de-1)*sx;
				}

				///////////////////////////////////////////////		
				// po y-ach
				pos=0;
				for(z=0;z<nz;z++)
				{
					for(y=0;y<sy-1;y++)
					{
						for(x=0;x<nx;x++)
						{
							newCellPts[0] =pos;
							newCellPts[1] =pos+sx;
							ID=newPolys->InsertNextCell(2,newCellPts);
							pos+=de;
						}
						pos-=(de-1);
					}
					pos+=sx+(de-1)*sx*sy;


				}

				// po z-ach
				pos=0;
				for(z=0;z<sz-1;z++)
				{
					for(y=0;y<ny;y++)
					{
						for(x=0;x<nx;x++)
						{
							newCellPts[0] =pos;
							newCellPts[1] =pos+sx*sy;
							ID=newPolys->InsertNextCell(2,newCellPts);

							pos+=de;

						}
						pos-=(de-1);
						pos+=sx*(de-1);

					}
					pos-=sx*(de-1);
					//pos+=sx*sy;
				}


				vtkPolyData *output=vtkPolyData::New();
				output->SetPoints(newPts);
				output->SetLines(newPolys);


				output->BuildCells();
//				//output->Update();

				return output;
	}

	return NULL;

}
//----------------------------------------
void	SurfaceProcessing::SlotRecomputeNormals(vtkPolyData	*Polys)
{
	vtkPolyDataNormals *Normals=vtkPolyDataNormals::New();
	Normals->SplittingOff ();
	Normals->ComputePointNormalsOn ();
	Normals->NonManifoldTraversalOn  () ;
	Normals->ComputeCellNormalsOff ();
	Normals->SetInputData(Polys);
	Normals->Update();

	vtkPolyData *output=vtkPolyData::New();
	output->DeepCopy(Normals->GetOutput());
//	//output->Update();	
	Normals->Delete();


	//skopiowac normalne z outputa do Polys


	vtkDataArray *normals=Polys->GetPointData()->GetNormals();
	vtkDataArray *normals1=output->GetPointData()->GetNormals();

	int nr=normals->GetNumberOfTuples();
//	int size=normals->GetNumberOfComponents();
	//Polys->GetPointData()->GetNormals()->DeepCopy(output->GetPointData()->GetNormals());
	//Polys->Update();
	int i;
	double *pointer;

	for(i=0;i<nr;i++)
	{
		pointer=normals1->GetTuple3(i);
		normals->SetTuple3(i,pointer[0],pointer[1],pointer[2]);
	}

	//------------------------------------

	output->Delete();

}
//---------------------------------------------------------------
void	SurfaceProcessing::SlotComputeNormals(vtkPolyData *&Polys)
{
	vtkPolyDataNormals *Normals=vtkPolyDataNormals::New();
	Normals->SplittingOff ();
	Normals->ComputePointNormalsOn ();
	Normals->NonManifoldTraversalOn  () ;
	Normals->ComputeCellNormalsOff ();
	Normals->SetInputData(Polys);
	Normals->Update();

	vtkPolyData *output=vtkPolyData::New();
	output->DeepCopy(Normals->GetOutput());
//	//output->Update();	
	Normals->Delete();


	vtkDataArray *TempData=Polys->GetPointData()->GetScalars();
	if (TempData!=NULL)
		if ((TempData->GetRange()!=NULL)&&(TempData->GetLookupTable()!=NULL))
		{
			Polys->GetPointData()->SetScalars(NULL);
			output->GetPointData()->SetScalars(TempData);
		}

		Polys->Delete();

		Polys=output;
}
//-------------------------------------
//-----------------------------------------
void SurfaceProcessing::MultiGridSurfaceProcess(void (SurfaceProcessing::*funcs)(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,
												const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent),
												LinkedList <PolygonSurfaceData*> *SurfaceDataList,
												const QString &filename, PaletteRGBA *pal,int subframes,bool loop,bool relative, int vcomponent, double maxDisp )
{
	if(!filename.isEmpty())
	{
		PolygonSurfaceData *Data=SurfaceDataList->GetCurrentListItem()->GetData();
		if (Data->GetTreeItem()->isSelected()==true)
		{
			RTimer.Start("Deforming Surface");

			//PaletteRGBA *pal=NULL;
			//if (pal!=NULL)
			//		pal=MainListItem->GetData()->GetPalette();	

			double damount=1.0/(subframes+1);


			//-wczytac grid
			vtkPolyData *Poly=SurfaceDataList->GetCurrentListItem()->GetData()->GetVPolyData();
			QString name=SurfaceDataList->GetCurrentListItem()->GetData()->GetPolygonSurfaceName();
			int *N=SurfaceDataList->GetCurrentListItem()->GetData()->GetPN();
			double *S=SurfaceDataList->GetCurrentListItem()->GetData()->GetPScale();

			FloatGrid3D *grid1,*grid0,*tmpgrid;
			grid1=NULL;grid0=NULL;tmpgrid=NULL;
			grid1=new FloatGrid3D();
			int nr=grid1->GetNumberOfGrids(filename);
			int nnx=grid1->GetNx();
			int nny=grid1->GetNy();
			int nnz=grid1->GetNz();
			delete grid1; grid1=NULL;
			grid0=new FloatGrid3D(nnx,nny,nnz);
			vtkPolyData *startPolys=NULL;

			if (nr>0)
			{
				double min[3];
				double nn[3];
				double max=0;
				int i,j;
				int pos;
				double Point[3];
				CalculateMinAndSizeOfPolys(Poly,N,S,min,nn);

				if (relative==true)
				{
					startPolys=vtkPolyData::New();
					/*int nrOfPtsx=Poly->GetNumberOfPoints();
					vtkPoints *pts=Poly->GetPoints();
					startPoints=new double[nrOfPtsx*3];
					pos=0;
					for(j=0;j<nrOfPtsx;j++)
					{
						pts->GetPoint(j,Point);
						startPoints[pos++]=Point[0];
						startPoints[pos++]=Point[1];
						startPoints[pos++]=Point[2];
					}*/
					startPolys->DeepCopy(Poly);
					//startPolys->Update();


//vtkPoints *p=startPolys->GetPoints();
				}
	

				if (pal) 
				{
					if (maxDisp==0.0)
						max=DetectMaxDeformation(Poly,min,nn,filename,relative,vcomponent);
					else
						max=maxDisp;

					emit SignalSendNewMaxDisp(QString::number(max));
					//zerowa pokolorowana		
					(this->*funcs)(grid0,Poly,0.0,name+"_"+QString::number(0),min,nn,pal,max,startPolys,vcomponent);
					emit SignalHideNewPolygonalSurfaces(); //tymczasowe
				}

				
				pos=1;
				for(i=0;i<nr;i++)
				{

					//usun i stworz nowy i wczytaj z pliku
					grid1=new FloatGrid3D();
					grid1->LoadGrid(filename,i);
//					grid1->SaveGrid(filename+QString::number(i));
					//odwracam pole bo w pliku .mgrid sa deformacje wielu do 1-go a ja chce 1 do wielu			
					//grid1->ReverseField();
					BSplineGridTransformation b(grid1,grid1->GetNx(),grid1->GetNy(),grid1->GetNz(),0,0,0);
					//b.InvertGrid();
					//grid wczytany teraz petla po amount w obrebie kazdego gridu
					for (j=1;j<subframes+1;j++)
					{
						//stworz kopie zerowego gridu
						if (tmpgrid!=NULL)	delete tmpgrid;
						tmpgrid=new FloatGrid3D();
						tmpgrid->Copy(*grid0);
						//zblenduj z gridem pierwszym
						tmpgrid->Blend(grid1,j*damount);
						RTimer.TemporaryAppendToMainText("("+QString::number(pos++)+"/"+QString::number(nr*subframes)+") " );
						//wykonac!
						(this->*funcs)(tmpgrid,Poly,1.0,name+"_"+QString::number(i)+"_"+QString::number(j),min,nn,pal,max,startPolys,vcomponent);
						emit SignalHideNewPolygonalSurfaces(); //tymczasowe
						delete tmpgrid;
						tmpgrid=NULL;
					}
					/////////////////////
					//deform klatki glownej
					(this->*funcs)(grid1,Poly,1.0,name+"_"+QString::number(i+1),min,nn,pal,max,startPolys,vcomponent);
					emit SignalHideNewPolygonalSurfaces(); //tymczasowe
					//----------------
					if (grid0!=NULL){delete grid0; grid0=NULL;}
					grid0=grid1;
					grid1=NULL;
				}
				// dorobie subklatki pomiedzy ostatnia a pierwsza - loop znaczy
				// dorobie zaraz checkboxa
				if (loop==true)
					if (subframes!=0)
					{
						grid1=new FloatGrid3D();
						grid1=new FloatGrid3D(nnx,nny,nnz);
						//grid1->LoadGrid(filename,0);
						//grid wczytany teraz petla po amount w obrebie kazdego gridu
						for (j=1;j<subframes+1;j++)
						{
							//stworz kopie zerowego gridu
							if (tmpgrid!=NULL)	delete tmpgrid;
							tmpgrid=new FloatGrid3D();
							tmpgrid->Copy(*grid0);
							//zblenduj z gridem pierwszym
							tmpgrid->Blend(grid1,j*damount);
							//wykonac!
							(this->*funcs)(tmpgrid,Poly,1.0,name+"_"+QString::number(i)+"_"+QString::number(j),min,nn,pal,max,startPolys,vcomponent);
							emit SignalHideNewPolygonalSurfaces(); //tymczasowe
							delete tmpgrid;
							tmpgrid=NULL;
						}
					}
					RTimer.Stop();
			}
			else
				RTimer.StopError("Nothing to deform");

			Poly=NULL;
			//if (startPoints!=NULL){delete []startPoints; startPoints=NULL;}
			if (startPolys!=NULL) startPolys->Delete();
			if (grid0!=NULL){delete grid0; grid0=NULL;}
			if (grid1!=NULL){delete grid1; grid1=NULL;}
			if (tmpgrid!=NULL){delete tmpgrid; tmpgrid=NULL;}

			emit SignalShowNewPolygonalSurfaces();
		}
	}

}
//-----------------------------------------------------------------------------------------------------
PolygonSurfaceData *SurfaceProcessing::GridSurfaceProcess(PolygonSurfaceData * (SurfaceProcessing::*funcs)(FloatGrid3D *_Grid,vtkPolyData *Polys,double amount,
												const QString &name,double *min,double *nn,PaletteRGBA *palette,double max,vtkPolyData *startPolys, int vcomponent),
												LinkedList <PolygonSurfaceData*> *SurfaceDataList,
												FloatGrid3D *_Grid, PaletteRGBA *pal,int subframes,bool loop,bool relative, int vcomponent, double maxDisp )
{
	PolygonSurfaceData *d=NULL;
	if(_Grid!=NULL)
	{
		PolygonSurfaceData *Data=SurfaceDataList->GetCurrentListItem()->GetData();
		if (Data->GetTreeItem()->isSelected()==true)
		{
			RTimer.Start("Deforming Surface");

			//PaletteRGBA *pal=NULL;
			//if (pal!=NULL)
			//		pal=MainListItem->GetData()->GetPalette();	

			double damount=1.0/(subframes+1);


			//-wczytac grid
			vtkPolyData *Poly=SurfaceDataList->GetCurrentListItem()->GetData()->GetVPolyData();
			QString name=SurfaceDataList->GetCurrentListItem()->GetData()->GetPolygonSurfaceName();
			int *N=SurfaceDataList->GetCurrentListItem()->GetData()->GetPN();
			double *S=SurfaceDataList->GetCurrentListItem()->GetData()->GetPScale();
			FloatGrid3D *grid1=_Grid;
			//FloatGrid3D *grid1,*grid0,*tmpgrid;
			//grid1=NULL;grid0=NULL;tmpgrid=NULL;
			//grid1=new FloatGrid3D();
			//int nr=grid1->GetNumberOfGrids(filename);
			int nnx=grid1->GetNx();
			int nny=grid1->GetNy();
			int nnz=grid1->GetNz();
			//delete grid1; grid1=NULL;
			//grid0=new FloatGrid3D(nnx,nny,nnz);
			vtkPolyData *startPolys=NULL;


			int aa=Poly->GetNumberOfPoints();
			//if (nr>0)
			{
				double min[3];
				double nn[3];
				double max=0;
				int i,j;
				int pos;
				double Point[3];
				CalculateMinAndSizeOfPolys(Poly,N,S,min,nn);

				if (relative==true)
				{
					startPolys=vtkPolyData::New();
						startPolys->DeepCopy(Poly);
				//	startPolys->Update();
				}
	

				/*if (pal) 
				{
					if (maxDisp==0.0)
						max=DetectMaxDeformation(Poly,min,nn,filename,relative,vcomponent);
					else
						max=maxDisp;

					emit SignalSendNewMaxDisp(QString::number(max));
					//zerowa pokolorowana		
					(this->*funcs)(_Grid,Poly,0.0,name+"_"+QString::number(0),min,nn,pal,max,startPolys,vcomponent);
					emit SignalHideNewPolygonalSurfaces(); //tymczasowe
				}*/

				
				pos=1;
				//for(i=0;i<nr;i++)
				{

					//usun i stworz nowy i wczytaj z pliku
					//grid1=new FloatGrid3D();
					//grid1->LoadGrid(filename,i);
//					//grid1->SaveGrid(filename+QString::number(i));
					//odwracam pole bo w pliku .mgrid sa deformacje wielu do 1-go a ja chce 1 do wielu			
					//grid1->ReverseField();
					BSplineGridTransformation b(grid1,grid1->GetNx(),grid1->GetNy(),grid1->GetNz(),0,0,0);
					//b.InvertGrid();
					//grid wczytany teraz petla po amount w obrebie kazdego gridu
					/*for (j=1;j<subframes+1;j++)
					{
						//stworz kopie zerowego gridu
						if (tmpgrid!=NULL)	delete tmpgrid;
						tmpgrid=new FloatGrid3D();
						tmpgrid->Copy(*grid0);
						//zblenduj z gridem pierwszym
						tmpgrid->Blend(grid1,j*damount);
						RTimer.TemporaryAppendToMainText("("+QString::number(pos++)+"/"+QString::number(nr*subframes)+") " );
						//wykonac!
						(this->*funcs)(tmpgrid,Poly,1.0,name+"_"+QString::number(i)+"_"+QString::number(j),min,nn,pal,max,startPolys,vcomponent);
						emit SignalHideNewPolygonalSurfaces(); //tymczasowe
						delete tmpgrid;
						tmpgrid=NULL;
					}*/
					/////////////////////
					//deform klatki glownej
					d= (this->*funcs)(grid1,Poly,1.0,name+"_def",min,nn,pal,max,startPolys,vcomponent);
					//emit SignalHideNewPolygonalSurfaces(); //tymczasowe
					//----------------
					//if (grid0!=NULL){delete grid0; grid0=NULL;}
					//grid0=grid1;
					//grid1=NULL;
				}
				// dorobie subklatki pomiedzy ostatnia a pierwsza - loop znaczy
				// dorobie zaraz checkboxa
				//if (loop==true)
				//	if (subframes!=0)
				//	{
				//		grid1=new FloatGrid3D();
				//		grid1=new FloatGrid3D(nnx,nny,nnz);
				//		//grid1->LoadGrid(filename,0);
				//		//grid wczytany teraz petla po amount w obrebie kazdego gridu
				//		for (j=1;j<subframes+1;j++)
				//		{
				//			//stworz kopie zerowego gridu
				//			if (tmpgrid!=NULL)	delete tmpgrid;
				//			tmpgrid=new FloatGrid3D();
				//			tmpgrid->Copy(*grid0);
				//			//zblenduj z gridem pierwszym
				//			tmpgrid->Blend(grid1,j*damount);
				//			//wykonac!
				//			(this->*funcs)(tmpgrid,Poly,1.0,name+"_"+QString::number(i)+"_"+QString::number(j),min,nn,pal,max,startPolys,vcomponent);
				//			emit SignalHideNewPolygonalSurfaces(); //tymczasowe
				//			delete tmpgrid;
				//			tmpgrid=NULL;
				//		}
				//	}
					RTimer.Stop();
			}
			//else
			//	RTimer.StopError("Nothing to deform");

			Poly=NULL;
			//if (startPoints!=NULL){delete []startPoints; startPoints=NULL;}
			if (startPolys!=NULL) startPolys->Delete();
			//if (grid0!=NULL){delete grid0; grid0=NULL;}
			//if (grid1!=NULL){delete grid1; grid1=NULL;}
			//if (tmpgrid!=NULL){delete tmpgrid; tmpgrid=NULL;}

			//emit SignalShowNewPolygonalSurfaces();
		}
	}
	return d;

}
//----------------------------------------
PolygonSurfaceData *SurfaceProcessing::SlotGridSurfaceDeform(
	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
	FloatGrid3D*_Grid, PaletteRGBA *pal,int subframes,bool loop,bool relative, int vcomponent, double maxDisp )
{

	return GridSurfaceProcess(&SurfaceProcessing::SlotApplyFFDDeformation2,SurfaceDataList,_Grid,pal,subframes,loop,relative,vcomponent,maxDisp);
	//SlotApplyFFDDeformation
}

//---------------------------------------------
void SurfaceProcessing::SlotMultiGridSurfaceDeform(
	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
	const QString &filename, PaletteRGBA *pal,int subframes,bool loop,bool relative, int vcomponent, double maxDisp )
{

	MultiGridSurfaceProcess(&SurfaceProcessing::SlotApplyFFDDeformation,SurfaceDataList,filename,pal,subframes,loop,relative,vcomponent,maxDisp);
	//SlotApplyFFDDeformation
}



//------------------------------------
void SurfaceProcessing::SlotMultiGridSurfaceDeformationArrows(
	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
	const QString &filename, PaletteRGBA *pal,int subframes,bool loop,bool relative, int vcomponent, double maxDisp )
{

	MultiGridSurfaceProcess(&SurfaceProcessing::SlotApplyFFDSurfaceDeformationArrows,SurfaceDataList,filename,pal,subframes,loop,relative,vcomponent,maxDisp);

}
//-----------------------------------
void SurfaceProcessing::SlotMultiGridSurfaceDeformationLines(
	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
	const QString &filename, PaletteRGBA *pal,int subframes,bool loop,bool relative, int vcomponent, double maxDisp )
{

	MultiGridSurfaceProcess(&SurfaceProcessing::SlotApplyFFDSurfaceDeformationLines,SurfaceDataList,filename,pal,subframes,loop,relative,vcomponent,maxDisp);

}
//-----------------------------------
void SurfaceProcessing::CalculateMinAndSizeOfPolys(vtkPolyData *Polys,int *N,double *S,double *min, double *nn)
{
	if (Polys!=NULL)
	{
		double bounds[6];
		Polys->GetBounds(bounds);
		double max[3];
		min[0]=bounds[0];min[1]=bounds[2];min[2]=bounds[4];
		max[0]=bounds[1];max[1]=bounds[3];max[2]=bounds[5];
		nn[0]=fabs(max[0]-min[0]);	nn[1]=fabs(max[1]-min[1]);	nn[2]=fabs(max[2]-min[2]);


		if ((N[0]*N[1]*N[2]*S[0]*S[1]*S[2])>0)
		{
			double tol=10e-7;
			double Ns[3];
			Ns[0]=(double)(N[0]-1)*S[0];		Ns[1]=(double)(N[1]-1)*S[1];		Ns[2]=(double)(N[2]-1)*S[2];

			if ((Ns[0]!=0)&&(Ns[0]!=0)&&(Ns[0]!=0))
				if ((min[0]>0)&& (min[1]>0)&& (min[2]>0))
					if ((N[0]-max[0]>0)&&(N[1]-max[1]>0)&&(N[2]-max[2]>0))
					{
						//znaczy sie lapie sie do tego zeby ja traktowac jak zrobiona ze zbioru woluemtrycznego
						// tj obiekt nie musi zajmowac calego boundsu
						nn[0]=Ns[0];nn[1]=Ns[1];nn[2]=Ns[2];
						min[0]=0;min[1]=0;min[2]=0;
					}

		}
	}
}
//----------------------------------
double SurfaceProcessing::DetectMaxDeformation(vtkPolyData *Polys,double *min, double *nn, const QString &filename,bool relative, int vcomponent)
{
	FILE *Do_opa;
	Do_opa=fopen("c:\\maxDist.txt","w");fprintf(Do_opa,"");fclose(Do_opa);
	double maxDist=0;
	if(!filename.isEmpty())
	{
		FloatGrid3D *grid1;
		grid1=new FloatGrid3D();
		int nr=grid1->GetNumberOfGrids(filename);
		double *startPoints=NULL;
		if (nr>0)
		{
			int i,j;
			double Point[3],Point1[3],Point2[3];
			double def[3];
			double dist;
			int pos;
			double tmpDist;
			int numPts=Polys->GetNumberOfPoints();
			vtkPoints *Pts=Polys->GetPoints();
			vtkDataArray *normals=Polys->GetPointData()->GetNormals();
			double	v[3];
			double	normal[3];
			if (relative==true)
			{
				startPoints=new double[numPts*3];
				//memset(startPoints,numPts*3,sizeof(double));
			}			
			BSplineGridTransformation *BSplineTransform;

			for(i=0;i<nr;i++)
			{
				tmpDist=0;
				grid1->LoadGrid(filename,i);
				grid1->ReverseField();	
				BSplineGridTransformation b(grid1,grid1->GetNx(),grid1->GetNy(),grid1->GetNz(),0,0,0);
				b.InvertGrid();
				BSplineTransform=new BSplineGridTransformation(grid1,nn[0],nn[1],nn[2],min[0],min[1],min[2]);

				if (relative==true)
				{
					if (i==0)
					{
						pos=0;
						for(j=0;j<numPts;j++)
						{
							Pts->GetPoint(j,Point);
							startPoints[pos++]=Point[0];
							startPoints[pos++]=Point[1];
							startPoints[pos++]=Point[2];
						}
					}

					pos=0;
					for(j=0;j<numPts;j++)
					{
						Pts->GetPoint(j,Point);
						//BSplineTransform->TransformPoint(Point,Point1,def);
						if (enableAffineTransformation==false)
							BSplineTransform->TransformPoint(Point,Point1,def);
						else
							affineTransform->TransformPoint(Point,Point1,def);
						/*if (vcomponent!=0)
						{
							normals->GetTuple(i,normal);
							GetVectorComponents(Point,Point1, normal,v,vcomponent);
							Point1[0]=v[0];Point1[1]=v[1];Point1[2]=v[2];
						}*/


						Point2[0]=startPoints[pos]-Point1[0];	startPoints[pos++]=Point1[0];
						Point2[1]=startPoints[pos]-Point1[1];	startPoints[pos++]=Point1[1];
						Point2[2]=startPoints[pos]-Point1[2];	startPoints[pos++]=Point1[2];

						dist=sqrt(Point2[0]*Point2[0]+Point2[1]*Point2[1]+Point2[2]*Point2[2]);
						if (dist>maxDist) maxDist=dist;
						if (dist>tmpDist) tmpDist=dist;

					}
				}
				else
				{
					for(j=0;j<numPts;j++)
					{
						Pts->GetPoint(j,Point);
						//BSplineTransform->TransformPoint(Point,Point1,def);
						if (enableAffineTransformation==false)
							BSplineTransform->TransformPoint(Point,Point1,def);
						else
							affineTransform->TransformPoint(Point,Point1,def);
						if (vcomponent!=0)
						{
							normals->GetTuple(i,normal);
							GetVectorComponents(Point,Point1, normal,v,vcomponent);
							Point1[0]=v[0];Point1[1]=v[1];Point1[2]=v[2];
							Point2[0]=Point[0]-Point1[0];
							Point2[1]=Point[1]-Point1[1];	
							Point2[2]=Point[2]-Point1[2];	

							dist=sqrt(Point2[0]*Point2[0]+Point2[1]*Point2[1]+Point2[2]*Point2[2]);

						}
						else
							dist=sqrt(def[0]*def[0]+def[1]*def[1]+def[2]*def[2]);
						if (dist>maxDist) maxDist=dist;
						if (dist>tmpDist) tmpDist=dist;
					}
				}	
				
				Do_opa=fopen("c:\\maxDist.txt","a");fprintf(Do_opa,"%lf\n",tmpDist);fclose(Do_opa);
				delete BSplineTransform;

			}
		}
		delete grid1;
		if (startPoints!=NULL) delete []startPoints;
	
	}

	return maxDist;
}
//------------------------------------------
void SurfaceProcessing::SlotMultiGridSurfaceDeformationLinePaths(
	LinkedList <PolygonSurfaceData*> *SurfaceDataList,
	const QString &filename, PaletteRGBA *pal)
{

	if(!filename.isEmpty())
	{
		PolygonSurfaceData *Data=SurfaceDataList->GetCurrentListItem()->GetData();
		if (Data->GetTreeItem()->isSelected()==true)
		{
			RTimer.Start("Line Paths Generation");
			


			//-wczytac grid
			vtkPolyData *Poly=SurfaceDataList->GetCurrentListItem()->GetData()->GetVPolyData();
			QString name=SurfaceDataList->GetCurrentListItem()->GetData()->GetPolygonSurfaceName();
			int *N=SurfaceDataList->GetCurrentListItem()->GetData()->GetPN();
			double *S=SurfaceDataList->GetCurrentListItem()->GetData()->GetPScale();

			FloatGrid3D *grid,*tmpgrid;
			grid=NULL;
			grid=new FloatGrid3D();
			int nr=grid->GetNumberOfGrids(filename);
			if (nr>0)
			{
			

				//stworzyc nowe poly data
				//stworzyc nowe vtkpoints z punktami - (nr+1)*NrOfSourcePoints
				//przedeformowac wg kolejnych deffields kolejne pkty
				//pokolorowac po czasie kolejne klatki
				// polaczyck pky w linie
				
				vtkPoints *SourcePoints=Poly->GetPoints();
				int NrOfSourcePoints=SourcePoints->GetNumberOfPoints();
				int NrOfTotalPoints=(nr+1)*NrOfSourcePoints;
				vtkPoints *DestPoints=vtkPoints::New();
				//DestPoints->Allocate(NrOfTotalPoints);
				int i,j;
				int pos;
				for(i=0;i<NrOfSourcePoints;i++)
					//DestPoints->SetPoint(i,SourcePoints->GetPoint(i));
					DestPoints->InsertNextPoint(SourcePoints->GetPoint(i));
				//skopiowane pkty zrodlowe
			//

//-------------------------------------
				double min[3];
				double nn[3];
				double max=0;
				CalculateMinAndSizeOfPolys(Poly,N,S,min,nn);
				double Point[3],Point1[3];
				double def[3];
				pos=NrOfSourcePoints;
				BSplineGridTransformation *BSplineTransform;


double *colTab2=new double[NrOfTotalPoints];
memset(colTab2,0,NrOfTotalPoints*sizeof(double));

				for (j=0;j<nr;j++)
				{
					grid->LoadGrid(filename,j);
					grid->ReverseField();	
					BSplineGridTransformation b(grid,grid->GetNx(),grid->GetNy(),grid->GetNz(),0,0,0);
					b.InvertGrid();

					BSplineTransform=new BSplineGridTransformation(grid,nn[0],nn[1],nn[2],min[0],min[1],min[2]);
					for(i=0;i<NrOfSourcePoints;i++)
					{
						SourcePoints->GetPoint(i,Point);
						//BSplineTransform->TransformPoint(Point,Point1,def);
						if (enableAffineTransformation==false)
							BSplineTransform->TransformPoint(Point,Point1,def);
						else
							affineTransform->TransformPoint(Point,Point1,def);
						DestPoints->InsertNextPoint(Point1);

colTab2[i]+=sqrt(def[0]*def[0]+def[1]*def[1]+def[2]*def[2]);
						// tu bede wstawial zdeformowane pkty
					}
					delete BSplineTransform;
				}
				delete grid;
				int a=DestPoints->GetNumberOfPoints();


double max2=0;
for(i=0;i<NrOfSourcePoints;i++)
{
	if (colTab2[i]>max2) max2=colTab2[i];
}
double scale=(max2>0)?255.0/max2:1.0;
for(i=0;i<NrOfSourcePoints;i++)
{
	colTab2[i]*=scale;
}
////////////////////////////////
				// tu bede wstawial nowe scalary
				double *colTab=new double[NrOfTotalPoints];
				double step=255.0/(double)nr; //poprawialem!!!!!!!!!!!!!!!!!!25.07.2007
				double color=0;
				pos=0;
				for (j=0;j<=nr;j++)
				{
					for(i=0;i<NrOfSourcePoints;i++)
						colTab[pos++]=color;
					color+=step;
				}
				//----------------------------------
				// pokolorowane
				//---------------------------------

				//stworzyc new polys
				vtkCellArray *elems = vtkCellArray::New();
				vtkIdType newCellPts[2];
				vtkIdType ID;
				pos=0;
				for (j=0;j<nr;j++)
				{
					for(i=0;i<NrOfSourcePoints;i++)
					{
						newCellPts[0] =pos;	newCellPts[1] =pos+NrOfSourcePoints;
						ID=elems->InsertNextCell(2,newCellPts);
						pos++;
					}
				}
				for(i=0;i<NrOfSourcePoints;i++)
				{
					newCellPts[0] =pos;	newCellPts[1] =i;
					ID=elems->InsertNextCell(2,newCellPts);
					pos++;
				}

				vtkPolyData *NewPolys=vtkPolyData::New();
				NewPolys->SetPoints(DestPoints);
				NewPolys->SetLines(elems);
				NewPolys->BuildCells();
//				NewPolys->Update();

	
				//stworzyc lines
				//dodac points i lines do new polys

				SurfaceColorize *sCol=new SurfaceColorize();
				sCol->MapTabPaletteToMesh(NewPolys,colTab,pal);
				delete sCol;
				delete []colTab;

				
	vtkPolyData *NewPolys2=vtkPolyData::New();
	NewPolys2->DeepCopy(Poly);
	SurfaceColorize *sCol2=new SurfaceColorize();
	sCol2->MapTabPaletteToMesh(NewPolys2,colTab2,pal);
	delete sCol2;
	delete []colTab2;
	//-----------------------------------------------------------------
	//----------------------------------------
	// tylko po to zeby zrobic nowa powierzchnie

	PolygonSurfaceData *Output1PolygonSurfaceData2=new PolygonSurfaceData(NewPolys2,VWidget);

	if (name=="")
		Output1PolygonSurfaceData2->SetPolygonSurfaceDataSetName("def_surface");
	else
		Output1PolygonSurfaceData2->SetPolygonSurfaceDataSetName(name+"-activ");

	vtkDataArray *TempData2=NewPolys2->GetPointData()->GetScalars();
	vtkMapper *VMapper2=Output1PolygonSurfaceData2->GetVMapper();
	if (TempData2!=NULL)
		if ((TempData2->GetRange()!=NULL)&&(TempData2->GetLookupTable()!=NULL))
		{
			double *nr2;	nr2=TempData2->GetRange();
			if (nr2!=0)	{VMapper2->ScalarVisibilityOn();	VMapper2->SetScalarRange(nr2[0],nr2[1]);	}
			else VMapper2->ScalarVisibilityOff();
			nr2=NULL;
		}
	emit	SignalSendNewSurface(Output1PolygonSurfaceData2);

		/*FILE *Do_opa;
		Do_opa=fopen("c:\\max-active.txt","a");fprintf(Do_opa,"%lf\n",max2);fclose(Do_opa);*/

				//-----------------------------------------------------------------
				//----------------------------------------
				// tylko po to zeby zrobic nowa powierzchnie

				PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(NewPolys,VWidget);

				if (name=="")
					Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("def_surface");
				else
					Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name+"-path");

				vtkDataArray *TempData=NewPolys->GetPointData()->GetScalars();
				vtkMapper *VMapper=Output1PolygonSurfaceData->GetVMapper();
				if (TempData!=NULL)
					if ((TempData->GetRange()!=NULL)&&(TempData->GetLookupTable()!=NULL))
					{
						double *nr1;	nr1=TempData->GetRange();
						if (nr1!=0)	{VMapper->ScalarVisibilityOn();	VMapper->SetScalarRange(nr1[0],nr1[1]);	}
						else VMapper->ScalarVisibilityOff();
						nr1=NULL;
					}
				emit	SignalSendNewSurface(Output1PolygonSurfaceData);


			}
			RTimer.Stop();
		}
	}

}
//----------------------------------------------
void SurfaceProcessing::GetVectorComponents(double *p0,double *p1, double *n,double *v,int vcomponent)
{

	double t[3];
	t[0]=p1[0]-p0[0];
	t[1]=p1[1]-p0[1];
	t[2]=p1[2]-p0[2];

	double nx2,ny2,nz2;
	double nr;
	nr=n[0]*t[0]+n[1]*t[1]+n[2]*t[2];

	if (vcomponent==1) //normal
	{
		v[0]=n[0]*nr+p0[0];		v[1]=n[1]*nr+p0[1];		v[2]=n[2]*nr+p0[2];

	}
	else			//tangential
	{
		v[0]=t[0]-n[0]*nr+p0[0];		v[1]=t[1]-n[1]*nr+p0[1];		v[2]=t[2]-n[2]*nr+p0[2];
	}


}
//-----------------------------------------
//SlotMultiGridSurfaceDeformationLinePaths
int	SurfaceProcessing::SlotMapSurfaceIntoVolume(RawDataSet *data,double *pos,unsigned short val)
{
	int nr=0;
	if (ObjectSurfaceData!=NULL)
	{

		double XS=1.0/data->GetXscale();
		double YS=1.0/data->GetYscale();
		double ZS=1.0/data->GetZscale();
		int Nx=data->GetNx();
		int Ny=data->GetNy();
		int Nz=data->GetNz();
		int xx,yy,zz;
		double pPoint[3];
		int i;

		vtkPoints *pPoints=ObjectSurfaceData->GetVPolyData()->GetPoints();
		int pIndex=pPoints->GetNumberOfPoints();


		for (i=0;i<pIndex;i++)
		{
			pPoints->GetPoint(i,pPoint);

			// cofam kazdy punkt do poczatku kostki
			xx =Round((double) ((pPoint[0]-pos[0])*XS));
			yy =Round((double) ((pPoint[1]-pos[1])*YS));
			zz =Round((double) ((pPoint[2]-pos[2])*ZS));

			// tu trzeba wstawic cube obraniczjacy a nie caly!
			if ((xx>=0)&&(xx<Nx)&&(yy>=0)&&(yy<Ny)&&(zz>=0)&&(zz<Nz))
			{			
				data->SetVoxelValue(xx,yy,zz,val);
				nr++;
			}
		}

	}
	return nr;
}
//-----------------------------------------
void	SurfaceProcessing::SlotCreatePointPolyData(RawDataSet *data,unsigned char *mask)
{

	if((data!=NULL)&&(mask!=NULL))
	{
		double	Point[3];

		double nnx;
		double nny;
		double nnz;
		double minx;
		double miny;
		double minz;
		double x,y,z;//,d;
		int i,j,k;

		vtkPoints *newPts=vtkPoints::New();
		int nx=data->GetNx();
		int ny=data->GetNy();
		int nz=data->GetNz();;
		double sx=data->GetXscale();
		double sy=data->GetYscale();
		double sz=data->GetZscale();
		int pos;
		for (int xx=0;xx<3;xx++)
		for(k=0;k<nz;k=k+2)
			for(j=0;j<ny;j=j+2)
				for(i=0;i<nx;i=i+2)
				{
					//
					pos=data->GetVoxelPos(i,j,k);
					if (mask[pos]!=0)
					{	
						x=(rand()*2*sx)/(double)RAND_MAX-sx;
						y=(rand()*2*sy)/(double)RAND_MAX-sy;
						z=(rand()*2*sz)/(double)RAND_MAX-sz;
						Point[0]=i*sx+x;
						Point[1]=j*sy+y;
						Point[2]=k*sz+z;
						newPts->InsertNextPoint(Point);
					}
						
					//pos++;
				}

	

	
		vtkPolyData *output=vtkPolyData::New();
		output->SetPoints(newPts);
		//output->SetLines(newPolys);
		//output->BuildCells();
//		//output->Update();

		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("points");

		if (ObjectSurfaceData!=NULL)
			for(i=0;i<3;i++)
			{
				Output1PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
				Output1PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];
			}


		emit	SignalSendNewSurface(Output1PolygonSurfaceData);

	}


}
//----------------------
void SurfaceProcessing::SlotEnableAffineTransform(double *tr, int nr,double *rotCnt)
{
	if (tr!=NULL)
	{	
		
		int i;
		rotCenter[0]=rotCnt[0];
		rotCenter[1]=rotCnt[1];
		rotCenter[2]=rotCnt[2];

		
		if (affineTransform!=NULL)
			delete affineTransform;

		affineTransform=new Transform(rotCenter,1,1,1);
		for (i=0;i<15;i++)
			trans[i]=tr[i];
		enableAffineTransformation=true;
		affineTransform->tr=trans;
		affineTransform->Matrix=matrix;

		//trans=NULL;
		//affineTransform=NULL;
	}

}
//-------------------------------------------------
void SurfaceProcessing::SlotDisableAffineTransform()
{
	if (affineTransform!=NULL)
	{
		affineTransform->tr=NULL;
		affineTransform->Matrix=NULL;
		delete affineTransform;
		affineTransform=NULL;
		enableAffineTransformation=false;
		
	}
}
//-----------------------------------------------------------
PolygonSurfaceData	*SurfaceProcessing::SlotCreateErrorSurface(vtkPolyData *p0,vtkPolyData *p1,vtkPolyData *p2,PaletteRGBA *palette,double max,const QString &name,const QString &name2)
{

	PolygonSurfaceData *Output1PolygonSurfaceData=NULL;
	int i,j,k;

	//p0 - orig
	//p1 - def
	//p2 - sim
	if ((p0!=NULL)&&(p1!=NULL)&&(p2!=NULL))
	{
		vtkPolyData *NewPolys=vtkPolyData::New();
		NewPolys->DeepCopy(p2);
		/*
		PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(Clean1->GetOutput(),VWidget);

		//--------------------------
		for(i=0;i<3;i++)
		{
			Output1PolygonSurfaceData->GetPN()[i]=ObjectSurfaceData->GetPN()[i];
			Output1PolygonSurfaceData->GetPScale()[i]=ObjectSurfaceData->GetPScale()[i];
		}
		Output1PolygonSurfaceData->GetVProperty()->DeepCopy(ObjectSurfaceData->GetVProperty());
		ObjectSurfaceData->SetHideWithoutRefresh(true);
		Output1PolygonSurfaceData->ComputeGeometricOrigin();
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("Combined Surface");
		emit	SignalSendNewSurface(Output1PolygonSurfaceData);
		*/

		vtkPoints *points0=p0->GetPoints();
		vtkPoints *points1=p1->GetPoints();
		vtkPoints *points2=p2->GetPoints();

		int nrP0=p0->GetNumberOfPoints();
		int nrP1=p1->GetNumberOfPoints();
		int nrP2=p2->GetNumberOfPoints();
	
		int nr=MIN3(nrP0,nrP1,nrP2);
		double l1,l2;

		double  relErr=0;
		double rmsErr=0;
		double rmsSim=0;
		double avgDiff=0;
		double divRms=0;
		double tol=1e-5;
		double point0[3],point1[3],point2[3],u1[3],u2[3],d[3];
		
		double sqL1;
		double *cTab=new double[nr];
 
		for(i=0;i<nr;i++)
		{
			points0->GetPoint(i,point0);
			points1->GetPoint(i,point1);
			points2->GetPoint(i,point2);

			u1[0]=point1[0]-point0[0];
			u1[1]=point1[1]-point0[1];
			u1[2]=point1[2]-point0[2];

			u2[0]=point2[0]-point0[0];
			u2[1]=point2[1]-point0[1];
			u2[2]=point2[2]-point0[2];

			l1=(  (u2[0]-u1[0])*(u2[0]-u1[0]) 
					+ (u2[1]-u1[1])*(u2[1]-u1[1])
					+ (u2[2]-u1[2])*(u2[2]-u1[2]));

			l2=( u2[0]*u2[0]+u2[1]*u2[1]+u2[2]*u2[2]);

			//relErr+=(sqrt(l2>tol)?l1/l2:0;
			rmsSim+=l2;
			rmsErr+=l1;
			sqL1=sqrt(l1);
			avgDiff+=sqL1;

			if (palette !=NULL)	
				cTab[i]=sqL1;
		}
		
		if (nr!=0)
		{	
			divRms=(rmsSim>tol)?sqrt(rmsErr/rmsSim)*100:0;
			
		//	relErr*=100.0/(double)nr;
			rmsErr=sqrt(rmsErr/(double)nr);
			avgDiff/=nr;
		}

		double maxx=0;
		double minx=INT_MAX;
		int *errorMap=NULL;
		//--------------
		if (palette !=NULL)
		{
			//find min and max
			errorMap=new int[256];
			memset(errorMap,0,256*sizeof(int));

			minx=0;

			if (max==0)
			{
				for (i=0;i<nr;i++)	if (cTab[i]>maxx) 	maxx=cTab[i];
			}				
			else
				maxx=max;

			double div=maxx-minx;
			if (maxx-minx==0) div=255.0; else div=255.0/div;
			//scale tab

			for (i=0;i<nr;i++)
			{
				cTab[i]=(cTab[i]-minx)*div;				
				if (cTab[i]>255) cTab[i]=255.0;
				errorMap[(int)cTab[i]]++;
			
			}

			SurfaceColorize *sCol=new SurfaceColorize();
			sCol->MapTabPaletteToMesh(NewPolys,cTab,palette);
			delete sCol;

		}
		delete []cTab;
		// wstawiam nowa!
		if (NewPolys->GetNumberOfPolys()>0) 
			SlotComputeNormals(	NewPolys);
		//------------------------------------
		Output1PolygonSurfaceData=new PolygonSurfaceData(NewPolys,VWidget);
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName("diff-surface");
		
		//	Output1PolygonSurfaceData->RebuildSurface();
		vtkDataArray *TempData=NewPolys->GetPointData()->GetScalars();
		vtkMapper *VMapper=Output1PolygonSurfaceData->GetVMapper();
		if (TempData!=NULL)
			if ((TempData->GetRange()!=NULL)&&(TempData->GetLookupTable()!=NULL))
			{
				double *nr1;	nr1=TempData->GetRange();
				if (nr1!=0)	{VMapper->ScalarVisibilityOn();	VMapper->SetScalarRange(nr1[0],nr1[1]);	}
				else VMapper->ScalarVisibilityOff();
				nr1=NULL;
			}

		//	SignalSendNewSurface(Output1PolygonSurfaceData);
		

		//const QString &name
		QString namex1,namex2;
		namex1=name+".errors";
		namex2=name2+".errorMap";

				//name.toLocal8Bit()
		FILE *Do_opa;
		Do_opa=fopen(namex1.toLocal8Bit(),"a");fprintf(Do_opa," max: %lf avgDiff %lf rmsErr: %lf rmsErr/rmsSim: %lf  \n",maxx,avgDiff,rmsErr,divRms);fclose(Do_opa);
			
		if (errorMap!=NULL)
		{
			Do_opa=fopen(namex2.toLocal8Bit(),"w");
			for(i=0;i<256;i++)
				fprintf(Do_opa,"%d\n",errorMap[i]);
			fclose(Do_opa);
			
			delete []errorMap;
		}

		
		
		Do_opa=fopen((name+".plt").toLocal8Bit(),"a");
		fprintf(Do_opa,"set terminal postscript eps 22 'Times-Roman' 20\n");
		fprintf(Do_opa,"set key off\n");
		fprintf(Do_opa,"set output '%s'\n",(name2+".eps").toLocal8Bit());
		fprintf(Do_opa,"set autoscale fix\n");
		fprintf(Do_opa,"set yrange [0:]\n");
		fprintf(Do_opa,"plot '%s' with lines 1\n",namex2.toLocal8Bit());
		fprintf(Do_opa,"set terminal windows\n");
		fclose(Do_opa);
	}

	return Output1PolygonSurfaceData;


}
//----------------- 
double SurfaceProcessing::GetSumOfArteryDiameters(int &npts,double &startDi,double &endDi)
{
	//zwraca kopie punktow vtkPoints jesli sa i nie ma polygonow - wtedy polydata to szkielet:)
	
	npts=0;
	if ((ObjectSurfaceData!=NULL))
	{
		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();

		vtkCellArray *newPolys		=	TempPoly->GetPolys();
		vtkPoints *InputPoints=TempPoly->GetPoints();
		int nrOfPoints=InputPoints->GetNumberOfPoints();

		if (nrOfPoints==0) return 0;
		int NumberOfPolys=newPolys->GetNumberOfCells();

		
//jezeli sa linie to tylko punkty z linii!
		int nlines=TempPoly->GetNumberOfLines();
		int ntriangles=TempPoly->GetNumberOfPolys();


		if ((nlines>0)&&(ntriangles>0))
		{
			vtkCellArray *lines=TempPoly->GetLines();
			
			vtkIdType *newCellPts;
			double p[3];
			double pd[3];
			int i;
			vtkIdType ID;
			vtkIdType *pts;
			vtkIdType ptNr=npts;
			//w artery jest tak, ze 1 srodkowy 10 punktow ma kolko, potem kolejne 1 sr i 10...
			int pos=0;
			double val=0;
			double diameter=0;
			double tmpDi;
			int dest=(nrOfPoints-nlines-1)/(nlines+1);
			bool first=true;
			vtkIdList* pointList = vtkIdList::New();
			for(i=0;i<NumberOfPolys+1;i++)
			{
				if ( TempPoly->GetCellType(i) == VTK_LINE )
				{
					
					TempPoly->GetCellPoints(i, pointList);
					//TempPoly->GetCellPoints(i, ptNr, pts);
					InputPoints->GetPoint(pointList->GetId(0),p);
					InputPoints->GetPoint(pos*dest,pd);
					pos++;
					tmpDi=2*sqrt( (p[0]-pd[0])*(p[0]-pd[0])+ (p[1]-pd[1])*(p[1]-pd[1]) +(p[2]-pd[2])*(p[2]-pd[2]));
					diameter+=tmpDi;

					if (first==true)	{	startDi=tmpDi; first=false;	} else {endDi=tmpDi;};
					

					if (i==nlines-1)
					{
						InputPoints->GetPoint(pointList->GetId(1),p);
						InputPoints->GetPoint(pos*dest,pd);
						diameter+=2*sqrt( (p[0]-pd[0])*(p[0]-pd[0])+ (p[1]-pd[1])*(p[1]-pd[1]) +(p[2]-pd[2])*(p[2]-pd[2]));
						pos++;	
					}
				}
				

			}
			npts=pos;
			return diameter;

		}
		else
		{
			
			return 0;
		}
	}
	return 0;



}
//------------------------------------
vtkPoints	*SurfaceProcessing::GetSkeletalPoints()
{
	//zwraca kopie punktow vtkPoints jesli sa i nie ma polygonow - wtedy polydata to szkielet:)
	if ((ObjectSurfaceData!=NULL))
	{
		FILE *Do_opa;
		//Do_opa=fopen("c:\\vol.txt","w");fprintf(Do_opa,"");fclose(Do_opa);
		vtkPolyData *TempPoly=ObjectSurfaceData->GetVPolyData();

		vtkCellArray *newPolys		=	TempPoly->GetPolys();
		vtkPoints *InputPoints=TempPoly->GetPoints();
		int nrOfPoints=InputPoints->GetNumberOfPoints();

		if (nrOfPoints==0) return NULL;
		int NumberOfPolys=newPolys->GetNumberOfCells();

		

		
//jezeli sa linie to tylko punkty z linii!
		int nlines=TempPoly->GetNumberOfLines();
		vtkIdList* pointList = vtkIdList::New();
		if (nlines>0)
		{
			vtkCellArray *lines=TempPoly->GetLines();
			vtkIdType npts;
			vtkIdType *newCellPts;
			vtkPoints *newPts=vtkPoints::New();
			double p[3];
			int i;
			vtkIdType ID;
			vtkIdType *pts;
			for(i=0;i<NumberOfPolys;i++)
			{
				//TempPoly->GetCell(i,c
				//ewPolys->GetCell(i,npts,newCellPts);
				if ( TempPoly->GetCellType(i) == VTK_LINE )
				{
					//TempPoly->GetCellPoints(i, pts);
					TempPoly->GetCellPoints(i, pointList);
					InputPoints->GetPoint(pointList->GetId(0),p);
					newPts->InsertNextPoint(p);

					if (i==nlines-1)
					{
						InputPoints->GetPoint(pointList->GetId(1),p);
						newPts->InsertNextPoint(p);
					}
				}
				

			}
			int aa=newPts->GetNumberOfPoints();
			return newPts;

		}
		else
		{
			vtkPoints *pPoints=vtkPoints::New();
			pPoints->DeepCopy(InputPoints);
			return pPoints;
		}
	}
	return NULL;
}
//------------------------------------
vtkPoints	*SurfaceProcessing::GetSkeletalPoints(vtkPolyData *pData)
{
	//zwraca kopie punktow vtkPoints jesli sa i nie ma polygonow - wtedy polydata to szkielet:)
//	if ((ObjectSurfaceData!=NULL))
	{
		FILE *Do_opa;
		//Do_opa=fopen("c:\\vol.txt","w");fprintf(Do_opa,"");fclose(Do_opa);
		

		vtkCellArray *newPolys		=	pData->GetPolys();
		vtkPoints *InputPoints=pData->GetPoints();
		int nrOfPoints=InputPoints->GetNumberOfPoints();

		if (nrOfPoints==0) return NULL;
		int NumberOfPolys=newPolys->GetNumberOfCells();

		

		
//jezeli sa linie to tylko punkty z linii!
		int nlines=pData->GetNumberOfLines();

		if (nlines>0)
		{
			vtkCellArray *lines=pData->GetLines();
			vtkIdType npts;
			vtkIdType *newCellPts;
			vtkPoints *newPts=vtkPoints::New();
			double p[3];
			vtkIdType i;
			vtkIdType ID;
			vtkIdType *pts;
			vtkIdList* pointList = vtkIdList::New();
			for(i=0;i<NumberOfPolys;i++)
			{
				//TempPoly->GetCell(i,c
				//ewPolys->GetCell(i,npts,newCellPts);
				if ( pData->GetCellType(i) == VTK_LINE )
				{
					pData->GetCellPoints(i, pointList);
					InputPoints->GetPoint(pointList->GetId(0), p);
					//pData->GetCellPoints(i, npts, pts);
					//InputPoints->GetPoint(pts[0],p);
					newPts->InsertNextPoint(p);

					if (i==nlines-1)
					{
						InputPoints->GetPoint(pointList->GetId(1),p);
						newPts->InsertNextPoint(p);
					}
				}
				

			}
			int aa=newPts->GetNumberOfPoints();
			return newPts;

		}
		else
		{
			vtkPoints *pPoints=vtkPoints::New();
			pPoints->DeepCopy(InputPoints);
			return pPoints;
		}
	}
	return NULL;
}

///--------------------------------------------
double	 SurfaceProcessing::GenerateLine(double p1[3],double p2[3], QString name, QString group, double col[3])
{
		vtkPoints *pts=vtkPoints::New();
		pts->InsertNextPoint(p1);pts->InsertNextPoint(p2);
		

		vtkCellArray *newPolys = vtkCellArray::New();
		vtkIdType newCellPts[2];

		newCellPts[0] =0;	newCellPts[1] =1;
		newPolys->InsertNextCell(2,newCellPts);
		


		vtkPolyData *output=vtkPolyData::New();
		output->SetPoints(pts);
		output->SetLines(newPolys);
		output->BuildCells();
		output->BuildLinks();
//		//output->Update();

		PolygonSurfaceData *o1=new PolygonSurfaceData(output,VWidget);
		o1->SetPolygonSurfaceDataSetName(name);
		o1->GetVProperty()->SetColor(col);
		o1->ComputeGeometricOrigin();
		o1->setTmpData(true);
		o1->SetGroupName(group);
			emit	SignalSendNewSurface(o1);
		return GetDistance(p1,p2);
}
//-------------------------------------------
void	 SurfaceProcessing::GeneratePoly(double **points,int nrpts,QString name, QString group, double col[4],double *lenghts, double *angles,double &area,double &circumference ,double normal[3] ,bool genPolydata)
{
		
	
		vtkPoints *pts=vtkPoints::New();
		int i;
		for(i=0;i<nrpts;i++)
		{
			pts->InsertNextPoint(points[i]);
		}
		

		vtkCellArray *newPolys = vtkCellArray::New();
		vtkIdType *newCellPts=new vtkIdType[nrpts];

		for(i=0;i<nrpts;i++)
		{
			newCellPts[i]=i;
		}
		newPolys->InsertNextCell(nrpts,newCellPts);
		

		if(genPolydata==true)
		{
			vtkPolyData *output=vtkPolyData::New();
			output->SetPoints(pts);
			output->SetPolys(newPolys);
			output->BuildCells();
			output->BuildLinks();
//			//output->Update();

			PolygonSurfaceData *o1=new PolygonSurfaceData(output,VWidget);
			o1->SetPolygonSurfaceDataSetName(name);
			o1->GetVProperty()->SetColor(col);
			o1->GetVProperty()->SetOpacity(col[3]);
			o1->ComputeGeometricOrigin();
			o1->setTmpData(true);
			o1->SetGroupName(group);
			emit	SignalSendNewSurface(o1);
		}

		int i1,i2,i3;
		circumference=0;
		for (i=0;i<nrpts;i++)
		{
			i1=(nrpts+i-1)%nrpts;
			i2=i;
			i3=(nrpts+i+1)%nrpts;
			angles[i]=GetAngleBetweenLines(points[i2],points[i1],points[i3])* to_deg;
			lenghts[i]=GetDistance(points[i2],points[i3]);
			circumference+=lenghts[i];
		}
		//obliczam linie i obwod

		//obliczam katy

		//obliczam pole
		area;
		vtkSmartPointer<vtkPolygon> polygon= vtkSmartPointer<vtkPolygon>::New();
		//double normal[3];
		area=vtkPolygon::ComputeArea(pts,nrpts,newCellPts,normal);

		if (genPolydata==false)
		{
			pts->Delete();
			newPolys->Delete();
			delete []newCellPts;
		}
	

}


//-----------------
//-----------------
void	SurfaceProcessing::SlotGenerateFusionOfMultiTubes(LinkedList<PolygonSurfaceData *> *pDataList,QString *names)
{

	//sprawdzam czy istnieja jakies rurki z takimi samymi roles
	//dla kazdej znalezionej przeprowadze procedure laczenia

	//dla danej multi rurki biore pierwsza i jej szkielet - lewy i prawy koniec
	//sprawdzam odleglosc od pozostalych koncow - lewych i prawych biore minimalny i sklejam rurki- mam nowy szkielet
	//powtarzam az wszystkie polacze w 1
	//przeprowadzam procedure optymalizacyjna meshu
	//dodaje powierzchnie


	int i,j,k;
	int nr=pDataList->GetNumberOfItems();
	int roleNr;

	PolygonSurfaceData *pData1,*pData2;

	for(i=0;i<nr;i++)
	{
		
		pDataList->GetItemAtPos(i)->GetData()->SetTestFlag(0,false);//flaga tylko do zaznaczania czy przetworzylem juz dane dane:)
	}

	int *elTab=new int[nr];
	int nrOfEl;
	bool stop=false;
	int numero;

	int* rolesNR = new int[nr];
	for (i = 0;i < nr;i++)
	{
		pData1 = pDataList->GetItemAtPos(i)->GetData();
		rolesNR[i] = pData1->GetRoleIndex();
	}
	
	for(i=0;i<nr;i++)
	{
		
		pData1=pDataList->GetItemAtPos(i)->GetData();
		roleNr=pData1->GetRoleIndex();
		if (pData1->GetTestFlag(0)==true) continue;
		else if (roleNr<3)
		{
			pDataList->GetItemAtPos(i)->GetData()->SetTestFlag(0,true);	
			continue;//omijam te przetworzone i nie oznaczone jako konkretne rurencje
		}
		
		nrOfEl=1;
		elTab[0]=i;
		//memset(elTab,0,nr,
		bool allTubes=true;
		for(j=i+1;j<nr;j++)
		{
			pData2=pDataList->GetItemAtPos(j)->GetData();
			if (pData2->GetRoleIndex()==roleNr)
			{
				elTab[nrOfEl++]=j;
				if ( pData2->GetPolygonSurfaceName().indexOf("tube ")<0)
					allTubes=false;
			}
		}






		//zaznacze elementy zebrane jako te ktore sa (znaczy sie beda za chwile) przetworzone
		//for(j=0;j<nrOfEl;j++)
		//{
		//	pDataList->GetItemAtPos(j)->GetData()->SetTestFlag(0,true);
	//	}

		if (nrOfEl>1)
		{
			//przetwarzam tylko sekwencje rurek z multirolami nie przetworzone wczesniej

			//robie kopie pierwszej z powierzchni i zabieram sie do roboty
			if (i==15)
			{
				i=i;
			}
			vtkPolyData *polys1=vtkPolyData::New();
			vtkPolyData *pp = pDataList->GetItemAtPos(elTab[0])->GetData()->GetVPolyData();
			polys1->DeepCopy(pp);
//			polys1->Update();
			numero=polys1->GetNumberOfPoints();
			double rgb[3];
			pDataList->GetItemAtPos(elTab[0])->GetData()->GetVProperty()->GetColor(rgb);
			int nrOfElJoined=1;
			int minDistEl;
			bool minDistSide;
			double llDist,rrDist,lrDist,rlDist;
			double minDist,dist;
		
			double pl1[3],pr1[3];
			double pl2[3],pr2[3];
			bool ends[2];
			double tol=1e-6;
			
			pDataList->GetItemAtPos(elTab[0])->GetData()->SetTestFlag(0,true);//tego juz nie bede przetwarzal
			PolygonSurfaceData *o1=new PolygonSurfaceData(polys1,VWidget);
			if (allTubes==true)
			{
				do
				{
					minDist=INT_MAX;
					minDistEl=-1;
					for(j=0;j<nrOfEl;j++)
					{
						if (pDataList->GetItemAtPos(elTab[j])->GetData()->GetTestFlag(0)==false)
						{
							vtkPoints *pts1=GetSkeletalPoints(polys1);
							vtkPoints *pts2=GetSkeletalPoints(pDataList->GetItemAtPos(elTab[j])->GetData()->GetVPolyData());
							pts2->GetPoint(0,pl2);
							pts2->GetPoint(pts2->GetNumberOfPoints()-1,pr2);
							pts1->GetPoint(0,pl1);
							pts1->GetPoint(pts1->GetNumberOfPoints()-1,pr1);

							//teraz musze sprawdzic lewy i prawy distance dla koncow szkieletow
							llDist=GetDistance(pl1,pl2);
							lrDist=GetDistance(pl1,pr2);
							rlDist=GetDistance(pr1,pl2);
							rrDist=GetDistance(pr1,pr2);

							dist=MIN(	lrDist	,	MIN3(llDist,rrDist,rlDist)	);
							if (dist<minDist)
							{
								minDist=dist;
								minDistEl=j;
							
								if		(fabs(dist-llDist)<tol){ends[0]=1;ends[1]=0;}
								else if (fabs(dist-lrDist)<tol){ends[0]=1;ends[1]=1;}
								else if (fabs(dist-rlDist)<tol){ends[0]=0;ends[1]=0;}
								else if (fabs(dist-rrDist)<tol){ends[0]=0;ends[1]=1;}

							}
							pts2->Delete();
							pts1->Delete();

							// teraz musze szybko i latwo wykombinowac jak laczyc
							//pDataList->GetItemAtPos(elTab[j])->GetData()->SetTestFlag(0,true);
						
							//nrOfElJoined++
						}
					}
					if (minDistEl!=-1)
					{
						//polaczenie aktualnego zlaczenia z najblizsza rurka
						vtkPolyData *xpolys=joinTwoTubes(polys1,pDataList->GetItemAtPos(elTab[minDistEl])->GetData()->GetVPolyData(),ends);
						
						polys1->DeepCopy(xpolys);
//						polys1->Update();
						xpolys->Delete();
						pDataList->GetItemAtPos(elTab[minDistEl])->GetData()->SetTestFlag(0,true);	
						nrOfElJoined++;
						/*if ((i==15) &&(elTab[minDistEl]==27))
						{
							stop=true;
							break;
						}*/
					}
					else
					{
						i=i;
					}

				}while(nrOfElJoined!=nrOfEl);
				o1->SetPolygonSurfaceDataSetName("joined tube "+names[roleNr]);
			}
			else
			{
				//jesli to nie rurki - poprostu sumuje wszystkie w 1 zbior
				
				for(j=1;j<nrOfEl;j++)
				{
						vtkPolyData *xpolys=AppendPolyData(polys1,pDataList->GetItemAtPos(elTab[j])->GetData()->GetVPolyData());
						numero=pDataList->GetItemAtPos(elTab[j])->GetData()->GetVPolyData()->GetNumberOfPoints();
						numero=xpolys->GetNumberOfPoints();
						polys1->DeepCopy(xpolys);
///						polys1->Update();
						xpolys->Delete();
				}
				o1->SetPolygonSurfaceDataSetName("joined "+names[roleNr]);
			}

			for(j=0;j<nrOfEl;j++)
			{
				pDataList->GetItemAtPos(elTab[j])->GetData()->SetRoleIndex(-roleNr);
				rolesNR[elTab[j]]=-roleNr;
			}
			
			//o1->GetVActor()->GetProperty()->SetColor(0,0,0);
			o1->ComputeGeometricOrigin();
			o1->SetRoleIndex(roleNr);
			o1->setTmpData(true);
			o1->GetVProperty()->SetColor(rgb);
			emit	SignalSendNewSurface(o1);

			//wyslij wynik

			//polys0->Delete();

		}
		if (stop==true) break;

		
	}
	delete[]rolesNR;

}
//-----------------
vtkPolyData *SurfaceProcessing::joinTwoTubes(vtkPolyData *tube1,vtkPolyData *tube2,bool *ends)
{

	bool myEnds[2];

	vtkPoints *pts1=GetSkeletalPoints(tube1);
	vtkPoints *pts2=GetSkeletalPoints(tube2);

	int nr1=pts1->GetNumberOfPoints();
	int nr2=pts2->GetNumberOfPoints();
	if ((nr1==0)||(nr2==0)) return NULL;


	if (ends==NULL)
	{

		double pl1[3],pr1[3];
		double pl2[3],pr2[3];
		double llDist,rrDist,lrDist,rlDist;
		double minDist,dist;

		pts1->GetPoint(0,pl1);	pts1->GetPoint(pts1->GetNumberOfPoints()-1,pr1);
		pts2->GetPoint(0,pl2);	pts2->GetPoint(pts2->GetNumberOfPoints()-1,pr2);

		llDist=GetDistance(pl1,pl2);
		lrDist=GetDistance(pl1,pr2);
		rlDist=GetDistance(pr1,pl2);
		rrDist=GetDistance(pr1,pr2);

		dist=MIN(	lrDist	,	MIN3(llDist,rrDist,rlDist)	);
		
		double tol=1e-6;
		if		(fabs(dist-llDist)<tol){myEnds[0]=1;myEnds[1]=0;}
		else if (fabs(dist-lrDist)<tol){myEnds[0]=1;myEnds[1]=1;}
		else if (fabs(dist-rlDist)<tol){myEnds[0]=0;myEnds[1]=0;}
		else if (fabs(dist-rrDist)<tol){myEnds[0]=0;myEnds[1]=1;}

						

	}
	else
	{
		myEnds[0]=ends[0];
		myEnds[1]=ends[1];
	}

	//biore na wejsciu 2 rurki i generuje wyjsciowa

	pts1->Delete();
	pts2->Delete();


	int endPos1[2];int endPos2[2];

	int nrTotal1=tube1->GetNumberOfPoints();
	int nrTotal2=tube2->GetNumberOfPoints();

	int nrTriPts1=nrTotal1-nr1;
	int nrTriPts2=nrTotal2-nr2;

	int dest1=(nrTriPts1)/(nr1);
	int dest2=(nrTriPts2)/(nr2);



	if (dest1!=dest2) return NULL;

	endPos1[0]=0;
	endPos1[1]=nrTriPts1-dest1;

	endPos2[0]=0;
	endPos2[1]=nrTriPts2-dest2;


	
	
	int direction[2];
	int startTriPtsPos[2];
	int startTriPos[2];
	int startSkeletPos[2];
	int startTriCapsPos[2];
	vtkCellArray *polys1		=	tube1->GetPolys();
	vtkCellArray *polys2		=	tube2->GetPolys();
	vtkCellArray *polysFinal	=	vtkCellArray::New();
	vtkIdType triangle[3];
	vtkGenericCell				*triangleCell=vtkGenericCell::New();
	int type;
	int nrOfTris1=0;
	int nrOfTris2=0;
	
	double p1[3],p2[3],p[3];
	int i,j,k;
	int pos;

	if			((myEnds[0]==0)&&(myEnds[1]==0))// lp -lp
	{
		direction[0]		=	1;				direction[1]		=	1;
		startTriPtsPos[0]		=	0;				startTriPtsPos[1]		=	0;
		startSkeletPos[0]	=	nrTriPts1;		startSkeletPos[1]	=	nrTriPts2;
		
	}
	else	if	((myEnds[0]==0)&&(myEnds[1]==1))// lp - pl
	{
		direction[0]		=	1;				direction[1]		=	-1;
		startTriPtsPos[0]		=	0;				startTriPtsPos[1]		=	nrTriPts2-1;
		startSkeletPos[0]	=	nrTriPts1;		startSkeletPos[1]	=	nrTriPts2+nr2-1;	
	}
	else	if	((myEnds[0]==1)&&(myEnds[1]==0))// pl - lp
	{
		direction[0]		=	-1;				direction[1]		=	1;
		startTriPtsPos[0]		=	nrTriPts1-1;	startTriPtsPos[1]		=	0;
		startSkeletPos[0]	=	nrTriPts1+nr1-1;	startSkeletPos[1]	=	nrTriPts2;	
	}
	else	if	((myEnds[0]==1)&&(myEnds[1]==1))// pl - pl
	{
		direction[0]		=	-1;				direction[1]		=	-1;
		startTriPtsPos[0]		=	nrTriPts1-1;	startTriPtsPos[1]		=	nrTriPts2-1;
		startSkeletPos[0]	=	nrTriPts1+nr1-1;	startSkeletPos[1]	=	nrTriPts2+nr2-1;	
	}
	
	
	int nrCells[2];
	nrCells[0]=tube1->GetNumberOfCells();
	nrCells[1]=tube2->GetNumberOfCells();

	//namierzam poczatek trokjatow
	for(i=0;i<nrCells[0];i++)
	{
			if (tube1->GetCellType(i)==5)
			nrOfTris1++;
	}
	nrOfTris1-=2*dest1;//minus dekle

	for(pos=0,i=0;i<nrCells[0];i++)
	{
			if ((tube1->GetCellType(i)==5)	)
			{
				if 	(pos==(myEnds[0])*(nrOfTris1-1))
				{
					startTriPos[0]=i; break;
				}
				pos++;
			}
			
			
	}


	for(i=0;i<nrCells[1];i++)
	{
			if (tube2->GetCellType(i)==5)
			nrOfTris2++;
	}
	nrOfTris2-=2*dest2;//minus dekle

	
	for(pos=0,i=0;i<nrCells[1];i++)
	{
			if ((tube2->GetCellType(i)==5)	)
			{
				if 	(pos==(myEnds[1])*(nrOfTris2-1))
				{
					startTriPos[1]=i; break;
				}
				pos++;
			}
			
	}



	//punkty dodaje jak leci


	vtkPoints *pts=vtkPoints::New();
	vtkPoints *ptsOrig1=tube1->GetPoints();
	vtkPoints *ptsOrig2=tube2->GetPoints();
	int tmp;
	int *triPtsLookup1=new int[nrTriPts1];
	int *triPtsLookup2=new int[nrTriPts2];

	for(i=startTriPtsPos[0],pos=0;pos<nrTriPts1;i+=direction[0],pos++)
	{
		ptsOrig1->GetPoint(i,p);
		tmp=pts->InsertNextPoint(p);
		triPtsLookup1[i]=tmp;
	}
	for(i=startTriPtsPos[1],pos=0;pos<nrTriPts2;i+=direction[1],pos++)
	{
		ptsOrig2->GetPoint(i,p);
		tmp=pts->InsertNextPoint(p);
		triPtsLookup2[i]=tmp;
	}
	//skopiowalem punkty dla rurek
	int nrTriPtsFinal=nrTriPts1+nrTriPts2;

	//teraz dodam punkty od centerline

	int centerPoints[2];
	
	for(i=startSkeletPos[0],pos=0;pos<nr1;i+=direction[0],pos++)
	//for(i=0;i<nr1;i++)
	{
		ptsOrig1->GetPoint(i,p);
		tmp=pts->InsertNextPoint(p);
	}
	centerPoints[0]=tmp;
	centerPoints[1]=tmp+1;
	for(i=startSkeletPos[1],pos=0;pos<nr2;i+=direction[1],pos++)
	{
		ptsOrig2->GetPoint(i,p);
		pts->InsertNextPoint(p);
	}
	//zrobie centerline
	int nrFinal=nr1+nr2;
	vtkCellArray *skeletLine=vtkCellArray::New();
	vtkIdType newCellPts[2];
	vtkIdType id;
	for(i=0;i<nrFinal-1;i++)
	{	
		newCellPts[0]=nrTriPtsFinal+i;
		newCellPts[1]=nrTriPtsFinal+i+1;
		skeletLine->InsertNextCell(2,newCellPts);
	}
		

	//skopiuje odpowiednie trojkaty dla obu rurek



	//kopiuje z pierwszej rury
	for(i=startTriPos[0],pos=0;pos<nrOfTris1;i+=direction[0],pos++)
	{
		if (tube1->GetCellType(i)==5)
		{
			
			tube1->GetCell(i,triangleCell);

			for(j=0;j<3;j++) 
			{
				triangle[j]=triangleCell->GetPointId(j);
				triangle[j]=triPtsLookup1[triangle[j]];
			}
			polysFinal->InsertNextCell(3,triangle);

			//polysFinal->InsertNextCell(triangleCell);
		}
		//if (pos==nrOfTris1) break;
	}

	
	//------------tu trzeba przygotowac punkty do funkcji poszukujacej laczenia
	//-zbiore indeksy punktow - 2 zestawy
	// przekaze punkty nowej rury
	// funkcja sprawdzi jakie pole powstalej rurki wyjdzie wszystkich dest kombinacjach
	// wynikiem  bedzie miniamlne pole - taka konf zwroci
	int startMPoints[2];
	
		int dest=dest1;
	//startMPoints[0]=nrTriPts1-dest+myEnds[0]*(dest-1) ;
	//startMPoints[1]=nrTriPts1+myEnds[1]*(dest-1) ;
	startMPoints[0]=nrTriPts1-dest ;
	startMPoints[1]=nrTriPts1;

	int dirr[2]={1,1};
	GenerateMissingTubePart(pts,polysFinal,startMPoints, dirr,dest,centerPoints);
	
	//-----------------------
	//-----------------------

	//kopiuje z drugiej - musze przesunac o ilosc pktow z pierwszej rury poniewaz sklejam sety
	pos=0;
	for(i=startTriPos[1],pos=0;pos<nrOfTris2;i+=direction[1],pos++)
	{
		if (tube2->GetCellType(i)==5)
		{
		
			tube2->GetCell(i,triangleCell);
			for(j=0;j<3;j++) 
			{
				triangle[j]=triangleCell->GetPointId(j);
				triangle[j]=triPtsLookup2[triangle[j]];
			}
			polysFinal->InsertNextCell(3,triangle);
		}
		//if (pos==nrOfTris2) break;
	}

	//dodam tylko te dekle na ktore wskazuja ends
	//llewy

	
	bool test=false;
	int nrOfSegments=nr1+nr2;

	int pp1,pp2,pp3;
	pp3=nrTriPtsFinal;
	for(j=0;j<nrOfSegments;j+=nrOfSegments-1)
	{
		for(k=0;k<dest;k++)
		{
			pp1=k+j*dest;
			pp2=(k+1)%(dest)+j*dest;
						
			if (test==false)
			{
				triangle[0]=pp1;
				triangle[1]=pp2;
				triangle[2]=pp3;
			}
			else
			{
				triangle[2]=pp1;
				triangle[1]=pp2;
				triangle[0]=pp3;
			}
				polysFinal->InsertNextCell(3,triangle);
				//counter++;
		}
		test=true;
		pp3+=nrFinal-1;
					
	}
	
	delete []triPtsLookup1;
	delete []triPtsLookup2;

	/*for(i=0;i<dest1;i++)
	{

	}*/



	//dorobie brakujace trojkaty

	

	//linie przepisuje na nowo doklejajac z odpowiedniej strony 
	//dodaje trojkaty dla missing region
	//usuwam trojkaty odpowiedzialne za dekle
	vtkPolyData *output=vtkPolyData::New();	
	output->SetPoints(pts);
		output->SetPolys(polysFinal);
		output->SetLines(skeletLine);
		//output->SetLines(skeletLine);
		output->BuildCells();
//		//output->Update();

		vtkPolyDataNormals *normals=vtkPolyDataNormals::New();
		normals->SplittingOff ();
		normals->ComputePointNormalsOn ();
		normals->NonManifoldTraversalOn  () ;
		normals->ComputeCellNormalsOff ();
		normals->SetInputData(output);
		normals->AutoOrientNormalsOn();
		normals->Update();
		


		output->Delete();
		output=vtkPolyData::New();
		output->DeepCopy(normals->GetOutput());
//		//output->Update();	
		normals->Delete();
	return output;
}
//---------------------
void	SurfaceProcessing::GenerateMissingTubePart(vtkPoints *pts,vtkCellArray *polysFinal,int *startMPoints, int*direction,int dest,int *centerPoints)
{
	//najpierw poszukam mozliwych kombinacji

	int i,j,k;
	double minArea=INT_MAX;
	double area;
	int minPos=0;
	vtkTriangle *tr=vtkTriangle::New();
	double p1[3],p2[3],p3[3],p4[3];
	double pT1[3],pT2[3],pT3[3];
	int pos[4];
	
	double maxVolume=0;
	double volume;
	double angle, minAngle;

	minAngle=INT_MAX;
	//dodam wklad z dekli

	double p1A[3],p2A[3],p1B[3],p2B[3],p0A[3],p0B[3];

	pts->GetPoint(centerPoints[0],p0A);
	pts->GetPoint(centerPoints[1],p0B);
	//dodam sprawdzanie kolizji trojkatow

	int *triTab=new int[dest*3*2];
	int triNr;
	int pos1;
	bool triCheck;
	int tmp;
		
	double LinePoint1[3];
	double LinePoint2[3];
	int coplanar;
	double tol=1e-6;
	for(j=0;j<dest;j++)
	{
		area=0;
		volume=0;
		angle=0;
		memset(triTab,0,dest*3*2*sizeof(int));
		triNr=0;
		pos1=0;
		triCheck=false;
		for(i=0;i<dest;i++)	
		{

			/*pos[0]=i;
			pos[1]=(i+1)%dest;
			pos[2]=(j+i)%dest;
			pos[3]=(j+i+1)%dest;

			pos[0]=startMPoints[0]+pos[0]*direction[0];
			pos[1]=startMPoints[0]+pos[1]*direction[0];
			pos[2]=startMPoints[1]+pos[2]*direction[1];
			pos[3]=startMPoints[1]+pos[3]*direction[1];

			pts->GetPoint(pos[0],p1A);
			pts->GetPoint(pos[1],p2A);
			pts->GetPoint(pos[2],p1B);
			pts->GetPoint(pos[3],p2B);*/


			//katy pomiedzy 
			//p0A,p1A,p1B
			//p2A,p1A,p1B

			//p0B,p1B,p1A
			//p2B,p1B,p1A

			//bede minimalizowal
			
			//angle+=fabs(90-(GetAngleBetweenLines(p0A,p1A,p1B)*deg));
			//angle+=fabs(90-(GetAngleBetweenLines(p2A,p1A,p1B)*deg));
			//angle+=fabs(90-(GetAngleBetweenLines(p0B,p1B,p1A)*deg));
			//angle+=fabs(90-(GetAngleBetweenLines(p2B,p1B,p1A)*deg));
			
			//   
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//

			
			

			pos[0]=i;											pos[1]=(j+i)%dest;									pos[2]=(j+i+1)%dest;
			pos[0]=startMPoints[0]+pos[0]*direction[0];			pos[1]=startMPoints[1]+pos[1]*direction[1];			pos[2]=startMPoints[1]+pos[2]*direction[1];
			
			
			
			
			pts->GetPoint(pos[0],p1);							pts->GetPoint(pos[1],p2);							pts->GetPoint(pos[2],p3);
			
			for(k=0;k<i*3;k+=3)
			{
				
				tmp=GetNumberOfCommonPointsOf2Tiangles(pos[0], pos[1], pos[2],triTab[k],triTab[k+1],triTab[k+2]);
				if (tmp <2)
				{
					pts->GetPoint(triTab[k],pT1);							pts->GetPoint(triTab[k+1],pT2);							pts->GetPoint(triTab[k+2],pT3);
					
					if (tri_tri_intersection_test_3d(p1, p2, p3,pT1,pT2,pT3,&coplanar,LinePoint1,LinePoint2)==1)
					{
						if (!((fabs(LinePoint1[0]-LinePoint2[0])<tol)&&(fabs(LinePoint1[1]-LinePoint2[1])<tol)&&(fabs(LinePoint1[2]-LinePoint2[2])<tol)))
						{
							// jezeli 1 pkt tylko sie styka
							triCheck =true;
							break;
						}
					}
				}
			}
			
			triTab[pos1++]=pos[0];								triTab[pos1++]=pos[1];								triTab[pos1++]=pos[2];
			
			triNr++;
			
			

			
			area+=tr->TriangleArea(p1,p2,p3);	
			volume+=(	-p3[0]*p2[1]*p1[2]	+p2[0]*p3[1]*p1[2]	+p3[0]*p1[1]*p2[2]	-p1[0]*p3[1]*p2[2]	-p2[0]*p1[1]*p3[2]	+p1[0]*p2[1]*p3[2]);

			pos[0]=i;											pos[1]=(j+i+1)%dest;								pos[2]=(i+1)%dest; 
			pos[0]=startMPoints[0]+pos[0]*direction[0];			pos[1]=startMPoints[1]+pos[1]*direction[1];			pos[2]=startMPoints[0]+pos[2]*direction[0];
			pts->GetPoint(pos[0],p1);							pts->GetPoint(pos[1],p2);							pts->GetPoint(pos[2],p3);
			triTab[pos1++]=pos[0];								triTab[pos1++]=pos[1];								triTab[pos1++]=pos[2];
			triNr++;
			
			area+=tr->TriangleArea(p1,p2,p3);	
			volume+=(	-p3[0]*p2[1]*p1[2]	+p2[0]*p3[1]*p1[2]	+p3[0]*p1[1]*p2[2]	-p1[0]*p3[1]*p2[2]	-p2[0]*p1[1]*p3[2]	+p1[0]*p2[1]*p3[2]);

				
				
		}
		/*if (angle<minAngle)
		{
			minAngle=angle;
			minPos=j;
		}*/

		/*if (area<minArea)
		{
			minArea=area;
			minPos=j;
		}*/
		if (((volume>maxVolume)||(maxVolume==0))&&(triCheck==false))
		{
			minPos=j;
			maxVolume=volume;
		}
	}

	
	/*if (maxVolume - 1e-6 < 0)
	{

		double testDist = INT_MAX;
		int bestPos = 0;

		int i = 0;
		for (int j = 0; j < dest; j++)
		{
			///generalnie kolizyjne szukam najblizszego punktu do pktu zero
			pos[0] = i;											pos[1] = (j + i) % dest;									pos[2] = (j + i + 1) % dest;
			pos[0] = startMPoints[0] + pos[0] * direction[0];			pos[1] = startMPoints[1] + pos[1] * direction[1];			pos[2] = startMPoints[1] + pos[2] * direction[1];




			pts->GetPoint(pos[0], p1);							pts->GetPoint(pos[1], p2);							pts->GetPoint(pos[2], p3);

			double tmpDist = GetDistance(p1, p2);
			if (tmpDist < testDist)
			{
				testDist = tmpDist;
				bestPos = j;
			}
		}
		minPos = bestPos;
	}*/




//	if (maxVolume > 0)
	{
		vtkIdType pos[3];
		j = minPos;
		for(i=0;i<dest;i++)
		{

			pos[0]=i;
			pos[1]=(j+i)%dest;
			pos[2]=(j+i+1)%dest;

			pos[0]=startMPoints[0]+pos[0]*direction[0];
			pos[1]=startMPoints[1]+pos[1]*direction[1];
			pos[2]=startMPoints[1]+pos[2]*direction[1];

			polysFinal->InsertNextCell(3,pos);


			pos[0]=i;
			pos[1]=(j+i+1)%dest;
			pos[2]=(i+1)%dest;

			pos[0]=startMPoints[0]+pos[0]*direction[0];
			pos[1]=startMPoints[1]+pos[1]*direction[1];
			pos[2]=startMPoints[0]+pos[2]*direction[0];

			polysFinal->InsertNextCell(3,pos);

		}
	}
	
	tr->Delete();

}

//---------------------
void	SurfaceProcessing::AddTextureCoords(vtkPolyData *pData,int mode,double *params,int nrOfParams)
{
	if (pData!=NULL)
	{
					double startTexturePos[2]={0.5,0.5};
					int nr=pData->GetNumberOfPoints();
					int startPos=0;
					double scaleFactor=1;
					double rotAngle=0;
					double startVector[3]={0,0,1};
					int iterations=1;
					int norm[2]={true};
					double scaleFactors[2]={1};

					if ((params!=NULL) )//&&(nrOfParams>5))
					{
						scaleFactor=params[0];
						rotAngle=params[1]* to_rad;
						if ((params[2]>=0)&&(params[2]<nr)) startPos=params[2];
						
						startVector[0]=params[3];	startVector[1]=params[4];	startVector[2]=params[5];
						iterations=params[6];


						startTexturePos[0]=params[11];
						startTexturePos[1]=params[12];

						scaleFactors[0]=params[9];
						scaleFactors[1]=params[10];

						norm[0]=(int)params[7];
						norm[1]=(int)params[8];

					}






				vtkFloatArray *tc=vtkFloatArray::New();
				tc->SetNumberOfComponents(2);
		
					vtkDataArray *normalsT=pData->GetPointData()->GetNormals();
				vtkPoints *pts=pData->GetPoints();
				int j;
				double tuple[2];
				double tuple0[2];

				if (mode==0)
				{
					for(j=0;j<nr;j++)
					{
						tuple[0]=(rand())/(double)RAND_MAX;
						tuple[1]=(rand())/(double)RAND_MAX;
						tc->InsertNextTuple(tuple);
					}
				}
				else if (mode==1)
				{
				

					double n[3];
					for(j=0;j<nr;j++)
					{
						normalsT->GetTuple(j,n);

						tuple[0]=n[0]/2+0.5;
						tuple[1]=n[1]/2+0.5;
						tc->InsertNextTuple(tuple);
					}
				}
				else if (mode==2)
				{
					vtkDataArray *normalsT=pData->GetPointData()->GetNormals();

					double n[3];
					double p[3];
					double v[3];
					double center[3]={0};
					double angles[3];
					//vtkPoints *pts=pData->GetPoints();
					//calculate the center
					for(j=0;j<nr;j++)
					{
						
						pts->GetPoint(j,p);
						center[0]+=p[0];center[1]+=p[1];center[2]+=p[2];
						/*normalsT->GetTuple(j,n);

						tuple[0]=n[0]/2+0.5;
						tuple[1]=n[1]/2+0.5;
						tc->InsertNextTuple(tuple);*/
					}
					
					center[0]/=(double)nr;	center[1]/=(double)nr;	center[2]/=(double)nr;


					double xx[3]={1,0,0};
					double yy[3]={0,1,0};
					double zz[3]={0,0,1};

					
					for(j=0;j<nr;j++)
					{
						pts->GetPoint(j,p);
						v[0]=p[0]-center[0]; v[1]=p[1]-center[1]; v[2]=p[2]-center[2];
						normV(v);
						angles[0]=GetAngleBetweenNormalizedVectors(xx,v);
						angles[1]=GetAngleBetweenNormalizedVectors(yy,v);
						angles[2]=GetAngleBetweenNormalizedVectors(zz,v);
						
						tuple[0]=((angles[0]*angles[2])/(M_PI*M_PI))/2+0.5;
						tuple[1]=((angles[1]*angles[2])/(M_PI*M_PI))/2+0.5;
					
					//	tuple[0]=((angles[0])/(M_PI))/2+0.5;
					//	tuple[1]=((angles[1])/(M_PI))/2+0.5;

						tc->InsertNextTuple(tuple);

					}


				}
				else if (mode==3) //linear
				{

					
					vtkFloatArray *tc2=vtkFloatArray::New();
					tc2->SetNumberOfComponents(2);
					//moje bajabongo
					tc->SetNumberOfTuples(nr);

					vtkFloatArray *temptc;

					//tablica obliczonych tcoordsow
					unsigned char *checkTab=new unsigned char[nr];

					int *listOld=new int[nr];
					int *listNew=new int[nr];
					int nrOld,nrNew=0;

					


				
				
					if (iterations>1)
						tc2->SetNumberOfTuples(nr);
				/*	startTVector[0]=sin(rotAngle);
					startTVector[2]=cos(rotAngle);*/

					//zbieram sasiadow do tablicy

					int maxNeigh=500;
					
					double length,dist;
					int pos,pos1;
					double p0[3],p1[3],p2[3],p[3];
	
					int val1,val2;
					int maxDiff;
					int diff;


					vtkIdList *pointList=vtkIdList::New();
					vtkIdList *cells=vtkIdList::New();
					int i,j,k,l;
					int tempNr;
					int *tempTab,*tempTab2;
	
					int nrrr;
					vtkIdType cellId,np,np2;
					int valMax;


					double meanDiff;
	
					

	
					int neighCount=0;
					int cellsCount=0;
					int idCount=0;
					//zliczam liczbe max sasiadow
					int idNr;
					//--------------
					int maxNr=0;
					for(i=0;i<nr;i++)
					{
						pData->GetPointCells(i,cells);
						nrrr=cells->GetNumberOfIds();

						pos=i*maxNeigh;

						
						pos1=0;
					//	valMax=0;
					//	val1=
						if (nrrr>cellsCount) cellsCount=nrrr;
						
						for(j=0;j<nrrr;j++)
						{
							cellId=cells->GetId(j);
							np2=pData->GetCellType(cellId);
							if ( np2 == VTK_TRIANGLE )
							{
								pData->GetCellPoints(cellId,pointList);
								idNr=pointList->GetNumberOfIds();
								if (idNr>idCount) idCount=idNr;

								for(k=0;k<idNr;k++)
								{
									np=pointList->GetId(k);
									if (np==i) continue;
									
									pos1++;
									
									
									if (pos1>neighCount) {neighCount=pos1;maxNr=i;};
								
								}
							}
						}
						
					}

					maxNeigh=neighCount+1;
					//-------------------
					//zbieram otoczenia i max grad
					int *neighTab=new int[nr*maxNeigh];
					memset(neighTab,0,nr*maxNeigh*sizeof(int));

					bool noStartVec=true;


					//maxDiff=0;
					for(i=0;i<nr;i++)
					{
						pData->GetPointCells(i,cells);
						nrrr=cells->GetNumberOfIds();

						pos=i*maxNeigh;

						pts->GetPoint(i,p);
						pos1=0;
					//	valMax=0;
					//	val1=
						for(j=0;j<nrrr;j++)
						{
							cellId=cells->GetId(j);
							np2=pData->GetCellType(cellId);
							if ( np2 == VTK_TRIANGLE )
							{
								pData->GetCellPoints(cellId,pointList);
								for(k=0;k<pointList->GetNumberOfIds();k++)
								{
									np=pointList->GetId(k);
									if (np==i) continue;
									pts->GetPoint(np,p1);
									pos1++;
									neighTab[pos+pos1]=np;
									neighTab[pos]++;
									if (neighTab[pos]>neighCount) neighCount=neighTab[pos];
								
								}
							}
						}
						
					}
					for(i=0;i<nr;i++)
					{
						tempTab=neighTab+i*maxNeigh;
						QSortInt(tempTab,1,tempTab[0]);;
			
						tempTab[0]/=2;
						for(j=0;j<tempTab[0];j++)
						{
							tempTab[j+1]=tempTab[j*2+1];
						}
			
					}
					//mam tablice w postaci
					// 0 sasiad1, sasiad2 ...
					// 1 sasiad1, sasiad2

					tempTab=neighTab+startPos*maxNeigh;
					int neighNr=tempTab[0];
					tuple[0]=startTexturePos[0];
					tuple[1]=startTexturePos[1];
					tc->SetTuple(startPos,tuple);
					

					

					double angle;
					double v0[3];
					double v0t[3];
					double lengthT;

					nrNew=0;nrOld=1;
					double totalStart=startPos;
					
					int actPos;
					int neighNr2;
					int *tmpx;
					int iter=0;
					double minAngle=360;
					double maxAngle=0;
					double percent=1;

					double tuple2[2];


					for(int ii=0;ii<iterations;ii++)
					{
						memset(checkTab,0,nr);
						tuple[0]=startTexturePos[0];
						tuple[1]=startTexturePos[1];
						tc->SetTuple(totalStart,tuple);
						
						checkTab[startPos]=1;


						do{

							tuple[0]=startTexturePos[0];
							tuple[1]=startTexturePos[1];
							tc->SetTuple(totalStart,tuple);

						nrNew=0;nrOld=1;
						listOld[0]=totalStart;
						if (ii>0)
						{
								startVector[0]=(rand()/(double)RAND_MAX);
								startVector[1]=(rand()/(double)RAND_MAX);
								startVector[2]=(rand()/(double)RAND_MAX);
								normV(startVector);
						}
						do 
						{
							iter++;
							nrNew=0;
							
							for(k=0;k<nrOld;k++)
							{
								int pos=0;
								startPos=listOld[k];
								tempTab=neighTab+startPos*maxNeigh;
								neighNr=tempTab[0];
								pts->GetPoint(startPos,p0);

								tc->GetTuple(startPos,tuple0);
								checkTab[startPos]=2;
								for(j=0;j<neighNr;j++)
								{
									actPos=tempTab[j+1];
									if (checkTab[actPos]==0)
									{
										//policz nowa pozycje w teksturze
									
										pts->GetPoint(actPos,p);
										tc->GetTuple(startPos,tuple0);//pozycja aktualnego w teksturze
										v0[0]=p[0]-p0[0]; v0[1]=p[1]-p0[1]; v0[2]=p[2]-p0[2];
										length=sqrt(DOT(v0,v0));
										normV(v0);
										angle=GetAngleBetweenNormalizedVectors(v0,startVector);
										/*if (angle*deg<45)
										{
											angle=(90-angle*deg)*rad;
										}
										else if (angle*deg>315)
										{
											angle=(315*2-angle*2)*rad;
										}
										if (angle<0)
										{
											i=i;
										}*/
										if (angle* to_deg<minAngle) minAngle=angle* to_deg;
										if (angle* to_deg>maxAngle) maxAngle=angle* to_deg;

										double cosa,sina;
										angle+=rotAngle;
										cosa=cos(angle);
										sina=sin(angle);
										lengthT=length/scaleFactor;
										tuple[0]=(tuple0[0]+lengthT*cosa);
										tuple[1]=(tuple0[1]+lengthT*sina);

								
										if (ii>0)
										{
											//poprawiam stary+(nowy-stary)*procent
											tc2->GetTuple(actPos,tuple2);
											tuple[0]=tuple2[0]+(tuple[0]-tuple2[0])*percent;
											tuple[1]=tuple2[1]+(tuple[1]-tuple2[1])*percent;

										}
									
										tc->SetTuple(actPos,tuple);

										//sprawdze czy jego otoczenie zostalo juz sprawdzone jesli nie dodaje do newTab;
										tempTab2=neighTab+actPos*maxNeigh;
										neighNr2=tempTab2[0];
										bool test=false;
										for(l=0;l<neighNr2;l++)
										{
											if (checkTab[tempTab2[l+1]]==0)
											{
												test=true;
												checkTab[actPos]=1;
												break;
											}
										}
										if(test==true)
										{
											listNew[nrNew++]=actPos;
										}
									

									}

								}
						
							}

							if (nrNew>0)
							{
								tmpx=listNew;
								listNew=listOld;
								listOld=tmpx;
								nrOld=nrNew;
							}

						}while (nrNew>0);

						totalStart=-1;
						for(l=0;l<nr;l++)
						{
							if (checkTab[l]==0)
							{
								totalStart=l;
								break;
							}
						}
						}
						while (totalStart>-1);

						//spap tabs
						if ((iterations>1)&&(ii<iterations-1))
						{
							temptc=tc2;
							tc2=tc;
							tc=temptc;
						}
					}


					delete []checkTab;
					delete []neighTab;
					delete []listOld;
					delete []listNew;
					tc2->FastDelete();
				}
				else if (mode==4)
				{
					//moje bajabongo
					tc->SetNumberOfTuples(nr);
					//tablica obliczonych tcoordsow
					unsigned char *checkTab=new unsigned char[nr];
					double *dirVectors=new double [nr*3];
					memset(dirVectors,0,nr*sizeof(double));

					int *listOld=new int[nr];
					int *listNew=new int[nr];
					int nrOld,nrNew=0;

					memset(checkTab,0,nr);


				/*	int startPos=0;
					double startTexturePos[2]={0.5,0.5};

					double scaleFactor=0.01;
					double rotAngle=0;
					double startVector[3]={0,0,1};
				
					if ((params!=NULL) &&(nrOfParams>5))
					{
						scaleFactor=params[0];
						rotAngle=params[1]*rad;
						if ((params[2]>=0)&&(params[2]<nr)) startPos=params[2];
						
						startVector[0]=params[3];	startVector[1]=params[4];	startVector[2]=params[5];

					}*/
					

				/*	startTVector[0]=sin(rotAngle);
					startTVector[2]=cos(rotAngle);*/

					//zbieram sasiadow do tablicy

					int maxNeigh=500;
					
					double length,dist;
					int pos,pos1;
					double p0[3],p1[3],p2[3],p[3];
	
					int val1,val2;
					int maxDiff;
					int diff;


					vtkIdList *pointList=vtkIdList::New();
					vtkIdList *cells=vtkIdList::New();
					int i,j,k,l;
					int tempNr;
					int *tempTab,*tempTab2;
	
					int nrrr;
					vtkIdType cellId,np,np2;
					int valMax;


					double meanDiff;
	
					

	
					int neighCount=0;
					int cellsCount=0;
					int idCount=0;
					//zliczam liczbe max sasiadow
					int idNr;
					//--------------
					int maxNr=0;
					for(i=0;i<nr;i++)
					{
						pData->GetPointCells(i,cells);
						nrrr=cells->GetNumberOfIds();

						pos=i*maxNeigh;

						
						pos1=0;
					//	valMax=0;
					//	val1=
						if (nrrr>cellsCount) cellsCount=nrrr;
						
						for(j=0;j<nrrr;j++)
						{
							cellId=cells->GetId(j);
							np2=pData->GetCellType(cellId);
							if ( np2 == VTK_TRIANGLE )
							{
								pData->GetCellPoints(cellId,pointList);
								idNr=pointList->GetNumberOfIds();
								if (idNr>idCount) idCount=idNr;

								for(k=0;k<idNr;k++)
								{
									np=pointList->GetId(k);
									if (np==i) continue;
									
									pos1++;
									
									
									if (pos1>neighCount) {neighCount=pos1;maxNr=i;};
								
								}
							}
						}
						
					}

					maxNeigh=neighCount+1;
					//-------------------
					//zbieram otoczenia i max grad
					int *neighTab=new int[nr*maxNeigh];
					memset(neighTab,0,nr*maxNeigh*sizeof(int));

					bool noStartVec=true;


					//maxDiff=0;
					for(i=0;i<nr;i++)
					{
						pData->GetPointCells(i,cells);
						nrrr=cells->GetNumberOfIds();

						pos=i*maxNeigh;

						pts->GetPoint(i,p);
						pos1=0;
					//	valMax=0;
					//	val1=
						for(j=0;j<nrrr;j++)
						{
							cellId=cells->GetId(j);
							np2=pData->GetCellType(cellId);
							if ( np2 == VTK_TRIANGLE )
							{
								pData->GetCellPoints(cellId,pointList);
								for(k=0;k<pointList->GetNumberOfIds();k++)
								{
									np=pointList->GetId(k);
									if (np==i) continue;
									pts->GetPoint(np,p1);
									pos1++;
									neighTab[pos+pos1]=np;
									neighTab[pos]++;
									if (neighTab[pos]>neighCount) neighCount=neighTab[pos];
								
								}
							}
						}
						
					}
					for(i=0;i<nr;i++)
					{
						tempTab=neighTab+i*maxNeigh;
						QSortInt(tempTab,1,tempTab[0]);;
			
						tempTab[0]/=2;
						for(j=0;j<tempTab[0];j++)
						{
							tempTab[j+1]=tempTab[j*2+1];
						}
			
					}
					//mam tablice w postaci
					// 0 sasiad1, sasiad2 ...
					// 1 sasiad1, sasiad2

					tempTab=neighTab+startPos*maxNeigh;
					int neighNr=tempTab[0];
					tuple[0]=startTexturePos[0];
					tuple[1]=startTexturePos[1];
					tc->SetTuple(startPos,tuple);
					checkTab[startPos]=1;

					

					double angle;
					double v0[3];
					double v0t[3];
					double lengthT;

					nrNew=0;nrOld=1;
					//startPos=0;
					int totalStart=startPos;
					listOld[0]=startPos;
					int actPos;
					int neighNr2;
					int *tmpx;
					int iter=0;
					double minAngle=360;
					double maxAngle=0;
					
					bool startx=true;
				
					do{
					nrNew=0;nrOld=1;
					listOld[0]=totalStart;
						
						
					do 
					{
						iter++;
						nrNew=0;
						for(k=0;k<nrOld;k++)
						{
							int pos=0;
							startPos=listOld[k];
							tempTab=neighTab+startPos*maxNeigh;
							neighNr=tempTab[0];
							pts->GetPoint(startPos,p0);

							tc->GetTuple(startPos,tuple0);
							checkTab[startPos]=2;
							for(j=0;j<neighNr;j++)
							{
								actPos=tempTab[j+1];
								if (checkTab[actPos]==0)
								{
									//policz nowa pozycje w teksturze
									
									if (startx==false)
									{
										startVector[0]=dirVectors[startPos*3];
										startVector[1]=dirVectors[startPos*3+1];
										startVector[2]=dirVectors[startPos*3+2];

									}

									pts->GetPoint(actPos,p);
									tc->GetTuple(startPos,tuple0);//pozycja aktualnego w teksturze
									v0[0]=p[0]-p0[0]; v0[1]=p[1]-p0[1]; v0[2]=p[2]-p0[2];
									length=sqrt(DOT(v0,v0));
									normV(v0);
									angle=GetAngleBetweenNormalizedVectors(v0,startVector);
								
									if (angle* to_deg<minAngle) minAngle=angle* to_deg;
									if (angle* to_deg>maxAngle) maxAngle=angle* to_deg;

									double cosa,sina;
									angle+=rotAngle;
									cosa=cos(angle);
									sina=sin(angle);
									lengthT=length/scaleFactor;
									tuple[0]=(tuple0[0]+lengthT*cosa);
									tuple[1]=(tuple0[1]+lengthT*sina);

									
									tc->SetTuple(actPos,tuple);
									dirVectors[actPos*3]=v0[0];
									dirVectors[actPos*3+1]=v0[1];
									dirVectors[actPos*3+2]=v0[2];

									//sprawdze czy jego otoczenie zostalo juz sprawdzone jesli nie dodaje do newTab;
									tempTab2=neighTab+actPos*maxNeigh;
									neighNr2=tempTab2[0];
									bool test=false;
									for(l=0;l<neighNr2;l++)
									{
										if (checkTab[tempTab2[l+1]]==0)
										{
											test=true;
											checkTab[actPos]=1;
											break;
										}
									}
									if(test==true)
									{
										listNew[nrNew++]=actPos;
									}
									

								}

							}
							if (startx==true) startx=false;
						/*	for(j=0;j<nr;j++)
							{
								if (checkTab[j]==1) checkTab[j]=0;
							}*/
						}

						if (nrNew>0)
						{
							tmpx=listNew;
							listNew=listOld;
							listOld=tmpx;
							nrOld=nrNew;
						}

					}while (nrNew>0);
						totalStart=-1;
						for(l=0;l<nr;l++)
						{
							if (checkTab[l]==0)
							{
								totalStart=l;
								break;
							}
						}
						}
						while (totalStart>-1);

					delete []checkTab;
					delete []neighTab;
					delete []listOld;
					delete []listNew;
					delete []dirVectors;
				}
				else if (mode==5)
				{
					//dihedral test
					tc->SetNumberOfTuples(nr);
					int nr=pData->GetNumberOfCells();
					
					vtkIdList *pointList=vtkIdList::New();
					vtkIdList *cells=vtkIdList::New();
					int i,j,k,l,m;
					double length,dist;
					int pos,pos1;
					double p0[3],p1[3],p2[3],p[3];
	
					int val1,val2;
					int maxDiff;
					int diff;
					int *listOld=new int[nr];
					int *listNew=new int[nr];
					int nrOld,nrNew=0;
					double startTexturePos[2]={0.5,0.5};
					unsigned char *checkTab=new unsigned char[nr];
					memset(checkTab,0,nr);

					//double scaleFactor=0.01;
					//double rotAngle=0;
					//double startVector[3]={0,0,1};
					//int startPos=0;
					//if ((params!=NULL) &&(nrOfParams>5))
					//{
					//	scaleFactor=params[0];
					//	rotAngle=params[1]*rad;
					//	if ((params[2]>=0)&&(params[2]<nr)) startPos=params[2];
					//	
					//	startVector[0]=params[3];	startVector[1]=params[4];	startVector[2]=params[5];

					//}

					int totalStart=startPos;
					listOld[0]=startPos;

					nrNew=0;
					
					//lista sasiadujacych trojkatow



					vtkIdType Triangle[3];
					vtkGenericCell		*triangleCell=vtkGenericCell::New();
					int *neighborTab=new int[nr*3];
				//	int *triangleTab=new int[nr*3];
					
					//memset(neighborTab,0,nr*3*sizeof(int));
					for(i=0;i<nr*3;i++)
					{
						neighborTab[i]=-1;
					}
				

					//memset(triangleTab,0,nr*3*sizeof(int));
					pos=0;
					//zbieram wszystkie trojkaty i ich sasiadow do tablicy
					/*for(i=0;i<nr;i++)
					{
						pData->GetCell(i,triangleCell);
						for(j=0;j<3;j++)
							triangleTab[pos+j]=triangleCell->GetPointId(j);

						pos+=3;
					}*/
					//zebrane

					// teraz poszukam sasiadow kazdego trojkata
					

					int nrOfNeighbors;
					int pos2;
					int tmp2;
					int tmp=0;
					pos=0;
						//vtkIdList *pointList=vtkIdList::New();
						int triangle[3];
						pData->BuildLinks();
					for(i=0;i<nr;i++)
					{
						pData->GetCell(i,triangleCell);
						nrOfNeighbors=0;
						for(j=0;j<3;j++)	triangle[j]=triangleCell->GetPointId(j);
					
						pData->GetCellEdgeNeighbors(i,triangle[0],triangle[1],pointList);
						tmp=pointList->GetNumberOfIds();
						if (tmp>0)				{				neighborTab[pos]=pointList->GetId(0);					}

						pData->GetCellEdgeNeighbors(i,triangle[0],triangle[2],pointList);						tmp=pointList->GetNumberOfIds();
						if (tmp>0)				{				neighborTab[pos+1]=pointList->GetId(0);					}

						pData->GetCellEdgeNeighbors(i,triangle[1],triangle[2],pointList);						tmp=pointList->GetNumberOfIds();
						if (tmp>0)				{				neighborTab[pos+2]=pointList->GetId(0);					}


//						for(j=0;j<nr;j++)
//						{
//
//							if (i!=j)
//							{
//								tmp2=0;
//								//porownuje wspolne punkty 2tr
//								for(k=0;k<3;k++)
//									for(l=0;l<3;l++)
//										if (triangleTab[pos+k]==triangleTab[pos2+l]) tmp2++;
//
//								if (tmp2==2) 
//								{
//									neighborTab[tmp++]=j;//sasiad!
//									nrOfNeighbors++;
//									
//									
//								}
//								if (nrOfNeighbors==3)
//								{	
//									break; //ten sam trojkat? moze byc jak popsuty mesh ze trojkat sie powtarza
//								}
//							}
//
//							/*if (nrOfNeighbors<3)
//							{
//								for (k=0;k<3-nrOfNeighbors;k++)
//								{
//									neighborTab[tmp++]=-1;
//
//								}
//							}
//*/
//
//
//							pos2+=3;
//						}
						pos+=3;

					}







					int *tmpx;

					int *tempTab;
					vtkIdType cellId,np,np2;
					int actPos;
					int *tempTab2;
					int neighNr2;
					int ppos[3],ppos1[3];

					double v1[3],v2[3];
					double l1,l2;
					double angle;
					double lengthT;
					//dodaj pierwsze 3pkty!!!
					// do tupli


	skeletLine=vtkCellArray::New();
	vtkIdType newCellPts[2];
	
	skelPts=vtkPoints::New();
	skelPts->SetNumberOfPoints(pData->GetNumberOfPoints());
	double p00[3]={0};
	for(i=0;i<pData->GetNumberOfPoints();i++)
	{
		skelPts->SetPoint(i,p00);
	}

					
					double mp0[3]={0};
					double mp1[3]={0};
					double mp2[3]={0};
					//startPos=listOld[k];
					tempTab=neighborTab+startPos*3;
					double cosa,sina;
					double scaleInv=1.0/scaleFactor;
					checkTab[startPos]=2;
					//cellId=cells->GetId(startPos);
					//np2=pData->GetCellType(start);
					pData->GetCellPoints(startPos,pointList);
					ppos[0]=pointList->GetId(0);				ppos[1]=pointList->GetId(1);				ppos[2]=pointList->GetId(2);

					pts->GetPoint(ppos[0],p0);					pts->GetPoint(ppos[1],p1);					pts->GetPoint(ppos[2],p2);

					//zerowy punkty 
					tuple0[0]=startTexturePos[0];				tuple0[1]=startTexturePos[1];
					tc->SetTuple(ppos[0],tuple0);

	mp0[0]=tuple0[0];mp0[1]=tuple0[1];
	skelPts->SetPoint(ppos[0],mp0);
			
			
			
			//obliczam pierwszy i drugi

					v1[0]=p1[0]-p0[0]; v1[1]=p1[1]-p0[1]; v1[2]=p1[2]-p0[2];	l1=sqrt(DOT(v1,v1));
					v2[0]=p2[0]-p0[0]; v2[1]=p2[1]-p0[1]; v2[2]=p2[2]-p0[2];	l2=sqrt(DOT(v2,v2));

					normV(v1); normV(v2);

					angle=GetAngleBetweenNormalizedVectors(v1,v2);
					//tylko dla pierwszego!
					angle+=rotAngle;
					cosa=cos(rotAngle);
					sina=sin(rotAngle);

					l1=1;
					l2=1;
					angle=60* to_rad;

					lengthT=l1*scaleInv;
					tuple[0]=(tuple0[0]+lengthT*cosa);			tuple[1]=(tuple0[1]+lengthT*sina);
					tc->SetTuple(ppos[1],tuple);

	mp1[0]=tuple[0];mp1[1]=tuple[1];
	skelPts->SetPoint(ppos[1],mp1);
	newCellPts[0]=ppos[0];			newCellPts[1]=ppos[1];
	skeletLine->InsertNextCell(2,newCellPts);
					//drugi
					cosa=cos(angle);
					sina=sin(angle);
					lengthT=l2*scaleInv;
					tuple[0]=(tuple0[0]+lengthT*cosa);			tuple[1]=(tuple0[1]+lengthT*sina);
					tc->SetTuple(ppos[2],tuple);

	mp2[0]=tuple[0];mp2[1]=tuple[1];
	skelPts->SetPoint(ppos[2],mp2);
	newCellPts[0]=ppos[0];			newCellPts[1]=ppos[2];
	skeletLine->InsertNextCell(2,newCellPts);


	newCellPts[0]=ppos[1];			newCellPts[1]=ppos[2];
	skeletLine->InsertNextCell(2,newCellPts);
	gCounter=0;

					int id0,id1,idlast,idnew;
					int test;
					nrOld=1;
					int iTest;

				
					//int ttt=2;
						do{
						nrNew=0;nrOld=1;
						listOld[0]=totalStart;
						
					do 
					{
						nrNew=0;
					
						for(k=0;k<nrOld;k++)
						{
							int pos=0;
							startPos=listOld[k];
							tempTab=neighborTab+startPos*3;

							checkTab[startPos]=2;
							//cellId=cells->GetId(startPos);
							//np2=pData->GetCellType(cellId);
							pData->GetCellPoints(startPos,pointList);
							ppos[0]=pointList->GetId(0);
							ppos[1]=pointList->GetId(1);
							ppos[2]=pointList->GetId(2);

							/*np=pointList->GetId(k);
									if (np==i) continue;
									pts->GetPoint(np,p1);*/
							//punkty 

							//

							

							for(j=0;j<3;j++)
							{
								actPos=tempTab[j];
								if (actPos==-1) break; //brak sasiada
								if (checkTab[actPos]==0)
								{
									// jesli juz nie byl analizowany
									//dodaj nowa pozycje tupla
								//	cellId=cells->GetId(actPos);
								//	np2=pData->GetCellType(cellId);
									pData->GetCellPoints(actPos,pointList);
									ppos1[0]=pointList->GetId(0);
									ppos1[1]=pointList->GetId(1);
									ppos1[2]=pointList->GetId(2);

									//szukam punktu z nowego trojkata ktory nie nalezy do startpoosa
									id0=-1;id1=-1;idlast=-1;
									for(l=0;l<3;l++)
									{
										iTest=0;
										for(m=0;m<3;m++)
										{
											if (ppos[l]==ppos1[m])
											{
												if (id0==-1) id0=ppos[l];
												else 		id1=ppos[l];
											}
											else
											{
												iTest++;
											}
											if(iTest==3) idlast=ppos[l];
											
										}
										/*if (iTest==3)
											break;*/
									}

									//dodam jeszcze puntk z nowego trojkata niepowtarrzalny
									for(l=0;l<3;l++)
									{
										if ((ppos1[l]!=id0)&&(ppos1[l]!=id1))
										{
											idnew=ppos1[l];
											break;
										}
									}
									
									if(l==3)
									{
										i=i;
									}

						

									pts->GetPoint(id0,p0);					pts->GetPoint(id1,p1);					pts->GetPoint(idnew,p2);
									v1[0]=p1[0]-p0[0]; v1[1]=p1[1]-p0[1]; v1[2]=p1[2]-p0[2];	l1=sqrt(DOT(v1,v1))*scaleInv;
									v2[0]=p2[0]-p0[0]; v2[1]=p2[1]-p0[1]; v2[2]=p2[2]-p0[2];	l2=sqrt(DOT(v2,v2))*scaleInv;

									normV(v1); normV(v2);

									angle=GetAngleBetweenNormalizedVectors(v1,v2);
									//sprawdzam, ktory punkt z listy ppos1 jest rozny od ppos
									//dla tego punktu wyliczam pozycje nowa i wstawiam nowego tupla do listy

									
									AddTupleForNeighborTriangle(tc,id0,id1,idlast,idnew, l2, angle);



									//brakuje tego 
									tempTab2=neighborTab+actPos*3;
									
									bool test=false;
									for(l=0;l<3;l++)
									{
										if (checkTab[tempTab2[l]]==0)
										{
											test=true;
											checkTab[actPos]=1;
											break;
										}
									}
									if(test==true)
									{
										listNew[nrNew++]=actPos;
									}
								}

							}

						}

						if (nrNew>0)
						{
							tmpx=listNew;
							listNew=listOld;
							listOld=tmpx;
							nrOld=nrNew;
						}
						//ttt--;
					}while (nrNew>0);//(ttt>0);//(!(nrNew>0));
						totalStart=-1;
						for(l=0;l<nr;l++)
						{
							if (checkTab[l]==0)
							{
								totalStart=l;
								break;
							}
						}
						}
						while (totalStart>-1);
					// biore pierwszy trojkat - mapuje go zgodnie ze skala
					// biore jego sasiadow i odginam na plaszczyzne - ignoruje katy  tylko odginam narazie 
					// dodam model 3d tej mapy zrzutowanych trojkatow zeby zobaczyc czy kicha

					delete []checkTab;
					delete []listOld;
					delete []listNew;
					delete []neighborTab;

					vtkPolyData *skelData=vtkPolyData::New();
								skelData->SetPoints(skelPts);
								skelData->SetLines(skeletLine);
								skelData->SetPolys(skeletLine);
								skelData->BuildCells();
//								skelData->Update();


					vtkPolyData *pp=vtkPolyData::New();	pp->DeepCopy(skelData);	/*pp->Update();*/
					PolygonSurfaceData *o2=new PolygonSurfaceData(pp,VWidget);	o2->SetPolygonSurfaceDataSetName("Jezyk ");	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
					o2->SetGroupName("hedgehog");
					emit	SignalSendNewSurface(o2);


					//delete []triangleTab;
				}
				else if (mode==6)//cylinder
				{
					vtkSmartPointer<vtkTextureMapToCylinder> tmap = vtkSmartPointer<vtkTextureMapToCylinder>::New();
					tmap->AutomaticCylinderGenerationOn	();
					tmap->SetInputData(pData);
					tmap->Update();
				
					tc->DeepCopy(tmap->GetOutput()->GetPointData()->GetTCoords());

				}
				else if (mode==7)//plane
				{
					double startVector[3]={0,0,1};
					double scaleFactor=1.0;
					double rotAngle=0;
					int startPos=0;
					int nr=pData->GetNumberOfCells();

					
					if ((params!=NULL) &&(nrOfParams>5))
					{
						scaleFactor=params[0];
						rotAngle=params[1]* to_rad;
						if ((params[2]>=0)&&(params[2]<nr)) startPos=params[2];
						
						startVector[0]=params[3];	startVector[1]=params[4];	startVector[2]=params[5];

					}
					double p0[3],p1[3],p2[3],n[3];
					double scaleInv=1.0/scaleFactor;
					double v1[3],v2[3];
					
					int id0,id1,id2;
					vtkGenericCell		*triangleCell=vtkGenericCell::New();
					pData->GetCell(startPos,triangleCell);

					double l1,l2;
					
					//pData->GetPoint(startPos,startPoint);

					vtkSmartPointer<vtkTextureMapToPlane> tmap = vtkSmartPointer<vtkTextureMapToPlane>::New();
					

					int nrc=triangleCell->GetNumberOfPoints();

					if(nrc==3)
					{

						tmap->AutomaticPlaneGenerationOff	();
						id0=triangleCell->GetPointId(0);
						id1=triangleCell->GetPointId(1);
						id2=triangleCell->GetPointId(2);

						pData->GetPoint(id0,p0);
						pData->GetPoint(id1,p1);
						pData->GetPoint(id2,p2);
						vtkDataArray *arraya=pData->GetPointData()->GetNormals();
						if (arraya!=NULL)
						{
							arraya->GetTuple(id1,n);
						
						}
						else
						{
							n[0]=startVector[0];	n[1]=startVector[1];	n[2]=startVector[2];
						}

						v1[0]=p1[0]-p0[0]; v1[1]=p1[1]-p0[1]; v1[2]=p1[2]-p0[2];	l1=sqrt(DOT(v1,v1));
						v2[0]=p2[0]-p0[0]; v2[1]=p2[1]-p0[1]; v2[2]=p2[2]-p0[2];	l2=sqrt(DOT(v2,v2));
						

						normV(v1);
						normV(v2);
						v1[0]*=scaleInv;v1[1]*=scaleInv;v1[2]*=scaleInv;
						v2[0]*=scaleInv;v2[1]*=scaleInv;v2[2]*=scaleInv;

						p1[0]=p0[0]+v1[0];p1[1]=p0[1]+v1[1];p1[2]=p0[2]+v1[2];
						p2[0]=p0[0]+v2[0];p2[1]=p0[1]+v2[1];p2[2]=p0[2]+v2[2];

						tmap->SetOrigin(p0);
						tmap->SetPoint1(p1);
						tmap->SetPoint2(p2);
						tmap->SetNormal(n);
					}
					else
					{
						tmap->AutomaticPlaneGenerationOn();
					}


					tmap->SetInputData(pData);
					tmap->SetNormal(startVector);
				//	tmap->SetOrigin(startPoint);
					tmap->Update();
					tc->DeepCopy(tmap->GetOutput()->GetPointData()->GetTCoords());


				}
				else if (mode==8)//sphere
				{
					vtkSmartPointer<vtkTextureMapToSphere> tmap = vtkSmartPointer<vtkTextureMapToSphere>::New();
					tmap->AutomaticSphereGenerationOn	();
					tmap->SetInputData(pData);
					tmap->Update();
					tc->DeepCopy(tmap->GetOutput()->GetPointData()->GetTCoords());

				}
				else if (mode==9)
				{
				
					vtkSmartPointer<vtkTriangularTCoords> tcoords = vtkSmartPointer<vtkTriangularTCoords>::New();
					tcoords->SetInputData(pData);
					tcoords->Update();
					tc->DeepCopy(tcoords->GetOutput()->GetPointData()->GetTCoords());
				

				}
				
				//------------------------
				double min[2]={INT_MAX};
				double max[2]={-INT_MAX};
				int i;
				double p[2];
				for(i=0;i<tc->GetNumberOfTuples();i++)
				{
					tc->GetTuple(i,p);
					if(p[0]<min[0]) min[0]=p[0];
					if(p[1]<min[1]) min[1]=p[1];
					if(p[0]>max[0]) max[0]=p[0];
					if(p[1]>max[1]) max[1]=p[1];
					if (p[0]<-1000)
					{
						i=i;
					}
				}

				double size[2];
				size[0]=fabs(max[0]-min[0]);
				size[1]=fabs(max[1]-min[1]);
				if (size[0]>0) size[0]=1.0/size[0];
				if (size[1]>0) size[1]=1.0/size[1];

				double min2[2]={INT_MAX};
				double max2[2]={-INT_MAX};
				for(i=0;i<tc->GetNumberOfTuples();i++)
				{
					tc->GetTuple(i,p);
					//p[0]-=startTexturePos[0];
					//p[1]-=startTexturePos[1];


					////skalowanie i normalizacja
					//

					//p0[

					//p[0]+=startTexturePos[0];
					//p[1]+=startTexturePos[1];

					if (norm[0]==1) p[0]=(p[0]-min[0])*size[0];
					if (norm[1]==1) p[1]=(p[1]-min[1])*size[1];


					p[0]-=startTexturePos[0];
					p[1]-=startTexturePos[1];

					p[0]*=scaleFactors[0];
					p[1]*=scaleFactors[1];

					p[0]+=startTexturePos[0];
					p[1]+=startTexturePos[1];
					
					if(p[0]<min2[0]) min2[0]=p[0];
					if(p[1]<min2[1]) min2[1]=p[1];
					if(p[0]>max2[0]) max2[0]=p[0];
					if(p[1]>max2[1]) max2[1]=p[1];
					tc->SetTuple(i,p);

				}


				//------------------------

				
				pData->GetPointData()->SetTCoords(tc);
				vtkDataArray * tcc=pData->GetPointData()->GetTCoords();
				if (tcc!=NULL)
				{
					tcc->DeepCopy(tc);
					pData->GetPointData()->SetTCoords(tcc);
//					//pData->Update();
						
				}
				else
				{
					pData->GetPointData()->SetTCoords(tc);
				}

				tc->FastDelete();
			
	}
}
//--	
void	SurfaceProcessing::AddTupleForNeighborTriangle(vtkFloatArray *tc,int id0,int id1, int idlast,int idnew, double length,double alpha)
{

	int nr=tc->GetNumberOfTuples();

	int maxx=MAX3(id0,id1,idlast);
	if ((maxx<0)||(maxx>=nr)) return;

	//pobierz pozycje tupli dla id0 i id1
	//musze dodac punkt po tej stronie linii gdzie nie lezy ten trzeci
	//tzn. musze tak dobrac orientacje alpha
	double p[2],p0[2],p1[2],plast[2];
	tc->GetTuple(id0,p0);
	tc->GetTuple(id1,p1);
	tc->GetTuple(idlast,plast);
	//po znalezieniu uzupelniam odpowiedniego tupla

	length=sqrt((p0[0]-p1[0])*(p0[0]-p1[0])+(p0[1]-p1[1])*(p0[1]-p1[1]))/2;
	alpha=3* to_rad;

	double mp0[3]={0};
	double mp1[3]={0};
	double mp[3]={0};
	
	double v[2]={p1[0]-p0[0],p1[1]-p0[1]};
	normV(v);
	v[0]*=length;
	v[1]*=length;
	if (gCounter==5)
	{
		alpha=alpha;
	}
	int sign=ChceckSideOfPointAndLine(p0,p1,plast);
	int sign2;
	//if (sign!=0)
	{
		//sign oznacza ze trzeci punkt lezy albo po lewej str prostej albo po prawej

		//teraz wylicze nowa pozycje punktu na podst len i alpha
		double ca=cos(alpha);
		double sa=sin(alpha);
		p[0]=p0[0]+ca*v[0]+sa*v[1];
		p[1]=p0[1]-sa*v[0]+ca*v[1];
		sign2=ChceckSideOfPointAndLine(p0,p1,p);

		//musza lezec po roznych stronach - jesli nie zmieniam znaku

		if(sign==sign2)
		{
			alpha=-alpha;
			ca=cos(alpha);
			sa=sin(alpha);
			p[0]=p0[0]+ca*v[0]+sa*v[1];
			p[1]=p0[1]-sa*v[0]+ca*v[1];		
		}

	//if(gCounter<9)
	{
	mp0[0]=p0[0];mp0[1]=p0[1];
	mp1[0]=p1[0];mp1[1]=p1[1];
	mp[0]=p[0];mp[1]=p[1];
	
	
	vtkIdType newCellPts[2];
	skelPts->SetPoint(idnew,mp);
	
	newCellPts[0]=id0;			newCellPts[1]=idnew;
	skeletLine->InsertNextCell(2,newCellPts);
	newCellPts[0]=id1;			newCellPts[1]=idnew;
	skeletLine->InsertNextCell(2,newCellPts);
	gCounter++;
	}
	

		tc->SetTuple(idnew,p);

	}


}
//-------------------------------------------
void	SurfaceProcessing::GenerateNormalsPolyData( vtkPolyData *pData, double scale, QString name)
{

	//zrob normalne
	if (pData!=NULL)
	{
	
		vtkSmartPointer<vtkHedgeHog >glyph=vtkSmartPointer<vtkHedgeHog>::New();
		glyph->SetInputData (pData);
		glyph->SetVectorModeToUseNormal();
		glyph->SetScaleFactor (scale);
		glyph->Update();

		vtkPolyData *pp=vtkPolyData::New();	pp->DeepCopy(glyph->GetOutput());	/*pp->Update();*/
		PolygonSurfaceData *o2=new PolygonSurfaceData(pp,VWidget);	o2->SetPolygonSurfaceDataSetName(name+"_n");	
		o2->GetVActor()->GetProperty()->SetColor(0,0,0);
		o2->SetGroupName("normals");
	
		o2->ComputeGeometricOrigin();

		emit	SignalSendNewSurface(o2);
	}
}