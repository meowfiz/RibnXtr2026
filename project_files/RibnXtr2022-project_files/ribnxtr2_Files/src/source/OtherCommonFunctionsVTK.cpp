
#include "OtherCommonFunctionsVTK.h"
double	GetAvgDistance(vtkPolyData *polys)
{
	 
	vtkPoints *pts = polys->GetPoints();
	int nrPts = pts->GetNumberOfPoints();


	double length, dist;
	int pos, pos1;
	double p0[3], p1[3], p2[3], p[3];

	int val1, val2, diff;


	vtkIdList *pointList = vtkIdList::New();
	vtkIdList *cells = vtkIdList::New();
	int i, j, k, l;
	int tempNr;
	int *tempTab;

	int nrrr;
	vtkIdType cellId, np, np2;

	long double distance = 0;
	int count = 0;

	for (i = 0; i<nrPts; i++)
	{
		polys->GetPointCells(i, cells);
		nrrr = cells->GetNumberOfIds();
		pts->GetPoint(i, p);


		for (j = 0; j<nrrr; j++)
		{
			cellId = cells->GetId(j);
			np2 = polys->GetCellType(cellId);
			if (np2 == VTK_TRIANGLE)
			{
				polys->GetCellPoints(cellId, pointList);
				for (k = 0; k<pointList->GetNumberOfIds(); k++)
				{
					np = pointList->GetId(k);
					if (np == i) continue;
					pts->GetPoint(np, p1);
					count++;
					distance += GetDistance(p1, p);
				}

			}

		}
	}
	distance /= (double)count;
	return (double)distance;
}
//-------------------------------------------------
double	GetMedDistance(vtkPolyData *polys)
{

	vtkPoints *pts = polys->GetPoints();
	int nrPts = pts->GetNumberOfPoints();


	double length, dist;
	int pos, pos1;
	double p0[3], p1[3], p2[3], p[3];

	int val1, val2, diff;


	vtkIdList *pointList = vtkIdList::New();
	vtkIdList *cells = vtkIdList::New();

	int i, j, k, l;
	int tempNr;
	int *tempTab;

	int nrrr;
	vtkIdType cellId, np, np2;

	long double distance = 0;
	int count = 0;

	double *medTab = new double[nrPts * 20];
	for (i = 0; i<nrPts; i++)
	{
		polys->GetPointCells(i, cells);
		nrrr = cells->GetNumberOfIds();
		pts->GetPoint(i, p);


		for (j = 0; j<nrrr; j++)
		{
			cellId = cells->GetId(j);
			np2 = polys->GetCellType(cellId);
			if (np2 == VTK_TRIANGLE)
			{
				polys->GetCellPoints(cellId, pointList);
				for (k = 0; k<pointList->GetNumberOfIds(); k++)
				{
					np = pointList->GetId(k);
					if (np == i) continue;
					pts->GetPoint(np, p1);

					//distance+=GetDistance(p1,p);
					medTab[count++] = GetDistance(p1, p);
				}

			}

		}
	}
	double median = Median(medTab, count);

	return median;
}
//--------------------------------------------------
int	DistanceRefineTrianglePolyDataFilter(vtkPolyData *pData, double dist)
{

	int i, j, k;
	int ptId;
	vtkIdType newCellPts[3];
	vtkIdType cellId;
	vtkIdList *pointList = vtkIdList::New();
	vtkIdList *cellPts = vtkIdList::New();
	vtkIdList *divisionCellIds;//=vtkIdList::New();
	vtkIdType divCellId[2];
	vtkIdType p0, p1, p2;


	vtkEdgeTable *edgeTable;

	vtkIdType np, np2, type;
	unsigned short ncells;
	double len;


	double point0[3], point1[3], point2[3];
	int edgeCount;
	int cc;
	int nrr;//=pData->GetNumberOfCells();
	int numStrips;
	vtkPoints *pts;//=pData->GetPoints();
	int nrOfPoints;//=pts->GetNumberOfPoints();
	vtkPoints *newPts;//=vtkPoints::New();

	vtkIdType tri[3], tri1[3], tri2[3];

	bool anyChg;

	edgeCount = 0;
	do
	{
		vtkCellArray *triangleCells = vtkCellArray::New();
		anyChg = false;
		divisionCellIds = vtkIdList::New();
		newPts = vtkPoints::New();
		nrr = pData->GetNumberOfCells();
		pts = pData->GetPoints();
		newPts->DeepCopy(pts);
		nrOfPoints = pts->GetNumberOfPoints();
		edgeTable = vtkEdgeTable::New();
		edgeTable->InitEdgeInsertion(pts->GetNumberOfPoints(), 1);

		numStrips = pData->GetNumberOfStrips();


		// stworzyc liste krawedzi
		for (i = 0; i<nrr; i++)
		{
			type = pData->GetCellType(i);

			if (type == VTK_TRIANGLE)
			{
				pData->GetCellPoints(i, pointList);

				for (j = 0; j<3; j++)
				{
					switch (j)
					{
					case 0:	p0 = pointList->GetId(0);	p1 = pointList->GetId(1);		break;
					case 1:	p0 = pointList->GetId(1);	p1 = pointList->GetId(2);		break;
					case 2:	p0 = pointList->GetId(2);	p1 = pointList->GetId(0);		break;
					};

					if (edgeTable->IsEdge(p0, p1) == -1)
					{
						edgeTable->InsertEdge(p0, p1);
						//jeszcze sprawdze dlugosc i jesli mniejsza od dist to wstawie punkt do listy
						len = GetDistance(pts, p0, p1);
						if (len>dist)
						{
							anyChg = true;
							GetLinearInterpolatedPoint(pts, point0, p0, p1, 0.5);
							// dodaje do nowych pktow					
							newPts->InsertNextPoint(point0);

							//dodac na liste
							// divCellId[0]=edgeCount++;
							// divCellId[1]=newPts->GetNumberOfPoints()-1;
							divisionCellIds->InsertNextId(nrOfPoints++);
							// divisionCells->InsertNextCell(2,divCellId);
							edgeCount++;
						}
						else
						{
							divisionCellIds->InsertNextId(0);
						}
					}
				}


			}

		}


		// dla kazdej krawedzi przypisac info czy dzielona czy nie (tablica z indeksem punktu podzialu), 0 - brak podzialu


		for (i = 0; i<nrr; i++)
		{
			type = pData->GetCellType(i);

			if (type == VTK_TRIANGLE)
			{
				pData->GetCellPoints(i, pointList);
				p0 = pointList->GetId(0);
				p1 = pointList->GetId(1);
				p2 = pointList->GetId(2);
				vtkIdType d0, d1, d2;
				vtkIdType edgeIds[3];

				edgeIds[0] = edgeTable->IsEdge(p0, p1);
				edgeIds[1] = edgeTable->IsEdge(p1, p2);
				edgeIds[2] = edgeTable->IsEdge(p2, p0);

				d0 = divisionCellIds->GetId(edgeIds[0]);
				d1 = divisionCellIds->GetId(edgeIds[1]);
				d2 = divisionCellIds->GetId(edgeIds[2]);


				cc = 0;
				if (d0>0) cc++; if (d1>0) cc++; if (d2>0) cc++;
				//sprawdzam case'y
				switch (cc)
				{
					// 0 - robie 1 triangle cell 0-1-2 i dodaje
				case 0:
					tri[0] = p0; tri[1] = p1; tri[2] = p2;
					triangleCells->InsertNextCell(3, tri);
					break;
				case 3:
					// 3 - robie 4 trojkaty
					tri[0] = p0; tri[1] = d0; tri[2] = d2;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d0; tri[1] = p1; tri[2] = d1;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d1; tri[1] = p2; tri[2] = d2;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d0; tri[1] = d1; tri[2] = d2;
					triangleCells->InsertNextCell(3, tri);
					break;


					// 2 - 3 trojkaty
				case 2:
					if ((d0>0) && (d2>0))
					{
						tri[0] = p0; tri[1] = d0; tri[2] = d2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d0; tri[1] = p2; tri[2] = d2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d0; tri[1] = p1; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
					}
					else
						if ((d1>0) && (d2>0))
						{
							tri[0] = p0; tri[1] = p1; tri[2] = d2;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = p1; tri[1] = d1; tri[2] = d2;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = d2; tri[1] = d1; tri[2] = p2;
							triangleCells->InsertNextCell(3, tri);

						}
						else
							//if ((d2>0)&&(d2>0))
						{
							tri[0] = p0; tri[1] = d1; tri[2] = p2;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = p0; tri[1] = d0; tri[2] = d1;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = d0; tri[1] = p1; tri[2] = d1;
							triangleCells->InsertNextCell(3, tri);

						}




					break;
					// 1 - 2 trojkaty
				case 1:

					if (d0>0)
					{
						tri[0] = p0; tri[1] = d0; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d0; tri[1] = p1; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
					}
					else
						if (d1>0)
						{
							tri[0] = p0; tri[1] = p1; tri[2] = d1;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = p0; tri[1] = d1; tri[2] = p2;
							triangleCells->InsertNextCell(3, tri);
						}
						else
						{
							tri[0] = p0; tri[1] = p1; tri[2] = d2;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = d2; tri[1] = p1; tri[2] = p2;
							triangleCells->InsertNextCell(3, tri);
						}



					break;
				}

			}

		}
		divisionCellIds->Delete();
		edgeTable->Delete();
		vtkPolyData *output = vtkPolyData::New();
		output->SetPoints(newPts);
		output->SetPolys(triangleCells);
		output->BuildCells();
		output->BuildLinks();
		pData->DeepCopy(output);
		//	//pData->Update();
		output->Delete();
	} while (anyChg == true);

	// generuje nowe dane
	// sprawdzam kazdy cell - sprawdzam krawedzie
	// mam 3 punkty dla celu i punkty dla danej krawedzi - jesli sa
	// jesli punktow podzialu brak - wstawiam oryginalny trojkato do nowych danych
	// jesli 3 puntky podzialu - powstanie 4 trojkaty
	// jesli 1 powstana 2 - podzial z wierzcholkiem nie nalezacym do krawedzi
	// jesli 2 - powstana 3 trojkaty - ...


	return edgeCount;



	//return output;

	//	pData->Delete();
	//	pData=output;
}
//---------------------------------------------
void ConstrainedPolyDataSmoothing(vtkPolyData *&polys, int *smoothTab, int nrOfSmoothPts, int iter, double factor)
{
	vtkPoints *pts1 = polys->GetPoints();
	int nrPts = pts1->GetNumberOfPoints();

	vtkPoints *pts2 = vtkPoints::New();
	pts2->DeepCopy(pts1);
	vtkPoints *pts3 = vtkPoints::New();
	pts3->DeepCopy(pts1);

	vtkIdList *pointList = vtkIdList::New();
	vtkIdList *cells = vtkIdList::New();
	int i, j, k, l;
	int tempNr;
	double dist;
	double pS[3];
	double p[3], p1[3];
	int mID;

	int nrrr;
	vtkIdType cellId, np, np2;

	for (l = 0; l<iter; l++)
	{
		for (i = 0; i<nrOfSmoothPts; i++)
		{
			if (smoothTab[i]<nrPts)
			{
				polys->GetPointCells(smoothTab[i], cells);
				nrrr = cells->GetNumberOfIds();

				//p[0]=0;	p[1]=0;	p[2]=0;
				pts2->GetPoint(smoothTab[i], p);
				tempNr = 1;
				for (j = 0; j<nrrr; j++)
				{
					cellId = cells->GetId(j);
					np2 = polys->GetCellType(cellId);
					if (np2 == VTK_TRIANGLE)
					{
						polys->GetCellPoints(cellId, pointList);
						for (k = 0; k<pointList->GetNumberOfIds(); k++)
						{
							np = pointList->GetId(k);
							//if (np==i) continue;
							pts2->GetPoint(np, p1);
							p[0] += p1[0];
							p[1] += p1[1];
							p[2] += p1[2];
							tempNr++;
						}

					}
				}
				p[0] /= tempNr;
				p[1] /= tempNr;
				p[2] /= tempNr;

				pts3->SetPoint(smoothTab[i], p);
			}
		}
		pts2->DeepCopy(pts3);
	}
	pts1->DeepCopy(pts2);
	//polys->Update();
	pts2->Delete();
	pts3->Delete();
	pointList->Delete();
	cells->Delete();
	//ComputeNormalsPolyData(polys);


}
double GetDistance(vtkPoints *pts, int a, int b)
{
	double p1[3], p2[3];
	pts->GetPoint(a, p1);
	pts->GetPoint(b, p2);
	double d[3];
	d[0] = p1[0] - p2[0];
	d[1] = p1[1] - p2[1];
	d[2] = p1[2] - p2[2];

	double dist = (d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
	if (dist>0) dist = sqrt(dist);
	return dist;

}
//------------------------------
void GetLinearInterpolatedPoint(vtkPoints *pts, double *p, int a, int b, double alpha)
{
	double p1[3], p2[3];
	pts->GetPoint(a, p1);
	pts->GetPoint(b, p2);

	p[0] = p2[0] * alpha + p1[0] * (1 - alpha);
	p[1] = p2[1] * alpha + p1[1] * (1 - alpha);
	p[2] = p2[2] * alpha + p1[2] * (1 - alpha);

}
//----------------------------------
vtkPolyData *IncreaseDensityInLinePolyData(vtkPolyData *pData, double minDist)
{
	int i, j, k;
	vtkPolyData *newPData = vtkPolyData::New();
	vtkPoints *newPts = vtkPoints::New();
	vtkCellArray *lineCells = vtkCellArray::New();
	vtkIdType line[2];

	int ptId;
	vtkIdType newCellPts[3];
	vtkIdType cellId;
	vtkIdList *pointList = vtkIdList::New();
	vtkIdList *cellPts = vtkIdList::New();
	vtkIdType divCellId[2];
	vtkIdType p0, p1, p2;
	vtkIdType id1, id2;
	vtkIdType type;
	double point0[3], point1[3];
	double pointT0[3], pointT1[3];

	int nrOfSeg;
	double dist;
	double alpha;
	int nrr = pData->GetNumberOfCells();
	vtkPoints *pts = pData->GetPoints();
	int aa = pts->GetNumberOfPoints();
	minDist=(minDist > 0)?  1.0 / minDist:1;
	for (i = 0; i<nrr; i++)
	{
		type = pData->GetCellType(i);

		if (type == VTK_LINE)
		{
			pData->GetCellPoints(i, pointList);
			p0 = pointList->GetId(0);	p1 = pointList->GetId(1);
			pts->GetPoint(p0, point0);
			pts->GetPoint(p1, point1);
			dist = GetDistance(pts, p0, p1);
			if (dist<minDist)
			{

				id1 = newPts->InsertNextPoint(point0);
				id2 = newPts->InsertNextPoint(point1);
				newCellPts[0] = id1;	newCellPts[1] = id2;
				lineCells->InsertNextCell(2, newCellPts);
			}
			else
			{
				nrOfSeg = (int)(dist * minDist);// (int)(dist + 0.5 / minDist) + 1; d
				if (nrOfSeg>0)
				{
					for (j = 0; j<nrOfSeg + 1; j++)
					{
						alpha = j / (double)nrOfSeg;
						GetLinearInterpolatedPoint(pts, point0, p0, p1, alpha);
						id1 = newPts->InsertNextPoint(point0);
						if (j>0)
						{
							newCellPts[0] = id1 - 1;
							newCellPts[1] = id1;
							lineCells->InsertNextCell(2, newCellPts);
						}
					}
				}
			}

		}
	}
	vtkSmartPointer<vtkPolyData> tmpData = vtkSmartPointer<vtkPolyData>::New();
	//vtkPolyData *tmpData = vtkPolyData::New();
	tmpData->SetPoints(newPts);
	tmpData->SetLines(lineCells);
	tmpData->BuildCells();
	tmpData->BuildLinks();



	vtkSmartPointer<vtkCleanPolyData>Clean2 = vtkSmartPointer<vtkCleanPolyData>::New();
	Clean2->SetInputData(tmpData);
	Clean2->PointMergingOn();
	Clean2->ConvertLinesToPointsOn();
	Clean2->SetConvertPolysToLines(true);
	Clean2->SetTolerance(1e-3);
	Clean2->Update();






	//CleanPolyData(tmpData);
	vtkPolyData *output = vtkPolyData::New();
	output->DeepCopy(Clean2->GetOutput());
	pointList->Delete();
	cellPts->Delete();


	//int delLines=CleanPolyDataDuplicatedLines(output);


	return output;


}

//--------------------------------------------------------
vtkPolyData *DoubleLinesInLinePolyData(vtkPolyData *pData)
{
	int i, j, k;
	int ptId;
	vtkIdType newCellPts[3];
	vtkIdType cellId;
	vtkIdList *pointList = vtkIdList::New();
	vtkIdList *cellPts = vtkIdList::New();
	vtkIdList *divisionCellIds = vtkIdList::New();
	vtkIdType divCellId[2];
	vtkIdType p0, p1, p2;


	vtkEdgeTable *edgeTable;
	edgeTable = vtkEdgeTable::New();
	vtkIdType np, np2, type;
	unsigned short ncells;
	double len;


	double point0[3], point1[3], point2[3];

	int nrr = pData->GetNumberOfCells();

	vtkPoints *pts = pData->GetPoints();
	int nrOfPoints = pts->GetNumberOfPoints();
	vtkPoints *newPts = vtkPoints::New();
	newPts->DeepCopy(pts);
	edgeTable->InitEdgeInsertion(pts->GetNumberOfPoints(), 1);

	int numStrips = pData->GetNumberOfStrips();

	int edgeCount = 0;
	// stworzyc liste krawedzi
	for (i = 0; i<nrr; i++)
	{
		type = pData->GetCellType(i);

		if (type == VTK_LINE)
		{
			pData->GetCellPoints(i, pointList);
			p0 = pointList->GetId(0);	p1 = pointList->GetId(1);

			if ((p0 == 121) || (p1 == 121))
			{
				i = i;
			}
			if (edgeTable->IsEdge(p0, p1) == -1)
			{
				edgeTable->InsertEdge(p0, p1);
				GetLinearInterpolatedPoint(pts, point0, p0, p1, 0.5);
				// dodaje do nowych pktow					
				newPts->InsertNextPoint(point0);


				divisionCellIds->InsertNextId(nrOfPoints++);
				// divisionCells->InsertNextCell(2,divCellId);
				edgeCount++;
			}
			else
			{
				divisionCellIds->InsertNextId(0);
			}
		}



	}

	vtkCellArray *lineCells = vtkCellArray::New();
	vtkIdType line[2];

	for (i = 0; i<nrr; i++)
	{
		type = pData->GetCellType(i);

		if (type == VTK_LINE)
		{
			pData->GetCellPoints(i, pointList);
			p0 = pointList->GetId(0);
			p1 = pointList->GetId(1);
			vtkIdType d0, d1, d2;
			vtkIdType edgeIds[3];

			edgeIds[0] = edgeTable->IsEdge(p0, p1);


			d0 = divisionCellIds->GetId(edgeIds[0]);

			if (d0>0)
			{
				line[0] = p0;	line[1] = d0;
				lineCells->InsertNextCell(2, line);
				line[0] = d0;	line[1] = p1;
				lineCells->InsertNextCell(2, line);
			}
			else
			{
				line[0] = p0;	line[1] = p1;
				lineCells->InsertNextCell(2, line);

			}
		}
	}
	vtkPolyData *output = vtkPolyData::New();
	output->SetPoints(newPts);
	output->SetLines(lineCells);
	output->BuildCells();
	output->BuildLinks();


	divisionCellIds->Delete();
	edgeTable->Delete();

	return output;


}
//---------------------------------------
vtkPolyData	*DistanceRefinePolyDataFilter(vtkPolyData *pData, double dist)
{

	int i, j, k;
	int ptId;
	vtkIdType newCellPts[3];
	vtkIdType cellId;
	vtkIdList *pointList = vtkIdList::New();
	vtkIdList *cellPts = vtkIdList::New();
	vtkIdList *divisionCellIds = vtkIdList::New();
	vtkIdType divCellId[2];
	vtkIdType p0, p1, p2;


	vtkEdgeTable *edgeTable;
	edgeTable = vtkEdgeTable::New();
	vtkIdType np, np2, type;
	unsigned short ncells;
	double len;


	double point0[3], point1[3], point2[3];

	int nrr = pData->GetNumberOfCells();

	vtkPoints *pts = pData->GetPoints();
	int nrOfPoints = pts->GetNumberOfPoints();
	vtkPoints *newPts = vtkPoints::New();
	newPts->DeepCopy(pts);
	edgeTable->InitEdgeInsertion(pts->GetNumberOfPoints(), 1);

	int numStrips = pData->GetNumberOfStrips();

	int edgeCount = 0;
	// stworzyc liste krawedzi
	for (i = 0; i<nrr; i++)
	{
		type = pData->GetCellType(i);

		if (type == VTK_TRIANGLE)
		{
			pData->GetCellPoints(i, pointList);

			for (j = 0; j<3; j++)
			{
				switch (j)
				{
				case 0:	p0 = pointList->GetId(0);	p1 = pointList->GetId(1);		break;
				case 1:	p0 = pointList->GetId(1);	p1 = pointList->GetId(2);		break;
				case 2:	p0 = pointList->GetId(2);	p1 = pointList->GetId(0);		break;
				};

				if (edgeTable->IsEdge(p0, p1) == -1)
				{
					edgeTable->InsertEdge(p0, p1);
					//jeszcze sprawdze dlugosc i jesli mniejsza od dist to wstawie punkt do listy
					len = GetDistance(pts, p0, p1);
					if (len>dist)
					{
						GetLinearInterpolatedPoint(pts, point0, p0, p1, 0.5);
						// dodaje do nowych pktow					
						newPts->InsertNextPoint(point0);

						//dodac na liste
						// divCellId[0]=edgeCount++;
						// divCellId[1]=newPts->GetNumberOfPoints()-1;
						divisionCellIds->InsertNextId(nrOfPoints++);
						// divisionCells->InsertNextCell(2,divCellId);
						edgeCount++;
					}
					else
					{
						divisionCellIds->InsertNextId(0);
					}
				}
			}
		}
	}


	// dla kazdej krawedzi przypisac info czy dzielona czy nie (tablica z indeksem punktu podzialu), 0 - brak podzialu
	vtkCellArray *triangleCells = vtkCellArray::New();
	vtkIdType tri[3], tri1[3], tri2[3];

	for (i = 0; i<nrr; i++)
	{
		type = pData->GetCellType(i);

		if (type == VTK_TRIANGLE)
		{
			pData->GetCellPoints(i, pointList);
			p0 = pointList->GetId(0);
			p1 = pointList->GetId(1);
			p2 = pointList->GetId(2);
			vtkIdType d0, d1, d2;
			vtkIdType edgeIds[3];

			edgeIds[0] = edgeTable->IsEdge(p0, p1);
			edgeIds[1] = edgeTable->IsEdge(p1, p2);
			edgeIds[2] = edgeTable->IsEdge(p2, p0);

			d0 = divisionCellIds->GetId(edgeIds[0]);
			d1 = divisionCellIds->GetId(edgeIds[1]);
			d2 = divisionCellIds->GetId(edgeIds[2]);


			int cc = 0;
			if (d0>0) cc++; if (d1>0) cc++; if (d2>0) cc++;
			//sprawdzam case'y
			switch (cc)
			{
				// 0 - robie 1 triangle cell 0-1-2 i dodaje
			case 0:
				tri[0] = p0; tri[1] = p1; tri[2] = p2;
				triangleCells->InsertNextCell(3, tri);
				break;
			case 3:
				// 3 - robie 4 trojkaty
				tri[0] = p0; tri[1] = d0; tri[2] = d2;
				triangleCells->InsertNextCell(3, tri);
				tri[0] = d0; tri[1] = p1; tri[2] = d1;
				triangleCells->InsertNextCell(3, tri);
				tri[0] = d1; tri[1] = p2; tri[2] = d2;
				triangleCells->InsertNextCell(3, tri);
				tri[0] = d0; tri[1] = d1; tri[2] = d2;
				triangleCells->InsertNextCell(3, tri);
				break;


				// 2 - 3 trojkaty
			case 2:
				if ((d0>0) && (d2>0))
				{
					tri[0] = p0; tri[1] = d0; tri[2] = d2;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d0; tri[1] = p2; tri[2] = d2;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d0; tri[1] = p1; tri[2] = p2;
					triangleCells->InsertNextCell(3, tri);
				}
				else
					if ((d1>0) && (d2>0))
					{
						tri[0] = p0; tri[1] = p1; tri[2] = d2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = p1; tri[1] = d1; tri[2] = d2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d2; tri[1] = d1; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);

					}
					else
						//if ((d2>0)&&(d2>0))
					{
						tri[0] = p0; tri[1] = d1; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = p0; tri[1] = d0; tri[2] = d1;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d0; tri[1] = p1; tri[2] = d1;
						triangleCells->InsertNextCell(3, tri);

					}




				break;
				// 1 - 2 trojkaty
			case 1:

				if (d0>0)
				{
					tri[0] = p0; tri[1] = d0; tri[2] = p2;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d0; tri[1] = p1; tri[2] = p2;
					triangleCells->InsertNextCell(3, tri);
				}
				else
					if (d1>0)
					{
						tri[0] = p0; tri[1] = p1; tri[2] = d1;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = p0; tri[1] = d1; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
					}
					else
					{
						tri[0] = p0; tri[1] = p1; tri[2] = d2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d2; tri[1] = p1; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
					}



				break;
			}

		}

	}
	// generuje nowe dane
	// sprawdzam kazdy cell - sprawdzam krawedzie
	// mam 3 punkty dla celu i punkty dla danej krawedzi - jesli sa
	// jesli punktow podzialu brak - wstawiam oryginalny trojkato do nowych danych
	// jesli 3 puntky podzialu - powstanie 4 trojkaty
	// jesli 1 powstana 2 - podzial z wierzcholkiem nie nalezacym do krawedzi
	// jesli 2 - powstana 3 trojkaty - ...

	vtkPolyData *output = vtkPolyData::New();
	output->SetPoints(newPts);
	output->SetPolys(triangleCells);
	output->BuildCells();
	output->BuildLinks();


	divisionCellIds->Delete();
	edgeTable->Delete();

	return output;

}
//---------------------------------------------
void	CleanPolyData(vtkPolyData *&polys)
{
	vtkCleanPolyData *Clean2 = vtkCleanPolyData::New();
	Clean2->SetInputData(polys);
	Clean2->PointMergingOn();
	Clean2->ConvertLinesToPointsOn();
	Clean2->SetConvertPolysToLines(true);
	Clean2->SetTolerance(1e-4);
	Clean2->Update();


	vtkPolyData *output = vtkPolyData::New();
	output->DeepCopy(Clean2->GetOutput());
	//	//output->Update();	
	Clean2->Delete();

	polys->Delete();

	polys = output;
}
//------------------------------------------
int		CleanPolyDataDuplicatedLines(vtkPolyData*& polys)
{
	int delCount = 0;

	if (polys != NULL)
	{


		vtkGenericCell* cell_i = vtkGenericCell::New();
		vtkGenericCell* cell_j = vtkGenericCell::New();

		

		int nrOfCells = polys->GetNumberOfCells();

		bool* delTab = new bool[nrOfCells];
		for (int i = 0;i < nrOfCells;i++)
		{
			delTab[i] = false;
		}

		int sum_i, sum_j;

		int id_i[2];
		int id_j[2];
		

		for (int i = 0;i < nrOfCells;i++)
		{
			polys->GetCell(i, cell_i);

			if ((cell_i->GetCellType() == VTK_LINE) &&(delTab[i]==false))
			{
				id_i[0] = cell_i->GetPointId(0);
				id_i[1] = cell_i->GetPointId(1);
				sum_i = id_i[0] + id_i[1];
				
				for (int j = i + 1;j < nrOfCells;j++)
				{
					polys->GetCell(j, cell_j);
					if ((cell_j->GetCellType() == VTK_LINE) && (delTab[j] == false))
					{
						id_j[0] = cell_j->GetPointId(0);
						id_j[1] = cell_j->GetPointId(1);
						sum_j = id_j[0] + id_j[1];

						if (sum_i == sum_j)
						{
							//linia podejrzana o to ze moze byc duplikat - sprawdzam teraz wszystkie casey

							if (
								((id_i[0] == id_j[0]) && (id_i[1] == id_j[1]))
								||
								((id_i[0] == id_j[1]) && (id_i[1] == id_j[0]))


								)
							{
								delTab[j] = true;
								delCount++;

							}

						}


					}

				}
			}
		}





		vtkPolyData* output = vtkPolyData::New();



		// robie kopie points
		//lines kopiuje tylko te co nie maja deltab=true


		vtkPoints* nPts = vtkPoints::New();
		nPts->DeepCopy(polys->GetPoints());
		output->SetPoints(nPts);


		vtkCellArray* lineCells = vtkCellArray::New();
		

		for (int i = 0;i < nrOfCells;i++)
		{
			polys->GetCell(i, cell_i);
			if ((delTab[i] == false)&& (cell_i->GetCellType() == VTK_LINE))
			{
				
				lineCells->InsertNextCell(cell_i);

			}
		}

		output->SetLines(lineCells);
		output->BuildCells();
		output->BuildLinks();

		delete []delTab;
		polys->Delete();
		polys = output;

	}
	else
	{
		delCount = -1;
	}


	return delCount;

}

//-------------------------------------------------
void	ComputeNormalsPolyData(vtkPolyData *&polys)
{
	vtkPolyDataNormals *Normals = vtkPolyDataNormals::New();
	Normals->SplittingOff();
	Normals->ComputePointNormalsOn();
	Normals->NonManifoldTraversalOn();
	Normals->ComputeCellNormalsOff();
	Normals->SetInputData(polys);
	Normals->Update();

	vtkPolyData *output = vtkPolyData::New();
	output->DeepCopy(Normals->GetOutput());
	//	//output->Update();	
	Normals->Delete();

	polys->Delete();

	polys = output;
}

//------------------------------------------
void	SmoothPolyData(vtkPolyData *&polys, float factor, int iterNumber, float featureAngle)
{

	vtkSmoothPolyDataFilter *SmoothFilter = vtkSmoothPolyDataFilter::New();
	SmoothFilter->SetInputData(polys);
	SmoothFilter->SetNumberOfIterations(iterNumber);
	SmoothFilter->SetRelaxationFactor(factor);
	SmoothFilter->SetFeatureAngle(featureAngle);
	SmoothFilter->FeatureEdgeSmoothingOff();
	SmoothFilter->BoundarySmoothingOff();
	SmoothFilter->SetConvergence(0);
	SmoothFilter->Update();
	int nrr = polys->GetNumberOfPoints();

	vtkPolyData *output = vtkPolyData::New();
	output->DeepCopy(SmoothFilter->GetOutput());
	//	//output->Update();	
	SmoothFilter->Delete();
	int nrr2 = output->GetNumberOfPoints();
	polys->Delete();
	int nrr3 = output->GetNumberOfPoints();
	polys = output;
}

//---------------------

vtkPolyData	*DistanceRefinePolyDataFilter(vtkPolyData *pData, double dist, unsigned char *avoidTab, int avoidTabLength, int maxIter)
{
	//count to bedzie liczba podzielonych trojkatow w danej iteracji
	int count = 0;
	int iter = 0;
	vtkPolyData *input = vtkPolyData::New();
	input->DeepCopy(pData);
	//	input->Update();

	do
	{
		count = 0;
		int i, j, k;
		int ptId;
		vtkIdType newCellPts[3];
		vtkIdType cellId;
		vtkIdList *pointList = vtkIdList::New();
		vtkIdList *cellPts = vtkIdList::New();
		vtkIdList *divisionCellIds = vtkIdList::New();
		vtkIdType divCellId[2];
		vtkIdType p0, p1, p2;


		vtkEdgeTable *edgeTable;
		edgeTable = vtkEdgeTable::New();
		vtkIdType np, np2, type;
		unsigned short ncells;
		double len;


		double point0[3], point1[3], point2[3];

		int nrr = input->GetNumberOfCells();

		vtkPoints *pts = input->GetPoints();
		int nrOfPoints = pts->GetNumberOfPoints();
		vtkPoints *newPts = vtkPoints::New();
		newPts->DeepCopy(pts);
		edgeTable->InitEdgeInsertion(pts->GetNumberOfPoints(), 1);

		int numStrips = input->GetNumberOfStrips();

		int edgeCount = 0;
		// stworzyc liste krawedzi

		double pp00[3];
		double pp11[3];
		pData->GetPoint(19, pp00);
		pData->GetPoint(18, pp11);


		for (i = 0; i<nrr; i++)
		{
			type = input->GetCellType(i);

			if (type == VTK_TRIANGLE)
			{
				input->GetCellPoints(i, pointList);

				for (j = 0; j<3; j++)
				{
					switch (j)
					{
					case 0:	p0 = pointList->GetId(0);	p1 = pointList->GetId(1);		break;
					case 1:	p0 = pointList->GetId(1);	p1 = pointList->GetId(2);		break;
					case 2:	p0 = pointList->GetId(2);	p1 = pointList->GetId(0);		break;
					};

					if (edgeTable->IsEdge(p0, p1) == -1)
					{
						edgeTable->InsertEdge(p0, p1);
						//jeszcze sprawdze dlugosc i jesli mniejsza od dist to wstawie punkt do listy
						len = GetDistance(pts, p0, p1);
						if (len > 100 * dist)
						{
							double pp0[3];
							double pp1[3];
							pts->GetPoint(p0, pp0);
							pts->GetPoint(p1, pp1);
							len = len;
						}
						if (len>dist)
						{
							GetLinearInterpolatedPoint(pts, point0, p0, p1, 0.5);
							// dodaje do nowych pktow					
							newPts->InsertNextPoint(point0);

							//dodac na liste
							// divCellId[0]=edgeCount++;
							// divCellId[1]=newPts->GetNumberOfPoints()-1;
							divisionCellIds->InsertNextId(nrOfPoints++);
							// divisionCells->InsertNextCell(2,divCellId);
							edgeCount++;
						}
						else
						{
							divisionCellIds->InsertNextId(0);
						}
					}
				}


			}

		}


		// dla kazdej krawedzi przypisac info czy dzielona czy nie (tablica z indeksem punktu podzialu), 0 - brak podzialu
		vtkCellArray *triangleCells = vtkCellArray::New();
		vtkIdType tri[3], tri1[3], tri2[3];

		for (i = 0; i<nrr; i++)
		{
			type = input->GetCellType(i);

			if (type == VTK_TRIANGLE)
			{
				input->GetCellPoints(i, pointList);
				p0 = pointList->GetId(0);
				p1 = pointList->GetId(1);
				p2 = pointList->GetId(2);
				vtkIdType d0, d1, d2;
				vtkIdType edgeIds[3];

				edgeIds[0] = edgeTable->IsEdge(p0, p1);
				edgeIds[1] = edgeTable->IsEdge(p1, p2);
				edgeIds[2] = edgeTable->IsEdge(p2, p0);

				d0 = divisionCellIds->GetId(edgeIds[0]);
				d1 = divisionCellIds->GetId(edgeIds[1]);
				d2 = divisionCellIds->GetId(edgeIds[2]);

				//tych nie moge dzielic!
				if ((p0<avoidTabLength) && (avoidTab[p0]>0) && (p1<avoidTabLength) && (avoidTab[p1]>0) && (avoidTab[p0] == avoidTab[p1])) d0 = 0;
				if ((p1<avoidTabLength) && (avoidTab[p1]>0) && (p2<avoidTabLength) && (avoidTab[p2]>0) && (avoidTab[p1] == avoidTab[p2])) d1 = 0;
				if ((p2<avoidTabLength) && (avoidTab[p2]>0) && (p0<avoidTabLength) && (avoidTab[p0]>0) && (avoidTab[p2] == avoidTab[p0])) d2 = 0;

				int cc = 0;
				if (d0>0) cc++; if (d1>0) cc++; if (d2>0) cc++;
				//sprawdzam case'y

				//zwiekszam gdy jakis trojkat zostanie podzielony
				if (cc>0) count++;

				switch (cc)
				{
					// 0 - robie 1 triangle cell 0-1-2 i dodaje
				case 0:
					tri[0] = p0; tri[1] = p1; tri[2] = p2;
					triangleCells->InsertNextCell(3, tri);
					break;
				case 3:
					// 3 - robie 4 trojkaty
					tri[0] = p0; tri[1] = d0; tri[2] = d2;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d0; tri[1] = p1; tri[2] = d1;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d1; tri[1] = p2; tri[2] = d2;
					triangleCells->InsertNextCell(3, tri);
					tri[0] = d0; tri[1] = d1; tri[2] = d2;
					triangleCells->InsertNextCell(3, tri);
					break;


					// 2 - 3 trojkaty
				case 2:
					if ((d0>0) && (d2>0))
					{
						tri[0] = p0; tri[1] = d0; tri[2] = d2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d0; tri[1] = p2; tri[2] = d2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d0; tri[1] = p1; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
					}
					else
						if ((d1>0) && (d2>0))
						{
							tri[0] = p0; tri[1] = p1; tri[2] = d2;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = p1; tri[1] = d1; tri[2] = d2;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = d2; tri[1] = d1; tri[2] = p2;
							triangleCells->InsertNextCell(3, tri);

						}
						else
							//if ((d2>0)&&(d2>0))
						{
							tri[0] = p0; tri[1] = d1; tri[2] = p2;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = p0; tri[1] = d0; tri[2] = d1;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = d0; tri[1] = p1; tri[2] = d1;
							triangleCells->InsertNextCell(3, tri);

						}




					break;
					// 1 - 2 trojkaty
				case 1:

					if (d0>0)
					{
						tri[0] = p0; tri[1] = d0; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
						tri[0] = d0; tri[1] = p1; tri[2] = p2;
						triangleCells->InsertNextCell(3, tri);
					}
					else
						if (d1>0)
						{
							tri[0] = p0; tri[1] = p1; tri[2] = d1;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = p0; tri[1] = d1; tri[2] = p2;
							triangleCells->InsertNextCell(3, tri);
						}
						else
						{
							tri[0] = p0; tri[1] = p1; tri[2] = d2;
							triangleCells->InsertNextCell(3, tri);
							tri[0] = d2; tri[1] = p1; tri[2] = p2;
							triangleCells->InsertNextCell(3, tri);
						}



					break;
				}

			}

		}
		// generuje nowe dane
		// sprawdzam kazdy cell - sprawdzam krawedzie
		// mam 3 punkty dla celu i punkty dla danej krawedzi - jesli sa
		// jesli punktow podzialu brak - wstawiam oryginalny trojkato do nowych danych
		// jesli 3 puntky podzialu - powstanie 4 trojkaty
		// jesli 1 powstana 2 - podzial z wierzcholkiem nie nalezacym do krawedzi
		// jesli 2 - powstana 3 trojkaty - ...

		vtkPolyData *output1 = vtkPolyData::New();
		output1->SetPoints(newPts);
		output1->SetPolys(triangleCells);
		output1->BuildCells();
		output1->BuildLinks();

		input->DeepCopy(output1);
		output1->Delete();

		divisionCellIds->Delete();
		edgeTable->Delete();

		iter++;
	} while ((iter<maxIter) && (count>0));

	return input;

}
//---------------------

RawDataSet *CreateStencilBuffer(vtkPolyData *polys, RawDataSet *origData, int closingS, unsigned char *mask0, int addDilate, int kernelType)
{
	int closingSize = closingS * 2 + 2;
	//int kernelType=0;//0cube 1sphere 2cross
	double bounds[6];
	int iBounds[6];
	polys->ComputeBounds();
	polys->GetBounds(bounds);

	int nrpts = polys->GetNumberOfPoints();
	double p[3];
	for (int i = 0; i<nrpts; i++)
	{
		polys->GetPoint(i, p);
	}
	double origOrigin[3];
	origData->GetOrigin(origOrigin);
	double origScale[3];
	origData->GetScale(origScale);

	int i, j;
	for (i = 0; i<3; i++)
	{
		bounds[i * 2] -= origOrigin[i];
		bounds[i * 2 + 1] -= origOrigin[i];

	}
	for (i = 0; i<3; i++)
	{
		bounds[i * 2] /= origScale[i];
		bounds[i * 2 + 1] /= origScale[i];
	}
	for (i = 0; i<6; i++)
	{
		iBounds[i] = Round(bounds[i]);

	}

	//	int closingSize=4;
	int size[3];
	double origin[3];

	double scale[3];
	origData->GetScale(scale);

	origin[0] = (iBounds[0] - closingSize)*scale[0] + origOrigin[0];
	origin[1] = (iBounds[2] - closingSize)*scale[1] + origOrigin[1];
	origin[2] = (iBounds[4] - closingSize)*scale[2] + origOrigin[2];

	size[0] = iBounds[1] - iBounds[0] + closingSize * 2 + 1;
	size[1] = iBounds[3] - iBounds[2] + closingSize * 2 + 1;
	size[2] = iBounds[5] - iBounds[4] + closingSize * 2 + 1;



	RawDataSet *newData = new RawDataSet(size[0], size[1], size[2], scale[0], scale[1], scale[2]);
	unsigned short *newDataArray = newData->GetDataArray();
	newData->SetOrigin(origin);
	newData->ZeroQ();


	vtkImageData *image = vtkImageData::New();
	image->SetSpacing(scale);
	image->SetOrigin(origin);
	image->SetExtent(0, newData->GetNx() - 1, 0, newData->GetNy() - 1, 0, newData->GetNz() - 1);

	//image->SetScalarTypeToUnsignedChar();

	image->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	vtkIdType count = image->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		image->GetPointData()->GetScalars()->SetTuple1(i, 0);
	}

	double boundsx[6];
	polys->GetBounds(boundsx);

	vtkPolyDataToImageStencil *p2s = vtkPolyDataToImageStencil::New();
	p2s->SetInputData(polys);

	p2s->SetOutputSpacing(scale);
	p2s->SetOutputOrigin(origin);
	p2s->SetOutputWholeExtent(0, newData->GetNx() - 1, 0, newData->GetNy() - 1, 0, newData->GetNz() - 1);

	vtkImageStencil *stencil = vtkImageStencil::New();
	stencil->SetInputData(image);
	stencil->SetStencilConnection(p2s->GetOutputPort());
	//stencil->SetStencilData (p2s->GetOutput());
	stencil->ReverseStencilOn();
	stencil->SetBackgroundValue(255);
	stencil->Update();


	unsigned char* tt = (unsigned char*)stencil->GetOutput()->GetPointData()->GetScalars()->GetVoidPointer(0);
	//(Data2,tt,Data1->GetVolumeSize());
	unsigned char first = tt[0];
	unsigned char *maskData = new unsigned char[newData->GetVolumeSize()];
	memset(maskData, 0, newData->GetVolumeSize());
	for (j = 0; j<newData->GetVolumeSize(); j++)
	{
		if (tt[j] == 255)
		{
			maskData[j] = 1;
			//newDataArray[j]=1;
		}
		//newDataArray[j]=tt[j];

	}
	//	emit SignalSendMask(Data2);




	if (mask0 == NULL)
	{
		for (i = 0; i<closingS; i++)
		{
			SlotBinarySimpleMorphology(newData, maskData, 1, 1, kernelType);
		}
		//SlotBinarySimpleMorphology(newData,maskData,1,closingS,kernelType);
		SlotBinarySimpleMorphology(newData, maskData, 0, closingS, kernelType);


	}
	else
	{
		//zrobie nowa mask0 dla danego rozmiaru
		unsigned char *newMask0 = new unsigned char[newData->GetVolumeSize()];
		memset(newMask0, 0, newData->GetVolumeSize());
		int x, y, z;
		int sPos[3];
		int newPos;
		int pos = 0;
		//-----------------
		for (z = 0; z<size[2]; z++)
		{
			for (y = 0; y<size[1]; y++)
			{
				for (x = 0; x<size[0]; x++)
				{
					//if (tmpDataArray[poss++]==1)
					{
						sPos[0] = Round(((x*scale[0] + origin[0]) - origOrigin[0]) / origScale[0]);
						sPos[1] = Round(((y*scale[1] + origin[1]) - origOrigin[1]) / origScale[1]);
						sPos[2] = Round(((z*scale[2] + origin[2]) - origOrigin[2]) / origScale[2]);

						newPos = origData->GetVoxelPos(sPos[0], sPos[1], sPos[2]);
						if ((newPos >= 0) && (newPos<origData->GetVolumeSize()))
						{
							if (mask0[newPos] == 1)
							{
								newMask0[pos] = 1;
							}
						}

					}

					pos++;
				}
			}
		}

		//----------------



		//SlotBinarySimpleMorphology(newData,maskData,1,closingS,kernelType);
		for (i = 0; i<closingS; i++)
		{
			SlotBinarySimpleMorphology(newData, maskData, 1, 1, kernelType);
		}
		SlotBinarySimpleMorphology(newData, maskData, 0, closingS, kernelType, newMask0);

		if (addDilate>0)
		{
			//for(i=0;i<closingS;i++)
			{
				SlotBinarySimpleMorphology(newData, maskData, 1, 1, kernelType, newMask0, addDilate);

			}
			//SlotBinarySimpleMorphology(newData,maskData,1,closingS,kernelType,newMask0,addDilate);
		}

		delete[]newMask0;
	}


	for (j = 0; j<newData->GetVolumeSize(); j++)
	{
		if (maskData[j] == 1) {
			newDataArray[j] = 1;
		}
	}


	image->Delete();
	p2s->Delete();
	stencil->Delete();
	delete[]maskData;
	return newData;

}
//--------------------------------------

void CreateStencilBufferMask(vtkPolyData *polys, RawDataSet *origData, int closingS, unsigned char *backgroundMask, unsigned char *finalMask, int addDilate, int kernelType, SegCriterion *cr, int fillValue)
{
	if ((polys == NULL) || (origData == NULL) || (finalMask == NULL))return;
	int closingSize = closingS * 2 + 2;
	//int kernelType=0;//0cube 1sphere 2cross
	double bounds[6];
	int iBounds[6];
	polys->ComputeBounds();
	polys->GetBounds(bounds);
	QElapsedTimer tm;
	tm.start();
	int nrpts = polys->GetNumberOfPoints();
	double p[3];
	for (int i = 0; i<nrpts; i++)
	{
		polys->GetPoint(i, p);
	}
	double origOrigin[3];
	origData->GetOrigin(origOrigin);
	double origScale[3];
	origData->GetScale(origScale);

	int i, j;
	for (i = 0; i<3; i++)
	{
		bounds[i * 2] -= origOrigin[i];
		bounds[i * 2 + 1] -= origOrigin[i];

	}
	for (i = 0; i<3; i++)
	{
		bounds[i * 2] /= origScale[i];
		bounds[i * 2 + 1] /= origScale[i];
	}
	for (i = 0; i<6; i++)
	{
		iBounds[i] = Round(bounds[i]);

	}

	//	int closingSize=4;
	int size[3];
	double origin[3];

	double scale[3];
	origData->GetScale(scale);

	origin[0] = (iBounds[0] - closingSize)*scale[0] + origOrigin[0];
	origin[1] = (iBounds[2] - closingSize)*scale[1] + origOrigin[1];
	origin[2] = (iBounds[4] - closingSize)*scale[2] + origOrigin[2];

	size[0] = iBounds[1] - iBounds[0] + closingSize * 2 + 1;
	size[1] = iBounds[3] - iBounds[2] + closingSize * 2 + 1;
	size[2] = iBounds[5] - iBounds[4] + closingSize * 2 + 1;



	RawDataSet *newData = new RawDataSet(size[0], size[1], size[2], scale[0], scale[1], scale[2]);
	unsigned short *newDataArray = newData->GetDataArray();
	newData->SetOrigin(origin);
	newData->ZeroQ();
	unsigned int t1 = tm.elapsed();
	unsigned int t2;
	vtkImageData *image = vtkImageData::New();
	image->SetSpacing(scale);
	image->SetOrigin(origin);
	image->SetExtent(0, newData->GetNx() - 1, 0, newData->GetNy() - 1, 0, newData->GetNz() - 1);

	image->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	vtkIdType count = image->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		image->GetPointData()->GetScalars()->SetTuple1(i, 0);
	}

	double boundsx[6];
	polys->GetBounds(boundsx);

	vtkPolyDataToImageStencil *p2s = vtkPolyDataToImageStencil::New();
	p2s->SetInputData(polys);

	p2s->SetOutputSpacing(scale);
	p2s->SetOutputOrigin(origin);
	p2s->SetOutputWholeExtent(0, newData->GetNx() - 1, 0, newData->GetNy() - 1, 0, newData->GetNz() - 1);

	vtkImageStencil *stencil = vtkImageStencil::New();
	stencil->SetInputData(image);
	stencil->SetStencilConnection(p2s->GetOutputPort());
	stencil->ReverseStencilOn();
	stencil->SetBackgroundValue(255);
	stencil->Update();
	t2 = tm.elapsed();	qWarning() << "Stencil: " << t2 - t1; t1 = t2;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	unsigned char* tt = (unsigned char*)stencil->GetOutput()->GetPointData()->GetScalars()->GetVoidPointer(0);

	unsigned char first = tt[0];
	unsigned char *maskData = new unsigned char[newData->GetVolumeSize()];
	memset(maskData, 0, newData->GetVolumeSize());
	int counter = 0;
	for (j = 0; j<newData->GetVolumeSize(); j++)
	{
		if (tt[j] == 255)
		{
			maskData[j] = 1;
			counter++;
		}
	}

	//pobiorę z backgroundMask (origData) to czego nie moge dylatować
	unsigned char *newMask0 = new unsigned char[newData->GetVolumeSize()];
	memset(newMask0, 0, newData->GetVolumeSize());
	int x, y, z;
	int sPos[3];
	int newPos;
	int pos = 0;
	counter = 0;
	//-----------------
	if (backgroundMask != NULL)
	{
		for (z = 0; z < size[2]; z++)
		{
			for (y = 0; y < size[1]; y++)
			{
				for (x = 0; x < size[0]; x++)
				{

					sPos[0] = Round(((x * scale[0] + origin[0]) - origOrigin[0]) / origScale[0]);
					sPos[1] = Round(((y * scale[1] + origin[1]) - origOrigin[1]) / origScale[1]);
					sPos[2] = Round(((z * scale[2] + origin[2]) - origOrigin[2]) / origScale[2]);

					newPos = origData->GetVoxelPos(sPos[0], sPos[1], sPos[2]);
					if ((newPos >= 0) && (newPos < origData->GetVolumeSize()))
					{
						if (backgroundMask[newPos] == 1)
						{
							newMask0[pos] = 0;
							counter++;
						}
					}

					pos++;
				}
			}
		}
	}
	t2 = tm.elapsed();	qWarning() << "Create Bck Mask: " << t2 - t1; t1 = t2;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!

																			  ////----------------


																			  /*if (cr == NULL)
																			  {

																			  SlotBinarySimpleMorphology(newData, maskData, 1, closingS, kernelType);
																			  for (i = 0; i<closingS; i++)
																			  {
																			  SlotBinarySimpleMorphology(newData, maskData, 1, 1, kernelType);
																			  }
																			  SlotBinarySimpleMorphology(newData, maskData, 0, closingS, kernelType, newMask0);

																			  if (addDilate>0)
																			  {
																			  for (i = 0; i<closingS; i++)
																			  SlotBinarySimpleMorphology(newData, maskData, 1, 1, kernelType, newMask0, addDilate);
																			  }

																			  }
																			  else*/
	{

		//preclosing

		if (cr!=NULL)cr->SetRawData(newData);//bo cr mialo zapisany wiekszy wolumen zrodlowy!!


		if (addDilate>0)
		{
			for (i = 0; i<addDilate; i++)
				SlotBinarySimpleMorphologyWithCriterion2(newData, maskData, 1, 1, kernelType, NULL, NULL, newMask0, cr);
		}
		t2 = tm.elapsed();	qWarning() << "Dilation: " << t2 - t1; t1 = t2;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (closingS > 0)
		{

			SlotBinarySimpleMorphologyWithCriterion2(newData, maskData, 1, closingSize, kernelType, NULL, NULL, newMask0, NULL);
			SlotBinarySimpleMorphologyWithCriterion2(newData, maskData, 0, closingSize, kernelType, NULL, NULL, newMask0, NULL);
		}
		t2 = tm.elapsed();	qWarning() << "Closing: " << t2 - t1; t1 = t2;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!
																		  ////postclosing
																		  //SlotBinarySimpleMorphologyWithCriterion(newData, maskData, 1, closingSize, kernelType, NULL, NULL, newMask0, NULL);
																		  //SlotBinarySimpleMorphologyWithCriterion(newData, maskData, 0, closingSize, kernelType, NULL, NULL, newMask0, NULL);
	}

	delete[]newMask0;

	//masData[] - tutaj jest zdylatowany volume teraz trzeba go przeniesc do maski finalMask!!!
	pos = 0;
	counter = 0;
	for (z = 0; z < size[2]; z++)
	{
		for (y = 0; y < size[1]; y++)
		{
			for (x = 0; x < size[0]; x++)
			{

				sPos[0] = Round(((x*scale[0] + origin[0]) - origOrigin[0]) / origScale[0]);
				sPos[1] = Round(((y*scale[1] + origin[1]) - origOrigin[1]) / origScale[1]);
				sPos[2] = Round(((z*scale[2] + origin[2]) - origOrigin[2]) / origScale[2]);

				newPos = origData->GetVoxelPos(sPos[0], sPos[1], sPos[2]);
				if ((newPos >= 0) && (newPos < origData->GetVolumeSize()))
				{
					if (maskData[pos] == 1)
					{
						finalMask[newPos] = fillValue;
						counter++;
					}


				}
				pos++;
			}
		}
	}

	delete[]maskData;
	delete newData;




}

vtkPolyData *GenerateSmoothedSurface(unsigned short thresh, RawDataSet *data, unsigned char *mdata, bool decim, bool smooth)
{


	if (mdata != NULL)
	{
		int smoothIterations = 100;
		double relaxionsFactor = 0.01;
		double smoothAngle = 60.0;
		double featureAngle = 60.0;
		double targetred = 0.3;

		double scale[3];
		double origin[3];
		int size[3];
		data->GetN(size);
		data->GetScale(scale);
		data->GetOrigin(origin);

		vtkSmartPointer<vtkStructuredPoints> structuredPoints = vtkSmartPointer<vtkStructuredPoints>::New();
		structuredPoints->SetSpacing(scale[0], scale[1], scale[2]);
		//structuredPoints->SetScalarType( VTK_CHAR);
		structuredPoints->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
		structuredPoints->SetExtent(0, size[0] - 1, 0, size[1] - 1, 0, size[2] - 1);
		//structuredPoints->SetNumberOfScalarComponents(1);
		structuredPoints->SetOrigin(origin);
		//StructuredPoints->set

		//vtkSmartPointer<vtkCharArray> marray=vtkSmartPointer<vtkCharArray>::New();

		vtkCharArray *marray = vtkCharArray::New();
		marray->SetVoidArray(mdata, size[0] * size[1] * size[2], 1);

		structuredPoints->GetPointData()->SetScalars(marray);

		//vtkDiscreteMarchingCubes *MarchingCubes=vtkDiscreteMarchingCubes::New();
		vtkSmartPointer<vtkMarchingCubes> marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
		//vtkMarchingCubes *marchingCubes = vtkMarchingCubes::New();
		marchingCubes->SetInputData(structuredPoints);
		marchingCubes->ComputeScalarsOff();
		marchingCubes->SetValue(0, 0.01);
		marchingCubes->ComputeNormalsOn();
		marchingCubes->Update();

		vtkSmartPointer<vtkPolyData> ppData = vtkSmartPointer<vtkPolyData>::New();
		if (smooth == true)
		{
			vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
			smoother->SetInputData(marchingCubes->GetOutput());
			smoother->SetNumberOfIterations(smoothIterations);
			smoother->SetRelaxationFactor(relaxionsFactor);
			smoother->SetFeatureAngle(smoothAngle);
			smoother->FeatureEdgeSmoothingOn();
			smoother->SetConvergence(0);
			smoother->Update();
			ppData->DeepCopy(smoother->GetOutput());
			//	//p//pData->Update();
		}
		else
		{
			ppData->DeepCopy(marchingCubes->GetOutput());
			////p//pData->Update();
		}


		if (decim == true)
		{
			vtkSmartPointer<vtkDecimatePro> decimate = vtkSmartPointer<vtkDecimatePro>::New();
			decimate->SetInputData(ppData);
			decimate->SetFeatureAngle(60.0);
			decimate->SetMaximumError(1);
			decimate->SetTargetReduction(targetred);
			decimate->Update();

			if (smooth == true)
			{
				vtkSmartPointer<vtkSmoothPolyDataFilter> smoother2 = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
				//smoother2->SetInputData(smoother->GetOutput()); 
				smoother2->SetInputData(decimate->GetOutput());
				smoother2->SetNumberOfIterations(smoothIterations);
				smoother2->SetRelaxationFactor(relaxionsFactor);
				smoother2->SetFeatureAngle(smoothAngle);
				smoother2->FeatureEdgeSmoothingOff();
				smoother2->SetConvergence(0);
				smoother2->Update();
				ppData->DeepCopy(smoother2->GetOutput());
				////p//pData->Update();
			}
			else
			{
				ppData->DeepCopy(marchingCubes->GetOutput());
				////p//pData->Update();
			}
		}

		vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
		/* if (decim==true)
		normals->SetInputData(smoother2->GetOutput());
		else*/
		normals->SetInputData(ppData);

		normals->SetFeatureAngle(featureAngle);
		normals->Update();



		vtkPolyData *output = vtkPolyData::New();
		output->DeepCopy(normals->GetOutput());
		//		//output->Update();	

		//PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
		//------------------------------
		marchingCubes->SetInputData(NULL);
		structuredPoints->GetPointData()->SetScalars(NULL);
		//structuredPoints->Delete();
		marray->SetVoidArray(NULL, 0, 1);
		marray->Delete();
		//marchingCubes->Delete();
		//-----------------------------
		//int i;
		/*for(i=0;i<3;i++)
		{
		Output1PolygonSurfaceData->GetPN()[i]=N[i];
		Output1PolygonSurfaceData->GetPScale()[i]=Scale[i];
		}

		Output1PolygonSurfaceData->SetCenterOrigin();
		Output1PolygonSurfaceData->SetPolygonSurfaceDataSetName(name);
		Output1PolygonSurfaceData->SetRepaintWithoutReset(repaintWithoutReset);
		Output1PolygonSurfaceData->SetIsSegCutted(isSegCutted);
		emit	SignalSendNewSurface(Output1PolygonSurfaceData);*/

		return output;
	}
	else
	{
		int smoothIterations = 100;
		double relaxionsFactor = 0.01;
		double smoothAngle = 60.0;
		double featureAngle = 60.0;
		double targetred = 0.3;

		double scale[3];
		double origin[3];
		int size[3];
		data->GetN(size);
		data->GetScale(scale);
		data->GetOrigin(origin);

		vtkSmartPointer<vtkStructuredPoints> structuredPoints = vtkSmartPointer<vtkStructuredPoints>::New();
		structuredPoints->SetSpacing(scale[0], scale[1], scale[2]);
		//structuredPoints->SetScalarType( VTK_UNSIGNED_SHORT);
		structuredPoints->AllocateScalars(VTK_UNSIGNED_SHORT, 1);
		structuredPoints->SetExtent(0, size[0] - 1, 0, size[1] - 1, 0, size[2] - 1);
		//structuredPoints->SetNumberOfScalarComponents(1);
		structuredPoints->SetOrigin(origin);
		//StructuredPoints->set

		//vtkSmartPointer<vtkCharArray> marray=vtkSmartPointer<vtkCharArray>::New();
		vtkUnsignedShortArray *marray = vtkUnsignedShortArray::New();
		marray->SetVoidArray(data->GetDataArray(), data->GetVolumeSize(), 1);
		//	StructuredPoints->GetPointData()->SetScalars(Array);
		//vtkCharArray *marray=vtkCharArray::New();
		//marray->SetVoidArray(mdata,size[0]*size[1]*size[2],1);

		structuredPoints->GetPointData()->SetScalars(marray);

		//vtkDiscreteMarchingCubes *MarchingCubes=vtkDiscreteMarchingCubes::New();
		vtkSmartPointer<vtkMarchingCubes> marchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
		//vtkMarchingCubes *marchingCubes = vtkMarchingCubes::New();
		marchingCubes->SetInputData(structuredPoints);
		marchingCubes->ComputeScalarsOff();
		marchingCubes->SetValue(0, 0.5);
		marchingCubes->ComputeNormalsOn();
		marchingCubes->Update();


		vtkSmartPointer<vtkPolyData> ppData = vtkSmartPointer<vtkPolyData>::New();
		if (smooth == true)
		{
			vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
			smoother->SetInputData(marchingCubes->GetOutput());
			smoother->SetNumberOfIterations(smoothIterations);
			smoother->SetRelaxationFactor(relaxionsFactor);
			smoother->SetFeatureAngle(smoothAngle);
			smoother->FeatureEdgeSmoothingOff();
			smoother->SetConvergence(0);
			smoother->Update();
			ppData->DeepCopy(smoother->GetOutput());
			////p//pData->Update();
		}
		else
		{
			ppData->DeepCopy(marchingCubes->GetOutput());
			////p//pData->Update();
		}

		if (decim == true)
		{
			vtkSmartPointer<vtkDecimatePro> decimate = vtkSmartPointer<vtkDecimatePro>::New();
			decimate->SetInputData(ppData);
			decimate->SetFeatureAngle(60.0);
			decimate->SetMaximumError(1);
			decimate->SetTargetReduction(targetred);
			decimate->Update();

			if (smooth == true)
			{
				vtkSmartPointer<vtkSmoothPolyDataFilter> smoother2 = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
				//smoother2->SetInputData(smoother->GetOutput()); 
				smoother2->SetInputData(decimate->GetOutput());
				smoother2->SetNumberOfIterations(smoothIterations);
				smoother2->SetRelaxationFactor(relaxionsFactor);
				smoother2->SetFeatureAngle(smoothAngle);
				smoother2->FeatureEdgeSmoothingOff();
				smoother2->SetConvergence(0);
				smoother2->Update();
				ppData->DeepCopy(smoother2->GetOutput());
				////p//pData->Update();
			}
			else
			{
				ppData->DeepCopy(marchingCubes->GetOutput());
				/////p//pData->Update();
			}
		}

		vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
		/* if (decim==true)
		normals->SetInputData(smoother2->GetOutput());
		else*/

		normals->SetFeatureAngle(featureAngle);
		normals->Update();



		vtkPolyData *output = vtkPolyData::New();
		output->DeepCopy(normals->GetOutput());
		//		//output->Update();	

		//PolygonSurfaceData *Output1PolygonSurfaceData=new PolygonSurfaceData(output,VWidget);
		//------------------------------
		marchingCubes->SetInputData(NULL);
		structuredPoints->GetPointData()->SetScalars(NULL);
		//structuredPoints->Delete();
		marray->SetVoidArray(NULL, 0, 1);
		marray->Delete();


		return output;
	}
	return NULL;

}
//---------------------------------------


void UpdateProperty(vtkProperty *prop, QString txt, QString &textureName, bool &repeatTexture, QString &groupName, vtkPolyDataMapper *vMapper)
{
	QFile f(txt);
	if (f.exists())
	{

		QSettings settings(txt, QSettings::IniFormat);


		double color[3], specularColor[3], diffuseColor[3], ambientColor[3];
		double specularValue, specularPower, diffuseValue, ambientValue;
		double opacity;
		QString interpolation;
		QString representation;
		QString lighting;
		QString repeat;

		prop->GetColor(color);
		prop->GetAmbientColor(ambientColor);
		prop->GetSpecularColor(specularColor);
		prop->GetDiffuseColor(diffuseColor);
		specularValue = prop->GetSpecular();
		specularPower = prop->GetSpecularPower();
		diffuseValue = prop->GetDiffuse();
		ambientValue = prop->GetAmbient();
		interpolation = prop->GetInterpolationAsString();
		representation = prop->GetRepresentationAsString();
		opacity = prop->GetOpacity();

		settings.beginGroup("Color");
		if (settings.contains("R")) color[0] = settings.value("R").toDouble();
		if (settings.contains("G")) color[1] = settings.value("G").toDouble();
		if (settings.contains("B")) color[2] = settings.value("B").toDouble();
		if (settings.contains("opacity")) opacity = settings.value("opacity").toDouble();
		settings.endGroup();

		settings.beginGroup("Specular");
		if (settings.contains("R")) specularColor[0] = settings.value("R").toDouble();
		if (settings.contains("G")) specularColor[1] = settings.value("G").toDouble();
		if (settings.contains("B")) specularColor[2] = settings.value("B").toDouble();
		if (settings.contains("value")) specularValue = settings.value("value").toDouble();
		if (settings.contains("power")) specularPower = settings.value("power").toDouble();
		settings.endGroup();


		settings.beginGroup("Diffuse");
		if (settings.contains("R")) diffuseColor[0] = settings.value("R").toDouble();
		if (settings.contains("G")) diffuseColor[1] = settings.value("G").toDouble();
		if (settings.contains("B")) diffuseColor[2] = settings.value("B").toDouble();
		if (settings.contains("value")) diffuseValue = settings.value("value").toDouble();
		settings.endGroup();


		settings.beginGroup("Ambient");
		if (settings.contains("R")) ambientColor[0] = settings.value("R").toDouble();
		if (settings.contains("G")) ambientColor[1] = settings.value("G").toDouble();
		if (settings.contains("B")) ambientColor[2] = settings.value("B").toDouble();
		if (settings.contains("value")) ambientValue = settings.value("value").toDouble();
		settings.endGroup();

		settings.beginGroup("Other");

		if (settings.contains("Interpolation")) interpolation = settings.value("Interpolation").toString();
		if (settings.contains("Representation")) representation = settings.value("Representation").toString();
		if (settings.contains("Lighting")) lighting = settings.value("Lighting").toString();
		settings.endGroup();

		settings.beginGroup("Texture");
		if (settings.contains("Name")) textureName = settings.value("Name").toString();
		if (settings.contains("Repeat")) repeat = settings.value("Repeat").toString();
		settings.endGroup();

		settings.beginGroup("GroupName");
		if (settings.contains("Name")) groupName = settings.value("Name").toString();
		settings.endGroup();

		prop->SetColor(color);
		prop->SetAmbientColor(ambientColor);
		prop->SetSpecularColor(specularColor);
		prop->SetDiffuseColor(diffuseColor);
		prop->SetSpecular(specularValue);
		prop->SetSpecularPower(specularPower);
		prop->SetDiffuse(diffuseValue);
		prop->SetAmbient(ambientValue);

		if (lighting.compare("On") == 0)
		{
			prop->LightingOn();
		}
		else if (lighting.compare("Off") == 0)
		{
			prop->LightingOff();
		}

		if (repeat.compare("On") == 0)
		{
			repeatTexture = true;
		}
		else if (repeat.compare("Off") == 0)
		{
			repeatTexture = false;
		}

		if (representation.compare("Points") == 0)
		{
			prop->SetRepresentationToPoints();
		}
		else if (representation.compare("Wireframe") == 0)
		{
			prop->SetRepresentationToWireframe();
		}
		else if (representation.compare("Surface") == 0)
		{
			prop->SetRepresentationToSurface();
		}



		if (interpolation.compare("Flat") == 0)
		{
			prop->SetInterpolationToFlat();
		}
		else if (interpolation.compare("Gouraud") == 0)
		{
			prop->SetInterpolationToGouraud();
		}
		else if (interpolation.compare("Phong") == 0)
		{
			prop->SetInterpolationToPhong();
		}

		prop->SetOpacity(opacity);


		if (vMapper != NULL)
		{

			double params[3] = { 0 };

			settings.beginGroup("Mapper");
			int mode = 0;
			QString modeName = "Off";
			if (settings.contains("Mode"))
			{
				modeName = settings.value("Mode").toString();
				if (modeName.compare("Off") == 0) mode = 0;
				else if (modeName.compare("PolygonOffset") == 0) mode = 1;
				else if (modeName.compare("ZBuffer") == 0) mode = 2;
			}

			if (settings.contains("ResolveP0")) params[0] = settings.value("ResolveP0").toDouble();
			if (settings.contains("ResolveP1")) params[1] = settings.value("ResolveP1").toDouble();
			if (settings.contains("ResolveP2")) params[2] = settings.value("ResolveP2").toDouble();


			switch (mode)
			{
			case 0:
				vMapper->SetResolveCoincidentTopology(0);
				break;

			case 2:

				vMapper->SetResolveCoincidentTopologyToShiftZBuffer();
				vMapper->SetResolveCoincidentTopologyZShift(params[0]);

				break;

			case 1:

				vMapper->SetResolveCoincidentTopologyToPolygonOffset();
				// We offset lines/vertices.
				vMapper->SetResolveCoincidentTopologyPolygonOffsetFaces(params[0]);
				vMapper->SetResolveCoincidentTopologyPolygonOffsetParameters(params[1], params[2]);
				break;



			}
			vMapper->Modified();
		}
	}
}
//--------------

void WriteProperty(vtkProperty *prop, QString filename, QString textureName, bool repeat, QString groupName, vtkPolyDataMapper *vMapper)
{

	double color[3], specularColor[3], diffuseColor[3], ambientColor[3];
	double specularValue, specularPower, diffuseValue, ambientValue;
	double opacity;
	QString interpolation;
	QString representation;
	QString groumName;
	//QString lighting;
	//QString repeat;

	prop->GetColor(color);
	prop->GetAmbientColor(ambientColor);
	prop->GetSpecularColor(specularColor);
	prop->GetDiffuseColor(diffuseColor);
	specularValue = prop->GetSpecular();
	specularPower = prop->GetSpecularPower();
	diffuseValue = prop->GetDiffuse();
	ambientValue = prop->GetAmbient();
	interpolation = prop->GetInterpolationAsString();
	representation = prop->GetRepresentationAsString();
	opacity = prop->GetOpacity();
	QString lighting = (prop->GetLighting() == true) ? "On" : "Off";
	QString repeatTexture = (repeat == true) ? "On" : "Off";

	//QString filename=rDir.absolutePath()+"/"+name+".prop";
	QSettings settings(filename, QSettings::IniFormat);



	settings.beginGroup("Color");
	settings.setValue("R", color[0]);
	settings.setValue("G", color[1]);
	settings.setValue("B", color[2]);
	settings.setValue("opacity", opacity);
	settings.endGroup();

	settings.beginGroup("Specular");
	settings.setValue("R", specularColor[0]);
	settings.setValue("G", specularColor[1]);
	settings.setValue("B", specularColor[2]);
	settings.setValue("value", specularValue);
	settings.setValue("power", specularPower);
	settings.endGroup();

	settings.beginGroup("Diffuse");
	settings.setValue("R", diffuseColor[0]);
	settings.setValue("G", diffuseColor[1]);
	settings.setValue("B", diffuseColor[2]);
	settings.setValue("value", diffuseValue);
	settings.endGroup();

	settings.beginGroup("Ambient");
	settings.setValue("R", ambientColor[0]);
	settings.setValue("G", ambientColor[1]);
	settings.setValue("B", ambientColor[2]);
	settings.setValue("value", ambientValue);
	settings.endGroup();

	settings.beginGroup("Other");
	settings.setValue("Representation", representation);
	settings.setValue("Interpolation", interpolation);
	settings.setValue("Lighting", lighting);
	settings.endGroup();

	if (!textureName.isEmpty())
	{
		settings.beginGroup("Texture");
		settings.setValue("Name", textureName);
		settings.setValue("Repeat", repeatTexture);
		settings.endGroup();
	}
	if (!groupName.isEmpty())
	{
		settings.beginGroup("GroupName");
		settings.setValue("Name", groupName);
		settings.endGroup();
	}
	if (vMapper != NULL)
	{
		double params[3] = { 0 };
		settings.beginGroup("Mapper");

		int mode = vMapper->GetResolveCoincidentTopology();

		switch (mode)
		{
		case 0:
			settings.setValue("Mode", "Off");

			break;

		case 1:

			params[0] = vMapper->GetResolveCoincidentTopologyPolygonOffsetFaces();
			vMapper->GetResolveCoincidentTopologyPolygonOffsetParameters(params[1], params[2]);
			settings.setValue("Mode", "PolygonOffset");
			settings.setValue("ResolveP0", params[0]);
			settings.setValue("ResolveP1", params[1]);
			settings.setValue("ResolveP2", params[2]);

			break;

		case 2:
			params[0] = vMapper->GetResolveCoincidentTopologyZShift();
			settings.setValue("Mode", "ZBuffer");
			settings.setValue("ResolveP0", params[0]);

			break;
		}


		settings.endGroup();

	}
}
//-----------------------------------
double GetPolyLineLength(vtkPoints* pts)
{
	double sum = 0;
	double p[3], p1[3];
	if (pts != NULL)
	{
		if (pts->GetNumberOfPoints() > 1)
		{
			for (int i = 0;i < pts->GetNumberOfPoints() - 1;i++)
			{
				pts->GetPoint(i, p);
				pts->GetPoint(i+1, p1);
				sum += GetDistance(p, p1);

			}
		}
	}
	return sum;
}