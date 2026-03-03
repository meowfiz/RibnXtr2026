#include "RawDataMixerLite.h"



RawDataMixerLite::RawDataMixerLite(LinkedList<DataSet*>	*_dataSetList, int _viewType, LinkedList <RawDataLayer *> *_rawDataLayerList)
{


	tm = NULL;
	timeStamp = 0;
	uCharTab = NULL;
	uShortTab = NULL;
	doubleTab = NULL;

	uCharTabSize = 0;
	uShortTabSize = 0;
	doubleTabSize = 0;

	viewType = _viewType;
	shapes = NULL;
	uShortTmpArray = NULL;
	uShortInputMode = true;
	layerFilterList = new LinkedList <LayerFilter*>;
	layerMaskFilterList = new LinkedList <LayerMaskFilter*>;

	initLayerMaskFilters(layerMaskFilterList);
	initLayerFilters(layerFilterList);


	for (int i = 0; i<layerFilterList->GetNumberOfItems(); i++)
	{
		LayerFilter *filter = layerFilterList->GetItemAtPos(i)->GetData();
		QObject::connect(filter, SIGNAL(SendMessage(const QString &)), this, SIGNAL(SendMessage(const QString &)));
	}

	for (int i = 0; i<layerMaskFilterList->GetNumberOfItems(); i++)
	{
		LayerMaskFilter *filter = layerMaskFilterList->GetItemAtPos(i)->GetData();
		QObject::connect(filter, SIGNAL(SendMessage(const QString &)), this, SIGNAL(SendMessage(const QString &)));
	}




	createdLayerCounter = 0;
	mainListItemPos = 0;

	//lastModifiedLayerID=-1;
	globalLeftThreshold = 0;
	globalRightThreshold = 0;
	mmask2 = NULL;
	mainBigMask2 = NULL;
	mmask = NULL;
	mainBigMask = NULL;
	dataToFilter = NULL;
	startOptID = 0;
	lastModifiedLayerItem = NULL;
	minLayerMods = 1;
	sliceNrOpt = 0;
	numberOfLayerMods = 0;
	childBufferOpt = NULL;
	dataBufferOpt = NULL;



	position[0] = 0;
	position[1] = 0;
	uShortTmpArray = NULL;

	for (int i = 0; i<3; i++)
		uShortBuffers[i] = NULL;
	//uShortBuffers[1] = NULL;
	uCharBuffers[0] = NULL;
	uCharBuffers[1] = NULL;
	intBuffers[0] = NULL;
	intBuffers[1] = NULL;


	if (_rawDataLayerList == NULL)
	{
		externalLayerList = false;
		rawDataLayerList = new LinkedList <RawDataLayer *>;
	}
	else
	{
		rawDataLayerList = _rawDataLayerList;
		externalLayerList = true;
	}

	dataSetList = _dataSetList;

	mainListItem = NULL;
	somethingToMix = false;
	dataView = NULL; //transversal


	childBuffer = NULL;

	dataView = NULL;
	dataViewTmp = NULL;


	mainDataSet = NULL;
	inverseOpacity = false;
	filterModeChanged = false;
	maskModeChanged = false;


	//if (funSize != max)
	int max = 65536;
	{
		/*	if (higFunc != NULL) delete[]shaFunc;
		if (shaFunc != NULL) delete[]higFunc;
		if (midFunc != NULL) delete[]midFunc;*/

		higFunc = new double[max];
		shaFunc = new double[max];
		midFunc = new double[max];
		funSize = max;

		//teraz trzeba wypelnic odpowiednimi wagami wszystkie tablice


		double scale = (max>0) ? 1.0 / max : 1;
		double x = 0;
		for (int i = 0; i < max; i++)
		{
			x += scale;
			midFunc[i] = (1 - (4.0*((x - 0.5)*(x - 0.5))));
			higFunc[i] = x*x;
			shaFunc[i] = (1 - x) *(1 - x);
		}



	}


	int pos = 0;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction0;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction1;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction2;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction3;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction4;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction5;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction6;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction7;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction8;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction9;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction10;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction11;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction12;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction13;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction14;
	MixFunction[pos++] = &RawDataMixerLite::MixFunction15;

	//emit signalConnectWithGUIElements();

	if (dataSetList == NULL)
	{

		dataSetList = new LinkedList<DataSet*>;
		noInputDataSetListMode = true;
	}
	else
	{
		noInputDataSetListMode = false;
	}

}

//--------------------------

RawDataMixerLite::~RawDataMixerLite()
{
	if (externalLayerList == false)
		delete rawDataLayerList;


	dataSetList = NULL;

	if (mmask != NULL) delete[]mmask;	mmask = NULL;
	if (mmask2 != NULL) delete[]mmask2;	mmask2 = NULL;

	if (childBuffer != NULL) delete[]childBuffer;	childBuffer = NULL;

	if (dataView != NULL) delete[]dataView;	dataView = NULL;
	if (dataViewTmp != NULL) delete[]dataViewTmp;	dataViewTmp = NULL;

	if (intBuffers[0] != NULL) delete[]intBuffers[0];
	if (intBuffers[1] != NULL) delete[]intBuffers[1];

	if (uCharBuffers[0] != NULL) delete[]uCharBuffers[0];
	if (uCharBuffers[1] != NULL) delete[]uCharBuffers[1];

	for (int i = 0; i<3; i++)
		if (uShortBuffers[i] != NULL) delete[]uShortBuffers[i];
	//if (uShortBuffers[1] != NULL) delete[]uShortBuffers[1];

	if (childBufferOpt != NULL) delete[]childBufferOpt;
	if (dataBufferOpt != NULL) delete[]dataBufferOpt;


	if (layerMaskFilterList != NULL) delete layerMaskFilterList;
	if (layerFilterList != NULL) delete layerFilterList;

	if (uShortTmpArray != NULL) delete[]uShortTmpArray;

	if (dataToFilter != NULL) delete dataToFilter;

	if (noInputDataSetListMode == true)
	{
		if (mainListItem != NULL)
		{
			mainListItem->GetData()->GetRawData()->SetDataArray(NULL, false);
			delete dataSetList;
		}
	}

	if (uCharTab != NULL) delete[]uCharTab;	uCharTab = NULL;
	if (uShortTab != NULL) delete[]uShortTab;	uShortTab = NULL;
	if (doubleTab != NULL) delete[]doubleTab;	doubleTab = NULL;

	if (higFunc != NULL) delete[]shaFunc;
	if (shaFunc != NULL) delete[]higFunc;
	if (midFunc != NULL) delete[]midFunc;

}
//----------------------------
void RawDataMixerLite::SlotUpdateBigMaskData(unsigned char *bigMask)
{
	mainBigMask = bigMask;
	position[0] = 0;
	position[1] = 0;

}
//------------------------------------------------
void RawDataMixerLite::SlotUpdateBigMaskData2(unsigned char* bigMask)
{
	mainBigMask2 = bigMask;
	position[0] = 0;
	position[1] = 0;

}
void RawDataMixerLite::UpdateMainData(ListItem<DataSet*>		*_mainListItem, bool createBuffers)
{
	startOptID = 0;
	lastModifiedLayerItem = NULL;
	minLayerMods = 1;
	sliceNrOpt = 0;


	position[0] = 0;
	position[1] = 0;

	mainListItemPos = -1000;


	for (int i = 0; i<this->dataSetList->GetNumberOfItems(); i++)
	{
		if (_mainListItem == dataSetList->GetItemAtPos(i))
		{
			mainListItemPos = i;
			break;
		}
	}

	if (mainListItemPos>-1000)
	{
		mainListItem = _mainListItem;
		if (mainListItem != NULL)		mainDataSet = mainListItem->GetData();
		UpdateBuffers();
	}
	else
	{
		mainListItem = NULL;
	}



	if (mainDataSet != NULL)
	{

		//***************
		//int n[3];
		//double scale[3];
		mainDataSet->GetRawData()->GetN(n);
		mainDataSet->GetRawData()->GetScale(scale);

		switch (viewType)
		{
		case 0:
			nX = n[0]; nY = n[1]; nZ = n[2];
			dX = scale[0]; dY = scale[1];
			GetSliceValue = &RawDataMixerLite::GetSliceValue0;
			break;

		case 1:

			nX = n[0]; nY = n[2]; nZ = n[1];
			dX = scale[0]; dY = scale[2];
			GetSliceValue = &RawDataMixerLite::GetSliceValue1;
			break;

		case 2:

			nX = n[1]; nY = n[2]; nZ = n[0];
			dX = scale[1]; dY = scale[2];
			GetSliceValue = &RawDataMixerLite::GetSliceValue2;

			break;
		}

		//*******************

		//int nx=mainDataSet->GetRawData()->GetNx();
		//int ny=mainDataSet->GetRawData()->GetNy();
		if (createBuffers == true)
		{

			for (int i = 0; i < this->rawDataLayerList->GetNumberOfItems(); i++)
			{

				rawDataLayerList->GetItemAtPos(i)->GetData()->CreateBuffers(nX, nY, nZ);
			}
			UpdateMaxValueForFilters(mainDataSet->GetRawData()->GetMaxValue());




		}


		int max = mainDataSet->GetRawData()->GetMaxValue();




	}
}

//---------------------------

void RawDataMixerLite::UpdateBuffers()
{


	if ((dataSetList != NULL) && (dataSetList->GetNumberOfItems()>0) && (mainListItem != NULL))
		if ((rawDataLayerList != NULL))
		{
			//this->isMixerEnabled=false;


			DataSet *tmpDataSet;
			DataSet *mainDataSet = mainListItem->GetData();


			if (childBuffer != NULL) delete[]childBuffer;	childBuffer = NULL;
			if (dataView != NULL) delete[]dataView;	dataView = NULL;
			if (dataViewTmp != NULL) delete[]dataViewTmp;	dataViewTmp = NULL;


			//***************
			//int n[3];
			//double scale[3];
			mainDataSet->GetRawData()->GetN(n);
			mainDataSet->GetRawData()->GetScale(scale);

			switch (viewType)
			{
			case 0:
				nX = n[0]; nY = n[1]; nZ = n[2];
				dX = scale[0]; dY = scale[1];
				GetSliceValue = &RawDataMixerLite::GetSliceValue0;
				break;

			case 1:

				nX = n[0]; nY = n[2]; nZ = n[1];
				dX = scale[0]; dY = scale[2];
				GetSliceValue = &RawDataMixerLite::GetSliceValue1;
				break;

			case 2:

				nX = n[1]; nY = n[2]; nZ = n[0];
				dX = scale[1]; dY = scale[2];
				GetSliceValue = &RawDataMixerLite::GetSliceValue2;
				break;
			}

			//*******************


			///*int size[3];
			//mainDataSet->GetRawData()->GetN(size);
			//Nx=size[0];
			//Ny=size[1];
			//Nz=size[2];
			//sliceSize=Nx*Ny;*/


			sliceSize = mainDataSet->GetRawData()->GetSliceSize();// nX*nY;


			int nrr = dataSetList->GetNumberOfItems();


			if (viewType == 0)
			{

				this->rawDataLayerList->DeleteAllItems();
				mainLayer = new RawDataLayer(layerMaskFilterList, layerFilterList, nX, nY, nZ, 0);

				//mainLayer->SetName(dataSetList->GetItemAtPos(mainListItemPos)->GetData()->GetRawData()->GetRawDataSetName());

				createdLayerCounter = 0;

				mainLayer->SetName("Layer " + QString::number(createdLayerCounter++));

				//guiWidget->SlotInsertNewVolumeMixerItem(mainLayer,this->rawDataLayerList);

				rawDataLayerList->InsertItem(mainLayer);
			}
			//mainLayer->GetTreeWidgetItem()->setSelected(true);


			int bufferSize = nX*nY;// sliceSize;



								   //for(int i=0;i<dataSetList->GetNumberOfItems();i++)
								   //{
								   //	//int ss=dataSetList->GetItemAtPos(i)->GetData()->GetRawData()->GetSliceSize();
								   //	
								   //	
								   //	if (ss>bufferSize) bufferSize=ss;
								   //}

			childBuffer = new unsigned short[bufferSize];
			dataView = new unsigned short[bufferSize]; //*3 dodam
			dataViewTmp = new unsigned short[bufferSize]; //*3 bo time level!

			if (intBuffers[0] != NULL) delete[]intBuffers[0];
			if (intBuffers[1] != NULL) delete[]intBuffers[1];

			if (uCharBuffers[0] != NULL) delete[]uCharBuffers[0];
			if (uCharBuffers[1] != NULL) delete[]uCharBuffers[1];

			for (int i = 0; i<3; i++)if (uShortBuffers[i] != NULL) delete[]uShortBuffers[i];
			//if (uShortBuffers[1] != NULL) delete[]uShortBuffers[1];


			if (childBufferOpt != NULL) delete[]childBufferOpt;
			if (dataBufferOpt != NULL) delete[]dataBufferOpt;


			if (dataToFilter != NULL) delete dataToFilter;
			dataToFilter = new RawDataSet(nX, nY, 1, dX, dY, dZ);


			if (mmask != NULL) delete[]mmask;	mmask = NULL;
			mmask = new unsigned char[bufferSize];
			memset(mmask, 0, bufferSize * sizeof(char));



			if (mmask2 != NULL) delete[]mmask2;	mmask2 = NULL;
			mmask2 = new unsigned char[bufferSize];
			memset(mmask2, 0, bufferSize * sizeof(char));

			intBuffers[0] = new int[bufferSize];
			intBuffers[1] = new int[bufferSize];

			memset(intBuffers[0], 0, bufferSize * sizeof(int));
			memset(intBuffers[1], 0, bufferSize * sizeof(int));

			uCharBuffers[0] = new unsigned char[bufferSize];
			uCharBuffers[1] = new unsigned char[bufferSize];

			memset(uCharBuffers[0], 0, bufferSize * sizeof(char));
			memset(uCharBuffers[1], 0, bufferSize * sizeof(char));

			for (int i = 0; i < 3; i++)
			{
				uShortBuffers[i] = new unsigned short[bufferSize];
				//	uShortBuffers[1] = new unsigned short[bufferSize];

				memset(uShortBuffers[i], 0, bufferSize * sizeof(unsigned short));
				//	memset(uShortBuffers[1], 0, bufferSize * sizeof(unsigned short));
			}

			if (uShortInputMode == false)
			{
				if (uShortTmpArray != NULL) delete[]uShortTmpArray;
				uShortTmpArray = new unsigned short[bufferSize];
				memset(uShortTmpArray, 0, bufferSize * sizeof(unsigned short));

			}

			childBufferOpt = new unsigned short[bufferSize];
			dataBufferOpt = new unsigned short[bufferSize];
			memset(childBufferOpt, 0, bufferSize * sizeof(unsigned short));
			memset(dataBufferOpt, 0, bufferSize * sizeof(unsigned short));

			emit signalUpdateNewVolumeMixerItem();
			somethingToMix = true;

		}
		else
		{
			somethingToMix = false;
		}

}


//------------------------------------
RawDataSet * RawDataMixerLite::SlotGenerateMixedData()
{
	RawDataSet *tData = NULL;
	if (somethingToMix == true)
	{
		//zrob mixa i wyslij




		int i, j, k;
		int pos = 0;
		unsigned short cc;
		RawDataSet *data = mainListItem->GetData()->GetRawData();

		//int NX,NY,NZ;

		tData = new RawDataSet();
		tData->Copy(*data, false);

		tData->ZeroQ();
		tData->SetRawDataSetName("d0");

		unsigned short *slice;

		unsigned short *dArray = tData->GetDataArray();

		int n[3];
		data->GetN(n);

		if (viewType == 0)
		{

			for (k = 0; k < n[2]; k++)
			{
				pos = data->GetSliceSize()*k;
				slice = SlotGenerateMixedSliceImage(k);
				if (slice != NULL)
				{
					for (i = 0; i < data->GetSliceSize(); i++)
					{
						dArray[pos++] = slice[i];
					}
				}
				emit SendMessage("Transversal Mixing: (" + QString::number(k) + " / " + QString::number(data->GetNz() - 1) + " )");
			}
		}
		else if (viewType == 1)
		{



			for (k = 0; k < n[1]; k++)
			{
				int voxelPos = 0;
				//pos = data->GetSliceSize()*k;
				slice = SlotGenerateMixedSliceImage(k);
				if (slice != NULL)
				{
					for (j = 0; j < n[2]; j++)
					{

						for (i = 0; i < n[0]; i++)
						{
							tData->SetVoxelValue(i, n[1] - k - 1, n[2] - j - 1, slice[voxelPos]);
							voxelPos++;
						}
					}
				}
				emit SendMessage("Coronal Mixing: (" + QString::number(k) + " / " + QString::number(data->GetNz() - 1) + " )");
			}



		}
		else if (viewType == 2)
		{

			for (k = 0; k < n[0]; k++)
			{
				int voxelPos = 0;

				//pos = data->GetSliceSize()*k;
				slice = SlotGenerateMixedSliceImage(k);
				if (slice != NULL)
				{
					for (j = 0; j < n[2]; j++)
					{

						for (i = 0; i < n[1]; i++)
						{
							tData->SetVoxelValue(k, i, n[2] - j - 1, slice[voxelPos]);
							voxelPos++;
						}
					}
				}
				emit SendMessage("Sagittal Mixing: (" + QString::number(k) + " / " + QString::number(data->GetNz() - 1) + " )");
			}
		}

		tData->SetRawDataSetName(data->GetRawDataSetName() + "_mix");
		tData->CalculateHistogram();
		tData->dicomLevelValue = tData->GetMaxValue() / 2;
		tData->dicomMinValue = 0;
		tData->dicomRescaleValue = 1;
		tData->dicomWindowWidthValue = tData->GetMaxValue();
		//emit signalSendNewRawDataSet(tData);

	}
	return tData;
}

//------------------------
unsigned short * RawDataMixerLite::SlotGenerateMixedSliceImage(int sliceNumber, double *inParamsX, int nrInParamsX, double *outParamsX, int nrOutParamsX)
{
	//countFps();




	if (somethingToMix == true)
	{
		if (shapes != NULL)
			this->shapes->shapeList.clear();
		//1 wygeneruj odpwowiedni mix na odpowiednim dataViews
		//2 przy uzyciu palette pomaluj bits
		//	int maxVal=65535;


		if ((dataView == NULL) || (dataViewTmp == NULL)) return NULL;


		int i, j;
		int pos = 0;
		unsigned short cc;
		RawDataSet *data = mainListItem->GetData()->GetRawData();

		int nrr = rawDataLayerList->GetNumberOfItems();


		bool validate;
		int mixMode;
		double mixOpacity, mixOpacity1;
		unsigned char mCharOpacity;

		unsigned short mixedVal;

		double multiplier = 1.0;
		double addition = 0.0;

		unsigned short *dd, *ddPrev;
		bool bufferAlreadySaved = false;

		dd = dataView;
		ddPrev = dataViewTmp;

		unsigned short a, b;
		//unsigned char a,b;
		int counter = 0;


		double m255;

		int maskMode = 0;
		int filterMode = 0;

		int sliceNr;
		RawDataLayer *layer;
		//unsigned short *dataTab;

		bool childModeCurrent = false;
		bool childModePrev = false;
		int currentSourceMaxValue = 0;

		int start = 0;

		//parent enabled =fasle
		// przebiegne po warstwach od gory
		// jesli wykryje rodzica schowanego sprawdze jego dzieci i ustawie im parent enabled =false

		bool isChildToHide = false;
		//for(int k=0;k<nrr;k++)
		for (int k = nrr - 1; k>0; k--)
		{
			//QTreeWidgetItem *tmpItem=mixerTreeWidget->topLevelItem(k);
			//for(int l=0;l<rawDataLayerList->GetNumberOfItems();l++)	{	if (this->rawDataLayerList->GetItemAtPos(l)->GetData()->GetTreeWidgetItem()==tmpItem)	{layer=rawDataLayerList->GetItemAtPos(l)->GetData();break;}}

			//	layer=GetTopLayerAtPos(k);
			layer = rawDataLayerList->GetItemAtPos(k)->GetData();
			layer->SetParentEnabledMode(true);
			if (isChildToHide == true)
			{
				if (layer->GetLayerChildMode() == true)
				{
					layer->SetParentEnabledMode(false);
				}
				else
				{
					isChildToHide = false;
				}
			}
			else
				if (layer->GetLayerMixerMode() == true) //jesli to rodzic z dziecmi
				{
					if ((layer->GetLayerEnabled() == false) || (layer->GetBlendingOpacity() == 0)) //jesli jest schowany
					{
						//schowaj jego dzieci
						isChildToHide = true;

					}
				}
		}



		//-----------------------------------
		//-----------------------------------
		//-----------------------------------

		//unsigned char *mmask=NULL;

		int nrOfLayers = rawDataLayerList->GetNumberOfItems();
		int  *checkForBufferUsageTab = new int[nrOfLayers];
		memset(checkForBufferUsageTab, 0, nrOfLayers * sizeof(int));

		for (int k = 0; k<nrOfLayers; k++)
		{
			int posx = rawDataLayerList->GetItemAtPos(k)->GetData()->GetDataSetNumber();
			int mix = rawDataLayerList->GetItemAtPos(k)->GetData()->GetMixedLayer();
			if (posx<0)
			{
				int lPos = -posx - 1;
				int v = (mix == false) ? 1 : 2;
				checkForBufferUsageTab[lPos] = v; //- 0 - dont copy, 1 - copy filtered layer, 2 - copy mixed filtered
			}
			else
			{
				RawDataLayer *layer = rawDataLayerList->GetItemAtPos(k)->GetData();
				if (layer->GetMaskEnabled() == true)
				{

					int nrr = layer->GetMaskNumber();
					if (nrr > 0) //bo zero oznacza current wiec nie trzeba dodatkowo blokowac zawartosci
					{

						checkForBufferUsageTab[nrr - 1] = 1;//dla maski tylko byc moze potrzebna warstwa
					}
				}

			}
		}




		//-----------------------------------
		//-----------------------------------
		//-----------------------------------


		//RawDataSet *dataToFilter=NULL;
		double maxCurrent = 0;
		int modex;
		for (int k = start; k<rawDataLayerList->GetNumberOfItems(); k++)
		{

			/*	if (viewType == 1)
			{
			if (k == 1) break;
			}*/

			//=NULL;

			sliceNr = sliceNumber;
			pos = 0;
			counter = 0;

			if (k == 0) //bazowa nie mixuje
			{

				rawDataLayerList->GetItemAtPos(0)->GetData()->SetDataOrigMaxValue(data->GetMaxValue());
				unsigned short *tab = data->GetDataArray() + (sliceNumber*data->GetSliceSize());;

				for (j = 0; j < nY; j++)
				{

					for (i = 0; i < nX; i++)
					{

						if (viewType == 0)
						{
							dd[pos] = tab[pos++];
						}
						else
						{
							dd[pos++] = (this->*GetSliceValue)(data->GetDataArray(), i, j, sliceNr, n[0], n[1], n[2], sliceSize, NULL);//testowe
						}
						counter++;
					}
				}

				if (checkForBufferUsageTab[0]>0)
				{
					unsigned short *dst = rawDataLayerList->GetItemAtPos(0)->GetData()->getFilterBufferData(viewType)->GetDataArray();

					memcpy(dst, dd, nX*nY * sizeof(unsigned short));
				}


				if (this->mainBigMask != NULL)
				{
					pos = 0;
					switch (viewType)
					{
					case 0:
						memcpy(mmask, mainBigMask + sliceNumber*sliceSize, sliceSize);


						// = n[0]; Ny = n[1]; Nz = n[2];
						//dX = scale[0]; dY = scale[1];
						break;

					case 1:
						for (j = 0; j < nY; j++)
						{
							for (i = 0; i < nX; i++)
							{
								mmask[pos++] = GetSliceValueMask1(mainBigMask, i, j, sliceNr, n[0], n[1], n[2], sliceSize, NULL);//testowe
							}
						}

						break;

					case 2:
						for (j = 0; j < nY; j++)
						{
							for (i = 0; i < nX; i++)
							{
								mmask[pos++] = GetSliceValueMask2(mainBigMask, i, j, sliceNr, n[0], n[1], n[2], sliceSize, NULL);//testowe
							}
						}

						break;
					}

					//mmask=mainBigMask+sliceNr*sliceNumber; //tu trzeba obslugiwac prawdziwym buforem i kopiowac
				}
				if (this->mainBigMask2 != NULL)
				{
					pos = 0;
					switch (viewType)
					{
					case 0:
						memcpy(mmask2, mainBigMask2 + sliceNumber * sliceSize, sliceSize);


						// = n[0]; Ny = n[1]; Nz = n[2];
						//dX = scale[0]; dY = scale[1];
						break;

					case 1:
						for (j = 0; j < nY; j++)
						{
							for (i = 0; i < nX; i++)
							{
								mmask2[pos++] = GetSliceValueMask1(mainBigMask2, i, j, sliceNr, n[0], n[1], n[2], sliceSize, NULL);//testowe
							}
						}

						break;

					case 2:
						for (j = 0; j < nY; j++)
						{
							for (i = 0; i < nX; i++)
							{
								mmask2[pos++] = GetSliceValueMask2(mainBigMask2, i, j, sliceNr, n[0], n[1], n[2], sliceSize, NULL);//testowe
							}
						}

						break;
					}

					//mmask=mainBigMask+sliceNr*sliceNumber; //tu trzeba obslugiwac prawdziwym buforem i kopiowac
				}

				childModeCurrent = false;

				if (viewType == 1)
				{
					int i = 1;
				}
			}
			else
			{

				//int currentLayerSource;

				int maxMain = this->mainDataSet->GetRawData()->GetMaxValue();

				//znalezc nastepne dane;

				//data1=NULL;



				layer = rawDataLayerList->GetItemAtPos(k)->GetData();
				mixOpacity = layer->GetBlendingOpacity();
				mCharOpacity = mixOpacity*255.0;
				m255 = mixOpacity / 255.0;

				if //((mixOpacity==0)||
					((layer->GetLayerEnabled() == false) //schowana wawstwa
						|| ((layer->GetLayerChildMode() == true) && (layer->GetParentEnabledMode() == false)) // warstwadziecko ale mixer zgaszony
																											  //	||((childModeCurrent==true)&&(layer->GetLayerMixerMode()==true)) //poprzedni rodzic, a aktualny mixer 


						) continue;

				//currentLayerSource=layer->GetDataSetNumber();


				mixMode = layer->GetBlendingMode();

				mixOpacity1 = 1 - mixOpacity;


				multiplier = layer->GetBlendingMultiplier();
				addition = layer->GetBlendingAddition();

				maskMode = layer->GetMaskFilterMode();
				filterMode = layer->GetFilterMode();


				int dataSetNumber = layer->GetDataSetNumber();
				if (dataSetNumber == 0)
				{
					//dataToFilter = this->dataSetList->GetItemAtPos(mainListItemPos)->GetData()->GetRawData();//zmiana!!!!!!!
					int pos = 0;
					RawDataSet *sourceData = this->dataSetList->GetItemAtPos(mainListItemPos)->GetData()->GetRawData();
					unsigned short *tab = sourceData->GetDataArray();//zmiana!!!!!!!
					unsigned short *dToFilterTab = dataToFilter->GetDataArray();

					rawDataLayerList->GetItemAtPos(k)->GetData()->SetDataOrigMaxValue(maxMain);

					for (int j = 0; j < nY; j++)
					{

						for (int i = 0; i < nX; i++)
						{

							if (viewType == 0)
							{
								dToFilterTab[pos] = tab[sliceNumber*data->GetSliceSize() + pos++];
							}
							else
							{
								dToFilterTab[pos++] = (this->*GetSliceValue)(tab, i, j, sliceNr, n[0], n[1], n[2], sliceSize, NULL);//testowe
							}

						}
					}

				}
				else
					if (dataSetNumber<0) // skopiuj z zapamietanego bufora  //layers
					{
						//max value z poprzedniej warstwy
						unsigned short sourceMax = rawDataLayerList->GetItemAtPos(-dataSetNumber - 1)->GetData()->GetDataOrigMaxValue();
						rawDataLayerList->GetItemAtPos(k)->GetData()->SetDataOrigMaxValue(sourceMax);


						unsigned short *qq = rawDataLayerList->GetItemAtPos(-dataSetNumber - 1)->GetData()->getFilterBufferData(viewType)->GetDataArray();
						memcpy(dataToFilter->GetDataArray(), qq, nX*nY * sizeof(unsigned short));
						//sliceNr=0;


						//dataToFilter=rawDataLayerList->GetItemAtPos(-dataSetNumber-1)->GetData()->getFilterBufferData(viewType);
						//unsigned short *xx=dataToFilter->GetDataArray();
						//prevLayerDataSet->SetDataArray(xx);
					}
					else //uzyj aktualnej warstwy zrodlowej //source data
					{
						//warstwa zrodlowa!!!




						RawDataSet *currentSourceData = this->dataSetList->GetItemAtPos(dataSetNumber - 1)->GetData()->GetRawData();

						//max  z oryginalnych danych;
						unsigned short sourceMax = currentSourceData->GetMaxValue();
						rawDataLayerList->GetItemAtPos(k)->GetData()->SetDataOrigMaxValue(sourceMax);

						int cN[3];
						currentSourceData->GetN(cN);


						int nX1, nY1, nZ1;
						double dX1, dY1, dZ1;
						int sliceSize1;

						int n[3];
						double scale[3];
						currentSourceData->GetN(n);
						currentSourceData->GetScale(scale);

						switch (viewType)
						{
						case 0:
							nX1 = n[0]; nY1 = n[1]; nZ1 = n[2];
							dX1 = scale[0]; dY1 = scale[1];
							break;

						case 1:

							nX1 = n[0]; nY1 = n[2]; nZ1 = n[1];
							dX1 = scale[0]; dY1 = scale[2];

							break;

						case 2:

							nX1 = n[1]; nY1 = n[2]; nZ1 = n[0];
							dX1 = scale[1]; dY1 = scale[2];

							break;
						}

						if ((nX == nX1) && (nY == nY1) && (nZ == nZ1))
						{
							//jesli dane zrodlowe warstwy maja takie same wymiary to latwo rob tak samo jak  k==0
							int pos = 0;
							unsigned short *tab = currentSourceData->GetDataArray();//zmiana!!!!!!!
							unsigned short *dToFilterTab = dataToFilter->GetDataArray();
							int tmpVal;
							int ssize = data->GetSliceSize();


							for (int j = 0; j < nY; j++)
							{

								for (int i = 0; i < nX; i++)
								{

									if (viewType == 0)
									{
										tmpVal = tab[sliceNumber*ssize + pos];
										//	dToFilterTab[pos] = ;
									}
									else
									{
										tmpVal = (this->*GetSliceValue)(tab, i, j, sliceNr, n[0], n[1], n[2], sliceSize, NULL);//testowe
									}
									if (tmpVal > 0)
									{
										i = i;
									}
									dToFilterTab[pos++] = tmpVal;
								}
							}
						}
						else
						{
							//dane zrodlowe maja inny wymiar!!!
							memset(dataToFilter->GetDataArray(), 0, nX*nY * sizeof(unsigned short)); //czyszcze 
							unsigned short *tab = currentSourceData->GetDataArray();//zmiana!!!!!!!
							unsigned short *dToFilterTab = dataToFilter->GetDataArray();
							bool validate = false;
							int pos1 = 0;
							//validate koniecznie, bo nie ma pewnosci ze sie zmieszcze w danych
							for (int j = 0; j < nY; j++)
							{
								for (int i = 0; i < nX; i++)
								{
									dToFilterTab[pos1++] = (this->*GetSliceValue)(tab, i, j, sliceNr, n[0], n[1], n[2], sliceSize, &validate);//testowe
								}
							}


							//id

						}

						//unsigned short *qq = this->dataSetList->GetItemAtPos(dataSetNumber - 1)->GetData()->GetRawData()->GetDataArray();
						//memcpy(dataToFilter->GetDataArray(), qq, sliceSize*sizeof(unsigned short));
						//dataToFilter=this->dataSetList->GetItemAtPos(dataSetNumber-1)->GetData()->GetRawData();
					}

				childModePrev = childModeCurrent;
				childModeCurrent = layer->GetLayerChildMode();
				modex = childModePrev * 10 + childModeCurrent;
				maxCurrent = maxMain;
				if (layer->GetAutoNormalize() == true)
				{
					if (modex != 10)
						maxCurrent = rawDataLayerList->GetItemAtPos(k)->GetData()->GetDataOrigMaxValue();
					//dataToFilter->GetMaxValue();
					else
						maxCurrent = maxMain;
					//layer->GetDataSet()->GetRawData()->GetMaxValue();
					if (maxCurrent>0)	multiplier = maxMain / maxCurrent;
				}



				if (viewType == 1)
				{
					int i = 1;
				}





				if (dataToFilter != NULL)
				{
					unsigned short *tmp = dd;				dd = ddPrev;				ddPrev = tmp;//buffer swap;
																									 /*int nX1, nY1, nZ1;
																									 double dX1,dY1,dZ1;
																									 int sliceSize1;

																									 int n[3];
																									 double scale[3];
																									 data->GetN(n);
																									 data->GetScale(scale);

																									 switch (viewType)
																									 {
																									 case 0:
																									 nX1 = n[0]; nY1 = n[1]; nZ1 = n[2];
																									 dX1 = scale[0]; dY1 = scale[1];
																									 break;

																									 case 1:

																									 nX1 = n[0]; nY1 = n[2]; nZ1 = n[1];
																									 dX1 = scale[0]; dY1 = scale[2];

																									 break;

																									 case 2:

																									 nX1 = n[1]; nY1 = n[2]; nZ1 = n[0];
																									 dX1 = scale[1]; dY1 = scale[2];

																									 break;
																									 }*/

																									 /*	nx1=dataToFilter->GetNx();
																									 ny1=dataToFilter->GetNy();
																									 nz1=dataToFilter->GetNz();
																									 ss1=dataToFilter->GetSliceSize();*/
																									 //	tab1=dataToFilter->GetDataArray();

																									 //sliceSize1 = nX1*nY1;


					bool maskFiltered = false;
					int dataFiltered = false;

					//	int nzTmp=nZ1;







					//tutaj trzeba robic podmianke getrawdata, dla mixedlayers!!!!!!!!!!!!!! to do


					// child to parent 10
					// parent to child 01
					// next child   11
					// next parent - 00 

					// jesli c2p - mix child buffer  previous parent - nie obliczamy niczego!!!!  - schowac parametry filters - zmieniam nazwe na layer comp?
					// jesli p2c - fill child buffer
					// jesli next child - mix curent with child buffer
					// jesli next parent - mix current with parent buffer
					/*if (objName.compare("coronalMixer") == 0)
					{
					qDebug() << "X";
					}
					*/
					unsigned short *fillBuffer;
					//bool createBuffer=((currentLayerSource<0)&&(checkForBufferUsageTab[-currentLayerSource-1]==true));
					int createBuffer = (checkForBufferUsageTab[k]);
					unsigned short *dst = layer->getFilterBufferData(viewType)->GetDataArray();

					//bool nonMixedResult=(createBuffer==1);

					//if (modex!=10) //mixer layer case
					//	double dx=data->GetXscale();
					//	double dy=data->GetYscale();

					//	int mnx = data->GetNx();
					//	int mny = data->GetNy();

					if ((nX > 3) && (nY > 3))
					{

						if (layer->GetMaskEnabled() == true)
						{
							//int nrr = layer->GetMaskNumber();
							//	if (nrr == 0)
							{
								LayerMaskFilter *layerMaskFilter = layerMaskFilterList->GetItemAtPos(maskMode)->GetData();
								layerMaskFilter->SetMainMaskData(mmask, position[0], position[1]);
								layerMaskFilter->SetMainMaskData2(mmask2);
								layerMaskFilter->SetMainDataPixelSizes(dX, dY);
								layerMaskFilter->SetMainDataSizes(nX, nY);
								layerMaskFilter->SetMainDataMaxValue(maxCurrent);
								layerMaskFilter->SetViewType(viewType);

								layerMaskFilter->SetBrightnessTabs(funSize, higFunc, midFunc, shaFunc);

								//2016.10.16 - du¿a zmiana!!!! - nie bedzie zrodlem danych dla maski cos innego niz dla danej warstwy - jesli chce cos innego to biore maske z warstwy innej!! mask number
								RawDataSet *tmpData = dataToFilter;
								//	RawDataSet *tmpData = (nrr == 0) ? dataToFilter:rawDataLayerList->GetItemAtPos(nrr-1)->GetData()->getFilterBufferData(viewType);
								//  jesli nrr==0 to znaczy ze zrodlem dla filtra maski ma byc to samo co dla filtra danych
								// jesli nrr>0 znaczy ze zrodlem jest wynik jakiegos filtrowania z poprzednich zapamietanych warstw -> zatem uzywam nrr-1
								///memset(layer->getMaskBuffer(viewType),)

								int nrMask = layer->GetMaskNumber();
								if (nrMask > 0)
								{//jesli zrodlem maski jest cos !=0 kopiuje z odpowiedniej warstwy poprzedzajacej!

									unsigned char *sourceMask = rawDataLayerList->GetItemAtPos(nrMask - 1)->GetData()->getMaskBuffer(viewType);
									unsigned char *dstMask = layer->getMaskBuffer(viewType);
									memcpy(dstMask, sourceMask, nX*nY * sizeof(unsigned char));

								}
								maskFiltered = layerMaskFilter->ApplyFiltering(this, tmpData, 0, layer->getMaskBuffer(viewType), layer->GetMaskFilterParams(), intBuffers, shapes, iWidget, ddPrev);
							}
						}

						if ((layer->GetFilterEnabled() == true) && (modex != 10))
						{
							LayerFilter *layerFilter = layerFilterList->GetItemAtPos(filterMode)->GetData();
							layerFilter->SetMainDataPixelSizes(dX, dY);
							layerFilter->SetMainDataSizes(nX, nY);
							layerFilter->SetMainDataMaxValue(maxCurrent);
							layerFilter->SetViewType(viewType);
							layerFilter->SetBrightnessTabs(funSize, higFunc, midFunc, shaFunc);
							//podmianka przekroju volumenu na przefiltrowane costam
							layerFilter->SetMainMaskData(mmask, position[0], position[1]);
							layerFilter->SetMainMaskData2(mmask2);
							layerFilter->SetPolyline(polygon);
							layerFilter->SetBrightnessFilterMode(layer->GetBrightnessFilterMode());

							layerFilter->SetOutputParams(outParamsX, nrOutParamsX);
							layerFilter->SetInputParams(inParamsX, nrInParamsX);

							layerFilter->SetGlobalThresholds(globalLeftThreshold, globalRightThreshold);

							dataFiltered = layerFilter->ApplyFiltering(this, uShortBuffers[0],
								uShortBuffers[1], layer->GetFilterParams(), shapes, iWidget, dataToFilter->GetDataArray(), layer->GetFilterInverted(),
								uCharBuffers[0], uCharBuffers[1], ddPrev, (maskFiltered == true) ? (layer->getMaskBuffer(viewType)) : NULL, intBuffers);

							//jesli zaden nie byl remembered oznaczony to wezme ostatni
							if (((layer->GetRememberBuffer() == true) || (k == nrOfLayers - 1))
								&& (bufferAlreadySaved == false))
							{
								memcpy(uShortBuffers[2], uShortBuffers[0], nX*nY * sizeof(unsigned short));//zapamietany bufor mozna wyciagnac pozniej!!!!
								bufferAlreadySaved = true;
							}



							if (dataFiltered > 0)
							{
								//tab1=layer->getFilterBufferData()->GetDataArray();
								sliceNr = 0;
								//nzTmp=nZ1;
							}
						}
					}


					unsigned char *mask = NULL;

					if (maskFiltered == true)
					{
						mask = layer->getMaskBuffer(viewType);
					}
					else //jesli maska nie zostala wyliczona to sprawdz, czy nie mamy uzyc maski z poprzedniej warstwy
					{

						int nrMask = layer->GetMaskNumber(); //trzeba brac -1 bo 0 to current layer
															 //bool mEnabled=rawDataLayerList->GetItemAtPos(nrr)->GetData()->GetMaskEnabled();
															 //if ((nrr>0)&&(mEnabled==true))
						if (nrMask>0)
						{
							mask = rawDataLayerList->GetItemAtPos(nrMask - 1)->GetData()->getMaskBuffer(viewType);
						}
						else
							layer->getMaskBuffer(viewType);


					}


					int val;
					//dd1=dd;

					unsigned short *startBuff, *endBuff, startSliceNr;

					unsigned short *beginBuff;

					if (dataFiltered>0)
					{
						beginBuff = uShortBuffers[0];
					}
					else
					{
						beginBuff = dataToFilter->GetDataArray();
					}

					switch (modex)
					{


					case 10: //c2p
						startBuff = ddPrev;		startSliceNr = 0;			endBuff = childBuffer;
						break;

					default:
						startBuff = beginBuff;				startSliceNr = sliceNr;	endBuff = ddPrev;
						break;
					}



					//chyba pos=0 mialop byc!
					for (j = 0; j<nY; j++)
					{
						for (i = 0; i<nX; i++)
						{
							//validate position!!!
							//if ((i < nX1) && (j < nY1))
							{
								b = startBuff[pos];
								validate = true;
							}
							/*else
							{
							b = 0;
							validate = false;
							}*/

							//b=GetSliceValue0(startBuff,i,j,startSliceNr,nX1,nY1,nzTmp,sliceSize1,&validate);

							if (createBuffer == 1)
							{
								dst[pos] = b;
							}


							//b=startBuff[pos];
							b = b*multiplier + addition;
							if
								//((validate==true)&&
								(((mask == NULL) || (mask[pos]>0)))
							{
								if (modex != 01)
								{
									a = endBuff[pos];

									if ((k == 3) && ((a>255) || (b>255)))
									{
										i = i;
									}

									double vvv = mixOpacity;
									if ((mask != NULL) && (mask[pos]<255))
									{
										vvv = mask[pos] * m255;
									}
									mixedVal = (this->*MixFunction[mixMode])(a, b);


									//val=Round(a*mixOpacity1+mixedVal*mixOpacity);
									//val=a+((unsigned char)	((((unsigned int)(a - b)<<8)*mCharOpacity)>>16)); // przejscie na maski 0-255 i dane uchar
									val = Round(a + (mixedVal - a)*vvv);
									if (val>maxMain) val = maxMain; else if (val<0) val = 0;
									dd[pos] = val;												counter++;
								}
								else
								{
									dd[pos] = b;
									childBuffer[pos] = ddPrev[pos];
								}


							}
							else
							{
								dd[pos] = endBuff[pos];
								if (modex == 01)
								{
									childBuffer[pos] = endBuff[pos];
								}
							}	pos++;
						}
					}
					//}
					//else
					//{
					//	//jesli child to parent - skopiuj ddPrev do dd 
					//	// i odswieze ppPrev zapamienanym child bufferem
					//	memcpy(dd,ddPrev,sliceSize*sizeof(unsigned short));
					//	memcpy(ddPrev,childBuffer,sliceSize*sizeof(unsigned short));
					//}
					//-----------------------------


					//jesli jakas pozniejsza warstwa uzywa wyniku tej - zapamietuje ja do bufora warstwy

					if (createBuffer == 1)//zapamietaj mix
					{
						memcpy(dst, dd, nX*nY * sizeof(unsigned short));
					}

					if (createBuffer>0)
						layer->getFilterBufferData(viewType)->CalculateHistogram();



				}

			}

		}
		delete[]checkForBufferUsageTab;


		if (tm != NULL)
		{
			timeStamp = tm->elapsed();
		}
		emit redrawed();
		qDebug() << "mixer "<< objectName();

		return dd;


	}


	return NULL;
}
//---------------------------------------
unsigned short RawDataMixerLite::GetSliceValue0(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//	data->GetVoxelValue(x,y,sliceNumber);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((x<0) || (x>nx - 1) || (y<0) || (y>ny - 1) || (sliceNumber<0) || (sliceNumber>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[sliceNumber*ss + y*nx + x];

}
//---------------------------------------
unsigned short RawDataMixerLite::GetSliceValue1(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//cc=dataT->GetVoxelValue(x,sy-SliceNumber-1,sz-y-1);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((x<0) || (x>nx - 1) || ((ny - sliceNumber - 1)<0) || ((ny - sliceNumber - 1)>ny - 1) || ((nz - y - 1)<0) || ((nz - y - 1)>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[(nz - y - 1)*ss + (ny - sliceNumber - 1)*nx + x];
}
//---------------------------------------
unsigned short RawDataMixerLite::GetSliceValue2(unsigned short *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//	cc=dataT->GetVoxelValue(SliceNumber,x,sz-y-1);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((sliceNumber<0) || (sliceNumber>nx - 1) || (x<0) || (x>ny - 1) || ((nz - y - 1)<0) || ((nz - y - 1)>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[(nz - y - 1)*ss + x*nx + sliceNumber];
}

//------------------------------------
unsigned char RawDataMixerLite::GetSliceValueMask0(unsigned char *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//	data->GetVoxelValue(x,y,sliceNumber);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((x<0) || (x>nx - 1) || (y<0) || (y>ny - 1) || (sliceNumber<0) || (sliceNumber>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[sliceNumber*ss + y*nx + x];

}
//---------------------------------------
unsigned char RawDataMixerLite::GetSliceValueMask1(unsigned char *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//cc=dataT->GetVoxelValue(x,sy-SliceNumber-1,sz-y-1);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((x<0) || (x>nx - 1) || ((ny - sliceNumber - 1)<0) || ((ny - sliceNumber - 1)>ny - 1) || ((nz - y - 1)<0) || ((nz - y - 1)>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[(nz - y - 1)*ss + (ny - sliceNumber - 1)*nx + x];
}
//---------------------------------------
unsigned char RawDataMixerLite::GetSliceValueMask2(unsigned char *tab, int x, int y, int sliceNumber, int nx, int ny, int nz, int ss, bool *validate)
{
	//	cc=dataT->GetVoxelValue(SliceNumber,x,sz-y-1);
	//return tab[ z*SliceSize +	y*Nx + x];
	if (validate != NULL)
	{
		if ((sliceNumber<0) || (sliceNumber>nx - 1) || (x<0) || (x>ny - 1) || ((nz - y - 1)<0) || ((nz - y - 1)>nz - 1)) { *validate = false; return 0; }
		else *validate = 1;
	}
	return tab[(nz - y - 1)*ss + x*nx + sliceNumber];
}

//------------------------------------
void RawDataMixerLite::countFps()
{
	last_time = current_time;
	current_time = current_time.currentTime();
	elapsed_time = (current_time.second() * 1000 + current_time.msec()) - (last_time.second() * 1000 + last_time.msec()); // /!\ max = 59 seconds 999 ms
	this->fps = 1000.0 / elapsed_time;

	if (fps<1)
	{
		fpsCount = 0;
		fpsSum = 0;
	}
	fpsCount++;
	fpsSum += fps;

	double avg = double(fpsSum) / fpsCount;

}

//-------------------------
void RawDataMixerLite::SlotSetTransversalPos(int x, int y)
{
	position[0] = x;
	position[1] = y;
}
//--------------------------

void RawDataMixerLite::SlotGetPolyLine(QPolygon p)
{

	polygon = p;



}
//--------------------------------------
void	RawDataMixerLite::SlotSetGlobalThresholds(int t1, int t2)
{
	globalLeftThreshold = t1;
	globalRightThreshold = t2;
};
//---------------------------------------
bool	RawDataMixerLite::SaveLayerSettings(const QString & filename)
{


	QFile file(filename);
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);

		stream << "TIME_ID:" << GetTimeDateName() << "\n";
		for (int i = 0; i<this->rawDataLayerList->GetNumberOfItems(); i++)
		{
			rawDataLayerList->GetItemAtPos(i)->GetData()->writeLayerParams(stream);
		}

		file.close();
	}
	else
	{
		return false;
	}
	return true;
}
//-----------------------------------

bool	RawDataMixerLite::LoadLayerSettings(const QString & filename, bool updateGUI)
{
	int nrOfDataSets = this->dataSetList->GetNumberOfItems();
	bool test;
	QFile file(filename);
	bool anyChg = false;

	if ((nrOfDataSets>0) && (file.open(QIODevice::ReadOnly)))
	{

		QTextStream stream(&file);
		do
		{
			RawDataLayer *layer = new RawDataLayer(layerMaskFilterList, layerFilterList);
			//qWarning() << "start read layer";
			test = layer->readLayerParams(stream);

			//qWarning() << "end read layer" << test;
			if (test == true)
			{


				if (anyChg == false)
					rawDataLayerList->DeleteAllItems();
				anyChg = true;

				rawDataLayerList->InsertItem(layer);


			}
			else
			{
				delete layer;
			}
		} while (test == true);

		file.close();


		//createBuffers!!!



		if (mainDataSet != NULL)
		{
			int nx = mainDataSet->GetRawData()->GetNx();
			int ny = mainDataSet->GetRawData()->GetNy();
			int nz = mainDataSet->GetRawData()->GetNz();
			for (int i = 0; i<this->rawDataLayerList->GetNumberOfItems(); i++)
			{

				rawDataLayerList->GetItemAtPos(i)->GetData()->CreateBuffers(nx, ny, nz);
			}
			UpdateMaxValueForFilters(mainDataSet->GetRawData()->GetMaxValue());

		}


		if (anyChg == true)
		{
			emit signalUpdateGUIAfterLoadingLayersFromFile();
		}



		return true;
	}
	return false;

}
//------------------------------------------------------
void	RawDataMixerLite::ClearShapes()
{
	if (shapes->mask != NULL)
	{

		memset(shapes->mask, 1, nX*nY);
		shapes->scale = 1.0;
		shapes->shapeList.clear();
	}
	else
	{
		int i = 1;
	}

}
//-----------------------------------

void	RawDataMixerLite::UpdateMaxValueForFilters(unsigned short maxValue)
{
	if (maxValue<255) maxValue = 255;
	int nrrr = layerMaskFilterList->GetNumberOfItems();
	for (int i = 0; i<nrrr; i++)
	{
		layerMaskFilterList->GetItemAtPos(i)->GetData()->SetMainDataMaxValue(maxValue);

	}

	nrrr = layerFilterList->GetNumberOfItems();
	for (int i = 0; i<nrrr; i++)
	{
		layerFilterList->GetItemAtPos(i)->GetData()->SetMainDataMaxValue(maxValue);
	}

}
//-----------------------------------
void RawDataMixerLite::SetShapesAndVisImage(ImageShapes 	*s, ImageWidget *w)
{

	shapes = s;
	iWidget = w;
	//int nrrr=layerMaskFilterList->GetNumberOfItems();
	//for(int i=0;i<nrrr;i++)
	//{
	//	layerMaskFilterList->GetItemAtPos(i)->GetData()->SetShapes(s);
	//	layerMaskFilterList->GetItemAtPos(i)->GetData()->SetVisImageWidget(w);
	//	
	//}

	//nrrr=layerFilterList->GetNumberOfItems();
	//for(int i=0;i<nrrr;i++)
	//{
	//	layerFilterList->GetItemAtPos(i)->GetData()->SetShapes(s);
	//	layerFilterList->GetItemAtPos(i)->GetData()->SetVisImageWidget(w);
	//}


}
//--------------------------------------------
bool	RawDataMixerLite::CreateNoInputDataSetListData(int nx, int ny, double dx, double dy)
{
	if ((noInputDataSetListMode == true) && (this->dataSetList != NULL))
	{
		RawDataSet *newRawData = new RawDataSet(nx, ny, 1, dx, dy, 1, 0, false);//tworze dane bez alokacji pamieci!
		newRawData = new RawDataSet(nx, ny, 1, dx, dy, 1, 0, false);//tworze dane bez alokacji pamieci!
		dataSetList->InsertItem(new DataSet(newRawData, NULL, 10));

		UpdateMainData(dataSetList->GetItemAtPos(0));
		return true;
	}

	return false;
}

//----------------------------------------------------
bool RawDataMixerLite::SetMainDataRawDataArray(void *array)
{
	if ((noInputDataSetListMode == true) && (this->dataSetList != NULL) && (this->mainDataSet != NULL) && (array != NULL))
	{
		if (uShortInputMode == true) //input ushort
		{
			mainDataSet->GetRawData()->SetDataArray((unsigned short*)array, false);
		}
		else
		{
			//if (uShortInputMode==true)
			{
				for (int i = 0; i<this->nX*nY; i++)
				{
					uShortTmpArray[i] = ((unsigned char*)array)[i];
				}
				mainDataSet->GetRawData()->SetDataArray(uShortTmpArray, false);
				return true;
			}


			return false;
		}
		return true;
	}
	return false;

}
//-------------------------------------------
#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_SetShapesAndVisImage(ImageShapes 	*s, ImageWidget *w)
{
	if (mixer_pointer != NULL)
	{
		mixer_pointer->SetShapesAndVisImage(s, w);
		return true;
	}
	return 0;
}
//--------------------------
#ifndef __GNUC__ 
extern "C" __declspec(dllexport)
#endif
int FiltDram_Initialization2(const char * p_plikKonfiguracja, LinkedList<DataSet*>	*_dataSetList, int mainDataNr)
{

	if (mixer_pointer != NULL)
	{
		delete mixer_pointer;
		mixer_pointer = NULL;
	}

	mixer_pointer = new RawDataMixerLite(_dataSetList);
	mixer_pointer->setObjName("tmpMixer");
	//robie dane
	mixer_pointer->SetuShortInputMode(true);
	mixer_pointer->UpdateMainData(_dataSetList->GetItemAtPos(0));
	//bool create_test = mixer_pointer->CreateNoInputDataSetListData(p_width, p_height, dx, dy);

	//if (create_test == false) return false;
	if (mainDataNr >= _dataSetList->GetNumberOfItems()) mainDataNr = 0;
	mixer_pointer->UpdateMainData(_dataSetList->GetItemAtPos(mainDataNr));
	bool load_test = mixer_pointer->LoadLayerSettings(p_plikKonfiguracja, false);

	if (load_test == false) return false;


	mixer_pointer->setNoInputDataSetListMode(true);//nifty trick

												   // UpdateMainData(ListItem<DataSet*>		*_mainListItem)

	return true;
}

#ifndef __GNUC__
extern "C"
#endif
int FiltDram_Initialization(const char * p_plikKonfiguracja, int p_width, int p_height, double dx, double dy, bool mode)
{
	if (mixer_pointer != NULL)
	{
		delete mixer_pointer;
		mixer_pointer = NULL;
	}

	mixer_pointer = new RawDataMixerLite(NULL);
	mixer_pointer->setObjName("tmpMixer");
	//robie dane
	mixer_pointer->SetuShortInputMode(mode);

	bool create_test = mixer_pointer->CreateNoInputDataSetListData(p_width, p_height, dx, dy);

	if (create_test == false) return false;

	bool load_test = mixer_pointer->LoadLayerSettings(p_plikKonfiguracja, false);

	if (load_test == false) return false;




	// UpdateMainData(ListItem<DataSet*>		*_mainListItem)

	return true;
}
//-------------------------------------------
#ifndef __GNUC__
extern "C"
#endif
int FiltDram_Filtruj(void * imgIN, void * imgOUT, int maxVal, double *inParamsX, int nrInParamsX, double *outParamsX, int nrOutParamsX)
{
	if ((mixer_pointer != NULL) && (mixer_pointer->GetNumberOfLayers()>0))
	{


		if (imgIN != NULL)
		{

			bool test = mixer_pointer->SetMainDataRawDataArray(imgIN);

			if (test == false) return false;

			//dane przygotowane do filtrowania
			int val;
			//filtruje
			if (maxVal == 0)
			{
				if ((mixer_pointer->GetuShortInputMode() == false))
				{
					for (int ii = 0; ii<mixer_pointer->getSliceSize(); ii++)
					{
						val = ((unsigned char*)imgIN)[ii];
						if (val>maxVal) maxVal = val;
					}
				}
				else
				{
					for (int ii = 0; ii<mixer_pointer->getSliceSize(); ii++)
					{
						val = ((unsigned short*)imgIN)[ii];
						if (val>maxVal) maxVal = val;
					}

				}
			}

			mixer_pointer->GetMainDataSet()->GetRawData()->SetMaxValue(maxVal);

		}

		unsigned short *result = mixer_pointer->SlotGenerateMixedSliceImage(0, inParamsX, nrInParamsX, outParamsX, nrOutParamsX);// zerowy przekroj bo dane kombinowane

		if (result == NULL) return false;

		if ((imgOUT != NULL))
		{

			//jesli trzeba zamieniam bufory
			if ((mixer_pointer->GetuShortInputMode() == 0))//uchar
			{
				for (int ii = 0; ii < mixer_pointer->getSliceSize(); ii++)
				{
					((unsigned char*)imgOUT)[ii] = result[ii];
				}
			}
			else
			{
				memcpy(imgOUT, result, mixer_pointer->getSliceSize() * sizeof(unsigned short));
			}
		}
		return true;

	}


	return 0;
}
//-------------------------------------------
#ifndef __GNUC__
extern "C"
#endif
int FiltDram_Free(void)
{
	if (mixer_pointer != NULL)
	{
		delete mixer_pointer;
		mixer_pointer = NULL;
		return true;
	}
	return 0;
}
//----------------------------------------------
#ifndef __GNUC__
extern "C"
#endif
int FiltDram_Free2(void)
{
	if (mixer_pointer != NULL)
	{
		mixer_pointer->setNoInputDataSetListMode(false);//nifty trick
		delete mixer_pointer;
		mixer_pointer = NULL;
		return true;
	}
	return 0;
}
//----------------------------------------------
#ifndef __GNUC__
extern "C"
#endif 
int FiltDram_CurrentSliceSize(void)
{

	if (mixer_pointer != NULL)
	{
		return mixer_pointer->getSliceSize();
	}
	return 0;

}

bool FilterDataSet(RawDataSet *input, RawDataSet *output, QString filterName)
{
	//char *parameterFileFilterModel =filterName;// = "filter_compositions/regFilterPreprocessing.layers";
	LinkedList<DataSet*>	*dataSetList = new LinkedList<DataSet*>;

	dataSetList->InsertItem(new DataSet(input, NULL, 1));

	bool init = FiltDram_Initialization2(filterName.toLocal8Bit(), dataSetList, 0);//


																				   //RawDataSet *filteredModel = new RawDataSet();

																				   //	filteredModel->Copy(*mSliceData, false);
	int result = FiltDram_Filtruj(input->GetDataArray(), output->GetDataArray());


	dataSetList->GetItemAtPos(0)->SetData(NULL);
	delete dataSetList;
	FiltDram_Free2();
	return result;
	//filteredModel->SaveRawDataSet2("f:\\data\\x\\test");
}
//--------------------------------
void		RawDataMixerLite::SetFilterParamBasedOnNames(QString filterName0, QString paramName0, double val)
{
	RawDataMixerLite *mixer = this;
	if ((mixer != NULL) && (mixer->GetNumberOfLayers() > 1))
	{



		LinkedList <RawDataLayer *>		*rawDataLayerList = mixer->GetLayerList();
		LinkedList <LayerMaskFilter*>		*layerMaskFilterList = mixer->GetLayerMaskFilterList();

		LinkedList <LayerFilter*>		*layerFilterList = mixer->GetLayerFilterList();


		int nrLayers = mixer->GetNumberOfLayers();

		RawDataLayer *layer;
		int filterMode;
		LayerMaskFilter *layerMaskFilter;
		LayerFilter *layerFilter;
		int i;
		for (i = 1; i < nrLayers; i++)
		{
			layer = rawDataLayerList->GetItemAtPos(i)->GetData();
			filterMode = layer->GetFilterMode();
			//layerMaskFilter = layerMaskFilterList->GetItemAtPos(filterMode)->GetData();
			layerFilter = layerFilterList->GetItemAtPos(filterMode)->GetData();
			QString filterName = layerFilter->GetFilterName();

			if (filterName.contains(filterName0) == true)
			{
				break;
			}
		}

		if (i < nrLayers)
		{
			int j;
			for (j = 0; j < layerFilter->GetNumberOfParams(); j++)
			{
				QString paramName = layerFilter->GetParamName(j);
				if (paramName.compare(paramName0) == 0) break;

			}

			if (j < layerFilter->GetNumberOfParams())
			{


				layer->SetFilterParam(j, val);
				//SlotRepaintViews();

				//layer->GetMaskFilterParam(j);
				//->tunningThicknessFilterSlider->setValue(val * 1000);
				//GUIPanel->tunningLabel->setText(QString::number(val));

			}
		}

	}
}