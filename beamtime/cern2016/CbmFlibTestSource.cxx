// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmFlibTestSource                        -----
// -----                    Created 20.06.2016 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmFlibTestSource.h"

#include "CbmTbDaqBuffer.h"

#include "TimesliceInputArchive.hpp"
#include "Timeslice.hpp"
#include "TimesliceSubscriber.hpp"
#include "MicrosliceContents.hpp"

#include "FairLogger.h"

#include "TSystem.h"
#include "TList.h"
#include "TObjString.h"
#include "TRegexp.h"
#include "TSystemDirectory.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "CbmTofDigi.h"

std::vector<CbmDigi*> vdigi;

CbmFlibTestSource::CbmFlibTestSource()
  : FairSource(),
    fFileName(""),
    fInputFileList(new TObjString()),
    fFileCounter(0),
    fHost("localhost"),
    fPort(5556),
    fUnpackers(),
    fDetectorSystemMap(),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fTSNumber(0),
    fTSCounter(0),
    fiReqDigiAddr(),
    fiReqMode(0),
    fdMaxDeltaT(500.),
    fTimer(),
    fBufferFillNeeded(kTRUE),
    fSource(NULL)
{
}

CbmFlibTestSource::CbmFlibTestSource(const CbmFlibTestSource& source)
  : FairSource(source),
    fFileName(""),
    fInputFileList(),
    fFileCounter(0),
    fHost("localhost"),
    fPort(5556),
    fUnpackers(),
    fDetectorSystemMap(),
    fBuffer(CbmTbDaqBuffer::Instance()),
    fTSNumber(0),
    fTSCounter(0),
    fiReqDigiAddr(),
    fiReqMode(0),
    fdMaxDeltaT(100.),
    fTimer(),
    fBufferFillNeeded(kTRUE),
    fSource(NULL)
{
}

CbmFlibTestSource::~CbmFlibTestSource()
{
}

Bool_t CbmFlibTestSource::Init()
{
  Int_t fNFiles = fInputFileList.GetEntries();
  if ( fiReqDigiAddr.size()>0 )
  for(UInt_t i=0; i<fiReqDigiAddr.size(); i++)
  LOG(info) << Form("Look for 0x%08x digis in %d input files",fiReqDigiAddr[i],fNFiles);
  if ( 0 == fNFiles ) { //fFileName.Length() ) {
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

void CbmFlibTestSource::AddPath(const TString& tFileDirectory,
                           const TString& tFileNameWildCard)
{

  FileStat_t tFileStat;
  if(1 == gSystem->GetPathInfo(tFileDirectory.Data(), tFileStat))
  {
    LOG(fatal) <<
                   TString::Format("\nInput data file directory %s does not "
                                   "exist.",
                                   tFileDirectory.Data()
                                  ).Data()
                  ;
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

  for (Int_t iRun=0; iRun< 999; iRun++) {

    Char_t* cRun = Form("_%04d.t",iRun);
    TIterator* tIter = tList->MakeIterator();
    TSystemFile* tSystemFile;
    TString tFileName;


    while(NULL != (tSystemFile = (TSystemFile*)tIter->Next()))
    {
      tFileName = tSystemFile->GetName();

      //LOG(info)<<" Check file  "<<tFileName.Data()<<" for "<<cRun;

      if( tFileName.Contains(*tRegexp) && tFileName.Contains(cRun) )
      {
	tFileName = tDirectoryName + tFileName;

	LOG(info)<<" Add file to input "<<tFileName.Data();

	AddFile(tFileName);
	break;
      }
    }
  }
  tList->Delete();
  if(0==fInputFileList.GetEntries())
  LOG(fatal)<<" did not find any valid input file at "<<tFileDirectory.Data();
}

void CbmFlibTestSource::SetParUnpackers()
{
	for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
		LOG(info) << "Set parameter container " << it->second->GetName() <<
				" for systemID 0x" << std::hex << it->first << std::dec;
	    it->second->SetParContainers();
	  }

}

Bool_t CbmFlibTestSource::InitUnpackers()
{
	Bool_t result = kTRUE;
	for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
		LOG(info) << "Initialize parameter container " << it->second->GetName() <<
				" for systemID 0x" << std::hex << it->first << std::dec;
	    result = result && it->second->InitContainers();
	}
    return result;
}

Bool_t CbmFlibTestSource::ReInitUnpackers()
{
	Bool_t result = kTRUE;
	for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
		LOG(info) << "Initialize parameter container " << it->second->GetName() <<
				" for systemID 0x" << std::hex << it->first << std::dec;
	    result = result && it->second->ReInitContainers();
	}
    return result;
}

Int_t CbmFlibTestSource::ReadEvent(UInt_t iEv)
{

  LOG(debug) << "Request received for "<<iEv<<". event ";


  while ( 1 ==  GetNextEvent()){
      if (fBufferFillNeeded) {
	Int_t iRet = FillBuffer();
	if (iRet>0) break;  // no more data
      }
  }

  if ( fSource->eos() && fFileCounter < fInputFileList.GetEntries()){
    fFileCounter++;
    TObjString* tmp =
      dynamic_cast<TObjString*>(fInputFileList.At(fFileCounter));
    if(NULL != tmp) {
      fFileName = tmp->GetString();
      LOG(info) << "Open the "<<fFileCounter<<". ("<<fInputFileList.GetEntries()
		<<") Flib input file " << fFileName;
      delete(fSource);
      fSource = new fles::TimesliceInputArchive(fFileName.Data());
      if ( !fSource) {
	LOG(fatal) << "Could not open input file.";
      }
    }
  }
  return fSource->eos(); // no more data; trigger end of run
}

void CbmFlibTestSource::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
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

Bool_t CbmFlibTestSource::CheckTimeslice(const fles::Timeslice& ts)
{
    if ( 0 == ts.num_components() ) {
      LOG(error) << "No Component in TS " << ts.index();
      return 1;
    }
    LOG(info) << "Found " << ts.num_components()
              << " different components in timeslice";
    return kTRUE;
}

void CbmFlibTestSource::Close()
{
  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    LOG(info) << "Finish " << it->second->GetName() << " for systemID 0x"
              << std::hex << it->first << std::dec;
    it->second->Finish();
  }

}

void CbmFlibTestSource::Reset()
{
  for (auto it=fUnpackers.begin(); it!=fUnpackers.end(); ++it) {
    it->second->Reset();
  }
}

Int_t CbmFlibTestSource::FillBuffer()
{

    while (auto timeslice = fSource->get()) {
      fTSCounter++;
      if ( 0 == fTSCounter%10000 ) {
        LOG(info) << "Processing TimeSlice " << fTSCounter;
      }

      const fles::Timeslice& ts = *timeslice;
      auto tsIndex = ts.index();
      if( (tsIndex != (fTSNumber+1)) &&( fTSNumber != 0) ) {
        LOG(warning) << "Missed Timeslices. Old TS Number was " << fTSNumber
                     << " New TS Number is " << tsIndex;
      }
      fTSNumber=tsIndex;

      if ( 0 ==fTSNumber%10000 ) {
        LOG(info) << "Reading Timeslice " << fTSNumber;
      }

      for (size_t c {0}; c < ts.num_components(); c++) {
        auto systemID = static_cast<int>(ts.descriptor(c, 0).sys_id);

        LOG(debug) << "Found systemID: " << std::hex
                   << systemID << std::dec;

        if(gLogger->IsLogNeeded( fair::Severity::debug1 )) {
          PrintMicroSliceDescriptor(ts.descriptor(c, 0));
        }

        auto it=fUnpackers.find(systemID);
        if (it == fUnpackers.end()) {
          LOG(debug) << "Could not find unpacker for system id 0x"
                     << std::hex << systemID << std::dec;
        } else {
          it->second->DoUnpack(ts, c);
        }
      }
      return 0;
    }

    return 1;
}

Int_t CbmFlibTestSource::GetNextEvent()
{
 const Int_t AddrMask=0x0001FFFF;
 UInt_t nDigi=0;
 Bool_t bOut=kFALSE;
 Double_t fTimeBufferOut=0.;
 //Double_t dTLast=0.;
 while (!bOut)
 {
  fTimeBufferOut = fBuffer->GetTimeLast();
  LOG(debug) << "Buffer has " << fBuffer->GetSize() << " entries and contains data from "
            << std::setprecision(9) << std::fixed
            << static_cast<Double_t>(fBuffer->GetTimeFirst()) * 1.e-9 << " to "
            << std::setprecision(9) << std::fixed
            << static_cast<Double_t>(fBuffer->GetTimeLast()) * 1.e-9 << " s";

  if(fTimeBufferOut - fBuffer->GetTimeFirst()<1.E6) {fBufferFillNeeded=kTRUE; return 1;} // store 1 msec of data in memory

  CbmDigi* digi = fBuffer->GetNextData(fTimeBufferOut);
  if (NULL == digi)         {fBufferFillNeeded=kTRUE; return 1;}

  LOG(debug) << "Buffer has " << fBuffer->GetSize() << " entries left for digitime = " <<digi->GetTime();

  Double_t dTEnd = digi->GetTime() + fdMaxDeltaT;

  if(dTEnd > fTimeBufferOut)  {
    LOG(warning) <<Form("Remaining buffer < %f with %d entries is not sufficient for digi ending at %f -> skipped ",
			fTimeBufferOut, fBuffer->GetSize(), dTEnd );
    fBufferFillNeeded=kTRUE; return 1;
  }

  Bool_t bDet[fiReqDigiAddr.size()][2];
  for(UInt_t i=0; i<fiReqDigiAddr.size(); i++) for(Int_t j=0; j<2; j++) bDet[i][j]=kFALSE; //initialize

  nDigi=0;
  while(digi) { // build digi array
    if (nDigi == vdigi.size()) vdigi.resize(nDigi+100);
    vdigi[nDigi++]=digi;
    for(UInt_t i=0; i<fiReqDigiAddr.size(); i++)
      if( (digi->GetAddress() & AddrMask) == fiReqDigiAddr[i]) {
        Int_t j = ((CbmTofDigi *)digi)->GetSide();
	bDet[i][j]=kTRUE;
	if (fiReqDigiAddr[i] == 0x00005006) bDet[i][1]=kTRUE; // diamond with pad readout
      }
    //if(bOut) LOG(info)<<Form("Found 0x%08x, Req 0x%08x ", digi->GetAddress(), fiReqDigiAddr);
    digi = fBuffer->GetNextData(dTEnd);
  }

  LOG(debug) << nDigi << " digis associated to dTEnd = " <<dTEnd<<":";
  for(UInt_t iDigi=0; iDigi<nDigi; iDigi++) LOG(debug)<<Form(" 0x%08x",vdigi[iDigi]->GetAddress());
  LOG(debug)<< "";
  if( fiReqDigiAddr.size() > 1)
    LOG(debug) << "Found Req coinc in event with " <<nDigi << " digis, dTEnd = " <<dTEnd;

  //dTLast = vdigi[nDigi-1]->GetTime();

  if(fiReqDigiAddr.size()==0) bOut=kTRUE;    // output everything
  else {
    if( fiReqMode == 0 ) {           // check for presence of requested detectors
      for(UInt_t i=0; i<fiReqDigiAddr.size(); i++)
	if(bDet[i][0]==kFALSE || bDet[i][1]==kFALSE ) break;
	else if( i == fiReqDigiAddr.size()-1 ) bOut=kTRUE;
    } else {                        // check for presence of any known detector
      Int_t iDetMul=0;
      for(UInt_t i=0; i<fiReqDigiAddr.size(); i++)
	if(bDet[i][0]==kTRUE && bDet[i][1]==kTRUE ) {
	  iDetMul++;
	  if(iDetMul == fiReqMode) {bOut=kTRUE; break;}
	}
    }

  }
  for(UInt_t iDigi=0; iDigi<nDigi; iDigi++){
    digi=vdigi[iDigi];
    Int_t detId = digi->GetSystemId();
    Int_t flibId = fDetectorSystemMap[detId];
    LOG(debug1) << "Digi has system ID " << detId
               << " which maps to FlibId "<< flibId;
    std::map<Int_t, CbmTSUnpack*>::iterator it=fUnpackers.find(flibId);
    if (it == fUnpackers.end()) {
      LOG(error) << "Skipping digi with unknown id "
                 << detId;
      continue;
    } else {
      //nDigi++;
      //LOG(debug) << "Found unpacker " <<  it->second;
      if(bOut)      it->second->FillOutput(digi);
      else          digi->Delete();
    }
    //    digi = fBuffer->GetNextData(dTEnd);
  };
  vdigi.clear();
 }  // end of bOut condition
 LOG(debug) << "Buffer has " << fBuffer->GetSize() << " entries left, "
            << nDigi <<" digis in current event. ";
 //( fBuffer->GetSize() <= 100000 ) ? fBufferFillNeeded=kTRUE : fBufferFillNeeded=kFALSE;
 //((dTLast + 1.E10) >  fTimeBufferOut) ? fBufferFillNeeded=kTRUE : fBufferFillNeeded=kFALSE;  // store 10 sec in memory
 return 0;
}

void CbmFlibTestSource::AddReqDigiAddr(Int_t iAddr)
{
  Int_t iNReq=fiReqDigiAddr.size();
  fiReqDigiAddr.resize(iNReq+1); // hopefully the old entries are preserved ...
  fiReqDigiAddr[iNReq]=iAddr;
}
ClassImp(CbmFlibTestSource)
