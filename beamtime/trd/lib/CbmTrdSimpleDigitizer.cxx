#include <numeric>
#include "CbmTrdSimpleDigitizer.h"
#include "CbmTrdDigi.h"
#include "CbmTrdAddress.h"

CbmTrdSimpleDigitizer::CbmTrdSimpleDigitizer ()
    : CbmTrdQABase ("CbmTrdSimpleDigitizer"), fDigis(nullptr)
{
}

InitStatus
CbmTrdSimpleDigitizer::Init ()
{
  LOG(debug) << "Initilization of " << this->GetName ();

  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance ();
  assert ( ioman );

  // Get a pointer to the previous already existing data level
  fRaw = static_cast<TClonesArray*> (ioman->GetObject ("SpadicRawMessage"));
  if (!fRaw)
    {
      LOG(fatal) << "No InputDataLevelName SpadicRawMessage array!\n"
                    << this->GetName () << " will be inactive";
      return kERROR;
    }

  // --- Branch for digis
  fDigis = new std::vector<CbmTrdDigi>;
  ioman->RegisterAny("TrdDigi", fDigis,
		     false);

  // Do whatever else is needed at the initilization stage
  // Create histograms to be filled
  // initialize variables
  CbmTrdSimpleDigitizer::CreateHistograms ();

  return kSUCCESS;
}

void
CbmTrdSimpleDigitizer::CreateHistograms ()
{
  for (Int_t layer = 0; layer < fBT->GetNrLayers(); layer++)
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
CbmTrdSimpleDigitizer::Exec (Option_t*)
{
  static Int_t NrTimeslice = 0;
  LOG(info) << this->GetName () << ": Clearing  Digis";
  //  fDigis->Clear ();
  fDigis->clear ();
  UInt_t nSpadicMessages = fRaw->GetEntriesFast (); //SPADIC messages per TimeSlice
  TString Statefullness = "stateless";
  LOG(info) << this->GetName () << ": Estimating Baseline " << Statefullness
               << " Digitizer";
  std::vector<TProfile2D*> Hitmaps;
  for (Int_t layer = 0; layer < fBT->GetNrLayers(); layer++)
    {
      TString HistName = "Baselinemap_Layer_" + std::to_string (layer);
      Hitmaps.push_back (fHm->P2 (HistName.Data ()));
    }
  for (UInt_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
          iSpadicMessage));
      if (!(raw->GetHit ()))
        {
          continue;
        }
      if(raw->GetTriggerType()==1 && (raw->GetSamples()[0]>-200 || raw->GetSamples()[4] >= raw->GetSamples()[5])/*(raw->GetSamples()[0]> raw->GetSamples()[4] || raw->GetSamples()[8]< raw->GetSamples()[4])*/)
	continue;
      Int_t Baseline = raw->GetSamples ()[0];
      if (Baseline < -150)
        Hitmaps.at (GetLayerID (raw))->Fill (GetColumnID (raw), GetRowID (raw),Baseline);
    }
  std::map<UInt_t, boost::circular_buffer<int16_t> > BaselineMap;
  for (UInt_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
          iSpadicMessage));
      if (!(raw->GetHit ()))
        {
          continue;
        }
      auto CurrentBuffer = BaselineMap.find (fBT->GetAddress (raw));
      if (CurrentBuffer == BaselineMap.end ())
        {
          auto& TempBuffer = BaselineMap[fBT->GetAddress (raw)];
          TempBuffer.set_capacity (20);
          CurrentBuffer = BaselineMap.find (fBT->GetAddress (raw));
        }
      if (CurrentBuffer->second.full ())
        continue;
      int16_t BaselineMeasure = fBT->GetBaseline (raw);
      //std::cout << " "<< BaselineMeasure << std::endl;
      if (BaselineMeasure < -170)
        CurrentBuffer->second.push_back (BaselineMeasure);
    }
  TString GraphName = "Digis_Created";
  Int_t NrDigis = 0;
  LOG(info) << this->GetName () << ": Creating Digis with " << Statefullness
               << " Digitizer";
  Float_t*Samples = new Float_t[32];
  for (UInt_t iSpadicMessage = 0; iSpadicMessage < nSpadicMessages;
      ++iSpadicMessage)
    {
      CbmSpadicRawMessage *raw = static_cast<CbmSpadicRawMessage*> (fRaw->At (
	  iSpadicMessage));
      if (!(raw->GetHit ()))
	{
	  continue;
	}
      if (raw->GetTriggerType () == 1
	  && (raw->GetSamples ()[0] > -200
	      || raw->GetSamples ()[4] >= raw->GetSamples ()[5])/*(raw->GetSamples()[0]> raw->GetSamples()[4] || raw->GetSamples()[8]< raw->GetSamples()[4])*/)
	continue;
      Float_t BaselineEstimate =
	  { 0 };
      auto CurrentBuffer = BaselineMap.find (fBT->GetAddress (raw));
      int16_t BaselineMeasure = fBT->GetBaseline (raw);
      if (BaselineMeasure < -200)
	{
	  CurrentBuffer->second.push_back (BaselineMeasure);
	}
      BaselineEstimate = std::accumulate (CurrentBuffer->second.begin (),
					  CurrentBuffer->second.end (), 0L);
      //Div-by-Zero Check, should NEVER occur.
      if (!CurrentBuffer->second.empty ())
	BaselineEstimate /= CurrentBuffer->second.size ();
      //std::cout << BaselineEstimate << std::endl;

      //BaselineEstimate=(fBT->GetBaseline(raw)<BaselineEstimate)?fBT->GetBaseline(raw):BaselineEstimate;
      for (Int_t i = 0; i < 32; i++)
	{
	  if (i < raw->GetNrSamples ())
	    Samples[i] = static_cast<Float_t> (raw->GetSamples ()[i])
	    - BaselineEstimate;
	  else
	    Samples[i] = 0.0;
	}
      UInt_t Address=fBT->GetAddress (raw);
      UInt_t RowID=CbmTrdAddress::GetRowId(Address),
	ColumnID=CbmTrdAddress::GetColumnId(Address);
      UInt_t PadID=RowID*fBT->GetNrColumns(fBT->GetLayerID(raw))+ColumnID;
      fDigis->emplace_back(static_cast<Int_t> (PadID),
			  fBT->GetMaximumAdc (raw,BaselineEstimate)*1.e4,
			  raw->GetFullTime () * fBT->GetSamplingTime(), //65 ns per timestamp
			  raw->GetTriggerType (),
			  1);
      /*
      new ((*fDigis)[NrDigis]) CbmTrdDigi (static_cast<Int_t> (PadID),
					   fBT->GetMaximumAdc (raw,BaselineEstimate)*1.e4,
					   raw->GetFullTime () * fBT->GetSamplingTime(), //65 ns per timestamp
					   raw->GetTriggerType (),
					   1);
      */
      static_cast<CbmTrdDigi> (fDigis->at(NrDigis++)).SetAddress(Address);
      //std::cout<< fBT->GetMaximumAdc (raw,BaselineEstimate)<< std::endl;
      if(fBT->GetMaximumAdc (raw,BaselineEstimate)>512)
	{
	  LOG(info)<< "CbmTrdSimpleDigitizer unusual maxADC";
	  for (int i=0;i<raw->GetNrSamples();i++)
	    {
	      std::cout << raw->GetSamples()[i] << " ";
	    }
	  LOG(info) << "/n";
	  LOG(fatal)<<" KILL";
	}
      /*std::cout << BaselineEstimate << " "<<fBT->GetMaximumAdc(raw,BaselineEstimate)<< " "<<
	  fBT->GetIntegratedCharge (raw, BaselineEstimate)  << std::endl;*/
    }
  delete[] Samples;
  fHm->G1 (GraphName.Data ())->SetPoint (fHm->G1 (GraphName.Data ())->GetN (),
                                         NrTimeslice++, NrDigis);
  //  fDigis->Sort ();
  LOG(info) << this->GetName () << ": Finished creating Digis with "
               << Statefullness << " Digitizer";
}

ClassImp(CbmTrdSimpleDigitizer)
