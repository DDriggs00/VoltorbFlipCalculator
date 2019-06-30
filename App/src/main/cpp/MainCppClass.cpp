#include "MainCppClass.h"

MainCppClass::~MainCppClass() {

}

MainCppClass::MainCppClass() {

}

bool MainCppClass::VerifyInputs(int *rt, int *rv, int *ct, int *cv) {
    // 1. Less than 5 total
    // 2. Too Many Voltorbs
    for (size_t i = 0; i < CELLS_PER_ROW; i++)
    {
        if (rt[i] + rv[i] < CELLS_PER_ROW) {
            return false;
        }
        if (ct[i] + cv[i] < CELLS_PER_ROW) {
            return false;
        }
        if (rv[i] > CELLS_PER_ROW) {
            return false;
        }
        if (cv[i] > CELLS_PER_ROW) {
            return false;
        }
    }
    // 2. Too large numbers
    for (size_t i = 0; i < CELLS_PER_ROW; i++)
    {
        int rowNumberCells = CELLS_PER_ROW - rv[i];
        int colNumberCells = CELLS_PER_ROW - cv[i];
        if (rt[i] - 3 * rowNumberCells > 0) {
            return false;
        }
        if (ct[i] - 3 * colNumberCells > 0) {
            return false;
        }
    }
    // 3. different total in rows vs. Columns
    // 4. different number of Voltorbs in rows vs. Columns
    int totalRowValue = 0;
    int totalColValue = 0;
    int totalRowVoltorbs = 0;
    int totalColVoltorbs = 0;
    for (size_t i = 0; i < CELLS_PER_ROW; i++)
    {
        totalRowValue += rt[i];
        totalColValue += ct[i];
        totalRowVoltorbs += rv[i];
        totalColVoltorbs += cv[i];
    }
    if (totalRowValue != totalColValue) {
        return false;
    }
    if (totalRowVoltorbs != totalColVoltorbs) {
        return false;
    }
    return true;
}

bool MainCppClass::Calculate(int *rt, int *rv, int *ct, int *cv) {
    mainBoard.PopulateValues(rt, rv, ct, cv);
    mainBoard.Initialize();

    mainBoard.GenerateLayouts(&layoutList, 0, &temp);

	if(layoutList.size() == 0) {
		return false;
	}

    CurrentGameCellStats.Initialize(layoutList);

    UpdateDisplayArray();

	return true;
}

std::string MainCppClass::getDisplayCell(int row, int col) {
    return displayedBoard[row][col];
}

bool MainCppClass::FlipCell(int row, int col, int val) {
    if(val < 0 || val > 3 || !CurrentGameCellStats.CanBe(row, col, val)) {
        return false;
    }
    CurrentGameCellStats.FlipCell(row, col, val);
    UpdateDisplayArray();
    return true;
}

void MainCppClass::UpdateDisplayArray() {
    for (size_t i = 0; i < CELLS_PER_ROW; ++i) {
        for (size_t j = 0; j < CELLS_PER_ROW; ++j) {
            displayedBoard[i][j] = CurrentGameCellStats.getDisplayValue(i, j);
        }
    }
}

int MainCppClass::getTotalLayouts() {
    return CurrentGameCellStats.GetTotalLayouts();
}

int MainCppClass::getMatchingLayouts(int row, int col, int val) {
    return CurrentGameCellStats.getMatchingLayouts(row, col, val);
}

bool MainCppClass::isBestCell(int row, int col) {
    return CurrentGameCellStats.IsBestCell(row, col);
}

bool MainCppClass::IsKnown(int row, int col) {
    return CurrentGameCellStats.IsKnown(row, col);
}
