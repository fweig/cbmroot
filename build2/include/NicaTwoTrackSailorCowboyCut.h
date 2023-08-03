/*
 * NicaTwoTrackSailorCowboyCut.h
 *
 *  Created on: 15 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATWOTRACKSAILORCOWBOYCUT_H_
#define NICATWOTRACKSAILORCOWBOYCUT_H_

#include "NicaHelix.h"
#include "NicaTwoTrackCut.h"
class NicaTwoTrackSailorCowboyCut : public NicaTwoTrackCut {
  Double_t fThreshold;
  NicaHelixZ fHz1, fHz2;

 protected:
  Int_t Sailor() const { return 1; };
  Int_t Cowboy() const { return -1; };
  Double_t NormalizeAngle(const TVector3 pos, Double_t x, Double_t y) const;
  Bool_t AreBetween(Double_t phi, Double_t sign) const;
  Bool_t Check();

 public:
  NicaTwoTrackSailorCowboyCut();
  NicaTwoTrackSailorCowboyCut(const NicaTwoTrackSailorCowboyCut &other);
  NicaTwoTrackSailorCowboyCut &operator=(
      const NicaTwoTrackSailorCowboyCut &other);
  virtual Bool_t Pass(NicaTwoTrack *pair);
  /**
   * if tracks cross at less than r_min then threat them as sailros
   * @param r_min
   */
  void SetThreshold(Double_t r_min) { fThreshold = r_min; };
  void Cowboys() { SetMinAndMax(-1); };
  void Sailors() { SetMinAndMax(1); }
  Bool_t Init(Int_t formatId);
  virtual ~NicaTwoTrackSailorCowboyCut();
  virtual NicaPackage *Report() const;
  ClassDef(NicaTwoTrackSailorCowboyCut, 1)
};

#endif /* NICATWOTRACKSAILORCOWBOYCUT_H_ */
