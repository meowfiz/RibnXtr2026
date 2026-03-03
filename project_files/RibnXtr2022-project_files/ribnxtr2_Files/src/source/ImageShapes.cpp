#include "ImageShapes.h"
MyShape::MyShape()
{
	textToLogFlag = false;
	representation = 1; // 0 - pts, 1 - polyline, 2 - polygon 
	polyColor.setRgb(255, 255, 255);
	ellipsesColor.setRgb(255, 255, 255);
	textColor.setRgb(255, 0, 0);
	textPos.setX(0);
	textPos.setY(0);
	shapeText = QString("");
	textAngle = 0;
	textAutoPos = -1;
	opacity = 0.0;
	font.setStyleHint(QFont::SansSerif);
	font.setStyle(QFont::StyleItalic);
	visible = true;
	cleanShape = false;

}
ImageShapes::ImageShapes()
{
	clickedLeftPoint.setX(0);
	clickedLeftPoint.setY(0);

	clickedRightPoint.setX(0);
	clickedRightPoint.setY(0);

	dx = 0;
	dy = 0;
	cleanPolygonFlag = true;
	mask = NULL;
	maskSize = 0;

	scale = 1.0;

	thresholds[0] = 0;
	thresholds[1] = 0;

}
//-------------------------------------------------------------
ImageShapes::~ImageShapes()
{
	if (mask != NULL) delete[]mask;
}