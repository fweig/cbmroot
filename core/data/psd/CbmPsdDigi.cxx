/* Copyright (C) 2012-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Nikolay Karpushkin, Volker Friese [committer], Florian Uhlig */

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

// --- Copy constructor
CbmPsdDigi::CbmPsdDigi(const CbmPsdDigi& other) : fuAddress(other.fuAddress), fdTime(other.fdTime), fdEdep(other.fdEdep)
{
}


// --- Move constructor
CbmPsdDigi::CbmPsdDigi(CbmPsdDigi&& other) : fuAddress(other.fuAddress), fdTime(other.fdTime), fdEdep(other.fdEdep) {}



// --- Set address from module and section number
void CbmPsdDigi::SetAddress(UInt_t moduleId, UInt_t sectionId)
{
  fuAddress = CbmPsdAddress::GetAddress(moduleId, sectionId);
}


// --- Info to string
std::string CbmPsdDigi::ToString() const
{
  TString string = Form("CbmPsdDigi: address = 0x%08X Charge = %f Time = %f", fuAddress, fdEdep, fdTime);
  return string.Data();
}


ClassImp(CbmPsdDigi)
