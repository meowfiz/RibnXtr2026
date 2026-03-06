#ifndef		MAIN_WINDOW
#define		MAIN_WINDOW
 
#include <Kinect.h>

#include "itkProcessObject.h"
#include "itkGroupSpatialObject.h"
#include "itkTubeSpatialObject.h"

// TubeTK Includes
#include "tubeWrappingMacros.h"
#include <itkGroupSpatialObject.h>
#include <itkSpatialObjectReader.h>
#include <itkDefaultStaticMeshTraits.h>
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkMetaImageIO.h>
#include "itkRGBPixel.h"
#include "itkRescaleIntensityImageFilter.h"
#include <itkImageToImageFilter.h>
#include <itkInPlaceImageFilter.h>

#include <vtkDoubleArray.h>
#include <vtkPolyLine.h>
#include <itkImageRegionIterator.h>
#include <vtkMetaImageReader.h>
#include "MyQtObjects.h"
//#include <QRandomGenerator>
//#include <vtkAutoInit.h>
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"
#include "xlsxworksheet.h"

//VTK_MODULE_INIT(vtkRenderingOpenGL2);
//#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)
//#include	<q3toolbar.h>
#include <armadillo>

#include <qstylefactory.h>
#include <qshortcut.h>
#include <qmimedata.h>
#include <qclipboard.h>

//#include	"Kinalyzer/kinlistener.h"
#include	"SymmetryPlanesFinder.h"
//#include	"RawDataMixer.h"
#include	"RawDataMixerExtended.h"
#include	"MyDockWidget.h"
#include	"Filters3D.h"

#include	"SurfaceRegistration.h"
#include	"SurfaceElasticRegistration.h"
#include	"GUIPanelWidget.h"
#include	"VolumeElasticRegistration.h"
#include	"SurfaceColorize.h"
#include	"ImageSetRegistration.h"
#include	"ImageSetElasticRegistration.h"

#include	"HistWidget.h"
#include	"VtkWidget.h"
#include	"RawDataSet.h"
#include	"MainWindow.h"
#include	"HybridMedianFilter.h"
#include	"PropertyWidget.h"
#include	"SurfaceData.h"
#include	"View2DSliceWidget.h"
#include	"Segmentation.h"
#include	"CylinderBoxActor.h"

#include	"SegCriterion.h"

#include	"Registration.h"
#include	"SurfaceProcessing.h"
#include	"DicomDataSet.h"
#include	"DicomDir.h"


#include	"LinkedList.h"
#include	"DataSet.h"
#include	"SurfaceCollection.h"
//#include	"Filtering.h"
#include	"OtherCommonFunctions.h"

#include    "OtherImageFunctions.h"
#include	"DistanceMap.h"

#include	"ImageWidget.h"
#include	"FeaturePointRegistration.h"

#include	"Animations.h"
#include	"TextureSegmentation.h"
#include	"ScatterPlotProcessing.h"

#include	"HeartSimulator.h"
#include	"vtkTextureObject.h"
//#include	"L3DReader.h"

#include	"ExplodeSegmentation.h"
#include	"ArteryData.h"

//#include	"StereoViewD3DWidget.h"
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>

#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#include	<QByteArray>
#include	<qmainwindow.h>
#include	<qinputdialog.h>
#include	<qapplication.h>
#include	<qmenubar.h>
#include	<qnamespace.h>
#include	<qwidget.h>
#include	<qimage.h>
#include	<qpainter.h>
#include	<qslider.h>
#include	<qlabel.h>
#include	<qlayout.h>
#include	<qsplitter.h>
//#include	<QMenu.h>
//#include	<q3filedialog.h>
#include	<qcolordialog.h>
#include	<qcolor.h>
#include	<qdir.h>
#include	<qtoolbox.h>
//#include	<q3dragobject.h>
//#include	<q3url.h> 
#include	<qstringlist.h>
//#include	<q3accel.h>
#include	<qstatusbar.h>
#include	<qprogressbar.h>
#include	<qprocess.h>

//Added by qt3to4:
#include	<QTextEdit>
//#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QKeyEvent>
//#include <QVBoxLayout>
#include <QDropEvent>

#include	<QPolygon>

#include	<vtkHausdorffDistancePointSetFilter.h>
#include	<vtkBMPWriter.h>
#include	<vtkImageShiftScale.h>
#include	<vtkArrowSource.h>
#include	<vtkTrivialProducer.h>
#include	<vtkMarchingContourFilter.h>
#include	<vtkNIFTIImageReader.h>
#include	<vtkRTAnalyticSource.h>
#include	<vtkImageCast.h>
#include	<vtkSmartVolumeMapper.h>
#include	<vtkClipVolume.h>
#include	<vtkPlaneCollection.h>
#include	<vtkPlanes.h>
#include	<vtkPropPicker.h>
#include	<vtkPropCollection.h>
#include	<vtkTriangleFilter.h>
#include	<vtkPolyDataWriter.h>
#include	<vtkPolyData.h>
#include	<vtkDataSetWriter.h>
#include	<vtkSphereSource.h>
#include	<vtkActor.h>
#include	<vtkPolyDataMapper.h>
#include	<vtkCylinderSource.h>
#include	<vtkConeSource.h>
#include	<vtkVolumeProperty.h>
//#include	<vtkVolumeRayCastCompositeFunction.h>
//#include	<vtkVolumeRayCastMapper.h>
#include	<vtkVolume.h>
//#include	<vtkVolumeProMapper.h>
#include	<vtkTexture.h>
#include	<vtkPlaneSource.h> 
#include	<vtkTextureMapToPlane.h> 
#include	<vtkDataSetMapper.h>
#include	<vtkExtractVOI.h>
#include	<vtkLookupTable.h>
#include	<vtkPolyDataReader.h>
#include	<vtkStructuredPoints.h>
#include	<vtkUnsignedShortArray.h>
#include	<vtkMatrix4x4.h>
#include	<vtkTubeFilter.h>
#include	<vtkLineSource.h>
#include	<vtkAppendPolyData.h>
#include	<vtkTransformPolyDataFilter.h>
#include	<vtkTransform.h>
#include	<vtkPiecewiseFunction.h>
#include	<vtkColorTransferFunction.h>
#include	<vtkPointData.h>
#include	<vtkPointPicker.h>
#include	<vtkVolumePicker.h>
#include "vtkSmartPointer.h"
#include	<vtkLookupTable.h>
#include	<vtkPoints.h>
#include	<vtkCellArray.h>
#include	<vtkCamera.h>
#include	<vtkLightCollection.h>
#include	<vtkPropCollection.h>
//#include	<vtkOpenGLCoincidentTopologyResolutionPainter.h>

//#include	<vtkVolumeTextureMapper.h>
//#include	<vtkVolumeTextureMapper2D.h> 
//#include	<vtkVolumeTextureMapper3D.h> 
#include	<vtkSplineFilter.h>
#include	<vtkKochanekSpline.h>
#include	<vtkAnnotatedCubeActor.h>
#include	<vtkOrientationMarkerWidget.h>
#include	<vtkPropAssembly.h>
//#include	<vtkPNGWriter.h>
#include	<vtkImageExport.h>
#include	<vtkImageAlgorithm.h>
#include	<vtkWindowToImageFilter.h> 
#include	<vtkJPEGReader.h>
#include	<vtkBMPReader.h>
#include	<vtkPNGReader.h>
#include	<vtkViewport.h> 
#include	<vtkWorldPointPicker.h>
#include	<vtkTransformCoordinateSystems.h>

#include	<vtkGlyph3D.h>
#include	<vtkUnstructuredGrid.h> 
#include	<vtkCollectionIterator.h>
//#include	<vtkPlaneWidget.h>
//#include	<vtkImplicitPlaneRepresentation.h>
#include	<vtkBox.h>
#include	<vtkMatrix4x4.h>
#include	<zlib.h>
#include	<qmdiarea.h>
#include	<qmdisubwindow.h>

#include	<qmdiarea.h>
#include	<qmdisubwindow.h>
#include	<qfileinfo.h>

#include	"FFMPEGTools.h"

#include <omp.h>

#include <vtkGPUInfoList.h>
#include <vtkGPUInfo.h>
//#include "ExternalFiles\QKinect.h"

//#include "opencv2/opencv.hpp"

#include <qcamerainfo.h>
#include <qcamera.h>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QVideoEncoderSettings>
#include <qmutex.h>
#include <QMetaType>
//class StereoViewWidget;

class	Surface;
struct	Palette;




#define DEPTH_BUFFERS_NR 10
#define NUMBER_OF_PALETTES 200

QTXLSX_USE_NAMESPACE


//using namespace cv;
class DepthBuffer
{
public:

	DepthBuffer();
	~DepthBuffer();
	unsigned char *buffer;
	bool locked;
	QMutex mutex;
	unsigned int timeID;
	int min;

};

//---------------------------
class MainWindow;
class KinectDrawThread : public QThread
{
public:

	KinectDrawThread(MainWindow *mw);

protected:

	MainWindow *mWindow;
	void run();
	void	SlotDrawKinectDepthMap();


};
//--------------------------------------

//---------------------------

class KinectDraw3DThread : public QThread
{
public:

	KinectDraw3DThread(MainWindow *mw);

protected:

	MainWindow *mWindow;
	void run();
	//void	SlotDrawKinectDepthMap();
	void	SlotRenderKinectDepthMap();


};

class CameraDrawThread : public QThread
{
public:

	CameraDrawThread(MainWindow *mw);

protected:

	MainWindow *mWindow;
	void run();
	void	SlotDrawCameraImage();

};
//--------------------------------------
//----------------------------------------------------
class MainWindow : public QMainWindow
{

	Q_OBJECT

public:
	

	MainWindow(int modex = 0, QWidget *parent = 0, const char *name = 0, Qt::WindowFlags  f = 0);

	//(unsigned char *_Pal,int _PalNr,QMenuBar *Menu,
	~MainWindow();
	
	vtkRenderer *renderer;
	vtkRenderWindow *renWin;
	bool disable3DUpdateFilter;
	QElapsedTimer elapsedTimer;

	VtkWidget *VWidget;
	QWidget* VWidgetParent;

	unsigned char  tmpValx;

	QProcess *process;

	//int   nrOfColors;
	ParentAndAvoidData parentAndAvoid[NR_OF_MASK_COLORS];
	unsigned char frontEndTabIn[NR_OF_MASK_COLORS], frontEndTabOut[NR_OF_MASK_COLORS], frontEndTabOutBeforeRemap[NR_OF_MASK_COLORS];
	int frontSizeRepetitionFailureTab[NR_OF_MASK_COLORS];
	int frontSizeCountTab[NR_OF_MASK_COLORS];
	int frontLengthTab[NR_OF_MASK_COLORS];
	int frontSizeTotalCountTab[NR_OF_MASK_COLORS];

	double bacground3DCol[3];
	double front3DCol[3];
	double randColors[768];

	int DefalutVolumeLayout;

	int lastVolumeMaskCount;


	arma::mat H;
	QTimer *resetCameraTimer;
	int resetCameraTime;
	int zoomCameraValue;
	bool kinCalibrated;
	int nrOfCurrentFastFiltersParams;
	unsigned short *homographyLookup;
	int homographyLookupSize;

	bool cameraRequest;

	QCamera *camera;
	QCameraImageCapture *imageCapture;
	int cameraFrameId;

	int *Palette;
	PaletteRGBA *Pal;
	PaletteRGBA *PalR;
	unsigned int kinectRender3DID;
	int kinectCorners[8];
	int kinectCrop[4];
	int customBox[4];
	int customBox2[4];
	bool singleScreenStereo;
	bool superMaskUpdate;
	int nrOfDepthBuffersFilled;
	void countFps();
	long long fpsCount, fpsSum;
	QTime current_time;
	QTime last_time;
	int elapsed_time;
	double fps; // current fps in the loop, depending on the "work time" required during the paintGL call

	unsigned char *ucharBuffer, *ucharBuffer2;
	unsigned char *ucharBuffer3;
	//unsigned char **depthBuffers;
	DepthBuffer **depthBuffers;
	bool sthTest;
	ImageWidget *kinectImageWidget;
	ImageWidget *statisticsWidget;
	QMutex m_kinectPreviewMutex;  // chroni dostęp do obrazu podglądu Kinecta (wątek roboczy vs paintEvent)
	qint64 m_lastKinectPreviewUpdate = 0;  // throttle odświeżania podglądu (~30 fps), żeby zmniejszyć „skakanie”
	bool kinInit = false;
	bool kinectFiltering = false;


	unsigned short *xPosDepthBigTab;
	unsigned short *yPosDepthBigTab;
	unsigned char *zPosDepthBigTab;
	unsigned char *maskData1;
	unsigned char *tmpMask;
	int depthBigTabSize;
	bool kinectStop;

	//std::vector<uint8_t> kinectDepthBuffer;
	//std::vector<uint16_t> kinectDepthBuffer16;
	//std::vector<uint8_t> rgb_buffer;
	//std::vector<uint8_t> rgb_buffer_back;
	double sBlock[8];
	bool	initKinectDone;
//	QKinect * m_kinect;
	bool kinectEnabled;
	int surfaceActivatedFromMenuIndex;
	QString configIniName;
	QString progDirPath;
	QString ffplayexePath;
	QString dopplerSegmentatorPath;
	QString dopplerSegmentatorModelPath;
	QString jadraSegmentatorPythonPath;
	QString jadraSegmentatorScriptPath;
	QString jadraSegmentatorConfigPath;
	QString jadraSegmentatorCheckpointPath;
	GUIPanelWidget	* GUIPanel;

	//private:
	bool kinect2Init;
	IKinectSensor* m_sensor = nullptr;
	IDepthFrameReader* m_depthFrameReader = nullptr;
	IColorFrameReader* m_colorFrameReader = nullptr;
	ICoordinateMapper* m_coordinateMapper = nullptr;

	uint32* m_colorBuffer = nullptr;
	uint32* m_colorBuffer_back = nullptr;
	ColorSpacePoint* m_colorSpacePoints = nullptr;
	uint16* m_depthBuffer = nullptr;
	int m_depthWidth = 0, m_depthHeight = 0;

	QList<QPushButton2*> buttonList;
	QList<QCheckBox2*> checkBoxList;
	QList<QComboBox2*> comboBoxList;
	QList<QLineEdit2*> lineEditList;
	QList<QSlider2*> sliderList;
	QList<QSpinBox2*> spinBoxList;

	int medMode;//0 - tubes , 1-pelvis , 2- to be edited
	vtkLookupTable* maskLut;
	
	
	KinectDrawThread *kinectThread = nullptr;
	KinectDraw3DThread *kinect3DThread = nullptr;
	CameraDrawThread *cameraThread = nullptr;

	int segMode;
	int prevTimeVal;
	QTime time;
	QString keyboardBuffer;
	//	vtkSmartPointer <vtkHomogeneousTransform> kinnectCameraTransform;
	int polyLineMode;
	QImage *arrowImage;
	double trans[20];
	RawDataMixerExtended *mixer;
	RawDataMixerLite*mixer1;
	RawDataMixerLite *mixer2;

	FFMPEGTools *videoTools;
	bool specialInit;
	double specialValStart;
	int specialFrameCount;

	double camPos[3];
	double camViewUp[3];
	double camFocalP[3];
	int lastKinnectMode;
//#ifdef USE_KINLISTENER1
//	KinListener *kinList;
//#endif
	int nrOfSkyboxes;
	int primaryScreenNr;
//	StereoViewWidget *stereoWidget;
	QImage *img3D;
	//StereoViewD3DWidget *stereoD3DWidget;
	int		lastCameraView;
	bool	cameraUsesBase;
	//	QString keyboardBuffer;
	bool	grabSixSides;
	bool	showMessageProcessEvents;
	QString MessageText;
	QProgressBar *progBar;
	int prevProgreessValue;
	QStatusBar	*statBar;

	MyViewWidget *viewWidget;

	QStringList filters3DImplemented;

	QString kinFilterMask;
	QString kinFilter;


	QString snapshotPath;
	QTime lastSnapshotTime1;
	QTime lastSnapshotTime2;
	QString filterMask;
	QString regFilterMask;
	QString filterMaskMatching;

	bool hideForArteriesOnly;
	bool hideForPelvisOnly;
	bool hideGUI;

	bool hideForMatching;
	bool hideForMatchingSimple;

	int nrOfSingleVeins, nrOfSymetricVeins;
	double seedPoint[3];
	ArteryData *arteryData;
	QMenu *representationMenu, *interpolationMenu, *opacityMenu, *lightingMenu;
	QMenu* file;
	QMenu* savefile;
	int droppedFileCount;
	double veinParams[50];
	bool firstCall;
	int nrOfPossibleVeins;
	int pseudoSurfaceSelection;
	//double veinRGB[3];
	//double jointRGB[3];
	int currentVeinRole;
	QString veinStringTab[100];

	QString selectedPointsNames[100];
	int selectedPointsNr;
	QString triangulatedPointsNames[100];
	int triangulatedPointsDefs[300];
	int triangulatedPointsNr;
	int pointPairsDefs[200];
	int pointPairsNr;
	int pointTriDefs[300];
	int pointTriNr;

	int planeFinderNr;
	int planeFinderDefs[20][6];//groupleft group right OBJ_CENTER_Front OBJ_CENTER_BACK POINT_CENTER_front POINT_CEnterBAck
	//int planeFinderLastPoint[20];
	//bool planeFinderLastPointExists[20];
	QString planeFinderNames[20];
	//int planeFinderPoints[20][4];
	QString planeFinderPointsNames[100];

	int basePlaneNr;
	int basePlaneDefs[20][5];
	bool basePlaneExists[20];
	double basePlanePoints[20][12];//x,y,z,origin
	QString basePlaneNames[20];


	QString centeredPointsNames[100];
	int centeredPointsDefs[100];
	int centeredPointsNr;

	int nrOfPlanes;
	QString planeNames[20];
	int planeDefs[20][3];
	int planeTestPoints[20][20];

	int veinPairs[200];
	int veinPairsNr;
	double	veinCameraOrientation[600];
	unsigned char	veinColor[300];
	int veinNr;
	bool lKeyPressed;
	bool 	undoRedoSucceed;

	double twist;

	RibonTimer RTimer;
	bool restartAppMode;
	int	LowerMemThreshold;
	int	UpperMemThreshold;


	FloatGrid3D *Grid;


	QString ProgramDirName;
	QString ProgramDefaultDirPath;
	QString FileDefaultDir;
	QStringList 	lastFileName;
	QString	GrabWindowPath;


	DistanceMap *MapOfDistances;
	ImageWidget *ScatterImageWidget;

	//*************************************
	QString GrabWindowName;

	//int		CurrentDicomDataSet;
	bool	Reparent;
	bool	LockLayout;
	bool	OpacityMode;
	int		PressedButton;
	bool	ViewsConnected;

	bool	ActorInteraction;
	bool	CuttingToolInteraction;
	bool	CubeActorInteraction;

	//Filtering *Filters;
	//*************************************
	//	DicomDataSet *DicomInfoTab;


	

	DicomDir *DicomDirData;

	LinkedList<PolygonSurfaceData*>	*LineSurfaceDataList;
	LinkedList<PolygonSurfaceData*>	*FrustumSurfaceDataList;

	LinkedList<PolygonSurfaceData*>	*SurfaceDataList;
	ListItem<PolygonSurfaceData*>	*ModelSurfaceDataSetListItem;
	ListItem<PolygonSurfaceData*>	*ObjectSurfaceDataSetListItem;
	ListItem<PolygonSurfaceData*>	*CurrentSurfaceDataSetListItem;


	//	LinkedList<DicomDataSet**>	*DICOMDirDataList;



	//LinkedList<RawDataSet*> *Lista;
	LinkedList<DataSet*>	*DataSetList;
	ListItem<DataSet*>		*MainListItem;
	ListItem<DataSet*>		*ObjectListItem;
	ListItem<DataSet*>		*View3DListItem;


	int MainDataPos;
	int	InspectionDataPos;
	int ObjectDataPos;
	int View3DDataPos;
	int PosX, PosY, PosZ;

	////////////////////////

	int ModelSurfaceDataPos;
	int ObjectSurfaceDataPos;
	int CurrentSurfaceDataPos;
	int CuttingSurfaceDataPos;




	int LastClickedView;

	int Interpolation;
	int CrossType;
	vtkLight *Light;
	bool NoDataLoaded;
	bool NoRegisteredDataLoaded;


	int *listForGrowing;
	
	int listForGrowingSize;

	unsigned char   *MaskVolume;
	unsigned char	*MaskData;
	unsigned char	*Mask2Data;
	RawDataSet *RegisteredData;
	SurfaceData *SData;
	//	Segmentation *Segment;

	CylinderBoxActor *BoxActor;
	vtkProperty *BoxProp;
	vtkSmartPointer <vtkProperty> selectProp;
	vtkSmartPointer <vtkProperty> selectPropTemp;
	//	PropertyWidget *BoxPropWidget;

	PropertyWidget *PropWidget;

	int MaxNumberOfSurfaces;

	int MaxNumberOfPlanes;
	vtkActor **planeActor; // to tez trzeba poprawic
	vtkActor **ObjectPlaneActor;

	vtkLookupTable *theLUT;

	int SelectedPoint[3]; //TO BYL FLOAT?! 2003.08.24
	double selectedDoublePoint[3];
	double selectedPrevDoublePoint[3];
	bool selectedPointExist[100];
	bool triangulatedPointExist[100];
	bool centeredPointExist[100];
	bool planeFinderPointsExist[80];


	double selectedPoints[300];
	double triangulatedPoints[300];
	double centeredPoints[300];
	double finderPoints[240];

	QString SurfacePath, DataPath;
	QString latestMaskDataPath;
	int ActualLayout;

	QWidget *MainWidget;

	QMenuBar *MainMenuBar;
	QMenu *OpacityMenu;
	QMenu *propertiesMenu;
	QMenu *OtherMenu;
	QMenu *VolumeMenu;
	QMenu *LayoutMenu;
	QMenu *WidgetMenu;
	QMenu *CameraMenu;
	QAction *cameraActions[15];
	//	QMenu *propMenu,*representationMenu;

	QMenu *SurfaceMenu;

	QMenu *Pointer2DMenu;
	QMenu *InterpolationMenu;

	QMenu *ModeMenu;


	bool InteractorMode;
	char	Endian;
	//	int		sx,sy,sz,sxy;



	unsigned char *PalTab;
	QList<QString> PalNames;
	int PalNr;
	int PalNrR;
	bool PalInvert;
	bool PalInvertR;
	QString	filename;
	QColor BckColor;
	HistWidget *Histogram;

	bool StretchPalette;
	bool StretchPaletteR;
	bool AxesExists;
	bool Planes3D;
	int NumberOfPlanes;
	int NumberOfPlanesO;

	bool ObjectPlanes3D;


	double *aL, *ra;
	//-----------------------------------

	/*QSplitter *HorizSplitter;

	QWidget *AllViewsWidget;

	QSplitter *MainHorizontalSplitter;
	QSplitter *MainVerticalSplitter;
	QSplitter *VerticalSplitter1,*VerticalSplitter2;
	QSplitter *HorizontalSplitter1,*HorizontalSplitter2;
	QSplitter *HistogramVerticalSplitter;

	QVBoxLayout *AllViewsLayout;*/

	//------------------------------------


	vtkActor	*AxeXActor, *AxeYActor, *AxeZActor, *ConeXActor, *ConeYActor, *ConeZActor;


	View *TransversalView;
	View *SagittalView;
	View *CoronalView;

	View* Views2D[3];


	Surface	*Sur;

	RawDataSet*	data;
	RawDataSet*	data1;
	//	HybridMedianFilter  *HMedian;


	//------Pointer-----
	QLabel *SizeLabel;
	QSlider *SizeSlider;
	QWidget *ResizeWidget;

	//------- Volume!! -------------
	bool isVolumeVisible;
	bool isVolumePointerVisible;
	bool isCylinderBoxActorVisible;
	bool is3DAxesVisible;
	bool is3DPlanesVisible;
	bool is3DParallelPlanesVisible;
	int nrOfThreads;
	vtkStructuredPoints			*VolumeStructuredPoints;
	vtkStructuredPoints			*MaskVolumeStructuredPoints;
	//vtkSmartVolumeMapper				*VolumeMapper;
	vtkGPUVolumeRayCastMapper	*VolumeMapper;
	vtkVolume					*Volume;
	//vtkPiecewiseFunction		*OpacityTransferFunctionScaled;
	vtkPiecewiseFunction		*OpacityTransferFunction;
	vtkSmartPointer <vtkPiecewiseFunction>	OpacityTransferFunction2;
//	vtkPiecewiseFunction		*OpacityTransferFunctionVis;
	vtkPiecewiseFunction		*GradientTransferFunction;

	vtkPiecewiseFunction		*GradientTransferFunction2;
	vtkVolumeProperty			*VolumeProperty;
	vtkColorTransferFunction	*ColorTransferFunction;
	vtkSmartPointer <vtkColorTransferFunction>		ColorTransferFunction2;
	//	bool						VolumeExists;
	//bool						VolumeProExists;
	bool						DataExists, Data1Exists;
	bool						VolumePointerExists;
	bool						AnySurfaceExists;
	vtkProperty					*VolumePointerProp;
	//--------------------

	QWidget *PlaneNumberWidget;
	QLabel	*PlaneNumberLabel;
	QSlider *PlaneNumberSlider;

	//explode
	mySphereDistTabsElement *mySphereDistanceTab;
	PolygonSurfaceData *shpereExplodeShadowSurfaceData;
	PolygonSurfaceData *liveTmpSurfaceData;
	vtkPolyData *shpereExplodeSource;
	int sphereExplodeSize;
	bool explode3DFinal;
	unsigned short *volumeTempData;
	unsigned short *volumeTempData2;
	unsigned char *totalMask;
	int chgMaskTimeStamp;
	int finishedGrowing3DTimeStamp;


	bool replayModeForGrowing3D;
//	int chgMask2TimeStamp;

	int chgMaskTimeStampOld;
//	int chgMask2TimeStampOld;

	//livegrowing3d
	vtkLookupTable *tmpLUT;
	unsigned char *tmpFlagMask;
	vtkStructuredPoints			*tmpMaskVolumeStructuredPoints;
	RawDataSet *tmpRawDataSet;
	vtkSmartPointer <vtkStructuredPoints>tmpVolumeStructuredPoints;
	vtkSmartPointer <vtkGPUVolumeRayCastMapper>tmpVolumeMapper;
//	vtkSmartPointer <vtkSmartVolumeMapper>tmpVolumeMapper;
	vtkSmartPointer <vtkVolume>tmpVolume;
	bool tmpMaskVolumeVisible;

	unsigned char volMaskUseHist[NR_OF_MASK_COLORS];

	
	int mixer0TimeStamp;
	int mixer1TimeStamp;
	int mixer2TimeStamp;

	//------Surfaces----------
	int ActualSurface;
	int NumberOfSurfaces;


	
	vtkPolyData **SurfacePolyData;
	vtkActor	**SurfaceActor;
	//-----------------------
	//bool depthSphereActorsVisible;
	//int destDepthActorsX, destDepthActorsY;
	//vtkActor	**depthSphereActors;
	vtkActor	*SphereActor;
	vtkActor	*XPointerActor, *YPointerActor, *ZPointerActor;

	int FirstCorner[3], SecondCorner[3];
	int Corner1[3], Corner2[3];

	bool Corner;

	bool InspectionWindow;

	//-------------Masks--------
	int		ActualMask;
	int		NumberOfMasks;
	int		**Masks;
	int		MinMask, MaxMask;
	bool	AllMasksFilled;

	int		ActualMask2;
	int		NumberOfMasks2;
	int		**Masks2;
	int		MinMask2, MaxMask2;
	bool	AllMasks2Filled;

	///////////////////////////////////////
	//Cutting


	vtkActor *CuttingToolActor;

	SurfaceProcessing *SProcessing;

	///////////////////////////////////////

	unsigned int PictureIndex;



	vtkDataSetWriter *Writer;


	int SliceNumber;


	void	SaveSurface(QString &filename);

	void	UpdateVolume();
	void	CreateTmpMaskVolume();

	//	void	CreateNewDICOMDirListItem(int Start, int Stop);
	//	void	CreateNewDICOMDirListItem(int *PosTab, int Nr);

	//	void	QSortPosX(DicomDataSet **A,int Lo,int Hi);
	//	void	QSortPosY(DicomDataSet **A,int Lo,int Hi);
	//	void	QSortPosZ(DicomDataSet **A,int Lo,int Hi);


	bool CheckForOutlineTriangle(int *trianglePoints, int *TetraPoints, int NumberOfPoints);

	bool	TempGrabVariable;


	///////////////////////////////////////////////////////////	
	/*
	int SequenceNumberOfPoints;
	vtkIntArray *CameraSubSteps;

	// camera
	vtkPoints *CameraPoints;
	vtkPoints *CameraViewUpPoints;
	vtkPoints *CameraFocalPoints;

	// 3D surfaces
	vtkPoints		**ColorPoints;
	vtkFloatArray	**OpacityPoints;

	// 3D Background color
	vtkPoints		*Background3DColorPoints;

	// volume opacity and thresholds
	vtkIntArray *LeftVolumeThresholdPoints;
	vtkIntArray *RightVolumeThresholdPoints;
	vtkPoints *OpacityVolumePoints;

	// volume palette
	vtkIntArray *PaletteNrPoints;

	//ACTOR
	vtkPoints		**ActorOriginPoints;
	vtkPoints		**ActorOrientationPoints;
	vtkPoints		**ActorPositionPoints;

	bool GenerateSequenceAbort;
	bool GenerateSeuenceInProgress;
	*/
	///////////////////////////////////////////////////////////
	double ignoreTolerance;

	double sumDiameters;
	int nOfDiPts;
	double arteryArea;
	int arteryDensity;

	double hdelta(double *hdeltad, int i, int j, int k);
	RawDataSet *centerofball(double *hdeltad, RawDataSet *grayData, RawDataSet *distData, double rangeBack, double thresh);
	void Inithdelta(double *hdeltad, double hx, double hy, double hz);
	vtkPoints *testPoints;
	double meshVol;
	QMenu *skyBoxMenu;
	vtkPoints *ModelFeaturePoints, *ObjectFeaturePoints;
	Animations *Anim;

	QTimer *cameraTimer;
	QTimer *testTimer;
	QTimer *selectTimer;
	QTimer *cycleTimer;
	QTimer *grabTimer;

	QTimer  *kinectTimer;
	QTimer  *kinectTimer3D;

	QTimer *redrawTimer;

	bool grabAnim;
	int recordMode;

	bool stereoFlag;
	int testCycleCounter;
	int temptimer;
	int *cycleTabList;
	int cyclePos;
	int cycleTabSize;

	int *cycleSurfaceTabList;
	int cycleSurfacePos;
	int cycleSurfaceTabSize;
	double cycleSurfaceOpacity;

	double baseOrigin[3];
	double baseVectors[9];

	double base4Point[4][3];

	double basePoint[3];
	double basePointM1[3];
	double basePointP1[3];
	double middleL1[3], middleL2[3];
	double vL1[3], vL2[3];
	double p1L1[3], p2L1[3], p1L2[3], p2L2[3];

	void	dragEnterEvent(QDragEnterEvent* event);
	void	dropEvent(QDropEvent *evt);
	void	closeEvent(QCloseEvent *event);
		//------------------------------
	void readSettings();
	//bool	eventFilter( QObject *obj, QEvent *event ); 

	bool isMultiDropEnabled;

	bool isSHIFTpressed;
	bool isCTRLpressed;

	QVector <int> aneurysmFrontValues;
	QVector <QVector3D> seeds;
	QVector <QVector3D> ignorePoints;

	QString defaultVolumeOpacityFilename;

	bool	image2DPressed;
	 
	bool magnifyingGlassMode;

	QVector<QVector<double>> radiusLists[10];


	QString defaultPropertyFilename;
	double linePoint1[3];
	double linePoint2[3];
	bool blockUpdateDefaultDir;
	bool blockChangeTempFlag;

	bool kinnectMenuVisible;
	int kinnectMode;
	double painterParams[500];

	int prevExplodeSliderVal;

	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void wheelEvent(QWheelEvent* event);
	//------------------improvements
	//void	SetRandomSurfaceColor();
	void	ApplySurfaceProcessing(void (MainWindow::*funcs) (void), const QString &text);
	void	ApplyActorTransformationAndRebuild();
	void	ResetActorTransformation();

	void	CleanPolyData();
	void	ComputeNormals();
	void	CalculateMeshVolume();
	void	CutCurrentSurface();
	void	CreateBoundaryDataFromCurrentSurface();
	void	DepthSortCurrentSurface();
	void	DecimateCurrentSurface();
	void	SmoothAndDecimCurrentSurfcace();
	void	SmoothCurrentSurface();
	void	ClosestToPointerSurfaceOnly();
	void	ExtractSeparatedSurfaces();
	void	LargestSurfaceOnlySurface();
	void	DeletePolygonSurfaceDataSet();
	void	DeletePolygonTmpSurfaceDataSet();
	void	DeletePolygonSurfaceDataSet(PolygonSurfaceData *pData);

	void	GetSkeletalPoints();
	void	GetAverageArteryDiameter();

	void	GetArteryArea();


	void	LoadVeinDefs();
	void	UpdateVeinDefs();
	bool	SetDataSetToCurrent(DataSet *dataSet, bool switchToVolumeTab = false);
	void	ShowCurrentSurfaceListAndHideOthers(LinkedList <PolygonSurfaceData*> *sList);
	//-------------------------------------------
	void	 Grab3DWindow(QString filename = "");
	QString		GrabVeinSurfaces();
	int		GetPositionOfRoleSurface(int role, LinkedList<PolygonSurfaceData*> *sList);
	void	ShowAndResetFixedSizeOfDockWidgets(char *tab);
	void	ApplyKinnectMode(int nr);


	QTreeWidgetItem * tempTreeWidgetItem;
	bool tempTreeParam;
	bool arePointsVisible;;
	int InitialPalette;
	double selectedPointsRadius;
	QString medicalSettingsDirName;
	double selectedPointsOpacity;
	void	SetupGlobalAppSettings();

	unsigned char currentMaskOpacity;
	bool addBarrier;

	bool guiHidden;


	void	SetupMainWindowSettings();

	QWidget *getSelectedGrabWidget();

	void InitOpacityTransferFunctions();


	void	showHideVolume(bool);
	void	showHideTmpMaskVolume(bool);
	void	showHideVolumePointer(bool);


	void UpdateFilterCompositions(QComboBox *cBox, QString filter);
	void ApplyFilterComposition(QComboBox *comboBox);
	bool LoadSnapshot(QString xname);
	void	CustomBox2Update();




	void	UpdateLayerSettings(RawDataLayer *layer, int &pos);
	void	UpdateFastFiltersGUI();
	void	UpdateFastFiltersGUI(QString filter);
	void	UpdateSliderSettings(RawDataLayer *layer, int &pos);
	void	ConnectAllFastFilters();
	void	DisconnectAllFastFilters();

	void GetHomographyMap();
	void UpdateKinectVertexLabels();

	bool calibrateDepthMap;

	void LoadHounsfieldSettings();

	QPolygon hounsfieldSettings;

	bool TestFilterForName(QString name);
	void CombineBooleanMaskWithCurrent(unsigned char *mask);


	RawDataSet *lastData3D;
	bool noClearVolMaskPts;

	//int  AddMaskToVolumeStructuredPoints(unsigned char *mask, int col, bool avoidZero=true, bool update=true,bool avoidExisting=false);
	void ClearVolumeStructuredPoints(int val, bool update3D=true, bool updateM2=true);
	//int  UpdateVolMaskToVolumeStructuredPoints(unsigned short *mask, int col, bool avoidZero, bool update, bool updateM2 = true, bool avoidExisting = false);
	int UpdateVolMaskToVolumeStructuredPoints(unsigned short *mask, int col, bool avoidNonZeroM2=true, bool update3D=true, bool updateM2=false,bool dontClear=false);
	int UpdateVolMaskToVolumeStructuredPoints(unsigned char *mask, int col, bool avoidNonZeroM2 = true, bool update3D = true, bool updateM2 = false, bool dontClear = false);
	void	LoadFastFilter(const QString & text);

	void	AddMaskToMask2(unsigned char*,bool clear=false);

	int FindFirstEmptyMaskUseHist();
	QString GetUniqueMaskName();
	
	void	CalculateMaskStatistics(unsigned char *mask,QString txt,bool drawHistogram=true,double *paramsTab=NULL,RawDataSet *orgData=NULL);
	unsigned char *GetCurrentMaskForStats(QString &txt);
	unsigned char* GetCurrentMultiMaskForStats(QString& txt,bool *selectedTab=NULL);
		RawDataSet *AutoCropWithMask();
		RawDataSet* AutoCropWithMask(RawDataSet* data, unsigned char*& MaskData);

	bool GetTTWSizes(const QString &filename, double *sizes);
	bool GetTTWPixels(const QString &filename, unsigned short *pixels,double *sizes);

	void CreateSphereStuff();


	int memorizedLineCounter;
	LinkedList <ThicknessLineItem*> *thicknessLineList;
	void UpdateLineNameLineEdit();
	
	void UpdateSegmentationToUseCropPlanes(unsigned char *maskData, RawDataSet *data);
	

	bool newMaskInserted;
	double prevTotalCheckSum;

	void	SetCamSettings(int mode);
	void	GenerateSurfacesFromMasks();

	void	GenAnimation(int start);

	bool dontSendSurfaceInfoAndBlockSelection;
	QTreeWidgetItem* mTreeWidgetItem, * oTreeWidgetItem;


	void	ArteryDataButtonClicked(int nr, ListItem<DataSet*>* mainListItem);
	void	UpdateGuiForMedDefs();
	void	ApplyJobElement(QString jobName);
	void	ApplyJob2Element(QString name);
	void	ApplyJob3Element(QString name);
	void	VisualizePlanes(RawDataSet* data, double* p, double* v1, double* v2, double* v3,QString name);

	double P1ForJoint[3];
	double P2ForJoint[3];

	QImage* testImage;
	void	GenerateXLSX();
	bool GetSelectedLineParams(double* p1,double* p2, QLineEdit2** lineEdits, QLineEdit2** angleEdits);

	void SetSurfaceGroupName(PolygonSurfaceData* data, QString groupName);

	QString arteryDataName;
	bool jobModeOn;
	void	CleanNonTubeSurfaces(QStringList* list,bool invertMode=false);
	void	ExtractLinesFromSurface();

	double originalScale[3];
	bool blockUpdateThresholds;

	void processJob(QString fn, QString jobType);
	void SlotApplyFilterComposition(QString name);
	bool ChangeGen3DFilterButtonVisibility();


	double lastRegFunctionValue;
	//int		GetLargestBlock(QString filename,int pos,bool getPeaks=FALSE,int *peakPosTab=NULL);
	int* MainWindow::GetLargestBlock(QString fn, int currentDataNr, int& largestBlock, bool getPeaks = false, bool getStab = false);
	bool SkipData(int pos);


	void	GenerateCroppedModelObjectTrans(int modelNr, int modelRange[2], int objectNr, int objectRange[2], QString destPath,QString destTransPath, bool genVol, RawDataSet* mainData=NULL,double *initTrans=NULL);

	void	SelectAllMasksFromMaskSetAndUseItAsMainMask();
	int VolumeInt;
	double VolumeFloat;
	void	CustomButtonClick(int i);
	void	CustomButtonSetClick(int i);
	void	ApplySufaceMatching();
	void	GenerateSurfaceDifferenceMapForTubes(QString dir="");

	int 	Shift2DMask(RawDataSet* data, unsigned char*& mask, QString shiftFileName);
	bool 	Shift2DRawData(RawDataSet* data,  QString shiftFileName);
	unsigned char* globalTestMask;

	QString universalHackNameBase;
	QXlsx::Document xlsx;
	//bool globalFix2D;
	void Grab3DWindowToUniversalHackNameBase(QString suffix);
	//safe way of deleting a COM object
	template<typename T>
	void SafeRelease(T& ptr) { if (ptr) { ptr->Release(); ptr = nullptr; } }


	public slots:

		void	SlotOnOffKinectFilter(int);
		void	SlotSaveCurrentMaskWithPreviousPath();
		void	SlotGenerateMaskUsingNeuralSegmentator();
		void	SlotGenerateMaskUsingJadraSegmentator();
		void	SlotSaveMaskSet(QString name);
		void SlotLastClickedButtonUpdateText(QString txt);
		void SlotMemorizedThresholds();
		void SlotApplyNewDevScale();
		
		void SlotResetOriginalScale();
		void SlotDopplerCalc();

		void	SlotExtractLinesFromData();
		void	SlotGenerateDistanceChart();

		void	SlotGrabDepthMapToClipboard();
		void	SetRestartAppMode(int);
		void	SlotStartJob(const QString filename);
		void	SlotStartJob();
		void	SlotStartJob2(const QString filename);
		void	SlotSnalyzeForLatestArteryDataFilesInSubdirectories();

		void	SlotFindTheBestCandidateForModelButton();
		
		void	SlotSelectFilesetButton();
		void	SlotSelectScriptButton();
		void	SlotSelectAffineTransPathButton();
		void	SlotSelectDestinationPathButton();
		void	SlotApplyJob3();
		void	SlotApplyTempOperation();
		void	SlotGetAllFilesForJobInDirectory();
		void	SlotGenerateRandomPointsAndPrepareJob2();
		void	SlotAddSubtitleLine();
		void	SlotSpecialFunctiionForGrowing1();

		void	SlotGenrateMasksAndSurfaces();
		void	SlotLoadState();
		void	SlotSaveState();
		void	SlotGenerateMedResults();

	void	SlotSetBoolVariableAfterXMiliSeconds(int x, bool& variable,bool value);
	void	SlotSendSurfaceTranslate(double *newP, double *oldP);
	void	SlotSendSurfaceZoom(double *vpNormal, double yf);
	void	SlotSendSurfaceRoll(double *vpNormal, double angle, double *TempOrig2);
	void	SlotActorPlaneMove(double *p0, double *p1);
	void	SlotActorPlaneRoll(double *normale, double angle, double *TempOrig2);
	void	SlotSurfaceRotate(double xf, double yf, double* viewUp, double* rotateY, double* TempOrigin2);

	void	SlotMasksUpdated();
	void	SlotCutstomButtonClick0();
	void	SlotCutstomButtonClick1();
	void	SlotCutstomButtonClick2();
	void	SlotCutstomButtonClick3();
	void	SlotCutstomButtonClick4();

	void	SlotCutstomButtonSetClick0();
	void	SlotCutstomButtonSetClick1();
	void	SlotCutstomButtonSetClick2();
	void	SlotCutstomButtonSetClick3();
	void	SlotCutstomButtonSetClick4();


	void	SlotLastButtonClick();
	void	SlotResetCropLines();
	void	SlotCleanNonTubeSurfaces();
	void	SlotSetOrientedSizes();
	void	SlotPartitionSkeleton();
	void	SlotGrabCurrentViewImage();
	void	SlotGrabAppImageToClipboard();
	void	SlotGrab3DWindowToClipboard();
	void	SlotGrabCurrentViewImageToClipboard();

	void	SlotUpdateFastFiltersGUIforPossibleChanges();
	void	SlotUpdateMainMixerGUIforPossibleChanges();
	void	SlotKeyReleaseEvent(QKeyEvent* event);
	void	SlotAddIgnorePoint();
	void	SlotRemoveIgnorePoint();
	void	SlotRemoveAllIgnorePoints();
	 
	void	SlotUpdateGuiForSpecialSlice(int);
	void	SlotSetNewRawDataSetName();
	void SlotAddSeedPoint();
	void SlotClearSeedPoints();
	void	SlotHistogramMode(int);

	void	Slot_memorizeUpperLinePushButtonPressed();
	void	Slot_memorizeLowerLinePushButtonPressed();
	void	Slot_calculateStatsBetweenLinesPushButtonPressed();
	void	Slot_deleteSelectedLinesPushButtonPressed();


	//void	SlotApplyFastFilter(int a);
	void	SlotAddLogText(const QString & text);
	void	SlotAddLogText2(const QString & text);



	//void	SlotAddLogText(const QString & text);
	void	SlotGenerateTestTubes();
	void	SlotMiddleVolumeBetweenTwoMasks();
	void	SlotTTW();
	void	SlotRice();
	void	SlotCropAirSpaces();
	void	SlotFitMinEllipsoid();//not ready|!
	void	SlotGenerateMaskStats();
	void	SlotGenerateGenerateSelMultiMaskStats();
	void	SlotGenerateSurfacesFromMasks();
	void	SlotGlyphsFromCurrentVol2();
	void	SlotUseMasksForBarrier();
	void	SlotChangeVolMaskColor(int);
	void	SlotShowMasksFromTree();
	void	SlotShowAllMasks();
	void	SlotClearVolMask();
	void	SlotDeleteVolMask();
	void	SlotUseMaskFor3DCheckBoxChanged(int);
	void	SlotAddCurrentMaskForTree();
	void	SlotLiveCurrentMaskForTree();
	void	SlotLiveCurrentMultiMaskForTree();

	void	SlotChangeMask3DOpacity(int);
	void	SlotResetClippingPlanes();
	void	SlotSetNewBounds3D(int*);
	void	SlotShow3DFPS();
	void	SlotLineSurfaceDataList();
	void	SlotUpdateVolumeColors(int);
	void	SlotUpdateVolumeColors();
	void	SlotGenerateMixedData();
	void	SlotGenerateMixedDataTransversal();
	void	SlotUpdateVolumeFromMixerMixedData();
	void	SlotChangeCropLinesVisibility(int);
	void	quit2();
	void	SlotGuiShowGUI(bool);
	void	SlotChangeCurrentWindowLevelSettings(QAction*action);
	void	SlotSetMagnifyingGlassScale(int);
	void	SlotShowVoxelCount(int);

	void	Slot2DWidgetClicked(int, int, int, int);
	void	SlotResetCameraTimer();

	void	Slot2DImageReleased(){ image2DPressed = false; };
	void	Slot2DImagePressed(){ image2DPressed = true; };

	void	SlotRedraw2DWidgets();
	void	SlotUpdateFastFilter();
	void	SlotUpdateFastFiltersSettings();
	void	SlotUpdateFastFiltersSettings(int);
	void	SlotUpdateFastFiltersSettings(const QString & text);


	void	SlotUpdateCurrentFastFilter();
	void	SlotForceRecalulateFilter();
	void	SlotApplyCurrentFastFilter(int v);

	void	SlotMixerFastFilterUpdate(int);
	void	SlotMixerFilterCurrentChanged(int);

	void	SlotCustomBox2Update(int);
	void	slotCameraCaptureFrame();
	void	slotProcessCapturedImage(int requestId, const QImage& img);
	//rgb to lookup 256 

	void	SlotRGBDataTo256Lookup();

	//artery
	void	SlotUpdateArterySettings();
	void	SlotUpdateArterySettings(int);
	void	SlotUpdateArterySettings(bool);

	//kinect
	void	SlotKinectGetBackgroundFrame();
	void	SlotMouseReleased();
	void	SlotKinectCalibrate();
	void	SlotKinectResetCrop();
	void	SlotGrabBackgroundDepth();
	void	SlotKinectFrustumGen();
	void	SlotUpdateKinectSettings();
	void	SlotUpdateKinectSettings(int);
	void	SlotUpdateKinectSettings(bool);
	void	SlotKinectEnable(int);
	//void	SlotDrawKinectDepthMap();
	//void	SlotRenderKinectDepthMap();
	void	SloInitKinectDone();
	void	SlotKinectValueChanged0(int);
	void	SlotCreateMaskFromBarrier();
	void	SlotCreateMaskFromPolygon();
	void	SlotCreateMaskFromLayer();
	void	SlotCreateEmptyMask();
	void	SlotOnOffMask(int);

	void	SlotSetTunningParam(int);
	void	SlotApplyImageSetStabilization();
	void	SlotApplyImageSetFrameMatching();
	void	SlotApplyImageSetFrameStabilization();
	void	SlotRawTreeWidgetItemClicked(QTreeWidgetItem * widget, int column);
	void	SlotRawTreeWidgetItemClicked2(QTreeWidgetItem * widget, int column);

	void	SlotMaskTreeWidgetItemClicked(QTreeWidgetItem * widget, int column);

	


	void	SlotUpdateFilterCompositions();
	void	SlotApplyFilterComposition();
	void	SlotApplyFilterComposition2(int a);
	void	SlotApplyFilterComposition2a();

	void	SlotAddFilterComposition();
	void	SlotAddFilterComposition(bool ask);

	void	UpdateViews();
	void	RepaintViews();
	void	SlotGrabFrameForFFMpegAnimation();

	void	SlotLoadUnknownRawDataSet(const QString &filename);
	void	SlotLoadTxtFloatVolume();
	//	void	SlotLoadL3DDataSet(const QString &filename);
	void	SlotLoadRawDataSet(const QString &filename);
	void	SlotLoadRawDataSet2(const QString &filename);

	void	SlotGenerateSphericalPointsCuts();
	void	SlotSetSelectedPointsRadius();
	void	SetCurrentSurfacePropertyToSelectProp();
	void	SlotRevertCurrentSurfaceProperty();
	void	SlotResetAllActorsTransformations();
	void	SlotShowAllActors();
	void	SlotHideSelectedActors();
	void	SlotUpdatePropsAndMaterials();
	void	SlotUpdateVolOpacityFuncs();
	void	SlotAddVolOpacityProperty();
	//void	SlotWriteUpdatedVolOpacity();
	void	SlotLineAntialiasingCheckBoxStateChanged(int);
	void	SlotDepthPeelingChceckBoxStateChanged(int);
	void	SlotShadowsChceckBoxStateChanged(int);

	
	void	SlotAddStandardProperty();

	void	SlotResolveConcidentGeometryMode();
	void	SlotResolveConcidentGeometryMode(int, double *);
	void	SlotGenerateTriangulationPoints();
	void	SlotGenerateCenteredPoints(int mode = 0);
	void	SlotGeneratePlaneFinderDefs();
	void	SlotShowHidePoints(bool);
	void	SlotShowHidePoints();
	//	void	SlotRandomTexSeed(int);
	//	void	SlotKinnectModeChange(KinMode kinm);
	void	SlotSetSkybox(int nr);
	void	SlotSetSkybox(QAction *Act);
	void	SlotSetBackgroundMode(int nr);
	void	SlotSetBackgroundMode(QAction *Act);
	void	slotSetFps(double, double);
	void	SlotStereoDisable();
	void SlotSegModeChanged(int nr);
	void	SlotVideoModeChanged(int nr);
	void	SlotShowMessage(const QString &Message);
	void	SlotShowMessage2(const QString &Message);
	void	SlotSetProgress(int Progress);

	void	SlotShowMessageProcessEvents(bool p);
	//void	slotChangeDockWidgetVisibility(QString name);
	void	SlotGetAngleParamsFromTwoPointSets(vtkPolyData *pD1, vtkPolyData *pD2, double *baseVectors, double *dirCos1, double *dirCos2, double *li1, double *li2, double &angle, bool fit = true, QString name = "", bool genSurface = true, bool commonCenter = false);
	void	SlotGenerateArteryResults();
	void	SlotSavePolyDataPts();
	void	SlotChangeSurfaceColor();
	void	SlotUpdateVeinCameraOrientation();
	void	SlotApplyVeinCameraOrientation();
	void	SlotSurfaceSeg3DRemoveFill(int, int);
	void	SlotSelect3DVolumePointOnSurface(int, int);
	void	SlotGenerateVisibleMask();
	void	SlotInsertDataSet(DataSet *dataset);
	void	SlotSetSurfaceRole(int nr);
	void	SlotSetSurfaceRole(QAction *);
	void	SlotShowSurfaceInfo(const QPoint & p);
	void	SlotLoadArteryData(const QString filename);
	void	SlotLoadArteryData();
	void	SlotSaveArteryData();

	void	SlotSetSurfacePoint(int);
	void	SlotSetSurfacePoint(const QString &);



	void	SlotLoadSeeds(const QString filename);
	void	SlotLoadSeeds();
	void	SlotSaveSeeds();
	void	SaveSeeds(QString name);
	void	SlotArteryClearDataButtonClicked(int nr);
	void	SlotArteryDataButtonClicked(int nr);
	void	SlotArteryDataShowButtonClicked(int nr);

	void	SlotChangeCurrentSurface3DInterpolationMode(int);
	void	SlotChangeCurrentSurfaceOpacity2(int);
	void	SlotChangeCurrentSurfaceRepresentation(int);
	void	SlotChangeCurrentSurfaceLighting(int);


	void	SlotChangeCurrentSurface3DInterpolationMode(QAction*);
	void	SlotChangeCurrentSurfaceOpacity2(QAction*);
	void	SlotChangeCurrentSurfaceRepresentation(QAction*);
	void	SlotChangeCurrentSurfaceLighting(QAction*);



	void	Slot3DWindowSurfaceMenu(int x, int y,QWidget *);
	void	SlotAppyPointArraySurfaceSegmentation(QPolygon qArray);
	void	SlotApplyPointArraySurfaceCreator(QPolygon qArray);
	void	SlotApplyFrustumOperation(QPolygon qArray);
	void	SlotReleaseFrustumPathCreator();

	void	SlotHideSurfaceWithChildren();
	void	SetOpacityWithChildren(PolygonSurfaceData* data, double opacity);
	void	SlotChangeCurrentSurfaceOpacity(int opacity);
	void	SlotMoveSphereActorToPos();
	void	SloUpdateClippedVolume();

	void	SlotGetFittedLineParams(double *line);
	void	SlotLive3DPointer(int x, int y);
	void	SlotSetTextPos(int x, int y);
	void	SlotActorPicker(int x, int y, int mode);
	void	SlotSetSurfaceOpacity(int);
	void	SlotGetSkeletalPoints();
	void	SlotGetAverageArteryDiameter();
	void	SlotGetAreaOfArteries();
	void	ApplyScatterPlotSegmentation(unsigned char *&tab, int sx, int sy);
	void	SlotProcessScatterImage(QImage *img);

	void	SlotVolumeGaussianSmoothing();
	void	SlotGetIntensityProbabilityData();
	void	SlotDetect3DEdges();
	void	SlotDetect2DEdges();
	// Feature point registration

	/*void	SlotAddModelPoint();
	void	SlotAddObjectPoint();
	void	SlotClearModelPointList();
	void	SlotClearObjectPointList();
	void	SlotStartFeaturePointOptimization();*/


	void	SlotLoadSnapshotForThickness();
	void	SlotLoadSnapshot();
	void	SlotLoadSeqSnapshot();
	void	SlotUpdateSnapshotButton();
	//-------Masks--------------

	void	SlotUndoMask();
	void	SlotRedoMask();
	void	SlotMaskToCurrent(int maskNr);
	void	SlotInsertNewMask();
	void	SlotInsertNewMask(ListItem<DataSet*>	*listItem, unsigned char *mask);


	void	SlotUndoMask2();
	void	SlotRedoMask2();
	//void	SlotMask2ToCurrent(int maskNr);
	void	SlotInsertNewMask2();
	void	SlotInsertNewMask2(ListItem<DataSet*>	*listItem, unsigned char *mask);

	void	SlotSwapMaskBuffers();
	void	SlotCopyMaskToBuffer();
	void	SlotBooleanMaskToBuffer();

	void	SlotCopyBufferToMask();
	void   SlotBooleanBufferToMask();

	//QPushButton *SwapMaskBuffersButton,*AddMaskToBufferButton,*BooleanMaskToBufferButton;

	void	SlotInsertMaskToModelOrObject(unsigned char *);
	void	SlotLoadMask(const QString &fn);
	void	SlotLoadMaskSet(const QString &filename);
	void	SlotLoadNewMaskAndCombineWithCurrent();
	void	SlotLoadMask();
	void	SlotSaveMask();

	void	SlotLoadMaskSet();
	void	SlotSaveMaskSet();

	void	SlotSwitchPaletteMode(QAction*);

	void	SlotSwitchToStretchedPaletteMode();
	void	SlotSwitchToNormalPaletteMode();
	void	SlotSwitchHistogramModes(int mode);
	void	SlotSwitchHistogramModes(QAction *);

	void	SlotOpenRawFile();
	void	SlotOpenVHPFile();
	void	SlotOpenDicomFile(const QString &filename);
	void	SlotOpenDicomFile();
	void	SlotSaveRawData(int Endian);
	void	SlotSaveRawDataSet2(const QString& filename);
	void	SlotSaveRawDataSet2();

	
	void	SlotLoadRawDataMaskAsUCharArray(const QString& filename);
	void	SlotLoadRawDataMaskAsUCharArray();

	void	SlotSaveRawDataMaskAsUCharArray(const QString& filename);
	void	SlotSaveRawDataMaskAsUCharArray();
	void	SetNewPalette(int nr);
	void	SetNewPaletteR(int nr);
	void	SetNewPalette(PaletteRGBA *_Palette);
	void	SlotSetNewPalette(PaletteRGBA _Palette);
	void	SetCorners(int x, int y, int z);
	void	SlotCropDataset();
	void	SlotAutoCropWithMask();

	void	SlotAutoLines();
	void	SlotSetP1ForJoint();
	void	SlotSetP2ForJoint();
	void	SlotCropVolume();
	RawDataSet *SlotCropVolume(RawDataSet *inData, unsigned char *&maskData, int *bounds);
	void	SlotRemovePart();
	void	SlotFipXDataset();
	void	SlotOptimCurrentPalette();
	void	InvertPalette();
	void	SlotRotateCurrentPalette(unsigned char value);
	void	InvertPaletteR();
	void	SlotMoveSphereActor(int x, int y, int z);
	void	SlotMoveSphereActor(double *pos, bool repaint = true, int mode = 0);
	void	SlotChangeBackgroundColor();
	void	SlotSetVolumeOpacity();
	void	SlotUpdateVolumeOpacityFunction(QPolygonF p, int mode);
	//	void	SlotSetVolumeOpacity(TransferFunction *UserTransferFunction);
	void	SlotSetVolumeColors();
	void	SlotSwitchVolumeOpacityMode();
	void	SlotShowHideVolume(int nr);
	void	SlotShowHideVolume(QAction *act);
	void	SlotChangeVolumePointerProperty();
	void	SwitchInteractorModes();
	void	SlotCreateResizeWidget();
	void	SlotResize3DPointer(int v);
	void	SlotUpdatePointerSize(int Value);
	void	SlotShowHideWidget(int nr);
	void	SlotShowHideWidget(QAction *action);
	void	SlotApplyScanlineSegmentation(void);
	void	SlotApplyThresholdSegmentation(void);
	void	SlotApplyThresholdRemove(void);


	void	SlotSetNewLayout(int i);
	void	SlotSetNewLayout(QAction *action);
	void	SlotReparentAll();
	void	SlotUpdateActors();

	void	SlotSaveGrab3DWindow();


	void	SlotSaveGrabWidgetWindow(QWidget *Widget, QString fn);
	void	SlotGrabApplicationWindow();

	void	SlotShowHide3DBoxActor();
	void	SlotShowHideAxes();
	void	SlotCreateAxePointer();
	void	SlotShowHide3DSlices();
	void	SlotUpdate3DSlicesTextureAndPos();
	void	SlotTransform3DObjectSlices(double *trans);
	void	SlotRotateCenterOf3DObjectSlices(double *R);

	void	SlotCreateObjectSlicesActor();

	//	void	SlotOpenSurface();
	void	SlotUpdate3DSlices();
	void	SlotRemove3DPlanes();
	void	SlotCreatePlaneNumberWidget();
	void	SlotUpdateNumberOfPlanes(int Value);
	//	void	SlotOpenMaskAndRemove();
	void	SlotShow3DPerpencdicularPlanes();
	void	SlotShowParallelSlicePlanes();
	void	SlotMoveLight();

	void	SlotGenerateScanlineHistogram();
	//	void	SlotOpenRegisteredFile();
	//	void	SlotRemoveRegisteredFile();
	void	SlotSwitchInspectionWindow(int);
	void	SlotSetNewInspectionMode(int);

	void	SlotApplyPropertySettings();
	void	SlotApplyPropertySettings(int);

	void	SlotApplyVolOpacitySettings();
	void	SlotApplyVolOpacitySettings(QString s);
	void	SlotApplyVolOpacitySettings(int);
	void	SlotApplyVolOpacitySettings2(int);


	void	SlotApplyShaderSettings();
	void	SlotFindPointClosestTo3DPointer();
	void	SlotSetSurfaceGroup();
	void	SetSurfaceGroupNumber(int);
	void	SetSurfaceGroupNumber(QAction *);
	void	SlotWriteUpdatedProps();
	void	SlotTextureTest();
	void	SlotTestTextureAnim();
	void	SlotShowShadersCheckBoxStateChanged(int);
	void	SlotRandTextureOrientation();
	void	SlotTextureAnim();

	void	SlotSwitchCreateSliceThroughPolyline(int);

	void	SlotChangePointerColor();
	void	SlotSetNewInspectionWindowSize(int, int, int);
	void	SlotChangeInterpolationMode(int);
	void	SlotChangeInterpolationMode(QAction *);
	void	SlotChangeCrossMode(int);
	void	SlotChangeCrossMode(QAction*);
	void	SlotChangeMaskColor();
	void	SlotChangeMaskColorX();
	void	SlotChangeMaskColorX2();

	void	SlotChangeMask2Color();
	void	SlotConnectAllViews();
	void	SlotDisconnectAllViews();

	void	SlotApplyFillSegmentation();
	void	SlotApplyFillMask();
	void	SlotErodeMask();
	void	SlotDilateMask();
	void	SlotInvertMask();
	void	SlotVolDilate();
	void	SlotVolErode();


	//	void	SlotSetLastClickedView(int);


	//--------------------------------------------

	void	SlotEnableDisableMixer(int);
	void	SlotUpdateDisableMixer(int); 
	void	SlotSetNewRawDataSet(QTreeWidgetItem *);
	//void	SlotSetCurrentDataSet(int);
	void	SlotDeleteDataSet(QTreeWidgetItem *);

	void	SlotCheckModelCompatibity();
	void	SlotSetNewInspectionData();
	void	SlotChangeSelectedDataSetOpacity(int);
	void	SlotSetNew3DView(RawDataSet *datax = NULL);
	void	InsertRawDataSet(RawDataSet *Xdata);
	void	SlotSetNewObjectDataSet();

	//void	SlotCreateEmptyMask();
	void	SlotCreateEmptyMask2();

	void	SlotCreateMaskANDMainDataDataSet();
	void	SlotCreateMaskNANDMainDataDataSet();
	void	SlotCreateANDorNANDDataSet(bool);

	RawDataSet *	CreateBlendingDataSet(RawDataSet *d);
	//*****************************************
	void	SlotGenerateParticlesModel();
	void	SlotGenerateParticlesModelFromMol();
	//--------------------------------------------
	void	SlotGenerateScatterPlot(double *Trans);
	void	SlotVolumeTransform(double *Trans);
	void	SlotGenerateMisregistrationGraphs(double *Trans);
	void	SlotGeneratePolyMisregistrationGraph(double *Trans);
	void	SlotGenerate3DVolumeGraph(double *Trans);
	void	SlotGenerate6DGraph(double *Trans);
	void	SlotStartRegistration(double *Trans);

	void	SlotApplyAffineTransformationFromVolumePanel(double *trans);

	void	SlotApplyAffineTransformationFromVolumePanel();
	//-------------------------------------

	
	void	SlotShowCameraSettings();
	void	SlotSetCameraSetting(int Mode);
	void	SlotSetCameraSettings2(int Mode);
	void	SlotSetCameraSetting(QAction *action);
	//////////////////////////////////
	void	SlotInteractWithCurrentActor();
	void	SlotInteractWithCamera();

	void	SlotInteractWith3DSection();

	void	SlotInteractWithCuttingTool();
	void	SlotSetNewCuttingToolActor();
	void	SlotShowHideCuttingToolActor();

	///////////////////////////////////
	void	SlotUpdatePropertyWidget();
	void	SlotOpenSurfacePropertyWidget();
	void	SlotInsertPolygonSurface(PolygonSurfaceData *Xdata);
	void	SlotInsertPolygonSurfaceAtTheAnd(PolygonSurfaceData *Xdata);

	void	SlotInsertPolygonSurfaceBeforeCurrent(PolygonSurfaceData *dataXX);


	void	SlotLoadPolygonSurfaceDataSet(const QString &filename);
	void	SlotLoadPolygonSurfaceDataSet();
	void	SlotCollapseAllSurfaceItems();
	void	SlotLoadPolygonSurfaceDataSet2(const QString &filename);
	void	SlotLoadPolygonSurfaceDataSet2();
	void	SlotLoadSTLSurfaceDataSet(const QString &filename);
	void	SlotLoadOBJSurfaceDataSet(const QString &filename);
	void	SlotLoadSTLSurfaceDataSet();


	void	LoadVTKSurfaceDataSet(const QString &filename);


	void	SlotSavePolygonSurfaceDataSet();
	void	SlotSaveSTLSurfaceDataSet();

	void	SlotShowPolygonSurfaceDataSet();
	void	SlotHidePolygonSurfaceDataSet();
	void	SlotSetNewModelPolygonSurfaceDataSet();
	void	SlotSetNewObjectPolygonSurfaceDataSet();
	void	SlotDeletePolygonSurfaceDataSet();
	void	SlotDeleteCurrentPolygonSurfaceDataSet();

	void	SlotDeleteAllPolygonSurfaceDataSets();
	void	SlotSetCurrentPolygonSurfaceDataSet(int);

	void	SlotCuttAllSurfaces();//done
	void	SlotCreateBoundaryDataFromSurfaces();//done
	void	SlotDecimateCurrentSurface();//done
	void	SlotSmoothAndDecimSurfcace();//done
	void	SlotGenerateNormalsPolydata();
	void	SlotDepthSortCurrentSurface();//done
	void	SlotCleanPolyData(); //done
	void	SlotComputeNormals(); //done
	void	SlotApplyDeformSurface();
	void	SlotSmoothSurface();//done
	void	SlotLargestSurfaceOnlySurface();//done
	void	SlotClosestToPointerSurfaceOnly();//done
	void	SlotSetRandomSurfaceColor();
	void	SlotExtractSeparatedSurfaces();


	void	SlotApplyActorTransformationAndRebuild(); //done
	void	SlotResetActorTransformation();	//done

	void	SlotTransformSelectedSurfaces();


	void	SlotCombineAllSurfaces();

	void	SlotApplySphereGeneration();
	void	SlotApplySurfaceTransformation();

	void	SlotGenerateDistanceMapSurfaceAutoSizeFromCrop();
	void	SlotGenerateDistanceMapFromCurrentSurface();
	void	SlotGenerateDistanceMapVolume();
	void	SlotGenerateDistanceMapVolumeFromMask();
	void	SlotGenerateDistanceMapVolumeFromEdgeMap();
	void	SlotDeformableSegmentation();

	void	SlotLoadLargestDicomSet();
	void	SlotAnalyzeDICOMdir();
	void	SlotAnalyzeDICOMdir(const QString &filename);
	void	SlotImportDICOMDataSet(int);
	void	SlotCompareTwoDicomFiles();
	void	SlotImportSelectedDICOMDataSet();

	void	SlotSaveAllSelectedSurfaces();

	//volume registration
	void	SlotVolumeRegistrationLoadTrans(const QString &filename);
	void	SlotVolumeRegistrationLoadTrans();
	void	SlotVolumeRegistrationSaveTrans();
	void	SlotVolumeRegistrationSaveTrans(const QString& filename);

	void	SlotElasticVolumeTransformation();
	void	SlotStartElasticVolumeRegistration();


	//MC
	void	SlotGenerateNewMarchingCubesSurface();
	void	SlotGenerateNewMarchingCubesDecimSurface();
	void	SlotGenerateMarchingCubesFilledIsland();
	void	SlotGenerateMarchingCubesFilledGreyIsland();
	void	SlotGenerateThresholdedMCSurface();
	void	SlotGenerateIslandsVolumeFromBuffers();

	//surface registration
	void	SlotDecolorizeTubeSurfacesWithoutRoles();
	void	SlotColorizeTubeSurfaceByRadius();
	void	SlotGetTubesStats();
	void	SlotGenerateSurfaceDifferenceMap();
	void	SlotGenerateSurfaceDifferenceMapForTubes();
	void	SlotApplySufaceMatching();
	void	SlotApplySufaceMatchingGenerateGraphs();
	void	SlotApplyElasticSurfaceMatching();
	void	SlotCreate3DGridPolyData();
	void	SlotMultiGridSurfaceDeform();
	void	SlotMultiGridSurfaceDeformLines();
	void	SlotMultiGridSurfaceDeformArrows();
	void	SlotMultiGridSurfaceDeformLinePaths();
	void	SlotMultiGridMultiVolumeDeform();
	void	SlotClear3DGrid();
	void	SlotLoadGrid3D();
	void	SlotLoadGrid3D(const QString &filename);
	void	SlotSaveGrid3D();
	void	SlotSaveGrid3D(const QString &filename);
	void	SlotElasticTransformation();

	void	SlotMapCorrespondingVolumeToMesh();
	void	SlotMapPaletteOnZAxisToMesh();

	//temp
	void	SaveCurrentArrayAsUnsignedChar();


	void	SlotSetApplicationMode(int);
	void	SlotRemoveAllModeConnections();
	void	SlotSetPressedButton(int, int);

	void	SlotSet2DViewsButtonMode(int Mode);
	void	SlotSetLastClickedView(int ID, int Slice);

	void	SlotAddMaskFromImageWidget(QImage img);
	void	SlotAddMaskFromImageWidget(unsigned char *tab);
	void	SlotAddMaskFromScatterPlot(unsigned char *tab);
	void	SlotAddMaskFromScatterPlot(unsigned char *tab, int sx, int sy);
	void	SlotAddMaskFromScatterPlot(QImage img);

	void	SlotModelObjectCombine(); //tandeta
	void	SlotCalculate2DataSetError(); //tez bez szalu

	void	SlotImportNastranData(const QString &filename);
	void	SlotImportNastranData();

	void	SlotImportVRMLData(const QString &filename);
	void	SlotImportVRMLData();


	void	SlotExportGMeshFile();
	void	SlotExportNastranMeshFile();

	void	SlotUnifyTriangleOrientation();
	void	SlotFlipTriangleOrientation();

	void	SlotFillPolyDataHolesAsPlanar();
	void	SlotFillPolyDataExtractedPlanarHoles();
	void	SlotFillPolyDataHoles();
	void	SlotFillPolyDataHolesMaxVolume();
	void	SlotFillPolyDataHolesWithCrenellations();

	void	SlotCalculateMeshVolume();
	void	SlotVoxelizeClosedVolume();
	void	SlotCalculateMaskVolume();





	void	SlotUpdateDefaultDir(const QString &filename, int Mode = -1);
	void	SlotCreateDefaultDirFileIfNotExists();

	void	SlotLoadLastFile();
	void	SlotLoadLastFilter();
	void	SlotLoadUniversalData(const QString &filename);
	void	SlotLoadUniversalData();



	void	SlotGet2DSlicesPos();

	void	SlotSetSurfaceOrigin(double *Origin);


	void	SlotCalculateNewSurfaceOrigin();

	void	SlotCopyToClipBoardCamera();
	void	SlotPasteFromClipboardCamera();

	void	SlotCameraRotation();
	void	SlotCameraRotationAndGrab();
	void	SlotMoveCamera(double *Point1, double *Point2);
	void	SlotMoveCameraPosAndViewUp(double *PosPoint, double *ViewUpPoint, double *FocalPoint);

	//---------------------------------------------------------
	void	SlotUpdateCamOnlyCheckBox(int a);
	void	SlotGoToKeyFrameItem();
	void	SlotGenerateAnimation();
	void	SlotGenerateAnimationFromCurrent();

	void	SlotInsertNewKeyFrameItem();
	void	SlotRemoveAllKeyFrameItems();
	void	SlotRemoveKeyFrameItem();
	void	SlotGrabAnimation();
	void	SlotAbortAnimation();
	void	SlotLoadAnimationSequence();
	void	SlotLoadAnimationSequence(const QString &filename);
	void	SlotSaveAnimationSequence();

	void	SlotLoadAnimationSequence2();
	void	SlotLoadAnimationSequence2(const QString &filename);
	void	SlotSaveAnimationSequence2();

	void	SlotMoveUpKeyFrameItem();
	void	SlotMoveDownKeyFrameItem();
	void	SlotSetPreviousKeyFrameItem();
	void	SlotSetNextKeyFrameItem();
	void	SlotUpdateKeyFrameItem();
	void	SlotUpdateAllSubsteps();
	//void	SlotSetNumberOfSubSteps(int Nr);
	//-----------------------------------------------------------

	void	SlotSet3DWindowSize(int nr);
	void	SlotSet3DWindowSize(int WSizeX, int WSizeY);



	void	SlotLoadJobScriptSequence();
	void	SlotLoadJobScriptSequence(const QString &filename);


	void	SlotSetGrabWindowPath();

	void	SlotMemorizeThresholds();


	//void	SlotUpdateSelectedSurfaces(QListView *);

	void	SlotUpdatePropertyOfAllSelectedSurfaces();


	void	SlotOpenBoxPropertyWidget();
	void	SlotUpdateBoxProperty();
	void	SlotDisconnectAllSlotsWithPropertyWidget();
	void	SlotOpenVolumePointerPropertyWidget();
	void	SlotUpdateVolumePointerProperty();
	void	SlotOpenCuttingToolPropertyWidget();
	void	SlotUpdateCuttingToolProperty();
	void	SlotOpenCuttingToolBackPropertyWidget();
	void	SlotUpdateCuttingToolBackProperty();

	void	SlotResizeCylinderBoxActor(double *Size);
	void	SlotChangeLightIntensity(int value);
	void	SlotShowHideLights();

	void	SlotExplodeUnselectedVisibleSurfaces(int);


	void	SlotMoveMaskByOneVoxel(int crossection, int direction);
	void	SlotCreateSliceThroughPolyline(QPolygon Array, double x, double y, int View);
	void	SlotUsePointArrayFrom2DView(QPolygon array, double x, double y, int View, int mode);

	void	SlotUpdateThresholds(int, int);
	void	SlotDeleteDistanceMap();
	void	SlotChangeRawDataSetsSelection();
	void	SlotSaveAllSelectedDataSets();
	void	SlotAverageRawDataSets();
	void	SlotAddNewMask(const unsigned char *mask);
	void	SlotAddNewMask2(const unsigned char *mask);

	void	SlotSegmentEllipsoid();
	void	SlotJoinTwoPointsWithTubeSemgent();
	void	SlotGeneratePathBasedSemgent();

	void	SlotStartCycleDatasetViews();
	void	SlotPauseCycleDatasetViews();
	void	SlotStopCycleDatasetViews();
	void	SlotGenerateCycle();


	void	SlotHideNewPolygonalSurfacesOn();
	void	SlotHideNewPolygonalSurfacesOff();
	void	SlotHideNewPolygonalSurfaces();
	void	SlotTest();
	void	SlotGeneratePointVolumeFromCurrentSurface();

	void	SlotReverseDeformationField();
	void	SlotScaleGrid();
	void	SlotGenerateTestGrid();

	void	SlotSaveRGBRawDataSet();
	void	SlotSaveRGBRawDataSetAsPng();
	void	SlotSaveRGBDataAs3DataSets();
	void	SlotGenerateDepthDataSet();

	void	SlotLoadMHAFIle(const QString& filename);
	void	SlotLoadTREFile(const QString& filename);
	void	SlotLoadVtkImageDataSet(const QString &filename, vtkImageReader2 *reader);
	void	SlotLoadJpegDataSet(const QString &filename);
	void	SlotLoadBmpDataSet(const QString &filename);
	void	SlotLoadTTWDataSet(const QString &filename);
	void    SlotLoadNIFTIWDataSet(const QString &filename);

	void	SlotStartRecordVideo(int mode);
	void	SlotStopRecordVideo();
	void	SlotPlayLastRecordedVideo();
	void	 SlotPauseRecordVideo();

	void	SlotSaveRawDataAsVideo();
	void	SlotSaveRawDataAsVideo(const QString &filename);
	void	SlotLoadVideoDataSet(const QString &filename);

	void	SlotLoadPngDataSet(const QString &filename);

	void	SlotTansformVolume();
	void	SlotGenerateHeartData();
	void	SlotSwitchSurfaceTransformMode(int i);
	void	SlotGenerateErrorSurface();
	void	SlotSwitchSurfaceTransformMode(int i, double *trans);
	void	SlotApplyImageSetRegistration();
	void	SlotApplyImageSetRegistration2();

	void	SlotCreateAverageSlice();

	void	SlotCreateBlendData();
	void	SlotCreateCarRawModel();
	void	SlotCreateHeightMapFromPictureData();
	void	SlotCreateSpheresMapFromPictureData();
	void	SlotStartElasticImageSetRegistration();
	void	SlotTestTransformImageSet();
	void	SlotGenerateArrowsImageSet();
	void	SlotAutoGrabTransversalImage();

	void	SlotGenerateTracedArrowsImageSet();

	void	SlotGenerateMaskAndSurfaceFromClippedVolume();
	void	SlotSphereShootSegmentation();
	void	SlotFilter3DBasedOn2DFilters();
	void	SlotFilter3DBasedOn2DFilters2();

	void	SlotExplode3DSegmentationLiveUpdate();
	void	SlotGrowing3DSegmentationLiveUpdate();
	void	Slot3DFilterLiveUpdate();
	void	Slot3DFilterLiveUpdate0();
	void	Slot3DFilterLiveUpdate1();
	void	Slot3DFilterLiveUpdate2();

	void	SlotExtractMaskedVolume();
	void	SlotKinnectTest(int nr);

	void	SlotSignalAmplitude();

	void	SlotUpdateVolumeMask();

	unsigned char	*SlotApplyThresholdSegmentation(int lthresh, int rthresh, RawDataSet *d);
	void	SlotArteryTrace();
	void	SlotArteryTrace2();
	//void	SlotTest1();
	void	SlotTest2();
	void	SlotTest3();
	void	SlotBoneMatching();
	void	SlotCreatePointMaskedDataSet();
	void	SlotArterySegmentation();
	void	SlotShowJoints();
	void	SlotStartRegistration(double *trans, ListItem<DataSet*>		*mainListItem, ListItem<DataSet*>		*objectListItem, double *pointsTab, int nrOfPoints, unsigned short val, RawDataSet *dd, RawDataSet *modelData, double *rot = NULL, double *constraints = NULL, bool oneScale = false);
	void	SlotVolumeTransform(double *trans, ListItem<DataSet*>		*mainListItem, ListItem<DataSet*>		*objectListItem);
	void	SlotGetTransformation(double *TParams);
	void	SlotTest4();




	void	SlotCropData();
	//void SlotGenerateArteryMeshFromThresh();
	void	SlotTestArteryMesh();
	void	SlotGenerateArteryMesh(int mode);
	void	SlotGetFirstLineParamsFromSelectedSkeletons();
	void	SlotGetSecondLineParamsFromSelectedSkeletons();
	void	SlotCalculateParamsOfArteries();
	void	SlotCalculateAngleBetweenTwoLines();



	void	SlotUndoSegCut();
	void	SlotRedoSegCut();
	void	SlotGetP4Plane(double *pts, double *base, bool vis = false, QString name = "");
	void	SlotGetP4PlaneFromBase4Points(double* pts, double* base, bool vis = false, QString name = "");

	void	SlotSetP4PointLU();
	void	SlotSetP4PointPU();
	void	SlotSetP4PointLO();
	void	SlotSetP4PointPO();
	void	SlotSetP4Plane();
	void	SlotSetCurrentPointAndText(double *pt, const QString &text);
	void	SlotSetBaseVectors(double *v1, double *v2, bool orthogonal = false);
	void	SlotGetBaseVectors(double *v1, double *v2, double *base, bool orthogonal = false);
	void	SlotSetBaseVectorsAndOrigin(double *origin, double *_baseVectors);
	void	SlotSetSeedPoint(double *seedP);

	void	SlotSetBaseOrigin(double *p);

	void	SlotSetPointPM1();
	void	SlotSetPointP();
	void	SlotSetPointPP1();
	void	SlotSetBaseVectors();
	void	SlotResetBasePoints();

	void	SlotSpecialCut();
	void	SlotInsertNewMaskAndUpdateMC(const unsigned char *);
	void	SlotInsertNewMask(const unsigned char *);
	void	SlotInsertNewMaskFromFrustum(const unsigned char*);
	void	SlotUpdatedLiveMask(const unsigned char *);
	void	SlotReplaceCurrentSurfaceWithMask();
	void	SlotChangeClippingPlanes(int);
	void	SlotBlockRepaintViewWidgets(bool a);


	void	SlotForSignalSendShape(vtkPoints *pts);
	void	SlotForSignalSendPointP1(double p1[3]);
	void	SlotForSignalSendPointP2(double p2[3]);

	void	SlotShowTmpSurfaces();
	void	SlotHideTmpSurfaces();
	void	SlotDeleteTmpSurfaces();
	//void	SlotSendTextForGui(const QStringList list);


	void SlotKinnectRightHand(float x, float y, float z, float x2d, float y2d);
	void SlotKinnectRotateRelative(float azimuth, float elevation, float roll);
	void SlotKinnectRotateRelGrab(float axisx, float axisy, float axisz, float angle);
	void SlotKinnectZoomRel(float zoomDist, float zoomProgress);
	void SlotKinnectRollAbs(float roll);


	void SlotDefineSelectedPoint(QAction *action);
	void SlotInvertSurfaceSelection();
	void SlotSelectAllSurfaces();
	void SlotSelectAllVisibleSurfaces();
	void SlotSelectAllInvisibleSurfaces();
	void SlotSelectAllSurfacesInCurrentGroup();
	void SlotSelectAllSurfacesWithRoles();
	void SlotSurfaceTreeWidgetSelectionChanged(QTreeWidgetItem * item, int column);
	void SlotTreeWidgetItemSelectionChanged();

	void SlotUpdateVolumeSettings(int);
	void SlotUpdateVolumeSettings();
	//void	SlotLoadMixerSettings();
	//void	SlotSaveMixerSettings();
	void	SlotLoadMixerSettings(const QString &filename);
	void	SlotSetMaskViewOptions(int);
	void	SlotUpdateCheckBoxes(int);
	void	SlotUpdateAirSpacesLineEdits(int);
	//void	SlotTest1(int x, int y, QMouseEvent* event) {};
	//SIGNALS!!!!!!!!!!!!!!!!!!!!

signals:

	void  signalBoolVariableAfterXMiliSeconds(int x, bool& variable,bool value);
	//void	signalXX(int x, int y, QMouseEvent* event);
	//void	signalKeyReleaseEvent(QKeyEvent* event,int );

	void	SignalKinectValueChanged0(double);

	void	SendSurfaceParemeters(SurfaceData *SData, int ActualActor, bool GaussCheck, double SdvX, double SdvY, double SdvZ, bool DecimateCheck, double Tred, bool SmoothingCheck, bool ConnectCheck);

	void	SendNewPalette(void);
	void	SendNewPaletteR(void);

	void	SendMessage(const QString &Message);
	void	SignalSendProgress(int Progress);
	void	SendDatasetInfo(int Nx, int Ny, int Nz, double Xscale, double Yscale, double Zscale, const QString &filename);
	void	SendtPolygonSurfaceDatasetInfo(int Nx, int Ny, int Nz, double Xscale, double Yscale, double Zscale, const QString &filename, int Nr1, int Nr2, double Or1, double Or2, double Or3);

	//-----------------------------------------------

	void	SignalSendModelCopmatibility(int*);
	void	SignalSendNewOpacity();

	//-----------------------------------------------
	void	SignalSendSectionParameters(int, int, int, int, int, int);


	void	Signal2DViewClicked();

	void	SignalSend2DSlicesPos(int, int, int);

	void	SignalSendBoundingBoxSize(double *);
	void	SignalSendShowMessageProcessEvents(bool);
	void	SignalPolygonSurfaceInserted();

	void	SignalSetSlice(int, int, int);

	void	signalEmitTransversalPos(int, int);
	void	SignalSendPolyLineForMixer(QPolygon array);

};
Q_DECLARE_METATYPE(QKeyEvent*)

//Q_DECLARE_METATYPE(QMouseEvent*)

#endif //MAIN_WINDOW
