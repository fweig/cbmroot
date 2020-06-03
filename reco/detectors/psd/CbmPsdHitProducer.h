// -------------------------------------------------------------------------
// -----                 CbmPsdHitProducerIdel header file             -----
// -----                  Created 15/05/12  by Alla                   -----
// -----------------------------------------------------------
//--------------


/** CbmPsdIdealDigitize.h
 *@author Alla Maevskaya <alla@inr.ru>
 **
 ** The ideal hit producer produces hits of type CbmPsdHit as 
 ** full energy deposited in 1 module
 **/

#ifndef CBMPSDHitProducer_H
#define CBMPSDHitProducer_H 


#include "FairTask.h"
#include "TH1F.h"
#include "CbmDigiManager.h"

class TClonesArray;
const Int_t NPsdMod = 44; //with 4 central mods


class CbmPsdHitProducer : public FairTask
{

 public:

  /** Default constructor **/  
  CbmPsdHitProducer();


  /** Destructor **/
  ~CbmPsdHitProducer();


  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);
  virtual void Finish();


 private:
  Int_t             fNHits;
   
  /** Output array of CbmPsdHit **/
  TClonesArray* fHitArray;

  /** Digi Manager for input **/
  CbmDigiManager* fDigiMan;  //!

  CbmPsdHitProducer(const CbmPsdHitProducer&);
  CbmPsdHitProducer operator=(const CbmPsdHitProducer&);

  void Reset();

  Float_t       fXi[NPsdMod];  //X coordinate of center of module
  Float_t       fYi[NPsdMod];  //X coordinate of center of module

  TH1F *fhModXNewEn; //edep in each module for Marina
 

  ClassDef(CbmPsdHitProducer,2);

};

#endif
