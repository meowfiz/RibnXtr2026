#ifndef OTHER_COMMON_FUNCTIONS
#define OTHER_COMMON_FUNCTIONS
const static  unsigned int indexcolors[269] = {
	0x000000FF, 0xFFFF00FF, 0x1CE6FFFF, 0xFF34FFFF, 0xFF4A46FF, 0x008941FF, 0x006FA6FF, 0xA30059FF,
	0xFFDBE5FF, 0x7A4900FF, 0x0000A6FF, 0x63FFACFF, 0xB79762FF, 0x004D43FF, 0x8FB0FFFF, 0x997D87FF,
	0x5A0007FF, 0x809693FF, 0xFEFFE6FF, 0x1B4400FF, 0x4FC601FF, 0x3B5DFFFF, 0x4A3B53FF, 0xFF2F80FF,
	0x61615AFF, 0xBA0900FF, 0x6B7900FF, 0x00C2A0FF, 0xFFAA92FF, 0xFF90C9FF, 0xB903AAFF, 0xD16100FF,
	0xDDEFFFFF, 0x000035FF, 0x7B4F4BFF, 0xA1C299FF, 0x300018FF, 0x0AA6D8FF, 0x013349FF, 0x00846FFF,
	0x372101FF, 0xFFB500FF, 0xC2FFEDFF, 0xA079BFFF, 0xCC0744FF, 0xC0B9B2FF, 0xC2FF99FF, 0x001E09FF,
	0x00489CFF, 0x6F0062FF, 0x0CBD66FF, 0xEEC3FFFF, 0x456D75FF, 0xB77B68FF, 0x7A87A1FF, 0x788D66FF,
	0x885578FF, 0xFAD09FFF, 0xFF8A9AFF, 0xD157A0FF, 0xBEC459FF, 0x456648FF, 0x0086EDFF, 0x886F4CFF,

	0x34362DFF, 0xB4A8BDFF, 0x00A6AAFF, 0x452C2CFF, 0x636375FF, 0xA3C8C9FF, 0xFF913FFF, 0x938A81FF,
	0x575329FF, 0x00FECFFF, 0xB05B6FFF, 0x8CD0FFFF, 0x3B9700FF, 0x04F757FF, 0xC8A1A1FF, 0x1E6E00FF,
	0x7900D7FF, 0xA77500FF, 0x6367A9FF, 0xA05837FF, 0x6B002CFF, 0x772600FF, 0xD790FFFF, 0x9B9700FF,
	0x549E79FF, 0xFFF69FFF, 0x201625FF, 0x72418FFF, 0xBC23FFFF, 0x99ADC0FF, 0x3A2465FF, 0x922329FF,
	0x5B4534FF, 0xFDE8DCFF, 0x404E55FF, 0x0089A3FF, 0xCB7E98FF, 0xA4E804FF, 0x324E72FF, 0x6A3A4CFF,
	0x83AB58FF, 0x001C1EFF, 0xD1F7CEFF, 0x004B28FF, 0xC8D0F6FF, 0xA3A489FF, 0x806C66FF, 0x222800FF,
	0xBF5650FF, 0xE83000FF, 0x66796DFF, 0xDA007CFF, 0xFF1A59FF, 0x8ADBB4FF, 0x1E0200FF, 0x5B4E51FF,
	0xC895C5FF, 0x320033FF, 0xFF6832FF, 0x66E1D3FF, 0xCFCDACFF, 0xD0AC94FF, 0x7ED379FF, 0x012C58FF,

	0x7A7BFFFF, 0xD68E01FF, 0x353339FF, 0x78AFA1FF, 0xFEB2C6FF, 0x75797CFF, 0x837393FF, 0x943A4DFF,
	0xB5F4FFFF, 0xD2DCD5FF, 0x9556BDFF, 0x6A714AFF, 0x001325FF, 0x02525FFF, 0x0AA3F7FF, 0xE98176FF,
	0xDBD5DDFF, 0x5EBCD1FF, 0x3D4F44FF, 0x7E6405FF, 0x02684EFF, 0x962B75FF, 0x8D8546FF, 0x9695C5FF,
	0xE773CEFF, 0xD86A78FF, 0x3E89BEFF, 0xCA834EFF, 0x518A87FF, 0x5B113CFF, 0x55813BFF, 0xE704C4FF,
	0x00005FFF, 0xA97399FF, 0x4B8160FF, 0x59738AFF, 0xFF5DA7FF, 0xF7C9BFFF, 0x643127FF, 0x513A01FF,
	0x6B94AAFF, 0x51A058FF, 0xA45B02FF, 0x1D1702FF, 0xE20027FF, 0xE7AB63FF, 0x4C6001FF, 0x9C6966FF,
	0x64547BFF, 0x97979EFF, 0x006A66FF, 0x391406FF, 0xF4D749FF, 0x0045D2FF, 0x006C31FF, 0xDDB6D0FF,
	0x7C6571FF, 0x9FB2A4FF, 0x00D891FF, 0x15A08AFF, 0xBC65E9FF, 0xFFFFFEFF, 0xC6DC99FF, 0x203B3CFF,

	0x671190FF, 0x6B3A64FF, 0xF5E1FFFF, 0xFFA0F2FF, 0xCCAA35FF, 0x374527FF, 0x8BB400FF, 0x797868FF,
	0xC6005AFF, 0x3B000AFF, 0xC86240FF, 0x29607CFF, 0x402334FF, 0x7D5A44FF, 0xCCB87CFF, 0xB88183FF,
	0xAA5199FF, 0xB5D6C3FF, 0xA38469FF, 0x9F94F0FF, 0xA74571FF, 0xB894A6FF, 0x71BB8CFF, 0x00B433FF,
	0x789EC9FF, 0x6D80BAFF, 0x953F00FF, 0x5EFF03FF, 0xE4FFFCFF, 0x1BE177FF, 0xBCB1E5FF, 0x76912FFF,
	0x003109FF, 0x0060CDFF, 0xD20096FF, 0x895563FF, 0x29201DFF, 0x5B3213FF, 0xA76F42FF, 0x89412EFF,
	0x1A3A2AFF, 0x494B5AFF, 0xA88C85FF, 0xF4ABAAFF, 0xA3F3ABFF, 0x00C6C8FF, 0xEA8B66FF, 0x958A9FFF,
	0xBDC9D2FF, 0x9FA064FF, 0xBE4700FF, 0x658188FF, 0x83A485FF, 0x453C23FF, 0x47675DFF, 0x3A3F00FF,
	0x061203FF, 0xDFFB71FF, 0x868E7EFF, 0x98D058FF, 0x6C8F7DFF, 0xD7BFC2FF, 0x3C3E6EFF, 0xD83D66FF,

	0x2F5D9BFF, 0x6C5E46FF, 0xD25B88FF, 0x5B656CFF, 0x00B57FFF, 0x545C46FF, 0x866097FF, 0x365D25FF,
	0x252F99FF, 0x00CCFFFF, 0x674E60FF, 0xFC009CFF, 0x92896B };

#ifndef		VTK
#define VTK
#endif

#ifndef M_PI
#define M_PI 3.141592653589793115997963468544
#endif
#define EPS 2.2204e-16
#define to_rad M_PI/180.0
#define to_deg 180/M_PI
//#define ln2 0.69314718055994530941723212145818
#define ln1przez2 1.4426950408889634073599246810023

#define PARAMS_NR 100

#define   MIN(a,b)  ( ((a)<(b) ) ? (a) : (b) )
#define   MAX(a,b)  ( ((a)>(b) ) ? (a) : (b) )
#define MIN3(a,b,c) MIN( MIN( a, b ), c )
#define MID3(a,b,c) MAX( MIN( MAX( a, b ), c ), MIN( a, b) )
#define MAX3(a,b,c) MAX( MAX( a, b ), c )
#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}

#define BOOLX(x) (!(!(x)))

#define BITSETX(arg,posn) {arg=(arg | (1 << posn));} //((arg) | (1L << (posn)))
#define BITCLRX(arg,posn) {arg=((arg) & ~(1L << (posn)));}
#define BITTESTX(arg,posn) BOOLX((arg) & (1L << (posn)))
#define BITFLPX(arg,posn) {arg=((arg) ^ (1L << (posn)));}

#define NR_OF_MASK_COLORS 4096
//int jointTabIndices[NR_OF_MASK_COLORS];

//#ifndef NULL
//#define	NULL 0
//#endif
//
//#ifndef true
//#define	true	true
//#endif
//
//#ifndef false
//#define	false	false
//#endif
//#include <QtOpenGL>
#include <QAction>
#include <QtWidgets/qtwidgetsglobal.h>
#include <qobject.h>
#include <qthread.h>
#include <QtOpengl>
#include <stdlib.h>
#include <assert.h>
#include "FilterDefs.h"
#include <qdebug.h>
#include <stdio.h>
#include <math.h>
#include <qdatetime.h> 
#include <qstring.h>
#include <qapplication.h>
#include <qsettings.h>
#include <qdebug.h>
#include <qpainter.h>
#include <qbytearray.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qimage.h>
#include <qelapsedtimer.h>
#include <qaction.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <omp.h>
#include <iostream>
//#include "DistanceMap.h"
#include "RawDataSet.h"
#include "ImageShapes.h"
#include "Transform.h"
#include "SegCriterion.h"
#include "LinkedList.h"

#include <qtreewidget.h>
//#include <ppl.h>
//using namespace Concurrency;
using namespace std;

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#define EPS 2.2204e-16


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

class JointPoint3D
{

public:
	JointPoint3D() { pos = -1; frontNumber = -1; minPoint = -1; maxPoint = -1; flagNoTrace = 0; };
	double point3D[3];
	int pos;
	int frontNumber;

	//dodaje to tylko do tego zeby przy tracingu od danego punktu nie mogl zbiec do innych punktow z tego jointa
	int minPoint;
	int maxPoint;
	int flagNoTrace;
};

class JointData
{
public:
	QVector<int>frontBreakPts;
	QVector<int>inputValues;
	int mainValue;
};

class ParentAndAvoidData
{
public:

	QVector<int>avoid;
	QVector<int> parent;
	bool createdFromJoint;


	ParentAndAvoidData()
	{
		createdFromJoint = false;
	};

};

void findPeaks(vector<float> x0, vector<int>& peakInds);
void drawText(QPainter& painter, qreal x, qreal y, Qt::Alignment flags,
	const QString& text, QRectF* boundingRect = 0);
void drawText(QPainter& painter, const QPointF& point, Qt::Alignment flags,
	const QString& text, QRectF* boundingRect = {});
int  CheckForImageLineBackgroundCollision(unsigned char* bits, int ww, int hh, int p0[2], int p1[2], unsigned char objColor);

void	GetBoundsWithMargin(RawDataSet* data, unsigned char* inputMask, int marginSize, bool is2D, int* bounds);
void	GetBoundsWithMargin(RawDataSet* data, unsigned short* inputMask, int marginSize, bool is2D, int* bounds);
static const double one_sixth = 1.0F / 6.0F;
unsigned short GetMaxValue(unsigned short* tab, int size);
unsigned char  GetMaxValue(unsigned char* tab, int size);



inline unsigned int average(unsigned int a, unsigned int b)
{
	return ((a & 0xfefefeffUL) + (b & 0xfefefeffUL)) >> 1;
}
void	ScaleNearest(unsigned char* Target, unsigned char* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight);

void	ScaleImageBresenham(QImage* Source_Image, QImage* DestImage);
void	ScaleImageNearest(QImage* Source_Image, QImage* DestImage);

void	ScaleNearest(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight);
void	ScaleLineAvg(unsigned int* Target, unsigned int* Source, int SrcWidth, int TgtWidth);
void	ScaleRectAvg(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight);


void	ScaleImageLinear(QImage* Source_Image, QImage* DestImage);
void	ScaleLinear(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight);
void	PaddingBy2(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight);


void	ScaleImageBicubic(QImage* Source_Image, QImage* DestImage, double* XFParamTab, double* YFParamTab);
double	Cubic_bspline(double x);
void	BicubicScaling(unsigned int* Target, unsigned int* Source, int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight, double* XFParamTab, double* YFParamTab);
void	CreateBSplineTabs(int SrcWidth, int SrcHeight, int TgtWidth, int TgtHeight, double* XTab, double* YTab);

void	DrawPixel(QImage* Img, int x, int y, unsigned int color);




void SettSettings(QSettings& sett, QString contains, QString compare, QCheckBox* box);
void SettSettings(QSettings& sett, QString contains, QString compare, QSlider* slider);
void SettSettings(QSettings& sett, QString contains, QString compare, QLineEdit* ledit);
void SettSettings(QSettings& sett, QString contains, QString compare, QSpinBox* sbox);

void SettSettings(QSettings& sett, QString contains, QString compare, QAction* sbox);
double* GetStatisticsFromDoubleTab(double* tabx, int size, int nrOfBins, double* params, bool dontCreateHist = false);


void	GetHistogram(unsigned short* inuptTab, int inputSize, int* hist, int nrOfBins, int minVal, int maxVal, bool clear);
void GetStatisticsFromUShortTab(unsigned short* tabx, int size, double* params,bool computerHigherStats=false);

RawDataSet* GetCroppedZVolume(RawDataSet* data, int zMin, int zMax);
unsigned char* GetCroppedZMask(RawDataSet* data, unsigned char* mask, int zMin, int zMax);

void GetStatisticsFromVolumeSampledTab(RawDataSet* data, int* tab, int tabSize, double* params);

int* GetStatisticsFromVolumeSampledTabUsingMask(RawDataSet* data, unsigned char* mask, double* params);

void GetExpTab(double* _Tab, int number, double sigma, double intensityFactor);
double GetIntensityProbability(unsigned short intensity, unsigned short max, double sigma, double* expTab, unsigned short* samples, int nrOfSamples, int nrOfBins);
void GetIntensityProbabilityTab(double* tab, int nr, double& min, double& max, double sigma, double* expTab, unsigned short* samples, int nrOfBins, unsigned short maxSample, int nrOfSamples);


void GetIntensityProbabilityTab2(double* tab, double maxValue, double& min, double& max, double sigma, double* expTab, unsigned short* samplesCount, unsigned short* samplesIndex, int nrOfBins, int nrOfSamples);
double GetIntensityProbability2(unsigned short intensity, unsigned short max, double sigma, double* expTab, unsigned short* samplesCount, unsigned short* samplesIndex, int nrOfBins, int nrOfSamples);


int RemoveCollisionsFromLineSet(QPolygonF* lines, bool improveCollisions = false, bool testAll = false);
bool LineCollision(double l1[4], double l2[4], bool& coincident, bool& intersection, double intersectionPoint[2]);
double GetEntropy(int* Histogram, int size, int count);
void CompMutual(double& mi, double& entropy, int* ST, int* MT, int* OT, int size, int count);
void DrawHistogram(int bins, int* hist, int startX, int startY, int width, int height, bool logScale, QColor backC, QColor frontC, QImage* visImage, bool rotate90 = false, bool inv = false, bool clean = true, int* minMax = NULL, int zeroLevel = 0, QColor zeroColor = QColor(Qt::red), QVector<int>* circlePos = NULL, QVector <int>* hlines = NULL, QVector<QColor>* hColors = NULL);
void DrawJointHistogram(int bins, int* scatter, int startX, int startY, int width, int height, bool logScale, QColor backC, QColor frontC, QImage* visImage, double multiply = 1.0);

bool UpdateStats(int bins, int* hist, double* stats, QPolygonF poly, unsigned char* mask, int nx, int ny, unsigned short* tab, int** tmpTab, int& count, bool showBounds = false,
	ImageShapes* shapes = NULL, int maxValue = 0, bool showRegion = false, QColor regionCol = QColor(255, 255, 255));




void  PrepareTrans(double* tsrc, double* tdest, double nr, int* posTab);
void  PrepareTrans2(double* tsrc, double* tdest, double nr, int* posTab);
void  UnPrepareTrans2(double* tsrc, double* tdest, double nr, int* posTab);
bool Update2DStats(int bins, int* hist1, int* hist2, int* scatter, double* statsScatter, QPolygonF poly, int nx, int ny,
	unsigned short* tab1, unsigned short* tab2, int** tmpTab, int& count, ImageShapes* shapes = NULL,
	int maxValue1 = 0, int  maxValue2 = 0, bool useBiggerMax = true, bool useThresholds = false, double leftTh = 0, double rightTh = 1, bool invThresh = false, bool showReg1 = false, bool showReg2 = false);




//trans
bool UpdateStats(int bins, int* hist, double* stats, int* bounds, unsigned int* maskImgBits, RawDataSet* modelData, unsigned char* mask, int** tmpTab, int& count,
	int maxValue = 0,
	bool useTrans = false, double* trans = NULL, bool invTrans = false, RawDataSet* objectData = NULL);

bool Update2DStats2(int bins, int* hist1, int* hist2, int* scatter, double* statsScatter, int* bounds, unsigned int* maskImgBits,
	RawDataSet* modelData, RawDataSet* objectData, int** tmpTab, int& count, unsigned short* result = NULL,
	bool useTrans = false, double* trans = NULL, bool invTrans = false, double* rCenter = NULL);
// end trans

void		CalculateSkewNessAndKurtosis(double* tab, int size, double& skewness, double& kurtosis);
void		CalculateSkewNessAndKurtosis(unsigned short* tab, int size, double& skewness, double& kurtosis);
QPolygonF SmoothPolyLine(QPolygonF* polygon, int iter, double factor, double dx, double dy);
QPolygonF CenterPolyline(QPolygonF* polygon, int nx, int ny, unsigned char* mask, unsigned char badVal, double factor = 1.0, bool genLines = false, int dest = 1, bool medianLines = false, double medianFactor = 1.0, int kSize = 2, double maxDist = 0);

double	GetAngleBetweenLines2D(double* Point0, double* Point1, double* Point2);
double	GetAngleBetweenLines2DSigned(double* Point0, double* Point1, double* Point2);
double	GetAngleBetweenLines2DFull(double* Point0, double* Point1, double* Point2);
class DiffXKernelData
{
public:


	int* kernelPlus;
	int kernelPlusSize;
	int* kernelMinus;
	int kernelMinusSize;
	void ClearData() { if (kernelPlus != NULL) delete[]kernelPlus; if (kernelMinus != NULL) delete[]kernelMinus; kernelPlus = NULL; kernelMinus = NULL; kernelPlusSize = 0; kernelMinusSize = 0; };
	void UpdateKernels(int* kpt, int ps, int* kmt, int ms)

	{

		kernelPlus = kpt;
		kernelMinus = kmt;
		kernelPlusSize = ps;
		kernelMinusSize = ms;



	};



	DiffXKernelData() { kernelPlus = NULL; kernelMinus = NULL; kernelPlusSize = 0; kernelMinusSize = 0; };
	~DiffXKernelData() { ClearData(); };



};


void FillUShortData(RawDataSet* Data, unsigned short* volumeTempData, int startPos, int val, int* testKernel, int testKernelSize, int listSSize, int*& listS, bool totalMode = false, int* bounds = NULL, unsigned char* blockMask = NULL, int blockVal = -1);

int SlotFillMask2(RawDataSet* Data1, unsigned char* Data2, int x, int y, int z, int endValue, int _KernelType, int* bounds = NULL, unsigned char* mask2 = NULL, double* center = NULL, double* centerTab = NULL);

int SlotFillMaskFromRawData(RawDataSet* data, unsigned char* mask, int endVal, int x, int y, int z, int _KernelType, int* bounds = NULL, unsigned char* secondMask = NULL, int secondVal = 0);

RawDataSet* GreyScaleSimpleMorphology(RawDataSet* data, int mode, int size, int kernelType = 0, unsigned char* mask0 = NULL, int inmode = -1, int backmode = -1, int maxIter = 1, int* bounds = NULL);


void	SlotBinarySimpleMorphology(RawDataSet* data, unsigned char* mask, int mode, int size, int kernelType = 0, unsigned char* mask0 = NULL, bool dualMode = false, bool FlipMode = false, int* bounds = NULL, unsigned char* blockMask = NULL);
void 	SlotBinarySimpleMorphologyWithCriterion(RawDataSet* data, unsigned char* mask, int mode, int size, int kernelType = 0, unsigned char* mask0 = NULL, int* bounds = NULL, unsigned char* blockMask = NULL, SegCriterion* cr = NULL);



void GetKernelIntesitiesToTab(unsigned short* tab, unsigned short* tab1, int& tmpSize, int* kernel, int kernelSize, int pos, int dsizemin1);
int GetKernelIntesitiesToTabForConvolution(unsigned short* tab, unsigned short* tab1, int* convSrc, int* convDst, int& tmpSize, int* kernel, int kernelSize, int pos, int dsizemin1);
int Convolve(unsigned short* tab, int* fitler, int size);
//**8
void 	SlotBinarySimpleMorphologyWithCriterion2(RawDataSet* data, unsigned char* mask, int mode, int size, int kernelType = 0, unsigned char* mask0 = NULL, int* bounds = NULL, unsigned char* blockMask = NULL, SegCriterion* cr = NULL, bool is2D = false, int backVal = -1, int blockValue = 1);
void	SlotBinarySimpleMorphologyWithCriterion2Kernel(int pos, unsigned char* mask, unsigned char mode, int kernelSize, int* kernel, int dsizemin1, unsigned char* blockMask, unsigned char* mask2, SegCriterion* cr, int blockValue = 1);
void 	SlotBinarySimpleMorphologyWithCriterion2(RawDataSet* data, unsigned short* mask, int mode, int size, int kernelType = 0, unsigned short* mask0 = NULL, int* bounds = NULL, unsigned char* blockMask = NULL, SegCriterion* cr = NULL, bool is2D = false, int backVal = -1, int blockValue = 1);
void	SlotBinarySimpleMorphologyWithCriterion2Kernel(int pos, unsigned short* mask, unsigned char mode, int kernelSize, int* kernel, int dsizemin1, unsigned char* blockMask, unsigned char* mask2, SegCriterion* cr, int blockValue = 1);
//***



//***

void 	SlotBinarySimpleMorphologyWithCriterion2InOut(RawDataSet* data, unsigned char* mask, int* frontTabIn, int frontTabInLength, int* frontTabOut, int& frontTabOutLength, int* kernel, int KernelSize, unsigned char* blockMask = NULL, SegCriterion* cr = NULL, unsigned char tmpVal = 0, int blockValue = 1);
void	SlotBinarySimpleMorphologyWithCriterion2KernelInOut(int pos, RawDataSet* data, unsigned char* mask, int kernelSize, int* kernel, int dsizemin1, unsigned char* blockMask, SegCriterion* cr, int* frontTabOut, int& frontTabOutLength, unsigned char tmpVal = 0, int blockValue = 1);
void	RemapOutFrontTabWithDidivedIslands(int* frontTabOut, int frontTabOutLength, unsigned char* islandMask, unsigned char* tmpIslandMask, int*& listS, int& listSSize, RawDataSet* data, int& maxIsleNr, int kernelType = 0, bool is2D = false, bool showDebug = false, int tmpBaskBitNr = 7);

void 	SlotBinarySimpleMorphologyWithCriterion2InOut(RawDataSet* data, unsigned short* mask, int* frontTabIn, int frontTabInLength, int* frontTabOut, int& frontTabOutLength, int* kernel, int KernelSize, unsigned char* blockMask = NULL, SegCriterion* cr = NULL, unsigned short tmpVal = 0, int blockValue = 1, int** parallel = NULL, ParentAndAvoidData* parentAndAvoid = NULL, int* iterNr = NULL);
void	SlotBinarySimpleMorphologyWithCriterion2KernelInOut(int pos, RawDataSet* data, unsigned short* mask, int kernelSize, int* kernel, int dsizemin1, unsigned char* blockMask, SegCriterion* cr, int* frontTabOut, int& frontTabOutLength, unsigned short tmpVal = 0, int blockValue = 1);


void RemapOutFrontTabWithDidivedIslands(int* frontTabOut, int frontTabOutLength, int* frontTabIn, int frontTabInLength, int* frontTabOld, int frontTabOldLength, QVector <JointData>* frontBreakPoints, unsigned short* islandMask, unsigned char* tmpIslandMask, ParentAndAvoidData* parentAndAvoid, int*& listS, int& listSSize, RawDataSet* data, int& maxIsleNr, int kernelType = 0, bool is2D = false, bool showDebug = false, int tmpBaskBitNr = 7, QVector<JointPoint3D>* jPts = NULL, int* iterNr = NULL);
bool ReplaceNotValidCenterPointByClosestValid(double* p, RawDataSet* data, unsigned short properVal, unsigned short* valVolume, int* frontTab, int frontTabLength);
//***


void	GetGradient2DTab(double* output, unsigned short* input3D, double* kernel, unsigned short* tmpBuff, int size, int view, int nx, int ny, int nz, int zPos);
void	Get3DModulus(double* output, double* inX, double* inY, double* inZ, int size);
void	Remove_Gradient_NonMaxima_Slice_2D(double* maxima, double* gx, double* gy, double* norme, int* bufferDims, double epsNorm, double epsDeriv);
void	Remove_Gradient_NonMaxima_Slice_3D(double* maxima, double* gx, double* gy, double* gz, double** norme, int* bufferDims, double epsNorm, double epsDeriv);



int Morphology(unsigned char* mask, RawDataSet* data, int* bounds, unsigned char valIn, unsigned char valInTest, unsigned char valOut, int valRest = -1);
void simple2DMophology(unsigned char* buffer, int nSizeX, int nSizeY, int kernelType, int morphologyType);
QString RelativePath(QString absolutePath, QString relativeTo, bool bIsFile = false);

void	GetGrowingKernel(int& KernelSize, int* Kernel, int View, int KernelType, int width, int sliceSize);
int* GetGrowingKernel2(int& kernelSice, int size, RawDataSet* data, int type = 0);
int RegionGrowingOnMask(int startPos, unsigned char* maskData, RawDataSet* data, int valInt, int valOut, int valTemp, int startSize, unsigned char* blockMask = NULL);
bool RegionGrowingOnMask2(int startPos, unsigned char* maskData, RawDataSet* data, int valIn, int valOut,
	int* kernel, int* kernelInc, int kernelSize, int* params, int* listS, int* listTmp);


MyShape createDilateConturPoints(int* params, RawDataSet* data, int* listTmp, int morphologyKernelType, int morphologyType, int* dilatePoints, int col[3], int morphologyVal = -1, unsigned char* origMask = NULL, int dilateRadius = 0);


bool CheckForValueBetweenTwoVoxels(int pos1, int pos2, int dest, RawDataSet* data, double value, double tol = 1e-6, unsigned short* mask = NULL);

void		GetLinearInterpolatedPoint(double* p, double* p1, double* p2, double alpha);

double		GetDistance(int p1[3], int p2[3]);
double		GetDistance(double p1[3], double p2[3]);
double		GetDistance2D(double p1[2], double p2[2]);


int* GetGrowingKernel(int& kernelSize, int size, RawDataSet* data, int type);
int* GetGrowingKernel2D(int& kernelSize, int size, RawDataSet* data, int type, bool addCenter = false);

int* GetGrowingKernelDirectional2D(int& kernelSize, int size, RawDataSet* data, DiffXKernelData* diffData = NULL, int type = 0, int direction = 0, int p1 = 0, int p2 = 0);

int* GetGrowingKernel1D(int& kernelSize, int size, RawDataSet* data, bool direction, bool addCenter = false);

double		GetTriangleArea2D(double p0[2], double p1[2], double p2[2]);




struct maskClass
{
public:
	maskClass() { data = NULL; w = 0; h = 0; xPos = 0; yPos = 0; angle = 0; d1 = 0; d2 = 0; count = 0; nr = 0; };
	~maskClass() { if (data != NULL) delete[]data; };
	unsigned char* data;
	int w;
	int h;
	int count;
	int xPos, yPos;
	int maxX, maxY;
	double angle;
	double rotCenter[2];
	double d1, d2;
	int origW, origH;
	int nr;
	QColor meanColor;

};

struct IntDoubleStruct
{

	int		IntVar;
	double	DoubleVar;
};

struct IntIntStruct
{

	int		Int1Var;
	int		Int2Var;
};

struct IntIntIntStruct
{

	int		Int1Var;
	int		Int2Var;
	int		Int3Var;

};
struct DoubleDoubleDoubleStruct
{

	double		Double1Var;
	double		Double2Var;
	double		Double3Var;

};

struct Node
{
	int x, y, z;
};
inline bool BoolOperation(int mode, bool A, bool B)
{
	switch (mode)
	{
	case 0: //OR - OK
		return A | B;
		break;
	case 1: //CLEAR - OK 
		return !B & A;
		break;

	case 2: //AND - OK
		return A & B;
		break;
	case 3: //NOR - OK
		return !(A & B);
		break;
	case 4: //NAND - OK
		return !(A & B);
		break;
	case 5: //XOR
		return (A ^ B);
		break;
	case 6: //!CLEAR - OK 
		return !A & B;
		break;


	}
	return false;
};

inline bool compare2Doubles(double a, double b, double tol)
{

	if (fabs(a - b) < tol) return true;
	return false;

}
//-------------
inline bool compare2Doubles3(double* a, double* b, double tol)
{
	if ((fabs(a[0] - b[0]) < tol) && (fabs(a[1] - b[1]) < tol) && (fabs(a[2] - b[2]) < tol)) return true;
	return false;
}
//-------
inline unsigned short BoolOperationUnsignedShort(int mode, unsigned short A, unsigned short B)
{
	switch (mode)
	{
	case 0: //OR - OK
		return A | B;
		break;
	case 1: //CLEAR - OK 
		return !B & A;
		break;

	case 2: //AND - OK
		return A & B;
		break;
	case 3: //NOR - OK
		return !(A & B);
		break;
	case 4: //NAND - OK
		return !(A & B);
		break;
	case 5: //XOR
		return (A ^ B);
		break;
	case 6: //!CLEAR - OK 
		return !A & B;
		break;


	}
	return false;
};

//najnowsze funkcje do szybkiego poszukiwania wolumetrycznie odleglosci najblizszego punktu od danej wartosci
double* GetRealDistKernel(int& kernelSize, int size, RawDataSet* data);
IntDoubleStruct** GetRadiusKernelTab(int& size, int maxRadius, RawDataSet* data);
double  FindFirstValueFromTheCurrentPoint(int startPos, int val, IntDoubleStruct** distTabsKernel, int kernelSize, RawDataSet* data);



RawDataSet* PaddingBySize(int Size, RawDataSet* Input, int Value = -1);
unsigned char* PaddingBySizeUCHAR(int Size, RawDataSet* Input, unsigned char* Mask, int Value = -1);
unsigned short  Median(unsigned short* A, int n);
unsigned char  Median(unsigned char* A, int n);
double			ArithmeticAverage(unsigned short* A, int n);



int				Median(int* A, int n);
double			Median(double* A, int n);

unsigned short Minimum(unsigned short* tab, int n);
unsigned short Maximum(unsigned short* tab, int n);
double	Interpolation3LinearDouble(unsigned short* dataX, int Nx, int ss, double X, double Y, double Z);

double	Interpolation3LinearDouble(RawDataSet* dataX, double X, double Y, double Z);
unsigned short	Interpolation3Linear(RawDataSet* dataX, double X, double Y, double Z);
double	Interpolation3LinearDoubleSafe(RawDataSet* dataX, double X, double Y, double Z);
unsigned short	Interpolation3LinearSafe(RawDataSet* dataX, double X, double Y, double Z);



template		<class TYPE>
double			 Interpolation3Linear(TYPE* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z);
double			 Interpolation3LinearUnsignedShort(unsigned short* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z);
double			 Interpolation3LinearUnsignedChar(unsigned char* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z);

template		<class TYPE>
double			 Interpolation3LinearSafe(TYPE* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z);
double			 Interpolation3LinearUnsignedShortSafe(unsigned short* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z);
double			 Interpolation3LinearUnsignedCharSafe(unsigned char* dataX, int Nx, int Ny, int Nz, double X, double Y, double Z);


//inline double	log2(double x) { return log(x) * ln1przez2; };




template		<class TYPE>
void 			Padding(TYPE* IQ, TYPE* PQ, int INx, int INy, int INz, int PNx, int PNy, int PNz, int Size, int Value = -1);
template <class TYPE>
void 	Padding2D(TYPE* IQ, TYPE* PQ, int INx, int INy, int PNx, int PNy, int Size, int Value = -1);
unsigned char* Padding2D_Unsigned_Char(unsigned char* IQ, int INx, int INy, int Size, int Value = -1);
unsigned short* Padding2D_Unsigned_Short(unsigned short* IQ, int INx, int INy, int Size, int Value = -1);
unsigned int* Padding2D_Unsigned_Int(unsigned int* IQ, int INx, int INy, int Size, int Value = -1);



template <class TYPE>
void GetIntensityTabXYZ(TYPE* Input, int Nx, int Ny, int Nz, int Type, int View, int Size, TYPE* Tab, int x, int y, int z);
inline void GetIntensityTabPos(int dx, int dy, int dz, int& ndx, int& ndy, int& ndz, int x, int y, int z, int nx, int ny, int nz);


template <class TYPE>
void GetIntensityTab1D(TYPE* Input, int nx, int ny, int nz, int view, int Size, TYPE* tab, int x, int y, int z);
void GetIntensityTab1D(double* Input, int nx, int ny, int nz, int view, int Size, double* tab, int x, int y, int z);
void GetIntensityTab1D(unsigned short* Input, int nx, int ny, int nz, int view, int Size, unsigned short* tab, int x, int y, int z);
void GetIntensityTab1D(unsigned char* Input, int nx, int ny, int nz, int view, int Size, unsigned char* tab, int x, int y, int z);
void GetIntensityTab1D(RawDataSet* data, int x, int y, int z, int view, unsigned short* tab, int size);


void	GetIntensityTabXYZ(RawDataSet* Input, int Type, int View, int Size, unsigned short* Tab, int x, int y, int z);

template <class TYPE>
void	GetIntensityTab0(TYPE* Input, int SliceSize, int Nx, int Type, int View, int Size, TYPE* Tab, int Pos);
void	GetIntensityTab(RawDataSet* Input, int Type, int View, int Size, unsigned short* Tab, int Pos);
void	GetIntensityTab(unsigned char* Input, int Width, int Height, int Type, int View, int Size, unsigned char* Tab, int Pos);
int		GetIntetnsityTabLength(int KernelType, int KernelSize);

//RYSOWANIE LINII I ELIPSY - BRESENHAM
template <class TYPE>
void	DrawPoint(TYPE* image, int W, int H, int X, int Y, TYPE Color);

template <class TYPE>
void	DrawLine(TYPE* image, int W, int H, int Ax, int Ay, int Bx, int By, TYPE Color);
template <class TYPE>
void	ScanlineXFillArray(TYPE* image, int W, int H, TYPE col1, TYPE col2);
template <class TYPE>
void	DrawEllipse(TYPE* image, int W, int H, int x0, int y0, int a, int b, TYPE I);
template <class TYPE>
void	Draw4QuarterPixels(TYPE* image, int W, int H, int x0, int y0, int X, int Y, TYPE I);



void	DrawLineUSHORT(unsigned short* image, int W, int H, int Ax, int Ay, int Bx, int By, unsigned short Color);
void	DrawEllipseUSHORT(unsigned short* image, int W, int H, int x0, int y0, int a, int b, unsigned short I);
void	ScanlineXFillArrayUSHORT(unsigned short* image, int W, int H, unsigned short col1, unsigned short col2);
void	DrawLineUINT(unsigned int* image, int W, int H, int Ax, int Ay, int Bx, int By, unsigned int Color);
void	DrawEllipseUINT(unsigned int* image, int W, int H, int x0, int y0, int a, int b, unsigned int I);
void	ScanlineXFillArrayUINT(unsigned int* image, int W, int H, unsigned int col1, unsigned int col2);
void	DrawLineUCHAR(unsigned char* image, int W, int H, int Ax, int Ay, int Bx, int By, unsigned char Color);
void	DrawEllipseUCHAR(unsigned char* image, int W, int H, int x0, int y0, int a, int b, unsigned char I);
void	ScanlineXFillArrayUCHAR(unsigned char* image, int W, int H, unsigned char col1, unsigned char col2);

void	DrawPointUSHORT(unsigned short* image, int W, int H, int X, int Y, unsigned short Color);
void	DrawPointUINT(unsigned int* image, int W, int H, int X, int Y, unsigned int Color);
void	DrawPointUCHAR(unsigned char* image, int W, int H, int X, int Y, unsigned char Color);

void	QSortUnsignedShort(unsigned short* A, int Lo, int Hi);
void	QSortInt(int* A, int Lo, int Hi);
void	QSortFloat(double* A, int Lo, int Hi);
void	QSortDouble(double* A, int Lo, int Hi);
void	QSortIntDouble(IntDoubleStruct** A, int Lo, int Hi);
void	QSortInt1Int(IntIntStruct** A, int Lo, int Hi);
void	QSortInt1IntInt(IntIntIntStruct** A, int Lo, int Hi);
void	QSortIntInt1(IntIntStruct** A, int Lo, int Hi);
void	QSortDouble1DoubleDouble(DoubleDoubleDoubleStruct** A, int Lo, int Hi);


double calculateAverage(const QVector<double>& vec);
double calculateMedian(QVector<double> vec);

void	QSortInt3(int** A, int Lo, int Hi, int nr);


void	skipline(FILE* f);
char	ReadFileLine(FILE* file, QString* text);

QString	GetTimeDateName();

void	GetGaussianKernel(double* tab, double radius, double stddev);
void	GetGaussianKernelFirstDerivative(double* tab, double radius, double stddev);
void	GetGaussianKernelSecondDerivative(double* tab, double radius, double stddev);
void	GaussianSmoothingFiltering(double stddev, int radius, RawDataSet* Input, RawDataSet* _data);

unsigned short	Apply1DNormFilter(double* Tab1, unsigned short* Tab2, int	Size);
unsigned char	Apply1DNormFilter(double* Tab1, unsigned char* Tab2, int	Size);
double			Apply1DNormFilterF(double* Tab1, unsigned short* Tab2, int	Size);
//double			Apply1DNormFilterF(float *Tab1,float *Tab2,int	Size);
//double			Apply1DNormFilterF(double *Tab1,double *Tab2,int	Size);
double			Apply1DNormFilterF(double* Tab1, double* Tab2, int	Size);
double			Apply1DNormFilterF(double* Tab1, unsigned short* Tab2, int	Size);
void resizeBilinear(int* pixels, int w, int h, int* output, int w2, int h2);
void resizeBilinearVTKtoQt(int* pixels, int w, int h, int* output, int w2, int h2);
void resizeBilinearVTKtoQtBox(int* pixels, int w, int h, int* output, int w2, int h2, int* box = NULL);
void resizeBilinearVTKtoQtBoxInterlaced(int* pixels, int w, int h, int* output, int w2, int h2, int* box, bool side);

inline int		Round(double x) { return (x > 0) ? (int)(x + 0.5) : (int)(x - 0.5); };

//
//
inline bool neighFunctionSquare0(int i, int j, int size, double* params) { return (((i <= 0) && (j <= 0)) && ((abs(i) <= params[0]) && (abs(j) <= params[1]))) ? true : false; };
inline bool neighFunctionSquare1(int i, int j, int size, double* params) { return (((i <= 0) && (j >= 0)) && ((abs(i) <= params[0]) && (abs(j) <= params[1]))) ? true : false; };
inline bool neighFunctionSquare2(int i, int j, int size, double* params) { return (((i >= 0) && (j <= 0) && ((abs(i) <= params[0]) && (abs(j) <= params[1])))) ? true : false; };
inline bool neighFunctionSquare3(int i, int j, int size, double* params) { return (((i >= 0) && (j >= 0)) && ((abs(i) <= params[0]) && (abs(j) <= params[1]))) ? true : false; };





inline bool neighFunctionTriangle0(int i, int j, int size, double* params) { return (((j <= 0) && (abs(i) <= abs(j))) && ((abs(i) <= params[0]) && (abs(j) <= params[1]))) ? true : false; };
inline bool neighFunctionTriangle1(int i, int j, int size, double* params) { return (((j >= 0) && (abs(i) <= abs(j))) && ((abs(i) <= params[0]) && (abs(j) <= params[1]))) ? true : false; };
inline bool neighFunctionTriangle2(int i, int j, int size, double* params) { return (((i <= 0) && (abs(i) >= abs(j))) && ((abs(i) <= params[0]) && (abs(j) <= params[1]))) ? true : false; };
inline bool neighFunctionTriangle3(int i, int j, int size, double* params) { return (((i >= 0) && (abs(i) >= abs(j))) && ((abs(i) <= params[0]) && (abs(j) <= params[1]))) ? true : false; };

inline bool neighFunctionEllipse(int i, int j, int size, double* params)
{
	//double angle=direction*rad; 
	double sizex2 = params[0];//p1*p1; //params[0]
	double sizey2 = params[1];//p2*p2; //params[1]
	double cosa = params[2];//cos(angle);//params[2]
	double sina = params[3];//sin(angle);//params[3]
	double x, y;

	x = cosa * i - sina * j;
	y = sina * i + cosa * j;
	double tol = 1e-3;
	return (((x * x) / sizex2 + (y * y) / sizey2 - 1 < tol)) ? 1 : 0;

};

inline bool neighFunctionEllipseHalfY(int i, int j, int size, double* params)
{
	//double angle=direction*rad; 
	double sizex2 = params[0];//p1*p1; //params[0]
	double sizey2 = sqrt(params[1]) * 0.5; ;//polowa normalnej elipsy

	double halfy = sizey2 / 2;
	sizey2 *= sizey2;
	double cosa = params[2];//cos(angle);//params[2]
	double sina = params[3];//sin(angle);//params[3]
	double x, y;

	x = cosa * i - sina * j;
	y = sina * i + cosa * j;
	double tol = 1e-3;
	return (((x * x) / sizex2 + (y + halfy) * (halfy + y) / sizey2 - 1 < tol)) ? 1 : 0;

};


inline bool neighFunctionSquareCenter(int i, int j, int size, double* params) { return ((abs(i) <= params[0]) && (abs(j) <= params[1])) ? 1 : 0; };





struct PaletteRGBA
{
	unsigned char R[256];
	unsigned char G[256];
	unsigned char B[256];
	unsigned char A[256];

};



class Timer
{
public:

	Timer();
	QString GetTime();

	QTime Time;



};

class RibonTimer : public QObject
{
	Q_OBJECT

public:


	void Start(const QString& text, bool _busy = true);
	void SetText(const QString& text);
	void TemporaryAppendToMainText(const QString& text);
	void Stop(const QString& text = "", bool showtime = true, bool qwarn = false);
	void StopError(const QString& text = "", bool showtime = true);
	void SendTextMode(bool mode) { sendText = mode; };

private:
	bool sendText;
	bool busy;
	QTime Time;
	QString MainText;

signals:
	void	SendMessage(const QString& Message);
};


void  GetIntensityTabPos(int dx, int dy, int dz, int& ndx, int& ndy, int& ndz, int x, int y, int z, int nx, int ny, int nz)
{

	ndx = dx; ndy = dy; ndz = dz;
	if (x + dx < 0)	ndx = -2 * x - dx;	else if (x + dx > nx) ndx = 2 * (nx - x) - dx;
	if (y + dy < 0)	ndy = -2 * y - dy;	else if (y + dy > ny) ndy = 2 * (ny - y) - dy;
	if (z + dz < 0)	ndz = -2 * z - dz;	else if (z + dz > nz) ndz = 2 * (nz - z) - dz;

}

//------------------------------------------------------------
class InterpolationFunction
{

public:
	InterpolationFunction() {};

	inline virtual	double GetInterpolatedValue(RawDataSet* data, double X, double Y, double Z) = 0;
	inline virtual	double GetInterpolatedValue(unsigned short* tab, int nx, int ss, double X, double Y, double Z) = 0;

};
//-------------------------------
class InterpolationFunctionTrilinear : public InterpolationFunction
{
public:




	inline double GetInterpolatedValue(unsigned short* tab, int nx, int ss, double x, double y, double z)
	{
		return Interpolation3LinearDouble(tab, nx, ss, x, y, z);
	}

	inline double GetInterpolatedValue(RawDataSet* data, double X, double Y, double Z)
	{
		return Interpolation3LinearDouble(data->GetDataArray(), data->GetNx(), data->GetSliceSize(), X, Y, Z);

		//return Interpolation3Linear(data,X,Y,Z);
	};


};

//-----------------------------------
class InterpolationFunctionNearest : public InterpolationFunction
{
public:

	inline double GetInterpolatedValue(unsigned short* tab, int nx, int ss, double x, double y, double z)
	{
		return tab[(int)(x + 0.5) + nx * (int)(y + 0.5) + ss * (int)(z + 0.5)];
	}
	inline double GetInterpolatedValue(RawDataSet* data, double X, double Y, double Z)
	{

		return data->GetVoxelValue((int)(X + 0.5), (int)(Y + 0.5), (int)(Z + 0.5));
	};

};

class InterpolationFunction2DNearest : public InterpolationFunction
{
public:

	inline double GetInterpolatedValue(unsigned short* tab, int nx, int ss, double x, double y, double z)
	{
		return tab[(int)(x + 0.5) + (int)(y + 0.5) * nx];
	}
	inline double GetInterpolatedValue(RawDataSet* data, double X, double Y, double Z)
	{
		return data->GetDataArray()[(int)(X + 0.5) + (int)(Y + 0.5) * data->GetNx()];
	};

};


class InterpolationFunction2DBilinear : public InterpolationFunction
{
public:

	inline double GetInterpolatedValue(unsigned short* tab, int nx, int ss, double x, double y, double z)
	{
		int xx, yy, x11, x12, x21, x22;
		double y1, y2;
		double dx, dy;
		xx = (int)x;
		yy = (int)y;
		dx = x - xx;
		dy = y - yy;
		//int nx=nx;
		int pos = xx + yy * nx + z * ss;

		//unsigned short *tab=data->GetDataArray();

		x11 = tab[pos];
		x12 = tab[pos + 1];
		x21 = tab[pos + nx];
		x22 = tab[pos + nx + 1];

		y1 = x11 * (1.0 - dx) + x12 * dx;
		y2 = x21 * (1.0 - dx) + x22 * dx;
		return	y1 * (1.0 - dy) + y2 * dy;
	}

	inline double GetInterpolatedValue(RawDataSet* data, double X, double Y, double Z)
	{
		return GetInterpolatedValue(data->GetDataArray(), data->GetNx(), data->GetSliceSize(), X, Y, Z);


	};

	inline double GetInterpolatedValue(unsigned char* tab, int nx, int ss, double X, double Y, double Z)
	{
		int xx, yy, x11, x12, x21, x22;
		double y1, y2;
		double dx, dy;
		xx = (int)X;
		yy = (int)Y;
		dx = X - xx;
		dy = Y - yy;
		//int nx=data->GetNx();
		int pos = xx + yy * nx + Z * ss;

		//unsigned short *tab=data->GetDataArray();

		x11 = tab[pos];
		x12 = tab[pos + 1];
		x21 = tab[pos + nx];
		x22 = tab[pos + nx + 1];

		y1 = x11 * (1.0 - dx) + x12 * dx;
		y2 = x21 * (1.0 - dx) + x22 * dx;
		return	y1 * (1.0 - dy) + y2 * dy;

	};
};



double GetMaskCoinScale(maskClass* mc, double diameter);

//-----------------------------------------------------
/*	static  const __m128 CONST_1111 = _mm_set1_ps(1);
static const __m128 CONST_256 = _mm_set1_ps(256);
inline  __m128 CalcWeights(float x, float y)
{
__m128 ssx = _mm_set_ss(x);
__m128 ssy = _mm_set_ss(y);
__m128 psXY = _mm_unpacklo_ps(ssx, ssy);      // 0 0 y x

//__m128 psXYfloor = _mm_floor_ps(psXY); // use this line for if you have SSE4
__m128 psXYfloor = _mm_cvtepi32_ps(_mm_cvtps_epi32(psXY));
__m128 psXYfrac = _mm_sub_ps(psXY, psXYfloor); // = frac(psXY)

__m128 psXYfrac1 = _mm_sub_ps(CONST_1111, psXYfrac); // ? ? (1-y) (1-x)
__m128 w_x = _mm_unpacklo_ps(psXYfrac1, psXYfrac);   // ? ?     x (1-x)
w_x = _mm_movelh_ps(w_x, w_x);      // x (1-x) x (1-x)
__m128 w_y = _mm_shuffle_ps(psXYfrac1, psXYfrac, _MM_SHUFFLE(1, 1, 1, 1)); // y y (1-y) (1-y)

// complete weight vector
return _mm_mul_ps(w_x, w_y);
}
inline int GetPixelSSE3(const int* img,int width,  double x, double y)
{
const int stride = width;
const int* p0 = img + (int)x + (int)y * stride; // pointer to first pixel

// Load the data (2 pixels in one load)
__m128i p12 = _mm_loadl_epi64((const __m128i*)&p0[0 * stride]);
__m128i p34 = _mm_loadl_epi64((const __m128i*)&p0[1 * stride]);

__m128 weight = CalcWeights(x, y);

// convert RGBA RGBA RGBA RGAB to RRRR GGGG BBBB AAAA (AoS to SoA)
__m128i p1234 = _mm_unpacklo_epi8(p12, p34);
__m128i p34xx = _mm_unpackhi_epi64(p1234, _mm_setzero_si128());
__m128i p1234_8bit = _mm_unpacklo_epi8(p1234, p34xx);

// extend to 16bit
__m128i pRG = _mm_unpacklo_epi8(p1234_8bit, _mm_setzero_si128());
__m128i pBA = _mm_unpackhi_epi8(p1234_8bit, _mm_setzero_si128());

// convert weights to integer
weight = _mm_mul_ps(weight, CONST_256);
__m128i weighti = _mm_cvtps_epi32(weight); // w4 w3 w2 w1
weighti = _mm_packs_epi32(weighti, weighti); // 32->2x16bit

//outRG = [w1*R1 + w2*R2 | w3*R3 + w4*R4 | w1*G1 + w2*G2 | w3*G3 + w4*G4]
__m128i outRG = _mm_madd_epi16(pRG, weighti);
//outBA = [w1*B1 + w2*B2 | w3*B3 + w4*B4 | w1*A1 + w2*A2 | w3*A3 + w4*A4]
__m128i outBA = _mm_madd_epi16(pBA, weighti);

// horizontal add that will produce the output values (in 32bit)
__m128i out = _mm_hadd_epi32(outRG, outBA);
out = _mm_srli_epi32(out, 8); // divide by 256

// convert 32bit->8bit
out = _mm_packus_epi32(out, _mm_setzero_si128());
out = _mm_packus_epi16(out, _mm_setzero_si128());

// return
return _mm_cvtsi128_si32(out);
}*/
//---------------------------------------
double	Real48ToDouble(unsigned char realValue[6]);


void	ColorizeIslandsOnUnsignedShortMask(unsigned short* tab, int ww, int hh, int inputVal, double* params, int* listS, int* listTmp, unsigned int* tabxCopy);
void	GetSeparatedMasks(LinkedList <maskClass*>* maskList, unsigned short* tab, int ww, int hh, int inputVal, double* params, int* listS, int* listTmp, unsigned int* tabxCopy, bool fitEllipse, QImage* img, QImage* origImg = NULL);
QPolygonF	ImproveSeparateMasks(unsigned short* tab, int ww, int hh, int inputVal, double* params, int* listS, int* listTmp, QImage* img, bool test = false);
//
int		MaskListConturExtraction(int* listTmp, int totalCount, int* params, int* dilatePoints);
void	FittEllipsetoBoundaryPoints(int nrOfDilatePoints, int* dilatePoints, int* params, double* outParams, QImage* img);

void  DivideBlobUsingAcuteAngleCriterion(maskClass* inputBlob, QPolygonF* lines, int morphologyKernelType, int morphologyType, int* listS, int* listTmp, int blobCount, bool test = false);
//
void	ColorizeIslandsOnUnsignedShortMask(unsigned short* tab, int ww, int hh, int inputVal, double* params, int* listS, int* listTmp, unsigned int* tabxCopy);
void	GetSeparatedMasks(LinkedList <maskClass*>* maskList, unsigned short* tab, int ww, int hh, int inputVal, double* params, int* listS, int* listTmp, unsigned int* tabxCopy, bool fitEllipse, QImage* img);
int		MaskListConturExtraction(int* listTmp, int totalCount, int* params, int* dilatePoints);
void FittEllipsetoBoundaryPoints(int nrOfDilatePoints, int* dilatePoints, int* params, double* outParams, QImage* img);
double evalSpline(double x, double* xs, double* ys, double* ks);
void  gaussJswapRows(double** m, int k, int l);
void gaussJsolve(double** A, double* x, int size);
void getNaturalKs(double* xs, double* ys, double* ks, int size);



void TabPeakDetector(double* tab, int size, bool useThreshold, double cutThreshold, bool detectMin, bool detectMax, bool checkends, int neighCheckSize, QVector <int>& maxPosVector);

void AngularPeakDetecotr(double* tab, int size, int neinghSize, int step, double angleNeighThresh, QVector <int>& maxPosVector);

class CardinalSpline
{
public:
	CardinalSpline(double* dp, int _size, int leftConstraint = 1, int rightConstraint = 1);
	~CardinalSpline();


	double Evaluate(double t);
private:

	void Compute();
	double* dataPointer;//pary x/y; - rozmiar 2xsize
	int size;

	//bool computed;
	int ClampValue;
	double* Intervals;
	double* Coefficients;
	int LeftConstraint;
	double LeftValue;
	int RightConstraint;
	double RightValue;
	double ParametricRange[2];

	// Helper methods
	double ComputeLeftDerivative();
	double ComputeRightDerivative();
	int FindIndex(int size, double t);
	void Fit1D(int n, double* x, double* y, double* w, double coefficients[][4],
		int leftConstraint, double leftValue, int rightConstraint,
		double rightValue);

};



class ThicknessLineItem
{

public:
	ThicknessLineItem() { treeItem = NULL; };
	~ThicknessLineItem() { if (treeItem != NULL) delete treeItem; };
	QString name;
	QPolygonF poly;
	QTreeWidgetItem* treeItem;

};




QPolygonF ConvertContourPointsToPolygon(maskClass* inputBlob, int kernelType, int* listS, int nrOfPoints, int blobCount, bool test = false);
void AngularPeakDetecotr(double* tab, int size, int neinghSize, int step, double angleThresh, double angleNeighThresh, int neighSize, QVector <int>& maxPosVector);



QString GetExcelColumnName(int columnNumber);

#endif //OTHER_COMMON_FUNCTIONS
