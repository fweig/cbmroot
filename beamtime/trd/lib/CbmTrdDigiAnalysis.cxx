/*
 * CbmTrdDigiAnalysis.cxx
 *
 *  Created on: Apr 22, 2017
 *      Author: philipp
 */

#include <CbmTrdDigiAnalysis.h>

ClassImp(CbmTrdDigiAnalysis)

CbmTrdDigiAnalysis::CbmTrdDigiAnalysis ()
    : CbmTrdQABase ("CbmTrdDigiAnalysis")
{
  // TODO Auto-generated constructor stub

}

CbmTrdDigiAnalysis::~CbmTrdDigiAnalysis ()
{
  // TODO Auto-generated destructor stub
}

void
CbmTrdDigiAnalysis::CreateHistograms ()
{
  if(false)
  for (Int_t Layer = 0; Layer <fBT->GetNrLayers (); ++Layer)
    {
      for (Int_t Row = 0; Row < fBT->GetNrRows(Layer) ; ++Row)
        {
          for (Int_t Col = 0; Col < fBT->GetNrColumns(Layer); Col++)
            {
              TString HistName = "Signalshape_Layer" + std::to_string(Layer) + "Row"+std::to_string(Row)+"Collumn"
                  + std::to_string (Col);
              fHm->Create2<TH2I> (HistName.Data (), HistName.Data (), 32, -0.5,
                                  31.5, 512 + 256, -255.5, 511.5);
              fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle (
                  "0-suppressed ADC-Value");
              fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("Timebin");
            }
        }
    }
  
  for (Int_t Layer = 0; Layer <fBT->GetNrLayers (); ++Layer)
    {
      TString HistName = "Hitmap_Layer" + std::to_string(Layer);
      fHm->Create2<TH2I> (HistName.Data (), HistName.Data (),fBT->GetNrColumns(Layer) , -0.5, fBT->GetNrColumns(Layer)-0.5, fBT->GetNrRows(Layer), -0.5, fBT->GetNrRows(Layer)-0.5);
      fHm->H2 (HistName.Data ())->GetXaxis ()->SetTitle ("Columns");
      fHm->H2 (HistName.Data ())->GetYaxis ()->SetTitle ("Rows");
    }
  
}

void
CbmTrdDigiAnalysis::Exec (Option_t*)
{
  const Int_t NrDigis = fDigiMan->GetNofDigis(ECbmModuleId::kTrd);
  //  LOG(info) << this->GetName () << ": Analyzing Signalshape of " << NrDigis
  //           << " Digis ";
  for (Int_t Index = 0; Index < NrDigis; Index++)
    {
      const CbmTrdDigi* Digi = fDigiMan->Get<CbmTrdDigi>(Index);
      Int_t Address=fBT->GetAddress(Digi);
      Int_t Layer=CbmTrdAddress::GetLayerId(Address);
      Int_t Row=CbmTrdAddress::GetRowId(Address);
      Int_t Col=CbmTrdAddress::GetColumnId(Address);
      TString HistName = "Hitmap_Layer" + std::to_string(Layer);
      
      TH2* Hist=fHm->H2(HistName.Data());
      //for(Int_t Sample=0;Sample<32;Sample++)
      Hist->Fill(Col,Row);
      
    }
  LOG(debug) << this->GetName () << ": Done Analyzing Signalshape of " << NrDigis
               << " Digis ";

}

