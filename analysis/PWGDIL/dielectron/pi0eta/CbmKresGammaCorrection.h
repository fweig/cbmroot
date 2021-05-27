#ifndef CBM_KRES_GAMMA_CORRECTION
#define CBM_KRES_GAMMA_CORRECTION

#include "TH2D.h"
#include <TClonesArray.h>

using namespace std;

class CbmKresGammaCorrection {

public:
  //***** brief Standard constructor.
  CbmKresGammaCorrection();
  //***** brief Standard destructor.
  virtual ~CbmKresGammaCorrection();


  void Init(std::vector<std::vector<double>>& vect_all, std::vector<std::vector<double>>& vect_two,
            std::vector<std::vector<double>>& vect_onetwo, double OA, double IM);
  void Finish();
  void InitHistograms();


private:
  vector<TH1*> fHistoList_factors;
  TH2D* Correction_factros_all;
  TH2D* Correction_factros_two;
  TH2D* Correction_factros_onetwo;


  //***** brief Copy constructor.
  CbmKresGammaCorrection(const CbmKresGammaCorrection&);

  //***** brief Assignment operator.
  CbmKresGammaCorrection operator=(const CbmKresGammaCorrection&);


  ClassDef(CbmKresGammaCorrection, 1)
};

#endif
