//
// Created by Jolteon on 7/5/2018.
//

#ifndef VOLTORBFLIPCALCULATOR2_GAME_H
#define VOLTORBFLIPCALCULATOR2_GAME_H

#include "layout.h"

class Game {
private:
    bool firstFlip;
    int points = 0;
    int level = 1;
    int pointCellsFlipped = 0;
    int pointValues[9] = {20,50,100,200,350,600,1000,2000}; //Max for a level is pointValues[level], min is pointValues[level-1]
    layout currentLayout;
    bool flipped[CELLS_PER_ROW][CELLS_PER_ROW];
    bool flagged[CELLS_PER_ROW][CELLS_PER_ROW];
    int rowTotals[CELLS_PER_ROW];
    int colTotals[CELLS_PER_ROW];
    int rowVoltorbs[CELLS_PER_ROW];
    int colVoltorbs[CELLS_PER_ROW];
public:
    Game();
    int getPoints();
    int getTotal(bool isVoltorb, bool isCol, int index);
    void nextLevel();
    int getLevel();
    void calculateBoard();
    void calculateTotals();
    int flipCell(int row, int col);
    int getCellVal(int row, int col);
    bool isFlipped(int row, int col);
    bool isWin();
    void flag(int row, int col);
    void unflag(int row, int col);
    bool isFlagged(int row, int col);
    int getNextLevel();
};

#endif //VOLTORBFLIPCALCULATOR2_GAME_H
