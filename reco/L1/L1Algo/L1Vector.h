/* Copyright (C) 2021-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

#ifndef L1Vector_H
#define L1Vector_H

/// @file L1Vector.h
/// @author Sergey Gorbunov
/// @date 2021-06-16


#include "CbmL1Def.h"
#ifndef FAST_CODE
#include <FairLogger.h>
#endif
#include <sstream>

using namespace std;

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
public:
  typedef std::vector<T> Tbase;

  L1Vector(const char* name = "no name") : Tbase(), fName(name) {};

  L1Vector(const L1Vector& v) : Tbase(), fName(v.fName)
  {
    Tbase::reserve(v.capacity());
    Tbase::assign(v.begin(), v.end());
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
  void resize(std::size_t count, Tinput... value)
  {
#ifndef FAST_CODE
    if (!Tbase::empty() && (count > Tbase::capacity())) {
      LOG(WARNING) << "L1Vector \"" << fName << "\"::resize(" << count << "): allocated capacity of "
                   << Tbase::capacity() << " is reached, re-allocate and copy." << std::endl;
    }
#endif
    Tbase::resize(count, value...);
  }

  template<typename Tinput>
  void push_back(Tinput value)
  {
#ifndef FAST_CODE
    if (Tbase::size() >= Tbase::capacity()) {
      LOG(WARNING) << "L1Vector \"" << fName << "\"::push_back(): allocated capacity of " << Tbase::capacity()
                   << " is reached, re-allocate and copy." << std::endl;
    }
#endif
    Tbase::push_back(value);
  }

  template<typename... Tinput>
  void emplace_back(Tinput&&... value)
  {
#ifndef FAST_CODE
    if (Tbase::size() >= Tbase::capacity()) {
      LOG(WARNING) << "L1Vector \"" << fName << "\"::emplace_back(): allocated capacity of " << Tbase::capacity()
                   << " is reached, re-allocate and copy." << std::endl;
    }
#endif
    Tbase::emplace_back(value...);
  }

  T& operator[](std::size_t pos)
  {
#ifndef FAST_CODE
    if (pos >= Tbase::size()) {
      LOG(ERROR) << "L1Vector \"" << fName << "\": trying to access element " << pos
                 << " outside of the vector of the size of " << Tbase::size() << std::endl;
      exit(0);
    }
#endif
    return Tbase::operator[](pos);
  }

  const T& operator[](std::size_t pos) const
  {
#ifndef FAST_CODE
    if (pos >= Tbase::size()) {
      LOG(ERROR) << "L1Vector \"" << fName << "\": trying to access element " << pos
                 << " outside of the vector of the size of " << Tbase::size() << std::endl;
      exit(0);
    }
#endif
    return Tbase::operator[](pos);
  }

  T& back()
  {
#ifndef FAST_CODE
    if (Tbase::size() == 0) {
      LOG(ERROR) << "L1Vector \"" << fName << "\": trying to access element of an empty vector" << std::endl;
      exit(0);
    }
#endif
    return Tbase::back();
  }

  const T& back() const
  {
#ifndef FAST_CODE
    if (Tbase::size() == 0) {
      LOG(ERROR) << "L1Vector \"" << fName << "\": trying to access element of an empty vector" << std::endl;
      exit(0);
    }
#endif
    return Tbase::back();
  }

  void assign(std::size_t count, const T& value)
  {
#ifndef FAST_CODE
    if (!Tbase::empty() && (count > Tbase::capacity())) {
      LOG(WARNING) << "L1Vector \"" << fName << "\"::assign(" << count << "): allocated capacity of "
                   << Tbase::capacity() << " is reached, re-allocate and copy." << std::endl;
    }
#endif
    Tbase::assign(count, value);
  }

  using Tbase::begin;
  using Tbase::capacity;
  using Tbase::clear;
  using Tbase::end;
  using Tbase::pop_back;
  using Tbase::reserve;
  using Tbase::size;
  using typename Tbase::iterator;

private:
  std::string fName;
  using Tbase::at;
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
