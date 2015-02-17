#include <iostream>
#include <streambuf>
#include <algorithm>
#include <cstdlib>
#include <cassert>
using namespace std;

#include "/Users/gautam/Dropbox/School/CS32/Homework/Homework3/Homework3/sport.cpp"
#include "/Users/gautam/Dropbox/School/CS32/Homework/Homework3/Problem2/linear.cpp"
#include "/Users/gautam/Dropbox/School/CS32/Homework/Homework3/Problem3/maze.cpp"
#include "/Users/gautam/Dropbox/School/CS32/Homework/Homework3/Problem4/tree.cpp"


void testone(int n)
{
	static string maze1[10] = {
        "XXXXXXXXXX",
        "X.X..X...X",
        "X.XX.X.XXX",
        "X....X.X.X",
        "XX.X.X...X",
        "XXX..X.X.X",
        "X...X...XX",
        "X.XX..X.XX",
        "X....X...X",
        "XXXXXXXXXX",
	};
	
	static string maze2[10] = {
        "XXXXXXXXXX",
        "X.X..X...X",
        "X.XX.X.XXX",
        "X....X.X.X",
        "XX.X.X...X",
        "XXX.XX.X.X",
        "X...X...XX",
        "X.XX..X.XX",
        "X....X...X",
        "XXXXXXXXXX",
	};
	
	static string maze3[10] = {
        "XXXXXXXXXX",
        "XX.....XXX",
        "X..XX....X",
        "X...X...XX",
        "X.X.XXX..X",
        "XXXX..X..X",
        "XX....X..X",
        "X.......XX",
        "X..XXXXXXX",
        "XXXXXXXXXX",
	};
	
	static string maze4[10] = {
        "XXXXXXXXXX",
        "XX.....XXX",
        "X..XX....X",
        "X...X...XX",
        "X.X.XXX..X",
        "XXXX..X..X",
        "XX....X..X",
        "X.....X.XX",
        "X..XXXXXXX",
        "XXXXXXXXXX",
	};
	
	double a[10] = { 3, 6, -5, 5, -2, 1, -7, -4, 8, 2 };
	double b[4] = { 6, -5, -2, -4 };
	double c[8] = { 10, 50, 40, 20, 50, 40, 30 };
	double d[12] = { 10, 20, 40, 10, 40, 30, 20, 10, 40, 50, 40, 30 };
	double e[12] = { 10, 10, 10, 20, 20, 30, 30, 40, 40, 40, 40, 50 };
	double f[16] = { 40, 70, 10, 120, 30, 40, 50, 110,
        90, 20, 10, 60, 100, 110, 40, 80 };
	double g[16] = { 120, 110, 110, 100, 90, 80, 70, 60,
        50, 40, 40, 40, 30, 20, 10, 10 };
    
	switch (n)
	{
        default: {
            assert(false);
        } break; case  1: {
            assert(anyNegative(a, 10));
        } break; case  2: {
            assert(anyNegative(a, 8));
        } break; case  3: {
            assert(anyNegative(a+2, 8));
        } break; case  4: {
            assert(!anyNegative(a+8, 2));
        } break; case  5: {
            assert(anyNegative(a+6, 2));
        } break; case  6: {
            assert(!anyNegative(a, 1));
        } break; case  7: {
            assert(anyNegative(a+2, 1));
        } break; case  8: {
            assert(!anyNegative(a, 0)  &&  !anyNegative(a+2, 0));
        } break; case  9: {
            assert(countNegatives(a, 10) == 4);
        } break; case 10: {
            assert(countNegatives(a, 8) == 4);
        } break; case 11: {
            assert(countNegatives(a+2, 8) == 4);
        } break; case 12: {
            assert(countNegatives(a+8, 2) == 0);
        } break; case 13: {
            assert(countNegatives(a+6, 2) == 2);
        } break; case 14: {
            assert(countNegatives(a, 1) == 0);
        } break; case 15: {
            assert(countNegatives(a+2, 1) == 1);
        } break; case 16: {
            assert(countNegatives(a, 0) == 0  &&  countNegatives(a+2, 0) == 0);
        } break; case 17: {
            assert(firstNegative(a, 10) == 2);
        } break; case 18: {
            assert(firstNegative(a, 8) == 2);
        } break; case 19: {
            assert(firstNegative(a+2, 8) == 0);
        } break; case 20: {
            assert(firstNegative(a+8, 2) == -1);
        } break; case 21: {
            assert(firstNegative(a+6, 2) == 0);
        } break; case 22: {
            assert(firstNegative(a, 1) == -1);
        } break; case 23: {
            assert(firstNegative(a+2, 1) == 0);
        } break; case 24: {
            assert(firstNegative(a, 0) == -1  &&  firstNegative(a+2, 0) == -1);
        } break; case 25: {
            assert(indexOfMin(a, 10) == 6);
        } break; case 26: {
            assert(indexOfMin(a, 7) == 6);
        } break; case 27: {
            assert(indexOfMin(a+6, 4) == 0);
        } break; case 28: {
            assert(indexOfMin(a+8, 2) == 1);
        } break; case 29: {
            assert(indexOfMin(a+7, 2) == 0);
        } break; case 30: {
            assert(indexOfMin(a, 1) == 0);
        } break; case 31: {
            assert(indexOfMin(a+2, 1) == 0);
        } break; case 32: {
            double x[8] = { 3, 6, -5, 2, -5, -5, -5, 2 };
            assert(indexOfMin(x, 8) == 2);
        } break; case 33: {
            assert(indexOfMin(a, 0) == -1  &&  indexOfMin(a+2, 0) == -1);
        } break; case 34: {
            assert(includes(a, 10, b, 4));
        } break; case 35: {
            assert(includes(a+1, 9, b, 4));
        } break; case 36: {
            assert(!includes(a+2, 8, b, 4));
        } break; case 37: {
            assert(!includes(a+1, 6, b, 4));
        } break; case 38: {
            assert(includes(a+1, 7, b, 4));
        } break; case 39: {
            assert(includes(a, 10, b+2, 1));
        } break; case 40: {
            assert(includes(a+1, 9, b, 1));
        } break; case 41: {
            assert(!includes(a+2, 8, b, 1));
        } break; case 42: {
            assert(includes(a, 10, b, 0));
        } break; case 43: {
            assert(includes(a, 0, b, 0));
        } break; case 44: {
            assert(includes(a+1, 2, b, 2));
        } break; case 45: {
            assert(!includes(a+2, 2, b+1, 2));
        } break; case 46: {
            assert(!includes(a+2, 2, b+1, 3));
        } break; case 47: {
            assert(!includes(a+1, 1, b, 2));
        } break; case 48: {
            assert(!includes(a+1, 0, b, 1));
        } break; case 49: {
            assert(includes(c, 7, d+3, 3));
        } break; case 50: {
            assert(!includes(c, 7, d+6, 3));
        } break; case 51: {
            assert(includes(c, 7, d+9, 3));
        } break; case 52: {
            assert(countIncludes(c, 7, d, 3) == 1);
        } break; case 53: {
            assert(countIncludes(c, 7, d+3, 3) == 2);
        } break; case 54: {
            assert(countIncludes(c, 7, d+6, 3) == 0);
        } break; case 55: {
            assert(countIncludes(c, 7, d+9, 3) == 3);
        } break; case 56: {
            assert(countIncludes(c, 7, d+2, 1) == 2);
        } break; case 57: {
            assert(countIncludes(c, 2, d+2, 1) == 0);
        } break; case 58: {
            assert(countIncludes(c, 7, d, 0) == 1);
        } break; case 59: {
            assert(countIncludes(c, 0, d, 0) == 1);
        } break; case 60: {
            order(f, 16);
            assert(equal(f, f+16, g));
        } break; case 61: {
            order(f, 1);
            assert(f[0] == 40);
        } break; case 62: {
            order(f, 0);
            assert(f[0] == 40);
        } break; case 63: {
            assert(pathExists(maze1, 10, 10, 8, 6, 1, 1));
            break;
        } break; case 64: {
            assert(!pathExists(maze2, 10, 10, 8, 6, 1, 1));
            break;
        } break; case 65: {
            assert(pathExists(maze3, 10, 10, 4, 3, 7, 1));
            break;
        } break; case 66: {
            assert(!pathExists(maze4, 10, 10, 4, 3, 7, 1));
            break;
        }
	}
}



int main()
{
    cout << "Enter test number: ";
    int n;
    cin >> n;
    testone(n);
    cout << "Passed" << endl;
}