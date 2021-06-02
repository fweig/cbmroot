/** @file CbmStar2019Algo.h
  * @copyright Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors P.-A. Loizeau, Florian Uhlig [committer] **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                         CbmStar2019Algo                           -----
// -----               Created 08.11.2018 by P.-A. Loizeau                 -----
// -----                Based on CbmStar2019Algo by F.Uhlig                  -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmStar2019Algo_H
#define CbmStar2019Algo_H


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

template<typename T>
bool is_this_type(const boost::any& varValue)
{
  if (auto q = boost::any_cast<T>(&varValue)) return true;
  else
    return false;
}


template<typename T>
class CbmStar2019Algo {
public:
  CbmStar2019Algo()
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
    , fParameterMap() {};
  virtual ~CbmStar2019Algo()              = default;
  CbmStar2019Algo(const CbmStar2019Algo&) = delete;
  CbmStar2019Algo& operator=(const CbmStar2019Algo&) = delete;

  virtual Bool_t Init() = 0;
  virtual void Reset()  = 0;
  virtual void Finish() = 0;

  virtual Bool_t ProcessTs(const fles::Timeslice& ts)                                   = 0;
  virtual Bool_t ProcessTs(const fles::Timeslice& ts, size_t component)                 = 0;
  virtual Bool_t ProcessMs(const fles::Timeslice& ts, size_t uMsCompIdx, size_t uMsIdx) = 0;

  virtual Bool_t InitContainers()   = 0;
  virtual Bool_t ReInitContainers() = 0;
  virtual TList* GetParList()       = 0;
  virtual void SetParameter(std::string /*param*/) { ; }
  virtual std::string GetParameter(std::string /*param*/) { return std::string {""}; }

  /// For monitor algos
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

  /// For unpacker algos
  void ClearVector() { fDigiVect.clear(); }
  std::vector<T>& GetVector() { return fDigiVect; }
  void ClearErrorVector() { fErrVect.clear(); }
  std::vector<CbmErrorMessage>& GetErrorVector() { return fErrVect; }

  /// Control flags
  void SetIgnoreOverlapMs(Bool_t bFlagIn = kTRUE) { fbIgnoreOverlapMs = bFlagIn; }

protected:
  /// Parameter management
  TList* fParCList;

  /// Parameters related to FLES containers
  std::vector<size_t> fvMsComponentsList;  //!
  size_t fuNbCoreMsPerTs;                  //!
  size_t fuNbOverMsPerTs;                  //!
  size_t fuNbMsLoop;                       //!
  Bool_t fbIgnoreOverlapMs;                //! /** Ignore Overlap Ms: all fuOverlapMsNb MS at the end of timeslice **/
  Double_t fdMsSizeInNs;                   //! Size of a single MS, [nanoseconds]
  Double_t fdTsCoreSizeInNs;               //! Total size of the core MS in a TS, [nanoseconds]
  Double_t fdTsFullSizeInNs;               //! Total size of the core MS in a TS, [nanoseconds]

  /// For monitor algos
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

  /// For unpacker algos
  std::vector<T> fDigiVect;
  std::vector<CbmErrorMessage> fErrVect = {};

  /// For any algo
  std::map<std::string, std::string> fParameterMap;  //! Map of parameter name and type

  Bool_t CheckParameterValidity(std::string /*parameterName*/, std::string /*parameterType*/) { return kTRUE; }

private:
};

#endif
