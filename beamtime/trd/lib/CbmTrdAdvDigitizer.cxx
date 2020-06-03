/*
 * CbmTrdAdvDigitizer.cpp
 *
 *  Created on: Apr 22, 2017
 *      Author: philipp
 */

#include <CbmTrdAdvDigitizer.h>

CbmTrdAdvDigitizer::CbmTrdAdvDigitizer ()
    : CbmTrdQABase ("CbmTrdAdvDigitizer")
{
  // TODO Auto-generated constructor stub

}

CbmTrdAdvDigitizer::~CbmTrdAdvDigitizer ()
{
  // TODO Auto-generated destructor stub
}

InitStatus
CbmTrdAdvDigitizer::Init ()
{
  LOG(debug) << "Initilization of " << this->GetName ();
  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance ();

  // Get a pointer to the previous already existing data level
  fRaw = static_cast<TClonesArray*> (ioman->GetObject ("SpadicRawMessage"));
  if (!fRaw)
    {
      LOG(fatal) << "No InputDataLevelName SpadicRawMessage array!\n"
                    << this->GetName () << " will be inactive";
      return kERROR;
    }

  SetOutputBranchPersistent("TrdDigi",false);

  fDigis = new TClonesArray ("CbmTrdDigi", 100);
  ioman->Register ("TrdDigi", "TRD Digis", fDigis,
                   IsOutputBranchPersistent ("TrdDigi"));


  // Do whatever else is needed at the initilization stage
  // Create histograms to be filled
  // initialize variables
  CbmTrdAdvDigitizer::CreateHistograms ();

  return kSUCCESS;

}

void
CbmTrdAdvDigitizer::CreateHistograms ()
{
  for (Int_t layer = 0; layer < 4; layer++)
    {
      TString HistName = "Baselinemap_Layer_" + std::to_string (layer);
      fHm->Add (
          HistName.Data (),
          new TProfile2D (HistName.Data (), HistName.Data (), 256, -0.5, 255.5,
                          6, -0.5, 5.5));
      fHm->P2 (HistName.Data ())->GetXaxis ()->SetTitle ("Column");
      fHm->P2 (HistName.Data ())->GetYaxis ()->SetTitle ("Row");
    }
  TString GraphName = "Digis_Created";
  fHm->Add (GraphName.Data (), new TGraph);
  fHm->G1 (GraphName.Data ())->SetNameTitle (GraphName.Data (),
                                             GraphName.Data ());
  fHm->G1 (GraphName.Data ())->GetXaxis ()->SetTitle ("TimeSlice");
  fHm->G1 (GraphName.Data ())->GetYaxis ()->SetTitle ("Nr of created Digis");
}

void
CbmTrdAdvDigitizer::Exec (Option_t*)
{
  static Int_t NrTimeslice = 0;
  //LOG(info) << this->GetName () << ": Clearing  Digis";
  //fDigis->Clear ();
  UInt_t nSpadicMessages = fRaw->GetEntriesFast (); //SPADIC messages per TimeSlice
  TString Statefullness = "stateful";
  LOG(info) << this->GetName () << ": Estimating Baseline " << Statefullness
               << " Digitizer";
  std::vector<TProfile2D*> Hitmaps;

  TString GraphName = "Digis_Created";
  fDigis->ExpandCreateFast (0);
  LOG(info) << this->GetName () << ": Creating Digis with " << Statefullness
               << " Digitizer";
  CbmTrdDigiFinder DF;
  DF.start();
  DF.process_event(SetArrays(fRaw,fDigis));
  for (UInt_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      DF.process_event(NewMessage(iSpadicMessage));
    }
  Int_t NrDigis=fDigis->GetEntries();
  fHm->G1 (GraphName.Data ())->SetPoint (fHm->G1 (GraphName.Data ())->GetN (),
                                         NrTimeslice++, NrDigis);
  fDigis->Sort ();
  LOG(info) << this->GetName () << ": Finished creating Digis with "
               << Statefullness << " Digitizer, with 0-suppressed Samples";
}

ClassImp(CbmTrdAdvDigitizer)
