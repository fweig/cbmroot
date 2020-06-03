// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmFlibTestSource                       -----
// -----                    Created 20.06.2016 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMFLIBTESTSOURCE_H
#define CBMFLIBTESTSOURCE_H

#include "TimesliceSource.hpp"
#include "Timeslice.hpp"
#include "MicrosliceDescriptor.hpp"

#include "FairSource.h"
#include "CbmTSUnpack.h"
#include "CbmTbDaqBuffer.h"

#include "TString.h"
#include "TClonesArray.h"
#include "TList.h"
#include "TObjString.h"
#include "TStopwatch.h"
#include "Rtypes.h"

#include <memory>
#include <map>


class CbmFlibTestSource : public FairSource
{
  public:
    CbmFlibTestSource();
    CbmFlibTestSource(const CbmFlibTestSource& source);
    virtual ~CbmFlibTestSource();

    Bool_t Init();
    Int_t ReadEvent(UInt_t);

    Source_Type GetSourceType() {return kONLINE;}

    virtual void SetParUnpackers();

    virtual Bool_t InitUnpackers();

    virtual Bool_t ReInitUnpackers();

    void Close();
    void Reset();

    void SetFileName(TString name) { fFileName = name; fInputFileList.Add(new TObjString(name));}
    void SetHostName(TString name) { fHost = name; }
    void SetPortNumber(Int_t port) { fPort = port; }
    void SetReqDigiAddr(Int_t ival)  {fiReqDigiAddr.resize(1); fiReqDigiAddr[0] = ival;};
    void AddReqDigiAddr(Int_t ival);
    void SetReqMode(Int_t ival)      { fiReqMode = ival; }
    void SetMaxDeltaT(Double_t dval) { fdMaxDeltaT = dval;}

    void AddUnpacker(CbmTSUnpack* unpacker, Int_t flibId, Int_t detId)
    {
      fUnpackers.insert ( std::pair<Int_t,CbmTSUnpack*>(flibId,unpacker) );
      fDetectorSystemMap.insert ( std::pair<Int_t,Int_t>(detId,flibId) );
    }

    void AddFile(const char * name) {
      fInputFileList.Add(new TObjString(name));
    }

    void AddPath(const TString& tFileDirectory,
                 const TString& tFileNameWildCard);

    UInt_t GetTsCount() { return fTSCounter; }

  private:

    TString fFileName;
    TList   fInputFileList;    ///< List of input files
    Int_t   fFileCounter;
    TString fHost;
    Int_t   fPort;

    std::map<Int_t, CbmTSUnpack*> fUnpackers;
    std::map<Int_t, Int_t> fDetectorSystemMap; //! Map detector system id to flib system id

    CbmTbDaqBuffer* fBuffer;

    UInt_t fTSNumber;
    UInt_t fTSCounter;
    std::vector< Int_t >  fiReqDigiAddr;
    Int_t fiReqMode;
    Double_t fdMaxDeltaT;
    TStopwatch fTimer;

    Bool_t fBufferFillNeeded; /** True if the input buffer has to be filled again **/

    fles::TimesliceSource* fSource; //!
    Bool_t CheckTimeslice(const fles::Timeslice& ts);
    void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);

    Int_t FillBuffer();
    Int_t GetNextEvent();

    CbmFlibTestSource operator=(const CbmFlibTestSource&);

    ClassDef(CbmFlibTestSource, 1)
};


#endif
