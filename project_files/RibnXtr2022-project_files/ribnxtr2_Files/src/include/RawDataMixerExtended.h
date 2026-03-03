#ifndef	RAWDATAMIXEREXTENDED_H
#define	RAWDATAMIXEREXTENDED_H

#include <qwidget.h>

#include <math.h>
#include <stdlib.h>

#include "RawDataSet.h"
#include "OtherCommonFunctions.h"
#include "DataSet.h"
#include "GUIPanelWidget.h"
#include "RawDataLayer.h"
#include "LayerMaskFilters.h"
#include "LayerFilters.h"
#include "LayerGenericFilter.h"


#include "ImageShapes.h"
#include "RawDataMixerLite.h"


//---------------------------------
class RawDataMixerExtended : public RawDataMixerLite, public LayerGenericFilter
{
	Q_OBJECT

private:
	RawDataLayer *selectedLayer;
	double params[PARAMS_NR];
	void	SetMainParam(int nr, double val) { params[nr] = val; };
	double	GetMainParam(int nr) { return params[nr]; };
	double *GetMainParams() { return params; };

	QTreeWidget *rawDataTreeWidget;

	GUIPanelWidget *guiWidget;
	QTreeWidget		*mixerTreeWidget;
	QComboBox		*mixerLayerMaskSourceComboBox;
	QComboBox		*mixerLayerSourceComboBox;
	//QLineEdit *rawDataLayerNameLineEdit;
	//	QCheckBox		*mixerLayerSourceMixedCheckBox;
	//QComboBox		*mixerModesComboBox;
	//QSlider			*mixerOpacitySlider;
	//QLineEdit		*mixerParamsLineEdit;
	//QLineEdit		*mixerMultipierLineEdit;
	//QLineEdit		*mixerAdditionLineEdit;

	//QLabel			*mixerOpacityLabel;
	//	QCheckBox		*enableFastFilterCheckBox;

	//	QCheckBox		*mixerMaskFilterEnableCheckBox;// , *mixerLayerOptimizeCheckBox;
	/*	QSlider			*mixerMaskThresholdSlider[2];
	QLineEdit		*mixerMaskParamsLineEdits[10];
	QLabel			*mixerMaskParamsLabels[10];
	QLabel			*mixerThresholdValueLabel[2];*/
	/*QCheckBox		*mixerMaskFilterParamsCheckBoxes[PARAMS_NR];
	QLineEdit		*mixerMaskFilterParamsLineEdits[PARAMS_NR];
	QLabel			*mixerMaskFilterParamsNameLabels[PARAMS_NR];
	QLabel			*mixerMaskFilterParamsLabels[PARAMS_NR];
	QSlider			*mixerMaskFilterParamsSliders[PARAMS_NR];
	QWidget			*mixerMaskFilterParamsWidgets[PARAMS_NR];*/

	//QCheckBox		*mixerFilterInverseCheckBox;
	//QCheckBox		*mixerFilterEnableCheckBox;
	//	QComboBox		*mixerFilterModeComboBox;
	/*QCheckBox		*mixerFilterParamsCheckBoxes[PARAMS_NR];
	QLineEdit		*mixerFilterParamsLineEdits[PARAMS_NR];
	QLabel			*mixerFilterParamsNameLabels[PARAMS_NR];
	QLabel			*mixerFilterParamsLabels[PARAMS_NR];
	QSlider			*mixerFilterParamsSliders[PARAMS_NR];
	QWidget			*mixerFilterParamsWidgets[PARAMS_NR];*/

	int mixerOpacityID;
	int mixerLayerEnableID;
	int mixerLayerSourceMixedID;
	int mixerModesID;
	int mixerFilterEnableID;
	int mixerFilterInverseID;
	int mixerFilterModeID;
	int mixerMultiplierID;
	int mixerAdditionID;
	int mixerAutoNormalizeID;
	int mixerMaskFilterEnableID;
	int brightnessFilterModeID;
	int rememberBufferID;
	//za³o¿enie takie, ¿e tylko Ÿróde³ warstw i typów filtrów nie dajemy do edytowania dla fast fitlers


	ParamsWidget	*mainLayerParamsWidgets[PARAMS_NR];
	QComboBox		*mixerMaskFilterModeComboBox;
	QComboBox		*mixerFilterModeComboBox;
	ParamsWidget	*mixerFilterParamsWidgets[PARAMS_NR];
	ParamsWidget	*mixerMaskFilterParamsWidgets[PARAMS_NR];
	//QCheckBox		*mixerAutoNormalizeDataValuesCheckBox;
	//QCheckBox		*mixerLayerEnableCheckBox;
	QCheckBox		*autoUpdateCurrentFilterCheckBox;
	int				 sliderValues[PARAMS_NR];
	RawDataLayer *GetTopLayerAtPos(int pos);


	void	UpdateSliderSettings(RawDataLayer *layer);




public:

	RawDataMixerExtended(LinkedList<DataSet*>	*_dataSetList, GUIPanelWidget *_guiWidget, int _viewtype = 0, LinkedList <RawDataLayer *> *rawDataLayerList = NULL);
	~RawDataMixerExtended();

	void	connectAll();
	void	disconnectAll();


	public slots:
	void			SlotChangeLayerName();
	void			SlotAutoUpdateCurrentFilter();
	void			SlotUpdateNewVolumeMixerItem();
	void			SlotUpdateGUIAfterLoadingLayersFromFile();
	void			SlotTreeWidgetItemChanged(QTreeWidgetItem * item, int column);
	void			SlotTreeWidgetItemClicked(QTreeWidgetItem *, int);
	void			SlotFilterParamsSliderMoved(int);
	void			SlotFastFiltersSettingsChanged(int);
	void			SlotMixerMultiplierChanged(const QString & text);
	void			SlotCreateLayer();
	//void			SlotCreateMixedLayer();


	void			SlotItemSelectionChanged();

	void			SlotUpdateSettings();
	void			SlotUpdateSettings(int);
	void			SlotUpdateSettings(const QString & text);

	void			SlotLayerSourceChanged(int);
	void			SlotLayerMaskSourceChanged(int);
	void			SlotMixerModeChanged(int);
	void			SlotMixerOpacityChanged(int);
	void			SlotMixerParamsChanged(const QString & text);

	void			SlotSwitchChildMode();
	void			SlotRemoveSelectedItems();

	void			SlotUpdateFilterMode(int);
	void			SlotUpdateMaskMode(int);


signals:


	void			signalUpdateSavedFilted(bool);

};

#endif //RAWDATAMIXEREXTENDED_H
