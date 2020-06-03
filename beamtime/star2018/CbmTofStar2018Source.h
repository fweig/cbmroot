// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmTofStar2018Source                     -----
// -----                    Created 19.01.2018 by P.-A. Loizeau            -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTOFSTAR2018SOURCE_H
#define CBMTOFSTAR2018SOURCE_H

#include "TimesliceSource.hpp"
#include "Timeslice.hpp"
#include "MicrosliceDescriptor.hpp"

#include "FairSource.h"
#include "CbmTSUnpack.h"
//#include "CbmTbDaqBuffer.h"

#include "TString.h"
#include "TClonesArray.h"
#include "TList.h"
#include "TObjString.h"
#include "TStopwatch.h"

#include <memory>
#include <map>

class TH1I;
class TProfile;

class CbmTofStar2018Source : public FairSource
{
  public:
    CbmTofStar2018Source();
    CbmTofStar2018Source(const CbmTofStar2018Source& source);
    virtual ~CbmTofStar2018Source();

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

    void AddUnpacker(CbmTSUnpack* unpacker, Int_t flibId, Int_t detId)
    {
      fUnpackers.insert ( std::pair<Int_t,CbmTSUnpack*>(flibId,unpacker) );
      fDetectorSystemMap.insert ( std::pair<Int_t,Int_t>(detId,flibId) );
    }

    void AddFile(const char * name) {
      fInputFileList.Add(new TObjString(name));
    }

    UInt_t GetTsCount() { return fTSCounter; }
    UInt_t GetNofTSSinceLastTS() { return fNofTSSinceLastTS; }

    void SetDataReductionMuch( UInt_t uTsReduction = 15 ) { fuTsReduction = uTsReduction; }

  private:

    TString fFileName;
    TList   fInputFileList;    ///< List of input files
    Int_t   fFileCounter;
    TString fHost;
    Int_t   fPort;

    std::map<Int_t, CbmTSUnpack*> fUnpackers;
    std::map<Int_t, Int_t> fDetectorSystemMap; //! Map detector system id to flib system id

//    CbmTbDaqBuffer* fBuffer;

    UInt_t fTSNumber;
    UInt_t fTSCounter;
    TStopwatch fTimer;

    Bool_t fBufferFillNeeded; /** True if the input buffer has to be filled again **/

    TH1I*     fHistoMissedTS;
    TProfile* fHistoMissedTSEvo;
    Int_t fNofTSSinceLastTS;

    UInt_t fuTsReduction;

    fles::TimesliceSource* fSource; //!
    Bool_t CheckTimeslice(const fles::Timeslice& ts);
    void PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc);

    Int_t FillBuffer();
    Int_t GetNextEvent();
    Bool_t OpenNextFile();

    CbmTofStar2018Source operator=(const CbmTofStar2018Source&);

    ClassDef(CbmTofStar2018Source, 1)
};


#endif
