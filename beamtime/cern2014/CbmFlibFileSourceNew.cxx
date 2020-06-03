// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmFlibFileSourceNew                        -----
// -----                    Created 01.11.2013 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmFlibFileSourceNew.h"


#include "TimesliceInputArchive.hpp"
#include "Timeslice.hpp"
#include "TimesliceSubscriber.hpp"
#include "MicrosliceContents.hpp"

#include "FairLogger.h"

#include <iostream>
#include <fstream>

CbmFlibFileSourceNew::CbmFlibFileSourceNew()
  : FairSource(),
    fFileName(""),
    fInputFileList(new TObjString()),
    fFileCounter(0),
    fHost("localhost"),
    fPort(5556),
    fUnpackers(),
    fTSNumber(0),
    fTSCounter(0),
    fTimer(),
    fSource(NULL)
{
}

CbmFlibFileSourceNew::CbmFlibFileSourceNew(const CbmFlibFileSourceNew& source)
  : FairSource(source),
    fFileName(""),
    fInputFileList(),
    fFileCounter(0),
    fHost("localhost"),
    fPort(5556),
    fUnpackers(),
    fTSNumber(0),
    fTSCounter(0),
    fTimer(),
    fSource(NULL)
{
}

CbmFlibFileSourceNew::~CbmFlibFileSourceNew()
{
}

Bool_t CbmFlibFileSourceNew::Init()
{
  if ( 0 == fFileName.Length() ) {
    TString connector = Form("tcp://%s:%i", fHost.Data(), fPort);
    LOG(info) << "Open TSPublisher at " << connector;
    fInputFileList.Add(new TObjString(connector));
    fSource = new fles::TimesliceSubscriber(connector.Data());
    if ( !fSource) {
      LOG(fatal) << "Could not connect to publisher.";
    }
  } else {
    // --- Open input file
    TObjString* tmp =
      dynamic_cast<TObjString*>(fInputFileList.At(fFileCounter));
    fFileName = tmp->GetString();

    LOG(info) << "Open the Flib input file " << fFileName;
    // Check if the input file exist
    FILE* inputFile = fopen(fFileName.Data(), "r");
    if ( ! inputFile )  {
      LOG(fatal) << "Input file " << fFileName << " doesn't exist.";
    }
    fclose(inputFile);
    fSource = new fles::TimesliceInputArchive(fFileName.Data());
    if ( !fSource) {
      LOG(fatal) << "Could not open input file.";
    }
  }

  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    LOG(info) << "Initialize " << it->second->GetName() <<
      " for systemID 0x" << std::hex << it->first << std::dec;
    it->second->Init();
    //    it->second->Register();
  }

  return kTRUE;
}

Int_t CbmFlibFileSourceNew::ReadEvent(UInt_t)
{

  //  fTimer.Start();
  //while( fFileCounter < fInputFileList.GetSize() ) {
    while (auto timeslice = fSource->get()) {
      fTSCounter++;
      if ( 0 == fTSCounter%10000 ) {
	/*
        fTimer.Stop();
	Double_t rtime = fTimer.RealTime();
	Double_t ctime = fTimer.CpuTime();
	*/
	LOG(info) << "Analyse Event " << fTSCounter;
	/*
        LOG(info) << "Real time/100 timeslices: " << rtime << " s";
        LOG(info) << "CPU time/100 timeslices: " << ctime << " s";
	fTimer<.Start();
	*/
      }
      const fles::Timeslice& ts = *timeslice;
      auto tsIndex = ts.index();
      if( (tsIndex != (fTSNumber+1)) &&( fTSNumber != 0) ) {
	LOG(warning) << "Missed Timeslices. Old TS Number was " << fTSNumber
		     << " New TS Number is " << tsIndex;
      }
      fTSNumber=tsIndex;
      for (size_t c {0}; c < ts.num_components(); c++) {
	auto systemID = ts.descriptor(c, 0).sys_id;

	if(gLogger->IsLogNeeded( fair::Severity::debug )) {
	  PrintMicroSliceDescriptor(ts.descriptor(c, 0));
	}

	auto it=fUnpackers.find(systemID);
	if (it == fUnpackers.end()) {
	  //LOG(fatal) << "Could not find unpacker for system id 0x" <<
	  //std::hex << systemID << std::dec;
	} else {
	  it->second->DoUnpack(ts, c);
	}
      }
      return 0;
    }
    /*
    // Check if there is another file in the list
    fFileCounter += 1;

    if ( fInputFileList.GetSize() > fFileCounter ) {
      delete fSource;
      TObjString* tmp =
	dynamic_cast<TObjString*>(fInputFileList.At(fFileCounter));
      fFileName = tmp->GetString();
      LOG(info) << "Open the Flib input file " << fFileName;
      // Check if the input file exist
      FILE* inputFile = fopen(fFileName.Data(), "r");
      if ( ! inputFile )  {
        LOG(fatal) << "Input file " << fFileName << " doesn't exist.";
      }
      fclose(inputFile);
      fSource = new fles::TimesliceInputArchive(fFileName.Data());
    }
    */
    // }
  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    LOG(info) << "Finish " << it->second->GetName() <<
      " for systemID 0x" << std::hex << it->first << std::dec;
    it->second->Finish();
    //    it->second->Register();
  }
  return 1; // no more data; trigger end of run
}

void CbmFlibFileSourceNew::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
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

Bool_t CbmFlibFileSourceNew::CheckTimeslice(const fles::Timeslice& ts)
{
    if ( 0 == ts.num_components() ) {
      LOG(error) << "No Component in TS " << ts.index();
      return 1;
    }
    LOG(info) << "Found " << ts.num_components()
	      << " different components in tiemeslice";
    return kTRUE;
}

void CbmFlibFileSourceNew::Close()
{
}

void CbmFlibFileSourceNew::Reset()
{
  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    it->second->Reset();
  }
}

ClassImp(CbmFlibFileSourceNew)
