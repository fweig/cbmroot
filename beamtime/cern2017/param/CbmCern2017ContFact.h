#ifndef CBMCERN2017CONTFACT_H
#define CBMCERN2017CONTFACT_H

#include "FairContFact.h"

class FairContainer;
class FairParSet;

class CbmCern2017ContFact : public FairContFact {
private:
  void setAllContainers();
  CbmCern2017ContFact(const CbmCern2017ContFact&);
  CbmCern2017ContFact& operator=(const CbmCern2017ContFact&);
public:
  CbmCern2017ContFact();
  ~CbmCern2017ContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef(CbmCern2017ContFact,0) // Factory for all TRD parameter containers
};

#endif  /* !CBMCERN2017CONTFACT_H */
