/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_CONFIG_PROPERTY_H
#define CBM_ALGO_BASE_CONFIG_PROPERTY_H

#include <optional>
#include <string_view>
#include <tuple>

#include <yaml-cpp/emittermanip.h>

#include "Prelude.h"

namespace cbm::algo::config
{

  template<typename Class, typename T>
  class Property {

  private:
    T Class::*fMember;
    std::string_view fKey;
    std::string_view fDescription;
    std::optional<YAML::EMITTER_MANIP> fFormat;
    std::optional<YAML::EMITTER_MANIP> fFormatEntries;

  public:
    using ClassType = Class;
    using ValueType = T;

    Property() = delete;

    constexpr Property(T Class::*member, std::string_view key, std::string_view description = "",
                       std::optional<YAML::EMITTER_MANIP> fmt = {}, std::optional<YAML::EMITTER_MANIP> fmtEntries = {})
      : fMember(member)
      , fKey(key)
      , fDescription(description)
      , fFormat(fmt)
      , fFormatEntries(fmtEntries)
    {
    }

    Property(const Property&) = delete;
    Property& operator=(const Property&) = delete;

    Property(Property&&) = default;
    Property& operator=(Property&&) = default;

    std::string_view Key() const { return fKey; }
    std::string_view Description() const { return fDescription; }
    std::optional<YAML::EMITTER_MANIP> Format() const { return fFormat; }
    std::optional<YAML::EMITTER_MANIP> FormatEntries() const { return fFormatEntries; }

    T& Get(Class& object) const { return object.*fMember; }
    const T& Get(const Class& object) const { return object.*fMember; }

    void Set(Class& object, const T& value) const { object.*fMember = value; }
  };

  template<typename Class, typename T>
  Property(T Class::*member, std::string_view key, std::string_view description) -> Property<Class, T>;

}  // namespace cbm::algo::config

#endif  // CBM_ALGO_BASE_CONFIG_PROPERTY_H
