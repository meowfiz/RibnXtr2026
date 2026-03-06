#ifndef	GUI_PANEL_WIDGET
#define	GUI_PANEL_WIDGET
#include "MyQtObjects.h"
#include <qsettings.h>
#include <qwidget.h>
#include <qimage.h>
#include <qpainter.h>
#include <qslider.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qactiongroup.h>
//#include <qgroupbox.h>
//#include <q3groupbox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
//#include <q3listbox.h>
//#include <q3hbox.h>
//#include <q3vbox.h>
#include <qcombobox.h>
#include <qpixmap.h>
//#include <q3canvas.h>
#include <qspinbox.h>
//#include <q3listview.h>
//#include <QMenu.h>
#include <qvalidator.h>
//#include <q3buttongroup.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qscrollarea.h>
#include <qmainwindow.h>
#include <qtreewidget.h>
#include <qtoolbar.h>
#include <qsplitter.h>
#include <qplaintextedit.h>

#include <qsignalmapper.h>
#include <qheaderview.h>
#include "RawDataLayer.h"

#include "ImageWidget.h"
#include "MyDockWidget.h"
#include "ParamsWidget.h"
#include "DataSet.h"
#include "SurfaceCollection.h"
#include <stdio.h>

//class QPushButton2;
//static int buttonCount = 0;
//static QVector <QPushButton2*>;
//static int currentButton = 0;

//#define CONFIG_NAME "config.ini"




class RawDataLayer;

#define NUMBER_OF_DOCKWIDGETS 21
#define NUMBER_OF_REG_SLIDERS 9
#define NUMBER_OF_KIN_SLIDERS 11
#define NUMBER_OF_KIN_CHECKBOXES 13

class TextFieldDoubleValidator : public QDoubleValidator {
public:
	TextFieldDoubleValidator(QObject * parent = 0) : QDoubleValidator(parent) {}
	TextFieldDoubleValidator(double bottom, double top, int decimals, QObject * parent) :
		QDoubleValidator(bottom, top, decimals, parent) {}

	QValidator::State validate(QString & s, int & pos) const {
		if (s.isEmpty() || (s.startsWith("-") && s.length() == 1)) {
			// allow empty field or standalone minus sign
			return QValidator::Intermediate;
		}
		// check length of decimal places
		QChar point = locale().decimalPoint();
		if (s.indexOf(point) != -1) {
			int lengthDecimals = s.length() - s.indexOf(point) - 1;
			if (lengthDecimals > decimals()) {
				return QValidator::Invalid;
			}
		}
		// check range of value
		bool isNumber;
		double value = s.toDouble(&isNumber);

		//QString revTxt = QString::number(value);

		//int len0 = s.length();
		//int len1 = revTxt.length();
		//int lenMin = min(len0, len1);
		//QString cut1 = s.mid(0, lenMin);
		//QString cut2 = revTxt.mid(0, lenMin);
		//
		//isNumber = (QString::compare(cut1, cut2) == 0);

		

		if ((isNumber)&& bottom() <= value && value <= top()) {
			return QValidator::Acceptable;
		}
		return QValidator::Invalid;
	}

};


class GUIPanelWidget : public QObject//: public MyDockWidget
{
	Q_OBJECT
private:

	 


public:
	double iconScaleFactor;
	//QPushButton2 *buttonTab[1000];
		QList<QPushButton2*> *buttonList;
		QList<QCheckBox2*> *checkBoxList;
		QList<QComboBox2*> *comboBoxList;
		QList<QLineEdit2*> *lineEditList;
		QList<QSlider2*> *sliderList;
		QList<QSpinBox2*> *spinBoxList;



		QLineEdit2* animTxtLineEdit;
		QLineEdit2* animTextPosLineEdit[4];
		QLineEdit2* animTextColorLineEdit[3];


		QCheckBox2* animOnlyOpacityOfGroupsCheckBox;
		QCheckBox2* updateVolumeCheckBox;
		QCheckBox2* autoLoadLastStateCheckBox;
		QCheckBox2* autoRestartAppCheckBox;

		QSpinBox2* nrOfRandbomSamplesSpinBox;
		QPushButton2* generateRandomPointsAndPrepareJob2Button;
		QPushButton2* analyzeForLatestArteryDataFilesInSubdirectoriesButton;
		QPushButton2* startJobButton;
		QPushButton2* createDirForJob3Button;
		QPushButton2* applyScriptForFob3Button;
		QPushButton2* applyTempButton;
		QLineEdit2* sufixJob3LineEdit;
		QLineEdit2* job3ScriptLineEdit;
		QLineEdit2* job3FilePathSetLineEdit;
		QLineEdit2* job3DestinationPathLineEdit;
		QLineEdit2* job3RawFileParamsLineEdit;
		QCheckBox2 *logRawDataCheckBox;

		QPushButton* lastClickButtonQPushButton;
		QPushButton* customButtons[5];
		QPushButton* customSetButtons[5];
		QCheckBox2* subdirsJob3CheckBox;
		QCheckBox2* onlyLatestFileInSubdirsJob3CheckBox;

		QLineEdit2* job3IndexListLineEdit;
		QLineEdit2* job3AffineTransPathLineEdit;
		QPushButton2* selectAffineTransPathButton;
		
		QPushButton2* selectFilesetButton;
		QPushButton2* selectScriptButton;
		QPushButton2* selectDestinationPathButton;
		int autoLoadCheckBoxNr;
		int rDataCounter;
		int mDataCounter;
		QList<QString> *paletteNames;
		QCheckBox2 *kinectOptionsCheckBox[NUMBER_OF_KIN_CHECKBOXES];
		QComboBox2 *kinectImagePrefilterComboBox;
		QSlider2 *kinectSlider[NUMBER_OF_KIN_SLIDERS];
		QSlider2 *kinectBlobRoiSlider[4];  // Blob ROI: minX, maxX, minY, maxY (depth image coords, 0..10000)
		QSlider2 *kinectMaxSatSlider;      // max saturation dla detekcji blobów (0..255)
		QLabel *kinectLabel[20];
		QPushButton2 *kinectFrustumButton;
		QPushButton2 *kinectCalibrateButton;
		QPushButton2 *kinectGrabBackgroundDepthButton;
		QPushButton2 *kinectGrabBackgroundFrameButton;


		QCheckBox2* job3TestModeCheckBox;
		QCheckBox2* job3DataNumberingCheckbox;
		QLineEdit2* job3CustomNameLineEdit;
		QLineEdit2* job3RegModelDataNumberLineEdit;

		QTreeWidget *DICOMDataTreeWidget;
		QCheckBox2 *ImportAllWithErrorsCheckBox;
		QCheckBox2	*IgnoreWindowLevelValuesCheckBox;
		QCheckBox2 *InvertDicomPixelRepresentationCheckBox;
		QCheckBox2 *ShiftValuesToZeroCheckBox;
		QLineEdit2 *ShiftValuesLineEdit;
		QLineEdit2 *dicomDistTolEdit;
		QCheckBox2* veinRGBCheckBox;
		QLineEdit2* veinRGBLineEdits[3];
		QGroupBox* seedsGroupBox;

		ImageWidget *visImage;
		ImageShapes	 shapes;
		QImage		*infoImg;

		QColor	distinctColors[300];
		QIcon	distinctColorIconsRect[300];
		QIcon	distinctColorIconsCirc[300];
	QPushButton2 *showTmpButton,*hideTmpButton,*deleteTmpButton;
	QPushButton2 **MaskToolButton;
	QSignalMapper *segToolsMapper;
	QSignalMapper *videoToolsMapper;
	QCheckBox2* genStabCheckBox;
	
	QLineEdit2* maxValueOverrideForDistanceMapLineEdit;

		MyDockWidget *GUIWidgets[NUMBER_OF_DOCKWIDGETS];
	
		MyDockWidget *widgetHistory[4];
	//MyDockWidget *lastRightDockWidget,*lastLeftDockWidget,*preLastRightDockWidget,*preLastLeftDockWidget;

		int findMyWidgetPos(MyDockWidget *w);

		// Helpers for persisting/restoring \"maximized\" dock widgets per side.
		QString getCurrentDockObjectNameForSide(Qt::DockWidgetArea area) const;
		void restoreCurrentDocksFromNames(const QString& leftName, const QString& rightName);

	GUIPanelWidget(unsigned char * _PalTab, int _PalNr,QList <QString> *names, QList<QPushButton2*>* _buttonList, QList<QCheckBox2*>* _checkBoxList, QList<QComboBox2*>* _comboBoxList,
		QList<QLineEdit2*>* _lineEditList, QList<QSlider2*>* _sliderList, QList<QSpinBox2*>* _spinBoxList,QWidget *parent=0, double iconFactor=1.0, QString name="");
	//void SetLists(QList<QPushButton2*>* _buttonList,	QList<QCheckBox2*>* _checkBoxList,	QList<QComboBox2*>* _comboBoxList,
	//	QList<QLineEdit2*>* _lineEditList,	QList<QSlider2*>* _sliderList,	QList<QSpinBox2*>* _spinBoxList)
	//{
	//	buttonList=_buttonList;
	//	checkBoxList= _checkBoxList;
	//	comboBoxList=_checkBoxList;
	//	lineEditList=_checkBoxList;
	//	sliderList=_checkBoxList;
	//	spinBoxList=_checkBoxList;
	//}

	QPlainTextEdit *logTextEdit;
	QPlainTextEdit* subtitleTextEdit;
	QPushButton2* addSubtiutleLineButton;
	QPushButton2 *clearLogPushButton;

	void	CreateDevTab(MyDockWidget* qtab);

	void	CreateJobsTab(MyDockWidget *qtab);
	void	CreateLogTab(MyDockWidget *qtab);

	void	CreateVolumeVisTab(MyDockWidget *qtab);
	void	CreateDICOMTab(MyDockWidget *qtab);
	void	CreateVolumeMixerTab(MyDockWidget *qtab);
	void	CreateFastFiltersTab(MyDockWidget *qtab);
    void	CreateInfoTab(MyDockWidget * qtab);
    void	CreateSurfaceTab(MyDockWidget *qtab);
	void	CreateMaskTab(MyDockWidget *qtab);
	void	CreateSurfacePropertyTab(MyDockWidget *qtab);
	void	CreateSegmentationTab(MyDockWidget *qtab);
	void	CreatePaletteTab(MyDockWidget *qtab);
	//void	CreateFilteringTab(MyDockWidget *qtab);
	void	CreateSurfaceProcessingTab(MyDockWidget *qtab);
	void	CreateRegistrationTab(MyDockWidget *qtab);
	void	CreateSurfaceRegistrationTab(MyDockWidget *qtab);
	void	CreateSequenceGenerationTab(MyDockWidget *qtab);
	void	CreateArteryAnalyzeTab(MyDockWidget *qtab);
	void	CreateArteryGenerationTab(MyDockWidget *qtab);
	void	CreateToolBars();

	QCheckBox2 *removeCurrentLiveAddFromMainMaskCheckBox;
	QCheckBox2 *AutoThresholdForAirspacesCheckBox;
	QCheckBox2 *AirspacesOptionsCheckBox[4];
	QPushButton2 *AirspacesOptionsButton[4];
	QLineEdit2 *AirspacesLineEdit[4];


	QPushButton2 * CropAirSpacesPushButton;
	QPushButton2 *GetMiddleVolumeBetweenTwoMasksPushButton;
	QPushButton2 *GenerateTestTubesButton;
	QTreeWidget *masksTreeWidget;

	QPushButton2 *riceButton;
	QPushButton2 *ttwButton;

	QCheckBox2	*useMaskFor3DViewCheckBox,*useFrustomOnLiveMaskCheckBox,*useMaskForBarrierCheckBox;
	QPushButton2 *fitMinEllipsoidButton;
	QPushButton2 *GenerateMaskStatsButton;
	QPushButton2* GenerateSelMultiMaskStatsButton;
	QComboBox2 *maskStatsSourceComboBox;

	QPushButton2* GenerateMaskUsingNeuralSegmentatorButton;
	QPushButton2* GenerateMaskUsingJadraSegmentatorButton;

	QPushButton2 *LoadMaskForTreeButton;
	QPushButton2 *SaveMaskForTreeButton;
	QPushButton2* LoadMaskAsUnsignedCharArrayButton;
	QPushButton2 *AddCurrentMaskForTreeButton, *AddLiveMaskForTreeButton, *AddLiveMutliMaskForTreeButton;
	QCheckBox2 *AddCurrentAddToMask2CheckBox;
	QComboBox2 *m2ComboBox;
	QLineEdit2	*CurrentMaskNameLineEdit;
	QSlider2		*CurrentMask3DOpacitySlider, *BlockMaskOpacitySlider;
	QPushButton2 *UseMasksForBarrierButton;
	QPushButton2 *ShowMasksFromTreeButton;
	QPushButton2 *ClearVolMaskButton;
	QComboBox2 *ChangeVolMaskColorComboBox;
	QPushButton2 *DeleteVolMaskButton;
	QPushButton2 *GenerateSurfacesFromMasksButton;
	QPushButton2 *GenerateGlyphsFromCurrentVol2PushButton;
	QPushButton2 *InvertDuplicateMaskButton;
	QSlider2 *MaskWeightsSlider1, *MaskWeightsSlider2;
	QToolBar *segToolBar, *videoToolBar, *segToolBar2, *volumeToolBar;
	QToolBar* customButtonsToolBar;
	QActionGroup *aGroup, *vGroup;
	QAction *segAction[20];
	QAction *videoAction[20];
	QCheckBox2 *genArteryOptionCheckBox[20];
	QLineEdit2 *minSkelLengthLineEdit;
	QLineEdit2 *tubeDestLineEdit;
	QActionGroup* cGroup;
	QToolBar* camToolBar;
	QSignalMapper* camToolsMapper;
	QAction* camAction[20];
	QSlider2 *MagnifyingGlassSlider;

	QButtonGroup *skeletonButtonGroup;
	QComboBox2 *cutGeometryComboBox,*cutModeComboBox, *singlePointComboBox;
	QCheckBox2 *separateSurfacesChceckBox;
	QLineEdit2 *depthCutLineEdit;

	QLabel *seedLabel[3];
	QPixmap pixmapRed, pixmapGreen;
	QPushButton2 *arterySetButtons[30];
	QPushButton2 *arteryClearButtons[30];
	QLabel *arteryStateLabels[30];
	QLabel *arteryNameLabels[30];
	QPushButton2 *loadArteryDataButton,*saveArteryDataButton;
	QButtonGroup *arteryStateButtonGroup,*arteryShowButtonGroup,*arteryClearButtonGroup;

	QSlider2 *planeSliders[6];
	QSlider2 *volumeRenSettingsSliders[9];
	QCheckBox2 *volumeRenCheckBox[6];
	QComboBox2 *volumeBlendComboBox;
	QPushButton2 *p4Button[4];
	QPushButton2 *p4PlaneButton;

	//QTabWidget *qtab;
	QPushButton2 *GenerateThresholdedMCSurfaceButton;
	QPushButton2 *AverageDataSetsButton;
	QLineEdit2 *AverageAmountEdit;

	bool	UpdateListBlocked;
	QCheckBox2 *CreateSliceThroughPolylineCheckBox;
	QButtonGroup *polyLineButtonGroup;
//	QListBox	*ModelListBox,*ObjectListBox;
	QPushButton2	*AddModelPointPushButton,*AddObjectPointPushButton;
	QPushButton2	*ClearModelPointList,*ClearObjectPointList;
	QPushButton2 *StartFeaturePointOptimization;

	QTreeWidget		*surfaceTreeWidget;

	QPushButton2	*CombineAllSurfacesButton;
	QPushButton2 *WhichSurfaceIsCurrentButton;

	QPushButton2 *ShowHide3DSectionButton;
	QPushButton2 *AnalyzeDICOMdirButton;

	QRadioButton *GrabAppOnRegRadioButton;
	QRadioButton *Grab3DWindowOnRegRadioButton;

//	QCheckBox2 *ImportAllWithErrorsCheckBox;
//	QCheckBox2 *InvertDicomPixelRepresentationCheckBox;
//	QCheckBox2 *ShiftValuesToZeroCheckBox;
//	QLineEdit2 *ShiftValuesLineEdit;
//	QLineEdit2 *dicomDistTolEdit;
	QPushButton2 *loadSeedsPushButton;
	QPushButton2 *saveSeedsPushButton;
	unsigned char	*PalTab;
	int PalNr;

	int MaxToleranceValue;
	//-------------------------------------------
//	QTreeWidget *DICOMDirTreeWidget;
	QCheckBox2 *autoWindowLevelCheckBox;
	QTreeWidget		*rawDataTreeWidget;
	QLineEdit2 *rawDataNameLineEdit;
	QWidget *paletteAndVisWidget;
	QWidget *Gb1PP;
	QLineEdit2 *rawDataLayerNameLineEdit;
	QWidget   *mixerFilterWidget;
	QWidget *mixerParamsWidget;
	QIcon	mixerIcons[10];
	QIcon	emptyIcon;
	QPushButton2 *LoadSnapshotPushButton;
	QCheckBox2   *autoLoadSnapshotCheckBox;
	QComboBox2	*thicknessFilterComboBox;
//	QSlider2		*tunningThicknessFilterSlider;
	QPushButton2 *LoadSnapshotSeqPushButton;
	QLabel		*tunningLabel;
	QPushButton2 *addSeedPushButton;
	QPushButton2 *clearSeedsPushButton;
	QWidget *GbSnapshot;

//#define PARAMS_NR 50
	QPushButton2		*ImportSelectedDICOMButton;


	QCheckBox2 *OnOffMaskCheckBox;
	QPushButton2 *AddBarierPushButton;
	QPushButton2 *AddPolygonToMaskPushButton;
	QPushButton2	*AddMaskFromLayerFilterPushButton;
	QPushButton2 *ClearMasksPushButton;
	QPushButton2 *ClearMasks2PushButton;
	QPushButton2 *Explode3DPushButton;
	QCheckBox2	*ExplodeLiveUpdateCheckBox;
	QSlider2 *ChangeMaskVisibilitySlider;

	QPushButton2 *RedoSegmentButton;
	QPushButton2 *UndoSegmentButton;

//#define PARAMS_NR 50
	QLineEdit2 *newTubeParams[10];
	QCheckBox2* errorTubeModeCheckBox;
	QPushButton2 *colorizeErrorTubesButton;
	QPushButton2 *colorizeTubesButton;
	QPushButton2 *getTubesStatsButton;
	QPushButton2* getOrientedSizesButton;
	QLineEdit2 *maxRadiusToColorizeEdit;
	QPushButton2* copyToClipboard3DDepthMapButton;

	QFrame* partitionSkeletonWidget;
	QPushButton2* decolorizeTubeSurfacesWithoutRolesButton;
	QPushButton2 *cleanNonTubeSurfacesButton;
	QPushButton2 *partitionSkeletonButton;
	QTreeWidget *thicknesLinesTreeWidget;
	QPushButton2 *memorizeUpperLinePushButton;
	QPushButton2 *memorizeLowerLinePushButton;
	QLineEdit2 *lineNameLineEdit;
	QPushButton2 *calculateStatsBetweenLinesPushButton;
	QPushButton2 *deleteSelectedLinesPushButton;


	QComboBox2* functionForGenAnimComboBox;
	QLineEdit2* functionForGenAnimLineEdit[4];


	QComboBox2 *filterCompositionsComboBox;
	QPushButton2 *AddFilterCompositionButton, *ApplyFilterCompositionButton;
	QCheckBox2	*autoUpdateCurrentFilterCheckBox;
	QLineEdit2 *FilterCompositionNameLineEdit;

	QCheckBox2 *enableFastFilterCheckBox;

	QComboBox2		*mixerMaskFilterModeComboBox;
	QComboBox2		*mixerLayerMaskSourceComboBox;
	QComboBox2		*mixerLayerSourceComboBox;
	QTreeWidget		*mixerTreeWidget;


	QCheckBox2		*disableRawDataMixerCheckBox;
	QCheckBox2		*disableSeparatedMixersCheckBox[3];
	QCheckBox2		*disableRawDataMixerCheckBox2;

	//QPushButton2		*mixerCreateMixedLayerPushButton;
	QPushButton2		*mixerMoveUpPushButton, *mixerMoveDownPushButton, *mixerRemovePushButton;
	QPushButton2		*mixerGenerateMixedVolumePushButton;
	QPushButton2		*mixerUpdate3DVolumePushButton;
	QPushButton2		*mixerChildModePushButton;
	
	QPushButton2		*mixerCreateLayerPushButton;

	//QCheckBox2		*mixerLayerEnableCheckBox;
	//QCheckBox2		*mixerLayerSourceMixedCheckBox;
	//QLineEdit2		*mixerMultipierLineEdit;
	//QLineEdit2		*mixerAdditionLineEdit;
	//QComboBox2		*mixerModesComboBox;
	//QSlider2			*mixerOpacitySlider;
	//QLineEdit2		*mixerParamsLineEdit;


	QLabel			*mixerOpacityLabel;

	//	QCheckBox2		*mixerLayerOptimizeCheckBox;
	









	//QCheckBox2		*mixerMaskFilterEnableCheckBox;//*mixerMaskFilterEnableThresholdReverseCheckBox;

	/*QSlider2			*mixerMaskThresholdSlider[2];
	QLineEdit2		*mixerMaskParamsLineEdits[10];
	QLabel			*mixerMaskParamsLabels[10];
	QLabel			*mixerThresholdValueLabel[2];
	QWidget			*mixerMaskParamsWidgets[10];*/

	//QCheckBox2		*mixerMaskFilterParamsCheckBoxes[PARAMS_NR];
	//QLineEdit2		*mixerMaskFilterParamsLineEdits[PARAMS_NR];
	//QLabel			*mixerMaskFilterParamsNameLabels[PARAMS_NR];
	//QLabel			*mixerMaskFilterParamsLabels[PARAMS_NR];
	//QSlider2			*mixerMaskFilterParamsSliders[PARAMS_NR];
	//QWidget			*mixerMaskFilterParamsWidgets[PARAMS_NR];




	QComboBox2		*mixerFilterModeComboBox;

	/*QCheckBox2		*mixerFilterParamsCheckBoxes[PARAMS_NR];
	QLineEdit2		*mixerFilterParamsLineEdits[PARAMS_NR];
	QLabel			*mixerFilterParamsNameLabels[PARAMS_NR];
	QLabel			*mixerFilterParamsLabels[PARAMS_NR];
	QSlider2			*mixerFilterParamsSliders[PARAMS_NR];
	QWidget			*mixerFilterParamsWidgets[PARAMS_NR];


	 

	QCheckBox2		*fastFilterParamsCheckBoxes[PARAMS_NR];
	QLineEdit2		*fastFilterParamsLineEdits[PARAMS_NR];
	QLabel			*fastFilterParamsNameLabels[PARAMS_NR];
	QLabel			*fastFilterParamsLabels[PARAMS_NR];
	QSlider2			*fastFilterParamsSliders[PARAMS_NR];
	QWidget			*fastFilterParamsWidgets[PARAMS_NR];*/

	ParamsWidget *mainLayerParamsWidgets[PARAMS_NR];
	ParamsWidget *fastFilterParamsWidgets[PARAMS_NR];
	ParamsWidget *mixerFilterParamsWidgets[PARAMS_NR];
	ParamsWidget *mixerMaskFilterParamsWidgets[PARAMS_NR];

	QComboBox2		*fastFiltersComboBox;
	QPushButton2		*applyCurrentFastFilterPushButton,*apply3DFastFilterPushButton;
	QPushButton2		* forceRecalculateFilterPushButton;
	QPushButton2		*updateFastFilterPushButton;


	//QCheckBox2		*mixerFilterEnableCheckBox;
	//QCheckBox2		*mixerFilterInverseCheckBox;

	//QCheckBox2		*mixerAutoNormalizeDataValuesCheckBox;

	QLineEdit2* editDopplerX, * editDopplerY;
	QPushButton* dopplerCalcGroupBox;
	QGroupBox* OCTGroupBox;

	QGroupBox* VolumeCropGroupBox;
	QGroupBox* FastFiltersGroupBox;
	QGroupBox* MasksAndParamsGroupBox;

	QPushButton2* ShowAllMasksButton;
	QPushButton2* GenrateMasksAndSurfacesButton;
	QPushButton2* LoadStateButton;
	QPushButton2* SaveStateButton;
	QPushButton2* GenerateMedResultsButton;


	QPushButton2		*DeleteItemButton;
	QPushButton2		*InsertItemButton;
	QPushButton2		*SaveRawDataSetButton;

	///////////

	QPushButton2* CollapseAllSurfaceItemsButton;
	QPushButton2		*DeleteSurfaceItemButton;
	QPushButton2		*InsertSurfaceItemButton;
	QPushButton2		*OpenSurfacePropertyWidgetButton;
	QPushButton2		*OpenSurfacePropertyWidgetButton2;


	QPushButton2		*SetNewDataSetButton;
	//QListViewItem	*CurrentItem;
	bool			Helper,	HelperFilt,Helper2;
	
	//QMenu		*ItemAttributesMenu;
	

	//QListViewItem	*ModelItem;
	//QListViewItem	*ObjectItem;

	//View2DItemArray	*View2DItems;
	//QListViewItem	*View3DItem;
	//QListViewItem	*InspectionItem;
	//View2DItemArray *InspectionItems;

	QSpinBox2 *volMorphInSpinBox,  *volMorphBackSpinBox,*volMorphMaxIterSpinBox;
	QPushButton2 *SwapMaskBuffersButton,*CopyMaskToBufferButton,*BooleanMaskToBufferButton,*CopyBufferToMaskButton,*BooleanBufferToMaskButton;
	QPushButton2 *IslandsVolumeFromBuffersButton;
	QPushButton2 *SphereShootSegmentationButton;
	QSpinBox2 *SphereShootItersSpinBox,*SphereShootMinSizeSpinBox;
	QPushButton2 *UndoSegment2Button,*RedoSegment2Button;

	///////////////////////////////////
	//QListViewItem	*CurrentPolygonSurfaceDataItem;
	//QListViewItem	*ModelPolygonSurfaceItem;
	//QListViewItem	*ObjectPolygonSurfaceItem;

//	QCheckBox2		*InsertAsModelCheckBox;
	int				NumberOfCompatibleItems;
	QPushButton2		*ClearMaskButton;
	QPushButton2		*ClearMask2Button;
	QPushButton2		*NANDButton;
	QPushButton2		*ANDButton;
	QLabel* lastclickedButtonNumberLabel;
	QPushButton2		*LoadMaskButton,*SaveMaskButton;
	QCheckBox2* blockFix2DCheckBox;
	QPushButton2* SaveMaskAsUnsignedCharArrayButton;
	QPushButton2* SaveCurrentMaskWithPreviousPathButton;
	//9999
	QComboBox2 *standardVolOpacityComboBox, *standardVolOpacityComboBox2;
	QComboBox2* fpsComboBox;
	QPushButton2 *addStandardVolOpacityButton;
	QLineEdit2 *standardVolOpacityLineEdit;
	//QPushButton2 *writeUpdatedVolOpacityButton;
	//9999

	QComboBox2 *volMorphTypeComboBox;
	QSpinBox2 *volMorphSizeSpinBox;
	QCheckBox2 *volMorphBoundsCheckBox;
	QCheckBox2 *volUseMaskCheckBox;
	QPushButton2 *showNormalsButton;
	QLineEdit2 *scaleNormalsLineEdit;
	QPushButton2 *setSurfaceGroupButton;
	QPushButton2* saveAllSelectedSurfacesButton;
	QComboBox2 *shaderSelectComboBox,*standardPropertyComboBox;
	QCheckBox2 *showShadersCheckBox, *depthPeelingChceckBox, *lineAntialiasingCheckBox, *shadowsCheckBox;;

	QPushButton2 *setShaderToSurfacesButton,*setStandardPropertyToSurfacesButton,*AddStandardPropertyButton;
	QLineEdit2 *standardPropertyLineEdit;
	QPushButton2 *writeUpdatedPropsButton;
	QPushButton2 *testTextureButton;
	QCheckBox2 *repeatTexCheckBox;
	QCheckBox2 *normXTexCheckBox,*normYTexCheckBox;
	QLineEdit2 *scaleXTextureLineEdit,*scaleYTextureLineEdit;
	QLineEdit2 *centerXTexLineEdit,*centerYTexLineEdit;
	QLineEdit2 *dxTexAnimLineEdit,*dyTexAnimLineEdit,*msecTexAnimLineEdit;
	QComboBox2 *testTextureSelectComboBox,*testTextureModeComboBox;
	QLineEdit2 *scaleTextureLineEdit,*angleTextureLineEdit,*startPointTextureLineEdit;
	QPushButton2 *findClosestPointTo3DPointerPushButton;
	QLineEdit2 *orientTextureLineEdit[3];
	QCheckBox2 *randomTexSeedCheckBox;
	QPushButton2 *randTextureOrientationButton;
	QLineEdit2 *iterTextureLineEdit;
	QPushButton2 *textureAnimButton;

	QPushButton2		*GenerateMarchingCubesSurfaceButton;
	QPushButton2		*GenerateMarchingCubesDecimSurfaceButton;
	QPushButton2		*GenerateMarchingCubesFilledIslandButton;
	QPushButton2		*GenerateMarchingCubesFilledGreyIslandButton;
	QPushButton2		*VolDilateButton,*VolErodeButton;
	QPushButton2		*CalculateMaskVolumeButton;
	QPushButton2		*AutoCropWithMaskButton;
	QCheckBox2 *setOpacityToPrevCheckBox;
	QSlider2 *opacityToPrevSlider;
	//------------------------------

	QSpinBox2	*FillGreyIslandsMinSizeBox;

	QPushButton2 *TransformSurfaceButton;

	QLineEdit2 *SurfaceScaleXEdit;
	QLineEdit2 *SurfaceScaleYEdit;
	QLineEdit2 *SurfaceScaleZEdit;

	QLineEdit2 *SurfaceShearXEdit;
	QLineEdit2 *SurfaceShearYEdit;
	QLineEdit2 *SurfaceShearZEdit;

	QLineEdit2 *SurfaceOriginXEdit;
	QLineEdit2 *SurfaceOriginYEdit;
	QLineEdit2 *SurfaceOriginZEdit;

	QPushButton2 *SetSurfaceOriginButton,*computeSurfaceCenterButton;
	
	QPushButton2 *ShowTransformSurfaceParamsButton;
	QWidget		*TransformSurfaceParamsWidget;



	QPushButton2 *SphereGenerationButton;
	QLineEdit2 *SpherePhiEdit;
	QLineEdit2 *SphereThetaEdit;

	QPushButton2 *FindMaxDistanceButton;
	//---------------------------------------------
	QWidget		*TransformWidget;
	QWidget		*FeaturePointsWidget;
	QPushButton2	*ShowTransformWidgetButton;
	QPushButton2 *ShowFeaturePointsWidgetButton;

	QPushButton2	*SetRigidTransformationModeButton;
	QPushButton2	*SetAffineTransformationModeButton;

	QPushButton2 *StartElasticVolumeRegistrationButton;
	QPushButton2 *StopElasticVolumeRegistrationButton;
	QPushButton2 *ApplyElasticVolumeTransformation;
	QSpinBox2	*ElasticIterBox,*ElasticStepBox;
	QCheckBox2	*ElasticRandomSamplingBox;
	QCheckBox2	*ElasticRegistrationFixedBoundsBox;
	QSpinBox2	*GridDensityBox;
	QPushButton2	*ReverseGridButton;
	QPushButton2 *ScaleGridButton;
	QPushButton2 *GenerateTestGridButton;

	QPushButton2 *ShowVolumeElasticRegistrationPanelButton;
	QWidget		*VolumeElasticRegistrationWidget;

	QPushButton2 *MultiGridDeformButton;
	//QLineEdit2	*MultiGridDeformAmountEdit;
	QSpinBox2	*MultiGridDeformSubFramesBox;
	QCheckBox2	*ElasticUpdateGridBox;
	QCheckBox2	*ElasticLoopBox;
	QSpinBox2	*MultiResolutionStepBox;
	QPushButton2 *MultiGridDeformLinesButton;
	QPushButton2 *MultiGridDeformArrowsButton;

	QComboBox2 *VectorDeformationComponentCombo;
	QCheckBox2		*MultiGridDeformColorizeBox;
	QCheckBox2		*MultiGridDeformRelativeBox;
	QPushButton2		*MultiGridDeformLinePathsButton;
	QCheckBox2		*MultiGridTransBox;

	QLineEdit2	*MultiGridMaxDistEdit;
	QCheckBox2	*MultiGridAutoMaxDistBox;

	QLineEdit2	*TEdit[20]; //na zapas
	QSlider2		*TSlider[20]; // na zapas
	QCheckBox2	*TCheckBox[20]; //na zapas

	QCheckBox2	*AutoCenterCheckBox;
	QCheckBox2	*AlignCentersCheckBox;
	double		*TParams;

	QSlider2 *affineRegistrationSlider;;
	QSlider2		*parallelThreadSlider;
	QLabel		*parallelThreadLabel;
	QCheckBox2	*parallelRegistrationCheckBox;
	QComboBox2	*ScatterSizeCombo;
	QComboBox2	*InterpolationCombo;
	QComboBox2	*MisregistrationFunctionCombo;
	QLabel		*MissregInfoLabel;
	QSpinBox2	*ToleranceSpinBox;
	QCheckBox2	*ReversedTransformCheckBox;
	ImageWidget *IWidget;
	QCheckBox2	*ScatterPlotVisualizationCheckBox;



	QPushButton2 *SaveTransformationButton;
	QPushButton2 *LoadTransformationButton;



	QPushButton2 *ResetTransformationButton;

	QPushButton2 *ScatterPlotButton;
	QPushButton2 *VolumeTransformButton;
	QPushButton2* SurfaceTransformButton;
	QPushButton2	*GenerateMisregistrationGraphsButton;
	QPushButton2	*StartRegistrationButton;
	QPushButton2	*StopRegistrationButton;


	QPushButton2 *ShowVolumeRegistrationGraphButton;
	QWidget		*VolumeRegistrationGraphWidget;
	QSpinBox2 *GraphXSpinBox,*GraphYSpinBox,*GraphZSpinBox;
	QSpinBox2 *ResolXSpinBox,*ResolYSpinBox,*ResolZSpinBox;
	QLineEdit2 *TTransSizeEdit,*TRotSizeEdit; 


	QPushButton2 *Generate6DRandomGraphButton;
	QPushButton2	*Generate3DMisregPolyGraphButton;

	QPushButton2 *GeneratePolyMisregistrationGraphButton;

	QSpinBox2 *RegStepXBox;
	QSpinBox2 *RegStepYBox;
	QSpinBox2 *RegStepZBox;

	//**********************************************
	
	//QListView	*AvailableFiltersListView;
	//QListView	*FilteringPipelineListView;
	QPushButton2	*AddFilterButton;
	QPushButton2	*RemoveFilterButton;
	QPushButton2	*RemoveAllFiltersButton;
	QPushButton2	*MoveUpSelectedFilterButton;
	QPushButton2	*MoveDownSelectedFilterButton;
	QPushButton2	*ApplyFilteringButton;


	//***********************************************

//	QTabWidget	*qtab;
//	QTabWidget  *VolumeDataTab;
//	QTabWidget	*SurfaceDataTab;
//	QTabWidget	*ArteryDataTab;




	QPushButton2 *CropDatasetButton;
	QPushButton2 *RemovePartButton;
	QPushButton2 *OpenNewSurfaceButton;

	QPushButton2 *FlipXButton;


	QPushButton2 *InvertPaletteButton;
	QComboBox2	*PaletteCombo;
	QPushButton2 *PrevPaletteButton;
	QPushButton2 *NextPaletteButton;

	QLabel *currentPaletteName;
	QPushButton2 *InvertPaletteButtonR;
	QComboBox2	*PaletteComboR;
	QPushButton2 *PrevPaletteButtonR;
	QPushButton2 *NextPaletteButtonR; 
	QSlider2		*OpacitySlider;
 	QSlider2		*MaskOpacitySlider;
	QSlider2* threshMaskOpacitySlider;
	QCheckBox2 *maskModeCheckBox[3];
	QSlider2		*MaskBufferOpacitySlider;
	QSlider2		*ToleranceSlider;
	QLineEdit2 *ToleranceEdit,*ToleranceValueEdit; 
	double TempPercent;

	QSlider2		*histogramModeSlider;
	QSlider2		*tunningThicknessFilterSlider;
	QComboBox2 *exploreMatchingResultsModeBox;
	QPushButton2 *exploreMatchingResultsPushButton;
	QSlider2 *exploreMatchingOpacitySlider;
//	QPushButton2 *LoadSnapshotPushButton;
	QPushButton2 *LoadSnapshotForThicknessPushButton;
//	QCheckBox2   *autoLoadSnapshotCheckBox;
//	QComboBox2	*thicknessFilterComboBox;
//	QSlider2		*tunningThicknessFilterSlider;
///	QLabel		*tunningLabel;
	QFrame		*frameToAnimMatchFrame;
//	QCheckBox2   *autoLoadSnapshotCheckBox;
//	QComboBox2	*thicknessFilterComboBox;
//	QSlider2		*tunningThicknessFilterSlider;
//	QLabel		*tunningLabel;
	QSlider2		*tunningThicknessFilterSlider2;
	QLabel		*tunningLabel2;
//	QFrame		*frameToAnimMatchFrame;
	QFrame		*frameToAnimMatchFrame2;
	QLineEdit2* scaleDevLineEdit[3];
	QPushButton2* applyNewDevScaleButton;
	QPushButton2* resetOriginalScaleButton;
	QWidget* transSurfacesWidget;
	QWidget* visNormalsWidget;
	QWidget* logTabMainWidget;
	QWidget* devTabMainWidget;
	QWidget* infoTabMainWidget;
	QGroupBox* sufreceInfoTabGroupBox;
	QWidget* dicomTabMainWidget;
	QWidget* frameMatchingWidget;
	//QPushButton2* maskopacity3DWidget;
	QLabel* l3dMasksOpacityLabel;
	QLabel* liveMaskOpacityLabel;


	QAction* buttonSegToolsActions[20];

	QPushButton2 *frameMatchingPushButton;
	QPushButton2 *frameStabPushButton;
	QPushButton2 *frameStabilizationPushButton;
	QPushButton2* frameStabilizationCopyPushButton;
	QCheckBox2 *frameMatchingCheckBox;
	QPushButton2* findTheBestCandidateForModelButton;
	QLineEdit2* medFactorJob3LineEdit;
	QLineEdit2* minSliceModelLineEdit;
	QLineEdit2* maxSliceModelLineEdit;
	QLineEdit2* minModelBlockSizeLineEdit;
	QCheckBox2* job3SegmentatorModeCheckBox;

	QCheckBox* inverseIndexListCheckBox;;
	QLabel		*LabelFileName;
	QLabel		*LabelNx,*LabelNy,*LabelNz;
	QLabel		*LabelXscale,*LabelYscale,*LabelZscale;


	QLabel		*LabelSurfaceFileName;
	QLabel		*LabelSurfaceNx,*LabelSurfaceNy,*LabelSurfaceNz;
	QLabel		*LabelSurfaceXscale,*LabelSurfaceYscale,*LabelSurfaceZscale;

	QLabel		*LabelSurfaceNumberOfPoints;
	QLabel		*LabelSurfaceNumberOfPolys;
	QLabel		*LabelSurfaceNumberOfPoints2;
	QLabel		*LabelSurfaceNumberOfPolys2;
//	QCheckBox2 *frameMatchingCheckBox;
	QPushButton2 *frameMatchingStopPushButton;

	QCheckBox2 *frameToAnimMatchCheckBox[10];

	QSlider2 *bruteForceSlider;
	QLabel *bruteForceLabel;

	QSlider2 *frameToAnimMissPixelsSlider;
	QLabel  *frameToAnimMissPixelsLabel;

	QSlider2 *rangeMatchSlider[2];
	QLabel *rangeMatchLabel[2];


	QSlider2 *imgMatchOptSlider[NUMBER_OF_REG_SLIDERS];
	QString imgMatchOptName[NUMBER_OF_REG_SLIDERS];
	QLabel  *imgMatchOptLabel[NUMBER_OF_REG_SLIDERS];
	int imgMatchOptRange[NUMBER_OF_REG_SLIDERS * 2];


	QComboBox2 *frameToAnimMatchFunctionComboBox;
	QComboBox2 *frameToAnimMatchPrefilterComboBox;
	QCheckBox2* usePrefilterCheckBox;
	////

	//Marching cubes
	QCheckBox2 *GaussCheck;
	QLineEdit2 *SdvX,*SdvY,*SdvZ;
	QCheckBox2 *DecimateCheck;
	QLineEdit2 *Tred;
	QCheckBox2 *SmoothingCheck;
	QCheckBox2 *ConnectCheck;


	QSpinBox2	*PercentSpinBox; 
	QSpinBox2	*NeighSpinBox;
	QPushButton2 *ScanLineSegmentButton;
	QPushButton2 *FillSegmentButton;



	QButtonGroup *MaskButtonGroup;




	QPushButton2 *DilateMaskButton;
	QPushButton2	*ErodeMaskButton;
	QPushButton2 *InvertMaskButton;
	QPushButton2	*FillMaskButton;
	QSpinBox2	*MorphologyKernelSizeSpinBox,*MorphologyKernelTypeSpinBox;;
	QCheckBox2	*MorphologyWithCriterionCheckBox;
	QCheckBox2	*ClearWithFillCheckBox;

	QCheckBox2	*ObjectThresholdCritBox;



	QComboBox2	*HomogenityCriterionCombo;
//	QCheckBox2	*ToleranceCheckBox;
//	QCheckBox2	*ThresholdsCheckBox;
	QCheckBox2	*ThreeDimCheckBox;
	QCheckBox2	*MaskAppendCheckBox;

//	QPushButton2 *RedoSegmentButton;
//	QPushButton2 *UndoSegmentButton;

	QCheckBox2	*ScanXCheck,*ScanYCheck,*ScanZCheck;

	QPushButton2 *ThresholdSegmentButton;
	QPushButton2 *ThresholdRemoveButton;


	QLineEdit2	*HEdit,*KEdit;

//	Q3ListBox	*SurfaceListBox;
	QCheckBox2	*SurfaceVisibleCheck;
	QPushButton2		*RemoveSurfaceButton;
	QLineEdit2	*SurfaceNameEdit;
	QPushButton2		*PropertySurfaceButton;


	QCheckBox2	*InspectionWindowCheckBox;
	QSlider2		*InspectionXSizeSlider;
	QSlider2		*InspectionYSizeSlider;
	QSlider2		*InspectionZSizeSlider;


	QComboBox2	*InspectionCombo;
	QPushButton2 *ScalineHistButton;	

	QCheckBox2 *specialSliceCheckBox;
	QComboBox2 *specialSliceComboBox;
	QSlider2	*specialSliceParam1Slider;
	QSlider2	*specialSliceParam2Slider;
	QSlider2	*specialSliceOpacitySlider;
//Surface Processing

	QLineEdit2 *resolveParams[3];
	QComboBox2 *resolveConcidentGeometryComboBox;
	QPushButton2 *applyResolveSettingsButton;

	QPushButton2	*SegmentCuttButton;
	QComboBox2	*CutterCombo;
	QPushButton2 *ShowHideCutterButton;
	QPushButton2 *CuttSurfaceButton;
	
	QPushButton2 *CreateBoundaryDataButton;
	QPushButton2* setMemorizedThresholdsButton;
	QSlider2		*CutterSizeSlider;
	QSlider2		*CutterHeightSizeSlider;
	
	QPushButton2 *SmoothAndDecimSurfcaceButton;
	QPushButton2 *DepthSortSurfaceButton;
	QPushButton2 *DecimateSurfaceButton;
	QLineEdit2	*DecimateValueEdit;
	QLineEdit2	*DecimateErrorEdit;

	QLineEdit2 *PointNumberTextEdit;
	QPushButton2 *TransSurfacePointButton;
	QSlider2		*TransSurfacePointSlider;
	
	QPushButton2 *DeformSurfaceButton;
	QLineEdit2	*DeformValueEdit;


	QComboBox* MedDefsComboBox;

	QPushButton2	*SmoothSurfaceButton;
	QLineEdit2	*SmoothFactorValueEdit;
	QLineEdit2	*SmoothIterValueEdit;

	QPushButton2	*LargestSurfaceOnlyButton;

	QPushButton2	*ClosestToPointerSurfaceOnlyButton;
	QPushButton2	*extractSeparatedSurfacesButton;

	QPushButton2	*ComputeNormalsButton;
	QPushButton2	*CleanPolyDataButton;

	QPushButton2 *SetRandomColorsToSelectedSurfacesButton;
	QPushButton2 *GenerateSphericalPointsCutsBuutton;
	QPushButton2 *ResetActorTransformationButton;
	QPushButton2 *ApplyActorTransformationButton;


	QPushButton* ExtractLinesFromDataButton;
	QPushButton* GenerateDistanceChartButton;

////////////////////////

	QPushButton2 *ModelObjectCombineButton;
	QPushButton2 *ModelObjectDistanceMapButton;
	QPushButton2	*DeleteDistanceMapButton;
	QLineEdit2 *LThreshLineEdit,*RThreshLineEdit;

	QPushButton2 *ApplyAffineTransformationFromVolumePanelButton;
	QCheckBox2 *invAffineCheckBox,*invVolAffineCheckBox;
	QPushButton2 *GenerateDistanceMapButton;
	QPushButton2* GenerateDistanceMapWithAutoCropButton;
	QPushButton2 *ApplySurfaceMatchingButton;
	QPushButton2 *GenerateSurfaceDistMapButton;

	QSpinBox2 *PointPercentSpinBox;
	
	QPushButton2 *ApplySufaceMatchingGenerateGraphsButton;

	QComboBox2 *FillModeCombo;
	QCheckBox2 *FillThroughAllSlices;

	QSpinBox2 *DistStepXBox;
	QSpinBox2 *DistStepYBox;
	QSpinBox2 *DistStepZBox;



	QSpinBox2 *GridSizeXBox,*GridSizeYBox,*GridSizeZBox;
	QSpinBox2 *GridSizeX1Box,*GridSizeY1Box,*GridSizeZ1Box;
	QPushButton2 *ApplyElasticSurfaceMatchingButton;
	QPushButton2 *LoadGrid3DButton,*SaveGrid3DButton,*ElasticTransformationButton;
	QLineEdit2	*ElasticTransAmountEdit;
	QPushButton2 *Create3DGridPolyDataButton;
	QPushButton2 *Clear3DGridPolyDataButton;
	QSpinBox2	*SurfaceElasticIterBox;
	QCheckBox2	*SurfaceElasticUpdateGridBox;


	QLineEdit2	*BBoxEdit[4]; //na zapas
	QSlider2		*BBoxSlider[4]; // na zapas
	QCheckBox2	*BBoxCheckBox[4]; //na zapas

	QPushButton2 *ShowBoundingBoxParamsButton;
	QWidget *BoundingBoxParamsWidget;
	double		BBoxParams[4];


	QCheckBox2 *BoundingBoxPointsCheckBox;

//////////////////////
	QLineEdit2 *TransformCameraRollEdit;
	QLineEdit2 *TransformCameraAzimuthEdit;
	QLineEdit2 *TransformCameraElevationEdit;

	QPushButton2 *TransformCameraButton;
	QPushButton2 *TransformCameraAndGrabButton;
	QPushButton2 *Grab3DWindowButton;
	QPushButton2 *CopyToClipBoardCameraButton;
	QPushButton2 *PasteFromClipboardCameraButton;
	
	
	QComboBox2 *Window3DSizeCombo;
	/////////////////////////
	bool LastSurfaceItemDeleted;


	//------------------------------------------------
	QPushButton2* updateAllSubstepsButton;
	QCheckBox2	*UpdateCamOnlyCheckBox;

	QPushButton2 *GoToKeyFrameItemButton;
	QSpinBox2	*NumberOfKeyFrameItemBox;

	QPushButton2 *MoveUpKeyFrameItemButton;
	QPushButton2 *MoveDownKeyFrameItemButton;	
	QPushButton2 *SetPreviousKeyFrameItemButton;	
	QPushButton2 *SetNextKeyFrameItemButton;	
	
	QPushButton2 *InsertNewKeyFrameItemButton;	
	QPushButton2 *UpdateKeyFrameItemButton;	

	QPushButton2 *RemoveKeyFrameItemButton;	
	QPushButton2 *RemoveAllKeyFrameItemButton;	

	QPushButton* GenerateAnimationFromCurrentKeyframeButton;
	QPushButton2 *GenerateAnimationButton;	
	QPushButton2 *GrabAnimationButton;	
	QPushButton2 *AbortAnimationButton;	

	QCheckBox2	*LoopAnimationCheckBox,*smoothAnimationCheckBox;
	QPushButton2 *loadAnimPushButton,*saveAnimPushButton;
	QLineEdit2	*CurrentKeyFrameNrEdit;
	QLineEdit2		*CurrentTKeyFrameNrEdit;
	QLabel		*NumberOfAllKeyFramesLabel;
	QSpinBox2	*NumberOfSequenceSubStepsBox;

	

	QCheckBox2 *LoopVisibilityChechBox;
	//QSpinBox2 *NrOfTotalLoopSurfacesBox,
	QSpinBox2	*NrOfFrameSurfacesBox;
	//FrameStepNr;
	QSlider2 *explodeRestSlider;
	QLineEdit2 *explodeSliderScaleLineEdit;
	QLineEdit2 *explodeRestRotLineEdit[3];
	
	QPushButton2 *ShowHideLightsButton;
	QSlider2		*LightIntensitySlider;
	QLabel		*LightIntensityLabel;
//-------------------------------------------------	
	QWidget *DeformableSegmentationWidget;
	QLineEdit2 *Shape1LineEdit,*Shape2LineEdit,*Intensity1LineEdit,*Intensity2LineEdit;
	QSpinBox2  *DensitySpinBox;
	QPushButton2 *ApplyDeformableSegmentationButton;
//---------------------------------------------------
	QPushButton2 *StartCycleDatasetViewsButton;
	QPushButton2 *StopCycleDataSetViewsButton;
	QPushButton2 *PauseCycleDatasetViewsButton;
	QLineEdit2	*CyclePeriodEdit;
	QCheckBox2	*CycleGrabCheckBox;
	QSpinBox2	*NrOfCycleSetsBox;

	QSpinBox2 *seedPointXBox,*seedPointYBox,*seedPointZBox;
	QSpinBox2 *cropSizeXBox,*cropSizeYBox,*cropSizeZBox;
	QSpinBox2 *maxDistBox,*firstThreshBox,*lastThreshBox;
	
	QPushButton2 *pButton[3];
	QPushButton2  *setBaseButton,*resetBaseButton;

	QLineEdit2	*v0Edit[3],*v1Edit[3],*v2Edit[3],*v3Edit[3];
	QPushButton2 *showArteryObjectsButton[12];
	QPushButton2 *loadLargestDicomSetButton;
	
	QLineEdit2	*l1AnglesEdit[3], *l2AnglesEdit[3];

	QSpinBox2 *tumorIterBox,*skelSmoothIterBox,*skelSmoothWeightBox;
	QCheckBox2 *smoothSkeletonBox,*medianTumorBox,*startTumorBox;
	QPushButton2 *UndoSegCuttButton,*RedoSegCuttButton;
	QComboBox2 *frustumSegModeComboBox;
	QPushButton2 *cropDataButton;
	QPushButton2 *cropVolumeButton;
	QPushButton2* cropVolumeButton2;
	QPushButton2 *resetCropLines;
	QCheckBox2 *showCropLines2DCheckBox;
	

	QPushButton2* autoLinesButton;
	QPushButton2* setP1ForJointButton;
	QPushButton2* setP2ForJointButton;
	QPushButton2 *testSkeletonButton;
	QPushButton2 *generateSkeletonButton,*generateFrontTreeButton;
	QPushButton2 *savePointsToFileButton;
	QPushButton2 *getFirstLineButton,*getSecondLineButton;
	QLineEdit2 *aFirstLineEdit,*bFirstLineEdit,*cFirstLineEdit;
	QLineEdit2 *aSecondLineEdit,*bSecondLineEdit,*cSecondLineEdit;
	QPushButton2 *calculateLineAngleButton;
	QLineEdit2 *lineAngleEdit;
	QPushButton2 *calculateParamsButton;
	QLineEdit2 *skelLengthEdit,*skelWKEdit,*skelWOEdit,*meshAreaEdit,*avgDiEdit;
	QLineEdit2 *meshVolumeEdit;
	QSlider2 *surfaceOpacitySlider;
	QCheckBox2 *setCurrentPropertyBox;

	QPushButton2 *generateClippedSurfaceButton;
	QPushButton2 *segmentBrainButton;
	QPushButton2 *extractMaskedVolumeButton;

	QPushButton2 *generateVisibleMaskButton;

	QSlider2* resize3DPointerScale1;
	QSlider2* resize3DPointerScale2;

	void	SetLayerFilterVisibilityIcons(bool,bool, QTreeWidgetItem *item=NULL);
	void	SetLayerVisibilityIcon(QTreeWidgetItem * , bool );

	void	GenerateDistinctColors();
	double Azimuth,  Elevation,  Roll;
	QPushButton2* showCameraAnglesButton;
public slots:

	void	SlotShowCurrentCameraAngles();
void	SlotShowCurrentPaletteName(int);
void	SlotSetFrameToAnimMissPixelsLabel(int);
void	SlotSetBruteForceSliderLabel(int);
void	SlotSetRangeMatchSlider0(int);
void	SlotSetRangeMatchSlider1(int);


void	SlotSetImgMatchOptSlider(int);

void	SlotRawTreeWidgetItemClicked(QTreeWidgetItem * item, int pos, int mode);

//void		QPushButton2		*mixerMoveUpPushButton,*mixerMoveDownPushButton, *mixerRemovePushButton;
void	SlotSetMixerItemMixerMode(QTreeWidgetItem *item, bool mode);
void	SlotSetMixerBlendOpacityPresentation(QTreeWidgetItem *item, double alpha);
void	SlotSetMixerBlendModePresentation(QTreeWidgetItem *item, int mode);
void	SlotMoveUpMixerItems();
void	SlotMoveDownMixerItems();
void	SlotSwitchChildMode(bool childMode);
void	SlotRemoveMixerItems();
void	SlotInsertNewVolumeMixerItem(RawDataLayer *layer, LinkedList <RawDataLayer *> *rawDataLayerList);

void	SlotInsertRawDataItem(DataSet *dataset, LinkedList <DataSet *> *dataSetList);
void	SlotInsertMaskDataItem(ByteMask *byteMask,int col);

	void	SlotUpdateTreeWidgetState( QTreeWidgetItem * , int  );
	void	SlotInsertNewSurfaceTreeItem(QString name, QString groupName, PolygonSurfaceData *dataXX,int  mode=0);
	void	SlotSetNewTreeItemColor(QTreeWidgetItem *item, double color[4]);
	void	slotDockLocationChanged(Qt::DockWidgetArea,QString name);
	void	SlotSetArteryButtonState(int nr,bool state);
	void	SlotCreateFloatingWidgets();

	void	SlotUpdateLightIntensityLabel(int);
	
	void	SlotUpdateBBoxSizes(int);
	void	SlotUpdateBBoxSizes();
	void	SlotUpdateBBoxSizes(const QString &);


	void	SlotSetNewCameraRotations(double Azimuth, double Elevation,double Roll);


	void	SlotUpdateTolerance(int);
	void	SlotUpdateTolerance();
	void	SlotUpdateTolerancePercent(const QString &);
	void	SlotUpdateToleranceValue(const QString &);
	void	SlotUpdateToleranceValue();


	//void	SlotListRightMouseButtonRMB( QListViewItem* Item, const QPoint & point, int );



	void	SlotShowInfo(int Nx,int Ny,int Nz,double Xscale,double Yscale,double Zscale,const QString &filename );
/////////////////
	
	void	SlotShowPolygonSurfaceDatasetInfo(int Nx,int Ny,int Nz,double Xscale,double Yscale,double Zscale,const QString &filename,int Nr1,int Nr2,double Or1,double Or2,double Or3);

	
	void	GetMarchingCubesSettings(void);
	void	SlotChangeActualSurface(int NR);
	void	SlotSetNewName( const QString &Name);

	void	SlotSetNextPaletteR(void);
	void	SlotSetPrevPaletteR(void);
	void	SlotSetPaletteItemR(int Item);
	
	void	SlotSetNextPalette(void);
	void	SlotSetPrevPalette(void);
	void	SlotSetPaletteItem(int Item);
	void	SlotSetNewOpacity();
	void	SlotSetNewMaskOpacity();
	void	SlotChangeInspectionWindowSize(int);
	void	SlotSetInspectionRanges(int X,int Y,int Z);
	void	SlotSetInspectionRanges(int X,int Y,int Z,int val1,int val2,int val3);


	//=---------------------------------------------



	//void	SlotInsertRawDataItem(const QString &name);
void	SlotDeleteRawDataItem();
	//void	SlotCurrentRawDataItemChanged(QListViewItem *);
	//void	SlotSetNewDataSetByDoubleClick(QListViewItem *);
	//void	SlotSetNewDataSet();
	//void	SlotUpdateItemState();
	//void	SlotSetAttributesToItemsCompatibleWithModel(int *);

	//void	SlotChangeAtributes(int);
	//int		*SlotCreate2DViewsArray();
	//int		*SlotCreateInspectionWindowsArray();
	void	SlotSetNewDataSetByDoubleClick(QTreeWidgetItem *, int);
///////////////////////////////


	void	SlotDeletePolygonSurfaceDataItem();
//	void	SlotCurrentPolygonSurfaceDataItemChanged(QListViewItem * );
	void	SlotChangeCurrentPolygonSurfaceDataItem(int pos);
	void	SlotChangeCurrentPolygonSurfaceDataItem(int pos,bool sel);
	//	void	SlotSetNewPolygonSurfaceDataSet( );
//	void	SlotSetNewPolygonSurfaceDataSetByDoubleClick(QListViewItem *Item );






	//----------------------------------------------

	void	SlotInsertPolygonSurfaceDataItem(const QString &name);
	void	SlotInsertPolygonSurfaceDataItemAtTheEnd(const QString &name);
	void	SlotInsertPolygonSurfaceDataItemAndBack(const QString &name);
	//******************************************************


	/*void	SlotAddFilterToListBox(const QString &Name);
	void	SlotAddFilterToFilteringPipeline();

	void	SlotMoveFilterItemUp();
	void	SlotMoveFilterItemDown();
	void	SlotRemoveFilterItem();
	void	SlotRemoveAllFilterItems();*/

	//void	SlotSelectCurrentAvailableFilter(QListViewItem * );

	//void	SlotEditFilterFromPipeline(QListViewItem * );
	//void	SlotCurrentFiltersPipelineItemChanged(QListViewItem * );



	//********************************************************




	void	SlotUpdateTransformation(int);
	void	SlotUpdateTransformation();
	void	SlotResetTransformation();
	void	SlotUpdateTransformation(const QString &);
	void	SlotScatterPlotButtonPressed();
	void	SlotSurfacesTransformButtonPressed();
	void	SlotVolumeTransformButtonPressed();
	void	SlotGenerateMisregistrationGraphsButtonPressed();
	void	SlotStartRegistrationButtonPressed();
	void	SlotGeneratePolyGraphButtonPressed();

	void	SlotGenerate3DVolumeGraphButtonPressed();
	void	SlotGenerate6DGraphButtonPressed();


	void	SlotUpdateTransformationParameters();
	void	SlotInsertNewTransformation(double*);
	void	SlotInsertNewTransformation2(double*);


	void	SlotFindSelectedDICOMPosition();

	void	SlotSetTransformationModeToAffine();
	void	SlotSetTransformationModeToRigid();
	
	
	void	SlotSetSurfaceOrigin(double *Origin);
	//void	SlotSurfaceListSelectionChanged();
	void	SlotBlockUpdateSelectionList(bool val);

	
	void	SlotSetNewBBoxSizes(double *Sizes);
	void	SlotSetSurfaceOpacityPresentation(QTreeWidgetItem *item, double alpha);
	void	SlotSetSurfaceColorPresentation(QTreeWidgetItem *item, double r,double g, double b, double alpha=1);
	//void	SlotSetSurfaceColorPresentation(QListViewItem *item, double r,double g, double b);
	void	SlotSetSurfaceColorPresentation(int pos, double r,double g, double b);
	void	slotChangeDockWidgetVisibility(QString name);
	void	SlotSurfaceTreeWidgetDoubleClicked( QTreeWidgetItem * item, int column );
	void	SlotParallelSliderValueChanged(int val);

	void	SlotShowTransformWidget();
signals:


	void	SignalSelectedDICOMAtPos(int);
	//--------------------------------------------------


	void	SignalDeleteRawDataItem(QTreeWidgetItem *);
	void	SignalSetCurrentDataSet(int);
	void	SignalSetNewDataSet(QTreeWidgetItem *);


	void	SignalSetNew3DViewDataSet();
	void	SignalSetNewInspectionDataSet();
	void	SignalSetNewObjectDataSet();
	void	SignalSetNew2DViews(int *);
	void	SignalSetNewInspectionWindows(int *);

	//--------------------------------------------------
	void	SignalSetCurrentPolygonSurfaceDataSet(int);
//	void	SignalSetNewPolygonSurfaceDataSet();
	void	SignalDeletePolygonSurfaceDataSet();
	//******************************************************

	void	SignalOpenAddFilterDialog(int AvailablePos,int CurrentInTheList);
	void	SignalOpenAddButtonClicked();
	void	SignalOpenUpdateFilterDialog(int);
	void	SignalRemoveFilterItem();
	void	SignalMoveFilterItemUp();
	void	SignalMoveFilterItemDown();

	void	SignalSetCurrentPipelineFilter(int);


	//******************************************************
	void	SendMarchingCubesSettings(bool _GaussCheck,double _SdvX,double _SdvY,double _SdvZ,bool _DecimateCheck,double _Tred,bool _SmoothingCheck,bool _ConnectCheck);
	void	SendNewActualSurface(int Value);
	void	SetNewPalette(int );

	void	SetNewPaletteR(int );
	void	SendOpacity(int  );
	void	SendMaskOpacity(int  );
	void	SendMaskBufferOpacity(int  );
	void	SignalSendInspectionWindowSize(int,int,int);

	void	SignalGetModelCompatibilityArray();
	void	SignalSendTransformation(double*);
	void	SignalSendTransformation2(double*);
	void	SignalSendTransformation3(double*);
	void	SignalSendTransformation4(double*);
	void	SignalSendTransformation5(double*);
	void	SignalSendTransformation6(double*);
	void	SignalSendTransformation7(double*);
	
	void	SignalSendTransformationForSurfaces(double*);

	////////////////

	void	SignalSetNewModelPolygonSurfaceDataSet();
	void	SignalSetNewObjectPolygonSurfaceDataSet();
	void	SignalHideCurrentPolygonSurface();
	void	SignalShowCurrentPolygonSurface();


	void	SignalSendNewInspectionMode(int);

	//void	SignalSendCurrentSurfaceSelectionList(QListView *ListView);

	void	SignalSendNewBBoxSize(double *Sizes);
};
#endif //   GUI_PANEL_WIDGET 
