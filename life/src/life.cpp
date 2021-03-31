/**
 * Opens up a game of life
 * @file life.cpp
 * @authors vikho305 & isaho220
 */

#include <iostream>
#include "grid.h"
#include "lifeutil.h"
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;

void tick(Grid<char>& grid);
void printGrid(const Grid<char>& grid);

int main() {
    Grid<char> grid;
    int gridWidth;
    int gridHeight;

    string rootPath = get_current_dir_name();
    string gridFileName;
    cout << "Grid input file name? ";
    cin >> gridFileName; // Player selects which grid to use
    ifstream gridFile (rootPath + "/" + gridFileName);

    // Create the selected grid
    if(gridFile.is_open()) {
        string gridRow;
        int loopCount = 0; // Used to know what each file lines means
        while(getline(gridFile, gridRow)) {
            if(loopCount == 0) // The file line is the grid height
            {
                stringstream str(gridRow);
                str >> gridHeight;
            }
            else if(loopCount == 1) // The file line is the grid width
            {
                stringstream str(gridRow);
                str >> gridWidth;
                grid.resize(gridHeight, gridWidth);
            }
            else // The file line is one grid row
            {
                int gridColumn = 0; // Used to know which column to place the next char
                for(char c : gridRow) {
                    grid.set(loopCount - 2, gridColumn, c);
                    gridColumn++;
                }
            }
            loopCount++;
        }
        gridFile.close();
    }

    // Game loop
    bool running = true;
    while(running) {
        printGrid(grid);
        cout << "a)nimate t)ick q)uit? ";
        string playerChoice;
        cin >> playerChoice; // Player selects which action to do

        if (playerChoice == "q") {
            running = false;
        }
        else if (playerChoice == "t") {
            tick(grid);
        }
        else if (playerChoice == "a") {
            for(int i = 0; i < 50; i++) {
                tick(grid);
                printGrid(grid);
                pause(100);
            }
            cout << "Animation ended!" << endl;
        }
    }

    cout << "Have a nice Life! "  << endl;
    return 0;
}

/**
 * Neatly prints a grid to the console
 * @param grid The grid which the game is played on
 */
void printGrid(const Grid<char>& grid) {
    for(int row = 0; row < grid.nRows; row++) {
        for(int column = 0; column < grid.nCols; column++) {
            cout << grid.get(row, column) << " ";
        }
        cout << endl;
    }
}

/**
 * Checks whether or not a particular cell is alive
 * @param grid The grid which the game is played on
 * @param row The row the cell is on
 * @param column The column the cell is in
 * @return Whether or not the cell is alive
 */
bool checkCellStatus(const Grid<char>& grid, int row, int column) {
    if(row < 0 || row >= grid.nRows) return false; // Act as if cell is dead if on non-existant row
    if(column < 0 || column >= grid.nCols) return false; // Act as if cell is dead if on non-existant column
    char gridCell = grid.get(row, column);
    return gridCell == 'X';
}

/**
 * Checks whether or not a particular cell should survive the next tick
 * @param grid The grid which the game is played on
 * @param row The row the cell is on
 * @param column The column the cell is in
 * @return Whether or not the cell should survive
 */
bool checkNextCellStatus(const Grid<char>& grid, int row, int column) {
    int neighbourCount = 0;

    // Count how many of the eight neighbouring cells are alive
    for(int offsetRow = -1; offsetRow <= 1; offsetRow++) {
        for(int offsetCol = -1; offsetCol <= 1; offsetCol++) {
            if (checkCellStatus(grid, row + offsetRow, column + offsetCol) && (offsetCol != 0 || offsetRow != 0))
                neighbourCount++;
        }
    }

    if (neighbourCount >= 4 || neighbourCount <= 1)
        return false;
    else if (neighbourCount == 2)
        return grid.get(row, column) == 'X';
    else
        return true;
}

/**
 * Moves the game forward one tick in time
 * @param grid The grid which the game is played on
 */
void tick(Grid<char>& grid) {
    Grid<char> newGrid;
    newGrid.resize(grid.nRows, grid.nCols);

    // Fill the new grid with the next generations cells
    for(int row = 0; row < grid.nRows; row++) {
        for(int column = 0; column < grid.nCols; column++) {
            if (checkNextCellStatus(grid, row, column))
                newGrid.set(row, column, 'X'); // Set the current cell to alive
            else
                newGrid.set(row, column, '-'); // Set the current cell to dead
        }
    }

    grid = newGrid;
}
