// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmFlibFileSource                       -----
// -----                    Created 08.08.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMFLIBFILESOURCE_H
#define CBMFLIBFILESOURCE_H

#include "TimesliceSource.hpp"
#include "Timeslice.hpp"
#include "MicrosliceDescriptor.hpp"
#include "Message.hpp"

#include "FairSource.h"

#include "TString.h"
#include "TClonesArray.h"

#include <memory>


class CbmFlibFileSource : public FairSource
{
  public:
    CbmFlibFileSource();
    virtual ~CbmFlibFileSource();

    Bool_t Init();
    Int_t ReadEvent(UInt_t);

    Source_Type GetSourceType() {return kONLINE;}

    virtual void SetParUnpackers() {;}

    virtual Bool_t InitUnpackers() {return kTRUE;}

    virtual Bool_t ReInitUnpackers() {return kTRUE;}

    void Close();
    void Reset();

    void SetFileName(TString name) { fFileName = name; }
    void SetHostName(TString name) { fHost = name; }
    void SetPortNumber(Int_t port) { fPort = port; }

  private:
  
    TString fFileName;
    TString fHost;
    Int_t   fPort;
    
    // --- Output arrays
    TClonesArray* fSpadicRaw;         ///< Output array of CbmSpadicRawMessage

    Int_t fTSCounter;

    fles::TimesliceSource* fSource; //!
    Bool_t CheckTimeslice(const fles::Timeslice& ts);
    void UnpackSpadicCbmNetMessage(const fles::Timeslice& ts, size_t component);
    void print_message(const spadic::Message& m);
    void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);

    CbmFlibFileSource operator=(const CbmFlibFileSource&);
    CbmFlibFileSource(const CbmFlibFileSource& source);

    ClassDef(CbmFlibFileSource, 1)
};


#endif
