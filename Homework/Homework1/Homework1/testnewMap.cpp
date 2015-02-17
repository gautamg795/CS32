//
//  testMap.cpp
//  Homework1
//
//  Created by Gautam Gupta on 1/16/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include "newMap.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int main()
{
    Map m;  // maps strings to doubles
    assert(m.empty());
    ValueType v = -1234.5;
    assert( !m.get("abc", v)  &&  v == -1234.5); // v unchanged by get failure
    m.insert("xyz", 9876.5);
    assert(m.size() == 1);
    KeyType k = "hello";
    assert(m.get(0, k, v)  &&  k == "xyz"  &&  v == 9876.5);
    Map gpas;
    gpas.insert("Fred", 2.956);
    gpas.insert("Ethel", 3.538);
    double v1;
    string k1;
    assert(gpas.get(1,k1,v1)  &&  (k1 == "Fred"  ||  k1 == "Ethel"));
    string k2;
    assert(gpas.get(1,k2,v1)  &&  k2 == k1);
    Map m1;
    m1.insert("Fred", 2.956);
    Map m2;
    m2.insert("Ethel", 3.538);
    m2.insert("Lucy", 2.956);
    m1.swap(m2);
    assert(m1.size() == 2  &&  m1.contains("Ethel")  &&  m1.contains("Lucy")  &&
           m2.size() == 1  &&  m2.contains("Fred"));
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k3[6] = { "a", "b", "c", "d", "e", "f" };
    ValueType v3  = 20;
    
    // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k3[n], v3));
    
    // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k3[5], v3));
    
    // When two Maps' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(k3[5], v3)  &&  b.insert(k3[5],v3));
    
    cout << "Passed all tests" << endl;
    Map m4;
    m4.insert("key", 4);
}