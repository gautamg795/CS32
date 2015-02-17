//
//  MultiMap.h
//  Project4
//
//  Created by Gautam Gupta on 3/6/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#ifndef __Project4__MultiMap__
#define __Project4__MultiMap__

#include <iostream>
#include <string>


class MultiMap
{
    
private: // Declare these structs first so Iterator can access them
    struct ValueNode
    {
        ValueNode(unsigned int _data) { vn_value = _data; vn_next = vn_prev =  nullptr; }
        unsigned int vn_value;
        ValueNode* vn_next;
        ValueNode* vn_prev;
    private:
        ValueNode(ValueNode& src);
        ValueNode& operator=(ValueNode& rhs);
    };
    struct TreeNode
    {
        TreeNode(TreeNode* _parent, std::string _key, unsigned int _val) { parent = _parent; left = right = nullptr; vn_head = nullptr; key = _key; insertValue(_val); }
        ~TreeNode(); // Must delete all ValueNodes
        TreeNode* parent;
        TreeNode* left;
        TreeNode* right;
        std::string key;
        ValueNode* vn_head;
        void insertValue(unsigned int val);
    private:
        TreeNode(TreeNode& src);
        TreeNode& operator=(TreeNode& rhs);
    };

public:
    // You must implement this public nested MultiMap::Iterator class
    class Iterator
    {
    public:
        Iterator(); // O(1)
        Iterator(TreeNode* _TN); // O(1)
        bool valid() const; // O(1)
        std::string getKey() const; // O(1)
        unsigned int getValue() const; // O(1)
        bool next(); // O(log N) to O(N)
        bool prev(); // O(log N) to O(N)
    private:
        TreeNode* curTNode;
        ValueNode* curVNode;
    };
    
    MultiMap(); // O(1)
    ~MultiMap(); // O(N)
    void clear(); // O(N)
    void insert(std::string key, unsigned int value); // O(log N)
    Iterator findEqual(std::string key) const; // O(log N) to O(N)
    Iterator findEqualOrSuccessor(std::string key) const; // O(log N) to O(N)
    Iterator findEqualOrPredecessor(std::string key) const; // O(log N) to O(N)
    
private:
    MultiMap(const MultiMap& other);
    MultiMap& operator=(const MultiMap& rhs);
    void clearRecursively(TreeNode* top);
    TreeNode* m_root;
    Iterator begin() const;
    Iterator end() const;
    
};


#endif /* defined(__Project4__MultiMap__) */

// No more public functions
// No STL containers
// Private Node type
