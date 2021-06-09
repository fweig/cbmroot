/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Claudia Hoehne, Semen Lebedev, Denis Bertini [committer] */

/**
* \file CbmRichRingTrackAssignIdeal.h
*
* \brief Ideal Ring-Track Assignment.
* CbmRichRingMatch must be run prior to this procedure.
*
* \author Claudia Hoehne and Semen Lebedev
* \date 2007
**/


#ifndef CBM_RICH_RING_TRACK_ASSIGN_IDEAL
#define CBM_RICH_RING_TRACK_ASSIGN_IDEAL

#include "CbmRichRingTrackAssignBase.h"

/**
* \class CbmRichRingTrackAssignIdeal
*
* \brief Ideal Ring-Track Assignment.
* CbmRichRingMatch must be run prior to this procedure.
*
* \author Claudia Hoehne and Semen Lebedev
* \date 2007
**/
class CbmRichRingTrackAssignIdeal : public CbmRichRingTrackAssignBase {

public:
  /**
   * \brief Default constructor.
   */
  CbmRichRingTrackAssignIdeal();

  /**
   * \brief Destructor.
   */
  virtual ~CbmRichRingTrackAssignIdeal();

  /**
    * \brief Inherited from CbmRichRingTrackAssignBase.
    */
  void Init();

  /**
   * \brief Inherited from CbmRichRingTrackAssignBase.
   */
  virtual void DoAssign(CbmEvent* event, TClonesArray* rings, TClonesArray* richProj);

private:
  TClonesArray* fGlobalTracks    = nullptr;
  TClonesArray* fRingMatches     = nullptr;
  TClonesArray* fStsTrackMatches = nullptr;

  /**
    * \brief Copy constructor.
    */
  CbmRichRingTrackAssignIdeal(const CbmRichRingTrackAssignIdeal&);

  /**
    * \brief Assignment operator.
    */
  CbmRichRingTrackAssignIdeal& operator=(const CbmRichRingTrackAssignIdeal&);
};

#endif
