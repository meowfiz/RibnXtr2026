

#include  <stdlib.h>
#include  <string.h>
#include  <stdio.h>
#include  <iostream>

#include  <iomanip>


#include "DicomObject.h"
#include "DicomInfo.h"


DicomInfo::DicomInfo()
{
	m_uActualGroup = 0;
	m_bPART10_PREAMBLE = false;

	

	
		
}

DicomInfo::~DicomInfo()
{

}

unsigned short  DicomInfo::ExtractUNSIGNED_SHORT( unsigned char* pFileData, unsigned int lDataPos )
{
	return (unsigned short)pFileData[lDataPos] + (((unsigned short)pFileData[lDataPos+1]) << 8);
}


unsigned int  DicomInfo::ExtractUNSIGNED_INT( unsigned char* pFileData, unsigned int lDataPos )
{
	return (unsigned long)pFileData[lDataPos] + 
		(((unsigned long)pFileData[lDataPos+1]) <<  8) +
		(((unsigned long)pFileData[lDataPos+2]) << 16) +
		(((unsigned long)pFileData[lDataPos+3]) << 24);
}





bool DicomInfo::ExtractDicomFileInfo( unsigned char *pFileData, unsigned int lDataLength, DicomFileInfo& Finfo )
{
	Finfo.jpegCompression=false;
	unsigned int i;

	char AuxString[512];
	unsigned int lDataPos = 0L;

	DicomObject  DObject;


	lDataPos = 128L;

	if ( pFileData[lDataPos  ] == 'D' && pFileData[lDataPos+1] == 'I' &&
		pFileData[lDataPos+2] == 'C' && pFileData[lDataPos+3] == 'M'   )
	{
		lDataPos = 128L + 4L;
		m_bPART10_PREAMBLE = true;
	}
	else 
		lDataPos = 0L;



	ie33_3DData=false;
	ie33_3DDataExtracted=false;
	ie33_3DWatchData=false;
	Finfo.iCode=0;
	Finfo.iDataPos=0;
	Finfo.iDataEndPos=0;

	while ( lDataPos < lDataLength ) 
	{

		// get one whole tag and its length and decode it: 

		DObject.Group   = ExtractUNSIGNED_SHORT( pFileData, lDataPos );  
		lDataPos += sizeof( DObject.Group );
		DObject.Element = ExtractUNSIGNED_SHORT( pFileData, lDataPos );
		lDataPos += sizeof( DObject.Element );

		/*
		DObject.Length  = ExtractUNSIGNED_INT( pFileData, lDataPos );
		lDataPos += sizeof( DObject.Length );
		*/



		// ---------------------------------------------------------------

		// check the explicit value representation


		if ( (pFileData[lDataPos] == 'O' && pFileData[lDataPos+1] == 'B') ||
			(pFileData[lDataPos] == 'O' && pFileData[lDataPos+1] == 'W') ||
			(pFileData[lDataPos] == 'U' && pFileData[lDataPos+1] == 'N') ||
			(pFileData[lDataPos] == 'S' && pFileData[lDataPos+1] == 'Q') ) 
		{
			// These explicit VR's have a 32-bit length.

			lDataPos = lDataPos + 4L;
			DObject.Length  = ExtractUNSIGNED_INT( pFileData, lDataPos );
			lDataPos += sizeof( DObject.Length );
		}
		else if ( (pFileData[lDataPos]=='A' && pFileData[lDataPos+1]=='E') ||
			(pFileData[lDataPos]=='A' && pFileData[lDataPos+1]=='S') ||
			(pFileData[lDataPos]=='A' && pFileData[lDataPos+1]=='T') ||
			(pFileData[lDataPos]=='C' && pFileData[lDataPos+1]=='S') ||
			(pFileData[lDataPos]=='D' && pFileData[lDataPos+1]=='A') ||
			(pFileData[lDataPos]=='D' && pFileData[lDataPos+1]=='S') ||
			(pFileData[lDataPos]=='D' && pFileData[lDataPos+1]=='T') ||
			(pFileData[lDataPos]=='F' && pFileData[lDataPos+1]=='L') ||
			(pFileData[lDataPos]=='F' && pFileData[lDataPos+1]=='D') ||
			(pFileData[lDataPos]=='I' && pFileData[lDataPos+1]=='S') ||
			(pFileData[lDataPos]=='L' && pFileData[lDataPos+1]=='O') ||
			(pFileData[lDataPos]=='L' && pFileData[lDataPos+1]=='T') ||
			(pFileData[lDataPos]=='P' && pFileData[lDataPos+1]=='N') ||
			(pFileData[lDataPos]=='S' && pFileData[lDataPos+1]=='H') ||
			(pFileData[lDataPos]=='S' && pFileData[lDataPos+1]=='L') ||
			(pFileData[lDataPos]=='S' && pFileData[lDataPos+1]=='S') ||
			(pFileData[lDataPos]=='S' && pFileData[lDataPos+1]=='T') ||
			(pFileData[lDataPos]=='T' && pFileData[lDataPos+1]=='M') ||
			(pFileData[lDataPos]=='U' && pFileData[lDataPos+1]=='I') ||
			(pFileData[lDataPos]=='U' && pFileData[lDataPos+1]=='L') ||
			(pFileData[lDataPos]=='U' && pFileData[lDataPos+1]=='S') ) 
		{
			// These explicit VR's have a 16-bit length.  This
			// allows them to fit into the same space as implicit
			// VR.

			lDataPos = lDataPos + 2L;
			DObject.Length  = (unsigned long)ExtractUNSIGNED_SHORT( pFileData, lDataPos );
			lDataPos = lDataPos + 2L;
		}
		else if  ((DObject.Group=0xFFFE)&&((DObject.Element=0xE00D)))
		{
			DObject.Length=4;
		}
		else 
		{
			DObject.Length  = ExtractUNSIGNED_INT( pFileData, lDataPos );
			//DObject.Length  = ExtractUNSIGNED_SHORT( pFileData, lDataPos );
			lDataPos += sizeof( DObject.Length );
		}

		// ---------------------------------------------------------------
		/*
		  static const char* DICOM_IMPLICIT_VR_LITTLE_ENDIAN = "1.2.840.10008.1.2";
  static const char* DICOM_LOSSLESS_JPEG = "1.2.840.10008.1.2.4.70";
  static const char* DICOM_LOSSY_JPEG_8BIT = "1.2.840.10008.1.2.4.50";
  static const char* DICOM_LOSSY_JPEG_16BIT = "1.2.840.10008.1.2.4.51";
  static const char* DICOM_EXPLICIT_VR_LITTLE_ENDIAN = "1.2.840.10008.1.2.1";
  static const char* DICOM_EXPLICIT_VR_BIG_ENDIAN = "1.2.840.10008.1.2.2";
  static const char* DICOM_GE_PRIVATE_IMPLICIT_BIG_ENDIAN = "1.2.840.113619.5.2";
		*/

		switch (DObject.Group) 
		{

		case 0x0002 : 

			switch (DObject.Element)
			{

			case 0x0010 :  
				for (i=0; i < DObject.Length; i++) Finfo.chTransferSyntaxUID[i] = pFileData[lDataPos+i];
				Finfo.chTransferSyntaxUID[DObject.Length] = '\0';

				if (strcmp(Finfo.chTransferSyntaxUID,"1.2.840.10008.1.2") == 0)
				{
					Finfo.bPixelDataLittleEndian = true;  // Implicit VR Little Endian
				}
				else 	if (strcmp(Finfo.chTransferSyntaxUID,"1.2.840.10008.1.2.1") == 0)
				{
					Finfo.bPixelDataLittleEndian = true;  // Explicit VR Little Endian 
				}
				else	if (strcmp(Finfo.chTransferSyntaxUID,"1.2.840.10008.1.2.2") == 0 )           
				{
					Finfo.bPixelDataLittleEndian = false;  // Explicit VR Big Endian
				}
				else if (	(strcmp(Finfo.chTransferSyntaxUID,"1.2.840.10008.1.2.4.70") == 0) || // DICOM_LOSSLESS_JPEG
							(strcmp(Finfo.chTransferSyntaxUID,"1.2.840.10008.1.2.4.50") == 0) || //DICOM_LOSSY_JPEG_8BIT
							(strcmp(Finfo.chTransferSyntaxUID,"1.2.840.10008.1.2.4.51") == 0))// || //DICOM_LOSSY_JPEG_16BIT
							//(strcmp(Finfo.chTransferSyntaxUID,"1.2.840.113619.5.2") == 0) )		//DICOM_GE_PRIVATE_IMPLICIT_BIG_ENDIAN
				{
					//dane z kompresja trzeba ustawic flage zeby wczytywac przez vtk!!!!
					Finfo.jpegCompression=true;
					Finfo.bPixelDataLittleEndian = true; //nie wiem - sprawdze czy cos wniesie
				}
				else 
				{
					Finfo.bPixelDataLittleEndian = true;  // Assumed Little Endian !!!
				}

				break;
			}
			break;


		case 0x0008 : 

			switch (DObject.Element)
			{
			case 0x0020 :  
				for (i=0; i < DObject.Length; i++) Finfo.chStudyDate[i] = pFileData[lDataPos+i];
				Finfo.chStudyDate[DObject.Length] = '\0';
				break;
			case 0x0060 : 
				// old version: strcpy((char *)Finfo.chModality,(char *)(pFileData+lDataPos));
				for (i=0; i < DObject.Length; i++) Finfo.chModality[i] = pFileData[lDataPos+i];
				Finfo.chModality[DObject.Length] = '\0';
				break;
			case 0x0070 : 
				for (i=0; i < DObject.Length; i++) Finfo.chManufacturer[i] = pFileData[lDataPos+i];
				Finfo.chManufacturer[DObject.Length] = '\0';
				break;
			case 0x0080 : 
				for (i=0; i < DObject.Length; i++) Finfo.chInstitutionName[i] = pFileData[lDataPos+i];
				Finfo.chInstitutionName[DObject.Length] = '\0';
				break;
			case 0x103E:
				for (i = 0; i < DObject.Length; i++) Finfo.chSeriesDescription[i] = pFileData[lDataPos + i];
				Finfo.chSeriesDescription[DObject.Length] = '\0';
				break;
			case 0x1030:
				for (i = 0; i < DObject.Length; i++) Finfo.chStudyDescription[i] = pFileData[lDataPos + i];
				Finfo.chStudyDescription[DObject.Length] = '\0';
				break;

			case 0x0040 : 
				for (i=0; i < DObject.Length; i++) Finfo.chDataSetType[i] = pFileData[lDataPos+i];
				Finfo.chDataSetType[DObject.Length] = '\0';
				break;

			case 0x0041 : 
				for (i=0; i < DObject.Length; i++) Finfo.chDataSetSubtype[i] = pFileData[lDataPos+i];
				Finfo.chDataSetSubtype[DObject.Length] = '\0';
				break;


			}
			break;


		case 0x0010 :

			switch (DObject.Element)
			{
			case 0x0010 : 
				for (i=0; i < DObject.Length; i++) Finfo.chPatientName[i] = pFileData[lDataPos+i];
				Finfo.chPatientName[DObject.Length] = '\0';
				break;
			case 0x0020 : 
				for (i=0; i < DObject.Length; i++) Finfo.chPatientID[i] = pFileData[lDataPos+i];
				Finfo.chPatientID[DObject.Length] = '\0';
				break;

			case 0x0030 : 
				for (i=0; i < DObject.Length; i++) Finfo.chPatientBirthDate[i] = pFileData[lDataPos+i];
				Finfo.chPatientBirthDate[DObject.Length] = '\0';
				break;

			case 0x0040 : 
				for (i=0; i < DObject.Length; i++) Finfo.chPatientSex[i] = pFileData[lDataPos+i];
				Finfo.chPatientSex[DObject.Length] = '\0';
				break;


			case 0x1010 : 
				for (i=0; i < DObject.Length; i++) Finfo.chPatientAge[i] = pFileData[lDataPos+i];
				Finfo.chPatientAge[DObject.Length] = '\0';
				break;

			case 0x1020 : 
				Finfo.fPatientSize = (double)atof((const char*)(pFileData+lDataPos));
				break;
		
			case 0x1030 : 
				Finfo.fPatientWeight = (double)atof((const char*)(pFileData+lDataPos));
				break;
		


			}
			break;


		case 0x0018 :

			switch (DObject.Element)
			{
			case 0x0050 : 
				Finfo.fSliceThickness = (double)atof((const char*)(pFileData+lDataPos));
				break;
			case 0x0088 : 
				sscanf((char *)(pFileData+lDataPos),"%lf",&(Finfo.fSpacingBetweenSlices));
				break;
			case 0x1120 : 
				sscanf((char*)(pFileData+lDataPos),"%lf",&(Finfo.fGantryTilt));
				break;

			case 0x0081 : 
				sscanf((char*)(pFileData+lDataPos),"%lf",&(Finfo.fEchoTime));
				break;

			case 0x1030 : 
				for (i=0; i < DObject.Length; i++) Finfo.chProtocolName[i] = pFileData[lDataPos+i];
				Finfo.chProtocolName[DObject.Length] = '\0';
				break;

			}
			break;


		case 0x0020 :

			switch (DObject.Element)
			{
			case 0x0010 : 
				for (i=0; i < DObject.Length; i++) Finfo.chStudyID[i] = pFileData[lDataPos+i];
				Finfo.chStudyID[DObject.Length] = '\0';
				break;
			case 0x0011 : 
				strncpy((char *)AuxString,(const char *)(pFileData+lDataPos),DObject.Length);
				sscanf((char *)AuxString,"%d",&(Finfo.iSeriesNumber));
				break;
			case 0x0013 : 
				strncpy((char *)AuxString,(const char *)(pFileData+lDataPos),DObject.Length);
				sscanf((char *)AuxString,"%d",&(Finfo.iImageNumber));
				break;
				/*       Image Position RET
				case 0x0030 : 
				strncpy((char *)AuxString,(const char *)(pFileData+lDataPos),DObject.Length);
				sscanf((char *)AuxString,"%lf\\%lf\\%lf",
				&(Finfo.vImagePosition.x), &(Finfo.vImagePosition.y), &(Finfo.vImagePosition.z));
				break;
				*/
			case 0x0032 :  /*  Image Position (Patient)  */
				strncpy((char *)AuxString,(const char *)(pFileData+lDataPos),DObject.Length);
				sscanf((char *)AuxString,"%lf\\%lf\\%lf",
					&(Finfo.vImagePosition[0]), &(Finfo.vImagePosition[1]), &(Finfo.vImagePosition[2]));
				break;
			case 0x0037 : 
				/*  strncpy((char *)AuxString,(const char *)(pFileData+lDataPos), DObject.Length);
				sscanf((char *)AuxString,"%lf\\%lf\\%lf\\%lf\\%lf\\%lf",
				&(Finfo.vImageOrientation.rx1), &(Finfo.vImageOrientation.ry1), &(Finfo.vImageOrientation.rz1),
				&(Finfo.vImageOrientation.cx1), &(Finfo.vImageOrientation.cy1), &(Finfo.vImageOrientation.cz1));
				*/
				sscanf((char*)(pFileData+lDataPos),"%lf\\%lf\\%lf\\%lf\\%lf\\%lf",
					&(Finfo.vImageOrientation[0]),&(Finfo.vImageOrientation[1]), &(Finfo.vImageOrientation[2]),
					&(Finfo.vImageOrientation[3]), &(Finfo.vImageOrientation[4]), &(Finfo.vImageOrientation[5]));
				break;
			case 0x0052 : 
				for (i=0; i < DObject.Length; i++) Finfo.chFrameOfReferenceUID[i] = pFileData[lDataPos+i];
				Finfo.chFrameOfReferenceUID[DObject.Length] = '\0';
				break;
			case 0x1041 : 

				sscanf((char *)(pFileData+lDataPos),"%lf",&(Finfo.fSliceLocation));
				//strncpy((char *)AuxString,(const char*)(pFileData+lDataPos),DObject.Length); 
				//sscanf((char *)AuxString,"%lf",&(Finfo.fSliceLocation));
				break;
			}
			break;


		case 0x0028 : 

			switch (DObject.Element)
			{
			case 0x0004 : 
				for (i=0; i < DObject.Length; i++) Finfo.chPhotometricInterpretation[i] = pFileData[lDataPos+i];
				Finfo.chPhotometricInterpretation[DObject.Length] = '\0';
				break;
			case 0x0010 : 
				Finfo.iRows = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;
				break;
			case 0x0011 : 
				Finfo.iColumns = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;
				break;
			case 0x0030 : 
				sscanf((char*)(pFileData+lDataPos),"%lf\\%lf",&(Finfo.fPixelSpacing[0]),&(Finfo.fPixelSpacing[1]));
				break;

			case 0x1054 : 
				for (i=0; i < DObject.Length; i++) Finfo.chRescaleType[i] = pFileData[lDataPos+i];
				Finfo.chRescaleType[DObject.Length] = '\0';
				break;

			case 0x0100 : 
				Finfo.uBitsAllocated = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;
				break;
			case 0x0101 : 
				Finfo.uBitsStored = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;
				break;
			case 0x0102 : 
				Finfo.uHighBit = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;
				break;
			case 0x0103 : 
				Finfo.uPixelRepresentation = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;
				break;
			case 0x0106 : 
				Finfo.minValue = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;
				break;

			

			case 0x1050 : 
				sscanf((char *)(pFileData+lDataPos),"%lf",&(Finfo.fWindowCenter));
				break;
			case 0x1051 : 

				sscanf((char *)(pFileData+lDataPos),"%lf",&(Finfo.fWindowWidth));        
				break;

			case 0x1052 : 

				sscanf((char *)(pFileData+lDataPos),"%lf",&(Finfo.fRescaleIntercept));
				break;
			case 0x1053 : 


				sscanf((char *)(pFileData+lDataPos),"%lf",&(Finfo.fRescaleSlope));


				break;

			}
			break;



			/*	 case 0x0028 : 

			switch (DObject.Element)
			{
			case 0x0004 : 
			for (i=0; i < DObject.Length; i++) Finfo.chPhotometricInterpretation[i] = pFileData[lDataPos+i];
			Finfo.chPhotometricInterpretation[DObject.Length] = '\0';
			break;
			}
			*/
		case 0x0029 : // Tylko dla danych z Trewiru !  UWAGA !!!!!

			switch (DObject.Element)
			{
			case 0x0010 : 
				/*  for (i=0; i < DObject.Length; i++) Finfo.chManufacturer[i] = pFileData[lDataPos+i];
				Finfo.chManufacturer[DObject.Length] = '\0';
				pFileData[lDataPos-8] = pFileData[lDataPos-8] - 1;
				pFileData[lDataPos+1] = pFileData[lDataPos+1] - (unsigned char)1;
				pFileData[lDataPos] = 256-10;

				Finfo.iRows = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;
				*/

				break;

			}
			break;


			// 3d dla danych 4d
			// int						iSlices;					// 0x3010, 0x1001, UL
			//  char						chManufacturer3D[64]		// 0x3010, 0x0010, LO

		case 0x3001:

			switch(DObject.Element)
			{

			case 0x0010 :

				for (i=0; i < DObject.Length; i++) Finfo.chManufacturer3D[i] = pFileData[lDataPos+i];
				Finfo.chManufacturer3D[DObject.Length] = '\0';

				break;

			case 0x1001 : 

				Finfo.iSlices = (int)pFileData[lDataPos] + (int)pFileData[lDataPos+1]*256;

				break;

			}
			break;
			/////////////////////////
			//0--------------------- 
			//-------------3d philips ultrasound ie33 tags


			//0---------------------
			//-------------3d philips ultrasound ie33 tags
		case 0x200D	: 
			switch (DObject.Element)
			{
				//case 0x3016:

			case 0x300D :
				for	(i=0; i	< DObject.Length; i++) 
					Aux64[i]	= pFileData[lDataPos+i];

				Aux64[DObject.Length] =	'\0';
				if (strcmp(Aux64,"UDM_USD_DATATYPE_DIN_3D_ECHO") == 0)
				{
					// sprawdzic czy nie jest UDM_USD_DATATYPE_DIN_3D_ECHO 

					ie33_3DData=true;
					//ie33_3DDataExtracted=false;

				}
				else
					ie33_3DData=false;
				break;


				//if (ie33_3DData==true)
				//	{
			case 0x3010:
				if (ie33_3DData==true)
				{	strncpy((char *)AuxString,(const char *)(pFileData+lDataPos),DObject.Length);
					sscanf((char *)AuxString,"%d",&(Finfo.iNumberOfSubVolumes));
				}//Finfo.iNumberOfSubVolumes = (int)pFileData[lDataPos] +	(int)pFileData[lDataPos+1]*256;
				break;
			case 0x3011:
				if (ie33_3DData==true)
				{	strncpy((char *)AuxString,(const char *)(pFileData+lDataPos),DObject.Length);
				sscanf((char *)AuxString,"%d",&(Finfo.iSubvolumeSize));
				}
					//Finfo.iSubvolumeSize = (int)pFileData[lDataPos] +	(int)pFileData[lDataPos+1]*256-512;
				break;
			case 0x3920:
				//if (ie33_3DData==true)
				{
					Finfo.iCode = (int)pFileData[lDataPos] +	(int)pFileData[lDataPos+1]*256;
					if ((Finfo.iDataPos!=0)&&(Finfo.iDataEndPos!=0))
					{
						//obliczyc wspolczynniki i wczytac dane
						Finfo.iRows=64;
						Finfo.iSlices=64-Finfo.iRows/8;
						Finfo.iColumns=(Finfo.iSubvolumeSize-512)/(Finfo.iRows*Finfo.iRows);


						Finfo.lPixelDataOffset = Finfo.iDataPos+400+30;//Finfo.iDataEndPos-(Finfo.iSubvolumeSize)*2;
						//-----------------------------------
						Finfo.RealDataLength=(unsigned int)(Finfo.iColumns*Finfo.iRows*Finfo.iSlices*Finfo.uBitsAllocated/8);
						Finfo.lPixelDataLength = Finfo.RealDataLength;//*Finfo.iNumberOfSubVolumes;
						if (((unsigned int)(lDataLength-Finfo.lPixelDataOffset)<Finfo.RealDataLength))				
							return false;	
						else 
							return true;


					}	


				}
				break;
			case 0x3207:
				//if (ie33_3DData==true)
					sscanf((char	*)(pFileData+lDataPos),"%lf",&(Finfo.fFrequency));		   
				break;	
			case 0x3315:
				//if (ie33_3DData==true)
					sscanf((char	*)(pFileData+lDataPos),"%d/%d/%d",&(Finfo.idR0),&(Finfo.idPhi0),&(Finfo.idTheta0),&(Finfo.idT0));	
				break;
			case 0x3316:
				//if (ie33_3DData==true)
					sscanf((char	*)(pFileData+lDataPos),"%d/%d/%d",&(Finfo.idR0),&(Finfo.idPhi),&(Finfo.idTheta),&(Finfo.idT));	
				break;
			case 0x3317:
				//if (ie33_3DData==true)
					sscanf((char	*)(pFileData+lDataPos),"%lf/%lf/%lf",&(Finfo.fsR),&(Finfo.fsPhi),&(Finfo.fsTheta),&(Finfo.fsT));	
				break;




				//int						iNumberOfSubVolumes;
				//int						iCode;
				//int						idR,idPhi,idTheta,idT;
				//int						idR0,idPhi0,idTheta0,idT0;
				//double						fsR,fsPhi,fsTheta,fsT;
				//int						iSubvolumeSize;
				//double						fFrequency;
				//
				// sprawdzic czy nie jest UDM_USD_DATATYPE_DIN_3D_ECHO  
				// jesli tak to bedziemy zbierac dalsze info:
				//
				//  0x3010 - liczba volumenow
				//  0x3011 - ilosc voxeli we wspolrzednych sferycznych (r,phi,theta) - 512
				// 0x???? - wyczaic i zapamietac skad sie zaczynaja dane
				//  0x3207 - czestotliwosc
				// 0x3315 - INT/INT/INT/INT - zakodowane ilosc wokseli w kazdym kierunku (r,phi,theta,time)
				// 0x3316 - INT/INT/INT/INT - zakodowana wysokosc w kazdym kierunku (r,phi,theta,time)
				// 0x3317 - DBL/DBL/DBL/DBL - skala w kazdym z kierunkow 
				//  0x3920 - kod




			case 0x300E : 

				if (ie33_3DData==true)
				{
					Finfo.iDataPos=lDataPos;
					ie33_3DWatchData=true;
				}
				//15
				/*Finfo.lPixelDataOffset = lDataPos+45285;//+52960+40;
				Finfo.lPixelDataLength = DObject.Length;
				Finfo.iColumns=400;
				Finfo.iRows=64;
				Finfo.iSlices=64;
				*/

				// 35
				/*	if (Finfo.iCode!=0)
				{
				//Finfo.lPixelDataOffset = lDataPos+42088;//+52960+40;
				//Finfo.lPixelDataLength = DObject.Length;
				//Finfo.iColumns=352;
				//Finfo.iRows=64;
				//Finfo.iSlices=64;
				//
				Finfo.RealDataLength=(unsigned int)(Finfo.iColumns*Finfo.iRows*Finfo.iSlices*Finfo.uBitsAllocated/8);
				Finfo.lPixelDataLength = Finfo.RealDataLength;
				if (((unsigned int)(lDataLength-lDataPos)<Finfo.RealDataLength))				
				return false;	
				else 
				return true;
				}
				break;
				}*/

				break;
			}
			//0---------------------------------
			//--------------------------------
		case 0x7FE0 : 

			switch (DObject.Element)
			{
			case 0x0010 : 
				Finfo.lPixelDataOffset = lDataPos;
				Finfo.lPixelDataLength = DObject.Length;
				// lDataPos = lDataLength+1;   /* EXIT */
				//	FILE *Do_opa;

				//	  Do_opa=fopen("c:\\listaxx.txt","a");fprintf(Do_opa,"%d %d %d\n ", DObject.Length,(unsigned int)(Finfo.iColumns*Finfo.iRows*Finfo.uBitsAllocated/8) ,lDataLength-lDataPos);fclose(Do_opa);

				//unsigned int 
				Finfo.RealDataLength=(unsigned int)(Finfo.iColumns*Finfo.iRows*Finfo.iSlices*Finfo.uBitsAllocated/8);
				//if ((DObject.Length!=RealDataLength)||((unsigned int)(lDataLength-lDataPos)<RealDataLength))		
				if (Finfo.jpegCompression==true)
				{
					return true;

				}
				else
				{
				if (((unsigned int)(lDataLength-lDataPos)<Finfo.RealDataLength))				
					return false;	
				else 
					return true;
				}
				break;
			}
			break;
		}




		if (DObject.Length == 0xFFFFFFFF)
		{
			do
			{

				DObject.Group   = ExtractUNSIGNED_SHORT( pFileData, lDataPos );  
				lDataPos += sizeof( DObject.Group );
				DObject.Element = ExtractUNSIGNED_SHORT( pFileData, lDataPos );
				lDataPos += sizeof( DObject.Element );
				DObject.Length  = ExtractUNSIGNED_INT( pFileData, lDataPos );
				lDataPos += sizeof( DObject.Length );

				if (( (DObject.Group == 0xFFFE) && (DObject.Element == 0xE000) &&  (DObject.Length == 0xFFFFFFFF) ))
				{
					lDataPos-=DObject.Length;
					break;
				}
				// if (!( (DObject.Group == 0xFFFE) && (DObject.Element == 0xE000) &&  (DObject.Length == 0xFFFFFFFF) ))
				if (DObject.Length == 0xFFFFFFFF)
				{

					// printf("\n-------------- (0xFFFFFFFF)  Item = (%X,%X): \n", DObject.Group, DObject.Element);

					while ( (DObject.Group != 0xFFFE) || (DObject.Element != 0xE0DD) ||
						(DObject.Length != 0x00000000) )
					{
						DObject.Group   = ExtractUNSIGNED_SHORT( pFileData, lDataPos );  
						lDataPos += sizeof( DObject.Group );
						DObject.Element = ExtractUNSIGNED_SHORT( pFileData, lDataPos );
						lDataPos += sizeof( DObject.Element );
						DObject.Length  = ExtractUNSIGNED_INT( pFileData, lDataPos );
						lDataPos += sizeof( DObject.Length );

						lDataPos = lDataPos - sizeof( DObject.Element ) - sizeof( DObject.Length );

						// printf("%X ", DObject.Group);
					} 

					lDataPos = lDataPos + sizeof( DObject.Element ) + sizeof( DObject.Length );

				}

				lDataPos = lDataPos + DObject.Length;

				// printf("\n-------------- (###)  DObject.Length = %d (%X) (%X,%X)\n", DObject.Length, DObject.Length, DObject.Group, DObject.Element);


			} while (( (DObject.Group != 0xFFFE) || (DObject.Element != 0xE0DD) ||
				(DObject.Length != 0x00000000) ));//||(!( (DObject.Group != 0xFFFE) && (DObject.Element != 0xE000) &&  (DObject.Length != 0xFFFFFFFF) )));
		}





		lDataPos = lDataPos + DObject.Length;


		if(ie33_3DWatchData==true)
		{
			Finfo.iDataEndPos=lDataPos;
			ie33_3DWatchData=false;
		}


	}

	return false;

}; 
//-------------------------------------
bool DicomInfo::ExtractDicomValuesFromTag(unsigned short group, unsigned short element, gdcm::DataSet *dataset,QString mode,void *data,int length)
{

	 gdcm::Tag t (group, element );
	 bool tester=(*dataset).FindDataElement( t);
	 if( tester)	
	 {
		 const gdcm::DataElement &de = (*dataset).GetDataElement( t );
		
		 if ((de.GetByteValue()!=NULL)&&(de.GetByteValue()->GetPointer()!=NULL))
		{
	
			int len=de.GetByteValue()->GetLength();
			if(len<512)
			{
			 char *origData=((char *) de.GetByteValue()->GetPointer());;
			 if (mode.compare("US")==0)
			 {
				 *((unsigned short *)data) =*((unsigned short *) de.GetByteValue()->GetPointer());
			 }
			 else
			 if (mode.compare("DS")==0)
			 {
				
				/* double *tab=(double*)data;
				 char *test=strtok(origData,"\\");

				 int ll=length;
				 for(int i=0;i<ll;i++)
				 {
					
					sscanf(test,"%lf",&tab[i]);
					test= strtok(NULL,"\\");*/
				 double *tab=(double*)data;
				 const char *text = reinterpret_cast < const char* >( origData );

				 QString tt(text);
				 tt.resize(len); 
				 QStringList fields = tt.split("\\", QString::SkipEmptyParts);
				 int count=fields.count();

				 for(int i=0;i<count;i++)
				 {
					 tab[i]=fields[i].toDouble();
				 }
				// s.readraw
					
				 //}
				
			 }
			 else
			if	(mode.compare("IS")==0)
			{
				/*int *tab=(int*)data;
				 char *test=strtok(origData,"\\");

				 int ll=length;
				 for(int i=0;i<ll;i++)
				 {
					
					sscanf(test,"%d",&tab[i]);
					test= strtok(NULL,"\\");
					
				 }*/
				 int *tab=(int*)data;
				 const char *text = reinterpret_cast < const char* >( origData );

				 QString tt(text);
				 tt.resize(len); 
				 QStringList fields = tt.split("\\", QString::SkipEmptyParts);
				 int count=fields.count();

				 for(int i=0;i<count;i++)
				 {
					 tab[i]=fields[i].toDouble();
				 }

			}
			else
				if
				((mode.compare("UL")==0))
			 {
				 unsigned long *tab=(unsigned long*)data;
				
				 for(int i=0;i<len/sizeof(unsigned long);i++)
				 {
					 
					 tab[i]=((unsigned long*) de.GetByteValue()->GetPointer())[i];
					
				 }
				// *((double *)data) =*((double*) de.GetByteValue()->GetPointer());
			 }
			 else 
			 if ((mode.compare("DA")==0)||
				 (mode.compare("CS")==0)||
				 (mode.compare("LO")==0)||
				 (mode.compare("PN")==0)||
				 (mode.compare("AS")==0)||
				 (mode.compare("SH")==0)||
				 (mode.compare("UI")==0))
				
				
			 {
			
				 strcpy((char*)data, de.GetByteValue()->GetPointer());
				 	((char*)data)[len] = '\0';
			 }


			 return true;
			}
		}

		
	 }

	 ////zeruj jesli nic nie znajdziesz
	 // if (mode.compare("US")==0)
		//	 {
		//		 *((unsigned short *)data) =0;
		//	 }
		//	 else
		//	 if (mode.compare("DS")==0)
		//	 {
		//		 double *tab=(double*)data;
		//		
		//		

		//	
		//		 for(int i=0;i<length;i++)
		//		 {
		//			tab[i]=0;
		//		 }
		//		
		//	 }
		//	 else
		//	if	(mode.compare("IS")==0)
		//	{
		//		int *tab=(int*)data;
		//		tab[0]=0;
		//	}
		//	else
		//	if	(mode.compare("UL")==0)
		//	 {
		//		 int *tab=(int*)data;
		//		
		//		 tab[0]=0;
		//		
		//	 }
		//	 else 
		//	 if ((mode.compare("DA")==0)||
		//		 (mode.compare("CS")==0)||
		//		 (mode.compare("LO")==0)||
		//		 (mode.compare("PN")==0)||
		//		 (mode.compare("AS")==0)||
		//		 (mode.compare("SH")==0)||
		//		 (mode.compare("UI")==0))
		//		
		//		
		//	 {
		//	
		//		 	((char*)data)[0] = '\0';
		//	 }














	 return false;
}
//---------------------------------------------------
  bool DicomInfo::ExtractDicomFileInfo(const char *name, DicomFileInfo& fileInfo )
  {


	gdcm::Reader reader;
	reader.SetFileName( name);
	bool test=reader.Read();

	if (test ==true)
	{
		gdcm::File &file = reader.GetFile();
		gdcm::DataSet &ds = file.GetDataSet();
	//	//cout <<"d";
		//ExtractDicomValuesFromTag(0x0020, 0x0032,&ds,"DS",(void *)&fileInfo.vImagePosition,3);////cout <<"d";
		//ExtractDicomValuesFromTag(0x0020, 0x0011,&ds,"IS",(void *)&fileInfo.iSeriesNumber);////cout <<"d";
		//	ExtractDicomValuesFromTag(0x0020, 0x0037,&ds,"DS",(void *)&fileInfo.vImageOrientation,6);////cout <<"d";
		// gdcm::Tag t (0x0028, 0x0011 );
		// int v;
		//if( ds.FindDataElement( t))		
		//{
		//
		//	const gdcm::DataElement &de = ds.GetDataElement( t );
		//	fileInfo.iColumns=*((unsigned short *) de.GetByteValue()->GetPointer());
		//	std:://cout << de.GetValue();
		//	//fileInfo.=(unsigned short)(de.GetByteValue()->GetPointer());
		//	
		
		//ds checked
		//LO checked
		//us checked
		// da checked
		// ui checked

		//cs checked
		// pn checked
		// as checked
		//is checked

		//}

		//ExtractDicomValuesFromTag(0x0028, 0x0011,&ds,"US",(void *)&fileInfo.iColumns);
		ExtractDicomValuesFromTag(0x0008, 0x0020,&ds,"DA",(void *)&fileInfo.chStudyDate);//cout <<"d";
		ExtractDicomValuesFromTag(0x0008, 0x0060,&ds,"CS",(void *)&fileInfo.chModality);//cout <<"d";
		ExtractDicomValuesFromTag(0x0008, 0x0070,&ds,"LO",(void *)&fileInfo.chManufacturer);//cout <<"d";
		ExtractDicomValuesFromTag(0x0008, 0x0080,&ds,"LO",(void *)&fileInfo.chInstitutionName);//cout <<"d";
		ExtractDicomValuesFromTag(0x0008, 0x103E, &ds, "LO", (void *)&fileInfo.chSeriesDescription);//cout <<"d";
		ExtractDicomValuesFromTag(0x0008, 0x1030, &ds, "LO", (void *)&fileInfo.chStudyDescription);//cout <<"d";


		ExtractDicomValuesFromTag(0x0010, 0x0010,&ds,"PN",(void *)&fileInfo.chPatientName);//cout <<"d";
		ExtractDicomValuesFromTag(0x0010, 0x0020,&ds,"LO",(void *)&fileInfo.chPatientID);//cout <<"d";
		ExtractDicomValuesFromTag(0x0010, 0x1010,&ds,"AS",(void *)&fileInfo.chPatientAge);//cout <<"d";




		ExtractDicomValuesFromTag(0x0010, 0x0030,&ds,"DA",(void *)&fileInfo.chPatientBirthDate);//cout <<"d";
		ExtractDicomValuesFromTag(0x0010, 0x0040,&ds,"CS",(void *)&fileInfo.chPatientSex);//cout <<"d";
		ExtractDicomValuesFromTag(0x0010, 0x1020,&ds,"DS",(void *)&fileInfo.fPatientSize,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0010, 0x1030,&ds,"DS",(void *)&fileInfo.fPatientWeight,1);//cout <<"d";

		ExtractDicomValuesFromTag(0x0020, 0x0010,&ds,"SH",(void *)&fileInfo.chStudyID);//cout <<"d";
		ExtractDicomValuesFromTag(0x0020, 0x0011,&ds,"IS",(void *)&fileInfo.iSeriesNumber);//cout <<"d";
		ExtractDicomValuesFromTag(0x0020, 0x0013,&ds,"IS",(void *)&fileInfo.iImageNumber);//cout <<"d";
		ExtractDicomValuesFromTag(0x0020, 0x0032,&ds,"DS",(void *)&fileInfo.vImagePosition,3);//cout <<"d";

		////cout <<"X\t" << fileInfo.iImageNumber << "\t" << fileInfo.vImagePosition[0] << "\t" << fileInfo.vImagePosition[1] << "\t" << fileInfo.vImagePosition[2] << "\n";
		ExtractDicomValuesFromTag(0x0020, 0x0037,&ds,"DS",(void *)&fileInfo.vImageOrientation,6);//cout <<"d";
		ExtractDicomValuesFromTag(0x0020, 0x0052,&ds,"UI",(void *)&fileInfo.chFrameOfReferenceUID);//cout <<"d";
		ExtractDicomValuesFromTag(0x0020, 0x1041,&ds,"DS",(void *)&fileInfo.fSliceLocation,1);//cout <<"d";

		ExtractDicomValuesFromTag(0x0028, 0x0004,&ds,"CS",(void *)&fileInfo.chPhotometricInterpretation);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x0010,&ds,"US",(void *)&fileInfo.iRows);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x0011,&ds,"US",(void *)&fileInfo.iColumns);//cout <<"d";


		ExtractDicomValuesFromTag(0x3010, 0x1001,&ds,"UL",(void *)&fileInfo.iSlices);//cout <<"d";
		//Number of Frames (0028,0008)
		ExtractDicomValuesFromTag(0x3010, 0x0010,&ds,"LO",(void *)&fileInfo.chManufacturer3D);//cout <<"d";


		ExtractDicomValuesFromTag(0x0028, 0x0030,&ds,"DS",(void *)&fileInfo. fPixelSpacing,2);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x1050,&ds,"DS",(void *)&fileInfo. fWindowCenter,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x1051,&ds,"DS",(void *)&fileInfo. fWindowWidth,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x1052,&ds,"DS",(void *)&fileInfo. fRescaleIntercept,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x1053,&ds,"DS",(void *)&fileInfo. fRescaleSlope,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x1054,&ds,"LO",(void *)&fileInfo. chRescaleType,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x0100,&ds,"US",(void *)&fileInfo. uBitsAllocated,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x0101,&ds,"US",(void *)&fileInfo. uBitsStored,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x0102,&ds,"US",(void *)&fileInfo. uHighBit,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0028, 0x0103,&ds,"US",(void *)&fileInfo. uPixelRepresentation,1);//cout <<"d";



		ExtractDicomValuesFromTag(0x0018, 0x0050,&ds,"DS",(void *)&fileInfo. fSliceThickness,1);//cout <<"d";
		ExtractDicomValuesFromTag(0x0018, 0x0088,&ds,"DS",(void *)&fileInfo. fSpacingBetweenSlices,1); //cout <<"d";
		ExtractDicomValuesFromTag(0x0018, 0x1120,&ds,"DS",(void *)&fileInfo. fGantryTilt,1);//cout <<"d";
	//	ExtractDicomValuesFromTag(0x0002, 0x0010,&ds,"UI",(void *)&fileInfo. chTransferSyntaxUID);//nie lapie i git - w dokumnentacji do readera napisano, ze nie czyta 0x0002



		ExtractDicomValuesFromTag(0x0008, 0x0040,&ds,"LO",(void *)&fileInfo.chDataSetType);//cout <<"d";
		ExtractDicomValuesFromTag(0x0008, 0x0041,&ds,"LO",(void *)&fileInfo.chDataSetSubtype);//cout <<"d";

		ExtractDicomValuesFromTag(0x0018, 0x0081,&ds,"DS",(void *)&fileInfo.fEchoTime);//cout <<"d";
		ExtractDicomValuesFromTag(0x0018, 0x1030,&ds,"LO",(void *)&fileInfo.chProtocolName);//cout <<"d";

		ExtractDicomValuesFromTag(0x7FE0, 0x0010,&ds,"US",(void *)&fileInfo. uPixelRepresentation,1);
		if (strcmp(fileInfo.chTransferSyntaxUID,"1.2.840.10008.1.2") == 0)
		{
			fileInfo.bPixelDataLittleEndian = true;  // Implicit VR Little Endian
		}
		else 	if (strcmp(fileInfo.chTransferSyntaxUID,"1.2.840.10008.1.2.1") == 0)
		{
			fileInfo.bPixelDataLittleEndian = true;  // Explicit VR Little Endian 
		}
		else	if (strcmp(fileInfo.chTransferSyntaxUID,"1.2.840.10008.1.2.2") == 0 )           
		{
			fileInfo.bPixelDataLittleEndian = false;  // Explicit VR Big Endian
		}
		else if (	(strcmp(fileInfo.chTransferSyntaxUID,"1.2.840.10008.1.2.4.70") == 0) || // DICOM_LOSSLESS_JPEG
					(strcmp(fileInfo.chTransferSyntaxUID,"1.2.840.10008.1.2.4.50") == 0) || //DICOM_LOSSY_JPEG_8BIT
					(strcmp(fileInfo.chTransferSyntaxUID,"1.2.840.10008.1.2.4.51") == 0))// || //DICOM_LOSSY_JPEG_16BIT
					//(strcmp(Finfo.chTransferSyntaxUID,"1.2.840.113619.5.2") == 0) )		//DICOM_GE_PRIVATE_IMPLICIT_BIG_ENDIAN
		{
			//dane z kompresja trzeba ustawic flage zeby wczytywac przez vtk!!!!
			fileInfo.jpegCompression=true;
			fileInfo.bPixelDataLittleEndian = true; //nie wiem - sprawdze czy cos wniesie
		}



			if (fileInfo.fPixelSpacing[0]==0.0)			fileInfo.fPixelSpacing[0]=1.0;
			if (fileInfo.fPixelSpacing[1]==0.0)			fileInfo.fPixelSpacing[1]=1.0;
			if (fileInfo.fSliceThickness==0.0)			fileInfo.fSliceThickness=1.0;
			if (fileInfo.fSpacingBetweenSlices==0.0)	fileInfo.fSpacingBetweenSlices=1.0;

			fileInfo.lPixelDataLength=(unsigned int)(fileInfo.iColumns*fileInfo.iRows*fileInfo.iSlices*fileInfo.uBitsAllocated/8);//cout <<"d";
		return true;
	}
	 
	return false;



	//const DataSet &ds = file.GetDataSet();
 //Attribute<0x0020,0x0032> at;
 //at.Set( ds );
 //if( at.GetValue() == 0.0 ) exit(1);

 // Attribute<0x28,0x100> at;
 //at.SetFromDataElement( ds.GetDataElement( at.GetTag() ) );
 //if( at.GetValue() != 8 ) exit(1);
	//  

	/*    sf=gdcm.StringFilter()
      sf.SetFile(r.GetFile())
      print sf.ToStringPair(gdcm.Tag(0x0028,0x0010))*/


  }



