#ifndef CBM_KRES_SELECT_GOOD_EVENTS
#define CBM_KRES_SELECT_GOOD_EVENTS


#include "FairTask.h"

#include <TClonesArray.h>
#include <vector>
#include "CbmMCTrack.h"

class FairMCApplication;

class CbmKresSelectGoodEvents : public FairTask
{

public:
  //***** brief Standard constructor.
  CbmKresSelectGoodEvents();
  //***** brief Standard destructor.
  virtual ~CbmKresSelectGoodEvents();


  virtual InitStatus Init();
  virtual void Finish();

  virtual void Exec(Option_t*);

private:
 
  TClonesArray* fMcTracks;
  FairMCApplication* fApp;
  std::vector<CbmMCTrack*> Electrons;

  //***** brief Copy constructor.
  CbmKresSelectGoodEvents(const CbmKresSelectGoodEvents&);

  //***** brief Assignment operator.
  CbmKresSelectGoodEvents operator=(const CbmKresSelectGoodEvents&);


  ClassDef(CbmKresSelectGoodEvents,1)
};

#endif

