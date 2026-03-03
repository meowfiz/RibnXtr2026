#include "ImageWidget.h"
//Added by qt3to4:
#include <QKeyEvent>
#include <QPolygon>
#include <QPixmap>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QPaintEvent>

//Widget used for drawing images

ImageWidget::ImageWidget(QWidget *pWidget, const char *name, Qt::WindowFlags f) : QWidget(pWidget, f)
{
	keyModesEnabled = true;
	cropLinesColor1 = QColor(Qt::magenta);
	cropLinesColor2 = QColor(Qt::darkCyan);
	warningMode = false;
	voxelScaleFactor = 1.0;
	sendXYMode = false;
	ellipseMode = false;
	ellipseDist = 2;;
	drawDualCross = false;
	drawTextsInCorners = false;
	pos3D[0] = 0; pos3D[1] = 0; pos3D[2] = 0;
	pos2D[0] = 0; pos2D[1] = 0;
	voxelRealValue = 0;

	for (int i = 0; i < 4; i++)
	{
		cropLines[i] = 0;
	}
	imageRescale = 1.0;
	cursonOnLines = false;
	moveLinesMode = false;
	nrOfSelectedLine = -1;
	showCropLines = false;

	maskColor = NULL;
	mask2Color = NULL;
	colsTab = NULL;
	volPointsOpacity = NULL;

	liveMaskTrace = false;
	shapes = NULL;
	borderSize = 8;
	colorButtonMode = -1;
	CrossColor = QColor(255, 255, 255);
	CrossColorValue = 0xffffffff;
	drawOpacityMode = false;
	sendShape = true;
	this->setAttribute(Qt::WA_NoSystemBackground, true);
	painterMode = -1;
	//PointArray=NULL;
	//PointIndex=0;
	SendMaskMode = false;
	PointArraySentAlready = true;



	Name = QString::fromLatin1(name);
	ImageExists = false;
	CrossType = 1;
	Interpolation = 0;
	InspectionMode = false;

	//Painter=new QPainter(this);

	ImageMask = NULL; //2003.11.26 - ciekawe ile takich kwiatkow jeszcze znajde

	TmpImage = NULL;
	tmpMask = NULL;
	tmpMask2 = NULL;
	maskImage = NULL;

	OldXSize = -1;
	OldYSize = -1;

	BlockRepaint = false;


	XFParamTab = NULL;
	YFParamTab = NULL;
	XIParamTab = NULL;
	YIParamTab = NULL;
	m_Image = NULL;
	this->setMouseTracking(true);


	PointPos.setX(-1);
	PointPos.setY(-1);
	PointSize.setX(-1);
	PointSize.setY(-1);
	connect(this, SIGNAL(SignalSendShape(QPolygonF*)), this, SLOT(SlotUpdatePolygon(QPolygonF*)));
}

//-----------------------------------------

void ImageWidget::DataCleared()
{


	ImageExists = false;
	InspectionMode = false;
	ImageMask = NULL;
	TmpImage = NULL;
	maskImage = NULL;
	tmpMask = NULL;
	tmpMask2 = NULL;
	OldXSize = -1;
	OldYSize = -1;
	BlockRepaint = false;
	XFParamTab = NULL;
	YFParamTab = NULL;
	XIParamTab = NULL;
	YIParamTab = NULL;
	m_Image = NULL;
	realXSize = 0;
	realYSize = 0;
}
//---------------------------------------

ImageWidget::~ImageWidget()
{
	//if (Painter!=NULL) delete Painter;
	if (TmpImage != NULL) delete TmpImage;
	if (maskImage != NULL) delete maskImage;
	if (tmpMask != NULL) delete[]tmpMask;
	if (tmpMask2 != NULL) delete[]tmpMask2;
	//Painter=NULL;
	tmpMask = NULL;
	tmpMask2 = NULL;
	TmpImage = NULL;
	maskImage = NULL;

}

//-------------------------------------------
void ImageWidget::setValues(QImage *pImage, int rx, int ry, double dx, double dy, unsigned char *_mask)
{
	mask = _mask;
	//QPainter Painter(this);
	QString aa = "setValues";
	//qWarning(aa);

	CrossType = 0;
	//MaskPaintMode=false;
	GetMaskMode = false;
	//PointArray=NULL;
	//PointIndex=0;
	//setMouseTracking(true);
	m_Image = pImage;
	m_rx = rx; //rozmiar rysunku
	m_ry = ry;
	m_dx = dx; //rozmiar pixela
	m_dy = dy;

	realXSize = rx*dx;
	realYSize = ry*dy;

	Button = false;
	Cross = false;

	TempX = 0;
	TempY = 0;
	XScale = 1.0;
	YScale = 1.0;

	//if (Painter!=NULL) delete Painter;
	//Painter=new QPainter(this);
	//

	//WYCZYSCIC QIDGET@!!!!!!
	//Painter.fillRect(0,0,width(),height(),BackgroundColor);
	//done


	//BackgroundColor=QColor(50,60,70);
	//setBackgroundColor(BackgroundColor);

	ImageExists = true;



	OldSrcWidth = -1;
	OldSrcHeight = -1;
	OldTgtWidth = -1;
	OldTgtHeight = -1;
	OldInterpolation = Interpolation;

	if (XFParamTab != NULL) delete[]XFParamTab;
	if (YFParamTab != NULL) delete[]YFParamTab;
	if (XIParamTab != NULL) delete[]XIParamTab;
	if (YIParamTab != NULL) delete[]YIParamTab;

	XFParamTab = NULL;
	YFParamTab = NULL;
	XIParamTab = NULL;
	YIParamTab = NULL;

	if (shapes != NULL)
	{
		shapes->dx = dx;
		shapes->dy = dy;
	}

}

//-------------------------------------------
void ImageWidget::SlotBlockRepaint(bool block)
{
	BlockRepaint = block;
}
//--------------------------------------------
void ImageWidget::paintEvent(QPaintEvent*)
{
	//qDebug() << Name << "-in";
	//QPainter Painter(this);
	//qWarning("paintEvent -in");

	/*if (parentName.compare("Fusion") == 0)
	{
	int i = 3;
	}*/

	if ((ImageExists == true) && (BlockRepaint == false))
	{
		//			QString aa="Paint Event"+QString::number(CrossType)+" "+QString(parent()->name());
		//qWarning(aa);
		//QPainter *Painter=new QPainter(this);
		//QBrush brush(CrossColor);
		//Painter->setBrush(brush);

		double	YMin, YMax, XMin, XMax;
		int		YMinI, YMaxI, XMinI, XMaxI;
		int		CrossYI, CrossXI;

		double m_sx, m_sy;
		//m_sx - tyle zajmuje 1 pixel szerokosci na ekranie

		if ((m_dx != 0) && (m_dy != 0))
		{
			m_sx = (double)(width()) / (double)(m_rx*m_dx);
			m_sy = (double)(height()) / (double)(m_ry*m_dy);
			m_s = m_sx<m_sy ? m_sx : m_sy;
			XScale = m_s*m_dx;
			YScale = m_s*m_dy;
		}
		else
		{
			m_sx = (double)(width()) / (double)(m_rx);
			m_sy = (double)(height()) / (double)(m_ry);
			m_s = 0;

			XScale = m_sx;
			YScale = m_sy;


		}


		int ImgSizeX1 = (int)(m_rx*XScale);
		int ImgSizeY1 = (int)(m_ry*YScale);
		int ImgPosX1 = (int)(0.5*(width() - ImgSizeX1));
		int ImgPosY1 = (int)(0.5*(height() - ImgSizeY1));

		cliprect.setCoords(ImgPosX1, ImgPosY1, ImgPosX1 + ImgSizeX1, ImgPosY1 + ImgSizeY1);


		ImgSizeX = (int)(m_rx*XScale)*imageRescale;
		ImgSizeY = (int)(m_ry*YScale)*imageRescale;
		ImgPosX = (int)(0.5*(width() - ImgSizeX));
		ImgPosY = (int)(0.5*(height() - ImgSizeY));
		XScale *= imageRescale;
		YScale *= imageRescale;



		rescaleDiffX = (ImgSizeX - ImgSizeX1) / 2;
		rescaleDiffY = (ImgSizeY - ImgSizeY1) / 2;

		if ((ImgSizeX <= 0) || (ImgSizeY <= 0)) return;




		if ((OldXSize != ImgSizeX) || (OldYSize != ImgSizeY))
		{

			if (TmpImage != NULL) delete TmpImage;
			if (maskImage != NULL) delete maskImage;

			TmpImage = new QImage(ImgSizeX, ImgSizeY, QImage::Format_RGB32);
			maskImage = new QImage(ImgSizeX, ImgSizeY, QImage::Format_ARGB32);
			//maskImage->fill(Qt::white);

			if (tmpMask != NULL) delete tmpMask;
			tmpMask = new unsigned char[ImgSizeX*ImgSizeY];
			if (tmpMask2 != NULL) delete tmpMask2;
			tmpMask2 = new unsigned char[ImgSizeX*ImgSizeY];
			QPainter Painter(this);  Painter.setPen(CrossColor); Painter.setClipRect(cliprect);


		}

		OldXSize = ImgSizeX;
		OldYSize = ImgSizeY;

		//qDebug() << Name << "-1";

		XMin = 0;
		YMin = 0;
		XMinI = (int)(XMin);
		YMinI = (int)(YMin);

		XMax = (m_rx)*XScale; //??
		YMax = (m_ry)*YScale; //??

		XMaxI = TmpImage->width() - 1;
		YMaxI = TmpImage->height() - 1;

		CrossX = XScale*(double)TempX;
		CrossY = YScale*(double)TempY;

		CrossXI = (int)(CrossX)+(int)(0.5*XScale);
		CrossYI = (int)(CrossY)+(int)(0.5*YScale);


		unsigned int *ImgTab = (unsigned int*)TmpImage->bits();
		int W = TmpImage->width();
		int H = TmpImage->height();


		//2003.09.29
		if (!((OldSrcWidth == m_rx) &&
			(OldSrcHeight == m_ry) &&
			(OldTgtWidth == ImgSizeX) &&
			(OldTgtHeight == ImgSizeY) &&
			(OldInterpolation == Interpolation)))
		{


			if (XFParamTab != NULL) delete[]XFParamTab;
			if (YFParamTab != NULL) delete[]YFParamTab;
			if (XIParamTab != NULL) delete[]XIParamTab;
			if (YIParamTab != NULL) delete[]YIParamTab;

			XFParamTab = NULL;
			YFParamTab = NULL;
			XIParamTab = NULL;
			YIParamTab = NULL;


			switch (Interpolation)
			{
			case 3:

				YFParamTab = new double[ImgSizeY * 4];
				XFParamTab = new double[ImgSizeY*ImgSizeX * 4];



				CreateBSplineTabs(m_rx, m_ry, ImgSizeX, ImgSizeY, XFParamTab, YFParamTab);


				break;
			}


			OldSrcWidth = m_rx;
			OldSrcHeight = m_ry;
			OldTgtWidth = ImgSizeX;
			OldTgtHeight = ImgSizeY;
			OldInterpolation = Interpolation;

		}

		if (Name.compare("Histogram") == 0)
		{
			int i = 0;
			/*QPainter painter(this);
			painter.setPen(CrossColor);
			ScaleImageNearest(m_Image, TmpImage);
			painter.drawImage(ImgPosX, ImgPosY, *TmpImage);
			return;*/
		}

		//qDebug() << Name << "-2";


		switch (Interpolation)
		{
		case 0:

			ScaleImageNearest(m_Image, TmpImage);

			break;

		case 1:

			ScaleImageBresenham(m_Image, TmpImage);

			break;

		case 2:
			ScaleImageLinear(m_Image, TmpImage);
			break;

		case 3:
			ScaleImageBicubic(m_Image, TmpImage, XFParamTab, YFParamTab);
			break;
		}


		int w1 = m_Image->width();
		int w2 = TmpImage->width();
		int h1 = m_Image->height();
		int h2 = TmpImage->height();
		if ((mask != NULL) && (maskColor != NULL))
		{

			// tu chcê namalowaæ po kolei - mask, mask2 oraz vol pts
			ScaleNearest(tmpMask, mask, w1, h1, w2, h2);
			unsigned int *bits = (unsigned int *)maskImage->bits();


			unsigned int col = (*maskColor).rgba();
			QColor backCol(0, 0, 0, 0);
			unsigned int back = backCol.rgba();
			int count = 0;

			for (int i = 0; i < w2*h2; i++)
			{
				bits[i] = (tmpMask[i] == 0) ? back : col;

				if (tmpMask[i]>0)count++;

			}

			{

				//sprawdzam czy cokolwiek jest w mask2
				int counter = 0;
				unsigned char *tMask = mask + w1*h1;
				for (int i = 0; i < w1* h1; i++)
				{
					if (tMask[i] > 0) { counter = 1; break; }
				}
				if (counter > 0)
				{
					ScaleNearest(tmpMask2, tMask, w1, h1, w2, h2); // bo mask2 jest o w1*h1 dalej
					QImage tmpIm(*maskImage);
					unsigned int *bits = (unsigned int *)tmpIm.bits();
					unsigned int col = (*mask2Color).rgba();
					QColor backCol(0, 0, 0, 0);
					unsigned int back = backCol.rgba();


					for (int i = 0; i < w2*h2; i++)
					{
						bits[i] = (tmpMask2[i] == 0) ? back : col;
					}

					QPainter pp(maskImage); //pp.setClipRect(cliprect);
					pp.drawImage(0, 0, tmpIm);

				}

			}

			if (colsTab != NULL)
			{

				//sprawdzam czy cokolwiek jest w mask2
				int counter = 0;
				unsigned char *tMask = mask + w1*h1 * 2; //a mask + w1*h1*2 - tu siedzi kolorwa maseczka
				for (int i = 0; i < w1* h1; i++)
				{
					if (tMask[i] > 0) { counter = 1; break; }
				}
				if (counter > 0)
				{
					unsigned int cols[256];
					int pos = 0;
					for (int i = 0; i < 256; i++)
					{
						cols[i] = QColor(colsTab[pos], colsTab[pos + 1], colsTab[pos + 2], *volPointsOpacity).rgba();;
						pos += 3;
					}

					ScaleNearest(tmpMask2, tMask, w1, h1, w2, h2); // bo mask2 jest o w1*h1 dalej
					QImage tmpIm(*maskImage);
					unsigned int *bits = (unsigned int *)tmpIm.bits();
					unsigned int col = (*mask2Color).rgba();
					QColor backCol(0, 0, 0, 0);
					unsigned int back = backCol.rgba();


					for (int i = 0; i < w2*h2; i++)
					{
						bits[i] = (tmpMask2[i] == 0) ? back : cols[tmpMask2[i]];
					}

					QPainter pp(maskImage); //pp.setClipRect(cliprect);
					pp.drawImage(0, 0, tmpIm);


				}
			}


			//	qDebug() << alpha;


		}

		if ((shapes != NULL) && (shapes->scale > 1.0))
		{

			double sc = shapes->scale;
			int interpolType = 0;//



			double center[2] = { shapes->xScalePos , shapes->yScalePos };


			int posTab[6] = { 0, 1, 2, 10, 11 };
			double t0[5];

			double scale[2];
			scale[0] = scale[1] = sc;

			t0[1] = 0;// center[0] * (1.0 - scale[0]); //tx
			t0[2] = 0;// center[1] * (1.0 - scale[1]); //ty
			t0[3] = scale[0];//scx
			t0[4] = scale[1];//scy
			double t[15];
			double matrix[16];
			//Transform *trans;
			double rCenter[3];

			for (int i = 0; i < 15; i++)
				t[i] = 0;
			t[10] = 1;
			t[11] = 1;
			t[12] = 1;

			rCenter[0] = center[0];
			rCenter[1] = center[1];
			rCenter[2] = 0;
			PrepareTrans2(t0, t, 4, posTab);
			Transform *trans = new Transform(rCenter, 1, 1, 1);

			trans->MakeCompleteAffineMatrixInv(matrix, t);
			//InterpolationFunction *intrp = NULL;


			//intrp = new InterpolationFunction2DNearest();


			int n[3];
			n[0] = w1;
			n[1] = h1;
			double t1 = n[0] - 1;
			double t2 = n[1] - 1;

			double Point[3] = { 0 };
			double Point1[3] = { 0 };
			unsigned short val;
			int ss = n[0] * n[1];
			int pos = 0;


			unsigned char *mask1A = new unsigned char[w1*h1];
			unsigned char *mask2A = new unsigned char[w1*h1];
			unsigned char *mask3A = new unsigned char[w1*h1];

			unsigned char *mask2 = mask + w1*h1;
			unsigned char *mask3 = mask + w1*h1 * 2;
			for (int j = 0; j < n[1]; j++)
			{
				//Point[0] = 0;


				for (int i = 0; i < n[0]; i++)
				{
					Point[0] = i;//przejscie do ukladu wspolrzednych globalnych 
					Point[1] = j;

					trans->MultiplyPoint(Point1, Point);
					//Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu
					val = 0;
					if ((Point1[0] >= 0) && (Point1[0] < t1) &&
						(Point1[1] >= 0) && (Point1[1] < t2))
					{
						//val= src->GetVoxelValue((int)(Point1[0]),(int)(Point1[1]),sliceNr);
						if ((shapes->mask != NULL) && (shapes->mask[pos] == 0))
						{
							mask1A[pos] = mask[pos];
							mask2A[pos] = mask2[pos];
							mask3A[pos] = mask3[pos];
						}
						else
						{

							int px = (int)(Point1[0] + 0.5);
							int py = (int)(Point1[1] + 0.5);
							int ppos = px + py*w1;
							mask1A[pos] = mask[ppos]; //intrp->GetInterpolatedValue(tab, n[0], ss, Point1[0], Point1[1], 0);
							mask2A[pos] = mask2[ppos];
							mask3A[pos] = mask3[ppos];
						}

					}
					else
					{
						mask1A[pos] = 0;
						mask2A[pos] = 0;
						mask3A[pos] = 0;
					}
					pos++;

					//Point[0]+=modelScale[0];
				}
			}
			//	delete intrp;
			delete trans;

			//	memcpy(mask+2*w1*h1, mask1A, w1*h1 * sizeof(unsigned char));
			//memset(mask,0, 2*w1*h1 * sizeof(unsigned char));
			ScaleNearest(tmpMask, mask1A, w1, h1, w2, h2);
			unsigned int *bits = (unsigned int *)maskImage->bits();
			//	QColor mColor = *maskColor;

			unsigned int col = (*maskColor).rgba();
			QColor backCol(0, 0, 0, 0);
			unsigned int back = backCol.rgba();
			int count = 0;
			int alpha = (*maskColor).alpha();
			for (int i = 0; i < w2*h2; i++)
			{
				bits[i] = (tmpMask[i] == 0) ? back : col;

				if (tmpMask[i]>0)count++;

			}


			{

				//sprawdzam czy cokolwiek jest w mask2
				int counter = 0;
				unsigned char *tMask = mask2A;
				for (int i = 0; i < w1* h1; i++)
				{
					if (tMask[i] > 0) { counter = 1; break; }
				}
				if (counter > 0)
				{
					ScaleNearest(tmpMask2, mask2A, w1, h1, w2, h2); // bo mask2 jest o w1*h1 dalej
					QImage tmpIm(*maskImage);
					unsigned int *bits = (unsigned int *)tmpIm.bits();
					unsigned int col = (*mask2Color).rgba();
					QColor backCol(0, 0, 0, 0);
					unsigned int back = backCol.rgba();


					for (int i = 0; i < w2*h2; i++)
					{
						bits[i] = (tmpMask2[i] == 0) ? back : col;
					}

					QPainter pp(maskImage); //pp.setClipRect(cliprect);
					pp.drawImage(0, 0, tmpIm);


				}

			}
			if (colsTab != NULL)
			{

				//sprawdzam czy cokolwiek jest w mask2
				int counter = 0;
				unsigned char *tMask = mask3A; //a mask + w1*h1*2 - tu siedzi kolorwa maseczka
				for (int i = 0; i < w1* h1; i++)
				{
					if (tMask[i] > 0) { counter = 1; break; }
				}
				if (counter > 0)
				{
					unsigned int cols[256];
					int pos = 0;
					for (int i = 0; i < 256; i++)
					{
						cols[i] = QColor(colsTab[pos], colsTab[pos + 1], colsTab[pos + 2], *volPointsOpacity).rgba();;
						pos += 3;
					}

					ScaleNearest(tmpMask2, mask3A, w1, h1, w2, h2); // bo mask2 jest o w1*h1 dalej
					QImage tmpIm(*maskImage);
					unsigned int *bits = (unsigned int *)tmpIm.bits();
					unsigned int col = (*mask2Color).rgba();
					QColor backCol(0, 0, 0, 0);
					unsigned int back = backCol.rgba();


					for (int i = 0; i < w2*h2; i++)
					{
						bits[i] = (tmpMask2[i] == 0) ? back : cols[tmpMask2[i]];
					}

					QPainter pp(maskImage); //pp.setClipRect(cliprect);
					pp.drawImage(0, 0, tmpIm);


				}
			}

			delete[]mask1A;
			delete[]mask2A;
			delete[]mask3A;


		}
		if (TransferFunctionMode == true)
		{
			//if (UserTransferFunction!=NULL)
			{

				//PaintTransferFunctionPoints(TmpImage,UserTransferFunction,TransferFunctionPointSize);
#if defined (VTK)
				PaintTransferFunctions(TmpImage);
#endif
			}


		}
		{
			//if (ellipseMode == true)
			//{
			//	int penwidth = 6;
			//	int ss = ellipseDist;
			//	QRect r(ss, ss, ImgSizeX - 2 * ss, ImgSizeY - 2 * ss);
			//	QRect r2(ss+6, ss+6, ImgSizeX - 2 * ss, ImgSizeY - 2 * ss);
			//	QImage cImg(ImgSizeX, ImgSizeY, QImage::Format_ARGB32);
			//	//QImage cImg(*TmpImage);
			//	QPainter circlePainter(&cImg);
			//	cImg.fill(backgroundColor);
			//	//QPainter::CompositionMode_Clear
			//
			//	circlePainter.setRenderHint(QPainter::Antialiasing);
			//	

			//	//QPen pen1(Qt::lightGray);
			//	//pen1.setWidth(6);
			//	//circlePainter.setPen(pen1);
			//	//circlePainter.drawEllipse(r2);
			//	
			//	
			//	QColor backCol(0, 0, 0, 0);
			//	circlePainter.setBrush(backCol);// Qt::red);
			//	QPen pen(Qt::black);
			//	pen.setWidth(penwidth);
			//	circlePainter.setPen(pen);
			//	circlePainter.drawEllipse(r);
			//	circlePainter.setCompositionMode(QPainter::CompositionMode_Clear);
			////	pen.setColor(Qt)
			//	pen.setWidth(2);
			//	circlePainter.setPen(pen);
			//	circlePainter.drawEllipse(r);
			//	
			//	

			//	QPainter testPainter(TmpImage);
			//	testPainter.drawImage(0, 0, cImg);
			//}

			//	cImg.save("c:\\pic2.png", "PNG");

			//narysuk prostokat, narysuj elipse zamaluj na TmpImage


			QPainter painter(this); painter.setClipRect(cliprect);
			painter.setPen(CrossColor);
			painter.drawImage(ImgPosX, ImgPosY, *TmpImage);
			//	qDebug() << Name << "-4.1";

			if (mask != NULL)
				painter.drawImage(ImgPosX, ImgPosY, *maskImage);
			//	QPolygon realPointArray;

			switch (painterMode)
			{

			case 1:
				painterMode = -1;

				//QPainter *Painter2=new QPainter(this);
				painter.setPen(CrossColor);

				if (colorButtonMode == 0)
					painter.setPen(Qt::red);
				else if (colorButtonMode == 1) painter.setPen(Qt::blue);
				painter.drawPolyline(PointArray);
				//painter.drawLine(PointArray->point(PointIndex-1).rx(),PointArray->point(PointIndex-1).ry(),PointX,PointY);
				//delete Painter2;
				break;
			case 2:
				painterMode = -1;


				painter.setClipRect(ImgPosX, ImgPosY, ImgSizeX, ImgSizeY);
				painter.setPen(CrossColor);
				painter.drawEllipse(xP1, yP1, wP1, hP1);


				break;
			case 3:
				painterMode = -1;

				painter.setClipRect(ImgPosX, ImgPosY, ImgSizeX, ImgSizeY);
				painter.setPen(CrossColor);
				painter.drawRect(xP1, yP1, wP1, hP1);


				break;
			case 4:
				painterMode = -1;
				painter.setPen(CrossColor);
				painter.drawPolyline(PointArray);
				//qWarning(QString::number(PointArray.size()));
				painter.drawLine(PointArray.point(PointArray.size() - 1).rx(), PointArray.point(PointArray.size() - 1).ry(), PointX, PointY);


				Point1.setX(PointX);
				Point1.setY(PointY);

				break;
			case 5:

				painterMode = -1;
				int qps = PointArray.size();


				QPolygon arr(PointArray);
				if (colorButtonMode == -1)
				{
					for (int i = 0; i < qps; i++)
					{
						//	realPointArray << QPoint(Round((PointArray.point(i).x() - ImgPosX)), Round((PointArray.point(i).y() - ImgPosY)));
						PointArray.setPoint(i, Round(fx*(PointArray.point(i).x() - ImgPosX)), Round(fy*(PointArray.point(i).y() - ImgPosY)));

					}

					emit SignalSendPointArray(PointArray, (double)m_rx / ImgSizeX, (double)m_ry / ImgSizeY);
				}
				else
				{
					int nXSize = this->ImgSizeX - 2 * borderSize;
					int nYSize = this->ImgSizeY - 2 * borderSize;

					double nfx = (double)m_rx / nXSize;
					double nfy = (double)m_ry / nYSize;

					for (int i = 0; i < qps; i++)
					{
						int xx = (arr.point(i).x() - ImgPosX) - borderSize;
						int yy = (arr.point(i).y() - ImgPosY) - borderSize;
						//	int xx=Round(nfx*(PointArray.point(i).x()-ImgPosX))-borderSize;
						//	int yy=Round(nfy*(PointArray.point(i).y()-ImgPosY))-borderSize;

						if (xx < 0) xx = 0;		if (xx > nXSize) xx = nXSize;
						if (yy < 0) yy = 0;		if (yy > nYSize) yy = nYSize;

						xx *= nfx;
						yy *= nfy;

						arr.setPoint(i, Round(xx), Round(yy));
					}

					emit SignalSendPointArray(arr, colorButtonMode, 0);//dla poprawy tr functions nie potrzebuje skali a przesle sobie nr klawisza - 0 lewy 1 prawy
					colorButtonMode = -1;
				}

				PointArraySentAlready = true;

				if (SendMaskMode == true)
				{
					//QPixmap Pixmap(m_rx,m_ry);
					//Pixmap.fill(Qt::black);
					QImage img(m_rx, m_ry, QImage::Format_RGB32);
					img.fill(Qt::black);

					QPainter Painter2(&img); //Painter2.setClipRect(cliprect);

					Painter2.setBrush(Qt::white);

					Painter2.setPen(1);

					Painter2.drawPolygon(PointArray);
					if (!((shapes != NULL) && (shapes->scale > 1.0)))
					{
						emit	SignalSendMaskImage(img);//(Image.convertDepth(8)));
					}
					else
					{
						MagnifyignGlassMask(&img);
					}

					//qDebug() << Name << "-4.02";
				}

				//qDebug() << Name << "-4.2";

				// to na chwile!!!!!!

				//delete PointArray;
				//PointArray=NULL;
				PointArray.clear();
				//qDebug() << Name << "-4.3";
				//	PointIndex=0;
				fPointArray.clear();
				//qDebug() << Name << "-4.4";
				emit ImageReleased(); //tmp
									  //qDebug() << Name << "-4.5";
				break;
			}
			//	qDebug() << Name << "-4.6";
		}
		//	case 6:
		if (painterMode == 6)
		{
			painterMode = -1;
			if ((PointPos.x() != -1) && (PointPos.y() != -1) && (PointSize.x() != -1) && (PointSize.y() != -1))
			{
				// (!((shapes != NULL) && (shapes->scale > 1.0)))
				{

					int x, y, w, h;

					x = Round((fx*PointPos.x()));
					y = Round((fy*PointPos.y()));
					w = Round((fx*PointSize.x()));
					h = Round((fy*PointSize.y()));

					QImage img(m_rx, m_ry, QImage::Format_RGB32);
					img.fill(Qt::black);
					QPainter Painter2(&img); //Painter2.setClipRect(cliprect);
					Painter2.setBrush(Qt::white);
					Painter2.setPen(1);
					Painter2.setPen(CrossColor);

					if (MaskPaintMode == 2)//ellipse
					{
						Painter2.drawEllipse(x, y, w, h);
					}
					else if (MaskPaintMode == 3)//rectangle
					{
						Painter2.drawRect(x, y, w, h);
					}




					//Image.save("c:\\pic2.png","PNG");
					//SendMaskMode
					//emit SignalSendMask((unsigned char*)(Image.convertDepth(8)).bits());

					if (!((shapes != NULL) && (shapes->scale > 1.0)))
					{
						emit	SignalSendMaskImage(img);//(Image.convertDepth(8)));
					}
					else
					{
						MagnifyignGlassMask(&img);
					}

					//emit	SignalSendMaskImage(img);//(Image.convertDepth(8)));
				}


				emit ImageReleased(); //tmp

				PointPos.setX(-1);
				PointPos.setY(-1);
				PointSize.setX(-1);
				PointSize.setY(-1);
			}
			///	break;
		}
		else
			if (painterMode == 7)
				//case 7:
			{
				painterMode = -1;
				//PointIndex++;
				//PointArray->resize(PointIndex+1);

				//PointArray->setPoint(PointIndex,Point1.x(),Point1.y());
				QPainter painter(this); painter.setClipRect(cliprect);
				PointArray << Point1;
				//QPainter *Painter2=new QPainter(this);
				painter.setPen(CrossColor);
				painter.drawPolyline(PointArray);
				//Painter2->drawLine(PointArray->point(PointIndex-1).rx(),PointArray->point(PointIndex-1).ry(),PointX,PointY);

				//break;
			}
		//	qDebug() << 3;

		//	qDebug() << Name << "-5";

		if ((mask != NULL) && (liveMaskTrace == true))
		{

			//skalujê maskê do rozmiarow rysunku za pomoca nearest
			QPainter painter(this); painter.setClipRect(cliprect);

			//znajduje granice maski

			QPolygon borders0;
			QPolygon borders1;
			unsigned char *line;
			//	poziom
			for (int j = 0; j < h2; j++)
			{
				line = tmpMask + j*w2;

				for (int i = 0; i < w2 - 1; i++)
				{
					if ((line[i] == 0) && (line[i + 1] == 1))
					{
						if ((i + j + timeChar) >> 2 & 1 == 1)
							borders0.append(QPoint(i + ImgPosX, j + ImgPosY));
						else
							borders1.append(QPoint(i + ImgPosX, j + ImgPosY));

					}
					else
						if ((line[i] == 1) && (line[i + 1] == 0))
						{
							if ((i + j + timeChar) >> 2 & 1 == 1)
								borders0.append(QPoint(i + 1 + ImgPosX, j + ImgPosY));
							else
								borders1.append(QPoint(i + 1 + ImgPosX, j + ImgPosY));
						}
				}
			}

			line = tmpMask;
			timeChar++;
			//pion
			for (int i = 0; i < w2 - 1; i++)

			{
				//line = tmpMask + j*w2;
				int tmpj0 = 0;
				int tmpj1 = w2;
				for (int j = 0; j < h2 - 1; j++)
				{
					if ((line[i + tmpj0] == 0) && (line[i + tmpj1] == 1))
					{
						if ((i + j + timeChar) >> 2 & 1 == 1)
							borders0.append(QPoint(i + ImgPosX, j + ImgPosY));
						else
							borders1.append(QPoint(i + ImgPosX, j + ImgPosY));

					}
					else
						if ((line[i + tmpj0] == 1) && (line[i + tmpj1] == 0))
						{
							if ((i + j + timeChar) >> 2 & 1 == 1)
								borders0.append(QPoint(i + ImgPosX, j + 1 + ImgPosY));
							else
								borders1.append(QPoint(i + ImgPosX, j + 1 + ImgPosY));

						}

					tmpj0 += w2;
					tmpj1 += w2;
				}
			}
			//	qDebug() << Name << "-5.01";
			painter.setPen(QColor(0, 0, 0));
			//qDebug() << Name << "-5.02";
			painter.drawPoints(borders0);
			//qDebug() << Name << "-5.03";
			painter.setPen(QColor(255, 255, 255));
			//qDebug() << Name << "-5.04";
			painter.drawPoints(borders1);
			//	qDebug() << Name << "-5.05";
			//robie dylacje
			//punkty dylacji rysujê na obrazku jako punkty painterem.


		}
		if (ellipseMode == true)
		{
			int penwidth = 6;
			int ss = ellipseDist;
			QRect r(ss, ss, ImgSizeX - 2 * ss, ImgSizeY - 2 * ss);
			QRect r2(ss + 6, ss + 6, ImgSizeX - 2 * ss, ImgSizeY - 2 * ss);
			QImage cImg(ImgSizeX, ImgSizeY, QImage::Format_ARGB32);
			//QImage cImg(*TmpImage);
			QPainter circlePainter(&cImg);
			cImg.fill(backgroundColor);
			//QPainter::CompositionMode_Clear

			circlePainter.setRenderHint(QPainter::Antialiasing);


			//QPen pen1(Qt::lightGray);
			//pen1.setWidth(6);
			//circlePainter.setPen(pen1);
			//circlePainter.drawEllipse(r2);


			QColor backCol(0, 0, 0, 0);
			circlePainter.setBrush(backCol);// Qt::red);
			QPen pen(Qt::black);
			pen.setWidth(penwidth);
			circlePainter.setPen(pen);
			circlePainter.drawEllipse(r);
			circlePainter.setCompositionMode(QPainter::CompositionMode_Clear);
			//	pen.setColor(Qt)
			pen.setWidth(2);
			circlePainter.setPen(pen);
			circlePainter.drawEllipse(r);



			//QPainter testPainter(this);
			QPainter painter(this);
			painter.drawImage(ImgPosX, ImgPosY, cImg);
		}
		//qDebug() << Name << "-6";

		if (shapes != NULL)
		{
			double xss = XScale;///m_dx;
			double yys = YScale;///m_ry;
			int aax = (int)(0.5*XScale);
			int aay = (int)(0.5*YScale);
			//QPainter painter(this);
			//qDebug() << 3.5;
			int nrShapes = shapes->shapeList.count();

			if (nrShapes>0)
			{

				MyShape shape;

				QList<MyShape> *slist = &shapes->shapeList;
				QPainter painter(this); painter.setClipRect(cliprect);


				for (int i = 0; i<nrShapes; i++)
				{
					//qDebug() << Name << "-7";
					shape = slist->at(i);
					//painter.setOpacity(shape.opacity);
					//polys
					if (shape.visible == false)continue; //jak nie widac to nic nie pokazuj
					if (shape.poly.count()>0)
					{
						int nrPts = shape.poly.count();
						QPolygon pp;
						QPointF pointf;
						int x, y;
						for (int jj = 0; jj<nrPts; jj++)
						{

							x = xss*shape.poly.at(jj).x() + ImgPosX + aax;
							y = yys*shape.poly.at(jj).y() + ImgPosY + aay;
							pp.append(QPoint(x, y));

						}

						painter.setPen(shape.polyColor);
						int pos;
						switch (shape.representation)
						{
						case 0:
							painter.drawPoints(pp);
							break;

						case 1:
							painter.drawPolyline(pp);
							break;
						case 2:

							if (shape.opacity == 0)
							{
								painter.drawPolygon(pp);
							}
							else
							{
								QPainterPath tmpPath;
								tmpPath.addPolygon(pp);
								painter.setOpacity(shape.opacity);
								painter.setBrush(QColor(shape.polyColor));
								painter.drawPath(tmpPath);// , QBrush(QColor::blue, Qt::SolidPattern));
								painter.setOpacity(1);
							}
							break;
						case 3: //separated segments
							for (int ii = 0; ii<pp.count() / 2; ii++)
							{
								painter.drawLine(pp.at(ii * 2), pp.at(ii * 2 + 1));
							}
							break;



						default:
							painter.drawPolyline(pp);

						}



						//case 4://ellipses
						//	pos=0;
						//	for(int ii=0;ii<pp.count()/6;ii++)
						//	{
						//		double center[2];
						//		double r[2],alpha,dest;
						//		center[0]=pp.at(pos).x
						//	}

					}

					//elipsy
					if (shape.ellipses.count()>0)
					{
						int nrEllipses = shape.ellipses.count() / 3;

						double center[2];
						double r[2], alpha, dest;
						int pos = 0;
						for (int ii = 0; ii<nrEllipses; ii++)
						{
							//CENTER
							center[0] = xss*shape.ellipses.at(pos).x() + ImgPosX + aax;
							center[1] = yys*shape.ellipses.at(pos).y() + ImgPosY + aay;
							//radiuses
							r[0] = XScale*shape.ellipses.at(pos + 1).x();
							r[1] = yys*shape.ellipses.at(pos + 1).y();

							//alpha
							alpha = shape.ellipses.at(pos + 2).x()*to_rad;
							//rot
							dest = (int)shape.ellipses.at(pos + 2).y();
							//

							double ca = cos(alpha);
							double sa = sin(alpha);

							double step = 360 / dest;

							double x, y;
							QPolygon ell;
							double t;
							double st, ct;
							for (int jj = 0; jj<360; jj += step)
							{
								t = jj *to_rad;
								ct = cos(t);
								st = sin(t);

								x = center[0] + r[0] * ct*ca - r[1] * st*sa;
								y = center[1] + r[0] * ct*sa + r[1] * st*ca;
								ell.append(QPoint(x, y));

							}

							painter.setPen(shape.ellipsesColor);


							if (shape.opacity == 0)
							{
								painter.drawPolygon(ell);
							}
							else
							{
								QPainterPath tmpPath;
								tmpPath.addPolygon(ell);
								painter.setOpacity(shape.opacity);
								painter.setBrush(QColor(shape.polyColor));
								painter.drawPath(tmpPath);// , QBrush(QColor::blue, Qt::SolidPattern));
								painter.setOpacity(1);
							}


							pos += 3;


						}

					}

					if (shape.shapeText.count()>0)
					{
						painter.save();
						painter.setFont(shape.font);
						painter.setPen(shape.textColor);
						painter.rotate(shape.textAngle);


						int dx = 0;
						int dy = 0;
						int fdx = 0;
						int fdy = 0;
						//if (shape.textAutoPos>=0)
						{
							//znajdz bounding box tekstu
							//wez bounding box obiektu
							// oblicz pozycje startu rysowania
							//dodaj ewenualna korekte

							if (shape.textToLogFlag == true)
							{
								emit this->SendMessageToLog(shape.shapeText);
								//shape = 
								//shape.shapeText = "";
								shape.textToLogFlag = false; //ma rysowac tylko raz
								slist->replace(i, shape);



							}


							QStringList fields;
							fields = shape.shapeText.split("\n", QString::SkipEmptyParts);


							int nrLines = fields.count();


							for (int ii = 0; ii<nrLines; ii++)
							{
								QFontMetrics fm(shape.font, this);
								QRect rect = fm.boundingRect(fields[ii]);
								if (rect.width()>fdx) fdx = rect.width();
								if (rect.height()>fdy) fdy = rect.height();

							}

							fdy *= nrLines*2.5;
							fdx *= 1.5;

							//mam rect textu



							//				    top

							//          left    oooooo right
							//					oooooo
							//					octroo
							//					oooooo
							//					oooooo
							//					
							//                  down
							//
							//
							//




							if (shape.textAutoPos >= 0)
							{
								qreal ox1, ox2, oy1, oy2 = 0;;
								shape.objectBounds.getCoords(&ox1, &oy1, &ox2, &oy2);

								//int fdx=abs(tx2-tx1);
								//int fdy=abs(ty2-ty1);

								int odx = fabs(ox2 - ox1);
								int ody = fabs(oy2 - oy1);


								int spacing = 5;//shape.font.pixelSize();

								switch (shape.textAutoPos)
								{
								case 0: //center
									dx = (ox1 + ox2)*0.5 - fdx*0.5;
									dy = (oy1 + oy2)*0.5 - fdy*0.5;


									break;

								case 1: //left
									dx = ox1 - fdx - spacing;
									dy = spacing + oy1;

									break;

								case 2: //right
									dx = spacing + ox2;
									dy = spacing + oy1;
									break;
								case 3: //top
									dx = spacing + ox1;
									dy = oy1 - fdy - spacing;

									break;

								case 4: //bottom
									dx = spacing + ox1;
									dy = spacing + oy2;

									break;


								}


							}

						}

						dx += shape.textPos.x();
						dy += shape.textPos.y();


						int pos[2];

						pos[0] = xss*dx + ImgPosX + aax;
						pos[1] = yys*dy + ImgPosY + aay;


						int pos2[2];
						pos2[0] = xss*(fdx)+ImgPosX + aax;
						pos2[1] = yys*(fdy)+ImgPosY + aay;



						painter.drawText(QRectF(pos[0], pos[1], pos2[0], pos2[1]), Qt::AlignLeft, shape.shapeText);

						/*	else
						{
						painter.translate(dx,dy);
						painter.drawText(0,0,shape.shapeText);

						}*/

						//painter.translate(pos[0],pos[1]);


						painter.restore();


					}


				}
			}

			//shapes->shapeList.clear(); //testowo 
			//	qDebug() << 4;
		}
		if ((CrossXI<TmpImage->width()) && (CrossXI >= 0) && (CrossYI<TmpImage->height()) && (CrossYI >= 0))
		{
			if (Cross == true)
			{




				int Arm = 10;
				int Arm2 = 5;

				//QColor c;
				//c.fromRgba((unsigned int)(CrossColorValue));
				QPainter painter(this); painter.setClipRect(cliprect);
				painter.setPen(CrossColor);


				int XLeft, XRight, YLeft, YRight;

				if (CrossX + Arm >= XMax)
					XRight = XMaxI - 1;
				else
					XRight = CrossXI + Arm;

				if (CrossX - Arm <= XMin)
					XLeft = XMinI + 1;
				else
					XLeft = CrossXI - Arm;


				if (CrossY + Arm >= YMax)
					YRight = YMaxI - 1;
				else
					YRight = CrossYI + Arm;
				if (CrossY - Arm <= YMin)
					YLeft = YMinI + 1;
				else
					YLeft = CrossYI - Arm;

				int xxr0 = 0, xxr1 = 0, yyr0 = 0, yyr1 = 0;
				if (CrossType>1)
				{


					double	IXScale = (double)TmpImage->width() / (double)m_Image->width();
					double	IYScale = (double)TmpImage->height() / (double)m_Image->height();




					if ((int)((double)(TempX - InspX)*IXScale)>0)
						xxr0 = (int)((double)((TempX - InspX))*IXScale);
					else
						xxr0 = 0;


					if ((int)((double)(TempY - InspY)*IYScale)>0)
						yyr0 = (int)((double)((TempY - InspY))*IYScale);
					else
						yyr0 = 0;


					if ((int)((double)(TempX + InspX)*IXScale)<TmpImage->width())
						xxr1 = (int)((double)(TempX + InspX + 1)*IXScale);
					else
						xxr1 = TmpImage->width() - 1;


					if ((int)((double)(TempY + InspY + 1)*IYScale)<TmpImage->height())
						yyr1 = (int)((double)(TempY + InspY + 1)*IYScale);
					else
						yyr1 = TmpImage->height() - 1;
				}


				CrossXI += ImgPosX;
				CrossYI += ImgPosY;

				XMaxI += ImgPosX;
				YMaxI += ImgPosY;

				QPen pen = painter.pen();
				QBrush brush = painter.brush();


				QPen pen1 = dualCrossPen1;
				QPen pen2 = dualCrossPen2;

				int triangleSize = 10;
				int p0[2], p1[2], p2[2];
				switch (CrossType)
				{
				case 0:

					//DrawPixel(TmpImage,CrossXI,CrossYI,CrossColorValue);
					//	DrawPointUINT(ImgTab, W, H, CrossXI, CrossYI, CrossColorValue);
					break;

				case 1:

					//	rescaleDiffX = (ImgSizeX - ImgSizeX1) / 2;
					//	rescaleDiffY = (ImgSizeY - ImgSizeY1) / 2;
					if (drawDualCross == true)
					{
						///qDebug() << "dual" << parentName;
						painter.setPen(pen1);
						if ((CrossXI - Arm2) >= 0)		painter.drawLine(CrossXI - Arm2, CrossYI, ImgPosX + rescaleDiffX, CrossYI);// DrawLineUINT(ImgTab, W, H, CrossXI - Arm2, CrossYI, 0, CrossYI, CrossColorValue);
						if ((CrossXI + Arm2) <= XMaxI)	painter.drawLine(CrossXI + Arm2, CrossYI, XMaxI - rescaleDiffX, CrossYI);// DrawLineUINT(ImgTab, W, H, CrossXI + Arm2, CrossYI, XMaxI, CrossYI, CrossColorValue);

																																 //trojkaciki
						p0[0] = ImgPosX + rescaleDiffX; p0[1] = CrossYI;
						p1[0] = ImgPosX + rescaleDiffX + triangleSize; p1[1] = CrossYI + triangleSize;
						p2[0] = ImgPosX + rescaleDiffX + triangleSize; p2[1] = CrossYI - triangleSize;
						painter.drawLine(p0[0], p0[1], p1[0], p1[1]); painter.drawLine(p0[0], p0[1], p2[0], p2[1]); painter.drawLine(p1[0], p1[1], p2[0], p2[1]);
						p0[0] = XMaxI - rescaleDiffX; p0[1] = CrossYI;
						p1[0] = XMaxI - rescaleDiffX - triangleSize; p1[1] = CrossYI + triangleSize;
						p2[0] = XMaxI - rescaleDiffX - triangleSize; p2[1] = CrossYI - triangleSize;
						painter.drawLine(p0[0], p0[1], p1[0], p1[1]); painter.drawLine(p0[0], p0[1], p2[0], p2[1]); painter.drawLine(p1[0], p1[1], p2[0], p2[1]);

						painter.setPen(pen2);
						if ((CrossYI - Arm2) >= 0)		painter.drawLine(CrossXI, ImgPosY + rescaleDiffY, CrossXI, CrossYI - Arm2);// DrawLineUINT(ImgTab, W, H, CrossXI, 0, CrossXI, CrossYI - Arm2, CrossColorValue);
						if ((CrossYI + Arm2) <= YMaxI)	painter.drawLine(CrossXI, CrossYI + Arm2, CrossXI, YMaxI - rescaleDiffY);// DrawLineUINT(ImgTab, W, H, CrossXI, CrossYI + Arm2, CrossXI, YMaxI, CrossColorValue);

						p0[0] = CrossXI; p0[1] = ImgPosY + rescaleDiffY;
						p1[0] = CrossXI + triangleSize; p1[1] = ImgPosY + rescaleDiffY + triangleSize;
						p2[0] = CrossXI - triangleSize; p2[1] = ImgPosY + rescaleDiffY + triangleSize;
						painter.drawLine(p0[0], p0[1], p1[0], p1[1]); painter.drawLine(p0[0], p0[1], p2[0], p2[1]); painter.drawLine(p1[0], p1[1], p2[0], p2[1]);

						p0[0] = CrossXI; p0[1] = YMaxI - rescaleDiffY;
						p1[0] = CrossXI + triangleSize; p1[1] = YMaxI - rescaleDiffY - triangleSize;
						p2[0] = CrossXI - triangleSize; p2[1] = YMaxI - rescaleDiffY - triangleSize;
						painter.drawLine(p0[0], p0[1], p1[0], p1[1]); painter.drawLine(p0[0], p0[1], p2[0], p2[1]); painter.drawLine(p1[0], p1[1], p2[0], p2[1]);
					}
					else
					{
						if ((CrossXI - Arm2) >= 0)		painter.drawLine(CrossXI - Arm2, CrossYI, ImgPosX, CrossYI);// DrawLineUINT(ImgTab, W, H, CrossXI - Arm2, CrossYI, 0, CrossYI, CrossColorValue);
						if ((CrossXI + Arm2) <= XMaxI)	painter.drawLine(CrossXI + Arm2, CrossYI, XMaxI, CrossYI);// DrawLineUINT(ImgTab, W, H, CrossXI + Arm2, CrossYI, XMaxI, CrossYI, CrossColorValue);
						if ((CrossYI - Arm2) >= 0)		painter.drawLine(CrossXI, ImgPosY, CrossXI, CrossYI - Arm2);// DrawLineUINT(ImgTab, W, H, CrossXI, 0, CrossXI, CrossYI - Arm2, CrossColorValue);
						if ((CrossYI + Arm2) <= YMaxI)	painter.drawLine(CrossXI, CrossYI + Arm2, CrossXI, YMaxI);// DrawLineUINT(ImgTab, W, H, CrossXI, CrossYI + Arm2, CrossXI, YMaxI, CrossColorValue);

						painter.drawPoint(CrossXI, CrossYI);
					}
					//painter.drawPoint(CrossXI, CrossYI);
					//	DrawPixel(TmpImage, CrossXI, CrossYI, CrossColorValue);
					painter.setPen(pen);
					break;


				case 2:

					if ((abs(PosZ - SliceNumber) <= InspZ))
					{
						int ix = (int)((InspX + 1)*XScale);
						int iy = (int)((InspY + 1)*YScale);
						//DrawEllipseUINT(ImgTab, W, H, CrossXI, CrossYI, ix, iy, CrossColorValue);
						painter.drawEllipse(QPoint(CrossXI, CrossYI), ix, iy);
						//DrawEllipseUCHAR(TempInspMask,TWidth,THeight,InspX,InspY,RX2,RY2,255); //rysuje elipse 255


					}

					//elipsa

					break;

				case 3:

					//kolko

					if ((abs(PosZ - SliceNumber) <= InspZ))
					{
						int rr = ((double)(InspX + 1)*XScale<(double)(InspY + 1)*YScale) ? (int)((InspX + 1)*XScale) : (int)((InspY + 1)*YScale);
						//DrawEllipseUINT(ImgTab, W, H, CrossXI, CrossYI, rr, rr, CrossColorValue);
						painter.drawEllipse(QPoint(CrossXI, CrossYI), rr, rr);

					}
					break;


				case 4:
					//trojkat

					if ((abs(PosZ - SliceNumber) <= InspZ))
					{

						//DrawLineUCHAR(TempInspMask,TWidth,THeight,InspX,0,TWidth-1,THeight-1,255);
						//DrawLineUCHAR(TempInspMask,TWidth,THeight,0,THeight-1,TWidth-1,THeight-1,255);
						//DrawLineUCHAR(TempInspMask,TWidth,THeight,0,THeight-1,InspX,0,255);
						int ix = (int)((InspX + 1)*XScale);
						int iy = (int)((InspY + 1)*YScale);

						painter.drawLine((int)CrossX, (int)(CrossY - iy), (int)(CrossX + ix), (int)(CrossY + iy));// DrawLineUINT(ImgTab, W, H, (int)CrossX, (int)(CrossY - iy), (int)(CrossX + ix), (int)(CrossY + iy), CrossColorValue);

						painter.drawLine((int)(CrossX + ix), (int)(CrossY + iy), (int)(CrossX - ix), (int)(CrossY + iy));// DrawLineUINT(ImgTab, W, H, (int)(CrossX + ix), (int)(CrossY + iy), (int)(CrossX - ix), (int)(CrossY + iy), CrossColorValue);
						painter.drawLine((int)(CrossX - ix), (int)(CrossY + iy), (int)CrossX, (int)(CrossY - iy));// DrawLineUINT(ImgTab, W, H, (int)(CrossX - ix), (int)(CrossY + iy), (int)CrossX, (int)(CrossY - iy), CrossColorValue);

					}

					break;


				case 5:
					DrawPixel(TmpImage, CrossXI, CrossYI, CrossColorValue);
					break;

				case 6:
					if ((abs(PosZ - SliceNumber) <= InspZ))
					{
						painter.drawLine(xxr0, yyr0, xxr1, yyr0);// DrawLineUINT(ImgTab, W, H, xxr0, yyr0, xxr1, yyr0, CrossColorValue);
						painter.drawLine(xxr0, yyr1, xxr1, yyr1);// DrawLineUINT(ImgTab, W, H, xxr0, yyr1, xxr1, yyr1, CrossColorValue);
						painter.drawLine(xxr0, yyr0, xxr0, yyr1);// DrawLineUINT(ImgTab, W, H, xxr0, yyr0, xxr0, yyr1, CrossColorValue);
						painter.drawLine(xxr1, yyr0, xxr1, yyr1);// DrawLineUINT(ImgTab, W, H, xxr1, yyr0, xxr1, yyr1, CrossColorValue);
					}




					break;

				}



			}
		}
		if (showCropLines == true)
		{




			QColor c1 = cropLinesColor1;
			QColor c2 = cropLinesColor2;

			QPainter painter(this); painter.setClipRect(cliprect);
			painter.setPen(Qt::magenta);

		//	int p[4] = { ImgPosX + cropLines[0] * XScale, ImgPosX + cropLines[1] * XScale, ImgPosY + cropLines[2] * YScale, ImgPosY + cropLines[3] * YScale };

			int p[4] = {
				ImgPosX + (cropLines[0]+0.5) * XScale,
				ImgPosX + (cropLines[1]-0.5) * XScale,
				ImgPosY + (cropLines[2]+0.5) * YScale,
				ImgPosY + (cropLines[3]-0.5) * YScale };


			painter.setPen(c1);	if (nrOfSelectedLine == 0)  painter.setPen(c2);
			painter.drawLine(p[0], ImgPosY, p[0], ImgPosY + TmpImage->height());
			painter.setPen(c1);	if (nrOfSelectedLine == 1)  painter.setPen(c2);
			painter.drawLine(p[1], ImgPosY, p[1], ImgPosY + TmpImage->height());
			painter.setPen(c1);	if (nrOfSelectedLine == 2)  painter.setPen(c2);
			painter.drawLine(ImgPosX, p[2], ImgPosX + TmpImage->width(), p[2]);
			painter.setPen(c1);	if (nrOfSelectedLine == 3)  painter.setPen(c2);
			painter.drawLine(ImgPosX, p[3], ImgPosX + TmpImage->width(), p[3]);

			//painter.setPen(shape.polyColor);
			//painter.restore();
			//painter.save();
		}

	}

	if (drawTextsInCorners == true)
	{
		QPainter p(this);
		const QPointF pt;

		p.setFont(font1);
		p.setPen(pen1);


		int pos = 0;

		if (objectTexts.count() > pos) drawText(p, QPointF(0, 0), Qt::AlignTop | Qt::AlignLeft, objectTexts.at(pos)); pos++;
		if (objectTexts.count() > pos) drawText(p, QPointF(width(), 0), Qt::AlignTop | Qt::AlignRight, objectTexts.at(pos)); pos++;

		p.setFont(font2);
		p.setPen(pen2);

		double x = pos2D[0];
		double y = pos2D[1];;
		//double x1, y1;
		//if ((IWidget->GetShapes() != NULL) && (IWidget->GetShapes()->scale > 1.0))
		//{
		//	double scale = IWidget->GetShapes()->scale;
		//	double center[2] = { IWidget->GetShapes()->xScalePos, IWidget->GetShapes()->yScalePos };
		//	//x = (x - center[0])*scale + x;
		//	//y = (y - center[1])*scale + y;

		//}

		QString unitPos = ((objectTexts.count() > 3)) ? objectTexts.at(3) : "";

		if (unitPos.compare("dont_show") != 0)
		{
			QString xxt; xxt.setNum(x, 'f', 2);
			QString yyt; yyt.setNum(y, 'f', 2);

			QString xx = xxt + unitPos;
			drawText(p, QPointF(0, height() - 80), Qt::AlignTop | Qt::AlignLeft, xx);
			QString yy = yyt + unitPos;
			drawText(p, QPointF(0, height() - 50), Qt::AlignTop | Qt::AlignLeft, yy);
		}

		double voxelValD = voxelRealValue*voxelScaleFactor;

		QString unitT = ((objectTexts.count() > 2)) ? objectTexts.at(2) : "";

		if (unitT.compare("dont_show") != 0)
		{

			QString nrl; nrl.setNum(voxelValD, 'f', 2);
			QString valT = nrl + unitT;


			drawText(p, QPointF(width(), height() - 50), Qt::AlignTop | Qt::AlignRight, valT);
		}
		if (warningMode == true)
		{
			p.setFont(warningFont);
			p.setPen(warningPen);
			drawText(p, QPointF(width(), height() - 100), Qt::AlignTop | Qt::AlignRight, warningText);
		}

	}
	painterMode = -1;
	//	qDebug() << Name << "-out";
	Cross = false;
	//	qWarning("paintEvent -out");
}
//-------------------------------------------------------
//-------------------------------------------------------

void ImageWidget::RepaintWithCross(int X, int Y)
{
	Cross = true;
	TempX = X;
	TempY = Y;
	//this->repaint();

}

//-------------------------------------------------------


void ImageWidget::mousePressEvent(QMouseEvent* event)
{
	emit signalRightWithAltPressed(event);
	emit signalImagePressed();
	SignalParentPleaseSendViewID();

	QPoint P = event->pos();

	PointX = P.rx();
	PointY = P.ry();

	int rx, ry;
	int tx, ty;

	tx = PointX - ImgPosX;
	ty = PointY - ImgPosY;

	double xss = realXSize / this->ImgSizeX;
	double yss = realYSize / this->ImgSizeY;

	if (cursonOnLines == true)
	{
		moveLinesMode = true;

		return;
	}
	if ((event->buttons() &Qt::LeftButton) && (event->modifiers()&Qt::ShiftModifier) && (sendXYMode == true))
	{
		double param1 = ((double)tx / ImgSizeX);
		double param2 = ((double)ty / ImgSizeY);
		pressX = param1; pressY = param2;

	}
	//qDebug() << Name << " " << param1 << " " << param2;
	if ((!(event->modifiers()&Qt::AltModifier)) || (PointArraySentAlready == true))
	{
		//(qWarning("pkt 1");
		if (PointArray.size()>0)
		{
			//delete PointArray;
			PointArray.clear();
			fPointArray.clear();
		};
		lastPoint.setX(tx*xss);
		lastPoint.setY(ty*yss);
	}

	this->setFocus();

	if ((event->buttons() &Qt::LeftButton) && (MaskPaintMode == 0))
	{
		emit SignalSendPointP1(tx, ty);

	}
	else if ((event->buttons() &Qt::RightButton) && (MaskPaintMode == 0))
	{
		emit SignalSendPointP2(tx, ty);
	}

	//qWarning("pts przed upd"+QString::number(PointArray.size()));
	if (((event->buttons() &Qt::RightButton) || ((drawOpacityMode == true) && ((event->buttons() &Qt::LeftButton)))) && (MaskPaintMode>0))
	{

		if ((tx >= 0) && (tx<ImgSizeX) && (ty >= 0) && (ty<ImgSizeY))
		{

			if (MaskPaintMode == 1)
			{
				if ((!(event->modifiers()&Qt::AltModifier)) || (PointArraySentAlready == true))
				{
					//(qWarning("pkt 1");
					//if (PointArray.size()>0)
					//{
					//	//delete PointArray;
					//	PointArray.clear();
					//	fPointArray.clear();
					//};
					//PointArray=new QPolygon();
					//PointIndex=0;

					//PointArray->setPoint(PointIndex,PointX,PointY);
					PointArray << QPoint(PointX, PointY);
					/*lastPoint.setX(tx*xss);
					lastPoint.setY(ty*yss);*/



					fPointArray << lastPoint;
					PointArraySentAlready = false;
					//PointIndex++;
				}
				else
				{
					// dodaj nowy punkt i przemaluj

					//qWarning(QString::number(PointArray.size()));
					if (PointArray.size()>0)
						if ((PointX != PointArray.point(PointArray.size() - 1).rx()) || (PointY != PointArray.point(PointArray.size() - 1).ry()))
						{

							//this->repaint();
							//PointIndex++;
							//PointArray->resize(PointIndex+1);


							PointArray << Point1;
							//PointArray->setPoint(PointIndex,Point1.x(),Point1.y());

							fPointArray << Point1f;
							lastPoint.setX(Point1f.x());
							lastPoint.setY(Point1f.y());
							/*QPainter *Painter2=new QPainter(this);
							Painter2->setPen(CrossColor);
							Painter2->drawPolyline(*PointArray);
							delete Painter2;*/
							//qWarning("pts kolejny z altem"+QString::number(PointArray.size()));
						}



				}

			}
			else if (MaskPaintMode == 2)//ellipse
			{
				Point1.setX(tx);
				Point1.setY(ty);

				Point1f.setX(tx*xss);
				Point1f.setY(ty*yss);

			}
			else if (MaskPaintMode == 3)//rectangle
			{
				Point1.setX(tx);
				Point1.setY(ty);

				Point1f.setX(tx*xss);
				Point1f.setY(ty*yss);
			}

		}
	}


	else
	{
		if ((m_dx>0) && (m_dy>0))
		{
			rx = (int)((double)tx / XScale);
			ry = (int)((double)ty / YScale);
		}
		else
		{
			rx = ((PointX*m_rx) / width());
			ry = ((PointY*m_ry) / height());
		}


		RX = rx;
		RY = ry;

		if ((event->buttons() &Qt::LeftButton) || (event->buttons() &Qt::RightButton) || (event->buttons() &Qt::MidButton))
		{


			if (event->buttons()&Qt::LeftButton)
			{
				Button = 0;
			}
			else if
				(event->buttons()&Qt::RightButton)
			{
				Button = 3;
			}
			else if
				(event->buttons()&Qt::MidButton)
			{
				Button = 6;
			}

			if ((rx >= 0) && (rx<m_rx))
			{

				if ((ry >= 0) && (ry<m_ry))
					emit SendPosition(rx, ry, Button);
				else if (ry<0)
					emit SendPosition(rx, 0, Button);
				else
					emit SendPosition(rx, m_ry - 1, Button);
			}
			else
			{

				if ((ry >= 0) && (ry<m_ry))
				{
					if (rx<0)
						emit SendPosition(0, ry, Button);
					else
						emit SendPosition(m_rx - 1, ry, Button);
				}
				else
				{
					if ((rx<0) && (ry<0))
						emit SendPosition(0, 0, Button);
					else if ((rx<0) && (ry>m_ry - 1))
						emit SendPosition(0, m_ry - 1, Button);
					else if ((rx>m_rx - 1) && (ry>m_ry - 1))
						emit SendPosition(m_rx - 1, m_ry - 1, Button);
					else if ((rx>m_rx - 1) && (ry<0))
						emit SendPosition(m_rx - 1, 0, Button);
				}

			}
#if defined (VTK)
			SlotPressTransferFunctionPoint(PointX, PointY, Button);
#endif
		}
	}

	if (drawOpacityMode == true)
	{
		if ((event->button() == Qt::RightButton))
		{
			colorButtonMode = 1;
		}
		else

			if ((event->button() == Qt::LeftButton))
			{
				colorButtonMode = 0;
			}
	}



	if (event->modifiers() & Qt::ControlModifier)
	{


		emit SendPosition2(m_rx - 1, m_ry - 1, Button);
	}



	if (event->button() & Qt::LeftButton)
	{
		if (event->type() & QEvent::MouseButtonDblClick)
		{
			emit signalDoubleClicked();
		}
	}
	//qWarning("pts przed po"+QString::number(PointArray.size()));
	//qWarning("mousePressEvent -out");
}

//--



//-----------------------------------------------
void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
	//qWarning("mouseMoveEvent -in");
	//qWarning("pts przed "+QString::number(PointArray.size()));
	//	emit SignalParentPleaseSendViewID();




	QPoint  P = event->pos();
	PointX = P.rx();
	PointY = P.ry();

	int rx, ry;

	int tx, ty;

	tx = PointX - ImgPosX;
	ty = PointY - ImgPosY;

	double xss = realXSize / this->ImgSizeX;
	double yss = realYSize / this->ImgSizeY;

	if ((event->buttons() &Qt::LeftButton) && (event->modifiers()&Qt::ShiftModifier) && (sendXYMode == true))
	{
		double param1 = ((double)tx / ImgSizeX);
		double param2 = ((double)ty / ImgSizeY);

		double dx = param1 - pressX;
		double dy = param2 - pressY;

		if (dx < -0.5)dx = -0.5;
		if (dy < -0.5)dy = -0.5;
		if (dx > 0.5)dx = 0.5;
		if (dy > 0.5)dy = 0.5;
		//double pressX = param1, pressY = param2;
		//
		//qDebug() << Name << " " << dx << " " << dy;
		emit	signalSendXY(dx, dy);
		return;
	}

	//---------------
	if ((showCropLines == true) && (moveLinesMode == false))
	{

		//wykrycie

		rx = (int)((double)tx / XScale);
		ry = (int)((double)ty / YScale);

		int tol = 4;
		if (abs(rx - cropLines[0])<tol ||
			abs((rx - cropLines[1]))<tol ||
			abs((ry - cropLines[2]))<tol ||
			abs((ry - cropLines[3]))<tol)
		{
			QApplication::setOverrideCursor(Qt::PointingHandCursor);
			cursonOnLines = true;


			if (abs(rx - cropLines[0]) < tol) nrOfSelectedLine = 0;
			else if (abs(rx - cropLines[1]) < tol) nrOfSelectedLine = 1;
			else if (abs(ry - cropLines[2]) < tol) nrOfSelectedLine = 2;
			else if (abs(ry - cropLines[3]) < tol) nrOfSelectedLine = 3;
			//	qDebug() << //"line nr: " << nrOfSelectedLine;

		}
		else
		{
			QApplication::setOverrideCursor(Qt::ArrowCursor);
			cursonOnLines = false;
			nrOfSelectedLine = -1;

		}
	}




	if (event->buttons() &Qt::LeftButton)
	{

		//przesuniecie
		if (moveLinesMode == true)
		{
			rx = (int)((double)tx / XScale);
			ry = (int)((double)ty / YScale);

			//qDebug() << "CURRENT LINE: " << nrOfSelectedLine;
			//qDebug() << "cropLines[2] " << cropLines[2] << " cropLines[3] " << cropLines[3] << " ry " << ry;
			switch (nrOfSelectedLine)
			{
			case 0:
				cropLines[0] = rx; if (cropLines[0] >= cropLines[1])cropLines[0] = cropLines[1] - 1;
				if (cropLines[0] < 0)  cropLines[0] = 0;
				break;
			case 1:
				cropLines[1] = rx; if (cropLines[1] <= cropLines[0])cropLines[1] = cropLines[0] + 1;
				if (cropLines[1] >m_rx - 1)  cropLines[1] = m_rx - 1;
				break;
			case 2:
				cropLines[2] = ry; if (cropLines[2] >= cropLines[3])cropLines[2] = cropLines[3] - 1;
				if (cropLines[2] < 0)  cropLines[2] = 0;
				break;
			case 3:
				cropLines[3] = ry; if (cropLines[3] <= cropLines[2])cropLines[3] = cropLines[2] + 1;
				if (cropLines[3] >m_ry - 1)  cropLines[3] = m_ry - 1;
				break;


			}
			//qDebug() << "cropLines[2] po zmianie " << cropLines[2];

			if ((nrOfSelectedLine >= 0) && (nrOfSelectedLine < 4))
			{


				emit signalCropLines(cropLines);
				//	qDebug() << "AAAAAAA";
			}


			return;
		}
	}


	if ((event->button() == Qt::LeftButton) && (MaskPaintMode == 0))
	{
		emit SignalSendPointP1(tx, ty);
	}
	else if ((event->button() == Qt::RightButton) && (MaskPaintMode == 0))
	{
		emit SignalSendPointP2(tx, ty);
	}


	//QMessageBox::about( 0,"move",QString::number(Button));
	//emit SendMessage(QString::number((int)event->buttons())+" "+QString::number(tx+ty));
	if (((event->buttons() &Qt::RightButton) || ((drawOpacityMode == true) && ((event->buttons() &Qt::LeftButton)))) && (MaskPaintMode>0))
		//if((event->buttons()&Qt::RightButton)&&(MaskPaintMode>0))
	{
		//qWarning(QString::number(PointArray.size()));

		if (MaskPaintMode == 1)//lasso
		{
			if ((tx >= 0) && (tx<ImgSizeX) && (ty >= 0) && (ty<ImgSizeY))
				if (PointArray.size()>0)
					if ((PointX != PointArray.point(PointArray.size() - 1).rx()) || (PointY != PointArray.point(PointArray.size() - 1).ry()))
					{

						//PointIndex++;
						//						PointArray->resize(PointIndex+1);

						PointArray << QPoint(PointX, PointY);
						//					PointArray->setPoint(PointIndex,PointX,PointY);
						fPointArray << QPointF(tx*xss, ty*yss);

						painterMode = 1;
						update();
						/*QPainter *Painter2=new QPainter(this);
						Painter2->setPen(CrossColor);
						Painter2->drawLine(PointArray->point(PointIndex-1).rx(),PointArray->point(PointIndex-1).ry(),PointX,PointY);
						delete Painter2;*/

						Point1.setX(PointX);
						Point1.setY(PointY);
						Point1f.setX(tx*xss);
						Point1f.setY(ty*yss);

						SlotGetPolylineParams(&fPointArray);

					}
		}
		else if (MaskPaintMode>1)
		{
			this->repaint();
			Point2.setX(tx);
			Point2.setY(ty);

			int x1, y1, x2, y2;
			x1 = Point1.x() + ImgPosX;
			x2 = Point2.x() + ImgPosX;
			y1 = Point1.y() + ImgPosY;
			y2 = Point2.y() + ImgPosY;



			if (x1<x2)
			{
				xP1 = x1; wP1 = x2 - xP1;
			}
			else
			{
				xP1 = x2; wP1 = x1 - xP1;
			}

			if (y1<y2)
			{
				yP1 = y1; hP1 = y2 - yP1;
			}
			else
			{
				yP1 = y2; hP1 = y1 - yP1;
			}


			if (event->modifiers()&Qt::ShiftModifier) // rowne
			{


				if (wP1<hP1)
				{
					if (y2 <= y1)
						yP1 += (hP1 - wP1);
					hP1 = wP1;
				}
				else
				{
					if (x2 <= x1)
						xP1 += (wP1 - hP1);
					wP1 = hP1;
				}
			}
			if (event->modifiers()&Qt::AltModifier) // center
			{
				xP1 = x1 - wP1;
				yP1 = y1 - hP1;
				wP1 *= 2;
				hP1 *= 2;


			}

			PointPos.setX(xP1 - ImgPosX);
			PointPos.setY(yP1 - ImgPosY);
			PointSize.setX(wP1);
			PointSize.setY(hP1);





			if (MaskPaintMode == 2)//ellipse
			{
				painterMode = 2;
				update();
				SlotGetEllipseParams(xss*PointPos.x(), yss*PointPos.y(), xss*PointSize.x(), yss*PointSize.y());
				/*QPainter *Painter2=new QPainter(this);
				Painter2->setClipRect ( ImgPosX, ImgPosY,ImgSizeX,ImgSizeY);
				Painter2->setPen(CrossColor);
				Painter2->drawEllipse(x,y,w,h);
				delete Painter2;*/
			}
			else if (MaskPaintMode == 3)//rectangle
			{
				SlotGetRectParams(xss*PointPos.x(), yss*PointPos.y(), xss*PointSize.x(), yss*PointSize.y());
				painterMode = 3;
				update();
				/*QPainter *Painter2=new QPainter(this);
				Painter2->setClipRect ( ImgPosX, ImgPosY,ImgSizeX,ImgSizeY);
				Painter2->setPen(CrossColor);
				Painter2->drawRect(x,y,w,h);
				delete Painter2;*/
			}

		}

	}
	else if (!((event->buttons()&Qt::LeftButton) || ((event->buttons()&Qt::RightButton)) || ((event->buttons()&Qt::MidButton))))
	{
		if ((MaskPaintMode == 1)) //lasso polygonal
		{
			if (event->modifiers()&Qt::AltModifier)
			{//qWarning("pts "+QString::number(PointArray.size()));
			 // jesli wcisniety jest alt
			 // narysuj aktualny polygon i linie do nast pktu
				if ((tx >= 0) && (tx<ImgSizeX) && (ty >= 0) && (ty<ImgSizeY))
					if (PointArray.size()>0)
						if ((PointX != PointArray.point(PointArray.size() - 1).rx()) || (PointY != PointArray.point(PointArray.size() - 1).ry()))
						{

							painterMode = 4;

							int nrr = fPointArray.count();
							lastPoint.setX(xss*tx);
							lastPoint.setY(yss*ty);
							update();
							SlotGetPolylineParams(&fPointArray);
							/*this->repaint();
							QPainter *Painter2=new QPainter(this);
							Painter2->setPen(CrossColor);
							Painter2->drawPolyline(*PointArray);
							Painter2->drawLine(PointArray->point(PointIndex).rx(),PointArray->point(PointIndex).ry(),PointX,PointY);
							delete Painter2;

							Point1.setX(PointX);
							Point1.setY(PointY);*/
							Point1f.setX(xss*tx);
							Point1f.setY(yss*ty);
						}
			}
			else if (PointArraySentAlready == false)
			{

				//mouseReleaseEvent(event);
			}
		}

	}
	else
	{
		if ((m_dx != 0) && (m_dy != 0))
		{
			rx = (int)((double)tx / XScale);
			ry = (int)((double)ty / YScale);
		}
		else
		{
			rx = (PointX*m_rx) / width();
			ry = (PointY*m_ry) / height();
		}



		RX = rx;
		RY = ry;

		if ((event->buttons()&Qt::LeftButton) || ((event->buttons()&Qt::RightButton)) || ((event->buttons()&Qt::MidButton)))
		{


			if (event->buttons()&Qt::LeftButton)
			{
				Button = 1;
			}
			else if
				(event->buttons()&Qt::RightButton)
			{
				Button = 4;
			}
			else if
				(event->buttons()&Qt::MidButton)
			{
				Button = 7;
			}

			if ((rx >= 0) && (rx<m_rx))
			{

				if ((ry >= 0) && (ry<m_ry - 1))
					emit SendPosition(rx, ry, Button);
				else if (ry<0)
					emit SendPosition(rx, 0, Button);
				else
					emit SendPosition(rx, m_ry - 1, Button);
			}
			else
			{
				if ((ry >= 0) && (ry<m_ry))
				{
					if (rx<0)
						emit SendPosition(0, ry, Button);
					else
						emit SendPosition(m_rx - 1, ry, Button);
				}
				else
				{
					if ((rx<0) && (ry<0))
						emit SendPosition(0, 0, Button);
					else if ((rx<0) && (ry>m_ry - 1))
						emit SendPosition(0, m_ry - 1, Button);
					else if ((rx>m_rx - 1) && (ry>m_ry - 1))
						emit SendPosition(m_rx - 1, m_ry - 1, Button);
					else if ((rx>m_rx - 1) && (ry<0))
						emit SendPosition(m_rx - 1, 0, Button);
				}


			}
#if defined (VTK)
			SlotMoveTransferFunctionPoint(PointX, PointY, Button);
#endif
		}


	}




}







//-------------------------------------------------------
void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{

	emit signalRightWithAltReleased(event);
	moveLinesMode = false;
	emit signalImageReleased();
	//qWarning("mouseReleaseEvent -in");
	//qWarning("pts przed upd"+QString::number(PointArray.size()));
	//	QMessageBox::about( 0,"release","");
	//	emit SignalParentPleaseSendViewID();
	this->setFocus();
	if (((Qt::RightButton) || ((drawOpacityMode == true) && ((Qt::LeftButton)))) && (MaskPaintMode>0))
		//if((Qt::RightButton)&&(MaskPaintMode>0))
	{

		fx = (double)m_rx / ImgSizeX;
		fy = (double)m_ry / ImgSizeY;
		////	if (SendMaskMode==false)
		//	{
		//		fx*=m_dx;
		//		fy*=m_dy;
		//	}

		int i;

		int x = event->modifiers();
		int y = Qt::AltModifier;
		int aa = x&y;

		if ((MaskPaintMode == 1))//lasso
		{

			if (PointArraySentAlready == false)
			{
				if (event->modifiers()&Qt::AltModifier)
				{
					/*PointIndex++;
					(*PointArray) <<QPoint(PointX,PointY);
					fPointArray<< this->lastPoint;*/
					painterMode = 4;

					update();
				}
				else

				{
					//PointIndex++;
					//PointArray->resize(PointIndex+1);
					//PointArray->setPoint(PointIndex,PointX,PointY);
					/*PointArray <<QPoint(PointX,PointY);
					fPointArray<< this->lastPoint;*/
					painterMode = 5;

					update();

					if (sendShape == true)
					{
						emit SignalSendShape(&fPointArray);
					}

				}
			}

		}

		else if (MaskPaintMode>1)
		{

			painterMode = 6;
			update();

			if (sendShape == true)
			{
				QPoint  P = event->pos();
				PointX = P.rx();
				PointY = P.ry();

				int rx, ry;

				int tx, ty;

				tx = PointX - ImgPosX;
				ty = PointY - ImgPosY;

				double xss = realXSize / this->ImgSizeX;
				double yss = realYSize / this->ImgSizeY;
				fPointArray.clear();

				if (MaskPaintMode == 2)//ellipse
				{
					//Painter2.drawEllipse(x,y,w,h);
					int count = 100;
					double center[2];
					double a = xss*PointSize.x() / 2;
					double b = yss*PointSize.y() / 2;
					double ab = a*b;
					center[0] = xss*PointPos.x() + a;
					center[1] = yss*PointPos.y() + b;


					double angle;
					double tmpA = to_rad * 360 / count;
					double radius;
					double aa, bb;
					double sa, ca;
					for (i = 0; i<count + 1; i++)
					{
						angle = tmpA*i;
						sa = sin(angle);
						ca = cos(angle);
						aa = a*sa;
						bb = b*ca;
						radius = ab / (sqrt(aa*aa + bb*bb));
						fPointArray << QPointF(center[0] + radius*ca, center[1] + radius*sa);
					}

					emit SignalSendShape(&fPointArray);
				}
				else if (MaskPaintMode == 3)//rectangle
				{

					//xss*PointPos.x(),yss*PointPos.y(),xss*PointSize.x(),yss*PointSize.y()
					fPointArray << QPointF(xss*PointPos.x(), yss*PointPos.y());
					fPointArray << QPointF(xss*PointPos.x() + xss*PointSize.x(), yss*PointPos.y());
					fPointArray << QPointF(xss*PointPos.x() + xss*PointSize.x(), yss*PointPos.y() + yss*PointSize.y());
					fPointArray << QPointF(xss*PointPos.x(), yss*PointPos.y() + yss*PointSize.y());
					fPointArray << QPointF(xss*PointPos.x(), yss*PointPos.y());

					emit SignalSendShape(&fPointArray);
					//Painter2.drawRect(x,y,w,h);
				}
			}

		}


	}







	if (event->buttons()&Qt::LeftButton)
	{
		Button = 2;
	}
	else if
		(event->buttons()&Qt::RightButton)
	{
		Button = 5;
	}
	else if
		(event->buttons()&Qt::MidButton)
	{
		Button = 8;
	}


	emit ImageReleased(Button);



	QPoint P = event->pos();
	PointX = P.rx();
	PointY = P.ry();



	//qWarning("pts po upd"+QString::number(PointArray.size()));
	//qWarning("mouseReleaseEvent -out");


}

//------------------------------------------------------------------

void  ImageWidget::keyPressEvent(QKeyEvent* event)
{
	//qWarning("keyPressEvent -in");
	/*	Qt::Key_Left
	Qt::Key_Up
	Qt::Key_Right
	Qt::Key_Down

	*/
	if (keyModesEnabled == true)
		if (event->modifiers()&Qt::ControlModifier)
		{
			if (event->key() == Qt::Key_Left)
				emit SignalMoveMask(0);
			else
				if (event->key() == Qt::Key_Right)
					emit SignalMoveMask(1);
				else
					if (event->key() == Qt::Key_Up)
						emit SignalMoveMask(2);
					else
						if (event->key() == Qt::Key_Down)
							emit SignalMoveMask(3);

						else if (event->key() == Qt::Key_C)
						{
							emit signalCopyMask();

						}
						else if (event->key() == Qt::Key_X)
						{
							emit signalPasteMask();

						}
			//qWarning() << "ctrlPressed";
		}
		else
		{
			int val = 0;
			if (event->key() == Qt::Key_Left)
				val = -1;
			if (event->key() == Qt::Key_Right)
				val = 1;
			emit signalArrowPressed(val);

		}
	if (event->key() == Qt::Key_F2)
	{
		unsigned char *bits = TmpImage->bits();
		emit SignalSendImage(TmpImage);
	}

	//qWarning("keyPressEvent -out");

}
//-------------------------------------------------------------

void  ImageWidget::keyReleaseEvent(QKeyEvent* event)
{

	if (event->modifiers()&Qt::ControlModifier)
	{
		//qWarning() << "ctrlReleased";
	}
	//qWarning("keyReleaseEvent -in");
	if ((MaskPaintMode == 1) && (event->key()&Qt::Key_Alt))//lasso
	{
		//if ((MaskPaintMode==1))//lasso
		{

			if (PointArraySentAlready == false)
			{
				////if (event->modifiers()&Qt::AltModifier)
				//{
				//	/*PointIndex++;
				//	(*PointArray) <<QPoint(PointX,PointY);
				//	fPointArray<< this->lastPoint;*/
				//	painterMode=4;

				//	update();
				//}
				//else

				{
					//PointIndex++;
					//PointArray->resize(PointIndex+1);
					//PointArray->setPoint(PointIndex,PointX,PointY);
					/*PointArray <<QPoint(PointX,PointY);
					fPointArray<< this->lastPoint;*/
					painterMode = 5;

					update();

					if (sendShape == true)
					{
						emit SignalSendShape(&fPointArray);
					}

				}
			}

		}
		//if (PointArray.size()>0)
		//	//			if ((PointX!=PointArray->point(PointIndex-1).rx())||(PointY!=PointArray->point(PointIndex-1).ry()))
		//{
		//	//qWarning("pts przed upd"+QString::number(PointArray.size()));
		//	painterMode=7;
		//	update();
		//	//qWarning(("pts po upd"+QString::number(PointArray.size())));
		//	//this->repaint();
		//	//PointIndex++;
		//	//PointArray->resize(PointIndex+1);
		//	//
		//	//PointArray->setPoint(PointIndex,Point1.x(),Point1.y());
		//	//QPainter *Painter2=new QPainter(this);
		//	//Painter2->setPen(CrossColor);
		//	//Painter2->drawPolyline(*PointArray);
		//	////Painter2->drawLine(PointArray->point(PointIndex-1).rx(),PointArray->point(PointIndex-1).ry(),PointX,PointY);
		//	//delete Painter2;
		//	/// a jezeli puszczony prawy przycisk myszy to zakoncz rysowanie krechy


		//}
	}
	//qWarning("keyReleaseEvent -out");

}

//------------------------------------------------------------------
void ImageWidget::SetInspectionWindowSizes(int x, int y, int z)
{

	InspX = x;
	InspY = y;
	InspZ = z;

}


void ImageWidget::SetMaskPaintMode(int Mode)
{
	MaskPaintMode = Mode;

}
//---------------------------------------------------------------------

void ImageWidget::SetGetMaskMode(int Mode)
{

	GetMaskMode = Mode;

}

//---------------------------------------------------------
void	ImageWidget::focusOutEvent(QFocusEvent *)
{
	//nothing // zeby jak traci focusa nie byl repaintowany! 2003.09.22
}
//------------------------------------------------------------



void ImageWidget::SetName(const QString &name)
{
	Name = name;
}

//------------------------------------------------

void ImageWidget::SetSliceNumber(int Nr)
{
	SliceNumber = Nr;
}


//------------------------------------------------
void ImageWidget::SetPosZ(int value)
{
	PosZ = value;
}

//------------------------------------------------
void ImageWidget::SetCrossColor(QColor Color)
{
	CrossColor = Color;
}

//------------------------------------------------
void ImageWidget::SetCrossType(int Type)
{
	//cout << "cross type" << Type << endl;
	//QString aa=parent()->name()+QString(" SetCrossType")+QString::number(Type);
	//qWarning(aa);
	CrossType = Type;
}

//------------------------------------------------
int	 ImageWidget::Getm_rx()
{
	return m_rx;
}

//------------------------------------------------
void ImageWidget::SetCrossColorValue(int Value)
{
	CrossColorValue = Value;
}

//------------------------------------------------
void ImageWidget::SetInterpolation(int Value)
{
	Interpolation = Value;
}

//------------------------------------------------

unsigned char *ImageWidget::FillMaskWithPolygon(QPolygon *Points, int ScreenX, int ScreenY, int MaskX, int MaskY)
{

	unsigned char *Tab = new unsigned char[MaskX*MaskY];
	memset(Tab, 0, MaskX*MaskY);
	int i;

	int x1, x2, y1, y2;

	double fx = (double)MaskX / ScreenX;
	double fy = (double)MaskY / ScreenY;

	int nrr = Points->size();
	if (nrr>1)

		for (i = 0; i<Points->size(); i++)
		{
			//Points->point(i).setX(Round(fx*Points->point(i).x()));
			//Points->point(i).setY(Round(fy*Points->point(i).y()));
			Points->setPoint(i, Round(fx*Points->point(i).x()), Round(fy*Points->point(i).y()));

		}


	// draw polyline
	for (i = 0; i<Points->size(); i++)
	{
		if (i<Points->size() - 2)
		{
			Points->point(i, &x1, &y1);
			Points->point(i + 1, &x2, &y2);
		}
		else
		{
			Points->point(i, &x1, &y1);
			Points->point(0, &x2, &y2);
		}

		//	DrawLineUCHAR(Tab,MaskX,MaskY,Round(x1*fx),Round(y1*fy),Round(x2*fx),Round(y2*fy),1);
		DrawLineUCHAR(Tab, MaskX, MaskY, x1, y1, x2, y2, 1);
	}






	// fill poly line
	//	emit SignalSendMask(Tab);
	/*	int start;

	unsigned char *TabX=Tab;

	for(j=0;j<MaskY;j++)
	{
	start=-1;

	for(i=0;i<MaskX;i.z++)
	{

	if (start==-1)
	{
	if (TabX[i]==1)
	start=i;
	}
	else
	{
	if (TabX[i]==1)
	{
	if ((i>0)&&(TabX[i-1]==0))
	{
	// namaluj kreche i wyzeruj start;
	memset(TabX+start,1,i-start);
	start=-1;
	}
	}

	}



	}
	TabX+=MaskX;
	}

	TabX=NULL;
	*/

	return Tab;
}

void ImageWidget::SlotSetSendMaskMode(bool a)
{

	SendMaskMode = a;
	if (a == true)
	{
		disconnect(this, SIGNAL(SignalSendShape(QPolygonF*)), this, SLOT(SlotUpdatePolygon(QPolygonF*)));
	}
	else
	{
		connect(this, SIGNAL(SignalSendShape(QPolygonF*)), this, SLOT(SlotUpdatePolygon(QPolygonF*)));
	}
}
//-----------------------------------------------
/*
void ImageWidget::initializeGL( void )
{
glClearColor( 0.0, 0.0, 0.0, 0.0 );
glShadeModel( GL_FLAT );
glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
}//initializeGL

//-----------------------------------------------

void ImageWidget::resizeGL( int w, int h )
{
glViewport(0, 0, (GLsizei) w, (GLsizei) h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

paintGL();
}


//-----------------------------------------------

void ImageWidget::paintGL()
{

glClear(GL_COLOR_BUFFER_BIT);
//glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB, GL_UNSIGNED_BYTE, checkImage);
glRasterPos2i ( 0, 0 );
//glPixelZoom (zoomFactor, zoomFactor);
glDrawPixels(this->width(), this->height(), GL_RGBA, GL_UNSIGNED_BYTE, scaledPicture);
glFlush ();

}//paintGL
//-------
*/
void	ImageWidget::SlotGetEllipseParams(double x0, double y0, double w, double h)
{
	double a = w*0.5;
	double b = h*0.5;
	double area = M_PI*a*b;
	double s = a*b;
	s = (fabs(s)>1e-5) ? sqrt(s) : 0;
	double length = M_PI*(1.5*(a + b) - s);

	QString mes = "Ellipse: ( Width:" + QString::number(w) + " Height:" + QString::number(h) + " Area:" + QString::number(area) + " Perimeter:" + QString::number(length) + ")";

	emit SendMessage2(mes);

	//emit	SignalSendShape(QPolygonF *p);
}
//-------
void	ImageWidget::SlotGetRectParams(double x0, double y0, double w, double h)
{
	double area = w*h;
	double length = 2 * (w + h);
	QString mes = "Rectangle: ( Width:" + QString::number(w) + " Height:" + QString::number(h) + " Area:" + QString::number(area) + " Perimeter:" + QString::number(length) + ")";

	emit SendMessage2(mes);
}
//-------
void	ImageWidget::SlotGetPolylineParams(QPolygonF *polys)
{
	int nr = polys->size();
	double p1[2], p2[3];
	double length = 0;
	double tlen = 0;
	QString angleTxt = "";

	if (nr>0)
	{
		for (int i = 1; i<nr + 1; i++)
		{
			p1[0] = polys->at(i - 1).x();
			p1[1] = polys->at(i - 1).y();

			if (i<nr)
			{
				p2[0] = polys->at(i).x();
				p2[1] = polys->at(i).y();
			}
			else
			{
				p2[0] = lastPoint.x();
				p2[1] = lastPoint.y();
			}
			tlen = (p2[0] - p1[0])*(p2[0] - p1[0]) + (p2[1] - p1[1])*(p2[1] - p1[1]);
			tlen = (fabs(tlen)>1e-5) ? sqrt(tlen) : 0;
			if ((shapes != NULL) && (shapes->scale > 0)) tlen /= shapes->scale;
			length += tlen;

		}
		if (nr>1)
		{
			double l1[2], l2[2];
			l1[0] = polys->at(nr - 2).x() - polys->at(nr - 1).x();
			l1[1] = polys->at(nr - 2).y() - polys->at(nr - 1).y();

			l2[0] = lastPoint.x() - polys->at(nr - 1).x();
			l2[1] = lastPoint.y() - polys->at(nr - 1).y();






			double len1 = sqrt(l1[0] * l1[0] + l1[1] * l1[1]);
			double len2 = sqrt(l2[0] * l2[0] + l2[1] * l2[1]);

			double scalar = l1[0] * l2[0] + l1[1] * l2[1];

			double m = len1*len2;
			double angle;
			if (fabs(m)>1e-5)
			{
				angle = acos(scalar / m)*to_deg;

			}
			else
				angle = 0;

			angleTxt = " angle: " + QString::number(angle);
		}

	}

	QString mes = QString::number(nr) + "PolyLine:( Total Length:" + QString::number(length) + " last Segment: " + QString::number(tlen) + angleTxt + " )";

	emit SendMessage2(mes);

}
//-------
void	ImageWidget::SlotGetPolygonParams(QPolygonF *polys)
{

}
//-------
void	ImageWidget::SlotUpdatePolygon(QPolygonF* poly)
{


	//wyczysc shape.p[olygon0
	//skopiuj zawartosc tego

	if (poly->count()>1)
	{
		if (shapes != NULL)
			if (shapes->cleanPolygonFlag == true)
			{

				shapes->polygonList.clear();
				shapes->polygonFlags.clear();


			}
		//	shapes->polygon0=*poly;
		QPolygonF newPoly;//=new QPolygonF;//n&shapes->polygon0;
		double xPrev, yPrev;
		double x, y;
		double m1x = 1.0 / m_dx;
		double m1y = 1.0 / m_dy;
		x = poly->at(0).x()*m1x;	y = poly->at(0).y()*m1y;


		newPoly.append(QPointF(x, y));

		x = newPoly.last().x();
		y = newPoly.last().y();
		double dist;
		for (int i = 1; i<poly->count(); i++)
		{
			xPrev = x;			yPrev = y;
			x = poly->at(i).x()*m1x;	y = poly->at(i).y()*m1y;
			dist = sqrt((double)(x - xPrev)*(x - xPrev) + (double)(y - yPrev)*(y - yPrev));
			if (dist>0.9)
			{
				if (dist>1.9)//test
				{
					//dodaj posrednie
					double xTmp, yTmp;
					double distStepX = (x - xPrev) / dist;
					double distStepY = (y - yPrev) / dist;
					for (int j = 1; j<dist; j++)
					{
						xTmp = xPrev + distStepX*j;
						yTmp = yPrev + distStepY*j;
						newPoly.append(QPointF(xTmp, yTmp));
					}

				}
				newPoly.append(QPointF(x, y));

			}
			else
			{
				x = xPrev; y = yPrev;
			}

		}
		if (shapes != NULL)
		{
			shapes->polygonList.append(newPoly);
			shapes->polygonFlags.append(0);
		}
		//trzeba zrobic dokladniej niz tylko kopiowanie - brac kolejne punkty - jesli nie ma
	}

	if (SendMaskMode == false)
	{
		//emit ImageReleased();
		emit SignalRefreshGui();
	}

}
#if defined (VTK)
//-----------------
void	ImageWidget::SetNewTransferFunction(vtkPiecewiseFunction *f1, vtkPiecewiseFunction *f2)
{
	volumeScalarOpacityFunction = f1;
	volumeGradientpacityFunction = f2;
};
void ImageWidget::SetTransferFunctionModeOn()
{
	TransferFunctionMode = true;
	MaskPaintMode = 1;
	drawOpacityMode = true;
}
void ImageWidget::PaintTransferFunctions(QImage *img)
{

	int size = borderSize;
	if (volumeScalarOpacityFunction != NULL)
	{
		double min = 0;
		double max = 0;
		volumeScalarOpacityFunction->GetRange(min, max);

		double tol = 1e-6;

		if (max - min<tol) return;

		int imgWidth = img->width() - 2 * size;
		int imgHeight = img->height() - 2 * size;

		if ((imgWidth <= 0) || (imgHeight <= 0)) return;

		int i, j;
		double val;
		double x, y;

		double yprev;
		QPainter p(img);
		p.setPen(Qt::red);
		val = volumeScalarOpacityFunction->GetValue(0);
		yprev = (1.0 - val)*imgHeight;
		double v1;
		for (i = 1; i<imgWidth; i++)
		{
			v1 = ((double)i*max) / imgWidth;
			val = 1.0 - volumeScalarOpacityFunction->GetValue(v1);

			y = val*imgHeight;

			p.drawLine((i - 1) + size, yprev + size, i + size, y + size);
			yprev = y;
		}

		int nrr = volumeScalarOpacityFunction->GetSize();
		double point[4];
		double max1 = (max>0) ? 1.0 / max : 1.0;
		int ss = 5;
		p.setPen(Qt::yellow);
		for (i = 0; i<nrr; i++)
		{
			volumeScalarOpacityFunction->GetNodeValue(i, point);
			v1 = point[0] * imgWidth*max1;
			p.drawEllipse(v1 - ss + size, imgHeight - ss, ss, ss);
		}

	}

	if (volumeGradientpacityFunction != NULL)
	{
		double min = 0;
		double max = 0;
		volumeGradientpacityFunction->GetRange(min, max);

		double tol = 1e-6;

		if (max - min<tol) return;

		int imgWidth = img->width() - 2 * size;
		int imgHeight = img->height() - 2 * size;

		if ((imgWidth <= 0) || (imgHeight <= 0)) return;

		int i, j;
		double val;
		double x, y;

		double yprev;
		QPainter p(img);
		p.setPen(Qt::blue);
		val = volumeGradientpacityFunction->GetValue(0);
		yprev = (1.0 - val)*imgHeight;
		double v1;
		for (i = 1; i<imgWidth; i++)
		{
			v1 = ((double)i*max) / imgWidth;
			val = 1.0 - volumeGradientpacityFunction->GetValue(v1);

			y = val*imgHeight;

			p.drawLine((i - 1) + size, yprev + size, i + size, y + size);
			yprev = y;
		}



	}

	QPainter p(img);
	p.setPen(Qt::green);


	int imgWidth = img->width() - 2 * size;
	int imgHeight = img->height() - 2 * size;
	p.drawRect(size, size, imgWidth, imgHeight);


}
//-------------------------------------------------------------

//-----------------------------------------------
void ImageWidget::SetTransferFunctionModeOff()
{
	TransferFunctionMode = false;
	MaskPaintMode = 0;
	drawOpacityMode = false;
}

void ImageWidget::SlotPressTransferFunctionPoint(int x, int y, int Button)
{

	//if(TransferFunctionMode==TRUE)
	//{
	//	if (UserTransferFunction!=NULL)
	//	{
	//		NumberOfPressedPoint=UserTransferFunction->GetNumberOfPressedPoint(x,y,TmpImage->width(),
	//			TmpImage->height(),TransferFunctionPointSize);
	//		//PaintTransferFunctionPoints(TmpImage,UserTransferFunction,10);
	//	}
	//}

	emit SignalSendUserTransferFunction();

}
//-----------------------------------------------
void ImageWidget::SlotMoveTransferFunctionPoint(int x, int y, int Button)
{
	/*if(TransferFunctionMode==TRUE)
	{
	if (UserTransferFunction!=NULL)
	{
	UserTransferFunction->MovePressedPoint(NumberOfPressedPoint,x,y,TmpImage->width(),
	TmpImage->height());
	}
	}*/
	emit SignalSendUserTransferFunction();
}
void ImageWidget::SlotReleaseTransferFunctionPoint(int x, int y, int Button)
{
	/*if (NumberOfPressedPoint!=-1)
	emit SignalSendUserTransferFunction(UserTransferFunction);

	NumberOfPressedPoint=-1;*/
}
//-----------------------------------------------
#endif
void ImageWidget::MagnifyignGlassMask(QImage *img)
{

	if ((shapes != NULL) && (shapes->scale > 1.0))
	{

		double sc = shapes->scale;
		int interpolType = 0;//



		double center[2] = { shapes->xScalePos, shapes->yScalePos };


		int posTab[6] = { 0, 1, 2, 10, 11 };
		double t0[5];

		double scale[2];
		scale[0] = scale[1] = sc;

		t0[1] = 0;// center[0] * (1.0 - scale[0]); //tx
		t0[2] = 0;// center[1] * (1.0 - scale[1]); //ty
		t0[3] = scale[0];//scx
		t0[4] = scale[1];//scy
		double t[15];
		double matrix[16];
		//Transform *trans;
		double rCenter[3];

		for (int i = 0; i < 15; i++)
			t[i] = 0;
		t[10] = 1;
		t[11] = 1;
		t[12] = 1;

		rCenter[0] = center[0];
		rCenter[1] = center[1];
		rCenter[2] = 0;
		PrepareTrans2(t0, t, 4, posTab);
		Transform *trans = new Transform(rCenter, 1, 1, 1);

		trans->MakeCompleteAffineMatrix(matrix, t);
		//InterpolationFunction *intrp = NULL;


		//intrp = new InterpolationFunction2DNearest();



		QImage imgX(m_rx, m_ry, QImage::Format_RGB32);
		imgX.fill(Qt::black);

		unsigned int *bitsX = (unsigned int *)imgX.bits();


		unsigned int *bits = (unsigned int *)img->bits();
		// wyczyszcze te ktore s¹ poza mask¹ z shapes
		int pos = 0;
		unsigned int white = QColor(255, 255, 255).rgb();
		//unsigned int black = Qt::black;
		//for (int j = 0; j < ImgSizeY; j++)
		//{
		//	//Point[0] = 0;


		//	for (int i = 0; i < ImgSizeX; i++)
		//	{
		//		if ((shapes->mask != NULL) )
		//		{
		//			if ((shapes->mask[pos] == 0))
		//			{

		//				bits[pos] = black;
		//			}
		//			else
		//			{
		//				i = i;
		//			}
		//		}
		//		pos++;
		//	}
		//
		//}

		//img->save("d:\\pic2.png", "PNG");
		int w1 = m_Image->width();
		int w2 = TmpImage->width();
		int h1 = m_Image->height();
		int h2 = TmpImage->height();

		int n[3];
		n[0] = w1;
		n[1] = h1;
		double t1 = n[0] - 1;
		double t2 = n[1] - 1;

		double Point[3] = { 0 };
		double Point1[3] = { 0 };
		unsigned short val;
		int ss = n[0] * n[1];

		//unsigned char *mask2 = new unsigned char[w1*h1];
		int pos1;
		//int countx = 0;
		for (int j = 0; j < h1; j++)
		{
			//Point[0] = 0;


			for (int i = 0; i < w1; i++)
			{
				Point[0] = i;//przejscie do ukladu wspolrzednych globalnych 
				Point[1] = j;

				trans->MultiplyPoint(Point1, Point);
				//Point1[0] *= objscale1[0]; Point1[1] *= objscale1[1];//laduje w ukladzie wspolrzednych obiektu
				val = 0;
				if ((Point1[0] >= 0) && (Point1[0] < t1) &&
					(Point1[1] >= 0) && (Point1[1] < t2))
				{
					//val= src->GetVoxelValue((int)(Point1[0]),(int)(Point1[1]),sliceNr);

					int px = (int)(Point1[0] + 0.5);
					int py = (int)(Point1[1] + 0.5);
					pos1 = px + py*w1;

					if (bits[pos1] == 0xffffffff)
						if (!((shapes->mask != NULL) && (shapes->mask[pos1] == 0)))
						{


							bitsX[pos] = 0xffffffff;
							//countx++;
						}


				}

				pos++;

				//Point[0]+=modelScale[0];
			}
		}
		//qDebug() << countx;
		//	delete intrp;
		delete trans;

		emit	SignalSendMaskImage(imgX);//(Image.convertDepth(8)));
										  //imgX.save("d:\\pic2.png", "PNG");
										  //img->save("d:\\pic1.png", "PNG");




										  //}


	}
}

void ImageWidget::SlotShowCropLines(bool val)
{
	showCropLines = val;
};
//----------------------------------
void  ImageWidget::SetCropLines(int *l)
{
	if (l != NULL)
	{
		for (int i = 0; i < 4; i++)
		{
			cropLines[i] = l[i];
		}
	}
}
