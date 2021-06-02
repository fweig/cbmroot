/** @file CbmRichProjectionProducerTGeo.h
  * @copyright Copyright (C) 2016-2021 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors S.Lebedev, Andrey Lebedev [committer] **/

/**
 * \file CbmRichProjectionProducerTGeo.h
 *
 * \brief Project track by straight line from imaginary plane
 * to the mirror and reflect it to the photodetector plane.
 *
 * \author S.Lebedev
 * \date 2016
 **/

#ifndef CBM_RICH_PROJECTION_PRODUCER_TGEO
#define CBM_RICH_PROJECTION_PRODUCER_TGEO
#include "CbmRichProjectionProducerBase.h"

#include "TObject.h"
#include "TVector3.h"

#include <string>

using namespace std;

class TClonesArray;
class TObjArray;
class FairTrackParam;

/**
 * \class CbmRichProjectionProducerTGeo
 *
 * \brief Project track by straight line from imaginary plane
 * to the mirror and reflect it to the photodetector plane.
 *
 * \author S.Lebedev
 * \date 2016
 **/
class CbmRichProjectionProducerTGeo : public CbmRichProjectionProducerBase {
public:
  /**
     * \brief Standard constructor.
     */
  CbmRichProjectionProducerTGeo();

  /**
     * \brief Destructor.
     */
  virtual ~CbmRichProjectionProducerTGeo();

  /**
     * \brief Initialization of the task.
     */
  virtual void Init();

  /**
     * \brief Execute task.
     * \param[out] richProj Output array of created projections.
     */
  virtual void DoProjection(CbmEvent* event, TClonesArray* richProj);


private:
  TClonesArray* fTrackParams = nullptr;
  int fEventNum              = 0;

  /**
     * \brief Copy constructor.
     */
  CbmRichProjectionProducerTGeo(const CbmRichProjectionProducerTGeo&);

  /**
     * \brief Assignment operator.
     */
  CbmRichProjectionProducerTGeo& operator=(const CbmRichProjectionProducerTGeo&);
};

#endif
