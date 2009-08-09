#include <iostream>
#include <cassert>

using namespace std;

#include "ResourceHandle.h"

struct TestResource
{
};

template<class T>
struct TestAdapter
{
    T& toT() {return *(T*)this;}

    size_t count() {return toT().get()->mRefCount;}

	void create(const char* name) {}
};

typedef ResourceHandle< TestResource, TestAdapter > ResHandle;

int main()
{
    ResHandle one, two, empty;

    one.create("one");

    assert(one.get()->mRefCount == one.count());
    assert(one.get()->mRefCount == 1);

    two = one;

    assert(one.get()->mRefCount == 2);
    assert(two.get()->mRefCount == 2);

    ResHandle three(one);

    assert(one.get()->mRefCount == 3);
    assert(two.get()->mRefCount == 3);
    assert(three.get()->mRefCount == 3);

    three = empty;

    assert(three.get() == 0);
    assert(one.get()->mRefCount == 2);
    assert(two.get()->mRefCount == 2);

    two = empty;

    assert(two.get() == 0);
    assert(one.get()->mRefCount == 1);

    cout << "Hello world!" << endl;
    return 0;
}
