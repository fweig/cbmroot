/*
 * CbmRichGeoManager.h
 *
 *  Created on: Dec 16, 2015
 *      Author: slebedev
 */

#include "CbmRichRecGeoPar.h"

#include "TVector3.h"

#ifndef RICH_CBMRICHGEOMANAGER_H_
#define RICH_CBMRICHGEOMANAGER_H_

#include <RtypesCore.h>  // for Bool_t, Double_t

class CbmRichRecGeoPar;
class TVector3;

class CbmRichGeoManager {

private:
public:
  CbmRichRecGeoPar* fGP;

public:
  /**
     * Return Instance of CbmRichGeoManager.
     */
  static CbmRichGeoManager& GetInstance()
  {
    static CbmRichGeoManager fInstance;
    return fInstance;
  }

  void DetectGeometryType();

  /**
     * Rotate points by
     * -theta, -phi for x>0, y>0
     * theta, -phi for x>0, y<0
     * theta,  phi for x<0, y<0
     * -theta,  phi for x<0, y>0
     * and shift x position in order to avoid overlap.
     * \param[in] inPos points position to be tilted.
     * \param[out] outPos point position after tilting.
     * \param[in] noTilting If you do not want to make tilting, needed for convenience.
     */
  void RotatePoint(TVector3* inPos, TVector3* outPos, Bool_t noTilting = false);


  void RotatePointCyl(TVector3* inPos, TVector3* outPos, Bool_t noTilting = false, Bool_t noShift = false);

  void RotatePointTwoWings(TVector3* inPos, TVector3* outPos, Bool_t noTilting = false);

  void RotatePointImpl(TVector3* inPos, TVector3* outPos, Double_t phi, Double_t theta, Double_t pmtX, Double_t pmtY,
                       Double_t pmtZ);

  /**
     * Check if point is inside PMT plane +-margin.
     *
     * \param[in] rotatedPoint Input point should be already rotated.
     */

  Bool_t IsPointInsidePmt(const TVector3* rotatedPoint);

private:
  /**
     * private Constructor.
     */
  CbmRichGeoManager();

  /**
     * Init geometry.
     */
  void InitGeometry();

  void InitMirror();

  void InitPmt();

  void InitPmtCyl();

  /**
     * \brief Copy constructor.
     */
  CbmRichGeoManager(const CbmRichGeoManager&);

  /**
     * \brief Assignment operator.
     */
  CbmRichGeoManager& operator=(const CbmRichGeoManager&);
};

#endif /* RICH_CBMRICHGEOMANAGER_H_ */
