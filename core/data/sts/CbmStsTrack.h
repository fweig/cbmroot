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

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Double32_t

#include <cassert>  // for assert
#include <string>   // for string
#include <vector>   // for vector

#include "CbmHit.h"    // for kSTSHIT
#include "CbmTrack.h"  // for CbmTrack

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
  void AddMvdHit(Int_t hitIndex) { fMvdHitIndex.push_back(hitIndex); }


  /** Associate a StsHit to the track
    ** @param hitIndex  Index of the STS hit in TClonesArray
    **/
  void AddStsHit(Int_t hitIndex) { AddHit(hitIndex, kSTSHIT); }


  /** Impact parameter
    ** @return  Impact parameter at target z in units of error [cm]
    **/
  Double_t GetB() const { return fB; }


  /** Index of a MVD hit
    ** @return Array index of the ith MVD hit of the track
    **
    ** Throws std::vector exception if out of bounds.
    **/
  Int_t GetMvdHitIndex(Int_t iHit) const { return fMvdHitIndex.at(iHit); }


  /** Total number of hits
    ** @return  Sum of numbers of STS and MVD hits
    **/
  virtual Int_t GetNofHits() const {
    return (GetNofStsHits() + GetNofMvdHits());
  }


  /** Number of MVD hits
    ** @return  Number of MVD hits associated to the track
    **/
  Int_t GetNofMvdHits() const { return fMvdHitIndex.size(); }


  /** Number of STS hits
    ** @return  Number of STS hits associated to the track
    **/
  Int_t GetNofStsHits() const { return CbmTrack::GetNofHits(); }


  /** Index of a STS hit
    ** @value Array index of the ith STS hit of the track
    **
    ** Throws std::vector exception if out of bounds.
    **/
  Int_t GetStsHitIndex(Int_t iHit) const {
    assert(iHit < GetNofStsHits());
    return GetHitIndex(iHit);
  }


  /** Set the impact parameter
    ** @param  Impact parameter at target z in units of error [cm]
    **/
  void SetB(Double_t b) { fB = b; }


  /** Debug output **/
  virtual std::string ToString() const;


  /** Get energy loss
   ** @return  median energy loss
   **/
  Float_t GetELoss() const { return fELoss; }

  /** Set energy loss
   ** @param  median energy loss
   **/
  void SetELoss(Float_t ELoss) { fELoss = ELoss; }

  constexpr static Float_t ELossOverflow() { return 1.e6; }

private:
  /** Array with indices of the MVD hits attached to the track **/
  std::vector<Int_t> fMvdHitIndex;


  /** Impact parameter of track at target z, in units of its error **/
  Double32_t fB;

  /** median dE/dx [e/300µm] **/
  Float_t fELoss {-1.f};

  ClassDef(CbmStsTrack, 3);
};

#endif
