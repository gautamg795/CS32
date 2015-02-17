// Map.h

#ifndef MAP_INCLUDED
#define MAP_INCLUDED

template <typename KeyType, typename ValueType>
class Map
{
  public:
    Map();               // Create an empty map.

    bool empty() const;  // Return true if the map is empty, otherwise false.

    int size() const;    // Return the number of key/value pairs in the map.

    bool insert(const KeyType& key, const ValueType& value);
      // If key is not equal to any key currently in the map, and if the
      // key/value pair can be added to the map, then do so and return true.
      // Otherwise, make no change to the map and return false (indicating
      // that either the key is already in the map, or the map has a fixed
      // capacity and is full.

    bool update(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // Otherwise, make no change to the map and return false.

    bool insertOrUpdate(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // If key is not equal to any key currently in the map, and if the
      // key/value pair can be added to the map, then do so and return true.
      // Otherwise, make no change to the map and return false (indicating
      // that the key is not already in the map and the map has a fixed
      // capacity and is full.

    bool erase(const KeyType& key);
      // If key is equal to a key currently in the map, remove the key/value
      // pair with that key from the map and return true.  Otherwise, make
      // no change to the map and return false.
     
    bool contains(const KeyType& key) const;
      // Return true if key is equal to a key currently in the map, otherwise
      // false.
     
    bool get(const KeyType& key, ValueType& value) const;
      // If key is equal to a key currently in the map, set value to the
      // value in the map that that key maps to and return true.  Otherwise,
      // make no change to the value parameter of this function and return
      // false.
     
    bool get(int i, KeyType& key, ValueType& value) const;
      // If 0 <= i < size(), copy into the key and value parameters the
      // key and value of one of the key/value pairs in the map and return
      // true.  Otherwise, leave the key and value parameters unchanged and
      // return false.

    void swap(Map<KeyType, ValueType>& other);
      // Exchange the contents of this map with the other one.

      // Housekeeping functions
    ~Map();
    Map(const Map<KeyType, ValueType>& other);
    Map& operator=(const Map<KeyType, ValueType>& rhs);

  private:
      // Representation:
      //   a circular doubly-linked list with a dummy node.
      //   m_head points to the dummy node.
      //   m_head->m_prev->m_next == m_head and m_head->m_next->m_prev == m_head
      //   m_size == 0  iff  m_head->m_next == m_head->m_prev == m_head
      //   If m_size > 0
      //       m_head->next points to the node at position 0.
      //       m_head->prev points to the node at position m_size-1.
      //   Nodes are in no particular order.

    struct Node
    {
        KeyType   m_key;
        ValueType m_value;
        Node*     m_next;
        Node*     m_prev;
    };

    Node* m_head;
    int   m_size;

    Node* find(const KeyType& key) const;
      // Return pointer to Node whose m_key == key if there is one, else m_head

    bool doInsertOrUpdate(const KeyType& key, const ValueType& value,
                          bool mayInsert, bool mayUpdate);
      // If the key is not present in the map and if mayInsert is true, insert
      // the pair if there is room.  If the key is present and mayUpdate is
      // true, update the pair with the given key.
};

// Declarations of non-member functions

template <typename KeyType, typename ValueType>
bool combine(const Map<KeyType, ValueType>& m1, const Map<KeyType, ValueType>& m2, Map<KeyType, ValueType>& result);
      // If a key/value pair occurs in m1 or m2 or both, then it will occur in
      // result upon return from this function.  Return true unless m1 and m2
      // have a pair with the same key but different values; neither such pair
      // will occur in result upon return.

template <typename KeyType, typename ValueType>
void subtract(const Map<KeyType, ValueType>& m1, const Map<KeyType, ValueType>& m2, Map<KeyType, ValueType>& result);
      // Upon return, result contains those pairs in m1 whose keys don't
      // appear in m2.

// Inline implementations

template <typename KeyType, typename ValueType>
inline
int Map<KeyType, ValueType>::size() const
{
    return m_size;
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::empty() const
{
    return size() == 0;
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::contains(const KeyType& key) const
{
    return find(key) != m_head;
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value)
{
    return doInsertOrUpdate(key, value, true /* insert */, false /* no update */);
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::update(const KeyType& key, const ValueType& value)
{
    return doInsertOrUpdate(key, value, false /* no insert */, true /* update */);
}

template <typename KeyType, typename ValueType>
inline
bool Map<KeyType, ValueType>::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    return doInsertOrUpdate(key, value, true /* insert */, true /* update */);
}

// Non-inline templated member implementations

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map()
: m_size(0)
{
    // create dummy node
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::~Map()
{
    // Delete the m_size non-dummy nodes plus the dummy node
    for (Node* p = m_head->m_prev ; m_size >= 0; m_size--)
    {
        Node* toBeDeleted = p;
        p = p->m_prev;
        delete toBeDeleted;
    }
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map(const Map<KeyType, ValueType>& other)
: m_size(other.m_size)
{
    // Create dummy node; don't initialize its pointers
    
    m_head = new Node;
    
    // Initialize prev to last node created
    
    Node* prev = m_head;
    
    // Copy each non-dummy node from the other list; each iteration will set
    // the m_next of the previous node copied
    
    for (Node* p = other.m_head->m_next ; p != other.m_head; p = p->m_next)
    {
        // Create a copy of the node p points to
        Node* pnew = new Node;
        pnew->m_key = p->m_key;
        pnew->m_value = p->m_value;
        
        // Connect the new node to the previous one
        pnew->m_prev = prev;
        prev->m_next = pnew;
        
        // Reset prev to last node created
        prev = pnew;
    }
    
    // Connect last node created to m_head
    m_head->m_prev = prev;
    prev->m_next = m_head;

}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::operator=(const Map<KeyType, ValueType>& rhs)
{
    if (this != &rhs)
    {
        Map temp(rhs);
        swap(temp);
    }
    return *this;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::erase(const KeyType& key)
{
    Node* p = find(key);
    
    if (p == m_head)  // not found
        return false;
    
    // unlink the node from the list and destroy it
    
    p->m_prev->m_next = p->m_next;
    p->m_next->m_prev = p->m_prev;
    delete p;
    
    m_size--;
    return true;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::get(const KeyType& key, ValueType& value) const
{
    Node* p = find(key);
    if (p == m_head)  // not found
        return false;
    value = p->m_value;
    return true;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0  ||  i >= m_size)
        return false;
    
    // Return the key and value at position i.  This is one way of ensuring
    // the required behavior of get:  If the Map doesn't change in the
    // interim,
    // * calling get with each i in 0 <= i < size() gets each of the
    //   Map pairs, and
    // * calling get with the same value of i each time gets the same pair.
    
    // If i is closer to the head of the list, go forward to reach that
    // position; otherwise, start from tail and go backward.
    
    Node* p;
    if (i < m_size / 2)  // closer to head
    {
        p = m_head->m_next;
        for (int k = 0; k != i; k++)
            p = p->m_next;
    }
    else  // closer to tail
    {
        p = m_head->m_prev;
        for (int k = m_size-1; k != i; k--)
            p = p->m_prev;
    }
    
    key = p->m_key;
    value = p->m_value;
    return true;
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::swap(Map<KeyType, ValueType>& other)
{
    // swap head pointers
    Node* tempHead = m_head;
    m_head = other.m_head;
    other.m_head = tempHead;
    
    // swap sizes
    int t = m_size;
    m_size = other.m_size;
    other.m_size = t;
}

template <typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::Node* Map<KeyType, ValueType>::find(const KeyType& key) const
{
    // Do a linear search through the list
    
    Node* p;
    for (p = m_head->m_next; p != m_head && p->m_key != key; p = p->m_next)
        ;
    return p;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::doInsertOrUpdate(const KeyType& key, const ValueType& value, bool mayInsert, bool mayUpdate)
{
    Node* p = find(key);
    
    if (p != m_head)  // found
    {
        if (mayUpdate)
            p->m_value = value;
        return mayUpdate;
    }
    if (!mayInsert)  // not found, and not allowed to insert
        return false;
    
    // Create a new node
    p = new Node;
    p->m_key = key;
    p->m_value = value;
    
    // Insert new item at tail of list (arbitrary choice of position)
    //     Connect it to tail
    p->m_prev = m_head->m_prev;
    p->m_prev->m_next = p;
    
    //     Connect it to dummy node
    p->m_next = m_head;
    m_head->m_prev = p;
    
    m_size++;
    return true;
}


// Non-inlined nonmember functions
template <typename KeyType, typename ValueType>
bool combine(const Map<KeyType, ValueType>& m1, const Map<KeyType, ValueType>& m2, Map<KeyType, ValueType>& result)
{
    // For better performance, the bigger map should be the basis for
    // the result, and we should iterate over the elements of the
    // smaller one, adjusting the result as required.
    
    const Map<KeyType, ValueType>* bigger;
    const Map<KeyType, ValueType>* smaller;
    if (m1.size() >= m2.size())
    {
        bigger = &m1;
        smaller = &m2;
    }
    else
    {
        bigger = &m2;
        smaller = &m1;
    }
    
    // Guard against the case that result is an alias for m1 or m2
    // (i.e., that result is a reference to the same map that m1 or m2
    // refers to) by building the answer in a local variable res.  When
    // done, swap res with result; the old value of result (now in res) will
    // be destroyed when res is destroyed.
    
    bool status = true;
    Map<KeyType, ValueType> res(*bigger);               // res starts as a copy of the bigger map
    for (int n = 0; n < smaller->size(); n++)  // for each pair in smaller
    {
        KeyType k;
        ValueType vsmall;
        smaller->get(n, k, vsmall);
        ValueType vbig;
        if (!res.get(k, vbig))      // key in smaller doesn't appear in bigger
            res.insert(k, vsmall);  //     so add it to res
        else if (vbig != vsmall)    // same key, different value
        {                           //     so pair shouldn't be in res
            res.erase(k);
            status = false;
        }
    }
    result.swap(res);
    return status;
}

template <typename KeyType, typename ValueType>
void subtract(const Map<KeyType, ValueType>& m1, const Map<KeyType, ValueType>& m2,  Map<KeyType, ValueType>& result)
{
    // Guard against the case that result is an alias for m1 or m2
    // (i.e., that result is a reference to the same map that m1 or m2
    // refers to) by building the answer in a local variable res.  When
    // done, swap res with result; the old value of result (now in res) will
    // be destroyed when res is destroyed.
    
    if (m1.size() <= m2.size())
    {
        // If m1 is smaller, if an item in m1 should be in the result because
        // its key is not in m2, add it
        
        Map<KeyType, ValueType> res;
        for (int n = 0; n < m1.size(); n++)
        {
            KeyType k;
            ValueType v;
            m1.get(n, k, v);
            if (!m2.contains(k))
                res.insert(k, v);
        }
        result.swap(res);
    }
    else
    {
        // If m1 is larger, copy it to the result and remove from result
        // keys that are in m2
        
        Map<KeyType, ValueType> res(m1);
        for (int n = 0; n < m2.size(); n++)
        {
            KeyType k;
            ValueType v;
            m2.get(n, k, v);
            res.erase(k);
        }
        result.swap(res);
    }

}
#endif // MAP_INCLUDED
