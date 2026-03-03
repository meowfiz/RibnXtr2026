#include "ByteMaskCollection.h"




ByteMaskCollection::ByteMaskCollection(int _VolumeSize,int _NumberOfMaxItems,bool createFirst)
{

	
	//	Mask=NULL;
	VolumeSize=_VolumeSize;
	NumberOfMaxItems=_NumberOfMaxItems;
	CurrentMask=NULL;
	CompressedMaskList=new LinkedList<ByteMask*>; 
	



	//int *EmptyMask=new int[4];
	//*(EmptyMask)=0;
	//*(EmptyMask+1)=2; //2004.02.05
	//*(EmptyMask+2)=0;
	//*(EmptyMask+3)=VolumeSize;

	
	if (createFirst==true)
	CompressedMaskList->InsertItem(new ByteMask(VolumeSize));

}

//----------------------------------------

ByteMaskCollection::~ByteMaskCollection()
{

	

	//skasowac wszystkie maski
	// dorobic delete all items w LinkedList

	CompressedMaskList->DeleteAllItems();

}

//----------------------------------------

void ByteMaskCollection::DeleteAllMasks()
{
	CompressedMaskList->DeleteAllItems();
}

//----------------------------------------
void	ByteMaskCollection::DeleteMaskAtPos(int pos)
{
	CompressedMaskList->DeleteItem(pos);
}
//----------------------------------------

void	ByteMaskCollection::AddNewMask(unsigned char *Data)
{

	//kompresja maski
	CompressedMaskList->SetCurrentItem(CompressedMaskList->GetNumberOfItems() - 1);
	CompressMask(Data);
	//jezeli maska nie jest ostatnia - wywalic wszsytkie nastepne 
	//dorobic to w Linked List.h
	// jezeli maska jest ostatnia:
	// jezeli pozycjaaktualnej=max wywal pierwsza i dostaw nowa na koniec
	//dostaw nowa na koniec

}
void	ByteMaskCollection::InsertNewMask(unsigned char *Data)
{

	//kompresja maski

	CompressMask(Data);
	//jezeli maska nie jest ostatnia - wywalic wszsytkie nastepne 
	//dorobic to w Linked List.h
	// jezeli maska jest ostatnia:
	// jezeli pozycjaaktualnej=max wywal pierwsza i dostaw nowa na koniec
	//dostaw nowa na koniec

}


//----------------------------------------

void	ByteMaskCollection::SetNumberOfMasks(int _NumberOfMaxItems)
{

	//tu jest co sniezle popieprzone - narazie nie warto uzywac!!

	if(_NumberOfMaxItems<CompressedMaskList->GetNumberOfItems())
	{

		if (CompressedMaskList->GetCurrentListItem()==CompressedMaskList->GetTailListItem())
		{
			//jak nowa liczba max jest > od poprzedniej- skasowac tyle od poczatku zeby bylo ok
			int NumberOfDeletions=CompressedMaskList->GetNumberOfItems()-_NumberOfMaxItems;

			while (NumberOfDeletions>0)
			{
				CompressedMaskList->DeleteItem(0);	
			}
		}
		else
		{

			if(CompressedMaskList->GetCurrentItemPos()+1>_NumberOfMaxItems)
			{
				CompressedMaskList->DeleteAllItemsAfterCurrent();

				int NumberOfDeletions=CompressedMaskList->GetNumberOfItems()-_NumberOfMaxItems;

				while (NumberOfDeletions>0)
				{
					CompressedMaskList->DeleteItem(0);	
				}

			}
			else
			{
				CompressedMaskList->DeleteAllItemsAfterCurrent();
			}



		}





	}


	NumberOfMaxItems=_NumberOfMaxItems;
}

//----------------------------------------

int	ByteMaskCollection::GetNumberOfMasks()
{

	return CompressedMaskList->GetNumberOfItems();
}

//----------------------------------------

bool	ByteMaskCollection::LoadMaskDataSet( const QString &filename, RawDataSet *data, bool fix2D, QString fix2DPath)
{


	if (!filename.isEmpty())
	{
		int VolSize, Size;
		//const  char *name1;	
		//const  char *name2;
		QString filename1, filename2;
		FILE *plik;
		filename2 = filename;
		if (filename2.lastIndexOf(".mask") == -1)
			filename2 += ".mask";
		//name1 = filename2.toLocal8Bit();

		int count;
		int vSize = 0;
		filename1 = filename2 + ".header";
		//name2 = filename1.toLocal8Bit();
		// QByteArray bb = filename1.toLocal8Bit();
		int n[3] = { 0 };
		double s[3], o[3];
		int size=0;
		QFile file(filename1);
		QString name;
		bool newStyle = false;
		int test[4] = { 0 };
		if (file.open(QIODevice::ReadOnly))
		{
			QTextStream stream(&file);

			QString line;//=stream.readLine();
			QStringList fields;// = line.split( " ");


		
			do
			{
				line = stream.readLine();

				if (QString::compare(line, QString("SIZE:")) == 0)
				{
					line = stream.readLine();
					size = line.toInt();
					line = stream.readLine();
					name = stream.readLine();
					if (vSize == 0) vSize = n[0] * n[1] * n[2];
					test[3] = 1;
				

				}
				else if (QString::compare(line, QString("SIZES:")) == 0)
				{
					line = stream.readLine();
					fields = line.split(" ", QString::SkipEmptyParts);
					count = fields.count();
					n[0] = fields[0].toInt();
					n[1] = fields[1].toInt();
					n[2] = fields[2].toInt();
					test[0] = 1;
					//if (Nx*Ny*Nz == 0) return false;

				}
				else
					if (QString::compare(line, QString("VOXEL_DIMS:")) == 0)
					{
						line = stream.readLine();
						fields = line.split(" ", QString::SkipEmptyParts);
						s[0] = fields[0].toDouble();
						s[1] = fields[1].toDouble();
						s[2] = fields[2].toDouble();
						test[1] = 1;
						//if (Xscale*Yscale*Zscale == 0) return false;
					}
					else
						if (QString::compare(line, QString("START_POS:")) == 0)
						{
							line = stream.readLine();
							fields = line.split(" ", QString::SkipEmptyParts);
							o[0] = fields[0].toDouble();
							o[1] = fields[1].toDouble();
							o[2] = fields[2].toDouble();
							test[2] = 1;
						}



			} while (!stream.atEnd());
			file.close();
		}

		if (test[0] * test[1] * test[2]*test[3] > 0)
		{

			if (vSize != VolumeSize)
			{
				if (data != NULL)
				{
					//ró¿ne rozmiary 
					int n1[3] = { 0 };
					double s1[3], o1[3];
					data->GetN(n1);
					data->GetScale(s1);
					data->GetOrigin(o1);

					double tol = 1e-6;
					/*if (
						(fabs(s[0] - s1[0]) < tol) &&
						(fabs(s[1] - s1[1]) < tol) &&
						(fabs(s[2] - s1[2]) < tol)
						)*/
					{
						//skale takie same moge obsluzyc!!!!


						QFile file2(filename2);
						if (file2.open(QIODevice::ReadOnly))
						{
							int *compressedMask = new int[size];
							if (file2.read((char*)compressedMask, size * sizeof(int)))
							{
								//ByteMask *newMask = new ByteMask(CompressedMask);
								//maskaWczytana - trzeba ja rozkompresowaæ do tymczasowego rozmiaru!!!

								double scaleInv[3] = { 1.0 / s[0],1.0 / s[1],1.0 / s[2] };

								RawDataSet *tmpData = new RawDataSet(n[0], n[1], n[2], s[0], s[1], s[2], 0, false);
								unsigned char *tmpMask = new unsigned char[tmpData->GetVolumeSize()];
								DecompressMask(compressedMask, tmpMask);
								unsigned char *mask = new unsigned char[data->GetVolumeSize()];
								memset(mask, 0, data->GetVolumeSize());
								double tmpPos[3];
								int tP[3];
								int pos = 0;
								int count = 0;
								for (int k = 0; k < n1[2]; k++)
								{
									tmpPos[2] = (k*s1[2] + o1[2] - o[2])*scaleInv[2];
									tP[2] = Round(tmpPos[2]);
									for (int j = 0; j < n1[1]; j++)
									{
										tmpPos[1] = (j*s1[1] + o1[1] - o[1])*scaleInv[1];
										tP[1] = Round(tmpPos[1]);
										for (int i = 0; i < n1[0]; i++)
										{
											tmpPos[0] = (i*s1[0] + o1[0] - o[0])*scaleInv[0];
											tP[0] = Round(tmpPos[0]);
											
											if (
												(tP[0] >= 0) && (tP[0] < n[0] - 1) &&
												(tP[1] >= 0) && (tP[1] < n[1] - 1) &&
												(tP[2] >= 0) && (tP[2] < n[2] - 1)
												)
											{
												int tmp=	tmpMask[tmpData->GetVoxelPos(tP[0], tP[1], tP[2])] ;
												mask[pos] = tmp;
												if (tmp > 0)count++;
											}
											pos++;
										}
										
											

										
									}
								}


								//teraz trzeba skompresowac i wstawic now¹

								if (fix2D == true)
								{
									count = 0;
									unsigned char* mask2 = new unsigned char[data->GetVolumeSize()];
									memset(mask2, 0, data->GetVolumeSize());
									pos = 0;

									double fixTrans2d[2] = { 0 };
									QFile file2q(fix2DPath);
									if (file2q.open(QIODevice::ReadOnly))
									{
										QTextStream stream(&file2q);//bo sprawdzilem wczesniej
										QString line = stream.readLine();
										QStringList fields = line.split(QRegExp(" |\t|:"), QString::SkipEmptyParts);
										int countFields = fields.count();
										if (countFields > 1)
										{
											fixTrans2d[0] = fields[0].toDouble();
											fixTrans2d[1] = fields[1].toDouble();
										}

										for (int k = 0; k < n1[2]; k++)
										{
											tP[2] = k;



											for (int j = 0; j < n1[1]; j++)
											{
												tP[1] = j - fixTrans2d[1];

												for (int i = 0; i < n1[0]; i++)
												{

													tP[0] = i - fixTrans2d[0];

													if (
														(tP[0] >= 0) && (tP[0] < n1[0]) &&
														(tP[1] >= 0) && (tP[1] < n1[1]) &&
														(tP[2] >= 0) && (tP[2] < n1[2])
														)
													{
														int tmp = mask[data->GetVoxelPos(tP[0], tP[1], tP[2])];
														mask2[pos] = tmp;
														if (tmp > 0) count++;
													}
													pos++;



												}




											}

											//readFix2D next values

											line = stream.readLine();
											fields = fields = line.split(QRegExp(" |\t|:"), QString::SkipEmptyParts);
											int countFields = fields.count();
											if (countFields > 1)
											{
												fixTrans2d[0] = fields[0].toDouble();
												fixTrans2d[1] = fields[1].toDouble();
											}
										}

										file2q.close();
									}

									//podmianka

									unsigned char* m = mask;
									mask = mask2;
									delete[]m;



								}



								//teraz trzeba skompresowac i wstawic now¹
							if (count >0)	CompressMask(mask);


								delete[]compressedMask;
								delete[]mask;
								delete[]tmpMask;
								delete tmpData;
								//delete newMask;
								//newMask->SetMaskName(name);
								//CompressedMaskList->InsertItem(newMask);
							}
							file2.close();
						}





					}


				}


			}
			else
			{
				//wczytaj!
				if (size > 0)
				{
					if (fix2D == false)
					{

						QFile file2(filename2);
						if (file2.open(QIODevice::ReadOnly))
						{
							int* CompressedMask = new int[size];
							if (file2.read((char*)CompressedMask, size * sizeof(int)))
							{
								ByteMask* newMask = new ByteMask(CompressedMask, filename2);
								newMask->SetMaskName(name);
								CompressedMaskList->InsertItem(newMask);
							}
							file2.close();
						}
						else
						{

							//zakoncz
							file.close();
							return false;
						}
					}
					else
					{
						QFile file2(filename2);
						if (file2.open(QIODevice::ReadOnly))
						{
							int* compressedMask = new int[size];
							

							if (file2.read((char*)compressedMask, size * sizeof(int)))
							{
								unsigned char* tmpMask = new unsigned char[data->GetVolumeSize()];
								DecompressMask(compressedMask, tmpMask);
								unsigned char* mask = new unsigned char[data->GetVolumeSize()];
								memset(mask, 0, data->GetVolumeSize());
								//wczytaj
								//zdekompresuj
								//przesun
								// 
								// 
								// w tmpMaskSiedzi orginal do przesuniecia a finalna bedzie w mask 

								//przesuwanie maski!!

								int n1[3];
								data->GetN(n1);
								int count = 0;
								double fixTrans2d[2] = { 0 };
								int sliceSize = data->GetSliceSize();
								int tP[3];
								int pos = 0;
								//readFIx2d firstvalue

								QFile file2q(fix2DPath);
								if (file2q.open(QIODevice::ReadOnly))
								{
									QTextStream stream(&file2q);//bo sprawdzilem wczesniej
									QString line = stream.readLine();
									QStringList fields = line.split(QRegExp(" |\t|:"), QString::SkipEmptyParts);
									int countFields = fields.count();
									if (countFields > 1)
									{
										fixTrans2d[0] = fields[0].toDouble();
										fixTrans2d[1] = fields[1].toDouble();
									}

									for (int k = 0; k < n1[2]; k++)
									{
										tP[2] = k;



										for (int j = 0; j < n1[1]; j++)
										{
											tP[1] = j - fixTrans2d[1];

											for (int i = 0; i < n1[0]; i++)
											{

												tP[0] = i - fixTrans2d[0];

												if (
													(tP[0] >= 0) && (tP[0] < n1[0]) &&
													(tP[1] >= 0) && (tP[1] < n1[1]) &&
													(tP[2] >= 0) && (tP[2] < n1[2])
													)
												{
													int tmp = tmpMask[data->GetVoxelPos(tP[0], tP[1], tP[2])];
													mask[pos] = tmp;
													if (tmp > 0) count++;
												}
												pos++;



											}




										}
										//readFix2D next values

										line = stream.readLine();
										fields = fields = line.split(QRegExp(" |\t|:"), QString::SkipEmptyParts);
										int countFields = fields.count();
										if (countFields > 1)
										{
											fixTrans2d[0] = fields[0].toDouble();
											fixTrans2d[1] = fields[1].toDouble();
										}
									}

									file2q.close();
								}
								//wstaw
								if (count > 0)
								{
									int* CompressedMask2 = CompressMaskExternal(mask);

									ByteMask* newMask = new ByteMask(CompressedMask2);//tez tego nie skasujê
									newMask->SetMaskName(name);
									//newMask->SetColorIndex(colIndex);
									CompressedMaskList->InsertItem(newMask);
									CompressedMask2 = NULL;//nie kasujê bo nie moge - przepisany wskaznik w compre

									//CompressMask(mask);
								}



								delete[]mask;
								delete[]tmpMask;
							}
							delete[]compressedMask;
						}


					}
				}
			}

		}
		else
		{

			plik = fopen(filename1.toLocal8Bit(), "rt");
			if (plik == NULL) return false;
			fscanf(plik, "%d %d ", &VolSize, &Size);
			fclose(plik);


			if (VolSize != VolumeSize) return false;
			plik = fopen(filename2.toLocal8Bit(), "rb");
			int *CompressedMask = new int[Size];
			fread(CompressedMask, sizeof(int), Size, plik);
			fclose(plik);
			////////////////////////////////////////////////////
			CompressedMaskList->InsertItem(new ByteMask(CompressedMask, filename2));
		}

		if (CompressedMaskList->GetCurrentListItem()!=CompressedMaskList->GetTailListItem())
		{
			CompressedMaskList->DeleteAllItemsAfterCurrent();
		}
		else
		{
			if (CompressedMaskList->GetNumberOfItems()>NumberOfMaxItems)
			{
				//jak przekroczylismy liczbe dopuszczalna trzeba skasowac pierwszy
				CompressedMaskList->DeleteItem(0);	
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------

bool	ByteMaskCollection::LoadMaskDataSetAndCombineWithCurrent( const QString &filename,int BoolMode,unsigned char *CurrentMask, RawDataSet *data )
{

	if(!filename.isEmpty())
	{

		int VolSize,Size;

	//	const  char *name1;	
	//	const  char *name2;
		QString filename1,filename2;	
		FILE *plik;


		filename2=filename;
		if (filename2.lastIndexOf(".mask")==-1)
			filename2+=".mask";
		//name1=filename2.toLocal8Bit();
		filename1=filename2+".header";
		//name2=filename1.toLocal8Bit();	



		plik = fopen(filename1.toLocal8Bit() , "rt");
		if (plik==NULL) return false;	
		fscanf(plik,"%d %d ",&VolSize,&Size);
		fclose(plik);

		if (VolSize!=VolumeSize) return false;


		plik = fopen(filename2.toLocal8Bit(), "rb");

		int *CompressedMask=new int[Size];

		fread( CompressedMask,sizeof(int),Size,plik);
		fclose(plik);


		////////////////////////////////////////////////////

		if ((CompressedMaskList->GetNumberOfItems()>0)&&(CurrentMask!=NULL))
		{

			unsigned char *NewMask=DecompressMask(CompressedMask);

			int i;
			for (i=0;i<VolSize;i++)
			{
				NewMask[i]=BoolOperation(BoolMode,NewMask[i],CurrentMask[i]);
			}





			delete []CompressedMask;
			CompressedMask=CompressMaskExternal(NewMask);


			delete []NewMask;

		}

		/// to tak jak normalnie po staremu 
		CompressedMaskList->InsertItem(new ByteMask(CompressedMask));

		if (CompressedMaskList->GetCurrentListItem()!=CompressedMaskList->GetTailListItem())
		{
			CompressedMaskList->DeleteAllItemsAfterCurrent();
		}
		else
		{
			if (CompressedMaskList->GetNumberOfItems()>NumberOfMaxItems)
			{
				//jak przekroczylismy liczbe dopuszczalna trzeba skasowac pierwszy
				CompressedMaskList->DeleteItem(0);	
			}
		}


		////////////////////////////////////////////////////






		return true;

	}
	else
	{
		return false;
	}



}
//--------------------------------------
bool	ByteMaskCollection::LoadMasks(const QString &filename, RawDataSet *data , bool fix2D , QString fix2DPath )
{
	if ((!filename.isEmpty()))
	{
		QString filename1, filename2;
		filename2 = filename;
		if (filename2.lastIndexOf(".maskSet") == -1)
			filename2 += ".maskSet";
		filename1 = filename2 + ".header";
		QFile file(filename1);
		if (file.open(QIODevice::ReadOnly))
		{
			int n[3] = { 0 };
			double s[3], o[3];
			//data->GetN(n);
		//	data->GetScale(s);
			//data->GetOrigin(o);
			int count;
			int vSize = 0;
			QTextStream stream(&file);

			QString line;//=stream.readLine();
			QStringList fields;// = line.split( " ");
			int size;
			QString name;
			QFile file2(filename2);
			int test[4] = { 0 };
			if (!file2.open(QIODevice::ReadOnly))
			{
				file.close();
				return false;
			}
			do
			{
				line = stream.readLine();
				if (QString::compare(line, QString("VolumeSize:")) == 0)
				{
					line = stream.readLine();
					fields = line.split(" ", QString::SkipEmptyParts);
					int count = fields.count();
					vSize = fields[0].toInt();
				
					//if (vSize != VolumeSize)
					//{

					//	//zakoncz
					//	file.close();
					//	return false;
					//}


				}
				else
					if (QString::compare(line, QString("Size:")) == 0)
					{
						line = stream.readLine();
						size = line.toInt();
						line = stream.readLine();
						name = stream.readLine();
						line = stream.readLine();
						int colIndex = stream.readLine().toInt();
						if (vSize==0) vSize = n[0]*n[1]*n[2];

						if ((size > 0) && (name.isEmpty()==false))
						{



							if (vSize != VolumeSize)
							{
								if (data != NULL)
								{
									//ró¿ne rozmiary 
									int n1[3] = { 0 };
									double s1[3], o1[3];
									data->GetN(n1);
									data->GetScale(s1);
									data->GetOrigin(o1);

									double tol = 1e-6;
									/*if (
									(fabs(s[0] - s1[0]) < tol) &&
									(fabs(s[1] - s1[1]) < tol) &&
									(fabs(s[2] - s1[2]) < tol)
									)*/
									{
										//skale takie same moge obsluzyc!!!!


										//QFile file2(filename2);
										//if (file2.open(QIODevice::ReadOnly))
										{
											int *compressedMask = new int[size];
											if (file2.read((char*)compressedMask, size * sizeof(int)))
											{
												//ByteMask *newMask = new ByteMask(CompressedMask);
												//maskaWczytana - trzeba ja rozkompresowaæ do tymczasowego rozmiaru!!!

												double scaleInv[3] = { 1.0 / s[0],1.0 / s[1],1.0 / s[2] };

												RawDataSet *tmpData = new RawDataSet(n[0], n[1], n[2], s[0], s[1], s[2], 0, false);
												unsigned char *tmpMask = new unsigned char[tmpData->GetVolumeSize()];
												DecompressMask(compressedMask, tmpMask);
												unsigned char *mask = new unsigned char[data->GetVolumeSize()];
												memset(mask, 0, data->GetVolumeSize());
												double tmpPos[3];
												int tP[3];
												int pos = 0;
												int count = 0;
												for (int k = 0; k < n1[2]; k++)
												{
													tmpPos[2] = (k*s1[2] + o1[2] - o[2])*scaleInv[2];
													tP[2] = Round(tmpPos[2]);
													for (int j = 0; j < n1[1]; j++)
													{
														tmpPos[1] = (j*s1[1] + o1[1] - o[1])*scaleInv[1];
														tP[1] = Round(tmpPos[1]);
														for (int i = 0; i < n1[0]; i++)
														{
															tmpPos[0] = (i*s1[0] + o1[0] - o[0])*scaleInv[0];
															tP[0] = Round(tmpPos[0]);

															if (
																(tP[0] >= 0) && (tP[0] < n[0] - 1) &&
																(tP[1] >= 0) && (tP[1] < n[1] - 1) &&
																(tP[2] >= 0) && (tP[2] < n[2] - 1)
																)
															{
																int tmp = tmpMask[tmpData->GetVoxelPos(tP[0], tP[1], tP[2])];
																mask[pos] = tmp;
																if (tmp > 0) count++;
															}
															pos++;
														}




													}
												}


												//teraz trzeba skompresowac i wstawic now¹

												if (fix2D == true)
												{
													count = 0;
													unsigned char* mask2 = new unsigned char[data->GetVolumeSize()];
													memset(mask2, 0, data->GetVolumeSize());

													pos = 0;
													double fixTrans2d[2] = { 0 };
													QFile file2q(fix2DPath);
													if (file2q.open(QIODevice::ReadOnly))
													{
														QTextStream stream(&file2q);//bo sprawdzilem wczesniej
														QString line = stream.readLine();
														QStringList fields  = line.split(QRegExp(" |\t|:"), QString::SkipEmptyParts);
														int countFields = fields.count();
														if (countFields > 1)
														{
															fixTrans2d[0] = fields[0].toDouble();
															fixTrans2d[1] = fields[1].toDouble();
														}

														for (int k = 0; k < n1[2]; k++)
														{
															tP[2] = k;



															for (int j = 0; j < n1[1]; j++)
															{
																tP[1] = j - fixTrans2d[1];

																for (int i = 0; i < n1[0]; i++)
																{

																	tP[0] = i - fixTrans2d[0];

																	if (
																		(tP[0] >= 0) && (tP[0] < n1[0] ) &&
																		(tP[1] >= 0) && (tP[1] < n1[1] ) &&
																		(tP[2] >= 0) && (tP[2] < n1[2] )
																		)
																	{
																		int tmp = mask[data->GetVoxelPos(tP[0], tP[1], tP[2])];
																		mask2[pos] = tmp;
																		if (tmp > 0) count++;
																	}
																	pos++;

																	

																}




															}

															//readFix2D next values

															line = stream.readLine();
															fields = fields = line.split(QRegExp(" |\t|:"), QString::SkipEmptyParts);
															int countFields = fields.count();
															if (countFields > 1)
															{
																fixTrans2d[0] = fields[0].toDouble();
																fixTrans2d[1] = fields[1].toDouble();
															}
														}

														file2q.close();
													}

													//podmianka

													unsigned char *m = mask;
													mask = mask2;
													delete[]m;



												}
											


												if (count > 0) 
												{
													int *CompressedMask2 = CompressMaskExternal(mask);
													
													ByteMask *newMask = new ByteMask(CompressedMask2);//tez tego nie skasujê
													newMask->SetMaskName(name);
													newMask->SetColorIndex(colIndex);
													CompressedMaskList->InsertItem(newMask);
													CompressedMask2 = NULL;//nie kasujê bo nie moge - przepisany wskaznik w compre
													
													//CompressMask(mask);
												}

												/*

												//jakos to trzeba ogarnac
													ByteMask *newMask = new ByteMask(CompressedMask);
									newMask->SetMaskName(name);
									CompressedMaskList->InsertItem(newMask);
												
												*/



												delete[]compressedMask;
												delete[]mask;
												delete[]tmpMask;
												delete tmpData;
												//delete newMask;
												//newMask->SetMaskName(name);
												//CompressedMaskList->InsertItem(newMask);
											}
											//file2.close();
										}



										

									}


								}


							}
							else
							{



								if (fix2D == false)
								{

									//wczytaj!
									int* CompressedMask = new int[size];
									if (file2.read((char*)CompressedMask, size * sizeof(int)))
									{
										ByteMask* newMask = new ByteMask(CompressedMask);
										newMask->SetMaskName(name);
										newMask->SetColorIndex(colIndex);
										CompressedMaskList->InsertItem(newMask);
										file2.close();

									}
									else
									{
										//zakoncz
										file.close();
										return false;
									}
								}
								else

								{
									int* compressedMask = new int[size];

									if (file2.read((char*)compressedMask, size * sizeof(int)))
									{
										unsigned char* tmpMask = new unsigned char[data->GetVolumeSize()];
										DecompressMask(compressedMask, tmpMask);
										unsigned char* mask = new unsigned char[data->GetVolumeSize()];
										memset(mask, 0, data->GetVolumeSize());
										//wczytaj
										//zdekompresuj
										//przesun
										// 
										// 
										// w tmpMaskSiedzi orginal do przesuniecia a finalna bedzie w mask 

										//przesuwanie maski!!

										int n1[3];
										data->GetN(n1);
										int count = 0;
										double fixTrans2d[2] = { 0 };
										int sliceSize = data->GetSliceSize();
										int tP[3];
										int pos = 0;
										//readFIx2d firstvalue

										QFile file2q(fix2DPath);
										if (file2q.open(QIODevice::ReadOnly))
										{
											QTextStream stream(&file2q);//bo sprawdzilem wczesniej
											QString line = stream.readLine();
											QStringList fields = line.split(QRegExp(" |\t|:"), QString::SkipEmptyParts);
											int countFields = fields.count();
											if (countFields > 1)
											{
												fixTrans2d[0] = fields[0].toDouble();
												fixTrans2d[1] = fields[1].toDouble();
											}

											for (int k = 0; k < n1[2]; k++)
											{
												tP[2] = k;



												for (int j = 0; j < n1[1]; j++)
												{
													tP[1] = j - fixTrans2d[1];

													for (int i = 0; i < n1[0]; i++)
													{

														tP[0] = i - fixTrans2d[0];

														if (
															(tP[0] >= 0) && (tP[0] < n1[0] ) &&
															(tP[1] >= 0) && (tP[1] < n1[1] ) &&
															(tP[2] >= 0) && (tP[2] < n1[2] )
															)
														{
															int tmp = tmpMask[data->GetVoxelPos(tP[0], tP[1], tP[2])];
															mask[pos] = tmp;
															if (tmp > 0) count++;
														}
														pos++;



													}




												}
												//readFix2D next values

												line = stream.readLine();
												fields = fields = line.split(QRegExp(" |\t|:"), QString::SkipEmptyParts);
												int countFields = fields.count();
												if (countFields > 1)
												{
													fixTrans2d[0] = fields[0].toDouble();
													fixTrans2d[1] = fields[1].toDouble();
												}
											}

											file2q.close();
										}
										//wstaw
										if (count > 0)
										{
											int* CompressedMask2 = CompressMaskExternal(mask);

											ByteMask* newMask = new ByteMask(CompressedMask2);//tez tego nie skasujê
											newMask->SetMaskName(name);
											newMask->SetColorIndex(colIndex);
											CompressedMaskList->InsertItem(newMask);
											CompressedMask2 = NULL;//nie kasujê bo nie moge - przepisany wskaznik w compre

											//CompressMask(mask);
										}


										
										delete[]mask;
										delete[]tmpMask;
									}
									delete[]compressedMask;

								}

							}





						}
						else
						{
							//zakoncz
							file.close();
							return false;
						}

					}
					else if (QString::compare(line, QString("SIZES:")) == 0)
					{
						line = stream.readLine();
						fields = line.split(" ", QString::SkipEmptyParts);
						count = fields.count();
						n[0] = fields[0].toInt();
						n[1] = fields[1].toInt();
						n[2] = fields[2].toInt();

						//if (Nx*Ny*Nz == 0) return false;

					}
					else
						if (QString::compare(line, QString("VOXEL_DIMS:")) == 0)
						{
							line = stream.readLine();
							fields = line.split(" ", QString::SkipEmptyParts);
							s[0] = fields[0].toDouble();
							s[1] = fields[1].toDouble();
							s[2] = fields[2].toDouble();

							//if (Xscale*Yscale*Zscale == 0) return false;
						}
						else
							if (QString::compare(line, QString("START_POS:")) == 0)
							{
								line = stream.readLine();
								fields = line.split(" ", QString::SkipEmptyParts);
								o[0] = fields[0].toDouble();
								o[1] = fields[1].toDouble();
								o[2] = fields[2].toDouble();
							}



			} while (!stream.atEnd());

			file2.close();
			file.close();
		}
		return true;
	}
	return false;
}
//-------------------
//--------------------------------------------------------
bool	ByteMaskCollection::SaveMasks(const QString &filename, RawDataSet *data )
{
	if ((!filename.isEmpty()) && (this->GetNumberOfMasks() > 0))
	{

		int Size;
		//const  char *name1;	
		//	const  char *name2;
		QString filename1, filename2;
		//FILE *plik;
		
		int *Data;

		for (int i = 0; i < GetNumberOfMasks(); i++)
		{
			/*if (maskNr == -1)
			{
				maskNr = CompressedMaskList->GetCurrentItemPos();
			}*/
			ByteMask *mask = CompressedMaskList->GetItemAtPos(i)->GetData();
			Data = mask->getMaskData();

			if (Data == NULL)  break;
			Size = *(Data + 1) + 4;




			filename2 = filename;
			if (filename2.lastIndexOf(".maskSet") == -1)
				filename2 += ".maskSet";
			//name1=filename2.toLocal8Bit();
			filename1 = filename2 + ".header";
			//name2=filename1.toLocal8Bit();		

			QFile file(filename1);


			if (i == 0)
			{
				//	plik = fopen(filename1.toLocal8Bit(), "w");
				file.open(QIODevice::WriteOnly);
			
			}
			else
				//plik = fopen(filename1.toLocal8Bit(), "a");
				file.open(QIODevice::Append);
			//fprintf(plik, "%d %d\n", VolumeSize, Size);
			//fclose(plik);
			QTextStream stream(&file);

			
			if (i == 0)
			{
				if (data == NULL)
				{
					stream << "VolumeSize:\n" << VolumeSize << "\n";
				}
				else
				{
					int n[3];
					double s[3], o[3];
					data->GetN(n);
					data->GetScale(s);
					data->GetOrigin(o);

					stream << "SIZES:\n" << n[0] << " " << n[1] << " " << n[2] << "\n\n";
					stream << "VOXEL_DIMS:\n" << s[0] << " " << s[1] << " " << s[2] << "\n\n";
					stream << "START_POS:\n" << o[0] << " " << o[1] << " " << o[2] << "\n\n";
					

				}
			}
			
			stream << "Size:\n" << Size <<  "\nNAME:\n" << mask->GetMaskName() << "\n";
			stream << "COLOR_INDEX:\n" << mask->GetColorIndex() << "\n";;
			file.close();


			QFile file2(filename2);
			if (i == 0)
				//plik = fopen(filename2.toLocal8Bit(), "wb");
				file2.open(QIODevice::WriteOnly);
			else
				file2.open(QIODevice::Append);
				//plik = fopen(filename2.toLocal8Bit(), "ab");

			//QTextStream stream(&file);
			//stream << "SIZES:\n" << Name << " " << Ny << " " << Nz << "\n\n";
			file2.write((const char*)Data, Size * sizeof(int));
			//fwrite(Data, sizeof(int), Size, plik);
		//	fclose(plik);
			file2.close();

		}
		
		return true;

	}
	return false;
}
//-----------------
bool	ByteMaskCollection::SaveMaskDataSet( const QString &filename,int maskNr, RawDataSet *data)
{
	
	if ((!filename.isEmpty()) && (maskNr < this->GetNumberOfMasks()))
	{
		int Size;
		//const  char *name1;	
	//	const  char *name2;
		QString filename1, filename2;
		FILE *plik;

		int *Data;
		if (maskNr == -1)
		{
			maskNr = CompressedMaskList->GetCurrentItemPos();
		}

		Data = CompressedMaskList->GetItemAtPos(maskNr)->GetData()->getMaskData();

		if (Data == NULL) return false;
		Size = *(Data + 1) + 4;




		filename2 = filename;
		if (filename2.lastIndexOf(".mask") == -1)
			filename2 += ".mask";
		//name1=filename2.toLocal8Bit();
		filename1 = filename2 + ".header";
		//name2=filename1.toLocal8Bit();		


		QFile file(filename1);

		if (data == NULL)
		{

			//stary styl
			plik = fopen(filename1.toLocal8Bit(), "w");
			fprintf(plik, "%d %d", VolumeSize, Size);
			fclose(plik);
		}
		else
		{
			//nowy styl;
			int n[3];
			double s[3], o[3];
			data->GetN(n);
			data->GetScale(s);
			data->GetOrigin(o);
			if (file.open(QIODevice::WriteOnly))
			{
				QTextStream stream(&file);

				QString line;//=stream.readLine();
				QStringList fields;// = line.split( " ");

				stream << "SIZES:\n" << n[0] << " " << n[1] << " " << n[2] << "\n\n";
				stream << "VOXEL_DIMS:\n" << s[0] << " " << s[1] << " " << s[2] << "\n\n";
				stream << "START_POS:\n" << o[0] << " " << o[1] << " " << o[2] << "\n\n";
				stream << "SIZE:\n" << Size;
				file.close();
			}
		}

		plik = fopen(filename2.toLocal8Bit(), "wb");
		fwrite( Data,sizeof(int),Size,plik);
		fclose(plik);


		CompressedMaskList->GetItemAtPos(maskNr)->GetData()->setPath(filename2);

		return true;


	}
	else
	{
		return false;
	}



}


//----------------------------------------	


int *ByteMaskCollection::CompressMaskExternal(unsigned char *Data)
{


	//tylko przypadek binarny ... inne narazie nie zaimplementowane!!


	//	int Step=262144;
	//	int Size=Step;

	int Size=(int)((double)VolumeSize*0.2);
	int Step=(int)((double)VolumeSize*0.05);

	int *TempVolume=new int[Size];//a to kwiatek !?! 2003.09.19 - poprawiam
	int *TempVolume2=NULL;
	int i;

	unsigned char Byte=Data[0];
	int	 NumberOfBytes=1;
	int	 TotalLength=2;
	unsigned char Temp=0;
	int pos=1;

	TempVolume[TotalLength++]=Byte;

	for(i=1;i<VolumeSize;i++)
	{
		if(Data[pos++]==Byte)	
		{
			NumberOfBytes++;
			Temp=0;
		}
		else
		{

			TempVolume[TotalLength++]=NumberOfBytes;

			if (TotalLength==Size)
			{

				//if (TempVolume2!=NULL) delete TempVolume2;
				TempVolume2=new int [Size+Step];

				memcpy( TempVolume2, TempVolume, sizeof(int)*(Size));

				Size+=Step;
				delete []TempVolume;
				TempVolume=TempVolume2;
				TempVolume2=NULL;

			}

			NumberOfBytes=1;
			Byte=!Byte;
			Temp=1;

		}
	}

	if (Temp==0)
	{

		TempVolume[TotalLength]=NumberOfBytes;
		NumberOfBytes=0;
		Byte=!Byte;
	}


	TempVolume[0]=0; //przypadek binarny
	TempVolume[1]=TotalLength-1; // trzeba podac jaka to ma dlugosc
	int *CompressedMask=new int[TotalLength+3];


	memcpy( CompressedMask, TempVolume, sizeof(int)*(TotalLength+1));
	delete []TempVolume;


	return CompressedMask;


}

//-----------------------------------------


void	ByteMaskCollection::CompressMask(unsigned char *Data)
{

	int *CompressedMask=CompressMaskExternal(Data);

	//tutaj trzeba bedzie poinsertowac
	//	return CompressedMask;
	CompressedMaskList->InsertItem(new ByteMask(CompressedMask));

	if (CompressedMaskList->GetCurrentListItem()!=CompressedMaskList->GetTailListItem())
	{

		//przypadek w ktorym trzeba cos bedzie kombinowac
		CompressedMaskList->DeleteAllItemsAfterCurrent();
		//jak bylem gdzies indziej niz na koncu listy to po wstawieniu nowego elementu
		//element ten staje sie ostatnim (trzeba reszte wywalic)



	}
	else
	{
		if (CompressedMaskList->GetNumberOfItems()>NumberOfMaxItems)
		{
			//jak przekroczylismy liczbe dopuszczalna trzeba skasowac pierwszy
			CompressedMaskList->DeleteItem(0);	
		}
	}





}




//----------------------------------------
unsigned char	*ByteMaskCollection::DecompressMask(int * tab)
{

	unsigned char *Mask=new unsigned char[VolumeSize];


	int *Data=tab;//=CompressedMaskList->GetCurrentListItem()->GetData();

	int i;
	unsigned char byte;
	int temp=0;
	int temp1=0;

	int Mode=*(Data);
	int length=*(Data+1);

	byte=*(Data+2);


	temp1+=3;

	if (Mode==0)
	{

		//przypadek binarny
		for(i=1;i<length;i++)
		{		
			memset(Mask+temp,byte,*(Data+temp1));
			temp+=*(Data+temp1);
			temp1++;
			byte=!byte;

		}
	}
	else
	{

		//przypadek inny
	}




	return Mask;

}


void ByteMaskCollection::DecompressMask(int * tab, unsigned char *destMask)
{

	//unsigned char *Mask = new unsigned char[VolumeSize];
	if ((tab != NULL) && (destMask != NULL))
	{
		int *Data = tab;//=CompressedMaskList->GetCurrentListItem()->GetData();

		int i;
		unsigned char byte;
		int temp = 0;
		int temp1 = 0;

		int Mode = *(Data);
		int length = *(Data + 1);

		byte = *(Data + 2);


		temp1 += 3;

		if (Mode == 0)
		{

			//przypadek binarny
			for (i = 1; i < length; i++)
			{
				memset(destMask + temp, byte, *(Data + temp1));
				temp += *(Data + temp1);
				temp1++;
				byte = !byte;

			}
		}
		else
		{

			//przypadek inny
		}


	}

	//return Mask;
}


void ByteMaskCollection::DecompressMask(unsigned char *destMask)
{
	DecompressMask(CompressedMaskList->GetCurrentListItem()->GetData()->getMaskData(),destMask);

}

unsigned char	*ByteMaskCollection::DecompressMask()
{
	return DecompressMask(CompressedMaskList->GetCurrentListItem()->GetData()->getMaskData());
}
unsigned char	*ByteMaskCollection::GetCurrentMaskData()
{
	return DecompressMask();
}
//------------------------------------------
void ByteMaskCollection::GetCurrentMaskData(unsigned char *maskDest)
{
	DecompressMask(maskDest);
}

bool	ByteMaskCollection::UndoPossible()
{
	return (CompressedMaskList->GetCurrentListItem()==CompressedMaskList->GetCurrentListItem()->PreviousListItem)?0:1;
}

//----------------------------------------

bool	ByteMaskCollection::RedoPossible()
{

	return (CompressedMaskList->GetCurrentListItem()==CompressedMaskList->GetCurrentListItem()->NextListItem)?0:1;
}

//----------------------------------------

unsigned char	*	ByteMaskCollection::UndoMask()
{
	if (UndoPossible())	
	{
		CompressedMaskList->SetCurrentListItem(CompressedMaskList->GetCurrentListItem()->PreviousListItem);
		return DecompressMask();
	}
	else
	{
		return NULL;
	}

}
//---------------
void ByteMaskCollection::UndoMask(unsigned char *mask)
{
	if (UndoPossible()&&(mask!=NULL))
	{
		CompressedMaskList->SetCurrentListItem(CompressedMaskList->GetCurrentListItem()->PreviousListItem);
		 DecompressMask(mask);
	}


}
//---------------
unsigned char	*	ByteMaskCollection::GetMask()
{
	return DecompressMask();

}
//----------
//---------------
void	ByteMaskCollection::GetMask(unsigned char *mask)
{
	 DecompressMask(mask);

}
//----------
ByteMask		*ByteMaskCollection::GetMaskDataAtPos(int pos)
{
	int nr=this->GetNumberOfMasks();
	if ((pos>=0)&&(pos<nr))
	{
		
		return this->CompressedMaskList->GetItemAtPos(pos)->GetData();
	}
	else
	{
		return NULL;
	}
	
	return NULL;
}
//----------------------------------------
unsigned char	*ByteMaskCollection::GetMaskDataAtPosAndSetAsCurrent(int pos)
{
	if (this->CompressedMaskList->GetNumberOfItems()>pos)
	{
		this->CompressedMaskList->SetCurrentItem(pos);
		return DecompressMask();
	}
	else 
	{
		return NULL;
	}
}

void ByteMaskCollection::GetMaskDataAtPosAndSetAsCurrent(int pos, unsigned char *maskDest)
{
	if ((this->CompressedMaskList->GetNumberOfItems()>pos)&&(maskDest!=NULL))
	{
		this->CompressedMaskList->SetCurrentItem(pos);
		 DecompressMask(maskDest);
	}

}
//-----------------------------
void ByteMaskCollection::RedoMask(unsigned char *mask)
{



	if (RedoPossible()&&(mask!=NULL))	
	{
		CompressedMaskList->SetCurrentListItem(CompressedMaskList->GetCurrentListItem()->NextListItem);
		 DecompressMask(mask);
	}


}
//------------------------------------------
//-----------------------------


int		*ByteMaskCollection::GetCurrentMask()
{
	return CompressedMaskList->GetCurrentListItem()->GetData()->getMaskData();
}
