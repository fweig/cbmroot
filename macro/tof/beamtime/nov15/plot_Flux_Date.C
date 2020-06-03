// -----------------------------------------------------------------------------
// -----                                                                   -----
// ----- intended use: unpacking of rate data acquired with MBS            -----
// -----               during the CERN Feb. 2015 beamtime                  -----
// -----                                                                   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
/*
 * Mapping:
 * Trigger TRIGLOG: Scaler 0-16, internal pulsers for periodic scaler saving
 * 
 * 6 OR scalers board => see MappingScalersCernNov2015.c
 * 
 * FREE TRLO:
 * #  1  => OR-HD-RPC P5         (V.add: 0A, LEMO 1)
 * #  2  => OR-HD-RPC P2         (V.add: 06, LEMO 1)
 * #  3  => OR-PADS              (V.add: 07, LEMO 1)
 * #  4  => OR-BUC-RPC Reference (V.add: 09, LEMO 1)
 * #  5  => OR-BUC-RPC 2012 Or   (V.add: 08, ECLO 5)
 * #  6  => OR-BUC-RPC 2015 Or 1 (V.add: 08, ECLO 6)
 * #  7  => OR-BUC-RPC 2015 Or 2 (V.add: 08, ECLO 7)
 * #  8  => -------------------- 
 * #  9  => OR-CRPC              (V.add: 07, LEMO 2))
 * # 10  => DIAMOND NIM
 * # 11  => OR-STAR-BOX 3 Or 1   (V.add: 05, ECLO 3)
 * # 12  => OR-STAR-BOX 3 Or 2   (V.add: 05, ECLO 4)
 * # 13  => OR-STAR-BOX 2 Or 1   (V.add: 05, ECLO 5)
 * # 14  => OR-STAR-BOX 2 Or 2   (V.add: 05, ECLO 6)
 * # 15  => OR-STAR-BOX 1 Or 1   (V.add: 05, ECLO 7)
 * # 16  => OR-STAR-BOX 1 Or 2   (V.add: 05, ECLO 8)
 * FREE TRLO, LMU OUT:
 * INPUT          1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 (fixed)
 *              : +  x  x  x  x  x  x  x  x  +  x  x  x  x  x  x  1           
 *              : x  +  x  x  x  x  x  x  x  +  x  x  x  x  x  x  2
 *              : x  x  x  x  +  x  x  x  x  +  x  x  x  x  x  x  3
 *              : x  x  x  +  x  x  x  x  x  +  x  x  x  x  x  x  4
 *              : x  x  x  x  x  x  x  x  +  +  x  x  x  x  x  x  5
 *              : +  +  x  x  x  x  x  x  x  +  x  x  x  x  x  x  6  O
 *              : x  x  x  +  +  +  +  x  x  +  x  x  x  x  x  x  7  U
 *              : +  +  +  +  +  +  +  +  +  +  x  x  x  x  x  x  8  T
 */

//   1 Trigger TrigLog used only to controls how often scalers were saved
// + 6 scalers boards: STAR-Boxes, HD-P2, PMT+PAD+CRPC, BUC-RPC 12+15, BUC-REF, HD-P5
// + 1 Free Trlo board (scalers + "16 -> 8" conversion for TRB trigger input)
// Diamond only in Triglog Free Trlo
const Int_t kiNbScalers = 7;
const Int_t kiScalerIndex[kiNbScalers] = { 1, 2, 3, 4, 5, 6, 7};

// 14 RPCs: HD-P2, HD-P5, TSUP, BUC-REF, BUC-2012, BUC-2015-1, BUC-2015-2, CRPC, 
//          SB-3-1, SB-3-2, SB-2-1, SB-2-2, SB-1-1, SB-1-2
const Int_t kiNbRpc = 14;
const Int_t kiRpcScalId[kiNbRpc]  = {    0,      0,      0,      0,      0,      0,
                                         1,      2,      2,      3,      3,      3,
                                         4,      5
                                    };
// channels in                          4- 5    6- 7    8- 9   10-11   12-13   14-15
//                                      0-15    8- 9    1- 6    0- 7    8- 9   10-11
//                                      0-15    0-15
const Int_t kiRpcInChSta[kiNbRpc] = {    4,      6,      8,     10,     12,     14,
                                         0,      8,      1,      0,      8,     10,
                                         0,      0
                                    };
const Int_t kiRpcInChSto[kiNbRpc] = {    5,      7,      9,     11,     13,     15,
                                        15,      9,      6,      7,      9,     11,
                                        15,     15
                                    };
// channels AND or out                 AND 1   AND 2   AND 3   AND 4   AND 5   AND 6
//                                      0- 7  LEMO 0  LEMO 1    0- 5   AND 4   AND 5
//                                      0- 7  LEMO 0
const Int_t kiRpcAndChSta[kiNbRpc]= {    1,      2,      3,      4,      5,      6,
                                         0,     -1,     -1,      0,      4,      5,
                                         0,     -1
                                    };
const Int_t kiRpcAndChSto[kiNbRpc]= {    1,      2,      3,      4,      5,      6,
                                         7,     -1,     -1,      3,      4,      5,
                                         7,     -1
                                    };
const Double_t kdRpcArea[kiNbRpc] = {  648.0,  648.0,  648.0,  648.0,  648.0,  648.0,
                                       648.0,   16.0,  230.0,  560.0,  560.0,  560.0,
                                       100.0,   60.0
                                    };
const TString  ksRpcName[kiNbRpc] = { "SB31", "SB32", "SB21", "SB22", "SB11", "SB12", 
                                      "HDP2", "CRPC", "TSUP", "BR12", "B151", "B152", 
                                      "BREF", "HDP5"
                                    };

const Int_t kiNbOrChanRpc  = 32;
const Int_t kiStart0rRpc   =  0;
const Int_t kiMidOrRpc     = 16;
const Int_t kiNbAndChanRpc = 16; // Not usable directly due to register map error
const Int_t kiStartAndRpc  =  1;
const Int_t kiMidAndRpc    =  9;

// 2 Plastics in same scaler board as TSUP and CRPC
const Int_t kiNbPmt = 2;
const Int_t kiPmtScalId = 2;
// B = Back, F = Front                  F      B     
const Int_t kiPlaScalCh[kiNbPmt]  = {   0,     7   };
const Double_t kdPmtArea[kiNbPmt] = {  16.0,  44.0 };
const TString  ksPmtName[kiNbPmt] = {  "F",   "B"  };

// 1 Diamond OR fed into a Free Trlo input
const Int_t    kiDiamScalId     =   6;
const Int_t    kiDiamScalCh     =   9;
const Double_t kdDiamArea       =   4.0;
const Int_t    kiSpillDistSec   =  30; // Approximate value, now just default
const Double_t kdSpillDiamThr   = 100.0; // 1/(s.cm^2)
const Double_t kdNoSpillDiamThr =  1e-5; // 1/(s.cm^2)

// With ROOT6 the function need to be defined before 1st call, as in C++ !!
void plot_Flux_Date_B(TString sInputName = "",
               Int_t iNbSecPerBin = 5, TTimeStamp tStartTime = 1425075060, // Start of rate data taking
               TTimeStamp tStopTime = 1425538380, // End of rate data taking
               Int_t iSpillDistSec= kiSpillDistSec,
               Int_t nEvents = -1);

// Overloaded functions used to get a user friendly date/time input as string
void plot_Flux_Date(TString sInputName = "/lustre/nyx/cbm/prod/beamtime/2015/11/cern/mbs_rates/unpack_rate_all.out.root",
               TString sStartDate = "2015-11-26 18:20:00",
               TString sStopDate  = "2015-12-01 08:00:00",
               Int_t iNbSecPerBin = 5,
               Int_t iSpillDistSec= kiSpillDistSec,
               Int_t nEvents = -1)
{
   // Use ROOT TDatime class for its nice string to date function
   TDatime dateStart(sStartDate);
   TDatime dateStop( sStopDate);
   cout<<dateStart.GetDate()<<" "<<dateStart.GetTime()<<endl;
   cout<<dateStop.GetDate() <<" "<<dateStop.GetTime() <<endl;

   // Convert the TDatime (rel. to 01/01/95) to UNIX-like TTimeStamp
   // TIP: need to explicitly add a 0 for the nanoseconds to be sure the right
   //      ctor is used!
   TTimeStamp timeStart( (time_t)( dateStart.Convert( kFALSE ) ), 0 );
   TTimeStamp timeStop(  (time_t)( dateStop.Convert(  kFALSE ) ), 0 );

   UInt_t uDay, uMonth, uYear;
   UInt_t uHour, uMin, uSec;
   timeStart.GetDate( kFALSE, 0, &uDay, &uMonth, &uYear);
   timeStart.GetTime( kFALSE, 0, &uHour, &uMin, &uSec);
   cout<<timeStart.GetSec()<<" "
       <<uDay<<"-"<<uMonth<<"-"<<uYear<<" "
       <<uHour<<":"<<uMin<<":"<<uSec<<endl;
   timeStop.GetDate( kFALSE, 0, &uDay, &uMonth, &uYear);
   timeStop.GetTime( kFALSE, 0, &uHour, &uMin, &uSec);
   cout<<timeStop.GetSec()<<" "
       <<uDay<<"-"<<uMonth<<"-"<<uYear<<" "
       <<uHour<<":"<<uMin<<":"<<uSec<<endl;

   // Set time offset for histograms to 0 => Start on 01/01/1970 as we use
   // are the UNIX time frame!
   gStyle->SetTimeOffset(0);

   // Do real call to function
   plot_Flux_Date_B( sInputName, iNbSecPerBin, timeStart, timeStop, iSpillDistSec, nEvents );
}

// Max nEvents: 198999999999
void plot_Flux_Date_B(TString sInputName, Int_t iNbSecPerBin, 
               TTimeStamp tStartTime, TTimeStamp tStopTime,
               Int_t iSpillDistSec,Int_t nEvents)
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
   Int_t iNbBins       = (Int_t)(tStopTime.GetSec() - tStartTime.GetSec()) /iNbSecPerBin;
   Double_t dStartTime = tStartTime.GetSec();
   Double_t dStopTime  = tStopTime.GetSec();
   Double_t dLastTime = 0;
   Double_t dLastTimeToLast = 0;
   
   Double_t dMeanFluxRpc[kiNbRpc];
   TProfile* tFluxRpc[kiNbRpc];
   for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
   {
      tFluxRpc[iRpc] = new TProfile( Form("tFlux%s", ksRpcName[iRpc].Data()),
                                          "", iNbBins, dStartTime, dStopTime);
      tFluxRpc[iRpc]->GetXaxis()->SetTimeDisplay(1);
      tFluxRpc[iRpc]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   } // for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
/*   
   TProfile* tFluxHdP2 = new TProfile("tFluxHdP2","", iNbBins, dStartTime, dStopTime);
   tFluxHdP2->GetXaxis()->SetTimeDisplay(1);
   tFluxHdP2->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile* tFluxHdP5 = new TProfile("tFluxHdP5","", iNbBins, dStartTime, dStopTime);
   tFluxHdP5->GetXaxis()->SetTimeDisplay(1);
   tFluxHdP5->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile* tFluxBRef = new TProfile("tFluxBRef","", iNbBins, dStartTime, dStopTime);
   tFluxBRef->GetXaxis()->SetTimeDisplay(1);
   tFluxBRef->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile* tFluxB13  = new TProfile("tFluxB13", "", iNbBins, dStartTime, dStopTime);
   tFluxB13->GetXaxis()->SetTimeDisplay(1);
   tFluxB13->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile* tFluxTsuS = new TProfile("tFluxTsuS","", iNbBins, dStartTime, dStopTime);
   tFluxTsuS->GetXaxis()->SetTimeDisplay(1);
   tFluxTsuS->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile* tFluxTsuP = new TProfile("tFluxTsuP","", iNbBins, dStartTime, dStopTime);
   tFluxTsuP->GetXaxis()->SetTimeDisplay(1);
   tFluxTsuP->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile* tFluxUstc = new TProfile("tFluxUstc","", iNbBins, dStartTime, dStopTime);
   tFluxUstc->GetXaxis()->SetTimeDisplay(1);
   tFluxUstc->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   TProfile* tFluxDiam = new TProfile("tFluxDiam","", iNbBins, dStartTime, dStopTime);
   tFluxDiam->GetXaxis()->SetTimeDisplay(1);
   tFluxDiam->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   Double_t dMeanFluxHdP2   = 0.0;
   Double_t dMeanFluxHdP5   = 0.0;
   Double_t dMeanFluxBRef   = 0.0;
   Double_t dMeanFluxB2013  = 0.0;
   Double_t dMeanFluxTsuStr = 0.0;
   Double_t dMeanFluxTsuPad = 0.0;
   Double_t dMeanFluxUstc   = 0.0;
*/

   TProfile* tFluxDiam = new TProfile("tFluxDiam","", iNbBins, dStartTime, dStopTime);
   tFluxDiam->GetXaxis()->SetTimeDisplay(1);
   tFluxDiam->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   Double_t dMeanFluxDiam   = 0.0;

   Double_t dMeanFluxPmt[kiNbPmt];
   TProfile* tFluxPmt[kiNbPmt];
   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
   {
      tFluxPmt[iPmt] = new TProfile( Form("tFluxPmt%s", ksPmtName[iPmt].Data()),
                                          "", iNbBins, dStartTime, dStopTime);
      tFluxPmt[iPmt]->GetXaxis()->SetTimeDisplay(1);
      tFluxPmt[iPmt]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   }
/*   
   tFluxPmt[0] = new TProfile("tFluxPmtTF","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[0]->GetXaxis()->SetTimeDisplay(1);
   tFluxPmt[0]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   tFluxPmt[1] = new TProfile("tFluxPmtTB","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[1]->GetXaxis()->SetTimeDisplay(1);
   tFluxPmt[1]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   tFluxPmt[2] = new TProfile("tFluxPmtBF","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[2]->GetXaxis()->SetTimeDisplay(1);
   tFluxPmt[2]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   tFluxPmt[3] = new TProfile("tFluxPmtBB","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[3]->GetXaxis()->SetTimeDisplay(1);
   tFluxPmt[3]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   tFluxPmt[4] = new TProfile("tFluxPmtT","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[4]->GetXaxis()->SetTimeDisplay(1);
   tFluxPmt[4]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   tFluxPmt[5] = new TProfile("tFluxPmtB","", iNbBins, dStartTime, dStopTime);
   tFluxPmt[5]->GetXaxis()->SetTimeDisplay(1);
   tFluxPmt[5]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
*/

   Int_t iNbBinsSpill   = (Int_t)(tStopTime.GetSec() - tStartTime.GetSec()) /iSpillDistSec;
   TProfile* tSpillFluxRpc[kiNbRpc];
   for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
   {
      tSpillFluxRpc[iRpc] = new TProfile( Form("tSpillFlux%s", ksRpcName[iRpc].Data()),
                                          "", iNbBinsSpill, dStartTime, dStopTime);
      tSpillFluxRpc[iRpc]->GetXaxis()->SetTimeDisplay(1);
      tSpillFluxRpc[iRpc]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   }
   TProfile* tSpillFluxPmt[kiNbPmt];
   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
   {
      tSpillFluxPmt[iPmt] = new TProfile( Form("tSpillFluxPmt%s", ksPmtName[iPmt].Data()),
                                          "", iNbBinsSpill, dStartTime, dStopTime);
      tSpillFluxPmt[iPmt]->GetXaxis()->SetTimeDisplay(1);
      tSpillFluxPmt[iPmt]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   }

   TProfile* tNoSpillFluxRpc[kiNbRpc];
   for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
   {
      tNoSpillFluxRpc[iRpc] = new TProfile( Form("tNoSpillFlux%s", ksRpcName[iRpc].Data()),
                                          "", iNbBinsSpill, dStartTime, dStopTime);
      tNoSpillFluxRpc[iRpc]->GetXaxis()->SetTimeDisplay(1);
      tNoSpillFluxRpc[iRpc]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   }
   TProfile* tNoSpillFluxPmt[kiNbPmt];
   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
   {
      tNoSpillFluxPmt[iPmt] = new TProfile( Form("tNoSpillFluxPmt%s", ksPmtName[iPmt].Data()),
                                          "", iNbBinsSpill, dStartTime, dStopTime);
      tNoSpillFluxPmt[iPmt]->GetXaxis()->SetTimeDisplay(1);
      tNoSpillFluxPmt[iPmt]->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   }
   
   TProfile* tSpillFluxDiam = new TProfile("tSpillFluxDiam","", iNbBins, dStartTime, dStopTime);
   tSpillFluxDiam->GetXaxis()->SetTimeDisplay(1);
   tSpillFluxDiam->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   
   TProfile* tNoSpillFluxDiam = new TProfile("tNoSpillFluxDiam","", iNbBins, dStartTime, dStopTime);
   tNoSpillFluxDiam->GetXaxis()->SetTimeDisplay(1);
   tNoSpillFluxDiam->GetXaxis()->SetTimeFormat("#splitline{%d\/%m}{%H:%M}");
   
   // Loop over measurements
   Long64_t lBranchEntries = tBranch->GetEntries();
   std::cout << lBranchEntries << " Entries in total in this file" << std::endl;
   for(Long64_t lBranchEntry = 0; lBranchEntry < lBranchEntries; lBranchEntry++)
   {
      if(0 == lBranchEntry%10000 && 0 < lBranchEntry)
         cout<<"Event "<<lBranchEntry<<" / "<< lBranchEntries <<" done!"<<endl;
      tArrayTrlo->Clear("C");
      tArray->Clear("C");

      tBranchTrlo->GetEntry(lBranchEntry);
      tBranch->GetEntry(lBranchEntry);

/*
      dMeanFluxHdP2   = 0.0;
      dMeanFluxHdP5   = 0.0;
      dMeanFluxBRef   = 0.0;
      dMeanFluxB2013  = 0.0;
      dMeanFluxTsuStr = 0.0;
      dMeanFluxTsuPad = 0.0;
      dMeanFluxUstc   = 0.0;
      dMeanFluxDiamA  = 0.0;
*/
      for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
         dMeanFluxRpc[iRpc] = 0.0;
      for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
         dMeanFluxPmt[iPmt] = 0.0;
      dMeanFluxDiam = 0.0;

      tTriglogBoard = (TTofTriglogBoard*)tArrayTrlo->At(0);

      // TIP: need to explicitly add a 0 for the nanoseconds to be sure the right
      //      ctor is used!
      TTimeStamp tEventTime( (time_t)(tTriglogBoard->GetMbsTimeSec()), 0 );

      // Jump events before chosen start time
      if( tEventTime < tStartTime )
         continue;
      // Stop looping when chosen stop time is reached
      if( tStopTime < tEventTime )
         break;

      // Board 0 is TRIGLOG for trigger
      tCalibTriglog = (TTofCalibScaler*)tArray->At(0);
      // Next boards are Scaler for the RPC
      for( Int_t iScal = 0; iScal < kiNbScalers; iScal++)
         tCalibScaler[iScal] = (TTofCalibScaler*)tArray->At(kiScalerIndex[iScal]);
      // Last Board is a TRIGLOG used as Scaler for PMT
//      tCalibTrloScal = (TTofCalibScaler*)tArray->At(1 + kiNbScalers);
/*
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
//         dMeanFluxB2013  += tCalibScaler[ kiRpcScalId[3] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );
         dMeanFluxTsuPad += tCalibScaler[ kiRpcScalId[5] ]->GetScalerValue( kiNbOrChanRpc + iAndChan );
      } // for(Int_t iAndCHan = 0; iAndChan < kiNbAndChanRpc; iAndChan ++)
*/
      // For each RPC, loop on the corresponding AND channels and sum them
      // (taking into account the offset of 1 channel due tot readout bug)
      for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
      {
         Int_t iChan = kiRpcAndChSta[ iRpc ];
         if( -1 < iChan )
         {
            // This case correspond to RPCs where we use the AND/Or intermediate scalers
            while( iChan <= kiRpcAndChSto[ iRpc ] )
            {
               dMeanFluxRpc[iRpc] += tCalibScaler[ kiRpcScalId[iRpc] ]->GetScalerValue( kiNbOrChanRpc + iChan );
               iChan++;
            } // while( iChan <= kiRpcAndChSta[ iRpc ] )
         } // if( -1 < iChan )
            else
            {
               // This case correspond to RPCs where we got directly to the LEMO out => use the input scalers
               iChan = kiRpcInChSta[ iRpc ];
               while( iChan <= kiRpcInChSto[ iRpc ] )
               {
                  dMeanFluxRpc[iRpc] += tCalibScaler[ kiRpcScalId[iRpc] ]->GetScalerValue( iChan );
                  iChan++;
               } // while( iChan <= kiRpcAndChSta[ iRpc ] )
            } // else of  if( -1 < iChan )
         dMeanFluxRpc[iRpc]   /= kdRpcArea[iRpc];
         tFluxRpc[iRpc]->Fill( tEventTime.GetSec(), dMeanFluxRpc[iRpc] );
      } // for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
/*********************************************************************************/
/*
      dMeanFluxHdP2   /= kdRpcArea[0];
      dMeanFluxHdP5   /= kdRpcArea[1];
      dMeanFluxBRef   /= kdRpcArea[2];
      dMeanFluxB2013  /= kdRpcArea[3];
      dMeanFluxTsuStr /= kdRpcArea[4];
      dMeanFluxTsuPad /= kdRpcArea[5];
      dMeanFluxUstc   /= kdRpcArea[6];

      tFluxHdP2->Fill( tEventTime.GetSec(), dMeanFluxHdP2 );
      tFluxHdP5->Fill( tEventTime.GetSec(), dMeanFluxHdP5 );
      tFluxBRef->Fill( tEventTime.GetSec(), dMeanFluxBRef );
      tFluxB13->Fill(  tEventTime.GetSec(), dMeanFluxB2013 );
      tFluxTsuS->Fill( tEventTime.GetSec(), dMeanFluxTsuStr );
      tFluxTsuP->Fill( tEventTime.GetSec(), dMeanFluxTsuPad );
      tFluxUstc->Fill( tEventTime.GetSec(), dMeanFluxUstc );
      tFluxDiam->Fill( tEventTime.GetSec(), dMeanFluxDiamA );
*/

      for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
      {
         dMeanFluxPmt[iPmt] = tCalibScaler[ kiPmtScalId ]->GetScalerValue( kiPlaScalCh[iPmt] ) / kdPmtArea[iPmt];
         tFluxPmt[iPmt]->Fill( tEventTime.GetSec(), dMeanFluxPmt[iPmt] );
      } // for( Int_t iPmt = 0; iPmt < kiNbRpc; iPmt++)
      
      dMeanFluxDiam += tCalibScaler[ kiDiamScalId ]->GetScalerValue( kiDiamScalCh );
      dMeanFluxDiam /= kdDiamArea;
      tFluxDiam->Fill( tEventTime.GetSec(), dMeanFluxDiam );

      // In spill mean value
/*
      dMeanFluxRpc[0] = dMeanFluxHdP2;
      dMeanFluxRpc[1] = dMeanFluxHdP5;
      dMeanFluxRpc[2] = dMeanFluxBRef;
      dMeanFluxRpc[3] = dMeanFluxB2013;
      dMeanFluxRpc[4] = dMeanFluxTsuStr;
      dMeanFluxRpc[5] = dMeanFluxTsuPad;
      dMeanFluxRpc[6] = dMeanFluxUstc;
      dMeanFluxRpc[7] = dMeanFluxDiamA;
*/
      if( kdSpillDiamThr <= dMeanFluxDiam )
      {
         for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
            tSpillFluxRpc[iRpc]->Fill( tEventTime.GetSec(), dMeanFluxRpc[iRpc] );
         for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
            tSpillFluxPmt[iPmt]->Fill( tEventTime.GetSec(), dMeanFluxPmt[iPmt] );
         tSpillFluxDiam->Fill( tEventTime.GetSec(), dMeanFluxDiam );
      } // if( kdSpillDiamThr <= dMeanFluxDiam )
         else if( dMeanFluxDiam < kdNoSpillDiamThr )
         {
            for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
               tNoSpillFluxRpc[iRpc]->Fill( tEventTime.GetSec(), dMeanFluxRpc[iRpc] );
            for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
               tNoSpillFluxPmt[iPmt]->Fill( tEventTime.GetSec(), dMeanFluxPmt[iPmt] );
            tNoSpillFluxDiam->Fill( tEventTime.GetSec(), dMeanFluxDiam );
         } // else if( dMeanFluxDiam < kdNoSpillDiamThr )

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


   delete gROOT->FindObjectAny( "tCanvasRpcA" );
   TCanvas* tCanvasRpcA = new TCanvas("tCanvasRpcA","Rpc Vs Time",0,0,1400,700);
   tCanvasRpcA->Divide(4, 2);

   tCanvasRpcA->cd(1);
//   tFluxHdP2->Draw("h");
   tFluxRpc[0]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcA->cd(2);
//   tFluxHdP5->Draw("h");
   tFluxRpc[1]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcA->cd(3);
//   tFluxBRef->Draw("h");
   tFluxRpc[2]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcA->cd(4);
//   tFluxB13->Draw("h");
   tFluxRpc[3]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcA->cd(5);
//   tFluxTsuS->Draw("h");
   tFluxRpc[4]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcA->cd(6);
//   tFluxTsuP->Draw("h");
   tFluxRpc[5]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcA->cd(7);
//   tFluxUstc->Draw("h");
   tFluxRpc[6]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcA->cd(8);
//   tFluxDiam->Draw("h");
   tFluxRpc[7]->Draw("h");
   gPad->SetLogy();
   
   delete gROOT->FindObjectAny( "tCanvasRpcB" );
   TCanvas* tCanvasRpcB = new TCanvas("tCanvasRpcB","Rpc Vs Time",0,0,1400,700);
   tCanvasRpcB->Divide(4, 2);

   tCanvasRpcB->cd(1);
   tFluxRpc[ 8]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcB->cd(2);
   tFluxRpc[ 9]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcB->cd(3);
   tFluxRpc[10]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcB->cd(4);
   tFluxRpc[11]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcB->cd(5);
   tFluxRpc[12]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcB->cd(6);
   tFluxRpc[13]->Draw("h");
   gPad->SetLogy();

   tCanvasRpcB->cd(8);
   tFluxDiam->Draw("h");
   gPad->SetLogy();
/*
   delete gROOT->FindObjectAny( "tCanvas2" );
   TCanvas* tCanvas2 = new TCanvas("tCanvas2","Setups Vs Time",0,0,1400,700);
   tCanvas2->Divide(2, 1);

   tCanvas2->cd(1);
   TH1 *frameA = gPad->DrawFrame(dStartTime, 1, dStopTime, 1e5);
//   frameA->GetXaxis()->SetTitle("X title");
//   frameA->GetYaxis()->SetTitle("Y title");
   frameA->GetXaxis()->SetTimeDisplay(1);
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
   frameB->GetXaxis()->SetTimeDisplay(1);
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
*/
   delete gROOT->FindObjectAny( "tCanvas3" );
   TCanvas* tCanvas3 = new TCanvas("tCanvas3","Plastics Vs time",0,0,1400,700);
   tCanvas3->Divide( kiNbPmt + 1 );

   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
   {
      tCanvas3->cd(1+iPmt);
      tFluxPmt[iPmt]->Draw("h");
      gPad->SetLogy();
   } // // for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)

   tCanvas3->cd( kiNbPmt + 1 );
   TH1 *frameC = gPad->DrawFrame(dStartTime, 1e-1, dStopTime, 1.5e6);
   frameC->GetXaxis()->SetTimeDisplay(1);
   gPad->Update();
   gPad->SetLogy();
   tFluxPmt[0]->SetLineColor( kBlack);
   tFluxPmt[0]->Draw("hsame");
   tFluxPmt[1]->SetLineColor( kBlue);
   tFluxPmt[1]->Draw("hsame");


   delete gROOT->FindObjectAny( "tCanvas4" );
   TCanvas* tCanvas4 = new TCanvas("tCanvas4","Rpc in Spill",0,0,1400,700);
   tCanvas4->Divide(4, 4);

   for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
   {
      tCanvas4->cd(1+iRpc);
      tSpillFluxRpc[iRpc]->Draw("h");
      gPad->SetLogy();
   } // // for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)

   tCanvas4->cd(16);
   tSpillFluxDiam->Draw("h");
   gPad->SetLogy();

   delete gROOT->FindObjectAny( "tCanvas5" );
   TCanvas* tCanvas5 = new TCanvas("tCanvas5","Pmt in Spill",0,0,1400,700);
   tCanvas5->Divide( kiNbPmt );

   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
   {
      tCanvas5->cd(1+iPmt);
      tSpillFluxPmt[iPmt]->Draw("h");
      gPad->SetLogy();
   } // // for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)

   delete gROOT->FindObjectAny( "tCanvasNoSpRpc" );
   TCanvas* tCanvasNoSpRpc = new TCanvas("tCanvasNoSpRpc","Rpc out of Spill",0,0,1400,700);
   tCanvasNoSpRpc->Divide(4, 4);

   for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)
   {
      tCanvasNoSpRpc->cd(1+iRpc);
      tNoSpillFluxRpc[iRpc]->Draw("h");
      gPad->SetLogy();
   } // // for( Int_t iRpc = 0; iRpc < kiNbRpc; iRpc++)

   tCanvasNoSpRpc->cd(16);
   tNoSpillFluxDiam->Draw("h");
   gPad->SetLogy();
   
   delete gROOT->FindObjectAny( "tCanvasNoSpPmt" );
   TCanvas* tCanvasNoSpPmt = new TCanvas("tCanvasNoSpPmt","Pmt out of Spill",0,0,1400,700);
   tCanvasNoSpPmt->Divide( kiNbPmt );

   for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
   {
      tCanvasNoSpPmt->cd(1+iPmt);
      tNoSpillFluxPmt[iPmt]->Draw("h");
      gPad->SetLogy();
   } // // for( Int_t iPmt = 0; iPmt < kiNbPmt; iPmt++)
}
