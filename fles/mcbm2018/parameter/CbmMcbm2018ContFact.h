#ifndef CBMMCBM2018CONTFACT_H
#define CBMMCBM2018CONTFACT_H

#include "FairContFact.h"

class FairContainer;
class FairParSet;

class CbmMcbm2018ContFact : public FairContFact {
private:
  void setAllContainers();
  CbmMcbm2018ContFact(const CbmMcbm2018ContFact&);
  CbmMcbm2018ContFact& operator=(const CbmMcbm2018ContFact&);

public:
  CbmMcbm2018ContFact();
  ~CbmMcbm2018ContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmMcbm2018ContFact, 0)  // Factory for all TRD parameter containers
};

#endif /* !CBMMCBM2018CONTFACT_H */
