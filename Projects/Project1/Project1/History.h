//
//  History.h
//  Project1
//
//  Created by Gautam Gupta on 1/8/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#ifndef __Project1__History__
#define __Project1__History__

#include "globals.h"
class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
private:
    int m_grid[MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
    
};
#endif /* defined(__Project1__History__) */
