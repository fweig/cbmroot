/** @file CbmRichProjectionProducerAnalytical.h
  * @copyright Copyright (C) 2016-2021 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors S.Lebedev (initial version by P.Stolpovsky in 2005), Andrey Lebedev [committer], Semen Lebedev **/

/**
 * \file CbmRichProjectionProducerAnalytical.h
 *
 * \brief Project track by straight line from imaginary plane
 * to the mirror and reflect it to the photodetector plane.
 *
 * \author S.Lebedev (initial version by P.Stolpovsky in 2005)
 * \date 2016
 **/

#ifndef CBM_RICH_PROJECTION_PRODUCER_ANALYTICAL
#define CBM_RICH_PROJECTION_PRODUCER_ANALYTICAL
#include "CbmRichProjectionProducerBase.h"

#include "TObject.h"
#include "TVector3.h"

#include <string>

using namespace std;

class TClonesArray;
class TObjArray;
class FairTrackParam;

/**
 * \class CbmRichProjectionProducerAnalytical
 *
 * \brief Project track by straight line from imaginary plane
 * to the mirror and reflect it to the photodetector plane.
 *
 * \author S.Lebedev (initial version by P.Stolpovsky in 2005)
 * \date 2016
 **/
class CbmRichProjectionProducerAnalytical : public CbmRichProjectionProducerBase {
public:
  /**
     * \brief Standard constructor.
     */
  CbmRichProjectionProducerAnalytical();

  /**
     * \brief Destructor.
     */
  virtual ~CbmRichProjectionProducerAnalytical();

  /**
     * \brief Initialization of the task.
     */
  virtual void Init();

  /**
     * \brief Execute task.
     * \param[out] richProj Output array of created projections.
     */
  virtual void DoProjection(CbmEvent* event, TClonesArray* richProj);

  void GetPmtIntersectionPointTwoWings(const TVector3* centerP, const TVector3* crossP, const TVector3* ref,
                                       TVector3* outPoint);

  /*
     * Find the intersection point with cylindrical PMT plane.
     *
     */
  void GetPmtIntersectionPointCyl(const TVector3* centerP, const TVector3* crossP, const TVector3* ref,
                                  TVector3* outPoint);


private:
  TClonesArray* fTrackParams = nullptr;
  int fEventNum              = 0;

  /**
     * \brief Copy constructor.
     */
  CbmRichProjectionProducerAnalytical(const CbmRichProjectionProducerAnalytical&);

  /**
     * \brief Assignment operator.
     */
  CbmRichProjectionProducerAnalytical& operator=(const CbmRichProjectionProducerAnalytical&);
};

#endif
