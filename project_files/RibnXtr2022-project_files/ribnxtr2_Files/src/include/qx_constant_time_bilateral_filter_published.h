/*************************************************************************************************
\Author:	Qingxiong Yang
\Function:	O(1) bilateral filtering (for grayscale images)
\reference:	Qingxiong Yang, Kar-Han Tan and Narendra Ahuja, Real-time O(1) Bilateral Filtering, 
			IEEE Conference on Computer Vision and Pattern Recognition (CVPR) 2009.
**************************************************************************************************/
#ifndef QX_CONSTANT_TIME_BILATERAL_FILTER_PUBLISHED_H
#define QX_CONSTANT_TIME_BILATERAL_FILTER_PUBLISHED_H
class qx_constant_time_bilateral_filter_published
{
public:
    qx_constant_time_bilateral_filter_published();
    ~qx_constant_time_bilateral_filter_published();
    void clean();
	int init(int h,int w,
		int nr_layer=8
		//nr_layer = 8 should be enough. The average PSNR value is over 52 dB on the test images selected by 
		//Sylvain Paris and Frédo Durand (http://people.csail.mit.edu/sparis/code/data/color_ppm.zip)
		//when sigma_spatial = 0.03 (x height/width) and sigma_range = 0.1 (x 255).
		);
	int filter(double**image_filtered,double**image,double sigma_spatial,double sigma_range,double**texture=NULL);
	//1: sigma_spatial and sigma_range in (0,1);
	//2: if (texture != NULL) it is a joint bilateral filter guided by texture
private:
	char m_str[300];
	int m_h,m_w,m_nr_layer;
	double***m_jk,***m_wk,*m_jk_d1,*m_wk_d1;
	double*m_grayscale;
	double m_table[QX_DEF_CHAR_MAX+1];
private:
	void update_range_table(double sigma_range);
};
#endif