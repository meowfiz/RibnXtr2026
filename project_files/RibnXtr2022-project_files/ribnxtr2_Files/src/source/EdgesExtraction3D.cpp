#include "EdgesExtraction3D.h"

EdgesExtraction3D::EdgesExtraction3D(RawDataSet *_rawData)
{
	rawData=_rawData;
	_EPSILON_DERIVATIVE_=0.95;
	_EPSILON_NORM_=0.5;
}
//------------------------------------------
EdgesExtraction3D::~EdgesExtraction3D()
{
	rawData=NULL;
}
//-------------------------------------------
RawDataSet	*EdgesExtraction3D::ApplyCanny3DEdgesExtraction(double radius,double stddev,int dim)
{

	if ((rawData!=NULL))	
	{
		//dim=2 - 2d
		//dim=3 - 3d
		double		*tabDerivatedInZ=NULL;
		RawDataSet	*smoothedRawDataInZ=NULL;
		unsigned short *Q=NULL;
		int sliceDims[3];

		int nx=rawData->GetNx();
		int ny=rawData->GetNy();
		int nz=rawData->GetNz();
		sliceDims[0]=nx;sliceDims[1]=ny;sliceDims[2]=1;
		
		int sliceSize=rawData->GetSliceSize();
		int volumeSize=rawData->GetVolumeSize();
		int i,j,k;
		int pos;

		int					iRadius=Round(radius);
		int					size2=2*iRadius+1;
		double				*gaussianKernelTab=new double [size2];
		double				*gaussian1stDerivKernelTab=new double [size2];

		unsigned short		*intensityTab=new unsigned short[size2];

		GetGaussianKernel(gaussianKernelTab,(double)iRadius,stddev);
		GetGaussianKernelFirstDerivative(gaussian1stDerivKernelTab,(double)iRadius,stddev);

		// stworze zbior danych tablice typu  double z najpierw przefiltrowanymi po x i y gaussem
		// a potem pierwsza pochodna w osi z
		//---------------------------------------
		RawDataSet *tmpRawData=new RawDataSet();
		tmpRawData->Copy(*rawData,false);

		RawDataSet *tmpRawData2=new RawDataSet();
		tmpRawData2->Copy(*rawData,false);

		Q=tmpRawData->GetDataArray();
		pos=0;
		for(k=0;k<nz;k++)
			for(j=0;j<ny;j++)
				for(i=0;i<nx;i++)
				{
					GetIntensityTab1D(rawData,i,j,k,0,intensityTab,iRadius);//filtering in Z direction
					Q[pos++]=Apply1DNormFilter(gaussianKernelTab,intensityTab,size2);		
				}

		Q=tmpRawData2->GetDataArray();
		pos=0;
		for(k=0;k<nz;k++)
			for(j=0;j<ny;j++)
				for(i=0;i<nx;i++)
				{
					GetIntensityTab1D(tmpRawData,i,j,k,1,intensityTab,iRadius);//filtering in Y direction
					Q[pos++]=Apply1DNormFilter(gaussianKernelTab,intensityTab,size2);		
				}
		//return tmpRawData2;
		delete tmpRawData;
		Q=NULL;
		// zostal 1 wygladzony w obu osiach i teraz trzeba zrobic tablice doubleow z pochodnymi

		tabDerivatedInZ=new double[volumeSize];

		// z derivation
		
		smoothedRawDataInZ=new RawDataSet();
		smoothedRawDataInZ->Copy(*rawData,false);

		Q=smoothedRawDataInZ->GetDataArray();
		pos=0;
		if (dim==3)
		{
			for(k=0;k<nz;k++)
				for(j=0;j<ny;j++)
					for(i=0;i<nx;i++)
					{
						GetIntensityTab1D(tmpRawData2,i,j,k,2,intensityTab,iRadius);//filtering in Z direction
						tabDerivatedInZ[pos]=Apply1DNormFilter(gaussian1stDerivKernelTab,intensityTab,size2);
						Q[pos++]=Apply1DNormFilter(gaussianKernelTab,intensityTab,size2);
					}
		}
		else
		{
			for(k=0;k<nz;k++)
				for(j=0;j<ny;j++)
					for(i=0;i<nx;i++)
					{
						GetIntensityTab1D(tmpRawData2,i,j,k,2,intensityTab,iRadius);//filtering in Z direction
						tabDerivatedInZ[pos]=0;//Apply1DNormFilter(gaussian1stDerivKernelTab,intensityTab,size2);
						Q[pos]=tmpRawData2->GetDataArray()[pos];//Apply1DNormFilter(gaussianKernelTab,intensityTab,size2);
						pos++;
					
					}
		}
	//	return tmpRawData2;
		delete tmpRawData2;
		//----------------------------------------------------
		Q=NULL;

		//-----------------------------------------------------------------
		double *doubleTab=new double[7*sliceSize];
		memset(doubleTab,0,7*sliceSize*sizeof(double));
		double *gx[2],*gy[2],*norm[3];
		double *tmp;
		double *gz;
		unsigned short *sz;

		gx[0]=&doubleTab[0];
		gx[1]=&doubleTab[sliceSize];
		gy[0]=&doubleTab[2*sliceSize];
		gy[1]=&doubleTab[3*sliceSize];
		norm[0]=&doubleTab[4*sliceSize];
		norm[1]=&doubleTab[5*sliceSize];
		norm[2]=&doubleTab[6*sliceSize];
		gz=tabDerivatedInZ;
		sz=smoothedRawDataInZ->GetDataArray();
		//-------------------------------------------------
		unsigned short	*tmpBuffUShort=new unsigned short[size2];

		GetGradient2DTab(gx[0],sz,gaussian1stDerivKernelTab,tmpBuffUShort,iRadius,0,nx,ny,nz,0);
		GetGradient2DTab(gy[0],sz,gaussian1stDerivKernelTab,tmpBuffUShort,iRadius,1,nx,ny,nz,0);
		Get3DModulus(norm[1], gx[0],gy[0],&gz[0],sliceSize);
		//to czego nie mam 2d
		Remove_Gradient_NonMaxima_Slice_2D(&gz[0],gx[0],gy[0],norm[1],sliceDims );

		GetGradient2DTab(gx[1],sz,gaussian1stDerivKernelTab,tmpBuffUShort,iRadius,0,nx,ny,nz,1);
		GetGradient2DTab(gy[1],sz,gaussian1stDerivKernelTab,tmpBuffUShort,iRadius,1,nx,ny,nz,1);
		Get3DModulus(norm[2], gx[1],gy[1],&gz[sliceSize],sliceSize);

		int tmppos=sliceSize;
		for(k=1;k<nz-1;k++)
		{
			tmppos+=sliceSize;
			tmp=gx[0];gx[0]=gx[1];gx[1]=tmp;
			tmp=gy[0];gy[0]=gy[1];gy[1]=tmp;
			tmp=norm[0];norm[0]=norm[1];norm[1]=norm[2];norm[2]=tmp;

			GetGradient2DTab(gx[1],sz,gaussian1stDerivKernelTab,tmpBuffUShort,iRadius,0,nx,ny,nz,k+1);
			GetGradient2DTab(gy[1],sz,gaussian1stDerivKernelTab,tmpBuffUShort,iRadius,1,nx,ny,nz,k+1);
			Get3DModulus(norm[2], gx[1],gy[1],&gz[tmppos],sliceSize);
			// to czego nie mam 3d
			if (dim==3)
				Remove_Gradient_NonMaxima_Slice_3D( &gz[tmppos-sliceSize], gx[0], gy[0], &gz[tmppos-sliceSize], norm, sliceDims );
			else
				Remove_Gradient_NonMaxima_Slice_2D(&gz[tmppos-sliceSize],gx[0],gy[0],norm[1],sliceDims );

		
		}
		//to czego nie mam 2d
		Remove_Gradient_NonMaxima_Slice_2D(&gz[tmppos],gx[1],gy[1],norm[2],sliceDims );


		//-------------------------------------------------
		
		sz=NULL;
		delete []tmpBuffUShort;
		//delete []tmpBuffFloat;
		delete []gaussian1stDerivKernelTab;
		delete []gaussianKernelTab;
		delete smoothedRawDataInZ;
		// zrobie ostatni zbior wynikowy
		double min=INT_MAX;
		double max=-INT_MAX;
		for(i=0;i<volumeSize;i++)
		{
			if (gz[i]<min) min=gz[i];
			if (gz[i]>max) max=gz[i];
		}
		double factor=max-min;
		factor=(factor!=0)?factor=(double)65535/factor:factor=1;
	
		RawDataSet *outputData=new RawDataSet();
		outputData->Copy(*rawData,false);
		Q=outputData->GetDataArray();
		
		for(i=0;i<volumeSize;i++)
			*(Q++)=(gz[i]-min)*factor;
		//outputData->CalculateHistogram();
		//----------------------------
		Q=NULL;
		gz=NULL;
		delete []tabDerivatedInZ;
		return outputData;
	}
	return NULL;
}

//----------------------------------------------
void EdgesExtraction3D::GetGradient2DTab(double *output,unsigned short *input3D,double *kernel,unsigned short *tmpBuff,int radius,int view,int nx,int ny,int nz,int zPos)
{

	int i,j;
	int pos=0;
	int size=2*radius+1;
	for (j=0;j<ny;j++)
		for(i=0;i<nx;i++)
		{
			GetIntensityTab1D(input3D,nx,ny,nz,view,radius,tmpBuff,i,j,zPos);
			output[pos++]=Apply1DNormFilterF(kernel,tmpBuff,size);
		}
		//smoothing po x lub y dorobic

}
//--------------------------------------------------------------
void EdgesExtraction3D::Get3DModulus(double *output, double *inX,double *inY,double *inZ,int size)
{
	int i;
	for (i=0;i<size;i++)
		output[i]=sqrt(inX[i]*inX[i]+inY[i]*inY[i]+inZ[i]*inZ[i]);
}
//-------------------------------------------------
void EdgesExtraction3D::Remove_Gradient_NonMaxima_Slice_2D( double *maxima,double *gx,double *gy,double *norme,int *bufferDims )
{
	/* 
	* the buffer norme[0] contains the gradient modulus of the 
	* previous slice, the buffer norme[1] the ones of the
	* slice under study, while norme[2] containes the ones
	* of the next slice.
	*/
	/*
	* dimensions
	*/
	int dimx = bufferDims[0];
	int dimy = bufferDims[1];
	int dimxMinusOne = dimx - 1;
	int dimxPlusOne = dimx + 1;
	int dimyMinusOne = dimy - 1;
	/* 
	* pointers
	*/
	double *fl_pt1 = (double*)NULL;
	double *fl_pt2 = (double*)NULL;
	double *fl_max = (double*)NULL;
	double *fl_nor = (double*)NULL;
	double *fl_upper_left = (double*)NULL;
	/*
	* coordinates and vector's components
	*/
	int x, y;
	double normalized_gx;
	double normalized_gy;
	double x_point_to_be_interpolated;
	double y_point_to_be_interpolated;
	int x_upper_left_corner;
	int y_upper_left_corner;
	/*
	* coefficients
	*/ 
	double dx, dy, dxdy;
	double c00, c01, c10, c11;
	/*
	* modulus
	*/
	double interpolated_norme;

	/*
	* we set the image border to zero.
	* First the borders along X direction,
	* second, the borders along the Y direction.
	*/
	fl_pt1 = maxima;
	fl_pt2 = maxima + (dimy-1)*dimx;
	for (x=0; x<dimx; x++, fl_pt1++, fl_pt2++ )
		*fl_pt1 = *fl_pt2 = 0.0;
	fl_pt1 = maxima + dimx;
	fl_pt2 = maxima + dimx + dimx - 1;
	for (y=1; y<dimy-1; y++, fl_pt1+=dimx, fl_pt2+=dimx )
		*fl_pt1 = *fl_pt2 = 0.0;

	/*
	* We investigate the middle of the image.
	*/
	/* 
	* Pointers are set to the first point
	* to be processed.
	*/
	fl_max = maxima + dimx + 1;
	fl_pt1 = gx + dimx + 1;
	fl_pt2 = gy + dimx + 1;
	fl_nor = norme + dimx + 1;
	for ( y=1; y<dimyMinusOne; y++, fl_max+=2, fl_pt1+=2, fl_pt2+=2, fl_nor+=2 )
		for ( x=1; x<dimxMinusOne; x++, fl_max++,  fl_pt1++,  fl_pt2++,  fl_nor++ ) {
			/*
			* If the modulus is too small, go to the next point.
			*/
			if ( *fl_nor < _EPSILON_NORM_ ) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* We normalize the vector gradient.
			*/
			normalized_gx = *fl_pt1 / *fl_nor;
			normalized_gy = *fl_pt2 / *fl_nor;

			/*
			* May we use the nearest value?
			*/
			if ( (-normalized_gx > _EPSILON_DERIVATIVE_) ||
				(normalized_gx > _EPSILON_DERIVATIVE_) ||
				(-normalized_gy > _EPSILON_DERIVATIVE_) ||
				(normalized_gy > _EPSILON_DERIVATIVE_) ) {
					/*
					* First point to be interpolated.
					*/
					x_upper_left_corner = (int)( (double)x + normalized_gx + 0.5 );
					y_upper_left_corner = (int)( (double)y + normalized_gy + 0.5 );
					interpolated_norme = *(norme + (x_upper_left_corner + y_upper_left_corner * dimx));
					if ( *fl_nor <= interpolated_norme ) {
						*fl_max = 0.0;
						continue;
					}
					/*
					* Second point to be interpolated.
					*/
					x_upper_left_corner = (int)( (double)x - normalized_gx + 0.5 );
					y_upper_left_corner = (int)( (double)y - normalized_gy + 0.5 );
					interpolated_norme = *(norme + (x_upper_left_corner + y_upper_left_corner * dimx));
					if ( *fl_nor < interpolated_norme ) {
						*fl_max = 0.0;
						continue;
					}
					/*
					* We found a gradient extrema.
					*/
					*fl_max = *fl_nor;
					continue;
				}


				/*
				* From here we perform a bilinear interpolation
				*/

				/*
				* First point to be interpolated.
				* It is the current point + an unitary vector
				* in the direction of the gradient.
				* It must be inside the image.
				*/
				x_point_to_be_interpolated = (double)x + normalized_gx;
				y_point_to_be_interpolated = (double)y + normalized_gy;
				if ( (x_point_to_be_interpolated < 0.0) ||
					(x_point_to_be_interpolated >= dimxMinusOne) ||
					(y_point_to_be_interpolated < 0.0) ||
					(y_point_to_be_interpolated >= dimyMinusOne) ) {
						*fl_max = 0.0;
						continue;
					}
					/* 
					* Upper left corner,
					* coordinates of the point to be interpolated
					* with respect to this corner.
					*/
					x_upper_left_corner = (int)x_point_to_be_interpolated;
					y_upper_left_corner = (int)y_point_to_be_interpolated;
					dx = x_point_to_be_interpolated - (double)x_upper_left_corner;
					dy = y_point_to_be_interpolated - (double)y_upper_left_corner;
					dxdy = dx * dy;
					/* 
					* bilinear interpolation of the gradient modulus 
					* norme[x_point_to_be_interpolated, y_point_to_be_interpolated] =
					*   norme[0,0] * ( 1 - dx) * ( 1 - dy ) +
					*   norme[1,0] * ( dx ) * ( 1 - dy ) +
					*   norme[0,1] * ( 1 - dx ) * ( dy ) +
					*   norme[1,1] * ( dx ) * ( dy )
					*/
					c00 = 1.0 - dx - dy + dxdy;
					c10 = dx - dxdy;
					c01 = dy - dxdy;
					c11 = dxdy;
					fl_upper_left = norme + (x_upper_left_corner + y_upper_left_corner * dimx);
					interpolated_norme = *(fl_upper_left) * c00 +
						*(fl_upper_left + 1) * c10 +
						*(fl_upper_left + dimx) * c01 +
						*(fl_upper_left + dimxPlusOne) * c11;
					/*
					* We compare the modulus of the point with the
					* interpolated modulus. It must be larger to be
					* still considered as a potential gradient extrema.
					*
					* Here, we consider that it is strictly superior.
					* The next comparison will be superior or equal.
					* This way, the extrema is in the light part of the
					* image. 
					* By inverting both tests, we can put it in the
					* dark side of the image.
					*/
					if ( *fl_nor <= interpolated_norme ) {
						*fl_max = 0.0;
						continue;
					}
					/*
					* Second point to be interpolated.
					* It is the current point - an unitary vector
					* in the direction of the gradient.
					* It must be inside the image.
					*/
					x_point_to_be_interpolated = (double)x - normalized_gx;
					y_point_to_be_interpolated = (double)y - normalized_gy;
					if ( (x_point_to_be_interpolated < 0.0) ||
						(x_point_to_be_interpolated >= dimxMinusOne) ||
						(y_point_to_be_interpolated < 0.0) ||
						(y_point_to_be_interpolated >= dimyMinusOne) ) {
							*fl_max = 0.0;
							continue;
						}
						/* 
						* Upper left corner.
						*/
						x_upper_left_corner = (int)x_point_to_be_interpolated;
						y_upper_left_corner = (int)y_point_to_be_interpolated;
						/* we do not recompute the coefficients
						dx = x_point_to_be_interpolated - (double)x_upper_left_corner;
						dy = y_point_to_be_interpolated - (double)y_upper_left_corner;
						dxdy = dx * dy;
						*/
						/*
						* We may use the previous coefficients.
						* norme[x_point_to_be_interpolated, y_point_to_be_interpolated] =
						*   norme[0,0] * c11 +
						*   norme[1,0] * c01 +
						*   norme[0,1] * c10 +
						*   norme[1,1] * c00
						*
						* WARNING: it works only if the cases where one derivative is close
						*          to -/+ 1 are already be independently processed, else
						*          it may lead to errors.
						*/
						/* we do not recompute the coefficients
						c00 = 1.0 - dx - dy + dxdy;
						c10 = dx - dxdy;
						c01 = dy - dxdy;
						c11 = dxdy;
						fl_upper_left = norme + (x_upper_left_corner + y_upper_left_corner * dimx);
						interpolated_norme = *(fl_upper_left) * c00 +
						*(fl_upper_left + 1) * c10 +
						*(fl_upper_left + dimx) * c01 +
						*(fl_upper_left + dimxPlusOne) * c11;
						*/
						fl_upper_left = norme + (x_upper_left_corner + y_upper_left_corner * dimx);
						interpolated_norme = *(fl_upper_left) * c11 +
							*(fl_upper_left + 1) * c01 +
							*(fl_upper_left + dimx) * c10 +
							*(fl_upper_left + dimxPlusOne) * c00;
						/*
						* Last test to decide whether or not we 
						* have an extrema
						*/
						if ( *fl_nor < interpolated_norme ) {
							*fl_max = 0.0;
							continue;
						}
						/*
						* We found a gradient extrema.
						*/
						*fl_max = *fl_nor;
		}
}
//-----------------------------------------------------------
void EdgesExtraction3D::Remove_Gradient_NonMaxima_Slice_3D( double *maxima,double *gx,double *gy,double *gz,double **norme,int *bufferDims )
{
	/* 
	* the buffer norme[0] contains the gradient modulus of the 
	* previous slice, the buffer norme[1] the ones of the
	* slice under study, while norme[2] containes the ones
	* of the next slice.
	*/
	/*
	* dimensions
	*/
	int dimx = bufferDims[0];
	int dimy = bufferDims[1];
	int dimxMinusOne = dimx - 1;
	int dimxPlusOne = dimx + 1;
	int dimyMinusOne = dimy - 1;
	/* 
	* pointers
	*/
	double *fl_pt1 = (double*)NULL;
	double *fl_pt2 = (double*)NULL;
	double *fl_pt3 = (double*)NULL;
	double *fl_max = (double*)NULL;
	double *fl_nor = (double*)NULL;
	double *fl_upper_left = (double*)NULL;
	/*
	* coordinates and vector's components
	*/
	int x, y;
	int z;
	double normalized_gx;
	double normalized_gy;
	double normalized_gz;
	double x_point_to_be_interpolated;
	double y_point_to_be_interpolated;
	double z_point_to_be_interpolated;
	int x_upper_left_corner;
	int y_upper_left_corner;
	int z_upper_left_corner;
	/*
	* coefficients
	*/ 
	double dx, dy, dz;
	double dxdy, dxdz, dydz;
	double c000, c010, c100, c110;
	double c001, c011, c101, c111;
	/*
	* modulus
	*/
	double interpolated_norme;

	/*
	* we set the image border to zero.
	* First the borders along X direction,
	* second, the borders along the Y direction.
	*/
	fl_pt1 = maxima;
	fl_pt2 = maxima + (dimy-1)*dimx;
	for (x=0; x<dimx; x++, fl_pt1++, fl_pt2++ )
		*fl_pt1 = *fl_pt2 = 0.0;
	fl_pt1 = maxima + dimx;
	fl_pt2 = maxima + dimx + dimx - 1;
	for (y=1; y<dimy-1; y++, fl_pt1+=dimx, fl_pt2+=dimx )
		*fl_pt1 = *fl_pt2 = 0.0;

	/*
	* We investigate the middle of the image.
	*/
	/* 
	* Pointers are set to the first point
	* to be processed.
	*/
	fl_max = maxima + dimx + 1;
	fl_pt1 = gx + dimx + 1;
	fl_pt2 = gy + dimx + 1;
	fl_pt3 = gz + dimx + 1;
	fl_nor = norme[1] + dimx + 1;
	z = 1;
	for ( y=1; y<dimyMinusOne; y++, fl_max+=2, fl_pt1+=2, fl_pt2+=2, fl_pt3+=2, fl_nor+=2 )
		for ( x=1; x<dimxMinusOne; x++, fl_max++,  fl_pt1++,  fl_pt2++,  fl_pt3++,  fl_nor++ ) {

			/*
			* If the modulus is too small, go to the next point.
			*/
			if ( *fl_nor < _EPSILON_NORM_ ) {
				*fl_max = 0.0;
				continue;
			}
			/*
			* We normalize the vector gradient.
			*/
			normalized_gx = *fl_pt1 / *fl_nor;
			normalized_gy = *fl_pt2 / *fl_nor;
			normalized_gz = *fl_pt3 / *fl_nor;

			/*
			* May we use the nearest value?
			*/
			if ( (-normalized_gx > _EPSILON_DERIVATIVE_) ||
				(normalized_gx > _EPSILON_DERIVATIVE_) ||
				(-normalized_gy > _EPSILON_DERIVATIVE_) ||
				(normalized_gy > _EPSILON_DERIVATIVE_) ||
				(-normalized_gz > _EPSILON_DERIVATIVE_) ||
				(normalized_gz > _EPSILON_DERIVATIVE_) ) {
					/*
					* First point to be interpolated.
					*/
					x_upper_left_corner = (int)( (double)x + normalized_gx + 0.5 );
					y_upper_left_corner = (int)( (double)y + normalized_gy + 0.5 );
					z_upper_left_corner = (int)( (double)z + normalized_gz + 0.5 );
					interpolated_norme = *(norme[z_upper_left_corner] 
					+ (x_upper_left_corner + y_upper_left_corner * dimx));
					if ( *fl_nor <= interpolated_norme ) {
						*fl_max = 0.0;
						continue;
					}
					/*
					* Second point to be interpolated.
					*/
					x_upper_left_corner = (int)( (double)x - normalized_gx + 0.5 );
					y_upper_left_corner = (int)( (double)y - normalized_gy + 0.5 );
					z_upper_left_corner = (int)( (double)z - normalized_gz + 0.5 );
					interpolated_norme = *(norme[z_upper_left_corner] 
					+ (x_upper_left_corner + y_upper_left_corner * dimx));
					if ( *fl_nor < interpolated_norme ) {
						*fl_max = 0.0;
						continue;
					}
					/*
					* We found a gradient extrema.
					*/
					*fl_max = *fl_nor;
					continue;
				}


				/*
				* From here we perform a trilinear interpolation
				*/

				/*
				* First point to be interpolated.
				* It is the current point + an unitary vector
				* in the direction of the gradient.
				* It must be inside the image.
				*/
				x_point_to_be_interpolated = (double)x + normalized_gx;
				y_point_to_be_interpolated = (double)y + normalized_gy;
				z_point_to_be_interpolated = (double)z + normalized_gz;
				if ( (x_point_to_be_interpolated < 0.0) ||
					(x_point_to_be_interpolated >= dimxMinusOne) ||
					(y_point_to_be_interpolated < 0.0) ||
					(y_point_to_be_interpolated >= dimyMinusOne) ) {
						*fl_max = 0.0;
						continue;
					}

					/* 
					* Upper left corner,
					* coordinates of the point to be interpolated
					* with respect to this corner.
					*/
					x_upper_left_corner = (int)x_point_to_be_interpolated;
					y_upper_left_corner = (int)y_point_to_be_interpolated;
					z_upper_left_corner = (int)z_point_to_be_interpolated;
					dx = x_point_to_be_interpolated - (double)x_upper_left_corner;
					dy = y_point_to_be_interpolated - (double)y_upper_left_corner;
					dz = z_point_to_be_interpolated - (double)z_upper_left_corner;
					/* 
					* trilinear interpolation of the gradient modulus 
					* norme[x_point_to_be_interpolated, 
					*       y_point_to_be_interpolated,
					*       z_point_to_be_interpolated] =
					*   norme[0,0,0] * ( 1 - dx) * ( 1 - dy ) * ( 1 - dz ) +
					*   norme[1,0,0] * ( dx ) * ( 1 - dy ) * ( 1 - dz ) +
					*   norme[0,1,0] * ( 1 - dx ) * ( dy ) * ( 1 - dz ) +
					*   norme[1,1,0] * ( dx ) * ( dy ) * ( 1 - dz ) +
					*   norme[0,0,1] * ( 1 - dx) * ( 1 - dy ) * ( dz ) +
					*   norme[1,0,1] * ( dx ) * ( 1 - dy ) * ( dz ) +
					*   norme[0,1,1] * ( 1 - dx ) * ( dy ) * ( dz ) +
					*   norme[1,1,1] * ( dx ) * ( dy ) * ( dz )
					*/
					dxdy = dx * dy;
					dydz = dy * dz;
					dxdz = dx * dz;
					c111 = dxdy * dz;
					c011 = dydz - c111;
					c101 = dxdz - c111;
					c001 = dz - dxdz - c011;
					c110 = dxdy - c111;
					c010 = dy - dxdy - c011;
					c100 = dx - dxdy - c101;
					c000 = 1.0 - dx - dy + dxdy - c001;
					fl_upper_left = norme[z_upper_left_corner]
					+ (x_upper_left_corner + y_upper_left_corner * dimx);
					interpolated_norme = *(fl_upper_left) * c000 +
						*(fl_upper_left + 1) * c100 +
						*(fl_upper_left + dimx) * c010 +
						*(fl_upper_left + dimxPlusOne) * c110;
					fl_upper_left = norme[z_upper_left_corner+1]
					+ (x_upper_left_corner + y_upper_left_corner * dimx);
					interpolated_norme += *(fl_upper_left) * c001 +
						*(fl_upper_left + 1) * c101 +
						*(fl_upper_left + dimx) * c011 +
						*(fl_upper_left + dimxPlusOne) * c111;
					/*
					* We compare the modulus of the point with the
					* interpolated modulus. It must be larger to be
					* still considered as a potential gradient extrema.
					*
					* Here, we consider that it is strictly superior.
					* The next comparison will be superior or equal.
					* This way, the extrema is in the light part of the
					* image. 
					* By inverting both tests, we can put it in the
					* dark side of the image.
					*/
					if ( *fl_nor <= interpolated_norme ) {
						*fl_max = 0.0;
						continue;
					}
					/*
					* Second point to be interpolated.
					* It is the current point - an unitary vector
					* in the direction of the gradient.
					* It must be inside the image.
					*/
					x_point_to_be_interpolated = (double)x - normalized_gx;
					y_point_to_be_interpolated = (double)y - normalized_gy;
					z_point_to_be_interpolated = (double)z - normalized_gz;
					if ( (x_point_to_be_interpolated < 0.0) ||
						(x_point_to_be_interpolated >= dimxMinusOne) ||
						(y_point_to_be_interpolated < 0.0) ||
						(y_point_to_be_interpolated >= dimyMinusOne) ) {
							*fl_max = 0.0;
							continue;
						}
						/* 
						* Upper left corner.
						*/
						x_upper_left_corner = (int)x_point_to_be_interpolated;
						y_upper_left_corner = (int)y_point_to_be_interpolated;
						z_upper_left_corner = (int)z_point_to_be_interpolated;
						/* we do not recompute the coefficients
						dx = x_point_to_be_interpolated - (double)x_upper_left_corner;
						dy = y_point_to_be_interpolated - (double)y_upper_left_corner;
						dz = z_point_to_be_interpolated - (double)z_upper_left_corner;
						*/
						/*
						* We use the previous coefficients. 
						* norme[x_point_to_be_interpolated, 
						*       y_point_to_be_interpolated,
						*       z_point_to_be_interpolated] =
						*   norme[0,0,0] * c111 +
						*   norme[1,0,0] * c011 +
						*   norme[0,1,0] * c101 +
						*   norme[1,1,0] * c001 +
						*   norme[0,0,1] * c110 +
						*   norme[1,0,1] * c010 +
						*   norme[0,1,1] * c100 +
						*   norme[1,1,1] * c000
						*

						fl_upper_left = norme[z_upper_left_corner]
						+ (x_upper_left_corner + y_upper_left_corner * dimx);
						interpolated_norme = *(fl_upper_left) * c111 +
						*(fl_upper_left + 1) * c011 +
						*(fl_upper_left + dimx) * c101 +
						*(fl_upper_left + dimxPlusOne) * c001;
						fl_upper_left = norme[z_upper_left_corner+1]
						+ (x_upper_left_corner + y_upper_left_corner * dimx);
						interpolated_norme += *(fl_upper_left) * c110 +
						*(fl_upper_left + 1) * c010 +
						*(fl_upper_left + dimx) * c100 +
						*(fl_upper_left + dimxPlusOne) * c000;

						*
						* WARNING: as in the 2D case it works only if the cases where one
						*          derivative is close to -/+ 1 are already be independently
						*          processed, else it may lead to errors.
						*/
						/* we do not recompute the coefficients
						dxdy = dx * dy;
						dydz = dy * dz;
						dxdz = dx * dz;
						c111 = dxdy * dz;
						c011 = dydz - c111;
						c101 = dxdz - c111;
						c001 = dz - dxdz - c011;
						c110 = dxdy - c111;
						c010 = dy - dxdy - c011;
						c100 = dx - dxdy - c101;
						c000 = 1.0 - dx - dy + dxdy - c001;
						fl_upper_left = norme[z_upper_left_corner]
						+ (x_upper_left_corner + y_upper_left_corner * dimx);
						interpolated_norme = *(fl_upper_left) * c000 +
						*(fl_upper_left + 1) * c100 +
						*(fl_upper_left + dimx) * c010 +
						*(fl_upper_left + dimxPlusOne) * c110;
						fl_upper_left = norme[z_upper_left_corner+1]
						+ (x_upper_left_corner + y_upper_left_corner * dimx);
						interpolated_norme += *(fl_upper_left) * c001 +
						*(fl_upper_left + 1) * c101 +
						*(fl_upper_left + dimx) * c011 +
						*(fl_upper_left + dimxPlusOne) * c111;
						*/

						fl_upper_left = norme[z_upper_left_corner]
						+ (x_upper_left_corner + y_upper_left_corner * dimx);
						interpolated_norme = *(fl_upper_left) * c111 +
							*(fl_upper_left + 1) * c011 +
							*(fl_upper_left + dimx) * c101 +
							*(fl_upper_left + dimxPlusOne) * c001;
						fl_upper_left = norme[z_upper_left_corner+1]
						+ (x_upper_left_corner + y_upper_left_corner * dimx);
						interpolated_norme += *(fl_upper_left) * c110 +
							*(fl_upper_left + 1) * c010 +
							*(fl_upper_left + dimx) * c100 +
							*(fl_upper_left + dimxPlusOne) * c000;

						/*
						* Last test to decide whether or not we 
						* have an extrema
						*/
						if ( *fl_nor < interpolated_norme ) {
							*fl_max = 0.0;
							continue;
						}
						/*
						* We found a gradient extrema.
						*/
						*fl_max = *fl_nor;
		}
}
