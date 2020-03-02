#include <gtest/gtest.h>
#include "Vector.h"
#include <vector>

using testing::Eq;
using std::vector;

TEST(Vector, InitTest) {
    Vector<int> simple_v;
    ASSERT_EQ(simple_v.capacity(), 0);
    ASSERT_EQ(simple_v.size(), 0);
    ASSERT_THROW(simple_v.at(0), std::out_of_range);
}

TEST(Vector, Constuctors) {
    // default constructor
    Vector<int> simple_v1(10);
    ASSERT_EQ(simple_v1.capacity(), 10);
    ASSERT_EQ(simple_v1.size(), 10);
    ASSERT_NO_THROW(simple_v1.at(9));
    ASSERT_THROW(simple_v1.at(10), std::out_of_range);

    // size + init_value
    Vector<float> simple_v2(3, 7.43f);
    ASSERT_EQ(simple_v2.capacity(), 3);
    ASSERT_EQ(simple_v2.size(), 3);
    ASSERT_NO_THROW(simple_v2.at(2));
    ASSERT_THROW(simple_v2.at(3), std::out_of_range);
    ASSERT_EQ(simple_v2[0], 7.43f);
    ASSERT_EQ(simple_v2[2], 7.43f);

    // copy constructor
    Vector<int> v1(2, 1);
    v1[0] = 7;
    Vector<int> v2(v1);
    ASSERT_EQ(v1.capacity(), v2.capacity());
    ASSERT_EQ(v1.capacity(), 2);
    ASSERT_EQ(v1.size(), v2.size());
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], v2[0]);
    ASSERT_EQ(v1[1], v2[1]);
    ASSERT_EQ(v1[0], 7);
    ASSERT_EQ(v1[1], 1);

    // move constructor
    Vector<int> v3(std::move(v1));
    ASSERT_EQ(v3.capacity(), 2);
    ASSERT_EQ(v3.size(), 2);
    ASSERT_EQ(v3[0], 7);
    ASSERT_EQ(v3[1], 1);
    EXPECT_EQ(v1.capacity(), 0);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_THROW(v1.at(0), std::out_of_range);
}

TEST(Vector, Assignment_Operator) {
    // without move-semantics
    Vector<int> v1(2, 1);
    v1[0] = 7;
    Vector<int> v2;
    v2 = v1;
    ASSERT_EQ(v1.capacity(), v2.capacity());
    ASSERT_EQ(v1.capacity(), 2);
    ASSERT_EQ(v1.size(), v2.size());
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], v2[0]);
    ASSERT_EQ(v1[1], v2[1]);
    ASSERT_EQ(v1[0], 7);
    ASSERT_EQ(v1[1], 1);

    // with move_sevantic
    Vector<int> v3;
    v3 = std::move(v1);
    ASSERT_EQ(v3.capacity(), 2);
    ASSERT_EQ(v3.size(), 2);
    ASSERT_EQ(v3[0], 7);
    ASSERT_EQ(v3[1], 1);
    EXPECT_EQ(v1.capacity(), 0);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_THROW(v1.at(0), std::out_of_range);
}

TEST(Vector, Pushback) {
    Vector<int> a;
    int x = 7;

    a.push_back(2);
    ASSERT_EQ(a[0], 2);
    ASSERT_EQ(a.size(), 1);
    EXPECT_EQ(a.capacity(), 1);

    a.push_back(x);
    ASSERT_EQ(a[1], 7);
    ASSERT_EQ(a.size(), 2);
    EXPECT_EQ(a.capacity(), 2);
    EXPECT_EQ(x, 7);

    a.push_back(x);
    ASSERT_EQ(a[2], 7);
    ASSERT_EQ(a.size(), 3);
    EXPECT_EQ(a.capacity(), 4);

    a.push_back(0);
    ASSERT_EQ(a[3], 0);
    ASSERT_EQ(a.size(), 4);
    EXPECT_EQ(a.capacity(), 4);
}

TEST(Vector, EmplaceBack_SimpleTest) {
    Vector<std::pair<int,int>> a;
    std::pair<int, int> x(2, 7);

    a.push_back(std::make_pair(2, 5));
    ASSERT_EQ(a[0], std::make_pair(2, 5));
    ASSERT_EQ(a.size(), 1);
    EXPECT_EQ(a.capacity(), 1);

    a.emplace_back(4, 5);
    ASSERT_EQ(a[1], std::make_pair(4, 5));
    ASSERT_EQ(a.size(), 2);
    EXPECT_EQ(a.capacity(), 2);

    a.push_back(std::move(x));
    ASSERT_EQ(a[2], std::make_pair(2, 7));
    ASSERT_EQ(a.size(), 3);
    EXPECT_EQ(a.capacity(), 4);
}

TEST(Vector, PopBack) {
    Vector<int> a;
    a.push_back(1);
    a.push_back(7);
    a.push_back(5);
    a.push_back(6);
    a.push_back(7);
    EXPECT_EQ(a.size(), 5);
    EXPECT_EQ(a.capacity(), 8);

    a.pop_back();
    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(a.capacity(), 8);

    a.pop_back();
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a.capacity(), 8);

    a.pop_back();
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a.capacity(), 4);

    a.pop_back();
    EXPECT_EQ(a.size(), 1);
    EXPECT_EQ(a.capacity(), 2);

    a.pop_back();
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.capacity(), 0);
}

TEST(Vector, EmplaceBack_HardTest) {
    Vector<Vector<int>> mass;
    Vector<int> v1(4, 1), v2;
    v2 = v1;

    mass.push_back(v1);
    ASSERT_EQ(mass.size(), 1);
    ASSERT_EQ(mass.capacity(), 1);
    ASSERT_EQ(mass[0], v1);
    ASSERT_EQ(v1.size(), 4);

    mass.push_back(std::move(v1));
    ASSERT_EQ(mass.size(), 2);
    ASSERT_EQ(mass.capacity(), 2);
    ASSERT_EQ(mass[1], v2);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);

    mass.emplace_back(4, 1);
    ASSERT_EQ(mass.size(), 3);
    ASSERT_EQ(mass.capacity(), 4);
    ASSERT_EQ(mass[2], v2);
    EXPECT_THROW(mass.at(3), std::out_of_range);
}

TEST(Vector, Clear) {
    Vector<int> a(5, 1);

    a.push_back(3);
    ASSERT_EQ(a.size(), 6);
    EXPECT_EQ(a.capacity(), 10);

    a.clear();
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.capacity(), 0);
    ASSERT_TRUE(a.empty());
    ASSERT_THROW(a.at(0), std::out_of_range);
}

TEST(Vector, Reserve_ShrinkToFit) {
    Vector<int> a(5, 6), b(5, 6);

    a.push_back(7);
    b.push_back(7);
    ASSERT_EQ(a.size(), 6);
    EXPECT_EQ(a.capacity(), 10);

    a.reserve(1000);
    ASSERT_EQ(a.size(), 6);
    EXPECT_EQ(a.capacity(), 1000);
    ASSERT_THROW(a.at(6), std::out_of_range);
    ASSERT_EQ(a, b);

    a.shrink_to_fit();
    ASSERT_EQ(a.size(), 6);
    EXPECT_EQ(a.capacity(), 6);
    ASSERT_THROW(a.at(6), std::out_of_range);
    ASSERT_EQ(a, b);
}

TEST(Vector, Resize) {
    Vector<int> a(5, 6), b(3, 6);

    a.resize(3);
    ASSERT_EQ(a.size(), 3);
    EXPECT_EQ(a.capacity(), 5);
    ASSERT_EQ(a, b);

    b.push_back(5);
    b.push_back(5);
    b.push_back(5);
    b.push_back(5);

    a.resize(7, 5);
    ASSERT_EQ(a.size(), 7);
    EXPECT_EQ(a.capacity(), 7);
    ASSERT_EQ(a, b);

    a.resize(1000);
    EXPECT_EQ(a[56], 0);
    EXPECT_EQ(a.capacity(), 1000);
    EXPECT_EQ(a.capacity(), 1000);

    a.resize(7);
    ASSERT_EQ(a.size(), 7);
    EXPECT_EQ(a.capacity(), 7);
    ASSERT_EQ(a, b);
}

TEST(Vector, CompareOperator_RangeBaseFor) {
    Vector<int> a(5, 6), b(a);

    ASSERT_TRUE(a == b);
    a.pop_back();
    ASSERT_TRUE(a != b);
    a.push_back(6);
    a.push_back(56);
    ASSERT_TRUE(a != b);
    a.pop_back();
    ASSERT_TRUE(a == b);

    a[0] = 0;
    a[4] = 8;
    ASSERT_EQ(a.front(), 0);
    ASSERT_EQ(a.back(), 8);

    for (auto& elem: a) {
        ++elem;
    }
    ASSERT_EQ(a.front(), 1);
    ASSERT_EQ(a.back(), 9);
}

TEST(Vector, ConstVector) {
    const Vector<int> a(5, 6);

    ASSERT_EQ(a.front(), 6);
    ASSERT_EQ(a.back(), 6);

    for (const auto& elem: a) {
        ASSERT_EQ(elem, 6);
    }
}