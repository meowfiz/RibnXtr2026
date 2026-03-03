

#ifndef _DICOMTYPEDEF_H_
#define _DICOMTYPEDEF_H_


struct SVector3D 
{  
  // doubleing point vector type

  double  x;
  double  y; 
  double  z;

};


// Image Orientation (0020,0037) specifies the direction cosines of the 

struct SImageOrientationVector
{

  // first row:
  double  rx1;  
  double  ry1; 
  double  rz1;

  // and the first column:
  double  cx1;
  double  cy1; 
  double  cz1;



};

//-------------------------------------

struct DicomFileInfo
{

  char                      chStudyDate[10];           // 0x0008, 0x0020, DA        
  char                      chModality[16];            // 0x0008, 0x0060, CS          
  char                      chManufacturer[64];        // 0x0008, 0x0070, LO
  char                      chInstitutionName[64];     // 0x0008, 0x0080, LO
  char						chSeriesDescription[64];   // 0x0008, 0x103E,LO
  char						chStudyDescription[64];   // 0x0008, 0x1030,LO


	char                      chPatientName[64];         // 0x0010, 0x0010, PN
	char                      chPatientID[64];           // 0x0010, 0x0020, LO
	char                      chPatientAge[6];           // 0x0010, 0x1010, AS

	char					chPatientBirthDate[10]; 	//{ 0x0010, 0x0030, 'DA', "Patient's Birth Date" },
	char					chPatientSex[16];			//{ 0x0010, 0x0040, 'CS', "Patient's Sex" },
	double					fPatientSize;				//{ 0x0010, 0x1020, 'DS', "Patient's Size" },
	double					fPatientWeight;				//{ 0x0010, 0x1030, 'DS', "Patient's Weight" },


  char                      chStudyID[16];             // 0x0020, 0x0010, SH
  int                       iSeriesNumber;             // 0x0020, 0x0011, IS
  int                       iImageNumber;              // 0x0020, 0x0013, IS
  //SVector3D                 vImagePosition;            // 0x0020, 0x0032, DS, "3"
  double vImagePosition[3];
  
  double   vImageOrientation[6];         // 0x0020, 0x0037, DS, "6"
  char                      chFrameOfReferenceUID[64]; // 0x0020, 0x0052, UI
 
  double                     fSliceLocation;            // 0x0020, 0x1041, DS
  char                      chPhotometricInterpretation[16]; // 0x0028, 0x0004, CS 
  unsigned short                       iRows;                     // 0x0028, 0x0010, US
  unsigned short                       iColumns;                  // 0x0028, 0x0011, US
 
  //////////
  //ie33 tags
  int						iNumberOfSubVolumes;
  int						iCode;
  int						idR,idPhi,idTheta,idT;
  int						idR0,idPhi0,idTheta0,idT0;
  double						fsR,fsPhi,fsTheta,fsT;
  int						iSubvolumeSize;
  double						fFrequency;
  int						iDataPos;
  int						iDataEndPos;
  bool				jpegCompression;
 //----------------------

  int						iSlices;					// 0x3010, 0x1001, UL
  char						chManufacturer3D[64];		// 0x3010, 0x0010, LO
  unsigned int				RealDataLength;
  /////////////
  
  
//  double                     fPixelSpacingRow;          // 0x0028, 0x0030, DS, "2"
 // double                     fPixelSpacingColumn;       // 0x0028, 0x0030, DS, "2"

  double					fPixelSpacing[2];
  double                     fWindowCenter;             // 0x0028, 0x1050, DS,
  double                     fWindowWidth;              // 0x0028, 0x1051, DS,
  double                     fRescaleIntercept;         // 0x0028, 0x1052, DS, 
  double                     fRescaleSlope;             // 0x0028, 0x1053, DS, 
  char                      chRescaleType[64];         // 0x0028, 0x1054, LO
  unsigned short            uBitsAllocated;            // 0x0028, 0x0100, US
  unsigned short            uBitsStored;               // 0x0028, 0x0101, US
  unsigned short            uHighBit;                  // 0x0028, 0x0102, US
  unsigned short            uPixelRepresentation;      // 0x0028, 0x0103, US 

  double                     fSliceThickness;           // 0x0018, 0x0050, DS
  double                     fSpacingBetweenSlices;     // 0x0018, 0x0088, DS 
  double                     fGantryTilt;               // 0x0018, 0x1120, DS
  char                      chTransferSyntaxUID[64];   // 0x0002, 0x0010, UI
  long                      lPixelDataOffset;          // 0x7FE0, 0x0010, OX 
  long                      lPixelDataLength;			 // Size of the image (in bytes)
  bool                      bPixelDataLittleEndian;    // Is the pixel data in little endian format ?

  char						chDataSetType[64];		   // 0x0008, 0x0040, LO
  char						chDataSetSubtype[64];	   // 0x0008, 0x0041, LO
  double						fEchoTime;				   // 0x0018, 0x0081, DS
  char						chProtocolName[64];		   // 0x0018, 0x1030, LO

    int						minValue;


	int tmpSeries;
};


//-------------------------------------


// DICOM Value Representations:

enum EValueRepresentationType
{  
  AE,  // Application Entity 
  AS,  // Age String 
  AT,  // Attribute Tag 
  CS,  // Code String 
  DA,  // Date 
  DS,  // Decimal String 
  DT,  // Date Time 
  FD,  // Floating point Double 
  FL,  // FLoating point single
  IS,  // Integer String 
  LO,  // LOng string 
  LT,  // Long Text 
  OB,  // Other Byte string
  OW,  // Other Word string
  OX,  // OB or OW depending on context 
  PN,  // Person Name 
  SH,  // Short String 
  SL,  // Signed Long 
  SQ,  // SeQuence of items 
  SS,  // Signed Short 
  ST,  // Short Text 
  TM,  // Time 
  UI,  // Unique Identifier (UID) 
  UL,  // Unsigned Long 
  US,  // Unsigned Short 
  UN,  // Unknown Value Representation - defined in supplement 14 
  UT,  // Unlimited Text - defined in CP 101 & CP 122 - needed 
       // for Structured Reporting (SR) 
  XS,  // SS or US depending on context 
  VS,  // Virtual String - defined in CP 101 
  NA,   // "not applicable", for data which has no VR 
  XX,
  NN
};


#endif
