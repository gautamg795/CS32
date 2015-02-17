//
//  Database.cpp
//  Project4
//
//  Created by Gautam Gupta on 3/6/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include "Database.h"
#include <string>
#include <vector>
#include "MultiMap.h"
#include "http.h"
#include "Tokenizer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <utility>

using namespace std;

Database::Database() // Clears all data members
{
    m_rows.clear();
    m_fieldIndex.clear();
    m_schema.clear();
}

Database::~Database()
{
    clearDatabase();
}
void Database::clearDatabase() // Deletes all dynamically allocated MultiMaps and clears all data members
{
    for (auto it = m_fieldIndex.begin(); it != m_fieldIndex.end(); it++)
        delete *it;
    m_fieldIndex.clear();
    m_rows.clear();
    m_schema.clear();
}

bool Database::specifySchema(const std::vector<FieldDescriptor>& schema)
{
    clearDatabase();
    bool atLeastOneIndex = false; // Ensure at least one element is exists
    for (size_t i = 0; i < schema.size(); i++)
    {
        m_schema.push_back(schema[i]);
        m_fieldIndex.push_back(nullptr);
        if (schema[i].index == it_indexed)
        {
            atLeastOneIndex = true;
            m_fieldIndex[i] = new MultiMap();
        }

    }
    if (atLeastOneIndex)
        return true;
    else
    {
        clearDatabase();
        return false;
    }
}

bool Database::addRow(const std::vector<std::string>& rowOfData)
{
    if (rowOfData.size() != m_schema.size())
        return false;
    m_rows.push_back(rowOfData);
    int index = static_cast<int>(m_rows.size())-1;
    for (size_t i = 0; i < m_schema.size(); i++)
    {
        if (m_schema[i].index == it_indexed)
        {
            m_fieldIndex[i]->insert(rowOfData[i], index); // m_fieldIndex[i] should not be nullptr because of specifySchema
        }
    }
    return true;
}

bool Database::loadFromStream(std::istream& ss) // Common function for loadFromFile and loadFromURL
{
    std::vector<FieldDescriptor> schema;
    string topLine;
    if (!getline(ss, topLine)) //Get the top line (schema)
        return false;
    Tokenizer s(topLine, ",");
    string curSchemaField;
    while (s.getNextToken(curSchemaField))
    {
        if (curSchemaField.at(curSchemaField.size()-1) == '*')
        {
            curSchemaField.pop_back(); // Remove the trailing asterisk
            FieldDescriptor temp_i;
            temp_i.name = curSchemaField;
            temp_i.index = it_indexed;
            schema.push_back(temp_i);
        }
        else
        {
            FieldDescriptor temp;
            temp.name = curSchemaField;
            temp.index = it_none;
            schema.push_back(temp);
        }
    }
    if (!specifySchema(schema))
        return false;
    
    string curRow;
    string curField;
    
    while (getline(ss, curRow)) // Continue processing the data fields
    {
        Tokenizer getRow(curRow, ",");
        vector<string> rowOfData;
        while (getRow.getNextToken(curField))
        {
            rowOfData.push_back(curField);
        }
        if (!addRow(rowOfData))
            return false;
    }
    
    return true;


}

bool Database::loadFromURL(std::string url)
{
    string page;
    if (!HTTP().get(url, page))
        return false;
    stringstream ss;
    ss << page;
    return loadFromStream(ss);
}

bool Database::loadFromFile(std::string filename)
{
    ifstream infile(filename.c_str());
    if (!infile)
        return false;
    return loadFromStream(infile);
}

int Database::getNumRows() const
{
    return static_cast<int>(m_rows.size());
}

bool Database::getRow(int rowNum, std::vector<std::string>& row) const
{
    if (rowNum < 0 || rowNum >= getNumRows())
        return false;
    row = m_rows[rowNum];
    return true;
}

int Database::search(const std::vector<SearchCriterion>& searchCriteria, const std::vector<SortCriterion>& sortCriteria, std::vector<int>& results)
{
    results.clear();
    
    if (searchCriteria.size() < 1)
        return ERROR_RESULT;

    vector<set<unsigned int> > preIntersection;
    for (size_t i = 0; i < searchCriteria.size(); i++)
    {
        SearchCriterion s = searchCriteria[i];
        if (s.maxValue == "" && s.minValue == "")
            return ERROR_RESULT;
        size_t j = 0;
        while (j < m_schema.size())
        {
            if (m_schema[j].name == s.fieldName)
            {
                if (m_fieldIndex[j] == nullptr)
                    return ERROR_RESULT;
                break;
            }
            if (j == m_schema.size()-1)
            {
                return ERROR_RESULT;
            }
            j++;
        }
        set<unsigned int> curSet;
        if (s.maxValue != "") // If min value is "", anything is a successor so no issue
        {
            MultiMap::Iterator it_begin = m_fieldIndex[j]->findEqualOrSuccessor(s.minValue);
            MultiMap::Iterator it_end = m_fieldIndex[j]->findEqualOrPredecessor(s.maxValue);
            pair<string, unsigned int> begin = it_begin.valid() ? make_pair(it_begin.getKey(), it_begin.getValue()) : make_pair("", 0);
            pair<string, unsigned int> end = it_end.valid() ? make_pair(it_end.getKey(), it_end.getValue()) : make_pair("", 0);
            while (begin != end && it_begin.valid())
            {
                curSet.insert(it_begin.getValue());
                if (!it_begin.next())
                    break;
                begin = make_pair(it_begin.getKey(), it_begin.getValue());
            }
            if (it_end.valid())
                curSet.insert(it_end.getValue());
        }
        else // no max value
        {
            MultiMap::Iterator it_begin = m_fieldIndex[j]->findEqualOrSuccessor(s.minValue);
            while (it_begin.valid())
            {
                curSet.insert(it_begin.getValue());
                it_begin.next();
            }
        }
        preIntersection.push_back(curSet);
    }
    set<unsigned int> tempFinals;
    
    for (size_t i = 0; i < preIntersection.size()-1; i++)
    {
        set_intersection(preIntersection[i].begin(), preIntersection[i].end(), preIntersection[i+1].begin(), preIntersection[i+1].end(), inserter(tempFinals, tempFinals.begin()));
        preIntersection[i+1] = tempFinals;
        tempFinals.clear();
    }
    set<unsigned int> finalValues(preIntersection[preIntersection.size()-1]);
    results.clear();
    results.reserve(finalValues.size());
    for (auto it = finalValues.begin(); it != finalValues.end(); it++)
    {
        results.push_back(*it);
    }
    if (sortCriteria.size() > 0)
//        std::sort(results.begin(), results.end(), dataSorter(sortCriteria, this));
        std::sort(results.begin(), results.end(), [&](const int& firstRow, const int& secondRow){
            vector<string> first = this->m_rows[firstRow];
            vector<string> second = this->m_rows[secondRow];
            for (auto it = sortCriteria.begin(); it != sortCriteria.end(); it++)
            {
                SortCriterion current = *it;
                size_t j = 0;
                while (j < this->m_schema.size())
                {
                    if (this->m_schema[j].name == current.fieldName)
                        break;
                    j++;
                }
                if (first[j] == second[j])
                    continue;
                if (current.ordering == ot_descending)
                    return (first[j] > second[j]);
                else return (first[j] < second[j]);
            }
            
            return false;

        });
    return static_cast<int>(results.size());
}

bool Database::dataSorter::operator() (const int& firstRow, const int& secondRow) const // Predicate function used by std::sort
{
    vector<string> first = m_db->m_rows[firstRow];
    vector<string> second = m_db->m_rows[secondRow];
    for (auto it = sortCriteria.begin(); it != sortCriteria.end(); it++)
    {
        SortCriterion current = *it;
        size_t j = 0;
        while (j < m_db->m_schema.size())
        {
            if (m_db->m_schema[j].name == current.fieldName)
                break;
            j++;
        }
        if (first[j] == second[j])
            continue;
        if (current.ordering == ot_descending)
            return (first[j] > second[j]);
        else return (first[j] < second[j]);
    }
    
    return false;
}
