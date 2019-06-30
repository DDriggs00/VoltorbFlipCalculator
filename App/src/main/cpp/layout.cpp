#include <iostream>

#include "layout.h"

layout::layout()
{
}

layout::layout(int Layout[CELLS_PER_ROW][CELLS_PER_ROW])
{
	for (size_t i = 0; i < CELLS_PER_ROW; i++)
	{
		for (size_t j = 0; j < CELLS_PER_ROW; j++)
		{
			currentLayout[i][j] = Layout[i][j];
		}
	}
}


layout::~layout()
{
}

void layout::set(int row, int col, int val)
{
	currentLayout[row][col] = val;
}

int layout::get(int row, int col)
{
	return currentLayout[row][col];
}
