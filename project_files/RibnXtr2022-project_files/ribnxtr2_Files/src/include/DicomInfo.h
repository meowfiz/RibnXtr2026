#ifndef DICOM_INFO
#define DICOM_INFO


#include "DicomTypeDef.h"
#include "OtherCommonFunctions.h"
#include <gdcmDataSet.h>
#include <gdcmByteValue.h>
#include <gdcmReader.h>
#include <qtextstream.h>
#include <qstring.h>
//#include <gdcmStringFilter.h>
//#include <gdcmAttribute.h>

class DicomInfo  
{
	public:
	  DicomInfo();
	  virtual ~DicomInfo();

	  char				AuxString[512];
	  unsigned short	m_uActualGroup;
	  bool				m_bPART10_PREAMBLE;
	  char				Aux[32];
	  char				Aux64[64];
	  bool				ie33_3DData;	
	  bool				ie33_3DDataExtracted;	
	  bool				ie33_3DWatchData;


	  

	  unsigned short	ExtractUNSIGNED_SHORT( unsigned char* pFileData, unsigned int lDataPos );
	  unsigned int		ExtractUNSIGNED_INT( unsigned char* pFileData, unsigned int lDataPos );
	  bool				ExtractDicomFileInfo( unsigned char *pFileData, unsigned int lDataLength, DicomFileInfo& fileInfo ); 

	  bool ExtractDicomFileInfo(const char *name, DicomFileInfo& fileInfo ); 
	  bool ExtractDicomValuesFromTag(unsigned short group, unsigned short element, gdcm::DataSet *dataset,QString mode,void *data,int length=1);
};

#endif // DICOM_INFO
