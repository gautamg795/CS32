//
//  mazequeue.cpp
//  Problem3
//
//  Created by Gautam Gupta on 2/1/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include <iostream>
#include <queue>
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
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X..X...XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    queue<Coord> cQueue;
    cQueue.push({sr, sc});
    maze[sr][sc] = '*';
    while(!cQueue.empty())
    {
        int curRow = cQueue.front().r();
        int curCol = cQueue.front().c();
        cQueue.pop();
        steps++;
        if (curRow == er && curCol == ec)
            return true;
        if (maze[curRow-1][curCol] == '.') // North
        {
            cQueue.push({curRow-1, curCol});
            maze[curRow-1][curCol] = '*';
        }
        if (maze[curRow][curCol+1] == '.') // East
        {
            cQueue.push({curRow, curCol+1});
            maze[curRow][curCol+1] = '*';
        }
        if (maze[curRow+1][curCol] == '.') // South
        {
            cQueue.push({curRow+1, curCol});
            maze[curRow+1][curCol] = '*';
        }
        if (maze[curRow][curCol-1] == '.') // West
        {
            cQueue.push({curRow, curCol-1});
            maze[curRow][curCol-1] = '*';
        }
        
        
    }
    
    return false;
}