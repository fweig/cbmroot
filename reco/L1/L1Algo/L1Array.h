/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

#ifndef L1Array_h
#define L1Array_h 1

#include <Logger.h>

#include <array>
#include <string>
#include <utility>

#include "L1Assert.h"

/// Idea: std::array based class, where the size() and copacity() return different numbers.
/// Size returns the effective size of array (i.e. the number of elements, which were really)

// TODO: What is better to be used here - std::size or int. Needs to be tested (S.Zharko)


template<class Tvalue, std::size_t MaxCapacity>
class L1Array : private std::array<Tvalue, MaxCapacity> {
public:
  // Type definitions
  using Tbase        = std::array<Tvalue, MaxCapacity>;
  using Tsize        = typename Tbase::size_type;
  using Tpointer     = typename Tbase::pointer;
  using Tcpointer    = typename Tbase::const_pointer;
  using Treference   = typename Tbase::reference;
  using Tcreference  = typename Tbase::const_reference;
  using Titerator    = typename Tbase::iterator;
  using Tciterator   = typename Tbase::const_iterator;
  using Tdifference  = typename Tbase::difference_type;
  using Triterator   = typename Tbase::reverse_iterator;
  using Tcriterator  = typename Tbase::const_reverse_iterator;
  using TarrayTraits = typename _GLIBCXX_STD_C::__array_traits<Tvalue, MaxCapacity>;

  template<typename... Tinput>
  L1Array(Tinput... value) : Tbase(value...)
  {
  }

  template<typename... Tinput>
  L1Array(int size, const char* name, Tinput... value) : Tbase(value...)
                                                       , fSize(static_cast<Tsize>(size))
                                                       , fName(name)
  {
  }

  // TODO: Probably, there is a way to do this better... (S.Zharko)
  L1Array(const char* name, std::initializer_list<Tvalue> init) : Tbase(), fName(name)
  {
#ifndef FAST_CODE
    if _GLIBCXX17_CONSTEXPR (L1Assert::kAssertionLevel >= 2 && Tbase::max_size() < init.size()) {
      LOG(fatal) << '\n'
                 << " ***** Level 2 assertion failed: " << '\n'
                 << " *****   message: in L1Array with the name " << fName << ": the size of the initializer list ("
                 << init.size() << ") is larger then the maximum size of the array (" << Tbase::max_size() << ')';
    }
#endif  // FAST_CODE
    fSize = init.size();
    for (auto idx = 0; idx < fSize; ++idx) {
      operator[](idx) = *(init.begin() + idx);
    }
  }

  L1Array(std::initializer_list<Tvalue> init) : L1Array("no name", init) {}

  /// Swap function
  //friend void swap(L1Array<T, N>& lhs, L1Array<T, N>& rhs)
  //{
  //  std::swap(static_cast<Tbase&>(lhs), static_cast<Tbase&>(rhs));
  //  std::swap(lhs.fName, rhs.fName);
  //  std::swap(lhs.fSize, rhs.fSize);
  //}

  /// Gets name of the array
  std::string GetName() const
  {
    std::string str = " L1Array<";
    str += fName + "> ";
    return str;
  }


  /// Sets name of array from stringstream
  void SetName(const std::basic_ostream<char>& name) { fName = dynamic_cast<const std::stringstream&>(name).str(); }

  /// Sets name of array from std::string parameters
  void SetName(const std::string& name) { fName = name; }

  //
  // BASE METHODS
  //
  // Element access

  Treference operator[](Tsize pos) noexcept
  {
#ifndef FAST_CODE
    if _GLIBCXX17_CONSTEXPR (L1Assert::kAssertionLevel >= 3 && pos >= fSize) {
      LOG(fatal) << '\n'
                 << " ***** Level 3 assertion failed: " << '\n'
                 << " *****   message: in L1Array with the name " << fName << ": index out of range (pos = " << pos
                 << " >= size = " << fSize << ")";
    }
    // TODO: do we need a simple assertion here to prevent the index out of range? (S.Zharko)
#endif  // FAST_CODE
    return Tbase::operator[](pos);
  }

  Tcreference operator[](Tsize pos) const noexcept
  {
#ifndef FAST_CODE
    if _GLIBCXX17_CONSTEXPR (L1Assert::kAssertionLevel >= 3 && pos >= fSize) {
      LOG(fatal) << '\n'
                 << " ***** Level 3 assertion failed: " << '\n'
                 << " *****   message: in L1Array with the name " << fName << ": index out of range (pos = " << pos
                 << " >= size = " << fSize << ")";
    }
    // TODO: do we need a simple assertion here to prevent the index out of range? (S.Zharko)
#endif  // FAST_CODE
    return Tbase::operator[](pos);
  }

  using Tbase::front;

  _GLIBCXX17_CONSTEXPR Treference back() noexcept { return fSize ? *(end() - 1) : *end(); }

  constexpr Tcreference back() const noexcept { return fSize ? *(end() - 1) : *end(); }

  using Tbase::data;
  // Iterators
  using Tbase::begin;
  //using Tbase::end;

  // NOTE: _GLIBCXX17_CONSTEXPR is used in a base class and places constexpr for c++17+ and nothing instead
  _GLIBCXX17_CONSTEXPR Titerator end() noexcept { return Titerator(data() + fSize); }

  _GLIBCXX17_CONSTEXPR Tciterator end() const noexcept { return Tciterator(data() + fSize); }

  _GLIBCXX17_CONSTEXPR Triterator rbegin() noexcept { return Triterator(end()); }

  _GLIBCXX17_CONSTEXPR Tcriterator rbegin() const noexcept { return Tcriterator(end()); }

  using Tbase::cbegin;
  using Tbase::rend;

  _GLIBCXX17_CONSTEXPR Tciterator cend() const noexcept { return Tciterator(data() + fSize); }

  _GLIBCXX17_CONSTEXPR Tcriterator crbegin() const noexcept { return Tcriterator(end()); }

  using Tbase::crend;

  //   Capasity
  constexpr Tsize size() const noexcept { return fSize; }

  using Tbase::max_size;
  constexpr bool empty();

  //   Operations
  void fill(const Tvalue& val) { std::fill_n(begin(), size(), val); }

  void swap(L1Array& other) noexcept(TarrayTraits::_Is_nothrow_swappable::value)
  {
    std::swap(fName, other.fName);
    std::swap(fSize, other.fSize);
    std::swap_ranges(begin(), end(), other.begin());
  }

private:
  using Tbase::at;

  Tsize fSize {0};
  std::string fName {"no name"};
};


#endif  // L1Array_h
