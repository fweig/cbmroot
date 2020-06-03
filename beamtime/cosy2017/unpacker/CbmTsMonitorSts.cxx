// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                        CbmTsMonitorSts                            -----
// -----               Created 08.03.2017 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTsMonitorSts.h"
#include "CbmStsUnpackPar.h"

#include "CbmStsAddress.h"
#include "CbmHistManager.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairRunOnline.h"

#include "TClonesArray.h"
#include "TString.h"
#include "TRandom.h"
#include "THttpServer.h"
#include "TROOT.h"
#include <TFile.h>

#include <iostream>
#include <stdint.h>
#include <iomanip>

using std::hex;
using std::dec;
using namespace std;

Bool_t bResetStsHistos = kFALSE;
Bool_t bSaveStsHistos  = kFALSE;
Bool_t bUpdateAdcHistosSts = kFALSE;

CbmTsMonitorSts::CbmTsMonitorSts()
  : CbmTSUnpack(),
   fNrOfNdpbs(0),
   fNrOfNdpbsA(0),
   fNrOfNdpbsB(0),
   fNrOfFebsPerNdpb(0),
   fMsgCounter(11,0), // length of enum MessageTypes initialized with 0
   fNdpbIdIndexMap(),
   fHM(new CbmHistManager()),
   fCurrentEpoch(),
   fNofEpochs(0),
   fCurrentEpochTime(0.),
   fdStartTime( -1 ),
   fdStartTimeMsSz(-1.),
   fcMsSizeAll(NULL),
   fEquipmentId(0),
   fUnpackPar(NULL),
   fFebRateDate_nDPB(),
   fHitDtDate_nDPB(),
   fHitDt_nDPB(),
   fdLastHitTime_nDPB(),
   fiRunStartDateTimeSec( -1 ),
   fiBinSizeDatePlots( -1 ),
   fHistMessType(NULL),
   fHistSysMessType(NULL),
   fChan_Counts_Sts(),
   fRaw_ADC_Sts(),
   fFebRate(),
   fHitMissEvo(),
   fADC_Mean_Sts(),
   fADC_Rms_Sts(),
   fADC_Skew_Sts(),
   fADC_Kurt_Sts()
{
}

CbmTsMonitorSts::~CbmTsMonitorSts()
{
}

Bool_t CbmTsMonitorSts::Init()
{
  LOG(info) << "Initializing flib nxyter unpacker for STS";

  FairRootManager* ioman = FairRootManager::Instance();
  if (ioman == NULL) {
    LOG(fatal) << "No FairRootManager instance";
  }

  return kTRUE;
}

void CbmTsMonitorSts::SetParContainers()
{
	LOG(info) << "Setting parameter containers for " << GetName();
	fUnpackPar = (CbmStsUnpackPar*)(FairRun::Instance()->GetRuntimeDb()->getContainer("CbmStsUnpackPar"));
}


Bool_t CbmTsMonitorSts::InitContainers()
{
	LOG(info) << "Init parameter containers for " << GetName();

   Bool_t bReInit = ReInitContainers();
   CreateHistograms();

   return bReInit;
}

Bool_t CbmTsMonitorSts::ReInitContainers()
{
	LOG(info) << "ReInit parameter containers for " << GetName();

   fNrOfNdpbsA = fUnpackPar->GetNrOfnDpbsModA();
   fNrOfNdpbsB = fUnpackPar->GetNrOfnDpbsModB();
   fNrOfNdpbs = fNrOfNdpbsA + fNrOfNdpbsB;
   fNrOfFebsPerNdpb = fUnpackPar->GetNrOfFebsPerNdpb();

	LOG(info) << "Nr. of nDPBs Mod. A: " << fNrOfNdpbsA;

	LOG(info) << "Nr. of nDPBs Mod. B: " << fNrOfNdpbsB;

   fNdpbIdIndexMap.clear();
   for (Int_t i = 0; i< fNrOfNdpbsA; ++i)
   {
     fNdpbIdIndexMap[fUnpackPar->GetNdpbIdA(i)] = i;
     LOG(info) << "nDPB Id of STS A " << i
               << " : 0x" << std::hex << fUnpackPar->GetNdpbIdA(i)
               << std::dec;
   } // for (Int_t i = 0; i< NrOfnDpbsModA; ++i)
   for (Int_t i = 0; i< fNrOfNdpbsB; ++i)
   {
     fNdpbIdIndexMap[fUnpackPar->GetNdpbIdB(i)] = i + fNrOfNdpbsA;
     LOG(info) << "nDPB Id of STS B " << i
               << " : 0x" << std::hex << fUnpackPar->GetNdpbIdB(i)
               << std::dec;
   } // for (Int_t i = 0; i< NrOfnDpbsModB; ++i)

	Int_t NrOfFebs = fUnpackPar->GetNrOfFebs();

	LOG(info) << "Nr. of FEBs : " << NrOfFebs;

	Int_t NrOfChannels = fUnpackPar->GetNrOfChannels();
	LOG(info) << "Nr. of Channels : " << NrOfChannels;

	return kTRUE;
}

void CbmTsMonitorSts::CreateHistograms()
{
    THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   TString sHistName{""};
   TString title{""};

   sHistName = "hMessageTypeSts";
   title = "Nb of message for each type; Type";
   TH1I* hMessageType = new TH1I(sHistName, title, 16, 0., 16.);
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_NOP,      "NOP");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_HIT,      "HIT");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_EPOCH,    "EPOCH");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_SYNC,     "SYNC");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_AUX,      "AUX");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_EPOCH2,   "EPOCH2");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4,     "GET4");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_SYS,      "SYS");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_SLC, "MSG_GET4_SLC");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_32B, "MSG_GET4_32B");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_GET4_SYS, "MSG_GET4_SYS");
   hMessageType->GetXaxis()->SetBinLabel(1 + 15, "GET4 Hack 32B");
   hMessageType->GetXaxis()->SetBinLabel(1 + ngdpb::MSG_NOP,      "NOP");
   fHM->Add(sHistName.Data(), hMessageType);
   if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));

   sHistName = "hSysMessTypeSts";
   title = "Nb of system message for each type; System Type";
   TH1I* hSysMessType = new TH1I(sHistName, title, 17, 0., 17.);
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_START,       "DAQ START");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_FINISH,      "DAQ FINISH");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_NX_PARITY,       "NX PARITY");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_SYNC_PARITY,     "SYNC PARITY");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_DAQ_RESUME,      "DAQ RESUME");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_FIFO_RESET,      "FIFO RESET");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_USER,            "USER");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_PCTIME,          "PCTIME");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_ADC,             "ADC");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_PACKETLOST,      "PACKET LOST");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_GET4_EVENT,      "GET4 ERROR");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_CLOSYSYNC_ERROR, "CLOSYSYNC ERROR");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_TS156_SYNC,        "TS156 SYNC");
   hSysMessType->GetXaxis()->SetBinLabel(1 + ngdpb::SYSMSG_GDPB_UNKWN,        "UNKW GET4 MSG");
   hSysMessType->GetXaxis()->SetBinLabel(1 + 16, "GET4 Hack 32B");
   fHM->Add(sHistName.Data(), hSysMessType);
   if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));

   // Number of rqte bins =
   //      9 for the sub-unit decade
   //    + 9 for each unit of each decade * 10 for the subdecade range
   //    + 1 for the closing bin top edge
   const Int_t iNbDecadesRate    = 9;
   const Int_t iNbStepsDecade    = 9;
   const Int_t iNbSubStepsInStep = 10;
   const Int_t iNbBinsRate = iNbStepsDecade
                           + iNbStepsDecade * iNbSubStepsInStep * iNbDecadesRate
                           + 1;
   Double_t dBinsRate[iNbBinsRate];
   Double_t dBinsDt[iNbBinsRate];
      // First fill sub-unit decade
   for( Int_t iSubU = 0; iSubU < iNbStepsDecade; iSubU ++ )
   {
      dBinsRate[ iSubU ] = 0.1 * ( 1 + iSubU );
      dBinsDt[ iSubU ] = 10 * dBinsRate[ iSubU ] ;
   } // for( Int_t iSubU = 0; iSubU < iNbStepsDecade; iSubU ++ )
   std::cout << std::endl;
      // Then fill the main decades
   Double_t dSubstepSize = 1.0 / iNbSubStepsInStep;
   for( Int_t iDecade = 0; iDecade < iNbDecadesRate; iDecade ++)
   {
      Double_t dBase = std::pow( 10, iDecade );
      Int_t iDecadeIdx = iNbStepsDecade
                       + iDecade * iNbStepsDecade * iNbSubStepsInStep;
      for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
      {
         Int_t iStepIdx = iDecadeIdx + iStep * iNbSubStepsInStep;
         for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
         {
            dBinsRate[ iStepIdx + iSubStep ] = dBase * (1 + iStep)
                                             + dBase * dSubstepSize * iSubStep;
            dBinsDt[ iStepIdx + iSubStep ] = 10 * dBinsRate[ iStepIdx + iSubStep ] ;
         } // for( Int_t iSubStep = 0; iSubStep < iNbSubStepsInStep; iSubStep++ )
      } // for( Int_t iStep = 0; iStep < iNbStepsDecade; iStep++ )
   } // for( Int_t iDecade = 0; iDecade < iNbDecadesRate; iDecade ++)
   dBinsRate[ iNbBinsRate - 1 ] = std::pow( 10, iNbDecadesRate );
   dBinsDt[ iNbBinsRate - 1 ] = 10 * dBinsRate[ iNbBinsRate - 1 ] ;

   TString sNdpbTag = "";
	TString sDateHistName{""};
   for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   {// looping on all the nDPBS IDs

      if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
      {
         sNdpbTag = Form("%04X", fUnpackPar->GetNdpbIdA(dpbId) );
      } // if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
         else
         {
            sNdpbTag = Form("%04X", fUnpackPar->GetNdpbIdB(dpbId - fNrOfNdpbsA) );
         } // else of if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )

      for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
      {// looping on all the FEB IDs
         sHistName = Form("Chan_Counts_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         title = Form("Channel counts Sts nDPB %s FEB %02u; channel; Counts",
                     sNdpbTag.Data(), febId);
		  fHM->Add( sHistName.Data(), new TH1F( sHistName.Data(), title.Data(), kiNbChanFebF, 0, kiNbChanFebF) );
        if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));

        sHistName = Form("Raw_ADC_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
        title = Form("Raw ADC Sts nDPB %s FEB %02u; channel; ADC value",
               sNdpbTag.Data(), febId);
		  fHM->Add( sHistName.Data(), new TH2F( sHistName.Data(), title.Data(),
                                              kiNbChanFebF, 0, kiNbChanFebF, 4096, 0, 4096) );
        if (server) server->Register("/StsRaw", fHM->H2(sHistName.Data()));

        sHistName = Form("ADC_Mean_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
        title = Form("ADC Mean Sts nDPB %s FEB %02u; channel; ADC Mean value",
               sNdpbTag.Data(), febId);
		  fHM->Add( sHistName.Data(), new TH1F( sHistName.Data(), title.Data(), kiNbChanFebF, 0, kiNbChanFebF ) );
        if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));
        sHistName = Form("ADC_Rms_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
        title = Form("ADC RMS Sts nDPB %s FEB %02u; channel; ADC RMS value",
               sNdpbTag.Data(), febId);
		  fHM->Add( sHistName.Data(), new TH1F( sHistName.Data(), title.Data(), kiNbChanFebF, 0, kiNbChanFebF ) );
        if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));
        sHistName = Form("ADC_Skew_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
        title = Form("ADC Skewness Sts nDPB %s FEB %02u; channel; ADC Skewness value",
               sNdpbTag.Data(), febId);
		  fHM->Add( sHistName.Data(), new TH1F( sHistName.Data(), title.Data(), kiNbChanFebF, 0, kiNbChanFebF ) );
        if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));
        sHistName = Form("ADC_Kurt_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
        title = Form("ADC Kurtosis Sts nDPB %s FEB %02u; channel; ADC Kurtosis value",
               sNdpbTag.Data(), febId);
		  fHM->Add( sHistName.Data(), new TH1F( sHistName.Data(), title.Data(), kiNbChanFebF, 0, kiNbChanFebF ) );
        if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));

        sHistName = Form("FebRate_n%s_f%1u", sNdpbTag.Data(), febId);
        title = Form("Counts per second in nDPB %s FEB %02u; Time[s] ; Counts",
                        sNdpbTag.Data(), febId);
		  fHM->Add( sHistName.Data(), new TH1F( sHistName.Data(), title.Data(), 1800, 0, 1800 ) );
        if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));

        if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec ) {
          sDateHistName = Form("FebRateDate_n%s_f%1u", sNdpbTag.Data(), febId);
          fHM->Add(sDateHistName.Data(), new TH1F(sDateHistName.Data(), title.Data(),
                        (5400 / fiBinSizeDatePlots),
                        fiRunStartDateTimeSec -10, fiRunStartDateTimeSec + 5400 - 10));
          ( fHM->H1(sDateHistName.Data()) )->GetXaxis()->SetTimeDisplay(1);
          if (server)
            server->Register("/StsRaw", fHM->H1(sDateHistName.Data()));

          sHistName = Form("HitDtDate_n%s_f%1u", sNdpbTag.Data(), febId );
          title = Form("Inverse Hit distance VS time in second in nDPB %s FEB %02u; Time[s] ; F [Hz]; Counts",
                     sNdpbTag.Data(), febId );
          fHM->Add(sHistName.Data(), new TH2F(sHistName.Data(), title.Data(),
                        (5400 / 2*fiBinSizeDatePlots),
                        fiRunStartDateTimeSec -10, fiRunStartDateTimeSec + 5400 - 10,
                        iNbBinsRate - 1, dBinsRate ));
          ( fHM->H2(sHistName.Data()) )->GetXaxis()->SetTimeDisplay(1);
          if (server)
            server->Register("/StsRaw", fHM->H2(sHistName.Data()));

          sHistName = Form("HitDt_n%s_f%1u", sNdpbTag.Data(), febId );
          title = Form("Hit distance in nDPB %s FEB %02u; dT [ns]; Counts",
                     sNdpbTag.Data(), febId );
          fHM->Add(sHistName.Data(), new TH1F(sHistName.Data(), title.Data(),
                        iNbBinsRate - 1, dBinsDt )); // 1ns to 10s
          if (server)
            server->Register("/StsRaw", fHM->H1(sHistName.Data()));
        } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )

        sHistName = Form("HitMissEvo_n%s_f%1u", sNdpbTag.Data(), febId);
        title = Form("Minimal hit loss per second in nDPB %s FEB %02u; Time[s] ; Min Loss",
                        sNdpbTag.Data(), febId);
		  fHM->Add( sHistName.Data(), new TH1F( sHistName.Data(), title.Data(), 1800, 0, 1800 ) );
        if (server) server->Register("/StsRaw", fHM->H1(sHistName.Data()));

      } // for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
   } // for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)

	sHistName = "Pad_Distribution";
	title = "Pad_Distribution; Sectors in Horizontal Direction; Channels in Vertical Direction";
	fHM->Add( sHistName.Data(), new TH2F(sHistName.Data(), title.Data(), 79, -0.5, 78.5, 23, -0.5, 22.5) );
   if (server) server->Register("/StsRaw", fHM->H2(sHistName.Data()));

   if (server)
   {
      server->RegisterCommand("/Reset_All_Sts", "bResetStsHistos=kTRUE");
      server->Restrict("/Reset_All_Sts", "allow=admin");

      server->RegisterCommand("/Save_All_Sts", "bSaveStsHistos=kTRUE");
      server->Restrict("/Save_All_Sts", "allow=admin");

      server->RegisterCommand("/Updt_Adc_Sts", "bUpdateAdcHistosSts=kTRUE");
      server->Restrict("/Updt_Adc_Sts", "allow=admin");
   } // if (server)

   /** Create summary Canvases for CERN 2016 **/
   Double_t w = 10;
   Double_t h = 10;
   Int_t iNbPadsPerDpb = fNrOfFebsPerNdpb/2 + fNrOfFebsPerNdpb%2;

   TCanvas* cStsChCounts = new TCanvas("cStsChCounts", "STS Channels counts", w, h);
   cStsChCounts->Divide( fNrOfNdpbs/2 + fNrOfNdpbs%2, fNrOfFebsPerNdpb );

   TCanvas* cStsFebRate = new TCanvas("cStsFebRate", "STS FEB rate", w, h);
   cStsFebRate->Divide( fNrOfNdpbs/2 + fNrOfNdpbs%2, fNrOfFebsPerNdpb );

   TH1* histPnt = NULL;
   for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   {// looping on all the nDPBS IDs
      if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
      {
         sNdpbTag = Form("%04X", fUnpackPar->GetNdpbIdA(dpbId) );
      } // if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
         else
         {
            sNdpbTag = Form("%04X", fUnpackPar->GetNdpbIdB(dpbId - fNrOfNdpbsA) );
         } // else of if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )

      for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
      {// looping on all the FEB IDs
         cStsChCounts->cd( 1 + dpbId * iNbPadsPerDpb + febId/2 );
         gPad->SetLogy();
         sHistName = Form("Chan_Counts_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());

         if( 0 == febId%2 )
         {
            histPnt->SetLineColor( kRed );  // => Change color for 1st of the 2/pad!
            histPnt->Draw();
         } // if( 0 == febId%2 )
            else
            {
               histPnt->SetLineColor( kBlue );  // => Change color for 1nd of the 2/pad!
               histPnt->Draw("same");
            } // if( 0 == febId%2 )

         cStsFebRate->cd( 1 + dpbId * iNbPadsPerDpb + febId/2 );
         gPad->SetLogy();
         sHistName = Form("FebRate_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());

         if( 0 == febId%2 )
         {
            histPnt->SetLineColor( kRed );  // => Change color for 1st of the 2/pad!
            histPnt->Draw();
         } // if( 0 == febId%2 )
            else
            {
               histPnt->SetLineColor( kBlue );  // => Change color for 1nd of the 2/pad!
               histPnt->Draw("same");
            } // if( 0 == febId%2 )

         // ADC statistical properties per channel
         /// Maybe use some option to switch these canvases ON/OFF
         TCanvas* cStsFebAdcStats = new TCanvas( Form("AdcStats_n%s_f%1u", sNdpbTag.Data(), febId),
                                                  Form("ADC statistical properties n%s f%1u", sNdpbTag.Data(), febId),
                                                  w, h);
//         cStsFebAdcStats->Divide( 2, 3 );
         cStsFebAdcStats->Divide( 2, 2 );

         cStsFebAdcStats->cd(1);
         gPad->SetGridx();
         gPad->SetGridy();
         gPad->SetLogy();
         sHistName = Form("FebRate_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());
         histPnt->Draw();

         cStsFebAdcStats->cd(2);
         gPad->SetGridx();
         gPad->SetGridy();
         gPad->SetLogz();
         sHistName = Form("Raw_ADC_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H2(sHistName.Data());
         histPnt->Draw("colz");

         cStsFebAdcStats->cd(3);
         gPad->SetGridx();
         gPad->SetGridy();
         sHistName = Form("ADC_Mean_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());
         histPnt->Draw("hist");

         cStsFebAdcStats->cd(4);
         gPad->SetGridx();
         gPad->SetGridy();
         sHistName = Form("ADC_Rms_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());
         histPnt->Draw("hist");
/*
         cStsFebAdcStats->cd(5);
         gPad->SetGridx();
         gPad->SetGridy();
         sHistName = Form("ADC_Skew_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());
         histPnt->Draw("hist");

         cStsFebAdcStats->cd(6);
         gPad->SetGridx();
         gPad->SetGridy();
         sHistName = Form("ADC_Kurt_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         histPnt = fHM->H1(sHistName.Data());
         histPnt->Draw("hist");
*/
      } // for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
   } // for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)

   /** Recovers/Create Ms Size Canvase for COSY 2017 **/
   // Try to recover canvas in case it was created already by another monitor
   // If not existing, create it
   fcMsSizeAll = dynamic_cast<TCanvas *>( gROOT->FindObject( "cMsSizeAll" ) );
   if( NULL == fcMsSizeAll )
   {
      fcMsSizeAll = new TCanvas("cMsSizeAll", "Evolution of MS size in last 300 s", w, h);
      fcMsSizeAll->Divide( 4, 4 );
      LOG(info) << "Created MS size canvas in STS monitor";
   } // if( NULL == fcMsSizeAll )
      else LOG(info) << "Recovered MS size canvas in STS monitor";
   /***************************************************/

   /** Save pointers to each histogram in class members to speed up access **/
   fHistMessType = fHM->H1("hMessageTypeSts");
   fHistSysMessType = fHM->H1("hSysMessTypeSts");

   for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   {// looping on all the nDPBS IDs
      if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
      {
         sNdpbTag = Form("%04X", fUnpackPar->GetNdpbIdA(dpbId) );
      } // if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
         else
         {
            sNdpbTag = Form("%04X", fUnpackPar->GetNdpbIdB(dpbId - fNrOfNdpbsA) );
         } // else of if( dpbId < fUnpackPar->GetNrOfnDpbsModA() )
      for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
      {// looping on all the FEB IDs
         sHistName = Form("Chan_Counts_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         fChan_Counts_Sts.push_back(fHM->H1(sHistName.Data()));
         sHistName = Form("Raw_ADC_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         fRaw_ADC_Sts.push_back(fHM->H2(sHistName.Data()));
         sHistName = Form("FebRate_n%s_f%1u", sNdpbTag.Data(), febId);
         fFebRate.push_back(fHM->H1(sHistName.Data()));
         sHistName = Form("HitMissEvo_n%s_f%1u", sNdpbTag.Data(), febId);
         fHitMissEvo.push_back(fHM->H1(sHistName.Data()));

         if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
         {
            sDateHistName = Form("FebRateDate_n%s_f%1u", sNdpbTag.Data(), febId);
            fFebRateDate_nDPB.push_back(fHM->H1(sDateHistName.Data()));

            sHistName = Form("HitDtDate_n%s_f%1u", sNdpbTag.Data(), febId );
            fHitDtDate_nDPB.push_back(fHM->H2(sHistName.Data()));
            fdLastHitTime_nDPB.push_back( -1.0 );

            sHistName = Form("HitDt_n%s_f%1u", sNdpbTag.Data(), febId );
            fHitDt_nDPB.push_back(fHM->H1(sHistName.Data()));
         } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )

         sHistName = Form("ADC_Mean_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         fADC_Mean_Sts.push_back(fHM->H1(sHistName.Data()));
         sHistName = Form("ADC_Rms_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         fADC_Rms_Sts.push_back(fHM->H1(sHistName.Data()));
         sHistName = Form("ADC_Skew_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         fADC_Skew_Sts.push_back(fHM->H1(sHistName.Data()));
         sHistName = Form("ADC_Kurt_Sts_n%s_f%1u", sNdpbTag.Data(), febId);
         fADC_Kurt_Sts.push_back(fHM->H1(sHistName.Data()));
      } // for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
   } // for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)

   for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )
   {
      fhMsSz[ component ] = NULL;
      fhMsSzTime[ component ] = NULL;
   } // for( Int_t component = 0; component < kiMaxNbFlibLinks; component ++ )
   /*************************************************************************/
}

Bool_t CbmTsMonitorSts::DoUnpack(const fles::Timeslice& ts, size_t component)
{
   THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

   if( bResetStsHistos )
   {
      LOG(info) << "Start resetting histos ";
      ResetAllHistos();
      bResetStsHistos = kFALSE;
   } // if( bResetStsHistos )

   if( bSaveStsHistos )
   {
      LOG(info) << "Start saving histos ";
      SaveAllHistos();
      bSaveStsHistos = kFALSE;
   } // if( bSaveStsHistos )

   if( bUpdateAdcHistosSts )
   {
      LOG(info) << "Start updatting ADC stats histos ";
      UpdateAdcStatHistos();
      bUpdateAdcHistosSts = kFALSE;
   } // if( bUpdateAdcHistosSts )

   LOG(debug) << "Timeslice contains " << ts.num_microslices(component)
              << "microslices.";

   if( component < kiMaxNbFlibLinks )
      if( NULL == fhMsSz[ component ] )
   {
      TString sMsSzName = Form("MsSz_link_%02lu", component);
      TString sMsSzTitle = Form("Size of MS for nDPB of link %02lu; Ms Size [bytes]", component);
      fHM->Add(sMsSzName.Data(), new TH1F( sMsSzName.Data(), sMsSzTitle.Data(),
                                    160000, 0., 20000. ) );
      fhMsSz[ component ] = fHM->H1(sMsSzName.Data());
      if (server) server->Register("/FlibRaw", fhMsSz[ component ] );
      sMsSzName = Form("MsSzTime_link_%02lu", component);
      sMsSzTitle = Form("Size of MS vs time for gDPB of link %02lu; Time[s] ; Ms Size [bytes]", component);
      fHM->Add(sMsSzName.Data(), new TProfile( sMsSzName.Data(), sMsSzTitle.Data(),
                                    15000, 0., 300. ) );
      fhMsSzTime[ component ] = fHM->P1(sMsSzName.Data());
      if (server) server->Register("/FlibRaw", fhMsSzTime[ component ] );
      if( NULL != fcMsSizeAll )
      {
         fcMsSizeAll->cd( 1 + component );
         gPad->SetLogy();
         fhMsSzTime[ component ]->Draw("hist le0");
      } // if( NULL != fcMsSizeAll )
      LOG(info) << "Added MS size histo for component: " << component
             << " (nDPB)";
   } // if( NULL == fhMsSz[ component ] )

   Int_t messageType = -111;
   // Loop over microslices
   for (size_t m = 0; m < ts.num_microslices(component); ++m)
   {
      constexpr uint32_t kuBytesPerMessage = 8;

      auto msDescriptor = ts.descriptor(component, m);
      fEquipmentId = msDescriptor.eq_id;
      const uint8_t* msContent = reinterpret_cast<const uint8_t*>(ts.content(component, m));

      uint32_t size = msDescriptor.size;
      LOG(debug) << "Microslice: " << msDescriptor.idx
                << " has size: " << size;

      if( component < kiMaxNbFlibLinks )
      {
         if( fdStartTimeMsSz < 0 )
            fdStartTimeMsSz = (1e-9) * static_cast<double>(msDescriptor.idx);
         fhMsSz[ component ]->Fill( size );
         fhMsSzTime[ component ]->Fill( (1e-9) * static_cast<double>( msDescriptor.idx) - fdStartTimeMsSz, size);
      } // if( component < kiMaxNbFlibLinks )

      // If not integer number of message in input buffer, print warning/error
      if( 0 != (size % kuBytesPerMessage) )
         LOG(error) << "The input microslice buffer does NOT "
                    << "contain only complete nDPB messages!";

      // Compute the number of complete messages in the input microslice buffer
      uint32_t uNbMessages = (size - (size % kuBytesPerMessage) )
                              / kuBytesPerMessage;

      // Prepare variables for the loop on contents
      const uint64_t* pInBuff = reinterpret_cast<const uint64_t*>( msContent );
      UInt_t uNdpbIdx;
      UInt_t uFebBase;
      for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
      {
         // Fill message
         uint64_t ulData = static_cast<uint64_t>( pInBuff[uIdx] );
         ngdpb::Message mess( ulData );

         if( 0 == uIdx )
         {
            Int_t rocId      = mess.getRocNumber();
            // First check if nDPB is mapped
            if( fNdpbIdIndexMap.end() == fNdpbIdIndexMap.find( rocId ) )
            {
               LOG(fatal) << "Unmapped nDPB Id " << std::hex << rocId << std::dec;
            } // if( fNdpbIdIndexMap.end() == fNdpbIdIndexMap.find( rocId ) )
            uNdpbIdx = fNdpbIdIndexMap[rocId];
            uFebBase = uNdpbIdx * fNrOfFebsPerNdpb;
         } // if( 0 == uIdx )

         if(gLogger->IsLogNeeded( fair::Severity::debug ))
         {
            mess.printDataCout();
         } // if(gLogger->IsLogNeeded( fair::Severity::debug ))

         // Increment counter for different message types
         // and fill the corresponding histogram
         messageType = mess.getMessageType();
         fMsgCounter[messageType]++;
         fHistMessType->Fill(messageType);

         switch( messageType )
         {
            case ngdpb::MSG_HIT:
               FillHitInfo(mess, uNdpbIdx, uFebBase);
               break;
            case ngdpb::MSG_EPOCH:
               FillEpochInfo(mess, uNdpbIdx, uFebBase);
               break;
            case ngdpb::MSG_SYNC:
               // Do nothing, this message is just there to make sure we get all Epochs
               break;
            case ngdpb::MSG_SYS:
               // Just keep track of which type of System message we receive
               fHistSysMessType->Fill(mess.getSysMesType());
               break;
            default:
               LOG(error) << "Message type " << std::hex << std::setw(2)
                          << static_cast< uint16_t >( mess.getMessageType() )
                          << " not yet include in nXYTER unpacker.";
         } // switch( messageType )
      } // for (uint32_t uIdx = 0; uIdx < uNbMessages; uIdx ++)
   } // for (size_t m = 0; m < ts.num_microslices(component); ++m)

   return kTRUE;
}

void CbmTsMonitorSts::FillHitInfo(ngdpb::Message mess, UInt_t /*uNdpbIdx*/, UInt_t uFebBase)
{
  // --- Get absolute time, NXYTER and channel number
  Int_t rocId      = mess.getRocNumber();
  Int_t nxyterId   = mess.getNxNumber();
  Int_t nxChannel  = mess.getNxChNum();
  Int_t charge     = mess.getNxAdcValue();

  LOG(debug) << "Hit: " << rocId << ", " << nxyterId
             << ", " << nxChannel << ", " << charge;

  //here converting channel number into the STS Digi.

/*
	Int_t address = CreateAddress(uFebBase,nxyterId,0, 0, 0, 0, nxChannel);
	if (address){
		LOG(debug) << "Got address for hit";
	}
	else {
		LOG(error) << "Unknown Roc Id " << rocId << " or nxyterId "<< nxyterId << " or channelId "
                 << nxChannel;
	}
*/
	Int_t iFebNr = uFebBase + nxyterId;
	fChan_Counts_Sts[iFebNr]->Fill(nxChannel);
	fRaw_ADC_Sts[iFebNr]->Fill(nxChannel, charge);

   if( fCurrentEpoch.end() != fCurrentEpoch.find( rocId ) )
   {
      if( fCurrentEpoch[rocId].end() != fCurrentEpoch[rocId].find( nxyterId ) )
      {
         Double_t dHitFullTime = mess.getMsgFullTimeD( fCurrentEpoch[rocId][nxyterId] );

         if( fdStartTime <= 0 )
         {
            fdStartTime = dHitFullTime;

            LOG(info) << "Start time set to " << (fdStartTime/1e9)
                      << " s using first hit on channel " << nxChannel
                      << " of FEB " << nxyterId
                      << " on nDPB " << fNdpbIdIndexMap[rocId]
                      << " in epoch " << fCurrentEpoch[rocId][nxyterId];
         } // if( fdStartTime <= 0 )

         if( 0 < fdStartTime )
         {
            fFebRate[iFebNr]->Fill( 1e-9*( dHitFullTime - fdStartTime)  );

            // General Time (date + time) rate evolution
            // Add offset of -1H as the filenames were using some times offset by 1 hour (Summer time?!?)
            if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
            {
               fFebRateDate_nDPB[iFebNr]->Fill(
                        1e-9 * ( dHitFullTime - fdStartTime ) + fiRunStartDateTimeSec  );
               if( 0 < fdLastHitTime_nDPB[ iFebNr ] )
               {
                  Double_t dTimeDiff = dHitFullTime - fdLastHitTime_nDPB[ iFebNr ];
                  fHitDtDate_nDPB[ iFebNr ]->Fill(
                           1e-9 * ( dHitFullTime - fdStartTime ) + fiRunStartDateTimeSec,
                           1e9/( dTimeDiff )
                                            );
                  if( 0 != dTimeDiff )
                     fHitDt_nDPB[ iFebNr ]->Fill( std::abs(dTimeDiff) ); // need abs due to messages not time sorted
               } // if( 0 < fdLastHitTime_nDPB[ channelNr ] )
               fdLastHitTime_nDPB[ iFebNr ] = dHitFullTime;
            } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
         } //  if( 0 < fdStartTime )
      } // if( fCurrentEpoch[rocId].end() != fCurrentEpoch[rocId].find( nxyterId ) )
   } // if( fCurrentEpoch.end() != fCurrentEpoch.find( rocId ) )

}

/*
Int_t CbmTsMonitorSts::CreateAddress(Int_t febBase, Int_t febId, Int_t stationId,
		Int_t layerId, Int_t sideId, Int_t moduleId, Int_t channelId)
{
	Int_t febNr = febBase + febId;
	Int_t sector  = fUnpackPar->GetPadX(febNr, channelId);
	Int_t channel = fUnpackPar->GetPadY(febNr, channelId);

	Int_t address = CbmStsAddress::GetAddress(stationId, layerId, sideId, moduleId, sector, channel);
	if(!(sector<0||channel<0)){

		fHistPadDistr->Fill(78-sector,22-channel);

	}

//	fHM->H2("Pad_Distribution")->Fill(sector,channel);
	return address;
   return 0;
}
*/

void CbmTsMonitorSts::FillEpochInfo(ngdpb::Message mess, UInt_t /*uNdpbIdx*/, UInt_t uFebBase)
{
  Int_t rocId          = mess.getRocNumber();
  Int_t nxyterId       = mess.getEpochNxNum();

  UInt_t uEpochVal = mess.getEpochNumber();
  fCurrentEpoch[rocId][nxyterId] = uEpochVal;
  fCurrentEpochTime = mess.getMsgFullTimeD( uEpochVal );

  if( fdStartTime <= 0 )
  {
    Int_t channelNr = uFebBase + nxyterId;
    fHitMissEvo[channelNr]->Fill( fCurrentEpochTime - fdStartTime, mess.getEpochMissed() );
  }

  fNofEpochs++;
  LOG(debug) << "Epoch message "
             << fNofEpochs << ", epoch " << uEpochVal
             << ", time " << std::setprecision(9) << std::fixed
             << fCurrentEpochTime * 1.e-9 << " s "
             << " for board ID " << std::hex << std::setw(4) << rocId << std::dec;
}

void CbmTsMonitorSts::Reset()
{
}

void CbmTsMonitorSts::Finish()
{
  TString message_type;

  for (unsigned int i=0; i< fMsgCounter.size(); ++i) {
    switch(i) {
    case 0: message_type ="NOP"; break;
    case 1: message_type ="HIT"; break;
    case 2: message_type ="EPOCH"; break;
    case 3: message_type ="SYNC"; break;
    case 4: message_type ="AUX"; break;
    case 5: message_type ="EPOCH2"; break;
    case 6: message_type ="GET4"; break;
    case 7: message_type ="SYS"; break;
    case 8: message_type ="GET4_SLC"; break;
    case 9: message_type ="GET4_32B"; break;
    case 10: message_type ="GET4_SYS"; break;
    }
    LOG(info) << message_type << " messages: "
              << fMsgCounter[i];
  }

   LOG(info) << "-------------------------------------";
   for( auto it = fCurrentEpoch.begin(); it != fCurrentEpoch.end(); ++it)
      for( auto itN = (it->second).begin(); itN != (it->second).end(); ++itN)
      LOG(info) << "Last epoch for nDPB: "
                << std::hex << std::setw(4) << it->first << std::dec
                << " , FEB  " << std::setw(4) << itN->first
                << " => " << itN->second;
   LOG(info) << "-------------------------------------";

   UpdateAdcStatHistos();
   SaveAllHistos();
}


void CbmTsMonitorSts::FillOutput(boost::any)
{
}

void CbmTsMonitorSts::ResetAllHistos()
{
  LOG(info) << "Reseting all STS histograms.";
  fHistMessType   ->Reset();
  fHistSysMessType->Reset();

   for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   {// looping on all the nDPBS IDs
      for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
      {// looping on all the FEB IDs
        UInt_t uFebBase = dpbId * fNrOfFebsPerNdpb;
        Int_t  iFebNr = uFebBase + febId;
        fChan_Counts_Sts[iFebNr]->Reset();
        fRaw_ADC_Sts    [iFebNr]->Reset();
        fFebRate        [iFebNr]->Reset();
        fHitMissEvo     [iFebNr]->Reset();

        if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
        {
          fFebRateDate_nDPB[iFebNr]->Reset();
          fHitDtDate_nDPB  [iFebNr]->Reset();
          fHitDt_nDPB      [iFebNr]->Reset();
        } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
      } // for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
   } // for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)

  for (UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; uLinks++)
  {
    if( NULL != fhMsSz[uLinks] )
      fhMsSz[uLinks]->Reset();
    if( NULL != fhMsSzTime[uLinks] )
      fhMsSzTime[uLinks]->Reset();
  } // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)

  fdStartTime = -1;
  LOG(info) << "Reset most histos done!";
}

void CbmTsMonitorSts::SaveAllHistos()
{
   TDirectory * oldDir = gDirectory;

   TFile * histoFile = new TFile("data/histos_test.root", "RECREATE");

   histoFile->cd();
   histoFile->mkdir("Sts_Raw");
   histoFile->cd("Sts_Raw");

   fHistMessType   ->Reset();
   fHistSysMessType->Reset();

   for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   {// looping on all the nDPBS IDs
      for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
      {// looping on all the FEB IDs
         UInt_t uFebBase = dpbId * fNrOfFebsPerNdpb;
         Int_t  iFebNr = uFebBase + febId;

         fChan_Counts_Sts[iFebNr]->Write();
         fRaw_ADC_Sts    [iFebNr]->Write();
         fFebRate        [iFebNr]->Write();
         fHitMissEvo     [iFebNr]->Write();

         if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )
         {
             fFebRateDate_nDPB[iFebNr]->Write();
             fHitDtDate_nDPB  [iFebNr]->Write();
             fHitDt_nDPB      [iFebNr]->Write();
         } // if( 0 < fiBinSizeDatePlots && 0 < fiRunStartDateTimeSec )

         fADC_Mean_Sts[iFebNr]->Write();
         fADC_Rms_Sts [iFebNr]->Write();
         fADC_Skew_Sts[iFebNr]->Write();
         fADC_Kurt_Sts[iFebNr]->Write();
      } // for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
   } // for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   histoFile->cd("..");

   histoFile->mkdir("Flib_Raw");
   histoFile->cd("Flib_Raw");
   for (UInt_t uLinks = 0; uLinks < kiMaxNbFlibLinks; uLinks++)
   {
      if( NULL != fhMsSz[uLinks] )
         fhMsSz[uLinks]->Write();
      if( NULL != fhMsSzTime[uLinks] )
         fhMsSzTime[uLinks]->Write();
   } // for( UInt_t uLinks = 0; uLinks < 16; uLinks ++)
   histoFile->cd("..");

   histoFile->Close();

   oldDir->cd();

  LOG(info) << "Save all histos done!";
}

void CbmTsMonitorSts::UpdateAdcStatHistos()
{
   TH1 * phChanAdcProj = NULL;
   for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)
   {// looping on all the nDPBS IDs
      for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
      {// looping on all the FEB IDs
         UInt_t uFebBase = dpbId * fNrOfFebsPerNdpb;
         Int_t channelNr = uFebBase + febId;

         // First reset plots before update
         fADC_Mean_Sts[channelNr]->Reset();
         fADC_Rms_Sts [channelNr]->Reset();
         fADC_Skew_Sts[channelNr]->Reset();
         fADC_Kurt_Sts[channelNr]->Reset();

         for( Int_t iChanId = 0; iChanId < kiNbChanFebF; iChanId ++)
         {// looping on all channels
            phChanAdcProj = fRaw_ADC_Sts[channelNr]->ProjectionY( Form("_py_%03d", iChanId),
                                                                   1 + iChanId,
                                                                   1 + iChanId );
            if( 0 < phChanAdcProj->GetEntries() )
            {
               fADC_Mean_Sts[channelNr]->Fill( iChanId, phChanAdcProj->GetMean() );
               fADC_Rms_Sts [channelNr]->Fill( iChanId, phChanAdcProj->GetRMS() );
               fADC_Skew_Sts[channelNr]->Fill( iChanId, phChanAdcProj->GetSkewness() );
               fADC_Kurt_Sts[channelNr]->Fill( iChanId, phChanAdcProj->GetKurtosis() );
            } // if( 0 < phChanAdcProj->GetEntries() )
               else
               {
                  fADC_Mean_Sts[channelNr]->Fill( iChanId, 0.0 );
                  fADC_Rms_Sts [channelNr]->Fill( iChanId, 0.0 );
                  fADC_Skew_Sts[channelNr]->Fill( iChanId, 0.0 );
                  fADC_Kurt_Sts[channelNr]->Fill( iChanId, 0.0 );
               } // else of if( 0 < phChanAdcProj->GetEntries() )

            delete phChanAdcProj;
         }
      } // for( Int_t febId = 0; febId < fNrOfFebsPerNdpb; febId++)
   } // for( Int_t dpbId = 0; dpbId < fNrOfNdpbs; dpbId++)

  LOG(info) << "Update of ADC stats histos done!";
}

void CbmTsMonitorSts::SetRunStart( Int_t dateIn, Int_t timeIn, Int_t iBinSize )
{
   TDatime * fRunStartDateTime     = new TDatime( dateIn, timeIn);
   fiRunStartDateTimeSec = fRunStartDateTime->Convert();
   fiBinSizeDatePlots    = iBinSize;

   LOG(info) << "Assigned new STS Run Start Date-Time: " << fRunStartDateTime->AsString();
}

ClassImp(CbmTsMonitorSts)
