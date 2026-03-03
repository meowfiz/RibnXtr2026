#ifndef	BYTE_MASK_COLLECTION
#define	BYTE_MASK_COLLECTION


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qstring.h>
#include	<qtreewidget.h>
#include "LinkedList.h"


#include "OtherCommonFunctions.h"

class ByteMask
{


public:

	ByteMask(int vSize)
	{
		int *EmptyMask=new int[4];
		*(EmptyMask)=0;
		*(EmptyMask+1)=2; //2004.02.05
		*(EmptyMask+2)=0;
		*(EmptyMask+3)=vSize; 
		data=EmptyMask;
		path="";
		treeItem = NULL;
		colorIndex = -1;
		
	};

	ByteMask(int *tab,const QString p="")
	{
		//if (data!=NULL) delete data;
		data=tab;
		path=p;
		treeItem = NULL;
		colorIndex = -1;

	
		


	};

	ByteMask(const ByteMask	&mask)
	{
		
		treeItem = mask.treeItem;
		path = mask.path;
		groupName=mask.groupName;
		maskName=mask.maskName;
		colorIndex = mask.colorIndex;

		int size = *(mask.data + 1) + 4;
		data = new int[size];
		memcpy(data, mask.data, size * sizeof(int));

	}

	~ByteMask()
	{
		if (data != NULL)
		{

			delete data;
		}
		if (treeItem != NULL)
		{
			delete treeItem;
		}
	};
	int *getMaskData(){return data;};
	QString getPath(){return path;};

	void setMaskData(int *tab)
	{
		if (data!=NULL) delete data;
		data=tab;
	};
	void setPath(QString p){path=p;};
	 

	void SetTreeItem(QTreeWidgetItem *item) { treeItem = item; };
	QTreeWidgetItem *GetTreeItem() { 
		return treeItem;
	};

	void SetGroupName(QString name) { groupName = name; };
	QString GetGroupName() { return groupName; };

	void	SetColorIndex(int col) { colorIndex = col; };
	int		GetColorIndex() { return colorIndex; };

	void SetMaskName(QString name) { maskName = name; };
	QString GetMaskName() { return maskName; };
private:
	int *data;
	QString path;
	QTreeWidgetItem *treeItem;
	QString groupName;
	QString maskName;
	int		colorIndex;
	

};

class ByteMaskCollection
{
	
	public:
	
		ByteMaskCollection(int,int,bool createFirst=true);
		~ByteMaskCollection();


		

		unsigned char	*UndoMask();
		unsigned char	*RedoMask();
		unsigned char	*GetMask();

		void UndoMask(unsigned char *maskDest);
		void RedoMask(unsigned char *maskDest);
		void GetMask(unsigned char *maskDest);


		void	InsertNewMask(unsigned char *);
		void	AddNewMask(unsigned char *);
		void	DeleteAllMasks();
		void	DeleteMaskAtPos(int pos);
	
		bool	UndoPossible();
		bool	RedoPossible();
	
	
		int setVolumeSize(int s) { VolumeSize = s; };///potrzebne gdy tworzone na pusto }

		void	SetNumberOfMasks(int);
		int		GetNumberOfMasks();
		int		*GetCurrentMask();
		ByteMask		*GetCurrentByteMask() { return CompressedMaskList->GetCurrentListItem()->GetData(); };
		ByteMask		*GetMaskDataAtPos(int pos);
		unsigned char	*GetCurrentMaskData();
		unsigned char	*GetMaskDataAtPosAndSetAsCurrent(int pos);

		void GetCurrentMaskData(unsigned char *maskDest);
		void GetMaskDataAtPosAndSetAsCurrent(int pos, unsigned char *maskDest);
		bool	LoadMaskDataSet(const QString &, RawDataSet *data = NULL, bool fix2D = false, QString fix2DPath = "");
		bool	SaveMaskDataSet( const QString &filename,int nr=-1, RawDataSet *data = NULL);

		bool	SaveMasks(const QString &filename, RawDataSet *data = NULL);
		bool	LoadMasks(const QString &filename, RawDataSet *data = NULL,bool fix2D=false,QString fix2DPath="");

		int		GetCurrentMaskNumber(){return CompressedMaskList->GetCurrentItemPos();};
		bool	LoadMaskDataSetAndCombineWithCurrent( const QString &filename,int Mode,unsigned char *CurrentMask, RawDataSet *data = NULL);
	

	

		LinkedList<ByteMask*> *GetMaskList() { return CompressedMaskList; };
		


	private:

		
		int		VolumeSize;
		int		NumberOfMaxItems;			
		int		*CurrentMask;

		//LinkedList<int*> *CompressedMaskList; 
		LinkedList<ByteMask*> *CompressedMaskList; 


	
		int		*CompressMaskExternal(unsigned char*);
		void	CompressMask(unsigned char*);

		unsigned char	*DecompressMask(int * tab);
		unsigned char	*DecompressMask();
		
	
		void DecompressMask(int * tab, unsigned char *destMask);
		void DecompressMask( unsigned char *destMask);


	
	
};



#endif //BYTE_MASK_COLLECTION
