/*
 * NicaTrackToFMass2Cut.h
 *
 *  Created on: 10 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKTOFMASS2CUT_H_
#define NICATRACKTOFMASS2CUT_H_
#include "NicaTrackExpCut.h"
/**
 * class for cut on ToF mass2
 * user can set standard minium and maximum cut (like in basic to cut)
 * and by SetPolyLIneUp/Down apply additional polynomial-like cut that can be
 * expressed as a + b*p + c*p^2
 */
class NicaTrackToFMass2Cut : public NicaTrackExpCut {
 protected:
  Double_t fLowLine[3];
  Double_t fHighLine[3];
  void GetPoly(Double_t &a, Double_t &b, Double_t &c, Double_t x1, Double_t y1,
               Double_t x2, Double_t y2, Double_t x3, Double_t y3);
  NicaTrackToFMass2Cut(Int_t nParams);

 public:
  NicaTrackToFMass2Cut();
  NicaTrackToFMass2Cut(const NicaTrackToFMass2Cut &other);
  void SetPolyLineDown(Double_t a, Double_t b, Double_t c);
  void SetPolyLineUp(Double_t a, Double_t b, Double_t c);
  void GetPolyUp(Double_t &a, Double_t &b, Double_t &c) const;
  void GetPolyDown(Double_t &a, Double_t &b, Double_t &c) const;
  void SetUpPoints(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
                   Double_t x3, Double_t y3);
  void SetDownPoints(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
                     Double_t x3, Double_t y3);
  virtual Bool_t Init(Int_t formad_id);
  virtual Bool_t Pass(NicaTrack *tr);
  virtual ~NicaTrackToFMass2Cut();
  virtual NicaPackage *Report() const;
  ClassDef(NicaTrackToFMass2Cut, 1)
};
/**
 * cut like  @see NicaTrackToFMass2Cut but also reject all particles with
 * mass==0
 */
class NicaTrackTofMass2CutNoZeros : public NicaTrackToFMass2Cut {
 public:
  NicaTrackTofMass2CutNoZeros(){};
  NicaTrackTofMass2CutNoZeros(const NicaTrackToFMass2Cut &other);
  virtual Bool_t Pass(NicaTrack *tr);
  virtual ~NicaTrackTofMass2CutNoZeros(){};
  ClassDef(NicaTrackTofMass2CutNoZeros, 1)
};
#endif /* NICATRACKTOFMASS2CUT_H_ */
