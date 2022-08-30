/* Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#ifndef L1Vector_H
#define L1Vector_H

/// @file L1Vector.h
/// @author Sergey Gorbunov
/// @date 2021-06-16


#include "L1Def.h"
#ifndef FAST_CODE
#include <FairLogger.h>
#endif
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#include <sstream>

/// L1Vector class is a wrapper around std::vector.
/// It does the following:
/// 1. gives names to vectors for better debugging
/// 2. controls the out-of-range access in debug mode
/// 3. supresses methods that are currently not controlled
/// 4. warns when slow memory operations are called,
///    i.e. when the preallocated capacity is reached and the entire vector should be copied to a new place
/// 5. blocks usage of boolean vectors, as they have a special
///    space-optimized but slow implementation in std. (Use L1Vector<char> instead).
///


template<class T>
class L1Vector : private std::vector<T> {
  friend class boost::serialization::access;

public:
  typedef std::vector<T> Tbase;

  template<typename... Tinput>
  L1Vector(Tinput... value) : Tbase(value...)
  {
  }

  template<typename... Tinput>
  L1Vector(const char* name, Tinput... value) : Tbase(value...)
                                              , fName(name)
  {
  }

  /// Constructor to make initializations like L1Vector<int> myVector {"MyVector", {1, 2, 3}}
  L1Vector(const std::string& name, std::initializer_list<T> init) : Tbase(init), fName(name) {}

  /// Copy constructor
  L1Vector(const L1Vector& v) : Tbase()
  {
    //LOG(info) << "\033[1;32mCALL L1Vector copy constructor\033[0m";
    *this = v;
  }

  /// Move constructor
  L1Vector(L1Vector&& v) noexcept : Tbase(std::move(v)), fName(std::move(v.fName))
  {
    //LOG(info) << "\033[1;32mCALL L1Vector move constructor\033[0m";
  }

  /// Copy assignment operator
  L1Vector& operator=(const L1Vector& v)
  {
    //LOG(info) << "\033[1;32mCALL L1Vector copy assignment operator\033[0m";
    if (this != &v) {
      fName = v.fName;
      Tbase::reserve(v.capacity());  // make sure that the capacity is transmitted
      Tbase::assign(v.begin(), v.end());
    }
    return *this;
  }

  /// Move assignment operator
  L1Vector& operator=(L1Vector&& v) noexcept
  {
    //LOG(info) << "\033[1;32mCALL L1Vector move assignment operator\033[0m";
    if (this != &v) {
      std::swap(fName, v.fName);
      Tbase::swap(v);
    }
    return *this;
  }

  void SetName(const std::string& s) { fName = s; }

  void SetName(const std::basic_ostream<char>& s)
  {
    // helps to set a composed name in a single line via:
    // SetName(std::stringstream()<<"my name "<<..whatever..);
    fName = dynamic_cast<const std::stringstream&>(s).str();
  }

  std::string GetName() const
  {
    std::string s = " L1Vector<";
    s += fName + "> ";
    return s;
  }

  template<typename... Tinput>
  void reset(std::size_t count, Tinput... value)
  {
    // does the same as Tbase::assign(), but works with the default T constructor too
    // (no second parameter)
    Tbase::clear();
    Tbase::resize(count, value...);
  }

  template<typename... Tinput>
  void enlarge(std::size_t count, Tinput... value)
  {
    if (count < Tbase::size()) {
      LOG(fatal) << "L1Vector \"" << fName << "\"::enlarge(" << count
                 << "): the new size is smaller than the current one " << Tbase::size() << ", something goes wrong."
                 << std::endl;
      assert(count >= Tbase::size());
    }
    if ((!Tbase::empty()) && (count > Tbase::capacity())) {
      LOG(warning) << "L1Vector \"" << fName << "\"::enlarge(" << count << "): allocated capacity of "
                   << Tbase::capacity() << " is reached, the vector of size " << Tbase::size()
                   << " will be copied to the new place." << std::endl;
    }
    Tbase::resize(count, value...);
  }

  void reduce(std::size_t count)
  {
    if (count > Tbase::size()) {
      LOG(fatal) << "L1Vector \"" << fName << "\"::reduce(" << count
                 << "): the new size is bigger than the current one " << Tbase::size() << ", something goes wrong."
                 << std::endl;
      assert(count < Tbase::size());
    }
    Tbase::resize(count);
  }

  void reserve(std::size_t count)
  {
    if (!Tbase::empty()) {
      LOG(fatal) << "L1Vector \"" << fName << "\"::reserve(" << count << "): the vector is not empty; "
                 << " it will be copied to the new place." << std::endl;
      assert(Tbase::empty());
    }
    Tbase::reserve(count);
  }


  template<typename Tinput>
  void push_back(Tinput value)
  {
#ifndef FAST_CODE
    if (Tbase::size() >= Tbase::capacity()) {
      LOG(warning) << "L1Vector \"" << fName << "\"::push_back(): allocated capacity of " << Tbase::capacity()
                   << " is reached, re-allocate and copy." << std::endl;
    }
#endif
    Tbase::push_back(value);
  }

  template<typename Tinput>
  void push_back_no_warning(Tinput value)
  {
    Tbase::push_back(value);
  }

  template<typename... Tinput>
  void emplace_back(Tinput&&... value)
  {
#ifndef FAST_CODE
    if (Tbase::size() >= Tbase::capacity()) {
      LOG(warning) << "L1Vector \"" << fName << "\"::emplace_back(): allocated capacity of " << Tbase::capacity()
                   << " is reached, re-allocate and copy." << std::endl;
    }
#endif
    Tbase::emplace_back(value...);
  }

  T& operator[](std::size_t pos)
  {
#ifndef FAST_CODE
    if (pos >= Tbase::size()) {
      LOG(fatal) << "L1Vector \"" << fName << "\": trying to access element " << pos
                 << " outside of the vector of the size of " << Tbase::size() << std::endl;
      assert(pos < Tbase::size());
    }
#endif
    return Tbase::operator[](pos);
  }

  const T& operator[](std::size_t pos) const
  {
#ifndef FAST_CODE
    if (pos >= Tbase::size()) {
      LOG(fatal) << "L1Vector \"" << fName << "\": trying to access element " << pos
                 << " outside of the vector of the size of " << Tbase::size() << std::endl;
      assert(pos < Tbase::size());
    }
#endif
    return Tbase::operator[](pos);
  }

  T& back()
  {
#ifndef FAST_CODE
    if (Tbase::size() == 0) {
      LOG(fatal) << "L1Vector \"" << fName << "\": trying to access element of an empty vector" << std::endl;
      assert(Tbase::size() > 0);
    }
#endif
    return Tbase::back();
  }

  const T& back() const
  {
#ifndef FAST_CODE
    if (Tbase::size() == 0) {
      LOG(fatal) << "L1Vector \"" << fName << "\": trying to access element of an empty vector" << std::endl;
      assert(Tbase::size() > 0);
    }
#endif
    return Tbase::back();
  }

  using Tbase::begin;
  using Tbase::capacity;
  using Tbase::clear;
  using Tbase::end;
  using Tbase::insert;  //TODO:: make it private
  using Tbase::pop_back;
  using Tbase::rbegin;
  using Tbase::reserve;
  using Tbase::size;
  using typename Tbase::iterator;

private:
  std::string fName {"no name"};
  using Tbase::assign;  // use reset() instead
  using Tbase::at;
  using Tbase::resize;

  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& boost::serialization::base_object<Tbase>(*this);
    ar& fName;
  }
};

///
/// std::vector<bool> has a special implementation that is space-optimized
/// and therefore slow and not thread-safe.
/// That is why one should use L1Vector<char> instead.
///
template<>
class L1Vector<bool> {  /// Make sure that L1Vector<bool> is not used
};


#endif
