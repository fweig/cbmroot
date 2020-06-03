#ifndef CBMFASTDECAYER_H
#define CBMFASTDECAYER_H


#include "TVirtualMCDecayer.h"
#include "FairGenerator.h"

#include <vector>       // std::vector

class FairPrimaryGenerator;
class TParticle;
class CbmStack;

class CbmFastDecayer : public FairGenerator {

  public:

  /** Default constructor without arguments should not be used. **/
  CbmFastDecayer();

  /** Standard constructor.
   * @param fileName The input file name
   **/
  CbmFastDecayer(const char* fileName, TString particle);

  /** Destructor. **/
  ~CbmFastDecayer();

  /** Reads on event from the input file and pushes the tracks onto
   ** the stack. Abstract method in base class.
   ** @param pStack    pointer to the stack
   ** @param ver       not used
   **/
  Bool_t ReadEvent(FairPrimaryGenerator* primGen);

  virtual Bool_t Init();
  TVirtualMCDecayer* GetDecayer()                      { return fDecayer; }
  void  SetExternalDecayer(TVirtualMCDecayer *decayer) { fDecayer=decayer;}

  void SetParticlesForDecay(char const *pdgs="");
  void SetParticlesForGeant(char const *pdgs="");

 protected:

  private:
  CbmStack    *fStack   = NULL;	              //!pointer to CbmStack
  TVirtualMCDecayer  *fDecayer = NULL;        //!pointer to decayer

  std::vector<int> fDecayPdgCodes;
  std::vector<int> fGeantPdgCodes;

  CbmFastDecayer(const CbmFastDecayer &EvtGen);
  CbmFastDecayer & operator=(const CbmFastDecayer & rhs);

  ClassDef(CbmFastDecayer,0) //CbmGenerator interface to EvtGen
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif
