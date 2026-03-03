

#include "TransferFunction.h"

TransferFunctionPoint::TransferFunctionPoint(double _X,double _Y,unsigned char *_Color)
{
	X=_X;
	Y=_Y;
	Color=_Color;

}


//-------------------------------------

TransferFunctionPoint::	~TransferFunctionPoint()
{


	if (Color!=NULL) delete Color;
	Color=NULL;
}



//-------------------------------------

//-------------------------------------

//-------------------------------------


TransferFunction::TransferFunction()
{

		TransferPointsList=new LinkedList<TransferFunctionPoint*>; 

	//	NumberOfPoints=0;

}


//-------------------------------------

TransferFunction::~TransferFunction()
{

	delete TransferPointsList;
}


//-------------------------------------


//-------------------------------------

void TransferFunction::InsertPoint(int Pos,TransferFunctionPoint* Item)
{
	
	TransferPointsList->InsertItem(Pos,Item);

}

//-------------------------------------

void TransferFunction::DeletePoint(int Pos)
{
	TransferPointsList->DeleteItem(Pos);	
}

//-------------------------------------

void TransferFunction::DeleteAllPoints()
{
		TransferPointsList->DeleteAllItems();
}

//-------------------------------------


TransferFunctionPoint* TransferFunction::GetPoint(int Pos)
{
	
	return TransferPointsList->GetItemAtPos(Pos)->GetData();
}

//-------------------------------------

int TransferFunction::GetNumberOfPoints()
{
	return TransferPointsList->GetNumberOfItems();
}

//-------------------------------------

int  TransferFunction::GetNumberOfPressedPoint(int X, int Y,int SizeX,int SizeY,int Distance)
{


	// zwraca nr punktu jezeli znajduje sie w okolicy pktu transferu

	int NR=TransferPointsList->GetNumberOfItems();
	
	if (NR>0)
	{
		int		i;

//		double	XRel=(100.0*X)/(double)(SizeX-1);
//		double	YRel=(100.0*Y)/(double)(SizeY-1);

		int		TmpX,TmpY;
		double	TmpXRel,TmpYRel;

		double	TempDist;

		for (i=0;i<NR;i++)
		{
			TmpXRel=TransferPointsList->GetItemAtPos(i)->GetData()->X; // a to w procentach 
			TmpYRel=TransferPointsList->GetItemAtPos(i)->GetData()->Y;


			TmpX=Round(((SizeX-1)*TmpXRel)/100.0); //to we wspolrzednych ekranowych
			TmpY=Round(((SizeY-1)*TmpYRel)/100.0);

			TempDist=sqrt((double)((X-TmpX)*(X-TmpX)+(Y-TmpY)*(Y-TmpY)));

			if (TempDist<=Distance)
				return i;
				
		}
	}

	return -1;


}

//-------------------------------------

void TransferFunction::MovePressedPoint(int Nr, int NewX, int NewY,int SizeX,int SizeY)
{

	int NrOfPoints=TransferPointsList->GetNumberOfItems();
	if ((Nr>=0)&&(Nr<NrOfPoints))
	{

	//	int		TmpXPrev,TmpXNext;
		double		TmpXPrev,TmpXNext;
		
	
// dla pierwszego i ostatniego inaczej
		double newX=(100.0*NewX)/(double)(SizeX-1);
		double newY=(100.0*NewY)/(double)(SizeY-1);

		if (Nr==0)
		{
		
		}
		else if (Nr==NrOfPoints-1)
		{

		}
		else
		{

		//	TmpXPrev=Round(((SizeX-1)*(TransferPointsList->GetItemAtPos(Nr-1)->GetData()->X))/100.0); //to we wspolrzednych ekranowych
		//	TmpXNext=Round(((SizeX-1)*(TransferPointsList->GetItemAtPos(Nr+1)->GetData()->X))/100.0); //to we wspolrzednych ekranowych
			TmpXPrev=TransferPointsList->GetItemAtPos(Nr-1)->GetData()->X;
			TmpXNext=TransferPointsList->GetItemAtPos(Nr+1)->GetData()->X;
			
			if (((newX>TmpXPrev)&&(newX<TmpXNext)))
			{
				//newX=TransferPointsList->GetItemAtPos(Nr)->GetData()->X;
				TransferPointsList->GetItemAtPos(Nr)->GetData()->X=newX;
		
			}


		}

		if ((newY>=0)&&(newY<=100))
			TransferPointsList->GetItemAtPos(Nr)->GetData()->Y=newY;
		else if (newY<0)
			TransferPointsList->GetItemAtPos(Nr)->GetData()->Y=0.0;
		else
			TransferPointsList->GetItemAtPos(Nr)->GetData()->Y=100.0;


			


		

		//	TransferPointsList->GetItemAtPos(Nr)->GetData()->X=newX;
		
	}


}

//-------------------------------------

void TransferFunction::InsertPoint(int Pos,int X, int Y,int SizeX,int SizeY,unsigned char *Color)
{

	TransferFunctionPoint *Point=new TransferFunctionPoint();
	Point->X=(100.0*X)/(double)(SizeX-1);
	Point->Y=(100.0*Y)/(double)(SizeY-1);
	Point->Color=Color;


	
	
	
	TransferPointsList->InsertItem(Pos,Point);

}

//-------------------------------------

void TransferFunction::GetPositionOfNewPoint(int X, int Y,int SizeX,int SizeY)
{
	
}
