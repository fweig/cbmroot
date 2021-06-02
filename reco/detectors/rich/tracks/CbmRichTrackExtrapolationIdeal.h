/** @file CbmRichTrackExtrapolationIdeal.h
  * @copyright Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Claudia Hoehne, Semen Lebedev, Denis Bertini [committer] **/

/**
* \file CbmRichTrackExtrapolationIdeal.h
*
* \brief This is the implementation of the TrackExtrapolation from MC points.
* It reads the STS track array, gets the corresponding MC RefPlanePoint
* and selects those to be projected to the Rich Photodetector.
*
* \author Claudia Hoehne
* \date 2006
**/

#ifndef CBM_RICH_TRACK_EXTRAPOLATION_IDEAL
#define CBM_RICH_TRACK_EXTRAPOLATION_IDEAL

#include "CbmRichTrackExtrapolationBase.h"

//class TClonesArray;

/**
* \class CbmRichTrackExtrapolationIdeal
*
* \brief "TrackExtrapolation" from MC points. It reads the PointArray with ImPlanePoints
* from MC and selects those to be projected to the Rich Photodetector.
*
* \author Claudia Hoehne
* \date 2006
**/
class CbmRichTrackExtrapolationIdeal : public CbmRichTrackExtrapolationBase {
public:
  /**
    * \brief Default constructor.
    */
  CbmRichTrackExtrapolationIdeal();

  /**
    * \brief Destructor.
    */
  virtual ~CbmRichTrackExtrapolationIdeal();

  /**
    * \brief Inherited from CbmRichTrackExtrapolationBase.
    */
  virtual void Init();


  /**
    * \brief Inherited from CbmRichTrackExtrapolationBase.
    */
  virtual void DoExtrapolation(CbmEvent* event, TClonesArray* globalTracks, TClonesArray* extrapolatedTrackParams,
                               double z);

private:
  TClonesArray* fRefPlanePoints  = nullptr;
  TClonesArray* fStsTracks       = nullptr;
  TClonesArray* fStsTrackMatches = nullptr;

  /**
    * \brief Copy constructor.
    */
  CbmRichTrackExtrapolationIdeal(const CbmRichTrackExtrapolationIdeal&);

  /**
    * \brief Assignment operator.
    */
  CbmRichTrackExtrapolationIdeal& operator=(const CbmRichTrackExtrapolationIdeal&);
};

#endif
