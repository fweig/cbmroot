/** @file test_timeslice.C
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 3 July 2018 **
 **/


// --------------------------------------------------------------------------
//
// Check correct timing of STS digis in time-slices 
//
// V. Friese   03/07/2018
//
// This macro checks for a raw data file generate by digitization:
// 1. Correct sorting of STS digis into time-slices
// 2. Time-ordering of STS digis
// 3. Time difference of subsequent digis in one channel (should be
//    more than the single-channel dead time.
// These checks test both the STS digitizer and the DAQ.
// --------------------------------------------------------------------------


// Includes needed for IDE
#if !defined(__CLING__)
#include "TStopwatch.h"
#include "FairSystemInfo.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "CbmDigiManager.h"
#include "CbmStsDigi.h"
#include "CbmTimeSlice.h"
#endif


void test_timeslice(TString dataset = "test") {

  // --- Remove old CTest runtime dependency file
  TString dataDir = gSystem->DirName(dataset);
  TString dataName = gSystem->BaseName(dataset);
  TString depFile = Remove_CTest_Dependency_File(dataDir, "test_timeslice",
                                                 dataName);

  // --- Temporary variables
  //TClonesArray* digis = new TClonesArray("CbmStsDigi", 1e6);
  CbmTimeSlice* slice = new CbmTimeSlice();
  const std::vector<CbmStsDigi>* digis = nullptr;
  const CbmStsDigi* digi = nullptr;
  Int_t nError1 = 0;
  Int_t nError2 = 0;
  Int_t nError3 = 0;
  std::map<Int_t, std::map<UShort_t, Double_t>> channelTime;
  Int_t address = 0;
  UShort_t channel = 0;
  Double_t digiTime = 0.;
  Double_t digiTimeLast = 0.;
  Double_t digiTimeChannel = 0.;

  // --- Channel dead time
  // --- Some margin because of time resolution
  Double_t deadTime = 750.;

  // --- Timer
  TStopwatch timer;
  timer.Start();
  Double_t ioTime = 0.;
  Double_t prTime = 0.;

  // --- Open file and get branches
  TString inFile = dataset + ".raw.root";
  std::cout << "+++ Opening " << inFile << std::endl;
  TFile f(inFile);
  assert(f.IsOpen());
  TTree* tree = (TTree*) f.Get("cbmsim");
  Int_t nEntries = tree->GetEntries();
  std::cout << "+++ Time slices: " << nEntries << std::endl;
  tree->SetBranchAddress("StsDigi", &digis);
  tree->SetBranchAddress("TimeSlice.", &slice);
  //CbmDigiManager* digiMan = CbmDigiManager::Instance();
  if ( digis == nullptr ) {
    std::cerr << "+++ No STS digi branch in tree!" << std::endl;
    f.Close();
    return;
  }
  timer.Stop();
  std::cout << "+++ Get branches: " << timer.RealTime() << " s " << std::endl;
  timer.Start();

  // Time-slice loop
  for (Int_t iEntry = 0; iEntry < nEntries; iEntry++) {
    std::cout << "+++ Time slice " << iEntry << std::endl;
    tree->GetEntry(iEntry);
    timer.Stop();
    ioTime += timer.RealTime();
    timer.Start();

    // Time-slice limits
    Double_t timeStart = slice->GetStartTime();
    Double_t timeEnd = slice->GetEndTime();
    Int_t iDigi = 0;
    std::cout << "+++ " << slice->ToString() << std::endl;

    // Digi loop
    for (auto& digi : *digis) {
      address  = digi.GetAddress();
      channel  = digi.GetChannel();
      digiTime = digi.GetTime();

      // Check correct sorting in time-slice
      if ( ! slice->IsFlexible() ) {
        if ( digiTime < timeStart ) {
          std::cout << "!!! Error for digi " << iDigi << ": t=  " << digiTime
              << " is before time-slice start " << timeStart << std::endl;
          nError1 ++;
        }
        if ( digiTime > timeEnd ) {
          std::cout << "!!! Error for digi " << iDigi << ": t = " << digiTime
              << " is after time-slice end " << timeEnd << std::endl;
          nError1 ++;
        }
      }

      // Check correct time-ordering
      if ( iEntry != 0 || iDigi != 0 ) {
        if ( digiTime < digiTimeLast ) {
          std::cout << "!!! Error for digi " << iDigi << ": time is "
              << digiTime << ", previous time was " << digiTimeLast
              << " difference " << digiTime - digiTimeLast
              << std::endl << std::endl;
          nError2++;
        } //? time before last digi
      } //? not first digi
      digiTimeLast = digiTime;

      // Check time difference in each channel
      auto itAddress = channelTime.find(address);
      if ( itAddress != channelTime.end() ) {
        auto itChannel = itAddress->second.find(channel);
        if ( itChannel != itAddress->second.end() ) {
          digiTimeChannel = channelTime[address][channel];
          if ( digiTime - digiTimeChannel < deadTime ) {
            std::cout << "!!! Error for digi " << iDigi << ": time is "
                << digiTime << ", previous time in channel was " << digiTimeChannel
                << " difference " << digiTime - digiTimeChannel
                << " is larger than dead time " << deadTime
                << std::endl << std::endl;
            nError3++;
          } //? time difference smaller than dead time
        } //? not first digi in channel
      } //? not first digi in module
      channelTime[address][channel] = digiTime;

      iDigi++;
    } //# digis

    timer.Stop();
    prTime += timer.RealTime();
    timer.Start();

  } // time slices

  timer.Stop();
  std::cout << "+++ I/O time         : " << ioTime << " s " << std::endl;
  std::cout << "+++ Process time     : " << prTime << " s " << std::endl;
  std::cout << "+++ Sorting errors   " << nError1 << std::endl;
  std::cout << "+++ Ordering errors  " << nError2 << std::endl;
  std::cout << "+++ Dead time errors " << nError3 << std::endl;
  if ( nError1 + nError2 + nError3 ) {
    std::cout << "+++ Macro finished with errors" << std::endl;
  }
  else {
    std::cout << " Test passed" << std::endl;
    std::cout << " All ok " << std::endl;
  }

  f.Close();
  Generate_CTest_Dependency_File(depFile);
}


