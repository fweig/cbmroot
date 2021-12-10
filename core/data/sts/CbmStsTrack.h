/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Denis Bertini [committer], Evgeny Lavrik */

/**
 ** \file CbmStsTrack.h
 ** \author V.Friese <v.friese@gsi.de>
 ** \since 28.08.06
 ** \date 07.09.15
 ** \brief Data class for STS tracks
 **
 ** Updated 25/06/2008 by R. Karabowicz.
 ** Updated 04/03/2014 by A. Lebedev <andrey.lebedev@gsi.de>
 ** Updated 10/06/2014 by V.Friese <v.friese@gsi.de>
 ** Updated 07/09/2015 by V.Friese <v.friese@gsi.de>
 **/


#ifndef CBMSTSTRACK_H
#define CBMSTSTRACK_H 1

#include "CbmHit.h"    // for kSTSHIT
#include "CbmTrack.h"  // for CbmTrack

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double32_t

#include <cassert>  // for assert
#include <cstdint>
#include <string>  // for string
#include <vector>  // for vector

/** @class CbmStsTrack.h
 ** @brief Data class with information on a STS local track
 ** @author V.Friese <v.friese@gsi.de>
 ** @date 07.09.15
 ** @version 2.0
 **
 ** The STS track is a collection of STS and MVD hits, together
 ** with the track parameters obtained by the track fit.
 **/
class CbmStsTrack : public CbmTrack {
public:
  /** Default constructor **/
  CbmStsTrack();


  /** Destructor **/
  virtual ~CbmStsTrack();


  /** Associate a MvdHit to the track
    ** @param hitIndex  Index of the MVD hit in TClonesArray
    **/
  void AddMvdHit(int32_t hitIndex) { fMvdHitIndex.push_back(hitIndex); }


  /** Associate a StsHit to the track
    ** @param hitIndex  Index of the STS hit in TClonesArray
    **/
  void AddStsHit(int32_t hitIndex) { AddHit(hitIndex, kSTSHIT); }


  /** Impact parameter
    ** @return  Impact parameter at target z in units of error [cm]
    **/
  double GetB() const { return fB; }


  /** Index of a MVD hit
    ** @return Array index of the ith MVD hit of the track
    **
    ** Throws std::vector exception if out of bounds.
    **/
  int32_t GetMvdHitIndex(int32_t iHit) const { return fMvdHitIndex.at(iHit); }


  /** Total number of hits
    ** @return  Sum of numbers of STS and MVD hits
    **/
  virtual int32_t GetNofHits() const { return (GetNofStsHits() + GetNofMvdHits()); }


  /** Number of MVD hits
    ** @return  Number of MVD hits associated to the track
    **/
  int32_t GetNofMvdHits() const { return fMvdHitIndex.size(); }


  /** Number of STS hits
    ** @return  Number of STS hits associated to the track
    **/
  int32_t GetNofStsHits() const { return CbmTrack::GetNofHits(); }


  /** Index of a STS hit
    ** @value Array index of the ith STS hit of the track
    **
    ** Throws std::vector exception if out of bounds.
    **/
  int32_t GetStsHitIndex(int32_t iHit) const
  {
    assert(iHit < GetNofStsHits());
    return GetHitIndex(iHit);
  }


  /** Set the impact parameter
    ** @param  Impact parameter at target z in units of error [cm]
    **/
  void SetB(double b) { fB = b; }


  /** Debug output **/
  virtual std::string ToString() const;


  /** Get energy loss
   ** @return  median energy loss
   **/
  float GetELoss() const { return fELoss; }

  /** Set energy loss
   ** @param  median energy loss
   **/
  void SetELoss(float ELoss) { fELoss = ELoss; }

  constexpr static float ELossOverflow() { return 1.e6; }

private:
  /** Array with indices of the MVD hits attached to the track **/
  std::vector<int32_t> fMvdHitIndex;


  /** Impact parameter of track at target z, in units of its error **/
  Double32_t fB;

  /** median dE/dx [e/300µm] **/
  float fELoss {-1.f};

  ClassDef(CbmStsTrack, 3);
};

#endif
