/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

/// \file   CbmQaPie.h
/// \brief  Definition of the CbmQaPie class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \date   07.11.2020

#ifndef CbmQaPie_H
#define CbmQaPie_H

#include "TPieSlice.h"

#include "TPie.h"

#include <vector>

class TBrowser;

/// A helper class for accessing protected members of TPieSlice
///
class CbmQaPieSlice : public TPieSlice {
public:
  /// assignment operator
  CbmQaPieSlice& operator=(const TPieSlice& inp) { return (*this = (const CbmQaPieSlice&) (inp)); }
  /// set a TPie pointer
  void SetPie(TPie* p) { fPie = p; }

  ClassDef(CbmQaPieSlice, 1);
};

/// A modification of TPie which fixes the following issues:
///
/// 1. When a TPie is read from a file as a part of TCanvas, it crashes at the destructor.
/// 2. When a TPie is created via copy constructor it crashes at the destructor.
/// 3. An empty TPie crashes at Draw()
/// 4. When one clicks on a TPie in the TBrowser, the TBrowser crashes.
/// 5. TBrowser dosen't draw a TPie by a mouse click.
///
class CbmQaPie : public TPie {
public:
  /// Reimplementation of any existing TPie constructor
  template<typename... Types>
  CbmQaPie(Types... args) : TPie(args...)
  {
  }

  /// Prevent original copy constructor from a crash
  CbmQaPie(const CbmQaPie& cpy);

  /// Destructor
  ~CbmQaPie() {}

  /// Draw TPie by a mouse click in the TBrowser
  void Browse(TBrowser* b);

  /// Prevents original TPie::Draw() from crashing when there are no entries
  void Draw(Option_t* option = "l");

private:
  /// a vector for slice streaming. It replaces the original array of pointers.
  std::vector<CbmQaPieSlice> fSliceStore;

  ClassDef(CbmQaPie, 1);
};

#endif
