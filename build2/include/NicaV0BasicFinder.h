/*
 * NicaV0BasicFinder.h
 *
 *  Created on: 27 lut 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAV0BASICFINDER_H_
#define NICAV0BASICFINDER_H_

#include <FairTask.h>
#include <Rtypes.h>
#include <RtypesCore.h>

#include "NicaCutMonitor.h"
#include "NicaTrackAna.h"
#include "NicaTrackCut.h"
#include "NicaTwoTrackCut.h"

class NicaV0Candidate;
class TClonesArray;
/**
 * basic class for finding V0 particles
 * don't use AddCut AddCutMonitor mehtods here
 */
class NicaV0BasicFinder : public NicaTrackAna {
protected:
  Int_t fPosPdg;
  Int_t fNegPdg;
  Int_t fV0Pdg;
  Double_t fPosMass;
  Double_t fNegMass;
  Double_t fV0Mass;
  Bool_t fFirstV0Finder;
  Bool_t fWrite;
  NicaV0Candidate* fV0Candiate;
  TClonesArray* fV0Tracks;
  virtual void ProcessEvent();
  virtual InitStatus Init();
  virtual void LinkCollections();

public:
  NicaV0BasicFinder();
  void Write() { fWrite = kTRUE; };
  virtual void Exec(Option_t* option);
  /**
   * assumed PDS's
   * @param dau1 postive daughter
   * @param dau2 negative daughter
   * @param v0 v0 particle
   */
  void SetPDG(Int_t dau1, Int_t dau2, Int_t v0);
  /**
   * defines the V0 candidate that will be used for creation of V0
   * VO candidate is responsigle for finding minimal distance between
   * reconstructed particles, and momentum at this point (decay vertex)
   * @param vo
   */
  void SetV0Container(const NicaV0Candidate& vo);
  /**
   * set cut for negative dauther
   * @param dau_neg
   * @param opt use "fast" to create fast cut
   */
  virtual void AddNegDauCut(const NicaTrackCut& dau_neg, Option_t* opt = "");
  /**
   * set cut for positive daugher selection
   * @param pos_cut
   * @param opt user fast" to create fast cut
   */
  virtual void AddPosDauCut(const NicaTrackCut& pos_cut, Option_t* opt = "");
  /**
   * add cut monitor for negative dauther
   * @param mon
   * @param opt not used
   */
  virtual void AddNegDauCutMon(const NicaCutMonitor& mon, Option_t* opt = "");
  /**
   * add cut monitor for postive daughther
   * @param mon
   * @param opt not used
   */
  virtual void AddPosDauCutMon(const NicaCutMonitor& mon, Option_t* opt = "");
  /**
   * cut for V0 candidates
   * @param obj
   * @param opt can be "fast"
   */
  virtual void AddCandicateCut(const NicaTwoTrackCut& obj, Option_t* opt = "");
  /**
   * add cut monitor for V0 candidates
   * @param mon
   * @param opt not used
   */
  virtual void AddCandicateCutMon(const NicaCutMonitor& mon, Option_t* opt = "");
  virtual void AddCut(const NicaCut& /*cut*/, Option_t* /*opt=""*/) {};
  virtual void AddCutMonitor(const NicaCutMonitor& /*mon*/, Option_t* /*opt=""*/) {};
  virtual NicaPackage* Report() const;
  virtual ~NicaV0BasicFinder();
  ClassDef(NicaV0BasicFinder, 1)
};

#endif /* NICAV0BASICFINDER_H_ */
