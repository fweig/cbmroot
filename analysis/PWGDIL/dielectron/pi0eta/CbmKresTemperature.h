#ifndef CBM_KRES_TEMPERATURE
#define CBM_KRES_TEMPERATURE

#include <TClonesArray.h>
#include <vector>

class TH1;
class TH1D;


using namespace std;

class CbmKresTemperature {

public:
  //***** brief Standard constructor.
  CbmKresTemperature();
  //***** brief Standard destructor.
  virtual ~CbmKresTemperature();


  void Init();
  void InitHistograms();
  void Finish();

  void Exec(int fEventNumTempr);


private:
  TClonesArray* fMcTracks;
  TClonesArray* fGlobalTracks;
  TClonesArray* fStsTracks;


  // Histograms
  vector<TH1*> fHistoList_MC;
  TH1D* MC_pi_minus_Tempr;
  TH1D* MC_pi_plus_Tempr;
  TH1D* MC_pi_zero_Tempr;
  TH1D* MC_proton_Tempr;
  TH1D* MC_kaon_zero_Tempr;
  TH1D* MC_kaon_plus_Tempr;
  TH1D* MC_kaon_minus_Tempr;
  TH1D* MC_direct_photons_Tempr;


  //***** brief Copy constructor.
  CbmKresTemperature(const CbmKresTemperature&);

  //***** brief Assignment operator.
  CbmKresTemperature operator=(const CbmKresTemperature&);


  ClassDef(CbmKresTemperature, 1)
};

#endif
