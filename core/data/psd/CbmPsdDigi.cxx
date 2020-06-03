/** @file CbmPsdDigi.cxx
 ** @author Nikolay Karpushkin <karpushkin@inr.ru>
 ** @date 09.10.2019
 **
 ** Code for Data class for PSD digital information
 **/

#include "CbmPsdDigi.h"

#include <TString.h>            // for Form, TString

#include <string>               // for basic_string


// --- Default constructor
CbmPsdDigi::CbmPsdDigi()
{
}


// --- Constructor with assignment
CbmPsdDigi::CbmPsdDigi(UInt_t address, Double_t edep, Double_t time)
  : fdEdep(edep),
    fdTime(time),
    fuAddress(address)
{
}


// --- Constructor with module and section number
CbmPsdDigi::CbmPsdDigi(UInt_t moduleId, UInt_t sectionId,
                       Double_t edep, Double_t time)
  : fdEdep(edep),
    fdTime(time),
    fuAddress(0)
{
   fuAddress = CbmPsdAddress::GetAddress( moduleId, sectionId );
}


// --- Copy constructor
CbmPsdDigi::CbmPsdDigi(const CbmPsdDigi& digiIn)
  : fdEdep(digiIn.fdEdep),
    fdTime(digiIn.fdTime),
    fuAddress(digiIn.fuAddress)
{
}


// --- Move constructor
CbmPsdDigi::CbmPsdDigi(CbmPsdDigi&& other)
  : fdEdep(other.fdEdep),
    fdTime(other.fdTime),
    fuAddress(other.fuAddress)
{
}


// --- Destructor
CbmPsdDigi::~CbmPsdDigi() {
}


// --- Assignment operator
CbmPsdDigi& CbmPsdDigi::operator=(const CbmPsdDigi& rhs)
{
  if (this != &rhs) {
    fdEdep = rhs.fdEdep;
    fdTime = rhs.fdTime;
    fuAddress = rhs.fuAddress;
  }
  return *this;
}


// --- Move assignment operator
CbmPsdDigi& CbmPsdDigi::operator=(CbmPsdDigi&& other)
{
  if (this != &other) {
    fdEdep = other.fdEdep;
    fdTime = other.fdTime;
    fuAddress = other.fuAddress;
  }
  return *this;
}


// --- Set address from module and section number
void CbmPsdDigi::SetAddress( UInt_t moduleId, UInt_t sectionId )
{
   fuAddress = CbmPsdAddress::GetAddress( moduleId, sectionId );
}


// --- Info to string
std::string CbmPsdDigi::ToString() const
{
   TString string = Form( "CbmPsdDigi: address = 0x%08X Charge = %f Time = %f",
                          fuAddress, fdEdep, fdTime);
   return string.Data();
}



ClassImp(CbmPsdDigi)
