
#include "OtherCommonFunctions.h"
#include <qaction.h>



// Funkcja do obliczania średniej
double calculateAverage(const QVector<double>& vec) {
	if (vec.isEmpty()) return 0.0;
	double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
	return sum / vec.size();
}
//-------------------------------------------------
// Funkcja do obliczania mediany
double calculateMedian(QVector<double> vec) {
	if (vec.isEmpty()) return 0.0;
	std::sort(vec.begin(), vec.end());
	int size = vec.size();
	if (size % 2 == 0) {
		return (vec[size / 2 - 1] + vec[size / 2]) / 2.0;
	}
	else {
		return vec[size / 2];
	}
}
//-----------------------------------
unsigned short GetMaxValue(unsigned short* tab, int size)
{
	if (tab != NULL)
	{

		unsigned short m = 0;
		for (int i = 0; i < size; i++)
		{
			if (tab[i] > m) m = tab[i];

		}
		return m;
	}
	else return 0;
}
//-------------------------
unsigned char  GetMaxValue(unsigned char* tab, int size)
{

	if (tab != NULL)
	{

		unsigned short m = 0;
		for (int i = 0; i < size; i++)
		{
			if (tab[i] > m) m = tab[i];

		}
		return m;
	}
	else return 0;
}

void DrawPixel(QImage* Img, int x, int y, unsigned int Col)
{
	//	((unsigned char *)Img->bits())

	((unsigned int*)Img->bits())[y * Img->width() + x] = Col;
}
//---------------------------------------------------

//---------------------------------------------------

//--------------------NEAREST NEIGHBORS--------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------


void ScaleImageNearest(QImage* Source_Image, QImage* Dest_Image)
{




	unsigned int* source_bits = (unsigned int*)Source_Image->bits();
	unsigned int* dest_bits = (unsigned int*)Dest_Image->bits();
	ScaleNearest(dest_bits, source_bits, Source_Image->width(), Source_Image->height(), Dest_Image->width(), Dest_Image->height());
}

//---------------------------------------------------

void ScaleNearest(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight)
{

	int i, j;


	unsigned int* TX = new unsigned int[TgtWidth];
	unsigned int* TY = new unsigned int[TgtHeight];


	unsigned int* ScanLineX = new unsigned int[TgtWidth];

	unsigned int* ScanLineXStart = ScanLineX;
	unsigned int* TXStart = TX;

	double ScaleX = (double)(SrcWidth) / (double)TgtWidth;
	double ScaleY = (double)SrcHeight / (double)TgtHeight;

	int rest;


	TX[0] = 0;
	TY[0] = 0;

	int tmp;

	tmp = 0;

	double t1 = 0.0;
	double t2 = 0.0;

	double t12tmp;
	for (i = 1; i < TgtWidth; i++)
	{
		t12tmp = (int)t1;
		t1 += ScaleX;
		TX[i] = (int)t1 - t12tmp;
		tmp += TX[i];
	}


	rest = (unsigned int)(SrcWidth - tmp);
	/*
	FILE *Do_opa;
	Do_opa=fopen("c:\\i.txt","a");
	fprintf(Do_opa,"%d   \n",rest);
	fclose(Do_opa);
	*/

	for (i = 1; i < TgtHeight; i++)
	{
		t12tmp = (int)t2;
		t2 += ScaleY;
		TY[i] = (int)t2 - t12tmp;
	}


	//--replay -- dla zerowej linii


	for (i = 0; i < TgtWidth; i++)
	{
		Source += *(TX++);
		*(ScanLineX++) = *(Source);
	}
	//Source+=rest-1;// to byl ten durny byk!2003.06.08 
	// 2003.09.25 - wykomentowalem to co powyzej i przeszedlem na double

	TX = TXStart;
	ScanLineX = ScanLineXStart;

	memcpy(Target, ScanLineX, TgtWidth * sizeof(unsigned int));
	Target += TgtWidth;

	Source += rest;

	int TgtWidthSizeOfPixel = TgtWidth * sizeof(unsigned int);
	//------------


	for (j = 1; j < TgtHeight; j++)
	{

		if ((TY[j] != 0))
		{
			if (TY[j] > 1)
				Source += SrcWidth * (TY[j] - 1);

			TX = TXStart;
			ScanLineX = ScanLineXStart;

			for (i = 0; i < TgtWidth; i++)
			{
				Source += *(TX++);
				*(ScanLineX++) = *(Source);
			}

			Source += rest;


		}

		ScanLineX = ScanLineXStart;
		memcpy(Target, ScanLineX, TgtWidthSizeOfPixel);
		Target += TgtWidth;


	}

	TX = TXStart;
	ScanLineX = ScanLineXStart;

	TXStart = NULL;
	ScanLineXStart = NULL;


	delete	TXStart;
	delete	ScanLineXStart;

	delete	ScanLineX;
	delete	TX;
	delete	TY;

}

//---------------------------------------------------
void ScaleNearest(unsigned char* Target, unsigned char* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight)
{

	int i, j;


	unsigned char* TX = new unsigned char[TgtWidth];
	unsigned char* TY = new unsigned char[TgtHeight];


	unsigned char* ScanLineX = new unsigned char[TgtWidth];

	unsigned char* ScanLineXStart = ScanLineX;
	unsigned char* TXStart = TX;

	double ScaleX = (double)(SrcWidth) / (double)TgtWidth;
	double ScaleY = (double)SrcHeight / (double)TgtHeight;

	int rest;


	TX[0] = 0;
	TY[0] = 0;

	int tmp;

	tmp = 0;

	double t1 = 0.0;
	double t2 = 0.0;

	double t12tmp;
	for (i = 1; i < TgtWidth; i++)
	{
		t12tmp = (int)t1;
		t1 += ScaleX;
		TX[i] = (int)t1 - t12tmp;
		tmp += TX[i];
	}


	rest = (unsigned int)(SrcWidth - tmp);
	/*
	FILE *Do_opa;
	Do_opa=fopen("c:\\i.txt","a");
	fprintf(Do_opa,"%d   \n",rest);
	fclose(Do_opa);
	*/

	for (i = 1; i < TgtHeight; i++)
	{
		t12tmp = (int)t2;
		t2 += ScaleY;
		TY[i] = (int)t2 - t12tmp;
	}


	//--replay -- dla zerowej linii


	for (i = 0; i < TgtWidth; i++)
	{
		Source += *(TX++);
		*(ScanLineX++) = *(Source);
	}
	//Source+=rest-1;// to byl ten durny byk!2003.06.08 
	// 2003.09.25 - wykomentowalem to co powyzej i przeszedlem na double

	TX = TXStart;
	ScanLineX = ScanLineXStart;

	memcpy(Target, ScanLineX, TgtWidth * sizeof(unsigned char));
	Target += TgtWidth;

	Source += rest;

	int TgtWidthSizeOfPixel = TgtWidth * sizeof(unsigned char);
	//------------


	for (j = 1; j < TgtHeight; j++)
	{

		if ((TY[j] != 0))
		{
			if (TY[j] > 1)
				Source += SrcWidth * (TY[j] - 1);

			TX = TXStart;
			ScanLineX = ScanLineXStart;

			for (i = 0; i < TgtWidth; i++)
			{
				Source += *(TX++);
				*(ScanLineX++) = *(Source);
			}

			Source += rest;


		}

		ScanLineX = ScanLineXStart;
		memcpy(Target, ScanLineX, TgtWidthSizeOfPixel);
		Target += TgtWidth;


	}

	TX = TXStart;
	ScanLineX = ScanLineXStart;

	TXStart = NULL;
	ScanLineXStart = NULL;


	delete	TXStart;
	delete	ScanLineXStart;

	delete	ScanLineX;
	delete	TX;
	delete	TY;

}

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//------------------SMOOTH BRESENHAM--------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------


//---------------------------------------------------

void ScaleImageBresenham(QImage* Source_Image, QImage* Dest_Image)
{


	unsigned int* source_bits = (unsigned int*)Source_Image->bits();
	unsigned int* dest_bits = (unsigned int*)Dest_Image->bits();
	ScaleRectAvg(dest_bits, source_bits, Source_Image->width(), Source_Image->height(), Dest_Image->width(), Dest_Image->height());


}


//-------------------------------------


void ScaleLineAvg(unsigned int* Target, unsigned int* Source, int SrcWidth, int TgtWidth)
{
	int NumPixels = TgtWidth;
	int IntPart = SrcWidth / TgtWidth;
	int FractPart = SrcWidth % TgtWidth;
	int Mid = TgtWidth / 2;
	int E = 0;
	int skip;
	unsigned int p;

	skip = (TgtWidth < SrcWidth) ? 0 : TgtWidth / (2 * SrcWidth) + 1;
	NumPixels -= skip;

	while (NumPixels-- > 0)
	{
		p = *Source;
		if (E >= Mid)
			p = average(p, *(Source + 1));
		*Target++ = p;
		Source += IntPart;
		E += FractPart;
		if (E >= TgtWidth)
		{
			E -= TgtWidth;
			Source++;
		}
	}

	while (skip-- > 0)
		*Target++ = *Source;
}

//---------------------------------------------------------------------


void ScaleRectAvg(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight)
{
	int NumPixels = TgtHeight;
	int IntPart = (SrcHeight / TgtHeight) * SrcWidth;
	int FractPart = SrcHeight % TgtHeight;
	int Mid = TgtHeight / 2;
	int E = 0;
	int skip;
	unsigned int* ScanLine, * ScanLineAhead;
	unsigned int* PrevSource = NULL;
	unsigned int* PrevSourceAhead = NULL;

	skip = (TgtHeight < SrcHeight) ? 0 : TgtHeight / (2 * SrcHeight) + 1;
	NumPixels -= skip;

	ScanLine = (unsigned int*)malloc(TgtWidth * sizeof(unsigned int));
	ScanLineAhead = (unsigned int*)malloc(TgtWidth * sizeof(unsigned int));

	while (NumPixels-- > 0)
	{


		if (Source != PrevSource)
		{
			if (Source == PrevSourceAhead)
			{
				/* the next scan line has already been scaled and stored in
				* ScanLineAhead; swap the buffers that ScanLine and ScanLineAhead
				* point to
				*/
				unsigned int* tmp = ScanLine;
				ScanLine = ScanLineAhead;
				ScanLineAhead = tmp;
			}
			else
			{
				ScaleLineAvg(ScanLine, Source, SrcWidth, TgtWidth);
			} /* if */
			PrevSource = Source;
		} /* if */


		if (E >= Mid && PrevSourceAhead != Source + SrcWidth)
		{
			int x;
			ScaleLineAvg(ScanLineAhead, Source + SrcWidth, SrcWidth, TgtWidth);
			for (x = 0; x < TgtWidth; x++)
				ScanLine[x] = average(ScanLine[x], ScanLineAhead[x]);
			PrevSourceAhead = Source + SrcWidth;
		} /* if */


		memcpy(Target, ScanLine, TgtWidth * sizeof(unsigned int));
		Target += TgtWidth;
		Source += IntPart;
		E += FractPart;

		if (E >= TgtHeight)
		{
			E -= TgtHeight;
			Source += SrcWidth;
		} /* if */


	} /* while */

	if (skip > 0 && Source != PrevSource)
		ScaleLineAvg(ScanLine, Source, SrcWidth, TgtWidth);

	while (skip-- > 0)
	{
		memcpy(Target, ScanLine, TgtWidth * sizeof(unsigned int));
		Target += TgtWidth;
	} /* while */

	free(ScanLine);
	free(ScanLineAhead);
}






//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//------------------LINEAR SCALING--------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------




void ScaleImageLinear(QImage* Source_Image, QImage* Dest_Image)
{




	unsigned int* source_bits = (unsigned int*)Source_Image->bits();
	unsigned int* dest_bits = (unsigned int*)Dest_Image->bits();
	ScaleLinear(dest_bits, source_bits, Source_Image->width(), Source_Image->height(), Dest_Image->width(), Dest_Image->height());
}


//---------------------------------------------------------------------


void PaddingBy2(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight)
{


	int width_p = SrcWidth + 4;
	int height_p = SrcHeight + 4;

	int i, j;


	unsigned int* padded_input = Target;
	unsigned int* input = Source;


	memset(padded_input, 0, (width_p * height_p * sizeof(unsigned int)));


	for (i = 0; i < SrcHeight; i++)
		memcpy(((width_p * (i + 2)) + 2 + padded_input), ((SrcWidth * i) + input), SrcWidth * sizeof(unsigned int));


	for (j = 2; j < (height_p - 2); j++)
	{
		for (i = 0; i < 2; i++)
			padded_input[i + (width_p * j)] = padded_input[2 + (width_p * j)];

		for (i = (width_p - 2); i < width_p; i++)
			padded_input[i + (width_p * j)] = padded_input[(width_p - 3) + (width_p * j)];
	}



	for (i = 0; i < width_p; i++)
	{
		for (j = 0; j < 2; j++)
			padded_input[i + (width_p * j)] = padded_input[i + (width_p * 2)];

		for (j = (height_p - 2); j < height_p; j++)
			padded_input[i + (width_p * j)] = padded_input[i + (width_p * (height_p - 3))];

	}



}


//---------------------------------------------------------------------



void ScaleLinear(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight)
{

	double	f_x, f_y, a, b, pix1, pix2, pix3, pix4;
	int		i_x, i_y;
	int		i, j, k;


	int width_p = SrcWidth + 4;
	//	int height_p = SrcHeight + 4;

	int wp = width_p * 4;
	int tgw = TgtWidth * 4;

	//	unsigned int *padded_input=(unsigned int *)malloc(width_p * height_p * sizeof(unsigned int));

	//	PaddingBy2(padded_input,Source, SrcWidth,  SrcHeight);

	unsigned int* padded_input = Padding2D_Unsigned_Int(Source, SrcWidth, SrcHeight, 2);




	unsigned char* Src = (unsigned char*)padded_input;
	unsigned char* Trg = (unsigned char*)Target;

	int xx, ii;
	int xx8, xx12, yy2, yy3;
	double aa;//,bb;

	double KX = (double)SrcWidth / (double)TgtWidth;
	double KY = (double)SrcHeight / (double)TgtHeight;


	// Interpolate the original image in raster order from top-left corner
	f_y = 0;
	int jtgw = -tgw;
	for (j = 0; j < TgtHeight; j++)
	{
		jtgw += tgw;

		f_y += KY;
		i_y = (int)(f_y);
		a = f_y - i_y;
		aa = (1.0F - a);


		yy2 = wp * (i_y + 1);
		yy3 = yy2 + wp;
		f_x = -KX;
		for (i = 0; i < TgtWidth; i++)
		{
			f_x += KX;

			i_x = (int)(f_x);
			b = f_x - i_x;

			xx = (i_x << 2);
			ii = (i << 2) + jtgw;


			xx8 = xx + 8;
			xx12 = xx8 + 4;

			for (k = 0; k < 3; k++)
			{
				pix1 = (double)Src[xx8 + yy2];
				pix2 = (double)Src[xx12 + yy2] - pix1;
				pix3 = (double)Src[xx8 + yy3];
				pix4 = (double)Src[xx12 + yy3] - pix3;

				Trg[ii++] = (unsigned int)(aa * (pix1 + b * pix2) + a * (pix3 + b * pix4));
				xx8++;
				xx12++;


			}
		}
	}

	free(padded_input);

}


//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//------------------Bicubic SCALING--------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------

//---------------------------------------------------




void ScaleImageBicubic(QImage* Source_Image, QImage* Dest_Image, double* XFParamTab, double* YFParamTab)
{




	unsigned int* source_bits = (unsigned int*)Source_Image->bits();
	unsigned int* dest_bits = (unsigned int*)Dest_Image->bits();
	BicubicScaling(dest_bits, source_bits, Source_Image->width(), Source_Image->height(), Dest_Image->width(), Dest_Image->height(), XFParamTab, YFParamTab);
}


//---------------------------------------------------------------------


void BicubicScaling(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight, double* XFParamTab, double* YFParamTab)
{

	double	f_x, f_y;
	int		i_x, i_y;
	int		i, j, m, n;


	int width_p = SrcWidth + 4;
	//	int height_p = SrcHeight + 4;

	int wp = width_p * 4;
	int tgw = TgtWidth * 4;


	unsigned int* padded_input = Padding2D_Unsigned_Int(Source, SrcWidth, SrcHeight, 2);
	//	unsigned int *padded_input=(unsigned int *)malloc(width_p * height_p * sizeof(unsigned int));

	//	PaddingBy2(padded_input,Source, SrcWidth,  SrcHeight);



	unsigned char* Src = (unsigned char*)padded_input;
	unsigned char* Trg = (unsigned char*)Target;

	int xx, ii;

	double tmp1, tmp2, tmp3;
	double r12;
	int yy2;

	int t1;

	int n4, mm;

	double KX = (double)SrcWidth / (double)TgtWidth;
	double KY = (double)SrcHeight / (double)TgtHeight;



	int posX = 0;
	int posY = 0;
	f_y = -KY;
	t1 = -tgw;
	for (j = 0; j < TgtHeight; j++)
	{
		f_y += KY;
		i_y = (int)f_y;
		yy2 = i_y + 1;
		t1 += tgw;
		f_x = -KX;

		for (i = 0; i < TgtWidth; i++)
		{
			f_x += KX;
			i_x = (int)f_x;

			xx = (i_x << 2) + 8;
			ii = (i << 2) + t1;


			tmp1 = 0.0;
			tmp2 = 0.0;
			tmp3 = 0.0;

			for (m = 0; m < 4; m++)
			{
				mm = (wp * (yy2 + m));
				for (n = 0; n < 4; n++)
				{
					n4 = (n << 2) + xx + mm;
					r12 = YFParamTab[posY + m] * XFParamTab[posX + n];
					tmp1 += (double)(Src[n4++]) * r12;
					tmp2 += (double)(Src[n4++]) * r12;
					tmp3 += (double)(Src[n4]) * r12;
				}

			}

			Trg[ii] = (unsigned char)tmp1;
			Trg[ii + 1] = (unsigned char)tmp2;
			Trg[ii + 2] = (unsigned char)tmp3;



			posX += 4;
		}
		posY += 4;
	}

	free(padded_input);


}



//---------------------------------------------------------------------

void CreateBSplineTabs(int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight, double* XTab, double* YTab)
{



	double	f_x, f_y, a, b;
	int		i_x, i_y;
	int		i, j;
	int yy2;
	int t1;
	double KX = (double)SrcWidth / (double)TgtWidth;
	double KY = (double)SrcHeight / (double)TgtHeight;
	int tgw = TgtWidth * 4;

	f_y = -KY;


	t1 = -tgw;


	int pos;

	pos = 0;


	for (j = 0; j < TgtHeight; j++)
	{
		f_y += KY;
		i_y = (int)f_y;
		a = f_y - i_y;
		yy2 = i_y + 1;

		YTab[pos++] = Cubic_bspline(-1.0 - a);
		YTab[pos++] = Cubic_bspline(a);
		YTab[pos++] = Cubic_bspline(1.0 - a);
		YTab[pos++] = Cubic_bspline(2.0 - a);
	}



	pos = 0;
	for (j = 0; j < TgtHeight; j++)
	{
		f_y += KY;
		i_y = (int)f_y;
		a = f_y - i_y;
		yy2 = i_y + 1;




		t1 += tgw;

		f_x = -KX;

		for (i = 0; i < TgtWidth; i++)
		{
			f_x += KX;
			i_x = (int)f_x;
			b = f_x - i_x;

			XTab[pos++] = Cubic_bspline(1.0 + b);
			XTab[pos++] = Cubic_bspline(-b);
			XTab[pos++] = Cubic_bspline(b - 1.0);
			XTab[pos++] = Cubic_bspline(b - 2.0);


		}
	}



}




double Cubic_bspline(double x)
{

	double a, b, c, d;
	a = (x <= -2.0) ? 0.0F : (x + 2) * (x + 2) * (x + 2);
	b = (x <= -1.0) ? 0.0F : (x + 1) * (x + 1) * (x + 1);
	c = (x <= 0.0) ? 0.0F : x * x * x;
	d = (x <= 1.0) ? 0.0F : (x - 1.0) * (x - 1.0) * (x - 1.0);
	return (one_sixth * (a - (4.0 * b) + (6.0 * c) - (4.0 * d)));

}

//---------------------------------------------------------------------
void SettSettings(QSettings& sett, QString contains, QString compare, QAction* act)

{

	if (sett.contains(contains))
	{

		QString txt = sett.value(contains).toString();
		bool test = (txt.compare(compare) == 0);
		act->setChecked(test);

	}
}

void SettSettings(QSettings& sett, QString contains, QString compare, QCheckBox* box)
{

	if (sett.contains(contains))
	{

		QString txt = sett.value(contains).toString();
		bool test = (txt.compare(compare) == 0);
		box->setChecked(test);

	}
}
//------------------------------------------------------
void SettSettings(QSettings& sett, QString contains, QString compare, QSlider* slider)
{

	if (sett.contains(contains))
	{

		int value = sett.value(contains).toInt();
		//bool test = (txt.compare(compare) == 0);
		slider->setValue(value);

	}
}
//------------------------------------------------------
void SettSettings(QSettings& sett, QString contains, QString compare, QLineEdit* ledit)
{

	if (sett.contains(contains))
	{

		//int value = sett.value(contains).toInt();
		//bool test = (txt.compare(compare) == 0);
		QString txt = sett.value(contains).toString();;
		ledit->setText(txt);

	}
}
//------------------
void SettSettings(QSettings& sett, QString contains, QString compare, QSpinBox* sbox)
{

	if (sett.contains(contains))
	{

		//int value = sett.value(contains).toInt();
		//bool test = (txt.compare(compare) == 0);
		int val = sett.value(contains).toInt();;
		sbox->setValue(val);

	}
}
//----------------------------------------------------------------
double		GetTriangleArea2D(double p0[2], double p1[2], double p2[2])

{
	double a = GetDistance2D(p0, p1);
	double b = GetDistance2D(p1, p2);
	double c = GetDistance2D(p2, p0);

	double s = (a + b + c) * 0.5;
	double perimeter = a + b + c;
	double area = sqrt(s * (s - a) * (s - b) * (s - c));
	return area;
}
//---------------------------------
void		CalculateSkewNessAndKurtosis(double* tab, int size, double& skewness, double& kurtosis)
{
	if ((tab != NULL) && (size > 1))
	{

		skewness = 0;
		kurtosis = 0;

		double n = 0;
		double M1 = 0;
		double M2 = 0;
		double M3 = 0;
		double M4 = 0;
		double x;
		double n1;
		double delta, delta_n, delta_n2, term1;;

		double tol = 1e-6;
		for (int i = 0; i < size; i++)
		{
			x = tab[i];
			n1 = n;
			n = n + 1;
			delta = x - M1;
			delta_n = delta / n;
			delta_n2 = delta_n * delta_n;
			term1 = delta * delta_n * n1;
			M1 += delta_n;
			M4 += term1 * delta_n2 * (n * n - 3 * n + 3) + 6 * delta_n2 * M2 - 4 * delta_n * M3;
			M3 += term1 * delta_n * (n - 2) - 3 * delta_n * M2;
			M2 += term1;
		}
		if (fabs(M2) > tol)
		{
			kurtosis = double(n) * M4 / (M2 * M2) - 3.0;
			skewness = sqrt(double(n)) * M3 / pow(M2, 1.5);
		}

	}

}
//--------------------
void		CalculateSkewNessAndKurtosis(unsigned short* tab, int size, double& skewness, double& kurtosis)
{
	if ((tab != NULL) && (size > 1))
	{

		skewness = 0;
		kurtosis = 0;

		double n = 0;
		double M1 = 0;
		double M2 = 0;
		double M3 = 0;
		double M4 = 0;
		double x;
		double n1;
		double delta, delta_n, delta_n2, term1;;

		double tol = 1e-6;
		for (int i = 0; i < size; i++)
		{
			x = tab[i];
			n1 = n;
			n = n + 1;
			delta = x - M1;
			delta_n = delta / n;
			delta_n2 = delta_n * delta_n;
			term1 = delta * delta_n * n1;
			M1 += delta_n;
			M4 += term1 * delta_n2 * (n * n - 3 * n + 3) + 6 * delta_n2 * M2 - 4 * delta_n * M3;
			M3 += term1 * delta_n * (n - 2) - 3 * delta_n * M2;
			M2 += term1;
		}
		if (fabs(M2) > tol)
		{
			kurtosis = double(n) * M4 / (M2 * M2) - 3.0;
			skewness = sqrt(double(n)) * M3 / pow(M2, 1.5);
		}

	}

}
//---------------------------------------------
double	GetAngleBetweenLines2D(double* Point0, double* Point1, double* Point2)
{

	double l1[2], l2[2];

	int i;

	for (i = 0; i < 2; i++)
	{
		l1[i] = Point0[i] - Point1[i];
		l2[i] = Point0[i] - Point2[i];
	}

	double len1 = sqrt(l1[0] * l1[0] + l1[1] * l1[1]);
	double len2 = sqrt(l2[0] * l2[0] + l2[1] * l2[1]);

	double scalar = l1[0] * l2[0] + l1[1] * l2[1];

	double m = len1 * len2;

	double tol = 1e-3;
	double v = scalar / m;
	if (v < -1)		 v = -1;
	if (v > 1)		 v = 1;

	if (m != 0)
	{
		return acos(v);

	}
	else
		return 0;

}
//-----------------------------------------------
double	GetAngleBetweenLines2DSigned(double* Point0, double* Point1, double* Point2)
{

	double l1[2], l2[2];

	int i;

	for (int i = 0; i < 2; i++)
	{
		l1[i] = Point0[i] - Point1[i];
		l2[i] = Point0[i] - Point2[i];
	}

	double len1 = sqrt(l1[0] * l1[0] + l1[1] * l1[1]);
	double len2 = sqrt(l2[0] * l2[0] + l2[1] * l2[1]);



	double angle = atan2(l2[1], l2[0]) - atan2(l1[1], l1[0]);
	//if (angle < 0) angle += 2 * M_PI;
	//	return angle;

	/*double angle = atan2(l2[1], l2[0]) - atan2(l1[1], l1[0]);
	if (angle < 0) angle += 2 * M_PI;

	if (angle > M_PI)
	{
	angle -= M_PI;
	angle *= -1;
	}
	*/
	return angle;


}
double	GetAngleBetweenLines2DFull(double* Point0, double* Point1, double* Point2)
{

	double l1[2], l2[2];

	int i;

	for (int i = 0; i < 2; i++)
	{
		l1[i] = Point0[i] - Point1[i];
		l2[i] = Point0[i] - Point2[i];
	}

	double len1 = sqrt(l1[0] * l1[0] + l1[1] * l1[1]);
	double len2 = sqrt(l2[0] * l2[0] + l2[1] * l2[1]);



	double angle = atan2(l2[1], l2[0]) - atan2(l1[1], l1[0]);
	if (angle < 0) angle += 2 * M_PI;
	return angle;


}


//---------------------------------------------------

//---------------------------------------------------
QPolygonF CenterPolyline(QPolygonF* polygon, int nx, int ny, unsigned char* mask, unsigned char badVal, double factor, bool genLines, int dest, bool medianLines, double medianFactor, int kSize, double maxDist)
{

	QPolygonF nPolys;
	int nrPts = polygon->count();
	if (nrPts > 2)
	{
		double* tab = new double[nrPts * 2];
		double* tab1 = new double[nrPts * 2];




		int pos = 0;
		for (int i = 0; i < nrPts; i++)
		{
			tab[pos++] = polygon->at(i).x();
			tab[pos++] = polygon->at(i).y();
		}

		//memcpy(tab1,tab,nrPts*2*sizeof(double));
		int newNrPts = 0;

		double dxf, dyf;
		double x1, y1;
		double xOrig, yOrig;
		double x0, y0;
		int p, pm1, pp1;
		double p0[2], p1[2], p2[2];
		double angle;
		double* tpts;

		double dx1, dy1, dx2, dy2;

		double vprost[2];
		int min = (genLines) ? 0 : 1;
		for (int i = min; i < nrPts - min; i++)
		{


			p = 2 * i;

			pm1 = (i > 0) ? 2 * (i - 1) : 2 * i;
			pp1 = (i < nrPts - 1) ? 2 * (i + 1) : 2 * i;
			x0 = tab[p];
			y0 = tab[p + 1];


			xOrig = tab[p];
			yOrig = tab[p + 1];





			p0[0] = x0;	p0[1] = y0;
			p1[0] = tab[pm1];	p1[1] = tab[pm1 + 1];
			p2[0] = tab[pp1];	p2[1] = tab[pp1 + 1];

			dx1 = p0[0] - p1[0];	dy1 = p0[1] - p1[1];
			dx2 = p2[0] - p0[0];	dy2 = p2[1] - p0[1];

			//prosotpadle


			vprost[1] = (dx1 + dx2) * 0.5;
			vprost[0] = -(dy1 + dy2) * 0.5;

			double len = sqrt(vprost[0] * vprost[0] + vprost[1] * vprost[1]);
			len *= 2;
			if (len > 0)
			{
				vprost[0] /= len;
				vprost[1] /= len;


				//mam wekrot prostopadly teraz trzeba strzelac w tym kierunku i przeciwnym i szukac czegostam;

				int count1 = 0;
				int count2 = 0;
				double upper[2], lower[2];

				upper[0] = p0[0];	lower[0] = p0[0];
				upper[1] = p0[1];	lower[1] = p0[1];
				bool stop = false;
				//w gore;

				do
				{
					upper[0] += vprost[0];
					upper[1] += vprost[1];
					if ((upper[0] < 0) || (upper[0] > nx - 1) || (upper[1] < 0) || (upper[1] > ny - 1))
					{
						stop = true;
					}
					else if (mask[(int)upper[0] + ((int)(upper[1])) * nx] == badVal)
					{
						stop = true;
					}
					count1++;

					if (maxDist > 0)
					{
						double nn[2] = { upper[0] - p0[0],upper[1] - p0[1] };
						double lenX = nn[0] * nn[0] + nn[1] * nn[1];
						if (lenX > 0) lenX = sqrt(lenX);
						if (lenX > maxDist) stop = true;
					}

				} while (stop == false);

				upper[0] -= vprost[0];
				upper[1] -= vprost[1];


				stop = false;
				//w gore;
				do
				{
					lower[0] -= vprost[0];
					lower[1] -= vprost[1];
					if ((lower[0] < 0) || (lower[0] > nx - 1) || (lower[1] < 0) || (lower[1] > ny - 1))
					{
						stop = true;
					}
					else if (mask[(int)lower[0] + ((int)(lower[1])) * nx] == badVal)
					{
						stop = true;
					}
					count2++;




					if (maxDist > 0)
					{
						double nn[2] = { lower[0] - p0[0],lower[1] - p0[1] };
						double lenX = nn[0] * nn[0] + nn[1] * nn[1];
						if (lenX > 0) lenX = sqrt(lenX);
						if (lenX > maxDist) stop = true;
					}

				} while (stop == false);

				lower[0] += vprost[0];
				lower[1] += vprost[1];

				if (genLines == true)
				{
					if (i % dest == 0)
					{
						nPolys.append(QPointF(lower[0], lower[1]));
						nPolys.append(QPointF(upper[0], upper[1]));
						tab1[newNrPts++] = x0;
						tab1[newNrPts++] = y0;
					}
				}
				else
				{
					double center[2];
					center[0] = (lower[0] + upper[0]) * 0.5;
					center[1] = (lower[1] + upper[1]) * 0.5;

					double nPoint[2];
					nPoint[0] = p0[0] + (center[0] - p0[0]) * factor;
					nPoint[1] = p0[1] + (center[1] - p0[1]) * factor;
					nPolys.append(QPointF(nPoint[0], nPoint[1]));

				}

			}




		}








		pos = 0;


		//	for(int i=0;i<nrPts;i++)

		//nPolys->append(QPointF(tab1[i*2],tab1[i*2+1]));

		//bool medianLines,double medianFactor

		if ((genLines == true) && (medianLines == true))
		{
			//wez dlugosci kresek na gorze i na dole - zapisz do tablicy
			//zrob mediane
			//popraw dlugosci
			int i, j;
			newNrPts /= 2;
			double* tabLines = new double[newNrPts * 4];
			double* tabUp = new double[newNrPts];
			double* tabDown = new double[newNrPts];
			double* tmpTab = new double[newNrPts];
			for (i = 0; i < newNrPts; i++)
			{
				tabLines[i * 4] = nPolys.at(i * 2).x();
				tabLines[i * 4 + 1] = nPolys.at(i * 2).y();

				tabLines[i * 4 + 2] = nPolys.at(i * 2 + 1).x();
				tabLines[i * 4 + 3] = nPolys.at(i * 2 + 1).y();
			}


			double len1, len2;
			double p0[2], p1[2], p2[2];
			for (i = 0; i < newNrPts; i++)
			{
				//center
				p0[0] = tab1[i * 2];
				p0[1] = tab1[i * 2 + 1];

				//up
				p1[0] = tabLines[i * 4];
				p1[1] = tabLines[i * 4 + 1];


				//down
				p2[0] = tabLines[i * 4 + 2];
				p2[1] = tabLines[i * 4 + 3];


				len1 = GetDistance2D(p0, p1);
				len2 = GetDistance2D(p0, p2);

				tabUp[i] = len1;
				tabDown[i] = len2;

			}


			//filtering
			double med;
			//	int kSize=2;
			double* medTab = new double[kSize * 2 + 1];
			int count;
			int pos;

			//UP
			for (i = 0; i < newNrPts; i++)
			{
				count = 0;
				//zbieram do mediany
				for (j = -kSize; j <= kSize; j++)
				{
					pos = i + j;
					if ((pos >= 0) && (pos < newNrPts))
					{
						medTab[count++] = tabUp[pos];
					}
				}

				//filtruje
				tmpTab[i] = Median(medTab, count - 1);
			}
			//blend

			for (i = 0; i < newNrPts; i++)
			{

				tabUp[i] = medianFactor * tmpTab[i] + (1.0 - medianFactor) * tabUp[i];
			}

			//DOWN
			for (i = 0; i < newNrPts; i++)
			{
				count = 0;
				//zbieram do mediany
				for (j = -kSize; j <= kSize; j++)
				{
					pos = i + j;
					if ((pos >= 0) && (pos < newNrPts))
					{
						medTab[count++] = tabDown[pos];
					}
				}

				//filtruje
				tmpTab[i] = Median(medTab, count - 1);
			}
			//blend

			for (i = 0; i < newNrPts; i++)
			{

				tabDown[i] = medianFactor * tmpTab[i] + (1.0 - medianFactor) * tabDown[i];
			}

			// replace original Values

			double v1[2], v2[2];
			for (i = 0; i < newNrPts; i++)
			{
				//center
				p0[0] = tab1[i * 2];
				p0[1] = tab1[i * 2 + 1];

				//up
				p1[0] = tabLines[i * 4];
				p1[1] = tabLines[i * 4 + 1];


				//down
				p2[0] = tabLines[i * 4 + 2];
				p2[1] = tabLines[i * 4 + 3];


				len1 = GetDistance2D(p0, p1);
				len2 = GetDistance2D(p0, p2);

				v1[0] = p1[0] - p0[0];
				v1[1] = p1[1] - p0[1];

				if (len1 > 0)
				{
					//norm
					v1[0] /= len1;
					v1[1] /= len1;
					// new len
					v1[0] *= tabUp[i];
					v1[1] *= tabUp[i];
					//new pos
					p1[0] = p0[0] + v1[0];
					p1[1] = p0[1] + v1[1];
				}
				//replace
				tabLines[i * 4] = p1[0];
				tabLines[i * 4 + 1] = p1[1];

				v2[0] = p2[0] - p0[0];
				v2[1] = p2[1] - p0[1];

				if (len2 > 0)
				{
					//norm
					v2[0] /= len2;
					v2[1] /= len2;
					// new len
					v2[0] *= tabDown[i];
					v2[1] *= tabDown[i];
					//new pos
					p2[0] = p0[0] + v2[0];
					p2[1] = p0[1] + v2[1];
				}
				//replace
				tabLines[i * 4 + 2] = p2[0];
				tabLines[i * 4 + 3] = p2[1];


			}

			//replace originalPts
			QPolygonF newPoly;
			for (i = 0; i < newNrPts; i++)
			{
				newPoly.append(QPointF(tabLines[i * 4], tabLines[i * 4 + 1]));
				newPoly.append(QPointF(tabLines[i * 4 + 2], tabLines[i * 4 + 3]));

			}
			nPolys = newPoly;

			delete[]medTab;
			delete[]tmpTab;
			delete[]tabLines;
			delete[]tabUp;
			delete[]tabDown;
		}

		delete[]tab1; delete[]tab;
	}

	return nPolys;
}


//----------------------
QPolygonF SmoothPolyLine(QPolygonF* polygon, int iter, double factor, double dx, double dy)
{
	QPolygonF nPolys;
	int nrPts = polygon->count();
	if (nrPts > 2)
	{
		double* tab = new double[nrPts * 2];
		double* tab1 = new double[nrPts * 2];
		double* tab2 = new double[nrPts * 2];



		int pos = 0;
		for (int i = 0; i < nrPts; i++)
		{
			tab[pos++] = polygon->at(i).x();
			tab[pos++] = polygon->at(i).y();
		}

		memcpy(tab1, tab, nrPts * 2 * sizeof(double));
		memcpy(tab2, tab, nrPts * 2 * sizeof(double));

		double dxf, dyf;
		double x1, y1;
		double xOrig, yOrig;
		double x0, y0;
		int p, pm1, pp1;
		double p0[2], p1[2], p2[2];
		double angle;
		double* tpts;
		double count = factor;
		for (int j = 0; j < iter; j++)
		{

			for (int i = 1; i < nrPts - 1; i++)
			{

				p = 2 * i;
				pm1 = 2 * (i - 1);
				pp1 = 2 * (i + 1);
				x0 = tab1[p];
				y0 = tab1[p + 1];


				xOrig = tab[p];
				yOrig = tab[p + 1];





				p0[0] = x0;	p0[1] = y0;
				p1[0] = tab1[pm1];	p1[1] = tab1[pm1 + 1];
				p2[0] = tab1[pp1];	p2[1] = tab1[pp1 + 1];

				angle = GetAngleBetweenLines2D(p0, p1, p2) * to_deg;
				if (angle > 120)
				{
					x1 = (p1[0] + p2[0] + count * p0[0]) / (2.0 + count);
					y1 = (p1[1] + p2[1] + count * p0[1]) / (2.0 + count);


				}
				else
				{
					//zmieniam jak za duzy kat i sie wogole nie ogladam
					x1 = (p1[0] + p2[0]) / (2.0);
					y1 = (p1[1] + p2[1]) / (2.0);


					tab[p] = x1;
					tab[p + 1] = y1;

				}


				dxf = fabs(x1 - xOrig);
				dyf = fabs(y1 - yOrig);


				if ((dxf < dx) && (dyf < dy))
				{
					tab2[p] = x1;
					tab2[p + 1] = y1;

				}
			}

			tpts = tab1;
			tab1 = tab2;
			tab2 = tpts;
			tpts = 0;


		}




		pos = 0;


		for (int i = 0; i < nrPts; i++)

			nPolys.append(QPointF(tab2[i * 2], tab2[i * 2 + 1]));



		delete[]tab1; delete[]tab2; delete[]tab;
	}

	return nPolys;


}

//------------------------------
QString RelativePath(QString absolutePath, QString relativeTo, bool bIsFile /*= false*/)
{


	QStringList absoluteDirectories = absolutePath.split('/', QString::SkipEmptyParts);
	//absoluteDirectories.split('/',absolutePath);

	QStringList relativeDirectories = relativeTo.split('/', QString::SkipEmptyParts);


	//QStringList absoluteDirectories = absolutePath.split( '/');//, QString::SkipEmptyParts );
	//QStringList relativeDirectories = relativeTo.split( '/');//, QString::SkipEmptyParts );

	int len1 = absoluteDirectories.count();
	int len2 = relativeDirectories.count();
	//Get the shortest of the two paths
	int length = absoluteDirectories.count() < relativeDirectories.count() ? absoluteDirectories.count() : relativeDirectories.count();

	//Use to determine where in the loop we exited
	int lastCommonRoot = -1;
	int index;

	//Find common root
	for (index = 0; index < length; index++)
		if (absoluteDirectories[index] == relativeDirectories[index])
			lastCommonRoot = index;
		else
			break;

	//If we didn't find a common prefix then throw
	if (lastCommonRoot == -1)
		throw QString("Paths do not have a common base");

	//Build up the relative path
	QString relativePath;

	//Add on the ..
	for (index = lastCommonRoot + 1; index < absoluteDirectories.count() - (bIsFile ? 1 : 0); index++)
		if (absoluteDirectories[index].length() > 0)
			relativePath.append("../");

	//Add on the folders
	for (index = lastCommonRoot + 1; index < relativeDirectories.count() - 1; index++)
		relativePath.append(relativeDirectories[index]).append("/");
	relativePath.append(relativeDirectories[relativeDirectories.count() - 1]);

	return relativePath;
}
//-----------------------------
double GetDistance(int p1[3], int p2[3])
{
	double d[3];
	d[0] = p1[0] - p2[0];
	d[1] = p1[1] - p2[1];
	d[2] = p1[2] - p2[2];

	double dist = (d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
	if (dist > 0) dist = sqrt(dist);
	return dist;

}

//--------------------------------------
double GetDistance(double p1[3], double p2[3])
{
	double d[3];
	d[0] = p1[0] - p2[0];
	d[1] = p1[1] - p2[1];
	d[2] = p1[2] - p2[2];

	double dist = (d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
	if (dist > 0) dist = sqrt(dist);
	return dist;

}
//---------------------------

double GetDistance2D(double p1[2], double p2[2])
{
	double d[2];
	d[0] = p1[0] - p2[0];
	d[1] = p1[1] - p2[1];

	double dist = (d[0] * d[0] + d[1] * d[1]);
	if (dist > 0) dist = sqrt(dist);
	return dist;

}
//---------------


//---------------
void GetLinearInterpolatedPoint(double* p, double* p1, double* p2, double alpha)
{

	p[0] = p2[0] * alpha + p1[0] * (1 - alpha);
	p[1] = p2[1] * alpha + p1[1] * (1 - alpha);
	p[2] = p2[2] * alpha + p1[2] * (1 - alpha);

}
//-----------


//---------------------------------------
int* GetGrowingKernel(int& kernelSize, int size, RawDataSet* data, int type)
{

	int i, j, k;

	if (type > 1) type = 0;
	if (type == 0)// szescian
	{
		kernelSize = (size * 2 + 1) * (size * 2 + 1) * (size * 2 + 1) - 1;

		int* kernel = new int[kernelSize];
		int pos = 0;

		for (k = -size; k < size + 1; k++)
		{

			for (j = -size; j < size + 1; j++)
			{
				for (i = -size; i < size + 1; i++)
				{
					if (!((i == 0) && (j == 0) && (k == 0)))
					{
						kernel[pos++] = data->GetVoxelPos(i, j, k);
					}

				}
			}
		}

		return kernel;

	}
	else if (type == 1) //kolko
	{
		kernelSize = (size * 2 + 1) * (size * 2 + 1) * (size * 2 + 1) - 1;

		int* kernel = new int[kernelSize];
		int pos = 0;

		for (k = -size; k < size + 1; k++)
		{

			for (j = -size; j < size + 1; j++)
			{
				for (i = -size; i < size + 1; i++)
				{
					if (!((i == 0) && (j == 0) && (k == 0)))
					{
						if ((i * i + j * j + k * k) < size * size + 1e-6)
						{
							kernel[pos++] = data->GetVoxelPos(i, j, k);
						}
					}

				}
			}
		}
		if (pos < kernelSize)
		{
			int* kernel2 = new int[pos];
			memcpy(kernel2, kernel, pos * sizeof(int));
			kernelSize = pos;
			return kernel2;

		}

		return kernel;

	}

	return NULL;


}

//-----------------------------------------------------------------
//-------------------Optimized				-----------------------
//-----------------------------------------------------------------

RawDataSet* PaddingBySize(int Size, RawDataSet* Input, int Value)
{

	RawDataSet* PaddedDataSet = new RawDataSet(Input->GetNx() + Size * 2,
		Input->GetNy() + Size * 2,
		Input->GetNz() + Size * 2,
		Input->GetXscale(),
		Input->GetYscale(),
		Input->GetZscale());

	int INx = Input->GetNx();
	int	INy = Input->GetNy();
	int	INz = Input->GetNz();
	unsigned short* IQ = Input->GetDataArray();

	int PNx = PaddedDataSet->GetNx();
	int	PNy = PaddedDataSet->GetNy();
	int	PNz = PaddedDataSet->GetNz();
	unsigned short* PQ = PaddedDataSet->GetDataArray();

	Padding <unsigned short>(IQ, PQ, INx, INy, INz, PNx, PNy, PNz, Size, Value);

	return PaddedDataSet;
}
//---------------------------------------------------------

unsigned char* PaddingBySizeUCHAR(int Size, RawDataSet* Input, unsigned char* Mask, int Value)
{

	RawDataSet* PaddedDataSet = new RawDataSet(Input->GetNx() + Size * 2,
		Input->GetNy() + Size * 2,
		Input->GetNz() + Size * 2,
		Input->GetXscale(),
		Input->GetYscale(),
		Input->GetZscale(), 0, false);

	int INx = Input->GetNx();
	int	INy = Input->GetNy();
	int	INz = Input->GetNz();


	int PNx = PaddedDataSet->GetNx();
	int	PNy = PaddedDataSet->GetNy();
	int	PNz = PaddedDataSet->GetNz();
	unsigned char* PQ = new unsigned char[PaddedDataSet->GetVolumeSize()];


	Padding <unsigned char>(Mask, PQ, INx, INy, INz, PNx, PNy, PNz, Size, Value);

	delete PaddedDataSet;
	return PQ;
}
//---------------------------------------------------------



template <class TYPE>
void 	Padding(TYPE* IQ, TYPE* PQ, int INx, int INy, int INz, int PNx, int PNy, int PNz, int Size, int Value)
{

	if (Value > -1) memset(PQ, Value, PNx * PNy * PNz * sizeof(TYPE));

	int	PSliceSize = PNx * PNy;

	int i, j, k;
	int pos1 = Size * PSliceSize;
	int pos2 = 0;
	int	SizeOfNx = INx * sizeof(TYPE);
	int t1 = Size * PNx;
	int t2 = INx + Size;

	for (k = 0; k < INz; k++)
	{
		pos1 += t1;
		for (j = 0; j < INy; j++)
		{
			pos1 += Size;
			memcpy(PQ + pos1, IQ + pos2, SizeOfNx);
			pos1 += t2;
			pos2 += INx;
		}
		pos1 += t1;

	}
	if (Value == -1)
	{
		///////////////////////////////////////////////
		//			- boczki ok!!!!!!!!!!!!!!!!!!!
		/////////////////////////////////////////////////////

		//uzupelnianie
		//boczki

		TYPE* TempQ1 = IQ;
		TYPE* TempQ2 = PQ;
		int t3 = INx - 1;

		int t4 = PNx * Size;

		TempQ2 += t4 + t4 * PNy - 1;//pierwszy krok 
		t4 *= 2;

		int TempVal1;//,TempVal2;
		for (k = 0; k < INz; k++)
		{
			for (i = 0; i < INy; i++)
			{

				TempVal1 = *(TempQ1);

				for (j = 0; j < Size; j++)
					*(++TempQ2) = TempVal1;

				TempQ1 += t3;
				TempVal1 = *(TempQ1++);

				TempQ2 += INx;
				for (j = 0; j < Size; j++)
					*(++TempQ2) = TempVal1;

			}
			TempQ2 += t4;
		}



		///////////////////////////////////////////////
		//			- przod i tyl ok!!!!!!!!!!!!!!!!!!!
		/////////////////////////////////////////////////////


		//przod i tyl
		t4 = PNx * Size;

		TempQ1 = PQ + t4 + t4 * PNy;//skad
		TempQ2 = PQ + t4 * PNy;//gdzie

		t3 = PNx * sizeof(TYPE);
		t2 = PNx * (INy);
		t1 = PNx * (INy - 1);

		for (k = 0; k < INz; k++)
		{

			for (j = 0; j < Size; j++)
			{
				memcpy(TempQ2, TempQ1, t3);
				TempQ2 += PNx;
			}

			TempQ2 += t2;
			TempQ1 += t1;

			for (j = 0; j < Size; j++)
			{
				memcpy(TempQ2, TempQ1, t3);
				TempQ2 += PNx;
			}

			TempQ1 += (Size * 2 + 1) * PNx;


		}
		///////////////////////////////////////////////
		//			- czapeczki z gory i z dolu ok!!!!!!!!!!!!!!!!!!!
		/////////////////////////////////////////////////////

		//czapeczki z gory i z dolu...

		int sizeoftype = PSliceSize * sizeof(TYPE);
		for (i = 0; i < Size; i++)
		{
			memcpy(PQ + i * PSliceSize,
				PQ + Size * PSliceSize,
				sizeoftype);
		}

		for (i = 0; i < Size; i++)
		{
			memcpy(PQ + (PNz - i - 1) * PSliceSize,
				PQ + (PNz - Size - 1) * PSliceSize,
				sizeoftype);

		}
	}

}
//--------------------------------

template <class TYPE>
void 	Padding2D(TYPE* IQ, TYPE* PQ, int INx, int INy, int PNx, int PNy, int Size, int Value)
{

	if (Value > -1) memset(PQ, Value, PNx * PNy * sizeof(TYPE));

	//	int	PSliceSize=PNx*PNy;

	int i, j;
	int pos1 = 0;
	int pos2 = 0;
	int	SizeOfNx = INx * sizeof(TYPE);
	int t1 = Size * PNx;
	int t2 = INx + Size;


	pos1 += t1;
	for (j = 0; j < INy; j++)
	{
		pos1 += Size;
		memcpy(PQ + pos1, IQ + pos2, SizeOfNx);
		pos1 += t2;
		pos2 += INx;
	}





	if (Value == -1)
	{
		///////////////////////////////////////////////
		//			- boczki ok!!!!!!!!!!!!!!!!!!!
		/////////////////////////////////////////////////////

		//uzupelnianie
		//boczki

		TYPE* TempQ1 = IQ;
		TYPE* TempQ2 = PQ;
		int t3 = INx - 1;

		int t4 = PNx * Size;

		TempQ2 += t4 - 1;//pierwszy krok 
		t4 *= 2;

		int TempVal1;//,TempVal2;

		for (i = 0; i < INy; i++)
		{

			TempVal1 = *(TempQ1);

			for (j = 0; j < Size; j++)
				*(++TempQ2) = *(TempQ1 + (Size - j) % INx);
			//*(++TempQ2)=TempVal1;

			TempQ1 += t3;
			TempVal1 = *(TempQ1++);

			TempQ2 += INx;
			for (j = 0; j < Size; j++)
				*(++TempQ2) = *(TempQ1 - (j + 1) % INx);
			//*(++TempQ2)=TempVal1;

		}

		///////////////////////////////////////////////
		//			- czapeczki z gory i z dolu ok!!!!!!!!!!!!!!!!!!!
		/////////////////////////////////////////////////////

		//czapeczki z gory i z dolu...

		for (i = 0; i < Size; i++)
		{
			memcpy(PQ + (Size - i - 1) * PNx,
				PQ + (Size + i % INy) * PNx,
				PNx * sizeof(TYPE));

			memcpy(PQ + (i + INy + Size) * PNx,
				PQ + (Size + INy - 1 - i % INy) * PNx,
				PNx * sizeof(TYPE));

		}
	}
}


//-------------------------------------
unsigned char* Padding2D_Unsigned_Char(unsigned char* IQ, int INx, int INy, int Size, int Value)
{
	int PNx = INx + 2 * Size;
	int PNy = INy + 2 * Size;
	unsigned char* PQ = new unsigned char[PNx * PNy];
	Padding2D <unsigned char>(IQ, PQ, INx, INy, PNx, PNy, Size, Value);
	return PQ;

}
unsigned short* Padding2D_Unsigned_Short(unsigned short* IQ, int INx, int INy, int Size, int Value)
{
	int PNx = INx + 2 * Size;
	int PNy = INy + 2 * Size;
	unsigned short* PQ = new unsigned short[PNx * PNy];
	Padding2D <unsigned short>(IQ, PQ, INx, INy, PNx, PNy, Size, Value);
	return PQ;
}

unsigned int* Padding2D_Unsigned_Int(unsigned int* IQ, int INx, int INy, int Size, int Value)
{
	int PNx = INx + 2 * Size;
	int PNy = INy + 2 * Size;
	unsigned int* PQ = new unsigned int[PNx * PNy];
	Padding2D <unsigned int>(IQ, PQ, INx, INy, PNx, PNy, Size, Value);
	return PQ;
}
//------------------------------------------------------------




//------------------------------------------------
double	ArithmeticAverage(unsigned short* A, int n)
{
	int i;

	double val = 0;

	for (i = 0; i < n; i++)
		val += (double)A[i];

	return (val / (double)n);


}


//------------------------------------------------
unsigned char  Median(unsigned char* A, int n)
{
	int  i, j, l, r, k;
	unsigned char V, T;

	k = n / 2;
	l = 1;   r = n - 1;

	while (r > l)
	{
		V = A[r];   i = l - 1;   j = r;
		do
		{
			do { i++; } while (A[i] < V);
			do { j--; } while ((j > -1) && A[j] > V);
			T = A[i];  A[i] = A[j];  A[j] = T;
		} while (j > i);


		A[j] = A[i];  A[i] = A[r];  A[r] = T;
		if (i >= k)  r = i - 1;
		if (i <= k)  l = i + 1;

	}
	return(A[k]);
}
//------------------------------------------------
unsigned short  Median(unsigned short* A, int n)
{
	int  i, j, l, r, k;
	unsigned short V, T;

	k = n / 2;
	l = 1;   r = n - 1;

	while (r > l)
	{
		V = A[r];   i = l - 1;   j = r;
		do
		{
			do { i++; } while (A[i] < V);
			do { j--; } while ((j > -1) && A[j] > V);
			T = A[i];  A[i] = A[j];  A[j] = T;
		} while (j > i);


		A[j] = A[i];  A[i] = A[r];  A[r] = T;
		if (i >= k)  r = i - 1;
		if (i <= k)  l = i + 1;

	}
	return(A[k]);
}

//------------------------------------------------

int  Median(int* A, int n)
{
	int  i, j, l, r, k;
	int V, T;

	k = n / 2;
	l = 0;   r = n - 1;

	while (r > l)
	{
		V = A[r];   i = l - 1;   j = r;
		do
		{
			do { i++; } while (A[i] < V);
			do { j--; } while ((j > -1) && A[j] > V);
			T = A[i];  A[i] = A[j];  A[j] = T;
		} while (j > i);


		A[j] = A[i];  A[i] = A[r];  A[r] = T;
		if (i >= k)  r = i - 1;
		if (i <= k)  l = i + 1;

	}
	return(A[k]);
}
//---------------------------------------
double  Median(double* A, int n)
{
	int  i, j, l, r, k;
	double V, T;

	k = n / 2;
	l = 0;   r = n - 1;

	while (r > l)
	{
		V = A[r];   i = l - 1;   j = r;
		do
		{
			do { i++; } while (A[i] < V);
			do { j--; } while ((j > -1) && A[j] > V);
			T = A[i];  A[i] = A[j];  A[j] = T;
		} while (j > i);


		A[j] = A[i];  A[i] = A[r];  A[r] = T;
		if (i >= k)  r = i - 1;
		if (i <= k)  l = i + 1;

	}
	return(A[k]);
}

//------------------------------------------------

template <class TYPE>
double	 Interpolation3Linear(TYPE* dataX, int Nx, int Ny, int Nz, double x, double y, double z)
{

	
	double tmp[3] = { x,y,z };

	int				x0, y0, z0, p;

	TYPE* Q, d0, d1, d2, d3, d4, d5, d6, d7;
	double			d, d8, d9, d10, d11, d12, d13;

	Q = dataX;

	x0 = (int)x;
	y0 = (int)y;
	z0 = (int)z;

	x -= (double)x0;
	y -= (double)y0;
	z -= (double)z0;

	int slicesize = Nx * Ny;

	p = z0 * slicesize + y0 * Nx + x0;
	Q += p;
	d0 = *(Q);


	d1 = *(Q + 1);
	d4 = *(Q + Nx);
	d5 = *(Q + Nx + 1);

	Q += slicesize;

	d2 = *(Q);
	d3 = *(Q + 1);
	d6 = *(Q + Nx);
	d7 = *(Q + Nx + 1);

	d8 = (double)d0 + z * (double)(d2 - d0);
	d9 = (double)d1 + z * (double)(d3 - d1);
	d10 = (double)d4 + z * (double)(d6 - d4);
	d11 = (double)d5 + z * (double)(d7 - d5);
	d12 = d8 + x * (d9 - d8);
	d13 = d10 + x * (d11 - d10);

	d = d12 + y * (d13 - d12);

	return d;


}

//------------------------------------------------------

double	 Interpolation3LinearUnsignedShort(unsigned short* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z)
{
	return Interpolation3Linear <unsigned short>(dataX, Nx, Ny, Nz, X, Y, Z);

}

//------------------------------------------------------

double	 Interpolation3LinearUnsignedChar(unsigned char* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z)
{
	return Interpolation3Linear <unsigned char>(dataX, Nx, Ny, Nz, X, Y, Z);

}

/*



unsigned char	*Padding2D_Unsigned_Char( unsigned char *IQ, int INx, int INy, int Size,int Value)
{
int PNx=INx+2*Size;
int PNy=INy+2*Size;
unsigned char *PQ=new unsigned char[PNx*PNy];
Padding2D <unsigned char> ( IQ,PQ, INx, INy, PNx,PNy,Size,Value);
return PQ;

}


*/
//-------------------------------------------------


unsigned short  Interpolation3Linear(RawDataSet* dataX, double x, double y, double z)
{

	/*
	//	if (z>=54)
	//QMessageBox::about( 0,"DICOM","JEscze nie zrobilem :(");
	int				x0,y0,z0,p;

	unsigned short	*Q,d0,d1,d2,d3,d4,d5,d6,d7;
	double			d,d8,d9,d10,d11,d12,d13;

	Q=dataX->GetDataArray();

	x0=(int)x;
	y0=(int)y;
	z0=(int)z;

	x-=(double)x0;
	y-=(double)y0;
	z-=(double)z0;

	p=z0*dataX->GetSliceSize()+y0*dataX->GetNx()+x0;
	Q+=p;
	d0=*(Q);


	d1=*(Q+1);
	d4=*(Q+dataX->GetNx());
	d5=*(Q+dataX->GetNx()+1);

	Q+=dataX->GetSliceSize();

	d2=*(Q);
	d3=*(Q+1);
	d6=*(Q+dataX->GetNx());
	d7=*(Q+dataX->GetNx()+1);

	d8=(double)d0+z*(double)(d2-d0);
	d9=(double)d1+z*(double)(d3-d1);
	d10=(double)d4+z*(double)(d6-d4);
	d11=(double)d5+z*(double)(d7-d5);
	d12=d8+x*(d9-d8);
	d13=d10+x*(d11-d10);

	d=d12+y*(d13-d12);
	*/
	return ((unsigned short)Round(Interpolation3LinearDouble(dataX, x, y, z)));

}

//------------------------------------------------
double	Interpolation3LinearDouble(unsigned short* Q, int nx, int ss, double x, double y, double z)
{
	int				x0, y0, z0, p;

	unsigned short	d0, d1, d2, d3, d4, d5, d6, d7;
	double			d, d8, d9, d10, d11, d12, d13;


	//Q=dataX->GetDataArray();

	x0 = (int)x;
	y0 = (int)y;
	z0 = (int)z;

	x -= (double)x0;
	y -= (double)y0;
	z -= (double)z0;

	/*p=z0*dataX->GetSliceSize()+y0*dataX->GetNx()+x0;
	Q+=p;
	d0=*(Q);


	d1=*(Q+1);
	d4=*(Q+dataX->GetNx());
	d5=*(Q+dataX->GetNx()+1);

	Q+=dataX->GetSliceSize();

	d2=*(Q);
	d3=*(Q+1);
	d6=*(Q+dataX->GetNx());
	d7=*(Q+dataX->GetNx()+1);*/

	int pos;
	pos = z0 * ss + y0 * nx + x0;
	d0 = Q[pos];
	d1 = Q[pos + 1];
	d4 = Q[pos + nx];
	d5 = Q[pos + nx + 1];
	pos += ss;
	d2 = Q[pos];
	d3 = Q[pos + 1];
	d6 = Q[pos + nx];
	d7 = Q[pos + nx + 1];

	d8 = (double)d0 + z * (double)(d2 - d0);
	d9 = (double)d1 + z * (double)(d3 - d1);
	d10 = (double)d4 + z * (double)(d6 - d4);
	d11 = (double)d5 + z * (double)(d7 - d5);
	d12 = d8 + x * (d9 - d8);
	d13 = d10 + x * (d11 - d10);

	d = d12 + y * (d13 - d12);

	return d;
}

double   Interpolation3LinearDouble(RawDataSet* dataX, double x, double y, double z)
{
	return Interpolation3LinearDouble(dataX->GetDataArray(), dataX->GetNx(), dataX->GetSliceSize(), x, y, z);



}
//--------------------------------------------------------------------
//---------------------------------------------------
//-------------234444444444444444444444444444444


template <class TYPE>
double	 Interpolation3LinearSafe(TYPE* dataX, int Nx, int Ny, int Nz, double x, double y, double z)
{

	//if ((x<0)||(y<0)||(z<0)||(x>=Nx-1)||(y>=Ny-1)||(z>=Nz-1)) return 0;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (z < 0) z = 0;
	if (x >= Nx - 1) x = Nx - 1.001;
	if (y >= Ny - 1) y = Ny - 1.001;
	if (z >= Nz - 1) z = Nz - 1.001;

	int				x0, y0, z0, p;

	TYPE* Q, d0, d1, d2, d3, d4, d5, d6, d7;
	double			d, d8, d9, d10, d11, d12, d13;

	Q = dataX;

	x0 = (int)x;
	y0 = (int)y;
	z0 = (int)z;

	x -= (double)x0;
	y -= (double)y0;
	z -= (double)z0;

	int slicesize = Nx * Ny;

	p = z0 * slicesize + y0 * Nx + x0;
	Q += p;
	d0 = *(Q);


	d1 = *(Q + 1);
	d4 = *(Q + Nx);
	d5 = *(Q + Nx + 1);

	Q += slicesize;

	d2 = *(Q);
	d3 = *(Q + 1);
	d6 = *(Q + Nx);
	d7 = *(Q + Nx + 1);

	d8 = (double)d0 + z * (double)(d2 - d0);
	d9 = (double)d1 + z * (double)(d3 - d1);
	d10 = (double)d4 + z * (double)(d6 - d4);
	d11 = (double)d5 + z * (double)(d7 - d5);
	d12 = d8 + x * (d9 - d8);
	d13 = d10 + x * (d11 - d10);

	d = d12 + y * (d13 - d12);

	return d;


}

//------------------------------------------------------

double	 Interpolation3LinearUnsignedShortSafe(unsigned short* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z)
{
	return Interpolation3LinearSafe <unsigned short>(dataX, Nx, Ny, Nz, X, Y, Z);

}

//------------------------------------------------------

double	 Interpolation3LinearUnsignedCharSafe(unsigned char* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z)
{
	return Interpolation3LinearSafe <unsigned char>(dataX, Nx, Ny, Nz, X, Y, Z);

}


//-------------------------------------------------

unsigned short  Interpolation3LinearSafe(RawDataSet* dataX, double x, double y, double z)
{


	return ((unsigned short)Round(Interpolation3LinearDoubleSafe(dataX, x, y, z)));

}

//------------------------------------------------


double   Interpolation3LinearDoubleSafe(RawDataSet* dataX, double x, double y, double z)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (z < 0) z = 0;
	int Nx = dataX->GetNx();
	int Ny = dataX->GetNy();
	int Nz = dataX->GetNz();
	if (x >= Nx - 1) { x = Nx - 1.001;if (x < 0) x = 0; };
	if (y >= Ny - 1) { y = Ny - 1.001;if (y < 0) y = 0; };
	if (z >= Nz - 1) { z = Nz - 1.001;if (z < 0) z = 0; };
	double d = 0;
	unsigned short* Q;
	Q = dataX->GetDataArray();
	if (Nz > 1)
	{

		int				x0, y0, z0, p;

		unsigned short  d0, d1, d2, d3, d4, d5, d6, d7;
		double			 d8, d9, d10, d11, d12, d13;

		

		x0 = (int)x;
		y0 = (int)y;
		z0 = (int)z;

		x -= (double)x0;
		y -= (double)y0;
		z -= (double)z0;



		p = z0 * dataX->GetSliceSize() + y0 * dataX->GetNx() + x0;
		Q += p;
		d0 = *(Q);


		d1 = *(Q + 1);
		d4 = *(Q + dataX->GetNx());
		d5 = *(Q + dataX->GetNx() + 1);

		Q += dataX->GetSliceSize();

		d2 = *(Q);
		d3 = *(Q + 1);
		d6 = *(Q + dataX->GetNx());
		d7 = *(Q + dataX->GetNx() + 1);

		d8 = (double)d0 + z * (double)(d2 - d0);
		d9 = (double)d1 + z * (double)(d3 - d1);
		d10 = (double)d4 + z * (double)(d6 - d4);
		d11 = (double)d5 + z * (double)(d7 - d5);
		d12 = d8 + x * (d9 - d8);
		d13 = d10 + x * (d11 - d10);

		d = d12 + y * (d13 - d12);
	}
	else
	{
		int ss = dataX->GetSliceSize();
		int xx, yy, x11, x12, x21, x22;
		double y1, y2;
		double dx, dy;
		xx = (int)x;
		yy = (int)y;
		dx = x - xx;
		dy = y - yy;
		//int nx=nx;
		int pos = xx + yy * Nx ;

		//unsigned short *tab=data->GetDataArray();

		x11 = Q[pos];
		x12 = Q[pos + 1];
		x21 = Q[pos + Nx];
		x22 = Q[pos + Nx + 1];

		y1 = x11 * (1.0 - dx) + x12 * dx;
		y2 = x21 * (1.0 - dx) + x22 * dx;
		d=	y1 * (1.0 - dy) + y2 * dy;
	}
	return d;

}

//--------------------------------------------------------------------
//---------------------------------------------------
//-------------234444444444444444444444444444444



//--------------------------------------------
void	QSortUnsignedShort(unsigned short* A, int Lo, int Hi)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	unsigned short V = A[k];
	i = Lo;   j = Hi;
	unsigned short T;
	do
	{
		while (A[i] < V) i++;
		while (A[j] > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortUnsignedShort(A, Lo, j);
	if (i < Hi) QSortUnsignedShort(A, i, Hi);
}
//------------
void	QSortInt(int* A, int Lo, int Hi)
{

	int   i, j;

	int V = A[(Lo + Hi) / 2];
	int T;
	i = Lo;   j = Hi;
	do
	{

		while (A[i] < V) i++;
		while (A[j] > V) j--;
		if (i <= j)
		{

			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);



	if (Lo < j) QSortInt(A, Lo, j);
	if (i < Hi) QSortInt(A, i, Hi);

}

//-----------------------------------------------------	
void	QSortFloat(double* A, int Lo, int Hi)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	double V = A[k];
	i = Lo;   j = Hi;
	double T;
	do
	{
		while (A[i] < V) i++;
		while (A[j] > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortFloat(A, Lo, j);
	if (i < Hi) QSortFloat(A, i, Hi);

}
//-------------------------------------
void	QSortDouble(double* A, int Lo, int Hi)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	double V = A[k];
	i = Lo;   j = Hi;
	double T;
	do
	{
		while (A[i] < V) i++;
		while (A[j] > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortDouble(A, Lo, j);
	if (i < Hi) QSortDouble(A, i, Hi);

}

//---------------------------------------

void	QSortIntDouble(IntDoubleStruct** A, int Lo, int Hi)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	double V;
	IntDoubleStruct* T = A[k];
	V = T->DoubleVar;
	i = Lo;   j = Hi;
	do
	{
		while (A[i]->DoubleVar < V) i++;
		while (A[j]->DoubleVar > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortIntDouble(A, Lo, j);
	if (i < Hi) QSortIntDouble(A, i, Hi);

}


//---------------------------------------

void	QSortInt1Int(IntIntStruct** A, int Lo, int Hi)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	int V;//26.08.2004
	IntIntStruct* T = A[k];
	V = T->Int1Var;
	i = Lo;   j = Hi;
	do
	{
		while (A[i]->Int1Var < V) i++;
		while (A[j]->Int1Var > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortInt1Int(A, Lo, j);
	if (i < Hi) QSortInt1Int(A, i, Hi);

}

void	QSortInt1IntInt(IntIntIntStruct** A, int Lo, int Hi)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	int V;//26.08.2004
	IntIntIntStruct* T = A[k];
	V = T->Int1Var;
	i = Lo;   j = Hi;
	do
	{
		while (A[i]->Int1Var < V) i++;
		while (A[j]->Int1Var > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortInt1IntInt(A, Lo, j);
	if (i < Hi) QSortInt1IntInt(A, i, Hi);

}

void	QSortDouble1DoubleDouble(DoubleDoubleDoubleStruct** A, int Lo, int Hi)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	double V;//26.08.2004
	DoubleDoubleDoubleStruct* T = A[k];
	V = T->Double1Var;
	i = Lo;   j = Hi;
	do
	{
		while (A[i]->Double1Var < V) i++;
		while (A[j]->Double1Var > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortDouble1DoubleDouble(A, Lo, j);
	if (i < Hi) QSortDouble1DoubleDouble(A, i, Hi);

}

void	QSortInt3(int** A, int Lo, int Hi, int nr)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	int V;//26.08.2004
	int* T = A[k];
	V = T[nr];
	i = Lo;   j = Hi;
	do
	{
		while (A[i][nr] < V) i++;
		while (A[j][nr] > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortInt3(A, Lo, j, nr);
	if (i < Hi) QSortInt3(A, i, Hi, nr);

}

//---------------------------------------

void	QSortIntInt1(IntIntStruct** A, int Lo, int Hi)
{
	int   i, j, k;
	k = (Lo + Hi) / 2;
	int V;
	IntIntStruct* T = A[k];
	V = T->Int2Var;
	i = Lo;   j = Hi;
	do
	{
		while (A[i]->Int2Var < V) i++;
		while (A[j]->Int2Var > V) j--;
		if (i <= j)
		{
			T = A[i];  A[i] = A[j];  A[j] = T;
			i++;
			if (j > 0) j--;
		}
	} while (i <= j);
	if (Lo < j) QSortIntInt1(A, Lo, j);
	if (i < Hi) QSortIntInt1(A, i, Hi);

}

//--------------------------------------------------------
void GetIntensityTab(RawDataSet* Input, int Type, int View, int Size, unsigned short* Tab, int Pos)
{

	//Padding <unsigned short> ( IQ, PQ, INx, INy, INz, PNx,PNy, PNz,Size,Value); 
	GetIntensityTab0 <unsigned short >(Input->GetDataArray(), Input->GetSliceSize(), Input->GetNx(), Type, View, Size, Tab, Pos);

}

void GetIntensityTab(unsigned char* Input, int Width, int Height, int Type, int View, int Size, unsigned char* Tab, int Pos)
{

	GetIntensityTab0 <unsigned char >(Input, Width * Height, Width, Type, View, Size, Tab, Pos);


}


template <class TYPE>
void GetIntensityTab0(TYPE* Input, int SliceSize, int Nx, int Type, int View, int Size, TYPE* Tab, int Pos)
{


	// TYPE - 0 - 3dcube, 1 - 3dcross, 2 - 3dplus 
	// TYPE - 3 - 2dcube, 4 - 2dcross, 5 - 2dplus
	// MODE - 0 - transv, 1 - coronal, 2 - sagitt
	//FILE *Do_opa;
	//
	int Nr = 0;
	int i, j, k;


	//	int x,y,z;



	//	int SS=Input->GetSliceSize();
	//	int NX=Input->GetNx();

	//	TYPE *Q=InputGetDataArray()+Pos;
	TYPE* Q = Input + Pos;
	int SS = SliceSize;
	int NX = Nx;

	//Do_opa=fopen("c:\\inten","a");

	switch (Type)
	{


	case 0:
	{
		int XX, YY, ZZ;

		//3d pelny median
		int T = -Size * NX;

		ZZ = -Size * SS;
		YY = T;
		XX = -Size;

		for (k = -Size; k <= Size; k++)
		{
			for (j = -Size; j <= Size; j++)
			{
				for (i = -Size; i <= Size; i++)
				{

					Tab[Nr++] = *(Q + XX + YY + ZZ);
					//	fprintf(Do_opa,"%d ",*(Q+XX+YY+ZZ));
					XX++;
				}
				XX = -Size;
				YY += NX;
			}
			ZZ += SS;
			YY = T;
		}

	}
	//	fprintf(Do_opa,"\n");
	break;

	case 1:
	{
		int XXM, YYM, ZZM, XXP, YYP, ZZP;

		//3d cross

		Tab[Nr++] = *(Q);

		XXM = -Size;
		YYM = -Size * NX;
		ZZM = -Size * SS;
		XXP = Size;
		YYP = Size * NX;
		ZZP = Size * SS;



		for (i = 1; i <= Size; i++)
		{
			/*	Tab[Nr++]=Input->GetVoxelValue(x+i,y+i,z+i);
			Tab[Nr++]=Input->GetVoxelValue(x-i,y+i,z+i);
			Tab[Nr++]=Input->GetVoxelValue(x+i,y-i,z+i);
			Tab[Nr++]=Input->GetVoxelValue(x-i,y-i,z+i);
			Tab[Nr++]=Input->GetVoxelValue(x+i,y+i,z-i);
			Tab[Nr++]=Input->GetVoxelValue(x-i,y+i,z-i);
			Tab[Nr++]=Input->GetVoxelValue(x+i,y-i,z-i);
			Tab[Nr++]=Input->GetVoxelValue(x-i,y-i,z-i);
			*/


			Tab[Nr++] = *(Q + XXP + YYP + ZZP);
			Tab[Nr++] = *(Q + XXM + YYP + ZZP);
			Tab[Nr++] = *(Q + XXP + YYM + ZZP);
			Tab[Nr++] = *(Q + XXM + YYM + ZZP);
			Tab[Nr++] = *(Q + XXP + YYP + ZZM);
			Tab[Nr++] = *(Q + XXM + YYP + ZZM);
			Tab[Nr++] = *(Q + XXP + YYM + ZZM);
			Tab[Nr++] = *(Q + XXM + YYM + ZZM);

			XXM++;
			YYM += NX;
			ZZM += SS;
			XXP--;
			YYP -= NX;
			ZZP -= SS;


		}


	}

	break;

	case 2:
	{
		int XXM, YYM, ZZM, XXP, YYP, ZZP;

		//3d plus


		//	Tab[Nr++]=Input->GetVoxelValue(x,y,z);


		Tab[Nr++] = *(Q);



		XXM = -Size;
		YYM = -Size * NX;
		ZZM = -Size * SS;
		XXP = Size;
		YYP = Size * NX;
		ZZP = Size * SS;

		for (i = 1; i <= Size; i++)
		{
			/*Tab[Nr++]=Input->GetVoxelValue(x+i,y,z);
			Tab[Nr++]=Input->GetVoxelValue(x-i,y,z);
			Tab[Nr++]=Input->GetVoxelValue(x,y+i,z);
			Tab[Nr++]=Input->GetVoxelValue(x,y-i,z);
			Tab[Nr++]=Input->GetVoxelValue(x,y,z+i);
			Tab[Nr++]=Input->GetVoxelValue(x,y,z-i);
			*/


			Tab[Nr++] = *(Q + XXP);
			Tab[Nr++] = *(Q + XXM);
			Tab[Nr++] = *(Q + YYP);
			Tab[Nr++] = *(Q + YYM);
			Tab[Nr++] = *(Q + ZZP);
			Tab[Nr++] = *(Q + ZZM);


			XXM++;
			YYM += NX;
			ZZM += SS;
			XXP--;
			YYP -= NX;
			ZZP -= SS;

		}
	}

	break;













	case 3:
	{
		//2d normal
		int XX, YY, ZZ;

		int xsmin = 0;
		int xsmax = 0;
		int ysmin = 0;
		int ysmax = 0;
		int zsmin = 0;
		int zsmax = 0;




		switch (View)
		{

		case 0:
			xsmin = -Size; xsmax = Size; ysmin = -Size; ysmax = Size; zsmin = 0; zsmax = 0;

			break;
		case 1:
			xsmin = -Size; xsmax = Size; ysmin = 0; ysmax = 0; zsmin = -Size; zsmax = Size;
			break;
		case 2:
			xsmin = 0; xsmax = 0; ysmin = -Size; ysmax = Size; zsmin = -Size; zsmax = Size;
			break;
		}
		////////////////////	21.09.2003
		int T = ysmin * NX;
		ZZ = zsmin * SS;
		YY = T;
		XX = xsmin;
		/////////////////////
		for (k = zsmin; k <= zsmax; k++)
		{
			for (j = ysmin; j <= ysmax; j++)
			{
				for (i = xsmin; i <= xsmax; i++)
				{
					Tab[Nr++] = *(Q + XX + YY + ZZ);
					XX += 1;
				}
				YY += NX;
				XX = -Size;
			}
			ZZ += SS;
			YY = T;
		}
	}


	break;


	case 4:
	{
		//2d cross
		int XXM, YYM, ZZM, XXP, YYP, ZZP;

		Tab[Nr++] = *(Q);


		switch (View)
		{

		case 0:
			//xy;
			XXM = -Size;
			YYM = -Size * NX;
			XXP = Size;
			YYP = Size * NX;

			for (i = 1; i <= Size; i++)
			{
				Tab[Nr++] = *(Q + XXP + YYP);
				Tab[Nr++] = *(Q + XXM + YYP);
				Tab[Nr++] = *(Q + XXP + YYM);
				Tab[Nr++] = *(Q + XXM + YYM);
				XXM++;
				YYM += NX;
				XXP--;
				YYP -= NX;

			}



			break;
		case 1:
			//xz
			XXM = -Size;
			ZZM = -Size * SS;
			XXP = Size;
			ZZP = Size * SS;


			for (i = 1; i <= Size; i++)
			{
				Tab[Nr++] = *(Q + XXP + ZZP);
				Tab[Nr++] = *(Q + XXM + ZZP);
				Tab[Nr++] = *(Q + XXP + ZZM);
				Tab[Nr++] = *(Q + XXM + ZZM);
				XXM++;
				ZZM += SS;
				XXP--;
				ZZP -= SS;
			}

			break;
		case 2:
			//yz
			YYM = -Size * NX;
			ZZM = -Size * SS;
			YYP = Size * NX;
			ZZP = Size * SS;
			for (i = 1; i <= Size; i++)
			{


				Tab[Nr++] = *(Q + YYP + ZZP);
				Tab[Nr++] = *(Q + YYM + ZZP);
				Tab[Nr++] = *(Q + YYP + ZZM);
				Tab[Nr++] = *(Q + YYM + ZZM);


				YYM += NX;
				ZZM += SS;
				YYP -= NX;
				ZZP -= SS;
			}

			break;
		}





	}



	break;

	case 5:
		//2d plus

		int XXM, YYM, ZZM, XXP, YYP, ZZP;

		Tab[Nr++] = *(Q);


		switch (View)
		{

		case 0:
			//xy;
			XXM = -Size;
			YYM = -Size * NX;
			XXP = Size;
			YYP = Size * NX;

			for (i = 1; i <= Size; i++)
			{
				Tab[Nr++] = *(Q + XXP);
				Tab[Nr++] = *(Q + XXM);
				Tab[Nr++] = *(Q + YYP);
				Tab[Nr++] = *(Q + YYM);
				XXM++;
				YYM += NX;
				XXP--;
				YYP -= NX;

			}



			break;
		case 1:
			//xz
			XXM = -Size;
			ZZM = -Size * SS;
			XXP = Size;
			ZZP = Size * SS;


			for (i = 1; i <= Size; i++)
			{
				Tab[Nr++] = *(Q + XXP);
				Tab[Nr++] = *(Q + XXM);
				Tab[Nr++] = *(Q + ZZP);
				Tab[Nr++] = *(Q + ZZM);
				XXM++;
				ZZM += SS;
				XXP--;
				ZZP -= SS;
			}

			break;
		case 2:
			//yz
			YYM = -Size * NX;
			ZZM = -Size * SS;
			YYP = Size * NX;
			ZZP = Size * SS;
			for (i = 1; i <= Size; i++)
			{


				Tab[Nr++] = *(Q + YYP);
				Tab[Nr++] = *(Q + YYM);
				Tab[Nr++] = *(Q + ZZP);
				Tab[Nr++] = *(Q + ZZM);


				YYM += NX;
				ZZM += SS;
				YYP -= NX;
				ZZP -= SS;
			}

			break;
		}





	}

	/*
	Tab[Nr++]=Input->GetVoxelValue(x,y,z);

	for (i=1;i<=Size;i++)
	{
	Tab[Nr++]=Input->GetVoxelValue(x+i,y,z);
	Tab[Nr++]=Input->GetVoxelValue(x-i,y,z);
	Tab[Nr++]=Input->GetVoxelValue(x,y+i,z);
	Tab[Nr++]=Input->GetVoxelValue(x,y-i,z);
	}
	*/
	//fclose(Do_opa);



}

int GetIntetnsityTabLength(int KernelType, int KernelSize)
{


	switch (KernelType)
	{
	case 0:
		//3D normal		
		return (2 * KernelSize + 1) * (2 * KernelSize + 1) * (2 * KernelSize + 1);
	case 1:
		//3D cross
		return 8 * KernelSize + 1;
	case 2:
		//3d plus
		return 6 * KernelSize + 1;
	case 3:
		//2d normal
		return (2 * KernelSize + 1) * (2 * KernelSize + 1);
	case 4:
		//2d cross
		return 4 * KernelSize + 1;
	case 5:
		//2d plus
		return 4 * KernelSize + 1;
	case 6:
		return 2 * KernelSize + 1;

	}
	return 0;
}

//----------------------------------

void skipline(FILE* f)
{
	char c;
loop:c = fgetc(f);
	if (c == '\n' || c == EOF)
		return;
	else goto loop;
	return;


}
//----------------------------------
Timer::Timer()
{
	Time.start();
}
//----------------------------------
QString Timer::GetTime()
{


	int time = Time.elapsed();

	int hour = time / 3600000;
	int minute = (time - hour * 3600000) / 60000;
	int second = (time - hour * 3600000 - minute * 60000) / 1000;
	int msecond = time - hour * 3600000 - minute * 60000 - second * 1000;

	QString Text;
	if (hour > 0)   Text.append(QString::number(hour) + "h ");
	if (minute > 0) Text.append(QString::number(minute) + "min ");
	if (second > 0) Text.append(QString::number(second) + "sec ");
	Text.append(QString::number(msecond) + "msec ");

	return Text;
}
//------------------------------
void RibonTimer::Start(const QString& text, bool _busy)
{
	busy = _busy;
	MainText = text + " ...";
	Time.start();
	if (sendText == true) emit SendMessage(MainText);
	//if (busy == true)
	//	QApplication::setOverrideCursor(Qt::WaitCursor);

}
//-----------------------------------------
void RibonTimer::SetText(const QString& text)
{
	MainText = text;
	if (sendText == true)emit SendMessage(MainText);
}
//-----------------------------------------
void RibonTimer::TemporaryAppendToMainText(const QString& text)
{
	if (sendText == true)emit SendMessage(MainText + text);
}
//-------------------------------------------
void RibonTimer::Stop(const QString& text, bool showtime, bool qWarn)
{

	int time = Time.elapsed();
	int hour = time / 3600000;
	int minute = (time - hour * 3600000) / 60000;
	int second = (time - hour * 3600000 - minute * 60000) / 1000;
	int msecond = time - hour * 3600000 - minute * 60000 - second * 1000;

	QString Text = " done: ";
	if (hour > 0)   Text.append(QString::number(hour) + "h ");
	if (minute > 0) Text.append(QString::number(minute) + "min ");
	if (second > 0) Text.append(QString::number(second) + "sec ");
	if (showtime == true)
		Text.append(QString::number(msecond) + "msec ");

	if (qWarn == false)
	{
		if (sendText == true)
		{
			if (text.length() > 0)
				emit SendMessage(text + Text);
			else
				emit SendMessage("&" + Text);
		}
	}
	else
	{
		if (sendText == true)
		{
			if (text.length() > 0)
				emit qWarning() << QString(text + Text);
			else
				emit qWarning() << QString("&" + Text);
		}
	}

	if (busy == true)
		QApplication::restoreOverrideCursor();

}
//-------------------------------------------
void RibonTimer::StopError(const QString& text, bool showtime)
{

	int time = Time.elapsed();
	int hour = time / 3600000;
	int minute = (time - hour * 3600000) / 60000;
	int second = (time - hour * 3600000 - minute * 60000) / 1000;
	int msecond = time - hour * 3600000 - minute * 60000 - second * 1000;

	QString Text = " ERROR: ";
	if (hour > 0)   Text.append(QString::number(hour) + "h ");
	if (minute > 0) Text.append(QString::number(minute) + "min ");
	if (second > 0) Text.append(QString::number(second) + "sec ");
	if (showtime == true)
		Text.append(QString::number(msecond) + "msec ");

	if (text.length() > 0)
		emit SendMessage(text + Text);
	else
		emit SendMessage("&" + Text);

	if (busy == true)
		QApplication::restoreOverrideCursor();
}
//---------------------------------------------
template <class TYPE>
void Draw4QuarterPixels(TYPE* image, int W, int H, int x0, int y0, int X, int Y, TYPE I)
{

	DrawPoint(image, W, H, x0 + X, y0 + Y, I);
	DrawPoint(image, W, H, x0 - X, y0 + Y, I);
	DrawPoint(image, W, H, x0 + X, y0 - Y, I);
	DrawPoint(image, W, H, x0 - X, y0 - Y, I);
	//	image[x0+X+(y0+Y)*W]=I;
	//	image[x0-X+(y0+Y)*W]=I;
	//	image[x0+X+(y0-Y)*W]=I;
	//	image[x0-X+(y0-Y)*W]=I;	
}

//---------------------------------------------

template <class TYPE>
void	DrawEllipse(TYPE* image, int W, int H, int x0, int y0, int a, int b, TYPE I)
{

	int x, y, a2, b2, S, T;

	a2 = a * a;
	b2 = b * b;
	x = 0;
	y = b;
	S = a2 * (1 - 2 * b) + 2 * b2;
	T = b2 - 2 * a2 * (2 * b - 1);
	Draw4QuarterPixels <TYPE>(image, W, H, x0, y0, x, y, I);
	do
	{
		if (S < 0)
		{
			S += 2 * b2 * (2 * x + 3);
			T += 4 * b2 * (x + 1);
			x++;
		}
		else if (T < 0)
		{
			S += 2 * b2 * (2 * x + 3) - 4 * a2 * (y - 1);
			T += 4 * b2 * (x + 1) - 2 * a2 * (2 * y - 3);
			x++;
			y--;
		}
		else
		{
			S -= 4 * a2 * (y - 1);
			T -= 2 * a2 * (2 * y - 3);
			y--;
		}
		Draw4QuarterPixels <TYPE>(image, W, H, x0, y0, x, y, I);
	} while (y > 0);
}

//---------------------------------------------
template <class TYPE>
void	ScanlineXFillArray(TYPE* image, int W, int H, TYPE col1, TYPE col2)
{
	//col 1 ten po ktorym skanuje
	// col 2 - ten ktorym narysuje wypelniacze
	//	FILE *Do_opa;
	int i, j;

	// scan X
	bool Criter;
	int Left, Right;

	int JMW;
	for (j = 0; j < H; j++)
	{
		JMW = j * W;
		//w lewo
		i = 0;
		Criter = false;
		while (Criter == false)
		{
			if (i < W - 1)
			{
				if (image[i + JMW] != col1)
					i++;
				else
					Criter = true;

			}
			else
				Criter = true;
		}
		Left = i;

		//w prawo
		i = W - 1;
		Criter = false;
		while (Criter == false)
		{
			if (i > 0)
			{
				if (image[i + JMW] != col1)
					i--;
				else
					Criter = true;

			}
			else
				Criter = true;
		}
		Right = i;


		if ((Left - Right) < W - 1)
		{
			if (sizeof(TYPE) == 1)
				memset((image + j * W + Left), col2, Right - Left + 1);
		}
	}



}

//------------------------------------------------

template <class TYPE>
void DrawLine(TYPE* image, int W, int H, int Ax, int Ay, int Bx, int By, TYPE Color)
{

	int dX = abs(Bx - Ax);
	int dY = abs(By - Ay);

	int CurrentX = Ax;
	int CurrentY = Ay;


	int Xincr, Yincr;
	if (Ax > Bx) { Xincr = -1; }
	else { Xincr = 1; }
	if (Ay > By) { Yincr = -1; }
	else { Yincr = 1; }


	if (dX >= dY)
	{
		int dPr = dY << 1;
		int dPru = dPr - (dX << 1);
		int P = dPr - dX;

		for (; dX >= 0; dX--)
		{

			//image[CurrentX+CurrentY*W]=Color;
			DrawPoint(image, W, H, CurrentX, CurrentY, Color);
			if (P > 0)
			{
				CurrentX += Xincr;
				CurrentY += Yincr;
				P += dPru;
			}
			else
			{
				CurrentX += Xincr;
				P += dPr;
			}
		}
	}
	else
	{
		int dPr = dX << 1;
		int dPru = dPr - (dY << 1);
		int P = dPr - dY;

		for (; dY >= 0; dY--)
		{

			//image[CurrentX+CurrentY*W]=Color;
			DrawPoint(image, W, H, CurrentX, CurrentY, Color);

			if (P > 0)
			{
				CurrentX += Xincr;
				CurrentY += Yincr;
				P += dPru;
			}
			else
			{
				CurrentY += Yincr;
				P += dPr;
			}
		}
	}


}


//------------------------------------------------

template <class TYPE>
void DrawPoint(TYPE* image, int W, int H, int X, int Y, TYPE Color)
{
	if ((X >= 0) && (Y >= 0) && (X < W) && (Y < H))
		image[X + Y * W] = Color;
}

void	DrawPointUINT(unsigned int* image, int W, int H, int X, int Y, unsigned int Color)
{
	DrawPoint <unsigned int>(image, W, H, X, Y, Color);
}
void	DrawPointUSHORT(unsigned short* image, int W, int H, int X, int Y, unsigned short Color)
{
	DrawPoint <unsigned short>(image, W, H, X, Y, Color);
}

void	DrawPointUCHAR(unsigned char* image, int W, int H, int X, int Y, unsigned char Color)
{
	DrawPoint <unsigned char>(image, W, H, X, Y, Color);
}

//------------------------------------------------

//------------------------------------------------
void	DrawLineUINT(unsigned int* image, int W, int H, int Ax, int Ay, int Bx, int By, unsigned int Color)
{
	DrawLine <unsigned int>(image, W, H, Ax, Ay, Bx, By, Color);
}
void	DrawLineUSHORT(unsigned short* image, int W, int H, int Ax, int Ay, int Bx, int By, unsigned short Color)
{
	DrawLine <unsigned short>(image, W, H, Ax, Ay, Bx, By, Color);
}
void	DrawLineUCHAR(unsigned char* image, int W, int H, int Ax, int Ay, int Bx, int By, unsigned char Color)
{
	DrawLine <unsigned char>(image, W, H, Ax, Ay, Bx, By, Color);
}

//------------------------------------------------


//------------------------------------------------

void	DrawEllipseUINT(unsigned int* image, int W, int H, int x0, int y0, int a, int b, unsigned int I)
{
	DrawEllipse<unsigned int>(image, W, H, x0, y0, a, b, I);
}
void	DrawEllipseUSHORT(unsigned short* image, int W, int H, int x0, int y0, int a, int b, unsigned short I)
{
	DrawEllipse<unsigned short>(image, W, H, x0, y0, a, b, I);
}
void	DrawEllipseUCHAR(unsigned char* image, int W, int H, int x0, int y0, int a, int b, unsigned char I)
{
	DrawEllipse<unsigned char>(image, W, H, x0, y0, a, b, I);
}
//------------------------------------------------

void	ScanlineXFillArrayUINT(unsigned int* image, int W, int H, unsigned int col1, unsigned int col2)
{
	ScanlineXFillArray <unsigned int >(image, W, H, col1, col2);

}

void	ScanlineXFillArrayUSHORT(unsigned short* image, int W, int H, unsigned short col1, unsigned short col2)
{
	ScanlineXFillArray <unsigned short >(image, W, H, col1, col2);
}

void	ScanlineXFillArrayUCHAR(unsigned char* image, int W, int H, unsigned char col1, unsigned char col2)
{
	ScanlineXFillArray <unsigned char >(image, W, H, col1, col2);
}


//------------------------------------------------


QString GetTimeDateName()
{
	QTime time = QTime::currentTime();
	QDate date = QDate::currentDate();

	QString name = QString::number(date.year()) + "_" +
		QString::number(date.month()) + "_" +
		QString::number(date.day()) + "_" +
		QString::number(time.hour()) + "_" +
		QString::number(time.minute()) + "_" +
		QString::number(time.second()) + "_";
	return name;

}


char ReadFileLine(FILE* file, QString* text)
{
	int maxsize = 1000;

	int i;
	char character;
	char* line = new char[maxsize];

	for (i = 0; i < maxsize; i++)
	{
		character = fgetc(file);
		if ((character != '\n') && (character != EOF))
			line[i] = character;
		else
			break;
	}

	*text = QString::fromLatin1(line, i);
	delete[]line;


	return character;

}
//--------------------------------------------
template <class TYPE>
void GetIntensityTabXYZ(TYPE* Input, int Nx, int Ny, int Nz, int Type, int View, int Size, TYPE* Tab, int x, int y, int z)
{


	// TYPE - 0 - 3dcube, 1 - 3dcross, 2 - 3dplus 
	// TYPE - 3 - 2dcube, 4 - 2dcross, 5 - 2dplus
	// TYPE - 6 1D
	// MODE - 0 - transv, 1 - coronal, 2 - sagitt
	//FILE *Do_opa;
	//
	int Nr = 0;
	int i, j, k;


	//	int x,y,z;

	int SliceSize = Nx * Ny;
	int pos = SliceSize * z + Nx * y + x;


	//	TYPE *Q=InputGetDataArray()+Pos;
	TYPE* Q = Input + pos;
	int SS = SliceSize;
	int NX = Nx;


	//	int length=GetIntetnsityTabLength(Type,Size);

	if ((x - Size >= 0) && (y - Size >= 0) && (z - Size >= 0)
		&& (x + Size < Nx) && (y + Size < Ny) && (z + Size < Nz))
		switch (Type)
		{
		case 0:
		{
			int XX, YY, ZZ;

			//3d pelny median
			int T = -Size * NX;
			ZZ = -Size * SS; YY = T; XX = -Size;

			for (k = -Size; k <= Size; k++)
			{
				for (j = -Size; j <= Size; j++)
				{
					for (i = -Size; i <= Size; i++)
					{
						Tab[Nr++] = *(Q + XX + YY + ZZ);
						XX++;
					}
					XX = -Size; YY += NX;
				}
				ZZ += SS;	YY = T;
			}

		}
		break;

		case 1:
		{
			int XXM, YYM, ZZM, XXP, YYP, ZZP;

			//3d cross

			Tab[Nr++] = *(Q);

			XXM = -Size;	YYM = -Size * NX;	ZZM = -Size * SS;
			XXP = Size;	YYP = Size * NX;	ZZP = Size * SS;

			for (i = 1; i <= Size; i++)
			{
				Tab[Nr++] = *(Q + XXP + YYP + ZZP);
				Tab[Nr++] = *(Q + XXM + YYP + ZZP);
				Tab[Nr++] = *(Q + XXP + YYM + ZZP);
				Tab[Nr++] = *(Q + XXM + YYM + ZZP);
				Tab[Nr++] = *(Q + XXP + YYP + ZZM);
				Tab[Nr++] = *(Q + XXM + YYP + ZZM);
				Tab[Nr++] = *(Q + XXP + YYM + ZZM);
				Tab[Nr++] = *(Q + XXM + YYM + ZZM);

				XXM++;	YYM += NX;	ZZM += SS;
				XXP--;	YYP -= NX;	ZZP -= SS;

			}
		}

		break;

		case 2:
		{
			int XXM, YYM, ZZM, XXP, YYP, ZZP;
			//3d plus
			Tab[Nr++] = *(Q);

			XXM = -Size;	YYM = -Size * NX;	ZZM = -Size * SS;
			XXP = Size;	YYP = Size * NX;	ZZP = Size * SS;

			for (i = 1; i <= Size; i++)
			{
				Tab[Nr++] = *(Q + XXP);
				Tab[Nr++] = *(Q + XXM);
				Tab[Nr++] = *(Q + YYP);
				Tab[Nr++] = *(Q + YYM);
				Tab[Nr++] = *(Q + ZZP);
				Tab[Nr++] = *(Q + ZZM);

				XXM++;	YYM += NX;	ZZM += SS;
				XXP--;	YYP -= NX;	ZZP -= SS;
			}
		}
		break;

		case 3:
		{
			//2d normal
			int XX, YY, ZZ;

			int xsmin = 0;	int xsmax = 0;	int ysmin = 0;
			int ysmax = 0;	int zsmin = 0;	int zsmax = 0;

			switch (View)
			{

			case 0:
				xsmin = -Size; xsmax = Size; ysmin = -Size; ysmax = Size; zsmin = 0; zsmax = 0;
				break;
			case 1:
				xsmin = -Size; xsmax = Size; ysmin = 0; ysmax = 0; zsmin = -Size; zsmax = Size;
				break;
			case 2:
				xsmin = 0; xsmax = 0; ysmin = -Size; ysmax = Size; zsmin = -Size; zsmax = Size;
				break;
			}

			int T = ysmin * NX;
			ZZ = zsmin * SS;
			YY = T;
			XX = xsmin;
			/////////////////////
			for (k = zsmin; k <= zsmax; k++)
			{
				for (j = ysmin; j <= ysmax; j++)
				{
					for (i = xsmin; i <= xsmax; i++)
					{
						Tab[Nr++] = *(Q + XX + YY + ZZ);
						XX += 1;
					}
					YY += NX;	XX = -Size;
				}
				ZZ += SS;	YY = T;
			}
		}
		break;

		case 4:
		{
			//2d cross
			int XXM, YYM, ZZM, XXP, YYP, ZZP;

			Tab[Nr++] = *(Q);

			switch (View)
			{

			case 0:
				//xy;
				XXM = -Size;	YYM = -Size * NX;
				XXP = Size;	YYP = Size * NX;

				for (i = 1; i <= Size; i++)
				{
					Tab[Nr++] = *(Q + XXP + YYP);
					Tab[Nr++] = *(Q + XXM + YYP);
					Tab[Nr++] = *(Q + XXP + YYM);
					Tab[Nr++] = *(Q + XXM + YYM);

					XXM++;	YYM += NX;
					XXP--;	YYP -= NX;
				}
				break;

			case 1:
				//xz
				XXM = -Size;	ZZM = -Size * SS;
				XXP = Size;	ZZP = Size * SS;

				for (i = 1; i <= Size; i++)
				{
					Tab[Nr++] = *(Q + XXP + ZZP);
					Tab[Nr++] = *(Q + XXM + ZZP);
					Tab[Nr++] = *(Q + XXP + ZZM);
					Tab[Nr++] = *(Q + XXM + ZZM);

					XXM++;		ZZM += SS;
					XXP--;		ZZP -= SS;
				}

				break;

			case 2:

				//yz
				YYM = -Size * NX;	ZZM = -Size * SS;
				YYP = Size * NX;	ZZP = Size * SS;

				for (i = 1; i <= Size; i++)
				{
					Tab[Nr++] = *(Q + YYP + ZZP);
					Tab[Nr++] = *(Q + YYM + ZZP);
					Tab[Nr++] = *(Q + YYP + ZZM);
					Tab[Nr++] = *(Q + YYM + ZZM);

					YYM += NX;	ZZM += SS;
					YYP -= NX;	ZZP -= SS;
				}

				break;
			}
		}



		break;

		case 5:
		{
			//2d plus

			int XXM, YYM, ZZM, XXP, YYP, ZZP;

			Tab[Nr++] = *(Q);

			switch (View)
			{

			case 0:
				//xy;
				XXM = -Size;	YYM = -Size * NX;
				XXP = Size;	YYP = Size * NX;

				for (i = 1; i <= Size; i++)
				{
					Tab[Nr++] = *(Q + XXP);
					Tab[Nr++] = *(Q + XXM);
					Tab[Nr++] = *(Q + YYP);
					Tab[Nr++] = *(Q + YYM);
					XXM++;	YYM += NX;
					XXP--;	YYP -= NX;

				}
				break;

			case 1:
				//xz
				XXM = -Size;	ZZM = -Size * SS;
				XXP = Size;	ZZP = Size * SS;

				for (i = 1; i <= Size; i++)
				{
					Tab[Nr++] = *(Q + XXP);
					Tab[Nr++] = *(Q + XXM);
					Tab[Nr++] = *(Q + ZZP);
					Tab[Nr++] = *(Q + ZZM);

					XXM++;	ZZM += SS;
					XXP--;	ZZP -= SS;
				}

				break;

			case 2:
				//yz
				YYM = -Size * NX;	ZZM = -Size * SS;
				YYP = Size * NX;	ZZP = Size * SS;

				for (i = 1; i <= Size; i++)
				{
					Tab[Nr++] = *(Q + YYP);
					Tab[Nr++] = *(Q + YYM);
					Tab[Nr++] = *(Q + ZZP);
					Tab[Nr++] = *(Q + ZZM);

					YYM += NX;	ZZM += SS;
					YYP -= NX;	ZZP -= SS;
				}
				break;
			}


			break;
		}
		case 6:
		{
			int XXM, YYM, ZZM, XXP, YYP, ZZP;
			//1D


			//Tab[Nr++]=*(Q);
			Tab[Size] = *(Q);

			switch (View)
			{

			case 0:
				//xy;
				XXM = -Size;
				XXP = Size;
				for (i = 1; i <= Size; i++)
				{
					Tab[Size + i] = *(Q + XXP);
					Tab[Size - 1] = *(Q + XXM);
					XXM++;	XXP--;
				}
				break;

			case 1:
				//xz
				ZZM = -Size * SS;	ZZP = Size * SS;
				for (i = 1; i <= Size; i++)
				{

					Tab[Size + i] = *(Q + ZZP);
					Tab[Size - i] = *(Q + ZZM);

					ZZM += SS;	ZZP -= SS;
				}
				break;

			case 2:
				//yz
				YYM = -Size * NX;	YYP = Size * NX;

				for (i = 1; i <= Size; i++)
				{
					Tab[Size + i] = *(Q + YYP);
					Tab[Size - i] = *(Q + YYM);
					YYM += NX;	YYP -= NX;
				}
				break;
			}
			break;
		}
		}
	else
		//	for(i=0;i<length;i++)
		//		Tab[i]=0;

	{
		//int ax,ay,az;

		int volumeSizem1 = SliceSize * (Nz - 1);
		int nxm1 = Nx - 1;
		int slicesizem1 = Nx * (Ny - 1);

		int ynx = y * Nx;
		int zss = z * SliceSize;
		int NXX, NYY, NZZ;

		switch (Type)
		{


		case 0:
		{
			int XX, YY, ZZ;

			//3d pelny 
			int T = -Size * NX;
			ZZ = -Size * SS; YY = T; XX = -Size;

			for (k = -Size; k <= Size; k++)
			{
				for (j = -Size; j <= Size; j++)
				{
					for (i = -Size; i <= Size; i++)
					{
						GetIntensityTabPos(XX, YY, ZZ, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1);
						Tab[Nr++] = *(Q + NXX + NYY + NZZ);

						XX++;
					}
					XX = -Size; YY += NX;
				}
				ZZ += SS;	YY = T;
			}

		}
		break;

		case 1:
		{
			int XXM, YYM, ZZM, XXP, YYP, ZZP;

			//3d cross

			Tab[Nr++] = *(Q);

			XXM = -Size;	YYM = -Size * NX;	ZZM = -Size * SS;
			XXP = Size;	YYP = Size * NX;	ZZP = Size * SS;

			for (i = 1; i <= Size; i++)
			{
				GetIntensityTabPos(XXP, YYP, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXM, YYP, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXP, YYM, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXM, YYM, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXP, YYP, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXM, YYP, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXP, YYM, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXM, YYM, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);



				//Tab[Nr++]=*(Q+XXP+YYP+ZZP);
				//Tab[Nr++]=*(Q+XXM+YYP+ZZP);
				//Tab[Nr++]=*(Q+XXP+YYM+ZZP);
				//Tab[Nr++]=*(Q+XXM+YYM+ZZP);
				//Tab[Nr++]=*(Q+XXP+YYP+ZZM);
				//Tab[Nr++]=*(Q+XXM+YYP+ZZM);
				//Tab[Nr++]=*(Q+XXP+YYM+ZZM);
				//Tab[Nr++]=*(Q+XXM+YYM+ZZM);


				XXM++;	YYM += NX;	ZZM += SS;
				XXP--;	YYP -= NX;	ZZP -= SS;

			}
		}

		break;

		case 2:
		{
			int XXM, YYM, ZZM, XXP, YYP, ZZP;
			//3d plus
			Tab[Nr++] = *(Q);

			XXM = -Size;	YYM = -Size * NX;	ZZM = -Size * SS;
			XXP = Size;	YYP = Size * NX;	ZZP = Size * SS;

			for (i = 1; i <= Size; i++)
			{
				GetIntensityTabPos(XXP, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXM, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(0, YYP, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(0, YYM, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(0, 0, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(0, 0, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);



				//Tab[Nr++]=*(Q+XXP);
				//Tab[Nr++]=*(Q+XXM);
				//Tab[Nr++]=*(Q+YYP);
				//Tab[Nr++]=*(Q+YYM);
				//Tab[Nr++]=*(Q+ZZP);
				//Tab[Nr++]=*(Q+ZZM);

				XXM++;	YYM += NX;	ZZM += SS;
				XXP--;	YYP -= NX;	ZZP -= SS;
			}
		}
		break;

		case 3:
		{
			//2d normal
			int XX, YY, ZZ;

			int xsmin = 0;	int xsmax = 0;	int ysmin = 0;
			int ysmax = 0;	int zsmin = 0;	int zsmax = 0;

			switch (View)
			{

			case 0:
				xsmin = -Size; xsmax = Size; ysmin = -Size; ysmax = Size; zsmin = 0; zsmax = 0;
				break;
			case 1:
				xsmin = -Size; xsmax = Size; ysmin = 0; ysmax = 0; zsmin = -Size; zsmax = Size;
				break;
			case 2:
				xsmin = 0; xsmax = 0; ysmin = -Size; ysmax = Size; zsmin = -Size; zsmax = Size;
				break;
			}

			int T = ysmin * NX;
			ZZ = zsmin * SS;
			YY = T;
			XX = xsmin;
			/////////////////////
			for (k = zsmin; k <= zsmax; k++)
			{
				for (j = ysmin; j <= ysmax; j++)
				{
					for (i = xsmin; i <= xsmax; i++)
					{
						GetIntensityTabPos(XX, YY, ZZ, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1);
						Tab[Nr++] = *(Q + NXX + NYY + NZZ);
						XX += 1;
					}
					YY += NX;	XX = -Size;
				}
				ZZ += SS;	YY = T;
			}
		}
		break;

		case 4:
		{
			//2d cross
			int XXM, YYM, ZZM, XXP, YYP, ZZP;

			Tab[Nr++] = *(Q);

			switch (View)
			{

			case 0:
				//xy;
				XXM = -Size;	YYM = -Size * NX;
				XXP = Size;	YYP = Size * NX;

				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(XXP, YYP, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXM, YYP, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXP, YYM, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXM, YYM, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);

					//Tab[Nr++]=*(Q+XXP+YYP);
					//Tab[Nr++]=*(Q+XXM+YYP);
					//Tab[Nr++]=*(Q+XXP+YYM);
					//Tab[Nr++]=*(Q+XXM+YYM);

					XXM++;	YYM += NX;
					XXP--;	YYP -= NX;
				}
				break;

			case 1:
				//xz
				XXM = -Size;	ZZM = -Size * SS;
				XXP = Size;	ZZP = Size * SS;

				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(XXP, 0, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXM, 0, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXP, 0, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXM, 0, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);

					//Tab[Nr++]=*(Q+XXP+ZZP);
					//Tab[Nr++]=*(Q+XXM+ZZP);
					//Tab[Nr++]=*(Q+XXP+ZZM);
					//Tab[Nr++]=*(Q+XXM+ZZM);

					XXM++;		ZZM += SS;
					XXP--;		ZZP -= SS;
				}

				break;

			case 2:

				//yz
				YYM = -Size * NX;	ZZM = -Size * SS;
				YYP = Size * NX;	ZZP = Size * SS;

				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(0, YYP, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, YYM, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, YYP, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, YYM, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);

					//Tab[Nr++]=*(Q+YYP+ZZP);
					//Tab[Nr++]=*(Q+YYM+ZZP);
					//Tab[Nr++]=*(Q+YYP+ZZM);
					//Tab[Nr++]=*(Q+YYM+ZZM);

					YYM += NX;	ZZM += SS;
					YYP -= NX;	ZZP -= SS;
				}

				break;
			}
		}



		break;

		case 5:
		{
			//2d plus

			int XXM, YYM, ZZM, XXP, YYP, ZZP;

			Tab[Nr++] = *(Q);

			switch (View)
			{

			case 0:
				//xy;
				XXM = -Size;	YYM = -Size * NX;
				XXP = Size;	YYP = Size * NX;

				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(XXP, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXM, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, YYP, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, YYM, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);

					//Tab[Nr++]=*(Q+XXP);
					//Tab[Nr++]=*(Q+XXM);
					//Tab[Nr++]=*(Q+YYP);
					//Tab[Nr++]=*(Q+YYM);
					XXM++;	YYM += NX;
					XXP--;	YYP -= NX;

				}
				break;

			case 1:
				//xz
				XXM = -Size;	ZZM = -Size * SS;
				XXP = Size;	ZZP = Size * SS;

				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(XXP, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXM, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, 0, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, 0, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);

					//Tab[Nr++]=*(Q+XXP);
					//Tab[Nr++]=*(Q+XXM);
					//Tab[Nr++]=*(Q+ZZP);
					//Tab[Nr++]=*(Q+ZZM);

					XXM++;	ZZM += SS;
					XXP--;	ZZP -= SS;
				}

				break;

			case 2:
				//yz
				YYM = -Size * NX;	ZZM = -Size * SS;
				YYP = Size * NX;	ZZP = Size * SS;

				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(0, YYP, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, YYM, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, 0, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, 0, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Nr++] = *(Q + NXX + NYY + NZZ);

					//Tab[Nr++]=*(Q+YYP);
					//Tab[Nr++]=*(Q+YYM);
					//Tab[Nr++]=*(Q+ZZP);
					//Tab[Nr++]=*(Q+ZZM);

					YYM += NX;	ZZM += SS;
					YYP -= NX;	ZZP -= SS;
				}
				break;
			}
		}
		break;
		case 6:
		{
			//1Dewe
			int XXM, YYM, ZZM, XXP, YYP, ZZP;

			//Tab[Nr++]=*(Q);
			Tab[Size] = *(Q);
			switch (View)
			{

			case 0:
				//xy;
				XXM = -Size;
				XXP = Size;
				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(XXP, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Size + i] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(XXM, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Size - i] = *(Q + NXX + NYY + NZZ);
					//Tab[Nr++]=*(Q+XXP);
					//Tab[Nr++]=*(Q+XXM);
					XXM++;	XXP--;
				}
				break;

			case 1:
				//xz
				ZZM = -Size * SS;	ZZP = Size * SS;
				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(0, 0, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Size + i] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, 0, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Size - i] = *(Q + NXX + NYY + NZZ);

					//Tab[Nr++]=*(Q+ZZP);
					//Tab[Nr++]=*(Q+ZZM);

					ZZM += SS;	ZZP -= SS;
				}
				break;

			case 2:
				//yz
				YYM = -Size * NX;	YYP = Size * NX;

				for (i = 1; i <= Size; i++)
				{
					GetIntensityTabPos(0, YYP, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Size + i] = *(Q + NXX + NYY + NZZ);
					GetIntensityTabPos(0, YYM, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumeSizem1); Tab[Size - i] = *(Q + NXX + NYY + NZZ);

					//Tab[Nr++]=*(Q+YYP);
					//Tab[Nr++]=*(Q+YYM);
					YYM += NX;	YYP -= NX;
				}
				break;
			}
			break;
		}
		}
	}

}
//--------------------------------------------


void	GetIntensityTabXYZ(RawDataSet* Input, int Type, int View, int Size, unsigned short* Tab, int x, int y, int z)
{
	GetIntensityTabXYZ<unsigned short>(

		Input->GetDataArray(), Input->GetNx(), Input->GetNy(), Input->GetNz(),
		Type, View, Size, Tab, x, y, z);
}
//--------------------------------------
void	GetGaussianKernel(double* tab, double radius, double stddev)
{
	/*int i;
	double sum;

	if (stddev == 0.0)
	{
	tab[0] = 1.0;
	return;
	}

	double dsigmsq=2.0*stddev*stddev;
	double coef=1.0/(dsigmsq*PI);

	int iRadius=Round(radius);
	// fill in kernel
	sum = 0.0;
	for (i = -iRadius; i<= iRadius; i++)
	{
	sum += tab[i+iRadius] =
	exp(- ((double)(i*i)) / (dsigmsq));
	}




	//sum*=coef;
	// normalize
	for (i = -iRadius; i<= iRadius; i++)
	tab[i+iRadius] /= sum;*/
	int i;
	double sum;

	if (stddev == 0.0)
	{
		tab[0] = 1.0;
		return;
	}

	double dsigmsq = 1.0 / (2.0 * stddev * stddev);
	//	double coef=1.0/(dsigmsq*PI);


	int iRadius = Round(radius);
	double ff = stddev / radius;
	// fill in kernel
	sum = 0.0;
	double x;

	for (i = -iRadius; i <= iRadius; i++)
	{
		//		x=ff*x;

		sum += tab[i + iRadius] =
			exp(-(double)(i * i) * ff);
	}


	//exp(-0.5*(sigma*i/iRadius)^2
	// normalize
	for (i = -iRadius; i <= iRadius; i++)
		tab[i + iRadius] /= sum;

}
//-------------------------------------------------------
void	GetGaussianKernelFirstDerivative(double* tab, double radius, double stddev)
{
	int i;
	double sum;

	if (stddev == 0.0)
	{
		tab[0] = 1.0;
		return;
	}

	int iRadius = Round(radius);
	double dsigmsq = 2.0 * stddev * stddev;
	double coef = -1.0 / (dsigmsq * M_PI * stddev * stddev);
	// fill in kernel
	sum = 0.0;
	double a = 0.0;
	for (i = -iRadius; i <= iRadius; i++)
	{

		a = tab[i + iRadius] = (double)i *
			exp(-((double)(i * i)) / (stddev * stddev * 2.0));
		if (i < 0) sum += a;
	}


	//	sum=(a>0)?a*2:1.0;
	for (i = -iRadius; i <= iRadius; i++)
		tab[i + iRadius] /= sum;
}
//-------------------------------------------------
void	GetGaussianKernelSecondDerivative(double* tab, double radius, double stddev)
{
	int i;
	double sum;

	if (stddev == 0.0)
	{
		tab[0] = 1.0;
		return;
	}

	int iRadius = Round(radius);
	double dsigmsq = 2.0 * stddev * stddev;
	double coef = -1.0 / (dsigmsq * M_PI * stddev * stddev);
	// fill in kernel
	sum = 0.0;
	for (i = -iRadius; i <= iRadius; i++)
	{
		sum += tab[i + iRadius] = coef * (1.0 - ((double)(i * i)) / (dsigmsq)) *
			exp(-((double)(i * i)) / (dsigmsq));
	}

	// normalize
	for (i = -iRadius; i <= iRadius; i++)
		tab[i + iRadius] /= sum;
}

//-------------------------------------------------------
//----------------------------------------------
void	GaussianSmoothingFiltering(double stddev, int radius, RawDataSet* Input, RawDataSet* _data)
{

	if ((Input != NULL) && (_data != NULL))
	{

		//radius=50.0;
		//stddev=1;

		int					size2 = 2 * radius + 1;
		double* gaussianKernelTab = new double[size2];
		unsigned short* intensityTab = new unsigned short[size2];
		GetGaussianKernel(gaussianKernelTab, (double)radius, stddev);

		//transversal 1d gauss

		RawDataSet rData = *Input;
		RawDataSet* tempData = &rData;

		//	unsigned short *buff0 = _data->GetDataArray();
		//	unsigned short *buff1 = new unsigned short[_data->GetVolumeSize()];

		unsigned short* Q;
		int i, j, k;
		int pos;
		unsigned short** tabz;



		int n[3];
		_data->GetN(n);
		int sliceSize = n[0] * n[1];


		Q = Input->GetDataArray();
		pos = 0;

		pos = 0;
		for (k = 0; k < _data->GetNz(); k++)
			for (j = 0; j < _data->GetNy(); j++)
				for (i = 0; i < _data->GetNx(); i++)
				{
					GetIntensityTab1D(tempData, i, j, k, 0, intensityTab, radius);
					//	GetIntensityTabXYZ(	tempData,6,0,radius,intensityTab,i,j,k);
					Q[pos++] = Apply1DNormFilter(gaussianKernelTab, intensityTab, size2);
				}

		//tempData2=tempData;	tempData=_data;	_data=tempData2;	tempData2=NULL;

		Q = tempData->GetDataArray();
		pos = 0;
		for (k = 0; k < _data->GetNz(); k++)
			for (j = 0; j < _data->GetNy(); j++)
				for (i = 0; i < _data->GetNx(); i++)
				{
					GetIntensityTab1D(_data, i, j, k, 1, intensityTab, radius);
					//	GetIntensityTabXYZ(	tempData,6,1,radius,intensityTab,i,j,k);
					//Q[pos++]=tempData->GetVoxelValue(i,j,k);
					Q[pos++] = Apply1DNormFilter(gaussianKernelTab, intensityTab, size2);
				}
		//	tempData2=tempData;	tempData=_data;	_data=tempData2;	tempData2=NULL;	Q=_data->GetDataArray();

		if (_data->GetNz() > radius)
		{
			Q = _data->GetDataArray();
			pos = 0;
			for (k = 0; k < _data->GetNz(); k++)
				for (j = 0; j < _data->GetNy(); j++)
					for (i = 0; i < _data->GetNx(); i++)
					{
						GetIntensityTab1D(tempData, i, j, k, 2, intensityTab, radius);
						//GetIntensityTabXYZ(	tempData,6,2,radius,intensityTab,i,j,k);
						Q[pos++] = Apply1DNormFilter(gaussianKernelTab, intensityTab, size2);
					}

		}
		else
		{
			memcpy(_data->GetDataArray(), tempData->GetDataArray(), _data->GetVolumeSize() * sizeof(unsigned short));
		}




		tempData = NULL;

		Q = NULL;
		delete[]gaussianKernelTab;
		delete[]intensityTab;


	}
}
//-------------------------------------------------

//----------------------------------------------
unsigned short	Apply1DNormFilter(double* Tab1, unsigned short* Tab2, int	Size)
{
	int	i;
	double	value = 0;
	for (i = 0; i < Size; i++)
		value += Tab2[i] * Tab1[i];

	//if (value<0) value=0;

	int a = Round(value);
	//	int a=(Size!=0)?value/(double)Size:value;

	if (a < 0) a = 0;
	else if (a > 65535) a = 65535;

	return a;

}
//------------------
unsigned char	Apply1DNormFilter(double* Tab1, unsigned char* Tab2, int	Size)
{
	int	i;
	double	value = 0;
	for (i = 0; i < Size; i++)
		value += Tab2[i] * Tab1[i];

	//if (value<0) value=0;

	int a = Round(value);
	//	int a=(Size!=0)?value/(double)Size:value;

	if (a < 0) a = 0;
	else if (a > 255) a = 255;

	return a;

}
//----------------------------------------------
double			Apply1DNormFilterF(double* Tab1, unsigned short* Tab2, int	Size)
{
	int	i;
	double	value = 0;
	for (i = 0; i < Size; i++)
		value += Tab2[i] * Tab1[i];

	return value;

}
//----------------------------------------------
//double			Apply1DNormFilterF(double *Tab1,double *Tab2,int	Size)
//{
//	int	i;
//	double	value=0;
//	for(i=0;i<Size;i++)
//		value+=Tab2[i]*Tab1[i];
//
//	return value;
//
//}
//----------------------------------------------
//float			Apply1DNormFilterF(float *Tab1,float *Tab2,int	Size)
//{
//	int	i;
//	float	value=0;
//	for(i=0;i<Size;i++)
//		value+=Tab2[i]*Tab1[i];
//
//	return value;
//
//}
//-----------------------------------------
//------------------------------------------------------
//double			Apply1DNormFilterF(double *Tab1,unsigned short *Tab2,int	Size)
//{
//	int	i;
//	double	value=0;
//	for(i=0;i<Size;i++)
//		value+=Tab2[i]*Tab1[i];
//
//	return value;
//
//}
//----------------------------------------------
//-------------------------------------------
double	Apply1DNormFilterF(double* Tab1, double* Tab2, int	Size)
{
	int	i;
	double	value = 0;
	for (i = 0; i < Size; i++)
		value += Tab2[i] * Tab1[i];

	return value;

}


///---------------------------------------------------
void GetIntensityTab1D(RawDataSet* data, int x, int y, int z, int view, unsigned short* tab, int size)
{


	int nx = data->GetNx();
	int ny = data->GetNy();
	int nz = data->GetNz();
	unsigned short* Input = data->GetDataArray();
	GetIntensityTab1D<unsigned short>(Input, nx, ny, nz, view, size, tab, x, y, z);
}
//-------------------------------------
template <class TYPE>
void GetIntensityTab1D(TYPE* Input, int nx, int ny, int nz, int view, int size, TYPE* tab, int x, int y, int z)
{


	int Nr = 0;
	int i, j, k;
	int ss = nx * ny;
	int pos = ss * z + nx * y + x;
	TYPE* Q = Input + pos;

	//int ss=data->GetSliceSize();
	//int pos=data->GetVoxelPos(x,y,z);
	//unsigned short *Q=data->GetDataArray()+pos;
	int volumesizem1 = ss * (nz - 1);
	int nxm1 = nx - 1;
	int slicesizem1 = nx * (ny - 1);

	int ynx = y * nx;
	int zss = z * ss;
	int NXX, NYY, NZZ;
	int XXM, YYM, ZZM, XXP, YYP, ZZP;
	int ss2 = size * 2 + 1;

	NXX = 0;
	NYY = 0;
	NZZ = 0;

	tab[size] = *Q;

	switch (view)
	{

	case 0:
		//xy;
		XXM = -size;
		XXP = size;
		if ((x - size < 0) || (x + size >= nx))
			for (i = 1; i <= size; i++)
			{
				GetIntensityTabPos(XXP, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumesizem1); tab[ss2 - i] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(XXM, 0, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumesizem1); tab[i - 1] = *(Q + NXX + NYY + NZZ);

				XXM++;	XXP--;
			}
		else
			for (i = 1; i <= size; i++)
			{
				tab[ss2 - i] = *(Q + XXP);
				tab[i - 1] = *(Q + XXM);
				XXM++;	XXP--;
			}
		break;

	case 2:
		//xz
		ZZM = -size * ss;	ZZP = size * ss;
		if ((z - size < 0) || (z + size >= nz))
			for (i = 1; i <= size; i++)
			{
				GetIntensityTabPos(0, 0, ZZP, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumesizem1); tab[ss2 - i] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(0, 0, ZZM, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumesizem1); tab[i - 1] = *(Q + NXX + NYY + NZZ);
				ZZM += ss;	ZZP -= ss;
			}
		else
			for (i = 1; i <= size; i++)
			{
				tab[ss2 - i] = *(Q + ZZP);
				tab[i - 1] = *(Q + ZZM);
				ZZM += ss;	ZZP -= ss;
			}
		break;

	case 1:
		//yz
		YYM = -size * nx;	YYP = size * nx;
		if ((y - size < 0) || (y + size >= ny))
			for (i = 1; i <= size; i++)
			{
				GetIntensityTabPos(0, YYP, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumesizem1); tab[ss2 - i] = *(Q + NXX + NYY + NZZ);
				GetIntensityTabPos(0, YYM, 0, NXX, NYY, NZZ, x, ynx, zss, nxm1, slicesizem1, volumesizem1); tab[i - 1] = *(Q + NXX + NYY + NZZ);
				YYM += nx;	YYP -= nx;
			}
		else
			for (i = 1; i <= size; i++)
			{
				tab[ss2 - i] = *(Q + YYP);
				tab[i - 1] = *(Q + YYM);
				YYM += nx;	YYP -= nx;
			}
		break;
	}



}
//--------------------------------------
void GetIntensityTab1D(double* Input, int nx, int ny, int nz, int view, int size, double* tab, int x, int y, int z)
{
	GetIntensityTab1D<double>(Input, nx, ny, nz, view, size, tab, x, y, z);
}
//--------------------------------------------------
void GetIntensityTab1D(unsigned short* Input, int nx, int ny, int nz, int view, int size, unsigned short* tab, int x, int y, int z)
{
	GetIntensityTab1D<unsigned short>(Input, nx, ny, nz, view, size, tab, x, y, z);
}
//--------------------------------------------------
void GetIntensityTab1D(unsigned char* Input, int nx, int ny, int nz, int view, int size, unsigned char* tab, int x, int y, int z)
{
	GetIntensityTab1D<unsigned char>(Input, nx, ny, nz, view, size, tab, x, y, z);
}
//-----------------------------------------------

int* GetGrowingKernel2(int& kernelSize, int size, RawDataSet* data, int type)
{

	int i, j, k;
	if (type > 2) type = 0;
	if (type == 0)// szescian
	{
		kernelSize = (size * 2 + 1) * (size * 2 + 1) * (size * 2 + 1) - 1;

		int* kernel = new int[kernelSize];
		int pos = 0;

		for (k = -size; k < size + 1; k++)
		{

			for (j = -size; j < size + 1; j++)
			{
				for (i = -size; i < size + 1; i++)
				{
					if (!((i == 0) && (j == 0) && (k == 0)))
					{
						kernel[pos++] = data->GetVoxelPos(i, j, k);
					}

				}
			}
		}

		return kernel;

	}
	else if (type == 1) //kolko
	{
		kernelSize = (size * 2 + 1) * (size * 2 + 1) * (size * 2 + 1) - 1;

		int* kernel = new int[kernelSize];
		int pos = 0;

		for (k = -size; k < size + 1; k++)
		{

			for (j = -size; j < size + 1; j++)
			{
				for (i = -size; i < size + 1; i++)
				{
					if (!((i == 0) && (j == 0) && (k == 0)))
					{
						if ((i * i + j * j + k * k) < size * size + 1e-6)
						{
							kernel[pos++] = data->GetVoxelPos(i, j, k);
						}
					}

				}
			}
		}
		if (pos < kernelSize)
		{
			int* kernel2 = new int[pos];
			memcpy(kernel2, kernel, pos * sizeof(int));
			kernelSize = pos;
			return kernel2;

		}

		return kernel;

	}
	else if (type == 2) //cross
	{

		kernelSize = (size * 2 + 1) * (size * 2 + 1) * 3 - 2;
		int* kernel = new int[kernelSize];
		int pos = 0;

		//dol
		for (k = -size; k < 0; k++)
		{
			for (j = -size; j < size + 1; j++)
			{
				for (i = -size; i < size + 1; i++)
				{

					if (((j != 0) && (i == 0)) || (j == 0))
					{
						kernel[pos++] = data->GetVoxelPos(i, j, k);
					}
				}
			}
		}


		//srodek

		k = 0;
		for (j = -size; j < size + 1; j++)
		{
			for (i = -size; i < size + 1; i++)
			{
				kernel[pos++] = data->GetVoxelPos(i, j, k);
			}
		}

		//dol
		for (k = 1; k < size + 1; k++)
		{
			for (j = -size; j < size + 1; j++)
			{
				for (i = -size; i < size + 1; i++)
				{
					if (((j != 0) && (i == 0)) || (j == 0))
					{
						kernel[pos++] = data->GetVoxelPos(i, j, k);
					}
				}
			}
		}
		if (pos < kernelSize)
		{
			int* kernel2 = new int[pos];
			memcpy(kernel2, kernel, pos * sizeof(int));
			kernelSize = pos;
			return kernel2;

		}

		return kernel;



	}

	return NULL;


}
//--------------------------------------------------------
int* GetGrowingKernel1D(int& kernelSize, int size, RawDataSet* data, bool direction, bool addCenter)
{
	int i, j;



	kernelSize = (size * 2 + 1) - 1;
	if (addCenter == true) kernelSize++;

	int* kernel = new int[kernelSize];
	int pos = 0;



	if (direction == 0) //x
	{
		for (i = -size; i < size + 1; i++)
		{
			if ((!(i == 0)) || (addCenter == true))
			{
				kernel[pos++] = data->GetVoxelPos(i, 0, 0);
			}

		}
	}
	else //y
	{
		for (i = -size; i < size + 1; i++)
		{
			if ((!(i == 0)) || (addCenter == true))
			{
				kernel[pos++] = data->GetVoxelPos(0, i, 0);
			}

		}
	}



	return kernel;




	//return NULL;

}
//----------------------------------
int* GetGrowingKernelDirectional2D(int& kernelSize, int size, RawDataSet* data, DiffXKernelData* diffData, int type, int direction, int p1, int p2)
{

	int i, j, k;
	int pos;
	int* kernel = NULL;//new int[kernelSize];

					   //double *func(int i,int j,int size, int *params)=NULL;

					   //typedef bool (*neighFunction) (int i,int j,int size, double *params);
					   //typedef bool (*neighFunction) (int i,int j,int size, double *params);

	bool(*f) (int i, int j, int size, double* params) = NULL;
	double angle, sizex2, sizey2, cosa, sina;

	double params[10] = { 0 };
	params[0] = p1;
	params[1] = p2;
	switch (type)
	{
	case -1:
		//params[0]=p1; params[1]=p2;
		f = neighFunctionSquareCenter;
		break;
		break;

	case 0: //square

		switch (direction)
		{
		case 0: 		f = neighFunctionSquare0; break;//pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)		if ((i<=0)&&(j<=0))	{			kernel[pos++]=data->GetVoxelPos(i,j,0);			}	break;	//upper-left
		case 1: 		f = neighFunctionSquare1; break;//pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)		if ((i<=0)&&(j>=0))	{			kernel[pos++]=data->GetVoxelPos(i,j,0);			}	break;	//lower-left
		case 2: 		f = neighFunctionSquare2; break;//pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)		if ((i>=0)&&(j<=0))	{			kernel[pos++]=data->GetVoxelPos(i,j,0);			}	break;	//upper-right
		case 3: 		f = neighFunctionSquare3; break;//pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)		if ((i>=0)&&(j>=0))	{			kernel[pos++]=data->GetVoxelPos(i,j,0);			}	break;	//lower-left
		}

		break;

	case 1: //triangle

		switch (direction)
		{
		case 0: 		f = neighFunctionTriangle0; break;//pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)		if ((j<=0)&&(abs(i)<=abs(j)))	{			kernel[pos++]=data->GetVoxelPos(i,j,0);			}	break;	//up
		case 1: 		f = neighFunctionTriangle1; break;//pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)		if ((j>=0)&&(abs(i)<=abs(j)))	{			kernel[pos++]=data->GetVoxelPos(i,j,0);			}	break;	//down
		case 2: 		f = neighFunctionTriangle2; break;//pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)		if ((i<=0)&&(abs(i)>=abs(j)))	{			kernel[pos++]=data->GetVoxelPos(i,j,0);			}	break;	//left
		case 3: 		f = neighFunctionTriangle3; break;//pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)		if ((i>=0)&&(abs(i)>=abs(j)))	{			kernel[pos++]=data->GetVoxelPos(i,j,0);			}	break;	//right
		}

		break;

	case 2: //ellipse


		angle = direction *to_rad;
		sizex2 = p1 * p1;
		sizey2 = p2 * p2;
		cosa = cos(angle);
		sina = sin(angle);

		params[0] = sizex2;
		params[1] = sizey2;
		params[2] = cosa;
		params[3] = sina;

		f = neighFunctionEllipse;
		break;

	case 3: //ellipse


		angle = direction * to_rad;
		sizex2 = p1 * p1;
		sizey2 = p2 * p2;
		cosa = cos(angle);
		sina = sin(angle);

		params[0] = sizex2;
		params[1] = sizey2;
		params[2] = cosa;
		params[3] = sina;

		f = neighFunctionEllipseHalfY;
		break;

		/*if ((p1>0)&&(p2>0))
		{
		double angle=direction*rad;
		double sizex2=p1*p1;
		double sizey2=p2*p2;
		double cosa=cos(angle);
		double sina=sin(angle);
		double x,y;
		pos=0;	for(j=-size;j<size+1;j++)for(i=-size;i<size+1;i++)
		{
		x=cosa*i-sina*j;
		y=sina*i+cosa*i;
		if (Round(x*x/sizex2+y*y/sizey2<=1))	kernel[pos++]=data->GetVoxelPos(i,j,0);
		}
		}

		break;*/



	}


	if (f == NULL)
	{
		return NULL;
	}

	size += 1;

	if (diffData == NULL)
	{



		kernelSize = (size * 2 + 1) * (size * 2 + 1);
		//int pos=-1;
		kernel = new int[kernelSize];
		pos = 0;
		for (j = -size; j < size + 1; j++)
		{
			for (i = -size; i < size + 1; i++)
			{
				if (f(i, j, size, params) == true)
				{
					kernel[pos++] = data->GetVoxelPos(i, j, 0);
				}

			}
		}

		if (pos > 0)
		{
			if (pos < kernelSize)
			{
				int* kernel2 = new int[pos];
				memcpy(kernel2, kernel, pos * sizeof(int));
				kernelSize = pos;
				return kernel2;

			}

			return kernel;
		}
		else
		{
			kernelSize = 0;
			delete[]kernel;
			return NULL;
		}

	}
	else
	{
		diffData->ClearData();


		//narysuje kernel 1 na kwadracie
		//narysuje kernel 2 na lwadracie drugim przesuniety o 1

		int ww = size * 2 + 1;
		int hh = ww;

		int sizeTab = ww * hh;

		int* tab0 = new int[sizeTab];//tu bedzie zwykly
		int* tab1 = new int[sizeTab];//tu bedzie przesuniety
		int x, y;
		pos = 0;
		for (j = -size; j < size + 1; j++)
		{
			for (i = -size; i < size + 1; i++)
			{
				x = i + size;
				y = j + size;
				if (f(i, j, size, params) == true)
				{
					//tab0[x + y*ww] = 1;
					tab0[pos] = 1;
				}
				else
				{
					tab0[pos] = 0;
				}

				if (f(i - 1, j, size, params) == true)
				{
					//tab0[x + y*ww] = 1;
					tab1[pos] = 1;
				}
				else
				{
					tab1[pos] = 0;
				}

				pos++;



			}
		}
		//teoretycznie powinienem miec kernel zebrany jako obrazek a drugi przesuniety o 1
		pos = 0;
		int realPos;

		kernelSize = (size * 2 + 1) * (size * 2 + 1);
		int plusSize = 0;
		int minusSize = 0;
		int newKernelSize = 0;
		int size4p1 = (size * 4 + 1);
		int* kernelPlus = new int[size4p1 * 2];
		int* kernelMinus = new int[size4p1 * 2];
		kernel = new int[kernelSize];

		for (j = -size; j < size + 1; j++)
		{
			for (i = -size; i < size + 1; i++)
			{
				realPos = data->GetVoxelPos(i, j, 0);
				if (tab0[pos] == 1)						kernel[newKernelSize++] = realPos;
				if ((tab0[pos] == 1) && (tab1[pos] == 0))		kernelMinus[minusSize++] = realPos;
				if ((tab0[pos] == 0) && (tab1[pos] == 1))		kernelPlus[plusSize++] = realPos;

				pos++;
			}
		}




		//int size4p1=(size*4+1);
		//
		//int *kernelPlus=new int[size4p1*2];
		//int *kernelMinus=new int[size4p1*2];
		//int plusSize=0;
		//int minusSize=0;

		//kernelSize=(size*2+2)*(size*2+1);

		//kernel =new int[kernelSize];
		//int pos1,pos2;
		//bool t1,t2;
		//memset (kernel,0,kernelSize*sizeof(int));
		////wypelnie i=0; 1

		//pos=0;	
		//for(j=-size;j<size+1;j++)
		//{
		//	for(i=-size;i<size+1;i++)	
		//	{
		//		if (f(i,j,size,params)==true)
		//		{			
		//			kernel[pos]=1;//data->GetVoxelPos(i,j,0);		
		//		}	
		//			
		//		pos++;
		//	}
		//	pos++;//bo przeskakuje o 1 tablica ma rozmiar o 1 wiekszy dla xow
		//}

		////dodam sytuacje gdy i=1; 2

		//pos=0;	
		//for(j=-size;j<size+1;j++)
		//{
		//	pos++;//przeskakuje o 1 na poczatku bo to x=1
		//	for(i=-size+1;i<size+2;i++)	
		//	{
		//		if (f(i-1,j,size,params)==true)
		//		{			
		//			kernel[pos]+=2;//data->GetVoxelPos(i,j,0);		
		//		}	
		//		pos++;
		//			
		//	}
		//}

		////zbieram
		//int newKernelSize=0;
		//pos=0;
		//int xPos;
		//int val;
		//for(j=-size;j<size+1;j++)
		//{
		//	for(i=-size;i<size+2;i++)	
		//	{
		//		
		//		val=kernel[pos++];
		//	
		//		if (val>0)
		//		{

		//			xPos=data->GetVoxelPos(i,j,0);					
		//			switch(val)
		//			{
		//				case 1:
		//					kernel[newKernelSize++]=xPos;
		//					kernelMinus[minusSize++]=xPos-1;
		//				break;

		//				case 2:
		//					kernelPlus[plusSize++]=xPos-1;
		//				break;

		//				case 3:
		//					kernel[newKernelSize++]=xPos;
		//				break;
		//			}
		//			
		//		}
		//	}

		//}


		if (newKernelSize > 0)
		{
			if (newKernelSize < kernelSize)
			{
				int* kernel2 = new int[newKernelSize];
				memcpy(kernel2, kernel, newKernelSize * sizeof(int));
				kernelSize = newKernelSize;
				delete[]kernel;
				kernel = kernel2;
				//return kernel2;

			}
			diffData->UpdateKernels(kernelPlus, plusSize, kernelMinus, minusSize);
			return kernel;
		}
		else
		{


			delete[]kernelPlus;
			delete[]kernelMinus;
			kernelSize = 0;
			delete[]kernel;
			return NULL;
		}

	}


	return NULL;

}
///////////////////////////////////////////---------------------------------------------
//----------------------------------------------------------------------------------------
int* GetGrowingKernel2D(int& kernelSize, int size, RawDataSet* data, int type, bool addCenter)
{

	int i, j;

	if (type == 0)// szescian
	{
		kernelSize = (size * 2 + 1) * (size * 2 + 1) - 1;
		if (addCenter == true) kernelSize++;

		int* kernel = new int[kernelSize];
		int pos = 0;



		for (j = -size; j < size + 1; j++)
		{
			for (i = -size; i < size + 1; i++)
			{
				if ((!((i == 0) && (j == 0))) || (addCenter == true))
				{
					kernel[pos++] = data->GetVoxelPos(i, j, 0);
				}

			}
		}


		return kernel;

	}
	else if (type == 1) //kolko
	{
		kernelSize = (size * 2 + 1) * (size * 2 + 1) - 1;

		if (addCenter == true) kernelSize++;
		int* kernel = new int[kernelSize];
		int pos = 0;



		for (j = -size; j < size + 1; j++)
		{
			for (i = -size; i < size + 1; i++)
			{
				if ((!((i == 0) && (j == 0))) || (addCenter == true))
				{
					if ((i * i + j * j) < size * size + 1e-6)
					{
						kernel[pos++] = data->GetVoxelPos(i, j, 0);
					}
				}

			}
		}

		if (pos < kernelSize)
		{
			int* kernel2 = new int[pos];
			memcpy(kernel2, kernel, pos * sizeof(int));
			kernelSize = pos;
			delete[]kernel;
			return kernel2;

		}

		return kernel;

	}


	return NULL;


}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void GetGrowingKernel(int& KernelSize, int* Kernel, int View, int KernelType, int width, int sliceSize)
{
	//if (View!=3) KernelType-=3;

	int A = 1;
	int B = A + width - 2;
	int C = B + 1;
	int D = C + 1;
	int E = D + sliceSize - 2 * width - 2;
	int F = E + 1;
	int G = F + 1;
	int H = G + width - 2;
	int I = H + 1;
	int J = I + 1;
	int K = J + width - 2;
	int L = K + 1;
	int M = L + 1;




	switch (View)
	{


	case 3:
		// 3D 
		switch (KernelType)
		{
		case 0:
			//full

			KernelSize = 13;
			Kernel[0] = A;
			Kernel[1] = B;
			Kernel[2] = C;
			Kernel[3] = D;
			Kernel[4] = E;
			Kernel[5] = F;
			Kernel[6] = G;
			Kernel[7] = H;
			Kernel[8] = I;
			Kernel[9] = J;
			Kernel[10] = K;
			Kernel[11] = L;
			Kernel[12] = M;

			break;

		case 1:
			//cross

			KernelSize = 6;
			Kernel[0] = B;
			Kernel[1] = D;
			Kernel[2] = E;
			Kernel[3] = G;
			Kernel[4] = K;
			Kernel[5] = M;

			break;

		case 2:
			//plus

			KernelSize = 3;
			Kernel[0] = A;
			Kernel[1] = C;
			Kernel[2] = I;

			break;


		}


		break;

	case 0:
		// transversal
		switch (KernelType)
		{
		case 0:
			//full

			KernelSize = 4;
			Kernel[0] = A;
			Kernel[1] = B;
			Kernel[2] = C;
			Kernel[3] = D;



			break;

		case 1:
			//cross

			KernelSize = 2;
			Kernel[0] = B;
			Kernel[1] = D;


			break;

		case 2:
			//plus
			KernelSize = 2;
			Kernel[0] = A;
			Kernel[1] = C;



			break;


		}


		break;

	case 1:
		// coronal
		switch (KernelType)
		{
		case 0:
			//full

			KernelSize = 4;
			Kernel[0] = A;
			Kernel[1] = H;
			Kernel[2] = I;
			Kernel[3] = J;



			break;

		case 1:
			//cross

			KernelSize = 2;
			Kernel[0] = H;
			Kernel[1] = J;


			break;

		case 2:
			//plus
			KernelSize = 2;
			Kernel[0] = A;
			Kernel[1] = I;



			break;


		}

		break;





	case 2:
		// sagittal
		switch (KernelType)
		{
		case 0:
			//full

			KernelSize = 4;
			Kernel[0] = C;
			Kernel[1] = F;
			Kernel[2] = I;
			Kernel[3] = L;



			break;

		case 1:
			//cross

			KernelSize = 2;
			Kernel[0] = F;
			Kernel[1] = L;


			break;

		case 2:
			//plus
			KernelSize = 2;
			Kernel[0] = C;
			Kernel[1] = I;



			break;


		}


	}



}

//-----------------------------------------------------------------
MyShape createDilateConturPoints(int* params, RawDataSet* data, int* listTmp, int morphologyKernelType, int morphologyType, int* dilatePoints, int col[3], int morphVal, unsigned char* origMask, int dilateRadius)
{

	int count = params[0];
	int minx = params[1];
	int maxx = params[2];
	int miny = params[3];
	int maxy = params[4];
	int width = params[5];
	int height = params[6];

	int nnx = maxx - minx + 1;
	int nny = maxy - miny + 1;

	int padd = 2;
	int nSizeX = nnx + padd * 2;
	int nSizeY = nny + padd * 2;
	int nSsize = nSizeX * nSizeY;

	int padd1 = 1;
	int nSizeX1 = nnx + padd1 * 2;
	int nSizeY1 = nny + padd1 * 2;

	//tworze maly zbiorek z danymi aktualnej wyspy powiekszonej o padd z kazdej strony
	unsigned char* tabx = new unsigned char[nSsize];
	memset(tabx, 0, nSsize * sizeof(unsigned char));

	int n[3];

	int dsizemin1 = data->GetSliceSize() - 1;
	data->GetN(n);
	double nx1d = 1.0 / n[0];
	//zmniejszona tablica 
	int x, y;
	for (int k = 0; k < count; k++)
	{

		y = listTmp[k] * nx1d;
		x = listTmp[k] - y * n[0];

		x += padd - minx;	y += padd - miny;
		tabx[x + y * nSizeX] = 1;

	}

	//tablica wypelniona

	RawDataSet* noneData = new RawDataSet(nSizeX, nSizeY, 1, 1, 1, 1, 1, false);
	int kSize;
	int kerenelSize2 = 0;
	int size2 = 1;
	int kernelType = morphologyKernelType;
	int* kernel2 = GetGrowingKernel2D(kerenelSize2, size2, noneData, kernelType);
	int tmp;
	int dsizemin1n = noneData->GetVolumeSize() - 1;


	int nrOfDilatePoints = 0;

	int startPos = nSizeX * padd1 + padd1;
	bool stop;
	double nnx1d = 1.0 / nSizeX;
	//contur extraction

	int frontVal = morphologyType;
	int backVal = !frontVal;
	int counterek = 0;
	int ax, ay;


	int dilKernelSize;
	int* dilKernel = NULL;
	//
	if (dilateRadius > 0)
	{
		dilKernel = GetGrowingKernel2D(dilKernelSize, dilateRadius, data, 1);//kolko
	}


	for (int jj = 0; jj < nSizeY1; jj++)
	{

		for (int ii = 0; ii < nSizeX1; ii++)
		{
			if (tabx[startPos] == frontVal)
			{
				stop = false;
				for (int kk = 0; kk < kerenelSize2; kk++)
				{

					tmp = startPos + kernel2[kk];
					if ((tmp > dsizemin1n) || (tmp < 0)) { continue; }
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

					y = startPos * nnx1d;
					x = startPos - y * nSizeX;

					if ((morphVal > 0) && (origMask != NULL))
					{
						int xx = x - padd + minx;
						int yy = y - padd + miny;
						int xPoss = xx + yy * n[0];
						if (!((xPoss < 0) || (xPoss > dsizemin1)))
						{


							origMask[xPoss] = morphVal;

							if (dilateRadius > 0)
							{
								int tmpp;
								for (int kk = 0; kk < dilKernelSize; kk++)
								{
									tmpp = xPoss + dilKernel[kk];
									if (!((tmpp < 0) || (tmpp > dsizemin1)))
									{
										origMask[tmpp] = morphVal;
									}

								}
							}
						}

					}

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




	MyShape shape;
	shape.polyColor.setRgb(col[0], col[1], col[2]);





	for (int ii = 0; ii < nrOfDilatePoints * 2; ii += 2)
	{
		shape.poly.append(QPointF(dilatePoints[ii], dilatePoints[ii + 1]));
	}



	shape.representation = 0;



	delete noneData;
	delete[]tabx;
	delete[]kernel2;
	if (dilKernel != NULL) delete[]dilKernel;
	return shape;
}
//---------------------------------


//-----------------------------------
bool RegionGrowingOnMask2(int startPos, unsigned char* maskData, RawDataSet* data, int valIn, int valOut,
	int* kernel, int* kernelInc, int kernelSize, int* params, int* listS, int* listTmp)
{
	int n[3];

	int dsizemin1 = data->GetSliceSize() - 1;
	data->GetN(n);
	int minx = n[0];
	int maxx = 0;
	int miny = n[1];
	int maxy = 0;
	int x, y;
	int nr0 = 0;
	int count = 0;
	int ap = startPos;
	int lp = 0;
	listS[0] = startPos;
	listTmp[(count++)] = startPos;
	int oldx, oldy;
	int tmp;
	double nx1d = 1.0 / n[0];
	//zalewam
	do
	{
		ap = listS[lp--];

		oldy = ap * nx1d;

		for (int i = 0; i < kernelSize; i++)
		{
			tmp = ap + kernel[i];
			y = tmp * nx1d;
			x = tmp - y * n[0];

			if (!((tmp > dsizemin1) || (tmp < 0) || (y != oldy + kernelInc[i]))) //brzydkie
			{
				if (maskData[tmp] == valIn)
				{
					maskData[tmp] = valOut;
					listS[++lp] = tmp;
					listTmp[count++] = tmp;



					if (x < minx) minx = x;
					if (x > maxx) maxx = x;
					if (y < miny) miny = y;
					if (y > maxy) maxy = y;
				}
			}
		}
	} while (lp >= 0);

	int width = maxx - minx;
	int height = maxy - miny;
	if (params != NULL)
	{

		params[0] = count;
		params[1] = minx;
		params[2] = maxx;
		params[3] = miny;
		params[4] = maxy;
		params[5] = width;
		params[6] = height;
	}
	return true;


}

//------------------------------------
int RegionGrowingOnMask(int startPos, unsigned char* maskData, RawDataSet* data, int valIn, int valOut, int valTemp, int startSize, unsigned char* blockMask)
{
	//block mask nie pozwala sie rozlewac na pola >0
	int pos = startPos;

	if (pos > 0)
	{
		int dataSize = data->GetVolumeSize();
		int size = (int)((double)dataSize * 0.2);
		int stepSize = size / 2;
		int tmp;
		int i;
		int dsizemin1 = dataSize - 1;

		if (startSize <= 0) startSize = size;

		//--------------------------------------------------dddddddddddddddd
		int lp = 0, ap = 0; //rozlewam sie na inne punkty z otoczenia nie oznaczone
		int count = 0;
		int* listS = new int[startSize];
		int kernelSize;
		int* step = GetGrowingKernel2(kernelSize, 1, data, 0);

		bool anyFound = true;
		ap = pos;
		if (maskData[pos] == valIn)
		{
			anyFound = false;

			if (blockMask == NULL)
			{
				for (i = 0; i < kernelSize; i++)
				{
					tmp = ap + step[i];
					if (!((tmp > dsizemin1) || (tmp < 0)))
					{
						if (maskData[tmp] == valIn)
						{
							anyFound = true;
							pos = tmp;
							break;
						}
					}
				}
			}
			else
			{
				for (i = 0; i < kernelSize; i++)
				{
					tmp = ap + step[i];
					if (!((tmp > dsizemin1) || (tmp < 0)))
					{
						if ((maskData[tmp] == valIn) && (blockMask[i] == 0))
						{
							anyFound = true;
							pos = tmp;
							break;
						}
					}
				}
			}
		}
		//findStart
		if (anyFound == false) return 0;
		listS[lp] = pos;
		int counter = 0;

		listS[lp] = pos;
		maskData[pos] = valTemp;
		int tmp2;
		int* listTemp = NULL;
		do
		{
			if (lp >= startSize - kernelSize)
			{
				tmp2 = (startSize + stepSize < dataSize) ? startSize + stepSize : dataSize;
				listTemp = new int[tmp2];
				memcpy(listTemp, listS, sizeof(int) * (startSize));
				startSize += stepSize;
				delete[]listS;
				listS = listTemp;
				listTemp = NULL;
			}

			ap = listS[lp--];
			counter++;
			if (blockMask == NULL)
			{
				for (i = 0; i < kernelSize; i++)
				{
					tmp = ap + step[i];
					if (!((tmp > dsizemin1) || (tmp < 0)))
					{
						if (maskData[tmp] == valIn)
						{
							maskData[tmp] = valTemp;
							counter++;
							listS[++lp] = tmp;

							//istPos[count++]=tmp;
						}
					}
				}
			}
			else
			{
				for (i = 0; i < kernelSize; i++)
				{
					tmp = ap + step[i];
					if (!((tmp > dsizemin1) || (tmp < 0)))
					{
						if ((maskData[tmp] == valIn) && (blockMask[i] == 0))
						{
							maskData[tmp] = valTemp;
							counter++;
							listS[++lp] = tmp;

							//istPos[count++]=tmp;
						}
					}
				}

			}
		} while (lp >= 0);

		delete[]listS;

		if (counter > 0)
		{

			for (i = 0; i < data->GetVolumeSize(); i++)
			{
				if (maskData[i] == valTemp) maskData[i] = valOut;
				//else if(MaskData[i]==2) MaskData[i]=1;
			}
		}
		return counter;
	}
	return 0;
}
///---------
int Morphology(unsigned char* mask, RawDataSet* data, int* bounds, unsigned char valIn, unsigned char valInTest, unsigned char valOut, int valRest)
{
	//dla kazdego valIn sprawdzam otoczenie i jesli ma wartosc valIn1 to w docelowej wstawiam valOut
	//jesli restVal>=0 to ustawiam na to jesli nie kopiuje z wzorca
	//dilate trzeba zrobic
	unsigned char* mMask = new unsigned char[data->GetVolumeSize()];
	memset(mMask, 0, data->GetVolumeSize());

	int	nx, ny, nz;
	int	nxny;
	nx = data->GetNx();
	ny = data->GetNy();
	nz = data->GetNz();
	nxny = data->GetSliceSize();
	int nn[3];
	data->GetN(nn);
	//unsigned char mode=valIn;
	//unsigned char cmode=valOut;;
	int x, y, z;

	unsigned char pp;
	bool stop;

	int chgs = 0;
	int i, j, k;
	int pos;

	for (k = -bounds[4]; k < nn[2] - bounds[5]; k++)
		for (j = -bounds[2]; j < nn[1] - bounds[3]; j++)
			for (i = -bounds[0]; i < nn[0] - bounds[1]; i++)
			{
				pos = nxny * k + nx * j + i;
				pp = mask[pos];


				stop = false;
				if (pp == valIn)
					for (z = bounds[4]; z <= bounds[5]; z++)
					{
						for (y = bounds[2]; y <= bounds[3]; y++)
						{
							for (x = bounds[0]; x <= bounds[1]; x++)
							{
								if (mask[nxny * (k + z) + nx * (j + y) + i + x] == valInTest)
									stop = true;
								chgs++;
								if (stop) break;
							}
							if (stop) break;
						}
						if (stop) break;
					}

				if (stop) mMask[pos] = valOut;
				else mMask[pos] = (valRest > -1) ? valRest : pp;
			}

	memcpy(mask, mMask, data->GetVolumeSize());
	delete[]mMask;
	return chgs;
}

//-------------------------------------

//---------------------------------------------
void resizeBilinear(int* pixels, int w, int h, int* output, int w2, int h2)
{
	double x_ratio = ((double)(w - 1)) / w2;
	double y_ratio = ((double)(h - 1)) / h2;


	double safe_w = w - 1.0001;
	double safe_h = h - 1.0001;

#pragma omp for   ordered schedule (guided) 
	for (int i = 0; i < h2; i++)
		//parallel_for(0,h2, [&](int i) 
	{
		int a, b, c, d, x, y, index;
		int offset = w2 * i;
		double x_diff, y_diff, blue, red, green;
		double one_min_x_diff, one_min_y_diff;
		double p1, p2, p3, p4;
		double tx, ty;
		ty = y_ratio * i;
		y = (int)(ty);
		y_diff = (ty)-y;
		one_min_y_diff = 1 - y_diff;
		if (y < safe_h)
			for (int j = 0; j < w2; j++)
			{
				//output[offset++]=GetPixelSSE3(pixels,w,  x_ratio * j,y_ratio * i);
				tx = x_ratio * j;
				x = (int)(tx);
				if (x < safe_w)
				{


					x_diff = (tx)-x;

					one_min_x_diff = 1 - x_diff;

					p1 = one_min_x_diff * one_min_y_diff;
					p2 = (x_diff)*one_min_y_diff;
					p3 = (y_diff)*one_min_x_diff;
					p4 = (x_diff * y_diff);

					index = (y * w + x);
					a = pixels[index];
					b = pixels[index + 1];
					c = pixels[index + w];
					d = pixels[index + w + 1];

					// blue element
					// Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
					blue = (a & 0xff) * p1 + (b & 0xff) * p2 + (c & 0xff) * p3 + (d & 0xff) * p4;

					// green element
					// Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
					green = ((a >> 8) & 0xff)* p1 + ((b >> 8) & 0xff)* p2 + ((c >> 8) & 0xff)* p3 + ((d >> 8) & 0xff)* p4;

					// red element
					// Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
					red = ((a >> 16) & 0xff)* p1 + ((b >> 16) & 0xff)* p2 + ((c >> 16) & 0xff)* p3 + ((d >> 16) & 0xff)* p4;

					output[offset++] = 0xff000000 | ((((int)red) << 16) & 0xff0000) | ((((int)green) << 8) & 0xff00) | ((int)blue);
				}
			}
	}
	//);
}
//---------------------------------------------

void resizeBilinearVTKtoQt(int* pixels, int w, int h, int* output, int w2, int h2)
{
	double x_ratio = ((double)(w - 1)) / w2;
	double y_ratio = ((double)(h - 1)) / h2;


	double safe_w = w - 1.0001;
	double safe_h = h - 1.0001;
	for (int i = 0; i < h2; i++)
		//parallel_for(0,h2, [&](int i) 
	{
		int a, b, c, d, x, y, index;

		int offset = w2 * i;
		double x_diff, y_diff, blue, red, green;
		double one_min_x_diff, one_min_y_diff;
		double p1, p2, p3, p4;
		double tx, ty;
		ty = h - 1 - (y_ratio * i);
		y = (int)(ty);
		y_diff = (ty)-y;
		one_min_y_diff = 1 - y_diff;
		if (y < safe_h)
			for (int j = 0; j < w2; j++)
			{
				//output[offset++]=GetPixelSSE3(pixels,w,  x_ratio * j,y_ratio * i);
				tx = x_ratio * j;
				x = (int)(tx);

				if (x < safe_w)
				{

					x_diff = (tx)-x;

					one_min_x_diff = 1 - x_diff;

					p1 = one_min_x_diff * one_min_y_diff;
					p2 = (x_diff)*one_min_y_diff;
					p3 = (y_diff)*one_min_x_diff;
					p4 = (x_diff * y_diff);

					index = (y * w + x);
					a = pixels[index];
					b = pixels[index + 1];
					c = pixels[index + w];
					d = pixels[index + w + 1];

					// blue element
					// Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
					red = (a & 0xff) * p1 + (b & 0xff) * p2 + (c & 0xff) * p3 + (d & 0xff) * p4;

					// green element
					// Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
					green = ((a >> 8) & 0xff)* p1 + ((b >> 8) & 0xff)* p2 + ((c >> 8) & 0xff)* p3 + ((d >> 8) & 0xff)* p4;

					// red element
					// Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
					blue = ((a >> 16) & 0xff)* p1 + ((b >> 16) & 0xff)* p2 + ((c >> 16) & 0xff)* p3 + ((d >> 16) & 0xff)* p4;

					output[offset++] = 0xff000000 | ((((int)red) << 16) & 0xff0000) | ((((int)green) << 8) & 0xff00) | ((int)blue);
				}
			}
	}
	//);
}
//------------------------
void resizeBilinearVTKtoQtBoxInterlaced(int* pixels, int w, int h, int* output, int w2, int h2, int* box, bool side)
{

	if (box == NULL)
	{
		resizeBilinearVTKtoQt(pixels, w, h, output, w2, h2);
	}
	else
	{

		double x_ratio = ((double)(w - 1)) / (box[2] - box[0]);
		double y_ratio = ((double)(h - 1)) / (box[3] - box[1]);


		double safe_w = w - 1.0001;
		double safe_h = h - 1.0001;
		// for (int i=0;i<h2;i++) 
		box[1] += side;
		//parallel_for(box[1],box[3],2, [&](int i) 
#pragma omp for   ordered schedule (guided) 
		for (int i = box[1]; i < box[3]; ++i)
		{
			int a, b, c, d, x, y, index;

			int offset = w2 * i + box[0];
			double x_diff, y_diff, blue, red, green;
			double one_min_x_diff, one_min_y_diff;
			double p1, p2, p3, p4;
			double tx, ty;
			ty = h - 1 - (y_ratio * (i - box[1]));
			y = (int)(ty);
			y_diff = (ty)-y;
			one_min_y_diff = 1 - y_diff;
			if (y < safe_h)
				for (int j = box[0]; j < box[2]; j++)
				{
					//output[offset++]=GetPixelSSE3(pixels,w,  x_ratio * j,y_ratio * i);
					tx = x_ratio * (j - box[0]);
					x = (int)(tx);

					if (x < safe_w)
					{

						x_diff = (tx)-x;

						one_min_x_diff = 1 - x_diff;

						p1 = one_min_x_diff * one_min_y_diff;
						p2 = (x_diff)*one_min_y_diff;
						p3 = (y_diff)*one_min_x_diff;
						p4 = (x_diff * y_diff);

						index = (y * w + x);
						a = pixels[index];
						b = pixels[index + 1];
						c = pixels[index + w];
						d = pixels[index + w + 1];

						// blue element
						// Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
						red = (a & 0xff) * p1 + (b & 0xff) * p2 + (c & 0xff) * p3 + (d & 0xff) * p4;

						// green element
						// Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
						green = ((a >> 8) & 0xff)* p1 + ((b >> 8) & 0xff)* p2 + ((c >> 8) & 0xff)* p3 + ((d >> 8) & 0xff)* p4;

						// red element
						// Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
						blue = ((a >> 16) & 0xff)* p1 + ((b >> 16) & 0xff)* p2 + ((c >> 16) & 0xff)* p3 + ((d >> 16) & 0xff)* p4;

						output[offset++] = 0xff000000 | ((((int)red) << 16) & 0xff0000) | ((((int)green) << 8) & 0xff00) | ((int)blue);
					}
				}
		}//);
	}
}

void resizeBilinearVTKtoQtBox(int* pixels, int w, int h, int* output, int w2, int h2, int* box)
{

	if (box == NULL)
	{
		resizeBilinearVTKtoQt(pixels, w, h, output, w2, h2);
	}
	else
	{

		double x_ratio = ((double)(w - 1)) / (box[2] - box[0]);
		double y_ratio = ((double)(h - 1)) / (box[3] - box[1]);


		double safe_w = w - 1.0001;
		double safe_h = h - 1.0001;
		// for (int i=0;i<h2;i++) 
		//parallel_for(box[1],box[3], [&](int i) 
		for (int i = box[1]; i < box[3]; ++i)
		{
			int a, b, c, d, x, y, index;

			int offset = w2 * i + box[0];
			double x_diff, y_diff, blue, red, green;
			double one_min_x_diff, one_min_y_diff;
			double p1, p2, p3, p4;
			double tx, ty;
			ty = h - 1 - (y_ratio * (i - box[1]));
			y = (int)(ty);
			y_diff = (ty)-y;
			one_min_y_diff = 1 - y_diff;
			if (y < safe_h)
				for (int j = box[0]; j < box[2]; j++)
				{
					//output[offset++]=GetPixelSSE3(pixels,w,  x_ratio * j,y_ratio * i);
					tx = x_ratio * (j - box[0]);
					x = (int)(tx);

					if (x < safe_w)
					{

						x_diff = (tx)-x;

						one_min_x_diff = 1 - x_diff;

						p1 = one_min_x_diff * one_min_y_diff;
						p2 = (x_diff)*one_min_y_diff;
						p3 = (y_diff)*one_min_x_diff;
						p4 = (x_diff * y_diff);

						index = (y * w + x);
						a = pixels[index];
						b = pixels[index + 1];
						c = pixels[index + w];
						d = pixels[index + w + 1];

						// blue element
						// Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
						red = (a & 0xff) * p1 + (b & 0xff) * p2 + (c & 0xff) * p3 + (d & 0xff) * p4;

						// green element
						// Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
						green = ((a >> 8) & 0xff)* p1 + ((b >> 8) & 0xff)* p2 + ((c >> 8) & 0xff)* p3 + ((d >> 8) & 0xff)* p4;

						// red element
						// Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
						blue = ((a >> 16) & 0xff)* p1 + ((b >> 16) & 0xff)* p2 + ((c >> 16) & 0xff)* p3 + ((d >> 16) & 0xff)* p4;

						output[offset++] = 0xff000000 | ((((int)red) << 16) & 0xff0000) | ((((int)green) << 8) & 0xff00) | ((int)blue);
					}
				}
		}//);
	}
}

//------------------------------

//-------------------------------------
RawDataSet* GreyScaleSimpleMorphology(RawDataSet* data, int mode, int size, int kernelType, unsigned char* mask0, int inmode, int backmode, int maxIter, int* bounds)
{
	//mask0 - maska - gdy wartosc 1 to ten pixel moge ewentualnie zmieniac
	//backmode - jesli >-1 to tylko te pixele moge zmieniac
	//inmode - jesli >-1 tylko te piksele z otoczenia moga wnosic wklad do zmiany 

	RawDataSet* tData = new RawDataSet();
	tData->Copy(*data, true);

	unsigned short* dataArray = data->GetDataArray();
	unsigned short* tDataArray = tData->GetDataArray();

	int	nx, ny, nz;
	int	nxny;
	nx = data->GetNx();
	ny = data->GetNy();
	nz = data->GetNz();
	nxny = data->GetSliceSize();

	unsigned char cmode = mode;

	int x, y, z;

	int pp;
	bool stop;
	int pos;

	int i, j, k;
	int kernelSize;

	int* kernel = GetGrowingKernel2(kernelSize, size, data, kernelType);
	int tmp;
	int dsizemin1 = data->GetVolumeSize() - 1;

	int chg = 0;

	int ppx[3];

	int val;
	double mBounds[6] = { size,nx - size - 1,size,ny - size - 1,size,nz - size - 1 };

	if (bounds != NULL)
	{
		if (bounds[0] > mBounds[0]) mBounds[0] = bounds[0];
		if (bounds[1] < mBounds[1]) mBounds[1] = bounds[1];
		if (bounds[2] > mBounds[2]) mBounds[2] = bounds[2];
		if (bounds[3] < mBounds[3]) mBounds[3] = bounds[3];
		if (bounds[4] > mBounds[4]) mBounds[4] = bounds[4];
		if (bounds[5] < mBounds[5]) mBounds[5] = bounds[5];

	}


	int count;;

	int l;

	for (l = 0; l < maxIter; l++)
	{
		count = 0;
		for (k = mBounds[4]; k <= mBounds[5]; k++)
		{
			for (j = mBounds[2]; j <= mBounds[3]; j++)
			{
				for (i = mBounds[0]; i <= mBounds[1]; i++)
				{
					pos = nxny * k + nx * j + i;
					val = dataArray[pos];

					if (val != 0)
					{
						i = i;
					}
					if ((mask0 != NULL) && (mask0[pos] == 0)) continue; //gdy jest maska - dzialam tylko na zapalonej masce
					if ((backmode != -1) && (val != backmode)) continue; // gdy jest val - dzialam tylko dla wejsciowych wartosci val

																		 //---------------wwwwwwwwwwwwwwwwwwwwwwwwwwwww---------------------
					if ((inmode == -1)) //zwykle min max
					{
						if (cmode == 1)//dilate
						{
							for (x = 0; x < kernelSize; x++)
							{
								tmp = pos + kernel[x];
								if ((tmp > dsizemin1) || (tmp < 0)) { continue; }
								else
								{
									if (dataArray[tmp] > val)
									{
										val = dataArray[tmp];
									}
								}
							}
						}
						else //erode
						{
							for (x = 0; x < kernelSize; x++)
							{
								tmp = pos + kernel[x];
								if ((tmp > dsizemin1) || (tmp < 0)) { continue; }
								else
								{

									if (dataArray[tmp] < val)
									{
										val = dataArray[tmp];
									}
								}
							}
						}
					}
					else //zwykle min max
					{
						if (cmode == 1)//dilate
						{
							for (x = 0; x < kernelSize; x++)
							{
								tmp = pos + kernel[x];

								if ((tmp > dsizemin1) || (tmp < 0) || (dataArray[tmp] != inmode)) { continue; }
								else { if (dataArray[tmp] > val) val = dataArray[tmp]; }
							}
						}
						else //erode
						{
							for (x = 0; x < kernelSize; x++)
							{
								tmp = pos + kernel[x];
								if ((tmp > dsizemin1) || (tmp < 0) || (dataArray[tmp] != inmode)) { continue; }
								else { if (dataArray[tmp] < val) val = dataArray[tmp]; }
							}
						}
					}

					if (tDataArray[pos] != val) count++;
					tDataArray[pos] = val;

				}
			}
		}
		if (count == 0)
		{
			break;
		}
	}

	delete[]kernel;

	/*if (mask0!=NULL)
	{
	for(i=0;i<data->GetVolumeSize();i++)
	{
	if (mask0[i]==1)
	{
	tDataArray[i]=dataArray[i];
	}
	}
	}*/

	tData->CalculateHistogram();
	return tData;
}

//-----------------------
void SlotBinarySimpleMorphology(RawDataSet* data, unsigned char* mask, int mode, int size, int kernelType, unsigned char* mask0, bool dualMode, bool flipMode, int* bounds, unsigned char* blockMask)
{


	unsigned char* mask2;

	mask2 = (dualMode == false) ? mask : mask0;
	int	nx, ny, nz;
	int	nxny;
	nx = data->GetNx();
	ny = data->GetNy();
	nz = data->GetNz();
	nxny = data->GetSliceSize();
	//bool mode=0;
	unsigned char cmode = mode;
	if (flipMode == true) { if (cmode == 0) cmode = 1; else if (cmode == 1) cmode = 0; };
	int x, y, z;

	int pp;
	bool stop;
	int pos;

	int i, j, k;
	int kernelSize;
	//int kernelType=1;
	//int ssize=1;
	int* kernel = GetGrowingKernel2(kernelSize, size, data, kernelType);
	int tmp;
	int dsizemin1 = data->GetVolumeSize() - 1;
	unsigned short* dataArray = data->GetDataArray();
	int chg = 0;
	/*	if (mask0==NULL)
	{*/

	int ppx[3];
	//unsigned char *nMask=new unsigned char [data->GetVolumeSize()];
	//memcpy(nMask,mask,data->GetVolumeSize());
	double mBounds[6] = { size,nx - size - 1,size,ny - size - 1,size,nz - size - 1 };

	if (bounds != NULL)
	{
		if (bounds[0] > mBounds[0]) mBounds[0] = bounds[0];
		if (bounds[1] < mBounds[1]) mBounds[1] = bounds[1];
		if (bounds[2] > mBounds[2]) mBounds[2] = bounds[2];
		if (bounds[3] < mBounds[3]) mBounds[3] = bounds[3];
		if (bounds[4] > mBounds[4]) mBounds[4] = bounds[4];
		if (bounds[5] < mBounds[5]) mBounds[5] = bounds[5];

	}


	if (blockMask == NULL)
	{
		for (k = mBounds[4]; k <= mBounds[5]; k++)
		{
			for (j = mBounds[2]; j <= mBounds[3]; j++)
			{
				for (i = mBounds[0]; i <= mBounds[1]; i++)
				{
					pos = nxny * k + nx * j + i;
					pp = mask[pos];



					stop = false;
					if ((pp != mode))

						for (x = 0; x < kernelSize; x++)
						{
							tmp = pos + kernel[x];
							if ((tmp > dsizemin1) || (tmp < 0))
							{
								continue;
							}
							else
							{


								if (mask2[tmp] == cmode)
								{
									stop = true;
									break;
								}
							}
						}

					if (stop)
					{
						mask[pos] = 255;
						chg++;
					}
					else
					{
						if (flipMode == true)
						{
							data->GetVoxelXYZPos(tmp, ppx);
							if ((ppx[2] == mBounds[4]) || (ppx[2] == mBounds[5])) stop = true;
							if ((ppx[1] == mBounds[2]) || (ppx[1] == mBounds[3])) stop = true;
							if ((ppx[0] == mBounds[0]) || (ppx[0] == mBounds[1])) stop = true;
						}
					}
					//else mask[pos]=pp;
				}
			}
		}
	}
	else
	{
		for (k = mBounds[4]; k <= mBounds[5]; k++)
		{
			for (j = mBounds[2]; j <= mBounds[3]; j++)
			{
				for (i = mBounds[0]; i <= mBounds[1]; i++)
				{
					pos = nxny * k + nx * j + i;
					pp = mask[pos];



					stop = false;
					if ((pp != mode))

						for (x = 0; x < kernelSize; x++)
						{
							tmp = pos + kernel[x];
							if ((tmp > dsizemin1) || (tmp < 0))
							{
								continue;
							}
							else
							{

								if (blockMask[tmp] == cmode)
								{
									stop = false;
									break;
								}

								if (mask2[tmp] == cmode)
								{
									stop = true;
									//break;
								}

							}
						}

					if (stop)
					{
						mask[pos] = 255;
						chg++;
					}
					else
					{
						if (flipMode == true)
						{
							data->GetVoxelXYZPos(tmp, ppx);
							if ((ppx[2] == mBounds[4]) || (ppx[2] == mBounds[5])) stop = true;
							if ((ppx[1] == mBounds[2]) || (ppx[1] == mBounds[3])) stop = true;
							if ((ppx[0] == mBounds[0]) || (ppx[0] == mBounds[1])) stop = true;
						}
					}
					//else mask[pos]=pp;
				}
			}
		}
	}


	delete[]kernel;
	//memcpy(mask,nMask,data->GetVolumeSize());
	//delete []nMask;
	if (mask0 == NULL)
	{
		for (i = 0; i < data->GetVolumeSize(); i++)
		{
			if (mask[i] == 255)
			{
				mask[i] = mode;
			}
		}
	}
	else
	{
		for (i = 0; i < data->GetVolumeSize(); i++)
		{
			if (mask[i] == 255)
			{
				mask[i] = mode;



			}
			if (mask0[i] == 1)//czyszcze zawsze pixel jesli maska0 byla tam zapalona
			{
				mask[i] = 0;
			}
		}
	}

	//memcpy(mask,nMask,data->GetVolumeSize());
	//delete []nMask;


}
//----------------------


//---------------------------------------------------------------------------------------------











//-------------------------------------------------------------------------------------------






void	SlotBinarySimpleMorphologyWithCriterion2KernelInOut(int pos, RawDataSet* data, unsigned char* mask, int kernelSize, int* kernel, int dsizemin1, unsigned char* blockMask, SegCriterion* cr, int* frontTabOut, int& frontTabOutLength, unsigned char tmpVal, int blockValue)
{



	bool stop;
	unsigned char pp;
	//	unsigned char mode;
	int tmp;

	pp = mask[pos];

	int n[3];
	data->GetN(n);
	//double n1 = 1.0 / n[0];
	int oldI = pos % n[0];

	unsigned char outVal = (tmpVal > 0) ? tmpVal : pp;




	for (int x = 0; x < kernelSize; x++)
	{
		stop = false;
		tmp = pos + kernel[x];
		if ((tmp > dsizemin1) || (tmp < 0))
		{
			continue;
		}
		else
		{
			int newI = tmp % n[0];

			if (abs(newI - oldI) > 1) //przejscie prezz sciane
			{
				continue;
			}

			if ((blockMask != NULL) && (blockMask[tmp] == blockValue)) // 1 bo growing odbywa się po 1 pixelu
			{
				stop = false;
				continue;//?????????????????
			}

			if (mask[tmp] == 0)//nieoznaczony - nie zalany
			{
				stop = true;



			}
		}


		if (stop)
		{
			bool newV = false;
			if (cr != NULL)
			{

				stop = (cr->CheckPoint(0, 0, 0, NULL, 0, tmp) > 0);
				if (stop)//drugi check czy moge zapalic - czy spelnia criterion
				{
					mask[tmp] = outVal;
					newV = true;

				}
			}
			else
			{
				mask[tmp] = outVal;
				newV = true;
			}
			if (newV == true)
			{
				frontTabOut[frontTabOutLength++] = tmp; //mam tablice z nowym frontem - ktory oznaczam źródłem pochodzenia w masce - po wyjscu trzeba bedzie przelabelowac na wyspy podziału
			}


		}



	}




}

//------------------

void	SlotBinarySimpleMorphologyWithCriterion2KernelInOut(int pos, RawDataSet* data, unsigned short* mask, int kernelSize, int* kernel, int dsizemin1, unsigned char* blockMask, SegCriterion* cr, int* frontTabOut, int& frontTabOutLength, unsigned short tmpVal, int blockValue)
{



	bool stop;
	unsigned short pp;
	//	unsigned char mode;
	int tmp;

	pp = mask[pos];

	int n[3];
	data->GetN(n);
	//double n1 = 1.0 / n[0];
	int sliceSize = data->GetSliceSize();

	double ss1 = 1.0 / sliceSize;
	double nx1 = 1.0 / n[0];

	//int oldX = pos%n[0];
	//int oldZ = (pos-oldX )*ss1;
	//int oldY = (pos - oldZ*sliceSize)*nx1;

	;
	/*	int oldZ=pos/sliceSize;
	int oldY=pos- (oldZ* sliceSize)*nx1;
	int oldX=(int)(pos-oldZ* sliceSize)-n[0]*oldY;*/


	/*int oldZ = (int)(pos / sliceSize);
	int oldY = (int)((pos - oldZ * sliceSize) / oldZ);
	int oldX = (int)(pos - p[2] * sliceSize) - o*p[1];
	*/
	unsigned short outVal = (tmpVal > 0) ? tmpVal : pp;


	bool test = ((cr->avoidValueExists == true) && (mask[pos] == cr->avoidValue));

	if (test == false)
		for (int x = 0; x < kernelSize; x++)
		{
			stop = false;
			tmp = pos + kernel[x];
			if ((tmp > dsizemin1) || (tmp < 0))
			{
				continue;
			}
			else
			{
				//int newZ = tmp / sliceSize;
				//int newY = tmp - (newZ* sliceSize)*nx1;
				//int newX = (int)(tmp - newZ * sliceSize) - n[0] *newY;
				//if ((abs(newX - oldX)>1)||(abs(newZ-oldZ)>1) | (abs(newY - oldY)>1)) //przejscie prezz sciane
				//{
				//	continue;
				//}

				if ((blockMask != NULL) && (blockMask[tmp] == blockValue)) // 1 bo growing odbywa się po 1 pixelu
				{
					stop = false;
					continue;//?????????????????
				}

				if (mask[tmp] == 0)//nieoznaczony - nie zalany
				{
					stop = true;



				}
			}


			if (stop)
			{
				bool newV = false;
				if (cr != NULL)
				{

					stop = (cr->CheckPoint(0, 0, 0, NULL, 0, tmp) > 0);
					if (stop)//drugi check czy moge zapalic - czy spelnia criterion
					{
						mask[tmp] = outVal;
						newV = true;

					}
				}
				else
				{
					mask[tmp] = outVal;
					newV = true;
				}
				if (newV == true)
				{
					frontTabOut[frontTabOutLength++] = tmp; //mam tablice z nowym frontem - ktory oznaczam źródłem pochodzenia w masce - po wyjscu trzeba bedzie przelabelowac na wyspy podziału
				}


			}



		}




}



//-------------------------


void 	SlotBinarySimpleMorphologyWithCriterion2InOut(RawDataSet* data, unsigned char* mask,
	int* frontTabIn, int frontTabInLength, int* frontTabOut, int& frontTabOutLength,
	int* kernel, int kernelSize, unsigned char* blockMask, SegCriterion* cr, unsigned char tmpValue, int blockValue)


{





	//data - dane volatile//
	// maska do dylatacji - source i dest
	//mode - 0,1 - erode dilate
	//size - wielkosc kernela
	//kernelType - rodzaj - 0- sq, 1 - circl
	//mask0 - source do sprawdzania - inne niz mask!!!
	//bounds - jak wykryte wczesniej moge biec tylko po obszarze nie calym volatile
	//blockmaask - tego nie moge zmieniac
	//is2D growing kerenel w wersji 3d albo 2d


	int	nx, ny, nz;
	int	nxny;
	nx = data->GetNx();
	ny = data->GetNy();
	nz = data->GetNz();
	nxny = data->GetSliceSize();
	//bool mode=0;




	int pp;
	bool stop;
	int pos;


	//int *kernel = GetGrowingKernel2(kernelSize, size, data, kernelType);
	int tmp;
	int dsizemin1 = data->GetVolumeSize() - 1;
	unsigned short* dataArray = data->GetDataArray();


	int ppx[3];
	int size = 1;
	int p[3];
	int n[3];
	/*data->GetN(n);
	int bound = 3;
	n[0] -= bound;
	n[1] -= bound;
	n[2] -= bound;*/
	frontTabOutLength = 0;
	for (int i = 0; i < frontTabInLength; i++)
	{
		//	data->GetVoxelXYZPos(frontTabIn[i], p);
		//if ((p[0]>bound)&& (p[1]>bound) && (p[2]>bound) )
		{
			//if ((p[0] < n[0]) && (p[1] < n[1]) && (p[2] < n[2]))
			{

				SlotBinarySimpleMorphologyWithCriterion2KernelInOut(frontTabIn[i], data, mask, kernelSize, kernel, dsizemin1, blockMask, cr, frontTabOut, frontTabOutLength, tmpValue, blockValue);
			}
		}
	}

}

//000000000000000000000000

void 	SlotBinarySimpleMorphologyWithCriterion2InOut(RawDataSet* data, unsigned short* mask,
	int* frontTabIn, int frontTabInLength, int* frontTabOut, int& frontTabOutLength,
	int* kernel, int kernelSize, unsigned char* blockMask, SegCriterion* cr, unsigned short tmpValue, int blockValue, int** frontOutParallelTabs, ParentAndAvoidData* parentAndAvoid, int* iterNr)


{

	if (iterNr != NULL)	(*iterNr)++;
	//data - dane volatile//
	// maska do dylatacji - source i dest
	//mode - 0,1 - erode dilate
	//size - wielkosc kernela
	//kernelType - rodzaj - 0- sq, 1 - circl
	//mask0 - source do sprawdzania - inne niz mask!!!
	//bounds - jak wykryte wczesniej moge biec tylko po obszarze nie calym volatile
	//blockmaask - tego nie moge zmieniac
	//is2D growing kerenel w wersji 3d albo 2d


	int	nx, ny, nz;
	int	nxny;
	nx = data->GetNx();
	ny = data->GetNy();
	nz = data->GetNz();
	nxny = data->GetSliceSize();
	//bool mode=0;




	int pp;
	bool stop;
	int pos;


	//int *kernel = GetGrowingKernel2(kernelSize, size, data, kernelType);
	int tmp;
	int dsizemin1 = data->GetVolumeSize() - 1;
	unsigned short* dataArray = data->GetDataArray();


	int ppx[3];
	int p[3];
	int n[3];
	int size = 1;
	data->GetN(n);
	int bound = 1;
	n[0] -= bound;
	n[1] -= bound;
	n[2] -= bound;



	if (frontOutParallelTabs == NULL)
	{
		bool testX, testY, testZ;

		frontTabOutLength = 0;
		for (int i = 0; i < frontTabInLength; i++)
		{
			data->GetVoxelXYZPos(frontTabIn[i], p);

			testX = ((p[0] > bound) && (p[0] < n[0]));
			testY = ((p[1] > bound) && (p[1] < n[1]));
			testZ = (n[2] > 1) ? ((p[2] > bound) && (p[2] < n[2])) : true;

			//	if ((p[0] > bound) && (p[1] > bound) && (p[2] > bound))
			{
				//	if ((p[0] < n[0]) && (p[1] < n[1]) && (p[2] < n[2]))
				if (testX && testY && testZ)
				{
					SlotBinarySimpleMorphologyWithCriterion2KernelInOut(frontTabIn[i], data, mask, kernelSize, kernel, dsizemin1, blockMask, cr, frontTabOut, frontTabOutLength, tmpValue, blockValue);
				}
			}
		}
	}

	else
	{
		int nrOfThreads = 0;
#pragma omp parallel
		{
			nrOfThreads = omp_get_num_threads();
		}
		frontTabOutLength = 0;
		//int nrOfThreads = omp_get_num_threads();
		int* counters = new int[nrOfThreads];
		memset(counters, 0, nrOfThreads * sizeof(int));
		//	qDebug() << "threads: " << nrOfThreads;
#pragma omp parallel
		{
#pragma omp  for  nowait ordered schedule (guided)
			for (int i = 0; i < frontTabInLength; i++)
			{
				int tid = omp_get_thread_num();
				SlotBinarySimpleMorphologyWithCriterion2KernelInOut(frontTabIn[i], data, mask, kernelSize, kernel, dsizemin1, blockMask, cr, frontOutParallelTabs[tid], counters[tid], tmpValue, blockValue);

			}
		}

		int* pointers = new int[nrOfThreads];
		pointers[0] = 0;
		for (int i = 1; i < nrOfThreads; i++)
		{
			pointers[i] = pointers[i - 1] + counters[i - 1];
			frontTabOutLength += counters[i];
		}


#pragma omp parallel
		{
#pragma omp  for  nowait ordered schedule (guided)
			for (int i = 0; i < nrOfThreads; i++)
			{

				int pp = pointers[i];
				int* tab = frontOutParallelTabs[i];
				for (int j = 0; j < counters[i]; j++)
				{
					frontTabOut[j + pp] = tab[j];
				}
				//	frontTabOutLength += counters[i];
			}
		}

		delete[]pointers;
		delete[]counters;


	}
}
//---------------------------------------------------------------------------------------------




void	GetBoundsWithMargin(RawDataSet* data, unsigned char* inputMask, int marginSize, bool is2D, int* bounds)
{

	bounds[0] = INT_MAX;	bounds[1] = INT_MIN;
	bounds[2] = INT_MAX;	bounds[3] = INT_MIN;
	bounds[4] = INT_MAX;	bounds[5] = INT_MIN;


	int ss = (is2D) ? 2 : 3;

	if (is2D == true)
	{
		bounds[4] = 0;
		bounds[5] = 0;
	}


	int n[3];
	data->GetN(n);

	//Find corner of the objectus
	int pos = 0;
	int zz = (is2D) ? 1 : n[2];
	for (int k = 0; k < zz; k++)
		for (int j = 0; j < n[1]; j++)
			for (int i = 0; i < n[0]; i++)
			{
				//Points->GetPoint(Tab[i],Point);
				if (inputMask[pos] > 0)
				{
					if (i > bounds[1]) bounds[1] = i;					if (j > bounds[3]) bounds[3] = j;
					if (i < bounds[0]) bounds[0] = i;					if (j < bounds[2]) bounds[2] = j;
					if (is2D == false)
					{
						if (k < bounds[4]) bounds[4] = k;						if (k > bounds[5]) bounds[5] = k;
					}
				}
				pos++;
			}

	if (marginSize > 0)
	{

		for (int i = 0; i < ss; i++)
		{
			bounds[i * 2] -= marginSize;
			if (bounds[i * 2] < 0) bounds[i * 2] = 0;
		}

		for (int i = 0; i < ss; i++)
		{
			bounds[i * 2 + 1] += marginSize;
			if (bounds[i * 2 + 1] > n[i] - 1) bounds[i * 2 + 1] = n[i] - 1;
		}


	}
}
//---------------------------------------------------------------------------------------------




void	GetBoundsWithMargin(RawDataSet* data, unsigned short* inputMask, int marginSize, bool is2D, int* bounds)
{

	bounds[0] = INT_MAX;	bounds[1] = INT_MIN;
	bounds[2] = INT_MAX;	bounds[3] = INT_MIN;
	bounds[4] = INT_MAX;	bounds[5] = INT_MIN;
	/*if (is2D == true)
	{
		bounds[4] = 0;
		bounds[5] = 0;
	}*/

	int ss = (is2D) ? 2 : 3;

	


	int n[3];
	data->GetN(n);

	//Find corner of the objectus
	int pos = 0;
	int zz = n[2];;//(is2D) ? 1 : n[2];
	for (int k = 0; k < zz; k++)
		for (int j = 0; j < n[1]; j++)
			for (int i = 0; i < n[0]; i++)
			{
				//Points->GetPoint(Tab[i],Point);
				if (inputMask[pos] > 0)
				{
					if (i > bounds[1]) bounds[1] = i;					if (j > bounds[3]) bounds[3] = j;
					if (i < bounds[0]) bounds[0] = i;					if (j < bounds[2]) bounds[2] = j;
					//if (is2D == false)
					{
						if (k < bounds[4]) bounds[4] = k;						if (k > bounds[5]) bounds[5] = k;
					}
				}
				pos++;
			}


	if (marginSize > 0)
	{

		for (int i = 0; i < ss; i++)
		{
			bounds[i * 2] -= marginSize;
			if (bounds[i * 2] < 0) bounds[i * 2] = 0;
		}

		for (int i = 0; i < ss; i++)
		{
			bounds[i * 2 + 1] += marginSize;
			if (bounds[i * 2 + 1] > n[i] - 1) bounds[i * 2 + 1] = n[i] - 1;
		}


	}


}
//---------------------------------------------------------------

void SlotBinarySimpleMorphologyWithCriterion2Kernel(int pos, unsigned char* mask, unsigned char mode, int kernelSize, int* kernel, int dsizemin1, unsigned char* blockMask, unsigned char* mask2, SegCriterion* cr, int blockVal)
{


	bool stop;
	unsigned char pp;
	//	unsigned char mode;
	int tmp;

	pp = mask[pos];



	stop = false;
	if ((pp != mode) && (pp != 255))

		for (int x = 0; x < kernelSize; x++)
		{
			tmp = pos + kernel[x];
			if ((tmp > dsizemin1) || (tmp < 0))
			{
				continue;
			}
			else
			{

				if ((blockMask != NULL) && (blockMask[tmp] == blockVal))
				{
					stop = false;
					break;//?????????????????
				}

				if (mask2[tmp] == mode)
				{
					stop = true;
					break;


				}
			}
		}

	if (stop)
	{
		if (cr != NULL)
		{

			stop = (cr->CheckPoint(0, 0, 0, NULL, 0, tmp) > 0);
			if (stop)//drugi check czy moge zapalic - czy spelnia criterion
			{
				mask[pos] = 255;

			}
		}
		else
		{
			mask[pos] = 255;

		}
	}


}
//------------

void SlotBinarySimpleMorphologyWithCriterion2Kernel(int pos, unsigned short* mask, unsigned char mode, int kernelSize, int* kernel, int dsizemin1, unsigned char* blockMask, unsigned short* mask2, SegCriterion* cr, int blockVal)
{


	bool stop;
	unsigned short pp;
	//	unsigned char mode;
	int tmp;

	pp = mask[pos];



	stop = false;
	if ((pp != mode))

		for (int x = 0; x < kernelSize; x++)
		{
			tmp = pos + kernel[x];
			if ((tmp > dsizemin1) || (tmp < 0))
			{
				continue;
			}
			else
			{

				if ((blockMask != NULL) && (blockMask[tmp] == blockVal))
				{
					stop = false;
					break;//?????????????????
				}

				if (mask[tmp] == mode)
				{
					stop = true;
					break;


				}
			}
		}

	if (stop)
	{
		if (cr != NULL)
		{

			stop = (cr->CheckPoint(0, 0, 0, NULL, 0, tmp) > 0);
			if (stop)//drugi check czy moge zapalic - czy spelnia criterion
			{
				mask[pos] = 255;

			}
		}
		else
		{
			mask[pos] = 255;

		}
	}


}

//--------------




void SlotBinarySimpleMorphologyWithCriterion2(RawDataSet* data, unsigned char* mask, int mode, int size, int kernelType,
	unsigned char* mask0, int* bounds, unsigned char* blockMask, SegCriterion* cr, bool is2D, int backVal, int blockValue)
{

	//parallel Version!!!!!



	//data - dane volatile//
	// maska do dylatacji - source i dest
	//mode - 0,1 - erode dilate
	//size - wielkosc kernela
	//kernelType - rodzaj - 0- sq, 1 - circl
	//mask0 - source do sprawdzania - inne niz mask!!!
	//bounds - jak wykryte wczesniej moge biec tylko po obszarze nie calym volatile
	//blockmaask - tego nie moge zmieniac
	//is2D growing kerenel w wersji 3d albo 2d

	if (backVal == -1) backVal = mode;//na co zamieniam zmienione pixele
	unsigned char* mask2;

	mask2 = (mask0 == NULL) ? mask : mask0;
	int	nx, ny, nz;
	int	nxny;
	nx = data->GetNx();
	ny = data->GetNy();
	nz = data->GetNz();
	nxny = data->GetSliceSize();
	//bool mode=0;




	int pp;
	bool stop;
	int pos;


	int kernelSize;
	int* kernel = NULL;
	if (!is2D)
		kernel = GetGrowingKernel(kernelSize, size, data, kernelType);
	else
		kernel = GetGrowingKernel2D(kernelSize, size, data, kernelType);


	//int *kernel = GetGrowingKernel2(kernelSize, size, data, kernelType);
	int tmp;
	int dsizemin1 = data->GetVolumeSize() - 1;
	unsigned short* dataArray = data->GetDataArray();


	int ppx[3];

	int mBounds[6] = { size, nx - size - 1, size, ny - size - 1, size, nz - size - 1 };

	if (is2D)
	{
		mBounds[4] = 0;
		mBounds[5] = 0;
	}

	if (bounds != NULL)
	{
		if (bounds[0] > mBounds[0]) mBounds[0] = bounds[0];
		if (bounds[1] < mBounds[1]) mBounds[1] = bounds[1];
		if (bounds[2] > mBounds[2]) mBounds[2] = bounds[2];
		if (bounds[3] < mBounds[3]) mBounds[3] = bounds[3];
		if (bounds[4] > mBounds[4]) mBounds[4] = bounds[4];
		if (bounds[5] < mBounds[5]) mBounds[5] = bounds[5];

	}



#pragma omp parallel
	{
		int nrOfThreads = omp_get_num_threads();
		//	qDebug() << "threads: " << nrOfThreads;

#pragma omp  for  nowait ordered schedule (guided)
		for (int k = mBounds[4]; k <= mBounds[5]; k++)
		{
			for (int j = mBounds[2]; j <= mBounds[3]; j++)
			{
				for (int i = mBounds[0]; i <= mBounds[1]; i++)
				{
					int posa = nxny * k + nx * j + i;
					SlotBinarySimpleMorphologyWithCriterion2Kernel(posa, mask, mode, kernelSize, kernel, dsizemin1, blockMask, mask2, cr, blockValue);

				}
			}
		}

	}

	delete[]kernel;


	for (int i = 0; i < data->GetVolumeSize(); i++)
	{
		if (mask[i] == 255)
		{
			mask[i] = backVal;
		}
	}

}
//----------------------



void SlotBinarySimpleMorphologyWithCriterion2(RawDataSet* data, unsigned short* mask, int mode, int size, int kernelType,
	unsigned short* mask0, int* bounds, unsigned char* blockMask, SegCriterion* cr, bool is2D, int backVal, int blockValue)
{

	//parallel Version!!!!!



	//data - dane volatile//
	// maska do dylatacji - source i dest
	//mode - 0,1 - erode dilate
	//size - wielkosc kernela
	//kernelType - rodzaj - 0- sq, 1 - circl
	//mask0 - source do sprawdzania - inne niz mask!!!
	//bounds - jak wykryte wczesniej moge biec tylko po obszarze nie calym volatile
	//blockmaask - tego nie moge zmieniac
	//is2D growing kerenel w wersji 3d albo 2d

	if (backVal == -1) backVal = mode;//na co zamieniam zmienione pixele
	unsigned short* mask2;

	mask2 = (mask0 == NULL) ? mask : mask0;
	int	nx, ny, nz;
	int	nxny;
	nx = data->GetNx();
	ny = data->GetNy();
	nz = data->GetNz();
	nxny = data->GetSliceSize();
	//bool mode=0;




	int pp;
	bool stop;
	int pos;


	int kernelSize;
	int* kernel = NULL;
	if (!is2D)
		kernel = GetGrowingKernel(kernelSize, size, data, kernelType);
	else
		kernel = GetGrowingKernel2D(kernelSize, size, data, kernelType);


	//int *kernel = GetGrowingKernel2(kernelSize, size, data, kernelType);
	int tmp;
	int dsizemin1 = data->GetVolumeSize() - 1;
	unsigned short* dataArray = data->GetDataArray();


	int ppx[3];

	int mBounds[6] = { size, nx - size - 1, size, ny - size - 1, size, nz - size - 1 };

	if (is2D)
	{
		mBounds[4] = 0;
		mBounds[5] = nz-1;
	}

	if (bounds != NULL)
	{
		if (bounds[0] > mBounds[0]) mBounds[0] = bounds[0];
		if (bounds[1] < mBounds[1]) mBounds[1] = bounds[1];
		if (bounds[2] > mBounds[2]) mBounds[2] = bounds[2];
		if (bounds[3] < mBounds[3]) mBounds[3] = bounds[3];
		if (bounds[4] > mBounds[4]) mBounds[4] = bounds[4];
		if (bounds[5] < mBounds[5]) mBounds[5] = bounds[5];

	}



#pragma omp parallel
	{
		int nrOfThreads = omp_get_num_threads();
		//	qDebug() << "threads: " << nrOfThreads;

#pragma omp  for  nowait ordered schedule (guided)
		for (int k = mBounds[4]; k <= mBounds[5]; k++)
		{
			for (int j = mBounds[2]; j <= mBounds[3]; j++)
			{
				for (int i = mBounds[0]; i <= mBounds[1]; i++)
				{
					int posa = nxny * k + nx * j + i;
					SlotBinarySimpleMorphologyWithCriterion2Kernel(posa, mask, mode, kernelSize, kernel, dsizemin1, blockMask, mask2, cr, blockValue);

				}
			}
		}

	}

	delete[]kernel;

	int countAdd = 0;
	for (int i = 0; i < data->GetVolumeSize(); i++)
	{
		if (mask[i] == 255)
		{
			countAdd++;
			mask[i] = backVal;
		}
	}
	qDebug() << "dilate add: " << countAdd;

}



//------------------------------------------------------------------------------------



void SlotBinarySimpleMorphologyWithCriterion(RawDataSet* data, unsigned char* mask, int mode, int size, int kernelType,
	unsigned char* mask0, int* bounds, unsigned char* blockMask, SegCriterion* cr)
{

	//data - dane volatile//
	// maska do dylatacji - source i dest
	//mode - 0,1 - erode dilate
	//size - wielkosc kernela
	//kernelType - rodzaj - 0- sq, 1 - circl
	//mask0 - source do sprawdzania - inne niz mask!!!
	//bounds - jak wykryte wczesniej moge biec tylko po obszarze nie calym volatile
	//blockmaask - tego nie moge zmieniac

	unsigned char* mask2;

	mask2 = (mask0 == NULL) ? mask : mask0;
	int	nx, ny, nz;
	int	nxny;
	nx = data->GetNx();
	ny = data->GetNy();
	nz = data->GetNz();
	nxny = data->GetSliceSize();
	//bool mode=0;


	int x, y, z;

	int pp;
	bool stop;
	int pos;

	int i, j, k;
	int kernelSize;
	//int kernelType=1;
	//int ssize=1;
	int* kernel = GetGrowingKernel2(kernelSize, size, data, kernelType);
	int tmp;
	int dsizemin1 = data->GetVolumeSize() - 1;
	unsigned short* dataArray = data->GetDataArray();
	int chg = 0;
	/*	if (mask0==NULL)
	{*/

	int ppx[3];
	//unsigned char *nMask=new unsigned char [data->GetVolumeSize()];
	//memcpy(nMask,mask,data->GetVolumeSize());
	double mBounds[6] = { size, nx - size - 1, size, ny - size - 1, size, nz - size - 1 };

	if (bounds != NULL)
	{
		if (bounds[0] > mBounds[0]) mBounds[0] = bounds[0];
		if (bounds[1] < mBounds[1]) mBounds[1] = bounds[1];
		if (bounds[2] > mBounds[2]) mBounds[2] = bounds[2];
		if (bounds[3] < mBounds[3]) mBounds[3] = bounds[3];
		if (bounds[4] > mBounds[4]) mBounds[4] = bounds[4];
		if (bounds[5] < mBounds[5]) mBounds[5] = bounds[5];

	}



	for (k = mBounds[4]; k <= mBounds[5]; k++)
	{
		for (j = mBounds[2]; j <= mBounds[3]; j++)
		{
			for (i = mBounds[0]; i <= mBounds[1]; i++)
			{
				pos = nxny * k + nx * j + i;
				pp = mask[pos];



				stop = false;
				if ((pp != mode))

					for (x = 0; x < kernelSize; x++)
					{
						tmp = pos + kernel[x];
						if ((tmp > dsizemin1) || (tmp < 0))
						{
							continue;
						}
						else
						{

							if ((blockMask != NULL) && (blockMask[tmp] > 0))
							{
								stop = false;
								break;//?????????????????
							}

							if (mask2[tmp] == mode)
							{
								stop = true;
								break;

								//if (stop==true)break;
							}
						}
					}

				if (stop)
				{
					if (cr != NULL)
					{

						stop = (cr->CheckPoint(0, 0, 0, NULL, 0, tmp) > 0);
						if (stop)//drugi check czy moge zapalic - czy spelnia criterion
						{
							mask[pos] = 255;
							chg++;
						}
					}
					else
					{
						mask[pos] = 255;
						chg++;
					}
				}

			}
		}
	}



	delete[]kernel;


	for (i = 0; i < data->GetVolumeSize(); i++)
	{
		if (mask[i] == 255)
		{
			mask[i] = mode;
		}
	}

	if (blockMask != NULL)
	{
		for (i = 0; i < data->GetVolumeSize(); i++)
		{

			if (blockMask[i] > 0)
			{
				mask[i] = 0; //nie wiem czy kiedykowliek prawda
			}
		}
	}





}
//----------------------
int  SlotFillMask2(RawDataSet* Data1, unsigned char* Data2, int x, int y, int z, int endValue, int _KernelType, int* _bounds, unsigned char* mask2, double* _center, double* centerTab)
{
	if (mask2 == NULL) mask2 = Data2;

	int width = Data1->GetNx();
	int height = Data1->GetNy();
	int nslice = Data1->GetNz();

	int sliceSize = width * height;
	int dataSize = sliceSize * nslice;
	unsigned char* mask = Data2;
	int startQ = x + y * width + z * sliceSize;

	//unsigned short *dstArray=Data1->GetDataArray();

	int Size = (int)((double)dataSize * 0.2);
	int Step = (int)((double)dataSize * 0.05);

	int* list = new int[Size];
	int* listtemp;



	//	unsigned short startVal = data[startQ];
	int lp = 0, ap = 0;
	list[lp] = startQ;
	int startValue = mask[startQ];
	mask2[list[lp]] = endValue;

	int dsizemin1 = dataSize - 1;


	//int step[13];
	int KernelSize;


	// TYPE - 0 - 3dcube, 1 - 3dcross, 2 - 3dplus 
	// TYPE - 3 - 2dcube, 4 - 2dcross, 5 - 2dplus



	//GetGrowingKernel2(KernelSize, step, View, _KernelType, width, sliceSize);
	int* kernel = GetGrowingKernel2(KernelSize, 1, Data1, _KernelType);
	//	int k = 0;

	int tmp;
	int tmp2;

	int i;
	int Ker2 = 2 * KernelSize;
	int count = 0;

	int bounds[6] = { 10000,0,10000,0,10000,0 };
	int pos[3];
	double center[3] = { 0,0,0 };
	double c[3];
	double origin[3];
	double scale[3];
	Data1->GetOrigin(origin);
	Data1->GetScale(scale);
	do
	{


		if (lp >= Size - Ker2)
		{
			tmp2 = (Size + Step < dataSize) ? Size + Step : dataSize;
			listtemp = new int[tmp2];
			memcpy(listtemp, list, sizeof(int) * (Size));
			Size += Step;
			delete[]list;
			list = listtemp;
			listtemp = NULL;

		}


		ap = list[lp--];

		//do przodu
		for (i = 0; i < KernelSize; i++)
		{
			tmp = ap + kernel[i];
			if ((tmp > dsizemin1) || (tmp < 0))
			{
				continue;
			}
			else
			{
				if ((mask[tmp] == startValue) && (mask2[tmp] != endValue))
				{
					mask2[tmp] = endValue;
					list[++lp] = tmp;



					if (_bounds != NULL)
					{
						Data1->GetVoxelXYZPos(tmp, pos);
						if (pos[0] < bounds[0]) bounds[0] = pos[0]; if (pos[0] > bounds[1]) bounds[1] = pos[0];
						if (pos[1] < bounds[2]) bounds[2] = pos[1]; if (pos[1] > bounds[3]) bounds[3] = pos[1];
						if (pos[2] < bounds[4]) bounds[4] = pos[2]; if (pos[2] > bounds[5]) bounds[5] = pos[2];
					}
					if (_center != NULL)
					{
						c[0] = pos[0] * scale[0] + origin[0];
						c[1] = pos[1] * scale[1] + origin[1];
						c[2] = pos[2] * scale[2] + origin[2];
						center[0] += c[0];
						center[1] += c[1];
						center[2] += c[2];

						if (centerTab != NULL)
						{
							centerTab[count * 3] = c[0];
							centerTab[count * 3 + 1] = c[1];
							centerTab[count * 3 + 2] = c[2];
						}
					}

					count++;
				}
			}
		}


	} while (lp >= 0);
	if (_bounds != NULL)
	{
		for (i = 0; i < 6; i++)
		{
			_bounds[i] = bounds[i];
		}
	}

	if ((_center != NULL) && (count > 0))
	{
		_center[0] = center[0] / count;
		_center[1] = center[1] / count;
		_center[2] = center[2] / count;
		if (centerTab != NULL)
		{
			double dist = 0;
			double minDist = INT_MAX;
			int minPos = 0;
			for (i = 0; i < count; i++)
			{
				dist = GetDistance(centerTab + i * 3, _center);
				if (dist < minDist)
				{
					minPos = i;
					minDist = dist;
				}
			}

			_center[0] = centerTab[minPos * 3];
			_center[1] = centerTab[minPos * 3 + 1];
			_center[2] = centerTab[minPos * 3 + 2];

		}

	}

	delete[]kernel;
	delete[]list;
	return count;


}
//--------------------------------------------------
int SlotFillMaskFromRawData(RawDataSet* data, unsigned char* mask, int endVal, int x, int y, int z, int _KernelType, int* _bounds, unsigned char* secondMask, int secondVal)
{


	int width = data->GetNx();
	int height = data->GetNy();
	int nslice = data->GetNz();

	int sliceSize = width * height;
	int dataSize = sliceSize * nslice;

	int startQ = x + y * width + z * sliceSize;

	unsigned short* dataArray = data->GetDataArray();
	int startVal = dataArray[startQ];
	int Size = (int)((double)dataSize * 0.2);
	int Step = (int)((double)dataSize * 0.05);

	int* list = new int[Size];
	int* listtemp;



	//	unsigned short startVal = data[startQ];
	int lp = 0, ap = 0;
	list[lp] = startQ;
	int startValue = dataArray[startQ];
	mask[list[lp]] = endVal;
	if (secondMask != NULL)
	{
		secondMask[list[lp]] = secondVal;
	}
	int dsizemin1 = dataSize - 1;


	//int step[13];
	int KernelSize;


	// TYPE - 0 - 3dcube, 1 - 3dcross, 2 - 3dplus 
	// TYPE - 3 - 2dcube, 4 - 2dcross, 5 - 2dplus



	//GetGrowingKernel2(KernelSize, step, View, _KernelType, width, sliceSize);
	int* kernel = GetGrowingKernel2(KernelSize, 1, data, _KernelType);
	//	int k = 0;

	int tmp;
	int tmp2;

	int i;
	int Ker2 = 2 * KernelSize;
	int count = 0;

	int bounds[6] = { 10000,0,10000,0,10000,0 };
	int pos[3];
	double center[3] = { 0,0,0 };
	double c[3];
	double origin[3];
	double scale[3];
	data->GetOrigin(origin);
	data->GetScale(scale);
	do
	{


		if (lp >= Size - Ker2)
		{
			tmp2 = (Size + Step < dataSize) ? Size + Step : dataSize;
			listtemp = new int[tmp2];
			memcpy(listtemp, list, sizeof(int) * (Size));
			Size += Step;
			delete[]list;
			list = listtemp;
			listtemp = NULL;

		}


		ap = list[lp--];

		//do przodu
		for (i = 0; i < KernelSize; i++)
		{
			tmp = ap + kernel[i];
			if ((tmp > dsizemin1) || (tmp < 0))
			{
				continue;
			}
			else
			{
				if ((dataArray[tmp] == startValue) && (mask[tmp] != endVal))
				{
					mask[tmp] = endVal;
					list[++lp] = tmp;

					if (secondMask != NULL)
					{
						secondMask[tmp] = secondVal;
					}
					if (_bounds != NULL)
					{
						data->GetVoxelXYZPos(tmp, pos);
						if (pos[0] < bounds[0]) bounds[0] = pos[0]; if (pos[0] > bounds[1]) bounds[1] = pos[0];
						if (pos[1] < bounds[2]) bounds[2] = pos[1]; if (pos[1] > bounds[3]) bounds[3] = pos[1];
						if (pos[2] < bounds[4]) bounds[4] = pos[2]; if (pos[2] > bounds[5]) bounds[5] = pos[2];
					}
					count++;
				}
			}
		}


	} while (lp >= 0);

	if (_bounds != NULL)
	{
		for (i = 0; i < 6; i++)
		{
			_bounds[i] = bounds[i];
		}
	}





	delete[]kernel;
	delete[]list;
	return count;


}
////--------------------------------------------------

void DrawJointHistogram(int bins, int* scatter, int startX, int startY, int width, int height, bool logScale, QColor backC, QColor frontC, QImage* visImage, double multiply)
{
	if ((scatter == NULL) || (visImage == NULL) || (startY > 255) || (startX > 255) || (width < 2) || (height < 2)) return;
	if (bins > 8)bins = 8;
	if (bins < 2)bins = 2;
	bins = pow((double)2, bins);
	int i, j;

	int scatterSize = bins;
	double* scatterTab2 = new double[scatterSize * scatterSize];
	int bb;
	double max = 0;
	int pos;

	for (i = 0; i < scatterSize * scatterSize; i++)
	{

		bb = scatter[i];
		if (bb > 0)
			scatterTab2[i] = log((double)bb);
		else
			scatterTab2[i] = 0;

		if (scatterTab2[i] > max) max = scatterTab2[i];
	}

	double scaleFactor = (double)(255) / (double)max;


	unsigned char* dataX;
	dataX = (unsigned char*)visImage->bits();
	int x = 0;
	unsigned char col;
	unsigned char xa;
	unsigned char ya;

	if (startY + height > 256)	height = 256 - startY;
	if (startX + width > 256)	width = 256 - startX;

	double scaleX = (scatterSize - 1) / (double)(width - 1);
	double scaleY = (scatterSize - 1) / (double)(height - 1);

	int stopY = height + startY;
	int stopX = width + startX;
	dataX += (startY * 256 + startX) * 4;
	int val;
	for (j = startY; j < stopY; j++)
	{
		for (i = startX; i < stopX; i++)
		{

			xa = (i - startX) * scaleX;
			ya = (j - startY) * scaleY;
			//col=Round((double)scatterTab2[x++]*scaleFactor);
			val = scatterTab2[xa + scatterSize * ya] * scaleFactor * multiply;
			col = (val > 255) ? 255 : val;

			*(dataX++) = col;
			*(dataX++) = col;
			*(dataX++) = col;
			dataX++;
		}
		dataX += (256 - width) * 4;
	}

	delete[]scatterTab2;


}
//---------------------------------------
void	GetHistogram(unsigned short *inuptTab,int inputSize,int* hist, int nrOfBins, int minVal, int maxVal, bool clear)
{
	if ((inuptTab != NULL) && (hist != NULL)&&(maxVal-minVal>1))
	{
		if (clear == true)
			memset(hist, 0, nrOfBins * sizeof(int));

		int range = maxVal - minVal;
		double scaleFactor = (double)(nrOfBins - 1) / range;
		for (int i = 0; i < inputSize; i++)
		{
			int val = inuptTab[i];
			if (val < minVal) val = minVal;
			else if (val > maxVal) val = maxVal;
			val -= minVal;
			
			int hVal = val * scaleFactor;
			hist[hVal]++;
		}

	}

}

//--------------------------------------------
void	DrawHistogram(int bins, int* hist, int startX, int startY, int width, int height, bool logScale, QColor backC, QColor frontC,
	QImage* visImage, bool rotate90, bool inv, bool clear, int* minMax, int zeroLevel, QColor zeroColor, QVector<int>* circlePos, QVector <int>* hlines, QVector<QColor>* hColors)
{
	if ((hist == NULL) || (visImage == NULL))return;

	if (bins > 8)bins = 8;
	if (bins < 2)bins = 2;
	bins = pow((double)2, bins);
	int histMax = 0;
	//int startY=0;
	//int height=128;
	double vLog;
	//int *hist=hist1;
	if (inv == true)
	{
		QColor colTmp = backC;
		backC = frontC;
		frontC = colTmp;
	}

	if (minMax == NULL)
	{
		for (int i = 0; i < bins; i++)
		{
			if (hist[i] > histMax) histMax = hist[i];
		}
	}
	else
	{
		histMax = minMax[1];
	}

	if (histMax > 0)
	{
		double hMaxLog = (logScale) ? log(1 + (double)(histMax)) * 100 : histMax;


		int val;
		QPainter p(visImage);

		if (clear == true)
			p.fillRect(QRect(0, 0, 256, 256), backC);

		int prevX, prevY;
		int newX, newY;

		if (rotate90 == false)
		{
			int iPos;

			double scale = (double)height / hMaxLog;
			double scaleX = (double)width / (bins - 1);




			p.fillRect(QRect(startX, startY, width, height), backC);




			int zeroLevelNew;
			zeroLevelNew = startY + (height - zeroLevel * scale);// startY - zeroLevel*scale;


			for (int i = 0; i < bins; i++)
			{
				if (hist[i] >= 0)
				{
					if (i > 0)
					{
						prevX = newX;
						prevY = newY;
					}

					iPos = (int)(i * scaleX);
					vLog = (logScale) ? log(1 + (double)(hist[i])) * 100 : hist[i];

					val = vLog * scale;//ma byc z zakresu 0-128
					if (inv == true) val = height - val;
					//p.setPen(backC);

				//	p.drawLine(startX + i, startY, startX + i, startY + (height - val));
					p.setPen(frontC);
					//if (hist[i]>=0)p.drawLine(startX + i, startY + height - val + 1, startX + i, zeroLevelNew );


					newX = startX + iPos;
					newY = startY + height - val + 1;


					//p.drawLine(newX, newY, startX + i, zeroLevelNew);

					if (i > 0)
					{
						//p.drawLine(newX, newY, prevX, prevY);
						p.fillRect(QRect(prevX, zeroLevelNew, newX - prevX, newY - zeroLevelNew), frontC);

					}
				}
			}
			if (zeroLevel >= 0)
			{
				p.setPen(zeroColor);
				p.drawLine(0, zeroLevelNew, width, zeroLevelNew);
			}

			if ((hlines != NULL) && (hColors != NULL))
			{

				for (int i = 0; i < hlines->count(); i++)
				{
					int pos = hlines->at(i);
					int level = startY + (height - pos * scale) - 1;
					p.setPen(hColors->at(i));
					p.drawLine(0, level, width, level);
				}
			}




			if (circlePos != NULL)
			{
				//QBrush b3(zeroColor);
				int radius = 3;
				for (int j = 0; j < circlePos->count(); j++)
				{
					int i = circlePos->at(j);
					iPos = (int)(i * scaleX);
					vLog = (logScale) ? log(1 + (double)(hist[i])) * 100 : hist[i];

					val = vLog * scale;//ma byc z zakresu 0-128
					if (inv == true) val = height - val;
					newX = startX + iPos;
					newY = startY + height - val + 1;


					QPen pen(zeroColor);
					pen.setWidth(3);

					p.setPen(pen);
					//	p.setBrush(b3);
					p.drawEllipse(QPoint(newX, newY), radius, radius);
				}

			}

		}
		else
		{
			int jPos;
			double scale = (double)width / hMaxLog;
			double scaleY = (bins - 1) / (double)height;
			for (int j = 0; j < height; j++)
			{
				jPos = (int)(j * scaleY);
				vLog = (logScale) ? log(1 + (double)(hist[jPos])) * 100 : hist[jPos];

				val = vLog * scale;//ma byc z zakresu 0-128
				if (inv == true) val = width - val;
				p.setPen(backC);

				p.drawLine(startX, startY + j, startX + (width - val), startY + j);
				p.setPen(frontC);
				p.drawLine(startX + width - val + 1, startY + j, startX + width, startY + j);
			}
		}
		p.setPen(Qt::lightGray);
		p.drawRect(startX, startY, width, height);
	}
}
//------------------------------------
//bool Update2DStats(int bins, int *hist1, int *hist2, int *scatter, double *statsScatter, QPolygonF poly, int nx, int ny,
//	unsigned short *tab1, unsigned short *tab2, int **tmpTab, int &count, ImageShapes		*shapes, int maxValue1, int  maxValue2,
//
//	bool useBiggerMax, bool useThresholds, double leftThD, double rightThD, bool invThresh, bool showReg1, bool showReg2)
//{
//	if ((hist1 == NULL) || (hist2 == NULL) || (scatter == NULL) || (statsScatter == NULL) || (tmpTab[0] == NULL) || (tmpTab[1] == NULL)) return false;
//	if (bins>8)bins = 8;
//	if (bins<2)bins = 2;
//	bins = pow((double)2, bins);
//
//	QRectF rect = poly.boundingRect();
//	qreal x1, x2, y1, y2;
//	rect.getCoords(&x1, &y1, &x2, &y2);
//	int X1 = (int)x1;
//	int Y1 = (int)y1;
//	int X2 = (int)x2 + 1;
//	int Y2 = (int)y2 + 1;
//
//
//	int NX = x2 - x1;
//	int NY = y2 - y1;
//
//	int sliceSize = nx*ny;
//
//	QImage img(nx, ny, QImage::Format_RGB32);
//	img.fill(Qt::black);
//	QPainter Painter2(&img);
//	Painter2.setBrush(Qt::white);
//	Painter2.setPen(1);
//	Painter2.drawPolygon(poly);
//
//	int i, j;
//	bool test;
//	int pos;
//	unsigned char *tab11 = img.bits();
//	unsigned int *tab1x = (unsigned int *)tab11;
//
//	memset(hist1, 0, bins*sizeof(int));
//	//memset (stats1,0,16*sizeof(double));
//	memset(hist2, 0, bins*sizeof(int));
//	//memset (stats2,0,16*sizeof(double));
//
//	memset(scatter, 0, bins*bins*sizeof(int));
//	memset(statsScatter, 0, 16 * sizeof(double));
//
//
//
//	double stddev;
//	int  		sum1 = 0;
//	int			sum2 = 0;
//	long int 	sum12 = 0;
//	long int	sum22 = 0;
//	count = 0;
//	unsigned char val1, val2;
//	unsigned char min1 = bins;
//	unsigned char min2 = bins;
//	unsigned char max1 = 0, max2 = 0;
//
//	int *realTab = tmpTab[0];
//
//
//	//findMAx
//
//	double scale1, scale2;
//	double leftTh1, leftTh2;
//	double rightTh1, rightTh2;
//	if (useBiggerMax == false)
//	{
//
//
//		scale1 = (maxValue1>0) ? (double)(bins - 1) / maxValue1 : 1.0;
//		scale2 = (maxValue2>0) ? (double)(bins - 1) / maxValue2 : 1.0;
//
//		leftTh1 = leftThD*maxValue1;
//		rightTh1 = rightThD*maxValue1;
//		leftTh2 = leftThD*maxValue2;
//		rightTh2 = rightThD*maxValue2;
//
//	}
//	else
//	{
//		unsigned short  biggerMaks = MAX(maxValue1, maxValue2);
//		scale1 = (biggerMaks>0) ? (double)(bins - 1) / biggerMaks : 1.0;
//		scale2 = (biggerMaks>0) ? (double)(bins - 1) / biggerMaks : 1.0;
//
//		leftTh1 = leftThD*biggerMaks;
//		rightTh1 = rightThD*biggerMaks;
//		leftTh2 = leftThD*biggerMaks;
//		rightTh2 = rightThD*biggerMaks;
//	}
//
//	double bins1 = 1.0 / (bins - 1);
//
//	//bool useBiggerMax,bool useThresholds,unsigned short leftTh,unsigned short rightTh,bool invThresh
//
//
//	/*
//	double leftTh1=leftThD*maxValue1;
//	double rightTh1=rightThD*maxValue1;
//
//
//	double leftTh2=leftThD*maxValue2;
//	double rightTh2=rightThD*maxValue2;*/
//
//	double floatEnergy = 0;
//	double tmpE;
//	double tmpE1, tmpE2;
//	tmpE1 = 0;
//	tmpE2 = 0;
//	tmpE = 0;
//	double ee;
//	double ssd = 0;
//	for (j = y1; j <= y2; j++)
//	{
//		for (i = x1; i <= x2; i++)
//		{
//			pos = i + j*nx;;
//			test = (((tab1x[pos]) == 0xffffffff));
//			if ((test == true))
//			{
//				//dobry pixel - jego wartosc bierzemy!
//				val1 = tab1[pos] * scale1; //rozciagam histogram na zakres 0-255;
//				val2 = tab2[pos] * scale2; //rozciagam histogram na zakres 0-255;
//
//				//test
//				bool test1 = true;
//				bool test2 = true;
//
//
//				//sprawdzam czy pomiedzy progami
//				if (useThresholds == true)
//				{
//					if (((tab1[pos] >= leftTh1) && (tab1[pos] <= rightTh1)) == invThresh)
//						test1 = false;
//				}
//				if (test1 == true)
//				{
//
//					if (val1>max1) max1 = val1; if (val1<min1) min1 = val1;
//					hist1[val1]++;
//					sum1 += val1;
//					sum12 += val1*val1;
//				}
//
//
//				//sprawdzam czy pomiedzy progami 
//				if (useThresholds == true)
//				{
//					if (((tab2[pos] >= leftTh2) && (tab2[pos] <= rightTh2)) == invThresh)
//						test2 = false;
//				}
//
//				if (test2 == true)
//				{
//					if (val2>max2) max2 = val2; if (val2<min2) min2 = val2;
//					hist2[val2]++;
//					sum2 += val2;
//					sum22 += val2*val2;
//				}
//
//
//
//				if ((test1 == true) && (test2 == true))
//				{
//					scatter[val1*bins + val2]++;
//					realTab[count] = pos;
//
//					ee = (val1 - val2)*bins1;
//					ssd += ee*ee;
//					tmpE += (val1 - val2)*(val1 - val2)*0.0001;
//					tmpE1 += val1*val1*0.0001;
//					tmpE2 += (val2*val2)*0.0001;
//
//					count++;
//
//				}
//
//
//
//
//
//			}
//		}
//
//	}
//
//
//
//	//histogramy pozbierane
//	if (count>0)
//	{
//
//
//		statsScatter[0] = count;	//count
//		statsScatter[1] = ssd / count; //ssd
//		statsScatter[2] = (tmpE1*tmpE2>0) ? tmpE / (sqrt(tmpE1)*sqrt(tmpE2)) : 0; //cross correlation
//
//
//		double entropy, mi;
//		CompMutual(mi, entropy, scatter, hist1, hist2, bins, count);
//
//		statsScatter[3] = entropy;
//		statsScatter[4] = mi;
//		statsScatter[5] = 2.0*mi / (GetEntropy(hist1, bins, count) + GetEntropy(hist2, bins, count)); //NMI1
//
//
//
//		//	stats[1]=
//		//policze statystyki
//	}
//	return true;
//
//
//}
//-------------------------------
//bool UpdateStats(int bins, int *hist,double *stats,QPolygonF poly,unsigned char *mask,int nx,int ny,unsigned short *tab,int **tmpTab,int &count,bool showBounds=false,ImageShapes		*shapes=NULL,int maxValue=0,bool showRegion=false,QColor regionCol=QColor(255,255,255));
//bool UpdateStats(int bins, int *hist, double *stats, QPolygonF poly, unsigned char *mask, int nx, int ny, unsigned short *tab, int **tmpTab, int &count, bool showBounds, ImageShapes		*shapes, int maxValue, bool showRegion, QColor regionCol)
//{
//	// bool useThresholds,unsigned short leftTh,unsigned short rightTh
//	if ((hist == NULL) || (stats == NULL) || (mask == NULL) || (tmpTab[0] == NULL) || (tmpTab[1] == NULL)) return false;
//
//	if (bins>8)bins = 8;
//	if (bins<2)bins = 2;
//	bins = pow((double)2, bins);
//
//
//	QRectF rect = poly.boundingRect();
//	qreal x1, x2, y1, y2;
//	rect.getCoords(&x1, &y1, &x2, &y2);
//	int X1 = (int)x1;
//	int Y1 = (int)y1;
//	int X2 = (int)x2 + 1;
//	int Y2 = (int)y2 + 1;
//
//
//	int NX = x2 - x1;
//	int NY = y2 - y1;
//
//	int sliceSize = nx*ny;
//
//	QImage img(nx, ny, QImage::Format_RGB32);
//	img.fill(Qt::black);
//	QPainter Painter2(&img);
//	Painter2.setBrush(Qt::white);
//	Painter2.setPen(1);
//	Painter2.drawPolygon(poly);
//
//	int i, j;
//	bool test;
//	int pos;
//	unsigned char *tab11 = img.bits();
//	unsigned int *tab1 = (unsigned int *)tab11;
//	memset(hist, 0, bins*sizeof(int));
//	memset(stats, 0, 16 * sizeof(double));
//
//	double stddev;
//
//
//	int  		sum = 0;
//	long int 	sum2 = 0;
//	count = 0;
//	unsigned short val;
//	unsigned char min = 255;
//	unsigned char max = 0;
//
//	int *realTab = tmpTab[0];
//
//	double scale = (maxValue>0) ? (double)(bins - 1) / maxValue : 1.0;
//	for (j = y1; j <= y2; j++)
//	{
//		for (i = x1; i <= x2; i++)
//		{
//			pos = i + j*nx;;
//			test = (((tab1[pos]) == 0xffffffff));
//			if ((test == true) && (mask[pos] == 255))
//			{
//				//dobry pixel - jego wartosc bierzemy!
//				val = tab[pos];//*scale; //rozciagam histogram na zakres 0-255;
//
//				if (val>max) max = val; if (val<min) min = val;
//				hist[(int)(tab[pos] * scale)]++;
//
//				realTab[count] = pos;
//				sum += val;
//				sum2 += val*val;
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
//		double mean = (double)sum / (double(count));
//		double mean2 = (double)sum2 / (double)(count);
//
//		stddev = mean2 - mean*mean;
//
//		double skewness = 0;
//		double kurtosis = 0;
//
//		double *dTab = new double[count];
//		for (i = 0; i<count; i++)
//		{
//			dTab[i] = tab[realTab[i]];
//		}
//
//		CalculateSkewNessAndKurtosis(dTab, count, skewness, kurtosis);
//		delete[]dTab;
//
//
//
//		stats[0] = min;
//		stats[1] = max;
//		stats[2] = mean;
//		stats[3] = stddev;
//		stats[4] = skewness;
//		stats[5] = kurtosis;
//
//
//		if (showBounds)
//		{
//			int *dilatePoints = tmpTab[1];
//			int kernelType = 1;
//			int kernelSize = 0;
//			int size = 1;
//			int tmp;
//			RawDataSet *rData = new RawDataSet(nx, ny, 1, 1, 1, 0, false);
//
//			int *kernel = GetGrowingKernel2D(kernelSize, size, rData, kernelType);
//
//			int dsizemin1 = rData->GetVolumeSize() - 1;
//
//			int x, y;
//
//			int nrOfDilatePoints = 0;
//			double nnx1d = 1.0 / nx;
//
//			for (i = 0; i<count; i++)
//			{
//				for (int kk = 0; kk<kernelSize; kk++)
//				{
//					tmp = realTab[i] + kernel[kk];
//					if ((tmp>dsizemin1) || (tmp<0))	{ continue; }
//					else
//					{
//						if (mask[tmp] == 0)
//						{
//							y = realTab[i] * nnx1d;
//							x = realTab[i] - y*nx;
//							//wspolrzedne konturu
//							dilatePoints[nrOfDilatePoints] = x;
//							dilatePoints[nrOfDilatePoints + 1] = y;
//							nrOfDilatePoints += 2;
//
//							break;
//						}
//					}
//				}
//			}
//			nrOfDilatePoints /= 2;
//
//			delete[]kernel;
//			delete rData;
//
//			//mam zebrane pkty konturu teraz je pokaze
//			if (showRegion == true)
//			{
//
//				MyShape shape;
//
//
//				shape.polyColor = regionCol;//.setRgb(255,0,255);
//
//				for (int ii = 0; ii<nrOfDilatePoints * 2; ii += 2)
//				{
//					shape.poly.append(QPointF(dilatePoints[ii], dilatePoints[ii + 1]));
//				}
//
//				shape.representation = 0;
//				shapes->shapeList.append(shape);
//			}
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
//----------------------------------
bool Update2DStats2(int bins, int* hist1, int* hist2, int* scatter, double* statsScatter, int* bounds, unsigned int* maskImgBits,
	RawDataSet* modelData, RawDataSet* objectData, int** tmpTab, int& count, unsigned short* result,

	bool useTrans, double* trans, bool invTrans, double* rc)
{
	if ((hist1 == NULL) || (hist2 == NULL) || (scatter == NULL) || (statsScatter == NULL)) return false;




	if (bins > 8)bins = 8;
	if (bins < 2)bins = 2;
	bins = pow((double)2, bins);


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



	InterpolationFunction2DBilinear* interpolation = NULL;
	Transform* transformation = NULL;


	double c[2];
	c[0] = (X2 + X1) * 0.5;
	c[1] = (Y2 + Y1) * 0.5;
	double rCenter[3] = { c[0] * modelScale[0], c[1] * modelScale[1], 0 };//krece modelem

	if (rc != NULL)
	{
		rc[0] = rCenter[0]; rc[1] = rCenter[1]; rc[2] = rCenter[2];
	}

	interpolation = new InterpolationFunction2DBilinear();
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


	max1 = modelData->GetMaxValue();
	max2 = objectData->GetMaxValue();

	//unsigned short  biggerMaks=MAX(maxValue1,maxValue2);
	scale1 = (max1 > 0) ? (double)(bins - 1) / max1 : 1.0;
	scale2 = (max2 > 0) ? (double)(bins - 1) / max2 : 1.0;



	double bins1 = 1.0 / (bins - 1);

	//bool useBiggerMax,bool useThresholds,unsigned short leftTh,unsigned short rightTh,bool invThresh


	/*
	double leftTh1=leftThD*maxValue1;
	double rightTh1=rightThD*maxValue1;


	double leftTh2=leftThD*maxValue2;
	double rightTh2=rightThD*maxValue2;*/

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

	for (j = y1; j <= y2; j++)
	{
		for (i = x1; i <= x2; i++)
		{
			pos = i + j * modelN[0];;


			test = (maskImgBits != NULL) ? (((maskImgBits[pos]) == 0xffffffff)) : true;
			if ((test == true))
			{
				countModel++;
				Point[0] = i * modelScale[0];//przejscie do ukladu wspolrzednych globalnych 
				Point[1] = j * modelScale[1];

				transformation->MultiplyPoint(Point1, Point);
				Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu



				if ((Point1[0] >= 0) && (Point1[0] < t1) &&
					(Point1[1] >= 0) && (Point1[1] < t2))
				{
					val1 = tab1[pos] * scale1;
					val2 = interpolation->GetInterpolatedValue(objectData, Point1[0], Point1[1], 0) * scale2;
					//if (val2>max) max = val; if (val < min) min = val;

					//test
					bool test1 = true;
					bool test2 = true;
					if (result != NULL) result[pos] = val2;

					//sprawdzam czy pomiedzy progami

					/*	if (test1 == true)
					{

					if (val1>max1) max1 = val1; if (val1<min1) min1 = val1;*/
					hist1[val1]++;
					sum1 += val1;
					sum12 += val1 * val1;
					//}


					//sprawdzam czy pomiedzy progami 


					//if (test2 == true)
					//{
					//	if (val2>max2) max2 = val2; if (val2<min2) min2 = val2;
					hist2[val2]++;
					sum2 += val2;
					sum22 += val2 * val2;
					//}



					//	if ((test1 == true) && (test2 == true))
					//{
					scatter[val1 * bins + val2]++;
					if (realTab != NULL) realTab[count] = pos;

					ee = (val1 - val2) * bins1;
					ssd += ee * ee;
					tmpE += (val1 - val2) * (val1 - val2) * 0.0001;
					tmpE1 += val1 * val1 * 0.0001;
					tmpE2 += (val2 * val2) * 0.0001;

					count++;

					//	}

				}






			}
		}

	}



	//histogramy pozbierane
	if ((count > 0))// &&  (realTab != NULL))
	{

		//if (count / (double)countModel > 0.9)
		{

			statsScatter[0] = count;	//count
			statsScatter[1] = ssd / count; //ssd
			statsScatter[2] = (tmpE1 * tmpE2 > 0) ? tmpE / (sqrt(tmpE1) * sqrt(tmpE2)) : 0; //cross correlation


			double entropy, mi;
			CompMutual(mi, entropy, scatter, hist1, hist2, bins, count);

			statsScatter[3] = entropy;
			statsScatter[4] = mi;
			statsScatter[5] = 2.0 * mi / (GetEntropy(hist1, bins, count) + GetEntropy(hist2, bins, count)); //NMI1
		}
		//else
		//{
		//	statsScatter[0] = count;	//count
		//	statsScatter[1] = 100000;
		//}




		//	stats[1]=
		//policze statystyki
	}

	delete transformation;
	delete interpolation;

	return true;


}
//-------------------------------
void  PrepareTrans(double* tsrc, double* tdest, double nr, int* posTab)
{
	for (int i = 1; i <= nr; i++)
	{
		tdest[posTab[i]] = tsrc[i];
	}
	//int posTab[4] = { 0, 1, 2, 6 };
}
//----------------------------
void  PrepareTrans2(double* tsrc, double* tdest, double nr, int* posTab)
{
	for (int i = 1; i <= nr; i++)
	{
		tdest[posTab[i]] = tsrc[i];
	}
	//int posTab[4] = { 0, 1, 2, 6 };
}
//----------------------------------------------
void  UnPrepareTrans2(double* tsrc, double* tdest, double nr, int* posTab)
{
	for (int i = 1; i <= nr; i++)
	{
		tsrc[i] = tdest[posTab[i]];
	}
	//int posTab[4] = { 0, 1, 2, 6 };
}
double GetEntropy(int* Histogram, int size, int count)
{

	int i;
	double Entropy = 0;
	//	double Entropy2=0;
	double tol = 10e-6;
	double Count1 = 1.0 / count;
	double p1;


	for (i = 0; i < size; i++)
	{
		if (Histogram[i] != 0)
		{
			p1 = (double)Histogram[i] * Count1;
			Entropy -= p1 * log(p1);



		}
	}

	return Entropy;

}

//-----------
void CompMutual(double& mi, double& entropy, int* ST, int* MT, int* OT, int size, int count)
{

	entropy = 0;
	mi = 0;

	double p, p1, p2;
	int* sp = ST;
	int i, j;
	double Count1 = 1.0 / count;
	for (j = 0; j < size; j++)
	{
		p1 = (double)MT[j] * Count1;

		for (i = 0; i < size; i++)
		{
			if ((*sp != 0) && (MT[j] != 0) && (OT[i] != 0))
			{
				p = (double)*sp * Count1;
				p2 = (double)OT[i] * Count1;

				mi += -p * log(p / (p1 * p2));
				entropy += -p * log(p);
			}
			sp++;
		}
	}


}

//---------------------------------
int RemoveCollisionsFromLineSet(QPolygonF* lines, bool improveCollisions, bool testAll)
{

	int nrLines = lines->size() / 2;
	if (nrLines < 1) return - 1;

	int nrPts = lines->size();
	int i, j, k;

	//pierwsza kreska

	double line0[4];
	double line1[4];


	double* tab = new double[nrPts * 2];
	bool* delTab = new bool[nrLines];
	for (i = 0; i < nrLines; i++) delTab[i] = false;
	int pos = 0;

	for (i = 0; i < nrPts; i++)
	{
		tab[pos++] = lines->at(i).x();
		tab[pos++] = lines->at(i).y();
	}

	//pierwsza kreska
	for (j = 0; j < 4; j++)	line0[j] = tab[j];
	bool concident, intersection;
	double intersectionPoint[2];
	bool test;

	int last0 = 0;

	if (testAll == false)
	{
		//forwardPass
		for (i = 1; i < nrLines - 1; i++)
		{
			if (delTab[i] == false)
			{
				//kolejna kreska	
				for (j = 0; j < 4; j++)	line1[j] = tab[j + i * 4];
				test = LineCollision(line0, line1, concident, intersection, intersectionPoint);
				if (test == true)
				{
					//zaznacz do wywalenia
					delTab[i] = true;
				}
				else
				{
					//wymiana line0 na aktualna
					for (j = 0; j < 4; j++)	line0[j] = line1[j];
					last0 = i;
				}
			}

		}
		//backwardPass
		for (i = last0 - 1; i >= 0; i--)
		{
			if (delTab[i] == false)
			{
				//kolejna kreska	
				for (j = 0; j < 4; j++)	line1[j] = tab[j + i * 4];
				test = LineCollision(line0, line1, concident, intersection, intersectionPoint);
				if (test == true)
				{
					//zaznacz do wywalenia
					delTab[i] = true;
				}
				else
				{
					//wymiana line0 na aktualna
					for (j = 0; j < 4; j++)	line0[j] = line1[j];
				}
			}

		}
	}
	else
	{
		//forwardPass
		for (i = 1; i < nrLines; i++)
		{
			if (delTab[i] == false)
			{
				//kolejna kreska	
				for (j = 0; j < 4; j++)	line1[j] = tab[j + i * 4];

				//test ze wszystkimi poprzednimi

				for (k = 0; k < i - 1; k++)
				{
					for (j = 0; j < 4; j++)	line0[j] = tab[j + k * 4];
					test = LineCollision(line0, line1, concident, intersection, intersectionPoint);
					if (test == true)
					{
						//zaznacz do wywalenia
						delTab[i] = true;
						break;
					}
				}

				if (delTab[i] == false)
				{
					for (j = 0; j < 4; j++)	line0[j] = line1[j];
					last0 = i;
				}
			}

		}




	}


	//powieksze o 1 wszedzie w lewo i w prawo

	for (int i = 2; i < nrLines - 2; i++)
	{
		if (delTab[i] == true) delTab[i - 1] = true;
	}
	for (int i = nrLines - 2; i > 1; i--)
	{
		if (delTab[i] == true) delTab[i + 1] = true;
	}

	//dla pewnosci, ze nie usuwam pierwszego i ostatniego!!!!
	delTab[0] = false;
	delTab[nrLines - 1] = false;
	//--------------

	if (improveCollisions == true)
	{
		int newSize = 0;



		for (i = 0; i < nrLines; i++)
		{
			if (delTab[i] == false) newSize++;
		}

		if (newSize > 0)
		{
			/*double *posTab = new double[newSize];
			double *xs = new double[newSize * 2];
			double *ys = new double[newSize * 2];
			double *ksx = new double[newSize * 2];
			double *ksy = new double[newSize * 2];*/
			double* funcTab1x = new double[newSize * 2];
			double* funcTab2x = new double[newSize * 2];
			double* funcTab1y = new double[newSize * 2];
			double* funcTab2y = new double[newSize * 2];

			memset(funcTab1x, 0, newSize * 2 * sizeof(double));
			memset(funcTab1y, 0, newSize * 2 * sizeof(double));
			memset(funcTab2x, 0, newSize * 2 * sizeof(double));
			memset(funcTab2y, 0, newSize * 2 * sizeof(double));

			int pos = 0;
			for (i = 0; i < nrLines; i++)
			{
				if (delTab[i] == false)
				{
					funcTab1x[pos * 2] = pos;
					funcTab1x[pos * 2 + 1] = tab[i * 4];
					funcTab1y[pos * 2] = pos;
					funcTab1y[pos * 2 + 1] = tab[i * 4 + 1];

					funcTab2x[pos * 2] = pos;
					funcTab2x[pos * 2 + 1] = tab[i * 4 + 2];
					funcTab2y[pos * 2] = pos;
					funcTab2y[pos * 2 + 1] = tab[i * 4 + 3];

					/*xs[pos] = tab[i * 4];
					ys[pos] = tab[i * 4 + 1];

					xs[pos + newSize] = tab[i * 4 + 2];
					ys[pos + newSize] = tab[i * 4 + 3];
					posTab[pos] = pos;*/
					pos++;
				}
			}

			/*	double *xs1 = xs + newSize;
			double *ys1 = ys + newSize;
			double *ksx1 = ksx + newSize;
			double *ksy1 = ksy + newSize;*/

			//getNaturalKs(posTab, xs, ksx, newSize);
			//getNaturalKs(posTab, ys, ksy, newSize);
			//getNaturalKs(posTab, xs1, ksx1, newSize);
			//getNaturalKs(posTab, ys1, ksy1, newSize);

			//powinienem miec 2 krzywe z gory i z dolu z odpowiednimi punktami ks - na

			//int start, stop;
			int splinePos = -1;
			int newJ;
			int count = 0;
			double t;
			double p[4];
			int newPos;


			CardinalSpline x1Spline(funcTab1x, newSize);
			CardinalSpline x2Spline(funcTab2x, newSize);
			CardinalSpline y1Spline(funcTab1y, newSize);
			CardinalSpline y2Spline(funcTab2y, newSize);

			for (int j = 0; j < nrLines; j++)
			{

				if (delTab[j] == true)
				{
					//j--;
					int k = j;
					do
					{
						k++;
					} while (delTab[k] == true);
					newJ = k;
					//znalazlem koniec sekcji do interpolacji
					count = k - j + 1;//liczba pktow do inteprolacji w sekcji
					for (k = 1; k < count; k++)
					{
						t = splinePos + k / (double)(count);

						p[0] = x1Spline.Evaluate(t);// evaluate(evalSpline(t, posTab, xs, ksx); //wspolrzedna  x gornej krzywej - nowy punkt
						p[1] = y1Spline.Evaluate(t); //evalSpline(t, posTab, ys, ksy); //wps y gornej
						p[2] = x2Spline.Evaluate(t);// evalSpline(t, posTab, xs1, ksx1); //wsp x dolnej
						p[3] = y2Spline.Evaluate(t); //evalSpline(t, posTab, ys1, ksy1); //wsp y dolnej
													 //wyliczylem interpolowanego zenka i wstawie go na miejsce zlego;
						newPos = (j + k - 1); //poczatek kreski
											  //pts->SetPoint(newPos, p1);

						lines->replace(newPos * 2, QPointF(p[0], p[1]));
						lines->replace(newPos * 2 + 1, QPointF(p[2], p[3]));
						//lines->remove(i * 2 + 1);
						//lines->remove(i * 2);
					}

					j = newJ;
					splinePos++;
				}
				else
				{
					splinePos++;
				}

			}

			delete[]funcTab1x;
			delete[]funcTab2x;
			delete[]funcTab1y;
			delete[]funcTab2y;

			//delete[]ksx;
			//delete[]ksy;
			//delete[]ys;
			//delete[]xs;
			//delete[]posTab;
		}

	}
	//wywalanie złych punktow
	int improved = 0;
	for (i = nrLines - 1; i >= 0; i--)
	{
		if (delTab[i] == true)
		{
			if (improveCollisions == false)
			{
				lines->remove(i * 2 + 1);
				lines->remove(i * 2);
			}
			improved++;

		}
	}

	delete[]tab;
	delete[]delTab;

	return improved;

}
//-------------------------
bool LineCollision(double l1[4], double l2[4], bool& coincident, bool& intersection, double intersectionPoint[2])
{
	double point1X = l1[0];
	double point1Y = l1[1];
	double point2X = l1[2];
	double point2Y = l1[3];


	double point3X = l2[0];
	double point3Y = l2[1];
	double point4X = l2[2];
	double point4Y = l2[3];

	double ua = (point4X - point3X) * (point1Y - point3Y) - (point4Y - point3Y) * (point1X - point3X);
	double ub = (point2X - point1X) * (point1Y - point3Y) - (point2Y - point1Y) * (point1X - point3X);
	double denominator = (point4Y - point3Y) * (point2X - point1X) - (point4X - point3X) * (point2Y - point1Y);

	intersection = coincident = false;

	if (fabs(denominator) <= 0.00001f)
	{
		if (fabs(ua) <= 0.00001f && fabs(ub) <= 0.00001f)
		{
			intersection = coincident = true;
			intersectionPoint[0] = (point1X + point2X) / 2;
			intersectionPoint[1] = (point1Y + point2Y) / 2;
		}
	}
	else
	{
		ua /= denominator;
		ub /= denominator;

		if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
		{
			intersection = true;
			intersectionPoint[0] = point1X + ua * (point2X - point1X);
			intersectionPoint[1] = point1Y + ua * (point2Y - point1Y);
		}
	}
	if ((intersection == true) || (coincident == true)) return true;

	return false;
}







//--------------------------- intensity probability map!!
void GetExpTab(double* _Tab, int number, double sigma, double intensityFactor)
{
	const double tolX = 10e-8;
	//	double fct=255.0/(double)number;
	//	fct*=fct;
	//if (fabs(sigma*v)>tolX)	
	{
		QElapsedTimer tm;
		tm.start();



		unsigned int t1 = tm.elapsed();
		//if ((sigma==0)||(v==0)) return;//	dziadostwo
		int	i;

		double b = intensityFactor / (2 * sigma * sigma);
		double tmp;
		double t;
		double min = INT_MAX;
		double max = -INT_MAX;
		double xa = (255.0 / (double)number);
		b *= xa * xa; //normalizacja tego co do expa

		int qmin; int qmax;
		/*for (i = 0; i <= 2 * number; i++)
		{
		t = -(double)(i - number)*(i - number)*b;
		if (t<min)	{ min = t; qmin = i; }
		if (t>max)	{ max = t; qmax = i; }
		tmp = exp(t);
		_Tab[i] = tmp;
		}*/

		for (i = -number; i < number; i++)
		{
			t = -(double)(i) * (i)*b; //zmina bo zamiast roznicy i1-i2 wole trzymac fabs|roznicy| co pomaga trzymac wartosci 0-number
			if (t < min) { min = t; qmin = i; }
			if (t > max) { max = t; qmax = i; }
			tmp = exp(t);
			_Tab[i + number] = tmp;
		}

		//qDebug() << "minmax: " << min << "   " << max;

		t = t;
		unsigned int t2 = tm.elapsed();

		//	qWarning() << "GetExpTab: " << t2 - t1;
	}
}
//--------------------------------------------
double GetIntensityProbability(unsigned short intensity, unsigned short max, double sigma, double* expTab, unsigned short* samples, int nrOfSamples, int nrOfBins)
{

	double prop = 0.0;
	int	i;
	int val = 0;
	for (i = 0; i < nrOfBins; i++)
	{
		val = samples[i];
		if (val > 0)
		{
			//prop += val*expTab[intensity - i + max];
			prop += val * expTab[(i - intensity) + max];
		}
	}

	prop /= (sqrt(2.0 * M_PI) * sigma * nrOfSamples);

	return prop;
}
//---------------------------------------------
void GetIntensityProbabilityTab(double* tab, int nr, double& min, double& max, double sigma, double* expTab, unsigned short* samples, int nrOfBins, unsigned short maxSample, int nrOfSamples)
{
	min = INT_MAX;
	max = -INT_MAX;;
	if (tab == NULL) return;
	int	i;


	QElapsedTimer tm;
	tm.start();



	unsigned int t1 = tm.elapsed();

	for (i = 0; i < nrOfBins; i++)
	{
		tab[i] = GetIntensityProbability(i, maxSample, sigma, expTab, samples, nrOfSamples, nrOfBins);
		if (tab[i] < min)	min = tab[i];
		if (tab[i] > max)	max = tab[i];
	}
	unsigned int t2 = tm.elapsed();

	//qWarning() << "GetIntensityProbabilityTab: " << t2 - t1 << "be if Bins:" << nrOfBins;
}
//-------------------------
double GetIntensityProbability2(unsigned short intensity, unsigned short max, double sigma, double* expTab, unsigned short* samplesCount, unsigned short* samplesIndex, int nrOfBins, int nrOfSamples)
{


	double prop = 0.0;
	int	i;
	int val = 0;
	for (i = 0; i < nrOfBins; i++)
	{
		//wersja zoptymalizana  na scisnieta samplestab - index zawirera wartosc, count zawiera ilosc wystapien
		prop += samplesCount[i] * expTab[(samplesIndex[i] - intensity) + max];

	}

	prop /= (sqrt(2.0 * M_PI) * sigma * nrOfSamples);

	return prop;
}
//-------------------------
void GetIntensityProbabilityTab2(double* tab, double maxValue, double& min, double& max, double sigma, double* expTab, unsigned short* samplesCount, unsigned short* samplesIndex, int nrOfBins, int nrOfSamples)
{
	//tab - dane do wypelnienia
	//min, max - do policzenia
	//sigma - 
	//exp tab - na wejsciu
	//samples count - ilosc wystapien danej jasnosci w startowym obszarze
	//samplesindes - indexy prawdziwych jasnosci odpowiadajacaych samplescount
	//nrofbins - ilosc jasnosci obrazu
	//nrofsamples - ilosc probek obszaru strartowego

	min = INT_MAX;
	max = -INT_MAX;;
	if (tab == NULL) return;
	int	i;


	QElapsedTimer tm;
	tm.start();



	unsigned int t1 = tm.elapsed();

	for (i = 0; i < maxValue; i++)
	{
		tab[i] = GetIntensityProbability2(i, maxValue, sigma, expTab, samplesCount, samplesIndex, nrOfBins, nrOfSamples);
		if (tab[i] < min)	min = tab[i];
		if (tab[i] > max)	max = tab[i];
	}
	unsigned int t2 = tm.elapsed();

	//qWarning() << "GetIntensityProbabilityTab2: " << t2 - t1 << "be if Bins:" << nrOfBins;
}
//----------------------------------------------
bool UpdateStats(int bins, int* hist, double* stats, QPolygonF poly, unsigned char* mask, int nx, int ny, unsigned short* tab, int** tmpTab, int& count, bool showBounds, ImageShapes* shapes, int maxValue, bool showRegion, QColor regionCol)
{
	// bool useThresholds,unsigned short leftTh,unsigned short rightTh
	if ((hist == NULL) || (stats == NULL) || (mask == NULL) || (tmpTab[0] == NULL) || (tmpTab[1] == NULL)) return false;

	if (bins > 8)bins = 8;
	if (bins < 2)bins = 2;
	bins = pow((double)2, bins);


	QRectF rect = poly.boundingRect();
	qreal x1, x2, y1, y2;
	rect.getCoords(&x1, &y1, &x2, &y2);
	int X1 = (int)x1;
	int Y1 = (int)y1;
	int X2 = (int)x2 + 1;
	int Y2 = (int)y2 + 1;


	int NX = x2 - x1;
	int NY = y2 - y1;

	int sliceSize = nx * ny;

	QImage img(nx, ny, QImage::Format_RGB32);
	img.fill(Qt::black);
	QPainter Painter2(&img);
	Painter2.setBrush(Qt::white);
	Painter2.setPen(1);
	Painter2.drawPolygon(poly);

	int i, j;
	bool test;
	int pos;
	unsigned char* tab11 = img.bits();
	unsigned int* tab1 = (unsigned int*)tab11;
	memset(hist, 0, bins * sizeof(int));
	memset(stats, 0, 16 * sizeof(double));

	double stddev;


	int  		sum = 0;
	long int 	sum2 = 0;
	count = 0;
	unsigned short val;
	unsigned char min = 255;
	unsigned char max = 0;

	int* realTab = tmpTab[0];

	double scale = (maxValue > 0) ? (double)(bins - 1) / maxValue : 1.0;
	for (j = y1; j <= y2; j++)
	{
		for (i = x1; i <= x2; i++)
		{
			pos = i + j * nx;;
			test = (((tab1[pos]) == 0xffffffff));
			if ((test == true) && (mask[pos] == 255))
			{
				//dobry pixel - jego wartosc bierzemy!
				val = tab[pos];//*scale; //rozciagam histogram na zakres 0-255;

				if (val > max) max = val; if (val < min) min = val;
				hist[(int)(tab[pos] * scale)]++;

				realTab[count] = pos;
				sum += val;
				sum2 += val * val;

				count++;

			}
		}
	}

	if (count > 0)
	{
		double stddev;
		double mean = (double)sum / (double(count));
		double mean2 = (double)sum2 / (double)(count);

		stddev = mean2 - mean * mean;

		double skewness = 0;
		double kurtosis = 0;

		double* dTab = new double[count];
		for (i = 0; i < count; i++)
		{
			dTab[i] = tab[realTab[i]];
		}

		CalculateSkewNessAndKurtosis(dTab, count, skewness, kurtosis);
		delete[]dTab;



		stats[0] = min;
		stats[1] = max;
		stats[2] = mean;
		stats[3] = stddev;
		stats[4] = skewness;
		stats[5] = kurtosis;


		if (showBounds)
		{
			int* dilatePoints = tmpTab[1];
			int kernelType = 1;
			int kernelSize = 0;
			int size = 1;
			int tmp;
			RawDataSet* rData = new RawDataSet(nx, ny, 1, 1, 1, 0, false);

			int* kernel = GetGrowingKernel2D(kernelSize, size, rData, kernelType);

			int dsizemin1 = rData->GetVolumeSize() - 1;

			int x, y;

			int nrOfDilatePoints = 0;
			double nnx1d = 1.0 / nx;

			for (i = 0; i < count; i++)
			{
				for (int kk = 0; kk < kernelSize; kk++)
				{
					tmp = realTab[i] + kernel[kk];
					if ((tmp > dsizemin1) || (tmp < 0)) { continue; }
					else
					{
						if (mask[tmp] == 0)
						{
							y = realTab[i] * nnx1d;
							x = realTab[i] - y * nx;
							//wspolrzedne konturu
							dilatePoints[nrOfDilatePoints] = x;
							dilatePoints[nrOfDilatePoints + 1] = y;
							nrOfDilatePoints += 2;

							break;
						}
					}
				}
			}
			nrOfDilatePoints /= 2;

			delete[]kernel;
			delete rData;

			//mam zebrane pkty konturu teraz je pokaze
			if (showRegion == true)
			{

				MyShape shape;


				shape.polyColor = regionCol;//.setRgb(255,0,255);

				for (int ii = 0; ii < nrOfDilatePoints * 2; ii += 2)
				{
					shape.poly.append(QPointF(dilatePoints[ii], dilatePoints[ii + 1]));
				}

				shape.representation = 0;
				shapes->shapeList.append(shape);
			}



		}



		return true;
	}


	return false;

}
//----------------------------------
bool Update2DStats(int bins, int* hist1, int* hist2, int* scatter, double* statsScatter, QPolygonF poly, int nx, int ny,
	unsigned short* tab1, unsigned short* tab2, int** tmpTab, int& count, ImageShapes* shapes, int maxValue1, int  maxValue2,

	bool useBiggerMax, bool useThresholds, double leftThD, double rightThD, bool invThresh, bool showReg1, bool showReg2)
{
	if ((hist1 == NULL) || (hist2 == NULL) || (scatter == NULL) || (statsScatter == NULL) || (tmpTab[0] == NULL) || (tmpTab[1] == NULL)) return false;
	if (bins > 8)bins = 8;
	if (bins < 2)bins = 2;
	bins = pow((double)2, bins);

	QRectF rect = poly.boundingRect();
	qreal x1, x2, y1, y2;
	rect.getCoords(&x1, &y1, &x2, &y2);
	int X1 = (int)x1;
	int Y1 = (int)y1;
	int X2 = (int)x2 + 1;
	int Y2 = (int)y2 + 1;


	int NX = x2 - x1;
	int NY = y2 - y1;

	int sliceSize = nx * ny;

	QImage img(nx, ny, QImage::Format_RGB32);
	img.fill(Qt::black);
	QPainter Painter2(&img);
	Painter2.setBrush(Qt::white);
	Painter2.setPen(1);
	Painter2.drawPolygon(poly);

	int i, j;
	bool test;
	int pos;
	unsigned char* tab11 = img.bits();
	unsigned int* tab1x = (unsigned int*)tab11;

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
	unsigned char max1 = 0, max2 = 0;

	int* realTab = tmpTab[0];


	//findMAx

	double scale1, scale2;
	double leftTh1, leftTh2;
	double rightTh1, rightTh2;
	if (useBiggerMax == false)
	{


		scale1 = (maxValue1 > 0) ? (double)(bins - 1) / maxValue1 : 1.0;
		scale2 = (maxValue2 > 0) ? (double)(bins - 1) / maxValue2 : 1.0;

		leftTh1 = leftThD * maxValue1;
		rightTh1 = rightThD * maxValue1;
		leftTh2 = leftThD * maxValue2;
		rightTh2 = rightThD * maxValue2;

	}
	else
	{
		unsigned short  biggerMaks = MAX(maxValue1, maxValue2);
		scale1 = (biggerMaks > 0) ? (double)(bins - 1) / biggerMaks : 1.0;
		scale2 = (biggerMaks > 0) ? (double)(bins - 1) / biggerMaks : 1.0;

		leftTh1 = leftThD * biggerMaks;
		rightTh1 = rightThD * biggerMaks;
		leftTh2 = leftThD * biggerMaks;
		rightTh2 = rightThD * biggerMaks;
	}

	double bins1 = 1.0 / (bins - 1);

	//bool useBiggerMax,bool useThresholds,unsigned short leftTh,unsigned short rightTh,bool invThresh


	/*
	double leftTh1=leftThD*maxValue1;
	double rightTh1=rightThD*maxValue1;


	double leftTh2=leftThD*maxValue2;
	double rightTh2=rightThD*maxValue2;*/

	double floatEnergy = 0;
	double tmpE;
	double tmpE1, tmpE2;
	tmpE1 = 0;
	tmpE2 = 0;
	tmpE = 0;
	double ee;
	double ssd = 0;
	for (j = y1; j <= y2; j++)
	{
		for (i = x1; i <= x2; i++)
		{
			pos = i + j * nx;;
			test = (((tab1x[pos]) == 0xffffffff));
			if ((test == true))
			{
				//dobry pixel - jego wartosc bierzemy!
				val1 = tab1[pos] * scale1; //rozciagam histogram na zakres 0-255;
				val2 = tab2[pos] * scale2; //rozciagam histogram na zakres 0-255;

										   //test
				bool test1 = true;
				bool test2 = true;


				//sprawdzam czy pomiedzy progami
				if (useThresholds == true)
				{
					if (((tab1[pos] >= leftTh1) && (tab1[pos] <= rightTh1)) == invThresh)
						test1 = false;
				}
				if (test1 == true)
				{

					if (val1 > max1) max1 = val1; if (val1 < min1) min1 = val1;
					hist1[val1]++;
					sum1 += val1;
					sum12 += val1 * val1;
				}


				//sprawdzam czy pomiedzy progami 
				if (useThresholds == true)
				{
					if (((tab2[pos] >= leftTh2) && (tab2[pos] <= rightTh2)) == invThresh)
						test2 = false;
				}

				if (test2 == true)
				{
					if (val2 > max2) max2 = val2; if (val2 < min2) min2 = val2;
					hist2[val2]++;
					sum2 += val2;
					sum22 += val2 * val2;
				}



				if ((test1 == true) && (test2 == true))
				{
					scatter[val1 * bins + val2]++;
					realTab[count] = pos;

					ee = (val1 - val2) * bins1;
					ssd += ee * ee;
					tmpE += (val1 - val2) * (val1 - val2) * 0.0001;
					tmpE1 += val1 * val1 * 0.0001;
					tmpE2 += (val2 * val2) * 0.0001;

					count++;

				}





			}
		}

	}



	//histogramy pozbierane
	if (count > 0)
	{


		statsScatter[0] = count;	//count
		statsScatter[1] = ssd / count; //ssd
		statsScatter[2] = (tmpE1 * tmpE2 > 0) ? tmpE / (sqrt(tmpE1) * sqrt(tmpE2)) : 0; //cross correlation


		double entropy, mi;
		CompMutual(mi, entropy, scatter, hist1, hist2, bins, count);

		statsScatter[3] = entropy;
		statsScatter[4] = mi;
		statsScatter[5] = 2.0 * mi / (GetEntropy(hist1, bins, count) + GetEntropy(hist2, bins, count)); //NMI1



																									  //	stats[1]=
																									  //policze statystyki
	}


	return true;


}
bool UpdateStats(int bins, int* hist, double* stats, int* bounds, unsigned int* maskImgBits, RawDataSet* modelData, unsigned char* mask, int** tmpTab,
	int& count,
	int maxValue,
	bool useTrans, double* trans, bool invTrans, RawDataSet* objectData)
{

	//RawDataSet *modelData, *objectData;


	//trans  poprawny zestaw parametrow przyjmowany standardowo przez class Transform

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





	// bool useThresholds,unsigned short leftTh,unsigned short rightTh
	if ((hist == NULL) || (stats == NULL)) return false;

	if (bins > 8)bins = 8;
	if (bins < 2)bins = 2;
	bins = pow((double)2, bins);







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


	if ((NX == 0) || (NY == 0)) //shape - poprawiam jesli shape pusty
	{

		x1 = 0; y1 = 0;
		x2 = modelN[0] - 1; y2 = modelN[1] - 1;

		NX = x2 - x1;
		NY = y2 - y1;

	}

	double c[2];
	c[0] = (X2 + X1) * 0.5;
	c[1] = (Y2 + Y1) * 0.5;
	double rCenter[3] = { c[0] * modelScale[0] * 0.5, c[1] * modelScale[1] * 0.5, 0 };//krece modelem

	InterpolationFunction2DBilinear* interpolation = NULL;
	Transform* transformation = NULL;
	//double rCenter[3] = { modelN[0] * modelScale[0] * 0.5, modelN[1] * modelScale[1] * 0.5, 0 };//krece modelem
	unsigned short* tab;
	if (useTrans == true)
	{
		interpolation = new InterpolationFunction2DBilinear();
		transformation = new Transform(rCenter, 1, 1, 1);
		transformation->MakeCompleteAffineMatrix(matrix, trans);
		tab = objectData->GetDataArray();
	}
	else
	{
		tab = modelData->GetDataArray();
	}
	//int sliceSize=nx*ny;

	//QImage img(modelN[0], modelN[1], QImage::Format_RGB32);
	//img.fill(Qt::black);
	//QPainter Painter2(&img);	
	//Painter2.setBrush(Qt::white);
	//Painter2.setPen(1);	
	//Painter2.drawPolygon(poly);

	int i, j;
	bool test;
	int pos;
	//	unsigned char *tab11=img.bits();
	//unsigned int *tab1=(unsigned int *)tab11;
	memset(hist, 0, bins * sizeof(int));
	memset(stats, 0, 16 * sizeof(double));

	double stddev;


	int  		sum = 0;
	long int 	sum2 = 0;
	count = 0;
	unsigned short val;
	unsigned char min = 255;
	unsigned char max = 0;

	int* realTab = tmpTab[0];

	double scale = (maxValue > 0) ? (double)(bins - 1) / maxValue : 1.0;

	if (useTrans == false)
	{


		for (j = y1; j <= y2; j++)
		{
			for (i = x1; i <= x2; i++)
			{
				pos = i + j * modelN[0];;
				//test=(((tab1[pos])==0xffffffff));
				test = (maskImgBits != NULL) ? (((maskImgBits[pos]) == 0xffffffff)) : true;
				if ((test == true))
				{
					//dobry pixel - jego wartosc bierzemy!
					val = tab[pos];//*scale; //rozciagam histogram na zakres 0-255;

					if (val > max) max = val; if (val < min) min = val;
					hist[(int)(val * scale)]++;

					if (realTab != NULL) realTab[count] = pos;
					sum += val;
					sum2 += val * val;

					count++;

				}
			}
		}
	}
	else
	{

		double scalef[2] = { modelScale[0] / objectScale[0], modelScale[1] / objectScale[1] };

		double objscale1[2] = { 1.0 / objectScale[0], 1.0 / objectScale[1] };

		double Point[3] = { 0 };
		double Point1[3] = { 0 };
		unsigned short val;
		double t1 = objectN[0] - 1;
		double t2 = objectN[1] - 1;

		for (j = y1; j <= y2; j++)
		{
			for (i = x1; i <= x2; i++)
			{
				pos = i + j * modelN[0];;
				//test = (((tab1[pos]) == 0xffffffff));
				test = (maskImgBits != NULL) ? (((maskImgBits[pos]) == 0xffffffff)) : true;
				if ((test == true))
				{
					Point[0] = i * modelScale[0];//przejscie do ukladu wspolrzednych globalnych 
					Point[1] = j * modelScale[1];

					transformation->MultiplyPoint(Point1, Point);
					Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu


					if ((Point1[0] >= 0) && (Point1[0] < t1) &&
						(Point1[1] >= 0) && (Point1[1] < t2))
					{
						val = interpolation->GetInterpolatedValue(objectData, Point1[0], Point1[1], 0);
						if (val > max) max = val; if (val < min) min = val;
						hist[(int)(val * scale)]++;

						if (realTab != NULL) realTab[count] = pos;
						sum += val;
						sum2 += val * val;

						count++;
					}
				}


			}
		}
	}

	if ((count > 0) && (realTab != NULL))
	{
		double stddev;
		double mean = (double)sum / (double(count));
		double mean2 = (double)sum2 / (double)(count);

		stddev = mean2 - mean * mean;

		double skewness = 0;
		double kurtosis = 0;

		double* dTab = new double[count];
		for (i = 0; i < count; i++)
		{
			dTab[i] = tab[realTab[i]];
		}

		CalculateSkewNessAndKurtosis(dTab, count, skewness, kurtosis);
		delete[]dTab;



		stats[0] = min;
		stats[1] = max;
		stats[2] = mean;
		stats[3] = stddev;
		stats[4] = skewness;
		stats[5] = kurtosis;





		//if (useTrans == true)
		//{
		//	delete transformation;
		//	delete interpolation;
		//}

		//return true;
	}

	if (useTrans == true)
	{
		delete transformation;
		delete interpolation;
	}

	return true;

}
//----------------------------------

void GetStatisticsFromVolumeSampledTab(RawDataSet* data, int* tab, int tabSize, double* params)
{

	double min = INT_MAX;
	double max = -INT_MAX;
	double stdDev;
	double* table = new double[tabSize];
	double mean = 0;
	double mean2 = 0;
	unsigned short* tabx = data->GetDataArray();
	for (int i = 0; i < tabSize; i++)
	{
		table[i] = tabx[tab[i]];
		if (table[i] < min) min = table[i];
		if (table[i] > max)max = table[i];
		mean += table[i];
		mean2 += table[i] * table[i];

	}

	mean /= tabSize;
	mean2 /= tabSize;

	stdDev = sqrt(mean2 - mean * mean);

	double skewness = 0;
	double kurtosis = 0;
	CalculateSkewNessAndKurtosis(table, tabSize, skewness, kurtosis);


	delete[]table;

	params[0] = min;
	params[1] = max;
	params[2] = mean;
	params[3] = stdDev;

	params[4] = skewness;
	params[5] = kurtosis;
	params[6] = tabSize;
	params[7] = tabSize * data->GetXscale() * data->GetYscale();



}
//-------------------------------------
void GetStatisticsFromUShortTab(unsigned short* table, int tabSize, double* params, bool computerHigherStats )
{

	double min = INT_MAX;
	double max = -INT_MAX;
	double stdDev;
	
	double mean = 0;
	double mean2 = 0;
	
	for (int i = 0; i < tabSize; i++)
	{
		
		if (table[i] < min) min = table[i];
		if (table[i] > max)max = table[i];
		mean += table[i];
		mean2 += table[i] * table[i];

	}

	mean /= tabSize;
	mean2 /= tabSize;

	stdDev = sqrt(mean2 - mean * mean);

	double skewness = 0;
	double kurtosis = 0;

	params[0] = min;
	params[1] = max;
	params[2] = mean;
	params[3] = stdDev;

	if (computerHigherStats == true)
	{
		CalculateSkewNessAndKurtosis(table, tabSize, skewness, kurtosis);
		params[4] = skewness;
		params[5] = kurtosis;
	}
	

}
//------------------------------------
double* GetStatisticsFromDoubleTab(double* tabx, int size, int nrOfBins, double* params,bool dontCreateHist)
{
	double min = INT_MAX;
	double max = -INT_MAX;
	double stdDev;

	double mean = 0;
	double mean2 = 0;
	int count = size;

	int volSize = size;

	double maxValue = 0;

	for (int i = 0;i < 7;i++)
	{
		params[i] = 0;
	}

	for (int i = 0; i < volSize; i++)
	{
		double val = tabx[i];
		//if (tabx[i] > maxValue) maxValue = tabx[i];
		if (val < min) min = val;
		if (val > max)max = val;
	}

	
//	if (fabs(max < 1e-5))return NULL;//
	double* hist = NULL;
	
	if (dontCreateHist == false)
	{
		hist = new double[nrOfBins];
		memset(hist, 0, nrOfBins * sizeof(double));
	}

	int pos = 0;
	for (int i = 0; i < volSize; i++)
	{

		double val = tabx[i];

		if (dontCreateHist == false)
		{
			int hPos = (int)(val * (nrOfBins - 1) / max);
			if (hPos >= nrOfBins)
			{
				i = i;
			}
			hist[hPos]++;
		}


		mean += val;
		mean2 += val * val;;

		pos++;


	}

	mean /= count;
	mean2 /= count;

	stdDev = sqrt(mean2 - mean * mean);

	double skewness = 0;
	double kurtosis = 0;
	CalculateSkewNessAndKurtosis(tabx, count, skewness, kurtosis);


	//delete[]table;

	params[0] = min;
	params[1] = max;
	params[2] = mean;
	params[3] = stdDev;

	params[4] = skewness;
	params[5] = kurtosis;
	params[6] = count;
	//params[7] = count*data->GetXscale()*data->GetYscale()*data->GetZscale();
	return hist;

}

int* GetStatisticsFromVolumeSampledTabUsingMask(RawDataSet* data, unsigned char* mask, double* params)
{
	double min = INT_MAX;
	double max = -INT_MAX;
	double stdDev;

	double mean = 0;
	double mean2 = 0;
	unsigned short* tabx = data->GetDataArray();

	int volSize = data->GetVolumeSize();
	int count = 0;
	int maxValue = 0;

	for (int i = 0; i < volSize; i++)
	{
		if (mask[i] > 0)
		{
			count++;
			if (tabx[i] > maxValue) maxValue = tabx[i];
		}
	}
	if ((maxValue == 0) || (count == 0))
	{
		return NULL;
	}

	double* table = new double[count];
	int* hist = new int[maxValue + 1];
	memset(hist, 0, maxValue * sizeof(int));
	int pos = 0;
	for (int i = 0; i < volSize; i++)
	{
		if (mask[i] > 0)
		{
			int val = tabx[i];
			table[pos] = val;
			hist[val]++;
			if (val < min) min = val;
			if (val > max)max = val;
			mean += val;
			mean2 += val * val;;

			pos++;
		}

	}

	mean /= count;
	mean2 /= count;

	stdDev = sqrt(mean2 - mean * mean);

	double skewness = 0;
	double kurtosis = 0;
	CalculateSkewNessAndKurtosis(table, count, skewness, kurtosis);

	double median = 0;
	int medianPos = 0;
	double maxPeakPos = 0; double maxPeakVal = 0;
	//if (calculateMedian == true)
	{
		medianPos = 0;;

		// zrobie nowa tabele dla roznych od zera - tylko niepuste w histogramie


		int countNonEmpty = 0;
		int lastNonZero = 0;

		maxPeakPos = 0;
		for (int i = 0; i < maxValue + 1; i++)
		{
			if (hist[i] > maxPeakVal)
			{
				maxPeakVal = hist[i];
				maxPeakPos = i;

			}

			if (hist[i] > 0)
			{
				lastNonZero = i;
				countNonEmpty++;
			}
		}
		medianPos = lastNonZero;

		if (countNonEmpty > 1)
		{

			double half = count / 2;

			int sum = 0;
			for (int i = 0; i < maxValue + 1; i++)
			{
				sum += hist[i];
				if (sum >= half)
				{
					medianPos = i;
					break;
				}
			}



		}






	}
	delete[]table;

	params[0] = min;
	params[1] = max;
	params[2] = mean;
	params[3] = stdDev;

	params[4] = skewness;
	params[5] = kurtosis;
	params[6] = count;
	params[7] = count * data->GetXscale() * data->GetYscale() * data->GetZscale();
	params[8] = medianPos;
	params[9] = maxPeakPos;
	return hist;

}
//-----------------------
void simple2DMophology(unsigned char* buffer, int nSizeX, int nSizeY, int kernelType, int morphologyType)
{
	RawDataSet* noneData = new RawDataSet(nSizeX, nSizeY, 1, 1, 1, 1, 1, false);
	int kSize;
	int kerenelSize2 = 0;
	int size2 = 1;
	//int kernelType=morphologyKernelType;
	int* kernel2 = GetGrowingKernel2D(kerenelSize2, size2, noneData, kernelType);
	int tmp;
	int dsizemin1n = noneData->GetVolumeSize() - 1;


	int nrOfDilatePoints = 0;


	bool stop;
	double nnx1d = 1.0 / nSizeX;
	//contur extraction

	int frontVal = morphologyType;
	int backVal = !frontVal;

	int ax, ay;
	int startPos = 0;
	for (int jj = 0; jj < nSizeY; jj++)
	{

		for (int ii = 0; ii < nSizeX; ii++)
		{
			if (buffer[startPos] == frontVal)
			{
				stop = false;
				for (int kk = 0; kk < kerenelSize2; kk++)
				{

					tmp = startPos + kernel2[kk];
					if ((tmp > dsizemin1n) || (tmp < 0)) { continue; }
					else {
						//jakikolwiek zapalony
						if (buffer[tmp] == backVal) {
							stop = true;
							break;
						}
					}


				}
				if (stop)
				{
					buffer[startPos] = 255;
				}
			}
			startPos++;
		}


	}

	for (int i = 0; i < nSizeY * nSizeX; i++)
	{
		if (buffer[i] == 255)buffer[i] = backVal;
	}

	delete[]kernel2;
	delete  noneData;
}

//-----------------------------
void	RemapOutFrontTabWithDidivedIslands(int* frontTabOut, int frontTabOutLength, unsigned char* islandMask, unsigned char* tmpIslandMask, int*& listS, int& listSSize, RawDataSet* data, int& maxIsleNr, int kernelType, bool is2D, bool showDebug, int tmpMaskBitNr)
{

	if (frontTabOutLength < 2)
	{
		return;
	}

	int size = 1;
	int kernelSize;
	int* kernel = NULL;
	if (!is2D)
		kernel = GetGrowingKernel(kernelSize, size, data, kernelType);
	else
		kernel = GetGrowingKernel2D(kernelSize, size, data, kernelType);


	int lp, ap, tmp;
	int isle = -1;

	//frontTabOut - zawiera indeksy frontu

	int** tempint = new int* [frontTabOutLength];

	int countTab[256] = { 0 };


	// isleTabStart wypelniona indeksami z maski - zrodlo
	for (int i = 0; i < frontTabOutLength; i++)
	{
		tempint[i] = new int[3];
		tempint[i][1] = islandMask[frontTabOut[i]];//intvar2 przechowuje start,var1 end - po end bede sorotwal
		tempint[i][2] = frontTabOut[i];//zapamietuje pozycje w tablicy - zeby wiedziec co przekolorowac


	}
	int nPos = -1;
	//int isle = 0;
	int dsizemin1 = data->GetVolumeSize() - 1;
	for (int j = 0; j < frontTabOutLength; j++)
	{

		lp = 0, ap = 0;
		int count = 0;
		ap = frontTabOut[j];
		listS[lp] = ap;

		if (BITTESTX(tmpIslandMask[ap], tmpMaskBitNr) == 1)
		{
			isle++; //zwiekszam bo to kolejna wysepka
					//tmpIslandMask[ap] = isle; //zapalama pierwszy na ten kolorek

					/*nPos++;
					tempint2[nPos][2] = isle;
					tempint2[nPos][3] = ap;*/

			countTab[isle]++;
			int zeroIsle = islandMask[ap];
			//islandMask[ap] = 0;//czyszcze zeby nie przeszkadzalo - na chwile
			BITCLRX(tmpIslandMask[ap], tmpMaskBitNr)
				islandMask[ap] = isle;
			do
			{
				ap = listS[lp--];

				for (int i = 0; i < kernelSize; i++)
				{
					tmp = ap + kernel[i];
					if (!((tmp > dsizemin1) || (tmp < 0)))
					{
						//if ((islandMask[tmp] == zeroIsle) && (tmpIslandMask[tmp] == 0)) //jesli taki jak startowy w island mask i nie zapalony w tmp
						if ((islandMask[tmp] == zeroIsle) && ((BITTESTX(tmpIslandMask[tmp], tmpMaskBitNr) == 1)))//i jesli pochodzi z nowego frontu
						{
							//tmpIslandMask[tmp] = isle; //zalewam reszte stykajacych sie
							islandMask[tmp] = isle;
							BITCLRX(tmpIslandMask[tmp], tmpMaskBitNr)

								listS[++lp] = tmp;

							countTab[isle]++;

							if (lp > listSSize - 100)
							{
								//jesli zblizam sie do max bufora to powieksze bufor 1.5raza - powinno byc bardzo rzadko.
								int newSize = listSSize * 2;
								int* tmp = new int[newSize];
								memcpy(tmp, listS, listSSize * sizeof(int));
								delete[]listS;
								listS = tmp; listSSize = newSize;

								/*int *tmp = new int[listSSize];
								memcpy(tmp, listS, listSSize*sizeof(int));
								delete[]listS;
								int newSize = listSSize * 1.5;
								listS = new int[newSize];
								memcpy(listS, tmp, listSSize*sizeof(int));
								listSSize = newSize;*/
							}
						}
					}
				}
			} while (lp >= 0);
		}
		//powinienem miec zapalone w tmp island mask uniklne wartosci dla wysp -  dla wszystkich co w islandMask maja ==255, 



	}
	//for (int i = 0; i < frontTabOutLength; i++)
	//{
	//	islandMask[tempint[i][2]] = tempint[i][1];
	//}
	for (int i = 0; i < frontTabOutLength; i++)
	{
		tempint[i][0] = islandMask[frontTabOut[i]];
		islandMask[tempint[i][2]] = tempint[i][1];
	}

	//QSortInt3(tempint, 0, frontTabOutLength - 1, 3);
	//QSortInt3(tempint2, 0, frontTabOutLength - 1, 3);

	/*for (int i = 0; i < frontTabOutLength; i++)
	{
	tempint[i][1] = tempint2[i][2];
	islandMask[tempint[i][3]] -= 128;
	}*/
	if (showDebug == true)
	{


		/*	cout << "przed:" << endl;

			for (int i = 0; i < frontTabOutLength; i++)
			{
				cout << tempint[i][1];
			}
			cout << endl;
			for (int i = 0; i < frontTabOutLength; i++)
			{
				cout << tempint[i][0];
			}
			cout << endl << endl;*/
			//to 
	}

	QSortInt3(tempint, 0, frontTabOutLength - 1, 0);

	//	QSortInt1IntInt(tempint, 0, frontTabOutLength - 1);

	//teoretycznie posortowalem w tabelce tak, ze powinny byc pod sobą nowe i stare fronty

	// old  00000111122222
	// new  00111222233344
	//	int notChanged = 0;
	int divisions = 0;
	if (showDebug == true)
	{

		/*	cout << "po sortowaniu:" << endl;

			for (int i = 0; i < frontTabOutLength; i++)
			{
				cout << tempint[i][1];
			}
			cout << endl;
			for (int i = 0; i < frontTabOutLength; i++)
			{
				cout << tempint[i][0];
			}
			cout << endl << endl;*/
	}
	//rozdzielanie
	int start, end;
	int j;
	for (int i = 0; i < frontTabOutLength; i++)
	{
		start = i;
		int cc = 0;

		for (j = start + 1; j < frontTabOutLength; j++)
		{

			if (tempint[start][1] != tempint[j][1]) break;
			if (tempint[j - 1][0] != tempint[j][0])//nowyfront
			{
				//if (countTab[tempint[j][0]]>1) 
				cc++;
			}
		}
		end = j - 1;

		//powinienem miec:
		// start, end - pocz indexow frontu zrodlowego
		// cc - ilosc podzialow - jesli cc>0 to znaczy, ze robie nowe wyspy i zamieniam indexy

		if (cc > 0)
		{
			divisions++;
			maxIsleNr++;
			tempint[start][1] = maxIsleNr;
			islandMask[tempint[start][2]] = maxIsleNr;//koloruje - zmienione wysepki - jesli podzial frontu nastapil
			for (int j = start + 1; j <= end; j++)
			{
				if (countTab[tempint[j][0]] > 0)
				{

					if (tempint[j - 1][0] != tempint[j][0])
					{
						maxIsleNr++;

					}
					tempint[j][1] = maxIsleNr;
					islandMask[tempint[j][2]] = maxIsleNr;//koloruje - zmienione wysepki - jesli podzial frontu nastapil
				}
				else
				{
					//notChanged++;
				}
			}

		}
		else
		{
			//int origVal= tempint[start][1];
			//for (int j = start; j <= end; j++)
			//{
			//	
			//	tempint[j][0] = origVal;
			//	//islandMask[tempint[j][2]] = origVal;
			//}
		}

		i = j - 1;

	}


	if (showDebug == true)
	{
		//cout << "divisions: " << divisions << endl;

		//cout << "po zamianie:" << endl;

		//for (int i = 0; i < frontTabOutLength; i++)
		//{
		//	cout << tempint[i][1];
		//}
		//cout << endl;
		//for (int i = 0; i < frontTabOutLength; i++)
		//{
		//	cout << (int)(islandMask[tempint[i][2]]);// tempint[i][0];
		//}
		//cout << endl << endl;


		//cout << "not changed:" << notChanged << endl;
	}



	for (int i = 0; i < frontTabOutLength; i++)
	{
		delete tempint[i];
		//delete tempint2[i];
	}
	delete[]tempint;
	//delete[]tempint2;
	//delete[]isleTabStart;
	//delete[]isleTabEnd;
	delete[]kernel;;



}

//-[-----

void	RemapOutFrontTabWithDidivedIslands(int* frontTabOut, int frontTabOutLength, int* frontTabIn, int frontTabInLength, int* frontTabOld, int frontTabOldLength, QVector <JointData>* joints, unsigned short* islandMask, unsigned char* tmpIslandMask, ParentAndAvoidData* parentAndAvoid, int*& listS, int& listSSize, RawDataSet* data, int& maxIsleNr, int kernelType, bool is2D, bool showDebug, int tmpMaskBitNr, QVector<JointPoint3D>* jPts, int* _iterNr)
{
	//int iterNr = *_iterNr;
	//if (iterNr != NULL)	(*iterNr)++;
	//QVector<int>clearVector;
	if (maxIsleNr == NR_OF_MASK_COLORS - 1)
	{
		return;// clearVector;
	}

	int clearVal = 65535;
	QFile file("d:/test.txt");
	bool write = false;
	if ((file.open(QIODevice::WriteOnly)) && (showDebug == true))
	{
		write = true;
	}
	else
	{
		showDebug = false;
	}
	if (frontTabOutLength < 2)
	{
		return; //clearVector;
	}
	int loopSize = 4;
	double origin[3];
	double scale[3];
	data->GetOrigin(origin);
	data->GetScale(scale);

	//int jointTabIndices[256];//tu zapale indeksy dla łączących sie frontów do wyiczenia punktow

	int* jointTabIndices = new int[NR_OF_MASK_COLORS];
	for (int i = 0; i < NR_OF_MASK_COLORS; i++)
	{
		jointTabIndices[i] = -1;
	}

	//memset(jointTabIndices, -1, 256 * sizeof(int));
	int size = 1;
	int kernelSize;
	int* kernel = NULL;
	if (!is2D)
		kernel = GetGrowingKernel(kernelSize, size, data, kernelType);
	else
		kernel = GetGrowingKernel2D(kernelSize, size, data, kernelType);


	int lp, ap, tmp;
	int isle = -1;

	//frontTabOut - zawiera indeksy frontu

	int** tempint = new int* [frontTabOutLength];

	int* countTab = new int[NR_OF_MASK_COLORS];
	memset(countTab, 0, (NR_OF_MASK_COLORS) * sizeof(int));


	// isleTabStart wypelniona indeksami z maski - zrodlo

	for (int i = 0; i < frontTabOutLength; i++)
	{
		tempint[i] = new int[3];
		tempint[i][1] = islandMask[frontTabOut[i]];//intvar2 przechowuje start,var1 end - po end bede sorotwal
		tempint[i][2] = frontTabOut[i];//zapamietuje pozycje w tablicy - zeby wiedziec co przekolorowac
		if (tempint[i][1] == 65535)
		{
			tempint[i][1] = 0;
		}

	}
	int nPos = -1;
	//int isle = 0;
	int dsizemin1 = data->GetVolumeSize() - 1;
	for (int j = 0; j < frontTabOutLength; j++)
	{

		lp = 0, ap = 0;
		int count = 0;
		ap = frontTabOut[j];
		listS[lp] = ap;

		if (BITTESTX(tmpIslandMask[ap], tmpMaskBitNr) == 1)
		{
			isle++; //zwiekszam bo to kolejna wysepka
					//tmpIslandMask[ap] = isle; //zapalama pierwszy na ten kolorek

					/*nPos++;
					tempint2[nPos][2] = isle;
					tempint2[nPos][3] = ap;*/

			countTab[isle]++;
			int zeroIsle = islandMask[ap];

			BITCLRX(tmpIslandMask[ap], tmpMaskBitNr)
				islandMask[ap] = isle;
			do
			{
				ap = listS[lp--];

				for (int i = 0; i < kernelSize; i++)
				{
					tmp = ap + kernel[i];
					if (!((tmp > dsizemin1) || (tmp < 0)))
					{

						if ((islandMask[tmp] != 0) && ((BITTESTX(tmpIslandMask[tmp], tmpMaskBitNr) == 1)))//i jesli pochodzi z nowego frontu - zmiana - ze jest jakikolwiek front - może być złączenie!
						{

							islandMask[tmp] = isle;
							BITCLRX(tmpIslandMask[tmp], tmpMaskBitNr)

								listS[++lp] = tmp;

							countTab[isle]++;

							if (lp > listSSize - 100)
							{
								//jesli zblizam sie do max bufora to powieksze bufor 1.5raza - powinno byc bardzo rzadko.
								int newSize = listSSize * 2;
								int* tmp = new int[newSize];
								memcpy(tmp, listS, listSSize * sizeof(int));
								delete[]listS;
								listS = tmp; listSSize = newSize;


							}
						}
					}
				}
			} while (lp >= 0);
		}
		//powinienem miec zapalone w tmp island mask uniklne wartosci dla wysp -  dla wszystkich co w islandMask maja ==255, 



	}


	//ta petlla ma wykrywac jelsi wyspa jest o wielkosci 1 i ma sprobowac ja przekolorowac jesli jest 
	//inna majaca tego samego rodzica
	for (int i = 0; i < isle; i++)
	{
		if (countTab[i] == 1)
		{

			int parentNr = -1;
			int badPos = -1;
			for (int j = 0; j < frontTabOutLength; j++)
			{
				if (islandMask[frontTabOut[j]] == i)
				{
					parentNr = tempint[j][1];
					badPos = j;
					break;
				}

			}
			if (parentNr >= 0)
			{
				for (int j = 0; j < frontTabOutLength; j++)
				{
					if (j != badPos)
					{
						if ((tempint[j][1] == parentNr) && (islandMask[frontTabOut[badPos]] != islandMask[frontTabOut[j]]))
						{
							islandMask[frontTabOut[badPos]] = islandMask[frontTabOut[j]];//przekolorowuje tym co nie jest 1
							break;//
						}
					}
				}

			}


		}
	}


	for (int i = 0; i < frontTabOutLength; i++)
	{
		tempint[i][0] = islandMask[frontTabOut[i]];
		islandMask[tempint[i][2]] = tempint[i][1];
	}



	if (showDebug == true)
	{

		QTextStream stream(&file);

		stream << "przed sort:" << endl;

		for (int i = 0; i < frontTabOutLength; i++)
		{
			stream << tempint[i][1] << "\t";
		}
		stream << endl;
		for (int i = 0; i < frontTabOutLength; i++)
		{
			stream << tempint[i][0] << "\t";
		}
		stream << endl << endl;
		//to 
	}

	QSortInt3(tempint, 0, frontTabOutLength - 1, 0);

	if (showDebug == true)
	{

		QTextStream stream(&file);

		stream << "po sort:" << endl;

		for (int i = 0; i < frontTabOutLength; i++)
		{
			stream << tempint[i][1] << "\t";
		}
		stream << endl;
		for (int i = 0; i < frontTabOutLength; i++)
		{
			stream << tempint[i][0] << "\t";
		}
		stream << endl << endl;
	}

	//chce dopisac dzis 2019/02/19 obsluge sytuacji:

	//  old   9999999888887777888881111
	// new    0000000111112222333334444
	//bo jest
	//old   9999999888887777888881111
	//new   9999999888887777888881111

	// a chce
	//old  9999999888888888877771111
	//new  00000001111122222333334444


	//musze sprawdzic czy w [1] nie powtarzaja sie te same fronty po przerwie - jesli tak to przestawie

	int** tempint2 = new int* [frontTabOutLength];




	// isleTabStart wypelniona indeksami z maski - zrodlo

	for (int i = 0; i < frontTabOutLength; i++)
	{
		tempint2[i] = new int[3];
	}



	int nrOfColors = 0;
	int* colorsTab = new int[frontTabOutLength];

	nrOfColors++;
	colorsTab[0] = tempint[0][1];

	for (int i = 1; i < frontTabOutLength; i++)
	{
		if (tempint[i][1] != colorsTab[nrOfColors - 1])
		{
			colorsTab[nrOfColors++] = tempint[i][1];
		}

	}


	QSortInt(colorsTab, 0, nrOfColors - 1);
	bool needToResort = false;
	for (int i = 0; i < nrOfColors - 1; i++)
	{
		if (colorsTab[i] == colorsTab[i + 1])
		{
			i = i;
			needToResort = true;
			break;
			//wykrrycie sytuacji! gdzie trzeba bedzie przekleic fronty 
		}
	}


	if (needToResort == true)
	{
		int newNrOfColors = 1;

		for (int i = 1; i < nrOfColors; i++)
		{
			if (colorsTab[i] == colorsTab[i - 1])
			{
				continue;
			}
			else
			{

				colorsTab[newNrOfColors] = colorsTab[i];
				newNrOfColors++;
			}
		}
		nrOfColors = newNrOfColors;

		int posx = 0;
		///sortuje po kolorach
		for (int j = 0; j < nrOfColors; j++)
		{

			for (int i = 0; i < frontTabOutLength; i++)
			{
				if (tempint[i][1] == colorsTab[j])
				{
					tempint2[posx][0] = tempint[i][0];
					tempint2[posx][1] = tempint[i][1];
					tempint2[posx][2] = tempint[i][2];
					posx++;

				}
			}

		}

		//kopiuje do starej

		for (int i = 0; i < frontTabOutLength; i++)
		{
			tempint[i][0] = tempint2[i][0];
			tempint[i][1] = tempint2[i][1];
			tempint[i][2] = tempint2[i][2];



		}


	}


	for (int i = 0; i < frontTabOutLength; i++)
	{
		delete tempint2[i];
		//delete tempint2[i];
	}
	delete[]tempint2;

	delete[]colorsTab;

	//	QSortInt1IntInt(tempint, 0, frontTabOutLength - 1);

	//teoretycznie posortowalem w tabelce tak, ze powinny byc pod sobą nowe i stare fronty

	// old  00000111122222
	// new  00111222233344
	//	int notChanged = 0;
	int divisions = 0;


	////	vtkSmartPointer<vtkPoints> centerPts = vtkSmartPointer<vtkPoints>::New();
	//	vtkSmartPointer<vtkPoints> xPts1 = vtkSmartPointer<vtkPoints>::New();
	//	vtkSmartPointer<vtkPoints> xPts2 = vtkSmartPointer<vtkPoints>::New();
	//	vtkSmartPointer<vtkIntArray> ints1 = vtkSmartPointer<vtkIntArray>::New();
	//	vtkSmartPointer<vtkIntArray> ints2 = vtkSmartPointer<vtkIntArray>::New();

	int pI[3];
	double pD[3];
	double pD1[3];
	//double pI[3];

	int start, end;
	int j;
	for (int i = 0; i < frontTabOutLength; i++)
	{
		start = i;
		int divCount = 0;
		int joinCount = 0;
		bool join = false;
		//int startx = tempint[start][1];
		for (j = start + 1; j < frontTabOutLength; j++)
		{
			bool chgOld = (tempint[j][1] != tempint[j - 1][1]);
			bool chgNew = (tempint[j - 1][0] != tempint[j][0]);
			if (tempint[j][1] == 87)
			{
				i = i;
			}

			if ((chgNew == true) && (chgOld == false))
			{
				if (joinCount > 0)
				{
					break;
				}
				divCount++;
			}
			else if ((chgNew == true) && (chgOld == true)) break;
			else if ((chgNew == false) && (chgOld == true))
			{
				join = true;
				//startx = tempint[j][1];


				if ((tempint[j][1] > NR_OF_MASK_COLORS - 1) || (tempint[j - 1][1] > NR_OF_MASK_COLORS - 1) || (tempint[j][1] < 0) || (tempint[j - 1][1] < 0))
				{
					j = j;
				}
				bool zeroTest = false;
				//if (joinCount == 0)
				if (jointTabIndices[tempint[j - 1][1]] < 0)
				{
					if (divCount > 0)
					{
						break;
					}
					jointTabIndices[tempint[j - 1][1]] = joinCount;
					joinCount++;
					zeroTest = true;
				}
				if (jointTabIndices[tempint[j][1]] < 0)
				{
					if (divCount > 0)
					{
						break;
					}
					jointTabIndices[tempint[j][1]] = joinCount;
					joinCount++;
				}
				else if (zeroTest == true)
				{
					j = j;
				}

			}



		}
		end = j - 1;
		// tutaj mam koniec analizowanego kawałka
		// muszę decydować co robić gdy się dzieli, albo łączy albo nic się nie dzieje

		if (
			divCount > 0)
		{


			maxIsleNr++; if (maxIsleNr == NR_OF_MASK_COLORS) maxIsleNr = NR_OF_MASK_COLORS - 1;;
			//	parentTab[maxIsleNr * 10] = 1;
			//	parentTab[maxIsleNr*10+1] = tempint[start][1];
			parentAndAvoid[maxIsleNr].parent.append(tempint[start][1]);

			//tempint[start][0] = maxIsleNr;
			islandMask[tempint[start][2]] = maxIsleNr;//koloruje - zmienione wysepki - jesli podzial frontu nastapil
			for (int j = start + 1; j <= end; j++)
			{

				if (tempint[j - 1][0] != tempint[j][0])
				{
					maxIsleNr++; if (maxIsleNr == NR_OF_MASK_COLORS) maxIsleNr = NR_OF_MASK_COLORS - 1;;
					//parentTab[maxIsleNr * 10] = 1;
					//parentTab[maxIsleNr*10+1] = tempint[j][1]; // nowy front maxIsleNr pochodzi od tego
					parentAndAvoid[maxIsleNr].parent.append(tempint[start][1]);
				}
				//tempint[j][0] = maxIsleNr;
				islandMask[tempint[j][2]] = maxIsleNr;
			}

		}
		else if (joinCount > 0)//ŁĄCZENIE!!!!!
		{

			// nowy cel 20187.05.30 - przywrocic algorytm front break!!!!!



			int jC = joinCount;// ?????????


							   // przy złączeniu prosto - wszystkie maluje tak samo
			//maxIsleNr++; if (maxIsleNr == 256) maxIsleNr = 255;;
			//parentTab[maxIsleNr * 10] = 1;
			//parentTab[maxIsleNr*10+1] = tempint[start][1];//biore pierwszy jako zrodlo - moze byc ich wiecej ale chyba 1 wystarczy

			int guidedV = tempint[start][1];// jointTabIndices[tempint[start][1]];
			int zeroFront = tempint[start][1];//zapamietuje tylko dla podzialu na 2
			//data->GetVoxelXYZPos(tempint[start][2], pI);
			data->GetVoxelRealPos(tempint[start][2], pD);
			//
			//xPts1->InsertNextPoint(pI[0], pI[1], pI[2]);//to nie wazne ze zapamietuje inty - bo interesuje mnie tylko odleglosc najmniejsza do namierzenia
			//xPts1->InsertNextPoint(pD);
			//ints1->InsertNextValue(tempint[start][2]);

			//parentTab[maxIsleNr * 10] = nrOfParents;
			//parentTab[maxIsleNr * 10 + nrOfParents] = tempint[start][1];//biore pierwszy jako zrodlo - moze byc ich wiecej ale chyba 1 wystarczy
			parentAndAvoid[maxIsleNr].parent.append(tempint[start][1]);
			int nrOfParents = 1;// parentAndAvoid[maxIsleNr].parent.count();

			int usedValues[10] = { -1 };

			int usedValuesCount[10] = { 0 };//bede trzymal jakie fronty są duze - wyczyszcze (front break) wszystkie oprocz najgrubszego
			for (int k = 0; k < 10; k++)
			{
				usedValues[k] = -1;
				usedValuesCount[k] = 0;
			}
			usedValues[0] = guidedV;

			/*	data->GetVoxelXYZPos(tempint[start][2], pI);
				pD[0] = pI[0]; pD[1] = pI[1]; pD[2] = pI[2];*/
				//	centerPts->InsertNextPoint(pD);

			usedValuesCount[0]++;
			for (int j = start + 1; j <= end; j++)
			{

				//	data->GetVoxelXYZPos(tempint[j][2], pI);
					//data->GetVoxelRealPos(tempint[j][2], pD);

					//xPts1->InsertNextPoint(pI[0], pI[1], pI[2]);//to nie wazne ze zapamietuje inty - bo interesuje mnie tylko odleglosc najmniejsza do namierzenia
					//xPts1->InsertNextPoint(pD);
					//ints1->InsertNextValue(tempint[start][2]);
					//if (tempint[j][1] == zeroFront)
					//{
					//	//xPts1->InsertNextPoint(pI[0], pI[1], pI[2]);
					//	xPts1->InsertNextPoint(pD);
					////	ints1->InsertNextValue(tempint[j][2]);
					//}
					//else
					//{
					//	//xPts2->InsertNextPoint(pI[0], pI[1], pI[2]);
					//	xPts2->InsertNextPoint(pD);
					////	ints2->InsertNextValue(tempint[j][2]);
					//}


				bool check = false;
				for (int k = 0; k < nrOfParents; k++)
				{
					//if (jointTabIndices[tempint[j][1]] == usedValues[k])
					if (tempint[j][1] == usedValues[k])
					{
						usedValuesCount[k]++;
						check = true;

						/*		centerPts->GetPoint(k, pD);
								data->GetVoxelXYZPos(tempint[j][2], pI);
								pD[0] += pI[0]; pD[1] += pI[1]; pD[2] += pI[2];
								centerPts->SetPoint(k, pD);*/

						break;
					}
				}


				//	if (jointTabIndices[tempint[j][1]] != guidedV)
				if (check == false)
				{
					usedValuesCount[nrOfParents]++;
					nrOfParents++;
					guidedV = tempint[j][1];// jointTabIndices[tempint[j][1]];
					usedValues[nrOfParents - 1] = guidedV;



				}
			}

			//qDebug() << "joint: " << maxIsleNr << " " << parentAndAvoid[maxIsleNr]. << " " << parentTab[maxIsleNr * 10 + 1] << " " << parentTab[maxIsleNr * 10 + 2];
			int maxPos = 0; int maxCount = 0;
			int maxIndex = 0;
			for (int l = 0; l < nrOfParents; l++)
			{
				if (usedValuesCount[l] > maxCount)
				{
					maxCount = usedValuesCount[l];
					maxPos = usedValues[l];
					maxIndex = l;
				}
				qDebug() << "front: " << usedValues[l] << " count: " << usedValuesCount[l];
			}

			//maxPos - najgrubszy - tym sprobuje podmienic aktyualny front a dla reszty dodam zielone kulki.

			//mam juz wartosc frontu ktorego nie bede czyscil - ponizej algorytm front break - czyszcze z poprzedniego kroku wszystkie ciensze od najgrubszego
			//for (int j = start ; j <= end; j++)
			//{
			//	if (jointTabIndices[tempint[j][1]] != maxPos)
			//	{
			//		int posToClean = tempint[j][2]; 
			//		//islandMask[posToClean] = clearVal;//cos trzeba wstwaic - zapora
			//		clearVector.append(posToClean);
			//	}
			//}
			//for (int l = 0; l < nrOfParents; l++)
			//{
			//	for (int m = 0; m < nrOfParents; m++)
			//	{
			//		if (l != m)
			//		{
			//			parentAndAvoid[usedValues[l]].avoid.append(usedValues[m]);// dla danego frontu wchodzacego do jointa dodaje avoid na wszystkie wartosci frntow pozostalych - nie moze trace wchodzic  na pozostale fronty
			//		}
			//	}
			//}

			//parentAndAvoid[maxIsleNr].createdFromJoint = true; //wstawiam cos takiego, bo to oznacza ze front powstal w wyniku złącznenia i nie mozna generowac na jego koncu kulki - chyba
			// 


			// mam tablicę usedValues - tam sa wartosci frontow ktore sie lacza
			// wiem ktory jest najgrubszy - maxPos

			//robie petle po wszystkich pktach zlaczenia 
			// wyliczam dla kazdego z nie najgrubszego pkt na srodku 
			// wyliczam z frontIn srodek dla tego samego frotnu - to idzie do jointa
			//  czyszcze dany frontIn



			/// (1)     AAAAAABBBB - tu wszystko zamienia na AAA(bo najgrubszy) i dla BBBB policze srodek
			/// (2)     AAA    BBB - ten BBB wywale
			/// (3)     AA     BBB - dla tego policze srodek  - jesli nie bedzie istnial zaden BBB to biore parent(BBB) ten juz musi istniec
			/// (4)     AA     BBB 

			if (maxPos == 156)
			{
				i = i;
			}

			int xSize = (nrOfParents) * 3;
			double* ppp = new double[xSize];
			double* ppp1 = new double[xSize];
			for (int l = 0; l < xSize; l++)
			{
				ppp[l] = 0;
				ppp1[l] = 0;
			}



			//2019.02.22 - pomysl zeby startowe propozycje srodkow nie byly tymi ktore sa z aktualnego frontu tylko z poprzedniej iteracji 

			for (int l = 0; l < nrOfParents; l++)
			{
				if (l != maxIndex)
				{
					double p[3] = { 0 };
					int pt[3];
					int pCount = 0;
					for (int j = 0; j < frontTabInLength; j++)
					{
						//if (tempint[j][1] == usedValues[l])
						if (islandMask[frontTabIn[j]] == usedValues[l])
						{


							data->GetVoxelXYZPos(frontTabIn[j], pt);
							p[0] += pt[0]; p[1] += pt[1]; p[2] += pt[2];
							pCount++;
						}
					}


					if (pCount > 0)
					{
						for (int k = 0; k < 3; k++) { p[k] /= (double)pCount;		ppp[l * 3 + k] = origin[k] + p[k] * scale[k]; }
					}
					// mam punkt trzeba go pokazac!!!!

				}
			}

			double centerMain[3] = { 0 };
			// dla najgrubszego - dodam puntkt najblizszy do centrum srodek z pozostalych z OUT frontu - bo on zostal nie przerwany
			{
				double p[3] = { 0 };
				int pt[3];
				int pCount = 0;
				for (int j = 0; j < frontTabOutLength; j++)
				{
					//if (tempint[j][1] == usedValues[l])
					if (islandMask[frontTabOut[j]] == usedValues[maxIndex])
					{


						data->GetVoxelXYZPos(frontTabIn[j], pt);
						p[0] += pt[0]; p[1] += pt[1]; p[2] += pt[2];
						pCount++;
					}
				}


				if (pCount > 0)
				{
					for (int k = 0; k < 3; k++) { p[k] /= (double)pCount;		centerMain[k] = origin[k] + p[k] * scale[k]; }
				}

			}


			if ((usedValues[0] == 87) || (usedValues[1] == 87))
			{
				int i = 1;

			}




			//analiza srodkow frontow aktualnych ale tylko cienkich - grubasa nie!!! ( (1)  cz 2)
			for (int l = 0; l < nrOfParents; l++)
			{
				if (l != maxIndex)
					bool test = ReplaceNotValidCenterPointByClosestValid(ppp + l * 3, data, usedValues[l], islandMask, frontTabIn, frontTabInLength);
			}

			//wyliczam centrum z nowego
			//szukam najblizszego i zamieniam
			int mPos = -1;
			double dist = INT_MAX;
			for (int l = 0; l < nrOfParents; l++)
			{
				if (l != maxIndex)
				{
					double* pp = ppp + 3 * l;
					double tmpDist = GetDistance(pp, centerMain);
					if (tmpDist < dist)
					{
						dist = tmpDist;
						mPos = l;
					}
				}
			}

			if (mPos != -1)
			{
				for (int k = 0; k < 3; k++)
				{
					ppp[maxIndex * 3 + k] = ppp[mPos * 3 + k];
				}
				bool test = ReplaceNotValidCenterPointByClosestValid(ppp + maxIndex * 3, data, usedValues[maxIndex], islandMask, frontTabOut, frontTabOutLength);
			}
			else
			{
				int i = 0;//kaszana
			}








			JointData joint;
			joint.mainValue = maxPos;
			for (int l = 0; l < nrOfParents; l++)
			{
				joint.inputValues.append(usedValues[l]);

				JointPoint3D xP;
				xP.point3D[0] = ppp[l * 3];		xP.point3D[1] = ppp[l * 3 + 1];		xP.point3D[2] = ppp[l * 3 + 2];
				xP.frontNumber = usedValues[l];/*usedValues[l];*/
				jPts->append(xP); //zmiana bo to musi byc przekolorowany grubas@!!!!

				if (l != maxIndex)//oprocz najgrubszego
				{


					for (int j = 0; j < frontTabOutLength; j++)
					{
						if (islandMask[frontTabOut[j]] == usedValues[l])
						{
							islandMask[frontTabOut[j]] = clearVal;//ma byc czyste!!!!
							joint.frontBreakPts.append(frontTabOut[j]);
						}

					}
				}

			}

			JointPoint3D xP;
			xP.point3D[0] = 0;		xP.point3D[1] = 0;		xP.point3D[2] = 0;
			xP.frontNumber = -2;//zly punkt!
			jPts->append(xP);

			joints->append(joint);

			////teoretycznie powinienem miec po tej pteli powinienem miec 3 punkty do kazdego jointa!!!
			//int *selectedPrevFrontTab = new int[nrOfParents];
			////teraz odpowiadajace z poprzedniego frotnu!!! 
			//bool takiNowyTest = true;
			//for (int l = 0; l < nrOfParents; l++)
			//{
			//	selectedPrevFrontTab[l] = 0;
			//	if (l != maxIndex)
			//	{
			//		// drugi punkt do jointa musi byc z (  (3) )
			//		bool test = ReplaceNotValidCenterPointByClosestValid(ppp1 + l * 3, data, usedValues[l], islandMask, frontTabOld, frontTabOldLength);//sprawdz
			//		if (test == false)
			//		{
			//			//(4)

			//			int parentIndex = parentAndAvoid[usedValues[l]].parent.at(0);
			//			test = ReplaceNotValidCenterPointByClosestValid(ppp1 + l * 3, data, parentIndex, islandMask, frontTabOld, frontTabOldLength);
			//			if (test == false)
			//			{
			//				test = test;
			//				qDebug() << " bad input";
			//				takiNowyTest = false;
			//			}
			//			else
			//			{
			//				selectedPrevFrontTab[l] = parentIndex;
			//			}
			//		}
			//		else
			//		{
			//			selectedPrevFrontTab[l] = usedValues[l];
			//		}
			//	}
			//}

			////robie front break;!!!!

			//JointData joint;
			//joint.mainValue = maxPos;

			//
			//if (takiNowyTest == true)
			//{
			//	for (int l = 0; l < nrOfParents; l++)
			//	{

			//		joint.inputValues.append(usedValues[l]);

			//		if (l != maxIndex)//oprocz najgrubszego
			//		{

			//			//for (int j = start; j <= end; j++)
			//			for (int j = 0; j < frontTabInLength; j++)
			//			{
			//				//if (tempint[j][1] == usedValues[l])
			//				if (islandMask[frontTabIn[j]] == usedValues[l])
			//				{
			//					islandMask[frontTabIn[j]] = clearVal;//ma byc czyste!!!!
			//					joint.frontBreakPts.append(frontTabIn[j]);
			//					//frontBreakPoints->append(frontTabIn[j]);//chce zrobic takie glyphs

			//				}
			//			}


			//			JointPoint3D xPTest;

			//			JointPoint3D xP;
			//			xP.point3D[0] = ppp[l * 3];		xP.point3D[1] = ppp[l * 3 + 1];		xP.point3D[2] = ppp[l * 3 + 2];
			//			xP.frontNumber = maxPos;/*usedValues[l];*/						jPts->append(xP); xPTest = jPts->at(jPts->count() - 1); //zmiana bo to musi byc przekolorowany grubas@!!!!

			//			if (jPts->count() == 59)
			//			{
			//				j = j;
			//			}

			//			xP.point3D[0] = ppp1[l * 3];		xP.point3D[1] = ppp1[l * 3 + 1];		xP.point3D[2] = ppp1[l * 3 + 2];
			//			xP.frontNumber = selectedPrevFrontTab[l];/* usedValues[l];	*/					jPts->append(xP); xPTest = jPts->at(jPts->count() - 1);//zmiana bo czasem nie jest to front poprzedni tylko jego rodzic
			//			//JointPoint3D xP;
			//			//ODDZIELACZ!!!!


			//			if (jPts->count() == 59)
			//			{
			//				j = j;
			//			}


			//			xP.point3D[0] = 0;		xP.point3D[1] = 0;		xP.point3D[2] = 0;
			//			xP.frontNumber = -2;//zly punkt!


			//			jPts->append(xP); xPTest = jPts->at(jPts->count() - 1);


			//			if (jPts->count() == 59)
			//			{
			//				j = j;
			//			}

			//		}
			//	}
			//	joints->append(joint);
			//}
			delete[]ppp;
			delete[]ppp1;
			///delete[]selectedPrevFrontTab;



			//if (jPts == NULL)
			{
				for (int j = start; j <= end; j++)
				{
					//tempint[j][0] = maxIsleNr;
					if (islandMask[tempint[j][2]] != clearVal)
						islandMask[tempint[j][2]] = maxPos;// maxIsleNr;
					tempint[j][1] = maxPos;
				}
			}


		}

		i = j - 1;
	}
























	if (showDebug == true)
	{
		//cout << "divisions: " << divCount << " joins: " << divCount;
		QTextStream stream(&file);

		stream << "po zamianie:" << endl;

		for (int i = 0; i < frontTabOutLength; i++)
		{
			stream << tempint[i][1] << "\t";
		}
		stream << endl;
		for (int i = 0; i < frontTabOutLength; i++)
		{
			stream << (int)(islandMask[tempint[i][2]]) << "\t";// tempint[i][0];
		}
		stream << endl << endl;


		//cout << "not changed:" << notChanged << endl;
	}



	for (int i = 0; i < frontTabOutLength; i++)
	{
		delete tempint[i];
		//delete tempint2[i];
	}

	delete[]countTab;
	delete[]jointTabIndices;
	delete[]tempint;
	//delete[]tempint2;
	//delete[]isleTabStart;
	//delete[]isleTabEnd;
	delete[]kernel;;

	if (write == true)
	{
		file.close();
	}

	//return clearVector;
}
//--------------------------------------------
bool ReplaceNotValidCenterPointByClosestValid(double* p, RawDataSet* data, unsigned short properVal, unsigned short* valVolume, int* frontTab, int frontTabLength)
{
	// jesli properval nie ma we frontTab zwroci false
	if ((data != NULL) && (frontTab != NULL) && (p != NULL) && (valVolume != NULL))
	{

		int pt[3];
		double scale[3];
		data->GetScale(scale);
		double origin[3];
		data->GetOrigin(origin);
		double tol = 1e-6;

		int pCount = 0;
		if ((fabs(p[0]) > tol) && (fabs(p[1]) > tol) && (fabs(p[1]) > tol))
		{
			pCount = 1;
			//zdefiniowany punkt! tu chce zeby byl to punkt srodka frontu rozdzileajacego!//dla dwoch to bedzie dzialac dla 3 jeszcze nie wiem
		}
		else
		{
			p[0] = 0;
			p[1] = 0;
			p[2] = 0;

			for (int k = 0; k < frontTabLength; k++)
			{
				int val = valVolume[frontTab[k]];
				if (val == properVal)
				{
					data->GetVoxelXYZPos(frontTab[k], pt);
					p[0] += pt[0]; p[1] += pt[1]; p[2] += pt[2];
					pCount++;
				}


				// mam punkt trzeba go pokazac!!!!
			}
			if (pCount > 0)
			{
				for (int l = 0; l < 3; l++) { p[l] /= (double)pCount;		p[l] = origin[l] + p[l] * scale[l]; }
			}
		}

		if (pCount > 0)
		{

			//qDebug() << j << " " << p[0] << " " << p[1] << " " << p[2] << " " << pCount;


			//fkcja zamienia punkt p na inny jesli pkt wejsciowy po przejsciu do wolumenu nie ma wartosci properVal
			//szukam wtedy najblizszego punktu do p sposrod wlasciwych z calego frontTab i zamieniam 



			int pI[3]; double pD[3];
			double scale1[3] = { 1.0 / scale[0],1.0 / scale[1],1.0 / scale[2] }; for (int k = 0; k < 3; k++) { pD[k] = (p[k] - origin[k]) * scale1[k];	pI[k] = Round(pD[k]); }
			int actualVoxelPos = data->GetVoxelPos(pI[0], pI[1], pI[2]);

			//trzeba sprawdzic czy actual voxel pos nalzey do wlasciwego frontu zeby nie lezal na starym!!!

			int check = false;//jesli tak zostanie po pretli to oznacza, ze nie znalazl zintowanego punktu wsrod aktualnych - trzeba znalezc inny co nalezy
			for (int k = 0; k < frontTabLength; k++)
			{
				if (frontTab[k] == actualVoxelPos)
				{
					check = true;
					break;
				}
			}
			if ((valVolume[actualVoxelPos] != properVal) || (check == false))
			{
				double minDist = INT_MAX;
				double minPt[3] = { 0 };
				double pTmp[3];
				for (int k = 0; k < frontTabLength; k++)
				{
					int val = valVolume[frontTab[k]];
					if (val == properVal)
					{
						data->GetVoxelXYZPos(frontTab[k], pt);			pTmp[0] = pt[0]; pTmp[1] = pt[1]; pTmp[2] = pt[2];

						for (int l = 0; l < 3; l++) { pTmp[l] = origin[l] + pTmp[l] * scale[l]; }

						double dist = GetDistance(p, pTmp);
						if (dist < minDist)
						{
							minDist = dist;
							minPt[0] = pTmp[0]; minPt[1] = pTmp[1]; minPt[2] = pTmp[2];
						}
						//break;
					}
				}

				p[0] = minPt[0]; p[1] = minPt[1]; p[2] = minPt[2];

			}


			return true;
		}
		else return false;

	}
	return false;


}
//-------------------------------------------------
double GetMaskCoinScale(maskClass* mc, double diameter)
{
	double scale = 1.0;

	//przebiegam po masce znajduje srodek
	// robie dylacje - biore tylko obwodke
	// licze promienie - biore ich mediane
	//wyliczam skale 

	int centerx = 0;
	int centery = 0;
	int count = 0;
	int pos = 0;
	for (int j = 0; j < mc->h; j++)
	{
		for (int i = 0; i < mc->w; i++)
		{
			if (mc->data[pos++] > 0)
			{
				centerx += i;
				centery += j;
				count++;
			}
		}
	}

	if (count > 0)
	{
		centerx /= count;
		centery /= count;




		RawDataSet* noneData = new RawDataSet(mc->w, mc->h, 1, 1, 1, 1, 1, false);

		int kSize;
		int kerenelSize2 = 0;
		int size2 = 1;
		int kernelType = 1;//kolko
		int* kernel2 = GetGrowingKernel2D(kerenelSize2, size2, noneData, kernelType);
		int tmp;
		int dsizemin1 = noneData->GetVolumeSize() - 1;


		int nrOfDilatePoints = 0;
		int nSizeX = mc->w;//???????????
		int nSizeY = mc->h;//???????????
		int startPos = 0; //od drugiego wiersza
		bool stop;

		double nnx1d = 1.0 / nSizeX;
		//contur extraction

		int frontVal = 1;
		int backVal = !frontVal;
		int countBound = 0;
		int x, y;
		double* radTab = new double[nSizeY * nSizeX];

		for (int jj = 0; jj < nSizeY; jj++)
		{

			for (int ii = 0; ii < nSizeX; ii++)
			{
				if (mc->data[startPos] == frontVal)
				{
					stop = false;
					for (int kk = 0; kk < kerenelSize2; kk++)
					{

						tmp = startPos + kernel2[kk];
						if ((tmp > dsizemin1) || (tmp < 0)) { continue; }
						else {
							//jakikolwiek zapalony
							if (mc->data[tmp] == backVal) {
								stop = true;
								break;
							}
						}


					}
					if (stop)
					{
						y = startPos * nnx1d;
						x = startPos - y * nSizeX;

						double dist = sqrt((x - centerx) * (x - centerx) + (y - centery) * (y - centery));
						radTab[countBound++] = dist;

						;

					}
				}
				startPos++;
			}


		}

		double radius = Median(radTab, countBound - 1);
		if (radius > 0)
		{

			scale = (diameter / (2.0 * radius));
		}
		delete[]radTab;

		delete noneData;

	}
	return scale;

}

//-------------------------------------------------------

void ColorizeIslandsOnUnsignedShortMask(unsigned short* tab, int ww, int hh, int inputVal, double* params, int* listS, int* listTmp, unsigned int* tabxCopy)
{

	int kernelSize = 8;
	int size = 1;
	int* kernel = new int[kernelSize];
	int* kernelInc = new int[kernelSize];
	int pos = 0;

	for (int j = -size; j < size + 1; j++)
	{
		for (int i = -size; i < size + 1; i++)
		{
			if (!((i == 0) && (j == 0)))
			{
				if (fabs(i) != fabs(j))
				{

					kernel[pos] = i + j * ww;
					kernelInc[pos++] = j;
				}
			}

		}
	}

	int ssize = ww * hh;

	int minArea = params[0];
	int maxArea = params[1];
	int minWidth = params[2];
	int maxWidth = params[3];
	int minHeight = params[4];
	int maxHeight = params[5];

	int coin_Area = params[6];
	double coin_Diameter = params[7];


	int startPos = 0;
	int ap = 0;
	int lp = 0;
	int count;
	int tmp;
	int dsizemin1 = ssize - 1;
	int nr0 = 0;
	count = 0;
	int currPos = 0;
	double nx1d = 1.0 / ww;
	ap = currPos;
	lp = 0;
	listS[0] = currPos;
	listTmp[(count++)] = currPos;
	//tab[ap] = 255;
	int oldy;
	int x, y;




	int morphologyType = 1;// params[morphologyTypeID];
	int morphologyKernelType = 0;// params[morphologyKernelTypeID];



	int objectNr = 0;
	int* dilatePoints = listS;//new int[sliceSize];


	int jStart = 0;
	int jStop = ssize;

	QList <int> centerList;






	int maxX = 0;
	int maxY = 0;

	double xScale = 1.0;
	double  yScale = 1.0;


	//------------
	//ppierwszy przebieg zalewania wysp zeby je pokolorowac i wyczyscic te co są słabe
	int nrOfObjects = inputVal;
	for (int j = jStart; j < jStop; j++)
	{
		int ccc = centerList.count();

		if (ccc > 0)
		{
			currPos = centerList.at(j);
		}
		else
		{
			currPos = j;
		}

		if (tab[currPos] == inputVal)
		{
			int minx = ww;
			int maxx = 0;
			int miny = hh;
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

				oldy = ap * nx1d;

				for (int i = 0; i < kernelSize; i++)
				{
					tmp = ap + kernel[i];
					y = tmp * nx1d;
					x = tmp - y * ww;

					if (!((tmp > dsizemin1) || (tmp < 0) || (y != oldy + kernelInc[i]))) //brzydkie
					{
						if (tab[tmp] == inputVal)
						{
							tab[tmp] = 65535;
							listS[++lp] = tmp;
							listTmp[count++] = tmp;



							if (x < minx) minx = x;
							if (x > maxx) maxx = x;
							if (y < miny) miny = y;
							if (y > maxy) maxy = y;
						}
					}
				}
			} while (lp >= 0);

			int width = maxx - minx;
			int height = maxy - miny;

			bool superTest = true;
			if (count > coin_Area)
			{
				//policze i zmienie scale dla calego volumenu


			}
			else
				if (((count < minArea) || (count > maxArea) || (width < minWidth) || (width > maxWidth) || (height < minHeight) || (height > maxHeight)))
				{

					//if ((count>coin_Area)) //czyszcze wielkie bloby wieksze od coina

					//wyczysc
					for (int i = 0; i < count; i++)
					{
						tab[listTmp[i]] = 0;
						if (tabxCopy != NULL) tabxCopy[listTmp[i]] = 0;
					}	//{	mask0[listS[dsizemin1-i]]=0;}
					superTest = false;

				}


			if (superTest == true) //sprawdz width i height - jesli zle to wyczysc jesli ok to zamaluj na 1
			{
				//if ((shape>0) && (shape<4))
				{
					int pixelArea = count;

					//nowe dane

					int nnx = maxx - minx + 1;
					int nny = maxy - miny + 1;

					int padd = 2;
					int nSizeX = nnx + padd * 2;
					int nSizeY = nny + padd * 2;
					int nSsize = nSizeX * nSizeY;

					int padd1 = 1;

					int nSizeX1 = nnx + padd1 * 2;
					int nSizeY1 = nny + padd1 * 2;

					if (nSizeX > maxX) maxX = nSizeX;
					if (nSizeY > maxY) maxY = nSizeY;


					nrOfObjects++;

					for (int k = 0; k < count; k++)
					{
						tab[listTmp[k]] = nrOfObjects;

					}


				}

			}
		}
	}


	params[9] = maxX;
	params[10] = maxY;
	params[8] = nrOfObjects;

	delete[]kernelInc;
	delete[]kernel;



}
//------------------------------------------
//(unsigned short *tab2, int ww, int hh, int inputVal, double *params, int *listS, int *listTmp, QImage *img, bool test)
/*

*/


//----------------------------------------------
QPolygonF	ImproveSeparateMasks(unsigned short* tab2, int ww, int hh, int inputVal, double* params, int* listS, int* listTmp, QImage* img, bool test)
{
	unsigned short* tab = new unsigned short[ww * hh];
	memcpy(tab, tab2, ww * hh * sizeof(unsigned short));
	// w lines zapisze kreski do podziałów - one beda potem wykorzystane do zamalowania na oryginalnym obrazie
	QPolygonF lines;
	unsigned int* tabxCopy = (unsigned int*)(img->bits());
	int kernelSize = 8;
	int size = 1;
	int* kernel = new int[kernelSize];
	int* kernelInc = new int[kernelSize];
	int pos = 0;

	for (int j = -size; j < size + 1; j++)
	{
		for (int i = -size; i < size + 1; i++)
		{
			if (!((i == 0) && (j == 0)))
			{
				kernel[pos] = i + j * ww;
				kernelInc[pos++] = j;
			}

		}
	}

	int ssize = ww * hh;

	int minArea = params[0];
	int maxArea = params[1];
	int minWidth = params[2];
	int maxWidth = params[3];
	int minHeight = params[4];
	int maxHeight = params[5];
	int minAreaDivide = params[9];

	int coin_Area = params[6];
	double coin_Diameter = params[7];


	int startPos = 0;
	int ap = 0;
	int lp = 0;
	int count;
	int tmp;
	int dsizemin1 = ssize - 1;
	int nr0 = 0;
	count = 0;
	int currPos = 0;
	double nx1d = 1.0 / ww;
	ap = currPos;
	lp = 0;
	listS[0] = currPos;
	listTmp[(count++)] = currPos;
	//	tab[ap] = 255;
	int oldy;
	int x, y;


	int lagrestArea = 0;

	int morphologyType = 0;// params[morphologyTypeID];
	int morphologyKernelType = 1;// params[morphologyKernelTypeID];



	int objectNr = 0;
	int* dilatePoints = listS;//new int[sliceSize];


	int jStart = 0;
	int jStop = ssize;

	QList <int> centerList;

	QPainter p(img);


	QFont font = p.font();
	font.setPointSize(1);

	p.setFont(font);




	int maxX = 0;
	int maxY = 0;

	double xScale = 1.0;
	double  yScale = 1.0;

	int blobCount = 0;
	for (int j = jStart; j < jStop; j++)
	{
		int ccc = centerList.count();

		if (ccc > 0)
		{
			currPos = centerList.at(j);
		}
		else
		{
			currPos = j;
		}

		if ((tab[currPos] > 0) && (tab[currPos] < 65535))
		{
			int minx = ww;
			int maxx = 0;
			int miny = hh;
			int maxy = 0;
			int x, y;
			nr0 = 0;
			count = 0;
			ap = currPos;
			lp = 0;
			listS[0] = currPos;
			listTmp[(count++)] = currPos;
			int oldx, oldy;
			int currFront = tab[ap];

			//zalewam
			do
			{
				ap = listS[lp--];

				oldy = ap * nx1d;

				for (int i = 0; i < kernelSize; i++)
				{
					tmp = ap + kernel[i];
					y = tmp * nx1d;
					x = tmp - y * ww;

					if (!((tmp > dsizemin1) || (tmp < 0) || (y != oldy + kernelInc[i]))) //brzydkie
					{
						if (tab[tmp] == currFront)
						{
							tab[tmp] = 65535;
							listS[++lp] = tmp;
							listTmp[count++] = tmp;



							if (x < minx) minx = x;
							if (x > maxx) maxx = x;
							if (y < miny) miny = y;
							if (y > maxy) maxy = y;
						}
					}
				}
			} while (lp >= 0);

			int width = maxx - minx;
			int height = maxy - miny;

			if (count > lagrestArea) lagrestArea = count;

			bool superTest = true;
			if (count > coin_Area)
			{
				//policze i zmienie scale dla calego volumenu
				int a = 5;

			}
			else
				if (((count < minAreaDivide) || (width < minWidth) || (width > maxWidth) || (height < minHeight) || (height > maxHeight)))//|| (count > maxArea)
				{

					if ((count < coin_Area))
					{
						//wyczysc
						for (int i = 0; i < count; i++) { tab[listTmp[i]] = 0; }	//{	mask0[listS[dsizemin1-i]]=0;}
					}
					superTest = false;

				}


			if (superTest == true) //sprawdz width i height - jesli zle to wyczysc jesli ok to zamaluj na 1
			{
				//if ((shape>0) && (shape<4))
				{
					int pixelArea = count;


					int nnx = maxx - minx + 1;
					int nny = maxy - miny + 1;

					int padd = 2;
					int nSizeX = nnx + padd * 2;
					int nSizeY = nny + padd * 2;
					int nSsize = nSizeX * nSizeY;

					int padd1 = 1;

					int nSizeX1 = nnx + padd1 * 2;
					int nSizeY1 = nny + padd1 * 2;

					if (nSizeX > maxX) maxX = nSizeX;
					if (nSizeY > maxY) maxY = nSizeY;

					//tworze maly zbiorek z danymi aktualnej wyspy powiekszonej o padd z kazdej strony
					unsigned char* tab1 = new unsigned char[nSsize];
					memset(tab1, 0, nSsize * sizeof(unsigned char));

					//zmniejszona tablica 
					int rr, gg, bb;
					rr = ((rand() * 255) / RAND_MAX); gg = ((rand() * 255) / RAND_MAX); bb = ((rand() * 255) / RAND_MAX);

					QColor c(rr, gg, bb);
					QColor c1(255 - rr, 255 - gg, 255 - bb);
					//QColor c(0, 255, 0);
					p.setPen(c1);
					unsigned int cI = (unsigned int)(c.rgba());



					//	int colorize = maskList->GetNumberOfItems() + 1;
					int fontx, fonty;
					for (int k = 0; k < count; k++)
					{

						y = listTmp[k] * nx1d;
						x = listTmp[k] - y * ww;

						if (k == 0)
						{
							fontx = x;
							fonty = y;
						}

						x += padd - minx;	y += padd - miny;
						tab1[x + y * nSizeX] = 1;
						tab[listTmp[k]] = 0;
						tabxCopy[listTmp[k]] = cI;
					}

					p.drawText(fontx, fonty, QString::number(blobCount));


					//------------------------------------------------












					//-----------------------------------------------------



					maskClass* mc = new maskClass();
					mc->data = tab1;
					mc->h = nSizeY;
					mc->w = nSizeX;
					mc->xPos = minx;
					mc->yPos = miny;
					mc->maxX = maxx;
					mc->maxY = maxy;
					mc->count = count;
					mc->origW = ww;
					mc->origH = hh;
					//LinkedList <maskClass*> *maskList = new LinkedList <maskClass*>;


					//LinkedList <maskClass*> *DivideBlobUsingAcuteAngleCriterion(maskClass *inputBlob)

					// - dla każdej wyspy przebiegnę po każdym blobie i wyszukam kąty ostre
					// - zrobię listę wierzchołków i połączę każdy z wierzchołków z innym najbliższym z listy
					//	QPolygonF lines;
					DivideBlobUsingAcuteAngleCriterion(mc, &lines, morphologyKernelType, morphologyType, listS, listTmp, blobCount, test);
					blobCount++;

					delete mc;
					//maskList->InsertItem(mc);
					/*	if (count > coin_Area)
					{
					double sc = GetMaskCoinScale(mc, coin_Diameter);
					xScale = sc;
					yScale = sc;
					params[11] = xScale;

					}*/

				}




			}
		}
	}


	params[9] = maxX;
	params[10] = maxY;


	//params[8] = nrOfObjects;

	delete[]kernelInc;
	delete[]kernel;
	return lines;

	delete[]tab;

}
//-------------------------------------------------
void GetSeparatedMasks(LinkedList <maskClass*>* maskList, unsigned short* tab, int ww, int hh, int inputVal, double* params, int* listS, int* listTmp, unsigned int* tabxCopy, bool fitEllipse, QImage* img, QImage* origImg)
{
	if (maskList == NULL) return;
	int kernelSize = 8;
	int size = 1;
	int* kernel = new int[kernelSize];
	int* kernelInc = new int[kernelSize];
	int pos = 0;

	for (int j = -size; j < size + 1; j++)
	{
		for (int i = -size; i < size + 1; i++)
		{
			if (!((i == 0) && (j == 0)))
			{
				kernel[pos] = i + j * ww;
				kernelInc[pos++] = j;
			}

		}
	}

	int ssize = ww * hh;

	int minArea = params[0];
	int maxArea = params[1];
	int minWidth = params[2];
	int maxWidth = params[3];
	int minHeight = params[4];
	int maxHeight = params[5];

	int coin_Area = params[6];
	double coin_Diameter = params[7];


	int startPos = 0;
	int ap = 0;
	int lp = 0;
	int count;
	int tmp;
	int dsizemin1 = ssize - 1;
	int nr0 = 0;
	count = 0;
	int currPos = 0;
	double nx1d = 1.0 / ww;
	ap = currPos;
	lp = 0;
	listS[0] = currPos;
	listTmp[(count++)] = currPos;
	//	tab[ap] = 255;
	int oldy;
	int x, y;




	int morphologyType = 1;// params[morphologyTypeID];
	int morphologyKernelType = 1;// params[morphologyKernelTypeID];



	int objectNr = 0;
	int* dilatePoints = listS;//new int[sliceSize];


	int jStart = 0;
	int jStop = ssize;

	QList <int> centerList;


	unsigned int* origColsTab = (origImg != NULL) ? ((unsigned int*)(origImg->bits())) : NULL;



	int maxX = 0;
	int maxY = 0;

	double xScale = 1.0;
	double  yScale = 1.0;

	double maxAreaCoin = 0;
	int coinDataPos = -1;;
	for (int j = jStart; j < jStop; j++)
	{
		double rr, gg, bb = 0;
		int ccc = centerList.count();

		if (ccc > 0)
		{
			currPos = centerList.at(j);
		}
		else
		{
			currPos = j;
		}

		if ((tab[currPos] > 0) && (tab[currPos] < 65535))
		{
			int minx = ww;
			int maxx = 0;
			int miny = hh;
			int maxy = 0;
			int x, y;
			nr0 = 0;
			count = 0;
			ap = currPos;
			lp = 0;
			listS[0] = currPos;
			listTmp[(count++)] = currPos;
			int oldx, oldy;
			int currFront = tab[ap];

			//zalewam
			do
			{
				ap = listS[lp--];

				oldy = ap * nx1d;

				for (int i = 0; i < kernelSize; i++)
				{
					tmp = ap + kernel[i];
					y = tmp * nx1d;
					x = tmp - y * ww;

					if (!((tmp > dsizemin1) || (tmp < 0) || (y != oldy + kernelInc[i]))) //brzydkie
					{
						if (tab[tmp] == currFront)
						{
							tab[tmp] = 65535;
							listS[++lp] = tmp;
							listTmp[count++] = tmp;



							if (x < minx) minx = x;
							if (x > maxx) maxx = x;
							if (y < miny) miny = y;
							if (y > maxy) maxy = y;


							if (origColsTab != NULL)
							{
								unsigned char* col = (unsigned char*)(origColsTab + tmp);//mam pozycje tego co trzeba - teraz potrzebuje rgb
								rr += col[0];
								gg += col[1];
								bb += col[2];
							}
						}
					}
				}
			} while (lp >= 0);

			int width = maxx - minx;
			int height = maxy - miny;

			bool superTest = true;
			if (count > coin_Area)
			{
				//policze i zmienie scale dla calego volumenu


			}
			else
				if (((count < minArea) || (count > maxArea) || (width < minWidth) || (width > maxWidth) || (height < minHeight) || (height > maxHeight)))
				{

					if ((count > coin_Diameter))

						//wyczysc
						for (int i = 0; i < count; i++) { tab[listTmp[i]] = 0; }	//{	mask0[listS[dsizemin1-i]]=0;}
					superTest = false;

				}


			if (superTest == true) //sprawdz width i height - jesli zle to wyczysc jesli ok to zamaluj na 1
			{
				//if ((shape>0) && (shape<4))
				{
					int pixelArea = count;


					int nnx = maxx - minx + 1;
					int nny = maxy - miny + 1;

					int padd = 2;
					int nSizeX = nnx + padd * 2;
					int nSizeY = nny + padd * 2;
					int nSsize = nSizeX * nSizeY;

					int padd1 = 1;

					int nSizeX1 = nnx + padd1 * 2;
					int nSizeY1 = nny + padd1 * 2;

					if (nSizeX > maxX) maxX = nSizeX;
					if (nSizeY > maxY) maxY = nSizeY;

					//tworze maly zbiorek z danymi aktualnej wyspy powiekszonej o padd z kazdej strony
					unsigned char* tab1 = new unsigned char[nSsize];
					memset(tab1, 0, nSsize * sizeof(unsigned char));

					//zmniejszona tablica 
					QColor c((rand() * 255) / RAND_MAX, (rand() * 255) / RAND_MAX, (rand() * 255) / RAND_MAX);
					unsigned int cI = (unsigned int)(c.rgba());

					if (maskList->GetNumberOfItems() == 20)
					{
						padd1 = padd1;
					}

					//	int colorize = maskList->GetNumberOfItems() + 1;
					for (int k = 0; k < count; k++)
					{

						y = listTmp[k] * nx1d;
						x = listTmp[k] - y * ww;

						x += padd - minx;	y += padd - miny;
						tab1[x + y * nSizeX] = 1;
						tab[listTmp[k]] = 0;
						tabxCopy[listTmp[k]] = cI;
					}





					//------------------------------------------------

					//zrobie dylacje!!@!!!
					simple2DMophology(tab1, nSizeX, nSizeY, 0, 0);;
					int countt = 0;
					for (int k = 0; k < nSizeX * nSizeY; k++)
					{
						if (tab1[k] == 1) countt++;
					}








					//-----------------------------------------------------



					maskClass* mc = new maskClass();
					mc->data = tab1;
					mc->h = nSizeY;
					mc->w = nSizeX;
					mc->xPos = minx;
					mc->yPos = miny;
					mc->maxX = maxx;
					mc->maxY = maxy;
					mc->count = countt;
					mc->origW = ww;
					mc->origH = hh;
					//LinkedList <maskClass*> *maskList = new LinkedList <maskClass*>;
					if (origColsTab != NULL)
					{
						rr /= count;
						gg /= count;
						bb /= count;

						QColor c(rr, gg, bb);
						mc->meanColor = c;

					}



					//LinkedList <maskClass*> *DivideBlobUsingAcuteAngleCriterion(maskClass *inputBlob)

					// - dla każdej wyspy przebiegnę po każdym blobie i wyszukam kąty ostre
					// - zrobię listę wierzchołków i połączę każdy z wierzchołków z innym najbliższym z listy
					//	QPolygonF lines;
					//	DivideBlobUsingAcuteAngleCriterion(mc, &lines, morphologyKernelType, morphologyType,listS,listTmp);

					if (fitEllipse == true)
					{
						//00000000000000000
						int prms[10];
						prms[0] = ww;
						prms[1] = hh;
						prms[2] = minx;
						prms[3] = maxx;
						prms[4] = miny;
						prms[5] = maxy;
						prms[6] = morphologyKernelType;
						prms[7] = morphologyType;;
						prms[8] = maskList->GetNumberOfItems();
						int nrOfDilPts = MaskListConturExtraction(listTmp, count, prms, listS);
						if (nrOfDilPts > 0)
						{
							double outParams[10];
							FittEllipsetoBoundaryPoints(nrOfDilPts, listS, prms, outParams, img);
							mc->angle = outParams[0];
							mc->d1 = outParams[1];
							mc->d2 = outParams[2];
							mc->nr = prms[8];
							mc->rotCenter[0] = outParams[3] - minx;
							mc->rotCenter[1] = outParams[4] - miny;

						}



						//nowy test na monete
						//zakladam najwiksze kółko na ekranie!!!

						if (count > maxAreaCoin)
						{
							double r = MAX((width / 2), (height / 2));
							double circleArea = r * r * M_PI;


							double pixToEll = count / circleArea;

							double tol = 0.1;

							if (fabs(pixToEll - 1.0) < tol)
							{
								maxAreaCoin = count;
								double sc = GetMaskCoinScale(mc, coin_Diameter);
								xScale = sc;
								yScale = sc;
								params[11] = xScale;

								//tylko po to zeby nie uwzglednial jego rozmiaru przy ostatecznym wolumenie
								params[9] = 0;
								params[10] = 0;
								coinDataPos = maskList->GetNumberOfItems();
							}

						}
					}

					maskList->InsertItem(mc);
					//if (count > coin_Area)
					//{
					//	double sc = GetMaskCoinScale(mc, coin_Diameter);
					//	xScale = sc;
					//	yScale = sc;
					//	params[11] = xScale;

					//	//tylko po to zeby nie uwzglednial jego rozmiaru przy ostatecznym wolumenie
					//	params[9] = 0;
					//	params[10] = 0;

					//}
					//else
					//{
					//	maskList->InsertItem(mc);
					//}

				}




			}
		}
	}




	//params[8] = nrOfObjects;

	delete[]kernelInc;
	delete[]kernel;

	if (coinDataPos >= 0)
	{
		maskClass* mc = maskList->GetItemAtPos(coinDataPos)->GetData();
		maskList->DeleteItem(coinDataPos);
	}

	int xm = 0;
	int ym = 0;
	for (int i = 0; i < maskList->GetNumberOfItems(); i++)
	{
		maskClass* mc = maskList->GetItemAtPos(i)->GetData();
		if (mc->w > xm)xm = mc->w;
		if (mc->h > ym)ym = mc->h;

	}
	params[9] = xm;
	params[10] = ym;


}


// --------------------------

int MaskListConturExtraction(int* listTmp, int totalCount, int* params, int* dilatePoints)
{

	if ((listTmp == NULL) || (dilatePoints == NULL) || (params == NULL) || (totalCount <= 0)) return 0;

	int nrOfDilatePoints = 0;
	int pixelArea = totalCount;
	//00000000000000000
	int ww = params[0];
	int hh = params[1];
	int minx = params[2];
	int maxx = params[3];
	int miny = params[4];
	int maxy = params[5];
	int morphologyKernelType = params[6];
	int morphologyType = params[7];

	//000000000000000000000000
	double nx1d = 1.0 / ww;;
	int nnx = maxx - minx + 1;
	int nny = maxy - miny + 1;

	int padd = 2;
	int nSizeX = nnx + padd * 2;
	int nSizeY = nny + padd * 2;
	int nSsize = nSizeX * nSizeY;

	int padd1 = 1;

	int nSizeX1 = nnx + padd1 * 2;
	int nSizeY1 = nny + padd1 * 2;

	//tworze maly zbiorek z danymi aktualnej wyspy powiekszonej o padd z kazdej strony
	unsigned char* tab = new unsigned char[nSsize];
	memset(tab, 0, nSsize * sizeof(unsigned char));

	int x, y;
	//zmniejszona tablica 
	for (int k = 0; k < totalCount; k++)
	{

		y = listTmp[k] * nx1d;
		x = listTmp[k] - y * ww;

		x += padd - minx;	y += padd - miny;
		tab[x + y * nSizeX] = 1;

	}

	//tablica wypelniona

	RawDataSet* noneData = new RawDataSet(nSizeX, nSizeY, 1, 1, 1, 1, 1, false);
	int kSize;
	int kerenelSize2 = 0;
	int size2 = 1;
	int kernelType = morphologyKernelType;
	int* kernel2 = GetGrowingKernel2D(kerenelSize2, size2, noneData, kernelType);
	int tmp;
	int dsizemin1 = noneData->GetVolumeSize() - 1;

	nrOfDilatePoints = 0;

	int startPos = nSizeX * padd1 + padd1;
	bool stop;
	double nnx1d = 1.0 / nSizeX;
	//contur extraction

	int frontVal = morphologyType;
	int backVal = !frontVal;

	for (int jj = 0; jj < nSizeY1; jj++)
	{

		for (int ii = 0; ii < nSizeX1; ii++)
		{
			if (tab[startPos] == frontVal)
			{
				stop = false;
				for (int kk = 0; kk < kerenelSize2; kk++)
				{

					tmp = startPos + kernel2[kk];
					if ((tmp > dsizemin1) || (tmp < 0)) { continue; }
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
					y = startPos * nnx1d;
					x = startPos - y * nSizeX;



					//tablica dilatepoints zawiera wspolrzedne konturu globalne
					dilatePoints[nrOfDilatePoints] = x + minx;

					dilatePoints[nrOfDilatePoints + 1] = y + miny;
					nrOfDilatePoints += 2;
				}
			}
			startPos++;
		}
		startPos += 2 * padd1;

	}

	nrOfDilatePoints /= 2;




	// sprawdzanie czy podobne do kszta�tu zajmujacego podobny obwod
	delete[]tab;
	delete[]kernel2;
	delete noneData;

	return nrOfDilatePoints;
}

//----------------------------------
void FittEllipsetoBoundaryPoints(int nrOfDilatePoints, int* dilatePoints, int* params, double* outParams, QImage* img)
{


	//00000000000000000
	int ww = params[0];
	int hh = params[1];
	int minx = params[2];
	int maxx = params[3];
	int miny = params[4];
	int maxy = params[5];
	int morphologyKernelType = params[6];
	int morphologyType = params[7];

	//000000000000000000000000
	double nx1d = 1.0 / ww;;
	int nnx = maxx - minx + 1;
	int nny = maxy - miny + 1;

	int padd = 2;
	int nSizeX = nnx + padd * 2;
	int nSizeY = nny + padd * 2;
	int nSsize = nSizeX * nSizeY;

	int padd1 = 1;

	int nSizeX1 = nnx + padd1 * 2;
	int nSizeY1 = nny + padd1 * 2;
	RawDataSet* noneData = new RawDataSet(nSizeX, nSizeY, 1, 1, 1, 1, 1, false);


	double minArea = ww * hh;;
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

	unsigned int* tabx = NULL;
	if (img != NULL) tabx = (unsigned int*)(img->bits());

	int x, y;
	for (int ii = 0; ii < nrOfDilatePoints; ii++)
	{
		x = dilatePoints[ii * 2];
		y = dilatePoints[ii * 2 + 1];
		center[0] += x;
		center[1] += y;

		tabx[x + y * ww] = Qt::black;
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
	for (int jj = 0; jj < 2; jj++)
	{
		//for(int ii=0;ii<360;ii+=Astep)
		for (int ii = startx; ii < stopx; ii += stepx)
		{
			minX = INT_MAX;		 maxX = -INT_MAX;	 minY = INT_MAX;		 maxY = -INT_MAX;
			//calculate area
			double alpha = ii * to_rad;			double ca = cos(alpha);		double sa = sin(alpha);

			for (int jj = 0; jj < nrOfDilatePoints; jj++)
			{
				double x = (double)dilatePoints[jj * 2] - center[0];		double y = (double)dilatePoints[jj * 2 + 1] - center[1];
				double xx = x * ca - y * sa + center[0];				double yy = x * sa + y * ca + center[1];

				if (xx < minX) minX = xx;						if (xx > maxX) maxX = xx;
				if (yy < minY) minY = yy;						if (yy > maxY) maxY = yy;
			}
			area = (maxX - minX) * (maxY - minY);
			if (area < minArea)
			{
				minArea = area;			area = minArea;				minPos = ii;
				rectX = (maxX - minX);		rectY = (maxY - minY);

				nCenter[0] = (minX + maxX) * 0.5;
				nCenter[1] = (minY + maxY) * 0.5;
			}
		}
		startx = minPos - Astep2; stopx = minPos + Astep2; stepx = spacing2;
	}


	//double ptsX[4], ptsY[4];





	double anglex = -minPos * to_rad;
	double ca = cos(anglex);		double sa = sin(anglex);
	double ptsX[4], ptsY[4];
	double nCenter2[2];

	double r1 = rectX / 2;
	double r2 = rectY / 2;

	nCenter2[0] = nCenter[0];
	nCenter2[1] = nCenter[1];

	ptsX[0] = nCenter2[0] - r1;		ptsY[0] = nCenter2[1] - r2;
	ptsX[1] = nCenter2[0] + r1;		ptsY[1] = nCenter2[1] - r2;
	ptsX[2] = nCenter2[0] + r1;		ptsY[2] = nCenter2[1] + r2;
	ptsX[3] = nCenter2[0] - r1;		ptsY[3] = nCenter2[1] + r2;

	QPolygonF poly;

	for (int ii = 0; ii < 4; ii++)
	{

		double xx = (ptsX[ii] - center[0]) * ca - (ptsY[ii] - center[1]) * sa + center[0];
		double yy = (ptsX[ii] - center[0]) * sa + (ptsY[ii] - center[1]) * ca + center[1];
		poly.append(QPointF(xx, yy));
	}
	if (img != NULL)
	{
		QPainter p(img);
		p.setPen(Qt::red);
		p.drawPolygon(poly);

		for (int ii = 0; ii < 4; ii++)
		{

			double xx = (ptsX[ii] - center[0]) * ca - (ptsY[ii] - center[1]) * sa + center[0];
			double yy = (ptsX[ii] - center[0]) * sa + (ptsY[ii] - center[1]) * ca + center[1];
			ptsX[ii] = xx;		ptsY[ii] = yy;
		}

		p.setPen(Qt::blue);
		//if (showFittedAxes)
		{
			{
				MyShape shape;
				shape.polyColor.setRgb(0, 128, 255);
				shape.representation = 2;
				shape.poly.append(QPointF(ptsX[0], ptsY[0]));
				shape.poly.append(QPointF(ptsX[1], ptsY[1]));
				//shapes->shapeList.append(shape);
				p.drawPolygon(shape.poly);

			}
			{
				MyShape shape;
				shape.polyColor.setRgb(0, 128, 255);
				shape.representation = 2;
				shape.poly.append(QPointF(ptsX[2], ptsY[2]));
				shape.poly.append(QPointF(ptsX[3], ptsY[3]));
				p.drawPolygon(shape.poly);
				//shapes->shapeList.append(shape);
			}


			QString text = QString::number(params[8]);
			p.setPen(Qt::blue);

			QFont font = p.font();
			font.setPointSize(5);
			p.setFont(font);
			p.drawText(-10 + ptsX[0], ptsY[0], 500, 500, Qt::AlignLeft, text);









		}







	}


	outParams[0] = anglex;
	outParams[1] = rectX;
	outParams[2] = rectY;
	outParams[3] = nCenter2[0];// center[0];
	outParams[4] = nCenter2[1];// center[1];

	delete noneData;





}
//-----------------------------------------------
void GetKernelIntesitiesToTab(unsigned short* tab, unsigned short* tab1, int& tmpSize, int* kernel, int kernelSize, int pos, int dsizemin1)
{
	tmpSize = 0;
	int tmpPos;
	for (int i = 0; i < kernelSize; i++)
	{
		tmpPos = pos + kernel[i];


		if (!((tmpPos > dsizemin1) || (tmpPos < 0)))
		{
			tab1[tmpSize++] = tab[tmpPos];
		}

	}
}
///
int GetKernelIntesitiesToTabForConvolution(unsigned short* tab, unsigned short* tab1, int* convSrc, int* convDst, int& tmpSize, int* kernel, int kernelSize, int pos, int dsizemin1)
{
	tmpSize = 0;
	int tmpPos;
	int weight = 0;
	for (int i = 0; i < kernelSize; i++)
	{
		tmpPos = pos + kernel[i];


		if (!((tmpPos > dsizemin1) || (tmpPos < 0)))
		{
			tab1[tmpSize] = tab[tmpPos];
			convDst[tmpSize] = convSrc[i];
			weight += convSrc[i];
			tmpSize++;
		}

	}
	return weight;
}


int Convolve(unsigned short* tab, int* filter, int size)
{
	int sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum += tab[i] * filter[i];
	}
	return sum;
}

unsigned short Minimum(unsigned short* tab, int n)
{
	unsigned short m = tab[0];
	for (int i = 1; i <= n; i++)
	{
		if (tab[i] < m)m = tab[i];
	}
	return m;
}
//-------------
unsigned short Maximum(unsigned short* tab, int n)
{
	unsigned short m = tab[0];
	for (int i = 1; i <= n; i++)
	{
		if (tab[i] > m)m = tab[i];
	}
	return m;
}

//----------------------------------


double Real48ToDouble(unsigned char realValue[6])
{

	if (realValue[0] == 0)
		return 0.0; // Null exponent = 0

	double exponent = realValue[0] - 129.0;
	double mantissa = 0.0;

	for (int byte = 1; byte <= 4; byte++) {
		mantissa += realValue[byte];
		mantissa *= 0.00390625; // mantissa /= 256
	}
	mantissa += (realValue[5] & 0x7F);
	mantissa *= 0.0078125; // mantissa /= 128
	mantissa += 1.0;

	if (realValue[5] & 0x80) // Sign bit check
		mantissa = -mantissa;
	return mantissa * pow(2.0, exponent);
}

//-------------------------------------------------
/*

*/
void DivideBlobUsingAcuteAngleCriterion(maskClass* inputBlob, QPolygonF* lines, int morphologyKernelType, int morphologyType, int* listS, int* listTmp, int blobCount, bool test)
{
	//bool test = true;

	int step = 10;// odległość punktów po lewej i poprawej od analizowanego zeby wyznaczyć kąt
	double angleThresh = 160; //próg akceptacji kąta
	int neighSize = 5; //  odległość +- tyle dla znalezionego minimum  - nie może być więcej ninimów w takim otoczeniu - biore najmniejsze
	double angleNeighThesh = 80; // maksymalny kąt jaki moze tworzyc wektor normalny dla danego min i połączone 2 minima
	double sizeV = 5; //dlugosc niebieskiej kreski norm


					  // - dla każdej wyspy przebiegnę po każdym blobie i wyszukam kąty ostre
					  // - zrobię listę wierzchołków i połączę każdy z wierzchołków z innym najbliższym z listy
					  //LinkedList <maskClass*> *maskList = new LinkedList <maskClass*>;


					  // jesli się nie podzieli to lista będzie pusta
					  //do lines będę dodawał linie podziału wyliczone z kątów



					  //1. zrobic boundary 
					  //2. przebiec po boundary i wykryc potencjalne kąty
					  //3. chyba powinna byc parzysta liczba
					  //4. wyznacz kreski od pktu do najblizszego drugiego

	int ssize = inputBlob->w * inputBlob->h;

	//int *listS = new int[ssize];
	//int *listTmp = new int[ssize];



	int prms[10];
	prms[0] = inputBlob->origW;
	prms[1] = inputBlob->origH;
	prms[2] = inputBlob->xPos;
	prms[3] = inputBlob->maxX;
	prms[4] = inputBlob->yPos;
	prms[5] = inputBlob->maxY;
	prms[6] = morphologyKernelType;
	prms[7] = morphologyType;;
	//prms[8] = maskList->GetNumberOfItems();
	int count = 0;

	QImage img0(inputBlob->w, inputBlob->h, QImage::Format_RGB32);
	unsigned int* t0 = (unsigned int*)(img0.bits());
	img0.fill(Qt::black);
	for (int i = 0; i < ssize; i++)
	{
		if (inputBlob->data[i] > 0)		 t0[i] = 0xffffffff;


	}
	img0.save("d:\\rice\\" + QString::number(blobCount) + "start.png");
	int nrOfDilPts = MaskListConturExtraction(listTmp, inputBlob->count, prms, listS);


	// w listS zebrane są punkty
	int x, y;
	double center[2] = { 0 };


	int pos0, posm1, posp1;
	double p0[2], pm1[2], pp1[2];
	double* angleTab = new double[nrOfDilPts];
	QPolygonF normalAnglePoints;


	//zapis punktow i kąta

	QString   nnn = "d:\\rice\\" + QString::number(blobCount) + "xyangle.txt";

	QFile file(nnn);
	bool fileExists = false;
	if (file.open(QIODevice::WriteOnly))
	{
		fileExists = true;
	}

	QTextStream stream(&file);




	QPolygonF poly = ConvertContourPointsToPolygon(inputBlob, 0, listS, nrOfDilPts, blobCount, test);

	int ww = inputBlob->w;
	int hh = inputBlob->h;
	QImage img2(inputBlob->w, inputBlob->h, QImage::Format_RGB32);
	img2.fill(Qt::black);
	unsigned int* tablinesImg = (unsigned int*)(img2.bits());
	QPainter Painter2a(&img2);
	Painter2a.setBrush(Qt::white);
	Painter2a.setPen(1);
	Painter2a.drawPolygon(poly);
	unsigned char* tabS = new unsigned char[ssize];
	memset(tabS, 0, ssize);

	for (int i = 0; i < ssize; i++)
	{
		if (tablinesImg[i] == 0xffffffff)
		{
			tabS[i] = 1;
		}

	}



	int nrr = poly.count();
	for (int ii = 0; ii < nrr; ii++)
	{

		//x i y to kolejne punkty obwodu bloba teraz mogę spróbować poszukać czy są jakieś acute angles
		pos0 = ii;
		posm1 = (nrr + ii - step) % nrr;
		posp1 = (ii + step) % nrr;



		//p0[0] = listS[pos0 * 2];		p0[1] = listS[pos0 * 2 + 1];
		//pm1[0] = listS[posm1 * 2];		pm1[1] = listS[posm1 * 2 + 1];
		//pp1[0] = listS[posp1 * 2];		pp1[1] = listS[posp1 * 2 + 1];

		p0[0] = poly.at(pos0).x();	p0[1] = poly.at(pos0).y();
		pm1[0] = poly.at(posm1).x();	pm1[1] = poly.at(posm1).y();
		pp1[0] = poly.at(posp1).x();	pp1[1] = poly.at(posp1).y();


		angleTab[ii] = GetAngleBetweenLines2DFull(p0, pm1, pp1) * to_deg;


		//obliczę teraz punkt normalny dla tego kąta


		double px[2];

		px[0] = 0.5 * (pm1[0] + pp1[0]);		px[1] = 0.5 * (pm1[1] + pp1[1]);

		double norm[2];
		norm[0] = p0[0] - px[0]; norm[1] = p0[1] - px[1];

		double len = norm[0] * norm[0] + norm[1] * norm[1];
		if (len > 0) len = sqrt(len);
		norm[0] /= len; norm[1] /= len;


		norm[0] *= sizeV;
		norm[1] *= sizeV;

		norm[0] += p0[0];
		norm[1] += p0[1];
		normalAnglePoints.append(QPointF(norm[0], norm[1]));

		if (fileExists == true)
		{
			stream << p0[0] << "," << p0[1] << "," << angleTab[ii] << "," << norm[0] << "," << norm[1] << endl;
		}

		//center[0] += x;
		//center[1] += y;

		//tabx[x + y*ww] = Qt::black;
	}



	if (fileExists == true) file.close();




	QPolygonF minPts;

	QPolygonF minPtsNorm;
	unsigned char* flags = new unsigned char[nrr];
	memset(flags, 0, nrr);
	for (int i = 0; i < nrr; i++)
	{
		if (flags[i] == 0)
		{
			if (angleTab[i] < angleThresh)//znalzl
			{
				double minAngle = angleTab[i];
				int minPos = i;
				for (int j = -neighSize; j <= neighSize; j++)
				{
					int nPos = (i + j + nrr) % nrr;
					if (angleTab[nPos] < minAngle)
					{
						minAngle = angleTab[nPos];
						minPos = nPos;
					}
					//flags[nPos] = 1;
				}
				//czyszcze flagi od znalezionego minimum o negih size w obie strony
				for (int j = -neighSize; j <= neighSize; j++)
				{
					int nPos = (minPos + j + nrr) % nrr;

					flags[nPos] = 1;
				}



				minPts.append(poly.at(minPos));

				minPtsNorm.append(normalAnglePoints.at(minPos));


			}
		}

	}

	delete[]flags;

	QImage img(inputBlob->w, inputBlob->h, QImage::Format_RGB32);

	img.fill(Qt::black);
	QPainter Painter2(&img);
	QPen pen;
	if (test == true)
	{
		Painter2.setBrush(Qt::white);
		Painter2.setPen(1);
		Painter2.drawPolygon(poly);
		Painter2.setBrush(Qt::red);


		QFont font = Painter2.font();
		font.setPointSize(3);
		Painter2.setFont(font);

		pen.setColor(Qt::magenta);
		pen.setWidth(1);
		Painter2.setPen(pen);
		pen.setColor(Qt::red);
		Painter2.setPen(pen);
		int eSize = 1;
		for (int i = 0; i < minPts.count(); i++)
		{
			//pen.setColor(Qt::yellow);
			Painter2.drawEllipse(minPts.at(i), eSize, eSize);

		}

		//pen.setColor(Qt::darkBlue);
		//	Painter2.setPen(pen);
		/*Painter2.setPen(Qt::darkBlue);
		for (int i = 0; i < minPts.count(); i++)
		{

		Painter2.drawText(minPts.at(i).x(), minPts.at(i).y(), 500, 500, Qt::AlignLeft, QString::number(i));
		}*/

		//----szukam minPts
		// przebiegne po wszystkich punktach dla danego punktu i sprawdze odleglosci - na poczatek wyznacze kreski do najblizszego
		// potem poprawie ze nie moze biec po białym tylko po zapalonych
	}
	int minPos;
	double minDist;
	double  p1[2], p2[2], p3[2];
	QPolygonF tempLines;
	flags = new unsigned char[minPts.count()];
	memset(flags, 0, minPts.count());
	for (int i = 0; i < minPts.count(); i++)
	{
		minDist = INT_MAX;
		minPos = i;
		p0[0] = minPts.at(i).x();
		p0[1] = minPts.at(i).y();

		p2[0] = minPtsNorm.at(i).x();
		p2[1] = minPtsNorm.at(i).y();

		bool anyGood = false;
		//if (flags[i] == 0)
		{
			for (int j = 0; j < minPts.count(); j++)
			{
				if ((i != j))//&& (flags[j] == 0))
				{
					p1[0] = minPts.at(j).x();
					p1[1] = minPts.at(j).y();
					double dist = GetDistance2D(p0, p1);

					//policzyc jeszcze kąt 




					double angle = GetAngleBetweenLines2DSigned(p0, p2, p1) * to_deg;

					p3[0] = minPtsNorm.at(j).x();
					p3[1] = minPtsNorm.at(j).y();
					//i ten po drugien stronie
					double angle1 = GetAngleBetweenLines2DSigned(p1, p3, p0) * to_deg;


					//sprawdze czy narysowanan kreska od pktu p1

					int pI0[2] = { p0[0],p0[1] };
					int pI1[2] = { p1[0],p1[1] };
					int nrOutside = CheckForImageLineBackgroundCollision(tabS, ww, hh, pI0, pI1, 1);
					if (nrOutside <= 5)
					{



						if ((dist < minDist) && (fabs(angle) < angleNeighThesh))//&&(fabs(angle1) < angleNeighThesh))
						{
							minDist = dist;
							minPos = j;
							anyGood = true;

						}
					}

				}
			}
		}
		if ((minPos != i) && (anyGood == true))
		{
			//dodaje kreske do lines;
			tempLines.append(minPts.at(i));
			tempLines.append(minPts.at(minPos));
			flags[i] = 1;
			flags[minPos] = 1;
		}
	}
	delete[]flags;
	///	Painter2.setBrush(Qt::green;)
	//QPen pen;

	if (test == true)
	{
		pen.setColor(Qt::blue);
		pen.setWidth(1);
		Painter2.setPen(pen);
		//narysuje na niepiesko normalne
		for (int i = 0; i < minPts.count(); i++)
		{

			Painter2.drawLine(minPts.at(i), minPtsNorm.at(i));
		}

		// i na zielono wygenerowane kreski
		pen.setColor(Qt::green);
		pen.setWidth(1);

		Painter2.setPen(pen);
		int pos = 0;
		for (int i = 0; i < tempLines.count() / 2; i++)
		{
			Painter2.drawLine(tempLines.at(pos).x(), tempLines.at(pos).y(), tempLines.at(pos + 1).x(), tempLines.at(pos + 1).y());
			pos += 2;
		}







		img.save("d:\\rice\\" + QString::number(blobCount) + "lines.png");

	}
	for (int i = 0; i < tempLines.count(); i++)
	{
		p0[0] = tempLines.at(i).x() + inputBlob->xPos;
		p0[1] = tempLines.at(i).y() + inputBlob->yPos;
		lines->append(QPointF(p0[0], p0[1]));
	}

	delete[]	tabS;
	delete[]angleTab;
	//	delete[]listS;
	//	delete[]listTmp;


	//tutaj będą kreski z liniami do podziału danych oryginalnych
}

//-------------------------------------------------

QPolygonF ConvertContourPointsToPolygon(maskClass* inputBlob, int kernelType, int* listS, int nrOfPoints, int blobCount, bool test)
{
	//bool test = true;
	int kernelSize = 1;
	int size;
	int nx = inputBlob->w;
	int ny = inputBlob->h;
	RawDataSet* rData = new RawDataSet(nx, ny, 1, 1, 1, 0, true);


	rData->ZeroQ();
	unsigned short* tab = rData->GetDataArray();;
	int pos = 0;
	int x, y;
	int xmin, ymin;
	xmin = inputBlob->xPos;
	ymin = inputBlob->yPos;
	for (int i = 0; i < nrOfPoints; i++)
	{
		x = listS[pos++] - xmin;
		y = listS[pos++] - ymin;
		tab[x + y * nx] = 1;
	}

	//int *kernel = GetGrowingKernel2D(size, kernelSize, rData, kernelType);
	int kernel[8] = { 1,nx,-1,-nx,-nx + 1,nx + 1,nx - 1,-nx - 1 };
	size = 8;

	//biore pierwszy punkt 1 z danych i zamalowuje na 2 
	//dodaje go do polygona
	//rozgladam sie po kernelu - dodaje kolejne az sie skonczy;

	int startPos = 0;

	int i;
	for (i = 0; i < rData->GetSliceSize(); i++)
	{
		if (tab[i] == 1) break;
	}

	if (test == true)
	{
		QImage img(nx, ny, QImage::Format_RGB32);
		img.fill(Qt::black);
		QPainter Painter2(&img);
		Painter2.setBrush(Qt::white);
		Painter2.setPen(1);
		//Painter2.drawPolygon(poly);



		unsigned int* tabxCopy = (unsigned int*)(img.bits());
		for (int i = 0; i < rData->GetSliceSize(); i++)
		{
			if (tab[i] == 1)
			{
				tabxCopy[i] = 0x00000000;


			}
			else
			{
				tabxCopy[i] = 0xffffffff;

			}
		}


		img.save("d:\\rice\\" + QString::number(blobCount) + "_origMask.png");
	}


	int p[3];
	QPolygonF poly;
	if (i < rData->GetSliceSize())
	{
		rData->GetVoxelXYZPos(i, p);
		poly.append(QPointF(p[0], p[1]));
		tab[i] = 2;
		//pierwszy dodalem teraz petla do

		startPos = i;
		int pos;
		bool anyfound = false;
		int counter = 1;
		do
		{
			anyfound = false;
			for (int i = 0; i < size; i++)
			{
				pos = startPos + kernel[i];
				if ((pos >= 0) && (pos < nx * ny))
					if (tab[pos] == 1)
					{
						anyfound = true;
						tab[pos] = 2;
						rData->GetVoxelXYZPos(pos, p);
						poly.append(QPointF(p[0], p[1]));
						startPos = pos;
						counter++;
						break;
					}
			}


		} while (anyfound == true);

		if (test == true)
		{
			QImage img(inputBlob->w, inputBlob->h, QImage::Format_RGB32);
			img.fill(Qt::white);
			QPainter Painter2(&img);
			Painter2.setBrush(Qt::white);
			Painter2.setPen(1);
			Painter2.drawPolyline(poly);



			/*unsigned int *tabxCopy = (unsigned int *)(img.bits());
			for (int i = 0; i < rData->GetSliceSize(); i++)
			{
			if (tab[i] == 2)
			{
			tabxCopy[i] = 0x00000000;


			}
			else
			{
			tabxCopy[i] = 0xffffffff;

			}
			}*/


			img.save("d:\\rice\\" + QString::number(blobCount) + "_polygon.png");
		}

		for (int i = 0; i < rData->GetSliceSize(); i++)
		{
			if (tab[i] == 2) tab[i] = 1;
		}


	}



	//	delete[]kernel;
	delete rData;
	return poly;
}


double evalSpline(double x, double* xs, double* ys, double* ks)
{
	int i = 1;
	while (xs[i] < x) i++;

	double t = (x - xs[i - 1]) / (xs[i] - xs[i - 1]);

	double a = ks[i - 1] * (xs[i] - xs[i - 1]) - (ys[i] - ys[i - 1]);
	double b = -ks[i] * (xs[i] - xs[i - 1]) + (ys[i] - ys[i - 1]);

	double q = (1 - t) * ys[i - 1] + t * ys[i] + t * (1 - t) * (a * (1 - t) + b * t);
	return q;
}
//-------------
void  gaussJswapRows(double** m, int k, int l) { double* p = m[k]; m[k] = m[l]; m[l] = p; }
//-------------
void gaussJsolve(double** A, double* x, int size)	// in Matrix, out solutions
{
	int m = size;// A.length;
	for (int k = 0; k < m; k++)	// column
	{
		// pivot for column
		int i_max = 0; int  vali = -INT_MAX;
		for (int i = k; i < m; i++) if (fabs(A[i][k]) > vali) { i_max = i; vali = fabs(A[i][k]); }
		gaussJswapRows(A, k, i_max);

		//if(A[k][k] == 0) console.log("matrix is singular!");

		// for all rows below pivot
		for (int i = k + 1; i < m; i++)
		{
			double cf = (A[i][k] / A[k][k]);
			for (int j = k; j < m + 1; j++)  A[i][j] -= A[k][j] * cf;
		}
	}

	for (int i = m - 1; i >= 0; i--)	// rows = columns
	{
		double v = A[i][m] / A[i][i];
		x[i] = v;
		for (int j = i - 1; j >= 0; j--)	// rows
		{
			A[j][m] -= A[j][i] * v;
			A[j][i] = 0;
		}
	}
}
//-------------
void getNaturalKs(double* xs, double* ys, double* ks, int size)    // in x values, in y values, out k values
{
	int  n = size;
	//var A = CSPL._gaussJ.zerosMat(n + 1, n + 2);
	double** A = new double* [n + 1];
	for (int i = 0; i < n + 1; i++)
	{
		A[i] = new double[n + 2];
		memset(A[i], 0, (n + 2) * sizeof(double));
	}



	for (int i = 1; i < n; i++)    // rows
	{
		A[i][i - 1] = 1 / (xs[i] - xs[i - 1]);

		A[i][i] = 2 * (1 / (xs[i] - xs[i - 1]) + 1 / (xs[i + 1] - xs[i]));

		A[i][i + 1] = 1 / (xs[i + 1] - xs[i]);

		A[i][n + 1] = 3 * ((ys[i] - ys[i - 1]) / ((xs[i] - xs[i - 1]) * (xs[i] - xs[i - 1]))
			+ (ys[i + 1] - ys[i]) / ((xs[i + 1] - xs[i]) * (xs[i + 1] - xs[i])));
	}

	A[0][0] = 2 / (xs[1] - xs[0]);
	A[0][1] = 1 / (xs[1] - xs[0]);
	A[0][n + 1] = 3 * (ys[1] - ys[0]) / ((xs[1] - xs[0]) * (xs[1] - xs[0]));

	A[n][n - 1] = 1 / (xs[n] - xs[n - 1]);
	A[n][n] = 2 / (xs[n] - xs[n - 1]);
	A[n][n + 1] = 3 * (ys[n] - ys[n - 1]) / ((xs[n] - xs[n - 1]) * (xs[n] - xs[n - 1]));

	gaussJsolve(A, ks, size);


	for (int i = 0; i < n + 1; i++)delete[]A[i];
	delete A;
}








//------------------------------------ CARDINAL SPLINE FROM VTK


CardinalSpline::CardinalSpline(double* dp, int _size, int _leftConstraint, int _rightConstraint)
{
	this->Intervals = NULL;
	this->Coefficients = NULL;
	this->LeftConstraint = _leftConstraint;
	this->LeftValue = 0.0;
	this->RightConstraint = _rightConstraint;
	this->RightValue = 0.0;


	this->ParametricRange[0] = -1;
	this->ParametricRange[1] = -1;
	dataPointer = dp;
	size = _size;
	Compute();

}

CardinalSpline::~CardinalSpline()
{
	delete[] this->Coefficients;
	delete[] this->Intervals;
}
double CardinalSpline::ComputeLeftDerivative()
{
	double* dptr = dataPointer;// this->PiecewiseFunction->GetDataPointer();
							   //int size = this->PiecewiseFunction->GetSize();
	if (dptr == NULL || size < 2)
	{
		return 0.0;
	}
	else
	{
		return (dptr[2] - dptr[0]);
	}

}
double CardinalSpline::ComputeRightDerivative()
{
	double* dptr = dataPointer;// this->PiecewiseFunction->GetDataPointer();
							   //	int size = this->PiecewiseFunction->GetSize();
	if (dptr == NULL || size < 2)
	{
		return 0.0;
	}
	else
	{
		return (dptr[(size - 1) * 2] - dptr[(size - 2) * 2]);
	}

}

int CardinalSpline::FindIndex(int size, double t)
{
	int index = 0;
	if (size > 2) //bisection method for speed
	{
		int rightIdx = size - 1;
		int centerIdx = rightIdx - size / 2;
		for (int converged = 0; !converged; )
		{
			if (this->Intervals[index] <= t && t <= this->Intervals[centerIdx])
			{
				rightIdx = centerIdx;
			}
			else //if ( this->Intervals[centerIdx] < t && t <= this->Intervals[rightIdx] )
			{
				index = centerIdx;
			}
			if ((index + 1) == rightIdx)
			{
				converged = 1;
			}
			else
			{
				centerIdx = index + (rightIdx - index) / 2;
			}
		}//while not converged
	}
	return index;
}

double CardinalSpline::Evaluate(double t)
{


	int index;
	double* intervals;
	double* coefficients;

	// check to see if we need to recompute the spline
	/*if (this->ComputeTime < this->GetMTime())
	{
	this->Compute();
	}*/

	// make sure we have at least 2 points
	//int size = this->PiecewiseFunction->GetSize();
	if (size < 2)
	{
		return 0.0;
	}

	intervals = this->Intervals;
	coefficients = this->Coefficients;

	/*if (this->Closed)
	{
	size = size + 1;
	}*/

	// clamp the function at both ends
	if (t < intervals[0])
	{
		t = intervals[0];
	}
	if (t > intervals[size - 1])
	{
		t = intervals[size - 1];
	}

	// find pointer to cubic spline coefficient using bisection method
	index = this->FindIndex(size, t);

	// calculate offset within interval
	t = (t - intervals[index]);

	// evaluate intervals value y
	return (t * (t * (t * *(coefficients + index * 4 + 3)
		+ *(coefficients + index * 4 + 2))
		+ *(coefficients + index * 4 + 1))
		+ *(coefficients + index * 4));
}

void CardinalSpline::Compute()
{
	double* ts, * xs;
	double* work;
	double* coefficients;
	double* dependent;
	//int size;
	int i;

	// get the size of the independent variables
	//size = this->PiecewiseFunction->GetSize();

	if (size < 2)
	{
		//vtkErrorMacro("Cannot compute a spline with less than 2 points. # of points is: " << size);
		return;
	}

	// copy the independent variables. Note that if the spline
	// is closed the first and last point are assumed repeated -
	// so we add and extra point
	//delete[] this->Intervals;

	//if (!this->Closed)
	//{
	this->Intervals = new double[size];
	ts = dataPointer;// this->PiecewiseFunction->GetDataPointer();
	for (i = 0; i < size; i++)
	{
		this->Intervals[i] = *(ts + 2 * i);
	}

	// allocate memory for work arrays
	work = new double[size];

	// allocate memory for coefficients
	//delete[] this->Coefficients;
	this->Coefficients = new double[4 * size];

	// allocate memory for dependent variables
	dependent = new double[size];

	// get start of coefficients for this dependent variable
	coefficients = this->Coefficients;

	// get the dependent variable values
	xs = this->dataPointer + 1;
	for (int j = 0; j < size; j++)
	{
		*(dependent + j) = *(xs + 2 * j);
	}

	this->Fit1D(size, this->Intervals, dependent,
		work, (double(*)[4])coefficients,
		this->LeftConstraint, this->LeftValue,
		this->RightConstraint, this->RightValue);
	//}



	// free the work array and dependent variable storage
	delete[] work;
	delete[] dependent;

	// update compute time
	//this->ComputeTime = this->GetMTime();

}
void CardinalSpline::Fit1D(int size, double* x, double* y,
	double* work, double coefficients[][4],
	int leftConstraint, double leftValue,
	int rightConstraint, double rightValue)
{
	double   b = 0.0;
	double   xlk;
	double   xlkp;
	int     k;

	// develop constraint at leftmost point.
	switch (leftConstraint)
	{
	case 0:
		// desired slope at leftmost point is derivative from two points
		coefficients[0][1] = 1.0;
		coefficients[0][2] = 0.0;
		work[0] = this->ComputeLeftDerivative();
		break;
	case 1:
		// desired slope at leftmost point is leftValue.
		coefficients[0][1] = 1.0;
		coefficients[0][2] = 0.0;
		work[0] = leftValue;
		break;
	case 2:
		// desired second derivative at leftmost point is leftValue.
		coefficients[0][1] = 2.0;
		coefficients[0][2] = 1.0;
		work[0] = 3.0 * ((y[1] - y[0]) / (x[1] - x[0])) -
			0.5 * (x[1] - x[0]) * leftValue;
		break;
	case 3:
		// desired second derivative at leftmost point is
		// leftValue times second derivative at first interior point.
		coefficients[0][1] = 2.0;
		coefficients[0][2] = 4.0 * ((0.5 + leftValue) /
			(2.0 + leftValue));
		work[0] = 6.0 * ((1.0 + leftValue) / (2.0 + leftValue)) *
			((y[1] - y[0]) / (x[1] - x[0]));
		break;
	default:
		assert("check: impossible case." && 0); // reaching this line is a bug.
		break;
	}

	// develop body of band matrix.
	for (k = 1; k < size - 1; k++)
	{
		xlk = x[k] - x[k - 1];
		xlkp = x[k + 1] - x[k];
		coefficients[k][0] = xlkp;
		coefficients[k][1] = 2.0 * (xlkp + xlk);
		coefficients[k][2] = xlk;
		work[k] = 3.0 * (((xlkp * (y[k] - y[k - 1])) / xlk) +
			((xlk * (y[k + 1] - y[k])) / xlkp));
	}


	// develop constraint at rightmost point.
	switch (rightConstraint)
	{
	case 0:
		// desired slope at leftmost point is derivative from two points
		coefficients[size - 1][0] = 0.0;
		coefficients[size - 1][1] = 1.0;
		work[size - 1] = this->ComputeRightDerivative();
		break;
	case 1:
		// desired slope at rightmost point is rightValue
		coefficients[size - 1][0] = 0.0;
		coefficients[size - 1][1] = 1.0;
		work[size - 1] = rightValue;
		break;
	case 2:
		// desired second derivative at rightmost point is rightValue.
		coefficients[size - 1][0] = 1.0;
		coefficients[size - 1][1] = 2.0;
		work[size - 1] = 3.0 * ((y[size - 1] - y[size - 2]) /
			(x[size - 1] - x[size - 2])) +
			0.5 * (x[size - 1] - x[size - 2]) * rightValue;
		break;
	case 3:
		// desired second derivative at rightmost point is
		// rightValue times second derivative at last interior point.
		coefficients[size - 1][0] = 4.0 * ((0.5 + rightValue) /
			(2.0 + rightValue));
		coefficients[size - 1][1] = 2.0;
		work[size - 1] = 6.0 * ((1.0 + rightValue) / (2.0 + rightValue)) *
			((y[size - 1] - y[size - 2]) /
			(x[size - 1] - x[size - 2]));
		break;
	default:
		assert("check: impossible case." && 0); // reaching this line is a bug.
		break;
	}

	// solve resulting set of equations.
	coefficients[0][2] = coefficients[0][2] / coefficients[0][1];
	work[0] = work[0] / coefficients[0][1];
	coefficients[size - 1][2] = 0.0;

	for (k = 1; k < size; k++)
	{
		coefficients[k][1] = coefficients[k][1] - (coefficients[k][0] *
			coefficients[k - 1][2]);
		coefficients[k][2] = coefficients[k][2] / coefficients[k][1];
		work[k] = (work[k] - (coefficients[k][0] * work[k - 1]))
			/ coefficients[k][1];
	}

	for (k = size - 2; k >= 0; k--)
	{
		work[k] = work[k] - (coefficients[k][2] * work[k + 1]);
	}

	// the column vector work now contains the first
	// derivative of the spline function at each joint.
	// compute the coefficients of the cubic between
	// each pair of joints.
	for (k = 0; k < size - 1; k++)
	{
		b = x[k + 1] - x[k];
		coefficients[k][0] = y[k];
		coefficients[k][1] = work[k];
		coefficients[k][2] = (3.0 * (y[k + 1] - y[k])) / (b * b) -
			(work[k + 1] + 2.0 * work[k]) / b;
		coefficients[k][3] = (2.0 * (y[k] - y[k + 1])) / (b * b * b) +
			(work[k + 1] + work[k]) / (b * b);
	}

	// the coefficients of a fictitious nth cubic
	// are evaluated.  This may simplify
	// algorithms which include both end points.

	coefficients[size - 1][0] = y[size - 1];
	coefficients[size - 1][1] = work[size - 1];
	coefficients[size - 1][2] = coefficients[size - 2][2] +
		3.0 * coefficients[size - 2][3] * b;
	coefficients[size - 1][3] = coefficients[size - 2][3];

}


//-------------

void GetGradient2DTab(double* output, unsigned short* input3D, double* kernel, unsigned short* tmpBuff, int radius, int view, int nx, int ny, int nz, int zPos)
{

	int i, j;
	int pos = 0;
	int size = 2 * radius + 1;
	for (j = 0; j < ny; j++)
		for (i = 0; i < nx; i++)
		{
			GetIntensityTab1D(input3D, nx, ny, nz, view, radius, tmpBuff, i, j, zPos);
			output[pos++] = Apply1DNormFilterF(kernel, tmpBuff, size);
		}
	//smoothing po x lub y dorobic

}
//--------------------------------------------------------------
void Get3DModulus(double* output, double* inX, double* inY, double* inZ, int size)
{
	int i;
	for (i = 0; i < size; i++)
		output[i] = sqrt(inX[i] * inX[i] + inY[i] * inY[i] + inZ[i] * inZ[i]);
}
//-------------------------------------------------
void Remove_Gradient_NonMaxima_Slice_2D(double* maxima, double* gx, double* gy, double* norme, int* bufferDims, double epsNorm, double epsDeriv)
{
	double _EPSILON_NORM_ = epsNorm;
	double _EPSILON_DERIVATIVE_ = epsDeriv;
	/*
	* the buffer norme[0] contains the gradient modulus of the
	* previous slice, the buffer norme[1] the ones of the
	* slice under study, while norme[2] containes the ones
	* of the next slice.
	*/
	/*
	* dimensions
	*/
	int dimx = bufferDims[0];
	int dimy = bufferDims[1];
	int dimxMinusOne = dimx - 1;
	int dimxPlusOne = dimx + 1;
	int dimyMinusOne = dimy - 1;
	/*
	* pointers
	*/
	double* fl_pt1 = (double*)NULL;
	double* fl_pt2 = (double*)NULL;
	double* fl_max = (double*)NULL;
	double* fl_nor = (double*)NULL;
	double* fl_upper_left = (double*)NULL;
	/*
	* coordinates and vector's components
	*/
	int x, y;
	double normalized_gx;
	double normalized_gy;
	double x_point_to_be_interpolated;
	double y_point_to_be_interpolated;
	int x_upper_left_corner;
	int y_upper_left_corner;
	/*
	* coefficients
	*/
	double dx, dy, dxdy;
	double c00, c01, c10, c11;
	/*
	* modulus
	*/
	double interpolated_norme;

	/*
	* we set the image border to zero.
	* First the borders along X direction,
	* second, the borders along the Y direction.
	*/
	fl_pt1 = maxima;
	fl_pt2 = maxima + (dimy - 1) * dimx;
	for (x = 0; x < dimx; x++, fl_pt1++, fl_pt2++)
		*fl_pt1 = *fl_pt2 = 0.0;
	fl_pt1 = maxima + dimx;
	fl_pt2 = maxima + dimx + dimx - 1;
	for (y = 1; y < dimy - 1; y++, fl_pt1 += dimx, fl_pt2 += dimx)
		*fl_pt1 = *fl_pt2 = 0.0;

	/*
	* We investigate the middle of the image.
	*/
	/*
	* Pointers are set to the first point
	* to be processed.
	*/
	fl_max = maxima + dimx + 1;
	fl_pt1 = gx + dimx + 1;
	fl_pt2 = gy + dimx + 1;
	fl_nor = norme + dimx + 1;
	for (y = 1; y < dimyMinusOne; y++, fl_max += 2, fl_pt1 += 2, fl_pt2 += 2, fl_nor += 2)
		for (x = 1; x < dimxMinusOne; x++, fl_max++, fl_pt1++, fl_pt2++, fl_nor++) {
			/*
			* If the modulus is too small, go to the next point.
			*/
			if (*fl_nor < _EPSILON_NORM_) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* We normalize the vector gradient.
			*/
			normalized_gx = *fl_pt1 / *fl_nor;
			normalized_gy = *fl_pt2 / *fl_nor;

			/*
			* May we use the nearest value?
			*/
			if ((-normalized_gx > _EPSILON_DERIVATIVE_) ||
				(normalized_gx > _EPSILON_DERIVATIVE_) ||
				(-normalized_gy > _EPSILON_DERIVATIVE_) ||
				(normalized_gy > _EPSILON_DERIVATIVE_)) {
				/*
				* First point to be interpolated.
				*/
				x_upper_left_corner = (int)((double)x + normalized_gx + 0.5);
				y_upper_left_corner = (int)((double)y + normalized_gy + 0.5);
				interpolated_norme = *(norme + (x_upper_left_corner + y_upper_left_corner * dimx));
				if (*fl_nor <= interpolated_norme) {
					*fl_max = 0.0;
					continue;
				}
				/*
				* Second point to be interpolated.
				*/
				x_upper_left_corner = (int)((double)x - normalized_gx + 0.5);
				y_upper_left_corner = (int)((double)y - normalized_gy + 0.5);
				interpolated_norme = *(norme + (x_upper_left_corner + y_upper_left_corner * dimx));
				if (*fl_nor < interpolated_norme) {
					*fl_max = 0.0;
					continue;
				}
				/*
				* We found a gradient extrema.
				*/
				*fl_max = *fl_nor;
				continue;
			}


			/*
			* From here we perform a bilinear interpolation
			*/

			/*
			* First point to be interpolated.
			* It is the current point + an unitary vector
			* in the direction of the gradient.
			* It must be inside the image.
			*/
			x_point_to_be_interpolated = (double)x + normalized_gx;
			y_point_to_be_interpolated = (double)y + normalized_gy;
			if ((x_point_to_be_interpolated < 0.0) ||
				(x_point_to_be_interpolated >= dimxMinusOne) ||
				(y_point_to_be_interpolated < 0.0) ||
				(y_point_to_be_interpolated >= dimyMinusOne)) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* Upper left corner,
			* coordinates of the point to be interpolated
			* with respect to this corner.
			*/
			x_upper_left_corner = (int)x_point_to_be_interpolated;
			y_upper_left_corner = (int)y_point_to_be_interpolated;
			dx = x_point_to_be_interpolated - (double)x_upper_left_corner;
			dy = y_point_to_be_interpolated - (double)y_upper_left_corner;
			dxdy = dx * dy;
			/*
			* bilinear interpolation of the gradient modulus
			* norme[x_point_to_be_interpolated, y_point_to_be_interpolated] =
			*   norme[0,0] * ( 1 - dx) * ( 1 - dy ) +
			*   norme[1,0] * ( dx ) * ( 1 - dy ) +
			*   norme[0,1] * ( 1 - dx ) * ( dy ) +
			*   norme[1,1] * ( dx ) * ( dy )
			*/
			c00 = 1.0 - dx - dy + dxdy;
			c10 = dx - dxdy;
			c01 = dy - dxdy;
			c11 = dxdy;
			fl_upper_left = norme + (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme = *(fl_upper_left)*c00 +
				*(fl_upper_left + 1) * c10 +
				*(fl_upper_left + dimx) * c01 +
				*(fl_upper_left + dimxPlusOne) * c11;
			/*
			* We compare the modulus of the point with the
			* interpolated modulus. It must be larger to be
			* still considered as a potential gradient extrema.
			*
			* Here, we consider that it is strictly superior.
			* The next comparison will be superior or equal.
			* This way, the extrema is in the light part of the
			* image.
			* By inverting both tests, we can put it in the
			* dark side of the image.
			*/
			if (*fl_nor <= interpolated_norme) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* Second point to be interpolated.
			* It is the current point - an unitary vector
			* in the direction of the gradient.
			* It must be inside the image.
			*/
			x_point_to_be_interpolated = (double)x - normalized_gx;
			y_point_to_be_interpolated = (double)y - normalized_gy;
			if ((x_point_to_be_interpolated < 0.0) ||
				(x_point_to_be_interpolated >= dimxMinusOne) ||
				(y_point_to_be_interpolated < 0.0) ||
				(y_point_to_be_interpolated >= dimyMinusOne)) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* Upper left corner.
			*/
			x_upper_left_corner = (int)x_point_to_be_interpolated;
			y_upper_left_corner = (int)y_point_to_be_interpolated;
			/* we do not recompute the coefficients
			dx = x_point_to_be_interpolated - (double)x_upper_left_corner;
			dy = y_point_to_be_interpolated - (double)y_upper_left_corner;
			dxdy = dx * dy;
			*/
			/*
			* We may use the previous coefficients.
			* norme[x_point_to_be_interpolated, y_point_to_be_interpolated] =
			*   norme[0,0] * c11 +
			*   norme[1,0] * c01 +
			*   norme[0,1] * c10 +
			*   norme[1,1] * c00
			*
			* WARNING: it works only if the cases where one derivative is close
			*          to -/+ 1 are already be independently processed, else
			*          it may lead to errors.
			*/
			/* we do not recompute the coefficients
			c00 = 1.0 - dx - dy + dxdy;
			c10 = dx - dxdy;
			c01 = dy - dxdy;
			c11 = dxdy;
			fl_upper_left = norme + (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme = *(fl_upper_left) * c00 +
			*(fl_upper_left + 1) * c10 +
			*(fl_upper_left + dimx) * c01 +
			*(fl_upper_left + dimxPlusOne) * c11;
			*/
			fl_upper_left = norme + (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme = *(fl_upper_left)*c11 +
				*(fl_upper_left + 1) * c01 +
				*(fl_upper_left + dimx) * c10 +
				*(fl_upper_left + dimxPlusOne) * c00;
			/*
			* Last test to decide whether or not we
			* have an extrema
			*/
			if (*fl_nor < interpolated_norme) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* We found a gradient extrema.
			*/
			*fl_max = *fl_nor;
		}
}
//-----------------------------------------------------------
void Remove_Gradient_NonMaxima_Slice_3D(double* maxima, double* gx, double* gy, double* gz, double** norme, int* bufferDims, double epsNorm, double epsDeriv)
{
	double _EPSILON_NORM_ = epsNorm;
	double _EPSILON_DERIVATIVE_ = epsDeriv;
	/*
	* the buffer norme[0] contains the gradient modulus of the
	* previous slice, the buffer norme[1] the ones of the
	* slice under study, while norme[2] containes the ones
	* of the next slice.
	*/
	/*
	* dimensions
	*/
	int dimx = bufferDims[0];
	int dimy = bufferDims[1];
	int dimxMinusOne = dimx - 1;
	int dimxPlusOne = dimx + 1;
	int dimyMinusOne = dimy - 1;
	/*
	* pointers
	*/
	double* fl_pt1 = (double*)NULL;
	double* fl_pt2 = (double*)NULL;
	double* fl_pt3 = (double*)NULL;
	double* fl_max = (double*)NULL;
	double* fl_nor = (double*)NULL;
	double* fl_upper_left = (double*)NULL;
	/*
	* coordinates and vector's components
	*/
	int x, y;
	int z;
	double normalized_gx;
	double normalized_gy;
	double normalized_gz;
	double x_point_to_be_interpolated;
	double y_point_to_be_interpolated;
	double z_point_to_be_interpolated;
	int x_upper_left_corner;
	int y_upper_left_corner;
	int z_upper_left_corner;
	/*
	* coefficients
	*/
	double dx, dy, dz;
	double dxdy, dxdz, dydz;
	double c000, c010, c100, c110;
	double c001, c011, c101, c111;
	/*
	* modulus
	*/
	double interpolated_norme;

	/*
	* we set the image border to zero.
	* First the borders along X direction,
	* second, the borders along the Y direction.
	*/
	fl_pt1 = maxima;
	fl_pt2 = maxima + (dimy - 1) * dimx;
	for (x = 0; x < dimx; x++, fl_pt1++, fl_pt2++)
		*fl_pt1 = *fl_pt2 = 0.0;
	fl_pt1 = maxima + dimx;
	fl_pt2 = maxima + dimx + dimx - 1;
	for (y = 1; y < dimy - 1; y++, fl_pt1 += dimx, fl_pt2 += dimx)
		*fl_pt1 = *fl_pt2 = 0.0;

	/*
	* We investigate the middle of the image.
	*/
	/*
	* Pointers are set to the first point
	* to be processed.
	*/
	fl_max = maxima + dimx + 1;
	fl_pt1 = gx + dimx + 1;
	fl_pt2 = gy + dimx + 1;
	fl_pt3 = gz + dimx + 1;
	fl_nor = norme[1] + dimx + 1;
	z = 1;
	for (y = 1; y < dimyMinusOne; y++, fl_max += 2, fl_pt1 += 2, fl_pt2 += 2, fl_pt3 += 2, fl_nor += 2)
		for (x = 1; x < dimxMinusOne; x++, fl_max++, fl_pt1++, fl_pt2++, fl_pt3++, fl_nor++) {

			/*
			* If the modulus is too small, go to the next point.
			*/
			if (*fl_nor < _EPSILON_NORM_) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* We normalize the vector gradient.
			*/
			normalized_gx = *fl_pt1 / *fl_nor;
			normalized_gy = *fl_pt2 / *fl_nor;
			normalized_gz = *fl_pt3 / *fl_nor;

			/*
			* May we use the nearest value?
			*/
			if ((-normalized_gx > _EPSILON_DERIVATIVE_) ||
				(normalized_gx > _EPSILON_DERIVATIVE_) ||
				(-normalized_gy > _EPSILON_DERIVATIVE_) ||
				(normalized_gy > _EPSILON_DERIVATIVE_) ||
				(-normalized_gz > _EPSILON_DERIVATIVE_) ||
				(normalized_gz > _EPSILON_DERIVATIVE_)) {
				/*
				* First point to be interpolated.
				*/
				x_upper_left_corner = (int)((double)x + normalized_gx + 0.5);
				y_upper_left_corner = (int)((double)y + normalized_gy + 0.5);
				z_upper_left_corner = (int)((double)z + normalized_gz + 0.5);
				interpolated_norme = *(norme[z_upper_left_corner]
					+ (x_upper_left_corner + y_upper_left_corner * dimx));
				if (*fl_nor <= interpolated_norme) {
					*fl_max = 0.0;
					continue;
				}
				/*
				* Second point to be interpolated.
				*/
				x_upper_left_corner = (int)((double)x - normalized_gx + 0.5);
				y_upper_left_corner = (int)((double)y - normalized_gy + 0.5);
				z_upper_left_corner = (int)((double)z - normalized_gz + 0.5);
				interpolated_norme = *(norme[z_upper_left_corner]
					+ (x_upper_left_corner + y_upper_left_corner * dimx));
				if (*fl_nor < interpolated_norme) {
					*fl_max = 0.0;
					continue;
				}
				/*
				* We found a gradient extrema.
				*/
				*fl_max = *fl_nor;
				continue;
			}


			/*
			* From here we perform a trilinear interpolation
			*/

			/*
			* First point to be interpolated.
			* It is the current point + an unitary vector
			* in the direction of the gradient.
			* It must be inside the image.
			*/
			x_point_to_be_interpolated = (double)x + normalized_gx;
			y_point_to_be_interpolated = (double)y + normalized_gy;
			z_point_to_be_interpolated = (double)z + normalized_gz;
			if ((x_point_to_be_interpolated < 0.0) ||
				(x_point_to_be_interpolated >= dimxMinusOne) ||
				(y_point_to_be_interpolated < 0.0) ||
				(y_point_to_be_interpolated >= dimyMinusOne)) {
				*fl_max = 0.0;
				continue;
			}

			/*
			* Upper left corner,
			* coordinates of the point to be interpolated
			* with respect to this corner.
			*/
			x_upper_left_corner = (int)x_point_to_be_interpolated;
			y_upper_left_corner = (int)y_point_to_be_interpolated;
			z_upper_left_corner = (int)z_point_to_be_interpolated;
			dx = x_point_to_be_interpolated - (double)x_upper_left_corner;
			dy = y_point_to_be_interpolated - (double)y_upper_left_corner;
			dz = z_point_to_be_interpolated - (double)z_upper_left_corner;
			/*
			* trilinear interpolation of the gradient modulus
			* norme[x_point_to_be_interpolated,
			*       y_point_to_be_interpolated,
			*       z_point_to_be_interpolated] =
			*   norme[0,0,0] * ( 1 - dx) * ( 1 - dy ) * ( 1 - dz ) +
			*   norme[1,0,0] * ( dx ) * ( 1 - dy ) * ( 1 - dz ) +
			*   norme[0,1,0] * ( 1 - dx ) * ( dy ) * ( 1 - dz ) +
			*   norme[1,1,0] * ( dx ) * ( dy ) * ( 1 - dz ) +
			*   norme[0,0,1] * ( 1 - dx) * ( 1 - dy ) * ( dz ) +
			*   norme[1,0,1] * ( dx ) * ( 1 - dy ) * ( dz ) +
			*   norme[0,1,1] * ( 1 - dx ) * ( dy ) * ( dz ) +
			*   norme[1,1,1] * ( dx ) * ( dy ) * ( dz )
			*/
			dxdy = dx * dy;
			dydz = dy * dz;
			dxdz = dx * dz;
			c111 = dxdy * dz;
			c011 = dydz - c111;
			c101 = dxdz - c111;
			c001 = dz - dxdz - c011;
			c110 = dxdy - c111;
			c010 = dy - dxdy - c011;
			c100 = dx - dxdy - c101;
			c000 = 1.0 - dx - dy + dxdy - c001;
			fl_upper_left = norme[z_upper_left_corner]
				+ (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme = *(fl_upper_left)*c000 +
				*(fl_upper_left + 1) * c100 +
				*(fl_upper_left + dimx) * c010 +
				*(fl_upper_left + dimxPlusOne) * c110;
			fl_upper_left = norme[z_upper_left_corner + 1]
				+ (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme += *(fl_upper_left)*c001 +
				*(fl_upper_left + 1) * c101 +
				*(fl_upper_left + dimx) * c011 +
				*(fl_upper_left + dimxPlusOne) * c111;
			/*
			* We compare the modulus of the point with the
			* interpolated modulus. It must be larger to be
			* still considered as a potential gradient extrema.
			*
			* Here, we consider that it is strictly superior.
			* The next comparison will be superior or equal.
			* This way, the extrema is in the light part of the
			* image.
			* By inverting both tests, we can put it in the
			* dark side of the image.
			*/
			if (*fl_nor <= interpolated_norme) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* Second point to be interpolated.
			* It is the current point - an unitary vector
			* in the direction of the gradient.
			* It must be inside the image.
			*/
			x_point_to_be_interpolated = (double)x - normalized_gx;
			y_point_to_be_interpolated = (double)y - normalized_gy;
			z_point_to_be_interpolated = (double)z - normalized_gz;
			if ((x_point_to_be_interpolated < 0.0) ||
				(x_point_to_be_interpolated >= dimxMinusOne) ||
				(y_point_to_be_interpolated < 0.0) ||
				(y_point_to_be_interpolated >= dimyMinusOne)) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* Upper left corner.
			*/
			x_upper_left_corner = (int)x_point_to_be_interpolated;
			y_upper_left_corner = (int)y_point_to_be_interpolated;
			z_upper_left_corner = (int)z_point_to_be_interpolated;
			/* we do not recompute the coefficients
			dx = x_point_to_be_interpolated - (double)x_upper_left_corner;
			dy = y_point_to_be_interpolated - (double)y_upper_left_corner;
			dz = z_point_to_be_interpolated - (double)z_upper_left_corner;
			*/
			/*
			* We use the previous coefficients.
			* norme[x_point_to_be_interpolated,
			*       y_point_to_be_interpolated,
			*       z_point_to_be_interpolated] =
			*   norme[0,0,0] * c111 +
			*   norme[1,0,0] * c011 +
			*   norme[0,1,0] * c101 +
			*   norme[1,1,0] * c001 +
			*   norme[0,0,1] * c110 +
			*   norme[1,0,1] * c010 +
			*   norme[0,1,1] * c100 +
			*   norme[1,1,1] * c000
			*

			fl_upper_left = norme[z_upper_left_corner]
			+ (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme = *(fl_upper_left) * c111 +
			*(fl_upper_left + 1) * c011 +
			*(fl_upper_left + dimx) * c101 +
			*(fl_upper_left + dimxPlusOne) * c001;
			fl_upper_left = norme[z_upper_left_corner+1]
			+ (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme += *(fl_upper_left) * c110 +
			*(fl_upper_left + 1) * c010 +
			*(fl_upper_left + dimx) * c100 +
			*(fl_upper_left + dimxPlusOne) * c000;

			*
			* WARNING: as in the 2D case it works only if the cases where one
			*          derivative is close to -/+ 1 are already be independently
			*          processed, else it may lead to errors.
			*/
			/* we do not recompute the coefficients
			dxdy = dx * dy;
			dydz = dy * dz;
			dxdz = dx * dz;
			c111 = dxdy * dz;
			c011 = dydz - c111;
			c101 = dxdz - c111;
			c001 = dz - dxdz - c011;
			c110 = dxdy - c111;
			c010 = dy - dxdy - c011;
			c100 = dx - dxdy - c101;
			c000 = 1.0 - dx - dy + dxdy - c001;
			fl_upper_left = norme[z_upper_left_corner]
			+ (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme = *(fl_upper_left) * c000 +
			*(fl_upper_left + 1) * c100 +
			*(fl_upper_left + dimx) * c010 +
			*(fl_upper_left + dimxPlusOne) * c110;
			fl_upper_left = norme[z_upper_left_corner+1]
			+ (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme += *(fl_upper_left) * c001 +
			*(fl_upper_left + 1) * c101 +
			*(fl_upper_left + dimx) * c011 +
			*(fl_upper_left + dimxPlusOne) * c111;
			*/

			fl_upper_left = norme[z_upper_left_corner]
				+ (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme = *(fl_upper_left)*c111 +
				*(fl_upper_left + 1) * c011 +
				*(fl_upper_left + dimx) * c101 +
				*(fl_upper_left + dimxPlusOne) * c001;
			fl_upper_left = norme[z_upper_left_corner + 1]
				+ (x_upper_left_corner + y_upper_left_corner * dimx);
			interpolated_norme += *(fl_upper_left)*c110 +
				*(fl_upper_left + 1) * c010 +
				*(fl_upper_left + dimx) * c100 +
				*(fl_upper_left + dimxPlusOne) * c000;

			/*
			* Last test to decide whether or not we
			* have an extrema
			*/
			if (*fl_nor < interpolated_norme) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* We found a gradient extrema.
			*/
			*fl_max = *fl_nor;
		}
}


//---------------------------------------------
int  CheckForImageLineBackgroundCollision(unsigned char* bits, int ww, int hh, int p0[2], int p1[2], unsigned char objColor)
{

	bool test = true;
	///unsigned int *bits = (unsigned int *)(img.bits());
	int ssize = ww * hh;// img.width()*img.height();
					  //int ww = img.width();
					  // narysuje kreske od p1 do p2 jesli trafie na inny kolor niz objColor to zwracam false

	int Ax = p0[0];
	int Ay = p0[1];
	int Bx = p1[0];
	int By = p1[1];

	int dX = abs(Bx - Ax);
	int dY = abs(By - Ay);

	int CurrentX = Ax;
	int CurrentY = Ay;

	int counter = 0;
	int Xincr, Yincr;
	if (Ax > Bx) { Xincr = -1; }
	else { Xincr = 1; }
	if (Ay > By) { Yincr = -1; }
	else { Yincr = 1; }

	int count = 0;
	if (dX >= dY)
	{
		int dPr = dY << 1;
		int dPru = dPr - (dX << 1);
		int P = dPr - dX;

		for (; dX >= 0; dX--)
		{

			//image[CurrentX+CurrentY*W]=Color;
			//DrawPoint(image, W, H, CurrentX, CurrentY, Color);
			int pos = CurrentX + CurrentY * ww;
			if ((pos > 0) && (pos < ssize))
			{
				if (bits[pos] != objColor)
				{
					//	test = false;
					count++;
				}
			}
			counter++;
			if (P > 0)
			{
				CurrentX += Xincr;
				CurrentY += Yincr;
				P += dPru;
			}
			else
			{
				CurrentX += Xincr;
				P += dPr;
			}
		}
	}
	else
	{
		int dPr = dX << 1;
		int dPru = dPr - (dY << 1);
		int P = dPr - dY;

		for (; dY >= 0; dY--)
		{

			//image[CurrentX+CurrentY*W]=Color;
			//DrawPoint(image, W, H, CurrentX, CurrentY, Color);
			int pos = CurrentX + CurrentY * ww;
			if ((pos > 0) && (pos < ssize))
			{
				if (bits[pos] != objColor)
				{
					//test = false;
					//break;
					count++;
				}
			}
			counter++;
			if (P > 0)
			{
				CurrentX += Xincr;
				CurrentY += Yincr;
				P += dPru;
			}
			else
			{
				CurrentY += Yincr;
				P += dPr;
			}
		}
	}


	return count;



}


//--------------------------

void drawText(QPainter& painter, qreal x, qreal y, Qt::Alignment flags,
	const QString& text, QRectF* boundingRect)
{
	const qreal size = 32767.0;
	QPointF corner(x, y - size);
	if (flags & Qt::AlignHCenter) corner.rx() -= size / 2.0;
	else if (flags & Qt::AlignRight) corner.rx() -= size;
	if (flags & Qt::AlignVCenter) corner.ry() += size / 2.0;
	else if (flags & Qt::AlignTop) corner.ry() += size;
	else flags |= Qt::AlignBottom;
	QRectF rect{ corner.x(), corner.y(), size, size };
	painter.drawText(rect, flags, text, boundingRect);
}
void drawText(QPainter& painter, const QPointF& point, Qt::Alignment flags,
	const QString& text, QRectF* boundingRect)
{
	drawText(painter, point.x(), point.y(), flags, text, boundingRect);
}

bool CheckForValueBetweenTwoVoxels(int pos1, int pos2, int dest, RawDataSet* data, double value, double tol, unsigned short* mask)
{

	int p1[3], p2[3];
	int dataSize = data->GetVolumeSize();
	if ((pos1 < 0) || (pos1 > dataSize - 1)) return true;
	if ((pos2 < 0) || (pos2 > dataSize - 1)) return true;

	int i, j;
	double point1[3];
	double point2[3];
	double delta[3];
	data->GetVoxelXYZPos(pos1, p1);
	data->GetVoxelXYZPos(pos2, p2);
	double scale[3];
	data->GetScale(scale);

	for (i = 0; i < 3; i++)
	{
		point1[i] = p1[i];
		point2[i] = p2[i];
		delta[i] = point2[i] - point1[i];
	}

	double point[3];
	//  double len=GetDistance(point1,point2);
	// if (len>0)
	for (i = 0; i < 3; i++) { delta[i] /= dest; }
	for (i = 0; i < 3; i++) { point[i] = point1[i] + delta[i]; }

	int n[3];
	data->GetN(n);

	double value1;
	for (j = 0; j < dest - 1; j++)
	{
		if (mask == NULL)
			value1 = Interpolation3LinearDoubleSafe(data, point[0], point[1], point[2]);
		else
			value = Interpolation3LinearUnsignedShort(mask, n[0], n[1], n[2], point[0], point[1], point[2]);
		//value1=data->GetVoxelValue((int)point[0],(int)point[1],(int)point[2]);

		if (fabs(value1 - value) < tol)
		{
			/// if (pos2==4455764)	QMessageBox::about( 0,"checlkWarning 1",QString::number(data->GetVoxelPos((int)point[0],(int)point[1],(int)point[2]))+" "+QString::number(value1)+"\t"+QString::number((int)point[0])+"\t"+QString::number((int)point[1])+"\t"+QString::number((int)point[2]));	
			return true;

		}
		for (i = 0; i < 3; i++) { point[i] += delta[i]; }
	}

	return false;


}

//---------------------------------------------------------
double* GetRealDistKernel(int& kernelSize, int size, RawDataSet* data, int*& tab2)
{

	double scale[3];
	data->GetScale(scale);
	//double scale2[3];
	scale[0] *= scale[0];
	scale[1] *= scale[1];
	scale[2] *= scale[2];
	int i, j, k;


	kernelSize = (size * 2 + 1) * (size * 2 + 1) * (size * 2 + 1) - 1;

	double* kernel = new double[kernelSize];
	tab2 = new int[kernelSize];
	memset(tab2, 0, kernelSize * sizeof(int));
	int pos = 0;

	for (k = -size; k < size + 1; k++)
	{

		for (j = -size; j < size + 1; j++)
		{
			for (i = -size; i < size + 1; i++)
			{
				if (!((i == 0) && (j == 0) && (k == 0)))
				{
					kernel[pos] = sqrt((double)(i * i * scale[0] + j * j * scale[1] + k * k * scale[2]));
					tab2[pos] = data->GetVoxelPos(i, j, k);
					pos++;
				}

			}
		}
	}

	//zebrane 2 tabele - odleglosci i przyrostow




	return kernel;
}
//---------------------------------
IntDoubleStruct** GetRadiusKernelTab(int& kernelSize, int maxRadius, RawDataSet* data)
{
	//fkcja na poczatek ma generowac tablice z przyrostami dla kolejnych wielkosci kerneli do max Radius;

	//int kernelSize;
	int* kernelInt;
	double* kernel = GetRealDistKernel(kernelSize, maxRadius, data, kernelInt);

	if (kernelSize > 0)

	{
		IntDoubleStruct** tempDouble = new IntDoubleStruct * [kernelSize];
		for (int i = 0; i < kernelSize; i++)
		{
			tempDouble[i] = new IntDoubleStruct;
		}

		for (int i = 0; i < kernelSize; i++)
		{
			tempDouble[i]->IntVar = kernelInt[i];
			tempDouble[i]->DoubleVar = kernel[i];
		}


		//if (counter>1)		
		QSortIntDouble(tempDouble, 0, kernelSize - 1);
		/*for (int i = 0; i < kernelSize; i++)
		{
			delete tempDouble[i];
		}
		delete[]tempDouble;

		int size = 0;*/
		delete[]kernelInt;
		delete[]kernel;
		return tempDouble;
	}
	else
		return NULL;

}
//funkcja poszukuje najbliszego punktu o danej odleglosci od zadanego w indeksie wolumetrycznycm
double  FindFirstValueFromTheCurrentPoint(int startPos, int val, IntDoubleStruct** distTabsKernel, int kernelSize, RawDataSet* data)
{
	//zwraca odleglosc pierwszego o wartosci val rozchodzac sie po okregach -spacer wolumetryczny
	// zwraca -1 jesli nic nie znalazl
	unsigned short* dataTab = data->GetDataArray();
	int dsizemin1 = data->GetVolumeSize() - 1;
	int tmpVoxelPos;
	double dist = -1;
	for (int x = 0; x < kernelSize; x++)
	{
		tmpVoxelPos = startPos + distTabsKernel[x]->IntVar;
		if ((tmpVoxelPos > dsizemin1) || (tmpVoxelPos < 0))
		{
			continue;
		}
		unsigned short cVal = dataTab[tmpVoxelPos];
		if (val == cVal)
		{
			dist = distTabsKernel[x]->DoubleVar;
			break;
		}


	}
	return dist;
}

//-------------------------------------------------------
void TabPeakDetector(double* tab, int size, bool useThreshold, double cutThreshold, bool detectMin, bool detectMax, bool checkEnds, int neighCheckSize, QVector <int>& maxPosVector)
{
	// cutThreshold = pow(cutThreshold, 3);
	// na skroty sprobuje z ABS

	double* tabSmooth = new double[size];
	double minValue = INT_MAX;
	double maxValue = -INT_MAX;


	for (int i = 0; i < size; i++)
	{
		//	tabSmooth[i] = fabs(tab[i]);
		if (tab[i] < minValue) minValue = tab[i];
		if (tab[i] > maxValue) maxValue = tab[i];
	}

	for (int i = 0; i < size; i++)
	{
		tabSmooth[i] = tab[i];// (tab[i] - minValue + maxValue * 2);
		//if (tab[i] < minValue) minValue = tab[i];
	///	if (tab[i] > maxValue) maxValue = tab[i];
	}

	if (size > 1)
	{
		tabSmooth[0] = tabSmooth[1];
	}

	if (size > 2)
	{
		tabSmooth[size - 1] = tabSmooth[size - 2];
	}

	double tmp2, tmpmax;



	double filterSize = 2.0;
	int ipj;
	int nr = Round((double)filterSize / 2.0) + 1;

	//moving average
	int tmpCounter;
	for (int k = 0; k < 1; k++)
		for (int i = 0; i < size; i++)
		{
			tmp2 = 0;
			tmpmax = 0;
			tmpCounter = 0;
			for (int j = -nr; j <= nr; j++)
			{
				ipj = i + j;
				if ((ipj >= 0) && (ipj < size))
				{
					tmpCounter++;
					tmp2 += tab[ipj];

				}
			}

			if (tmpCounter > 0)			tabSmooth[i] = tmp2 / (double)(tmpCounter);
		}



	//IntDoubleStruct	**tempdouble = new IntDoubleStruct*[size];

	double p;
	int d = 10;
	double maxdl, maxdr, delta;
	int pos;
	int peaks = 0;
	double tol = 1e-3;
	double invTol = 1.0 / tol;

	double maxVal = 0;
	double avg = 0;

	//	vector<float> v;
		//vector<int> maxV;
	int deltaSize = 3;

	QVector <QPoint> vector;
	vector.append(QPoint(0, tabSmooth[0] * invTol));
	int prevVal = tabSmooth[0] * invTol;
	for (int i = 1; i < size; i++)
	{
		int val = tabSmooth[i] * invTol;
		if (i == 121)
		{
			i = i;
		}
		if (abs(val - prevVal) > 5)
		{
			vector.append(QPoint(i, tabSmooth[i] * invTol));
			prevVal = val;
		}
	}
	//teoretycznie mam pozsuwana tablice - jesli jest zero to wypalam
	cutThreshold *= invTol;


	//na poczatku czy jest 
	if ((vector.count() > 3) && (checkEnds == true))
	{
		double val = vector.at(1).y();
		double prev = vector.at(0).y();
		double next = vector.at(2).y();
		if (((fabs(val) > cutThreshold) && (useThreshold == true)) || (useThreshold == false))
		{

			//ten sam kierunek tym razem!!!
			bool isMin = ((val > prev) && (val < next));
			bool isMax = ((val < prev) && (val > next));

			if ((detectMin) && (isMin))
			{

				int x = vector.at(1).x();
				maxPosVector.append(x);

			}
			if ((detectMax) && (isMax))
			{
				int x = vector.at(1).x();
				maxPosVector.append(x);
			}
		}

	}


	for (int i = 2; i < vector.count() - 2; i++)
	{
		double val = vector.at(i).y();
		double prev = vector.at(i - 1).y();
		double next = vector.at(i + 1).y();
		bool test;
		bool testValuesAvailable = false;;
		double prev2, next2;
		if (neighCheckSize > 0)
		{
			int prev2Pos = i - neighCheckSize;
			int prev2Next = i + neighCheckSize;
			if ((prev2Pos >= 0) && (prev2Next < vector.count() - 1))
			{
				prev2 = vector.at(prev2Pos).y();
				next2 = vector.at(prev2Next).y();
				testValuesAvailable = true;
			}
		}

		if (((fabs(val) > cutThreshold) && (useThreshold == true)) || (useThreshold == false))
		{


			bool isMax = ((val > prev) && (val > next));
			bool isMin = ((val < prev) && (val < next));

			if ((detectMin) && (isMin))
			{

				bool test = true;

				if (testValuesAvailable)
				{
					test = ((val < prev2) && (val < next2));
				}

				if (test == true)
				{
					int x = vector.at(i).x();
					maxPosVector.append(x);
				}
			}
			if ((detectMax) && (isMax))
			{
				bool test = true;

				if (testValuesAvailable)
				{
					test = ((val > prev2) && (val > next2));
				}

				if (test == true)
				{
					int x = vector.at(i).x();
					maxPosVector.append(x);
				}
			}
		}

	}
	int ss = vector.count();
	//na koncu czy jest 
	if ((vector.count() > 3) && (checkEnds == true))
	{
		double val = vector.at(ss - 2).y();
		double prev = vector.at(ss - 3).y();
		double next = vector.at(ss - 1).y();
		if (((fabs(val) > cutThreshold) && (useThreshold == true)) || (useThreshold == false))
		{

			//ten sam kierunek tym razem!!!
			bool isMin = ((val > prev) && (val < next));
			bool isMax = ((val < prev) && (val > next));

			if ((detectMin) && (isMin))
			{
				int x = vector.at(ss - 2).x();
				maxPosVector.append(x);
			}
			if ((detectMax) && (isMax))
			{
				int x = vector.at(ss - 2).x();
				maxPosVector.append(x);
			}
		}

	}
	/*for (int i = 0; i < size; i++)
	{
		v.push_back(tabSmooth[i]);
	}



	findPeaks(v, maxV);

	for (int i = 0; i < maxV.size(); i++)
	{
		double val = tabSmooth[maxV.at(i)];
		if ((fabs(val) > cut)||(nocut==true))  maxPosVector.append(maxV.at(i));
	}*/


	//robie wektor 2d i bede usuwal te ktore sa prawie takie same





	delete[]tabSmooth;

	//int *tabVals = new int[size];
	//QVector <QPoint> vector;
	//int lastVal = -100000;
	//for (int i = 0; i < size; i++)
	//{
	//	

	//	if (fabs(tabAbs[i]) > cut)
	//	{
	//		double prevVal = (i > 0) ? tabAbs[i-1] : -1;
	//		double currentVal = (i < pos) ? tabAbs [i]: -1;
	//		double nextVal = (i < pos - 1) ? tabAbs[i+1] : -1;


	//		int cVal, pVal, nVal;
	//		cVal = currentVal * 10000;
	//		pVal = prevVal * 10000;
	//		nVal = nextVal * 10000;

	//		if (abs(cVal - pVal) < 10) pVal = cVal;
	//		if (abs(cVal - nVal) < 10) nVal = cVal;

	//		double tol = 1e-2;

	//		int val = 0;
	//		if ((pVal>cVal) && (cVal<nVal)) //dolek
	//		{
	//			val = 10; 
	//		}
	//		else if ((pVal<cVal ) && (cVal>nVal))//gorka
	//		{
	//				val = 20;
	//		}
	//		else if ((pVal>cVal) && (cVal>nVal)) //spadek
	//		{
	//			val = -1; if (lastVal == val) val = 0; // ten warunek bo nie chce gromadzic kolejnych wzrostow
	//		}		
	//		else if ((pVal<cVal) && (cVal<nVal)) //wzrost
	//		{
	//			val = 1; if (lastVal == val) val = 0;
	//		}

	//		
	//		if (val != 0)
	//		{
	//			vector.append(QPoint(i, val));
	//			lastVal = val;
	//		}
	//	}
	//}


	//for (int i = 0; i < vector.count(); i++)
	//{
	//		int prevVal = (i > 0) ? vector.at(i - 1).y() : -1000;
	//		int currentVal = (i < vector.count()) ? vector.at(i).y() : -1000;
	//		int nextVal = (i < vector.count() - 1) ? vector.at(i + 1).y() : -1000;

	//		if ((currentVal == 10) || (currentVal==20))
	//		{
	//			maxPosVector.append(vector.at(i).x());
	//		}
	//		else	if (((prevVal == -1) && (nextVal == 1))|| ((prevVal == 1) && (nextVal == -1)))
	//		{
	//			int newX = (vector.at(i - 1).x() + vector.at(i + 1).x())*0.5;
	//			maxPosVector.append(newX);
	//		}

	//		 
	//}


	//delete[]tabAbs;
	//	tempdouble[i] = new IntDoubleStruct;
	//	tempdouble[i]->IntVar = i;
			//maxdl = maxdr = 0;
			//int left, right;
			//int sign;
			//if (i > 1)
			//{
			//	delta = tabAbs[i - 1] - tabAbs[i];
			//	sign = 0; if (delta > tol) sign = 1; else if (delta < -tol) sign = -1;
			//	left = sign;
			//	for (int j = 2; j < d; j++)
			//	{
			//		pos = i - j;
			//		if (pos >= 0)
			//		{
			//			delta = tabAbs[pos] - tabAbs[i];
			//			sign = 0; if (delta > 0) sign = 1; else sign = -1;

			//			if (!((sign == left) || (sign == 0) || ((sign == 1) && (left == 0)) || ((sign == -1) && (left == 0))))
			//				//if  ((sign!=left)
			//				//	&&(!(sign*left==0)))
			//			{
			//				maxdl = 0;
			//				left = -2;
			//				break;
			//			}
			//			if (fabs(delta) > maxdl) maxdl = fabs(delta);
			//		}
			//	}
			//}
			//if (i < size - 1)
			//{
			//	delta = tabAbs[i + 1] - tabAbs[i];
			//	sign = 0; if (delta > tol) sign = 1; else if (delta < -tol) sign = -1;
			//	right = sign;
			//	for (int j = 2; j < d; j++)
			//	{
			//		pos = i + j;
			//		if (pos < size)
			//		{

			//			delta = tabAbs[pos] - tabAbs[i];
			//			//sign =0; if (delta>0) sign=1;else sign=-1;

			//			//if ((sign!=right)
			//			//	&&(!(sign*right==0)))
			//			if (!((sign == right) || (sign == 0) || ((sign == 1) && (right == 0)) || ((sign == -1) && (right == 0))))
			//			{
			//				maxdr = 0;
			//				right = 2;
			//				break;
			//			}
			//			if (fabs(delta) > maxdr) maxdr = fabs(delta);
			//		}
			//	}
			//}
			////sprawdzam z prawej czy jest skok
			//double val = -1;
			//if ((left == -1) && (right == -1))	val = 8;
			//if ((left == -1) && (right == 0))	val = 7;
			//if ((left == -1) && (right == 1))	val = 6;
			//if ((left == 0) && (right == -1))	val = 5;
			//if ((left == 0) && (right == 0))	val = 4;
			//if ((left == 0) && (right == 1))	val = 3;
			//if ((left == 1) && (right == -1))	val = 2;
			//if ((left == 1) && (right == 0))	val = 1;
			//if ((left == 1) && (right == 1))	val = 0;


			//tabVals[i] = val;

//	}

	//if t

	//int startSpadek = -1;
	//int endSpadek = -1;
	//int startWzrost = -1;
	//int endWzrost = -1;
	//int pos = 0;


	////int realStart = -1;
	////int realStop = -1;

	//QVector <QPoint> vector;
	//for (int i = 0; i < size; i++)
	//{
	//	if (tabVals[i] == -1)continue;
	//	else
	//	{
	//		//tabVals[pos++] = tabVals[i];
	//		vector.append(QPoint(i, tabVals[i]));
	//		//if (realStart == -1) realStart = i;
	//		//realStop = i;
	//	}
	//}


	////if (((realStart = 0)&&
	//for (int i = 0; i < pos; i++)
	//{
	//	//maxPosVector.append(i);
	//	int prevVal = (i > 0) ? vector.at(i - 1).y() : -1;
	//	int currentVal = (i < pos) ? vector.at(i).y() : -1;
	//	int nextVal = (i < pos - 1) ? vector.at(i + 1).y() : -1;

	//	int test = false;
	//	if ((currentVal == 8) ||( currentVal == 0)) //pewny pik
	//	{
	//		test = true;


	//	}

	//}




		//if ((tabVals[i] == 8) || (tabVals[i] == 0)) //pewny peak
		//{

		//	startSpadek = -1;
		//	endSpadek = -1;
		//	startWzrost = -1;
		//	endWzrost = -1;
		//	maxPosVector.append(i);
		//}
		//else
		//{
		//	if 
		//}

	//}


	//for (int i = 0; i < size; i++)
	//{
	//	delete tempdouble[i];
	//}
	//delete[]tempdouble;


}


void diff(vector<float> in, vector<float>& out)
{
	out = vector<float>(in.size() - 1);

	for (int i = 1; i < in.size(); ++i)
		out[i - 1] = in[i] - in[i - 1];
}

void vectorProduct(vector<float> a, vector<float> b, vector<float>& out)
{
	out = vector<float>(a.size());

	for (int i = 0; i < a.size(); ++i)
		out[i] = a[i] * b[i];
}

void findIndicesLessThan(vector<float> in, float threshold, vector<int>& indices)
{
	for (int i = 0; i < in.size(); ++i)
		if (in[i] < threshold)
			indices.push_back(i + 1);
}

void selectElements(vector<float> in, vector<int> indices, vector<float>& out)
{
	for (int i = 0; i < indices.size(); ++i)
		out.push_back(in[indices[i]]);
}

void selectElements(vector<int> in, vector<int> indices, vector<int>& out)
{
	for (int i = 0; i < indices.size(); ++i)
		out.push_back(in[indices[i]]);
}

void signVector(vector<float> in, vector<int>& out)
{
	out = vector<int>(in.size());

	for (int i = 0; i < in.size(); ++i)
	{
		if (in[i] > 0)
			out[i] = 1;
		else if (in[i] < 0)
			out[i] = -1;
		else
			out[i] = 0;
	}
}


void findPeaks(vector<float> x0, vector<int>& peakInds)
{
	int minIdx = distance(x0.begin(), min_element(x0.begin(), x0.end()));
	int maxIdx = distance(x0.begin(), max_element(x0.begin(), x0.end()));

	float sel = (x0[maxIdx] - x0[minIdx]) / 4.0;

	int len0 = x0.size();

	vector<float> dx;
	diff(x0, dx);
	replace(dx.begin(), dx.end(), 0.0, -EPS);
	vector<float> dx0(dx.begin(), dx.end() - 1);
	vector<float> dx1(dx.begin() + 1, dx.end());
	vector<float> dx2;

	vectorProduct(dx0, dx1, dx2);

	vector<int> ind;
	findIndicesLessThan(dx2, 0, ind); // Find where the derivative changes sign

	vector<float> x;

	vector<int> indAux(ind.begin(), ind.end());
	selectElements(x0, indAux, x);
	x.insert(x.begin(), x0[0]);
	x.insert(x.end(), x0[x0.size() - 1]);;


	ind.insert(ind.begin(), 0);
	ind.insert(ind.end(), len0);

	int minMagIdx = distance(x.begin(), min_element(x.begin(), x.end()));
	float minMag = x[minMagIdx];
	float leftMin = minMag;
	int len = x.size();

	if (len > 2)
	{
		float tempMag = minMag;
		bool foundPeak = false;
		int ii;

		// Deal with first point a little differently since tacked it on
		// Calculate the sign of the derivative since we tacked the first
		//  point on it does not neccessarily alternate like the rest.
		vector<float> xSub0(x.begin(), x.begin() + 3);//tener cuidado subvector
		vector<float> xDiff;//tener cuidado subvector
		diff(xSub0, xDiff);

		vector<int> signDx;
		signVector(xDiff, signDx);

		if (signDx[0] <= 0) // The first point is larger or equal to the second
		{
			if (signDx[0] == signDx[1]) // Want alternating signs
			{
				x.erase(x.begin() + 1);
				ind.erase(ind.begin() + 1);
				len = len - 1;
			}
		}
		else // First point is smaller than the second
		{
			if (signDx[0] == signDx[1]) // Want alternating signs
			{
				x.erase(x.begin());
				ind.erase(ind.begin());
				len = len - 1;
			}
		}

		if (x[0] >= x[1])
			ii = 0;
		else
			ii = 1;

		float maxPeaks = ceil((float)len / 2.0);
		vector<int> peakLoc(maxPeaks, 0);
		vector<float> peakMag(maxPeaks, 0.0);
		int cInd = 1;
		int tempLoc;

		while (ii < len)
		{
			ii = ii + 1;//This is a peak
						//Reset peak finding if we had a peak and the next peak is bigger
						//than the last or the left min was small enough to reset.
			if (foundPeak)
			{
				tempMag = minMag;
				foundPeak = false;
			}

			//Found new peak that was lager than temp mag and selectivity larger
			//than the minimum to its left.

			if (x[ii - 1] > tempMag&& x[ii - 1] > leftMin + sel)
			{
				tempLoc = ii - 1;
				tempMag = x[ii - 1];
			}

			//Make sure we don't iterate past the length of our vector
			if (ii == len)
				break; //We assign the last point differently out of the loop

			ii = ii + 1; // Move onto the valley

						 //Come down at least sel from peak
			if (!foundPeak && tempMag > sel + x[ii - 1])
			{
				foundPeak = true; //We have found a peak
				leftMin = x[ii - 1];
				peakLoc[cInd - 1] = tempLoc; // Add peak to index
				peakMag[cInd - 1] = tempMag;
				cInd = cInd + 1;
			}
			else if (x[ii - 1] < leftMin) // New left minima
				leftMin = x[ii - 1];

		}

		// Check end point
		if (x[x.size() - 1] > tempMag&& x[x.size() - 1] > leftMin + sel)
		{
			peakLoc[cInd - 1] = len - 1;
			peakMag[cInd - 1] = x[x.size() - 1];
			cInd = cInd + 1;
		}
		else if (!foundPeak && tempMag > minMag)// Check if we still need to add the last point
		{
			peakLoc[cInd - 1] = tempLoc;
			peakMag[cInd - 1] = tempMag;
			cInd = cInd + 1;
		}

		//Create output
		if (cInd > 0)
		{
			vector<int> peakLocTmp(peakLoc.begin(), peakLoc.begin() + cInd - 1);
			selectElements(ind, peakLocTmp, peakInds);
			//peakMags = vector<float>(peakLoc.begin(), peakLoc.begin()+cInd-1);
		}



	}


}

//--------------------------------------------
void AngularPeakDetecotr(double* tab, int size, int neighSize, int step, double angleThresh, double angleNeighThresh, int aa, QVector <int>& maxPosVector)
{



	int pos0, posm1, posp1;
	double p0[2], pm1[2], pp1[2];
	double* angleTab = new double[size];
	double* angleTab2 = new double[size];
	int nrr = size;
	for (int ii = 0; ii < nrr; ii++)
	{

		//x i y to kolejne punkty obwodu bloba teraz mogę spróbować poszukać czy są jakieś acute angles
		pos0 = ii;
		posm1 = (ii - step) >= 0 ? (ii - step) : 0;
		posp1 = (ii + step) < nrr ? (ii + step) : nrr - 10;



		//p0[0] = listS[pos0 * 2];		p0[1] = listS[pos0 * 2 + 1];
		//pm1[0] = listS[posm1 * 2];		pm1[1] = listS[posm1 * 2 + 1];
		//pp1[0] = listS[posp1 * 2];		pp1[1] = listS[posp1 * 2 + 1];

		p0[0] = pos0;	p0[1] = tab[pos0];
		pm1[0] = posm1;	pm1[1] = tab[posm1];
		pp1[0] = posp1;	pp1[1] = tab[posp1];

		if (ii == 200)
		{
			ii = ii;
		}
		angleTab[ii] = fabs(GetAngleBetweenLines2DFull(p0, pm1, pp1) * to_deg - 180);


	}


	double v1, v2, v0;

	angleTab2[0] = 0;
	angleTab2[nrr - 1] = 0;
	for (int i = 1; i < nrr - 1; i++)
	{
		v0 = angleTab[i];
		v1 = angleTab[i - 1];
		v2 = angleTab[i + 1];
		bool test1 = v0 - v1 > 0;
		bool test2 = v0 - v2 > 0;
		if ((test1 == true) && (test2 == true))
		{
			angleTab2[i] = angleTab[i];
		}
		else
		{
			angleTab2[i] = 0;
		}
	}





	unsigned char* flags = new unsigned char[nrr];
	memset(flags, 0, nrr);
	for (int i = 1; i < nrr - 1; i++)
	{
		if (flags[i] == 0)
		{
			if (angleTab2[i] > 0)//znalzl
			{
				double maxAngle = angleTab2[i];
				int minPos = i;
				for (int j = -neighSize; j <= neighSize; j++)
				{
					int nPos = (i + j);
					if (nPos < 0) nPos = 0;
					if (nPos > nrr - 1) nPos = nrr - 1;
					if (angleTab2[nPos] > maxAngle)
					{
						maxAngle = angleTab2[nPos];
						minPos = nPos;
					}
					//flags[nPos] = 1;
				}
				//czyszcze flagi od znalezionego minimum o negih size w obie strony
				for (int j = -neighSize; j <= neighSize; j++)
				{
					int nPos = (minPos + j);
					if ((nPos >= 0) && (nPos < nrr))

						flags[nPos] = 1;
				}



				maxPosVector.append(minPos);


			}
		}

	}

	delete[]flags;
	delete[]angleTab;
	delete[]angleTab2;
}


//------------------------------

void FillUShortData(RawDataSet* data, unsigned short* volumeTempData, int startPos, int val, int* testKernel, int testKernelSize, int listSSize, int*& listS, bool totalMode, int* bounds, unsigned char* blockMask, int blockVal)
{

	int ssize = data->GetVolumeSize();

	//int startPos = 0;
	int ap = 0;
	int lp = 0;
	int count;
	int tmp;
	int dsizemin1 = ssize - 1;
	int nr0 = 0;
	int ww = data->GetNx();
	count = 0;
	int currPos = startPos;
	double nx1d = 1.0 / ww;
	ap = currPos;
	lp = 0;

	listS[0] = currPos;
	//listTmp[(count++)] = currPos;
	int startVal = volumeTempData[startPos];
	volumeTempData[ap] = 65535;
	int oldy;
	int x, y;
	int oldPos[3];
	int newPos[3];
	int sliceSize = data->GetSliceSize();

	if (startVal == val)
	{

		qDebug() << "Nothing to do!";
	}
	else
	{
		do
		{
			ap = listS[lp--];

			data->GetVoxelXYZPos(ap, oldPos);

			//oldy = ap * nx1d;

			for (int i = 0; i < testKernelSize; i++)
			{
				tmp = ap + testKernel[i];
				data->GetVoxelXYZPos(tmp, newPos);
				//	y = tmp * nx1d;
					//x = tmp - y * ww;


				if (!((tmp > dsizemin1) || (tmp < 0)))
				{
					int diff[3];
					for (int j = 0; j < 3; j++)
					{
						diff[j] = fabs(oldPos[j] - newPos[j]);
					}

					if ((diff[0] < 2) && (diff[1] < 2) && (diff[2] < 2))//bo kernel jest 1 wiec nie moze sie roznic nic o > niz jeden
					{

						if ((volumeTempData[tmp] == startVal))//!= 255)&&(volumeTempData[tmp]>0))
						{


							volumeTempData[tmp] = 65535;
							listS[++lp] = tmp;
							count++;


							if (bounds != NULL)
							{

								if (newPos[0] < bounds[0]) bounds[0] = newPos[0]; if (newPos[0] > bounds[1]) bounds[1] = newPos[0];
								if (newPos[1] < bounds[2]) bounds[2] = newPos[1]; if (newPos[1] > bounds[3]) bounds[3] = newPos[1];
								if (newPos[2] < bounds[4]) bounds[4] = newPos[2]; if (newPos[2] > bounds[5]) bounds[5] = newPos[2];
							}

						}

						if (lp == listSSize - 1)
						{
							//resize
							int newSize = listSSize + sliceSize * 10;
							int* tmp = new int[newSize];
							memcpy(tmp, listS, listSSize * sizeof(int));
							delete[]listS;
							listS = tmp;
							listSSize = newSize;

						}
					}



				}
			}
		} while (lp >= 0);

		if (totalMode == false)
		{
#pragma omp for   ordered schedule (guided) 
			for (int i = 0; i < data->GetVolumeSize(); i++)
			{
				if (volumeTempData[i] == 65535) volumeTempData[i] = val; //pomaluj tylko te wysegmentowane na wlasciwy kolor
				//volumeTempData[i] = (volumeTempData[i] == 65535) ? 0 : 3;
			}
		}
		else
		{
#pragma omp for   ordered schedule (guided) 
			for (int i = 0; i < data->GetVolumeSize(); i++)
			{
				volumeTempData[i] = (volumeTempData[i] == 65535) ? 0 : 3; //pomyaluj wysegmentowane na zero, reszte ustaw na wlasciwy kolor
			}
		}
	}


}
//-------------------------------

QString GetExcelColumnName(int columnNumber)
{
	int dividend = columnNumber;
	QString columnName = "";
	int modulo;

	while (dividend > 0)
	{
		modulo = (dividend - 1) % 26;
		columnName = QString((char)65 + modulo) + columnName;
		dividend = (int)((dividend - modulo) / 26);
	}

	return columnName;
}
//-------------------------------------------------------
RawDataSet* GetCroppedZVolume(RawDataSet* data, int zMin, int zMax)
{
	RawDataSet* output_data = NULL;
	if (data != NULL)
	{
		int n[3];
		data->GetN(n);
		double scale[3];
		double origin[3];
		data->GetScale(scale);
		data->GetOrigin(origin);
		if (zMin < 0) zMin = 0;
		if (zMax > n[2] - 1) zMax = n[2] - 1;

		int newN[3];
		newN[0] = n[0];
		newN[1] = n[1];
		newN[2] = zMax-zMin+1;//do przetestowania

		double newOrigin[3];
		newOrigin[0] = origin[0];
		newOrigin[1] = origin[1];
		newOrigin[2] = origin[2]+zMin*scale[2];//do przetestowania

		output_data = new RawDataSet(newN, scale, newOrigin, data->GetGantryAngle(), true);
		unsigned short* sourcePointer;
		unsigned short* destPointer;
		int sliceSize = data->GetSliceSize();
		for (int i = 0;i < newN[2];i++)
		{
			sourcePointer = data->GetDataArray() + (i + zMin) * sliceSize;
			destPointer= output_data->GetDataArray() + (i ) * sliceSize;
			memcpy(destPointer, sourcePointer, sliceSize * sizeof(unsigned short));
		}




	}

	return output_data;





}


unsigned char* GetCroppedZMask(RawDataSet *data,unsigned char* mask, int zMin, int zMax)
{

	unsigned char* output_mask = NULL;
	if (data != NULL)
	{
		int n[3];
		data->GetN(n);
		double scale[3];
		double origin[3];
		data->GetScale(scale);
		data->GetOrigin(origin);
		if (zMin < 0) zMin = 0;
		if (zMax > n[2] - 1) zMax = n[2] - 1;

		int newN[3];
		newN[0] = n[0];
		newN[1] = n[1];
		newN[2] = zMax - zMin + 1;//do przetestowania

		double newOrigin[3];
		newOrigin[0] = origin[0];
		newOrigin[1] = origin[1];
		newOrigin[2] = origin[2] + zMin * scale[2];//do przetestowania

		int newVolSize = newN[0] * newN[1] * newN[2];
		output_mask = new unsigned char[newVolSize];
		unsigned char* sourcePointer;
		unsigned char* destPointer;
		int sliceSize = data->GetSliceSize();
		for (int i = 0;i < newN[2];i++)
		{
			sourcePointer = mask + (i + zMin) * sliceSize;
			destPointer = output_mask + (i)*sliceSize;
			memcpy(destPointer, sourcePointer, sliceSize * sizeof(unsigned char));
		}




	}

	return output_mask;
}