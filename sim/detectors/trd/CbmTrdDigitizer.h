/* Copyright (C) 2008-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Andrey Lebedev */

/**
 * \file CbmTrdDigitizer.h
 * \author Florian Uhlig <f.uhlig@gsi.de>
 * \date 2008
 * \brief TRD digitizer.
 * Updated 24/04/2013 by Andrey Lebedev <andrey.lebedev@gsi.de>
 * Updated 4/06/2018 by Alex Bercuci <abercuci@niham.nipne.ro>
 **/

#ifndef CBMTRDDIGITIZER_H_
#define CBMTRDDIGITIZER_H_

#include "CbmDefs.h"  // for ECbmModuleId
#include "CbmDigitize.h"
#include "CbmMatch.h"
#include "CbmTrdCheckUtil.h"
#include "CbmTrdDigi.h"
#include "CbmTrdRawToDigiR.h"

#include <map>

class TClonesArray;

class CbmTrdParSetAsic;
class CbmTrdParSetGas;
class CbmTrdParSetDigi;
class CbmTrdParSetGain;
class CbmTrdParSetGeo;
class CbmTrdRadiator;

//class CbmTrdDigi;
//class CbmMatch;

//class CbmTrdGeoHandler;
class CbmTrdModuleSim;

class CbmTrdDigitizer : public CbmDigitize<CbmTrdDigi> {
public:
  enum CbmTrdSimDef
  {
    kTime = 0,        ///< select Time based/Event by event simulations
    kNoise,           ///< switch noise digits generation
    kWeightDistance,  ///< use weighting distance for link generation
    kFASP             ///< use FASP ASIC for triangular pad plane geometry
  };

  /**
  * \brief Constructor.
  * \param[in] radiator TRD radiator to be used in digitization.
  */
  CbmTrdDigitizer(CbmTrdRadiator* radiator = NULL);

  /**
  * \brief Destructor.
  **/
  virtual ~CbmTrdDigitizer();
  static Bool_t AddNoise() { return TESTBIT(fConfig, kNoise); }
  static Bool_t IsTimeBased() { return TESTBIT(fConfig, kTime); }
  static Bool_t UseWeightedDist() { return TESTBIT(fConfig, kWeightDistance); }
  static Bool_t UseFASP() { return TESTBIT(fConfig, kFASP); }


  ECbmModuleId GetSystemId() const { return ECbmModuleId::kTrd; }

  /**
  * \brief Inherited from FairTask.
  */
  virtual InitStatus Init();

  /**
  * \brief Inherited from FairTask.
  */
  virtual void SetParContainers();

  /**
  * \brief Inherited from FairTask.
  */
  virtual void Exec(Option_t* option);

  /**
  * \brief Inherited from FairTask.
  */
  virtual void Finish();

  static void SetAddNoise(Bool_t set = kTRUE) { set ? SETBIT(fConfig, kNoise) : CLRBIT(fConfig, kNoise); }
  static void SetUseFASP(Bool_t set = kTRUE) { set ? SETBIT(fConfig, kFASP) : CLRBIT(fConfig, kFASP); }
  static void SetTimeBased(Bool_t set = kTRUE) { set ? SETBIT(fConfig, kTime) : CLRBIT(fConfig, kTime); }
  static void SetWeightedDist(Bool_t set = kTRUE)
  {
    set ? SETBIT(fConfig, kWeightDistance) : CLRBIT(fConfig, kWeightDistance);
  }

private:
  CbmTrdDigitizer(const CbmTrdDigitizer&);
  CbmTrdDigitizer& operator=(const CbmTrdDigitizer&);

  /**
    * \brief Flush local digi buffers to CbmDaqBuffer.
    */
  void FlushBuffers();
  /**
    * \brief Create module for current MC point.
    * \param[in] detId unique identifier of module.
    * \return pointer to newly created module
    */
  CbmTrdModuleSim* AddModule(Int_t detId);
  /**
    * \brief Recursive reset all private monitoring counters
    */
  void ResetCounters();

  /** @brief Clear data arrays **/
  virtual void ResetArrays();

  static Int_t fConfig;  ///< Configuration map for the digitizer. See CbmTrdSimDef for details

  // event info
  Double_t fLastEventTime;  ///< time of last event [ns]

  // counters
  Int_t fpoints;
  Int_t nofBackwardTracks;

  Float_t fEfficiency;  ///< Digi production efficiency [0..1]

  TClonesArray* fPoints;                ///< Trd MC points
  TClonesArray* fTracks;                ///< MC Track information
  std::vector<CbmTrdDigi>* fDigis;      ///< Output CbmTrdDigi array
  std::vector<CbmMatch>* fDigiMatches;  ///< Output CbmMatch array

  CbmTrdParSetAsic* fAsicPar;  ///< parameter list for ASIC characterization
  CbmTrdParSetGas* fGasPar;    ///< parameter list for HV status
  CbmTrdParSetDigi* fDigiPar;  ///< parameter list for read-out geometry
  CbmTrdParSetGain* fGainPar;  ///< parameter list for keV->ADC gain conversion
  CbmTrdParSetGeo* fGeoPar;    ///< parameter list for geometry definitions
  CbmTrdRadiator* fRadiator;   ///< parametrization of radiator TR yield

  CbmTrdRawToDigiR* fConverter;
  CbmTrdCheckUtil* fQA;

  //CbmTrdGeoHandler *fGeoHandler;///< helper to decript geoManager path into proper module type
  std::map<Int_t, CbmTrdModuleSim*> fModuleMap;  ///< list of modules being processed

  // Temporary storage for digis.
  std::map<Int_t, std::pair<CbmTrdDigi*, CbmMatch*>> fDigiMap;

  ClassDef(CbmTrdDigitizer, 1);
};
#endif  // CBMTRDDIGITIZER_H_
