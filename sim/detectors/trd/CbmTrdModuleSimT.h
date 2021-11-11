/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci, Florian Uhlig [committer] */

#ifndef CBMTRDMODULESIMT_H
#define CBMTRDMODULESIMT_H

#include "CbmTrdModuleSim.h"

class CbmTimeSlice;
class CbmTrdFASP;
class CbmTrdTrianglePRF;
class CbmTrdParSetAsic;
/** @class CbmTrdModuleSimT
 ** @brief Simulation module implementation for TRD-2D physics and FEE
 ** @author Alex Bercuci <abercuci@niham.nipne.ro>  
 ** @since 01.02.2019 
 ** @date 10.10.2021 
 **
 ** The class is steered via CbmTrdDigitizer by looping over all MC points 
 ** generated during track propagation. The class can be used to digitize MC 
 ** output but also simulate laboratory set-ups (\sa SetLabMeasurement()) like 
 ** 55Fe (\sa SetFeCalib()) or X-rays (\sa SetFeCalib(kFALSE))   
 **/
class CbmTrdModuleSimT : public CbmTrdModuleSim {
public:
  enum ECbmTrdModuleSimT
  {
    kMeasurement = 0  ///< default simulate setup interactions, if set simulate laboratory measurement, see kLab
      ,
    kLab  ///< default simulate 55Fe, if set X-rays, see X-rays spectrum generator
      ,
    kFEE  ///< FEE simulator. Default FASP otherwise SPADIC
  };
  CbmTrdModuleSimT(Int_t mod, Int_t ly, Int_t rot, Bool_t FASP = kTRUE);
  virtual ~CbmTrdModuleSimT();

  Bool_t IsLabMeasurement() const { return TESTBIT(fConfig, kMeasurement); }
  Bool_t IsFeCalib() const { return TESTBIT(fConfig, kLab); }
  /**
   * \brief Flush local buffer of digits which can no longer interact with current event
   * \param time current event time or 0 for all
   * \author A.Bercuci <abercuci@niham.nipne.ro>
   **/
  Int_t FlushBuffer(ULong64_t time = 0);

  /**
   * \brief Steer building of digits for triangular pad geometry
   * \param[in] p MC point to be digitized 
   * \param[in] time Event time 
   * \param[in] TR Request TR generation on top of dEdx  
   * \sa ScanPadPlane()
   * \author A.Bercuci <abercuci@niham.nipne.ro>
   **/
  Bool_t MakeDigi(CbmTrdPoint* p, Double_t time, Bool_t TR);
  Bool_t MakeRaw(/*CbmTrdPoint *p*/) { return kTRUE; }
  void SetGamma(Double_t /*gamma*/) { ; }
  void SetMessageConverter(CbmTrdRawToDigiR* conv = NULL) { (void) conv; }
  void SetQA(CbmTrdCheckUtil* qa = NULL) { (void) qa; }
  void SetLabMeasurement(Bool_t set = kTRUE)
  {
    set ? SETBIT(fConfig, kMeasurement) : CLRBIT(fConfig, kMeasurement);
    SetFeCalib(set);
  }
  void SetFeCalib(Bool_t set = kTRUE) { set ? SETBIT(fConfig, kLab) : CLRBIT(fConfig, kLab); }

  /**
   * \brief Set the FEE type operating on the chamber
   * \param[in] set default use FASP/GETS via CbmTrdFASP class. If set to false use SPADIC TODO
   **/
  void SetFasp(Bool_t set = kTRUE) { set ? SETBIT(fConfig, kFEE) : CLRBIT(fConfig, kFEE); }
  void SetAsicPar(CbmTrdParSetAsic* p = NULL);
  Bool_t UseFasp() const { return TESTBIT(fConfig, kFEE); }

private:
  CbmTrdModuleSimT(const CbmTrdModuleSimT& ref);
  const CbmTrdModuleSimT& operator=(const CbmTrdModuleSimT& ref);

  /**
   * \brief Build digits for the triangular pad geometry
   * \param point Position of hit on the anode wire in c.s.
   * \param dx    Track projection length on the closest anode wire [cm]
   * \param E     Energy loss from either ionization or X [keV]
   * \sa CbmTrdTriangle CbmTrdRadiator AddDigi()
   * \author A.Bercuci <abercuci@niham.nipne.ro>
   **/
  Bool_t ScanPadPlane(const Double_t* point, Double_t dx, Double_t E, Double_t tdrift);
  /**
   * \brief Adding triangular digits to time slice buffer
   * \param pointId The TRD hit in global coordinates beeing processed
   * \param address column/row unique index
   * \param charge Energy deposit in ADC chs for tilt [0] and rectangular [1] coupled pads
   * \param time   time of the CS for 80MHz clocks
   * \param fTR    TR fraction of total energy
   * \sa FlushBuffer()
   * \author A.Bercuci <abercuci@niham.nipne.ro>
   **/
  void AddDigi(Int_t address, Double_t* charge, Double_t time);
  /**
   * \brief Print current buffer content
   * \author A.Bercuci <abercuci@niham.nipne.ro>
   **/
  void DumpBuffer() const;

  UChar_t fConfig;                      ///< bit map for configuration. See class documentation
  CbmTrdTrianglePRF* fTriangleBinning;  ///< Integration of PRF on triangular pad-plane geometry
  CbmTrdFASP* fFASP;                    ///< FASP simulator
  CbmTimeSlice* fTimeSlice;             ///< link to CBM time slice
  ULong64_t fTimeOld;                   ///< time [ns] of the last event processed (check CbmDaq)

  ClassDef(CbmTrdModuleSimT, 1)
};

#endif
