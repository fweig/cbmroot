/** @file CbmLmvmCuts.h
  * @copyright Copyright (C) 2015-2018 Justus-Liebig-Universitaet Giessen, Giessen
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Elena Lebedeva [committer], Gregor Pitsch **/

/**
 * @author Elena Lebedeva <e.lebedeva@gsi.de>
 * @since 2015
 * @version 1.0
 **/


#ifndef CBM_LMVM_CUTS_H
#define CBM_LMVM_CUTS_H

#include "TObject.h"

#include <iostream>

class CbmLmvmCuts {
public:
  CbmLmvmCuts()
    : fMomentumCut(0.)
    , fChiPrimCut(0.)
    , fPtCut(0.)
    , fAngleCut(0.)
    , fGammaCut(0.)
    , fStCutAngle(0.)
    , fStCutPP(0.)
    , fTtCutAngle(0.)
    , fTtCutPP(0.)
    , fRtCutAngle(0.)
    , fRtCutPP(0.)
    , fMvd1CutP(0.)
    , fMvd1CutD(0.)
    , fMvd2CutP(0.)
    , fMvd2CutD(0.)
  {
    SetDefaultCuts();
  }

  /*
	 * Set default electron ID and analysis cuts.
	 */
  void SetDefaultCuts()
  {
    //electron ID cuts, we use CbmLitGlobalElectronId for identification
    fMomentumCut = -1.;  // if cut < 0 it is not used

    // analysis cuts auau
    fPtCut      = 0.2;
    fAngleCut   = 1.;
    fChiPrimCut = 3.;
    fGammaCut   = 0.025;
    //fStCutAngle = 1.5;
    //fStCutPP = 1.5;
    fStCutAngle = 2.4;
    fStCutPP    = 1.;
    //fTtCutAngle = 0.75;
    //fTtCutPP = 4.0;
    fTtCutAngle = 1.5;
    fTtCutPP    = 1.7;
    //fRtCutAngle = 1.0;
    //fRtCutPP = 2.5;
    fRtCutAngle = 1.2;
    fRtCutPP    = 1.6;
    fMvd1CutP   = 1.2;
    fMvd1CutD   = 0.4;
    fMvd2CutP   = 1.5;
    fMvd2CutD   = 0.5;

    // analysis cuts agag
    /*    fPtCut = 0.2;
		fAngleCut = 1.;
		fChiPrimCut = 3.;
		fGammaCut = 0.025;
		fStCutAngle = 2.4;
                fStCutPP = 1.;
		fTtCutAngle = 1.5;
                fTtCutPP = 1.7;
	       	fRtCutAngle = 1.2;
                fRtCutPP = 1.6;
		fMvd1CutP = 1.2;
		fMvd1CutD = 0.4;
		fMvd2CutP = 1.5;
		fMvd2CutD = 0.5;
            */
  }

  /*
	 * Print out cuts.
	 */
  void Print()
  {
    std::cout << "Used cuts:" << std::endl
              << "fChiPrimCut = " << fChiPrimCut << std::endl
              << "fPtCut = " << fPtCut << std::endl
              << "fAngleCut = " << fAngleCut << std::endl
              << "fGammaCut = " << fGammaCut << std::endl
              << "fStCut (ang,pp) = (" << fStCutAngle << "," << fStCutPP << ")" << std::endl
              << "fRtCut (ang,pp) = (" << fRtCutAngle << "," << fRtCutPP << ")" << std::endl
              << "fTtCut (ang,pp) = (" << fTtCutAngle << "," << fTtCutPP << ")" << std::endl
              << "fMvd1Cut (p,d) = (" << fMvd1CutP << "," << fMvd1CutD << ")" << std::endl
              << "fMvd2Cut (p,d) = (" << fMvd2CutP << "," << fMvd2CutD << ")" << std::endl
              << "fMomentumCut = " << fMomentumCut << std::endl;
  }

public:
  // ID cuts, we use CbmLitGlobalElectronId for identification
  Double_t fMomentumCut;  // if cut < 0 then it will not be used

  // Analysis cuts
  Double_t fChiPrimCut;
  Double_t fPtCut;
  Double_t fAngleCut;
  Double_t fGammaCut;
  Double_t fStCutAngle;
  Double_t fStCutPP;
  Double_t fTtCutAngle;
  Double_t fTtCutPP;
  Double_t fRtCutAngle;
  Double_t fRtCutPP;
  Double_t fMvd1CutP;
  Double_t fMvd1CutD;
  Double_t fMvd2CutP;
  Double_t fMvd2CutD;
};

#endif
