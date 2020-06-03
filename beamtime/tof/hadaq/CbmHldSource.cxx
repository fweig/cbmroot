/** @file CbmHldSource.cxx
 ** @author Christian Simon <csimon@physi.uni-heidelberg.de>
 ** @date 2015-11-25
 **/

#include "CbmHldSource.h"

#include "HadaqDefines.h"

#include "FairLogger.h"

#include "TSystem.h"
#include "TList.h"
#include "TObjString.h"
#include "TRegexp.h"
#include "TSystemDirectory.h"

// =====   default constructor   ===============================================
CbmHldSource::CbmHldSource()
  : FairMbsSource(),
    fFileNames(new TList()),
    fNFiles(0),
    fCurrentFile(0),
    fCurrentFileName(""),
  	fNEvents(0),
  	fCurrentEvent(0),
    fFileStream(new std::ifstream()),
    fDataBuffer(new Char_t[hadaq::HADTU_PAYLOAD_MAX]),
    fNoMoreEvents(kFALSE)
{
}
// =============================================================================

// =============================================================================
CbmHldSource::~CbmHldSource()
{
  fFileNames->Delete(); //TODO: does it delete the TObjString instances?
  delete fFileNames;

  delete fFileStream;
 
  delete[] fDataBuffer;
}
// =============================================================================

// =============================================================================
Bool_t CbmHldSource::Init()
{
  fNFiles = fFileNames->GetEntries();

  if(!fNFiles)
  {
    LOG(error) << 
                   TString::Format("\nNo input data file given to the data "
                                   "source. Please add at least one input data "
                                   "file by calling CbmHldSource::AddFile "
                                   "prior to calling FairRunOnline::Init."
                                  ).Data();

    return kFALSE;
  }

  for(Int_t i = 0; i < fNFiles; i++)
  {
    TString tFileName = ((TObjString*)fFileNames->At(i))->GetString();

    if(!tFileName.EndsWith(".hld"))
    {
      LOG(error) <<
                     TString::Format("\nFile extension of input data file %s "
                                     "is not compatible with CbmHldSource.",
                                     tFileName.Data()
                                    ).Data();

      return kFALSE;
    }
  }

  if(!OpenNextFile())
  {
    return kFALSE;
  }

  fNEvents = 0;

  return kTRUE;
}
// =============================================================================

// =============================================================================
Int_t CbmHldSource::ReadEvent(UInt_t)
{
  // return values: 0 -- all fine
  //                1 -- source could not read event from file (stop event loop)
  //                2 -- unpacker could not handle event (skip it)

  if(fNoMoreEvents)
  {
    LOG(info) << 
                  TString::Format("\nNo more events to be offered by this "
                                  "FairSource instance.\nIn total, %10d events "
                                  "were successfully unpacked for analysis.",
                                  fNEvents
                                 ).Data();

    return 1;

  }

  Int_t iEvHeaderSize = sizeof(hadaq::RawEvent)*sizeof(Char_t);
  std::streampos headerPos = fFileStream->tellg();
  fFileStream->read(fDataBuffer, iEvHeaderSize);

  hadaq::RawEvent* tRawEventHeader = (hadaq::RawEvent*)fDataBuffer;

  switch(tRawEventHeader->GetId()) 
  {
    case hadaq::EvtId_runStop:
    {
      if(static_cast<UInt_t>(iEvHeaderSize)
         != tRawEventHeader->GetPaddedSize()
        )
      {

        LOG(warn) <<
                         TString::Format("\nInput data file %s does not "
                                         "contain a valid stop event. Possible "
                                         "data corruption. ",
                                         fCurrentFileName.Data()
                                        ).Data();
      }

      LOG(info) << 
                    TString::Format("\nFound a valid stop event in input data "
                                    "file %s.\nIn total, %10d events were "
                                    "extracted.\nThe file will be closed.",
                                    fCurrentFileName.Data(),
                                    fCurrentEvent
                                   ).Data();

      // reading the last (unknown) character and a hypothetical additional one
      // from file to force the EOF bit of ifstream to be set
      fFileStream->read(fDataBuffer,2);

      if(!fFileStream->eof())
      {
        LOG(warn) <<
                         TString::Format("\nEnd of input data file %s has not "
                                         "been reached although a valid stop "
                                         "event was found. The additional file "
                                         "content will be discarded.",
                                         fCurrentFileName.Data()
                                        ).Data();
      }

      CloseFile();

      if(fCurrentFile != fNFiles)
      {
        if(OpenNextFile())
        {
          return ReadEvent();
        }
        else
        {
          LOG(error) <<
                         TString::Format("\nAborting event loop as the next "
                                         "input file could not be opened."
                                        ).Data();         
        }
      }
      else
      {
        LOG(info) << 
                      TString::Format("\nAll events in all input data files "
                                      "added to the source have been read.\n"
                                      "In total, %10d events were "
                                      "extracted.",
                                      fNEvents
                                     ).Data();
      }

      fNoMoreEvents = kTRUE;
      return 1;

      break;
    }

    case hadaq::EvtId_DABC_TrbNet_Type_0:
    case hadaq::EvtId_DABC_TrbNet_Type_1:
    case hadaq::EvtId_DABC_TrbNet_Type_2:
    case hadaq::EvtId_DABC_TrbNet_Type_3:
    case hadaq::EvtId_DABC_TrbNet_Type_4:
    case hadaq::EvtId_DABC_TrbNet_Type_5:
    case hadaq::EvtId_DABC_TrbNet_Type_6:
    case hadaq::EvtId_DABC_TrbNet_Type_7:
    {
      fFileStream->seekg(headerPos);
      Int_t* iEvtStart = (Int_t*)fDataBuffer;
      Int_t iEvtSize = tRawEventHeader->GetPaddedSize();

      fFileStream->read(fDataBuffer, iEvtSize);

      if(fFileStream->eof())
      {
        LOG(error) << 
                       TString::Format("\nEnd of input data file %s reached "
                                       "after reading regular event %u. We "
                                       "close this file and try to open the "
                                       "next input data file if there is any.",
                                       fCurrentFileName.Data(),
                                       fCurrentEvent
                                      ).Data();
        CloseFile();

        if(fCurrentFile != fNFiles)
        {
          if(OpenNextFile())
          {
            return ReadEvent();
          }
          else
          {
            LOG(error) <<
                           TString::Format("\nAborting event loop as the next "
                                           "input file could not be opened."
                                          ).Data();
          }
        }
        else
        {
          LOG(info) <<
                        TString::Format("\nAll events in all input data files "
                                        "added to the source have been read.\n"
                                        "In total, %10d events were "
                                        "extracted.",
                                        fNEvents
                                       ).Data();
        }

        fNoMoreEvents = kTRUE;
        return 1;
      }

      LOG(debug1) <<
                      TString::Format("\nSuccessfully read event %u from input "
                                      "data file %s. The raw data is forwarded "
                                      "to the FairUnpack instances registered "
                                      "with the source.",
                                      fCurrentEvent,
                                      fCurrentFileName.Data()
                                     ).Data();

      for (Int_t i = 0; i < fUnpackers->GetEntriesFast(); i++) 
      {
        FairUnpack* tUnpacker = (FairUnpack*)fUnpackers->At(i);

        // giving 4-byte words as second function argument for compatibility
        // with FairRoot MBS sources
        if(!tUnpacker->DoUnpack(iEvtStart, iEvtSize/4))
        {
          LOG(info) <<
                        TString::Format("\nFairUnpack instance indexed %u "
                                        "of type %s did not unpack HADAQ raw "
                                        "event %u (see log message above). "
                                        "Fully ignore this event.",
                                        i,
                                        tUnpacker->ClassName(),
                                        tRawEventHeader->GetSeqNr()
                                       ).Data();
          return 2;
        }
      }

      fNEvents++;
      fCurrentEvent++;
      return 0;

      break;
    }

    case hadaq::EvtId_DABC_TrbNet_Type_8:
    case hadaq::EvtId_DABC_TrbNet_Type_9:
    case hadaq::EvtId_DABC_TrbNet_Type_A:
    case hadaq::EvtId_DABC_TrbNet_Type_B:
    case hadaq::EvtId_DABC_TrbNet_Type_C:
    case hadaq::EvtId_DABC_TrbNet_Type_D:
    case hadaq::EvtId_DABC_TrbNet_Type_E:
    case hadaq::EvtId_DABC_TrbNet_Type_F:
    {
      LOG(warn) <<
                       TString::Format("\nThe HadTuId 0x%.8x indicates event "
                                       "%u in input data file %s being "
                                       "triggered by a calibration trigger of "
                                       "type 0x%.1x. Skip unpacking and "
                                       "FairTask execution for this event. If "
                                       "the problem persists, check the TrbNet "
                                       "trigger map in the CTS monitor!",
                                       tRawEventHeader->GetId(),
                                       fCurrentEvent,
                                       fCurrentFileName.Data(),
                                       tRawEventHeader->GetTrigType()
                                      ).Data();

      headerPos += static_cast<Int_t>(tRawEventHeader->GetPaddedSize());
      fFileStream->seekg(headerPos);

      fCurrentEvent++;

      // The framework will call FairSource::ReadEvent repeatedly until it
      // receives return value 0 (event unpacked) or 1 (abort event loop).
      return 2;

      break;
    }

    default:
      LOG(error) <<
                     TString::Format("\nUnknown HadTuId 0x%.8x found in event "
                                     "%u in input data file %s. We stop "
                                     "reading events from this file and try to "
                                     "open the next input data file if there "
                                     "is any.",
                                     tRawEventHeader->GetId(),
                                     fCurrentEvent,
                                     fCurrentFileName.Data()
                                    ).Data();

      CloseFile();

      if(fCurrentFile != fNFiles)
      {
        if(OpenNextFile())
        {
          return ReadEvent();
        }
        else
        {
          LOG(error) <<
                         TString::Format("\nAborting event loop as the next "
                                         "input file could not be opened."
                                        ).Data();
        }
      }
      else
      {
        LOG(info) <<
                      TString::Format("\nAll events in all input data files "
                                      "added to the source have been read.\n"
                                      "In total, %10d events were "
                                      "extracted.",
                                      fNEvents
                                     ).Data();
      }

      fNoMoreEvents = kTRUE;
      return 1;

      break;
  }

}
// =============================================================================

// =============================================================================
void CbmHldSource::Close()
{
}
// =============================================================================

// =============================================================================
Bool_t CbmHldSource::OpenNextFile()
{
  fCurrentFileName = ((TObjString*)fFileNames->At(fCurrentFile))->GetString();

  fFileStream->open(fCurrentFileName, std::ifstream::binary | 
                                      std::ifstream::ate
                   );

  if(!fFileStream->is_open())
  {
    LOG(error) <<
                   TString::Format("\nCould not open input data file %s.",
                                   fCurrentFileName.Data()
                                  ).Data();

    return kFALSE;
  }

  Int_t tFileSize = fFileStream->tellg();
  fFileStream->seekg(0, std::ios::beg);

  LOG(info) <<
                TString::Format("\nSuccessfully opened input data file %s.\n"
                                "Size: %4d MiB.",
                                fCurrentFileName.Data(),
                                tFileSize/1048576
                               ).Data();

  //TODO: HADES file name decoding (optional)

  if(fFileStream->eof())
  {
    LOG(error) <<
                   TString::Format("\nInput data file %s seems to be empty.",
                                   fCurrentFileName.Data()
                                  ).Data();

    CloseFile();

    return kFALSE;
  }

  Int_t iEvHeaderSize = sizeof(hadaq::RawEvent)*sizeof(Char_t);

  fFileStream->read(fDataBuffer, iEvHeaderSize);

  hadaq::RawEvent* tRawEventHeader = (hadaq::RawEvent*)fDataBuffer;

  if((static_cast<UInt_t>(iEvHeaderSize) != tRawEventHeader->GetPaddedSize())
     || (hadaq::EvtId_runStart != tRawEventHeader->GetId())
    )
  {
    LOG(error) <<
                   TString::Format("\nInput data file %s does not contain a "
                                   "valid start event. Possible data "
                                   "corruption. ",
                                   fCurrentFileName.Data()
                                  ).Data();

    CloseFile();

    return kFALSE;
  }

  fCurrentFile++;
  fCurrentEvent = 0;

  return kTRUE;

}
// =============================================================================

// =============================================================================
void CbmHldSource::CloseFile()
{
  fFileStream->close();
}
// =============================================================================

// =============================================================================
void CbmHldSource::AddFile(const TString& tFileName)
{
  FileStat_t tFileStat;

  if(gSystem->GetPathInfo(tFileName.Data(), tFileStat))
  {
    LOG(fatal) <<
                   TString::Format("\nInput data file %s does not exist.",
                                   tFileName.Data()
                                  ).Data();
  }

  TObjString* tObjString = new TObjString(tFileName);
  fFileNames->Add(tObjString);
}
// =============================================================================

// =============================================================================
void CbmHldSource::AddPath(const TString& tFileDirectory,
                           const TString& tFileNameWildCard)
{

  FileStat_t tFileStat;
  if(1 == gSystem->GetPathInfo(tFileDirectory.Data(), tFileStat))
  {
    LOG(fatal) <<     
                   TString::Format("\nInput data file directory %s does not "
                                   "exist.",
                                   tFileDirectory.Data()
                                  ).Data();
  }

  TRegexp* tRegexp = new TRegexp(tFileNameWildCard.Data(), kTRUE);

  TSystemDirectory* tSystemDirectory = 
         new TSystemDirectory("dir", tFileDirectory.Data());

  TString tDirectoryName(tFileDirectory);

  if(!tDirectoryName.EndsWith("/"))
  {
      tDirectoryName += "/";
  }

  TList* tList = tSystemDirectory->GetListOfFiles();
  tList->Sort();

  TIterator* tIter = tList->MakeIterator();
  TSystemFile* tSystemFile;

  TString tFileName;

  while(NULL != (tSystemFile = (TSystemFile*)tIter->Next()))
  {
    tFileName = tSystemFile->GetName();

    if(tFileName.Contains(*tRegexp))
    {
      tFileName = tDirectoryName + tFileName;

      AddFile(tFileName);
    }
  }

  tList->Delete();
}
// =============================================================================

ClassImp(CbmHldSource)
