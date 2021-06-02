/** @file CbmTofTBClusterizer.h
  * @copyright Copyright (C) 2016-2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov, Pierre-Alain Loizeau [committer], Volker Friese **/

#ifndef CBMTOFTBCLUSTERIZER_H
#define CBMTOFTBCLUSTERIZER_H

#include "CbmTofCell.h"
#include "CbmTofDetectorId_v12b.h"
#include "CbmTofDetectorId_v14a.h"
#include "CbmTofDigiBdfPar.h"
#include "CbmTofDigiExp.h"
#include "CbmTofDigiPar.h"
#include "CbmTofGeoHandler.h"

#include "FairTask.h"

#include "TClonesArray.h"


class CbmTofTBClusterizer : public FairTask {
public:
  struct ChannelDigis {
    ChannelDigis() : topDigis(), bottomDigis(), digiPairs() {};

    struct DigiDesc {
      CbmTofDigiExp* pDigi;
      Int_t digiInd;
    };

    struct DigiPair {
      Double_t y;
      DigiDesc topDigi;
      DigiDesc bottomDigi;
    };

    std::map<Double_t, DigiDesc> topDigis;
    std::map<Double_t, DigiDesc> bottomDigis;
    std::map<Double_t, DigiPair> digiPairs;
  };

  CbmTofTBClusterizer();
  InitStatus Init();
  void SetParContainers();
  void Exec(Option_t* option);
  void Finish();

private:
  Bool_t InitCalibParameter();
  void GetEventInfo(Int_t& inputNr, Int_t& eventNr, Double_t& eventTime);

private:
  CbmTofGeoHandler* fGeoHandler;
  CbmTofDetectorId* fTofId;
  CbmTofDigiPar* fDigiPar;
  CbmTofCell* fChannelInfo;
  CbmTofDigiBdfPar* fDigiBdfPar;
  std::vector<std::vector<Double_t>> fvCPSigPropSpeed;                       //[nSMT][nRpc]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPDelTof;   //[nSMT][nRpc][nbClDelTofBinX][nbTrg]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPTOff;     //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<Double_t>>>> fvCPTotGain;  //[nSMT][nRpc][nCh][nbSide]
  std::vector<std::vector<std::vector<std::vector<std::vector<Double_t>>>>>
    fvCPWalk;  //[nSMT][nRpc][nCh][nbSide][nbWalkBins]
  TClonesArray* fTofDigis;
  TClonesArray* fTofPoints;
  TClonesArray* fTofHits;
  TClonesArray* fTofDigiMatchs;
  //std::vector<std::vector<std::vector<std::list<CbmTofDigiExp*> > > > fStorDigiExp; //[nbType][nbSm*nbRpc][nbCh]{nDigis}


  std::vector<std::vector<std::vector<ChannelDigis>>> fStorDigiExp;

  std::vector<std::vector<
    std::vector<std::pair<std::pair<std::map<Double_t, std::pair<CbmTofDigiExp*, Int_t>>,
                                    std::map<Double_t, std::pair<CbmTofDigiExp*, Int_t>>>,
                          std::map<Double_t, std::pair<Double_t, std::pair<std::pair<CbmTofDigiExp*, Int_t>,
                                                                           std::pair<CbmTofDigiExp*, Int_t>>>>>>>>
    fStorDigiExpOld;  //[nbType][nbSm*nbRpc][nbCh]<[->|nTopDigis][->|nBottomDigis]>

  Double_t fOutTimeFactor;

  /** Make copy constructor and copy operator private to avoid warning due to pointer members **/
  CbmTofTBClusterizer(const CbmTofTBClusterizer&);
  CbmTofTBClusterizer& operator=(const CbmTofTBClusterizer&);

  ClassDef(CbmTofTBClusterizer, 1);
};

#endif /* CBMTOFTBCLUSTERIZER_H */
