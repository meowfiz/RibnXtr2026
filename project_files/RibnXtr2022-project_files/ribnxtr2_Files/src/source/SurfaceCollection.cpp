#include <QTextStream>
#include "SurfaceCollection.h"


PolygonSurfaceData::PolygonSurfaceData(vtkPolyData *NewPolyData,VtkWidget *_VWidget)
{
	savedTimeStamp=0;
	mapperTimeStamp=0;
	textureName="";
	NumberOfCuttingPlanes=-1;
	treeItem=NULL;
	groupName="";
	tmpGroupName = "";
	tmpData=false;
	isSegCutted=false;
	isSegTemp=false;
	roleIndex=0;
	PolygonSurfacePath="";
	childrenPolygonSurfaceDataList=new LinkedList <PolygonSurfaceData*>;
	parentPolygonSurfaceData=NULL;

	colorLoaded=false;
	repaintWithoutReset=false;
	//SurfaceActive=false;
	int i;

	for(i=0;i<3;i++)
	{
		PN[i]=0;
		PScale[i]=0;
	}

	VPolyData=NewPolyData;


	VMapper=vtkPolyDataMapper::New();
	VMapper->ScalarVisibilityOff();
	VMapper->SetInputData(VPolyData);
	VActor=vtkActor::New();
	VActor->SetMapper(VMapper);
	VProperty=VActor->GetProperty();
	
	
	tempProperty=vtkSmartPointer <vtkProperty>::New();
	selectProperty=vtkSmartPointer <vtkProperty>::New();


	selectProperty->SetDiffuseColor(1.0,0,0);
	selectProperty->SetSpecular(0.5);
	selectProperty->SetSpecularPower(128);
	selectProperty->SetDiffuse(1.0);
	selectProperty->SetAmbient(0.8);
	selectProperty->SetColor(38/255.0, 166/255.0, 235/255.0);
	selectProperty->SetAmbientColor(0.4,0.4,0.4);	
	selectProperty->SetSpecularColor(0.25,0.25,0.25);

	

	VWidget=_VWidget;
	VRenderer=NULL;

	Visible=false;
	HideWithoutRefresh=false;

	CuttingEdgePolyData=NULL;
	CuttingEdgeMapper=NULL;
	CuttingEdgeActor=NULL;

	Matrix=vtkMatrix4x4::New();
	VActor->SetUserMatrix(Matrix);
	isTempPropertyOn=false;

	defaultPropertyFilename="";
	progDirPath="";
	if (VWidget!=NULL) 
	{
		VRenderer = VWidget->Renderer;
		defaultPropertyFilename=VWidget->GetDefaultPropertyFileName();
		progDirPath=VWidget->GetProgDirPath();
	}


	
	SetDefaultProperty();

}

//-----------------------------------------
PolygonSurfaceData::PolygonSurfaceData(VtkWidget *_VWidget)
{
	savedTimeStamp=0;
	mapperTimeStamp=0;
	textureName="";
	textureRepeat=true;
	NumberOfCuttingPlanes=-1;
	treeItem=NULL;
	groupName="";
	tmpGroupName = "";
	tmpData=false;
	isSegCutted=false;
	isSegTemp=false;
	roleIndex=0;
	PolygonSurfacePath="";
	childrenPolygonSurfaceDataList=new LinkedList <PolygonSurfaceData*>;
	parentPolygonSurfaceData=NULL;

	colorLoaded=false;
	int i;

	for(i=0;i<3;i++)
	{
		PN[i]=0;
		PScale[i]=0;
	}

	VActor=vtkActor::New();
	VProperty=VActor->GetProperty();



	VWidget=_VWidget;
	VRenderer=NULL;
	VPolyData=NULL;
	VMapper=NULL;
	Visible=false;
	HideWithoutRefresh=false;

	CuttingEdgePolyData=NULL;
	CuttingEdgeMapper=NULL;
	CuttingEdgeActor=NULL;

	Matrix=vtkMatrix4x4::New();
	VActor->SetUserMatrix(Matrix);
	
	tempProperty=vtkSmartPointer <vtkProperty>::New();
	selectProperty=vtkSmartPointer <vtkProperty>::New();


	selectProperty->SetDiffuseColor(1.0,0,0);
	selectProperty->SetSpecular(0.5);
	selectProperty->SetSpecularPower(128);
	selectProperty->SetDiffuse(1.0);
	selectProperty->SetAmbient(0.8);
	selectProperty->SetColor(38/255.0, 166/255.0, 235/255.0);
	selectProperty->SetAmbientColor(0.4,0.4,0.4);	
	selectProperty->SetSpecularColor(0.25,0.25,0.25);
	//selectProperty->SetMetallic(1);
	isTempPropertyOn=false;



	defaultPropertyFilename="";
	if (VWidget!=NULL) 
	{
		VRenderer = VWidget->Renderer;
		defaultPropertyFilename=VWidget->GetDefaultPropertyFileName();
		progDirPath=VWidget->GetProgDirPath();
	}
}

//-----------------------------------------
PolygonSurfaceData::PolygonSurfaceData()
{
	savedTimeStamp=0;
	mapperTimeStamp=0;
	textureName="";
	textureRepeat=true;
	defaultPropertyFilename="";
	NumberOfCuttingPlanes=-1;
	treeItem=NULL;
	groupName="";
	tmpGroupName = "";
	roleIndex=0;
	PolygonSurfacePath="";
	VMapper=NULL;

	VActor=NULL;
	VPolyData=NULL;
	VProperty=NULL;

	

	CuttingEdgePolyData=NULL;
	CuttingEdgeMapper=NULL;
	CuttingEdgeActor=NULL;
	childrenPolygonSurfaceDataList=NULL;

	defaultPropertyFilename="";
}
//--------------------------------------
PolygonSurfaceData::~PolygonSurfaceData()
{
	if (treeItem!=NULL)
		delete treeItem; treeItem = NULL;
	if(HideWithoutRefresh==false)
		HideActor();
	else
		HideActorWithoutRefresh();

	///	ofstream myFile("c:/out.txt");



	//	myFile << "po hideact : " << endl<< endl<< *VPolyData << endl;  


	if (VActor!=NULL)				VActor->Delete();

	if (VMapper!=NULL)				VMapper->Delete();
	//	myFile << "maper del " << endl<< endl<< *VPolyData << endl;  


	//	myFile << "actor del " << endl<< endl<< *VPolyData << endl;  

	if (VPolyData!=NULL)			VPolyData->Delete();


	//	myFile << "polydatadel: "<< endl<< endl << *VPolyData << endl;  

	if (CuttingEdgeMapper!=NULL)	CuttingEdgeMapper->Delete();
	if (CuttingEdgeActor!=NULL)		CuttingEdgeActor->Delete();
	if (CuttingEdgePolyData!=NULL)	CuttingEdgePolyData->Delete();

	
	VMapper=NULL;

	VActor=NULL;
	VPolyData=NULL;
	VProperty=NULL;



	CuttingEdgePolyData=NULL;
	CuttingEdgeMapper=NULL;
	CuttingEdgeActor=NULL;


	int i;
	if (childrenPolygonSurfaceDataList!=NULL)
	{
		for(i=0;i<childrenPolygonSurfaceDataList->GetNumberOfItems();i++)
			{
				childrenPolygonSurfaceDataList->GetItemAtPos(i)->SetData(new PolygonSurfaceData());
				//pList->GetItemAtPos(i)->SetData(NULL);
				
			}
		delete childrenPolygonSurfaceDataList;
		}
	parentPolygonSurfaceData=NULL;

}




//-----------------------------------------

//void	PolygonSurfaceData::SetSurfaceActive(bool val)
//{
//
//	SurfaceActive=val;
//}
////-----------------------------------------

bool PolygonSurfaceData::LoadPolygonSurfaceDataSet2( const QString &filename)
{



	if(!filename.isEmpty())
	{
		int r,g,b,alpha;
		int i;
		int representation=2;
		int interpolation=0;
		QString filename1,filename2;	
		FILE *plik;
		int count;

		filename2=filename;
		if (filename2.lastIndexOf(".sdata")==-1)
			filename2+=".sdata";
		
		filename1=filename2+".header";
		
		QFile file(filename1);
	
		if ( file.open( QIODevice::ReadOnly ) ) 
		{
			QTextStream stream( &file );

			QString line;//=stream.readLine();
			QStringList fields;// = line.split( " ");

			do 
			{
				line=stream.readLine();
				if (QString::compare(line,QString("SIZES:"))==0)
				{
					line=stream.readLine();
					fields=line.split(" ", QString::SkipEmptyParts);
					count=fields.count();
					PN[0]=fields[0].toInt();
					PN[1]=fields[1].toInt();
					PN[2]=fields[2].toInt();

					

				}
				else
					if (QString::compare(line,QString("VOXEL_DIMS:"))==0)
				{
					line=stream.readLine();
					fields=line.split(" ", QString::SkipEmptyParts);
					PScale[0]=fields[0].toDouble();
					PScale[1]=fields[1].toDouble();
					PScale[2]=fields[2].toDouble();

					
				}
				else
				if (QString::compare(line,QString("RGBA:"))==0)
				{
					line=stream.readLine();
					fields=line.split(" ", QString::SkipEmptyParts);
					r=fields[0].toDouble();
					g=fields[1].toDouble();
					b=fields[2].toDouble();
					alpha=fields[3].toDouble();
				}
				else
					if (QString::compare(line,QString("REPRESENTATION:"))==0)
				{
					line=stream.readLine();
					fields=line.split(" ", QString::SkipEmptyParts);
					representation=fields[0].toInt();
					
				}
				else
				if (QString::compare(line,QString("INTERPOLATION:"))==0)
				{
					line=stream.readLine();
					fields=line.split(" ", QString::SkipEmptyParts);
					interpolation=fields[0].toInt();
					
				}
					if (QString::compare(line,QString("GROUP_NAME:"))==0)
				{
					line=stream.readLine();
					if (!(QString::compare(line,QString(""))==0))
					{
						groupName=line;
					}
					
				}
					
			}while	( !stream.atEnd() );

		
		}



		

		
		this->PolygonSurfacePath=filename;
		vtkPolyDataReader *Reader = vtkPolyDataReader::New();
	//	const char *name = filename.toLocal8Bit().constData();
		Reader->SetFileName( filename.toLocal8Bit());// filename.toLocal8Bit());
		Reader->ReadAllFieldsOn();
		Reader->Update();


		

		if (Visible==true)
			HideActor();

		if (VPolyData!=NULL)	VPolyData->Delete();
		if (VActor!=NULL)		VActor->Delete();
		if (VMapper!=NULL)		VMapper->Delete();




		VPolyData=vtkPolyData::New();
		VPolyData->DeepCopy(Reader->GetOutput());
		//VPolyData=Reader->GetOutput();
	//	VPolyData->Update();

			//vtkDataArray *arr=VPolyData->GetPointData()->GetTCoords();




		VActor= vtkActor::New();
		VProperty=VActor->GetProperty();
		//SetDefaultProperty();

		VMapper=vtkPolyDataMapper::New();
		VMapper->ScalarVisibilityOff();
		VMapper->SetInputData(VPolyData);
		VActor->SetMapper(VMapper);

		QString textureName="";
		QFile f(filename2+".prop");
		QString gr="";
		bool repeat=true;
		if (f.exists())
		{
			UpdateProperty(VProperty,filename2+".prop",textureName,repeat,gr,VMapper);
		}
		else
	//	if (NumberOfCuttingPlanes==-1)
		{
			VProperty->SetColor(r/255.0,g/255.0,b/255.0);
			VProperty->SetOpacity(alpha/255.0);
			VProperty->SetInterpolation(interpolation);
			VProperty->SetRepresentation(representation);
			//colorLoaded=true;
		}
		//// dorabiam zeby costam sobie pokolorowac w raie czego
		/*vtkDataArray *TempData=VPolyData->GetPointData()->GetScalars();

		if (TempData!=NULL)
			if ((TempData->GetLookupTable()!=NULL))
			{
				double *nr1;
				nr1=TempData->GetLookupTable()->GetRange();
				if (nr1!=0)
				{
					VMapper->ScalarVisibilityOn();
					VMapper->SetScalarRange(nr1[0],nr1[1]);
				}
				else 
					VMapper->ScalarVisibilityOff();

				nr1=NULL;
			}

*/
		SetupMapper();


			//Matrix=vtkMatrix4x4::New();
			VActor->SetUserMatrix(Matrix);



			//Wstaw jako nazwe nazwe pliku
			QString tt;
			tt=filename;

			if(tt.mid(tt.length()-6,6)==".sdata")
				tt.remove(tt.length()-6,6);


			PolygonSurfaceName=tt;

			int pos=PolygonSurfaceName.lastIndexOf("/");
			if (pos>=0)
				PolygonSurfaceName.remove(0,pos+1);



			UpdateTexture(textureName,repeat);
			if (!gr.isEmpty())
			{
				groupName=gr;
			}
			//ustawiam srodek

			//	for(i=0;i<3;i++)
			//		Origin[i]=(double)(PN[i]-1)*(PScale[i])/2.0;


			//SetCenterOrigin();
			

			ComputeGeometricOrigin();

			Reader->Delete();
			SetupMapper();
			vtkTimeStamp *st=vtkTimeStamp::New();		st->Modified();		
			savedTimeStamp=st->GetMTime();
			st->Delete();
		//	this->CheckIfModified();
			return true;
	}
	else
	{
		return false;
	}







}
//-----------------------------
void	PolygonSurfaceData::UpdateTexture(QString name,bool repeat)

{

	if (!name.isEmpty())
			{
				//czy sa tcoords
				if (VPolyData->GetPointData()->GetTCoords()!=NULL)
				{
					// sprobuje wczytac plik z tekstura

					// najpierw lokalnie a potem w glownym

					QString filename="";
					int pos=PolygonSurfacePath.lastIndexOf("/");
					int size=PolygonSurfacePath.count();
				//	QString localName=PolygonSurfacePath.remove(pos+1,size-pos)+name+".jpg";;
					QString localName=PolygonSurfacePath;
					localName.remove(pos+1,size-pos);
					localName+=name+".jpg";

					QFile f(localName);
					if (f.exists())
					{
						filename=localName;
					}
					else 
					{
						QString globalName=progDirPath+"/texture/"+name+".jpg";
						QFile f(globalName);
						if (f.exists())
						{
							filename=globalName;
						}
					}

					if (!filename.isEmpty())
					{
						textureName=name;
						textureRepeat=repeat;
						//QString nname=this->progDirPath+"/texture/"+txt+".jpg";
						filename.replace("/","\\\\");
						const QString filename2=filename;
						vtkSmartPointer<vtkJPEGReader> imageIn =   vtkSmartPointer<vtkJPEGReader>::New();
						imageIn->SetFileName(filename2.toLocal8Bit());

						vtkSmartPointer<vtkTexture> imageTexture =
						vtkSmartPointer<vtkTexture>::New();
						if (repeat==true)
						{

							imageTexture->RepeatOn();
							imageTexture->EdgeClampOff();
						}
						else
						{
							imageTexture->EdgeClampOn();
							imageTexture->RepeatOff();
						}
							imageTexture->InterpolateOn();
						imageTexture->SetInputConnection(imageIn->GetOutputPort());

						VActor->SetTexture(imageTexture);
						

					}

					
				}
				//jak sa to poteksturuj
				
			}
}
//--------------------------------------------------------------------

bool PolygonSurfaceData::SavePolygonSurfaceDataSet2( const QString &filename)
{
	if(!filename.isEmpty())
	{

		QString filename2,filename0;	
		FILE *header;
		
		filename0=filename;
		if (filename0.lastIndexOf(".sdata")==-1)
			filename0+=".sdata";

		filename2=filename0+".header";
		QFile file( filename2);
		if ( file.open( QIODevice::WriteOnly ) ) 
		{
			QTextStream stream( &file );
			stream << "SIZES:\n" << PN[0] <<" "<< PN[1] <<" "<< PN[2] << "\n\n";
			stream << "VOXEL_DIMS:\n" << PScale[0] <<" "<< PScale[1] <<" "<< PScale[2] << "\n\n";
			/*double rgb[3];
			double opacity;
			VProperty->GetColor(rgb);
			opacity= VProperty->GetOpacity();
			stream <<  "RGBA:\n" <<	(int)(rgb[0]*255)<<" "<<(int)(rgb[1]*255)<<" "<<(int)(rgb[2]*255)<<" "<<(int)(opacity*255)<< "\n\n";
			int representation=VProperty->GetRepresentation();
			int interpolation=VProperty->GetInterpolation();
			stream <<  "REPRESENTATION:\n" <<	representation<< "\n\n";
			stream <<  "INTERPOLATION:\n" <<	representation<< "\n\n";
			if (!(QString::compare(groupName,QString(""))==0))
			{
				stream <<  "GROUP_NAME:\n" <<	groupName<< "\n\n";
			}*/
			//for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
			//	stream << *it << "\n";
			file.close();
			WriteProperty(VProperty,filename0+".prop",textureName,textureRepeat,groupName,VMapper);
		}	
		else
		{
			return false;
		}
		QFile file2(filename0);
		if ( file2.open( QIODevice::WriteOnly ) ) 
		{
		//	vtkDataArray *arr=VPolyData->GetPointData()->GetTCoords();
			//int nrrr=VPolyData->GetPointData()->GetTCoords()->GetNumberOfTuples();

			vtkPolyDataWriter *Writer	=	vtkPolyDataWriter::New();

			Writer->SetFileTypeToBinary();
			//Writer->SetFileTypeToASCII();
			Writer->SetInputData(VPolyData);
			Writer->SetFileName(filename0.toLocal8Bit());	
			Writer->Write();
			
			vtkTimeStamp *st=vtkTimeStamp::New();		st->Modified();		
			savedTimeStamp=st->GetMTime();	
			st->Delete();
			

			this->PolygonSurfacePath=filename0;
		}
		else
		{
			return false;
		}
		

		return true;

	}
	return false;
}
//---------------------------------------------------------
bool PolygonSurfaceData::SavePolygonSurfaceDataSet( const QString &filename)
{
	if(!filename.isEmpty())
	{
		//save surface to a file + a header file
		//const char *name;	
		//const char *name2;
		QString filename2,filename0;	
		FILE *header;

		filename0=filename;
		if (filename0.lastIndexOf(".poly")==-1)
			filename0+=".poly";



		//name=filename0.toLocal8Bit();

		filename2=filename0+".header";

		//name2=filename2.toLocal8Bit();

		vtkPolyDataWriter *Writer	=	vtkPolyDataWriter::New();
		Writer->SetFileTypeToBinary();
		Writer->SetInputData(VPolyData);
		Writer->SetFileName(filename0.toLocal8Bit());
		Writer->Write();

		this->PolygonSurfacePath=filename0;


		header = fopen(filename2.toLocal8Bit(), "w");
		fprintf(header,"%d %d %d \n%lf %lf %lf\n ",PN[0],PN[1],PN[2],PScale[0],PScale[1],PScale[2]);
	

		double rgb[3];
		double opacity;
		VProperty->GetColor(rgb);
		opacity= VProperty->GetOpacity();
			
		fprintf(header,"-1 \n%d %d %d %d",(int)(rgb[0]*255),(int)(rgb[1]*255),(int)(rgb[2]*255),(int)(opacity*255));  	

		int representation=VProperty->GetRepresentation();
		int interpolation=VProperty->GetInterpolation();

		fprintf(header,"\n %d %d",representation,interpolation);
		
				
		fclose(header);


		return true;
	}
	else
	{
		return false;
	}
}

//-----------------------------------------


bool PolygonSurfaceData::SaveSTLSurfaceDataSet( const QString &filename)
{
	if(!filename.isEmpty())
	{

		//const char *name;
		QString filename0;

		filename0=filename;
		if (filename0.lastIndexOf(".stl")==-1)
			filename0+=".stl";

		//name=filename0.toLocal8Bit();



		vtkSTLWriter *Writer	=	vtkSTLWriter::New();





		switch( QMessageBox::question( NULL, "Save STL as:","","Binary",
			"ASCII" ) ) 
		{
		case 0: // The user clicked the Retry again button or pressed Enter
			// try again
			Writer->SetFileTypeToBinary();
			break;
		case 1: // The user clicked the Quit or pressed Escape
			Writer->SetFileTypeToASCII();
			// exit
			break;

			Writer->SetFileTypeToBinary();
		}


		//

		Writer->SetInputData(VPolyData);
		Writer->SetFileName(filename0.toLocal8Bit());
		Writer->Write();



		return true;
	}
	else
	{
		return false;
	}
}

//-----------------------------------------


bool PolygonSurfaceData::LoadPolygonSurfaceDataSet( const QString &filename)
{



	if(!filename.isEmpty())
	{
		int r,g,b,alpha;
		int i;
		int representation=2;
		int interpolation=0;
		//const char *name1;	
		//const char *name2;
		QString filename1,filename2;	
		FILE *plik;


		filename2=filename;
		if (filename2.lastIndexOf(".poly")==-1)
			filename2+=".poly";
		//name1=filename2.toLocal8Bit();
		filename1=filename2+".header";
		//name2=filename1.toLocal8Bit();	



		plik = fopen(filename1.toLocal8Bit(), "rt");
		if (plik==NULL) return false;	
		fscanf(plik,"%d %d %d %lf %lf %lf",&PN[0],&PN[1],&PN[2],&PScale[0],&PScale[1],&PScale[2]);

		int a=fscanf(plik,"%d",&NumberOfCuttingPlanes);

		if ((a!=-1)&&(NumberOfCuttingPlanes)>0)
		{
			CuttingPlanesPoints=new double [NumberOfCuttingPlanes*9];


			for (i=0;i<NumberOfCuttingPlanes*9;i++)
				fscanf(plik,"%lf",&CuttingPlanesPoints[i]);
		}
		else
		{
			if (NumberOfCuttingPlanes==-1) //zaslepka ze bedzie troche kolorkow
			{
				int nrr=0;
				nrr=fscanf(plik,"%d %d %d %d ",&r,&g,&b,&a);
				
				nrr=fscanf(plik,"%d %d",&representation,&interpolation);
				

				a=a;
			}
		}

		fclose(plik);



		this->PolygonSurfacePath=filename;
		vtkPolyDataReader *Reader = vtkPolyDataReader::New();
		Reader->SetFileName(filename.toLocal8Bit());
		//Reader->ReadAllColorScalarsOn();
		//Reader->ReadAllTCoordsOn();
		Reader->Update();

		if (Visible==true)
			HideActor();

		if (VPolyData!=NULL)	VPolyData->Delete();
		if (VActor!=NULL)		VActor->Delete();
		if (VMapper!=NULL)		VMapper->Delete();




		VPolyData=vtkPolyData::New();
		VPolyData->DeepCopy(Reader->GetOutput());
		//VPolyData=Reader->GetOutput();
//		VPolyData->Update();

		


		VActor= vtkActor::New();
		VProperty=VActor->GetProperty();
		SetDefaultProperty();

		VMapper=vtkPolyDataMapper::New();
		VMapper->ScalarVisibilityOff();
		VMapper->SetInputData(VPolyData);
		VActor->SetMapper(VMapper);

		//if (NumberOfCuttingPlanes==-1)
		{
			VProperty->SetColor(r/255.0,g/255.0,b/255.0);
			VProperty->SetOpacity(a/255.0);
			VProperty->SetInterpolation(interpolation);
			VProperty->SetRepresentation(representation);
			//colorLoaded=true;
		}
		//// dorabiam zeby costam sobie pokolorowac w raie czego
		/*vtkDataArray *TempData=VPolyData->GetPointData()->GetScalars();

		if (TempData!=NULL)
			if ((TempData->GetLookupTable()!=NULL))
			{
				double *nr1;
				nr1=TempData->GetLookupTable()->GetRange();
				if (nr1!=0)
				{
					VMapper->ScalarVisibilityOn();
					VMapper->SetScalarRange(nr1[0],nr1[1]);
				}
				else 
					VMapper->ScalarVisibilityOff();

				nr1=NULL;
			}

*/
		SetupMapper();


			//Matrix=vtkMatrix4x4::New();
			VActor->SetUserMatrix(Matrix);



			//Wstaw jako nazwe nazwe pliku
			QString tt;
			tt=filename;

			if(tt.mid(tt.length()-5,5)==".poly")
				tt.remove(tt.length()-5,5);


			PolygonSurfaceName=tt;

			int pos=PolygonSurfaceName.lastIndexOf("/");
			if (pos>=0)
				PolygonSurfaceName.remove(0,pos+1);




			//ustawiam srodek

			//	for(i=0;i<3;i++)
			//		Origin[i]=(double)(PN[i]-1)*(PScale[i])/2.0;


			//SetCenterOrigin();


			ComputeGeometricOrigin();

			Reader->Delete();
			return true;
	}
	else
	{
		return false;
	}







}

//--------------------------------------------------------------------

bool PolygonSurfaceData::LoadOBJSurfaceDataSet( const QString &filename)
{



	if(!filename.isEmpty())
	{


		vtkSmartPointer<vtkOBJReader>Reader = vtkSmartPointer<vtkOBJReader>::New();
		Reader->SetFileName(filename.toLocal8Bit());
		//Reader->DebugOn();
//		Reader->MergingOn ();

		Reader->Update();

		if (Visible==true)
			HideActor();

		if (VPolyData!=NULL)	VPolyData->Delete();
		if (VActor!=NULL)		VActor->Delete();
		if (VMapper!=NULL)		VMapper->Delete();




		//vtkSmartPointer<vtkPolyData> pData=vtkSmartPointer<vtkPolyData>::New();
		//p//Data->DeepCopy(Reader->GetOutput());
		VPolyData=vtkPolyData::New();
		//VPolyData=Reader->GetOutput();
		
		
	
	/*	vtkSmartPointer<vtkPolyDataNormals> nrls=vtkSmartPointer<vtkPolyDataNormals>::New();
		nrls->SetInputData(Reader->GetOutput());
		nrls->NonManifoldTraversalOn();
		nrls->ComputeCellNormalsOn();	
		nrls->Update();
		VPolyData->DeepCopy(nrls->GetOutput());*/
		
		VPolyData->DeepCopy(Reader->GetOutput());
//		VPolyData->Update();




		
		VActor= vtkActor::New();
		VProperty=VActor->GetProperty();

		SetDefaultProperty();

		//VProperty->LightingOff();
		VMapper=vtkPolyDataMapper::New();
		VMapper->SetInputData(VPolyData);
		VActor->SetMapper(VMapper);


		//Matrix=vtkMatrix4x4::New();
		VActor->SetUserMatrix(Matrix);

		QString tt;
		tt=filename;

		if(tt.mid(tt.length()-4,4)==".stl")
			tt.remove(tt.length()-4,4);


		PolygonSurfaceName=tt;

		int pos=PolygonSurfaceName.lastIndexOf("/");
		if (pos>=0)
			PolygonSurfaceName.remove(0,pos+1);

		ComputeGeometricOrigin();
		//Reader->Delete();



		return true;
	}
	else
	{
		return false;
	}


}
bool PolygonSurfaceData::LoadSTLSurfaceDataSet( const QString &filename)
{



	if(!filename.isEmpty())
	{


		vtkSmartPointer<vtkSTLReader>Reader = vtkSmartPointer<vtkSTLReader>::New();
		Reader->SetFileName(filename.toLocal8Bit());
		//Reader->DebugOn();
//		Reader->MergingOn ();

		Reader->Update();

		if (Visible==true)
			HideActor();

		if (VPolyData!=NULL)	VPolyData->Delete();
		if (VActor!=NULL)		VActor->Delete();
		if (VMapper!=NULL)		VMapper->Delete();




		//vtkSmartPointer<vtkPolyData> pData=vtkSmartPointer<vtkPolyData>::New();
		//p//Data->DeepCopy(Reader->GetOutput());
		VPolyData=vtkPolyData::New();
		//VPolyData=Reader->GetOutput();
		VPolyData->DeepCopy(Reader->GetOutput());
//		VPolyData->Update();
		VActor= vtkActor::New();
		VProperty=VActor->GetProperty();

		SetDefaultProperty();

		VMapper=vtkPolyDataMapper::New();
		VMapper->SetInputData(VPolyData);
		VActor->SetMapper(VMapper);


		//Matrix=vtkMatrix4x4::New();
		VActor->SetUserMatrix(Matrix);

		QString tt;
		tt=filename;

		if(tt.mid(tt.length()-4,4)==".stl")
			tt.remove(tt.length()-4,4);


		PolygonSurfaceName=tt;

		int pos=PolygonSurfaceName.lastIndexOf("/");
		if (pos>=0)
			PolygonSurfaceName.remove(0,pos+1);

		ComputeGeometricOrigin();
		//Reader->Delete();



		return true;
	}
	else
	{
		return false;
	}


}
//-----------------------------------------




bool PolygonSurfaceData::LoadVTKSurfaceDataSet( const QString &filename)
{



	if(!filename.isEmpty())
	{


		vtkPolyDataReader *Reader = vtkPolyDataReader::New();
		Reader->SetFileName(filename.toLocal8Bit());
		Reader->Update();

		if (Visible==true)
			HideActor();

		if (VPolyData!=NULL)	VPolyData->Delete();
		if (VActor!=NULL)		VActor->Delete();
		if (VMapper!=NULL)		VMapper->Delete();





		//VPolyData=Reader->GetOutput();
		VPolyData->DeepCopy(Reader->GetOutput());
//		VPolyData->Update();
		VActor= vtkActor::New();
		VProperty=VActor->GetProperty();

		SetDefaultProperty();

		VMapper=vtkPolyDataMapper::New();
		VMapper->SetInputData(VPolyData);
		VActor->SetMapper(VMapper);


		//Matrix=vtkMatrix4x4::New();
		VActor->SetUserMatrix(Matrix);

		QString tt;
		tt=filename;

		if(tt.mid(tt.length()-4,4)==".stl")
			tt.remove(tt.length()-4,4);


		PolygonSurfaceName=tt;

		int pos=PolygonSurfaceName.lastIndexOf("/");
		if (pos>=0)
			PolygonSurfaceName.remove(0,pos+1);

		ComputeGeometricOrigin();

		Reader->Delete();

		return true;
	}
	else
	{
		return false;
	}


}
//-----------------------------------------

//--------------------------------------------------------------------

bool PolygonSurfaceData::InsertSurfaceDataSet( vtkPolyData *Input, const QString &name)
{



	if(Input!=NULL)
	{






		if (Visible==true)
			HideActor();

		if (VPolyData!=NULL)	VPolyData->Delete();
		if (VActor!=NULL)		VActor->Delete();
		if (VMapper!=NULL)		VMapper->Delete();





		VPolyData=Input;


//		VPolyData->Update();
		VActor= vtkActor::New();
		VProperty=VActor->GetProperty();

		SetDefaultProperty();

		VMapper=vtkPolyDataMapper::New();
		VMapper->SetInputData(VPolyData);
		VActor->SetMapper(VMapper);


		//Matrix=vtkMatrix4x4::New();
		VActor->SetUserMatrix(Matrix);




		PolygonSurfaceName=name;

		int pos=PolygonSurfaceName.lastIndexOf("/");
		if (pos>=0)
			PolygonSurfaceName.remove(0,pos+1);

		ComputeGeometricOrigin();



		return true;
	}
	else
	{
		return false;
	}


}
//-----------------------------------------

void PolygonSurfaceData::SetPolygonSurfaceDataSetName(const QString &name)
{

	PolygonSurfaceName=name;

}

//-----------------------------------------
void PolygonSurfaceData::SetupMapper()
{
	if ((VActor!=NULL)&&(VPolyData!=NULL))
	{
	//	this->CheckIfModified();
		vtkDataArray *TempData=VPolyData->GetPointData()->GetScalars();
		if (TempData!=NULL)
			if ((TempData->GetLookupTable()!=NULL))
			{
				
				vtkLookupTable* lut = TempData->GetLookupTable();
				int nrOfScalars = TempData->GetNumberOfValues();
				int nrOfColors = TempData->GetLookupTable()->GetNumberOfColors();

				if ((nrOfScalars > 0) && (nrOfColors > 0))
				{
					
					
					//VMapper->ScalarVisibilityOn();
					// w mpwu, vtk tak trzeba!!!! 
					lut->SetAlphaRange(0, 256);
				
					lut->SetTableRange(0, 256);
					lut->Build();

				}

				double *nr1;
				nr1=TempData->GetLookupTable()->GetRange();
				if (nr1!=0)
				{
					if (VMapper->GetScalarVisibility()==false)
					{
						VMapper->ScalarVisibilityOn();
					}
					double scalarR[2];
					VMapper->GetScalarRange(scalarR);
					
					double tol=1e-5;
					if ((fabs(scalarR[0]-nr1[0])>tol)||((fabs(scalarR[1]-nr1[1])>tol)))
					{
						VMapper->SetScalarRange(nr1[0],nr1[1]);
					}
				}
				else if (VMapper->GetScalarVisibility()==1)
				{
					VMapper->ScalarVisibilityOff();
				}

				nr1=NULL;
			}
			
	//	this->CheckIfModified();
	}
}
//----------------------------------
void PolygonSurfaceData::ShowActor()
{

	if ((VActor!=NULL)&&(VPolyData!=NULL)&&(Visible==false)&&(VWidget!=NULL))
	{
		VRenderer->AddActor(VActor);

		if (CuttingEdgeActor!=NULL)
			VRenderer->AddActor(CuttingEdgeActor);
		Visible=true;

		/*vtkDataArray *TempData=VPolyData->GetPointData()->GetScalars();
		if (TempData!=NULL)
			if ((TempData->GetLookupTable()!=NULL))
			{
				double *nr1;
				nr1=TempData->GetLookupTable()->GetRange();
				if (nr1!=0)
				{
					if (VMapper->GetScalarVisibility()==false)	VMapper->ScalarVisibilityOn();
					double scalarR[2];
					VMapper->GetScalarRange(scalarR);
					
					double tol=1e-5;
					if ((fabs(scalarR[0]-nr1[0])>tol)||((fabs(scalarR[1]-nr1[1])>tol)))
					{
						VMapper->SetScalarRange(nr1[0],nr1[1]);
					}
				}
				else if (VMapper->GetScalarVisibility()==true)
					VMapper->ScalarVisibilityOff();

				nr1=NULL;
			}
*/
		
		SetupMapper();
		
			if ((repaintWithoutReset==true))
			{
			
				VWidget->RepaintWidget();
				repaintWithoutReset=false;
			

			}
			else
			if(HideWithoutRefresh==false)
			{
		
				if (VWidget->GetResetCameraMode()==true)
				{

					VRenderer->ResetCamera();
				
				}
				
				VWidget->RepaintWidget();
				
			}
	}


}

//-----------------------------------------

void PolygonSurfaceData::HideActor()
{
	if ((VActor!=NULL)&&(VPolyData!=NULL)&&(Visible==true)&&(VWidget!=NULL))
	{
		//		ofstream myFile("c:/out.txt");	
		//myFile << " PRZED HIDE ACTOR: "<< endl<< endl << *VPolyData << endl; 

		VRenderer->RemoveActor(VActor);

		if 	(CuttingEdgeActor!=NULL)	VRenderer->RemoveActor(CuttingEdgeActor);

		Visible=false;
		//if (VWidget->GetResetCameraMode()==true) VRenderer->ResetCamera();
		//if (VWidget->GetRefreshMode()==true) VWidget->RepaintWidget();
	}
}

//-----------------------------------------

void PolygonSurfaceData::HideActorWithoutRefresh()
{
	if ((VActor!=NULL)&&(VPolyData!=NULL)&&(Visible==true) && (VWidget != NULL))
	{

		//ofstream myFile("c:/out.txt");	
		//myFile << "PRZED HIDE ACTOR WITHOUT REF: "<< endl<< endl << *VPolyData << endl; 



		VRenderer->RemoveActor(VActor);
		if 	(CuttingEdgeActor!=NULL)	VRenderer->RemoveActor(CuttingEdgeActor);
		Visible=false;
		//myFile << "PO HIDE ACTOR WITHOUT REF: "<< endl<< endl << *VPolyData << endl; 

	}
}


//-----------------------------------------

void PolygonSurfaceData::ComputeGeometricOrigin()
{

	double bounds[6];

	VPolyData->GetBounds(bounds);

	MinBound[0]=bounds[0];
	MaxBound[0]=bounds[1];
	MinBound[1]=bounds[2];
	MaxBound[1]=bounds[3];
	MinBound[2]=bounds[4];
	MaxBound[2]=bounds[5];

	Origin[0]=(MaxBound[0]-MinBound[0])/2.0+MinBound[0];
	Origin[1]=(MaxBound[1]-MinBound[1])/2.0+MinBound[1];
	Origin[2]=(MaxBound[2]-MinBound[2])/2.0+MinBound[2];

	VActor->SetOrigin(Origin);
	//	QMessageBox::about( 0,"DICOM",QString::number(Origin[0])+" "+ QString::number(Origin[1])+" "+ QString::number(Origin[2]));


}

//-----------------------------------------

void PolygonSurfaceData::SetCenterOrigin()
{
	int i;
	for(i=0;i<3;i++)
		Origin[i]=(double)(PN[i])*(PScale[i])/2.0;

	VActor->SetOrigin(Origin);

	for(i=0;i<3;i++)
	{
		MinBound[i]=0;
		MaxBound[i]=(double)(PN[i])*(PScale[i]);
	}



}
//-----------------------------------------

void PolygonSurfaceData::SetCustomOrigin(double *CustomOrigin)
{
	int i;
	for(i=0;i<3;i++)
		Origin[i]=CustomOrigin[i];

	VActor->SetOrigin(Origin);

}
//-----------------------------------------

void	PolygonSurfaceData::ReplaceCuttingEdgePolyData(vtkPolyData *CEPolyData)
{

	if (CuttingEdgeMapper!=NULL)	CuttingEdgeMapper->Delete();
	if (CuttingEdgeActor!=NULL)
	{
		if (Visible==true)
			VRenderer->RemoveActor(CuttingEdgeActor);
		CuttingEdgeActor->Delete();
	}
	if (CuttingEdgePolyData!=NULL)	CuttingEdgePolyData->Delete();




	CuttingEdgePolyData=CEPolyData;
	CuttingEdgeMapper=vtkPolyDataMapper::New();
	CuttingEdgeMapper->SetInputData(CuttingEdgePolyData);
	CuttingEdgeActor=vtkActor::New();
	CuttingEdgeActor->SetMapper(CuttingEdgeMapper);

	if (Visible==true) VRenderer->AddActor(CuttingEdgeActor);


}

//-----------------------------------------

void	PolygonSurfaceData::RebuildSurface(int Range)
{



	if (Visible==true)
	{
		HideActorWithoutRefresh();
		vtkProperty *Prop=vtkProperty::New();
		Prop->DeepCopy(VActor->GetProperty());

		VRenderer->RemoveActor(VActor);
		if (VMapper!=NULL)		VMapper->Delete();
		if (VActor!=NULL)		VActor->Delete();


		VActor= vtkActor::New();
		VActor->SetProperty(Prop);
		VProperty=VActor->GetProperty();

		//To jest zle trzeba zapamietac stara 1!!!!
		//Matrix=vtkMatrix4x4::New();
		VActor->SetUserMatrix(Matrix);

		VMapper=vtkPolyDataMapper::New();
		VMapper->SetInputData(VPolyData);

		/*	if (Range!=0)
		{
		VMapper->ScalarVisibilityOn();
		VMapper->SetScalarRange(0,Range);
		}
		else 
		VMapper->ScalarVisibilityOff();
		*/





		//// dorabiam zeby costam sobie pokolorowac w raie czego
		/*vtkDataArray *TempData=VPolyData->GetPointData()->GetScalars();

		if (TempData!=NULL)
			if ((TempData->GetLookupTable()!=NULL))
			{
				double *nr1;
				nr1=TempData->GetLookupTable()->GetRange();
				if (nr1!=0)
				{
					VMapper->ScalarVisibilityOn();
					VMapper->SetScalarRange(nr1[0],nr1[1]);
				}
				else 
					VMapper->ScalarVisibilityOff();

				nr1=NULL;
			}*/

		SetupMapper();
			VActor->SetMapper(VMapper);
			HideWithoutRefresh=true;
			ShowActor();
			HideWithoutRefresh=false;
			VWidget->RepaintWidget();


	}


}

//-----------------------------------------

//-----------------------------------------

void	PolygonSurfaceData::SetDefaultProperty()
{
	if (defaultPropertyFilename.isEmpty())
	{
	VProperty->SetInterpolationToPhong();
	VProperty->SetSpecular(0.5);
	VProperty->SetSpecularPower(128);
	VProperty->SetDiffuse(1.0);
	VProperty->SetAmbient(0.1);
	VProperty->SetAmbientColor(0.4,0.4,0.4);	
	VProperty->SetSpecularColor(0.25,0.25,0.25);
	VProperty->SetDiffuseColor(0.2,0.8,1.0);
	//shading&material
	VProperty->SetColor((double)(0/255.0),(double)(161/255.0),(double)(97/255.0));
	}
	else
	{
		QString gr;
		UpdateProperty(VProperty,defaultPropertyFilename,textureName,textureRepeat,gr);
		UpdateTexture(textureName,textureRepeat);
	}
	/*VProperty->LoadMaterial("D:\\meow\\programming\\RibnXtr2011\\material\\src_axlGui_shader_PhongRedPlasticIsophote.xml");
	VProperty->ShadingOn();*/
	
//	//VProperty->LoadMaterial("Ch06Brick.xml");
//	//VProperty->ShadingOn();
//	  char shaders[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \
//  <Material name=\"GenericAttributes1\"> \
//    <Shader scope=\"Vertex\" name=\"VertexShader\" location=\"Inline\"\
//      language=\"GLSL\" entry=\"main\"> attribute vec3 genAttrVector; \
//      varying vec4 color; \
//      void main(void) \
//      { \
//       vec3 ecPosition = vec3 (gl_ModelViewMatrix * gl_Vertex);\
//vec3 tnorm = normalize(gl_NormalMatrix * gl_Normal);\
//vec3 lightVec = normalize(LightPosition - ecPosition);\
//vec3 reflectVec = reflect(-lightVec, tnorm);\
//vec3 viewVec = normalize(-ecPosition);\
//float diffuse = max(dot(lightVec, tnorm), 0.0);\
//float spec = 0.0;\
//if (diffuse > 0.0) {\
//spec = max(dot(reflectVec, viewVec), 0.0);\
//spec = pow(spec, 16.0);\
//}\
//// Phong Shading\
//LightIntensity =\
//DiffuseContribution * diffuse +\
//SpecularContribution * spec;\
//MCposition = gl_Vertex.xy;\
//gl_Position = ftransform();\
//      } \
//  </Material>";
//
//	VProperty->LoadMaterialFromString(shaders);
//	VProperty->SetColor(1.0, 0.0, 0.0);
//	VProperty->GetShading();
//	VProperty->ShadingOn();
//	
	/*
	VProperty->SetInterpolationToPhong();
	VProperty->SetSpecular(0.05);
	VProperty->SetSpecularPower(128);
	VProperty->SetDiffuse(1.0);
	VProperty->SetAmbient(0.1);
	VProperty->SetAmbientColor(0.4,0.4,0.4);	
	VProperty->SetSpecularColor(0.25,0.25,0.25);
	VProperty->SetDiffuseColor(0.2,0.8,1.0);
	VProperty->SetColor((double)(100/255.0),(double)(100/255.0),(double)(100/255.0));

	*/

}


//--------------------------------------------
bool PolygonSurfaceData::LoadVRMLSurfaceDataSet( const QString &filename)
{
	/*	

	if(!filename.isEmpty())
	{

	vtkRenderWindow *RenWin=VWidget->Interactor->GetRenderWindow();



	vtkVRMLImporter *importer=vtkVRMLImporter::New();
	importer->SetRenderWindow(RenWin);
	importer->SetFileName(filename);
	importer->Read();

	vtkAppendPolyData *appendData= vtkAppendPolyData::New();
	vtkActorCollection *collection=VWidget->Interactor->GetRenderer()->GetActors();
	int count =collection->GetNumberOfItems();
	collection->InitTraversal();

	int i;
	for( i=0;i< count ;i++)
	{
	vtkActor *actor=collection->GetNextActor();
	actor->InitPartTraversal();
	int count1=actor->GetNumberOfParts();
	for(int j=0;j< count1 ;j++)
	{
	vtkActor *actorNew=actor->GetNextPart();
	//	prop=actor->GetProperty();
	vtkPolyData *data=(vtkPolyData *) (actorNew->GetMapper()->GetInputAsDataSet());
	appendData->AddInputData(data);
	//	VWidget->Interactor->GetRenderer()-
	}
	}


	for( i=0;i< count ;i++)
	{
	vtkActor *actor=collection->GetNextActor();
	VWidget->Interactor->GetRenderer()->RemoveActor(actor);
	}

	/*

	vtkActorCollection *collection1=renWin.GetRenderer().GetActors();
	int count1 =collection.GetNumberOfItems();
	collection.InitTraversal();

	for(int i=0;i< count ;i++){
	vtkActor actor=collection.GetNextActor();
	renWin.GetRenderer().RemoveActor(actor);
	}
	//	importer->ImportActors();
	//int nactors = importer->GetRenderer()->GetActors()->GetNumberOfItems();

	*/





	//		QMessageBox::about( 0,"boing",QString::number(nactors));
	//		return false;
	/*
	vtkPolyDataReader *Reader = vtkPolyDataReader::New();
	Reader->SetFileName(filename);
	Reader->Update();
	*/
	/*			if (Visible==true)
	HideActor();

	if (VPolyData!=NULL)	VPolyData->Delete();
	if (VActor!=NULL)		VActor->Delete();
	if (VMapper!=NULL)		VMapper->Delete();





	VPolyData=appendData->GetOutput();
	VPolyData->Update();
	VActor= vtkActor::New();
	VProperty=VActor->GetProperty();

	SetDefaultProperty();

	VMapper=vtkPolyDataMapper::New();
	VMapper->SetInputData(VPolyData);
	VActor->SetMapper(VMapper);


	//Matrix=vtkMatrix4x4::New();
	VActor->SetUserMatrix(Matrix);

	QString tt;
	tt=filename;

	if(tt.mid(tt.length()-4,4)==".stl")
	tt.remove(tt.length()-4,4);


	PolygonSurfaceName=tt;

	int pos=PolygonSurfaceName.lastIndexOf("/");
	if (pos>=0)
	PolygonSurfaceName.remove(0,pos+1);

	ComputeGeometricOrigin();



	return true;
	}
	else
	{
	return false;
	}
	*/
	return false;
}

//-----------------------------------------
bool PolygonSurfaceData::LoadNASTRANSurfaceDataSet( const QString &filename)
{



	if(!filename.isEmpty())
	{




		int i,j;
		const char *header;
		header =filename.toLocal8Bit();
		FILE *headerek;
		headerek=fopen(header,"rt");
		if (headerek==NULL)  QMessageBox::about( 0,"problemiki","");;

		for(i=0;i<7;i++)
			skipline(headerek);

		char tempChar[8];
		QString CTETRA	="CTETRA";
		QString	GRID	="GRID*";	

		fscanf(headerek,"%s\t",tempChar);
		//		int Number=0;
		int t;

		int *TetraPoints=new int[4*80000];

		int Pos=0;
		int NumberOfPoints=0;
		int NumberOfTetra=0;
		while(tempChar==CTETRA)
		{
			fscanf(headerek,"%d\t",&t);
			fscanf(headerek,"%d\t",&t);

			for(i=0;i<4;i++)
			{
				fscanf(headerek,"%d\t",&t);//Wspolrzedne wierzcholkow tetrahedronow
				if (t>NumberOfPoints) NumberOfPoints=t;
				TetraPoints[Pos++]=t;
			}
			fscanf(headerek,"%s\t",tempChar);

			NumberOfTetra++;

		}
		NumberOfPoints++;


		if (NumberOfPoints>0)
		{

			skipline(headerek);
			skipline(headerek);
			fscanf(headerek,"%s\t",tempChar);
			char AA[9];


			//DODAJE PUNKTY DO TABLICY
			vtkIdType	newCellPts[3];
			double		newPt[3];
			vtkPoints	*newPts=vtkPoints::New();
			vtkFloatArray *Tscalars = vtkFloatArray::New();
			newPts->SetNumberOfPoints(NumberOfPoints);
			int ptId;
			//			double scalar=1.0;




			fscanf(headerek,"%d\t",&ptId); 

			while(tempChar==GRID)
			{

				fscanf(headerek,"%lf\t",&newPt[0]);
				fscanf(headerek,"%lf\t",&newPt[1]);
				fscanf(headerek,"%s\t",AA);
				fscanf(headerek,"%lf\t",&newPt[2]);
				fscanf(headerek,"%s\t",tempChar);
				newPts->SetPoint(ptId,newPt);

				fscanf(headerek,"%d\t",&ptId);



			}

			fclose(headerek);
			//----------------------------------------------------------------
			int NumberOfTriangles=NumberOfTetra*4;
			vtkCellArray *newPolys = vtkCellArray::New();
			newPolys->Allocate(newPolys->EstimateSize(3,NumberOfTriangles));

			vtkPolyData *output=vtkPolyData::New();
			output->SetPoints(newPts);
			output->SetPolys(newPolys);

			vtkCellData		*outputCD=output->GetCellData();


			//			int numNewPts = 0;



			double scalar1=0.1;
			double scalar2=0.2;
			double scalar3=0.5;
			double scalar4=0.9;


			vtkIdType ID;
			newPts->SetNumberOfPoints(NumberOfPoints);

			//int *Triangle=new int[3];
			vtkIdType Triangle[3];
			int ttt=0;
			j=0;
			for(i=0;i<NumberOfTetra;i++)
			{

				//Pierwszy Trojkat

				Triangle[0]=TetraPoints[j];Triangle[1]=TetraPoints[j+2];Triangle[2]=TetraPoints[j+1];

				if (CheckForOutlineTriangle(Triangle,TetraPoints,NumberOfTetra))
				{

					ID=newPolys->InsertNextCell(3,Triangle);
					Tscalars->InsertTuple(ID,&scalar1);
					ttt++;
				}

				//DrugiTrojkat

				Triangle[0]=TetraPoints[j];Triangle[1]=TetraPoints[j+1];Triangle[2]=TetraPoints[j+3];

				if (CheckForOutlineTriangle(Triangle,TetraPoints,NumberOfTetra))
				{

					ID=newPolys->InsertNextCell(3,Triangle);
					Tscalars->InsertTuple(ID,&scalar2);
					ttt++;
				}

				//TrzeciTrojkat
				Triangle[0]=TetraPoints[j+2];Triangle[1]=TetraPoints[j];Triangle[2]=TetraPoints[j+3];



				if (CheckForOutlineTriangle(Triangle,TetraPoints,NumberOfTetra))
				{

					ID=newPolys->InsertNextCell(3,Triangle);
					Tscalars->InsertTuple(ID,&scalar3);
					ttt++;
				}


				//CzwartyTrojkat
				Triangle[0]=TetraPoints[j+1];Triangle[1]=TetraPoints[j+2];Triangle[2]=TetraPoints[j+3];


				if (CheckForOutlineTriangle(Triangle,TetraPoints,NumberOfTetra))
				{
					newCellPts[0] =Triangle[0];
					newCellPts[1] =Triangle[1];
					newCellPts[2] =Triangle[2];
					ID=newPolys->InsertNextCell(3,Triangle);
					Tscalars->InsertTuple(ID,&scalar4);
					ttt++;
				}

				j+=4;


			}



			QMessageBox::about( 0,"nr",QString::number(ttt));



			outputCD->SetScalars(Tscalars); 
			//output->GetPointData()->SetScalars(Tscalars); 
			output->BuildLinks();
			output->BuildCells();
//			//output->Update();
			newPolys->Delete();
			newPts->Delete();





			delete []TetraPoints;


			if (Visible==true)
				HideActor();

			if (VPolyData!=NULL)	VPolyData->Delete();
			if (VActor!=NULL)		VActor->Delete();
			if (VMapper!=NULL)		VMapper->Delete();





			VPolyData=output;
//			VPolyData->Update();
			VActor= vtkActor::New();
			VProperty=VActor->GetProperty();

			SetDefaultProperty();

			VMapper=vtkPolyDataMapper::New();
			VMapper->SetInputData(VPolyData);
			VActor->SetMapper(VMapper);


			//Matrix=vtkMatrix4x4::New();
			VActor->SetUserMatrix(Matrix);

			QString tt;
			tt=filename;

			if(tt.mid(tt.length()-4,4)==".stl")
				tt.remove(tt.length()-4,4);


			PolygonSurfaceName=tt;

			int pos=PolygonSurfaceName.lastIndexOf("/");
			if (pos>=0)
				PolygonSurfaceName.remove(0,pos+1);

			ComputeGeometricOrigin();



			return true;
		}
		else
		{
			return false;
		}

	}

	return false;

}
//-----------------------------------------
bool PolygonSurfaceData::CheckForOutlineTriangle(vtkIdType *trianglePoints,int *TetraPoints,int NumberOfTetra)
{

	//int trianglePoints[3];
	int pos=0;
	int i,j,k;
	int temp1,temp2;
	temp2=0;
	for (i=0;i<NumberOfTetra;i++)
	{
		temp1=0;
		//sprawdzam czy trojkat wystapil w tetrahedronie
		for(j=0;j<3;j++)
			for(k=0;k<4;k++)
				if (trianglePoints[j]==TetraPoints[pos+k]) temp1++;

		if (temp1>=3) temp2++;

		if (temp2==2) 
			return false;	



		pos+=4;

	}
	return true;
}
//----------------------------------------
//bool	PolygonSurfaceData::IsSurfaceActive()
//{
//	return SurfaceActive; 
//}
//------------------------------------------

void	PolygonSurfaceData::SlotSetTempPropAsCurrentOn()
{
	SetTempPropAsCurrent(true);
}
//------------------------
void	PolygonSurfaceData::SlotSetTempPropAsCurrentOff()
{
	SetTempPropAsCurrent(false);
}
//--------------------------------
void	PolygonSurfaceData::SetTempPropAsCurrent(bool tt)
{
	if ( VWidget != NULL)
	if (tt==true)
	{
		if(isTempPropertyOn==false)
		{
			tempProperty->DeepCopy(VProperty);
			VProperty->DeepCopy(selectProperty);

		/*	vtkProperty *tp=vtkProperty::New();
			tp->DeepCopy(VProperty);
			VProperty->DeepCopy(tempProperty);
			tempProperty->DeepCopy(tp);
			tp->Delete();*/

			isTempPropertyOn=true;
			VWidget->RepaintWidget();
			//QTimer timer;
			timer.setSingleShot(true);
			connect(&timer,SIGNAL(timeout()),this,SLOT(SlotSetTempPropAsCurrentOff()));
			timer.start(250);
			

		}
		// set temp as current
	}
	else
	{
		if(isTempPropertyOn==true)
		{
			/*vtkProperty *tp=vtkProperty::New();
			tp->DeepCopy(VProperty);
			VProperty->DeepCopy(tempProperty);
			tempProperty->DeepCopy(tp);
			tp->Delete();*/
		
			VProperty->DeepCopy(tempProperty);

			isTempPropertyOn=false;
			VWidget->RepaintWidget();
		}
		//set orig as temp
	}


}
//--------------------------------
void	PolygonSurfaceData::SetParentData(PolygonSurfaceData *pData)
{
	parentPolygonSurfaceData=pData;
}
//-----------------------------------------------
void	PolygonSurfaceData::AddNewChildData(PolygonSurfaceData *pData)
{
	childrenPolygonSurfaceDataList->InsertItem(pData);
}
//-------------------------------------------------

void		PolygonSurfaceData::SetVMapper(vtkPolyDataMapper *vMapper)
{
	if (VWidget != NULL)
	{

		VRenderer->RemoveActor(VActor);
		vtkPolyDataMapper* tmpMapper = VMapper;
		VMapper = vMapper;
		VMapper->ScalarVisibilityOff();
		VMapper->SetInputData(VPolyData);
		VActor->SetMapper(VMapper);
		VActor->Modified();
		tmpMapper->Delete();
		VRenderer->AddActor(VActor);
	}


}
//---------------------------------------------------
bool	PolygonSurfaceData::CheckIfModified()
{
	 unsigned long t1,t2,t3;

	 t1=VPolyData->GetMTime();
	 t2=VProperty->GetMTime();
	 t3=mapperTimeStamp;

	if (t1>this->savedTimeStamp) return true;
	if (t2>this->savedTimeStamp) return true;
	if (t3>this->savedTimeStamp) return true;
	return false;
}
//----
void	PolygonSurfaceData::MapperModified()
{
	vtkTimeStamp *st=vtkTimeStamp::New();		st->Modified();		
	mapperTimeStamp=st->GetMTime();
	st->Delete();
}