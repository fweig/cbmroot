/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Denis Bertini [committer], Volker Friese, Florian Uhlig */

// -------------------------------------------------------------------------
// -----                      CbmMCTrack header file                   -----
// -----                  Created 03/08/04  by V. Friese               -----
// -------------------------------------------------------------------------


/** CbmMCTrack.h
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Data class for storing Monte Carlo tracks processed by the CbmStack.
 ** A MCTrack can be a primary track put into the simulation or a
 ** secondary one produced by the transport through decay or interaction.
 **
 ** Redesign 13/06/07 by V. Friese
 **/


#ifndef CBMMCTRACK_H
#define CBMMCTRACK_H 1

#include "CbmDefs.h"  // for ECbmModuleId, ECbmModuleId::kMvd, ECbmModuleId::kPsd, ECbmModuleId::kSts

#include <Rtypes.h>          // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>      // for Double_t, Int_t, Double32_t, UInt_t
#include <TLorentzVector.h>  // for TLorentzVector
#include <TMath.h>           // for Sqrt
#include <TObject.h>         // for TObject
#include <TVector3.h>        // for TVector3

#include <string>  // for string

class TParticle;

class CbmMCTrack : public TObject {

public:
  /**  Default constructor  **/
  CbmMCTrack();


  /**  Standard constructor  **/
  CbmMCTrack(Int_t pdgCode, Int_t motherID, Double_t px, Double_t py, Double_t pz, Double_t x, Double_t y, Double_t z,
             Double_t t, Int_t nPoints);

  /**  Copy constructor  **/
  CbmMCTrack(const CbmMCTrack& track);


  /**  Constructor from TParticle  **/
  CbmMCTrack(TParticle* particle);


  /**  Destructor  **/
  virtual ~CbmMCTrack();

  Int_t AccMVD() const { return GetNPoints(ECbmModuleId::kMvd); }
  Int_t AccSTS() const { return GetNPoints(ECbmModuleId::kSts); }
  Int_t AccPSD() const { return GetNPoints(ECbmModuleId::kPsd); }

  /**  Accessors  **/
  UInt_t GetGeantProcessId() const { return fProcessId; }
  Int_t GetPdgCode() const { return fPdgCode; }
  Int_t GetMotherId() const { return fMotherId; }
  Double_t GetPx() const { return fPx; }
  Double_t GetPy() const { return fPy; }
  Double_t GetPz() const { return fPz; }
  Double_t GetStartX() const { return fStartX; }
  Double_t GetStartY() const { return fStartY; }
  Double_t GetStartZ() const { return fStartZ; }
  Double_t GetStartT() const { return fStartT; }

  /** @brief Mass of the associated particle
   ** @return Particle mass [GeV]
   **
   ** The mass is taken from TDatabasePDG if the particle exists there.
   ** For ions, the mass is calculated from the PDG code. The method
   ** throws an error for unknown PDG codes.
   **/
  Double_t GetMass() const;

  /** @brief Charge of the associated particle
   ** @return Particle charge [e]
   **
   ** The charge is taken from TDatabasePDG if the particle exists there.
   ** For ions, the charge is calculated from the PDG code. The method
   ** throws an error for unknown PDG codes.
   **/
  Double_t GetCharge() const;

  Double_t GetEnergy() const;
  Double_t GetPt() const { return TMath::Sqrt(fPx * fPx + fPy * fPy); }
  Double_t GetP() const { return TMath::Sqrt(fPx * fPx + fPy * fPy + fPz * fPz); }
  Double_t GetRapidity() const;
  void GetMomentum(TVector3& momentum) const;
  void Get4Momentum(TLorentzVector& momentum) const;
  void GetStartVertex(TVector3& vertex) const;


  /** Accessors to the number of MCPoints in the detectors **/
  Int_t GetNPoints(ECbmModuleId detId) const;


  /**  Modifiers  **/
  void SetMotherId(Int_t id) { fMotherId = id; }
  void SetNPoints(ECbmModuleId iDet, Int_t np);


  /** String output **/
  std::string ToString() const;


private:
  /**  TMCProcess code  **/
  UInt_t fProcessId;

  /**  PDG particle code  **/
  Int_t fPdgCode;

  /**  Index of mother track. -1 for primary particles.  **/
  Int_t fMotherId;

  /** Momentum components at start vertex [GeV]  **/
  Double32_t fPx, fPy, fPz;

  /** energy at start vertex [GeV]  **/
  Double32_t fE;

  /** Coordinates of start vertex [cm, ns]  **/
  Double32_t fStartX, fStartY, fStartZ, fStartT;

  /**  Bitvector representing the number of MCPoints for this track in 
   **  each subdetector. The detectors are represented by
   **  REF:         Bit  0      (1 bit,  max. value  1)
   **  MVD:         Bit  1 -  3 (3 bits, max. value  7)
   **  STS:         Bit  4 -  8 (5 bits, max. value 31)
   **  RICH:        Bit  9      (1 bit,  max. value  1)
   **  MUCH:        Bit 10 - 14 (5 bits, max. value 31)
   **  TRD:         Bit 15 - 19 (5 bits, max. value 31)
   **  TOF:         Bit 20 - 23 (4 bits, max. value 15)
   **  ECAL:        Bit 24      (1 bit,  max. value  1)
   **  ZDC:         Bit 25      (1 bit,  max. value  1)
   **  The respective point numbers can be accessed and modified 
   **  with the inline functions. 
   **  Bits 26-31 are spare for potential additional detectors.
   **/
  Int_t fNPoints;


  ClassDef(CbmMCTrack, 3)
};


// ==========   Inline functions   ========================================

inline Double_t CbmMCTrack::GetEnergy() const
{
  if (fE > 0.) return fE;
  Double_t mass = GetMass();
  return TMath::Sqrt(mass * mass + fPx * fPx + fPy * fPy + fPz * fPz);
}


inline void CbmMCTrack::GetMomentum(TVector3& momentum) const { momentum.SetXYZ(fPx, fPy, fPz); }


inline void CbmMCTrack::Get4Momentum(TLorentzVector& momentum) const { momentum.SetXYZT(fPx, fPy, fPz, GetEnergy()); }


inline void CbmMCTrack::GetStartVertex(TVector3& vertex) const { vertex.SetXYZ(fStartX, fStartY, fStartZ); }


#endif
