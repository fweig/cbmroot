/* Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, Pierre-Alain Loizeau [committer] */

/** \file CbmPsdMCbmHit.h
 ** \author Nikolay Karpushkin <karpushkin@inr.ru>
 ** \date 11.03.2020
 **/

/** \class CbmPsdMCbmHit
 ** \brief data class for hit information in PSD
 ** \author Nikolay Karpushkin <karpushkin@inr.ru>
 ** \version 1.0
 **
 ** A hit in the PSD is an energy measurement in a PSD section
 **/


#ifndef CBMPSDMCBMHIT_H
#define CBMPSDMCBMHIT_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t
#include <TObject.h>     // for TObject

#include <string>  // for string

class CbmPsdMCbmHit : public TObject {
public:
  /** Default constructor **/
  CbmPsdMCbmHit();


  /** Constructor with all parameters
     ** @param moduleId    Triggered module index
     ** @param sectionId   Triggered section index
     ** @param energy      Energy deposition
     ** @param time        Hit time [ns]
     **/
  CbmPsdMCbmHit(UInt_t moduleId, UInt_t sectionId, Double_t energy, Double_t time);


  /** Destructor **/
  virtual ~CbmPsdMCbmHit();

  /** Info to string **/
  virtual std::string ToString() const;


  /** Accessors **/
  /** @brief  Index of PSD module
     ** @value  module index []
     **/
  Int_t GetModuleID() const { return fuModuleId; }


  /** @brief  Index of PSD section
     ** @value  section index []
     **/
  Double_t GetSectionID() const { return fuSectionId; }


  /** @brief  Energy deposited in PSD section
     ** @value  Energy [a.u.]
     **/
  Double_t GetEdep() const { return fdEdep; }


  /** @brief  Time of the signal
     ** @value  Time [ns]
     **/
  Int_t GetTime() const { return fdTime; }


  /** Modifiers **/
  void SetModuleID(UInt_t mod) { fuModuleId = mod; }
  void SetSectionID(UInt_t sec) { fuSectionId = sec; }
  void SetEdep(Double_t edep) { fdEdep = edep; }
  void SetTime(Double_t time) { fdTime = time; }

private:
  UInt_t fuModuleId  = 0;    /// Module index
  UInt_t fuSectionId = 0;    /// Section index
  Double_t fdEdep    = 0.;   /// Energy deposition
  Double_t fdTime    = -1.;  /// Time of measurement

  ClassDef(CbmPsdMCbmHit, 1)
};

#endif  //CBMPSDMCBMHIT_H
