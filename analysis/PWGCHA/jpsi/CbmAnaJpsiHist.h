#ifndef CBM_ANA_JPSI_HIST_H
#define CBM_ANA_JPSI_HIST_H

#include <vector>
#include <string>

/*
 * \brief Enumeration for different sources.
 */
enum CbmAnaJpsiSourceTypes {
  kJpsiSignal = 0,
  kJpsiBg = 1,
  kJpsiPi0 = 2,
  kJpsiGamma = 3
};

/*
 * \brief Enumeration for analysis steps.
 */
enum CbmAnaJpsiAnalysisSteps {
  kJpsiMc = 0,
  kJpsiAcc = 1,
  kJpsiReco = 2,
  kJpsiChi2Prim = 3,
  kJpsiElId = 4,
  kJpsiPtCut = 5
};

class CbmAnaJpsiHist
{
public:

   const static int fNofSourceTypes = 4;
   const static std::vector<std::string> fSourceTypes;
   const static std::vector<std::string> fSourceTypesLatex;
   const static std::vector<int> fSourceTypesColor;

   const static int fNofAnaSteps = 6;
   const static std::vector<std::string> fAnaSteps;
   const static std::vector<std::string> fAnaStepsLatex;
   const static std::vector<int> fAnaStepsColor;

};

#endif

