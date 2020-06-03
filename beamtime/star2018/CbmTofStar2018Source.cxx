// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmTofStar2018Source                     -----
// -----                    Created 19.01.2018 by P.-A. Loizeau            -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTofStar2018Source.h"

//#include "CbmTbDaqBuffer.h"

#include "TimesliceInputArchive.hpp"
#include "Timeslice.hpp"
#include "TimesliceSubscriber.hpp"
#include "MicrosliceContents.hpp"

#include "FairLogger.h"
#include "FairRunOnline.h"

#include "TH1.h"
#include "THttpServer.h"
#include "TProfile.h"

#include <iostream>
#include <fstream>
#include <iomanip>

CbmTofStar2018Source::CbmTofStar2018Source()
  : FairSource(),
    fFileName(""),
    fInputFileList(new TObjString()),
    fFileCounter(0),
    fHost("localhost"),
    fPort(5556),
    fUnpackers(),
    fDetectorSystemMap(),
//    fBuffer(CbmTbDaqBuffer::Instance()),
    fTSNumber(0),
    fTSCounter(0),
    fTimer(),
    fBufferFillNeeded(kTRUE),
    fHistoMissedTS(NULL),
    fHistoMissedTSEvo(NULL),
    fNofTSSinceLastTS(0),
    fuTsReduction(1),
    fSource(NULL)
{
}

CbmTofStar2018Source::CbmTofStar2018Source(const CbmTofStar2018Source& source)
  : FairSource(source),
    fFileName(""),
    fInputFileList(),
    fFileCounter(0),
    fHost("localhost"),
    fPort(5556),
    fUnpackers(),
    fDetectorSystemMap(),
//    fBuffer(CbmTbDaqBuffer::Instance()),
    fTSNumber(0),
    fTSCounter(0),
    fTimer(),
    fBufferFillNeeded(kTRUE),
    fHistoMissedTS(NULL),
    fHistoMissedTSEvo(NULL),
    fNofTSSinceLastTS(0),
    fuTsReduction(1),
    fSource(NULL)
{
}

CbmTofStar2018Source::~CbmTofStar2018Source()
{
}

Bool_t CbmTofStar2018Source::Init()
{
  if ( 0 == fFileName.Length() && 0 == fInputFileList.GetSize()) {
    TString connector = Form("tcp://%s:%i", fHost.Data(), fPort);
    LOG(info) << "Open TSPublisher at " << connector;
    fInputFileList.Add(new TObjString(connector));
    fSource = new fles::TimesliceSubscriber(connector.Data());
    if ( !fSource) {
      LOG(fatal) << "Could not connect to publisher.";
    }
  } else {

    if( kFALSE == OpenNextFile() )
    {
      LOG(error) << "Could not open the first file in the list, Doing nothing!";
      return kFALSE;
    } // if( kFALSE == OpenNextFile() )
  }

  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    LOG(info) << "Initialize " << it->second->GetName() <<
      " for systemID 0x" << std::hex << it->first << std::dec;
    it->second->Init();
    //    it->second->Register();
  }

  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  fHistoMissedTS = new TH1I("Missed_TS", "Missed TS", 2, 0., 2.);
  fHistoMissedTSEvo  = new TProfile("Missed_TS_Evo", "Missed TS evolution; TS Idx []", 100000, 0., 10000000.);

  if (server)
  {
    server->Register("/TofRaw", fHistoMissedTS);
    server->Register("/TofRaw", fHistoMissedTSEvo);
  } // if (server)


  return kTRUE;
}

void CbmTofStar2018Source::SetParUnpackers()
{
	for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
		LOG(info) << "Set parameter container " << it->second->GetName() <<
				" for systemID 0x" << std::hex << it->first << std::dec;
	    it->second->SetParContainers();
	  }

}

Bool_t CbmTofStar2018Source::InitUnpackers()
{
	Bool_t result = kTRUE;
	for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
		LOG(info) << "Initialize parameter container " << it->second->GetName() <<
				" for systemID 0x" << std::hex << it->first << std::dec;
	    result = result && it->second->InitContainers();
	}
    return result;
}

Bool_t CbmTofStar2018Source::ReInitUnpackers()
{
	Bool_t result = kTRUE;
	for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
		LOG(info) << "Initialize parameter container " << it->second->GetName() <<
				" for systemID 0x" << std::hex << it->first << std::dec;
	    result = result && it->second->ReInitContainers();
	}
    return result;
}

Int_t CbmTofStar2018Source::ReadEvent(UInt_t)
{
//  LOG(info) << "Before FillBuffer";
//  Int_t retVal = -1;
  if (fBufferFillNeeded) {
    FillBuffer();
  }
//  LOG(info) << "After FillBuffer";

//  retVal = GetNextEvent();
//  LOG(debug) << "After GetNextEvent: " << retVal;

  Int_t bla = 0;
  // If no more data and file mode, try to read next file in List
  if( fSource->eos() && 0 < fFileName.Length() )
  {
    fFileCounter ++; // Increment file counter to go to next item in List
    bla = ( kFALSE == OpenNextFile() ? 1 : 0 );
  } // if( fSource->eos() && 0 < fFileName.Length() )

//  Int_t bla = fSource->eos(); // no more data; trigger end of run
//  LOG(info) << "After fSource->eos: " << bla;
  return bla; // no more data; trigger end of run
}

void CbmTofStar2018Source::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
{
  LOG(info) << "Header ID: Ox" << std::hex << static_cast<int>(mdsc.hdr_id)
            << std::dec;
  LOG(info) << "Header version: Ox" << std::hex << static_cast<int>(mdsc.hdr_ver)
            << std::dec;
  LOG(info) << "Equipement ID: " << mdsc.eq_id;
  LOG(info) << "Flags: " << mdsc.flags;
  LOG(info) << "Sys ID: Ox" << std::hex << static_cast<int>(mdsc.sys_id)
            << std::dec;
  LOG(info) << "Sys version: Ox" << std::hex << static_cast<int>(mdsc.sys_ver)
            << std::dec;
  LOG(info) << "Microslice Idx: " << mdsc.idx;
  LOG(info) << "Checksum: " << mdsc.crc;
  LOG(info) << "Size: " << mdsc.size;
  LOG(info) << "Offset: " << mdsc.offset;
}

Bool_t CbmTofStar2018Source::CheckTimeslice(const fles::Timeslice& ts)
{
    if ( 0 == ts.num_components() ) {
      LOG(error) << "No Component in TS " << ts.index();
      return 1;
    }
    LOG(info) << "Found " << ts.num_components()
              << " different components in timeslice";
    return kTRUE;
}

void CbmTofStar2018Source::Close()
{
  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    LOG(info) << "Finish " << it->second->GetName() << " for systemID 0x"
              << std::hex << it->first << std::dec;
    it->second->Finish();
  }
  fHistoMissedTS->Write();
  fHistoMissedTSEvo->Write();
}

void CbmTofStar2018Source::Reset()
{
  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    it->second->Reset();
  }
}

Int_t CbmTofStar2018Source::FillBuffer()
{

    while (auto timeslice = fSource->get()) {
      fTSCounter++;
      if ( 0 == fTSCounter%10000 ) {
        LOG(info) << "Analyse Event " << fTSCounter;
      }

      const fles::Timeslice& ts = *timeslice;
      auto tsIndex = ts.index();
      if( (tsIndex != (fTSNumber+1)) &&( fTSNumber != 0) ) {
        LOG(debug) << "Missed Timeslices. Old TS Number was " << fTSNumber
                     << " New TS Number is " << tsIndex;
        fHistoMissedTS->Fill(1, tsIndex-fTSNumber);
        fHistoMissedTSEvo->Fill( tsIndex, 1, tsIndex-fTSNumber);
        fNofTSSinceLastTS=tsIndex-fTSNumber;
      } else {
        fHistoMissedTS->Fill(0);
        fHistoMissedTSEvo->Fill( tsIndex, 0, 1);
        fNofTSSinceLastTS=1;
      }
      fTSNumber=tsIndex;

      if ( 0 ==fTSNumber%1000 ) {
        LOG(info) << "Reading Timeslice " << fTSNumber;
      }

      for (size_t c {0}; c < ts.num_components(); c++) {
        auto systemID = static_cast<int>(ts.descriptor(c, 0).sys_id);

        LOG(debug) << "Found systemID: " << std::hex
                  << systemID << std::dec;
/*
        if(gLogger->IsLogNeeded( fair::Severity::debug1 )) {
          PrintMicroSliceDescriptor(ts.descriptor(c, 0));
        }
*/
        auto it=fUnpackers.find(systemID);
        if (it == fUnpackers.end()) {
          LOG(debug) << "Could not find unpacker for system id 0x"
                     << std::hex << systemID << std::dec;
        } else {
           if( 0 == tsIndex%fuTsReduction ||  systemID != 0x10 )
             it->second->DoUnpack(ts, c);
        }
      }
      return 0;
    }

    return 1;
}

Int_t CbmTofStar2018Source::GetNextEvent()
{
/*
  Double_t fTimeBufferOut = fBuffer->GetTimeLast();
  LOG(debug) << "Timeslice contains data from "
            << std::setprecision(9) << std::fixed
            << static_cast<Double_t>(fBuffer->GetTimeFirst()) * 1.e-9 << " to "
            << std::setprecision(9) << std::fixed
            << static_cast<Double_t>(fBuffer->GetTimeLast()) * 1.e-9 << " s";

  LOG(debug) << "Buffer has " << fBuffer->GetSize() << " entries.";


  CbmDigi* digi = fBuffer->GetNextData(fTimeBufferOut);

//  LOG(info) << "Before if";
  if (NULL == digi) return 1;
//  LOG(info) << "After if";

  while(digi) {
    Int_t detId = digi->GetSystemId();
    Int_t flibId = fDetectorSystemMap[detId];
    LOG(debug) << "Digi has system ID " << detId
              << " which maps to FlibId "<< flibId;
    std::map<Int_t, CbmTSUnpack*>::iterator it=fUnpackers.find(flibId);
    if (it == fUnpackers.end()) {
      LOG(error) << "Skipping digi with unknown id "
                 << detId;
      continue;
    } else {
      it->second->FillOutput(digi);
    }
    digi = fBuffer->GetNextData(fTimeBufferOut);
  };
*/
  return 0;
}

Bool_t CbmTofStar2018Source::OpenNextFile()
{
   // First Close and delete existing source
   if( NULL != fSource )
      delete fSource;

   if( fFileCounter < fInputFileList.GetSize() )
   {
      // --- Open current input file
      TObjString* tmp =
      dynamic_cast<TObjString*>(fInputFileList.At(fFileCounter));
      fFileName = tmp->GetString();

      LOG(info) << "Open the Flib input file " << fFileName;
      // Check if the input file exist
      FILE* inputFile = fopen(fFileName.Data(), "r");
      if ( ! inputFile )  {
         LOG(error) << "Input file " << fFileName << " doesn't exist.";
         return kFALSE;
      }
      fclose(inputFile);
      fSource = new fles::TimesliceInputArchive(fFileName.Data());
      if ( !fSource) {
         LOG(error) << "Could not open input file.";
         return kFALSE;
      }
   } // if( fFileCounter < fInputFileList.GetSize() )
      else
      {
         LOG(info) << "End of files list reached: file counter is " << fFileCounter
                   << " for " << fInputFileList.GetSize() << " entries in the file list.";
         return kFALSE;
      } // else of if( fFileCounter < fInputFileList.GetSize() )

   return kTRUE;
}


ClassImp(CbmTofStar2018Source)
