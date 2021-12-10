/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Andrey Lebedev */

/**
 * \file CbmStripHit.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 *
 * Base class for strip-like hits used for tracking in CBM.
 * Derives from CbmBaseHit.
 * Additional members are u coordinate, phi angle and du, dphi measurement errors.
 **/
#ifndef CBMSTRIPHIT_H_
#define CBMSTRIPHIT_H_

#include "CbmHit.h"  // for CbmHit

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef

#include <cstdint>
#include <string>  // for string

class TVector3;

class CbmStripHit : public CbmHit {
public:
  /**
	 * \brief Default constructor.
	 */
  CbmStripHit();

  /**
	 * \brief Standard constructor.
	 * \param[in] address detector unique identifier
	 * \param[in] u coordinate in the rotated c.s. [cm]
	 * \param[in] phi strip rotation angle [rad]
	 * \param[in] z Z position of the hit [cm]
	 * \param[in] du U measurement error [cm]
	 * \param[in] dphi PHI measurement error [rad]
	 * \param[in] z Z position of the hit [cm]
	 * \param[in] refId some reference ID
	 * \param[in] time Hit time [ns].
	 * \param[in] timeError Error of hit time [ns].

	 **/
  CbmStripHit(int32_t address, double u, double phi, double z, double du, double dphi, double dz, int32_t refId,
              double time = -1., double timeError = -1.);

  /**
	 * \brief Standard constructor.
	 * \param[in] address Detector unique identifier.
	 * \param[in] pos Position of the hit as TVector3 (u, phi, z) [cm].
	 * \param[in] err Position errors of the hit as TVector3 (du, dphi, dz) [cm].
	 * \param[in] refId Some reference ID.
	 * \param[in] time Hit time [ns].
	 * \param[in] timeError Error of hit time [ns].
	 **/
  CbmStripHit(int32_t address, const TVector3& pos, const TVector3& err, int32_t refId, double time = -1.,
              double timeError = -1.);

  /**
	 * \brief Destructor.
	 */
  virtual ~CbmStripHit();

  /**
	 * \brief Inherited from CbmBaseHit.
	 **/
  virtual std::string ToString() const;

  /* Accessors */
  double GetU() const { return fU; }
  double GetPhi() const { return fPhi; }
  double GetDu() const { return fDu; }
  double GetDphi() const { return fDphi; }

  /* Setters */
  void SetU(double u) { fU = u; }
  void SetPhi(double phi) { fPhi = phi; }
  void SetDu(double du) { fDu = du; }
  void SetDphi(double dphi) { fDphi = dphi; }

private:
  double fU;     ///< U coordinate in the rotated c.s [cm]
  double fDu;    ///< U error [cm]
  double fPhi;   ///< strip rotation angle [rad]
  double fDphi;  ///< strip rotation error [rad]

  ClassDef(CbmStripHit, 1);
};

#endif /* CBMSTRIPHIT_H_ */
