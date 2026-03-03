#ifndef	DATASET
#define	DATASET


#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"LinkedList.h"
#include	"RawDataSet.h"
#include	"ByteMaskCollection.h"
#include	"OtherCommonFunctions.h"
#include	<qtreewidget.h>

class DataSet
{
		private:

		bool isDataSelected;

		RawDataSet *RawData;
		ByteMaskCollection *Masks;
		ByteMaskCollection *Masks2;
		ByteMaskCollection *InternalByteMaks;
		PaletteRGBA *Palette;
		int PaletteNr;
		bool PaletteInv;
		unsigned char Opacity;
		unsigned char *InspectionArray0; //transversal
		unsigned char *InspectionArray1; //coronal
		unsigned char *InspectionArray2; //sagittal
		int InspectionMode;
		bool InspectionEnabled;
		int PosX,PosY,PosZ;
		int InspX,InspY,InspZ;
		
		QTreeWidgetItem *treeItem;
		int dataCounter;
		
	public:

	

		DataSet(RawDataSet*, PaletteRGBA *,int);
		~DataSet();

		int		GetDataCounter(){return dataCounter;};
		void	SetDataCounter(int nr){dataCounter=nr;};

		QTreeWidgetItem *GetTreeWidgetItem(){return treeItem;};;
		void	SetTreeWidgetItem(QTreeWidgetItem *t,int m){treeItem=t;dataCounter=m;};

		void SetNewPalette(PaletteRGBA *_Palette,int PalNr=-1,bool Inv=false);
	
	

		void SetDataSelected(bool a){isDataSelected=a;};
	

		void SetRawData(RawDataSet *_RawData){RawData=_RawData;};
		void SetMasks(ByteMaskCollection *_Masks){Masks=_Masks;};
		void SetMasks2(ByteMaskCollection *_Masks2){Masks2=_Masks2;};
		void SetPalette(PaletteRGBA *_Palette){Palette=_Palette;};
		void SetPaletteNr(int _PaletteNr){PaletteNr=_PaletteNr;};
		void SetPaletteInv(bool _PaletteInv){PaletteInv=_PaletteInv;};
		void SetOpacity(unsigned char _Opacity){Opacity=_Opacity;};
	
		void SetInspectionArray0(unsigned char *_InspectionArray0){InspectionArray0=_InspectionArray0;};
		void SetInspectionArray1(unsigned char *_InspectionArray1){InspectionArray1=_InspectionArray1;};
		void SetInspectionArray2(unsigned char *_InspectionArray2){InspectionArray2=_InspectionArray2;};
		void SetInspectionMode(int _InspectionMode){InspectionMode=_InspectionMode;};
		void SetInspectionEnabled(bool _InspectionEnabled){InspectionEnabled=_InspectionEnabled;};
		void SetPosXYZ(int x,int y,int z){PosX=x;PosY=y;PosZ=z;};
		void SetInspXYZ(int x,int y,int z){InspX=x;InspY=y;InspZ=z;};


		void DeleteInspectionArray0(){if (InspectionArray0!=NULL) {delete []InspectionArray0; InspectionArray0=NULL;}};
		void DeleteInspectionArray1(){if (InspectionArray1!=NULL) {delete []InspectionArray1; InspectionArray1=NULL;}};
		void DeleteInspectionArray2(){if (InspectionArray2!=NULL) {delete []InspectionArray2; InspectionArray2=NULL;}};

		bool GetDataSelected(){return isDataSelected;};
		RawDataSet *GetRawData(){return RawData;};
		ByteMaskCollection *GetMasks(){return Masks;};
		ByteMaskCollection *GetMasks2(){return Masks2;};

		ByteMaskCollection *GetByteMaskInternal() { return InternalByteMaks; };

		void	SwapMasksSets(){ByteMaskCollection *tmp=Masks;Masks=Masks2;Masks2=tmp;};

		PaletteRGBA *GetPalette(){return Palette;};
		int GetPaletteNr(){return PaletteNr;};
		bool GetPaletteInv(){return PaletteInv;};
		unsigned char GetOpacity(){return Opacity;};


	

		unsigned char *GetInspectionArray0(){return InspectionArray0;};
		unsigned char *GetInspectionArray1(){return InspectionArray1;};
		unsigned char *GetInspectionArray2(){return InspectionArray2;};
		int GetInspectionMode(){return InspectionMode;};
		bool GetInspectionEnabled(){return InspectionEnabled;};
	
		int GetPosX(){return PosX;};
		int GetPosY(){return PosY;};
		int GetPosZ(){return PosZ;};
		int GetInspX(){return InspX;};
		int GetInspY(){return InspY;};
		int GetInspZ(){return InspZ;};







		//int PaaddingSize;
		//RawDataSet *PaddedDataSet;
		//-------------------------------



};




#endif //DATASET
