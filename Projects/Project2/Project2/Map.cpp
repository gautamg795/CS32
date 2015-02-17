//
//  Map.cpp
//  Project2
//
//  Created by Gautam Gupta on 1/22/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <cstdlib>
using namespace std;
Map::Node::Node()
{}

Map::Node::Node(KeyType key, ValueType value)
: m_key(key), m_value(value)
{}

Map::Map()
{
    m_head = new Node;
    m_size = 0;
    m_head->m_next = m_head; // Set up circular structure
    m_head->m_prev = m_head;
    
}

Map::~Map()
{
    Node* ptr = m_head;
    Node* next_ptr = ptr->m_next; // So we still know where the next node is
    do
    {
        ptr = next_ptr;
        next_ptr = ptr->m_next;
        delete ptr;
    }
    while (ptr != m_head); //Loop stops after all elements including dummy have been deleted
}

Map::Map(const Map& src)
{
    
    m_head = new Node;
    m_head->m_next = m_head; // Set up circular structure
    m_head->m_prev = m_head;
    m_size = 0; // No need to copy this from src as insert() will take care of the size
    for (int i = 0; i < src.m_size; i++)
    {
        KeyType key;
        ValueType value;
        src.get(i, key, value);
        insert(key, value);
    }
}

Map& Map::operator=(const Map& rhs)
{
    if (this != &rhs) // Aliasing check
    {
        Map temp(rhs);
        swap(temp); // Might as well use the swap function with the copy constructor
    }
    return *this;
}


bool Map::empty() const
{
    return (m_size == 0);
}

int Map::size() const
{
    return m_size;
}

Map::Node* Map::keyExists(KeyType key) const // Returns nullptr if 'key' is not found; otherwise
{                                            // returns a Node* pointer to matching node.
    if (m_size == 0)
        return nullptr;
    Node* ptr = m_head;
    do {
        ptr = ptr->m_next;
        if (ptr != m_head && ptr->m_key == key) //First condition so we don't access uninitialized key
            break;
    } while (ptr != m_head);
    if (ptr == m_head)
        return nullptr;
    return ptr;
}

bool Map::insert(const KeyType& key, const ValueType& value) // Inserts directly after head
{                                                            // (at the top of the list)
    if (keyExists(key) != nullptr)
        return false;
    Node* ptr = new Node(key, value);
    ptr->m_next = m_head->m_next;
    ptr->m_prev = m_head;
    m_head->m_next->m_prev = ptr;
    m_head->m_next = ptr;
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    Node* ptr = keyExists(key);
    if (ptr == nullptr)
        return false;
    ptr->m_value = value;
    return true;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (insert(key, value))
        return true;
    else // If insert returns false, then update
        return update(key, value);
}

bool Map::erase(const KeyType& key)
{
    Node* ptr = keyExists(key);
    if (m_size == 0 || ptr == nullptr)
        return false;
    ptr->m_prev->m_next = ptr->m_next;
    ptr->m_next->m_prev = ptr->m_prev;
    delete ptr;
    m_size--;
    return true;
}

bool Map::contains(const KeyType& key) const
{
    Node* ptr = keyExists(key);
    if (ptr == nullptr)
        return false;
    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* ptr = keyExists(key);
    if (ptr == nullptr)
        return false;
    value = ptr->m_value;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i >= m_size || i < 0)
        return false;
    Node* ptr = m_head->m_next; // It points at m_next so index 0 is already pointing at
    int index = 0;              // the first node of interest
    for(; index < i; index++)
        ptr = ptr->m_next;
    key = ptr->m_key;
    value = ptr->m_value;
    return true;
}

void Map::swap(Map& other) // Swaps the size and then points m_head at the other dummy node
{
    int tempSize = m_size;
    Node* tempHead = m_head;
    
    m_size = other.m_size;
    m_head = other.m_head;
    
    other.m_size = tempSize;
    other.m_head = tempHead;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    if (&m1 == &m2) // Save some time if they are the same
    {
        result = m1;
        return true;
    }
    
    Map tempMap; // Protect against aliasing by making changes to a temp variable
    bool returnValue = true;
    
    for (int i = 0; i < m1.size(); i++) // Step through first Map and insert nodes into the tempMap
    {                                   // if they match the parameters
        KeyType key;
        ValueType value;
        ValueType temp;
        m1.get(i, key, value); // Store key/value from m1 into the variables
        if (!m2.contains(key))
            tempMap.insert(key, value); // If m2 doesn't contain this key, put it in tempMap
        else if (m2.get(key, temp) && value == temp)
            tempMap.insert(key, value); // If m2 does contain it, insert only if values are
        else                            // identical between the two maps
            returnValue = false;
    }
    
    for (int i = 0; i < m2.size(); i++) // Now step through m2 to find values exclusive to it
    {
        KeyType key;
        ValueType value;
        m2.get(i, key, value);
        if (!m1.contains(key)) // If the key is exclusive to m2, insert it to tempMap
            tempMap.insert(key, value);
    }
    
    result = tempMap; // 'result' only changes at the end of the function (aliasing protection)
    return returnValue;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
    if (&m1 == &m2) // Save some time if they are the same
    {
        Map empty;
        result = empty;
        return;
    }
    Map tempMap; // Protect against aliasing by making changes to a temp variable
    tempMap = m1; // Fill tempMap with all of m1's nodes
    for (int pos = 0; pos < m2.size(); pos++)
    {
        KeyType curKey;
        ValueType curVal;
        m2.get(pos, curKey, curVal);
        if (tempMap.contains(curKey)) // If the key obtained from m2 matches something in tempMap,
            tempMap.erase(curKey);    // remove it from the tempMap (subtraction)
    }
    result = tempMap; // 'result' only changes at the end of the function (aliasing protection)
    return;
}