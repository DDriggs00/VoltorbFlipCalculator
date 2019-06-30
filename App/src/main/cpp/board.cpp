#include "board.h"

board::board()
{
}

board::~board()
{
	for (size_t i = 0; i < CELLS_PER_ROW; i++)
	{
		for (size_t j = 0; j < CELLS_PER_ROW; j++)
		{
			gameBoard[i][j].~cell();
		}
	}
}

//cell board::getCell(int row, int col)
//{
//	return gameBoard[row][col];
//}


//bool board::Win()
//{
//	bool win = true;
//	for (size_t i = 0; i < CELLS_PER_ROW; i++)
//	{
//		for (size_t j = 0; j < CELLS_PER_ROW; j++)
//		{
//			if (!gameBoard[i][j].IsKnown() && gameBoard[i][j].CanHavePoints()) {
//				win = false;
//			}
//		}
//	}
//	return win;
//}

void board::Initialize()
{
	for (size_t i = 0; i < CELLS_PER_ROW; i++) 
	{
		for (size_t j = 0; j < CELLS_PER_ROW; j++) 
		{
			gameBoard[i][j].FindPossibleVals(rowTotals[i], rowVoltorbs[i], colTotals[j], colVoltorbs[j], CELLS_PER_ROW, CELLS_PER_ROW);
		}
	}

	CalculatePhase2();
}

//int board::GetCalculationNumber()
//{
//	int calculations = 1;
//	for (size_t i = 0; i < CELLS_PER_ROW; i++)
//	{
//		for (size_t j = 0; j < CELLS_PER_ROW; j++)
//		{
//			calculations = calculations * gameBoard[i][j].CountPossibleVals();
//		}
//	}
//	return calculations;
//}

void board::CalculatePhase2()
{
	bool finished = false;
	while(!finished) 
	{
		finished = true;
		bool rowRecheck[CELLS_PER_ROW] = { 0,0,0,0,0 };
		bool colRecheck[CELLS_PER_ROW] = { 0,0,0,0,0 };

		// Mark cells for rechecking
		for (size_t i = 0; i < CELLS_PER_ROW; i++)
		{
			for (size_t j = 0; j < CELLS_PER_ROW; j++)
			{
				if (gameBoard[i][j].IsStatic() && !gameBoard[i][j].IsKnown()) {
					MakeCellKnown(i,j);
					rowRecheck[i] = true;
					colRecheck[j] = true;
					finished = false;
				}
				if (!gameBoard[i][j].CanHavePoints() && !gameBoard[i][j].IsNoPoints() && !gameBoard[i][j].IsKnown()) {
					gameBoard[i][j].MakeNoPoints();
					rowRecheck[i] = true;
					colRecheck[j] = true;
					finished = false;
				}
			}
		}
		
		if (finished) {
			return;
		}

		// Do Regular rechecking
		for (size_t i = 0; i < CELLS_PER_ROW; i++)
		{
			for (size_t j = 0; j < CELLS_PER_ROW; j++)
			{
				if (rowRecheck[i] || colRecheck[j]) {
					if (!gameBoard[i][j].IsKnown()) {
						gameBoard[i][j].FindPossibleVals(rowTotalsRemaining[i], rowVoltorbsRemaining[i], colTotalsRemaining[j], colVoltorbsRemaining[j], rowCellsRemaining[i], colCellsRemaining[j]);
					}
				}
			}
		}

		// Advanced checking will fail if there are unknown static cells,
		// So if there are, do it next round
		bool advanced = true;
		for (size_t i = 0; i < CELLS_PER_ROW; i++)
		{
			for (size_t j = 0; j < CELLS_PER_ROW; j++)
			{
				if (gameBoard[i][j].IsStatic() && !gameBoard[i][j].IsKnown()) {
					advanced = false;
				}
			}
		}

		if (advanced) {
			// Check for cells that cannot be low because of row
			for (size_t i = 0; i < CELLS_PER_ROW; i++)
			{
				int pointCells = 0;
				int twoOnlyCells = 0;
				int threeOnlyCells = 0;
				int twoThreeCells = 0;

				// set values of above variables
				for (size_t j = 0; j < CELLS_PER_ROW; j++)
				{
					if (!gameBoard[i][j].IsStatic()) {
						if (gameBoard[i][j].CanHavePoints()) {
							pointCells++;
							if (gameBoard[i][j].CanBe(3)) {
								if (!gameBoard[i][j].CanBe(2)) {
									threeOnlyCells++;
								}
								else {
									twoThreeCells++;
								}
							}
							else {
								twoOnlyCells++;
							}
						}

					}
				}

				int spareNums = (rowTotalsRemaining[i] + rowVoltorbsRemaining[i]) - rowCellsRemaining[i] + pointCells;
				switch (pointCells)
				{
				case 0:
					break;
				case 1:
					if (twoOnlyCells == 1) {
						for (size_t j = 0; j < CELLS_PER_ROW; j++)
						{
							if (gameBoard[i][j].CanBe(2) && !gameBoard[i][j].IsKnown()) {
								gameBoard[i][j].Set(2);
							}
						}
					}
					else { // A three is possible (therefore must be three)
						for (size_t j = 0; j < CELLS_PER_ROW; j++)
						{
							if (gameBoard[i][j].CanBe(3) && !gameBoard[i][j].IsKnown()) {
								gameBoard[i][j].Set(3);
							}
						}
					}
					break;
				default:
					if (spareNums == (pointCells * 3) - 1) {
						for (size_t j = 0; j < CELLS_PER_ROW; j++)
						{
							if (gameBoard[i][j].CanHavePoints() && !gameBoard[i][j].IsKnown()) {
								gameBoard[i][j].Disable(0);
								gameBoard[i][j].Disable(1);
							}
						}
					}
					else if (spareNums == (pointCells * 3)) {
						for (size_t j = 0; j < CELLS_PER_ROW; j++)
						{
							if (gameBoard[i][j].CanBe(3) && !gameBoard[i][j].IsKnown()) {
								gameBoard[i][j].Set(3);
							}
						}
					}
					break;
				}
			}
		
			// Check for cells that cannot be low because of column
			for (size_t j = 0; j < CELLS_PER_ROW; j++)
			{
				int pointCells = 0;
				int twoOnlyCells = 0;
				int threeOnlyCells = 0;
				int twoThreeCells = 0;

				// set values of above variables
				for (size_t i = 0; i < CELLS_PER_ROW; i++)
				{
					if (!gameBoard[i][j].IsStatic()) {
						if (gameBoard[i][j].CanHavePoints()) {
							pointCells++;
							if (gameBoard[i][j].CanBe(3)) {
								if (!gameBoard[i][j].CanBe(2)) {
									threeOnlyCells++;
								}
								else {
									twoThreeCells++;
								}
							}
							else {
								twoOnlyCells++;
							}
						}

					}
				}

				int spareNums = (colTotalsRemaining[j] + colVoltorbsRemaining[j]) - colCellsRemaining[j] + pointCells;
				switch (pointCells) 
				{
				case 0:
					break;
				case 1:
					if (twoOnlyCells == 1) {
						for (size_t i = 0; i < CELLS_PER_ROW; i++)
						{
							if (gameBoard[i][j].CanBe(2) && !gameBoard[i][j].IsKnown()) {
								gameBoard[i][j].Set(2);
							}
						}
					}
					else { // A three is possible (therefore must be three)
						for (size_t i = 0; i < CELLS_PER_ROW; i++)
						{
							if (gameBoard[i][j].CanBe(3) && !gameBoard[i][j].IsKnown()) {
								gameBoard[i][j].Set(3);
							}
						}
					}
					break;
				default:
					if (spareNums == (pointCells * 3) - 1) {
						for (size_t i = 0; i < CELLS_PER_ROW; i++)
						{
							if (gameBoard[i][j].CanHavePoints() && !gameBoard[i][j].IsKnown()) {
								gameBoard[i][j].Disable(0);
								gameBoard[i][j].Disable(1);
							}
						}
					}
					else if (spareNums == (pointCells * 3)) {
						for (size_t i = 0; i < CELLS_PER_ROW; i++)
						{
							if (gameBoard[i][j].CanBe(3) && !gameBoard[i][j].IsKnown()) {
								gameBoard[i][j].Set(3);
							}
						}
					}
					break;
				}
			}

			//Check for cells that cannot be 3 because of row
			for (size_t i = 0; i < CELLS_PER_ROW; i++)
			{
				int pointCells = 0;
				int nonZeroCells = 0;

				// Find number of nonZero and pointCells
				for (size_t j = 0; j < CELLS_PER_ROW; j++)
				{
					if (!gameBoard[i][j].IsKnown() && gameBoard[i][j].CanHavePoints()) {
						pointCells++;
						if (!gameBoard[i][j].CanBe(0) && !gameBoard[i][j].CanBe(1)) {
							nonZeroCells++;
						}
					}
				}

				if (nonZeroCells == 0) {
					continue;
				}

				int spareNums = (rowTotalsRemaining[i] + rowVoltorbsRemaining[i]) - rowCellsRemaining[i] + pointCells;
			
				if ((spareNums - (2 * nonZeroCells)) <= (pointCells - 1 + 1)) {
					for (size_t j = 0; j < CELLS_PER_ROW; j++)
					{
						if (!gameBoard[i][j].IsKnown() && gameBoard[i][j].CanHavePoints()) {
							if (gameBoard[i][j].CanBe(0) || gameBoard[i][j].CanBe(1)) {
								gameBoard[i][j].Disable(3);
							}
						}
					}
				}
			}

			//Check for cells that cannot be 3 because of column
			for (size_t j = 0; j < CELLS_PER_ROW; j++)
		{
			int pointCells = 0;
			int nonZeroCells = 0;

			// Find number of nonZero and pointCells
			for (size_t i = 0; i < CELLS_PER_ROW; i++)
			{
				if (!gameBoard[i][j].IsStatic() && gameBoard[i][j].CanHavePoints()) {
					pointCells++;
					if (!gameBoard[i][j].CanBe(0) && !gameBoard[i][j].CanBe(1)) {
						nonZeroCells++;
					}
				}
			}

			if (nonZeroCells == 0) {
				continue;
			}

			int spareNums = (colTotalsRemaining[j] + colVoltorbsRemaining[j]) - colCellsRemaining[j] + pointCells;
			
			if ((spareNums - (2 * nonZeroCells)) <= (pointCells - 1 + 1)) {
				for (size_t i = 0; i < CELLS_PER_ROW; i++)
				{
					if (!gameBoard[i][j].IsStatic() && gameBoard[i][j].CanHavePoints()) {
						if (gameBoard[i][j].CanBe(0) || gameBoard[i][j].CanBe(1)) {
							gameBoard[i][j].Disable(3);
						}
					}
				}
			}
		}
		}
	}
}

void board::MakeCellKnown(int row, int col)
{
	if (!gameBoard[row][col].IsStatic() || gameBoard[row][col].IsKnown()) {
		return;
	}

	gameBoard[row][col].MakeKnown();

	rowCellsRemaining[row] -= 1;
	colCellsRemaining[col] -= 1;
	if (gameBoard[row][col].GetValue() == 0) {
		rowVoltorbsRemaining[row] -= 1;
		colVoltorbsRemaining[col] -= 1;
	}
	else {
		rowTotalsRemaining[row] -= gameBoard[row][col].GetValue();
		colTotalsRemaining[col] -= gameBoard[row][col].GetValue();
	}

}

void board::FlipCell(int row, int col, int val)
{
	if (val > 3 || val < 0) {
		return;
	}
	if (!gameBoard[row][col].CanBe(val)) {
		return;
	}
	gameBoard[row][col].Set(val);
	CalculatePhase2();
}

void board::GenerateLayouts(std::list<layout> *layoutList, int currentCell, layout *temp)
{
	// TODO: reduce memory usage (Possibly impossible)
	if (currentCell == 25) {
		// Verify Layout
		bool goodLayout = true;
		for(size_t i = 0; i < CELLS_PER_ROW; ++i)
		{
			int rv = 0, cv = 0, rt = 0, ct = 0;
			for(size_t j = 0; j < CELLS_PER_ROW; ++j)
			{
				// Checking Row i
				switch(temp->get(i, j)) {
				case 0:
					rv++;
					break;
				case 1:
					rt++;
					break;
				case 2:
					rt += 2;
					break;
				case 3:
					rt += 3;
					break;
				default:
					break;
				}

				// Checking col i
				switch(temp->get(j, i)) {
				case 0:
					cv++;
					break;
				case 1:
					ct++;
					break;
				case 2:
					ct += 2;
					break;
				case 3:
					ct += 3;
					break;
				default:
					break;
				}
			}
			if(rt != rowTotals[i]) {
				goodLayout = false;
			}
			if(ct != colTotals[i]) {
				goodLayout = false;
			}
			if(rv != rowVoltorbs[i]) {
				goodLayout = false;
			}
			if(cv != colVoltorbs[i]) {
				goodLayout = false;
			}
		}

		if(goodLayout) {
			layoutList->push_front(*temp);
		}
		return;
	}

	//Back up previous state of gameboard
	cell previousState[CELLS_PER_ROW][CELLS_PER_ROW];
	int previousRowTotalsRemaining[CELLS_PER_ROW];   // Going down the right
	int previousRowVoltorbsRemaining[CELLS_PER_ROW];
	int previousRowCellsRemaining[CELLS_PER_ROW];
	int previousColTotalsRemaining[CELLS_PER_ROW];   // Across the bottom
	int previousColVoltorbsRemaining[CELLS_PER_ROW];
	int previousColCellsRemaining[CELLS_PER_ROW];

	for (size_t i = 0; i < CELLS_PER_ROW; i++)
	{
		previousRowTotalsRemaining[i] = rowTotalsRemaining[i]; 
		previousRowVoltorbsRemaining[i] = rowVoltorbsRemaining[i];
		previousRowCellsRemaining[i] = rowCellsRemaining[i];
		previousColTotalsRemaining[i] = colTotalsRemaining[i];
		previousColVoltorbsRemaining[i] = colVoltorbsRemaining[i];
		previousColCellsRemaining[i] = colCellsRemaining[i];
		for (size_t j = 0; j < CELLS_PER_ROW; j++)
		{
			previousState[i][j] = gameBoard[i][j];
		}
	}

	// Do main checking
	if (gameBoard[currentCell / 5][currentCell % 5].IsKnown()) {
		temp->set(currentCell / 5, currentCell % 5, gameBoard[currentCell / 5][currentCell % 5].GetValue());
		GenerateLayouts(layoutList, currentCell + 1, temp);

		//restore Previous Layout before ending
		for (size_t i = 0; i < CELLS_PER_ROW; i++)
		{
			rowTotalsRemaining[i] =	previousRowTotalsRemaining[i];
			rowVoltorbsRemaining[i] = previousRowVoltorbsRemaining[i];
			rowCellsRemaining[i] = previousRowCellsRemaining[i];
			colTotalsRemaining[i] =	previousColTotalsRemaining[i];
			colVoltorbsRemaining[i] = previousColVoltorbsRemaining[i];
			colCellsRemaining[i] = previousColCellsRemaining[i];
			for (size_t j = 0; j < CELLS_PER_ROW; j++)
			{
				gameBoard[i][j] = previousState[i][j];
			}
		}
	}
	else { //Cell is not known
		for (size_t i = 0; i < 4; i++)
		{
			if (gameBoard[currentCell / 5][currentCell % 5].CanBe(i)) {
				FlipCell(currentCell / 5, currentCell % 5, i);
				temp->set(currentCell / 5, currentCell % 5, i);
				GenerateLayouts(layoutList, currentCell + 1, temp);

				//restore Previous Layout before trying next combo
				for (size_t j = 0; j < CELLS_PER_ROW; j++)
				{
					rowTotalsRemaining[j] = previousRowTotalsRemaining[j];
					rowVoltorbsRemaining[j] = previousRowVoltorbsRemaining[j];
					rowCellsRemaining[j] = previousRowCellsRemaining[j];
					colTotalsRemaining[j] = previousColTotalsRemaining[j];
					colVoltorbsRemaining[j] = previousColVoltorbsRemaining[j];
					colCellsRemaining[j] = previousColCellsRemaining[j];
					for (size_t k = 0; k < CELLS_PER_ROW; k++)
					{
						gameBoard[j][k] = previousState[j][k];
					}
				}
			}
		}
	}
}

void board::PopulateValues(int rt[CELLS_PER_ROW], int rv[CELLS_PER_ROW], int ct[CELLS_PER_ROW], int cv[CELLS_PER_ROW]) {
	for (size_t i = 0; i < CELLS_PER_ROW; ++i) {
		rowTotalsRemaining[i] = rowTotals[i] = rt[i];
		rowVoltorbsRemaining[i] = rowVoltorbs[i] = rv[i];
		colTotalsRemaining[i] = colTotals[i] = ct[i];
		colVoltorbsRemaining[i] = colVoltorbs[i] = cv[i];
	}
}
