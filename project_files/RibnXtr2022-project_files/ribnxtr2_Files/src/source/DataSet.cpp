#include "DataSet.h"

DataSet::DataSet(RawDataSet *_Data, PaletteRGBA *_Palette,int _NumberOfMaxItems)
{
	RawData=_Data;
	Masks=new ByteMaskCollection(RawData->GetVolumeSize(),_NumberOfMaxItems);
	Masks2=new ByteMaskCollection(RawData->GetVolumeSize(),_NumberOfMaxItems);
	InternalByteMaks = new ByteMaskCollection(RawData->GetVolumeSize(), _NumberOfMaxItems,false);

	Opacity=0;
	 
	Palette=new PaletteRGBA[256];
	PaletteNr=-1;
	PaletteInv=false;

	SetNewPalette(_Palette);

	//		PaaddingSize=0;
	//		PaddedDataSet=NULL;

	InspectionArray0=NULL; //transversal
	InspectionArray1=NULL; //coronal
	InspectionArray2=NULL; //sagittal

	InspectionEnabled=false;
	InspectionMode=0;
	PosX=0;PosY=0;PosZ=0;
	InspX=1;InspY=1;InspZ=1;

	treeItem=NULL;


}

//-------------------------------------------------------------

DataSet::~DataSet()
{
	if (treeItem!=NULL) delete []treeItem;
	if (RawData!=NULL)			delete RawData;
	if (InternalByteMaks != NULL) delete InternalByteMaks;
	if (Masks!=0)				delete Masks;
	if (Masks2!=0)				delete Masks2;
	if (Palette!=0)				delete Palette;
	//	if (PaddedDataSet!=NULL)	delete PaddedDataSet;
	if (InspectionArray0!=NULL)	delete []InspectionArray0;
	if (InspectionArray1!=NULL)	delete []InspectionArray1;
	if (InspectionArray2!=NULL)	delete []InspectionArray2;




	RawData=NULL;
	Masks=NULL;
	Masks2=NULL;
	Palette=NULL;
	//PaddedDataSet=NULL;
	InspectionArray0=NULL;
	InspectionArray1=NULL;
	InspectionArray2=NULL;


}


void DataSet::SetNewPalette(PaletteRGBA *_Palette,int PalNr,bool PalInv)
{

	int i;
	if (_Palette!=NULL)
		for( i=0;i<256;i++)
		{
			Palette->R[i]=_Palette->R[i];
			Palette->G[i]=_Palette->G[i];
			Palette->B[i]=_Palette->B[i];
		}

		if (PalNr>=0)
		{
			PaletteNr=PalNr;
		}
		PaletteInv=PalInv;

}
//-------------------------------------------------------------
