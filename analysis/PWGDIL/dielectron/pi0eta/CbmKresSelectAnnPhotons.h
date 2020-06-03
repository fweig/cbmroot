#ifndef CBM_KRES_SELECT_ANN_PHOTONS
#define CBM_KRES_SELECT_ANN_PHOTONS

#include <TClonesArray.h>
#include "TVector3.h"
#include "TMultiLayerPerceptron.h"

using namespace std;

class CbmKresSelectAnnPhotons
{

public:
  //***** brief Standard constructor.
  CbmKresSelectAnnPhotons();
  //***** brief Standard destructor.
  virtual ~CbmKresSelectAnnPhotons();


  void Init();
  double DoSelect(double InvariantMass, double OpeningAngle, double PlaneAngle_last, double ZPos, TVector3 Momentum1, TVector3 Momentum2);


private:
 
  std::string fAnnWeights;
  TMultiLayerPerceptron* fNN;

  //***** brief Copy constructor.
  CbmKresSelectAnnPhotons(const CbmKresSelectAnnPhotons&);

  //***** brief Assignment operator.
  CbmKresSelectAnnPhotons operator=(const CbmKresSelectAnnPhotons&);


	ClassDef(CbmKresSelectAnnPhotons,1)
};

#endif

