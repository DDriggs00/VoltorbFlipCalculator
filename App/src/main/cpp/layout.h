#pragma once

const int CELLS_PER_ROW = 5;

class layout
{
public:
	layout();
	layout(int Layout[CELLS_PER_ROW][CELLS_PER_ROW]);
	~layout();
	void set(int row, int col, int val);
	int get(int row, int col);
private:
	int currentLayout[CELLS_PER_ROW][CELLS_PER_ROW];
};

