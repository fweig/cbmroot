/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Andrey Lebedev */

/**
 * \file CbmPixelHit.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 *
 * Base class for pixel hits used for tracking in CBM.
 * Derives from CbmBaseHit.
 * Additional members are x, y coordinates and x, y, dxy covariances.
 **/
#ifndef CBMPIXELHIT_H_
#define CBMPIXELHIT_H_

#include "CbmHit.h"  // for CbmHit

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <TVector3.h>    // for TVector3

#include <cstdint>
#include <string>  // for string

class CbmPixelHit : public CbmHit {
public:
  /**
	 * \brief Default constructor.
	 */
  CbmPixelHit();

  /**
	 * \brief Standard constructor.
	 * \param[in] address Detector unique identifier.
	 * \param[in] x X position of the hit [cm].
	 * \param[in] y Y position of the hit [cm].
	 * \param[in] z Z position of the hit [cm].
	 * \param[in] dx X position error of the hit [cm].
	 * \param[in] dy Y position error of the hit [cm].
	 * \param[in] dz Z position error of the hit [cm].
	 * \param[in] dxy XY correlation of the hit.
	 * \param[in] refId Some reference ID.
         * \param[in] time Hit time [ns].   
         * \param[in] timeError Error of hit time [ns].         
 	 **/
  CbmPixelHit(int32_t address, double x, double y, double z, double dx, double dy, double dz, double dxy, int32_t refId,
              double time = -1., double timeError = -1.);

  /**
	 * \breif Standard constructor.
	 * \param address Detector unique identifier.
	 * \param pos Position of the hit as TVector3 [cm].
	 * \param err Position errors of the hit as TVector3 [cm].
	 * \param dxy XY correlation of the hit.
	 * \param refId Some reference ID.
         * \param[in] time Hit time [ns].   
         * \param[in] timeError Error of hit time [ns].         
	 **/
  CbmPixelHit(int32_t address, const TVector3& pos, const TVector3& err, double dxy, int32_t refId, double time = -1.,
              double timeError = -1.);

  /**
	 * brief Destructor.
	 */
  virtual ~CbmPixelHit();

  /**
	 * \brief Inherited from CbmBaseHit.
	 **/
  virtual std::string ToString() const;

  /* Accessors */
  double GetX() const { return fX; }
  double GetY() const { return fY; }
  double GetDx() const { return fDx; }
  double GetDy() const { return fDy; }
  double GetDxy() const { return fDxy; }

  /**
	 * \brief Copies hit position to pos.
	 * \param pos Output hit position.
	 */
  void Position(TVector3& pos) const;

  /**
	 * \brief Copies hit position error to pos.
	 * \param pos Output hit position error.
	 */
  void PositionError(TVector3& dpos) const;

  /* Setters */
  void SetX(double x) { fX = x; }
  void SetY(double y) { fY = y; }
  void SetDx(double dx) { fDx = dx; }
  void SetDy(double dy) { fDy = dy; }
  void SetDxy(double dxy) { fDxy = dxy; }

  /**
	 * \brief Sets position of the hit.
	 * \param pos new hit position.
	 **/
  void SetPosition(const TVector3& pos);

  /**
	 * \breif Sets position error of the hit.
	 * \param dpos new hit position error
	 **/
  void SetPositionError(const TVector3& dpos);

private:
  double fX, fY;    ///< X, Y positions of hit [cm]
  double fDx, fDy;  ///< X, Y errors [cm]
  double fDxy;      ///< XY correlation

  ClassDef(CbmPixelHit, 1);
};

#endif /* CBMPIXELHIT_H_ */
