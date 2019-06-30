//
// Created by Metagross on 5/20/2018.
//

#ifndef VOLTORBFLIPCALCULATOR2_MAINCPPCLASS_H
#define VOLTORBFLIPCALCULATOR2_MAINCPPCLASS_H

#include <list>

#include "cell.h"
#include "board.h"
#include "layout.h"
#include "cellStats.h"

class MainCppClass {
public:
    MainCppClass();
    ~MainCppClass();
    bool VerifyInputs(int rt[CELLS_PER_ROW], int rv[CELLS_PER_ROW], int ct[CELLS_PER_ROW], int cv[CELLS_PER_ROW]);
    bool Calculate(int rt[CELLS_PER_ROW], int rv[CELLS_PER_ROW], int ct[CELLS_PER_ROW], int cv[CELLS_PER_ROW]);
    void UpdateDisplayArray();
    bool FlipCell(int row, int col, int val);
    std::string getDisplayCell(int row, int col);
    int getTotalLayouts();
    int getMatchingLayouts(int row, int col, int val);
    bool isBestCell(int row, int col);
    bool IsKnown(int row, int col);

private:
    board mainBoard;
    layout temp;
    std::list<layout> layoutList;
    cellStats CurrentGameCellStats;
    std::string displayedBoard[CELLS_PER_ROW][CELLS_PER_ROW];
};


#endif //VOLTORBFLIPCALCULATOR2_MAINCPPCLASS_H
