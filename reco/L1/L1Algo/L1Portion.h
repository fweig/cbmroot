/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

#ifndef L1Portion_H
#define L1Portion_H

#include <vector>

#include "L1TrackPar.h"

using std::vector;
// class for portions organization

template<typename T>
class L1Portion;

template<>
class L1Portion<L1TrackPar> {
public:
  typedef L1TrackPar T;
  typedef L1Vector<T> vType;

  L1Portion() {};
  L1Portion(int size) { reserve(size); };
  L1Portion(int size, int size2) : dataSize(size2)
  {
    reserve(size);
    //     reserve2(size2);
  };
  vType& operator[](int i) { return a[i]; }
  void resize(int size) { a.resize(size); };
  void reserve(int size) { a.reserve(size); };
  void reserve2(int size)
  {
    for (unsigned int i = 0; i < a.size(); i++)
      a[i].reserve(size);
  };
  void push_back(vType& v) { a.push_back(v); };
  void add_void()
  {
    vType v("L1Portion<L1TrackPar>::add_void");
    //     v.resize(dataSize);
    a.push_back(v);
    a[a.size() - 1].reserve(dataSize);
  };
  void add_void(int i)
  {
    T t;
    a[i].push_back(t);
  };

  int CalcSize()
  {
    int size = 0;
    for (unsigned int i = 0; i < a.size(); i++)
      size += a[i].size();
    return size * sizeof(T);
  };

private:
  vector<vType> a {"L1Portion<L1TrackPar>::a"};
  //   int mainSize; // size of a
  int dataSize {0};  // size of vType
};

template<>
class L1Portion<L1FieldRegion> {
public:
  typedef L1FieldRegion T;
  typedef L1Vector<T> vType;

  L1Portion() {};
  L1Portion(int size) { reserve(size); };
  L1Portion(int size, int size2) : dataSize(size2)
  {
    reserve(size);
    //     reserve2(size2);
  };

  vType& operator[](int i) { return a[i]; }
  void resize(int size) { a.resize(size); };
  void reserve(int size) { a.reserve(size); };
  void reserve2(int size)
  {
    for (unsigned int i = 0; i < a.size(); i++)
      a[i].reserve(size);
  };
  void push_back(vType& v) { a.push_back(v); };
  void add_void()
  {
    vType v("L1Portion<L1FieldRegion>::add_void");
    //     v.resize(dataSize);
    a.push_back(v);
    a[a.size() - 1].reserve(dataSize);
  };
  void add_void(int i)
  {
    T t;
    a[i].push_back(t);
  };

  int CalcSize()
  {
    int size = 0;
    for (unsigned int i = 0; i < a.size(); i++)
      size += a[i].size();
    return size * sizeof(T);
  };

private:
  vector<vType> a {"L1Portion<L1FieldRegion>::a"};
  //   int mainSize; // size of a
  int dataSize {0};  // size of vType
};


template<typename T>
class L1Portion {
public:
  typedef vector<T> vType;

  L1Portion() {};
  L1Portion(int size) { reserve(size); };
  L1Portion(int size, int size2) : dataSize(size2)
  {
    reserve(size);
    //     reserve2(size2);
  };
  vType& operator[](int i) { return a[i]; }
  void resize(int size) { a.resize(size); };
  void reserve(int size) { a.reserve(size); };
  void reserve2(int size)
  {
    for (int i = 0; i < a.size(); i++)
      a[i].reserve(size);
  };
  void push_back(vType& v) { a.push_back(v); };
  void add_void()
  {
    vType v("L1Portion<T>::add_void");
    //     v.resize(dataSize);
    a.push_back(v);
    a[a.size() - 1].reserve(dataSize);
  };
  void add_void(int i)
  {
    T t;
    a[i].push_back(t);
  };

  int CalcSize()
  {
    int size = 0;
    for (int i = 0; i < a.size(); i++)
      size += a[i].size();
    return size * sizeof(T);
  };

private:
  vector<vType> a {"L1Portion<T>::a"};
  //   int mainSize; // size of a
  int dataSize {0};  // size of vType
};

#endif  // L1Portion_H
