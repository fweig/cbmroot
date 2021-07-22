/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina, Sergey Gorbunov[committer] */

#ifndef L1Triplet_h
#define L1Triplet_h

// @file L1Triplet.h
// @author Sergey Gorbunov
// @author Valentina Akishina
// @date 2021-05-18

#include "L1Def.h"

/// L1Triplet class represents a short 3-hit track segment called a "triplet".
///
class L1Triplet {

public:
  /// default constructor
  L1Triplet() = default;

  /// constructor
  L1Triplet(unsigned int iHitL, unsigned int iHitM, unsigned int iHitR, unsigned int iStaL, unsigned int iStaM,
            unsigned int iStaR, unsigned char Level, unsigned int firstNeighbour, char nNeighbours, fscal Chi2,
            fscal Qp, fscal Cqp, fscal tx, fscal Ctx, fscal ty, fscal Cty)
    : fChi2(Chi2)
    , fQp(Qp)
    , fCqp(Cqp)
    , fTx(tx)
    , fCtx(Ctx)
    , fTy(ty)
    , fCty(Cty)
    , fFirstNeighbour(firstNeighbour)
    , fHitL(iHitL)
    , fHitM(iHitM)
    , fHitR(iHitR)
    , fNneighbours(nNeighbours)
    , fLevel(Level)
    , fSta((iStaL << 4) + ((iStaM - iStaL - 1) << 2) + (iStaR - iStaL - 2))
  {
  }

  /// Setters and getters

  void SetLevel(unsigned char Level) { fLevel = Level; }
  unsigned char GetLevel() const { return fLevel; }

  THitI GetLHit() const { return fHitL; }
  THitI GetMHit() const { return fHitM; }
  THitI GetRHit() const { return fHitR; }

  void SetNNeighbours(char n) { fNneighbours = n; }
  char GetNNeighbours() const { return fNneighbours; }

  void SetFNeighbour(unsigned int n) { fFirstNeighbour = n; }
  unsigned int GetFNeighbour() const { return fFirstNeighbour; }

  fscal GetQp() const { return fQp; }
  fscal GetChi2() const { return fChi2; }
  fscal GetTime() const { return -111.; }

  int GetLSta() const { return fSta >> 4; }
  int GetMSta() const { return ((fSta % 16) >> 2) + GetLSta() + 1; }
  int GetRSta() const { return (fSta % 4) + GetLSta() + 2; }

  fscal GetCqp() const { return fCqp; }
  fscal GetTx() const { return fTx; }
  fscal GetCtx() const { return fCtx; }
  fscal GetTy() const { return fTy; }
  fscal GetCty() const { return fCty; }

  /// pack station, thread and triplet indices to an unique triplet ID
  static unsigned int PackTripletID(unsigned int Station, unsigned int Thread, unsigned int Triplet)
  {
    return Station * 100000000 + Thread * 1000000 + Triplet;
  }

  /// unpack the triplet ID to its station, thread, triplet index
  static void UnpackTripletID(unsigned int ID, unsigned int& Station, unsigned int& Thread, unsigned int& Triplet)
  {
    Station = ID / 100000000;
    Thread  = (ID - Station * 100000000) / 1000000;
    Triplet = (ID - Station * 100000000 - Thread * 1000000);
  }

  /// print the tracklet parameters
  void Print();

private:
  fscal fChi2 = 0.f;  // chi^2
  fscal fQp   = 0.f;  // q/p
  fscal fCqp  = 0.f;  // RMS of q/p
  fscal fTx   = 0.f;  // tx at the left hit
  fscal fCtx  = 0.f;  // RMS of tx
  fscal fTy   = 0.f;  // ty at the left hit
  fscal fCty  = 0.f;  // RMS of ty

  unsigned int fFirstNeighbour = 0;  // ID of the first neighbouring triplet
  THitI fHitL                  = 0;  // left hit index (16b) in vStsHits array
  THitI fHitM                  = 0;  // middle hit index (16b)
  THitI fHitR                  = 0;  // right hit index (16b)
  char fNneighbours            = 0;  // n of neighbouring triplets
  unsigned char fLevel         = 0;  // triplet level
                                     // == its possible position on the longest track candidate it belongs to.
                                     // level 0 = rightmost triplet of a track candidate
                                     // level k = k-ths triplet along the track counting upstream, from right to left.
  unsigned char fSta = 0;            // packed station numbers: staL (4b), staM-1-staL (2b), staR-2-staL (2b)
};

#endif
