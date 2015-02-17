//
//  History.cpp
//  Project1
//
//  Created by Gautam Gupta on 1/8/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//
#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;
History::History(int nRows, int nCols)
: m_rows(nRows), m_cols(nCols)
{
    for (int j = 0; j < nRows; j++)
        for (int k = 0; k < nCols; k++)
            m_grid[j][k] = 0;
}

bool History::record(int r, int c)
{
    if (r > m_rows || c > m_cols || r < 1 || c < 1)
        return false;
    m_grid[r-1][c-1] += 1;
    return true;
}

void History::display() const
{
    clearScreen();
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    for (r = 0; r < m_rows; r++)
        for (c = 0; c < m_cols; c++)
        {
            if (m_grid[r][c] == 0)
                displayGrid[r][c] = '.';
            else if (m_grid[r][c] <= 26)
                displayGrid[r][c] = 'A' + m_grid[r][c] - 1;
            else
                displayGrid[r][c] = 'Z';
        }
    //Display the grid
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
            cout << displayGrid[r][c];
        cout << endl;
    }
    cout << endl;

}