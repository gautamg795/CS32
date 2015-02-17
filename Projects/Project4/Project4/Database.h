//
//  Database.h
//  Project4
//
//  Created by Gautam Gupta on 3/6/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#ifndef __Project4__Database__
#define __Project4__Database__

#include <iostream>
#include <string>
#include <vector>
#include "MultiMap.h"
class Database
{
public:
    enum IndexType {it_none, it_indexed };
    enum OrderingType { ot_ascending, ot_descending };

    struct FieldDescriptor
    {
        std::string name;
        IndexType index;
    };
    
    struct SearchCriterion
    {
        std::string fieldName;
        std::string minValue;
        std::string maxValue;
    };
    
    struct SortCriterion
    {
        std::string fieldName;
        OrderingType ordering;
    };
    
    static const int ERROR_RESULT = -1;
    
    Database();
    ~Database();
    bool specifySchema(const std::vector<FieldDescriptor>& schema);
    bool addRow(const std::vector<std::string>& rowOfData);
    bool loadFromURL (std::string url);
    bool loadFromFile(std::string filename);
    int getNumRows() const;
    bool getRow(int rowNum, std::vector<std::string>& row) const;
    int search(const std::vector<SearchCriterion>& searchCriteria, const std::vector<SortCriterion>& sortCriteria, std::vector<int>& results);
private:
    Database(const Database& other);
    Database& operator=(const Database& rhs);
    void clearDatabase(); // Deletes all dynamically allocated MultiMaps
    bool loadFromStream(std::istream& ss); // Common function for the loading functions
    std::vector<std::vector<std::string> > m_rows;
    std::vector<MultiMap*> m_fieldIndex;
    std::vector<FieldDescriptor> m_schema;
    
    struct dataSorter // Used by Database::search() and std::sort as a predicate functor
    {
        dataSorter(const std::vector<SortCriterion>& sortCrit, Database* _db) : sortCriteria(sortCrit), m_db(_db) {}
        bool operator()(const int& firstRow, const int& secondRow) const;
        
        const std::vector<SortCriterion>& sortCriteria;
        Database* m_db;
    };
};

#endif /* defined(__Project4__Database__) */
