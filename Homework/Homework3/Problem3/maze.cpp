//
//  maze.cpp
//  Problem3
//
//  Created by Gautam Gupta on 2/5/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = '*';
    if (maze[sr-1][sc] == '.') // North
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
    if (maze[sr][sc+1] == '.') // East
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    if (maze[sr+1][sc] == '.') // South
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    if (maze[sr][sc-1] == '.') // West
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    return false;
}