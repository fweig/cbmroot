/* Copyright (C) 2012-2020 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Jordan Bendarouach [committer] */

/**
* \file CbmRichProjectionProducerBase.h
*
* \brief Base class for STS track projections onto the photodetector plane.
*
* \author Semen Lebedev
* \date 2012
**/

#ifndef CBM_RICH_PROJECTION_PRODUCER_BASE
#define CBM_RICH_PROJECTION_PRODUCER_BASE

using std::string;
using namespace std;
#include "CbmRichMirrorMisalignmentCorrectionUtils.h"

class TClonesArray;

/**
* \class CbmRichProjectionProducerBase
*
* \brief Base class for STS track projections onto the photodetector plane.
*
* \author Semen Lebedev
* \date 2012
**/
class CbmRichProjectionProducerBase {
public:
  /**
    * brief Default constructor.
    */
  CbmRichProjectionProducerBase() : fPathToMirrorCorrectionParameterFile(""), fMirrorCorrectionParameterFile(NULL) {}

  /**
    * \brief Destructor.
    */
  virtual ~CbmRichProjectionProducerBase() {}


  /**
    * \brief Initialization in case one needs to initialize some TCloneArrays.
    */
  virtual void Init() {}

  /**
    * Creates track projections onto the photodetector plane.
    * \param[out] richProj Array of track projections onto the photodetector plane.
    **/
  virtual void DoProjection(TClonesArray* richProj) = 0;

  virtual void SetMirrorCorrectionParameterFile(const string& s) { fPathToMirrorCorrectionParameterFile = s; }


protected:
  string fPathToMirrorCorrectionParameterFile;
  CbmRichMirrorMisalignmentCorrectionUtils* fMirrorCorrectionParameterFile;

private:
  /**
    * \brief Copy constructor.
    */
  CbmRichProjectionProducerBase(const CbmRichProjectionProducerBase&);

  /**
    * \brief Assignment operator.
    */
  CbmRichProjectionProducerBase& operator=(const CbmRichProjectionProducerBase&);
};

#endif
