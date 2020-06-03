// -----------------------------------------------------------------------------
// -----                                                                   -----
// ----- intended use: unpacking of rate data acquired with MBS            -----
// -----               during the CERN Feb. 2015 beamtime                  -----
// -----                                                                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
/*
 * Known problems:
 * 1) Due to a mistake in the MBS readout, the ANDs scalers are shifter by one channel
 *    => Channel 15 is missing
 *    => Channel 0 to 15 are mapped on channels 1 to 15
 *    (Reason: Old register map was used, and scaler register were shifted due
 *     to new inversion register, cf ~/user1Home/cern15/mbs_cern15/vulom3def.h and
 *     ~/user1Home/cern15/scalers_cern15/vulom3def.h)
 */

// 5 scalers: HD-RPC, BUC-RPC, TSU-RPC, USTC-RPC, DIAM
const Int_t kiNbScalers = 5;
const Int_t kiScalerIndex[kiNbScalers] = { 1, 2, 3, 4, 5};

// 8 RPCs: HD-P2, HD-P5, BUC-REF, BUC-13, TSU-STR, TSU-PAD, USTC-RPC, DIAM
const Int_t kiNbRpc = 8;
const Int_t kiRpcScalId[kiNbRpc]  = {    0,     0,     1,     1,     2,     2,     3,     4  };
// channels                            1-8     9-15   1-8    9-15   1-6   10-15   1-4    0-15
const Double_t kdRpcArea[kiNbRpc] = {  648.0,  60.0, 100.0, 490.0, 648.0, 230.0, 864.0,   4.0};
const TString  ksRpcName[kiNbRpc] = { "HDP2","HDP5","BREF","BR13","TSUS","TSUP","USTC","DIAM"};

const Int_t kiNbOrChanRpc  = 32;
const Int_t kiNbAndChanRpc = 16; // Not usable directly due to register map error
const Int_t kiStartAndRpc  =  1;
const Int_t kiMidAndRpc    =  9;
const Int_t kiNbOrChanDiam = 16;

// 4 Plastics
const Int_t kiNbPmt = 6;
// T = TOP, B = Bottom, F = Front      TF     TB     BF     BB     T     B
const Int_t kiPlaScalId[kiNbPmt]  = {   0,     1,     2,     3,    6,    7  };
const Double_t kdPmtArea[kiNbPmt] = {  16.0,  44.0,   8.0,   8.0,  1.0,  1.0};
const TString  ksPmtName[kiNbPmt] = {  "TF",  "TB",  "BF",  "BB",  "T",  "B"};

const Int_t    kiSpillDistSec = 30; // Approximate value
const Double_t kdSpillDiamThr = 100.0; // 1/(s.cm^2)

// Max nEvents: 198999999999
void plot_Flux(TString sInputName = "",
               Int_t iNbSecPerBin = 5, Double_t dStartTimeMin = 0, Double_t dStopTimeMin = 15*60,
               Int_t nEvents = -1)
{
   if( "" == sInputName )
   {
      cout<<"Empty input filename!!!"<<endl;
      return;
   }

   TFile * fInput = new TFile( sInputName, "READ");

   TTree* tTree = (TTree*)fInput->Get("cbmsim");
   if(!tTree)
   {
    cout<<"Output tree could not be retrieved from file. Abort macro execution."<<endl;
    return;
   } // if(!tTree)

   TBranch* tBranchTrlo = tTree->GetBranch("TofTriglog");
   if(!tBranchTrlo)
   {
    cout<<"Branch 'TofTriglog' not found in output tree. Abort macro execution."<<endl;
    return;
   } // if(!tBranchTrlo)

   void* vOldAddressTrlo = tBranchTrlo->GetAddress();

   TBranch* tBranch = tTree->GetBranch("TofCalibScaler");
   if(!tBranch)
   {
    cout<<"Branch 'TofCalibScaler' not found in output tree. Abort macro execution."<<endl;
    return;
   } // if(!tBranch)

   void* vOldAddress = tBranch->GetAddress();

   TDirectory* tOldDirectory = gDirectory;
   gROOT->cd();

   TClonesArray* tArrayTrlo = new TClonesArray("TTofTriglogBoard");
   tBranchTrlo->SetAddress(&tArrayTrlo);

   TTofTriglogBoard* tTriglogBoard;

   TClonesArray* tArray = new TClonesArray("TTofCalibScaler");
   tBranch->SetAddress(&tArray);

   TTofCalibScaler* tCalibTriglog;
   TTofCalibScaler* tCalibScaler[kiNbScalers];
   TTofCalibScaler* tCalibTrloScal;

   // Prepare histos and variables
   Int_t iNbBins       = (Int_t)(dStopTimeMin - dStartTimeMin)*60 /iNbSecPerBin;
   Double_t dStartTime = dStartTimeMin * 60.0;
   Double_t dStopTime  = dStopTimeMin  * 60.0;
   TProfile* tFluxHdP2 = new TProfile("tFluxHdP2","", iNbBins, dStartTime, dStopTime);
   TProfile* tFluxHdP5 = new TProfile("tFluxHdP5","", iNbBins, dStartTime, dStopTime);
   TProfile* tFluxBRef = new TProfile("tFluxBRef","", iNbBins, dStartTime, dStopTime);
   TProfile* tFluxB13  = new TProfile("tFluxB13", "", iNbBins, dStartTime, dStopTime);
   TProfile* tFluxTsuS = new TProfile("tFluxTsuS","", iNbBins, dStartTime, dStopTime);
   TProfile* tFluxTsuP = new TProfile("tFluxTsuP","", iNbBins, dStartTime, dStopTime);
   TProfile* tFluxUstc = new TProfile("tFluxUstc","", iNbBins, dStartTime, dStopTime);
   TProfile* tFluxDiam = new TProfile("tFluxDiam","", iNbBins, dStartTime, dStopTime);
   Double_t dLastTime = 0;
   Double_t dLastTimeToLast = 0;
   Double_t dMeanFluxHdP2   = 0.0;
   Double_t dMeanFluxHdP5   = 0.0;
   Double_t dMeanFluxBRef   = 0.0;
   Double_t dMeanFluxB2013  = 0.0;
   Double_t dMeanFluxTsuStr = 0.0;
   Double_t dMeanFluxTsuPad = 0.0;
   Double_t dMeanFluxUstc   = 0.0;
   Double_t dMeanFluxDiamA  = 0.0;
   Double_t dMeanFluxRpc[kiNbRpc];

   Double_t dMeanFluxPmt[kiNbPmt];
   TProfile* tFluxPmt[kiNbPmt];
   tFluxPmt[0] = new TProfile("tFluxPmtTF","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[1] = new TProfile("tFluxPmtTB","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[2] = new TProfile("tFluxPmtBF","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[3] = new TProfile("tFluxPmtBB","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[4] = new TProfile("tFluxPmtT","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[5] = new TProfile("tFluxPmtB","", iNbBins, dStartTime, dStopTime);

   Int_t iNbBinsSpill   = (Int_t)(dStopTimeMin - dStartTimeMin)*60 /kiSpillDistSec;
   TProfile* tSpillFluxRpc[kiNbRpc];
   for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
      tSpillFluxRpc[iRpc] = new TProfile( Form("tSpillFlux%s", ksRpcName[iRpc].Data()),
                                          "", iNbBinsSpill, dStartTime, dStopTime);
   TProfile* tSpillFluxPmt[kiNbPmt];
   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
      tSpillFluxPmt[iPmt] = new TProfile( Form("tSpillFluxPmt%s", ksPmtName[iPmt].Data()),
                                          "", iNbBinsSpill, dStartTime, dStopTime);

   // Loop over measurements
   Long64_t lBranchEntries = tBranch->GetEntries();
   for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
   {
      if(0 == lBranchEntry%10000 && 0 < lBranchEntry)
         cout<<"Event "<<lBranchEntry<<" / "<< lBranchEntries <<" done!"<<endl;
      tArrayTrlo->Clear("C");
      tArray->Clear("C");

      tBranchTrlo->GetEntry(lBranchEntry);
      tBranch->GetEntry(lBranchEntry);

      dMeanFluxHdP2   = 0.0;
      dMeanFluxHdP5   = 0.0;
      dMeanFluxBRef   = 0.0;
      dMeanFluxB2013  = 0.0;
      dMeanFluxTsuStr = 0.0;
      dMeanFluxTsuPad = 0.0;
      dMeanFluxUstc   = 0.0;
      dMeanFluxDiamA  = 0.0;
      for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
         dMeanFluxRpc[iRpc] = 0.0;
      for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
         dMeanFluxPmt[iPmt] = 0.0;

      tTriglogBoard = (TTofTriglogBoard*)tArrayTrlo->At(0);

      // Board 0 is TRIGLOG for trigger
      tCalibTriglog = (TTofCalibScaler*)tArray->At(0);
      // Next boards are Scaler for the RPC
      for( Int_t iScal = 0; iScal < kiNbScalers; iScal++)
         tCalibScaler[iScal] = (TTofCalibScaler*)tArray->At(kiScalerIndex[iScal]);
      // Last Board is a TRIGLOG used as Scaler for PMT
      tCalibTrloScal = (TTofCalibScaler*)tArray->At(1 + kiNbScalers);

      for(Int_t iAndChan = kiStartAndRpc; iAndChan < kiMidAndRpc; iAndChan ++)
      {
         dMeanFluxHdP2   += tCalibScaler[ kiRpcScalId[0] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );
         dMeanFluxBRef   += tCalibScaler[ kiRpcScalId[2] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );
         dMeanFluxTsuStr += tCalibScaler[ kiRpcScalId[4] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );
         dMeanFluxUstc   += tCalibScaler[ kiRpcScalId[6] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );

      } // for(Int_t iAndCHan = 0; iAndChan < kiNbAndChanRpc; iAndChan ++)
      for(Int_t iAndChan = kiMidAndRpc; iAndChan < kiNbAndChanRpc; iAndChan ++)
      {
         dMeanFluxHdP5   += tCalibScaler[ kiRpcScalId[1] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );
         dMeanFluxB2013  += tCalibScaler[ kiRpcScalId[3] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );
         dMeanFluxTsuPad += tCalibScaler[ kiRpcScalId[5] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );
      } // for(Int_t iAndCHan = 0; iAndChan < kiNbAndChanRpc; iAndChan ++)
      dMeanFluxHdP2   /= kdRpcArea[0];
      dMeanFluxHdP5   /= kdRpcArea[1];
      dMeanFluxBRef   /= kdRpcArea[2];
      dMeanFluxB2013  /= kdRpcArea[3];
      dMeanFluxTsuStr /= kdRpcArea[4];
      dMeanFluxTsuPad /= kdRpcArea[5];
      dMeanFluxUstc   /= kdRpcArea[6];

      for(Int_t iOrChan = 0; iOrChan < kiNbOrChanDiam; iOrChan ++)
         dMeanFluxDiamA += tCalibScaler[ kiRpcScalId[7] ]->GetScalerValue( iOrChan );
      dMeanFluxDiamA /= kdRpcArea[7];

      tFluxHdP2->Fill( tCalibScaler[kiRpcScalId[0]]->GetTimeToFirst(), dMeanFluxHdP2 );
      tFluxHdP5->Fill( tCalibScaler[kiRpcScalId[1]]->GetTimeToFirst(), dMeanFluxHdP5 );
      tFluxBRef->Fill( tCalibScaler[kiRpcScalId[2]]->GetTimeToFirst(), dMeanFluxBRef );
      tFluxB13->Fill(  tCalibScaler[kiRpcScalId[3]]->GetTimeToFirst(), dMeanFluxB2013 );
      tFluxTsuS->Fill( tCalibScaler[kiRpcScalId[4]]->GetTimeToFirst(), dMeanFluxTsuStr );
      tFluxTsuP->Fill( tCalibScaler[kiRpcScalId[5]]->GetTimeToFirst(), dMeanFluxTsuPad );
      tFluxUstc->Fill( tCalibScaler[kiRpcScalId[6]]->GetTimeToFirst(), dMeanFluxUstc );
      tFluxDiam->Fill( tCalibScaler[kiRpcScalId[7]]->GetTimeToFirst(), dMeanFluxDiamA );
//      tRateTestE->Fill( tCalibTrloScal->GetTimeToFirst(),  tCalibTrloScal->GetScalerValue( 3, 1 ) );
      for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
      {
         /*
         if( TMath::IsNaN( tCalibTrloScal->GetScalerValue( kiPlaScalId[iPmt], 1 ) ))
            dMeanFluxPmt[iPmt] = 0.0;
            else
            */
               dMeanFluxPmt[iPmt] = tCalibTrloScal->GetScalerValue( kiPlaScalId[iPmt], 1 ) / kdPmtArea[iPmt];
         tFluxPmt[iPmt]->Fill( tCalibTrloScal->GetTimeToFirst(), dMeanFluxPmt[iPmt] );
      } // for( Int_t iPmt = 0; iPmt < kiNbRpc; iPmt++)

      // In spill mean value
      dMeanFluxRpc[0] = dMeanFluxHdP2;
      dMeanFluxRpc[1] = dMeanFluxHdP5;
      dMeanFluxRpc[2] = dMeanFluxBRef;
      dMeanFluxRpc[3] = dMeanFluxB2013;
      dMeanFluxRpc[4] = dMeanFluxTsuStr;
      dMeanFluxRpc[5] = dMeanFluxTsuPad;
      dMeanFluxRpc[6] = dMeanFluxUstc;
      dMeanFluxRpc[7] = dMeanFluxDiamA;
      if( kdSpillDiamThr <= dMeanFluxDiamA )
      {
         for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
            tSpillFluxRpc[iRpc]->Fill( tCalibTrloScal->GetTimeToFirst(), dMeanFluxRpc[iRpc] );
         for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
            tSpillFluxPmt[iPmt]->Fill( tCalibTrloScal->GetTimeToFirst(), dMeanFluxPmt[iPmt] );
      } // if( kdSpillDiamThr <= dMeanFluxDiamA )

      if( tCalibTrloScal->GetTimeToFirst() < dLastTime)
      {
         cout<<" Probably a counter cycle: "<< dLastTime <<" "<<tCalibTrloScal->GetTimeToFirst()<<endl;
         cout<<"                           "<< dLastTimeToLast <<" "<<tCalibTrloScal->GetTimeToLast()<<endl;
      }
      dLastTime = tCalibTrloScal->GetTimeToFirst();
      dLastTimeToLast = tCalibTrloScal->GetTimeToLast();


      if(0 == lBranchEntry%10000 && 0 < lBranchEntry)
         cout<<"Event time: "<< tTriglogBoard->GetMbsTimeSec() <<" done!"<<endl;
   } // for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)


   delete gROOT->FindObjectAny( "tCanvas1" );
   TCanvas* tCanvas1 = new TCanvas("tCanvas1","Detectors",0,0,1400,700);
   tCanvas1->Divide(4, 2);

   tCanvas1->cd(1);
   tFluxHdP2->Draw("h");
   gPad->SetLogy();

   tCanvas1->cd(2);
   tFluxHdP5->Draw("h");
   gPad->SetLogy();

   tCanvas1->cd(3);
   tFluxBRef->Draw("h");
   gPad->SetLogy();

   tCanvas1->cd(4);
   tFluxB13->Draw("h");
   gPad->SetLogy();

   tCanvas1->cd(5);
   tFluxTsuS->Draw("h");
   gPad->SetLogy();

   tCanvas1->cd(6);
   tFluxTsuP->Draw("h");
   gPad->SetLogy();

   tCanvas1->cd(7);
   tFluxUstc->Draw("h");
   gPad->SetLogy();

   tCanvas1->cd(8);
   tFluxDiam->Draw("h");
   gPad->SetLogy();

   delete gROOT->FindObjectAny( "tCanvas2" );
   TCanvas* tCanvas2 = new TCanvas("tCanvas2","Setups",0,0,1400,700);
   tCanvas2->Divide(2, 1);

   tCanvas2->cd(1);
   TH1 *frameA = gPad->DrawFrame(dStartTime, 1, dStopTime, 1e5);
//   frameA->GetXaxis()->SetTitle("X title");
//   frameA->GetYaxis()->SetTitle("Y title");
   gPad->Update();
   gPad->SetLogy();
   tFluxHdP2->SetLineColor( kBlack);
   tFluxHdP2->Draw("hsame");
   tFluxHdP5->SetLineColor( kBlue);
   tFluxHdP5->Draw("hsame");
   tFluxTsuS->SetLineColor( kRed);
   tFluxTsuS->Draw("hsame");
   tFluxUstc->SetLineColor( kGreen);
   tFluxUstc->Draw("hsame");

   tCanvas2->cd(2);
   TH1 *frameB = gPad->DrawFrame(dStartTime, 1e-1, dStopTime, 1e8);
//   frameB->GetXaxis()->SetTitle("X title");
//   frameB->GetYaxis()->SetTitle("Y title");
   gPad->Update();
   gPad->SetLogy();
   tFluxBRef->SetLineColor( kBlack);
   tFluxBRef->Draw("hsame");
   tFluxB13->SetLineColor( kBlue);
   tFluxB13->Draw("hsame");
   tFluxTsuP->SetLineColor( kRed);
   tFluxTsuP->Draw("hsame");
   tFluxDiam->SetLineColor( kGreen);
   tFluxDiam->Draw("hsame");

   delete gROOT->FindObjectAny( "tCanvas3" );
   TCanvas* tCanvas3 = new TCanvas("tCanvas3","Plastics",0,0,1400,700);
   tCanvas3->Divide(4, 2);

   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
   {
      tCanvas3->cd(1+iPmt);
      tFluxPmt[iPmt]->Draw("h");
      gPad->SetLogy();
   } // // for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)

   tCanvas3->cd(7);
   TH1 *frameC = gPad->DrawFrame(dStartTime, 1e-1, dStopTime, 1e5);
   gPad->Update();
   gPad->SetLogy();
   tFluxPmt[0]->SetLineColor( kBlack);
   tFluxPmt[0]->Draw("hsame");
   tFluxPmt[1]->SetLineColor( kBlue);
   tFluxPmt[1]->Draw("hsame");
   tFluxPmt[4]->SetLineColor( kRed);
   tFluxPmt[4]->Draw("hsame");

   tCanvas3->cd(8);
   TH1 *frameD = gPad->DrawFrame(dStartTime, 1e-1, dStopTime, 1e5);
   gPad->Update();
   gPad->SetLogy();
   tFluxPmt[2]->SetLineColor( kBlack);
   tFluxPmt[2]->Draw("hsame");
   tFluxPmt[3]->SetLineColor( kBlue);
   tFluxPmt[3]->Draw("hsame");
   tFluxPmt[5]->SetLineColor( kRed);
   tFluxPmt[5]->Draw("hsame");


   delete gROOT->FindObjectAny( "tCanvas4" );
   TCanvas* tCanvas4 = new TCanvas("tCanvas4","Rpc Spill",0,0,1400,700);
   tCanvas4->Divide(4, 2);

   for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
   {
      tCanvas4->cd(1+iRpc);
      tSpillFluxRpc[iRpc]->Draw("h");
      gPad->SetLogy();
   } // // for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)

   delete gROOT->FindObjectAny( "tCanvas5" );
   TCanvas* tCanvas5 = new TCanvas("tCanvas5","Pmt Spill",0,0,1400,700);
   tCanvas5->Divide(3, 2);

   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
   {
      tCanvas5->cd(1+iPmt);
      tSpillFluxPmt[iPmt]->Draw("h");
      gPad->SetLogy();
   } // // for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
}
