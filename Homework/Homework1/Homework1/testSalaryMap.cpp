//
//  testSalaryMap.cpp
//  Homework1
//
//  Created by Gautam Gupta on 1/16/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//
#include "SalaryMap.h"
#include <iostream>
#include <cassert>
int main()
{
    SalaryMap s;
    s.add("Employee1", 40000);
    assert(s.size() == 1);
    s.add("Employee2", 100000);
    assert(s.size() == 2);
    assert(s.salary("Employee1") == 40000 && s.salary("Employee2") == 100000);
    assert(s.raise("Employee2", 10));
    assert(s.salary("Employee2") == 110000);
    assert(!s.raise("Employee1", -1000));
    assert(s.salary("Employee1") == 40000);
    assert(s.raise("Employee1", -50));
    assert(s.salary("Employee1") == 20000);
    std::cerr << "Original SalaryMap printing...\n";
    s.print();
    SalaryMap s2(s);
    assert(s2.size() == 2);
    assert(s2.salary("Employee1") == 20000);
    assert(s2.salary("Employee2") == 110000);
    std::cerr << "\nCopy-constructed SalaryMap printing...\n";
    s2.print();
    SalaryMap s3;
    s3 = s;
    assert(s3.size() == 2);
    assert(s3.salary("Employee1") == 20000);
    assert(s3.salary("Employee2") == 110000);
    std::cerr << "\nAssgined SalaryMap printing...\n";
    s3.print();
    
}