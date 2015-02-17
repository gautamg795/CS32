//
//  main.cpp
//  Problem1
//
//  Created by Gautam Gupta on 2/1/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//
#include <cassert>
#include <iostream>
#include <stack>
#include <string>
using namespace std;
class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

int main()
{
    
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> cStack;
    cStack.push({sr, sc});
    maze[sr][sc] = '*';
    while(!cStack.empty())
    {
        int curRow = cStack.top().r();
        int curCol = cStack.top().c();
        cStack.pop();
        if (curRow == er && curCol == ec)
            return true;
        if (maze[curRow-1][curCol] == '.') // North
        {
            cStack.push({curRow-1, curCol});
            maze[curRow-1][curCol] = '*';
        }
        if (maze[curRow][curCol+1] == '.') // East
        {
            cStack.push({curRow, curCol+1});
            maze[curRow][curCol+1] = '*';
        }
        if (maze[curRow+1][curCol] == '.') // South
        {
            cStack.push({curRow+1, curCol});
            maze[curRow+1][curCol] = '*';
        }
        if (maze[curRow][curCol-1] == '.') // West
        {
            cStack.push({curRow, curCol-1});
            maze[curRow][curCol-1] = '*';
        }
        
            
    }
        
    return false;
}