/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_CONFIG_DESERIALIZE_H
#define CBM_ALGO_BASE_CONFIG_DESERIALIZE_H

#include <sstream>
#include <string_view>

#include <fmt/format.h>
#include <yaml-cpp/yaml.h>

#include "BaseTypes.h"
#include "Prelude.h"
#include "Property.h"

namespace cbm::algo::config
{

  template<typename T, T... Values, typename Func>
  constexpr void ForEach(std::integer_sequence<T, Values...>, Func&& func)
  {
    (func(std::integral_constant<T, Values> {}), ...);
  }

  template<typename T>
  T Read(const YAML::Node& node)
  {
    using Type = std::remove_cv_t<std::remove_reference_t<T>>;

    // TODO: error handling
    if constexpr (IsBaseType<Type>) { return node.as<Type>(); }
    else if constexpr (IsEnum<Type>) {
      return static_cast<Type>(node.as<std::underlying_type_t<Type>>());
    }
    else if constexpr (IsVector<Type>) {
      Type vector;
      for (const auto& element : node) {
        vector.push_back(Read<typename Type::value_type>(element));
      }
      return vector;
    }
    else if constexpr (IsArray<Type>) {
      Type array {};
      auto vector = Read<std::vector<typename Type::value_type>>(node);
      if (vector.size() != array.size()) {
        throw std::runtime_error(fmt::format("Array size mismatch: expected {}, got {}", array.size(), vector.size()));
      }
      std::copy(vector.begin(), vector.end(), array.begin());
      return array;
    }
    else {
      Type object;
      constexpr auto nProperties = std::tuple_size<decltype(Type::Properties)>::value;
      ForEach(std::make_integer_sequence<std::size_t, nProperties> {}, [&](auto index) {
        auto& property   = std::get<index>(Type::Properties);
        using ValueType  = std::remove_cv_t<std::remove_reference_t<decltype(property.Get(object))>>;
        ValueType& value = property.Get(object);
        value            = Read<ValueType>(node[std::string {property.Key()}]);
      });
      return object;
    }
  }

  template<typename T>
  std::string MakeDocString(int indent = 0)
  {
    using Type = std::remove_cv_t<std::remove_reference_t<T>>;

    std::stringstream docString;

    if constexpr (IsBaseType<Type>) { docString << BaseTypeToStr<Type>(); }
    else if constexpr (IsVector<Type> || IsArray<Type>) {
      using ChildType = typename Type::value_type;
      if constexpr (IsBaseType<ChildType>) {
        docString << std::string(indent, ' ') << "list of " << BaseTypeToStr<ChildType>() << std::endl;
      }
      else {
        docString << std::string(indent, ' ') << "list of" << std::endl;
        docString << MakeDocString<ChildType>(indent + 2);
      }
    }
    else {
      constexpr auto nProperties = std::tuple_size<decltype(Type::Properties)>::value;
      ForEach(std::make_integer_sequence<std::size_t, nProperties> {}, [&](auto index) {
        using ChildType = std::remove_cv_t<
          std::remove_reference_t<decltype(std::get<index>(Type::Properties).Get(std::declval<Type>()))>>;
        auto& property = std::get<index>(Type::Properties);
        if constexpr (IsBaseType<ChildType>) {
          docString << std::string(indent, ' ') << property.Key() << ": " << property.Description() << " ["
                    << BaseTypeToStr<ChildType>() << "]" << std::endl;
        }
        else {
          docString << std::string(indent, ' ') << property.Key() << ": " << property.Description() << std::endl;
          docString << MakeDocString<ChildType>(indent + 2);
        }
      });
    }

    return docString.str();
  }

  class Dump {

  public:
    template<typename T>
    std::string operator()(const T& object)
    {
      YAML::Emitter ss;
      ss << YAML::BeginDoc;
      DoDump(object, ss);
      ss << YAML::EndDoc;
      return ss.c_str();
    }

  private:
    template<typename T>
    void DoDump(const T& object, YAML::Emitter& ss, std::optional<YAML::EMITTER_MANIP> formatEntries = {})
    {
      if constexpr (IsBaseType<T>) { ss << object; }
      else if constexpr (IsVector<T> || IsArray<T>) {
        ss << YAML::BeginSeq;
        for (const auto& element : object) {
          if (formatEntries.has_value()) { ss << formatEntries.value(); }
          DoDump(element, ss);
        }
        ss << YAML::EndSeq;
      }
      else {
        constexpr auto nProperties = std::tuple_size<decltype(T::Properties)>::value;
        ss << YAML::BeginMap;
        ForEach(std::make_integer_sequence<std::size_t, nProperties> {}, [&](auto index) {
          auto& property = std::get<index>(T::Properties);
          auto& value    = property.Get(object);
          auto format    = property.Format();
          ss << YAML::Key << std::string {property.Key()};
          if (format.has_value()) { ss << format.value(); }
          ss << YAML::Value;
          DoDump(value, ss, property.FormatEntries());
        });
        ss << YAML::EndMap;
      }
    }
  };

}  // namespace cbm::algo::config

#endif
