
#ifndef	POLYGON_SURFACE_DATA
#define	POLYGON_SURFACE_DATA


#include	"VtkWidget.h"
#include	"OtherCommonFunctions.h"
#include	"PropertyWidget.h"
#include	"LinkedList.h"


#include	<vtkPolyData.h>
#include	<vtkActor.h>
#include	<vtkPolyDataMapper.h>
#include	<vtkProperty.h>
#include	<vtkRenderer.h>
#include	<vtkPolyDataReader.h>
#include	<vtkPolyDataWriter.h>
#include	<vtkSTLReader.h>
#include	<vtkSTLWriter.h>
#include	<vtkPoints.h>
#include	<vtkMatrix4x4.h>
#include	<vtkFloatArray.h>
#include	<vtkCellArray.h>
#include	<vtkCellData.h>
//#include	<vtkVRMLImporter.h>
#include	<vtkAppendPolyData.h>
#include	<vtkActorCollection.h>
#include	<vtkPointData.h>
#include	<vtkLookupTable.h>

#include	<vtkOBJReader.h>

#include	<vtkTimeStamp.h>

#include	<qstring.h>
#include	<qmessagebox.h>
#include	<qwidget.h>
#include	<qtreewidget.h>
#include	<qtimer.h>
class VtkWidget;
class PropertyWidget;



class PolygonSurfaceData  : public QObject
{
	Q_OBJECT

	private:
	
	QTimer timer;
	bool testFlag[10];
	bool	isSegCutted;
	bool	isSegTemp;
	int		roleIndex;
	bool	tmpData;
	bool	repaintWithoutReset;
	bool isTempPropertyOn;
	vtkSmartPointer <vtkProperty> tempProperty,selectProperty;		
	VtkWidget			*VWidget;

	vtkPolyDataMapper	*VMapper;
	vtkActor			*VActor;
	vtkPolyData			*VPolyData;
	vtkMatrix4x4		*Matrix;
	
	vtkPolyDataMapper	*CuttingEdgeMapper;
	vtkActor			*CuttingEdgeActor;
	vtkPolyData			*CuttingEdgePolyData;
	
	QString progDirPath;

	vtkProperty			*VProperty;

	double	startPos[3];
	double  startOrient[3];

	double Origin[3];
	double MinBound[3],MaxBound[3];
	
	vtkRenderer		*VRenderer;
	bool			Visible;

	bool			HideWithoutRefresh;

	int				PN[3];
	double			PScale[3];
	int				NumberOfCuttingPlanes;
	double			*CuttingPlanesPoints;
	QString			PolygonSurfaceName;
	QString			PolygonSurfacePath;
	QString			textureName;
	bool			textureRepeat;
	bool colorLoaded;
	double randVals[10];
	QString defaultPropertyFilename;

	void SetupMapper();

	protected:

	//bool SurfaceActive;



		 QTreeWidgetItem *treeItem;
		 QString groupName;
		 QString tmpGroupName;
		 
		
		 unsigned long savedTimeStamp;
		 unsigned long mapperTimeStamp;;
		 
	public:



	PolygonSurfaceData();
	PolygonSurfaceData(vtkPolyData *NewPolyData,VtkWidget *VWidget);
	PolygonSurfaceData(VtkWidget *VWidget);
	~PolygonSurfaceData();

	void	MapperModified();
	bool	CheckIfModified();

	void	UpdateTexture(QString name,bool repeat=true);
	void	SetDefaultPropertyFileName(QString n){defaultPropertyFilename=n;};
	QString GetDefaultPropertyFileName(){return defaultPropertyFilename;};

	bool	SavePolygonSurfaceDataSet2( const QString &filename);
	bool	LoadPolygonSurfaceDataSet2( const QString &filename);
	bool	SavePolygonSurfaceDataSet( const QString &filename);
	bool	SaveSTLSurfaceDataSet( const QString &filename);
	bool	InsertSurfaceDataSet( vtkPolyData *Input, const QString &name);
	bool	LoadPolygonSurfaceDataSet( const QString &filename);
	bool	LoadSTLSurfaceDataSet( const QString &filename);
	bool	LoadOBJSurfaceDataSet(const QString &filename);
	bool	LoadNASTRANSurfaceDataSet( const QString &filename);
	bool	LoadVTKSurfaceDataSet( const QString &filename);

	bool	LoadVRMLSurfaceDataSet( const QString &filename);
	void	SetPolygonSurfaceDataSetName(const QString &name);

	void	ReplaceCuttingEdgePolyData(vtkPolyData *CEPolyData);

	void	SetRepaintWithoutReset(bool b){		repaintWithoutReset=b;};

	void	ShowActor();
	void	HideActor();
	void	HideActorWithoutRefresh();
	void	RebuildSurface(int Range=0);

	void	ComputeGeometricOrigin();
	void	SetCenterOrigin();
	void	SetCustomOrigin(double *CustomOrigin);

	void	SetDefaultProperty();

	void	SetParentData(PolygonSurfaceData *pData);
	void	AddNewChildData(PolygonSurfaceData *pData);

	
//narazie!

	bool	CheckForOutlineTriangle(vtkIdType *trianglePoints,int *TetraPoints,int NumberOfTetra);	


//	void	SetSurfaceActive(bool val);
//	bool	IsSurfaceActive();
	PolygonSurfaceData *parentPolygonSurfaceData;
	LinkedList <PolygonSurfaceData*> *childrenPolygonSurfaceDataList;

	vtkPolyData	*GetVPolyData()	{return VPolyData;};
	VtkWidget	*GetVWidget()	{return VWidget;};
	vtkActor	*GetVActor()	{return VActor;};
	vtkMatrix4x4 *GetMatrix()	{return Matrix;};
	vtkPolyDataMapper	*GetVMapper()	{return VMapper;};

	vtkPolyData	*GetCuttingEdgePolyData()	{return CuttingEdgePolyData;};
	vtkActor	*GetCuttingEdgeActor()	{return CuttingEdgeActor;};
	vtkProperty	*GetVProperty()	{return VProperty;};
	int			*GetPN(){return PN;};
	double		*GetPScale(){return PScale;};
	double		*GetOrigin(){return Origin;};
	double		*GetMaxBound(){return MaxBound;};
	double		*GetMinBound(){return MinBound;};

	void		SetVMapper(vtkPolyDataMapper *vMapper);

	void		SetRandVal(int nr,double val){randVals[nr]=val;};
	double		GetRandVal(int nr){return randVals[nr];};

	void		SetStartPos(double *pos){startPos[0]=pos[0];startPos[1]=pos[1];startPos[2]=pos[2];};
	void		SetStartOrient (double *orient){startOrient[0]=orient[0];startOrient[1]=orient[1];startOrient[2]=orient[2];};


	double		*GetStartPos(){return startPos;};
	double		*GetStartOrient(){return startOrient;}

	void		GetStartPos(double *pos){pos[0]=startPos[0];pos[1]=startPos[1];pos[2]=startPos[2];};
	void		GetStartOrient (double *orient){orient[0]=startOrient[0];orient[1]=startOrient[1];orient[2]=startOrient[2];};


	void		SetPScale(double *scale){PScale[0]=scale[0];PScale[1]=scale[1];PScale[2]=scale[2];};
	void		SetPN(int *n){PN[0]=n[0];PN[1]=n[1];PN[2]=n[2];};

	void		SetPolygonSurfaceName(const QString name){PolygonSurfaceName=name;};
	void		SetPolygonSurfacePath(const QString name){PolygonSurfacePath=name;};
	QString		GetPolygonSurfaceName(){return PolygonSurfaceName;};
	QString		GetPolygonSurfacePath(){return PolygonSurfacePath;};
	bool		GetHideWithoutRefresh(){return HideWithoutRefresh;};


	void	SetCuttingEdgePolyData(vtkPolyData *data){CuttingEdgePolyData=data;};
	void	SetHideWithoutRefresh(bool h){HideWithoutRefresh=h;};
	void	SetVPolyData(vtkPolyData *data) {		VPolyData = data;	};// { vtkPolyData *tmp = VPolyData;  VPolyData = data; if (tmp != NULL)  tmp->Delete();	};//?? test

	
	void	SetTempProperty (vtkProperty *p){tempProperty->DeepCopy(p);};
	vtkProperty *GetTempProperty(){ return tempProperty;};

	void	SetTempPropAsCurrent(bool);

	
	void	SetRoleIndex(int index){roleIndex=index;};
	int		GetRoleIndex(){return roleIndex;};


	bool	isTmpData(){return tmpData;};
	void	setTmpData(bool v){tmpData=v;};


	bool	GetIsSegCutted(){return isSegCutted;};
	void	SetIsSegCutted(bool v){isSegCutted=v;};
	
	bool	GetIsSegTemp(){return isSegTemp;};
	void	SetIsSegTemp(bool v){isSegTemp=v;};


	bool	GetTestFlag(int nr){return testFlag[nr];};
	void	SetTestFlag(int nr,bool value){testFlag[nr]=value;};
//	vtkPolyData	*GetPolyData()	{return VPolyData;};
//	vtkPolyData	*GetPolyData()	{return VPolyData;};
//	vtkPolyData	*GetPolyData()	{return VPolyData;};
			void SetTreeItem( QTreeWidgetItem *item){treeItem=item;};
		QTreeWidgetItem *GetTreeItem(){return treeItem;};

		void SetGroupName(QString name){groupName=name;};
		QString GetGroupName(){return groupName;};

		
		
		void SetTmpGroupName(QString name) { tmpGroupName = name; };
		QString GetTmpGroupName() { return tmpGroupName; };


		void SetTextureName(QString name){textureName=name;};
	
		QString GetTextureName(){return textureName;};
		
		void SetTextureRepeat(bool r){textureRepeat=r;};
		bool GetTextureRepeat(){return textureRepeat;};
		
		public slots:


		void	SlotSetTempPropAsCurrentOn();
		void	SlotSetTempPropAsCurrentOff();


};


#endif //POLYGON_SURFACE_DATA

