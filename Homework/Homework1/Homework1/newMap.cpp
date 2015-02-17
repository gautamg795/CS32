//
//  newMap.cpp
//  Homework1
//
//  Created by Gautam Gupta on 1/16/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include "newMap.h"
// #include <algorithm>
Map::Map(int num)
{
    m_size = 0;
    m_capacity = num;
    m_mapData = new kvPair[num];
}

Map::~Map()
{
    delete [] m_mapData;
}

Map::Map(const Map& src)
{
    m_size = src.m_size;
    m_capacity = src.m_capacity;
    m_mapData = new kvPair[m_capacity];
    for (int i = 0; i < m_size; i++)
    {
        m_mapData[i] = src.m_mapData[i];
    }
}

Map& Map::operator=(const Map& src)
{
    if (this != &src)
    {
        delete [] m_mapData;
        m_capacity = src.m_capacity;
        m_size = src.m_size;
        m_mapData = new kvPair[m_capacity];
        for (int i = 0; i < m_size; i++)
            m_mapData[i] = src.m_mapData[i];
    }
    return *this;
}

int Map::keyExists(const KeyType& key) const
{
    
    for (int i = 0; i < m_size; i++)
        if (key == m_mapData[i].m_key)
            return i;
    return -1;
}

bool Map::empty() const
{
    return (m_size == 0);
}

int Map::size() const
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (m_size >= m_capacity)
        return false;
    int i = keyExists(key);
    if (i != -1)
        return false;
    m_mapData[m_size].m_key = key;
    m_mapData[m_size].m_value = value;
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    int i = keyExists(key);
    if (i == -1)
        return false;
    m_mapData[i].m_key = key;
    m_mapData[i].m_value = value;
    return true;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    int i = keyExists(key);
    if (i == -1)
    {
        if(insert(key, value))
            return true;
        else
            return false;
    }
    else
        update(key, value);
    return true;
}

bool Map::erase(const KeyType& key)
{
    int i = keyExists(key);
    if (i > -1)
    {
        if(i == m_size - 1)
        {
            m_size--;
            return true;
        }
        for (; i < m_size-1; i++)
        {
            m_mapData[i] = m_mapData[i+1];
        }
        m_size--;
        return true;
    }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    return (keyExists(key) > -1);
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    int i = keyExists(key);
    if (i == -1)
        return false;
    value = m_mapData[i].m_value;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0 || i >= size())
        return false;
    key = m_mapData[i].m_key;
    value = m_mapData[i].m_value;
    return true;
}

void Map::swap(Map& other)
{
//    std::swap(*this, other); // This works too...but is lame
    
    int sizeTemp = m_size;
    int capacityTemp = m_capacity;
    kvPair* ptrTemp = m_mapData;
    
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_mapData = other.m_mapData;
    
    other.m_size = sizeTemp;
    other.m_capacity = capacityTemp;
    other.m_mapData = ptrTemp;
}



