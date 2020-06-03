#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class AnalysisCuts+;
#pragma link C++ class AnalysisFilter+;

#pragma link C++ class PairAnalysisTrack+;
#pragma link C++ class PairAnalysisEvent+;

#pragma link C++ class PairAnalysisPair+;
#pragma link C++ class PairAnalysisPairLV+;
#pragma link C++ class PairAnalysisPairKF+;

#pragma link C++ class PairAnalysisCutQA+;
#pragma link C++ class PairAnalysisPairLegCuts+;
#pragma link C++ class PairAnalysisVarCuts+;
#pragma link C++ class PairAnalysisObjectCuts+;
#pragma link C++ class PairAnalysisCutGroup+;
#pragma link C++ class PairAnalysisCutCombi+;


#pragma link C++ namespace PairAnalysisHelper;
#pragma link C++ namespace PairAnalysisStyler;

#pragma link C++ class PairAnalysisVarManager+;

#pragma link C++ class PairAnalysisMetaData+;



#pragma link C++ class PairAnalysis+;

#pragma link C++ class PairAnalysisHistos+;
#pragma link C++ class PairAnalysisHn+;
///////1D
//w/o weights
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*);
//w/ weights
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, char*);
// profs w/o weights
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, char*, TString);
//w/ weights
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, char*, TString, char*);
///////2D
//w/o weights
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*);
//w/ weights
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*, char*);
//profs w/o weights
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, char*, TString);
//w/ weights
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, char*, TString, char*);
////// 3D
//w/o weights
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*);
//w/ weights
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t, char*);
#pragma link C++ function PairAnalysisHistos::AddHistogram(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*, char*);
//profs w/o weights
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*, UInt_t, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t, char*, TString);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*, char*, TString);
//w/ weights
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*, UInt_t, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*, char*, TString, UInt_t);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*, UInt_t, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, UInt_t, TVectorD*, char*, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, UInt_t, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, UInt_t, TVectorD*, char*, TVectorD*, char*, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, UInt_t, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, UInt_t, TVectorD*, char*, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, UInt_t, char*, TString, char*);
#pragma link C++ function PairAnalysisHistos::AddProfile(char*, TVectorD*, char*, TVectorD*, char*, TVectorD*, char*, char*, TString, char*);

//#pragma link C++ class PairAnalysisHistosHelper+;

#pragma link C++ class PairAnalysisHF+;
//#pragma link C++ class PairAnalysisHFhelper+;
#pragma link C++ class PairAnalysisMC+;
#pragma link C++ class AnalysisTaskMultiPairAnalysis+;

#pragma link C++ class PairAnalysisSignalMC+;

#pragma link C++ class PairAnalysisTrackRotator+;
#pragma link C++ class PairAnalysisMixedEvent+;
#pragma link C++ class PairAnalysisMixingHandler+;

#pragma link C++ class PairAnalysisFunction+;
#pragma link C++ class PairAnalysisSignalExt+;
//#pragma link C++ class PairAnalysisSignalFit+;
#pragma link C++ class PairAnalysisSpectrum+;
#pragma link C++ class Extraction+;


#endif
