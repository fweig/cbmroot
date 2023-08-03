/*
 * NicaTrackMC.h
 *
 *  Created on: 1 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAMCTRACK_H_
#define NICAMCTRACK_H_

#include "NicaTrack.h"
/**
 * basic class for stroing MC tracks something like "NicaParticle"
 */
class NicaMCTrack : public NicaTrack {

protected:
  TLorentzVector* fFreez;
  TLorentzVector* fStart;
  Int_t fPDG;

public:
  NicaMCTrack();
  NicaMCTrack& operator=(const NicaMCTrack& other);
  /**
   *
   * @return track pid code
   */
  virtual Int_t GetPdg() const { return fPDG; };
  /**
   * @return freezout position
   */
  inline TLorentzVector* GetFreezoutPosition() const { return fFreez; };
  /**
   *
   * @return start position of particle
   */
  inline TLorentzVector* GetStartPosition() const { return fStart; };
  virtual void CopyData(NicaTrack* other);
  void Boost(Double_t x, Double_t y, Double_t z);
  void SetPdg(Int_t pdg) { fPDG = pdg; };
  virtual Float_t GetFieldVal(Int_t fieldID) const;
  virtual TString GetFieldName(Int_t fieldID) const;
  virtual ~NicaMCTrack();
  ClassDef(NicaMCTrack, 1)
};

#endif /* NICAMCTRACK_H_ */
