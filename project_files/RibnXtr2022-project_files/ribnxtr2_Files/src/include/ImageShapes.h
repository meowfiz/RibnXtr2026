#ifndef	IMAGE_SHAPES
#define	IMAGE_SHAPES

#include	<qpainter.h>
#include	<qlist.h>
#include	<qstring.h>
#include	<qfont.h>
//------------------
class MyShape
{
public:

	MyShape();

	QString			name;
	QPolygonF		poly;
	int				representation; // 0 - pts, 1 - polyline, 2 - polygon, 3 - separated lines
	QColor			polyColor;

	QString			shapeText;
	QColor			textColor;
	bool			textToLogFlag;
	QPointF			textPos;
	int				textAutoPos;
	QRectF			objectBounds;
	double			opacity;

	QFont			font;
	double			textAngle;
	QPolygonF		ellipses;
	QColor			ellipsesColor;
	bool			visible;
	bool			cleanShape;

};

class ImageShapes
{
public:
	~ImageShapes();
	ImageShapes();
	QList <MyShape> shapeList;

	QPointF			clickedLeftPoint;
	QPointF			clickedRightPoint;

	QList<QPolygonF>		polygonList;
	QList <int> polygonFlags;
	unsigned char *mask;
	int maskSize;
	bool			cleanPolygonFlag;


	//magnifying glass

	double scale;
	int xScalePos, yScalePos;

	//QPolygonF		polygon1;
	//QPolygonF		polygon2;

	// dymczasowe:
	int thresholds[2];

	double dx, dy;


};



#endif //ImageShapes
