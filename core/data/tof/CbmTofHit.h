/** @file CbmTofHit.h
  * @copyright Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors E. Cordier, Andrey Lebedev, Florian Uhlig, Denis Bertini [committer], Pierre-Alain Loizeau **/

/**
 * \file CbmTofHit.h
 * \author E. Cordier
 * Modified by D. Gonzalez-Diaz 06/09/06
 * Modified by A.Lebedev 26/05/09
 * Modified by nh 16/12/12
 * Modified by A.Lebedev 15/05/13
**/

#ifndef CBMTOFHIT_H_
#define CBMTOFHIT_H_

#include "CbmHit.h"       // for CbmHit
#include "CbmPixelHit.h"  // for CbmPixelHit

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t
#include <TMath.h>       // for Sqrt
#include <TVector3.h>    // for TVector3

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <string>  // for string

class CbmTofHit : public CbmPixelHit {
public:
  /**
   * \brief Default constructor.
   **/
  CbmTofHit();

  /**
   * \brief Constructor with hit parameters (1b).
   **/
  CbmTofHit(Int_t address, TVector3 pos, TVector3 dpos, Int_t refIndex, Double_t time, Double_t dtime, Int_t flag,
            Int_t channel);

  /**
   * \brief Constructor with hit parameters (1a).
   **/
  CbmTofHit(Int_t address, TVector3 pos, TVector3 dpos, Int_t refIndex, Double_t tof, Int_t flag, Int_t channel);

  /**
   * \brief Constructor with hit parameters (1).
   **/
  CbmTofHit(Int_t address, TVector3 pos, TVector3 dpos, Int_t refIndex, Double_t tof, Int_t flag);

  /**
   * \brief Constructor with hit parameters (2) [not the flag]
   **/
  CbmTofHit(Int_t address, TVector3 pos, TVector3 dpos, Int_t refIndex, Double_t tof);

  /**
   * \brief Destructor.
   **/
  virtual ~CbmTofHit();

  /**
   * \brief Inherited from CbmBaseHit.
   */
  virtual std::string ToString() const;

  /**
   * \brief Inherited from CbmBaseHit.
   */
  Int_t GetPlaneId() const { return 0; }

  /** Accessors **/
  Int_t GetFlag() const { return fFlag; }
  Int_t GetCh() const { return fChannel; }

  Double_t GetR() const { return TMath::Sqrt(GetX() * GetX() + GetY() * GetY() + GetZ() * GetZ()); }
  Double_t GetRt() const { return TMath::Sqrt(GetX() * GetX() + GetY() * GetY()); }
  Double_t GetCosThe() const { return GetZ() / GetR(); }
  Double_t GetSinThe() const { return TMath::Sqrt(GetX() * GetX() + GetY() * GetY()) / GetR(); }
  Double_t GetCosPhi() const { return GetX() / GetRt(); }
  Double_t GetSinPhi() const { return GetY() / GetRt(); }

  /** Modifiers **/
  void SetFlag(Int_t flag) { fFlag = flag; };

  // Make this method otherwise inherited from CbmHit through CbmPixelHit
  // private to prevent its usage
  Int_t GetRefId() const __attribute__((deprecated)) { return -1; }
  // Field is instead used to store the number of strips used to generate the hit
  Int_t GetClusterSize() { return CbmHit::GetRefId(); }

private:
  Int_t fFlag;     ///< Flag for general purposes [TDC, event tagging...]
  Int_t fChannel;  ///< Channel identifier

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fFlag;
    ar& fChannel;
  }

  ClassDef(CbmTofHit, 5)
};

#endif
