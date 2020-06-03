#ifndef CBMTOFSTAR2018CONTFACT_H
#define CBMTOFSTAR2018CONTFACT_H

#include "FairContFact.h"

class FairContainer;
class FairParSet;

class CbmTofStar2018ContFact : public FairContFact {
private:
  void setAllContainers();
  CbmTofStar2018ContFact(const CbmTofStar2018ContFact&);
  CbmTofStar2018ContFact& operator=(const CbmTofStar2018ContFact&);
public:
  CbmTofStar2018ContFact();
  ~CbmTofStar2018ContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmTofStar2018ContFact,0) // Factory for all TRD parameter containers
};

#endif  /* !CBMTOFSTAR2018CONTFACT_H */
