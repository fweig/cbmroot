/*
 * NicaTrackFlagCuts.h
 *
 *  Created on: 2 sty 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICATRACKFLAGCUTS_H_
#define NICATRACKFLAGCUTS_H_

#include "NicaTrackCut.h"

class NicaTrack;

class NicaTrackPrimaryFlagCut : public NicaTrackCut {
public:
  NicaTrackPrimaryFlagCut() : NicaTrackCut(1) { SetUnitName("PrimaryFlag [AU]"); };
  static Int_t Primary() { return 1; };
  static Int_t NonPrimary() { return 0; };
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~NicaTrackPrimaryFlagCut() {};
  ClassDef(NicaTrackPrimaryFlagCut, 1)
};

class NicaTrackV0FlagCut : public NicaTrackCut {
public:
  NicaTrackV0FlagCut() : NicaTrackCut(1) { SetUnitName("V0Flag [AU]"); };
  static Int_t BadV0() { return 1; };
  static Int_t GoodV0() { return 2; };
  static Int_t NonV0() { return 0; }
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~NicaTrackV0FlagCut() {};
  ClassDef(NicaTrackV0FlagCut, 1)
};


class NicaTrackGoodSecondaryFlagCut : public NicaTrackCut {
public:
  NicaTrackGoodSecondaryFlagCut() : NicaTrackCut(1) { SetUnitName("MotherFlag [AU]"); };
  static Int_t HasMother() { return 1; };
  static Int_t IsOrphan() { return 0; }
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~NicaTrackGoodSecondaryFlagCut() {};
  ClassDef(NicaTrackGoodSecondaryFlagCut, 1)
};

class NicaTrackGlobalFlagCut : public NicaTrackCut {
public:
  NicaTrackGlobalFlagCut() : NicaTrackCut(1) { SetUnitName("GlobalFlag [AU]"); };
  static Int_t GlobalTrack() { return 1; }
  static Int_t NonGlobalTrack() { return 0; }
  virtual Bool_t Pass(NicaTrack* track);
  virtual ~NicaTrackGlobalFlagCut() {};
  ClassDef(NicaTrackGlobalFlagCut, 1)
};

#endif /* NICAFEMTO_CUTS_TRACKCUTS_NICATRACKFLAGCUTS_H_ */
