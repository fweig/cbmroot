/** @file CbmRichRingFitterCircle.h
  * @copyright Copyright (C) 2006-2012 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Supriya Das, Semen Lebedev, Denis Bertini [committer] **/

/**
* \file CbmRichRingFitterCircle.h
*
* \brief Implementation of a ring fitting algorithm with equation of a circle.
*  Algorithm from F77 subroutine of S.Sadovsky.
*
* \author Supriya Das
* \date 2006
**/

#ifndef CBM_RICH_RING_FITTER_CIRCLE
#define CBM_RICH_RING_FITTER_CIRCLE

#include "CbmRichRingFitterBase.h"

/**
* \class CbmRichRingFitterCircle
*
* \brief Implementation of a ring fitting algorithm with equation of a circle.
*  Algorithm from F77 subroutine of S.Sadovsky.
*
* \author Supriya Das
* \date 2006
**/
class CbmRichRingFitterCircle : public CbmRichRingFitterBase {
public:
  /**
    * \brief Default constructor.
    */
  CbmRichRingFitterCircle();

  /**
    * \brief Destructor.
    */
  virtual ~CbmRichRingFitterCircle();

  /**
    * \brief Inherited from CbmRichRingFitterBase.
    */
  void DoFit(CbmRichRingLight* ring);
};

#endif
