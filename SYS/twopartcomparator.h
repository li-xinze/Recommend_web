#ifndef TWOPARTCOMPARATOR_H
#define TWOPARTCOMPARATOR_H
/*#include <leveldbapi.h>
#include <leveldb/comparator.h>
#include <leveldb/slice.h>

class TwoPartComparator : public leveldb::Comparator
{
    public:
    TwoPartComparator();
    // Three-way comparison function:
    //   if a < b: negative result
    //   if a > b: positive result
    //   else: zero result
    int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
        int a1, a2, b1, b2;
        ParseKey(a, &a1, &a2);
        ParseKey(b, &b1, &b2);
        if (a1 < b1) return -1;
        if (a1 > b1) return +1;
        if (a2 < b2) return -1;
        if (a2 > b2) return +1;
        return 0;
    }

    // Ignore the following methods for now:
    const char* Name() const { return "TwoPartComparator"; }
    void FindShortestSeparator(std::string*, const leveldb::Slice&) const { }
    void FindShortSuccessor(std::string*) const { }
};

#endif // TWOPARTCOMPARATOR_H*/
