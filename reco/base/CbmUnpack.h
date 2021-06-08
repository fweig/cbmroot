/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                         CbmUnpack                                 -----
// -----               Created 07.02.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMUNPACK_H
#define CBMUNPACK_H

/// CbmRoot (+externals) headers
#include "CbmErrorMessage.h"

#include "Timeslice.hpp"

/// FairRoot headers

/// Fairsoft (Root, Boost, ...) headers
#include "Rtypes.h"

#include <boost/any.hpp>

/// C/C++ headers
#include <map>
#include <string>
#include <utility>
#include <vector>

class TList;
class TNamed;
class TCanvas;

template<class T>
bool is_this_type(const boost::any& varValue)
{
  if (auto q = boost::any_cast<T>(&varValue)) return true;
  else
    return false;
}


template<class T>
class CbmUnpack {
public:
  CbmUnpack()
    : fParCList(nullptr)
    , fvMsComponentsList()
    , fuNbCoreMsPerTs(0)
    , fuNbOverMsPerTs(0)
    , fuNbMsLoop(0)
    , fbIgnoreOverlapMs(kFALSE)
    , fdMsSizeInNs(-1.0)
    , fdTsCoreSizeInNs(-1.0)
    , fdTsFullSizeInNs(-1.0)
    , fvpAllHistoPointers()
    , fDigiVect()
    , fErrVect()
    , fParameterMap() {};
  virtual ~CbmUnpack()        = default;
  CbmUnpack(const CbmUnpack&) = delete;
  CbmUnpack& operator=(const CbmUnpack&) = delete;

  virtual Bool_t Init() = 0;
  virtual void Reset()  = 0;
  virtual void Finish() = 0;

  virtual Bool_t ProcessTs(const fles::Timeslice& ts) = 0;
  //      virtual Bool_t ProcessTs( const fles::Timeslice& ts, size_t component ) = 0;
  //      virtual Bool_t ProcessMs( const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx ) = 0;

  virtual Bool_t InitContainers()   = 0;
  virtual Bool_t ReInitContainers() = 0;
  virtual TList* GetParList()       = 0;
  virtual void SetParameter(std::string /*param*/) { ; }
  virtual std::string GetParameter(std::string /*param*/) { return std::string {""}; }

  /// For monitoring of internal processes.
  void AddHistoToVector(TNamed* pointer, std::string sFolder = "")
  {
    fvpAllHistoPointers.push_back(std::pair<TNamed*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TNamed*, std::string>> GetHistoVector() { return fvpAllHistoPointers; }
  void AddCanvasToVector(TCanvas* pointer, std::string sFolder = "")
  {
    fvpAllCanvasPointers.push_back(std::pair<TCanvas*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TCanvas*, std::string>> GetCanvasVector() { return fvpAllCanvasPointers; }

  /// Output vector
  void AssignOutputVector(std::vector<T>& rVect) { fDigiVect = rVect; }
  void AssignErrorVector(std::vector<CbmErrorMessage>& rVect) { fErrVect = rVect; }
  //      void ClearVector() {fDigiVect->clear();}
  //      std::vector<T> * GetVector() {return fDigiVect;}

  /// Control flags
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE) { fbIgnoreOverlapMs = bFlagIn; }

protected:
  /// Parameter management
  TList* fParCList = nullptr;

  /// Parameters related to FLES containers
  std::vector<size_t> fvMsComponentsList;  //! List of components used in the TS, updated internaly by the Algos
  size_t fuNbCoreMsPerTs;                  //! Number of Core MS in the TS
  size_t fuNbOverMsPerTs;                  //! Number of Overlap MS at the end of the TS
  size_t fuNbMsLoop;  //! Number of MS for the loop in each MS, updated internaly by the Algos to read OverMS or not
  Bool_t fbIgnoreOverlapMs;   //! Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice
  Double_t fdMsSizeInNs;      //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInNs;  //! Total size of the core MS in a TS, [nanoseconds]
  Double_t fdTsFullSizeInNs;  //! Total size of the core MS in a TS, [nanoseconds]

  /// For monitoring of internal processes.
  /// => Pointers should be filled with TH1*, TH2*, TProfile*, ...
  /// ==> To check if object N is of type T, use "T ObjectPointer = dynamic_cast<T>( fvpAllHistoPointers[N].first );" and check for nullptr
  /// ==> To get back the original class name use "fvpAllHistoPointers[N].first->ClassName()" which returns a const char * (e.g. "TH1I")
  /// ===> Usage example with feeding a THttpServer:
  /// ===> #include "TH2.h"
  /// ===> std::string sClassName = vHistos[ uHisto ].first.ClassName();
  /// ===> if( !strncmp( sClassName, "TH1", 3 ) )
  /// ===>    server->Register( vHistos[ uHisto ].second.data(), dynamic_cast< TH1 * >(vHistos[ uHisto ].first) );
  /// ===> else if( !strncmp( sClassName, "TH2", 3 ) )
  /// ===>    server->Register( vHistos[ uHisto ].second.data(), dynamic_cast< TH2 * >(vHistos[ uHisto ].first) );
  std::vector<std::pair<TNamed*, std::string>>
    fvpAllHistoPointers;  //! Vector of pointers to histograms + optional folder name
  std::vector<std::pair<TCanvas*, std::string>>
    fvpAllCanvasPointers;  //! Vector of pointers to canvases + optional folder name

  /// Output vector
  std::vector<T>& fDigiVect;                    //! Vector of digis FIXME: check that the reference works as expected
  std::vector<CbmErrorMessage>& fErrVect = {};  //! Vector of error messages

  /// For any algo
  std::map<std::string, std::string> fParameterMap;  //! Map of parameter name and type

  Bool_t CheckParameterValidity(std::string /*parameterName*/, std::string /*parameterType*/) { return kTRUE; }

private:
};

#endif  // CBMUNPACK_H
