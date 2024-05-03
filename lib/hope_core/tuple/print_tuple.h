/* Copyright (C) 2021 - 2024 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope-core
 */

#pragma once

#include "hope_core/tuple/flat_tuple.h"
#include "hope_core/tuple/tuple_from_struct.h"

#include <iostream>

/*! \defgroup <reflection> Static reflection
    @{
    \file
    \brief Implementation of helper print functions. This functions might be used to print any instance of the
    aggregate type to the output stream; File contains a quit heavy include : iostream and should never be included
    at the header. Be careful with usages of these operators
*/

namespace hope{

    namespace detail {

        /**
         * Prints tuple to the given stream
         */
        template<typename TStream, typename... Ts, std::size_t... VIs>
        void print_impl(TStream& stream, const flat_tuple<Ts...>& tuple, std::index_sequence<VIs...>) {
            stream << "{ ";
            ((stream << (VIs == 0 ? "" : ", ") << tuple.template get<VIs>()), ...);
            stream << " };";
        }
    
    }

    /**
     * This overload of the stream operator is intended to be used in the hope namespace
     * Or might bw called via adl;
     */
    template<typename TStream, typename... Ts>
    constexpr TStream& operator<< (TStream& stream, const flat_tuple<Ts...>& tuple) {
        print_impl(stream, tuple, std::make_index_sequence<size(type_list<Ts...>{})>());
        return stream;
    }
}

/*! @} */