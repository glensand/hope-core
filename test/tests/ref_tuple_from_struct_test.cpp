/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope-core
 */

#include "gtest/gtest.h"

#include "hope_core/tuple/flat_tuple.h"
#include "hope_core/tuple/tuple_from_struct.h"

namespace detail {
    struct test_struct final {
        int _0;
        float _1;
        double _2;
    };
}

TEST(StructToRefTuple, AllCvModifiedTypesAreEqual)
{
    // for some undefined reason gcc compiler does not support ref qualifiers at the constexpr context  
    constexpr detail::test_struct instance{ 1, 1.1f, 1.3 };

    auto ref = hope::tuple_from_struct(instance, hope::field_policy::reference{});

    ASSERT_TRUE(ref.get<0>() == ref.get<int>());
    ASSERT_TRUE(ref.get<0>() == ref.get<int&>());
    ASSERT_TRUE(ref.get<0>() == ref.get<const int&>());
    ASSERT_TRUE(ref.get<0>() == ref.get<const int>());

    ASSERT_TRUE(ref.get<1>() == ref.get<float>());
    ASSERT_TRUE(ref.get<1>() == ref.get<float&>());
    ASSERT_TRUE(ref.get<1>() == ref.get<const float&>());
    ASSERT_TRUE(ref.get<1>() == ref.get<const float>());
}

TEST(StructToRefTuple, StructureCanBeModifiedViaTuple)
{
    detail::test_struct instance{ 1, 1.1f, 1.3 };
    auto&& ref = hope::tuple_from_struct(instance, hope::field_policy::reference{});
    ref.get<0>() = 42;
    ref.get<1>() = 42.f;

    ASSERT_TRUE(42 == instance._0);
    ASSERT_TRUE(abs(42.f - instance._1) < FLT_EPSILON);
}
