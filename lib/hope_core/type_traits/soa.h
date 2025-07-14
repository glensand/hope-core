/* Copyright (C) 2025 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope-core
 */

#pragma once

#include "hope_core/tuple/tuple_from_struct.h"

namespace hope {

	// array of struct -> struct of arrays
	template<typename TSoa, typename TAos>
	void soa(const TAos* in, TSoa& out, std::size_t count) {
		auto&& dst_tuple = tuple_from_struct(out, field_policy::reference{});
		for (std::size_t i = 0; i < count; ++i) {
			auto&& src_tuple = tuple_from_struct(in[i], field_policy::reference{});
			for_each(src_tuple, dst_tuple, [i](const auto& src, auto&& dst)
			{
				dst[i] = src;
			});
		}
	}

	// array of struct -> struct of arrays
	template<typename TSoa, typename TAos>
	void soa(const std::vector<TAos>& in, TSoa& out) {
		soa(in.data(), out, in.size());
	}

	// array of struct -> struct of arrays + allocate memory, required resize() method
	template<typename TSoa, typename TAos>
	void soa_with_resize(const std::vector<TAos>& in, TSoa& out) {
		auto&& dst_tuple = tuple_from_struct(out, field_policy::reference{});
		dst_tuple.for_each([size = in.size()](auto& v) {
			v.resize(size);
		});
		soa(in.data(), out, in.size());
	}

	// struct of arrays -> array of struct
	template<typename TSoa, typename TAos>
	auto aos(const TSoa& in, TAos* out, std::size_t count) {
		const auto& src_tuple = tuple_from_struct(in, field_policy::reference{});
		for (std::size_t i = 0; i < count; ++i) {
			auto&& dst_tuple = tuple_from_struct(out[i], field_policy::reference{});
			for_each(src_tuple, dst_tuple, [i](const auto& src, auto&& dst)
			{
				dst = src[i];
			});
		}
	}

	// struct of arrays -> array of struct
	template<typename TSoa, typename TAos>
	auto aos(const TSoa& in, std::vector<TAos>& out) {
		aos(in, out.data(), out.size());
	}

	// helper function
	template<typename... Ts>
	auto make_toa(const flat_tuple<Ts...>&) {
		return flat_tuple<std::vector<Ts>...>{};
	}

	// array of struct -> tuple of arrays
	template<typename TAos>
	auto toa(const TAos* in, std::size_t count) {
		auto&& dst_tuple = make_toa(tuple_from_struct(TAos{}));
		for (std::size_t i = 0; i < count; ++i) {
			auto&& src_tuple = tuple_from_struct(in[i], field_policy::reference{});
			for_each(src_tuple, dst_tuple, [i](const auto& src, auto&& dst)
			{
				dst.push_back(src);
			});
		}
		return dst_tuple;
	}
}