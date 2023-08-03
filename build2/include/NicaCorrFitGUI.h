/*
 * NicaCorrFitGUI.h
 *
 *  Created on: 11 sty 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_NICACORRFITGUI_H_
#define NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_NICACORRFITGUI_H_

#include <TF1.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGSlider.h>
#include <TGWindow.h>
#include <TObject.h>
#include <TRandom.h>
#include <TRootEmbeddedCanvas.h>


#include "NicaCorrFit.h"

class NicaCorrFitParButton : public TGHorizontalFrame {
  TGNumberEntry* fNumberEntry;
  TGComboBox* fComboBox;
  std::vector<double> fSteps;
  Bool_t fDiscrete;

public:
  NicaCorrFitParButton(const TGWindow* p = 0,
                       UInt_t w          = 1,
                       UInt_t h          = 1,
                       UInt_t options    = kChildFrame,
                       Pixel_t back      = GetDefaultFrameBackground());
  void Init(NicaCorrFitGUI* gui, TString parName, NicaFitParam par);
  Double_t GetValue();
  virtual ~NicaCorrFitParButton() {};
  ClassDef(NicaCorrFitParButton, 0)
};

class NicaCorrFitChiSelector : public TGHorizontalFrame {
  TGComboBox* fParName;
  TGNumberEntry* fMin;
  TGNumberEntry* fMax;
  TGNumberEntry* fSteps;

public:
  NicaCorrFitChiSelector(const TGWindow* p = 0,
                         UInt_t w          = 1,
                         UInt_t h          = 1,
                         UInt_t options    = kChildFrame,
                         Pixel_t back      = GetDefaultFrameBackground());
  void Init(NicaCorrFitGUI* gui, NicaCorrFit* f);
  void GetValue(Int_t& par, Int_t& steps, Double_t& min, Double_t& max);
  virtual ~NicaCorrFitChiSelector() {};
  ClassDef(NicaCorrFitChiSelector, 0)
};


class NicaCorrFitGUI : public TGMainFrame {
  NicaCorrFit* fFunc;
  NicaCorrFitParButton** fSliders;
  TGCheckButton* fNormStyle;
  NicaCorrFitChiSelector* fPairChi1;
  NicaCorrFitChiSelector* fPairChi2;
  TGCheckButton* fChiMin;
  Double_t fInitalNorm;
  const Int_t fNormIndex;
  void SetParams();

public:
  NicaCorrFitGUI(NicaCorrFit* f);
  void ApplyParams();
  void DrawChi2();
  virtual ~NicaCorrFitGUI();
  ClassDef(NicaCorrFitGUI, 0)
};


#endif /* NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_NICACORRFITGUI_H_ */
