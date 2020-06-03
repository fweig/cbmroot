// ------------------------------------------------------------------
// -----                     TMbsUnpackTofPar                   -----
// -----              Created 08/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#ifndef MBSUNPACKTOFCONTFACT_H
#define MBSUNPACKTOFCONTFACT_H

#include "FairContFact.h"

class FairContainer;

class TMbsUnpackTofContFact : public FairContFact {
private:
  void setAllContainers();
  TMbsUnpackTofContFact(const TMbsUnpackTofContFact&);
  TMbsUnpackTofContFact& operator=(const TMbsUnpackTofContFact&);
public:
  TMbsUnpackTofContFact();
  ~TMbsUnpackTofContFact() {}
  FairParSet* createContainer(FairContainer*);
  ClassDef( TMbsUnpackTofContFact,0) // Factory for all TOF unpack parameter containers
};

#endif  /* !MBSUNPACKTOFCONTFACT_H */
