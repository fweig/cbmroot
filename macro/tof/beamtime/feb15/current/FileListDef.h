// Definitions for the HV monitoring in Feb. 2015 beamtime at CERN SPS
// 22/04/2015, P.-A. Loizeau

/************************ Files definitions *************************/
// 2015-02-24 3
// 240215 240215 240215
//   0957   1941   2224

// 2015-02-25 2
// 250215 250215
//   1026   2024

// 2015-02-26 4
// 260215 260215 260215 260215
//   0018   0949   1420   1831

// 2015-02-27 2
// 270215 270215
//   0930   1715

// 2015-02-28 2
// 280215 280215
//   1401   2133

// 2015-03-01 5
// 010315 010315 010315 010315 010315
//   0947   1509   1512   2022   2332

// 2015-03-02 12
// 020315 020315 020315 020315 020315 020315 020315 020315 020315 020315 020315 020315
//   1102   1417   1940   2002   2025   2032   2045   2056   2144   2202   2228   2248

// 2015-03-03 3
// 030315 030315 030315
//   0141   1423   2214

// 2015-03-04 2
// 040315 040315
//   0252   1717

const Int_t   kiNbFiles  = 3 + 2 + 4 + 2 + 2 + 5 + 12 + 3 + 2;
const TString ksFilesDay[kiNbFiles]  = { "240215", "240215", "240215",
                                         "250215", "250215",
                                         "260215", "260215", "260215", "260215",
                                         "270215", "270215",
                                         "280215", "280215",
                                         "010315", "010315", "010315", "010315", "010315",
                                         "020315", "020315", "020315", "020315", "020315", "020315",
                                         "020315", "020315", "020315", "020315", "020315", "020315",
                                         "030315", "030315", "030315",
                                         "040315", "040315" };
const TString ksFilesHour[kiNbFiles] = {   "0957",   "1941",   "2224",
                                           "1026",   "2024",
                                           "0018",   "0949",   "1420",   "1831",
                                           "0930",   "1715",
                                           "1401",   "2133",
                                           "0947",   "1509",   "1512",   "2022",   "2332",
                                           "1102",   "1417",   "1940",   "2002",   "2025",   "2032",
                                           "2045",   "2056",   "2144",   "2202",   "2228",   "2248",
                                           "0141",   "1423",   "2214",
                                           "0252",   "1717" };

/************************ Boards definitions ************************/
const Int_t kiNbBoardsUsed = 8;
   // Slot in crate
Int_t kiSlotIndex[kiNbBoardsUsed] = { 0,  4, 2, 6, 13, 14,  8, 10};
   // Offset in file size in Byte
std::streampos kiSlotSzOff[kiNbBoardsUsed] = { 0,  0, 0, 0,  2,  2,  0,  1};

/*********************** Detectors definitions **********************/
const Int_t   kiNbRpc         = 12;
TString sChNameRpc[kiNbRpc]   = {  "HD_P2", "HD_P5", "------",  "USTC", "TS_STR", "TS_PAD", "BUC_Ref", "BUC_2013", "B_2012_1", "B_2012_2", "B_2012_3", "B_2012_4"};
Int_t   iSlotRpcNeg[kiNbRpc]  = {       0 ,      0 ,       0 ,      0 ,       0 ,       0 ,        1 ,         1 ,         1 ,         1 ,         1 ,         1 };
Int_t   iBdIdxRpcNeg[kiNbRpc] = {       0 ,      0 ,       0 ,      0 ,       0 ,       0 ,        4 ,         4 ,         4 ,         4 ,         4 ,         4 };
Int_t   iChIdxRpcNeg[kiNbRpc] = {       0 ,      1 ,       2 ,      3 ,       4 ,       5 ,        0 ,         1 ,         2 ,         3 ,         4 ,         5 };
Int_t   iSlotRpcPos[kiNbRpc]  = {       2 ,      2 ,       2 ,      2 ,       2 ,       2 ,        3 ,         3 ,         3 ,         3 ,         3 ,         3 };
Int_t   iBdIdxRpcPos[kiNbRpc] = {       2 ,      2 ,       2 ,      2 ,       2 ,       2 ,        6 ,         6 ,         6 ,         6 ,         6 ,         6 };
Int_t   iChIdxRpcPos[kiNbRpc] = {       0 ,      1 ,       2 ,      3 ,       4 ,       5 ,        0 ,         1 ,         2 ,         3 ,         4 ,         5 };

const Int_t   kiPmtOffset = 20;
const Int_t   kiNbPmt         = 11;
TString sChNamePmt[kiNbPmt]   = {    "HFL",   "HFR",   "calo",   "HBL",    "HBR", "BFL", "BFR", "BBL", "BBR", "PMT5", "Dia_PMT" };
Int_t   iSlotPmtNeg[kiNbPmt]  = {       4 ,      4 ,       4 ,      4 ,       4 ,    5 ,    5 ,    5 ,    5 ,     5 ,         5 };
Int_t   iBdIdxPmtNeg[kiNbPmt] = {      13 ,     13 ,      13 ,     13 ,      13 ,   14 ,   14 ,   14 ,   14 ,    14 ,        14 };
Int_t   iChIdxPmtNeg[kiNbPmt] = {       0 ,      1 ,       2 ,      4 ,       5 ,    0 ,    1 ,    2 ,    3 ,     4 ,         5 };

const Int_t   kiTrdOffset = 40;
const Int_t   kiNbTrd         = 5;
TString sChNameTrd[kiNbTrd]   = { "T_2010", "T_2012_1", "T_2012_2", "T_2012_3", "T_2012_4" };
Int_t   iSlotTrdNeg[kiNbTrd]  = {       6 ,         6 ,         6 ,         6 ,         6  };
Int_t   iBdIdxTrdNeg[kiNbTrd] = {       8 ,         8 ,         8 ,         8 ,         8 };
Int_t   iChIdxTrdNeg[kiNbTrd] = {       0 ,         1 ,         1 ,         1 ,         1 };
Int_t   iSlotTrdPos[kiNbTrd]  = {       7 ,         7 ,         7 ,         7 ,         7 };
Int_t   iBdIdxTrdPos[kiNbTrd] = {      10 ,        10 ,        10 ,        10 ,        10 };
Int_t   iChIdxTrdPos[kiNbTrd] = {       0 ,         1 ,         2 ,         3 ,         4 };

const Int_t    kiSpillDistSec   =  30;  // Approximate value, now just default
const Double_t kdSpillDiamThr   =  1e-4; // µA
const Double_t kdNoSpillDiamThr =  1e-5; // µA

void PrintHelp()
{
   cout<<"Please provide a detector index, a data, a start time and a stop time as input!!!!"<<endl;
   cout<<"Example:        root -l \'XXXXXXXX.C( 3, \"2015-03-04 19:30:00\", \"2015-03-04 20:00:00\", \"YYY.root\"  )\' "<<endl;
   cout<<"or within root: .x currentConv.C( 1 ) "<<endl;
   cout<<"Possible detector values are: "<<endl;
   cout<<" 0 for HD RPC-P3 "<<endl;
   cout<<" 1 for HD RPC-P2 "<<endl;
   cout<<" 2 for HD RPC-P5 "<<endl;
   cout<<" 3 for USTC "     <<endl;
   cout<<" 4 for TSU Strip "<<endl;
   cout<<" 5 for TSU PAD "  <<endl;
   cout<<" 6 for BUC ref "  <<endl;
   cout<<" 7 for BUC 2013 " <<endl;
   cout<<" 8 for BUC 2012 #1 "<<endl;
   cout<<" 9 for BUC 2012 #2 "<<endl;
   cout<<"10 for BUC 2012 #3 "<<endl;
   cout<<"11 for BUC 2012 #4 "<<endl;
   cout<<"-----------------"<<endl;
   cout<<"20 for PMT Top Front Left" <<endl;
   cout<<"21 for PMT Top Front Right"<<endl;
   cout<<"22 for Pb Calorimeter" <<endl;
   cout<<"23 for PMT Top Back  Left" <<endl;
   cout<<"24 for PMT Top Back  Right"<<endl;
   cout<<"25 for PMT Bot Front Left"  <<endl;
   cout<<"26 for PMT Bot Front Right"  <<endl;
   cout<<"27 for PMT Bot Back  Left"  <<endl;
   cout<<"28 for PMT Top Back  Right"  <<endl;
   cout<<"29 for PMT5"  <<endl;
   cout<<"30 for PMT Diamond"  <<endl;
   cout<<"-----------------"<<endl;
   cout<<"40 for TRD 2010" <<endl;
   cout<<"41 for TRD 2012 Neg or TRD 2012 #1 Pos"<<endl;
   cout<<"42 for TRD 2012 Neg or TRD 2012 #2 Pos" <<endl;
   cout<<"43 for TRD 2012 Neg or TRD 2012 #3 Pos" <<endl;
   cout<<"44 for TRD 2012 Neg or TRD 2012 #4 Pos"<<endl;
   cout<<"-----------------"<<endl;
   cout<<"The date format is: YYYYMMDD"<<endl;
   cout<<"The time format is: HHMM"<<endl;
}
