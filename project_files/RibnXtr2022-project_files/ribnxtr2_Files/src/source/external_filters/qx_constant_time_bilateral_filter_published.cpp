/*$Id: qx_image_filter_color_weighted_gpu.cpp,v 1.1 2007/01/03 22:04:32 liiton Exp $*/
#include "qx_basic.h"
#include "qx_constant_time_bilateral_filter_published.h"
qx_constant_time_bilateral_filter_published::qx_constant_time_bilateral_filter_published()
{
	m_jk=NULL;
	m_wk=NULL;
	m_jk_d1=NULL;
	m_wk_d1=NULL;
	m_grayscale=NULL;
}
qx_constant_time_bilateral_filter_published::~qx_constant_time_bilateral_filter_published()
{
	clean();
}
void qx_constant_time_bilateral_filter_published::clean()
{
	qx_freed_3(m_jk); m_jk=NULL;
	qx_freed_3(m_wk); m_wk=NULL;
	qx_freed_1(m_jk_d1); m_jk_d1=NULL;
	qx_freed_1(m_wk_d1); m_wk_d1=NULL;
	qx_freed_1(m_grayscale); m_grayscale=NULL;
}
int qx_constant_time_bilateral_filter_published::init(int h,int w,int nr_layer)
{
	clean();
	m_h=h; m_w=w; m_nr_layer=nr_layer;
	/*memory allocation*/
	m_jk=qx_allocd_3(m_h,m_w,m_nr_layer);
	m_wk=qx_allocd_3(m_h,m_w,m_nr_layer);
	m_jk_d1=new double [m_nr_layer];
	m_wk_d1=new double [m_nr_layer];
	m_grayscale=new double [m_nr_layer];
	return(0);
}
void qx_constant_time_bilateral_filter_published::update_range_table(double sigma_range)
{
	for(int i=0;i<=QX_DEF_CHAR_MAX;i++) m_table[i]=exp(-(0.5*i*i)/(sigma_range*sigma_range*QX_DEF_CHAR_MAX*QX_DEF_CHAR_MAX));
}
int qx_constant_time_bilateral_filter_published::filter(double**image_filtered,double**image,double sigma_spatial,double sigma_range,double**texture)
{
	if(texture==NULL) texture=image;
	int rx=qx_round(sigma_spatial*m_w);
	int ry=qx_round(sigma_spatial*m_h);
	if(rx>(m_w>>1))
	{
		printf("qx_constant_time_bilateral_filter_published::filter: sigma = [%1.3f] is too large for this implementation!!\n",sigma_spatial);
		memset(image_filtered[0],0,sizeof(double)*m_h*m_w);
		getchar();
		return(-1);
	}
	update_range_table(sigma_range);
	double image_min,image_max; 
	if(texture)
	{
		vec_min_val(image_min,texture[0],m_h*m_w);
		vec_max_val(image_max,texture[0],m_h*m_w);
	}
	m_grayscale[0]=(double)image_min;
	m_grayscale[m_nr_layer-1]=(double)image_max;
	double delta_scale=qx_div(image_max-image_min,double(m_nr_layer-1));
	double inv_delta_scale=qx_div(double(m_nr_layer-1),image_max-image_min);
	for(int i=1;i<m_nr_layer-1;i++) m_grayscale[i]=(double)image_min+delta_scale*i;


	//cost computation and aggregation (using integral image)
	double*image_=image[0];
	double*texture_=texture[0];
	double*wk_=m_wk[0][0];
	double*jk_=m_jk[0][0];
		double image_yx=*image_++;
		double texture_yx=*texture_++;
		for(int d=0;d<m_nr_layer;d++) 
		{
			double weight=m_table[qx_round(abs(m_grayscale[d]-texture_yx))];
			*wk_++=weight;
			*jk_++=weight*image_yx;
		}
	double*wk_p=m_wk[0][0];
	double*jk_p=m_jk[0][0];
	for(int x=1;x<m_w;x++) //when y=0 & x=[1,m_w]
	{
		image_yx=*image_++;
		texture_yx=*texture_++;
		for(int d=0;d<m_nr_layer;d++) 
		{
			double weight=m_table[qx_round(abs(m_grayscale[d]-texture_yx))];
			*wk_++=weight+(*wk_p++);
			*jk_++=weight*image_yx+(*jk_p++);
		}
	}
	for(int y=1;y<m_h;y++) //when x=0 & y=[1,m_h]
	{
		image_yx=image[y][0];
		texture_yx=texture[y][0];
		wk_=m_wk[y][0];
		wk_p=m_wk[y-1][0];
		jk_=m_jk[y][0];
		jk_p=m_jk[y-1][0];
		for(int d=0;d<m_nr_layer;d++) 
		{
			double weight=m_table[qx_round(abs(m_grayscale[d]-texture_yx))];
			*wk_++=weight+(*wk_p++);
			*jk_++=weight*image_yx+(*jk_p++);
		}
	}
	double*wk_A=m_wk[0][0];//[y-1][x-1]
	double*wk_B=m_wk[0][1];//[y-1][x]
	double*wk_D=m_wk[1][0];//[y][x-1]
	wk_=m_wk[1][1];//[y][x];
	double*jk_A=m_jk[0][0];//[y-1][x-1]
	double*jk_B=m_jk[0][1];//[y-1][x]
	double*jk_D=m_jk[1][0];//[y][x-1]
	jk_=m_jk[1][1];//[y][x];

	image_=&(image[1][0]);
	texture_=&(texture[1][0]);
	for(int y=1;y<m_h;y++)//when y>0 & x>0
	{
		*image_++;
		*texture_++;
		for(int x=1;x<m_w;x++)
		{
			image_yx=*image_++;
			texture_yx=*texture_++;
			for(int d=0;d<m_nr_layer;d++)
			{
				double weight=m_table[qx_round(abs(m_grayscale[d]-texture_yx))];
				*wk_++=weight+(*wk_B++)+(*wk_D++)-(*wk_A++);
				*jk_++=weight*image_yx+(*jk_B++)+(*jk_D++)-(*jk_A++);
			}
		}
		for(int d=0;d<m_nr_layer;d++) {*wk_++; *wk_B++; *wk_D++;*wk_A++; *jk_++; *jk_B++; *jk_D++;*jk_A++;}
	}

	//cost computation and aggregation (using integral image)
	texture_=texture[0];
	double*image_filtered_=image_filtered[0];
	for(int y=0;y<m_h;y++)
	{
		for(int x=0;x<m_w;x++)
		{
			double*jk_d1=m_jk_d1;
			double*wk_d1=m_wk_d1;
			int x0=x-rx-1;
			int xt=min(x+rx,m_w-1);
			int y0=y-ry-1;
			int yt=min(y+ry,m_h-1);
			if(x0<0&&y0<0) 
			{
				double*jk_tt=m_jk[yt][xt];
				double*wk_tt=m_wk[yt][xt];
				for(int d=0;d<m_nr_layer;d++) 
				{
					(*jk_d1++)=*jk_tt++;
					(*wk_d1++)=*wk_tt++;
				}
			}
			else if(x0<0) 
			{
				double*jk_0t=m_jk[y0][xt];
				double*jk_tt=m_jk[yt][xt];
				double*wk_0t=m_wk[y0][xt];
				double*wk_tt=m_wk[yt][xt];
				for(int d=0;d<m_nr_layer;d++) 
				{
					(*jk_d1++)=(*jk_tt++)-(*jk_0t++);
					(*wk_d1++)=(*wk_tt++)-(*wk_0t++);
				}
			}
			else if(y0<0) 
			{
				double*jk_t0=m_jk[yt][x0];
				double*jk_tt=m_jk[yt][xt];
				double*wk_t0=m_wk[yt][x0];
				double*wk_tt=m_wk[yt][xt];
				for(int d=0;d<m_nr_layer;d++)
				{
					(*jk_d1++)=(*jk_tt++)-(*jk_t0++);
					(*wk_d1++)=(*wk_tt++)-(*wk_t0++);
				}
			}
			else 
			{
				double*jk_00=m_jk[y0][x0];
				double*jk_0t=m_jk[y0][xt];
				double*jk_t0=m_jk[yt][x0];
				double*jk_tt=m_jk[yt][xt];
				double*wk_00=m_wk[y0][x0];
				double*wk_0t=m_wk[y0][xt];
				double*wk_t0=m_wk[yt][x0];
				double*wk_tt=m_wk[yt][xt];
				for(int d=0;d<m_nr_layer;d++) 
				{
					(*jk_d1++)=(*jk_tt++)+(*jk_00++)-(*jk_t0++)-(*jk_0t++);
					(*wk_d1++)=(*wk_tt++)+(*wk_00++)-(*wk_t0++)-(*wk_0t++);
				}
			}
			//double d=depth_sub_pixel(m_temp_d1,m_nr_layer); 
			//image_filtered[y][x]=d*inv_scalar+val_min;
			double kf=((*texture_++)-image_min)*inv_delta_scale; /*cross/joint bilateral filtering*/
			int k=int(kf);
			if(k==(m_nr_layer-1)) *image_filtered_++=qx_div(m_jk_d1[k],m_wk_d1[k]);
			else
			{
				double alpha=(k+1)-kf;
				*image_filtered_++=alpha*qx_div(m_jk_d1[k],m_wk_d1[k])+(1.0-alpha)*qx_div(m_jk_d1[k+1],m_wk_d1[k+1]);
			}
		}
	}
	return(0);
}

