/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer], Pierre-Alain Loizeau */

#include "HitFinderTof.h"

// TOF Classes and includes
#include "CbmTofDigi.h"

// C++ Classes and includes
#include <iomanip>
#include <iostream>

namespace cbm::algo
{
  HitFinderTof::resultType HitFinderTof::operator()(std::vector<CbmTofDigi> digisIn,
                                                    const std::vector<int32_t>& digiIndexIn)
  {
    // Intermediate storage variables
    std::vector<std::vector<CbmTofDigi*>> digisExp;  //[nbCh][nDigis]
    std::vector<std::vector<int32_t>> digisInd;      //[nbCh][nDigis]

    digisExp.resize(fParams.fChanPar.size());
    digisInd.resize(fParams.fChanPar.size());

    {  // digi calibration
      const int32_t numClWalkBinX = fParams.numClWalkBinX;
      const double TOTMax         = fParams.TOTMax;
      const double TOTMin         = fParams.TOTMin;

      for (size_t iDigi = 0; iDigi < digisIn.size(); iDigi++) {
        CbmTofDigi* pDigi = &digisIn[iDigi];
        assert(pDigi);

        // These are doubles in the digi class
        const double chan   = pDigi->GetChannel();
        const double side   = pDigi->GetSide();
        const double charge = pDigi->GetTot() * fParams.fChanPar[chan].fvCPTotGain[side];  // calibrate Digi ToT
        const double time   = pDigi->GetTime() - fParams.fChanPar[chan].fvCPTOff[side];    // calibrate Digi Time

        digisExp[chan].push_back(pDigi);
        digisInd[chan].push_back(digiIndexIn[iDigi]);

        pDigi->SetTot(charge);

        {  // walk correction
          const double totBinSize = (TOTMax - TOTMin) / 2. / numClWalkBinX;
          int32_t iWx             = (int32_t)((charge - TOTMin / 2.) / totBinSize);
          if (0 > iWx) iWx = 0;
          if (iWx > (numClWalkBinX - 1)) iWx = numClWalkBinX - 1;

          std::vector<double>& cpWalk = fParams.fChanPar[chan].fvCPWalk[side];
          double dWT                  = cpWalk[iWx];
          const double dDTot          = (charge - TOTMin / 2.) / totBinSize - (double) iWx - 0.5;

          if (dDTot > 0) {
            if (iWx < numClWalkBinX - 1) {  // linear interpolation to next bin
              dWT += dDTot * (cpWalk[iWx + 1] - cpWalk[iWx]);
            }
          }
          else {
            if (0 < iWx) {  // linear interpolation to next bin
              dWT -= dDTot * (cpWalk[iWx - 1] - cpWalk[iWx]);
            }
          }
          pDigi->SetTime(time - dWT);
        }
      }
    }  // digi calibration

    // Hit variables
    TofCluster cluster;
    std::vector<TofCluster> clustersOut;

    // Reference cell of a cluster
    TofCell* trafoCell = nullptr;
    int32_t iTrafoCell = -1;

    // Last Channel Temp variables
    int32_t lastChan = -1;
    double lastPosY  = 0.0;
    double lastTime  = 0.0;

    //reset counter
    numSameSide = 0;

    for (int32_t chan = 0; chan < fParams.fChanPar.size(); chan++) {

      std::vector<CbmTofDigi*>& storDigiExp = digisExp[chan];
      std::vector<int32_t>& storDigiInd     = digisInd[chan];
      HitFinderTofChanPar& chanPar          = fParams.fChanPar[chan];

      auto digiExpIt = storDigiExp.begin();
      auto digiIndIt = storDigiInd.begin();

      while (1 < std::distance(digiExpIt, storDigiExp.end())) {
        while ((*digiExpIt)->GetSide() == (*std::next(digiExpIt, 1))->GetSide()) {
          // Not one Digi of each end!
          numSameSide++;
          digiExpIt++;
          digiIndIt++;
          if (2 > std::distance(digiExpIt, storDigiExp.end())) break;
        }
        if (2 > std::distance(digiExpIt, storDigiExp.end())) break;  // 2 Digis = both sides present

        TofCell* channelInfo = &chanPar.cell;
        CbmTofDigi* xDigiA   = *digiExpIt;
        CbmTofDigi* xDigiB   = *std::next(digiExpIt, 1);

        // The "Strip" time is the mean time between each end
        const double time = 0.5 * (xDigiA->GetTime() + xDigiB->GetTime());

        // Weight is the total charge => sum of both ends ToT
        const double totSum = xDigiA->GetTot() + xDigiB->GetTot();

        if (nullptr == trafoCell) {
          trafoCell  = channelInfo;
          iTrafoCell = chan;
        }

        // switch to local coordinates, (0,0,0) is in the center of counter  ?
        // It is assumed that the cell size is the same for all channels within one rpc!
        ROOT::Math::XYZVector pos(((double) (-iTrafoCell + chan)) * trafoCell->sizeX, 0., 0.);

        if (1 == xDigiA->GetSide()) {
          // 0 is the top side, 1 is the bottom side
          pos.SetY(fParams.CPSigPropSpeed * (xDigiA->GetTime() - xDigiB->GetTime()) / 2.0);
        }
        else {
          // 0 is the bottom side, 1 is the top side
          pos.SetY(fParams.CPSigPropSpeed * (xDigiB->GetTime() - xDigiA->GetTime()) / 2.0);
        }

        if (channelInfo->sizeY / 2.0 < pos.Y() || -1 * channelInfo->sizeY / 2.0 > pos.Y()) {
          // if outside of strip limits, the pair is bad => try to remove one end and check the next pair
          // (if another possibility exist)
          digiExpIt++;
          digiIndIt++;
          continue;
        }  // Pair leads to hit oustide of strip limits

        // Now check if a hit/cluster is already started
        if (0 < cluster.numChan()) {
          // a cluster is already started => check distance in space/time
          // For simplicity, just check along strip direction for now
          // and break cluster when a not fired strip is found
          if (!(std::abs(time - lastTime) < fParams.maxTimeDist && lastChan == chan - 1
                && std::abs(pos.Y() - lastPosY) < fParams.maxSpaceDist)) {
            // simple error scaling with TOT
            // weightedTimeErr = TMath::Sqrt( weightedTimeErr ) * SysTimeRes / weightsSum;
            // OR harcoded value: weightedTimeErr = SysTimeRes;
            cluster.normalize(fParams.SysTimeRes);

            // Save Hit and start a new one
            cluster.finalize(*trafoCell, iTrafoCell, fParams);
            clustersOut.push_back(cluster);
            cluster.reset();
          }
        }
        cluster.add(pos, time, totSum, totSum, *digiIndIt, *std::next(digiIndIt, 1));
        digiExpIt += 2;
        digiIndIt += 2;

        lastChan = chan;
        lastPosY = pos.Y();
        lastTime = time;
      }  // while (1 < storDigiExp.size())
    }    // for( int32_t chan = 0; chan < iNbCh; chan++ )

    // Save last cluster if it exists
    if (0 < cluster.numChan()) {
      cluster.normalize(fParams.SysTimeRes);
      cluster.finalize(*trafoCell, iTrafoCell, fParams);
      clustersOut.push_back(cluster);
    }

    return clustersOut;
  }
} /* namespace cbm::algo */
