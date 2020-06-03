#ifndef CBMFLESTESTCONTFACT_H
#define CBMFLESTESTCONTFACT_H

#include "FairContFact.h"

class FairContainer;
class FairParSet;

class CbmFlesTestContFact : public FairContFact {
private:
  void setAllContainers();
  CbmFlesTestContFact(const CbmFlesTestContFact&);
  CbmFlesTestContFact& operator=(const CbmFlesTestContFact&);
public:
  CbmFlesTestContFact();
  ~CbmFlesTestContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmFlesTestContFact,0) // Factory for all TRD parameter containers
};

#endif  /* !CBMFLESTESTCONTFACT_H */
