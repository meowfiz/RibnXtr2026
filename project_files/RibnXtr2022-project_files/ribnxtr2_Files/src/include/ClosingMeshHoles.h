#ifndef CLOSING_MESH_HOLES
#define CLOSING_MESH_HOLES

#include "Geometry.h"
#include "OtherCommonFunctions.h"
#include "PlaneObject.h"



class PolyDataHolesExtractor
{

	public:	
		PolyDataHolesExtractor()
		{
			BoundaryList=new LinkedList<BoundaryElement*>;
			ForbiddenConnectionList=NULL;
			NumberOfForbiddenConnections=0;
			//ExtractHoles(Input);
		}

		virtual ~PolyDataHolesExtractor()
		{
			if (BoundaryList!=NULL)
				delete BoundaryList;

			if (ForbiddenConnectionList!=NULL)
				delete []ForbiddenConnectionList;
		}

		virtual bool ExtractHoles(vtkPolyData *Input,bool param=false)=0;
		int GetNumberOfBoundaries()
		{
			return BoundaryList->GetNumberOfItems();
		}

		virtual int *GetBoundaryTab(int pos)
		{
			return BoundaryList->GetItemAtPos(pos)->GetData()->PointIndex;
		}

		virtual int GetLengthOfBoundaryTab(int pos)
		{
			return BoundaryList->GetItemAtPos(pos)->GetData()->NumberOfPoints-1;	
		}

		virtual int *GetForbiddenConnectionList()
		{
			return ForbiddenConnectionList;
		}

		virtual int GetNumberOfForbiddenConnections()
		{
			return NumberOfForbiddenConnections;
		}
		//NA CHWILE!!!
		LinkedList<BoundaryElement*>	*BoundaryList;

	protected:	


		int *ForbiddenConnectionList;
		int NumberOfForbiddenConnections;

};
//-------------------------------------------------
class SimplePolyDataHolesExtractor : public PolyDataHolesExtractor
{
	public:
		virtual bool ExtractHoles(vtkPolyData *Input, bool ForbiddenConnections=false);
};
//----------------------------------------------
class PlanarPolyDataHolesExtractor : public PolyDataHolesExtractor
{
	public:
		virtual bool ExtractHoles(vtkPolyData *Input, bool ForbiddenConnections=false);
};
//----------------------------------------------
class TriangulationWeightingFunction
{
	// powinno zalezec od tego czy jest lista zakazanych 
	public:
		TriangulationWeightingFunction(int *tab,int _size,vtkPolyData *_InputPolyData,bool _CheckIntersection=false)
		{
			size=_size;
			InputPolyData=_InputPolyData;
			InputPoints=InputPolyData->GetPoints();
			v=tab;
			O=new int[(size+1)*(size+1)]; 
			memset(O,0,(size+1)*(size+1)*sizeof(int));
			//memset(WAngle,0,n*n*sizeof(double));
			//memset(WArea,0,n*n*sizeof(int));
			ForbiddenConnectionList=NULL;
			NumberOfForbiddenConnections=0;
			IntersectionCells=NULL;
			CheckIntersection=_CheckIntersection;
		}

		virtual ~TriangulationWeightingFunction()
		{
			if  (O!=NULL) 
			{
				delete []O;
				O=NULL;
			}
			v=NULL;
		}

		//void InitWeights();
		virtual void SetWeight(int i,int k,int value)=0;
		virtual void FindMinWeight(int i,int k)=0;
		virtual void SetInitWeight(int i)=0;
		virtual int *GetTriangulationTab(){return O;};// zwraca tablice "O"

		virtual void SetForbiddenConnectionList(int *List,int Nr)
		{
			ForbiddenConnectionList=List;
			NumberOfForbiddenConnections=Nr;
		}
	
	protected:

		double	GetWeightingFunctionValue();
		void	GetIntersectionCells();
		int size;
		int *O;
		int *v;
		vtkPoints *InputPoints;
		vtkPolyData *InputPolyData;
		int *ForbiddenConnectionList;
		int NumberOfForbiddenConnections;
		vtkCellArray *IntersectionCells;
		bool CheckIntersection;




};
//----------------------------------------------
class CrenellationWeightingFunction: public TriangulationWeightingFunction
{
	public:

		CrenellationWeightingFunction(int *tab,int _size,vtkPolyData *_InputPolyData,bool _CheckIntersection=false);
		virtual ~CrenellationWeightingFunction();

		virtual void SetWeight(int i,int k,int value);
		virtual void FindMinWeight(int i,int k);
		virtual void SetInitWeight(int i);
		virtual void SetTolerance(double tol){tolerance=tol;};


	protected:

		double	*WAngle;
		double	*WArea;
		int		Triangle0[3],Triangle1[3],Triangle2[3],Triangle3[3];
		vtkGenericCell	*TriangleCell1,*TriangleCell2,*TriangleCell3;
		vtkPolyData *AdjacentPolyData;
		void	GetIntersectionCells();
		double	tolerance;
};
//------------------------------------------------------
class MinimalAreaWeightingFunciton: public TriangulationWeightingFunction
{
	public:

		MinimalAreaWeightingFunciton(int *tab,int _size,vtkPolyData *_InputPolyData,bool _CheckIntersection=false);
		virtual ~MinimalAreaWeightingFunciton();

		virtual void SetWeight(int i,int k,int value);
		virtual void FindMinWeight(int i,int k);
		virtual void SetInitWeight(int i);
		virtual void SetTolerance(double tol){tolerance=tol;};

	protected:

		double	*WArea;
		double	tolerance;
		int		Triangle[3];
		vtkGenericCell	*TriangleCell;
		void	GetIntersectionCells();
};
//------------------------------------------------------

void	Trace(vtkCellArray *Polys,int *O,int *v,int n,int i,int k);

vtkPolyData	*FillPolyDataHolesWithCrenellations( vtkPolyData *InputPolyData);
vtkPolyData	*FillPolyDataHolesWithMinimalArea( vtkPolyData *InputPolyData);
vtkPolyData	*FillPolyDataPlanarHolesWithMinimalArea( vtkPolyData *InputPolyData);
vtkPolyData	*FillPolyDataExtractedPlanarHoles( vtkPolyData *InputPolyData);

bool FillPolyDataHole(vtkPolyData *InputPolyData,int *PointTab,int NrOfPoints,TriangulationWeightingFunction *WFunc,vtkCellArray *Polys);
bool TestAdjacentTriangles(int p1,int p2,int p3,int pos,double min, double max, double tol,
						   int *AllowedList,int NrOfAllowed, vtkPolyData *AdjacentPolyData,vtkGenericCell *TriangleCell,double &Angle);
void FindLargestPlanarPointSet(int &pos,int &size,vtkPoints *Points, int *PointTab,int Npts,
							   int *AllConTab,int NrOfAllCon,vtkPolyData *AdjPData,int *OrigPosTab,double *center);

#endif
