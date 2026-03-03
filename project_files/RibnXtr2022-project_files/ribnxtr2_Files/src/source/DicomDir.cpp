#include "DicomDir.h"

DicomDir::DicomDir()
{
	ingnoreWindowLevel = false;
	invertPixelRepresentation=false;
	shiftValuesToZero=false;
	shiftValue=0;
	CurrentDicomDataSet=-1;

	//DICOMDirDataList=new LinkedList<DicomDataSet**>;
	dicomItemsTab=NULL;
	nrOfDicomFiles=0;
	nrOfDicomItems=0;
	DicomInfoTab=NULL;
}
//--------------------------------------------------
DicomDir::~DicomDir()
{
	 //if (DICOMDirDataList!=NULL)
	//	 delete DICOMDirDataList;
	if (dicomItemsTab!=NULL) delete []dicomItemsTab;
	if (DicomInfoTab!=NULL)
	{
		for(int i=0;i<nrOfDicomFiles;i++)
		{
				delete DicomInfoTab[i];
		}
			delete[] DicomInfoTab;
	}
			
}
//---------------------------------------------------
bool DicomDir::CheckDicomPreamble(QString path)
{
	QFile f(path);
	if (f.open(QIODevice::ReadOnly)==true)
	{
		//QDataStream stream(f);
		f.seek(128);
		char test[5]={0};
		int br=f.read(test,4);
		int val=strcmp(test,"DICM");
		f.close();
		if (val==0) return true;
	}
	
	return false;

}
//-----------------
QString DicomDir::GetSubdirPathWithLargestVolume(QString path)
{
	
	QStringList *list=new QStringList();
	UpdateSubdirPathList(list,path);
	int nrOfItems=list->count();
	QString largestDir;
	int largestVolume=0;
	int tmpVol;
	QString tempPath;
	if (nrOfItems>0)
	{
		 for ( QStringList::Iterator it = list->begin(); it != list->end(); ++it ) 
		 {
			 tempPath=*it;
			 QDir d(tempPath);
			 if (d.exists()==true)
			 {
				tmpVol=0;
				d.setFilter( QDir::Files);
				 QFileInfoList  files = d.entryInfoList();
				 if ( !files.isEmpty() ) 
				{
					 QListIterator<QFileInfo> it2(files);
					//QFileInfoListIterator it2( files.begin() );
					QFileInfo  fi;
					//while( (fi=(*it) != 0 )
					//for(it2=files.begin();it2!=files.end();++it2)
					while (it2.hasNext())
					{
						//fi=*it2;
						fi = it2.next();
						//++it;
						//sprawdzam czy to dicom
						//if (CheckDicomPreamble(fi->filePath())==true)
						tmpVol+=(fi.size());
		   			}
				}
				if (tmpVol>largestVolume)
				{
					largestVolume=tmpVol;
					largestDir=tempPath;
				}
			}
		 }
    }
	QMessageBox::about(0,"",largestDir);
	delete list;
	return largestDir;
}
//--------------------------------------------
void DicomDir::UpdateSubdirPathList(QStringList * list,  const QString path)
{
	//dodaj aktualny path do qstringlist
	//wez liste wszystkich katalogow i odpal update dla kazdego z nich
	list->append(path);
	//QMessageBox::about(0,"",path);

	QDir d(path);
	if (d.exists()==true)
	{
		d.setFilter( QDir::Dirs);
		 QFileInfoList  dirs = d.entryInfoList();
		 if ( !dirs.isEmpty() ) 
		{
			//QFileInfoListIterator it2( dirs.begin() );
			
			 QListIterator<QFileInfo> it2(dirs);
			 //QFileInfoListIterator it2( list.begin() );
			QFileInfo  di;
			//bool test=false;
			//while( (di=it.current()) != 0 )
			//	for(it2=dirs.begin();it2!=dirs.end();++it2)
			while (it2.hasNext())
			{
				//++it;
			//	di=*it2;
				di = it2.next();
				if (!( di.fileName() == "." || di.fileName() == ".." ||  di.isSymLink())) 
		
				{
					UpdateSubdirPathList(list, di.filePath() );
				}
			
		   	}
	    }
	}

	

}
//------------------------------------------------------
int	DicomDir::AnalyzeDicomDir(const QString &filename, QTreeWidget* ListView, bool ImportAll, bool isDir, double tolx)
{

	if (!filename.isEmpty())
	{
		int maxPos=0;
		int maxVal=0;
		CurrentDicomDataSet=-1;

		
		QString Ntemp;
		int pos;
		if (isDir==false)
		{
			 pos=filename.lastIndexOf("/");
			if (pos>=0)
			{
				Ntemp=filename;
				Ntemp.remove(pos,Ntemp.length());
			}
		}
		else
			Ntemp=filename;

//QMessageBox::about(0,"",Ntemp);

		QString text="";
		QString TMP;
		QDir d;
		d.setPath(Ntemp);
		d.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
		//  d.setSorting( QDir::Size | QDir::Reversed );

		QFileInfoList list = d.entryInfoList();
		//QFileInfoListIterator it2( list.begin() );
	
		QListIterator<QFileInfo> it2(list);
		QFileInfo fi;


		int nrOfDicoms=d.count();

		if (DicomInfoTab!=NULL)
		{
			for(int i=0;i<nrOfDicomFiles;i++)
			{
					delete DicomInfoTab[i];
			}
				delete[] DicomInfoTab;
		}
		DicomInfoTab=new DicomDataSet*[nrOfDicoms];
			
		//	DICOMDirDataList=new LinkedList<DicomDataSet*>;

		//int StartNr=-1;
		//int StopNr=-1;

	///	int i=0;

		text+= "Bytes Filename "+QString::number(nrOfDicoms)+"\n" ;


		//	QString TMP1,TMP2;

		//cout << "zbieram dane \n";
		//while ( (fi = it.current()) != 0 )
		//for(it2=list.begin();it2!=list.end();++it2)
		int iX = 0;
		while (it2.hasNext())
		{
			TMP=Ntemp;
			//fi=*it2;
			fi = it2.next();
			TMP+="/"+fi.fileName();
			//DicomInfoTab[i].SetNewFile(TMP);
			DicomInfoTab[iX]=new DicomDataSet(TMP);
			if(DicomInfoTab[iX]->Init==true)	
			{
				//QMessageBox::about( 0,"DICOM",TMP+"\n"+filename);
				//if (TMP.upper()==filename.upper())
				//{
				//	DicomInfoTab[i]->SelectedData=true;
				////QMessageBox::about( 0,"DICOM",QString::number(i));
				//}
				DicomInfoTab[iX]->testVal=iX;
				//cout <<"Y\t"<< i << "\t" <<DicomInfoTab[i]->Finfo.vImagePosition[0]<<"\t" << DicomInfoTab[i]->Finfo.vImagePosition[1]<<"\t"<< DicomInfoTab[i]->Finfo.vImagePosition[2]<<"\n";

				iX++;
			}
			else
			{
				cout <<"init problem\n";
			}
			
			//++it;	
		}
			//cout << "zebrane dane \n";
		nrOfDicomFiles=iX;
		if (nrOfDicomFiles>0)
		{	
			if (dicomItemsTab!=NULL) delete []dicomItemsTab;

			dicomItemsTab=new int[nrOfDicomFiles*2];
			nrOfDicomItems=0;
			double tol =1e-5;
			






			//musze posortowac tablice dicominfotabow najpierw ze wzgledu na orientacje

			for(int i=0;i<nrOfDicomFiles;i++)
			{
				DicomInfoTab[i]->Finfo.tmpSeries=-1;
			}

			double oRient[3]; 
			double tolO=1e-4;

			bool chg;
			int seriesNr=-1;
			int zero=0;
			int origSeries;
			//QString origSeriesDescription;
			//QString origStudyDescription;

			char iSeriesDescription[64] = { '\0' };
			char iStudyDescription[64] = { '\0' };
			


			char						oSeriesDescription[64] = { '\0' };   // 0x0008, 0x103E,LO
			char						oStudyDescription[64] = { '\0' };   // 0x0008, 0x1030,LO
			int countX;
			bool emptyStudy, emptySeries=false;

			QFile file("d:\\txt.txt");
			int m, i;
			bool fileopen = file.open(QIODevice::WriteOnly);
		
			do
			{
				chg=false;

				for(int i=0;i<nrOfDicomFiles;i++)
				{
					if (chg==false)
					{
						if(DicomInfoTab[i]->Finfo.tmpSeries==-1)
						{
							chg=true;
							zero=i;
							seriesNr++;
							if (seriesNr>0)
							{
								qDebug() << "problem " << seriesNr;
							}


						//	char iSeriesDescription[64];
	//						char iStudyDescription[64];


							memcpy(iSeriesDescription, DicomInfoTab[i]->Finfo.chSeriesDescription, 64);
							memcpy(iStudyDescription, DicomInfoTab[i]->Finfo.chStudyDescription, 64);

							DicomInfoTab[i]->Finfo.tmpSeries=seriesNr;
							origSeries = DicomInfoTab[i]->Finfo.iSeriesNumber;
						//	origSeriesDescription = QString::fromLatin1(iSeriesDescription);
							//origStudyDescription = QString::fromLatin1(iStudyDescription);

							

							memcpy(oSeriesDescription, DicomInfoTab[i]->Finfo.chSeriesDescription, 64);
							memcpy(oStudyDescription, DicomInfoTab[i]->Finfo.chStudyDescription, 64);

							if ((oSeriesDescription[0] == '\0') || (oSeriesDescription[0]<0)) emptySeries = true; else emptySeries = false;//nifty trick
							if ((oStudyDescription[0] == '\0') || (oStudyDescription[0]<0)) emptyStudy = true; else emptyStudy = false;
							int iii = 0;
							countX = 1;
						}
					}
					else
					{
						if(DicomInfoTab[i]->Finfo.tmpSeries==-1)//jesli orientacja taka sama jak poczatkowego nowej serii to dodaj
						{
							if (i == 58)
							{
								i = i;
							}
							//if ((origSeriesDescription.compare(QString::fromLatin1(DicomInfoTab[i]->Finfo.chSeriesDescription)) == 0))
							//if ((origStudyDescription.compare(QString::fromLatin1(DicomInfoTab[i]->Finfo.chStudyDescription)) == 0))
							bool check0 = true;
							bool check1 = true;
							int v1 = strcmp(iStudyDescription, oStudyDescription);
							int v2 = strcmp(iSeriesDescription, oSeriesDescription);
							if (!emptyStudy) check0 = (memcmp(iStudyDescription, oStudyDescription,64) == 0); //nifty trick
							if (!emptySeries) check1 = (memcmp(iSeriesDescription, oSeriesDescription,64) == 0);

							QString tt = ((check0 == true) && (check1 == true)) ? " good" : " bad " ;

						

							if (fileopen)
							{
								QTextStream stream(&file);

								if (!((check0 == true) && (check1 == true)))
								{
								//	stream << i << tt << " " << check0 << " " << check1 << "\n";
									stream << i << "\n";
									stream << "DicomInfoTab[i]->Finfo.chStudyDescription: " << iStudyDescription << "\n";
									stream << "oStudyDescription: " << oStudyDescription <<"\n";

									stream << "DicomInfoTab[i]->Finfo.chSeriesDescription: " << iSeriesDescription << "\n";
									stream << "oSeriesDescription: " << oSeriesDescription << "\n";



								
									
								}
							}

							if ((check0 == true) && (check1==true))
							//if (strcmp(DicomInfoTab[i]->Finfo.chStudyDescription, oStudyDescription) == 0)
							{


								bool test[10];
								for (int kk = 0; kk < 6; kk++)
								{
									test[kk] = fabs(DicomInfoTab[zero]->Finfo.vImageOrientation[kk] - DicomInfoTab[i]->Finfo.vImageOrientation[kk]) < tolO;
								}

								test[7] = true;

								if (ingnoreWindowLevel == false)
								{
									if ((fabs(DicomInfoTab[zero]->Finfo.fWindowCenter - DicomInfoTab[i]->Finfo.fWindowCenter) < tolO)
										&& (fabs(DicomInfoTab[zero]->Finfo.fWindowWidth - DicomInfoTab[i]->Finfo.fWindowWidth) < tolO))
									{
										test[7] = true;
									}
									else
									{
										test[7] = false;
									}

								}

								double aa[10];

								aa[0] = fabs(DicomInfoTab[zero]->Finfo.vImageOrientation[0] - DicomInfoTab[i]->Finfo.vImageOrientation[0]);
								aa[1] = fabs(DicomInfoTab[zero]->Finfo.vImageOrientation[1] - DicomInfoTab[i]->Finfo.vImageOrientation[1]);
								aa[2] = fabs(DicomInfoTab[zero]->Finfo.vImageOrientation[2] - DicomInfoTab[i]->Finfo.vImageOrientation[2]);
								aa[3] = fabs(DicomInfoTab[zero]->Finfo.fSliceThickness - DicomInfoTab[i]->Finfo.fSliceThickness);
								aa[4] = fabs(DicomInfoTab[zero]->Finfo.fSpacingBetweenSlices - DicomInfoTab[i]->Finfo.fSpacingBetweenSlices);


								if (
									/*(fabs(DicomInfoTab[zero]->Finfo.vImageOrientation[0] - DicomInfoTab[i]->Finfo.vImageOrientation[0]) < tolO)
									&& (fabs(DicomInfoTab[zero]->Finfo.vImageOrientation[1] - DicomInfoTab[i]->Finfo.vImageOrientation[1]) < tolO)
									&& (fabs(DicomInfoTab[zero]->Finfo.vImageOrientation[2] - DicomInfoTab[i]->Finfo.vImageOrientation[2]) < tolO)*/
									//&& (fabs(DicomInfoTab[zero]->Finfo.fSliceThickness - DicomInfoTab[i]->Finfo.fSliceThickness) < tolO)
									(test[0] == true) && (test[1] == true) && (test[2] == true) && (test[3] == true) && (test[4] == true) && (test[5] == true)
									&& (fabs(DicomInfoTab[zero]->Finfo.fSpacingBetweenSlices - DicomInfoTab[i]->Finfo.fSpacingBetweenSlices) < tolO)
									&& (DicomInfoTab[i]->Finfo.iSeriesNumber == origSeries)

									&& (test[7] == true)
						/*			&&(fabs(DicomInfoTab[zero]->Finfo.fWindowCenter-DicomInfoTab[i]->Finfo.fWindowCenter)<tolO)
									&&(fabs(DicomInfoTab[zero]->Finfo.fWindowWidth-DicomInfoTab[i]->Finfo.fWindowWidth)<tolO)*/
									)
								{


									//qWarning() << "good " << " nr: " << i << "tests:" << test[0] << " " << test[1] << " " << test[2] << " " << test[3] << " " << test[4] << " " << test[5];
								//	qWarning() << aa[0] << " " << aa[1] << " " << aa[2] << " " << aa[3] << " " << aa[4];

									DicomInfoTab[i]->Finfo.tmpSeries = seriesNr; 
									countX++;
								}
								else
								{
									

									//qWarning() << "bad " << " nr: " << i << "tests:" << test[0] << " " << test[1] << " " << test[2] << " " << test[3] << " " << test[4] << " " << test[5];
									//qWarning() << aa[0] << " " << aa[1] << " " << aa[2] << " " << aa[3] << " " << aa[4];
									//aa[5]=fabs(DicomInfoTab[zero]->Finfo.fWindowCenter-DicomInfoTab[i]->Finfo.fWindowCenter);
									//aa[6]=fabs(DicomInfoTab[zero]->Finfo.fWindowWidth-DicomInfoTab[i]->Finfo.fWindowWidth);
									i = i;
								}
							}
							
						
							
						}
						
					}
				}
				 




			}while(chg==true);

			file.close();


			

			//sortuje dane tak, zeby byly poukladane seriami
			QSortTmpSeries(DicomInfoTab,0,nrOfDicomFiles-1);


	/*		for (int i = 0; i < nrOfDicomFiles; i++)
			{
				qDebug() << i << " " << QString::fromLatin1(DicomInfoTab[i]->Finfo.chSeriesDescription) << " " << DicomInfoTab[i]->Finfo.tmpSeries;
				;;
			}
*/


			//if (seriesNr==0)
			//{
			//	//tymczasowo! to jest srednie i nie zawsze zadziala
			//	if(fabs(	DicomInfoTab[0]->Finfo.vImagePosition[0]-	DicomInfoTab[nrOfDicomFiles-1]->Finfo.vImagePosition[0])>tol)
			//		QSortPos(DicomInfoTab,0,nrOfDicomFiles-1,0);
			//	if(	fabs(DicomInfoTab[0]->Finfo.vImagePosition[1]-	DicomInfoTab[nrOfDicomFiles-1]->Finfo.vImagePosition[1])>tol)
			//		QSortPos(DicomInfoTab,0,nrOfDicomFiles-1,1);
			//	if(fabs(	DicomInfoTab[0]->Finfo.vImagePosition[2]-	DicomInfoTab[nrOfDicomFiles-1]->Finfo.vImagePosition[2])>tol)
			//		QSortPos(DicomInfoTab,0,nrOfDicomFiles-1,2);
			//		//cout << "posortowane dane \n";
			//}
			//else
			{

				int start, stop;
				int j;
				for(int i=0;i<nrOfDicomFiles;i++)
				{
					start=i;
					for( j=i+1;j<nrOfDicomFiles;j++)
					{
						if (DicomInfoTab[start]->Finfo.tmpSeries!=DicomInfoTab[j]->Finfo.tmpSeries) break;
					}
					stop=j-1;
					
							if(fabs(	DicomInfoTab[start]->Finfo.vImagePosition[0]-	DicomInfoTab[stop]->Finfo.vImagePosition[0])>tol)
								QSortPos(DicomInfoTab,start,stop,0);
							if(	fabs(DicomInfoTab[start]->Finfo.vImagePosition[1]-	DicomInfoTab[stop]->Finfo.vImagePosition[1])>tol)
								QSortPos(DicomInfoTab,start,stop,1);
							if(fabs(	DicomInfoTab[start]->Finfo.vImagePosition[2]-	DicomInfoTab[stop]->Finfo.vImagePosition[2])>tol)
								QSortPos(DicomInfoTab,start,stop,2);

					
					i=j-1;
				}


			}
			/*for (int ii = 0; ii < nrOfDicomFiles; ii++)
			{
				qDebug() << ii << " " << QString::fromLatin1(DicomInfoTab[ii]->Finfo.chSeriesDescription) << " " << DicomInfoTab[ii]->Finfo.tmpSeries;
				;;
			}*/

			double *TTT=new double[nrOfDicomFiles];

			//posortowane // teraz zbieram odleglosci do tab

			double fDx,fDy,fDz;

			//zbieram roznice do tablicy...
			for(int i=0;i<nrOfDicomFiles-1;i++)
			{
				 fDx =	DicomInfoTab[i+1]->Finfo.vImagePosition[0] - DicomInfoTab[i]->Finfo.vImagePosition[0];
				 fDy =	DicomInfoTab[i+1]->Finfo.vImagePosition[1] - DicomInfoTab[i]->Finfo.vImagePosition[1];
				 fDz =	DicomInfoTab[i+1]->Finfo.vImagePosition[2] - DicomInfoTab[i]->Finfo.vImagePosition[2];
				 TTT[i]=(double)sqrt(fDx*fDx + fDy*fDy + fDz*fDz);
				//cout << i<< "\t"<< DicomInfoTab[i]->testVal  <<"\t" <<TTT[i] << "\t" <<DicomInfoTab[i+1]->Finfo.vImagePosition[0]<<"\t" << DicomInfoTab[i+1]->Finfo.vImagePosition[1]<<"\t"<< DicomInfoTab[i+1]->Finfo.vImagePosition[2]<<"\n";

			}

			//zebrane odleglosci miedzy slieami



			int first=0;
			int last=-1;
			double tolerance=tolx;


		
			ListView->clear();
			

			int start, stop;
			int j;
			for (int i = 0; i<nrOfDicomFiles; i++)
			{
					start = i;
					for ( j = i + 1; j<nrOfDicomFiles; j++)
					{
					

						if (DicomInfoTab[start]->Finfo.tmpSeries != DicomInfoTab[j]->Finfo.tmpSeries) break;
					}
					stop = j - 1;
					//mam start i stop dla serii

					if ((ImportAll == true) || (stop-start<2))
					{
						CreateNewDICOMDirListItem(start, stop, ListView);
					}
					else
					{
						//first = start;
						//int *tmpSeries2 = new int[stop - start + 1];
						first = start;
						last = first;
						int ii = start;
						for ( ii = start; ii<stop ; ii++)
						{
							if (ii>start)
							if (fabs(TTT[ii-1] - TTT[ii])>tolerance)
							{
										last=ii-1;
										CreateNewDICOMDirListItem(first, last, ListView);
										first=ii;
							}
						}
						if (last < stop - 1)
						{
							CreateNewDICOMDirListItem(first, stop, ListView);
						}
					}
				
				

				i = j - 1;
			}
			qDebug() << seriesNr;
			delete[]TTT;

			
		}

	

		return maxPos;
		
	}
	return -1;

}

//---------------------------------------------------



//---------------------------------------------------

int	DicomDir::CreateNewDICOMDirListItem(int *PosTab, int Nr, QTreeWidget *ListView, bool ImportAll)
{
	int NR=-1;

	QString text="";
	int i;
	DicomDataSet **DicomListItem=new DicomDataSet*[Nr];
	
	//cout <<"nr: " << Nr <<"\n";
	double tol=1e-3;
	for(i=0;i<Nr;i++)
	{
		DicomListItem[i]=DicomInfoTab[PosTab[i]];
		
		//if(DicomListItem[i]->SelectedData==true)
			//		NR=DICOMDirDataList->GetNumberOfItems();
		/*if (PosTab[i]!=i)
		{
			//cout <<"wyciety\t" <<i << "\t" <<PosTab[i] <<"\n" ;
		}
		else
	
		{
			//cout <<"dobry\t" <<i << "\t" <<PosTab[i] <<"\n" ;
		}*/

		DicomListItem[i]->testVal=i;
		////cout << i<< "\t"<< DicomListItem[i]->testVal  << "\t" << TTT[i] << "\t" <<DicomListItem[i+1]->Finfo.vImagePosition[0]<<"\t" << DicomListItem[i+1]->Finfo.vImagePosition[1]<<"\t"<< DicomListItem[i+1]->Finfo.vImagePosition[2]<<"\n";

		if(		fabs(DicomListItem[0]->Finfo.vImagePosition[1]-DicomListItem[Nr-1]->Finfo.vImagePosition[1])>tol)
		{
				//cout <<"sort Y\n";
			QSortPos(DicomListItem,0,Nr-1,1);
		}
		if(		fabs(DicomListItem[0]->Finfo.vImagePosition[2]-DicomListItem[Nr-1]->Finfo.vImagePosition[2])>tol)
		{
				//cout <<"sort Z\n";
			QSortPos(DicomListItem,0,Nr-1,2);
	}
	////cout <<Nr <<"\n";
	//double tol =1e-5;
	//if (Nr>1)
	//{
	//	//tymczasowo! to jest srednie i nie zawsze zadziala
	//	if(fabs(	DicomListItem[0]->Finfo.vImagePosition[0]-	DicomListItem[Nr-1]->Finfo.vImagePosition[0])>tol)
	//		QSortPos(DicomListItem,0,Nr-1,0);
	//	if(	fabs(DicomListItem[0]->Finfo.vImagePosition[1]-	DicomListItem[Nr-1]->Finfo.vImagePosition[1])>tol)
	//		QSortPos(DicomListItem,0,Nr-1,1);
	//	if(fabs(	DicomListItem[0]->Finfo.vImagePosition[2]-	DicomListItem[Nr-1]->Finfo.vImagePosition[2])>tol)
	//		QSortPos(DicomListItem,0,Nr-1,2);
	//}

		//QSortImageNumber(DicomListItem,0,Nr-1);
	}


	if (Nr>2 )
	{

		double Tolerance=1e-2;
		double *TTT=new double[Nr];

		double fDx,fDy,fDz;

		//zbieram roznice do tablicy...
		for(i=0;i<Nr-1;i++)
		{
			 fDx =	DicomListItem[i+1]->Finfo.vImagePosition[0] - DicomListItem[i]->Finfo.vImagePosition[0];
			 fDy =	DicomListItem[i+1]->Finfo.vImagePosition[1] - DicomListItem[i]->Finfo.vImagePosition[1];
			 fDz =	DicomListItem[i+1]->Finfo.vImagePosition[2] - DicomListItem[i]->Finfo.vImagePosition[2];
			 TTT[i]=(double)sqrt(fDx*fDx + fDy*fDy + fDz*fDz);
			 //cout << i<< "\t"<< DicomListItem[i]->testVal  << "\t" << TTT[i] << "\t" <<DicomListItem[i+1]->Finfo.vImagePosition[0]<<"\t" << DicomListItem[i+1]->Finfo.vImagePosition[1]<<"\t"<< DicomListItem[i+1]->Finfo.vImagePosition[2]<<"\n";
			 //cout <<"after sort\t"<<i<< " "<< DicomListItem[i]->testVal  <<"\t"<< DicomListItem[i]->Finfo.vImagePosition[0] <<"\t"<<  DicomListItem[i]->Finfo.vImagePosition[1] <<"\t"<<  DicomListItem[i]->Finfo.vImagePosition[2] <<"\n";

			 //qWarning()<<(QString::number(i)+" "+QString::number(TTT[i]));

		}

		int *Series=new int[Nr];

		Series[0]=0;

		int pos=0;

		double val0,val1,val2;
		
		//tworze tablice z numerami poczatkow kolejnych serii
		if (ImportAll==false)
		for(i=1;i<Nr-1;i++)
		{
			//QMessageBox::about( 0,QString::number(i),QString::number(TTT[i])+" "+QString::number(TTT[Series[pos]]));
			
			
			if(fabs(TTT[i-1]-TTT[i])>Tolerance)
			//if(!((TTT[i-1]-Tolerance<TTT[i])&&(TTT[i]<TTT[i-1]+Tolerance)))
			
			{
				
			
				Series[++pos]=i;
				//cout << "bad dist " <<i <<"\t"<<TTT[i-1]<<" " <<TTT[i] <<"\n";
				QMessageBox::about( 0,"DICOM","");
			}
		}

		Series[++pos]=Nr;

		if (pos>0)
		{

			//int start=0,
			int stop=0;

			// a teraz chce poszukac pierwszej dluzszej niz 2 rysunki jak jest - jak nie biore pierwsza

			if ((Series[1])>1)
			{
			// to bedzie ostatni element serii
				stop=Series[1]-1;
			}
			else
			{
				stop=0;
			}
	
			int NrTemp=stop+1;
			for(i=NrTemp;i<Nr;i++)
				DicomListItem[i]->Associated=false;
			DicomDataSet **DicomListItemTemp=new DicomDataSet*[NrTemp];
	
			for(i=0;i<NrTemp;i++)
				DicomListItemTemp[i]=DicomListItem[i];
			delete DicomListItem;
		
			DicomListItem=DicomListItemTemp;
			Nr=NrTemp;


		}
		delete []TTT;
		delete []Series;

		//---------------------------------- koniec experymentu
	}

	 
	DicomListItem[0]->NumberOfSlices=Nr;  //paskudnie ale jest pierwsza w nocy...
	

	//TEMP

	QString Name=DicomListItem[0]->FileName;
	int pos2=Name.lastIndexOf("/");
	if (pos2>=0)
		Name.remove(0,pos2+1);

	//TEMP


	QTreeWidgetItem *item = new QTreeWidgetItem();
	QString name = Name + " " + QString::fromLatin1(DicomListItem[0]->Finfo.chPatientName) + " " + QString::fromLatin1(
		DicomListItem[0]->Finfo.chModality);
	item->setText(1, QString::number(Nr));

	item->setText(0, name);
	//QListViewItem *Item=new QListViewItem( ListView,ListView->currentIndex(),
	//	Name+" "+QString::fromLatin1( DicomListItem[0]->Finfo.chPatientName)+" "+QString::fromLatin1(
	//	DicomListItem[0]->Finfo.chModality),QString::number(Nr) );

//	int a=ListView->childCount();

	//ListView->setCurrentIndex(ListView->currentIndex()->nextSibling());
	//ListView->setSelected(ListView->currentIndex(),true); 		


	//DICOMDirDataList->InsertItem(DicomListItem);
	QMessageBox::about( 0,"Test","");
	if (NR!=-1) CurrentDicomDataSet=NR;//QMessageBox::about( 0,"DICOM",QString::number(NR));
	return Nr;

}


//---------------------------------------------------

int	DicomDir::CreateNewDICOMDirListItem(int Start, int Stop,QTreeWidget *ListView)
{

	int Nr=Stop-Start+1;
	int i;
	DicomDataSet **DicomListItem=new DicomDataSet*[Nr];
	

	for(i=0;i<Nr;i++)
		DicomListItem[i]=DicomInfoTab[Start+i];

	DicomListItem[0]->NumberOfSlices=Nr;  //paskudnie ale jest pierwsza w nocy...
	

	/*QTreeWidgetItem *item = new QTreeWidgetItem();
	QString name = QString::fromLatin1(DicomListItem[0]->Finfo.chPatientName) + " " + QString::fromLatin1(		DicomListItem[0]->Finfo.chModality);

	item->setText(1, QString::number(Nr));

	item->setText(0, name);
*/

	/*QListViewItem *Item=new QListViewItem( ListView,ListView->currentIndex(),
						QString::fromLatin1( DicomListItem[0]->Finfo.chPatientName)+" "+QString::fromLatin1(
						DicomListItem[0]->Finfo.chModality),QString::number(Nr) );

	ListView->setCurrentIndex(ListView->currentIndex()->nextSibling());
	ListView->setSelected(ListView->currentIndex(),true); 		*/

	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setFlags(item->flags());// | Qt::ItemIsEditable);

	bool emptySeries;
	if ((DicomListItem[0]->Finfo.chSeriesDescription[0] == '\0') || (DicomListItem[0]->Finfo.chSeriesDescription[0]<0)) emptySeries = true; else emptySeries = false;//nifty trick
	if (emptySeries == true)
	{
		bool emptyStudy;
		if ((DicomListItem[0]->Finfo.chStudyDescription[0] == '\0') || (DicomListItem[0]->Finfo.chStudyDescription[0]<0)) emptyStudy = true; else emptyStudy = false;
		if (emptyStudy == false)
		{

			item->setText(0, QString::fromLatin1(DicomListItem[0]->Finfo.chSeriesDescription));
		}
		else
		{
			QString namesss =
			//	DicomListItem[0]->Finfo.chPatientName;			namesss += "# ImageOrientation     = " +
				QString::number(DicomListItem[0]->Finfo.vImageOrientation[0]) + " "
				+ QString::number(DicomListItem[0]->Finfo.vImageOrientation[1])
				+ QString::number(DicomListItem[0]->Finfo.vImageOrientation[2]) + "|"
				+ QString::number(DicomListItem[0]->Finfo.vImageOrientation[3]) 
				+ QString::number(DicomListItem[0]->Finfo.vImageOrientation[4]) 
				+ QString::number(DicomListItem[0]->Finfo.vImageOrientation[5]) + "\n";

			item->setText(0, namesss);
		}
	}
	else
	//	item->setText(0, QString::fromLatin1(DicomListItem[0]->Finfo.chStudyDescription));
	item->setText(0, QString::fromLatin1(DicomListItem[0]->Finfo.chSeriesDescription) );
	
	item->setText(1, QString::number(Nr));
	//item->setText(3,QString::number(mode,'g',2));

	
	ListView->insertTopLevelItem(0, item);
	
	dicomItemsTab[nrOfDicomItems*2]=Start;
	dicomItemsTab[nrOfDicomItems*2+1]=Stop;
	nrOfDicomItems++;



	//zrobie rysunek


	int sx = 64;
	int sy = 64;
	QImage Image(sx, sy, QImage::Format_Indexed8);
	QPixmap pixmap;

	for (  i=0;i<256;i++)
	Image.setColor(i, qRgb(i,i,i));

	/*int i1, j1;
	for (i1 = 0; i1<sx; i1++)
	for (j1 = 0; j1 < sy; j1++)
		*(Image.scanLine(j1) + i1) = i1;;*/



	int nrPic = (Start + Stop) / 2;
	RawDataSet *tmpData = DicomInfoTab[nrPic]->GetRawDataSet2(invertPixelRepresentation, shiftValuesToZero, shiftValue);
	//DicomListItem[nrPic]// ten trzeba wczytac

	tmpData->CalculateHistogram();
	int max = tmpData->GetMaxValue();;


	double xScale = (double)tmpData->GetNx() / (double)sx;
	double yScale = (double)tmpData->GetNy() / (double)sy;
	double vScale = (double)255 / (double)max;


	int wcenter = DicomInfoTab[nrPic]->Finfo.fWindowCenter;
	if (DicomInfoTab[nrPic]->Finfo.minValue<0) wcenter-= DicomInfoTab[nrPic]->Finfo.minValue;
	int wwidth = DicomInfoTab[nrPic]->Finfo.fWindowWidth;

	int minVal = wcenter - wwidth / 2;
	int maxVal = wcenter + wwidth / 2;
	

	if (minVal < 0) minVal = 0;
	if (maxVal>max) maxVal = max;

	int realWidth = maxVal - minVal;

	double nScale = 255.0 / realWidth;
	unsigned short *q = tmpData->GetDataArray();

	unsigned char *bits = Image.bits();
	int pos = 0;
	int pos2;
	for (int j = 0; j < sy; j++)
	{
		for (int i = 0; i < sx; i++)
		{
			pos2 = (int)(i*xScale) + (int)(j*yScale)*tmpData->GetNx();
			int valx = q[pos2] ;
			if (valx < minVal) valx = minVal; else if (valx>maxVal) valx = maxVal;
			//[pos++] = (tmpData->GetDataArray()[pos2])*vScale;
			bits[pos++] = (valx-minVal)*nScale;
		}
	}


	delete tmpData;
	//	QImage tmpa = Image.convertToFormat(QImage::Format_RGB32);
	QImage tmpa = Image.convertToFormat(QImage::Format_RGB32);
	pixmap.convertFromImage(Image);
//	pixmap.save("d:\\pic2.png", "PNG");
	//	QImage smaller = tmpa.scaled(sxa, 12);

		//Pixmap.fromImage( tmpa);
	//	QPixmap pp(sxa, sy);
	//	QPainter paint(&pp);
//paint.drawImage(0, 0, smaller);
	item->setIcon(2, QIcon(pixmap));

	//item->setPixmap(1,pixmap);


	//DICOMDirDataList->InsertItem(DicomListItem);
	return Nr;
}

//---------------------------------------------------
/*
void	DicomDir::SlotImportDICOMDataSet(int pos)
{
 
	emit SendMessage("Importing DICOM file ...");
	QApplication::setOverrideCursor( waitCursor );

	QString text="";
	QString Name;
//	int ind;




	DICOMDirDataList->setCurrentIndex(pos);

	DicomDataSet **Temp=DICOMDirDataList->GetCurrentListItem()->GetData();
	//teraz tylko create volume...

	double fGantryShift;
	double fSliceDistance;

	double fDx,fDy,fDz,fImageDistance;
	if(Temp[0]->NumberOfSlices>1)
	{

		 fDx = Temp[1]->Finfo.vImagePosition.x - Temp[0]->Finfo.vImagePosition.x;
		 fDy =	Temp[1]->Finfo.vImagePosition.y - Temp[0]->Finfo.vImagePosition.y;
		 fDz = Temp[1]->Finfo.vImagePosition.z - Temp[0]->Finfo.vImagePosition.z;
		 fImageDistance	= (double)sqrt(fDx*fDx + fDy*fDy + fDz*fDz);



							
		 fSliceDistance = (double)(fImageDistance * cos(Temp[0]->Finfo.fGantryTilt * rad));
		 fGantryShift = 0.0f;
		
		fGantryShift = sin(Temp[0]->Finfo.fGantryTilt * rad) * fImageDistance / Temp[0]->Finfo.fPixelSpacingRow;
	}
	else
	{
		fSliceDistance=1;
		fGantryShift=0;

	}
	//w sumie to nie wiem ile
	if (fSliceDistance==0) fSliceDistance=1.0;

	//Zamienilem linijke nizej rows/columns!!!!
	int	Ny=Temp[0]->Finfo.iColumns +(int)(ceil((fabs(fGantryShift) * (double)(Temp[0]->NumberOfSlices))));


	 RawDataSet *dataset=new RawDataSet( Temp[0]->Finfo.iRows, Ny, Temp[0]->NumberOfSlices,
		 Temp[0]->Finfo.fPixelSpacingRow, Temp[0]->Finfo.fPixelSpacingColumn,fSliceDistance,Temp[0]->Finfo.fGantryTilt,true);

	unsigned short *PixelData=dataset->GetDataArray();

	int i;
	int SliceSize=dataset->GetSliceSize();

	if(fGantryShift>=0)
		for(i=0;i<Temp[0]->NumberOfSlices;i++)
		{
			Temp[i]->GetPixelData(PixelData+(int)ceil(i*fGantryShift)*Temp[0]->Finfo.iRows);
			PixelData+=SliceSize;

		


		}
	else
		for(i=0;i<Temp[0]->NumberOfSlices;i++)
		{

			Temp[i]->GetPixelData(PixelData-(int)ceil((Temp[0]->NumberOfSlices-1-i)*fGantryShift)*Temp[0]->Finfo.iRows);
			PixelData+=SliceSize;
		}

	dataset->GetRawDataSetName()= QString::fromLatin1( Temp[0]->Finfo.chPatientName)+"_"+QString::fromLatin1( Temp[0]->Finfo.chModality) ;


//// a gdy signed to teraz wszystko przesune...

	if (Temp[0]->Finfo.uPixelRepresentation==1)
	{
		dataset->ConvertFromSignedData();
	}
//



	InsertRawDataSet(dataset );

	QApplication::restoreOverrideCursor(  );
	emit SendMessage("Importing DICOM file ... done ");


//	QMessageBox::about( 0,"DICOM",text);

}
*/

//---------------------------------------------------
/*
void	MainWindow::SlotCompareTwoDicomFiles()
{
	QString filename1 = QFileDialog::getOpenFileName(FileDefaultDir,"",this);
	SlotUpdateDefaultDir(filename1);
	QString filename2 = QFileDialog::getOpenFileName(FileDefaultDir,"",this);
	SlotUpdateDefaultDir(filename2);
	if((!filename1.isEmpty())&&(!filename2.isEmpty()))
	{

		DicomDataSet *d1=new DicomDataSet(filename1);
		DicomDataSet *d2=new DicomDataSet(filename2);

		d1->CompareWithFile(d2);
	}

	
}
*/
//---------------------------------------------------
void	DicomDir::QSortTmpSeries(DicomDataSet **A,int Lo,int Hi)
{
		int   i,j,k;
		k = (Lo+Hi)/2;
		double V;
		DicomDataSet *T=A[k];
		V=T->Finfo.tmpSeries;
		i = Lo;   j =Hi;
		do
		{
			while(A[i]->Finfo.tmpSeries < V) i++;
			while(A[j]->Finfo.tmpSeries > V) j--;
			if (i<=j)
			{
				T = A[i];  A[i] = A[j];  A[j] = T;
				i++;
				if (j>0) j--;
			}
		}while (i<=j);	
		if(Lo<j) QSortTmpSeries(A,Lo,j);
		if(i<Hi) QSortTmpSeries(A,i,Hi);

	}
//-----------------------------
void	DicomDir::QSortPos(DicomDataSet **A,int Lo,int Hi,int nr)
{
		int   i,j,k;
		k = (Lo+Hi)/2;
		double V;
		DicomDataSet *T=A[k];
		V=T->Finfo.vImagePosition[nr];
		i = Lo;   j =Hi;
		do
		{
			while(A[i]->Finfo.vImagePosition[nr] < V) i++;
			while(A[j]->Finfo.vImagePosition[nr] > V) j--;
			if (i<=j)
			{
				T = A[i];  A[i] = A[j];  A[j] = T;
				i++;
				if (j>0) j--;
			}
		}while (i<=j);	
		if(Lo<j) QSortPos(A,Lo,j,nr);
		if(i<Hi) QSortPos(A,i,Hi,nr);

	}
//-----------------------------
void	DicomDir::QSortOrient(DicomDataSet **A,int Lo,int Hi,int nr)
{
		int   i,j,k;
		k = (Lo+Hi)/2;
		double V;
		DicomDataSet *T=A[k];
		V=T->Finfo.vImageOrientation[nr];
		i = Lo;   j =Hi;
		do
		{
			while(A[i]->Finfo.vImageOrientation[nr] < V) i++;
			while(A[j]->Finfo.vImageOrientation[nr] > V) j--;
			if (i<=j)
			{
				T = A[i];  A[i] = A[j];  A[j] = T;
				i++;
				if (j>0) j--;
			}
		}while (i<=j);	
		if(Lo<j) QSortOrient(A,Lo,j,nr);
		if(i<Hi) QSortOrient(A,i,Hi,nr);

	}


//-----------------------------------------------------
void	DicomDir::QSortImageNumber(DicomDataSet **A,int Lo,int Hi)
{
		int   i,j,k;
		k = (Lo+Hi)/2;
		double V;
		DicomDataSet *T=A[k];
		V=T->Finfo.iImageNumber;
		i = Lo;   j =Hi;
		do
		{
			while(A[i]->Finfo.iImageNumber < V) i++;
			while(A[j]->Finfo.iImageNumber > V) j--;
			if (i<=j)
			{
				T = A[i];  A[i] = A[j];  A[j] = T;
				i++;
				if (j>0) j--;
			}
		}while (i<=j);	
		if(Lo<j) QSortImageNumber(A,Lo,j);
		if(i<Hi) QSortImageNumber(A,i,Hi);

	}