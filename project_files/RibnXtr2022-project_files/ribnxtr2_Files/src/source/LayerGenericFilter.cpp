#include "LayerGenericFilter.h"

LayerGenericFilter::LayerGenericFilter()
{
#pragma omp parallel
	nrOfThreads = omp_get_num_threads();
	//maxValue = 0;

	numberOfParams = 0;
	globalLeftThreshold = 0;
	globalRightThreshold = 0;
	//nrOfShowTexts=0;
	this->filterName = "";

	for (int i = 0; i<PARAMS_NR; i++)
	{
		defaultParams[i] = 0;
		this->paramsNames[i] = "";
		viewMode[i] = 0;
	}

	for (int i = 0; i<PARAMS_NR * 5; i++)
	{
		// showTextStartStop[i] = -1;
	}
	mdx = 1; mdy = 1;
	mainMask = NULL;
	mainMask2 = NULL;
	viewType = 0;
	inParams = NULL;
	outParams = NULL;
	nrInParams = 0;
	nrOutParams = 0;
	mainDataMaxValue = 0;

	shaFunc = NULL;
	higFunc = NULL;
	midFunc = NULL;
	funSize = 0;
	brightnessFilterMode = 0;

}
//------------------------------------
LayerGenericFilter::~LayerGenericFilter()
{

}

void		LayerGenericFilter::SetMainMaskData(unsigned char *mask, int x, int y)
{
	mainMask = mask;
	position[0] = x;
	position[1] = y;
}
//---------------------------------
void		LayerGenericFilter::SetMainMaskData2(unsigned char* mask)
{
	mainMask2 = mask;

}


//---------------------------------------------------

bool	LayerGenericFilter::GetSliderParams(int nr, double sParams[3])
{
	if ((nr >= 0) && (nr<PARAMS_NR))
	{

		if (viewMode[nr]>0) //slider
		{
			sParams[0] = this->minVals[nr];
			sParams[1] = this->maxVals[nr];
			sParams[2] = this->steps[nr];

			return true;
		}

	}

	return false;
}
//-------------------------

void	LayerGenericFilter::AddParam(QString name, double defVal, int mode, double min, double max, double step, int &paramID, QStringList *names)
{
	if (numberOfParams<PARAMS_NR)
	{
		paramsNames[numberOfParams] = name;
		defaultParams[numberOfParams] = defVal;
		viewMode[numberOfParams] = mode;
		this->minVals[numberOfParams] = min;
		this->maxVals[numberOfParams] = max;
		this->steps[numberOfParams] = step;
		 


		if (names != NULL)
		{
			if ((((int)step) == 1) && (((int)(max - min + 1)) == names->size()))
			{
				//showTextStartStop[numberOfParams*2]=showTextList.size();
				for (int i = 0; i<names->size(); i++)
				{
					showTextList[numberOfParams] << names->at(i);
					//nrOfShowTexts++
				}
				//showTextStartStop[numberOfParams*2+1]=showTextList.size()-1;
			}
		}

		paramID = numberOfParams;
		numberOfParams++;
	}
	else
		paramID = -1;
}
//-------------------------------------------------------------------
//void	LayerGenericFilter::GetShowTextStartStop(int index,int tab[2])
//{
//
//	if ((index>=0)&&(index <PARAMS_NR))
//	{
//		tab[0]=showTextStartStop[index*2];
//		tab[1]=showTextStartStop[index*2+1];
//	}
//
//}
//---------------------------------------------------------
QString		LayerGenericFilter::GetShowText(int nr, int pos)
{
	if ((pos >= 0) && (pos<showTextList[nr].size()))
	{
		return showTextList[nr].at(pos);
	}
	return "";
}
//-------------------------------------------------------------------
