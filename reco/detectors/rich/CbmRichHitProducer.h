/* Copyright (C) 2004-2021 GSI/JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Boris Polichtchouk, Semen Lebedev, Andrey Lebedev [committer], Volker Friese */

/**
 * \file CbmRichHitProducer.h
 *
 * \brief Class for producing RICH hits directly from MCPoints.
 *
 * \author B. Polichtchouk
 * \date 2004
 **/

#ifndef CBM_RICH_HIT_PRODUCER
#define CBM_RICH_HIT_PRODUCER

#include "CbmRichRecGeoPar.h"

#include "FairTask.h"

#include <TClonesArray.h>  // for ROOTCLING

class TVector3;
class CbmEvent;
class CbmDigiManager;

/**
 * \class CbmRichHitProducer
 *
 * \brief Class for producing RICH hits directly from MCPoints.
 *
 * \author B. Polichtchouk
 * \date 2004
 **/
class CbmRichHitProducer : public FairTask {
public:
  /**
     * \brief Default constructor.
     */
  CbmRichHitProducer();

  /**
     * \brief Destructor.
     */
  virtual ~CbmRichHitProducer();

  /**
     * \brief Inherited from FairTask.
     */
  virtual void SetParContainers();

  /**
     * \brief Inherited from FairTask.
     */
  virtual InitStatus Init();

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Exec(Option_t* option);

  /**
     * \brief Inherited from FairTask.
     */
  virtual void Finish();

  /**
     * Processblock of data either event-by-event or CbmEvent
     */
  Int_t ProcessData(CbmEvent* event);

  /**
     * Process RichDigi. CbmEvent can be NULL.
     */
  void ProcessDigi(CbmEvent* event, Int_t digiIndex);

  void SetRotationNeeded(Bool_t b) { fRotationNeeded = b; }


private:
  CbmDigiManager* fDigiMan = nullptr;  //!
  TClonesArray* fRichHits  = nullptr;  // RICH hits
  TClonesArray* fCbmEvents = nullptr;  // CbmEvent for time-based simulations

  Int_t fNofTs           = 0;   // number of timeslices
  Int_t fNofEvents       = 0;   // number of events
  Long64_t fNofDigisAll  = 0;   // all digis in input
  Long64_t fNofDigisUsed = 0;   // digis used for hit finding
  Double_t fTime         = 0.;  // processing time

  Bool_t fRotationNeeded = kTRUE;

  Double_t fHitError = 0.6 / sqrt(12);

  /**
     * \brief Add hit to the output array (and) CbmEvent if it is not NULL.
     */

  void AddHit(CbmEvent* event, TVector3& posHit, Double_t time, Int_t index);

  /**
     * \brief Copy constructor.
     */
  CbmRichHitProducer(const CbmRichHitProducer&);

  /**
     * \brief Assignment operator.
     */
  CbmRichHitProducer& operator=(const CbmRichHitProducer&);

  ClassDef(CbmRichHitProducer, 1)
};

#endif
