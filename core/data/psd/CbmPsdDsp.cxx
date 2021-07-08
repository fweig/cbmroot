/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, Volker Friese [committer], Florian Uhlig */

/** @file CbmPsdDsp.cxx
 ** @author Nikolay Karpushkin <karpushkin@inr.ru>
 ** @date 04.07.2021
 **
 ** Code for Data class for PSD digital signal processing (DSP)
 **/

#include "CbmPsdDsp.h"

#include <TBuffer.h>  // for TBuffer
#include <TClass.h>   // for CbmPsdDsp::IsA()
#include <TString.h>  // for Form, TString

#include <string>  // for basic_string

// --- Default constructor
CbmPsdDsp::CbmPsdDsp()
  : fuAddress()
  , fdTime()
  , fdEdep()
  , fuZL()
  , fdAccum()
  , fdAdcTime()

  , fdEdepWfm()
  , fdAmpl()
  , fuMinimum()
  , fuTimeMax()
  , fuWfm()

  , fdFitAmpl()
  , fdFitZL()
  , fdFitEdep()
  , fdFitR2()
  , fdFitTimeMax()
  , fuFitWfm()
{
}

// clang-format off
// --- Constructor with assignment
CbmPsdDsp::CbmPsdDsp(UInt_t address,
                     Double_t time, 
                     Double_t edep, 
                     UInt_t zl, 
                     Double_t accum, 
                     Double_t adc_time,

                     Double_t edep_wfm, 
                     Double_t ampl,
                     UInt_t minimum, 
                     UInt_t time_max, 
                     std::vector<std::uint16_t> wfm,

                     Double_t fit_ampl, 
                     Double_t fit_zl, 
                     Double_t fit_edep, 
                     Double_t fit_r2, 
                     Double_t fit_time_max, 
                     std::vector<std::uint16_t> fit_wfm)

  : fuAddress(address)
  , fdTime(time)
  , fdEdep(edep)
  , fuZL(zl)
  , fdAccum(accum)
  , fdAdcTime(adc_time)

  , fdEdepWfm(edep_wfm)
  , fdAmpl(ampl)
  , fuMinimum(minimum)
  , fuTimeMax(time_max)
  , fuWfm(wfm)

  , fdFitAmpl(fit_ampl)
  , fdFitZL(fit_zl)
  , fdFitEdep(fit_edep)
  , fdFitR2(fit_r2)
  , fdFitTimeMax(fit_time_max)
  , fuFitWfm(fit_wfm)
{
}
// clang-format on

// --- Copy constructor
CbmPsdDsp::CbmPsdDsp(const CbmPsdDsp& other)
  : fuAddress(other.fuAddress)
  , fdTime(other.fdTime)
  , fdEdep(other.fdEdep)
  , fuZL(other.fuZL)
  , fdAccum(other.fdAccum)
  , fdAdcTime(other.fdAdcTime)

  , fdEdepWfm(other.fdEdepWfm)
  , fdAmpl(other.fdAmpl)
  , fuMinimum(other.fuMinimum)
  , fuTimeMax(other.fuTimeMax)
  , fuWfm(other.fuWfm)

  , fdFitAmpl(other.fdFitAmpl)
  , fdFitZL(other.fdFitZL)
  , fdFitEdep(other.fdFitEdep)
  , fdFitR2(other.fdFitR2)
  , fdFitTimeMax(other.fdFitTimeMax)
  , fuFitWfm(other.fuFitWfm)
{
}


// --- Move constructor
CbmPsdDsp::CbmPsdDsp(CbmPsdDsp&& other)
  : fuAddress(other.fuAddress)
  , fdTime(other.fdTime)
  , fdEdep(other.fdEdep)
  , fuZL(other.fuZL)
  , fdAccum(other.fdAccum)
  , fdAdcTime(other.fdAdcTime)

  , fdEdepWfm(other.fdEdepWfm)
  , fdAmpl(other.fdAmpl)
  , fuMinimum(other.fuMinimum)
  , fuTimeMax(other.fuTimeMax)
  , fuWfm(other.fuWfm)

  , fdFitAmpl(other.fdFitAmpl)
  , fdFitZL(other.fdFitZL)
  , fdFitEdep(other.fdFitEdep)
  , fdFitR2(other.fdFitR2)
  , fdFitTimeMax(other.fdFitTimeMax)
  , fuFitWfm(other.fuFitWfm)
{
}


// --- Destructor
CbmPsdDsp::~CbmPsdDsp()
{
  std::vector<uint16_t>().swap(fuWfm);
  std::vector<uint16_t>().swap(fuFitWfm);
}


// --- Assignment operator
CbmPsdDsp& CbmPsdDsp::operator=(const CbmPsdDsp& other)
{
  if (this != &other) {
    fuAddress = other.fuAddress;
    fdTime    = other.fdTime;
    fdEdep    = other.fdEdep;
    fuZL      = other.fuZL;
    fdAccum   = other.fdAccum;
    fdAdcTime = other.fdAdcTime;

    fdEdepWfm = other.fdEdepWfm;
    fdAmpl    = other.fdAmpl;
    fuMinimum = other.fuMinimum;
    fuTimeMax = other.fuTimeMax;
    fuWfm     = other.fuWfm;

    fdFitAmpl    = other.fdFitAmpl;
    fdFitZL      = other.fdFitZL;
    fdFitEdep    = other.fdFitEdep;
    fdFitR2      = other.fdFitR2;
    fdFitTimeMax = other.fdFitTimeMax;
    fuFitWfm     = other.fuFitWfm;
  }
  return *this;
}


// --- Move assignment operator
CbmPsdDsp& CbmPsdDsp::operator=(CbmPsdDsp&& other)
{
  if (this != &other) {
    fuAddress = other.fuAddress;
    fdTime    = other.fdTime;
    fdEdep    = other.fdEdep;
    fuZL      = other.fuZL;
    fdAccum   = other.fdAccum;
    fdAdcTime = other.fdAdcTime;

    fdEdepWfm = other.fdEdepWfm;
    fdAmpl    = other.fdAmpl;
    fuMinimum = other.fuMinimum;
    fuTimeMax = other.fuTimeMax;
    fuWfm     = other.fuWfm;

    fdFitAmpl    = other.fdFitAmpl;
    fdFitZL      = other.fdFitZL;
    fdFitEdep    = other.fdFitEdep;
    fdFitR2      = other.fdFitR2;
    fdFitTimeMax = other.fdFitTimeMax;
    fuFitWfm     = other.fuFitWfm;
  }
  return *this;
}


ClassImp(CbmPsdDsp)
