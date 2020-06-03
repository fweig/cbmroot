#ifndef CBMTOFCONTFACT_H
#define CBMTOFCONTFACT_H

#include <Rtypes.h>        // for THashConsistencyHolder, ClassDef

#include <FairContFact.h>  // for FairContFact

class FairParSet;
class FairContainer;

class CbmTofContFact : public FairContFact {
private:
  void setAllContainers();
  CbmTofContFact(const CbmTofContFact&);
  CbmTofContFact& operator=(const CbmTofContFact&);
public:
  CbmTofContFact();
  ~CbmTofContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef( CbmTofContFact,0) // Factory for all TOF parameter containers
};

#endif  /* !CBMTOFCONTFACT_H */
