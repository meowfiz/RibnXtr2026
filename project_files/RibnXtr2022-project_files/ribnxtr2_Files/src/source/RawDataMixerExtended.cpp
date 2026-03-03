#include "RawDataMixerExtended.h"


RawDataMixerExtended::RawDataMixerExtended(LinkedList<DataSet*>	*_dataSetList, GUIPanelWidget *_guiWidget, int _viewtype, LinkedList <RawDataLayer *> *rawDataLayerList) : RawDataMixerLite(_dataSetList, _viewtype, rawDataLayerList)
{
	guiWidget = _guiWidget;
	rawDataTreeWidget = this->guiWidget->rawDataTreeWidget;
	mixerTreeWidget = this->guiWidget->mixerTreeWidget;

	mixerLayerSourceComboBox = this->guiWidget->mixerLayerSourceComboBox;
	mixerLayerMaskSourceComboBox = this->guiWidget->mixerLayerMaskSourceComboBox;


	autoUpdateCurrentFilterCheckBox = this->guiWidget->autoUpdateCurrentFilterCheckBox;

	connect(this->mixerTreeWidget, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(SlotTreeWidgetItemClicked(QTreeWidgetItem *, int)));
	connect(this->mixerTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(SlotTreeWidgetItemChanged(QTreeWidgetItem *, int)));


	connect(this->guiWidget->mixerRemovePushButton, SIGNAL(clicked()), this, SLOT(SlotRemoveSelectedItems()));
	connect(this->guiWidget->mixerCreateLayerPushButton, SIGNAL(clicked()), this, SLOT(SlotCreateLayer()));
	connect(this->guiWidget->mixerChildModePushButton, SIGNAL(clicked()), this, SLOT(SlotSwitchChildMode()));

	connect(this, SIGNAL(signalUpdateGUIAfterLoadingLayersFromFile()), this, SLOT(SlotUpdateGUIAfterLoadingLayersFromFile()));

	connect(this->guiWidget->rawDataLayerNameLineEdit, SIGNAL(editingFinished()), this, SLOT(SlotChangeLayerName()));

	mixerMaskFilterModeComboBox = this->guiWidget->mixerMaskFilterModeComboBox;
	connect(mixerMaskFilterModeComboBox, SIGNAL(activated(int)), this, SLOT(SlotUpdateSettings(int)));

	mixerFilterModeComboBox = this->guiWidget->mixerFilterModeComboBox;
	connect(this, SIGNAL(signalParamsChanged()), this, SLOT(SlotAutoUpdateCurrentFilter()));
	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (this->guiWidget->mixerFilterParamsWidgets[i] != NULL)
		{
			mixerFilterParamsWidgets[i] = this->guiWidget->mixerFilterParamsWidgets[i];
		}
		else
		{
			mixerFilterParamsWidgets[i] = NULL;
		}
	}

	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (this->guiWidget->mixerMaskFilterParamsWidgets[i] != NULL)
		{
			mixerMaskFilterParamsWidgets[i] = this->guiWidget->mixerMaskFilterParamsWidgets[i];
		}
		else
		{
			mixerMaskFilterParamsWidgets[i] = NULL;
		}
	}



	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (this->guiWidget->mainLayerParamsWidgets[i] != NULL)
		{
			mainLayerParamsWidgets[i] = this->guiWidget->mainLayerParamsWidgets[i];
		}
		else
		{
			mainLayerParamsWidgets[i] = NULL;

		}
	}


	//int mixerOpacityID;//DONE
	//int mixerLayerEnabledID;//DONE
	//int mixerLayerSourceMixedID;//DONE
	//int mixerModesID;//DONE
	//int mixerFilterEnableID;//done
	//int mixerFilterInverseID;//done
	//int mixerMultiplierID;//done
	//int mixerAdditionID;//done
	//int mixerAutoNormalizeID;//done
	//int mixerMaskFilterEnabledID;

	QStringList mainFilterNames;
	mainFilterNames << "Opacity" << "LayerEnable" << "LayerSourceMixed" << "Modes" << "FilterEnable" << "FilterInverse" << "Multiplier" << "Addition" << "AutoNormalize" << "MaskFilterEnable" << "FilterMode" << "BrightnessFilterMode" << "RememberBuffer";


	QStringList list;
	AddParam("Opacity", 1, 1, 0, 1, 0.01, mixerOpacityID);
	list.clear(); list << "false" << "true";
	AddParam("LayerEnable", 0, 1, 0, 1, 1, mixerLayerEnableID, &list);
	AddParam("LayerSourceMixed", 0, 1, 0, 1, 1, mixerLayerSourceMixedID, &list);
	list.clear();
	list.append("normal");
	list.append("average");
	list.append("multiply");
	list.append("screen");
	list.append("darken");
	list.append("lighten");
	list.append("difference");
	list.append("add");
	list.append("overlay");
	list.append("hard light");
	list.append("soft light");
	list.append("color dodge mode");
	list.append("color burn mode");
	list.append("reflect mode");
	list.append("replace a with b except 0");
	list.append("replace b with a except 0");
	AddParam("Modes", 0, 2, 0, list.count() - 1, 1, mixerModesID, &list);
	list.clear(); list << "false" << "true";
	AddParam("FilterEnable", 1, 1, 0, 1, 1, mixerFilterEnableID, &list);
	AddParam("FilterInverse", 0, 1, 0, 1, 1, mixerFilterInverseID, &list);



	AddParam("Multiplier", 1, 1, 0.01, 1000.0, 0.01, mixerMultiplierID);
	AddParam("Addition", 1, 1, -1000, 1000.0, 0.01, mixerAdditionID);
	AddParam("AutoNormalize", 1, 1, 0, 1, 1, mixerAutoNormalizeID, &list);
	AddParam("MaskFilterEnable", 0, 1, 0, 1, 1, mixerMaskFilterEnableID, &list);

	list.clear();
	list.append("normal");
	list.append("highlights");
	list.append("midtones");
	list.append("shadows");
	AddParam("BrightnessMode", 0, 2, 0, 3, 1, brightnessFilterModeID, &list);
	AddParam("RememberBuffer", 0, 1, 0, 1, 1, rememberBufferID);//zero nic nie pamietaj - 1//zapamietaj warstwe w buforze - uwaga - inne moga zamazac!
																/*
																QStringList list;

																AddParam("scaleFactor", 1, true, 0.01, 100, 0.01, scaleFactorID);
																AddParam("thresholdLeft", 0,true,0,100,0.001,thresholdLeftID);
																AddParam("thresholdRight", 100, true, 0, 100, 0.001, thresholdRightID);
																list.clear(); list <<"false" <<"true" ;
																AddParam("inverseThresh", 0, true, 0, 1, 1, inverseThreshID, &list);
																AddParam("fillGapsFromDistance", 0, true, 0, 1, 1, fillGapsFromDistanceID, &list);
																AddParam("use Mask Settings", 0,true,0,1,1,useMaskSettingsID,&list);
																//AddParam("use Prev Layer", 0,true,0,1,1,prevLayerID,&list);

																AddParam("test", 0, true, 0, 1, 1, testID, &list);
																*/




																/*
																mixerModesComboBox->addItem("normal");
																mixerModesComboBox->addItem("average");
																mixerModesComboBox->addItem("multiply");
																mixerModesComboBox->addItem("screen");
																mixerModesComboBox->addItem("darken");
																mixerModesComboBox->addItem("lighten");
																mixerModesComboBox->addItem("difference");
																mixerModesComboBox->addItem("add");
																mixerModesComboBox->addItem("overlay");
																mixerModesComboBox->addItem("hard light");
																mixerModesComboBox->addItem("soft light");
																mixerModesComboBox->addItem("color dodge mode");
																mixerModesComboBox->addItem("color burn mode");
																mixerModesComboBox->addItem("reflect mode");
																mixerModesComboBox->addItem("replace a with b except 0");
																mixerModesComboBox->addItem("replace b with a except 0");*/

	for (int i = 0; i<layerFilterList->GetNumberOfItems(); i++)
	{
		this->mixerFilterModeComboBox->addItem(layerFilterList->GetItemAtPos(i)->GetData()->GetFilterName());
	}

	for (int i = 0; i<layerMaskFilterList->GetNumberOfItems(); i++)
	{
		this->mixerMaskFilterModeComboBox->addItem(layerMaskFilterList->GetItemAtPos(i)->GetData()->GetFilterName());
	}



	this->mixerMaskFilterModeComboBox->setCurrentIndex(0);
	connect(this, SIGNAL(signalUpdateNewVolumeMixerItem()), this, SLOT(SlotUpdateNewVolumeMixerItem()));
	connectAll();
}

//--------------------------

RawDataMixerExtended::~RawDataMixerExtended()
{



}
//----------------------------


//---------------------------
void RawDataMixerExtended::SlotUpdateNewVolumeMixerItem()
{
	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))
		{
			DataSet *mainDataSet = mainListItem->GetData();
			guiWidget->SlotInsertNewVolumeMixerItem(mainLayer, this->rawDataLayerList);
			mainLayer->GetTreeWidgetItem()->setSelected(true);

		}

}
//-----------------------------------

//----------------------------
void RawDataMixerExtended::SlotTreeWidgetItemClicked(QTreeWidgetItem * widget, int column)
{


	if (widget != NULL)
	{
		bool up = false;
		int len = -1;
		int fen = -1;

		if (column == 1)
		{
			bool v = params[mixerLayerEnableID];			params[mixerLayerEnableID] = !v;

			len = v;
			up = true;
			//this->mixerLayerEnableCheckBox->setChecked(!this->mixerLayerEnableCheckBox->isChecked());
		}
		else if (column == 3)
		{
			bool v = params[mixerFilterEnableID];			params[mixerFilterEnableID] = !v;
			fen = v;
			up = true;
			//this->mixerFilterEnableCheckBox->setChecked(!this->mixerFilterEnableCheckBox->isChecked());

		}
		else if (column == 5)
		{


			for (int l = 0; l<rawDataLayerList->GetNumberOfItems(); l++)
			{
				if (this->rawDataLayerList->GetItemAtPos(l)->GetData()->GetTreeWidgetItem() == widget)
				{
					RawDataLayer *layer = rawDataLayerList->GetItemAtPos(l)->GetData();

					//	layer->SetBlendingOpacity(1.0-layer->GetBlendingOpacity());
					//double val = params[mixerOpacityID]; params[mixerOpacityID] = !val;
					double val = layer->GetBlendingOpacity(); val = 1 - val;
					up = true;
					SlotMixerOpacityChanged(val * 255);

					emit signalParamsChanged();
					break;
				}
			}


		}

		for (int l = 0; l < rawDataLayerList->GetNumberOfItems(); l++)
		{
			if (this->rawDataLayerList->GetItemAtPos(l)->GetData()->GetTreeWidgetItem() == widget)
			{
				RawDataLayer *layer = rawDataLayerList->GetItemAtPos(l)->GetData();
				if (len >= 0) layer->SetLayerEnabled(len);
				if (fen >= 0) layer->SetFilterEnabled(fen);

			}
		}
		if (up == true) SlotUpdateSettings();

		SlotItemSelectionChanged();

	}

}
//------------------------------------
void RawDataMixerExtended::SlotTreeWidgetItemChanged(QTreeWidgetItem * widget, int column)
{


	if (widget != NULL)
	{
		if (column == 2)//text
		{
			bool v = params[mixerLayerEnableID];			params[mixerLayerEnableID] = !v;
			//this->mixerLayerEnableCheckBox->setChecked(!this->mixerLayerEnableCheckBox->isChecked());

			for (int l = 0; l<rawDataLayerList->GetNumberOfItems(); l++)
			{
				if (this->rawDataLayerList->GetItemAtPos(l)->GetData()->GetTreeWidgetItem() == widget)
				{
					RawDataLayer *layer = rawDataLayerList->GetItemAtPos(l)->GetData();


					QString changedText = widget->text(2);
					QString layerName = layer->GetName();

					qDebug() << "old: " << layerName << " new: " << changedText;
					if (layerName.compare(changedText) != 0)
					{
						layer->SetName(changedText);
					}
					break;
				}
			}

		}

	}

}
//------------------------------------

//---------------------------------
RawDataLayer *RawDataMixerExtended::GetTopLayerAtPos(int pos)
{
	RawDataLayer *layer = NULL;
	QTreeWidgetItem *tmpItem = mixerTreeWidget->topLevelItem(pos);
	for (int l = 0; l<rawDataLayerList->GetNumberOfItems(); l++)
	{
		if (this->rawDataLayerList->GetItemAtPos(l)->GetData()->GetTreeWidgetItem() == tmpItem)
		{
			layer = rawDataLayerList->GetItemAtPos(l)->GetData();
			break;
		}
	}
	return layer;
}
//------------------------


void RawDataMixerExtended::SlotItemSelectionChanged()
{

	//znajdzie pierwszy zaznaczony i wez jego parametry do opacity i mode
	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			disconnectAll();



			DataSet *d;
			RawDataLayer *layer;
			if (this->guiWidget->mixerTreeWidget->selectedItems().count()>0)
			{
				QTreeWidgetItem *item = this->guiWidget->mixerTreeWidget->selectedItems().at(0);
				for (int i = 0; i<rawDataLayerList->GetNumberOfItems(); i++)
				{
					//d=rawDataLayerList->GetItemAtPos(i)->GetData()->GetDataSet();
					layer = rawDataLayerList->GetItemAtPos(i)->GetData();
					if (item == layer->GetTreeWidgetItem())
					{
						this->guiWidget->rawDataLayerNameLineEdit->setText(layer->GetName());
						int currentLayerSource = layer->GetDataSetNumber();
						mixerLayerSourceComboBox->clear();
						QString namex;

						//qDebug() << "SlotItemSelectionChanged In";
						//Ÿród³a z oryginalow
						//NOWOSC - dodaje zerow¹ jako main data set!!!
						namex = "MainDataSet";//"source";
						mixerLayerSourceComboBox->addItem(guiWidget->distinctColorIconsRect[this->mainDataSet->GetDataCounter()], namex);
						///-----------
						for (int j = 0; j<dataSetList->GetNumberOfItems(); j++)
						{
							namex = dataSetList->GetItemAtPos(j)->GetData()->GetRawData()->GetRawDataSetName();//"source";
							mixerLayerSourceComboBox->addItem(guiWidget->distinctColorIconsRect[dataSetList->GetItemAtPos(j)->GetData()->GetDataCounter()], namex);
						}

						//qDebug() << "SlotItemSelectionChanged AfterDAta";


						//zrodlo warstwy to wynik z poprzednich warstw
						for (int j = 0; j<i; j++)
						{
							namex = rawDataLayerList->GetItemAtPos(j)->GetData()->GetName();//"mixer result";

							mixerLayerSourceComboBox->addItem(guiWidget->distinctColorIconsCirc[rawDataLayerList->GetItemAtPos(j)->GetData()->GetDataCounter()], namex);
						}

						//qDebug() << "SlotItemSelectionChanged AfterLayers";
						mixerLayerMaskSourceComboBox->clear();
						mixerLayerMaskSourceComboBox->addItem(guiWidget->distinctColorIconsCirc[0], "Current Layer");

						for (int j = 0; j<i; j++)
						{
							namex = rawDataLayerList->GetItemAtPos(j)->GetData()->GetName() + " - copy";//"mixer result";

							mixerLayerMaskSourceComboBox->addItem(guiWidget->distinctColorIconsCirc[rawDataLayerList->GetItemAtPos(j)->GetData()->GetDataCounter()], namex);
						}
						int aa = mixerLayerMaskSourceComboBox->count();

						//qDebug() << "SlotItemSelectionChanged AfterMasks";
						mixerLayerMaskSourceComboBox->setCurrentIndex(layer->GetMaskNumber());


						int lPos = 0;//=currentLayerSource;

						if (currentLayerSource == 0)
						{
							lPos = 0;
						}
						else
							if (currentLayerSource>0)
							{
								lPos = currentLayerSource; //checked
							}
							else
							{
								lPos = dataSetList->GetNumberOfItems() + (-currentLayerSource); //checked
							}

						//qDebug() << "SlotItemSelectionChanged 1";
						mixerLayerSourceComboBox->setCurrentIndex(lPos);
						if (i == 0)
						{
							this->guiWidget->mixerParamsWidget->setEnabled(false);
							//break;
						}
						else
						{
							this->guiWidget->mixerParamsWidget->setEnabled(true);
						}




						RawDataSet *rData = NULL;
						if (currentLayerSource == 0)
						{
							rData = this->mainDataSet->GetRawData();
						}
						else if (currentLayerSource>0)
							rData = dataSetList->GetItemAtPos(currentLayerSource - 1)->GetData()->GetRawData();
						else
							rData = rawDataLayerList->GetItemAtPos(-currentLayerSource - 1)->GetData()->getFilterBufferData(RawDataMixerLite::viewType);


						this->mixerMaskFilterModeComboBox->setCurrentIndex(layer->GetMaskFilterMode());
						//trzeba pobrac jeszcze nazwy parametrow z obiektu typu mask filter!!!
						int nrr = layer->GetFilterMode();
						LayerFilter *layerFilter = layerFilterList->GetItemAtPos(nrr)->GetData();
						int nrOfParams = layerFilter->GetNumberOfParams();
						//qDebug() << "SlotItemSelectionChanged 4";


						for (int i = 0; i < nrOfParams; i++)
						{
							if (mainLayerParamsWidgets[i] != NULL)
							{
								mainLayerParamsWidgets[i]->paramsLineEdit->setText(QString::number(params[i]));

							}
						}
						/*	this->mixerLayerSourceMixedCheckBox->setChecked(layer->GetMixedLayer());
						this->mixerFilterEnableCheckBox->setChecked(layer->GetFilterEnabled());
						this->mixerFilterInverseCheckBox->setChecked(layer->GetFilterInverted());
						this->mixerLayerEnableCheckBox->setChecked(layer->GetLayerEnabled());*/
						//filtry
						this->mixerFilterModeComboBox->setCurrentIndex(layer->GetFilterMode());
						for (int i = 0; i < nrOfParams; i++)
						{
							if (mixerFilterParamsWidgets[i] != NULL)
							{
								mixerFilterParamsWidgets[i]->paramsLineEdit->setText(QString::number(layer->GetFilterParam(i)));

							}
						}
						nrr = layer->GetMaskFilterMode();
						LayerMaskFilter *layerMaskFilter = layerMaskFilterList->GetItemAtPos(nrr)->GetData();
						nrOfParams = layerMaskFilter->GetNumberOfParams();
						for (int i = 0; i < nrOfParams; i++)
						{
							if (mixerMaskFilterParamsWidgets[i] != NULL)
							{
								mixerMaskFilterParamsWidgets[i]->paramsLineEdit->setText(QString::number(layer->GetMaskFilterParam(i)));

							}
						}
						//trzeba pobrac jeszcze nazwy parametrow z obiektu typu filter!!!

						QString tt = mixerMaskFilterParamsWidgets[0]->paramsLineEdit->text();
						//update gui filtry masek



						//qDebug() << "SlotItemSelectionChanged 5";

						if (layer->GetLayerMixerMode() == true)
						{
							this->guiWidget->mixerFilterWidget->setEnabled(false);

						}
						else
						{
							this->guiWidget->mixerFilterWidget->setEnabled(true);
						}
						if (i > 0)
						{
							UpdateSliderSettings(layer);
						}
						else
						{
							//wszystko schowam dla warstwy zero bo nie edytowalna

							for (int i = 0; i < PARAMS_NR; i++)
							{
								if (mixerMaskFilterParamsWidgets[i] != NULL)
								{
									mixerMaskFilterParamsWidgets[i]->hide();
								}
							}
							for (int i = 0; i < PARAMS_NR; i++)
							{
								if (mixerFilterParamsWidgets[i] != NULL)
								{
									mixerFilterParamsWidgets[i]->hide();
								}
							}

						}
						//qDebug() << "SlotItemSelectionChanged out";
						break;
					}
				}
			}

			connectAll();


		}



}
//--------------------------------------
void	RawDataMixerExtended::SlotMixerModeChanged(int mode)
{

	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			for (int i = 0; i<rawDataLayerList->GetNumberOfItems(); i++)
			{
				QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem();
				if ((item != NULL) && (item->isSelected() == true))
				{
					//rawDataLayerList->GetItemAtPos(i)->GetData()->SetBlendingMode(mode);
					this->guiWidget->SlotSetMixerBlendModePresentation(item, mode);
					params[ModesID] = mode;

				}
			}
			emit signalParamsChanged();
		}


}
//--------------------------------------
void	RawDataMixerExtended::SlotLayerMaskSourceChanged(int mode)
{

	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{

			int nrOfDatasets = rawDataLayerList->GetNumberOfItems();

			bool chg2 = false;
			int aa = mixerLayerMaskSourceComboBox->count();
			mode = mixerLayerMaskSourceComboBox->currentIndex();

			for (int i = 0; i<rawDataLayerList->GetNumberOfItems(); i++)
			{
				QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem();
				if ((item != NULL) && (item->isSelected() == true))
				{
					rawDataLayerList->GetItemAtPos(i)->GetData()->SetMaskNumber(mode);
					chg2 = true;
				}
			}

			if (chg2 == true) emit signalParamsChanged();
		}


}
//-----------------------------------
void	RawDataMixerExtended::SlotLayerSourceChanged(int mode)
{

	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			bool chg1 = false;



			int nrOfDatasets = dataSetList->GetNumberOfItems();
			mode = mixerLayerSourceComboBox->currentIndex();



			for (int i = 0; i<rawDataLayerList->GetNumberOfItems(); i++)
			{
				QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem();
				if ((item != NULL) && (item->isSelected() == true))
				{

					if (mode == 0)
					{
						//int nr = mainListItemPos;//??
						rawDataLayerList->GetItemAtPos(i)->GetData()->SetDataSetNumber(mode); //checked

						chg1 = true;
					}
					else

					{
						//mode += 1;//zwiekszam bo zmienilem ze lista jest o 1 dluzsza - poczatkowy to maindataset
						int xMode = 0;
						if (mode <= nrOfDatasets)
						{
							xMode = mode; //source is raw data set nr Mode //checked
						}
						else
						{
							//xMode = -((mode - nrOfDatasets) + 1);
							xMode = -(mode - nrOfDatasets); //checked
						}

						rawDataLayerList->GetItemAtPos(i)->GetData()->SetDataSetNumber(xMode);

						chg1 = true;
					}

				}
			}



			if (chg1 == true) emit signalParamsChanged();
		}


}
//------------------------------------
void	RawDataMixerExtended::SlotMixerOpacityChanged(int val)
{

	double opacity = val / 255.0;
	params[mixerOpacityID] = opacity;
	//mixerOpacityLabel->setText(QString::number(opacity));
	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			for (int i = 0; i<rawDataLayerList->GetNumberOfItems(); i++)
			{
				QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem();
				if ((item != NULL) && (item->isSelected() == true))
				{
					//rawDataLayerList->GetItemAtPos(i)->GetData()->SetBlendingOpacity(opacity);

					this->guiWidget->SlotSetMixerBlendOpacityPresentation(item, opacity);
					//void	SlotSetMixerBlendModePresentation(QTreeWidgetItem *item, int mode);

				}
			}
			//	emit signalParamsChanged();
		}
}
//------------------------------------------
void	RawDataMixerExtended::SlotMixerParamsChanged(const QString & text)
{

	// tu nie mam pomyslu
	/// chyba 1 element tablicy to bedzie ich liczba a potem leci
	emit signalParamsChanged();

}
//------------------------------------------
void	RawDataMixerExtended::SlotMixerMultiplierChanged(const QString & text)
{

	//double multipiler = params[mixerMultiplierID];// mixerMultipierLineEdit->text().toDouble();
	//double addition = params[mixerAdditionID];// mixerAdditionLineEdit->text().toDouble();
	//// tu nie mam pomyslu
	///// chyba 1 element tablicy to bedzie ich liczba a potem leci
	//if ((dataSetList!=NULL)&&(dataSetList->GetNumberOfItems()>0)&&(mainListItem!=NULL))
	//	if((rawDataLayerList!=NULL))//&&(dataSetList!=NULL))
	//	{
	//		for(int i=0;i<rawDataLayerList->GetNumberOfItems();i++)
	//		{
	//			QTreeWidgetItem *item=rawDataLayerList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem();
	//			if ((item!=NULL)&&(item->isSelected()==true))
	//			{
	//				rawDataLayerList->GetItemAtPos(i)->GetData()->SetBlendingMultiplier(multipiler);
	//				rawDataLayerList->GetItemAtPos(i)->GetData()->SetBlendingAddition(addition);


	//			}
	//		}
	//		emit signalParamsChanged();
	//	}

}

//----------------------------------------

void	RawDataMixerExtended::SlotSwitchChildMode()
{


	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			disconnectAll();

			DataSet *d;
			RawDataLayer *layer;
			if (this->guiWidget->mixerTreeWidget->selectedItems().count()>0)
			{
				QTreeWidgetItem *item = this->guiWidget->mixerTreeWidget->selectedItems().at(0);

				for (int i = 0; i<this->rawDataLayerList->GetNumberOfItems() - 1; i++)
				{
					if (item == rawDataLayerList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem())
					{


						bool isChild = rawDataLayerList->GetItemAtPos(i)->GetData()->GetLayerChildMode();

						isChild = !isChild;

						rawDataLayerList->GetItemAtPos(i)->GetData()->SetLayerChildMode(isChild);
						guiWidget->SlotSwitchChildMode(isChild);



						QTreeWidgetItem *upItem = NULL;
						QTreeWidgetItem *downItem = NULL;





						//znajde pozycje w toplevelitems dla item
						int k;
						for (k = 0; k<this->rawDataLayerList->GetNumberOfItems() - 1; k++)
						{
							if (mixerTreeWidget->topLevelItem(k) == item) break;
						}



						//wezme pozycje 1 wyzej i nizej z tree widgetu


						if (k>0) upItem = mixerTreeWidget->topLevelItem(k - 1);
						downItem = mixerTreeWidget->topLevelItem(k + 1);




						RawDataLayer *upLayer = NULL;
						RawDataLayer *downLayer = NULL;
						RawDataLayer *currentLayer = NULL;


						for (k = 0; k<this->rawDataLayerList->GetNumberOfItems(); k++)
						{
							if (rawDataLayerList->GetItemAtPos(k)->GetData()->GetTreeWidgetItem() == upItem) upLayer = rawDataLayerList->GetItemAtPos(k)->GetData();
							if (rawDataLayerList->GetItemAtPos(k)->GetData()->GetTreeWidgetItem() == downItem) downLayer = rawDataLayerList->GetItemAtPos(k)->GetData();
							if (rawDataLayerList->GetItemAtPos(k)->GetData()->GetTreeWidgetItem() == item) currentLayer = rawDataLayerList->GetItemAtPos(k)->GetData();
						}





						if ((isChild == true))
						{
							//parent zamienil sie w childa
							if (upItem != NULL)
							{
								if (upLayer->GetLayerChildMode() == false) //nad nim jest parent - zrob mixer
								{
									// dziecko a nad nim rodzic //zapal ikonke mixera wyzej
									this->guiWidget->SlotSetMixerItemMixerMode(upItem, true);
									currentLayer->SetLayerMixerMode(false);
									upLayer->SetLayerMixerMode(true);

								}
							}

						}
						else
						{
							//child zamienil sie w rodzica
							if (upItem != NULL)
							{
								if (upLayer->GetLayerChildMode() == false) // nad nim jest mixer - zrob parent
								{
									this->guiWidget->SlotSetMixerItemMixerMode(upItem, false);
									upLayer->SetLayerMixerMode(false);
								}
							}

							//jesli pod nowym rodzicem jest child - zmien aktualnego w mixer
							if (downLayer->GetLayerChildMode() == true)
							{
								this->guiWidget->SlotSetMixerItemMixerMode(item, true);
								currentLayer->SetLayerMixerMode(true);
							}
						}

					}
				}
			}
			connectAll();
		}







}
//--------------------------------
void	RawDataMixerExtended::SlotRemoveSelectedItems()
{
	QList<QTreeWidgetItem *> selectedItems = mixerTreeWidget->selectedItems();

	if (selectedItems.isEmpty())
		return;


	QMessageBox msgBox;
	QPushButton *okButton = msgBox.addButton(tr("Delete"), QMessageBox::ActionRole);;

	QPushButton *cancelButton = msgBox.addButton(QMessageBox::Discard);
	msgBox.setText("Are you sure you want to remove items?");
	msgBox.exec();



	if (msgBox.clickedButton() == okButton)
	{



		int nrr = rawDataLayerList->GetNumberOfItems();

		for (int i = nrr - 1; i>0; i--)
		{
			QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem();
			if ((item != NULL) && (item->isSelected() == true))
			{
				rawDataLayerList->GetItemAtPos(i)->GetData()->setTmp(1);
			}

		}



		for (int i = nrr - 1; i >= 0; i--)
		{
			QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem();
			if ((item != NULL))//&&(item->isSelected()==true)&&(this->mainListItem->GetData()!=rawDataLayerList->GetItemAtPos(i)->GetData()->GetDataSet()))
			{
				if (rawDataLayerList->GetItemAtPos(i)->GetData()->getTmp() == 1)
					rawDataLayerList->DeleteItem(i);
			}
		}
	}


}


//-------------------------------------------
void	RawDataMixerExtended::SlotCreateLayer()
{

	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			disconnectAll();
			numberOfLayerMods = 0;
			DataSet *d;
			RawDataLayer *layer;
			if (rawDataTreeWidget->selectedItems().count()>0)
			{
				QTreeWidgetItem *item = this->guiWidget->rawDataTreeWidget->selectedItems().at(0);

				int nrr = rawDataTreeWidget->topLevelItemCount();
				//	for(int i=0;i<this->dataSetList->GetNumberOfItems();i++)
				{
					//if (item==dataSetList->GetItemAtPos(i)->GetData()->GetTreeWidgetItem()) //numer na liscie raw data - nie koniecznie odpowiada kolejnosci na tree widgecie
					//if (item == rawDataTreeWidget->topLevelItem(i))
					{
						RawDataLayer *layer = new RawDataLayer(layerMaskFilterList, layerFilterList, nX, nY, nZ, 0);
						layer->SetName("Layer " + QString::number(createdLayerCounter++));


						guiWidget->SlotInsertNewVolumeMixerItem(layer, rawDataLayerList);
						rawDataLayerList->InsertItem(layer);
						guiWidget->SetLayerFilterVisibilityIcons(layer->GetLayerEnabled(), layer->GetFilterEnabled(), layer->GetTreeWidgetItem());





						//	break;
					}
				}
			}
			connectAll();
		}
}

//-----------------------------------------

void	RawDataMixerExtended::SlotUpdateSettings()
{
	disconnectAll();
	fps = 0;
	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			for (int j = 0; j<rawDataLayerList->GetNumberOfItems(); j++)
			{
				QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(j)->GetData()->GetTreeWidgetItem();
				RawDataLayer *layer;



				if ((item != NULL) && (item->isSelected() == true))
				{


					layer = rawDataLayerList->GetItemAtPos(j)->GetData();



					//	layer->SetMaskEnabled(params[mixerMaskFilterEnableID]);// mixerMaskFilterEnableCheckBox->isChecked());
					//	layer->SetAutoNormalize(params[mixerAutoNormalizeID]);// mixerAutoNormalizeDataValuesCheckBox->isChecked());
					//	

					////	layerOptimize=mixerLayerOptimizeCheckBox->isChecked();
					//	//------------------------------------------------------------
					//	//-------------------------------------------------------------
					//	//------------------------------------------------------------
					//	layer->SetMixedLayer(params[mixerLayerSourceMixedID]);// mixerLayerSourceMixedCheckBox->isChecked());
					//	layer->SetFilterEnabled(params[mixerFilterEnableID]);// mixerFilterEnableCheckBox->isChecked());
					//	layer->SetFilterInverted(params[mixerFilterInverseID]);// mixerFilterInverseCheckBox->isChecked());
					//	layer->SetLayerEnabled(params[mixerLayerEnableID]);// mixerLayerEnableCheckBox->isChecked());




					if (layer->GetLayerMixerMode() == true)
					{
						//zrob to samo dla wszystkich dzieci!!!!
						int i, j;
						for (i = 0; i<rawDataLayerList->GetNumberOfItems(); i++)
						{
							if (item == mixerTreeWidget->topLevelItem(i)) break;
						}

						if (i<rawDataLayerList->GetNumberOfItems() - 1)
						{
							QTreeWidgetItem *downItem = NULL;
							RawDataLayer *downLayer;
							bool dChild = false;
							do
							{

								downItem = mixerTreeWidget->topLevelItem(++i);
								for (j = 0; j<rawDataLayerList->GetNumberOfItems(); j++)
								{
									downLayer = rawDataLayerList->GetItemAtPos(j)->GetData();
									if (downItem == downLayer->GetTreeWidgetItem())
									{
										if (downLayer->GetLayerChildMode() == true)
										{
											dChild = true;
											//zmien visibility
											//downLayer->SetLayerEnabled(layer->GetLayerEnabled());
											downLayer->SetMainParam(mixerLayerEnableID, GetMainParam(mixerLayerEnableID));
											// i jeszcze w gui
											guiWidget->SetLayerVisibilityIcon(downItem, GetMainParam(mixerLayerEnableID));
										}

										break;
									}
								}




							} while ((downItem != NULL) && (dChild == true));


						}



					}

					//guiWidget->SetLayerFilterVisibilityIcons(params[mixerLayerEnableID], params[mixerFilterEnableID]);// mixerLayerEnableCheckBox->isChecked(), mixerFilterEnableCheckBox->isChecked());
					guiWidget->SetLayerFilterVisibilityIcons(layer->GetLayerEnabled(), layer->GetFilterEnabled(), layer->GetTreeWidgetItem());
					//mask filter
					int lastMaskFilterMode = layer->GetMaskFilterMode();

					layer->SetMaskFilterMode(this->mixerMaskFilterModeComboBox->currentIndex());
					int currentMaskFilterMode = layer->GetMaskFilterMode();

					//mainWidgets
					//for (int i = 0; i<numberOfParams; i++)
					//{
					//	if (mainLayerParamsWidgets[i] != NULL)
					//	{
					//		if (currentMaskFilterMode != lastMaskFilterMode)
					//		{
					//			//set default parameteres
					//			mainLayerParamsWidgets[i]->paramsLineEdit->setText(QString::number(GetDefaultParam(i)));

					//		}
					//		layer->SetMainParam(i, mainLayerParamsWidgets[i]->paramsLineEdit->text().toDouble());//->setText(QString::number(layer->GetMaskParam(i)));
					//		layer->SetMainParamForGui(i, mainLayerParamsWidgets[i]->paramsCheckBox->isChecked());
					//	}
					//}



					int nrr = layer->GetMaskFilterMode();
					LayerMaskFilter *layerMaskFilter = layerMaskFilterList->GetItemAtPos(nrr)->GetData();
					int nrOfParams = layerMaskFilter->GetNumberOfParams();
					for (int i = 0; i<nrOfParams; i++)
					{
						if (mixerMaskFilterParamsWidgets[i] != NULL)
						{
							if (currentMaskFilterMode != lastMaskFilterMode)
							{
								//set default parameteres
								mixerMaskFilterParamsWidgets[i]->paramsLineEdit->setText(QString::number(layerMaskFilter->GetDefaultParam(i)));

							}
							layer->SetMaskFilterParam(i, mixerMaskFilterParamsWidgets[i]->paramsLineEdit->text().toDouble());//->setText(QString::number(layer->GetMaskParam(i)));
							layer->SetMaskFilterParamForGui(i, mixerMaskFilterParamsWidgets[i]->paramsCheckBox->isChecked());
						}
					}
					//filter
					//********************

					int lastFilterMode = layer->GetFilterMode();
					layer->SetFilterMode(this->mixerFilterModeComboBox->currentIndex());
					int currentFilterMode = layer->GetFilterMode();

					int filterMode = layer->GetFilterMode();
					LayerFilter *layerFilter = layerFilterList->GetItemAtPos(filterMode)->GetData();
					int nrOfFilterParams = layerFilter->GetNumberOfParams();
					for (int i = 0; i<nrOfFilterParams; i++)
					{
						if (mixerFilterParamsWidgets[i] != NULL)
						{
							if (currentFilterMode != lastFilterMode)
							{
								//set default parameteres
								mixerFilterParamsWidgets[i]->paramsLineEdit->setText(QString::number(layerFilter->GetDefaultParam(i)));

							}
							layer->SetFilterParam(i, mixerFilterParamsWidgets[i]->paramsLineEdit->text().toDouble());//->setText(QString::number(layer->GetMaskParam(i)));
							layer->SetFilterParamForGui(i, mixerFilterParamsWidgets[i]->paramsCheckBox->isChecked());
						}
					}
					//************************


					UpdateSliderSettings(layer);




				}
			}
			emit signalParamsChanged();

		}

	connectAll();

}
//--------
void RawDataMixerExtended::SlotAutoUpdateCurrentFilter()
{
	if (autoUpdateCurrentFilterCheckBox->isChecked() == true)
	{
		emit signalUpdateSavedFilted(false);
	}
}
//-----------------------------------------------------
void RawDataMixerExtended::UpdateSliderSettings(RawDataLayer *layer)
{
	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (i<this->numberOfParams)
		{
			bool sel = layer->GetMainParamForGui(i);
			double val = layer->GetMainParam(i);

			mainLayerParamsWidgets[i]->UpdateSliderSettings((LayerGenericFilter*)this, i, sel, val, layer->GetParamsAddress(0, i), layer->GetColorsAddress(0, i)); // ind, paramName, sliderTest, sliderParams, ttext, sel);
			int col[3];
			layer->GetMainParamsColors(i, col);
			mainLayerParamsWidgets[i]->SetBackgroundColor(col[0], col[1], col[2]);



		/*	int col[3], col2[3];
			layer->GetMainParamsColors(i, col);
			mainLayerParamsWidgets[i]->GetBackGroundColor(col2);
			if ((col[0] - col2[0] != 0) || (col[1] - col2[1] != 0) || (col[2] - col2[2] != 0) && (col[0] >= 0))
			{
				layer->SetMainParamsColors(i, col2);
			}
			layer->GetMainParamsColors(i, col);
			mainLayerParamsWidgets[i]->SetBackgroundColor(col[0], col[1], col[2]);*/

		}
		else
		{
			mainLayerParamsWidgets[i]->setVisible(false);
		}
	}

	int filterMode = layer->GetFilterMode();
	LayerFilter *layerFilter = layerFilterList->GetItemAtPos(filterMode)->GetData();
	int nrOfFilterParams = layerFilter->GetNumberOfParams();

	//double sliderParams[3];
	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (i<nrOfFilterParams)
		{
			bool sel = layer->GetFilterParamForGui(i);
			double val = layer->GetFilterParam(i);

			mixerFilterParamsWidgets[i]->UpdateSliderSettings((LayerGenericFilter*)layerFilter, i, sel, val, layer->GetParamsAddress(1, i), layer->GetColorsAddress(1, i));// ind, paramName, sliderTest, sliderParams, ttext, sel);
			int col[3];
			layer->GetFilterParamsColors(i, col);
			mixerFilterParamsWidgets[i]->SetBackgroundColor(col[0], col[1], col[2]);


			/*int col[3], col2[3];
			layer->GetFilterParamsColors(i, col);
			mixerFilterParamsWidgets[i]->GetBackGroundColor(col2);
			if ((col[0] - col2[0] != 0) || (col[1] - col2[1] != 0) || (col[2] - col2[2] != 0) && (col[0] >= 0))
			{
				layer->SetFilterParamsColors(i, col2);
			}
			layer->GetFilterParamsColors(i, col);
			mixerFilterParamsWidgets[i]->SetBackgroundColor(col[0], col[1], col[2]);*/

		}
		else
		{
			mixerFilterParamsWidgets[i]->setVisible(false);
		}
	}

	//--------------------------------------------------------
	int MaskFilterMode = layer->GetMaskFilterMode();
	LayerMaskFilter *layerMaskFilter = layerMaskFilterList->GetItemAtPos(MaskFilterMode)->GetData();
	int nrOfMaskFilterParams = layerMaskFilter->GetNumberOfParams();

	//double sliderParams[3];
	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (i<nrOfMaskFilterParams)
		{
			bool sel = layer->GetMaskFilterParamForGui(i);
			double val = layer->GetMaskFilterParam(i);

			mixerMaskFilterParamsWidgets[i]->UpdateSliderSettings((LayerGenericFilter*)layerMaskFilter, i, sel, val, layer->GetParamsAddress(2, i), layer->GetColorsAddress(2, i)); // ind, paramName, sliderTest, sliderParams, ttext, sel);
			int col[3];
			layer->GetFilterParamsColors(i, col);
			mixerMaskFilterParamsWidgets[i]->SetBackgroundColor(col[0], col[1], col[2]);


		/*	int col[3], col2[3];
			layer->GetMaskFilterParamsColors(i, col);
			mixerMaskFilterParamsWidgets[i]->GetBackGroundColor(col2);
			if ((col[0] - col2[0] != 0) || (col[1] - col2[1] != 0) || (col[2] - col2[2] != 0) && (col[0] >= 0))
			{
				layer->GetMaskFilterParamsColors(i, col2);
			}
			layer->GetMaskFilterParamsColors(i, col);
			mixerMaskFilterParamsWidgets[i]->SetBackgroundColor(col[0], col[1], col[2]);*/
		}
		else
		{
			mixerMaskFilterParamsWidgets[i]->setVisible(false);
		}
	}

}
//--------------------------------------------------
void	RawDataMixerExtended::SlotFastFiltersSettingsChanged(int)
{
	//int vals[4];
	//for(int i=0;i<4;i++) vals[i]=mixerFilterParamsSliders[i]->value();
	disconnectAll();
	fps = 0;


	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			for (int j = 0; j<rawDataLayerList->GetNumberOfItems(); j++)
			{
				QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(j)->GetData()->GetTreeWidgetItem();
				RawDataLayer *layer;
				if ((item != NULL) && (item->isSelected() == true))
				{


					layer = rawDataLayerList->GetItemAtPos(j)->GetData();

					// MAIN
					for (int i = 0; i<numberOfParams; i++)
					{
						bool v = mainLayerParamsWidgets[i]->paramsCheckBox->isChecked();
						layer->SetMainParamForGui(i, v);
					}

					//FILTER

					int filterMode = layer->GetFilterMode();
					LayerFilter *layerFilter = layerFilterList->GetItemAtPos(filterMode)->GetData();
					int nrOfFilterParams = layerFilter->GetNumberOfParams();


					for (int i = 0; i<nrOfFilterParams; i++)
					{
						bool v = mixerFilterParamsWidgets[i]->paramsCheckBox->isChecked();
						layer->SetFilterParamForGui(i, v);

					}


					// MASK FILTER


					int filterMaskMode = layer->GetMaskFilterMode();
					LayerMaskFilter *layerMaskFilter = layerMaskFilterList->GetItemAtPos(filterMaskMode)->GetData();
					int nrOfMaskFilterParams = layerMaskFilter->GetNumberOfParams();

					double sliderParams[3];
					for (int i = 0; i<nrOfMaskFilterParams; i++)
					{
						bool v = mixerMaskFilterParamsWidgets[i]->paramsCheckBox->isChecked();
						layer->SetMaskFilterParamForGui(i, v);

					}


				}
			}
		}



	emit signalParamsChanged();
	connectAll();
}
//-------------
void	RawDataMixerExtended::SlotFilterParamsSliderMoved(int)
{

	//int vals[4];
	//for(int i=0;i<4;i++) vals[i]=mixerFilterParamsSliders[i]->value();
	disconnectAll();
	fps = 0;


	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			for (int j = 0; j<rawDataLayerList->GetNumberOfItems(); j++)
			{
				QTreeWidgetItem *item = rawDataLayerList->GetItemAtPos(j)->GetData()->GetTreeWidgetItem();
				RawDataLayer *layer;
				if ((item != NULL) && (item->isSelected() == true))
				{


					layer = rawDataLayerList->GetItemAtPos(j)->GetData();

					// MAIN
					for (int i = 0; i<numberOfParams; i++)
					{
						double v = mainLayerParamsWidgets[i]->SliderSettingsChanged((LayerGenericFilter*)this, i);
						layer->SetMainParam(i, v);


						int col[3], col2[3];
						layer->GetMainParamsColors(i, col);
						mainLayerParamsWidgets[i]->GetBackGroundColor(col2);
						if ((col[0] - col2[0] != 0) || (col[1] - col2[1] != 0) || (col[2] - col2[2] != 0))
						{
							layer->SetMainParamsColors(i, col2);
						}
					}

					//FILTER

					int filterMode = layer->GetFilterMode();
					LayerFilter *layerFilter = layerFilterList->GetItemAtPos(filterMode)->GetData();
					int nrOfFilterParams = layerFilter->GetNumberOfParams();


					for (int i = 0; i<nrOfFilterParams; i++)
					{
						double v = mixerFilterParamsWidgets[i]->SliderSettingsChanged((LayerGenericFilter*)layerFilter, i);
						layer->SetFilterParam(i, v);

						int col[3], col2[3];
						layer->GetFilterParamsColors(i, col);
						mixerFilterParamsWidgets[i]->GetBackGroundColor(col2);
						if ((col[0] - col2[0] != 0) || (col[1] - col2[1] != 0) || (col[2] - col2[2] != 0))
						{
							if (col2[0] > 255)
							{

							}
							layer->SetFilterParamsColors(i, col2);
						}

					}


					// MASK FILTER


					int filterMaskMode = layer->GetMaskFilterMode();
					LayerMaskFilter *layerMaskFilter = layerMaskFilterList->GetItemAtPos(filterMaskMode)->GetData();
					int nrOfMaskFilterParams = layerMaskFilter->GetNumberOfParams();

					double sliderParams[3];
					for (int i = 0; i<nrOfMaskFilterParams; i++)
					{
						double v = mixerMaskFilterParamsWidgets[i]->SliderSettingsChanged((LayerGenericFilter*)layerMaskFilter, i);
						layer->SetMaskFilterParam(i, v);

						int col[3], col2[3];
						layer->GetMaskFilterParamsColors(i, col);
						mixerMaskFilterParamsWidgets[i]->GetBackGroundColor(col2);
						if ((col[0] - col2[0] != 0) || (col[1] - col2[1] != 0) || (col[2] - col2[2] != 0))
						{
							layer->SetMaskFilterParamsColors(i, col2);
						}


					}


				}
			}
		}



	emit signalParamsChanged();
	connectAll();

}
//----------------------------------
void	RawDataMixerExtended::SlotUpdateSettings(int)
{
	SlotUpdateSettings();
}

//----------------------------------
void	RawDataMixerExtended::SlotUpdateSettings(const QString & text)
{
	//	SlotUpdateSettings();
	SlotFilterParamsSliderMoved(0);
}

//----------------------------------
//--------------------------------

//------------------------
void	RawDataMixerExtended::connectAll()
{
	//AutoUpdateCurrentFilter();
	connect(mixerLayerSourceComboBox, SIGNAL(activated(int)), this, SLOT(SlotLayerSourceChanged(int)));
	connect(mixerLayerMaskSourceComboBox, SIGNAL(activated(int)), this, SLOT(SlotLayerMaskSourceChanged(int)));

	//connect(mainLayerParamsWidgets[mixerModesID], SIGNAL(valueChanged(int)), this, SLOT(SlotMixerModeChanged(int)));
	//connect(mixerModesComboBox,SIGNAL( activated (int)),this,SLOT(SlotMixerModeChanged(int)));
//	connect(mainLayerParamsWidgets[mixerOpacityID], SIGNAL(valueChanged(int)), this, SLOT(SlotMixerOpacityChanged(int)));
	//connect(mixerOpacitySlider,SIGNAL(valueChanged(int)),this,SLOT(SlotMixerOpacityChanged(int)));
	//connect(mixerParamsLineEdit,SIGNAL( textChanged ( const QString &  )),this,SLOT(SlotMixerParamsChanged(const QString &  )));
	connect(mixerTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(SlotItemSelectionChanged()));
	/*connect(mixerMultipierLineEdit,SIGNAL( textChanged ( const QString &  )),this,SLOT(SlotMixerMultiplierChanged(const QString &  )));
	connect(mixerAdditionLineEdit,SIGNAL( textChanged ( const QString &  )),this,SLOT(SlotMixerMultiplierChanged(const QString &  )));*/

	connect(mixerMaskFilterModeComboBox, SIGNAL(activated(int)), this, SLOT(SlotUpdateMaskMode(int)));


	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (mainLayerParamsWidgets[i] != NULL)
		{
			connect(mainLayerParamsWidgets[i], SIGNAL(textChanged(const QString &)), this, SLOT(SlotUpdateSettings(const QString &)));
			connect(mainLayerParamsWidgets[i], SIGNAL(valueChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
			connect(mainLayerParamsWidgets[i], SIGNAL(stateChanged(int)), this, SLOT(SlotFastFiltersSettingsChanged(int)));
			connect(mainLayerParamsWidgets[i], SIGNAL(backgroundColorChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
		}
	}
	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (mixerMaskFilterParamsWidgets[i] != NULL)
		{
			connect(mixerMaskFilterParamsWidgets[i], SIGNAL(textChanged(const QString &)), this, SLOT(SlotUpdateSettings(const QString &)));
			connect(mixerMaskFilterParamsWidgets[i], SIGNAL(valueChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
			connect(mixerMaskFilterParamsWidgets[i], SIGNAL(stateChanged(int)), this, SLOT(SlotFastFiltersSettingsChanged(int)));
			connect(mixerMaskFilterParamsWidgets[i], SIGNAL(backgroundColorChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));


		}
	}
	//connect(mixerMaskFilterEnableCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 
	////connect(mixerLayerOptimizeCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 

	//connect(mixerAutoNormalizeDataValuesCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 



	//for(int i=0;i<10;i++)	if (mixerFilterParamsLineEdits[i]!=NULL)	connect(mixerFilterParamsLineEdits[i],SIGNAL(textChanged ( const QString &  )),this,SLOT(SlotUpdateSettings ( const QString &  )));	
	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (mixerFilterParamsWidgets[i] != NULL)
		{
			/*connect(mixerFilterParamsLineEdits[i],SIGNAL(textChanged ( const QString &  )),this,SLOT(SlotUpdateSettings ( const QString &  )));
			connect(mixerFilterParamsSliders[i],SIGNAL(valueChanged ( int  )),this,SLOT(SlotFilterParamsSliderMoved ( int )));
			connect(mixerFilterParamsCheckBoxes[i], SIGNAL(stateChanged(int)), this, SLOT(SlotUpdateSettings(int)));*/
			connect(mixerFilterParamsWidgets[i], SIGNAL(textChanged(const QString &)), this, SLOT(SlotUpdateSettings(const QString &)));
			connect(mixerFilterParamsWidgets[i], SIGNAL(valueChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
			connect(mixerFilterParamsWidgets[i], SIGNAL(stateChanged(int)), this, SLOT(SlotFastFiltersSettingsChanged(int)));
			connect(mixerFilterParamsWidgets[i], SIGNAL(backgroundColorChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));

		}


	}
	/*connect(mixerLayerSourceMixedCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int)));
	connect(mixerFilterEnableCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int)));
	connect(mixerFilterInverseCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int)));
	connect(mixerLayerEnableCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); */

	connect(mixerFilterModeComboBox, SIGNAL(activated(int)), this, SLOT(SlotUpdateFilterMode(int)));

	//emit signalParamsChanged();
}
//-----------------------------
void	RawDataMixerExtended::disconnectAll()
{
	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (mainLayerParamsWidgets[i] != NULL)
		{
			disconnect(mainLayerParamsWidgets[i], SIGNAL(textChanged(const QString &)), this, SLOT(SlotUpdateSettings(const QString &)));
			disconnect(mainLayerParamsWidgets[i], SIGNAL(valueChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
			disconnect(mainLayerParamsWidgets[i], SIGNAL(stateChanged(int)), this, SLOT(SlotFastFiltersSettingsChanged(int)));
			disconnect(mainLayerParamsWidgets[i], SIGNAL(backgroundColorChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
		}
	}

	//disconnect(mainLayerParamsWidgets[mixerModesID], SIGNAL(valueChanged(int)), this, SLOT(SlotMixerModeChanged(int)));
	//disconnect(mixerModesComboBox,SIGNAL( activated (int)),this,SLOT(SlotMixerModeChanged(int)));
	disconnect(mixerLayerSourceComboBox, SIGNAL(activated(int)), this, SLOT(SlotLayerSourceChanged(int)));
	disconnect(mixerLayerMaskSourceComboBox, SIGNAL(activated(int)), this, SLOT(SlotLayerMaskSourceChanged(int)));
	//disconnect(mainLayerParamsWidgets[mixerOpacityID], SIGNAL(valueChanged(int)), this, SLOT(SlotMixerOpacityChanged(int)));
	//disconnect(mixerOpacitySlider,SIGNAL(valueChanged(int)),this,SLOT(SlotMixerOpacityChanged(int)));
	//disconnect(mixerParamsLineEdit,SIGNAL( textChanged ( const QString &  )),this,SLOT(SlotMixerParamsChanged(const QString &  )));
	disconnect(mixerTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(SlotItemSelectionChanged()));
	//disconnect(mixerMultipierLineEdit,SIGNAL( textChanged ( const QString &  )),this,SLOT(SlotMixerMultiplierChanged(const QString &  )));
	//disconnect(mixerAdditionLineEdit,SIGNAL( textChanged ( const QString &  )),this,SLOT(SlotMixerMultiplierChanged(const QString &  )));

	disconnect(mixerMaskFilterModeComboBox, SIGNAL(activated(int)), this, SLOT(SlotUpdateMaskMode(int)));



	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (mixerMaskFilterParamsWidgets[i] != NULL)
		{
			/*disconnect(mixerMaskFilterParamsLineEdits[i],SIGNAL(textChanged ( const QString &  )),this,SLOT(SlotUpdateSettings ( const QString &  )));

			disconnect(mixerMaskFilterParamsSliders[i],SIGNAL(valueChanged ( int  )),this,SLOT(SlotFilterParamsSliderMoved ( int )));
			disconnect(mixerMaskFilterParamsCheckBoxes[i], SIGNAL(stateChanged(int)), this, SLOT(SlotUpdateSettings(int)));*/
			disconnect(mixerMaskFilterParamsWidgets[i], SIGNAL(textChanged(const QString &)), this, SLOT(SlotUpdateSettings(const QString &)));
			disconnect(mixerMaskFilterParamsWidgets[i], SIGNAL(valueChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
			disconnect(mixerMaskFilterParamsWidgets[i], SIGNAL(stateChanged(int)), this, SLOT(SlotFastFiltersSettingsChanged(int)));
			disconnect(mixerMaskFilterParamsWidgets[i], SIGNAL(backgroundColorChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
		}

	}
	//disconnect(mixerMaskFilterEnableCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 
	//disconnect(mixerLayerOptimizeCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 
	//disconnect(mixerAutoNormalizeDataValuesCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 


	for (int i = 0; i<PARAMS_NR; i++)
	{
		if (mixerFilterParamsWidgets[i] != NULL)
		{
			disconnect(mixerFilterParamsWidgets[i], SIGNAL(textChanged(const QString &)), this, SLOT(SlotUpdateSettings(const QString &)));
			disconnect(mixerFilterParamsWidgets[i], SIGNAL(valueChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
			disconnect(mixerFilterParamsWidgets[i], SIGNAL(stateChanged(int)), this, SLOT(SlotFastFiltersSettingsChanged(int)));
			disconnect(mixerFilterParamsWidgets[i], SIGNAL(backgroundColorChanged(int)), this, SLOT(SlotFilterParamsSliderMoved(int)));
		}

	}

	//disconnect(mixerLayerEnableCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 
	//disconnect(mixerFilterInverseCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 
	//disconnect(mixerLayerSourceMixedCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 
	//disconnect(mixerFilterEnableCheckBox,SIGNAL(stateChanged(int)),this,SLOT(SlotUpdateSettings(int))); 
	disconnect(mixerFilterModeComboBox, SIGNAL(activated(int)), this, SLOT(SlotUpdateFilterMode(int)));


}
//-------------------------

void		RawDataMixerExtended::SlotUpdateFilterMode(int)
{

	SlotUpdateSettings();
}
//-----------------------------
void		RawDataMixerExtended::SlotUpdateMaskMode(int)
{


	SlotUpdateSettings();

}
//------------------------------------


//------------------------------------------------------
void	RawDataMixerExtended::SlotUpdateGUIAfterLoadingLayersFromFile()
{
	//wyczysc cale gui z listy
	//dodaj do gui dane
	int nrLayers = rawDataLayerList->GetNumberOfItems();
	int nrDataSet = this->rawDataTreeWidget->topLevelItemCount();
	for (int i = 0; i<nrLayers; i++)
	{
		RawDataLayer *layer = rawDataLayerList->GetItemAtPos(i)->GetData();

		int nrr = layer->GetDataSetNumber();

		//if (nrr>0) 
		//{
		//	//if (nrr>nrLayers) layer->SetDataSetNumber(1);
		//	if (nrr>nrDataSet) layer->SetDataSetNumber(mainListItemPos);//jak za duzo bierz aktualn¹ warstwê main
		//}
		//else if (nrr<0)
		//{
		//	if (nrr<-nrLayers) layer->SetDataSetNumber(mainListItemPos);
		//}

		//checked

		nrr = layer->GetMaskNumber();
		if (nrr>nrLayers) layer->SetMaskNumber(0);


		guiWidget->SlotInsertNewVolumeMixerItem(layer, rawDataLayerList);
		guiWidget->SetLayerFilterVisibilityIcons(layer->GetLayerEnabled(), layer->GetFilterEnabled(), layer->GetTreeWidgetItem());
	}


	//	if (this->guiWidget->mixerTreeWidget->selectedItems().count()>0)

	//	QTreeWidgetItem *item=this->guiWidget->mixerTreeWidget->selectedItems().at(0);


	for (int i = 0; i<rawDataLayerList->GetNumberOfItems(); i++)
	{
		//int nr=rawDataLayerList->GetNumberOfItems()-i-1;

		RawDataLayer *layer = rawDataLayerList->GetItemAtPos(i)->GetData();
		if (layer->GetLayerChildMode() == true)
		{
			guiWidget->mixerTreeWidget->clearSelection();
			layer->GetTreeWidgetItem()->setSelected(true);
			layer->SetLayerChildMode(false);
			SlotSwitchChildMode();
		}

	}
	guiWidget->mixerTreeWidget->clearSelection();
	rawDataLayerList->GetItemAtPos(nrLayers - 1)->GetData()->GetTreeWidgetItem()->setSelected(true);
}
//-----------------------------------
void	RawDataMixerExtended::SlotChangeLayerName()
{

	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems() > 0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))//&&(dataSetList!=NULL))
		{
			disconnectAll();



			DataSet *d;
			RawDataLayer *layer;
			if (this->guiWidget->mixerTreeWidget->selectedItems().count() > 0)
			{
				QTreeWidgetItem *item = this->guiWidget->mixerTreeWidget->selectedItems().at(0);
				for (int i = 0; i < rawDataLayerList->GetNumberOfItems(); i++)
				{
					//d=rawDataLayerList->GetItemAtPos(i)->GetData()->GetDataSet();
					layer = rawDataLayerList->GetItemAtPos(i)->GetData();
					if (item == layer->GetTreeWidgetItem())
					{
						QTreeWidgetItem * widget = layer->GetTreeWidgetItem();
						QString txt = this->guiWidget->rawDataLayerNameLineEdit->text();
						widget->setText(2, txt);
						layer->SetName(txt);
						
						break;
					}
				}
			}
		}
}
