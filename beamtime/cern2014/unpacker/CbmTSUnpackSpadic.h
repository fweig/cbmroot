// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTSUnpackSpadic                       -----
// -----                    Created 07.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACKSPADIC_H
#define CBMTSUNPACKSPADIC_H

#include "Timeslice.hpp"
#include "Message.hpp"


#include "CbmTSUnpack.h"
#include "CbmBeamDefaults.h"

#include "TClonesArray.h"

class CbmTSUnpackSpadic : public CbmTSUnpack
{
 public:
  
  CbmTSUnpackSpadic();
  virtual ~CbmTSUnpackSpadic();
    
  virtual Bool_t Init();

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  void print_message(const spadic::Message& m);

  virtual void Reset();

  virtual void Finish();

  void SetParContainers() {;}

  // protected:
  //  virtual void Register();

  virtual void FillOutput(boost::any){;}
 private:

  TClonesArray* fSpadicRaw;

  Int_t fEpochMarkerArray[NrOfSyscores][NrOfHalfSpadics];
//Array to store the previous Epoch counter
  Int_t fPreviousEpochMarkerArray[NrOfSyscores][NrOfHalfSpadics];
//Suppress multiple Epoch Messages in duplicated Microslices. NOTE:Currently Buggy
  const Bool_t SuppressMultipliedEpochMessages = false;
  Int_t fSuperEpochArray[NrOfSyscores][NrOfHalfSpadics];

  Int_t fEpochMarker;
  Int_t fSuperEpoch;
  Int_t fNrExtraneousSamples;
  void GetEpochInfo(Int_t link, Int_t addr);
  void FillEpochInfo(Int_t link, Int_t addr, Int_t epoch_count);

  CbmTSUnpackSpadic(const CbmTSUnpackSpadic&);
  CbmTSUnpackSpadic operator=(const CbmTSUnpackSpadic&);

  ClassDef(CbmTSUnpackSpadic, 2)
};

#endif

