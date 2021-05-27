#ifndef MCBM_RICH_TOTSHIFTER
#define MCBM_RICH_TOTSHIFTER

#include "FairTask.h"
class CbmMcbm2018RichPar;
class TClonesArray;
class CbmHistManager;
class TVector3;
class CbmDigiManager;

#include "TH1D.h"

#include <map>
#include <vector>

using namespace std;


class CbmRichMCbmToTShifter : public FairTask {

public:
  /**
     * \brief Standard constructor.
     */
  CbmRichMCbmToTShifter();

  /**
     * \brief Standard destructor.
     */
  virtual ~CbmRichMCbmToTShifter() {};

  /**
     * \brief Inherited from FairTask.
     */
  virtual InitStatus Init();

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Exec(Option_t* option);

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Finish();

  /**
     * \brief Set output directory where you want to write results (figures and json).
     * \param[in] dir Path to the output directory.
     */
  void SetOutputDir(const string& dir) { fOutputDir = dir; }


  /**
     * \brief Flag Funtion to control PDF output
     */
  void GeneratePDF(Bool_t b = true) { fGeneratePDFs = b; };

  /**
     * \brief Flag Funtion to control printout for ParameterFile
     */
  void ShowTdcId(Bool_t b = true) { fShowTdcId = b; };


private:
  CbmDigiManager* fDigiMan = nullptr;

  Int_t fEventNum;

  std::string fOutputDir;  // output dir for results

  std::map<Int_t, std::map<Int_t, TH1*>> fhTotMap;

  Bool_t fGeneratePDFs;

  Bool_t fShowTdcId;

  /**
     *  \brief Handler for TH1 Histograms
     */
  TH1* GetTotH1(Int_t tdc, Int_t channel);


  uint16_t calcDirichAddr(uint32_t cnt)
  {
    return (0x7 << 12) | ((cnt / 18) << 8) | (((cnt % 18) / 2) << 4) | ((cnt % 2) << 0);
  };

  /**
     * \brief Fill output lines with 0's if DiRICh Address is not in use in input file
     */
  std::string printEmpty();

  /**
     * \brief Initialize histograms.
     */
  void InitHistograms();


  /**
     *  \brief Extract the Tdc Address from the encoded DiRICH Address.
     */
  inline int getDirichAddress(const int dirich) { return (dirich >> 16) & 0xffff; }

  /**
     *  \brief Extract the channel Address from the encoded DiRICH Address.
     */
  inline int getDirichChannel(const int dirich) { return (dirich) &0xffff; }

  /**
     *  \brief Find the Maximum in a TH1 Histogram
     */
  Double_t GetMaxH1(TH1* h);

  /**
     * \brief Copy constructor.
     */
  CbmRichMCbmToTShifter(const CbmRichMCbmToTShifter&);

  /**
     * \brief Assignment operator.
     */
  CbmRichMCbmToTShifter& operator=(const CbmRichMCbmToTShifter&);


  ClassDef(CbmRichMCbmToTShifter, 1)
};

#endif
