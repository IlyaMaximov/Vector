#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "Vector.h"
#include <vector>

using testing::Eq;
using std::cout, std::vector;

TEST(Vector, InitTest) {
    Vector<int> a;
    a.pushBack(45);
    a.pushBack(25);
    a.pushBack(4);
    a.pushBack(5);
    a.pushBack(5);

    for (auto elem: a) {
        cout << elem << " ";
    }
    cout << std::endl;

    std::cout << a.getCapacity() << '\n';
    a.shrink_to_fit();
    std::cout << a.getCapacity() << '\n';

    for (auto elem: a) {
        cout << elem << " ";
    }
    cout << std::endl;
}