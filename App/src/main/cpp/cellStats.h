#pragma once
#include <list>
#include <string>

#include "layout.h"

//const int CELLS_PER_ROW = 5;

class cellStats
{
public:
	cellStats();
	cellStats(std::list<layout> layoutList);
	~cellStats();
	void Initialize(std::list<layout> layoutList);
	void update();
	void FlipCell(int row, int col, int val);
    bool CanBe(int row, int col, int val);
    bool IsKnown(int row, int col);
    std::string getDisplayValue(int row, int col);
    int GetTotalLayouts();
    int getMatchingLayouts(int row, int col, int val);
    bool IsBestCell(int row, int col);
private:
	int bestCellPercent = 100;
    bool bestCellSafe = true;
	std::list<layout> layoutListPointer;
	int totalOptions;
	int voltorbs[CELLS_PER_ROW][CELLS_PER_ROW];
	int ones[CELLS_PER_ROW][CELLS_PER_ROW];
	int twos[CELLS_PER_ROW][CELLS_PER_ROW];
	int threes[CELLS_PER_ROW][CELLS_PER_ROW];
	int percentVoltorb[CELLS_PER_ROW][CELLS_PER_ROW];
};

