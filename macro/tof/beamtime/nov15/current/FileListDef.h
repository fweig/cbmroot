// Definitions for the HV monitoring in Nov. 2015 beamtime at CERN SPS
// 15/12/2015, P.-A. Loizeau

/************************ Files definitions *************************/
// 2015-11-26 1
// 261115
//   1820

// 2015-11-27 2
// 271115 271115
//   1142   1901

// 2015-11-28 1
// 281115
//   0803

// 2015-11-29 1
// 291115 
//   1602   

// 2015-11-30 3
// 301115 301115 301115
//   0030   1046   2019

const Int_t   kiNbFiles  = 1 + 2 + 1 + 1 + 3;
const TString ksFilesDay[kiNbFiles]  = { "261115",
                                         "271115", "271115",
                                         "281115",
                                         "291115",
                                         "301115", "301115", "301115" };
const TString ksFilesHour[kiNbFiles] = {   "1820",
                                           "1142",   "1901",
                                           "0803",
                                           "1602",
                                           "0030",   "1046",   "2019" };

/************************ Boards definitions ************************/
const Int_t kiNbBoardsUsed = 8;
   // Slot in crate
Int_t kiSlotIndex[kiNbBoardsUsed] = { 0,  2, 4, 6,  8, 10, 13, 14};
   // Offset in file size in Byte
std::streampos kiSlotSzOff[kiNbBoardsUsed] = { 0,  0, 0, 0,  0,  1,  2,  2};

/*********************** Detectors definitions **********************/
const Int_t   kiNbRpc         = 17;
TString sChNameRpc[kiNbRpc]   = {  "HD_P2", "HD_P5", "TSU_ds", "TSU_du", "TSU_su", "TSU_PAD", "USTC_du", "USTC_sss", "USTC_ssu", "B_2015_SS", "B_2015_DS", "B_2012", "BUC_Ref", "BUC_2013", "BUC_2010", "CRPC_0", "CRPC_1"};
Int_t   iSlotRpcNeg[kiNbRpc]  = {       0 ,      0 ,       0 ,       0 ,       0 ,        0 ,        2 ,         2 ,         2 ,          2 ,          2 ,       2 ,        4 ,         4 ,         4 ,       4 ,       4 };
Int_t   iBdIdxRpcNeg[kiNbRpc] = {       0 ,      0 ,       0 ,       0 ,       0 ,        0 ,        4 ,         4 ,         4 ,          4 ,          4 ,       4 ,        8 ,         8 ,         8 ,       8 ,       8 };
Int_t   iChIdxRpcNeg[kiNbRpc] = {       0 ,      1 ,       2 ,       3 ,       4 ,        5 ,        0 ,         1 ,         2 ,          3 ,          4 ,       5 ,        0 ,         1 ,         2 ,       4 ,       5 };
Int_t   iSlotRpcPos[kiNbRpc]  = {       1 ,      1 ,       1 ,       1 ,       1 ,        1 ,        3 ,         3 ,         3 ,          3 ,          3 ,       3 ,        5 ,         5 ,         5 ,       5 ,       5 };
Int_t   iBdIdxRpcPos[kiNbRpc] = {       2 ,      2 ,       2 ,       2 ,       2 ,        2 ,        6 ,         6 ,         6 ,          6 ,          6 ,       6 ,       10 ,        10 ,        10 ,      10 ,      10 };
Int_t   iChIdxRpcPos[kiNbRpc] = {       0 ,      1 ,       2 ,       3 ,       4 ,        5 ,        0 ,         1 ,         2 ,          3 ,          4 ,       5 ,        0 ,         1 ,         2 ,       4 ,       5 };

const Int_t   kiPmtOffset = 20;
const Int_t   kiNbPmt         = 10;
TString sChNamePmt[kiNbPmt]   = {   "PMT1",  "PMT2",   "PMT3",   "PMT4",    "BFL",    "BFR",     "BBL",      "BBR",     "PMT5", "Dia_PMT" };
Int_t   iSlotPmtNeg[kiNbPmt]  = {       6 ,      6 ,       6 ,       6 ,       7 ,       7 ,        7 ,         7 ,         7 ,         7 };
Int_t   iBdIdxPmtNeg[kiNbPmt] = {      13 ,     13 ,      13 ,      13 ,      14 ,      14 ,       14 ,        14 ,        14 ,        14 };
Int_t   iChIdxPmtNeg[kiNbPmt] = {       0 ,      1 ,       4 ,       5 ,       0 ,       1 ,        2 ,         3 ,         4 ,         5 };

const Int_t   kiTrdOffset = 40;
const Int_t   kiNbTrd         = 0;
/*
TString sChNameTrd[kiNbTrd]   = { "T_2010", "T_2012_1", "T_2012_2", "T_2012_3", "T_2012_4" };
Int_t   iSlotTrdNeg[kiNbTrd]  = {       6 ,         6 ,         6 ,         6 ,         6  };
Int_t   iBdIdxTrdNeg[kiNbTrd] = {       8 ,         8 ,         8 ,         8 ,         8 };
Int_t   iChIdxTrdNeg[kiNbTrd] = {       0 ,         1 ,         1 ,         1 ,         1 };
Int_t   iSlotTrdPos[kiNbTrd]  = {       7 ,         7 ,         7 ,         7 ,         7 };
Int_t   iBdIdxTrdPos[kiNbTrd] = {      10 ,        10 ,        10 ,        10 ,        10 };
Int_t   iChIdxTrdPos[kiNbTrd] = {       0 ,         1 ,         2 ,         3 ,         4 };
*/

const Int_t    kiSpillDistSec   =  30;  // Approximate value, now just default
const Double_t kdSpillDiamThr   =  1e-4; // µA
const Double_t kdNoSpillDiamThr =  1e-5; // µA

void PrintHelp()
{
   cout<<"Please provide a detector index, a data, a start time and a stop time as input!!!!"<<endl;
   cout<<"Example:        root -l \'XXXXXXXX.C( 3, \"2015-11-27 19:30:00\", \"2015-11-27 20:00:00\", \"YYY.root\"  )\' "<<endl;
   cout<<"or within root: .x currentConv.C( 1 ) "<<endl;
   cout<<"Possible detector values are: "<<endl;
   cout<<" 0 for HD RPC-P2 "  <<endl;
   cout<<" 1 for HD RPC-P5 "  <<endl;
   cout<<" 2 for TSU ds "     <<endl;
   cout<<" 3 for TSU du "     <<endl;
   cout<<" 4 for TSU su "     <<endl;
   cout<<" 5 for TSU PAD "    <<endl;
   cout<<" 6 for USTC du "    <<endl;
   cout<<" 7 for USTC sss "   <<endl;
   cout<<" 8 for USTC ssu "   <<endl;
   cout<<" 9 for BUC 2015 SS" <<endl;
   cout<<"10 for BUC 2015 SU" <<endl;
   cout<<"11 for BUC 2012 "   <<endl;
   cout<<"12 for BUC ref "    <<endl;
   cout<<"13 for BUC 2013 "   <<endl;
   cout<<"14 for BUC 2010 "   <<endl;
   cout<<"15 for Cer RPC #0 " <<endl;
   cout<<"16 for Cer RPC #1 " <<endl;
   cout<<"-----------------"  <<endl;
   cout<<"20 for PMT #1" <<endl;
   cout<<"21 for PMT #2"<<endl;
   cout<<"22 for PMT #3" <<endl;
   cout<<"23 for PMT #4" <<endl;
   cout<<"24 for PMT Bot Front Left"  <<endl;
   cout<<"25 for PMT Bot Front Right"  <<endl;
   cout<<"26 for PMT Bot Back  Left"  <<endl;
   cout<<"27 for PMT Top Back  Right"  <<endl;
   cout<<"28 for PMT5"  <<endl;
   cout<<"29 for PMT Diamond"  <<endl;
   cout<<"-----------------"<<endl;
   /*
   cout<<"40 for TRD 2010" <<endl;
   cout<<"41 for TRD 2012 Neg or TRD 2012 #1 Pos"<<endl;
   cout<<"42 for TRD 2012 Neg or TRD 2012 #2 Pos" <<endl;
   cout<<"43 for TRD 2012 Neg or TRD 2012 #3 Pos" <<endl;
   cout<<"44 for TRD 2012 Neg or TRD 2012 #4 Pos"<<endl;
   cout<<"-----------------"<<endl;
   */
   cout<<"The date format is: YYYYMMDD"<<endl;
   cout<<"The time format is: HHMM"<<endl;
}
