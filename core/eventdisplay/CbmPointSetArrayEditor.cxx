/* Copyright (C) 2005-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: J. Brandt, Florian Uhlig [committer] */

// -------------------------------------------------------------------------
// -----               CbmPointSetArrayEditor source file              -----
// -----                Created 18/07/05  by J. Brandt                 -----
// -----               Following class TEvePointSetArrayEditor         -----
// -------------------------------------------------------------------------

#include "CbmPointSetArrayEditor.h"

#include "CbmPointSetArray.h"  // for CbmPointSetArray

#include <TGComboBox.h>         // for TGComboBox
#include <TGFrame.h>            // for TGCompositeFrame, kFixedWidth, kHoriz...
#include <TGLabel.h>            // for TGLabel
#include <TGLayout.h>           // for TGLayoutHints, kLHintsExpandX, kLHint...
#include <TGedFrame.h>          // for TGedFrame
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TObject.h>            // for TObject

////////////////////////////////////////////////////////////////////////////////
/// Constructor.

CbmPointSetArrayEditor::CbmPointSetArrayEditor(const TGWindow* p, Int_t width, Int_t height, UInt_t options,
                                               Pixel_t back)
  : TGedFrame(p, width, height, options | kVerticalFrame, back)
  , fM(0)
  , fComboColor(0)
  , fComboMarker(0)
{
  fM = 0;
  MakeTitle("CbmPointSetArray");

  // frame for ComboBox Color
  TGCompositeFrame* fCframe = new TGCompositeFrame(this, width, 20, kHorizontalFrame | kFixedWidth);
  TGLabel* fLabel           = new TGLabel(fCframe, "Color:");
  fCframe->AddFrame(fLabel, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 1, 1, 2, 1));
  fComboColor = new TGComboBox(fCframe);
  // options for Color-Coding (have to be in CbmPointSetArray::ApplyColor() aswell with same integer)
  fComboColor->AddEntry("Id", 3);
  fComboColor->AddEntry("Time", 1);
  fComboColor->AddEntry("ToT", 2);
  fComboColor->AddEntry("Red", 4);
  fComboColor->Resize(170, 20);
  // connect ComboBox to functionality DoColor()
  fComboColor->Connect("Selected(Int_t)", "CbmPointSetArrayEditor", this, "DoColor()");
  fCframe->AddFrame(fComboColor, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 1, 1, 2, 1));
  AddFrame(fCframe, new TGLayoutHints(kLHintsTop, 1, 1, 2, 1));

  // frame for ComboBox Marker
  TGCompositeFrame* fCframe2 = new TGCompositeFrame(this, width, 20, kHorizontalFrame | kFixedWidth);
  TGLabel* fLabel2           = new TGLabel(fCframe2, "Marker:");
  fCframe2->AddFrame(fLabel2, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 1, 1, 2, 1));
  fComboMarker = new TGComboBox(fCframe2);
  // options for Marker-Coding (have to be in CbmPointSetArray::ApplyMarker() aswell with same integer)
  fComboMarker->AddEntry("const.", 3);
  fComboMarker->AddEntry("CluSize", 1);
  fComboMarker->AddEntry("CluSize (real)", 2);
  fComboMarker->Resize(170, 20);
  // connect ComboBox to functionality DoMarker()
  fComboMarker->Connect("Selected(Int_t)", "CbmPointSetArrayEditor", this, "DoMarker()");
  fCframe2->AddFrame(fComboMarker, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 1, 1, 2, 1));

  AddFrame(fCframe2, new TGLayoutHints(kLHintsTop, 1, 1, 2, 1));
}

////////////////////////////////////////////////////////////////////////////////
/// Destructor.

CbmPointSetArrayEditor::~CbmPointSetArrayEditor() {}

////////////////////////////////////////////////////////////////////////////////
/// Set model object.

void CbmPointSetArrayEditor::SetModel(TObject* obj)
{
  fM = dynamic_cast<CbmPointSetArray*>(obj);
  // set default selected values
  fComboColor->Select(fM->GetColorMode());
  fComboMarker->Select(fM->GetMarkerMode());
}

/////////////////////////////////////////////////////////////////////////////////
/// Set the colorMode of CbmPointSetArray

void CbmPointSetArrayEditor::DoColor()
{
  fM->SetColorMode(fComboColor->GetSelected());
  fM->ApplyColorMode();
  Update();
}

/////////////////////////////////////////////////////////////////////////////////
/// Set the markerMode of CbmPointSetArray

void CbmPointSetArrayEditor::DoMarker()
{
  fM->SetMarkerMode(fComboMarker->GetSelected());
  fM->ApplyMarkerMode();
  Update();
}

ClassImp(CbmPointSetArrayEditor);
