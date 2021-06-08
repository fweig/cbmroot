/* Copyright (C) 2012-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alla Maevskaya, Volker Friese [committer] */

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


#include "CbmDigiManager.h"

#include <FairTask.h>

#include <TH1F.h>

class TClonesArray;
class CbmEvent;
const Int_t NPsdMod = 44;  //with 4 central mods


class CbmPsdHitProducer : public FairTask {

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
  Int_t fNHits = 0;

  /** Output array of CbmPsdHit **/
  TClonesArray* fHitArray = nullptr;

  /** Digi Manager for input **/
  CbmDigiManager* fDigiMan = nullptr;  //!

  /** Event array **/
  TClonesArray* fEvents = nullptr;  //!


  /** @brief Processing of digis **/
  std::pair<Int_t, Int_t> ProcessData(CbmEvent* event);

  /** Counters **/
  ULong64_t fNofTs     = 0;
  ULong64_t fNofEvents = 0;
  ULong64_t fNofDigis  = 0;
  ULong64_t fNofHits   = 0;
  Double_t fTimeTot    = 0.;


  CbmPsdHitProducer(const CbmPsdHitProducer&);
  CbmPsdHitProducer operator=(const CbmPsdHitProducer&);

  void Reset();

  Float_t fXi[NPsdMod];  //X coordinate of center of module
  Float_t fYi[NPsdMod];  //X coordinate of center of module

  TH1F* fhModXNewEn = nullptr;  //edep in each module for Marina


  ClassDef(CbmPsdHitProducer, 2);
};

#endif
