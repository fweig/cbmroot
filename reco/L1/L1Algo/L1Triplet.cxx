/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina, Sergey Gorbunov[committer] */

#include "L1Triplet.h"

#include <iostream>

void L1Triplet::Print()
{
  /// print the tracklet parameters
  std::cout << " Triplet: station L/M/R " << GetLSta() << "/" << GetMSta() << "/" << GetRSta() << "\n"
            << "          hit L/M/R " << fHitL << "/" << fHitM << "/" << fHitR << "\n"
            << "          level " << fLevel << " first neighbor " << fFirstNeighbour << " Nneighbors " << fNneighbours
            << "\n"
            << "          qp " << fQp << " Cqp " << fCqp << " chi2 " << fChi2 << "\n"
            << "          tx " << fTx << " Ctx " << fCtx << " ty " << fTy << " Cty " << fCty << std::endl;
}
