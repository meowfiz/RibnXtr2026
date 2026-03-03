#include "LayerFilters.h"

LayerFilter::LayerFilter()
{
	//#pragma omp parallel
	//nrOfThreads = omp_get_num_threads();
	//maxValue=0;
	////shapes=NULL;
	////visImage=NULL;
	////this->iWidget=NULL;
	//numberOfParams=0;
	//globalLeftThreshold=0;
	//globalRightThreshold=0;
	////nrOfShowTexts=0;
	//this->filterName="";

	//for(int i=0;i<PARAMS_NR;i++)
	//{
	//	defaultParams[i]=0; 
	//	this->paramsNames[i]="";
	//}

	//for(int i=0;i<PARAMS_NR*5;i++)
	//{
	//	showTextStartStop[i]=-1;
	//}
	//mdx=1;mdy=1;
	//mainMask=NULL;

	//viewType = 0;
	//inParams = NULL;
	//outParams = NULL;
	//nrInParams = 0;
	//nrOutParams = 0;
	//mainDataMaxValue = 0;
}
//------------------------------------
LayerFilter::~LayerFilter()
{

}

//void		LayerFilter::SetMainMaskData(unsigned char *mask, int x,int y)
//{
//	mainMask=mask;
//	position[0]=x;
//	position[1]=y;
//}
//---------------------------------

//void	LayerFilter::SetiWidget(ImageWidget *s)
//{
//	if(s!=NULL)
//	{
//		iWidget=s;
//		visImage=s->GetCurrentImage();
//	}	
//};


//---------------------------------------------------

//bool	LayerFilter::GetSliderParams (int nr,double sParams[3])
//{
//	if ((nr>=0)&&(nr<PARAMS_NR))
//	{
//
//		if (paramsSliders[nr]==true)
//		{
//			sParams[0]=this->minVals[nr];
//			sParams[1]=this->maxVals[nr];
//			sParams[2]=this->steps[nr];
//
//			return true;
//		}
//
//	}
//
//	return false;
//}
//----------------------------------------------
void		LayerFilter::ApplyWeightMap(unsigned short *inTab, unsigned short *filter, unsigned short *outTab, unsigned short *weightMap, unsigned short maxWeightVal, int size, double amount, bool invertWeights)
{

	double multiply = 1.0 / maxWeightVal;
	//	double filterResult;
	int res;
	double alpha;
	double alpha_mod;
	double diff;
	double weight;
	if (invertWeights == false)
	{
		for (int i = 0; i<size; i++)
		{
			//	alpha=weightMap[i]*multiply*amount;
			//

			//	diff=filter[i]-inTab[i];

			//	res=inTab[i]+alpha*diff;


			//filterResult=filter[i]-inTab[i];

			weight = weightMap[i] * multiply; //0-1
			alpha_mod = 1.0 - weight*amount;

			res = inTab[i] * (1 - alpha_mod) + filter[i] * alpha_mod;

			//res=inTab[i]*(1-alpha)+filter[i]*alpha;
			//if (res<0) res=0; else if (res>255) res=255;
			outTab[i] = res; //weightMap[i]*amount - zakres 0-1 *amount
		}
	}
	else
	{
		for (int i = 0; i<size; i++)
		{

			weight = 1.0 - weightMap[i] * multiply; //0-1
			alpha_mod = 1.0 - weight*amount;

			res = inTab[i] * (1 - alpha_mod) + filter[i] * alpha_mod;

			/*alpha=(maxWeightVal-weightMap[i])*multiply*amount;

			diff=filter[i]-inTab[i];

			res=inTab[i]+alpha*diff;*/

			//filterResult=filter[i]-inTab[i];

			//res=inTab[i]*(1-alpha)+filter[i]*alpha;
			//if (res<0) res=0; else if (res>255) res=255;
			outTab[i] = res; //weightMap[i]*amount - zakres 0-1 *amount
		}
	}

}
//-------------------------
void	LayerFilter::invertData(unsigned short *data, int size, int maxValue)
{


	for (int i = 0; i<size; i++)
	{
		data[i] = maxValue - data[i];
	}

}

void		LayerFilter::updateBrightness(unsigned short *input, unsigned short *data, int size, int maxValue, bool invData, int bMode)
{

	//cout << "bmode: " << bMode << endl;
	if (bMode > 0)
	{

		int maxVal = maxValue;
		int val;
		//highlights
		double scaler = 65535.0 / mainDataMaxValue;

		//	int tt[3] = { highlights,shadows,midtones };
		double *ttTab[3];// = { higFunc,shaFunc,midFunc };
		ttTab[0] = higFunc;	ttTab[1] = shaFunc;	ttTab[2] = midFunc;

		double *func = ttTab[bMode];
		if (func != NULL)
			if (bMode == 1)
			{
				for (int i = 0; i < sliceSize; i++)
				{
					int vi = scaler*input[i];
					val = input[i] * (1 - func[vi]) + data[i] * func[vi]; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
					data[i] = val;
				}

			}

	
	}


	if (invData == true)
	{
		for (int i = 0; i < size; i++)
		{
			data[i] = maxValue - data[i];
		}
	}


}

//----------------------------------


//
//void	LayerFilter::AddParam(QString name, double defVal, bool slider, double min, double max, double step, int &paramID,QStringList *names)
//{
//	if (numberOfParams<PARAMS_NR)
//	{
//		paramsNames[numberOfParams]=name;
//		defaultParams[numberOfParams]=defVal;
//		paramsSliders[numberOfParams]=slider;
//		this->minVals[numberOfParams]=min;
//		this->maxVals[numberOfParams]=max;
//		this->steps[numberOfParams]=step;
//
//
//
//		if (names!=NULL)
//		{
//			if (	(((int)step)==1)&& (((int)(max-min+1))==names->size()))
//			{
//				showTextStartStop[numberOfParams*2]=showTextList.size();
//				for(int i=0;i<names->size();i++)
//				{
//					showTextList << names->at(i);
//					//nrOfShowTexts++
//				}
//				showTextStartStop[numberOfParams*2+1]=showTextList.size()-1;
//			}
//		}
//
//		paramID=numberOfParams;
//		numberOfParams++;
//	}
//	else
//		paramID=-1;
//}
////-------------------------------------------------------------------
//void	LayerFilter::GetShowTextStartStop(int index,int tab[2])
//{
//
//	if ((index>=0)&&(index <PARAMS_NR))
//	{
//		tab[0]=showTextStartStop[index*2];
//		tab[1]=showTextStartStop[index*2+1];
//	}
//
//}
////---------------------------------------------------------
//QString		LayerFilter::GetShowText(int pos)
//{
//	if ((pos>=0)&&(pos<showTextList.size()))
//	{
//		return showTextList.at(pos);
//	}
//	return "";
//}
//-------------------------------------------------------------------
LayerEmptyFilter::LayerEmptyFilter()
{
	filterName = "EmptyFilter";
	//numberOfParams=0;
	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	
}
//------------------------------------
int	LayerEmptyFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	
	


		double scale = 1;
		double maxv = mainDataMaxValue;


		for (int i = 0; i<sliceSize; i++)
		{
			usData0[i] = tab[i];
		}

		updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);



		return true;
	

	return false;
}
//----------------------------------
LayerEmptyResampleFilter::LayerEmptyResampleFilter()
{
	filterName = "LayerEmptyResampleFilter";
	//numberOfParams=0;
	AddParam("nX", 256, true, 1, 2048, 1, nxID);
	AddParam("nY", 256, true, 1, 2048, 1, nyID);
	AddParam("nZ", 256, true, 1, 2048, 1, nzID);
	QStringList list;
	list.clear(); list << "nearest" << "linear" << "cosine" << "cubic4" << "spline6" << "catmulRom" << "hermite" << "HermiteLaurent" << "WelchSinc" << "HanHammingSinc" << "Lanczos";
	AddParam("interpolation", 0, true, 0, 10, 1, modeID, &list);

	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	
}
//------------------------------------
int	LayerEmptyResampleFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{



	QStringList list;
	{

		nx = params[nxID];
		ny = params[nyID];
		nz = params[nzID];
		mode = params[modeID];

		for (int i = 0; i<sliceSize; i++)
		{
			usData0[i] = tab[i];
		}
		double maxv = mainDataMaxValue;
		updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);



		return true;
	}

	return false;
}
//----------------------------------
LayerAbsUcharFilter::LayerAbsUcharFilter()
{
	filterName = "AbsUcharFilter";





	AddParam("centerValue", 128, true, 0, 255, 1, centerValID);
	QStringList list;
	list.clear(); list << "false" << "true";
	AddParam("rescale", 0, true, 0, 1, 1, rescaleID, &list);


}
//------------------------------------
int	LayerAbsUcharFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	{

		/*	if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
		unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;*/

		bool createUcharBuffers = ((src != NULL) && (dest != NULL)) ? false : true;
		if (createUcharBuffers == true)
		{
			src = new unsigned char[mnx*mny];
		}


		double scale = 1;
		double maxv = mainDataMaxValue;
		if (maxv>0) { if (maxv>256) { scale = 255.0 / maxv; } }
		double scale1 = 1.0 / scale;


		bool rescale = params[rescaleID];
		unsigned char centerVal = params[centerValID];

		if (rescale == false)
		{
			for (int i = 0; i<sliceSize; i++)
			{
				usData0[i] = (unsigned char)(abs(src[i] - centerVal))*scale1;
			}
		}
		else
		{
			double scale = (centerVal>0) ? 255.0 / centerVal : 1;
			for (int i = 0; i<sliceSize; i++)
			{
				usData0[i] = (unsigned char)(abs(src[i] - centerVal))*scale*scale1;
			}
		}

		updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

		if (createUcharBuffers == true)
		{
			delete[]src;

		}

		return true;
	}

	return false;
}
//----------------------------------
LayerAbsUShortFilter::LayerAbsUShortFilter()
{
	filterName = "AbsUShortFilter";




	AddParam("centerValue", 50, true, 0, 100, 0.01, centerValID);
	QStringList list;
	list.clear(); list << "false" << "true";
	//AddParam("rescale", 0,true,0,1,1,rescaleID,&list);


}
//------------------------------------
int	LayerAbsUShortFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	//if(((inverse==true)))
	{



		unsigned short *buff1 = usData1;
		unsigned short *tmpBuff = tab;



		//	bool rescale=params[rescaleID];
		unsigned short centerVal = params[centerValID] * 0.01*mainDataMaxValue;

		//if (rescale==false)
		{
			for (int i = 0; i<sliceSize; i++)
			{
				usData0[i] = (unsigned short)(abs(tmpBuff[i] - centerVal));
			}
		}
		//else
		//{
		//	//double scale=(centerVal>0)?maxv/centerVal:1;
		//	for(int i=0;i<sliceSize;i++)
		//	{
		//		usData0[i] = (unsigned short)(abs(tmpBuff[i] - centerVal));// *scale;
		//	}
		//}

		updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);



		return true;
	}

	return false;
}
//----------------------------------
LayerDistanceTransformFilter::~LayerDistanceTransformFilter()
{
	if (tempDistanceVectorArrayX != NULL) delete[]tempDistanceVectorArrayX;
	if (tempDistanceVectorArrayY != NULL) delete[]tempDistanceVectorArrayY;


	if (sqtab_X != NULL) delete[]sqtab_X;
	if (sqtab_Y != NULL) delete[]sqtab_Y;
	if (sqtabMX != NULL) delete[]sqtabMX;
	if (sqtabMY != NULL) delete[]sqtabMY;
	if (sqtabPX != NULL) delete[]sqtabPX;
	if (sqtabPY != NULL) delete[]sqtabPY;
	if (sqrtTabNorm != NULL) delete[]sqrtTabNorm;

}
//---------------------------------------------------------
LayerDistanceTransformFilter::LayerDistanceTransformFilter()
{

	filterName = "DistanceTransformFilter";
	nx = 0;
	ny = 0;
	n2x = 0;
	n2y = 0;
	ss = 0;
	ss2 = 0;
	infinity = 0;
	infp1 = 0;

	tempDistanceVectorArrayX = NULL;
	tempDistanceVectorArrayY = NULL;

	sqtab_X = NULL;
	sqtab_Y = NULL;
	sqtabMX = NULL;
	sqtabMY = NULL;
	sqtabPX = NULL;
	sqtabPY = NULL;
	sqrtTabNorm = NULL;

	//numberOfParams=1;
	QStringList list;

	AddParam("scaleFactor", 1, true, 0.01, 100, 0.01, scaleFactorID);
	AddParam("thresholdLeft", 0, true, 0, 100, 0.001, thresholdLeftID);
	AddParam("thresholdRight", 100, true, 0, 100, 0.001, thresholdRightID);
	list.clear(); list << "false" << "true";
	AddParam("inverseThresh", 0, true, 0, 1, 1, inverseThreshID, &list);
	AddParam("fillGapsFromDistance", 0, true, 0, 1, 1, fillGapsFromDistanceID, &list);
	AddParam("use Mask Settings", 0, true, 0, 1, 1, useMaskSettingsID, &list);
	//AddParam("use Prev Layer", 0,true,0,1,1,prevLayerID,&list);

	AddParam("test", 0, true, 0, 1, 1, testID, &list);


}
//----------------------------------------------------
void LayerDistanceTransformFilter::CalculateDistanceArrays2D()
{
	CalculateForwardUp2D();
	CalculateBackwardDown2D();

}
//----------------------------------------------------
void LayerDistanceTransformFilter::CalculateForwardUp2D()
{


	//	FILE *Do_opa;

	//forward up pass of distance map calculation
	//short *tX0,*tX1,*tX2,*tX4;
	//short *tY0,*tY1,*tY2,*tY4;

	//int yT,zT;
	//int t0,t1,t2,t4;
	//zT=0;
	//yT=0;

	//	for (z=1;z<=Nz;z++)
	//	{
	//
	//	zT+=ss2;
	///yT=0;
#pragma omp  for  nowait ordered schedule (guided)
	for (int y = 1; y <= ny; y++)
	{

		double a, b, c, min;
		short *tX0, *tX1, *tX2, *tX4;
		short *tY0, *tY1, *tY2, *tY4;

		int yT, zT;
		int t0, t1, t2, t4;

		yT = n2x*y;

		//yT+=n2x;

		t0 = yT;			//(x,y,z)
		t1 = t0 - 1;			//(x-1,y,z)
		t2 = t0 - n2x;		//(x,y-1,z)
		t4 = t0 + nx + 1;		//(x+1,y,z)

		tX0 = tempDistanceVectorArrayX + t0; tX1 = tempDistanceVectorArrayX + t1; tX2 = tempDistanceVectorArrayX + t2; tX4 = tempDistanceVectorArrayX + t4;
		tY0 = tempDistanceVectorArrayY + t0; tY1 = tempDistanceVectorArrayY + t1; tY2 = tempDistanceVectorArrayY + t2; tY4 = tempDistanceVectorArrayY + t4;
		for (int x = 1; x <= nx; x++)	/*right*/
		{

			tX0++; tX1++; tX2++;
			tY0++; tY1++; tY2++;


			a = sqtab_X[*tX0 + infp1] + sqtab_Y[*tY0 + infp1];
			b = sqtabPX[*tX1 + infp1] + sqtab_Y[*tY1 + infp1];
			c = sqtab_X[*tX2 + infp1] + sqtabPY[*tY2 + infp1];

			min = (a<b) ? a : b;
			min = (c<min) ? c : min;

			if (min == b)
			{
				*tX0 = *tX1 + 1;
				*tY0 = *tY1;

			}
			else if (min == c)
			{
				*tX0 = *tX2;
				*tY0 = *tY2 + 1;
			}

		}

		for (int x = nx; x >= 1; x--)
		{
			a = sqtab_X[*tX0 + infp1] + sqtab_Y[*tY0 + infp1];
			b = sqtabMX[*tX4 + infp1] + sqtab_Y[*tY4 + infp1];

			if (b<a)
			{
				*tX0 = *tX4 - 1;
				*tY0 = *tY4;
			}
			tX0--; tY0--;
			tX4--; tY4--;
		}
	}
	//}
}

//---------------------------------------------------------------------------------

void LayerDistanceTransformFilter::CalculateBackwardDown2D()
{
	//,z;
	//	FILE *Do_opa;
	//backward down pass of distance map calculation
	//short *tX0,*tX1,*tX2,*tX4;
	//short *tY0,*tY1,*tY2,*tY4;

	//int yT,zT;
	//int t0,t1,t2,t4;
	//int yTtemp=n2x*(ny+1);
	//int nz=0;
	//int zTemp=0;//ss2*(nz+1);
	//zT=zTemp;

	//for (z=Nz;z>=1;z--)
	//{
	//zT-=N2xy;
	//yT=yTtemp;
#pragma omp  for  nowait ordered schedule (guided)
	for (int y = ny; y >= 1; y--)
	{
		double a, b, c, min;

		short *tX0, *tX1, *tX2, *tX4;
		short *tY0, *tY1, *tY2, *tY4;

		int yT;
		int t0, t1, t2, t4;

		yT = n2x*y;
		//yT-=n2x;
		t0 = yT + 1 + nx;		//(x,y,z)
		t1 = t0 + 1;				//(x+1,y,z)
		t2 = t0 + n2x;			//(x,y+1,z)

		t4 = yT;		//(x-1,y,z)

		tX0 = tempDistanceVectorArrayX + t0; tX1 = tempDistanceVectorArrayX + t1; tX2 = tempDistanceVectorArrayX + t2; tX4 = tempDistanceVectorArrayX + t4;
		tY0 = tempDistanceVectorArrayY + t0; tY1 = tempDistanceVectorArrayY + t1; tY2 = tempDistanceVectorArrayY + t2; tY4 = tempDistanceVectorArrayY + t4;
		for (int x = nx; x >= 1; x--)
		{
			tX0--; tX1--; tX2--;
			tY0--; tY1--; tY2--;

			a = sqtab_X[*tX0 + infp1] + sqtab_Y[*tY0 + infp1];
			b = sqtabMX[*tX1 + infp1] + sqtab_Y[*tY1 + infp1];
			c = sqtab_X[*tX2 + infp1] + sqtabMY[*tY2 + infp1];

			min = (a<b) ? a : b;
			min = (c<min) ? c : min;

			if (min == b)
			{
				*tX0 = *tX1 - 1;
				*tY0 = *tY1;
			}
			else if (min == c)
			{
				*tX0 = *tX2;
				*tY0 = *tY2 - 1;
			}

		}
		for (int x = 1; x <= nx; x++)
		{
			a = sqtab_X[*tX0 + infp1] + sqtab_Y[*tY0 + infp1];
			b = sqtabPX[*tX4 + infp1] + sqtab_Y[*tY4 + infp1];
			if (b<a)
			{
				*tX0 = *tX4 + 1;
				*tY0 = *tY4;

			}
			tX0++; tY0++;
			tX4++; tY4++;
		}
	}
	//}
}



//-------------------------
int	LayerDistanceTransformFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{
	double scaleFactor = params[scaleFactorID];

	double threshLeftD = params[thresholdLeftID];
	int threshLeft = threshLeftD* mainDataMaxValue*0.01;

	double threshRightD = params[thresholdRightID];
	int threshRight = threshRightD* mainDataMaxValue*0.01;


	if (threshLeft == threshRight) return 0;
	bool inverseThresh = params[inverseThreshID];

	//unsigned short scaleDistance = params[scaleDistanceID];



	//	bool usePrev=params[prevLayerID];
	bool useMask = params[useMaskSettingsID];

	bool fillGapsFromDistance = params[fillGapsFromDistanceID];

	unsigned short prevMax = 0;


	//if ((usePrev == true) && (prevImg != NULL))
	//{
	//	prevMax=GetMaxValue(prevImg, sliceSize);
	//}

	if ((useMask == true) && (mask == false)) useMask = false;

	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;

	//	int radius=params[0];

	//	if (radius<1) return 0;

	int pos;
	int i, j;

	if ((mnx != nx) || (mny != ny) || (tempDistanceVectorArrayX == NULL))
	{
		if (tempDistanceVectorArrayX != NULL) delete[]tempDistanceVectorArrayX;
		if (tempDistanceVectorArrayY != NULL) delete[]tempDistanceVectorArrayY;
		if (sqtab_X != NULL) delete[]sqtab_X;
		if (sqtab_Y != NULL) delete[]sqtab_Y;
		if (sqtabMX != NULL) delete[]sqtabMX;
		if (sqtabMY != NULL) delete[]sqtabMY;
		if (sqtabPX != NULL) delete[]sqtabPX;
		if (sqtabPY != NULL) delete[]sqtabPY;
		if (sqrtTabNorm != NULL) delete[]sqrtTabNorm;

		nx = mnx;
		ny = mny;
		n2x = nx + 2;
		n2y = ny + 2;
		ss = nx*ny;
		ss2 = n2x*n2y;
		infinity = (nx > ny) ? nx : ny;
		infp1 = infinity + 1;


		tempDistanceVectorArrayX = new short[ss2];
		tempDistanceVectorArrayY = new short[ss2];

		sqtab_X = new int[infp1 * 2 + 1];
		sqtab_Y = new int[infp1 * 2 + 1];
		sqtabMX = new int[infp1 * 2 + 1];
		sqtabMY = new int[infp1 * 2 + 1];
		sqtabPX = new int[infp1 * 2 + 1];
		sqtabPY = new int[infp1 * 2 + 1];
		sqrtTabNorm = new float[infp1*infp1];

		pos = 0;
		//double scaleFactor = 255.0 / (infinity*sqrt(2.0));

		for (j = 0; j < infp1; j++)
		{

			for (i = 0; i<infp1; i++)
			{
				sqrtTabNorm[pos] = sqrt((float)(i*i + j*j));

				pos++;
			}
		}
	}
	//if (maxSq == 0) return 0;

	for (i = 0; i < ss2; i++) tempDistanceVectorArrayX[i] = infinity;

	double maxv, scale;


	int pos2 = n2x;
	pos = 0;
	int pCount = 0;
	bool test = params[testID];
	if ((useMask == true))
	{
		//maxv=65535;//mainDataMaxValue;
		//scale=65535.0/maxv;
		for (j = 0; j < ny; j++)
		{
			pos2++;
			for (i = 0; i<nx; i++)
			{
				if (mask[pos] == 0) { pCount++; tempDistanceVectorArrayX[pos2] = 0; }
				if (test == true)
				{
					usData0[pos] = (mask[pos] == 0) ? mainDataMaxValue : 0;
				}
				pos++;		pos2++;
			}
			pos2++;
		}
	}
	//else if ((usePrev==true)&&(prevImg!=NULL))
	//{
	//	maxv = 65535;//mainDataMaxValue;
	//	scale = 65535 / maxv;
	//	for( j=0;j<ny;j++)
	//	{
	//		pos2++;
	//		for(i=0;i<nx;i++)
	//		{
	//			bool testx = ((prevImg[pos] >= threshLeft) && (tab[pos] <= threshRight)); if (inverseThresh == true) testx = !testx;
	//			pos++;		pos2++;
	//		}
	//		pos2++;
	//	}
	//}
	else
	{
		//maxv = 65535;//mainDataMaxValue;
		//scale = 65535 / maxv;

		int maxv = 0;

		for (int i = 0; i < nx*ny; i++)
		{
			if (tab[pos] > maxv) maxv = tab[pos];
		}


		for (j = 0; j < ny; j++)
		{
			pos2++;
			for (i = 0; i < nx; i++)
			{
				bool testx = ((tab[pos] >= threshLeft) && (tab[pos] <= threshRight)); if (inverseThresh == true) testx = !testx;

				if (testx == true) { pCount++; tempDistanceVectorArrayX[pos2] = 0; }
				if (test == true) {
					usData0[pos] = (testx == true) ? mainDataMaxValue : 0;
				}
				pos++;		pos2++;
			}
			pos2++;
		}
	}




	memcpy(tempDistanceVectorArrayY, tempDistanceVectorArrayX, ss2 * sizeof(short));
	//uzupelnione obie tabele
	maxSq = 0;
	pos = 0;
	int tmp0, tmp1, tmp2;
	for (i = -infp1; i <= infp1; i++)
	{
		tmp0 = (i - 1)*(i - 1);
		tmp1 = i*i;
		tmp2 = (i + 1)*(i + 1);
		sqtabMX[pos] = tmp0;
		sqtab_X[pos] = tmp1;
		sqtabPX[pos] = tmp2;

		sqtabMY[pos] = tmp0;
		sqtab_Y[pos] = tmp1;
		sqtabPY[pos] = tmp2;
		pos++;
	}

	CalculateDistanceArrays2D();



	//float *fTab=new float [sliceSize];


	pos2 = n2x;
	pos = 0;
	int dx, dy;
	double max = 0;
	//	int maxdx=0;
	//	int maxdy=0;



	/*if (scaleDistance > 0)
	{

	scaleFactor = scaleDistance*1.0 / maxSq;
	}*/


	if (test == true)
	{
		//nic
	}
	else
	{
		for (j = 0; j < ny; j++)
		{
			pos2++;
			for (i = 0; i < nx; i++)
			{
				dx = tempDistanceVectorArrayX[pos2];
				//	if (dx>maxdx) maxdx=dx;
				dy = tempDistanceVectorArrayY[pos2];
				////	if (dy>maxdy) maxdy=dy;
				if (fillGapsFromDistance == false)
				{
					usData0[pos] = sqrtTabNorm[abs(dy)*infp1 + abs(dx)] * scaleFactor;
				}
				else
				{
					if ((dx == 0) && (dy == 0))
					{
						usData0[pos] = tab[pos];
					}
					else
					{
						int posxx = (i - dx) + (j - dy)*nx;
						if ((posxx >= 0) && (posxx < ss))
						{
							usData0[pos] = tab[posxx];
						}
						else
						{
							usData0[pos] = 0;
						}

					}
				}

				pos++;		pos2++;
			}
			pos2++;
		}
	}
	//	qWarning("DX: "+QString::number(maxdx)+" DY: "+QString::number(maxdy));


	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);


	if (useMask == true) return 2;
	return 1;
}



//----------------------------------
LayerMedianFilter::LayerMedianFilter()
{
	filterName = "LayerMedianFilter";
	QStringList list;
	AddParam("radius", 2, true, 1, 10, 1, radiusID);
	list << "square" << "circle";
	AddParam("kerenl type", 0, true, 0, 1, 1, kernelTypeID, &list);
	list.clear();

	list << "median" << "min" << "max";
	AddParam("mode", 0, true, 0, 2, 1, modeID, &list);

}
//-------------------------
int	LayerMedianFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	////if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	////unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;
	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);

	//	double stddev=params[0];
	radius = params[radiusID];
	type = params[kernelTypeID];
	mode = params[modeID];
	if (radius<1) return 0;
	//unsigned char *src,*dest;
	//unsigned short *buff0=usData0;
	//unsigned short *buff1=usData1;
	//jesli rozmiar range raw data jest wiekszy od 255 to robie przeskalowanie

	/*src=new unsigned char[sliceSize];
	dest=new unsigned char[sliceSize];*/




	// tu odpalenie filterka



	int nz = 1;
	int nx = mnx;
	int ny = mny;
	int sx = nx;
	int sliceSize = nx*ny;
	int dsizemin1 = sliceSize - 1;
	//------------------------------------------


	int maxvalue = 255;
	int kernelSize = 0;
	int tmpPos;
	int count;
	//int radius = 1;
	//int type = 0;
	RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
	int *kernel = GetGrowingKernel2D(kernelSize, radius, data, type, true);
	delete data;

	int *tab1 = new int[kernelSize];
	//double *tab1=new double[2*maxvalue];


	int pos = 0;
	//int l;

	double stddev, mean;
	double localMean, localStddev;
	//double nx1d=1.0/nx;

	double kVal, fVal;
	double cVal;
	double cMinVal;
	double finalVal;
	int chgs;
	int tmpPos2;

	chgs = 0;
	pos = 0;


	//nrOfThreads = omp_get_num_threads();
	unsigned short **tabz;

	tabz = new unsigned short *[nrOfThreads];

	for (int i = 0; i<nrOfThreads; i++)
	{
		tabz[i] = new unsigned short[kernelSize];
	}

	int tid = omp_get_thread_num();
#pragma omp parallel for  ordered schedule (guided) 
	for (int j = 0; j < sliceSize; j++)
	{

		//tmpSize = 0;
		//for (int i = 0; i < kernelSize; i++)
		//{
		//	tmpPos = j + kernel[i];
		//	localMean = 0;
		//	localStddev = 0;

		//	if (!((tmpPos > dsizemin1) || (tmpPos < 0)))//||(y!=oldy+kernelInc[i]))) //brzydkie
		//	{
		//		tab1[tmpSize++] = tab[tmpPos];
		//	}

		//}
		int tmpSize;
		int tid = omp_get_thread_num();
		GetKernelIntesitiesToTab(tab, tabz[tid], tmpSize, kernel, kernelSize, j, dsizemin1);
		if (tmpSize > 1)
		{
			switch (mode)
			{
			case 0:
				usData0[j] = Median(tabz[tid], tmpSize - 1);
				break;
			case 1:
				usData0[j] = Minimum(tabz[tid], tmpSize - 1);
				break;
			case 2:
				usData0[j] = Maximum(tabz[tid], tmpSize - 1);
				break;

			}
		}
		else
		{
			usData0[j] = tab[j];
		}

	}

	for (int i = 0; i<nrOfThreads; i++)
	{
		delete[]tabz[i]; tabz[i] = NULL;
	}
	delete[]tabz;
	delete[]kernel;
	delete[]tab1;

	//-----------------------------------------------





	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);
	//	delete []src;
	//	delete []dest;

	return 1;
}



LayerMedianFast2DFilter::LayerMedianFast2DFilter()
{
	filterName = "LayerMedianFast2DFilter";

	AddParam("radius", 2, true, 1, 100, 1, radiusID);


}
//-------------------------
int	LayerMedianFast2DFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	////if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	////unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;
	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);

	//	double stddev=params[0];
	int radius = params[radiusID];


	if (radius<1) return 0;
	//unsigned char *src,*dest;
	//unsigned short *buff0=usData0;
	//unsigned short *buff1=usData1;
	//jesli rozmiar range raw data jest wiekszy od 255 to robie przeskalowanie
	bool createUcharBuffers = ((src != NULL) && (dest != NULL)) ? false : true;

	if (createUcharBuffers == true)
	{
		src = new unsigned char[sliceSize];
		dest = new unsigned char[sliceSize];
	}
	/*src=new unsigned char[sliceSize];
	dest=new unsigned char[sliceSize];*/


	double scale = 1;
	double maxv = mainDataMaxValue;
	if (maxv>0)
	{
		if (maxv>256)
		{
			scale = 255.0 / maxv;
		}
	}
	double scale1 = 1.0 / scale;


	for (int i = 0; i < sliceSize; i++)
	{
		src[i] = tab[i] * scale;
	}

	// tu odpalenie filterka

	ctmf(src, dest, mnx, mny, mnx, mnx, radius, 1, 512 * 1024);


	for (int i = 0; i<sliceSize; i++)
	{
		usData0[i] = dest[i] * scale1;
	}

	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);
	//	delete []src;
	//	delete []dest;
	if (createUcharBuffers == true)
	{
		delete[]src;
		delete[]dest;
	}
	return 1;
}

//------------------------------------------------------
LayerBilateralFilter::LayerBilateralFilter()
{
	filterName = "BilateralFilter";
	QStringList list;
	list.clear(); list << "false" << "true";

	AddParam("iter", 1, true, 1, 20, 1, iterID);
	AddParam("sigma_spatial", 0.0001, true, 0.0001, 0.1, 0.001, sigma_spatialID);
	AddParam("sigma_range", 0.02, true, 0.02, 1.0, 0.001, sigma_rangeID);
	AddParam("use Prev Textire", 0, true, 0, 1, 1, textureID, &list);





	//this->paramsNames[numberOfParams]="iter";	defaultParams[numberOfParams]=1;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="sigma_spatial";	defaultParams[numberOfParams]=0.0001;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0.0001;		this->maxVals[numberOfParams]=0.1;	this->steps[numberOfParams]=0.001;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="sigma_range";	defaultParams[numberOfParams]=0.02;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0.02;		this->maxVals[numberOfParams]=1.0;	this->steps[numberOfParams]=0.001;		numberOfParams++;
	//this->paramsNames[numberOfParams]="texture";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=1;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;

}
//-------------------------
int	LayerBilateralFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	/*if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;*/

	int iter = params[iterID];
	double sigma_spatial = params[sigma_spatialID];
	double sigma_range = params[sigma_rangeID];
	bool texturing = params[textureID];



	double scale = 1;
	double maxv = mainDataMaxValue;
	if (maxv>0)
	{
		if (maxv>255)
		{
			scale = 255.0 / maxv;
		}
	}
	double scale1 = 1.0 / scale;



	qx_constant_time_bilateral_filter_published m_ctbf;
	int h = mny;
	int w = mnx;

	double**in = qx_allocd(h, w);
	double**out = qx_allocd(h, w);


	double**texture = NULL;

	if ((texturing == true) && (prevImg != NULL))
	{
		texture = qx_allocd(h, w);
		int pos = 0;
		for (int y = 0; y<h; y++) for (int x = 0; x<w; x++)
		{
			texture[y][x] = prevImg[pos++] * scale;
		}

	}

	m_ctbf.init(h, w);

	int pos = 0;
	for (int y = 0; y<h; y++) for (int x = 0; x<w; x++)
	{
		in[y][x] = tab[pos++] * scale;
	}

	qx_timer timer;
	timer.start();
	for (int i = 0; i<iter; i++)
	{
		m_ctbf.filter(out, in, sigma_spatial, sigma_range, texture);
		if (i == iter - 1)	continue;
		double  **tmp;
		tmp = in;
		in = out;
		out = tmp;
	}
	//timer.time_display("bilateral filtering", iter);
	pos = 0;
	//for(int y=0;y<h;y++) for(int x=0;x<w;x++)  usData0[pos++]=(unsigned char)(min(255.0,max(0.0,out[y][x])))*scale1;

	for (int y = 0; y<h; y++) for (int x = 0; x<w; x++)
	{
		double diff = out[y][x] - in[y][x];
		int  val = tab[pos] + diff*scale1;
		if (val < 0)val = 0; else if (val > maxv) val = maxv;
		usData0[pos] = val;
		pos++;
	}


	qx_freed(in);
	qx_freed(out);
	if ((texturing == true) && (prevImg != NULL))
	{
		qx_freed(texture);
	}
	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);


	return 1;
}

//------------------------------------------------------
LayerHighPassFilter::LayerHighPassFilter()
{
	filterName = "HighPassFilter";


	QStringList list;
	AddParam("radius", 2, true, 1, 100, 1, radiusID);
	AddParam("stddev", 0.3, true, 0.01, 2, 0.01, stddevID);
	AddParam("center", 128, true, 0, 255, 1, centerID);
	list.clear(); list << "false" << "true";
	AddParam("fabs", 0, true, 0, 1, 1, fabsID, &list);





	//	numberOfParams=0;

	//	this->paramsNames[numberOfParams]="radius";	defaultParams[numberOfParams]=4;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=100;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="stddev";	defaultParams[numberOfParams]=0.3;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0.01;		this->maxVals[numberOfParams]=2;	this->steps[numberOfParams]=0.01;		numberOfParams++;
	//this->paramsNames[numberOfParams]="center";	defaultParams[numberOfParams]=128;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=255;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="fabs";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=1;	this->steps[numberOfParams]=1;		numberOfParams++;

	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;


	//this->paramsNames[0]="radius";	defaultParams[0]=4;		paramsSliders[0]=true;		this->minVals[0]=1;		this->maxVals[0]=100;	this->steps[0]=1;
	//this->paramsNames[1]="stddev";	defaultParams[1]=0.3;	paramsSliders[1]=true;		this->minVals[1]=0.01;		this->maxVals[1]=2;	this->steps[1]=0.01;
	//this->paramsNames[2]="center";	defaultParams[1]=0.3;	paramsSliders[1]=true;		this->minVals[1]=0.01;		this->maxVals[1]=2;	this->steps[1]=0.01;
	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
}
//-------------------------
int	LayerHighPassFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;

	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j;

	unsigned char val1, val2;


	double stddev = params[stddevID];
	int radius = params[radiusID];
	if (radius<1) return 0;
	int					size2 = 2 * radius + 1;
	double				*gaussianKernelTab = new double[size2];
	unsigned short		*intensityTab = new unsigned short[size2];
	GetGaussianKernel(gaussianKernelTab, (double)radius, stddev);




	int center = params[centerID];


	int maxVal = mainDataMaxValue;
	int offset = (maxVal*center) / 255;

	bool mAbs = params[fabsID];

	unsigned short *Q;

	int pos;

	Q = tab;
	pos = 0;
	int n[2] = { mnx, mny };

	unsigned short *tmpBuff = tab;


	//x filtering
	for (j = 0; j<mny; j++)
	{
		for (i = 0; i<mnx; i++)
		{
			GetIntensityTab1D(tmpBuff, n[0], n[1], 1, 0, radius, intensityTab, i, j, 0);
			buff0[pos++] = Apply1DNormFilter(gaussianKernelTab, intensityTab, size2);

		}
	}

	//y-filtering

	pos = 0;
	int val;

	if (mAbs == false)
	{
		for (j = 0; j<mny; j++)
		{
			for (i = 0; i<mnx; i++)
			{
				GetIntensityTab1D(buff0, n[0], n[1], 1, 1, radius, intensityTab, i, j, 0);

				val = offset + (tab[pos] - Apply1DNormFilter(gaussianKernelTab, intensityTab, size2));
				if (val<0) val = 0; else if (val >= maxVal)val = maxVal;
				buff1[pos] = val;;

				pos++;
			}
		}
	}
	else
	{
		for (j = 0; j<mny; j++)
		{
			for (i = 0; i<mnx; i++)
			{
				GetIntensityTab1D(buff0, n[0], n[1], 1, 1, radius, intensityTab, i, j, 0);

				val = offset + abs(tab[pos] - Apply1DNormFilter(gaussianKernelTab, intensityTab, size2));
				if (val<0) val = 0; else if (val >= maxVal)val = maxVal;
				buff1[pos] = val;;

				pos++;
			}
		}
	}
	delete[]gaussianKernelTab;
	delete[]intensityTab;
	//swap buffers
	buffTmp = usData0;
	usData0 = usData1;
	usData1 = buffTmp;
	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);
	return 1;

}







//-------------------------------------------------------------

LayerUnsharpMaskFilter::LayerUnsharpMaskFilter()
{
	filterName = "UnsharpMaskFilter";
	//	numberOfParams=0;


	QStringList list;
	AddParam("radius", 2, true, 1, 100, 1, radiusID);
	AddParam("stddev", 0.3, true, 0.01, 2, 0.01, stddevID);
	AddParam("amount", 30, true, 0, 1000, 1, amountID);
	AddParam("threshold", 50, true, 0, 100, 0.1, thresholdID);
	//list.clear(); list <<"false" <<"true" ;
	//AddParam("fabs", 0,true,0,1,1,&list);



	/*this->paramsNames[numberOfParams]="radius";	defaultParams[numberOfParams]=2;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=100;	this->steps[numberOfParams]=1;		numberOfParams++;
	this->paramsNames[numberOfParams]="stddev";	defaultParams[numberOfParams]=2.5;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0.01;		this->maxVals[numberOfParams]=3;	this->steps[numberOfParams]=0.01;	numberOfParams++;
	this->paramsNames[numberOfParams]="amount";	defaultParams[numberOfParams]=30;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=1000;	this->steps[numberOfParams]=1;		numberOfParams++;
	this->paramsNames[numberOfParams]="threshold";	defaultParams[numberOfParams]=50;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=100;	this->steps[numberOfParams]=0.1;		numberOfParams++;
	this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	*/
}
//-------------------------
int	LayerUnsharpMaskFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;

	double percent = params[amountID] * 0.01;
	double thresh = params[thresholdID] * 0.01*mainDataMaxValue;

	//

	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);
	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j;
	//	bool revth=maskThresholds[2];
	unsigned char val1, val2;



	int radius = params[radiusID];
	double stddev = params[stddevID];
	//bool useWeightMap = params[useWeightMapID];
	double amount = params[amountID];
	//	bool invertWeights = params[invertWeightsID];;


	if (radius<1) return 0;
	int					size2 = 2 * radius + 1;
	double				*gaussianKernelTab = new double[size2];
	unsigned short		*intensityTab = new unsigned short[size2];
	GetGaussianKernel(gaussianKernelTab, (double)radius, stddev);


	unsigned short *Q;

	int pos;

	Q = tab;
	pos = 0;
	int n[2] = { mnx, mny };

	unsigned short *tmpBuff = tab;



	unsigned short **tabz;
	//#pragma omp parallel private (tabz) num_threads(4)
	{
		unsigned short *datatab = tab;
		tabz = new unsigned short *[nrOfThreads];
		//#pragma omp critical
		{
			for (i = 0; i<nrOfThreads; i++)
			{
				tabz[i] = new unsigned short[size2];
			}
		}
		//	int tid = omp_get_thread_num();
		;// cout << tid''
#pragma omp parallel for  ordered schedule (guided) 
		for (int j = 0; j < mny; j++)
		{
			int pos = j*mnx;
			int  tid = omp_get_thread_num();
			for (int i = 0; i < mnx; i++)
			{

				GetIntensityTab1D(tmpBuff, n[0], n[1], 1, 0, radius, tabz[tid], i, j, 0);
				buff0[pos++] = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
			}
		}

		//y-filtering
		pos = 0;
		//	qDebug() << "max: " << mainDataMaxValue;

#pragma omp parallel for  ordered schedule (guided) 
		for (int j = 0; j < mny; j++)
		{
			int pos = j*mnx;
			int  tid = omp_get_thread_num();
			for (int i = 0; i<mnx; i++)
			{
				GetIntensityTab1D(buff0, n[0], n[1], 1, 1, radius, tabz[tid], i, j, 0);
				///buff1[pos++] =  Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);

				int filterres = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
				int diff = (tab[pos] - filterres);
				int val = tab[pos] + diff*percent;
				if (val<0) val = 0; else if (val>mainDataMaxValue) val = mainDataMaxValue;

				buff1[pos] = (tab[pos] > thresh) ? val : tab[pos];;
				pos++;
			}
		}
		//#pragma omp critical
		{
			for (i = 0; i<nrOfThreads; i++)
			{
				delete[]tabz[i]; tabz[i] = NULL;
			}
			delete[]tabz;
		}
	}

	//swap buffers
	buffTmp = usData0;
	usData0 = usData1;
	usData1 = buffTmp;


	delete[]gaussianKernelTab;
	delete[]intensityTab;




	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);
	return 1;
	//	unsigned short *buff0=usData0;
	//	unsigned short *buff1=usData1;
	//	unsigned short *buffTmp;
	//	int i,j;
	//
	//	unsigned char val1,val2;
	//
	//
	//	double stddev=params[stddevID];
	//	int radius=params[radiusID];
	//	if (radius<1) return 0;
	//	int					size2=2*radius+1;
	//	double				*gaussianKernelTab=new double [size2];
	//	unsigned short		*intensityTab=new unsigned short[size2];
	//	GetGaussianKernel(gaussianKernelTab,(double)radius,stddev);
	//
	//	int maxValue=mainDataMaxValue;
	//	double percent=params[amountID]*0.01;
	//	double thresh=params[thresholdID]*0.01*maxValue;
	//	unsigned short *Q;
	//
	//	int pos;
	//
	//	Q=tab;
	//	pos=0;
	//	int n[2] = { mnx, mny };
	//
	//	unsigned short *tmpBuff=tab;
	//
	//	
	//	nrOfThreads = omp_get_num_threads();
	//	unsigned short **tabz;
	//	//#pragma omp parallel private (tabz) 
	//	{
	//		tabz = new unsigned short *[nrOfThreads];
	//	//	#pragma omp critical
	//		{
	//			for (i = 0; i<nrOfThreads; i++)
	//			{
	//				tabz[i] = new unsigned short[size2];
	//			}
	//		}
	//		int tid = omp_get_thread_num();
	//#pragma omp parallel for  ordered schedule (guided) 
	//		for (int j = 0; j < mny; j++)
	//		{
	//			int pos = j*mnx;
	//			int tid = omp_get_thread_num();
	//			for (int i = 0; i < mnx; i++)
	//			{
	//				GetIntensityTab1D(tmpBuff, n[0], n[1], 1, 0, radius, tabz[tid], i, j, 0);
	//				buff0[pos++] = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
	//			}
	//		}
	//
	//		//y-filtering
	//		pos = 0;
	//
	//		int val;
	//#pragma omp parallel for  ordered schedule (guided) 
	//		for (int j = 0; j < mny; j++)
	//		{
	//			int pos = j*mnx;
	//			int tid = omp_get_thread_num();
	//			for (int i = 0; i<mnx; i++)
	//			{
	//				GetIntensityTab1D(buff0, n[0], n[1], 1, 1, radius, tabz[tid], i, j, 0);
	//				val = tab[pos] + (tab[pos] - Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2))*percent;
	//				if (val<0) val = 0; else if (val>maxValue) val = maxValue;
	//
	//				buff1[pos] = (tab[pos]>thresh) ? val : tab[pos];;
	//				pos++;
	//			}
	//		}
	//	//	#pragma omp critical
	//		{
	//			for (i = 0; i<nrOfThreads; i++)
	//			{
	//				delete[]tabz[i]; tabz[i] = NULL;
	//			}
	//			delete[]tabz;
	//		}
	//	}
	//
	//
	//
	//	//swap buffers
	//	buffTmp = usData0;
	//	usData0=usData1;
	//	usData1=buffTmp;
	//	updateBrightness(tab,usData0,sliceSize,mainDataMaxValue,inverse,brightnessFilterMode);
	//	return 1;

}
//-------------------------------------------------------------

LayerStatisticalHomogeneousMaskFilter::LayerStatisticalHomogeneousMaskFilter()
{
	filterName = "StatisticalHomogeneousMaskFilter";
	//numberOfParams=0;

	AddParam("iter", 2, true, 1, 100, 1, iterID);


	//	this->paramsNames[numberOfParams]="radius";			defaultParams[numberOfParams]=2;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="kernel type";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=1;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="K Function";		defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;				maxVals[numberOfParams]=2;		steps[numberOfParams]=1;		numberOfParams++;

	//	this->paramsNames[numberOfParams]="iter";			defaultParams[numberOfParams]=1;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=100;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;


}
//-------------------------
int	LayerStatisticalHomogeneousMaskFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{
	int iter = params[iterID];

	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;
	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);
	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j, k, l;

	double coef = 0.1 / 6.0;// lambda=0.1, k=6 (6 neighbors)


	{
		double *src, *dest;


		src = new double[sliceSize];
		dest = new double[sliceSize];
		for (int i = 0; i < mnx*mny; i++)
		{
			src[i] = tab[i];
		}


		double scale = 1;
		double maxv = mainDataMaxValue;
		if (maxv>0)
		{
			//if (maxv>256)
			{
				scale = 1.0 / maxv;
			}
		}
		double scale1 = 1.0 / scale;



		// tu odpalenie filterka

		int nz = 1;
		int nx = mnx;
		int ny = mny;
		int sx = nx;
		int sliceSize = nx*ny;
		int dsizemin1 = sliceSize - 1;
		//------------------------------------------


		int maxvalue = 255;
		int kernelSize = 0;
		int tmpPos;
		int count;
		int radius = 1;
		int type = 0;
		RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
		int *kernel = GetGrowingKernel2D(kernelSize, radius, data, type, true);
		delete data;

		double *tab1 = new double[kernelSize];
		//double *tab1=new double[2*maxvalue];


		int pos = 0;
		//int l;

		double stddev, mean;
		double localMean, localStddev;
		//double nx1d=1.0/nx;

		double kVal, fVal;
		double cVal;
		double cMinVal;
		double finalVal;
		int chgs;
		int tmpPos2;
		for (l = 0; l<iter; l++)
		{
			chgs = 0;
			pos = 0;



			for (j = 0; j<sliceSize; j++)
			{

				cMinVal = 10000;
				finalVal = src[j];
				for (i = 0; i<kernelSize; i++)
				{
					tmpPos = j + kernel[i];
					localMean = 0;
					localStddev = 0;

					if (!((tmpPos>dsizemin1) || (tmpPos<0)))//||(y!=oldy+kernelInc[i]))) //brzydkie
					{
						count = 0;
						for (k = 0; k<kernelSize; k++)
						{
							tmpPos2 = tmpPos + kernel[k];
							if (!((tmpPos2>dsizemin1) || (tmpPos2<0)))//||(y!=oldy+kernelInc[i]))) //brzydkie
							{
								tab1[count] = src[tmpPos2];
								localMean += tab1[count];
								count++;
							}

						}
						if (count>0)
						{
							localMean /= (double)count;
							if ((localMean>0))
							{
								for (k = 0; k<count; k++) { localStddev += (localMean - tab1[k])*(localMean - tab1[k]); }		localStddev /= localMean; //sigma^2
								cVal = localStddev / count;
								if (cVal<cMinVal)
								{
									cMinVal = cVal;
									finalVal = localMean;
								}
							}
						}
					}
				}
				if (finalVal != src[j]) chgs++;

				dest[j] = finalVal;

			}
			qDebug() << "iter " + QString::number(l) + " chgs " + QString::number(chgs);
			if (chgs == 0) l = iter - 1;
			//if (l==iter-1)	continue;
			double *tmp;
			tmp = src;
			src = dest;
			dest = tmp;
		}

		delete[]kernel;
		delete[]tab1;
		//-----------------------------------------------

		double *tmp;
		tmp = src;
		src = dest;
		dest = tmp;
		for (i = 0; i<sliceSize; i++)
		{
			usData0[i] = dest[i];// *scale1;
		}
		updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

		delete[]src;
		delete[]dest;


		return 1;
	}
	return 1;

}
//-----------------------


LayerStatisticalFilter1Faster::LayerStatisticalFilter1Faster()
{
	filterName = "StatisticalFilter1Faster";
	numberOfParams = 0;




	AddParam("radius", 2, true, 1, 20, 1, radiusID);
	AddParam("optimMode", 2, true, 1, 3, 1, optimID);


	//this->paramsNames[numberOfParams]="radius";			defaultParams[numberOfParams]=10;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="iter";			defaultParams[numberOfParams]=1;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=100;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="optim";			defaultParams[numberOfParams]=3;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=3;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;


}
//-------------------------
int	LayerStatisticalFilter1Faster::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	/*if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;*/

	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j, k, l, m;




	{
		double *src, *dest;

		src = new double[sliceSize];
		dest = new double[sliceSize];

		for (int i = 0; i < mnx*mny; i++)
		{
			src[i] = tab[i];
		}

		double scale = 1;
		double maxv = mainDataMaxValue;
		if (maxv>0)
		{
			//if (maxv>256)
			{
				scale = 1.0 / maxv;
			}
		}
		double scale1 = 1.0 / scale;





		int radius = params[radiusID];


		int iter = 1;//params[1];
		int optim = params[optimID];



		int nz = 1;
		int nx = mnx;
		int ny = mny;
		int sx = nx;
		int sliceSize = nx*ny;
		int dsizemin1 = sliceSize - 1;
		//------------------------------------------


		int maxvalue = 255;
		int kernelSize = 0;
		int tmpPos;
		int count;

		//int *kernel=GetGrowingKernel2D(kernelSize,radius, data,type);
		RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
		int *kernel = GetGrowingKernel1D(kernelSize, radius, data, 1, true);
		delete data;
		double *tab1 = new double[(radius * 2 + 1)*(radius * 2 + 1)];



		int pos = 0;
		//int l;

		double stddev, mean, mean2;
		double localMean, localStddev, localMean2;
		double nx1d = 1.0 / nx;

		double kVal, kVal1, fVal;
		int size = radius * 2 + 1;
		list <lFilterColumn*> colList;
		//define filter columns
		for (i = 0; i<size; i++)
		{
			colList.push_back(new lFilterColumn(size));
		}

		double *tabx;


		lFilterColumn **myColTab = new lFilterColumn*[size];
		lFilterColumn *firstColItem;
		lFilterColumn *currentItem;
		for (i = 0; i<size; i++)
		{
			myColTab[i] = new lFilterColumn(size);
			if (i>0) myColTab[i - 1]->next = myColTab[i];
		}
		myColTab[i - 1]->next = myColTab[0];;//loop

		firstColItem = myColTab[0];
		double val;
		for (l = 0; l<iter; l++)
		{
			pos = 0;

			//policze tutaj globalna stddev i srednia
			/*mean=0;
			for (i=0;i<sliceSize;i++)
			{
			mean+=src[i];
			}
			mean/=(double)sliceSize;

			stddev=0;

			for(i=0;i<sliceSize;i++)
			{
			stddev+=(mean-src[i])*(mean-src[i]);
			}

			stddev/=(double)sliceSize;*/


			mean = 0;
			mean2 = 0;

			for (i = 0; i<sliceSize; i++)
			{
				val = src[i];
				mean += val;
				mean2 += val*val;
			}
			mean /= (double)sliceSize;
			mean2 /= (double)sliceSize;

			stddev = sqrt(mean2 - mean*mean);

			/*for(i=0;i<sliceSize;i++)
			{
			stddev+=(mean-src[i])*(mean-src[i]);
			}

			stddev/=(double)sliceSize;*/


			//----------------- do optymalizacji ------------
			//----------------- do optymalizacji ------------
			//----------------- do optymalizacji ------------
			//----------------- do optymalizacji ------------

			if (optim == 0)
			{
				//lista stl
				int tmpA;
				pos = 0;
				for (j = 0; j<ny; j++)
				{
					for (i = 0; i<nx; i++)
					{
						if (i == 0)
						{
							//zbierz wszystkie size buforow
							//int pos1=pos;
							list<lFilterColumn*>::iterator ii;
							tmpA = -radius;
							for (ii = colList.begin(); ii != colList.end(); ++ii)
							{
								count = 0;
								double localSum = 0;
								if (tmpA++ >= 0)// radius pierwszych elementow jest poza obrazem=0
								{
									tabx = (*ii)->tab;
									for (k = 0; k<kernelSize; k++)
									{
										tmpPos = pos + tmpA + kernel[k];
										//	tmpPos=pos+kernel[k];//do poprawienia tak jak faster2
										if (!((tmpPos>dsizemin1) || (tmpPos<0)))
										{
											tabx[count++] = src[tmpPos];
											localSum += src[tmpPos];

										}
									}
								}
								(*ii)->count = count;
								(*ii)->sum = localSum;
								//wszystkie slupki uzupelnione
							}
						}
						else
						{
							//dobierz nowy bufor na koncu i wywal stary
							//wypelnij pierwszy slupek nastepnym pobranym
							//przerzuc pierwszy slupek na koniec listy
							//-------------------------------
							//-------------------------------
							list<lFilterColumn*>::iterator ii;
							ii = colList.begin();
							count = 0;
							double localSum = 0;
							//if (tmpA++>=0)// radius pierwszych elementow jest poza obrazem=0
							if (i<nx - radius - 1)
							{
								tabx = (*ii)->tab;
								for (k = 0; k<kernelSize; k++)
								{

									tmpPos = pos + radius + kernel[k];
									//tmpPos=pos+kernel[k]; //do poprawienia tak jak faster2
									if (!((tmpPos>dsizemin1) || (tmpPos<0)))
									{
										tabx[count++] = src[tmpPos];
										localSum += src[tmpPos];
									}
								}
							}
							(*ii)->count = count;
							(*ii)->sum = localSum;

							colList.splice(colList.end(), colList, ii);//move from the begining to the end
																	   //-------------------------------
																	   //-------------------------------

						}

						count = 0;
						localMean = 0;
						localStddev = 0;
						list<lFilterColumn*>::iterator ii;
						for (ii = colList.begin(); ii != colList.end(); ++ii)
						{

							localMean += (*ii)->sum;
							count += (*ii)->count;

						}

						localMean /= (double)count;
						if ((count>0) && (localMean>0))
						{
							for (ii = colList.begin(); ii != colList.end(); ++ii)
							{
								int ssize = (*ii)->count;
								if (ssize>0)
								{
									tabx = (*ii)->tab;
									for (k = 0; k<ssize; k++)
									{
										localStddev += (localMean - tabx[k])*(localMean - tabx[k]);
									}
								}

							}

							localStddev /= count; //sigma^2

							kVal = localStddev / (localMean*localMean*stddev + localStddev);//k2
							fVal = localMean + kVal*(src[pos] - localMean);

							dest[pos] = fVal;


						}
						else
						{
							dest[pos] = src[j];
						}


						pos++;


					}
				}


			}
			else if (optim == 1)
			{
				//wersja bez listy
				int tmpA;
				pos = 0;
				for (j = 0; j<ny; j++)
				{

					for (i = 0; i<nx; i++)
					{
						if (i == 0)
						{
							tmpA = -radius;
							currentItem = firstColItem;
							for (int k1 = 0; k1<size; k1++)
							{
								count = 0;
								double localSum = 0;
								if (tmpA++ >= 0)// radius pierwszych elementow jest poza obrazem=0
								{
									tabx = currentItem->tab;
									for (k = 0; k<kernelSize; k++)
									{
										tmpPos = pos + tmpA + kernel[k];
										//tmpPos=pos+kernel[k];
										if (!((tmpPos>dsizemin1) || (tmpPos<0)))
										{
											tabx[count++] = src[tmpPos];
											localSum += src[tmpPos];

										}
									}
								}
								currentItem->count = count;
								currentItem->sum = localSum;
								//wszystkie slupki uzupelnione
								currentItem = currentItem->next;

							}


						}
						else
						{
							//dobierz nowy bufor na koncu i wywal stary
							//wypelnij pierwszy slupek nastepnym pobranym
							//przerzuc pierwszy slupek na koniec listy

							count = 0;
							double localSum = 0;
							//if (tmpA++>=0)// radius pierwszych elementow jest poza obrazem=0
							if (i<nx - radius - 1)
							{
								tabx = firstColItem->tab;
								for (k = 0; k<kernelSize; k++)
								{
									tmpPos = pos + radius + kernel[k];
									//tmpPos=pos+kernel[k];
									if (!((tmpPos>dsizemin1) || (tmpPos<0)))
									{
										tabx[count++] = src[tmpPos];
										localSum += src[tmpPos];

									}
								}
							}
							firstColItem->count = count;
							firstColItem->sum = localSum;


							lFilterColumn *tmpItem = firstColItem;
							firstColItem = firstColItem->next;




						}




						count = 0;
						localMean = 0;
						localStddev = 0;


						currentItem = firstColItem;
						for (k = 0; k<size; k++)
						{

							localMean += currentItem->sum;
							count += currentItem->count;
							currentItem = currentItem->next;

						}

						localMean /= (double)count;
						if ((count>0) && (localMean>0))
						{


							currentItem = firstColItem;
							for (int kk = 0; kk<size; kk++)
							{
								int ssize = currentItem->count;
								if (ssize>0)
								{
									tabx = currentItem->tab;
									for (k = 0; k<ssize; k++)
									{
										localStddev += (localMean - tabx[k])*(localMean - tabx[k]);
									}
								}
								currentItem = currentItem->next;

							}
							localStddev /= count; //sigma^2

							kVal = localStddev / (localMean*localMean*stddev + localStddev);//k2
							fVal = localMean + kVal*(src[pos] - localMean);

							dest[pos] = fVal;


						}
						else
						{
							dest[pos] = src[j];
						}


						pos++;


					}
				}
			}
			else if (optim == 2)
			{
				//lista stl - singlepass
				double bigSum, bigSum2;
				int bigCount;
				int tmpA;
				pos = 0;
				for (j = 0; j<ny; j++)
				{
					for (i = 0; i<nx; i++)
					{
						if (i == 0)
						{
							//zbierz wszystkie size buforow
							//int pos1=pos;
							bigSum = 0;
							bigSum2 = 0;
							bigCount = 0;
							list<lFilterColumn*>::iterator ii;
							tmpA = -radius;
							for (ii = colList.begin(); ii != colList.end(); ++ii)
							{
								count = 0;
								double localSum = 0;
								double localSum2 = 0;
								if (tmpA++ >= 0)// radius pierwszych elementow jest poza obrazem=0
								{
									//tabx=(*ii)->tab;
									for (k = 0; k<kernelSize; k++)
									{
										//tmpPos=pos+kernel[k];
										tmpPos = pos + tmpA + kernel[k];
										if (!((tmpPos>dsizemin1) || (tmpPos<0)))
										{
											val = src[tmpPos];
											//tabx[count++]=val;
											localSum += val;
											localSum2 += val*val;
											count++;

										}
									}
								}
								(*ii)->count = count;		(*ii)->sum = localSum;		(*ii)->sum2 = localSum2;
								bigCount += count;		bigSum += localSum;			bigSum2 += localSum2;

								//wszystkie slupki uzupelnione
							}
						}
						else
						{
							//dobierz nowy bufor na koncu i wywal stary
							//wypelnij pierwszy slupek nastepnym pobranym
							//przerzuc pierwszy slupek na koniec listy
							//-------------------------------
							//-------------------------------
							list<lFilterColumn*>::iterator ii;
							ii = colList.begin();
							count = 0;
							double localSum = 0;
							double localSum2 = 0;
							//if (tmpA++>=0)// radius pierwszych elementow jest poza obrazem=0
							if (i<nx - radius - 1)
							{
								//tabx=(*ii)->tab;
								for (k = 0; k<kernelSize; k++)
								{
									tmpPos = pos + radius + kernel[k];
									//tmpPos=pos+kernel[k];
									if (!((tmpPos>dsizemin1) || (tmpPos<0)))
									{
										val = src[tmpPos];
										//tabx[count++]=val;
										localSum += val;
										localSum2 += val*val;
										count++;
									}
								}
							}
							bigCount -= (*ii)->count;		bigSum -= (*ii)->sum;			bigSum2 -= (*ii)->sum2;
							(*ii)->count = count;			(*ii)->sum = localSum;		(*ii)->sum2 = localSum2;
							bigCount += count;			bigSum += localSum;			bigSum2 += localSum2;
							colList.splice(colList.end(), colList, ii);//move from the begining to the end
																	   //-------------------------------
																	   //-------------------------------

						}
						if (bigCount>0)
						{
							localMean = bigSum / (double)bigCount;
							localMean2 = bigSum2 / (double)bigCount;
							localStddev = localMean2 - localMean*localMean;
							kVal = localStddev / (localMean*localMean*stddev + localStddev);//k2
							fVal = localMean + kVal*(src[pos] - localMean);

							dest[pos] = fVal;
						}

						else
						{
							dest[pos] = src[j];
						}


						pos++;


					}
				}


			}
			else if (optim == 3)
			{
				//wersja bez listy - singlepass
				double bigSum, bigSum2;
				int bigCount;
				int tmpA;
				pos = 0;
				for (j = 0; j<ny; j++)
				{

					for (i = 0; i<nx; i++)
					{
						if (i == 0)
						{
							//int pos1=pos;
							bigSum = 0;
							bigSum2 = 0;
							bigCount = 0;
							tmpA = -radius;
							currentItem = firstColItem;
							for (int k1 = 0; k1<size; k1++)
							{
								count = 0;
								double localSum = 0;
								double localSum2 = 0;
								if (tmpA++ >= 0)// radius pierwszych elementow jest poza obrazem=0
								{
									//tabx=currentItem->tab;
									for (k = 0; k<kernelSize; k++)
									{
										tmpPos = pos + k1 - radius + kernel[k];
										//tmpPos=pos+kernel[k];
										if (!((tmpPos>dsizemin1) || (tmpPos<0)))
										{
											//tabx[count++]=src[tmpPos];
											//localSum+=src[tmpPos];
											val = src[tmpPos];
											//tabx[count++]=val;
											localSum += val;
											localSum2 += val*val;
											count++;
										}
									}
								}

								currentItem->count = count;		currentItem->sum = localSum;			currentItem->sum2 = localSum2;
								bigCount += count;				bigSum += localSum;					bigSum2 += localSum2;

								currentItem = currentItem->next;
							}


							i = i;
						}
						else
						{
							//dobierz nowy bufor na koncu i wywal stary
							//wypelnij pierwszy slupek nastepnym pobranym
							//przerzuc pierwszy slupek na koniec listy

							count = 0;
							double localSum = 0;
							double localSum2 = 0;
							//if (tmpA++>=0)// radius pierwszych elementow jest poza obrazem=0
							if (i<nx - radius - 1)
							{
								tabx = firstColItem->tab;
								for (k = 0; k<kernelSize; k++)
								{
									tmpPos = pos + radius + kernel[k];
									//tmpPos=pos+kernel[k];
									if (!((tmpPos>dsizemin1) || (tmpPos<0)))
									{
										/*tabx[count++]=src[tmpPos];
										localSum+=src[tmpPos];*/

										val = src[tmpPos];	localSum += val;	localSum2 += val*val;		count++;
									}
								}
							}

							bigCount -= firstColItem->count;		bigSum -= firstColItem->sum;			bigSum2 -= firstColItem->sum2;
							firstColItem->count = count;			firstColItem->sum = localSum;			firstColItem->sum2 = localSum2;
							bigCount += count;					bigSum += localSum;					bigSum2 += localSum2;

							lFilterColumn *tmpItem = firstColItem;		firstColItem = firstColItem->next;




						}


						if (bigCount>0)
						{
							localMean = bigSum / (double)bigCount;
							localMean2 = bigSum2 / (double)bigCount;
							localStddev = localMean2 - localMean*localMean;
							kVal = localStddev / (localMean*localMean*stddev + localStddev);//k2
							fVal = localMean + kVal*(src[pos] - localMean);

							dest[pos] = fVal;
						}


						else
						{
							dest[pos] = src[j];
						}


						pos++;


					}
				}
			}
			//--------------- koniec bloku do optymalizacji ------------------
			//--------------- koniec bloku do optymalizacji ------------------
			//--------------- koniec bloku do optymalizacji ------------------
			//--------------- koniec bloku do optymalizacji ------------------
			//--------------- koniec bloku do optymalizacji ------------------
			//--------------- koniec bloku do optymalizacji ------------------
			//--------------- koniec bloku do optymalizacji ------------------
			//--------------- koniec bloku do optymalizacji ------------------

			//if (l==iter-1)	continue;
			double *tmp;
			tmp = src;
			src = dest;
			dest = tmp;
		}


		for (i = 0; i<size; i++)
		{
			delete myColTab[i];
		}
		delete[]myColTab;

		colList.clear();
		delete[]kernel;
		delete[]tab1;
		//-----------------------------------------------

		//if (iter % 2 == 0)
		{
			double *tmp;
			tmp = src;
			src = dest;
			dest = tmp;
		}


		for (i = 0; i<sliceSize; i++)
		{
			usData0[i] = dest[i];// *scale1;
		}
		updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

		delete[]src;
		delete[]dest;

		return 1;
	}

	return 1;

}
//-----------------------




//NOWOSCI!!!!!


LayerStatisticalDirectionalFilter::LayerStatisticalDirectionalFilter()
{
	filterName = "StatisticalDirectionalFilter";

	QStringList list;
	numberOfParams = 0;

	KFuncs[0] = &LayerStatisticalDirectionalFilter::K1m;
	KFuncs[1] = &LayerStatisticalDirectionalFilter::K2;
	KFuncs[2] = &LayerStatisticalDirectionalFilter::K3;
	KFuncs[3] = &LayerStatisticalDirectionalFilter::K3;
	KFuncs[4] = &LayerStatisticalDirectionalFilter::K4;
	KFuncs[5] = &LayerStatisticalDirectionalFilter::K2;
	KFuncs[6] = &LayerStatisticalDirectionalFilter::K3;
	KFuncs[7] = &LayerStatisticalDirectionalFilter::K3;

	//int radius1ID,radius2ID,finalValueID,rotPartsID,minFuncID;

	AddParam("radius1", 2, true, 1, 20, 1, radius1ID);
	AddParam("radius2", 2, true, 1, 20, 1, radius2ID);



	list.clear(); list << "center" << "sq-1-1" << "sq-1+1" << "sq+1-1" << "sq+1+1" << "tr-1-1" << "tr-1+1" << "tr+1-1" << "tr+1+1" << "tri+sq" << "tri+sq+rot" << "rot" << "half rot" << "rot +half";
	AddParam("type", 0, 2, 0, list.count() - 1, 1, typeID, &list);


	list.clear(); list << "localMean" << "sthFancy" << "min" << "max" << "median" << "weighedSum";
	AddParam("finalValue", 0, 2, 0, list.count() - 1, 1, finalValueID, &list);
	AddParam("testWeight", 1, true, 0, 1, 0.00001, testWeightID);
	AddParam("testWeight1", 0.00001, true, 1, 2, 0.0001, testWeight1ID);

	//AddParam("finalValue", 0,2,0,4,1,finalValueID, &list);



	AddParam("rot parts", 1, true, 1, 20, 1, rotPartsID);
	AddParam("startAngle", 0, 1, 0, 359, 1, startAngleID);
	list.clear(); list << "(1 - m2*s2) / (s2*(1 + sn2))" << "s2 / (m2*sn2 + s2)" << "(s2 - sn2) / s2;" << "(s2 / m2)" << "s2";//
	AddParam("minFunc", 0, 2, 0, list.count() - 1, 1, minFuncID, &list);


	list.clear(); list << "useMin" << "stdThresh" << "stdThresh2" << "scaledDirection";
	AddParam("finalValMode", 0, 2, 0, list.count() - 1, 1, finalValModeID, &list);

	AddParam("stdDevThresh", 0, true, 0, 0.001, 0.000001, stdDevThreshID);
	AddParam("valThresh", 0, true, 0, 1, 0.0001, valThreshID);

	list.clear(); list << "false" << "true";
	AddParam("maskFromPrevLayer", 0, true, 0, 1, 1, prevLayerID, &list);
	AddParam("maskPrevThresh", 0, true, 0, 254, 1, maskPrevThreshID);
	list.clear(); list << "false" << "true";
	AddParam("superTest", 0, true, 0, 1, 1, superTestID, &list);



	//this->paramsNames[numberOfParams]="radius1";			defaultParams[numberOfParams]=2;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="radius2";			defaultParams[numberOfParams]=2;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="mode";			defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=2;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;

	//	this->paramsNames[numberOfParams]="type";			defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="finalValue";		defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="rot parts";		defaultParams[numberOfParams]=1;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="minFunc";		defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=3;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="maskFromPrevLayer";		defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=1;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	this->paramsNames[numberOfParams]="maskPrevThresh";		defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=254;	this->steps[numberOfParams]=1;		numberOfParams++;

}
//-------------------------
int	LayerStatisticalDirectionalFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *desta, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{



	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;

	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j, k, l, m;

	//int mode=params[1];
	int iter = 1;//(mode==2)?2:1;

	unsigned short *src = usData0;
	unsigned short *dest = usData1;

	int sliceSize = mnx*mny;
	memcpy(src, tab, sliceSize * sizeof(unsigned short));

	double scale = 1;
	unsigned short maxv = mainDataMaxValue;
	double scale1 = 1.0 / scale;


	//int radius=params[0];
	//int type=params[1];

	//int iter=1;//params[1];
	//int optim=params[1];
	double startAngle = params[startAngleID];

	int finalValMode = params[finalValModeID];
	double stdDevThresh = params[stdDevThreshID];
	double valThresh = params[valThreshID] * maxv;


	int nz = 1;
	int nx = mnx;
	int ny = mny;
	int sx = nx;
	//	int sliceSize=nx*ny;
	int dsizemin1 = sliceSize - 1;
	//------------------------------------------


	int maxvalue = 255;
	int kernelSize = 0;
	int tmpPos;
	int count;


	int kernelSizes[100];
	int degs[] = { 0,45,90,135 };
	int *kernels[100];
	//unsigned short *kernelVals[100];

	DiffXKernelData *diffdata[100];

	int sxx = params[radius1ID];
	int syy = params[radius2ID];
	int radius = max(sxx, syy);
	//int kernelsNr=4;
	//for(i=0;i<kernelsNr;i++)
	//{
	//	diffdata[i]=new DiffXKernelData();
	//	//kernels[i]=GetGrowingKernelDirectional2D(kernelSizes[i],radius,data,diffdata[i],2,degs[i],sxx,syy); 
	//	kernels[i]=GetGrowingKernelDirectional2D(kernelSizes[i],radius,data,diffdata[i],1,i); 
	//}


	double (LayerStatisticalDirectionalFilter::*func)(double, double, double, double);
	int knr = params[minFuncID];
	if (knr<0) knr = 0; else if (knr>3) knr = 3;
	func = (this->KFuncs[knr]);
	//CFuncs

	bool maskPrev = params[prevLayerID];
	unsigned char maskThresh = params[maskPrevThreshID];

	double testWeight = params[testWeightID];
	double testWeight1 = params[testWeight1ID];
	int pos = 0;

	int sizexx;

	i = params[typeID];
	int rotParts = params[rotPartsID];
	if (rotParts<1) rotParts = 1;
	int tmpRadius;
	RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
	switch (i)
	{
	case 0:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], -1, 0, radius - 1, radius - 1); pos++; //center
		break;
	case 1:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 0, 0, radius - 1, radius - 1); pos++;//center
		break;
	case 2:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 0, 1, radius - 1, radius - 1); pos++;//center
		break;
	case 3:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 0, 2, radius - 1, radius - 1); pos++;//center
		break;
	case 4:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 0, 3, radius - 1, radius - 1); pos++;//center
		break;
	case 5:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 1, 0, radius - 1, radius - 1); pos++;//center
		break;
	case 6:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 1, 1, radius - 1, radius - 1); pos++;//center
		break;
	case 7:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 1, 2, radius - 1, radius - 1); pos++;//center
		break;
	case 8:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 1, 3, radius - 1, radius - 1); pos++;//center
		break;
	case 9:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], -1, 0, radius - 1, radius - 1); pos++; //center
		for (i = 0; i<4; i++)
		{
			diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 0, i, radius - 1, radius - 1); pos++;//center
		}

		for (i = 0; i<4; i++)
		{
			diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 1, i, radius - 1, radius - 1); pos++;//center
		}
		break;


	case 10:

		tmpRadius = radius;
		radius = 2;

		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], -1, 0, radius - 1, radius - 1); pos++; //center
		for (i = 0; i<4; i++)
		{
			diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 0, i, radius - 1, radius - 1); pos++;//center
		}

		for (i = 0; i<4; i++)
		{
			diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 1, i, radius - 1, radius - 1); pos++;//center
		}


		radius = tmpRadius;
		//elyptical
		sizexx = 180 / rotParts;

		pos = 0;
		for (int rr = startAngle; rr<startAngle + 180; rr += sizexx)
		{
			diffdata[pos] = new DiffXKernelData();
			kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 2, rr, sxx, syy);
			pos++;
		}

		break;


	case 11:

		//elyptical
		sizexx = 180 / rotParts;

		pos = 0;
		for (int rr = startAngle; rr<startAngle + 180; rr += sizexx)
		{
			diffdata[pos] = new DiffXKernelData();
			kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 2, rr, sxx, syy);
			pos++;
		}


		break;

	case 12:

		//elyptical half
		sizexx = 360 / rotParts;

		pos = 0;
		for (int rr = startAngle; rr<startAngle + 360; rr += sizexx)
		{
			diffdata[pos] = new DiffXKernelData();
			kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 3, rr, sxx, syy);
			pos++;
		}


		break;
	case 13:

		//elyptical +half

		sizexx = 180 / ((int)(rotParts / 2) + 1);// o polowe mniej

		pos = 0;
		for (int rr = startAngle; rr<startAngle + 180; rr += sizexx)
		{
			diffdata[pos] = new DiffXKernelData();
			kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 2, rr, sxx, syy);
			pos++;
		}
		sizexx = 360 / rotParts;

		//pos = 0;
		for (int rr = startAngle; rr<startAngle + 360; rr += sizexx)
		{
			diffdata[pos] = new DiffXKernelData();
			kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], 3, rr, sxx, syy);
			pos++;
		}


		break;
	default:
		diffdata[pos] = new DiffXKernelData(); kernels[pos] = GetGrowingKernelDirectional2D(kernelSizes[pos], radius, data, diffdata[pos], -1, 0, radius - 1, radius - 1); pos++; //center
		break;



	}

	int finalValue = params[finalValueID];
	delete data;

	int kernelsNr = pos;
	int maxKernelSize = 0;
	//test
	bool superTest = params[superTestID];
	if (superTest)
	{
		int *kernel;
		int KernelSize;
		for (int m = 0; m < kernelsNr; m++)
		{
			/*vals = kernelVals[m];
			vals[0] = 0;*/
			kernel = kernels[m];
			kernelSize = kernelSizes[m];

			int *minusKernel = diffdata[m]->kernelMinus;
			int *plusKernel = diffdata[m]->kernelPlus;
			int plusSize = diffdata[m]->kernelPlusSize;
			int minusSize = diffdata[m]->kernelMinusSize;

			//zrobie 3 rysunki dla kazdego kernela - albo calosc dam na 1
			QImage img(500, 500, QImage::Format_RGB32);
			img.fill(Qt::black);
			QPainter Painter2(&img);
			Painter2.setBrush(Qt::white);
			Painter2.setPen(2);
			int addVal = (radius)+(radius)*mnx;
			for (int i = 0; i < kernelSize; i++)
			{
				int y = (kernel[i] + addVal) / mnx;
				int x = (kernel[i] + addVal) - y*mnx;

				/*	x += 20;
				y += 20;*/
				Painter2.drawRect(x * 10, y * 10, 10, 10);
			}
			img.save("d:\\testX\\" + QString::number(m) + ".png");
			img.fill(Qt::black);

			Painter2.setBrush(Qt::white);
			Painter2.setPen(2);
			for (int i = 0; i < minusSize; i++)
			{
				int y = (minusKernel[i] + addVal) / mnx;
				int x = (minusKernel[i] + addVal) - y*mnx;
				Painter2.drawRect(x * 10, y * 10, 10, 10);
			}
			img.save("d:\\testX\\" + QString::number(m) + "minus.png");
			img.fill(Qt::black);

			Painter2.setBrush(Qt::white);
			Painter2.setPen(2);
			for (int i = 0; i < plusSize; i++)
			{
				int y = (plusKernel[i] + addVal) / mnx;
				int x = (plusKernel[i] + addVal) - y*mnx;

				Painter2.drawRect(x * 10, y * 10, 10, 10);
			}
			img.save("d:\\testX\\" + QString::number(m) + "plus.png");
		}


	}



	//---------------



	for (int i = 0; i < kernelsNr; i++)
	{
		int ss = kernelSizes[i];
		if (ss > maxKernelSize) maxKernelSize = ss;
		/*if ((ss > 10000) || (ss <= 0))
		{
		ss = ss;
		}*/
		//	kernelVals[i] = new unsigned short[ss + 1];
	}

	int *lSum = new int[kernelsNr];
	int *lSum2 = new int[kernelsNr];
	int *lCount = new int[kernelsNr];
	double *localMeans = new double[kernelsNr];
	double *localMeans2 = new double[kernelsNr];
	double *localStddevs = new double[kernelsNr];
	double *localkVals = new double[kernelsNr];
	double *localfVals = new double[kernelsNr];
	pos = 0;
	//int l;

	unsigned short *maxTab = new unsigned short[maxKernelSize];

	double stddev, mean, mean2;
	double localMean, localStddev, localMean2;
	double nx1d = 1.0 / nx;

	double kVal, kVal1, fVal;
	int size = radius * 2 + 1;


	double div1 = (maxv>0) ? 1.0 / maxv : 1;
	double div2 = div1*div1;

	unsigned short *tab1a;
	int sizes;
	double val;
	long bigSum;
	double bigSum2;
	int bigCount;
	int *kernel;
	int x, y, x1, y1;


	double stdDevScale;

	double xScaler = (double)mainDataMaxValue / (kernelsNr);
	for (l = 0; l<iter; l++)
	{

		pos = 0;


		bigSum = 0;
		bigSum2 = 0;

		for (i = 0; i<sliceSize; i++)
		{
			val = src[i];
			bigSum += val;
			bigSum2 += val*val*div2;// to moze przekroczyc zakres
		}

		mean = (bigSum*div1) / (double)sliceSize;
		mean2 = (bigSum2) / (double)sliceSize;
		//unsigned short *vals;
		stddev = sqrt(mean2 - mean*mean);
		{
			//wersja bez listy - singlepass

			int tmpA;
			pos = 0;
			for (j = 0; j<ny; j++)
			{

				for (i = 0; i<nx; i++)
				{

					y = j*nx1d;
					x = j - y*nx;

					if (i == 0)
					{
						//int pos1=pos;
						for (int m = 0; m<kernelsNr; m++)
						{
							/*vals = kernelVals[m];
							vals[0] = 0;*/
							kernel = kernels[m];
							kernelSize = kernelSizes[m];


							count = 0;
							int localSum = 0;
							int localSum2 = 0;

							for (k = 0; k<kernelSize; k++)
							{
								tmpPos = pos + kernel[k];


								if (!((tmpPos>dsizemin1) || (tmpPos<0)))
								{

									val = src[tmpPos];		localSum += val;			localSum2 += val*val;				count++;
									//	vals[vals[0] + 1] = val; vals[0]++;

								}
							}

							lCount[m] = count;		lSum[m] = localSum;			lSum2[m] = localSum2;

						}

					}
					else
					{

						for (int m = 0; m<kernelsNr; m++)
						{
							/*vals = kernelVals[m];
							vals[0] = 0;*/
							kernel = kernels[m];
							kernelSize = kernelSizes[m];

							int *minusKernel = diffdata[m]->kernelMinus;
							int *plusKernel = diffdata[m]->kernelPlus;
							int plusSize = diffdata[m]->kernelPlusSize;
							int minusSize = diffdata[m]->kernelMinusSize;

							int localSum = 0;
							int localSum2 = 0;
							int count = 0;
							int xpos = pos - 1;
							for (k = 0; k<minusSize; k++)
							{
								tmpPos = xpos + minusKernel[k];
								if (!((tmpPos>dsizemin1) || (tmpPos<0)))
								{

									val = src[tmpPos];		localSum -= val;			localSum2 -= val*val;				count--;
									//vals[vals[0] + 1] = val; vals[0]++;
								}
							}

							for (k = 0; k<plusSize; k++)
							{
								tmpPos = xpos + plusKernel[k];
								if (!((tmpPos>dsizemin1) || (tmpPos<0)))
								{

									val = src[tmpPos];		localSum += val;			localSum2 += val*val;				count++;
									//vals[vals[0] + 1] = val; vals[0]++;
								}
							}

							lCount[m] += count;		lSum[m] += localSum;			lSum2[m] += localSum2;

						}


					}

					int minSel = 0;
					bool anyCount = false;
					double meanx = -1;
					double std;
					double fval;
					double minVal;
					//if (bigCount>0)



					fVal = 65535;
					minVal = 65535;

					double stdSum = 0;
					double stdSum2 = 0;
					double stdDevDevMin = INT_MAX;
					double stdDevDevMax = -INT_MAX;
					for (int m = 0; m<kernelsNr; m++)
					{
						if (lCount[m]>0)
						{
							anyCount = true;
							localMeans[m] = (lSum[m] * div1) / (double)lCount[m];
							localMeans2[m] = (lSum2[m] * div2) / (double)lCount[m];
							localStddevs[m] = localMeans2[m] - localMeans[m] * localMeans[m];
							localkVals[m] = localStddevs[m] / (localMeans[m] * localMeans[m] * stddev + localStddevs[m]);//k2
							localfVals[m] = localMeans[m] + localkVals[m] * (src[pos] * div1 - localMeans[m]);
							fVal = (this->*func)(localMeans[m] * localMeans[m], localStddevs[m], 0, stddev);

							stdSum += localStddevs[m];
							stdSum2 += localStddevs[m] * localStddevs[m];

							if (fVal<minVal)
							{
								//fVal=localStddevs[m];//localfVals[m];
								//meanx=localMeans[m];
								minSel = m;
								minVal = fVal;

							}

							if (localStddevs[m] > stdDevDevMax) stdDevDevMax = localStddevs[m];
							if (localStddevs[m] < stdDevDevMin) stdDevDevMin = localStddevs[m];
							//					qDebug(QString::number(minSel));
						}
					}

					//dest[pos]=fVal;


					double mean = (double)stdSum / (double(kernelsNr));
					double mean2 = (double)stdSum2 / (double)(kernelsNr);

					double stddevStd = mean2 - mean*mean;//odchylenie standardowe dla odchylen standardowych :)
					double diffStdDevs = stdDevDevMax - stdDevDevMin;
					if (stddevStd > 0)
					{
						i = i;
					}



					//if (stddevStd > stdDevDevMax) stdDevDevMax = stddevStd;
					//if (stddevStd < stdDevDevMin) stdDevDevMin = stddevStd;


					//double meanStd;
					double meanStd2;
					double stdStd;

				//	meanStd /= kernelsNr;

					bool tt = ((maskPrev == true) && (prevImg[pos] * scale <= maskThresh));// przepuszczam tylko 

					if (tt == true)
					{
						i = i;
					}

					if ((anyCount == true) && (tt == false))
					{
						int countMin = 0;
						if (finalValue > 1)
						{
							//pobieram caly kernel dla wybranego czegostam i licze min max albo med

							//int pos1=pos;
							int m = minSel;


							kernel = kernels[m];
							kernelSize = kernelSizes[m];


							countMin = 0;
							int localSum = 0;
							int localSum2 = 0;

							for (k = 0; k<kernelSize; k++)
							{
								tmpPos = pos + kernel[k];


								if (!((tmpPos>dsizemin1) || (tmpPos<0)))
								{

									val = src[tmpPos];		localSum += val;			localSum2 += val*val;
									maxTab[countMin] = val;
									//	vals[vals[0] + 1] = val; vals[0]++;
									countMin++;
								}
							}





						}


						switch (finalValue)
						{
						case 0:

							fVal = localMeans[minSel];
							break;

						case 1:

							fVal = localfVals[minSel];
							break;

						case 2:
							//min


							fVal = Minimum(maxTab, countMin - 1);
							if (fVal > 0)
							{
								i = i;
							}
							break;
						case 3:
							//max


							fVal = Maximum(maxTab, countMin - 1);
							break;
						case 4:
							//med



							fVal = Median(maxTab, countMin - 1);
							break;
						case 5:
							fVal = 0;
							for (int m = 0; m < kernelsNr; m++)
							{
								fVal += localMeans[m] * pow(localStddevs[m] * testWeight1, testWeight);
							}
							if (fVal > 0)
							{
								i = i;
							}
							break;

							//	break;
						default:

							fVal = localMeans[minSel];
							break;
						}
						int finalVal = 0;
						if (fVal < 2)
						{
							finalVal = (fVal > 1) ? maxv : fVal*maxv;
						}
						else
						{
							finalVal = fVal;
						}

						switch (finalValMode)
						{
						case 0:

							dest[pos] = finalVal;

							break;

						case 1:

							if ((diffStdDevs > stdDevThresh) && (finalVal >= valThresh))
							{
								dest[pos] = finalVal;
							}
							else
							{
								dest[pos] = 0;
							}

							break;

						case 2:
							dest[pos] = finalVal;


							break;
						case 3:
							dest[pos] = xScaler*minSel;
							break;

						default:
							dest[pos] = finalVal;
							break;

						}
					}
					else
					{
						dest[pos] = src[pos];
					}


					pos++;


				}
			}
		}


		//	if (l==iter-1)	continue;
		unsigned short *tmp;
		tmp = src;
		src = dest;
		dest = tmp;
	}



	for (i = 0; i<kernelsNr; i++)
	{
		delete[]kernels[i];
		//	delete[]kernelVals[i];

		diffdata[i] = new DiffXKernelData();
	}




	//-----------------------------------------------


	if (usData0 != src)
	{
		unsigned short *tmp;
		tmp = usData1;
		usData1 = usData0;
		usData0 = tmp;
	}
	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

	delete[]maxTab;

	delete[]lSum;
	delete[]lSum2;
	delete[]lCount;
	delete[]localMeans;
	delete[]localMeans2;
	delete[]localStddevs;
	delete[]localkVals;
	delete[]localfVals;

	return 1;

}
//-----------------------



LayerStatisticalFilterDualCross::LayerStatisticalFilterDualCross()
{
	filterName = "StatisticalFilterDualCross";
	//numberOfParams=0;
	QStringList list;



	AddParam("radius", 2, true, 1, 100, 1, radiusID);
	list.clear(); list << "-" << "|" << "+";
	AddParam("mode", 0, true, 0, 2, 1, modeID, &list);

	//this->paramsNames[numberOfParams]="radius";			defaultParams[numberOfParams]=10;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="mode";			defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=2;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;


}
//-------------------------
int	LayerStatisticalFilterDualCross::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *adest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{



	/*if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;
	*/
	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j, k, l, m;

	int mode = params[modeID];
	int iter = (mode == 2) ? 2 : 1;


	//bool createUcharBuffers= ((src!=NULL)&&(dest!=NULL))?false:true;

	//if (createUcharBuffers==true)
	//{
	//	src=new unsigned char[sliceSize];
	//	dest=new unsigned char[sliceSize];
	//}
	//unsigned short *src = tab;
	unsigned short *src = usData0;
	unsigned short *dest = usData1;

	int sliceSize = mnx*mny;
	memcpy(src, tab, sliceSize * sizeof(unsigned short));

	double scale = 1;
	unsigned short maxv = mainDataMaxValue;
	double scale1 = 1.0 / scale;


	int radius = params[radiusID];



	int nz = 1;
	int nx = mnx;
	int ny = mny;
	int sx = nx;
	//	int sliceSize=nx*ny;
	int dsizemin1 = sliceSize - 1;
	//------------------------------------------


	int maxvalue = 255;
	int kernelSize = 0;
	int tmpPos;
	int count;

	RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
	int *kernely = GetGrowingKernel1D(kernelSize, radius, data, 1, true);
	int *kernelx = GetGrowingKernel1D(kernelSize, radius, data, 0, true);


	delete data;

	int pos = 0;
	//int l;

	double stddev, mean, mean2;
	double localMean, localStddev, localMean2;
	double nx1d = 1.0 / nx;

	double kVal, kVal1, fVal;
	int size = radius * 2 + 1;


	double div1 = 1.0 / maxv;
	double div2 = 1.0 / (maxv*maxv);

	lFilterColumn2 **myColTab = new lFilterColumn2*[size];
	lFilterColumn2 *firstColItem;
	lFilterColumn2 *currentItem;
	for (i = 0; i<size; i++)
	{
		myColTab[i] = new lFilterColumn2;
		if (i>0) myColTab[i - 1]->next = myColTab[i];
	}
	myColTab[i - 1]->next = myColTab[0];;//loop

	firstColItem = myColTab[0];
	double val;
	long bigSum;
	double bigSum2;
	int bigCount;

	// iter=1;	
	for (l = 0; l<iter; l++)
	{
		pos = 0;		bigSum = 0;		bigSum2 = 0;

		for (i = 0; i<sliceSize; i++)
		{
			val = src[i];
			bigSum += val;
			bigSum2 += val*val*div2;// to moze przekroczyc zakres
		}

		mean = (bigSum*div1) / (double)sliceSize;		mean2 = (bigSum2) / (double)sliceSize;		stddev = sqrt(mean2 - mean*mean);

		//wersja bez listy - singlepass

		int tmpA;

		//ypass
		if ((mode == 1) || ((mode == 2) && (l == 1)))
		{
			pos = 0;
			for (j = 0; j<ny; j++)
			{

				for (i = 0; i<nx; i++)
				{

					count = 0;						double localSum = 0;						double localSum2 = 0;
					{
						for (k = 0; k<kernelSize; k++)
						{
							tmpPos = pos + kernely[k];
							if (!((tmpPos>dsizemin1) || (tmpPos<0)))
							{
								val = src[tmpPos];	localSum += val;	localSum2 += val*val;		count++;
							}
						}
					}
					bigCount = count;		bigSum = localSum;	bigSum2 = localSum2;

					if (bigCount>0)
					{
						localMean = (bigSum*div1) / (double)bigCount;						localMean2 = (bigSum2*div2) / (double)bigCount;						localStddev = localMean2 - localMean*localMean;
						kVal = localStddev / (localMean*localMean*stddev + localStddev);//k2
						fVal = localMean + kVal*(src[pos] * div1 - localMean);
						dest[pos] = fVal*maxv;
					}
					else
					{
						dest[pos] = src[j];
					}
					pos++;
				}
			}
		}
		else
			//xpass
			if ((mode == 0) || ((mode == 2) && (l == 0)))
			{
				pos = 0;
				for (j = 0; j<ny; j++)
				{
					for (i = 0; i<nx; i++)
					{
						count = 0;						double localSum = 0;						double localSum2 = 0;
						{
							for (k = 0; k<kernelSize; k++)
							{
								tmpPos = pos + kernelx[k];
								if (!((tmpPos>dsizemin1) || (tmpPos<0)))
								{
									val = src[tmpPos];	localSum += val;	localSum2 += val*val;		count++;
								}
							}
						}
						bigCount = count;		bigSum = localSum;	bigSum2 = localSum2;

						if (bigCount>0)
						{
							localMean = (bigSum*div1) / (double)bigCount;						localMean2 = (bigSum2*div2) / (double)bigCount;						localStddev = localMean2 - localMean*localMean;
							kVal = localStddev / (localMean*localMean*stddev + localStddev);//k2
							fVal = localMean + kVal*(src[pos] * div1 - localMean);
							dest[pos] = fVal*maxv;
						}
						else
						{
							dest[pos] = src[j];
						}
						pos++;
					}
				}
			}


		//if (l==iter-1)	continue;
		unsigned short *tmp;
		tmp = src;
		src = dest;
		dest = tmp;
	}




	for (i = 0; i<size; i++)
	{
		delete myColTab[i];
	}
	delete[]myColTab;


	delete[]kernely;
	delete[]kernelx;

	//delete []tab1;
	//-----------------------------------------------


	/*for(i=0;i<sliceSize;i++)
	{
	usData0[i]=dest[i]*scale1;
	}*/

	if (usData0 != src)
	{
		unsigned short *tmp;
		tmp = usData1;
		usData1 = usData0;
		usData0 = tmp;
	}


	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

	/*if (createUcharBuffers==true)
	{
	delete []src;
	delete []dest;
	}*/



	return 1;

}
//-----------------------

















LayerStatisticalFilter1Faster2::LayerStatisticalFilter1Faster2()
{
	filterName = "StatisticalFilter1Faster2";
	//numberOfParams=0;


	QStringList list;
	list.clear(); list << "false" << "true";

	AddParam("radius", 2, true, 1, 20, 1, radiusID);
	AddParam("iter", 1, true, 1, 10, 1, iterID);
	AddParam("alpha", 1, true, 0.1, 1.0, 0.001, alphaID);

	AddParam("use Prev Layer", 0, true, 0, 1, 1, prevLayerID, &list);
	AddParam("maskPrevThresh", 0, true, 0, 254, 1, maskPrevThreshID);

	AddParam("useWeightMap", 0, true, 0, 1, 1, useWeightMapID, &list);
	AddParam("amount", 1.0, true, 0, 1, 0.01, amountID);
	AddParam("invertWeights", 0, true, 0, 1, 1, invertWeightsID, &list);




	//this->paramsNames[numberOfParams]="radius";			defaultParams[numberOfParams]=10;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="iter";			defaultParams[numberOfParams]=1;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=30;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="alpha";			defaultParams[numberOfParams]=0.3;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0.001;		this->maxVals[numberOfParams]=1.0;	this->steps[numberOfParams]=0.001;		numberOfParams++;
	////this->paramsNames[numberOfParams]="optim";			defaultParams[numberOfParams]=3;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=3;	this->steps[numberOfParams]=1;		numberOfParams++;
	//			this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;

	//this->paramsNames[numberOfParams]="maskFromPrevLayer";		defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=1;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="maskPrevThresh";		defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=254;	this->steps[numberOfParams]=1;		numberOfParams++;
}
//-------------------------
int	LayerStatisticalFilter1Faster2::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *desta, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{
	int iter = params[iterID];
	double alpha = params[alphaID];
	double alpha1 = 1.0 - alpha;
	double alpha255 = alpha * 255;


	bool useWeightMap = params[useWeightMapID];
	double amount = params[amountID];
	bool invertWeights = params[invertWeightsID];;


	bool usePrevLayer = params[prevLayerID];
	if ((usePrevLayer == true) && (prevImg == NULL))	usePrevLayer = false;
	unsigned char maskThresh = params[maskPrevThreshID];

	////if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	////unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;

	unsigned short *src = usData0;
	unsigned short *dest = usData1;
	int sliceSize = mnx*mny;
	memcpy(src, tab, sliceSize * sizeof(unsigned short));
	int i, j, k, l, m;


	if (usePrevLayer == true)
	{
		tab = prevImg;

	}


	//bool createUcharBuffers= ((src!=NULL)&&(dest!=NULL))?false:true;

	//if (createUcharBuffers==true)
	//{
	//	src=new unsigned char[sliceSize];
	//	dest=new unsigned char[sliceSize];
	//}


	double scale = 1;
	unsigned short maxv = mainDataMaxValue;
	if (maxv>0)
	{
		//if (maxv>256)
		{
			scale = 255.0 / maxv;
		}
	}
	double scale1 = 1.0 / scale;


	int radius = params[radiusID];




	int nz = 1;
	int nx = mnx;
	int ny = mny;
	int sx = nx;
	//	int sliceSize=nx*ny;
	int dsizemin1 = sliceSize - 1;
	//------------------------------------------


	int maxvalue = 255;
	int kernelSize = 0;
	int tmpPos;
	int count;

	RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
	int *kernel = GetGrowingKernel1D(kernelSize, radius, data, 1, true);
	delete data;



	int pos = 0;
	//int l;

	double stddev, mean, mean2;
	double localMean, localStddev, localMean2;
	double nx1d = 1.0 / nx;

	double kVal, kVal1, fVal;
	int size = radius * 2 + 1;


	double div1 = 1.0 / 255.0;
	double div2 = 1.0 / (255.0*255.0);

	lFilterColumn2 **myColTab = new lFilterColumn2*[size];
	lFilterColumn2 *firstColItem;
	lFilterColumn2 *currentItem;
	for (i = 0; i<size; i++)
	{
		myColTab[i] = new lFilterColumn2;
		if (i>0) myColTab[i - 1]->next = myColTab[i];
	}
	myColTab[i - 1]->next = myColTab[0];;//loop

	firstColItem = myColTab[0];
	double val;
	long bigSum;
	double bigSum2;
	int bigCount;

	// iter=1;	
	for (l = 0; l<iter; l++)
	{

		pos = 0;


		bigSum = 0;
		bigSum2 = 0;

		for (i = 0; i<sliceSize; i++)
		{
			val = src[i];
			bigSum += val;
			bigSum2 += val*val*div2;// to moze przekroczyc zakres
		}

		mean = (bigSum*div1) / (double)sliceSize;
		mean2 = (bigSum2) / (double)sliceSize;

		stddev = sqrt(mean2 - mean*mean);
		{
			//wersja bez listy - singlepass

			int tmpA;
			pos = 0;
			for (j = 0; j<ny; j++)
			{

				for (i = 0; i<nx; i++)
				{
					if (i == 0)
					{
						//int pos1=pos;
						bigSum = 0;
						bigSum2 = 0;
						bigCount = 0;
						tmpA = -radius;
						currentItem = firstColItem;
						for (int k1 = 0; k1<size; k1++)
						{
							count = 0;
							int localSum = 0;
							int localSum2 = 0;
							if (tmpA++ >= 0)// radius pierwszych elementow jest poza obrazem=0
							{

								for (k = 0; k<kernelSize; k++)
								{
									tmpPos = pos + k1 - radius + kernel[k];
									if (!((tmpPos>dsizemin1) || (tmpPos<0)))
									{

										val = src[tmpPos];		localSum += val;			localSum2 += val*val;				count++;
									}
								}
							}
							currentItem->count = count;		currentItem->sum = localSum;			currentItem->sum2 = localSum2;
							bigCount += count;				bigSum += localSum;					bigSum2 += localSum2;

							currentItem = currentItem->next;
						}

						i = i;
					}
					else
					{
						//dobierz nowy bufor na koncu i wywal stary
						//wypelnij pierwszy slupek nastepnym pobranym
						//przerzuc pierwszy slupek na koniec listy

						count = 0;
						double localSum = 0;
						double localSum2 = 0;
						//if (tmpA++>=0)// radius pierwszych elementow jest poza obrazem=0
						if (i<nx - radius - 1)
						{

							for (k = 0; k<kernelSize; k++)
							{
								tmpPos = pos + radius + kernel[k];
								if (!((tmpPos>dsizemin1) || (tmpPos<0)))
								{
									val = src[tmpPos];	localSum += val;	localSum2 += val*val;		count++;
									//if (pos==49664+128)
									/*	if ((i>128-22)&&(i<=128)&&(j==97))
									{
									qDebug(QString::number(src[tmpPos]));
									}*/
								}
							}
						}

						bigCount -= firstColItem->count;		bigSum -= firstColItem->sum;			bigSum2 -= firstColItem->sum2;
						firstColItem->count = count;			firstColItem->sum = localSum;			firstColItem->sum2 = localSum2;
						bigCount += count;					bigSum += localSum;					bigSum2 += localSum2;

						firstColItem = firstColItem->next;
					}

					if (pos == 49664 + 128)
					{
						i = i;
					}

					//					bool tt=((maskPrev==true)&&(prevImg[pos]*scale<=maskThresh));// przepuszczam tylko

					if ((bigCount>0))//&&(tt==false))
					{
						localMean = (bigSum*div1) / (double)bigCount;
						localMean2 = (bigSum2*div2) / (double)bigCount;
						localStddev = localMean2 - localMean*localMean;
						kVal = localStddev / (localMean*localMean*stddev + localStddev);//k2
						fVal = localMean + kVal*(src[pos] * div1 - localMean);

						dest[pos] = src[pos] * alpha1 + fVal*alpha255;
					}


					else
					{
						dest[pos] = src[pos];
					}


					pos++;


				}
			}
		}


		//	if (l==iter-1)	continue;
		unsigned short *tmp;
		tmp = src;
		src = dest;
		dest = tmp;
	}


	for (i = 0; i<size; i++)
	{
		delete myColTab[i];
	}
	delete[]myColTab;


	delete[]kernel;
	//delete []tab1;
	//-----------------------------------------------

	unsigned short *tmp;
	tmp = src;
	src = dest;
	dest = tmp;

	for (i = 0; i<sliceSize; i++)
	{
		usData0[i] = dest[i];
	}



	if ((useWeightMap == true) && (prevImg != NULL))
	{
		unsigned short maxWeightVal = 0;
		for (i = 0; i<sliceSize; i++)
		{
			if (prevImg[i]>maxWeightVal) maxWeightVal = prevImg[i];
		}

		ApplyWeightMap(tab, usData0, usData0, prevImg, maxWeightVal, sliceSize, amount, invertWeights);
	}



	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);


	//if (createUcharBuffers==true)
	//{
	//	delete []src;
	//	delete []dest;
	//}



	return 1;

}
//-----------------------


//------------------------------------

LayerStatisticalFilter1::LayerStatisticalFilter1()
{
	filterName = "StatisticalFilter1";
	//numberOfParams=0;

	//	KFuncs[0]=&LayerStatisticalFilter1::K1;
	//	KFuncs[1]=&LayerStatisticalFilter1::K2;
	//	KFuncs[2]=&LayerStatisticalFilter1::K3;
	QStringList list;
	AddParam("radius", 2, true, 1, 20, 1, radiusID);
	list << "square" << "circle";
	AddParam("kerenl type", 0, true, 0, 1, 1, kernelTypeID, &list);
	AddParam("iter", 1, true, 1, 20, 1, iterID);



	//this->paramsNames[numberOfParams]="radius";			defaultParams[numberOfParams]=10;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="kernel type";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=1;	this->steps[numberOfParams]=1;		numberOfParams++;
	////	this->paramsNames[numberOfParams]="K Function";		defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;				maxVals[numberOfParams]=2;		steps[numberOfParams]=1;		numberOfParams++;

	//this->paramsNames[numberOfParams]="iter";			defaultParams[numberOfParams]=1;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=1;		this->maxVals[numberOfParams]=100;	this->steps[numberOfParams]=1;		numberOfParams++;

	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;

}
//-------------------------
int	LayerStatisticalFilter1::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	/*if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;*/
	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);
	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j, k, l;

	double coef = 0.1 / 6.0;// lambda=0.1, k=6 (6 neighbors)


	{
		double *src, *dest;
		//unsigned short *buff0=usData0;
		//unsigned short *buff1=usData1;
		//jesli rozmiar range raw data jest wiekszy od 255 to robie przeskalowanie

		src = new double[sliceSize];
		dest = new double[sliceSize];
		for (int i = 0; i < mnx*mny; i++)
		{
			src[i] = tab[i];
		}

		double scale = 1;
		double maxv = mainDataMaxValue;



		if (maxv>0)
		{
			//if (maxv>256)
			{
				scale = 1.0 / maxv;
			}
		}
		double scale1 = 1.0 / scale;



		// tu odpalenie filterka
		int radius = params[radiusID];
		int type = params[kernelTypeID];
		//	int knr=1;//params[2];
		int iter = params[iterID];



		//	double (LayerStatisticalFilter1::*func)(double,double,double,double);
		//int cnr=params[0];
		//	if (knr<0 ) knr=0; else if (knr>2) knr=2;
		//func=(this->KFuncs[knr]);
		//CFuncs


		int nz = 1;
		int nx = mnx;
		int ny = mny;
		int sx = nx;
		int sliceSize = nx*ny;
		int dsizemin1 = sliceSize - 1;
		//------------------------------------------


		int maxvalue = 255;
		int kernelSize = 0;
		int tmpPos;
		int count;
		RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
		int *kernel = GetGrowingKernel2D(kernelSize, radius, data, type, true);
		delete data;
		double *tab1 = new double[kernelSize];
		//double *tab1=new double[2*maxvalue];


		int pos = 0;
		//int l;

		double stddev, mean;
		double localMean, localStddev;
		double nx1d = 1.0 / nx;

		double kVal, kVal1, fVal;
		for (l = 0; l<iter; l++)
		{
			pos = 0;

			//policze tutaj globalna stddev i srednia
			mean = 0;
			for (i = 0; i<sliceSize; i++)
			{
				//tab1[i]=(this->*func)((double)i,k0);
				mean += src[i];
			}
			mean /= (double)sliceSize;

			stddev = 0;

			for (i = 0; i<sliceSize; i++)
			{
				stddev += (mean - src[i])*(mean - src[i]);
			}

			stddev /= (double)sliceSize;
			//	mean*=mean;
			int x, y;
			int x1, y1;

			for (j = 0; j<sliceSize; j++)
			{
				count = 0;
				localMean = 0;
				localStddev = 0;
				y = j*nx1d;
				x = j - y*nx;
				for (i = 0; i<kernelSize; i++)
				{
					tmpPos = j + kernel[i];

					y1 = tmpPos*nx1d;
					x1 = tmpPos - y1*nx;
					if ((x1 >= x - radius) && (x1 <= x + radius))
						if ((y1 >= y - radius) && (y1 <= y + radius))
							if (!((tmpPos>dsizemin1) || (tmpPos<0)))//||(y!=oldy+kernelInc[i]))) //brzydkie
							{
								tab1[count] = src[tmpPos];
								localMean += tab1[count];
								count++;
								/*if (j==49664+128)
								{
								qDebug(QString::number(src[tmpPos]));
								}*/
							}
				}
				localMean /= (double)count;



				if ((count>0) && (localMean>0))
				{

					for (i = 0; i<count; i++)
					{
						localStddev += (localMean - tab1[i])*(localMean - tab1[i]);
					}

					localStddev /= count; //sigma^2
										  ///localMean*=localMean; //mean^2


										  // mam lokalna miare stddev i mean w ototczeniu o typie kwadratowym albo kolowym
										  //kVal=(this->*func)(localMean*localMean,localStddev,0,stddev);
										  /*
										  double K1(double m2, double s2,double mn2,double sn2)	{return (1-m2*s2)/(s2*(1+sn2));}
										  double K2(double m2, double s2,double mn2,double sn2)	{return s2/(m2*sn2+s2);}
										  double K3(double m2, double s2,double mn2,double sn2)	{return (s2-sn2)/s2;}
										  */

										  //kVal=K2(localMean*localMean,localStddev,0,stddev);
					kVal = localStddev / (localMean*localMean*stddev + localStddev);//k2
					fVal = localMean + kVal*(src[j] - localMean);
					//kVal=

					//	if (fVal>255) fVal=255; else if (fVal<0) fVal=0;
					if (fVal < 0) fVal = 0; else if (fVal > maxv) fVal = maxv;
					dest[j] = fVal;

				}
				else
				{
					dest[j] = src[j];
				}


			}

			//	if (l==iter-1)	continue;
			double *tmp;
			tmp = src;
			src = dest;
			dest = tmp;
		}

		delete[]kernel;
		delete[]tab1;
		//-----------------------------------------------

		//if (iter % 2 == 0)
		{
			double *tmp;
			tmp = src;
			src = dest;
			dest = tmp;
		}


		for (i = 0; i<sliceSize; i++)
		{
			usData0[i] = dest[i];// *scale1;
		}
		updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

		delete[]src;
		delete[]dest;
		return 1;
	}

	return 1;

}
//-----------------------
LayerGaussianFilter::LayerGaussianFilter()
{
	filterName = "LayerGaussianFilter";
	//numberOfParams=0;


	QStringList list;
	AddParam("radius", 2, 1, 1, 100, 1, radiusID);
	AddParam("stddev", 0.3, 1, 0.01, 2, 0.01, stddevID);

	list.clear(); list << "false" << "true";
	AddParam("useWeightMap", 0, 1, 0, 1, 1, useWeightMapID, &list);
	AddParam("amount", 1.0, 1, 0, 1, 0.01, amountID);
	AddParam("invertWeights", 0, 1, 0, 1, 1, invertWeightsID, &list);



}
//-------------------------
int	LayerGaussianFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);
	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j;
	//	bool revth=maskThresholds[2];
	unsigned char val1, val2;



	radius = params[radiusID];
	stddev = params[stddevID];
	bool useWeightMap = params[useWeightMapID];
	double amount = params[amountID];
	bool invertWeights = params[invertWeightsID];;


	if (radius<1) return 0;
	int					size2 = 2 * radius + 1;
	double				*gaussianKernelTab = new double[size2];
	unsigned short		*intensityTab = new unsigned short[size2];
	GetGaussianKernel(gaussianKernelTab, (double)radius, stddev);


	unsigned short *Q;

	int pos;

	Q = tab;
	pos = 0;
	int n[2] = { mnx, mny };

	unsigned short *tmpBuff = tab;



	unsigned short **tabz;
	//#pragma omp parallel private (tabz) num_threads(4)
	{
		unsigned short *datatab = tab;
		tabz = new unsigned short *[nrOfThreads];
		//#pragma omp critical
		{
			for (i = 0; i<nrOfThreads; i++)
			{
				tabz[i] = new unsigned short[size2];
			}
		}
		//	int tid = omp_get_thread_num();
		;// cout << tid''
#pragma omp parallel for  ordered schedule (guided) 
		for (int j = 0; j < mny; j++)
		{
			int pos = j*mnx;
			int  tid = omp_get_thread_num();
			for (int i = 0; i < mnx; i++)
			{

				GetIntensityTab1D(tmpBuff, n[0], n[1], 1, 0, radius, tabz[tid], i, j, 0);
				buff0[pos++] = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
			}
		}

		//y-filtering
		pos = 0;

		int val;
#pragma omp parallel for  ordered schedule (guided) 
		for (int j = 0; j < mny; j++)
		{
			int pos = j*mnx;
			int  tid = omp_get_thread_num();
			for (int i = 0; i<mnx; i++)
			{
				GetIntensityTab1D(buff0, n[0], n[1], 1, 1, radius, tabz[tid], i, j, 0);
				buff1[pos++] = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
			}
		}
		//#pragma omp critical
		{
			for (i = 0; i<nrOfThreads; i++)
			{
				delete[]tabz[i]; tabz[i] = NULL;
			}
			delete[]tabz;
		}
	}

	//swap buffers
	buffTmp = usData0;
	usData0 = usData1;
	usData1 = buffTmp;
	delete[]gaussianKernelTab;
	delete[]intensityTab;

	if ((useWeightMap == true) && (prevImg != NULL))
	{
		unsigned short maxWeightVal = 0;
		for (i = 0; i<sliceSize; i++)
		{
			if (prevImg[i]>maxWeightVal) maxWeightVal = prevImg[i];
		}

		ApplyWeightMap(tab, usData0, usData0, prevImg, maxWeightVal, sliceSize, amount, invertWeights);
	}




	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);
	return 1;

}
//-----------------------



LayerAnisotropicDiffusionFilter::LayerAnisotropicDiffusionFilter()
{
	filterName = "AnisotropicDiffusionFilter";
	ConductivityFuncs[0] = &LayerAnisotropicDiffusionFilter::C1;
	ConductivityFuncs[1] = &LayerAnisotropicDiffusionFilter::C2;
	ConductivityFuncs[2] = &LayerAnisotropicDiffusionFilter::C3;
	ConductivityFuncs[3] = &LayerAnisotropicDiffusionFilter::C4;
	ConductivityFuncs[4] = &LayerAnisotropicDiffusionFilter::C5;

	//	numberOfParams=0;

	//int cfunctionID,iterID,k0ID,aID,dtID;

	QStringList list;
	list << "c1" << "c2" << "Turkey" << "Huber" << "Weickert";
	AddParam("C Function", 0, 1, 0, 4, 1, cfunctionID, &list);
	AddParam("iter", 2, 1, 1, 20, 1, iterID);
	AddParam("K0", 5, 1, 0.01, 20, 0.1, k0ID);
	AddParam("a", 10, 1, 0.0, 100, 0.01, aID);
	AddParam("dt", 0.05, 1, 0.01, 10, 0.001, dtID);
	AddParam("scaler", 0.01, 1, 0.01, 2, 0.01, scalerID);

	//this->paramsNames[numberOfParams]="C Function";		defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;		maxVals[numberOfParams]=4;		steps[numberOfParams]=1;		numberOfParams++;
	////this->paramsNames[numberOfParams]="Iterations";		defaultParams[numberOfParams]=15;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=1;		maxVals[numberOfParams]=100;	steps[numberOfParams]=1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="K0";				defaultParams[numberOfParams]=5;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;		maxVals[numberOfParams]=20;		steps[numberOfParams]=0.1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="a";				defaultParams[numberOfParams]=10;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;		maxVals[numberOfParams]=10;		steps[numberOfParams]=0.1;		numberOfParams++;
	//this->paramsNames[numberOfParams]="dt";				defaultParams[numberOfParams]=0.05;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;		maxVals[numberOfParams]=1;		steps[numberOfParams]=0.001;	numberOfParams++;

	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;

}
//-------------------------
int	LayerAnisotropicDiffusionFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *desta, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);
	unsigned short *src = usData1;
	unsigned short *dest = usData0;
	unsigned short *buffTmp;
	double scaleFinal = params[scalerID];
	int i, j, k, l;

	double coef = 0.1 / 6.0;// lambda=0.1, k=6 (6 neighbors)

	double (LayerAnisotropicDiffusionFilter::*func)(double, double);
	int cnr = params[cfunctionID];
	if (cnr<0) cnr = 0; else if (cnr>4) cnr = 4;
	func = (this->ConductivityFuncs[cnr]);




	//bool createUcharBuffers= ((src!=NULL)&&(dest!=NULL))?false:true;

	//if (createUcharBuffers==true)
	//{
	//	src=new unsigned char[sliceSize];
	//	dest=new unsigned char[sliceSize];
	//}


	double scale = 1;
	double maxv = mainDataMaxValue;
	if (maxv>0)
	{
		/*if (maxv>256)
		{
		scale=255.0/maxv;
		}*/
		scale = maxv;
	}
	double scale1 = 1.0 / scale;

	memcpy(src, tab, sliceSize * sizeof(unsigned short));

	// tu odpalenie filterka

	int iter = params[iterID];
	double k0 = params[k0ID];
	double a = params[aID];
	double dt = params[dtID];
	int nz = 1;
	int nx = mnx;
	int ny = mny;
	int sx = nx;
	//------------------------------------------

	//ctmf(src,dest,mnx,mny,mnx,mnx, radius, 1,512*1024);
	//	int maxvalue = GetMaxValue(tab, sliceSize);
	int maxValue = this->mainDataMaxValue;
	if (maxValue == 0) maxValue = 1;

	double *tab1 = new double[2 * maxValue];

	//	double proggress=0;
	//double dproggress=100.0/(nz*iter);
	int pos = 0;
	//int l;
	double scaler = 255.0 / maxValue;
	double scaler1 = 1.0 / scaler;
	for (l = 0; l<iter; l++)
	{
		pos = 0;
		for (int i = 0; i<2 * maxValue; i++)
		{
			tab1[i] = (this->*func)((double)i*scaler, k0);
		}

		// parallel_for(0,nz, [&](int k) 
		//for (int k=0;k<nz;k++)

#pragma omp parallel for  ordered schedule (guided) 
		for (int k = 0; k<nz; k++)
		{
			int i, j;
			double tmp;
			int grad;
			double val0;
			int nr;
			int pos = k*nx*ny;
			for (j = 0; j<ny; j++)
				for (i = 0; i<nx; i++)
				{
					nr = 0;
					tmp = 0;

					val0 = src[pos];

					if (val0 > 0)
					{
						i = i;
					}
					if (i<nx - 1)
					{	//x+1

						grad = src[pos + 1] - val0;

						tmp += grad*tab1[abs(grad)];
						nr++;
					}
					if (i >0)
					{	//x-1
						grad = src[pos - 1] - val0;

						tmp += grad*tab1[abs(grad)];
						nr++;
					}
					if (j<ny - 1)
					{	//y+1

						grad = src[pos + sx] - val0;

						tmp += grad*tab1[abs(grad)];
						nr++;
					}
					if (j>0)
					{	//y-1

						grad = src[pos - sx] - val0;

						tmp += grad*tab1[abs(grad)];
						nr++;
					}


					if (nr != 0) coef = 1.0 / nr;
					tmp *= scaleFinal*coef;


					if (tmp > 0)
					{
						i = i;
					}
					else if (tmp < 0)
					{
						i = i;
					}

					val0 = (int)(src[pos] + tmp);

					if (val0<0) val0 = 0; else if (val0>maxValue) val0 = maxValue;

					dest[pos] = val0;

					if (fabs((double)src[pos] - dest[pos]) > 5)
					{
						i = i;
					}
					pos++;


				}
			//proggress+=dproggress;
			//emit SignalSendProgress((int)proggress);
		};

		k0 += a*dt;
		//if (l<iter-1)
		//TempData->MakeCopyOfDataArray(FinalData->GetDataArray());
		//swap
	//	if (l == iter - 1)	continue;
		unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
	}

	delete[]tab1;
	//-----------------------------------------------



	if (usData0 != src)
	{
		unsigned short *tmp;
		tmp = usData1;
		usData1 = usData0;
		usData0 = tmp;
	}

	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);





	return 1;

}

//-------------------------------------------------------------------------
LayerSapheniaFilter::LayerSapheniaFilter()
{
	filterName = "SapheniaFilter";
	success = false;
	/*numberOfParams=0;

	this->paramsNames[numberOfParams]="RescaleIntercept";		defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-32768;		maxVals[numberOfParams]=65535;		steps[numberOfParams]=1;	addText[numberOfParams]="float";	numberOfParams++;
	this->paramsNames[numberOfParams]="RescaleSlope";			defaultParams[numberOfParams]=1;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;			maxVals[numberOfParams]=1000;		steps[numberOfParams]=1;	addText[numberOfParams]="float";	numberOfParams++;
	this->paramsNames[numberOfParams]="MinimumIntensity";		defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-32768;		maxVals[numberOfParams]=65535;		steps[numberOfParams]=1;	addText[numberOfParams]="float";	numberOfParams++;
	this->paramsNames[numberOfParams]="MaximumIntensity";		defaultParams[numberOfParams]=255;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-32768;		maxVals[numberOfParams]=65535;		steps[numberOfParams]=1;	addText[numberOfParams]="float";	numberOfParams++;
	this->paramsNames[numberOfParams]="BackgroundIntensity";	defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-32768;		maxVals[numberOfParams]=65535;		steps[numberOfParams]=1;	addText[numberOfParams]="float";	numberOfParams++;
	this->paramsNames[numberOfParams]="Invert";					defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;			maxVals[numberOfParams]=1;			steps[numberOfParams]=1;	addText[numberOfParams]="bool";		numberOfParams++;
	this->paramsNames[numberOfParams]="Focus";					defaultParams[numberOfParams]=37;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;			maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;

	this->paramsNames[numberOfParams]="NoiseNearField";			defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="ContrastNearField";		defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="SharpenNearField";		defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="EffectNearField";		defaultParams[numberOfParams]=80;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;			maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;

	this->paramsNames[numberOfParams]="NoiseFocus";				defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="ContrastFocus";			defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="SharpenFocus";			defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="EffectFocus";			defaultParams[numberOfParams]=70;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;			maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;

	this->paramsNames[numberOfParams]="NoiseFarField";			defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="ContrastFarField";		defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="SharpenFarField";		defaultParams[numberOfParams]=0;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=-100;		maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;
	this->paramsNames[numberOfParams]="EffectFarField";			defaultParams[numberOfParams]=80;	paramsSliders[numberOfParams]=true;		minVals[numberOfParams]=0;			maxVals[numberOfParams]=100;		steps[numberOfParams]=1;	addText[numberOfParams]="int32";	numberOfParams++;


	*/
#if defined(USG)

	char *parameterFile = "draminski_v1-2.spf";
	char *type = "u8";

	scppStatus = SCPPCreateEx(&scppHandle, errorMsg, ERR_MSG_LEN);
	if (scppStatus != SCPP_ERR_OK)
	{
		fprintf(stderr, "SCPPCreateEx: %s\n", errorMsg);
		return;// 0;
	}

	scppStatus = SCPPReadParameterFile(scppHandle, parameterFile,
		&nParameterBlocks, &parameterBlockNames, &nSettings);
	if (scppStatus != SCPP_ERR_OK)
	{
		fprintf(stderr, "SCPPReadParameterFile: %s\n", SCPPGetErrorMessage(scppHandle));
		return;//0;
	}

	blockNbr = 0;
	setting = 0;


	scppStatus = SCPPGetProcessingInfo(scppHandle, parameterBlockNames[blockNbr],
		setting, &processingInfo);


	const TSCPPTuningPanelInfo **tpInfo = new const TSCPPTuningPanelInfo*[0];
	const char *parameterBlockName;//------------------
	int setting = 0;

	scppStatus = SCPPGetTuningPanelInfo(scppHandle, parameterBlockNames[blockNbr], setting, tpInfo);




	if (scppStatus != SCPP_ERR_OK)
	{
		fprintf(stderr, "SCPPGetProcessingInfo: %s\n", SCPPGetErrorMessage(scppHandle));
		return;//0;
	}

	TSCPPTPParameterInfo *tmpInfo = (*tpInfo)->parameterInfo;
	const char **value = new const const char*[0];
	do
	{


		this->paramsNames[numberOfParams] = tmpInfo->name;

		paramsSliders[numberOfParams] = true;
		minVals[numberOfParams] = QString(tmpInfo->min).toDouble();
		maxVals[numberOfParams] = QString(tmpInfo->max).toDouble();
		steps[numberOfParams] = 1;
		addText[numberOfParams] = tmpInfo->type;

		if (addText[numberOfParams].compare("bool") == 0)
		{
			minVals[numberOfParams] = 0;
			maxVals[numberOfParams] = 1;
		}




		scppStatus = SCPPGetTuningPanelValue(scppHandle, parameterBlockNames[blockNbr], setting, tmpInfo->name, value);

		defaultParams[numberOfParams] = QString(value[0]).toDouble();




		numberOfParams++;



		tmpInfo = tmpInfo->pNext;

	} while (tmpInfo != NULL);


	//tmpInfo=tpInfo[0];



	success = true;
#endif


}
//-------------------------
//-------------------------
int	LayerSapheniaFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{
#if defined(USG)


	bool createUcharBuffers = ((src != NULL) && (dest != NULL)) ? false : true;
	if ((sliceNr<0) || (sliceNr >= data->GetNz()) || (success == false)) return 0;
	unsigned short *tab = (dataTab == NULL) ? data->GetDataArray() + (sliceNr*sliceSize) : dataTab;
	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);



	//update params!!!!!!!!!!!!!!!



	const TSCPPTuningPanelInfo **tpInfo = new const TSCPPTuningPanelInfo*[0];
	const char *parameterBlockName;//------------------
	int setting = 0;

	scppStatus = SCPPGetTuningPanelInfo(scppHandle, parameterBlockNames[blockNbr], setting, tpInfo);


	if (scppStatus != SCPP_ERR_OK)
	{
		fprintf(stderr, "SCPPGetProcessingInfo: %s\n", SCPPGetErrorMessage(scppHandle));
		return 0;//0;
	}

	TSCPPTPParameterInfo *tmpInfo = (*tpInfo)->parameterInfo;
	int nr = 0;
	do
	{


		this->paramsNames[nr] = tmpInfo->name;



		//const char **value=new const char*[0];
		if (addText[nr].compare("bool") == 0)
		{
			if (params[nr] == 0)
			{
				scppStatus = SCPPSetTuningPanelValue(scppHandle, parameterBlockNames[blockNbr], setting, tmpInfo->name, "false");
			}
			else
			{
				scppStatus = SCPPSetTuningPanelValue(scppHandle, parameterBlockNames[blockNbr], setting, tmpInfo->name, "true");
			}
		}
		else
		{
			scppStatus = SCPPSetTuningPanelValue(scppHandle, parameterBlockNames[blockNbr], setting, tmpInfo->name, QString::number(params[nr]));
		}
		//defaultParams[numberOfParams]=QString(value[0]).toDouble();	

		nr++;



		tmpInfo = tmpInfo->pNext;

	} while (tmpInfo != NULL);




	//-------------------------------------------------











	//	double stddev=params[0];
	//	int radius=params[0];
	//
	//	if (radius<1) return 0;
	//unsigned char *src,*dest;
	//unsigned short *buff0=usData0;
	//unsigned short *buff1=usData1;
	//jesli rozmiar range raw data jest wiekszy od 255 to robie przeskalowanie
	if (createUcharBuffers == true)
	{
		src = new unsigned char[sliceSize];
		dest = new unsigned char[sliceSize];
	}
	//	char *parameterFile = "draminski_v1-2.spf";
	char *type = "u8";




	//SCPPHandle scppHandle; // A handle to a SCPP instance
	//SCPPError scppStatus; // An error code variable
	//int nParameterBlocks; // Number of parameter blocks in parameter file
	//const char **parameterBlockNames; // Parameter block names in parameter file
	//const int *nSettings; // Number of settings in each parameter block
	//char *
	//	parameterFile = "draminski_v1-2.spf"; // Parameter file (just an example)

	//char *parameterFile = "testDestructSingleFrame.spf";
	//int hgh,wid; // Image height and width
	//char *type="u8"; // Image pixel type (just an example)
	//#define ERR_MSG_LEN 2048
	//char errorMsg[ERR_MSG_LEN]; // Error Message from SCPPCreateEx()


	// A handle to an instance of SCPP must be created.
	// Several instances of SCPP can be created if necessary.





	//int blockNbr=0; // Use the first parameter block in this example.
	// The possible range is [0..(nParameterBlocks-1)]
	//int setting=0; // Use setting 0 in this example. Possible is [0..(nSettings[blockNbr]-1)]

	// We need to find out what type of processing the selected block
	// and setting contains in the loaded parameter file.
	//TSCPPProcessingInfo processingInfo;
	/*scppStatus = SCPPGetProcessingInfo(scppHandle, parameterBlockNames[blockNbr],
	setting, &processingInfo);*/


	//SetTuningPanelValue("RescaleIntercept",0,



	//processingInfo.processingType=SCPP_PROCESSING_SINGLE_FRAME;

	double scale1;



	double scale = 1;
	double maxv = mainDataMaxValue;
	if (maxv>0)
	{
		if (maxv>256)
		{
			scale = 255.0 / maxv;
		}
	}
	scale1 = 1.0 / scale;


	for (int i = 0; i<sliceSize; i++)
	{
		src[i] = tab[i] * scale;
	}


	//while( 1 ) 
	//{ 
	//	// Loop until all images have been processed
	//	void *pInBuffer=(void*)src; // Input image buffer
	//	void *pOutBuffer=(void*)dest; // Output image buffer (of same size as input buffer)
	//	// Allocate and read input image into inBuffer here.
	//	// Allocate output buffer of same size as input buffer.
	//	// Set wid, hgh and image type.
	//	// Then process the image with this call:
	//	hgh=mny;
	//	wid=mnx;


	//


	//	scppStatus = SCPPProcessImage(scppHandle, pInBuffer, hgh, wid, type, pOutBuffer,
	//	parameterBlockNames[blockNbr], setting);
	//	if( scppStatus != SCPP_ERR_OK )
	//	{
	//		fprintf(stderr,"SCPPProcessImage: %s\n", SCPPGetErrorMessage(scppHandle));
	//		return 0;
	//	}
	//	// Optionally: Get the min and max values in the result image
	//	// if these values are needed e.g. in a DICOM header
	//	// Note: This operation should only be performed if necessary.
	//	float minPixel, maxPixel;
	//	scppStatus = SCPPGetImageMinMax(scppHandle, pOutBuffer,
	//	hgh, wid, type, &minPixel, &maxPixel);
	//	if( scppStatus != SCPP_ERR_OK ) 
	//	{
	//		fprintf(stderr,"SCPPGetImageMinMax: %s\n", SCPPGetErrorMessage(scppHandle));
	//		return 0;
	//	}
	//	// Save the processed result from the output image buffer
	//}









	//case SCPP_PROCESSING_SEQUENCE:{
	// The processing type is sequence. The processing of image sequences can use the
	// the information in the images ?before? (and sometimes ?after?) the current image
	// to produce a better processing result.
	// The processingInfo.nInputImages value tells how many input
	// images are needed to produce one output image.
	// E.g. a value of 1 means that only one image is needed, a value of 2 means that
	// also the next image in the sequence is needed and so on.
	// Note: For processing of ultrasound/fluoro images the value is usually 1. I.e. only
	// the current image is needed.
	// The output image is always the processing result for the first input image.
	// (The number of output images is always one at the moment.)
	int nInputImages = 1; // The number of images in the image sequence
	void **pInputImages = new void *[1];
	pInputImages[0] = src;
	// Vector of pointers to input images (size should be
	// processingInfo.nInputImages)
	void **pOutputImages = new void *[1];
	pOutputImages[0] = dest;
	//void *pOutputImages[];// Vector of pointers to output images (size should be
	// processingInfo.nOutputImages, i.e. 1 at the moment)
	hgh = mny;
	wid = mnx;
	for (int outNbr = 0; outNbr < nInputImages; outNbr++)
	{
		for (int inNbr = outNbr; inNbr< outNbr + processingInfo.nInputImages; inNbr++)
		{
			int i = inNbr - outNbr;
			if (inNbr< nInputImages)
				pInputImages[i] = src;// <set to pointer to image #inNbr in the image sequence>
			else
				pInputImages[i] = (void *)0; // No more images available. Set to 0;
		}
		pOutputImages[0] = dest;// <set pointer to output buffer>; //(just one 1 at the moment)
		if (outNbr == 0)
		{
			// Process the first image in the sequence
			scppStatus = SCPPProcessImageSequenceFirst(scppHandle, pInputImages, hgh, wid, type,
				pOutputImages,
				parameterBlockNames[blockNbr], setting);
		}
		else
		{
			// Process the remaining images in the sequence
			scppStatus = SCPPProcessImageSequenceNext(scppHandle, pInputImages, hgh, wid, type,
				pOutputImages,
				parameterBlockNames[blockNbr], setting);
		}
		if (scppStatus != SCPP_ERR_OK)
		{
			fprintf(stderr, "SCPPProcessImageSequence: %s\n", SCPPGetErrorMessage(scppHandle));
			return -1;
		}
		// Optionally: Get the min and max values in the result image
		// if these values are needed e.g. in a DICOM header
		// Note: This operation should only be performed if necessary.
		float minPixel, maxPixel;


		//	hgh, wid, type, &minPixel, &maxPixel);

		/*	scppStatus = SCPPGetImageMinMax(scppHandle, pOutputImages[0],	hgh, wid, type, &minPixel, &maxPixel);

		if( scppStatus != SCPP_ERR_OK )
		{
		fprintf(stderr,"SCPPGetImageMinMax: %s\n", SCPPGetErrorMessage(scppHandle));
		return -1;
		}*/
		// Save the processed result from the output image buffer
	}




	//scppStatus = SCPPProcessImage(scppHandle, pInBuffer, hgh, wid, type, pOutBuffer,
	//	parameterBlockNames[blockNbr], setting);


	for (int i = 0; i<sliceSize; i++)
	{
		usData0[i] = dest[i] * scale1;
	}


	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

	if (createUcharBuffers == true)
	{
		delete[]src;
		delete[]dest;
	}
#endif
	return 1;
}
//----------------------------

LayerMeasureFilter1::LayerMeasureFilter1()
{


	expMax = 0;
	intensityFactor = 0;
	expTab = NULL;// new double[2 * expMax + 2];
	samplingTab1 = NULL;
	samplingTabIndex = NULL;

	filterName = "MeasureFilter1";

	AddParam("sigma", 1.5, true, 0.1, 5.0, 0.01, sigmaID);
	AddParam("intensityFactor", 0.5, true, 0.001, 1, 0.001, intensityFactorID);
	QStringList list;
	list.clear(); list << "false" << "true";
	AddParam("CleanShapes", 1, true, 0, 1, 1, cleanShapesID, &list);

}
LayerMeasureFilter1::~LayerMeasureFilter1()
{
	if (expTab != NULL) delete[]expTab;
	if (samplingTab1 != NULL)	delete[]samplingTab1;
	if (samplingTabIndex != NULL)	delete[]samplingTabIndex;
}
//-------------------------
int	LayerMeasureFilter1::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);

	//	double stddev=params[0];

	memset(usData0, 0, sliceSize * sizeof(unsigned short));

	QList<QPolygonF> *slist = &shapes->polygonList;
	int nrPolygons = 0;

	nrPolygons = shapes->polygonList.count();
	//int i,j;
	bool cleanShapes = params[cleanShapesID];
	shapes->cleanPolygonFlag = cleanShapes;


	double mSigma = params[sigmaID];
	unsigned short maxValue = (mainDataMaxValue > 0) ? mainDataMaxValue : 1;// data->GetMaxValue();

	bool sigmaChg = fabs(mSigma - sigma) < 1e-5 ? false : true;

	bool intensChg = fabs(intensityFactor - params[intensityFactorID])< 1e-5 ? false : true;
	intensityFactor = params[intensityFactorID];

	if ((expMax != maxValue) || (sigmaChg == true) || (intensChg == true))
	{

		sigma = mSigma;
		if (expTab != NULL)delete[]expTab;
		expMax = maxValue;
		expTab = new double[expMax * 2 + 1];

		if (samplingTab1 != NULL)delete[]samplingTab1;
		samplingTab1 = new unsigned short[expMax];
		if (samplingTabIndex != NULL)delete[]samplingTabIndex;
		samplingTabIndex = new unsigned short[expMax];


		//intensityFactor = 1.0 / (double)(expMax*expMax);
		GetExpTab(expTab, expMax, sigma, intensityFactor);

	}


	memset(samplingTab1, 0, expMax * sizeof(unsigned short));

	if (nrPolygons>0)
	{
		QImage img(mnx, mny, QImage::Format_RGB32);
		img.fill(Qt::black);
		QPainter Painter2(&img);
		Painter2.setBrush(Qt::white);
		Painter2.setPen(2);


		for (int j = 0; j<nrPolygons; j++)
		{
			MyShape s;
			s.poly = slist->at(j);
			Painter2.drawPolygon(s.poly);
		}



		unsigned char *tab11 = img.bits();
		unsigned int *tabx = (unsigned int *)tab11;
		int count = 0;
		memset(usData1, 0, sliceSize * sizeof(unsigned short));
		for (int i = 0; i<sliceSize; i++)
		{
			if ((*tabx++) == 0xffffffff)
			{
				usData1[i] = 255;
				count++;
			}
		}


		int samplesCount = count;

		if (samplesCount>0)
		{
			//unsigned short *samplingTab=new unsigned short[expMax];
			//memset(samplingTab,0,expMax*sizeof(unsigned short));

			//	double *intensityProbabilityTab=new double[expMax];

			int posx = 0;
			for (int i = 0; i<sliceSize; i++)
			{
				if (usData1[i] == 255)
				{
					samplingTab1[tab[i]]++;
				}
			}

			int maxPosNew = 0;
			for (int i = 0; i < expMax; i++)
			{
				if (samplingTab1[i]>0)
				{
					samplingTab1[maxPosNew] = samplingTab1[i];
					samplingTabIndex[maxPosNew] = i;
					maxPosNew++;

					//teraz wystarczy biec po index tab do maxpos new zeby dobrac sie do wszystkich
				}

			}



			//double min,max;
			//GetIntensityProbabilityTab(intensityProbabilityTab,samplesCount,min,max,sigma,expTab,samplingTab,expMax,expMax,samplesCount);//ta	sie	przyda
			//double factor=(max-min!=0)?255/(max-min):1;


			//int mmin = 65535;
			//int mmax = 0;

			double *intensityProbabilityTab1 = new double[expMax];
			double min1, max1;
			GetIntensityProbabilityTab2(intensityProbabilityTab1, expMax, min1, max1, sigma, expTab, samplingTab1, samplingTabIndex, maxPosNew, samplesCount);//ta	sie	przyda
			double factor1 = (max1 - min1 != 0) ? 4096.0 / (max1 - min1) : 1;

			int mmin = 65535;
			int mmax = 0;
			for (int i = 0; i<sliceSize; i++)
			{
				usData0[i] = (unsigned short)((intensityProbabilityTab1[tab[i]] - min1)*factor1);
				if (usData0[i] < mmin)mmin = usData0[i];
				if (usData0[i]>mmax)mmax = usData0[i];
			}

			delete[]intensityProbabilityTab1;

			//delete []intensityProbabilityTab;
			//delete []samplingTab;

			QString text =
				"count: " + QString::number(count) + "\n" +
				"mmin: " + QString::number(mmin) + "\n" +
				"mmax: " + QString::number(mmax) + "\n";

			MyShape shape;						shape.shapeText = QString(text);
			shape.textPos.setX(5);				shape.textPos.setY(10 + 128);
			shape.textColor.setRgb(255, 0, 0);	shapes->shapeList.append(shape);

		}



	}

	//if (mainMask!=NULL)
	//{
	//	int i;
	//	for(i=0;i<sliceSize;i++)
	//	{
	//		if (mainMask[i]==1) usData0[i]=128;
	//		//	usData0[i]=mainMask[i]*255;
	//	}
	//}

	/*int xpos = 0;
	for (int i = 0; i < mny;i++)
	for (int j = 0; j < mnx; j++)
	{
	usData0[xpos++] = (i*j)%this->mainDataMaxValue;
	}*/
	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);
	return 1;

}
//----------------------------

LayerMixerLiteFilter::LayerMixerLiteFilter()
{
	init = false;
	filterName = "LayerMixerLiteFilter";
	//AddParam("radius", 2,true,1,100,1,radiusID);


}
//----------------------------
LayerMixerLiteFilter::~LayerMixerLiteFilter()
{
	if (init == true)
	{
		FiltDram_Free();
	}
}
//-------------------------
int	LayerMixerLiteFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	char *parameterFile = "filter_compositions//grad test.layers";

	init = FiltDram_Initialization(parameterFile, mnx, mny, mdx, mdy, 0);//
																		 //	}

																		 //jesli rozmiar range raw data jest wiekszy od 255 to robie przeskalowanie
	bool createUcharBuffers = ((src != NULL) && (dest != NULL)) ? false : true;

	if (createUcharBuffers == true)
	{
		src = new unsigned char[sliceSize];
		dest = new unsigned char[sliceSize];
	}
	/*src=new unsigned char[sliceSize];
	dest=new unsigned char[sliceSize];*/


	double scale = 1;
	double maxv = mainDataMaxValue;
	if (maxv>0)
	{
		if (maxv>256)
		{
			scale = 255.0 / maxv;
		}
	}
	double scale1 = 1.0 / scale;





	// tu odpalenie filterka
	FiltDram_Filtruj(src, dest);
	//ctmf(src,dest,mnx,mny,mnx,mnx, radius, 1,512*1024);


	for (int i = 0; i<sliceSize; i++)
	{
		usData0[i] = dest[i] * scale1;
	}

	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);
	//	delete []src;
	//	delete []dest;
	if (createUcharBuffers == true)
	{
		delete[]src;
		delete[]dest;
	}
	FiltDram_Free();
	return 1;
}

//---------------------------------
//LayerMutualIntensityAnalysisFilter::LayerMutualIntensityAnalysisFilter()
//{
//	filterName="LayerMutualIntensityAnalysisFilter";
//	numberOfParams=0;
/*
int leftThresholdID,rightThresholdID;
int useThresholdsID;
int useHistLogScaleID;

int showRegionInfoID;
int showInfoPlaceID;

int showMutualStatsID;

int showRealRegionBoundsID;
*/
LayerMutualIntensityAnalysisFilter::LayerMutualIntensityAnalysisFilter()
{
	filterName = "LayerMutualIntensityAnalysisFilter";

	QStringList list;




	list.clear(); list << "Transversal" << "Coronal" << "Sagittal";
	AddParam("func", 0, 2, 0, 2, 1, viewID, &list);

	list.clear(); list << "ssd" << "cross" << "entropy" << "MI" << "NMI";
	AddParam("func", 4, true, 0, 4, 1, functionID, &list);









	list.clear(); list << "false" << "true";
	AddParam("Tx", 0, true, -64, 64, 0.01, translateXID);
	AddParam("Ty", 0, true, -64, 64, 0.01, translateYID);
	AddParam("rot", 0, true, -3.14, 3.14, 0.01, rotateID);

	//AddParam ("lefthTh",0,true,0,1,0.001,leftThresholdID);
	//AddParam ("righthTh",1,true,0,1,0.001,rightThresholdID);
	//AddParam ("invertThreshods",0,true,0,1,1,invertThreshodsID);


	AddParam("sliceNumber", 0, true, 0, 512, 1, sliceNumberID);
	AddParam("showMutualStats", 1, true, 0, 1, 1, showMutualStatsID, &list);
	AddParam("scatterMultiply", 1, true, 1, 10, 0.01, scatterMultiplyID);

	//AddParam ("useBiggerMax",1,true,0,1,1,useBiggerMaxID,&list);
	//AddParam ("scatterUseThresholds",1,true,0,1,1,scatterUseThresholdsID,&list);

	list.clear(); list << "8" << "16" << "32" << "64" << "128" << "256";
	AddParam("scatterSize", 5, true, 0, 5, 1, scatterSizeID, &list);

	AddParam("mutualHistSize", 30, true, 10, 200, 1, mutualHistSizeID);
	list.clear(); list << "false" << "true";
	AddParam("useHistLogScale", 1, true, 0, 1, 1, useHistLogScaleID, &list);
	//AddParam ("showRealRegionBounds",1,true,0,1,1,showRealRegionBoundsID,&list);
	//AddParam ("showRegionInfo",1,true,0,1,1,showRegionInfoID,&list);
	//AddParam ("showReg1",1,true,0,1,1,showReg1ID,&list);
	//AddParam ("showReg2",1,true,0,1,1,showReg2ID,&list);

	list.clear(); list << "center" << "left" << "right" << "top" << "bottom";
	AddParam("showInfoPlace", 1, true, 0, 4, 1, showInfoPlaceID, &list);







}
//------------------------------------
int	LayerMutualIntensityAnalysisFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{
	if (viewType == params[viewID])
	{
		bool hideGUI = false;

		double tr[10];
		tr[0] = 0;
		tr[1] = params[translateXID];
		tr[2] = params[translateYID];
		tr[3] = params[rotateID];

		// inparams:
		if ((nrInParams > 0) && (inParams != NULL))
		{
			hideGUI = (bool)(inParams[0]);
			// 0 - hide gui

			// 1 - tx
			// 2 - ty
			// 3 - rot

			tr[1] = (double)(inParams[1]);
			tr[2] = (double)(inParams[2]);
			tr[3] = (double)(inParams[3]);


		}

		RawDataSet *modelData = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, false);
		RawDataSet *objectData = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, false);




		int sliceNumber = params[sliceNumberID];


		int sliceNr = 0;// (sliceNumber%dv);


						//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;


		modelData->SetDataArray(prevImg);
		objectData->SetDataArray(tab);

		modelData->FindMinMaxValue();
		objectData->FindMinMaxValue();
		//tab - source - z odpowiedniego przkroju
		//usDAta0 = wynik
		//prevImg - prevDataSource

		//---------------------------------------------------------------------------




		bool	useHistLogScale = params[useHistLogScaleID];
		//bool	showRealRegionBounds=params[showRealRegionBoundsID];
		//bool	showRegionInfo=params[showRegionInfoID];
		//int		showInfoPlace=params[showInfoPlaceID];
		bool	showMutualStats = params[showMutualStatsID];
		//int		sliceSize=sliceSize;
		//int		showReg1=params[showReg1ID];
		//int		showReg2=params[showReg2ID];
		int		scatterSize = params[scatterSizeID] + 3;
		int		mutualHistSize = params[mutualHistSizeID];

		//	bool	scatterUseThresholds=params[scatterUseThresholdsID];
		//bool	useBiggerMax=params[useBiggerMaxID];


		double scatterMultiply = params[scatterMultiplyID];
		shapes->cleanPolygonFlag = true;


		int currentMax = mainDataMaxValue;

		int prevMax = 0;
		for (int i = 0; i < sliceSize; i++)
		{
			if (prevImg[i] > prevMax) prevMax = prevImg[i];
		}


		unsigned char val1, val2;




		//bool revth=params[invertThreshodsID];
		//if (revth==false){	val1=255;val2=0;}else{	val1=0;val2=255;}


		QList<QPolygonF> *slist = &shapes->polygonList;
		int nrPolygons = shapes->polygonList.count();

		unsigned char *buffer = new unsigned char[sliceSize];


		//	memset(buffer,255,sliceSize);

		//*******************************
		bool polyExist = false;
		int bounds[4];
		unsigned int *imgMaskBits = NULL;
		QPolygonF polygon;

		if (nrPolygons > 0)
		{
			polygon = slist->at(0);
			if (polygon.size() > 2)polyExist = true;
		}


		qreal x1, x2, y1, y2;
		QImage img(mnx, mny, QImage::Format_RGB32);
		if (polyExist == true)
		{

			QRectF rect = polygon.boundingRect();
			rect.getCoords(&x1, &y1, &x2, &y2);


			img.fill(Qt::black);
			QPainter Painter2(&img);
			Painter2.setBrush(Qt::white);
			Painter2.setPen(1);
			Painter2.drawPolygon(polygon);

			imgMaskBits = (unsigned int *)(img.bits());

		}
		else
		{
			x1 = 0;
			y1 = 0;
			x2 = mnx - 1;
			y2 = mny - 1;
		}
		bounds[0] = (int)x1;
		bounds[2] = (int)y1;
		bounds[1] = (int)x2 + 1;
		bounds[3] = (int)y2 + 1;


		int NX = x2 - x1;
		int NY = y2 - y1;



		///********************************










		//if (nrPolygons>0)
		{
			QImage *visImage = iWidget->GetCurrentImage();
			memset(visImage->bits(), 0, 65536 * 4);
			int nx = mnx;
			int ny = mny;

			QImage img(mnx, mny, QImage::Format_RGB32);
			img.fill(Qt::black);
			QPainter Painter2(&img);
			Painter2.setBrush(Qt::white);
			Painter2.setPen(1);
			//	qDebug () <<"fpos:"+QString::number(shapes->polygonList.at(0).at(0).x())<<" , "<<QString::number(shapes->polygonList.at(0).at(0).y());
			//for(int ix=0;ix<nrPolygons;ix++)
			int ix = 0;
			{

				//MyShape s;
				//s.poly=slist->at(ix);
				//Painter2.drawPolygon(s.poly);
				////wez shape i policz srednia x i y;
				//{
				//	MyShape shape;
				//	shape.polyColor.setRgb(255,0,0);
				//	shape.representation=2;
				//	shape.poly=s.poly;
				//	shapes->shapeList.append(shape);
				//}


				unsigned short *tabx;
				int count1, count2;
				QString text = "";

				double t[15];

				int i, j, k;
				for (i = 0; i < 15; i++)
					t[i] = 0;
				t[10] = 1;
				t[11] = 1;
				t[12] = 1;

				int posTab[4] = { 0, 1, 2, 6 };
				//	t1[1] = t0[1];//tx
				//t1[2] = t0[2];//ty
				//t1[6] = t0[3] * rad;//rot
				PrepareTrans(tr, t, 3, posTab);
				if (showMutualStats == false)
				{
					////if ((nrOfRegion1==ix)&&(updateRegion1==true))
					{

						tabx = prevImg;
						//unsigned short leftThreshold=params[leftThresholdID]*prevMax;
						//	unsigned short rightThreshold=params[rightThresholdID]*prevMax;
						/*for(int ia=0;ia<sliceSize;ia++)
						{
						if ((tabx[ia]>=leftThreshold)&&(tabx[ia]<=rightThreshold))	{		buffer[ia]=val1; 	}	else	{		buffer[ia]=val2; 	}		;
						}*/

						stats1Calculated = UpdateStats(scatterSize, hist1, stats1, bounds, imgMaskBits, modelData, buffer, intBuffers, count1, GetMaxValue(modelData->GetDataArray(), sliceSize));
						{

							double area1 = count1*mdx*mdy;
							text += "Region 1:\nPixelCount: " + QString::number(count1);
							text += "\nArea: " + QString::number(area1) + " mm2";


						}

					}
					//	if ((nrOfRegion2==ix)&&(updateRegion2==true))

					{
						tabx = tab;
						//unsigned short leftThreshold=params[leftThresholdID]*currentMax;
						//	unsigned short rightThreshold=params[rightThresholdID]*currentMax;
						//
						/*for(int ia=0;ia<sliceSize;ia++)
						{
						if ((tabx[ia]>=leftThreshold)&&(tabx[ia]<=rightThreshold))	{		buffer[ia]=val1; 	}	else	{		buffer[ia]=val2; 	}		;
						}*/
						//bede przebiegał po wszystkich punktach modelu i sprawdzał czy należą do obiektu!

						stats2Calculated = UpdateStats(scatterSize, hist2, stats2, bounds, imgMaskBits, modelData, buffer, intBuffers, count2, GetMaxValue(objectData->GetDataArray(), sliceSize), true, t, false, objectData);
						{


							double area1 = count2*mdx*mdy;
							text += "\nRegion 2:\nPixelCount: " + QString::number(count2);
							text += "\nArea: " + QString::number(area1) + " mm2";



						}
					}

					//MyShape shape;						shape.shapeText=QString(text);	
					//shape.textColor.setRgb(255,33,0);	
					//QRectF rect=s.poly.boundingRect();
					//shape.objectBounds=rect;
					////shape.textAutoPos=showInfoPlace;


					//shapes->shapeList.append(shape);
					////qreal x1,x2,y1,y2;
					////rect.getCoords(&x1,&y1,&x2,&y2);
					//{
					//	QString text=QString::number(ix);
					//	MyShape shape;						shape.shapeText=QString(text);
					//	//shape.textPos.setX((x1+x2)/2);		shape.textPos.setY((y1+y2)/2);				
					//	shape.textColor.setRgb(255,0,0);	
					//	shape.objectBounds=rect;
					//	shape.textAutoPos=0;

					//	shapes->shapeList.append(shape);
					//}





					//narysowane
					unsigned char *tab11 = img.bits();
					unsigned int *tab1 = (unsigned int *)tab11;

					bool test;



					QColor backCol(Qt::black);
					QColor col1(Qt::blue);
					QColor col2(Qt::green);
					iWidget->GetShapes()->shapeList.clear();





					if ((stats1Calculated == true) && (hideGUI == false))
					{
						//malujemy histogram - na gorze i opis

						DrawHistogram(scatterSize, hist1, 0, 10, 255, 120, useHistLogScale, backCol, col1, visImage);


						QString text =
							"min: " + QString::number(stats1[0]) + "   " +
							"max: " + QString::number(stats1[1]) + "\n" +
							"avg: " + QString::number(stats1[2]) + "   " +
							"stddev: " + QString::number(stats1[3]) + "\n" +
							"skwewness: " + QString::number(stats1[4]) + "   " +
							"kurtosis: " + QString::number(stats1[5]);

						MyShape shape;						shape.shapeText = QString(text);
						shape.textPos.setX(5);				shape.textPos.setY(10);
						shape.textColor.setRgb(255, 0, 0);	iWidget->GetShapes()->shapeList.append(shape);






					}

					if ((stats2Calculated == true) && (hideGUI == false))
					{
						DrawHistogram(scatterSize, hist2, 0, 138, 255, 120, useHistLogScale, backCol, col2, visImage);

						QString text =
							"min: " + QString::number(stats2[0]) + "   " +
							"max: " + QString::number(stats2[1]) + "\n" +
							"avg: " + QString::number(stats2[2]) + "   " +
							"stddev: " + QString::number(stats2[3]) + "\n" +
							"skwewness: " + QString::number(stats2[4]) + "   " +
							"kurtosis: " + QString::number(stats2[5]);
						MyShape shape;						shape.shapeText = QString(text);
						shape.textPos.setX(5);				shape.textPos.setY(10 + 128);
						shape.textColor.setRgb(255, 0, 0);	iWidget->GetShapes()->shapeList.append(shape);


					}
				}
				else
				{


					iWidget->GetShapes()->shapeList.clear();
					QColor backCol(Qt::black);
					QColor col1(Qt::blue);
					QColor col2(Qt::green);


					int count = 0;







					bool test = Update2DStats2(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, modelData, objectData, intBuffers, count, usData0, true, t, false);


					/*	bool Update2DStats(int bins, int *hist1, int *hist2, int *scatter, double *statsScatter, int *bounds, unsigned int*maskImgBits,
					RawDataSet *modelData, RawDataSet *objectData, int **tmpTab, int &count, int maxValue1, int  maxValue2,

					bool useTrans, double *trans, bool invTrans)
					*/


					if ((test == true) && (outParams != NULL))
					{
						int funcx = (int)params[functionID] + 1;
						outParams[0] = statsMutual[funcx];

						int x1 = bounds[0];
						int x2 = bounds[1];
						int y1 = bounds[2];
						int y2 = bounds[3];
						int X1 = (int)x1;
						int Y1 = (int)y1;
						int X2 = (int)x2 + 1;
						int Y2 = (int)y2 + 1;
						double c[2];
						c[0] = (X2 + X1)*0.5;
						c[1] = (Y2 + Y1)*0.5;
						double rCenter[3] = { c[0] * modelData->GetXscale() * 0.5, c[1] * modelData->GetYscale() * 0.5, 0 };//krece modelem

						outParams[1] = rCenter[0];
						outParams[2] = rCenter[1];
						outParams[3] = rCenter[2];

					}

					if ((test == true) && (hideGUI == false))
					{
						int size = mutualHistSize;
						int spacing = 5;
						int valx = 255 - 2 * spacing - size;
						DrawJointHistogram(scatterSize, scatter, spacing + size, spacing + size, valx, valx, true, backCol, col1, visImage, scatterMultiply);

						DrawHistogram(scatterSize, hist1, spacing, spacing + size, size, valx, useHistLogScale, backCol, col1, visImage, true);
						DrawHistogram(scatterSize, hist2, spacing + size, spacing, valx, size, useHistLogScale, backCol, col2, visImage, false, false);


						QString text =
							"count: " + QString::number(statsMutual[0]) + "\n" +
							"ssd: " + QString::number(statsMutual[1]) + "\n" +
							"cross: " + QString::number(statsMutual[2]) + "\n" +
							"entropy: " + QString::number(statsMutual[3]) + "\n" +
							"MI: " + QString::number(statsMutual[4]) + "\n" +
							"NMI: " + QString::number(statsMutual[5]);

						MyShape shape;						shape.shapeText = QString(text);
						shape.textPos.setX(5);				shape.textPos.setY(10 + 128);
						shape.textColor.setRgb(255, 0, 0);	iWidget->GetShapes()->shapeList.append(shape);


					}

					//zrobie mutual info
				}

			}






			iWidget->update();




		}
		delete[]buffer;
		//for(int i=0;i<sliceSize;i++)
		//{
		//	////mask0[i]=val;
		//	//test=(((*tab1++)==0xffffffff));//' mask0[i]&=255;
		//	//if (!((test==true)&&(mask[i]==255)))

		//	// mask0[i]=0;
		//	usData0[i]=tab[i];//

		//}


		//--------------------------------------------------------------------------
		modelData->SetDataArray(NULL, false); delete modelData;
		objectData->SetDataArray(NULL, false); delete objectData;
	}
	else
	{
		for (int i = 0; i<sliceSize; i++)
		{
			usData0[i] = tab[i];
		}
	}


	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);







	return true;
}
//----------------------------------
LayerGradientTestFilter::LayerGradientTestFilter()
{
	filterName = "LayerGradientTestFilter";




	//
	//AddParam("centerValue", 32767,true,0,65535,1,centerValID);
	QStringList list;
	//list.clear(); list <<"false" <<"true" ;
	//AddParam("invert", 0,true,0,1,1,invertID,&list);
	list.clear(); list << "vert" << "horiz";
	AddParam("direction", 0, true, 0, 1, 1, directionID, &list);
	list.clear(); list << "false" << "true";
	AddParam("rescale", 1, true, 0, 1, 1, rescaleID, &list);


}
//------------------------------------
int	LayerGradientTestFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;



	unsigned short *buff1 = usData1;
	unsigned short *tmpBuff = tab;


	tmpBuff = tab;




	bool direction = params[directionID];
	bool rescale = params[rescaleID];

	int n[2] = { mnx, mny };
	//	data->GetN(n);

	double scaleX; double scaleY;

	if (rescale == false)
	{
		scaleX = (double)256 / n[0];
		scaleY = (double)256 / n[1];
	}
	else
	{
		scaleX = (double)mainDataMaxValue / n[0];
		scaleY = (double)mainDataMaxValue / n[1];
	}

	int pos = 0;
	for (int j = 0; j<n[1]; j++)
	{
		for (int i = 0; i<n[0]; i++)
		{
			if (direction == 0)
				usData0[pos] = i*scaleX;
			else
				usData0[pos] = j*scaleY;
			pos++;
		}

	}

	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);




	return true;
}
//----------------------------------




LayerMagnifyFilter::LayerMagnifyFilter()
{
	filterName = "LayerMagnifyFilter";




	//
	//AddParam("centerValue", 32767,true,0,65535,1,centerValID);
	QStringList list;
	//list.clear(); list <<"false" <<"true" ;
	//AddParam("invert", 0,true,0,1,1,invertID,&list);
	//list.clear(); list << "vert" << "horiz";
	AddParam("scale", 1, true, 1, 10, 0.0001, scaleID);
	list.clear(); list << "nearest" << "bilinear";

	AddParam("inteprolation", 1, true, 0, 1, 1, interpolID, &list);

	list.clear(); list << "false" << "true";
	AddParam("center Clicked", 1, true, 0, 1, 1, useCenterID, &list);


	AddParam("PosX", 500, true, 0, 1000, 1, posXID);
	AddParam("PosY", 500, true, 0, 1000, 1, posYID);

	for (int i = 0; i < 15; i++)
		t[i] = 0;
	t[10] = 1;
	t[11] = 1;
	t[12] = 1;

}
//------------------------------------
int	LayerMagnifyFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	//if ((sliceNr<0) || (sliceNr >= data->GetNz())) return 0;
	//unsigned short *tab = (dataTab == NULL) ? data->GetDataArray() + (sliceNr*sliceSize) : dataTab;



	unsigned short *buff1 = usData1;
	unsigned short *tmpBuff = tab;


	tmpBuff = tab;


	double sc = params[scaleID];
	int interpolType = params[interpolID];

	bool usePos = params[useCenterID];

	/*bool direction = params[directionID];

	/*bool direction = params[directionID];
	bool rescale = params[rescaleID];

	int n[3];
	data->GetN(n);

	double scaleX; double scaleY;

	if (rescale == false)
	{
	scaleX = (double)256 / n[0];
	scaleY = (double)256 / n[1];
	}
	else
	{
	scaleX = (double)mainDataMaxValue / n[0];
	scaleY = (double)mainDataMaxValue / n[1];
	}

	int pos = 0;
	for (int j = 0; j<n[1]; j++)
	{
	for (int i = 0; i<n[0]; i++)
	{
	if (direction == 0)
	usData0[pos] = i*scaleX;
	else
	usData0[pos] = j*scaleY;
	pos++;
	}

	}


	*/


	double center[2] = { mnx / 2.0, mny / 2.0 };
	if (shapes != NULL)
	{
		center[0] = shapes->clickedLeftPoint.rx();
		center[1] = shapes->clickedLeftPoint.ry();


	}

	if (usePos == false)
	{
		int posX = params[posXID];
		int posY = params[posYID];

		center[0] = (posX*mnx) / 1000.0;
		center[1] = (posY*mny) / 1000.0;
	}


	shapes->scale = sc;
	shapes->xScalePos = center[0];
	shapes->yScalePos = center[1];

	int posTab[6] = { 0, 1, 2, 10,11 };
	double t0[5];

	double scale[2];
	scale[0] = scale[1] = sc;

	t0[1] = 0;// center[0] * (1.0 - scale[0]); //tx
	t0[2] = 0;// center[1] * (1.0 - scale[1]); //ty
	t0[3] = scale[0];//scx
	t0[4] = scale[1];//scy
	rCenter[0] = center[0];
	rCenter[1] = center[1];
	rCenter[2] = 0;
	//	qDebug() << "X: " << rCenter[0] << " Y: " << rCenter[1];
	PrepareTrans2(t0, t, 4, posTab);
	Transform *trans = new Transform(rCenter, 1, 1, 1);

	trans->MakeCompleteAffineMatrixInv(matrix, t);
	InterpolationFunction *intrp = NULL;

	switch (interpolType)
	{
	case 0:
		intrp = new InterpolationFunction2DNearest();
		break;

	case 1:
		intrp = new InterpolationFunction2DBilinear();
		break;

	default:
		intrp = new InterpolationFunction2DBilinear();
	}

	//int n[3];
	//data->GetN(n);
	int n[2] = { mnx, mny };
	double t1 = n[0] - 1;
	double t2 = n[1] - 1;


	unsigned short val;
	int ss = n[0] * n[1];
	int pos = 0;

#pragma omp parallel for  ordered schedule (guided) 
	for (int j = 0; j < n[1]; j++)
	{
		//qDebug() << "watki " << omp_get_thread_num();
		TransformLine(j, tab, usData0, trans, intrp, n);
		////Point[0] = 0;


		//for (int i = 0; i < n[0]; i++)
		//{
		//	double Point[3] = { 0 };
		//	double Point1[3] = { 0 };
		//	Point[0] = i;//przejscie do ukladu wspolrzednych globalnych 
		//	Point[1] = j;

		//	trans->MultiplyPoint(Point1, Point);
		//	//Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu
		//	val = 0;
		//	if ((Point1[0] >= 0) && (Point1[0] < t1) &&
		//		(Point1[1] >= 0) && (Point1[1] < t2))
		//	{
		//		//val= src->GetVoxelValue((int)(Point1[0]),(int)(Point1[1]),sliceNr);

		//		usData0[pos] = intrp->GetInterpolatedValue(tab, n[0], ss, Point1[0], Point1[1], 0);


		//	}
		//	else
		//	{
		//		usData0[pos] = 0;
		//	}
		//	pos++;

		//	//Point[0]+=modelScale[0];
		//}
	}
	delete intrp;
	delete trans;

	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);




	return true;
}
//----------------------------------

void LayerMagnifyFilter::TransformLine(int j, unsigned short *tab, unsigned short *dest, Transform *trans, InterpolationFunction *intrp, int *n)
{
	double t1 = n[0] - 1;
	double t2 = n[1] - 1;


	unsigned short val;
	int ss = n[0] * n[1];
	int pos = n[0] * j;

	for (int i = 0; i < n[0]; i++)
	{
		double Point[3] = { 0 };
		double Point1[3] = { 0 };
		Point[0] = i;//przejscie do ukladu wspolrzednych globalnych 
		Point[1] = j;

		trans->MultiplyPoint(Point1, Point);
		//Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu
		//val = 0;
		if ((Point1[0] >= 0) && (Point1[0] < t1) &&
			(Point1[1] >= 0) && (Point1[1] < t2))
		{
			//val= src->GetVoxelValue((int)(Point1[0]),(int)(Point1[1]),sliceNr);

			dest[pos] = intrp->GetInterpolatedValue(tab, n[0], ss, Point1[0], Point1[1], 0);


		}
		else
		{
			dest[pos] = 0;
		}
		pos++;

	}
}
//-------------------------------------
LayerGrowingRegionFilter::LayerGrowingRegionFilter()
{
	frontTabIn = NULL;
	frontTabOut = NULL;
	frontTabOut2 = NULL;
	frontTabOld = NULL;
	mask0 = NULL;
	tempMaskTab = NULL;
	frontSizeMax = 0;




	for (int i = 0; i < 10; i++) exportParams[i] = 0;
	equalRadius = false;
	radius[0] = 1; radius[1] = 1; radius[2] = 1;
	leftTh = 0;
	rightTh = 0;
	revth = false;
	samplingTab = NULL;
	samplesCount = 0;
	perc = 0;
	maxDist = 0;

	sigma = 1.5;
	expMax = 4096;
	/*samplingTab1 = new unsigned short[expMax];
	memset(samplingTab1, 0, expMax*sizeof(unsigned short));
	intensityFactor = 1.0 / (double)(expMax*expMax);
	expTab = new double[2 * expMax + 2];
	GetExpTab(expTab, expMax, sigma, intensityFactor);*/

	expMax = 0;
	intensityFactor = 0;
	expTab = NULL;// new double[2 * expMax + 2];
	samplingTab1 = NULL;
	samplingTabIndex = NULL;

	//intensityFactor2=intensityFactor*intensityFactor;




	filterName = "LayerGrowingRegionFilter";
	QStringList list;
	


	list.clear(); list << "normal" << "global" <<"islands";
	AddParam("segmentMode", 0, true, 0, 2, 1, segmentModeID, &list);
	list.clear(); list << "false" << "true";

	AddParam("fillGaps", 0, true, 0, 1, 1, fillGapsID, &list);
	AddParam("multiFront", 0, true, 0, 1, 1, mutliFrontID, &list);
	AddParam("multiSeed", 0, true, 0, 1, 1, multiSeedID, &list);
	AddParam("findEndPoints", 0, true, 0, 1, 1, findEndPointID, &list);


	AddParam("don't Calculate2D", 1, true, 0, 1, 1, dontCalculate2DID, &list);

	AddParam("multiply", 1000, true, 1, 32768, 1, multiplyID);
	AddParam("mcValue", 0.01, true, 0.01, 255, 0.01, mcValueID);

	AddParam("GrowFromPoint", 1, true, 0, 1, 1, GrowFromPointID, &list);
	
	
	list.clear(); list << "s/s" << "s+p/p" << "s+p/s+p"<<"s/p";
	AddParam("GrowFromClickedOnly", 0, true, 0, 3, 1, useOnlyClickedPointForGrowingID, &list);
	
	list.clear(); list << "none" << "ask" << "all" << "selected";
	AddParam("useMasksForAneurysmInit", 0, true, 0, 3, 1, useMasksForAneurysmInitID, &list);

	
	
	list.clear(); list << "false" << "true";



	

	AddParam("showDebug", 0, true, 0, 1, 1, showDebugID, &list);
	list.clear(); list << "threshold" << "probability";
	AddParam("Mode", 0, true, 0, 1, 1, probOrThreshID, &list);
	list.clear(); list << "false" << "true";

	AddParam("lth", 0, true, 0, 10000, 1, threshold1ID);
	AddParam("rth", 10000, true, 0, 10000, 1, threshold2ID);
	AddParam("autoThreshold", 1, true, 0, 1, 1, autoThresholdID, &list);
	AddParam("useGlobalThresholds", 0, true, 0, 1, 1, useGlobalThresholdsID, &list);



	AddParam("thresholdAddLeft", 0, true, 0, 1000, 1, thresholdAdd1ID);
	AddParam("thresholdAddRight", 0, true, 0, 1000, 1, thresholdAdd2ID);

	AddParam("minLeafLength", 6, true, 0.00, 100, 0.01, minLeafLengthID);
	

	AddParam("blockSmallFronts", 0, true, 0, 1, 1, blockSmallFrontsID, &list);
	AddParam("minSizeToBlock", 0, true, 0, 200, 1, minSizeToBlockID);
	AddParam("maxSizeToBlock", 0, true, 0, 1000, 1, maxSizeToBlockID);

	AddParam("minEndPointFrontSize", 0, true, 0, 200, 1, minEndPointFrontSizeID);
	

	AddParam("nrOfChecks", 3, true, 0, 10, 1, nrOfRepetitionsBeforeBlockID);


	list.clear(); list << "2D Source" << "2D Result" << "3D Source";
	AddParam("sampling3DMode", 0, true, 0, 2, 1, sampling3DModeID, &list);



	list.clear(); list << "false" << "true";

	//AddParam("showMask", 0, true, 0, 1, 1, showMaskID, &list);
	//AddParam("segOpacity", 0.5, true, 0.0, 1, 0.001, segOpacityID);

	AddParam("angleTestDist", 0, true, 0, 50, 1, angleTestDistID);
	AddParam("angleTestAngle", 120, true, 0, 180, 0.1, angleTestAngleID);

	AddParam("kernelSkelSearchSize", 3, true, 2, 10, 1, kernelSkelSearchSizeID);
	AddParam("cutSkelFromBegining", 0, true, 0, 20,1, cutSkelFromBeginingID);
	AddParam("cutSkelThresh", 0, true, 0, 50, 1, cutSkelThreshID);


	
	
	AddParam("connectCloseEndPoints", 0, true, 0, 1, 1, connectCloseEndPointsID, &list);
	AddParam("connectSkelAndJoints", 0, true, 0, 1, 1, connectNewSkeletonlWithJointsID, &list);
	AddParam("generateEndPointsBalls", 0, true, 0, 1, 1, generateEndPointsBallsID, &list);
	AddParam("generateTracingSkeletons", 0, true, 0, 1, 1, generateTracingSkeletonsID, &list);

	AddParam("dontRemoveDistMap", 0, true, 0, 1, 1, dontRemoveDistMapID, &list);


	AddParam("showAdvGlyph", 0, true, 0, 1, 1, advGlyphID, &list);
	list.clear(); list << "front" << "advFront" << "remap";
	AddParam("drawMode", 0, true, 0, 2, 1, drawModeID, &list);
	list.clear(); list << "false" << "true";
	
	
	AddParam("showBreakGlyph", 0, true, 0, 1, 1, breakGlyphID, &list);
	
	AddParam("showStats", 0, true, 0, 1, 1, calculateStatsID, &list);


	AddParam("SphereRadiusX", 0, true, 0, 1, 1, sphereRadiusXID, &list);
	AddParam("radiusX", 1, true, 0.1, 500, 0.01, radiusXID);
	AddParam("radiusY", 1, true, 0.1, 500, 0.01, radiusYID);
	AddParam("radiusZ", 1, true, 0.1, 500, 0.01, radiusZID);


	AddParam("maxVolume", 20, true, 0, 10000, 0.01, maxVolumeID);
	AddParam("maxDist", 10, true, 2, 10000, 1, maxDistID);

	AddParam("EuclidMaxDist", 0, true, 0, 10000, 0.1, euclideanDistanceID);

	/*AddParam("minStdDev", 0, true, 0, 32768, 0.01, minStdDevID);
	AddParam("maxStdDev", 0, true, 0, 32768, 0.01, maxStdDevID);*/
	
	
	AddParam("histSmplSize", 1, true, 1, 3, 1, histSamplingSizeID);
	AddParam("deltaStdDev", 0, true, 0, 100, 0.001, deltaStdDevID);
	
	list.clear(); list << "normal" << "existing" << "tmp";
	AddParam("histSimilarityMode", 0, true, 0, 2, 1, histSimilarityModeID, &list);
	list.clear(); list << "false" << "true";

	AddParam("histSimilarityThreshold", 0, true, 0.00, 10, 0.001, histSimilarityThresholdID);
	AddParam("hist2DSimilarityThreshold", 0, true, 0.00, 10, 0.001, histSimilarityThreshold2DID);

	list.clear(); list << "16" << "32" << "64"<<"128"<<"256";
	AddParam("nrOfHistBins", 5, true, 4, 8, 1, nrOfHistBinsID, &list);
	list.clear(); list << "false" << "true";
	
	//int minStdDevID, int maxStdDevID;

	
	AddParam("minVolForPoint", 10, true, 2, 4096, 1, minVolumeToGenPointID);
	


	list.clear(); list << "square" << "circle";
	AddParam("morphKernelType", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	AddParam("morphKernelSize", 1, true, 0, 10, 1, morphKernelSizeID);
	list.clear(); list << "false" << "true";
	AddParam("morph2D", 0, true, 0, 1, 1, morph2DID);
	

	AddParam("Erode/Dilate", 1, true, -5, 5, 1, dilateSizeID);
	AddParam("Opening/Closing", 0, true, -5, 5, 1, closingSizeID);
	//	AddParam("closingSize3D", 2, true, 0, 4, 1, closingSizeID);

	AddParam("showFrontNr", 1, true, 1, 255, 1, showFrontID);




	
	AddParam("useBigMask", 1, true, 0, 1, 1, useBigMaskID, &list);

	list.clear(); list << "clear" << "add";
	AddParam("bigMaskMode", 0, true, 0, 1, 1, bigMaskModeID, &list);

	AddParam("propPercentCoarse", 0.8, true, 0.01, 0.9, 0.1, probabilityDensityPercentageCoarseID);
	AddParam("propPercentFine", 0.0, true, 0.0, 0.09999, 0.0001, probabilityDensityPercentageFineID);
	AddParam("sigma", 1.5, true, 0.01, 6, 0.01, sigmaID);
	AddParam("intensityFactor", 0.5, true, 0.001, 1, 0.001, intensityFactorID);



	list.clear(); list << "dilate" << "erode";


	int morphologyTypeID;
	int morphologyKernelTypeID;


	list.clear(); list << "false" << "true";
	AddParam("drawFonts", 1, true, 0, 1, 1, drawFontsID, &list);


	list.clear();



	AddParam("endPointRadius", 0.01, true, 0.001, 5, 0.0011, endPointRadiusID);
	AddParam("infoType", 0, true, 0, 7, 1, infoTypeID, &list);

	AddParam("r1", 255, true, 0, 255, 1, r1ID);
	AddParam("g1", 255, true, 0, 255, 1, g1ID);
	AddParam("b1", 255, true, 0, 255, 1, b1ID);


	AddParam("r2", 255, true, 0, 255, 1, r2ID);
	AddParam("g2", 255, true, 0, 255, 1, g2ID);
	AddParam("b2", 255, true, 0, 255, 1, b2ID);

	AddParam("fontSize", 15, true, 4, 100, 1, fontSizeID);
	list.clear(); list << "center" << "left" << "right" << "top" << "bottom";
	AddParam("showInfoPlace", 1, true, 0, 4, 1, showInfoPlaceID, &list);
	AddParam("drawHistogram", 0, true, 0, 1, 1, drawHistogramID, &list);

}
//----------------------------------------------
LayerGrowingRegionFilter::~LayerGrowingRegionFilter()
{
	if (expTab != NULL) delete[]expTab;
	if (samplingTab1 != NULL)	delete[]samplingTab1;
	if (samplingTabIndex != NULL)	delete[]samplingTabIndex;

	if (samplingTab != NULL)	delete[]samplingTab;


	if (frontTabIn != NULL) delete[]frontTabIn;
	if (frontTabOut != NULL) delete[]frontTabOut;
	if (frontTabOut2 != NULL) delete[]frontTabOut2;
	if (frontTabOld != NULL) delete[]frontTabOld;
	if (mask0 != NULL)delete[]mask0;
	if (tempMaskTab != NULL) delete[]tempMaskTab;



}
//------------------------------------
int	LayerGrowingRegionFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)

{
	minEndPointFrontSize = params[minEndPointFrontSizeID];
	connectCloseEndPoints = params[connectCloseEndPointsID];
	int kernelSize = params[morphKernelSizeID];
	morphKernelSize = kernelSize;
	useMasksForAneurysmInit=params[useMasksForAneurysmInitID];

	if (params[segmentModeID] >0)
	{
		params[mutliFrontID] = false;// po to zeby byl tylko semgnet w 1 kolorze

	}
	morph2D = params[morph2DID];
	histSimilarityMode = params[histSimilarityModeID];

	histSamplingSize = params[histSamplingSizeID];
	histSimilarityThreshold = params[histSimilarityThresholdID];
	histSimilarityThreshold2D = params[histSimilarityThreshold2DID];
	//minStdDev = params[minStdDevID];
	//maxStdDev= params[maxStdDevID];
	deltaStdDev = params[deltaStdDevID];

	segmentMode = params[segmentModeID];

	nrOfHistBins = params[nrOfHistBinsID];
	nrOfHistBins = pow(2, nrOfHistBins);

	if (params[sphereRadiusXID] == true)
	{
		params[radiusYID] = params[radiusXID];
		params[radiusZID] = params[radiusXID];
	}
	
	
	for (int i = 0; i<sliceSize; i++)
	{
		usData0[i] = tab[i];
	}
	
	fillGaps = params[fillGapsID];
	minLeafLength = params[minLeafLengthID];

	connectNewSkeletonlWithJoints = params[connectNewSkeletonlWithJointsID];
	generateEndPointsBalls=params[generateEndPointsBallsID];
	generateTracingSkeletons=params[generateTracingSkeletonsID];


	dontRemoveDistMap = params[dontRemoveDistMapID];
	breakGlyph = params[breakGlyphID];
	advGlyph = params[advGlyphID];
	drawMode = params[drawModeID];
	endPointRadius = params[endPointRadiusID];
	blockSmallFronts = params[blockSmallFrontsID];
	nrOfRepetitionsBeforeBlock = params[nrOfRepetitionsBeforeBlockID];
	minSizeToBlock = params[minSizeToBlockID];
	maxSizeToBlock = params[maxSizeToBlockID];
	multiSeed = params[multiSeedID];
	findEndPoint = params[findEndPointID];
	//return 0;

	maxDist = params[maxDistID];

	euclideanDistance = params[euclideanDistanceID];

	maxVolume = params[maxVolumeID];

	double mSigma = params[sigmaID];
	bool useStartShapes = true;

	QList<QPolygonF> *slist = &shapes->polygonList;

	if (slist == NULL) return 0;

	int nrPolygons = shapes->polygonList.count();

	//if ((sliceNr < 0) || (sliceNr >= data->GetNz()) || (shapes == NULL)) return 0;
	//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	//unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	//unsigned short *mask0 = new unsigned short[sliceSize];

	unsigned char *masktmp;
	//	int  j, k;
	revth = false;
	unsigned char val1, val2;
	if (revth == false)
	{
		val1 = 2; val2 = 0;
	}
	else
	{
		val1 = 0; val2 = 2;
	}

	int c1 = 0;
	int c2 = 0;

	//progowanie
	leftTh = params[threshold1ID];// *mainDataMaxValue * 0.0001;;
	rightTh = params[threshold2ID];// * mainDataMaxValue *0.0001;

	shapes->cleanPolygonFlag = true;


	mode = params[probOrThreshID];//prob 0 thresh 1


	unsigned short maxValue = mainDataMaxValue;

	//int mmmm = data->GetMaxValue();
	RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
	data->SetDataArray(tab, false);
	int n[3];
	int sliceSize = data->GetSliceSize();
	data->GetN(n);


	double center[2] = { data->GetNx() / 2.0, data->GetNy() / 2.0 };
	if (shapes != NULL)
	{
		center[0] = shapes->clickedLeftPoint.rx();
		center[1] = shapes->clickedLeftPoint.ry();
	}


	MyShape shape2;
	double centerE[2] = { center[0], center[1] };


	double rr1a = 1;
	double rr2a = 1;

	switch (viewType)
	{
	case 0:
		rr1a = params[radiusXID];
		rr2a = params[radiusYID];
		break;
	case 1:
		rr1a = params[radiusXID];
		rr2a = params[radiusZID];
		break;
	case 2:
		rr1a = params[radiusYID];
		rr2a = params[radiusZID];
		break;

	}

	radius[0] = params[radiusXID];
	radius[1] = params[radiusYID];
	radius[2] = params[radiusZID];


	double r1 = rr1a / mdx;
	double r2 = rr2a / mdy;
	//double angle = params[angleID];
	double angle = 0;
	shape2.ellipses.append(QPointF(centerE[0], centerE[1]));//center
	shape2.ellipses.append(QPointF(r1, r2));//rads
	shape2.ellipses.append(QPointF(-angle, 50));

	//	int col1[] = { params[r1ID], params[g1ID], params[b1ID] };

	shape2.ellipsesColor.setRgb(params[r2ID], params[g2ID], params[b2ID]);

	shapes->shapeList.append(shape2);


	samplesCount = 0;
	sampling3DMode = params[sampling3DModeID];



	double alpha = angle* to_rad;			double ca = cos(alpha);		double sa = sin(alpha);


	bool sigmaChg = fabs(mSigma - sigma) < 1e-5 ? false : true;

	bool intensChg = fabs(intensityFactor - params[intensityFactorID])< 1e-5 ? false : true;
	intensityFactor = params[intensityFactorID];

	//qDebug() << "przed kaswowaniem";
	if ((expMax != maxValue) || (sigmaChg == true) || (intensChg == true))
	{

		sigma = mSigma;
	//	qDebug() << "1";
		if (expTab != NULL)delete[]expTab;
		expMax = maxValue;
		expTab = new double[expMax * 2 + 1];
	//	qDebug() << "2";
		if (samplingTab1 != NULL)delete[]samplingTab1;
		samplingTab1 = new unsigned short[expMax+1];
		//qDebug() << "3";
		if (samplingTabIndex != NULL)delete[]samplingTabIndex;
		samplingTabIndex = new unsigned short[expMax+1];

		//qDebug() << "4";
		if (samplingTab != NULL)delete[]samplingTab;
		samplingTab = new unsigned short[expMax+1];
		//qDebug() << "5";
		GetExpTab(expTab, expMax, sigma, intensityFactor);
		//qDebug() << "6";
	}

	//qDebug() << "po kaswowaniem";
	memset(samplingTab, 0, (expMax+1) * sizeof(unsigned short));
	memset(samplingTab1, 0, (expMax+1) * sizeof(unsigned short));

	growFromPoint = params[GrowFromPointID];
	useOnlyClickedPointForGrowing = params[useOnlyClickedPointForGrowingID];

	useBigMask = params[useBigMaskID];
	bigMaskMode = params[bigMaskModeID];


	int valx;
	int minValx = 65535; int maxValx = 0;

	//	RawDataSet *newData = new RawDataSet(data->GetNx(), data->GetNy(), 1);
	//newData->ZeroQ();
	int posxx = 0;
	if (mSize != sliceSize)
	{
		if (mask0 != NULL)delete[]mask0;
		if (tempMaskTab != NULL) delete[]tempMaskTab;
		tempMaskTab = new unsigned char[sliceSize];
		mask0 = new unsigned short[sliceSize];
		mSize = sliceSize;
	}
	memset(mask0, 0, sliceSize * sizeof(unsigned short));

	int minI[2] = { n[0], n[1] };
	int maxI[2] = { 0, 0 };
	memset(usData1, 0, sliceSize * sizeof(unsigned short));
	int advDist = 1;
	for (int j = 0; j < data->GetNy(); j++)
	{
		for (int i = 0; i < data->GetNx(); i++)
		{
			double x, y;
			x = i - center[0];
			y = j - center[1];

			double xx = x*ca - y*sa;				double yy = x*sa + y*ca;

			double xr1 = xx / r1;
			double yr2 = yy / r2;
			double ellipse = xr1*xr1 + yr2*yr2;
			if (ellipse < 1)
			{
				bool check = true;
				if ((useBigMask == true) && (mainMask != NULL))
				{
					if (mainMask[posxx] == 0)
					{
						check = false;
					}
				}

				if (check == true)
				{

					//int pos = i + j*n[0];
					valx = (int)(tab[posxx]);
					samplingTab[valx]++;
					samplesCount++;
					if (growFromPoint == false)
					{
						mask0[posxx] = 1;
						usData1[posxx] = advDist;
					}

					if (i<minI[0]) minI[0] = i;
					if (i>maxI[0]) maxI[0] = i;
					if (j<minI[1]) minI[1] = j;
					if (j>maxI[1]) maxI[1] = j;

				}

				//	newData->GetDataArray()[posxx] = valx; 

				if (minValx > valx)minValx = valx;
				if (maxValx<valx) maxValx = valx;

			}
			posxx++;
		}
		//val=

	}


	double percCoarse = params[probabilityDensityPercentageCoarseID]; //0-100%
	double percFine = params[probabilityDensityPercentageFineID]; //0-100%

	perc = percCoarse + percFine;



	QPolygonF testPolyMask;

	QElapsedTimer tm;
	tm.start();
	unsigned int t1 = tm.elapsed();


	//bool showMask = params[showMaskID];



	//	double dest = params[destID];


	alpha = -angle* to_rad;
	ca = cos(alpha);
	sa = sin(alpha);

	//double step = 360 / dest;
	double t;
	double st, ct;
	double x, y;
	double dx, dy, len;
	int x1, y1;
	int val;
	bool test1;
	QPolygonF testPoly;

	int neighSize = 2;
	int neighType = 0;//0-cube 1-sphere
	double neighPercentage = 0.3;
	bool calc = params[dontCalculate2DID];
	exportParams[0] = params[mcValueID];


	exportParams[1] = params[kernelSkelSearchSizeID];
	exportParams[2] = params[cutSkelFromBeginingID];
	exportParams[3] = params[cutSkelThreshID];
	exportParams[4] = params[angleTestDistID];
	exportParams[5] = params[angleTestAngleID];

	showDebug = params[showDebugID];
	multiFront = params[mutliFrontID];
	bool calculateStats = params[calculateStatsID];
	int kernelType = params[morphologyKernelTypeID]; exportParams[6] = params[morphologyKernelTypeID];
	int col1[] = { params[r1ID], params[g1ID], params[b1ID] };
	dilateSize = params[dilateSizeID];
	closingSize = params[closingSizeID];
	

	bool autoThresh = params[autoThresholdID];
	int threshold1Add = params[thresholdAdd1ID];
	int threshold2Add = params[thresholdAdd2ID];
	if (autoThresh)
	{
		leftTh = minValx;
		rightTh = maxValx;

		if (threshold1Add > 0) { leftTh -= threshold1Add; if (leftTh < 0) leftTh = 0; }
		if (threshold2Add > 0) { rightTh += threshold2Add; if (rightTh > maxValue) rightTh = maxValue; }


	}
	useGlobalThresholds = params[useGlobalThresholdsID];
	if ((shapes != NULL) && (useGlobalThresholds == true))
	{
		leftTh = shapes->thresholds[0];
		rightTh = shapes->thresholds[1];
	}

	if (samplesCount == 0) return false;

	//pozbieram indexy wystepujace do krotszej tabeli!!!!

	int maxPosNew = 0;
	for (int i = 0; i < expMax; i++)
	{
		if (samplingTab[i]>0)
		{
			samplingTab1[maxPosNew] = samplingTab[i];
			samplingTabIndex[maxPosNew] = i;
			maxPosNew++;

			//teraz wystarczy biec po index tab do maxpos new zeby dobrac sie do wszystkich
		}

	}




	double *intensityProbabilityTab1 = new double[expMax];
	double min1, max1;

	GetIntensityProbabilityTab2(intensityProbabilityTab1, expMax, min1, max1, sigma, expTab, samplingTab1, samplingTabIndex, maxPosNew, samplesCount);//ta	sie	przyda
	double factor1 = (max1 != 0) ? 1.0 / (max1) : 1;																																		  //double factor1 = (max1 - min1 != 0) ? 1.0 / (max1 - min1) : 1;



	//int arrNx = 1024;
	//int arrNy = 768;
	//QImage arrowImg(arrNx, arrNy, QImage::Format_RGB32);

	//arrowImg.fill(Qt::white);
	//QPainter Painter2x(&arrowImg);
	//Painter2x.setBrush(Qt::black);
	//Painter2x.setPen(1);
	////int maxValue = data->GetMaxValue();
	//unsigned short *remapTab = new unsigned short[maxValue];
	//double ff = (double)maxValue / max1;
	//for (int i = 0; i < maxValue; i++)
	//{
	//	remapTab[i] = (intensityProbabilityTab1[i])*ff;
	//}

	//double scx = (maxValue>0) ? (double)arrNx / maxValue : 1;
	//for (int i = 0; i < maxValue; i++)
	//{
	//	Painter2x.drawLine(QPoint(i*scx, 0), QPoint(remapTab[i] * scx, arrNy));
	//}
	//delete[]remapTab;
	//arrowImg.save("d:\\mapping-lines.png");


	if (params[drawHistogramID] == true)
	{
		//if (viewType == params[viewID])
		{



			QColor backCol(Qt::black);
			QColor col1(Qt::yellow);

			bool useHistLogScale = true;
			QImage *visImage = iWidget->GetCurrentImage();

			int *hist1 = new int[256];
			memset(hist1, 0, 256 * sizeof(int));
			double scale = 1024 * factor1;

			double xScaler = 255.0 / expMax;

			/*for (int i = 0; i < expMax; i++)
			{
				int pos = i*xScaler;
				int val = intensityProbabilityTab1[i] * scale;
				if (val > hist1[pos]) hist1[pos] = val;
			}*/
			for (int i = 0; i < sliceSize; i++)
			{
				hist1[tab[i]]++;
			}


			DrawHistogram(256, hist1, 0, 0, 255, 255, false, backCol, col1, visImage);
			delete[]hist1;
			iWidget->update();
		}
	}

	if (!calc)
	{

		SegCriterion *cr;

		if (mode == true)
		{
			//prob map

			cr = new SegCriterionProbabilityMap(data, intensityProbabilityTab1, perc, min1, factor1, perc, neighType, neighSize);
		}
		else
		{
			//thresholds
			cr = new SegCriterionThresholds(data, leftTh, rightTh, rightTh, revth, neighPercentage, neighType, neighSize);
		}
		cr->Set2DMode(true);

		cr->SetAvoidValue(true, 65535);





		unsigned char *blockMask = NULL;
		if ((useBigMask == true) && (mainMask != NULL)) blockMask = mainMask;
		bool is2d = true;

		//startuję z obrysu - robię dylatację tego co jest - przetestuję ileśtam dylatacji

		//morph
		int kerenelSize2 = 0;
		//int dilateDist = params[dilateDistID]; exportParams[5] = params[dilateDistID];

		int size2 = 1;

		//int closingSize = params[closingSizeID]; exportParams[7] = params[closingSizeID];




		//int *kernel2 = GetGrowingKernel2D(kerenelSize2, size2, data, kernelType);
		int tmp;
		int dsizemin1n = n[0] * n[1] - 1;



		minI[0] -= 1; if (minI[0] < 0) minI[0] = 0;
		minI[1] -= 1; if (minI[1] < 0) minI[1] = 0;
		maxI[0] += 1; if (maxI[0] >= n[0] - 1) maxI[0] = n[0] - 1;
		maxI[1] += 1; if (maxI[1] >= n[1] - 1) maxI[1] = n[1] - 1; //granice obiektu samplowanego powiekszone o 1 do dylatacji
		int boundsX[6] = { minI[0], maxI[0], minI[1], maxI[1], 0, 0 };

		int nrFront = 2;//tak koloruje pierwszy front

		if (growFromPoint == true)
		{

			//memset(mask0, 0, sliceSize * sizeof(unsigned char));
			int p[2] = { center[0],center[1] };
			mask0[p[0] + p[1] * data->GetNx()] = 1;
		}

		
		SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 1, kernelSize, kernelType, NULL, boundsX, blockMask, cr, is2d, nrFront, 0); //z criterion na danych
																																 //zmienilem mode start z dylacji na erozje bo chce uniknac wstepnego rozdwojenia masek



		if (frontSizeMax < sliceSize)
		{
			if (frontTabIn != NULL) delete[]frontTabIn;
			if (frontTabOut != NULL) delete[]frontTabOut;
			if (frontTabOut2 != NULL) delete[]frontTabOut2;
			if (frontTabOld != NULL) delete[]frontTabOld;
		
			frontTabIn = new int[sliceSize];
			frontTabOut = new int[sliceSize];
			frontTabOut2 = new int[sliceSize];
			frontTabOld = new int[sliceSize];
			
			frontSizeMax = sliceSize;
		}
		//celem jest zrobic dylatacje
		

		int frontTabInLength = 0;
		int frontTabOutLength = 0;
		int frontTabOldLength = 0;

		int *kernelX = NULL;
		int kernelSizeX = 0;
		kernelX = GetGrowingKernel2D(kernelSizeX, kernelSize, data, kernelType);


		//zbieram startowe punkty 

		for (int j = minI[1]; j <= maxI[1]; j++)
		{
			for (int i = minI[0]; i <= maxI[0]; i++)
			{

				int pos = data->GetVoxelPos(i, j, 0);
				if (mask0[pos] == 2)
				{
					frontTabIn[frontTabInLength++] = pos;
				}
			}
		}



		if (sampling3DMode == 0) //kopiuje aktualny stan do tablicy uShortTabMixera - zaznaczenie kolek w srodku
		{

			if (mixer != NULL)
			{
				if (mixer->uShortTabSize != expMax)
				{
					if (mixer->uShortTab != NULL) delete[]mixer->uShortTab;
					mixer->uShortTab = new unsigned short[expMax];

				}
				memcpy(mixer->uShortTab, samplingTab, expMax * sizeof(unsigned short));
				mixer->uShortTabSize = expMax;
			}
		}




		//unsigned char *tempMaskTab = new unsigned char[sliceSize];
		int listSSize = sliceSize;
		memset(tempMaskTab, 0, sliceSize * sizeof(unsigned char));
		int *listS = intBuffers[0];
		int maxIsleNr = nrFront; //bo 1 startowe kolko, 2 pierwsza dylacja
		int frontSizeCountTab[256] = { 0 };
		int frontSizeRepetitionFailureTab[256] = { 0 };
		unsigned char *frontEndTabIn = new unsigned char[256];//tablica po to, zeby wiedziec czy front ktory wystapil w poprzednim jest tez w nowej iteracji //stary (2 - sa oba, 1 jest tylko w starym)
		memset(frontEndTabIn, 0, 256);
		unsigned char *frontEndTabOut = new unsigned char[256];//tablica po to, zeby wiedziec czy front ktory wystapil w poprzednim jest tez w nowej iteracji//nowy  (2 - sa oba, 1 jest tylko w starym)
		memset(frontEndTabOut, 0, 256);
		unsigned char *frontEndTabOutBeforeRemap = new unsigned char[256];//tablica po to, zeby wiedziec czy front ktory wystapil w poprzednim jest tez w nowej iteracji//nowy  (2 - sa oba, 1 jest tylko w starym)
		memset(frontEndTabOut, 0, 256);
		int *frontVolumeCountTab = new int[256];//tablica po to, zeby wiedziec czy front ktory wystapil w poprzednim jest tez w nowej iteracji //stary (2 - sa oba, 1 jest tylko w starym)
		memset(frontVolumeCountTab, 0, 256*sizeof(int));

		int tmpMaskBitNr = 3;

		qDebug() << "max dist: " << maxDist;
		QVector <JointData> joints;
		QVector <JointPoint3D> jPts2;
		QVector<int> distVector;
		QVector <JointPoint3D> xPts;
		int *frontLengthTab = new int[260];
		memset(frontLengthTab, 0, 256 * sizeof(int));;

		unsigned short *volumeTempData2 = usData1;
		unsigned char*Mask2Data = tempMaskTab;
		unsigned short*volumeTempData = mask0;

		int minVParam = params[minVolumeToGenPointID];// tu beidze 

		for (int i = 0; i < maxDist; i++)
		{
			advDist++;
			if (frontTabInLength == 0) break;
			//przebiec po punktach z ostatniego rzutu
		/*	SlotBinarySimpleMorphologyWithCriterion2InOut(data, mask0,
				frontTabIn, frontTabInLength, frontTabOut, frontTabOutLength,
				kernelX, kernelSizeX, blockMask, cr, 0, 0);*/
			if (i == 21)
			{
				i=i;
			}
			SlotBinarySimpleMorphologyWithCriterion2InOut(data, mask0,
				frontTabIn, frontTabInLength, frontTabOut, frontTabOutLength,
				kernelX, kernelSizeX, blockMask, cr, 0, 1, NULL, parentAndAvoid);// , frontOutParallelTabs);
																				 //sprawdzam jakie numery frontów są w starym podziale
			if (findEndPoint == true)
			{
				//zapalam 1 dla tych frontow, ktore wystapily
				int cSize = maxIsleNr + 1; if (cSize > 255) cSize = 255;
				memset(frontEndTabIn, 0, cSize);
				for (int j = 0; j < frontTabInLength; j++)
				{
					int val = volumeTempData[frontTabIn[j]];
					if ((val > 0)&&(val<255)) frontEndTabIn[val] = 1;
				}
			}
			if (frontTabOutLength > sliceSize)
			{
				i = i;
			}

			//swap

			//oznacze flage 0 w tempMaskTab - tylko te co maja 1 beda analizowane
			for (int j = 0; j < frontTabOutLength; j++)
			{
				usData1[frontTabOut[j]] = advDist;
			}

			if (frontTabOutLength == 0)
			{
				break;
			}

			//biorę punkty ktore maja flage 255 - zalweam sie na nie
			//RegionGrowingOnMask2(currPos, mask0, data, 1, 255, kernel, kernelInc, kernelSize, prms, listS, listTmp);

			if (multiFront == true)
			{
				//memset(frontSizeCountTab, 0, 256 * sizeof(int));

				//for (int j = 0; j < frontTabOutLength; j++)
				//{
				//	int val = mask0[frontTabOut[j]];

				//	if (val == 65535)
				//	{
				//		i = i;
				//	}

				//	frontTabOut2[j] = tempMaskTab[frontTabOut[j]];
				//	tempMaskTab[frontTabOut[j]] = 0;
				//	BITSETX(tempMaskTab[frontTabOut[j]], tmpMaskBitNr);//flaga ustawiona
				//}
				int j;
#pragma omp for   ordered schedule (guided) private(j)

				for ( j = 0; j < frontTabOutLength; j++)
				{
					volumeTempData2[frontTabOut[j]] = advDist;
					frontTabOut2[j] = Mask2Data[frontTabOut[j]];
					Mask2Data[frontTabOut[j]] = 0;
					BITSETX(Mask2Data[frontTabOut[j]], tmpMaskBitNr);//flaga ustawiona


					//glyphColTabGood[j] = adVDist % 255;
				}
				//RemapOutFrontTabWithDidivedIslands(frontTabOut, frontTabOutLength, mask0, tempMaskTab, listS, listSSize, data, maxIsleNr, 0, true, showDebug, tmpMaskBitNr);
				
				if (findEndPoint == true)
				{
					//ta tablica przed remapem- dzieki niej wiem, ze front byl i mogl sie rozgalezic - nie tworzyć czerwonej kulkie wtedy
					memset(frontEndTabOutBeforeRemap, 0, 256);
					for (int j = 0; j < frontTabOutLength; j++)
					{
						int val = volumeTempData[frontTabOut[j]];
						if (val > 0) frontEndTabOutBeforeRemap[val] = 1;

					}
				}
				
				RemapOutFrontTabWithDidivedIslands(frontTabOut, frontTabOutLength, frontTabIn, frontTabInLength, frontTabOld, frontTabOldLength, &joints, mask0, tempMaskTab, parentAndAvoid, listS, listSSize, data, maxIsleNr, 0, true, showDebug, tmpMaskBitNr, &jPts2);
				
				if (findEndPoint == true)
				{
					// tutaj sprobuje odnalezc zakonczenia frontow
					// jesli jest front co byl w frontTabIn a nie ma go w frontTabOut 
					// to wyliczam jego srodek i dodaje do listy koncow!

					//bool any1 = false;

					//sprawdzam jakie numery frontów są w nowym podziale
					int cSize = maxIsleNr + 1; if (cSize > 255) cSize = 255;
					memset(frontEndTabOut, 0, 256);
					for (int j = 0; j < frontTabOutLength; j++)
					{
						int val = volumeTempData[frontTabOut[j]];
						if ((val > 0) && (val < 256))
						{
							frontEndTabOut[val] = 1;
							frontVolumeCountTab[val]++;
						}
						//if ((val > 0) && (frontEndTabOut[val] == 1)) { frontEndTab[val] = 2; }
					}

					for (j = 1; j <= maxIsleNr; j++)
					{
						if ((frontEndTabIn[j] == 1) && (frontEndTabOut[j] == 0) && ((frontEndTabOutBeforeRemap[j] == 0)))//był, ale się zmył
						{

							if ((parentAndAvoid[j].createdFromJoint == false))//tego druugiego warunku jeszcz enie dodalem
							{

								if (frontVolumeCountTab[j] > minVParam)
								{

									double p[3] = { 0 };


									bool test = ReplaceNotValidCenterPointByClosestValid(p, data, j, volumeTempData, frontTabIn, frontTabInLength);
									if (test == true)
									{
										JointPoint3D xP;		xP.point3D[0] = p[0];		xP.point3D[1] = p[1];		xP.point3D[2] = p[2];
										xPts.append(xP);

										distVector.append(advDist - 1);
									}
								}

							}
						}

						if ((frontEndTabIn[j] == 1) && (frontEndTabOut[j] == 1))
						{
							//zwiekszam info o dlugosci frontow
							frontLengthTab[j]++;
						}

					}


				}
				
				memset(frontSizeCountTab, 0, 256 * sizeof(int));
				//#pragma omp for   ordered schedule (guided) private(j)

				for (int j = 0; j < frontTabOutLength; j++)
				{

					int val = volumeTempData[frontTabOut[j]];
					if ((val > 0) && (val < 256))frontSizeCountTab[val]++;

					Mask2Data[frontTabOut[j]] = frontTabOut2[j];
					//frontSizeCountTab[frontTabOut2[j]]

				}
				if (blockSmallFronts == true)
				{
					//#pragma omp for   ordered schedule (guided) private(j)
					for (int j = 1; j <= maxIsleNr; j++) //od 3 bo 2 to startowy front
					{
						bool block = false;
						int ss = frontSizeCountTab[j];
						if (ss > 0)
						{
							if ((minSizeToBlock > 0) && (ss < minSizeToBlock)) block = true;
							if ((maxSizeToBlock > 0) && (ss > maxSizeToBlock)) block = true;

							//if ((frontSizeCountTab[j] > 0) && (frontSizeCountTab[j] < minSizeToBlock))
							//	if ((frontSizeCountTab[j] > 0) && ((frontSizeCountTab[j] < minSizeToBlock) || (frontSizeCountTab[j] > maxSizeToBlock)))
							if (block == true)
							{
								frontSizeRepetitionFailureTab[j]++;
							}
						}
					}

					bool outFrontNeedsRebuild = false;
					//#pragma omp for   ordered schedule (guided) private(j)
					for (int j = 1; j <= maxIsleNr; j++)
					{
						if (frontSizeRepetitionFailureTab[j] > nrOfRepetitionsBeforeBlock)
						{
							//zablokuj ten front!!! chyba wystarczy usunąć te punkty w rozrastaniu na nastepna iteracje - trzeba sprawdzic
							outFrontNeedsRebuild = true;
							break;
						}
					}

					if (outFrontNeedsRebuild == true)
					{
						//robie nowa tablice kopie rozmiaru outtab
						//do tablicy przenosze frontouttab ale tylko te elemeny ktore nie sa blokowane
						//robie update rozmiaru tablicy out
						int newSize = 0;
						//#pragma omp for   ordered schedule (guided) private(j)
						for (int j = 0; j < frontTabOutLength; j++)
						{
							int val = volumeTempData[frontTabOut[j]];
							if (val <256)
							if (frontSizeRepetitionFailureTab[val] < nrOfRepetitionsBeforeBlock)
							{
								frontTabOut[newSize++] = frontTabOut[j];
							}
						}

						frontTabOutLength = newSize;


					}


				}
			//	for (int j = 0; j < frontTabOutLength; j++)
			//	{


			//		int val = mask0[frontTabOut[j]];

			//		if (val == 65535)
			//		{
			//			i = i;
			//		}

			//		if ((val > 0) && (val<256))frontSizeCountTab[val]++;

			//		tempMaskTab[frontTabOut[j]] = frontTabOut2[j];
			//	}

			//	if (blockSmallFronts == true)
			//	{

			//		for (int j = 3; j < 256; j++) //od 3 bo 2 to startowy front
			//		{
			//			if ((frontSizeCountTab[j] > 0) && ((frontSizeCountTab[j] < minSizeToBlock) || (frontSizeCountTab[j] > maxSizeToBlock)))
			//			{
			//				frontSizeRepetitionFailureTab[j]++;
			//			}
			//		}

			//		bool outFrontNeedsRebuild = false;
			//		for (int j = 3; j < 256; j++)
			//		{
			//			if (frontSizeRepetitionFailureTab[j] > nrOfRepetitionsBeforeBlock)
			//			{
			//				//zablokuj ten front!!! chyba wystarczy usunąć te punkty w rozrastaniu na nastepna iteracje - trzeba sprawdzic
			//				outFrontNeedsRebuild = true;
			//				break;
			//			}
			//		}

			//		if (outFrontNeedsRebuild == true)
			//		{
			//			//robie nowa tablice kopie rozmiaru outtab
			//			//do tablicy przenosze frontouttab ale tylko te elemeny ktore nie sa blokowane
			//			//robie update rozmiaru tablicy out
			//			int newSize = 0;
			//			for (int j = 0; j < frontTabOutLength; j++)
			//			{
			//				int val = mask0[frontTabOut[j]];
			//				if (val<256)
			//				if (frontSizeRepetitionFailureTab[val] < nrOfRepetitionsBeforeBlock)
			//				{
			//					frontTabOut[newSize++] = frontTabOut[j];
			//				}
			//			}

			//			frontTabOutLength = newSize;


			//		}


			//	}
			}

			memcpy(frontTabOld, frontTabIn, frontTabInLength * sizeof(int));
			frontTabOldLength = frontTabInLength;;


			int *tabswap = frontTabOut; frontTabOut = frontTabIn; frontTabIn = tabswap;
			frontTabInLength = frontTabOutLength;
			if (maxIsleNr > 250) break;
		}

		if (findEndPoint == true)
		{
		for (int j = 1; j <= maxIsleNr; j++)
		{
			if (((frontEndTabOut[j] == 1)))// &&(jointFlagTab[j]==0))&&(jointFlagTab[parentsTab[j]]==0))//|| (frontEndTabOut[j] == 1))//koncowka - dowolny co jest obecnie
			{

				if (frontVolumeCountTab[j] > minVParam)
				{
					double p[3] = { 0 };

					bool test = ReplaceNotValidCenterPointByClosestValid(p, data, j, volumeTempData, frontTabIn, frontTabInLength);
					if (test == true)
					{
						JointPoint3D xP;		xP.point3D[0] = p[0];		xP.point3D[1] = p[1];		xP.point3D[2] = p[2];
						xPts.append(xP);

						distVector.append(advDist);
					}
				}

			}
		}
		}
	//	delete[]tempMaskTab;


		int maxVal = 0;


		//int frontToShow = params[showFrontID];

		if ((dilateSize > 0) || (closingSize>0))
		{

			//pomaluj wszystkie na 2 i potem zrob dilate i po sprawie

			for (int i = 0; i < sliceSize; i++)
			{
				if (mask0[i] > 0)
				{
					mask0[i] = 1;
				}
			}

			int mm = MAX(dilateSize, closingSize);

			int bounds[6];
			GetBoundsWithMargin(data, mask0, dilateSize, true, bounds);


			if (closingSize > 0)
			{
				SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 1, closingSize, kernelType, NULL, bounds, blockMask, NULL, is2d, 2, 0); //z criterion na danych
				SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 0, closingSize, kernelType, NULL, bounds, blockMask, NULL, is2d, 2, 0); //z criterion na danych
			}

			if (dilateSize > 0)
			{
				SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 1, dilateSize, kernelType, NULL, bounds, blockMask, NULL, is2d, 2, 0); //z criterion na danych
			}




		}

		int conuntPts = 0;
		unsigned char *mask1 = new unsigned char[sliceSize];
	//	if (multiFront == true)
		{
			for (int i = 0; i < sliceSize; i++)
			{
				if (mask0[i] > 0)
				{
				//	mask0[i] = 1;
					conuntPts++;
				}
				mask1[i] = mask0[i];;
			}

		}

		//*******************************
		//**********************************
		//*******************************

		if (sampling3DMode == 1) //kopiuje aktualny stan do tablicy uShortTabMixera - zaznaczenie pelna segmentacja
		{
			memset(samplingTab, 0, expMax * sizeof(unsigned short));
			samplesCount = 0;

			for (int i = 0; i < sliceSize; i++)
			{
				if (mask0[i] > 0)
				{
					samplingTab[tab[i]]++;
					samplesCount++;
				}

			}

			if (mixer != NULL)
			{
				if (mixer->uShortTabSize != expMax)
				{
					if (mixer->uShortTab != NULL) delete[]mixer->uShortTab;
					mixer->uShortTab = new unsigned short[expMax];

				}
				memcpy(mixer->uShortTab, samplingTab, expMax * sizeof(unsigned short));
				mixer->uShortTabSize = expMax;
			}
		}


		//**********************************
		//*******************************
		if (findEndPoint == true)
		{
			int r = endPointRadius*1000.0;
			for (int i = 0; i < xPts.count();i++)
			{
				
				double centerE[2] = { xPts.at(i).point3D[0],xPts.at(i).point3D[1] };
				MyShape s;
				s.ellipses.append(QPointF(centerE[0], centerE[1]));//center
				s.ellipses.append(QPointF(r,r));//rads
				s.ellipses.append(QPointF(0, 10));
				s.ellipsesColor.setRgb(params[r2ID], params[g2ID], params[b2ID]);
				shapes->shapeList.append(s);

			}

			for (int i = 0; i < jPts2.count(); i++)
			{

				double centerE[2] = { jPts2.at(i).point3D[0],jPts2.at(i).point3D[1] };
				MyShape s;
				s.ellipses.append(QPointF(centerE[0], centerE[1]));//center
				s.ellipses.append(QPointF(r, r));//rads
				s.ellipses.append(QPointF(0, 10));
				s.ellipsesColor.setRgb(params[r1ID], params[g1ID], params[b1ID]);
				shapes->shapeList.append(s);

			}
		}

		//**********************************


		if (conuntPts > 0)
		{
			int *listTmp = intBuffers[1];
			int kernelSize = 8;
			int size = 1;
			int *kernel = new int[kernelSize];
			int *kernelInc = new int[kernelSize];
			int pos = 0;
			for (int j = -size; j < size + 1; j++)
			{
				for (int i = -size; i < size + 1; i++)
				{
					if (!((i == 0) && (j == 0)))
					{
						kernel[pos] = i + j*n[0];
						kernelInc[pos++] = j;


					}

				}
			}
			int currPos = -1;

		
			for (int i = 0; i < sliceSize; i++)
			{
				if (mask0[i] == 1)
				{
					currPos = i;
					break;
				}
			}
			int prms[7];

			double paramsx[10];

			unsigned char *dilateBuffer1 = new unsigned char[sliceSize];
			memset(dilateBuffer1, 0, sliceSize);




			RegionGrowingOnMask2(currPos, mask1, data, 1, 255, kernel, kernelInc, kernelSize, prms, listS, listTmp);
			double nx1d = 1.0 / n[0];

			int count = prms[0];
			int minx = prms[1];
			int maxx = prms[2];
			int miny = prms[3];
			int maxy = prms[4];
			int width = prms[5];
			int height = prms[6];


			//wyczyszcze samplingTab, zrobie nowe z aktualnymi samplami!
			memset(samplingTab, 0, expMax * sizeof(unsigned short));
			samplesCount = 0;

			if (count > 1)
			{
				int morphologyType = 0;//params[morphologyTypeID];
				int morphologyKernelType = 0;// params[morphologyKernelTypeID];
				int adjacentDist = 1;
				int *dilatePoints = intBuffers[1];//new int[sliceSize];

												  /*	if (calculateStats == true)
												  {
												  GetStatisticsFromVolumeSampledTab(data, listTmp, count, paramsx);
												  }*/

				MyShape shape = createDilateConturPoints(prms, data, listTmp, morphologyKernelType, morphologyType, dilatePoints, col1, 1, dilateBuffer1, adjacentDist); //dilateBuffer zamalowany dylacja
				shape.polyColor = QColor(col1[0], col1[1], col1[2]);;
				shapes->shapeList.append(shape);



				if (shapes->mask == NULL)
				{
					shapes->maskSize = n[0] * n[1];
					shapes->mask = new unsigned char[n[0] * n[1]];
				}
				else
					if (shapes->maskSize != n[0] * n[1])
					{
						delete[]shapes->mask;
						shapes->maskSize = n[0] * n[1];
						shapes->mask = new unsigned char[n[0] * n[1]];
					}
				for (int i = 0; i < n[0] * n[1]; i++)
				{
					if (mask0[i] == 255)
					{
						shapes->mask[i] = 1;
						samplingTab[tab[i]]++;
						samplesCount++;
					}
					else
					{
						shapes->mask[i] = 0;
					}
					//shapes->mask[i] = (mask0[i] == 255) ? 1 : 0;
				}

			}


			delete[]kernel;
			delete[]kernelInc;

			delete[]dilateBuffer1;
			delete[]mask1;

		}


		delete[]frontLengthTab;
		delete[]frontEndTabIn;
		delete[]frontEndTabOut;
		delete[]frontEndTabOutBeforeRemap;
		delete[]frontVolumeCountTab;
		delete cr;
		int multiply = params[multiplyID];

		//qDebug() << "nrOfFronts: " << maxVal;

		if (drawMode == 0)
		{
			for (int i = 0; i < sliceSize; i++)
			{
				usData0[i] = mask0[i] * multiply;
			}
		}
		else if(drawMode == 1) 
		{
			for (int i = 0; i < sliceSize; i++)
			{
				usData0[i] =usData1[i]%254+1;
			}
		}
		else if (drawMode == 2)
		{
			double scale = 255.0*factor1;
			for (int i = 0; i < sliceSize; i++)
			{
				int newval = intensityProbabilityTab1[usData0[i]] * scale;
				usData0[i] = newval;
			}
		}

	}
	



	//*******************************
	//**********************************
	//*******************************
	//**********************************
	//*******************************
	//**********************************


	//for (int i = 0; i < sliceSize; i++)
	//{
	//	if (mask0[i]>maxVal) maxVal = mask0[i];
	//	//	if (mask0[i]>1) mask0[i] = 1;//na chwile zeby zobaczyc czy sie rozrasta

	//	if (mask0[i] == frontToShow) mask0[i] = 1; else mask0[i] = 0;
	//}


	data->SetDataArray(NULL, false);
	delete data;

	
	

	///delete[]mask0;


	//delete[]kernel;
	///delete[]kernelInc;

	//delete[]dilateBuffer1;
	delete[]intensityProbabilityTab1;
	unsigned int t2 = tm.elapsed();
	//qWarning() << "reszta: " << t2 - t1;

	//zrob dylatacje
	//podziel na wyspy
	//sprawdz jakie wyspy spelniaja kryterium rozmiaru
	//wystrzel kolejne segmentacje wzdluz promieni

	//memset(mask0, 255, mnx*mny*sizeof(unsigned char));
	return true;
}
//---------------------------------










//-------------------------------------------------------------
BrightnessContrastFilter::BrightnessContrastFilter()
{
	filterName = "BrightnessContrastFilter";



	QStringList list;
	list.clear(); list << "false" << "true";
	AddParam("brightness", 0, true, -100, 100, 0.01, brightnessID);
	AddParam("contrast", 100, true, 0, 200, 0.01, contrastID);

	list.clear(); list << "false" << "true";
	AddParam("useGlobalThresholds", 1, true, 0, 1, 1, useGlobalThresholdsID, &list);



}
//------------------------------------
int	BrightnessContrastFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *desta, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;

	int globalThresholds[2] = { 0,mainDataMaxValue };
	bool useGlobalThresholds = params[useGlobalThresholdsID];
	if ((shapes != NULL) && (useGlobalThresholds == true))
	{
		globalThresholds[0] = shapes->thresholds[0];
		globalThresholds[1] = shapes->thresholds[1];
	}
	int diff = globalThresholds[1] - globalThresholds[0];
	int mid = globalThresholds[0] + (diff)*0.5;

	unsigned short *buff1 = usData1;
	unsigned short *tmpBuff = tab;


	tmpBuff = tab;

	double tol = 1e-6;

	int maxVal = this->mainDataMaxValue;
	int brightness = params[brightnessID] * diff*0.01;
	double contrast = params[contrastID] * 0.01;


	int sliceSize = mnx*mny;
	int val;
	unsigned short *src = usData0;
	unsigned short *dest = usData1;

	//contrast
	if (fabs(contrast - 1.0) > tol)
	{
		for (int i = 0; i < sliceSize; i++)
		{
			val = tab[i] - mid;
			val *= contrast;
			val += mid;

			if (val < 0) val = 0; if (val > maxVal) val = maxVal;
			dest[i] = val;
		}

	}
	else
	{
		memcpy(dest, tab, sliceSize * sizeof(unsigned short));
	}
	unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
	//brightness

	{
		for (int i = 0; i < sliceSize; i++)
		{
			val = src[i] + brightness; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
			dest[i] = val;
		}
		unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
	}


	/*for (int i = 0; i < sliceSize; i++)
	{
	if (tab[i] < globalThresholds[0]) usData0[i] = globalThresholds[0]-1; else
	if (tab[i] > globalThresholds[1]) usData0[i] = globalThresholds[1]+1;
	}*/
	//qDebug() << "viewType " << viewType << " maxVal " << maxVal;


	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);




	return true;
}
//------------------------------------------------------


IntensitySimpleFilter::IntensitySimpleFilter()
{
	filterName = "IntensitySimpleFilter";





	//int brightnessID;
	//int constrastID;
	//int shadowsID;
	//int highlightsID;
	//int midtonesID;

	//
	//AddParam("centerValue", 32767,true,0,65535,1,centerValID);
	QStringList list;
	//list.clear(); list <<"false" <<"true" ;
	//AddParam("invert", 0,true,0,1,1,invertID,&list);
	//list.clear(); list << "vert" << "horiz";
	//AddParam("direction", 0, true, 0, 1, 1, directionID, &list);
	list.clear(); list << "false" << "true";
	AddParam("brightness", 0, true, -100, 100, 0.01, brightnessID);
	AddParam("contrast", 100, true, 0, 400, 0.01, contrastID);
	//AddParam("centerVal", 50, true, 0, 100, 0.01, centerValueID);

	AddParam("highlightsID", 0, true, -100, 100, 0.01, highlightsID);
	AddParam("midtonesID", 0, true, -100, 100, 0.01, midtonesID);
	AddParam("shadowsID", 0, true, -100, 100, 0.01, shadowsID);
	list.clear(); list << "false" << "true";
	AddParam("useGlobalThresholds", 0, true, 0, 1, 1, useGlobalThresholdsID, &list);


	AddParam("drawHistogram", 0, true, 0, 1, 1, drawHistogramID, &list);
	list.clear(); list << "Transversal" << "Coronal" << "Sagittal";
	AddParam("view", 0, 2, 0, 2, 1, viewID, &list);



}
//------------------------------------
int	IntensitySimpleFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *desta, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;

	int globalThresholds[2] = { 0,mainDataMaxValue };
	bool useGlobalThresholds = params[useGlobalThresholdsID];
	if ((shapes != NULL) && (useGlobalThresholds == true))
	{
		globalThresholds[0] = shapes->thresholds[0];
		globalThresholds[1] = shapes->thresholds[1];
	}
	int diff = globalThresholds[1] - globalThresholds[0];
	int mid = globalThresholds[0] + (diff)*0.5;


	unsigned short *buff1 = usData1;
	unsigned short *tmpBuff = tab;


	tmpBuff = tab;

	double tol = 1e-6;

	int maxVal = this->mainDataMaxValue;
	int brightness = params[brightnessID] * diff*0.01;
	double contrast = params[contrastID] * 0.01;


	//int centerVal = params[centerValueID] * 0.01*maxVal;;
	//int brightnessID;
	//int constrastID;
	//int shadowsID;
	//int highlightsID;
	//int midtonesID;

	//int mid = centerVal;// maxVal / 2;

	int sliceSize = mnx*mny;
	int val;
	unsigned short *src = usData1;
	unsigned short *dest = usData0;

	//contrast
	if (fabs(contrast - 1.0) > tol)
	{
		for (int i = 0; i < sliceSize; i++)
		{
			val = tab[i] - mid;
			val *= contrast;
			val += mid;

			if (val < 0) val = 0; if (val > maxVal) val = maxVal;
			dest[i] = val;
		}

	}
	else
	{
		memcpy(dest, tab, sliceSize * sizeof(unsigned short));
	}
	unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
	//brightness

	{
		for (int i = 0; i < sliceSize; i++)
		{
			val = src[i] + brightness; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
			dest[i] = val;
		}
		unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
	}


	if (useGlobalThresholds == false)
	{
		int highlights = params[highlightsID] * maxVal*0.01;
		int shadows = params[shadowsID] * maxVal*0.01;
		int midtones = params[midtonesID] * maxVal*0.01;

		//highlights
		int tt[3] = { highlights,shadows,midtones };
		double *ttTab[3];// = { higFunc,shaFunc,midFunc };
		ttTab[0] = higFunc;	ttTab[1] = shaFunc;	ttTab[2] = midFunc;
		// highlights
		double scaler = 65535.0 / maxVal;
		for (int j = 0; j < 3; j++)
		{
			double *func = ttTab[j];
			int xVal = tt[j];
			if (func != NULL)

				if (fabs(xVal) > tol)
				{
					for (int i = 0; i < sliceSize; i++)
					{
						val = src[i] + xVal*func[(int)(src[i] * scaler)]; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
						dest[i] = val;
					}
					unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
				}
		}

		////shadows
		//if (shaFunc != NULL)
		//	if (fabs(shadows) > tol)
		//	{
		//		for (int i = 0; i < sliceSize; i++)
		//		{
		//			val = src[i] + shadows*shaFunc[src[i]]; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
		//			dest[i] = val;
		//		}
		//		unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
		//	}
		////midtones
		//if (midFunc != NULL)
		//	if (fabs(midtones) > tol)
		//	{
		//		for (int i = 0; i < sliceSize; i++)
		//		{
		//			val = src[i] + midtones*midFunc[src[i]]; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
		//			dest[i] = val;
		//		}
		//		unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
		//	}
	}
	else
	{
		int highlights = params[highlightsID] * diff*0.01;
		int shadows = params[shadowsID] * diff*0.01;
		int midtones = params[midtonesID] * diff*0.01;
		double scaler = (double)65535.0 / diff;
		int tt[3] = { highlights,shadows,midtones };
		double *ttTab[3];// = { higFunc,shaFunc,midFunc };
		ttTab[0] = higFunc;	ttTab[1] = shaFunc;	ttTab[2] = midFunc;
		// highlights
		for (int j = 0; j < 3; j++)
		{
			double *func = ttTab[j];
			int xVal = tt[j];
			if (func != NULL)

				if (fabs(xVal) > tol)
				{


					for (int i = 0; i < sliceSize; i++)
					{
						int srcv = src[i];
						if ((srcv > globalThresholds[0]) && (srcv < globalThresholds[1]))
						{
							int pos = (srcv - globalThresholds[0])*scaler;
							val = src[i] + xVal*func[pos]; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
							dest[i] = val;
						}
						else
							dest[i] = srcv;
					}
					unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
				}
		}

	}



	//if (dest != usData0) 	memcpy(usData0, src, sliceSize * sizeof(unsigned short));

	if (usData0 != src)
	{
		unsigned short *tmp;
		tmp = usData1;
		usData1 = usData0;
		usData0 = tmp;
	}


	if (params[drawHistogramID] == true)
	{
		if (viewType == params[viewID])
		{



			QColor backCol(Qt::black);
			QColor col1(Qt::blue);
			int scatterSize = 256;
			bool useHistLogScale = true;
			QImage *visImage = iWidget->GetCurrentImage();

			int *hist1 = new int[scatterSize];
			memset(hist1, 0, scatterSize * sizeof(int));
			double scale = ((double)(scatterSize - 1)) / maxVal;

			for (int i = 0; i < sliceSize; i++)
			{
				int val = usData0[i] * scale;
				hist1[val]++;
			}

			DrawHistogram(scatterSize, hist1, 0, 10, 255, 120, useHistLogScale, backCol, col1, visImage);
			delete[]hist1;
			iWidget->update();
		}
	}

	//drawhist


	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);




	return true;
}

IntensitySimpleFilter::~IntensitySimpleFilter()
{

	/*if (higFunc != NULL) delete[]shaFunc;
	if (shaFunc != NULL) delete[]higFunc;
	if (midFunc != NULL) delete[]midFunc;*/

}
//----------------------------------


//----------------------------------
LayerConvolutionPredefinedFilter::LayerConvolutionPredefinedFilter()
{
	filterName = "LayerConvolutionPredefinedFilter";
	QStringList list;
	int pos = 0;
	for (int i = 0; i < NUMBER_OF_FILTERS - 1; i++)
	{
		list << defs.Filter2DNameDefs[i];
		filterStart.append(pos);
		int size = defs.data[pos + 1];
		pos += size*size + 2;
		if (pos >= SIZE_OF_FILTER_TAB)
		{
			break;
		}

	}
	AddParam("mode", 0, 2, 0, list.count() - 1, 1, filterNrID, &list);

}
//-------------------------
int	LayerConvolutionPredefinedFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	////if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	////unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;
	//unsigned short *tab=data->GetDataArray()+(sliceNr*sliceSize);

	//	double stddev=params[0];
	int filterNr = params[filterNrID];
	int fs = filterStart.at(filterNr);


	int div = defs.data[fs];
	int radius = defs.data[fs + 1];


	int type = 0;//kwadrat

	if (radius<1) return 0;
	//unsigned char *src,*dest;
	//unsigned short *buff0=usData0;
	//unsigned short *buff1=usData1;
	//jesli rozmiar range raw data jest wiekszy od 255 to robie przeskalowanie

	/*src=new unsigned char[sliceSize];
	dest=new unsigned char[sliceSize];*/




	// tu odpalenie filterka



	int nz = 1;
	int nx = mnx;
	int ny = mny;
	int sx = nx;
	int sliceSize = nx*ny;
	int dsizemin1 = sliceSize - 1;
	//------------------------------------------


	int maxvalue = 255;
	int kernelSize = 0;
	int tmpPos;
	int count;
	//int radius = 1;
	//int type = 0;
	int ssize = radius / 2;
	RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
	int *kernel = GetGrowingKernel2D(kernelSize, ssize, data, type, true);
	delete data;

	int *filterKernel = new int[radius*radius];
	memcpy(filterKernel, (defs.data) + fs + 2, radius*radius * sizeof(int));
	int *tab1 = new int[kernelSize];
	//double *tab1=new double[2*maxvalue];


	int pos = 0;
	//int l;

	double stddev, mean;
	double localMean, localStddev;
	//double nx1d=1.0/nx;

	double kVal, fVal;
	double cVal;
	double cMinVal;
	double finalVal;
	int chgs;
	int tmpPos2;

	chgs = 0;
	pos = 0;


	//nrOfThreads = omp_get_num_threads();
	unsigned short **tabz;
	int **tabz2;

	tabz = new unsigned short *[nrOfThreads];
	tabz2 = new int*[nrOfThreads];

	for (int i = 0; i<nrOfThreads; i++)
	{
		tabz[i] = new unsigned short[kernelSize];
		tabz2[i] = new int[kernelSize];
	}

	int tid = omp_get_thread_num();
#pragma omp parallel for  ordered schedule (guided) 
	for (int j = 0; j < sliceSize; j++)
	{
		int weight = 0;

		int tmpSize;
		int tid = omp_get_thread_num();
		//GetKernelIntesitiesToTab(tab, tabz[tid], tmpSize, kernel, kernelSize, j, dsizemin1);
		weight = GetKernelIntesitiesToTabForConvolution(tab, tabz[tid], filterKernel, tabz2[tid], tmpSize, kernel, kernelSize, j, dsizemin1);
		int c = Convolve(tabz[tid], tabz2[tid], tmpSize);

		if (weight > 0)
		{
			c /= weight;
		}
		if (c < 0) c = 0; else if (c > mainDataMaxValue) c = mainDataMaxValue;
		usData0[j] = c;
		/*	if (tmpSize > 1)
		{

		usData0[j] = Median(tabz[tid], tmpSize - 1);


		}
		}
		else
		{
		usData0[j] = tab[j];
		}*/

	}

	for (int i = 0; i<nrOfThreads; i++)
	{
		delete[]tabz[i]; tabz[i] = NULL;
		delete[]tabz2[i]; tabz[i] = NULL;
	}
	delete[]tabz;
	delete[]tabz2;
	delete[]kernel;
	delete[]tab1;
	delete[]filterKernel;

	//-----------------------------------------------





	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

	return 1;
}
//--------------------------------




CannyEdgeDetectorFilter::CannyEdgeDetectorFilter()
{
	filterName = "CannyEdgeDetectorFilter";



	//int brightnessID;
	//int constrastID;
	//int shadowsID;
	//int highlightsID;
	//int midtonesID;

	//
	//AddParam("centerValue", 32767,true,0,65535,1,centerValID);
	QStringList list;
	//list.clear(); list <<"false" <<"true" ;
	//AddParam("invert", 0,true,0,1,1,invertID,&list);
	//list.clear(); list << "vert" << "horiz";
	//AddParam("direction", 0, true, 0, 1, 1, directionID, &list);
	list.clear(); list << "false" << "true";
	AddParam("epsilonDerivative", 0.95, true, 0.01, 2, 0.01, epsilonDerivativeID);
	AddParam("epsilonNorm", 0.5, true, 0.01, 1, 0.01, epsilonNormID);
	AddParam("radius", 2, true, 1, 100, 1, radiusID);
	AddParam("stddev", 0.3, true, 0.03, 2, 0.01, stddevID);
	AddParam("scale", 1, true, 0.01, 1, 0.01, scaleID);






}
//------------------------------------
int	CannyEdgeDetectorFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *desta, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;

	double epsilonNorm = params[epsilonNormID];
	double epsilonDerivative = params[epsilonDerivativeID];
	unsigned short *buff0 = usData0;
	unsigned short *buff1 = usData1;
	unsigned short *buffTmp;
	int i, j;
	//	bool revth=maskThresholds[2];
	unsigned char val1, val2;
	double scale = params[scaleID];


	radius = params[radiusID];
	stddev = params[stddevID];



	if (radius<1) return 0;
	int					size2 = 2 * radius + 1;
	double				*gaussianKernelTab = new double[size2];
	double				*gaussian1stDerivKernelTab = new double[size2];
	unsigned short		*intensityTab = new unsigned short[size2];
	GetGaussianKernel(gaussianKernelTab, (double)radius, stddev);
	GetGaussianKernelFirstDerivative(gaussian1stDerivKernelTab, (double)radius, stddev);

	unsigned short *Q;

	int pos;

	Q = tab;
	pos = 0;
	int n[2] = { mnx, mny };

	unsigned short *tmpBuff = tab;



	unsigned short **tabz;
	//#pragma omp parallel private (tabz) num_threads(4)
	{
		unsigned short *datatab = tab;
		tabz = new unsigned short *[nrOfThreads];
		//#pragma omp critical
		{
			for (i = 0; i<nrOfThreads; i++)
			{
				tabz[i] = new unsigned short[size2];
			}
		}
		//	int tid = omp_get_thread_num();
		;// cout << tid''
#pragma omp parallel for  ordered schedule (guided) 
		for (int j = 0; j < mny; j++)
		{
			int pos = j*mnx;
			int  tid = omp_get_thread_num();
			for (int i = 0; i < mnx; i++)
			{

				GetIntensityTab1D(tmpBuff, n[0], n[1], 1, 0, radius, tabz[tid], i, j, 0);
				buff0[pos++] = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
			}
		}

		//y-filtering
		pos = 0;

		int val;
#pragma omp parallel for  ordered schedule (guided) 
		for (int j = 0; j < mny; j++)
		{
			int pos = j*mnx;
			int  tid = omp_get_thread_num();
			for (int i = 0; i<mnx; i++)
			{
				GetIntensityTab1D(buff0, n[0], n[1], 1, 1, radius, tabz[tid], i, j, 0);
				buff1[pos++] = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
			}
		}
		//#pragma omp critical
		{
			for (i = 0; i<nrOfThreads; i++)
			{
				delete[]tabz[i]; tabz[i] = NULL;
			}
			delete[]tabz;
		}
	}




	double *doubleTab = new double[4 * sliceSize];
	memset(doubleTab, 0, 4 * sliceSize * sizeof(double));
	double *gx, *gy, *gz, *norm;
	double *tmp;
	//double *gz=new double[sliceSize];

	unsigned short *sz;
	int pp = 0;
	gx = &doubleTab[pp]; pp += sliceSize;
	gy = &doubleTab[pp]; pp += sliceSize;
	gz = &doubleTab[pp]; pp += sliceSize;
	norm = &doubleTab[pp]; pp += sliceSize;

	//memset(gz, 0, sliceSize * sizeof(double));

	sz = buff1;

	//unsigned short	*tmpBuffUShort = new unsigned short[size2];
	GetGradient2DTab(gx, sz, gaussian1stDerivKernelTab, buff0, radius, 0, mnx, mny, 1, 0);
	GetGradient2DTab(gy, sz, gaussian1stDerivKernelTab, buff0, radius, 1, mnx, mny, 1, 0);
	Get3DModulus(norm, gx, gy, gz, sliceSize);

	int dims[2] = { mnx,mny };
	Remove_Gradient_NonMaxima_Slice_2D(gz, gx, gy, norm, dims, epsilonNorm, epsilonDerivative);



	sz = NULL;
	//	delete[]tmpBuffUShort;

	double min = INT_MAX;
	double max = -INT_MAX;
	for (i = 0; i<sliceSize; i++)
	{
		if (gz[i]<min) min = gz[i];
		if (gz[i]>max) max = gz[i];
	}
	double factor = (max - min)*scale;
	factor = (factor != 0) ? factor = (double)mainDataMaxValue / factor : factor = 1;


	for (int i = 0; i<sliceSize; i++)
	{
		int val = (gz[i] - min)*factor;;
		usData0[i] = (val > mainDataMaxValue) ? mainDataMaxValue : val;
	}

	////swap buffers
	//buffTmp = usData0;
	//usData0 = usData1;
	//usData1 = buffTmp;


	delete[]gaussianKernelTab;
	delete[]gaussian1stDerivKernelTab;
	delete[]intensityTab;
	delete[]doubleTab;




	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);
	return 1;
}

CannyEdgeDetectorFilter::~CannyEdgeDetectorFilter()
{

	/*if (higFunc != NULL) delete[]shaFunc;
	if (shaFunc != NULL) delete[]higFunc;
	if (midFunc != NULL) delete[]midFunc;*/

}



//class HistogramEqualizationFilter : public LayerFilter
//{
//	int threshID;
//	int modeID;
//
//





HistogramEqualizationFilter::HistogramEqualizationFilter()
{
	filterName = "HistogramEqualizationFilter";



	//int brightnessID;
	//int constrastID;
	//int shadowsID;
	//int highlightsID;
	//int midtonesID;

	//
	//AddParam("centerValue", 32767,true,0,65535,1,centerValID);
	QStringList list;
	//list.clear(); list <<"false" <<"true" ;
	//AddParam("invert", 0,true,0,1,1,invertID,&list);
	//list.clear(); list << "vert" << "horiz";
	//AddParam("direction", 0, true, 0, 1, 1, directionID, &list);
	list.clear(); list << "normal" << "alpha" << "xa";
	AddParam("thresh", 32758, true, 0, 65535, 1, threshID);
	AddParam("mode", 0, 1, 0, 2, 1, modeID, &list);
	//list.clear(); list << "abs" << "percent";
	//AddParam("mode", 0, 1, 0, 1, 1, modeID, &list);


}
//------------------------------------
int	HistogramEqualizationFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *desta, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;




	//unsigned short *buff1 = usData1;
	//unsigned short *tmpBuff = tab;

	//int mode = params[modeID];
	int mode = params[modeID];
	int valI = params[threshID];

	//int valMode = 1;

	/*if (valMode == 0)
	{
	xa = valI;
	}
	else
	{
	xa = (valI / 65535.0)*mainDataMaxValue;
	}*/
	double alpha = valI / 65535.0;

	RawDataSet *data = new RawDataSet(mnx, mny, 1, mdx, mdy, 1, 0, false);
	data->SetDataArray(tab);
	data->CalculateHistogram();
	int *histogram = data->GetHistogramData();

	int maxVp1 = data->GetLastHistValue() + 1;
	int maxV = data->GetLastHistValue();


	int size = mnx*mny;
	double scale = maxV / (double)size;
	double nl = (double)(size) / maxVp1;

	if (mode == 1)//trzeba przerobic histogram wg algorytmu
	{
		for (int i = 1; i < maxVp1; i++)
		{
			histogram[i] = alpha*(histogram[i] - nl) + nl;
		}


	}

	int *cumulativeHist = new int[maxVp1];
	int *lookupTab = new int[maxVp1];
	memset(lookupTab, 0, (maxVp1) * sizeof(int));
	memset(cumulativeHist, 0, (maxVp1) * sizeof(int));


	//cumulative hist
	cumulativeHist[0] = histogram[0];
	long long xa = 0;
	for (int i = 1; i < maxVp1; i++)
	{
		xa += histogram[i] * i;
		cumulativeHist[i] += histogram[i] + cumulativeHist[i - 1];

	}
	xa /= size;

	if ((mode == 0) || (mode == 1))//normalny histogram
	{
		for (int i = 0; i < maxVp1; i++)
		{
			lookupTab[i] = cumulativeHist[i] * scale;
		}


	}
	else if (mode == 2)//xa
	{
		int h1 = 0;
		int xa1 = xa - 1;


		for (int i = 0; i < xa; i++)
		{
			h1 += histogram[i];
		}
		int h2 = size - h1;
		double scale2 = (h1>0) ? (double)xa1 / h1 : 1;

		double scale3 = (h2 > 0) ? (double)(maxVp1 - xa) / h2 : 1;
		for (int i = 0; i < xa; i++)
		{
			lookupTab[i] = cumulativeHist[i] * scale2;

		}
		for (int i = xa; i < maxVp1; i++)
		{
			lookupTab[i] = (cumulativeHist[i] - h1)*scale3 + xa;
		}

	}



	for (int i = 0; i < size; i++)
	{
		usData0[i] = lookupTab[tab[i]];

	}
	delete[]lookupTab;
	delete[]cumulativeHist;


	data->SetDataArray(NULL, false);
	delete data;

	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);




	return true;
}


LayerThresholdFilter::LayerThresholdFilter()
{
	filterName = "LayerThresholdFilter";


	QStringList list;
	list.clear(); list << "false" << "true";
	AddParam("lefthTh", 0, true, 0, 1, 0.0001, threshLowerID);
	AddParam("righthTh", 1, true, 0, 1, 0.0001, threshUpperID);
	AddParam("invThresh", 0, true, 0, 1, 1, invThreshID, &list);
	//numberOfParams=0;
	//this->paramsNames[numberOfParams]="temporal average";	defaultParams[numberOfParams]=0;		paramsSliders[numberOfParams]=true;		this->minVals[numberOfParams]=0;		this->maxVals[numberOfParams]=20;	this->steps[numberOfParams]=1;		numberOfParams++;
	//	
}
//------------------------------------
int	LayerThresholdFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{



	int leftTh = params[threshLowerID] * mainDataMaxValue;;
	int rightTh = params[threshUpperID] * mainDataMaxValue;
	bool invTh = params[invThreshID];




	int val;
	bool test;
	for (int i = 0; i<sliceSize; i++)
	{
		val = tab[i];
		test = false;
		if ((val >= leftTh) && (val <= rightTh)) { test = true; }

		if (invTh == true) test = !test;

		usData0[i] = test*mainDataMaxValue;
	}

	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);


	return true;



}

//----------------------------------------------
//-----------------------------------------------
//----------------------------------------------
//-----------------------------------------------
//----------------------------------------------
//-----------------------------------------------
//----------------------------------------------
//-----------------------------------------------
//----------------------------------------------
//-----------------------------------------------
//----------------------------------------------
//-----------------------------------------------
//----------------------------------------------
//-----------------------------------------------
void initLayerFilters(LinkedList <LayerFilter*> *layerFilterList)
{

	layerFilterList->InsertItem(new LayerEmptyFilter());//works
	layerFilterList->InsertItem(new LayerThresholdFilter());//works
	layerFilterList->InsertItem(new HistogramEqualizationFilter());//works
	layerFilterList->InsertItem(new IntensityFunctionFilter());


	layerFilterList->InsertItem(new BrightnessContrastFilter()); //works
	layerFilterList->InsertItem(new IntensitySimpleFilter()); //works
	layerFilterList->InsertItem(new 	LayerConvolutionPredefinedFilter()); // works
	layerFilterList->InsertItem(new 	CannyEdgeDetectorFilter()); // works


	layerFilterList->InsertItem(new LayerMagnifyFilter());//works

	layerFilterList->InsertItem(new LayerMutualIntensityAnalysisFilter());
	layerFilterList->InsertItem(new LayerMixerLiteFilter());
	layerFilterList->InsertItem(new LayerMeasureFilter1()); //works
#if defined (USG)
	layerFilterList->InsertItem(new LayerSapheniaFilter());
#endif	
	layerFilterList->InsertItem(new LayerDistanceTransformFilter());
	layerFilterList->InsertItem(new LayerBilateralFilter());
	layerFilterList->InsertItem(new LayerGaussianFilter());
	layerFilterList->InsertItem(new LayerUnsharpMaskFilter());
	layerFilterList->InsertItem(new LayerHighPassFilter());
	layerFilterList->InsertItem(new LayerMedianFast2DFilter());
	layerFilterList->InsertItem(new LayerMedianFilter());
	layerFilterList->InsertItem(new LayerAnisotropicDiffusionFilter());
	layerFilterList->InsertItem(new LayerStatisticalDirectionalFilter());
	layerFilterList->InsertItem(new LayerStatisticalFilter1());
	layerFilterList->InsertItem(new LayerStatisticalFilter1Faster());
	layerFilterList->InsertItem(new LayerStatisticalFilter1Faster2());
	layerFilterList->InsertItem(new LayerStatisticalFilterDualCross());
	layerFilterList->InsertItem(new LayerStatisticalHomogeneousMaskFilter());
	layerFilterList->InsertItem(new LayerAbsUcharFilter());
	layerFilterList->InsertItem(new LayerAbsUShortFilter());
	layerFilterList->InsertItem(new LayerGradientTestFilter());
	layerFilterList->InsertItem(new LayerGrowingRegionFilter());
	layerFilterList->InsertItem(new LayerEmptyResampleFilter());
	layerFilterList->InsertItem(new LayerHistogramLineViewFilter());
	layerFilterList->InsertItem(new LayerMorphologyFilter());

	





}






IntensityFunctionFilter::IntensityFunctionFilter()
{
	filterName = "IntensityFunctionFilter";



	//int brightnessID;
	//int constrastID;
	//int shadowsID;
	//int highlightsID;
	//int midtonesID;

	//
	//AddParam("centerValue", 32767,true,0,65535,1,centerValID);
	QStringList list;
	//list.clear(); list <<"false" <<"true" ;
	//AddParam("invert", 0,true,0,1,1,invertID,&list);
	//list.clear(); list << "vert" << "horiz";
	//AddParam("direction", 0, true, 0, 1, 1, directionID, &list);
	list.clear(); list << "false" << "true";
	AddParam("scale", 1, true, 0.0001, 1, 0.0001, scaleID);
	AddParam("gamma", 0.1, true, 0.1, 5, 0.00001, gammaID);
	AddParam("param1", 0, true, 0, 1, 0.00001, param1ID);
	AddParam("param2", 1, true, 0, 1, 0.00001, param2ID);


	list.clear(); list << "logTH" << "1-logTH" << "pow" << "1-pow";
	AddParam("function", 0, 2, 0, 3, 1, functionID, &list);
	list.clear(); list << "false" << "true";
	AddParam("drawHistogram", 0, true, 0, 1, 1, drawHistogramID, &list);
	list.clear(); list << "Transversal" << "Coronal" << "Sagittal";
	AddParam("view", 0, 2, 0, 2, 1, viewID, &list);


}
//------------------------------------
int	IntensityFunctionFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short *&usData0, unsigned short *&usData1, double *params, ImageShapes 	*shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *srca, unsigned char *desta, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{


	//if ((sliceNr<0)||(sliceNr>=data->GetNz())) return 0;
	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*sliceSize):dataTab;




	unsigned short *buff1 = usData1;
	unsigned short *tmpBuff = tab;


	tmpBuff = tab;

	double tol = 1e-6;

	int func = params[functionID];
	double p1 = params[param1ID];
	double p2 = params[param2ID];

	int lVal = 0;
	int rVal = mainDataMaxValue;
	double *function = new double[mainDataMaxValue + 1];

	double scale = params[scaleID];// (mainDataMaxValue > 0) ? 1.0 / mainDataMaxValue : 1;
	scale *= scale;
	double gamma = params[gammaID];
	gamma *= gamma;
	/*if (func == 0)
	{

	for (int i = 0; i < mainDataMaxValue; i++)
	{
	function[i] = log(1 + i*scale);
	}
	}
	else if (func == 1)
	{

	for (int i = 0; i < mainDataMaxValue; i++)
	{
	function[i] = log(1 + (mainDataMaxValue-i)*scale);
	}
	lVal = mainDataMaxValue;
	rVal = 0;
	}
	else*/ if (func == 0)
	{
		//th

		double th0 = p1*mainDataMaxValue;
		double th1 = p2*mainDataMaxValue;

		if ((fabs(th1 - th0)<tol) || (th1<th0))
		{
			for (int i = 0; i < mainDataMaxValue; i++)
			{
				function[i] = i;
			}
		}
		else
		{
			for (int i = 0; i < th0; i++)
			{
				function[i] = -65536;
			}
			double sc = mainDataMaxValue / (double)(th1 - th0);
			for (int i = th0; i < th1; i++)
			{
				double val = (i - th0)*sc;
				function[i] = log(1 + (val)*scale);
			}
			for (int i = th1; i <= mainDataMaxValue; i++)
			{
				function[i] = 65536;
			}

		}
	}
	else if (func == 1)
	{
		//1-th

		double th0 = p1*mainDataMaxValue;
		double th1 = p2*mainDataMaxValue;

		if ((fabs(th1 - th0)<tol) || (th1<th0))
		{
			for (int i = 0; i < mainDataMaxValue; i++)
			{
				function[i] = i;
			}
		}
		else
		{
			for (int i = 0; i < th0; i++)
			{
				function[i] = -65536;
			}
			double sc = mainDataMaxValue / (double)(th1 - th0);
			for (int i = th0; i < th1; i++)
			{
				double val = (th1 - (i - th0))*sc;
				function[i] = log(1 + (val)*scale);
			}
			for (int i = th1; i <= mainDataMaxValue; i++)
			{
				function[i] = 65536;
			}

		}
		lVal = mainDataMaxValue;
		rVal = 0;
	}
	else
		if (func == 2)
		{
			//pow

			double th0 = p1*mainDataMaxValue;
			double th1 = p2*mainDataMaxValue;

			if ((fabs(th1 - th0)<tol) || (th1<th0))
			{
				for (int i = 0; i < mainDataMaxValue; i++)
				{
					function[i] = i;
				}
			}
			else
			{
				for (int i = 0; i < th0; i++)
				{
					function[i] = -65536;
				}
				double sc = 1.0 / (double)(th1 - th0);
				for (int i = th0; i < th1; i++)
				{
					double val = (i - th0)*sc;
					function[i] = pow(val, gamma);// log(1 + (val)*scale);
				}
				for (int i = th1; i <= mainDataMaxValue; i++)
				{
					function[i] = 65536;
				}

			}
		}
		else if (func == 3)
		{
			//1-th

			double th0 = p1*mainDataMaxValue;
			double th1 = p2*mainDataMaxValue;

			if ((fabs(th1 - th0)<tol) || (th1<th0))
			{
				for (int i = 0; i < mainDataMaxValue; i++)
				{
					function[i] = i;
				}
			}
			else
			{
				for (int i = 0; i < th0; i++)
				{
					function[i] = -65536;
				}
				double sc = 1.0 / (double)(th1 - th0);
				for (int i = th0; i < th1; i++)
				{
					double val = (th1 - (i - th0))*sc;
					function[i] = pow(val, gamma); //log(1 + (val)*scale);
				}
				for (int i = th1; i <= mainDataMaxValue; i++)
				{
					function[i] = 65536;
				}

			}
			lVal = mainDataMaxValue;
			rVal = 0;
		}
		else
			for (int i = 0; i < mainDataMaxValue; i++)
			{
				function[i] = i;
			}




	double minVal = INT_MAX;
	double maxVal = -INT_MAX;
	for (int i = 0; i < mainDataMaxValue; i++)
	{

		if ((function[i] > -65535) && (function[i] < 65536))
		{
			if (function[i] < minVal) minVal = function[i];
			if (function[i] > maxVal) maxVal = function[i];
		}
	}
	//uzupelniam dla tych ponizej i powyzej zakresu 
	for (int i = 0; i < mainDataMaxValue; i++)
	{

		if (function[i] <-65535) function[i] = lVal;
		if (function[i] > 65535) function[i] = rVal;

	}

	double dScale = (maxVal - minVal);
	if (dScale > 0) dScale = 1.0 / dScale;

	double val;

	for (int i = 0; i < mainDataMaxValue; i++)
	{
		double val = (function[i] - minVal)*dScale;
		val *= mainDataMaxValue;//normalizacja
								//	
		if (val < 0) val = 0; if (val > mainDataMaxValue) val = mainDataMaxValue;
		function[i] = val;
	}

	for (int i = 0; i < sliceSize; i++)
	{
		//val = (function[tab[i]]-minVal)*dScale;//zakres 0-1
		//if (val > 0)
		//{
		//i = i;
		//}
		//val *= mainDataMaxValue;//normalizacja
		//	
		//if (val < 0) val = 0; if (val > mainDataMaxValue) val = mainDataMaxValue;
		//usData0[i] = val;
		usData0[i] = function[tab[i]];

	}



	////highlights
	//if (higFunc != NULL)
	//	if (fabs(highlights) > tol)
	//	{
	//		for (int i = 0; i < sliceSize; i++)
	//		{
	//			val = src[i] + highlights*higFunc[src[i]]; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
	//			dest[i] = val;
	//		}
	//		unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
	//	}

	////shadows
	//if (shaFunc != NULL)
	//	if (fabs(shadows) > tol)
	//	{
	//		for (int i = 0; i < sliceSize; i++)
	//		{
	//			val = src[i] + shadows*shaFunc[src[i]]; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
	//			dest[i] = val;
	//		}
	//		unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
	//	}
	////midtones
	//if (midFunc != NULL)
	//	if (fabs(midtones) > tol)
	//	{
	//		for (int i = 0; i < sliceSize; i++)
	//		{
	//			val = src[i] + midtones*midFunc[src[i]]; if (val < 0) val = 0; if (val > maxVal) val = maxVal;
	//			dest[i] = val;
	//		}
	//		unsigned short *tmp;		tmp = src;		src = dest;		dest = tmp;
	//	}






	/*if (usData0 != src)
	{
	unsigned short *tmp;
	tmp = usData1;
	usData1 = usData0;
	usData0 = tmp;
	}

	*/
	if (params[drawHistogramID] == true)
	{
		if (viewType == params[viewID])
		{



			QColor backCol(Qt::black);
			QColor col1(Qt::blue);
			int scatterSize = 256;
			bool useHistLogScale = true;
			QImage *visImage = iWidget->GetCurrentImage();

			int *hist1 = new int[scatterSize];
			memset(hist1, 0, scatterSize * sizeof(int));
			double scale = ((double)(scatterSize - 1)) / mainDataMaxValue;

			for (int i = 0; i < sliceSize; i++)
			{
				int val = usData0[i] * scale;
				hist1[val]++;
			}

			DrawHistogram(scatterSize, hist1, 0, 10, 255, 120, useHistLogScale, backCol, col1, visImage);

			QPainter p(visImage);
			int tHist[256] = { 0 };

			double sc = (mainDataMaxValue>0) ? 255.0 / mainDataMaxValue : 1;
			double scY = 255.0 / mainDataMaxValue;
			for (int i = 0; i < mainDataMaxValue; i++)
			{
				val = function[i];
				//int val = function[i];
				int scaledPos = i*sc;
				if (tHist[scaledPos] < val)
				{
					tHist[scaledPos] = val;
				}
			}

			QPolygon poly;
			for (int i = 0; i < 255; i++)
			{
				int x = i;
				int y = 255 - tHist[i] * scY;
				poly.append(QPoint(x, y));
			}
			p.setPen(Qt::yellow);
			p.drawPolyline(poly);
			delete[]hist1;
			iWidget->update();
		}
	}
	delete[]function;
	//drawhist

	updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);



	return true;
}

IntensityFunctionFilter::~IntensityFunctionFilter()
{

	/*if (higFunc != NULL) delete[]shaFunc;
	if (shaFunc != NULL) delete[]higFunc;
	if (midFunc != NULL) delete[]midFunc;*/

}

//---------------------------------

LayerHistogramLineViewFilter::LayerHistogramLineViewFilter()
{
	filterName = "LayerHistogramLineViewFilter";
	QStringList list;
	list.clear(); list << "vert" << "horiz";
	AddParam("direction", 0, true, 0, 1, 1, orientationID, &list);
	AddParam("size", 0.3, true, 0.1, 1, 0.01, sizeID);
	AddParam("r1", 255, true, 0, 255, 1, r1ID);
	AddParam("g1", 255, true, 0, 255, 1, g1ID);
	AddParam("b1", 255, true, 0, 255, 1, b1ID);
	list.clear(); list << "false" << "true";
	AddParam("followMouse", 0, true, 0, 1, 1, followMouseID, &list);
	AddParam("centerHist", 0, true, 0, 1, 1, centerHistID, &list);
	AddParam("showText", 0, true, 0, 1, 1, showTextID, &list);
	AddParam("logScale", 0, true, 0, 1, 1, logScaleID, &list);

	

	

	


}
//-------------------------
int	LayerHistogramLineViewFilter::ApplyFiltering(RawDataMixerLite *mixer, unsigned short*&usData0, unsigned short *&usData1, double *params, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *tab, bool inverse, unsigned char *src, unsigned char *dest, unsigned short *prevImg, unsigned char *mask, int **intBuffers)
{

	
	{

		

		double scale = 1;
		double maxv = GetMaxValue(tab,sliceSize);
		

		

	
		double center[2] = { mnx / 2.0, mny / 2.0 };
		if (shapes != NULL)
		{
			center[0] = shapes->clickedLeftPoint.rx();
			center[1] = shapes->clickedLeftPoint.ry();


		}
		MyShape shape;
		shape.representation = 1;

		int orientation = params[orientationID];
		double percent = params[sizeID];

		bool followMouse = params[followMouseID];
		bool centerHist = params[centerHistID];

		double min = INT_MAX;
		double max = -INT_MAX;
		double stdDev;

		double mean = 0;
		double mean2 = 0;
		

		int volSize =mnx*mny;
		int count = 0;
		int maxValue = 0;


		bool logScale = params[logScaleID];
		double maxUsed = (logScale == true) ? log(1 + (double)(maxv)) : maxv;
		if (orientation == 0)
		{
			int ss = mnx*percent;
			int pos;
			int H2 = (followMouse==true)?center[0]:mnx / 2;
			int h2 = (centerHist==true)?ss / 2:0;
			int shift = -h2 + H2;

			//if (followMouse == true) shift = -h2 + center[0];
			for (int i = 0; i < mny; i++)
			{
				//pos = center[0] + center[1] * mny;
				int x = center[0];
				int y = i;
				pos = x + y * mnx;
				int val = tab[pos];
				double shownVal = ((logScale == true) ? log(1 + (double)(val)) : val);
				int  dataVal = ((double)(shownVal*ss) / maxUsed)+shift;
				shape.poly.append(QPointF(dataVal, i));
				if (val < min)min = val;
				if (val > max) max = val;

				mean += val;
				mean2 += val*val;;
				count++;
				 
			}
		}
		else
		{
			int ss = mny*percent;
			int pos;
			int H2 = (followMouse == true) ? center[1] : mny / 2;
			int h2 = (centerHist == true) ? ss / 2 : 0;
			int shift = -h2 + H2;
			for (int i = 0; i < mnx; i++)
			{
				//pos = center[0] + center[1] * mny;
				int x =i;
				int y = center[1];
				pos = x + y * mnx;
				int val = tab[pos];
				double shownVal = maxUsed-((logScale == true) ? log(1 + (double)(val))  : val);
				int  dataVal = ((double)(shownVal*ss) / maxUsed) + shift;
				shape.poly.append(QPointF(i,dataVal));
				if (val < min)min = val;
				if (val > max) max = val;

				mean += val;
				mean2 += val*val;;
				count++;

			}

		}
		mean /= count;
		mean2 /= count;
		stdDev = sqrt(mean2 - mean*mean);

	

		shape.polyColor = QColor(params[r1ID], params[g1ID], params[b1ID]);;
		shapes->shapeList.append(shape);


		bool showText = params[showTextID];
		if (showText == true)
		{

			MyShape shape2;

			QString txt =
				"min: " + QString::number(min) + "   " + "max: " + QString::number(max) + "   " + "\n" +
				"mean: " + QString::number(mean) + "   " + "stddev: " + QString::number(stdDev);
			double fSize = 15;
			shape2.shapeText = QString(txt);
			shape2.textPos.setX(10);
			shape2.textPos.setY(10);//fSize*1.2);
			shape2.font.setPixelSize(fSize);
			//shape.textAngle=-minPos;

			shape2.textColor.setRgb(255, 0, 0);






			shape2.textToLogFlag = true;
			shapes->shapeList.append(shape2);




		



		}
		for (int i = 0; i<sliceSize; i++)
		{
			usData0[i] = tab[i];
		}

		//updateBrightness(tab, usData0, sliceSize, mainDataMaxValue, inverse, brightnessFilterMode);

		return true;
	}

	return false;
}

//----------------------

//-------------------------------------
LayerMorphologyFilter::LayerMorphologyFilter()
{





	
	leftTh = 0;
	rightTh = 0;
	revth = false;
	samplingTab = NULL;
	samplesCount = 0;
	perc = 0;
	

	sigma = 1.5;
	expMax = 4096;
	
	expMax = 0;
	intensityFactor = 0;
	expTab = NULL;// new double[2 * expMax + 2];
	samplingTab1 = NULL;
	samplingTabIndex = NULL;

	


	filterName = "LayerMorphologyFilter";
	QStringList list;



	
	list.clear(); list << "false" << "true";






	AddParam("showDebug", 0, true, 0, 1, 1, showDebugID, &list);
	list.clear(); list << "threshold" << "probability";
	AddParam("Mode", 0, true, 0, 1, 1, probOrThreshID, &list);
	list.clear(); list << "false" << "true";

	AddParam("lth", 0, true, 0, 10000, 1, threshold1ID);
	AddParam("rth", 10000, true, 0, 10000, 1, threshold2ID);
	AddParam("autoThreshold", 1, true, 0, 1, 1, autoThresholdID, &list);
	AddParam("useGlobalThresholds", 0, true, 0, 1, 1, useGlobalThresholdsID, &list);



	AddParam("thresholdAddLeft", 0, true, 0, 1000, 1, thresholdAdd1ID);
	AddParam("thresholdAddRight", 0, true, 0, 1000, 1, thresholdAdd2ID);

	



	
	AddParam("showStats", 0, true, 0, 1, 1, calculateStatsID, &list);


	


	


	AddParam("histSmplSize", 1, true, 1, 3, 1, histSamplingSizeID);
	AddParam("deltaStdDev", 0, true, 0, 100, 0.001, deltaStdDevID);

	list.clear(); list << "normal" << "existing" << "tmp";
	AddParam("histSimilarityMode", 0, true, 0, 2, 1, histSimilarityModeID, &list);
	list.clear(); list << "false" << "true";

	AddParam("histSimilarityThreshold", 0, true, 0.00, 10, 0.001, histSimilarityThresholdID);
	AddParam("hist2DSimilarityThreshold", 0, true, 0.00, 10, 0.001, histSimilarityThreshold2DID);

	list.clear(); list << "16" << "32" << "64" << "128" << "256";
	AddParam("nrOfHistBins", 5, true, 4, 8, 1, nrOfHistBinsID, &list);
	list.clear(); list << "false" << "true";

	//int minStdDevID, int maxStdDevID;


	list.clear(); list << "square" << "circle";
	AddParam("morphKernelType", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	AddParam("morphKernelSize", 1, true, 0, 10, 1, morphKernelSizeID);
	list.clear(); list << "false" << "true";
	AddParam("morph2D", 0, true, 0, 1, 1, morph2DID);


	AddParam("Erode/Dilate", 1, true, -5, 5, 1, dilateSizeID);
	AddParam("Opening/Closing", 0, true, -5, 5, 1, closingSizeID);
	//	AddParam("closingSize3D", 2, true, 0, 4, 1, closingSizeID);

	

	

	AddParam("propPercentCoarse", 0.8, true, 0.01, 0.9, 0.1, probabilityDensityPercentageCoarseID);
	AddParam("propPercentFine", 0.0, true, 0.0, 0.09999, 0.0001, probabilityDensityPercentageFineID);
	AddParam("sigma", 1.5, true, 0.01, 6, 0.01, sigmaID);
	AddParam("intensityFactor", 0.5, true, 0.001, 1, 0.001, intensityFactorID);



	list.clear(); list << "dilate" << "erode";


	int morphologyTypeID;
	int morphologyKernelTypeID;


	list.clear(); list << "false" << "true";
	AddParam("drawFonts", 1, true, 0, 1, 1, drawFontsID, &list);


	list.clear();



	
	AddParam("infoType", 0, true, 0, 7, 1, infoTypeID, &list);

	

}
//----------------------------------------------
LayerMorphologyFilter::~LayerMorphologyFilter()
{
	if (expTab != NULL) delete[]expTab;
	if (samplingTab1 != NULL)	delete[]samplingTab1;
	if (samplingTabIndex != NULL)	delete[]samplingTabIndex;

	if (samplingTab != NULL)	delete[]samplingTab;


	



}
//------------------------------------
int	LayerMorphologyFilter::ApplyFiltering(RawDataMixerLite* mixer, unsigned short*& usData0, unsigned short*& usData1, double* params, ImageShapes* shapes, ImageWidget* iWidget, unsigned short* tab, bool inverse, unsigned char* src, unsigned char* dest, unsigned short* prevImg, unsigned char* mask, int** intBuffers)
{
	return 1;
}