/* Copyright (C) 2021 Justus Liebig Universität Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Adrian Weber [commiter]*/


/**
 * @file CbmRichUnpackMonitor.h
 * @author Adrian Weber (adrian.a.weber@exp2.physik.uni-giessen.de)
 * @brief Monitoring historgrams class for Rich unpacker
 * @version 0.1
 * @date 2022-02-14
 *
 * @copyright Copyright (c) 2022
 *
 * This is the monitoring class for Rich unpacker
 *
*/

#ifndef CbmRichUnpackMonitor_H
#define CbmRichUnpackMonitor_H

#include "CbmMcbm2018RichPar.h"

#include "Rtypes.h"
#include "TH1.h"
#include "TH2.h"

#include <cstdint>

class CbmRichUnpackMonitor {
public:
  CbmRichUnpackMonitor();

  virtual ~CbmRichUnpackMonitor();

  //Variables for debugging info
  std::vector<uint32_t> vNbMessType;
  std::string sMessPatt = "";
  bool bError           = false;

  void ResetDebugInfo();
  void PrintDebugInfo(const uint64_t MsStartTime, const size_t NrProcessedTs, const uint16_t msDescriptorFlags,
                      const uint32_t uSize);

  /** @brief Init all required parameter informations and histograms */
  Bool_t Init(CbmMcbm2018RichPar* parset);

  Bool_t CreateHistograms(CbmMcbm2018RichPar* pUnpackPar);
  Bool_t ResetHistograms();

  Bool_t CreateDebugHistograms(CbmMcbm2018RichPar* pUnpackPar);
  Bool_t ResetDebugHistograms();


  /** @brief Write all histograms to file */
  void Finish();

  void SetHistoFileName(TString nameIn) { fHistoFileName = nameIn; }

  void AddHistoToVector(TNamed* pointer, std::string sFolder = "")
  {
    fvpAllHistoPointers.push_back(std::pair<TNamed*, std::string>(pointer, sFolder));
  }
  std::vector<std::pair<TNamed*, std::string>> GetHistoVector() { return fvpAllHistoPointers; }


  ///Fill general histograms
  void FillVectorSize(ULong64_t TsIdx, UInt_t Size) { fhVectorSize->Fill(TsIdx, Size); }
  void FillVectorCapacity(ULong64_t TsIdx, UInt_t Capacity) { fhVectorCapacity->Fill(TsIdx, Capacity); }

  void FillDigisTimeInRun(Double_t Time) { fhDigisTimeInRun->Fill(Time * 1e-9); }

  void FillDigisToT(Double_t ToT) { fhDigisToT->Fill(ToT); }

  void FillPerTimesliceCountersHistos(double_t dTsStartTime);

  /** @brief Activate the debug mode */
  bool GetDebugMode() { return fDebugMode; }

  /** @brief Activate the debug mode */
  void SetDebugMode(bool value) { fDebugMode = value; }

private:
  TString fHistoFileName = "HistosUnpackerRich.root";


  double_t dFirstTsStartTime = 0;

  ///General histograms
  TH1* fhDigisTimeInRun = nullptr;
  TH1* fhDigisToT       = nullptr;
  TH1* fhVectorSize     = nullptr;
  TH1* fhVectorCapacity = nullptr;

  /** @brief Flag if debug mode is active or not */
  bool fDebugMode = false;


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

  CbmRichUnpackMonitor(const CbmRichUnpackMonitor&);
  CbmRichUnpackMonitor operator=(const CbmRichUnpackMonitor&);

  ClassDef(CbmRichUnpackMonitor, 1)
};

#endif
