#include "ArteryData.h"

//---------------------------------------------
//
//void	ArteryData::SetMainData(RawDataSet *_mainData)
//{
//	if (mainData!=NULL) mainData=_mainData;
//
//}
ArteryData::ArteryData( PaletteRGBA *_Palette,unsigned char *_veinCol,int _maxMaskNr)
{
	selectedPointsNr=0;
	veinColor=_veinCol;
	vWidget=NULL;
	maxMaskNr=_maxMaskNr;
	palette=_Palette;
	mainData[0]=NULL;
	mainData[1]=NULL;
	mainRawData[0]=NULL;
	mainRawData[1]=NULL;

	int i,j;

	for(i=0;i<2;i++)
	{
		for(j=0;j<3;j++)
		{
			this->mainDataMasks[i][j]=-1;
		}
	}




	baseVectors[0]=1;	baseVectors[1]=0;	baseVectors[2]=0;	
	baseVectors[3]=0;	baseVectors[4]=1;	baseVectors[5]=0;	
	baseVectors[6]=0;	baseVectors[7]=0;	baseVectors[8]=1;	

	baseOrigin[0]=0;	baseOrigin[1]=0;	baseOrigin[2]=0;
	seedPoint[0]=0;		seedPoint[1]=0;		seedPoint[2]=0;

	frontSurfaces=new LinkedList <PolygonSurfaceData*> ;
	tubeSurfaces=new LinkedList <PolygonSurfaceData*> ;


};
//-------------------------
ArteryData::~ArteryData()
{
	// nie moge kasowac tych polsurfdata - bo przetrzymuje tylko wskazniki - musze zrobic taki trik jak ponizej za kazdym razem
	//int i;
	//	if (childrenPolygonSurfaceDataList!=NULL)
	//	{
	//		for(i=0;i<childrenPolygonSurfaceDataList->GetNumberOfItems();i++)
	//			{
	//				childrenPolygonSurfaceDataList->GetItemAtPos(i)->SetData(new PolygonSurfaceData());
	//				//pList->GetItemAtPos(i)->SetData(NULL);
	//				
	//			}
	//		delete childrenPolygonSurfaceDataList;
	//		}
	//	parentPolygonSurfaceData=NULL;
	//
	//}


	RemoveAllItemsByClearingPoiners(tubeSurfaces);
	RemoveAllItemsByClearingPoiners(frontSurfaces);
	delete tubeSurfaces;
	delete frontSurfaces;

}

void	ArteryData::SetOriginAndBaseVectors(double *_baseOrigin,double *_baseVectors)
{
	int i;

	for(i=0;i<9;i++)
		baseVectors[i]=_baseVectors[i];
	for(i=0;i<3;i++)
		baseOrigin[i]=_baseOrigin[i];


}

//------------------------------------

void	ArteryData::SetSeedPoint(double *_seedPoint)
{
	for(int i=0;i<3;i++)
		seedPoint[i]=_seedPoint[i];

}
//----------------------------
void	ArteryData::ClearFrontSurfaces()
{
	RemoveAllItemsByClearingPoiners(frontSurfaces);
}
//----------------------------
void	ArteryData::ClearTubeSurfaces()
{
	RemoveAllItemsByClearingPoiners(tubeSurfaces);

}
//-------------------------------------
bool	ArteryData::SetFrontSurfaces(LinkedList <PolygonSurfaceData*> *surfaces)
{

	//potrzebne dane - czy jest widoczna
	//czy ma ustawiony role index - zapiszemy
	//czy jest zapisana - jesli tak to sciezke zapisujemy
	//jesli nie to zapisujemy na dysku (tworzymy katalog na wszystkie nie zapisane powierzchnie i zapisujemy


	if (surfaces!=NULL)
	{
		int nr=surfaces->GetNumberOfItems();

		if (nr>0)
		{
			RemoveAllItemsByClearingPoiners(frontSurfaces);
			int i;
			int chgs=0;
			double opacity;
			for(i=0;i<nr;i++)
			{
			//	opacity=surfaces->GetItemAtPos(i)->GetData()->GetVProperty()->GetOpacity();
			//	if (opacity>0)
				if (surfaces->GetItemAtPos(i)->GetData()->GetTreeItem()->isSelected() == true)
				{
					frontSurfaces->InsertItem(surfaces->GetItemAtPos(i)->GetData());//wstawiam aktualnie widoczne danea wlasciwie wskazniki do nich tylko po to zeby moc to zasaveowac - 
					int roleIndex=0;
					surfaces->GetItemAtPos(i)->GetData()->SetRoleIndex(roleIndex);
					if (veinColor!=NULL)
					{
						
						/*double r=this->veinColor[(roleIndex)*3]/255.0;
						double g=this->veinColor[(roleIndex)*3+1]/255.0;
						double b=this->veinColor[(roleIndex)*3+2]/255.0;
						surfaces->GetItemAtPos(i)->GetData()->GetVProperty()->SetColor(r,g,b);*/
					}
					chgs++;
				}
			}
			if (chgs>0)
			{
				emit SendMessage("New front surfaces. nr="+QString::number(chgs));
				return true;
			}
		}
		return false;
	}
	return false;

}
//------------------------------------
bool	ArteryData::SetTubeSurfaces(LinkedList <PolygonSurfaceData*> *surfaces)
{

	if (surfaces!=NULL)
	{
		int nr=surfaces->GetNumberOfItems();

		if (nr>0)
		{
			RemoveAllItemsByClearingPoiners(tubeSurfaces);
			int i;
			int chgs=0;
			double opacity;
			for(i=0;i<nr;i++)
			{
				//opacity=surfaces->GetItemAtPos(i)->GetData()->GetVProperty()->GetOpacity();
				if (surfaces->GetItemAtPos(i)->GetData()->GetTreeItem()->isSelected()==true)
				//if (opacity>0)
				{
					tubeSurfaces->InsertItem(surfaces->GetItemAtPos(i)->GetData());//wstawiam aktualnie widoczne danea wlasciwie wskazniki do nich tylko po to zeby moc to zasaveowac - 
					int roleIndex=	surfaces->GetItemAtPos(i)->GetData()->GetRoleIndex();
					if (roleIndex==0) 
					{	
						QString name=surfaces->GetItemAtPos(i)->GetData()->GetPolygonSurfaceName();
						if (name.indexOf("joint")>=0)
						{
							roleIndex=1;
						}
						else if (name.indexOf("tube")>=0)
						{
							roleIndex=2;
						}
						else
						{
							roleIndex=0;
						}
						
						surfaces->GetItemAtPos(i)->GetData()->SetRoleIndex(roleIndex);
						if (veinColor!=NULL)
						{
						
						double r=this->veinColor[(roleIndex)*3]/255.0;
						double g=this->veinColor[(roleIndex)*3+1]/255.0;
						double b=this->veinColor[(roleIndex)*3+2]/255.0;
						surfaces->GetItemAtPos(i)->GetData()->GetVProperty()->SetColor(r,g,b);
						}
					}
				

				
					chgs++;
				}
			}
			if (chgs>0)
			{	emit SendMessage("New tube surfaces. nr="+QString::number(chgs));
				return true;
			}
		}
		return false;
	}
	return false;
}
//------------------------------------


void	ArteryData::LoadState(const QString filename,bool *fields)
{
	if (!filename.isEmpty())
	{

		QString mainPath,filePath;

		int pos=filename.lastIndexOf("/");
		if (pos>=0)
		{
			mainPath=filename;
			mainPath.remove(pos,mainPath.length());
		}


		
		mainDataDir=filename;
			
		if(mainDataDir.mid(mainDataDir.length()-11,11)==".arterydata")
			mainDataDir.remove(mainDataDir.length()-11,11);



		QFile file(filename);

		if (file.open(QIODevice::ReadOnly))
		{
			//QString path;
			
			QTextStream stream( &file );
			
			QString line;//=stream.readLine();

			//bool fields[15]={false};
			//if (QString::compare(line,QString("single:"))==0)
			//{
			do
			{
				line=stream.readLine();
				if (!(QString::compare(line,QString(""))==0))
				{
					if (QString::compare(line,QString("DATA_0:"))==0)
					{
						line=stream.readLine();		filePath=GetAbsPath( mainPath,line);

						//QMessageBox::about( 0,"",mainPath+"\n"+line+"\n"+filePath);

						RawDataSet *rData=new RawDataSet();
						bool test=rData->LoadRawDataSet2(filePath);
						if (test == false) continue;
						DataSet *mData=new DataSet(rData,palette, maxMaskNr);
						mainData[0]=mData;
						mainRawData[0]=mData;


						ByteMaskCollection* masks = mData->GetByteMaskInternal();
						QString tt;
						tt = filePath;
						if (tt.mid(tt.length() - 6, 6) == ".rdata")
							tt.remove(tt.length() - 6, 6);
						masks->LoadMasks(tt, rData);

						fields[0]=true;
					}
					else	if (QString::compare(line,QString("DATA_1:"))==0)
					{
						line=stream.readLine();		filePath=GetAbsPath( mainPath,line);

						//QMessageBox::about( 0,"",mainPath+"\n"+line+"\n"+filePath);

						RawDataSet *rData=new RawDataSet();
						bool test=rData->LoadRawDataSet2(filePath);
						if (test == false) continue;
						DataSet *mData=new DataSet(rData,palette, maxMaskNr);
						mainData[1]=mData;
						mainRawData[1]=mData;
						ByteMaskCollection* masks = mData->GetByteMaskInternal();
						QString tt;
						tt = filePath;
						if (tt.mid(tt.length() - 6, 6) == ".rdata")
							tt.remove(tt.length() - 6, 6);
						masks->LoadMasks(tt, rData);
						fields[1]=true;
					}else if (QString::compare(line.left(4),QString("MASK"))==0)
					{
						int dataNr=(line.mid(5,1)).toInt();
						int maskNr=(line.mid(7,1)).toInt();

						if (fields[dataNr]==false) continue;
						line=stream.readLine();		filePath=GetAbsPath( mainPath,line);
						//QMessageBox::about( 0,"",mainPath+"\n"+line+"\n"+filePath);

						bool test;

						test=mainData[dataNr]->GetMasks()->LoadMaskDataSet(filePath);
						if (test == false) continue;
						int nrr=mainData[dataNr]->GetMasks()->GetCurrentMaskNumber();
						SetMainDataMask(dataNr,maskNr,mainData[dataNr],nrr);
						fields [2+dataNr*3+maskNr]=true;
						


					}
					else if (QString::compare(line,QString("FRONT_SURFACES:"))==0)
					{
						bool test;
						test=loadSurfaceDataSet(this->frontSurfaces,stream,mainPath);
						if (test==true) fields[8]=true;
					}
					else if (QString::compare(line,QString("TUBE_SURFACES:"))==0)
					{
						bool test;
						test=loadSurfaceDataSet(this->tubeSurfaces,stream,mainPath);
						if (test==true) fields[9]=true;


					}
					else if (QString::compare(line,QString("BASE_ORIGIN:"))==0)
					{
						bool test=loadNumbers(baseOrigin, 1,3,stream);
						if (test==true) fields[10]=true;
					}
					else if (QString::compare(line,QString("BASE_VECTORS:"))==0)
					{
						bool test=loadNumbers(baseVectors, 3,3,stream);
						if (test==true) fields[11]=true;
					}
					else if (QString::compare(line,QString("SEED_POINT:"))==0)
					{
						bool test=loadNumbers(seedPoint, 1,3,stream);
						//if (test==true) fields[12]=true;
					}
					else if (QString::compare(line, QString("SEEDS:")) == 0)
					{
						bool test = loadPoints(&seeds, stream, mainPath);
					//	if (test == true) fields[13] = true;
					}
					else if (QString::compare(line, QString("IGNORE_POINTS:")) == 0)
					{
						bool test = loadPoints(&ignorePoints, stream, mainPath);
						//if (test == true) fields[13] = true;
					}
					else if (QString::compare(line,QString("SELECTED_POINTS:"))==0)
					{
						bool test;
						test=loadPoints(selectedPointsNr,selectedPoints,selectedPointsExist,stream,mainPath);
						//if (test==true) fields[8]=true;
					}
					


					
				}

			}while	(( !stream.atEnd() ));

			if ((fields[8]==true)&&(fields[9]==true))
			{
				//gdy sa i tubes i front to schowam front zeby nie wkurzalo przy wczytywaniu
				int nrr=frontSurfaces->GetNumberOfItems();
				int i;
				for(i=0;i<nrr;i++)
				{
					frontSurfaces->GetItemAtPos(i)->GetData()->GetVProperty()->SetOpacity(0);
				}
			}

			if (fields[1] == true)	emit SignalSendDataSet(mainData[1]);
			if (fields[0]==true)	emit SignalSendDataSet(mainData[0]);
			

			//fields[2,3,4] - maindata[0]
			//fields[5,6,7] - mainadat[1]
			//fields[8] - frontsurfaces
			//fields[9] - tubesurfaces
			//fields[10] - base origin
			//fields[11] - base vectors
			//fields[12] - seed point
			emit SingnalMaskUpdated();

		}
		else
		{
			emit this->SendMessage("Error loading file");
		}

	}
}
//-----------------------------------
void	ArteryData::SaveState(const QString filename)
{

	/*

	podaj plik
	jesli nie istnieje - zakladamy, jesli tak pyt czy update

	zakladamy katalog nazwa taka jak plik bez rozszerzenia
	tam zapisujemy pliki wszystkie jakie sa pod danymi stanami i uzupelniamy plik ze sciezkami - sciezki relatywne do glownego pliku zeby mozna bylo kopiowac


	*/
	//QString RelativePath( QString absolutePath, QString relativeTo, bool bIsFile /*= false*/ )

	//QString filename = QFileDialog::getSaveFileName(FileDefaultDir+"/"+data->GetRawDataSetName(),"*.arterydata",this);
	if (!filename.isEmpty())
	{
		mainDataDir="";
		//if (QFile::exists(filename)==true)
		//{
		//	//pytanie o update
		//}
		//else
		{
			QString	filename1,filename2;	
			//dodaje rozszerzenie
			filename2=filename;
			if (filename2.lastIndexOf(".arterydata")==-1)
				filename2+=".arterydata";
			QString pathName;
			QString relPathName;

			//zakladam katalog
			//usuwam rozszerzenie z zadanego pliku i zakladam taki katalog
			QString mainPath;

			int pos=filename.lastIndexOf("/");
			if (pos>=0)
			{
				mainPath=filename;
				mainPath.remove(pos,mainPath.length());
			}


			QString	dirName;
			dirName=filename;

			if(dirName.mid(dirName.length()-11,11)==".arterydata")
				dirName.remove(dirName.length()-11,11);




			pos=dirName.lastIndexOf("/");
			if (pos>=0)
				dirName.remove(0,pos+1);//


			QDir d(mainPath);
			//jeszcze trzeba sprawdzic czy dirname nie istnieje przypadkiem

			{
				d.mkdir(dirName);
				
			}

			bool test;

			// probuje przejsc do zalozonego katalogu - jesli sie udalo moge isc dalej
			test=d.cd(dirName);
			QString dirPath=d.path();

			mainDataDir=dirPath;

			if (test==true)
			{

				if ((mainRawData[0] != NULL) && (mainRawData[1] != NULL))
				{
					QString name0 = mainRawData[0]->GetRawData()->GetRawDataSetName();
					QString name1 = mainRawData[1]->GetRawData()->GetRawDataSetName();
					if (name0.compare(name1) == 0)//nazwy takie same - zmieniam name1!!!
					{
						name1 += "_1";
						mainRawData[1]->GetRawData()->SetRawDataSetName(name1);
					}
				}


				QFile file( filename2);
				if (file.open(QIODevice::WriteOnly))
				{
					QTextStream stream( &file );


					saveRawDataInfo(mainRawData[0],dirPath,stream, mainPath,"DATA_0");
					saveMaskDataInfo(mainData[0],mainDataMasks[0][0],dirPath, stream, mainPath,"MASK_0_0");
					saveMaskDataInfo(mainData[0],mainDataMasks[0][1],dirPath, stream, mainPath,"MASK_0_1");
					saveMaskDataInfo(mainData[0],mainDataMasks[0][2],dirPath, stream, mainPath,"MASK_0_2");

					saveRawDataInfo(mainRawData[1],dirPath,stream, mainPath,"DATA_1");
					saveMaskDataInfo(mainData[1],mainDataMasks[1][0],dirPath, stream, mainPath,"MASK_1_0");
					saveMaskDataInfo(mainData[1],mainDataMasks[1][1],dirPath, stream, mainPath,"MASK_1_1");
					saveMaskDataInfo(mainData[1],mainDataMasks[1][2],dirPath, stream, mainPath,"MASK_1_2");

					saveSurfaceSet(this->frontSurfaces,dirPath, stream, mainPath,"FRONT_SURFACES");
					saveSurfaceSet(this->tubeSurfaces,dirPath, stream, mainPath,"TUBE_SURFACES");

					saveNumbers(baseOrigin,1,3,stream,"BASE_ORIGIN");
					saveNumbers(baseVectors,3,3,stream,"BASE_VECTORS");
					saveNumbers(seedPoint,1,3,stream,"SEED_POINT");
					
					if (selectedPointsNr>0)
					savePoints(selectedPointsNr,selectedPoints,selectedPointsExist,dirPath, stream, mainPath,"SELECTED_POINTS");
					//saveFrontSurfaces


					if (seeds.count() > 0)
						savePoints(&seeds, stream, mainPath, "SEEDS");

					if (ignorePoints.count() > 0)
						savePoints(&ignorePoints, stream, mainPath, "IGNORE_POINTS");


					file.close();

				}

			}

		}



	}

}
//-------------------------------------
bool ArteryData::loadNumbers(double *number, int nrOfItems, int itemCount,QTextStream &stream)
{
	int i,j;
	QString line;
	int pos=0;
	for(j=0;j<nrOfItems;j++)
	{
		line=stream.readLine();
		QStringList fields = line.split(" ", QString::SkipEmptyParts);
		if (fields.count()<itemCount) return false;
		for(i=0;i<itemCount;i++)
		{
			number[pos++]=fields[i].toDouble();
		}
	}

	return true;
}
//-------------------------------

bool ArteryData::saveNumbers(double *number,int nrOfItems,int itemCount,QTextStream &stream,const QString text)
{
	int i;
	
	stream << text+":\n" ;
	
	for(i=0;i<nrOfItems*itemCount;i++)
	{
		stream << number[i] << " ";
		if (((i+1)%itemCount==0)&&(i>0)) stream << "\n";
	}

		stream << "\n\n";

	return true;

}
//----------------
bool ArteryData::loadPoints(int &nr, double *pts, bool *exTab,QTextStream &stream,const QString mainPath)
{
	int i,j;
	QString line;
	//int pos=0;
//		QString line;
	QString relPath,filePath;
	do
	{
		line=stream.readLine();
		if (!(QString::compare(line,QString(""))==0))
		{
			//line=stream.readLine();
			QStringList fields = line.split(" ", QString::SkipEmptyParts);
			int fc=fields.count();
			if (fc < 4) continue; // return false;
			int pos=fields[0].toInt();
			if (pos>=0)
			{
				exTab[pos]=true;
				for(i=0;i<3;i++)
				{
					pts[pos*3+i]=fields[i+1].toDouble();
				}
			}
		}

	}while	(( !stream.atEnd() )&&(!(QString::compare(line,QString(""))==0)));
	return true;
}

bool ArteryData::loadPoints( QVector <QVector3D> *pts,  QTextStream &stream, const QString mainPath)
{
	int i, j;
	QString line;
	//int pos=0;
	//		QString line;
	QString relPath, filePath;
	do
	{
		line = stream.readLine();
		if (!(QString::compare(line, QString("")) == 0))
		{
			//line=stream.readLine();
			QStringList fields = line.split(" ", QString::SkipEmptyParts);
			int fc = fields.count();
			if (fc<3) continue;

			pts->append(QVector3D(fields[0].toDouble(), fields[1].toDouble(), fields[2].toDouble()));

		}

	} while ((!stream.atEnd()) && (!(QString::compare(line, QString("")) == 0)));
	return true;
}

bool ArteryData::loadSurfaceDataSet(LinkedList <PolygonSurfaceData*> *surfaces,QTextStream &stream,const QString mainPath)
{
	if (surfaces==NULL) return false; 
	RemoveAllItemsByClearingPoiners(surfaces);
	QString line;
	QString relPath,filePath;
	do
	{
		line=stream.readLine();
		if (!(QString::compare(line,QString(""))==0))
		{
			int pos=line.indexOf(" ");
			int roleIndex=line.mid(0,pos).toInt();
			relPath=line.mid(pos+1,line.length()-pos+1);
			filePath=GetAbsPath( mainPath,relPath); 
			//QMessageBox::about( 0,"",mainPath+"\n"+relPath+"\n"+filePath);
			PolygonSurfaceData *dataXX=new PolygonSurfaceData(vWidget);
			bool test;
			test=dataXX->LoadPolygonSurfaceDataSet2(filePath);
			if (test==false) return false;

			dataXX->SetRoleIndex(roleIndex);

			if ((veinColor!=NULL)&&(roleIndex>0))
			{
					
					double r=this->veinColor[(roleIndex)*3]/255.0;
					double g=this->veinColor[(roleIndex)*3+1]/255.0;
					double b=this->veinColor[(roleIndex)*3+2]/255.0;
				

				dataXX->GetVProperty()->SetColor(r,g,b);

			}
			emit	SignalSendNewSurface(dataXX);
			surfaces->InsertItem(dataXX);

		}
	}while	(( !stream.atEnd() )&&(!(QString::compare(line,QString(""))==0)));//&&(!QString::compare(line.right(1),":")==0)&&(	); //dwukropek oznacza nowe slowo kluczowe

	return true;

} 
//----------------------------------
bool	ArteryData::savePoints(QVector <QVector3D> *pts, QTextStream &stream, const QString mainPath, const QString text)
{
	if ((pts->count()>=0) )
	{
		int i;
		stream << text + ":\n";

		for (i = 0; i<pts->count(); i++)
		{
		
		
				stream   << pts->at(i).x() << " " << pts->at(i).y() <<" "<< pts->at(i).z() << "\n";
		
			//saveSurfaceInfo(surfaces->GetItemAtPos(i)->GetData(),dirPath,stream,mainPath,text);
		}

		stream << "\n";
		return true;



	}

	return false;

}
//---------------------------
bool ArteryData::savePoints(int nr, double *pts, bool *existTab,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text)
{
	if ((pts!=NULL)&&(nr>0))
	{
			int i;
			stream << text+":\n" ;

			for(i=0;i<nr;i++)
			{
				if (existTab[i]==true)
				{
					stream << i << " " << pts[i*3] << " " << pts[i*3+1] << " " << pts[i*3+2] << "\n";
				}
				//saveSurfaceInfo(surfaces->GetItemAtPos(i)->GetData(),dirPath,stream,mainPath,text);
			}

			stream << "\n";
			return true;

		

	}

	return false;
}
//-0--------------
bool ArteryData::saveSurfaceSet(LinkedList <PolygonSurfaceData*> *surfaces,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text)
{

	if (surfaces!=NULL)
	{
		PolygonSurfaceData* pData;
		int nr=surfaces->GetNumberOfItems();
		if (nr>0)
		{
			int i;
			stream << text+":\n" ;

			bool globalOverwrite=false;
			bool uniqueName=false;
			bool singleOverwrite=false;

			bool modified;

			for(i=0;i<nr;i++)
			{
				bool testOver = false;
				if (globalOverwrite == true) testOver = true;

				
				pData=surfaces->GetItemAtPos(i)->GetData();
				// sprawdzam czy zmodyfikowany
				
				modified=true;
				if (pData->GetPolygonSurfacePath()!="")	modified=pData->CheckIfModified();
				else
				 {
					//nowy trzeba zapisac
					testOver = true; //chodzi o to zeby nie pytal jak nie musi
				}
				//modified=true;
				if ((globalOverwrite)||(testOver))
				{
					//uniqueName=true;
					//singleOverwrite=true;
				}
				else if (modified)
				{

					//zapytaj czy nadpisac ten, czy dac unique names, czy overwrite all

						QMessageBox msgBox;
						QPushButton *overwriteSingleButton=msgBox.addButton(tr("Overvwrite single"), QMessageBox::ActionRole);;
						QPushButton *overwriteAllButton=msgBox.addButton(tr("Overwrite all"), QMessageBox::ActionRole);;
						QPushButton *uniqueSingleButton=msgBox.addButton(tr("Use Unique name single"), QMessageBox::ActionRole);;
						QPushButton *uniqueAllButton=msgBox.addButton(tr("Use Unique name  all"), QMessageBox::ActionRole);;
						
						msgBox.setText("Surface Modified: "+pData->GetPolygonSurfaceName());
						msgBox.exec();


						if (msgBox.clickedButton() ==	overwriteSingleButton)
						{
							uniqueName=false;			globalOverwrite=false;


						}
						else if (msgBox.clickedButton() ==	overwriteAllButton)
						{
							uniqueName=false;			globalOverwrite=true;
						}
						else if (msgBox.clickedButton() ==	uniqueSingleButton)
						{
							uniqueName=true;			globalOverwrite=false;
						}
						else if (msgBox.clickedButton() ==	uniqueAllButton)
						{
							uniqueName=true;			globalOverwrite=true;
						}
						
				}


				saveSurfaceInfo(pData,dirPath,stream,mainPath,text,uniqueName,modified);
				


				//zmieniam nazwe albo zapisuje na qpe


				
				//saveSurfaceInfo(pData,dirPath,stream,mainPath,text);
			}

			stream << "\n";
			return true;

		}

	}

	return false;
}
//---------------------------
QString ArteryData::GetAbsPath(QString mainPath,QString relFilePath)
{
	//QString mainPath;

	QString name1,name0;
	int pos=relFilePath.lastIndexOf("/");
	if (pos>=0)
	{
		name0=relFilePath;
		name1=relFilePath;
		name1.remove(pos,name1.length());
		name0.remove(0,pos+1);

		QDir d(mainPath);
		d.cd(name1);
		name1 = d.absolutePath() + "/" + name0;
		return name1;

	}
	else
	{
		QDir d(mainPath);
		name1 = d.absolutePath() + "/" + relFilePath;
		return name1;
	}

	return QString("");



}
//--------------------------------------------------
bool ArteryData::saveSurfaceInfo(PolygonSurfaceData *pData,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text, bool uniqueName,bool modified)
{

	QString pathName,relPathName;
	if (pData!=NULL)
	{

		pathName=pData->GetPolygonSurfacePath();

		if (pathName=="")
		{
			QString nn=GetTimeDateName()+"_poly";
			if (pData->GetPolygonSurfaceName()=="")	pData->SetPolygonSurfaceName(nn);
			pathName=dirPath+"/"+pData->GetPolygonSurfaceName()+".sdata";
			pData->SavePolygonSurfaceDataSet2(pathName);
		}
		else
		{	
				if (modified==true)
				{
					QString tt= pData->GetPolygonSurfaceName();

		
					QString oldPath=pData->GetPolygonSurfacePath();

					int pos=oldPath.lastIndexOf("/");
					if (pos>=0)
					{
						oldPath.remove(pos,oldPath.length());
					}




					if(tt.mid(tt.length()-5,5)==".sdata")	tt.remove(tt.length()-5,5);
					if(dirPath.compare(oldPath)!=0)
					{
						if (uniqueName)			pData->SetPolygonSurfaceDataSetName(tt+"X");
					}
					pathName=dirPath+"/"+pData->GetPolygonSurfaceName()+".sdata";
					pData->SavePolygonSurfaceDataSet2(pathName);
				}
				/*vtkProperty *prop=pData->GetVProperty();
				QString tex=pData->GetTextureName();
				QString gr=pData->GetGroupName();
				if (prop!=NULL)
				{
					WriteProperty(prop,pathName+".prop",tex,gr,pData->GetVMapper());
				}*/
		}

		//QMessageBox::about( 0,"",mainPath+"\n"+pathName	);

		relPathName=RelativePath( mainPath,pathName, false );
		stream << pData->GetRoleIndex() <<" " << relPathName  <<"\n";
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------
bool ArteryData::saveRawDataInfo(DataSet *dataBig,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text)
{
	//	QTextStream stream( &file );

	//QMessageBox::about( 0,"","dupa");


	QString pathName,relPathName;
	if (dataBig != NULL)
	{
		RawDataSet* data = dataBig->GetRawData();
		if (data != NULL)
		{
			

			pathName = data->GetRawDataSetPath();

			if (pathName == "")
			{
				QString nn = GetTimeDateName() + "_data";
				if (data->GetRawDataSetName() == "") data->SetRawDataSetName(nn);
				pathName = dirPath + "/" + data->GetRawDataSetName() + ".rdata";
				data->SaveRawDataSet2(pathName);


				QString tt;
				tt = pathName;
				if (tt.mid(tt.length() - 6, 6) == ".rdata")
					tt.remove(tt.length() - 6, 6);

				QString mName = tt;

				int pos = tt.lastIndexOf("/");
				if (pos >= 0)
					tt.remove(0, pos + 1);

				data->SetRawDataSetName(tt);
				//	GUIPanel->RawDataListView->currentItem()->setText(0,tt);

			}
			



			//zapisze dodatiowo maski!!!
			ByteMaskCollection* masks = dataBig->GetByteMaskInternal();
			int nr = masks->GetMaskList()->GetNumberOfItems();
			if (nr > 0)
			{
				//zapiszê maski w pliku nazwa.masks
				//masks->SaveMasks(name);

					//uaktualniê nazwy:
				/*for (int i = 0; i < nr; i++)
				{
					QString newName = masks->GetMaskDataAtPos(i)->GetTreeItem()->text(0);
					if (!newName.isEmpty())
						masks->GetMaskDataAtPos(i)->SetMaskName(newName);
				}*/

				QString tt;
				tt = pathName;
				if (tt.mid(tt.length() - 6, 6) == ".rdata")
					tt.remove(tt.length() - 6, 6);

				QString mName = tt;
				masks->SaveMasks(mName, data);
			}





		

			//QMessageBox::about( 0,"",mainPath+"\n"+pathName	);

			relPathName = RelativePath(mainPath, pathName, false);
			stream << text + ":\n" << relPathName << "\n\n";
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
//-------------------------------------
bool ArteryData::saveMaskDataInfo(DataSet *data,int maskNr,const QString dirPath,QTextStream &stream,const QString mainPath,const QString text)
{
	//QTextStream stream( &file );

	//QMessageBox::about( 0,"mainPath","");

	//return true;
	QString pathName,relPathName;
	if (data!=NULL)
	{
		ByteMask *mData;

		mData=data->GetMasks()->GetMaskDataAtPos(maskNr);
		if (mData!=NULL)
		{
			pathName = "";// mData->getPath();

			if (pathName=="")
			{
				QString nn=GetTimeDateName()+"_"+text;
				//if (data->GetRawDataSetName()=="") data->SetRawDataSetName(nn);
				pathName=dirPath+"/"+nn+".mask";
				data->GetMasks()->SaveMaskDataSet(pathName,maskNr,data->GetRawData());
				//data->SaveRawDataSet2(pathName);
			}
			//QMessageBox::about( 0,"",mainPath+"\n"+pathName	);

			relPathName=RelativePath( mainPath,pathName, false );
			//QMessageBox::about( 0,"",relPathName);
			stream <<text+":\n" << relPathName <<"\n\n";
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
//--------------------------
void	ArteryData::RemoveAllItemsByClearingPoiners(LinkedList <PolygonSurfaceData*> *list)
{
	// metoda oszukuje - ma uszu
	if (list!=NULL)
	{
		int nr=list->GetNumberOfItems();

		if (nr>0)
		{
			int i;
			for(i=0;i<list->GetNumberOfItems();i++)
			{
				list->GetItemAtPos(i)->SetData(new PolygonSurfaceData());	
			}
			list->DeleteAllItems();
		}
	}
}
//	if (childrenPolygonSurfaceDataList!=NULL)
//	{
//		for(i=0;i<childrenPolygonSurfaceDataList->GetNumberOfItems();i++)
//			{
//				childrenPolygonSurfaceDataList->GetItemAtPos(i)->SetData(new PolygonSurfaceData());
//				//pList->GetItemAtPos(i)->SetData(NULL);
//				
//			}
//		delete childrenPolygonSurfaceDataList;
//		}
//	parentPolygonSurfaceData=NULL;
//
//}
//---------------------------------
bool		ArteryData::SetMainData(int dataNr,DataSet *_mainData)
{

	if (_mainData!=NULL)
	{
		if (_mainData->GetRawData()!=NULL)
		{

			mainData[dataNr]=_mainData;
			mainRawData[dataNr]=mainData[dataNr];
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
//------------------------
bool	ArteryData::SetMainDataMask(int dataNr,int pos,DataSet *_mainData,int nr)
{

	if ((_mainData!=NULL)&&(pos>=0)&&(pos<3)&&(nr>0))
	//if ((_mainData!=NULL)&&(pos>=0)&&(pos<3)&&(mainData[dataNr]==_mainData)&&(nr>0))
	{
		if ((_mainData->GetRawData()!=NULL))//&&(_mainData->GetRawData()==mainRawData[dataNr]))
		{
			if (_mainData->GetMasks()!=NULL)
			{
				int nrMax=_mainData->GetMasks()->GetNumberOfMasks();
				if (nrMax>nr)
				{
					this->mainDataMasks[dataNr][pos]=nr;
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;

}

//-----------------------
