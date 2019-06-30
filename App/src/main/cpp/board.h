#pragma once
#include <list>

#include "cell.h"
#include "layout.h"

//const int CELLS_PER_ROW = 5;

class board
{
public:
	board();
	~board();
	void PopulateValues(int rt[CELLS_PER_ROW], int rv[CELLS_PER_ROW], int ct[CELLS_PER_ROW], int cv[CELLS_PER_ROW]);
//	cell getCell(int row, int col);
//	bool Win();
	void Initialize();
//	int GetCalculationNumber();
	void CalculatePhase2();
	void MakeCellKnown(int row, int col);
	void FlipCell(int row, int col, int val);
	void GenerateLayouts(std::list<layout> *layoutList, int currentCell, layout *temp);
private:
	int rowTotals[CELLS_PER_ROW];			// Going down the right
	int rowVoltorbs[CELLS_PER_ROW];

	int colTotals[CELLS_PER_ROW];			// Across the bottom
	int colVoltorbs[CELLS_PER_ROW];
	
	int rowTotalsRemaining[CELLS_PER_ROW];   // Going down the right
	int rowVoltorbsRemaining[CELLS_PER_ROW];
	int rowCellsRemaining[CELLS_PER_ROW] = {5,5,5,5,5};

	int colTotalsRemaining[CELLS_PER_ROW];   // Across the bottom
	int colVoltorbsRemaining[CELLS_PER_ROW];
	int colCellsRemaining[CELLS_PER_ROW] = {5,5,5,5,5};

	cell gameBoard[CELLS_PER_ROW][CELLS_PER_ROW];	//The Board
};

