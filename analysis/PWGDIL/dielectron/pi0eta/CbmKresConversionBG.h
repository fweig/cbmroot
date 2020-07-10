#ifndef CBM_KRES_CONVERSION_BG
#define CBM_KRES_CONVERSION_BG

#include "CbmMCTrack.h"
#include <TClonesArray.h>
#include <TH1.h>

using namespace std;

class CbmKresConversionBG {

public:
  //***** brief Standard constructor.
  CbmKresConversionBG();
  //***** brief Standard destructor.
  virtual ~CbmKresConversionBG();


  void Init();

  void Exec(CbmMCTrack* mctrack1,
            CbmMCTrack* mctrack2,
            CbmMCTrack* mctrack3,
            CbmMCTrack* mctrack4,
            Double_t invmassRecoPi0,
            vector<TH1*> BGCases);


private:
  TClonesArray* fMcTracks;

  //***** brief Copy constructor.
  CbmKresConversionBG(const CbmKresConversionBG&);

  //***** brief Assignment operator.
  CbmKresConversionBG operator=(const CbmKresConversionBG&);


  ClassDef(CbmKresConversionBG, 1)
};

#endif
