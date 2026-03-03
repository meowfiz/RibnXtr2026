#ifndef PLANEX_H
#define PLANEX_H

#include <stdio.h>
#include <math.h>

#include "OtherCommonFunctions.h"

class PlaneX
{
	
	public:
	
		PlaneX();
		~PlaneX(void);
	
	double 	x1x0,x2x0,y1y0,y2y0,z1z0,z2z0,xx0,yy0,zz0;
	double p0_[3];

	double	SetPlanePoints(double p0[3],double p1[3],double p2[3]);
	double	CheckPoint(double Point[3]);

	double xa,ya,za,wa;
	double norm;


};



#endif //PLANEX_H
