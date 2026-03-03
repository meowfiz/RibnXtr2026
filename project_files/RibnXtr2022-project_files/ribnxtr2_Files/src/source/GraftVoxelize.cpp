#include "GraftVoxelize.h"
//--------------------------------
//--------------------------------
//--------------------------------
GraftVoxelize::GraftVoxelize()
{

}
//----------------------------------------------
GraftVoxelize::~GraftVoxelize()
{

}
//-----------------------------------------------
vtkPolyData	*GraftVoxelize::CloseGraftSurface( vtkPolyData *Input, int percentage, double precision, int *NumberOfPlanes,double *PlanePoints)
{
	/*
	int i,j,k,l;

	vtkPolyData *TempBoundary=NULL;

	LinkedList<BoundaryElement*>	*BoundaryList;
	BoundaryList=new LinkedList<BoundaryElement*>;
	vtkPolyData *OutputBoundary=vtkPolyData::New();
	if (GetSortedBoundaryList(BoundaryList,OutputBoundary, Input)==false) return NULL;	
	int *LinePointIndexArray=NULL;
	vtkPoints *pPointsX=OutputBoundary->GetPoints();

	PlaneObject *MegaPlane=new PlaneObject();	
	double Point[3],Point0[3],Point1[3],Point2[3];
	int indexX;
	double PlaneDist;
	vtkPoints *pPoints1;
	vtkPolyData *Boundary=vtkPolyData::New();
	//	vtkPolyData *Boundary=Input;

	vtkPolyData *BoundaryPart;

	bool AllSurfacesCreated=false;
	int NR=0;

	int ActualPos=0;

	int FirstStep,SecondStep;


	int step;
	double Angle;
	int aa1,aa2,aa3;

	QString text;
	precision=0.0001;
	///////////////////////////////////////////////

	int *CopyOfLinePointIndexArray;
	int *CopyOfLinePointIndexArray2;
	int *LinePointIndexArray2;

	//QMessageBox::about( 0,"Number of boundaries",QString::number(BoundaryList->GetNumberOfItems()));


	int NumberOfPoints;
	int p1,p2,p3;

	int max;
	int actualmax;
	int maxpos;
	int tempstep;


	for (l=0;l<BoundaryList->GetNumberOfItems();l++)
	{
	LinePointIndexArray=BoundaryList->GetItemAtPos(l)->GetData()->PointIndex;
	NumberOfPoints=BoundaryList->GetItemAtPos(l)->GetData()->NumberOfPoints;


	CopyOfLinePointIndexArray=new int[NumberOfPoints];
	LinePointIndexArray2=new int[NumberOfPoints];
	for (i=0;i<NumberOfPoints;i++)
	{
	CopyOfLinePointIndexArray[i]=LinePointIndexArray2[i]=LinePointIndexArray[i];

	}
	/////////////
	//////////////////////////////////////////////////////////////
	step=1;
	FirstStep=step;
	SecondStep=2*step;
	ActualPos=0;

	max=0;
	actualmax=0;
	maxpos=1;
	//(rand())/32768.0

	int stepolek=0;
	do
	{
	text="";
	indexX=-1;


	// sprawdzam wszystkie trojki i wybieram maxa

	max=0;maxpos=0;
	tempstep=1;

	for (k=1;k<10;k++)
	{
	step=k;
	FirstStep=step;
	SecondStep=2*step;

	for (i=0;i<NumberOfPoints;i++)
	{

	p1=i%NumberOfPoints;
	p2=(i+FirstStep)%NumberOfPoints;
	p3=(i+SecondStep)%NumberOfPoints;

	//zrob plane
	actualmax=0;

	aa1=LinePointIndexArray2[p1];
	aa2=LinePointIndexArray2[p2];
	aa3=LinePointIndexArray2[p3];

	// tu trzeba zmienic inne pozycje punktow sprawdzanej plaszczyzny!
	pPointsX->GetPoint(aa1,Point0);
	pPointsX->GetPoint(aa2,Point1);
	pPointsX->GetPoint(aa3,Point2);

	Angle=MegaPlane->SetPlanePoints(Point1,Point0,Point2);

	// sprawdz ile pktow nalezy jak max to aktualna proba

	if((Angle>5)&&(Angle<175))
	{
	for (j=0;j<NumberOfPoints;j++)
	{
	pPointsX->GetPoint(LinePointIndexArray2[j],Point);
	PlaneDist=MegaPlane->CheckPoint(Point);
	if ((fabs(PlaneDist)<precision))
	actualmax++;

	}
	}

	if (actualmax>max)
	{
	max=actualmax;
	maxpos=i;
	tempstep=k;		
	}
	}

	}

	step=tempstep;
	FirstStep=step;
	SecondStep=2*step;

	p1=maxpos%NumberOfPoints;
	p2=(maxpos+FirstStep)%NumberOfPoints;
	p3=(maxpos+SecondStep)%NumberOfPoints;

	aa1=LinePointIndexArray2[p1];
	aa2=LinePointIndexArray2[p2];
	aa3=LinePointIndexArray2[p3];
	pPointsX->GetPoint(aa1,Point0);
	pPointsX->GetPoint(aa2,Point1);
	pPointsX->GetPoint(aa3,Point2);
	Angle=MegaPlane->SetPlanePoints(Point0,Point1,Point2);

	pPoints1=vtkPoints::New();



	for (j=0;j<NumberOfPoints;j++)
	{	

	pPointsX->GetPoint(LinePointIndexArray2[j],Point);
	PlaneDist=MegaPlane->CheckPoint(Point);
	if ((fabs(PlaneDist)<precision))
	{
	indexX++;
	pPoints1->InsertPoint(indexX,Point);
	CopyOfLinePointIndexArray[j]=-10;

	}

	}

	ActualPos=maxpos;



	///////////////////////////////
	text+=QString::number(indexX)+ "\n"; 
	//QMessageBox::about( 0,"DICOM",text);



	if (indexX>(int)(NumberOfPoints*percentage/100.0))
	{
	BoundaryPart=SortedPlanarBoundaryPointsToPolyDataSurface(pPoints1);

	TempBoundary=AppendPolyData(Boundary,BoundaryPart);

	Boundary->Delete();

	Boundary=TempBoundary;
	pPoints1->Delete();

	for (i=0;i<3;i++)
	{
	PlanePoints[NR*9+i]=Point0[i];
	PlanePoints[NR*9+3+i]=Point1[i];
	PlanePoints[NR*9+6+i]=Point2[i];		
	}

	NR++;

	for (i=0;i<NumberOfPoints;i++)
	if (CopyOfLinePointIndexArray[i]==-10) CopyOfLinePointIndexArray[i]=-1;

	}
	else
	{
	for (i=0;i<NumberOfPoints;i++)
	if (CopyOfLinePointIndexArray[i]==-10) CopyOfLinePointIndexArray[i]=10;

	CopyOfLinePointIndexArray[ActualPos]=-1;
	pPoints1->Delete();
	}


	step=1;
	FirstStep=step;
	SecondStep=2*step;


	for (i=0;i<NumberOfPoints;i++)
	if (CopyOfLinePointIndexArray[i]!=-1)
	break;

	if (i==NumberOfPoints)	AllSurfacesCreated=true;


	// skracam tablice o to co zostalo juz zuzyte
	if (AllSurfacesCreated==false)
	{
	CopyOfLinePointIndexArray2=new int[NumberOfPoints];

	int posX=0;
	for (i=0;i<NumberOfPoints;i++)
	{
	if (CopyOfLinePointIndexArray[i]!=-1)
	{

	CopyOfLinePointIndexArray2[posX]=LinePointIndexArray2[posX]=LinePointIndexArray2[i];
	posX++;
	}
	else if ((CopyOfLinePointIndexArray[abs(i-1)%NumberOfPoints]!=-1)||(CopyOfLinePointIndexArray[(i+1)%NumberOfPoints]!=-1))
	{

	// brzegowe tez zostaw
	CopyOfLinePointIndexArray2[posX]=LinePointIndexArray2[posX]=LinePointIndexArray2[i];
	posX++;
	}
	}

	int *tempol=CopyOfLinePointIndexArray;
	CopyOfLinePointIndexArray=CopyOfLinePointIndexArray2;
	delete []tempol;
	CopyOfLinePointIndexArray2=NULL;

	NumberOfPoints=posX;
	}	

	stepolek++;
	}
	while ((AllSurfacesCreated!=true)&&stepolek<9);



	//////////////////////////////////////////////////////////////////
	//reszta

	delete []CopyOfLinePointIndexArray;

	delete []LinePointIndexArray2;


	}

	delete BoundaryList;
	*NumberOfPlanes=NR;
	return TempBoundary;
	*/
	return NULL;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
RawDataSet		*GraftVoxelize::CreateVolumeFormSurface(QString filename, vtkPolyData *Input,RawDataSet *Data)
{

	QCursor cursor;
	cursor.setShape(Qt::WaitCursor);

	QApplication::setOverrideCursor(cursor);

	int i,j,k;
	vtkOBBTree *obbTree=vtkOBBTree::New();


	int *NumberOfPlanes=new int;
	double *PlanePoints=new double[9*30];


	obbTree->SetDataSet(CloseGraftSurface( Input,1,0.00007,NumberOfPlanes,PlanePoints));
	obbTree->SetTolerance(0.0000001);
	obbTree->BuildLocator();

	vtkPolyData *obb=vtkPolyData::New();

	obbTree->GenerateRepresentation(-10,obb);

	double pPoint1[3];
	vtkPoints *pPoints1=	Input->GetPoints();			
	int NPoint=pPoints1->GetNumberOfPoints();

	double min[3],max[3];
	min[0]=10000;
	min[1]=10000;
	min[2]=10000;
	max[0]=-10000;
	max[1]=-10000;
	max[2]=-10000;
	double tempA[3];

	//Find Center of the objectus
	for (i=0;i<NPoint;i++)
	{
		pPoints1->GetPoint(i,pPoint1);

		tempA[0]=pPoint1[0];
		tempA[1]=pPoint1[1];
		tempA[2]=pPoint1[2];

		if (tempA[0]>max[0]) max[0]=tempA[0];
		if (tempA[1]>max[1]) max[1]=tempA[1];
		if (tempA[2]>max[2]) max[2]=tempA[2];
		if (tempA[0]<min[0]) min[0]=tempA[0];
		if (tempA[1]<min[1]) min[1]=tempA[1];
		if (tempA[2]<min[2]) min[2]=tempA[2];
	}




	//---------------------------------------------------------------
	//---------------------------------------------
	//-------------------------------------------

	//tworzy zbiorek
	miX=Round(min[0]/Data->GetXscale());
	int maX=Round(max[0]/Data->GetXscale());
	miY=Round(min[1]/Data->GetYscale());
	int maY=Round(max[1]/Data->GetYscale());
	miZ=Round(min[2]/Data->GetZscale());
	int maZ=Round(max[2]/Data->GetZscale());


	RawDataSet *DData = new RawDataSet(maX-miX+1,maY-miY+1,maZ-miZ+1,Data->GetXscale(),Data->GetYscale(),Data->GetZscale(),0);
	DData->ZeroQ();

	//	FILE *Do_opa;


	double Poincik[3];
	// to jest tandetna i brudna sztuczka - michale popraw to jak to zrozumiesz qrde!
	int pomocy=0;

	for (i=miX;i<=maX;i=i+maX-miX)
		for (j=miY;j<=maY;j=j+maY-miY)	
			for (k=miZ;k<=maZ;k=k+maZ-miZ)
			{
				Poincik[0]=i*Data->GetXscale();
				Poincik[1]=j*Data->GetYscale();
				Poincik[2]=k*Data->GetZscale();

				if(obbTree->InsideOrOutside(Poincik)==-1) pomocy++;
			}





			//

			for (i=miX;i<=maX;i++)
			{
				for (j=miY;j<=maY;j++)	
					for (k=miZ;k<=maZ;k++)
			  {
				  Poincik[0]=i*Data->GetXscale();
				  Poincik[1]=j*Data->GetYscale();
				  Poincik[2]=k*Data->GetZscale();

				  if (pomocy<5)
				  {
					  if(obbTree->InsideOrOutside(Poincik)==-1)
						  DData->SetVoxelValue((i-miX),(j-miY),(k-miZ),100); 
				  }
				  else
				  {
					  if(obbTree->InsideOrOutside(Poincik)==1)
						  DData->SetVoxelValue((i-miX),(j-miY),(k-miZ),100);
				  }
			  }	   

			  // Do_opa=fopen("c:\\Do_opaX.txt","a");
			  //fprintf(Do_opa,"X= %d z %d \n",i,maX );
			  //fclose(Do_opa);
			}




			//Zapisz




			//const char *name;	
			//const char *name2;
			QString filename2;	
			FILE *plik;


			if (filename.lastIndexOf(".dat")==-1)
				filename+=".dat";



			//name=filename.toLocal8Bit();
			filename2=filename+".header";
			//name2=filename2.toLocal8Bit();

			plik = fopen(filename2.toLocal8Bit(), "w");
			fprintf(plik,"%d %d  \n%lf %lf %lf \n%d %lf  \n%d %d %d \n",DData->GetNx(),DData->GetNy(),DData->GetXscale(),DData->GetYscale(),DData->GetZscale(),0,DData->GetGantryAngle(),miX+1,miY+1,miZ+1);




			fprintf(plik,"%d \n",*NumberOfPlanes);
			for (i=0;i<*NumberOfPlanes;i++)
				for(j=0;j<3;j++)
				{
					for(k=0;k<3;k++)
						fprintf(plik,"%lf ",PlanePoints[i*9+j*3+k]);
					fprintf(plik,"\n");
				}


				fclose(plik);
				plik = fopen(filename.toLocal8Bit(), "wb");
				fwrite( DData->GetDataArray(),sizeof(unsigned short),DData->GetVolumeSize(),plik);
				fclose(plik);





				QApplication::restoreOverrideCursor(  );


				return DData;

}
//----------------------------------------------------------------------------------------------
RawDataSet		*GraftVoxelize::CreateVolumeFormSurface(vtkPolyData *Input,int sx,int sy,int sz)
{

	int i,j,k;
	vtkOBBTree *obbTree=vtkOBBTree::New();

	obbTree->SetDataSet(Input);
	obbTree->SetTolerance(0.0000001);
	obbTree->BuildLocator();

	vtkPolyData *obb=vtkPolyData::New();

	obbTree->GenerateRepresentation(-10,obb);

	//	double pPoint1[3];
	//	vtkPoints *pPoints1=	Input->GetPoints();			
	//	int NPoint=pPoints1->GetNumberOfPoints();

	double min[3],max[3];


	double bounds[6];

	Input->GetBounds(bounds);

	min[0]=bounds[0];
	max[0]=bounds[1];
	min[1]=bounds[2];
	max[1]=bounds[3];
	min[2]=bounds[4];
	max[2]=bounds[5];


	//---------------------------------------------------------------
	//---------------------------------------------
	//-------------------------------------------
	double FloatSizeX=max[0]-min[0];
	double FloatSizeY=max[1]-min[1];
	double FloatSizeZ=max[2]-min[2];


	double dx=FloatSizeX/(double)(sx);
	double dy=FloatSizeY/(double)(sy);
	double dz=FloatSizeZ/(double)(sz);



	//tworzy zbiorek
	/*	miX=Round((double)min[0]/dx);
	int maX=Round((double)max[0]/dx);
	miY=Round((double)min[1]/dy);
	int maY=Round((double)max[1]/dy);
	miZ=Round((double)min[2]/dz);
	int maZ=Round((double)max[2]/dz);
	*/
	//	int SX=maX-miX+1;
	//	int SY=maY-miY+1;
	//	int SZ=maZ-miZ+1;


	RawDataSet *DData = new RawDataSet(sx,sy,sz,dx,dy,dz);
	DData->ZeroQ();


	double Poincik[3];
	// to jest tandetna i brudna sztuczka - michale popraw to jak to zrozumiesz qrde!
	int pomocy=0;
	/*
	for (k=0;k<=dx)
	for (j=miY;j<=maY;j=j+maY-miY)	
	for (k=miZ;k<=maZ;k=k+maZ-miZ)
	{
	Poincik[0]=i*dx;
	Poincik[1]=j*dy;
	Poincik[2]=k*dz;

	if(obbTree->InsideOrOutside(Poincik)==-1) pomocy++;
	}
	*/

	////////////////////////////////////////////////////
	// for (i=miX;i<=maX;i++)
	//{
	//	  for (j=miY;j<=maY;j++)	
	//		  for (k=miZ;k<=maZ;k++)

	unsigned short *QX=DData->GetDataArray();

	for(k=0;k<sz;k++)
		for(j=0;j<sy;j++)
			for(i=0;i<sx;i++)
			{
				Poincik[0]=i*dx+min[0];
				Poincik[1]=j*dy+min[1];
				Poincik[2]=k*dz+min[2];

				if (pomocy<5)
				{
					if(obbTree->InsideOrOutside(Poincik)==-1)
						*(QX)=100;
				}
				else
				{
					if(obbTree->InsideOrOutside(Poincik)==1)
						*(QX)=100;
				}

				QX++;
			}	   

			return DData;

}

//----------------------------------------------------
vtkCellArray	*GraftVoxelize::FillPlanarHole(vtkPoints *Points, int *HoleIndexPoints,int NumberOfPoints)
{

	int i,j,k,m;
	//	int		NR=0;
	QString text;
	double	*W=NULL;
	int		*v=NULL;
	int		n;
	double	temp1;
	int		*O;
	bool trial=true;
	//	int NrTempCells;
	//	vtkIdType *TriangleCell;
	//	vtkIdType TriangleNpts;
	//	int NumberOfFailed=0;
	vtkCellArray *FinalPolys= vtkCellArray::New();




	v=HoleIndexPoints;
	n=NumberOfPoints;

	int PointNr=Points->GetNumberOfPoints();
	double Point0[3];
	double *tabX=new double[PointNr*3];
	int aa=0;
	//speedup
	for(i=0;i<PointNr;i++)
	{
		Points->GetPoint(i,Point0);	
		tabX[aa++]=Point0[0];
		tabX[aa++]=Point0[1];
		tabX[aa++]=Point0[2];
	}
	//------------



	W=new double[n*n];
	O=new int[n*n](); 
	memset(W,0,n*n*sizeof(double));
	memset(O,0,n*n*sizeof(int));

	n=n-1;

	for (i=0;i<=n-3;i++)
		W[i+(i+2)*n]=TriangleArea(Points,v[i],v[i+1],v[i+2]);

	j=2;

	//	int kn;
	//	int ikn;

	int ik,mk,im;


	do
	{
		j+=1;
		for (i=0;i<=n-j-1;i++)
		{
			k=i+j;
			ik=i+n*k;
			W[ik]=ULONG_MAX;

			for(m=i+1;m<k;m++)
			{
				im=i+n*m;
				mk=m+n*k;
				temp1=W[im]+W[mk]+TriangleArea(tabX,v[i],v[m],v[k]);
				if (temp1<W[ik])
				{
					W[ik]=temp1;
					O[ik]=m;
				}
			}
		}
	}while(j<n-1); 


	trial=true;
	try
	{
		Trace(FinalPolys,O,v,n,0,n-1);
	}
	catch(...)
	{
		trial=false;
	}

	delete []W;
	delete []O;
	delete []tabX;

	if (trial==true)
	{
		return FinalPolys;
	}
	else
	{
		FinalPolys->Delete();
		return NULL;
	}



}

//------------------------------------------------------------------------
vtkPolyData		*GraftVoxelize::FillPolydataHoles( vtkPolyData *Input)
{
	int i,l;
	LinkedList<BoundaryElement*>	*BoundaryList=new LinkedList<BoundaryElement*>;	
	vtkPolyData						*OutputBoundary=vtkPolyData::New();

	int NRofBad=GetSortedBoundaryList(BoundaryList,OutputBoundary, Input);
	if (NRofBad>0)
		QMessageBox::about( 0,"Warning!","Number of bad boundaries: "+QString::number(NRofBad));

	if (BoundaryList->GetNumberOfItems()<1) 
	{
		QMessageBox::about( 0,"Warning!","Nothing to close");
		return NULL;
	}

	//	int			*LinePointIndexArray=NULL;
	vtkPoints	*Points=OutputBoundary->GetPoints();
	//	int			PointNr=OutputBoundary->GetNumberOfPoints();
	//	vtkPolyData *Boundary=vtkPolyData::New();

	QString		text;
	//	int			NumberOfPoints;

	vtkCellArray	*FinalPolys= vtkCellArray::New();
	vtkCellArray	*TempPolys=NULL;

	int				NrTempCells;
	vtkIdType		*TriangleCell;
	vtkIdType		TriangleNpts;
	int				NumberOfFailed=0;

	RibonTimer RTimer;
	RTimer.Start("Filling Holes");
	connect(&RTimer,SIGNAL( SendMessage(const QString &)),this,SIGNAL( SendMessage(const QString &)));

	for (l=0;l<BoundaryList->GetNumberOfItems();l++)
	{	

		//minimum area triangulation of the almost planar holes
		// Klincsek Berequet
		TempPolys=FillPlanarHole(Points,
			BoundaryList->GetItemAtPos(l)->GetData()->PointIndex,
			BoundaryList->GetItemAtPos(l)->GetData()->NumberOfPoints);
		vtkIdList* pointList = vtkIdList::New();

		if (TempPolys!=NULL)
		{
			NrTempCells=TempPolys->GetNumberOfCells();
			for(i=0;i<NrTempCells;i++)
			{	
				TempPolys->GetCell(i, pointList);
				FinalPolys->InsertNextCell(pointList);
			}
			TempPolys->Delete();
			TempPolys=NULL;
		}
		else
		{
			NumberOfFailed++;
		}
	}


	BoundaryList->DeleteAllItems();
	RTimer.Stop();


	if (NumberOfFailed>0)
		QMessageBox::about( 0,"ERROR","couldn't close"+QString::number(NumberOfFailed)+" Holes");
	return CreateFillHolesPolydataOutput(Input,FinalPolys,Points);


}

//-------------------------------------------------------------
vtkPolyData		*GraftVoxelize::FillPolydataHolesWithCrenellations( vtkPolyData *InputPolyData)
{

	/*

	int i,j,k,l,m,n,p;

	//	vtkPolyData *TempBoundary=NULL;

	LinkedList<BoundaryElement*>	*BoundaryList;
	BoundaryList=new LinkedList<BoundaryElement*>;


	int NRofBad=GetSortedBoundaryList2(BoundaryList,InputPolyData);
	if (NRofBad>0)
	QMessageBox::about( 0,"Warning!","Number of bad boundaries: "+QString::number(NRofBad));

	if (BoundaryList->GetNumberOfItems()<1) 
	{
	QMessageBox::about( 0,"Warning!","Nothing to close");
	return NULL;
	}

	int *LinePointIndexArray=NULL;
	vtkPoints *InputPoints=InputPolyData->GetPoints();
	int NRR=0;
	int *List=GetForbiddenPointConnectionList(InputPolyData,BoundaryList,NRR); // dostajemy liste zakazanych krawedzi

	///////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	//	vtkPolyData *Boundary=vtkPolyData::New();

	//	int		NR=0;
	QString text;
	int		NumberOfPoints;
	double	*WAngle=NULL;
	double	*WArea=NULL;
	int		*O;
	int		*v=NULL;
	double	tol=1e-6;
	int		ik;
	int		im;
	int		mk;

	double	TempAngle=0;
	double	TempArea=0;
	double	Angle1=0;
	double	Angle2=0;
	double	Angle3=0;



	int		Triangle0[3];
	int		Triangle1[3];
	int		Triangle2[3];
	int		Triangle3[3];

	vtkGenericCell		*TriangleCell1=vtkGenericCell::New();
	vtkGenericCell		*TriangleCell2=vtkGenericCell::New();
	//	vtkGenericCell		*TriangleCell3=vtkGenericCell::New();




	vtkPolyData *AdjacentPolyData=NULL;

	vtkCellArray *newPolys=NULL;
	vtkCellArray *FinalPolys= vtkCellArray::New();

	vtkCellArray *IntersectionCells=NULL;

	bool trial=true;
	int NrTempCells;
	vtkIdType *TriangleCell;
	vtkIdType TriangleNpts;
	int NumberOfFailed=0;

	int NrOfAdjacent;
	int tempX=0;
	double *Bounds=NULL;

	for (l=0;l<BoundaryList->GetNumberOfItems();l++)
	{	
	if (newPolys==NULL) newPolys = vtkCellArray::New();

	LinePointIndexArray=BoundaryList->GetItemAtPos(l)->GetData()->PointIndex;
	NumberOfPoints=BoundaryList->GetItemAtPos(l)->GetData()->NumberOfPoints;
	v=LinePointIndexArray;
	n=NumberOfPoints;
	//////////////////////////////////////
	if (Bounds!=NULL)	delete []Bounds;
	Bounds=GetBoundaryBoundingBox(v,NumberOfPoints,InputPoints);
	if (IntersectionCells!=NULL)  IntersectionCells->Delete();
	IntersectionCells=vtkCellArray::New();
	int t=0;
	for(i=0;i<InputPolyData->GetNumberOfCells();i++)
	{
	InputPolyData->GetCell(i,TriangleCell1);
	for(j=0;j<3;j++)
	{
	Triangle0[j]=TriangleCell1->GetPointId(j);
	}

	// sprawdze czy nalezy do bounds

	if (TriangleInsideBoundingBox(Triangle0[0],Triangle0[1],Triangle0[2],InputPoints,Bounds))
	{
	IntersectionCells->InsertNextCell(3,Triangle0); 
	t++;
	}
	}
	//////////////////////////////////////////
	WArea=new double[n*n];
	WAngle=new double[n*n];
	O=new int[n*n]; 
	n=n-1;

	memset(WArea,0,n*n*sizeof(double));
	memset(WAngle,0,n*n*sizeof(double));
	memset(O,0,n*n*sizeof(int));


	for(i=0;i<=n-2;i++)
	{
	WAngle[i+(i+1)*n]=0;
	WArea[i+(i+1)*n]=0;
	}

	AdjacentPolyData=vtkPolyData::New();
	NrOfAdjacent=GetAdjacentTriangles(AdjacentPolyData,InputPolyData,LinePointIndexArray, n);
	if (NrOfAdjacent!=n) return NULL;

	for (i=0;i<=n-3;i++)
	{
	WArea[i+(i+2)*n]=TriangleArea(InputPoints,v[i],v[i+1],v[i+2]);

	//////////////////////////////////////////////////////

	// pierwszy trojkat tworzony z kolejnych punktow krawedzi
	Triangle0[0]=v[i];
	Triangle0[1]=v[i+1];
	Triangle0[2]=v[i+2];

	// dwa pozostale trojkaty biore z istniejacych trojkatow przyleglych do brzegu
	AdjacentPolyData->GetCell(i,TriangleCell1);
	AdjacentPolyData->GetCell(i+1,TriangleCell2);

	for(j=0;j<3;j++)
	{
	Triangle1[j]=TriangleCell1->GetPointId(j);
	Triangle2[j]=TriangleCell2->GetPointId(j);
	}

	Angle1=GetDihedralAngle(Triangle0,Triangle1,InputPoints);
	Angle2=GetDihedralAngle(Triangle0,Triangle2,InputPoints);

	WAngle[i+(i+2)*n]=MAX(Angle1,Angle2);  
	}

	j=2;

	do
	{
	j+=1;
	for (i=0;i<=n-j-1;i++)
	{

	k=i+j;
	ik=i+n*k;

	/// szukam minimum
	WAngle[ik]=3*PI;
	WArea[ik]=999999999;

	for(m=i+1;m<k;m++)
	{

	im=i+n*m;
	mk=m+n*k;

	TempArea=WArea[im]+WArea[mk]+TriangleArea(InputPoints,v[i],v[m],v[k]);
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	Triangle0[0]=v[i];	Triangle0[1]=v[m];		Triangle0[2]=v[k];
	Triangle1[0]=v[i];	Triangle1[1]=v[O[im]];	Triangle1[2]=v[m];
	Triangle2[0]=v[m];	Triangle2[1]=v[O[mk]];	Triangle2[2]=v[k];
	/////////////////////////////////////////////////////////////////////
	Angle1=	(O[im]!=0)?GetDihedralAngle(Triangle0,Triangle1,InputPoints):0;
	Angle2= (O[mk]!=0)?GetDihedralAngle(Triangle0,Triangle2,InputPoints):0;
	/////////////////////////////////////////////////////////////////////
	if ((m-i==1)||(k-m==1)||(k-i==n-1))
	{
	if (m-i==1)	
	tempX=i;
	if (k-m==1)	
	tempX=m;
	if (k-i==n-1)
	tempX=k;


	AdjacentPolyData->GetCell(tempX,TriangleCell1);

	for(p=0;p<3;p++)
	Triangle3[p]=TriangleCell1->GetPointId(p);
	Angle3=GetDihedralAngle(Triangle0,Triangle3,InputPoints);

	}
	else 
	Angle3=0;

	TempAngle=MAX3(Angle1,Angle2,Angle3);
	TempAngle=MAX3(WAngle[im],WAngle[mk],TempAngle);

	//////////////////////////////////////////////////////

	if ((TempAngle<WAngle[ik])||((fabs(TempAngle-WAngle[ik])<tol)&&(TempArea<WArea[ik])))
	{

	if ((NRR==0)||(CheckIfTriangleNorForbidden(v[i],v[k],v[m],List,NRR)))
	{

	if(IntersectTriangleWithPolyData(v[i],v[k],v[m],InputPoints,IntersectionCells)==0)
	//	if(IntersectTriangleWithPolyData(v[i],v[k],v[m],InputPolyData)==0)
	{
	WArea[ik]=TempArea;
	WAngle[ik]=TempAngle;
	O[ik]=m; 
	}

	}
	}

	}


	}
	}while(j<n-1); 


	trial=true;
	try
	{
	Trace(newPolys,O,v,n,0,n-1);
	}
	catch(...)
	{
	trial=false;
	NumberOfFailed++;
	}
	if (trial==true)
	{
	NrTempCells=newPolys->GetNumberOfCells();
	for(i=0;i<NrTempCells;i++)
	{	
	newPolys->GetNextCell(TriangleNpts,TriangleCell);
	FinalPolys->InsertNextCell(3,TriangleCell); 
	//	delete []TriangleCell;
	}
	newPolys->Delete();
	newPolys=NULL;
	}

	delete []WAngle;
	delete []WArea;
	delete []O;




	WAngle=NULL;
	WArea=NULL;
	O=NULL;

	AdjacentPolyData->Delete();

	}

	if (IntersectionCells!=NULL)	IntersectionCells->Delete();
	if (Bounds!=NULL)				delete []Bounds;

	BoundaryList->DeleteAllItems();
	delete []List;

	if (NumberOfFailed>0)
	QMessageBox::about( 0,"ERROR","couldn't close"+QString::number(NumberOfFailed)+" Holes");
	return CreateFillHolesPolydataOutput(InputPolyData,FinalPolys,InputPoints);
	*/
	return NULL;
}

//--------------------------------------------------------------------------------

vtkPolyData		*GraftVoxelize::FillPolydataHolesMaxVolume( vtkPolyData *InputPolyData)
{
	/*
	int i,j,k,l,m,n;

	//	vtkPolyData *TempBoundary=NULL;

	LinkedList<BoundaryElement*>	*BoundaryList;
	BoundaryList=new LinkedList<BoundaryElement*>;


	int NRofBad=GetSortedBoundaryList2(BoundaryList,InputPolyData);
	if (NRofBad>0)
	QMessageBox::about( 0,"Warning!","Number of bad boundaries: "+QString::number(NRofBad));

	if (BoundaryList->GetNumberOfItems()<1) 
	{
	QMessageBox::about( 0,"Warning!","Nothing to close");
	return NULL;
	}

	int *LinePointIndexArray=NULL;

	vtkPoints *InputPoints=InputPolyData->GetPoints();


	int NRR=0;
	int *List=GetForbiddenPointConnectionList(InputPolyData,BoundaryList,NRR); // dostajemy liste zakazanych krawedzi

	///////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	//	vtkPolyData *Boundary=vtkPolyData::New();

	//	int		NR=0;
	QString text;
	int		NumberOfPoints;
	double	*WVolume=NULL;
	double	*WArea=NULL;
	int		*O;
	int		*v=NULL;
	//	double	tol=1e-6;
	int		ik;
	int		im;
	int		mk;
	double 	TempArea;
	double	TempVolume;


	int		Triangle0[3];
	//	int		Triangle1[3];
	//	int		Triangle2[3];
	//	int		Triangle3[3];


	vtkGenericCell		*TriangleCell1=vtkGenericCell::New();

	vtkCellArray *newPolys=NULL;
	vtkCellArray *FinalPolys= vtkCellArray::New();
	vtkCellArray *IntersectionCells=NULL;

	bool trial=true;
	int NrTempCells;
	vtkIdType *TriangleCell;
	vtkIdType TriangleNpts;
	int NumberOfFailed=0;


	//	int tempX;

	double *Bounds=NULL;

	for (l=0;l<BoundaryList->GetNumberOfItems();l++)
	{
	if (newPolys==NULL) newPolys = vtkCellArray::New();
	LinePointIndexArray=BoundaryList->GetItemAtPos(l)->GetData()->PointIndex;
	NumberOfPoints=BoundaryList->GetItemAtPos(l)->GetData()->NumberOfPoints;
	v=LinePointIndexArray;

	n=NumberOfPoints;

	//////////////////////////////////////
	if (Bounds!=NULL)	delete []Bounds;
	Bounds=GetBoundaryBoundingBox(v,NumberOfPoints,InputPoints);
	if (IntersectionCells!=NULL)  IntersectionCells->Delete();
	IntersectionCells=vtkCellArray::New();

	for(i=0;i<InputPolyData->GetNumberOfCells();i++)
	{
	InputPolyData->GetCell(i,TriangleCell1);
	for(j=0;j<3;j++)
	{
	Triangle0[j]=TriangleCell1->GetPointId(j);
	}

	// sprawdze czy nalezy do bounds

	if (TriangleInsideBoundingBox(Triangle0[0],Triangle0[1],Triangle0[2],InputPoints,Bounds))
	IntersectionCells->InsertNextCell(3,Triangle0); 
	}
	//////////////////////////////////////////	

	WArea=new double[n*n];
	WVolume=new double[n*n];
	O=new int[n*n]; 
	n=n-1;


	memset(WArea,0,n*n*sizeof(double));
	memset(WVolume,0,n*n*sizeof(double));
	memset(O,0,n*n*sizeof(int));




	for(i=0;i<=n-2;i++)
	{
	WVolume[i+(i+1)*n]=0;
	WArea[i+(i+1)*n]=0;
	}



	for (i=0;i<=n-3;i++)
	{
	WArea[i+(i+2)*n]=	TriangleArea(InputPoints,v[i],v[i+1],v[i+2]);
	WVolume[i+(i+2)*n]=	TriangleTetraVolume(InputPoints,v[i],v[i+1],v[i+2]);
	//////////////////////////////////////////////////////
	}

	j=2;

	do
	{
	j+=1;
	for (i=0;i<=n-j-1;i++)
	{

	k=i+j;
	ik=i+n*k;

	/// szukam minimum
	WVolume[ik]=-999999999;
	WArea[ik]=999999999;

	for(m=i+1;m<k;m++)
	{

	im=i+n*m;
	mk=m+n*k;

	TempArea=WArea[im]+WArea[mk]+TriangleArea(InputPoints,v[i],v[m],v[k]);
	TempVolume=WVolume[im]+WVolume[mk]+TriangleTetraVolume(InputPoints,v[i],v[m],v[k]);



	//////////////////////////////////////////////////////
	if ((TempVolume>WVolume[ik])&&(TempArea<WArea[ik]))
	// 	if ((TempVolume>WVolume[ik])||((fabs(TempVolume-WVolume[ik])<tol)&&(TempArea<WArea[ik])))
	{

	if ((NRR==0)||(CheckIfTriangleNorForbidden(v[i],v[k],v[m],List,NRR)))
	{
	if(IntersectTriangleWithPolyData(v[i],v[k],v[m],InputPoints,IntersectionCells)==0)
	//if(IntersectTriangleWithPolyData(v[i],v[k],v[m],InputPolyData)==0)
	{
	WArea[ik]=TempArea;
	WVolume[ik]=TempVolume;
	O[ik]=m;
	}

	}
	}

	}


	}
	}while(j<n-1); 

	trial=true;
	try
	{
	Trace(newPolys,O,v,n,0,n-1);
	}
	catch(...)
	{

	trial=false;
	NumberOfFailed++;
	//	return NULL;

	}
	if (trial==true)
	{
	NrTempCells=newPolys->GetNumberOfCells();
	for(i=0;i<NrTempCells;i++)
	{	
	newPolys->GetNextCell(TriangleNpts,TriangleCell);
	FinalPolys->InsertNextCell(3,TriangleCell); 
	}
	newPolys->Delete();
	newPolys=NULL;
	}

	delete []WVolume;
	delete []WArea;
	delete []O;


	WVolume=NULL;
	WArea=NULL;
	O=NULL;



	}


	if (IntersectionCells!=NULL)	IntersectionCells->Delete();
	if (Bounds!=NULL)				delete []Bounds;
	BoundaryList->DeleteAllItems();
	delete []List;

	if (NumberOfFailed>0)
	QMessageBox::about( 0,"ERROR","couldn't close"+QString::number(NumberOfFailed)+" Holes");
	return CreateFillHolesPolydataOutput(InputPolyData,FinalPolys,InputPoints);

	}

	//-------------------------------------------------------------
	vtkPolyData		*GraftVoxelize::FillPolydataPlanarHoles( vtkPolyData *Input)
	{

	int i,j,l;
	//	vtkPolyData						*TempBoundary=NULL;
	LinkedList<BoundaryElement*>	*BoundaryList=new LinkedList<BoundaryElement*>;	
	vtkPolyData						*OutputBoundary=vtkPolyData::New();


	int NRofBad=GetSortedBoundaryList(BoundaryList,OutputBoundary, Input);
	if (NRofBad>0)
	QMessageBox::about( 0,"Warning!","Number of bad boundaries: "+QString::number(NRofBad));

	if (BoundaryList->GetNumberOfItems()<1) 
	{
	QMessageBox::about( 0,"Warning!","Nothing to close");
	return NULL;
	}

	//	int			*LinePointIndexArray=NULL;
	vtkPoints	*Points=OutputBoundary->GetPoints();
	//	int			PointNr=OutputBoundary->GetNumberOfPoints();
	//	vtkPolyData *Boundary=vtkPolyData::New();

	QString		text;
	int			NumberOfPoints;

	vtkCellArray	*FinalPolys= vtkCellArray::New();
	vtkCellArray	*TempPolys=NULL;

	int				NrTempCells;
	vtkIdType		*TriangleCell;
	vtkIdType		TriangleNpts;
	int				NumberOfFailed=0;

	QApplication::setOverrideCursor( waitCursor );
	Timer Stoper;
	emit SendMessage("Filling Holes ...");





	int p1,p2,p3;

	int *PointTab;
	int *PointTabTemp;
	int *PointTabTemp2;
	int NumberOfTemp2Points;

	PlaneObject *MegaPlane=new PlaneObject();	
	double	Point[3],PointLeft[3],PointRight[3],PointMid[3];
	double	Angle,tempAngle;
	//	int		aa1,aa2,aa3;
	double	tolerance=0.1;
	int		left,right;
	double	planedist;
	int		NumberOfTempPoints;
	int		pos;
	bool	test;


	for (l=0;l<BoundaryList->GetNumberOfItems();l++)
	{	

	//minimum area triangulation of the almost planar holes
	// Klincsek Berequet
	//TempPolys=FillPlanarHole(Points,
	PointTab=BoundaryList->GetItemAtPos(l)->GetData()->PointIndex;
	NumberOfPoints=BoundaryList->GetItemAtPos(l)->GetData()->NumberOfPoints;
	// stworze kopie tablicy z pktami
	PointTabTemp=new int[NumberOfPoints];
	memcpy(PointTabTemp,PointTab,NumberOfPoints*sizeof(int));
	NumberOfTempPoints=NumberOfPoints-1;

	//	UsedPointsTab=new int[NumberOfPoints];
	//	memset(UsedPointsTab,0,NumberOfPoints*sizeof(int));
	// po wybraniu pktow nalezacych do 1 kawalka wywale z kopii uzyte pkty

	//do
	for(int z=0;z<3;z++)
	{

	//text="";

	//--- tu zamierzam podzielic dziure na kawalki plaszczyznowe


	// 1.wybrac 1 pkt i sprawdzic jego sasiadow czy wyznaczaja plaszczyzne

	//pos=5;

	//	p1=PointTabTemp[pos%NumberOfTempPoints];
	//	p2=PointTabTemp[(pos+1)%NumberOfTempPoints];
	//	p3=PointTabTemp[(pos+2)%NumberOfTempPoints];

	//		UsedPointsTab[NumberOfTempPoints-1]=1;


	pos=5;
	// 2a.jezeli nie przesunac 1 pkt i tak do skutku
	do
	{
	pos++;
	p1=PointTabTemp[pos%NumberOfTempPoints];
	p2=PointTabTemp[(pos+1)%NumberOfTempPoints];
	p3=PointTabTemp[(pos+2)%NumberOfTempPoints];

	Points->GetPoint(p1,PointLeft);
	Points->GetPoint(p2,PointMid);
	Points->GetPoint(p3,PointRight);

	Angle=MegaPlane->SetPlanePoints(PointMid,PointLeft,PointRight);
	}while((!(Angle<87))&&(p3<NumberOfTempPoints));

	//}while((!((Angle>5)&&(Angle<175)))&&(p3<NumberOfTempPoints));


	// 2b.jezeli tak to zbieram okoliczne wszystkie pkty 
	// szukam w lewo i prawo jakie pkty naleza do tej plaszczyzny
	//left=0;
	//right=1;
	int posT;
	j=1;
	// w lewo
	do
	{
	posT=(pos+NumberOfTempPoints-j)%NumberOfTempPoints;
	Points->GetPoint(PointTabTemp[posT],Point);
	planedist=MegaPlane->CheckPoint(Point);
	j++;


	test=fabs(planedist)<tolerance;
	if (test==true)
	{
	tempAngle=MegaPlane->SetPlanePoints(PointMid,Point,PointRight);
	if (tempAngle<Angle)
	{
	PointLeft[0]=Point[0];
	PointLeft[1]=Point[1];
	PointLeft[2]=Point[2]; 
	Angle=tempAngle;
	}
	else
	MegaPlane->SetPlanePoints(PointMid,PointLeft,PointRight);
	}



	}while((test)&&(j<NumberOfTempPoints+pos));
	left=j-2;


	j=2;
	// w prawo
	do
	{
	posT=(pos+j+2)%NumberOfTempPoints;
	Points->GetPoint(PointTabTemp[posT],Point);
	planedist=MegaPlane->CheckPoint(Point);
	j++;

	test=fabs(planedist)<tolerance;
	if (test==true)
	{
	tempAngle=MegaPlane->SetPlanePoints(PointMid,PointLeft,Point);
	if (tempAngle<Angle)
	{
	PointRight[0]=Point[0];
	PointRight[1]=Point[1];
	PointRight[2]=Point[2]; 
	Angle=tempAngle;
	}
	else
	MegaPlane->SetPlanePoints(PointMid,PointLeft,PointRight);
	}

	}while((test)&&(j<NumberOfTempPoints+pos));
	right=j-1;

	//////////////////////////////////////
	// znalazlem granice teraz trzeba zebrac w osobna tablice 
	NumberOfTemp2Points=right+left+2;

	PointTabTemp2=new int [NumberOfTemp2Points];

	for(i=0;i<NumberOfTemp2Points-1;i++)
	{
	//zbieram do tablicy i tworze nowa bazowa
	posT=(pos+NumberOfTempPoints-left+i)%NumberOfTempPoints;
	PointTabTemp2[i]=PointTabTemp[posT];
	//	text+=QString::number(PointTabTemp2[i])+"\n";
	}
	PointTabTemp2[NumberOfTemp2Points-1]=PointTabTemp2[0];

	//QMessageBox::about( 0,"",text);


	//minimum area triangulation of the almost planar holes
	// Klincsek Berequet
	TempPolys=FillPlanarHole(Points,
	PointTabTemp2,
	NumberOfTemp2Points);






	if (NumberOfTemp2Points==NumberOfTempPoints)
	{
	// trzeba to przerwac bo wszystkie pkty wykorzystane
	i=i;
	}
	else
	{
	//tu trzeba bedzie stworzyc nowa tablice z niewykorzystanymi pktami

	// brzydko ale teraz nei zrobie lepiej
	for(i=1;i<NumberOfTemp2Points-2;i++)
	{	
	posT=(pos+NumberOfTempPoints-left+i)%NumberOfTempPoints;
	PointTabTemp[posT]=-1;
	}

	delete []PointTabTemp2;
	NumberOfTemp2Points=NumberOfTempPoints-NumberOfTemp2Points+3;

	PointTabTemp2=new int[NumberOfTemp2Points];

	i=0;
	j=0;
	do
	{
	if (PointTabTemp[i++]!=-1) PointTabTemp2[j++]=PointTabTemp[i-1];

	}while (i<NumberOfTempPoints);


	NumberOfTempPoints=NumberOfTemp2Points;
	delete PointTabTemp;
	PointTabTemp=PointTabTemp2;
	PointTabTemp2=NULL;
	NumberOfTemp2Points=0;


	}


	//while(NumberOfTemp2Points<NumberOfTempPoints);
	// przerobic na triangulacje
	// usunac wykorzystane pkty z tablicy
	// i powtarzac do wykorzystania wszystkich pktow



	if (TempPolys!=NULL)
	{
	NrTempCells=TempPolys->GetNumberOfCells();
	for(i=0;i<NrTempCells;i++)
	{	
	TempPolys->GetNextCell(TriangleNpts,TriangleCell);
	FinalPolys->InsertNextCell(3,TriangleCell); 
	}
	TempPolys->Delete();
	TempPolys=NULL;
	}
	else
	{
	NumberOfFailed++;
	}
	}
	}

	BoundaryList->DeleteAllItems();
	emit SendMessage("Filling Holes ... done : "+Stoper.GetTime());
	QApplication::restoreOverrideCursor(  );


	if (NumberOfFailed>0)
	QMessageBox::about( 0,"ERROR","couldn't close"+QString::number(NumberOfFailed)+" Holes");
	vtkPolyData *InputX=vtkPolyData::New();

	return CreateFillHolesPolydataOutput(InputX,FinalPolys,Points);




	}

	//-------------------------------------------------------------
	void			GraftVoxelize::Trace(vtkCellArray *Polys,int *O,int *v,int n,int i,int k)
	{
	if (i==k) throw 0;

	vtkIdType Triangle[3];
	vtkIdType ID;
	int o;
	if (i+2==k)
	{
	Triangle[0]=v[i];
	Triangle[1]=v[i+1];
	Triangle[2]=v[k];
	ID=Polys->InsertNextCell(3,Triangle); // DODAJE SOBIE TROJKATERASA - A CO WOLNO MI :)

	//	QMessageBox::about( 0,"",QString::number(v[i])+" "+QString::number(v[i+1])+" "+QString::number(v[k]));

	}
	else
	{
	o=O[i+k*n];

	if (o!=i+1) Trace(Polys,O,v,n,i,o);

	Triangle[0]=v[i];
	Triangle[1]=v[o];
	Triangle[2]=v[k];

	ID=Polys->InsertNextCell(3,Triangle); // DODAJE SOBIE TROJKATERASA - A CO WOLNO MI :)

	//	QMessageBox::about( 0,"",QString::number(v[i])+" "+QString::number(v[k])+" "+QString::number(v[o]));

	if (o!=k-1) Trace(Polys,O,v,n,o,k);

	}
	*/
	return NULL;

}

//--------------------------------------------------------------

