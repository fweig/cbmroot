/** \file CbmPsdMCbmHit.cxx
 ** \author Nikolay Karpushkin <karpushkin@inr.ru>
 ** \date 11.03.2020
 **/

#include "CbmPsdMCbmHit.h"

#include <TObject.h>  // for TObject

#include <sstream>

using namespace std;

// -----   Default constructor
CbmPsdMCbmHit::CbmPsdMCbmHit()
  : TObject(), fuModuleId(0), fuSectionId(0), fdEdep(-1.), fdTime(-1.) {}


// -----   Constructor with parameters
CbmPsdMCbmHit::CbmPsdMCbmHit(UInt_t ModuleId,
                             UInt_t SectionId,
                             Double_t Edep,
                             Double_t Time)
  : TObject()
  , fuModuleId(ModuleId)
  , fuSectionId(SectionId)
  , fdEdep(Edep)
  , fdTime(Time) {}


// -----   Destructor
CbmPsdMCbmHit::~CbmPsdMCbmHit() {}


// --- String output
string CbmPsdMCbmHit::ToString() const {
  stringstream ss;
  //ss << "PsdHit: address " << GetAddress() << " | time " << GetTime()
  //   << " +- " << GetTimeError();
  return ss.str();
}


ClassImp(CbmPsdMCbmHit)
