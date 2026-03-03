
#include "OtherImageFunctions.h"
QPolygonF	OpenSeparatedIslandsWithDistanceMap(unsigned short *tab2, int ww, int hh, int inputVal, double *params, int *listS, int *listTmp, DistanceMap *distMap, QImage *img, bool test)
{

	unsigned short *tab = new unsigned short[ww*hh];
	memcpy(tab, tab2, ww*hh * sizeof(unsigned short));
	// w lines zapisze kreski do podziałów - one beda potem wykorzystane do zamalowania na oryginalnym obrazie
	QPolygonF lines;
	//unsigned int *tabxCopy = (unsigned int *)(img->bits());
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
				kernel[pos] = i + j*ww;
				kernelInc[pos++] = j;
			}

		}
	}

	int ssize = ww*hh;

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

	int morphologyType = 1;// params[morphologyTypeID];
	int morphologyKernelType = 1;// params[morphologyKernelTypeID];



	int objectNr = 0;
	int *dilatePoints = listS;//new int[sliceSize];


	int jStart = 0;
	int jStop = ssize;

	QList <int> centerList;

	//QPainter p(img);


	//QFont font = p.font();
	//font.setPointSize(3);

	//p.setFont(font);




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

				oldy = ap*nx1d;

				for (int i = 0; i < kernelSize; i++)
				{
					tmp = ap + kernel[i];
					y = tmp*nx1d;
					x = tmp - y*ww;

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
			//if (count > coin_Area)
			//{
			//	//policze i zmienie scale dla calego volumenu
			//	int a = 5;

			//}
			//else
			//	if (((count < minAreaDivide) || (width < minWidth) || (width > maxWidth) || (height < minHeight) || (height > maxHeight)))//|| (count > maxArea)
				//{

				//	if ((count < coin_Area))
				//	{
				//		//wyczysc
				//		for (int i = 0; i < count; i++) { tab[listTmp[i]] = 0; }	//{	mask0[listS[dsizemin1-i]]=0;}
				//	}
				//	superTest = false;

				//}
			if (count == 0) superTest = false;

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
					int nSsize = nSizeX*nSizeY;

					int padd1 = 1;

					int nSizeX1 = nnx + padd1 * 2;
					int nSizeY1 = nny + padd1 * 2;

					if (nSizeX > maxX) maxX = nSizeX;
					if (nSizeY > maxY) maxY = nSizeY;

					//tworze maly zbiorek z danymi aktualnej wyspy powiekszonej o padd z kazdej strony
					unsigned char *tab1 = new unsigned char[nSsize];
					memset(tab1, 0, nSsize * sizeof(unsigned char));

					//zmniejszona tablica 
					//int rr, gg, bb;
					//rr = ((rand() * 255) / RAND_MAX); gg = ((rand() * 255) / RAND_MAX); bb = ((rand() * 255) / RAND_MAX);

					//QColor c(rr, gg, bb);
					//QColor c1(255 - rr, 255 - gg, 255 - bb);
					////QColor c(0, 255, 0);
					//p.setPen(c1);
					//unsigned int cI = (unsigned int)(c.rgba());



					//	int colorize = maskList->GetNumberOfItems() + 1;
					int fontx, fonty;
					for (int k = 0; k < count; k++)
					{

						y = listTmp[k] * nx1d;
						x = listTmp[k] - y*ww;

						if (k == 0)
						{
							fontx = x;
							fonty = y;
						}

						x += padd - minx;	y += padd - miny;
						tab1[x + y*nSizeX] = 1;
						tab[listTmp[k]] = 0;
						//tabxCopy[listTmp[k]] = cI;
					}

					//p.drawText(fontx, fonty, QString::number(blobCount));


					//------------------------------------------------












					//-----------------------------------------------------



					maskClass *mc = new maskClass();
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
					//DivideBlobUsingAcuteAngleCriterion(mc, &lines, morphologyKernelType, morphologyType, listS, listTmp, blobCount, test);
					DivideBlobUsingDistanceMap(mc, &lines, morphologyKernelType, morphologyType, listS, listTmp, distMap, test);

					blobCount++;

					delete mc;
					//maskList->InsertItem(mc);


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


void  DivideBlobUsingDistanceMap(maskClass *inputBlob, QPolygonF *lines, int morphologyKernelType, int morphologyType, int *listS, int *listTmp, DistanceMap *distMap, bool test )
{

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
	int nrOfDilPts = MaskListConturExtraction(listTmp, inputBlob->count, prms, listS);
	
	//	xmin = inputBlob->xPos;
	//	ymin = inputBlob->yPos;
	int pos = 0;
	int x, y;
	int xmin = 0;
	int ymin = 0;
	int xMin = 0;
	int yMin = 0;
	int dxMin, dyMin;
	double minDist = INT_MAX;
	int x1, y1;
	double dist;

	bool anyFound = false;
	for (int i = 0; i < nrOfDilPts; i++)
	{
		x = listS[pos++];
		y = listS[pos++];
		test = distMap->Get2DPos(x, y, x1, y1);
		if (test == true)
		{
			dist = sqrt(x1*x1 + y1*y1);
			if (dist < minDist)
			{
				minDist = dist;
				xMin = x;
				yMin = y;
				dxMin = x1;
				dyMin = y1;
				anyFound = true;
			}
		}

		//	tab[x + y*nx] = 1;
	}
	if (anyFound == true)
	{
		lines->append(QPointF(xMin, yMin));
		lines->append(QPointF(xMin - dxMin, yMin - dyMin));

	}
	//mam konktur bloba
	//pierwszy test znalezc minimum distance i zrobic kreske od punktu do tego miejsca.


}