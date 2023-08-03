/*
 * NicaDetectedTrack.h
 *
 *  Created on: 7 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAEXPTRACK_H_
#define NICAEXPTRACK_H_

#include "NicaHelix.h"
#include "NicaToFTrack.h"
#include "NicaTpcTrack.h"
#include "NicaTrack.h"
#include <TObject.h>
#include <TVector3.h>

/**
 * basic representation of experimental track
 */
class NicaExpTrack : public NicaTrack {
public:
private:
  Int_t fNHits;
  Float_t fChi2;
  Float_t fTrackLenght;
  TVector3* fDCA;

public:
  NicaExpTrack();
  /**
   *
   * @return number of hits in all detectors
   */
  inline Int_t GetNHits() const { return fNHits; };
  /**
   *
   * @return chi2 of fitting in all detectors
   */
  inline Float_t GetChi2() const { return fChi2; }
  /**
   *
   * @return track lenghth
   */
  inline Float_t GetTrackLenght() const { return fTrackLenght; }
  /**
   *
   * @return DCA vector
   */
  inline TVector3* GetDCA() const { return fDCA; };
  /**
   *
   * @param nHits set total number of htis
   */
  void SetNHits(Int_t nHits) { fNHits = nHits; }
  /**
   *
   * @param chi2 set chi2 of fit in all detectors
   */
  void SetChi2(Float_t chi2) { fChi2 = chi2; }
  /**
   * set track lenght
   * @param lenght
   */
  void SetTrackLenght(Float_t lenght) { fTrackLenght = lenght; }
  /**
   *
   * @param detID detector id
   * @return
   */
  virtual TObject* GetDetTrack(const UInt_t /*detID*/) const { return nullptr; };
  virtual void CopyData(NicaTrack* other);
  /**
   *
   * @return true if track is global
   */
  Bool_t IsGlobal() const { return TESTBIT(fType, kGlobal); };
  /**
   * set status of track
   * @param global set true if track is global
   */
  void SetGlobal(Bool_t global = kTRUE);
  /**
   * return track property by ID, this should be positve number above 100 (for ID outside of framework)
   * @param fieldID
   * @return
   */
  virtual Float_t GetFieldVal(Int_t fieldID) const;
  /**
   * return name of the track property by ID
   * @param fieldID
   * @return
   */
  virtual TString GetFieldName(Int_t fieldID) const;
  virtual ~NicaExpTrack();
  ClassDef(NicaExpTrack, 1)
};

class NicaExpTrackHelix : public NicaExpTrack {
  NicaHelix* fHelix;

protected:
  void UpdateHelix();

public:
  NicaExpTrackHelix();
  virtual void CopyData(NicaTrack* other);
  /**
   *
   * @return helix  - warning assumed fiedl at const Z!
   */
  inline NicaHelix* GetHelix() const { return fHelix; };
  virtual ~NicaExpTrackHelix();
  ClassDef(NicaExpTrackHelix, 1)
};

#endif /* NICAEXPTRACK_H_ */
