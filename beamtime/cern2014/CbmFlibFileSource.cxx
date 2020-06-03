// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmFlibFileSource                        -----
// -----                    Created 01.11.2013 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmFlibFileSource.h"
#include "CbmSpadicRawMessage.h"

#include "TimesliceInputArchive.hpp"
#include "Timeslice.hpp"
#include "TimesliceSubscriber.hpp"
#include "MicrosliceContents.hpp"

// note M. Krieger, 2014-08-15: these includes should not be needed, please test
//#if 0
//#include "Message.hpp"
//#include "message_reader.h"
//#endif

#include "TimesliceReader.hpp"

#include "FairLogger.h"
#include "FairRootManager.h"

#include "TClonesArray.h"

#include <iostream>
#include <fstream>

CbmFlibFileSource::CbmFlibFileSource()
  : FairSource(),
    fFileName(""),
    fHost("localhost"),
    fPort(5556),
    fSpadicRaw(new TClonesArray("CbmSpadicRawMessage", 10)),
    fTSCounter(0),
    fSource(NULL)
{
}



/*
CbmFlibFileSource::CbmFlibFileSource(const CbmFlibFileSource& source)
  : FairSource(source),
    fFileName(""),
    fHost("localhost"),
    fPort(5556),
    fSpadicRaw(NULL),
    fSource(NULL)
{
}
*/

CbmFlibFileSource::~CbmFlibFileSource()
{
}

Bool_t CbmFlibFileSource::Init()
{
  if ( 0 == fFileName.Length() ) {
    TString connector = Form("tcp://%s:%i", fHost.Data(), fPort);
    LOG(info) << "Open TSPublisher at " << connector;
    fSource = new fles::TimesliceSubscriber(connector.Data());
    if ( !fSource) {
      LOG(fatal) << "Could not connect to publisher.";
    }
  } else {
    LOG(info) << "Open the Flib input file " << fFileName;
    // Check if the input file exist
    FILE* inputFile = fopen(fFileName.Data(), "r");
    if ( ! inputFile )	{
      LOG(fatal) << "Input file " << fFileName << " doesn't exist.";
    }
    fclose(inputFile);
    // Open the input file
    fSource = new fles::TimesliceInputArchive(fFileName.Data());
    //fSource.reset(new fles::TimesliceInputArchive(fFileName.Data()));
    if ( !fSource) {
      LOG(fatal) << "Could not open input file.";
    }
  }

  FairRootManager* ioman = FairRootManager::Instance();
  ioman->Register("SpadicRawMessage", "spadic raw data", fSpadicRaw, kTRUE);

  return kTRUE;

}

Int_t CbmFlibFileSource::ReadEvent(UInt_t)
{
  fSpadicRaw->Clear();
  while (auto timeslice = fSource->get()) {
    fTSCounter++;
    if ( 0 == fTSCounter%100 ) {
      LOG(info) << "Analyse Event " << fTSCounter;
    }
    const fles::Timeslice& ts = *timeslice;
    for (size_t c {0}; c < ts.num_components(); c++) {
      auto systemID = ts.descriptor(c, 0).sys_id;

      if(gLogger->IsLogNeeded( fair::Severity::debug )) {
	PrintMicroSliceDescriptor(ts.descriptor(c, 0));
      }
      switch (systemID) {
      case 0xFA:
	LOG(info) << "It is flesnet pattern generator data";
	break;
      case 0xF0:
	LOG(info) << "It is flib pattern generator data";
	break;
      case 0xBC:
	LOG(info) << "It is spadic data with wrong system ID";
        UnpackSpadicCbmNetMessage(ts, c);
	break;
      case 0x40:
	LOG(debug) << "It is spadic data with correct system ID";
        UnpackSpadicCbmNetMessage(ts, c);
	break;
      default:
	LOG(info) << "Not known now";
      }
    }
    return 0;
  }

  return 1; // no more data; trigger end of run
}

void CbmFlibFileSource::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
{
  LOG(debug) << "Header ID: Ox" << std::hex << static_cast<int>(mdsc.hdr_id)
	    << std::dec;
  LOG(debug) << "Header version: Ox" << std::hex << static_cast<int>(mdsc.hdr_ver)
	    << std::dec;
  LOG(debug) << "Equipement ID: " << mdsc.eq_id;
  LOG(debug) << "Flags: " << mdsc.flags;
  LOG(debug) << "Sys ID: Ox" << std::hex << static_cast<int>(mdsc.sys_id)
	    << std::dec;
  LOG(debug) << "Sys version: Ox" << std::hex << static_cast<int>(mdsc.sys_ver)
	    << std::dec;
  LOG(debug) << "Microslice Idx: " << mdsc.idx;
  LOG(debug) << "Checksum: " << mdsc.crc;
  LOG(debug) << "Size: " << mdsc.size;
  LOG(debug) << "Offset: " << mdsc.offset;
}


void CbmFlibFileSource::UnpackSpadicCbmNetMessage(const fles::Timeslice& ts, size_t component)
{
  spadic::TimesliceReader r;
  Int_t counter=0;

  r.add_component(ts, component);

  for (auto addr : r.sources()) {
//    std::cout << "---- reader " << addr << " ----" << std::endl;
    while (auto mp = r.get_message(addr)) {
      if (gLogger->IsLogNeeded( fair::Severity::debug )) {
        print_message(*mp);
      }
      Int_t link = ts.descriptor(component, 0).eq_id;
      Int_t address = addr;
      Int_t channel = mp->channel_id();
      Int_t epoch = -1;
      Int_t time = mp->timestamp();
      Int_t samples = mp->samples().size();
      Int_t* sample_values = new Int_t[samples];
      Int_t counter1=0;
      for (auto x : mp->samples()) {
        sample_values[counter1] = x;
        ++counter1;
      }
      new( (*fSpadicRaw)[counter] )
	CbmSpadicRawMessage(link, address, channel, epoch, time, -1, -1, -1, -1,  -1, -1, samples, sample_values);
      ++counter;
      delete[] sample_values;
    }
  }

}

void CbmFlibFileSource::print_message(const spadic::Message& m)
{
  std::cout << "v: " << (m.is_valid() ? "o" : "x");
  std::cout << " / gid: " << static_cast<int>(m.group_id());
  std::cout << " / chid: " << static_cast<int>(m.channel_id());
  if ( m.is_hit() ) {
    std::cout << " / ts: " << m.timestamp();
    std::cout << " / samples (" << m.samples().size() << "):";
    for (auto x : m.samples()) {
      std::cout << " " << x;
    }
    std::cout << std::endl;
  } else {
    if ( m.is_epoch_marker() ) {
      std::cout << " This is an Epoch Marker" << std::endl;
    } else if ( m.is_epoch_out_of_sync() ) {
      std::cout << " This is an out of sync Epoch Marker" << std::endl;
    } else {
      std::cout << " This is not known" << std::endl;
    }
  }
}




Bool_t CbmFlibFileSource::CheckTimeslice(const fles::Timeslice& ts)
{
    if ( 0 == ts.num_components() ) {
      LOG(error) << "No Component in TS " << ts.index();
      return 1;
    }
    LOG(info) << "Found " << ts.num_components()
	      << " different components in tiemeslice";
    return kTRUE;
}

void CbmFlibFileSource::Close()
{
}

void CbmFlibFileSource::Reset()
{
}


ClassImp(CbmFlibFileSource)
