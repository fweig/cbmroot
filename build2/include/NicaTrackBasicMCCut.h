/*
 * NicaTrackBasicMCCut.h
 *
 *  Created on: 19 maj 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKBASICMCCUT_H_
#define NICATRACKBASICMCCUT_H_

#include "NicaTrackMCCut.h"
/**
 * track for selection of tracks with only one cut
 */
class NicaTrackBasicMCCut : public NicaTrackMCCut {
  static const int fgStatusId;
  static const int fgPdgId;
  static const int fgPtId;
  static const int fgEtaId;

 public:
  /**
   * default constructor
   */
  NicaTrackBasicMCCut();
  /**
   * set cut on status
   * @param stat status cut value
   */
  void SetStatusCut(Int_t stat);
  /**
   * set pdg cut
   * @param pdg pdg cut value
   */
  void SetPdgCut(Int_t pdg);
  /**
   * set pt cut
   * @param min minimal value of pt
   * @param max maximal value of pt
   */
  void SetPtCut(Double_t min, Double_t max);
  /**
   * set pseudorapidity cut
   * @param min mimal value of eta
   * @param max maximal value of eta
   */
  void SetEtaCut(Double_t min, Double_t max);
  /**
   *
   * @return id of status parameter
   */
  static Int_t Status() { return fgStatusId; };
  /**
   *
   * @return id of pdg parameter
   */
  static Int_t Pdg() { return fgPdgId; };
  /**
   *
   * @return id of pt parameter
   */
  static Int_t Pt() { return fgPtId; };
  /**
   *
   * @return id of eta parameter
   */
  static Int_t Eta() { return fgEtaId; };
  Bool_t Pass(NicaTrack *track);
  virtual ~NicaTrackBasicMCCut();
  ClassDef(NicaTrackBasicMCCut, 1)
};

#endif /* NICATRACKBASICMCCUT_H_ */
