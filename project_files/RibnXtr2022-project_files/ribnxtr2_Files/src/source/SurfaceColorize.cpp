#include "SurfaceColorize.h"


SurfaceColorize::SurfaceColorize()
{

}

//-------------------------------------

SurfaceColorize::~SurfaceColorize()
{

}
//-------------------------------------

void SurfaceColorize::MapCorrespondingVolumeToMesh(PolygonSurfaceData *MeshData,DataSet *Data)
{

	if ((MeshData!=NULL)&&(Data!=NULL))
	{
		int i,j;
		double Point1[3];
		double Point2[3];
		int count=0;
		RawDataSet *TempData=Data->GetRawData();
		vtkPoints	*Points=MeshData->GetVPolyData()->GetPoints();
		int			NPoint=Points->GetNumberOfPoints();

		PaletteRGBA *Palette=Data->GetPalette();

		//int tmp=0;
		double origin[3];
		double scale[3];
		TempData->GetScale(scale);
		TempData->GetOrigin(origin);
		if ((TempData->GetMode()==4)&&(TempData->GetBChannelUCHARArray()!=NULL)&&(TempData->GetGChannelUCHARArray()!=NULL))

		{

			vtkLookupTable *LUT = vtkLookupTable::New();
			LUT->SetAlphaRange(0,NPoint);
			LUT->SetNumberOfColors(NPoint);
			LUT->SetTableRange(0,NPoint);


//double	Interpolation3LinearDouble(RawDataSet *dataX,double X,double Y, double Z);
			vtkIntArray *scalars1 = vtkIntArray::New();

			//	int *RGB=new int;
			//*RGB=0;

			//	unsigned char *URGB=(unsigned char*)RGB;
			int x,y,z;
			//			int pos;
			double a=0,b=0,c=0;
		
			for (i=0;i<NPoint;i++)
			{
				Points->GetPoint(i,Point1);

				for(j=0;j<3;j++)
				Point2[j]=(Point1[j]-origin[j])/scale[j];
				//Point2[1]=(Point1[1]/TempData->GetYscale();
				//Point2[2]=(Point1[2]/TempData->GetZscale();

				if ((Point2[0]>0)&&(Point2[0]<TempData->GetNx()-1)&&
					(Point2[1]>0)&&(Point2[1]<TempData->GetNy()-1)&&
					(Point2[2]>0)&&(Point2[2]<TempData->GetNz()-1))
				{

					x=(int)(Point2[0]);
					y=(int)(Point2[1]);
					z=(int)(Point2[2]);

					//	pos=Interpolation3LinearDouble(TempData,Point2[0],Point2[1],Point2[2]);

					a=Interpolation3LinearUnsignedShort(TempData->GetRChannelArray(),TempData->GetNx(),TempData->GetNy(),TempData->GetNz(),x,y,z);
					b=Interpolation3LinearUnsignedChar(TempData->GetGChannelUCHARArray(),TempData->GetNx(),TempData->GetNy(),TempData->GetNz(),x,y,z);
					c=Interpolation3LinearUnsignedChar(TempData->GetBChannelUCHARArray(),TempData->GetNx(),TempData->GetNy(),TempData->GetNz(),x,y,z);

					//fake na momencik :)

						double R=30;
					double G=90;
					double B=150;

					double tol=27.0;
					count++;
					if ((fabs(R-a)<tol)&&(fabs(G-b)<tol)&&(fabs(B-c)<tol))
					a=c=b;
					
				}


				LUT->SetTableValue(i,
					(double)(a/255.0),
					(double)(b/255.0),
					(double)(c/255.0),1.0);

				scalars1->InsertValue(i,i);

			}

			LUT->Build();
			scalars1->SetLookupTable(LUT);
			MeshData->GetVPolyData()->GetPointData()->SetScalars(scalars1);
			MeshData->RebuildSurface(NPoint);
		}
		else
		{

			vtkLookupTable *LUT = vtkLookupTable::New();
			LUT->SetAlphaRange(0,TempData->GetMaxValue());
			LUT->SetNumberOfColors(256);

			LUT->SetTableRange(0, TempData->GetMaxValue());

			for (i=0;i<=255;i++)
			{
				LUT->SetTableValue(i,
					(double)(Palette->R[i]/255.0),
					(double)(Palette->G[i]/255.0),
					(double)(Palette->B[i]/255.0),1.0);
			}

			LUT->Build();

			vtkFloatArray *scalars1 = vtkFloatArray::New();
			//	scalars1->MakeObject ();
			scalars1->Allocate(NPoint);
			scalars1->SetLookupTable(LUT);

			for (i=0;i<NPoint;i++)
			{
				Points->GetPoint(i,Point1);

				//Point2[0]=Point1[0]/TempData->GetXscale();
				//Point2[1]=Point1[1]/TempData->GetYscale();
				//Point2[2]=Point1[2]/TempData->GetZscale();
				for(j=0;j<3;j++)
				Point2[j]=(Point1[j]-origin[j])/scale[j];
				if ((Point2[0]>0)&&(Point2[0]<TempData->GetNx()-1)&&
					(Point2[1]>0)&&(Point2[1]<TempData->GetNy()-1)&&
					(Point2[2]>0)&&(Point2[2]<TempData->GetNz()-1))
				{
					scalars1->InsertValue(i,Interpolation3LinearDouble(TempData,Point2[0],Point2[1],Point2[2]));
					//scalars1->InsertValue(i,TempData->GetVoxelValue((int)(Point2[0]+0.5),(int)(Point2[1]+0.5),(int)(Point2[2]+0.5)));
					count++;
				}
			}

			if (count>0)
			{
				MeshData->GetVPolyData()->GetPointData()->SetScalars(scalars1);
				MeshData->RebuildSurface(TempData->GetMaxValue());
			}
		}
	}

}

//-----------------------------------

void SurfaceColorize::MapPaletteOnZAxisToMesh(PolygonSurfaceData *MeshData,DataSet *Data)
{
	if ((MeshData!=NULL)&&(Data!=NULL))
	{
		int i;
		double Point1[3];

		RawDataSet *TempData=Data->GetRawData();
		vtkPoints	*Points=MeshData->GetVPolyData()->GetPoints();
		int			NPoint=Points->GetNumberOfPoints();

		PaletteRGBA *Palette=Data->GetPalette();

		vtkLookupTable *LUT = vtkLookupTable::New();
		LUT->SetAlphaRange(0,TempData->GetMaxValue());
		LUT->SetNumberOfColors(256);


		LUT->SetTableRange(0, TempData->GetMaxValue());


		for (i=0;i<=255;i++)
		{
			LUT->SetTableValue(i,
				(double)(Palette->R[i]/255.0),
				(double)(Palette->G[i]/255.0),
				(double)(Palette->B[i]/255.0),1.0);
		}

		LUT->Build();

		vtkFloatArray *scalars1 = vtkFloatArray::New();
		//	scalars1->MakeObject ();
		scalars1->Allocate(NPoint);
		scalars1->SetLookupTable(LUT);
		//int tmp=0;

		double MaxZ=-999999999.0;
		double MinZ=999999999.0;

		for (i=0;i<NPoint;i++)
		{
			Points->GetPoint(i,Point1);
			MaxZ=MAX(MaxZ,Point1[2]);
			MinZ=MIN(MinZ,Point1[2]);
		}

		double ScalePalette=(double)(255.0/(MaxZ-MinZ));

		for (i=0;i<NPoint;i++)
		{
			Points->GetPoint(i,Point1);
			scalars1->InsertValue(i,(Point1[2]-MinZ)*ScalePalette);
		}

		MeshData->GetVPolyData()->GetPointData()->SetScalars(scalars1);
		MeshData->RebuildSurface(255);



	}
}

//-----------------------------------
void	SurfaceColorize::MapTabPaletteToMesh(vtkPolyData *meshData,double *tab,PaletteRGBA *palette)
{
	if ((meshData!=NULL)&&(tab!=NULL)&&(palette!=NULL))
	{
		int i;
		double point1[3];

		vtkPoints	*points=meshData->GetPoints();
		int			nPoint=points->GetNumberOfPoints();

		
		vtkFloatArray *scalars1 = vtkFloatArray::New();
		scalars1->Allocate(nPoint);
		
	
		double max=0;
		double t;
		int t1;
		for (i=0;i<nPoint;i++)
		{
			points->GetPoint(i,point1);
			t=(int)tab[i];
			scalars1->InsertValue(i,t);
			if (t>max) max=t;
		}
		t=max/255.0;

		vtkLookupTable *lut = vtkLookupTable::New();
		lut->SetAlphaRange(0,256);
		lut->SetNumberOfColors(256);
		lut->SetTableRange(0, 256);
		for (i=0;i<256;i++)
		{t1=Round(t*i);   lut->SetTableValue(i,(double)(palette->R[t1]/255.0),(double)(palette->G[t1]/255.0),	(double)(palette->B[t1]/255.0),1.0);}
		lut->Build();
		scalars1->SetLookupTable(lut);


		meshData->GetPointData()->SetScalars(scalars1);
		//meshData->RebuildSurface(255);
	}

}
//-----------------------------------

void	SurfaceColorize::MapTabPaletteToMesh(PolygonSurfaceData *meshData, double *tab, PaletteRGBA *palette)
{
	if ((meshData != NULL) && (tab != NULL) && (palette != NULL))
	{
		int i;
		double point1[3];

		vtkPoints	*points = meshData->GetVPolyData()->GetPoints();
		int			nPoint = points->GetNumberOfPoints();


		vtkFloatArray *scalars1 = vtkFloatArray::New();
		scalars1->Allocate(nPoint);


		double max = 0;
		double t;
		int t1;
		for (i = 0; i<nPoint; i++)
		{
			points->GetPoint(i, point1);
			t = (int)tab[i];
			scalars1->InsertValue(i, t);
			if (t>max) max = t;
		}
		t = max / 255.0;

		vtkLookupTable *lut = vtkLookupTable::New();
		lut->SetAlphaRange(0, 256);
		lut->SetNumberOfColors(256);
		lut->SetTableRange(0, 256);
		for (i = 0; i<256; i++)
		{
			t1 = Round(t*i);   lut->SetTableValue(i, (double)(palette->R[t1] / 255.0), (double)(palette->G[t1] / 255.0), (double)(palette->B[t1] / 255.0), 1.0);
		}
		lut->Build();
		scalars1->SetLookupTable(lut);


		meshData->GetVPolyData()->GetPointData()->SetScalars(scalars1);
		meshData->RebuildSurface(255);
	}

}