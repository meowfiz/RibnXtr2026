#include "ImageSetRegistration.h"

ImageSetRegistration::ImageSetRegistration(DataSet *ModelDataSet, DataSet *ObjectDataSet)
{
	useGlobalMax = false;
	removeOutliers = false;
	totalTime = 0;
	counter = 0;
	scatterSize = 0;
	parallelComputing = 1;
#pragma omp parallel
	this->numberOfThreads = omp_get_num_threads();
	numberOfThreadsUsed = numberOfThreads;
	scatterTabs = new int *[numberOfThreads];
	paramsTabs = new double*[numberOfThreads];
	for (int i = 0; i < numberOfThreads; i++)
	{

		scatterTabs[i] = NULL;
		paramsTabs[i] = new double[NROFPARAMS];
	}
	tabsInitialized = false;
	actualBins = 0;

	if ((ModelDataSet != NULL) && (ObjectDataSet != NULL))
	{
		ImageSetRegistration(ModelDataSet->GetRawData(), ObjectDataSet->GetRawData());
		paletteReg = ObjectDataSet->GetPalette();

	}

	/*if (ModelDataSet != NULL)
	{
		modelData = ModelDataSet->GetRawData();
	}
	if (ObjectDataSet != NULL)
	{
		objectData = ObjectDataSet->GetRawData();
	}
*/

	interpolation = new InterpolationFunction2DBilinear();
	transformation = NULL;

	for (int i = 0; i < NUMBER_OF_REG_SLIDERS; i++)
	{
		paramsReg[i] = 1;
	}

}
//----------------------
ImageSetRegistration::ImageSetRegistration(RawDataSet *_ModelData, RawDataSet *_ObjectData)
{
	useGlobalMax = false;
	removeOutliers = false;
	totalTime = 0;
	counter = 0;
	scatterSize = 0;
	//parallelComputing = 1;
#pragma omp parallel
	this->numberOfThreads = omp_get_num_threads();
	numberOfThreadsUsed = numberOfThreads;
	//if (numberOfThreadsUsed > 6) numberOfThreadsUsed =6;
	scatterTabs = new int *[numberOfThreads];
	paramsTabs = new double*[numberOfThreads];
	for (int i = 0; i < numberOfThreads; i++)
	{

		scatterTabs[i] = NULL;
		paramsTabs[i] = new double[NROFPARAMS];
	}
	tabsInitialized = false;
	actualBins = 0;

	transversalView = NULL;

	paletteReg = NULL;
	transversalDataSet = NULL;
	paintTransformedSlicesEnabled = false;
	transversalView = NULL;
	nrMissPixels = 0;
	shapes = NULL;
	visWidget = NULL;
	grayMode = false;
	modelData = _ModelData;
	objectData = _ObjectData;

	colSrcData = NULL;
	colDstData = NULL;
	srcData = NULL;
	dstData = NULL;
	edgeSrcData = NULL;
	edgeDstData = NULL;
	srcDistData = NULL;
	dstDistData = NULL;
	ghostData = NULL;
	sliceNumber = 0;
	intrpVal = 0;
	if (objectData != NULL)
	{
		rCenter[0] = objectData->GetNx() / 2;
		rCenter[1] = objectData->GetNy() / 2;
		rCenter[2] = 0;
	}
	//intrp = new InterpolationFunction2DBilinear();
	intrp = new InterpolationFunction2DBilinear();
	interpolation = new InterpolationFunction2DBilinear();
	transformation = NULL;
	/*srcData=new RawDataSet(objectData->GetNx(), objectData->GetNy(),objectData->GetNz(),1,1,1);
	dstData=new RawDataSet(modelData->GetNx(), modelData->GetNy(),modelData->GetNz(),1,1,1);
	srcData->SetRawDataSetName("src-s");
	dstData->SetRawDataSetName("dst-s");
	rCenter[0]=srcData->GetNx()/2;
	rCenter[1]=srcData->GetNy()/2;
	rCenter[2]=0;
	intrp=new InterpolationFunction2DBilinear();
	int i,j;
	// montuje zawartosc zbioru w szarosciach
	if (objectData->GetRGBExists()==false)
	{
	memcpy(srcData->GetDataArray(),objectData->GetDataArray(),srcData->GetVolumeSize()*sizeof(unsigned short));
	}
	else
	{
	unsigned short *p1,*p2;
	unsigned char  *cp1,*cp2;
	unsigned char r,g,b;

	p1=srcData->GetDataArray();
	for (i=0;i<srcData->GetVolumeSize();i++)
	{
	r=(unsigned char)objectData->GetRChannelArray()[i];
	g=objectData->GetGChannelUCHARArray()[i];
	b=objectData->GetBChannelUCHARArray()[i];
	if ((r>2*b) && (r>2*g))
	*p1=255;
	else
	*p1=0;
	p1++;
	}
	}
	//drugi
	if (modelData->GetRGBExists()==false)
	{
	memcpy(dstData->GetDataArray(),modelData->GetDataArray(),dstData->GetVolumeSize()*sizeof(unsigned short));
	}
	else
	{
	unsigned short *p1,*p2;
	unsigned char  *cp1,*cp2;
	unsigned char r,g,b;
	p1=dstData->GetDataArray();
	for (i=0;i<dstData->GetVolumeSize();i++)
	{
	r=(unsigned char)modelData->GetRChannelArray()[i];
	g=modelData->GetGChannelUCHARArray()[i];
	b=modelData->GetBChannelUCHARArray()[i];
	if ((r>2*b) && (r>2*g))
	*p1=255;
	else
	*p1=0;
	p1++;
	}
	}
	srcData->CalculateHistogram();
	dstData->CalculateHistogram();*/

	trans = new Transform(rCenter, 1, 1, 1);
	for (int i = 0; i < NUMBER_OF_REG_SLIDERS; i++)
	{
		paramsReg[i] = 1;
	}
}
void	ImageSetRegistration::SetShapesAndVisImage(ImageShapes 	*s, ImageWidget *w)
{
	shapes = s;
	visWidget = w;
}
//----------------------------------------------
ImageSetRegistration::~ImageSetRegistration()
{
	//emit 	SignalSendRawDataSet(srcData);
	//emit	SignalSendRawDataSet(dstData);
	if (transversalDataSet != NULL) delete transversalDataSet;

	for (int i = 0; i < numberOfThreads; i++)
	{
		//delete []ObjectTabs[i];		ObjectTabs[i]=NULL;
		//delete []ModelTabs[i];		ModelTabs[i]=NULL;
		if (scatterTabs[i] != NULL) { delete[]scatterTabs[i];	scatterTabs[i] = NULL; }
		if (paramsTabs[i] != NULL) { delete[]paramsTabs[i];	paramsTabs[i] = NULL; }
	}

	if (interpolation != NULL) delete transformation;

}
//-------------------------------------------
void ImageSetRegistration::SetData(RawDataSet *s, RawDataSet *d, RawDataSet *m)
{
	if (srcData != NULL) delete srcData;
	if (dstData != NULL) delete dstData;
	//	if (distData!=NULL) delete distData;
	srcData = s;
	dstData = d;
	//distData=m;


	int i, j, k;

	int size = 30;
	double ss;
	intrpVal = 325;//65535.0/(double)(MAX(distData->GetNx(),distData->GetNy()));

	/*RawDataSet *me=new RawDataSet(distData->GetNx()+2*size,distData->GetNy()+2*size,distData->GetNz(),1,1,1);
	unsigned short *tab=me->GetDataArray();
	for(k=0;k<me->GetNz();k++)
	for(j=0;j<me->GetNy();j++)
	for(i=0;i<me->GetNx();i++)
	{
	ss=GetDistanceFromRawData(distData, i-30,j-30, k)/2;
	*(tab++)=ss;
	}

	emit 	SignalSendRawDataSet(me);*/
}
//-------------------------------------------

//-----------------------------------------------
void ImageSetRegistration::ApplyRegistration()
{
	intrpVal = 269;
	rCenter[0] = srcData->GetNx() / 2;
	rCenter[1] = srcData->GetNy() / 2;
	rCenter[2] = 0;

	double tra[3];
	tra[1] = (dstData->GetNx() - srcData->GetNx()) / 2.0;
	tra[2] = (dstData->GetNy() - srcData->GetNy()) / 2.0;

	double tr[10];
	tr[1] = 0;
	tr[2] = 0;
	tr[3] = 0;
	int i, j, k;
	for (i = 0; i < 15; i++)
		t[i] = 0;
	t[10] = 1;
	t[11] = 1;
	t[12] = 1;

	bool tmpBool;
	double Tolerance = 10e-5;
	//	double tolX=10e-8;
	int Iterations;
	double FValue;
	RawDataSet *dataX = new RawDataSet();
	dataX->Copy(*dstData, false);
	RawDataSet *dataX2 = new RawDataSet();
	dataX2->Copy(*srcData, false);

	tempData = new RawDataSet(edgeSrcData->GetNx(), edgeSrcData->GetNy(), 1, 1, 1, 1);
	//tempData->Copy(*edgeSrcData,false);
	int tab[12] = { 1, 2, 3, 4, 15, 16, 24, 25, 34, 35, 36, 42 };


	FILE *Do_opa;
	OptimizationPowell *Optim = NULL;
	int nr = 1;
	int subIter = 5;
	double percentage = 0.5;
	i = 28;
	RawDataSet *colDstData2 = NULL;
	if ((colSrcData != NULL) && (colDstData != NULL))
	{
		colDstData2 = new RawDataSet();
		colDstData2->Copy(*colDstData, false);
		colDstData2->initRGBArrays();
		colDstData2->ZeroQ();
	}

	//wkopiuje wszedzie 42 slice;
	scatterSize = paramsReg[6]; //3-8

	//for (i=0;i<dstData->GetNz();i++)
	for (i = dstData->GetNz() - 1; i > -1; i--)
	{




		sliceNumber = i;
		//tr[1]=tra[1];
		//tr[2]=tra[2];
		MatchCenters(t, rCenter);
		if (i == 5) t[1] -= 10;
		tr[1] = 0;
		//tr[4]=1;


		Iterations = 0;
		FValue = 0;

		//memcpy(	tempData->GetDataArray(),
		//		edgeSrcData->GetDataArray()+sliceNumber*tempData->GetSliceSize(),
		//		tempData->GetSliceSize()*sizeof(unsigned short));
		Optim = NULL;
		tmpBool = false;
		for (k = 0; k < 12; k++)
		{
			if (tab[k] == i + 1)
			{
				tmpBool = true;
				break;
			}

		}

		//if ((i+1==tab[0])||(i+1==tab[1])||(i+1==tab[2])||(i+1==tab[3])&&(i+1==tab[4])&&(i+1==tab[5])&&(i+1==tab[6]))
		//if (tmpBool==true)
		for (j = 0; j < subIter; j++)
		{
			GetPercentDistMaskData(dstDistData, edgeSrcData, tempData, percentage, tr);
			Optim = new OptimizationPowell(tr, nr, Tolerance, &Iterations, &FValue, this);
			Optim->StartOptimization();

		}

		TransformSlice(srcData, dataX, i, tr);
		TransformSlice(modelData, dataX2, i, tr, false);

		if ((colSrcData != NULL) && (colDstData != NULL))
		{
			TransformSlice(colSrcData, colDstData2, i, tr, false);
		}

		emit SendMessage(QString::number(i) + "/" + QString::number(dstData->GetNz() - 1));
		//Do_opa=fopen("c:\\imgrg.txt","a");
		//		fprintf(Do_opa,"%d [" ,i);

		//	for (j=1;j<=nr;j++)
		//		fprintf(Do_opa,"%lf " ,tr[j]);

		//	fprintf (Do_opa, " ] - %lf\n",FValue);
		//	fclose(Do_opa);
		if (Optim != NULL)delete Optim;

		//tymczasowe!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if ((colDstData2 != NULL))
		if (i == dstData->GetNz() - 1)
		{
			for (j = dstData->GetNz() - 2; j > -1; j--)
			{
				memcpy(colDstData2->GetDataArray() + j*tempData->GetSliceSize(),
					colDstData2->GetDataArray() + (dstData->GetNz() - 1)*tempData->GetSliceSize(),
					colDstData2->GetSliceSize()*sizeof(unsigned short));

				memcpy(colDstData2->GetGChannelUCHARArray() + j*tempData->GetSliceSize(),
					colDstData2->GetGChannelUCHARArray() + (dstData->GetNz() - 1)*tempData->GetSliceSize(),
					colDstData2->GetSliceSize()*sizeof(unsigned char));

				memcpy(colDstData2->GetBChannelUCHARArray() + j*tempData->GetSliceSize(),
					colDstData2->GetBChannelUCHARArray() + (dstData->GetNz() - 1)*tempData->GetSliceSize(),
					colDstData2->GetSliceSize()*sizeof(unsigned char));
			}
		}
		///////////////////////////////////

	}
	emit	SignalSendRawDataSet(tempData);
	dataX->SetRawDataSetName("transData");
	emit 	SignalSendRawDataSet(dataX);

	//	TransformSlice(srcData, dataX, i,tr);
	dataX2->SetRawDataSetName("transData2");
	emit 	SignalSendRawDataSet(dataX2);

	if ((colDstData2 != NULL))
	{
		colDstData2->SetRawDataSetName("final");
		emit 	SignalSendRawDataSet(colDstData2);
	}

	//     TransformImage(tr);
}
//-------------------------------------------------------

double ImageSetRegistration::GetFunctionValue(double *tr)
{



	double val = 0;
	//if (grayMode == false)
	//{
	//	//val = CalculateSumOfSquareDifferencesSimilarity(objectData, modelData, sliceNumber, tr, true);
	//	val = CalculateSumOfSquareDifferencesSimilarityGrayImg(oSliceData, mSliceData, tr, false);
	//}
	//else
	{
		//val= CalculateSumOfSquareDifferencesSimilarityGrayImg(oSliceData,mSliceData,tr,true);
		//double inParams[4] = { 0, tr[1], tr[2], tr[3] };
		//double outParmas[4];
		//int nrInParams = 4;
		//int nrOutParams = 4;

		//int result = FiltDram_Filtruj(NULL, NULL, 0, inParams, nrInParams, outParmas, nrOutParams);
		//if (result == 1)
		//{
		//	val = outParmas[0];//tu ma byc zapisany wynik dopasowania
		//	rCenter[0] = outParmas[1];
		//	rCenter[1] = outParmas[2];
		//	rCenter[2] = outParmas[3];
		//}
		//else
		//	val = INT_MAX;
		int ccount;
		double fValue;
		int *intBuffers[2] = { NULL, NULL };
		PrepareTrans2(tr, t, numOfParams, posTab);
		int samplingX = paramsReg[4];
		int samplingY = paramsReg[5];
		//bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter);
		bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter, samplingX, samplingY);


		if (paintTransformedSlicesEnabled == true) PaintTransformedSlices(t);
		if (t[3] > 0)
		{
			int i = 0;
		}
	//	qApp->processEvents();
		//functionNumber ; //0 - count, 1 - ssd, 2 - cross cor, 3 - entropy, 4 -  mi, 5- nmi
		fValue = statsMutual[functionNr];
		ccount = statsMutual[0];

		if ((ccount > minCount))
		{
			if (maxFound > val)maxFound = val;
			val = fValue;
		}
		else
			val = maxFound;;


	}
	//	qWarning ("FVal : "+QString::number(val)); 
	//emit SignalSendActualTransform (t);
	counter++;
	//emit SendMessage("(" + QString::number(sliceNumber) + "/)" + QString::number(resultNrSlices) + " NrCount: " + QString::number(counter) + "\t" + "Value: " + QString::number(val));
	return val;
}
//----------------------------------
void ImageSetRegistration::TransformSlice(RawDataSet *src, RawDataSet *dst, int sliceNr, double *tr, bool inv)
{
	int i, j;
	/*for(i=0;i<15;i++)
		t[i]=0;
		t[10]=1;
		t[11]=1;
		t[12]=1;*/

	PrepareTrans(tr, t);

	//t[1]=tr[1];
	//t[2]=tr[2];
	//t[11]=tr[3];
	//t[10]=tr[3];
	//t[11]=tr[4];
	//RawDataSet *rr =new RawDataSet(rdst->GetNx(),rdst->GetNy(),1,1,1,1);
	if (inv == true)
		trans->MakeCompleteAffineMatrixInv(matrix, t);
	else
		trans->MakeCompleteAffineMatrix(matrix, t);

	int pos = sliceNr*dst->GetSliceSize();
	int v = dst->GetVolumeSize();
	bool rgb = false;
	unsigned char *srcG, *srcB, *dstG, *dstB;
	if ((dst->GetRGBExists() == true) && (src->GetRGBExists() == true))
	{
		rgb = true;
		srcG = src->GetGChannelUCHARArray();
		srcB = src->GetBChannelUCHARArray();
		dstB = dst->GetBChannelUCHARArray();
		dstG = dst->GetGChannelUCHARArray();
	}

	unsigned short *ptr = (dst->GetDataArray() + sliceNr*dst->GetSliceSize());
	//rr->ZeroQ();
	double *Point = new double[3];
	double *Point1 = new double[3];
	double t1 = src->GetNx() - 1;
	double t2 = src->GetNy() - 1;
	Point[0] = 0; Point[1] = 0; Point[2] = 0;
	int count = 0;
	unsigned short val;

	int nx = src->GetNx();
	int ss = src->GetSliceSize();
	for (j = 0; j < dst->GetNy(); j++)
	{
		Point[0] = 0;
		for (i = 0; i < dst->GetNx(); i++)
		{
			trans->MultiplyPoint(Point1, Point);
			val = 0;
			if ((Point1[0] >= 0) && (Point1[0] < t1) &&
				(Point1[1] >= 0) && (Point1[1] < t2))
			{
				//val= src->GetVoxelValue((int)(Point1[0]),(int)(Point1[1]),sliceNr);
				val = intrp->GetInterpolatedValue(src, Point1[0], Point1[1], sliceNr);
				*(ptr) = val;

				if (rgb == true)
				{
					dstG[pos] = ((InterpolationFunction2DBilinear*)intrp)->GetInterpolatedValue(srcG, nx, ss, Point1[0], Point1[1], sliceNr);
					dstB[pos] = ((InterpolationFunction2DBilinear*)intrp)->GetInterpolatedValue(srcB, nx, ss, Point1[0], Point1[1], sliceNr);
				}
			}
			else
			{
				val = 0;
				*(ptr) = val;
				if (rgb == true)
				{
					dstG[pos] = 0;
					dstB[pos] = 255;
				}
			}
			pos++;
			if (val > 0)
			{
				val = val;
			};
			ptr++;
			Point[0]++;
		}
		Point[1]++;
	}
}
//---------------------
void ImageSetRegistration::TransformSlice2(RawDataSet *src, RawDataSet *dst, int sliceNr, double *tr, bool inv)
{
	int i, j;
	/*for(i=0;i<15;i++)
	t[i]=0;
	t[10]=1;
	t[11]=1;
	t[12]=1;*/

	//int posTab[4] = { 0, 1, 2, 6 };
	//	t1[1] = t0[1];//tx
	//t1[2] = t0[2];//ty
	//t1[6] = t0[3] * rad;//rot
	//PrepareTrans(tr, t, 3, posTab);

	PrepareTrans2(tr, t, numOfParams, posTab);

	//t[1]=tr[1];
	//t[2]=tr[2];
	//t[11]=tr[3];
	//t[10]=tr[3];
	//t[11]=tr[4];
	//RawDataSet *rr =new RawDataSet(rdst->GetNx(),rdst->GetNy(),1,1,1,1);

	trans->RCenter[0] = this->rCenter[0];
	trans->RCenter[1] = this->rCenter[1];
	trans->RCenter[2] = this->rCenter[2];
	if (inv == true)
		trans->MakeCompleteAffineMatrixInv(matrix, t);
	else
		trans->MakeCompleteAffineMatrix(matrix, t);

	int pos = sliceNr*dst->GetSliceSize();
	int v = dst->GetVolumeSize();
	bool rgb = false;
	unsigned char *srcG, *srcB, *dstG, *dstB;
	if ((dst->GetRGBExists() == true) && (src->GetRGBExists() == true))
	{
		rgb = true;
		srcG = src->GetGChannelUCHARArray();
		srcB = src->GetBChannelUCHARArray();
		dstB = dst->GetBChannelUCHARArray();
		dstG = dst->GetGChannelUCHARArray();
	}

	unsigned short *ptr = (dst->GetDataArray() + sliceNr*dst->GetSliceSize());
	//rr->ZeroQ();

	double Point[3];
	double Point1[3];
	//double *Point = new double[3];
	//double *Point1 = new double[3];
	double t1 = src->GetNx() - 1;
	double t2 = src->GetNy() - 1;
	Point[0] = 0; Point[1] = 0; Point[2] = 0;
	int count = 0;
	unsigned short val;

	int nx = src->GetNx();
	int ss = src->GetSliceSize();

	double modelScale[3], objectScale[3];
	src->GetScale(objectScale);
	dst->GetScale(modelScale);

	double scalef[2] = { modelScale[0] / objectScale[0], modelScale[1] / objectScale[1] };
	double objscale1[2] = { 1.0 / objectScale[0], 1.0 / objectScale[1] };


	for (j = 0; j < dst->GetNy(); j++)
	{
		//Point[0] = 0;


		for (i = 0; i < dst->GetNx(); i++)
		{
			Point[0] = i*modelScale[0];//przejscie do ukladu wspolrzednych globalnych 
			Point[1] = j*modelScale[1];

			trans->MultiplyPoint(Point1, Point);
			Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu
			val = 0;
			if ((Point1[0] >= 0) && (Point1[0] < t1) &&
				(Point1[1] >= 0) && (Point1[1] < t2))
			{
				//val= src->GetVoxelValue((int)(Point1[0]),(int)(Point1[1]),sliceNr);
				val = intrp->GetInterpolatedValue(src, Point1[0], Point1[1], sliceNr);
				*(ptr) = val;

				if (rgb == true)
				{
					dstG[pos] = ((InterpolationFunction2DBilinear*)intrp)->GetInterpolatedValue(srcG, nx, ss, Point1[0], Point1[1], sliceNr);
					dstB[pos] = ((InterpolationFunction2DBilinear*)intrp)->GetInterpolatedValue(srcB, nx, ss, Point1[0], Point1[1], sliceNr);
				}
			}
			else
			{
				val = 0;
				*(ptr) = val;
				if (rgb == true)
				{
					dstG[pos] = 0;
					dstB[pos] = 255;
				}
			}
			pos++;
			if (val > 0)
			{
				val = val;
			};
			ptr++;
			//Point[0]+=modelScale[0];
		}
		//Point[1]+=modelScale[1];
	}
}
//---------------------
double ImageSetRegistration::GetDistanceFromRawData(RawDataSet *dist, double x, double y, double slice)
{
	int Nx = dist->GetNx();
	int Ny = dist->GetNy();
	double fval = 0;
	double fvaltmp = 0;
	if ((x < (double)Nx - 1) && (x >= 0.0) && (y < (double)Ny - 1) && (y >= 0.0))
	{
		fval = intrp->GetInterpolatedValue(dist, x, y, slice);
	}
	else
	{
		double qx, qy, dx, dy;

		if ((x >= 0.0) && (x < (double)Nx - 1.0))
			qx = x;
		else
		if (x < 0.0)
			qx = 0.0;
		else
			qx = (double)Nx - 1.0001;


		if ((y >= 0.0) && (y < (double)Ny - 1.0))
			qy = y;
		else
		if (y < 0.0)
			qy = 0.0;
		else
			qy = (double)Ny - 1.0001;

		dx = (x - qx)*intrpVal;
		dy = (y - qy)*intrpVal;
		fval = intrp->GetInterpolatedValue(dist, qx, qy, slice);
		fvaltmp = sqrt(dx*dx + dy*dy);
		fval += fvaltmp;
		//65535 - max(nx,ny)
		//	x - dx
	}
	return fval;
}
//---------------- 
void ImageSetRegistration::PrepareTrans(double *t0, double *t1)
{
	t1[1] = t0[1];//tx
	t1[2] = t0[2];//ty
	t1[6] = t0[3] * to_rad;//rot
	//t1[10]=t0[3];
	//t1[11]=t0[3];

}
//--------------------------------------------
double ImageSetRegistration::GetSqDistSimilarityFunction(double *tr, RawDataSet *distData, double scaleFactor,
	RawDataSet *edgeData, bool inverse)
{
	if ((distData != NULL) && (edgeData != NULL))
	{
		PrepareTrans(tr, t);
		if (inverse == true)
			trans->MakeCompleteAffineMatrixInv(matrix, t);
		else
			trans->MakeCompleteAffineMatrix(matrix, t);

		int i, j;
		double val = 0;
		double fval;
		double *Point = new double[3];
		double *Point1 = new double[3];
		double t1, t2;
		unsigned short *ptr;
		if (inverse == true)
		{
			t1 = edgeData->GetNx() - 1;
			t2 = edgeData->GetNy() - 1;
			ptr = distData->GetDataArray() + sliceNumber*distData->GetSliceSize();;
		}
		else
		{
			t1 = distData->GetNx() - 1;
			t2 = distData->GetNy() - 1;
			ptr = tempData->GetDataArray();//edgeData->GetDataArray()+sliceNumber*edgeData->GetSliceSize();;
		}

		double vSrc, vDst;
		//double scaler=1.0/(double)(10.0);

		//double div=1.0/(65535.0*65535);
		Point[0] = 0; Point[1] = 0; Point[2] = 0;
		int count = 0;
		fval = 0;

		if (inverse == true)
		{
			for (j = 0; j < distData->GetNy(); j++)
			{
				Point[0] = 0;
				for (i = 0; i < distData->GetNx(); i++)
				{
					trans->MultiplyPoint(Point1, Point);
					if ((Point1[0] >= 0) && (Point1[0] < t1) &&
						(Point1[1] >= 0) && (Point1[1]<t2))
					{
						vSrc = intrp->GetInterpolatedValue(edgeData, Point1[0], Point1[1], sliceNumber);
						if (vSrc>0)
						{
							vDst = (*ptr)*scaleFactor;
							fval += vDst*vDst;
							count++;
						}
					}
					ptr++;
					Point[0]++;
				}
				Point[1]++;
			}
		}
		else
		{
			for (j = 0; j < edgeData->GetNy(); j++)
			{
				Point[0] = 0;
				for (i = 0; i<edgeData->GetNx(); i++)
				{
					if ((*ptr)>0)
					{
						trans->MultiplyPoint(Point1, Point);
						vDst = GetDistanceFromRawData(distData, Point1[0], Point1[1], sliceNumber)*scaleFactor;
						val = (vDst*vDst);
						fval += val*val;
						count++;
					}
					ptr++;
					Point[0]++;
				}
				Point[1]++;
			}
		}
		if (count > 0)
			fval /= (double)count;

		fval = sqrt(fval);
		// tr[1]=t[1];
		//  tr[2]=t[2];
		//   tr[3]=t[6];

		//FILE *Do_opa;
		////Do_opa=fopen("c:\\powell3.txt","a");fprintf(Do_opa,"[%lf %lf %lf]  FVal=%lf\n",tr[1],tr[2],tr[3],fval);fclose(Do_opa);

		return fval;
	}
	else
		return 0;
}
//----------------------------------
double ImageSetRegistration::GetFunctionValueDist(double *tr)
{
	double dist1, dist2;
	dist1 = GetSqDistSimilarityFunction(tr, dstDistData, dstScaleDist, edgeSrcData, false);
	//	dist2=GetSqDistSimilarityFunction(tr,srcDistData, srcScaleDist, edgeDstData,true);
	//	double fval=dist1+dist2;
	//	FILE *Do_opa;
	//	//Do_opa=fopen("c:\\powell3.txt","a");fprintf(Do_opa,"[%lf %lf %lf]  d%lf+%lf=%lf\n",tr[1],tr[2],tr[3],dist1,dist2,fval);fclose(Do_opa);

	return dist1;
	//RawDataSet *srcDistData, *dstDistData;
	//double srcScaleDist,dstScaleDist;
	//czynniki do skalowania!!!!
}
//---------------------------------------
void ImageSetRegistration::GetPercentDistMaskData(RawDataSet *distData, RawDataSet *edgeData, RawDataSet *maskData, double percent, double *tr)
{

	PrepareTrans(tr, t);

	trans->MakeCompleteAffineMatrix(matrix, t);

	int i, j;
	double val = 0;
	double fval;
	double *Point = new double[3];
	double *Point1 = new double[3];
	double t1, t2;
	unsigned short *ptr;

	t1 = distData->GetNx() - 1;
	t2 = distData->GetNy() - 1;
	ptr = edgeData->GetDataArray() + sliceNumber*edgeData->GetSliceSize();


	double vSrc, vDst;
	//double scaler=1.0/(double)(10.0);

	//double div=1.0/(65535.0*65535);
	Point[0] = 0; Point[1] = 0; Point[2] = 0;
	int count = 0;
	fval = 0;

	int pos = 0;
	for (j = 0; j < edgeData->GetNy(); j++)
	{
		for (i = 0; i<edgeData->GetNx(); i++)
		{
			if (ptr[pos++]>0)count++;
		}
	}

	IntIntStruct	**tempint = new IntIntStruct*[count];
	pos = 0;
	int pos2 = 0;
	for (j = 0; j < edgeData->GetNy(); j++)
	{
		Point[0] = 0;
		for (i = 0; i<edgeData->GetNx(); i++)
		{
			if ((*ptr)>0)
			{
				trans->MultiplyPoint(Point1, Point);
				vDst = GetDistanceFromRawData(distData, Point1[0], Point1[1], sliceNumber);
				tempint[pos] = new IntIntStruct;
				tempint[pos]->Int2Var = pos2;
				tempint[pos]->Int1Var = vDst;
				pos++;
			}
			ptr++;
			Point[0]++;
			pos2++;
		}
		Point[1]++;

	}
	QSortInt1Int(tempint, 0, count - 1);

	maskData->ZeroQ();

	int cc = percent*count;
	if (cc > count) cc = count;

	ptr = maskData->GetDataArray();
	int xx;
	for (i = 0; i < cc; i++)
	{
		xx = tempint[i]->Int2Var;
		ptr[xx] = 1;
	}

	for (i = 0; i < count; i++)
		delete tempint[i];
	delete tempint;

}
//--------------------------------------------------------
//-----------------------------------------------
void ImageSetRegistration::MatchCenters(double *tr, double *center)
{
	intrpVal = 269;
	rCenter[0] = srcData->GetNx() / 2;
	rCenter[1] = srcData->GetNy() / 2;
	rCenter[2] = 0;

	double tra[3];
	tra[1] = (dstData->GetNx() - srcData->GetNx()) / 2.0;
	tra[2] = (dstData->GetNy() - srcData->GetNy()) / 2.0;



	int i, j, k;
	double Tolerance = 10e-5;
	//	double tolX=10e-8;
	int Iterations;
	double FValue;
	//RawDataSet *dataX=new RawDataSet();
	//->Copy(*dstData,false);
	//RawDataSet *dataX2=new RawDataSet();
	//dataX2->Copy(*srcData,false);

	//tempData=new RawDataSet(edgeSrcData->GetNx(),edgeSrcData->GetNy(),1,1,1,1);
	//tempData->Copy(*edgeSrcData,false);


	//	FILE *Do_opa;
	//	OptimizationPowell *Optim=NULL;
	//	int nr=2;
	//	int subIter=5;
	//	double percentage=0.8;
	i = 28;
	i = sliceNumber;
	//for (i=0;i<dstData->GetNz();i++)
	{

		//	tr[1]=tra[1];
		//	tr[2]=tra[2];
		//	tr[3]=1;
		//tr[4]=1;
		//sliceNumber=i;

		//	Iterations=0;
		//	FValue=0;

		int sizesSrc[4] = { srcData->GetNx(), 0, srcData->GetNy(), 0 };
		int sizesDst[4] = { dstData->GetNx(), 0, dstData->GetNy(), 0 };
		unsigned short *ptr = srcData->GetDataArray() + i*srcData->GetSliceSize();
		int pos = 0;
		for (k = 0; k<srcData->GetNy(); k++)
		{
			for (j = 0; j<srcData->GetNx(); j++)
			{
				if (ptr[pos++]>0)
				{
					if (sizesSrc[0]>j) sizesSrc[0] = j; //minx
					if (sizesSrc[1]<j) sizesSrc[1] = j; //maxx
					if (sizesSrc[2]>k) sizesSrc[2] = k; //miny
					if (sizesSrc[3] < k) sizesSrc[3] = k; //maxy
				}
			}
		}

		ptr = dstData->GetDataArray() + i*dstData->GetSliceSize();
		pos = 0;
		for (k = 0; k<dstData->GetNy(); k++)
		{
			for (j = 0; j<dstData->GetNx(); j++)
			{

				if (ptr[pos++]>0)
				{
					if (sizesDst[0]>j) sizesDst[0] = j; //minx
					if (sizesDst[1]<j) sizesDst[1] = j; //maxx
					if (sizesDst[2]>k) sizesDst[2] = k; //miny
					if (sizesDst[3] < k) sizesDst[3] = k; //maxy
				}


			}
		}

		int ctrSrc[2], ctrDst[2];
		ctrSrc[0] = (sizesSrc[1] + sizesSrc[0]) / 2;
		ctrSrc[1] = (sizesSrc[3] + sizesSrc[2]) / 2;
		ctrDst[0] = (sizesDst[1] + sizesDst[0]) / 2;
		ctrDst[1] = (sizesDst[3] + sizesDst[2]) / 2;

		tr[1] = -(ctrSrc[0] - ctrDst[0]);
		tr[2] = -(ctrSrc[1] - ctrDst[1]);

		center[0] = ctrSrc[0];
		center[1] = ctrSrc[1];
		center[2] = 0;
		//policzyc srodek

		/*	memcpy(	tempData->GetDataArray(),
					edgeSrcData->GetDataArray()+sliceNumber*tempData->GetSliceSize(),
					tempData->GetSliceSize()*sizeof(unsigned short));

					for (j=0;j<subIter;j++)
					{
					Optim=new OptimizationPowell(tr,nr,Tolerance,&Iterations,&FValue,this);
					Optim->StartOptimization();
					GetPercentDistMaskData(dstDistData,edgeSrcData,tempData, percentage,tr);
					}

					TransformSlice(srcData, dataX, i,tr);
					TransformSlice(modelData, dataX2, i,tr,false);

					TransformSlice(srcData, dataX, i,tr);*/
		//TransformSlice(modelData, dataX2, i,tr,false);

		//emit SendMessage(QString::number(i)+"/"+QString::number(dstData->GetNz()-1));
		/*//Do_opa=fopen("c:\\imgrg.txt","a");
		fprintf(Do_opa,"%d [" ,i);

		for (j=1;j<=nr;j++)
		fprintf(Do_opa,"%lf " ,tr[j]);

		fprintf (Do_opa, "\n");
		fclose(Do_opa);*/


	}
	/*dataX->SetRawDataSetName("transData");
	emit 	SignalSendRawDataSet(dataX);	*/


	//     TransformImage(tr);
}
void  ImageSetRegistration::SetColorData(RawDataSet *s, RawDataSet *d)
{
	colSrcData = s;
	colDstData = d;
}
//-------------------------------------------------------
void  ImageSetRegistration::FindBestFittingCandidats()
{

	// tworze tablice w ktorej umieszcze wynik funkcji podobienstwa
	double *tab = new double[objectData->GetNz()*modelData->GetNz()];

	int i, j, k;
	double tr[3];
	int pos = 0;

	//j=0;
	for (j = 0; j < modelData->GetNz(); j++)
	{
		//i=0;		
		emit	SendMessage("(" + QString::number(j) + "/" + QString::number(modelData->GetNz() - 1) + ")");

		for (i = 0; i < objectData->GetNz(); i++)
		{
			//1 zgrywam srodki zbiorow dst(j), src(i)
			MatchCenters(i, j, tr);
			//2 wyliczam podobienstwo i umieszczam w tablicy
			tab[pos++] = CalculateSumOfSquareDifferencesSimilarity(i, j, tr);



		}
	}

	RawDataSet *r = new RawDataSet(objectData->GetNz(), modelData->GetNz(), 1, 10, 1, 1);
	unsigned short *tabx = r->GetDataArray();

	RawDataSet *r1 = new RawDataSet(objectData->GetNz(), modelData->GetNz(), 1, 10, 1, 1);
	r1->ZeroQ();



	double max = 0;
	for (i = 0; i<r->GetVolumeSize(); i++)
	{
		if (tab[i]>max) max = tab[i];
	}

	if (max == 0) max = 1;
	for (i = 0; i < r->GetVolumeSize(); i++)
	{
		tabx[i] = (tab[i] * 65535.0) / max;
		/*if (tabx[i]<10)
		{
		FILE *Do_opa;
		//Do_opa=fopen("c:\\imgrgaaaff.txt","w");
		fprintf(Do_opa,"%lf ",tab[i]);
		fclose(Do_opa);
		}*/
	}


	//znalezc najwieksza roznice 

	int nrO, nrM;
	double mmin, mmax, delta;
	int pos2;
	delta = 0;
	nrO = 0;
	nrM = 0;

	for (i = 0; i < objectData->GetNz(); i++)
	{
		//i=0;
		mmin = max;
		mmax = 0;
		for (j = 0; j<modelData->GetNz(); j++)
		{
			pos = r->GetVoxelPos(i, j, 0);
			if (tab[pos]>mmax) { mmax = tab[pos]; }
			if (tab[pos]<mmin) { mmin = tab[pos]; pos2 = j; }
		}
		if (mmin>delta)
		{
			nrO = i;
			delta = mmin;
			nrM = pos2;
		}
	}
	QString aa = QString::number(nrO) + " " + QString::number(nrM) + "   ";

	//a potem poszukac po kolei minimow

	RawDataSet *dS = new RawDataSet(modelData->GetNx(), modelData->GetNy(), objectData->GetNz());
	RawDataSet *dD = new RawDataSet(modelData->GetNx(), modelData->GetNy(), objectData->GetNz());
	dS->initRGBArrays();
	dD->initRGBArrays();

	unsigned short *tM = modelData->GetDataArray();
	//int pp=nr;
	pos = 0;
	unsigned short *tS = dS->GetDataArray();
	unsigned short *tD = dD->GetDataArray();
	int x, y;
	bool mfound = false;
	int nzzD = modelData->GetNz();
	int nzzS = objectData->GetNz();
	//	pp=0;
	int nrO2, nrM2;

	nrO2 = nrO%nzzS;
	nrM2 = nrM%nzzD;
	int posO2 = nrO2;
	int l;
	int posx, pos3;
	int tmp;

	unsigned short *dsRTab = dS->GetRChannelArray();
	unsigned char  *dsGTab = dS->GetGChannelUCHARArray();
	unsigned char  *dsBTab = dS->GetBChannelUCHARArray();
	unsigned short *ddRTab = dD->GetRChannelArray();
	unsigned char  *ddGTab = dD->GetGChannelUCHARArray();
	unsigned char  *ddBTab = dD->GetBChannelUCHARArray();

	/*memset (dsRTab,0,dS->GetVolumeSize()*2);
	memset (dsGTab,0,dS->GetVolumeSize());
	memset (dsBTab,255,dS->GetVolumeSize());

	memset (ddRTab,0,dS->GetVolumeSize()*2);
	memset (ddGTab,0,dS->GetVolumeSize());
	memset (ddBTab,255,dS->GetVolumeSize());*/

	unsigned short *srcRTab = objectData->GetRChannelArray();
	unsigned char  *srcGTab = objectData->GetGChannelUCHARArray();
	unsigned char  *srcBTab = objectData->GetBChannelUCHARArray();
	unsigned short *dstRTab = modelData->GetRChannelArray();
	unsigned char  *dstGTab = modelData->GetGChannelUCHARArray();
	unsigned char  *dstBTab = modelData->GetBChannelUCHARArray();
	//nrO2+=nzzS;
	FILE *Do_opa;
	//Do_opa=fopen("c:\\imgrgaaa.txt","w");
	int deltaPos = dS->GetVoxelPos(0, 0, nrO2);
	int posA, pos2A;
	for (k = 0; k < dS->GetNz(); k++)
	{


		r1->SetVoxelValue(nrO2, nrM2, 0, 255);
		pos2 = modelData->GetVoxelPos(0, 0, nrM2);
		MatchCenters(nrO2, nrM2, tr);
		//		fprintf(Do_opa,"%d %d (%lf %lf)\n",nrO2,nrM2,tr[1],tr[2]);

		//posA=objectData->GetVoxelPos(0,0,nrO2-posO2+);
		//posA=objectData->GetVolumeSize()-(pos-deltaPos+objectData->GetVolumeSize()-objectData->GetSliceSize())%objectData->GetVolumeSize()-objectData->GetSliceSize();
		posA = dS->GetVolumeSize() - (pos - deltaPos + dS->GetVolumeSize() - dS->GetSliceSize()) % dS->GetVolumeSize() - dS->GetSliceSize();

		for (j = 0; j < dS->GetNy(); j++)
		{
			for (i = 0; i < dS->GetNx(); i++)
			{
				//tD[pos]=tM[pos2];

				ddRTab[posA] = dstRTab[pos2];
				ddGTab[posA] = dstGTab[pos2];
				ddBTab[posA] = dstBTab[pos2];


				x = i - tr[1];
				y = j - tr[2];

				if ((x >= 0) && (x < objectData->GetNx()) && (y >= 0) && (y < objectData->GetNy()))
				{
					tmp = objectData->GetVoxelPos(x, y, nrO2);
					//tS[pos]=objectData->GetVoxelValue(x,y,nrO2);
					dsRTab[posA] = srcRTab[tmp];
					dsGTab[posA] = srcGTab[tmp];
					dsBTab[posA] = srcBTab[tmp];
					//pos1=objectData->StVoxelPos(x,y,posSrc);
				}
				else
				{
					dsRTab[posA] = 0;
					dsGTab[posA] = 0;
					dsBTab[posA] = 255;
					//	tS[pos]=0;
				}
				posA++;
				pos++;
				pos2++;
			}
		}



		//find next minimum!

		mfound = false;

		nrO2--;
		if (nrO2 < 0) nrO2 += nzzS;
		nrO2 = nrO2%nzzS;

		pos2 = nrO2*(nzzD);
		/*do
		{
		nrM2--;
		if (tab[(nrM2+nzzD)%nzzD+pos2]>tab[(nrM2+nzzD-1)%nzzD+pos2])
		{
		mfound=true;
		nrM2=(nrM2+nzzD)%nzzD;
		}

		}while (mfound==false);*/

		mmin = max;
		mmax = 0;
		tmp = nrM2;
		for (l = 1; l<modelData->GetNz() / 4; l++)
		{

			posx = r->GetVoxelPos(nrO2, (tmp + l) % nzzD, 0);
			if (tab[posx]>mmax) { mmax = tab[posx]; }
			if (tab[posx] < mmin) { mmin = tab[posx]; nrM2 = (tmp + l) % nzzD; }
		}


		//nr+=1;
		//pp=nr%nzz;

	}
	//	fclose(Do_opa);

	delete[]tab;
	r->SetRawDataSetName("mapka");
	r1->SetRawDataSetName("mapka1");
	dS->SetRawDataSetName("src");
	dD->SetRawDataSetName("dst");

	emit 	SignalSendRawDataSet(r);
	emit 	SignalSendRawDataSet(r1);
	emit	SignalSendRawDataSet(dS);
	emit	SignalSendRawDataSet(dD);
	emit	SendMessage(aa);




}
//-------------------------------------
double ImageSetRegistration::CalculateSumOfSquareDifferencesSimilarityGrayImg(RawDataSet *src, RawDataSet *dst, double *tr, bool inv)
{

	int i, j;

	int count = 0;
	PrepareTrans(tr, t);


	if (inv == true)
		trans->MakeCompleteAffineMatrixInv(matrix, t);
	else
		trans->MakeCompleteAffineMatrix(matrix, t);

	int pos = dst->GetSliceSize();
	int v = dst->GetVolumeSize();
	bool rgb = false;

	int nx = src->GetNx();
	int ss = src->GetSliceSize();
	int slicesize = dst->GetSliceSize();
	double fr = 0;;
	double f = 0;
	//max factor daje taki sam tutaj dla src i dst i jest ustawiony globalnie!

	double maxFactorX = (maxFactor > 0) ? maxFactor : 1;
	parallelComputing = false;
	
	if (parallelComputing == false)
	{
		unsigned short* ptr = (dst->GetDataArray());
		unsigned short* srcD = (src->GetDataArray());
		//rr->ZeroQ();
		double* Point = new double[3];
		double* Point1 = new double[3];
		double t1 = src->GetNx() - 1;
		double t2 = src->GetNy() - 1;
		Point[0] = 0; Point[1] = 0; Point[2] = 0;

		double val;
	
		double mm, oo;

	
		
		double ff = 1.0 / 255.0;

		for (j = 0; j < dst->GetNy(); j++)
		{
			Point[0] = 0;
			for (i = 0; i < dst->GetNx(); i++)
			{
				trans->MultiplyPoint(Point1, Point);
				val = 0;
				if ((Point1[0] >= 0) && (Point1[0] < t1) &&
					(Point1[1] >= 0) && (Point1[1] < t2))
				{
					//val= src->GetVoxelValue((int)(Point1[0]),(int)(Point1[1]),sliceNr);
					mm = *ptr * maxFactorX;
					oo = intrp->GetInterpolatedValue(src, Point1[0], Point1[1], 0) * maxFactorX;
					if ((oo > 0) && (mm > 0))
					{
						val = (mm - oo);
						val *= val;
						fr += val;
						//*(ptr)=val;
						count++;
					}
				}
				pos++;
				if (val > 0)
				{
					val = val;
				};
				ptr++;
				Point[0]++;
			}
			Point[1]++;
		}
	}
	else
	{
		
		double* tabsEnergy=NULL;
		int* counts=NULL;
		//numberOfThreadsUsed = 1;
	//	numberOfThreadsUsed = 1;
		int nntt = omp_get_num_threads();;
	
		tabsEnergy = new double[numberOfThreadsUsed];
		counts = new int[numberOfThreadsUsed];
		for (int k = 0; k < this->numberOfThreadsUsed; k++)
		{
			tabsEnergy[k] = 0;
			counts[k] = 0;

		}

#pragma omp parallel  num_threads(numberOfThreadsUsed) //private(counts,tabsEnergy)//private (counts,tabs)//shared(Q1,paddQ)  //num_threads(1)
		{
			

			
			int tid = omp_get_thread_num();

			RawDataSet* sData = src;
			RawDataSet* dData = dst;
//#pragma omp for  nowait ordered schedule (guided) default(none) 
//#pragma omp parallel for default(none) 
			int j;
			if (inv == true)
			{
#pragma omp for  nowait ordered schedule (guided) private (j)
				for (int j = 0; j < dst->GetNy(); j++)
				{
					AddSqDistEnergy(j, tabsEnergy[tid], counts[tid], sData, dData, tid);
				}
			}

			else
			{
#pragma omp for  nowait ordered schedule (guided) private (j)
				for (int j = 0; j < src->GetNy(); j++)
				{
					AddSqDistEnergy(j, tabsEnergy[tid], counts[tid], dData, sData, tid);
				}
			}
#pragma omp critical
			for (int i = 0; i < this->numberOfThreadsUsed; i++)
			{
				count += counts[i];
				fr += tabsEnergy[i];
			}



		
		}
		if (tabsEnergy!=NULL)delete[]tabsEnergy;
		if (counts!=NULL) delete[]counts;
	
	}
	if (count < 0.5*slicesize) return INT_MAX;// count=1;
	//f=(0.299*fr + 0.587*fg + 0.114*fb)/count;
	f = (fr / count);
	return f;

}
//-----------------------------------
void ImageSetRegistration::AddSqDistEnergy(int j, double& energy, int& count, RawDataSet* src, RawDataSet* dst,int tid)
{

	double Point[3];
	Point[0] = 0; Point[1] = j; Point[2] = 0;
	double Point1[3];

	double val;
	int dstNx = dst->GetNx();
	unsigned short* ptr = dst->GetDataArray() + j * dstNx;
	
	int srcNx = src->GetNx();
	int srcNy = src->GetNy();
	double mm, oo;
	//max factor daje taki sam tutaj dla src i dst i jest ustawiony globalnie!
	
	double maxFactorX = (maxFactor > 0) ? maxFactor : 1;

	trans->MultiplyPoint(Point1, Point);
	double Point2[3] = { Point[0] + 1,Point[1],0 };
	double Point1a[3];
	trans->MultiplyPoint(Point1a, Point2);

	double diffx = Point1a[0] - Point1[0];
	double diffy = Point1a[1] - Point1[1];
	for (int i = 0; i < dstNx; i++)
	{

		if ((Point1[0] >= 0) && (Point1[0] < srcNx-1) &&
			(Point1[1] >= 0) && (Point1[1] < srcNy-1))
		{

			mm = *ptr * maxFactorX;
			oo = intrp->GetInterpolatedValue(src, Point1[0], Point1[1], 0) * maxFactorX;
			
			val = (mm - oo);
			val *= val;
			energy += val;
	
			count++;
			ptr++;
		}
		//Point[0]++;
		Point1[0] += diffx;
		Point1[1] += diffy;

	}


}
//-----------------------------------
double ImageSetRegistration::CalculateSumOfSquareDifferencesSimilarity(RawDataSet *src, RawDataSet *dst, int sliceNr, double *tr, bool inv)
{
	int i, j;
	/*for(i=0;i<15;i++)
		t[i]=0;
		t[10]=1;
		t[11]=1;
		t[12]=1;*/
	int count = 0;
	PrepareTrans(tr, t);

	//t[1]=tr[1];
	//t[2]=tr[2];
	//t[11]=tr[3];
	//t[10]=tr[3];
	//t[11]=tr[4];
	//RawDataSet *rr =new RawDataSet(rdst->GetNx(),rdst->GetNy(),1,1,1,1);
	if (inv == true)
		trans->MakeCompleteAffineMatrixInv(matrix, t);
	else
		trans->MakeCompleteAffineMatrix(matrix, t);

	int pos = sliceNr*dst->GetSliceSize();
	int v = dst->GetVolumeSize();
	bool rgb = false;
	unsigned char *srcG, *srcB, *dstG, *dstB;

	if ((dst->GetRGBExists() == true) && (src->GetRGBExists() == true))
	{
		rgb = true;
		srcG = src->GetGChannelUCHARArray();
		srcB = src->GetBChannelUCHARArray();
		dstB = dst->GetBChannelUCHARArray();
		dstG = dst->GetGChannelUCHARArray();
	}

	unsigned short *ptr = (dst->GetDataArray() + sliceNr*dst->GetSliceSize());
	unsigned short *srcD = (src->GetDataArray() + sliceNr*src->GetSliceSize());
	//rr->ZeroQ();
	double *Point = new double[3];
	double *Point1 = new double[3];
	double t1 = src->GetNx() - 1;
	double t2 = src->GetNy() - 1;
	Point[0] = 0; Point[1] = 0; Point[2] = 0;

	unsigned short val;

	int nx = src->GetNx();
	int ss = src->GetSliceSize();

	double fr, fg, fb;
	double f = 0;
	fr = 0;
	fg = 0;
	fb = 0;
	for (j = 0; j < dst->GetNy(); j++)
	{
		Point[0] = 0;
		for (i = 0; i < dst->GetNx(); i++)
		{
			trans->MultiplyPoint(Point1, Point);
			val = 0;
			if ((Point1[0] >= 0) && (Point1[0] < t1) &&
				(Point1[1] >= 0) && (Point1[1] < t2))
			{
				//val= src->GetVoxelValue((int)(Point1[0]),(int)(Point1[1]),sliceNr);
				val = *ptr - intrp->GetInterpolatedValue(src, Point1[0], Point1[1], sliceNr);
				val *= val;
				fr += val;
				//*(ptr)=val;


				if (rgb == true)
				{
					val = dstG[pos] - ((InterpolationFunction2DBilinear*)intrp)->GetInterpolatedValue(srcG, nx, ss, Point1[0], Point1[1], sliceNr);
					val *= val;
					fg += val;

					val = dstB[pos] - ((InterpolationFunction2DBilinear*)intrp)->GetInterpolatedValue(srcB, nx, ss, Point1[0], Point1[1], sliceNr);
					val *= val;
					fb += val;

				}
				count++;
			}
			pos++;
			if (val > 0)
			{
				val = val;
			};
			ptr++;
			Point[0]++;
		}
		Point[1]++;
	}

	if (count == 0) count = 1;
	//f=(0.299*fr + 0.587*fg + 0.114*fb)/count;
	f = fb / count;
	return f;
}
//-----------------------------------------------
double ImageSetRegistration::CalculateSumOfSquareDifferencesSimilarity(int posSrc, int posDst, double *tr)
{
	double f = 0;
	double fr, fg, fb;

	unsigned short *srcRTab = objectData->GetRChannelArray();
	unsigned char  *srcGTab = objectData->GetGChannelUCHARArray();
	unsigned char  *srcBTab = objectData->GetBChannelUCHARArray();
	unsigned short *dstRTab = modelData->GetRChannelArray();
	unsigned char  *dstGTab = modelData->GetGChannelUCHARArray();
	unsigned char  *dstBTab = modelData->GetBChannelUCHARArray();

	//int posSrc,posDst;
	int pos1, pos2;
	int x, y;
	int count = 0;
	double val;
	f = 0;
	fr = 0;
	fg = 0;
	fb = 0;

	int i, j, k;
	//RawDataSet *tt=new RawDataSet(modelData->GetNx(),modelData->GetNy(),1,1,1,1);

	for (j = 0; j < modelData->GetNy(); j++)
	{
		for (i = 0; i < modelData->GetNx(); i++)
		{

			x = i - tr[1];
			y = j - tr[2];
			if ((x >= 0) && (x < objectData->GetNx()) && (y >= 0) && (y < objectData->GetNy()))
			{
				pos1 = objectData->GetVoxelPos(x, y, posSrc);
				pos2 = modelData->GetVoxelPos(i, j, posDst);

				//tt->SetVoxelValue(i,j,0,srcRTab[pos1]);
				val = srcRTab[pos1] - dstRTab[pos2];
				val *= val;
				fr += val;

				val = srcGTab[pos1] - dstGTab[pos2];
				val *= val;
				fg += val;

				val = srcBTab[pos1] - dstBTab[pos2];
				val *= val;
				fb += val;

				count++;
			}


		}
	}
	if (count == 0) count = 1;
	//emit SignalSendRawDataSet(tt);
	//f=(fr+fg/2+fb/3)/255.0;
	f = (0.299*fr + 0.587*fg + 0.114*fb) / count;


	return f;

}
//-------------------------------------
void ImageSetRegistration::MatchCenters(int posSrc, int posDst, double *tr)
{

	if ((objectData->GetRGBExists()) && (modelData->GetRGBExists()))
	{

		int srcBnds[4];
		int dstBnds[4];

		srcBnds[0] = INT_MAX;
		srcBnds[1] = INT_MIN;
		srcBnds[2] = INT_MAX;
		srcBnds[3] = INT_MIN;
		dstBnds[0] = INT_MAX;
		dstBnds[1] = INT_MIN;
		dstBnds[2] = INT_MAX;
		dstBnds[3] = INT_MIN;

		int i, j, k;
		unsigned char r, g, b;
		unsigned short *rTab;
		unsigned char  *gTab;
		unsigned char  *bTab;

		int pos;
		int xx, yy;
		int pos2;

		rTab = objectData->GetRChannelArray();
		gTab = objectData->GetGChannelUCHARArray();
		bTab = objectData->GetBChannelUCHARArray();
		pos = 0;
		for (j = 0; j < objectData->GetNy(); j++)
		{
			for (i = 0; i<objectData->GetNx(); i++)
			{
				pos = objectData->GetVoxelPos(i, j, posSrc);
				r = (unsigned char)rTab[pos];		g = gTab[pos];		b = bTab[pos];

				if (!((r<25) && (g<25) && (b>200)))
				{
					if (i>srcBnds[1]) srcBnds[1] = i;			if (j>srcBnds[3]) srcBnds[3] = j;
					if (i < srcBnds[0]) srcBnds[0] = i;			if (j < srcBnds[2]) srcBnds[2] = j;
				}
			}
		}

		rTab = modelData->GetRChannelArray();
		gTab = modelData->GetGChannelUCHARArray();
		bTab = modelData->GetBChannelUCHARArray();
		pos = 0;
		for (j = 0; j < modelData->GetNy(); j++)
		{
			for (i = 0; i<modelData->GetNx(); i++)
			{
				pos = modelData->GetVoxelPos(i, j, posDst);
				r = (unsigned char)rTab[pos];		g = gTab[pos];		b = bTab[pos];

				if (!((r<25) && (g<25) && (b>200)))
				{
					if (i>dstBnds[1]) dstBnds[1] = i;			if (j>dstBnds[3]) dstBnds[3] = j;
					if (i < dstBnds[0]) dstBnds[0] = i;			if (j < dstBnds[2]) dstBnds[2] = j;
				}
			}
		}

		int ctrSrc[2], ctrDst[2];
		ctrSrc[0] = (srcBnds[1] + srcBnds[0]) / 2;
		ctrSrc[1] = (srcBnds[3] + srcBnds[2]) / 2;
		ctrDst[0] = (dstBnds[1] + dstBnds[0]) / 2;
		ctrDst[1] = (dstBnds[3] + dstBnds[2]) / 2;

		tr[1] = -(ctrSrc[0] - ctrDst[0]);
		tr[2] = -(ctrSrc[1] - ctrDst[1]);


	}

}

//--------------------------------------

void ImageSetRegistration::ApplyRegistration2()
{
	//intrpVal=269;
	rCenter[0] = objectData->GetNx() / 2;
	rCenter[1] = objectData->GetNy() / 2;
	rCenter[2] = 0;

	double tra[3];
	tra[1] = (modelData->GetNx() - objectData->GetNx()) / 2.0;
	tra[2] = (modelData->GetNy() - objectData->GetNy()) / 2.0;

	double tr[10];
	tr[1] = 0;
	tr[2] = 0;
	tr[3] = 0;
	int i, j, k;
	for (i = 0; i < 15; i++)
		t[i] = 0;
	t[10] = 1;
	t[11] = 1;
	t[12] = 1;

	bool tmpBool;
	double Tolerance = 10e-5;
	//	double tolX=10e-8;
	int Iterations;
	double FValue;
	//RawDataSet *dataX=new RawDataSet();
	//dataX->Copy(*dstData,false);
	//RawDataSet *dataX=new RawDataSet();
	//dataX2->Copy(*modelData,false);

	//tempData=new RawDataSet(edgeSrcData->GetNx(),edgeSrcData->GetNy(),1,1,1,1);
	//tempData->Copy(*edgeSrcData,false);
	//int tab[12]={1,2,3,4,15,16,24,25,34,35,36,42};


	//	FILE *Do_opa;
	OptimizationPowell *Optim = NULL;
	int nr = 1;
	//int subIter=5;
	//double percentage=0.5;
	//i=28;
	RawDataSet *colDstData2 = NULL;
	if ((modelData->GetRGBExists()) && (objectData->GetRGBExists()))
	{
		colDstData2 = new RawDataSet();
		colDstData2->Copy(*modelData, false);
		colDstData2->initRGBArrays();
		colDstData2->ZeroQ();
	}

	//wkopiuje wszedzie 42 slice;


	//for (i=0;i<dstData->GetNz();i++)
	for (i = 0; i < modelData->GetNz(); i++)
	{

		sliceNumber = i;
		//MatchCenters(t,rCenter);

		Iterations = 0;
		FValue = 0;

		Optim = NULL;
		tmpBool = false;
		tr[1] = 0;
		tr[2] = 0;
		tr[3] = 0;
		//Do_opa=fopen("c:\\imgrg.txt","a");
		//	fprintf(Do_opa,"%lf \t" ,CalculateSumOfSquareDifferencesSimilarity(objectData,modelData, sliceNumber,tr,true));		
		Optim = new OptimizationPowell(tr, nr, Tolerance, &Iterations, &FValue, this);
		Optim->StartOptimization();



		//TransformSlice(srcData, dataX, i,tr);
		//TransformSlice(modelData, dataX2, i,tr,false);

		if ((colDstData2 != NULL))
		{
			TransformSlice(objectData, colDstData2, i, tr, false);
		}








		//emit SendMessage(QString::number(i)+"/"+QString::number(dstData->GetNz()-1));

		//fprintf(Do_opa,"%d [" ,i);

		//	for (j=1;j<=nr;j++)
		//	fprintf(Do_opa,"%lf " ,tr[j]);

		//fprintf (Do_opa, " ] - %lf\n",FValue);
		//fclose(Do_opa);
		if (Optim != NULL)delete Optim;

		//tymczasowe!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		///////////////

	}

	if ((colDstData2 != NULL))
	{
		colDstData2->SetRawDataSetName("final");
		emit 	SignalSendRawDataSet(colDstData2);
	}

	//     TransformImage(tr);
}


//-------------
void ImageSetRegistration::GetSimTab(double* simTab,double *stabTab)
{
	scatterSize = paramsReg[6];
	grayMode = false;
	//intrpVal=269;
	rCenter[0] = modelData->GetNx() / 2;
	rCenter[1] = modelData->GetNy() / 2;
	rCenter[2] = 0;

	//double tra[3];
	//tra[1]=0;
	//tra[2]=0;

	double tr[4];
	tr[1] = 0;
	tr[2] = 0;
	tr[3] = 0;
	int i, j, k;
	for (i = 0; i < 15; i++)
		t[i] = 0;
	t[10] = 1;
	t[11] = 1;
	t[12] = 1;
	
	bool tmpBool;
	double Tolerance = 10e-3;

	int Iterations;
	double FValue;
	double trTotal[4] = { 0 };

	//RawDataSet* 
		filteredModel = NULL;
	//RawDataSet*
		filteredObject = NULL;
	unsigned short* a1, * a2;
	mSliceData = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, 1, 1, 1, 0, false);
	oSliceData = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, 1, 1, 1, 0, false);

	int calcZ = modelData->GetNz();
	maxFactor = modelData->GetMaxValue();
	maxFactor = (maxFactor > 0) ? 255.0 / maxFactor : 1.0;
	globalMaxValue= modelData->GetMaxValue();
	useGlobalMax = true;
	bool filterMode = (!prefilterName.isEmpty()) ? 1 : 0;

	bounds[0] = 0;
	bounds[2] = 0;
	bounds[1] = modelData->GetNx() -1;
	bounds[3] = modelData->GetNy() - 1;
	if (filterMode == true)
	{
		filteredModel = new RawDataSet();
		filteredModel->Copy(*mSliceData, false);
		a1 = modelData->GetDataArray();
		mSliceData->SetDataArray(a1, false); // 
		FilterDataSet(mSliceData, filteredModel, prefilterName);
		filteredModel->FindMinMaxValue();
		filteredObject = new RawDataSet();
		filteredObject->Copy(*oSliceData, false);

	}
	else
	{
		filteredModel = mSliceData;
		filteredObject = oSliceData;
		a1 = modelData->GetDataArray()  ;
		mSliceData->SetDataArray(a1, false); // 
		filteredModel->FindMinMaxValue();
	}
	double val;
	numOfParams = 2;
	int nr = numOfParams;
	int* intBuffers[2] = { NULL, NULL };
	int ccount = 0;

	int samplingX = paramsReg[4];
	int samplingY = paramsReg[5];
	posTab[0] = 0;
	posTab[1] = 1;
	posTab[2] = 2;
	posTab[3] = 6;
	

	imgMaskBits = NULL;

	for (int i = 1; i < calcZ;i++)
	{
		tr[0] = 0;
		tr[1] = 0;
		tr[2] = 0;
		tr[3] = 0;
	
		a2 = modelData->GetDataArray() + modelData->GetSliceSize() * i;
		if (filterMode == false)
		{
			filteredObject->SetDataArray(a2, false); //
			filteredObject->FindMinMaxValue();
		}
		else
		{
			oSliceData->SetDataArray(a2, false); //
			oSliceData->FindMinMaxValue();
		}

		if (filterMode == true)
		{
			FilterDataSet(oSliceData, filteredObject, prefilterName);
			filteredObject->FindMinMaxValue();
		}
		PrepareTrans2(tr, t, nr, posTab);
	

		bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter, samplingX, samplingY);
		val = statsMutual[functionNr];
		
		if (stabTab != NULL)
		{
			OptimizationPowell* Optim = new OptimizationPowell(tr, nr, Tolerance, &Iterations, &FValue, this);
			Optim->StartOptimization();
			delete Optim;
			double after = this->GetFunctionValue(tr);
			stabTab[i * 3] = tr[1];
			stabTab[i * 3+1] = tr[2];
			stabTab[i * 3+2] = after;
		}

		qDebug() << i << " " << tr[1] << " " << tr[2];

		if (i == calcZ - 1)
		{
			i = i;
		}
		simTab[i] = val;//this->GetFunctionValue(tr);

		//if (filterMode == true)
		{
			RawDataSet* swapData = filteredObject;
			filteredObject = filteredModel;
			filteredModel = swapData;


		}

	}

	mSliceData->SetDataArray(NULL, false);
	oSliceData->SetDataArray(NULL, false);

	delete mSliceData;
	delete oSliceData;

	if (filterMode == true)
	{
		delete filteredObject;
		delete  filteredModel;
		//skasowac filtryu
	}
}
//--------------------------------------------------
void ImageSetRegistration::ApplyStabilization(double medThresh)
{
	scatterSize = paramsReg[6];
	grayMode = true;
	//intrpVal=269;
	rCenter[0] = modelData->GetNx() / 2;
	rCenter[1] = modelData->GetNy() / 2;
	rCenter[2] = 0;

	//double tra[3];
	//tra[1]=0;
	//tra[2]=0;

	double tr[4];
	tr[1] = 0;
	tr[2] = 0;
	tr[3] = 0;
	int i, j, k;
	for (i = 0; i < 15; i++)
		t[i] = 0;
	t[10] = 1;
	t[11] = 1;
	t[12] = 1;

	bool tmpBool;
	double Tolerance = 10e-3;

	int Iterations;
	double FValue;
	double trTotal[4] = { 0 };

	OptimizationPowell *Optim = NULL;
	/*int nr = 2;*/

	//RawDataSet *colDstData2 = NULL;

	//{
	//	colDstData2 = new RawDataSet();
	//	colDstData2->Copy(*modelData, false);

	//	colDstData2->ZeroQ();
	//}

	unsigned short *a1, *a2;
	mSliceData = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, 1, 1, 1, 0, false);
	oSliceData = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, 1, 1, 1, 0, false);
	


	maxFactor = modelData->GetMaxValue();
	maxFactor = (maxFactor > 0) ? 255.0 / maxFactor : 1.0;
	//memcpy(colDstData2->GetDataArray(), modelData->GetDataArray(), modelData->GetSliceSize()*sizeof(unsigned short));
	//wypelniony slice 0 kopia z oryginalu

	//i=1;
	// bede sprawdzal roznice!!!
	double* simTab = new double[modelData->GetNz()];



	int calcZ= modelData->GetNz();


	for (int i = 1; i < calcZ ;i++)
	{
		tr[0] = 0;
		tr[1] = 0;
		tr[2] = 0;
		tr[3] = 0;
		a1 = modelData->GetDataArray() + modelData->GetSliceSize() * (i-1);
		a2 = modelData->GetDataArray() + modelData->GetSliceSize() * i;
		mSliceData->SetDataArray(a1, false); // 
		oSliceData->SetDataArray(a2, false); //
		simTab[i] = this->GetFunctionValue(tr);

		//emit SendMessageToLog(QString::number(i) + " " + QString::number(simTab[i]));
	}
	QString filename = "d:\\t\\simBefore.txt";

	QFile funcFile(filename);
	if (funcFile.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&funcFile);
		QString line;
		for (int i = 1; i < calcZ; i++)
		{
			stream << simTab[i] << "\n";
		}

		funcFile.close();
	}

	double* tmp = new double[modelData->GetNz() * sizeof(double)];
	memcpy(tmp, simTab, modelData->GetNz() * sizeof(double));
	double median = Median(tmp, modelData->GetNz() - 1);
	delete[]tmp;

	RawDataSet *colDstData2 = NULL;
	{
		colDstData2 = new RawDataSet();
		colDstData2->Copy(*modelData, false);

		colDstData2->ZeroQ();
	}
	memcpy(colDstData2->GetDataArray(), modelData->GetDataArray(), modelData->GetSliceSize() * sizeof(unsigned short));
	//pierwsz skopiowany
	int nr = 2;
	double totalTr[4] = { 0,0,0,0 };
	RawDataSet *dstSlice = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, 1, 1, 1, 0, false);
	nrMissPixels = 50.0;
	int ssize = 0;
	int nz = modelData->GetNz();
	bool* matchTab = new bool[nz];
	memset(matchTab, 0, nz);
	for (int i = 0;i < nz;i++)
	{
		if (!(simTab[i] < median * medThresh))
		{
			for (int j = i - ssize;j <= i + ssize;j++)
			{
				if (!((j < 0) || (j > nz - 1)))
				{
					matchTab[j] =true;
				}
			}
		}
	}
	for (int i = 1; i < modelData->GetNz(); i++)
	{
		
		a1 = modelData->GetDataArray() + modelData->GetSliceSize() * (i - 1);
		a2 = modelData->GetDataArray() + modelData->GetSliceSize()*(i);
		mSliceData->SetDataArray(a1, false);  
   		oSliceData->SetDataArray(a2, false); 

		dstSlice->SetDataArray(colDstData2->GetDataArray() + modelData->GetSliceSize() * (i), false);
		//if (!(simTab[i] < median *1.5 ))
		if (matchTab[i] == true)
		{

			Iterations = 0;
			FValue = 0;
			double tr[4] = { 0,0,0,0 };
			for (int j = 0;j < 4;j++)
				tr[j]= totalTr[j];

			double before = this->GetFunctionValue(tr);
			Optim = new OptimizationPowell(tr, nr, Tolerance, &Iterations, &FValue, this);
			Optim->StartOptimization();
			delete Optim;
			double after = this->GetFunctionValue(tr);
			//TransformSlice(oSliceData, dstSlice, 0, totalTr, true);
			for (int j = 0;j < 4;j++)
				totalTr[j] += tr[j];

		}
		TransformSlice(oSliceData, dstSlice, 0, totalTr, true);

//	oSliceData->SetDataArray(a2, false); //
	}
	{
		for (int i = 1; i < calcZ;i++)
		{
			tr[0] = 0;
			tr[1] = 0;
			tr[2] = 0;
			tr[3] = 0;
			a1 = colDstData2->GetDataArray() + colDstData2->GetSliceSize() * (i - 1);
			a2 = colDstData2->GetDataArray() + colDstData2->GetSliceSize() * i;
			mSliceData->SetDataArray(a1, false); // 
			oSliceData->SetDataArray(a2, false); //
			simTab[i] = this->GetFunctionValue(tr);

			//emit SendMessageToLog(QString::number(i) + " " + QString::number(simTab[i]));
		}
		QString filename = "d:\\t\\simAfter.txt";

		QFile funcFile(filename);
		if (funcFile.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&funcFile);
			QString line;
			for (int i = 1; i < calcZ; i++)
			{
				stream << simTab[i] << "\n";
			}

			funcFile.close();
		}
	}

	//for (i = 1; i < modelData->GetNz(); i++)//modelData->GetNz()
	//{

	//	sliceNumber = i;
	//	a1 = colDstData2->GetDataArray() + modelData->GetSliceSize()*(sliceNumber - 1);
	//	a2 = modelData->GetDataArray() + modelData->GetSliceSize()*(sliceNumber);


	//	mSliceData->SetDataArray(a1, false); // 
	//	oSliceData->SetDataArray(a2, false); //
	//	dstSlice->SetDataArray(colDstData2->GetDataArray() + modelData->GetSliceSize()*(sliceNumber), false); //

	//	//MatchCenters(t,rCenter);

	//	Iterations = 0;
	//	FValue = 0;

	//	Optim = NULL;
	//	tmpBool = false;
	//	tr[0] = 0;
	//	tr[1] = 0;
	//	tr[2] = 0;
	//	tr[3] = 0;




	//	Optim = new OptimizationPowell(tr, nr, Tolerance, &Iterations, &FValue, this);

	//	double val = this->GetFunctionValue(tr);
	//	if (val > Tolerance)
	//	{
	//		Optim->StartOptimization();
	//	}
	//	else
	//	{
	//		val = val;
	//	}

	//	//trTotal[1]+=tr[1];		trTotal[2]+=tr[2];		trTotal[3]+=tr[3];

	//	if ((tr[1] < 1.0) && (tr[2] < 1.0))
	//	{
	//		tr[1] = 0;tr[2] = 0;
	//	}

	//	if ((colDstData2 != NULL))
	//	{
	//		TransformSlice(oSliceData, dstSlice, 0, tr, true);
	//	}

	//	//qWarning(QString::number(i)+" "+QString::number(tr[1])+" "+QString::number(tr[2])+" "+QString::number(tr[3]));
	//	emit SendMessageToLog(QString::number(i)+" "+QString::number(tr[1])+" "+QString::number(tr[2])+" "+QString::number(tr[3]));
	//	if (Optim != NULL)delete Optim;

	////	emit SendMessage(QString::number(i) + "/" + QString::number(colDstData2->GetNz() - 1));
	//	//QCoreApplication::processEvents();
	//	//tymczasowe!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//	///////////////

	//}
	//
	QCoreApplication::processEvents();

	delete[]simTab;
	delete[]matchTab;


	mSliceData->SetDataArray(NULL, false);
	oSliceData->SetDataArray(NULL, false);
	dstSlice->SetDataArray(NULL, false);

	delete mSliceData;
	delete oSliceData;
	delete dstSlice;


	if ((colDstData2 != NULL))
	{
		colDstData2->SetRawDataSetName("final");
		emit 	SignalSendRawDataSet(colDstData2);
	}
	grayMode = false;
	//     TransformImage(tr);
}
//-------------------------------------------------------
void	ImageSetRegistration::SetMatchSingleFrameWithModelParams(bool *_chcks, QString _prefilterName, int _nrOfSamples, int _functionNr, int nrRange[2], int *_params,
	int _nrMissPixels, View *TransversalV, bool paintView, PaletteRGBA *pal, unsigned char opacity)
{

	for (int i = 0; i < 10; i++)
	{
		chcks[i] = _chcks[i];
	}


	//[0] - preMatch
	//[1],[2],[3] - phase0;
	//[4],[5],[6] - phase1;
	//[7] -parallel
	//[8] - show
	//[9] - outliers

//	chcks[0] = _chcks[0]; chcks[1] = _chcks[1]; chcks[2] = _chcks[2];
	prefilterName = _prefilterName;
	nrOfSamples = _nrOfSamples;
	functionNr = _functionNr;
	objSliceRange[0] = nrRange[0];
	objSliceRange[1] = nrRange[1];
	nrMissPixels = _nrMissPixels;
	paintTransformedSlicesEnabled = chcks[8];//_chckpaintView;
	transversalView = TransversalV;

	this->paletteReg = pal;
	this->regOpacity = opacity;

	if (transversalView != NULL)
	{
		transversalDataSet = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, 1, 1, 1, 0);
	}

	parallelComputing = _chcks[7];
	removeOutliers = _chcks[9];

	for (int i = 0; i < NUMBER_OF_REG_SLIDERS; i++)
	{
		paramsReg[i] = _params[i];
	}

}
//-------------------------------------------------------
void ImageSetRegistration::ApplyStabilization2()
{

	objectData = modelData;

	numberOfThreadsUsed = paramsReg[7];
	double minCounttPercent = nrMissPixels / 100.0;// 0.8;
	//sliceNr = 5;
	//sort=false;
	//dopasuj 1 klatke obiektu do wszystkich - zapamietaj przeksztalcenia i wartosc dopasowania
	//posortuj klatki (jesli trzeba
	//wygeneruj wolumen z wynikiem
	int nrOfModelSlices = modelData->GetNz();
	//if ((sliceNr < 0) return;
	//if ((sliceNr > nrOfModelSlices - 1)) return;

	grayMode = true;
	//intrpVal=269;
	rCenter[0] = modelData->GetNx() / 2;
	rCenter[1] = modelData->GetNy() / 2;
	rCenter[2] = 0;


	double tr[4];
	tr[0] = 0;
	tr[1] = 0;
	tr[2] = 0;
	tr[3] = 0;
	int i, j, k;
	for (i = 0; i < 15; i++)
		t[i] = 0;
	t[10] = 1;
	t[11] = 1;
	t[12] = 1;

	bool tmpBool;
	double Tolerance = 10e-3;

	int Iterations;
	double FValue;
	double trTotal[4] = { 0 };

	OptimizationPowell* Optim = NULL;

	numOfParams = 3;
	int nr = numOfParams;

	double modelScale[3];
	modelData->GetScale(modelScale);
	double objectScale[3];
	objectData->GetScale(objectScale);




	unsigned short* a1, * a2;

	//	if (sliceNr == -1) objectData = modelData;



	mSliceData = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, modelScale[0], modelScale[1], 1, 0, false); // zawiera orginalny przekroj modelu do ktorego dopasowuje wszystkie


	oSliceData = new RawDataSet(objectData->GetNx(), objectData->GetNy(), 1, objectScale[0], objectScale[1], 1, 0, false); //bedzie zawierac dane z kolejnych przekrojow obiektu


	mSliceData->SetRawDataSetName("model");
	oSliceData->SetRawDataSetName("object");

	RawDataSet* dstSlice = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, modelScale[0], modelScale[1], 1, 0, true);

	//wrzucam klatke do ktorej szukam podobnych
	int modelDataSliceSize = modelData->GetSliceSize();

	int ssl = 0;// pierwszy przekrój jest bazowy - do niego matchujê kolejny
	memcpy(dstSlice->GetDataArray(), modelData->GetDataArray() + ssl * modelDataSliceSize, modelDataSliceSize * sizeof(unsigned short));
	//wypelniony slice 0 kopia z oryginalu
	mSliceData->SetDataArray(dstSlice->GetDataArray(), false); // 
	mSliceData->CalculateHistogram();



	//---------------------robie przefiltrowany model
	//----------------------------------------------

	filteredModel = new RawDataSet();
	filteredModel->Copy(*mSliceData, false);

	QString filterPreprocessingName = prefilterName;// "filter_compositions/regFilterPreprocessing.layers";
	FilterDataSet(mSliceData, filteredModel, filterPreprocessingName);
	filteredModel->FindMinMaxValue();
	filteredModel->SaveRawDataSet2("e:\\data\\x\\model");
	//delete filteredModel;


	filteredObject = new RawDataSet();
	filteredObject->Copy(*oSliceData, false);


	//------------------------------------------------------
	///------------------ koniec robienia przefiltrowanego modelu!
	resultNrSlices = objSliceRange[1] - objSliceRange[0] + 1;


	//if (sliceNr == -1)
	//{
	//	resultNrSlices = modelData->GetNz() - 1;

	//}


	//FilterDataSet(oSliceData, filteredObject, filterPreprocessingName);

	//i=1;
	int nrOfFrames = resultNrSlices;
	double* params = new double[3 * nrOfFrames];
	memset(params, 0, 3 * nrOfFrames * sizeof(double));
	//double *vals=new double[3
	//IntDoubleStruct** tempS = NULL;
	double totalMaxDist = 0;
	//if (sort == true)
	/*{
		tempS = new IntDoubleStruct * [nrOfFrames];
		for (i = 0; i < nrOfFrames; i++)
		{
			tempS[i] = new IntDoubleStruct;
			tempS[i]->IntVar = i;
		}
	}*/
	////biegne po wszystkich klatkach modelu i je dopasowuje

	////DLL INIT
	//char *parameterFile = "filter_compositions//mutualMIX.layers";

	////FiltDram_Initialization(char * p_plikKonfiguracja, LinkedList<DataSet*>	*_dataSetList)
	//LinkedList<DataSet*>	*dataSetList = new LinkedList<DataSet*>;
	//dataSetList->InsertItem(new DataSet(oSliceData, NULL, 1));
	//dataSetList->InsertItem(new DataSet(mSliceData, NULL, 1));
	//
	//bool init = FiltDram_Initialization2(parameterFile, dataSetList,1);//

	//if (init == false) return;
	//if ((init==true)&&(visWidget != NULL) && (shapes != NULL))
	//{
	//	FiltDram_SetShapesAndVisImage(shapes, visWidget);
	//}

	//nrOfFrames = 1;
	//for (i = 1; i < 2;i++)


	//****************************************
	//****************************************
	//****************************************
	int MX = paramsReg[0];
	int MY = paramsReg[1];

	double maxDeltaX = 0.01;// o tyle procent moze wyjsc rysunek poza przy testach kratownicy
	double maxDeltaY = 0.01;

	scatterSize = paramsReg[6]; //3-8
	functionNumber = functionNr; //0 - count, 1 - ssd, 2 - cross cor, 3 - entropy, 4 -  mi, 5- nmi
	//int posTab[4] = { 0, 1, 2, 6 };



	minCount = 0;
	//****************************************
	//****************************************
	//****************************************
	//-----------------wydobywam bounding box i maske!




	int mnx = filteredModel->GetNx();
	int mny = filteredModel->GetNy();

	//QList<QPolygonF>* slist = &shapes->polygonList;
	//int nrPolygons = shapes->polygonList.count();

	bool polyExist = false;

	imgMaskBits = NULL;
	//QPolygonF polygon;

	//if (nrPolygons > 0)
	//{
	//	polygon = slist->at(0);
	//	if (polygon.size() > 2)polyExist = true;
	//}


	//qreal x1, x2, y1, y2;
	//QImage img(mnx, mny, QImage::Format_RGB32);
	//int countWhite = 0;
	//int countWhiteSampling = 0;
	//int samplingX = paramsReg[4];
	//int samplingY = paramsReg[5];

	//double percentageRejection = paramsReg[8] / 100.0;
	//if (polyExist == true)
	//{

	//	QRectF rect = polygon.boundingRect();
	//	rect.getCoords(&x1, &y1, &x2, &y2);


	//	img.fill(Qt::black);
	//	QPainter Painter2(&img);
	//	Painter2.setBrush(Qt::white);
	//	Painter2.setPen(1);
	//	Painter2.drawPolygon(polygon);

	//	imgMaskBits = (unsigned int*)(img.bits());
	//	//(((maskImgBits[pos]) == 0xffffffff))

	//	int ppos = 0;
	//	for (j = y1; j <= y2; j += samplingY)
	//	{
	//		for (i = x1; i <= x2; i += samplingX)
	//		{
	//			ppos = i + j * mSliceData->GetNx();
	//			if (imgMaskBits[ppos] == 0xffffffff) countWhiteSampling++;
	//		}
	//	}
	//	ppos = 0;
	//	for (j = y1; j <= y2; j += 1)
	//	{
	//		for (i = x1; i <= x2; i += 1)
	//		{
	//			ppos = i + j * mSliceData->GetNx();
	//			if (imgMaskBits[ppos] == 0xffffffff) countWhite++;
	//		}
	//	}
	//}
	//else
	//{
	//	x1 = 0;
	//	y1 = 0;
	//	x2 = mnx - 1;
	//	y2 = mny - 1;
	//}


	////x1 = 69;
	////y1 = 65;
	////x2 = 126;
	////y2 = 125;

	//bounds[0] = (int)x1;
	//bounds[2] = (int)y1;
	//bounds[1] = (int)x2 + 1;
	//bounds[3] = (int)y2 + 1;

	//int BX = x2 - x1;
	//int BY = y2 - y1;

	//int minCountAll = 0;
	///*if (countWhite > 0)
	//{
	//	minCountAll = minCounttPercent * countWhite;

	//}
	//else*/
	//	//minCountAll = minCounttPercent * (BX - 1) * (BY - 1);

	///*if (countWhiteSampling > 0)
	//{
	//	minCount = minCounttPercent * countWhiteSampling;
	//}
	//else*/
	//	minCount = minCounttPercent * (modelData->GetSliceSize() );
	//	minCountAll = minCount;
	////PrepareTrans2(tr, t, 3, posTab);

	//bool test = Update2DStats(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, modelData, objectData, intBuffers, count, usData0, true, t, false);

	//teraz musze obliczyc minx miny i step dla kratownicy


	//int MX = 10;
	//int MY = 10;

	//int maxDeltaX = 0.1;// o tyle procent moze wyjsc rysunek poza przy testach kratownicy
	//int maxDeltaY = 0.1;

bounds[0] = 0;
bounds[2] = 0;
bounds[1] = modelData->GetNx() - 1;
bounds[3] = modelData->GetNy() - 1;

	int  oN[3];
	filteredObject->GetN(oN);
	int maxDeltaRealX = maxDeltaX * oN[0];
	int maxDeltaRealY = maxDeltaY * oN[1];


	//wyliczam startowa pozycje boundingboxa po translacji

	//double po0[2], po1[2];

	double scalef[2] = { modelScale[0] / objectScale[0], modelScale[1] / objectScale[1] };
	//po0[0] = x1 * scalef[0];//tu wyladuje poczatek bounding boxa w przestzreni obiektu 
	//po0[1] = y1 * scalef[1];

	/*int tMinX = -maxDeltaRealX - po0[0];
	int tMaxX = oN[0] + maxDeltaRealX - po0[0] - BX * scalef[0];
	int tMinY = -maxDeltaRealY - po0[1];
	int tMaxY = oN[1] + maxDeltaRealY - po0[1] - BY * scalef[1];

	int dBoundX = tMaxX - tMinX;
	int dBoundY = tMaxY - tMinY;*/




	bool doPreMatch = true;
	//if ((dBoundX < 0) || (dBoundY < 0))
	//{
	//	doPreMatch = false;
	//}

	int stepx = 1;
	int stepy = 1;
	/*if (doPreMatch == true)
	{

		if (dBoundX < MX)
		{
			MX = dBoundX;
		}

		if (dBoundY < MY)
		{
			MY = dBoundY;
		}

		stepx = dBoundX / MX;
		stepy = dBoundY / MY;
	}*/


	//powinienem teraz miec dobre dobrane bounds i step

	int* intBuffers[2] = { NULL, NULL };

	i = 0;
	//RawDataSet *tmpRawData = new RawDataSet();
	//tmpRawData->Copy(*mSliceData, false);

	//for (i = objSliceRange[0]; i<=objSliceRange[1]; i++)//modelData->GetNz()
	optimizationStopped = false;

	bool saved = false;
	double minFunctionValue = INT_MAX;

	int tmpSize = resultNrSlices * 10;
	double* fvaluesX = new double[tmpSize];
	memset(fvaluesX, 0, tmpSize * sizeof(double));






	/*QFile file("e:\\data\\x\\param1s.txt");
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		int poss = 0;
		for (i = 0; i < 6; i++)
		{

				stream << chcks[i] << "\t";

			stream << "\n";
		}


		file.close();
	}*/


	//[0] - preMatch
	//[1],[2],[3] - phase0;
	//[4],[5],[6] - phase1;
	//[7] -parallel
	//[8] - show
	//[9] - outliers

	QFile file22("e:\\data\\x\\params2.txt");
	bool f2Open = (file22.open(QIODevice::WriteOnly));
	QTextStream stream22(&file22);


	QFile file33("e:\\data\\x\\totalTransParams.txt");
	bool f3Open = (file33.open(QIODevice::WriteOnly));
	QTextStream stream33(&file33);

	double* tr2d = new double[resultNrSlices * 2];
	memset(tr2d, 0, resultNrSlices * 2 * sizeof(double));
	double maxDist = 5.0;


	resultNrSlices = modelData->GetNz() - 1;

	int minCountAll = modelData->GetSliceSize() * 0.8;//musi sie pokrywac w 0.7
	minCount = minCountAll;

	for (i = 1; i <= resultNrSlices; i++)//resultNrSlices
	{
		if (i > 1)
		{
			//swap
			mSliceData->Copy(*oSliceData, true);
			filteredModel->Copy(*filteredObject, true);

		}
		if (optimizationStopped == true) break;
		sliceNumber = i;
		//a1=colDstData2->GetDataArray()+modelData->GetSliceSize()*(sliceNumber-1);
		int vala = objectData->GetSliceSize() * (sliceNumber + objSliceRange[0]);
		qWarning() << "val: " << vala;

		a2 = objectData->GetDataArray() + objectData->GetSliceSize() * (sliceNumber + objSliceRange[0]);
		qWarning() << "po val";

		posTab[0] = 0;
		posTab[1] = 1;
		posTab[2] = 2;
		posTab[3] = 6;
		numOfParams = 3;
		qWarning() << "przed filtracja";
		oSliceData->SetDataArray(a2, false); // bufor obiektu uzupelniony
		oSliceData->CalculateHistogram();
		FilterDataSet(oSliceData, filteredObject, filterPreprocessingName);
		filteredObject->FindMinMaxValue();
		qWarning() << "po filtracji";
		if (saved == false)
		{
			saved = true;
			filteredObject->SaveRawDataSet2("e:\\data\\x\\test_" + QString::number(i));

		}


		//------------------------------------------------------------
		// mam na wejsciu przefiltrowane obj i model
		// biore model i jego obrys z polygona
		// biore bounds modelu i przebiegam po danych obiektu zmieniajac po kracie o rozmiarze X i Y pozycje modelu
		// sprawdzam count i fValue - biore to tx i ty dla ktoreych jest min fvalue i count <0.9 wszystkich modelu
		// potem odpalam stara metode



		//------------------------------------------------------------

		//if (sort == false) dstSlice->SetDataArray(colDstData2->GetDataArray() + colDstData2->GetSliceSize()*(sliceNumber), false); // miejsce w ktore wrzuce wynik

		//MatchCenters(t,rCenter);

		Iterations = 0;
		FValue = 0;

		Optim = NULL;
		tmpBool = false;
		tr[0] = 0;
		tr[1] = 0;
		tr[2] = 0;
		tr[3] = 0;

		double testFValue = INT_MAX;
		double minTrans[4] = { 0, 0, 0, 0 };

		int ccount = 0;
		int fCount = 0;
		//filteredObject->SaveRawDataSet2("f:\\data\\x\\test1");

		counter = 0;
		totalTime = 0;
		totalTime1 = 0;
		totalTime2 = 0;
		totalTime3 = 0;
		totalTime4 = 0;

		//	srand((unsigned)time(NULL));
			//	srand((unsigned)0);
			//((rand()) / (double)RAND_MAX)

		minTrans[1] = 0; minTrans[1] = 0; minTrans[2] = 0;

		double preAngle = paramsReg[2] * to_rad;
		int nrOfAngleProbes = paramsReg[3];

		double ra = 0;

		double mmin[2] = { INT_MAX };
		double mmax[2] = { -INT_MAX };
		double val;

		//int s

		int tMinX = 0;
		int tMaxX = modelData->GetNx() - 1;
		int tMinY = 0;
		int tMaxY= modelData->GetNy() - 1;
		int samplingX = paramsReg[4];
		int samplingY = paramsReg[5];

		if ((doPreMatch == true) && (chcks[0] == true))
		{



			for (int xx = 0; xx < nrOfAngleProbes; xx++)

				//for (int xx = 0; xx < nrOfSamples; xx++)
				for (int y = tMinY; y < tMaxY; y += stepy)
				{
					if (optimizationStopped == true) break;
					for (int x = tMinX; x < tMaxX; x += stepx)
					{




						double ra = 0;
						if ((nrOfAngleProbes > 1) && (preAngle > 0))
							ra = -preAngle + 2 * preAngle * ((rand()) / (double)RAND_MAX);

						tr[1] = x * objectScale[0];
						tr[2] = y * objectScale[1];
						tr[3] = ra;



						PrepareTrans2(tr, t, nr, posTab);
						//	tmpRawData->ZeroQ();
						bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter, samplingX, samplingY);
						val = statsMutual[functionNumber];
						counter++;
						emit SendMessage("(" + QString::number(sliceNumber) + "/)" + QString::number(resultNrSlices) + " NrCount: " + QString::number(counter) + "\t" + "Value: " + QString::number(val));

						if (paintTransformedSlicesEnabled == true) PaintTransformedSlices(t);

						qApp->processEvents();
						//tmpRawData->SaveRawDataSet2("f:\\data\\x\\test1-" + QString::number(x) + "-" + QString::number(y));
						//functionNumber ; //0 - count, 1 - ssd, 2 - cross cor, 3 - entropy, 4 -  mi, 5- nmi
						FValue = statsMutual[functionNumber];
						ccount = statsMutual[0];

						if ((FValue < testFValue) && (ccount > minCount))
						{
							UnPrepareTrans2(tr, t, nr, posTab);
							testFValue = FValue;
							fCount = ccount;
							for (int xx = 1; xx <= 3; xx++)			minTrans[xx] = tr[xx];

						}
					}
				}







		}


		for (int xx = 1; xx < nr + 2; xx++)			tr[xx] = minTrans[xx];

		qWarning() << "Phase 0" << "Value:" << testFValue << "tx:" << tr[1] << "ty:" << tr[2] << "rot:" << tr[3];
		//automat!
		maxFound = 0;
		//PrepareTrans2(tr, t, 3, posTab);

		//1,2,3
		int valTest = chcks[1] + chcks[2] + chcks[3];
		//	if (chcks[1] == true)
		if (valTest > 0)
		{

			int nrrx = 1;
			if (chcks[1] == true) { tr[nrrx] = minTrans[1]; posTab[nrrx++] = 1; }
			if (chcks[2] == true) { tr[nrrx] = minTrans[2]; posTab[nrrx++] = 2; }
			if (chcks[3] == true) { tr[nrrx] = minTrans[6]; posTab[nrrx++] = 6; }

			nrrx--;
			numOfParams = nrrx;

			Optim = new OptimizationPowell(tr, nrrx, Tolerance, &Iterations, &FValue, this);
			connect(this, SIGNAL(SignalStopOptimization()), Optim, SLOT(SlotStopOptimization()));
			Optim->StartOptimization();
			if (Optim->OptimizationStopped == true)this->optimizationStopped = true;
			delete Optim; Optim = NULL;
			//filteredModel->SaveRawDataSet2("e:\\data\\x\\model_" + QString::number(i));
			//filteredObject->SaveRawDataSet2("e:\\data\\x\\object_" + QString::number(i));
			PrepareTrans2(tr, t, nr, posTab);
			bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter);

			qWarning() << "Phase 1" << "Value:" << FValue << "Count:" << ccount << "tx:" << tr[1] << "ty:" << tr[2] << "rot:" << tr[3];

			if (ccount >= minCountAll)
			{
				PrepareTrans2(tr, t, nrrx, posTab);
				//for (int xx = 1; xx < nr + 2; xx++)			 minTrans[xx] = tr[xx];
				fCount = ccount;

				minTrans[1] = t[1];
				minTrans[2] = t[2];
				minTrans[3] = t[6];
			}
		}
		for (int xx = 1; xx <= 3; xx++)			tr[xx] = minTrans[xx];
		//4/5/6



		valTest = chcks[4] + chcks[5] + chcks[6];
		//	if (chcks[1] == true)
		if (valTest > 0)
		{

			int nrrx = 1;
			if (chcks[4] == true) { tr[nrrx] = minTrans[1]; posTab[nrrx++] = 1; }
			if (chcks[5] == true) { tr[nrrx] = minTrans[2]; posTab[nrrx++] = 2; }
			if (chcks[6] == true) { tr[nrrx] = minTrans[6]; posTab[nrrx++] = 6; }
			nrrx--;
			numOfParams = nrrx;
			//if (chcks[2] == true)
			//{
			Optim = new OptimizationPowell(tr, nrrx, Tolerance, &Iterations, &FValue, this);
			connect(this, SIGNAL(SignalStopOptimization()), Optim, SLOT(SlotStopOptimization()));
			Optim->StartOptimization();
			if (Optim->OptimizationStopped == true)this->optimizationStopped = true;
			delete Optim; Optim = NULL;
			PrepareTrans2(tr, t, nr, posTab);
			bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter);

			qWarning() << "Phase 2" << "Value:" << FValue << "Count:" << ccount << "tx:" << tr[1] << "ty:" << tr[2] << "rot:" << tr[3];

			if (ccount >= minCountAll)
			{
				PrepareTrans2(tr, t, nrrx, posTab);
				//for (int xx = 1; xx < nr + 2; xx++)			 minTrans[xx] = tr[xx];
				fCount = ccount;

				minTrans[1] = t[1];
				minTrans[2] = t[2];
				minTrans[3] = t[6];
			}
		}



		stream22 << i << "\t" << minTrans[1] << "\n";
		//dla bezpieczenstwa - przesuniecie jesli zbyt duze - przerywam
		tr2d[i * 2] = minTrans[1];
		tr2d[i * 2 + 1] = minTrans[2];

		if (i > 0)
		{
			double distx = GetDistance2D(tr2d + (i - 1) * 2, tr2d + i * 2);
			if (distx > maxDist)
			{
				minTrans[1] = tr2d[(i - 1) * 2];
				minTrans[2] = tr2d[(i - 1) * 2 + 1];
				tr2d[i * 2] = tr2d[(i - 1) * 2];
				tr2d[i * 2 + 1] = tr2d[(i - 1) * 2 + 1];
			}
			if (distx > totalMaxDist) totalMaxDist = distx;
		}



		for (int xx = 1; xx <= 3; xx++)			tr[xx] = minTrans[xx];
		PrepareTrans2(tr, t, nr, posTab);
		bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter);
		if (optimizationStopped == true) break;



		params[i * 3] = tr[1];
		params[i * 3 + 1] = tr[2];
		params[i * 3 + 2] = tr[3];

		double fValX = (statsMutual[0] > minCountAll) ? statsMutual[functionNumber] : INT_MAX;

		//tempS[i]->DoubleVar = fValX;



		int ppos = i * 10;
		fvaluesX[ppos++] = sliceNumber + objSliceRange[0];
		fvaluesX[ppos++] = tr[1];
		fvaluesX[ppos++] = tr[2];
		fvaluesX[ppos++] = tr[3];
		fvaluesX[ppos++] = statsMutual[0];
		fvaluesX[ppos++] = statsMutual[1];
		fvaluesX[ppos++] = statsMutual[2];
		fvaluesX[ppos++] = statsMutual[3];
		fvaluesX[ppos++] = statsMutual[4];
		fvaluesX[ppos++] = statsMutual[5];

		//if (sliceNr == -1) //dopasowanie stabilizacyjne - update mSliceData
		{
			posTab[0] = 0;
			posTab[1] = 1;
			posTab[2] = 2;
			posTab[3] = 6;
			numOfParams = 3;

			//memset(t, 0, 15 * sizeof(double));

		///	PrepareTrans2(tr, t, numOfParams, posTab);
			stream33 << i << "\t" << numOfParams << "\t";
			for (int kk = 0; kk < numOfParams; kk++)
			{
				stream33 << posTab[i] << "\t" << tr[i] << "\t";

			}

			stream33 << "\n";

			TransformSlice2(oSliceData, dstSlice, 0, tr, false);
			stream33 << "\n";
			memcpy(mSliceData->GetDataArray(), dstSlice->GetDataArray(), modelDataSliceSize * sizeof(unsigned short));
			FilterDataSet(mSliceData, filteredModel, filterPreprocessingName);
			filteredModel->FindMinMaxValue();

			//	

		}

		if (fValX < minFunctionValue) minFunctionValue = fValX;

		//qWarning(QString::number(i)+" "+QString::number(tr[1])+" "+QString::number(tr[2])+" "+QString::number(tr[3]));
		//	qWarning(QString::number(i)+" "+QString::number(trTotal[1])+" "+QString::number(trTotal[2])+" "+QString::number(trTotal[3]));
		qWarning() << "przed deloptim";
		if (Optim != NULL)delete Optim;

		emit SendMessage(QString::number(i) + "/" + QString::number(resultNrSlices - 1));
		QCoreApplication::processEvents();
		//tymczasowe!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		///////////////
		qWarning() << "koniec petli";
	}


	//QString fn = "f:\\data\\x\\test\params.txt";
	{QFile file("f:\\data\\x\\params.txt");
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		int poss = 0;
		for (i = 0; i < resultNrSlices; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				stream << fvaluesX[poss++] << "\t";
			}
			stream << "\n";
		}


		file.close();
	}}

	//zbieranie i zrzut do pliku
	delete[] fvaluesX;




	//delete tmpRawData; 
	//delete []intBuffers;
	delete filteredModel;
	filteredModel = NULL;
	delete filteredObject;
	filteredObject = NULL;
	////DLL FREE
	//dataSetList->GetItemAtPos(0)->SetData(NULL);
	//dataSetList->GetItemAtPos(1)->SetData(NULL);
	//delete dataSetList;
	//FiltDram_Free();

	int newNrOfFrames = resultNrSlices;
	RawDataSet* colDstData2 = NULL;



	RawDataSet* ghostFinalData = NULL;
	if ((optimizationStopped == false))
	{
		//if ((sort == true) && (sliceNr != -1))
		//{

		//	QSortIntDouble(tempS, 0, nrOfFrames - 1);//posortowane
		//}

		//if ((removeOutliers == true) && (sliceNr != -1))
		//{
		//	newNrOfFrames = 0;
		//	for (i = 0; i < nrOfFrames; i++)//modelData->GetNz()
		//	{

		//		double fVal = tempS[i]->DoubleVar;


		//		bool test = false;
		//		if (minFunctionValue > 0)
		//		{
		//			test = (fVal < minFunctionValue* (1.0 + percentageRejection));
		//		}
		//		else
		//		{
		//			test = (fVal < minFunctionValue* (1.0 - percentageRejection));
		//		}

		//		if (test == true) newNrOfFrames++;
		//	}

		//	//if (newNrOfFrames < nrOfFrames)
		//	//{
		//	//	RawDataSet *nRawData = new RawDataSet(modelData->GetNx(), modelData->GetNy(), newNrOfFrames, modelScale[0], modelScale[1], 1, 0); //nx,ny jak model ilosc przekrojow jak obiekt
		//	//	nRawData->ZeroQ();
		//	//	delete colDstData2;
		//	//	colDstData2 = nRawData;
		//	//}
		//}


		//int nrrrr = (sliceNr == -1) ? newNrOfFrames + 1 : newNrOfFrames; //musi byc o 1 wieksza dla stabilizacji

		colDstData2 = new RawDataSet(modelData->GetNx(), modelData->GetNy(), modelData->GetNz(), modelScale[0], modelScale[1], 1, 0); //nx,ny jak model ilosc przekrojow jak obiekt
		colDstData2->ZeroQ();

		if (ghostData != NULL)
		{
			ghostFinalData = new RawDataSet();
			ghostFinalData->Copy(*colDstData2, false);
			ghostFinalData->ZeroQ();
		}

		int pposX = 0;
		//if (sliceNr == -1)
		{
			pposX++;
			memcpy(colDstData2->GetDataArray(), modelData->GetDataArray(), modelDataSliceSize * sizeof(unsigned short));  //skopiuje dane modelu na slice 0
		}

		posTab[0] = 0;
		posTab[1] = 1;
		posTab[2] = 2;
		posTab[3] = 6;
		numOfParams = 3;
		for (i = 0; i < nrOfFrames; i++)//modelData->GetNz()
		{

		
			//a1=colDstData2->GetDataArray()+modelData->GetSliceSize()*(sliceNumber-1);

			Iterations = 0;
			FValue = 0;

			Optim = NULL;
			tmpBool = false;

			tr[0] = 0;
			tr[1] = params[sliceNumber * 3];
			tr[2] = params[sliceNumber * 3 + 1];
			tr[3] = params[sliceNumber * 3 + 2];

			{

				a2 = objectData->GetDataArray() + objectData->GetSliceSize() * (sliceNumber + objSliceRange[0]);

				oSliceData->SetDataArray(a2, false); // bufor obiektu uzupelniony
				dstSlice->SetDataArray(colDstData2->GetDataArray() + colDstData2->GetSliceSize() * (pposX), false); // miejsce w ktore wrzuce wynik

				TransformSlice2(oSliceData, dstSlice, 0, tr, false);


				if (ghostFinalData != NULL)
				{
					a2 = ghostData->GetDataArray() + ghostData->GetSliceSize() * (sliceNumber + objSliceRange[0]);

					oSliceData->SetDataArray(a2, false); // bufor obiektu uzupelniony
					dstSlice->SetDataArray(ghostFinalData->GetDataArray() + ghostFinalData->GetSliceSize() * (pposX), false); // miejsce w ktore wrzuce wynik

					TransformSlice2(oSliceData, dstSlice, 0, tr, false);
				}

				if (Optim != NULL)delete Optim;

				emit SendMessage("Transforming :" + QString::number(i) + "/" + QString::number(colDstData2->GetNz() - 1));
			//	qWarning() << "FValue" + QString::number(fVal) + "\t Transforming slice: " + QString::number(sliceNumber) + "\tto position: " + QString::number(pposX);
				//QCoreApplication::processEvents();
				//tymczasowe!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				///////////////


				pposX++;
			}

		}


	}

	file22.close();
	file33.close();
	//QCoreApplication::processEvents();

	if ((optimizationStopped == false) )
	{

		RawDataSet* tmpRawDatax = new RawDataSet();
		tmpRawDatax->Copy(*colDstData2, false);
		unsigned short* dd = tmpRawDatax->GetDataArray();
		unsigned short* md = mSliceData->GetDataArray();
		for (i = 0; i < newNrOfFrames; i++)
		{
			memcpy(dd + mSliceData->GetSliceSize() * i, md, mSliceData->GetSliceSize() * sizeof(unsigned short));
		}
		tmpRawDatax->SetRawDataSetName("modelTest");
		tmpRawDatax->CalculateHistogram();
		emit 	SignalSendRawDataSet(tmpRawDatax);
	}



	mSliceData->SetDataArray(NULL, false);
	oSliceData->SetDataArray(NULL, false);
	dstSlice->SetDataArray(NULL, false);

	delete mSliceData;
	delete oSliceData;
	delete dstSlice;

	
	delete[]params;
	if ((colDstData2 != NULL))
	{
		if ((optimizationStopped == false))
		{

			colDstData2->SetRawDataSetName("finalTest");
			colDstData2->CalculateHistogram();
			emit 	SignalSendRawDataSet(colDstData2);

			if (ghostFinalData != NULL)
			{
				ghostFinalData->SetRawDataSetName("ghostData");
				ghostFinalData->CalculateHistogram();
				emit 	SignalSendRawDataSet(ghostFinalData);
			}
		}
		else
			delete colDstData2;

	}
	grayMode = false;





}


//------------------------------
void	ImageSetRegistration::MatchSingleFrameWithModel(int sliceNr, bool sort)
{


	
	numberOfThreadsUsed = paramsReg[7];
	double minCounttPercent = nrMissPixels / 100.0;// 0.8;
	//sliceNr = 5;
	//sort=false;
	//dopasuj 1 klatke obiektu do wszystkich - zapamietaj przeksztalcenia i wartosc dopasowania
	//posortuj klatki (jesli trzeba
	//wygeneruj wolumen z wynikiem
	int nrOfModelSlices = modelData->GetNz();
	//if ((sliceNr < 0) return;
	if ((sliceNr>nrOfModelSlices - 1)) return;

	grayMode = true;
	//intrpVal=269;
	rCenter[0] = modelData->GetNx() / 2;
	rCenter[1] = modelData->GetNy() / 2;
	rCenter[2] = 0;


	double tr[4];
	tr[0] = 0;
	tr[1] = 0;
	tr[2] = 0;
	tr[3] = 0;
	int i, j, k;
	for (i = 0; i < 15; i++)
		t[i] = 0;
	t[10] = 1;
	t[11] = 1;
	t[12] = 1;

	bool tmpBool;
	double Tolerance = 10e-3;

	int Iterations;
	double FValue;
	double trTotal[4] = { 0 };

	OptimizationPowell *Optim = NULL;

	numOfParams = 3;
	int nr = numOfParams;

	double modelScale[3];
	modelData->GetScale(modelScale);
	double objectScale[3];
	objectData->GetScale(objectScale);



	
	unsigned short *a1, *a2;

//	if (sliceNr == -1) objectData = modelData;

	mSliceData = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, modelScale[0], modelScale[1], 1, 0, false); // zawiera orginalny przekroj modelu do ktorego dopasowuje wszystkie
	
	
	oSliceData = new RawDataSet(objectData->GetNx(), objectData->GetNy(), 1, objectScale[0], objectScale[1], 1, 0, false); //bedzie zawierac dane z kolejnych przekrojow obiektu
	

	mSliceData->SetRawDataSetName("model");
	oSliceData->SetRawDataSetName("object");

	RawDataSet *dstSlice = new RawDataSet(modelData->GetNx(), modelData->GetNy(), 1, modelScale[0], modelScale[1], 1, 0, true);

	//wrzucam klatke do ktorej szukam podobnych
	int modelDataSliceSize = modelData->GetSliceSize();

	int ssl = (sliceNr < 0)? 0:sliceNr;
	memcpy(dstSlice->GetDataArray(), modelData->GetDataArray() + ssl*modelDataSliceSize, modelDataSliceSize*sizeof(unsigned short));
	//wypelniony slice 0 kopia z oryginalu
	mSliceData->SetDataArray(dstSlice->GetDataArray(), false); // 
	mSliceData->CalculateHistogram();



	//---------------------robie przefiltrowany model
	//----------------------------------------------

	filteredModel = new RawDataSet();
	filteredModel->Copy(*mSliceData, false);

	QString filterPreprocessingName = prefilterName;// "filter_compositions/regFilterPreprocessing.layers";
	FilterDataSet(mSliceData, filteredModel, filterPreprocessingName);
	filteredModel->FindMinMaxValue();
	filteredModel->SaveRawDataSet2("e:\\data\\x\\model");
	//delete filteredModel;


	filteredObject = new RawDataSet();
	filteredObject->Copy(*oSliceData, false);


	//------------------------------------------------------
	///------------------ koniec robienia przefiltrowanego modelu!
	resultNrSlices = objSliceRange[1] - objSliceRange[0] + 1;


	if (sliceNr == -1)
	{
		resultNrSlices = modelData->GetNz() - 1;

	}


	//FilterDataSet(oSliceData, filteredObject, filterPreprocessingName);

	//i=1;
	int nrOfFrames = resultNrSlices;
	double *params = new double[3 * nrOfFrames];
	memset(params, 0, 3 * nrOfFrames*sizeof(double));
	//double *vals=new double[3
	IntDoubleStruct **tempS = NULL;
	double totalMaxDist = 0;
	//if (sort == true)
	{
		tempS = new IntDoubleStruct*[nrOfFrames];
		for (i = 0; i < nrOfFrames; i++)
		{
			tempS[i] = new IntDoubleStruct;
			tempS[i]->IntVar = i;
		}
	}
	////biegne po wszystkich klatkach modelu i je dopasowuje

	////DLL INIT
	//char *parameterFile = "filter_compositions//mutualMIX.layers";

	////FiltDram_Initialization(char * p_plikKonfiguracja, LinkedList<DataSet*>	*_dataSetList)
	//LinkedList<DataSet*>	*dataSetList = new LinkedList<DataSet*>;
	//dataSetList->InsertItem(new DataSet(oSliceData, NULL, 1));
	//dataSetList->InsertItem(new DataSet(mSliceData, NULL, 1));
	//
	//bool init = FiltDram_Initialization2(parameterFile, dataSetList,1);//

	//if (init == false) return;
	//if ((init==true)&&(visWidget != NULL) && (shapes != NULL))
	//{
	//	FiltDram_SetShapesAndVisImage(shapes, visWidget);
	//}

	//nrOfFrames = 1;
	//for (i = 1; i < 2;i++)


	//****************************************
	//****************************************
	//****************************************
	int MX = paramsReg[0];
	int MY = paramsReg[1];

	double maxDeltaX = 0.01;// o tyle procent moze wyjsc rysunek poza przy testach kratownicy
	double maxDeltaY = 0.01;

	scatterSize = paramsReg[6]; //3-8
	functionNumber = functionNr; //0 - count, 1 - ssd, 2 - cross cor, 3 - entropy, 4 -  mi, 5- nmi
	//int posTab[4] = { 0, 1, 2, 6 };

	

	minCount = 0;
	//****************************************
	//****************************************
	//****************************************
	//-----------------wydobywam bounding box i maske!




	int mnx = filteredModel->GetNx();
	int mny = filteredModel->GetNy();

	QList<QPolygonF> *slist = &shapes->polygonList;
	int nrPolygons = shapes->polygonList.count();

	bool polyExist = false;

	imgMaskBits = NULL;
	QPolygonF polygon;

	if (nrPolygons > 0)
	{
		polygon = slist->at(0);
		if (polygon.size() > 2)polyExist = true;
	}


	qreal x1, x2, y1, y2;
	QImage img(mnx, mny, QImage::Format_RGB32);
	int countWhite = 0;
	int countWhiteSampling = 0;
	int samplingX = paramsReg[4];
	int samplingY = paramsReg[5];

	double percentageRejection = paramsReg[8]/100.0;
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
		//(((maskImgBits[pos]) == 0xffffffff))

		int ppos=0;
		for (j = y1; j <= y2; j += samplingY)
		{
			for (i = x1; i <= x2; i += samplingX)
			{
				ppos = i + j*mSliceData->GetNx();
				if (imgMaskBits[ppos] == 0xffffffff) countWhiteSampling++;
			}
		}
		ppos = 0;
		for (j = y1; j <= y2; j += 1)
		{
			for (i = x1; i <= x2; i += 1)
			{
				ppos = i + j*mSliceData->GetNx();
				if (imgMaskBits[ppos] == 0xffffffff) countWhite++;
			}
		}
	}
	else
	{
		x1 = 0;
		y1 = 0;
		x2 = mnx - 1;
		y2 = mny - 1;
	}


	//x1 = 69;
	//y1 = 65;
	//x2 = 126;
	//y2 = 125;

	bounds[0] = (int)x1;
	bounds[2] = (int)y1;
	bounds[1] = (int)x2 + 1;
	bounds[3] = (int)y2 + 1;

	int BX = x2 - x1;
	int BY = y2 - y1;

	int minCountAll = 0;
	if (countWhite > 0)
	{
		minCountAll = minCounttPercent*countWhite;

	}
	else
		minCountAll = minCounttPercent*(BX - 1)*(BY - 1);
	
	if (countWhiteSampling > 0)
	{
		minCount = minCounttPercent*countWhiteSampling;
	}
	else
	minCount = minCounttPercent*(BX - 1)*(BY - 1);

	//PrepareTrans2(tr, t, 3, posTab);

	//bool test = Update2DStats(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, modelData, objectData, intBuffers, count, usData0, true, t, false);

	//teraz musze obliczyc minx miny i step dla kratownicy


	//int MX = 10;
	//int MY = 10;

	//int maxDeltaX = 0.1;// o tyle procent moze wyjsc rysunek poza przy testach kratownicy
	//int maxDeltaY = 0.1;



	int  oN[3];
	filteredObject->GetN(oN);
	int maxDeltaRealX = maxDeltaX*oN[0];
	int maxDeltaRealY = maxDeltaY*oN[1];


	//wyliczam startowa pozycje boundingboxa po translacji

	double po0[2], po1[2];

	double scalef[2] = { modelScale[0] / objectScale[0], modelScale[1] / objectScale[1] };
	po0[0] = x1*scalef[0];//tu wyladuje poczatek bounding boxa w przestzreni obiektu 
	po0[1] = y1*scalef[1];

	int tMinX = -maxDeltaRealX - po0[0];
	int tMaxX = oN[0] + maxDeltaRealX - po0[0] - BX*scalef[0];
	int tMinY = -maxDeltaRealY - po0[1];
	int tMaxY = oN[1] + maxDeltaRealY - po0[1] - BY*scalef[1];

	int dBoundX = tMaxX - tMinX;
	int dBoundY = tMaxY - tMinY;




	bool doPreMatch = true;
	if ((dBoundX < 0) || (dBoundY < 0))
	{
		doPreMatch = false;
	}

	int stepx = 1;
	int stepy = 1;
	if (doPreMatch == true)
	{

		if (dBoundX < MX)
		{
			MX = dBoundX;
		}

		if (dBoundY < MY)
		{
			MY = dBoundY;
		}

		stepx = dBoundX / MX;
		stepy = dBoundY / MY;
	}


	//powinienem teraz miec dobre dobrane bounds i step

	int *intBuffers[2] = { NULL, NULL };

	i = 0;
	//RawDataSet *tmpRawData = new RawDataSet();
	//tmpRawData->Copy(*mSliceData, false);

	//for (i = objSliceRange[0]; i<=objSliceRange[1]; i++)//modelData->GetNz()
	optimizationStopped = false;

	bool saved = false;
	double minFunctionValue = INT_MAX;

	int tmpSize = resultNrSlices * 10;
	double *fvaluesX = new double[tmpSize];
	memset(fvaluesX, 0, tmpSize*sizeof(double));



	


	/*QFile file("e:\\data\\x\\param1s.txt");
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		int poss = 0;
		for (i = 0; i < 6; i++)
		{
			
				stream << chcks[i] << "\t";
			
			stream << "\n";
		}


		file.close();
	}*/


	//[0] - preMatch
	//[1],[2],[3] - phase0;
	//[4],[5],[6] - phase1;
	//[7] -parallel
	//[8] - show
	//[9] - outliers

	QFile file22("e:\\data\\x\\params2.txt");
	bool f2Open = (file22.open(QIODevice::WriteOnly));
	QTextStream stream22(&file22);


	QFile file33("e:\\data\\x\\totalTransParams.txt");
	bool f3Open = (file33.open(QIODevice::WriteOnly));
	QTextStream stream33(&file33);

	double *tr2d = new double[resultNrSlices * 2];
	memset(tr2d, 0, resultNrSlices * 2 * sizeof(double));
	double maxDist = 5.0;
	for (i = 0; i < resultNrSlices; i++)//resultNrSlices
	{
		if (optimizationStopped == true) break;
		sliceNumber = i;
		//a1=colDstData2->GetDataArray()+modelData->GetSliceSize()*(sliceNumber-1);
		int vala = objectData->GetSliceSize()*(sliceNumber + objSliceRange[0]);
		qWarning() << "val: " <<vala;

		a2 = objectData->GetDataArray() + objectData->GetSliceSize()*(sliceNumber + objSliceRange[0]);
		qWarning() << "po val";

		posTab[0] = 0;
		posTab[1] = 1;
		posTab[2] = 2;
		posTab[3] = 6;
		numOfParams = 3;
		qWarning() << "przed filtracja";
		oSliceData->SetDataArray(a2, false); // bufor obiektu uzupelniony
		oSliceData->CalculateHistogram();
		FilterDataSet(oSliceData, filteredObject, filterPreprocessingName);
		filteredObject->FindMinMaxValue();
		qWarning() << "po filtracji";
		if (saved == false)
		{
			saved = true;
			filteredObject->SaveRawDataSet2("e:\\data\\x\\test_" + QString::number(i));

		}


		//------------------------------------------------------------
		// mam na wejsciu przefiltrowane obj i model
		// biore model i jego obrys z polygona
		// biore bounds modelu i przebiegam po danych obiektu zmieniajac po kracie o rozmiarze X i Y pozycje modelu
		// sprawdzam count i fValue - biore to tx i ty dla ktoreych jest min fvalue i count <0.9 wszystkich modelu
		// potem odpalam stara metode



		//------------------------------------------------------------

		//if (sort == false) dstSlice->SetDataArray(colDstData2->GetDataArray() + colDstData2->GetSliceSize()*(sliceNumber), false); // miejsce w ktore wrzuce wynik

		//MatchCenters(t,rCenter);

		Iterations = 0;
		FValue = 0;

		Optim = NULL;
		tmpBool = false;
		tr[0] = 0;
		tr[1] = 0;
		tr[2] = 0;
		tr[3] = 0;

		double testFValue = INT_MAX;
		double minTrans[4] = { 0, 0, 0, 0 };

		int ccount = 0;
		int fCount = 0;
		//filteredObject->SaveRawDataSet2("f:\\data\\x\\test1");

		counter = 0;
		totalTime = 0;
		totalTime1 = 0;
		totalTime2 = 0;
		totalTime3 = 0;
		totalTime4 = 0;

	//	srand((unsigned)time(NULL));
		//	srand((unsigned)0);
		//((rand()) / (double)RAND_MAX)
		int tmpBounds[2] = { dBoundX, dBoundY };
		int tmpMin[2] = { tMinX, tMinY };

		minTrans[1] = 0; minTrans[1] = 0; minTrans[2] = 0;

		double preAngle = paramsReg[2] * to_rad;
		int nrOfAngleProbes = paramsReg[3];

		double ra = 0;

		double mmin[2] = { INT_MAX };
		double mmax[2] = { -INT_MAX };
		double val;

		//int s

		

		if ((doPreMatch == true) && (chcks[0] == true))
		{
			
			

			for (int xx = 0; xx < nrOfAngleProbes; xx++)

			//for (int xx = 0; xx < nrOfSamples; xx++)
				for (int y = tMinY; y < tMaxY; y += stepy)
			{
				if (optimizationStopped == true) break;
					for (int x = tMinX; x < tMaxX; x += stepx)
				{


					//if ((xx == nrOfSamples / 2) || (xx==(nrOfSamples*3)/4))
					//{
					//	//biore aktualnie najlepszy zmniejszam o polowe bounds i ukladam dookola aktyalnego
					//	int xa = minTrans[1];
					//	int ya = minTrans[2];
					//	

					//	tmpBounds[0] /= 2;
					//	tmpBounds[1] /= 2;
					//	tmpMin[0] = xa - tmpBounds[0];
					//	tmpMin[1] = ya - tmpBounds[1];
					//}



				/*	double x = tmpMin[0] + tmpBounds[0] * ((rand()) / (double)RAND_MAX);
					double y = tmpMin[1] + tmpBounds[1] * ((rand()) / (double)RAND_MAX);
				

					

					if (x < mmin[0]) mmin[0] = x;
					if (y < mmin[1]) mmin[1] = y;
					if (x>mmax[0]) mmax[0] = x;
					if (y>mmax[1])mmax[1] = y;*/


				//	x = 0; y = 0; ra = 0;


						double ra = 0;
						if ((nrOfAngleProbes>1) && (preAngle>0))
							 ra = -preAngle + 2 * preAngle*((rand()) / (double)RAND_MAX);

					tr[1] = x*objectScale[0];
					tr[2] = y*objectScale[1];
					tr[3] = ra;
					
			

					PrepareTrans2(tr, t, nr, posTab);
					//	tmpRawData->ZeroQ();
					bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter,samplingX,samplingY);
					val = statsMutual[functionNumber];
					counter++;
					emit SendMessage("("+QString::number(sliceNumber)+"/)"+QString::number(resultNrSlices)+" NrCount: " + QString::number(counter) + "\t" + "Value: " + QString::number(val));

					if (paintTransformedSlicesEnabled == true) PaintTransformedSlices(t);

					qApp->processEvents();
					//tmpRawData->SaveRawDataSet2("f:\\data\\x\\test1-" + QString::number(x) + "-" + QString::number(y));
					//functionNumber ; //0 - count, 1 - ssd, 2 - cross cor, 3 - entropy, 4 -  mi, 5- nmi
					FValue = statsMutual[functionNumber];
					ccount = statsMutual[0];

					if ((FValue < testFValue) && (ccount > minCount))
					{
						UnPrepareTrans2(tr, t, nr, posTab);
						testFValue = FValue;
						fCount = ccount;
						for (int xx = 1; xx <=3; xx++)			minTrans[xx] = tr[xx];

					}
				}
			}







		}


		for (int xx = 1; xx < nr + 2; xx++)			tr[xx] = minTrans[xx];

		qWarning() << "Phase 0" << "Value:" << testFValue << "tx:" << tr[1] << "ty:" << tr[2] << "rot:" << tr[3];
		//automat!
		maxFound = 0;
		//PrepareTrans2(tr, t, 3, posTab);

		//1,2,3
		int valTest = chcks[1] + chcks[2] + chcks[3];
	//	if (chcks[1] == true)
		if (valTest>0)
		{

			int nrrx = 1;
			if (chcks[1] == true) { tr[nrrx] = minTrans[1]; posTab[nrrx++] = 1; }
			if (chcks[2] == true) { tr[nrrx] = minTrans[2]; posTab[nrrx++] = 2; }
			if (chcks[3] == true) { tr[nrrx] = minTrans[6]; posTab[nrrx++] = 6; }

			nrrx--;
			numOfParams = nrrx;

			Optim = new OptimizationPowell(tr, nrrx, Tolerance, &Iterations, &FValue, this);
			connect(this, SIGNAL(SignalStopOptimization()), Optim, SLOT(SlotStopOptimization()));
			Optim->StartOptimization();
			if (Optim->OptimizationStopped == true)this->optimizationStopped = true;
			delete Optim; Optim = NULL;
			//filteredModel->SaveRawDataSet2("e:\\data\\x\\model_" + QString::number(i));
			//filteredObject->SaveRawDataSet2("e:\\data\\x\\object_" + QString::number(i));
			PrepareTrans2(tr, t, nr, posTab);
			bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter);

			qWarning() << "Phase 1" << "Value:" << FValue << "Count:" << ccount << "tx:" << tr[1] << "ty:" << tr[2] << "rot:" << tr[3];

			if (ccount >= minCountAll)
			{
				PrepareTrans2(tr, t, nrrx, posTab);
				//for (int xx = 1; xx < nr + 2; xx++)			 minTrans[xx] = tr[xx];
				fCount = ccount;

				minTrans[1] = t[1];
				minTrans[2] = t[2];
				minTrans[3] = t[6];
			}
		}
		for (int xx = 1; xx <=3; xx++)			tr[xx] = minTrans[xx];
		//4/5/6
		
		
		
		valTest = chcks[4] + chcks[5] + chcks[6];
		//	if (chcks[1] == true)
		if (valTest>0)
		{

			int nrrx = 1;
			if (chcks[4] == true) { tr[nrrx] = minTrans[1]; posTab[nrrx++] = 1; }
			if (chcks[5] == true) { tr[nrrx] = minTrans[2]; posTab[nrrx++] = 2; }
			if (chcks[6] == true) { tr[nrrx] = minTrans[6]; posTab[nrrx++] = 6; }
			nrrx--;
			numOfParams = nrrx;
		//if (chcks[2] == true)
		//{
			Optim = new OptimizationPowell(tr, nrrx, Tolerance, &Iterations, &FValue, this);
			connect(this, SIGNAL(SignalStopOptimization()), Optim, SLOT(SlotStopOptimization()));
			Optim->StartOptimization();
			if (Optim->OptimizationStopped == true)this->optimizationStopped = true;
			delete Optim; Optim = NULL;
			PrepareTrans2(tr, t, nr, posTab);
			bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter);

			qWarning() << "Phase 2" << "Value:" << FValue << "Count:" << ccount << "tx:" << tr[1] << "ty:" << tr[2] << "rot:" << tr[3];
		
			if (ccount >= minCountAll)
			{
				PrepareTrans2(tr, t, nrrx, posTab);
				//for (int xx = 1; xx < nr + 2; xx++)			 minTrans[xx] = tr[xx];
				fCount = ccount;

				minTrans[1] = t[1];
				minTrans[2] = t[2];
				minTrans[3] = t[6];
			}
		}

	

		stream22 << i << "\t" << minTrans[1] << "\n";
		//dla bezpieczenstwa - przesuniecie jesli zbyt duze - przerywam
		tr2d[i * 2] = minTrans[1];
		tr2d[i * 2 + 1] = minTrans[2];

		if (i > 0)
		{
			double distx = GetDistance2D(tr2d + (i - 1) * 2, tr2d + i * 2);
			if (distx > maxDist)
			{
				minTrans[1] = tr2d[(i - 1) * 2];
				minTrans[2] = tr2d[(i - 1) * 2 + 1];
				tr2d[i * 2] = tr2d[(i - 1) * 2];
				tr2d[i * 2 + 1] = tr2d[(i - 1) * 2+1];
			}
			if (distx > totalMaxDist) totalMaxDist = distx;
		}



		for (int xx = 1; xx <= 3; xx++)			tr[xx] = minTrans[xx];
		PrepareTrans2(tr, t, nr, posTab);
		bool test = Update2DStatsParallel(scatterSize, hist1, hist2, scatter, statsMutual, bounds, imgMaskBits, filteredModel, filteredObject, intBuffers, ccount, NULL, true, t, false, rCenter);
		if (optimizationStopped == true) break;
	


		params[i * 3] = tr[1];
		params[i * 3 + 1] = tr[2];
		params[i * 3 + 2] = tr[3];

		double fValX = (statsMutual[0] > minCountAll) ? statsMutual[functionNumber] : INT_MAX;

		tempS[i]->DoubleVar = fValX;



		int ppos = i * 10;
		fvaluesX[ppos++] = sliceNumber + objSliceRange[0];
		fvaluesX[ppos++] = tr[1];
		fvaluesX[ppos++] = tr[2];
		fvaluesX[ppos++] = tr[3];
		fvaluesX[ppos++] = statsMutual[0];
		fvaluesX[ppos++] = statsMutual[1];
		fvaluesX[ppos++] = statsMutual[2];
		fvaluesX[ppos++] = statsMutual[3];
		fvaluesX[ppos++] = statsMutual[4];
		fvaluesX[ppos++] = statsMutual[5];

		if (sliceNr == -1) //dopasowanie stabilizacyjne - update mSliceData
		{
			posTab[0] = 0;
			posTab[1] = 1;
			posTab[2] = 2;
			posTab[3] = 6;
			numOfParams = 3;

			//memset(t, 0, 15 * sizeof(double));
			
		///	PrepareTrans2(tr, t, numOfParams, posTab);
			stream33 << i << "\t"<<numOfParams<< "\t";
			for (int kk = 0; kk < numOfParams; kk++)
			{
				stream33 << posTab[i] << "\t" <<tr[i]<< "\t";

			}
			
			stream33 << "\n";
			
			TransformSlice2(oSliceData, dstSlice, 0, tr, false);
			stream33 << "\n";
			memcpy(mSliceData->GetDataArray(), dstSlice->GetDataArray(), modelDataSliceSize*sizeof(unsigned short));
			FilterDataSet(mSliceData, filteredModel, filterPreprocessingName);
			filteredModel->FindMinMaxValue();
			
		//	

		}

		if (fValX < minFunctionValue) minFunctionValue = fValX;

		//qWarning(QString::number(i)+" "+QString::number(tr[1])+" "+QString::number(tr[2])+" "+QString::number(tr[3]));
		//	qWarning(QString::number(i)+" "+QString::number(trTotal[1])+" "+QString::number(trTotal[2])+" "+QString::number(trTotal[3]));
		qWarning() << "przed deloptim";
		if (Optim != NULL)delete Optim;

		emit SendMessage(QString::number(i) + "/" + QString::number(resultNrSlices - 1));
		QCoreApplication::processEvents();
		//tymczasowe!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		///////////////
		qWarning() << "koniec petli";
	}


	//QString fn = "f:\\data\\x\\test\params.txt";
	{QFile file("f:\\data\\x\\params.txt");
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		int poss = 0;
		for (i = 0; i < resultNrSlices; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				stream << fvaluesX[poss++] << "\t";
			}
			stream << "\n";
		}


		file.close();
	}}

	//zbieranie i zrzut do pliku
	delete [] fvaluesX;




	//delete tmpRawData; 
	//delete []intBuffers;
	delete filteredModel;
	filteredModel = NULL;
	delete filteredObject;
	filteredObject = NULL;
	////DLL FREE
	//dataSetList->GetItemAtPos(0)->SetData(NULL);
	//dataSetList->GetItemAtPos(1)->SetData(NULL);
	//delete dataSetList;
	//FiltDram_Free();

	int newNrOfFrames = resultNrSlices;
	RawDataSet *colDstData2 = NULL;

	

	RawDataSet *ghostFinalData = NULL;
	if ( (optimizationStopped == false))
	{
		if ((sort == true) && (sliceNr != -1))
		{

			QSortIntDouble(tempS, 0, nrOfFrames - 1);//posortowane
		}

		if ((removeOutliers == true) && (sliceNr!=-1))
		{
			newNrOfFrames=0;
			for (i = 0; i < nrOfFrames; i++)//modelData->GetNz()
			{

				double fVal = tempS[i]->DoubleVar;


				bool test = false;
				if (minFunctionValue > 0)
				{
					test = (fVal < minFunctionValue*(1.0 + percentageRejection));
				}
				else
				{
					test = (fVal < minFunctionValue*(1.0 - percentageRejection));
				}

				if (test == true) newNrOfFrames++;
			}

			//if (newNrOfFrames < nrOfFrames)
			//{
			//	RawDataSet *nRawData = new RawDataSet(modelData->GetNx(), modelData->GetNy(), newNrOfFrames, modelScale[0], modelScale[1], 1, 0); //nx,ny jak model ilosc przekrojow jak obiekt
			//	nRawData->ZeroQ();
			//	delete colDstData2;
			//	colDstData2 = nRawData;
			//}
		}

		
		int nrrrr = (sliceNr == -1) ? newNrOfFrames + 1 : newNrOfFrames; //musi byc o 1 wieksza dla stabilizacji
		
		colDstData2 = new RawDataSet(modelData->GetNx(), modelData->GetNy(), nrrrr, modelScale[0], modelScale[1], 1, 0); //nx,ny jak model ilosc przekrojow jak obiekt
		colDstData2->ZeroQ();

		if (ghostData != NULL)
		{
			ghostFinalData = new RawDataSet();
			ghostFinalData->Copy(*colDstData2,false);
			ghostFinalData->ZeroQ();
		}

		int pposX = 0;
		if (sliceNr == -1)
		{
			pposX++;
			memcpy(colDstData2->GetDataArray(), modelData->GetDataArray(), modelDataSliceSize*sizeof(unsigned short));  //skopiuje dane modelu na slice 0
		}

		posTab[0] = 0;
		posTab[1] = 1;
		posTab[2] = 2;
		posTab[3] = 6;
		numOfParams = 3;
		for (i = 0; i < nrOfFrames; i++)//modelData->GetNz()
		{

			sliceNumber = tempS[i]->IntVar;
			//a1=colDstData2->GetDataArray()+modelData->GetSliceSize()*(sliceNumber-1);
			
			Iterations = 0;
			FValue = 0;

			Optim = NULL;
			tmpBool = false;

			tr[0] = 0;
			tr[1] = params[sliceNumber * 3];
			tr[2] = params[sliceNumber * 3 + 1];
			tr[3] = params[sliceNumber * 3 + 2];

		/*	if ((sliceNr == -1) && (i == 0))
			{
				tr[0] = 0;
				tr[1] = 0;
				tr[2] = 0;
				tr[3] = 0;
			}*/

			double fVal = tempS[i]->DoubleVar;


			bool test = false;
			if (minFunctionValue > 0)
			{
				test = (fVal < minFunctionValue*(1.0 + percentageRejection));
			}
			else
			{
				test = (fVal < minFunctionValue*(1.0 - percentageRejection));
			}

			///if (test==false)
			//if (fVal < minFunctionValue*(1.0 + percentageRejection))
		/*	{
				tr[0] = 0;
				tr[1] = 0;
				tr[2] = 0;
				tr[3] = 0;
			}*/

			if (sliceNr == -1)test = true;
			if ((removeOutliers == false) || ((removeOutliers == true) && (test == true)))
			{

				a2 = objectData->GetDataArray() + objectData->GetSliceSize()*(sliceNumber + objSliceRange[0]);

				oSliceData->SetDataArray(a2, false); // bufor obiektu uzupelniony
				dstSlice->SetDataArray(colDstData2->GetDataArray() + colDstData2->GetSliceSize()*(pposX), false); // miejsce w ktore wrzuce wynik

				TransformSlice2(oSliceData, dstSlice, 0, tr, false);


				if (ghostFinalData != NULL)
				{
					a2 = ghostData->GetDataArray() + ghostData->GetSliceSize()*(sliceNumber + objSliceRange[0]);

					oSliceData->SetDataArray(a2, false); // bufor obiektu uzupelniony
					dstSlice->SetDataArray(ghostFinalData->GetDataArray() + ghostFinalData->GetSliceSize()*(pposX), false); // miejsce w ktore wrzuce wynik

					TransformSlice2(oSliceData, dstSlice, 0, tr, false);
				}

				if (Optim != NULL)delete Optim;

				emit SendMessage("Transforming :" + QString::number(i) + "/" + QString::number(colDstData2->GetNz() - 1));
				qWarning() << "FValue" + QString::number(fVal) + "\t Transforming slice: " + QString::number(sliceNumber) + "\tto position: " + QString::number(pposX);
				//QCoreApplication::processEvents();
				//tymczasowe!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				///////////////


				pposX++;
			}

		}


	}

	file22.close();
	file33.close();
	//QCoreApplication::processEvents();

	if ((optimizationStopped == false) && (sliceNr!=-1))
	{

		RawDataSet *tmpRawDatax = new RawDataSet();
		tmpRawDatax->Copy(*colDstData2, false);
		unsigned short *dd = tmpRawDatax->GetDataArray();
		unsigned short *md = mSliceData->GetDataArray();
		for (i = 0; i < newNrOfFrames; i++)
		{
			memcpy(dd + mSliceData->GetSliceSize()*i, md, mSliceData->GetSliceSize()*sizeof(unsigned short));
		}
		tmpRawDatax->SetRawDataSetName("modelTest");
		tmpRawDatax->CalculateHistogram();
		emit 	SignalSendRawDataSet(tmpRawDatax);
	}



	mSliceData->SetDataArray(NULL, false);
	oSliceData->SetDataArray(NULL, false);
	dstSlice->SetDataArray(NULL, false);

	delete mSliceData;
	delete oSliceData;
	delete dstSlice;

	if (tempS != NULL)
	{
		for (i = 0; i < nrOfFrames; i++)
		{
			delete tempS[i];
		}
		delete[]tempS;
	}
	delete[]params;
	if ((colDstData2 != NULL))
	{
		if ((optimizationStopped == false))
		{

			colDstData2->SetRawDataSetName("finalTest");
			colDstData2->CalculateHistogram();
			emit 	SignalSendRawDataSet(colDstData2);

			if (ghostFinalData != NULL)
			{
				ghostFinalData->SetRawDataSetName("ghostData");
				ghostFinalData->CalculateHistogram();
				emit 	SignalSendRawDataSet(ghostFinalData);
			}
		}
		else
			delete colDstData2;

	}
	grayMode = false;

//	QMessageBox::about(0, "", QString::number(totalTime) + " " + QString::number(totalTime1) + " " + QString::number(totalTime2) + " " + QString::number(totalTime3) + " " + QString::number(totalTime4));


	//     TransformImage(tr);
}
//-------------------------------------
void ImageSetRegistration::SlotStopOptimization()
{

	emit SignalStopOptimization();
	this->optimizationStopped = true;

}
//-------------------------------------------------------

void ImageSetRegistration::PaintTransformedSlices(double *TransT)
{

	int i, j;



	unsigned char Opacity = (regOpacity);


	if (Opacity != 0)
	{
		transversalDataSet->ZeroQ();




		transversalDataSet->SetFirstHistValue(oSliceData->GetFirstHistValue());
		transversalDataSet->SetLastHistValue(oSliceData->GetLastHistValue());

		//--------------------------------------
		//double tolX=1e-5;	

		double t1 = (double)(oSliceData->GetNx() - 1);
		double t2 = (double)(oSliceData->GetNy() - 1);



		unsigned short *TempPointer;

		Transform *transX = new Transform(rCenter, 1, 1, 1, false);
		/*		transX->CalculateTrigonometricFunctionsInv(TransT);
		transX->MakeMatrixInv(Matrix,TransT);
		*/
		//shear poprawka 2003.10.04
		/*	transX->CalculateTrigonometricFunctionsInv(TransT);
		transX->CalculateShearParametersInv(TransT);
		transX->MakeAffineMatrixInv(Matrix,TransT);
		*/
		transX->MakeCompleteAffineMatrix(matrix, TransT);

		//Normal
		double Point[3] = { 0 };
		double Point1[3] = { 0 };
		double PointF[3] = { 0 };

		//shear poprawka 2003.10.04

		double Ox = 1.0 / (oSliceData->GetXscale());
		double Oy = 1.0 / (oSliceData->GetYscale());






		//--------------------------------------
		//--------------------------------------
		//--------------------------------------
		//--------------------------------------
		//--------------------------------------

		TempPointer = transversalDataSet->GetDataArray();
		Point[0] = 0.0;
		Point[1] = 0.0;

		for (j = 0; j < transversalDataSet->GetNy(); j++)
		{
			for (i = 0; i < transversalDataSet->GetNx(); i++)
			{

				transX->MultiplyPoint(Point1, Point);
				PointF[0] = (Point1[0])*Ox;
				PointF[1] = (Point1[1])*Oy;
				if ((PointF[0] >= 0) && (PointF[0] < t1) &&
					(PointF[1] >= 0) && (PointF[1] < t2))
					//*(TempPointer)=Interpolation3Linear(ObjectData,PointF[0],PointF[1],PointF[2]);
					*(TempPointer) = oSliceData->GetDataArray()[(int)(PointF[0] + 0.5) + (int)(PointF[1] + 0.5)*objectData->GetNx()];
				TempPointer++;
				Point[0] += mSliceData->GetXscale();

			}
			Point[0] = 0.0;
			Point[1] += mSliceData->GetYscale();
		}

		transversalDataSet->SetMaxValue(oSliceData->GetMaxValue());
		transversalView->UpdatRegSlice(transversalDataSet, paletteReg, Opacity);



		TempPointer = NULL;
		delete	TempPointer;
		delete   transX;


	}

}

//-------------------------------------------------
void ImageSetRegistration::SlotSetParalellComputing(bool p, int nr)
{
	this->parallelComputing = p;
	this->numberOfThreadsUsed = nr;

}

//---------parallel version
bool ImageSetRegistration::Update2DStatsParallel(int bins, int* hist1, int* hist2, int* scatter, double* statsScatter, int* bounds, unsigned int* maskImgBits,
	RawDataSet* modelData, RawDataSet* objectData, int** tmpTab, int& count, unsigned short* result,

	bool useTrans, double* trans, bool invTrans, double* rc, int samplingX, int samplingY)
{

	unsigned int t1, t2;

	QTime time1;// = QTime::currentTime();// = datetime1.currentTime();
	time1.start();
	//t1 = time1.msec();



	if ((hist1 == NULL) || (hist2 == NULL) || (scatter == NULL) || (statsScatter == NULL)) return false;




	if (bins > 8)bins = 8;
	if (bins < 2)bins = 2;
	bins = pow((double)2, bins);




	if (parallelComputing == true)
	{

		if (tabsInitialized == true)
		{
			if (actualBins != bins)
			{
				for (int i = 0; i < numberOfThreads; i++)
				{
					delete[]scatterTabs[i];	scatterTabs[i] = NULL;
					scatterTabs[i] = new int[bins * bins];
				}
				actualBins = bins;
			}
		}
		else
		{

			for (int i = 0; i < numberOfThreads; i++)
			{
				scatterTabs[i] = new int[bins * bins];
			}


			actualBins = bins;
			tabsInitialized = true;
		}




		for (int i = 0; i < numberOfThreads; i++)
		{
			//	memset(ObjectTabs[i], 0, ScatterSize * sizeof(int));
			//	memset(ModelTabs[i], 0, ScatterSize * sizeof(int));
			memset(scatterTabs[i], 0, bins * bins * sizeof(int));
			memset(paramsTabs[i], 0, NROFPARAMS * sizeof(double));
			//counts[i]=0;
		}
	}




	double matrix[16];

	int objectN[3], modelN[3];
	double modelScale[3], objectScale[3];
	if (objectData != NULL)
	{

		objectData->GetScale(objectScale);
		objectData->GetN(objectN);
	}


	modelData->GetScale(modelScale);
	modelData->GetN(modelN);


	unsigned short* tab1 = modelData->GetDataArray();
	unsigned short* tab2 = objectData->GetDataArray();

	/*qreal x1, x2, y1, y2;
	int pCount = poly.count();
	bool polyExist;
	if (pCount > 1)
	{


	QRectF rect = poly.boundingRect();

	rect.getCoords(&x1, &y1, &x2, &y2);
	polyExist = true;
	}
	else
	{
	x1 = 0;
	y1 = 0;
	x2 = modelN[0] - 1;
	y2 = modelN[1] - 1;
	polyExist = false;

	}*/

	int x1 = bounds[0];
	int x2 = bounds[1];
	int y1 = bounds[2];
	int y2 = bounds[3];


	int X1 = (int)x1;
	int Y1 = (int)y1;
	int X2 = (int)x2 + 1;
	int Y2 = (int)y2 + 1;


	int NX = x2 - x1;
	int NY = y2 - y1;


	//if (interpolation != NULL) delete interpolation;interpolation = NULL;


	if (transformation != NULL) delete transformation; transformation = NULL;


	double c[2];
	c[0] = (X2 + X1) * 0.5;
	c[1] = (Y2 + Y1) * 0.5;
	double rCenter[3] = { c[0] * modelScale[0], c[1] * modelScale[1], 0 };//krece modelem

	if (rc != NULL)
	{
		rc[0] = rCenter[0]; rc[1] = rCenter[1]; rc[2] = rCenter[2];
	}


	transformation = new Transform(rCenter, 1, 1, 1);
	transformation->MakeCompleteAffineMatrix(matrix, trans);


	//int sliceSize=nx*ny;

	/*QImage img(modelN[0], modelN[1], QImage::Format_RGB32);
	img.fill(Qt::black);
	QPainter Painter2(&img);
	Painter2.setBrush(Qt::white);
	Painter2.setPen(1);
	Painter2.drawPolygon(poly);*/

	int i, j;
	bool test;
	int pos;
	//unsigned char *tab11=img.bits();
	//unsigned int *tab1x=(unsigned int *)tab11;

	memset(hist1, 0, bins * sizeof(int));
	//memset (stats1,0,16*sizeof(double));
	memset(hist2, 0, bins * sizeof(int));
	//memset (stats2,0,16*sizeof(double));

	memset(scatter, 0, bins * bins * sizeof(int));
	memset(statsScatter, 0, 16 * sizeof(double));



	double stddev;
	int  		sum1 = 0;
	int			sum2 = 0;
	long int 	sum12 = 0;
	long int	sum22 = 0;
	count = 0;
	unsigned char val1, val2;
	unsigned char min1 = bins;
	unsigned char min2 = bins;
	unsigned short max1 = 0, max2 = 0;

	int* realTab = tmpTab[0];


	//findMAx

	double scale1, scale2;

	if (useGlobalMax == false)
	{
		max1 = modelData->GetMaxValue();
		max2 = objectData->GetMaxValue();
	}
	else
	{
		max1 = max2 = globalMaxValue;
	}

	//unsigned short  biggerMaks=MAX(maxValue1,maxValue2);
	scale1 = (max1 > 0) ? (double)(bins - 1) / max1 : 1.0;
	scale2 = (max2 > 0) ? (double)(bins - 1) / max2 : 1.0;


	double m1Inv = 1.0 / max1;
	double m2Inv = 1.0 / max2;
	double bins1 = 1.0 / (bins - 1);

	//bool useBiggerMax,bool useThresholds,unsigned short leftTh,unsigned short rightTh,bool invThresh


	/*
	double leftTh1=leftThD*maxValue1;
	double rightTh1=rightThD*maxValue1;


	double leftTh2=leftThD*maxValue2;
	double rightTh2=rightThD*maxValue2;*/

	totalTime += time1.elapsed();
	time1.restart();
	if (parallelComputing == false)
	{

		double floatEnergy = 0;
		double tmpE;
		double tmpE1, tmpE2;
		tmpE1 = 0;
		tmpE2 = 0;
		tmpE = 0;
		double ee;
		double ssd = 0;

		double scalef[2] = { modelScale[0] / objectScale[0], modelScale[1] / objectScale[1] };
		double objscale1[2] = { 1.0 / objectScale[0], 1.0 / objectScale[1] };

		double Point[3] = { 0 };
		double Point1[3] = { 0 };
		unsigned short val;
		double t1 = objectN[0] - 1;
		double t2 = objectN[1] - 1;
		int countModel = 0;
		double dV1, dV2;
		for (j = y1; j <= y2; j+=samplingY)
		{
			for (i = x1; i <= x2; i+=samplingX)
			{
				pos = i + j*modelN[0];;


				test = (maskImgBits != NULL) ? (((maskImgBits[pos]) == 0xffffffff)) : true;
				if ((test == true))
				{
					countModel++;
					Point[0] = i*modelScale[0];//przejscie do ukladu wspolrzednych globalnych 
					Point[1] = j*modelScale[1];

					transformation->MultiplyPoint(Point1, Point);
					Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu



					if ((Point1[0] >= 0) && (Point1[0] < t1) &&
						(Point1[1] >= 0) && (Point1[1] < t2))
					{
						dV1 = tab1[pos] ;
						dV2= interpolation->GetInterpolatedValue(objectData, Point1[0], Point1[1], 0) ;


						val1 = dV1*scale1;//tab1[pos] * scale1; - zakres 0-bins-1
						val2 = dV2*scale2;//interpolation->GetInterpolatedValue(objectData, Point1[0], Point1[1], 0)* scale2;- zakres 0-bins-1

						dV1 *= m1Inv;//zakres 0-1
						dV2 *= m2Inv;//zakres 0-1

						//if (val2>max) max = val; if (val < min) min = val;

						//test
						bool test1 = true;
						bool test2 = true;
						if (result != NULL) result[pos] = val2;


						hist1[val1]++;
						sum1 += dV1;
						sum12 += dV1*dV1;

						hist2[val2]++;
						sum2 += dV2;
						sum22 += dV2 * dV2;

						scatter[val1*bins + val2]++;
						if (realTab != NULL) realTab[count] = pos;

						ee = (dV1 - dV2);
						ssd += ee*ee;
						tmpE += (dV1 - dV2) * (dV1 - dV2);//*0.0001;
						tmpE1 += dV1 * dV1;// *0.0001;
						tmpE2 += dV2 * dV2;//(val2* val2) * 0.0001;

						count++;

						//	}

					}






				}
			}

		}


		totalTime1 += time1.elapsed();
		time1.restart();

		//histogramy pozbierane
		if ((count > 0))// &&  (realTab != NULL))
		{

			//if (count / (double)countModel > 0.9)
			{

				statsScatter[0] = count;	//count
				statsScatter[1] = ssd / count; //ssd
				statsScatter[2] = (tmpE1*tmpE2 > 0) ? tmpE / (sqrt(tmpE1)*sqrt(tmpE2)) : 0; //cross correlation


				double entropy, mi;
				CompMutual(mi, entropy, scatter, hist1, hist2, bins, count);

				statsScatter[3] = entropy;
				statsScatter[4] = mi;
				statsScatter[5] = 2.0*mi / (GetEntropy(hist1, bins, count) + GetEntropy(hist2, bins, count)); //NMI1
			}
			//else
			//{
			//	statsScatter[0] = count;	//count
			//	statsScatter[1] = 100000;
			//}




			//	stats[1]=
			//policze statystyki
		}
	}
	else
	{
		//wersja zrownloeglona dopasowania






		int **tabs;
		tabs = new int *[numberOfThreadsUsed];
		int k;
		for (k = 0; k<this->numberOfThreadsUsed; k++)
		{
			tabs[k] = scatterTabs[k];

		}


		double **tabs2;
		tabs2 = new double *[numberOfThreadsUsed];
		
		for (k = 0; k<this->numberOfThreadsUsed; k++)
		{
			tabs2[k] = paramsTabs[k];

		}


		//int *counts;
		#pragma omp parallel  num_threads(numberOfThreadsUsed) //private (tabs1,tabs2)//private(counts)//private (counts,tabs)//shared(Q1,paddQ)  //num_threads(1)
		{
			int tid = omp_get_thread_num();
			//unsigned short *t1 = Q1;
		//	unsigned short *t2 = paddQ;

			//counts = new int[numberOfThreads];
		//	#pragma omp critical
			//for (k = 0; k<this->numberOfThreads; k++)counts[k] = 0;
			//#pragma omp critical



			#pragma omp for  nowait ordered schedule (guided) private(k) //private (counts)//ordered schedule(dynamic) p
			for (k = y1; k<=y2; k += samplingY)
			{
				//int thNr= omp_get_thread_num();
		
				//UpdateHistograms(y, tabs[tid], Qmodel, Qobject, x1, x2, , counts[tid],paramsTabs[tid]);
				UpdateHistograms(k, tabs[tid], modelData, objectData, x1, x2, samplingX, tabs2[tid], scale1, scale2,m1Inv,m2Inv, maskImgBits, bins);
			}

#pragma omp critical
			{

				/*for (k = 0; k<numberOfThreads; k++)
					count += counts[k];
				delete[]counts;*/
			}

		}
		totalTime1 += time1.elapsed();
		time1.restart();
#pragma omp for
		for (i = 0; i<this->numberOfThreadsUsed; i++)
		{
			for (j = 0; j<bins*bins; j++)
			{
				scatter[j] += tabs[i][j];
			}

		}
		totalTime2 += time1.elapsed();
		time1.restart();
		for (k = 0; k<numberOfThreadsUsed; k++)
		{
			tabs[k] = NULL;
			tabs2[k] = NULL;
			//count+=counts[k];
		}
		delete[]tabs;
		delete[]tabs2;
		//	delete []counts;

	
		//teoretycznie zebrane trzeba teraz uporzadkowac
//#pragma omp parallel  num_threads(numberOfThreadsUsed) 
		

//#pragma omp
	
		int pos1 = 0;
		int pos2 = 0;
		for (j = 0; j < bins; j++)
		{
			pos1 = j;
			for (i = 0; i < bins; i++)
			{
				//hist1[j] += scatter[bins*i + j];
				//hist2[j] += scatter[bins*j + i];
				hist2[j] += scatter[pos1];
				hist1[j] += scatter[pos2++];
				pos1 += bins;
			}
			

		}


		
		totalTime3 += time1.elapsed();
		time1.restart();

		double params[NROFPARAMS] = { 0 };
		for (i = 0; i < this->numberOfThreadsUsed; i++)
		{
			for (j = 0; j < 9; j++)
			{
				params[j] += paramsTabs[i][j];
			}
		}
			//statsScatter



		int count = params[0]; 

		if (count > 0)
		{
			statsScatter[0] = count;	//count
			statsScatter[1] = params[5] / count; //ssd
			statsScatter[2] = (params[7] * params[8] > 0) ? params[6] / (sqrt(params[7])*sqrt(params[8])) : 0; //cross correlation


			double entropy, mi;
			CompMutual(mi, entropy, scatter, hist1, hist2, bins, count);

			statsScatter[3] = entropy;
			statsScatter[4] = mi;
			statsScatter[5] = 2.0*mi / (GetEntropy(hist1, bins, count) + GetEntropy(hist2, bins, count)); //NMI1

		}
		totalTime4 += time1.elapsed();
		//time1.restart();
		

	}
	//if (interpolation != NULL) delete interpolation; interpolation = NULL;


	if (transformation != NULL) delete transformation; transformation = NULL;


	
	
	//QTime time2=QTime::currentTime();// = datetime1.currentTime();
	
	
	

	
	//time1.restart();

	return true;


}
//----------------------------------------------
void	ImageSetRegistration::UpdateHistograms(int y, int*scatTab, RawDataSet *modelData, RawDataSet *objectData, int x1, int x2, int stepx, double *params, double scale1, double scale2,double m1Inv,double m2Inv,
	unsigned int*maskImgBits,	int bins)
{
	
	/*int nx;
	unsigned int*maskImgBits;


	int objectN[3];
	int modelN[3];
	double objectScale[3];
	double modelScale[3];
	double scale1, scale2;

*/



	double Point[3] = { 0 };
	double Point1[3] = { 0 };
	int pos = 0;
	bool test;

	int objectN[3];
	objectData->GetN(objectN);
	double objectScale[3];
	objectData->GetScale(objectScale);

	int modelN[3];
	modelData->GetN(modelN);
	double modelScale[3];
	modelData->GetScale(modelScale);
	
	double t1 = objectN[0] - 1;
	double t2 = objectN[1] - 1;
	double objscale1[2] = { 1.0 / objectScale[0], 1.0 / objectScale[1] };

	unsigned short *modelTab = modelData->GetDataArray();
	double bins1 = 1.0 / (bins - 1);
	double ee;
	unsigned char val1, val2;

	val1 = 0;
	val2 = 0;
	double dV1, dV2;
	for (int i = x1; i <= x2; i += stepx)
	{
		pos = i + y*modelN[0];
		test = (maskImgBits != NULL) ? (((maskImgBits[pos]) == 0xffffffff)) : true;
		if ((test == true))
		{
			Point[0] = i*modelScale[0];//przejscie do ukladu wspolrzednych globalnych 
			Point[1] = y*modelScale[1];

			transformation->MultiplyPoint(Point1, Point);
			Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu

			if ((Point1[0] >= 0) && (Point1[0] < t1) &&
				(Point1[1] >= 0) && (Point1[1] < t2))
			{

				dV1 = modelTab[pos];
				dV2 = interpolation->GetInterpolatedValue(objectData, Point1[0], Point1[1], 0);


				val1 = dV1 * scale1; //0-bins-1
				val2 = dV2 * scale2;//0-bins-1

				dV1 *= m1Inv;//zakres 0-1
				dV2 *= m2Inv;//zakres 0-1

				//val1 = modelTab[pos] * scale1;
				//val2 = interpolation->GetInterpolatedValue(objectData, Point1[0], Point1[1], 0)*scale2;


				scatTab[val1*bins + val2]++;

				params[0] ++; //count
				params[1] += dV1; //sum1
				params[2] += dV1 * dV1;//sum12
				params[3] += dV2; //sum2
				params[4] += dV2 * dV2;//sum22
				
				ee = (dV1 - dV2);//*bins1;
				params[5] += ee*ee; //ssd
				params[6] += (dV1 - dV2) * (dV1 - dV2);//*0.0001; //tmpE
				params[7] += dV1 * dV1;// *0.0001; //tmpE1
				params[8] += (dV2 * dV2);//*0.0001; //tmpE2
				
			}


			
		}


	}




}