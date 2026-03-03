#include "ExplodeSegmentation.h"

RayMapMaxGradMedianPolyFilter::RayMapMaxGradMedianPolyFilter(int _maxGrad, int _lth, int _rth, int _sth, int _thMode, double _neigh,  float _neighPrecent ) 
{
	lth=_lth;
	rth=_rth;
	neighDist=_neigh;
	
	neighPercent=_neighPrecent;
	mth=_maxGrad;

	if (_sth!=0) 
		sth=_sth;
	else 
		sth=rth+200;

	thMode=_thMode;

}
//-0---------
//RayMapMaxGradMedianFilter::~RayMapMaxGradMedianFilter()
//{
//
//}
//---------

//---------------------------------------
void	RayMapMaxGradMedianPolyFilter:: ApplyFiltering(vtkPolyData *polys,RawDataSet *rayData,RawDataSet *rayHistData, RawDataSet *rayMapStatusData)
{

	//musze zbudowac tablice z zebranymi otoczeniami
	int maxNeigh=100;
	int maxIter=10;
	int *lenTab=new int [maxNeigh];
	int median;
	int chgs=0;
	double percent=.01;
	
	vtkPoints *pts=polys->GetPoints();
	int nrPts=pts->GetNumberOfPoints();
	int *neighTab=new int[nrPts*maxNeigh];
	int *medTab=new int[nrPts];
	int *maxGradTab=new int[nrPts];
	memset(maxGradTab,0,nrPts*sizeof(int));
	memset(neighTab,0,nrPts*maxNeigh*sizeof(int));

	//unsigned short *rayHistDataTab=rayHistData->GetDataArray();

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
	int *tempTab;
	
	int nrrr;
	vtkIdType cellId,np,np2;
	int valMax;


	double meanDiff;
	
	//zbieram otoczenia i max grad
	
	

		//maxDiff=0;
		for(i=0;i<nrPts;i++)
		{
			polys->GetPointCells(i,cells);
			nrrr=cells->GetNumberOfIds();

			pos=i*maxNeigh;

			pts->GetPoint(i,p);
			pos1=0;
		//	valMax=0;
		//	val1=
			for(j=0;j<nrrr;j++)
			{
				cellId=cells->GetId(j);
				np2=polys->GetCellType(cellId);
				if ( np2 == VTK_TRIANGLE )
				{
					polys->GetCellPoints(cellId,pointList);
					for(k=0;k<pointList->GetNumberOfIds();k++)
					{
						np=pointList->GetId(k);
						if (np==i) continue;
						pts->GetPoint(np,p1);
						pos1++;
						neighTab[pos+pos1]=np;
						neighTab[pos]++;

						if (neighTab[pos]>100)
						{
							i=i;
						}
						//diff=GetDistance(p,p1);
						//if (diff>maxDiff) maxDiff=diff;
					}

				}
			
			}
			//maxGradTab[i]=maxDiff;
		}
		//wybieram sasiadow
		for(i=0;i<nrPts;i++)
		{
			tempTab=neighTab+i*maxNeigh;
			QSortInt(tempTab,1,tempTab[0]);;
			
			tempTab[0]/=2;
			for(j=0;j<tempTab[0];j++)
			{
				tempTab[j+1]=tempTab[j*2+1];
			}
			
		}

		//sortuje po gradiencie
		//QSortDouble(maxGradTab,0,nrPts-1);
		/*meanDiff=0;
		for(i=0;i<nrPts;i++)
		{
			meanDiff+=maxGradTab[i];
		}
		meanDiff/=(double)nrPts;*/


	for(k=0;k<maxIter;k++)
	{

		for(i=0;i<nrPts;i++)
		{
			//if ((maxGradTab[i]<minus)||(maxGradTab[i]>plus))
			val1=rayData->GetVoxelValue(0,i,0);
			maxDiff=0;
			{
				tempTab=neighTab+i*maxNeigh;
				for(j=0;j<tempTab[0];j++)
				{
					//pos=rayData->GetVoxelPos(tempTab[j+1],0,0);
					lenTab[j]=rayData->GetVoxelValue(0,tempTab[j+1],0);
					//lenTab[j]=ttab[tempTab[j+1]*512];
				}

				for(j=0;j<tempTab[0];j++)
				{
					diff=abs(val1-lenTab[j]);
					if (diff>maxDiff) maxDiff=diff;
				}
				
				maxGradTab[i]=maxDiff;
			//	median=Median(lenTab,tempTab[0]);
				//rayData->SetVoxelValue(0,tempTab[j+1],0,median);
				//medTab[i]=median;
				//ttab[tempTab[j+1]*512]=median;
				chgs++;
			}

		}

		meanDiff=0;
		for(i=0;i<nrPts;i++)
		{
			meanDiff+=maxGradTab[i];
		}
		meanDiff/=(double)nrPts;


		/// mam liste sasiadow teraz zrobide mediane

		unsigned short* ttab=rayData->GetDataArray();
		
		chgs=0;
		int minus=Round((double)meanDiff*(1.0-percent));
		int plus=Round((double)meanDiff*(1.0+percent));
		for(i=0;i<nrPts;i++)
		{
			//if ((maxGradTab[i]<minus)||(maxGradTab[i]>plus))
			if (maxGradTab[i]>20)
			{
				tempTab=neighTab+i*maxNeigh;
				for(j=0;j<tempTab[0];j++)
				{
					//pos=rayData->GetVoxelPos(tempTab[j+1],0,0);
					lenTab[j]=rayData->GetVoxelValue(0,tempTab[j+1],0);
					//lenTab[j]=ttab[tempTab[j+1]*512];
				}
				median=Median(lenTab,tempTab[0]);
				//rayData->SetVoxelValue(0,tempTab[j+1],0,median);
				if( (k>maxIter-2)|| ((rayData->GetVoxelValue(median,i,0)<this->rth)&&(rayData->GetVoxelValue(median,i,0)>this->lth)))
				{
					medTab[i]=median;
					ttab[tempTab[j+1]*512]=median;
					chgs++;
					rayData->SetVoxelValue(0,i,0,medTab[i]);
				}
			}

		}

		//for(i=0;i<nrPts;i++)
		//{
		//	rayData->SetVoxelValue(0,i,0,medTab[i]);
		//}
	}
	//while (chgs<00);

	delete []lenTab;
	pointList->Delete();
	cells->Delete();
	
	delete	[]neighTab;
	delete	[]maxGradTab;
	delete []medTab;




	//for(i=0;i<nrOfPts;i++)
	//{
	//	pts->GetPoint(i,p0);//teraz musze wysledzic punkty okoliczne w odleglosci<dist
	//	val1=ray2DHistDataTab[i];
	//	for(j=0;j<nrOfPts/2+1;j++)
	//	{
	//		pts->GetPoint(j,p1);
	//		dist=GetDistance(p0,p1);
	//		val2=rayHistDataTab[j];
	//		if (dist<neighDist)
	//		{
	//			pos=i*maxNeigh;
	//			pos1=neighTab[pos];
	//			if (pos1<maxNeigh-1)
	//			{
	//				pos1++;
	//				neighTab[pos+pos1]=j;
	//				neighTab[pos]++;
	//			}

	//			pos=j*maxNeigh;
	//			pos1=neighTab[pos];
	//			if (pos1<maxNeigh-1)
	//			{
	//				pos1++;
	//				neighTab[pos+pos1]=i;
	//				neighTab[pos]++;
	//			}

	//			diff=fabs(val1-val2);
	//			if (maxGradTab[i]<diff) maxGradTab[i]=diff;
	//			if (maxGradTab[j]<diff) maxGradTab[j]=diff;
	//		}
	//		
	//	}
	//}

	//delete []neighTab;
	//mam zebrane w postaci:

	// k(i)sasiadow 1 2 3 ... k(i)
	
//
//	int size=(neighSize*2+1)*(neighSize*2+1)*neighPercent;
//
//	RawDataSet *ray2DHistDataB=new RawDataSet();
//	ray2DHistDataB->Copy(*ray2DHistData,true);
//
//	RawDataSet *flagData=new RawDataSet();
//	flagData->Copy(*ray2DHistData,false);
//	flagData->ZeroQ();
//	unsigned short *flagTab=flagData->GetDataArray();
//
//
////-------
//	int i,j;
//	int grad;
//	int maxGrad;
//	int mx,my;
//	int mVal;
//	int tVal;
//	int xVal;
//	int chgs=0;
//	int step;
//	int x1,x2;
//	float testPrc=0.1;
//	int maxFlag=4;
//	int nrN;
////	int pos;
//	//	int chgs=0;
//	do
//	{
//		chgs++;
//
//		maxGrad=0;
////		pos=2;
//		for(j=0;j<ray2DHistDataB->GetNy();j++)
//		{
//			for(i=0;i<ray2DHistDataB->GetNx();i++)
//			{
//				grad=GetGradValueFromDepthMap(ray2DHistDataB,i,j);
//
//				if (abs(grad)>abs(maxGrad))
//				{
//
//					if (flagTab[i+j*flagData->GetNx()]<maxFlag)
//					{
//						maxGrad=grad;
//						mx=i;
//						my=j;
//					}
//				}
////				pos++;
//			}
//		}
//		if (abs(maxGrad)>mth)
//		{
//		
//			xVal=ray2DHistDataB->GetVoxelValue(mx,my,0);
//			//znalezc nowa na zmiane
//			//mediana na poczatek
//			mVal=GetMedValueFromDepthMap(ray2DHistDataB,mx,my,2);
//
//			//if (!(flagTab[mx+my*flagData->GetNx()]>1)) //byles wiecej niz 1x? to zmieniaj nie sprawdzaj
//			{
//				//- sprawdze czy pomiedzy mediana a wartoscia pierwotna nie zdazyl sie punkt krytyczny jesli tak to na niego ustawiam wartosc!
//				//	step=(mVal-xVal>0)?1:-1;
//				if (mVal<xVal)	
//				{
//					x1=mVal;	x2=xVal;	step=1;
//				}
//				else
//				{
//					x1=xVal;	x2=mVal;	step=1;
//				}
//				if (thMode==0)
//				{
//					for(i=x1;i!=x2;i+=step)
//					{
//						tVal=rayData->GetVoxelValue(mx,i,my);
//						if ((tVal<=lth)||((tVal>sth)))// kryterium definitywnego stopu
//						{
//
//							nrN=GetNrOfNeighborsBetweenThreshInDepthMap(ray2DHistDataB,neighSize, (1-testPrc)*i, (1+testPrc)*i, mx,my);
//							if (nrN>=size) //jesli z otoczenia 7x7 s5 sasiadow bedzie tez definityw to zmiana
//							{
//								mVal=i;
//								break;
//							}
//						}
//
//					}
//				}
//				else
//				{
//					for(i=x1;i!=x2;i+=step)
//					{
//						tVal=rayData->GetVoxelValue(mx,i,my);
//						if (!((tVal<=lth)||((tVal>sth))))// kryterium definitywnego stopu
//						{
//
//							nrN=GetNrOfNeighborsBetweenThreshInDepthMap(ray2DHistDataB,neighSize, (1-testPrc)*i, (1+testPrc)*i, mx,my);
//							if (nrN>=size) //jesli z otoczenia 7x7 s5 sasiadow bedzie tez definityw to zmiana
//							{
//								mVal=i;
//								break;
//							}
//						}
//
//					}
//				}
//			}
//
//
//			//--------------------------
//
//
//			ray2DHistDataB->SetVoxelValue(mx,my,0,mVal);
//
//			if (rayMapStatusData!=NULL)
//			{
//				if (mVal<xVal)	
//					{
//						rayMapStatusData->SetVoxelValue(mx,my,0,512-(xVal-mVal));
//					}
//				else
//				{
//					rayMapStatusData->SetVoxelValue(mx,my,0,-(xVal-mVal));
//				}
//			}
//			//chgs++;
//			//flagData->SetVoxelValue(mx,my,0,1);
//			flagTab[mx+my*flagData->GetNx()]++;
//		}
//	}while((abs(maxGrad)>mth));
////--------
//
////if (timer!=NULL) timer->TemporaryAppendToMainText("Median Last Pass");
//	//wywalanie bardzo wielkich pikow co zostaly
//
//	for(j=0;j<ray2DHistDataB->GetNy();j++)
//	{
//		for(i=0;i<ray2DHistDataB->GetNx();i++)
//		{
//			xVal=ray2DHistDataB->GetVoxelValue(i,j,0);
//			mVal=GetMedValueFromDepthMap(ray2DHistDataB,i,j,5);
//
//			if (abs(xVal-mVal)>30) 
//			{
//				ray2DHistDataB->SetVoxelValue(i,j,0,mVal);
//			}
//
//		}
//	}
//
//
//
//	memcpy(ray2DHistData->GetDataArray(),ray2DHistDataB->GetDataArray(),ray2DHistDataB->GetVolumeSize()*sizeof(unsigned short));
//
//
//	delete ray2DHistDataB;
//	delete flagData;
}
//------------------------

//------------------------------------------

RayMapMaxGradMedianFilter::RayMapMaxGradMedianFilter(int _maxGrad, int _lth, int _rth, int _sth, int _thMode, int _neighSize, int _neighType,  float _neighPrecent ) 
{
	lth=_lth;
	rth=_rth;
	neighSize=_neighSize;
	neighType=_neighType;
	neighPercent=_neighPrecent;
	mth=_maxGrad;

	if (_sth!=0) 
		sth=_sth;
	else 
		sth=rth+200;

	thMode=_thMode;

}
//-0---------
//RayMapMaxGradMedianFilter::~RayMapMaxGradMedianFilter()
//{
//
//}
//---------

//---------------------------------------
void	RayMapMaxGradMedianFilter:: ApplyFiltering(RawDataSet *rayData,RawDataSet *ray2DHistData, RawDataSet *rayMapStatusData)


{
	

	int size=(neighSize*2+1)*(neighSize*2+1)*neighPercent;

	RawDataSet *ray2DHistDataB=new RawDataSet();
	ray2DHistDataB->Copy(*ray2DHistData,true);

	RawDataSet *flagData=new RawDataSet();
	flagData->Copy(*ray2DHistData,false);
	flagData->ZeroQ();
	unsigned short *flagTab=flagData->GetDataArray();


//-------
	int i,j;
	int grad;
	int maxGrad;
	int mx,my;
	int mVal;
	int tVal;
	int xVal;
	int chgs=0;
	int step;
	int x1,x2;
	float testPrc=0.1;
	int maxFlag=4;
	int nrN;
//	int pos;
	//	int chgs=0;
	do
	{
		chgs++;

		maxGrad=0;
//		pos=2;
		for(j=0;j<ray2DHistDataB->GetNy();j++)
		{
			for(i=0;i<ray2DHistDataB->GetNx();i++)
			{
				grad=GetGradValueFromDepthMap(ray2DHistDataB,i,j);

				if (abs(grad)>abs(maxGrad))
				{

					if (flagTab[i+j*flagData->GetNx()]<maxFlag)
					{
						maxGrad=grad;
						mx=i;
						my=j;
					}
				}
//				pos++;
			}
		}
		if (abs(maxGrad)>mth)
		{
		
			xVal=ray2DHistDataB->GetVoxelValue(mx,my,0);
			//znalezc nowa na zmiane
			//mediana na poczatek
			mVal=GetMedValueFromDepthMap(ray2DHistDataB,mx,my,2);

			//if (!(flagTab[mx+my*flagData->GetNx()]>1)) //byles wiecej niz 1x? to zmieniaj nie sprawdzaj
			{
				//- sprawdze czy pomiedzy mediana a wartoscia pierwotna nie zdazyl sie punkt krytyczny jesli tak to na niego ustawiam wartosc!
				//	step=(mVal-xVal>0)?1:-1;
				if (mVal<xVal)	
				{
					x1=mVal;	x2=xVal;	step=1;
				}
				else
				{
					x1=xVal;	x2=mVal;	step=1;
				}
				if (thMode==0)
				{
					for(i=x1;i!=x2;i+=step)
					{
						tVal=rayData->GetVoxelValue(mx,i,my);
						if ((tVal<=lth)||((tVal>sth)))// kryterium definitywnego stopu
						{

							nrN=GetNrOfNeighborsBetweenThreshInDepthMap(ray2DHistDataB,neighSize, (1-testPrc)*i, (1+testPrc)*i, mx,my);
							if (nrN>=size) //jesli z otoczenia 7x7 s5 sasiadow bedzie tez definityw to zmiana
							{
								mVal=i;
								break;
							}
						}

					}
				}
				else
				{
					for(i=x1;i!=x2;i+=step)
					{
						tVal=rayData->GetVoxelValue(mx,i,my);
						if (!((tVal<=lth)||((tVal>sth))))// kryterium definitywnego stopu
						{

							nrN=GetNrOfNeighborsBetweenThreshInDepthMap(ray2DHistDataB,neighSize, (1-testPrc)*i, (1+testPrc)*i, mx,my);
							if (nrN>=size) //jesli z otoczenia 7x7 s5 sasiadow bedzie tez definityw to zmiana
							{
								mVal=i;
								break;
							}
						}

					}
				}
			}


			//--------------------------


			ray2DHistDataB->SetVoxelValue(mx,my,0,mVal);

			if (rayMapStatusData!=NULL)
			{
				if (mVal<xVal)	
					{
						rayMapStatusData->SetVoxelValue(mx,my,0,512-(xVal-mVal));
					}
				else
				{
					rayMapStatusData->SetVoxelValue(mx,my,0,-(xVal-mVal));
				}
			}
			//chgs++;
			//flagData->SetVoxelValue(mx,my,0,1);
			flagTab[mx+my*flagData->GetNx()]++;
		}
	}while((abs(maxGrad)>mth));
//--------

//if (timer!=NULL) timer->TemporaryAppendToMainText("Median Last Pass");
	//wywalanie bardzo wielkich pikow co zostaly

	for(j=0;j<ray2DHistDataB->GetNy();j++)
	{
		for(i=0;i<ray2DHistDataB->GetNx();i++)
		{
			xVal=ray2DHistDataB->GetVoxelValue(i,j,0);
			mVal=GetMedValueFromDepthMap(ray2DHistDataB,i,j,5);

			if (abs(xVal-mVal)>30) 
			{
				ray2DHistDataB->SetVoxelValue(i,j,0,mVal);
			}

		}
	}



	memcpy(ray2DHistData->GetDataArray(),ray2DHistDataB->GetDataArray(),ray2DHistDataB->GetVolumeSize()*sizeof(unsigned short));


	delete ray2DHistDataB;
	delete flagData;
}
//------------------------

//--------------------------------------
//Create surface form a Thresholded volume
ExplodeSegmentation::ExplodeSegmentation(VtkWidget *v,RibonTimer *_timer)
{
	testFlag=false;
	testData=NULL;
	vWidget=v;
	timer=_timer;
	/*tData=NULL;
	tData2=NULL;
	tDataA=NULL;
	tData2A=NULL;*/


}
//-------------------------------------------

void	ExplodeSegmentation::ExplodeSegmentation3D(vtkPolyData *sourcePolys, PolygonSurfaceData *polysForUpdate, RawDataSet *data, unsigned char *mask, int center[3],int radius[3],RawDataSet *shootEllipoidData,
	int thresh[2], bool invThresh, bool mode, double *probMap, double percentage, double minPerc, double factorPerc, double maxDist, int sphereLevel, SegCriterion *cr, mySphereDistTabsElement *neighTab, double *exportParams)
{
	double p[3];
	double scale[3];
	data->GetScale(scale);
	double minSize = 1;// MIN3(scale[0], scale[1], scale[2]);
	double origin[3];
	data->GetOrigin(origin);
	double centerD[3] = { center[0] * scale[0] + origin[0], center[1] * scale[1] + origin[1], center[2] * scale[2] + origin[2] };

	double centerZero[3] = { 0 };

	//vtkPolyData *sourcePolys = CreateMySphere(sphereLevel, minSize, centerZero);



	//************************

	////testgen
	//for (int j = 3; j < 8; j++)
	//{

	//	vtkPolyData *sourcePolys = this->CreateMySphere(j, 1, centerZero);

	//	QString filename = "spheresource_" + QString::number(j);
	//	PolygonSurfaceData *o2 = new PolygonSurfaceData(sourcePolys, vWidget);	o2->SetPolygonSurfaceDataSetName(filename);	o2->GetVActor()->GetProperty()->SetColor(0.1, 0.1, 1.0);	o2->ComputeGeometricOrigin();
	//	emit	SignalSendNewSurface(o2);
	//}

	//---------------------------


	//vtkPolyData *testPolys = vtkPolyData::New();
	//testPolys->DeepCopy(sourcePolys);


	vtkPoints *pts = sourcePolys->GetPoints();
	
	vtkPolyData *dstPolyData = polysForUpdate->GetVPolyData();
	vtkPoints *dstPoints = dstPolyData->GetPoints();

	int nr = pts->GetNumberOfPoints();
	for (int i = 0; i<nr; i++)
	{
		pts->GetPoint(i, p);
	//	p[0] += center[0];	p[1] += center[1];	p[2] += center[2];
		for (int j = 0; j < 3; j++)
		{
			p[j] = p[j] * minSize;
			p[j] += center[j];
		}
		dstPoints->SetPoint(i, p);
	}


	//SegCriterion *cr = NULL;


	//	--tu bylo
	//\


	double distX = 1.0;
	//RayMapPolyFilter *rf = new RayMapMaxGradMedianPolyFilter(5, lthresh, rthresh, rthresh + 800, 0, distX, 0.5);

	int nrPtsOld = nr;

	
	RayShootSimple(data, mask, dstPolyData, minSize, cr, nrPtsOld, maxDist, center, shootEllipoidData, neighTab, exportParams);

	//if (testFlag == true)
	{
		
	
		
		//pp = vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
		
		for (int i = 0; i < nr; i++)
		{
			dstPoints->GetPoint(i, p);
			for (int j = 0; j < 3; j++)
			{
				p[j] = p[j] * scale[j] + origin[j];
			}
			dstPoints->SetPoint(i, p);
		}
		ComputeNormalsPolyData(dstPolyData);
		polysForUpdate->SetVPolyData(dstPolyData);
//		dstPoints->GetPoint(0, p);
		///dstPolyData->SetPoints(dstPoints);
		polysForUpdate->RebuildSurface();
	

		//o2->ComputeGeometricOrigin();
		
	}

}

//-------------------------------------
ExplodeSegmentation::~ExplodeSegmentation()
{
	//	
}
void	ExplodeSegmentation::ExplodeSegmentation3D(RawDataSet *data, unsigned char *mask, int center[3], int thresh[2], bool invThresh, bool mode, double *probMap, double percentage, double minPerc, double factorPerc,double maxDist, int sphereLevel)
{
	double p[3];
	double scale[3];
	data->GetScale(scale);
	double minSize = MIN3(scale[0],scale[1],scale[2]) ;
	double origin[3];
	data->GetOrigin(origin);
	double centerD[3] = { center[0] * scale[0] + origin[0], center[1] * scale[1] + origin[1], center[2] * scale[2] + origin[2] };

	double centerZero[3] = { 0 };

	vtkPolyData *sourcePolys = CreateMySphere(sphereLevel, minSize,centerZero);



	//************************

	////testgen
	//for (int j = 3; j < 8; j++)
	//{

	//	vtkPolyData *sourcePolys = this->CreateMySphere(j, 1, centerZero);

	//	QString filename = "spheresource_" + QString::number(j);
	//	PolygonSurfaceData *o2 = new PolygonSurfaceData(sourcePolys, vWidget);	o2->SetPolygonSurfaceDataSetName(filename);	o2->GetVActor()->GetProperty()->SetColor(0.1, 0.1, 1.0);	o2->ComputeGeometricOrigin();
	//	emit	SignalSendNewSurface(o2);
	//}

	//---------------------------


	vtkPolyData *testPolys = vtkPolyData::New();
	testPolys->DeepCopy(sourcePolys);


	vtkPoints *pts = testPolys->GetPoints();
	int nr = pts->GetNumberOfPoints();
	for (int i = 0; i<nr; i++)
	{
		pts->GetPoint(i, p);
		p[0] += center[0];	p[1] += center[1];	p[2] += center[2];
		pts->SetPoint(i, p);
	}


	SegCriterion *cr = NULL;

	int neighSize = 2;
	int neighType = 0;//0-cube 1-sphere
	double neighPercentage = 0.3;
	if (mode == true)
	{
		//prob map

		cr = new SegCriterionProbabilityMap(data, probMap, percentage, minPerc, factorPerc, neighPercentage, neighType, neighSize);
	}
	else
	{
		//thresholds
		cr = new SegCriterionThresholds(data, thresh[0], thresh[1], thresh[1], invThresh, neighPercentage, neighType, neighSize); 
	}


	//	--tu bylo
	//\


	double distX = 1.0;
	//RayMapPolyFilter *rf = new RayMapMaxGradMedianPolyFilter(5, lthresh, rthresh, rthresh + 800, 0, distX, 0.5);

	int nrPtsOld = nr;
	RayShootSimple(data, mask, testPolys, minSize, cr, nrPtsOld, maxDist,center);

	//if (testFlag == true)
	{
		vtkPolyData *pp;
		PolygonSurfaceData *o2;
		ComputeNormalsPolyData(testPolys);
		pp = vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
		vtkPoints *pp1 = pp->GetPoints();
		for (int i = 0; i < nr; i++)
		{
			pp1->GetPoint(i, p);
			for (int j = 0; j < 3; j++)
			{
				p[j] = p[j] * scale[j] + origin[j];
			}
			pp1->SetPoint(i, p);
		}



		o2 = new PolygonSurfaceData(pp, vWidget);	o2->SetPolygonSurfaceDataSetName("Brain1 " + data->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1, 0.1, 1.0);	o2->ComputeGeometricOrigin();
		emit	SignalSendNewSurface(o2);
	}

}

//-------------------------------------
void	ExplodeSegmentation::SlotScanlineSegmentBrain(RawDataSet *Data1,unsigned char *Data2,int x,int y,int z,int lthresh,int rthresh,int iter, int minsize)
{
	testFlag=true;

	//emit SendMessage(initText+" ("+QString::number(++pos)+"/"+t+") "+" multires:("+ QString::number(m+1)+"/"+QString::number(multiressteps)+")");
	unsigned char *mask0=new unsigned char [Data1->GetVolumeSize()];
	memset(mask0,0,Data1->GetVolumeSize());

	int i,j,k;
	unsigned short vval;
	unsigned short *data1Array=Data1->GetDataArray();
	for(i=0;i<Data1->GetVolumeSize();i++)
	{
		vval=data1Array[i];
		if(((vval<lthresh)||(vval>rthresh)) || (Data2[i]==1))
		{
			mask0[i]=1; 
			Data2[i]=0; //czyszcze blokade
		}

	}
	//emit SignalSendMask(mask0);

	
//	delete []mask0;return ;

	resolution=60;
	bool medianTumor=true;
	int medianTumorIter=1;

	double xS=Data1->GetXscale();
	double yS=Data1->GetYscale();
	double zS=Data1->GetZscale();
	double scale[3]={xS,yS,zS};

//	resolution=_resolution;
//resolution=60;,
	//robie kulke
	//strzelam promieniami i ja deformuje
	//mask[maxPos] = 2; - na brzegach

	double minSize=MIN3(Data1->GetXscale(),Data1->GetYscale(),Data1->GetZscale())/2;

	//vtkSphereSource *sphere=vtkSphereSource::New();
	//sphere->SetPhiResolution(resolution);
	//sphere->SetThetaResolution(resolution);
	//sphere->SetCenter(x*xS,y*yS,z*zS);
	//sphere->SetRadius(minSize);
	////SphereActor	=	vtkActor::New();
	////vtkPolyDataMapper *SphereMapper	=	vtkPolyDataMapper::New();
	////SphereMapper->SetInputData(Sphere->GetOutput());
	//vtkPolyData *spherePolys=sphere->GetOutput();
	//spherePolys->Update();
	//spherePolys->BuildCells();
	//spherePolys->BuildLinks();
	//testFlag=true;

	//vtkPolyData *testPolys=vtkPolyData::New();
	//testPolys->DeepCopy(spherePolys);
	double origin[3];
	Data1->GetOrigin(origin);
	double center[3]={x*xS+origin[0],y*yS+origin[1],z*zS+origin[2]};

	double centerZero[3]={0};
	vtkPolyData *sourcePolys=CreateMySphere(5,minSize,centerZero);
	vtkPolyData *sourcePolys2=CreateMySphere(4,minSize,centerZero);
	//vtkPolyData *testPolys=this->CreateMySphere(5,minSize,center);

	vtkPolyData *testPolys=vtkPolyData::New();
	testPolys->DeepCopy(sourcePolys);
	//testPolys->Update();

	vtkPoints *pts=testPolys->GetPoints();




	//	--tu bylo
	//\

	int nr=pts->GetNumberOfPoints();


	double n[3];

	
	double dx,dy,dz;
	double length;
	double p[3],p1[3];
	/*vtkPoints *nrls=vtkPoints::New();
	for(i=0;i<nr;i++)
	{
		pts->GetPoint(i,p);
		n[0]=p[0]-center[0];	n[1]=p[1]-center[1];	n[2]=p[2]-center[2];
		nrls->InsertNextPoint(n);

	}

	FindFirstGuessCenterAlongNormals(Data1,lthresh,rthresh,pts, nrls,0.6,center);*/

	int ic[3];
	double ss[3];
	Data1->GetScale(ss);
	//for(i=0;i<3;i++)
	//	ic[i]=center[i]/ss[i];
	////ShootAlongNormals(Data1,lthresh,rthresh,pts, nrls,0.6,center);
	//nrls->Delete();

	for(i=0;i<nr;i++)
	{	
		pts->GetPoint(i,p);
		p[0]+=center[0];	p[1]+=center[1];	p[2]+=center[2];
		pts->SetPoint(i,p);
	}


	RawDataSet *rayMapData=new RawDataSet();
	RawDataSet *rayMapStatusData=new RawDataSet();
	rayMapStatusData->ZeroQ();
	

	double avg,med,param;
	int nrOfPtsX;

	double smoothlevel=0.01;
	int smoothIter=50;
	int count=0;

	
	double minsc=MIN3(ss[0],ss[1],ss[2])/2;
	////////////////////////

	////////////////////////////IIITTTTTTEEEEEEEEEEEEEERRRRRRRRRRR
	int thChange=400;
	SegCriterion *cr=new SegCriterionThresholds(Data1,lthresh,rthresh,rthresh+800,0,2,0,0.3);
	
	double distX=1.0;
	RayMapPolyFilter *rf=new RayMapMaxGradMedianPolyFilter(5,lthresh,rthresh,rthresh+800,0,distX,0.5);
	
	if (testFlag == true)
	{

		vtkPolyData *pp;
		PolygonSurfaceData *o2;
		pp = vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
		o2 = new PolygonSurfaceData(pp, vWidget);	o2->SetPolygonSurfaceDataSetName("Sphere " + Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1, 0.1, 1.0);	o2->ComputeGeometricOrigin();
		emit	SignalSendNewSurface(o2);
	}
	RayShoot(Data1,Data2, testPolys, minSize,cr,rf,0,mask0);

	//count=DistanceRefineTrianglePolyDataFilter(testPolys, minsc);
	SmoothPolyData(testPolys,smoothlevel,smoothIter);	
	
	//zwiekszenie rozdzielczosci
	//resolution +=40;
	//tkPolyData *incPolys=IncreaseRayMapResolution(Data1,resolution,center,testPolys,minSize, rayMapData,rayMapStatusData,cr,rf);
	
	if (testFlag==true)
	{
		vtkPolyData *pp;
		PolygonSurfaceData *o2;
		ComputeNormalsPolyData(testPolys);
		pp=vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
		o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain1 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
		emit	SignalSendNewSurface(o2);
	}
	//00000000000000000000000000000000000000000

	int closingS=4;
	
	Segmentation *seg=new Segmentation();
	int maxIter=iter;
	AddMaskFromStencilBuffer(Data1, Data2, testPolys, closingS,mask0,1);
	//emit SignalSendMask(Data2);
	int ncount, lastcount;
	ncount=0;
	for (j=0;j<Data1->GetVolumeSize();j++)
	{
		if (Data2[j]==1) ncount++;
	}

	//if (testFlag==true)	
		emit SignalSendMask(Data2);
	for(i=0;i<maxIter;i++)
	{
		
	
			
		/*if (i==2)
					{
						testFlag=true;
				}*/
			
			int nrOfCenters=0;
			double *centers=GetIslandList(nrOfCenters,Data1, mask0, Data2,minsize);
			emit SendMessage("ShootSeg (iter:"+QString::number(i)+"/ "+QString::number(maxIter-1)+")");
	//testFlag=true;
			for(j=0;j<nrOfCenters;j++)
			{

				//podmien testPolys za kulke wzorcowa o srodku w centers[i]
				//strzelaj i dodawaj

				testPolys->DeepCopy(sourcePolys2);
				//testPolys->Update();
				pts=testPolys->GetPoints();
				nr=pts->GetNumberOfPoints();
				for(k=0;k<nr;k++)
				{	
					pts->GetPoint(k,p);
					p[0]+=centers[j*3];	p[1]+=centers[j*3+1];	p[2]+=centers[j*3+2];
					pts->SetPoint(k,p);
				}
				//testPolys->Update();
					//---------
				//if ((i==2)&&(j==0)) testFlag=true;
				RayShoot(Data1,Data2, testPolys, minSize,cr,rf,0,mask0);
				SmoothPolyData(testPolys,smoothlevel,smoothIter);	
				ComputeNormalsPolyData(testPolys);

				//---------
				if (testFlag==true)
				{
					vtkPolyData *pp;
					PolygonSurfaceData *o2;
					pp=vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
					o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain1 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
					o2->SetPolygonSurfaceDataSetName("add "+QString::number(j));	
					o2->SetGroupName("add from island");
					o2->GetVActor()->GetProperty()->SetColor(((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX));
					emit	SignalSendNewSurface(o2);
				}
				//-----------
				


				AddMaskFromStencilBuffer(Data1, Data2, testPolys, closingS,mask0);
				if (testFlag==true)emit SignalSendMask(Data2);
				//	testFlag=false;
			/*	if (testFlag==true)
					{	emit SignalSendMask(Data2);
					pp=vtkPolyData::New();	pp->DeepCopy(testPolys);	
					o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain1 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
					o2->SetPolygonSurfaceDataSetName("add "+QString::number(j));	
					o2->GetVActor()->GetProperty()->SetColor(((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX));
					emit	SignalSendNewSurface(o2);
				}
			*/}
			/*emit SignalSendMask(Data2);
			{
				
				vtkPolyData *newPolys=GenerateSmoothedSurface(0, Data1,Data2,true,false);
				o2=new PolygonSurfaceData(newPolys,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain1 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
				o2->SetPolygonSurfaceDataSetName("add "+QString::number(j));	
				o2->GetVActor()->GetProperty()->SetColor(((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX));
	
				int pn[3];
				Data1->GetN(pn);
				o2->SetPN(pn);
				o2->SetPScale(scale);
				emit	SignalSendNewSurface(o2);
			}*/



			lastcount=ncount;
			ncount=0;
			for (j=0;j<Data1->GetVolumeSize();j++)
			{
				if (Data2[j]==1) ncount++;
			}

			if (ncount==lastcount)
			{
				emit SendMessage("Nothing to add at iteration("+QString::number(i)+"/ "+QString::number(maxIter-1)+")");
				break;
	
			}

		
	}
	
	int bounds[6]={10000,0,10000,0,10000,0};
	
	int pos=0;
	
	int NN[3];
	Data1->GetN(NN);
	for(k=0;k<NN[2];k++)
	{
		for(j=0;j<NN[1];j++)
		{
			for(i=0;i<NN[0];i++)
			{

				if (Data2[pos++]>0)

				{
					if(i<bounds[0]) bounds[0]=i; if (i>bounds[1]) bounds[1]=i;
					if(j<bounds[2]) bounds[2]=j; if (j>bounds[3]) bounds[3]=j;
					if(k<bounds[4]) bounds[4]=k; if (k>bounds[5]) bounds[5]=k;
				}
		
			}
		}
	}
	 SlotBinarySimpleMorphology(Data1,Data2,1,1,1,mask0,false,false,bounds);
	emit SignalSendMask(Data2);
	
	vtkPolyData *newPolys=GenerateSmoothedSurface(0, Data1,Data2,false,false);
	vtkPolyData *pp;
	PolygonSurfaceData *o2;
	o2=new PolygonSurfaceData(newPolys,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain1 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
	o2->SetPolygonSurfaceDataSetName("add "+QString::number(j));	
	o2->SetGroupName("final");
	o2->GetVActor()->GetProperty()->SetColor(((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX));
	
	int pn[3];
	Data1->GetN(pn);
	o2->SetPN(pn);
	o2->SetPScale(scale);
	o2->SetIsSegTemp(true);
	emit	SignalSendNewSurface(o2);



	//SlotBinarySimpleMorphology(newData,frontMask,1,1,kernelType,newMask0);
	//pp=vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
	//o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain1 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
	//emit	SignalSendNewSurface(o2);



	//SegCriterion *cr2=new SegCriterionThresholdsWithRayCheck(Data1,lthresh,rthresh-thChange,rthresh-thChange,0,2,1,0.2);

	//RayMapFilter *rf2=new RayMapMaxGradMedianFilter(5,lthresh,rthresh-thChange,rthresh-thChange,0,3,0,0.1);


	//testData=vtkPolyData::New();
	//pierwsze strzelanie


	//vtkPolyData	*testPolys2x=DistanceRefinePolyDataFilter(testPolys, 100.0);
	//testPolys->DeepCopy(testPolys2x);
	//testPolys2x->Delete();
	////SmoothPolyData(testPolys,0.01,50);
	//ComputeNormalsPolyData(testPolys);

//
//	RayShoot(Data1,center,testPolys,minSize,0,NULL, rayMapData,rayMapStatusData,true,resolution,cr,rf);
//	
//	
////
//	//zwiekszenie rozdzielczosci
////	resolution +=40;
//	//vtkPolyData *incPolys=IncreaseRayMapResolution(Data1,resolution,center,testPolys,minSize, rayMapData,rayMapStatusData,cr,rf);
//	//SmoothPolyData(incPolys,0.01,50);
//	//ComputeNormalsPolyData(incPolys);
//	SmoothPolyData(testPolys,0.01,50);
//	ComputeNormalsPolyData(testPolys);
//	if (testData!=NULL) UpdateRayPolyData(testData,center,testPolys);
//
//	testPolys->DeepCopy(testPolys);
//	vtkPolyData *pp=vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
//	PolygonSurfaceData *o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain0 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
//	emit	SignalSendNewSurface(o2);
//
//	//drugie strzelanie
//	RayShoot(Data1,center,testPolys,minSize,0,NULL, rayMapData,rayMapStatusData,false, resolution,cr,rf);
//
//	SmoothPolyData(testPolys,0.01,50);
//	ComputeNormalsPolyData(testPolys);
//	if (testData!=NULL) UpdateRayPolyData(testData,center,testPolys);
//
//	pp=vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
//	o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain1 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
//	emit	SignalSendNewSurface(o2);
//	//	incPolys->Delete();
//
//
//	//trzecie strzelanie
//	RayShoot(Data1,center,testPolys,minSize,0,NULL, rayMapData,rayMapStatusData,false, resolution,cr,rf);
//
//	SmoothPolyData(testPolys,0.01,50);
//	ComputeNormalsPolyData(testPolys);
//	if (testData!=NULL) UpdateRayPolyData(testData,center,testPolys);

	//pp=vtkPolyData::New();	pp->DeepCopy(testPolys);	/*pp->Update();*/
	//o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain3 "+Data1->GetRawDataSetName());	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
	//emit	SignalSendNewSurface(o2);
	//
	////promienie na wsteczbym
	////rayMapStatusData->ZeroQ();// po to zeby odblokowac punkty i umozliwic powrot


	//// proba reperacji - dodaje teraz obszar w jakim ma dzialac
	////double area[4]={0.6,0.0,1.0,1.0};
	////RayShoot(Data1,center,testPolys,-minSize,20,area, rayMapData,rayMapStatusData,false, resolution,cr2,rf2);



	////testPolys->Update();
	//SmoothPolyData(testPolys,0.01,50);
	//ComputeNormalsPolyData(testPolys);
	//if (testData!=NULL)
	//{
	//	UpdateRayPolyData(testData,center,testPolys);
	//	PolygonSurfaceData *o12=new PolygonSurfaceData(testData,vWidget);
	//	o12->SetPolygonSurfaceDataSetName("krechy"+Data1->GetRawDataSetName());
	//	//o1->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);
	//	o12->ComputeGeometricOrigin();
	//	emit	SignalSendNewSurface(o12);
	//}

//		vtkPolyData	*testPolys2=DistanceRefinePolyDataFilter(testPolys, 5.0);
//	testPolys->DeepCopy(testPolys2);
//	testPolys2->Delete();
//	//SmoothPolyData(testPolys,0.01,50);
//	ComputeNormalsPolyData(testPolys);
//
////ComputeNormalsPolyData(testPolys);
//
//	vtkHedgeHog *glyph=vtkHedgeHog::New();
//	glyph->SetInput (testPolys);
//	glyph->SetVectorModeToUseNormal();
//	glyph->SetScaleFactor (20.0);
//	glyph->Update();
//
//	
//	pp=vtkPolyData::New();	pp->DeepCopy(glyph->GetOutput());	/*pp->Update();*/
//	o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Brain3 "+Data1->GetRawDataSetName());		o2->ComputeGeometricOrigin();
//	emit	SignalSendNewSurface(o2);
//
//
//	
//vtkPolyData *ppd=this->CreateMySphere(3,50.0,center);
//o2=new PolygonSurfaceData(ppd,vWidget);	o2->SetPolygonSurfaceDataSetName("x");		o2->ComputeGeometricOrigin();
//	emit	SignalSendNewSurface(o2);
//ppd=this->CreateMySphere(3,50.0,center);
//o2=new PolygonSurfaceData(ppd,vWidget);	o2->SetPolygonSurfaceDataSetName("X");		o2->ComputeGeometricOrigin();
//	emit	SignalSendNewSurface(o2);

	//
	//testPolys2=DistanceRefinePolyDataFilter(testPolys, 10.0);
	//testPolys->DeepCopy(testPolys2);
	//testPolys2->Delete();
	//SmoothPolyData(testPolys,0.01,50);
	//ComputeNormalsPolyData(testPolys);


	
	//vtkLinearSubdivisionDistanceFilter *linear=vtkLinearSubdivisionDistanceFilter::New();
	////vtkLinearSubdivisionFilter	*linear=vtkLinearSubdivisionFilter::New();
	//linear->SetDistance(5.0);
	//linear->SetInputData(testPolys);
	//linear->SetNumberOfSubdivisions(1);
	//linear->Update();
	//
	//vtkPolyData *testPolys2=vtkPolyData::New();
	//testPolys2->DeepCopy(linear->GetOutput());
	//testPolys2->Update();
	////testPolys2->Update();
	//testPolys->DeepCopy(testPolys2);
	//testPolys->Update();

	//testPolys2->Delete();
	//linear->Delete();
	

	//SmoothPolyData(testPolys,0.01,50);
	//ComputeNormalsPolyData(testPolys);
	


	//int nrsc=testData->GetPointData()->GetScalars()->GetSize();
	//vtkLookupTable *ttt=testData->GetPointData()->GetScalars()->GetLookupTable();

	

	//testData->Delete();

	//testData=NULL;

	//wsteczne strzelanie tylko w okreslonym kwadracie

//	double area[4]={0.6,0.0,1.0,1.0};

//	this->RayShoot(Data1,center,spherePolys,-minSize,15,area, rayMapData,rayMapStatusData,resolution,cr,NULL);


	//if (timer!=NULL) timer->TemporaryAppendToMainText("smooth step");
	

	//delete rayMapStatusData;
	//rayMapStatusData->SetRawDataSetName("status");
	//emit SignalSendNewRawDataSet(rayMapStatusData);
	//tDataA=rayMapData;//rayMapStatusData;
	//spherePolys->DeepCopy(incPolys);

	
	delete cr;
//	delete cr2;
	delete rf;
	delete []mask0;
	//delete rf2;



	//--------------------------------


//	SmoothPolyData(testPolys,0.01,50);
//	ComputeNormalsPolyData(testPolys);











}
void ExplodeSegmentation::AddMaskFromStencilBuffer(RawDataSet *data, unsigned char *mask, vtkPolyData *pData, int closingSize,unsigned char *mask0,int addDilate)
{
	int i,j,k;
	double scale[3];
	double origin[3];
	data->GetScale(scale);
	data->GetOrigin(origin);
	int iBounds[6];
	RawDataSet *tmpData= CreateStencilBuffer(pData,data,closingSize,mask0);
	//mam stencil update Maski
	unsigned short *tmpDataArray=tmpData->GetDataArray();
	int x,y,z;
	int nn[3];
	int poss=0;
	int newPos;
	double sss[3];
	double sOrigin[3];
	tmpData->GetN(nn);
	tmpData->GetScale(sss);
	tmpData->GetOrigin(sOrigin);
	double sPos[3];
	double addS[3];
	for(k=0;k<3;k++)
	{
		addS[k]=(sOrigin[k]-origin[k])/scale[k];
	}

	

	for(z=0;z<nn[2];z++)
	{
		for(y=0;y<nn[1];y++)
		{
			for(x=0;x<nn[0];x++)
			{
				if (tmpDataArray[poss++]==1)
				{
					sPos[0]=((x*sss[0]+sOrigin[0])-origin[0])/scale[0];
					sPos[1]=((y*sss[1]+sOrigin[1])-origin[1])/scale[1];
					sPos[2]=((z*sss[2]+sOrigin[2])-origin[2])/scale[2];

					newPos=data->GetVoxelPos(Round(sPos[0]),Round(sPos[1]),Round(sPos[2]));			
					if ((newPos>=0)&&(newPos<data->GetVolumeSize()))
					{	
						mask[newPos]=1;
					}
			
				}

						
			}
		}
	}

			delete tmpData;
}
//------------------------------
double *ExplodeSegmentation::GetIslandList(int &nrOfCenters,RawDataSet *data, unsigned char *mask0, unsigned char *mask,int minsize)
{

	int isleSeparation=1;
	int minNrOfIsleVoxels=minsize;
	//zrobie cropa
	int closingS=1;
	int i,j,k;
	int x,y,z;
	int bounds[6]={10000,0,10000,0,10000,0};
	int start=0;
	int pos=0;
	
	int NN[3];
	data->GetN(NN);
	for(k=0;k<NN[2];k++)
	{
		for(j=0;j<NN[1];j++)
		{
			for(i=0;i<NN[0];i++)
			{

				if (mask[pos++]>0)

				{
					if(i<bounds[0]) bounds[0]=i; if (i>bounds[1]) bounds[1]=i;
					if(j<bounds[2]) bounds[2]=j; if (j>bounds[3]) bounds[3]=j;
					if(k<bounds[4]) bounds[4]=k; if (k>bounds[5]) bounds[5]=k;
				}
		
			}
		}
	}

	//mam bounds zrobie crop
	int size[3];
	double origin[3];
	int posN[3];
	int closingSize=(closingS+isleSeparation)*2+3;
	int pos2;
	double scale[3];
	data->GetScale(scale);
	double origin0[3];
	data->GetOrigin(origin0);

	origin[0]=(bounds[0]-closingSize)*scale[0]+origin0[0];
	origin[1]=(bounds[2]-closingSize)*scale[1]+origin0[1];
	origin[2]=(bounds[4]-closingSize)*scale[2]+origin0[2];
				
	size[0]=bounds[1]-bounds[0]+closingSize*2+1;
	size[1]=bounds[3]-bounds[2]+closingSize*2+1;
	size[2]=bounds[5]-bounds[4]+closingSize*2+1;

	RawDataSet *newData=new RawDataSet(size[0],size[1],size[2],scale[0],scale[1],scale[2]);
	unsigned short *newDataArray=newData->GetDataArray();
	newData->SetOrigin(origin);
	newData->ZeroQ();

	unsigned char *newMask0=new unsigned char[newData->GetVolumeSize()];
	memset(newMask0,0,newData->GetVolumeSize());
	unsigned char *newMask=new unsigned char[newData->GetVolumeSize()];
	memset(newMask,0,newData->GetVolumeSize());



	int testBounds[6];
	testBounds[0]=bounds[0]-closingSize; if (testBounds[0]<0)testBounds[0]=0;
	testBounds[2]=bounds[2]-closingSize; if (testBounds[2]<0)testBounds[2]=0;
	testBounds[4]=bounds[4]-closingSize; if (testBounds[4]<0)testBounds[4]=0;

	testBounds[1]=bounds[1]+closingSize; if (testBounds[1]>NN[0]-1)testBounds[1]=NN[0]-1;
	testBounds[3]=bounds[3]+closingSize; if (testBounds[3]>NN[1]-1)testBounds[3]=NN[1]-1;
	testBounds[5]=bounds[5]+closingSize; if (testBounds[5]>NN[2]-1)testBounds[5]=NN[2]-1;


	for(z=testBounds[4];z<testBounds[5]+1;z++)
	{
			for(y=testBounds[2];y<testBounds[3]+1;y++)
			{
					for(x=testBounds[0];x<testBounds[1]+1;x++)
					{
					
						pos=data->GetVoxelPos(x,y,z);
						
						{
							posN[0]=x-bounds[0]+closingSize;
							posN[1]=y-bounds[2]+closingSize;
							posN[2]=z-bounds[4]+closingSize;
							pos2=newData->GetVoxelPos(posN[0],posN[1],posN[2]);
							
						}
						newMask[pos2]=mask[pos];
						newMask0[pos2]=mask0[pos];

					}
			}
	}





	////ekstrakcja brzegu

	////idz po wszystkich pikselach
	////sprawdzaj czy nie zapalony w frontDataArray
	//// jesli nie to sprawdz czy punkt nalezy do brzegu jelsi tak to sie rozlej
	//

	unsigned char *frontMask=new unsigned char [newData->GetVolumeSize()];
	memcpy(frontMask,newMask,newData->GetVolumeSize());
	unsigned char *frontMask2=new unsigned char [newData->GetVolumeSize()];

	//dylatacja z maska

	int kernelType=1;//0cube 1sphere 2cross
	SlotBinarySimpleMorphology(newData,frontMask,1,closingS,kernelType,newMask0);

	//druga dylatacja
	memcpy(frontMask2,frontMask,newData->GetVolumeSize());
	SlotBinarySimpleMorphology(newData,frontMask2,1,closingS,kernelType,newMask0);

	//if (testFlag==false)
	//maski fr i fr2 zawiaraja tylko dylatacje pierwsza i druga
	for(i=0;i<newData->GetVolumeSize();i++)
	{

		if ((frontMask[i]==1)	&&(frontMask2[i]==1)) 
		{
			frontMask2[i]=0;
		}


		if ((newMask[i]==1)	&&(frontMask[i]==1)) 
		{
			frontMask[i]=0;
		}
	}



	/// usuwam te punkty, ktore naleza do frontMask i w ich otoczeniu o wielkosci K jest newMask0

	//flipnewmask
	if (testFlag==true)
				
										{
										//-------------------------sprawdzanie czy dobrze wycialem
										//-------------------------sprawdzanie czy dobrze wycialem
									unsigned char *testMask=new unsigned char [data->GetVolumeSize()];
									memset(testMask,0,data->GetVolumeSize());
									int newPos;
									pos=0;
									int sPos[3];
									for(z=0;z<size[2];z++)
									{
										for(y=0;y<size[1];y++)
										{
											for(x=0;x<size[0];x++)
											{
												if (frontMask[pos++]==1)
												{
													sPos[0]=Round(((x*scale[0]+origin[0])-origin0[0])/scale[0]);
													sPos[1]=Round(((y*scale[1]+origin[1])-origin0[1])/scale[1]);
													sPos[2]=Round(((z*scale[2]+origin[2])-origin0[2])/scale[2]);

													newPos=data->GetVoxelPos(sPos[0],sPos[1],sPos[2]);			
													if ((newPos>=0)&&(newPos<data->GetVolumeSize()))
													{	
														testMask[newPos]=1;
													}
			
												}

						
											}
										}
									}


									emit SignalSendMask(testMask);


									vtkPolyData *newPolys=GenerateSmoothedSurface(0, data,testMask,false,false);
									PolygonSurfaceData *o2=new PolygonSurfaceData(newPolys,vWidget);	o2->SetPolygonSurfaceDataSetName("x");	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
									o2->SetPolygonSurfaceDataSetName("add "+QString::number(j));	
									o2->SetGroupName("island source");
									//o2->GetVActor()->GetProperty()->SetColor(((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX));
									emit	SignalSendNewSurface(o2);

									delete []testMask;
									}
									//		//-------------------------sprawdzanie czy dobrze wycialem
									//	//-------------------------sprawdzanie czy dobrze wycialem
	//if (minNrOfIsleVoxels==0)
	{
	//	SlotBinarySimpleMorphology(newData,frontMask,0,isleSeparation,kernelType,newMask0,true,true,NULL,frontMask2);//true dualmode
	}
	//else
	//{

		SlotBinarySimpleMorphology(newData,frontMask,0,isleSeparation,kernelType,newMask0,true,true);//true dualmode
	//}
	if (testFlag==true)	
	{
										//-------------------------sprawdzanie czy dobrze wycialem
										//-------------------------sprawdzanie czy dobrze wycialem
									unsigned char *testMask=new unsigned char [data->GetVolumeSize()];
									memset(testMask,0,data->GetVolumeSize());
									int newPos;
									pos=0;
									int sPos[3];
									for(z=0;z<size[2];z++)
									{
										for(y=0;y<size[1];y++)
										{
											for(x=0;x<size[0];x++)
											{
												if (frontMask[pos++]==1)
												{
													sPos[0]=Round(((x*scale[0]+origin[0])-origin0[0])/scale[0]);
													sPos[1]=Round(((y*scale[1]+origin[1])-origin0[1])/scale[1]);
													sPos[2]=Round(((z*scale[2]+origin[2])-origin0[2])/scale[2]);

													newPos=data->GetVoxelPos(sPos[0],sPos[1],sPos[2]);			
													if ((newPos>=0)&&(newPos<data->GetVolumeSize()))
													{	
														testMask[newPos]=1;
													}
			
												}

						
											}
										}
									}


									emit SignalSendMask(testMask);
									vtkPolyData *newPolys=GenerateSmoothedSurface(0, data,testMask,false,false);
									PolygonSurfaceData *o2=new PolygonSurfaceData(newPolys,vWidget);	o2->SetPolygonSurfaceDataSetName("x");	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
									o2->SetPolygonSurfaceDataSetName("add "+QString::number(j));	
									//o2->GetVActor()->GetProperty()->SetColor(((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX),((rand())/(double)RAND_MAX));
									o2->SetGroupName("island final");
									emit	SignalSendNewSurface(o2);
									delete []testMask;
									}
	int ssize=0;
	double allCenter[3]={0};
	int p[3];
	for(i=0;i<newData->GetVolumeSize();i++)
	{

		if (frontMask[i]==1)
		{
			ssize++;		
			frontMask[i]=255;
			newData->GetVoxelXYZPos(i,p);
			allCenter[0]+=p[0];
			allCenter[1]+=p[1];
			allCenter[2]+=p[2];

		}

		
	}
	if (ssize>0)
	{
		allCenter[0]=(allCenter[0]/ssize)*scale[0]+origin[0];
		allCenter[1]=(allCenter[1]/ssize)*scale[1]+origin[1];
		allCenter[2]=(allCenter[2]/ssize)*scale[2]+origin[2];
		
	}

	//zalewanie
	//robie sobie liste powiedzmy max 50 wysp
	// opis - liczba sasiadow, center[3], bounds[6];
	// zalewam po kolei kazda i jesli liczba sasiadow < K to wywalam i nastepna
	// potem z kazdego sensownego center wystrzele kulka i dodam do segm glownej
	// potem closing 
	// i dotad az nie bedzie zadnej sensownej wyspy.

	double centerSet[50][3];
	int boundsSet[50][6];
	int neighCount[50]={0};
	int nrOfIsles=0;
	
	//********************************************
	//****************************************
	//*****************************************
	bool chg;
	//int isleNr=0;
				
				int  posX[3];
	double center[3];			
	start=0;			
	int count=0;


	double *centerTab=new double[ssize*3];


	for(i=start;i<newData->GetVolumeSize();i++)
	{
		if ((frontMask[i]==255))
		{
				
			//nrOfIsles++;
			start=i;
			newData->GetVoxelXYZPos(i,posX);
		//	int  SlotFillMask2(RawDataSet *Data1,unsigned char *Data2, int x,int y,int z,int endValue,int _KernelType,int *_bounds,unsigned char *mask2)
			count=SlotFillMask2(newData,frontMask,posX[0],posX[1],posX[2],nrOfIsles+1,0,bounds,NULL,center,centerTab);

			if (count>minNrOfIsleVoxels)
			{


				centerSet[nrOfIsles][0]=center[0];		centerSet[nrOfIsles][1]=center[1];	centerSet[nrOfIsles][2]=center[2];	
				for(j=0;j<6;j++)
				{
					boundsSet[nrOfIsles][j]=bounds[j];
				}
				neighCount[nrOfIsles]=count;
				nrOfIsles++;
			}
		}
		//identyfikacja wysp
			if (nrOfIsles==50)
			{
				break;
			}
	}
	delete []centerTab;



	delete []newMask;
	delete []newMask0;
	delete []frontMask;
	delete []frontMask2;
	delete newData;

	nrOfCenters=nrOfIsles;

	if (nrOfIsles==0)
		return NULL;

	double *centers=new double [3*nrOfIsles];
	int pp=0;
	for(i=0;i<nrOfCenters;i++)
	{
		centers[pp++]=centerSet[i][0];
		centers[pp++]=centerSet[i][1];
		centers[pp++]=centerSet[i][2];
	}
	return centers;
}
//----------------------------------------------------------------------------
void	ExplodeSegmentation::ShootAlongNormals(RawDataSet *data,int lth,int rth,vtkPoints *pts,vtkPoints *normals,double percent, double *center)
{
//	int rayLen=400;
//	double nPercent=0.2;
//	double definitiveBone=rth+100;//
//	int ss=2;
//	int i,j,k;
//
//	double xS=data->GetXscale();
//	double yS=data->GetYscale();
//	double zS=data->GetZscale();
//	double scale[3]={xS,yS,zS};
//
//	int nr=pts->GetNumberOfPoints();
//	double p[3],p1[3],d[3],n[3];
//	if (center==NULL)
//	{
//		//policz center
//		center=new double[3];
//		center[0]=0;	center[1]=0;	center[2]=0;
//
//		for(i=0;i<nr;i++)
//		{
//			pts->GetPoint(i,p);
//			center[0]+=p[0];	center[1]+=p[1];	center[2]+=p[2];
//
//		}
//		if( nr>0)
//		{
//			center[0]/=(double)nr;	center[1]/=(double)nr;	center[2]/=(double)nr;
//		}
//	}
//
//
//	vtkPoints *ptsX=vtkPoints::New();
//	ptsX->DeepCopy(pts);
//
//
//	double pStart[3];
//	double dx,dy,dz;
//	double dx1,dy1,dz1;
//	double length,length1;
//	//startVal
//	bool stop;
//	double fx,fy,fz;
//	double fx1,fy1,fz1;
//	int ix,iy,iz;
//	unsigned short val;
//	int pos;
//
//
//
//
//
//	int ksize;
//	//bede sie rozgladal po otoczeniu
//
//	int *step2=GetGrowingKernel(ksize,ss, data,1);
//	int	dsizemin1=data->GetVolumeSize()-1;
//	//tablica na problematyczne punkty
//	unsigned char *tTab=new unsigned char[nr];
//	unsigned short *dData=data->GetDataArray();
//	unsigned short *rayTab=new unsigned short[4096];
//	int rayPos;
//	int maxRayVal;
//	int maxRayPos;
//	int tmp;
//	memset(tTab,0,nr);
//
//	pStart[0]=center[0];
//	pStart[1]=center[1];
//	pStart[2]=center[2];
//
//	if (timer!=NULL) timer->TemporaryAppendToMainText("First Shooting");
//	int mmax=0;
//	int res=resolution-2;
//	int z=nr/(res);
//
//
//
//	bool *lessThanLeftTab=new bool[nr];
//	memset(lessThanLeftTab,0,nr*sizeof(bool));
//
//
//
//
//	normals->GetPoint(0,n);
//	dx=n[0];
//	dy=n[1];
//	dz=n[2];
//	length=sqrt(dx*dx+dy*dy+dz*dz);
//
//	tData2=new RawDataSet(res,z,1);
//	tData2->ZeroQ();
//	tData2->SetRawDataSetName("2D Hist-"+data->GetRawDataSetName());
//
//	tData=new RawDataSet(res,rayLen,z,1.0,length,1.0);
//	tData->ZeroQ();
//	tData->SetRawDataSetName("Mountains-"+data->GetRawDataSetName());
//
//	RawDataSet *tDataCheck=new RawDataSet(res,rayLen,z,1.0,length,1.0);
//	tDataCheck->ZeroQ();
//	tDataCheck->SetRawDataSetName("Mountains check");
//
//	for (i=0;i<nr;i++)
//	{
//		pts->GetPoint(i,p);
//		normals->GetPoint(i,n);
//
//		dx=n[0];
//		dy=n[1];
//		dz=n[2];
//		fx=p[0];
//		fy=p[1];
//		fz=p[2];
//
//
//		int testval;
//
//		stop=false;
//		rayPos=0;
//
//		do
//		{
//			ix=Round(fx/xS);
//			iy=Round(fy/yS);
//			iz=Round(fz/zS);
//
//
//
//
//			if ((ix<=ss)||(iy<=ss)||(iz<=ss)||(ix>=data->GetNx()-ss)||(iy>=data->GetNy()-ss)||(iz>=data->GetNz()-ss))//krance
//			{
//				stop=true;
//				//fx-=dx;	fy-=dy;	fz-=dz;
//				//kicha!!! - ustawie na -1 odleglosc
//				tTab[i]=1;
//
//			}
//			else
//			{
//
//				pos=data->GetVoxelPos(ix,iy,iz);
//
//				rayTab[rayPos++]=data->GetVoxelValue(ix,iy,iz);
//
//				val=0;
//
//				testval=0;
//				for (j=0;j<ksize;j++)
//				{	
//					tmp=pos+step2[j];
//					if ((tmp>dsizemin1)||(tmp<0))
//						break;
//					else
//					{
//						val=dData[tmp];
//						if ((val>=rth)||(val<=lth)) testval++;
//
//						if ((val<=lth)||((val>definitiveBone)))// kryterium definitywnego stopu
//						{
//							lessThanLeftTab[i]=true;
//							break;
//						}
//						//if (val>lth) testval++;
//					}
//
//					if (testval>=ksize*nPercent) break;
//				}
//
//				if ((testval>ksize*nPercent)||( lessThanLeftTab[i]==true))
//				{
//					//fx-=dx;	fy-=dy;	fz-=dz;
//					stop=true;
//				}
//				else
//				{
//					fx+=dx;	fy+=dy;	fz+=dz;
//				}
//
//			}
//
//		}while(stop==false);
//
//
//
//		int t1,t2,t3;
//		if (i>1)
//		{
//			t1=(i-2)%(res);
//			t3=(i-2)/(res);
//			for(j=0;j<rayPos;j++)
//			{
//
//				t2=j;
//
//				tData->SetVoxelValue(t1,t2,t3,rayTab[j]);
//				tDataCheck->SetVoxelValue(t1,t2,t3,rayTab[j]);
//			}
//
//			if (tTab[i]!=1) // uzupelnie do konca tDataCheck - potrzebne do medianfiltering stop criterion
//			{
//				stop=false;
//				fx1=fx;	 fy1=fy;	fz1=fz;
//				do
//				{
//					fx1+=dx;	fy1+=dy;	fz1+=dz;
//					ix=Round(fx1/xS);
//					iy=Round(fy1/yS);
//					iz=Round(fz1/zS);
//					if ((ix<=ss)||(iy<=ss)||(iz<=ss)||(ix>=data->GetNx()-ss)||(iy>=data->GetNy()-ss)||(iz>=data->GetNz()-ss))//krance
//					{
//						stop=true;
//					}
//					else
//					{
//
//						tDataCheck->SetVoxelValue(t1,j++,t3,data->GetVoxelValue(ix,iy,iz));
//
//					}
//				}while(stop==false);
//			}
//
//			tData2->SetVoxelValue(t1,t3,0,rayPos);
//		}
//
//
//		if (rayPos>mmax) mmax=rayPos;
//
//
//		pts->GetPoint(i,n);
//		p[0]=fx;
//		p[1]=fy;
//		p[2]=fz;
//		pts->SetPoint(i,p);
//	}
//
//
//
//
//
//	delete []step2;
//	delete []rayTab;
//
//
//	//--------------ddddddddddddddddddddddddddddddddddddd
//	if (timer!=NULL) timer->TemporaryAppendToMainText("Median Gradient Filtering");
//	RawDataSet *tDataB=new RawDataSet();
//	tDataB->Copy(*tData2,true);
//
//	RawDataSet *flagData=new RawDataSet();
//	flagData->Copy(*tData2,false);
//	flagData->ZeroQ();
//	unsigned short *flagTab=flagData->GetDataArray();
//
//
//
//	int mTh=10;
//	int grad;
//	int maxGrad;
//	int mx,my;
//	int mVal;
//	int tVal;
//	int xVal;
//	int chgs=0;
//	int step;
//	int x1,x2;
//	float testPrc=0.1;
//	int nrN;
//	//	int chgs=0;
//	do
//	{
//		chgs++;
//
//		maxGrad=0;
//		pos=2;
//		for(j=0;j<tDataB->GetNy();j++)
//		{
//			for(i=0;i<tDataB->GetNx();i++)
//			{
//				grad=GetGradValueFromDepthMap(tDataB,i,j);
//				//lessThanLeftTab
//
//				if (abs(grad)>abs(maxGrad))
//				{
//					////jesli punkt o duzym gradiencie jest w dziurze, i <leftth nie wyciagaj go
//					//if (((grad<0)&&(lessThanLeftTab[pos]==true)))
//					//{
//					//i=i;
//					//}
//					//else
//					if (flagTab[i+j*flagData->GetNx()]<4)
//					{
//						maxGrad=grad;
//						mx=i;
//						my=j;
//					}
//					//							mVal=tVal;
//				}
//				pos++;
//			}
//		}
//		if (abs(maxGrad)>mTh)
//		{
//			if ((mx==48)&&(my==67))
//			{
//				i=i;
//			}
//			xVal=tDataB->GetVoxelValue(mx,my,0);
//			//znalezc nowa na zmiane
//			//mediana na poczatek
//			mVal=GetMedValueFromDepthMap(tDataB,mx,my,2);
//
//			//if (!(flagTab[mx+my*flagData->GetNx()]>1)) //byles wiecej niz 1x? to zmieniaj nie sprawdzaj
//			{
//				//- sprawdze czy pomiedzy mediana a wartoscia pierwotna nie zdazyl sie punkt krytyczny jesli tak to na niego ustawiam wartosc!
//				//	step=(mVal-xVal>0)?1:-1;
//				if (mVal<xVal)	
//				{
//					x1=mVal;	x2=xVal;	step=1;
//				}
//				else
//				{
//					x1=xVal;	x2=mVal;	step=1;
//				}
//				for(i=x1;i!=x2;i+=step)
//				{
//					tVal=tDataCheck->GetVoxelValue(mx,i,my);
//					if ((tVal<=lth)||((tVal>definitiveBone)))// kryterium definitywnego stopu
//					{
//
//						nrN=GetNrOfNeighborsBetweenThreshInDepthMap(tDataB,3, (1-testPrc)*i, (1+testPrc)*i, mx,my);
//						if (nrN>=5) //jesli z otoczenia 7x7 s5 sasiadow bedzie tez definityw to zmiana
//						{
//							mVal=i;
//							break;
//						}
//					}
//
//				}
//			}
//
//
//			//--------------------------
//
//			tDataB->SetVoxelValue(mx,my,0,mVal);
//			//chgs++;
//			//flagData->SetVoxelValue(mx,my,0,1);
//			flagTab[mx+my*flagData->GetNx()]++;
//		}
//	}while((abs(maxGrad)>mTh));
//
//	if (timer!=NULL) timer->TemporaryAppendToMainText("Median Last Pass");
//	//wywalanie bardzo wielkich pikow co zostaly
//
//	for(j=0;j<tDataB->GetNy();j++)
//	{
//		for(i=0;i<tDataB->GetNx();i++)
//		{
//			xVal=tDataB->GetVoxelValue(i,j,0);
//			mVal=GetMedValueFromDepthMap(tDataB,i,j,5);
//
//			if (abs(xVal-mVal)>30) 
//			{
//				tDataB->SetVoxelValue(i,j,0,mVal);
//			}
//
//		}
//	}
//
//
//
//	//tData2->Copy(*tDataB,true);
//	//delete tDataB;
//	tData2A=tDataB;
//	tData2A->SetRawDataSetName("2D Hist-x-"+data->GetRawDataSetName());
//	flagTab=NULL;
//	delete flagData;
//
//	////policze se wartosc srednia
//	//int sr=0;
//	//pos=0;
//	//unsigned short *tTabX=tData->GetDataArray();
//	//for(i=0;i<tData->GetVolumeSize();i++)
//	//{
//	//		if (tTabX[i]>0)
//	//		{
//	//			pos++;
//	//			sr+=tTabX[i];
//	//		}
//	//}
//	//if (pos>0) sr/=pos;
//	//// policzona srednia
//
//	tDataA=new RawDataSet();
//	tDataA->Copy(*tData,true);
//	tDataA->SetRawDataSetName("Mountains-x-"+data->GetRawDataSetName());
//
//
//	for(j=0;j<tData->GetNz();j++)
//	{
//		for(i=0;i<tData->GetNx();i++)
//		{
//			tVal=tData2A->GetVoxelValue(i,j,0);
//			for(k=0;k<tVal;k++)
//			{
//				xVal=tData->GetVoxelValue(i,k,j);
//				if (xVal==0) tDataA->SetVoxelValue(i,k,j,tDataCheck->GetVoxelValue(i,k,j));
//			}
//			for(k=tVal;k<tData->GetNy();k++)
//			{
//				tDataA->SetVoxelValue(i,k,j,0);
//			}
//		}
//	}
//	//		tTabX=NULL;
//
//
//
//	//----------------------sssssssssssssssssssssssssssssssss
//	if (timer!=NULL) timer->TemporaryAppendToMainText("Improving Brain Shape");
//
//
//	unsigned short *newLengthTab=tData2A->GetDataArray();
//	for (i=2;i<nr;i++)
//	{
//		ptsX->GetPoint(i,p);
//		normals->GetPoint(i,n);
//		dx=n[0];
//		dy=n[1];
//		dz=n[2];
//		//length=sqrt(dx*dx+dy*dy+dz*dz);
//		p[0]+=dx*newLengthTab[i-2];
//		p[1]+=dy*newLengthTab[i-2];
//		p[2]+=dz*newLengthTab[i-2];
//
//		pts->SetPoint(i,p);
//	}
//	newLengthTab=NULL;
//
//
//	if (timer!=NULL) timer->TemporaryAppendToMainText("sth Else");
//
//	//--------------------------------------- brain shape:)
//
//	//sprawdze tTab - tablica z punktami zlymi
//	/*bool outOfBounds=false;
//	do
//	{*/
//
//	////float minDist=99999;
//	////int minPos=-1;
//
//	//////odrzuce outliers
//
//	////float *lengthTab=new float[nr];
//
//	////double ps[3];
//	////ps[0]=0;
//	////ps[1]=0;
//	////ps[2]=0;
//
//	////pos=0;
//	////for(i=0;i<nr;i++)
//	////{
//	////	if (tTab[i]!=1)
//	////	{
//	////		pts->GetPoint(i,p);
//	////		ps[0]+=p[0];
//	////		ps[1]+=p[1];
//	////		ps[2]+=p[2];
//	////		pos++;
//	////	}
//
//	////}
//
//	////ps[0]/=(float)pos;
//	////ps[1]/=(float)pos;
//	////ps[2]/=(float)pos;
//	////for(i=0;i<nr;i++)
//	////{
//	////	if (tTab[i]==1)
//	////	{
//	////		lengthTab[i]=-1;
//	////	}
//	////	else
//	////	{
//
//	////	
//
//	////		pts->GetPoint(i,p);
//	////		dx=p[0]-ps[0];
//	////		dy=p[1]-ps[1];
//	////		dz=p[2]-ps[2];
//	////		length=sqrt(dx*dx+dy*dy+dz*dz);
//
//	////		lengthTab[i]=length;
//	////	}
//	////}
//	////pos=0;
//	////float lenAvg=0;
//	////for(i=0;i<nr;i++)
//	////{
//
//	////
//
//	////	if (lengthTab[i]>0)
//	////	{
//	////		pos++;
//	////		lenAvg+=lengthTab[i];
//	////	}
//	////}
//	////if (pos>0) lenAvg/=(float)pos;
//
//
//	////for(i=0;i<nr;i++)
//	////{
//
//
//	////
//	////	if (lengthTab[i]<lenAvg*percent)
//	////		tTab[i]=1;
//	////	//wyrzucam te punkty ktorych odl od srodka jest mniejsza niz percent*avg
//	////}
//	////delete []lengthTab;
//
//	////if (timer!=NULL) timer->TemporaryAppendToMainText("First Shooting - bad points removal");
//	////
//	////for(i=0;i<nr;i++)
//	////{
//	////	
//
//	////	if (tTab[i]==1)
//	////	{
//	////		//znajde najblizszy i zastapie jego odlegloscia od srodka skopana odleglosc
//	////		ptsX->GetPoint(i,p);
//	////		minDist=99999;
//	////		minPos=-1;
//
//	////		for(j=0;j<nr;j++)
//	////		{
//	////			if((j!=i) && (tTab[j]==0))
//	////			{
//
//	////				ptsX->GetPoint(j,p1);
//	////				dx=p[0]-p1[0];
//	////				dy=p[1]-p1[1];
//	////				dz=p[2]-p1[2];
//	////				length=sqrt(dx*dx+dy*dy+dz*dz);
//
//
//	////				if (length<minDist)
//	////				{
//	////					minDist=length;
//	////					minPos=j;
//	////				}
//
//
//	////			}
//	////		}
//
//
//
//	////		if (minPos>0)
//	////		{
//	////			pts->GetPoint(i,p);
//	////			//zmiana odl na nowa
//	////			dx=p[0]-pStart[0];	dy=p[1]-pStart[1];	dz=p[2]-pStart[2];
//
//	////			length1=sqrt(dx*dx+dy*dy+dz*dz);
//	////			if (length>0)
//	////			{
//	////				dx/=length1;	dy/=length1;	dz/=length1;
//	////			}
//
//
//	////			pts->GetPoint(minPos,p1);
//	////			dx1=p1[0]-pStart[0];	dy1=p1[1]-pStart[1];	dz1=p1[2]-pStart[2];
//	////			length=sqrt(dx1*dx1+dy1*dy1+dz1*dz1);
//	////			dx*=length;		dy*=length;		dz*=length;
//	////			p[0]=pStart[0]+dx;		p[1]=pStart[1]+dy;		p[2]=pStart[2]+dz;
//	////			pts->SetPoint(i,p);
//
//	////		}
//
//	////	}
//
//	////}
//	delete tDataCheck; 
//	ptsX->Delete();
//	delete []tTab;
//	delete []lessThanLeftTab;


}
//-----------------------------------------------------------------------------
void	ExplodeSegmentation::MedianAgainstCenter(vtkPolyData *pData,int iter)
{

	vtkPoints *pts=pData->GetPoints();
	int nr=pts->GetNumberOfPoints();
	double p[3],p1[3],d[3];
	int i,j,k;
	double dx,dy,dz,length;




	vtkPoints *pts1=vtkPoints::New();
	pts1->DeepCopy(pts);

	vtkPoints *pts2=vtkPoints::New();
	pts2->DeepCopy(pts);

	int ptId;
	vtkIdType newCellPts[3];
	vtkIdType cellId;
	vtkIdList *pointList=vtkIdList::New();
	vtkIdList *cells=vtkIdList::New();
	vtkIdType np,np2;
	unsigned short ncells;
	double tab[500];

	int nrr;


	double length2;

	//	double ps[3];
	int nrrr;



	double ps[3];


	int l;
	for(l=0;l<iter;l++)
	{
		if (timer!=NULL) timer->TemporaryAppendToMainText("Median filtering ("+QString::number(l+1)+"/"+QString::number(iter)+")");
		for(i=0;i<nr;i++)
		{

			//compute sr;
			ps[0]=0;		ps[1]=0;		ps[2]=0;
			for(k=0;k<nr;k++)
			{
				pts1->GetPoint(k,p1);
				ps[0]+=p1[0];			ps[1]+=p1[1];			ps[2]+=p1[2];
			}
			ps[0]/=(double)nr;		ps[1]/=(double)nr;		ps[2]/=(double)nr;

			nrr=pData->GetNumberOfCells();
			nrr=0;
			pData->GetPointCells(i,cells);

			pts1->GetPoint(i,p);
			dx=p[0]-ps[0];		dy=p[1]-ps[1];		dz=p[2]-ps[2];
			length=sqrt(dx*dx+dy*dy+dz*dz);
			tab[nrr++]=length;


			nrrr=cells->GetNumberOfIds();

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
						if(np!=i)
						{
							pts1->GetPoint(np,p);
							dx=p[0]-ps[0];		dy=p[1]-ps[1];			dz=p[2]-ps[2];
							length=sqrt(dx*dx+dy*dy+dz*dz);
							tab[nrr++]=length;
						}
					}


				}
			}
			//nowa dlugosc to mediana z otoczenia
			length2=Median(tab,nrr-1);

			//wyliczam nowe wspolrzedne i podmieniam;
			pts1->GetPoint(i,p);
			dx=p[0]-ps[0];		dy=p[1]-ps[1];		dz=p[2]-ps[2];
			length=sqrt(dx*dx+dy*dy+dz*dz);
			if (length>1e-5)
			{
				dx/=length;			dy/=length;			dz/=length;
			}

			p1[0]=ps[0]+dx*length2;		p1[1]=ps[1]+dy*length2;		p1[2]=ps[2]+dz*length2;

			pts2->SetPoint(i,p1);
		}

		pts1->DeepCopy(pts2);

	}


	for (i=0;i<nr;i++)
	{
		pts1->GetPoint(i,p);
		pts->SetPoint(i,p);
	}



	pts1->Delete();
	pts2->Delete();




}
//--------------------------------------------------------------------------------
int GetGradValueFromDepthMap(RawDataSet *mapData,int x,int y)
{

	int i,j;
	int xp,yp;
	int size=1;
	int ssize=(size*2+1)*(size*2+1);
	int nx=mapData->GetNx();
	int ny=mapData->GetNy();

	int val1=mapData->GetVoxelValue(x,y,0);

	//x+=nx;
	y+=ny;



	int diff=0;
	int mDiff=0;
	int val2;


	for(j=y-size;j<=y+size;j++)
	{
		for(i=x-size;i<=x+size;i++)
		{	
			yp=j%ny;

			if (i<0) 
				xp=0;
			else 
				if (i>nx-1) 
					xp=nx-1;
				else
					xp=i;

			val2=mapData->GetVoxelValue(xp,yp,0);

			diff=(val1-val2);
			if (abs(diff)>abs(mDiff))
			{
				mDiff=diff;

			}

		}
	}

	return mDiff;

}
//----------------------------
int GetNrOfNeighborsBetweenThreshInDepthMap(RawDataSet *mapData,int size,int lth,int rth, int x,int y)
{

	int i,j;
	int xp,yp;

	int ssize=(size*2+1)*(size*2+1);
	int nx=mapData->GetNx();
	int ny=mapData->GetNy();
	int neigh=0;

	//int val1=mapData->GetVoxelValue(x,y,0);

	//x+=nx;
	y+=ny;



	int diff=0;
	int mDiff=0;
	int val2;


	for(j=y-size;j<=y+size;j++)
	{
		for(i=x-size;i<=x+size;i++)
		{	
			if ((i!=x)&&(j!=y))
			{
				yp=j%ny;

				if (i<0) 
					xp=0;
				else 
					if (i>nx-1) 
						xp=nx-1;
					else
						xp=i;

				val2=mapData->GetVoxelValue(xp,yp,0);

				//if ((val2<=lth)||((tVal>rt))
				if (((val2>=lth)&&(val2<=rth)))
					neigh++;
				/*diff=(val1-val2);
				if (diff!=0)
				{
				mDiff=diff;

				}*/
			}

		}
	}

	return neigh;

}
//------------
int GetMedValueFromDepthMap(RawDataSet *mapData,int x,int y,int size)
{

	int i,j;
	int xp,yp;
	//	int size=2;
	int ssize=(size*2+1)*(size*2+1);
	int nx=mapData->GetNx();
	int ny=mapData->GetNy();

	int val1=mapData->GetVoxelValue(x,y,0);

	//	x+=nx;
	y+=ny;



	int diff=0;
	int mDiff=0;
	//	int min=0;
	int val2;

	unsigned short *tab=new unsigned short[ssize];
	memset(tab,0,ssize*sizeof(unsigned short));

	int pos=0;
	for(j=y-size;j<=y+size;j++)
	{
		for(i=x-size;i<=x+size;i++)
		{	
			//xp=i%nx;
			yp=j%ny;
			if (i<0) 
				xp=0;
			else 
				if (i>nx-1) 
					xp=nx-1;
				else
					xp=i;
			val2=mapData->GetVoxelValue(xp,yp,0);

			tab[pos++]=val2;

		}
	}


	int val=Median(tab,ssize);
	return val;

}
//------------------------------

void	ExplodeSegmentation::FindFirstGuessCenterAlongNormals(RawDataSet *data,int lth,int rth,vtkPoints *pts,vtkPoints *normals,double percent, double *center)
{
	double origin[3];
	data->GetOrigin(origin);
	double nPercent=0.2;
	double definitiveBone=rth+200;//
	int ss=2;
	int i,j,k;

	double xS=data->GetXscale();
	double yS=data->GetYscale();
	double zS=data->GetZscale();
	double scale[3]={xS,yS,zS};

	int nr=pts->GetNumberOfPoints();
	double p[3],p1[3],d[3],n[3];
	if (center==NULL)
	{
		//policz center
		center=new double[3];
		center[0]=0;	center[1]=0;	center[2]=0;

		for(i=0;i<nr;i++)
		{
			pts->GetPoint(i,p);
			center[0]+=p[0];	center[1]+=p[1];	center[2]+=p[2];

		}
		if( nr>0)
		{
			center[0]/=(double)nr;	center[1]/=(double)nr;	center[2]/=(double)nr;
		}
	}


	vtkPoints *ptsX=vtkPoints::New();
	ptsX->DeepCopy(pts);


	double pStart[3];
	double dx,dy,dz;
	double dx1,dy1,dz1;
	double length,length1;
	//startVal
	bool stop;
	double fx,fy,fz;
	int ix,iy,iz;
	unsigned short val;
	int pos;





	int ksize;
	//bede sie rozgladal po otoczeniu

	int *step2=GetGrowingKernel(ksize,ss, data,1);
	int	dsizemin1=data->GetVolumeSize()-1;
	//tablica na problematyczne punkty

	unsigned short *dData=data->GetDataArray();

	int rayPos;
	int maxRayVal;
	int maxRayPos;
	int tmp;


	pStart[0]=center[0];
	pStart[1]=center[1];
	pStart[2]=center[2];

	if (timer!=NULL) timer->TemporaryAppendToMainText("Center Estimation");
	int mmax=0;
	int res=resolution-2;
	int z=nr/(res);

	bool *lessThanLeftTab=new bool[nr];
	memset(lessThanLeftTab,0,nr*sizeof(bool));
	for (i=0;i<nr;i++)
	{
		pts->GetPoint(i,p);
		normals->GetPoint(i,n);


		dx=n[0];
		dy=n[1];
		dz=n[2];
		fx=p[0]-origin[0];
		fy=p[1]-origin[1];
		fz=p[2]-origin[2];
		length=sqrt(dx*dx+dy*dy+dz*dz);

		int testval;

		stop=false;
		rayPos=0;

		do
		{
			ix=Round(fx/xS);
			iy=Round(fy/yS);
			iz=Round(fz/zS);




			if ((ix<=ss)||(iy<=ss)||(iz<=ss)||(ix>=data->GetNx()-ss)||(iy>=data->GetNy()-ss)||(iz>=data->GetNz()-ss))//krance
			{
				stop=true;
				//fx-=dx;	fy-=dy;	fz-=dz;
				/*fx-=dx;
				fy-=dy;
				fz-=dz;*/ // cofalem o 1 krok- narazie skasowalem bo wyglada lepiej

				//kicha!!! - ustawie na -1 odleglosc
				//tTab[i]=1;

			}
			else
			{

				pos=data->GetVoxelPos(ix,iy,iz);



				val=0;

				testval=0;
				for (j=0;j<ksize;j++)
				{	
					tmp=pos+step2[j];
					if ((tmp>dsizemin1)||(tmp<0))
						break;
					else
					{
						val=dData[tmp];
						if ((val>=rth)||(val<=lth)) testval++;

						if ((val<=lth)||((val>definitiveBone)))
						{
							lessThanLeftTab[i]=true;
							break;
						}
						//if (val>lth) testval++;
					}

					if (testval>=ksize*nPercent) break;
				}

				if ((testval>ksize*nPercent)||( lessThanLeftTab[i]==true))
				{
					//fx-=dx;	fy-=dy;	fz-=dz;
					stop=true;
				}
				else
				{
					fx+=dx;	fy+=dy;	fz+=dz;
				}

			}

		}while(stop==false);



		pts->GetPoint(i,n);
		p[0]=fx+origin[0];
		p[1]=fy+origin[1];
		p[2]=fz+origin[2];
		ptsX->SetPoint(i,p);
	}

	double oldCenter[3]={center[0],center[1],center[2]};
	center[0]=0;
	center[1]=0;
	center[2]=0;

	for(i=0;i<nr;i++)
	{
		ptsX->GetPoint(i,p);
		center[0]+=p[0];
		center[1]+=p[1];
		center[2]+=p[2];
	}

	center[0]/=nr;
	center[1]/=nr;
	center[2]/=nr;

	oldCenter[0]-=center[0];
	oldCenter[1]-=center[1];
	oldCenter[2]-=center[2];


	//przesuwam punkty do nowego srodka
	for(i=0;i<nr;i++)
	{
		pts->GetPoint(i,p);
		p[0]-=oldCenter[0];
		p[1]-=oldCenter[1];
		p[2]-=oldCenter[2];
		pts->SetPoint(i,p);
		
	}

	delete []lessThanLeftTab;
	ptsX->Delete();


}

//-----------------------------
//---------------------------
//--------------------------



//----------------------------------------------------------------------------
vtkPolyData * ExplodeSegmentation::IncreaseRayMapResolution(RawDataSet *data, int newRes,double *center, vtkPolyData *polys, double minsize, RawDataSet *rayMapData,RawDataSet *rayMapStatusData,SegCriterion *cr,RayMapFilter *rf)
{
	//wygladze mesh


		double p[3],p1[3],d[3],n[3];
		double dx,dy,dz;
		double length;

	vtkSphereSource *sphere=vtkSphereSource::New();
	sphere->SetPhiResolution(newRes);
	sphere->SetThetaResolution(newRes);
	sphere->SetCenter(center);
	sphere->SetRadius(minsize);
	

	vtkPolyData *spherePolys=sphere->GetOutput();
//	spherePolys->Update();
	spherePolys->BuildCells();
	spherePolys->BuildLinks();



	vtkPoints *pts=spherePolys->GetPoints();
	int nr=pts->GetNumberOfPoints();
	int res=newRes-2;
	int z=nr/(res);

	RawDataSet *newRayMapData=new RawDataSet(res,z,1);
	newRayMapData->ZeroQ();
	newRayMapData->SetRawDataSetName("2D Hist-new"+data->GetRawDataSetName());

	RawDataSet *newRayMapStatusData=new RawDataSet(res,z,1);
	newRayMapStatusData->ZeroQ();
	newRayMapStatusData->SetRawDataSetName("status-new"+data->GetRawDataSetName());



	int i,j,k;

	double scaleF[3];
	int nOld[3],nNew[3];
	rayMapData->GetN(nOld);
	newRayMapData->GetN(nNew);

	for(i=0;i<2;i++) scaleF[i]=(double)(nOld[i]-1)/(nNew[i]-1);


	//inteproluje liniowo ze starej mapy na nowa
	double val;
	for(j=0;j<newRayMapData->GetNy();j++)
	{
		for(i=0;i<newRayMapData->GetNx();i++)
		{
			dx=i*scaleF[0];
			dy=j*scaleF[1];
			if (	(dx<rayMapData->GetNx())	&&	(dy<rayMapData->GetNy())	)
			{	val=Interpolation3Linear(rayMapData,dx,dy,0);
				newRayMapData->SetVoxelValue(i,j,0,val);

				val=Interpolation3Linear(rayMapStatusData,dx,dy,0);
				newRayMapStatusData->SetVoxelValue(i,j,0,val);


			}
		}
	}


	vtkDataArray *normalsT=polys->GetPointData()->GetNormals();
	vtkPoints *nrls=vtkPoints::New();
	//licze nowe normalne

	for(i=0;i<nr;i++)
	{
		pts->GetPoint(i,p);
		n[0]=p[0]-center[0];	n[1]=p[1]-center[1];	n[2]=p[2]-center[2];

		nrls->InsertNextPoint(n);

	}

	/*for(i=0;i<nr;i++)
	{
		normalsT->GetTuple(i,n);

		dx=n[0];
		dy=n[1];
		dz=n[2];
		length=sqrt(dx*dx+dy*dy+dz*dz);
		if (length>0)
		{
			n[0]=n[0]*minsize/length;
			n[1]=n[1]*minsize/length;
			n[2]=n[2]*minsize/length;
		}

		nrls->InsertNextPoint(n);
	}*/


	unsigned short *newLengthTab=newRayMapData->GetDataArray();
	int len;
	

	//vtkPoints *ptsX=vtkPoints::New();
	for (i=0;i<nr;i++)
	{
		pts->GetPoint(i,p);
		nrls->GetPoint(i,n);
		dx=n[0];
		dy=n[1];
		dz=n[2];
		if (i>1) len=newLengthTab[i-2];
		else if (i==0)
		{
			len=newLengthTab[0];
		}
		else 
		{
			len=newLengthTab[newRes-2-1];
		}
		len-=1;
		//length=sqrt(dx*dx+dy*dy+dz*dz);
		
		p[0]+=dx*len;
		p[1]+=dy*len;
		p[2]+=dz*len;
		

		pts->SetPoint(i,p);
	}
	/*pts->DeepCopy(ptsX);
	ptsX->Delete();
	spherePolys
	*/
	////liniowo interpoluje nowy - jesli jakis puntk ma slaby status to uwazniej mu sie przygladam
	rayMapData->Copy(*newRayMapData,true);
	rayMapStatusData->Copy(*newRayMapStatusData,true);

	delete newRayMapData;
	delete newRayMapStatusData;

	vtkPolyData *pD=vtkPolyData::New();
	pD->DeepCopy(spherePolys);
	return pD;

}
//-------------------------------
void	ExplodeSegmentation::UpdateTopPoints(vtkPolyData *pData)
{
	// biore punkty dla p 0 i 1 i licz

	int nr=pData->GetNumberOfPoints();
	

	if (nr>1)
	{
		vtkPoints *pts=pData->GetPoints();	
		int i,j,k;
		vtkIdType newCellPts[3];
		vtkIdType cellId;
		vtkIdList *pointList=vtkIdList::New();
		vtkIdList *cells=vtkIdList::New();
		vtkIdType np,np2;
		unsigned short ncells;
		int nrrr;
		double p[3],p1[3];
		int neigh;

		for(i=0;i<2;i++)
		{
			pData->GetPointCells(i,cells);
			nrrr=cells->GetNumberOfIds();
			p[0]=0;p[1]=0;p[2]=0;
			neigh=0;
			for(j=0;j<nrrr;j++)
			{
				cellId=cells->GetId(j);
				np2=pData->GetCellType(cellId);
				pData->GetCellPoints(cellId,pointList);
				
				for(k=0;k<pointList->GetNumberOfIds();k++)
				{
					np=pointList->GetId(k);
					if(np!=i)
					{
						neigh++;
						pts->GetPoint(np,p1);
						p[0]+=p1[0]; p[1]+=p1[1]; p[2]+=p1[2];
					}
				}
			}

			if (neigh>0)
			{
				p[0]/=neigh; p[1]/=neigh;p[2]/=neigh;
				pts->SetPoint(i,p);
			}


		}

	}


}
void ExplodeSegmentation::RayShoot(RawDataSet *data, double *center, vtkPolyData *polys, double minsize,double maxRay, double *shootArea, RawDataSet *rayMapData,RawDataSet *rayMapStatusData,bool getNewRayStatusMap, int resolution,SegCriterion *cr,RayMapFilter *rf)
//void	ExplodeSegmentation::ShootAlongNormals(RawDataSet *data,int lth,int rth,vtkPoints *pts,vtkPoints *normals,double percent, double *center)
{
	
	
	int i,j,k;
	int ss=cr->neighSize;
	double p[3],p1[3],d[3],n[3];

	double pStart[3];
	double dx,dy,dz;
	double dx1,dy1,dz1;
	double length,length1;
	//startVal
	bool stop;
	double fx,fy,fz;
	double fx1,fy1,fz1;
	int ix,iy,iz;
	unsigned short val;
	int pos;


	vtkPoints *pts=polys->GetPoints();


	
	int nr=pts->GetNumberOfPoints();
	//vtkDataArray *normalsT=polys->GetPointData()->GetNormals();
	vtkPoints *nrls=vtkPoints::New();
	
//normalne dla aktualnego meshu generuje z vtk - moge olac center


	vtkPolyDataNormals *normals=vtkPolyDataNormals::New();
	normals->SplittingOff ();
	normals->ComputePointNormalsOn ();
	normals->NonManifoldTraversalOn  () ;
	normals->ComputeCellNormalsOff ();
	normals->SetInputData(polys);
	normals->Update();


	vtkPolyData *tempData=vtkPolyData::New();
	tempData->DeepCopy(normals->GetOutput());
//	tem//pData->Update();
	vtkDataArray *normalsT=tempData->GetPointData()->GetNormals();


	for(i=0;i<nr;i++)
	{
		normalsT->GetTuple(i,n);

		dx=n[0];
		dy=n[1];
		dz=n[2];
		length=sqrt(dx*dx+dy*dy+dz*dz);
		if (length>0)
		{
			n[0]=n[0]*minsize/length;
			n[1]=n[1]*minsize/length;
			n[2]=n[2]*minsize/length;
		}

		nrls->InsertNextPoint(n);
	}


	normals->Delete();
	tempData->Delete();
//	normalsT->Delete();

//normalne wygenerowaen

	int rayLen=512;
	

	double xS=data->GetXscale();
	double yS=data->GetYscale();
	double zS=data->GetZscale();
	double scale[3]={xS,yS,zS};

//	int nr=pts->GetNumberOfPoints();

	if (center==NULL)
	{
		//policz center
		center=new double[3];
		center[0]=0;	center[1]=0;	center[2]=0;

		for(i=0;i<nr;i++)
		{
			pts->GetPoint(i,p);
			center[0]+=p[0];	center[1]+=p[1];	center[2]+=p[2];

		}
		if( nr>0)
		{
			center[0]/=(double)nr;	center[1]/=(double)nr;	center[2]/=(double)nr;
		}
	}


	vtkPoints *ptsX=vtkPoints::New();
	ptsX->DeepCopy(pts);






	int ksize;
	//bede sie rozgladal po otoczeniu

//	int *step2=GetGrowingKernel(ksize,ss, data,1);
//	int	dsizemin1=data->GetVolumeSize()-1;
	//tablica na problematyczne punkty
	unsigned char *tTab=new unsigned char[nr];
	unsigned short *dData=data->GetDataArray();
	unsigned short *rayTab=new unsigned short[4096];
	int rayPos;
	int maxRayVal;
	int maxRayPos;
	int tmp;
	memset(tTab,0,nr);

	pStart[0]=center[0];
	pStart[1]=center[1];
	pStart[2]=center[2];

	if (timer!=NULL) timer->TemporaryAppendToMainText("First Shooting");
	int mmax=0;
	int res=resolution-2;
	int z=nr/(res);


 
	nrls->GetPoint(0,n);
	dx=n[0];
	dy=n[1];
	dz=n[2];
	length=sqrt(dx*dx+dy*dy+dz*dz);

	RawDataSet *tData2=new RawDataSet(res,z,1);
	tData2->ZeroQ();
	tData2->SetRawDataSetName("2D Hist-"+data->GetRawDataSetName());

	
	RawDataSet *tData=new RawDataSet(res,rayLen,z,1.0,length,1.0);
	tData->ZeroQ();
	tData->SetRawDataSetName("Mountains-"+data->GetRawDataSetName());


	RawDataSet *tDataCheck=new RawDataSet(res,rayLen,z,1.0,length,1.0);
	tDataCheck->ZeroQ();
	tDataCheck->SetRawDataSetName("Mountains check");

	bool dontChange;

	int sArea[4];

	if (shootArea!=NULL)
	{
		sArea[0]=shootArea[0]*(res-1);//x1
		sArea[1]=shootArea[1]*(z-1);//y1

		sArea[2]=shootArea[2]*(res-1);//x2
		sArea[3]=shootArea[3]*(z-1);//y2



	}
	double ddx,ddy,ddz;

	for (i=0;i<nr;i++)
	{
		dontChange=false;
		if ((shootArea!=NULL)||(getNewRayStatusMap==false))
		{
				
			int xmap,ymap;	
			if (i>1)
			{
				xmap=(i-2)%(res);
				ymap=(i-2)/(res);
			}
			//szczegolne punkty na gorze i dole kuli nie uwzglednione w raymap
			else if (i==1)
			{
				xmap=0;ymap=0;
			}
			else
			{
				xmap=res-1; ymap=0;
			}
			if (shootArea!=NULL)
			if (!((xmap>=sArea[0]) &&(xmap<=sArea[2]) &&(ymap>=sArea[1]) &&(ymap<=sArea[3])))
				dontChange=true; //przerywam jesli nie znajduje sie w kwadrace (0,2) (1,3)
			//if (getNewRayStatusMap==false)
			//{
			//	if (rayMapStatusData->GetVoxelValue(xmap,ymap,0)>255)
			//	{
			//		i=i;
			//	}
			//	int nrN=GetNrOfNeighborsBetweenThreshInDepthMap(rayMapStatusData,2, 256, 512, xmap,ymap);
			//	if (nrN>0)
			//		dontChange=true;
			//	


			////	if (rayMapStatusData->GetVoxelValue(xmap,ymap,0)>255) //nie pozwalam na zmiany tych punktow ktore trzeba bylo scinac wstecz - np. wyszly przez oczy
			//		
			//}


		}


		pts->GetPoint(i,p);
		nrls->GetPoint(i,n);

		dx=n[0];
		dy=n[1];
		dz=n[2];
		fx=p[0];
		fy=p[1];
		fz=p[2];


		int testval;

		stop=false;
		rayPos=0;


	

		do
		{
		

			ix=Round(fx/xS);
			iy=Round(fy/yS);
			iz=Round(fz/zS);

			ddx=fx/xS;
			ddy=fy/yS;
			ddz=fz/zS;



			if ((ix<=ss)||(iy<=ss)||(iz<=ss)||(ix>=data->GetNx()-ss)||(iy>=data->GetNy()-ss)||(iz>=data->GetNz()-ss))//krance
			{
				stop=true;
				//fx-=dx;	fy-=dy;	fz-=dz;
				//kicha!!! - ustawie na -1 odleglosc
				tTab[i]=1;
			}
			else
			{

			//	pos=data->GetVoxelPos(ix,iy,iz);

				//rayTab[rayPos++]=data->GetVoxelValue(ix,iy,iz);
				rayTab[rayPos++]=Interpolation3Linear(data,ddx,ddy,ddz);
				if (cr->CheckPoint(ix,iy,iz,rayTab,rayPos)>0) 
				{	stop=true;}
				else
				{
					fx+=dx;	fy+=dy;	fz+=dz;
					if (maxRay>0)
					{
						double rr[3];
						rr[0]=fx-p[0];
						rr[1]=fy-p[1];
						rr[2]=fz-p[2];
						length=sqrt(rr[0]*rr[0]+rr[1]*rr[1]+rr[2]*rr[2]);
						if (length>maxRay)
						{
							dontChange=true;
							stop=true;
						}
					}

				}
			}
	

		}while(stop==false);


		if (dontChange==false)
		{
			int t1,t2,t3;
			if (i>1)
			{
				t1=(i-2)%(res);
				t3=(i-2)/(res);
				for(j=0;j<rayPos;j++)
				{

					t2=j;

					tData->SetVoxelValue(t1,t2,t3,rayTab[j]);
					tDataCheck->SetVoxelValue(t1,t2,t3,rayTab[j]);
				}

				if (tTab[i]!=1) // uzupelnie do konca tDataCheck - potrzebne do medianfiltering stop criterion
				{
					stop=false;
					fx1=fx;	 fy1=fy;	fz1=fz;
					do
					{
						fx1+=dx;	fy1+=dy;	fz1+=dz;
						ix=Round(fx1/xS);
						iy=Round(fy1/yS);
						iz=Round(fz1/zS);
						if ((ix<=ss)||(iy<=ss)||(iz<=ss)||(ix>=data->GetNx()-ss)||(iy>=data->GetNy()-ss)||(iz>=data->GetNz()-ss))//krance
						{
							stop=true;
						}
						else
						{

							tDataCheck->SetVoxelValue(t1,j++,t3,data->GetVoxelValue(ix,iy,iz));

						}
					}while(stop==false);
				}

				tData2->SetVoxelValue(t1,t3,0,rayPos);
			}


			if (rayPos>mmax) mmax=rayPos;


			pts->GetPoint(i,n);
			p[0]=fx;
			p[1]=fy;
			p[2]=fz;
			pts->SetPoint(i,p);
		}
	}





//	delete []step2;
	delete []rayTab;


	//--------------ddddddddddddddddddddddddddddddddddddd
	if(rf!=NULL)
	{
		if (timer!=NULL) timer->TemporaryAppendToMainText("Median Gradient Filtering");
		if 	(getNewRayStatusMap==true)
		{
			rayMapStatusData->Copy(*tData2,false);
			rayMapStatusData->SetRawDataSetName("test");
			rayMapStatusData->ZeroQ();
			rf->ApplyFiltering(tDataCheck,tData2,rayMapStatusData);
		}
		else
		{
			rf->ApplyFiltering(tDataCheck,tData2,NULL);
		}
	}

	int tVal;
	for(j=0;j<tData->GetNz();j++)
	{
		for(i=0;i<tData->GetNx();i++)
		{
			tVal=tData2->GetVoxelValue(i,j,0);
			/*if (rayMapStatusData->GetVoxelValue(i,j,0)>0) tVal+=30;
			tData2->SetVoxelValue(i,j,0,tVal);*/
			for(k=0;k<tVal;k++)
			{
				//xVal=tData->GetVoxelValue(i,k,j);
				//if (xVal==0)
					tData->SetVoxelValue(i,k,j,tDataCheck->GetVoxelValue(i,k,j));
			}
			for(k=tVal;k<tData->GetNy();k++)
			{
				tData->SetVoxelValue(i,k,j,0);
			}
		}
	}
	//		tTabX=NULL;

 

	//----------------------sssssssssssssssssssssssssssssssss
	if (timer!=NULL) timer->TemporaryAppendToMainText("Improving Brain Shape");


	unsigned short *newLengthTab=tData2->GetDataArray();
	for (i=2;i<nr;i++)
	{
		ptsX->GetPoint(i,p);
		nrls->GetPoint(i,n);
		dx=n[0];
		dy=n[1];
		dz=n[2];
		if (newLengthTab[i-2]>10)
		{
		i=i;
		}
		//length=sqrt(dx*dx+dy*dy+dz*dz);
		p[0]+=dx*(newLengthTab[i-2]-1);
		p[1]+=dy*(newLengthTab[i-2]-1);
		p[2]+=dz*(newLengthTab[i-2]-1);

		pts->SetPoint(i,p);
	}
	
	//usredniam pozycje z otoczen pktow 0 i 1
	UpdateTopPoints(polys);

	//if (testData!=NULL) UpdateRayPolyData(testData,center,tData2,nrls);

	if (timer!=NULL) timer->TemporaryAppendToMainText("sth Else");


	delete tDataCheck; 
	ptsX->Delete();
	delete []tTab;

	rayMapData->Copy(*tData2,true);

	//delete tData,
	//delete tData2;
	emit SignalSendNewRawDataSet(tData);
	emit SignalSendNewRawDataSet(tData2);

	//	RawDataSet *tDataB=new RawDataSet();
//	tDataB->Copy(*tData2,true);


}
//-----------------------------------------------------------------------------



//--------------------------------
void	UpdateRayPolyData(vtkPolyData *pData, double *center,RawDataSet *mapData, vtkPoints *nrls)
{

//	unsigned short *newLengthTab=tData2->GetDataArray();
//for (i=2;i<nr;i++)
//{
//	ptsX->GetPoint(i,p);
//	nrls->GetPoint(i,n);
//	dx=n[0];
//	dy=n[1];
//	dz=n[2];
//	if (newLengthTab[i-2]>10)
//	{
//	i=i;
//	}
//	//length=sqrt(dx*dx+dy*dy+dz*dz);
//	p[0]+=dx*(newLengthTab[i-2]-1);
//	p[1]+=dy*(newLengthTab[i-2]-1);
//	p[2]+=dz*(newLengthTab[i-2]-1);

//	pts->SetPoint(i,p);
//}
//


	// 1. sprawdzam czy pData jest puste
	// jesli nie bede dodawal linie od srodka, jesli nie od poprzednich punktow.

	int nr0=pData->GetNumberOfPoints();
	bool centerStart=(nr0>0)?false:true;

	if (centerStart==true)
	{
		//tworze skalary i dodaje do polydata

		vtkPoints	*pts=vtkPoints::New(); //pkty

		pts->InsertNextPoint(center);
		vtkCellArray *lines = vtkCellArray::New(); // krechy
		vtkFloatArray *scalars = vtkFloatArray::New(); // przypisuje skalar do danego el luta


		vtkLookupTable *lut = vtkLookupTable::New(); // lut raz a dobrze
		lut->SetAlphaRange(0,3);
		lut->SetNumberOfColors(3);
		//lut->SetTableRange(0, 3);
		lut->SetTableValue(0,0.5,0.5,0); //r
		lut->SetTableValue(1,0,0,0); //g
		lut->SetTableValue(2,0,0.5,0.5); //b
		lut->Build();
		scalars->SetLookupTable(lut);
		pData->GetPointData()->SetScalars(scalars);
		 		 
		pData->SetPoints(pts);
		pData->SetLines(lines);
	}

	vtkPoints		*pts	=	pData->GetPoints();
	vtkCellArray	*lines	=	pData->GetLines();
	vtkFloatArray	*scalars=	(vtkFloatArray*)pData->GetPointData()->GetScalars();

	int nr=nrls->GetNumberOfPoints()-2;
	int totalPts=pData->GetNumberOfPoints();

	int i;

	double p[3],n[3],d[3];
	
	unsigned short *ltab=mapData->GetDataArray();
	//vtkCellArray *elems = vtkCellArray::New();
	vtkIdType newCellPts[2];
	vtkIdType ID;

	int size=MIN(mapData->GetVolumeSize(),nr);//uwaga gdy ltab jest kroetsze od nr

	//dodaje nowe punkty i linie
	for(i=0;i<size;i++) 
	{
		if (centerStart==true)
		{
			p[0]=center[0];p[1]=center[1];p[2]=center[2];
			newCellPts[0]=0;

		}
		else
		{	
			pts->GetPoint(totalPts-nr+i,p);
		}
		
		nrls->GetPoint(i+2,n);

		
	
		//length=sqrt(dx*dx+dy*dy+dz*dz);
		p[0]+=n[0]*(ltab[i]-1);
		p[1]+=n[1]*(ltab[i]-1);
		p[2]+=n[2]*(ltab[i]-1);
		pts->InsertNextPoint(p);

		
		if (centerStart==false) newCellPts[0]=totalPts-nr+i;
		newCellPts[1]=totalPts+i;
		lines->InsertNextCell(2,newCellPts);

		//jeszcze tylko kolory
		scalars->InsertNextValue(((int)((totalPts+i-1)/size))%3);

		//pts->SetPoint(i,p);
	}


	pData->BuildCells();
//	//pData->Update();

}

//-------------------------------------
void	UpdateRayPolyData(vtkPolyData *pData, double *center,vtkPolyData *sData)
{


	// 1. sprawdzam czy pData jest puste
	// jesli nie bede dodawal linie od srodka, jesli nie od poprzednich punktow.

	int nrOfCol=3;
	int nr0=pData->GetNumberOfPoints();
	bool centerStart=(nr0>0)?false:true;

	if (centerStart==true)
	{
		//tworze skalary i dodaje do polydata

		vtkPoints	*pts=vtkPoints::New(); //pkty

		pts->InsertNextPoint(center);
		vtkCellArray *lines = vtkCellArray::New(); // krechy
		vtkFloatArray *scalars = vtkFloatArray::New(); // przypisuje skalar do danego el luta


		vtkLookupTable *lut = vtkLookupTable::New(); // lut raz a dobrze
		lut->SetAlphaRange(0,3);
		lut->SetNumberOfColors(nrOfCol);
		//lut->SetTableRange(0, 3);
		lut->SetTableValue(0,0,1,0); //
		lut->SetTableValue(1,0,0,1); //
		lut->SetTableValue(2,1,0,0); //
		lut->Build();
		scalars->SetLookupTable(lut);
		pData->GetPointData()->SetScalars(scalars);
		 		 
		pData->SetPoints(pts);
		pData->SetLines(lines);
	}

	vtkPoints		*pts	=	pData->GetPoints();
	vtkCellArray	*lines	=	pData->GetLines();
	vtkFloatArray	*scalars=	(vtkFloatArray*)pData->GetPointData()->GetScalars();

	int nr=sData->GetNumberOfPoints()-2;
	int totalPts=pData->GetNumberOfPoints();

	int i;

	double p[3],n[3],d[3];
	
	
	//vtkCellArray *elems = vtkCellArray::New();
	vtkIdType newCellPts[2];
	vtkIdType ID;

	int size=nr;
	vtkPoints *pts2=sData->GetPoints();

	//dodaje nowe punkty i linie
	for(i=0;i<size;i++) 
	{
		if (centerStart==true)
		{
			p[0]=center[0];p[1]=center[1];p[2]=center[2];
			newCellPts[0]=0;

		}
		else
		{	
			pts->GetPoint(totalPts-nr+i,p);
		}
		
		pts2->GetPoint(i+2,n);

		
	
		//length=sqrt(dx*dx+dy*dy+dz*dz);
		/*p[0]+=n[0]*(ltab[i]-1);
		p[1]+=n[1]*(ltab[i]-1);
		p[2]+=n[2]*(ltab[i]-1);*/
		pts->InsertNextPoint(n);

		
		if (centerStart==false) newCellPts[0]=totalPts-nr+i;
		newCellPts[1]=totalPts+i;
		lines->InsertNextCell(2,newCellPts);

		//jeszcze tylko kolory
		scalars->InsertNextValue(((int)((totalPts+i-1)/size))%nrOfCol);

		//pts->SetPoint(i,p);
	}


	pData->BuildCells();
//	//pData->Update();

}//-------------------------

void ExplodeSegmentation::RayShootSimple(RawDataSet *data, unsigned char *mask, vtkPolyData *polys, double minsize, SegCriterion *cr, int nrPtsOld, double maxDist, int *center, RawDataSet *shootCentralMaskData, mySphereDistTabsElement *neighTab , double *exportParams)
{

	double origin[3];
	data->GetOrigin(origin);
	double p[3], p1[3], d[3], n[3];
	double nP[3];
	int iP[3];
	int iPPrev[3] = { -1, -1, -1 };
	double pStart[3];
	double dx, dy, dz;
	double dx1, dy1, dz1;
	double length, length1;
	//startVal
	bool stop;
	double fx, fy, fz;
	double fx1, fy1, fz1;
	int ix, iy, iz;
	double val;
	int pos;
	double ddx, ddy, ddz;
	int rayPos;
	
	int nSize1[3];
	data->GetN(nSize1);
	nSize1[0] -= 1;	nSize1[1] -= 1;	nSize1[2] -= 1;

	unsigned short *dTab = data->GetDataArray();

	//start to kulka o srodku w punkcie klikniecia
	vtkPoints *pts = polys->GetPoints();
	int nrPts = pts->GetNumberOfPoints();

	int shootCentralMaskCenter[3];
	int shootCentralMaskN[3];
	int shootCentralMaskN1[3];
	
	double centerD[3] = { center[0], center[1], center[2] };
	//docelowo pierwszy strza³ zawsze powinien byc z punktu klikniecia to nie ma sensu walczyc z normalnymi 
	int nrChg = 0;
	if (center != NULL)
	{


		if (shootCentralMaskData != NULL)
		{
			shootCentralMaskData->GetN(shootCentralMaskN);
			for (int i = 0; i < 3; i++)
			{
				shootCentralMaskCenter[i] = (int)(shootCentralMaskN[i] / 2) + 1;
				shootCentralMaskN1[i] = shootCentralMaskN[i] - 1;
			}

		}

		//double *rayIntLengthTab = new double[nrPts];
		double *lengthTab = new double[nrPts];
		double *lengthTab2 = new double[nrPts];
		double *pointAndNormalTab = new double[nrPts*6];
		for (int i = 0; i < nrPts; i++)
		{
			pts->GetPoint(i, p);


			for (int j = 0; j < 3; j++)
			{
				n[j] = p[j] - center[j];
				//p1[j] = p[j];
			}

			dx = n[0];
			dy = n[1];
			dz = n[2];
			length = sqrt(dx*dx + dy*dy + dz*dz);
			double len1 = 0;
			if (length > 0)
			{
				//	len1 = minsize / length;
				len1 = 1.0 / length;
				n[0] *= len1;
				n[1] *= len1;
				n[2] *= len1;
			}
			//dostalem normaln¹ do strza³u dla ktorej bede biegl az kryterium stopu bedzie okej//
			// mam n przeskalowane do minimalnego kroku

			double maxDistD = maxDist / minsize;
			bool test = true;
			double ii = 0;
			int pos1D;
			unsigned short val;

			for (int j = 0; j < 3; j++)
			{
				iPPrev[j] = iP[j];
			}

			int ppxpos = i * 6;
			pointAndNormalTab[ppxpos++] = p[0]; pointAndNormalTab[ppxpos++] = p[1]; pointAndNormalTab[ppxpos++] = p[2];
			pointAndNormalTab[ppxpos++] = n[0]; pointAndNormalTab[ppxpos++] = n[1]; pointAndNormalTab[ppxpos++] = n[2];

			for (ii = 0; ii < maxDistD; ii += minsize)
			{
				//krok w kierunku zgodnym z normaln¹
				for (int j = 0; j < 3; j++)
				{
					iP[j] = p[j] + n[j] * ii;
					//p1[j] += n[j];
					//iP[j] = Round(p1[j]);
					//iP[j] = 1;
				}
				if (iP[0]<0 || iP[1]<0 || iP[2]<0 ||
					iP[0]>nSize1[0] || iP[1]>nSize1[1] || iP[2]>nSize1[2]) //wyszedlem poza dane - cofam sie o 1
				{
					ii -= 1; break;// test = false; break;
				}

				pos1D = data->GetVoxelPos(iP[0], iP[1], iP[2]);
				val = dTab[pos1D];
				if (mask != NULL)
				{
					if (mask[pos1D] ==0)
					{
						ii -= 1; break;// test = false; break;
					}
				}

				if (iP[0] != iPPrev[0] || iP[1] != iPPrev[1] || iP[2] != iPPrev[2])
				{
					if (shootCentralMaskData != NULL)
					{
						////maska do testu wstepnego
						//test czy punkt strzelony znajduje sie w masce - zeby nie trzeba bylo sprawdzac! >0

						//zgrzewam center i volume center

						int newP[3];
						for (int kk = 0; kk < 3; kk++)
						{
							newP[kk] = iP[kk] - center[kk] + shootCentralMaskCenter[kk];
						}

						bool toTest = true;
						//jesli punkt newP lezy w nowej masce to sprawdzam czy >0
						if (!(newP[0]<0 || newP[1]<0 || newP[2]<0 ||
							newP[0]>shootCentralMaskN1[0] || newP[1]>shootCentralMaskN1[1] || newP[2]>shootCentralMaskN1[2]))
						{
							int vv = shootCentralMaskData->GetVoxelValue(newP[0], newP[1], newP[2]);
							if (vv > 0)
							{
								test = true;
								toTest = false;
							}

						}

						if (toTest == true)
							test = (cr->CheckPoint(iP[0], iP[1], iP[2], NULL, 0) > 0);
					}
					else
					{

						test = (cr->CheckPoint(iP[0], iP[1], iP[2], NULL, 0) > 0);
					}

					for (int j = 0; j < 3; j++)
					{
						iPPrev[j] = iP[j];
					}
				}
				if (test == false)
				{
					ii -= 1;
					break;
				}


			}

			//if ((test == true) && 

			for (int j = 0; j < 3; j++)
			{
				p1[j] = p[j] + n[j] * ii;
			}
			//rayIntLengthTab[i] = ii;
			lengthTab[i] = ii;// GetDistance(centerD, p1);;

			//if ((ii > 0))
			//{
			//	//lengthTab[jj] = ii;
			//	//przesunpunkt 

			//	for (int j = 0; j < 3; j++)
			//	{
			//		//p1[j] = p[j] + n[j] * ii;
			//		//p1[j] -= n[j];

			//		p1[j] = p[j] + n[j] * ii;
			//	}
			//	pts->SetPoint(i, p1);
			//	nrChg++;
			//}
		}

	
		if (exportParams != NULL)
		{

			//biore sie za mediany
			bool median = exportParams[0];
		
			
		
			if (median == true)
			{
				//double distMed3D = 1.0;
				int maxMedSamples = 400;
				memset(lengthTab2, 0, nrPts*sizeof(double));
				int medianKernel = exportParams[1];
				double medianFactor = exportParams[2];
				bool onlyShortening = exportParams[3];
				int medianThreshold = exportParams[4];
				int dilateDist = exportParams[5];
				int dilateType = exportParams[6];
				//zebrac wczesniej dist tab a potem filtowac po otoczeniach!
				qDebug() << "median:" << median << "medianKernel: " << medianKernel;
				//zbiore tabelke z kolejnymi odl dla medianSize;

				/*int *distxTab = new int[nrPts];
				int nrCat = 0;
				double tol = 1e-3;
				for (int i = 1; i < nrPts; i++)
				{
					if (fabs(neighTab[0].distTab[i - 1] - neighTab[0].distTab[i ]) > tol) nrCat++;
					distxTab[i] = nrCat;

				}*/




				double *medTab = new double[maxMedSamples];
				int pp;
				qDebug() << "nrPT:s " << nrPts << " maxMedSamples" << maxMedSamples ;
				for (int i = 0; i < nrPts; i++)
				{
					int lenMax = neighTab[i].nrOfElements;
					int *distxTab = neighTab[i].distTab;
					//qDebug() << "lenMax " << lenMax << " maxMedSamples" << maxMedSamples ;

					int j = 0;
					for ( j = 0; j < maxMedSamples; j++)
					{
					
						if ((j >= lenMax) || (distxTab[j]>medianKernel))// (neighTab[i].distTab[j]>distMed3D))
						{
							//j--;
							break;
						}
						pp = neighTab[i].indexTab[j];
						medTab[j] =lengthTab[pp] ;
						
					}
					j--;

					//qDebug() << "j: " << j;
					if (j > 1)
					{

						double med = Median(medTab, j);
						lengthTab2[i] = med - lengthTab[i]; //roznica

						//qDebug() << "median:" << med  << " lengthTab[i]" << lengthTab[i] << " tblen: " << j;

						//qDebug() << "0:" << medTab[0] << " srodek: " << medTab[j / 2] << " koniec:" << medTab[j];

					}
				}
				int lenx;
				for (int i = 0; i < nrPts; i++)
				{
					lenx = ((fabs(lengthTab2[i]) < medianThreshold) ? 0 : medianFactor*lengthTab2[i]) + lengthTab[i];// (1.0 - medianFactor)*lengthTab2[i + medianKernel];


					lengthTab[i] = (!onlyShortening) ? lenx : (lenx < lengthTab[i]) ? lenx : lengthTab[i]; //wstawiam to co wyszlo z median chyba ze mam tylko skracanie to sprawdzam czy krotszy przed zamiana
				}
				delete[]medTab;
				//delete[]distxTab;
			
			}

		}

		int nPos = 0;
		for (int i = 0; i < nrPts; i++)
		{
			for (int j = 0; j < 3; j++)p[j] = pointAndNormalTab[nPos++];
			for (int j = 0; j < 3; j++)n[j] = pointAndNormalTab[nPos++];
			for (int j = 0; j < 3; j++)p1[j] = p[j] + n[j] * lengthTab[i];
			pts->SetPoint(i, p1);

		}
		
		//delete[] rayIntLengthTab;
		delete[]lengthTab;
		delete[]lengthTab2;
		delete[]pointAndNormalTab;

	}









}
//------------------------------------------
void ExplodeSegmentation::RayShoot(RawDataSet *data,unsigned char *mask, vtkPolyData *polys, double minsize,SegCriterion *cr,RayMapPolyFilter *rf, int nrPtsOld,unsigned char *mask0)
{
	double origin[3];
	data->GetOrigin(origin);
	//robie kopie polys - 
	//pierwsza pokoloruje po first guess
	//druga pokoloruje po appllyfilter

	//vtkPolyData *spherePolys=vtkPolyData::New();
	//spherePolys->DeepCopy(polys);
	//spherePolys->Update();




	 
	int i,j,k;
	double ss=cr->neighSize+0.01;
	double p[3],p1[3],d[3],n[3];

	double pStart[3];
	double dx,dy,dz;
	double dx1,dy1,dz1;
	double length,length1;
	//startVal
	bool stop;
	double fx,fy,fz;
	double fx1,fy1,fz1;
	int ix,iy,iz;
	double val;
	int pos;
	double ddx,ddy,ddz;
	int rayPos;



	vtkPoints *pts=polys->GetPoints();


	
	int nr=pts->GetNumberOfPoints();

	vtkPoints *nrls=vtkPoints::New();
	vtkPolyDataNormals *normals=vtkPolyDataNormals::New();
	normals->SplittingOff ();

	normals->ComputePointNormalsOn ();
	normals->NonManifoldTraversalOn  () ;
	normals->ComputeCellNormalsOff ();
	normals->SetInputData(polys);
	normals->Update();


	vtkPolyData *tempData=vtkPolyData::New();
	tempData->DeepCopy(normals->GetOutput());
//	tem//pData->Update();
	vtkDataArray *normalsT=tempData->GetPointData()->GetNormals();


	for(i=0;i<nr;i++)
	{
		normalsT->GetTuple(i,n);

		dx=n[0];
		dy=n[1];
		dz=n[2];
		length=sqrt(dx*dx+dy*dy+dz*dz);
		if (length>0)
		{
			n[0]=-n[0]*minsize/length;
			n[1]=-n[1]*minsize/length;
			n[2]=-n[2]*minsize/length;
		}

		nrls->InsertNextPoint(n);
	}


	normals->Delete();
	tempData->Delete();


//normalne wygenerowaen

	int rayLen=512;
	

	double xS=data->GetXscale();
	double yS=data->GetYscale();
	double zS=data->GetZscale();
	double scale[3]={xS,yS,zS};

	vtkPoints *ptsX=vtkPoints::New();
	ptsX->DeepCopy(pts);

	unsigned short *rayTab=new unsigned short[rayLen];
	RawDataSet *rayData=new RawDataSet(rayLen,nr,1);
		//RawDataSet *rayData=new RawDataSet(rayLen,nr/100+1,100);
	rayData->ZeroQ();
	unsigned short *rayDataTab=rayData->GetDataArray();
// 1. PRZEBIEGNE po wszystkich punktach i zbiore promienie
	double valc;
	int NN[3];
	data->GetN(NN);

	if (testFlag==true)
	{
		i=i;
	}
	for (i=0;i<nr;i++)
	{
		pts->GetPoint(i,p);
		nrls->GetPoint(i,n);

		dx=n[0];				dy=n[1];				dz=n[2];
		fx=p[0]-origin[0];		fy=p[1]-origin[1];		fz=p[2]-origin[2];

		int testval;

		stop=false;
		rayPos=1;
		rayTab[0]=0;
		int valc2;

		memset(rayTab,0,rayLen*sizeof(unsigned short));
		do
		{
		

			ix=Round(fx/xS);		iy=Round(fy/yS);		iz=Round(fz/zS);
			
			//ix=(int)(fx/xS);		iy=(int)(fy/yS);		iz=(int)(fz/zS);
			
			ddx=fx/xS;			ddy=fy/yS;			ddz=fz/zS;

			if ((ix<=ss)||(iy<=ss)||(iz<=ss)||(ix>NN[0]-ss)||(iy>=NN[1]-ss)||(iz>NN[2]-ss)||(rayPos==rayLen-1))//krance
			{
				stop=true;
				if (rayTab[0]==0) rayTab[0]=rayPos;
			}
			else
			{

				valc=Interpolation3LinearUnsignedChar(mask,NN[0],NN[1],NN[2],ddx,ddy,ddz);
			

				rayTab[rayPos++]=val=Interpolation3Linear(data,ddx,ddy,ddz);
				
// 2. na poczatu promienia wstawie punkt konca
				if (rayTab[0]==0)
				{
					if (mask0!=NULL)
					{
						valc2=mask0[data->GetVoxelPos(ix,iy,iz)];
						if (valc2==1) 
							{
								rayTab[0]=rayPos-1;
								break;
						}
						
					}
					
					if ((rayPos>1)&&(valc>0.9))
					{
						rayTab[0]=rayPos-1;
					}
					else

					/*if
						(val>=500)*/
					if ((cr->CheckPoint(ix,iy,iz,rayTab,rayPos)>0) )
					{
						 rayTab[0]=rayPos;
					}
				}
				fx+=dx;	fy+=dy;	fz+=dz;
			}
	

		}while(stop==false);

		//wkopiowuje aktualny raylen do duzego zbioru;
		memcpy(rayDataTab+i*rayLen,rayTab,rayLen*sizeof(unsigned short));


	}



	for (i=nrPtsOld;i<nr;i++)
{
	ptsX->GetPoint(i,p);
	nrls->GetPoint(i,n);
	dx=n[0];
	dy=n[1];
	dz=n[2];
	val=rayDataTab[i*rayLen];
		
	//length=sqrt(dx*dx+dy*dy+dz*dz);
	p[0]+=dx*(val);
	p[1]+=dy*(val);
	p[2]+=dz*(val);

	pts->SetPoint(i,p);
}

		vtkPolyData *pp;
	PolygonSurfaceData *o2;

	{
rf->ApplyFiltering(polys,rayData,NULL,NULL);
	}

	if (testFlag==true)
	{

								vtkCellArray *skeletLine=vtkCellArray::New();
								vtkIdType newCellPts[2];
								vtkIdType id1,id2;
								vtkPoints *skelPts=vtkPoints::New();


for (i=nrPtsOld;i<nr;i++)
{
	ptsX->GetPoint(i,p);
								id1=skelPts->InsertNextPoint(p);
	nrls->GetPoint(i,n);
	dx=n[0];
	dy=n[1];
	dz=n[2];
	val=rayDataTab[i*rayLen];
	//length=sqrt(dx*dx+dy*dy+dz*dz);
	p[0]+=dx*(val);
	p[1]+=dy*(val);
	p[2]+=dz*(val);
								id2=skelPts->InsertNextPoint(p);
								newCellPts[0]=id1;
								newCellPts[1]=id2;
								skeletLine->InsertNextCell(2,newCellPts);

	pts->SetPoint(i,p);

}



								vtkPolyData *skelData=vtkPolyData::New();
								skelData->SetPoints(skelPts);
								skelData->SetLines(skeletLine);
								skelData->SetPolys(skeletLine);
								skelData->BuildCells();
//								skelData->Update();


								pp=vtkPolyData::New();	pp->DeepCopy(skelData);	/*pp->Update();*/
								o2=new PolygonSurfaceData(pp,vWidget);	o2->SetPolygonSurfaceDataSetName("Jezyk ");	o2->GetVActor()->GetProperty()->SetColor(0.1,0.1,1.0);	o2->ComputeGeometricOrigin();
								o2->SetGroupName("hedgehog");
								emit	SignalSendNewSurface(o2);

	}
//*******************

//if (testFlag==false)
for (i=nrPtsOld;i<nr;i++)
{
	ptsX->GetPoint(i,p);
								
	nrls->GetPoint(i,n);
	dx=n[0];
	dy=n[1];
	dz=n[2];
	val=rayDataTab[i*rayLen];
	//length=sqrt(dx*dx+dy*dy+dz*dz);
	p[0]+=dx*(val);
	p[1]+=dy*(val);
	p[2]+=dz*(val);
							

	pts->SetPoint(i,p);

}				

delete []rayTab;
ptsX->Delete();

rayData->SetRawDataSetName("raymap");
//rayData->CalculateHistogram();
//if (testFlag==true) 
//emit this->SignalSendNewRawDataSet(rayData);
//else
delete rayData;
//*******************


}
//-----------------------------------------------------------------------------

vtkPolyData *ExplodeSegmentation::ColorizeData(vtkPolyData *pData,int *tab)
{
	vtkLookupTable *lut = vtkLookupTable::New();
	lut->SetNumberOfColors(256);
	
	int i;
	for(i=0;i<256;i++)
	{
		lut->SetTableValue(i,i/255.0,i/255.0,i/255.0);
	}
	lut->Build();

	vtkIntArray *scalars = vtkIntArray::New(); 

	int nrOfPoints=pData->GetNumberOfPoints();
	double max=0;
	double min=100000;
	for(i=0;i<nrOfPoints;i++)
	{
		if (tab[i]>max) max=tab[i];
		if (tab[i]<min) min=tab[i];

	}
	if (abs(max)<1e-5) max=1.0;
	int val=0;
	max=max-min;
	for(i=0;i<nrOfPoints;i++)
	{
		val=(tab[i]-min)*255.0/max;
		scalars->InsertNextValue(val);
	}
	lut->SetAlphaRange(0,255);
	lut->SetTableRange(0,255);
	scalars->SetLookupTable(lut);
	vtkPolyData *colPolys= vtkPolyData::New();
	colPolys->DeepCopy(pData);
	colPolys->GetPointData()->SetScalars(scalars);
	//colPolys->Update();
	
	return colPolys;

	

}
