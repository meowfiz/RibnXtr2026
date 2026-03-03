#ifndef	DICOM_DATASET
#define	DICOM_DATASET



#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	<qapplication.h>
#include	<qstring.h>
#include	<qmessagebox.h>
#include	<qfile.h>

#include	"RawDataSet.h"
#include	"DicomTypeDef.h"
#include	"DicomInfo.h"
#include	"DicomObject.h"

#include <gdcmReader.h>

#include <gdcmFile.h>

#include <gdcmImageReader.h>
#include <gdcmImage.h>
#include "gdcmFile.h"
#include <gdcmDataSet.h>
#include <gdcmReader.h>
//#include	<vtkGDCMImageReader.h>
#include	<vtkImageCast.h>

#include	<vtkImageData.h>
#include	<vtkSmartPointer.h>
#include <QTextCodec>
#include	<zlib.h>

//using namespace gdcm;

class DicomDataSet :public QObject
{
	Q_OBJECT

	public:
		
		
		DicomDataSet(const QString &,bool ShInfo=false);
		DicomDataSet();
		
		//RawDataSet *
		void GetRawDataSet(bool inv=false,bool shift=false,int shiftVal=0);
		RawDataSet* GetRawDataSet2(bool inv = false, bool shift = false, int shiftVal = 0);
		void ShowInfo();
		void InitEmptyInfo();

		bool LittleEndianTest();

		void SetNewFile(const QString &);
		void GetID();


		bool IsDicomFile(const QString &) const;
		void CompareTwoDicomFiles(DicomDataSet *d1,DicomDataSet *d2);
		void CompareWithFile(DicomDataSet *d1);

		int testVal;

		QString CompareTwoStrings(QString Title,QString A, QString B);

		bool Init;
		int  NumberOfSlices;
		bool SelectedData;

		QString				FileName;
		QString				DataSetID;
		unsigned char       *Header;

		
	 
		void GetPixelData(unsigned short *Data,int nr,bool inv=false,bool shift=false,int shiftVal=0, int globalMinValue=0);
		//void GetPixelData(unsigned short *Q,int nr,unsigned char *tab,bool inv=false);

		unsigned short     *Z; 
  
		bool Associated;

		DicomFileInfo    Finfo;
//		DicomInfo        DicomInfo;
		DicomInfo DicomInf;

		int                Qmax, Qmin, HPN, VPN, 
							MinDisplayIntensity, MaxDisplayIntensity;
		bool                little_endian;
		char               Input[2], c;
		FILE               *Fin, *Fout;
		long				lDataLength;
	
		union tag
		{
		  unsigned short  word;
		  signed   short  sword;
		  unsigned char   byte[2];
		} tag;

signals:
		void SignalSendRawDataSet(RawDataSet *);
		void sendShiftValue(const QString &);

};

#endif //DICOM_DATASET
