/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dominik Smith [committer], Pierre-Alain Loizeau */

#ifndef HITFINDERTOF_H
#define HITFINDERTOF_H

// TOF Classes and includes
class CbmTofDigi;

// ROOT Classes and includes
#include "Math/Rotation3D.h"
#include "Math/Vector3Dfwd.h"

// C++ Classes and includes
#include <vector>

#include <cmath>

namespace cbm::algo
{
  struct TofCell {
    double sizeX, sizeY;
    ROOT::Math::XYZVector pos;
    ROOT::Math::Rotation3D rotation;
  };

  struct HitFinderTofChanPar {
    std::vector<double> fvCPTOff;               //[nbSide]
    std::vector<double> fvCPTotGain;            //[nbSide]
    std::vector<std::vector<double>> fvCPWalk;  //[nbSide][nbWalkBins]
    int32_t address;                            //unique address
    TofCell cell;
  };

  struct HitFinderTofRpcPar {
    double FeeTimeRes;
    double SysTimeRes;
    double CPSigPropSpeed;
    double outTimeFactor;
    double TOTMax;
    double TOTMin;
    double maxTimeDist;
    double maxSpaceDist;
    double gapSize;
    int32_t numGaps;
    int32_t numClWalkBinX;
    std::vector<HitFinderTofChanPar> fChanPar = {};
  };

  struct TofCluster {
    //temporary values
    std::vector<int32_t> vDigiIndRef;
    ROOT::Math::XYZVector weightedPos = ROOT::Math::XYZVector(0.0, 0.0, 0.0);
    double weightedTime               = 0.0;
    double weightedTimeErr            = 0.0;
    double weightsSum                 = 0.0;

    //after finalization
    ROOT::Math::XYZVector globalPos = ROOT::Math::XYZVector(0.0, 0.0, 0.0);
    ROOT::Math::XYZVector globalErr = ROOT::Math::XYZVector(0.0, 0.0, 0.0);
    int32_t channel                 = 0;
    int32_t detId                   = 0;

    int32_t numChan() { return vDigiIndRef.size() / 2; }

    void reset()
    {
      vDigiIndRef.clear();
      weightedPos     = ROOT::Math::XYZVector(0.0, 0.0, 0.0);
      globalPos       = ROOT::Math::XYZVector(0.0, 0.0, 0.0);
      globalErr       = ROOT::Math::XYZVector(0.0, 0.0, 0.0);
      weightedTime    = 0.0;
      weightedTimeErr = 0.0;
      weightsSum      = 0.0;
      channel         = 0;
      detId           = 0;
    }

    void add(ROOT::Math::XYZVector pos, double time, double timeErr, double weight, int32_t digiIndA, int32_t digiIndB)
    {
      vDigiIndRef.push_back(digiIndA);
      vDigiIndRef.push_back(digiIndB);
      weightedPos += pos * weight;
      weightedTime += time * weight;
      weightedTimeErr += timeErr * weight;
      weightsSum += weight;
    }

    void normalize(double timeErr)
    {
      // a/=b is translated to a := a*(1/b) in the ROOT::Math::XYZVector class, which has a different
      // rounding behavior than a := (a/b). In rare cases this leads to 1.000... becoming 0.999... inside
      // the floor() operation in the finalize() function of this class, and results in a different
      // channel being associated with the cluster. To reproduce the output of the old hit finder, we
      // divide element-wise instead. Perhaps floor() should be replaced by round().
      //////// weightedPos /= weightsSum;

      weightedPos.SetXYZ(weightedPos.X() / weightsSum, weightedPos.Y() / weightsSum, weightedPos.Z() / weightsSum);
      weightedTime /= weightsSum;
      weightedTimeErr = timeErr;
    }

    void finalize(const TofCell& trafoCell, const int32_t iTrafoCell, const HitFinderTofRpcPar& par)
    {
      // prepare local->global trafo
      ROOT::Math::Rotation3D rotMatrix = trafoCell.rotation;

      // get offset from weighted cluster position by rotation to master frame
      ROOT::Math::XYZVector hitposOffset = rotMatrix(weightedPos);

      //get hit position by adding offset to cell coordinates
      globalPos = trafoCell.pos + hitposOffset;

      // Simple errors, not properly done at all for now
      // Right way of doing it should take into account the weight distribution
      // and real system time resolution
      ROOT::Math::XYZVector hitErrLocal;
      hitErrLocal.SetX(trafoCell.sizeX / std::sqrt(12.0));    // Single strips approximation
      hitErrLocal.SetY(par.FeeTimeRes * par.CPSigPropSpeed);  // Use the electronics resolution
      hitErrLocal.SetZ(par.numGaps * par.gapSize / 10.0       // Change gap size in cm
                       / std::sqrt(12.0));                    // Use full RPC thickness as "Channel" Z size

      //store results
      globalErr = rotMatrix(hitErrLocal);
      channel   = iTrafoCell + floor(weightedPos.X() / trafoCell.sizeX);
      detId     = par.fChanPar[channel].address;
      weightedTime *= par.outTimeFactor;
    }
  };

  class HitFinderTof {
  public:
    typedef std::vector<TofCluster> resultType;

    /**
       ** @brief Constructor.
       **/
    HitFinderTof() {};

    /**
       ** @brief Destructor.
       **/
    ~HitFinderTof() {};

    /**
       ** @brief Build clusters out of ToF Digis and store the resulting info in a TofHit.
       **/
    resultType operator()(std::vector<CbmTofDigi> digisIn, const std::vector<int32_t>& digiIndexIn);

    /** @brief Set the parameter container
     ** @param params Vectorer to parameter container
     **/
    void SetParams(std::unique_ptr<HitFinderTofRpcPar> params) { fParams = *(std::move(params)); }

  private:
    HitFinderTofRpcPar fParams = {};  ///< Parameter container

    int32_t numSameSide;  // Digis quality
  };

} /* namespace cbm::algo */

#endif  // HITFINDERTOF_H
