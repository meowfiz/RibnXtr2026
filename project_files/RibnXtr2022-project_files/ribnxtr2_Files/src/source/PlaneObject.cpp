#include "PlaneObject.h"



PlaneObject::PlaneObject()
{
	norm=0;
	xa=0;
	ya=0;
	za=0;
	wa=0;
}

PlaneObject::~PlaneObject()
{

}
//--------------------------------------------------
PlaneObject::PlaneObject(const PlaneObject &planeo)
{
	for (int i = 0; i < 3; i++)
	{
		p00[i] = planeo.p00[i];
		p01[i] = planeo.p01[i];
		p02[i] = planeo.p02[i];

	}
	SetPlanePoints(p01, p00, p02);
}
//-------------------------------------------------

double	PlaneObject::SetPlanePoints(double p1[3],double p0[3],double p2[3])
{
	for (int i = 0; i < 3; i++)
	{
		p00[i] = p0[i];
		p01[i] = p1[i];
		p02[i] = p2[i];

	}
	 
	double angle, lcross,dot;

	v1[0]=p1[0]-p0[0]; //
	v1[1]=p1[1]-p0[1]; //
	v1[2]=p1[2]-p0[2]; //

	v2[0]=p2[0]-p0[0]; //
	v2[1]=p2[1]-p0[1]; //
	v2[2]=p2[2]-p0[2]; //

	CROSS(cross,v1,v2);
	lcross=DOT(cross,cross);
	if (lcross>0) lcross=sqrt(lcross);
	dot=DOT(v1,v2);	
	n1[0]=0;n1[1]=0;n1[2]=1;
	if (DOT(cross, n1) < 0)
		lcross = -lcross;
	angle=atan2(lcross,dot);
//////////////
	xa=cross[0];
	ya=cross[1];
	za=cross[2];
	wa=-(xa*p0[0]+ya*p0[1]+za*p0[2]);
	norm=sqrt(xa*xa+ya*ya+za*za);
	if (norm!=0) norm=1.0/norm;
///////////////
	return 	fabs(57.295779513*angle);
	//zwraca w stopniach

////---------------------------------------------
/*	p0_[0]=p0[0];
	p0_[1]=p0[1];
	p0_[2]=p0[2];

	x1x0=p1[0]-p0[0];
	y1y0=p1[1]-p0[1];
	z1z0=p1[2]-p0[2];

	y2y0=p2[1]-p0[1];
	x2x0=p2[0]-p0[0];
	z2z0=p2[2]-p0[2];
	xa=y1y0*z2z0-y2y0*z1z0;
	ya=z1z0*x2x0-z2z0*x1x0;
	za=x1x0*y2y0-x2x0*y1y0;

	wa=-(xa*p0_[0]+ya*p0_[1]+za*p0_[2]);

	norm=sqrt(xa*xa+ya*ya+za*za);

	norm=(norm!=0)?1.0/norm:0;
	double len1=sqrt(x1x0*x1x0+y1y0*y1y0+z1z0*z1z0);
	double len2=sqrt(x2x0*x2x0+y2y0*y2y0+z2z0*z2z0);

	double scalar= x1x0*x2x0+y1y0*y2y0+z1z0*z2z0;

	double m=len1*len2;
	if (m!=0)
	{
		return fabs(90-deg*fabs(acos(scalar/m)));
	}
	else 
		return 0;


*/
}


//-------------------------------------------------


double	PlaneObject::CheckPoint(double Point[3])
{

//	xx0=Point[0]-p0_[0];
//	yy0=Point[1]-p0_[1];
//	zz0=Point[2]-p0_[2];

/*	FILE *Do_opa;
	Do_opa=fopen("c:\\Do_opa1.txt","a");
	fprintf(Do_opa,"Index %lf %lf %lf\n",Point[0]-p0_[0],Point[1]-p0_[1],Point[2]-p0_[2]);
	fclose(Do_opa);
*/

// rownanie plaszczyzny:

	
	return (norm!=0)?(xa*Point[0]+ya*Point[1]+za*Point[2]+wa)*norm:0;

	//return xx0*y1y0*z2z0+x1x0*y2y0*zz0+yy0*z1z0*x2x0
	//		-yy0*x1x0*z2z0-zz0*y1y0*x2x0-z1z0*y2y0*xx0;



}
