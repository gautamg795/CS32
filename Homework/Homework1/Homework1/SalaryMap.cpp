//
//  SalaryMap.cpp
//  Homework1
//
//  Created by Gautam Gupta on 1/16/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include "SalaryMap.h"
#include <iostream>

SalaryMap::SalaryMap()
{
    
}

bool SalaryMap::add(KeyType name, ValueType salary)
{
    if(salary >= 0)
        if(m_salaries.insert(name, salary))
            return true;
    return false;
}

bool SalaryMap::raise(KeyType name, ValueType pct)
{
    double current;
    if (!m_salaries.get(name, current) || pct < -100)
        return false;
    current = (current + pct*current/100);
    return m_salaries.update(name, current);
}

double SalaryMap::salary(KeyType name) const
{
    double returnSalary;
    return (m_salaries.get(name, returnSalary) ? returnSalary : -1);
}

int SalaryMap::size() const
{
    return m_salaries.size();
}

void SalaryMap::print() const
{
    for(int i = 0; i < size(); i++)
    {
        KeyType eName;
        ValueType eSalary;
        m_salaries.get(i, eName, eSalary);
        std::cout << eName << " " << eSalary << std::endl;
    }
}

