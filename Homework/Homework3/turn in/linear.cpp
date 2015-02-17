//
//  linear.cpp
//  Problem2
//
//  Created by Gautam Gupta on 2/5/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

// Return true if any of the array elements is negative, false
// otherwise.
bool anyNegative(const double a[], int n)
{
    if (*a < 0)
        return true;
    if (n <= 0)
        return false;
    return anyNegative(a+1, n-1);
}

// Return the number of negative elements in the array.
int countNegatives(const double a[], int n)
{
    if (n <= 0)
        return 0;
    if (*a < 0)
        return 1+countNegatives(a+1, n-1);
    return countNegatives(a+1, n-1);
}

// Return the subscript of the first negative element in the array.
// If no element is negative, return -1.
int firstNegative(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (*a < 0)
        return 0;
    int k = 1+firstNegative(a+1, n-1);
    if (k == 0)
        return -1;
    return k;
}

// Return the subscript of the smallest element in the array.  If
// more than one element has the same smallest value, return the
// smallest subscript of such an element.  If the array has no
// elements to examine, return -1.
int indexOfMin(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (n == 1)
        return 0;
    int j = 1 + indexOfMin(a+1, n-1);
    if (a[0] <= a[j])
        return 0;
    return j;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
        return true;
    if (n1 < n2)
        return false;
    if (*a1 == *a2)
        return includes(a1+1, n1-1, a2+1, n2-1);
    return includes(a1+1, n1-1, a2, n2);
}
