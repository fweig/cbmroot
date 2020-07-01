// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmMcbm2018Source                     -----
// -----                    Created 19.01.2018 by P.-A. Loizeau            -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmMcbm2018Source.h"

#include "CbmMcbmUnpack.h"
#include "TimesliceMetaData.h"

#include "TimesliceMultiInputArchive.hpp"
#include "TimesliceInputArchive.hpp"
#include "Timeslice.hpp"
#include "TimesliceMultiSubscriber.hpp"
#include "TimesliceSubscriber.hpp"
#include "MicrosliceContents.hpp"

#include "FairLogger.h"
#include "FairRunOnline.h"

#include "TH1.h"
#include "THttpServer.h"
#include "TProfile.h"
#include "TClonesArray.h"

#include <iostream>
#include <fstream>
#include <iomanip>

CbmMcbm2018Source::CbmMcbm2018Source()
  : FairSource(),
    fFileName(""),
    fDirName(""),
    fInputFileList(new TObjString()),
    fHost("localhost"),
    fUnpackers(),
    fUnpackersToRun(),
    fTSNumber(0),
    fTSCounter(0),
    fTimer(),
    fHistoMissedTS(nullptr),
    fHistoMissedTSEvo(nullptr),
    fNofTSSinceLastTS(0),
    fuTsReduction(1),
    fSource(nullptr),
    fuSubscriberHwm( 1 ),
    fbWriteOutput( kFALSE ),
    fTimeSliceMetaDataArray( nullptr )
{
}

CbmMcbm2018Source::~CbmMcbm2018Source()
{
}

Bool_t CbmMcbm2018Source::Init()
{
  if ( 0 == fFileName.Length() && 0 == fInputFileList.GetSize()) {
    // Create a ";" separated string with all host/port combinations
    fInputFileList.Add(new TObjString(fHost));
    std::string fileList{""};
    for(const auto&& obj: fInputFileList) {
      std::string fileName = dynamic_cast<TObjString*>(obj)->GetString().Data();
      fileList += fileName;
      fileList += ";";
    }
    fileList.pop_back(); // Remove the last ;
    fSource.reset(new fles::TimesliceMultiSubscriber(fileList, fuSubscriberHwm));

    if ( !fSource) {
      LOG(fatal) << "Could not connect to publisher.";
    }
  } else {
    // Create a ";" separated string with all file names
    std::string fileList{""};
    for(const auto&& obj: fInputFileList) {
      std::string fileName = dynamic_cast<TObjString*>(obj)->GetString().Data();
      fileList += fileName;
      fileList += ";";
    }
    fileList.pop_back(); // Remove the last ;
    LOG(info) << "Input File String: " << fileList;
    if(fDirName.Length() > 0) {
      fSource.reset(new fles::TimesliceMultiInputArchive(fileList, fDirName.Data()));
    } else {
      fSource.reset(new fles::TimesliceMultiInputArchive(fileList));
    }
  }

  /// Build list of unpackers without multiples from unpacker dealing with 2 or more detectors
  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it)
    fUnpackersToRun.insert( it->second );

  for( auto itUnp = fUnpackersToRun.begin(); itUnp != fUnpackersToRun.end(); ++ itUnp ) {
    LOG(info) << "Initialize " << (*itUnp)->GetName();
    (*itUnp)->Init();
  }

  THttpServer* server = FairRunOnline::Instance()->GetHttpServer();

  fHistoMissedTS = new TH1I("Missed_TS", "Missed TS", 2, 0., 2.);
  fHistoMissedTSEvo  = new TProfile("Missed_TS_Evo", "Missed TS evolution; TS Idx []", 100000, 0., 10000000.);

  if (server)
  {
    server->Register("/Fles", fHistoMissedTS);
    server->Register("/Fles", fHistoMissedTSEvo);
  } // if (server)

  /// Prepare output of TS meta-data
  FairRootManager* ioman = FairRootManager::Instance();
  if( NULL == ioman )
  {
    LOG(fatal) << "No FairRootManager instance";
  }
  fTimeSliceMetaDataArray= new TClonesArray("TimesliceMetaData", 10);
  if( NULL == fTimeSliceMetaDataArray )
  {
    LOG(fatal) << "Failed creating the TS meta data TClonesarray ";
  }
  ioman->Register("TimesliceMetaData", "TS Meta Data", fTimeSliceMetaDataArray, fbWriteOutput);

  return kTRUE;
}

void CbmMcbm2018Source::SetParUnpackers()
{
	for( auto itUnp = fUnpackersToRun.begin(); itUnp != fUnpackersToRun.end(); ++ itUnp ) {
		LOG(info) << "Set parameter container " << (*itUnp)->GetName();
	    (*itUnp)->SetParContainers();
	  }

}

Bool_t CbmMcbm2018Source::InitUnpackers()
{
	Bool_t result = kTRUE;
	for( auto itUnp = fUnpackersToRun.begin(); itUnp != fUnpackersToRun.end(); ++ itUnp ) {
		LOG(info) << "Initialize parameter container " << (*itUnp)->GetName();
	    result = result && (*itUnp)->InitContainers();
	}
    return result;
}

Bool_t CbmMcbm2018Source::ReInitUnpackers()
{
	Bool_t result = kTRUE;
	for( auto itUnp = fUnpackersToRun.begin(); itUnp != fUnpackersToRun.end(); ++ itUnp ) {
		LOG(info) << "Initialize parameter container " << (*itUnp)->GetName();
	    result = result && (*itUnp)->ReInitContainers();
	}
    return result;
}

Int_t CbmMcbm2018Source::ReadEvent(UInt_t)
{
  Int_t retVal = FillBuffer();

  if (1 == retVal) {
    LOG(info) << "No more input";
  }

  return retVal; // no more data; trigger end of run
}

void CbmMcbm2018Source::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
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

Bool_t CbmMcbm2018Source::CheckTimeslice(const fles::Timeslice& ts)
{
    if ( 0 == ts.num_components() ) {
      LOG(error) << "No Component in TS " << ts.index();
      return 1;
    }
    LOG(info) << "Found " << ts.num_components()
              << " different components in timeslice";
    return kTRUE;
}

void CbmMcbm2018Source::Close()
{
  for( auto itUnp = fUnpackersToRun.begin(); itUnp != fUnpackersToRun.end(); ++ itUnp )
  {
    LOG(info) << "Finish " << (*itUnp)->GetName();
    (*itUnp)->Finish();
  }
/*
  fHistoMissedTS->Write();
  fHistoMissedTSEvo->Write();
*/
}

void CbmMcbm2018Source::Reset()
{
  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    it->second->Reset();
  }
  fTimeSliceMetaDataArray->Clear();
}

Int_t CbmMcbm2018Source::FillBuffer()
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

    if( 1 == fTSCounter ) {
      for (size_t c {0}; c < ts.num_components(); c++) {
	     auto systemID = static_cast<int>(ts.descriptor(c, 0).sys_id);
        LOG(info) << "Found systemID: " << std::hex
                  << systemID << std::dec;

        /// Get range of all unpackers matching this system ID <= Trick for STS + MUCH
        auto it_list = fUnpackers.equal_range(systemID);
        if ( it_list.first == it_list.second ) {
          LOG(info) << "Could not find unpacker for system id 0x"
                    << std::hex << systemID << std::dec;
        } else { // if( it == fUnpackers.end() )
          for ( auto it = it_list.first; it != it_list.second; ++it ) {
            it->second->AddMsComponentToList( c, systemID );
            it->second->SetNbMsInTs( ts.num_core_microslices(),
				     ts.num_microslices( c ) - ts.num_core_microslices() );
          } // for( auto it = it_list.first; it != it_list.second; ++it )
        } // else of if( it == fUnpackers.end() )
      } // for (size_t c {0}; c < ts.num_components(); c++)
    } // if( 1 == fTSCounter )

    /// Apply TS throttling as set by user (default = 1 => no throttling)
    if ( 0 == tsIndex % fuTsReduction ) {
      for ( auto itUnp = fUnpackersToRun.begin(); itUnp != fUnpackersToRun.end(); ++ itUnp ) {
        (*itUnp)->DoUnpack(ts, 0);
      } // for( auto itUnp = fUnpackersToRun.begin(); itUnp != fUnpackersToRun.end(); ++ itUnp )
    } // if( 0 == tsIndex % fuTsReduction )

    /// Save the TimeSlice meta-data for access by higher level tasks
    /// TODO: do not hard-code the duration here!
    new( (*fTimeSliceMetaDataArray)[ fTimeSliceMetaDataArray->GetEntriesFast() ] )
      TimesliceMetaData( ts.descriptor( 0, 0).idx ) ;

    return 0;
  }
  return 1;
}

ClassImp(CbmMcbm2018Source)
