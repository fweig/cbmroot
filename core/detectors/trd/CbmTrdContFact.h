#ifndef CBMTRDCONTFACT_H
#define CBMTRDCONTFACT_H

#include <Rtypes.h>        // for ClassDef

#include "FairContFact.h"  // for FairContFact

class FairParSet;
class FairContainer;

class CbmTrdContFact : public FairContFact {
private:
  void setAllContainers();
  CbmTrdContFact(const CbmTrdContFact&);
  CbmTrdContFact& operator=(const CbmTrdContFact&);
public:
  CbmTrdContFact();
  ~CbmTrdContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmTrdContFact,0) // Factory for all TRD parameter containers
};

#endif  /* !CBMTRDCONTFACT_H */
