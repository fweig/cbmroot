#ifndef CBMCOSY2019CONTFACT_H
#define CBMCOSY2019CONTFACT_H

#include "FairContFact.h"

class FairContainer;
class FairParSet;

class CbmCosy2019ContFact : public FairContFact {
private:
  void setAllContainers();
  CbmCosy2019ContFact(const CbmCosy2019ContFact&);
  CbmCosy2019ContFact& operator=(const CbmCosy2019ContFact&);

public:
  CbmCosy2019ContFact();
  ~CbmCosy2019ContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmCosy2019ContFact, 0)  // Factory for all mcbm parameter containers
};

#endif /* !CBMCOSY2019CONTFACT_H */
