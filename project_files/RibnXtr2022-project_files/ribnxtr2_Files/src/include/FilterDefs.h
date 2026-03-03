#ifndef FILTERDEFS_H
#define	FILTERDEFS_H

#include <qstring.h>

#define SIZE_OF_FILTER_TAB 585+11
#define NUMBER_OF_FILTERS 52

struct FilterDefs
{
	 int data[SIZE_OF_FILTER_TAB] = {

	159,5,

	2,4,5,4,2,
	4,9,12,9,4,
	5,12,15,12,
	5,4,9,12,9,
	4,2,4,5,4,2,

	1,3,

	-1,0,1,
	-1,1,1,
	-1,0,1,

	1,3,

	1,1,1,
	0,1,0,
	-1,-1,-1,

	1,3,

	0,1,1,
	-1,1,1,
	-1,-1,0,

	1,3,

	1,1,0,
	1,1,-1,
	0,-1,-1,

	1,3,

	-1,-1,-1,
	0,1,0,
	1,1,1,


	1,3,

	-1,-1,0,-1,1,1,0,1,1,

	1,3,

	1,1,0,1,1,-1,0,-1,-1,

	1,3,

	1,0,-1,1,1,-1,1,0,-1,

	1,3,

	0,0,0,-1,0,1,0,0,0,

	1,3,

	0,1,0,0,0,0,0,-1,0,

	1,3,

	1,0,0,0,0,0,0,0,-1,

	1,3,

	0,0,1,0,0,0,-1,0,0,

	1,3,

	0,1,0,0,0,0,0,-1,0,

	1,3,

	-1,0,0,0,0,0,0,0,1,

	1,3,

	0,0,-1,0,0,0,1,0,0,

	1,3,

	0,0,0,1,0,-1,0,0,0,

	1,3,

	-1,1,1,-1,-2,1,-1,1,1,

	1,3,

	1,1,1,1,-2,1,-1,-1,-1,

	1,3,

	1,1,1,-1,-2,1,-1,-1,1,

	1,3,

	1,1,1,1,-2,-1,1,-1,-1,

	1,3,

	-1,-1,-1,1,-2,1,1,1,1,

	1,3,

	-1,-1,1,-1,-2,1,1,1,1,

	1,3,

	1,-1,-1,1,-2,-1,1,1,1,

	1,3,

	1,1,-1,1,-2,-1,1,1,-1,

	1,3,

	-1,-1,-1,-1,9,-1,-1,-1,-1,

	1,3,

	0,-1,0,-1,5,-1,0,-1,0,

	1,3,

	1,-2,1,-2,5,-2,1,-2,1,

	16,3,

	0,-1,0,-1,20,-1,0,-1,0,

	1,3,

	0,-1,0,-1,4,-1,0,-1,0,

	1,3,

	-1,-1,-1,-1,8,-1,-1,-1,-1,

	1,3,

	1,-2,1,-2,4,-2,1,-2,1,

	1,3,

	-1,0,-1,0,4,0,-1,0,-1,

	1,3,

	0,-1,0,0,2,0,0,-1,0,

	1,3,

	0,0,0,-1,2,-1,0,0,0,

	9,3,

	1,1,1,1,1,1,1,1,1,

	16,3,

	1,2,1,2,4,2,1,2,1,

	10,3,

	1,1,1,1,2,1,1,1,1,

	12,3,

	1,1,1,1,4,1,1,1,1,

	20,3,

	1,1,1,1,12,1,1,1,1,

	1,3,

	-1,-1,-1,0,0,0,1,1,1,

	1,3,

	1,0,-1,1,0,-1,1,0,-1,

	1,3,

	0,-1,0,
	0,1,0,
	0,0,0,

	1,3,

	-1,0,0,
	0,1,0,
	0,0,0,

	1,3,

	0,0,0,
	-1,1,0,
	0,0,0,

	1,3,

	1,2,1,
	0,0,0,
	-1,-2,-1,

	1,3,

	1,0,-1,
	2,0,-2,
	1,0,-1,

	1,1,

	1,

	19,5,

	0,1,0,0,0,
	1,1,1,0,0,
	0,1,0,2,3,
	1,1,1,0,0,
	0,1,0,2,3,

	1,3,

	1,0,1,
	0,-4,0,
	1,0,1,

	1,3,

	1,4,1,
	4,-20,4,
	1,4,1,

	1,3,

	0,0,0,
	0,1,0,
	0,0,0

	};

	/*
	1,5,


	0,0,0,0,0,
	0,0,0,0,0,
	0,0,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0
	*/
	/*


	  1,9,

	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,

	  */


	QString Filter2DNameDefs[NUMBER_OF_FILTERS] = {
	"Gaussian_function_with_sigma=1.4",
	"Contour_Filter_(Embossing_East)",
	"Contour_Filter_(Embossing_North)",
	"Contour_Filter_(Embossing_North-East)",
	"Contour_Filter_(Embossing_North-West)",
	"Contour_Filter_(Embossing_South)",
	"Contour_Filter_(Embosing_Southeast)",
	"Contour_Filter_(Embosing_Southwest)",
	"Contour_Filter_(Embosing_West)",
	"Contour_Filter_2_(Embossing_-_East)",
	"Contour_Filter_2_(Embossing_-_North)",
	"Contour_Filter_2_(Embossing_-_Northwest)",
	"Contour_Filter_2_(Embossing_-_Northeast)",
	"Contour_Filter_2_(Embossing_-_South)",
	"Contour_Filter_2_(Embossing_-_Southeast)",
	"Contour_Filter_2_(Embossing_-_Southwest)",
	"Contour_Filter_2_(Embossing_-_West)",
	"Gradient_Directional_Edge_Enhancement_-_East",
	"Gradient_Directional_Edge_Enhancement_-_North",
	"Gradient_Directional_Edge_Enhancement_-_Northeast",
	"Gradient_Directional_Edge_Enhancement_-_Northwest",
	"Gradient_Directional_Edge_Enhancement_-_South",
	"Gradient_Directional_Edge_Enhancement_-_Southeast",
	"Gradient_Directional_Edge_Enhancement_-_Southwest",
	"Gradient_Directional_Edge_Enhancement_-_West",
	"High_Pass_Spatial_Filter_-_Local_Mean_Removal",
	"High_Pass_Spatial_Filter_1",
	"High_Pass_Spatial_Filter_2",
	"High_Pass_Spatial_Filter_3",
	"Laplacian_Edge_Enhancement_1",
	"Laplacian_Edge_Enhancement_2",
	"Laplacian_Edge_Enhancement_3",
	"Diagonal_Laplacian_Edge_Enhancement",
	"Horizontal_Laplacian_Edge_Enhancement",
	"Vertical_Laplacian_Edge_Enhancement",
	"Low_Pass_Spatial_Average_Filter",
	"Low_Pass_Spatial_'Gaussian'_Filter",
	"Low_Pass_Spatial_Filter_1",
	"Low_Pass_Spatial_Filter_2",
	"Low_Pass_Spatial_Filter_3",
	"Prewitt_Horizontal_Edge_Enhancements_Filter",
	"Prewitt_Vertical_Edge_Enhancements_Filter",
	"Horizontal_Shift_and_Difference_Edge_Enhancement",
	"Vertical_and_Horizontal_Shift&Diff_Edge_Enhanceme",
	"Vertical_Shift_and_Difference_Edge_Enhancement",
	"Sobel_Horizontal_Edge_Enhancements_Filter",
	"Sobel_Vertical_Edge_Enhancements_Filter",
	"Unity_Spatial_Filter_(no_operation)",
	"Smooth",
	"Discrete1",
	"Discrete2",
	};


};

 //51
 //585


#endif // FILTERDEFS_H
