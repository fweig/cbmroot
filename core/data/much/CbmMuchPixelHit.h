/* Copyright (C) 2009-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Mikhail Ryzhinskiy */

/**
 * \file CbmMuchPixelHit.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 * \brief Class for pixel hits in MUCH detector.
 **/
#ifndef CBMMUCHPIXELHIT_H_
#define CBMMUCHPIXELHIT_H_

#include "CbmPixelHit.h"  // for CbmPixelHit

#include <Rtypes.h>      // for ClassDef

#include <cstdint>

class TVector3;

class CbmMuchPixelHit : public CbmPixelHit {
public:
  /**
    * \brief Default constructor.
    */
  CbmMuchPixelHit();

  /**
	 * Standard constructor.
	 * \param address detector unique identifier
	 * \param x X pposition of the hit [cm]
	 * \param y Y position of the hit [cm]
	 * \param z Z position of the hit [cm]
	 * \param dx X position error of the hit [cm]
	 * \param dy Y position error of the hit [cm]
	 * \param dz Z position error of the hit [cm]
	 * \param dxy XY correlation of the hit
	 * \param refId some reference ID
	 * \param planeId detector plane identifier
	 **/
  CbmMuchPixelHit(int32_t address, double x, double y, double z, double dx, double dy, double dz, double dxy,
                  int32_t refId, int32_t planeId, double time, double dtime);

  /**
	 * \brief Standard constructor
	 * \param address  Unique detector ID (including module number)
	 * \param pos Position in global c.s. [cm]
    * \param err Errors of position in global c.s. [cm]
	 * \param dxy XY correlation of the hit
	 * \param refId Index of digi or cluster
	 * \param planeId Detector plane identifier
	 **/
  CbmMuchPixelHit(int32_t address, const TVector3& pos, const TVector3& err, double dxy, int32_t refId,
                  int32_t planeId);

  /** Standard constructor
	  *\param address     Unique detector ID (including module number)
	  *\param pos       Position in global c.s. [cm]
	  *\param err       Errors of position in global c.s. [cm]
	  *\param dxy       Covariance of x and y
	  *\param refId     index of digi or cluster
	  *\param planeId   detectror plane identifier
	  *\param time      Time since event start [ns]
	  *\param dTime     Time resolution [ns]
	**/
  CbmMuchPixelHit(int32_t address, const TVector3& pos, const TVector3& err, double dxy, int32_t refId, int32_t planeId,
                  double time, double dtime);

  /**
	 * \brief Destructor.
	 */
  virtual ~CbmMuchPixelHit();

  /**
	 * \brief Inherited from CbmBaseHit.
	 */
  virtual int32_t GetPlaneId() const { return fPlaneId; }

  /** Accessors **/
  int32_t GetFlag() const { return fFlag; }


  /** Modifiers **/
  void SetFlag(int32_t flag) { fFlag = flag; }

private:
  int32_t fPlaneId;  // Plane number
  int32_t fFlag;     // Flag

  ClassDef(CbmMuchPixelHit, 3);
};

#endif /* CBMMUCHPIXELHIT_H_ */
