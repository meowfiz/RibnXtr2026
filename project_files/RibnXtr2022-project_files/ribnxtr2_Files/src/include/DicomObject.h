#ifndef DICOM_OBJECT
#define DICOM_OBJECT


#include "DicomTypeDef.h"


class DicomObject  
{

	public:

	  unsigned short			Group;
	  unsigned short			Element;
	  unsigned int				Length;      // always even numbered
	  void						*pData;      // pointer to the data
	  EValueRepresentationType   ValueType;   // type of the Value Representation


	  DicomObject();
	  virtual ~DicomObject();


	  bool  operator  >  (DicomObject  &vr);
	  bool  operator  <  (DicomObject  &vr);
	  bool  operator  ==  (DicomObject  &vr);


};

#endif // DICOM_OBJECT
