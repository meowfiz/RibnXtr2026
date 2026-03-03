#ifndef GRID3D_H
#define GRID3D_H
	
#include <stdio.h>
#include <memory.h>

#include <qstring.h>

#include "PlaneObject.h"

///////////////////////////////////////////////////////

class FilePointer
{
public:
	FilePointer(int p){pos=p;};
	~FilePointer(){};
	//FILE *plik;
	int pos; 
};

class FloatGrid3D
{

	public:
		FloatGrid3D(int nx,int ny,int nz);
		FloatGrid3D();
		FloatGrid3D(const FloatGrid3D&);
		~FloatGrid3D();

		void Copy(const FloatGrid3D&,bool copydata=true);
		void Clear();
		
		void Init(int nx,int ny,int nz);
		inline double	GetDX(int x,int y,int z){return GridXTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)];}		
		inline double	GetDY(int x,int y,int z){return GridYTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)];}
		inline double	GetDZ(int x,int y,int z){return GridZTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)];}

		inline double	GetDXN(int x,int y,int z){return GridXTab[z*Nxy2+y*Nx2+x];}		
		inline double	GetDYN(int x,int y,int z){return GridYTab[z*Nxy2+y*Nx2+x];}
		inline double	GetDZN(int x,int y,int z){return GridZTab[z*Nxy2+y*Nx2+x];}

		inline double	GetDXNP(int pos){return GridXTab[pos];}		
		inline double	GetDYNP(int pos){return GridYTab[pos];}
		inline double	GetDZNP(int pos){return GridZTab[pos];}

		inline int		GetPos(int x,int y,int z){return (z+1)*Nxy2+(y+1)*Nx2+(x+1);};

		inline void	SetDx(int x,int y,int z,double v){GridXTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)]=v;};
		inline void	SetDy(int x,int y,int z,double v){GridYTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)]=v;};
		inline void	SetDz(int x,int y,int z,double v){GridZTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)]=v;};


		inline void	SetD(int i,int x,int y,int z,double v)
		{
			switch(i)
			{
				case 0:
					GridXTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)]=v;
				break;
				case 1:
					GridYTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)]=v;
				break;
				case 2:
					GridZTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)]=v;
				break;
			}
		}

		inline double	GetD(int i,int x,int y,int z)
		{
			double a;
			switch(i)
			{
				case 0:
					a= GridXTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)];
				break;
				case 1:
					a=GridYTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)];;
				break;
				case 2:
					a=GridZTab[(z+1)*Nxy2+(y+1)*Nx2+(x+1)];;
				break;
			}
			return a;
		}
		
		


		inline void	SetDx(int pos,double v){GridXTab[pos]=v;};
		inline void	SetDy(int pos,double v){GridYTab[pos]=v;};
		inline void	SetDz(int pos,double v){GridZTab[pos]=v;};



		inline int		GetNx(){return Nx;}
		inline int		GetNy(){return Ny;}
		inline int		GetNz(){return Nz;}

		inline int GetNxy2(){return Nxy2;};
		inline int GetNx2(){return Nx2;};
		bool LoadGrid(const QString &filename,int nr=0);
		bool SaveGrid(const QString &filename,bool multi=false);
		
		int GetNumberOfGrids(const QString &filename);
		void Blend(const FloatGrid3D *Grid,double amount);
	

		void	Scale(double x,double y,double z);
		bool	Check3DLocus(int x,int y,int z);
		double	GetMaxDistanceWith3DLocus(int x,int y,int z);


		void	ReverseField();

	private:

		double *GridXTab,*GridYTab,*GridZTab;
		int Nx,Ny,Nz;
		int Nx2,Ny2,Nz2,Nxy2;

		int size;
		int Nxy;


		LinkedList <FilePointer*> *fileList;

};


#endif //
//////////////////////////////////////////////////////
