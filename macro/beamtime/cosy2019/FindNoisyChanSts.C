/** @file FindNoisyChanSts.C
  * @copyright Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer] **/


Bool_t FindNoisyChanSts(TString sFilename, Double_t dNoiseThr = 60e3, UInt_t fuTotalNrOfFebs = 2)
{
  const UInt_t kuNbChanPerFeb   = 1024;
  const UInt_t kuFebIndexOffset = 1;

  TFile* pFile = TFile::Open(sFilename);

  if (nullptr == pFile) return kFALSE;

  gROOT->cd();

  TProfile* tempProfile = nullptr;

  TProfile* phRateProfFeb = new TProfile();
  /// Obtaining rate profile for this FEB
  for (UInt_t uFeb = 0; uFeb < fuTotalNrOfFebs; ++uFeb) {
    UInt_t uFebIdx = uFeb + kuFebIndexOffset;
    tempProfile    = (TProfile*) (pFile->FindObjectAny(Form("hStsFebChanRateProf_%03u", uFebIdx)));
    if (nullptr != tempProfile) {
      tempProfile->Copy(*(phRateProfFeb));  ///?
    }                                       // if( NULL != tempProfile )
    else
      return kFALSE;


    for (UInt_t uChan = 0; uChan < kuNbChanPerFeb; ++uChan) {
      Double_t dMeanRate = phRateProfFeb->GetBinContent(1 + uChan);

      if (dNoiseThr < dMeanRate)
        std::cout << Form("Noisy channel: FEB %02u Channel %4u Mean Rate %7.0f", uFebIdx, uChan, dMeanRate)
                  << std::endl;
    }  // for( UInt_t uChan = 0; uChan < kuNbChanPerFeb; ++uChan )
  }    // for( UInt_t uFeb = 0; uFeb < fuTotalNrOfFebs; ++uFeb )

  pFile->Close();

  return kTRUE;
}
