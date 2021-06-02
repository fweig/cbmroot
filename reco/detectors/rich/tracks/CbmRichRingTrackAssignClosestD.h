/** @file CbmRichRingTrackAssignClosestD.h
  * @copyright Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Claudia Hoehne and Semen Lebedev, Semen Lebedev, Denis Bertini [committer], Semen Lebedev **/

/**
* \file CbmRichRingTrackAssignClosestD.h
*
* \brief Ring-Track Assignment according to the closest distance criterion.
*
* \author Claudia Hoehne and Semen Lebedev
* \date 2007
**/

#ifndef CBM_RICH_RING_TRACK_ASSIGN_CLOSEST_D
#define CBM_RICH_RING_TRACK_ASSIGN_CLOSEST_D

#include "CbmRichRingTrackAssignBase.h"

class TClonesArray;

enum CbmRichRingTrackAssignClosestDAlgorithmEnum
{
  TrackRing,
  RingTrack,
  Combined
};

/**
* \class CbmRichRingTrackAssignClosestD
*
* \brief Ring-Track Assignment according to the closest distance criterion.
*
* \author Claudia Hoehne and Semen Lebedev
* \date 2007
**/
class CbmRichRingTrackAssignClosestD : public CbmRichRingTrackAssignBase {
public:
  /**
   * \brief Default constructor.
   */
  CbmRichRingTrackAssignClosestD();

  /**
	 * \brief Destructor.
	 */
  virtual ~CbmRichRingTrackAssignClosestD();

  /**
	 * \brief Inherited from CbmRichRingTrackAssignBase.
	 */
  void Init();

  /**
	 * \brief Inherited from CbmRichRingTrackAssignBase.
	 */
  void DoAssign(CbmEvent* event, TClonesArray* rings, TClonesArray* richProj);

  /**
	 * \brief Implementation of the ring-track version of the algorithm.
	 */
  void DoAssignRingTrack(CbmEvent* event, TClonesArray* rings, TClonesArray* richProj);

  /**
	 * \brief Implementation of the track-ring version of the algorithm.
	 */
  void DoAssignTrackRing(CbmEvent* event, TClonesArray* rings, TClonesArray* richProj);

private:
  TClonesArray* fGlobalTracks = nullptr;
  TClonesArray* fTrdTracks    = nullptr;

  double fTrdAnnCut = -0.5;   // ANN cut for electron identification in TRD
  bool fUseTrd      = false;  // if true electron identification in TRD will be used
  CbmRichRingTrackAssignClosestDAlgorithmEnum fAlgorithmType = RingTrack;
  int fEventNum                                              = 0;

  /**
    * \brief Check if global track was identified as electron in the TRD detector.
    * \param[in] iTrack Index of global track.
    * \return true if track is identified as electron, else return false.
    */
  bool IsTrdElectron(int iTrack);

  /**
   * \brief Copy constructor.
   */
  CbmRichRingTrackAssignClosestD(const CbmRichRingTrackAssignClosestD&);

  /**
   * \brief Assignment operator.
   */
  CbmRichRingTrackAssignClosestD& operator=(const CbmRichRingTrackAssignClosestD&);
};

#endif
