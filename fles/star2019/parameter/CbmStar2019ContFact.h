#ifndef CBMSTAR2019CONTFACT_H
#define CBMSTAR2019CONTFACT_H

#include "FairContFact.h"

class FairContainer;
class FairParSet;

class CbmStar2019ContFact : public FairContFact {
private:
  void setAllContainers();
  CbmStar2019ContFact(const CbmStar2019ContFact&);
  CbmStar2019ContFact& operator=(const CbmStar2019ContFact&);

public:
  CbmStar2019ContFact();
  ~CbmStar2019ContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmStar2019ContFact, 0)  // Factory for all TRD parameter containers
};

#endif /* !CBMSTAR2019CONTFACT_H */
