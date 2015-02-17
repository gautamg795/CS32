//
//  Map.h
//  Project2
//
//  Created by Gautam Gupta on 1/22/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#ifndef __Project2__Map__
#define __Project2__Map__
#include <string>
typedef std::string KeyType;
typedef double ValueType;
class Map
{
public:
    Map();
    ~Map();
    Map(const Map& src);
    Map& operator=(const Map& rhs);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
private:
    struct Node // Circular, doubly linked list
    {
        Node();
        Node(KeyType key, ValueType value); // Just to make life a little easier
        KeyType m_key;
        ValueType m_value;
        Node* m_next;
        Node* m_prev;
    };
    Node* m_head;
    int m_size;
    Node* keyExists(KeyType key) const; // Returns nullptr if 'key' is not found; otherwise
                                        // returns a Node* pointer to matching node.
    
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif /* defined(__Project2__Map__) */


