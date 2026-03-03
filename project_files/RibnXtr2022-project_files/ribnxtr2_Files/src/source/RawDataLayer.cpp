#include "RawDataLayer.h"

RawDataLayer::RawDataLayer(LinkedList <LayerMaskFilter*> *_layerMaskFilterList, LinkedList <LayerFilter*> *_layerFilterList)
{



	mainFilterNames << "Opacity" << "LayerEnable" << "LayerSourceMixed" << "Modes" << "FilterEnable" << "FilterInverse" << "Multiplier" << "Addition" << "AutoNormalize" << "MaskFilterEnable" << "BrightnessFilterMode" << "RememberBuffer";
	double tt[] = { 1,			1,					0,				0,				1,				0,					1,				0,			1,						0 ,				0,						0 };
	//enum  mainIDS{ OpacityID		,LayerEnableID ,	LayerSourceMixedID, ModesID,  FilterEnableID,   FilterInverseID,  MultiplierID,   AdditionID,   AutoNormalizeID,   MaskFilterEnableID, mixerFilterBrightnessModeID,BrightnessFilterModeID }
	for (int i = 0; i < mainFilterNames.count(); i++)
	{
		defaultMainParams[i] = tt[i];
	}


	layerMaskFilterList = _layerMaskFilterList;
	layerFilterList = _layerFilterList;
	//	isMaskInverted=0;
	dataSetNumber = 0;
	maskNumber = 0;// zakladam ze maska ma byc brana z filtra danej warstwy na poczatku
				   //nx=0,ny=0;
				   //mainDataSize=nx*ny;
	filterBufferData[0] = NULL; filterBufferData[1] = NULL; filterBufferData[2] = NULL;
	//filterBuffer=new unsigned short[mainDataSize*sizeof(unsigned short)];
	maskBuffer[0] = NULL; maskBuffer[1] = NULL; maskBuffer[2] = NULL;

	isChild = false;
	isMixerMode = false;
	this->treeItem = NULL;


	for (int i = 0; i < NR_FILTER_PARAMS; i++) { mainParams[i] = defaultMainParams[i];	this->mainParamForGui[i] = false; };
	for (int i = 0; i < NR_FILTER_PARAMS; i++) { maskFilterParams[i] = 0;	this->maskFilterParamForGui[i] = false; };
	maskFilterMode = 0;
	for (int i = 0; i < NR_FILTER_PARAMS; i++) { filterParams[i] = 0;	this->filterParamForGui[i] = false; }
	filterMode = 0;

	for (int i = 0; i < NR_FILTER_PARAMS*3; i++) 
	{
		mainParamsColors[i] = -1;
		filterParamsParamsColors[i] = -1;
		maskFilterParamsColors[i] = -1;
	}
	


}
//----------------------------------
RawDataLayer::RawDataLayer(LinkedList <LayerMaskFilter*> *_layerMaskFilterList, LinkedList <LayerFilter*> *_layerFilterList, int _nx, int _ny, int _nz, int nr)
{
	mainFilterNames << "Opacity" << "LayerEnable" << "LayerSourceMixed" << "Modes" << "FilterEnable" << "FilterInverse" << "Multiplier" << "Addition" << "AutoNormalize" << "MaskFilterEnable" << "BrightnessFilterMode" << "RememberBuffer";
	double tt[] = { 1,			1,					0,				0,				1,				0,					1,				0,			1,						0 ,				0,						0 };
	//enum  mainIDS{ OpacityID		,LayerEnableID ,	LayerSourceMixedID, ModesID,  FilterEnableID,   FilterInverseID,  MultiplierID,   AdditionID,   AutoNormalizeID,   MaskFilterEnableID, mixerFilterBrightnessModeID,BrightnessFilterModeID }
	for (int i = 0; i < mainFilterNames.count(); i++)
	{
		defaultMainParams[i] = tt[i];
	}

	layerMaskFilterList = _layerMaskFilterList;
	layerFilterList = _layerFilterList;


	dataSetNumber = nr;
	maskNumber = 0;// zakladam ze maska ma byc brana z filtra danej warstwy na poczatku
				   //filterBufferData=NULL;
	filterBufferData[0] = NULL; filterBufferData[1] = NULL; filterBufferData[2] = NULL;
	//filterBuffer=new unsigned short[mainDataSize*sizeof(unsigned short)];
	maskBuffer[0] = NULL; maskBuffer[1] = NULL; maskBuffer[2] = NULL;
	CreateBuffers(_nx, _ny, _nz);




	//filterBuffer=new unsigned short[mainDataSize*sizeof(unsigned short)];

	//isMaskInverted=0;
	isChild = false;
	isMixerMode = false;
	/*blendingMode=mode;
	blendingOpacity=opacity;
	blendingMultiplier=1.0;
	blendingAddition=0.0;*/

	this->treeItem = NULL;


	for (int i = 0; i < NR_FILTER_PARAMS; i++) { mainParams[i] = defaultMainParams[i];	this->mainParamForGui[i] = false; };
	for (int i = 0; i < NR_FILTER_PARAMS; i++) { maskFilterParams[i] = 0;	this->maskFilterParamForGui[i] = false; };

	for (int i = 0; i < NR_FILTER_PARAMS; i++) { filterParams[i] = 0;	this->filterParamForGui[i] = false; }

	for (int i = 0; i < NR_FILTER_PARAMS * 3; i++)
	{
		mainParamsColors[i] = -1;
		filterParamsParamsColors[i] = -1;
		maskFilterParamsColors[i] = -1;
	}


	filterMode = 0;
	maskFilterMode = 0;


}
//----------------------------------
void RawDataLayer::CreateBuffers(int _nx, int _ny, int _nz)
{

	int n[3] = { _nx, _ny, _nz };
	int nX, nY, nZ;
	int mainDataSize;

	for (int viewType = 0; viewType < 3; viewType++)
	{
		switch (viewType)
		{
		case 0:
			nX = n[0]; nY = n[1]; nZ = n[2];

			break;


		case 1:

			nX = n[0]; nY = n[2]; nZ = n[1];

			break;

		case 2:
			nX = n[1]; nY = n[2]; nZ = n[0];
			break;
		}
		mainDataSize = nX*nY;
		if (filterBufferData[viewType] != NULL) delete filterBufferData[viewType];
		filterBufferData[viewType] = new RawDataSet(nX, nY, 1);
		if (maskBuffer[viewType] != NULL) delete[]maskBuffer[viewType];
		maskBuffer[viewType] = new unsigned char[mainDataSize * sizeof(unsigned char)];
		memset(maskBuffer[viewType], 255, mainDataSize * sizeof(unsigned char));


	}






	//nx=_nx,ny=_ny;
	//mainDataSize=nx*ny;
	//if  (filterBufferData!=NULL) delete filterBufferData;
	//filterBufferData=new RawDataSet (nx,ny,1);
	//if  (maskBuffer!=NULL) delete []maskBuffer;
	//maskBuffer=new unsigned char[mainDataSize*sizeof(unsigned char)];
}
//------------------------------------
RawDataLayer::~RawDataLayer()
{
	delete treeItem;
	for (int viewType = 0; viewType < 3; viewType++)
	{
		if (filterBufferData[viewType] != NULL) delete filterBufferData[viewType];
		if (maskBuffer[viewType] != NULL) delete[]maskBuffer[viewType];
	}
}
//---------------------------------
//void	RawDataLayer::SetMaskEnabled(bool t)
//{
//	isMaskEnabled=t;
//};
////------------------------------------		
//void	RawDataLayer::SetFilterEnabled(bool t)
//{
//	isFilterEnabled=t;
//};
//--------------------------------------		
void		RawDataLayer::writeLayerParams(QTextStream &stream)
{

	LayerFilter *lf = layerFilterList->GetItemAtPos(this->filterMode)->GetData();
	LayerMaskFilter *mf = layerMaskFilterList->GetItemAtPos(this->maskFilterMode)->GetData();

	stream << "\n--------------------------------------------\n";
	writeParam(stream, "LAYER_BEGIN", "\n");
	writeParam(stream, "LAYER_NAME", name);


	//	writeParam(stream,"LAYER_ENABLED",				QString::number((this->isLayerEnabled)));
	writeParam(stream, "CHILD_MODE", QString::number((this->isChild)));
	//	writeParam(stream,"MIXER_MODE",					QString::number((this->isMixerMode)));


	writeParam(stream, "DATASET_NUMBER", QString::number((dataSetNumber)));

	for (int i = 0; i < mainFilterNames.count(); i++)
	{
		if (mainParamsColors[i*3] < 0)
		writeParam(stream, "main_" + mainFilterNames[i], QString::number(mainParams[i]), QString::number(mainParamForGui[i]));
		else
			writeParam(stream, "main_" + mainFilterNames[i], QString::number(mainParams[i]), QString::number(mainParamForGui[i]), QString::number(mainParamsColors[i * 3]), QString::number(mainParamsColors[i * 3+1]), QString::number(mainParamsColors[i * 3+2]));
	}

	writeParam(stream, "LAYER_FILTER_NAME", lf->GetFilterName());
	//stream <<	"\n";
	for (int i = 0; i<lf->GetNumberOfParams(); i++)
	{
		//writeParam(stream, "prm_" + lf->GetParamName(i), QString::number(filterParams[i]), QString::number(filterParamForGui[i]));

		if (filterParamsParamsColors[i * 3] <0)
			writeParam(stream, "prm_" + lf->GetParamName(i), QString::number(filterParams[i]), QString::number(filterParamForGui[i]));
		else
			writeParam(stream, "prm_" + lf->GetParamName(i), QString::number(filterParams[i]), QString::number(filterParamForGui[i]),
				QString::number(filterParamsParamsColors[i * 3]), QString::number(filterParamsParamsColors[i * 3 + 1]), QString::number(filterParamsParamsColors[i * 3 + 2]));

	}
	stream << "\n";

	writeParam(stream, "MASK_NUMBER", QString::number((maskNumber)));
	writeParam(stream, "LAYER_MASK_FILTER_NAME", mf->GetFilterName());
	for (int i = 0; i<mf->GetNumberOfParams(); i++)
	{
		//writeParam(stream, "prm_" + mf->GetParamName(i), QString::number(maskFilterParams[i]), QString::number(maskFilterParamForGui[i]));

		if (maskFilterParamsColors[i * 3]  <0)
			writeParam(stream, "prm_" + mf->GetParamName(i), QString::number(maskFilterParams[i]), QString::number(maskFilterParamForGui[i]));
		else
			writeParam(stream, "prm_" + mf->GetParamName(i), QString::number(maskFilterParams[i]), QString::number(maskFilterParamForGui[i]),
				QString::number(maskFilterParamsColors[i * 3]), QString::number(maskFilterParamsColors[i * 3 + 1]), QString::number(maskFilterParamsColors[i * 3 + 2]));

	}

	writeParam(stream, "LAYER_END", "\n");

	stream << "\n";



}
//----------------------------------------
//-----------------------------------------------------------
bool		RawDataLayer::readLayerParams(QTextStream &stream)
{
	QString line;
	QStringList fields;
	bool test = false;
	bool test1 = false;
	int count;
	QString title, value;
	bool nxSet = false;
	bool nySet = false;
	bool readNextLine = true;
	bool layerTest = false;
	bool strEnd = false;
	do
	{
		if (readNextLine == true)		line = stream.readLine(); else readNextLine = true;

		fields = line.split(": ", QString::SkipEmptyParts);
		count = fields.count();
		if (count > 0)
			title = fields[0];
		else
			title = "";

		if (count >= 3)
		{
			//value = fields[1];
			if ((title.mid(0, 5) == "main_") && (count >= 2))
			{

				QString paramName = title.mid(5, title.length() - 5);
				for (int ii = 0; ii < mainFilterNames.count(); ii++)
				{
					if (compareText(paramName, mainFilterNames[ii]))
					{
						mainParams[ii] = fields[1].toDouble();

						if (count >= 3)
						{
							mainParamForGui[ii] = fields[2].toInt();
						}
						if (count >= 6)
						{
							mainParamsColors[ii * 3] = fields[3].toInt();
							mainParamsColors[ii * 3+1] = fields[4].toInt();
							mainParamsColors[ii * 3+2] = fields[5].toInt();
						}


						break;
					}
				}
			}

		}
		else
			if (count == 2)
			{
				value = fields[1];
				if (compareText(title, "LAYER_NAME")) { this->name = value; }
				else
					if (compareText(title, "CHILD_MODE")) { this->isChild = value.toInt(); }
					else
						if (compareText(title, "DATASET_NUMBER")) { this->dataSetNumber = value.toInt(); }
						else
							if (compareText(title, "LAYER_FILTER_NAME"))
							{

								for (int i = 0; i<layerFilterList->GetNumberOfItems(); i++)
								{
									LayerFilter *layerFilter = layerFilterList->GetItemAtPos(i)->GetData();

									//qWarning() << "layerName:" << value << " ToCompare:" << layerFilter->GetFilterName();
									if (compareText(value, layerFilter->GetFilterName()))
									{
										//przeczytaj wszystkie parametry
										layerTest = true;
										test1 = true;
										int posx = 0;

										//setupdefaults
										for (int ii = 0; ii<layerFilter->GetNumberOfParams(); ii++)
										{
											filterParams[ii] = layerFilter->GetDefaultParam(ii);
										}

										do
										{
											line = stream.readLine();					fields = line.split(":", QString::SkipEmptyParts);					count = fields.count();	if (count>0) title = fields[0];
											if ((title.mid(0, 4) == "prm_") && (count >= 2))
											{
												//sprawdze czy istnieje parametr o takiej nazwie i wpisze go we wlasciwe miesjce

												//						filterParams[posx++]=fields[1].toDouble();
												test1 = true;
												QString paramName = title.mid(4, title.length() - 4);
												for (int ii = 0; ii<layerFilter->GetNumberOfParams(); ii++)
												{
													if (compareText(paramName, layerFilter->GetParamName(ii)))
													{
														filterParams[ii] = fields[1].toDouble();

														if (count >= 3)
														{
															filterParamForGui[ii] = fields[2].toInt();
														}
														if (count >= 6)
														{
															filterParamsParamsColors[ii * 3] = fields[3].toInt();
															filterParamsParamsColors[ii * 3 + 1] = fields[4].toInt();
															filterParamsParamsColors[ii * 3 + 2] = fields[5].toInt();
														}


														break;
													}
												}

											}
											else
											{
												test1 = false;						readNextLine = false;
											}
										} while (test1 == true);

										filterMode = i;
										break;
									}
								}


							}
							else


								//if	(compareText(title,"MASK_ENABLED"))				{			this->isMaskEnabled=value.toInt();									}else
								if (compareText(title, "MASK_NUMBER")) { this->maskNumber = value.toInt(); }
								else
									//if	(compareText(title,"INVERSE_MASK"))				{			this->isMaskInverted=value.toInt();									}else

									//if	(compareText(title,"LAYER_FILTER_ENABLED"))				{			this->isFilterEnabled=value.toInt();									}else



									if (compareText(title, "LAYER_MASK_FILTER_NAME"))
									{
										for (int i = 0; i<layerFilterList->GetNumberOfItems(); i++)
										{
											LayerMaskFilter *layerMaskFilter = layerMaskFilterList->GetItemAtPos(i)->GetData();
											if (compareText(value, layerMaskFilter->GetFilterName()))
											{
												//przeczytaj wszystkie parametry
												test1 = true;
												int posx = 0;

												for (int ii = 0; ii<layerMaskFilter->GetNumberOfParams(); ii++)
												{
													maskFilterParams[ii] = layerMaskFilter->GetDefaultParam(ii);
												}
												do
												{
													line = stream.readLine();					fields = line.split(":", QString::SkipEmptyParts);					count = fields.count();	if (count>0) title = fields[0];
													if ((title.mid(0, 4) == "prm_") && (count >= 2))
													{
														test1 = true;					//		maskFilterParams[posx++]=fields[1].toDouble();
														QString paramName = title.mid(4, title.length() - 4);
														for (int ii = 0; ii<layerMaskFilter->GetNumberOfParams(); ii++)
														{
															if (compareText(paramName, layerMaskFilter->GetParamName(ii)))
															{
																maskFilterParams[ii] = fields[1].toDouble();
																if (count >= 3)
																{
																	maskFilterParamForGui[ii] = fields[2].toInt();
																}
																if (count >= 6)
																{
																	maskFilterParamsColors[ii * 3] = fields[3].toInt();
																	maskFilterParamsColors[ii * 3 + 1] = fields[4].toInt();
																	maskFilterParamsColors[ii * 3 + 2] = fields[5].toInt();
																}

																break;
															}
														}
													}
													else
													{
														test1 = false;						readNextLine = false;
													}
												} while (test1 == true);

												maskFilterMode = i;
												break;
											}
										}

									}

			}

		if (count>0)
			test = (fields[0].compare("LAYER_END") == 0);

		strEnd = stream.atEnd();
		if (strEnd == true)
		{
			strEnd;
		}
		/*if (count > 0)
		{

		qWarning() << line ;
		}
		else
		{
		qWarning() << "empty";
		}*/
	} while ((!strEnd) && (test == false));

	//if ((nxSet==true)&&(nySet==true)){	
	//	this->CreateBuffers(nx,ny);
	//	return true;}	else	return false;
	return layerTest;
	//return true;

}