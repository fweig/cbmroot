/** @file CbmRichRingFitterRobustCOP.h
  * @copyright Copyright (C) 2006-2012 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Alexander Ayriyan, Gennadi Ososkov, Claudia Hoehne, Semen Lebedev, Semen Lebedev, Denis Bertini [committer] **/

/**
* \file CbmRichRingFitterRobustCOP.h
*
* \brief Here the ring is fitted with the RobustCOP algorithm from A. Ayriyan/G. Ososkov.
*
* \author Alexander Ayriyan, Gennadi Ososkov, Claudia Hoehne, Semen Lebedev <s.lebedev@gsi.de>
* \date 2012
**/
#ifndef CBM_RICH_RING_FITTER_ROBUST_COP
#define CBM_RICH_RING_FITTER_ROBUST_COP

#include "CbmRichRingFitterBase.h"

/**
* \class CbmRichRingFitterRobustCOP
*
* \brief Here the ring is fitted with the RobustCOP algorithm from A. Ayriyan/G. Ososkov.
*
* \author Alexander Ayriyan, Gennadi Ososkov, Claudia Hoehne, Semen Lebedev <s.lebedev@gsi.de>
* \date 2012
**/
class CbmRichRingFitterRobustCOP : public CbmRichRingFitterBase {
public:
  /**
    * \brief Default constructor.
    */
  CbmRichRingFitterRobustCOP();

  /**
    * \brief Destructor.
    */
  virtual ~CbmRichRingFitterRobustCOP();

  /**
    * \brief Inherited from CbmRichRingFitterBase.
    */
  virtual void DoFit(CbmRichRingLight* ring);
};

#endif
