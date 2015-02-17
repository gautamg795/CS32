#include <iostream>
#include <algorithm>
#include <numeric>  // for std::accumulate
#include <vector>
#include <string>
#include <cstdlib>  // for std::rand
#include <cassert>

using namespace std;

//========================================================================
//  Set this to false to skip the insertion sort tests; you'd do this if
//  you're sorting so many items that insertion_sort would take more time
//  than you're willing to wait.

const bool TEST_INSERTION_SORT = true;
//========================================================================

//========================================================================
// Time         - a type to hold a timer reading
// operator-    - milliseconds between two Time objects' creations
//========================================================================

#if __cplusplus == 201103L  // C++11

#include <chrono>

class Time
{
public:
    Time()
    : m_time(std::chrono::high_resolution_clock::now())
    {}
    double operator-(Time start) const
    {
        std::chrono::duration<double,std::milli> diff(m_time - start.m_time);
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

#elif defined(_MSC_VER)  // not C++11, but Windows

#include <windows.h>

class Time
{
public:
    Time()
    {
        QueryPerformanceCounter(&m_time);
    }
    double operator-(Time start) const
    {
        LARGE_INTEGER ticksPerSecond;
        QueryPerformanceFrequency(&ticksPerSecond);
        return (1000.0 * (m_time.QuadPart - start.m_time.QuadPart)) /
        ticksPerSecond.QuadPart;
    }
private:
    LARGE_INTEGER m_time;
};

#else // not C++11 or Windows, so C++98

#include <ctime>

class Time
{
public:
    Time()
    : m_time(std::clock())
    {}
    double operator-(Time start) const
    {
        return (1000.0 * (m_time - start.m_time)) / CLOCKS_PER_SEC;
    }
private:
    std::clock_t m_time;
};

#endif

//========================================================================

// Here's a class that is not cheap to copy -- the vector holds a pointer
// to dynamic memory, so vector's assignment operator and copy constructor
// have to allocate storage.

// We'll simplify writing our timing tests by declaring everything public
// in this class.  (We wouldn't make data public in a class intended for
// wider use.)

typedef int IdType;

struct Store
{
    IdType id;
    double total;
    double weeklySales[52];
    Store(IdType i) : id(i)
    {
        // create twelve random sales figures (from 20000 to 60000)
        for (size_t k = 0; k < 52; k++)
            weeklySales[k] = 20000 + rand() % 40000;
        // (accumulate computes 0.0 + weeklySales[0] + weeklySales[1] + ...)
        total = accumulate(weeklySales, weeklySales + 52, 0.0);
    }
};

inline
bool compareStore(const Store& lhs, const Store& rhs)
{
    // The Store with the higher total should come first.  If they have
    // the same total, then the Store with the smaller id number should
    // come first.  Return true iff lhs should come first.  Notice that
    // this means that a false return means EITHER that rhs should come
    // first, or there's a tie, so we don't care which comes first,
    
    if (lhs.total > rhs.total)
        return true;
    if (lhs.total < rhs.total)
        return false;
    return lhs.id < rhs.id;
}

inline
bool compareStorePtr(const Store* lhs, const Store* rhs)
{
    return compareStore(*lhs, *rhs);
}

bool isSorted(const vector<Store>& s)
{
    // Return true iff the vector is sorted according to the ordering
    // relationship compareStore
    
    for (size_t k = 1; k < s.size(); k++)
    {
        if (compareStore(s[k], s[k-1]))
            return false;
    }
    return true;
}

void insertion_sort(vector<Store>& s, bool comp(const Store&, const Store&))
{
    for (int i = 1; i < s.size(); i++)
    {
        Store sortMe = s[i];
        int currentLoc = i;
        while (currentLoc > 0 && comp(sortMe, s[currentLoc-1]))
        {
            swap(s[currentLoc], s[currentLoc-1]);
            currentLoc--;
            
        }
        s[currentLoc] = sortMe;
    }
}

// Report the results of a timing test

void report(string caption, double t, const vector<Store>& s)
{
    cout << t << " milliseconds; " << caption
    << "; first few stores are\n  ";
    size_t n = s.size();
    if (n > 5)
        n = 5;
    for (size_t k = 0; k < n; k++)
        cout << " (" << s[k].id << ", " << s[k].total << ")";
    cout << endl;
}

int main()
{
    size_t nstores;
    cout << "Enter number of stores to sort: ";
    cin >> nstores;
    if ( !cin  ||  nstores <= 0)
    {
        cout << "You must enter a positive number.  Goodbye." << endl;
        return 1;
    }
    
    // Create a random ordering of id numbers 0 through nstores-1
    vector<IdType> ids;
    for (size_t j = 0; j < nstores; j++)
        ids.push_back(IdType(j));
    random_shuffle(ids.begin(), ids.end());  // from <algorithm>
    
    // Create a bunch of Stores
    vector<Store> unorderedStores;
    for (size_t k = 0; k < ids.size(); k++)
        unorderedStores.push_back(Store(ids[k]));
    
    // Sort the Stores using the STL sort algorithm.  It uses a variant
    // of quicksort called introsort.
    
    vector<Store> stores(unorderedStores);
    Time start1;
    sort(stores.begin(), stores.end(), compareStore);
    Time end1;
    report("STL sort", end1-start1, stores);
    assert(isSorted(stores));
    
    // Sort the already sorted array using the STL sort.  This should be
    // fast.
    
    Time start2;
    sort(stores.begin(), stores.end(), compareStore);
    Time end2;
    report("STL sort if already sorted", end2-start2, stores);
    assert(isSorted(stores));
    
    if (TEST_INSERTION_SORT)
    {
        // Sort the original unsorted array using insertion sort.  This
        // should be really slow.  If you have to wait more than a minute,
        // try rerunning the program with a smaller number of Stores.
        
        stores = unorderedStores;
        Time start3;
        insertion_sort(stores, compareStore);
        Time end3;
        report("insertion sort if not already sorted", end3-start3, stores);
        assert(isSorted(stores));
        
        // Sort the already sorted array using insertion sort.  This should
        // be fast.
        
        Time start4;
        insertion_sort(stores, compareStore);
        Time end4;
        report("insertion sort if already sorted", end4-start4, stores);
        assert(isSorted(stores));
    }
    
    // Since Stores are expensive to copy, and since the STL's sort copies
    // Stores O(N log N) times, let's sort POINTERS to the Stores, then
    // make one final pass to rearrange the Stores according to the
    // reordered pointers.  We'll write some code; you write the rest.
    
    // Set stores to the original unsorted sequence
    stores = unorderedStores;
    
    // Start the timing
    Time start5;
    
    // Create a auxiliary copy of stores, to faciliate the later reordering.
    // We create it in a local scope so that we also account for the
    // destruction time.
    {
        vector<Store> auxStores(stores);
        vector<Store*> storePtrs;
        for (auto it = auxStores.begin(); it != auxStores.end(); it++)
            storePtrs.push_back(&(*it));
        sort(storePtrs.begin(), storePtrs.end(), compareStorePtr);
        stores.clear();
        for (auto it = storePtrs.begin(); it != storePtrs.end(); it++)
            stores.push_back(**it);
        
    } // auxStores will be destroyed here
    
    // End the timing, report, and verify the sort worked
    Time end5;
    report("STL sort of pointers", end5-start5, stores);
    assert(isSorted(stores));
}