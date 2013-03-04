
#include <knownset.h>

/**
*   Knownset data structures
*/
struct _Tree
{
    PICC_Tree father;
    PICC_Tree left;
    PICC_Tree right;
    void* val;
};

struct _KnownsetTree
{
    int type;
    PICC_Tree tree;
};

struct _KnownsetList
{
    int type;
    int size;
    void** liste;
};

struct _Knownset
{
    int type;
};

struct _KnownSetIterator
{
    PICC_KnownSet set;
};

struct _KnownSetTreeIterator
{
    PICC_KnownSetTree set;
    PICC_Tree current;
    PICC_Tree next;
};

struct _KnownSetListIterator
{
    PICC_KnownSetList set;
    int next;
};

extern void PICC_KnownSet_inv(PICC_KnownSet ks);
