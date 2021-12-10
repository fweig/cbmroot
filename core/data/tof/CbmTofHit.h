/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: E. Cordier, Andrey Lebedev, Florian Uhlig, Denis Bertini [committer], Pierre-Alain Loizeau */

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

#include <Rtypes.h>    // for ClassDef
#include <TVector3.h>  // for TVector3

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cstdint>
#include <string>  // for string

#include <cmath>

class CbmTofHit : public CbmPixelHit {
public:
  /**
   * \brief Default constructor.
   **/
  CbmTofHit();

  /**
   * \brief Constructor with hit parameters (1b).
   **/
  CbmTofHit(int32_t address, TVector3 pos, TVector3 dpos, int32_t refIndex, double time, double dtime, int32_t flag,
            int32_t channel);

  /**
   * \brief Constructor with hit parameters (1a).
   **/
  CbmTofHit(int32_t address, TVector3 pos, TVector3 dpos, int32_t refIndex, double tof, int32_t flag, int32_t channel);

  /**
   * \brief Constructor with hit parameters (1).
   **/
  CbmTofHit(int32_t address, TVector3 pos, TVector3 dpos, int32_t refIndex, double tof, int32_t flag);

  /**
   * \brief Constructor with hit parameters (2) [not the flag]
   **/
  CbmTofHit(int32_t address, TVector3 pos, TVector3 dpos, int32_t refIndex, double tof);

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
  int32_t GetPlaneId() const { return 0; }

  /** Accessors **/
  int32_t GetFlag() const { return fFlag; }
  int32_t GetCh() const { return fChannel; }

  double GetR() const { return sqrt(GetX() * GetX() + GetY() * GetY() + GetZ() * GetZ()); }
  double GetRt() const { return sqrt(GetX() * GetX() + GetY() * GetY()); }
  double GetCosThe() const { return GetZ() / GetR(); }
  double GetSinThe() const { return sqrt(GetX() * GetX() + GetY() * GetY()) / GetR(); }
  double GetCosPhi() const { return GetX() / GetRt(); }
  double GetSinPhi() const { return GetY() / GetRt(); }

  /** Modifiers **/
  void SetFlag(int32_t flag) { fFlag = flag; };

  // Make this method otherwise inherited from CbmHit through CbmPixelHit
  // private to prevent its usage
  int32_t GetRefId() const __attribute__((deprecated)) { return -1; }
  // Field is instead used to store the number of strips used to generate the hit
  int32_t GetClusterSize() { return CbmHit::GetRefId(); }

private:
  int32_t fFlag;     ///< Flag for general purposes [TDC, event tagging...]
  int32_t fChannel;  ///< Channel identifier

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
