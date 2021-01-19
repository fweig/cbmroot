#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmDigiManager + ;
#pragma link C++ class CbmDigitizeBase + ;

#pragma link C++ class CbmMCDataArray;
#pragma link C++ class CbmMCDataManager;
#pragma link C++ class CbmMCDataObject;
#pragma link C++ class CbmRadDamage;

#pragma link C++ class CbmDaq + ;

#pragma link C++ function SetDefaultDrawStyle();
#pragma link C++ function DrawH1(TH1*,                                         \
                                 HistScale,                                    \
                                 HistScale,                                    \
                                 const string&,                                \
                                 Int_t,                                        \
                                 Int_t,                                        \
                                 Int_t,                                        \
                                 Int_t,                                        \
                                 Int_t);
#pragma link C++ function DrawH2(                                              \
  TH2*, HistScale, HistScale, HistScale, const string&);
#pragma link C++ function DrawH1(const vector <TH1*>&,                         \
                                 const vector <string>&,                       \
                                 HistScale,                                    \
                                 HistScale,                                    \
                                 Bool_t,                                       \
                                 Double_t,                                     \
                                 Double_t,                                     \
                                 Double_t,                                     \
                                 Double_t,                                     \
                                 const string&);
#pragma link C++ function DrawGraph(TGraph*,                                   \
                                    HistScale,                                 \
                                    HistScale,                                 \
                                    const string&,                             \
                                    Int_t,                                     \
                                    Int_t,                                     \
                                    Int_t,                                     \
                                    Int_t,                                     \
                                    Int_t);
#pragma link C++ function DrawGraph(const vector <TGraph*>&,                   \
                                    const vector <string>&,                    \
                                    HistScale,                                 \
                                    HistScale,                                 \
                                    Bool_t,                                    \
                                    Double_t,                                  \
                                    Double_t,                                  \
                                    Double_t,                                  \
                                    Double_t);
#pragma link C++ function DrawGraph2D(                                         \
  TGraph2D*, HistScale, HistScale, HistScale, const string&);

#pragma link C++ class CbmHistManager;
#pragma link C++ class CbmReport + ;
#pragma link C++ class CbmSimulationReport + ;
#pragma link C++ class CbmStudyReport + ;
#pragma link C++ class CbmReportElement + ;
#pragma link C++ class CbmTextReportElement + ;
#pragma link C++ class CbmLatexReportElement + ;
#pragma link C++ class CbmHtmlReportElement + ;

#pragma link C++ class CbmMatchRecoToMC + ;

#pragma link C++ class CbmMediaList;
// For some platforms (OS + compiler) without the following line the check_media.C macro
// can not read the file with the CbmMediaList object. For other platforms there is no
// problem without the line. So generate the correct streamer info in any way.
#pragma link C++ class std::pair < TString, TString>;

#pragma link C++ function Cbm::File::IsRootFile( string );
#endif
