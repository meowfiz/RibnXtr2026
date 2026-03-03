#ifndef	TRANSFER_FUNCTION
#define	TRANSFER_FUNCTION


#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"LinkedList.h"
#include	"OtherCommonFunctions.h"

class TransferFunctionPoint
{
	public:

		TransferFunctionPoint(double _X=0,double _Y=0,unsigned char *_Color=NULL);
		~TransferFunctionPoint();


		double X;				// (0-100%)
		double Y;				// (0-100%)
		unsigned char *Color; // (R,G,B);

	

};

class TransferFunction
{
	public:

		TransferFunction();
		~TransferFunction();



		TransferFunctionPoint* GetPoint(int Pos);	
	
		void	InsertPoint(int Pos,TransferFunctionPoint* Item );
		void	InsertPoint(int Pos,int X, int Y,int SizeX,int SizeY,unsigned char *Color=NULL);//ni

		void	GetPositionOfNewPoint(int X, int Y,int SizeX,int SizeY);//ni 
		
		void	DeletePoint(int Pos);


		void	DeleteAllPoints();

		int		GetNumberOfPoints();

		int		GetNumberOfPressedPoint(int X, int Y,int SizeX,int SizeY,int Distance);
		void	MovePressedPoint(int Nr, int NewX, int NewY,int SizeX,int SizeY);
		
		
		
		LinkedList<TransferFunctionPoint*> *TransferPointsList; 

	//	int NumberOfPoints;




};




#endif //TRANSFER_FUNCTION
