#include "StsCosyBL.h"

#include <FairRun.h>              // for FairRun
#include <FairTask.h>             // for InitStatus, kSUCCESS, FairTask, kERROR
#include <TAxis.h>                // for TAxis
#include <TFile.h>                // for TFile
#include <TH1.h>                  // for TH1F
#include <TH2.h>                  // for TH2F
#include <TString.h>              // for Form
#include <FairLogger.h>           // for LOG, Logger
#include <stdio.h>                // for sprintf, NULL
#include <iostream>               // for operator<<, endl, basic_ostream, cout
#include "CbmFiberHodoAddress.h"  // for CbmFiberHodoAddress
#include "CbmFiberHodoDigi.h"     // for CbmFiberHodoDigi
#include "CbmStsAddress.h"        // for GetElementId, kStsSide
#include "CbmStsDigi.h"           // for CbmStsDigi
#include "CbmStsSetup.h"          // for CbmStsSetup
#include <FairRootManager.h>      // for FairRootManager
#include <FairRunAna.h>           // for FairRunAna
#include <FairRunOnline.h>        // for FairRunOnline
#include <TClonesArray.h>         // for TClonesArray

using namespace std;


// ---- Default constructor -------------------------------------------
StsCosyBL::StsCosyBL()
  :FairTask("StsCosyBL",1),
   auxDigis(nullptr),
   fDigis(nullptr),
   fBLDigis(nullptr),
   hBLDigis(nullptr),
   hDigis(nullptr),
   cDigis(nullptr),
   chDigis(nullptr),
   fChain(new TChain("cbmsim")),
   outFile(NULL),
   baseline_ch(),
   raw_ch(),
   raw_ch_woBL(),
   calibr_ch(),
   calibr_ch1D(),
   hodo_baseline_ch(),
   hodo_calib_ch(),
   calib(kFALSE),
   hodo_calib(kFALSE),
   fTriggeredMode(kFALSE),
   fTriggeredStation(1),
   fNofEvent(0),
   base_line_array(),
   hodo_BL_array()
{ 
     LOG(debug) << "Default Constructor of StsCosyBL";
}
   
// ---- Destructor ----------------------------------------------------
StsCosyBL::~StsCosyBL()
{
  delete cDigis;
  delete fChain;
  if(fDigis){

  }
  LOG(debug) << "Destructor of StsCosyBL";
}

// ----  Initialisation  ----------------------------------------------
void StsCosyBL::SetParContainers()
{
  LOG(debug) << "SetParContainers of StsCosyBL";
}

// ---- Init ----------------------------------------------------------
InitStatus StsCosyBL::Init()
{
  LOG(debug) << "Initilization of StsCosyBL";
  
  // Get a handle from the IO manager
  FairRootManager* ioman = FairRootManager::Instance();
  
  // Get a pointer to the previous already existing data level
  
  fDigis = ioman->InitObjectAs<std::vector<CbmStsDigi> const *>("StsDigi");
  fBLDigis = ioman->InitObjectAs<std::vector<CbmStsDigi> const *>("StsBaselineDigi");
  hBLDigis = ioman->InitObjectAs<std::vector<CbmFiberHodoDigi> const *>("HodoBaselineDigi");
  hDigis = ioman->InitObjectAs<std::vector<CbmFiberHodoDigi> const *>("HodoDigi");
  auxDigis = ioman->InitObjectAs<std::vector<CbmAuxDigi> const *>("AuxDigi");

  /*
  fDigis =  (TClonesArray*) ioman->GetObject("StsDigi");
  fBLDigis = (TClonesArray*) ioman->GetObject("StsBaselineDigi");
  hBLDigis = (TClonesArray*) ioman->GetObject("HodoBaselineDigi");
  hDigis = (TClonesArray*) ioman->GetObject("HodoDigi");
  auxDigis= (TClonesArray*) ioman->GetObject("AuxDigi");   
  */
  
  if ( ! fDigis ) 
    {
      LOG(error) << "No InputDataLevelName array!\n StsCosyBL will be inactive";
      return kERROR;
    }
  
  calib = kFALSE;
  hodo_calib = kFALSE;
  
  Int_t NStations = 3; // TODO: from where do I get the number of stations?
  Int_t NSides = 2; // TODO: from where do I get the number of sides per station?
  Int_t NStrips = 1024; // TODO: from where do I get the number of strips per station side?
  
  base_line_array.resize(NStations );
  for( UInt_t iStation = 0; iStation <  base_line_array.size(); iStation++ ) 
    {
      base_line_array.at( iStation ).resize( NSides );
      for( UInt_t iSide = 0; iSide <  base_line_array.at( iStation ).size(); iSide++ ) 
	{
	  base_line_array.at( iStation ).at( iSide ).resize( NStrips );
	  for( UInt_t iStrip = 0; iStrip <  base_line_array.at( iStation ).at( iSide ).size(); iStrip++ ) 
	    {
	      base_line_array.at( iStation ).at( iSide ).at(iStrip)=0;
	    }
	}
      }

  Int_t hStations = 2; // TODO: from where do I get the number of stations?
  Int_t hSides = 2; // TODO: from where do I get the number of sides per station?
  Int_t hStrips = 64; // TODO: from where do I get the number of strips per station side?
  
  hodo_BL_array.resize(hStations );
  for( UInt_t iStation = 0; iStation <  hodo_BL_array.size(); iStation++ ) 
    {
      hodo_BL_array.at( iStation ).resize(hSides );
      for( UInt_t iSide = 0; iSide <  hodo_BL_array.at( iStation ).size(); iSide++ ) 
	{
	  hodo_BL_array.at( iStation ).at( iSide ).resize( hStrips );
	  for( UInt_t iStrip = 0; iStrip <  hodo_BL_array.at( iStation ).at( iSide ).size(); iStrip++ ) 
	    {
	      hodo_BL_array.at( iStation ).at( iSide ).at(iStrip)=0;
	    }
	}
    }
  
  
  for(int i =0; i<3;i++)
    {
      for(int s=0; s<2; s++)
	{
	  Char_t inName[200];
	  sprintf(inName,"base_line_vs_channel_STS%i_side%i",i,s);	  
	  baseline_ch[i][s] = new TH2F(inName, inName, 300, 0,300, 2000, -10, 3000);

	  sprintf(inName,"raw_charge_vs_channel_STS%i_side%i_ifBL",i,s);
	  raw_ch[i][s] = new TH2F(inName, inName, 300, 0,300, 2000, -10, 3000);

	  sprintf(inName,"raw_charge_vs_channel_STS%i_side%i_all",i,s);
	  raw_ch_woBL[i][s] = new TH2F(inName, inName, 300, 0,300, 2000, -10, 3000);

	  sprintf(inName,"calibr_charge_vs_channel_STS%i_side%i",i,s);
	  calibr_ch[i][s] = new TH2F(inName, inName, 300, 0,300, 2000, -10, 3000);
	  
	  sprintf(inName,"calibr_charge_vs_channel_STS%i_side%i_1D",i,s);
	  calibr_ch1D[i][s] = new TH1F(inName, inName, 2000, -10, 3000);
	}
    }
    
  for(int i =0; i<2;i++)
    {
      for(int s=0; s<2; s++)
	{
	  Char_t inName[200];
	  sprintf(inName,"base_line_vs_channel_Hodo%i_side%i",i,s);
	  hodo_baseline_ch[i][s] = new TH2F(inName, inName, kBaselineNBins, kBaselineMinAdc, kBaselineMaxAdc, 2000, -10, 3000);
	  
	  sprintf(inName,"calibr_charge_vs_channel_Hodo%i_side%i",i,s);
	  hodo_calib_ch[i][s] = new TH2F(inName, inName, kBaselineNBins, kBaselineMinAdc, kBaselineMaxAdc, 2000, -10, 3000);
	}
    }
  
  fNofEvent = 0;

  cDigis = new std::vector<CbmStsDigi>;
  ioman->RegisterAny("StsCalibDigi", cDigis,
                     IsOutputBranchPersistent("StsCalibDigi"));

  chDigis = new std::vector<CbmFiberHodoDigi>;
  ioman->RegisterAny("HodoCalibDigi", chDigis,
                     IsOutputBranchPersistent("HodoCalibDigi"));

  /*  
  cDigis = RegisterOutput<CbmStsDigi>("StsCalibDigi");
  chDigis = RegisterOutput<CbmFiberHodoDigi>("HodoCalibDigi");

  cDigis = new TClonesArray("CbmStsDigi", 100);
  ioman->Register("StsCalibDigi", "Calibrated", cDigis, IsOutputBranchPersistent("StsCalibDigi"));
  chDigis = new TClonesArray("CbmFiberHodoDigi", 100);
  ioman->Register("HodoCalibDigi", "HodoCalibrated", chDigis, IsOutputBranchPersistent("HodoCalibDigi"));
  */
  return kSUCCESS;
  
}

// ---- ReInit  -------------------------------------------------------
InitStatus StsCosyBL::ReInit()
{
  LOG(debug) << "Reinitilization of StsCosyBL";
  return kSUCCESS;
}

// ---- Exec ----------------------------------------------------------
void StsCosyBL::Exec(Option_t*)
{

  /*
    cDigis->Clear();
    chDigis->Clear();
  */
  cDigis->clear();
  chDigis->clear();
  
  
//  int fNDigis =0;
//  int hNDigis =0;
  
  
  {
    Int_t nofSTS = fDigis->size();
    const CbmStsDigi* StsDigi = NULL;
    for (Int_t iDigi=0; iDigi < nofSTS; iDigi++ ) 
      {
	StsDigi = &(fDigis->at(iDigi));
	int station = CbmStsSetup::Instance()->GetStationNumber(StsDigi->GetAddress());
	int side = CbmStsAddress::GetElementId(StsDigi->GetAddress(),kStsSide);
	int ch = StsDigi->GetChannel();
	raw_ch_woBL[station][side]->Fill(ch,StsDigi->GetCharge());
      }    
  }
  
  if (0 < fBLDigis->size() && 0 < hBLDigis->size() ) {
    BaseLine(fBLDigis, base_line_array);      
    HodoBaseLine(hBLDigis,hodo_BL_array);
  } else {
    if(calib==kTRUE && hodo_calib ==kTRUE) {    
      Int_t nofSTS = fDigis->size();
      Int_t nofHodo = hDigis->size();
      
      const CbmStsDigi* StsDigi = nullptr;
      const CbmFiberHodoDigi* HodoDigi = nullptr;
      for (Int_t iDigi=0; iDigi < nofHodo; iDigi++ ) {
	HodoDigi = &(hDigis->at(iDigi));
	      
	Int_t system = CbmFiberHodoAddress::GetStationId(HodoDigi->GetAddress());
	Int_t layer = CbmFiberHodoAddress::GetSideId(HodoDigi->GetAddress());
	Int_t fiberNr = CbmFiberHodoAddress::GetStripId( HodoDigi->GetAddress());
	
	double adc = -HodoDigi->GetCharge() + hodo_BL_array.at(system).at(layer).at(fiberNr);
	
	hodo_baseline_ch[system][layer] ->Fill(fiberNr, hodo_BL_array.at(system).at(layer).at(fiberNr));
	hodo_calib_ch[system][layer]->Fill(fiberNr,adc);
	
	if(adc>0) {

	  chDigis->emplace_back(HodoDigi->GetAddress(), adc,HodoDigi->GetTime());
				/*
	  new ( (*chDigis)[hNDigis] )CbmFiberHodoDigi(HodoDigi->GetAddress(), adc,HodoDigi->GetTime());
	  hNDigis++;
				*/
	}
	else return;
	
      }
      
      //	  int sts[3]={0,0,0};
      //	  int side_sts[3][2]={0,0,0,0,0,0};
      
      for (Int_t iDigi=0; iDigi < nofSTS; iDigi++ ) {
	StsDigi = &(fDigis->at(iDigi));
	      
	int station = CbmStsSetup::Instance()->GetStationNumber(StsDigi->GetAddress());
	int side = CbmStsAddress::GetElementId(StsDigi->GetAddress(),kStsSide);
	int ch = StsDigi->GetChannel();

	double adc = -StsDigi->GetCharge() + base_line_array.at(station).at(side).at(ch);
				
	raw_ch[station][side]->Fill(ch,StsDigi->GetCharge());
	baseline_ch[station][side] ->Fill(ch, base_line_array.at(station).at(side).at(ch));
	
	//if(station==fTriggeredStation && (ch < 21 || ch > 27))continue; //cut strip area
	
	calibr_ch[station][side]->Fill(ch,(UShort_t)adc);
	calibr_ch1D[station][side]->Fill((UShort_t)adc);
	
	if(fTriggeredMode && station==fTriggeredStation) {
          cDigis->emplace_back(StsDigi->GetAddress(),
			       StsDigi->GetChannel(),
			       StsDigi->GetTime(), adc);
	  /*
	  new ( (*cDigis)[fNDigis] ) CbmStsDigi(StsDigi->GetAddress(),
						StsDigi->GetChannel(),
						StsDigi->GetTime(), adc);
	  fNDigis++;
	  */
	} else if(adc>0) {
          cDigis->emplace_back(StsDigi->GetAddress(),
			       StsDigi->GetChannel(),
			       StsDigi->GetTime(), (UShort_t)adc);
	  /*
	  new ( (*cDigis)[fNDigis] ) CbmStsDigi(StsDigi->GetAddress(),
						StsDigi->GetChannel(),
						StsDigi->GetTime(), (UShort_t)adc);
	  fNDigis++;
	  */
	}
	else return;
      }
      
    }
  }

    
  LOG(debug) << "Exec of StsCosyBL";
//  Reset();
}

 void StsCosyBL::FinishEvent()
 {
   //Reset();
   // cout << "---I ------------- FinishEvent" << endl;
}




// ---- Finish --------------------------------------------------------
void StsCosyBL::Finish()
{
  LOG(debug) << "Finish of StsCosyBL";
  
  // Write standard file
  FairRun* ana = FairRunAna::Instance();
  if ( ! ana ) {
    ana = FairRunOnline::Instance();
  }
  TFile* fout = ana->GetOutputFile();
  
  for(int i =0; i<3;i++) {
    for(int s=0; s<2; s++) {
      baseline_ch[i][s]->Write();
      raw_ch_woBL[i][s]->Write();
      raw_ch[i][s]->Write();
      calibr_ch[i][s]->Write();
      calibr_ch1D[i][s]->Write();
    }
  }
  
  for(int i =0; i<2;i++) {
    for(int s=0; s<2; s++) {
      hodo_baseline_ch[i][s]->Write();
      hodo_calib_ch[i][s]->Write();
    }
  }
  
  if (fout) fout->Write();
  
  LOG(info);
  LOG(info) << "=====================================================";
  LOG(info) << GetName() << ": Finish";
  LOG(info);
  LOG(info) << "=====================================================";
  LOG(info);
  
}
	   

void StsCosyBL::BaseLine(const std::vector<CbmStsDigi>* fBaselineDigis, vector< vector < vector < double> > >)
{

  //BLInit();
  Int_t kNStations = 3; // TODO: from where do I get the number of stations?
  Int_t kNSides = 2; // TODO: from where do I get the number of sides per station?
  Int_t kNStrips = 1024; // TODO: from where do I get the number of strips per station side?
  
  vector< vector < vector < TH1F * > > > fBaselines;
  fBaselines.resize( kNStations );
  
  for( UInt_t iStation = 0; iStation <  fBaselines.size(); iStation++ ) 
    {
      fBaselines.at( iStation ).resize( kNSides );
      for( UInt_t iSide = 0; iSide < fBaselines.at( iStation ).size(); iSide++ ) 
	{
	  fBaselines.at( iStation ).at( iSide ).resize( kNStrips );
	  for( UInt_t iStrip = 0; iStrip < fBaselines.at( iStation ).at( iSide ).size(); iStrip++ ) 
	    {
	      const char * nametitle = Form( "blhist_sta%d_side%d_str%d", iStation, iSide, iStrip );
	      fBaselines.at( iStation ).at( iSide ).at( iStrip ) = new TH1F( nametitle, nametitle, kBaselineNBins, kBaselineMinAdc, kBaselineMaxAdc );
	    }
	}
    }
    
  cout << "---------CALIBRATION ---------ON ------------------- " << endl;
  calib=kTRUE;
  Int_t nBaselineEntries = fBaselineDigis->size();
  if( nBaselineEntries ) 
    { // TODO: Check here in a proper way if the event is a baseline event
      
      /* Baseline data should be taken from only one iteration of the baseline measurement
       * (i.e. data from several different iterations of baseline calibration should not be mixed up)
       * Therefore the baseline histograms should be zeroed before filling them with the data of the
       * next calibration iteration */
      for( UInt_t iStation = 0; iStation <  fBaselines.size(); iStation++ ) 
	{
	  for( UInt_t iSide = 0; iSide < fBaselines.at( iStation ).size(); iSide++ ) 
	    {
	      for( UInt_t iStrip = 0; iStrip < fBaselines.at( iStation ).at( iSide ).size(); iStrip++ ) 
		{
		  fBaselines.at( iStation ).at( iSide ).at( iStrip )->Reset();
		}
	    }
	}
      
      for( Int_t iDigi = 0; iDigi < nBaselineEntries; ++iDigi ) 
	{
	  const CbmStsDigi * digi = &(fBaselineDigis->at(iDigi));
	  Int_t station = CbmStsSetup::Instance()->GetStationNumber( digi->GetAddress() );
	  Int_t side = CbmStsAddress::GetElementId( digi->GetAddress(), kStsSide );
	  Int_t strip = digi->GetChannel();
	  Double_t adc = digi->GetCharge();
	  fBaselines.at( station ).at( side ).at( strip )->Fill( adc );

	}
      
      for( UInt_t iStation = 0; iStation <  fBaselines.size(); iStation++ ) 
	{
	  for( UInt_t iSide = 0; iSide < fBaselines.at( iStation ).size(); iSide++ ) 
	    {
	      for( UInt_t iStrip = 0; iStrip < fBaselines.at( iStation ).at( iSide ).size(); iStrip++ ) 
		{
		  TH1F * blHist = fBaselines.at( iStation ).at( iSide ).at( iStrip );
		  if( blHist->GetEntries() ) 
		    {
		      Double_t bl = GetBlPos( blHist );
		      base_line_array.at(iStation).at(iSide).at(iStrip)=bl;
		      //	    LOG(info) << "Baseline of station " << iStation << " side " << iSide << " strip " << iStrip << " is " << base_line_array.at(iStation).at(iSide).at(iStrip)
		      //;
		    }
		  if(blHist)
		    {
		      blHist->Delete();
		    }
		}
	    }
	}
    }
  // BLClean();
 cout << "---------CALIBRATION ---------OFF ------------------- " << endl;  
 
}

void StsCosyBL::HodoBaseLine(const std::vector<CbmFiberHodoDigi>* fBaselineDigis, vector< vector < vector < double> > >)
{

  Int_t hStations = 2; // TODO: from where do I get the number of stations?
  Int_t hSides = 2; // TODO: from where do I get the number of sides per station?
  Int_t hStrips = 64; // TODO: from where do I get the number of strips per station side?
  vector< vector < vector < TH1F * > > > fBaselines;
  
  fBaselines.resize(hStations );
  for( UInt_t iStation = 0; iStation <  fBaselines.size(); iStation++ ) 
    {
      fBaselines.at( iStation ).resize(hSides );
      for( UInt_t iSide = 0; iSide <  fBaselines.at( iStation ).size(); iSide++ ) 
	{
	  fBaselines.at( iStation ).at( iSide ).resize( hStrips );
	  for( UInt_t iStrip = 0; iStrip <  fBaselines.at( iStation ).at( iSide ).size(); iStrip++ ) 
	    {
	      const char * nametitle = Form( "blhist_sta%d_side%d_str%d", iStation, iSide, iStrip );
	      fBaselines.at( iStation ).at( iSide ).at(iStrip)=new TH1F( nametitle, nametitle, kBaselineNBins, kBaselineMinAdc, kBaselineMaxAdc );
	    }
	}
    }

  cout << "---------HODO------------CALIBRATION ---------ON ------------------- " << endl;
  hodo_calib=kTRUE;
  Int_t nBaselineEntries = fBaselineDigis->size();
  if( nBaselineEntries ) 
    { // TODO: Check here in a proper way if the event is a baseline event
      
      /* Baseline data should be taken from only one iteration of the baseline measurement
       * (i.e. data from several different iterations of baseline calibration should not be mixed up)
       * Therefore the baseline histograms should be zeroed before filling them with the data of the
       * next calibration iteration */
      for( UInt_t iStation = 0; iStation <  fBaselines.size(); iStation++ ) 
	{
	  for( UInt_t iSide = 0; iSide < fBaselines.at(iStation).size(); iSide++ ) 
	    {
	      for( UInt_t iStrip = 0; iStrip < fBaselines.at(iStation).at( iSide ).size(); iStrip++ ) 
		{
		  fBaselines.at(iStation).at( iSide ).at( iStrip )->Reset();
		}
	    }
	}
      
      for( Int_t iDigi = 0; iDigi < nBaselineEntries; ++iDigi ) 
	{
	  const CbmFiberHodoDigi * digi = &(fBaselineDigis->at(iDigi));
	  Int_t station = CbmFiberHodoAddress::GetStationId(digi->GetAddress());
	  Int_t side = CbmFiberHodoAddress::GetSideId(digi->GetAddress());
	  Int_t strip = CbmFiberHodoAddress::GetStripId(digi->GetAddress());

	  Double_t adc = digi->GetCharge();
	  fBaselines.at(station).at( side ).at( strip )->Fill( adc );
	  
	}
      for( UInt_t iStation = 0; iStation <  fBaselines.size(); iStation++ ) 
	{
	  for( UInt_t iSide = 0; iSide < fBaselines.at(iStation).size(); iSide++ ) 
	    {
	      for( UInt_t iStrip = 0; iStrip < fBaselines.at(iStation).at( iSide ).size(); iStrip++ ) 
		{
		  TH1F * blHist = fBaselines.at(iStation).at( iSide ).at( iStrip );
		  if( blHist->GetEntries() ) 
		    {
		      Double_t bl = GetBlPos( blHist );
		      hodo_BL_array.at(iStation).at(iSide).at(iStrip)=bl;
		      //		  if(iSide ==0 || iSide ==1 || iSide==16 ||iSide ==17)
		      //		    LOG(info) << "Baseline of hodo " <<  " layer " << iSide << " strip " << iStrip << " is " << hodo_BL_array.at(iSide).at(iStrip);
		    }
		  if(blHist)
		    {
		      blHist->Delete();
		    }
		}
	    }
	}
    }
  // BLClean();
  cout << "---------HODO-------------CALIBRATION ---------OFF ------------------- " << endl;  
  
}

Double_t StsCosyBL::GetBlPos( TH1F * hist )
{
  Int_t medianBin = 1;
  
  Double_t * integral = hist->GetIntegral();
  while( integral[ medianBin + 1 ] <= 0.5 ) {
    medianBin++;
  }
  
  return hist->GetXaxis()->GetBinCenter( medianBin );
}

void  StsCosyBL::Reset()
{
  /*
  if (cDigis) {
    cDigis->Clear();
  }
  if (chDigis) {
    chDigis->Clear();
  }
  */
  if (cDigis) {
    cDigis->clear();
  }
  if (chDigis) {
    chDigis->clear();
  }
}

ClassImp(StsCosyBL)
