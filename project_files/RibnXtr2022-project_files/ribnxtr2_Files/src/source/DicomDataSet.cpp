
#include "DicomDataSet.h"


DicomDataSet::DicomDataSet()
{
	Init=false;
	Associated=false;
	NumberOfSlices=-1;
	SelectedData=false;


	char test[64] = { '\0' };

	memcpy(Finfo.chStudyDescription, test, 64);
	memcpy(Finfo.chSeriesDescription, test, 64);


}

//---------------------------------------------------------------

DicomDataSet::DicomDataSet(const QString &filename,bool ShInfo)
{
	////cout << "in";
	Init=false;
	Associated=false;

	SetNewFile(filename);
	if (ShInfo==true) ShowInfo();
//	//cout <<"out\n";
}

//---------------------------------------------------------------

void DicomDataSet::SetNewFile(const QString &filename)
{

	//if (IsDicomFile(filename.toLocal8Bit()))
	if (IsDicomFile(filename))
	{


		FileName=filename;

		MinDisplayIntensity = 0, MaxDisplayIntensity = 4095;
		little_endian=LittleEndianTest();


		bool     i;

		QFileInfo check_file(filename);
		// check if file exists and if yes: Is it really a file and no directory?
		if (!(check_file.exists() && check_file.isFile()))
		//	return;
		//if ( (Fin = fopen(FileName.toLocal8Bit().constData(),"r"))  == NULL )
		{ 
			QMessageBox::about(0,"","Something wrong with the %s !\n"+filename);


		}
		else
		{

		/*	if ((FileName.lastIndexOf(".gz")!=-1)||(FileName.lastIndexOf(".GZ")!=-1))
			{

				unsigned char  *TmpBuff=new unsigned char[4194304];


				gzFile Zipek;
				Zipek=gzopen (FileName.toLocal8Bit(),"rb"); 
				lDataLength =gzread(Zipek,TmpBuff,4194304);
				gzclose (Zipek); 

				Header = (unsigned char *)calloc( sizeof(* Header), lDataLength);
				memcpy(Header,TmpBuff,lDataLength);

				delete []TmpBuff;




			}
			else
			{

				fseek(Fin,0L,2); 
				lDataLength = (long) ftell(Fin);
				Header = (unsigned char *)calloc( sizeof( Header), lDataLength);
				fseek(Fin,0L,0);
				fread(Header,sizeof(* Header),lDataLength,Fin);

			}*/

			///fclose(Fin);
			////cout <<"a";
			InitEmptyInfo();
			//i = DicomInf.ExtractDicomFileInfo( Header, lDataLength, Finfo );
			i=DicomInf.ExtractDicomFileInfo(filename.toLocal8Bit().constData(), Finfo );
			////cout <<"e";
			i=true;
			if (Finfo.fPixelSpacing[0]==0.0) Finfo.fPixelSpacing[0]=1.0;
			if (Finfo.fPixelSpacing[1]==0.0) Finfo.fPixelSpacing[1]=1.0;
			if (Finfo.fSliceThickness==0.0) Finfo.fSliceThickness=1.0;
			if (Finfo.fSpacingBetweenSlices==0.0) Finfo.fSpacingBetweenSlices=1.0;
				////cout <<"b";
			//Finfo.uPixelRepresentation=1;
		
			//cout <<"X\t" << Finfo.iImageNumber << "\t" << Finfo.vImagePosition[0] << "\t" << Finfo.vImagePosition[1] << "\t" << Finfo.vImagePosition[2] << "\n";

			//free(Header);
			if 	(i==true)
				Init=true;

		}

	}

	if (Init==true) GetID();
	////cout <<"c";

}


//---------------------------------------------------------------
//---------------------------------------------------------------

void DicomDataSet::GetPixelData(unsigned short *Q,int nr,bool inv,bool shift,int shiftVal, int globalMinValue)
{


	int     i;

	int MinValue, MaxValue=0;

		gdcm::ImageReader reader;
		reader.SetFileName(FileName.toLocal8Bit().constData());
		reader.Read();
		const gdcm::Image &image = reader.GetImage();
		//image.Print(std::cout);

		double sl = 1.0 / Finfo.fRescaleSlope;
		double in = Finfo.fRescaleIntercept;
		int ssize = Finfo.iRows*Finfo.iColumns;
		//Q = (signed short *)calloc(sizeof(*Q), volumesize);
		
//		int min;
		MinValue = in;
		if (Finfo.uPixelRepresentation == 1)
		{
			int bsize = image.GetBufferLength()/2;
			if (bsize <= ssize)
			{
				short *sTab = new  short[ssize];

			
				(short *)image.GetBuffer((char*)sTab);
				//iData->GetScalarPointer();
				//cast->SetOutputScalarTypeToShort();
				short *tab = (short *)Q;

				int mVal = 0;
				int val;
				for (int i = 0; i < ssize; i++)
				{
					val = sTab[i] * sl + in;
					if (val > mVal)mVal = val;
					tab[i] = val;
				}
				delete[]sTab;
			}

			

		}
		else
		{//
			//cast->SetOutputScalarTypeToUnsignedShort();
			int bsize = image.GetBufferLength()/2;
			if (bsize <= ssize*2)
			{
				unsigned short *sTab = new unsigned short[ssize * 2];
				(unsigned short  *)image.GetBuffer((char*)sTab);
				unsigned short *tab = (unsigned short *)Q;
				//			  minx=65535;
				for (int i = 0; i < ssize; i++)
				{
					int val = sTab[i];// *sl - in;
					tab[i] = val;// (unsigned short)(val*sl + in);
				}

				delete[]sTab;
			}


		}

		bool tt=Finfo.uPixelRepresentation;
		if (inv==true) tt=!tt;
		if  (tt==1)
			//if (Temp[0]->Finfo.uPixelRepresentation==1)

		{
				signed short	*Q2=(signed	short *) Q;
				
				
				

					MinValue = MaxValue = Q2[0];

					int	i;
					for (i = 0; i<ssize; i++)
					{
						//if (MaxValue<Q2[i]) MaxValue=Q2[i];
						//else if 
						if (MinValue>Q2[i]) MinValue = Q2[i];
					}
				


				if (globalMinValue != 0)
				{
					if (globalMinValue != MinValue)
					{

						MinValue = globalMinValue;
					}
				}
	
				if (MinValue < 0)

				{
					int mmax = -1000000;
					int mmin = 1000000;
					int minq2, maxq2;
					int val;
					for (i = 0; i < ssize; i++)
					{
						val = Q2[i] - MinValue;
						if (val < 0) val = 0;
						Q[i] =val ;
						
						if (val < mmin) { mmin = val; minq2 = Q2[i]; };
						if (val > mmax) { mmax = val; maxq2 = Q2[i];};
					}
					mmin = mmin;
				
				}
		}
			 	 if (shift==true)
				 {
					 unsigned short *tt=(unsigned short *)Q;
					int  min;
					if (shiftVal==0)
					{
					 min=65535;
					 for(int i=1;i<ssize;i++)
					 {
						

						 if ((tt[i]<min)&&(tt[i]>0)) min=tt[i];
						// if (tab[i]>maxx) maxx=tab[i];
					 }
					}
					else
					{
						min=shiftVal;
					}
			
					 if (min>1)
					 {
						 int val;
						 for (int i=0;i<ssize;i++)
						 {	
							 val=(tt[i]-min);
							 tt[i]=(val>0)?val:0;
						 }
						// dataset->CalculateHistogram();
					 }
				 }
		
				///////////////
				//GetPixelData((unsigned short*)Q,i,Header);
			//	memcpy(Q,iData->GetScalarPointer(),Finfo.iRows*Finfo.iColumns*sizeof(unsigned short));
				 Finfo.minValue = MinValue;

			Qmin = 100000000;  Qmax = -100000000;

			for (i = 0; i < Finfo.iRows*Finfo.iColumns; i++)  
			{
				if (Qmin > Q[i]) Qmin = Q[i]; 
				if (Qmax < Q[i]) Qmax = Q[i];
			}

		//	cout <<  " max: " << Qmax ;
			if ((Finfo.fWindowCenter == 0.0) && (Finfo.fWindowWidth == 0.0))
			{

				Finfo.fWindowCenter = (double)(Qmin + (Qmax - Qmin)/2.0); 
				Finfo.fWindowWidth  = (double)(Qmax - Qmin);
			}





			//free(Header);


}

//----------------------
//void DicomDataSet::GetPixelData(unsigned short *Q,int nr,unsigned char *Header)
//{
//
//
//	//int offset=Finfo.lPixelDataOffset+nr*Finfo.RealDataLength;
//
//	//	int     i;
//
//
//	//if (Finfo.uBitsAllocated == 16)
//	//{
//	//	for (i = 0; i < Finfo.iRows*Finfo.iColumns*Finfo.iSlices; i++)  
//	//	{
//
//	//		if (little_endian == 1)
//	//	 {
//	//		 tag.byte[0] = Header[offset + 2*i     ];
//	//		 tag.byte[1] = Header[offset + 2*i + 1 ];
//	//		 Q[i] = (unsigned short)tag.word;
//
//	//		 // Q[i] = (unsigned short)*(Header + Finfo.lPixelDataOffset + 2*i); 
//	//	 }
//	//		else  // SWAP BYTES
//	//		{
//	//			tag.byte[1] = Header[offset + 2*i     ];
//	//			tag.byte[0] = Header[offset + 2*i + 1 ];
//	//			Q[i] = (unsigned short)tag.word;
//	//		}
//	//	}
//	//}
//	//else
//	//	if (Finfo.uBitsAllocated == 8)
//	//		for (i = 0; i < Finfo.iRows*Finfo.iColumns*Finfo.iSlices; i++)  
//	//		{
//	//			Q[i] = (unsigned short)Header[offset + i]; 
//
//	//		}
//	//	else  
//	//		if (Finfo.uBitsAllocated == 0)
//	//		{
//	//			if ( Finfo.lPixelDataLength / (Finfo.iRows*Finfo.iColumns) == 1 )
//	//				for (i = 0; i < Finfo.iRows*Finfo.iColumns; i++)  
//	//				{
//	//					Q[i] = (unsigned short)Header[offset + i]; 
//	//				}
//	//			else  
//	//				if ( Finfo.lPixelDataLength / (Finfo.iRows*Finfo.iColumns) == 2 )
//	//					for (i = 0; i < Finfo.iRows*Finfo.iColumns; i++)  
//	//					{
//	//						tag.byte[0] = Header[offset + 2*i    ];
//	//						tag.byte[1] = Header[offset + 2*i +1 ];
//	//						Q[i] = (unsigned short)tag.word;
//	//					}
//	//				else
//	//				{}
//	//		}
//	//		else
//	//		{}
//
//	//		//
//	//		Qmin = 100000000;  Qmax = -100000000;
//
//	//		for (i = 0; i < Finfo.iRows*Finfo.iColumns; i++)  
//	//		{
//	//			if (Qmin > Q[i]) Qmin = Q[i]; 
//	//			if (Qmax < Q[i]) Qmax = Q[i];
//	//		}
//
//
//	//		if ((Finfo.fWindowCenter == 0.0) && (Finfo.fWindowWidth == 0.0))
//	//		{
//
//	//			Finfo.fWindowCenter = (double)(Qmin + (Qmax - Qmin)/2.0); 
//	//			Finfo.fWindowWidth  = (double)(Qmax - Qmin);
//	//		}
//
//
//
//
//
//	//		//free(Header);
//
//
//}
RawDataSet* DicomDataSet::GetRawDataSet2(bool inv , bool shift , int shiftVal )
{


	if (Init == true)
	{
		signed short       *Q;

		int volumesize = Finfo.lPixelDataLength;

		int numberofsubvolumes = (int)(Finfo.lPixelDataLength / volumesize);
		int i;

		int MinValue = 0;
		
			i = 0;

			gdcm::ImageReader reader;
			reader.SetFileName(FileName.toLocal8Bit().constData());
			reader.Read();
			const gdcm::Image &image = reader.GetImage();
			image.Print(std::cout);



			//iData->Update();

			double sl = 1.0 / Finfo.fRescaleSlope;
			double in = Finfo.fRescaleIntercept;
			int ssize = Finfo.iRows*Finfo.iColumns;
			Q = (signed short *)calloc(sizeof(*Q), volumesize);
			int minx, maxx;
			int min, max;
			if (Finfo.uPixelRepresentation == 1)
			{
				int bsize = image.GetBufferLength()/2;
				if (bsize <= ssize)
				{
					short *sTab = new  short[ssize];

					(short *)image.GetBuffer((char*)sTab);
					//iData->GetScalarPointer();
					//cast->SetOutputScalarTypeToShort();
					short *tab = (short *)Q;





					for (int i = 0; i < ssize; i++)
					{
						tab[i] = sTab[i] * sl + in;
					}








					MinValue = tab[0];

					for (int i = 0; i < ssize; i++)
					{
						//if (MaxValue<Q2[i]) MaxValue=Q2[i];
						//else if 
						if (MinValue > tab[i]) MinValue = tab[i];
					}




					delete[]sTab;
				}

			}
			else
			{//
				//cast->SetOutputScalarTypeToUnsignedShort();
				int bsize = image.GetBufferLength()/2;
				if (bsize <= ssize)
				{
					unsigned short *sTab = new unsigned short[ssize];
					(unsigned short  *)image.GetBuffer((char*)sTab);
					unsigned short *tab = (unsigned short *)Q;
					//			  minx=65535;
					for (int i = 0; i < ssize; i++)
					{
						int val = sTab[i];
						tab[i] = val;// (unsigned short)(val*sl + in);
					}

					delete[]sTab;
				}

			}

			Finfo.minValue = MinValue;

			///////////////
			//GetPixelData((unsigned short*)Q,i,Header);
			//memcpy(Q,iData->GetScalarPointer(),Finfo.iRows*Finfo.iColumns*sizeof(unsigned short));


			/////////////////
			RawDataSet *dataset = new RawDataSet(Finfo.iColumns, Finfo.iRows, Finfo.iSlices, Finfo.fPixelSpacing[1], Finfo.fPixelSpacing[0], 1.0, 0.0, true);
			dataset->SetDataArray((unsigned short *)Q);
			//if (Finfo.uPixelRepresentation==1)

			bool tt = Finfo.uPixelRepresentation;
			if (inv == true) tt = !tt;
			if (tt == 1)
				dataset->ConvertFromSignedData();//true,-Finfo.fRescaleIntercept);

			if (shift == true)
			{
				unsigned short *tt = dataset->GetDataArray();
				if (shiftVal == 0)
				{
					min = 65535;
					for (int i = 1; i<ssize; i++)
					{


						if ((tt[i]<min) && (tt[i]>0)) min = tt[i];

					}
				}
				else
				{
					min = shiftVal;
				}
				if (min>1)
				{
					int val;
					for (int i = 0; i<ssize; i++)
					{
						val = (tt[i] - min);
						tt[i] = (val>0) ? val : 0;
					}
					emit sendShiftValue(QString::number(min));
					dataset->CalculateHistogram();
				}
			}


			Q = NULL;

			QString nn = FileName;
			int	pos = nn.lastIndexOf("/");
			if (pos >= 0)
				nn.remove(0, pos + 1);


			nn += "_";
			if (i<100) nn += "0";
			if (i<10) nn += "0";
			nn += QString::number(i);
			dataset->SetRawDataSetName(nn);



		/*	int wcenter = Finfo.fWindowCenter;
			if (Finfo.minValue<0) wcenter -= Finfo.minValue;
			int wwidth = Finfo.fWindowWidth;

			int minVal = wcenter - wwidth / 2;
			int maxVal = wcenter + wwidth / 2;

			dataset->dicomLeftThreshold = minVal;
			dataset->dicomRightThreshold = maxVal;*/


			dataset->dicomLevelValue = Finfo.fWindowCenter;
			dataset->dicomWindowWidthValue = Finfo.fWindowWidth;
			if (tt == 1)
			{
				dataset->dicomMinValue = MinValue;
			}
			else
			dataset->dicomMinValue = Finfo.fRescaleIntercept;
			dataset->dicomRescaleValue = Finfo.fRescaleSlope;
		
			return dataset;
		



	}
	else return NULL;
}
//----------------------------
void DicomDataSet::GetRawDataSet(bool inv,bool shift,int shiftVal)
{

	RawDataSet *datat = GetRawDataSet2(inv, shift, shiftVal);
	if (datat != NULL) emit SignalSendRawDataSet(datat);
	//if (Init==true)
	//{
	//	signed short       *Q;

	//	int volumesize=Finfo.lPixelDataLength;

	//	int numberofsubvolumes=(int)(Finfo.	lPixelDataLength/volumesize);
	//	int i;
	//

	//	{
	//		i=0;
	//	
	//		gdcm::ImageReader reader;
	//		reader.SetFileName(FileName.toLocal8Bit());
	//		reader.Read();
	//		const gdcm::Image &image = reader.GetImage();
	//		image.Print(std::cout);

	//	

	//		//iData->Update();
	//		
	//		double sl=1.0/Finfo.fRescaleSlope;
	//		double in=Finfo.fRescaleIntercept;
	//		int ssize=Finfo.iRows*Finfo.iColumns;
	//		Q = (signed short *)calloc( sizeof(* Q), volumesize);
	//		int minx,maxx;
	//		int min,max;
	//		 if (Finfo.uPixelRepresentation==1)
	//		 {
	//			 short *sTab =new  short[ssize];

	//			 (short *)image.GetBuffer((char*)sTab);
	//				 //iData->GetScalarPointer();
	//			 //cast->SetOutputScalarTypeToShort();
	//			 short *tab=(short *)Q;

	//			 
	//			  for(int i=0;i<ssize;i++)
	//			 {
	//				 tab[i]=sTab[i]*sl+in;
	//			  }

	//			  delete[]sTab;
	//		
	//		 }
	//		 else
	//		 {//
	//			//cast->SetOutputScalarTypeToUnsignedShort();
	//			 unsigned short *sTab = new unsigned short[ssize];
	//			 (unsigned short  *)image.GetBuffer((char*)sTab);
	//			 unsigned short *tab=(unsigned short *)Q;
	//				//			  minx=65535;
	//			 for(int i=0;i<ssize;i++)
	//			 {
	//				 int val = sTab[i] ;
	//				 tab[i] = val;// (unsigned short)(val*sl + in);
	//			  }

	//			 delete[]sTab;
	//			
	//		 }


	//			
	//			///////////////
	//			//GetPixelData((unsigned short*)Q,i,Header);
	//			//memcpy(Q,iData->GetScalarPointer(),Finfo.iRows*Finfo.iColumns*sizeof(unsigned short));


	//			/////////////////
	//			RawDataSet *dataset=new RawDataSet( Finfo.iColumns,Finfo.iRows,Finfo.iSlices, Finfo.fPixelSpacing[1], Finfo.fPixelSpacing[0],1.0,0.0,true);
	//			dataset->SetDataArray((unsigned short *)Q);
	//			//if (Finfo.uPixelRepresentation==1)
	//			
	//			 bool tt=Finfo.uPixelRepresentation;
	//			if (inv==true) tt=!tt;
	//			 if  (tt==1)
	//				dataset->ConvertFromSignedData();//true,-Finfo.fRescaleIntercept);
	//		

	//			 if (shift==true)
	//			 {
	//				 unsigned short *tt=dataset->GetDataArray();
	//				if (shiftVal==0)
	//				{
	//				 min=65535;
	//				 for(int i=1;i<ssize;i++)
	//				 {
	//					

	//					 if ((tt[i]<min)&&(tt[i]>0)) min=tt[i];
	//				
	//				 }
	//				}
	//				else
	//				{
	//					min=shiftVal;
	//				}
	//				 if (min>1)
	//				 {
	//					 int val;
	//					 for (int i=0;i<ssize;i++)
	//					 {	
	//						 val=(tt[i]-min);
	//						 tt[i]=(val>0)?val:0;
	//					 }
	//					 emit sendShiftValue(QString::number(min));
	//					 dataset->CalculateHistogram();
	//				 }
	//			 }


	//			Q=NULL;

	//			QString nn=FileName;
	//			int	pos=nn.lastIndexOf("/");
	//			if (pos>=0)
	//			nn.remove(0,pos+1);


	//			nn+="_";
	//			if(i<100) nn+="0";
	//			if(i<10) nn+="0";
	//			nn+=QString::number(i);
	//			dataset->SetRawDataSetName(nn);


	//			emit SignalSendRawDataSet(dataset);

	//	}

	//
	//
	//}
	//

	

}

//---------------------------------------------------------------

bool DicomDataSet::LittleEndianTest() 
/* Little or Big Endian ? */
{
	union
	{
		long l;
		char c[sizeof (long)];
	} u;

	u.l = 1;

	return ( u.c[sizeof (long) - 1] == 0 );

}


//---------------------------------------------------------------

void DicomDataSet::ShowInfo()
{

	if (Init==true)
	{
		QString text="";

		text+="# FILE_INFO_RECORD:      " + FileName+  "\n"
			+ "# DataLength:            " + QString::number(lDataLength) + " bytes ("+ QString::number(lDataLength/1024) + " kB)\n\n";

		if (DicomInf.m_bPART10_PREAMBLE) 
			text+="# Part 10 preamble !   ---------------------------------------------------\n"; 
		text+= "# StudyDate            = "	+QString::fromLatin1 ( Finfo.chStudyDate)    + "\n"; 
		text+= "# Modality             = "	+QString::fromLatin1( Finfo.chModality)     + "\n"; 
		text+= "# Manufacturer         = "	+QString::fromLatin1( Finfo.chManufacturer) + "\n"; 
		text+= "# PatientName          = "	+QString::fromLatin1( Finfo.chPatientName)  + "\n"; 
		text+= "# FrameOfReferenceUID  = "	+QString::fromLatin1( Finfo.chFrameOfReferenceUID) + "\n"; 
		text+= "# PatientID            = "	+QString::fromLatin1( Finfo.chPatientID)    + "\n"; 
		text+= "# StudyID              = "	+QString::fromLatin1( Finfo.chStudyID)      + "\n"; 
		text+= "# SeriesNumber         = "	+QString::number( Finfo.iSeriesNumber)  + "\n"; 
		text+= "# ImageNumber          = "	+QString::number( Finfo.iImageNumber)   + "\n"; 
		text+= "# PatientAge		   = "  +QString::fromLatin1( Finfo.chPatientAge) + "\n"; 
		text+= "# PatientBirthDate		   = "  +QString::fromLatin1( Finfo.chPatientBirthDate) + "\n"; 
		text+= "# PatientSex		   = "  +QString::fromLatin1( Finfo.chPatientSex) + "\n"; 
		text+= "# PatientSize        = "	+QString::number( Finfo.fPatientSize)        + "\n"; 
		text+= "# PatientWeight        = "	+QString::number( Finfo.fPatientWeight)        + "\n"; 


		text+= "# Rows                 = "	+QString::number( Finfo.iRows)          + "\n"; 
		text+= "# Columns              = "	+QString::number( Finfo.iColumns)       + "\n"; 
		text+= "# ImagePosition        = "	+QString::number( Finfo.vImagePosition[0]) + " "
			+QString::number( Finfo.vImagePosition[1]) + " " 
			+QString::number( Finfo.vImagePosition[2]) + "\n"; 
		text+= "# ImageOrientation     = "	+QString::number( Finfo.vImageOrientation[0]) + " " 
			+QString::number( Finfo.vImageOrientation[1]) + " " 
			+QString::number( Finfo.vImageOrientation[2]) + " | " 
			+QString::number( Finfo.vImageOrientation[3]) + " "
			+QString::number( Finfo.vImageOrientation[4]) + " "
			+QString::number( Finfo.vImageOrientation[5]) + "\n"; 
		text+= "# SliceLocation        = "	+QString::number( Finfo.fSliceLocation)        + "\n"; 
		text+= "# PixelSpacing         = "	+QString::number( Finfo.fPixelSpacing[0])      + " "
			+QString::number( Finfo.fPixelSpacing[1])   + "\n"; 
		text+= "# SliceThickness       = "	+QString::number( Finfo.fSliceThickness)       + "\n"; 
		text+= "# SpacingBetweenSlices = "	+QString::number( Finfo.fSpacingBetweenSlices) + "\n"; 
		text+= "# GantryTilt           = "	+QString::number( Finfo.fGantryTilt)           + "\n"; 
		text+= "# FrameOfReferenceUID  = "	+QString::fromLatin1( Finfo.chFrameOfReferenceUID) + "\n"; 
		text+= "# RescaleIntercept     = "	+QString::number( Finfo.fRescaleIntercept)     + "\n"; 
		text+= "# RescaleSlope         = "	+QString::number( Finfo.fRescaleSlope)         + "\n";  
		text+= "# RescaleType          = "	+QString::fromLatin1( Finfo.chRescaleType)         + "\n"; 
		text+= "# WindowCenter         = "	+QString::number( Finfo.fWindowCenter)         + "\n"; 
		text+= "# WindowWidth          = "	+QString::number( Finfo.fWindowWidth)          + "\n"; 
		text+= "# BitsAllocated        = "	+QString::number( Finfo.uBitsAllocated)        + "\n"; 
		text+= "# BitsStored           = "	+QString::number( Finfo.uBitsStored)           + "\n"; 
		text+= "# HighBit              = "	+QString::number( Finfo.uHighBit)              + "\n"; 
		text+= "# PixelRepresentation  = "	+QString::number( Finfo.uPixelRepresentation)  + "\n"; 
		text+= "# TransferSyntaxUID    = "	+QString::fromLatin1( Finfo.chTransferSyntaxUID)   + "\n"; 
		text+= "# PixelDataOffset      = "	+QString::number( Finfo.lPixelDataOffset)      + "\n"; 
		text+= "# PixelDataLength      = "	+QString::number( Finfo.lPixelDataLength)      + "\n"; 

		text+= "# EchoTime			   = "	+QString::number( Finfo.fEchoTime)      + "\n"; 
		text+= "# chDataSetType		   = "	+QString::fromLatin1( Finfo.chDataSetType)   + "\n"; 
		text+= "# chDataSetSubtype     = "	+QString::fromLatin1( Finfo.chDataSetSubtype)   + "\n"; 
		text+= "# chProtocolName	   = "	+QString::fromLatin1( Finfo.chProtocolName)   + "\n"; 




		text+= "# END OF DECODING !  ---------------------------------- \n";


		QMessageBox::about( 0,"DICOM",text);
	}

}


//---------------------------------------------------------------

void DicomDataSet::InitEmptyInfo()
{

	//=================================================


	Finfo.fSpacingBetweenSlices		= 0.0;
	Finfo.fGantryTilt				= 0.0;
	Finfo.vImagePosition[0]			= 0.0;
	Finfo.vImagePosition[1]		= 0.0;
	Finfo.vImagePosition[2]			= 0.0;
	Finfo.vImageOrientation[0]		= 0.0; 
	Finfo.vImageOrientation[1]		= 0.0; 
	Finfo.vImageOrientation[2]		= 0.0; 
	Finfo.vImageOrientation[3]		= 0.0;
	Finfo.vImageOrientation[4]		= 0.0;
	Finfo.vImageOrientation[5]		= 0.0;
	Finfo.fSliceLocation			= 0.0;
	Finfo.fSliceThickness			= 0.0;
	Finfo.fPixelSpacing[0]			= 1.0;
	Finfo.fPixelSpacing[1]		= 1.0;

	Finfo.fWindowCenter				= 0.0;          
	Finfo.fWindowWidth				= 0.0;         
	Finfo.fRescaleIntercept			= 0.0;  
	Finfo.fRescaleSlope				= 1.0;

	Finfo.uBitsAllocated			= 0;
	Finfo.uBitsStored				= 0;
	Finfo.uHighBit					= 0;
	Finfo.chStudyDate[0]			= '\0';
	Finfo.chModality[0]				= '\0';
	Finfo.chFrameOfReferenceUID[0]	= '\0';
	Finfo.chPatientID[0]			= '\0';
	Finfo.chStudyID[0]				= '\0';
	Finfo.chRescaleType[0]			= '\0';
	Finfo.chTransferSyntaxUID[0]	= '\0';
	Finfo.chManufacturer[0]			= '\0';
	Finfo.chPatientName[0]				= '\0';
	Finfo.chPhotometricInterpretation[0]= '\0';
	Finfo.chPatientAge[0]= '\0';


	Finfo.chManufacturer3D[0]			= '\0';
	Finfo.iSlices					=	1;

	Finfo.chPatientBirthDate[0]='\0';
	Finfo.chPatientSex[0]='\0';

	Finfo.fPatientSize=0;
	Finfo.fPatientWeight=0;



	Finfo.chInstitutionName[0]			= '\0';
	Finfo.iSeriesNumber					=	0;
	Finfo.iImageNumber					=	0;
	Finfo.iRows							=	0;
	Finfo.iColumns						=	0;
	Finfo.uPixelRepresentation			=	0;
	Finfo.lPixelDataOffset				=	0;
	Finfo.lPixelDataLength				=	0;


	Finfo.bPixelDataLittleEndian = true;
	Finfo.fEchoTime					= 0.0; 
	Finfo.chProtocolName[0]			= '\0';
	Finfo.chDataSetType[0]				= '\0';
	Finfo.chDataSetSubtype[0]			= '\0';


	/*

	//	 CHAR                      chStudyDate[10];           // 0x0008, 0x0020, DA        
	// CHAR                      chModality[16];            // 0x0008, 0x0060, CS          
	// CHAR                      chManufacturer[64];        // 0x0008, 0x0070, LO
	// CHAR                      chInstitutionName[64];     // 0x0008, 0x0080, LO
	//  CHAR                      chPatientName[64];         // 0x0010, 0x0010, PN
	// CHAR                      chPatientID[64];           // 0x0010, 0x0020, LO
	//  CHAR                      chStudyID[16];             // 0x0020, 0x0010, SH
	// INT                       iSeriesNumber;             // 0x0020, 0x0011, IS
	// INT                       iImageNumber;              // 0x0020, 0x0013, IS
	// SVector3D                 vImagePosition;            // 0x0020, 0x0032, DS, "3"
	// SImageOrientationVector   vImageOrientation;         // 0x0020, 0x0037, DS, "6"
	// CHAR                      chFrameOfReferenceUID[64]; // 0x0020, 0x0052, UI
	// CHAR                      chPatientAge[6];           // 0x0010, 0x1010, AS
	// FLOAT                     fSliceLocation;            // 0x0020, 0x1041, DS
	// CHAR                      chPhotometricInterpretation[16]; // 0x0028, 0x0004, CS 
	// INT                       iRows;                     // 0x0028, 0x0010, US
	// INT                       iColumns;                  // 0x0028, 0x0011, US
	// FLOAT                     fPixelSpacingRow;          // 0x0028, 0x0030, DS, "2"
	//  FLOAT                     fPixelSpacingColumn;       // 0x0028, 0x0030, DS, "2"
	//  FLOAT                     fWindowCenter;             // 0x0028, 0x1050, DS,
	// FLOAT                     fWindowWidth;              // 0x0028, 0x1051, DS,
	//  FLOAT                     fRescaleIntercept;         // 0x0028, 0x1052, DS, 
	// FLOAT                     fRescaleSlope;             // 0x0028, 0x1053, DS, 
	// CHAR                      chRescaleType[64];         // 0x0028, 0x1054, LO
	// USHORT                    uBitsAllocated;            // 0x0028, 0x0100, US
	// USHORT                    uBitsStored;               // 0x0028, 0x0101, US
	// USHORT                    uHighBit;                  // 0x0028, 0x0102, US
	// USHORT                    uPixelRepresentation;      // 0x0028, 0x0103, US 
	//  FLOAT                     fSliceThickness;           // 0x0018, 0x0050, DS
	// FLOAT                     fSpacingBetweenSlices;     // 0x0018, 0x0088, DS 
	//FLOAT                     fGantryTilt;               // 0x0018, 0x1120, DS
	// CHAR                      chTransferSyntaxUID[64];   // 0x0002, 0x0010, UI
	//  unsigned int                      lPixelDataOffset;          // 0x7FE0, 0x0010, OX 
	// unsigned int                      lPixelDataLength;			 // Size of the image (in bytes)
	//  bool                      bPixelDataLittleEndian;    // Is the pixel data in little endian format ?

	CHAR						chDataSetType[64];		   // 0x0008, 0x0040, LO
	CHAR						chDataSetSubtype[64];	   // 0x0008, 0x0041, LO
	FLOAT						fEchoTime;				   // 0x0018, 0x0081, DS
	CHAR						chProtocolName[64];		   // 0x0018, 0x1030, LO
	*/


	NumberOfSlices=-1;
}


//---------------------------------------------------------------

void DicomDataSet::GetID()
{


	if (Init==true)
	{	

		QString text="";

		text+=QString::fromLatin1 ( Finfo.chStudyDate); 
		text+=QString::fromLatin1( Finfo.chModality)      ; 
		text+=QString::fromLatin1( Finfo.chManufacturer)  ; 
		text+=QString::fromLatin1( Finfo.chPatientName)   ; 
		text+=QString::fromLatin1( Finfo.chPatientID)     ; 
		text+=QString::fromLatin1( Finfo.chStudyID)       ; 
		//	text+=QString::number( Finfo.iSeriesNumber)   ; //wywalilem!!!11.05.2003
		//	text+=QString::number( Finfo.iImageNumber)    ; 
		//	text+=QString::fromLatin1( Finfo.chPhotometricInterpretation)  ; 
		text+=QString::number( Finfo.iRows)           ; 
		text+=QString::number( Finfo.iColumns)        ; 
		text+=QString::number( Finfo.vImageOrientation[0])
			+QString::number( Finfo.vImageOrientation[1])
			+QString::number( Finfo.vImageOrientation[2]) 
			+QString::number( Finfo.vImageOrientation[3])
			+QString::number( Finfo.vImageOrientation[4])
			+QString::number( Finfo.vImageOrientation[5]) ; 
		text+=QString::number( Finfo.fPixelSpacing[0])      
			+QString::number( Finfo.fPixelSpacing[1]);//  +"\n"  ; 
		//	text+=QString::number( Finfo.fSliceThickness)        ; 
		//	text+=QString::number( Finfo.fSpacingBetweenSlices)  ; 
		text+=QString::number( Finfo.fGantryTilt)            ; 
		text+=QString::fromLatin1( Finfo.chFrameOfReferenceUID); 
		//	text+=QString::number( Finfo.fRescaleIntercept)      ; 
		//	text+=QString::number( Finfo.fRescaleSlope)          ;  
		//	text+=QString::fromLatin1( Finfo.chRescaleType)      ; 
		//	text+=QString::number( Finfo.fWindowCenter)          ; 
		//	text+=QString::number( Finfo.fWindowWidth)           ; 
		text+=QString::number( Finfo.uBitsAllocated)         ; 
		text+=QString::number( Finfo.uBitsStored)            ; 
		text+=QString::number( Finfo.uHighBit)               ; 
		text+=QString::number( Finfo.uPixelRepresentation)   ; 


		text+=QString::number( Finfo.fEchoTime)				 ; 
		text+=QString::fromLatin1( Finfo.chDataSetType)      ; 
		text+=QString::fromLatin1( Finfo.chDataSetSubtype)   ; 

		//f (QString::fromLatin1( Finfo.chModality)!="CT")
		text+=QString::fromLatin1( Finfo.chProtocolName)     ; 





		//	text+=QString::fromLatin1( Finfo.chTransferSyntaxUID); 
		//	text+=QString::number( Finfo.lPixelDataOffset)       ; 
		//	text+=QString::number( Finfo.lPixelDataLength)       ; 

		DataSetID=text;

	}
}

//---------------------------------------------------------------

bool DicomDataSet::IsDicomFile(const QString & szPath) const
{

//	vtkGDCMImageReader *dicomReader = vtkGDCMImageReader::New();
//	dicomReader->SetFileName(szPath);
//	dicomReader->FileLowerLeftOn();
//	dicomReader->Update();
//	int val=dicomReader->CanReadFile(szPath);		
//	if (val!=0)
//	{
//	// QString refms = gdcm.Testing.GetMediaStorageFromFile(filename);
//      const char *refms = gdcm::Testing::GetMediaStorageFromFile(szPath);  
//
//	if( gdcm::MediaStorage::IsImage( gdcm::MediaStorage::GetMSType(refms) ) )
//        {
//       
//			qWarning()<<("problem with file: "+QString(szPath));
//			// System.Console.Write( "Problem with file: " + filename + "\n" );
//        return true;
//        }
//      // not an image
//	qWarning()<<("bad file: "+QString(szPath));
//      return false;
//}
//return true;

	//vtkGDCMImageReader *dicomReader = vtkGDCMImageReader::New();
	//		dicomReader->SetFileName(szPath);
	//		dicomReader->CanReadFile(szPath);
	//		dicomReader->FileLowerLeftOn();
	//		dicomReader->Update();
	//		vtkImageCast *cast = vtkImageCast::New();
	//		cast->SetInputData( dicomReader->GetOutput() );
	//	
	//		

	//		vtkImageData *iData=cast->GetOutput();
	//		iData->Update();

	//vtkGDCMImageReader *dicomReader = vtkGDCMImageReader::New();
	//dicomReader->SetFileName(szPath);
	//dicomReader->FileLowerLeftOn();
	//dicomReader->Update();
	//int val=dicomReader->CanReadFile(szPath);	
	//dicomReader->Delete();
	//if (val!=0) return true;
	

	//QString aa = QObject:tr("¥©˜†¢ˆä¾«");

	//QString p1 = QObject::tr(szPath);
	QFile f(szPath);
	bool read = f.open(QIODevice::ReadOnly);
	if (read == true)
	{
		f.close();
	}

	gdcm::Reader reader;
	//const char * tt = szPath.toLocal8Bit();
	
	//QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
	//
	//QByteArray encodedString = szPath.toLocal8Bit();// codec->fromUnicode(szPath);

	//const char * tt =encodedString.data();
	const char *bb = szPath.toLocal8Bit().data();
	const char* c = szPath.toLatin1().data();
//	const char* d = szPath.constData()->unicode()->;

	reader.SetFileName(szPath.toLocal8Bit().constData());

	///QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8Windows-1250"));
	std::cout <<"test"  << (szPath.toLocal8Bit().constData());

	//std::cout << QTextCodec::codecForLocale();


	bool test=reader.CanRead();
	return test;

	const unsigned	_DICOM_HEADERLENGTH		= 1024U;

	const char*		_DICOM_P10_IDENTIFIER	= "DICM";
	const char*		_ISO_IR_100_IDENTIFIER	= "ISO_IR";
	

	Q_ASSERT(szPath != NULL);

	QFile file(szPath);

	unsigned ur;

	if (file.open(QIODevice::ReadOnly)) 
	{

		QString FileName=szPath;
		unsigned char pbFullHeader[_DICOM_HEADERLENGTH];

		if ((FileName.lastIndexOf(".gz")!=-1)||(FileName.lastIndexOf(".GZ")!=-1))
		{


			gzFile Zipek;
			Zipek=gzopen (FileName.toLocal8Bit().constData(),"rb");
			ur =gzread(Zipek,pbFullHeader,1024);
			gzclose (Zipek); 

		}
		else
		{
			ur = file.read((char*)pbFullHeader, _DICOM_HEADERLENGTH);
		}

		if (ur == _DICOM_HEADERLENGTH)	{


			int nr = ::memcmp(&pbFullHeader[128], _DICOM_P10_IDENTIFIER, ::strlen(_DICOM_P10_IDENTIFIER));
			if (nr==-1) 
			{
				nr= ::memcmp(&pbFullHeader[128], _ISO_IR_100_IDENTIFIER, ::strlen(_ISO_IR_100_IDENTIFIER));
			}

		/*	if (nr == 0) {

				return true;
			}
			else*/	{

				long           lDataPos = 0L;
				bool           test1 = false, test2 = false, test3 = false;

				DicomObject  DObject;


				while (lDataPos < (_DICOM_HEADERLENGTH - 8))	{   // 8 = (Group + Element + Length) bytes 

					// get one whole tag and its length and decode it: 

					DObject.Group = (unsigned short)pbFullHeader[lDataPos] +
						(((unsigned short)pbFullHeader[lDataPos+1]) << 8);

					lDataPos += sizeof( DObject.Group );

					DObject.Element = (unsigned short)pbFullHeader[lDataPos] +
						(((unsigned short)pbFullHeader[lDataPos+1]) << 8);

					lDataPos += sizeof( DObject.Element );

					DObject.Length  = (unsigned long)pbFullHeader[lDataPos] +
						(((unsigned long)pbFullHeader[lDataPos+1]) <<  8) +
						(((unsigned long)pbFullHeader[lDataPos+2]) << 16) +
						(((unsigned long)pbFullHeader[lDataPos+3]) << 24);

					lDataPos += sizeof( DObject.Length );         

					// Looking for one of the required objects in the DICOM header

					switch (DObject.Group)	{

						case 0x0008 : 

							switch (DObject.Element)	{

						case 0x0020 :  // Study Date
							test1 = true;
							break;

						case 0x0060 :  // Modality
							test2 = true;
							break;
							}

							break;

						case 0x0010 :

							switch (DObject.Element)	{

						case 0x0010 : // Patient Name
							test3 = true;
							break;
							}

							break;

					}

					lDataPos = lDataPos + DObject.Length;

				} // while

				if (((test1) && (test2)) || ((test2) && (test3)))
					return true;

			} // else
		} 
	}

	return false; 
	
} 

//---------------------------------------------------------------

void DicomDataSet::CompareTwoDicomFiles(DicomDataSet *d1,DicomDataSet *d2)
{

	QString text="";


	text+=CompareTwoStrings("FileName",						d1->FileName,													d2->FileName);
	text+=CompareTwoStrings("chStudyDate",					QString::fromLatin1(d1->Finfo.chStudyDate),						QString::fromLatin1(d2->Finfo.chStudyDate));
	text+=CompareTwoStrings("chModality",					QString::fromLatin1(d1->Finfo.chModality),						QString::fromLatin1(d2->Finfo.chModality));
	text+=CompareTwoStrings("chManufacturer",				QString::fromLatin1(d1->Finfo.chManufacturer),					QString::fromLatin1(d2->Finfo.chManufacturer));
	text += CompareTwoStrings("chInstitutionName", QString::fromLatin1(d1->Finfo.chInstitutionName), QString::fromLatin1(d2->Finfo.chInstitutionName));
	text += CompareTwoStrings("chSeriesDescription", QString::fromLatin1(d1->Finfo.chSeriesDescription), QString::fromLatin1(d2->Finfo.chSeriesDescription));
	text += CompareTwoStrings("chStudyDescription", QString::fromLatin1(d1->Finfo.chStudyDescription), QString::fromLatin1(d2->Finfo.chStudyDescription));
	text+=CompareTwoStrings("chPatientName",				QString::fromLatin1(d1->Finfo.chPatientName),					QString::fromLatin1(d2->Finfo.chPatientName));
	text+=CompareTwoStrings("chPatientID",					QString::fromLatin1(d1->Finfo.chPatientID),						QString::fromLatin1(d2->Finfo.chPatientID));
	text+=CompareTwoStrings("chStudyID",					QString::fromLatin1(d1->Finfo.chStudyID),						QString::fromLatin1(d2->Finfo.chStudyID));
	text+=CompareTwoStrings("chTransferSyntaxUID",			QString::fromLatin1(d1->Finfo.chTransferSyntaxUID),				QString::fromLatin1(d2->Finfo.chTransferSyntaxUID));
	text+=CompareTwoStrings("chFrameOfReferenceUID",		QString::fromLatin1(d1->Finfo.chFrameOfReferenceUID),			QString::fromLatin1(d2->Finfo.chFrameOfReferenceUID));
	text+=CompareTwoStrings("chPatientAge",					QString::fromLatin1(d1->Finfo.chPatientAge),					QString::fromLatin1(d2->Finfo.chPatientAge));
	
	
	text+=CompareTwoStrings("chPatientBirthDate",			QString::fromLatin1(d1->Finfo.chPatientBirthDate),				QString::fromLatin1(d2->Finfo.chPatientBirthDate));
	text+=CompareTwoStrings("chPatientSex",					QString::fromLatin1(d1->Finfo.chPatientSex),					QString::fromLatin1(d2->Finfo.chPatientSex));
	
	text+=CompareTwoStrings("fPatientSize",					QString::number(d1->Finfo.fPatientSize),						QString::number(d2->Finfo.fPatientSize));
	text+=CompareTwoStrings("fPatientWeight",				QString::number(d1->Finfo.fPatientWeight),						QString::number(d2->Finfo.fPatientWeight));

	
	text+=CompareTwoStrings("chRescaleType",				QString::fromLatin1(d1->Finfo.chRescaleType),					QString::fromLatin1(d2->Finfo.chRescaleType));
	text+=CompareTwoStrings("chPhotometricInterpretation",	QString::fromLatin1(d1->Finfo.chPhotometricInterpretation),		QString::fromLatin1(d2->Finfo.chPhotometricInterpretation));
	text+=CompareTwoStrings("fSliceLocation",				QString::number(d1->Finfo.fSliceLocation),						QString::number(d2->Finfo.fSliceLocation));
	text+=CompareTwoStrings("iRows",						QString::number(d1->Finfo.iRows),								QString::number(d2->Finfo.iRows));
	text+=CompareTwoStrings("iColumns",						QString::number(d1->Finfo.iColumns),							QString::number(d2->Finfo.iColumns));
	text+=CompareTwoStrings("fPixelSpacingRow",				QString::number(d1->Finfo.fPixelSpacing[0]),					QString::number(d2->Finfo.fPixelSpacing[0]));
	text+=CompareTwoStrings("fPixelSpacingColumn",			QString::number(d1->Finfo.fPixelSpacing[1]),					QString::number(d2->Finfo.fPixelSpacing[1]));
	text+=CompareTwoStrings("fWindowCenter",				QString::number(d1->Finfo.fWindowCenter),						QString::number(d2->Finfo.fWindowCenter));
	text+=CompareTwoStrings("fWindowWidth",					QString::number(d1->Finfo.fWindowWidth),						QString::number(d2->Finfo.fWindowWidth));
	text+=CompareTwoStrings("fRescaleIntercept",			QString::number(d1->Finfo.fRescaleIntercept),					QString::number(d2->Finfo.fRescaleIntercept));
	text+=CompareTwoStrings("fRescaleSlope",				QString::number(d1->Finfo.fRescaleSlope),						QString::number(d2->Finfo.fRescaleSlope));
	text+=CompareTwoStrings("uBitsAllocated",				QString::number(d1->Finfo.uBitsAllocated),						QString::number(d2->Finfo.uBitsAllocated));
	text+=CompareTwoStrings("uBitsStored",					QString::number(d1->Finfo.uBitsStored),							QString::number(d2->Finfo.uBitsStored));
	text+=CompareTwoStrings("uHighBit",						QString::number(d1->Finfo.uHighBit),							QString::number(d2->Finfo.uHighBit));
	text+=CompareTwoStrings("uPixelRepresentation",			QString::number(d1->Finfo.uPixelRepresentation),				QString::number(d2->Finfo.uPixelRepresentation));
	text+=CompareTwoStrings("fSliceThickness",				QString::number(d1->Finfo.fSliceThickness),						QString::number(d2->Finfo.fSliceThickness));
	text+=CompareTwoStrings("fSpacingBetweenSlices",		QString::number(d1->Finfo.fSpacingBetweenSlices),				QString::number(d2->Finfo.fSpacingBetweenSlices));
	text+=CompareTwoStrings("fGantryTilt",					QString::number(d1->Finfo.fGantryTilt),							QString::number(d2->Finfo.fGantryTilt));
	text+=CompareTwoStrings("lPixelDataOffset",				QString::number(d1->Finfo.lPixelDataOffset),					QString::number(d2->Finfo.lPixelDataOffset));
	text+=CompareTwoStrings("lPixelDataLength",				QString::number(d1->Finfo.lPixelDataLength),					QString::number(d2->Finfo.lPixelDataLength));
	text+=CompareTwoStrings("iSeriesNumber",				QString::number(d1->Finfo.iSeriesNumber),						QString::number(d2->Finfo.iSeriesNumber));
	text+=CompareTwoStrings("iImageNumber",					QString::number(d1->Finfo.iImageNumber),						QString::number(d2->Finfo.iImageNumber));
	text+=CompareTwoStrings("bPixelDataLittleEndian",		QString::number(d1->Finfo.bPixelDataLittleEndian),				QString::number(d2->Finfo.bPixelDataLittleEndian));
	text+=CompareTwoStrings("vImagePosition.x",				QString::number(d1->Finfo.vImagePosition[0]),					QString::number(d2->Finfo.vImagePosition[0]));
	text+=CompareTwoStrings("vImagePosition.y",				QString::number(d1->Finfo.vImagePosition[1]),					QString::number(d2->Finfo.vImagePosition[1]));
	text+=CompareTwoStrings("vImagePosition.z",				QString::number(d1->Finfo.vImagePosition[2]),					QString::number(d2->Finfo.vImagePosition[2]));
	text+=CompareTwoStrings("vImageOrientation.rx1",		QString::number(d1->Finfo.vImageOrientation[0]),				QString::number(d2->Finfo.vImageOrientation[0]));
	text+=CompareTwoStrings("vImageOrientation.ry1",		QString::number(d1->Finfo.vImageOrientation[1]),				QString::number(d2->Finfo.vImageOrientation[1]));
	text+=CompareTwoStrings("vImageOrientation.ry1",		QString::number(d1->Finfo.vImageOrientation[2]),				QString::number(d2->Finfo.vImageOrientation[2]));
	text+=CompareTwoStrings("vImageOrientation.cx1",		QString::number(d1->Finfo.vImageOrientation[3]),				QString::number(d2->Finfo.vImageOrientation[3]));
	text+=CompareTwoStrings("vImageOrientation.cy1",		QString::number(d1->Finfo.vImageOrientation[4]),				QString::number(d2->Finfo.vImageOrientation[4]));
	text+=CompareTwoStrings("vImageOrientation.cz1",		QString::number(d1->Finfo.vImageOrientation[5]),				QString::number(d2->Finfo.vImageOrientation[5]));


	text+=CompareTwoStrings("fEchoTime",					QString::number(d1->Finfo.fEchoTime),							QString::number(d2->Finfo.fEchoTime));
	text+=CompareTwoStrings("chDataSetType",				QString::fromLatin1(d1->Finfo.chDataSetType),					QString::fromLatin1(d2->Finfo.chDataSetType));
	text+=CompareTwoStrings("chDataSetSubtype",				QString::fromLatin1(d1->Finfo.chDataSetSubtype),				QString::fromLatin1(d2->Finfo.chDataSetSubtype));
	text+=CompareTwoStrings("chProtocolName",				QString::fromLatin1(d1->Finfo.chProtocolName),					QString::fromLatin1(d2->Finfo.chProtocolName));


	//text=DataSetID+"\n"+d1->GetData()SetID;	
	//	DataSetID
	QMessageBox::about( 0,"DICOM",text);

}

//---------------------------------------------------------------

void DicomDataSet::CompareWithFile(DicomDataSet *d1)
{
	CompareTwoDicomFiles(this,d1);

}

//---------------------------------------------------------------

QString DicomDataSet::CompareTwoStrings(QString Title,QString A, QString B)
{


	if (A!=B)
		return (Title+" "+A+" <> "+B+"\n");
	else
		return "";


}
