//
//  MultiMap.cpp
//  Project4
//
//  Created by Gautam Gupta on 3/6/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include "MultiMap.h"
#include <string>
#include <cstdlib>

//---------------------------------------------------------
// MultiMap Functions
//---------------------------------------------------------
#pragma mark MultiMap
MultiMap::MultiMap()
: m_root(nullptr)
{}

MultiMap::~MultiMap()
{
    clear();
}

void MultiMap::clearRecursively(MultiMap::TreeNode* top)
{
    if (top == nullptr)
        return;
    if (top->left != nullptr)
        clearRecursively(top->left);
    if (top->right != nullptr)
        clearRecursively(top->right);
    delete top;
}

void MultiMap::clear()
{
    if (m_root != nullptr)
        clearRecursively(m_root);
    m_root = nullptr;
}

void MultiMap::insert(std::string key, unsigned int value)
{
    if (m_root == nullptr)
    {
        m_root = new TreeNode(nullptr, key, value);
        return;
    }
    
    else
    {
        TreeNode* cur = m_root;
        while (true)
        {
            if (cur->key == key)
            {
                cur->insertValue(value);
                return;
            }
            if (cur->left != nullptr && key < cur->key)
            {
                cur = cur->left;
                continue;
            }
            if (cur->right != nullptr && key > cur->key)
            {
                cur = cur->right;
                continue;
            }
            if (cur->right == nullptr && key > cur->key)
            {
                cur->right = new TreeNode(cur, key, value);
                return;
            }
            if (cur->left == nullptr && key < cur->key)
            {
                cur->left = new TreeNode(cur, key, value);
                return;
            }
            
        }
        
    }
}

MultiMap::Iterator MultiMap::findEqual(std::string key) const
{
    if (m_root == nullptr)
        return Iterator();
    TreeNode* cur = m_root;
    while (cur != nullptr)
    {
        if (key == cur->key)
            break;
        if (key > cur->key)
        {
            cur = cur->right;
            continue;
        }
        if (key < cur->key)
        {
            cur = cur->left;
            continue;
        }
    }
    if (cur == nullptr)
        return Iterator();
    Iterator it(cur);
    Iterator getUsToFirstValue = it;
    for (; getUsToFirstValue.valid() && getUsToFirstValue.getKey() == key; getUsToFirstValue.prev())
        it = getUsToFirstValue;
    return it;
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(std::string key) const
{
    Iterator t = findEqual(key);
    if (t.valid())
        return t;
    if (m_root == nullptr || (m_root->key < key && m_root->right == nullptr))
        return Iterator();
    TreeNode* cur = m_root;
    TreeNode* returner = nullptr;
    while (cur != nullptr)
    {
        if (key < cur->key)
        {
            returner = cur;
            if (cur->left != nullptr)
            {
                cur = cur->left;
                continue;
            }
            else break;
        }
        if (key > cur->key)
        {
            if (cur->right != nullptr)
            {
                cur = cur->right;
                continue;
            }
            else break;
        }
    }
    if (returner == nullptr)
        return Iterator();
    Iterator it(returner);
    return it;
   

}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(std::string key) const
{
    Iterator t = findEqual(key);
    if (t.valid())
    {
        Iterator it(t);
        std::string temp = it.getKey();
        Iterator getUsToLastValue = it;
        for (; getUsToLastValue.valid() && getUsToLastValue.getKey() == temp; getUsToLastValue.next())
            it = getUsToLastValue;
        return it;
    }
    if (m_root == nullptr || (m_root->key > key && m_root->left == nullptr))
        return Iterator();
    TreeNode* cur = m_root;
    TreeNode* returner = nullptr;
    while (cur != nullptr)
    {
        if (key > cur->key)
        {
            returner = cur;
            if (cur->right != nullptr)
            {
                cur = cur->right;
                continue;
            }
            else break;
            
        }
        if (key < cur->key)
        {
            if (cur->left != nullptr)
            {
                cur = cur->left;
                continue;
            }
            else break;
            
        }
        
    }
    if (returner == nullptr)
        return Iterator();
    Iterator it(returner);
    std::string temp = it.getKey();
    Iterator getUsToLastValue = it;
    for (; getUsToLastValue.valid() && getUsToLastValue.getKey() == temp; getUsToLastValue.next())
        it = getUsToLastValue;
    return it;
}

MultiMap::Iterator MultiMap::begin() const
{
    if (m_root == nullptr)
        return Iterator();
    Iterator it(m_root);
    Iterator p;
    for (; it.valid(); it.prev())
        p = it;
    return p;
}

MultiMap::Iterator MultiMap::end() const
{
    if (m_root == nullptr)
        return Iterator();
    Iterator it(m_root);
    Iterator p;
    for (; it.valid(); it.next())
        p = it;
    return p;
}

//---------------------------------------------------------
// Iterator Functions
//---------------------------------------------------------
#pragma mark -
#pragma mark Iterator
MultiMap::Iterator::Iterator()
: curTNode(nullptr), curVNode(nullptr) {}

MultiMap::Iterator::Iterator(TreeNode* _TN)
{
    curTNode = _TN;
    curVNode = _TN != nullptr ? _TN->vn_head : nullptr;
}


bool MultiMap::Iterator::valid() const
{
    return (curTNode != nullptr && curVNode != nullptr);
}

std::string MultiMap::Iterator::getKey() const
{
    if (valid())
        return curTNode->key;
    else
        abort();
}

unsigned int MultiMap::Iterator::getValue() const
{
    if (valid())
        return curVNode->vn_value;
    else
        abort();
}

bool MultiMap::Iterator::next()
{
    if (!valid()) return false;
    
    if (curVNode->vn_next != nullptr)
    {
        curVNode = curVNode->vn_next;
        return true;
    }
    
    else if (curTNode->right != nullptr)
    {
        curTNode = curTNode->right;
        while (curTNode->left != nullptr)
            curTNode = curTNode->left;
        curVNode = curTNode->vn_head;
        return true;
    }
    else if (curTNode->parent != nullptr)
    {
        std::string cur = curTNode->key;
        curTNode = curTNode->parent;
        while (curTNode != nullptr && curTNode->key < cur)
        {
            curTNode = curTNode->parent;
        }
        if (curTNode == nullptr)
            return false;
        curVNode = curTNode->vn_head;
        return true;
    }
    else
    {
        curTNode = nullptr;
        return false;
    }
}

bool MultiMap::Iterator::prev()
{
    if (!valid()) return false;
    
    if (curVNode->vn_prev != nullptr)
    {
        curVNode = curVNode->vn_prev;
        return true;
    }
    
    else if (curTNode->left != nullptr)
    {
        curTNode = curTNode->left;
        while (curTNode->right != nullptr)
        {
            curTNode = curTNode->right;
        }
        curVNode = curTNode->vn_head;
        while (curVNode->vn_next != nullptr)
            curVNode = curVNode->vn_next;
        return true;
    }
    
    else if (curTNode->parent != nullptr)
    {
        std::string cur = curTNode->key;
        curTNode = curTNode->parent;
        while (curTNode != nullptr && curTNode->key > cur)
            curTNode = curTNode->parent;
        if (curTNode == nullptr)
            return false;
        curVNode = curTNode->vn_head;
        while (curVNode->vn_next != nullptr)
            curVNode = curVNode->vn_next;
        return true;
    }
    else
    {
        curTNode = nullptr;
        return false;
    }
}

//---------------------------------------------------------
// Node Functions
//---------------------------------------------------------
#pragma mark -
#pragma mark Nodes

MultiMap::TreeNode::~TreeNode()
{
    if (vn_head == nullptr)
        return;
    ValueNode* cur = vn_head;
    ValueNode* next;
    do {
        next = cur->vn_next;
        delete cur;
        cur = next;
    } while (cur != nullptr);

}

void MultiMap::TreeNode::insertValue(unsigned int val)
{
    if (vn_head == nullptr)
        vn_head = new ValueNode(val);
    else
    {
        ValueNode* cur = vn_head;
        vn_head = new ValueNode(val);
        cur->vn_prev = vn_head;
        vn_head->vn_next = cur;
    }
}
