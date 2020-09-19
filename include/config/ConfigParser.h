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

namespace quantum {

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
            for (const auto &i : value.as<toml::Array>()) {
                val.push_back({});
                ConfigParser<T>()(i, val.back());
            }
        }
    }

    template <typename T>
    struct ConfigParser<std::unordered_map<std::string, T>> {
        void operator()(const toml::Value &value, std::unordered_map<std::string, T> &val) const {
            for (const auto &i : value.as<toml::Table>()) {
                ConfigParser<T>()(i.second, val.insert({i.first, T()}).first->second);
            }
        }
    }

}

#endif
