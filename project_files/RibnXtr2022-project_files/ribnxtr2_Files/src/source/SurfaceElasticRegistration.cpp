
#include "SurfaceElasticRegistration.h"




SurfaceElasticRegistration::SurfaceElasticRegistration(PolygonSurfaceData *ModelSurfaceData,PolygonSurfaceData *ObjectSurfaceData,
													   DistanceMap *_Distance,double *_trans,double *Origin,VtkWidget *_VWidget,FloatGrid3D *_Grid,const QString &text)
{
	tempPolyData=NULL;
	tempActor=NULL;

	objN=ObjectSurfaceData->GetPN();
	objS=ObjectSurfaceData->GetPScale();


	int i,j,k;
	initText=text;

	Grid=new FloatGrid3D();
	Grid->Copy(*_Grid);
	OrigGrid=_Grid;

	NodX=0;
	NodY=0;
	NodZ=0;

	VWidget=_VWidget;
	distance=_Distance;
	ModelPolyData=ModelSurfaceData->GetVPolyData();
	ObjectPolyData=ObjectSurfaceData->GetVPolyData();
	ObjectCuttingEdgeActor=ObjectSurfaceData->GetCuttingEdgeActor();
	ObjectActor=ObjectSurfaceData->GetVActor();


	tempProperty=vtkProperty::New();
	tempProperty->DeepCopy(ObjectActor->GetProperty());

	tempOpacity=tempProperty->GetOpacity();
	ObjectActor->GetProperty()->SetOpacity(0.0);
	

	Matrix=ObjectSurfaceData->GetMatrix();
	trans=_trans;
	precalculate=false;
	transx=NULL;

	scaleX=ModelSurfaceData->GetPScale()[0];
	scaleY=ModelSurfaceData->GetPScale()[1];
	scaleZ=ModelSurfaceData->GetPScale()[2];

	RotateCenter = new double[3];
	for(i=0;i<3;i++)
		RotateCenter[i]=Origin[i];

	transform=new Transform(ObjectSurfaceData,RotateCenter,-1);
	transform->CreateIndexedArray(ModelSurfaceData->GetPScale()[0],ModelSurfaceData->GetPScale()[1],ModelSurfaceData->GetPScale()[2]);
	transform->CalculateTrigonometricFunctions(trans);



	//nnx nny nnz to rozciaglasc w osiach powierzchni;
	double nnx=0;
	double nny=0;
	double nnz=0;
	///////////////////////////
	int nr=transform->NPoint;
	double *tab=transform->IndexedArray;

	double x,y,z;
	int pos=0;

	double MinBound[3],MaxBound[3];
	MinBound[0]=INT_MAX; MinBound[1]=INT_MAX; MinBound[2]=INT_MAX;
	MaxBound[0]=INT_MIN; MaxBound[1]=INT_MIN; MaxBound[2]=INT_MIN;

	double a,b,c;
	for(i=1;i<=nr;i++)
	{
		transform->ApplyTransformationFloat(i,trans,x,y,z,a,b,c);
		if (x>MaxBound[0]) MaxBound[0]=x;	if (y>MaxBound[1]) MaxBound[1]=y;	if (z>MaxBound[2]) MaxBound[2]=z;
		if (x<MinBound[0]) MinBound[0]=x;	if (y<MinBound[1]) MinBound[1]=y;	if (z<MinBound[2]) MinBound[2]=z;
		tab[pos++]=x; tab[pos++]=y; tab[pos++]=z;
	}
	nnx=fabs(MaxBound[0]-MinBound[0]);	nny=fabs(MaxBound[1]-MinBound[1]);	nnz=fabs(MaxBound[2]-MinBound[2]);
	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////
	// policzyc ile pktow w kazdym cuboidzie
	nsize=Grid->GetNx()*Grid->GetNy()*Grid->GetNz();
	tabnr=new int[nsize];
	//tabXXX=new int[transform->NPoint];
	memset(tabnr,0,nsize*sizeof(int));
	int index=0;


	tdx=(double)(Grid->GetNx()-1)/nnx;
	tdy=(double)(Grid->GetNy()-1)/nny;
	tdz=(double)(Grid->GetNz()-1)/nnz;

	fx=1.0/tdx;
	fy=1.0/tdx;
	fz=1.0/tdx;
	//Grid->Scale(fx,fy,fz);

	nx=Grid->GetNx();
	ny=Grid->GetNy();
	nz=Grid->GetNz();
	nxny=Grid->GetNx()*Grid->GetNy();

	double P1[3];
	//double dx,dy,dz;
	int posx;
	//sprawdzam ile najsamprzod
	pos=0;
	for(i=1;i<=nr;i++)
	{
		P1[0]=transform->IndexedArray[pos++]-MinBound[0];
		P1[1]=transform->IndexedArray[pos++]-MinBound[1];
		P1[2]=transform->IndexedArray[pos++]-MinBound[2];

		posx=((int)(P1[2]*tdz)*nxny+(int)(P1[1]*tdy)*nx+(int)(P1[0]*tdx));
		tabnr[posx]++;

	}


	tabpos=new int*[nsize];
	for(i=0;i<nsize;i++)
		tabpos[i]=new int[tabnr[i]];

	memset(tabnr,0,nsize*sizeof(int));
	//pozbieram do tablic
	pos=0;
	index=0;
	for(i=1;i<=nr;i++)
	{
		P1[0]=transform->IndexedArray[pos++]-MinBound[0];
		P1[1]=transform->IndexedArray[pos++]-MinBound[1];
		P1[2]=transform->IndexedArray[pos++]-MinBound[2];

		posx=((int)(P1[2]*tdz)*nxny+(int)(P1[1]*tdy)*nx+(int)(P1[0]*tdx));
		tabpos[posx][tabnr[posx]++]=index;
		index+=3;
	}

	// indexy pozbierane mozna uzywac
	/////////////////////////////////
	/////////////////////////////////
	/////////////////////////////////
	BSplineTransform=new BSplineGridTransformation(Grid,nnx,nny,nnz,MinBound[0],MinBound[1],MinBound[2]);

	//Step[0]=nnx/((Grid->GetNx()-1));
	//Step[1]=nny/((Grid->GetNy()-1));
	//Step[2]=nnz/((Grid->GetNz()-1));
	Step[0]=1.0;
	Step[1]=1.0;
	Step[2]=1.0;

}

//--------------------------------------------------------------------

SurfaceElasticRegistration::~SurfaceElasticRegistration()
{
	tempProperty->Delete();
	tempProperty=NULL;
	if (tempPolyData!=NULL)
	{	
		VWidget->Renderer->RemoveActor(tempActor);
		tempPolyData->Delete();
		tempActor->Delete();
		tempActor=NULL;
		tempPolyData=NULL;
		ObjectActor->GetProperty()->SetOpacity(tempOpacity);
	}

	VWidget->Interactor->repaint();
	
	objN=NULL;
	objS=NULL;

	if (Grid!=NULL)
		delete Grid;
	OrigGrid=NULL;

	if (BSplineTransform!=NULL) delete BSplineTransform;


	if (tabnr!=NULL) delete []tabnr;
	if (tabpos!=NULL)
	{

		int i;
		for (i=0;i<nsize;i++)
			if (tabpos[i]!=NULL)
				delete []tabpos[i];
		delete []tabpos;
	}

}

//--------------------------------------------------------------------

void SurfaceElasticRegistration::ActorUpdate()
{
	if ((ObjectPolyData!=NULL)&&(Grid!=NULL))
	{
		if (tempPolyData!=NULL)
		{	
			VWidget->Renderer->RemoveActor(tempActor);
			tempPolyData->Delete();
			tempActor->Delete();
		
		}
		
		
			// zrobic nowe polydata kopiujac objectpolydata i deformujac je z deka
			tempPolyData=vtkPolyData::New();
			tempPolyData->DeepCopy(ObjectPolyData);
			SurfaceProcessing *sProc=new SurfaceProcessing(VWidget);
			double min[3],nn[3];
			sProc->CalculateMinAndSizeOfPolys(tempPolyData,objN,objS,min,nn);
			sProc->ApplyFFDDeformation(Grid,tempPolyData,1.0,min,nn);
//			tempPolyData->Update();


			tempActor	=	vtkActor::New();
			vtkPolyDataMapper *tempMapper	=	vtkPolyDataMapper::New();
			tempMapper->SetInputData(tempPolyData);
			tempActor->SetMapper(tempMapper);
			tempActor->SetProperty(tempProperty);
			//tempActor->GetProperty()->SetRepresentationToWireframe ();
			VWidget->Renderer->AddActor(tempActor);
			delete sProc;
			VWidget->Interactor->repaint();
			emit	SignalGrab3Window();
	}	
}

//--------------------------------------------------------------------

double SurfaceElasticRegistration::GetEnergy()
{
	// dla ustalonego gridu znieksztalcen
	//	double  x1,y1,z1;
	double P1[3],P2[3];
	int i;
	double Distance;
	double DistX,DistY,DistZ;//,rx1,ry1,rz1;

	double Energy=0;
	// sprawdzoc czy znieksztalcenie jest za duze jesli tak zwrocic maxa
	double def[4];

	GetTransFromCurrentNode(def);
	if  (
		((fabs(def[1])>Step[0])||(fabs(def[2])>Step[1])||(fabs(def[3])>Step[2]))
		||
		(Grid->Check3DLocus(NodX,NodY,NodZ)==false)
		)
	{
		return INT_MAX;
	}
	else
	{
		int pos=0;
		for (i=1;i<=transform->NPoint;i++) 
		{		
			//transform->ApplyTransformationdouble(i,ta,x1,y1,z1,rx1,ry1,rz1);
			P1[0]=transform->IndexedArray[pos++];
			P1[1]=transform->IndexedArray[pos++];
			P1[2]=transform->IndexedArray[pos++];	

			BSplineTransform->TransformPoint(P1,P2);
			distance->GetDistanceValues(P2[0],P2[1],P2[2],Distance,DistX,DistY,DistZ);
			Energy+=Distance*Distance;
		}
	}
	return Energy;


}
//--------------------------------------------------------------------
double SurfaceElasticRegistration::GetEnergy(int x,int y,int z)
{
	double def[4];

	double beta;
	int i,j,k;
	int pos;
	double energy=0.0;
	/*if ((NodX==0)&&(NodY==2)&&(NodZ==0))
	{
			i=i;
	}*/
	GetTransFromCurrentNode(def);
	//beta=Grid->GetMaxDistanceWith3DLocus(NodX,NodY,NodZ);
	//		//beta=pow(exp(beta),0.01)-1;
	//		beta*=beta;
	beta=1;
	if  (
		((fabs(def[1])>Step[0]/2)||(fabs(def[2])>Step[1]/2)||(fabs(def[3])>Step[2]/2))
		||
		(Grid->Check3DLocus(NodX,NodY,NodZ)==false)
		)
	{
		return INT_MAX;
	}
	else
		//for (k=z-3;k<z+4;k++)
		//	for (j=y-3;j<y+4;j++)
		//		for (i=x-3;i<x+4;i++)
		//		{
		//			if ((i>=0)&&(i<nx))
		//				if ((j>=0)&&(j<ny))
		//					if ((k>=0)&&(k<nz))
		//					{
		//						pos=k*nxny+j*nx+i;
		//						// no i tu bede sprawdzal pkty w kolejnym cuboidzie
		//						energy+=GetEnergy(pos);
		//					}
		//		}
	energy=GetEnergy(-1);
				//energy*=beta;
				return energy;
}

//-------------------------------------------------
double SurfaceElasticRegistration::GetEnergy(int posx)
{
	// dla ustalonego gridu znieksztalcen
	//	double  x1,y1,z1;
	double P1[3],P2[3];
	int i;//,j;
	double Distance;
	double DistX,DistY,DistZ;//,rx1,ry1,rz1;

	double energy=0;
	//	int pos=0;
	int *tab;
	int posA; 
	//	double ex=0;
	//	double ey=0;
	//	double ez=0;
	//	int ii=0;

	if (posx==-1)
	{
		posA=0;
		for(i=0;i<transform->NPoint;i++)
		{
			P1[0]=transform->IndexedArray[posA++];
			P1[1]=transform->IndexedArray[posA++];
			P1[2]=transform->IndexedArray[posA];	

			BSplineTransform->TransformPoint(P1,P2);
			distance->GetDistanceValues(P2[0],P2[1],P2[2],Distance,DistX,DistY,DistZ);
			energy+=Distance*Distance;	
		}
	}
	else
	{
		tab=tabpos[posx];
		for (i=0;i<tabnr[posx];i++)
		{
			posA=tab[i];
			P1[0]=transform->IndexedArray[posA++];
			P1[1]=transform->IndexedArray[posA++];
			P1[2]=transform->IndexedArray[posA];	

			BSplineTransform->TransformPoint(P1,P2);
			distance->GetDistanceValues(P2[0],P2[1],P2[2],Distance,DistX,DistY,DistZ);
			energy+=Distance*Distance;	

		}
	}
	tab=NULL;



	return energy;

}

//------------------------------------------------
//------------------------------------------------
int SurfaceElasticRegistration::GetNumberOfNeighboringPoints(int x,int y,int z)
{
	int pos;
	int i,j,k;
	int pts=0;
	for (k=z-1;k<z+3;k++)
		for (j=y-1;j<y+3;j++)
			for (i=x-1;i<x+3;i++)
			{
				if ((i>=0)&&(i<nx))
					if ((j>=0)&&(j<ny))
						if ((k>=0)&&(k<nz))
						{
							pos=k*nxny+j*nx+i;
							// no i tu bede sprawdzal pkty w kolejnym cuboidzie
							pts+=tabnr[pos];
						}
			}
			return pts;
}
//-------------------------------------------------
void SurfaceElasticRegistration::Optimize(int iter)
{

	double Tolerance=10e-2;
	int Iterations=0;
	double FValue=0;
	int NumberOfOptimParams=3;

	if (transx!=NULL) delete []transx;
	transx=new double[4];
	//OptimTrans

	OptimizationPowell *Optim=NULL;


	int i,j,k,l;

	//FILE *Do_opa;
	///Do_opa=fopen("c:\\powell_costam.txt","w");
	//fprintf(Do_opa,"start \t%lf\n",FValue);
//	fclose(Do_opa);
	int nr;

	double temptrans[4];
		
	double e,e1;
	double tolX=10e-8;
	//int iter=2;


	//QString t="("+QString::number(nx)+"/"+QString::number(ny)+"/"+QString::number(nz)+"/"+QString::number(iter)+")";
	QString t=QString::number(nx*ny*nz*iter);

	int ii,jj,kk;
	int pos=0;
	int step=0;
	for(l=0;l<iter;l++)

		for (k=step;k<nz-step;k++)
			for (j=step;j<ny-step;j++)
				for (i=step;i<nx-step;i++)
				{

					if ((l%2)==0)	{	ii=i;	jj=j;	kk=k;}
					else			{ii=nx-1-i;	jj=ny-1-j;	kk=nz-1-k;}

					//	emit SendMessage("("+QString::number(i+1)+"/"+QString::number(j+1)+"/"+QString::number(k+1)+"/"+QString::number(l+1)+")/"+t);
					emit SendMessage(initText+" ("+QString::number(++pos)+"/"+t+")");				
					SetCurrentNode(ii,jj,kk);
					GetTransFromCurrentNode(transx);

					e=GetFunctionValue(transx);
					temptrans[1]=transx[1];
					temptrans[2]=transx[2];
					temptrans[3]=transx[3];

					nr=GetNumberOfNeighboringPoints(ii,jj,kk);
					if (nr>0)
					{
						Optim=new OptimizationPowell(transx,NumberOfOptimParams,Tolerance,&Iterations,&FValue,this); //:) tak mi sie zrobilo 30.09.2003
						Optim->StartOptimization();
						delete Optim;
						ActorUpdate(); //- zeby bylo widac!!!

					e1=FValue;

					// jezeli stara-nowa jest w gr tolerancji to ustaw stara
					if (fabs(e1-e)<tolX)
					{
						transx[1]=temptrans[1];
						transx[2]=temptrans[2];
						transx[3]=temptrans[3];

					//	Do_opa=fopen("c:\\powellvolelas.txt","a");fprintf(Do_opa,"qq\n");fclose(Do_opa);
						e1=e;
					}

						// ta linijka nizej jest po to aby ustawic optymalna transformacje
						SetTransToCurrentNode(transx);
					//	Do_opa=fopen("c:\\powell_costam.txt","a");
					//	//fprintf(Do_opa,"%d \t%d\t%d\t%lf\n",ii,jj,kk,FValue);	
					//	fclose(Do_opa);
					}

				}
				// zeby grid wyjsciowy zawieral wartosci 0-1
				OrigGrid->Copy(*Grid);
				//				OrigGrid->Scale(tdx,tdy,tdz);


				//connect(this,SIGNAL(SignalStopOptimization()),Optim,SLOT(SlotStopOptimization())); 
				//
				//Optim->StartOptimization();
				//OptimStopped=Optim->OptimizationStopped;



}
//-------------------------------------------------------------------
double	SurfaceElasticRegistration::GetFunctionValue(double *params)
{

	SetTransToCurrentNode(params);
	//double e=GetEnergy();
	double e=GetEnergy(NodX,NodY,NodZ);

	//FILE *Do_opa;
	//Do_opa=fopen("c:\\powell_costam.txt","a");


	/*for(int i=1;i<4;i++)
	{fprintf(Do_opa,"%lf\t",params[i]);}
	fprintf(Do_opa,"e= %lf \n",e);
	fclose(Do_opa);
*/
	return e;

}
//-------------------------------------------------------------------
void	SurfaceElasticRegistration::SetCurrentNode(int x,int y,int z)
{
	if ((x>=0) && (x<Grid->GetNx()))
		if ((y>=0) && (y<Grid->GetNy()))
			if ((z>=0) && (z<Grid->GetNz()))
			{

				NodX=x;
				NodY=y;
				NodZ=z;
			}
}
//-------------------------------------------------------------------
void SurfaceElasticRegistration::GetTransFromCurrentNode(double *tr)
{

	tr[1]=Grid->GetDX(NodX,NodY,NodZ);
	tr[2]=Grid->GetDY(NodX,NodY,NodZ);
	tr[3]=Grid->GetDZ(NodX,NodY,NodZ);
}
//-------------------------------------------------------------------
void SurfaceElasticRegistration::SetTransToCurrentNode(double *tr)
{
	Grid->SetDx(NodX,NodY,NodZ,tr[1]);
	Grid->SetDy(NodX,NodY,NodZ,tr[2]);
	Grid->SetDz(NodX,NodY,NodZ,tr[3]);

}



//#include "SurfaceElasticRegistration.h"
//
//
//
//
//SurfaceElasticRegistration::SurfaceElasticRegistration(PolygonSurfaceData *ModelSurfaceData,PolygonSurfaceData *ObjectSurfaceData,
//													   DistanceMap *_Distance,double *_trans,double *Origin,VtkWidget *_VWidget,FloatGrid3D *_Grid,const QString &text,QString _fileDefaultDir)
//{	
//	fileDefaultDir=_fileDefaultDir;
//
////////////////////////
//	/*{
//	
//	double Distance, DistX,DistY,DistZ;
//	RawDataSet *r=new RawDataSet(200,200,200,1.0,1.0,1.0);
//	r->ZeroQ();
//
//	int i,j,k;
//	for(k=0;k<200;k++)
//		for(j=0;j<200;j++)
//			for(i=0;i<200;i++)
//			{
//
//				_Distance->GetDistanceValues(i,j,k,Distance,DistX,DistY,DistZ);
//				r->SetVoxelValue(i,j,k,Distance);
//			}
//
//			QString name=this->fileDefaultDir+"\\y.dat";
//			r->SaveRawDataSet(name);
//
//	}*/
//////////////////////////
//
//
//
//
//	tempPolyData=NULL;
//	tempActor=NULL;
//
//	objN=ObjectSurfaceData->GetPN();
//	objS=ObjectSurfaceData->GetPScale();
//
//	
//
//
//
//	int i,j,k;
//	initText=text;
//
//	Grid=new FloatGrid3D();
//	Grid->Copy(*_Grid);
//	OrigGrid=_Grid;
//
//	NodX=0;
//	NodY=0;
//	NodZ=0;
//
//	VWidget=_VWidget;
//	distance=_Distance;
//	ModelPolyData=ModelSurfaceData->GetVPolyData();
//	ObjectPolyData=ObjectSurfaceData->GetVPolyData();
//	ObjectCuttingEdgeActor=ObjectSurfaceData->GetCuttingEdgeActor();
//	ObjectActor=ObjectSurfaceData->GetVActor();
//
//
//	tempProperty=vtkProperty::New();
//	tempProperty->DeepCopy(ObjectActor->GetProperty());
//
//	tempOpacity=tempProperty->GetOpacity();
//	ObjectActor->GetProperty()->SetOpacity(0.0);
//
//
//	Matrix=ObjectSurfaceData->GetMatrix();
//	trans=_trans;
//	precalculate=false;
//	transx=NULL;
//
//	scaleX=1.0/ModelSurfaceData->GetPScale()[0];
//	scaleY=1.0/ModelSurfaceData->GetPScale()[1];
//	scaleZ=1.0/ModelSurfaceData->GetPScale()[2];
//
//
//
//
//
//	RotateCenter = new double[3];
//	for(i=0;i<3;i++)
//		RotateCenter[i]=0;
//		//RotateCenter[i]=Origin[i];
//
//	transform=new Transform(ObjectSurfaceData,RotateCenter,-1);
//	transform->CreateIndexedArray(ModelSurfaceData->GetPScale()[0],ModelSurfaceData->GetPScale()[1],ModelSurfaceData->GetPScale()[2]);
//	transform->CalculateTrigonometricFunctions(trans);
//
//
//	//nnx nny nnz to rozciaglasc w osiach powierzchni;
//	nnx=0;
//	nny=0;
//	nnz=0;
//	///////////////////////////
//	int nr=transform->NPoint;
//	double *tab=transform->IndexedArray;
//
//	double x,y,z;
//	int pos=0;
//
//	double MinBound[3],MaxBound[3];
//	MinBound[0]=INT_MAX; MinBound[1]=INT_MAX; MinBound[2]=INT_MAX;
//	MaxBound[0]=INT_MIN; MaxBound[1]=INT_MIN; MaxBound[2]=INT_MIN;
//
//	double a,b,c;
//	for(i=1;i<=nr;i++)
//	{
//		//transform->ApplyTransformationdouble(i,trans,x,y,z,a,b,c);
//		x=tab[pos++];
//		y=tab[pos++];
//		z=tab[pos++];
//		
//		if (x>MaxBound[0]) MaxBound[0]=x;	if (y>MaxBound[1]) MaxBound[1]=y;	if (z>MaxBound[2]) MaxBound[2]=z;
//		if (x<MinBound[0]) MinBound[0]=x;	if (y<MinBound[1]) MinBound[1]=y;	if (z<MinBound[2]) MinBound[2]=z;
//		//tab[pos++]=x; tab[pos++]=y; tab[pos++]=z;
//	}
//	nnx=fabs(MaxBound[0]-MinBound[0]);	nny=fabs(MaxBound[1]-MinBound[1]);	nnz=fabs(MaxBound[2]-MinBound[2]);
//
//	/////////////////////////////////
//	/////////////////////////////////
//	/////////////////////////////////
//	/////////////////////////////////
//	// policzyc ile pktow w kazdym cuboidzie
//	nsize=Grid->GetNx()*Grid->GetNy()*Grid->GetNz();
//	tabnr=new int[nsize];
//	//tabXXX=new int[transform->NPoint];
//	memset(tabnr,0,nsize*sizeof(int));
//	int index=0;
//
//
//	tdx=(double)(Grid->GetNx()-1)/nnx;
//	tdy=(double)(Grid->GetNy()-1)/nny;
//	tdz=(double)(Grid->GetNz()-1)/nnz;
//
//	fx=1.0/tdx;
//	fy=1.0/tdx;
//	fz=1.0/tdx;
//	//Grid->Scale(fx,fy,fz);
//
//	nx=Grid->GetNx();
//	ny=Grid->GetNy();
//	nz=Grid->GetNz();
//	nxny=Grid->GetNx()*Grid->GetNy();
//
//
//	double P1[3];
//	//double dx,dy,dz;
//	int posx;
//	//sprawdzam ile najsamprzod
//	pos=0;
//	for(i=1;i<=nr;i++)
//	{
//		P1[0]=transform->IndexedArray[pos++]-MinBound[0];
//		P1[1]=transform->IndexedArray[pos++]-MinBound[1];
//		P1[2]=transform->IndexedArray[pos++]-MinBound[2];
//
//		/*P1[0]=transform->IndexedArray[pos++];
//		P1[1]=transform->IndexedArray[pos++];
//		P1[2]=transform->IndexedArray[pos++];*/
//		posx=((int)(P1[2]*tdz)*nxny+(int)(P1[1]*tdy)*nx+(int)(P1[0]*tdx));
//		tabnr[posx]++;
//
//	}
//	
//
//	tabpos=new int*[nsize];
//	for(i=0;i<nsize;i++)
//		tabpos[i]=new int[tabnr[i]];
//
//	memset(tabnr,0,nsize*sizeof(int));
//	//pozbieram do tablic
//	pos=0;
//	index=0;
//	for(i=1;i<=nr;i++)
//	{
//		P1[0]=transform->IndexedArray[pos++]-MinBound[0];
//		P1[1]=transform->IndexedArray[pos++]-MinBound[1];
//		P1[2]=transform->IndexedArray[pos++]-MinBound[2];
//		/*P1[0]=transform->IndexedArray[pos++];
//		P1[1]=transform->IndexedArray[pos++];
//		P1[2]=transform->IndexedArray[pos++];*/
//
//		posx=((int)(P1[2]*tdz)*nxny+(int)(P1[1]*tdy)*nx+(int)(P1[0]*tdx));
//		tabpos[posx][tabnr[posx]++]=index;
//		index+=3;
//	}
//
//	// indexy pozbierane mozna uzywac
//	/////////////////////////////////
//	/////////////////////////////////
//	/////////////////////////////////
//	BSplineTransform=new BSplineGridTransformation(Grid,nnx,nny,nnz,0,0,0);//MinBound[0],MinBound[1],MinBound[2]);
//
//	//Step[0]=nnx/((Grid->GetNx()-1));
//	//Step[1]=nny/((Grid->GetNy()-1));
//	//Step[2]=nnz/((Grid->GetNz()-1));
//	Step[0]=1.0;
//	Step[1]=1.0;
//	Step[2]=1.0;
//	
//
//}
//
////--------------------------------------------------------------------
//
//SurfaceElasticRegistration::~SurfaceElasticRegistration()
//{
//	tempProperty->Delete();
//	tempProperty=NULL;
//	if (tempPolyData!=NULL)
//	{	
//		VWidget->Renderer->RemoveActor(tempActor);
//		tempPolyData->Delete();
//		tempActor->Delete();
//		tempActor=NULL;
//		tempPolyData=NULL;
//		ObjectActor->GetProperty()->SetOpacity(tempOpacity);
//	}
//
//	VWidget->Interactor->repaint();
//
//	objN=NULL;
//	objS=NULL;
//
//	if (Grid!=NULL)
//		delete Grid;
//	OrigGrid=NULL;
//
//	if (BSplineTransform!=NULL) delete BSplineTransform;
//
//
//	if (tabnr!=NULL) delete []tabnr;
//	if (tabpos!=NULL)
//	{
//
//		int i;
//		for (i=0;i<nsize;i++)
//			if (tabpos[i]!=NULL)
//				delete []tabpos[i];
//		delete []tabpos;
//	}
//
//}
//
////--------------------------------------------------------------------
//
//void SurfaceElasticRegistration::ActorUpdate()
//{
//	if ((ObjectPolyData!=NULL)&&(Grid!=NULL))
//	{
//		if (tempPolyData!=NULL)
//		{	
//			VWidget->Renderer->RemoveActor(tempActor);
//			tempPolyData->Delete();
//			tempActor->Delete();
//
//		}
//
//
//		// zrobic nowe polydata kopiujac objectpolydata i deformujac je z deka
//		tempPolyData=vtkPolyData::New();
//		tempPolyData->DeepCopy(ObjectPolyData);
//		SurfaceProcessing *sProc=new SurfaceProcessing(VWidget);
//		double min[3],nn[3];
//		sProc->CalculateMinAndSizeOfPolys(tempPolyData,objN,objS,min,nn);
//		sProc->ApplyFFDDeformation(Grid,tempPolyData,1.0,min,nn);
//		tempPolyData->Update();
//
//
//		tempActor	=	vtkActor::New();
//		vtkPolyDataMapper *tempMapper	=	vtkPolyDataMapper::New();
//		tempMapper->SetInputData(tempPolyData);
//		tempActor->SetMapper(tempMapper);
//		tempActor->SetProperty(tempProperty);
//		//tempActor->GetProperty()->SetRepresentationToWireframe ();
//		VWidget->Renderer->AddActor(tempActor);
//		delete sProc;
//		VWidget->Interactor->repaint();
//		emit	SignalGrab3Window();
//	}	
//}
//
////--------------------------------------------------------------------
//
//double SurfaceElasticRegistration::GetEnergy()
//{
//	// dla ustalonego gridu znieksztalcen
//	//	double  x1,y1,z1;
//	double P1[3],P2[3];
//	int i;
//	double Distance;
//	double DistX,DistY,DistZ;//,rx1,ry1,rz1;
//
//	double Energy=0;
//	// sprawdzoc czy znieksztalcenie jest za duze jesli tak zwrocic maxa
//	double def[4];
//
//	GetTransFromCurrentNode(def);
//	/*if  (
//		((fabs(def[1])>Step[0])||(fabs(def[2])>Step[1])||(fabs(def[3])>Step[2]))
//		||
//		(Grid->Check3DLocus(NodX,NodY,NodZ)==false)
//		)
//	{
//		return INT_MAX;
//	}
//	else*/
//	{
//		int pos=0;
//		for (i=1;i<=transform->NPoint;i++) 
//		{		
//			//transform->ApplyTransformationdouble(i,ta,x1,y1,z1,rx1,ry1,rz1);
//			P1[0]=transform->IndexedArray[pos++];
//			P1[1]=transform->IndexedArray[pos++];
//			P1[2]=transform->IndexedArray[pos++];	
//
//			BSplineTransform->TransformPoint(P1,P2);
//			distance->GetDistanceValues(P2[0],P2[1],P2[2],Distance,DistX,DistY,DistZ);
//			Energy+=Distance*Distance;
//		}
//	}
//	return Energy;
//
//
//}
////--------------------------------------------------------------------
//double SurfaceElasticRegistration::GetEnergy(int x,int y,int z,double *params)
//{
//	return GetEnergy(x,y,z);
//}
////-------------
//double SurfaceElasticRegistration::GetEnergy(int x,int y,int z)
//{
//	double def[4];
//
//
//	int i,j,k;
//	int pos;
//	double energy=0.0;
//	/*if ((NodX==0)&&(NodY==2)&&(NodZ==0))
//	{
//	i=i;
//	}*/
//	GetTransFromCurrentNode(def);
//	/*if  (
//		((fabs(def[1])>Step[0])||(fabs(def[2])>Step[1])||(fabs(def[3])>Step[2]))
//		||
//		(Grid->Check3DLocus(NodX,NodY,NodZ)==false)
//		)
//	{
//		return INT_MAX;
//	}
//	else*/
//		for (k=z-1;k<z+3;k++)
//			for (j=y-1;j<y+3;j++)
//				for (i=x-1;i<x+3;i++)
//						/*for (k=z-2;k<z+1;k++)
//			for (j=y-2;j<y+1;j++)
//				for (i=x-2;i<x+1;i++)*/
//				{
//					if ((i>=0)&&(i<nx))
//						if ((j>=0)&&(j<ny))
//							if ((k>=0)&&(k<nz))
//							{
//								pos=k*nxny+j*nx+i;
//								// no i tu bede sprawdzal pkty w kolejnym cuboidzie
//								energy+=GetEnergy(pos);
//							}
//				}
//
//				return energy;
//}
//
////-------------------------------------------------
//double SurfaceElasticRegistration::GetEnergy(int posx)
//{
//	// dla ustalonego gridu znieksztalcen
//	//	double  x1,y1,z1;
//	double P1[3],P2[3];
//	int i;//,j;
//	double Distance;
//	double DistX,DistY,DistZ;//,rx1,ry1,rz1;
//
//	double energy=0;
//	//	int pos=0;
//	int *tab;
//	int posA; 
//	//	double ex=0;
//	//	double ey=0;
//	//	double ez=0;
//	//	int ii=0;
//
//	tab=tabpos[posx];
//	for (i=0;i<tabnr[posx];i++)
//	{
//		posA=tab[i];
//		P1[0]=transform->IndexedArray[posA++];
//		P1[1]=transform->IndexedArray[posA++];
//		P1[2]=transform->IndexedArray[posA];	
//
//		BSplineTransform->TransformPoint(P1,P2);
//		P2[0]*=scaleX;
//		P2[1]*=scaleY;
//		P2[2]*=scaleZ;
//		distance->GetDistanceValues(P2[0],P2[1],P2[2],Distance,DistX,DistY,DistZ);
//		energy+=Distance*Distance;	
//
//	}
//
//
//	
//
//	tab=NULL;
//
//	return energy;
//
//}
//
////------------------------------------------------
////------------------------------------------------
//int SurfaceElasticRegistration::GetNumberOfNeighboringPoints(int x,int y,int z)
//{
//	int pos;
//	int i,j,k;
//	int pts=0;
//	for (k=z-1;k<z+3;k++)
//		for (j=y-1;j<y+3;j++)
//			for (i=x-1;i<x+3;i++)
//	/*		for (k=z-2;k<z+1;k++)
//			for (j=y-2;j<y+1;j++)
//				for (i=x-2;i<x+1;i++)*/
//			{
//				if ((i>=0)&&(i<nx))
//					if ((j>=0)&&(j<ny))
//						if ((k>=0)&&(k<nz))
//						{
//							pos=k*nxny+j*nx+i;
//							// no i tu bede sprawdzal pkty w kolejnym cuboidzie
//							pts+=tabnr[pos];
//						}
//			}
//			return pts;
//}
////-------------------------------------------------
//void SurfaceElasticRegistration::Optimize(int iter)
//{
//GetFValue=&SurfaceElasticRegistration::GetEnergy;
//	double Tolerance=10e-2;
//	int Iterations=0;
//	double FValue=0;
//	int NumberOfOptimParams=3;
//
//	if (transx!=NULL) delete []transx;
//	transx=new double[4];
//	//OptimTrans
//
//	OptimizationPowell *Optim=NULL;
//
//
//	int i,j,k,l;
//
//	FILE *Do_opa;
//	Do_opa=fopen("c:\\powell_costam.txt","w");
//	fprintf(Do_opa,"start \t%lf\n",FValue);
//	fclose(Do_opa);
//	int nr;
//
//	double temptrans[4];
//
//	double e,e1;
//	double tolX=10e-8;
//	//int iter=2;
//
//
//	//QString t="("+QString::number(nx)+"/"+QString::number(ny)+"/"+QString::number(nz)+"/"+QString::number(iter)+")";
//	QString t=QString::number(nx*ny*nz*iter);
//
//	int ii,jj,kk;
//	int pos=0;
//	for(l=0;l<iter;l++)
//
//		for (k=0;k<nz;k++)
//			for (j=0;j<ny;j++)
//				for (i=0;i<nx;i++)
//				{
//
//					if ((l%2)==0)	{	ii=i;	jj=j;	kk=k;}
//					else			{ii=nx-1-i;	jj=ny-1-j;	kk=nz-1-k;}
//
//					//	emit SendMessage("("+QString::number(i+1)+"/"+QString::number(j+1)+"/"+QString::number(k+1)+"/"+QString::number(l+1)+")/"+t);
//					emit SendMessage(initText+" ("+QString::number(++pos)+"/"+t+")");				
//					SetCurrentNode(ii,jj,kk);
//					GetTransFromCurrentNode(transx);
//
//					e=GetFunctionValue(transx);
//					temptrans[1]=transx[1];
//					temptrans[2]=transx[2];
//					temptrans[3]=transx[3];
//
//					nr=GetNumberOfNeighboringPoints(ii,jj,kk);
//					if (nr>0)
//					{
//						Optim=new OptimizationPowell(transx,NumberOfOptimParams,Tolerance,&Iterations,&FValue,this); //:) tak mi sie zrobilo 30.09.2003
//						Optim->StartOptimization();
//						delete Optim;
//						//ActorUpdate();
//
//						e1=FValue;
//
//						// jezeli stara-nowa jest w gr tolerancji to ustaw stara
//						if (fabs(e1-e)<tolX)
//						{
//							transx[1]=temptrans[1];
//							transx[2]=temptrans[2];
//							transx[3]=temptrans[3];
//
//							//	Do_opa=fopen("c:\\powellvolelas.txt","a");fprintf(Do_opa,"qq\n");fclose(Do_opa);
//							e1=e;
//						}
//
//						// ta linijka nizej jest po to aby ustawic optymalna transformacje
//						SetTransToCurrentNode(transx);
//		const char *header;
//		QString hh=fileDefaultDir+"/powell_costam.txt";
//		//		QByteArray adata=hh.totoLocal8Bit();
//		//		header =adata.constData();
//		header=hh.toLocal8Bit();
//	//	FILE *headerek; 
//			Do_opa=fopen(header,"a");
//					//	Do_opa=fopen("c:\\powell_costam.txt","a");
//						fprintf(Do_opa,"%d \t%d\t%d\t%lf\n",ii,jj,kk,FValue);	
//						fclose(Do_opa);
//					}
//
//				}
//				// zeby grid wyjsciowy zawieral wartosci 0-1
//				OrigGrid->Copy(*Grid);
//				//				OrigGrid->Scale(tdx,tdy,tdz);
//
//
//				//connect(this,SIGNAL(SignalStopOptimization()),Optim,SLOT(SlotStopOptimization())); 
//				//
//				//Optim->StartOptimization();
//				//OptimStopped=Optim->OptimizationStopped;
//
//
//
//}
////-------------------------------------------------------------------
//double	SurfaceElasticRegistration::GetFunctionValue(double *params)
//{
//
//	SetTransToCurrentNode(params);
//	//double e=GetEnergy();
//	//double e=GetEnergy(NodX,NodY,NodZ);
//double e=(this->*GetFValue)(NodX,NodY,NodZ,params);
//	FILE *Do_opa;
//	//Do_opa=fopen("c:\\powell_costam.txt","a");
//const char *header;
//		QString hh=fileDefaultDir+"/powell_costam.txt";
//		//		QByteArray adata=hh.totoLocal8Bit();
//		//		header =adata.constData();
//		header=hh.toLocal8Bit();
//		Do_opa=fopen(header,"a");
//
//	for(int i=1;i<4;i++)
//	{fprintf(Do_opa,"%lf\t",params[i]);}
//	fprintf(Do_opa,"e= %lf \n",e);
//	fclose(Do_opa);
//
//	return e;
//
//}
////-------------------------------------------------------------------
//void	SurfaceElasticRegistration::SetCurrentNode(int x,int y,int z)
//{
//	if ((x>=0) && (x<Grid->GetNx()))
//		if ((y>=0) && (y<Grid->GetNy()))
//			if ((z>=0) && (z<Grid->GetNz()))
//			{
//
//				NodX=x;
//				NodY=y;
//				NodZ=z;
//			}
//}
////-------------------------------------------------------------------
//void SurfaceElasticRegistration::GetTransFromCurrentNode(double *tr)
//{
//
//	tr[1]=Grid->GetDX(NodX,NodY,NodZ);
//	tr[2]=Grid->GetDY(NodX,NodY,NodZ);
//	tr[3]=Grid->GetDZ(NodX,NodY,NodZ);
//}
////-------------------------------------------------------------------
//void SurfaceElasticRegistration::SetTransToCurrentNode(double *tr)
//{
//	Grid->SetDx(NodX,NodY,NodZ,tr[1]);
//	Grid->SetDy(NodX,NodY,NodZ,tr[2]);
//	Grid->SetDz(NodX,NodY,NodZ,tr[3]);
//
//}
//
////--------------------------------------------------------------------
//
//void SurfaceElasticRegistration::OptimizeBig(int _iter,int _optimStep,bool _randomSampling,int multiressteps, bool _fixBounds)
//{
//	alpha=0;
//	fixBounds=_fixBounds;
//	GetFValue=&SurfaceElasticRegistration::GetTotalFunctionValue;
//	GetDerivatives=&SurfaceElasticRegistration::CalculateTotalDerivative;
//
//	optimStep=_optimStep;
//	randomSampling=_randomSampling;
//
//	int i,j,k,l,m;
//	QString t;
//	int ii,jj,kk;
//	int pos;
//	double testtol;
//
//	double Tolerance=10e-4;//10e-4;
//	double tolX=10e-4;
//	int Iterations=0;
//	double FValue=0;
//	double threshold=10.0;
//
//	int NumberOfOptimParams;
//	FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasB.txt","w");	fprintf(Do_opa,"start\n");	fclose(Do_opa);
//
//	double e,e1;
//
//	//zabezpieczenie zeby rozmiar gridu nie byl wiekszy od zbioru
//	int min=MIN3(nnx,nny,nnz);
//	int maxgrid=MAX3(nx,ny,nz);
//	int multimax=(maxgrid)*pow((double)2,multiressteps);
//
//	if (multimax>min)
//	{
//		int a;
//		multiressteps=0;
//		while((a=(maxgrid)*pow((double)2,multiressteps+1))<min) 	
//			multiressteps+=1;
//	}
//	//////////////////////////////////
//
//	//Function *f=new TempFunction();
//	OptimizationDFPowell *Optim=NULL;
//	int optimStepX=optimStep;
//	optimStep=1;
//	//	ModelDataScaled=NULL;
//
//	for(m=0;m<multiressteps;m++)
//	{
//		iterx=0;
//		fval=INT_MAX;
//
//		/*if ((ModelDataScaled!=NULL)&& (ModelDataScaled!=ModelData)) 
//		delete ModelDataScaled;
//		if (optimStepX>1)
//		ModelDataScaled=ScaleDataSet(ModelData,1.0/optimStepX);
//		else
//		ModelDataScaled=ModelData;*/
//
//		/*	BSplineTransform->SetupForwardAndBackwardScalingFactors(
//		ModelDataScaled->GetNx(),ModelDataScaled->GetNy(),ModelDataScaled->GetNz(),
//		ModelData->GetNx(),ModelData->GetNy(),ModelData->GetNz());*/
//		//		SetSizeVariables(Grid);
//
//		pos=0;
//
//		nrOfNodes=GetNodesToOptimization(Grid,transx,fixBounds,threshold,nodes);
//		//t=nrOfNodes;
//		NumberOfOptimParams=3*nrOfNodes;
//		
//		/*NumberOfOptimParams=(fixBounds)?
//		(Grid->GetNx()-2)*(Grid->GetNy()-2)*(Grid->GetNz()-2):
//		Grid->GetNx()*Grid->GetNy()*Grid->GetNz();*/
//		//nrOfNodes=NumberOfOptimParams;
//		NumberOfOptimParams*=3;
//		
//
//		nrOfParams=NumberOfOptimParams;
//		derivs=new double[NumberOfOptimParams+1];
//		if (transx!=NULL) delete []transx;
//		transx=new double[NumberOfOptimParams+1];
//		memset(transx,0,(NumberOfOptimParams+1)*sizeof(double));
//		//		t=QString::number((nx-2*st)*(ny-2*st)*(nz-2*st)*iter);
//		pos=0;
//
//		double ss=1.0;
//		//double ss=1.0;
//		smallStepX=0.1;//BSplineTransform->GetGxDivNx()*ss;
//		smallStepY=0.1;//BSplineTransform->GetGyDivNy()*ss;
//		smallStepZ=0.1;//BSplineTransform->GetGzDivNz()*ss;
//
//
//
//		/*SetParamsFromGridToArray(Grid,transx,fixBounds);
//		Optim=new OptimizationDFPowell(transx,NumberOfOptimParams,tolX,&Iterations,&FValue,this); //:) 
//		Optim->StartOptimization();
//		delete Optim;
//		*/
//		for (int tt=0;tt<1;tt++)
//		{
//			GradientDescentOptimizer(transx,NumberOfOptimParams,tolX,&Iterations,&FValue);	
//			SetParamsFromArrayToGrid(Grid,transx,fixBounds);
//			ActorUpdate();
//		
//		}
//		FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasB.txt","a");	fprintf(Do_opa,"iter=%d\n",iterx);	fclose(Do_opa);
//		if (multiressteps-m>1)
//		{
//			//przebudowac
//			UpdateData(nx*2-1,ny*2-1,nz*2-1);
//			delete TempGrid;
//			TempGrid=new FloatGrid3D();
//			TempGrid->Copy(*Grid);
//			if (optimStepX>=2) optimStepX/=2;
//		}
//		delete []derivs;derivs=NULL;
//
//		delete []transx;transx=NULL;
//		/*if (nodes!=NULL) 
//		{		
//		delete []nodes;
//		nodes=NULL;
//		}*/
//	}
//	/*if ((ModelDataScaled!=NULL)&& (ModelDataScaled!=ModelData)) 
//	delete ModelDataScaled;*/
//	OrigGrid->Copy(*Grid);
//	stopOptimization=false;
//}
////------------------------------------
//double 	SurfaceElasticRegistration::GetTotalFunctionValue(int a,int b,int c,double *Params)
//{
//	SetParamsFromArrayToGrid(Grid,Params,fixBounds);
//	int i,j,k;
//	double energy[2];energy[0]=0;energy[1]=0;
//	int count=0;
//
//	double P1[3],P2[3];
//
//	double Distance;
//	double DistX,DistY,DistZ;//,rx1,ry1,rz1;
//
//	/*for(k=0;k<nz1;k+=optimStep)
//	for(j=0;j<ny1;j+=optimStep)
//	for(i=0;i<nx1;i+=optimStep)	
//	AddNewEnergyPointWithoutDerivs(i,j,k,count,energy);*/
//
//
//	//	double ex=0;
//	//	double ey=0;
//	//	double ez=0;
//	//	int ii=0;
//
//
//	int nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=NodZ;
//	double ddDdPL[3];
//	int pos=0;
//	for (i=1;i<=transform->NPoint;i++) 
//	{
//		if (i==transform->NPoint)
//		{
//			i=i;
//		}
//		P1[0]=transform->IndexedArray[pos++];
//		P1[1]=transform->IndexedArray[pos++];
//		P1[2]=transform->IndexedArray[pos++];	
//
//		BSplineTransform->TransformPoint(P1,P2,nodPos,ddDdPL);
//		distance->GetDistanceValues(P2[0],P2[1],P2[2],Distance,DistX,DistY,DistZ);
//		energy[0]+=Distance*Distance;	
//		energy[1]+=(ddDdPL[0]+ddDdPL[1]+ddDdPL[2]);	
//		count++;
//
//	}
//
//
//
//	double f=energy[0]+alpha*energy[1];
//	//if (count >0)	if (f!=0) f/=(double)count;
//	f= (count >0)?f/=(double)count:INT_MAX;
//	if (f<fval)
//	{
//		fval=f;
//		FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasB.txt","a");	fprintf(Do_opa,"%lf-min\n",f);	fclose(Do_opa);
//	}
//	else
//	{	FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasB.txt","a");	fprintf(Do_opa,"%lf\n",f);	fclose(Do_opa);}
//
//	emit SendMessage(initText+" ("+QString::number(iterx)+"="+QString::number(f)+") ");
//
//	return (double)f;
//
//}
////---------------------------------------
////----------------0000000000000000000============
//void	SurfaceElasticRegistration::CalculateTotalDerivative(double *Deriv,double *Params)
//{
//	//double mm=(maxP+minP)/(double)optimStep;
//	maxNrOfSamples=	transform->NPoint;
//
//
//	double *drvTempTab=new double [maxNrOfSamples];
//	FloatGrid3D *g1=new FloatGrid3D();
//	FloatGrid3D *g2=new FloatGrid3D();
//	g1->Copy(*Grid);
//	g2->Copy(*Grid);
//	BSplineGridTransformation *bTr1=new BSplineGridTransformation(g1,nnx,nny,nnz);
//	BSplineGridTransformation *bTr2=new BSplineGridTransformation(g2,nnx,nny,nnz);
//	bTr1->SetupForwardAndBackwardScalingFactors(nx1,ny1,nz1,mx1,my1,mz1);
//	bTr2->SetupForwardAndBackwardScalingFactors(nx1,ny1,nz1,mx1,my1,mz1);
//
//	QTime t;
//	t.start();
//	SetParamsFromArrayToGrid(Grid,Params,fixBounds);
//	//jade po kolejnych nodach wyliczam dla nich pochodne i wrzucam do Deriv[pos++]
//	int pos=1;
//	double der[6];
//	int st=(fixBounds==true)?1:0;
//	int count=0;
//	double energy[2];energy[0]=0;energy[1]=0;
//
//	double cn;
//	int i,j,k,ii,jj,kk;
//	int l;
//	//for (k=st;k<nz-st;k++)
//	//	for (j=st;j<ny-st;j++)
//	//		for (i=st;i<nx-st;i++)
//	for (l=0;l<nrOfNodes;l++)
//	{
//		i=nodes[l].x;j=nodes[l].y;k=nodes[l].z;
//
//		der[0]=0;	der[1]=0;	der[2]=0; der[3]=0;	der[4]=0;	der[5]=0;
//		energy[0]=0;	energy[1]=0; count=0;
//		CalculateDerivativesAtPoint(i,j,k,der,drvTempTab,count, bTr1,bTr2);
//		//---------------------------------------
//
//		cn=(count>0)?1.0/count:1.0;
//
//		Deriv[pos++]=cn*(der[0]+alpha*der[3]);
//		Deriv[pos++]=cn*(der[1]+alpha*der[4]);
//		Deriv[pos++]=cn*(der[2]+alpha*der[5]);
//
//	}
//
//	FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasB.txt","a");	fprintf(Do_opa,"der=%d\n",t.elapsed());	fclose(Do_opa);
//	emit SendMessage(initText+" ("+QString::number(iterx)+"="+"DERIV"+") ");
//	delete bTr1;
//	delete bTr2;
//	delete g1;
//	delete g2;
//	delete []drvTempTab;
//
//
//}
////-------------------------------------
//void SurfaceElasticRegistration::SetParamsFromGridToArray(FloatGrid3D *Grid,double *Params, bool fixbounds)
//{
//	int pos=1;
//	double transx[4];
//	int i;
//
//	if ((nodes!=NULL)&&(nrOfNodes>0))
//	{
//		for (i=0;i<nrOfNodes;i++)
//		{
//			SetCurrentNode(nodes[i].x,nodes[i].y,nodes[i].z);
//			GetTransFromCurrentNode(transx,Grid);
//			Params[pos++]=transx[1];Params[pos++]=transx[2];Params[pos++]=transx[3];
//		}
//	}
//	else
//	{
//		int j,k;
//		int st=(fixbounds==true)?1:0;
//		for (k=st;k<nz-st;k++)
//			for (j=st;j<ny-st;j++)
//				for (i=st;i<nx-st;i++)
//				{
//					SetCurrentNode(i,j,k);
//					GetTransFromCurrentNode(transx,Grid);
//					Params[pos++]=transx[1];Params[pos++]=transx[2];Params[pos++]=transx[3];
//				}
//	}
//
//}
////---------------------------------------
//void SurfaceElasticRegistration::SetParamsFromArrayToGrid(FloatGrid3D *Grid,double *Params, bool fixbounds)
//{
//	int pos=1;
//	double transx[4];
//	int i;
//
//	if ((nodes!=NULL)&&(nrOfNodes>0))
//	{
//		for (i=0;i<nrOfNodes;i++)
//		{
//			transx[1]=Params[pos++];transx[2]=Params[pos++];transx[3]=Params[pos++];
//			SetCurrentNode(nodes[i].x,nodes[i].y,nodes[i].z);
//			SetTransToCurrentNode(transx,Grid);
//		}
//	}
//	/*else
//	{
//		int j,k;
//		int st=(fixbounds==true)?1:0;
//		for (k=st;k<nz-st;k++)
//			for (j=st;j<ny-st;j++)
//				for (i=st;i<nx-st;i++)
//				{
//					transx[1]=Params[pos++];transx[2]=Params[pos++];transx[3]=Params[pos++];
//					SetCurrentNode(i,j,k);
//					SetTransToCurrentNode(transx,Grid);
//				}
//	}*/
//
//
//}
////-----------------
////----------------------------------
////double  SurfaceElasticRegistration::GetGradI(int i,int j,int k,double smallStep,double fvalue,int view)
////{
//	////fkcja liczy gradient w zadanym kierunku osi glownych - view
//	////dla danego pktu zbioru danych Model Data i danego noda
//	////smallStep jest to dx=dy,dz dla liczenia gradientu
//	//double plus, minus,value=0;
//	//double Point[3];
//	//double x,y,z;
//	//switch (view)
//	//{
//	//case 0:		x=smallStep;y=0;z=0;	break;
//	//case 1:		x=0;y=smallStep;z=0;	break;
//	//case 2:		x=0;y=0;z=smallStep;	break;
//	//}
//
//	//BSplineTransform->TransformPont(i,j,k,Point,x,y,z,NodX,NodY,NodZ);
//	//if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&
//	//	(Point[1]>=0)&&(Point[1]<my1-1)&&
//	//	(Point[2]>=0)&&(Point[2]<mz1-1)))
//	//	return 0;
//	//plus=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2])*maxFactor;//-fvalue;
//
//	//BSplineTransform->TransformPont(i,j,k,Point,-x,-y,-z,NodX,NodY,NodZ);
//	//if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&
//	//	(Point[1]>=0)&&(Point[1]<my1-1)&&
//	//	(Point[2]>=0)&&(Point[2]<mz1-1)))
//	//	return 0;
//	//minus=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2])*maxFactor;//-fvalue;
//
//	//value=0.5*(plus-minus);
//
//	//return value;
//
////};
////------------------
//double  SurfaceElasticRegistration::GetGradI(int posx,BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf)
//{
//	double plus, minus,value;
//	double Point[3];
//	double pa[3];
//	double Distance;
//	double DistX,DistY,DistZ;//,rx1,ry1,rz1;
//	//pa[0]=i;pa[1]=j;pa[2]=k;
//	//posx*=3;
//	pa[0]=transform->IndexedArray[posx++];
//	pa[1]=transform->IndexedArray[posx++];
//	pa[2]=transform->IndexedArray[posx];	
//
//	plusTransf->TransformPoint(pa,Point);
//	distance->GetDistanceValues(Point[0],Point[1],Point[2],Distance,DistX,DistY,DistZ);
//	plus=Distance;
//
//	minusTransf->TransformPoint(pa,Point);
//	distance->GetDistanceValues(Point[0],Point[1],Point[2],Distance,DistX,DistY,DistZ);
//	minus=Distance;
//	value=0.5*(plus-minus);
//	return value;
//
//	/*plusTransf->TransformPoint(i,j,k,Point);
//	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&		(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))) return 0;
//	plus=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2]);
//	minusTransf->TransformPoint(i,j,k,Point);
//	if (!((Point[0]>=0)&&(Point[0]<mx1-1)&&		(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1)))	return 0;
//	minus=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2]);
//	value=0.5*(plus-minus);
//	return value;*/
//
//};
////-------------------------------------
//void SurfaceElasticRegistration::CalculateDerivativesAtPoint(int x,int y,int z,double *der,double *drvTempTab,int &count, BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf)
//{
//	int i,j,k,ii;
//	double *ptr=drvTempTab;
//	SetCurrentNode(x,y,z);
//
//	if (der!=NULL)
//	{
//		int pos=0;
//		int *tab;
//		int posA; 
//		int st=0;
//	for (k=st;k<nz-st;k++)
//		for (j=st;j<ny-st;j++)
//			for (i=st;i<nx-st;i++)
//
//	//	for (k=z-1;k<z+3;k++)
////			for (j=y-1;j<y+3;j++)
////				for (i=x-1;i<x+3;i++)
//		/*for (k=z-2;k<z+1;k++)
//			for (j=y-2;j<y+1;j++)
//				for (i=x-2;i<x+1;i++)*/
//
//				{
//					if ((i>=0)&&(i<nx))
//						if ((j>=0)&&(j<ny))
//							if ((k>=0)&&(k<nz))
//							{
//								pos=k*nxny+j*nx+i;
//								tab=tabpos[pos];
//
//								for (ii=0;ii<tabnr[pos];ii++)
//								{
//									posA=tab[ii];
//									AddGradPart1(posA,count,der,drvTempTab[pos++]);
//								}
//							}
//
//
//							tab=NULL;
//							//	tab2=NULL;
//				}
//							AddGradPart2(der,drvTempTab,smallStepX,0,plusTransf,minusTransf);
//							AddGradPart2(der,drvTempTab,smallStepY,1,plusTransf,minusTransf);
//							AddGradPart2(der,drvTempTab,smallStepZ,2,plusTransf,minusTransf);
//				
//
//	}
//}
////---------------------------------------------------
//void SurfaceElasticRegistration::AddGradPart2(double *der,double *drvTempTab,double smallStep,int view,BSplineGridTransformation *plusTransf,BSplineGridTransformation *minusTransf)
//{
//
//	FloatGrid3D *plusGrid	=	plusTransf->GetGrid();
//	FloatGrid3D *minusGrid	=	minusTransf->GetGrid();
//	int ii,jj,kk;
//	int pos=Grid->GetPos(NodX,NodY,NodZ);
//	double val;//bo plus i minus maja byc takie same na starcie
//	//int v=view+1;
//	switch (view)
//	{
//	case 0:	val=plusGrid->GetDXNP(pos);	plusGrid->SetDx(pos,val+smallStep);	minusGrid->SetDx(pos,val-smallStep);	break;
//	case 1:	val=plusGrid->GetDYNP(pos);	plusGrid->SetDy(pos,val+smallStep);	minusGrid->SetDy(pos,val-smallStep);	break;
//	case 2:	val=plusGrid->GetDZNP(pos);	plusGrid->SetDz(pos,val+smallStep);	minusGrid->SetDz(pos,val-smallStep);	break;
//
//	}
//
//	//	for(kk=posZstart;kk<posZstop;kk+=optimStep)
//	///		for(jj=posYstart;jj<posYstop;jj+=optimStep)
//
//	//			for(ii=posXstart;ii<posXstop;ii+=optimStep)
//
//	int i,j,k;
//	int *tab;
//	int posA; 
//int x,y,z;
//
//x=NodX;
//y=NodY;
//z=NodZ;
//int st=0;
//	for (k=st;k<nz-st;k++)
//		for (j=st;j<ny-st;j++)
//			for (i=st;i<nx-st;i++)
//
//	//for (k=z-1;k<z+3;k++)
//	//	for (j=y-1;j<y+3;j++)
//	//		for (i=x-1;i<x+3;i++)
//	
//	/*for (k=z-2;k<z+1;k++)
//			for (j=y-2;j<y+1;j++)
//				for (i=x-2;i<x+1;i++)*/
//			{
//				if ((i>=0)&&(i<nx))
//					if ((j>=0)&&(j<ny))
//						if ((k>=0)&&(k<nz))
//						{
//							pos=k*nxny+j*nx+i;
//							tab=tabpos[pos];
//
//							for (ii=0;ii<tabnr[pos];ii++)
//							{
//								posA=tab[ii];
//								if (*drvTempTab!=0)
//									der[view]+=*(drvTempTab)*GetGradI(posA,plusTransf,minusTransf);
//								drvTempTab++;
//							}
//						}
//
//
//						tab=NULL;
//
//
//
//						switch (view)
//						{
//						case 0:	plusGrid->SetDx(pos,val);	minusGrid->SetDx(pos,val);	break;
//						case 1:	plusGrid->SetDy(pos,val);	minusGrid->SetDy(pos,val);	break;
//						case 2:	plusGrid->SetDz(pos,val);	minusGrid->SetDz(pos,val);	break;
//						}
//
//			}
//}
////---------------------------------------
//void SurfaceElasticRegistration::AddGradPart1(int posx, int &count,double *drv,double &drvel)
//{
//	double pa[3];
//	//pa[0]=i;pa[1]=j;pa[2]=k;
//	//posx*=3;
//	pa[0]=transform->IndexedArray[posx++];
//	pa[1]=transform->IndexedArray[posx++];
//	pa[2]=transform->IndexedArray[posx];	
//
//	int nodPos[3];nodPos[0]=NodX;nodPos[1]=NodY;nodPos[2]=NodZ;
//	double ddDdP[9];
//	double ddDdPL[3];
//	double ddDdQ;
//	double ddDPdQ[3];
//	double Point[3];
//	double P1[3],P2[3];
//	int i;//,j;
//	double Distance;
//	double DistX,DistY,DistZ;//,rx1,ry1,rz1;
//
//	double energy=0;
//	//	int pos=0;
//	//	int *tab;
//	//	int posA; 
//	//tab=tabpos[posx];
//
//	BSplineTransform->TransformPoint(pa,P2,true,nodPos,ddDdP,ddDdPL,&ddDdQ,ddDPdQ);
//
//	double m=0;
//	//if ((Point[0]>=0)&&(Point[0]<mx1-1)&&	(Point[1]>=0)&&(Point[1]<my1-1)&&	(Point[2]>=0)&&(Point[2]<mz1-1))
//	//{	
//	//m=Interpolation3LinearDouble(ObjectData,Point[0],Point[1],Point[2]);
//	//double a=ModelDataScaled->GetVoxelValue(i,j,k)-m;
//	distance->GetDistanceValues(P2[0],P2[1],P2[2],Distance,DistX,DistY,DistZ);
//	drvel=Distance*ddDdQ;
//
//	drv[3]+=(ddDdP[0]*ddDPdQ[0]+ddDdP[3]*ddDPdQ[1]+ddDdP[6]*ddDPdQ[2]);
//	drv[4]+=(ddDdP[1]*ddDPdQ[0]+ddDdP[4]*ddDPdQ[1]+ddDdP[7]*ddDPdQ[2]);
//	drv[5]+=(ddDdP[2]*ddDPdQ[0]+ddDdP[5]*ddDPdQ[1]+ddDdP[8]*ddDPdQ[2]);	
//	count++;
//	//}
//}
////----------------------------------
//
//void SurfaceElasticRegistration::UpdateData(int gridNx, int gridNy, int gridNz)
//{
//
//	FloatGrid3D *tempGrid=new FloatGrid3D(gridNx,gridNy,gridNz);
//	BSplineTransform->ScaleGrid(tempGrid);
//
//	delete BSplineTransform;
//	//SetSizeVariables(tempGrid);
//	BSplineTransform=new BSplineGridTransformation(tempGrid,nnx,nny,nnz);
//
//	delete Grid;
//	Grid=tempGrid;
//
//
//
//}
////-----------
//void SurfaceElasticRegistration::GetTransFromCurrentNode(double *tr, FloatGrid3D *grid)
//{
//	tr[1]=grid->GetDX(NodX,NodY,NodZ);
//	tr[2]=grid->GetDY(NodX,NodY,NodZ);
//	tr[3]=grid->GetDZ(NodX,NodY,NodZ);
//}
////-------------------------------------------------------------------
//void SurfaceElasticRegistration::SetTransToCurrentNode(double *tr, FloatGrid3D *grid)
//{
//	grid->SetDx(NodX,NodY,NodZ,tr[1]);
//	grid->SetDy(NodX,NodY,NodZ,tr[2]);
//	grid->SetDz(NodX,NodY,NodZ,tr[3]);
//}
////----------------------------------------------------------------
//void	SurfaceElasticRegistration::GradientDescentOptimizer(double *params0,int nrOfParams,double tol,int *iter,double *fval)
//{
///*	if (params0!=NULL)
//	{
//
//		int itmax=100;
//		double *der=new double [nrOfParams+1];
//		double *params1=new double [nrOfParams+1];
//		double e0,e1,tmpE;
//		int i,its;
//		double lambda=1.0;
//		double sLambda;
//		double fLambda=15;
//		double tmpLambda;
//		double nrmSq;
//		double dotPr;
//		double tmp1,tmp2;
//
//		e0=GetFunctionValue(params0);
//		CalculateDerivative(der,params0);
//
//		for(i=1;i<=nrOfParams;i++)
//			params1[i]=params0[i]-lambda*der[i];
//
//		e1=GetFunctionValue(params1);
//
//		if (e1>e0)
//		{
//			lambda/=fLambda;
//			for(i=1;i<=nrOfParams;i++)
//				params1[i]=params0[i]-lambda*der[i];
//			e1=GetFunctionValue(params1);
//		}
//
//		if (fabs(e1-e0)<tol)
//		{
//			for(i=1;i<=nrOfParams;i++)
//				params0[i]=params1[i];
//			*fval=e1;
//			delete []params1;delete []der; *fval=e1; return ;
//		}
//
//		for (its=0;its<itmax;its++) 
//		{
//			CalculateDerivative(der,params1);
//
//			tmpE=e0-e1;
//			nrmSq=0;
//			dotPr=0;
//			for(i=1;i<=nrOfParams;i++)
//			{
//				//--- sLambda -----------------------
//				tmp1=params1[i]-params0[i];
//				nrmSq+=tmp1*tmp1;
//				dotPr+=tmp1*der[i];
//			}
//			tmp2=tmpE+dotPr;	
//			sLambda=(tmp2!=0)?0.5*(nrmSq/tmp2):sLambda=0;
//
//			for(i=1;i<=nrOfParams;i++)
//				params0[i]=params1[i]-sLambda*der[i];
//
//
//			e0=e1;
//			e1=GetFunctionValue(params0);
//
//			lambda=sLambda;
//			if (e1>e0)
//			{	//???????????
//				lambda/=fLambda;
//				for(i=1;i<=nrOfParams;i++)
//					params1[i]=params0[i]-lambda*der[i];
//				e1=GetFunctionValue(params1);
//			}
//			//else
//			//	lambda=sLambda;
//
//			if (fabs(e1-e0)<tol)
//			{
//				for(i=1;i<=nrOfParams;i++)
//					params0[i]=params1[i];
//				delete []params1;delete []der; *fval=e1; return ;
//			}
//			//-----------------------------------
//		}
//
//		for(i=1;i<=nrOfParams;i++)
//			params0[i]=params1[i];
//		delete []params1;delete []der; *fval=e1; return ;
//
//	}
//	return ;
//*/
//
//		if (params0!=NULL)
//	{
//
//		FILE *Do_opa;
//		int it=10;
//		double *der=new double [nrOfParams+1];
//		double *params1=new double [nrOfParams+1];
//		double e0,e1,tmpE;
//		int i,its;
//		double lambda=0.5;
//		double sLambda;
//		double fLambda=15;
//		double tmpLambda;
//		double nrmSq;
//		double dotPr;
//		double tmp1,tmp2;
//		double epsilon=1e+6;
//	
//
//
//		its=0;
//
//		// taka wartosc fkcji jest na starcie
//		e0=GetFunctionValue(params0);
//		CalculateDerivative(der,params0);
//
//		//ustal nowe parametry i wartosc fkcji
//		
//		for(i=1;i<=nrOfParams;i++)
//			params1[i]=params0[i]-lambda*der[i];
//
//		//ActorUpdate();
//	/*	e1=GetFunctionValue(params1);
//		//-------------------------------------
//		Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"start %lf nowa %lf\n",e0,e1);fclose(Do_opa);	
//
//		// jezeli nowa wartosc fkcji jest gorsza
//		if (e1>e0)
//		{
//			do
//			{
//				it--;
//				//zmniejsz lambde i oblicz nowa wartosc fkcji
//				lambda/=2.0;
//				for(i=1;i<=nrOfParams;i++)
//					params1[i]=params0[i]-lambda*der[i];
//				e1=GetFunctionValue(params1);
//				Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"gorzej - po zmniejszeniu %lf,%d\n",e1,it);fclose(Do_opa);	
//				if (e1<e0+epsilon) break;
//			// rob to do czasu az wartosc fkcji bedzie lepsza lub iteracje Ci sie skoncza
//			}while (it>0);
//		// a jesli jest lepsza
//		}
//		else
//		{
//			//zwiekszaj krok lambda do czasu az e1 nie bedzi lepsze od poprzedniego
//			do
//			{
//				Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"lepiej - zwieksz %lf %d\n",e1,it);fclose(Do_opa);	
//
//				e0=e1;
//				it--;
//				lambda*=2.0;
//
//				for(i=1;i<=nrOfParams;i++)
//					params1[i]=params0[i]-lambda*der[i];
//				e1=GetFunctionValue(params1);
//
//				Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"lepiej - nowy test %lf %d\n",e1,it);fclose(Do_opa);	
//
//			}while ((e1<e0)&&(it>0));
//
//			if (it>0)
//			{
//				lambda/=2.0;
//				for(i=1;i<=nrOfParams;i++)
//					params1[i]=params0[i]-lambda*der[i];
//			}
//		}
//		*/
//	//	if (it>0)
//	//	{
//			//Do_opa=fopen("c:\\opter.txt","a");fprintf(Do_opa,"ustalam nowe parametry \n");fclose(Do_opa);	
//
//			for(i=1;i<=nrOfParams;i++)
//				params0[i]=params1[i];	
//	//	}
//
//		delete []der;
//		delete []params1;
//
//	}
//
//}
//void	SurfaceElasticRegistration::CalculateDerivative(double *Deriv,double *Params)
//{
//	if ((Deriv!=NULL)&&(Params!=NULL))
//	{
//		(this->*GetDerivatives)(Deriv,Params);
//		//CalculateTotalDerivative(Deriv,Params);
//		//CalculateSingleNodDerivative(Deriv,Params);
//		//		emit SendMessage(initText+" ("+QString::number(iterx)+"="+"DERIV"+") ");
//				FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasB.txt","a");	fprintf(Do_opa,"Deriv [%lf %lf %lf]\n", Deriv[1],Deriv[2],Deriv[3]);	fclose(Do_opa);
//	}
//
//}
//
//
//
//int SurfaceElasticRegistration::GetNodesToOptimization(FloatGrid3D *Grid,double *Params, bool fixbounds,double threshold,Node *&Nodes)
//{
//
//	int NrOfNodes=(fixbounds)?
//		(Grid->GetNx()-2)*(Grid->GetNy()-2)*(Grid->GetNz()-2):
//	Grid->GetNx()*Grid->GetNy()*Grid->GetNz();
//	//double *mEnergyTab=new double[NrOfNodes];
//
//	double energy[2];energy[0]=0;energy[1]=0;
//	int pos=0;
//	int i,j,k;
//	int ii,jj,kk;
//	double f;
//	int Nr=0;
//	int st=(fixbounds==true)?1:0;
//
//	Node *tmpNodes=new Node[NrOfNodes];
//
//	for (k=st;k<nz-st;k++)
//		for (j=st;j<ny-st;j++)
//			for (i=st;i<nx-st;i++)
//			{
//				energy[0]=0;	energy[1]=0; //count=0;
//				SetCurrentNode(i,j,k);
//
//				//for(kk=posZstart;kk<posZstop;kk+=optimStep)
//				//	for(jj=posYstart;jj<posYstop;jj+=optimStep)
//				//		for(ii=posXstart;ii<posXstop;ii+=optimStep)
//				//			AddNewEnergyPointWithoutDerivs(ii,jj,kk,count,energy);
//
//				//f=energy[0]+alpha*energy[1];
//				////if (count >0)	if (f!=0) f/=(double)count;
//				//f= (count >0)?f/=(double)count:INT_MAX;
//				////mEnergyTab[pos++]=f;
//				//if (f>threshold) 
//				//{
//					tmpNodes[Nr].x=i;tmpNodes[Nr].y=j;tmpNodes[Nr].z=k;
//					Nr++;
//				//}
//			}
//
//			//przepisac do wlasciwej	
//			if (Nodes!=NULL) delete []Nodes;
//			Nodes=new Node[Nr];
//			for (i=0;i<Nr;i++)
//				Nodes[i]=tmpNodes[i];
//			delete []tmpNodes;
//
//			FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasC.txt","w");	fprintf(Do_opa,"Nodes: %d,%d",NrOfNodes,Nr);
//			//sort
//			//QSortDouble(mEnergyTab,0,NrOfNodes-1);
//			/*FILE *Do_opa;	Do_opa=fopen("c:\\powellvolelasC.txt","a");	fprintf(Do_opa,"Nodes: %d\n",NrOfNodes);	
//
//			for (i=0;i<NrOfNodes;i++)
//			fprintf(Do_opa,"%d %lf\n",i,mEnergyTab[NrOfNodes-i-1]);	
//
//			fclose(Do_opa);*/
//
//
//
//			//delete []mEnergyTab;
//
//			return Nr;
//}
////----------------------------------------------------------------
