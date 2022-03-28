/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

void pulser_dt_full(TString filename = "1935.digi.root", uint32_t uNbTs = 10)
{

  gROOT->cd();
  TFile* file = new TFile(filename, "READ");
  TTree* tree = (TTree*) (file->Get("cbmsim"));

  std::vector<CbmTofDigi>* vDigisBmon  = new std::vector<CbmTofDigi>();
  std::vector<CbmStsDigi>* vDigisSts   = new std::vector<CbmStsDigi>();
  std::vector<CbmStsDigi>* vDigisStsP  = new std::vector<CbmStsDigi>();
  std::vector<CbmMuchDigi>* vDigisMuch = new std::vector<CbmMuchDigi>();
  std::vector<CbmTrdDigi>* vDigisTrd   = new std::vector<CbmTrdDigi>();
  std::vector<CbmTofDigi>* vDigisTof   = new std::vector<CbmTofDigi>();
  std::vector<CbmRichDigi>* vDigisRich = new std::vector<CbmRichDigi>();

  std::vector<CbmTofDigi> vPulserDigisBmon {};
  std::vector<CbmStsDigi> vPulserDigisSts {};
  std::vector<CbmMuchDigi> vPulserDigisMuch {};
  std::vector<CbmTrdDigi> vPulserDigisTrd1d {};
  std::vector<CbmTrdDigi> vPulserDigisTrd2d {};
  std::vector<CbmTofDigi> vPulserDigisTof {};
  std::vector<CbmRichDigi> vPulserDigisRich {};

  std::vector<CbmTofDigi> vNotPulserDigisBmon {};
  std::vector<CbmStsDigi> vNotPulserDigisSts {};
  std::vector<CbmMuchDigi> vNotPulserDigisMuch {};
  std::vector<CbmTrdDigi> vNotPulserDigisTrd1d {};
  std::vector<CbmTrdDigi> vNotPulserDigisTrd2d {};
  std::vector<CbmTofDigi> vNotPulserDigisTof {};
  std::vector<CbmRichDigi> vNotPulserDigisRich {};

  tree->SetBranchAddress("T0Digi", &vDigisBmon);
  tree->SetBranchAddress("StsDigi", &vDigisSts);
  tree->SetBranchAddress("StsDigiPulser", &vDigisStsP);
  tree->SetBranchAddress("MuchDigi", &vDigisMuch);
  tree->SetBranchAddress("TrdDigi", &vDigisTrd);
  tree->SetBranchAddress("TofDigi", &vDigisTof);
  tree->SetBranchAddress("RichDigi", &vDigisRich);

  uint32_t nentries = tree->GetEntries();
  cout << "Entries: " << nentries << endl;
  nentries = (uNbTs && uNbTs < nentries ? uNbTs : nentries);

  double_t dHistoRange = 6000.;
  // clang-format off
  TH1F *fPulserTofBmonDt      = new TH1F("fPulserTofBmonDt",
                                         "Bmon-Tof;time diff [ns];Counts",
                                         2*dHistoRange, -dHistoRange, dHistoRange);
  TH1F *fPulserTofStsDt       = new TH1F("fPulserTofStsDt",
                                         "Sts-Tof;time diff [ns];Counts",
                                         dHistoRange/3.125, -dHistoRange, dHistoRange);
  TH1F *fPulserStsStsDt       = new TH1F("fPulserStsStsDt",
                                         "Sts-Sts;time diff [ns];Counts",
                                         5001,     -5000, 50005000);
  TH2F *fPulserTofStsDtAdc    = new TH2F("fPulserTofStsDtAdc",
                                         "Sts-Tof;time diff [ns];Counts",
                                         dHistoRange/3.125, -dHistoRange, dHistoRange,
                                         32, 0, 32);
  TH1F *fPulserTofMuchDt      = new TH1F("fPulserTofMuchDt",
                                         "Much-Tof;time diff [ns];Counts",
                                         2*dHistoRange/3.125, -dHistoRange, dHistoRange);
  TH1F *fPulserTofTrd1dDt     = new TH1F("fPulserTofTrd1dDt",
                                         "Trd1d-Tof;time diff [ns];Counts",
                                         2*dHistoRange/60, -dHistoRange, dHistoRange);
  TH1F *fPulserTofTrd1dMainDt = new TH1F("fPulserTofTrd1dMainDt",
                                         "Trd1d-Tof;time diff [ns];Counts",
                                         2*dHistoRange/60, -dHistoRange, dHistoRange);
  TH1F *fPulserTofTrd2dDt     = new TH1F("fPulserTofTrd2dDt",
                                         "Trd2d-Tof;time diff [ns];Counts",
                                         2*dHistoRange/60, -dHistoRange, dHistoRange);
  TH1F *fPulserTofRichDt      = new TH1F("fPulserTofRichDt",
                                         "Rich-Tof;time diff [ns];Counts",
                                         2*dHistoRange, -dHistoRange, dHistoRange);
  TH1F *fPulserTofTofDt       = new TH1F("fPulserTofTofDt",
                                         "Tof-Tof;time diff [ns];Counts",
                                         5001,     -5000, 50005000);

  TH1F *fTofBmonDt  = new TH1F("fTofBmonDt",
                               "Bmon-Tof;time diff [ns];Counts",
                               2*dHistoRange, -dHistoRange, dHistoRange);
  TH1F *fTofStsDt   = new TH1F("fTofStsDt",
                               "Sts-Tof;time diff [ns];Counts",
                               2*dHistoRange/3.125, -dHistoRange, dHistoRange);
  TH1F *fTofMuchDt  = new TH1F("fTofMuchDt",
                               "Much-Tof;time diff [ns];Counts",
                               2*dHistoRange/3.125, -dHistoRange, dHistoRange);
  TH1F *fTofTrd1dDt = new TH1F("fTofTrd1dDt",
                               "Trd1d-Tof;time diff [ns];Counts",
                               2*dHistoRange/60, -dHistoRange, dHistoRange);
  TH1F *fTofTrd2dDt = new TH1F("fTofTrd2dDt",
                               "Trd2d-Tof;time diff [ns];Counts",
                               2*dHistoRange/60, -dHistoRange, dHistoRange);
  TH1F *fTofRichDt  = new TH1F("fTofRichDt",
                               "Rich-Tof;time diff [ns];Counts",
                               2*dHistoRange, -dHistoRange, dHistoRange);
  TH1F *fTofTofDt   = new TH1F("fTofTofDt",
                               "Tof-Tof;time diff [ns];Counts",
                               5001,     -5000, 50005000);
  // clang-format on

  std::map<uint32_t, uint32_t> mTrdModules;
  std::map<uint32_t, uint32_t> mTrdPad;
  uint32_t uAddressFirstTof = 0;
  for (Int_t iEntry = 0; iEntry < nentries; iEntry++) {
    //if (iEntry % 10 == 0 ) std::cout << "Entry " << iEntry << " / " << nentries << std::endl;

    tree->GetEntry(iEntry);
    uint32_t nDigisBmon = vDigisBmon->size();
    uint32_t nDigisSts  = vDigisSts->size();
    uint32_t nDigisStsP = vDigisStsP->size();
    uint32_t nDigisMuch = vDigisMuch->size();
    uint32_t nDigisTrd  = vDigisTrd->size();
    uint32_t nDigisTof  = vDigisTof->size();
    uint32_t nDigisRich = vDigisRich->size();

    if (iEntry % 10 == 0) {
      std::cout << "BMON  Digis: " << nDigisBmon << std::endl;
      std::cout << "STS   Digis: " << nDigisSts << std::endl;
      std::cout << "STS P Digis: " << nDigisStsP << std::endl;
      std::cout << "MUCH  Digis: " << nDigisMuch << std::endl;
      std::cout << "TRD   Digis: " << nDigisTrd << std::endl;
      std::cout << "TOF   Digis: " << nDigisTof << std::endl;
      std::cout << "RICH  Digis: " << nDigisRich << std::endl;
    }

    // Bmon
    // double_t dLatPulsTime = 0;
    for (uint32_t uDigiBmon = 0; uDigiBmon < nDigisBmon; ++uDigiBmon) {

      CbmTofDigi& pDigi = vDigisBmon->at(uDigiBmon);

      // if ( (0 == pDigi.GetChannel() || 32 == pDigi.GetChannel())
      //      && (185 < pDigi.GetCharge() && pDigi.GetCharge() < 189) ) {
      if ((185 < pDigi.GetCharge() && pDigi.GetCharge() < 189)) {  //
        vPulserDigisBmon.push_back(pDigi);
      }  // feb condition
      else {
        //   if( 400 < pDigi.GetTime() - dLatPulsTime ) {
        vNotPulserDigisBmon.push_back(pDigi);
        // }
      }
    }
    // STS
    for (uint32_t uDigiSts = 0; uDigiSts < nDigisStsP; ++uDigiSts) {

      CbmStsDigi& pDigi = vDigisStsP->at(uDigiSts);

      uint32_t uAddr = pDigi.GetAddress();
      if ((uAddr == 0x10107C02 || uAddr == 0x101FFC02) && 5 < pDigi.GetCharge()) {  //
        vPulserDigisSts.push_back(pDigi);
      }  // feb condition
    }
    for (uint32_t uDigiSts = 0; uDigiSts < nDigisSts; ++uDigiSts) {

      CbmStsDigi& pDigi = vDigisSts->at(uDigiSts);

      if (3 < pDigi.GetCharge()) {  //
        vNotPulserDigisSts.push_back(pDigi);
      }  // feb condition
    }
    // Much
    for (uint32_t uDigiMuch = 0; uDigiMuch < nDigisMuch; ++uDigiMuch) {

      CbmMuchDigi& pDigi = vDigisMuch->at(uDigiMuch);

      if (2 < pDigi.GetCharge()) {
        // if (pDigi.GetAddress()==0x10107C02 || pDigi.GetAddress()==0x101FFC02) { //
        vPulserDigisMuch.push_back(pDigi);
        // } // feb condition
        // else {
        vNotPulserDigisMuch.push_back(pDigi);
        // }
      }
    }
    // TRD
    for (uint32_t uDigiTrd = 0; uDigiTrd < nDigisTrd; ++uDigiTrd) {

      CbmTrdDigi& pDigi = vDigisTrd->at(uDigiTrd);

      if (pDigi.GetType() == CbmTrdDigi::eCbmTrdAsicType::kSPADIC) {
        uint32_t uAddr = pDigi.GetAddress();
        uint32_t uMod  = CbmTrdAddress::GetModuleId(uAddr);
        if (mTrdModules.find(uMod) == mTrdModules.end()) {  //
          mTrdModules[uMod] = 1;
        }
        else
          mTrdModules[uMod]++;

        if (mTrdPad.find(uAddr) == mTrdPad.end()) {  //
          mTrdPad[uAddr] = 1;
        }
        else
          mTrdPad[uAddr]++;

        // if (0x0008007b == uAddr ) { //
        if (0x80070 <= uAddr && uAddr <= 0x800ff) {  //
          vPulserDigisTrd1d.push_back(pDigi);
        }
        else {
          vNotPulserDigisTrd1d.push_back(pDigi);
        }
      }
      else if (pDigi.GetType() == CbmTrdDigi::eCbmTrdAsicType::kFASP) {
        vNotPulserDigisTrd2d.push_back(pDigi);
      }
    }
    // TOF
    double_t dLatPulsTime = 0;
    for (uint32_t uDigiTof = 0; uDigiTof < nDigisTof; ++uDigiTof) {

      CbmTofDigi& pDigi = vDigisTof->at(uDigiTof);

      // if ( (0 == uAddressFirstTof || uAddressFirstTof == pDigi.GetAddress() )
      if (true  //
          && (0 == pDigi.GetChannel() || 32 == pDigi.GetChannel())
          && (185 < pDigi.GetCharge() && pDigi.GetCharge() < 189)) {
        if (0 == uAddressFirstTof) {  //
          uAddressFirstTof = pDigi.GetAddress();
        }
        if (uAddressFirstTof == pDigi.GetAddress()) {
          vPulserDigisTof.push_back(pDigi);
          dLatPulsTime = pDigi.GetTime();
        }
      }  // feb condition
      else {
        if (400 < pDigi.GetTime() - dLatPulsTime) {  //
          vNotPulserDigisTof.push_back(pDigi);
        }
      }
    }
    // Rich
    for (uint32_t uDigiRich = 0; uDigiRich < nDigisRich; ++uDigiRich) {

      CbmRichDigi& pDigi = vDigisRich->at(uDigiRich);

      // if (pDigi.GetAddress()==0x10107C02 || pDigi.GetAddress()==0x101FFC02) { //
      vPulserDigisRich.push_back(pDigi);
      // } // feb condition
      // else {
      vNotPulserDigisRich.push_back(pDigi);
      // }
    }

    /// PULSER ********************************************************************************************************///
    double_t dLastStsTime         = 0;
    double_t dLastTofTime         = 0;
    uint32_t uLastFirstMatchBmon  = 0;
    uint32_t uLastFirstMatchSts   = 0;
    uint32_t uLastFirstMatchMuch  = 0;
    uint32_t uLastFirstMatchTrd1d = 0;
    uint32_t uLastFirstMatchTrd2d = 0;
    uint32_t uLastFirstMatchRich  = 0;
    for (uint32_t uPulsStsIdx = 0; uPulsStsIdx < vPulserDigisSts.size(); ++uPulsStsIdx) {
      double_t dStsTime = vPulserDigisSts[uPulsStsIdx].GetTime();
      if (0 < uPulsStsIdx) {  //
        fPulserStsStsDt->Fill(dStsTime - dLastStsTime);
      }  // if 0 < uPulsTofIdx )
      dLastStsTime = dStsTime;
    }

    for (uint32_t uPulsTofIdx = 0; uPulsTofIdx < vPulserDigisTof.size(); ++uPulsTofIdx) {
      double_t dTofTime = vPulserDigisTof[uPulsTofIdx].GetTime();
      if (0 < uPulsTofIdx) {  //
        fPulserTofTofDt->Fill(dTofTime - dLastTofTime);
      }  // if 0 < uPulsTofIdx )
      dLastTofTime = dTofTime;

      /// Bmon - TOF
      for (uint32_t uPulsBmonIdx = uLastFirstMatchBmon; uPulsBmonIdx < vPulserDigisBmon.size(); ++uPulsBmonIdx) {
        double_t dBmonTime = vPulserDigisBmon[uPulsBmonIdx].GetTime();
        double_t dBmonDt   = dBmonTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dBmonDt < -dHistoRange) {  //
          uLastFirstMatchBmon = uPulsBmonIdx;
        }
        else if (dBmonDt <= dHistoRange) {
          fPulserTofBmonDt->Fill(dBmonDt);
        }
        else
          break;
      }  // for( uint32_t uPulsBmonIdx = uLastFirstMatchBmon; uPulsBmonIdx < vPulserDigisBmon.size(); ++uPulsBmonIdx)

      /// STS - TOF
      for (uint32_t uPulsStsIdx = uLastFirstMatchSts; uPulsStsIdx < vPulserDigisSts.size(); ++uPulsStsIdx) {
        double_t dStsTime = vPulserDigisSts[uPulsStsIdx].GetTime();
        double_t dStsDt   = dStsTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dStsDt < -dHistoRange) {  //
          uLastFirstMatchSts = uPulsStsIdx;
        }
        else if (dStsDt <= dHistoRange) {
          fPulserTofStsDt->Fill(dStsDt);
          fPulserTofStsDtAdc->Fill(dStsDt, vPulserDigisSts[uPulsStsIdx].GetCharge());
        }
        else
          break;
      }  // for( uint32_t uPulsStsIdx = uLastFirstMatchSts; uPulsStsIdx < vPulserDigisSts.size(); ++uPulsStsIdx)

      /// Much - TOF
      for (uint32_t uPulsMuchIdx = uLastFirstMatchMuch; uPulsMuchIdx < vPulserDigisMuch.size(); ++uPulsMuchIdx) {
        double_t dMuchTime = vPulserDigisMuch[uPulsMuchIdx].GetTime();
        double_t dMuchDt   = dMuchTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dMuchDt < -dHistoRange) {  //
          uLastFirstMatchMuch = uPulsMuchIdx;
        }
        else if (dMuchDt <= dHistoRange) {
          fPulserTofMuchDt->Fill(dMuchDt);
        }
        else
          break;
      }  // for( uint32_t uPulsMuchIdx = uLastFirstMatchMuch; uPulsMuchIdx < vPulserDigisMuch.size(); ++uPulsMuchIdx)

      /// TRD1D - TOF
      for (uint32_t uPulsTrdIdx = uLastFirstMatchTrd1d; uPulsTrdIdx < vPulserDigisTrd1d.size(); ++uPulsTrdIdx) {
        double_t dTrdTime = vPulserDigisTrd1d[uPulsTrdIdx].GetTime();
        double_t dTrdDt   = dTrdTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dTrdDt < -dHistoRange) {  //
          uLastFirstMatchTrd1d = uPulsTrdIdx;
        }
        else if (dTrdDt <= dHistoRange) {
          if (0x0008007b == vPulserDigisTrd1d[uPulsTrdIdx].GetAddress()) {  //
            fPulserTofTrd1dMainDt->Fill(dTrdDt);
          }
          else {
            fPulserTofTrd1dDt->Fill(dTrdDt);
          }
        }
        else
          break;
      }  // for( uint32_t uPulsTrdIdx = uLastFirstMatchTrd1d; uPulsTrdIdx < vPulserDigisTrd1d.size(); ++uPulsTrdIdx)

      /// TRD2D - TOF
      for (uint32_t uPulsTrdIdx = uLastFirstMatchTrd2d; uPulsTrdIdx < vPulserDigisTrd2d.size(); ++uPulsTrdIdx) {
        double_t dTrdTime = vPulserDigisTrd2d[uPulsTrdIdx].GetTime();
        double_t dTrdDt   = dTrdTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dTrdDt < -dHistoRange) {  //
          uLastFirstMatchTrd2d = uPulsTrdIdx;
        }
        else if (dTrdDt <= dHistoRange) {
          fPulserTofTrd2dDt->Fill(dTrdDt);
        }
        else
          break;
      }  // for( uint32_t uPulsTrdIdx = uLastFirstMatchTrd2d; uPulsTrdIdx < vPulserDigisTrd2d.size(); ++uPulsTrdIdx)

      /// RICH - TOF
      for (uint32_t uPulsRichIdx = uLastFirstMatchRich; uPulsRichIdx < vPulserDigisRich.size(); ++uPulsRichIdx) {
        double_t dRichTime = vPulserDigisRich[uPulsRichIdx].GetTime();
        double_t dRichDt   = dRichTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dRichDt < -dHistoRange) {  //
          uLastFirstMatchRich = uPulsRichIdx;
        }
        else if (dRichDt <= dHistoRange) {
          fPulserTofRichDt->Fill(dRichDt);
        }
        else
          break;
      }  // for( uint32_t uPulsRichIdx = uLastFirstMatchRich; uPulsRichIdx < vPulserDigisRich.size(); ++uPulsRichIdx)
    }
    /// ***************************************************************************************************************///

    /// NOT PULSER ****************************************************************************************************///
    dLastTofTime         = 0;
    uLastFirstMatchBmon  = 0;
    uLastFirstMatchSts   = 0;
    uLastFirstMatchMuch  = 0;
    uLastFirstMatchTrd1d = 0;
    uLastFirstMatchTrd2d = 0;
    uLastFirstMatchRich  = 0;
    for (uint32_t uTofIdx = 0; uTofIdx < vNotPulserDigisTof.size(); ++uTofIdx) {
      double_t dTofTime = vNotPulserDigisTof[uTofIdx].GetTime();
      if (0 < uTofIdx) {  //
        fTofTofDt->Fill(dTofTime - dLastTofTime);
      }  // if 0 < uTofIdx )
      dLastTofTime = dTofTime;

      /// Bmon - TOF
      for (uint32_t uBmonIdx = uLastFirstMatchBmon; uBmonIdx < vNotPulserDigisBmon.size(); ++uBmonIdx) {
        double_t dBmonTime = vNotPulserDigisBmon[uBmonIdx].GetTime();
        double_t dBmonDt   = dBmonTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dBmonDt < -dHistoRange) {  //
          uLastFirstMatchBmon = uBmonIdx;
        }
        else if (dBmonDt <= dHistoRange) {
          fTofBmonDt->Fill(dBmonDt);
        }
        else
          break;
      }  // for( uint32_t uBmonIdx = uLastFirstMatchBmon; uBmonIdx < vNotPulserDigisBmon.size(); ++uBmonIdx)

      /// STS - TOF
      for (uint32_t uStsIdx = uLastFirstMatchSts; uStsIdx < vNotPulserDigisSts.size(); ++uStsIdx) {
        double_t dStsTime = vNotPulserDigisSts[uStsIdx].GetTime();
        double_t dStsDt   = dStsTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dStsDt < -dHistoRange) {  //
          uLastFirstMatchSts = uStsIdx;
        }
        else if (dStsDt <= dHistoRange) {
          fTofStsDt->Fill(dStsDt);
        }
        else
          break;
      }  // for( uint32_t uStsIdx = uLastFirstMatchSts; uStsIdx < vNotPulserDigisSts.size(); ++uStsIdx)

      /// Much - TOF
      for (uint32_t uMuchIdx = uLastFirstMatchMuch; uMuchIdx < vNotPulserDigisMuch.size(); ++uMuchIdx) {
        double_t dMuchTime = vNotPulserDigisMuch[uMuchIdx].GetTime();
        double_t dMuchDt   = dMuchTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dMuchDt < -dHistoRange) {  //
          uLastFirstMatchMuch = uMuchIdx;
        }
        else if (dMuchDt <= dHistoRange) {
          fTofMuchDt->Fill(dMuchDt);
        }
        else
          break;
      }  // for( uint32_t uMuchIdx = uLastFirstMatchMuch; uMuchIdx < vNotPulserDigisMuch.size(); ++uMuchIdx)

      /// Trd 1d - TOF
      for (uint32_t uTrdIdx = uLastFirstMatchTrd1d; uTrdIdx < vNotPulserDigisTrd1d.size(); ++uTrdIdx) {
        double_t dTrdTime = vNotPulserDigisTrd1d[uTrdIdx].GetTime();
        double_t dTrdDt   = dTrdTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dTrdDt < -dHistoRange) {  //
          uLastFirstMatchTrd1d = uTrdIdx;
        }
        else if (dTrdDt <= dHistoRange) {
          fTofTrd1dDt->Fill(dTrdDt);
        }
        else
          break;
      }  // for( uint32_t uTrdIdx = uLastFirstMatchTrd1d; uTrdIdx < vNotPulserDigisTrd1d.size(); ++uTrdIdx)

      /// Trd 2d - TOF
      for (uint32_t uTrdIdx = uLastFirstMatchTrd2d; uTrdIdx < vNotPulserDigisTrd2d.size(); ++uTrdIdx) {
        double_t dTrdTime = vNotPulserDigisTrd2d[uTrdIdx].GetTime();
        double_t dTrdDt   = dTrdTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dTrdDt < -dHistoRange) {  //
          uLastFirstMatchTrd2d = uTrdIdx;
        }
        else if (dTrdDt <= dHistoRange) {
          fTofTrd2dDt->Fill(dTrdDt);
        }
        else
          break;
      }  // for( uint32_t uTrdIdx = uLastFirstMatchTrd2d; uTrdIdx < vNotPulserDigisTrd2d.size(); ++uTrdIdx)

      /// RICH - TOF
      for (uint32_t uRichIdx = uLastFirstMatchRich; uRichIdx < vNotPulserDigisRich.size(); ++uRichIdx) {
        double_t dRichTime = vNotPulserDigisRich[uRichIdx].GetTime();
        double_t dRichDt   = dRichTime - dTofTime;

        // Find beginning of match window (assume both vectors are fully time sorted)
        if (dRichDt < -dHistoRange) {  //
          uLastFirstMatchRich = uRichIdx;
        }
        else if (dRichDt <= dHistoRange) {
          fTofRichDt->Fill(dRichDt);
        }
        else
          break;
      }  // for( uint32_t uRichIdx = uLastFirstMatchRich; uRichIdx < vNotPulserDigisRich.size(); ++vNotPulserDigisRich)
    }

    /// ***************************************************************************************************************///
    std::cout << "BMON  Pulser Digis: " << vPulserDigisBmon.size() << std::endl;
    std::cout << "STS   Pulser Digis: " << vPulserDigisSts.size() << std::endl;
    std::cout << "MUCH  Pulser Digis: " << vPulserDigisMuch.size() << std::endl;
    std::cout << "TRD1D Pulser Digis: " << vPulserDigisTrd1d.size() << std::endl;
    std::cout << "TRD2D Pulser Digis: " << vPulserDigisTrd2d.size() << std::endl;
    std::cout << "TOF   Pulser Digis: " << vPulserDigisTof.size() << std::endl;
    std::cout << "RICH  Pulser Digis: " << vPulserDigisRich.size() << std::endl;
    /// ***************************************************************************************************************///
    std::cout << "BMON  Non-Pulser Digis: " << vNotPulserDigisBmon.size() << std::endl;
    std::cout << "STS   Non-Pulser Digis: " << vNotPulserDigisSts.size() << std::endl;
    std::cout << "MUCH  Non-Pulser Digis: " << vNotPulserDigisMuch.size() << std::endl;
    std::cout << "TRD1D Non-Pulser Digis: " << vNotPulserDigisTrd1d.size() << std::endl;
    std::cout << "TRD2D Non-Pulser Digis: " << vNotPulserDigisTrd2d.size() << std::endl;
    std::cout << "TOF   Non-Pulser Digis: " << vNotPulserDigisTof.size() << std::endl;
    std::cout << "RICH  Non-Pulser Digis: " << vNotPulserDigisRich.size() << std::endl;
    /// ***************************************************************************************************************///

    vPulserDigisBmon.clear();
    vPulserDigisSts.clear();
    vPulserDigisMuch.clear();
    vPulserDigisTrd1d.clear();
    vPulserDigisTrd2d.clear();
    vPulserDigisTof.clear();
    vPulserDigisRich.clear();

    vNotPulserDigisBmon.clear();
    vNotPulserDigisSts.clear();
    vNotPulserDigisMuch.clear();
    vNotPulserDigisTrd1d.clear();
    vNotPulserDigisTrd1d.clear();
    vNotPulserDigisTof.clear();
    vNotPulserDigisRich.clear();
  }
  /// ***************************************************************************************************************///
  /*
  for (auto& stsModcCn : mStsModules) {
     std::cout << "STS module 0x" << std::hex << std::setw(8) << stsModcCn.first << std::dec
               << " had " << std::setw(8) << stsModcCn.second << " digis" << std::endl;
  }
  */
  /// ***************************************************************************************************************///
  /*
  for (auto& trdModcCn : mTrdModules) {
     std::cout << "TRD module 0x" << std::hex << std::setw(8) << trdModcCn.first << std::dec
               << " had " << std::setw(8) << trdModcCn.second << " digis" << std::endl;
  }
  */
  /// ***************************************************************************************************************///
  /*
  for (auto& trdPadcCn : mTrdPad) {
     std::cout << "TRD Pad 0x" << std::hex << std::setw(8) << trdPadcCn.first << std::dec
               << " had " << std::setw(8) << trdPadcCn.second << " digis" << std::endl;
  }
  */
  /// ***************************************************************************************************************///

  /// Timesort
  /*
  std::sort(vPulserDigisTrd.begin(), vPulserDigisTrd.end(),
            [](const CbmTrdDigi& a, const CbmTrdDigi& b) -> bool { return a.GetTime() < b.GetTime(); });
  */


  TCanvas* pCanv = new TCanvas("canv", "Pulser diff to TOF");
  pCanv->Divide(3, 2);

  pCanv->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fPulserTofBmonDt->Draw();

  pCanv->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fPulserTofStsDt->Draw();

  pCanv->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fPulserTofMuchDt->Draw();

  pCanv->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fPulserTofTrd1dDt->Draw();

  pCanv->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fPulserTofRichDt->Draw();

  pCanv->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fPulserTofTofDt->Draw();

  TCanvas* pCanvNot = new TCanvas("canvNot", "Not Pulser diff to TOF");
  pCanvNot->Divide(4, 2);

  pCanvNot->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fTofBmonDt->Draw();

  pCanvNot->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fTofStsDt->Draw();

  pCanvNot->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fTofMuchDt->Draw();

  pCanvNot->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fTofTrd1dDt->Draw();

  pCanvNot->cd(5);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fTofTrd2dDt->Draw();

  pCanvNot->cd(6);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fTofRichDt->Draw();

  pCanvNot->cd(7);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogy();
  fTofTofDt->Draw();
}
