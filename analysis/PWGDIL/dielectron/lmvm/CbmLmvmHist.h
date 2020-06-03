#ifndef CBM_LMVM_HIST_H
#define CBM_LMVM_HIST_H

#include <vector>
#include <string>

/*
 * \brief Enumeration for different sources.
 */
enum CbmLmvmSourceTypes {
  kSignal = 0,
  kBg = 1,
  kPi0 = 2,
  kGamma = 3
};

/*
 * \brief Enumeration for analysis steps.
 */
enum CbmLmvmAnalysisSteps {
  kMc = 0,
  kAcc = 1,
  kReco = 2,
  kChi2Prim = 3,
  kElId = 4,
  kGammaCut = 5,
  kMvd1Cut = 6,
  kMvd2Cut = 7,
  kStCut = 8,
  kRtCut = 9,
  kTtCut = 10,
  kPtCut = 11
};

/*
 * \brief Enumeration for different sources of BG pairs
 */
enum CbmLmvmBgPairSource {
  kGG = 0, // gamma-gamma
  kPP = 1, // pi0-pi0
  kOO = 2, // other-other
  kGP = 3, // gamma-pi0
  kGO = 4, // gamma-other
  kPO = 5 // pi0-other
};

class CbmLmvmHist
{
public:

   const static int fNofSourceTypes = 4;
   const static std::vector<std::string> fSourceTypes;
   const static std::vector<std::string> fSourceTypesLatex;
   const static std::vector<int> fSourceTypesColor;

   const static int fNofAnaSteps = 12;
   const static std::vector<std::string> fAnaSteps;
   const static std::vector<std::string> fAnaStepsLatex;
   const static std::vector<int> fAnaStepsColor;

   const static std::vector<std::string> fSignalNames;

   const static int fNofBgPairSources = 6;
   const static std::vector<std::string> fBgPairSourceLatex;
};

#endif

