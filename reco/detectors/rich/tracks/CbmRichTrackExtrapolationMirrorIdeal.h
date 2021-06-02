/** @file CbmRichTrackExtrapolationMirrorIdeal.h
  * @copyright Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Claudia Hoehne, Andrey Lebedev, Denis Bertini [committer], Semen Lebedev **/

/**
 * \file CbmRichTrackExtrapolationMirrorIdeal.h
 *
 * \brief This is the implementation of the TrackExtrapolation from MC points -
 * operating on points in the RICH mirror!
 * It reads the STS track array, gets the corresponding MC MirrorPoint
 * and selects those to be projected to the Rich Photodetector
 * points will be stored on mirror surface again.
 * WARNING!!!: ProjectionProducer needs to be run with zflag==2!!!
 *
 * \author Claudia Hoehne
 * \date 2006
 **/

#ifndef CBM_RICH_TARCK_EXTRAPOLATION_MIRROR_IDEAL
#define CBM_RICH_TARCK_EXTRAPOLATION_MIRROR_IDEAL

#include "CbmRichTrackExtrapolationBase.h"

class TClonesArray;

/**
 * \class CbmRichTrackExtrapolationMirrorIdeal
 *
 * \brief This is the implementation of the TrackExtrapolation from MC points -
 * operating on points in the RICH mirror!
 * It reads the STS track array, gets the corresponding MC MirrorPoint
 * and selects those to be projected to the Rich Photodetector
 * points will be stored on mirror surface again.
 * WARNING!!!: ProjectionProducer needs to be run with zflag==2!!!
 *
 * \author Claudia Hoehne
 * \date 2006
 **/
class CbmRichTrackExtrapolationMirrorIdeal : public CbmRichTrackExtrapolationBase {
public:
  /**
     * \brief Default constructor.
     */
  CbmRichTrackExtrapolationMirrorIdeal();

  /**
     * \brief Destructor.
     */
  virtual ~CbmRichTrackExtrapolationMirrorIdeal();

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
  TClonesArray* fRichMirrorPoints = nullptr;
  TClonesArray* fMcTracks         = nullptr;
  TClonesArray* fStsTracks        = nullptr;
  TClonesArray* fStsTrackMatches  = nullptr;

  /**
     * \brief Copy constructor.
     */
  CbmRichTrackExtrapolationMirrorIdeal(const CbmRichTrackExtrapolationMirrorIdeal&);

  /**
     * \brief Assignment operator.
     */
  CbmRichTrackExtrapolationMirrorIdeal& operator=(const CbmRichTrackExtrapolationMirrorIdeal&);
};

#endif
