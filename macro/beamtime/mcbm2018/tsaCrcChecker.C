/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

/** @file tsaCrcChecker
 ** @author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 ** @date 02.02.2019
 **
 ** ROOT macro to read tsa files, loop on the timeslices and microslices,
 ** check the CRC of each microslice and dump the ones with corrupt data
 ** to the standard output.
 */

Bool_t tsaCrcChecker(TString inFile, Int_t iStartFile = -1, Int_t iStopFile = -1)
{
  CbmMcbm2018MsCrcCheck* msChecker = new CbmMcbm2018MsCrcCheck();

  if ("" != inFile) {
    if (0 <= iStartFile && iStartFile < iStopFile) {
      for (Int_t iFileIdx = iStartFile; iFileIdx < iStopFile; ++iFileIdx) {
        TString sFilePath = Form("%s_%04u.tsa", inFile.Data(), iFileIdx);

        msChecker->AddFile(sFilePath);

        std::cout << "Added " << sFilePath << std::endl;
      }  // for( Int_t iFileIdx = iStartFile; iFileIdx < iStopFile; ++iFileIdx )
    }    // if( 0 < iStartFile && 0 < iStopFile )
    else
      msChecker->SetFileName(inFile);
  }  // if( "" != inFile )
  else
    std::cout << "This macro needs to be provided with an input file full path!!" << std::endl;

  msChecker->Run();

  delete msChecker;

  return kTRUE;
}
