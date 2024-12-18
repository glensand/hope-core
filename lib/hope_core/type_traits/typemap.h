/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope-core
 */

#pragma once

#include "hope_core/type_traits/type_list.h"

#include <type_traits>

namespace hope {

    template <typename K, typename V>
    struct type_pair final {
        using Key = K;
        using Value = V;
    };

    template <typename... Types>
    class type_map final {
        constexpr static type_list<Types...> m_types{ };
    public:

        template <typename Key>
        static constexpr auto get() noexcept {
            constexpr auto index = index_of<Key>();
            static_assert(index < size(m_types));
            using pair = NthType<index, Types...>;
            return type_holder<typename pair::Value>();
        }

        template <typename Value>
        static constexpr auto get_v() noexcept {
            constexpr auto index = index_v<Value>();
            static_assert(index < size(m_types));
            using pair = NthType<index, Types...>;
            return type_holder<typename pair::Key>();
        }

        template<typename Key>
        using get_t = typename decltype(get<Key>())::Type;

        template<typename Value>
        using get_v_t = typename decltype(get_v<Value>())::Type;
    private:

        template <typename Key>
        constexpr static std::size_t index_of() {
            return find_if(m_types, [](auto pair) {
                using pair_t = typename decltype(pair)::Type;
                return std::is_same_v<Key, typename pair_t::Key>;
                });
        }

        template <typename Value>
        constexpr static std::size_t index_v() {
            return find_if(m_types, [](auto pair) {
                using pair_t = typename decltype(pair)::Type;
                return std::is_same_v<Value, typename pair_t::Value>;
                });
        }

        template <typename T>
        struct type_checker final : std::false_type {};

        template <typename Key, typename Value>
        struct type_checker<type_pair<Key, Value>> final : std::true_type {};

        static_assert(all_of<type_checker>(m_types));
    };

}