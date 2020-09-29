/** @file CbmPsdDigi.cxx
 ** @author Nikolay Karpushkin <karpushkin@inr.ru>
 ** @date 09.10.2019
 **
 ** Code for Data class for PSD digital information
 **/

#include "CbmPsdDigi.h"

#include <TBuffer.h>  // for TBuffer
#include <TClass.h>   // for CbmPsdDigi::IsA()
#include <TString.h>  // for Form, TString

#include <string>  // for basic_string

// --- Default constructor
CbmPsdDigi::CbmPsdDigi() {}


// --- Constructor with assignment
CbmPsdDigi::CbmPsdDigi(UInt_t address, Double_t edep, Double_t time)
  : fdEdep(edep), fdTime(time), fuAddress(address) {}


// --- Constructor with module and section number
CbmPsdDigi::CbmPsdDigi(UInt_t moduleId,
                       UInt_t sectionId,
                       Double_t edep,
                       Double_t time)
  : fdEdep(edep), fdTime(time), fuAddress(0) {
  fuAddress = CbmPsdAddress::GetAddress(moduleId, sectionId);
}


// --- Copy constructor
CbmPsdDigi::CbmPsdDigi(const CbmPsdDigi& digiIn)
  : fdEdep(digiIn.fdEdep), fdTime(digiIn.fdTime), fuAddress(digiIn.fuAddress) {}


// --- Move constructor
CbmPsdDigi::CbmPsdDigi(CbmPsdDigi&& other)
  : fdEdep(other.fdEdep), fdTime(other.fdTime), fuAddress(other.fuAddress) {}


// --- Destructor
CbmPsdDigi::~CbmPsdDigi() {}


// --- Assignment operator
CbmPsdDigi& CbmPsdDigi::operator=(const CbmPsdDigi& rhs) {
  if (this != &rhs) {
    fdEdep    = rhs.fdEdep;
    fdTime    = rhs.fdTime;
    fuAddress = rhs.fuAddress;
  }
  return *this;
}


// --- Move assignment operator
CbmPsdDigi& CbmPsdDigi::operator=(CbmPsdDigi&& other) {
  if (this != &other) {
    fdEdep    = other.fdEdep;
    fdTime    = other.fdTime;
    fuAddress = other.fuAddress;
  }
  return *this;
}


// --- Set address from module and section number
void CbmPsdDigi::SetAddress(UInt_t moduleId, UInt_t sectionId) {
  fuAddress = CbmPsdAddress::GetAddress(moduleId, sectionId);
}


// --- Info to string
std::string CbmPsdDigi::ToString() const {
  TString string = Form("CbmPsdDigi: address = 0x%08X Charge = %f Time = %f",
                        fuAddress,
                        fdEdep,
                        fdTime);
  return string.Data();
}

// --- Custom Streamer
void CbmPsdDigi::Streamer(TBuffer& R__b) {
  // Stream an object of class CbmPsdDigi.

  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
    if (R__v) {}
    //      LOG(info) << "Reading CbmPsdDigi version " << R__v;
    if (3 == R__v) {
      R__b >> fdEdep;
      R__b >> fdTime;
      R__b >> fuAddress;
      R__b.CheckByteCount(R__s, R__c, CbmPsdDigi::IsA());
    } else if (4 == R__v) {
      R__b >> ffFitHarmonic1;
      R__b >> ffFitHarmonic2;
      R__b >> ffFitR2;
      R__b >> ffFitEdep;
      R__b >> fdEdep;
      R__b >> fdTime;
      R__b >> fuAddress;
      R__b >> fuAmpl;
      R__b >> fuZL;
      R__b >> fdEdepWfm;
      R__b.CheckByteCount(R__s, R__c, CbmPsdDigi::IsA());
    }
  } else {
    // The function WriteVersion does not return the version number
    // but the position where ti write the next data
    // to get the class version use TClass->GetClassVersion()

    R__c              = R__b.WriteVersion(CbmPsdDigi::IsA(), kTRUE);
    Version_t version = CbmPsdDigi::IsA()->GetClassVersion();

    // We don't intent to write data in old format
    /*
      if ( 3 == version ) {
        R__b << fdEdep;
        R__b << fdTime;
        R__b << fuAddress;
        R__b.SetByteCount(R__c, kTRUE);
      } else if ( 4 == version ) {
      */
    if (4 == version) {
      R__b << ffFitHarmonic1;
      R__b << ffFitHarmonic2;
      R__b << ffFitR2;
      R__b << ffFitEdep;
      R__b << fdEdep;
      R__b << fdTime;
      R__b << fuAddress;
      R__b << fuAmpl;
      R__b << fuZL;
      R__b << fdEdepWfm;
      R__b.SetByteCount(R__c, kTRUE);
    }
  }
}

ClassImp(CbmPsdDigi)
