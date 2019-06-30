#include "cellStats.h"

cellStats::cellStats(std::list<layout> layoutList)
{
    // Keep a copy of the list pointer
    layoutListPointer = layoutList;

    update();
}

cellStats::~cellStats()
{
}

void cellStats::update()
{
    // Set the number of total options
    totalOptions = layoutListPointer.size();

    // Initialize number lists to 0 before using incrementers
    for (size_t i = 0; i < CELLS_PER_ROW; i++)
    {
        for (size_t j = 0; j < CELLS_PER_ROW; j++)
        {
            voltorbs[i][j] = 0;
            ones[i][j] = 0;
            twos[i][j] = 0;
            threes[i][j] = 0;
			percentVoltorb[i][j] = 100;
        }
    }

    // Populate the lists of numbers
    for (std::list<layout>::iterator Layout = layoutListPointer.begin(); Layout != layoutListPointer.end(); Layout++)
    {
        for (size_t i = 0; i < CELLS_PER_ROW; i++)
        {
            for (size_t j = 0; j < CELLS_PER_ROW; j++)
            {
                switch (Layout->get(i, j))
                {
                    case 0:
                        voltorbs[i][j]++;
                        break;
                    case 1:
                        ones[i][j]++;
                        break;
                    case 2:
                        twos[i][j]++;
                        break;
                    case 3:
                        threes[i][j]++;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    // Populate the Voltorb Percent array
    for (size_t i = 0; i < CELLS_PER_ROW; i++)
    {
        for (size_t j = 0; j < CELLS_PER_ROW; j++)
        {
            percentVoltorb[i][j] = (voltorbs[i][j] / (float)totalOptions) * 100;
        }
    }

    // Find the best cell
    int tempBestPercent = 100;
    for (size_t i = 0; i < CELLS_PER_ROW; i++)
    {
        for (size_t j = 0; j < CELLS_PER_ROW; j++)
        {

            if (percentVoltorb[i][j] < tempBestPercent && !IsKnown(i, j) && (CanBe(i, j, 2) || CanBe(i, j, 3))) {
                tempBestPercent = percentVoltorb[i][j];

            }
        }
    }
    bestCellSafe = (tempBestPercent == 0);
	bestCellPercent = tempBestPercent;
}

void cellStats::FlipCell(int row, int col, int val)
{
    std::list<layout>::iterator Layout = layoutListPointer.begin();
    while (Layout != layoutListPointer.end()) {
        if (Layout->get(row, col) != val) {
            Layout = layoutListPointer.erase(Layout);
        }
        else {
            Layout++;
        }
    }

    update();
}

void cellStats::Initialize(std::list<layout> layoutList) {

    // Keep a copy of the list pointer
    layoutListPointer = layoutList;

    update();
}

cellStats::cellStats() {

}

bool cellStats::CanBe(int row, int col, int val) {
    switch(val) {
    case 0:
        if(voltorbs[row][col] != 0) {
            return true;
        }
        break;
    case 1:
        if(ones[row][col] != 0) {
            return true;
        }
        break;
    case 2:
        if(twos[row][col] != 0) {
            return true;
        }
        break;
    case 3:
        if(threes[row][col] != 0) {
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

std::string cellStats::getDisplayValue(int row, int col) {
    if(IsKnown(row, col)) {
        if(CanBe(row, col, 0)) {
            return "V";
        }
        else if(CanBe(row, col, 1)) {
            return "1";
        }
        else if(CanBe(row, col, 2)) {
            return "2";
        }
        else if(CanBe(row, col, 3)) {
            return "3";
        }
    }
    else if(!CanBe(row, col, 2) && !CanBe(row, col, 3)) {
        return "V/1";
    }
    else if(!CanBe(row, col, 0) && !CanBe(row, col, 1)) {
        return "2/3";
    }
    //if not known
    return std::to_string(percentVoltorb[row][col]) + "%";
}

bool cellStats::IsKnown(int row, int col) {
    int possibleValues = 0;
    if(voltorbs[row][col] != 0) {
        possibleValues++;
    }
    if(ones[row][col] != 0) {
        possibleValues++;
    }
    if(twos[row][col] != 0) {
        possibleValues++;
    }
    if(threes[row][col] != 0) {
        possibleValues++;
    }
	if(possibleValues == 1) {
        return true;
    }
    else {
        return false;
    }
}
	int cellStats::GetTotalLayouts() {
    return totalOptions;
}

int cellStats::getMatchingLayouts(int row, int col, int val) {
    switch(val) {
        case 0:
            return voltorbs[row][col];
        case 1:
            return ones[row][col];
        case 2:
            return twos[row][col];
        case 3:
            return threes[row][col];
        default:
            return -1;
    }
}

bool cellStats::IsBestCell(int row, int col) {
    if(bestCellSafe || percentVoltorb[row][col] != bestCellPercent || IsKnown(row, col) || (!CanBe(row, col, 2) && !CanBe(row, col, 3))) {
        return false;
    }
    return true;
}
