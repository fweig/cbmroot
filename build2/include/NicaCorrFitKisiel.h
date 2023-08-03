/*
 * NicaCorrFitKisiel.h
 *
 *  Created on: 27 lis 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFITKISIEL_H_
#define NICACORRFITKISIEL_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "NicaArray.h"
#include "NicaCorrFit1DCF.h"
#include "NicaSplines.h"

class NicaAnaFile;
class TClonesArray;

/**
 * class for fitting correlation function by using map, similar procedure to
 * those used in A. Kisiel's CorrFit
 */
class NicaCorrFitKisiel : public NicaCorrFit1DCF {
  NicaAnaFile* fPackage;

protected:
  /**
   * map used to fitting
   */
  NicaSpline2D* fMap;
  /**
   * store information about numerical errors
   */
  NicaArray_1<Double_t>* fCFError;
  ENicaFemtoKinematics fMapFrame;
  Double_t fQPrfScale;
  void CalculateNumErrors(TH1* h);
  virtual void Check();
  virtual Double_t GetNumericalError(Int_t binX) const {
    if (fSkipError) return 0;
    return (*fCFError)[binX];
  };
  virtual Double_t CalculateCF(const Double_t* x, const Double_t* params) const;
  virtual Double_t CalculateCFExtrapolated(const Double_t* x, const Double_t* params) const;
  Bool_t fSkipError;
  NicaCorrFitKisiel(Int_t params);

public:
  NicaCorrFitKisiel();
  /**
   * l
   * @param draw_option @see NicaCorrFit1DCF, with option full+ee also numerical
   * errors are drawn as boxes around CF markers
   */
  virtual void Draw(Option_t* draw_option = "full+ee");
  /**
   * skip numerical errrors during calculations
   */
  virtual void SkipNumErrors() { fSkipError = kTRUE; }
  /**
   * load map from root file created by NicaCorrFitAnaMap
   * @param file name of file to load map from
   * @param kt_bin number of kt_bin
   */
  virtual void LoadMapFromFile(TString file, Int_t kt_bin = 0);
  /**
   * set map directly from histogram
   * @param h histogram with map
   * @param mapFrame map frame
   */
  virtual void LoadMap(TH2D* h, ENicaFemtoKinematics mapFrame);
  virtual void LoadMaps(TClonesArray* array);
  void SmearMap() { fMap->Refit(); };
  NicaSpline2D* GetMap() const { return fMap; };
  /**
   * load map from "war
   */
  virtual ~NicaCorrFitKisiel();
  ClassDef(NicaCorrFitKisiel, 1)
};

#endif /* NICACORRFITKISIEL_H_ */
