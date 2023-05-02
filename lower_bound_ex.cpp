// By Aliaksei Sanko
// http://lnkd.in/kVJXfm

 
#include <algorithm>
#include <iostream>
 
#include <time.h>
 
#include "binary_search.hpp"

using std::cout;
using std::cerr;
using std::endl;
 
#define ITER(stage)     \
if (stage <= curPlace)  \
{                       \
    enum { OFFSET = (1 << stage) / 2 }; \
    typedef I Array[OFFSET]; \
    first = ((Array*) first)[*(first + (OFFSET - 1)) < val]; \
}
 
template <int curPlace, typename I, typename V>
inline
I* fast_lower_bound(I* first, const V& val)
{
    ITER(28)
    ITER(27)
    ITER(26)
    ITER(25)
    ITER(24)
    ITER(23)
    ITER(22)
    ITER(21)
    ITER(20)
    ITER(19)
    ITER(18)
    ITER(17)
    ITER(16)
    ITER(15)
    ITER(14)
    ITER(13)
    ITER(12)
    ITER(11)
    ITER(10)
    ITER(9)
    ITER(8)
    ITER(7)
    ITER(6)
    ITER(5)
    ITER(4)
    ITER(3)
    ITER(2)
    ITER(1)
 
    return first;
}
 
 
#define HANDLE_GROUPS_3(a, b, c) \
    HANDLE_GROUPS_4(a, b, c, 0) \
    HANDLE_GROUPS_4(a, b, c, 1) \
    HANDLE_GROUPS_4(a, b, c, 2) \
    HANDLE_GROUPS_4(a, b, c, 3)
 
#define HANDLE_GROUPS_2(a, b) \
    HANDLE_GROUPS_3(a, b, 0) \
    HANDLE_GROUPS_3(a, b, 1) \
    HANDLE_GROUPS_3(a, b, 2) \
    HANDLE_GROUPS_3(a, b, 3)
 
 
#define HANDLE_GROUPS_1(a) \
    HANDLE_GROUPS_2(a, 0) \
    HANDLE_GROUPS_2(a, 1) \
    HANDLE_GROUPS_2(a, 2) \
    HANDLE_GROUPS_2(a, 3)
 
 
#define HANDLE_GROUPS_4(a, b, c, d) \
case (a * 64 + b * 16 + c * 4 + d): \
{ \
    enum { flags = (a * 64 + b * 16 + c * 4 + d) }; \
    HANDLE_FLAG(6); \
    HANDLE_FLAG(5); \
    HANDLE_FLAG(4); \
    HANDLE_FLAG(3); \
    HANDLE_FLAG(2); \
    HANDLE_FLAG(1); \
    HANDLE_FLAG(0); \
    break; \
}
 
#define HANDLE_FLAG(flag) \
{ \
    enum { curPlace = flag + SEGMENT * 7 }; \
    if (*(first + ((1 << curPlace) - 1)) < val) \
        first += (1 << curPlace); \
    else \
        return fast_lower_bound<curPlace>(first, val); \
    if (flags & (1 << flag)) \
    { \
        if (*(first + ((1 << curPlace) - 1)) < val) \
            first += (1 << curPlace); \
        else \
            return fast_lower_bound<curPlace>(first, val); \
    } \
}
 
 
template <typename I, int SEGMENT>
struct bound_continue_helper
{
    template<typename V>
    static I lower_bound_ex(I first, unsigned int distance, const V& val)
    {
        switch ((distance >> (SEGMENT * 7)) & 127)
        {
            HANDLE_GROUPS_1(0)
            HANDLE_GROUPS_1(1)
        }
 
        return bound_continue_helper<I, SEGMENT - 1>::lower_bound_ex(first, distance, val);
    }
};
 
template <typename I>
struct bound_continue_helper<I, -1>
{
    template<typename V>
    static I lower_bound_ex(I first, unsigned int, const V&)
    {
        return first;
    }
};
 
#undef HANDLE_GROUPS_4
#define HANDLE_GROUPS_4(a, b, c, d) \
case (a * 64 + b * 16 + c * 4 + d): \
{ \
    enum { flags = (a * 64 + b * 16 + c * 4 + d) }; \
    if (0 == flags) \
        return bound_entry_helper<I, SEGMENT-1>::lower_bound_ex(first, distance, val); \
    HANDLE_FLAG(6); \
    HANDLE_FLAG(5); \
    HANDLE_FLAG(4); \
    HANDLE_FLAG(3); \
    HANDLE_FLAG(2); \
    HANDLE_FLAG(1); \
    HANDLE_FLAG(0); \
    break; \
}
 
#undef HANDLE_FLAG
#define HANDLE_FLAG(flag) \
if (flags >= (1 << (flag+1))) \
{ \
    enum { curPlace = flag + SEGMENT * 7 }; \
    if (*(first + ((1 << curPlace) - 1)) < val) \
        first += (1 << curPlace); \
    else \
        return fast_lower_bound<curPlace>(first, val); \
    if (flags & (1 << flag)) \
    { \
        if (*(first + ((1 << curPlace) - 1)) < val) \
            first += (1 << curPlace); \
        else \
            return fast_lower_bound<curPlace>(first, val); \
    } \
}
 
template <typename I, int SEGMENT>
struct bound_entry_helper
{
    template<typename V>
    static I lower_bound_ex(I first, unsigned int distance, const V& val)
    {
        switch ((distance >> (SEGMENT * 7)) & 127)
        {
            HANDLE_GROUPS_1(0)
            HANDLE_GROUPS_1(1)
        }
 
        return bound_continue_helper<I, SEGMENT - 1>::lower_bound_ex(first, distance, val);
    }
};
 
template <typename I>
struct bound_entry_helper<I, -1>
{
    template<typename V>
    static I lower_bound_ex(I first, unsigned int, const V&)
    {
        return first;
    }
};
 
 
template<typename I, typename V>
inline I lower_bound_ex(I first, I last,
                        const V& val)
{
    unsigned int distance = (unsigned int) (last - first);
    unsigned int flags = distance + 1;
 
    return (flags >= 0x00004000)
        ? bound_entry_helper<I, 3>::lower_bound_ex(first, flags, val)
        : bound_entry_helper<I, 1>::lower_bound_ex(first, flags, val);
}
 
 
unsigned int HashKey(int key)
{
    return (key * 2654435761UL) >> 11;
}
 
 
void DoTest(int size, int iterations)
{
    cout << "size: " << size << "; iterations: " << iterations << endl;
 
    int* arr = new int[size + 1];
 
    for (int i = 0; i <= size; ++i)
        arr[i] = i;

    // lower_bound_ex
    clock_t start = clock();
 
    for (int k = 0; k < iterations; ++k)
        for (int i = 0; i < size; ++i)
        {
            int j = (HashKey(i) % size);
            int* p = lower_bound_ex(arr, arr + size - 1, j);
            if (j != *p)
            {
                cerr << "Erroneous lower_bound_ex test result: " << *p 
                    << " instead of " << j << endl;
                exit(1);
            }
        }
 
    cout << "  lower_bound_ex time: " <<
        (double)(clock() - start) / CLOCKS_PER_SEC  <<
        " seconds" << endl;
 
    // branchless_lower_bound
    start = clock();

    for (int k = 0; k < iterations; ++k)
        for (int i = 0; i < size; ++i)
        {
            int j = (HashKey(i) % size);
            int* p = branchless_lower_bound(arr, arr + size - 1, j);
            if (j != *p)
            {
                cerr << "Erroneous branchless_lower_bound test result: " << *p
                    << " instead of " << j << endl;
                exit(1);
            }
        }

    cout << "  branchless_lower_bound time: " <<
        (double)(clock() - start) / CLOCKS_PER_SEC <<
        " seconds" << endl;

    // std::lower_bound
    start = clock();
 
    for (int k = 0; k < iterations; ++k)
        for (int i = 0; i < size; ++i)
        {
            int j = (HashKey(i) % size);
            int* p = std::lower_bound(arr, arr + size - 1, j);
            if (j != *p)
            {
                cerr << "Erroneous std::lower_bound test result: " << *p 
                    << " instead of " << j << endl;
                exit(1);
            }
        }
 
    cout << "  std::lower_bound time: " <<
        (double)(clock() - start) / CLOCKS_PER_SEC  <<
        " seconds" << endl;
 
    delete[] arr;
}
 
 
int main(int argc, char* argv[])
{
   DoTest(128 * 1024, 100);
   DoTest(95369, 100);
 
   return 0;
}
