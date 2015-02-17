//
//  main.cpp
//  Project4
//
//  Created by Gautam Gupta on 3/5/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//
//
//#include <iostream>
//#include <cassert>
//#include "MultiMap.h"
//#include "http.h"
//#include "Tokenizer.h"
//#include "Database.h"
//using namespace std;

//int main()
//{
//    MultiMap myMultiMap;
//    myMultiMap.insert("Andrea", 6);
//    myMultiMap.insert("Bill", 2);
//    myMultiMap.insert("Carey", 5);
//    MultiMap::Iterator it = myMultiMap.findEqual("Andrea");
//    assert(it.valid());
//    assert(it.getKey() == "Andrea");
//    assert(it.getValue() == 6);
//    assert(it.next());
//    assert(it.getKey() == "Bill");
//    it.prev();
//    assert(!it.prev());
//    assert(!it.valid());
//    myMultiMap.clear();
//    myMultiMap.insert("Bill", 2);
//    myMultiMap.insert("Carey", 5);
//    myMultiMap.insert("Bill", 8);
//    myMultiMap.insert("Batia", 4);
//    myMultiMap.insert("Larry", 7);
//    myMultiMap.insert("Larry", 9);
//    myMultiMap.insert("Bill", 3);
//    myMultiMap.insert("Ab", 100);
//    
//    it = myMultiMap.findEqualOrSuccessor("A");
//    assert(it.valid());
//    assert(!it.prev());
//    assert(!it.valid());
//    
//}
//
//#include <sstream>
//int main()
//{
//    Database d;
//    d.loadFromFile("/Volumes/Data/Users/gautam/Downloads/census.csv");
//    string page;
//    stringstream ss;
//    HTTP().get("http://cs.ucla.edu/classes/winter14/cs32/Projects/4/Data/census.csv", page);
//    ss << page;
//    string line;
//    getline(ss, line);
//    cout << line;
// }

#include "test.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    string filename;
    switch (argc)
    {
        case 1:
            cout << "Enter test script file name: ";
            getline(cin, filename);
            break;
        case 2:
            filename = argv[1];
            break;
        default:
            cout << "Usage: " << argv[0] << " scriptname" << endl;
            return 1;
    }
    
    Test t;
    string problem;
    
    if (!t.loadAndRunTestScript(filename, problem))
    {
        cout << "Problem running script: " << problem << endl;
        return 1;
    }
    return 0;
}