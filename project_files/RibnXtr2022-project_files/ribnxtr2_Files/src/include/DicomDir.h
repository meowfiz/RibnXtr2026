#ifndef DICOM_DIR
#define DICOM_DIR

#include	"DicomDataSet.h"
#include	"OtherCommonFunctions.h"
#include	"LinkedList.h"

#include	<qtreewidget.h>
//#include	<qlistview.h>
#include	<qdir.h>
//#include	<qfileinfolist.h>

	
class DicomDir
{
	public:
		DicomDir();
		~DicomDir();
		
		int *dicomItemsTab;
		int nrOfDicomItems;
		int nrOfDicomFiles;

		bool ingnoreWindowLevel;
		int			shiftValue;
		bool		invertPixelRepresentation;
		bool		shiftValuesToZero;
		int		CurrentDicomDataSet;
		DicomDataSet **DicomInfoTab;
		//LinkedList<DicomDataSet**>	*DICOMDirDataList;

		int	CreateNewDICOMDirListItem(int Start, int Stop,QTreeWidget* ListView);
		int	CreateNewDICOMDirListItem(int *PosTab, int Nr, QTreeWidget *ListView, bool ImportAll);

		void	QSortImageNumber(DicomDataSet **A,int Lo,int Hi);
		void	QSortPos(DicomDataSet **A,int Lo,int Hi,int nr);
		void	QSortOrient(DicomDataSet **A,int Lo,int Hi,int nr);
		void	QSortTmpSeries(DicomDataSet **A,int Lo,int Hi);
	//	void	QSortPosY(DicomDataSet **A,int Lo,int Hi);
	//	void	QSortPosZ(DicomDataSet **A,int Lo,int Hi);

		void		setInvertPixelRepresentation(bool inv){invertPixelRepresentation=inv;};
		void		setShiftValuesToZero(bool shift,int _shiftValue){shiftValuesToZero=shift;shiftValue=_shiftValue;};

		void		setIgnoreWindowLevel(bool val) { ingnoreWindowLevel = val; };

		
		int	AnalyzeDicomDir(const QString &filename, QTreeWidget* ListView, bool ImportAll, bool isDir = false, double tol = 0);


		QString GetSubdirPathWithLargestVolume(QString path);
		void UpdateSubdirPathList(QStringList * list, const QString path);
		bool CheckDicomPreamble(QString path);
		/*

		void	SlotImportDICOMDataSet(int);
		void	SlotCompareTwoDicomFiles();

	*/
};

#endif
