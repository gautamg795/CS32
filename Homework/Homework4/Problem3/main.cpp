//
//  main.cpp
//  Problem3
//
//  Created by Gautam Gupta on 3/1/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include <iostream>

#include <string>
#include <vector>

using namespace std;

class Class
{
public:
    Class(string nm) : m_name(nm) {}
    string name() const { return m_name; }
    const vector<Class*>& subclasses() const { return m_subclasses; }
    void add(Class* d) { m_subclasses.push_back(d); }
    ~Class();
private:
    string m_name;
    vector<Class*> m_subclasses;
};

Class::~Class()
{
    for (size_t k = 0; k < m_subclasses.size(); k++)
        delete m_subclasses[k];
}

void listAll(const Class* c, string path)  // two-parameter overload
{
    cout << path + c->name() << endl;
    for (auto it = c->subclasses().cbegin(); it != c->subclasses().cend(); it++)
        listAll(*it, path + c->name() + "=>");
}

void listAll(const Class* c)  // one-parameter overload
{
    if (c != NULL)
        listAll(c, "");
}

int main()
{
    Class* d1 = new Class("ComplexZumi");
    listAll(d1);
    cout << "====" << endl;
    d1->add(new Class("GeniusZumi"));
    Class* d2 = new Class("Zumi");
    d2->add(new Class("SimpleZumi"));
    d2->add(d1);
    Class* d3 = new Class("Goodie");
    d3->add(new Class("ExtraLifeGoodie"));
    d3->add(new Class("WalkThroughWallsGoodie"));
    d3->add(new Class("IncreaseSimultaneousSprayersGoodie"));
    listAll(d3);
    cout << "====" << endl;
    Class* d4 = new Class("Actor");
    d4->add(d2);
    d4->add(new Class("Player"));
    d4->add(d3);
    listAll(d4);
    delete d4;
}