/* Copyright (C) 2006-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Matus Kalisky [committer], Florian Uhlig, Andrey Lebedev */

/**
 * \file CbmTrdHit.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 * \brief Class for hits in TRD detector.
 **/

#ifndef CBMTRDHIT_H_
#define CBMTRDHIT_H_

#include "CbmPixelHit.h"    // for CbmPixelHit
#include "CbmTrdAddress.h"  // for CbmTrdAddress

#include <Rtypes.h>      // for CLRBIT, SETBIT, TESTBIT, ClassDef
#include <RtypesCore.h>  // for Double32_t

#include <cstdint>
#include <string>  // for string

class TVector3;

/** \class CbmTrdHit
 * \brief  data class for a reconstructed Energy-4D measurement in the TRD
 * 
 * A hit in the TRD is one average position estimation for the localization of ionization and TR
 * processes produced by a charged particle crossing the active volume of the TRD.
 * The particle trajectory normal, 2D spatial coordinates, are provided by the read-out 
 * pad-plane geometry (as defined by fX and fY and covariance matrix fDx, fDy, fDxy) while the coordinate 
 * along trajectory (fZ, fDz) is the local position of the anode plane with uncertainty  depending on 
 * hit time profile and mis-alignment.   
 * Time reconstruction (fTime, fTimeError) depends on FEE calibration, hit time profile and 
 * systematic effects of hit topology.
 * Additionally the class provide also a minimal interface to underlying (digit) information for a 
 * context based estimation of energy reconstruction (fELoss) uncertainty.   
 */
class CbmTrdHit : public CbmPixelHit {
public:
  enum CbmTrdHitDef
  {
    kType = 0  ///< set type of pad layout
      ,
    kMaxType  ///< set type of pad on which the maximum charge is found
      ,
    kRowCross  ///< mark hit defined by 2 clusters
      ,
    kOvfl  ///< mark over-flow in the data
  };
  /**
	 * \brief Default constructor.
	 */
  CbmTrdHit();

  /**
	 * \brief Standard constructor.
	  *\param address Unique detector ID
	  *\param pos Position in global c.s. [cm]
    *\param dpos Errors of position in global c.s. [cm]
	  *\param dxy XY correlation of the hit
	  *\param refId Index of digi or cluster
	  *\param eLoss TR + dEdx
	  **/
  CbmTrdHit(int32_t address, const TVector3& pos, const TVector3& dpos, double dxy, int32_t refId, double eLoss,
            double time = 0., double timeError = 0.);

  /** \brief Destructor. */
  virtual ~CbmTrdHit();

  /** \brief Inherited from CbmBaseHit.*/
  int32_t GetPlaneId() const { return CbmTrdAddress::GetLayerId(GetAddress()); }

  /** \brief Inherited from CbmBaseHit.**/
  virtual std::string ToString() const;

  /** Accessors **/
  double GetELoss() const { return fELoss; }
  bool GetClassType() const { return TESTBIT(fDefine, kType); }
  bool GetMaxType() const { return TESTBIT(fDefine, kMaxType); }
  bool HasOverFlow() const { return TESTBIT(fDefine, kOvfl); }
  bool IsRowCross() const { return TESTBIT(fDefine, kRowCross); }
  bool IsUsed() const { return (GetRefId() < 0); }

  /** Setters **/
  void SetELoss(double loss) { fELoss = loss; }
  /** \brief Mark overflow in one or more digits which define the hit.*/
  void SetOverFlow(bool set = true) { set ? SETBIT(fDefine, kOvfl) : CLRBIT(fDefine, kOvfl); }
  /** \brief Mark hit reconstructed between pad rows.*/
  void SetRowCross(bool set = true) { set ? SETBIT(fDefine, kRowCross) : CLRBIT(fDefine, kRowCross); }
  /** \brief Type of pad layout used in reconstruction R[0], T[1]*/
  void SetClassType(bool set = true) { set ? SETBIT(fDefine, kType) : CLRBIT(fDefine, kType); }
  /** \brief Extra bool definition for the hit (e.g. the type of maximum for triangular pads).*/
  void SetMaxType(bool set = true) { set ? SETBIT(fDefine, kMaxType) : CLRBIT(fDefine, kMaxType); }

private:
  uint8_t fDefine;      // hit extra info
  int32_t fNeighborId;  // refId in case of row cross clusters
  Double32_t fELoss;    // Energy deposit due to TR + dEdx

  ClassDef(CbmTrdHit, 4);
};

#endif
