//
//  main.cpp
//  Project2
//
//  Created by Gautam Gupta on 1/22/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;
void ThisFunctionWillNeverBeCalled();
void test()
{
    Map m;
    assert(m.insert("Fred", 123));
    assert(m.insert("Ethel", 456));
    assert(m.size() == 2);
    double d = 42;
    assert(m.get("Fred", d) && d == 123);
    d = 42;
    string s1;
    assert(m.get(0, s1, d) && ((s1 == "Fred" && d == 123) || (s1 == "Ethel" && d == 456)));
    string s2;
    assert(m.get(1, s2, d) && s1 != s2 && ((s2 == "Fred" && d == 123) || (s2 == "Ethel" && d == 456)));
}
bool empty() { return true; }
int main()
{

    Map m1;
    assert(m1.size() == 0);
    assert(m1.empty());
    assert(!m1.contains("testKey"));
    assert(m1.insert("testKey", 0));
    KeyType k1;
    ValueType v1;
    assert(!m1.insert("testKey", 0));
    assert(m1.get("testKey", v1));
    assert(v1 == 0);
    assert(m1.size() == 1);
    assert(m1.update("testKey", 10));
    assert(m1.get("testKey", v1));
    assert(v1 == 10);
    Map m8(m1);
    assert(!m8.insert("testKey", 0));
    assert(m8.get("testKey", v1));
    assert(v1 == 10);
    v1 = 0;
    assert(m1.get(0, k1, v1));
    assert(k1 == "testKey" && v1 == 10);
    assert(!m1.get(1, k1, v1));
    assert(m1.insertOrUpdate("Key2", 1));
    assert(m1.size() == 2);
    assert(m1.contains("testKey") && m1.contains("Key2"));
    assert(m1.insertOrUpdate("Key2", 100));
    assert(m1.get("Key2", v1));
    assert(v1 == 100);
    KeyType k2;
    ValueType v2;
    assert(m1.get(0, k1, v1) && m1.get(1, k2, v2));
    assert(k1 != k2 && v1 != v2);
    assert(!m1.erase("NotExist"));
    assert(m1.erase("testKey"));
    assert(m1.size() == 1);
    assert(!m1.contains("testKey"));
    assert(m1.get(0, k1, v2));
    assert(k1 == "Key2" && v2 == 100);
    assert(!m1.get(1, k1, v1));
    assert(m1.insert("Key3", 3) && m1.insert("Key4", 4) && m1.insert("Key5", 5));
    assert(m1.size() == 4);
    Map m2;
    assert(m2.empty());
    m1.swap(m2);
    assert(m1.empty());
    assert(m2.size() == 4);
    assert(m2.contains("Key3"));
    const Map emptyMap;
    m1 = emptyMap;
    m2 = emptyMap;
    assert(m1.empty() && m2.empty());
    m1.insert("Fred", 123);
    m1.insert("Ethel", 456);
    m1.insert("Lucy", 789);
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);
    Map result;
    assert(combine(m1, m2, result));
    assert(result.size() == 4);
    assert(result.contains("Fred") && result.contains("Ricky") && result.contains("Lucy") && result.contains("Ethel"));
    Map m4(m1);
    assert(combine(m4, m2, result));
    assert(result.size() == 4);
    assert(result.contains("Fred") && result.contains("Ricky") && result.contains("Lucy") && result.contains("Ethel"));
    m2 = emptyMap;
    m2.insert("Lucy", 654);
    m2.insert("Ricky", 321);
    assert(!combine(m1, m2, result));
    assert(result.size() == 3);
    assert(result.contains("Fred") && result.contains("Ricky") && result.contains("Ethel") && !result.contains("Lucy"));
    result = emptyMap;
    Map& result2 = m1;
    m2 = emptyMap;
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);
    m2.insert("Ethel", 654);
    subtract(m1, m2, result2);
    assert(result2.size() == 1);
    assert(result2.contains("Fred"));
    Map original;
    original.insert("one", 1);
    original.insert("two", 2);
    original.insert("three", 3);
    original.swap(original);
    assert(original.size() == 3);
    m1 = emptyMap;
    m2 = emptyMap;
    m2.insert("name1", 0);
    m1.insert("name1", 1);
    m1.insert("name2", 2);
    combine(m1, m2, m1);
    assert(m1.size() == 1);
    assert(!m1.contains("name1"));
    test();
    ThisFunctionWillNeverBeCalled();
    std::cerr << "All tests succeeded!" << endl;
    std::cout << "All tests succeded" << endl;
}

#define CHECKTYPE(f, t)  \
    {                    \
        auto p = (t)(f); \
        (void) p;        \
    }

static_assert(std::is_default_constructible<Map>::value,
              "Map must be default-constructible.");
static_assert(std::is_copy_constructible<Map>::value,
              "Map must be copy-constructible.");

void ThisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Map::operator=, Map& (Map::*)(const Map&));
    CHECKTYPE(&Map::empty, bool (Map::*)() const);
    CHECKTYPE(&Map::size, int (Map::*)() const);
    CHECKTYPE(&Map::insert, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::update, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::insertOrUpdate, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::erase, bool (Map::*)(const KeyType&));
    CHECKTYPE(&Map::contains, bool (Map::*)(const KeyType&) const);
    CHECKTYPE(&Map::get, bool (Map::*)(const KeyType&, ValueType&) const);
    CHECKTYPE(&Map::get, bool (Map::*)(int, KeyType&, ValueType&) const);
    CHECKTYPE(&Map::swap, void (Map::*)(Map&));
    CHECKTYPE(combine, bool (*)(const Map&, const Map&, Map&));
    CHECKTYPE(subtract, void (*)(const Map&, const Map&, Map&));
}