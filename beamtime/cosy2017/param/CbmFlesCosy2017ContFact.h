#ifndef CBMFLESCOSY2017CONTFACT_H
#define CBMFLESCOSY2017CONTFACT_H

#include "FairContFact.h"

class FairContainer;
class FairParSet;

class CbmFlesCosy2017ContFact : public FairContFact {
private:
  void setAllContainers();
  CbmFlesCosy2017ContFact(const CbmFlesCosy2017ContFact&);
  CbmFlesCosy2017ContFact& operator=(const CbmFlesCosy2017ContFact&);
public:
  CbmFlesCosy2017ContFact();
  ~CbmFlesCosy2017ContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmFlesCosy2017ContFact,0) // Factory for all COSY 2017 parameter containers
};

#endif  /* !CBMFLESCOSY2017CONTFACT_H */
