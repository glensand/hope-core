/* Copyright (C) 2020 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope-core
 */

#include "gtest/gtest.h"

#include "hope_core/tuple/flat_tuple.h"
#include "hope_core/tuple/tuple_from_struct.h"
#include "hope_core/tuple/tuple_from_struct_unsafe.h"
#include "hope_core/tuple/tuple_utils.h"

namespace {

struct pod_imitator {
    constexpr static int DefaultInt{ 11 };
    float val1{ 10.f };
    int val2{ DefaultInt };
};

struct test_struct_3 {
    double _0 = 0;
    float _1 = 0;
    int _2 = 0;
    bool _3 = false;
};

struct recursive_structure final {
    test_struct_3 v1;
    int value = 0;
};

struct struct_string final {
    std::string name;
    int index = 0;
};

struct struct_int_vector final {
    std::string name;
    std::vector<int> vec_i;
};

struct struct_string_vector final {
    std::string name;
    std::vector<int> vec_i;
    std::vector<std::string> vec_s;
};

}

TEST(TupleTest, ConstexprInitialization)
{
    constexpr auto tuple = hope::make_flat_tuple(1.f, std::string_view("string"), pod_imitator{});

    static_assert(tuple.get<0>() == tuple.get<float>());
    static_assert(tuple.get<1>() == tuple.get<std::string_view>());
    static_assert(tuple.get<2>().val2 == pod_imitator::DefaultInt);
}

TEST(TupleTest, DedactionGuide)
{
    constexpr hope::flat_tuple tuple(1.f, std::string_view("string"), pod_imitator{});

    static_assert(tuple.get<0>() == tuple.get<float>());
    static_assert(tuple.get<1>() == tuple.get<std::string_view>());
    static_assert(tuple.get<2>().val2 == pod_imitator::DefaultInt);
}

TEST(TupleTest, ValueChanging)
{
    auto tuple = hope::make_flat_tuple(pod_imitator{}, pod_imitator{ 0.1f, 1 });
    auto&& secondInt = tuple.get<1>().val2;
    ASSERT_EQ(secondInt, 1);
    secondInt = 12;
    ASSERT_EQ(secondInt, 12);
}

TEST(TupleTest, TupleFromStruct)
{
    constexpr test_struct_3 ts3{ 0.1, 0.1f, 11, true};
    constexpr auto ts3Tuple = hope::tuple_from_struct(ts3);

    static_assert(ts3Tuple.get<2>() == ts3._2);
    static_assert(ts3Tuple.get<3>() == ts3._3);
    ASSERT_TRUE(std::abs(ts3Tuple.get<0>() - ts3._0) < std::numeric_limits<double>::epsilon());
}

TEST(TupleTest, TupleFromStructUnsafe)
{
    constexpr test_struct_3 ts3{ 0.1, 0.1f, 11, true };
    auto ts3Tuple = hope::tuple_from_struct_unsafe(ts3);

    ASSERT_TRUE(ts3Tuple.get<0>() == ts3._0);
}

TEST(TupleTest, StructStringValue)
{
    struct_string s{ "field1", 11 };
    auto&& tuple = hope::tuple_from_struct(s);

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();

    ASSERT_TRUE(s.name == first);
    ASSERT_TRUE(s.index == second);
}

TEST(TupleTest, StructVectorIntValue)
{
    const struct_int_vector s{ "field1", {11, 12, 13} };
    auto&& tuple = hope::tuple_from_struct(s);

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();

    ASSERT_TRUE(s.name == first);

    for (std::size_t i{ 0 }; i < second.size(); ++i)
        ASSERT_TRUE(second[i] == s.vec_i[i]);
}

TEST(TupleTest, StructVectorStringValue)
{
    struct_string_vector s{ "field1", {11, 12, 13}, {"first", "second", "third"} };
    auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();
    auto&& third = tuple.get<2>();

    ASSERT_TRUE(s.name == first);

    for (std::size_t i{ 0 }; i < second.size(); ++i)
        ASSERT_TRUE(second[i] == s.vec_i[i]);

    for (std::size_t i{ 0 }; i < third.size(); ++i)
        ASSERT_TRUE(third[i] == s.vec_s[i]);
}

TEST(TupleTest, StructStringReference)
{
    struct_string s{ "field1", 11 };
    auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();

    ASSERT_TRUE(s.name == first);
    ASSERT_TRUE(s.index == second);

    first = "field1_modified";
    second = 12;

    ASSERT_TRUE(s.name == "field1_modified");
    ASSERT_TRUE(s.index == 12);
}

TEST(TupleTest, StructVectorIntReference)
{
    struct_int_vector s{ "field1", {11, 12, 13} };
    auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();

    ASSERT_TRUE(s.name == first);

    for(std::size_t i{ 0 }; i < second.size(); ++i)
        ASSERT_TRUE(second[i] == s.vec_i[i]);

    first = "field1_modified";
    second.push_back(144);

    ASSERT_TRUE(s.name == "field1_modified");
    ASSERT_TRUE(s.vec_i.back() == 144);
}

TEST(TupleTest, StructVectorStringReference)
{
    struct_string_vector s{ "field1", {11, 12, 13}, {"first", "second", "third"} };
    auto&& tuple = hope::tuple_from_struct(s, hope::field_policy::reference{});

    auto&& first = tuple.get<0>();
    auto&& second = tuple.get<1>();
    auto&& third = tuple.get<2>();

    ASSERT_TRUE(s.name == first);

    for (std::size_t i{ 0 }; i < second.size(); ++i)
        ASSERT_TRUE(second[i] == s.vec_i[i]);

    for (std::size_t i{ 0 }; i < third.size(); ++i)
        ASSERT_TRUE(third[i] == s.vec_s[i]);

    first = "field1_modified";
    second.push_back(144);
    third.emplace_back("last");

    ASSERT_TRUE(s.name == "field1_modified");
    ASSERT_TRUE(s.vec_i.back() == 144);
    ASSERT_TRUE(s.vec_s.back() == "last");
}

TEST(TupleTest, TupleLikeHolder)
{
    struct s final
    {
        hope::flat_tuple<struct_int_vector, struct_string, struct_string_vector> t;

        const struct_int_vector& get1() const { return t.get<struct_int_vector>(); }
        struct_int_vector& get1() { return t.get<struct_int_vector>(); }
    };

    const s test_struct1;
    s test_struct2;

    auto&& res1 = test_struct1.get1();
    auto&& res2 = test_struct2.get1();
}

TEST(TupleTest, StructCompare){
    test_struct_3 s1, s2;
    ASSERT_TRUE(hope::compare(s1, s2));
    s1._0 = 1.f;
    ASSERT_TRUE(!hope::compare(s1, s2));
}

TEST(TupleTest, ComplicatedStructCompare){
    struct_string s1, s2;
    ASSERT_TRUE(hope::compare(s1, s2));
    s1.index = 10;
    ASSERT_TRUE(!hope::compare(s1, s2));
}

TEST(TupleTest, RecursiveStructCompare){
    recursive_structure s1, s2;
    ASSERT_TRUE(hope::compare(s1, s2));
    s1.v1._0 = 10.0;
    ASSERT_TRUE(!hope::compare(s1, s2));
}


TEST(TupleTest, StructureFieldValueChange){
    auto&& t = hope::make_flat_tuple(recursive_structure{}, 1, false);
    t.get<recursive_structure>().value = 1000;
    ASSERT_TRUE(t.get<recursive_structure>().value == 1000);
}