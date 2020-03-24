/*
 * Configuration parser with metaprogramming
 * Copyright (C) 2016 iTX Technologies
 *
 * This file is part of Cenisys.
 *
 * Cenisys is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cenisys is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Cenisys.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CENISYS_CONFIGPARSER_H
#define CENISYS_CONFIGPARSER_H

#include <unordered_map>
#include <vector>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/variadic.hpp>
#include <boost/utility/string_ref.hpp>
#include <toml/toml.h>

namespace Cenisys {
	template <typename T>
	struct is_toml_container {
		static constexpr bool value = false;
	}
	
	template <typename T>
	struct is_toml_container<std::vector<T>> {
		static constexpr bool value = true;
	}
	
	template <>
	struct is_toml_container<toml::Table> {
		static constexpr bool value = false;
	}
	
	template <typename T>
	constexpr bool is_toml_container_v = is_toml_container<T>::value;
	
	template <typename T>
	struct ConfigParser {
		void operator()(const toml::Value &value, T &val) const {
			val = value.as<T>();
		}
	}
	
	template <typename T>
	struct ConfigParser<std::vector<T>> {
		void operator()(const toml::Value &value, std::vector<T> &val) const {
			for(const auto &i : value.as<toml::Array>()) {
				val.push_back({});
				ConfigParser<T>()(i, val.back());
			}
		}
	}
	
	template <typename T>
	struct ConfigParser<std::unordered_map<std::string, T>> {
		void operator()(const toml::Value &value, std::unordered_map<std::string, T> &val) const {
			for(const auto &i : value.as<toml::Table>()) {
				ConfigParser<T>()(i.second, val.insert({i.first, T()}).first->second);
			}
		}
	}
	
} // namespace Cenisys

#endif // CENISYS_CONFIGPARSER_H