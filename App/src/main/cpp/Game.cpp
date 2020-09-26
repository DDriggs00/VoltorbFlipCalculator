//
// Created by Jolteon on 7/5/2018.
//

#include <cstdlib>
#include <algorithm>
#include <random>
#include "Game.h"

Game::Game() {
    for (int i = 0; i < CELLS_PER_ROW; ++i) {
        rowTotals[i] = 0;
        colTotals[i] = 0;
        rowVoltorbs[i] = 0;
        colVoltorbs[i] = 0;
        for (int j = 0; j < CELLS_PER_ROW; ++j) {
            flipped[i][j] = false;
        }
    }
    level = 0;
    points = 0;
    pointCellsFlipped = 0;
    nextLevel();

}

int Game::getPoints() {
    return points;
}

int Game::getTotal(bool isVoltorb, bool isCol, int index) {
    if(isVoltorb) {
        if(isCol) {
            return colVoltorbs[index];
        }
        else {
            return rowVoltorbs[index];
        }
    }
    else {
        if(isCol) {
            return colTotals[index];
        }
        else {
            return rowTotals[index];
        }
    }
}

void Game::nextLevel() {
    level = getNextLevel();
    points = 0;
    pointCellsFlipped = 0;
    for (int i = 0; i < CELLS_PER_ROW; ++i) {
        for (int j = 0; j < CELLS_PER_ROW; ++j) {
            flipped[i][j] = false;
        }
    }
    firstFlip = true;
    calculateBoard();
    calculateTotals();
}

void Game::calculateBoard() {
    srand(static_cast<unsigned int>(time(0)));
    struct RNG {
        int operator() (int n) {
            return (int)(std::rand() / (1.0 + RAND_MAX) * n);
        }
    };
    int points = 1;
    int voltorbs = level + CELLS_PER_ROW;
    int unorderedCells[CELLS_PER_ROW*CELLS_PER_ROW];
    for (int i = 0; i < (CELLS_PER_ROW*CELLS_PER_ROW); ++i) {
        if(voltorbs > 0) {
            unorderedCells[i] = 0;
            voltorbs--;
        }
        else if(points <= pointValues[level-1]) {
            if((rand() % 2) == 0) {
                points *= 3;
                unorderedCells[i] = 3;
            }
            else {
                points *= 2;
                unorderedCells[i] = 2;
            }
        }
        else {
            unorderedCells[i] = 1;
        }
    }
    //Check for too low
    if(points < pointValues[level-1]) {
        for (int i = 0; i < CELLS_PER_ROW * CELLS_PER_ROW; ++i) {
            if(unorderedCells[i] == 2) {
                unorderedCells[i] = 3;
                points /= 2;
                points *= 3;
                if(points <= pointValues[level-1]) {
                    return;
                }
            }
        }
    }
    //Check for too high
    bool noThrees = false;
    bool notEnoughTwos = false;
    while(points > pointValues[level]) {
        if(((rand() % 2) == 0 || notEnoughTwos) && (!noThrees)) {
            for (int i = 0; i < CELLS_PER_ROW * CELLS_PER_ROW; ++i) {
                if(unorderedCells[i] == 3) {
                    unorderedCells[i] = 2;
                    points /= 3;
                    points *= 2;
                    notEnoughTwos = false;
                    break;
                }
                else if(i == CELLS_PER_ROW * CELLS_PER_ROW - 1) {
                    noThrees = true;
                }
            }
        }
        else {
            int temp1 = -1;
            int temp2 = -1;
            for (int i = 0; i < CELLS_PER_ROW * CELLS_PER_ROW; ++i) {
                if(unorderedCells[i] == 2) {
                    if(temp1 == -1) {
                        temp1 = i;
                    }
                    else {
                        temp2 = i;
                        break;
                    }
                }
            }
            if(temp2 != -1) {
                unorderedCells[temp1] = 3;
                unorderedCells[temp2] = 1;
                points /= 4;
                points *= 3;
                noThrees = false;
            } else {
                notEnoughTwos = true;
            }
        }
    }
    std::shuffle(std::begin(unorderedCells), std::end(unorderedCells), std::mt19937(std::random_device()()));

    for (int i = 0; i < CELLS_PER_ROW; ++i) {
        for (int j = 0; j < CELLS_PER_ROW; ++j) {
            currentLayout.set(i,j,unorderedCells[5*i+j]);
        }
    }
}

void Game::calculateTotals() {
    for (int i = 0; i < CELLS_PER_ROW; ++i) {
        rowTotals[i] = 0;
        colTotals[i] = 0;
        rowVoltorbs[i] = 0;
        colVoltorbs[i] = 0;
    }

    for (int i = 0; i < CELLS_PER_ROW; ++i) {
        for (int j = 0; j < CELLS_PER_ROW; ++j) {
            if(currentLayout.get(i,j) == 0) {
                rowVoltorbs[i]++;
                colVoltorbs[j]++;
            }
            else {
                rowTotals[i] += currentLayout.get(i,j);
                colTotals[j] += currentLayout.get(i,j);
            }
        }
    }
}

int Game::flipCell(int row, int col) {
    if(flipped[row][col]) {
        return -1;
    }
    flipped[row][col] = true;
    if(firstFlip) {
        points = 1;
        firstFlip = false;
    }
    points *= currentLayout.get(row,col);

    if(currentLayout.get(row, col) > 1) {
        pointCellsFlipped += 1;
    }

    return currentLayout.get(row,col);
}

int Game::getCellVal(int row, int col) {
    return currentLayout.get(row, col);
}

bool Game::isFlipped(int row, int col) {
    return flipped[row][col];
}

bool Game::isWin() {
    for (int i = 0; i < CELLS_PER_ROW; ++i) {
        for (int j = 0; j < CELLS_PER_ROW; ++j) {
            if((!isFlipped(i,j)) && (getCellVal(i,j) >= 2)) {
                return false;
            }
        }
    }

    return true;
}

int Game::getLevel() {
    return level;
}

void Game::flag(int row, int col) {
    flagged[row][col] = true;
}

void Game::unflag(int row, int col) {
    flagged[row][col] = false;
}

bool Game::isFlagged(int row, int col) {
    return flagged[row][col];
}

int Game::getNextLevel() {
    if(points != 0) { // You Won!
        return level + 1;
    }
    else if((1 < pointCellsFlipped) && (pointCellsFlipped <= (level + 1))) {
        return pointCellsFlipped - 1;
    }
    else { //No point Cells Flipped
        return 1;
    }
}
