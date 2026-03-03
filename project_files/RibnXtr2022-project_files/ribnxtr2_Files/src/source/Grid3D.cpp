#include "Grid3D.h"

FloatGrid3D::FloatGrid3D(int nx,int ny,int nz)
{
	Init( nx, ny, nz);
	fileList=new 	LinkedList <FilePointer*>;
}

//---------------------------------
FloatGrid3D::FloatGrid3D()
{
	Nx=0;
	Ny=0;
	Nz=0;
	Nx2=0;
	Ny2=0;
	Nz2=0;
	Nxy2=0;
	size=0;
	Nxy=0;

	GridXTab = NULL;
	GridYTab = NULL;
	GridZTab = NULL;
	fileList=new 	LinkedList <FilePointer*>;
}
//---------------------------------
FloatGrid3D::~FloatGrid3D()
{
	if (GridXTab!=NULL)
		delete []GridXTab;
	if (GridYTab!=NULL)
		delete []GridYTab;
	if (GridZTab!=NULL)
		delete []GridZTab;

	//if (fileList->GetNumberOfItems()>0)
		delete fileList;
}
//---------------------------------
//---------------------------------
void FloatGrid3D::Init(int nx,int ny,int nz)
{
	
	Nx=nx;
	Ny=ny;
	Nz=nz;
	Nx2=Nx+2;
	Ny2=Ny+2;
	Nz2=Nz+2;
	Nxy2=Nx2*Ny2;
	size=Nxy2*Nz2;
	Nxy=nx*ny;
//poprawic - a jak juz sa?!
	GridXTab=new double[size];
	GridYTab=new double[size];
	GridZTab=new double[size];

	Clear();
}
//---------------------------------
void FloatGrid3D::Copy(const FloatGrid3D &g,bool copydata)
{
	size=g.size;
		
	if (GridXTab!=NULL) delete[]GridXTab;
	if (GridYTab!=NULL) delete[]GridYTab;
	if (GridZTab!=NULL) delete[]GridZTab;

	GridXTab=new double[size];
	GridYTab=new double[size];
	GridZTab=new double[size];



	Nx=g.Nx;
	Ny=g.Ny;
	Nz=g.Nz;
	Nx2=g.Nx2;
	Ny2=g.Ny2;
	Nz2=g.Nz2;
	Nxy2=g.Nxy2;
	//size=g.size;
	Nxy=g.Nxy;

	if (copydata==true)
	{
		if (g.GridXTab!=NULL)	memcpy(GridXTab,g.GridXTab,size*sizeof(double));
		if (g.GridYTab!=NULL)	memcpy(GridYTab,g.GridYTab,size*sizeof(double));
		if (g.GridZTab!=NULL)	memcpy(GridZTab,g.GridZTab,size*sizeof(double));
	}
	else
	{
		Clear();
	}
		fileList=new 	LinkedList <FilePointer*>;
	
}
//-----------------------------------
FloatGrid3D::FloatGrid3D(const FloatGrid3D &g)
{	
	GridXTab = NULL;
	GridYTab = NULL;
	GridZTab = NULL;
	Copy(g);
}
//-------------------------------------------
void FloatGrid3D::Clear()
{
	if (GridXTab!=NULL) memset(GridXTab,0,size*sizeof(double));
	if (GridYTab!=NULL) memset(GridYTab,0,size*sizeof(double));
	if (GridZTab!=NULL) memset(GridZTab,0,size*sizeof(double));

}
//-----------------------------------------------
bool FloatGrid3D::LoadGrid(const QString &filename,int nr)
{
	int i;

	FILE *plik;
	plik=fopen(filename.toLocal8Bit(),"rt");

	fscanf(plik,"%d %d %d\r\n",&Nx,&Ny,&Nz);
	int pos=0;
	int itemsNr=fileList->GetNumberOfItems();
	if ((Nx>0)&&(Ny>0)&&(Nz>0))
	{
		if (nr==0)
		{
			Init(Nx,Ny,Nz);
			for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridXTab[i]);
			for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridYTab[i]);
			for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridZTab[i]);

		}
		else if (nr<itemsNr)
		{

				//FILE *plik2=fileList->GetItemAtPos(nr)->GetData()->plik;
				fseek(plik,fileList->GetItemAtPos(nr)->GetData()->pos,0);
				fscanf(plik,"%d %d %d\r\n",&Nx,&Ny,&Nz);
				Init(Nx,Ny,Nz);
			
				for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridXTab[i]);
				for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridYTab[i]);
				for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridZTab[i]);
		}
		else
		{
			double tmp;
			//skip pierwszy
			for(i=0;i<3*(Nx+2)*(Ny+2)*(Nz+2);i++)
				fscanf(plik,"%lf,",&tmp);

			char t[2];//t[0]='\n';
		
			do 
			{
				t[0]=' ';
				fscanf(plik,"%c",t);
				if ((t[0]=='\n')||(t[0]=='\r'))
				{
					pos++;
					fscanf(plik,"%d %d %d\n",&Nx,&Ny,&Nz);
					if (!((Nx>0)&&(Ny>0)&&(Nz>0))) return false;

					if (pos==nr)
					{
						//wczytam dane
						Init(Nx,Ny,Nz);
						for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridXTab[i]);
						for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridYTab[i]);
						for(i=0;i<size;i++)	fscanf(plik,"%lf,",&GridZTab[i]);
						fclose(plik);
						return true;
					}
					else
					{
						//skip
						for(i=0;i<3*(Nx+2)*(Ny+2)*(Nz+2);i++)
							fscanf(plik,"%lf,",&tmp);
					}
				}
				else
				{
					fclose(plik);
					return false;
				}
				
			}while((pos<nr)&&(t[0]=='\n')||(t[0]=='\r'));
			// tu trzeba skipowac i wczytac jesli jest odpowiedni plik
		}
	}
	else 
	{
		fclose(plik);
		return false;
	}
	fclose(plik);
	return true;

}
//---------------------------------
int FloatGrid3D::GetNumberOfGrids(const QString &filename)
{
	int i;
	int tpos;
	if (fileList->GetNumberOfItems()>0)
		fileList->DeleteAllItems();

	FILE *plik;
	FILE *plikTmp;
	FilePointer *fPointer;
	plik=fopen(filename.toLocal8Bit(),"rt");

//	plikTmp=plik;
	tpos=ftell(plik);
	fscanf(plik,"%d %d %d\n",&Nx,&Ny,&Nz);
	int pos=0;
	if ((Nx>0)&&(Ny>0)&&(Nz>0))
	{
		fPointer=new FilePointer(tpos);
		fileList->InsertItem(fPointer);
		
		pos++;
		double tmp;
	
		//skip pierwszy

		for(i=0;i<3*(Nx+2)*(Ny+2)*(Nz+2);i++)
			fscanf(plik,"%lf,",&tmp);

		char t[2];//t[0]='\n';
	
		do 
		{
			t[0]=' ';
			fscanf(plik,"%c",t);
			if ((t[0]=='\n')||(t[0]=='\r'))
			{
				pos++;
				tpos=ftell(plik);
				//plikTmp=plik;
				fscanf(plik,"%d %d %d\n",&Nx,&Ny,&Nz);
				if (!((Nx>0)&&(Ny>0)&&(Nz>0))) return false;

				//skip
				fPointer=new FilePointer(tpos);
				fileList->InsertItem(fPointer);
				for(i=0;i<3*(Nx+2)*(Ny+2)*(Nz+2);i++)
					fscanf(plik,"%lf,",&tmp);
				
			}
			
		}while ((t[0]=='\n')||(t[0]=='\r'));
			// tu trzeba skipowac i wczytac jesli jest odpowiedni plik
		
	}
	else 
	{
		fclose(plik);
		return 0;
	}

	fclose(plik);
	return pos;

}
//---------------------------------
bool FloatGrid3D::SaveGrid(const QString &filename,bool multi)
{

	int i;

	QString name=filename;
	FILE *plik;
	
	if (multi==false)
	{
		if (filename.lastIndexOf(".grid")==-1)
		name+=".grid";
		plik=fopen(name.toLocal8Bit(),"wt");
	}
	else
	{
		if (filename.lastIndexOf(".mgrid")==-1)
		name+=".mgrid";
		plik=fopen(name.toLocal8Bit(),"a+t");
		
		// to cos sprawdza jak dlugi jest plik
		fseek(plik,0L,2);
		int b=ftell(plik); 
		if (b!=0)
			fprintf(plik,"\n");	

	}


	fprintf(plik,"%d %d %d\n",Nx,Ny,Nz);

	if (GridXTab!=NULL)
		for(i=0;i<size;i++)
			fprintf(plik,"%lf,",GridXTab[i]);

	if (GridYTab!=NULL)
		for(i=0;i<size;i++)
			fprintf(plik,"%lf,",GridYTab[i]);

	if (GridZTab!=NULL)
		for(i=0;i<size;i++)
			fprintf(plik,"%lf,",GridZTab[i]);

	fclose(plik);


	return true;
}
//---------------------------------
void	FloatGrid3D::Scale(double x,double y,double z)
{
	int i,j,k;
	for (k=0;k<Nz;k++)
		for (j=0;j<Ny;j++)
			for (i=0;i<Nx;i++)
			{

				SetDx(i,j,k,GetDX(i,j,k)*x);
				SetDy(i,j,k,GetDY(i,j,k)*y);
				SetDz(i,j,k,GetDZ(i,j,k)*z);
			}
}

//---------------------------------
void FloatGrid3D::Blend(const FloatGrid3D *Grid,double amount)
{
	//zaraz
	if ((Nx==Grid->Nx)&&(Ny==Grid->Ny)&&(Nz==Grid->Nz))
	{
		int i;
		for(i=0;i<size;i++)
		{
			GridXTab[i]=GridXTab[i]*(1.0-amount)+Grid->GridXTab[i]*amount;
			GridYTab[i]=GridYTab[i]*(1.0-amount)+Grid->GridYTab[i]*amount;
			GridZTab[i]=GridZTab[i]*(1.0-amount)+Grid->GridZTab[i]*amount;
		}
	}
	else
	{
		//to jak mnie natchnie
		//bo tu trzebba zrobic bedzie plynne przejscie pomiedzy gridami o roznych wielkosciach
		// i trzba bedzie dokonac b-spline deformacji na gridzie.
	}

}
//----------------------------------
bool FloatGrid3D::Check3DLocus(int x,int y,int z)
{

	// jak jest ok to zwraca true a jak nie siedzi w srodku to zwraca falsz
	
	int p0=(z+1)*Nxy2+(y+1)*Nx2+(x+1);	//x    y    z
	int p1=p0+1;						//x+1  y    z
	int p2=p0-1;						//x-1  y    z
	int p3=p0+Nx2;						//x    y+1  z
	int p4=p0-Nx2;						//x    y-1  z
	int p5=p0+Nxy2;						//x    y    z+1
	int p6=p0-Nxy2;						//x    y    z-1


	double P1[3],P2[3],P3[3];
	double P4[3],P5[3],P6[3];
	double P0[3];//ten jest staly
	double tol=10e-5;

	P0[0]=GridXTab[p0];	 		P0[1]=GridYTab[p0];			P0[2]=GridZTab[p0];
	P1[0]=GridXTab[p1]+1.0;		P1[1]=GridYTab[p1];			P1[2]=GridZTab[p1];
	P2[0]=GridXTab[p2]-1.0;		P2[1]=GridYTab[p2];			P2[2]=GridZTab[p2];
	P3[0]=GridXTab[p3];			P3[1]=GridYTab[p3]+1.0;		P3[2]=GridZTab[p3];
	P4[0]=GridXTab[p4];			P4[1]=GridYTab[p4]-1.0;		P4[2]=GridZTab[p4];
	P5[0]=GridXTab[p5];			P5[1]=GridYTab[p5];			P5[2]=GridZTab[p5]+1.0;
	P6[0]=GridXTab[p6];			P6[1]=GridYTab[p6];			P6[2]=GridZTab[p6]-1.0;
	
	// pobieram punkty robie z nich plaszczyzne i 
	// sprawdzam czy lezy po dobrej str pkt w razie niepowodzenia zwracam falsz


	PlaneObject Plane;
	double a;
	Plane.SetPlanePoints(P5,P3,P2);a=-Plane.CheckPoint(P0);if (a>tol)	return false;
	Plane.SetPlanePoints(P5,P1,P3);a=-Plane.CheckPoint(P0);if (a>tol)	return false;
	Plane.SetPlanePoints(P5,P4,P1);a=-Plane.CheckPoint(P0);if (a>tol)	return false;
	Plane.SetPlanePoints(P5,P2,P4);a=-Plane.CheckPoint(P0);if (a>tol)	return false;
	Plane.SetPlanePoints(P6,P3,P2);a=Plane.CheckPoint(P0);if (a>tol)	return false;
	Plane.SetPlanePoints(P6,P1,P3);a=Plane.CheckPoint(P0);if (a>tol)	return false;
	Plane.SetPlanePoints(P6,P4,P1);a=Plane.CheckPoint(P0);if (a>tol)	return false;
	Plane.SetPlanePoints(P6,P2,P4);a=Plane.CheckPoint(P0);if (a>tol)	return false;
	return true;

}

double FloatGrid3D::GetMaxDistanceWith3DLocus(int x,int y,int z)
{

	// jak jest ok to zwraca true a jak nie siedzi w srodku to zwraca falsz
	
	int p0=(z+1)*Nxy2+(y+1)*Nx2+(x+1);	//x    y    z
	int p1=p0+1;						//x+1  y    z
	int p2=p0-1;						//x-1  y    z
	int p3=p0+Nx2;						//x    y+1  z
	int p4=p0-Nx2;						//x    y-1  z
	int p5=p0+Nxy2;						//x    y    z+1
	int p6=p0-Nxy2;						//x    y    z-1


	double P1[3],P2[3],P3[3];
	double P4[3],P5[3],P6[3];
	double P0[3];//ten jest staly
	double tol=10e-5;

	P0[0]=GridXTab[p0];	 		P0[1]=GridYTab[p0];			P0[2]=GridZTab[p0];
	P1[0]=GridXTab[p1]+1.0;		P1[1]=GridYTab[p1];			P1[2]=GridZTab[p1];
	P2[0]=GridXTab[p2]-1.0;		P2[1]=GridYTab[p2];			P2[2]=GridZTab[p2];
	P3[0]=GridXTab[p3];			P3[1]=GridYTab[p3]+1.0;		P3[2]=GridZTab[p3];
	P4[0]=GridXTab[p4];			P4[1]=GridYTab[p4]-1.0;		P4[2]=GridZTab[p4];
	P5[0]=GridXTab[p5];			P5[1]=GridYTab[p5];			P5[2]=GridZTab[p5]+1.0;
	P6[0]=GridXTab[p6];			P6[1]=GridYTab[p6];			P6[2]=GridZTab[p6]-1.0;
	
	// pobieram punkty robie z nich plaszczyzne i 
	// sprawdzam czy lezy po dobrej str pkt w razie niepowodzenia zwracam falsz

	PlaneObject Plane;
	double maxD=0;
	double a;
	Plane.SetPlanePoints(P5,P3,P2);a=-Plane.CheckPoint(P0);if (a>tol)	return 0;else if (a<maxD) maxD=a;
	Plane.SetPlanePoints(P5,P1,P3);a=-Plane.CheckPoint(P0);if (a>tol)	return 0;else if (a<maxD) maxD=a;
	Plane.SetPlanePoints(P5,P4,P1);a=-Plane.CheckPoint(P0);if (a>tol)	return 0;else if (a<maxD) maxD=a;
	Plane.SetPlanePoints(P5,P2,P4);a=-Plane.CheckPoint(P0);if (a>tol)	return 0;else if (a<maxD) maxD=a;
	Plane.SetPlanePoints(P6,P3,P2);a=Plane.CheckPoint(P0);if (a>tol)	return 0;else if (a<maxD) maxD=a;
	Plane.SetPlanePoints(P6,P1,P3);a=Plane.CheckPoint(P0);if (a>tol)	return 0;else if (a<maxD) maxD=a;
	Plane.SetPlanePoints(P6,P4,P1);a=Plane.CheckPoint(P0);if (a>tol)	return 0;else if (a<maxD) maxD=a;
	Plane.SetPlanePoints(P6,P2,P4);a=Plane.CheckPoint(P0);if (a>tol)	return 0;else if (a<maxD) maxD=a;
	return maxD;

}
//----------------------------------
void FloatGrid3D::ReverseField()
{
	if ((Nx>0)&&(Ny>0)&&(Nz>0))
	{

		int i;
		for(i=0;i<size;i++)
		{
			GridXTab[i]*=-1.0;
			GridYTab[i]*=-1.0;
			GridZTab[i]*=-1.0;
		}
	}
}