#include "LayerMaskFilters.h"

LayerMaskFilter::LayerMaskFilter()
{

	//	#pragma omp parallel
	//	nrOfThreads = omp_get_num_threads();
	//	numberOfParams = 0;
	//	//maxValue = 0;
	//	this->filterName = "";
	//	/*shapes = NULL;
	//	visImage = NULL;
	//	this->iWidget = NULL;*/
	//
	//	for (int i = 0; i<PARAMS_NR; i++)
	//	{
	//		defaultParams[i] = 0;
	//		this->paramsNames[i] = "";
	//	}
	//
	//	for (int i = 0; i<PARAMS_NR * 5; i++)
	//	{
	//		showTextStartStop[i] = -1;
	//	}
	//	mdx = 1;
	//	mdy = 1;
	//	mainMask = NULL;
	//	mainDataMaxValue = 0;
	//	viewType = 0;
}
//------------------------------------
LayerMaskFilter::~LayerMaskFilter()
{

}
//----------------------------------
//void		LayerMaskFilter::SetMainMaskData(unsigned char *mask, int x, int y)
//{
//	mainMask = mask;
//	position[0] = x;
//	position[1] = y;
//}
//---------------------------------
//void	LayerMaskFilter::SetiWidget(ImageWidget *s)
//{
//	if (s != NULL)
//	{
//		iWidget = s;
//		visImage = s->GetCurrentImage();
//	}
//};
//------------------------------
//void	LayerMaskFilter::AddParam(QString name, double defVal, bool slider, double min, double max, double step, int &paramID, QStringList *names)
//{
//	if (numberOfParams<PARAMS_NR)
//	{
//		paramsNames[numberOfParams] = name;
//		defaultParams[numberOfParams] = defVal;
//		paramsSliders[numberOfParams] = slider;
//		this->minVals[numberOfParams] = min;
//		this->maxVals[numberOfParams] = max;
//		this->steps[numberOfParams] = step;
//
//
//
//		if (names != NULL)
//		{
//			if ((((int)step) == 1) && (((int)(max - min + 1)) == names->size()))
//			{
//				showTextStartStop[numberOfParams * 2] = showTextList.size();
//				for (int i = 0; i<names->size(); i++)
//				{
//					showTextList << names->at(i);
//					//nrOfShowTexts++
//				}
//				showTextStartStop[numberOfParams * 2 + 1] = showTextList.size() - 1;
//			}
//		}
//
//		paramID = numberOfParams;
//		numberOfParams++;
//	}
//	else
//		paramID = -1;
//}
////-------------------------------------------------------------------
//void	LayerMaskFilter::GetShowTextStartStop(int index, int tab[2])
//{
//
//	if ((index >= 0) && (index <PARAMS_NR))
//	{
//		tab[0] = showTextStartStop[index * 2];
//		tab[1] = showTextStartStop[index * 2 + 1];
//	}
//
//}
////---------------------------------------------------------
//QString		LayerMaskFilter::GetShowText(int pos)
//{
//	if ((pos >= 0) && (pos<showTextList.size()))
//	{
//		return showTextList.at(pos);
//	}
//	return "";
//}
/////-----------------------------------
//bool	LayerMaskFilter::GetSliderParams(int nr, double sParams[3])
//{
//	if ((nr >= 0) && (nr<PARAMS_NR))
//	{
//
//		if (paramsSliders[nr] == true)
//		{
//			sParams[0] = this->minVals[nr];
//			sParams[1] = this->maxVals[nr];
//			sParams[2] = this->steps[nr];
//
//			return true;
//		}
//
//	}
//
//	return false;
//}

//----------------------------------------


LayerMaskCheckedFilter::LayerMaskCheckedFilter()
{
	filterName = "MaskCheckedFilter";


	QStringList list;

	AddParam("sizeX", 50, true, 2, 256, 1, sizeXID);//0.001,0.001,sigma_spatialID);
	AddParam("sizeY", 50, true, 2, 256, 1, sizeYID);//0.001,0.001,sigma_spatialID);
	AddParam("width", 20, true, 0, 256, 1, blendWidthID);
	list.clear(); list << "false" << "true";
	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);
	//this->paramsNames[0]="orientation";
	//this->paramsNames[1]="pos";
	//this->paramsNames[2]="blendWidth";
	//int sizeXID,sizeYID,blendWidthID;
}
//----------------------------------------------
bool	LayerMaskCheckedFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{




	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	int i, j;
	bool revth = params[inverseID];
	unsigned char val1, val2;


	int n[3];
	data->GetN(n);



	int blendWidth = params[blendWidthID];


	int px = params[sizeXID];
	int py = params[sizeYID];


	int a1 = 0;
	int a2 = 1;

	if (revth == false) { val1 = 255; val2 = 0; }
	else { a1 = 255; a2 = -1; val1 = 0; val2 = 255; }

	int nx = n[0];

	bool xx = 0;
	bool yy = 0;
	int  stepx, stepy;
	stepy = 0;
	for (j = 0; j<n[1]; j++)
	{
		stepx = 0;
		xx = true;
		for (i = 0; i<n[0]; i++)
		{
			if (xx == yy)
			{
				mask0[nx*j + i] = val1;
			}
			else
			{
				mask0[nx*j + i] = val2;
			}
			stepx++; if (stepx == px) { stepx = 0; xx = !xx; };
		}
		stepy++; if (stepy == py) { stepy = 0; yy = !yy; };
	}






	return 1;

}


LayerMaskLinearBlendFilter::LayerMaskLinearBlendFilter()
{
	filterName = "MaskLinearBlendFilter";


	QStringList list;
	list.clear(); list << "Vertical" << "Horizontal";
	AddParam("orientation", 0, true, 0, 1, 1, orientationID, &list);
	AddParam("position", 0.5, true, 0, 1, 0.001, positionID);//0.001,0.001,sigma_spatialID);
	AddParam("width", 20, true, 0, 256, 1, blendWidthID);
	list.clear(); list << "false" << "true";
	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);
	//this->paramsNames[0]="orientation";
	//this->paramsNames[1]="pos";
	//this->paramsNames[2]="blendWidth";

}
//----------------------------------------------
bool	LayerMaskLinearBlendFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{




	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	int i, j;
	bool revth = params[inverseID];
	unsigned char val1, val2;


	int n[3];
	data->GetN(n);

	int orientation = params[orientationID];
	int width = (orientation == 0) ? n[0] : n[1];


	int position = width*params[positionID];
	int blendWidth = params[blendWidthID];


	if (position<0) position = 0; else if (position >= width) position = width;



	int p1, p2;
	p1 = position - blendWidth / 2;
	p2 = position + blendWidth / 2;

	int ww = p2 - p1;
	if (ww == 0) ww = 1;
	double blendP = 255.0 / (ww);

	if (p1<0) p1 = 0; else if (p1 >= width) p1 = width;
	if (p2<0) p2 = 0; else if (p2 >= width) p2 = width;

	int a1 = 0;
	int a2 = 1;

	if (revth == false) { val1 = 255; val2 = 0; }
	else { a1 = 255; a2 = -1; val1 = 0; val2 = 255; }

	int nx = n[0];

	if (orientation == 0)
	{
		for (i = 0; i<n[0]; i++)
		{
			if ((i<p1)) { for (j = 0; j<n[1]; j++) { mask0[nx*j + i] = val2; } }
			else if (i>p2) { for (j = 0; j<n[1]; j++) { mask0[nx*j + i] = val1; } }
			else { for (j = 0; j<n[1]; j++) { mask0[nx*j + i] = a1 + a2*(i - p1)*blendP; } }
		}
	}
	else
	{
		for (j = 0; j<n[1]; j++)
		{
			if ((j<p1)) { for (i = 0; i<n[0]; i++) { mask0[nx*j + i] = val2; } }
			else if (j>p2) { for (i = 0; i<n[0]; i++) { mask0[nx*j + i] = val1; } }
			else {
				for (i = 0; i<n[0]; i++) { mask0[nx*j + i] = a1 + a2*(j - p1)*blendP; }
			}
		}
	}

	return 1;

}
//----------------------------------------

LayerMaskEmptyFilter::LayerMaskEmptyFilter()
{
	QStringList list;
	filterName = "MaskEmptyFilter";
	list.clear(); list << "false" << "true";
	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);
}
bool LayerMaskEmptyFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{
	if (masks == NULL) return 0;

	bool inverse = params[inverseID];

	if (inverse == true)
	{
		for (int i = 0; i < mnx*mny; i++)
		{
			masks[i] = 255 - masks[i];
		}
	}

	return 1;
}
//---------------




//--------------------------------

LayerMaskGaussianBlurFilter::LayerMaskGaussianBlurFilter()
{
	filterName = "MaskGaussianFilter";
	//numberOfParams=0;


	QStringList list;
	AddParam("radius", 2, true, 1, 100, 1, radiusID);
	AddParam("stddev", 0.3, true, 0.01, 2, 0.01, stddevID);

	list.clear(); list << "false" << "true";

	AddParam("amount", 1.0, true, 0, 1, 0.01, amountID);
	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);


}
//----------------------------------
bool	LayerMaskGaussianBlurFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{




	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	//unsigned char *mask0 = masks;


	unsigned char *mask0 = new unsigned char[mnx*mny];
	unsigned char *mask1 = new unsigned char[mnx*mny];
	unsigned char *maskTmp;

	unsigned char val1, val2;



	int radius = params[radiusID];
	double stddev = params[stddevID];
	double amount = params[amountID];


	if (radius<1) return 0;
	int					size2 = 2 * radius + 1;
	double				*gaussianKernelTab = new double[size2];
	unsigned short		*intensityTab = new unsigned short[size2];
	GetGaussianKernel(gaussianKernelTab, (double)radius, stddev);


	int pos;

	unsigned char* Q = masks;
	pos = 0;
	int n[2] = { mnx, mny };

	unsigned char *tmpBuff = masks;




	bool inverse = params[inverseID];
	if (inverse == true)
	{
		for (int i = 0; i < mnx*mny; i++)
		{
			masks[i] = 255 - masks[i];
		}
	}

	unsigned char **tabz;
	//#pragma omp parallel private (tabz) num_threads(4)
	{
		unsigned char *datatab = masks;
		tabz = new unsigned char *[nrOfThreads];

		{
			for (int i = 0; i<nrOfThreads; i++)
			{
				tabz[i] = new unsigned char[size2];
			}
		}

#pragma omp parallel for  ordered schedule (guided) 
		for (int j = 0; j < mny; j++)
		{
			int pos = j*mnx;
			int  tid = omp_get_thread_num();
			for (int i = 0; i < mnx; i++)
			{

				GetIntensityTab1D(tmpBuff, n[0], n[1], 1, 0, radius, tabz[tid], i, j, 0);
				mask0[pos++] = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
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
				GetIntensityTab1D(mask0, n[0], n[1], 1, 1, radius, tabz[tid], i, j, 0);
				mask1[pos++] = Apply1DNormFilter(gaussianKernelTab, tabz[tid], size2);
			}
		}
		//#pragma omp critical
		{
			for (int i = 0; i<nrOfThreads; i++)
			{
				delete[]tabz[i]; tabz[i] = NULL;
			}
			delete[]tabz;
		}
	}

	for (int i = 0; i < mnx*mny; i++)
	{
		masks[i] = mask1[i];
	}



	//swap buffers
	//maskTmp = mask0;
	//mask0 = mask1;
	//usData1 = buffTmp;


	delete[]mask0;
	delete[]mask1;

	return 1;

}
//----------------------------






LayerMaskThresholdFilter::LayerMaskThresholdFilter()
{
	filterName = "MaskThresholdFilter";



	QStringList list;

	AddParam("thresholdLeft", 0, true, 0, 100, 0.001, thresholdLeftID);
	AddParam("thresholdRight", 100, true, 0, 100, 0.001, thresholdRightID);
	list.clear(); list << "false" << "true";
	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);


}

//----------------------------------------------
bool	LayerMaskThresholdFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{




	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	int i, j;
	bool revth = params[inverseID];
	unsigned char val1, val2;
	val1 = 255;
	val2 = 0;
	//if (revth == false)
	//{
	//	val1 = 255; val2 = 0;
	//}
	//else
	//{
	//	val1 = 0; val2 = 255;
	//}

	//int leftTh = params[leftThresholdID] * data->GetMaxValue() / 255.0;
	//int rightTh = params[rightThresholdID] * data->GetMaxValue() / 255.0;

	double threshLeftD = params[thresholdLeftID];
	int threshLeft = threshLeftD* mainDataMaxValue*0.01;

	double threshRightD = params[thresholdRightID];
	int threshRight = threshRightD* mainDataMaxValue*0.01;


	if (threshLeft == threshRight) return 0;

	for (i = 0; i<data->GetSliceSize(); i++)
	{
		//if ((tab[i] >= leftTh) && (tab[i] <= rightTh))	

		bool testx = ((tab[i] >= threshLeft) && (tab[i] <= threshRight)); if (revth == true) testx = !testx;

		if (testx == true)
		{
			mask0[i] = val1;
		}
		else {
			mask0[i] = val2;
		};
	}

	return 1;

}
//----------------------------






//----------------------------------------
LayerMaskPointShapeFilter::LayerMaskPointShapeFilter()
{
	filterName = "LayerMaskPointShapeFilter";



	QStringList list;
	AddParam("sizeX", 100, true, 1, 512, 1, sizeXID);
	AddParam("sizeY", 100, true, 1, 512, 1, sizeYID);
	AddParam("angle", 0, true, 0, 360, 1, angleID);

	list.clear(); list << "ellipse" << "square" << "triangle";
	AddParam("type", 0, true, 0, 2, 1, typeID, &list);




	list.clear(); list << "false" << "true";

	AddParam("showBounds", 0, true, 0, 1, 1, showBoundsID, &list);

	AddParam("bRed", 0, true, 0, 255, 1, bRID);
	AddParam("bGreen", 0, true, 0, 255, 1, bGID);
	AddParam("bBlue", 0, true, 0, 255, 1, bBID);


	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);


}
//----------------------------------------------
bool	LayerMaskPointShapeFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{


	//

	//unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	int i, j;
	bool revth = params[inverseID];
	unsigned char val1, val2;
	if (revth == false)
	{
		val1 = 255; val2 = 0;
	}
	else
	{
		val1 = 0; val2 = 255;
	}

	double sizeX = params[sizeXID];
	double sizeY = params[sizeYID];
	double r1 = sizeX / 2.0;
	double r2 = sizeY / 2.0;

	double center[2] = { mnx / 2, mny / 2 };
	if (shapes != NULL)
	{
		center[0] = shapes->clickedLeftPoint.rx();
		center[1] = shapes->clickedLeftPoint.ry();
	}

	shapes->cleanPolygonFlag = true;
	int type = params[typeID];
	bool showBounds = params[showBoundsID];

	double angle = params[angleID];
	double alpha = angle* to_rad;			double ca = cos(alpha);		double sa = sin(alpha);

	if (type == 0) //elipsa
	{

		for (j = 0; j < mny; j++)
		{
			for (i = 0; i < mnx; i++)
			{
				double x, y;
				x = i - center[0];
				y = j - center[1];

				double xx = x*ca - y*sa;				double yy = x*sa + y*ca;

				double xr1 = xx / r1;
				double yr2 = yy / r2;
				double ellipse = xr1*xr1 + yr2*yr2;
				if (ellipse < 1) { *(mask0++) = val1; }
				else {
					*(mask0++) = val2;
				}
			}
			//val=

		}

		if (showBounds == true)
		{
			MyShape shape2;
			double centerE[2] = { center[0], center[1] };

			shape2.ellipses.append(QPointF(centerE[0], centerE[1]));//center
			shape2.ellipses.append(QPointF(r1, r2));//rads
			shape2.ellipses.append(QPointF(-angle, 50));
			shape2.ellipsesColor.setRgb(params[bRID], params[bGID], params[bBID]);
			shapes->shapeList.append(shape2);
		}
	}
	else if (type == 1)//prostokat
	{

		for (j = 0; j < mny; j++)
		{
			for (i = 0; i < mnx; i++)
			{
				double x, y;
				x = i - center[0];
				y = j - center[1];

				double xx = x*ca - y*sa;				double yy = x*sa + y*ca;
				//	double xr1 = x / r1;
				//	double yr2 = y / r2;
				//	double ellipse = xr1*xr1 + yr2*yr2;
				//if (ellipse < 1)	
				if ((xx*xx<r1*r1) && (yy*yy<r2*r2))

				{
					*(mask0++) = val1;
				}
				else {
					*(mask0++) = val2;
				}

			}
			//val=

		}

		if (showBounds == true)
		{
			QPolygonF testPoly;

			double pttab[] = { -r1, -r2, r1, -r2, r1, r2, -r1, r2 };
			alpha = -angle* to_rad;			double ca = cos(alpha);		double sa = sin(alpha);
			for (int i = 0; i < 4; i++)
			{
				double x, y;
				x = pttab[i * 2]; y = pttab[i * 2 + 1];
				double xx = x*ca - y*sa + center[0];				double yy = x*sa + y*ca + center[1];
				testPoly.append(QPointF(xx, yy));

			}

			MyShape s;
			s.representation = 2;
			s.poly = testPoly;
			s.polyColor.setRgb(params[bRID], params[bGID], params[bBID]);
			shapes->shapeList.append(s);
			shapes->shapeList.append(s);
		}
	}
	else if (type == 2)//trojkat
	{
		double dx = r1 / (2.0*r2);
		double ta = 0;
		double firstHit = false;



		double a1, a2, a3, b1, b2, b3;
		double pttab[] = { 0, -r2, -r1, r2, r1, r2 };



		{
			double alpha = angle* to_rad;			double ca = cos(alpha);		double sa = sin(alpha);
			for (int i = 0; i < 3; i++)
			{

				double x, y;
				x = pttab[i * 2];						y = pttab[i * 2 + 1];
				double xx = x*ca - y*sa;	double yy = x*sa + y*ca;
				pttab[i * 2] = x;						pttab[i * 2 + 1] = y;

			}
			//qDebug() << alpha << " " << pttab[0] << " " << pttab[1] << " " << pttab[2] << " " << pttab[3] << " " << pttab[4] << " " << pttab[5];

		}

		double p0[2] = { pttab[0], pttab[1] };
		double p1[2] = { pttab[2], pttab[3] };
		double p2[2] = { pttab[4], pttab[5] };

		double dx1 = p0[0] - p1[0];
		double dx2 = p0[0] - p2[0];
		double dx3 = p1[0] - p2[0];
		if ((dx1 != 0) && (dx2 != 0) && (dx3 != 0))
		{

			double dy1 = (p0[1] - p1[1]);
			double dy2 = (p0[1] - p2[1]);
			double dy3 = (p1[1] - p2[1]);

			a1 = dy1 / dx1;
			a2 = dy2 / dx2;
			a3 = dy3 / dx3;

			b1 = p0[1] - a1*p0[0];
			b2 = p0[1] - a2*p0[0];
			b3 = p1[1] - a3*p0[0];

			//double pos1  = ((Bx - Ax) * (Y - Ay) - (By - Ay) * (X - Ax))



			for (j = 0; j < mny; j++)
			{
				for (i = 0; i < mnx; i++)
				{
					double x, y;
					x = i - center[0];
					y = j - center[1];

					double xx = x*ca - y*sa;				double yy = x*sa + y*ca;
					//	double xr1 = x / r1;
					//	double yr2 = y / r2;
					//	double ellipse = xr1*xr1 + yr2*yr2;
					//if (ellipse < 1)	
					//	if ((xx*xx < r1*r1) && (yy*yy < r2*r2))

					{
						/*if (firstHit == false) firstHit = true;*/

						double pos1 = dx1 * (yy - p1[1]) - dy1 * (xx - p1[0]);
						double pos2 = dx2 * (yy - p2[1]) - dy2 * (xx - p2[0]);
						double pos3 = dx3 * (yy - p2[1]) - dy3 * (xx - p2[0]);

						if ((pos1>0) && (pos2 < 0) && (pos3>0))
							*(mask0++) = val1;
						else
							*(mask0++) = val2;
					}
					/*else
					{
					*(mask0++) = val2;
					}*/

				}
				//val=
				/*	if (firstHit == true)
				ta += dx;*/

			}
		}






		if (showBounds == true)
		{
			QPolygonF testPoly;

			double pttab[] = { 0, -r2, -r1, r2, r1, r2 };
			alpha = -angle* to_rad;			double ca = cos(alpha);		double sa = sin(alpha);
			for (int i = 0; i < 3; i++)
			{
				double x, y;
				x = pttab[i * 2]; y = pttab[i * 2 + 1];
				double xx = x*ca - y*sa + center[0];				double yy = x*sa + y*ca + center[1];
				testPoly.append(QPointF(xx, yy));

			}

			MyShape s;
			s.representation = 2;
			s.poly = testPoly;
			s.polyColor.setRgb(params[bRID], params[bGID], params[bBID]);
			shapes->shapeList.append(s);
			shapes->shapeList.append(s);
		}
	}


	int n[2] = { mnx, mny };
	//data->GetN(n);

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

	int countx = 0;
	for (int i = 0; i < n[0] * n[1]; i++)
	{
		shapes->mask[i] = (masks[i] >0) ? 1 : 0;

		if (masks[i] == val1) countx++;
	}
	//qDebug() << "MaskPixels: " << countx;
	return 1;

}
//----------------------------


//----------------------------------------
LayerMaskThicknessAnalysisFilter::LayerMaskThicknessAnalysisFilter()
{

	samplesCount1 = 0;


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
	//sigma = 1.5;
	///*expMax = 256;
	//prevMaxValue = 0; 
	//intensityFactor = 1.0 / (double)(expMax*expMax);*/
	////intensityFactor2=intensityFactor*intensityFactor;

	//expMax = 0;
	//intensityFactor = 0;
	//expTab = NULL;// new double[2 * expMax + 2];
	////GetExpTab(expTab, expMax, sigma, intensityFactor);

	filterName = "LayerMaskThicknessAnalysisFilter";



	QStringList list;
	list.clear(); list << "false" << "true";


	AddParam("samplingThickness", 1, SLIDER_MODE, 1, 30, 1, samplingThicknessID);
	AddParam("ClosingProbMap", 0, SLIDER_MODE, 0, 20, 1, closingBinaryProbMapID);

	AddParam("leftInputCutt", 0, SLIDER_MODE, 0.0, 100, 0.001, leftCutID);
	AddParam("rightInputCutt", 100, SLIDER_MODE, 0.0, 100, 0.001, rightCutID);

	AddParam("autoThreshold", 1, SLIDER_MODE, 0, 1, 1, autoThresholdID, &list);

	AddParam("median", 1, SLIDER_MODE, 0, 1, 1, medianID, &list);
	AddParam("medianFactor", 1, SLIDER_MODE, 0, 1, 0.001, medianFactorID);
	AddParam("medianKernel", 2, SLIDER_MODE, 1, 20, 1, medianKernelID);
	AddParam("removeCollisions", 1, SLIDER_MODE, 0, 1, 1, removeCollisionsID, &list);
	AddParam("removeAllCollisions", 1, SLIDER_MODE, 0, 1, 1, removeAllCollisionsID, &list);
	AddParam("improveCollision", 1, SLIDER_MODE, 0, 1, 1, improveCollisionID, &list);

	AddParam("useProbMap", 1, true, 0, 1, 1, useProbabilityDensityMapID, &list);

	AddParam("IntensityStats", 1, true, 0, 1, 1, calculateIntensityStatsID, &list);
	AddParam("leftIntensTh", 33, SLIDER_MODE, 0.0, 100, 0.001, leftIntensityStatsThreshID);
	AddParam("rightIntensTh", 66, SLIDER_MODE, 0.0, 100, 0.001, rightIntensityStatsThreshID);



	//AddParam("propPercent", 0.85, true, 0, 0.9999, 0.001, probabilityDensityPercentageID);
	AddParam("propPercentCoarse", 0.8, SLIDER_MODE, 0.0, 0.9, 0.1, probabilityDensityPercentageCoarseID);
	AddParam("propPercentFine", 0.00001, SLIDER_MODE, 0.00001, 0.09999, 0.00001, probabilityDensityPercentageFineID);
	AddParam("sigma", 1.5, SLIDER_MODE, 0.01, 6, 0.01, sigmaID);
	AddParam("intensityFactor", 0.5, SLIDER_MODE, 0.001, 1, 0.001, intensityFactorID);

	unsigned short *samplingTab1;
	unsigned short *samplingTabIndex;
	int samplesCount1;

	AddParam("maxDistance", 0, SLIDER_MODE, 0, 256, 0.1, maxDistanceID, &list);
	AddParam("maxDistanceRealDist", 0, SLIDER_MODE, 0, 1, 1, maxDistanceRealDistModeID, &list);


	AddParam("useBigMask", 1, SLIDER_MODE, 0, 1, 1, useBigMaskID, &list);

	list.clear(); list << "clear" << "add";
	AddParam("bigMaskMode", 0, SLIDER_MODE, 0, 1, 1, bigMaskModeID, &list);
	list.clear(); list << "false" << "true";

	AddParam("inverse", 0, SLIDER_MODE, 0, 1, 1, inverseID, &list);
	AddParam("lefthTh", 0, SLIDER_MODE, 0, 255, 0.01, leftThresholdID);
	AddParam("rightthTh", 255, SLIDER_MODE, 0, 255, 0.01, rightThresholdID);


	AddParam("firstSmoothingIters", 5, SLIDER_MODE, 0, 200, 1, firstSmoothingIterID);
	AddParam("secondSmoothingIters", 20, SLIDER_MODE, 0, 200, 1, secondSmoothingID);

	AddParam("MaxDx", 5, SLIDER_MODE, 0, 50, 0.01, smoothingDxID);
	AddParam("MaxDy", 5, SLIDER_MODE, 0, 50, 0.01, smoothingDyID);

	AddParam("factor", 1, SLIDER_MODE, 1, 50, 0.01, factorID);

	AddParam("updateCenter", 0, SLIDER_MODE, 0, 1, 1, updateCenterID, &list);
	AddParam("updateCenterFactor", 0.1, SLIDER_MODE, 0.001, 1, 0.001, updateCenterFactorID);
	AddParam("thicknessSampling", 0, true, 1, 40, 1, samplingID);


	AddParam("showOrigLine", 1, SLIDER_MODE, 0, 1, 1, showOrigLineID, &list);
	AddParam("showSmoothed1", 1, SLIDER_MODE, 0, 1, 1, showSmoothed1ID, &list);
	AddParam("showSmoothed2", 0, SLIDER_MODE, 0, 1, 1, showSmoothed2ID, &list);
	AddParam("showLines", 1, SLIDER_MODE, 0, 1, 1, showLinesID, &list);
	AddParam("fontSize", 15, SLIDER_MODE, 4, 100, 1, fontSizeID);


	AddParam("showFront1", 1, SLIDER_MODE, 0, 1, 1, showFront1ID, &list);
	AddParam("showFront2", 2, SLIDER_MODE, 0, 1, 1, showFront2ID, &list);



	AddParam("CleanShapes", 1, SLIDER_MODE, 0, 1, 1, cleanShapesID, &list);


	AddParam("textXPos", 0, SLIDER_MODE, 0, 1, 0.001, textXPosID);
	AddParam("textYPos", 0, SLIDER_MODE, 0, 1, 0.001, textYPosID);

	list.clear();
	list << "None" << "Min" << "Max" << "Average" << "StdDev" << "Skewness" << "Kurtosis";

	AddParam("infoType", 0, SLIDER_MODE, 0, 6, 1, infoTypeID, &list);


	AddParam("lineUpperColor", 0, SLIDER_MODE, 0, 19, 1, lineUpperColorID);
	AddParam("lineCenterColor", 0, SLIDER_MODE, 0, 19, 1, lineCenterColorID);
	AddParam("lineLowerColorID", 0, SLIDER_MODE, 0, 19, 1, lineLowerColorID);
	AddParam("lineSegmentsColor", 0, SLIDER_MODE, 0, 19, 1, lineSegmentsColorID);
	AddParam("textColor", 0, SLIDER_MODE, 0, 19, 1, textColorID);
	AddParam("lineSmoothed1Color", 0, SLIDER_MODE, 0, 19, 1, lineSmoothed1ColorID);
	AddParam("lineSmoothed2Color", 0, SLIDER_MODE, 0, 19, 1, lineSmoothed2ColorID);
	AddParam("fillColor", 0, SLIDER_MODE, 0, 19, 1, fillColorID);
	AddParam("fillOpacity", 1.0, SLIDER_MODE, 0.0, 1.0, 0.01, fillOpacityID);



	list.clear(); list << "false" << "true";

	AddParam("testMode", 0, SLIDER_MODE, 0, 1, 1, testID, &list);
	//int lineUpperColorID;
	//int lineCenterColorID;
	//int lineSmoothedColorID;
	//int lineLowerColorID;
	//int linesegmentsColorID;
	//int textColorID;
	//int fillColorID;
	//int fillOpacityID;

}
LayerMaskThicknessAnalysisFilter::~LayerMaskThicknessAnalysisFilter()
{

	if (expTab != NULL) delete[]expTab;
	if (samplingTab1 != NULL)	delete[]samplingTab1;
	if (samplingTabIndex != NULL)	delete[]samplingTabIndex;

}
//----------------------------------------------
bool	LayerMaskThicknessAnalysisFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{
	bool test = params[testID];

	if ((sliceNr<0) || (sliceNr >= data->GetNz()) || (shapes == NULL)) return 0;

	//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	//int i, j, k;
	bool revth = params[inverseID];
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

	double p0[2], p1[2], p2[2], p3[2];

	//if (expMax != mainDataMaxValue)
	//{

	//	if (expTab != NULL)delete []expTab;
	//	expMax = mainDataMaxValue;
	//	expTab=new double[expMax+1];
	//	intensityFactor = 1.0 / (double)(expMax*expMax);
	//	GetExpTab(expTab, expMax, sigma, intensityFactor);

	//}
	//

	//if (data->GetMaxValue() >= expMax)
	//{
	//	expMax = data->GetMaxValue() + 1;
	//	intensityFactor = 1.0 / (double)(expMax*expMax);
	//	if (expTab != NULL) delete[]expTab;
	//	expTab = new double[2 * expMax + 2];
	//	GetExpTab(expTab, expMax, sigma, intensityFactor);
	//	//narazie nie dodaje pomniejszania bo nie wiem czy sie przyda
	//}
	double mSigma = params[sigmaID];
	unsigned short maxValue = (mainDataMaxValue > 0) ? mainDataMaxValue : data->GetMaxValue();

	bool sigmaChg = fabs(mSigma - sigma) < 1e-5 ? false : true;

	bool intensChg = fabs(intensityFactor - params[intensityFactorID])< 1e-5 ? false : true;
	intensityFactor = params[intensityFactorID];

	if ((expMax != maxValue + 1) || (sigmaChg == true) || (intensChg == true))
	{

		sigma = mSigma;
		if (expTab != NULL)delete[]expTab;
		expMax = maxValue + 1;
		expTab = new double[expMax * 2 + 1];

		if (samplingTab1 != NULL)delete[]samplingTab1;
		samplingTab1 = new unsigned short[expMax];
		if (samplingTabIndex != NULL)delete[]samplingTabIndex;
		samplingTabIndex = new unsigned short[expMax];


		//intensityFactor = 1.0 / (double)(expMax*expMax);
		GetExpTab(expTab, expMax, sigma, intensityFactor);

	}


	memset(samplingTab1, 0, expMax * sizeof(unsigned short));


	//unsigned short maxValue = (mainDataMaxValue > 0) ? mainDataMaxValue : data->GetMaxValue();
	int leftTh = params[leftThresholdID] * maxValue/ 255.0;
	int rightTh = params[rightThresholdID] * maxValue / 255.0;








	int firstSmoothingIter = params[firstSmoothingIterID];
	int secondSmoothing = params[secondSmoothingID];
	double smoothingDx = params[smoothingDxID];
	double smoothingDy = params[smoothingDyID];
	double factor = params[factorID];
	bool updateCenter = params[updateCenterID];
	double samplingDest = params[samplingID];
	int infoType = params[infoTypeID];
	int fontSize = params[fontSizeID];

	bool showOrigLine = params[showOrigLineID];
	bool showSmoothed1 = params[showSmoothed1ID];
	bool showSmoothed2 = params[showSmoothed2ID];
	bool showLines = params[showLinesID];

	bool autoThreshold = params[autoThresholdID];
	double maxDistance = params[maxDistanceID];// *mdx * 100;
	if (((bool)(params[maxDistanceRealDistModeID])) == true) maxDistance *= mdx * 100;

	double updateCenterFactor = params[updateCenterFactorID];

	bool cleanShapes = params[cleanShapesID];

	if (shapes != NULL) shapes->cleanPolygonFlag = cleanShapes;

	int nrPolygons = 0;

	nrPolygons = shapes->polygonList.count();

	QList<QPolygonF> *slist = &shapes->polygonList;
	int sliceSize = data->GetSliceSize();
	int nx = data->GetNx();
	double nx1d = 1.0 / nx;
	int ny = data->GetNy();
	int max = MAX(c1, c2);
	int samplesCount = 0;


	unsigned short val;
	//zbieram wszystkie jasnosci w tabelke i wybieram min max
	//podmieniam treszholdy
	unsigned short minThresh = 65535;
	unsigned short maxThresh = 0;
	for (int j = 0; j<nrPolygons; j++)
	{
		MyShape s;
		s.poly = slist->at(j);



		// spr ilosc pktow
		int counter = 0;
		QPolygonF testPoly;
		for (int i = 0; i<s.poly.count(); i++)
		{
			int pos = (int)(s.poly.at(i).y())*nx + (int)(s.poly.at(i).x());
			if ((pos >= 0) && (pos<sliceSize))
			{
				samplesCount++;
				val = tab[pos];
				if (val<minThresh) minThresh = val;
				if (val>maxThresh) maxThresh = val;
			}
		}

	}

	if (autoThreshold == true)
	{

		leftTh = minThresh;
		rightTh = maxThresh;
	}

	bool useProb = params[useProbabilityDensityMapID];

	if (useProb == true)
	{
		//use sampling points
		//unsigned short *samplingTab = new unsigned short[expMax];
		//memset(samplingTab, 0, expMax*sizeof(unsigned short));
		int  samplingThickness = params[samplingThicknessID] * 2 + 1;


		// robie rysunek o wielkosci bounds +sampling thickness po obu stronach 
		// rysuje na nim kreski o grubosci sampling thickness+1
		// sampluje punkty

		int bbounds[4] = { INT_MAX,-INT_MAX,INT_MAX,-INT_MAX };
		for (int j = 0; j < nrPolygons; j++)
		{
			MyShape s;
			s.poly = slist->at(j);

			for (int i = 0; i < s.poly.count(); i++)
			{
				int x = (int)(s.poly.at(i).x());
				int y = (int)(s.poly.at(i).y());
				if (x < bbounds[0]) bbounds[0] = x;
				if (x > bbounds[1]) bbounds[1] = x;
				if (y < bbounds[2]) bbounds[2] = y;
				if (y > bbounds[3]) bbounds[3] = y;
			}

		}

		// zebrałem bounds teraz nowe wymiary i obrazki

		int nx1 = bbounds[1] - bbounds[0];
		int ny1 = bbounds[3] - bbounds[2];

		int nx2 = nx1 + samplingThickness * 2;
		int ny2 = ny1 + samplingThickness * 2;

		//robie nowy rysunek i poprawiam polygony

		QImage testImg(nx2, ny2, QImage::Format_RGB32);
		unsigned int *tablinesImg = (unsigned int *)(testImg.bits());
		memset(tablinesImg, 0, nx2*ny2 * sizeof(int));
		QPainter p2(&testImg);
		QPen pen2;
		pen2.setWidth(samplingThickness);
		pen2.setColor(Qt::white);
		p2.setPen(pen2);

		int minx = bbounds[0];
		int miny = bbounds[2];

		int xmst = minx - samplingThickness;
		int ymst = miny - samplingThickness;
		for (int j = 0; j < nrPolygons; j++)
		{
			MyShape s;
			s.poly = slist->at(j);
			QPolygon newPoly;
			for (int i = 0; i < s.poly.count(); i++)
			{
				int x = (int)(s.poly.at(i).x());
				int y = (int)(s.poly.at(i).y());
				//	x += -minx + samplingThickness;
				//	y += -miny + samplingThickness;
				x -= xmst;
				y -= ymst;
				newPoly.append(QPoint(x, y));

			}
			p2.drawPolyline(newPoly);

		}
		if (test == true) testImg.save("d:\\thickness\\aa.png");


		//robie sampling
		//memset(tablinesImg, 0, nx2*ny2 * sizeof(int));
		int pos = 0;
		int posx = 0;
		int firstCount = samplesCount;
		samplesCount = 0;
		for (int j = 0; j < ny2; j++)
		{
			for (int i = 0; i < nx2; i++)
			{
				if (tablinesImg[pos] == 0xffffffff) //pixel zapalony
				{
					int xx = i + xmst;
					int yy = j + ymst;

					int pos2 = yy*nx + xx;
					if ((pos2 >= 0) && (pos2 < sliceSize))
					{
						int val = tab[pos2];
						samplingTab1[val]++;
						samplesCount++;

						if (test == true)  tablinesImg[pos] = (unsigned int)(QColor(val, val, val).rgba());
					}
				}
				pos++;
			}
		}
		if (test == true) testImg.save("d:\\thickness\\aa-smaples.png");


		qDebug() << samplesCount;

		//}
		//zebralem probki

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

		//teraz prob map!!!
		double *intensityProbabilityTab = new double[expMax];
		double min, max;
		//GetIntensityProbabilityTab(intensityProbabilityTab, samplesCount, min, max, sigma, expTab, samplingTab, expMax, expMax, samplesCount);//ta	sie	przyda
		//double factor = (max - min != 0) ? 1.0 / (max - min) : 1;


		//double perc = params[probabilityDensityPercentageID]; //0-100%
		GetIntensityProbabilityTab2(intensityProbabilityTab, expMax, min, max, sigma, expTab, samplingTab1, samplingTabIndex, maxPosNew, samplesCount);//ta	sie	przyda
		double factor1 = (max != 0) ? 1.0 / (max) : 1;
		double percCoarse = params[probabilityDensityPercentageCoarseID]; //0-100%
		double percFine = params[probabilityDensityPercentageFineID]; //0-100%

		double perc = percCoarse + percFine;


		for (int i = 0; i<data->GetSliceSize(); i++)
		{

			val = tab[i];
			double testV = (intensityProbabilityTab[val])*factor1;
			bool test = testV>perc;

			if (test) { mask0[i] = val1; c1++; }
			else { mask0[i] = val2; c2++; };
		}

		if (test == true)
		{

			QImage testImg2Col(nx, ny, QImage::Format_RGB32);
			QImage testImg2Prob(nx, ny, QImage::Format_RGB32);
			unsigned char *tabIm = (unsigned char *)(testImg2Prob.bits());
			unsigned char *tabCol = (unsigned char *)(testImg2Col.bits());
			int pos = 0;
			double scal = (maxValue>0) ? 255.0 / maxValue : 1;
			for (int j = 0; j < ny; j++)
			{
				for (int i = 0; i < nx; i++)
				{
					unsigned short val = tab[pos] * scal;
					tabCol[pos * 4] = val;
					tabCol[pos * 4 + 1] = val;
					tabCol[pos * 4 + 2] = val;
					tabCol[pos * 4 + 3] = 255;

					unsigned short newVal = (intensityProbabilityTab[val])*factor1 * 255;
					tabIm[pos * 4] = newVal;
					tabIm[pos * 4 + 1] = newVal;
					tabIm[pos * 4 + 2] = newVal;
					tabIm[pos * 4 + 3] = 255;
					pos++;
				}
			}

			testImg2Col.save("d:\\thickness\\colframe.png");
			testImg2Prob.save("d:\\thickness\\probremapframe.png");


			unsigned short *remapTab = new unsigned short[maxValue];
			for (int i = 0; i < maxValue; i++)
			{
				remapTab[i] = (intensityProbabilityTab[i])*factor1 * maxValue;
			}


			int arrNx = 1024;
			int arrNy = 768;
			QImage arrowImg(arrNx, arrNy, QImage::Format_RGB32);

			arrowImg.fill(Qt::white);
			QPainter Painter2x(&arrowImg);
			Painter2x.setBrush(Qt::black);
			Painter2x.setPen(1);

			double scx = (maxValue>0) ? (double)arrNx / maxValue : 1;
			for (int i = 0; i < maxValue; i++)
			{
				Painter2x.drawLine(QPoint(i*scx, 0), QPoint(remapTab[i] * scx, arrNy));
			}
			delete[]remapTab;
			arrowImg.save("d:\\thickness\\mapping-lines.png");

			QImage testImg2(nx, ny, QImage::Format_RGB32);
			unsigned int *tabImx = (unsigned int *)(testImg2.bits());
			memset(tabIm, 0, nx*ny * sizeof(int));
			pos = 0;



			for (int j = 0; j < ny; j++)
			{
				for (int i = 0; i < nx; i++)
				{
					if (mask0[pos] == val1) //pixel zapalony
					{



						tabImx[pos] = 0xffffffff;//(unsigned int)(QColor(val, val, val).rgba());

					}
					pos++;
				}
			}
			testImg2.save("d:\\thickness\\aa-prob-01.png");
		}

		int closingBinaryProbMap = params[closingBinaryProbMapID];
		if (closingBinaryProbMap > 0)
		{
			//zrobie dylatacje mask0 zeby zatkac nieciaglosci w mapie spowodowane przez pojedyncze pixele


			bool is2d = true;
			int kernelType = 1;
			SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 2, closingBinaryProbMap, kernelType, NULL, NULL, NULL, NULL, is2d, 2); //z criterion na danych
			SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 0, closingBinaryProbMap, kernelType, NULL, NULL, NULL, NULL, is2d, 0); //z criterion na danych



			if (test == true)
			{
				QImage testImg2(nx, ny, QImage::Format_RGB32);
				unsigned int *tabIm = (unsigned int *)(testImg2.bits());
				memset(tabIm, 0, nx*ny * sizeof(int));
				int pos = 0;



				for (int j = 0; j < ny; j++)
				{
					for (int i = 0; i < nx; i++)
					{
						if (mask0[pos] == val1) //pixel zapalony
						{



							tabIm[pos] = 0xffffffff;//(unsigned int)(QColor(val, val, val).rgba());

						}
						pos++;
					}
				}
				testImg2.save("d:\\thickness\\aa-prob-afterDil-01.png");
			}

		}



		delete[]intensityProbabilityTab;
		//	delete[]samplingTab;

	}
	else
		for (int i = 0; i<data->GetSliceSize(); i++)
		{
			if ((tab[i] >= leftTh) && (tab[i] <= rightTh)) { mask0[i] = val1; c1++; }
			else { mask0[i] = val2; c2++; };
		}

	bool useBigMask = params[useBigMaskID];
	int bigMaskMode = params[bigMaskModeID];

	if ((useBigMask == true) && (mainMask != NULL))
	{
		for (int i = 0; i<data->GetSliceSize(); i++)
		{
			if (mainMask[i] == 1) mask0[i] = bigMaskMode;
		}
	}



	//if (shapes->polygon0.count()==0) return 1;

	//if (max==sliceSize)
	{
		for (int i = 0; i<data->GetSliceSize(); i++)
		{
			if (mask0[i] == 2) mask0[i] = 255;
		}
		//return 1;
	}


	// 1. sprawdzam jak wyglada shape.polygon0
	// jesli ma ilosc pktow >1
	// sprawdzam czy punkty (chociaz 2) leza w masce aktualnej
	// jesli tak narysuje
	// zrobie smoothing
	// strzele prostopadlymi kreskami - znajde punkty przeciecia - gora dol
	// przeniose pkty na srodek
	// zrobie smoothing - byc moze jeszcze raz strzelanie
	// narysuje krechy
	// zrobie statysytki



	for (int ix = 0; ix<nrPolygons; ix++)
	{

		MyShape s;
		s.poly = slist->at(ix);
		s.polyColor.setRgb(255, 0, 255);
		//	shapes->shapeList.append(s);


		// spr ilosc pktow
		int counter = 0;
		QPolygonF testPoly;
		for (int i = 0; i<s.poly.count(); i++)
		{
			int pos = (int)(s.poly.at(i).y())*nx + (int)(s.poly.at(i).x());
			if ((pos >= 0) && (pos<sliceSize))
			{
				if (mask0[pos] == 255)
				{
					counter++;
					testPoly.append(s.poly.at(i));
				}
			}
		}
		if (showOrigLine)
		{
			if (counter>1)
			{

				MyShape s;
				s.poly = testPoly;
				QColor c((Qt::GlobalColor)((int)(params[lineCenterColorID])));
				s.polyColor = c;// setRgb(0, 255, 0);
				shapes->shapeList.append(s);

			}
		}

		//odciecia od lewej i od prawej zrobie - przesampluje sobie punkt po punkcie całą krzywą i odetnę co nie potrzebne


		// DOPISAć!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


		//*****************************************



		QPolygonF polySmooth = SmoothPolyLine(&testPoly, firstSmoothingIter, factor, smoothingDx, smoothingDy);//wygladzam
		if (showSmoothed1)
		{
			if (polySmooth.count()>1)
			{

				MyShape s;
				s.poly = polySmooth;
				QColor c((Qt::GlobalColor)((int)(params[lineSmoothed1ColorID])));
				s.polyColor = c;// setRgb(0, 255, 0);
								//s.polyColor.setRgb(255, 0, 0);
				shapes->shapeList.append(s);

			}
		}


		//strzelanie gora dol i szukanie srodka odcinka!!!

		QPolygonF centerized;
		if (updateCenter == true)
			centerized = CenterPolyline(&polySmooth, nx, ny, mask0, 0, updateCenterFactor);//poprawiam srodek
		else
			centerized = polySmooth;


		polySmooth = SmoothPolyLine(&centerized, secondSmoothing, factor, smoothingDx, smoothingDy); //wygladzam wysrodkowane
																									 /*if (showSmoothed2==true)
																									 {
																									 if  (polySmooth.count()>1)
																									 {

																									 MyShape s;
																									 s.poly=polySmooth;
																									 s.polyColor.setRgb(0,0,255);
																									 shapes->shapeList.append(s);

																									 }
																									 }*/

																									 //if (updateCenter==true)
																									 //	centerized=CenterPolyline(&polySmooth,nx,ny,mask0,0);//poprawiam srodek
																									 //else
																									 //	centerized=polySmooth;

																									 //QPolygonF *polygon,int nx,int ny,unsigned char *mask,unsigned char badVal,double factor,bool genLines,int dest
																									 //polySmooth = SmoothPolyLine(&centerized, secondSmoothing, factor, smoothingDx, smoothingDy); //wygladzam wysrodkowane
		if (showSmoothed2 == true)
		{
			if (polySmooth.count()>1)
			{

				MyShape s;
				s.poly = polySmooth;
				QColor c((Qt::GlobalColor)((int)(params[lineSmoothed2ColorID])));
				s.polyColor = c;// setRgb(0, 255, 0);
								//s.polyColor.setRgb(0, 128, 255);
				shapes->shapeList.append(s);

			}
		}

		double lenCenter = 0;

		for (int j = 0; j<polySmooth.count() - 1; j++)
		{
			p0[0] = polySmooth.at(j).x()*mdx;
			p0[1] = polySmooth.at(j).y()*mdy;

			p1[0] = polySmooth.at(j + 1).x()*mdx;
			p1[1] = polySmooth.at(j + 1).y()*mdy;
			lenCenter += GetDistance2D(p0, p1);
		}


		bool medianLines = params[medianID];
		double medianFactor = params[medianFactorID];
		int medianKernel = params[medianKernelID];



		//ucinanie kresek
		int nrOfSegments = polySmooth.count(); //tyle jest kresek 

		double leftCutt = params[leftCutID] * (nrOfSegments)*0.01;
		double rightCutt = params[rightCutID] * (nrOfSegments)*0.01;
		if (leftCutt < rightCutt)
		{
			for (int i = nrOfSegments - 1; i >= 0; i--)
			{
				if ((i > rightCutt) || (i < leftCutt))
				{
					//	polySmooth.remove(i * 2+1);
					polySmooth.remove(i);// *2);
				}
			}

		}




		QPolygonF lines = CenterPolyline(&polySmooth, nx, ny, mask0, 0, 1, true, samplingDest, medianLines, medianFactor, medianKernel, maxDistance);//poprawiam srodek



		bool collisionCheck = params[removeCollisionsID];
		int delPts = 0;
		if (collisionCheck == true)
		{
			//wywale koliduj�ce kreski i skr�c� tabx i nrPts


			delPts = RemoveCollisionsFromLineSet(&lines, params[improveCollisionID], params[removeAllCollisionsID]);

		}
		if (showLines == true)
		{
			if (lines.count()>1)
			{

				MyShape s;
				s.name = "areaPolys";
				s.poly = lines;
				s.representation = 3;
				QColor c((Qt::GlobalColor)((int)(params[lineSegmentsColorID])));
				s.polyColor = c;// setRgb(0, 255, 0);
								//	s.polyColor.setRgb(170, 0, 255);
				shapes->shapeList.append(s);


			}
		}
		//-----------------------------------------------------
		//obliczenia:
		//------------------------------------

		double mmin = INT_MAX;
		double mmax = -INT_MAX;

		//double dx=1.0;
		//double dy=1.0;
		int nrPts = lines.count() / 2;

		double len;
		double stddev;


		double 	mean = 0;
		double 	mean2 = 0;

		/*	for (i=0;i<sliceSize;i++)
		{
		val=src[i];
		mean+=val;
		mean2+=val*val;
		}
		mean/=(double)sliceSize;
		mean2/=(double)sliceSize;

		stddev=mean2-mean*mean;*/
		double ddx = mdx*0.01;
		double ddy = mdy*0.01;

		double minX = nx;
		double maxX = -nx;
		double minY = ny;
		double maxY = -ny;
		double dax, day;

		double *tabx = new double[nrPts];
		for (int i = 0; i<nrPts; i++)
		{
			p0[0] = lines.at(i * 2).x()*ddx;
			p0[1] = lines.at(i * 2).y()*ddy;

			p1[0] = lines.at(i * 2 + 1).x()*ddx;
			p1[1] = lines.at(i * 2 + 1).y()*ddy;

			dax = p1[0] - p0[0];
			day = p1[1] - p0[1];
			len = sqrt(dax*dax + day*day);
			tabx[i] = len;
			if (len<mmin) mmin = len;
			if (len>mmax) mmax = len;

			mean += len;
			mean2 += len*len;

			if (p0[0]<minX) minX = p0[0];						if (p0[0]>maxX) maxX = p0[0];
			if (p0[1]<minY) minY = p0[1];						if (p0[1]>maxY) maxY = p0[1];
			if (p1[0]<minX) minX = p1[0];						if (p0[0]>maxX) maxX = p1[0];
			if (p1[1]<minY) minY = p1[1];						if (p0[1]>maxY) maxY = p1[1];

		}


		mean /= (double)nrPts;
		mean2 /= (double)nrPts;

		mean *= 100.0;
		mean2 *= 100 * 100.0;
		mmin *= 100.0;
		mmax *= 100.0;
		stddev = sqrt(mean2 - mean*mean);

		double skewness = 0;
		double kurtosis = 0;
		//if ((infoType==5)||(infoType==6))
		{

			CalculateSkewNessAndKurtosis(tabx, nrPts, skewness, kurtosis);
		}
		delete[]tabx;
		//int infoType=params[infoTypeID];

		double l1Len = 0;
		double l2Len = 0;
		double area = 0;
		QString text;
		bool intStats = (bool)params[calculateIntensityStatsID];
		if (lines.count()>2)
		{


			QPolygonF l1;

			for (int j = 0; j<lines.count() / 2; j++)
			{
				l1.append(lines.at(j * 2));
			}
			int v = lines.count() / 2 - 1;
			for (int j = 0; j<lines.count() / 2; j++)
			{
				l1.append(lines.at((v - j) * 2 + 1));
			}


			//polygon l1 zawiera obrys segmentacji

			if (intStats == true)
			{

				int leftT = params[leftIntensityStatsThreshID] * maxValue*0.01;
				int rightT = params[rightIntensityStatsThreshID] * maxValue*0.01;
				if (leftT < rightT)
				{
					//zrobie rysunek z maską wysegmentowaną
					//zrobie histogram dla maski
					//zrobie stats
					QImage img(data->GetNx(), data->GetNy(), QImage::Format_RGB32);
					img.fill(Qt::black);
					QPainter Painter2(&img);
					Painter2.setBrush(Qt::white);
					Painter2.setPen(1);
					Painter2.drawPolygon(l1);

					unsigned int *imgTab = (unsigned int *)(img.bits());

					int *histTab = new int[maxValue + 1];
					memset(histTab, 0, (maxValue + 1)*(sizeof(int)));
					int minx = INT_MAX;
					int maxx = -INT_MAX;
					int med = 0;
					int count = 0;
					for (int i = 0; i < data->GetSliceSize(); i++)
					{
						if (imgTab[i] == 0xffffffff)
						{
							unsigned short val = tab[i];
							histTab[val]++;
							if (val < minx) minx = val;
							if (val > maxx)maxx = val;
							count++;
						}
					}
					int sum = 0;
					int c2 = count / 2;
					//sum += histTab[0];
					int sum0 = 0;
					int sum1 = 0;
					int sum2 = 0;
					bool stopcountmed = false;
					for (int i = 0; i < maxValue + 1; i++)
					{
						sum += histTab[i];
						if ((sum > c2) && (i>0) && (stopcountmed == false))
						{
							med = i - 1;
							stopcountmed = true;
							//break;

						}
						if (i < leftT)
						{
							sum0 += histTab[i];
						}
						else if ((i >= leftT) && (i < rightT))
						{
							sum1 += histTab[i];
						}
						else
						{
							sum2 += histTab[i];
						}
					}

					double dxdy = mdx*mdy;
					double perc = 100.0 / count;
					double sum0mm = sum0*dxdy;
					double sum1mm = sum1*dxdy;
					double sum2mm = sum2*dxdy;

					double percent0 = sum0*perc;
					double percent1 = sum1*perc;
					double percent2 = sum2*perc;

					double area = count*dxdy;



					MyShape shape;






					text += "INTENSITIES:\n";
					text +=
						"leftTh: " + QString::number(leftT) + "   " + "rightTh: " + QString::number(rightT) + "   " + "\n" +
						"minValue: " + QString::number(minx) + "   " + "maxValue: " + QString::number(maxx) + "   " + +"medValue: " + QString::number(med) + "   ""\n" +
						"dark%: " + QString::number(percent0) + "   " + "mid%: " + QString::number(percent1) + "   " + +"bright%: " + QString::number(percent2) + "   " + "\n" +
						"darkCount: " + QString::number(sum0) + "   " + "midCount: " + QString::number(sum1) + "   " + +"brightCount: " + QString::number(sum2) + "   " + "\n" +
						"darkmm^2: " + QString::number(sum0mm) + "   " + "midmm^2: " + QString::number(sum1mm) + "   " + +"brightmm^2: " + QString::number(sum2mm) + "   " + "\n"

						"totalCount: " + QString::number(count) + "   " + +"areamm^2: " + QString::number(area) + "   " + "\n";

					text += "---------------------------------------------------------------------------------\n";



					//area
					//lineCenter
					//lineRed
					//lineGreen


					//double xPos = params[textXPosID] * nx;
					//double yPos = params[textYPosID] * ny;

					//double fSize = fontSize;
					//shape.shapeText = QString(text);
					//shape.textPos.setX(xPos);
					//shape.textPos.setY(yPos);//fSize*1.2);
					//shape.font.setPixelSize(fSize);







					//zebralem histogram
					//img.save("d:\\segment.png", "PNG");
					delete[]histTab;
				}
			}



			if ((double)params[fillOpacityID]>0)
			{

				MyShape s;
				s.poly = l1;
				s.opacity = (double)params[fillOpacityID];
				s.representation = 2;
				QColor c((Qt::GlobalColor)((int)(params[fillColorID])));
				s.polyColor = c;// setRgb(0, 255, 0);
				shapes->shapeList.append(s);
			}

			if ((bool)params[showFront1ID] == true)
			{

				QPolygonF l1;
				for (int j = 0; j<lines.count() / 2; j++)
				{
					l1.append(lines.at(j * 2));
				}

				MyShape s;
				s.poly = l1;
				QColor c((Qt::GlobalColor)((int)(params[lineUpperColorID])));
				s.polyColor = c;// setRgb(0, 255, 0);
				s.name = "upperLine";
				//s.polyColor.setRgb(255, 0, 0);
				shapes->shapeList.append(s);


				for (int j = 0; j<l1.count() - 1; j++)
				{
					p0[0] = l1.at(j).x()*mdx;
					p0[1] = l1.at(j).y()*mdy;

					p1[0] = l1.at(j + 1).x()*mdx;
					p1[1] = l1.at(j + 1).y()*mdy;
					l1Len += GetDistance2D(p0, p1);
				}


			}
			if ((bool)params[showFront2ID] == true)
			{
				QPolygonF l1;
				for (int j = 0; j<lines.count() / 2; j++)
				{
					l1.append(lines.at(j * 2 + 1));
				}

				MyShape s;
				s.poly = l1;
				//	s.polyColor.setRgb(0, 255, 0);
				QColor c((Qt::GlobalColor)((int)(params[lineLowerColorID])));
				s.polyColor = c;// setRgb(0, 255, 0);
				s.name = "lowerLine";
				shapes->shapeList.append(s);


				for (int j = 0; j<l1.count() - 1; j++)
				{
					p0[0] = l1.at(j).x()*mdx;
					p0[1] = l1.at(j).y()*mdy;

					p1[0] = l1.at(j + 1).x()*mdx;
					p1[1] = l1.at(j + 1).y()*mdy;
					l2Len += GetDistance2D(p0, p1);
				}
			}


			//calculate area;

			for (int j = 0; j<lines.count() / 2 - 1; j++)
			{

				//  p0   p2
				//
				//
				//
				//  p1  p3

				p0[0] = lines.at(j * 2).x()*mdx;
				p0[1] = lines.at(j * 2).y()*mdy;

				p1[0] = lines.at(j * 2 + 1).x()*mdx;
				p1[1] = lines.at(j * 2 + 1).y()*mdy;

				p2[0] = lines.at(j * 2 + 2).x()*mdx;
				p2[1] = lines.at(j * 2 + 2).y()*mdy;

				p3[0] = lines.at(j * 2 + 3).x()*mdx;
				p3[1] = lines.at(j * 2 + 3).y()*mdy;


				area += GetTriangleArea2D(p0, p1, p2);
				area += GetTriangleArea2D(p1, p2, p3);


			}
		}

		//lines boundinbBox;






		if (infoType>0)
		{



			//QString text;

			//switch (infoType)
			//{
			//
			//	case 1: text+="mean: "+QString::number(mean);break;
			//	
			//	case 2: text+="min: "+QString::number(mmin);break;
			//	case 3: text+="max: "+QString::number(mmax);break;
			//	case 4: text+="stddev: "+QString::number(stddev);break;
			//	case 5: text+="skewness: "+QString::number(skewness);break;
			//	case 6: text+="kurtosis: "+QString::number(kurtosis);break;
			//	//default: text+="Number: " +QString::number(objectNr); break;
			//}



			text += "THICKNESS:\n";
			text +=
				"min: " + QString::number(mmin) + "   " + "max: " + QString::number(mmax) + "   " + "\n" +
				"mean: " + QString::number(mean) + "   " + "stddev: " + QString::number(stddev) + "   " + "\n" +
				"skewness: " + QString::number(skewness) + "   " + "kurtosis: " + QString::number(kurtosis) + "   " + "\n";
			if ((area>0) && (intStats == false)) text += "area: " + QString::number(area) + " mm^2\n";
			if (lenCenter>0)text += "lenCenter: " + QString::number(lenCenter) + " ";
			if (l1Len>0) text += "lenRed: " + QString::number(l1Len) + " ";
			if (l2Len>0) text += "lenGreen: " + QString::number(l2Len) + " ";



			//area
			//lineCenter
			//lineRed
			//lineGreen




		}

		MyShape shape;
		double xPos = params[textXPosID] * nx;
		double yPos = params[textYPosID] * ny;

		double fSize = fontSize;
		shape.shapeText = QString(text);
		shape.textPos.setX(xPos);
		shape.textPos.setY(yPos);//fSize*1.2);
		shape.font.setPixelSize(fSize);
		//shape.textAngle=-minPos;

		//shape.textColor.setRgb(255, 0, 0);


		QColor c((Qt::GlobalColor)((int)(params[textColorID])));


		shape.textColor = c;
		shape.textToLogFlag = true;
		shapes->shapeList.append(shape);


	}
	memset(mask0, 255, mnx*mny * sizeof(unsigned char));
	return 1;

}
//----------------------------

LayerMaskIslandFilter::LayerMaskIslandFilter()
{
	filterName = "MaskIslandFilter";
	QStringList list;
	list.clear(); list << "false" << "true";

	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);
	AddParam("lefthTh", 0, true, 0, 255, 0.01, leftThresholdID);
	AddParam("rightthTh", 255, true, 0, 255, 0.01, rightThresholdID);

	AddParam("minArea", 0, true, 0, 124288, 1, minAreaID);
	AddParam("maxArea", 0, true, 0, 124288, 1, maxAreaID);



	AddParam("minWidth", 0, true, 0, 1024, 1, minWidthID);
	AddParam("maxWidth", 0, true, 0, 1024, 1, maxWidthID);
	AddParam("minHeight", 0, true, 0, 1024, 1, minHeightID);
	AddParam("maxHeight", 0, true, 0, 1024, 1, maxHeightID);

	list.clear(); list << "nothing " << "rectangle" << "circle" << "ellipse";

	AddParam("shape", 0, true, 0, 3, 1, shapeID, &list);
	AddParam("tolerance", 0.5, true, 0, 1, 0.001, toleranceID);

	list.clear(); list << "false" << "true";
	AddParam("showOrigBounds", 0, true, 0, 1, 1, showOrigBoundsID, &list);
	AddParam("showFittedBounds", 1, true, 0, 1, 1, showFittedBoundsID, &list);
	AddParam("showFittedAxes", 1, true, 0, 1, 1, showFittedAxesID, &list);
	AddParam("showFittedEllipse", 1, true, 0, 1, 1, showFittedEllipseID, &list);
	AddParam("showContourPoints", 1, true, 0, 1, 1, showContourPointsID, &list);

	list.clear(); list << "dilate" << "erode";
	AddParam("morphology", 0, true, 0, 1, 1, morphologyTypeID, &list);
	list.clear(); list << "square" << "circle";
	AddParam("ktype", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	int morphologyTypeID;
	int morphologyKernelTypeID;


	list.clear(); list << "false" << "true";
	AddParam("drawFonts", 1, true, 0, 1, 1, drawFontsID, &list);
	AddParam("CleanShapes", 1, true, 0, 1, 1, cleanShapesID, &list);
	AddParam("useStartShapes", 0, true, 0, 1, 1, useStartShapesID, &list);

	list.clear();
	list << "Number: " << "pixel area: " << "box area: " << "pix/box: " << "ellipse area: " << "pix/ellipse " << "D1: " << "D2"<<"all";



	AddParam("infoType", 0, true, 0, 8, 1, infoTypeID, &list);

	AddParam("fontSize", 15, true, 4, 100, 1, fontSizeID);
	list.clear(); list << "center" << "left" << "right" << "top" << "bottom"<<"absolute";
	AddParam("showInfoPlace", 1, true, 0, 5, 1, showInfoPlaceID, &list);




	//int globalXCorrectionID, globalYCorrectionID;
	
	
	AddParam("globalX", 0, true, -512, 512, 1, globalXCorrectionID, &list);
	AddParam("globalY", 0, true, -512, 512, 1, globalYCorrectionID, &list);
	// 
	// 
	//	AddParam ("rotate",0,true,0,360,1,rotateID);
	//int shapeID,int toleranceID;

}
//------------------------------------

bool	LayerMaskIslandFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{



	if ((sliceNr<0) || (sliceNr >= data->GetNz()) || (shapes == NULL)) return 0;
	//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	int globalXCorrection = params[globalXCorrectionID];
	int globalYCorrection = params[globalYCorrectionID];

	double scale[2] = { data->GetXscale(),data->GetYscale() };
	double scale1[2] = { 1.0 / scale[0],1.0 / scale[1] };
	unsigned char *masktmp;
	int i, j, k;
	bool revth = params[inverseID];
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
	unsigned short maxValue = (mainDataMaxValue > 0) ? mainDataMaxValue : data->GetMaxValue();
	int leftTh = params[leftThresholdID] * maxValue / 255.0;
	int rightTh = params[rightThresholdID] * maxValue/ 255.0;

	for (i = 0; i<data->GetSliceSize(); i++)
	{
		if ((tab[i] >= leftTh) && (tab[i] <= rightTh)) { mask0[i] = val1; c1++; }
		else { mask0[i] = val2; c2++; };
	}


	int sliceSize = data->GetSliceSize();
	int nx = data->GetNx();
	double nx1d = 1.0 / nx;
	int ny = data->GetNy();
	int max = MAX(c1, c2);
	if (max == sliceSize)
	{
		for (i = 0; i<data->GetSliceSize(); i++)
		{
			if (mask0[i] == 2) mask0[i] = 255;
		}
		return 1;
	}

	//zalewam wyspy

	//biegne po wszystkich pixelach i zalewam dwojki
	//zbieram wspolrzedne pktow dodanych do danej wyspy do tablicy
	//sprawdzam parametry
	//jesli ok zamieniam na 1
	//jesli nie zeruje


	int *listS = intBuffers[0];//new int [sliceSize];
	int *listTmp = intBuffers[1];
	int nr0 = 0;


	//make small kernel



	int kernelSize = 8;
	int size = 1;
	int *kernel = new int[kernelSize];
	int *kernelInc = new int[kernelSize];
	int pos = 0;
	for (j = -size; j<size + 1; j++)
	{
		for (i = -size; i<size + 1; i++)
		{
			if (!((i == 0) && (j == 0)))
			{
				kernel[pos] = i + j*nx;
				kernelInc[pos++] = j;
			}

		}
	}


	int startPos = 0;
	int ap = 0;
	int lp = 0;
	int count;
	int tmp;
	int dsizemin1 = sliceSize - 1;


	//int minAreaID,maxAreaID,minWidthID,maxWidthID,minHeightID,maxHeightID;


	int minArea = params[minAreaID];
	int maxArea = params[maxAreaID];
	int minWidth = params[minWidthID];
	int maxWidth = params[maxWidthID];
	int minHeight = params[minHeightID];
	int maxHeight = params[maxHeightID];

	if (maxArea == 0) maxArea = sliceSize;
	if (maxWidth == 0) maxWidth = nx;
	if (maxHeight == 0) maxHeight = ny;
	//if (minWidth==0) minWidth=nx;

	int shape = params[shapeID];
	double tol = params[toleranceID];


	int morphologyType = params[morphologyTypeID];
	int morphologyKernelType = params[morphologyKernelTypeID];


	int showOrigBound = params[showOrigBoundsID];
	int showFittedBounds = params[showFittedBoundsID];
	int showFittedAxes = params[showFittedAxesID];
	int showFittedEllipse = params[showFittedEllipseID];
	int showContourPoints = params[showContourPointsID];

	bool drawFonts = params[drawFontsID];

	int infoType = params[infoTypeID];

	bool useStartShapes = params[useStartShapesID];
	bool cleanShapes = params[cleanShapesID];
	shapes->cleanPolygonFlag = cleanShapes;

	int objectNr = 0;
	int *dilatePoints = intBuffers[0];//new int[sliceSize];


	int jStart = 0;
	int jStop = sliceSize;

	QList <int> centerList;
	if (useStartShapes == true)
	{
		//zrob tylko dla srodkow z polygons

		QList<QPolygonF> *slist = &shapes->polygonList;
		int nrPolygons = shapes->polygonList.count();


		for (int ix = 0; ix<nrPolygons; ix++)
		{

			MyShape s;
			s.poly = slist->at(ix);
			//wez shape i policz srednia x i y;
			if (s.poly.count()>0)
			{
				double center[2] = { 0 };
				for (int ii = 0; ii<s.poly.count(); ii++)
				{
					center[0] += s.poly.at(ii).x();
					center[1] += s.poly.at(ii).y();
				}
				center[0] /= (double)s.poly.count();
				center[1] /= (double)s.poly.count();

				//oblicz pos calkowita

				int posx = (int)(center[0]) + ((int)(center[1]))*nx;

				if ((posx >= 0) && (posx<sliceSize))
				{
					if (mask0[posx] == 2)
					{
						centerList.append(posx);
					}
				}
			}
		}

		if (centerList.count()>0)
		{
			jStop = centerList.count();
		}

	}
	else
	{

	}



	int currPos;

	for (j = jStart; j<jStop; j++)
	{
		int ccc = centerList.count();

		if (ccc>0)
		{
			currPos = centerList.at(j);
		}
		else
		{
			currPos = j;
		}

		if (mask0[currPos] == 2)
		{
			int minx = nx;
			int maxx = 0;
			int miny = ny;
			int maxy = 0;
			int x, y;
			nr0 = 0;
			count = 0;
			ap = currPos;
			lp = 0;
			listS[0] = currPos;
			listTmp[(count++)] = currPos;
			int oldx, oldy;
			//zalewam
			do
			{
				ap = listS[lp--];

				oldy = ap*nx1d;

				for (i = 0; i<kernelSize; i++)
				{
					tmp = ap + kernel[i];
					y = tmp*nx1d;
					x = tmp - y*nx;

					if (!((tmp>dsizemin1) || (tmp<0) || (y != oldy + kernelInc[i]))) //brzydkie
					{
						if (mask0[tmp] == 2)
						{
							mask0[tmp] = 3;
							listS[++lp] = tmp;
							listTmp[count++] = tmp;



							if (x<minx) minx = x;
							if (x>maxx) maxx = x;
							if (y<miny) miny = y;
							if (y>maxy) maxy = y;
						}
					}
				}
			} while (lp >= 0);

			int width = maxx - minx;
			int height = maxy - miny;
			if ((count<minArea) || (count>maxArea) || (width<minWidth) || (width>maxWidth) || (height<minHeight) || (height>maxHeight))
			{


				//wyczysc
				for (i = 0; i<count; i++) { mask0[listTmp[i]] = 0; }	//{	mask0[listS[dsizemin1-i]]=0;}

			}
			else //sprawdz width i height - jesli zle to wyczysc jesli ok to zamaluj na 1
			{
				if ((shape>0) && (shape<4))
				{
					int pixelArea = count;
					/*if (showOrigBound)
					{
					MyShape shape;
					shape.polyColor.setRgb(0,255,0);
					shape.poly.append(QPointF(minx,miny));
					shape.poly.append(QPointF(maxx,miny));
					shape.poly.append(QPointF(maxx,maxy));
					shape.poly.append(QPointF(minx,maxy));

					shape.representation=2;
					shapes->shapeList.append(shape);
					}*/


					//ekstrakcja konturu

					//nowe dane

					int nnx = maxx - minx + 1;
					int nny = maxy - miny + 1;

					int padd = 2;
					int nSizeX = nnx + padd * 2;
					int nSizeY = nny + padd * 2;
					int nSsize = nSizeX*nSizeY;

					int padd1 = 1;

					int nSizeX1 = nnx + padd1 * 2;
					int nSizeY1 = nny + padd1 * 2;

					//tworze maly zbiorek z danymi aktualnej wyspy powiekszonej o padd z kazdej strony
					unsigned char *tab = new unsigned char[nSsize];
					memset(tab, 0, nSsize * sizeof(unsigned char));

					//zmniejszona tablica 
					for (int k = 0; k<count; k++)
					{

						y = listTmp[k] * nx1d;
						x = listTmp[k] - y*nx;

						x += padd - minx;	y += padd - miny;
						tab[x + y*nSizeX] = 1;

					}

					//tablica wypelniona

					RawDataSet *noneData = new RawDataSet(nSizeX, nSizeY, 1, 1, 1, 1, 1, false);
					int kSize;
					int kerenelSize2 = 0;
					int size2 = 1;
					int kernelType = morphologyKernelType;
					int *kernel2 = GetGrowingKernel2D(kerenelSize2, size2, noneData, kernelType);
					int tmp;
					int dsizemin1 = noneData->GetVolumeSize() - 1;


					int nrOfDilatePoints = 0;

					int startPos = nSizeX*padd1 + padd1;
					bool stop;
					double nnx1d = 1.0 / nSizeX;
					//contur extraction

					int frontVal = morphologyType;
					int backVal = !frontVal;

					for (int jj = 0; jj<nSizeY1; jj++)
					{

						for (int ii = 0; ii<nSizeX1; ii++)
						{
							if (tab[startPos] == frontVal)
							{
								stop = false;
								for (int kk = 0; kk<kerenelSize2; kk++)
								{

									tmp = startPos + kernel2[kk];
									if ((tmp>dsizemin1) || (tmp<0)) { continue; }
									else {
										//jakikolwiek zapalony
										if (tab[tmp] == backVal) {
											stop = true;
											break;
										}
									}


								}
								if (stop)
								{
									y = startPos*nnx1d;
									x = startPos - y*nSizeX;



									//tablica dilatepoints zawiera wspolrzedne konturu globalne
									dilatePoints[nrOfDilatePoints] = x - padd + minx;

									dilatePoints[nrOfDilatePoints + 1] = y - padd + miny;
									nrOfDilatePoints += 2;
								}
							}
							startPos++;
						}
						startPos += 2 * padd1;

					}

					nrOfDilatePoints /= 2;








					//draw dilate points
					if (showContourPoints)

					{
						MyShape shape;
						shape.polyColor.setRgb(255, 0, 255);





						for (int ii = 0; ii<nrOfDilatePoints * 2; ii += 2)
						{
							shape.poly.append(QPointF(dilatePoints[ii], dilatePoints[ii + 1]));
						}

						shape.representation = 0;
						shapes->shapeList.append(shape);

					}

					//draw dilate points bounding box;
					if (showOrigBound)
					{


						int minx = nx;
						int maxx = 0;
						int miny = ny;
						int maxy = 0;



						for (int ii = 0; ii<nrOfDilatePoints * 2; ii += 2)
						{
							x = dilatePoints[ii];
							y = dilatePoints[ii + 1];

							if (x<minx) minx = x;
							if (x>maxx) maxx = x;
							if (y<miny) miny = y;
							if (y>maxy) maxy = y;

						}


						MyShape shape;
						shape.polyColor.setRgb(255, 255, 255);





						shape.poly.append(QPointF(minx, miny));
						shape.poly.append(QPointF(maxx, miny));
						shape.poly.append(QPointF(maxx, maxy));
						shape.poly.append(QPointF(minx, maxy));


						shape.representation = 2;
						shapes->shapeList.append(shape);

					}







					// mamy kontur zebrany w dilatePoints
					// przebiegne po kacie 360stopni i sprawdze bounding box - 

					double minArea = data->GetSliceSize();;
					double area;
					double minPos = 0;
					double angle;
					double minAngle;


					double minX = nnx;
					double maxX = -nnx;
					double minY = nny;
					double maxY = -nny;

					double rectX = 0;
					double rectY = 0;
					//get center

					double center[2] = { 0 };
					for (int ii = 0; ii<nrOfDilatePoints; ii++)
					{
						center[0] += dilatePoints[ii * 2];
						center[1] += dilatePoints[ii * 2 + 1];
					}

					center[0] /= nrOfDilatePoints;
					center[1] /= nrOfDilatePoints;
					//najpierw rzadkie obroty 
					//	int ii=params[rotateID];

					double nCenter[2];
					double startx, stopx, stepx;
					double Astep = 45;//45.0;
					double Astep2 = Astep;

					double spacing2 = 360 / Astep2;
					spacing2 = Astep / spacing2;

					startx = 0; stopx = 260, stepx = Astep;
					for (int jj = 0; jj<2; jj++)
					{
						//for(int ii=0;ii<360;ii+=Astep)
						for (int ii = startx; ii<stopx; ii += stepx)
						{
							minX = INT_MAX;		 maxX = -INT_MAX;	 minY = INT_MAX;		 maxY = -INT_MAX;
							//calculate area
							double alpha = ii* to_rad;			double ca = cos(alpha);		double sa = sin(alpha);

							for (int jj = 0; jj<nrOfDilatePoints; jj++)
							{
								double x = (double)dilatePoints[jj * 2] - center[0];		double y = (double)dilatePoints[jj * 2 + 1] - center[1];
								x *= scale[0];y *= scale[1];//zeby obiekt po obrocie byl we wlasciwych proporcjach do liczenia pola
								double xx = x * ca - y * sa;// +center[0];		
								double yy = x * sa + y * ca;//+center[1];

								if (xx<minX) minX = xx;						if (xx>maxX) maxX = xx;
								if (yy<minY) minY = yy;						if (yy>maxY) maxY = yy;
							}
							area = (maxX - minX)*(maxY - minY);
							if (area <minArea)
							{
								minArea = area;			area = minArea;				minPos = ii;
								rectX = (maxX - minX);		rectY = (maxY - minY);

								nCenter[0] = (minX + maxX)*0.5*scale1[0]+center[0];
								nCenter[1] = (minY + maxY) * 0.5 * scale1[1]+center[1];
								//nCenter[0] = center[0];
								//nCenter[1] = center[1];
							}
						}
						startx = minPos - Astep2; stopx = minPos + Astep2; stepx = spacing2;
					}

					{
						double ptsX[4], ptsY[4];



						double angle = -minPos*to_rad;
						double ca = cos(angle);		double sa = sin(angle);

						double r1 = rectX / 2;
						double r2 = rectY / 2;


						double nCenter2[2];



						nCenter2[0] = nCenter[0];
						nCenter2[1] = nCenter[1];

						r1 *= scale1[0];
						r2 *= scale1[1];
						ptsX[0] = nCenter2[0] - r1;		ptsY[0] = nCenter2[1] - r2;
						ptsX[1] = nCenter2[0] + r1;		ptsY[1] = nCenter2[1] - r2;
						ptsX[2] = nCenter2[0] + r1;		ptsY[2] = nCenter2[1] + r2;
						ptsX[3] = nCenter2[0] - r1;		ptsY[3] = nCenter2[1] + r2;




						QPolygonF poly;
						for (int ii = 0; ii < 4; ii++)
						{

							/*	double xx = (ptsX[ii] - center[0])*ca - (ptsY[ii] - center[1])*sa + center[0];
								double yy = (ptsX[ii] - center[0])*sa + (ptsY[ii] - center[1])*ca + center[1];*/

							double xx = ((ptsX[ii] - center[0]) * ca * scale[0] - (ptsY[ii] - center[1]) * sa * scale[1]) * scale1[0] + center[0];
							double yy = ((ptsX[ii] - center[0]) * sa * scale[0] + (ptsY[ii] - center[1]) * ca * scale[1]) * scale1[1] + center[1];

							poly.append(QPointF(xx, yy));
						}

						if (showFittedBounds)
						{
							MyShape shape;
							shape.polyColor.setRgb(255, 0, 0);
							double xx, yy;
							

							shape.poly = poly;

							shape.representation = 2;
							shapes->shapeList.append(shape);
						}

						//if (showFittedBounds)
						//{
						//	MyShape shape;
						//	shape.polyColor.setRgb(255, 0, 0);
						//	double xx, yy;
						//	for (int ii = 0; ii<4; ii++)
						//	{

						//	/*	double xx = (ptsX[ii] - center[0])*ca - (ptsY[ii] - center[1])*sa + center[0];
						//		double yy = (ptsX[ii] - center[0])*sa + (ptsY[ii] - center[1])*ca + center[1];*/

						//		double xx = ((ptsX[ii] - center[0]) * ca * scale[0] - (ptsY[ii] - center[1]) * sa * scale[1])*scale1[0] + center[0];
						//		double yy = ((ptsX[ii] - center[0]) * sa * scale[0] + (ptsY[ii] - center[1]) * ca * scale[1])*scale1[1] + center[1];

						//		shape.poly.append(QPointF(xx, yy));
						//	}



						//	shape.representation = 2;
						//	shapes->shapeList.append(shape);
						//}


						//osie glowne:)

						if (drawFonts == true)
						{
							MyShape shape;

							double ellipseArea = r1*r2*M_PI;
							double boxArea = rectX*rectY;

							double pixToEll = pixelArea / ellipseArea;
							double pixToBox = pixelArea / boxArea;

							double d1 = 2 * r1;
							double d2 = 2 * r2;

							QString text;

							switch (infoType)
							{
							case 0: text += QString::number(objectNr); break;
							case 1: text += QString::number(pixelArea); break;
							case 2: text += QString::number(boxArea); break;
							case 3: text += QString::number(pixToBox); break;
							case 4: text += QString::number(ellipseArea); break;
							case 5: text += QString::number(pixToEll); break;
							case 6: text += QString::number(d1); break;
							case 7: text += QString::number(d2); break;
							case 8: 


								//int fontSize = params[fontSizeID];
								//MyShape shape;
								//QString text;
								text = "objectNr: " + QString::number(objectNr) + "\n" +
									"pixelArea: " + QString::number(pixelArea) + "\n" +
									"boxArea: " + QString::number(boxArea) + "\n" +
									"pixToBox: " + QString::number(pixToBox) + "\n" +
									"ellipseArea: " + QString::number(ellipseArea) + "\n" +
									"pixToEll: " + QString::number(pixToEll) + "\n" +
									"d1: " + QString::number(d1) + "\n" +
									"d2: " + QString::number(d2) + "\n";

								break;
								//default: text+="Number: " +QString::number(objectNr); break;
							}
							shape.shapeText = QString(text);
						
							//shape.textAngle=-minPos;
							shape.textColor.setRgb(255, 255, 255);

							int fontSize = params[fontSizeID];
							int		showInfoPlace = params[showInfoPlaceID];

							double fSize = fontSize;
							shape.font.setPixelSize(fSize);

							if (showInfoPlace < 5)
							{
								QRectF rect = poly.boundingRect();
								shape.objectBounds = rect;


								shape.textAutoPos = showInfoPlace;
							}
							else
							{

								//int globalXCorrection = params[globalXCorrectionID];
								//int globalYCorrection = params[globalYCorrectionID];
								shape.textPos.setX(ptsX[0]+ globalXCorrection);
								shape.textPos.setY(ptsY[0]+ globalYCorrection);
							}
							shapes->shapeList.append(shape);

						}



						ptsX[0] = nCenter2[0] - r1;		ptsY[0] = nCenter2[1];
						ptsX[1] = nCenter2[0] + r1;		ptsY[1] = nCenter2[1];
						ptsX[2] = nCenter2[0];		ptsY[2] = nCenter2[1] - r2;
						ptsX[3] = nCenter2[0];		ptsY[3] = nCenter2[1] + r2;



						for (int ii = 0; ii<4; ii++)
						{

							/*	double xx = (ptsX[ii] - center[0])*ca - (ptsY[ii] - center[1])*sa + center[0];
									double yy = (ptsX[ii] - center[0])*sa + (ptsY[ii] - center[1])*ca + center[1];*/

							double xx = ((ptsX[ii] - center[0]) * ca * scale[0] - (ptsY[ii] - center[1]) * sa * scale[1]) * scale1[0] + center[0];
							double yy = ((ptsX[ii] - center[0]) * sa * scale[0] + (ptsY[ii] - center[1]) * ca * scale[1]) * scale1[1] + center[1];
							ptsX[ii] = xx;		ptsY[ii] = yy;
						}

						if (showFittedAxes)
						{
							{
								MyShape shape;
								shape.polyColor.setRgb(0, 128, 255);
								shape.representation = 2;
								shape.poly.append(QPointF(ptsX[0], ptsY[0]));
								shape.poly.append(QPointF(ptsX[1], ptsY[1]));
								shapes->shapeList.append(shape);
							}
							{
								MyShape shape;
								shape.polyColor.setRgb(0, 128, 255);
								shape.representation = 2;
								shape.poly.append(QPointF(ptsX[2], ptsY[2]));
								shape.poly.append(QPointF(ptsX[3], ptsY[3]));
								shapes->shapeList.append(shape);
							}
						}

						//dodam elipse 
						if (showFittedEllipse)
						{
							MyShape shape2;
							double centerE[2] = { (ptsX[0] + ptsX[1]) / 2.0, (ptsY[0] + ptsY[1]) / 2.0 };
							shape2.ellipses.append(QPointF(centerE[0], centerE[1]));//center
							shape2.ellipses.append(QPointF(r1, r2));//rads
							shape2.ellipses.append(QPointF(-minPos, 50));
							shape2.ellipsesColor.setRgb(128, 128, 240);
							shapes->shapeList.append(shape2);
						}


					}
					//analiza osi glownych konturu
					// sprawdzanie czy podobne do kszta�tu zajmujacego podobny obwod
					delete[]tab;
					delete[]kernel2;
					delete noneData;
					objectNr++;
				}
				// zamien na 1
				for (i = 0; i<count; i++) { mask0[listTmp[i]] = 255; }	//	mask0[listS[dsizemin1-i]]=1;}
																		//}

			}




		}
	}

	//delete []dilatePoints;
	//delete []listS;
	delete[]kernel;
	delete[]kernelInc;






	return 1;

}


//--------------------------------------------------
LayerMaskIntensityAnalysisFilter::LayerMaskIntensityAnalysisFilter()
{
	filterName = "LayerMaskIntensityAnalysisFilter";
	QStringList list;
	list.clear(); list << "false" << "true";

	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);
	AddParam("lefthTh", 0, true, 0, 255, 1, leftThresholdID);
	AddParam("rightthTh", 255, true, 0, 255, 1, rightThresholdID);
	AddParam("useThreholds", 0, true, 0, 1, 1, useThresholdsID, &list);



	AddParam("CleanShapes", 1, true, 0, 1, 1, cleanShapesID, &list);

	AddParam("nrOfRegion1", 0, true, 0, 50, 1, nrOfRegion1ID);
	AddParam("nrOfRegion2", 1, true, 0, 50, 1, nrOfRegion2ID);

	AddParam("useHistLogScale", 1, true, 0, 1, 1, useHistLogScaleID, &list);


	AddParam("updateReg1", 1, true, 0, 1, 1, updateRegion1ID, &list);
	AddParam("updateReg2", 1, true, 0, 1, 1, updateRegion2ID, &list);

	AddParam("showRealRegionBounds", 1, true, 0, 1, 1, showRealRegionBoundsID, &list);

	AddParam("showRegionInfo", 1, true, 0, 1, 1, showRegionInfoID, &list);

	list.clear(); list << "center" << "left" << "right" << "top" << "bottom";
	AddParam("showInfoPlace", 1, true, 0, 4, 1, showInfoPlaceID, &list);




	list.clear();


	stats1Calculated = false;
	stats2Calculated = false;

	//	AddParam ("rotate",0,true,0,360,1,rotateID);
	//int shapeID,int toleranceID;

}
//------------------------------------

bool	LayerMaskIntensityAnalysisFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{


	if ((sliceNr<0) || (sliceNr >= data->GetNz()) || (shapes == NULL)) return 0;
	//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	int i, j, k;
	bool revth = params[inverseID];
	unsigned char val1, val2;
	if (revth == false)
	{
		val1 = 255; val2 = 0;
	}
	else
	{
		val1 = 0; val2 = 255;
	}

	int c1 = 0;
	int c2 = 0;
	//progowanie

	unsigned short maxValue = (mainDataMaxValue > 0) ? mainDataMaxValue : data->GetMaxValue();
	int leftTh = params[leftThresholdID] *maxValue / 255.0;
	int rightTh = params[rightThresholdID] * maxValue/ 255.0;

	int	 nrOfRegion1 = params[nrOfRegion1ID];
	int  nrOfRegion2 = params[nrOfRegion2ID];


	bool updateRegion1 = params[updateRegion1ID];
	bool updateRegion2 = params[updateRegion2ID];

	if (updateRegion1 == true) stats1Calculated = false;
	if (updateRegion2 == true) stats2Calculated = false;


	bool useThresholds = params[useThresholdsID];
	bool useHistLogScale = params[useHistLogScaleID];

	bool showRealRegionBounds = params[showRealRegionBoundsID];


	bool	showRegionInfo = params[showRegionInfoID];
	int		showInfoPlace = params[showInfoPlaceID];

	int sliceSize = data->GetSliceSize();

	if (useThresholds == true)
	{
		for (i = 0; i<sliceSize; i++)
		{
			if ((tab[i] >= leftTh) && (tab[i] <= rightTh)) { mask0[i] = val1; c1++; }
			else { mask0[i] = val2; c2++; };
		}
	}
	else
	{
		for (int ii = 0; ii<sliceSize; ii++)
		{
			mask0[ii] = 255;
		}
		//memset(mask0,255,sliceSize);
	}
	//zalewam wyspy

	//biegne po wszystkich pixelach i zalewam dwojki
	//zbieram wspolrzedne pktow dodanych do danej wyspy do tablicy
	//sprawdzam parametry
	//jesli ok zamieniam na 1
	//jesli nie zeruje



	bool cleanShapes = params[cleanShapesID];


	shapes->cleanPolygonFlag = cleanShapes;

	int objectNr = 0;



	QList <int> centerList;


	//zrob tylko dla srodkow z polygons

	QList<QPolygonF> *slist = &shapes->polygonList;
	int nrPolygons = shapes->polygonList.count();

	unsigned char *buffer = new unsigned char[sliceSize];
	memcpy(buffer, mask0, sliceSize);
	if (nrPolygons>0)
	{
		QImage *visImage = iWidget->GetCurrentImage();
		memset(visImage->bits(), 0, 65536 * 4);
		int nx = data->GetNx();
		int ny = data->GetNy();

		QImage img(nx, ny, QImage::Format_RGB32);
		img.fill(Qt::black);
		QPainter Painter2(&img);
		Painter2.setBrush(Qt::white);
		Painter2.setPen(1);
		//	qDebug () <<"fpos:"+QString::number(shapes->polygonList.at(0).at(0).x())<<" , "<<QString::number(shapes->polygonList.at(0).at(0).y());
		int scatterSize = 8;
		for (int ix = 0; ix<nrPolygons; ix++)
		{

			MyShape s;
			s.poly = slist->at(ix);
			Painter2.drawPolygon(s.poly);
			//wez shape i policz srednia x i y;
			{
				MyShape shape;
				shape.polyColor.setRgb(255, 0, 0);
				shape.representation = 2;
				shape.poly = s.poly;
				shapes->shapeList.append(shape);
			}



			int count1, count2;
			if ((nrOfRegion1 == ix) && (updateRegion1 == true))
			{
				stats1Calculated = UpdateStats(scatterSize, hist1, stats1, s.poly, buffer, nx, ny, tab, intBuffers, count1, showRealRegionBounds, shapes, mainDataMaxValue, true, QColor(255, 0, 255));
				{

					double area1 = count1*mdx*mdy;
					QString text = "PixelCount: " + QString::number(count1);
					text += "\nArea: " + QString::number(area1) + " mm2";

					MyShape shape;						shape.shapeText = QString(text);
					shape.textColor.setRgb(255, 33, 0);
					QRectF rect = s.poly.boundingRect();
					shape.objectBounds = rect;
					shape.textAutoPos = showInfoPlace;

					shapes->shapeList.append(shape);
				}

			}
			if ((nrOfRegion2 == ix) && (updateRegion2 == true))
			{
				stats2Calculated = UpdateStats(scatterSize, hist2, stats2, s.poly, buffer, nx, ny, tab, intBuffers, count2, showRealRegionBounds, shapes, mainDataMaxValue, true, QColor(255, 0, 255));
				{

					double area1 = count2*mdx*mdy;
					QString text = "PixelCount: " + QString::number(count2);
					text += "\nArea: " + QString::number(area1) + " mm2";

					MyShape shape;						shape.shapeText = QString(text);
					shape.textColor.setRgb(255, 33, 0);
					QRectF rect = s.poly.boundingRect();
					shape.objectBounds = rect;
					shape.textAutoPos = showInfoPlace;

					shapes->shapeList.append(shape);
				}
			}

			QRectF rect = s.poly.boundingRect();
			//qreal x1,x2,y1,y2;
			//rect.getCoords(&x1,&y1,&x2,&y2);
			{
				QString text = QString::number(ix);
				MyShape shape;						shape.shapeText = QString(text);
				//shape.textPos.setX((x1+x2)/2);		shape.textPos.setY((y1+y2)/2);				
				shape.textColor.setRgb(255, 0, 0);
				shape.objectBounds = rect;
				shape.textAutoPos = 0;

				shapes->shapeList.append(shape);
			}




		}
		//narysowane
		unsigned char *tab11 = img.bits();
		unsigned int *tab1 = (unsigned int *)tab11;

		bool test;
		for (i = 0; i<data->GetSliceSize(); i++)
		{
			//mask0[i]=val;
			test = (((*tab1++) == 0xffffffff));//' mask0[i]&=255;
			if (!((test == true) && (mask0[i] == 255)))

				mask0[i] = 0;
		}


		QColor backCol(Qt::black);
		QColor col1(Qt::blue);
		QColor col2(Qt::green);
		iWidget->GetShapes()->shapeList.clear();




		if (stats1Calculated == true)
		{
			//malujemy histogram - na gorze i opis

			DrawHistogram(scatterSize, hist1, 0, 10, 255, 120, useHistLogScale, backCol, col1, visImage);


			QString text = "Nr: " + QString::number(nrOfRegion1) + "   " +
				"min: " + QString::number(stats1[0]) + "   " +
				"max: " + QString::number(stats1[1]) + "\n" +
				"avg: " + QString::number(stats1[2]) + "   " +
				"stddev: " + QString::number(stats1[3]) + "   "
				"max: " + QString::number(stats1[1]) + "\n" +
				"skwewness: " + QString::number(stats1[4]) + "   " +
				"kurtosis: " + QString::number(stats1[5]);

			MyShape shape;						shape.shapeText = QString(text);
			shape.textPos.setX(5);				shape.textPos.setY(10);
			shape.textColor.setRgb(255, 0, 0);	iWidget->GetShapes()->shapeList.append(shape);



			if (updateRegion1 == false)
			{
				QString text = "BLOCKED";
				MyShape shape;						shape.shapeText = QString(text);
				shape.textPos.setX(200);				shape.textPos.setY(64);
				shape.textColor.setRgb(0, 255, 0);	iWidget->GetShapes()->shapeList.append(shape);
			}



		}

		if (stats2Calculated == true)
		{
			DrawHistogram(scatterSize, hist2, 0, 138, 255, 120, useHistLogScale, backCol, col2, visImage);

			QString text = "Nr: " + QString::number(nrOfRegion2) + "   " +
				"min: " + QString::number(stats2[0]) + "   " +
				"max: " + QString::number(stats2[1]) + "\n" +
				"avg: " + QString::number(stats2[2]) + "   " +
				"stddev: " + QString::number(stats2[3]) + "   "
				"max: " + QString::number(stats2[1]) + "\n" +
				"skwewness: " + QString::number(stats2[4]) + "   " +
				"kurtosis: " + QString::number(stats2[5]);
			MyShape shape;						shape.shapeText = QString(text);
			shape.textPos.setX(5);				shape.textPos.setY(10 + 128);
			shape.textColor.setRgb(255, 0, 0);	iWidget->GetShapes()->shapeList.append(shape);


			if (updateRegion2 == false)
			{
				QString text = "BLOCKED";
				MyShape shape;						shape.shapeText = QString(text);
				shape.textPos.setX(200);				shape.textPos.setY(64 + 128);
				shape.textColor.setRgb(0, 255, 0);	iWidget->GetShapes()->shapeList.append(shape);
			}


		}


		/// napisy



		//updateRegion1


		iWidget->update();


		//	MyShape shape;

		//				
		////	QString text="fpos:"+QString::number(shapes->polygonList.at(0).at(0).x())+" , "+QString::number(shapes->polygonList.at(0).at(0).y());
		//				
		//					
		//	shape.shapeText=QString(text);
		//	shape.textPos.setX(0);
		//	shape.textPos.setY(0);
		//					
		//	shape.textColor.setRgb(255,0,0);
		//	shapes->shapeList.append(shape);


	}
	delete[]buffer;





	return 1;

}
//----------
//void	LayerMaskIntensityAnalysisFilter::DrawHistogram(int *hist, int startY,int backClean, int height, bool logScale,QColor backC,QColor frontC,QImage *vus)
//{
//
//			int histMax=0;
//			//int startY=0;
//			//int height=128;
//			double vLog;
//			//int *hist=hist1;
//
//
//			for(int i=0;i<256;i++)
//			{
//				if (hist[i]>histMax) histMax=hist[i];
//			}
//
//			if (histMax>0)
//			{
//				double hMaxLog=(logScale)?log(1+(double)(histMax))*100:histMax;
//
//				double scale=(double)height/hMaxLog;
//				int val;
//				QPainter p(visImage);
//	
//
//				for(int i=0;i<256;i++)
//				{
//					vLog=(logScale)?log(1+(double)(hist[i]))*100:hist[i];
//
//					val=vLog*scale;//ma byc z zakresu 0-128
//					p.setPen(backC);
//					p.drawLine(i,startY-backClean,i,startY+(height-val));
//					p.setPen(frontC);
//					p.drawLine(i,startY+height-val+1,i,startY+height);
//				}
//
//			}
//}
////------------------------------------
//bool LayerMaskIntensityAnalysisFilter::UpdateStats(int *hist,double *stats,QPolygonF poly,unsigned char *mask,int nx,int ny,unsigned short *tab,int **tmpTab,int &count,bool showBounds)
//{
//	if ((hist==NULL)||(stats==NULL)||(mask==NULL)||(tmpTab[0]==NULL)||(tmpTab[1]==NULL)) return false;
//		
//	QRectF rect=poly.boundingRect();
//	qreal x1,x2,y1,y2;
//	rect.getCoords(&x1,&y1,&x2,&y2);
//	int X1=(int)x1;
//	int Y1=(int)y1;
//	int X2=(int)x2+1;
//	int Y2=(int)y2+1;
//
//
//	int NX=x2-x1;
//	int NY=y2-y1;
//
//	int sliceSize=nx*ny;
//
//	QImage img(nx,ny,QImage::Format_RGB32);
//	img.fill(Qt::black);
//	QPainter Painter2(&img);	
//	Painter2.setBrush(Qt::white);
//	Painter2.setPen(1);	
//	Painter2.drawPolygon(poly);
//
//	int i,j;
//	bool test;
//	int pos;
//	unsigned char *tab11=img.bits();
//	unsigned int *tab1=(unsigned int *)tab11;
//	memset(hist,0,256*sizeof(int));
//	memset (stats,0,16*sizeof(double));
//
//	double stddev;
//
//
//	int  		sum=0;
//	long int 	sum2=0;
//	count=0;
//	unsigned char val;
//	unsigned char min=255;
//	unsigned char max=0;
//
//	int *realTab=tmpTab[0];
//
//	double scale=(maxValue>0)?255.0/maxValue:1.0;
//	for(j=y1; j<=y2;j++)
//	{
//		for(i=x1;i<=x2;i++)
//		{
//			pos=i+j*nx;;
//			test=(((tab1[pos])==0xffffffff));
//			if ((test==true)&&(mask[pos]==255))
//			{
//				//dobry pixel - jego wartosc bierzemy!
//				val=tab[pos]*scale; //rozciagam histogram na zakres 0-255;
//
//				if (val>max) max=val; if (val<min) min=val;
//				hist[val]++;
//				
//				realTab[count]=pos;
//				sum+=val;
//				sum2+=val*val;
//				
//				count++;
//
//			}
//		}
//	}
//
//	if (count>0)
//	{
//		double stddev;
//		double mean=(double)sum/(double(count));
//		double mean2=(double)sum2/(double)(count);
//
//		stddev=mean2-mean*mean;
//
//		double skewness=0;
//		double kurtosis=0;
//
//		double *dTab=new double[count];
//		for(i=0;i<count;i++) 
//		{
//			dTab[i]=tab[realTab[i]];
//		}
//
//		CalculateSkewNessAndKurtosis(dTab,count,skewness,kurtosis);
//		delete []dTab;
//
//
//
//		stats[0]=min;
//		stats[1]=max;
//		stats[2]=mean;
//		stats[3]=stddev;
//		stats[4]=skewness;
//		stats[5]=kurtosis;
//
//
//		if (showBounds)
//		{
//			int *dilatePoints=tmpTab[1];
//			int kernelType=1;
//			int kernelSize=0;
//			int size=1;
//			int tmp;
//			RawDataSet *rData=new RawDataSet(nx,ny,1,1,1,0,false);
//
//			int *kernel=GetGrowingKernel2D(kernelSize,size, rData,kernelType);
//			
//			int dsizemin1=rData->GetVolumeSize()-1;
//
//			int x,y;
//			
//			int nrOfDilatePoints=0;
//			double nnx1d=1.0/nx;
//			
//			for(i=0;i<count;i++)
//			{
//				for(int kk=0;kk<kernelSize;kk++)
//				{
//					tmp=realTab[i]+kernel[kk];
//					if ((tmp>dsizemin1)||(tmp<0))	{		continue;		}	
//					else	
//					{	if (mask[tmp]==0)
//						{
//							y=realTab[i]*nnx1d;
//							x=realTab[i]-y*nx;
//							//wspolrzedne konturu
//							dilatePoints[nrOfDilatePoints]=x;
//							dilatePoints[nrOfDilatePoints+1]=y;
//							nrOfDilatePoints+=2;
//
//							break;
//						}
//					}
//				}
//			}
//			nrOfDilatePoints/=2;
//		
//			delete []kernel;
//			delete rData;
//
//			//mam zebrane pkty konturu teraz je pokaze
//			
//			MyShape shape;
//			shape.polyColor.setRgb(255,0,255);
//					
//			for(int ii=0;ii<nrOfDilatePoints*2;ii+=2)
//			{
//				shape.poly.append(QPointF(dilatePoints[ii],dilatePoints[ii+1]));
//			}
//					
//			shape.representation=0;
//			shapes->shapeList.append(shape);
//
//				
//
//
//		}
//
//
//
//		return true;
//	}
//
//	
//	return false;
//	
//}
//---------------------------------

LayerMaskAdjacentRegionFilter::LayerMaskAdjacentRegionFilter()
{
	filterName = "LayerMaskAdjacentRegionFilter";
	QStringList list;
	list.clear(); list << "false" << "true";

	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);
	AddParam("lefthTh_1", 0, true, 0, 255, 0.01, leftThreshold1ID);
	AddParam("rightthTh_1", 255, true, 0, 255, 0.01, rightThreshold1ID);

	AddParam("lefthTh_2", 0, true, 0, 255, 0.01, leftThreshold2ID);
	AddParam("rightthTh_2", 255, true, 0, 255, 0.01, rightThreshold2ID);

	AddParam("minArea", 0, true, 0, 124288, 1, minAreaID);
	AddParam("maxArea", 0, true, 0, 124288, 1, maxAreaID);



	AddParam("minWidth", 0, true, 0, 1024, 1, minWidthID);
	AddParam("maxWidth", 0, true, 0, 1024, 1, maxWidthID);
	AddParam("minHeight", 0, true, 0, 1024, 1, minHeightID);
	AddParam("maxHeight", 0, true, 0, 1024, 1, maxHeightID);



	list.clear(); list << "false" << "true";


	AddParam("showContour1Points", 0, true, 0, 1, 1, showContour1PointsID, &list);
	AddParam("showContour2Points", 0, true, 0, 1, 1, showContour2PointsID, &list);
	list.clear(); list << "dilate" << "erode";


	//AddParam ("morphology",0,true,0,1,1,morphologyTypeID,&list);
	list.clear(); list << "square" << "circle";
	AddParam("ktype", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	int morphologyTypeID;
	int morphologyKernelTypeID;


	list.clear(); list << "false" << "true";
	AddParam("drawFonts", 1, true, 0, 1, 1, drawFontsID, &list);
	AddParam("CleanShapes", 1, true, 0, 1, 1, cleanShapesID, &list);


	list.clear();
	//list << "Number: "<<"pixel area: " << "box area: " <<"pix/box: "<<"ellipse area: "<<"pix/ellipse " << "D1: " <<"D2";



	AddParam("infoType", 0, true, 0, 7, 1, infoTypeID, &list);

	AddParam("r1", 255, true, 0, 255, 1, r1ID);
	AddParam("g1", 255, true, 0, 255, 1, g1ID);
	AddParam("b1", 255, true, 0, 255, 1, b1ID);
	AddParam("r2", 255, true, 0, 255, 1, r2ID);
	AddParam("g2", 255, true, 0, 255, 1, g2ID);
	AddParam("b2", 255, true, 0, 255, 1, b2ID);
	//


	//	AddParam ("rotate",0,true,0,360,1,rotateID);
	//int shapeID,int toleranceID;

}
//------------------------------------

bool	LayerMaskAdjacentRegionFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{
	bool useStartShapes = true;

	QList<QPolygonF> *slist = &shapes->polygonList;

	if (slist == NULL) return 0;

	int nrPolygons = shapes->polygonList.count();
	if (nrPolygons == 0) return 0;
	if ((sliceNr<0) || (sliceNr >= data->GetNz()) || (shapes == NULL)) return 0;
	//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	int i, j, k;
	bool revth = params[inverseID];
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

	unsigned short maxValue = (mainDataMaxValue > 0) ? mainDataMaxValue : data->GetMaxValue();


	int leftTh1 = params[leftThreshold1ID] * maxValue / 255.0;
	int rightTh1 = params[rightThreshold1ID] * maxValue / 255.0;

	int leftTh2 = params[leftThreshold2ID] * maxValue / 255.0;
	int rightTh2 = params[rightThreshold2ID] * maxValue / 255.0;


	for (i = 0; i<data->GetSliceSize(); i++)
	{
		if ((tab[i] >= leftTh1) && (tab[i] <= rightTh1)) { mask0[i] = val1; c1++; }
		else { mask0[i] = val2; c2++; };
	}


	int sliceSize = data->GetSliceSize();
	int nx = data->GetNx();
	double nx1d = 1.0 / nx;
	int ny = data->GetNy();
	int max = MAX(c1, c2);
	if (max == sliceSize)
	{
		for (i = 0; i<data->GetSliceSize(); i++)
		{
			if (mask0[i] == 2) mask0[i] = 255;
		}
		return 1;
	}

	//zalewam wyspy

	//biegne po wszystkich pixelach i zalewam dwojki
	//zbieram wspolrzedne pktow dodanych do danej wyspy do tablicy
	//sprawdzam parametry
	//jesli ok zamieniam na 1
	//jesli nie zeruje


	int *listS = intBuffers[0];//new int [sliceSize];
	int *listTmp = intBuffers[1];
	int nr0 = 0;


	//make small kernel



	int kernelSize = 8;
	int size = 1;
	int *kernel = new int[kernelSize];
	int *kernelInc = new int[kernelSize];
	int pos = 0;
	for (j = -size; j<size + 1; j++)
	{
		for (i = -size; i<size + 1; i++)
		{
			if (!((i == 0) && (j == 0)))
			{
				kernel[pos] = i + j*nx;
				kernelInc[pos++] = j;
			}

		}
	}

	int r1 = params[r1ID];
	int g1 = params[g1ID];
	int b1 = params[b1ID];
	int r2 = params[r2ID];
	int g2 = params[g2ID];
	int b2 = params[b2ID];

	/*int r1=255,g1=255,b1=255;
	int r2=255,g2=200,b2=200;*/


	int startPos = 0;
	int ap = 0;
	int lp = 0;
	int count;
	int tmp;
	int dsizemin1 = sliceSize - 1;


	//int minAreaID,maxAreaID,minWidthID,maxWidthID,minHeightID,maxHeightID;


	int minArea = params[minAreaID];
	int maxArea = params[maxAreaID];
	int minWidth = params[minWidthID];
	int maxWidth = params[maxWidthID];
	int minHeight = params[minHeightID];
	int maxHeight = params[maxHeightID];

	if (maxArea == 0) maxArea = sliceSize;
	if (maxWidth == 0) maxWidth = nx;
	if (maxHeight == 0) maxHeight = ny;
	//if (minWidth==0) minWidth=nx;



	int morphologyType = 0;//params[morphologyTypeID];
	int morphologyKernelType = params[morphologyKernelTypeID];

	bool showContour1Points = params[showContour1PointsID];
	bool showContour2Points = params[showContour2PointsID];


	bool drawFonts = params[drawFontsID];

	int infoType = params[infoTypeID];


	bool cleanShapes = params[cleanShapesID];
	shapes->cleanPolygonFlag = cleanShapes;

	int objectNr = 0;
	int *dilatePoints = intBuffers[1];//new int[sliceSize];


	int jStart = 0;
	int jStop = sliceSize;

	QList <int> centerList;
	if (useStartShapes == true)
	{
		//zrob tylko dla srodkow z polygons




		for (int ix = 0; ix<nrPolygons; ix++)
		{

			MyShape s;
			s.poly = slist->at(ix);
			//wez shape i policz srednia x i y;
			if (s.poly.count()>0)
			{
				double center[2] = { 0 };
				for (int ii = 0; ii<s.poly.count(); ii++)
				{
					center[0] += s.poly.at(ii).x();
					center[1] += s.poly.at(ii).y();
				}
				center[0] /= (double)s.poly.count();
				center[1] /= (double)s.poly.count();

				//oblicz pos calkowita

				int posx = (int)(center[0]) + ((int)(center[1]))*nx;

				if ((posx >= 0) && (posx<sliceSize))
				{
					if (mask0[posx] == 2)
					{
						centerList.append(posx);
					}
				}
			}
		}

		if (centerList.count()>0)
		{
			jStop = centerList.count();
		}

	}
	else
	{

	}



	int currPos;

	for (j = jStart; j<jStop; j++)
	{
		int ccc = centerList.count();

		if (ccc>0)
		{
			currPos = centerList.at(j);
		}
		else
		{
			currPos = j;
		}

		if (mask0[currPos] == 2)
		{
			int minx = nx;
			int maxx = 0;
			int miny = ny;
			int maxy = 0;
			int x, y;
			nr0 = 0;
			count = 0;
			ap = currPos;
			lp = 0;
			listS[0] = currPos;
			listTmp[(count++)] = currPos;
			int oldx, oldy;
			//zalewam
			do
			{
				ap = listS[lp--];

				oldy = ap*nx1d;

				for (i = 0; i<kernelSize; i++)
				{
					tmp = ap + kernel[i];
					y = tmp*nx1d;
					x = tmp - y*nx;

					if (!((tmp>dsizemin1) || (tmp<0) || (y != oldy + kernelInc[i]))) //brzydkie
					{
						if (mask0[tmp] == 2)
						{
							mask0[tmp] = 3;
							listS[++lp] = tmp;
							listTmp[count++] = tmp;



							if (x<minx) minx = x;
							if (x>maxx) maxx = x;
							if (y<miny) miny = y;
							if (y>maxy) maxy = y;
						}
					}
				}
			} while (lp >= 0);
			int width = maxx - minx;
			int height = maxy - miny;
			//int width = params[5];
			//int height = params[6];
			/*	int params[7];
			RegionGrowingOnMask2(currPos, mask0, data, 2, 3, kernel, kernelInc, kernelSize, params, listS, listTmp);

			double nx1d = 1.0 / nx;

			count = params[0];
			int minx=params[1];
			int maxx=params[2];
			int miny=params[3];
			int maxy=params[4];

			int x, y;
			*/

			if ((count<minArea) || (count>maxArea) || (width<minWidth) || (width>maxWidth) || (height<minHeight) || (height>maxHeight))
			{


				//wyczysc
				for (i = 0; i<count; i++) { mask0[listTmp[i]] = 0; }	//{	mask0[listS[dsizemin1-i]]=0;}

			}
			else //sprawdz width i height - jesli zle to wyczysc jesli ok to zamaluj na 1
			{
				//if ((shape>0)&&(shape<4))
				{
					int pixelArea = count;
					/*if (showOrigBound)
					{
					MyShape shape;
					shape.polyColor.setRgb(0,255,0);
					shape.poly.append(QPointF(minx,miny));
					shape.poly.append(QPointF(maxx,miny));
					shape.poly.append(QPointF(maxx,maxy));
					shape.poly.append(QPointF(minx,maxy));

					shape.representation=2;
					shapes->shapeList.append(shape);
					}*/


					//ekstrakcja konturu
					int counterek = 0;
					//nowe dane
					{
						int nnx = maxx - minx + 1;
						int nny = maxy - miny + 1;

						int padd = 2;
						int nSizeX = nnx + padd * 2;
						int nSizeY = nny + padd * 2;
						int nSsize = nSizeX*nSizeY;

						int padd1 = 1;

						int nSizeX1 = nnx + padd1 * 2;
						int nSizeY1 = nny + padd1 * 2;

						//tworze maly zbiorek z danymi aktualnej wyspy powiekszonej o padd z kazdej strony
						unsigned char *tabx = new unsigned char[nSsize];
						memset(tabx, 0, nSsize * sizeof(unsigned char));

						//zmniejszona tablica 
						for (int k = 0; k<count; k++)
						{

							y = listTmp[k] * nx1d;
							x = listTmp[k] - y*nx;

							x += padd - minx;	y += padd - miny;
							tabx[x + y*nSizeX] = 1;

						}

						//tablica wypelniona

						RawDataSet *noneData = new RawDataSet(nSizeX, nSizeY, 1, 1, 1, 1, 1, false);
						int kSize;
						int kerenelSize2 = 0;
						int size2 = 1;
						int kernelType = morphologyKernelType;
						int *kernel2 = GetGrowingKernel2D(kerenelSize2, size2, noneData, kernelType);
						int tmp;
						int dsizemin1n = noneData->GetVolumeSize() - 1;


						int nrOfDilatePoints = 0;

						int startPos = nSizeX*padd1 + padd1;
						bool stop;
						double nnx1d = 1.0 / nSizeX;
						//contur extraction

						int frontVal = morphologyType;
						int backVal = !frontVal;

						int ax, ay;
						for (int jj = 0; jj<nSizeY1; jj++)
						{

							for (int ii = 0; ii<nSizeX1; ii++)
							{
								if (tabx[startPos] == frontVal)
								{
									stop = false;
									for (int kk = 0; kk<kerenelSize2; kk++)
									{

										tmp = startPos + kernel2[kk];
										if ((tmp>dsizemin1n) || (tmp<0)) { continue; }
										else {
											//jakikolwiek zapalony
											if (tabx[tmp] == backVal) {
												stop = true;
												break;
											}
										}


									}
									if (stop)
									{
										y = startPos*nnx1d;
										x = startPos - y*nSizeX;



										//tablica dilatepoints zawiera wspolrzedne konturu globalne
										ax = x + minx - padd;
										ay = y + miny - padd;
										dilatePoints[nrOfDilatePoints] = ax;

										dilatePoints[nrOfDilatePoints + 1] = ay;
										nrOfDilatePoints += 2;
										listS[counterek++] = ax + ay*nx;
									}
								}
								startPos++;
							}
							startPos += 2 * padd1;

						}

						nrOfDilatePoints /= 2;








						//draw dilate points
						if (showContour1Points)

						{
							MyShape shape;
							shape.polyColor.setRgb(r1, g1, b1);





							for (int ii = 0; ii<nrOfDilatePoints * 2; ii += 2)
							{
								shape.poly.append(QPointF(dilatePoints[ii], dilatePoints[ii + 1]));
							}

							shape.representation = 0;
							shapes->shapeList.append(shape);

						}
						delete noneData;
						delete[]tabx;
						delete[]kernel2;
					}
					//biore dilate points i wrzucam je na na mask0 w kolorze 2
					// zalewam po kolei punkty z dilate points
					// jesli zalalem



					//----------------------------------------------------------------------


					int minx = nx;
					int maxx = 0;
					int miny = ny;
					int maxy = 0;
					int x, y;
					nr0 = 0;
					count = 0;
					currPos = listS[0];
					//	ap=currPos;
					lp = counterek - 1;
					//listS[0]=currPos;
					//listTmp[(count++)]=currPos;
					int oldx, oldy;


					for (i = 0; i<lp; i++)
					{
						tmp = listS[i];
						if ((tab[tmp] >= leftTh2) && (tab[tmp] <= rightTh2))
						{
							//	mask0[tmp]=4;
							//	listTmp[count++]=tmp;
						}
					}

					//	zalewam
					do
					{
						ap = listS[lp--];
						//if (mask0[ap]!=0) continue;

						oldy = ap*nx1d;

						for (i = 0; i<kernelSize; i++)
						{
							tmp = ap + kernel[i];
							y = tmp*nx1d;
							x = tmp - y*nx;

							if (!((tmp>dsizemin1) || (tmp<0) || (y != oldy + kernelInc[i]))) //brzydkie
							{
								if (mask0[tmp] == 0)
									if ((tab[tmp] >= leftTh2) && (tab[tmp] <= rightTh2))
									{
										mask0[tmp] = 4;
										listS[++lp] = tmp;
										listTmp[count++] = tmp;



										if (x<minx) minx = x;
										if (x>maxx) maxx = x;
										if (y<miny) miny = y;
										if (y>maxy) maxy = y;
									}
							}
						}
					} while (lp >= 0);

					// mam zalane co trzeba teraz robie kontur

					//***************************
					//***************************
					//***************************
					//***************************
					{
						int nnx = maxx - minx + 1;
						int nny = maxy - miny + 1;

						int padd = 2;
						int nSizeX = nnx + padd * 2;
						int nSizeY = nny + padd * 2;
						int nSsize = nSizeX*nSizeY;

						int padd1 = 1;

						int nSizeX1 = nnx + padd1 * 2;
						int nSizeY1 = nny + padd1 * 2;

						//tworze maly zbiorek z danymi aktualnej wyspy powiekszonej o padd z kazdej strony
						unsigned char *tabx = new unsigned char[nSsize];
						memset(tabx, 0, nSsize * sizeof(unsigned char));

						//zmniejszona tablica 
						for (int k = 0; k<count; k++)
						{

							y = listTmp[k] * nx1d;
							x = listTmp[k] - y*nx;

							x += padd - minx;	y += padd - miny;
							tabx[x + y*nSizeX] = 1;

						}

						//tablica wypelniona

						RawDataSet *noneData = new RawDataSet(nSizeX, nSizeY, 1, 1, 1, 1, 1, false);
						int kSize;
						int kerenelSize2 = 0;
						int size2 = 1;
						int kernelType = morphologyKernelType;
						int *kernel2 = GetGrowingKernel2D(kerenelSize2, size2, noneData, kernelType);
						int tmp;
						int dsizemin1n = noneData->GetVolumeSize() - 1;


						int nrOfDilatePoints = 0;

						int startPos = nSizeX*padd1 + padd1;
						bool stop;
						double nnx1d = 1.0 / nSizeX;
						//contur extraction

						int frontVal = morphologyType;
						int backVal = !frontVal;
						int counterek = 0;
						int ax, ay;
						for (int jj = 0; jj<nSizeY1; jj++)
						{

							for (int ii = 0; ii<nSizeX1; ii++)
							{
								if (tabx[startPos] == frontVal)
								{
									stop = false;
									for (int kk = 0; kk<kerenelSize2; kk++)
									{

										tmp = startPos + kernel2[kk];
										if ((tmp>dsizemin1n) || (tmp<0)) { continue; }
										else {
											//jakikolwiek zapalony
											if (tabx[tmp] == backVal) {
												stop = true;
												break;
											}
										}


									}
									if (stop)
									{
										y = startPos*nnx1d;
										x = startPos - y*nSizeX;



										//tablica dilatepoints zawiera wspolrzedne konturu globalne
										ax = x + minx - padd;
										ay = y + miny - padd;
										dilatePoints[nrOfDilatePoints] = ax;

										dilatePoints[nrOfDilatePoints + 1] = ay;
										nrOfDilatePoints += 2;
										//listS[counterek++]=ax+ay*nx;
									}
								}
								startPos++;
							}
							startPos += 2 * padd1;

						}

						nrOfDilatePoints /= 2;








						//draw dilate points
						if (showContour2Points)

						{
							MyShape shape;
							shape.polyColor.setRgb(r2, g2, b2);





							for (int ii = 0; ii<nrOfDilatePoints * 2; ii += 2)
							{
								shape.poly.append(QPointF(dilatePoints[ii], dilatePoints[ii + 1]));
							}

							shape.representation = 0;
							shapes->shapeList.append(shape);

						}
						delete noneData;
						delete[]tabx;
						delete[]kernel2;
					}

					//***************************
					//***************************
					//***************************
					//***************************


					//----------------------------------------------------------------------





					objectNr++;
				}
				// zamien na 1
				//for(i=0;i<count;i++)	{mask0[listTmp[i]]=255;}	//	mask0[listS[dsizemin1-i]]=1;}



				//}

			}




		}
	}

	//delete []dilatePoints;
	//delete []listS;
	delete[]kernel;
	delete[]kernelInc;


	for (i = 0; i<sliceSize; i++)
	{
		mask0[i] = (mask0[i] == 4) ? 255 : 0;
	}




	return 1;

}

//---------------------

LayerMaskExplodeSegmentationRegionFilter::LayerMaskExplodeSegmentationRegionFilter()
{
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


	filterName = "LayerMaskExplodeSegmentationRegionFilter";
	QStringList list;
	list.clear(); list << "false" << "true";

	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);

	list.clear(); list << "threshold" << "probability";
	AddParam("Mode", 0, true, 0, 1, 1, probOrThreshID, &list);
	list.clear(); list << "false" << "true";

	AddParam("lth", 0, true, 0, 10000, 1, threshold1ID);
	AddParam("rth", 10000, true, 0, 10000, 1, threshold2ID);

	list.clear(); list << "2D Source" << "2D Result" << "3D Source";
	AddParam("sampling3DMode", 0, true, 0, 2, 1, sampling3DModeID, &list);

	AddParam("autoThreshold", 1, true, 0, 1, 1, autoThresholdID, &list);
	//list.clear(); list << "polyline" << "area";
	//AddParam("samplingType", 0, true, 0, 1, 1, samplingTypeID, &list);

	list.clear(); list << "false" << "true";

	AddParam("showMask", 0, true, 0, 1, 1, showMaskID, &list);
	AddParam("segOpacity", 0.5, true, 0.0, 1, 0.001, segOpacityID);


	//	AddParam("useAsMask", 1, true, 1, 6, 1, useAsMaskID);
	//AddParam("updateProbabilityTabs", true, true, 0, 1, 1, updateProbabilityTabsID, &list);

	//	AddParam("showSourceLines",
	AddParam("showStats", 0, true, 0, 1, 1, calculateStatsID, &list);
	AddParam("radiusX", 1, true, 0.1, 50, 0.01, radiusXID);
	AddParam("radiusY", 1, true, 0.1, 50, 0.01, radiusYID);
	AddParam("radiusZ", 1, true, 0.1, 50, 0.01, radiusZID);
	//AddParam("radius2", 10, true, 0.1, 200, 0.01, radius2ID);

	AddParam("angle", 0, true, 0, 360, 1, angleID);
	AddParam("dest", 50, true, 4, 360, 1, destID);

	AddParam("maxDist", 10, true, 1, 512, 1, maxDistID);
	AddParam("median", 1, true, 0, 1, 1, medianID, &list);
	AddParam("medianFactor", 1, true, 0, 1, 0.001, medianFactorID);
	AddParam("medianKernel", 2, true, 1, 20, 1, medianKernelID);
	AddParam("medianKernel3D", 2, true, 1, 100, 1, medianKernel3DID);
	AddParam("dest3D", 4, true, 2, 6, 1, dest3DID);

	AddParam("medianThreshold", 10, true, 1, 512, 1, medianThresholdID);

	list.clear(); list << "square" << "circle";
	AddParam("ktype", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	AddParam("dilateDist", 2, true, 0, 20, 1, dilateDistID);
	AddParam("closingSize3D", 2, true, 0, 4, 1, closingSizeID);



	list.clear(); list << "false" << "true";
	AddParam("useBigMask", 1, true, 0, 1, 1, useBigMaskID, &list);

	list.clear(); list << "clear" << "add";
	AddParam("bigMaskMode", 0, true, 0, 1, 1, bigMaskModeID, &list);

	AddParam("propPercentCoarse", 0.8, true, 0.0, 0.9, 0.1, probabilityDensityPercentageCoarseID);
	AddParam("propPercentFine", 0.0, true, 0.0, 0.09999, 0.0001, probabilityDensityPercentageFineID);
	AddParam("sigma", 1.5, true, 0.01, 6, 0.01, sigmaID);
	AddParam("intensityFactor", 0.5, true, 0.001, 1, 0.001, intensityFactorID);


	list.clear(); list << "false" << "true";
	AddParam("onlyShortening", 0, true, 0, 1, 1, onlyShorteningID, &list);
	AddParam("ellipseRayDest", 0, true, 0, 1, 1, ellipseRayDestID, &list);



	AddParam("showContour1Points", 0, true, 0, 1, 1, showContour1PointsID, &list);
	list.clear(); list << "dilate" << "erode";


	//AddParam ("morphology",0,true,0,1,1,morphologyTypeID,&list);
	//list.clear(); list << "square" << "circle";
	//AddParam("ktype", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	int morphologyTypeID;
	int morphologyKernelTypeID;


	list.clear(); list << "false" << "true";
	AddParam("drawFonts", 1, true, 0, 1, 1, drawFontsID, &list);
	//	AddParam("CleanShapes", 1, true, 0, 1, 1, cleanShapesID, &list);


	list.clear();
	//list << "Number: "<<"pixel area: " << "box area: " <<"pix/box: "<<"ellipse area: "<<"pix/ellipse " << "D1: " <<"D2";



	AddParam("infoType", 0, true, 0, 7, 1, infoTypeID, &list);

	AddParam("r1", 255, true, 0, 255, 1, r1ID);
	AddParam("g1", 255, true, 0, 255, 1, g1ID);
	AddParam("b1", 255, true, 0, 255, 1, b1ID);

	AddParam("fontSize", 15, true, 4, 100, 1, fontSizeID);
	list.clear(); list << "center" << "left" << "right" << "top" << "bottom";
	AddParam("showInfoPlace", 1, true, 0, 4, 1, showInfoPlaceID, &list);

}
//----------------------------------------------
LayerMaskExplodeSegmentationRegionFilter::~LayerMaskExplodeSegmentationRegionFilter()
{
	if (expTab != NULL) delete[]expTab;
	if (samplingTab1 != NULL)	delete[]samplingTab1;
	if (samplingTabIndex != NULL)	delete[]samplingTabIndex;

	if (samplingTab != NULL)	delete[]samplingTab;



}
//------------------------------------
bool	LayerMaskExplodeSegmentationRegionFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{

	//return 0;

	double mSigma = params[sigmaID];
	bool useStartShapes = true;

	QList<QPolygonF> *slist = &shapes->polygonList;

	if (slist == NULL) return 0;

	int nrPolygons = shapes->polygonList.count();

	if ((sliceNr < 0) || (sliceNr >= data->GetNz()) || (shapes == NULL)) return 0;
	//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	//	int  j, k;
	revth = params[inverseID];
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
	leftTh = params[threshold1ID] * mainDataMaxValue *0.0001;;
	rightTh = params[threshold2ID] * mainDataMaxValue *0.0001;

	shapes->cleanPolygonFlag = true;
	dest3D = params[dest3DID];

	mode = params[probOrThreshID];//prob 0 thresh 1


	unsigned short maxValue = (mainDataMaxValue > 0) ? mainDataMaxValue : data->GetMaxValue();

	//int mmmm = data->GetMaxValue();

	int n[3];
	int sliceSize = data->GetSliceSize();
	data->GetN(n);





	double center[2] = { data->GetNx() / 2, data->GetNy() / 2 };
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
	double angle = params[angleID];

	shape2.ellipses.append(QPointF(centerE[0], centerE[1]));//center
	shape2.ellipses.append(QPointF(r1, r2));//rads
	shape2.ellipses.append(QPointF(-angle, 50));

	//	int col1[] = { params[r1ID], params[g1ID], params[b1ID] };

	shape2.ellipsesColor.setRgb(params[r1ID], params[g1ID], params[b1ID]);

	shapes->shapeList.append(shape2);







	// mapa prawdopodobienstwa

	samplesCount = 0;


	sampling3DMode = params[sampling3DModeID];


	double alpha = angle* to_rad;			double ca = cos(alpha);		double sa = sin(alpha);

	//	double scaleF = (maxValue < 4096) ? 1.0 : 4095.0 / maxValue;
	//	double scaleF = 4095.0 / maxValue;


	bool sigmaChg = fabs(mSigma - sigma) < 1e-5 ? false : true;

	bool intensChg = fabs(intensityFactor - params[intensityFactorID])< 1e-5 ? false : true;
	intensityFactor = params[intensityFactorID];

	if ((expMax != maxValue + 1) || (sigmaChg == true) || (intensChg == true))
	{

		sigma = mSigma;
		if (expTab != NULL)delete[]expTab;
		expMax = maxValue + 1;
		expTab = new double[expMax * 2 + 1];

		if (samplingTab1 != NULL)delete[]samplingTab1;
		samplingTab1 = new unsigned short[expMax];
		if (samplingTabIndex != NULL)delete[]samplingTabIndex;
		samplingTabIndex = new unsigned short[expMax];


		if (samplingTab != NULL)delete[]samplingTab;
		samplingTab = new unsigned short[expMax];



		//intensityFactor = 1.0 / (double)(expMax*expMax);
		GetExpTab(expTab, expMax, sigma, intensityFactor);

	}

	memset(samplingTab, 0, expMax * sizeof(unsigned short));
	memset(samplingTab1, 0, expMax * sizeof(unsigned short));



	bool useBigMask = params[useBigMaskID];
	int bigMaskMode = params[bigMaskModeID];


	int valx=0;
	int minValx = 65535; int maxValx = 0;

	//	RawDataSet *newData = new RawDataSet(data->GetNx(), data->GetNy(), 1);
	//newData->ZeroQ();
	int posxx = 0;




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
				}

				//	newData->GetDataArray()[posxx] = valx; 

				if (minValx > valx)minValx = valx;
				if (maxValx<valx) maxValx = valx;

			}
			posxx++;
		}
		//val=

	}
	//newData->SaveRawDataSet2("f:\\x\\test");

	if (samplesCount == 0) return false;

	bool autoThresh = params[autoThresholdID];
	if (autoThresh)
	{
		leftTh = minValx;
		rightTh = maxValx;
	}



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
																																					  //double factor1 = (max1 - min1 != 0) ? 1.0 / (max1 - min1) : 1;
	double factor1 = (max1 != 0) ? 1.0 / (max1) : 1;
	double percCoarse = params[probabilityDensityPercentageCoarseID]; //0-100%
	double percFine = params[probabilityDensityPercentageFineID]; //0-100%

	perc = percCoarse + percFine;



	QPolygonF testPolyMask;

	QElapsedTimer tm;
	tm.start();
	unsigned int t1 = tm.elapsed();


	bool showMask = params[showMaskID];



	double dest = params[destID];
	maxDist = params[maxDistID];

	alpha = -angle* to_rad;
	ca = cos(alpha);
	sa = sin(alpha);

	double step = 360 / dest;
	double t;
	double st, ct;
	double x, y;
	double dx, dy, len;
	int x1, y1;
	int val;
	bool test1;
	QPolygonF testPoly;



	double *rayTab = new double[10000];
	int nrRays = 0;

	double r0 = 1;

	bool ellipseRayDest = params[ellipseRayDestID];
	equalRadius = ellipseRayDest;
	double rr1 = r1;
	double rr2 = r2;
	if (ellipseRayDest)
	{
		rr1 = r0;
		rr2 = r0;
	}


	for (int jj = 0; jj < 360; jj += step)
	{
		t = jj* to_rad;
		ct = cos(t);
		st = sin(t);

		x = centerE[0] + rr1 * ct*ca - rr2 * st*sa;
		y = centerE[1] + rr1 * ct*sa + rr2 * st*ca;


		dx = x - centerE[0];
		dy = y - centerE[1];
		double len = sqrt(dx*dx + dy*dy);
		if (len > 0)
		{
			dx /= len;
			dy /= len;


			rayTab[nrRays * 4] = x;
			rayTab[nrRays * 4 + 1] = y;
			rayTab[nrRays * 4 + 2] = dx;
			rayTab[nrRays * 4 + 3] = dy;
			nrRays++;
		}


	}


	SegCriterion *cr;


	int neighSize = 2;
	int neighType = 0;//0-cube 1-sphere
	double neighPercentage = 0.3;

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




	double *lengthTab = new double[nrRays];
	double *lengthTab2 = new double[nrRays];
	double minRayStep = 1;// MIN(mdx, mdy)*0.5;
	for (int jj = 0; jj < nrRays; jj++)
		//for (int jj = 0; jj < 360; jj += step)
	{

		{
			x = rayTab[jj * 4];
			y = rayTab[jj * 4 + 1];
			dx = rayTab[jj * 4 + 2];
			dy = rayTab[jj * 4 + 3];

			bool test = true;
			double ii;
			double maxDistD = maxDist / minRayStep;
			for (ii = 0; ii < maxDistD; ii += minRayStep)
			{
				x1 = x + dx*ii;
				y1 = y + dy*ii;

				if (x1<0 || y1<0 || x1>n[0] - 1 || y1>n[1] - 1)
				{
					ii -= 1; break;// test = false; break;
				}

				int ppos = x1 + y1*n[0];


				if ((useBigMask == true) && (mainMask != NULL))
				{
					if (mainMask[ppos] == 0)
					{
						ii -= 1;
						break;
					}
				}


				test1 = (cr->CheckPoint(0, 0, 0, NULL, 0, ppos) > 0);

				if (test1 == false)
				{

					break;
				}

			}


			if ((test == true) && (ii >= 0))
			{
				lengthTab[jj] = ii;

			}


		}

	}

	//ewentualne filtrowanie
	bool medianLines = params[medianID]; exportParams[0] = params[medianID];
	int medianKernel = params[medianKernelID]; exportParams[1] = params[medianKernel3DID]; // bo nie dziala tak samo !!!
	double medianFactor = params[medianFactorID]; exportParams[2] = params[medianFactorID];



	//trzeba dodac rozmiar kernela do filtra medianowego

	if (medianLines == true)
	{
		bool onlyShortening = params[onlyShorteningID]; exportParams[3] = params[onlyShorteningID];
		int medianThreshold = params[medianThresholdID]; exportParams[4] = params[medianThresholdID];
		double *medTab = new double[medianKernel * 2 + 1];

		//filtruje
		for (int jj = 0; jj < nrRays; jj++)
		{
			for (int j = 0; j < medianKernel * 2 + 1; j++)
			{
				int pos = (jj + nrRays + j - medianKernel) % nrRays;
				//if ((pos >= 0) && (pos<newNrPts))
				{
					medTab[j] = lengthTab[pos];
				}
			}


			double med = Median(medTab, medianKernel * 2);
			lengthTab2[jj] = med - lengthTab[jj]; //roznica
		}
		//podmieniam z wagami
		int lenx;
		for (int i = 0; i < nrRays; i++)
		{
			lenx = ((fabs(lengthTab2[i]) < medianThreshold) ? 0 : medianFactor*lengthTab2[i]) + lengthTab[i];// (1.0 - medianFactor)*lengthTab2[i + medianKernel];


			lengthTab[i] = (!onlyShortening) ? lenx : (lenx < lengthTab[i]) ? lenx : lengthTab[i]; //wstawiam to co wyszlo z median chyba ze mam tylko skracanie to sprawdzam czy krotszy przed zamiana
		}
		delete[]medTab;
	}


	//po przefiltrowaniu
	for (int jj = 0; jj < nrRays; jj++)
	{


		testPoly.append(QPoint(centerE[0], centerE[1]));

		x = rayTab[jj * 4];
		y = rayTab[jj * 4 + 1];
		dx = rayTab[jj * 4 + 2];
		dy = rayTab[jj * 4 + 3];
		x1 = x + dx*lengthTab[jj];
		y1 = y + dy*lengthTab[jj];
		testPoly.append(QPoint(x1, y1));
		testPolyMask.append(QPoint(x1, y1));

	}


	if (testPoly.count() > 0)
	{
		MyShape s;
		//s.name = "areaPolys";
		s.poly = testPoly;
		s.representation = 3;
		s.polyColor.setRgb(170, 0, 255);
		shapes->shapeList.append(s);
	}

	if (showMask == true)
	{
		MyShape s;
		s.name = "areaPolys";
		s.poly = testPolyMask;
		s.representation = 2;
		s.opacity = params[segOpacityID];
		s.polyColor.setRgb(255, 0, 0);
		shapes->shapeList.append(s);

	}

	delete[]rayTab;
	delete[]lengthTab;
	delete[]lengthTab2;

	//zbierze segmentacje do maski

	int nx = data->GetNx();
	int ny = data->GetNy();
	QImage img(nx, ny, QImage::Format_RGB32);
	img.fill(Qt::black);
	QPainter Painter2(&img);
	Painter2.setBrush(Qt::white);
	Painter2.setPen(1);

	Painter2.drawPolygon(testPolyMask);


	//img.save("d:\\pic2.png", "PNG");
	//namalowane - zbieram do wlasciwego
	unsigned int *tabImg = (unsigned int *)img.bits();;
	unsigned char *dilateBuffer1 = new unsigned char[sliceSize];
	memset(dilateBuffer1, 0, sliceSize);

	int minx = n[0];
	int maxx = 0;
	int miny = n[1];
	int maxy = 0;
	int ix, iy;
	double nx1d = 1.0 / n[0];
	for (int i = 0; i < nx*ny; i++)
	{

		unsigned char val = ((*tabImg++) == 0xffffffff) ? 1 : 0;
		mask0[i] = val;

		if (val == 1)
		{
			iy = i*nx1d;
			ix = i - iy*n[0];

			if (ix<minx) minx = ix;
			if (ix>maxx) maxx = ix;
			if (iy<miny) miny = iy;
			if (iy>maxy) maxy = iy;
		}
	}


	//morph
	int kerenelSize2 = 0;
	int dilateDist = params[dilateDistID]; exportParams[5] = params[dilateDistID];


	int size2 = 1;
	int kernelType = params[morphologyKernelTypeID]; exportParams[6] = params[morphologyKernelTypeID];
	int closingSize = params[closingSizeID]; exportParams[7] = params[closingSizeID];

	//int *kernel2 = GetGrowingKernel2D(kerenelSize2, size2, data, kernelType);
	int tmp;
	int dsizemin1n = n[0] * n[1] - 1;


	int nrOfDilatePoints = 0;


	bool stop;
	double nnx1d = 1.0 / n[0];
	//contur extraction

	//int frontVal = 0;
	//int backVal = 1;

	int ax, ay;

	int minx1 = minx - dilateDist; if (minx1 < 0) minx1 = 0;
	int miny1 = miny - dilateDist; if (miny1 < 0) miny1 = 0;
	int maxx1 = maxx + dilateDist; if (maxx1 > n[0] - 1) maxx1 = n[0] - 1;
	int maxy1 = maxy + dilateDist; if (maxy1 > n[1] - 1) maxy1 = n[1] - 1;
	bool test2;


	int bounds[6] = { minx1, maxx1, miny1, maxy1, 0, 0 };
	unsigned char *blockMask = NULL;
	if ((useBigMask == true) && (mainMask != NULL)) blockMask = mainMask;
	bool is2d = true;
	for (int kk = 0; kk < dilateDist; kk++)
	{
		SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 1, 1, kernelType, NULL, bounds, blockMask, cr, is2d, 0); //z criterion na danych
	}
	if (closingSize > 0)
	{

		SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 1, closingSize, kernelType, NULL, NULL, blockMask, NULL, is2d, 0);//domykanie bez brania pod uwage danych
		SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 0, closingSize, kernelType, NULL, NULL, NULL, NULL, is2d);
	}

	//delete[]kernel2;

	//---------------
	//show dilate points


	int *listS = intBuffers[0];//new int [sliceSize];
	int *listTmp = intBuffers[1];
	int nr0 = 0;


	//make small kernel



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
	for (int i = 0; i < nx*ny; i++)
	{
		if (mask0[i] == 1)
		{
			currPos = i;
			break;
		}
	}
	int prms[7];
	bool calculateStats = params[calculateStatsID];
	double paramsx[10];


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
	if (currPos >= 0)
	{

		RegionGrowingOnMask2(currPos, mask0, data, 1, 255, kernel, kernelInc, kernelSize, prms, listS, listTmp);
		double nx1d = 1.0 / n[0];

		int count = prms[0];
		int minx = prms[1];
		int maxx = prms[2];
		int miny = prms[3];
		int maxy = prms[4];
		int width = prms[5];
		int height = prms[6];
		int col1[] = { params[r1ID], params[g1ID], params[b1ID] };

		//wyczyszcze samplingTab, zrobie nowe z aktualnymi samplami!
		memset(samplingTab, 0, expMax * sizeof(unsigned short));
		samplesCount = 0;

		if (count > 1)
		{
			int morphologyType = 1;//params[morphologyTypeID];
			int morphologyKernelType = 0;// params[morphologyKernelTypeID];
			int adjacentDist = 1;
			int *dilatePoints = intBuffers[1];//new int[sliceSize];

			if (calculateStats == true)
			{
				GetStatisticsFromVolumeSampledTab(data, listTmp, count, paramsx);
			}

			MyShape shape = createDilateConturPoints(prms, data, listTmp, morphologyKernelType, morphologyType, dilatePoints, col1, 1, dilateBuffer1, adjacentDist); //dilateBuffer zamalowany dylacja
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
	}

	if (sampling3DMode == 1) //kopiuje aktualny stan do tablicy uShortTabMixera - zaznaczenie pelna segmentacja
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
	//************************
	if (calculateStats == true)
	{
		int fontSize = params[fontSizeID];
		MyShape shape;
		QString text;
		text = "lth: " + QString::number(leftTh) + "   " + "rth: " + QString::number(rightTh) + "\n" +
			"min: " + QString::number(paramsx[0]) + "   " + "max: " + QString::number(paramsx[1]) + "   " + "\n" +
			"mean: " + QString::number(paramsx[2]) + "   " + "stddev: " + QString::number(paramsx[3]) + "   " + "\n" +
			"skewness: " + QString::number(paramsx[4]) + "   " + "kurtosis: " + QString::number(paramsx[5]) + "   " + "\n" +
			"voxelCount: " + QString::number(paramsx[6]) + "   " + "area: " + QString::number(paramsx[7]) + "   " + " mm^2\n";


		double xPos = 10;// params[textXPosID] * nx;
		double yPos = 10;// params[textYPosID] * ny;


		int		showInfoPlace = params[showInfoPlaceID];

		//MyShape shape;	
		shape.shapeText = QString(text);
		shape.textColor.setRgb(255, 33, 0);
		//QRectF rect(10, 10, mnx-10, mny-10);
		//shape.objectBounds = rect;
		//shape.textAutoPos = showInfoPlace;
		shape.textPos.setX(xPos);
		shape.textPos.setY(yPos);//fSize*1.2);
		double fSize = fontSize;
		shape.font.setPixelSize(fSize);


		shapes->shapeList.append(shape);


		//	double fSize = fontSize;
		//	shape.shapeText = QString(text);
		//	shape.textPos.setX(xPos);
		//	shape.textPos.setY(yPos);//fSize*1.2);
		//	shape.font.setPixelSize(fSize);
		//	//shape.textAngle=-minPos;
		////	shape2.ellipsesColor.setRgb(params[r1ID], params[g1ID], params[b1ID]);
		//	shape.textColor.setRgb(params[r1ID], params[g1ID], params[b1ID]);
		//	shape.textAutoPos = showInfoPlace;
		//	shapes->shapeList.append(shape);

	}

	delete cr;
	delete[]kernel;
	delete[]kernelInc;

	delete[]dilateBuffer1;
	delete[]intensityProbabilityTab1;
	unsigned int t2 = tm.elapsed();
	//qWarning() << "reszta: " << t2 - t1;

	//zrob dylatacje
	//podziel na wyspy
	//sprawdz jakie wyspy spelniaja kryterium rozmiaru
	//wystrzel kolejne segmentacje wzdluz promieni

	memset(mask0, 255, mnx*mny * sizeof(unsigned char));
	return true;
}
//---------------------------------

LayerMaskProbabilityAdjacentRegionFilter::LayerMaskProbabilityAdjacentRegionFilter()
{

	samplesCount2 = 0;
	samplesCount1 = 0;

	sigma = 1.5;
	expMax = 4096;
	samplingTab1 = new unsigned short[expMax];
	samplingTab2 = new unsigned short[expMax];
	memset(samplingTab1, 0, expMax * sizeof(unsigned short));
	memset(samplingTab2, 0, expMax * sizeof(unsigned short));
	intensityFactor = 1.0 / (double)(expMax*expMax);
	//intensityFactor2=intensityFactor*intensityFactor;

	expTab = new double[2 * expMax + 2];
	GetExpTab(expTab, expMax, sigma, intensityFactor);
	filterName = "LayerMaskProbabilityAdjacentRegionFilter";
	QStringList list;
	list.clear(); list << "false" << "true";

	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);
	AddParam("currentShapeEdit", 1, true, 1, 2, 1, currentShapeEditID);
	//AddParam("lefthTh_1", 0, true, 0, 255, 0.01, leftThreshold1ID);
	//AddParam("rightthTh_1", 255, true, 0, 255, 0.01, rightThreshold1ID);

	//AddParam("lefthTh_2", 0, true, 0, 255, 0.01, leftThreshold2ID);
	//AddParam("rightthTh_2", 255, true, 0, 255, 0.01, rightThreshold2ID);

	list.clear(); list << "polyline" << "area";
	AddParam("samplingType", 0, true, 0, 1, 1, samplingTypeID, &list);

	list.clear(); list << "false" << "true";
	AddParam("onlyAdjacent", true, true, 0, 1, 1, onlyAdjacentID, &list);
	AddParam("useAsMask", 1, true, 1, 6, 1, useAsMaskID);
	AddParam("adjacentDist", 0, true, 0, 6, 1, adjacentDistID);
	AddParam("cleanShape", true, true, 0, 1, 1, cleanShapesID, &list);
	AddParam("updateProbabilityTabs", true, true, 0, 1, 1, updateProbabilityTabsID, &list);

	AddParam("showSourceLines", true, true, 0, 1, 1, showSourceLinesID, &list);


	AddParam("propPercent1", 0.85, true, 0.7, 0.9999, 0.001, probabilityDensityPercentage1ID);
	AddParam("propPercent2", 0.85, true, 0.7, 0.9999, 0.001, probabilityDensityPercentage2ID);

	AddParam("minArea1", 0, true, 0, 124288, 1, minArea1ID);
	AddParam("maxArea1", 0, true, 0, 124288, 1, maxArea1ID);
	AddParam("minWidth1", 0, true, 0, 1024, 1, minWidth1ID);
	AddParam("maxWidth1", 0, true, 0, 1024, 1, maxWidth1ID);
	AddParam("minHeight1", 0, true, 0, 1024, 1, minHeight1ID);
	AddParam("maxHeight1", 0, true, 0, 1024, 1, maxHeight1ID);

	AddParam("minArea2", 0, true, 0, 124288, 1, minArea2ID);
	AddParam("maxArea2", 0, true, 0, 124288, 1, maxArea2ID);
	AddParam("minWidth2", 0, true, 0, 1024, 1, minWidth2ID);
	AddParam("maxWidth2", 0, true, 0, 1024, 1, maxWidth2ID);
	AddParam("minHeight2", 0, true, 0, 1024, 1, minHeight2ID);
	AddParam("maxHeight2", 0, true, 0, 1024, 1, maxHeight2ID);
	list.clear(); list << "false" << "true";


	AddParam("showContour1Points", 0, true, 0, 1, 1, showContour1PointsID, &list);
	AddParam("showContour2Points", 0, true, 0, 1, 1, showContour2PointsID, &list);
	list.clear(); list << "dilate" << "erode";


	//AddParam ("morphology",0,true,0,1,1,morphologyTypeID,&list);
	list.clear(); list << "square" << "circle";
	AddParam("ktype", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	/*int morphologyTypeID;
	int morphologyKernelTypeID;
	*/

	list.clear(); list << "false" << "true";
	AddParam("drawFonts", 1, true, 0, 1, 1, drawFontsID, &list);
	//	AddParam("CleanShapes", 1, true, 0, 1, 1, cleanShapesID, &list);


	list.clear();
	//list << "Number: "<<"pixel area: " << "box area: " <<"pix/box: "<<"ellipse area: "<<"pix/ellipse " << "D1: " <<"D2";



	AddParam("infoType", 0, true, 0, 7, 1, infoTypeID, &list);

	AddParam("r1", 255, true, 0, 255, 1, r1ID);
	AddParam("g1", 255, true, 0, 255, 1, g1ID);
	AddParam("b1", 255, true, 0, 255, 1, b1ID);
	AddParam("r2", 255, true, 0, 255, 1, r2ID);
	AddParam("g2", 255, true, 0, 255, 1, g2ID);
	AddParam("b2", 255, true, 0, 255, 1, b2ID);
	AddParam("fontSize", 15, true, 4, 100, 1, fontSizeID);

}
//------------------------------------

bool	LayerMaskProbabilityAdjacentRegionFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{
	bool useStartShapes = true;

	QList<QPolygonF> *slist = &shapes->polygonList;

	if (slist == NULL) return 0;

	int nrPolygons = shapes->polygonList.count();
	if (nrPolygons == 0) return 0;
	if ((sliceNr<0) || (sliceNr >= data->GetNz()) || (shapes == NULL)) return 0;
	//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	int i, j, k;
	bool revth = params[inverseID];
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
	//int leftTh1 = params[leftThreshold1ID] * data->GetMaxValue() / 255.0;
	//int rightTh1 = params[rightThreshold1ID] * data->GetMaxValue() / 255.0;

	//int leftTh2 = params[leftThreshold2ID] * data->GetMaxValue() / 255.0;
	//int rightTh2 = params[rightThreshold2ID] * data->GetMaxValue() / 255.0;


	//for (i = 0; i<data->GetSliceSize(); i++)
	//{
	//	if ((tab[i] >= leftTh1) && (tab[i] <= rightTh1))	{ mask0[i] = val1; c1++; }
	//	else	{ mask0[i] = val2; c2++; };
	//}

	//robie prob mape dla shape0 i shape1

	// robie maski osobno dla mask0 i mask1
	// wykrywam wyspy o zadanych parametrach na obu
	//zostawiam do wizualizacji i obliczen tylko takie ktore zostaly i stykaja sie z druga maska
	bool showSourceLines = params[showSourceLinesID];

	shapes->cleanPolygonFlag = false;

	int currencShapeEdit = params[currentShapeEditID];
	bool clear = params[cleanShapesID];

	//int nrPolygons = 0;

	//nrPolygons = shapes->polygonList.count();

	//QList<QPolygonF> *slist = &shapes->polygonList;
	bool testx = false;
	if (clear == true)//wywal wszystkie z numerem currentShapeEdit
	{
		//sprawdze najpierw czy jest jakis==0 - jak nie spadam!

		for (i = 0; i < nrPolygons; i++)
		{
			int flag = shapes->polygonFlags.at(i);
			if (flag == 0)
			{
				testx = true;
				break;
			}
		}
		if (testx == true)
		{


			for (int i = nrPolygons - 1; i >= 0; i--)
			{

				MyShape s;
				s.poly = slist->at(i);;

				int flag = shapes->polygonFlags.at(i);

				if (flag == currencShapeEdit)
				{

					shapes->polygonList.removeAt(i);
					shapes->polygonFlags.removeAt(i);
				}
				//	s.polyColor.setRgb(0, 255, 0);
				//shapes->shapeList.append(s);
			}
		}
	}


	nrPolygons = slist->count();
	//ustlam zero na aktualna currentShapeEdit
	for (i = 0; i < nrPolygons; i++)
	{
		int flag = shapes->polygonFlags.at(i);
		if (flag == 0)
		{
			shapes->polygonFlags[i] = currencShapeEdit;
		}
	}


	for (i = 0; i < nrPolygons; i++)
	{
		int flag = shapes->polygonFlags.at(i);
		QRgb col;
		if (flag == 1)
		{
			col = qRgb(255, 0, 0);
		}
		else if (flag == 2)
		{
			col = qRgb(0, 255, 0);
		};
		if (flag == 0) continue;

		MyShape s;
		s.poly = slist->at(i);

		s.polyColor.setRgb(col);
		shapes->shapeList.append(s);

	}


	// jesli jest i 1 i 2 to ide dalej

	int tt[3] = { 0 };
	for (i = 0; i < nrPolygons; i++)
	{
		int flag = shapes->polygonFlags.at(i);
		tt[flag]++;

		shapes->shapeList[i].visible = showSourceLines;

	}

	if (tt[1] * tt[2] == 0) return 0;




	int adjacentDist = params[adjacentDistID];
	//czas zrobic obie statystyki

	bool updateProbabilityTabs = params[updateProbabilityTabsID];
	//unsigned short *samplingTab1 = new unsigned short[expMax];
	//unsigned short *samplingTab2 = new unsigned short[expMax];
	double scaleF = (data->GetMaxValue() < 4096) ? 1.0 : 4095.0 / data->GetMaxValue();
	int n[3];
	int sliceSize = data->GetSliceSize();
	data->GetN(n);
	int posx = 0;
	if (testx == true)//tylko zmieniam updates jesli cos nowego wlasnie narysowalem
		if ((updateProbabilityTabs == true) || (samplesCount1 == 0) || (samplesCount2 == 0))
		{

			unsigned short * tt = (currencShapeEdit == 1) ? samplingTab1 : samplingTab2;
			//memset(samplingTab1, 0, expMax*sizeof(unsigned short));
			//	memset(samplingTab2, 0, expMax*sizeof(unsigned short));
			memset(tt, 0, expMax * sizeof(unsigned short));

			// samplesCount1 = 0;
			// samplesCount2 = 0;
			int &samplesCount = (currencShapeEdit == 1) ? samplesCount1 : samplesCount2;

			samplesCount = 0;
			int type = params[samplingTypeID];
			if (type == 0)
			{

				for (j = 0; j < nrPolygons; j++)
				{
					MyShape s;
					s.poly = slist->at(j);



					// spr ilosc pktow
					int counter = 0;
					QPolygonF testPoly;
					int flag = shapes->polygonFlags.at(j);
					if (flag == currencShapeEdit)
						for (int i = 0; i < s.poly.count(); i++)
						{
							int pos = (int)(s.poly.at(i).y())*n[0] + (int)(s.poly.at(i).x());
							if ((pos >= 0) && (pos < sliceSize))
							{

								//samplingTab[posx++]=tab[pos];
								/* if (flag == 1)
								{

								samplingTab1[(int)(tab[pos] * scaleF)]++;
								samplesCount1++;
								}
								else
								{

								samplingTab2[(int)(tab[pos] * scaleF)]++;
								samplesCount2++;
								}*/
								tt[(int)(tab[pos] * scaleF)]++;
								samplesCount++;
							}
						}
				}
			}
			else
			{
				int nx = data->GetNx();
				int ny = data->GetNy();
				QImage img(nx, ny, QImage::Format_RGB32);
				img.fill(Qt::black);
				QPainter Painter2(&img);
				Painter2.setBrush(Qt::white);
				Painter2.setPen(1);

				for (j = 0; j < nrPolygons; j++)
				{
					MyShape s;
					s.poly = slist->at(j);



					// spr ilosc pktow
					int counter = 0;
					QPolygonF testPoly;
					int flag = shapes->polygonFlags.at(j);
					if (flag == currencShapeEdit)
					{
						//namaluj
						Painter2.drawPolygon(s.poly);

					}
				}
				img.save("d:\\pic2.png", "PNG");
				//namalowane - zbieram do wlasciwego
				unsigned int *tabImg = (unsigned int *)img.bits();;
				for (int i = 0; i < nx*ny; i++)
				{
					unsigned char val = ((*tabImg++) == 0xffffffff) ? 1 : 0;
					if (val == 1)
					{
						tt[(int)(tab[i] * scaleF)]++;
						samplesCount++;
					}
				}
			}


		}
	//zebralem probki

	//teraz prob map!!!
	double *intensityProbabilityTab1 = new double[expMax];
	double min1, max1;
	GetIntensityProbabilityTab(intensityProbabilityTab1, samplesCount1, min1, max1, sigma, expTab, samplingTab1, expMax, expMax, samplesCount1);//ta	sie	przyda
	double factor1 = (max1 != 0) ? 1.0 / (max1) : 1;
	double perc1 = params[probabilityDensityPercentage1ID]; //0-100%

	double *intensityProbabilityTab2 = new double[expMax];
	double min2, max2;
	GetIntensityProbabilityTab(intensityProbabilityTab2, samplesCount2, min2, max2, sigma, expTab, samplingTab2, expMax, expMax, samplesCount2);//ta	sie	przyda
	double factor2 = (max2 != 0) ? 1.0 / (max2) : 1;
	double perc2 = params[probabilityDensityPercentage2ID]; //0-100%



															/*delete[]samplingTab1;
															delete[]samplingTab2;*/

	int val;
	for (i = 0; i<data->GetSliceSize(); i++)
	{

		val = tab[i] * scaleF;
		mask0[i] = 0;
		bool test1 = ((intensityProbabilityTab1[val])*factor1>perc1);
		bool test2 = ((intensityProbabilityTab2[val])*factor2>perc2);

		if (test1) { mask0[i] += 1; }
		if (test2) { mask0[i] += 2; }

		//wynik - 0 nic, 1- z mapy1 2 - z mapy2, 3 - z obu
	}
	delete[]intensityProbabilityTab1;
	delete[]intensityProbabilityTab2;
	//zadanie:
	// 1. pokazac obszary 1,2, 3
	// 2. startujac tylko z obszarow 3 zalac sie na 1 i 2 stykajace sie
	// 3. pokazac wynik i podac obliczenia dla 1 i 2 na styku 
	// 4. dodac kryterium rozmiaru i szta�tu wysp


	//AddParam("cleanShape1", true, true, 0, 1, 1, cleanShapesID, &list);
	//AddParam("cleanShape2", true, true, 0, 1, 1, cleanShapes2ID, &list);
	//	AddParam("currentShapeEdit", 1, true, 1, 2, 1, currentShapeEditID);




	int *listS = intBuffers[0];//new int [sliceSize];
	int *listTmp = intBuffers[1];
	int nr0 = 0;


	//make small kernel



	int kernelSize = 8;
	int size = 1;
	int *kernel = new int[kernelSize];
	int *kernelInc = new int[kernelSize];
	int pos = 0;
	for (j = -size; j<size + 1; j++)
	{
		for (i = -size; i<size + 1; i++)
		{
			if (!((i == 0) && (j == 0)))
			{
				kernel[pos] = i + j*n[0];
				kernelInc[pos++] = j;
			}

		}
	}

	int col1[] = { params[r1ID], params[g1ID], params[b1ID] };
	int col2[] = { params[r2ID], params[g2ID], params[b2ID] };


	/*int r1=255,g1=255,b1=255;
	int r2=255,g2=200,b2=200;*/


	int startPos = 0;
	int ap = 0;
	int lp = 0;
	int count;
	int tmp;
	int dsizemin1 = sliceSize - 1;




	int minArea1 = params[minArea1ID];
	int maxArea1 = params[maxArea1ID];
	int minWidth1 = params[minWidth1ID];
	int maxWidth1 = params[maxWidth1ID];
	int minHeight1 = params[minHeight1ID];
	int maxHeight1 = params[maxHeight1ID];
	if (maxArea1 == 0) maxArea1 = sliceSize;
	if (maxWidth1 == 0) maxWidth1 = n[0];
	if (maxHeight1 == 0) maxHeight1 = n[1];


	int minArea2 = params[minArea2ID];
	int maxArea2 = params[maxArea2ID];
	int minWidth2 = params[minWidth2ID];
	int maxWidth2 = params[maxWidth2ID];
	int minHeight2 = params[minHeight2ID];
	int maxHeight2 = params[maxHeight2ID];
	if (maxArea2 == 0) maxArea2 = sliceSize;
	if (maxWidth2 == 0) maxWidth2 = n[0];
	if (maxHeight2 == 0) maxHeight2 = n[1];



	int morphologyType = 0;//params[morphologyTypeID];
	int morphologyKernelType = params[morphologyKernelTypeID];

	bool showContour1Points = params[showContour1PointsID];
	bool showContour2Points = params[showContour2PointsID];


	bool drawFonts = params[drawFontsID];

	int infoType = params[infoTypeID];


	int objectNr = 0;
	int *dilatePoints = intBuffers[1];//new int[sliceSize];


	int jStart = 0;
	int jStop = sliceSize;

	int mousePosition = shapes->clickedLeftPoint.rx() + shapes->clickedLeftPoint.ry()*n[0];

	int currPos;

	unsigned char *dilateBuffer1 = new unsigned char[sliceSize];
	memset(dilateBuffer1, 0, sliceSize);

	unsigned char *cBuffer = new unsigned char[sliceSize];
	memset(cBuffer, 0, sliceSize);
	for (i = 0; i< sliceSize; i++)
	{
		if ((mask0[i] == 1) || (mask0[i] == 3)) cBuffer[i] = 1;
	}
	bool onlyAdjacent = params[onlyAdjacentID];
	int nrRegions = 0;

	bool statsCalculated = false;
	double paramsx[10];

	for (j = jStart; j < jStop; j++)
	{
		currPos = j;

		if (cBuffer[currPos] == 1)
		{
			nrRegions++
				;
			int params[7];

			RegionGrowingOnMask2(currPos, cBuffer, data, 1, 255, kernel, kernelInc, kernelSize, params, listS, listTmp);

			double nx1d = 1.0 / n[0];

			count = params[0];
			int minx = params[1];
			int maxx = params[2];
			int miny = params[3];
			int maxy = params[4];
			int width = params[5];
			int height = params[6];

			if (count > 1)
			{
				i = i;
			}

			if ((count<minArea1) || (count>maxArea1) || (width<minWidth1) || (width>maxWidth1) || (height<minHeight1) || (height>maxHeight1))
			{
				//wyczysc
				for (i = 0; i<count; i++) { cBuffer[listTmp[i]] = 0; }
			}
			else
			{
				if (onlyAdjacent == false)
					for (int i = 0; i < count; i++)
					{
						if (listTmp[i] == mousePosition)
						{
							GetStatisticsFromVolumeSampledTab(data, listTmp, count, paramsx);
							statsCalculated = true;
							break;
						}
					}

				MyShape shape = createDilateConturPoints(params, data, listTmp, morphologyKernelType, morphologyType, dilatePoints, col1, 1, dilateBuffer1, adjacentDist); //dilateBuffer zamalowany dylacja



				if (onlyAdjacent == false)
				{

					shapes->shapeList.append(shape);




				}
			}


		}


	}

	unsigned char *dilateBuffer2 = new unsigned char[sliceSize];
	memset(dilateBuffer2, 0, sliceSize);

	unsigned char *cBuffer2 = new unsigned char[sliceSize];
	memset(cBuffer2, 0, sliceSize);
	for (i = 0; i< sliceSize; i++)
	{
		if ((mask0[i] == 2) || (mask0[i] == 3)) cBuffer2[i] = 1;
	}

	int nrRegions1 = 0;
	for (j = jStart; j < jStop; j++)
	{
		currPos = j;

		if (cBuffer2[currPos] == 1)
		{
			nrRegions1++;
			int params[7];

			RegionGrowingOnMask2(currPos, cBuffer2, data, 1, 255, kernel, kernelInc, kernelSize, params, listS, listTmp);

			double nx1d = 1.0 / n[0];

			count = params[0];
			int minx = params[1];
			int maxx = params[2];
			int miny = params[3];
			int maxy = params[4];
			int width = params[5];
			int height = params[6];

			if (count > 1)
			{
				i = i;
			}

			if ((count<minArea2) || (count>maxArea2) || (width<minWidth2) || (width>maxWidth2) || (height<minHeight2) || (height>maxHeight2))
			{
				//wyczysc
				for (i = 0; i<count; i++) { cBuffer2[listTmp[i]] = 0; }
			}
			else
			{
				bool testx = true;
				if (onlyAdjacent == true)
				{
					testx = false;
					for (i = 0; i < count; i++)
					{
						if (dilateBuffer1[listTmp[i]] >0)
						{
							testx = true; break;
						} // jesli cBuffer2 dla danej wyspy styka sie z dowolnym pktem z cbuffer to jest adjacent i git
					}

				}

				if (testx == true)
				{
					if (statsCalculated == false)
						for (int i = 0; i < count; i++)
						{
							if (listTmp[i] == mousePosition)
							{
								GetStatisticsFromVolumeSampledTab(data, listTmp, count, paramsx);
								statsCalculated = true;
								break;
							}
						}
					MyShape shape = createDilateConturPoints(params, data, listTmp, morphologyKernelType, morphologyType, dilatePoints, col2, 1, dilateBuffer2, adjacentDist);
					shapes->shapeList.append(shape);

				}
				else
				{
					//wyczysc bo sie nie styka!
					for (i = 0; i<count; i++) { cBuffer2[listTmp[i]] = 0; }
				}
			}


		}


	}



	if (onlyAdjacent == true) //jeszcze raz tym razem czyszczac z buffora 2 nie stykajace sie
	{
		nrRegions = 0;
		for (j = jStart; j < jStop; j++)
		{
			currPos = j;

			if (cBuffer[currPos] == 255)
			{
				nrRegions++
					;
				int params[7];

				RegionGrowingOnMask2(currPos, cBuffer, data, 255, 254, kernel, kernelInc, kernelSize, params, listS, listTmp);

				double nx1d = 1.0 / n[0];

				count = params[0];
				int minx = params[1];
				int maxx = params[2];
				int miny = params[3];
				int maxy = params[4];
				int width = params[5];
				int height = params[6];

				if (count > 1)
				{
					i = i;
				}

				//if ((count<minArea1) || (count>maxArea1) || (width<minWidth1) || (width>maxWidth1) || (height<minHeight1) || (height>maxHeight1))
				//{
				//	//wyczysc
				//	for (i = 0; i<count; i++)	{ cBuffer[listTmp[i]] = 0; }
				//}
				//	else
				{

					bool testx = true;
					//if (onlyAdjacent == true)
					{
						testx = false;
						for (i = 0; i < count; i++)
						{
							if (dilateBuffer2[listTmp[i]] >0)
							{
								testx = true; break;
							} // jesli cBuffer2 dla danej wyspy styka sie z dowolnym pktem z cbuffer to jest adjacent i git
						}

					}

					if (testx == true)
					{
						if (statsCalculated == false)
							for (int i = 0; i < count; i++)
							{
								if (listTmp[i] == mousePosition)
								{
									GetStatisticsFromVolumeSampledTab(data, listTmp, count, paramsx);
									statsCalculated = true;
									break;
								}
							}
						MyShape shape = createDilateConturPoints(params, data, listTmp, morphologyKernelType, morphologyType, dilatePoints, col1);
						shapes->shapeList.append(shape);

					}
					else
					{
						//wyczysc bo sie nie styka!
						for (i = 0; i<count; i++) { cBuffer[listTmp[i]] = 0; }
					}
				}


			}


		}
	}

	//mam cBuffer - teraz musze sprawdzic te wyspy co zosta�y czy cos jakikolwiek piksel zosta� 





	//3 przebiegi. - pierwszy po masce szuka 1 lub 3 - zamalowuje te placki na 0, ktore nie spelniaja parametrow wielkosci - jesli onlyadjacent jest false dodaje obwodki
	// - drugi szuka 2 lub 3 - zmalowuje na 0 te co z�e i jesli onlyAdjacent jest prawdziwe - sprawdza czy styka sie z jakims z cBuffer1 - jak nie tez czysci -dla tych co zostaly dodaje obwodki
	//- trzeci - jesli onlyadj jest true - przebierga po cbuffer1 i patrzy czy jest adj z cbuffer2 - jesli nie - czysci placki - na koniec to co zostanie dodaje obwodki






	if (statsCalculated == true)

	{
		int fontSize = params[fontSizeID];
		MyShape shape;
		QString text;
		text =
			"min: " + QString::number(paramsx[0]) + "   " + "max: " + QString::number(paramsx[1]) + "   " + "\n" +
			"mean: " + QString::number(paramsx[2]) + "   " + "stddev: " + QString::number(paramsx[3]) + "   " + "\n" +
			"skewness: " + QString::number(paramsx[4]) + "   " + "kurtosis: " + QString::number(paramsx[5]) + "   " + "\n" +
			"voxelCount: " + QString::number(paramsx[6]) + "   " + "area: " + QString::number(paramsx[7]) + "   " + " mm^2\n";


		double xPos = 10;// params[textXPosID] * nx;
		double yPos = 10;// params[textYPosID] * ny;

		double fSize = fontSize;
		shape.shapeText = QString(text);
		shape.textPos.setX(xPos);
		shape.textPos.setY(yPos);//fSize*1.2);
		shape.font.setPixelSize(fSize);
		//shape.textAngle=-minPos;

		shape.textColor.setRgb(255, 0, 0);
		shapes->shapeList.append(shape);
	}










	int testVal = params[useAsMaskID];
	if (testVal == 1)
	{
		for (i = 0; i<data->GetSliceSize(); i++)
		{
			//if ((mask0[i] == testVal) || (mask0[i] == 3)) mask0[i] = 255; else mask0[i] = 0;
			mask0[i] = (cBuffer[i]>0) ? 255 : 0;
		}
	}
	else if (testVal == 2)
	{
		for (i = 0; i<data->GetSliceSize(); i++)
		{
			//if ((mask0[i] == testVal) || (mask0[i] == 3)) mask0[i] = 255; else mask0[i] = 0;
			mask0[i] = (cBuffer2[i]>0) ? 255 : 0;
		}
	}
	else if (testVal == 3)
	{
		for (i = 0; i<data->GetSliceSize(); i++)
		{
			if (mask0[i] == 3) mask0[i] = 255; else mask0[i] = 0;

		}
	}
	else if (testVal == 4)
	{
		for (i = 0; i<data->GetSliceSize(); i++)
		{
			if (dilateBuffer1[i] == 1) 				mask0[i] = 255;			else 				mask0[i] = 0;
		}
	}
	else if (testVal == 5)
	{
		for (i = 0; i<data->GetSliceSize(); i++)
		{
			if (dilateBuffer2[i] == 1) 				mask0[i] = 255;			else 				mask0[i] = 0;
		}
	}
	delete[]cBuffer;
	delete[]cBuffer2;
	delete[]dilateBuffer1;
	delete[]dilateBuffer2;
	return 1;

}

LayerMaskProbabilityAdjacentRegionFilter::~LayerMaskProbabilityAdjacentRegionFilter()
{
	if (expTab != NULL) delete[]expTab;
	if (samplingTab1 != NULL)	delete[]samplingTab1;
	if (samplingTab2 != NULL)delete[]samplingTab2;

}
//---------------------------------









LayerMaskGrowingRegionFilter::LayerMaskGrowingRegionFilter()
{
	frontTabIn = NULL;
	frontTabOut = NULL;
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




	filterName = "LayerMaskGrowingRegionFilter";
	QStringList list;
	list.clear(); list << "false" << "true";

	AddParam("inverse", 0, true, 0, 1, 1, inverseID, &list);

	list.clear(); list << "threshold" << "probability";
	AddParam("Mode", 0, true, 0, 1, 1, probOrThreshID, &list);
	list.clear(); list << "false" << "true";

	AddParam("lth", 0, true, 0, 10000, 1, threshold1ID);
	AddParam("rth", 10000, true, 0, 10000, 1, threshold2ID);

	list.clear(); list << "2D Source" << "2D Result" << "3D Source";
	AddParam("sampling3DMode", 0, true, 0, 2, 1, sampling3DModeID, &list);

	AddParam("autoThreshold", 1, true, 0, 1, 1, autoThresholdID, &list);


	list.clear(); list << "false" << "true";

	AddParam("showMask", 0, true, 0, 1, 1, showMaskID, &list);
	AddParam("segOpacity", 0.5, true, 0.0, 1, 0.001, segOpacityID);


	//	AddParam("useAsMask", 1, true, 1, 6, 1, useAsMaskID);
	//AddParam("updateProbabilityTabs", true, true, 0, 1, 1, updateProbabilityTabsID, &list);

	//	AddParam("showSourceLines",
	AddParam("showStats", 0, true, 0, 1, 1, calculateStatsID, &list);
	AddParam("radiusX", 10, true, 0.1, 200, 0.01, radiusXID);
	AddParam("radiusY", 10, true, 0.1, 200, 0.01, radiusYID);
	AddParam("radiusZ", 10, true, 0.1, 200, 0.01, radiusZID);
	//AddParam("radius2", 10, true, 0.1, 200, 0.01, radius2ID);

	//AddParam("angle", 0, true, 0, 360, 1, angleID);
	//AddParam("dest", 50, true, 4, 360, 1, destID);

	AddParam("maxDist", 10, true, 1, 1024, 1, maxDistID);
	//AddParam("median", 1, true, 0, 1, 1, medianID, &list);
	//AddParam("medianFactor", 1, true, 0, 1, 0.001, medianFactorID);
	//AddParam("medianKernel", 2, true, 1, 20, 1, medianKernelID);
	//AddParam("medianKernel3D", 2, true, 1, 100, 1, medianKernel3DID);
	//AddParam("medianThreshold", 10, true, 1, 512, 1, medianThresholdID);

	list.clear(); list << "square" << "circle";
	AddParam("ktype", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	AddParam("dilateDist", 2, true, 0, 20, 1, dilateDistID);
	AddParam("closingSize3D", 2, true, 0, 4, 1, closingSizeID);

	AddParam("showFrontNr", 1, true, 1, 255, 1, showFrontID);




	list.clear(); list << "false" << "true";
	AddParam("useBigMask", 1, true, 0, 1, 1, useBigMaskID, &list);

	list.clear(); list << "clear" << "add";
	AddParam("bigMaskMode", 0, true, 0, 1, 1, bigMaskModeID, &list);

	AddParam("propPercentCoarse", 0.8, true, 0.0, 0.9, 0.1, probabilityDensityPercentageCoarseID);
	AddParam("propPercentFine", 0.0, true, 0.0, 0.09999, 0.0001, probabilityDensityPercentageFineID);
	AddParam("sigma", 1.5, true, 0.01, 6, 0.01, sigmaID);
	AddParam("intensityFactor", 0.5, true, 0.001, 1, 0.001, intensityFactorID);


	list.clear(); list << "false" << "true";
	//AddParam("onlyShortening", 0, true, 0, 1, 1, onlyShorteningID, &list);
	//AddParam("ellipseRayDest", 0, true, 0, 1, 1, ellipseRayDestID, &list);



	AddParam("showContour1Points", 0, true, 0, 1, 1, showContour1PointsID, &list);
	list.clear(); list << "dilate" << "erode";


	//AddParam ("morphology",0,true,0,1,1,morphologyTypeID,&list);
	//list.clear(); list << "square" << "circle";
	//AddParam("ktype", 0, true, 0, 1, 1, morphologyKernelTypeID, &list);
	int morphologyTypeID;
	int morphologyKernelTypeID;


	list.clear(); list << "false" << "true";
	AddParam("drawFonts", 1, true, 0, 1, 1, drawFontsID, &list);
	//	AddParam("CleanShapes", 1, true, 0, 1, 1, cleanShapesID, &list);


	list.clear();
	//list << "Number: "<<"pixel area: " << "box area: " <<"pix/box: "<<"ellipse area: "<<"pix/ellipse " << "D1: " <<"D2";



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

}
//----------------------------------------------
LayerMaskGrowingRegionFilter::~LayerMaskGrowingRegionFilter()
{
	if (expTab != NULL) delete[]expTab;
	if (samplingTab1 != NULL)	delete[]samplingTab1;
	if (samplingTabIndex != NULL)	delete[]samplingTabIndex;

	if (samplingTab != NULL)	delete[]samplingTab;


	if (frontTabIn != NULL) delete[]frontTabIn;
	if (frontTabOut != NULL) delete[]frontTabOut;



}
//------------------------------------
bool	LayerMaskGrowingRegionFilter::ApplyFiltering(RawDataMixerLite *mixer, RawDataSet *data, int sliceNr, unsigned char *masks, double *params, int **intBuffers, ImageShapes  *shapes, ImageWidget *iWidget, unsigned short *dataTab)
{

	//return 0;

	double mSigma = params[sigmaID];
	bool useStartShapes = true;

	QList<QPolygonF> *slist = &shapes->polygonList;

	if (slist == NULL) return 0;

	int nrPolygons = shapes->polygonList.count();

	if ((sliceNr < 0) || (sliceNr >= data->GetNz()) || (shapes == NULL)) return 0;
	//	unsigned short *tab=(dataTab==NULL)?data->GetDataArray()+(sliceNr*data->GetSliceSize()):dataTab;
	unsigned short *tab = data->GetDataArray() + (sliceNr*data->GetSliceSize());
	unsigned char *mask0 = masks;

	unsigned char *masktmp;
	//	int  j, k;
	revth = params[inverseID];
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
	leftTh = params[threshold1ID] * mainDataMaxValue *0.0001;;
	rightTh = params[threshold2ID] * mainDataMaxValue *0.0001;

	shapes->cleanPolygonFlag = true;


	mode = params[probOrThreshID];//prob 0 thresh 1


	unsigned short maxValue = (mainDataMaxValue > 0) ? mainDataMaxValue : data->GetMaxValue();

	//int mmmm = data->GetMaxValue();

	int n[3];
	int sliceSize = data->GetSliceSize();
	data->GetN(n);


	double center[2] = { data->GetNx() / 2, data->GetNy() / 2 };
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

	if ((expMax != maxValue + 1) || (sigmaChg == true) || (intensChg == true))
	{

		sigma = mSigma;
		if (expTab != NULL)delete[]expTab;
		expMax = maxValue + 1;
		expTab = new double[expMax * 2 + 1];

		if (samplingTab1 != NULL)delete[]samplingTab1;
		samplingTab1 = new unsigned short[expMax];
		if (samplingTabIndex != NULL)delete[]samplingTabIndex;
		samplingTabIndex = new unsigned short[expMax];


		if (samplingTab != NULL)delete[]samplingTab;
		samplingTab = new unsigned short[expMax];

		GetExpTab(expTab, expMax, sigma, intensityFactor);

	}

	memset(samplingTab, 0, expMax * sizeof(unsigned short));
	memset(samplingTab1, 0, expMax * sizeof(unsigned short));



	bool useBigMask = params[useBigMaskID];
	int bigMaskMode = params[bigMaskModeID];


	int valx;
	int minValx = 65535; int maxValx = 0;

	//	RawDataSet *newData = new RawDataSet(data->GetNx(), data->GetNy(), 1);
	//newData->ZeroQ();
	int posxx = 0;

	memset(mask0, 0, sliceSize * sizeof(unsigned char));

	int minI[2] = { n[0], n[1] };
	int maxI[2] = { 0, 0 };

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
					if (mainMask[posxx] == 1)
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
					mask0[posxx] = 1;

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


	if (samplesCount == 0) return false;

	bool autoThresh = params[autoThresholdID];
	if (autoThresh)
	{
		leftTh = minValx;
		rightTh = maxValx;
	}



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
																																					  //double factor1 = (max1 - min1 != 0) ? 1.0 / (max1 - min1) : 1;
	double factor1 = (max1 != 0) ? 1.0 / (max1) : 1;
	double percCoarse = params[probabilityDensityPercentageCoarseID]; //0-100%
	double percFine = params[probabilityDensityPercentageFineID]; //0-100%

	perc = percCoarse + percFine;



	QPolygonF testPolyMask;

	QElapsedTimer tm;
	tm.start();
	unsigned int t1 = tm.elapsed();


	bool showMask = params[showMaskID];



	//	double dest = params[destID];
	maxDist = params[maxDistID];

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








	unsigned char *blockMask = NULL;
	if ((useBigMask == true) && (mainMask != NULL)) blockMask = mainMask;
	bool is2d = true;

	//startuję z obrysu - robię dylatację tego co jest - przetestuję ileśtam dylatacji

	//morph
	int kerenelSize2 = 0;
	int dilateDist = params[dilateDistID]; exportParams[5] = params[dilateDistID];


	int size2 = 1;
	int kernelType = params[morphologyKernelTypeID]; exportParams[6] = params[morphologyKernelTypeID];
	int closingSize = params[closingSizeID]; exportParams[7] = params[closingSizeID];

	//int *kernel2 = GetGrowingKernel2D(kerenelSize2, size2, data, kernelType);
	int tmp;
	int dsizemin1n = n[0] * n[1] - 1;



	minI[0] -= 1; if (minI[0] < 0) minI[0] = 0;
	minI[1] -= 1; if (minI[1] < 0) minI[1] = 0;
	maxI[0] += 1; if (maxI[0] >= n[0] - 1) maxI[0] = n[0] - 1;
	maxI[1] += 1; if (maxI[1] >= n[1] - 1) maxI[1] = n[1] - 1; //granice obiektu samplowanego powiekszone o 1 do dylatacji
	int boundsX[6] = { minI[0], maxI[0], minI[1], maxI[1], 0, 0 };

	int nrFront = 2;//tak koloruje pierwszy front
	SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 1, 1, kernelType, NULL, boundsX, blockMask, cr, is2d, nrFront); //z criterion na danych




	if (frontSizeMax < sliceSize)
	{
		if (frontTabIn != NULL) delete[]frontTabIn;
		if (frontTabOut != NULL) delete[]frontTabOut;

		frontTabIn = new int[sliceSize];
		frontTabOut = new int[sliceSize];
		frontSizeMax = sliceSize;
	}
	//celem jest zrobic dylatacje


	int frontTabInLength = 0;
	int frontTabOutLength = 0;


	int *kernelX = NULL;
	int kernelSizeX = 0;
	kernelX = GetGrowingKernel2D(kernelSizeX, 1, data, kernelType);


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





	unsigned char *tempMaskTab = new unsigned char[sliceSize];
	memset(tempMaskTab, 0, sliceSize * sizeof(unsigned char));
	int *listS = intBuffers[0];
	int listSSize = sliceSize;
	int maxIsleNr = nrFront; //bo 1 startowe kolko, 2 pierwsza dylacja
	for (int i = 0; i < maxDist; i++)
	{
		if (frontTabInLength == 0) break;
		//przebiec po punktach z ostatniego rzutu
		SlotBinarySimpleMorphologyWithCriterion2InOut(data, mask0,
			frontTabIn, frontTabInLength, frontTabOut, frontTabOutLength,
			kernelX, kernelSizeX, blockMask, cr);

		//swap




		//biorę punkty ktore maja flage 255 - zalweam sie na nie
		//RegionGrowingOnMask2(currPos, mask0, data, 1, 255, kernel, kernelInc, kernelSize, prms, listS, listTmp);

		RemapOutFrontTabWithDidivedIslands(frontTabOut, frontTabOutLength, mask0, tempMaskTab, listS, listSSize, data, maxIsleNr, 0, true);

		int *tabswap = frontTabOut; frontTabOut = frontTabIn; frontTabIn = tabswap;
		frontTabInLength = frontTabOutLength;

	}


	delete[]tempMaskTab;


	int maxVal = 0;


	int frontToShow = params[showFrontID];


	for (int i = 0; i < sliceSize; i++)
	{
		if (mask0[i]>maxVal) maxVal = mask0[i];
		//	if (mask0[i]>1) mask0[i] = 1;//na chwile zeby zobaczyc czy sie rozrasta

		if (mask0[i] == frontToShow) mask0[i] = 1; else mask0[i] = 0;
	}


	qDebug() << "nrOfFronts: " << maxVal;


	////////double *rayTab = new double[10000];
	////////int nrRays = 0;

	////////double r0 = 1;

	////////bool ellipseRayDest = params[ellipseRayDestID];
	////////equalRadius = ellipseRayDest;
	////////double rr1 = r1;
	////////double rr2 = r2;
	////////if (ellipseRayDest)
	////////{
	////////	rr1 = r0;
	////////	rr2 = r0;
	////////}


	////////for (int jj = 0; jj < 360; jj += step)
	////////{
	////////	t = jj*rad;
	////////	ct = cos(t);
	////////	st = sin(t);

	////////	x = centerE[0] + rr1 * ct*ca - rr2 * st*sa;
	////////	y = centerE[1] + rr1 * ct*sa + rr2 * st*ca;


	////////	dx = x - centerE[0];
	////////	dy = y - centerE[1];
	////////	double len = sqrt(dx*dx + dy*dy);
	////////	if (len > 0)
	////////	{
	////////		dx /= len;
	////////		dy /= len;


	////////		rayTab[nrRays * 4] = x;
	////////		rayTab[nrRays * 4 + 1] = y;
	////////		rayTab[nrRays * 4 + 2] = dx;
	////////		rayTab[nrRays * 4 + 3] = dy;
	////////		nrRays++;
	////////	}


	////////}


	////////c


	////////int neighSize = 2;
	////////int neighType = 0;//0-cube 1-sphere
	////////double neighPercentage = 0.3;

	////////if (mode == true)
	////////{
	////////	//prob map

	////////	cr = new SegCriterionProbabilityMap(data, intensityProbabilityTab1, perc, min1, factor1, perc, neighType, neighSize);
	////////}
	////////else
	////////{
	////////	//thresholds
	////////	cr = new SegCriterionThresholds(data, leftTh, rightTh, rightTh, revth, neighPercentage, neighType, neighSize);
	////////}
	////////cr->Set2DMode(true);


	////////double *lengthTab = new double[nrRays];
	////////double *lengthTab2 = new double[nrRays];
	////////double minRayStep = 1;// MIN(mdx, mdy)*0.5;
	////////for (int jj = 0; jj < nrRays; jj++)
	////////	//for (int jj = 0; jj < 360; jj += step)
	////////{

	////////	{
	////////		x = rayTab[jj * 4];
	////////		y = rayTab[jj * 4 + 1];
	////////		dx = rayTab[jj * 4 + 2];
	////////		dy = rayTab[jj * 4 + 3];

	////////		bool test = true;
	////////		double ii;
	////////		double maxDistD = maxDist / minRayStep;
	////////		for (ii = 0; ii < maxDistD; ii += minRayStep)
	////////		{
	////////			x1 = x + dx*ii;
	////////			y1 = y + dy*ii;

	////////			if (x1<0 || y1<0 || x1>n[0] - 1 || y1>n[1] - 1)
	////////			{
	////////				ii -= 1; break;// test = false; break;
	////////			}

	////////			int ppos = x1 + y1*n[0];


	////////			if ((useBigMask == true) && (mainMask != NULL))
	////////			{
	////////				if (mainMask[ppos] == 1)
	////////				{
	////////					ii -= 1;
	////////					break;
	////////				}
	////////			}


	////////			test1 = (cr->CheckPoint(0, 0, 0, NULL, 0, ppos) > 0);

	////////			if (test1 == false)
	////////			{

	////////				break;
	////////			}

	////////		}


	////////		if ((test == true) && (ii >= 0))
	////////		{
	////////			lengthTab[jj] = ii;

	////////		}


	////////	}

	////////}

	//////////ewentualne filtrowanie
	////////bool medianLines = params[medianID]; exportParams[0] = params[medianID];
	////////int medianKernel = params[medianKernelID]; exportParams[1] = params[medianKernel3DID]; // bo nie dziala tak samo !!!
	////////double medianFactor = params[medianFactorID]; exportParams[2] = params[medianFactorID];



	//////////trzeba dodac rozmiar kernela do filtra medianowego

	////////if (medianLines == true)
	////////{
	////////	bool onlyShortening = params[onlyShorteningID]; exportParams[3] = params[onlyShorteningID];
	////////	int medianThreshold = params[medianThresholdID]; exportParams[4] = params[medianThresholdID];
	////////	double *medTab = new double[medianKernel * 2 + 1];

	////////	//filtruje
	////////	for (int jj = 0; jj < nrRays; jj++)
	////////	{
	////////		for (int j = 0; j < medianKernel * 2 + 1; j++)
	////////		{
	////////			int pos = (jj + nrRays + j - medianKernel) % nrRays;
	////////			//if ((pos >= 0) && (pos<newNrPts))
	////////			{
	////////				medTab[j] = lengthTab[pos];
	////////			}
	////////		}


	////////		double med = Median(medTab, medianKernel * 2);
	////////		lengthTab2[jj] = med - lengthTab[jj]; //roznica
	////////	}
	////////	//podmieniam z wagami
	////////	int lenx;
	////////	for (int i = 0; i < nrRays; i++)
	////////	{
	////////		lenx = ((fabs(lengthTab2[i]) < medianThreshold) ? 0 : medianFactor*lengthTab2[i]) + lengthTab[i];// (1.0 - medianFactor)*lengthTab2[i + medianKernel];


	////////		lengthTab[i] = (!onlyShortening) ? lenx : (lenx < lengthTab[i]) ? lenx : lengthTab[i]; //wstawiam to co wyszlo z median chyba ze mam tylko skracanie to sprawdzam czy krotszy przed zamiana
	////////	}
	////////	delete[]medTab;
	////////}


	//////////po przefiltrowaniu
	////////for (int jj = 0; jj < nrRays; jj++)
	////////{


	////////	testPoly.append(QPoint(centerE[0], centerE[1]));

	////////	x = rayTab[jj * 4];
	////////	y = rayTab[jj * 4 + 1];
	////////	dx = rayTab[jj * 4 + 2];
	////////	dy = rayTab[jj * 4 + 3];
	////////	x1 = x + dx*lengthTab[jj];
	////////	y1 = y + dy*lengthTab[jj];
	////////	testPoly.append(QPoint(x1, y1));
	////////	testPolyMask.append(QPoint(x1, y1));

	////////}


	////////if (testPoly.count() > 0)
	////////{
	////////	MyShape s;
	////////	//s.name = "areaPolys";
	////////	s.poly = testPoly;
	////////	s.representation = 3;
	////////	s.polyColor.setRgb(170, 0, 255);
	////////	shapes->shapeList.append(s);
	////////}

	////////if (showMask == true)
	////////{
	////////	MyShape s;
	////////	s.name = "areaPolys";
	////////	s.poly = testPolyMask;
	////////	s.representation = 2;
	////////	s.opacity = params[segOpacityID];
	////////	s.polyColor.setRgb(255, 0, 0);
	////////	shapes->shapeList.append(s);

	////////}

	////////delete[]rayTab;
	////////delete[]lengthTab;
	////////delete[]lengthTab2;

	//zbierze segmentacje do maski

	int nx = data->GetNx();
	int ny = data->GetNy();
	//QImage img(nx, ny, QImage::Format_RGB32);
	//img.fill(Qt::black);
	//QPainter Painter2(&img);
	//Painter2.setBrush(Qt::white);
	//Painter2.setPen(1);

	//Painter2.drawPolygon(testPolyMask);


	////img.save("d:\\pic2.png", "PNG");
	////namalowane - zbieram do wlasciwego
	//unsigned int *tabImg = (unsigned int *)img.bits();;
	unsigned char *dilateBuffer1 = new unsigned char[sliceSize];
	memset(dilateBuffer1, 0, sliceSize);

	int minx = n[0];
	int maxx = 0;
	int miny = n[1];
	int maxy = 0;
	int ix, iy;
	double nx1d = 1.0 / n[0];
	for (int i = 0; i < sliceSize; i++)
	{

		val = mask0[i];
		if (val == 1)
		{
			iy = i*nx1d;
			ix = i - iy*n[0];

			if (ix<minx) minx = ix;
			if (ix>maxx) maxx = ix;
			if (iy<miny) miny = iy;
			if (iy>maxy) maxy = iy;
		}
	}


	//mask0 wypelnione ksztaltem elipsy!








	int nrOfDilatePoints = 0;


	bool stop;
	double nnx1d = 1.0 / n[0];
	//contur extraction

	//int frontVal = 0;
	//int backVal = 1;

	int ax, ay;

	int minx1 = minx - dilateDist; if (minx1 < 0) minx1 = 0;
	int miny1 = miny - dilateDist; if (miny1 < 0) miny1 = 0;
	int maxx1 = maxx + dilateDist; if (maxx1 > n[0] - 1) maxx1 = n[0] - 1;
	int maxy1 = maxy + dilateDist; if (maxy1 > n[1] - 1) maxy1 = n[1] - 1;
	bool test2;


	int bounds[6] = { minx1, maxx1, miny1, maxy1, 0, 0 };

	for (int kk = 0; kk < dilateDist; kk++)
	{
		SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 1, 1, kernelType, NULL, bounds, blockMask, cr, is2d); //z criterion na danych
	}
	if (closingSize > 0)
	{

		SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 1, closingSize, kernelType, NULL, NULL, blockMask, NULL, is2d);//domykanie bez brania pod uwage danych
		SlotBinarySimpleMorphologyWithCriterion2(data, mask0, 0, closingSize, kernelType, NULL, NULL, blockMask, NULL, is2d);
	}

	//delete[]kernel2;

	//---------------
	//show dilate points


	//	int *listS = intBuffers[0];//new int [sliceSize];
	int *listTmp = intBuffers[1];
	int nr0 = 0;


	//make small kernel



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
	for (int i = 0; i < nx*ny; i++)
	{
		if (mask0[i] == 1)
		{
			currPos = i;
			break;
		}
	}
	int prms[7];
	bool calculateStats = params[calculateStatsID];
	double paramsx[10];


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
	if (currPos >= 0)
	{

		RegionGrowingOnMask2(currPos, mask0, data, 1, 255, kernel, kernelInc, kernelSize, prms, listS, listTmp);
		double nx1d = 1.0 / n[0];

		int count = prms[0];
		int minx = prms[1];
		int maxx = prms[2];
		int miny = prms[3];
		int maxy = prms[4];
		int width = prms[5];
		int height = prms[6];
		int col1[] = { params[r1ID], params[g1ID], params[b1ID] };

		//wyczyszcze samplingTab, zrobie nowe z aktualnymi samplami!
		memset(samplingTab, 0, expMax * sizeof(unsigned short));
		samplesCount = 0;

		if (count > 1)
		{
			int morphologyType = 0;//params[morphologyTypeID];
			int morphologyKernelType = 0;// params[morphologyKernelTypeID];
			int adjacentDist = 1;
			int *dilatePoints = intBuffers[1];//new int[sliceSize];

			if (calculateStats == true)
			{
				GetStatisticsFromVolumeSampledTab(data, listTmp, count, paramsx);
			}

			MyShape shape = createDilateConturPoints(prms, data, listTmp, morphologyKernelType, morphologyType, dilatePoints, col1, 1, dilateBuffer1, adjacentDist); //dilateBuffer zamalowany dylacja
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
	}

	if (sampling3DMode == 1) //kopiuje aktualny stan do tablicy uShortTabMixera - zaznaczenie pelna segmentacja
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
	//************************
	if (calculateStats == true)
	{
		int fontSize = params[fontSizeID];
		MyShape shape;
		QString text;
		text = "lth: " + QString::number(leftTh) + "   " + "rth: " + QString::number(rightTh) + "\n" +
			"min: " + QString::number(paramsx[0]) + "   " + "max: " + QString::number(paramsx[1]) + "   " + "\n" +
			"mean: " + QString::number(paramsx[2]) + "   " + "stddev: " + QString::number(paramsx[3]) + "   " + "\n" +
			"skewness: " + QString::number(paramsx[4]) + "   " + "kurtosis: " + QString::number(paramsx[5]) + "   " + "\n" +
			"voxelCount: " + QString::number(paramsx[6]) + "   " + "area: " + QString::number(paramsx[7]) + "   " + " mm^2\n";


		double xPos = 10;// params[textXPosID] * nx;
		double yPos = 10;// params[textYPosID] * ny;


		int		showInfoPlace = params[showInfoPlaceID];

		//MyShape shape;	
		shape.shapeText = QString(text);
		shape.textColor.setRgb(255, 33, 0);
		//QRectF rect(10, 10, mnx-10, mny-10);
		//shape.objectBounds = rect;
		//shape.textAutoPos = showInfoPlace;
		shape.textPos.setX(xPos);
		shape.textPos.setY(yPos);//fSize*1.2);
		double fSize = fontSize;
		shape.font.setPixelSize(fSize);


		shapes->shapeList.append(shape);


		//	double fSize = fontSize;
		//	shape.shapeText = QString(text);
		//	shape.textPos.setX(xPos);
		//	shape.textPos.setY(yPos);//fSize*1.2);
		//	shape.font.setPixelSize(fSize);
		//	//shape.textAngle=-minPos;
		////	shape2.ellipsesColor.setRgb(params[r1ID], params[g1ID], params[b1ID]);
		//	shape.textColor.setRgb(params[r1ID], params[g1ID], params[b1ID]);
		//	shape.textAutoPos = showInfoPlace;
		//	shapes->shapeList.append(shape);

	}

	delete cr;
	delete[]kernel;
	delete[]kernelInc;

	delete[]dilateBuffer1;
	delete[]intensityProbabilityTab1;
	unsigned int t2 = tm.elapsed();
	//qWarning() << "reszta: " << t2 - t1;

	//zrob dylatacje
	//podziel na wyspy
	//sprawdz jakie wyspy spelniaja kryterium rozmiaru
	//wystrzel kolejne segmentacje wzdluz promieni

	memset(mask0, 255, mnx*mny * sizeof(unsigned char));
	return true;
}
//---------------------------------














void initLayerMaskFilters(LinkedList <LayerMaskFilter*> *layerMaskFilterList)
{

	layerMaskFilterList->InsertItem(new LayerMaskEmptyFilter());
	layerMaskFilterList->InsertItem(new LayerMaskThresholdFilter());
	layerMaskFilterList->InsertItem(new LayerMaskLinearBlendFilter());
	layerMaskFilterList->InsertItem(new LayerMaskCheckedFilter());
	layerMaskFilterList->InsertItem(new LayerMaskGaussianBlurFilter());
	layerMaskFilterList->InsertItem(new LayerMaskAdjacentRegionFilter());
	layerMaskFilterList->InsertItem(new LayerMaskIslandFilter());
	layerMaskFilterList->InsertItem(new LayerMaskPointShapeFilter());
	layerMaskFilterList->InsertItem(new LayerMaskThicknessAnalysisFilter());
	layerMaskFilterList->InsertItem(new LayerMaskIntensityAnalysisFilter());
	layerMaskFilterList->InsertItem(new LayerMaskProbabilityAdjacentRegionFilter());
	layerMaskFilterList->InsertItem(new LayerMaskExplodeSegmentationRegionFilter());

	layerMaskFilterList->InsertItem(new LayerMaskGrowingRegionFilter());



	layerMaskFilterList->InsertItem(new LayerMaskAdjacentRegionFilter());






}