/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov [committer] */

/// \file   CbmQaPie.cxx
/// \brief  Implementation of the CbmQaPie class
/// \author Sergey Gorbunov <se.gorbunov@gsi.de>
/// \date   07.11.2020

#include "CbmQaPie.h"

#include "TBrowser.h"
#include "TBuffer.h"
#include "TPad.h"

ClassImp(CbmQaPieSlice);
ClassImp(CbmQaPie);

CbmQaPie::CbmQaPie(const CbmQaPie& cpy) : TPie(cpy.GetName(), cpy.GetTitle(), cpy.fNvals)
{
  /// Prevent original copy constructor from a crash
  cpy.TAttText::Copy(*this);
  fAngularOffset = cpy.fAngularOffset;
  fX             = cpy.fX;
  fY             = cpy.fY;
  fRadius        = cpy.fRadius;
  for (Int_t i = 0; i < fNvals; ++i) {
    *fPieSlices[i] = *cpy.fPieSlices[i];
    ((CbmQaPieSlice*) fPieSlices[i])->SetPie(this);
  }
}

void CbmQaPie::Browse(TBrowser* b)
{
  /// Draw CbmQaPie by a mouse click in the TBrowser
  Draw(b ? b->GetDrawOption() : "");
  gPad->Update();
}

void CbmQaPie::Draw(Option_t* option)
{
  // Prevents original TPie::Draw() from crashing when there are no entries
  double sum = 0.;
  for (int i = 0; i < fNvals; i++) {
    sum += fabs(GetEntryVal(i));
  }
  if (sum < 1.e-20) {
    for (int i = 0; i < fNvals; i++) {
      SetEntryVal(i, 1.e-20);
    }
  }
  TPie::Draw(option);
  if (sum < 1.e-20) {
    for (int i = 0; i < fNvals; i++) {
      SetEntryVal(i, 0.);
    }
  }
}

void CbmQaPie::Streamer(TBuffer& R__b)
{
  /// The Streamer is declared by ClassDef() macro
  /// Stream an object of class CbmQaPie
  ///

  if (R__b.IsReading()) {

    for (int i = 0; i < fNvals; i++) {
      if (gPad && gPad->GetListOfPrimitives()) { gPad->GetListOfPrimitives()->Remove(fPieSlices[i]); }
      delete fPieSlices[i];
      fPieSlices[i] = nullptr;
    }
    delete[] fPieSlices;
    fPieSlices = nullptr;
    fNvals     = 0;

    R__b.ReadClassBuffer(CbmQaPie::Class(), this);

    fNvals     = fSliceStore.size();
    fPieSlices = new TPieSlice*[fNvals];
    for (int i = 0; i < fNvals; i++) {
      fSliceStore[i].SetPie(this);
      fPieSlices[i] = new TPieSlice(fSliceStore[i]);
    }
    fSliceStore.clear();
  }
  else {

    fSliceStore.resize(fNvals);
    for (int i = 0; i < fNvals; i++) {
      fSliceStore[i] = *fPieSlices[i];
      fSliceStore[i].SetPie(nullptr);
    }
    TPieSlice** tmp = fPieSlices;
    fPieSlices      = nullptr;
    fNvals          = 0;
    R__b.WriteClassBuffer(CbmQaPie::Class(), this);
    fPieSlices = tmp;
    fNvals     = fSliceStore.size();
    fSliceStore.clear();
  }
}
