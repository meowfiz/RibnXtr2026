
#ifndef LINKED_LIST
#define LINKED_LIST


template <class LISTDATA>
class ListItem
{
public:
	ListItem();
	~ListItem();

	void	SetData(LISTDATA data){ Data = data; };
	LISTDATA	GetData(){ return Data; };

	ListItem<LISTDATA>   *NextListItem;
	ListItem<LISTDATA>   *PreviousListItem;

private:
	LISTDATA	Data;


};

//-----------------------------------

template <class LISTDATA>
class   LinkedList
{

public:

	LinkedList();
	~LinkedList();

	bool	LastDeleted;

	void	InsertItem(LISTDATA _Data); //done
	void	InsertItem(int pos, LISTDATA _Data); //done not tested

	void	SetNextItem(); //done
	void	SetPreviousItem(); //done

	void	SetCurrentItem(int pos); //done
	int		GetCurrentItemPos(); //done


	void	DeleteItem(); //done
	void	DeleteItem(int pos); //done not tested
	void	DeleteAllItems(); //done not tested
	void	DeleteAllItemsAfterCurrent(); //done not tested
	void	DeleteAllItemsBeforeCurrent(); //done not tested

	ListItem<LISTDATA>	*GetHeadListItem();
	ListItem<LISTDATA>	*GetTailListItem();
	ListItem<LISTDATA>	*GetCurrentListItem();
	void SetCurrentListItem(ListItem<LISTDATA> *Item);


	int	GetNumberOfItems();
	void	MoveItemUp();
	void	MoveItemDown();

	ListItem<LISTDATA> *GetItemAtPos(int pos); //not tested

private:

	int NumberOfItems;

	ListItem<LISTDATA>	*HeadListItem;
	ListItem<LISTDATA>	*TailListItem;
	ListItem<LISTDATA>	*CurrentListItem;


};

template <class LISTDATA>
ListItem<LISTDATA>::ListItem()
{

}

//---------------------------------

template <class LISTDATA>
ListItem<LISTDATA>::~ListItem()
{
	NextListItem = NULL;
	PreviousListItem = NULL;
	if (Data != NULL) delete Data;
	Data = NULL;

}
//------------------------------------------

template <class LISTDATA>
LinkedList<LISTDATA>::LinkedList()
{
	HeadListItem = (ListItem<LISTDATA>*)0;
	TailListItem = (ListItem<LISTDATA>*)0;
	CurrentListItem = (ListItem<LISTDATA>*)0;
	NumberOfItems = 0;
	LastDeleted = 0;

}

//---------------------------------------

template <class LISTDATA>
LinkedList<LISTDATA>::~LinkedList()
{

	DeleteAllItems();

}

//---------------------------------------
template <class LISTDATA>
void LinkedList<LISTDATA>::InsertItem(LISTDATA _Data)
{



	//forward insert
	ListItem<LISTDATA>* TempItem = new ListItem<LISTDATA>;
	TempItem->SetData(_Data);

	NumberOfItems++;

	if (HeadListItem == (ListItem<LISTDATA>*)0)
	{
		TempItem->NextListItem = TempItem;
		TempItem->PreviousListItem = TempItem;
		HeadListItem = TempItem;
		TailListItem = TempItem;
		CurrentListItem = TempItem;


		return;
	}


	if (CurrentListItem == TailListItem)
	{
		TempItem->NextListItem = TempItem;
		TailListItem = TempItem;

	}
	else
	{
		TempItem->NextListItem = CurrentListItem->NextListItem;
		CurrentListItem->NextListItem->PreviousListItem = TempItem;
	}

	ListItem<LISTDATA> *TempItem2 = CurrentListItem;
	CurrentListItem = TempItem;

	TempItem2->NextListItem = TempItem;

	TempItem->PreviousListItem = TempItem2;







}

//---------------------------------------
template <class LISTDATA>
void LinkedList<LISTDATA>::InsertItem(int pos, LISTDATA _Data)
{
	SetCurrentItem(pos);
	InsertItem(_Data);

}
//---------------------------------------
template <class LISTDATA>
void LinkedList<LISTDATA>::DeleteItem()
{

	if (NumberOfItems>0)
	{
		LastDeleted = 0;

		NumberOfItems--;

		if (NumberOfItems == 0)
		{

			delete CurrentListItem;
			HeadListItem = (ListItem<LISTDATA>*)0;
			TailListItem = (ListItem<LISTDATA>*)0;
			CurrentListItem = (ListItem<LISTDATA>*)0;


			return;
		}

		ListItem<LISTDATA> *TempItem = CurrentListItem;

		if (TempItem != HeadListItem)
			TempItem->PreviousListItem->NextListItem = TempItem->NextListItem;
		else
		{
			HeadListItem = TempItem->NextListItem;
			CurrentListItem = HeadListItem;
			CurrentListItem->PreviousListItem = CurrentListItem;
			delete TempItem;
			return;

		}
		if (CurrentListItem != TailListItem)
		{
			CurrentListItem->NextListItem->PreviousListItem = TempItem->PreviousListItem;
			CurrentListItem = TempItem->NextListItem;

			delete TempItem;
		}
		else
		{
			CurrentListItem = TempItem->PreviousListItem;
			CurrentListItem->NextListItem = CurrentListItem;
			TailListItem = CurrentListItem;
			delete TempItem;

			LastDeleted = 1;

		}



	}



}
//---------------------------------------
template <class LISTDATA>
void LinkedList<LISTDATA>::DeleteItem(int pos)
{

	ListItem<LISTDATA> *TempItem = CurrentListItem;

	SetCurrentItem(pos);


	if (TempItem == CurrentListItem)
	{
		DeleteItem();
	}
	else
	{

		DeleteItem();
		CurrentListItem = TempItem;

	}


}
//---------------------------------------
template <class LISTDATA>
void LinkedList<LISTDATA>::DeleteAllItems()
{

	while (CurrentListItem != (ListItem<LISTDATA>*)0)
		DeleteItem();




}


//---------------------------------------
template <class LISTDATA>
void LinkedList<LISTDATA>::SetNextItem()
{
	//	if (CurrentListItem->NextListItem!=(ListItem<LISTDATA>*)0)
	CurrentListItem = CurrentListItem->NextListItem;

}


//---------------------------------------
template <class LISTDATA>
void LinkedList<LISTDATA>::SetPreviousItem()
{
	//if (CurrentListItem->PreviousListItem!=(ListItem<LISTDATA>*)0)
	CurrentListItem = CurrentListItem->PreviousListItem;

}

//---------------------------------------


template <class LISTDATA>
void LinkedList<LISTDATA>::SetCurrentItem(int pos)
{

	ListItem<LISTDATA> *TempItem = HeadListItem;
	int Tpos = 0;

	if (pos<NumberOfItems)
	{
		while (Tpos<pos)
		{
			TempItem = TempItem->NextListItem;
			Tpos++;
		}

		CurrentListItem = TempItem;
	}
	else
	{
		CurrentListItem = TailListItem;
	}


}

//---------------------------------------


template <class LISTDATA>
int LinkedList<LISTDATA>::GetCurrentItemPos()
{

	ListItem<LISTDATA> *TempItem = CurrentListItem;
	int Tpos = 0;

	//	Do_opa=fopen("c:\\tab2x","a");fprintf(Do_opa,"nic3\n");fclose(Do_opa);
	if (TempItem != NULL)
	while ((TempItem->PreviousListItem != TempItem))
	{
		TempItem = TempItem->PreviousListItem;
		Tpos++;
	}

	return Tpos;

}

//---------------------------------------


template <class LISTDATA>
void LinkedList<LISTDATA>::DeleteAllItemsAfterCurrent()
{


	int TEMP = 0;
	CurrentListItem = CurrentListItem->NextListItem;

	while (TEMP == 0)
	{

		if (CurrentListItem == TailListItem)
			TEMP = 1;

		DeleteItem();
	}



}

//---------------------------------------

template <class LISTDATA>
void LinkedList<LISTDATA>::DeleteAllItemsBeforeCurrent()
{

	// to pewnie jeszcze nie dziala!!!

	CurrentListItem = CurrentListItem->PreviousListItem;



	while (CurrentListItem != HeadListItem)
	{
		DeleteItem();
		if (CurrentListItem != TailListItem)	CurrentListItem = CurrentListItem->PreviousListItem;

	}



}

//---------------------------------------


template <class LISTDATA>
ListItem<LISTDATA> *LinkedList<LISTDATA>::GetItemAtPos(int pos)
{

	ListItem<LISTDATA> *TempItem = CurrentListItem;

	SetCurrentItem(pos);

	ListItem<LISTDATA> *TempItem2 = CurrentListItem;

	CurrentListItem = TempItem;

	return TempItem2;

}



template <class LISTDATA>
void LinkedList<LISTDATA>::MoveItemUp()
{

	if (CurrentListItem != HeadListItem)
	{



		ListItem<LISTDATA>	*C = CurrentListItem;
		ListItem<LISTDATA>	*B = C->PreviousListItem;
		ListItem<LISTDATA>	*A = B->PreviousListItem;//
		ListItem<LISTDATA>	*D = C->NextListItem;//

		C->PreviousListItem = (B != A) ? A : C;
		C->NextListItem = B;

		B->PreviousListItem = C;
		B->NextListItem = (C != D) ? D : B;

		if (B != A)
			A->NextListItem = C;
		else
			HeadListItem = C;

		if (C != D)
			D->PreviousListItem = B;

		if (C == TailListItem) TailListItem = B; //2004.04.28


	}



}

template <class LISTDATA>
void LinkedList<LISTDATA>::MoveItemDown()
{


	if (CurrentListItem != TailListItem)
	{

		ListItem<LISTDATA>	*B = CurrentListItem;
		ListItem<LISTDATA>	*C = B->NextListItem;
		ListItem<LISTDATA>	*D = C->NextListItem;//
		ListItem<LISTDATA>	*A = B->PreviousListItem;//




		B->NextListItem = (C != D) ? D : B;
		B->PreviousListItem = C;


		C->NextListItem = B;
		C->PreviousListItem = (B != A) ? A : C;

		if (C != D)
			D->PreviousListItem = B;

		if (B != A)
			A->NextListItem = C;
		else
			HeadListItem = C;


		if (C == TailListItem) TailListItem = B; //2004.04.28

	}


}

//-------------------------------

template <class LISTDATA>
int LinkedList<LISTDATA>::GetNumberOfItems()
{
	return NumberOfItems;
}

//-------------------------------

template <class LISTDATA>
ListItem<LISTDATA> *LinkedList<LISTDATA>::GetHeadListItem()
{
	return HeadListItem;
}

//-------------------------------

template <class LISTDATA>
ListItem<LISTDATA> *LinkedList<LISTDATA>::GetTailListItem()
{
	return TailListItem;
}

//-------------------------------

template <class LISTDATA>
ListItem<LISTDATA> *LinkedList<LISTDATA>::GetCurrentListItem()
{
	return CurrentListItem;
}

//---------------------------------
//-------------------------------
/*
template <class LISTDATA>
void LinkedList<LISTDATA>::SetHeadListItem(ListItem<LISTDATA> *Item)
{
HeadListItem
}

//-------------------------------

template <class LISTDATA>
ListItem<LISTDATA> *LinkedList<LISTDATA>::GetTailListItem()
{
return TailListItem;
}

//-------------------------------
*/
template <class LISTDATA>
void LinkedList<LISTDATA>::SetCurrentListItem(ListItem<LISTDATA> *Item)
{
	CurrentListItem = Item;
}

//---------------------------------
#endif //LINKED_LIST


