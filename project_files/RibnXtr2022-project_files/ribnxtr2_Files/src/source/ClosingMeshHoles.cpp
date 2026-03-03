#include "ClosingMeshHoles.h"


bool PlanarPolyDataHolesExtractor::ExtractHoles(vtkPolyData *Input, bool ForbiddenConnections)
{
	int NRofBad=GetSortedBoundaryList2(BoundaryList,Input);
	if (NRofBad>0)
		QMessageBox::about( 0,"Warning!","Number of bad boundaries: "+QString::number(NRofBad));

	//----------------------------------------------------------------
	int *PointTab;
	int *PointTabTemp;
	int *PointTabTemp2;
	int *OrigPosTab=NULL;
	int NumberOfPoints;
	int NumberOfTemp2Points;

//	PlaneObject   *MegaPlane=new PlaneObject();	
//	double	P[3],PL[3],PR[3],PM[3];

//	vtkGenericCell *TriangleCell=vtkGenericCell::New();
//	double	Angle,tempAngle;
//	int		aa1,aa2,aa3;
//	double	tolerance=10;
//	int		left,right;
//	double	planedist;
	int		NTPts;
	int		NTPtsM1;
//	bool	test=true;
//	int		p1,p2,p3;
//	int		p,r,common;
int p;

	int		i,j,k;//,l;
	//int     stop=0;
	int		posopt=0,sizeopt=0;
//	int		temppos=0;
//	int		planesetsize;
//	int		pos;
	bool	loopexit;
	bool	delcurrbound;
	int		g1=0,g2=0;
//	double	tAngle=0;
//	double	tAngle2=0;
//	double	tAngleopt=-999999;
//	double	g3=0;
	int		*AllowConTab=NULL;
	int		NrOfAllCon;
//	double	minangle=-50.0;
//	double	maxangle=170.0;
	LinkedList<BoundaryElement*>	*BoundaryTempList=NULL;
	int		posT;//,posTm1,posTm2,posTp1,posTp2;

	vtkPoints *Points=Input->GetPoints();
	vtkPolyData *AdjacentPolyData=NULL;

//	FILE *Do_opa;
	//Do_opa=fopen("c:\\a.txt","w");fprintf(Do_opa,"\n \n");fclose(Do_opa);
	int NR=BoundaryList->GetNumberOfItems();

	/////////////////////////////////

	//znajde srodek

	double center[3];
	double bounds[6];

	Input->GetBounds(bounds);

	center[0]=(bounds[0]+bounds[1])/2.0;
	center[1]=(bounds[2]+bounds[3])/2.0;
	center[2]=(bounds[4]+bounds[5])/2.0;




	////////////////////////////////
	for (i=0;i<NR;i++)
	{	

		//Do_opa=fopen("c:\\a.txt","a");fprintf(Do_opa,"\n %d \n",i);fclose(Do_opa);


		PointTab=BoundaryList->GetCurrentListItem()->GetData()->PointIndex;
		NumberOfPoints=BoundaryList->GetCurrentListItem()->GetData()->NumberOfPoints;
		// stworze kopie tablicy z pktami
		PointTabTemp=new int[NumberOfPoints];
		memcpy(PointTabTemp,PointTab,NumberOfPoints*sizeof(int));
		NTPts=NumberOfPoints-1;
		NTPtsM1=NTPts-1;
		loopexit=false;
		delcurrbound=true;

		if (AllowConTab!=NULL) delete []AllowConTab;
		AllowConTab=new int[NTPts*2];
		NrOfAllCon=0;

		//////////////////////////////////////////////////
		if (AdjacentPolyData!=NULL) AdjacentPolyData->Delete();

		AdjacentPolyData=vtkPolyData::New();

		int a=GetAdjacentTriangles(AdjacentPolyData,Input,PointTab, NumberOfPoints-1);
		if(a!=NumberOfPoints-1) throw 0;
		//////////////////////////////////////////////////

		for(g2=0;g2<1;g2++)
			//do
		{	

			//////////////////////////////////////
			if (OrigPosTab!=NULL) delete []OrigPosTab;
			OrigPosTab=new int[NTPts];
			for (j=0;j<NTPts;j++)
			{
				p=-1;
				for(k=0;k<NumberOfPoints-1;k++)
				{if (PointTabTemp[j]==PointTab[k]){OrigPosTab[j]=k;p=0;break;}}
				if (p==-1){p=p;}
			}



			FindLargestPlanarPointSet(posopt,sizeopt,Points, PointTabTemp,NTPts,
				AllowConTab,NrOfAllCon,AdjacentPolyData,OrigPosTab,center);

			// znalazlem granice teraz trzeba zebrac w osobna tablice 
			if ((sizeopt<NTPts)||((sizeopt==NTPts)&&(delcurrbound==false)))
			{
				NumberOfTemp2Points=sizeopt+1;
				PointTabTemp2=new int [NumberOfTemp2Points];
				for(j=0;j<NumberOfTemp2Points-1;j++)
				{
					//zbieram do tablicy i tworze nowa bazowa
					posT=(posopt+j)%NTPts;
					PointTabTemp2[j]=PointTabTemp[posT];
					if ((j>0)&&(j<NumberOfTemp2Points-2))
						PointTabTemp[posT]=-1; // do celow pozniejszych
				}

				AllowConTab[NrOfAllCon*2]=PointTabTemp2[0];
				AllowConTab[NrOfAllCon*2+1]=PointTabTemp2[NumberOfTemp2Points-2];
				NrOfAllCon++;

				PointTabTemp2[NumberOfTemp2Points-1]=PointTabTemp2[0];
				//zmontowane - teraz trzeba przerobic to na boundary
				BoundaryElement *El=new BoundaryElement(NumberOfTemp2Points,PointTabTemp2);
				if (BoundaryTempList==NULL)
					BoundaryTempList=new LinkedList<BoundaryElement*>;
				BoundaryTempList->InsertItem(El);
				PointTabTemp2=NULL;	
				if ((sizeopt==NTPts)&&(delcurrbound==false)) loopexit=true;
				if (loopexit==false)
				{
					if (delcurrbound==true)
					{				
						//BoundaryList->DeleteItem(); // skasowac tylko za pierwszym razem
						delcurrbound=false;
					}
					// przerobic PointTabTemp i powtorzyc
					//-------------------------------------
					//tu trzeba bedzie stworzyc nowa tablice z niewykorzystanymi pktami
					NumberOfTemp2Points=NTPts-NumberOfTemp2Points+3;
					PointTabTemp2=new int[NumberOfTemp2Points+1];

					j=0,k=0;
					do
					{
						if (PointTabTemp[k++]!=-1) PointTabTemp2[j++]=PointTabTemp[k-1];
					}while (k<NTPts);
					g1=j;
					NTPts=NumberOfTemp2Points;
					NTPtsM1=NTPts-1;

					delete []PointTabTemp;
					PointTabTemp=PointTabTemp2;
					PointTabTemp2=NULL;
					NumberOfTemp2Points=0;
				}
				//--------------------------------------
			}
			else
				loopexit=true;
		}//while (loopexit==false);

		if (delcurrbound==false)				
			BoundaryList->DeleteItem(); // skasowac tylko za pierwszym razem

		//--------------------------------------------------------------------
		PointTab=NULL;
		delete []PointTabTemp;
		PointTabTemp=NULL;
	}

	AdjacentPolyData->Delete();
	if (OrigPosTab!=NULL)
	{
		delete []OrigPosTab;
		OrigPosTab=NULL;	
	}

	if (AllowConTab!=NULL)
	{
		delete []AllowConTab;
		AllowConTab=NULL;
		NrOfAllCon=0;
	}

	if (BoundaryTempList!=NULL)
	{
		k=BoundaryTempList->GetNumberOfItems();
		for(i=0;i<k;i++)
		{
			BoundaryList->InsertItem(BoundaryTempList->GetItemAtPos(i)->GetData());
			BoundaryTempList->GetItemAtPos(i)->SetData(NULL);
		}

		delete BoundaryTempList;
		BoundaryTempList=NULL;
	}



	//----------------------------------------------------------------
	if (BoundaryList->GetNumberOfItems()<1) 
	{
		QMessageBox::about( 0,"Warning!","Nothing to close");
		return false;
	}

	if (ForbiddenConnections==true)
	{
		if (ForbiddenConnectionList!=NULL)
			delete []ForbiddenConnectionList;
		ForbiddenConnectionList=GetForbiddenPointConnectionList(Input,BoundaryList,NumberOfForbiddenConnections); // dostajemy liste zakazanych krawedzi
	}
	return true;
}
//--------------------------------------------------------
bool SimplePolyDataHolesExtractor::ExtractHoles(vtkPolyData *Input, bool ForbiddenConnections)
{
	int NRofBad=GetSortedBoundaryList2(BoundaryList,Input);
	if (NRofBad>0)
		QMessageBox::about( 0,"Warning!","Number of bad boundaries: "+QString::number(NRofBad));

	if (BoundaryList->GetNumberOfItems()<1) 
	{
		QMessageBox::about( 0,"Warning!","Nothing to close");
		return false;
	}

	if (ForbiddenConnections==true)
	{
		if (ForbiddenConnectionList!=NULL)
			delete []ForbiddenConnectionList;
		ForbiddenConnectionList=GetForbiddenPointConnectionList(Input,BoundaryList,NumberOfForbiddenConnections); // dostajemy liste zakazanych krawedzi
	}
	return true;
}
//--------------------------------------------------------
CrenellationWeightingFunction::CrenellationWeightingFunction(int *tab,int _size,vtkPolyData *_InputPolyData,bool _CheckIntersection):TriangulationWeightingFunction(tab,_size,_InputPolyData,_CheckIntersection)
{			
	WArea=new double[(size+1)*(size+1)];
	WAngle=new double[(size+1)*(size+1)];
	AdjacentPolyData=vtkPolyData::New();

	int a=GetAdjacentTriangles(AdjacentPolyData,InputPolyData,v, size);
	if(a!=size) throw 0;

	TriangleCell1=vtkGenericCell::New();
	TriangleCell2=vtkGenericCell::New();
	TriangleCell3=vtkGenericCell::New();

	if (CheckIntersection==true) GetIntersectionCells();
	tolerance=1e-6;

}
//-------------------------------------------------------
CrenellationWeightingFunction::~CrenellationWeightingFunction()
{
	if (WArea!=NULL) 
	{
		delete []WArea;
		WArea=NULL;
	}
	if (WAngle!=NULL) 
	{
		delete []WAngle;
		WAngle=NULL;
	}
	if (IntersectionCells!=NULL) IntersectionCells->Delete();

	TriangleCell1->Delete();
	TriangleCell2->Delete();
	TriangleCell3->Delete();
	AdjacentPolyData->Delete();


}
//------------------------------------------------------
void CrenellationWeightingFunction::GetIntersectionCells()
{
	int j;
	vtkIdType i;
	double *Bounds;
	Bounds=GetBoundaryBoundingBox(v,size,InputPoints);
	if (IntersectionCells!=NULL)  IntersectionCells->Delete();
	IntersectionCells=vtkCellArray::New();

	vtkIdType triangle[3];
	for(i=0;i<InputPolyData->GetNumberOfCells();i++)
	{
		InputPolyData->GetCell(i,TriangleCell1);
		for(j=0;j<3;j++)
			triangle[j]=TriangleCell1->GetPointId(j);
		if (TriangleInsideBoundingBox(triangle[0],Triangle0[1],triangle[2],InputPoints,Bounds))
			IntersectionCells->InsertNextCell(3,triangle); 
	}

	delete []Bounds;

}
//-------------------------------------------------------
void CrenellationWeightingFunction::SetWeight(int i,int k,int value)
{
	WAngle[i+k*size]=value;
	WArea[i+k*size]=value;

}
//-------------------------------------------------------
void CrenellationWeightingFunction::FindMinWeight(int i,int k)
{	
	int im,ik,mk,l=0;
	double	TempArea;
	double Angle1,Angle2,Angle3,TempAngle;
	ik=i+size*k;

	WAngle[ik] = 3 * M_PI;
	WArea[ik]=999999999;


	for(int m=i+1;m<k;m++)
	{
		im=i+size*m;
		mk=m+size*k;

		TempArea=WArea[im]+WArea[mk]+TriangleArea(InputPoints,v[i],v[m],v[k]);
		Triangle0[0]=v[i];	Triangle0[1]=v[m];		Triangle0[2]=v[k];
		Triangle1[0]=v[i];	Triangle1[1]=v[O[im]];	Triangle1[2]=v[m];
		Triangle2[0]=v[m];	Triangle2[1]=v[O[mk]];	Triangle2[2]=v[k];
		Angle1=	(O[im]!=0)?GetDihedralAngle(Triangle0,Triangle1,InputPoints):0;
		Angle2= (O[mk]!=0)?GetDihedralAngle(Triangle0,Triangle2,InputPoints):0;

		if ((m-i==1)||(k-m==1)||(k-i==size-1))
		{
			if (m-i==1)		l=i;
			if (k-m==1)		l=m;
			if (k-i==size-1)	l=k;
			AdjacentPolyData->GetCell(l,TriangleCell1);

			for(int p=0;p<3;p++)
				Triangle3[p]=TriangleCell1->GetPointId(p);
			Angle3=GetDihedralAngle(Triangle0,Triangle3,InputPoints);
		}
		else 
			Angle3=0;

		TempAngle=MAX3(Angle1,Angle2,Angle3);
		TempAngle=MAX3(WAngle[im],WAngle[mk],TempAngle);

		if ((TempAngle<WAngle[ik])||((fabs(TempAngle-WAngle[ik])<tolerance)&&(TempArea<WArea[ik])))
		{

			if ((NumberOfForbiddenConnections==0)||(CheckIfTriangleNorForbidden(v[i],v[k],v[m],ForbiddenConnectionList,NumberOfForbiddenConnections)))
			{
				if(IntersectTriangleWithPolyData(v[i],v[k],v[m],InputPoints,IntersectionCells)==0)
				{
					WArea[ik]=TempArea;
					WAngle[ik]=TempAngle;
					O[ik]=m; 
				}
			}
		}
	}
}
//-------------------------------------------------------
void CrenellationWeightingFunction::SetInitWeight(int i)
{

	WArea[i+(i+2)*size]=TriangleArea(InputPoints,v[i],v[i+1],v[i+2]);
	// pierwszy trojkat tworzony z kolejnych punktow krawedzi
	Triangle0[0]=v[i];
	Triangle0[1]=v[i+1];
	Triangle0[2]=v[i+2];
	// dwa pozostale trojkaty biore z istniejacych trojkatow przyleglych do brzegu
	AdjacentPolyData->GetCell(i,TriangleCell1);
	AdjacentPolyData->GetCell(i+1,TriangleCell2);

	int j;
	for(j=0;j<3;j++)
	{
		Triangle1[j]=TriangleCell1->GetPointId(j);
		Triangle2[j]=TriangleCell2->GetPointId(j);
	}

	double Angle1,Angle2=0;
	Angle1=GetDihedralAngle(Triangle0,Triangle1,InputPoints);
	Angle2=GetDihedralAngle(Triangle0,Triangle2,InputPoints);

	WAngle[i+(i+2)*size]=MAX(Angle1,Angle2);  
}
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//MinimalAreaWeightingFunciton
MinimalAreaWeightingFunciton::MinimalAreaWeightingFunciton(int *tab,int _size,vtkPolyData *_InputPolyData,bool _CheckIntersection):TriangulationWeightingFunction(tab,_size,_InputPolyData,_CheckIntersection)
{			
	WArea=new double[(size+1)*(size+1)];
	IntersectionCells=NULL;
	TriangleCell=vtkGenericCell::New();
	if (CheckIntersection==true) GetIntersectionCells();
	tolerance=1e-6;
}
//-------------------------------------------------------
MinimalAreaWeightingFunciton::~MinimalAreaWeightingFunciton()
{
	if (WArea!=NULL) delete WArea;
	if (IntersectionCells!=NULL) IntersectionCells->Delete();
	TriangleCell->Delete();

}
//------------------------------------------------------
void MinimalAreaWeightingFunciton::GetIntersectionCells()
{
	int i,j;
	double *Bounds;
	Bounds=GetBoundaryBoundingBox(v,size,InputPoints);
	if (IntersectionCells!=NULL)  IntersectionCells->Delete();
	IntersectionCells=vtkCellArray::New();

	vtkIdType triangle[3];
	for(i=0;i<InputPolyData->GetNumberOfCells();i++)
	{
		InputPolyData->GetCell(i,TriangleCell);
		for(j=0;j<3;j++)
			triangle[j]=TriangleCell->GetPointId(j);
		if (TriangleInsideBoundingBox(triangle[0],triangle[1],triangle[2],InputPoints,Bounds))
			IntersectionCells->InsertNextCell(3,triangle); 
	}

	delete []Bounds;

}
//-------------------------------------------------------
void MinimalAreaWeightingFunciton::SetWeight(int i,int k,int value)
{
	WArea[i+k*size]=0;
}
//-------------------------------------------------------
void MinimalAreaWeightingFunciton::FindMinWeight(int i,int k)
{	
	int im,ik,mk;//,l;
	double	TempArea;
	ik=i+size*k;
	WArea[ik]=999999999;

	for(int m=i+1;m<k;m++)
	{
		im=i+size*m;
		mk=m+size*k;

		TempArea=WArea[im]+WArea[mk]+TriangleArea(InputPoints,v[i],v[m],v[k]);
		if (TempArea<WArea[ik])
		{
			if ((NumberOfForbiddenConnections==0)||(CheckIfTriangleNorForbidden(v[i],v[k],v[m],ForbiddenConnectionList,NumberOfForbiddenConnections)))
			{
				if(IntersectTriangleWithPolyData(v[i],v[k],v[m],InputPoints,IntersectionCells)==0)
				{
					WArea[ik]=TempArea;
					O[ik]=m; 
				}
			}
		}
	}
}
//-------------------------------------------------------
void MinimalAreaWeightingFunciton::SetInitWeight(int i)
{
	WArea[i+(i+2)*size]=TriangleArea(InputPoints,v[i],v[i+1],v[i+2]);
}
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
bool FillPolyDataHole(vtkPolyData *InputPolyData,int *PointTab,int NrOfPoints,TriangulationWeightingFunction *WFunc,vtkCellArray *Polys)
{
	if (Polys==NULL) return false; // tymczasowo

	int i,j,k;
	int n=NrOfPoints;
	int *v=PointTab;

	int NrTempCells=0;

	bool trial;

	for(i=0;i<=n-2;i++)
		WFunc->SetWeight(i,i+1,0);	
	for(i=0;i<=n-3;i++)
		WFunc->SetInitWeight(i);

	j=2;
	do
	{
		j+=1;
		for (i=0;i<=n-j-1;i++)
		{
			k=i+j;
			WFunc->FindMinWeight(i,k);	
		}
	}while(j<n-1);


	///// zbieranie do kupy
	vtkCellArray *newPolys=vtkCellArray::New();
	//vtkIdType *TriangleCell;
	vtkIdType TriangleNpts;
	vtkGenericCell* TriangleCell;
	vtkIdList* pointList = vtkIdList::New();
	int *O=WFunc->GetTriangulationTab();
	try
	{
		Trace(newPolys,O,v,n,0,n-1);
		NrTempCells=newPolys->GetNumberOfCells();
		for(i=0;i<NrTempCells;i++)
		{	

			newPolys->GetCell(i, pointList);
			//newPolys->GetNextCell(TriangleNpts,TriangleCell);

			Polys->InsertNextCell(pointList);
		}
		trial=true;
	}
	catch(...)
	{
		trial=false;
	}

	newPolys->Delete();
	newPolys=NULL;

	return trial;
}
//---------------------------------------------------------------
vtkPolyData		*FillPolyDataHolesWithCrenellations( vtkPolyData *InputPolyData)
{
	PolyDataHolesExtractor *HolesExtractor=new SimplePolyDataHolesExtractor();	
	HolesExtractor->ExtractHoles(InputPolyData,true);

	int i;
	vtkCellArray *FinalPolys= vtkCellArray::New();
	TriangulationWeightingFunction *WFunc=NULL;

	int NumberOfFailed=0;
	for(i=0;i<HolesExtractor->GetNumberOfBoundaries();i++)
	{
		WFunc=new CrenellationWeightingFunction(HolesExtractor->GetBoundaryTab(i),HolesExtractor->GetLengthOfBoundaryTab(i),InputPolyData,true);
		WFunc->SetForbiddenConnectionList(HolesExtractor->GetForbiddenConnectionList(),HolesExtractor->GetNumberOfForbiddenConnections());
		if(FillPolyDataHole(InputPolyData,HolesExtractor->GetBoundaryTab(i),HolesExtractor->GetLengthOfBoundaryTab(i),WFunc,FinalPolys)==false)
			NumberOfFailed++;
		delete WFunc;
	}
	if (NumberOfFailed>0)
		QMessageBox::about( 0,"ERROR","couldn't close"+QString::number(NumberOfFailed)+" Holes");
	return CreateFillHolesPolydataOutput(InputPolyData,FinalPolys,InputPolyData->GetPoints());
}
//---------------------------------------------------------------
vtkPolyData		*FillPolyDataHolesWithMinimalArea( vtkPolyData *InputPolyData)
{
	PolyDataHolesExtractor *HolesExtractor=new SimplePolyDataHolesExtractor();	
	HolesExtractor->ExtractHoles(InputPolyData,true);

	int i;
	vtkCellArray *FinalPolys= vtkCellArray::New();
	TriangulationWeightingFunction *WFunc=NULL;
	int NumberOfFailed=0;
	for(i=0;i<HolesExtractor->GetNumberOfBoundaries();i++)
	{
		WFunc=new MinimalAreaWeightingFunciton(HolesExtractor->GetBoundaryTab(i),HolesExtractor->GetLengthOfBoundaryTab(i),InputPolyData,true);
		WFunc->SetForbiddenConnectionList(HolesExtractor->GetForbiddenConnectionList(),HolesExtractor->GetNumberOfForbiddenConnections());
		if(FillPolyDataHole(InputPolyData,HolesExtractor->GetBoundaryTab(i),HolesExtractor->GetLengthOfBoundaryTab(i),WFunc,FinalPolys)==false)
			NumberOfFailed++;
		delete WFunc;
	}
	if (NumberOfFailed>0)
		QMessageBox::about( 0,"ERROR","couldn't close"+QString::number(NumberOfFailed)+" Holes");
	return CreateFillHolesPolydataOutput(InputPolyData,FinalPolys,InputPolyData->GetPoints());
}
//---------------------------------------------------------------------------
vtkPolyData		*FillPolyDataPlanarHolesWithMinimalArea( vtkPolyData *InputPolyData)
{
	PolyDataHolesExtractor *HolesExtractor=new SimplePolyDataHolesExtractor();	
	HolesExtractor->ExtractHoles(InputPolyData);

	int i;
	vtkCellArray *FinalPolys= vtkCellArray::New();
	TriangulationWeightingFunction *WFunc=NULL;
	int NumberOfFailed=0;
	for(i=0;i<HolesExtractor->GetNumberOfBoundaries();i++)
	{
		WFunc=new MinimalAreaWeightingFunciton(HolesExtractor->GetBoundaryTab(i),HolesExtractor->GetLengthOfBoundaryTab(i),InputPolyData);
		if(FillPolyDataHole(InputPolyData,HolesExtractor->GetBoundaryTab(i),HolesExtractor->GetLengthOfBoundaryTab(i),WFunc,FinalPolys)==false)
			NumberOfFailed++;
		delete WFunc;
	}
	if (NumberOfFailed>0)
		QMessageBox::about( 0,"ERROR","couldn't close"+QString::number(NumberOfFailed)+" Holes");
	return CreateFillHolesPolydataOutput(InputPolyData,FinalPolys,InputPolyData->GetPoints());
}
//--------------------------------------------------------------------
vtkPolyData	*FillPolyDataExtractedPlanarHoles( vtkPolyData *InputPolyData)
{
	PolyDataHolesExtractor *HolesExtractor=new PlanarPolyDataHolesExtractor();	
	HolesExtractor->ExtractHoles(InputPolyData);

	int i;
	vtkCellArray *FinalPolys= vtkCellArray::New();
	TriangulationWeightingFunction *WFunc=NULL;
	int NumberOfFailed=0;
	for(i=0;i<HolesExtractor->GetNumberOfBoundaries();i++)
	{
		WFunc=new MinimalAreaWeightingFunciton(HolesExtractor->GetBoundaryTab(i),HolesExtractor->GetLengthOfBoundaryTab(i),InputPolyData);
		if(FillPolyDataHole(InputPolyData,HolesExtractor->GetBoundaryTab(i),HolesExtractor->GetLengthOfBoundaryTab(i),WFunc,FinalPolys)==false)
			NumberOfFailed++;
		delete WFunc;
	}
	if (NumberOfFailed>0)
		QMessageBox::about( 0,"ERROR","couldn't close"+QString::number(NumberOfFailed)+" Holes");
	return CreateFillHolesPolydataOutput(InputPolyData,FinalPolys,InputPolyData->GetPoints());

}
//--------------------------------------------------------------------
void	Trace(vtkCellArray *Polys,int *O,int *v,int n,int i,int k)
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
		ID=Polys->InsertNextCell(3,Triangle); 
	}
	else
	{
		o=O[i+k*n];
		if (o!=i+1) Trace(Polys,O,v,n,i,o);
		Triangle[0]=v[i];
		Triangle[1]=v[o];
		Triangle[2]=v[k];
		ID=Polys->InsertNextCell(3,Triangle); // DODAJE SOBIE TROJKATERASA - A CO WOLNO MI :)
		if (o!=k-1) Trace(Polys,O,v,n,o,k);
	}
}


// poprawka
// w lewo
/*k=-1;
do
{
k++;
posT=(j+NTPts-k-1)%NTPts;
Points->GetPoint(PointTabTemp[posT],Point);
planedist=MegaPlane->CheckPoint(Point);
test=fabs(planedist)<tolerance;
}while((test)&&(k<NTPts-3));
left=k;
pos=(j+NTPts-left-1)%NTPts;//to bedzie ewentualny startowy
// w prawo
k=-1;
do
{
k++;
posT=(k+j+3)%NTPts;
Points->GetPoint(PointTabTemp[posT],Point);
planedist=MegaPlane->CheckPoint(Point);
test=fabs(planedist)<tolerance;
}while((test)&&(k<NTPts-3-left));
right=k;
*/

//////////////////////////
bool TestAdjacentTriangles(int p1,int p2,int p3,int pos,double min, double max, double tol, 
						   int *AllowedList,int NrOfAllowed, 
						   vtkPolyData *AdjacentPolyData,vtkGenericCell *TriangleCell,double &Angle)
{
	int Triangle0[3];
	int Triangle1[3];
	int p;
	Triangle0[0]=p1;
	Triangle0[1]=p2;
	Triangle0[2]=p3;
	int common=0;
	double tAngle=0;
//	int i;
//	int sum;
//	int step;


	AdjacentPolyData->GetCell(pos,TriangleCell);
	for(p=0;p<3;p++)
		Triangle1[p]=TriangleCell->GetPointId(p);
	// jesli liczba wspolnych pktow ==2 to mozna liczyc dalej

	if ((Triangle0[0]==Triangle1[0])||(Triangle0[0]==Triangle1[1])||(Triangle0[0]==Triangle1[2])) common++;
	if ((Triangle0[1]==Triangle1[0])||(Triangle0[1]==Triangle1[1])||(Triangle0[1]==Triangle1[2])) common++;
	if ((Triangle0[2]==Triangle1[0])||(Triangle0[2]==Triangle1[1])||(Triangle0[2]==Triangle1[2])) common++;
	if (common==2)
	{	
		tAngle=57.295779513*GetOrientedDihedralAngle(Triangle1,Triangle0,AdjacentPolyData->GetPoints());
		Angle=tAngle;
		if ((((tAngle-min>tol)&&(tAngle-max<tol)))||(tAngle>360)) // bo czasem nic nie wynika z kata dwusciennego
			return true;
	}else if (common==1)
	{
		/*step=0;
		for(i=0;i<NrOfAllowed;i++)
		{	

		//sprawdzic czy w liscie AllowedList jest jakis odcinek ktory nalezy do trojkata Triangle0
		// jesli tak zwroc true
		// a nie to false
		sum=0;
		if ((Triangle0[0]==AllowedList[step])||(Triangle0[0]==AllowedList[step+1])) sum++;
		if ((Triangle0[1]==AllowedList[step])||(Triangle0[1]==AllowedList[step+1])) sum++;
		if ((Triangle0[2]==AllowedList[step])||(Triangle0[2]==AllowedList[step+1])) sum++;
		if (sum==2) return true;
		step+=2;
		}*/
		return true;

		//return false;// zaraz cos z tym musze zrobic (updatowana lista krawedzi ktore sa ok)
	}
	else if (common==3)
	{
		Angle=0;
		return false;
	}

	return false;
}
/////////////////////////////////////////////////////////////////////////
void FindLargestPlanarPointSet(int &posopt,int &sizeopt,vtkPoints *Points, int *PointTab,int Npts,int *AllowConTab,int NrOfAllCon,vtkPolyData *AdjPData,int *OrigPosTab,double *center)
{
	// znajduje punkty wzdluz krzywej
	sizeopt=0;


	double sAngle;
	double dihAngle;
	double Angle=0;
//	double tempAngle;
//	double dihangle;
	int  i,j,k;

	double tAngleopt=0;
	double P[3],PL[3],PR[3],PM[3];
	double P0[3],P1[3],P2[3];
	double PT[3];
	int   p1,p2,p3;
	int   t1,t2,t3;
	bool  test;
	double AMin=1;
	double AMax=179;

	double AMinAdj=0;
	double AMaxAdj=179;
	double tol=1e-4;
	double distmin=0.01; // to trzeba wyznaczac!
	double planedist;
	int   pos=0,posT=0;
	int   temppos;


	int left, right;
	int planesetsize=0;
	PlaneObject   *MegaPlane=new PlaneObject();	
	PlaneObject   *TestPlane=new PlaneObject();	
	vtkGenericCell *TriangleCell=vtkGenericCell::New();

//	int Triangle0[3];
//	int Triangle1[3];


//	double pd;
	//	PlaneObject   *TestPlane2=new PlaneObject();	

//	FILE *Do_opa;
	for(j=0;j<Npts;j++)
	{

		p1=PointTab[j%Npts];	 p2=PointTab[(j+1)%Npts];  p3=PointTab[(j+2)%Npts];
		Points->GetPoint(p1,PL); Points->GetPoint(p2,PM);  Points->GetPoint(p3,PR);

		Angle=-99999999;
		test= TestAdjacentTriangles(p1,p2,p3,OrigPosTab[j%Npts],
			AMinAdj,AMaxAdj, tol,AllowConTab,NrOfAllCon,
			AdjPData,TriangleCell,dihAngle);

		Angle=MegaPlane->SetPlanePoints(PL,PM,PR);
		sAngle=Angle;

		// ustalam liczbe pktow nalezacych do 1 plaszczyzny
		if(((Angle-AMin>tol)&&(Angle-AMax<tol))&&(test==true))
		{
			// w lewo
			//////////////////////////////////////////////////////////////////////////////
			k=-1;
			do
			{
				k++;
				posT=(j+Npts-k-1)%Npts;
				Points->GetPoint(PointTab[posT],P);
				planedist=MegaPlane->CheckPoint(P);
				test=fabs(planedist)<distmin;
				/////////////////////////////////////////
				/*if (TestAdjacentTriangles(PointTab[posT],
				PointTab[(j+Npts-k)%Npts],PointTab[(j+Npts-k+1)%Npts],OrigPosTab[posT],
				AMinAdj,AMaxAdj, tol,AllowConTab,NrOfAllCon,
				AdjPData,TriangleCell,Angle)==false) test=false;*/
				///////////////////////////////////////////////
				/*if (test==true)
				{
				tempAngle=MegaPlane->SetPlanePoints(P,PM,PR);
				if ((tempAngle>0)&&(fabs(tempAngle-90.0))<(fabs(Angle-90.0)))// bardziej zblizony do 90%
				{PL[0]=P[0]; PL[1]=P[1]; PL[2]=P[2]; Angle=tempAngle;}
				else
				MegaPlane->SetPlanePoints(PL,PM,PR);
				}*/
			}while((test==true)&&(k<Npts-3));
			left=k;

			if (test==false)
				pos=(j+Npts-left)%Npts;//to bedzie ewentualny startowy
			else
				pos=(j+Npts-left-1)%Npts;//to bedzie ewentualny startowy

			// w prawo
			/////////////////////////////////////////////////////////////////////////////
			k=-1;
			do
			{
				k++;
				posT=(k+j+3)%Npts;
				Points->GetPoint(PointTab[posT],P);
				planedist=MegaPlane->CheckPoint(P);
				test=fabs(planedist)<distmin;
				/////////////////////////////////////////
				/*if (TestAdjacentTriangles(PointTab[(k+j+1)%Npts],
				PointTab[(k+j+2)%Npts],PointTab[posT],OrigPosTab[(k+j+1)%Npts],
				AMinAdj,AMaxAdj, tol,AllowConTab,NrOfAllCon,
				AdjPData,TriangleCell,Angle)==false) test=false;*/
				///////////////////////////////////////////////
			/*	if (test==true)
				{
				tempAngle=MegaPlane->SetPlanePoints(PL,PM,P);
				if ((tempAngle>0)&&(fabs(tempAngle-90.0))<(fabs(Angle-90.0)))	// bardziej zblizony do 90%							
				{PR[0]=P[0]; PR[1]=P[1]; PR[2]=P[2]; Angle=tempAngle;}
				else
				MegaPlane->SetPlanePoints(PL,PM,PR);
				}*/
			}while((test==true)&&(k<Npts-3-left));
			right=k;

			planesetsize=right+left+3;;
		}
		else
		{
			planesetsize=-1;
		}


		//Do_opa=fopen("c:\\a.txt","a");fprintf(Do_opa,"j=%d \t (%d,%d,%d) \t angle=%lf \t dih=%lf \t size=%d\n",j,p1,p2,p3,sAngle,dihAngle,planesetsize);fclose(Do_opa);
		if ((planesetsize>sizeopt)||((planesetsize==sizeopt)&&(tAngleopt>fabs(Angle))))
		{



			//123 //158 //132 //221 21
			if ((j==99))
			{
				pos=pos;
			}
			//////////////////
			// sprawdze czy srodkowy punkt lezy po wlasciwej stronie...
			t1=pos;
			t2=(pos+planesetsize-1)%Npts;
			t3=(pos+(planesetsize-1)/2)%Npts;


			p1=PointTab[t1];//lewy
			p2=PointTab[t2];//prawy
			p3=PointTab[t3];//srodek

			Points->GetPoint(p1,P0);// lewy
			Points->GetPoint(p2,P1);//prawy
			Points->GetPoint(p3,P);//srodek

			MegaPlane->SetPlanePoints(P0,P,P1);

			// sprawdzic czy wszystkie pkty sa po tej samej stronie

			//MegaPlane->SetPlanePoints(P0,P1,P);

			// trzeci punkt ma tworzyc z p1 wekt prostopadly do plaszczyzny w ktorej lezy dziura
			P2[0]=P1[0]+MegaPlane->xa;  //+a
			P2[1]=P1[1]+MegaPlane->ya;  //+b
			P2[2]=P1[2]+MegaPlane->za;  //+c
			TestPlane->SetPlanePoints(P0,P1,P2);

			int exit=false;

			Points->GetPoint(PointTab[(pos+1)%Npts],PT);
			int sign=(TestPlane->CheckPoint(PT)>0)?1:-1;

			for(i=5;i<planesetsize-3;i++)
			{
				Points->GetPoint(PointTab[(pos+i+2)%Npts],PT);
				if (TestPlane->CheckPoint(PT)*sign<0) //rozne znaki
				{
					exit=true;
					break;
				}

			}

			if (exit==false)
			{

				//TestPlane->SetPlanePoints(P0,P2,P1);
				//planedist=TestPlane->CheckPoint(P);
				//Points->GetPoint(1021,P);// testowy
				//pd=MegaPlane->CheckPoint(P);
				///////////////

/*
// chyba bez sensu
				Triangle0[0]=p3;
				Triangle0[1]=p2;
				Triangle0[2]=p1;

				AdjPData->GetCell(OrigPosTab[t2],TriangleCell);
				for(i=0;i<3;i++)
					Triangle1[i]=TriangleCell->GetPointId(i);

				dihangle=GetOrientedDihedralAngle(Triangle1,Triangle0,AdjPData->GetPoints());
*/
				//pd=MegaPlane->CheckPoint(center);
				//if (pd<0)
				///////////////
				//if (t1>t2) 
				//{
				//planedist*=-1;
				///}
				//	pd=TestPlane2->CheckPoint(P);
				//	if (planesetsize<61)

				if ((planedist>0))
				{
					//----------------
					//////////////////
					posopt=pos;
					sizeopt=planesetsize;
					temppos=j;
					tAngleopt=Angle;
				}
			}
		}

	}
	//Do_opa=fopen("c:\\a.txt","a");fprintf(Do_opa,"POSOPT=%d t[popt]=%d size=%d Angle=%lf\n",temppos,PointTab[temppos],sizeopt,tAngleopt);fclose(Do_opa);

	delete MegaPlane;
	delete TestPlane;
	//	delete TestPlane2;

	TriangleCell->Delete();

}
