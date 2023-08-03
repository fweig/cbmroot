/*
 * NicaCorrFitFunctor.h
 *
 *  Created on: 18 maj 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFITFUNCTOR_H_
#define NICACORRFITFUNCTOR_H_

#include <TString.h>

#include "NicaArray.h"
#include "NicaCorrFitDatabase.h"

class NicaCorrFitInfo;
class TFile;
class TTree;
/**
 * class that represents entire map from corrfit tree
 */

class NicaCorrFitFunctor : public TObject {
  TString fMapFile;
  NicaCorrFitInfo* fInfo;
  NicaCorrFitDbParamsSetup fSetup;
  TTree* fChain;
  TFile* fFile;
  Int_t fParams;
  NicaArray_1<Float_t>* fData;

public:
  NicaCorrFitFunctor(TString map_file = "");
  void PrintInfo();
  void GetParameterConfig(Int_t no, TString& name, Double_t& min, Double_t& max, Int_t& nsteps);
  Int_t GetNParams() const;
  NicaArray_1<Float_t>* GetData(Double_t* params);
  virtual ~NicaCorrFitFunctor();
  ClassDef(NicaCorrFitFunctor, 1)
};

#endif /* NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_MAPGENERATOR_MULTIDIM_NICACORRFITFUNCTOR_H_                                           \
        */
